/*
 * md.c - A driver for controlling modem of RDA
 *
 * Copyright (C) 2017 RDA Microelectronics Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

//#include <plat/rda_hwspinlock.h>

#include "os_api.h"
#include "sci_types.h"
#include "globals.h"
#include "uws6121e_ipc_cfg.h"
#include "mailbox_reg_uws6121e.h"
#include "ipc_drvapi.h"
#include "md_ipc.h"
#include "sio.h"
#include "mmu_drvapi.h"
#include "load_modem.h"
#include "idle.h"
#include "mem_cfg.h"


extern void ipc_bt_assert_notify(void);

static void sysmail_PhySuspendResumeLogin(void);

static struct md_dev g_modem_dev;
static struct md_dev *g_mdev;
static uint32 ipc_initialized;
static uint32 sysmail_irq_status;
static uint32 Last32K_Num;
uint8 *rda_sm_base;
uint8 *sram_sm_base;
uint32 g_sysmail_regs_data[SYSMAIL_SAVE_REG_NUM];
static struct HAL_IP_SUSPEND_RESUME_T g_sysmail_node;

#define ALIGN(x,a) (((x)+(a)-1)&~((a)-1))
#define IPC_SET_MASK(_dev_, _val_) \
            (_dev_)->regs->INTMR0 = _val_
#define IPC_GET_MASK(_dev_) \
            (_dev_)->regs->INTMR0
#define IPC_GET_STAT(_dev_) \
            (_dev_)->regs->INTSR0
#define IPC_CLR_IRQ(_dev_, _val_) \
            (_dev_)->regs->INTCR0 = _val_
#define IPC_SET_CP_IRQ(_dev_, _val_) \
            (_dev_)->regs->INTSTR1 = _val_
#define IPC_SET_PS_UL_CNT_B(_dev_, _val_) \
            (_dev_)->regs->SYSMAIL4 = _val_
#define IPC_SET_PS_UL_CNT_L(_dev_, _val_) \
            (_dev_)->regs->SYSMAIL5 = _val_
#define IPC_SET_PS_DL_CNT_B(_dev_, _val_) \
            (_dev_)->regs->SYSMAIL6 = _val_
#define IPC_SET_PS_DL_CNT_L(_dev_, _val_) \
            (_dev_)->regs->SYSMAIL7 = _val_
#define IPC_SET_MD_VER_OFF(_dev_, _val_) \
            (_dev_)->regs->SYSMAIL8 = _val_
#define IPC_SET_MD_EXEC_OFF(_dev_, _val_) \
            (_dev_)->regs->SYSMAIL9 = _val_
//#define IPC_GET_BT_ASSERT(_dev_) \
//            (_dev_)->regs->SYSMAIL97

#define MD_SYS_HDR_SIZE     sizeof(struct md_sys_hdr)

static uint32 __ch_buf_read_avail(struct ch_buf *buf)
{
	uint32 space;

	SCI_DisableIRQ();
	space = (buf->pctrl->head - buf->pctrl->tail) & buf->fifo_mask;
	SCI_RestoreIRQ();

	return space;
}

static uint32 __ch_buf_write_avail(struct ch_buf *buf)
{
	uint32 space;

	SCI_DisableIRQ();
	space = buf->fifo_mask - ((buf->pctrl->head - buf->pctrl->tail) & \
				buf->fifo_mask);
	SCI_RestoreIRQ();

	return space;
}

static void __ch_buf_read_done(struct ch_buf *buf, uint32 count)
{
	SCI_DisableIRQ();
	buf->pctrl->tail = (buf->pctrl->tail + count) & buf->fifo_mask;
	SCI_RestoreIRQ();
}

static void __ch_buf_write_done(struct ch_buf *buf, uint32 count)
{
	SCI_DisableIRQ();
	buf->pctrl->head = (buf->pctrl->head + count) & buf->fifo_mask;
	SCI_RestoreIRQ();
}

static uint32 __ch_buf_read(struct ch_buf *buf, uint8 *data, uint32 len)
{
    uint32 copy_len, l;
    uint32 avail;

    avail = __ch_buf_read_avail(buf);
    if (len < avail) {
        copy_len = len;
    } else {
        copy_len = avail;
    }

    /* first get the data from tail until the end of buffer */
    if (copy_len < (buf->fifo_size - buf->pctrl->tail)) {
        l = copy_len;
    } else {
        l = buf->fifo_size - buf->pctrl->tail;
    }
	MMU_DmaCacheSync((uint32)(buf->pfifo + buf->pctrl->tail), l, DMABUFFER_FROM_DEVICE);
    memcpy(data, buf->pfifo + buf->pctrl->tail, l);

    /* then get the rest(if any) at the beginning of the buffer */
    if (copy_len > l) {
		MMU_DmaCacheSync((uint32)buf->pfifo, copy_len - l, DMABUFFER_FROM_DEVICE);
        memcpy(data + l, buf->pfifo, copy_len - l);
	}

    /* update tail */
    __schedule_barrier();
    __ch_buf_read_done(buf, copy_len);

    return copy_len;
}

static uint32 __ch_buf_write(struct ch_buf *buf, const uint8 *data, uint32 len)
{
    uint32 copy_len, l;
    uint32 avail;

    struct smd_ch *ch = (struct smd_ch *)buf->ch;
    struct ps_header *hdr;
    uint32 *offset = (uint32 *)data;
	uint32 i = 0, loop_cnt = 0;
    uint32 curr_offset, next_offset;

    avail = __ch_buf_write_avail(buf);
    if (len < avail) {
        copy_len = len;
    } else {
        copy_len = avail;
    }

    /* first put the data starting from head to bufffer end */
    if (copy_len < (buf->fifo_size - buf->pctrl->head)) {
        l = copy_len;
    } else {
        l = buf->fifo_size - buf->pctrl->head;
    }
    memcpy(buf->pfifo + buf->pctrl->head, data, l);
	MMU_DmaCacheSync((uint32)(buf->pfifo + buf->pctrl->head), l, DMABUFFER_TO_DEVICE);

    /* then put the rest(if any) at the beginning of the buffer */
    if (copy_len > l) {
        memcpy(buf->pfifo, data + l, copy_len - l);
		MMU_DmaCacheSync((uint32)buf->pfifo, copy_len - l, DMABUFFER_TO_DEVICE);
	}

	if (ch->id == SMD_CH_PS)
    {
        loop_cnt = copy_len >> 2; 
        for (i = 0; i < loop_cnt; i++)
        {
            curr_offset = offset[i];
        
            do {
                hdr = (struct ps_header *)(rda_sm_base + curr_offset);
                next_offset = hdr->next;

                SCI_TraceLow("ipc: ul off 0x%x, len: %d\n", curr_offset, hdr->len);
                MMU_DmaCacheSync((uint32)hdr, hdr->buf_size, DMABUFFER_TO_DEVICE);
                curr_offset = next_offset;
            } while (next_offset);
        }
    }        

    /* update head */
    __schedule_barrier();
    __ch_buf_write_done(buf, copy_len);

    return copy_len;
}

static int32 __ch_stream_read(struct ch_buf *buf, void *data, uint32 len)
{
	if (!data || len <= 0) {
		return -1;
	}

	return __ch_buf_read(buf, data, len);
}

static int32 __ch_stream_write(struct ch_buf *buf, const void *data,
			uint32 len)
{
	if (!data || len <= 0) {
		return -1;
	}

	return __ch_buf_write(buf, data, len);
}

static int32 __ch_stream_read_avail(struct ch_buf *buf)
{
	int32 space;

	SCI_DisableIRQ();
	space = (buf->pctrl->head - buf->pctrl->tail) & buf->fifo_mask;
	SCI_RestoreIRQ();

	return space;
}

static int32 __ch_stream_write_avail(struct ch_buf *buf)
{
	int32 space;

	SCI_DisableIRQ();
	space = buf->fifo_mask - ((buf->pctrl->head - buf->pctrl->tail) & \
				buf->fifo_mask);
	SCI_RestoreIRQ();

	return space;
}

static int32 __ch_packet_read(struct ch_buf *buf, void *data, uint32 len)
{
	uint32 opt_len;

	if (!data || len <= 0) {
		return -1;
	}

	opt_len = len << 2;

	return __ch_buf_read(buf, data, opt_len);
}

static int32 __ch_packet_write(struct ch_buf *buf, const void *data,
			uint32 len)
{
	uint32 opt_len;

	if (!data || len <= 0) {
		return -1;
	}

	opt_len = len << 2;

	return __ch_buf_write(buf, data, opt_len);
}

static int32 __ch_packet_read_avail(struct ch_buf *buf)
{
	int32 space;
	unsigned long flags;

	SCI_DisableIRQ();
	space = ((buf->pctrl->head - buf->pctrl->tail) & buf->fifo_mask) >> 2;
	SCI_RestoreIRQ();

	return space;
}

static int32 __ch_packet_write_avail(struct ch_buf *buf)
{
	int32 space;
	unsigned long flags;

	SCI_DisableIRQ();
	space = (buf->fifo_size - ((buf->pctrl->head - buf->pctrl->tail) & \
				buf->fifo_mask)) >> 2;
	SCI_RestoreIRQ();

	return space;
}

static uint32 __ch_get_free_buf_cnt(uint32 alloc, uint32 free,
			uint32 mask)
{
	if (alloc > free) {
		return (mask - (alloc - free));
	} else if (free > alloc) {
		return free - alloc - 1;
	} else {
		return mask;
	}
}

