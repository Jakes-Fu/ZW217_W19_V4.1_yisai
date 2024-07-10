/*
 * md_ipc.h - A header file of modem of RDA
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

#ifndef _RDA_MD_IPC_H_
#define _RDA_MD_IPC_H_

#include "ipc_drvapi.h"


#ifndef BIT
#define BIT(x) (1 << (x))
#endif
/*
 * SMD Channel
 */
#define SMD_CH_STREAM_TYPE   1
#define SMD_CH_PACKET_TYPE   2
#define SMD_CH_QUEUE_TYPE    3
#define SMD_CH_SYSCMD_TYPE   4


/*
 * Channel Event
 */
#define CH_READ_AVAIL        0x1
#define CH_WRITE_AVAIL       0x2
#define CH_OPENED            0x4
#define CH_RW_MASK           (CH_READ_AVAIL | CH_WRITE_AVAIL)

/*
 * PS Channel Info
 */
#define PS_STAT_UL_BUSY      0x1
#define PS_STAT_UL_IDLE      0x2

#define PS_BUF_FULL          BIT(0) // no free buf now
#define PS_UL_BUSY           BIT(1) // cp set ul busy flag

/*
 * Share Memory Layout
 */
//#define PS_BUF_LEN_B         1600
//#define PS_BUF_LEN_L         300
//#define PS_BUF_RESERVED_LEN  100  /* only for ul */
//#define PS_LIT_BUF_MAX_LEN   100

/* PS Packet Data define */
#define PS_DATA_LEN_B        1500
#define PS_DATA_LEN_L        100

/* PS Packet For AP-CP */
#define PS_BUF_UL_LEN_B      (PS_UL_HDR_LEN+PS_DATA_LEN_B+28) 	    // 1540->1568
#define PS_BUF_UL_LEN_L      (PS_UL_HDR_LEN+PS_DATA_LEN_L+20) 	    // 140->160
#define PS_BUF_DL_LEN_B      (PS_DL_HDR_LEN_B+PS_DATA_LEN_B+18)     // 1582->1600 
#define PS_BUF_DL_LEN_L	     (PS_DL_HDR_LEN_L+PS_DATA_LEN_L+10)     // 310->320

#define IPC_CMD_SIZE         16
#define IPC_CMD_QUEUE_LEN    8
#define MD_VERSION_SIZE      40
#define MD_EXEC_CAUSE_SIZE   256

#define AT2_BUF_SIZE         1024
#define PS_CTRL_BUF_SIZE     2048
#define A2C_CTRL_BUF_SIZE    (IPC_CMD_QUEUE_LEN * IPC_CMD_SIZE)
#if 1
#define AT1_BUF_SIZE 512 //1024
#define GNSS_LOW_BUF_SIZE 512
#define GNSS_HIGH_BUF_SIZE 2048 //512
#define WIFI_SCAN_BUF_SIZE 512
#else
#define AT1_BUF_SIZE         1024
#define AUD_CTRL_BUF_SIZE    512
#endif


/* Channel ctrl info */
#define AT1_UL_CTRL_OFFSET   0x0
#define AT1_DL_CTRL_OFFSET   0x2C
#define AT2_UL_CTRL_OFFSET   0x58
#define AT2_DL_CTRL_OFFSET   0x84
#define PS_UL_CTRL_OFFSET    0xB0
#define PS_DL_CTRL_OFFSET    0xDC
#define A2C_UL_CTRL_OFFSET   0x108
#define A2C_DL_CTRL_OFFSET   0x134
#if 1
#define GNSS_LOW_UL_CTRL_OFFSET 0x160
#define GNSS_LOW_DL_CTRL_OFFSET 0x18C
#define GNSS_HIGH_UL_CTRL_OFFSET 0x1B8
#define GNSS_HIGH_DL_CTRL_OFFSET 0x1E4
#define WIFI_SCAN_UL_CTRL_OFFSET 0x210
#define WIFI_SCAN_DL_CTRL_OFFSET 0x23C
#else
#define AUD_UL_CTRL_OFFSET 0x160
#define AUD_DL_CTRL_OFFSET 0x18C
#endif

