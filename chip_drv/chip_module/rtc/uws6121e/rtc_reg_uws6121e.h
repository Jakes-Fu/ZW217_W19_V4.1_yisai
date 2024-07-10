/******************************************************************************
 ** File Name:    rtc_reg_uws6121e.h                                            *
 ** Author:       mingwei.zhang                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    mingwei.zhang   Create.                                     *
 ******************************************************************************/
#ifndef _RTC_REG_UWS6121E_H_
#define _RTC_REG_UWS6121E_H_

// =============================================================================
//  MACROS
// =============================================================================
#define   RDA2720_RTC_BASE_ADDR     (0x600)        /*rda2720 rtc?¨´¦Ì??¡¤*/

typedef enum {
    RDA2720_RTC_SEC_CNT_VALUE                   = 0x00,
    RDA2720_RTC_MIN_CNT_VALUE                   = 0x04,
    RDA2720_RTC_HRS_CNT_VALUE                   = 0x08,
    RDA2720_RTC_DAY_CNT_VALUE                   = 0x0C,
    RDA2720_RTC_SEC_CNT_UPD                     = 0x10,
    RDA2720_RTC_MIN_CNT_UPD                     = 0x14,
    RDA2720_RTC_HRS_CNT_UPD                     = 0x18,
    RDA2720_RTC_DAY_CNT_UPD                     = 0x1C,
    RDA2720_RTC_SEC_ALM_UPD                     = 0x20,
    RDA2720_RTC_MIN_ALM_UPD                     = 0x24,
    RDA2720_RTC_HRS_ALM_UPD                     = 0x28,
    RDA2720_RTC_DAY_ALM_UPD                     = 0x2C,
    RDA2720_RTC_INT_EN                          = 0x30,
    RDA2720_RTC_INT_RAW_STS                     = 0x34,
    RDA2720_RTC_INT_CLR                         = 0x38,
    RDA2720_RTC_INT_MASK_STS                    = 0x3C,
    RDA2720_RTC_SEC_ALM_VALUE                   = 0x40,
    RDA2720_RTC_MIN_ALM_VALUE                   = 0x44,
    RDA2720_RTC_HRS_ALM_VALUE                   = 0x48,
    RDA2720_RTC_DAY_ALM_VALUE                   = 0x4C,
    RDA2720_RTC_SPG_VALUE                       = 0x50,
    RDA2720_RTC_SPG_UPD                         = 0x54,
    RDA2720_RTC_PWR_FLAG_CTRL                   = 0x58,
    RDA2720_RTC_PWR_FLAG_STS                    = 0x5C,
    RDA2720_RTC_SEC_AUXALM_UPD                  = 0x60,
    RDA2720_RTC_MIN_AUXALM_UPD                  = 0x64,
    RDA2720_RTC_HRS_AUXALM_UPD                  = 0x68,
    RDA2720_RTC_DAY_AUXALM_UPD                  = 0x6C,
    RDA2720_RTC_SEC_CNT_RAW                     = 0x70,
    RDA2720_RTC_MIN_CNT_RAW                     = 0x74,
    RDA2720_RTC_HRS_CNT_RAW                     = 0x78,
    RDA2720_RTC_DAY_CNT_RAW                     = 0x7C,
} RDA2720_RTC_REG_MAP_T;


//RDA2720_RTC_BASE_ADDR+RDA2720_RTC_SPG_UPD


#define RDA2720_RTC_INT_ALL_MSK (0xFFFF&(~(BIT_5|BIT_6|BIT_7)))



#define RDA2720_RTC_SEC_BIT                 BIT_0        //Sec int enable
#define RDA2720_RTC_MIN_BIT                 BIT_1        //Min int enable
#define RDA2720_RTC_HOUR_BIT                BIT_2        //Hour int enable
#define RDA2720_RTC_DAY_BIT                 BIT_3        //Day int enable
#define RDA2720_RTC_ALARM_BIT               BIT_4        //Alarm int enable
#define RDA2720_RTCCTL_HOUR_FMT_SEL         BIT_5        //Hour format select


//The corresponding bit of RTC_CTL register.
#define RTC_SEC_BIT                 BIT_0        //Sec int enable
#define RTC_MIN_BIT                 BIT_1        //Min int enable
#define RTC_HOUR_BIT                BIT_2        //Hour int enable
#define RTC_DAY_BIT                 BIT_3        //Day int enable
#define RTC_ALARM_BIT               BIT_4        //Alarm int enable
#define RTCCTL_HOUR_FMT_SEL         BIT_5        //Hour format select


#define RTC_SEC_ACK_BIT             BIT_8        // Sec ack int enable
#define RTC_MIN_ACK_BIT             BIT_9        //Min ack int enable
#define RTC_HOUR_ACK_BIT            BIT_10        //Hour ack int enable
#define RTC_DAY_ACK_BIT             BIT_11        //Day ack int enable
#define RTC_SEC_ALM_ACK_BIT         BIT_12        //Sec alm ack int enable
#define RTC_MIN_ALM_ACK_BIT         BIT_13        //Min alm ack int enable
#define RTC_HOUR_ALM_ACK_BIT        BIT_14        //Hour alm ack int enable
#define RTC_DAY_ALM_ACK_BIT         BIT_15        //Day alm ack int enable

#define RTC_INT_ALL_MSK             (0xFFFF&(~(BIT_5|BIT_6|BIT_7)))
#define SPRD_RTC_UNLOCK             (0xa5)
#define SPRD_RTC_LOCK               (SPRD_RTC_UNLOCK - 1)

#define SPRD_RTC_SPARE      0xbb

#endif