static int32 __ch_try_alloc_one_lit_psbuf(struct ch_buf *buf)
{
	struct md_dev *mdev = g_mdev;
	struct ps_header *hdr;
	uint32 alloc, free, mask;
	int32 cnt, offset;

	/* check lit free */
	alloc = buf->pctrl->alloc_net_l;
	free  = buf->pctrl->free_net_l;
	mask  = mdev->ul_lit_ps_buf_cnt - 1;
	//pr_info("ipc: ul lit %u/%u/%u\n", alloc, free, mask);
	cnt = __ch_get_free_buf_cnt(alloc, free, mask);
	if (cnt <= 1) {
		//pr_info("ipc: no lit2 buf, %u-%u-%u\n", alloc, free, mask);

		/* check big free */
		alloc = buf->pctrl->alloc_net;
		free  = buf->pctrl->free_net;
		mask  = mdev->ul_big_ps_buf_cnt - 1;
		//pr_info("ipc: ul big %u/%u/%u\n", alloc, free, mask);
		cnt = __ch_get_free_buf_cnt(alloc, free, mask);
		if (cnt <= 1) {
			//pr_info("ipc: no ul buf, %u-%u-%u\n", alloc, free, mask);
			buf->pctrl->status |= PS_BUF_FULL;
			return -1;
		}

		/* alloc from big free */
		MMU_DmaCacheSync((uint32)(&buf->net_free_buf_b[alloc]), 4, DMABUFFER_FROM_DEVICE);
		offset = buf->net_free_buf_b[alloc++];
		if (alloc == mdev->ul_big_ps_buf_cnt) {
			alloc = 0;
		}
		buf->pctrl->alloc_net = alloc;

		hdr = (struct ps_header *)(mdev->sm_base + offset);
		//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_FROM_DEVICE);
		//if (hdr->flag == BUF_IN_USE) {
			//panic("ul big offset 0x%x, alloc %u, free %u\n",
			//		offset, alloc, free);
		//}
		//hdr->flag = BUF_IN_USE;
		hdr->buf_size = PS_BUF_UL_LEN_B;
		hdr->next = 0;
		//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_TO_DEVICE);
		
		return offset;
	}else {
		/* alloc from lit free */
		MMU_DmaCacheSync((uint32)(&buf->net_free_buf_l[alloc]), 4, DMABUFFER_FROM_DEVICE);
		offset = buf->net_free_buf_l[alloc++];
		if (alloc == mdev->ul_lit_ps_buf_cnt) {
			alloc = 0;
		}
		buf->pctrl->alloc_net_l = alloc;

		hdr = (struct ps_header *)(mdev->sm_base + offset);
		//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_FROM_DEVICE);
		//if (hdr->flag == BUF_IN_USE) {
			//panic("ul big offset 0x%x, alloc %u, free %u\n",
			//		offset, alloc, free);
		//}
		//hdr->flag = BUF_IN_USE;
		hdr->buf_size = PS_BUF_UL_LEN_L;
		hdr->next = 0;
		//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_TO_DEVICE);
		
		return offset;
	}
}

static int32 __ch_alloc_one_big_psbuf(struct ch_buf *buf)
{
	struct md_dev *mdev = g_mdev;
	struct ps_header *hdr;
	uint32 alloc, free, mask;
	int32 cnt, offset;

	/* check big free */
	alloc = buf->pctrl->alloc_net;
	free  = buf->pctrl->free_net;
	mask  = mdev->ul_big_ps_buf_cnt - 1;

	//pr_info("ipc: ul big %u/%u/%u\n", alloc, free, mask);

	cnt = __ch_get_free_buf_cnt(alloc, free, mask);
	if (cnt <= 1) {
		//pr_info("ipc: no big2 buf, %u-%u-%u\n", alloc, free, mask);
		buf->pctrl->status |= PS_BUF_FULL;
		return -1;
	}

	/* alloc from big free */
	MMU_DmaCacheSync((uint32)(&buf->net_free_buf_b[alloc]), 4, DMABUFFER_FROM_DEVICE);
	offset = buf->net_free_buf_b[alloc++];
	if (alloc == mdev->ul_big_ps_buf_cnt) {
		alloc = 0;
	}
	buf->pctrl->alloc_net = alloc;

	hdr = (struct ps_header *)(mdev->sm_base + offset);
	//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_FROM_DEVICE);
	//if (hdr->flag == BUF_IN_USE) {
		//panic("ul big offset 0x%x, alloc %u, free %u\n",
		//		offset, alloc, free);
	//}
	//hdr->flag = BUF_IN_USE;
	hdr->buf_size = PS_BUF_UL_LEN_B;
	hdr->next = 0;
	//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_TO_DEVICE);
	
	return offset;
}

static int32 __ch_packet_alloc_psbuf(struct ch_buf *buf, uint32 size)
{
	if (size > PS_DATA_LEN_L) {
		return __ch_alloc_one_big_psbuf(buf);
	} else {
		return __ch_try_alloc_one_lit_psbuf(buf);
	}
}

static int32 __ch_packet_free_psbuf(struct ch_buf *buf, uint32 offset)
{
	struct md_dev *mdev = g_mdev;
	//struct ps_header *hdr;
	uint32 alloc, free;

    SCI_DisableIRQ();
	if ((offset >= mdev->dl_big_start) && (offset <= mdev->dl_big_end)) {
		//hdr = (struct ps_header *)(rda_sm_base + offset);
		//if (hdr->flag == BUF_IN_USE) {
		//	hdr->flag = BUF_IN_IDLE;
		//} else {
		//	SCI_TraceLow("Free dl big offset 0x%x Error.\n", offset);
			//SCI_ASSERT(0); 
		//}

		alloc = buf->pctrl->alloc_net;
		free = buf->pctrl->free_net;

		buf->net_free_buf_b[free] = offset;
		//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_TO_DEVICE);
		MMU_DmaCacheSync((uint32)(&buf->net_free_buf_b[free]), 4, DMABUFFER_TO_DEVICE);
		free++;
		if (free == mdev->dl_big_ps_buf_cnt) {
			free = 0;
		}
		buf->pctrl->free_net = free;

		SCI_TraceLow("dl big: %u-%u 0x%x\n", alloc, free, offset);
	} else if ((offset >= mdev->dl_lit_start) && (offset <= mdev->dl_lit_end)) {
		//hdr = (struct ps_header *)(rda_sm_base + offset);
		//if (hdr->flag == BUF_IN_USE) {
		//	hdr->flag = BUF_IN_IDLE;
		//} else {
		//	SCI_TraceLow("Free dl lit offset 0x%x Error.\n", offset);
            //SCI_ASSERT(0);
		//}

		alloc = buf->pctrl->alloc_net_l;
		free = buf->pctrl->free_net_l;

		buf->net_free_buf_l[free] = offset;
		//MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_TO_DEVICE);
		MMU_DmaCacheSync((uint32)(&buf->net_free_buf_l[free]), 4, DMABUFFER_TO_DEVICE);
		free++;
		if (free == mdev->dl_lit_ps_buf_cnt) {
			free = 0;
		}
		buf->pctrl->free_net_l = free;

		SCI_TraceLow("dl lit: %u-%u 0x%x\n", alloc, free, offset);
	} else {
		SCI_TraceLow("ipc: error dl offset 0x%x\n", offset);
		//SCI_ASSERT(0);
	}
    SCI_RestoreIRQ();

	buf->pctrl->status &= ~PS_BUF_FULL;

	return 0;
}

static int32 __ch_queue_read(struct ch_buf *buf, void *data, uint32 len)
{
	uint32 opt_len;
	int32 ret;
	unsigned long flags;

	if (!data || len <= 0) {
		return -1;
	}

	opt_len = len * sizeof(struct ipc_cmd);

	/* ipc cmd size is 16 Bytes */
	SCI_DisableIRQ();
	ret = __ch_buf_read(buf, data, opt_len);
	SCI_RestoreIRQ();

	return ret;
}

static int32 __ch_queue_write(struct ch_buf *buf, const void *data,
			uint32 len)
{
	uint32 opt_len;
	int32 ret;
	unsigned long flags;

	if (!data || len <= 0) {
		return -1;
	}

	opt_len = len * sizeof(struct ipc_cmd);

	/* ipc cmd size is 16 Bytes */
	SCI_DisableIRQ();
	ret = __ch_buf_write(buf, data, opt_len);
	SCI_RestoreIRQ();

	return ret;
}

static int32 __ch_queue_read_avail(struct ch_buf *buf)
{
	int32 space;
	unsigned long flags;

	SCI_DisableIRQ();
	space = (((buf->pctrl->head - buf->pctrl->tail) & buf->fifo_mask)) /
			sizeof(struct ipc_cmd);
	SCI_RestoreIRQ();

	return space;
}

static int32 __ch_queue_write_avail(struct ch_buf *buf)
{
	int32 space;
	unsigned long flags;

	SCI_DisableIRQ();
	space = (buf->fifo_mask - ((buf->pctrl->head - buf->pctrl->tail) & \
				   buf->fifo_mask)) / sizeof(struct ipc_cmd);
	SCI_RestoreIRQ();

	return space;
}

static int32 __ch_read_hdr(struct ch_buf *buf, void *data, int len)
{
	void *ptr;
	uint8 *pbuf = data;
	int orig_len = len;
	uint32 n;
	uint32 head = buf->pctrl->head;
	uint32 tail = buf->pctrl->tail;

	while (len > 0) {
		ptr = (void *)(buf->pfifo + tail);

		if (tail <= head) {
			n = head - tail;
		} else {
			n = buf->fifo_size - tail;
		}

		if (n == 0) {
			/* Fifo is empty. */
			break;
		}

		if (n > len) {
			/* There are more than a packet. */
			n = len;
		}

		memcpy(pbuf, ptr, n);

		pbuf += n;
		len -= n;
		tail = (tail + n) & buf->fifo_mask;
	}

	return orig_len - len;
}

static int32 __ch_clear_fifo(struct ch_buf *buf)
{
	unsigned long flags;

	SCI_DisableIRQ();
	buf->pctrl->head = buf->pctrl->tail;
	SCI_RestoreIRQ();

	return 0;
}

static int32 __ch_syscmd_read(struct ch_buf *buf, void *data, uint32 len)
{
	struct md_sys_hdr fhdr = {0};
	struct md_sys_hdr *phdr = NULL;
	char *pbuf = data;
	int size = 0;
	int opt_size;
	int r;

	if (!len && !data) {
		/* Just return a packet's length */
		r = __ch_read_hdr(buf, (void *)&fhdr, sizeof(fhdr));
		if (r < MD_SYS_HDR_SIZE) {
			/*
			 * It might that we would read later data,
			 * but BP didn't trigger interrupt.
			 */
			return -1;
		}

		/* Check if this is a vaild frame. */
		if (fhdr.magic != MD_SYS_MAGIC) {
			return -1;
		}

		return (r + ALIGN(fhdr.ext_len, 4));
	} else if (!data) {
		return -1;
	}

	if (len < MD_SYS_HDR_SIZE) {
		return -1;
	}

	r = __ch_buf_read(buf, pbuf, sizeof(struct md_sys_hdr));
	if (!r) {
		return r;
	}

	phdr = (struct md_sys_hdr *)pbuf;
	if (phdr->magic != MD_SYS_MAGIC) {
		return -1;
	}

	size += r;
	opt_size = ALIGN(phdr->ext_len, 4);

	if (opt_size > 0) {
		if (len - size < opt_size) {
			return -1;
		}

		pbuf += r;
		r = __ch_buf_read(buf, pbuf, opt_size);
		size += r;
	}

	return size;
}

