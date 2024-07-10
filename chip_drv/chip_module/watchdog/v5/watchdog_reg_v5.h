/******************************************************************************
 ** File Name:    watchdog_reg_v5.h                                            *
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
#ifndef _WATCHDOG_REG_V5_H_
#define _WATCHDOG_REG_V5_H_
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
/*----------Watchdog Timer Counter Register----------*/
#define WDG_LOAD_LOW         (WDG_BASE + 0x00)
#define WDG_LOAD_HIGH        (WDG_BASE + 0x04)
#define WDG_CTRL             (WDG_BASE + 0x08)
#define WDG_INT_CLR          (WDG_BASE + 0x0C)
#define WDG_INT_RAW          (WDG_BASE + 0x10)
#define WDG_INT_MSK          (WDG_BASE + 0x14)
#define WDG_CNT_LOW          (WDG_BASE + 0x18)
#define WDG_CNT_HIGH         (WDG_BASE + 0x1C)
#define WDG_LOCK             (WDG_BASE + 0x20)
#define WDG_CNT_RD_LOW       (WDG_BASE + 0x24)
#define WDG_CNT_RD_HIGH      (WDG_BASE + 0x28)
#define WDG_IRQVALUE_LOW     (WDG_BASE + 0x002C)
#define WDG_IRQVALUE_HIGH    (WDG_BASE + 0x0030)
#define WDG_LOAD_HIGHER    (WDG_BASE + 0x0034)
#define WDG_CNT_HIGHER    (WDG_BASE + 0x0038)
#define WDG_CNT_READ_HIGHER    (WDG_BASE + 0x003C)
#define WDG_IRQ_VALUE_HIGHER    (WDG_BASE + 0x0040)

/******************************************************************************
 * Register Name       : WDG_LOAD_LOW
 * Register Offset     : 0x0000
 * Register Description: Low 16 bit of watchdog load value
 *    WDG_LOAD_LOW                    : [15:0 ] wdg_load_low: low 16 bit of
 *                                      watchdog timer load value. 
 ******************************************************************************/
#define WDG_LOAD_LOW_SHIFT              (0)
#define WDG_LOAD_LOW_MSK                (0xFFFFUL << WDG_LOAD_LOW_SHIFT)

/******************************************************************************
 * Register Name       : WDG_LOAD_HIGH
 * Register Offset     : 0x0004
 * Register Description: High 16 bit of watchdog load value
 *    WDG_LOAD_HIGH                   : [15:0 ] See wdg_load_low description
 ******************************************************************************/
#define WDG_LOAD_HIGH_SHIFT             (0)
#define WDG_LOAD_HIGH_MSK               (0xFFFFUL)

/******************************************************************************
 * Register Name       : WDG_CTRL
 * Register Offset     : 0x0008
 * Register Description: Watchdog control
 *    WDG_RST_EN                     :[3    ] Watchdog reset enable bit
 *                                    0: reset is disabled
 *                                    1: reset is enabled
 *                                    In reset mode and combined mode, this bit should be 1
 *    WDG_NEW                       : [2    ] Watchdog version
 *    WDG_RUN                       : [1    ] Watchdog counter open: 
 *    WDG_IRQ_EN                    : [0    ] Watchdog mode
 ******************************************************************************/
#define WDG_RST_EN                      (BIT_3)
#define WDG_NEW                         (BIT_2)
#define WDG_RUN                         (BIT_1)
#define WDG_IRQ_EN                      (BIT_0)

/******************************************************************************
 * Register Name       : WDG_INT_CLR
 * Register Offset     : 0x000C
 * Register Description: Watchdog interrupt clear
 *     WDG_RST_CLR                     : [3    ] Watchdog reset clear 
 *                                      Write 1 to this bit to clear reset
 *                                      Read this bit always get 0. 
 *     WDG_INT_CLR                     : [0    ] Watchdog interrupt clear 
 ******************************************************************************/
#define WDG_RST_CLR                     (BIT_3)
#define WDG_INT_CLR_BIT                     (BIT_0)

/******************************************************************************
 * Register Name       : WDG_INT_RAW
 * Register Offset     : 0x0010
 * Register Description: Watchdog interrupt raw status
 *     WDG_LD_BUSY                     : [4    ] Watchdog load busy status
 *     WDG_RST_RAW                     : [3    ] Watchdog reset  raw status. 
 *                                        Watch dog reset cannot clear this raw 
 *                                        status, so it can be used to judge if 
 *                                        or not system rebooting comes from 
 *                                        watchdog reset. Write wdg_rst_clr 
 *                                        can clear this raw status. 
 *     WDG_INT_RAW                     : [0    ] Watchdog interrupt raw status 
 ******************************************************************************/
