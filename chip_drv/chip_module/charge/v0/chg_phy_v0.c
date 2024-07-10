/******************************************************************************
 ** File Name:      chg_phy.c                                                 *
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
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
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
#define CHG_CTL             (GPI_PG0_BASE + 0x0000)
#define CHGINT                  2
#define ANATST_CTL          (GREG_BASE + 0x003C)

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
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
    uint32 reg_value = REG32 (GR_CHGR_CTL) & 0xFFFFFFC3;

    if (mode == CHG_DEFAULT_MODE)
    {
        reg_value &= ~ (BIT_2 | BIT_4);
        reg_value |= (BIT_3 | BIT_5);   // BIT_5 reset USB_500ma_en, BIT_3 reset adapter_en
        CHIP_REG_SET (GR_CHGR_CTL, reg_value);
    }
    else if (mode == CHG_NORMAL_ADAPTER)
    {
        reg_value &= ~ (BIT_3 | BIT_4);
        reg_value |= (BIT_5 | BIT_2);  // BIT_23 reset USB_500ma_en, BIT_20 set adapter_en
        CHIP_REG_SET (GR_CHGR_CTL, reg_value);
    }
    else if (mode == CHG_USB_ADAPTER)
    {
        reg_value &= ~ (BIT_2 | BIT_5);
        reg_value |= (BIT_4 | BIT_3);  //BIT_22 set USB_500ma_en, BIT_21 reset adapter_en
        CHIP_REG_SET (GR_CHGR_CTL, reg_value);
    }
}

/*****************************************************************************/
//  Description:    This function is used to set usb charge current.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHG_PHY_SetUSBChargeCurrent (uint16 current)
{
    uint32 reg_value = REG32 (GR_CHGR_CTL) & 0xFFFCFFFF; //BIT16 and BIT17

    switch (current)
    {
        case CHARGER_CURRENT_300MA:
            /* only in this mode, charge current would be 300mA */
            reg_value = reg_value | (0  << 16);
            break;
        case CHARGER_CURRENT_400MA:
            reg_value = reg_value | (0x1  << 16);
            break;
        case CHARGER_CURRENT_500MA:
            reg_value = reg_value | (0x3  << 16);
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
    }

    CHIP_REG_SET (GR_CHGR_CTL, reg_value);
    
    //CHG_PRINT:"CHGMNG:CHG_SetUSBChargeCurrent=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V0_117_112_2_17_23_3_31_487,(uint8*)"d", current);
}

/*****************************************************************************/
//  Description:    This function is used to set adapter charge current.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHG_PHY_SetNormalChargeCurrent (uint16 current)
{
    uint32 reg_value = REG32 (GR_CHGR_CTL) & 0xFFF3FFFF; //BIT18 and BIT19

    switch (current)
    {
        case CHARGER_CURRENT_300MA:
            /* only in this mode, charge current would be 300mA */
            _CHG_PHY_SetAdapterMode (CHG_DEFAULT_MODE);
            return;
        case CHARGER_CURRENT_400MA:
            reg_value = reg_value | (0 << 18);
            break;
        case CHARGER_CURRENT_600MA:
            reg_value = reg_value | (1 << 18);
            break;
        case CHARGER_CURRENT_800MA:
            reg_value = reg_value | (2 << 18);
            break;
        case CHARGER_CURRENT_1000MA:
            reg_value = reg_value | (3 << 18);
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
    }

    CHIP_REG_SET (GR_CHGR_CTL, reg_value);
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
    GPIO_Enable (2);
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
    return (REG32 (CHG_CTL) & BIT_2) ? SCI_TRUE : SCI_FALSE;
}

/*****************************************************************************/
//  Description:    This function is used to shut down the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_ShutDown (void)
{
    CHIP_REG_OR (GR_CHGR_CTL, BIT_20);
    //CHG_PRINT(("CHGMNG:CHG_ShutDown"));
}

/*****************************************************************************/
//  Description:    This function is used to turn on the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_TurnOn (void)
{
    CHIP_REG_AND (GR_CHGR_CTL, ~BIT_20);

    //CHG_PRINT(("CHGMNG:CHG_TurnOn"));
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
    return SCI_FALSE;
}

/*****************************************************************************/
//  Description:    This function restarts the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetRecharge (void)
{
    CHIP_REG_OR (GR_CHGR_CTL, BIT_6);
}

/*****************************************************************************/
//  Description:    This function stops the recharge mode.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_StopRecharge (void)
{
    CHIP_REG_AND (GR_CHGR_CTL, ~BIT_6);
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

    CHIP_REG_SET (GR_CHGR_CTL, (REG32 (GR_CHGR_CTL) & (~0x1F00) | (eswitchpoint << 8)));
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

    current_switchpoint = (REG32 (GR_CHGR_CTL) & 0x00000F00) >> 8;
    shift_bit = (REG32 (GR_CHGR_CTL) & BIT_12) >> 12;

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
    CHIP_REG_SET (GR_CHGR_CTL, (REG32 (GR_CHGR_CTL) & (~0x1F00) | (chg_switchpoint << 8)));

    //SCI_TRACE_LOW:"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V0_292_112_2_17_23_3_32_488,(uint8*)"d", chg_switchpoint);
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
    return (REG32 (CHG_CTL) & BIT_2) ? SCI_TRUE : SCI_FALSE; //This register depends on GPIO pinmap setting!
}

PUBLIC void CHG_PHY_SetChgCurrent(uint16 current)
{
    //CHG_PRINT:"CHGMNG:CHG_PHY_SetChgCurrent=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V0_309_112_2_17_23_3_32_489,(uint8*)"d", current);
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
    REG32 (0x8b00004c) |= BIT_3;
    
    if ( ( (REG32 (0x8b000054)) & BIT_1) == BIT_1)
    {
        return CHGMNG_ADP_STANDARD;
    }
    else
    {
        return CHGMNG_ADP_UNKNOW;
    }
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of chg_drv.c