static int32 __ch_syscmd_write_avail(struct ch_buf *buf)
{
	int space = __ch_stream_write_avail(buf);

	return (space > MD_SYS_HDR_SIZE ? (space - MD_SYS_HDR_SIZE) : 0);
}

static int32 __ch_syscmd_write(struct ch_buf *buf, const void *data, uint32 len)
{
	if (len == 0)
		return __ch_clear_fifo(buf);

	if (__ch_syscmd_write_avail(buf) < (len - MD_SYS_HDR_SIZE)) {
		return -1;
	}

	__ch_stream_write(buf, data, len);

	return len;
}

static void  __ch_enable_irq(struct smd_ch *ch)
{
	struct md_dev *mdev = ch->pmd;
	uint32 mask = IPC_GET_MASK(mdev);

	switch (ch->id) {
	case SMD_CH_AT1:
		mask &= ~AT1_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

	case SMD_CH_AT2:
		mask &= ~AT2_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

	case SMD_CH_PS:
		mask &= ~PS_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

	case SMD_CH_A2C_CTRL:
		mask &= ~A2C_CTRL_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

#if 1
	case SMD_CH_GNSS_LOW:
		mask &= ~GNSS_LOW_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

	case SMD_CH_GNSS_HIGH:
		mask &= ~GNSS_HIGH_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

	case SMD_CH_WIFI_SCAN:
		mask &= ~WIFI_SCAN_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;

#else
	case SMD_CH_AUD_CTRL:
		mask &= ~AUD_CTRL_READY_BIT;
		IPC_SET_MASK(mdev, mask);
		break;
#endif
	default:
		break;
	}
}

static void  __ch_disable_irq(struct smd_ch *ch)
{
	struct md_dev *mdev = ch->pmd;
	uint32 mask = IPC_GET_MASK(mdev);

	switch (ch->id) {
	case SMD_CH_AT1:
		IPC_SET_MASK(mdev, AT1_READY_BIT | mask);
		break;

	case SMD_CH_AT2:
		IPC_SET_MASK(mdev, AT2_READY_BIT | mask);
		break;

	case SMD_CH_PS:
		IPC_SET_MASK(mdev, PS_READY_BIT | mask);
		break;

	case SMD_CH_A2C_CTRL:
		IPC_SET_MASK(mdev, A2C_CTRL_READY_BIT | mask);
		break;

#if 1
	case SMD_CH_GNSS_LOW:
		IPC_SET_MASK(mdev, GNSS_LOW_READY_BIT | mask);
		break;

	case SMD_CH_GNSS_HIGH:
		IPC_SET_MASK(mdev, GNSS_HIGH_READY_BIT | mask);
		break;

	case SMD_CH_WIFI_SCAN:
		IPC_SET_MASK(mdev, WIFI_SCAN_READY_BIT | mask);
		break;

#else
	case SMD_CH_AUD_CTRL:
		IPC_SET_MASK(mdev, AUD_CTRL_READY_BIT | mask);
		break;
#endif
	default:
		break;
	}
}

static void ipc_get_ipaddr(struct md_dev *mdev)
{
	uint32 ip_type = mdev->regs->SYSMAIL24;

	//pr_info("MD_IP_ADDR_READY ip_type 0x%x\n", ip_type);

	mdev->md_ip_addr_status = 0;
	mdev->md_ipv6_addr_status = 0;
	mdev->ip_type = ip_type;

	switch (ip_type) {
	case IP_ADDR_IPV4V6:
		mdev->md_ip_addr = mdev->regs->SYSMAIL16;
		mdev->md_dns_addr = mdev->regs->SYSMAIL17;
		mdev->md_ipv6_addr_l[0] = mdev->regs->SYSMAIL18;
		mdev->md_ipv6_addr_l[1] = mdev->regs->SYSMAIL19;
		mdev->md_dnsv6_addr[0] = mdev->regs->SYSMAIL20;
		mdev->md_dnsv6_addr[1] = mdev->regs->SYSMAIL21;
		mdev->md_dnsv6_addr[2] = mdev->regs->SYSMAIL22;
		mdev->md_dnsv6_addr[3] = mdev->regs->SYSMAIL23;
		mdev->md_ip_addr_status = IP_ADDR_CHANGED;
		mdev->md_ipv6_addr_status = IP_ADDR_CHANGED;
		break;

	case IP_ADDR_ONLY_IPV4:
		mdev->md_ip_addr = mdev->regs->SYSMAIL16;
		mdev->md_dns_addr = mdev->regs->SYSMAIL17;
		mdev->md_ip_addr_status = IP_ADDR_CHANGED;
		break;

	case IP_ADDR_ONLY_IPV6:
		mdev->md_ipv6_addr_l[0] = mdev->regs->SYSMAIL18;
		mdev->md_ipv6_addr_l[1] = mdev->regs->SYSMAIL19;
		mdev->md_dnsv6_addr[0] = mdev->regs->SYSMAIL20;
		mdev->md_dnsv6_addr[1] = mdev->regs->SYSMAIL21;
		mdev->md_dnsv6_addr[2] = mdev->regs->SYSMAIL22;
		mdev->md_dnsv6_addr[3] = mdev->regs->SYSMAIL23;
		mdev->md_ipv6_addr_status = IP_ADDR_CHANGED;
		break;

	default:
		break;
	};
}

static void ipc_clr_ipaddr(struct md_dev *mdev)
{
	//pr_info("MD_IP_ADDR_DEL_NOTIFY\n");
	mdev->md_ip_addr = 0;
	mdev->md_dns_addr = 0;
	mdev->md_ipv6_addr_l[0] = 0;
	mdev->md_ipv6_addr_l[1] = 0;
	mdev->md_dnsv6_addr[0] = 0;
	mdev->md_dnsv6_addr[1] = 0;
	mdev->md_dnsv6_addr[2] = 0;
	mdev->md_dnsv6_addr[3] = 0;
	mdev->md_ip_addr_status = IP_ADDR_DELETE;
	mdev->md_ipv6_addr_status = IP_ADDR_DELETE;
}

static ISR_EXE_T ipc_mailbox_isr(uint32 data)
{
	struct md_dev *mdev = (struct md_dev *)&g_modem_dev;
	struct smd_ch *ch = NULL;
	struct smd_ch *at1 = &mdev->chs[SMD_CH_AT1];
	struct smd_ch *at2 = &mdev->chs[SMD_CH_AT2];
	uint32 irq_status = IPC_GET_STAT(mdev);
	uint32 event = 0;
	unsigned long flags;

	//pr_info("ipc: irq 0x%x\n", irq_status);	
	SCI_DisableIRQ();

	// add irq status, to use in ipc hisr
	sysmail_irq_status |= irq_status;
	
	// clear irq status
	IPC_CLR_IRQ(mdev, irq_status);

  #if 0
  // MD_EXECPTION, may assert
	if (irq_status & MD_EXECPTION_BIT) {
		if (mdev->exec_handler) {
			mdev->exec_handler();
		}
		//pmic_set_soft_reset();
		//IPC_CLR_IRQ(mdev, MD_EXECPTION_BIT);
	}
	
	//check if cp wake message?
	if (irq_status & BIT(10)) {
		IPC_CLR_IRQ(mdev, BIT(10));
	}
	
	if (irq_status & MD_IP_ADDR_ADD_BIT) {
		ipc_get_ipaddr(mdev);
		//IPC_CLR_IRQ(mdev, MD_IP_ADDR_ADD_BIT);
	}

	if (irq_status & MD_IP_ADDR_DEL_BIT) {
		ipc_clr_ipaddr(mdev);
		//IPC_CLR_IRQ(mdev, MD_IP_ADDR_DEL_BIT);
	}

	if ((irq_status & AT1_READY_BIT)
		|| (at1->event_mask & CH_RW_MASK)) {
		if ((ipc_ch_read_avail(at1) > 0)
			&& (at1->event_mask & CH_READ_AVAIL)) {
			event |= CH_READ_AVAIL;
		}
		if ((ipc_ch_write_avail(at1) > 0)
			&& (at1->event_mask & CH_WRITE_AVAIL)) {
			event |= CH_WRITE_AVAIL;
		}

		if (at1->notify) {
			//at1->notify(at1->priv, event);
		}
		event = 0;
		IPC_CLR_IRQ(mdev, AT1_READY_BIT);
	}

	if ((irq_status & AT2_READY_BIT)
		|| (at2->event_mask & CH_RW_MASK)) {
		if ((ipc_ch_read_avail(at2) > 0)
			&& (at2->event_mask & CH_READ_AVAIL)) {
			event |= CH_READ_AVAIL;
		}
		if ((ipc_ch_write_avail(at2) > 0)
			&& (at2->event_mask & CH_WRITE_AVAIL)) {
			event |= CH_WRITE_AVAIL;
		}

		if (at2->notify) {
			at2->notify(at2->priv, event);
		}
		event = 0;
		IPC_CLR_IRQ(mdev, AT2_READY_BIT);
	}

	if (irq_status & PS_READY_BIT) {
		ch = &mdev->chs[SMD_CH_PS];
		if (ch->notify) {
			//ch->notify(ch->priv, 0);
		}
		IPC_CLR_IRQ(mdev, PS_READY_BIT);
	}

	if (irq_status & A2C_CTRL_READY_BIT) {
		ch = &mdev->chs[SMD_CH_A2C_CTRL];
		if (ch->notify) {
			//ch->notify(ch->priv, 0);
		}
		IPC_CLR_IRQ(mdev, A2C_CTRL_READY_BIT);
	}

	if (irq_status & AUD_CTRL_READY_BIT) {
		ch = &mdev->chs[SMD_CH_AUD_CTRL];
		if (ch->notify) {
			//ch->notify(ch->priv, 0);
		}
		IPC_CLR_IRQ(mdev, AUD_CTRL_READY_BIT);
	}

	if (irq_status & MD_TRACE_CTRL_BIT
		|| irq_status & ZSP_TRACE_CTRL_BIT) {
		if (mdev->trace_handler) {
			uint32 value = g_mdev->regs->SYSMAIL0;
			//mdev->trace_handler(value);
		}
		IPC_CLR_IRQ(mdev, MD_TRACE_CTRL_BIT | ZSP_TRACE_CTRL_BIT);
	}
  #endif
	SCI_RestoreIRQ();

	return CALL_HISR;
}


