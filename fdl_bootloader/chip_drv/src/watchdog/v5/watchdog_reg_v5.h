/******************************************************************************
 ** File Name:    watchdog_reg_v3.h                                            *
 ** Author:       mingwei.zhang                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
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
///
///#define WDG_BASE                     0x82000040
#define WDG_LOAD_LOW            (WDG_BASE + 0x00)
#define WDG_LOAD_HIGH       (WDG_BASE + 0x04)
#define WDG_CTRL            (WDG_BASE + 0x08)
#define WDG_INT_CLR         (WDG_BASE + 0x0C)
#define WDG_INT_RAW         (WDG_BASE + 0x10)
#define WDG_INT_MSK         (WDG_BASE + 0x14)
#define WDG_CNT_LOW         (WDG_BASE + 0x18)
#define WDG_CNT_HIGH        (WDG_BASE + 0x1C)
#define WDG_LOCK            (WDG_BASE + 0x20)
#define WDG_CNT_RD_LOW         (WDG_BASE + 0x24)
#define WDG_CNT_RD_HIGH        (WDG_BASE + 0x28)
#define WDG_IRQVALUE_LOW                 (WDG_BASE + 0x002C)
#define WDG_IRQVALUE_HIGH                (WDG_BASE + 0x0030)

//------------------------------------------------------------------------------
//Register Name       : WDG_LOAD_LOW
//Register Offset     : 0x0000
//Register Description:        Low 16 bit of watchdog load value
//    wdg_load_low                    : [15:0 ] wdg_load_low: low 16 bit of     
//                                       watchdog timer load value. 
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_load_low_MSK                       (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : WDG_LOAD_HIGH
//Register Offset     : 0x0004
//Register Description:        High 16 bit of watchdog load value
//    wdg_load_high                   : [15:0 ] See wdg_load_low description
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_load_high_MSK                      (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : WDG_CTRL
//Register Offset     : 0x0008
//Register Description:        Watchdog control
//    wdg_new                         : [2    ] Watchdog version
//    wdg_run                         : [1    ] Watchdog counter open: 
//    wdg_irq_en                        : [0    ] Watchdog mode
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_rst_en                             (BIT_3)
#define wdg_new                                (BIT_2)
#define wdg_run                                (BIT_1)
#define wdg_irq_en                             (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : WDG_INT_CLR
//Register Offset     : 0x000C
//Register Description:        Watchdog interrupt clear
//    wdg_int_clr                     : [0    ] Watchdog interrupt clear 
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_int_clr                            (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : WDG_INT_RAW
//Register Offset     : 0x0010
//Register Description:        Watchdog interrupt raw status
//    wdg_ld_busy                     : [4    ] Watchdog load busy status
//    wdg_int_raw                     : [0    ] Watchdog interrupt raw status 
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_ld_busy                            (BIT_4)
#define wdg_int_raw                            (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : WDG_INT_MASK
//Register Offset     : 0x0014
//Register Description:        Watchdog interrupt mask status
//    wdg_int_mask                    : [0    ] Watchdog interrupt masked status
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_int_mask                           (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : WDG_CNT_LOW
//Register Offset     : 0x0018
//Register Description:        Low 16 bit of watchdog counter value
//    wdg_cnt_low                     : [15:0 ] wdg_cnt_low: Low 16 bit of      
//                                       watchdog timer counter value.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_cnt_low_MSK                        (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : WDG_CNT_HIGH
//Register Offset     : 0x001C
//Register Description:        High 16 bit of watchdog counter value
//    wdg_cnt_high                    : [15:0 ] See wdg_cnt_low description.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_cnt_high_MSK                       (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : WDG_LOCK
//Register Offset     : 0x0020
//Register Description:        Watchdog lock control
//    wdg_lock                        : [15:0 ] Watchdog lock control
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_lock_MSK                           (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : WDG_CNT_RD_LOW
//Register Offset     : 0x0024
//Register Description:        Low 16 bit of watchdog counter value for read
//    wdg_cnt_rd_low                  : [15:0 ] wdg_cnt_rd_low: Low 16 bit of   
//                                       watchdog timer counter value for read.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_cnt_rd_low_MSK                     (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : WDG_CNT_RD_HIGH
//Register Offset     : 0x0028
//Register Description:        High 16 bit of watchdog counter value for read
//    wdg_cnt_rd_high                 : [15:0 ] Refer to  wdg_cnt_rd_low
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define wdg_cnt_rd_high_MSK                    (0xFFFFUL)


#define WDG_UNLOCK_KEY          0xE551

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_WATCHDOG_REG_V3_H_


