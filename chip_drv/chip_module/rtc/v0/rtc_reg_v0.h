/******************************************************************************
 ** File Name:    rtc_reg_v0.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _RTC_REG_V0_H_
#define _RTC_REG_V0_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/


/*----------Real Timer Counter Register----------*/
//RTC_BASE      0x89000000
#define RTC_SECOND_CNT                  (RTC_BASE + 0x0000)
#define RTC_MINUTE_CNT                  (RTC_BASE + 0x0004)
#define RTC_HOUR_CNT                    (RTC_BASE + 0x0008)
#define RTC_DAY_CNT                     (RTC_BASE + 0x000C)
#define RTC_SECOND_UPDATE               (RTC_BASE + 0x0010)
#define RTC_MINUTE_UPDATE               (RTC_BASE + 0x0014)
#define RTC_HOUR_UPDATE                 (RTC_BASE + 0x0018)
#define RTC_DAY_UPDATE                  (RTC_BASE + 0x001C)
#define RTC_SECOND_ALM                  (RTC_BASE + 0x0020)
#define RTC_MINUTE_ALM                  (RTC_BASE + 0x0024)
#define RTC_HOUR_ALM                    (RTC_BASE + 0x0028)
#define RTC_DAY_ALM                     (RTC_BASE + 0x002C)
#define RTC_CTL                         (RTC_BASE + 0x0030)
#define RTC_STS                         (RTC_BASE + 0x0034)
#define RTC_ICLR                        (RTC_BASE + 0x0038)
#define RTC_IMSK                        (RTC_BASE + 0x003C)


//The corresponding bit of RTC_CTL register.
#define RTCCTL_SEC_IE                   (1 << 0)        //Sec int enable
#define RTCCTL_MIN_IE                   (1 << 1)        //Min int enable
#define RTCCTL_HOUR_IE                  (1 << 2)        //Hour int enable
#define RTCCTL_DAY_IE                   (1 << 3)        //Day int enable
#define RTCCTL_ALARM_IE                 (1 << 4)        //Alarm int enable
#define RTCCTL_HOUR_FMT_SEL_IE          (1 << 5)        //Hour format select
#define RTCCTL_EN                       (1 << 6)        //Rtc module enable


#define RTCCTL_SEC_ACK_IE                   (1 << 8)        // Sec ack int enable
#define RTCCTL_MIN_ACK_IE                   (1 << 9)        //Min ack int enable
#define RTCCTL_HOUR_ACK_IE                  (1 << 10)        //Hour ack int enable
#define RTCCTL_DAY_ACK_IE                   (1 << 11)        //Day ack int enable

#define RTCCTL_SEC_ALM_ACK_IE                   (1 << 12)        //Sec alm ack int enable
#define RTCCTL_MIN_ALM_ACK_IE                   (1 << 13)        //Min alm ack int enable
#define RTCCTL_HOUR_ALM_ACK_IE                  (1 << 14)        //Hour alm ack int enable
#define RTCCTL_DAY_ALM_ACK_IE                   (1 << 15)        //Day alm ack int enable



//The corresponding bit of RTC_STS register.
#define RTCSTS_SEC                      (1 << 0)        //Second int
#define RTCSTS_MIN                      (1 << 1)        //Minute int
#define RTCSTS_HOUR                     (1 << 2)        //Hour int
#define RTCSTS_DAY                      (1 << 3)        //Day int
#define RTCSTS_ALARM                    (1 << 4)        //Alarm int

#define RTCSTS_SEC_ACK                      (1 << 8)        //Second ack int 
#define RTCSTS_MIN_ACK                      (1 << 9)        //Minute ack int
#define RTCSTS_HOUR_ACK                 (1 << 10)        //Hour ack int
#define RTCSTS_DAY_ACK                      (1 << 11)        //Day ack int

#define RTCSTS_SEC_ALM_ACK                          (1 << 12)        //Second alm ack int
#define RTCSTS_MIN_ALM_ACK                          (1 << 13)        //Minute alm ack int
#define RTCSTS_HOUR_ALM_ACK                     (1 << 14)        //Hour alm ack int
#define RTCSTS_DAY_ALM_ACK                      (1 << 15)        //Day alm ack int



//The corresponding bit of RTC_ICLR register.
#define RTCICLR_SEC                     (1 << 0)        //Second int is cleared by write 1 to this bit
#define RTCICLR_MIN                     (1 << 1)        //Minute int is cleared by write 1 to this bit
#define RTCICLR_HOUR                    (1 << 2)        //Hour int is cleared by write 1 to this bit
#define RTCICLR_DAY                     (1 << 3)        //Day int is cleared by write 1 to this bit
#define RTCICLR_ALARM                   (1 << 4)        //Alarm int is cleared by write 1 to this bit

#define RTCICLR_SEC_ACK                         (1 << 8)        //Second ack int is cleared by write 1 to this bit
#define RTCICLR_MIN_ACK                         (1 << 9)        //Minute ack int is cleared by write 1 to this bit
#define RTCICLR_HOUR_ACK                    (1 << 10)        //Hour ack int is cleared by write 1 to this bit
#define RTCICLR_DAY_ACK                     (1 << 11)        //Day ack int is cleared by write 1 to this bit

#define RTCICLR_SEC_ALM_ACK                     (1 << 12)        //Second alm ack int is cleared by write 1 to this bit
#define RTCICLR_MIN_ALM_ACK                     (1 << 13)        //Minute alm ack int is cleared by write 1 to this bit
#define RTCICLR_HOUR_ALM_ACK                    (1 << 14)        //Hour alm ack  int is cleared by write 1 to this bit
#define RTCICLR_DAY_ALM_ACK                     (1 << 15)        //Day alm ack int is cleared by write 1 to this bit



//The corresponding bit of RTC_IMSK register.
#define RTCIMSK_SEC                     (1 << 0)        //      
#define RTCIMSK_MIN                     (1 << 1)        //      
#define RTCIMSK_HOUR                    (1 << 2)        //
#define RTCIMSK_DAY                     (1 << 3)        //      
#define RTCIMSK_ALARM                   (1 << 4)        //      

#define RTCIMSK_SEC_ACK                     (1 << 8)        //      
#define RTCIMSK_MIN_ACK                     (1 << 9)        //      
#define RTCIMSK_HOUR_ACK                    (1 << 10)        //
#define RTCIMSK_DAY_ACK                     (1 << 11)        //  

#define RTCIMSK_SEC_ALM_ACK                     (1 << 12)        //      
#define RTCIMSK_MIN_ALM_ACK                     (1 << 13)        //      
#define RTCIMSK_HOUR_ALM_ACK                    (1 << 14)        //
#define RTCIMSK_DAY_ALM_ACK                     (1 << 15)        //  


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
//Real Time Counter.
typedef struct rtc_tag
{
    VOLATILE uint32 sec_cnt;
    VOLATILE uint32 min_cnt;
    VOLATILE uint32 hour_cnt;
    VOLATILE uint32 day_cnt;
    VOLATILE uint32 sec_update;
    VOLATILE uint32 min_update;
    VOLATILE uint32 hour_update;
    VOLATILE uint32 day_update;
    VOLATILE uint32 sec_alm;
    VOLATILE uint32 min_alm;
    VOLATILE uint32 hour_alm;
    VOLATILE uint32 day_alm;
    VOLATILE uint32 ctl;
    VOLATILE uint32 sts;
    VOLATILE uint32 iclr;
    VOLATILE uint32 imsk;
} rtc_s;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