void ipc_clear_mail_status(uint32 mail_bit)
{
    IPC_CLR_IRQ(g_mdev, mail_bit);
}

static void dummy_notify(void *priv, uint32 flags)
{
	//pr_info("ipc: channel has no notify\n");
}

int32 ipc_ch_open(int32 ch_id, struct smd_ch **ch, void *priv,
		void (*notify) (void *, uint32))
{
	struct smd_ch *tmp_ch = NULL;

	if (ipc_initialized == 0) {
		return -1;
	}

	if (ch_id >= SMD_CH_MAX) {
		//pr_err("ipc: Invalid channel id\n");
		return -1;
	}

	tmp_ch = &g_mdev->chs[ch_id];
	if (tmp_ch->opened >= 1) {
		return -1;
	} else {
		tmp_ch->opened++;
		tmp_ch->dl.pctrl->status |= CH_OPENED;
	}

	if (notify == NULL) {
		notify = dummy_notify;
	}

	tmp_ch->notify = notify;
	tmp_ch->priv = priv;

	*ch = tmp_ch;

	__ch_enable_irq(tmp_ch);

	return 0;
}

int32 ipc_ch_read(struct smd_ch *ch, void *data, uint32 len)
{
    int ret;
	uint32 *offset = (uint32 *)data;
    uint32 curr_offset, next_offset, i;
    struct ps_header *hdr;
    
    if (ch->id != SMD_CH_PS)
        SCI_GetSemaphore(ch->dl.ch_lock, SCI_WAIT_FOREVER);
    
	ret = ch->read(&ch->dl, data, len);

    if (ch->id == SMD_CH_PS)
    {
        for (i = 0; i < (ret>>2); i++)
        {
            curr_offset = offset[i];
        
            do {
                hdr = (struct ps_header *)(g_mdev->sm_base + curr_offset);
				MMU_DmaCacheSync((uint32)hdr, sizeof(struct ps_header), DMABUFFER_FROM_DEVICE);
                next_offset = hdr->next;

				MMU_DmaCacheSync((uint32)hdr, hdr->buf_size, DMABUFFER_FROM_DEVICE);
                curr_offset = next_offset;
            } while (next_offset);
        }
    }
    if (ch->id != SMD_CH_PS)
        SCI_PutSemaphore(ch->dl.ch_lock);

    return ret;
}

int32 ipc_ch_write(struct smd_ch *ch, const void *data, uint32 len)
{
	struct md_dev *mdev = ch->pmd;
	uint32 ret = 0;

    if (ch->id != SMD_CH_PS)
        SCI_GetSemaphore(ch->ul.ch_lock, SCI_WAIT_FOREVER);

	if (ch->ul.pctrl->status & CH_OPENED) {
		//pr_info("ipc: write %d\n", len);
	} else {
		//pr_err("ipc: ipc_ch_write ch%d not ready\n", ch->id);
		return -1;
	}

	ret = ch->write(&ch->ul, data, len);
	if (ret > 0) {
        SCI_DisableIRQ();
		switch (ch->id) {
		case SMD_CH_AT1:
			IPC_SET_CP_IRQ(mdev, AT1_READY_BIT);
			break;

		case SMD_CH_AT2:
			IPC_SET_CP_IRQ(mdev, AT2_READY_BIT);
			break;

		case SMD_CH_PS:
			IPC_SET_CP_IRQ(mdev, PS_READY_BIT);
			break;

		case SMD_CH_A2C_CTRL:
			IPC_SET_CP_IRQ(mdev, A2C_CTRL_READY_BIT);
			break;

#if 1
		case SMD_CH_GNSS_LOW:
			IPC_SET_CP_IRQ(mdev,GNSS_LOW_READY_BIT);
			break;

		case SMD_CH_GNSS_HIGH:
			IPC_SET_CP_IRQ(mdev,GNSS_HIGH_WRITE_BIT);
			break;

		case SMD_CH_WIFI_SCAN:
			IPC_SET_CP_IRQ(mdev,WIFI_SCAN_WRITE_BIT);
			break;
#else
		case SMD_CH_AUD_CTRL:
			IPC_SET_CP_IRQ(mdev, AUD_CTRL_READY_BIT);
			break;
#endif
		default:
			break;
		}
        SCI_RestoreIRQ();
	}

    if (ch->id != SMD_CH_PS)
        SCI_PutSemaphore(ch->ul.ch_lock);

	return ret;
}

int32 ipc_ch_close(struct smd_ch *ch)
{
	if (!ch) {
		return -1;
	}

	if (ch->opened != 1) {
		return -1;
	}
	ch->opened--;

	__ch_disable_irq(ch);

	return 0;
}

int32 ipc_ch_read_avail(struct smd_ch *ch)
{
	return ch->read_avail(&ch->dl);
}

int32 ipc_ch_write_avail(struct smd_ch *ch)
{
	return ch->write_avail(&ch->ul);
}

