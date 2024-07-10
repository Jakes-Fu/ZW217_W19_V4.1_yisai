
#ifndef _IPC_DRVAPI_H_
#define _IPC_DRVAPI_H_

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**
 * @brief IPC channel ID
 */
enum smd_ch_id
{
    SMD_CH_AT = 1,       ///< alias for SMD_CH_AT1
    SMD_CH_AT1 = 1,      ///< AT1, stream type
    SMD_CH_AT2 = 2,      ///< AT2, stream type
    SMD_CH_PS = 3,       ///< PS, packet type
    SMD_CH_A2C_CTRL = 4, ///< A2C_CTRL, queue type
    SMD_CH_GNSS_LOW = 5,  ///< GNSS LowPro, stream type
    SMD_CH_GNSS_HIGH = 6, ///< GNSS HighPro, stream type
    SMD_CH_WIFI_SCAN = 7, ///< WIFI SCAN, stream type
    SMD_CH_MAX ///< placeholder for count
};


// nv cmd id.
// CFW NV
#define NV_ID_CFW_NV        1

// SMS DM
#define NV_ID_SMS_DM        2

// 4G static NV
#define NV_ID_STATIC_NV     3
// 4G dynamic NV
#define NV_ID_DYNAMIC_NV    4
// 4G psm NV
#define NV_ID_PSM_NV        5

// lte calib
#define NV_ID_LTE_CALIB     6

// gge calib
#define NV_ID_GSM_CALIB     7

// audio calib
#define NV_ID_AUD_CALIB     8

// factory data
#define NV_ID_PHY_NV        9

// VOLTE NV
#define NV_ID_GOLDEN_BOARD  10

// RF NV
#define NV_ID_RF_NV         11

// 4G static NV for sim2
#define NV_ID_STATIC_NV_2   12

// 4G dynamic NV for sim2
#define NV_ID_DYNAMIC_NV_2  13

// 4G dynamic NV for sim2
#define NV_ID_PHY_NV_2      14

#define NV_ID_MAX           16

// NV channel queue size
#define IPC_CMD_QUEUE_LEN    8

// NV cmd id
#define NV_CMD_CREATE        1
#define NV_CMD_OPEN          2
#define NV_CMD_READ          3
#define NV_CMD_WRITE         4

#define NV_CMD_CALIB         5

#define NV_CMD_IWRITE        6

// NV process result
#define NV_CMD_SUCCESS       0
#define NV_CMD_FAILURE       0xFF


/* PS Packet Header define */
#define PS_HEADER_SIZE       20
#define PS_UL_ROHC_LEN       18
#define PS_UL_PDCP_LEN       2
#define PS_RNDIS_H_LEN       58
#define PS_DL_ROHC_LEN       128
#define PS_UL_RESERVED       0
#define PS_DL_RESERVED       4 // ps_header->id for dl debug

#define PS_UL_HDR_LEN        \
	(PS_HEADER_SIZE+PS_UL_ROHC_LEN+PS_UL_PDCP_LEN+PS_UL_RESERVED)
#define PS_DL_HDR_LEN_B      \
	(PS_HEADER_SIZE+PS_RNDIS_H_LEN+PS_DL_RESERVED)
#define PS_DL_HDR_LEN_L      \
	(PS_HEADER_SIZE+PS_RNDIS_H_LEN+PS_DL_ROHC_LEN+PS_DL_RESERVED)

#define IP_ADDR_ONLY_IPV4	1
#define IP_ADDR_IPV4V6		2
#define IP_ADDR_ONLY_IPV6	3


struct ps_header {
	uint32 next;
	uint8 cid;
	uint8 simid;
	uint16 len;
	uint32 flag;      /* BUF_IN_USR or BUF_IN_IDLE */
	uint32 buf_size;  /* 300B or 1600B */
	uint32 data_off;  /* data off in buffer */
	uint32 id;
};

struct ipc_cmd {
	uint32 id;
	uint32 para0;
	uint32 para1;
	uint32 para2;
};

struct md_sys_hdr {
	unsigned short magic;
	unsigned short mod_id;
	unsigned int req_id;
	unsigned int ret_val;
	unsigned short msg_id;
	unsigned short ext_len;
};

struct md_sys_hdr_ext {
	unsigned short magic;
	unsigned short mod_id;
	unsigned int req_id;
	unsigned int ret_val;
	unsigned short msg_id;
	unsigned short ext_len;
	unsigned char ext_data[];
};

struct ch_buf_ctrl {
	uint32 head;
	uint32 tail;
	uint32 status;
	uint32 alloc_net;
	uint32 free_net;
	uint32 alloc_vol;
	uint32 free_vol;
	uint32 alloc_net_l;
	uint32 free_net_l;
	uint32 alloc_vol_l;
	uint32 free_vol_l;
};

struct ch_buf {
	struct ch_buf_ctrl *pctrl;
	uint8 *pfifo;
	uint32 fifo_size;
	uint32 fifo_mask;
	uint32 queue_len;
	uint32 *net_free_buf_b;
	uint32 *net_free_buf_l;
	uint32 *vol_free_buf_b;
	uint32 *vol_free_buf_l;

	//spinlock_t lock;
	SCI_SEMAPHORE_PTR ch_lock;
	void *ch;
};

struct smd_ch {
	uint32 id;
	uint32 type;
	uint32 opened;

	struct ch_buf ul;	/* Send to Net */
	struct ch_buf dl;	/* Recv from Net */

	//spinlock_t notify_lock;
	void (*notify)(void *priv, uint32 flags);

	int32(*read)(struct ch_buf *buf, void *data, uint32 len);
	int32(*write)(struct ch_buf *buf, const void *data, uint32 len);
	int32(*read_avail)(struct ch_buf *buf);
	int32(*write_avail)(struct ch_buf *buf);
	int32(*alloc_ul_psbuf)(struct ch_buf *buf, uint32 size);
	int32(*free_dl_psbuf)(struct ch_buf *buf, uint32 offset);

	uint32 event_mask;
	void (*notify_other_cpu)(void);

	void *priv;
	void *pmd;
};

struct ip_info {
	uint32 ip_type;
	uint32 ipv4_addr;
	uint32 dnsv4_primary;
	uint32 dnsv4_auxiliary;
	uint8 ipv6_intf_id[8];
	uint8 dnsv6_primary[16];
	uint8 dnsv6_auxiliary[16];
};

extern uint8 *rda_sm_base;

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
extern void ipc_register_music_notify(void (*handler) (void));
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

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
