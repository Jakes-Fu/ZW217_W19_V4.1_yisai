#ifndef _TROUT_FM_REG_DEF_H_
#define _TROUT_FM_REG_DEF_H_

#include "sci_types.h"

#define FM_CTL_STI_MODE_NORMAL		0x0
#define	FM_CTL_STI_MODE_SEEK		0x1
#define	FM_CTL_STI_MODE_TUNE		0x2

#define 	REG_AON_APB_EB0 			(CTL_BASE_GLOABL+0xA8)
#define 	REG_AON_APB_EB1 			(CTL_BASE_GLOABL+0xAC)
#define 	REG_AON_APB_RST0 		(CTL_BASE_GLOABL+0x68)
#define   REG_APB_FM_RF1 		(CTL_BASE_GLOABL+0x2C8)
#define   REG_APB_ANALOG_BB_TOP3 		(CTL_BASE_GLOABL+0x2E4)

#define 	FM_REG_FM_CTL 			(CTL_BASE_FM)
#define 	FM_REG_FM_EN 			(CTL_BASE_FM + 0x04)
#define 	FM_REG_RF_INIT_GAIN 	(CTL_BASE_FM + 0x08)
#define 	FM_REG_CHAN 			(CTL_BASE_FM + 0x0C)
#define 	FM_REG_AGC_TBL_CLK 		(CTL_BASE_FM + 0x10)
#define 	FM_REG_SEEK_LOOP 		(CTL_BASE_FM + 0x14)
#define 	FM_REG_FM_CTL_STI 		(CTL_BASE_FM + 0x18)
#define 	FM_REG_BAND_LMT 		(CTL_BASE_FM + 0x1C)
#define 	FM_REG_BAND_SPACE 		(CTL_BASE_FM + 0x20)
#define 	FM_REG_RFCTRL0 			(CTL_BASE_FM + 0x24)
#define 	FM_REG_ADC_BITMAP		(CTL_BASE_FM + 0x28)

#define 	FM_REG_INT_EN 			(CTL_BASE_FM + 0x30)
#define 	FM_REG_INT_CLR 			(CTL_BASE_FM + 0x34)
#define 	FM_REG_INT_STS 			(CTL_BASE_FM + 0x38)

#define 	FM_REG_ADC_INFCTRL 		(CTL_BASE_FM + 0x3C)

#define 	FM_REG_SEEK_CH_TH 		(CTL_BASE_FM + 0x40)
#define 	FM_REG_AGC_TBL_RFRSSI0 	(CTL_BASE_FM + 0x44)
#define 	FM_REG_AGC_TBL_RFRSSI1 	(CTL_BASE_FM + 0x48)
#define 	FM_REG_AGC_TBL_WBRSSI 	(CTL_BASE_FM + 0x4C)
#define 	FM_REG_AGC_IDX_TH 		(CTL_BASE_FM + 0x50)
#define 	FM_REG_AGC_RSSI_TH 		(CTL_BASE_FM + 0x54)
#define 	FM_REG_SEEK_ADPS 		(CTL_BASE_FM + 0x58)
#define 	FM_REG_STER_PWR 		(CTL_BASE_FM + 0x5C)
#define 	FM_REG_AGC_CTL 			(CTL_BASE_FM + 0x60)
#define 	FM_REG_AGC_ITV_TH 		(CTL_BASE_FM + 0x64)
#define 	FM_REG_AGC_ADPS0 		(CTL_BASE_FM + 0x68)
#define 	FM_REG_AGC_ADPS1 		(CTL_BASE_FM + 0x6C)
#define 	FM_REG_PDP_TH 			(CTL_BASE_FM + 0x70)
#define 	FM_REG_PDP_DEV 			(CTL_BASE_FM + 0x74)
#define 	FM_REG_ADP_ST_CONF 		(CTL_BASE_FM + 0x78)
#define 	FM_REG_ADP_LPF_CONF 	(CTL_BASE_FM + 0x7C)
#define 	FM_REG_DEPHA_SCAL 		(CTL_BASE_FM + 0x80)
#define 	FM_REG_HW_MUTE 			(CTL_BASE_FM + 0x84)
#define 	FM_REG_SW_MUTE 			(CTL_BASE_FM + 0x88)
#define 	FM_REG_UPD_CTRL 		(CTL_BASE_FM + 0x8C)
#define 	FM_REG_AUD_BLD0 		(CTL_BASE_FM + 0x90)
#define 	FM_REG_AUD_BLD1 		(CTL_BASE_FM + 0x94)
#define 	FM_REG_AGC_HYS 			(CTL_BASE_FM + 0x98)
#define 	FM_REG_MONO_PWR 		(CTL_BASE_FM + 0x9C)
#define 	FM_REG_RF_CFG_DLY 		(CTL_BASE_FM + 0xA0)