BOOLEAN ipc_ps_buf_full(struct smd_ch *ch)
{
	if (ch->ul.pctrl->status & PS_BUF_FULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOLEAN ipc_ps_ul_idle(struct smd_ch *ch)
{
	if (ch->ul.pctrl->status & PS_UL_BUSY) {
		return FALSE;
	} else {
		return TRUE;
	}
}

void ipc_set_ps_ul_stat(struct smd_ch *ch, uint32 stat)
{
	unsigned long flags;

	SCI_DisableIRQ();

	switch (stat) {
	case PS_STAT_UL_BUSY:
		ch->ul.pctrl->status |= PS_UL_BUSY;
		break;

	case PS_STAT_UL_IDLE:
		ch->ul.pctrl->status &= ~PS_UL_BUSY;
		break;

	default:
		break;
	}

	SCI_RestoreIRQ();
}

void ipc_reset_ps_buf(struct smd_ch *ch)
{
	unsigned long flags;

	SCI_DisableIRQ();
	ch->ul.pctrl->tail = 0;
	ch->ul.pctrl->head = 0;
	ch->ul.pctrl->alloc_net = 0;
	ch->ul.pctrl->free_net = 0;
	ch->ul.pctrl->alloc_vol = 0;
	ch->ul.pctrl->free_vol = 0;
	ch->ul.pctrl->alloc_net_l = 0;
	ch->ul.pctrl->free_net_l = 0;
	ch->ul.pctrl->alloc_vol_l = 0;
	ch->ul.pctrl->free_vol_l = 0;
	SCI_RestoreIRQ();
}

int32 ipc_alloc_ul_ps_buf(struct smd_ch *ch, uint32 size)
{
	int ret = 0;

	ret = hwspin_lock_timeout(HWSPINLOCK_IPC, 10000); // timeout is 10s
	if (ret < 0) {
		SCI_TraceLow("get ipc hwlock failed\n");
		return ret;
	}
	ret = ch->alloc_ul_psbuf(&ch->ul, size);
	hwspin_unlock(HWSPINLOCK_IPC);

	return ret;
}

int32 ipc_free_dl_ps_buf(struct smd_ch *ch, uint32 offset)
{
	return ch->free_dl_psbuf(&ch->dl, offset);
}

uint32 get_dl_free_buf_cnt(struct smd_ch *ch)
{
	uint32 alloc_ptr = ch->dl.pctrl->alloc_net;
	uint32 free_ptr = ch->dl.pctrl->free_net;

	/* only big buf */
	if (alloc_ptr > free_ptr) {
		return (g_mdev->dl_big_ps_buf_cnt - (alloc_ptr - free_ptr) - 1);
	} else if (free_ptr > alloc_ptr) {
		return free_ptr - alloc_ptr - 1;
	} else {
		return g_mdev->dl_big_ps_buf_cnt - 1;
	}
}

void ipc_disable_ch_irq(struct smd_ch *ch)
{
	__ch_disable_irq(ch);
}

void ipc_enable_ch_irq(struct smd_ch *ch)
{
	__ch_enable_irq(ch);
}

uint32 ipc_get_ch_status(struct smd_ch *ch)
{
	return ch->ul.pctrl->status;
}

BOOLEAN ipc_ps_ul_opened(void)
{
	struct smd_ch *ch = &g_mdev->chs[SMD_CH_PS];

	if (ch->ul.pctrl->status & CH_OPENED) {
		return TRUE;
	} else {
		//pr_info("ipc: ul ps not open\n");
		return FALSE;
	}
}

void ipc_notify_volte_data(void)
{
    if (g_mdev == NULL)
        return;

    SCI_DisableIRQ();
    IPC_SET_CP_IRQ(g_mdev,VOLTE_DATA_READY_BIT);
    SCI_RestoreIRQ();
}

void ipc_register_music_notify(void (*handler) (void))
{
    g_mdev->music_handler = handler;
}

void ipc_register_exec_handler(void (*handler) (void))
{
	g_mdev->exec_handler = handler;
}

void ipc_register_trace_handler(void (*handler) (uint32))
{
	g_mdev->trace_handler = handler;
}

uint32 ipc_get_cp_exec_type(void)
{
	return g_mdev->regs->SYSMAIL0;
}

uint32 ipc_get_zsp_exec_type(void)
{
	return g_mdev->regs->SYSMAIL1;
}

 void ipc_nofity_cp_assert(void)
{
	g_mdev->regs->INTSTR1 = AP_NOTIFY_CP_EXEC_BIT;
}

/*
 * is_off = 1, stop log
 * is_off = 0, start log
 */
void ipc_notify_cp_stop_log(uint32 is_off)
{
	g_mdev->regs->INTSTR1 = AP_NOTIFY_CP_STOPLOG_BIT;
	g_mdev->regs->SYSMAIL37 = is_off;
}

void ipc_set_modem_boot_mode(uint32 boot_mode)
{
	g_mdev->regs->SYSMAIL30 = boot_mode;
}

void ipc_set_modem_rf_band(uint32 rf_band_id)
{
	hwp_mailbox->SYSMAIL38 = rf_band_id;
}

#if defined(PLATFORM_UWS6121E)
void ipc_set_clock_group(uint32 clock_type_id)
{
    hwp_mailbox->SYSMAIL31 = clock_type_id;
}
#else
void ipc_set_bt_flow(uint32 bt_flow_flag)
{
    hwp_mailbox->SYSMAIL31 = bt_flow_flag;
}
#endif

void ipc_set_modem_release_time(uint32 value)
{
	g_mdev->regs->SYSMAIL12 = value;
}

uint8 *ipc_get_sbc_buf_addr(void)
{
        SCI_TRACE_LOW("[IPC]:  sbc_buf addr 0x%x\n", rda_sm_base + g_mdev->sbc_buf_off);
        return rda_sm_base + g_mdev->sbc_buf_off;
}

uint32 ipc_get_modem_version(void)
{
    SCI_TRACE_LOW("[IPC]: Modem Version SVN%d\n", 
			*(uint32 *)(rda_sm_base + g_mdev->md_version_off));
    return *(uint32 *)(rda_sm_base + g_mdev->md_version_off);
}

char * ipc_get_modem_assert_info(void)
{
    SCI_TRACE_LOW("[IPC]: Modem assert:%s\n", 
			(char *)(rda_sm_base + g_mdev->md_exec_cause_off));
    return (char *)(rda_sm_base + g_mdev->md_exec_cause_off);
}


void ipc_get_ip_dns_addr(struct ip_info *info)
{
	switch (g_mdev->ip_type) {
	case IP_ADDR_IPV4V6:
		info->ip_type			= IP_ADDR_IPV4V6;
		info->ipv4_addr 		= g_mdev->md_ip_addr;
		info->dnsv4_primary 	= g_mdev->md_dns_addr;
		memcpy(info->ipv6_intf_id, g_mdev->md_ipv6_addr_l, 8);
		memcpy(info->dnsv6_primary, g_mdev->md_dnsv6_addr, 16);
		break;

	case IP_ADDR_ONLY_IPV4:
		info->ip_type			= IP_ADDR_ONLY_IPV4;
		info->ipv4_addr		= g_mdev->md_ip_addr;
		info->dnsv4_primary 	= g_mdev->md_dns_addr;
		break;

	case IP_ADDR_ONLY_IPV6:
		info->ip_type			= IP_ADDR_ONLY_IPV6;
		memcpy(info->ipv6_intf_id, g_mdev->md_ipv6_addr_l, 8);
		memcpy(info->dnsv6_primary, g_mdev->md_dnsv6_addr, 16);
		break;

	default:
		info->ip_type			= 0;
		break;
	};
}

void ipc_set_sbc_buf_addr(uint32 addr)
{
    g_mdev->regs->SYSMAIL10 = addr;
    g_mdev->regs->INTSTR2 = BIT(9);
}

uint8 *ipc_get_ims_nv_buf_addr(void)
{
    return rda_sm_base + g_mdev->delta_nv_off;
}

uint8 *ipc_get_ims_nv_buf_info_addr(void)
{
	g_mdev->regs->SYSMAIL36 = (uint32)(rda_sm_base + g_mdev->ims_nv_info_off);
    return rda_sm_base + g_mdev->ims_nv_info_off;
}

void ipc_set_delta_nv_bin_addr(uint32 addr, uint32 len)
{
    g_mdev->regs->SYSMAIL34 = addr;
    g_mdev->regs->SYSMAIL35 = len;
}

void ipc_set_bt_rf_sm_addr(void)
{
    //g_mdev->regs->SYSMAIL87 = g_mdev->sm_base + g_mdev->bt_rf_sm_off;
}

void ipc_set_rfpara(uint32 *src_addr)
{
    g_mdev->regs->SYSMAIL90 = *src_addr++;
    g_mdev->regs->SYSMAIL91 = *src_addr++;
    g_mdev->regs->SYSMAIL92 = *src_addr++;
    g_mdev->regs->SYSMAIL93 = *src_addr++;
    g_mdev->regs->SYSMAIL94 = *src_addr++;
}

static int32 ipc_init_ch(struct smd_ch *ch)
{
	struct md_dev *mdev = (struct md_dev *)ch->pmd;
	struct ps_header *hdr;
	uint32 i, end_offset;
	uint32 ps_ul_free_buf_len_b, ps_ul_free_buf_len_l;
	uint32 ps_dl_free_buf_len_b, ps_dl_free_buf_len_l;
	uint32 ps_ul_free_buf_off_b, ps_ul_free_buf_off_l;
	uint32 ps_dl_net_free_buf_off_b, ps_dl_net_free_buf_off_l;
	uint32 ps_dl_vol_free_buf_off_b, ps_dl_vol_free_buf_off_l;
	uint32 ps_ul_ip_data_off_b, ps_ul_ip_data_off_l;
	uint32 ps_dl_ip_data_off_b, ps_dl_ip_data_off_l;

	switch (ch->id) {
	case SMD_CH_AT1:
		ch->type = SMD_CH_STREAM_TYPE;

		ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						AT1_UL_CTRL_OFFSET);
		ch->ul.pfifo = (uint8 *)(mdev->sm_base + AT1_UL_DATA_OFFSET);
		ch->ul.fifo_size = AT1_BUF_SIZE;
		ch->ul.fifo_mask = AT1_BUF_SIZE - 1;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipcat1_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem ipcat1_ul failed!", __FILE__, __LINE__);

		ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						AT1_DL_CTRL_OFFSET);
		ch->dl.pfifo = (uint8 *)(mdev->sm_base + AT1_DL_DATA_OFFSET);
		ch->dl.fifo_size = AT1_BUF_SIZE;
		ch->dl.fifo_mask = AT1_BUF_SIZE - 1;
        ch->dl.ch_lock = SCI_CreateSemaphore("ipcat1_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem ipcat1_dl failed!", __FILE__, __LINE__);
		break;

	case SMD_CH_AT2:
		ch->type = SMD_CH_STREAM_TYPE;

		ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						AT2_UL_CTRL_OFFSET);
		ch->ul.pfifo = (uint8 *)(mdev->sm_base + AT2_UL_DATA_OFFSET);
		ch->ul.fifo_size = AT2_BUF_SIZE;
		ch->ul.fifo_mask = AT2_BUF_SIZE - 1;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipcrpc_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem iprpc_ul failed!", __FILE__, __LINE__);

		ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						AT2_DL_CTRL_OFFSET);
		ch->dl.pfifo = (uint8 *)(mdev->sm_base + AT2_DL_DATA_OFFSET);
		ch->dl.fifo_size = AT2_BUF_SIZE;
		ch->dl.fifo_mask = AT2_BUF_SIZE - 1;
        ch->dl.ch_lock = SCI_CreateSemaphore("ipcrpc_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem iprpc_dl failed!", __FILE__, __LINE__);
		break;

	case SMD_CH_A2C_CTRL:
		ch->type = SMD_CH_QUEUE_TYPE;

		ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						A2C_UL_CTRL_OFFSET);
		ch->ul.pfifo = (uint8 *)(mdev->sm_base + A2C_UL_DATA_OFFSET);
		ch->ul.fifo_size = A2C_CTRL_BUF_SIZE;
		ch->ul.fifo_mask = A2C_CTRL_BUF_SIZE - 1;
		ch->ul.queue_len = IPC_CMD_QUEUE_LEN;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipctrl_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem ipctrl_ul failed!", __FILE__, __LINE__);

		ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						A2C_DL_CTRL_OFFSET);
		ch->dl.pfifo = (uint8 *)(mdev->sm_base + A2C_DL_DATA_OFFSET);
		ch->dl.fifo_size = A2C_CTRL_BUF_SIZE;
		ch->dl.fifo_mask = A2C_CTRL_BUF_SIZE - 1;
		ch->dl.queue_len = IPC_CMD_QUEUE_LEN;
        ch->dl.ch_lock = SCI_CreateSemaphore("ipctrl_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem ipctrl_dl failed!", __FILE__, __LINE__);
		break;

#if 1 //def GPS_TRANS_VIA_SIPC
 case SMD_CH_GNSS_LOW:
        ch->type = SMD_CH_STREAM_TYPE;

        ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base +
                                              GNSS_LOW_UL_CTRL_OFFSET);
        ch->ul.pfifo = (uint8 *)(mdev->sm_base + GNSS_LOW_UL_DATA_OFFSET);
        ch->ul.fifo_size = GNSS_LOW_BUF_SIZE;
        ch->ul.fifo_mask = GNSS_LOW_BUF_SIZE - 1;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipcgnssL_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem ipcgnssL_ul failed!", __FILE__, __LINE__);

        ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base +
                                              GNSS_LOW_DL_CTRL_OFFSET);
        ch->dl.pfifo = (uint8 *)(mdev->sm_base + GNSS_LOW_DL_DATA_OFFSET);
        ch->dl.fifo_size = GNSS_LOW_BUF_SIZE;
        ch->dl.fifo_mask = GNSS_LOW_BUF_SIZE - 1;
        ch->dl.ch_lock = SCI_CreateSemaphore("ipcgnssL_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem ipcgnssL_dl failed!", __FILE__, __LINE__);
		
        break;

    case SMD_CH_GNSS_HIGH:
        ch->type = SMD_CH_STREAM_TYPE;

        ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base +
                                              GNSS_HIGH_UL_CTRL_OFFSET);
        ch->ul.pfifo = (uint8 *)(mdev->sm_base + GNSS_HIGH_UL_DATA_OFFSET);
        ch->ul.fifo_size = GNSS_HIGH_BUF_SIZE;
        ch->ul.fifo_mask = GNSS_HIGH_BUF_SIZE - 1;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipcgnssH_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem ipcgnssH_ul failed!", __FILE__, __LINE__);

        ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base +
                                              GNSS_HIGH_DL_CTRL_OFFSET);
        ch->dl.pfifo = (uint8 *)(mdev->sm_base + GNSS_HIGH_DL_DATA_OFFSET);
        ch->dl.fifo_size = GNSS_HIGH_BUF_SIZE;
        ch->dl.fifo_mask = GNSS_HIGH_BUF_SIZE - 1;
	ch->dl.ch_lock = SCI_CreateSemaphore("ipcgnssH_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem ipcgnssH_dl failed!", __FILE__, __LINE__);
	
        break;
