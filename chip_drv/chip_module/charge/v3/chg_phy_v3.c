/******************************************************************************
 ** File Name:      chg_phy_v3.c                                                 *
 ** Author:         Benjamin.Wang                                                   *
 ** DATE:           24/11/2004                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 charger.                                               *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                               *
 ** 24/11/2004      Benjamin.Wang       Create.                                   *
 ** 01/12/2009      Yi.Qiu              for SC6600L
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
//#include "../../analog/v3/analog_reg_v3.h"
//#include "adi_hal_internal.h"
#include "chg_drvapi.h"
#include "gpio_drvapi.h"
#include "../charge_phy.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_CHG

#ifdef DEBUG_CHG
#define CHG_PRINT( _format_string )   SCI_TRACE_LOW _format_string
#else
#define CHG_PRINT( _format_string )
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

#define CHG_CTL             (ANA_GPIN_PG0_BASE + 0x0000)
#define ANATST_CTL          (GREG_BASE + 0x003C)

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#if 0
typedef enum
{
    CHG_DEFAULT_MODE = 0,
    CHG_NORMAL_ADAPTER,
    CHG_USB_ADAPTER
}CHG_ADAPTER_MODE_E;
#endif
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void _CHG_PHY_SetAdapterMode (CHG_ADAPTER_MODE_E mode);
LOCAL void _CHG_PHY_SetUSBChargeCurrent (uint16 current);
LOCAL void _CHG_PHY_SetNormalChargeCurrent (uint16 current);
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to set the adapter mode.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHG_PHY_SetAdapterMode (CHG_ADAPTER_MODE_E mode)
{
    if (mode == CHG_DEFAULT_MODE)
    {
        // BIT_5 reset USB_500ma_en, BIT_3 reset adapter_en
        ANA_REG_MSK_OR (ANA_CHGR_CTL0, (CHGR_ADATPER_EN_RST_BIT|CHGR_USB_500MA_EN_RST_BIT), CHAR_ADAPTER_MODE_MSK);
    }
    else if (mode == CHG_NORMAL_ADAPTER)
    {
        // BIT_23 reset USB_500ma_en, BIT_20 set adapter_en
        ANA_REG_MSK_OR (ANA_CHGR_CTL0, (CHGR_ADATPER_EN_BIT|CHGR_USB_500MA_EN_RST_BIT), CHAR_ADAPTER_MODE_MSK);
    }
    else if (mode == CHG_USB_ADAPTER)
    {
        //BIT_22 set USB_500ma_en, BIT_21 reset adapter_en
        ANA_REG_MSK_OR (ANA_CHGR_CTL0, (CHGR_ADATPER_EN_RST_BIT|CHGR_USB_500MA_EN_BIT), CHAR_ADAPTER_MODE_MSK);
    }
}

/*****************************************************************************/
//  Description:    This function is used to set usb charge current.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHG_PHY_SetUSBChargeCurrent (uint16 current)
{
    
    switch (current)
    {
        case CHARGER_CURRENT_300MA:
            /* only in this mode, charge current would be 300mA */
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (0 << CHGR_USB_CHG_SHIFT), CHGR_USB_CHG_MSK);
            break;
        case CHARGER_CURRENT_400MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (0x1 << CHGR_USB_CHG_SHIFT), CHGR_USB_CHG_MSK);
            break;
        case CHARGER_CURRENT_500MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (0x3 << CHGR_USB_CHG_SHIFT), CHGR_USB_CHG_MSK);
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
    }
        
    //CHG_PRINT:"CHGMNG:CHG_SetUSBChargeCurrent=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V3_116_112_2_17_23_3_33_490,(uint8*)"d", current);
}