#define 	FM_REG_AGC_TBL_STS 		(CTL_BASE_FM + 0xA4)
#define 	FM_REG_ADP_BIT_SFT 		(CTL_BASE_FM + 0xA8)
#define 	FM_REG_SEEK_CNT 		(CTL_BASE_FM + 0xAC)
#define 	FM_REG_RSSI_STS 		(CTL_BASE_FM + 0xB0)

#define 	FM_REG_CHAN_FREQ_STS 	(CTL_BASE_FM + 0xB8)
#define 	FM_REG_FREQ_OFF_STS 	(CTL_BASE_FM + 0xBC)
#define 	FM_REG_INPWR_STS 		(CTL_BASE_FM + 0xC0)
#define 	FM_REG_RF_RSSI_STS 		(CTL_BASE_FM + 0xC4)
#define 	FM_REG_NO_LPF_STS 		(CTL_BASE_FM + 0xC8)
#define 	FM_REG_SMUTE_STS 		(CTL_BASE_FM + 0xCC)
#define 	FM_REG_MISC_STS 		(CTL_BASE_FM + 0xD0)
#define 	FM_REG_PILOT_STS 		(CTL_BASE_FM + 0xD4)
#define 	FM_REG_K1_VALUE 		(CTL_BASE_FM + 0xD8)
#define 	FM_REG_K2_VALUE 		(CTL_BASE_FM + 0xDC)
#define 	FM_REG_RC_DC_CALI 		(CTL_BASE_FM + 0xE0)


//spur removal control reg
#define 	FM_REG_SPUR_FREQ0 		(CTL_BASE_FM + 0x100)
#define 	FM_REG_SPUR_FREQ1 		(CTL_BASE_FM + 0x104)
//iis ctl reg
#define 	FM_REG_IIS_CONFIG1 		(CTL_BASE_FM + 0x108)
#define 	FM_REG_IIS_CONFIG2 		(CTL_BASE_FM + 0x10C)
#define 	FM_REG_IIS_CONFIG3 		(CTL_BASE_FM + 0x110)
#define 	FM_REG_IIS_CONFIG4 		(CTL_BASE_FM + 0x114)
//sinc filter ctl reg
#define 	FM_REG_CIC16_CONFIG1 	(CTL_BASE_FM + 0x118)
#define 	FM_REG_CIC16_CONFIG2 	(CTL_BASE_FM + 0x11C)
#define 	FM_REG_CIC16_FREQ 		(CTL_BASE_FM + 0x120)
//fm ctl reg1
#define 	FM_REG_REG_CTRL1 		(CTL_BASE_FM + 0x124)

#define 	FM_REG_AMP_ADJ	 		(CTL_BASE_FM + 0x128)
#define 	FM_REG_IQ_CALI	 		(CTL_BASE_FM + 0x12C)
#define 	FM_REG_SIN_GEN	 		(CTL_BASE_FM + 0x130)
#define 	FM_REG_CALI_CTL	 		(CTL_BASE_FM + 0x134)
//rf ctl reg
#define 	FM_REG_RFCTRL1 			(CTL_BASE_FM + 0x13C)
#define 	FM_REG_RFCTRL2 			(CTL_BASE_FM + 0x140)
#define 	FM_REG_RFCTRL3 			(CTL_BASE_FM + 0x144)

#define 	FM_REG_DB_COMP 			(CTL_BASE_FM + 0x160)
#define	FM_SOFTMUTE_CTRL0		(CTL_BASE_FM + 0x164)
#define	FM_SOFTMUTE_CTRL1		(CTL_BASE_FM + 0x168)
#define	FM_SOFTMUTE_CTRL2		(CTL_BASE_FM + 0x16C)
#define	FM_SOFTMUTE_CTRL3		(CTL_BASE_FM + 0x170)
//agc table
#define 	FM_REG_AGC_TBL 			(CTL_BASE_FM + 0x400)