#endif

#if 1 //def WIFI_SUPPORT
    case SMD_CH_WIFI_SCAN:
        ch->type = SMD_CH_STREAM_TYPE;

        ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base +
                                              WIFI_SCAN_UL_CTRL_OFFSET);
        ch->ul.pfifo = (uint8 *)(mdev->sm_base + WIFI_SCAN_UL_DATA_OFFSET);
        ch->ul.fifo_size = WIFI_SCAN_BUF_SIZE;
        ch->ul.fifo_mask = WIFI_SCAN_BUF_SIZE - 1;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipcwifi_scan_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem ipcwifi_scan_dl failed!", __FILE__, __LINE__);


        ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base +
                                              WIFI_SCAN_DL_CTRL_OFFSET);
        ch->dl.pfifo = (uint8 *)(mdev->sm_base + WIFI_SCAN_DL_DATA_OFFSET);
        ch->dl.fifo_size = WIFI_SCAN_BUF_SIZE;
        ch->dl.fifo_mask = WIFI_SCAN_BUF_SIZE - 1;
	ch->dl.ch_lock = SCI_CreateSemaphore("ipcwifi_scan_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem ipcwifi_scan_dl failed!", __FILE__, __LINE__);
		
        break;
#endif

#if 0
	case SMD_CH_AUD_CTRL:
		ch->type = SMD_CH_SYSCMD_TYPE;

		ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						AUD_UL_CTRL_OFFSET);
		ch->ul.pfifo = (uint8 *)(mdev->sm_base + AUD_UL_DATA_OFFSET);
		ch->ul.fifo_size = AUD_CTRL_BUF_SIZE;
		ch->ul.fifo_mask = AUD_CTRL_BUF_SIZE - 1;
        ch->ul.ch_lock = SCI_CreateSemaphore("ipaud_ul", 1);
        if (ch->ul.ch_lock == NULL)
            SCI_Assert("CreateSem ipaud_ul failed!", __FILE__, __LINE__);

		ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						AUD_DL_CTRL_OFFSET);
		ch->dl.pfifo = (uint8 *)(mdev->sm_base + AUD_DL_DATA_OFFSET);
		ch->dl.fifo_size = AUD_CTRL_BUF_SIZE;
		ch->dl.fifo_mask = AUD_CTRL_BUF_SIZE - 1;
        ch->dl.ch_lock = SCI_CreateSemaphore("ipaud_dl", 1);
        if (ch->dl.ch_lock == NULL)
            SCI_Assert("CreateSem ipaud_dl failed!", __FILE__, __LINE__);
		break;
#endif
	case SMD_CH_PS:
		ch->type = SMD_CH_PACKET_TYPE;

		ps_ul_free_buf_len_b = mdev->ul_big_ps_buf_cnt * 4;
		ps_ul_free_buf_len_l = mdev->ul_lit_ps_buf_cnt * 4;
		ps_dl_free_buf_len_b = mdev->dl_big_ps_buf_cnt * 4;
		ps_dl_free_buf_len_l = mdev->dl_lit_ps_buf_cnt * 4;

		ps_ul_free_buf_off_b = PS_DL_CTRL_DATA_OFFSET + PS_CTRL_BUF_SIZE + 32;
		ps_ul_free_buf_off_l = ps_ul_free_buf_off_b + ps_ul_free_buf_len_b + 32;
		ps_dl_net_free_buf_off_b = ps_ul_free_buf_off_l + ps_ul_free_buf_len_l + 32;
		ps_dl_net_free_buf_off_l = ps_dl_net_free_buf_off_b + ps_dl_free_buf_len_b + 32;
		ps_dl_vol_free_buf_off_b = ps_dl_net_free_buf_off_l + ps_dl_free_buf_len_l + 32;
		ps_dl_vol_free_buf_off_l = ps_dl_vol_free_buf_off_b + ps_dl_free_buf_len_b + 32;

        /* ip data off align to cache line (32Bytes) */
		ps_ul_ip_data_off_b = ps_dl_vol_free_buf_off_l + ps_dl_free_buf_len_l;
        if (ps_ul_ip_data_off_b & 0xFFF)
        {
            ps_ul_ip_data_off_b = (ps_ul_ip_data_off_b + 0xFFF) & (~0xFFF);
        }
		ps_ul_ip_data_off_l = ps_ul_ip_data_off_b + (mdev->ul_big_ps_buf_cnt * PS_BUF_UL_LEN_B);
		ps_dl_ip_data_off_b = ps_ul_ip_data_off_l + (mdev->ul_lit_ps_buf_cnt * PS_BUF_UL_LEN_L);
		ps_dl_ip_data_off_l = ps_dl_ip_data_off_b + (mdev->dl_big_ps_buf_cnt * PS_BUF_DL_LEN_B);

		end_offset = ps_dl_ip_data_off_l + (mdev->dl_lit_ps_buf_cnt * PS_BUF_DL_LEN_L);
		//if (end_offset > mdev->md_version_off) {
			//dev_err(&pdev->dev, "end_offset is out of range\n");
			//BUG();
		//}

        mdev->sbc_buf_off = end_offset + 256;
        if (mdev->sbc_buf_off & 0xFFF)
        {
            mdev->sbc_buf_off = ((mdev->sbc_buf_off + 0xFFF) & (~0xFFF)) + 256;
        }
        mdev->delta_nv_off = mdev->sbc_buf_off + 2048;
        mdev->ims_nv_info_off = mdev->delta_nv_off + 80*1024;
        mdev->bt_rf_sm_off = mdev->ims_nv_info_off + 4096;

        if (mdev->bt_rf_sm_off > mdev->md_version_off) { 
            SCI_Assert("IPC end_offset is out of range", __FILE__, __LINE__);
        }

        mdev->regs->SYSMAIL67 = mdev->sm_base + mdev->delta_nv_off;
        mdev->regs->SYSMAIL68 = mdev->sm_base + mdev->ims_nv_info_off;
        //mdev->regs->SYSMAIL87 = mdev->sm_base + mdev->bt_rf_sm_off;

#if 0
		dev_info(&pdev->dev, ">> free buf info\n");
		dev_info(&pdev->dev, "ul_big 0x%x\n", ps_ul_free_buf_off_b);
		dev_info(&pdev->dev, "ul_lit 0x%x\n", ps_ul_free_buf_off_l);
		dev_info(&pdev->dev, "dl_big_net 0x%x\n", ps_dl_net_free_buf_off_b);
		dev_info(&pdev->dev, "dl_lit_net 0x%x\n", ps_dl_net_free_buf_off_l);
		dev_info(&pdev->dev, "dl_big_vol 0x%x\n", ps_dl_vol_free_buf_off_b);
		dev_info(&pdev->dev, "dl_lit_vol 0x%x\n", ps_dl_vol_free_buf_off_l);

		dev_info(&pdev->dev, ">> data buf info\n");
		dev_info(&pdev->dev, "ul_big_ip 0x%x\n", ps_ul_ip_data_off_b);
		dev_info(&pdev->dev, "ul_lit_ip 0x%x\n", ps_ul_ip_data_off_l);
		dev_info(&pdev->dev, "dl_big_ip 0x%x\n", ps_dl_ip_data_off_b);
		dev_info(&pdev->dev, "dl_lit_ip 0x%x\n", ps_dl_ip_data_off_l);
