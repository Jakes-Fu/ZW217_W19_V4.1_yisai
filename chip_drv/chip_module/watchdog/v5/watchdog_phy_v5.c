/******************************************************************************
 ** File Name:      watchdog_phy_v5.c                                                 *
 ** Author:         Jie Dai                                                   *
 ** DATE:           08/02/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/02/2010     Jie Dai            Create.                                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "watchdog_reg_v5.h"
#include "../watchdog_phy.h"
#include "adi_hal_internal.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define WDG_NEWMODE

#ifdef WDG_NEWMODE
#define WDG_LOAD_TIMER_VALUE(value) \
    do{\
        ANA_REG_SET( WDG_LOAD_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_LOAD_LOW , (uint16)((value)  & 0xffff) );\
    }while(0)

#define WDG_LOAD_IRQTIMER_VALUE(value) \
    do{\
        ANA_REG_SET( WDG_IRQVALUE_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_IRQVALUE_LOW , (uint16)((value)  & 0xffff) );\
    }while(0)
#else
#define ANA_WDG_LOAD_TIMEOUT_NUM    (10000)

#define WDG_LOAD_TIMER_VALUE(value) \
    do{\
        uint32   cnt          =  0;\
        while((ANA_REG_GET(WDG_INT_RAW) & WDG_LD_BUSY) && ( cnt < ANA_WDG_LOAD_TIMEOUT_NUM )) cnt++;\
        SCI_PASSERT((cnt < ANA_WDG_LOAD_TIMEOUT_NUM), ("Load WDG Value Timeout!"));/*assert verified*/\
        ANA_REG_SET( WDG_LOAD_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_LOAD_LOW , (uint16)((value)  & 0xffff) );\
    }while(0)

#define WDG_LOAD_IRQTIMER_VALUE(value) \
    do{\
        uint32   cnt          =  0;\
        while((ANA_REG_GET(WDG_INT_RAW) & WDG_LD_BUSY) && ( cnt < ANA_WDG_LOAD_TIMEOUT_NUM )) cnt++;\
        SCI_PASSERT((cnt < ANA_WDG_LOAD_TIMEOUT_NUM), ("Load WDG Value Timeout!"));/*assert verified*/\
        ANA_REG_SET( WDG_IRQVALUE_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_IRQVALUE_LOW , (uint16)((value)  & 0xffff) );\
    }while(0)
#endif

#define ANA_WDG_CLR_INT_TIMEOUT_NUM (10000)

#define CLEAR_WDG_INT(msk) \
    do{ \
        uint32   cnt          =  0;\
        ANA_REG_SET(WDG_INT_CLR, (msk));\
        while((ANA_REG_GET(WDG_INT_RAW) & (msk))&&(cnt < ANA_WDG_CLR_INT_TIMEOUT_NUM)) cnt++; \
        SCI_PASSERT((cnt < ANA_WDG_CLR_INT_TIMEOUT_NUM), ("CLR WDG INT Timeout!"));/*assert verified*/\
    }while(0)


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define  WDG_TRACE  SCI_TRACE_LOW

/*****************************************************************************/
//  Description:    This function config the watch dog module.
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_CONFIG (WDG_CONFIG_T *cfg)
{
    //uint32 ctrl = 0;
    //uint32 val  = 0;

    ///WDG_TRACE("Watch Dog Trace: Watch Dog Value 0x%8.8x", CHIP_REG_GET(WDG_VALUE));

    ANA_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);

    switch (cfg->mode)
    {
        case WDG_TIMEOUT_MODE_RESET:
            ANA_REG_OR (WDG_CTRL, (WDG_RST_EN|WDG_IRQ_EN));  // int before reset
#ifdef WDG_NEWMODE
            ANA_REG_OR (WDG_CTRL, WDG_NEW);  // wdg new mode
#endif
            WDG_LOAD_IRQTIMER_VALUE(WDG_IRQ_COUNT);
            
            break;

        case WDG_TIMEOUT_MODE_INT:
            ANA_REG_AND (WDG_CTRL, ~(WDG_RST_EN));  //int mode only
            ANA_REG_OR (WDG_CTRL, WDG_IRQ_EN);
#ifdef WDG_NEWMODE
            ANA_REG_OR (WDG_CTRL, WDG_NEW);  // wdg new mode
#endif
            WDG_LOAD_IRQTIMER_VALUE(WDG_IRQ_COUNT);

            break;

        default:
            break;  //No need to change
    }

    if (WDG_TIMER_STATE_STOP != cfg->state)
    {
        WDG_LOAD_TIMER_VALUE (cfg->val);
    }    

    switch (cfg->state)
    {
        case WDG_TIMER_STATE_STOP:
            ANA_REG_AND (WDG_CTRL, ~ WDG_RUN);
            break;

        case WDG_TIMER_STATE_START:
            ANA_REG_OR (WDG_CTRL, WDG_RUN);
            break;

        default:
            break;  //No need to change
    }

    //WDG_TRACE:"Watch Dog Trace: Watch Dog Control 0x%8.8x"
//    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_PHY_V5_115_112_2_17_23_8_6_1492,(uint8*)"d", ANA_REG_GET (WDG_CTRL));
    ///    WDG_TRACE ("Watch Dog Trace: Watch Dog LOAD    0x%8.8x", CHIP_REG_GET (WDG_LOAD));

    ANA_REG_SET (WDG_LOCK, (~WDG_UNLOCK_KEY));

    return 0;
}

/*****************************************************************************/
//  Description:    This function clear the watch dog interrupt
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_INT_CLR (void)
{
    ANA_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);
    CLEAR_WDG_INT (WDG_INT_CLR_BIT);
    ANA_REG_SET (WDG_LOCK, (~WDG_UNLOCK_KEY));
    return 0;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif

/*  End Of File */