/* Channel data buf */
#define AT1_UL_DATA_OFFSET   0x800
#define AT1_DL_DATA_OFFSET   (AT1_UL_DATA_OFFSET + AT1_BUF_SIZE)
#define AT2_UL_DATA_OFFSET   (AT1_DL_DATA_OFFSET + AT1_BUF_SIZE)
#define AT2_DL_DATA_OFFSET   (AT2_UL_DATA_OFFSET + AT2_BUF_SIZE)
#define A2C_UL_DATA_OFFSET   (AT2_DL_DATA_OFFSET + AT2_BUF_SIZE)
#define A2C_DL_DATA_OFFSET   (A2C_UL_DATA_OFFSET + A2C_CTRL_BUF_SIZE)
#if 1
#define GNSS_LOW_UL_DATA_OFFSET (A2C_DL_DATA_OFFSET + A2C_CTRL_BUF_SIZE)
#define GNSS_LOW_DL_DATA_OFFSET (GNSS_LOW_UL_DATA_OFFSET + GNSS_LOW_BUF_SIZE)
#define GNSS_HIGH_UL_DATA_OFFSET (GNSS_LOW_DL_DATA_OFFSET + GNSS_LOW_BUF_SIZE)
#define GNSS_HIGH_DL_DATA_OFFSET (GNSS_HIGH_UL_DATA_OFFSET + GNSS_HIGH_BUF_SIZE)
#define WIFI_SCAN_UL_DATA_OFFSET (GNSS_HIGH_DL_DATA_OFFSET + GNSS_HIGH_BUF_SIZE)
#define WIFI_SCAN_DL_DATA_OFFSET (WIFI_SCAN_UL_DATA_OFFSET + WIFI_SCAN_BUF_SIZE)
#define PS_UL_CTRL_DATA_OFFSET (WIFI_SCAN_DL_DATA_OFFSET + WIFI_SCAN_BUF_SIZE + 128)
#else
#define AUD_UL_DATA_OFFSET   (A2C_DL_DATA_OFFSET + A2C_CTRL_BUF_SIZE)
#define AUD_DL_DATA_OFFSET   (AUD_UL_DATA_OFFSET + AUD_CTRL_BUF_SIZE)
#define PS_UL_CTRL_DATA_OFFSET  (AUD_DL_DATA_OFFSET + AUD_CTRL_BUF_SIZE + 128)
#endif
#define PS_DL_CTRL_DATA_OFFSET  (PS_UL_CTRL_DATA_OFFSET + PS_CTRL_BUF_SIZE)

/*
 * IRQ Bit
 */
#define AT1_READY_BIT BIT(0)            // CP->AP, AP->CP
#define AT2_READY_BIT BIT(1)            // CP->AP, AP->CP
#define PS_READY_BIT BIT(2)             // CP->AP, AP->CP
#define A2C_CTRL_READY_BIT BIT(3)       // CP->AP, AP->CP
#define MD_EXECPTION_BIT BIT(5)        // CP->AP
#define MD_IP_ADDR_ADD_BIT BIT(6)      // CP->AP
#define MD_IP_ADDR_DEL_BIT BIT(7)      // CP->AP
#define MD_TRACE_CTRL_BIT BIT(8)       // CP->AP
#define CP_LPS_READY_BIT BIT(10)       // CP->AP
//#define MD_SYSCORE_RESUME    BIT(10)