#endif

		ch->ul.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
						PS_UL_CTRL_OFFSET);
		ch->ul.pfifo = (uint8 *)(mdev->sm_base + PS_UL_CTRL_DATA_OFFSET);
		ch->ul.fifo_size = PS_CTRL_BUF_SIZE;
		ch->ul.fifo_mask = PS_CTRL_BUF_SIZE - 1;
		ch->ul.net_free_buf_b = (uint32 *)(mdev->sm_base + ps_ul_free_buf_off_b);
		ch->ul.net_free_buf_l = (uint32 *)(mdev->sm_base + ps_ul_free_buf_off_l);
		ch->ul.vol_free_buf_b = NULL;
		ch->ul.vol_free_buf_l = NULL;
		for (i = 0; i < mdev->ul_big_ps_buf_cnt; i++) {
			ch->ul.net_free_buf_b[i] = ps_ul_ip_data_off_b + (i * PS_BUF_UL_LEN_B);
			hdr = (struct ps_header *)(rda_sm_base + ch->ul.net_free_buf_b[i]);
			hdr->flag = BUF_IN_IDLE;
		}
		for (i = 0; i < mdev->ul_lit_ps_buf_cnt; i++) {
			ch->ul.net_free_buf_l[i] = ps_ul_ip_data_off_l + (i * PS_BUF_UL_LEN_L);
			hdr = (struct ps_header *)(rda_sm_base + ch->ul.net_free_buf_l[i]);
			hdr->flag = BUF_IN_IDLE;
		}
		mdev->ul_big_start = ch->ul.net_free_buf_b[0];
		mdev->ul_big_end   = ch->ul.net_free_buf_b[mdev->ul_big_ps_buf_cnt - 1];
		mdev->ul_lit_start = ch->ul.net_free_buf_l[0];
		mdev->ul_lit_end   = ch->ul.net_free_buf_l[mdev->ul_lit_ps_buf_cnt - 1];
        ch->ul.ch_lock = NULL;
        //dev_info(&pdev->dev, "ul big free buf offset range 0x%x -- 0x%x\n",
		//			mdev->ul_big_start, mdev->ul_big_end);
		//dev_info(&pdev->dev, "ul lit free buf offset range 0x%x -- 0x%x\n",
		//			mdev->ul_lit_start, mdev->ul_lit_end);

		ch->dl.pctrl = (struct ch_buf_ctrl *)(sram_sm_base + \
							PS_DL_CTRL_OFFSET);
		ch->dl.pfifo = (uint8 *)(mdev->sm_base + PS_DL_CTRL_DATA_OFFSET);
		ch->dl.fifo_size = PS_CTRL_BUF_SIZE;
		ch->dl.fifo_mask = PS_CTRL_BUF_SIZE - 1;
		ch->dl.net_free_buf_b = (uint32 *)(mdev->sm_base + ps_dl_net_free_buf_off_b);
		ch->dl.net_free_buf_l = (uint32 *)(mdev->sm_base + ps_dl_net_free_buf_off_l);
		ch->dl.vol_free_buf_b = (uint32 *)(mdev->sm_base + ps_dl_vol_free_buf_off_b);
		ch->dl.vol_free_buf_l = (uint32 *)(mdev->sm_base + ps_dl_vol_free_buf_off_l);
		for (i = 0; i < mdev->dl_big_ps_buf_cnt; i++) {
			ch->dl.net_free_buf_b[i] = ps_dl_ip_data_off_b + (i * PS_BUF_DL_LEN_B);
			ch->dl.vol_free_buf_b[i] = 0;
		}
		for (i = 0; i < mdev->dl_lit_ps_buf_cnt; i++) {
			ch->dl.net_free_buf_l[i] = ps_dl_ip_data_off_l + (i * PS_BUF_DL_LEN_L);
			ch->dl.vol_free_buf_l[i] = 0;
		}
		mdev->dl_big_start = ch->dl.net_free_buf_b[0];
		mdev->dl_big_end   = ch->dl.net_free_buf_b[mdev->dl_big_ps_buf_cnt - 1];
		mdev->dl_lit_start = ch->dl.net_free_buf_l[0];
		mdev->dl_lit_end   = ch->dl.net_free_buf_l[mdev->dl_lit_ps_buf_cnt - 1];
        ch->dl.ch_lock = NULL;
        //dev_info(&pdev->dev, "dl big free buf offset range 0x%x -- 0x%x\n",
		//			mdev->dl_big_start, mdev->dl_big_end);
		//dev_info(&pdev->dev, "dl lit free buf offset range 0x%x -- 0x%x\n",
		//			mdev->dl_lit_start, mdev->dl_lit_end);
		break;

	default:
		//dev_err(&pdev->dev, "Invalid channel id %d\n", ch->id);
		return -1;
	};

	//spin_lock_init(&ch->ul.lock);
	//spin_lock_init(&ch->dl.lock);
	//spin_lock_init(&ch->notify_lock);

	ch->ul.ch = (void *)ch;
	ch->dl.ch = (void *)ch;

	if (ch->type == SMD_CH_STREAM_TYPE) {
		ch->read = __ch_stream_read;
		ch->write = __ch_stream_write;
		ch->read_avail = __ch_stream_read_avail;
		ch->write_avail = __ch_stream_write_avail;
	} else if (ch->type == SMD_CH_PACKET_TYPE) {
		ch->read = __ch_packet_read;
		ch->write = __ch_packet_write;
		ch->read_avail = __ch_packet_read_avail;
		ch->write_avail = __ch_packet_write_avail;
		ch->alloc_ul_psbuf = __ch_packet_alloc_psbuf;
		ch->free_dl_psbuf = __ch_packet_free_psbuf;
	} else if (ch->type == SMD_CH_QUEUE_TYPE) {
		ch->read = __ch_queue_read;
		ch->write = __ch_queue_write;
		ch->read_avail = __ch_queue_read_avail;
		ch->write_avail = __ch_queue_write_avail;
	} else if (ch->type == SMD_CH_SYSCMD_TYPE) {
		ch->read = __ch_syscmd_read;
		ch->write = __ch_syscmd_write;
		ch->read_avail = __ch_stream_read_avail;
		ch->write_avail = __ch_syscmd_write_avail;
	}

	return 0;
}

#define LPS_CP2AP_IPCCOMM	  (hwp_idleLps->lps_res9)

extern PUBLIC uint32 SLEEP_GetCur32kNum(void);

void ipc_hisr_func (uint32 i, void *d)
{
	struct md_dev *mdev = (struct md_dev *)&g_modem_dev;
	struct smd_ch *ch = NULL;
	struct smd_ch *at1 = &mdev->chs[SMD_CH_AT1];
	struct smd_ch *at2 = &mdev->chs[SMD_CH_AT2];
	uint32 event = 0;
	uint32 Temp32K = 0;
	unsigned long flags;
	

	SCI_TraceLow("ipc hisr, status: 0x%x,32K:0x%x", sysmail_irq_status,SLEEP_GetCur32kNum());			
	//SCI_CPCPU_EnableDeepSleep(0);

 // MD_EXECPTION, may assert
	if (sysmail_irq_status & MD_EXECPTION_BIT) {
		if (mdev->exec_handler) {
			mdev->exec_handler();
		}
	}

#if 0//BT wakeup donot use this interrupt anymore
    if (sysmail_irq_status & RISC_BT_NOTIFY_AP_WAKEUP) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~RISC_BT_NOTIFY_AP_WAKEUP;
		WCN_BT_Wakeup_Callback();
		SCI_RestoreIRQ();
	}
#endif
    if (sysmail_irq_status & RISC_BT_NOTIFY_AP_SLEEP) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~RISC_BT_NOTIFY_AP_SLEEP;
#ifdef BLUETOOTH_SUPPORT
		WCN_BT_Sleep_Callback(g_mdev->regs->SYSMAIL88);
#endif
		SCI_RestoreIRQ();
	}

        if (sysmail_irq_status & ZSP_MUSIC_NOTIFY) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~ZSP_MUSIC_NOTIFY;
		SCI_RestoreIRQ();
		Temp32K = SLEEP_GetCur32kNum();
		SCI_TraceLow("ipc hisr, status: 0x%x,Cur32K:%d,Dur:%dms, call hard callback", 
			sysmail_irq_status,Temp32K,(((Temp32K-Last32K_Num)*1000) >>15));	
		Last32K_Num = SLEEP_GetCur32kNum();
		//ARMVB_PLAYBACK_Callback(); // TODO: LJF TEMP CODE
               //ARMVB_CAPTURE_Callback();
            if (mdev->music_handler) {
                mdev->music_handler();
            }
        }

         if (sysmail_irq_status & ZSP_SBC_NOTIFY) {
                SCI_TraceLow("ipc hisr, status: 0x%x call sbc callback", sysmail_irq_status);	
                sysmail_irq_status &= ~ZSP_SBC_NOTIFY;
#ifdef BLUETOOTH_SUPPORT
                SBC_8910_ZSP_Callback();
#endif
        }

	//check if cp wake message?
	//bringup_tmp
	#if 0
	if (sysmail_irq_status & BIT(10)) {
		SCI_DisableIRQ();
		if(LPS_CP2AP_IPCCOMM == 0xCE){
			extern void TCPIP_SetState(int flag);
			TCPIP_SetState(FALSE);
			SCI_CPCPU_EnableDeepSleep(1);
			SLEEP_LpsGetCpUnlock32K();
		}
		else{
			SCI_CPCPU_EnableDeepSleep(0);
			//if (hwp_idle->IDL_CTRL_SYS1&0x1)
			if (hwp_idleLps->lps_ctrl_cp &0x1)
			{
				SCI_CPCPU_EnableDeepSleep(1);
				SLEEP_LpsGetCpUnlock32K();
			}
			
		}
		sysmail_irq_status &= ~(BIT(10));
		SCI_RestoreIRQ();
		LPS_CP2AP_IPCCOMM = 0;
	}
	#endif
	
	if (sysmail_irq_status & MD_IP_ADDR_ADD_BIT) {
		ipc_get_ipaddr(mdev);
		
		SCI_DisableIRQ();
		sysmail_irq_status &= ~MD_IP_ADDR_ADD_BIT;
		SCI_RestoreIRQ();
	}

	if (sysmail_irq_status & MD_IP_ADDR_DEL_BIT) {
		ipc_clr_ipaddr(mdev);
		
		SCI_DisableIRQ();
		sysmail_irq_status &= ~MD_IP_ADDR_DEL_BIT;
		SCI_RestoreIRQ();
	}

 // not support at1
 #if 0
	if ((sysmail_irq_status & AT1_READY_BIT)
		|| (at1->event_mask & CH_RW_MASK)) {
		if ((ipc_ch_read_avail(at1) > 0)
			&& (at1->event_mask & CH_READ_AVAIL)) {
			event |= CH_READ_AVAIL;
		}
		if ((ipc_ch_write_avail(at1) > 0)
			&& (at1->event_mask & CH_WRITE_AVAIL)) {
			event |= CH_WRITE_AVAIL;
		}

		if (at1->notify && (event != 0)) {
			//at1->notify(at1->priv, event);
		}
		event = 0;
	}
 #endif

#ifdef GPS_TRANS_VIA_SIPC

       if (sysmail_irq_status & GNSS_HIGH_READY_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~GNSS_HIGH_READY_BIT;
		SCI_RestoreIRQ();

		ch = &mdev->chs[SMD_CH_GNSS_HIGH];
		if (ch->notify) {
			ch->notify(ch->priv, 0);
		}
	}

       if (sysmail_irq_status & GNSS_LOW_READY_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~GNSS_LOW_READY_BIT;
		SCI_RestoreIRQ();

		ch = &mdev->chs[SMD_CH_GNSS_LOW];
		if (ch->notify) {
			ch->notify(ch->priv, 0);
		}
	}	   
 #endif

 #ifdef WIFI_SUPPORT
       if (sysmail_irq_status & WIFI_SCAN_READY_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~WIFI_SCAN_READY_BIT;
		SCI_RestoreIRQ();

		ch = &mdev->chs[SMD_CH_WIFI_SCAN];
		if (ch->notify) {
			ch->notify(ch->priv, 0);
		}
	}
#endif

	if (sysmail_irq_status & AT2_READY_BIT) {
		if (ipc_ch_read_avail(at2) > 0) {
			event |= CH_READ_AVAIL;
		}
		if (ipc_ch_write_avail(at2) > 0) {
			event |= CH_WRITE_AVAIL;
		}
		
		SCI_DisableIRQ();
		sysmail_irq_status &= ~AT2_READY_BIT;
		SCI_RestoreIRQ();

		if (at2->notify && (event != 0)) {
			at2->notify(at2->priv, event);
		}
		event = 0;
	}

	if (sysmail_irq_status & PS_READY_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~PS_READY_BIT;
		SCI_RestoreIRQ();

		ch = &mdev->chs[SMD_CH_PS];
		if (ch->notify) {
			ch->notify(ch->priv, 0);
		}
	}
	
	if (sysmail_irq_status & A2C_CTRL_READY_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~A2C_CTRL_READY_BIT;
		SCI_RestoreIRQ();

		ch = &mdev->chs[SMD_CH_A2C_CTRL];
		if (ch->notify) {
			 ch->notify(ch->priv, 0);
		}
	}

 // not support?
 #if 0
	if (sysmail_irq_status & AUD_CTRL_READY_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~AUD_CTRL_READY_BIT;
		SCI_RestoreIRQ();

		ch = &mdev->chs[SMD_CH_AUD_CTRL];
		if (ch->notify) {
			ch->notify(ch->priv, 0);
		}
	}
 #endif

	if (sysmail_irq_status & MD_TRACE_CTRL_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~(MD_TRACE_CTRL_BIT);
		SCI_RestoreIRQ();

		SIO_SendModemTrace(g_mdev->regs->SYSMAIL3);
	}
	if (sysmail_irq_status & ZSP_TRACE_CTRL_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~(ZSP_TRACE_CTRL_BIT);
		SCI_RestoreIRQ();

		SIO_SendModemTrace(g_mdev->regs->SYSMAIL4);
	}