#define WDG_LD_BUSY                     (BIT_4)
#define WDG_RST_RAW                     (BIT_3)
#define WDG_INT_RAW_BIT                     (BIT_0)

/******************************************************************************
 * Register Name       : WDG_INT_MASK
 * Register Offset     : 0x0014
 * Register Description: Watchdog interrupt mask status
 *     WDG_INT_MASK                    : [0    ] Watchdog interrupt masked status
 ******************************************************************************/
#define WDG_INT_MASK_BIT                    (BIT_0)

/******************************************************************************
 * Register Name       : WDG_CNT_LOW
 * Register Offset     : 0x0018
 * Register Description:  Low 16 bit of watchdog counter value
 *     WDG_CNT_LOW                     : [15:0 ] Low 16 bit of      
 *                                        watchdog timer counter value.
 ******************************************************************************/
#define WDG_CNT_LOW_SHIFT               (0)
#define WDG_CNT_LOW_MSK                 (0xFFFFUL << WDG_CNT_LOW_SHIFT)

/******************************************************************************
 * Register Name       : WDG_CNT_HIGH
 * Register Offset     : 0x001C
 * Register Description: High 16 bit of watchdog counter value
 *     WDG_CNT_HIGH                    : [15:0 ]  high 16 bit of      
 *                                        watchdog timer counter value.
 ******************************************************************************/
#define WDG_CNT_HIGH_SHIFT              (0)
#define WDG_CNT_HIGH_MSK                (0xFFFFUL << WDG_CNT_HIGH_SHIFT)

/******************************************************************************
 * Register Name       : WDG_LOCK
 * Register Offset     : 0x0020
 * Register Description: Watchdog lock control
 *     WDG_LOCK                        : [15:0 ] Watchdog lock control
 ******************************************************************************/
#define WDG_LOCK_SHIFT          (0)
#define WDG_LOCK_MSK            (0xFFFFUL << 0)
#define WDG_UNLOCK_KEY          (0xE551)

/******************************************************************************
 * Register Name       : WDG_CNT_RD_LOW
 * Register Offset     : 0x0024
 * Register Description: Low 16 bit of watchdog counter value for read
 *     WDG_CNT_RD_LOW                  : [15:0 ] Low 16 bit of   
 *                                        watchdog timer counter value for read.
 ******************************************************************************/
#define WDG_CNT_RD_LOW_SHIFT                   (0)
#define WDG_CNT_RD_LOW_MSK                     (0xFFFFUL << WDG_CNT_RD_LOW_SHIFT)

/******************************************************************************
 * Register Name       : WDG_CNT_RD_HIGH
 * Register Offset     : 0x0028
 * Register Description: High 16 bit of watchdog counter value for read
 *     WDG_CNT_RD_HIGH                 : [15:0 ] High 16 bit of   
 *                                        watchdog timer counter value for read.
 ******************************************************************************/
#define WDG_CNT_RD_HIGH_SHIFT                   (0)
#define WDG_CNT_RD_HIGH_MSK                    (0xFFFFUL << WDG_CNT_RD_HIGH_SHIFT)

/******************************************************************************
 * Register Name       : WDG_IRQVALUE_LOW
 * Register Offset     : 0x002C
 * Register Description:  Low 16 bit of watchdog irq value
 *     WDG_IRQVALUE_LOW                 : [15:0 ] Low 16 bit of watchdog irqvalue.
 ******************************************************************************/
#define WDG_IRQVALUE_LOW_SHIFT                   (0)
#define WDG_IRQVALUE_LOW_MSK                    (0xFFFFUL << WDG_IRQVALUE_LOW_MSK)

/******************************************************************************
 * Register Name       : WDG_IRQVALUE_HIGH
 * Register Offset     : 0x0030
 * Register Description:  high 16 bit of watchdog irq value
 *     WDG_IRQVALUE_HIGH                 : [15:0 ] high 16 bit of watchdog irqvalue.
 ******************************************************************************/
#define WDG_IRQVALUE_HIGH_SHIFT                   (0)
#define WDG_IRQVALUE_HIGH_MSK                    (0xFFFFUL << WDG_IRQVALUE_HIGH_SHIFT)


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif  //_WATCHDOG_REG_V5_H_