#define CPRF_PARA_STORE_BIT BIT(17)    // CP->AP
//#define CP_NOTIFY_AP_RESET_BIT BITS(18) // CP->AP
#define MD_MOS_CTRL_BIT BIT(19)        // CP->AP
#if 1 //def CONFIG_SOC_8850
#define GNSS_LOW_READY_BIT BIT(4)   // CP->AP, AP->CP
#define GNSS_HIGH_WRITE_BIT BIT(5)  // AP->CP
#define WIFI_SCAN_WRITE_BIT BIT(6)  // AP->CP
#define VOLTE_DATA_READY_BIT BIT(19) // AP->CP
#define GNSS_HIGH_READY_BIT BIT(20) // CP->AP
#define WIFI_SCAN_READY_BIT BIT(21) // CP->AP
#else
#define AUD_CTRL_READY_BIT BIT(4) // CP->AP, AP->CP
#endif

#define AP_NOTIFY_ZSP_EXEC_BIT BIT(2) // AP->ZSP
#define ZSP_TRACE_CTRL_BIT BIT(9)     // ZSP->AP

#define AP_NOTIFY_CP_EXEC_BIT BIT(7)    // AP->CP
#define AP_NOTIFY_CP_STOPLOG_BIT BIT(9) // AP->CP
#define ZSP_MUSIC_NOTIFY BIT(11)    // ZSP->AP
#define ZSP_SBC_NOTIFY       BIT(12)

#define AP_NOTIFY_CP_SIM1_DETECT BIT(17) // AP->CP
#define AP_NOTIFY_CP_SIM2_DETECT BIT(18) // AP->CP

#define RISC_BT_LOG_NOTIFY BIT(13)
#define RISC_BT_ASSERT_NOTIFY BIT(14)
#define RISC_BT_NOTIFY_AP_WAKEUP BIT(15)
#define RISC_BT_NOTIFY_AP_SLEEP BIT(16)


#define AP_NOTIFY_ZSP_EXEC_BIT   BIT(2)

#if 1 
#define IRQ_MASK         (AT1_READY_BIT | AT2_READY_BIT | PS_READY_BIT | A2C_CTRL_READY_BIT | GNSS_LOW_READY_BIT |GNSS_HIGH_READY_BIT |WIFI_SCAN_READY_BIT)
#else
#define IRQ_MASK         (AT1_READY_BIT | AT2_READY_BIT | PS_READY_BIT | A2C_CTRL_READY_BIT | AUD_CTRL_READY_BIT)
#endif
/*
 * CTRL CMD ID
 */
#define C2A_DYNAMIC_NVM_UPDATE  0x1
#define C2A_STATIC_NVM_UPDATE   0x2
#define C2A_PSM_RTC_TIME        0x3
#define C2A_CSW_DATA_UPDATE     0x4
#define C2A_RF_CALIB_UPDATE     0x5
#define C2A_AUD_CALIB_UPDATE    0x6
#define C2A_PHY_DUMP_DATA_SAVE  0x7

#define AP_NOTIFY_CP_ASSERT     0x1
#define AP_NOTIFY_ZSP_ASSERT    0x2

#define MD_SYS_MAGIC    0xA8B1

#define BUF_IN_USE      (0xAAAAAAAA)
#define BUF_IN_IDLE     (0x55555555)

#define IP_ADDR_ONLY_IPV4	1
#define IP_ADDR_IPV4V6		2
#define IP_ADDR_ONLY_IPV6	3

#define  SYSMAIL_SAVE_REG_NUM     (1)


struct track_info {
    uint32 widx;
    uint32 mailbox[256];
    uint32 time[256];
};

struct md_dev {
	struct smd_ch chs[SMD_CH_MAX];

	HWP_CP_MAILBOX_T *regs;
	uint8 *sm_base;
	uint32 sm_len;

	//struct work_struct notify_work;
	//spinlock_t irq_lock;
	int irq;
	uint32 ul_big_ps_buf_cnt;
	uint32 ul_lit_ps_buf_cnt;
	uint32 dl_big_ps_buf_cnt;
	uint32 dl_lit_ps_buf_cnt;
	uint32 md_version_off;
	uint32 md_exec_cause_off;

	uint32 ip_type;
	uint32 md_ip_addr_status;
	uint32 md_ip_addr;
	uint32 md_dns_addr;