#ifdef BT_USB_LOG
	if (sysmail_irq_status & RISC_BT_LOG_NOTIFY) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~(RISC_BT_LOG_NOTIFY);
		SCI_RestoreIRQ();
		SIO_BT_LOG_Info_Notify(g_mdev->regs->SYSMAIL27);
	}
#endif

	if (sysmail_irq_status & RISC_BT_ASSERT_NOTIFY)
    {
            SCI_DisableIRQ();
            sysmail_irq_status &= ~(RISC_BT_ASSERT_NOTIFY);
            SCI_RestoreIRQ();
            ipc_bt_assert_notify();
	}

       if (sysmail_irq_status & CPRF_PARA_STORE_BIT) {
		SCI_DisableIRQ();
		sysmail_irq_status &= ~CPRF_PARA_STORE_BIT;
		SCI_RestoreIRQ();
              FLASH_RfParaStoreCtrl((volatile uint32 *)(&g_mdev->regs->SYSMAIL90));
	}
}

void ipc_modem_assert_notify(void)
{
     SCI_TraceLow("CP modem notify AP assert!");		
     SCI_Assert((ipc_get_modem_assert_info()), __FILE__, __LINE__);
}

void ipc_bt_assert_notify(void)
{
//no support BT on w217
#if 0
     uint32 value = IPC_GET_BT_ASSERT(g_mdev);
#if 0
     char a_bt_assert[50]={0};
     SCI_TraceLow("BT notify AP assert!");	
     sprintf (a_bt_assert, "BT assert 0x%x",IPC_GET_BT_ASSERT(g_mdev));	 
     SCI_Assert(a_bt_assert, __FILE__, __LINE__);
#else
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    BT_Self_Recover(value);
#endif
#endif
#endif
}

uint32 ipc_get_init_status(void)
{
    return ipc_initialized;
}

 int ipc_init(void)
{
	struct md_dev *mdev;
	struct smd_ch *ch;
	int32 i, ret, buf_len = 0;

	mdev = (struct md_dev *)&g_modem_dev;
	mdev->sm_base = GET_MemoryStartAddressAndLen(MEM_AP_CP_SM_NAME,&mdev->sm_len);
	mdev->ul_big_ps_buf_cnt = UL_BIG_BUF_CNT;
	mdev->ul_lit_ps_buf_cnt = UL_LIT_BUF_CNT;
	mdev->dl_big_ps_buf_cnt = DL_BIG_BUF_CNT;
	mdev->dl_lit_ps_buf_cnt = DL_LIT_BUF_CNT;
	mdev->md_version_off	= (mdev->sm_len - MD_EXEC_CAUSE_SIZE - MD_VERSION_SIZE);
	mdev->md_exec_cause_off = (mdev->sm_len - MD_EXEC_CAUSE_SIZE);
	
	rda_sm_base = mdev->sm_base ;
        sram_sm_base = AON_IRAM_CFG_IPC_BASE_ADDR;
        memset(sram_sm_base, 0, AON_IRAM_CFG_IPC_SIZE);
	memset(rda_sm_base, 0, mdev->sm_len);
	//dev_info(&pdev->dev, "sm_base %p, len 0x%x\n",
	//			rda_sm_base, mdev->sm_len);

	mdev->regs = REG_MAILBOX_BASE;
	if (!mdev->regs) {
		//dev_err(&pdev->dev, "remap mailbox failed!\n");
		return -1;
	}

	mdev->irq = TB_MAILBOX_ARM_AP_INT;
	if (mdev->irq < 0) {
		//dev_err(&pdev->dev, "no irq resource\n");
		return mdev->irq;
	}

#if 0
	dev_info(&pdev->dev, "ul_big_ps_buf_cnt %u\n", mdev->ul_big_ps_buf_cnt);
	dev_info(&pdev->dev, "ul_lit_ps_buf_cnt %u\n", mdev->ul_lit_ps_buf_cnt);
	dev_info(&pdev->dev, "dl_big_ps_buf_cnt %u\n", mdev->dl_big_ps_buf_cnt);
	dev_info(&pdev->dev, "dl_lit_ps_buf_cnt %u\n", mdev->dl_lit_ps_buf_cnt);
	dev_info(&pdev->dev, "md_version_off %u\n", mdev->md_version_off);
	dev_info(&pdev->dev, "md_exec_cause_off %u\n", mdev->md_exec_cause_off);
#endif

	IPC_SET_PS_UL_CNT_B(mdev, mdev->ul_big_ps_buf_cnt);
	IPC_SET_PS_UL_CNT_L(mdev, mdev->ul_lit_ps_buf_cnt);
	IPC_SET_PS_DL_CNT_B(mdev, mdev->dl_big_ps_buf_cnt);
	IPC_SET_PS_DL_CNT_L(mdev, mdev->dl_lit_ps_buf_cnt);
	IPC_SET_MD_VER_OFF(mdev, mdev->md_version_off);
	IPC_SET_MD_EXEC_OFF(mdev, mdev->md_exec_cause_off);

	for (i = 0; i < SMD_CH_MAX; i++) {
		ch = &mdev->chs[i];
		ch->id = i;
		ch->pmd = (void *)mdev;

		ipc_init_ch(ch);
	}
	IPC_SET_MASK(mdev, IRQ_MASK);

	ret = ISR_RegHandler_Ex(mdev->irq, (TB_ISR)ipc_mailbox_isr, ipc_hisr_func, 
			CHIPDRV_HISR_PRIO_1, NULL);
	if (ret != TB_SUCCESS) 
	{
		return ret;
	}
	CHIPDRV_EnableIRQINT(mdev->irq);

	//ret = devm_request_irq(&pdev->dev, mdev->irq, ipc_mailbox_isr,
	//			IRQF_NO_SUSPEND | IRQF_ONESHOT, pdev->name,
	//			(void *)mdev);
	//if (ret < 0) {
		//dev_err(&pdev->dev, "request irq failed\n");
		//goto err_request_irq;
	//}

	//g_ipc_hwlock = hwspin_lock_request_specific(HWLOCK_IPC);
	//if (!g_ipc_hwlock) {
	//	dev_err(&pdev->dev, "request hwspinlock failed\n");
	//	goto err_request_irq;
	//}
	
	buf_len = mdev->dl_lit_end - AT1_UL_DATA_OFFSET;
    if (buf_len & 0xFFF)
    {
        buf_len = (buf_len + 0xFFF) & (~0xFFF);
    }
    mmu_MapOnPage_Set(rda_sm_base + AT1_UL_DATA_OFFSET, 
		rda_sm_base + AT1_UL_DATA_OFFSET, MEM_TYPE_MEMORY_WB_DATA, buf_len);

	mdev->pdata = NULL;//(void *)pdev;
	mdev->md_ip_addr = 0;
	mdev->md_dns_addr = 0;
	mdev->md_ip_addr_status = 0;
	ipc_initialized = 1;
	g_mdev = mdev;

	//dev_info(&pdev->dev, "init ok\n");
	
       // register ipc assert function
    ipc_register_exec_handler(ipc_modem_assert_notify);
    //ipc_set_bt_rf_sm_addr();

    // register lps sysmail reg save/resume node
    //sysmail_PhySuspendResumeLogin();
	return 0;
}

static void sysmail_PhyRegSave(void)
{    
    uint32 sysmail_regs_protect_table[SYSMAIL_SAVE_REG_NUM] = 
    {    
        REG_MAILBOX_BASE+0x35C,        /*sysmail87 BT & RF share mem addr*/
    };     
    Slp_LpsRegsSave((uint32*)g_sysmail_regs_data, sysmail_regs_protect_table, SYSMAIL_SAVE_REG_NUM);
}

static void sysmail_PhyRegResume(void)
{   
    uint32 sysmail_regs_protect_table[SYSMAIL_SAVE_REG_NUM] = 
    {    
        REG_MAILBOX_BASE+0x35C,        /*sysmail87 BT & RF share mem addr*/
    };     
    
    Slp_LpsRegsResume((uint32*)g_sysmail_regs_data, sysmail_regs_protect_table, SYSMAIL_SAVE_REG_NUM);
}

static void sysmail_PhySuspendResumeLogin(void)
{   
    strcpy(g_sysmail_node.module_name, "SYSMAIL");    
    g_sysmail_node.reg_suspend = (pfunc_void)sysmail_PhyRegSave;    
    g_sysmail_node.reg_resume = (pfunc_void)sysmail_PhyRegResume;     
    Slp_LpsSuspendLogin(&g_sysmail_node); 
}   

void sysmail_PhySuspendResumeLogout(void)
{
    Slp_LpsSuspendLogout(&g_sysmail_node);
}

void ipc_notify_sim_detect(int simId, BOOLEAN connect)
{
    if (g_mdev == NULL)
        return;
    SCI_DisableIRQ();
    SCI_TRACE_LOW("ipc_notify_sim_detect, simId:%d,connect:%d",simId,connect);
    if (simId == 0)
    {
        g_mdev->regs->SYSMAIL85 = connect ? 1 : 0;
        IPC_SET_CP_IRQ(g_mdev,AP_NOTIFY_CP_SIM1_DETECT);
    }
    else if (simId == 1)
    {
         g_mdev->regs->SYSMAIL86 = connect ? 1 : 0;
        IPC_SET_CP_IRQ(g_mdev,AP_NOTIFY_CP_SIM2_DETECT);
    }

    SCI_RestoreIRQ();
}