/*****************************************************************************/
//  Description:    This function is used to set adapter charge current.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHG_PHY_SetNormalChargeCurrent (uint16 current)
{
    switch (current)
    {
        case CHARGER_CURRENT_300MA:
            /* only in this mode, charge current would be 300mA */
            _CHG_PHY_SetAdapterMode (CHG_DEFAULT_MODE);
            return;
        case CHARGER_CURRENT_400MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (0 << CHGR_ADAPTER_CHG_SHIFT), CHGR_ADAPTER_CHG_MSK);
            break;
        case CHARGER_CURRENT_600MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (1 << CHGR_ADAPTER_CHG_SHIFT), CHGR_ADAPTER_CHG_MSK);
            break;
        case CHARGER_CURRENT_800MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (2 << CHGR_ADAPTER_CHG_SHIFT), CHGR_ADAPTER_CHG_MSK);
            break;
        case CHARGER_CURRENT_1000MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (3 << CHGR_ADAPTER_CHG_SHIFT), CHGR_ADAPTER_CHG_MSK);
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
    }
}


/*****************************************************************************/
//  Description:    This function initialize charger function. It enables the control registers for
//                  GPIO48~GPIO58, sets the mask bits and sets the direction of GPIO55~58 to output.
//  Author:         Benjamin.Wang
//  Note:           This module should be initialized after GPIO_Init.
/*****************************************************************************/
PUBLIC void  CHG_PHY_Init (void)
{
    /* GPIO for charge int */
    GPIO_Enable (162);
}

/*****************************************************************************/
//  Description:    This function indicates that the charger input Vchg is above 3V.
//                  Return: SCI_TRUE - Charge INT has happened.
//                              SCI_FALSE - Charge INT hasn't happened.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CHG_PHY_GetCHGIntStatus (void)
{
#ifndef FPGA_VERIFICATION
    return (CHIP_REG_GET (CHG_CTL) & BIT_11) ? SCI_TRUE : SCI_FALSE;
#else
    return SCI_FALSE;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to shut down the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_ShutDown (void)
{
    ANA_REG_OR (ANA_CHGR_CTL0,CHGR_PD_BIT);
    //CHG_PRINT ( ("CHGMNG:CHG_ShutDown"));
}

/*****************************************************************************/
//  Description:    This function is used to turn on the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_TurnOn (void)
{
    ANA_REG_AND (ANA_CHGR_CTL0,~CHGR_PD_BIT);
    //CHG_PRINT ( ("CHGMNG:CHG_TurnOn"));
}

/*****************************************************************************/
//  Description:    This function indicates the charger status.If it is "1", the charge is done.
//                  Return: SCI_TRUE - charge had done.
//                              SCI_FALSE - charge hadn't finished.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CHG_PHY_GetCHGDoneStatus (void)
{
    return SCI_FALSE;       ///sc8800g don't have this function.
}

/*****************************************************************************/
//  Description:    This function restarts the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetRecharge (void)
{
    ANA_REG_OR (ANA_CHGR_CTL0,CHGR_RECHG_BIT);
    //CHG_PRINT ( ("CHGMNG:CHG_SetRecharge"));
}

/*****************************************************************************/
//  Description:    This function stops the recharge mode.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_StopRecharge (void)
{
    ANA_REG_AND (ANA_CHGR_CTL0,~CHGR_RECHG_BIT);
    //CHG_PRINT:"CHGMNG:CHG_StopRecharge"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V3_215_112_2_17_23_3_33_491,(uint8*)"");
}

/*****************************************************************************/
//  Description:    This function sets the lowest switchover point between constant-current
//                      and constant-voltage modes.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetSwitchoverPoint (
    CHG_SWITPOINT_E eswitchpoint         // the input range is 0~31. shifup/down 1bit,
    // CC-CV switchover voltage shift amount, MSB, 2bits
    // CC-CV switchover voltage shift amount, LSB, 2bits
)
{
    SCI_ASSERT (eswitchpoint <=31);/*assert verified*/
    ANA_REG_MSK_OR (ANA_CHGR_CTL1, (eswitchpoint<<CHAR_SW_POINT_SHIFT), CHAR_SW_POINT_MSK);
}