#define 	FM_RDS_REG_BASE 		(CTL_BASE_FM + 0x180)
#define 	FM_RDS_BUF 				(FM_RDS_REG_BASE + 0x0)	//180
#define 	FM_RDS_REG0 			(FM_RDS_REG_BASE + 0x40)//1c0
#define 	FM_RDS_REG1 			(FM_RDS_REG_BASE + 0x44)//1c4
#define 	FM_RDS_REG2 			(FM_RDS_REG_BASE + 0x48)//1c8
#define 	FM_RDS_REG3 			(FM_RDS_REG_BASE + 0x4C)//1cc
#define 	FM_RDS_REG4 			(FM_RDS_REG_BASE + 0x50)//1d0
#define 	FM_RDS_REG5 			(FM_RDS_REG_BASE + 0x54)//1d4
#define 	FM_RDS_REG6 			(FM_RDS_REG_BASE + 0x58)//1d8
#define 	FM_RDS_REG7 			(FM_RDS_REG_BASE + 0x5C)//1dc
#define 	FM_RDS_REG8 			(FM_RDS_REG_BASE + 0x60)//1e0
#define 	FM_RDS_REG9				(FM_RDS_REG_BASE + 0x64)//1e4
#define 	FM_RDS_REG10 			(FM_RDS_REG_BASE + 0x68)//1e8
#define 	FM_RDS_REG11 			(FM_RDS_REG_BASE + 0x6C)//1ec
#define 	FM_RDS_REG12 			(FM_RDS_REG_BASE + 0x70)//1f0
#define 	FM_RDS_REG13 			(FM_RDS_REG_BASE + 0x74)//1f4
#define 	FM_RDS_REG14 			(FM_RDS_REG_BASE + 0x78)//1f8
#define 	FM_RDS_REG15 			(FM_RDS_REG_BASE + 0x7c)//1fc
#define 	FM_RDS_REG16 			(FM_RDS_REG_BASE + 0x80)//200
#define 	FM_RDS_REG17 			(FM_RDS_REG_BASE + 0x84)//204

#define 	FM_RDS_EN 				(BIT_0)
#define 	FM_RDS_MATCH_EN 		(BIT_0)
#define 	FM_RDS_COMP_EN 			(BIT_1)

#define 	FM_RDS_CONF_RSMP_EN 	(BIT_1)

#define 	FM_RDS_CONF_SAMPLE_TRACK_EN 	(BIT_0)
#define 	FM_RDS_CONF_SFA_STEP_EN 	(BIT_1)
#define 	FM_RDS_CONF_SPA_STEP_EN 	(BIT_9)

//#define 	FM_RDS_CONF_SFA_STEP_EN 	(BIT_1)
#define 	CAPTURE_DATA_NUM			(0x5000)
#define 	CTL_BASE_CAPTURE 			(0x70000000)
#define 	CAPTURE_CFG					(CTL_BASE_CAPTURE + 0x80)
#define 	CAPTURE_PACKET_LEN			(CTL_BASE_CAPTURE + 0x84)
#define 	CAPTURE_IQ_START_ADDR		(CTL_BASE_CAPTURE + 0x88)
#define 	CAPTURE_STATUS_START_ADDR	(CTL_BASE_CAPTURE + 0x8C)
#define 	CAPTURE_BUF_LEN				(CTL_BASE_CAPTURE + 0x90)
#define 	CAPTURE_WIFI2_CAL_DELAY 	(CTL_BASE_CAPTURE + 0x94)
#define 	CAPTURE_SAMPLE_ADDR			(CTL_BASE_CAPTURE + 0x98)
#define 	CAPTURE_INT_STATE			(CTL_BASE_CAPTURE + 0x9C)
#define 	CAPTURE_INT_MASK			(CTL_BASE_CAPTURE + 0xA0)
#define 	CAPTURE_INT_CLEAN			(CTL_BASE_CAPTURE + 0xA4)

#define 	WIFI_RAM_BASE				(0x40023000) //(0x70040000)

#if FM_SC6531EFM_FPGA
#define		INIT_FREQ				(980)
#else
#define		INIT_FREQ				(98000)
#endif

typedef struct
{
    uint8          search_type;
    uint8          search_direct;
    BOOLEAN 	int_happen;
    BOOLEAN		seek_success;
    uint32		freq_set;
    uint32		freq_seek;
    uint32		freq_offset;
    uint32		rssi;
    uint32		rf_rssi;
    uint32		seek_cnt;
    uint32		inpwr_sts;
    uint32		fm_sts;
    uint32      agc_sts;
	uint32      pilot_sts;
    void	(*irq_handler)(uint32);
} trout_fm_info_t;

#endif /*TROUT_FM_REG_DEF_H_*/