	uint32 ul_big_start;
	uint32 ul_big_end;
	uint32 ul_lit_start;
	uint32 ul_lit_end;
	uint32 dl_big_start;
	uint32 dl_big_end;
	uint32 dl_lit_start;
	uint32 dl_lit_end;
	uint32 sbc_buf_off;
	uint32 delta_nv_off;
	uint32 ims_nv_info_off;
    uint32 bt_rf_sm_off;

	uint32 md_ipv6_addr_status;
	uint32 md_ipv6_addr_l[2];
	uint32 md_dnsv6_addr[4];

	void (*exec_handler)(void);	/* Modem execption handler */
	void (*trace_handler)(uint32 handler); /* CP Trace handler */
	void (*music_handler)(void);

	void *pdata;
};

enum ip_addr_status {
	IP_ADDR_NOCHANGE = 0,
	IP_ADDR_CHANGED = 1,
	IP_ADDR_READY = 2,
	IP_ADDR_DELETE = 3,
};

struct HAL_IP_SUSPEND_RESUME_T            
{
    char module_name[10];
    void (*reg_suspend)();                       //func addr of reg save
    void (*reg_resume)();                        //func addr of reg resume
    struct HAL_IP_SUSPEND_RESUME_T *next;        //next node addr
};

#if 0
extern uint8 *rda_sm_base;
extern uint8 *rda_sm_phys;
extern struct smd_ch *g_ps_ch;

extern int32 ipc_ch_open(int32 ch_id, struct smd_ch **ch, void *priv,
			void (*notify)(void *, uint32));
extern int32 ipc_ch_read(struct smd_ch *ch, void *data, uint32 len);
extern int32 ipc_ch_write(struct smd_ch *ch, const void *data, uint32 len);
extern int32 ipc_ch_close(struct smd_ch *ch);
extern int32 ipc_ch_read_avail(struct smd_ch *ch);
extern int32 ipc_ch_write_avail(struct smd_ch *ch);

extern BOOLEAN ipc_ps_buf_full(struct smd_ch *ch);
extern BOOLEAN ipc_ps_ul_idle(struct smd_ch *ch);
extern void ipc_reset_ps_buf(struct smd_ch *ch);
extern void ipc_set_ps_ul_stat(struct smd_ch *ch, uint32 stat);
extern int32 ipc_alloc_ul_ps_buf(struct smd_ch *ch, uint32 size);
extern int32 ipc_free_dl_ps_buf(struct smd_ch *ch, uint32 offset);

extern void ipc_disable_ch_irq(struct smd_ch *ch);
extern void ipc_enable_ch_irq(struct smd_ch *ch);
extern void ipc_enable_ravail_event(struct smd_ch *ch);
extern void ipc_disable_ravail_event(struct smd_ch *ch);
extern void ipc_enable_wavail_event(struct smd_ch *ch);
extern void ipc_disable_wavail_event(struct smd_ch *ch);
extern void ipc_register_exec_handler(void (*handler)(void));
extern uint32 ipc_get_ch_status(struct smd_ch *ch);
extern uint32 ipc_get_cp_exec_type(void);
extern uint32 ipc_get_zsp_exec_type(void);
extern void ipc_ap_execption_notify(void);
extern void ipc_register_trace_handler(void (*handler) (uint32));

extern uint32 get_dl_free_buf_cnt(struct smd_ch *ch);
extern BOOLEAN ipc_ps_ul_opened(void);
extern BOOLEAN ipc_ps_ul_free(uint32 size);
extern void pmic_set_soft_reset(void);
extern void ipc_get_ip_dns_addr(struct ip_info *info);
extern void ipc_set_sbc_buf_addr(uint32 addr);
extern uint32 ipc_get_modem_version(void);
extern void ipc_notify_cp_stop_log(uint32 is_off);
#endif
void ipc_notify_volte_data(void);
typedef void (*pfunc_void)(void);
#endif