/*****************************************************************************/
//  Description:    This function is used to update one level of the lowest switchover point
//                      between constant-current and constant-voltage modes.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHG_PHY_UpdateSwitchoverPoint (BOOLEAN up_or_down)
{
    uint8 current_switchpoint;
    uint8 shift_bit;
    uint8 chg_switchpoint;

    chg_switchpoint = (ANA_REG_GET (ANA_CHGR_CTL1) & CHAR_SW_POINT_MSK) >> CHAR_SW_POINT_SHIFT;
    shift_bit = chg_switchpoint >> 4;
    current_switchpoint = chg_switchpoint&0x0F;

    if (up_or_down)
    {
        if (shift_bit > 0)
        {
            if (current_switchpoint < 0xF)
            {
                current_switchpoint += 1;
            }
        }
        else
        {
            if (current_switchpoint > 0)
            {
                current_switchpoint -= 1;
            }
            else
            {
                shift_bit = 1;
            }
        }
    }
    else
    {
        if (shift_bit > 0)
        {
            if (current_switchpoint > 0)
            {
                current_switchpoint -= 1;
            }
            else
            {
                shift_bit = 0;
            }
        }
        else
        {
            if (current_switchpoint < 0xF)
            {
                current_switchpoint += 1;
            }
        }
    }

    chg_switchpoint = (shift_bit << 4) | current_switchpoint;
    ANA_REG_MSK_OR (ANA_CHGR_CTL1, (chg_switchpoint<<CHAR_SW_POINT_SHIFT), CHAR_SW_POINT_MSK);
    //SCI_TRACE_LOW:"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V3_291_112_2_17_23_3_33_492,(uint8*)"d", chg_switchpoint);
    return chg_switchpoint;
}

/*****************************************************************************/
//  Description:    This function sets the lowest switchover point between constant-current
//                      and constant-voltage modes.
//                  Return: SCI_TRUE - Charger is present.
//                              SCI_FALSE - Charger had unplugged.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CHG_PHY_IsChargerPresent (void)
{
#ifndef FPGA_VERIFICATION
    return (ANA_REG_GET (CHG_CTL) & BIT_2) ? SCI_TRUE : SCI_FALSE;
#else
    return SCI_FALSE;
#endif
}

PUBLIC void CHG_PHY_SetChgCurrent(uint16 current)
{
    //CHG_PRINT:"CHGMNG:CHG_PHY_SetChgCurrent=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V3_312_112_2_17_23_3_33_493,(uint8*)"d", current);
    switch(current)
    {
        case CHARGER_CURRENT_300MA:
        case CHARGER_CURRENT_500MA:
            _CHG_PHY_SetUSBChargeCurrent(current);
            _CHG_PHY_SetAdapterMode(CHG_USB_ADAPTER);
        break;
        case CHARGER_CURRENT_400MA:
        case CHARGER_CURRENT_600MA:
        case CHARGER_CURRENT_800MA:
        case CHARGER_CURRENT_1000MA:
            _CHG_PHY_SetNormalChargeCurrent (current);
            _CHG_PHY_SetAdapterMode(CHG_NORMAL_ADAPTER);
        break;
        default:
           SCI_ASSERT (0);/*assert verified*/ 
    }
}

PUBLIC CHGMNG_ADAPTER_TYPE_E CHG_PHY_IdentifyAdpType(void)
{
    uint32 ret;
    uint32 i;

    CHIP_REG_AND (USB_PHY_CTRL, (~ (USB_DM_PULLDOWN_BIT|USB_DP_PULLDOWN_BIT)));

    //Identify USB charger
    CHIP_REG_OR (USB_PHY_CTRL, USB_DM_PULLUP_BIT);

    for (i = 0; i < 200; i++)
    {
    }

    ret = GPIO_GetValue (145);  ///USB DM:GPIO145 in SC8800G2
    CHIP_REG_AND (USB_PHY_CTRL, (~USB_DM_PULLUP_BIT));

    //normal charger
    if (ret)
    {
        ///Identify standard adapter
        CHIP_REG_OR (USB_PHY_CTRL, USB_DM_PULLDOWN_BIT);

        for (i = 0; i < 200; i++)
        {
        }

        if (GPIO_GetValue (145) == GPIO_GetValue (146))
        {
            CHIP_REG_AND (USB_PHY_CTRL, (~USB_DM_PULLDOWN_BIT));
            return CHGMNG_ADP_STANDARD;
        }
        else
        {
            CHIP_REG_AND (USB_PHY_CTRL, (~USB_DM_PULLDOWN_BIT));
            return CHGMNG_ADP_NONSTANDARD;
        } 
    }

    return CHGMNG_ADP_USB;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of chg_drv.c

