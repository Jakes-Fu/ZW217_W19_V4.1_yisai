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
#include "chg_drvapi.h"
#include "eic_hal.h"
#include "eic_hal.h"
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

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    This function initialize charger function. It enables the control registers for
//                  GPIO48~GPIO58, sets the mask bits and sets the direction of GPIO55~58 to output.
//  Author:         Benjamin.Wang
//  Note:           This module should be initialized after GPIO_Init.
/*****************************************************************************/
PUBLIC void  CHG_PHY_Init (void)
{
#ifdef CHG_SHARP_FEATRUE
    ANA_REG_OR (ANA_CHGR_CTL0, (CHGR_CC_EN|BIT_9)); //Charger current setting enable //sharp featrue
#else
    ANA_REG_OR (ANA_CHGR_CTL0, (CHGR_CC_EN));   //Charger current setting enable
#endif
}


/*****************************************************************************/
//  Description:    This function is used to shut down the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_ShutDown (void)
{
    ANA_REG_OR (ANA_CHGR_CTL0,CHGR_PD);
    //CHG_PRINT ( ("CHGMNG:CHG_ShutDown"));
}

/*****************************************************************************/
//  Description:    This function is used to turn on the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_TurnOn (void)
{
    ANA_REG_AND (ANA_CHGR_CTL0,~CHGR_PD);
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
	ANA_REG_OR (ANA_CHGR_CTL0,CHGR_RECHG);
    //CHG_PRINT ( ("CHGMNG:CHG_SetRecharge"));
}

/*****************************************************************************/
//  Description:    This function stops the recharge mode.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_StopRecharge (void)
{
    ANA_REG_AND (ANA_CHGR_CTL0,~CHGR_RECHG);
    //CHG_PRINT:"CHGMNG:CHG_StopRecharge"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT,CHG_PHY_V5_114_112_2_17_23_3_33_494, (uint8 *) "");
}

/*****************************************************************************/
//  Description:    This function sets the lowest switchover point between constant-current
//                      and constant-voltage modes.
//  Author:         Ryan.Liao
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetSwitchoverPoint (CHG_SWITPOINT_E eswitchpoint)
{
   	SCI_ASSERT (eswitchpoint <=31);/*assert verified*/

#if defined(CHIP_VER_6531)
	if (CHIP_GetADIEChipID() == CHIP_ID_SR1131BA) {
		ANA_REG_MSK_OR (ANA_CHGR_CTL1, (eswitchpoint<<CHGR_SW_POINT_SHIFT_METAL), CHGR_SW_POINT_MSK_METAL);
	} else {
    	ANA_REG_MSK_OR (ANA_CHGR_CTL1, (eswitchpoint<<CHGR_SW_POINT_SHIFT), CHGR_SW_POINT_MSK);
	}
#else
	ANA_REG_MSK_OR (ANA_CHGR_CTL1, (eswitchpoint<<CHGR_SW_POINT_SHIFT), CHGR_SW_POINT_MSK);
#endif

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

#ifdef CHIP_VER_6531 
	if (CHIP_GetADIEChipID() == CHIP_ID_SR1131BA) {
		chg_switchpoint = (ANA_REG_GET (ANA_CHGR_CTL1) & CHGR_SW_POINT_MSK_METAL) >> CHGR_SW_POINT_SHIFT_METAL;

		if (up_or_down) {
			if (0x1F == chg_switchpoint) {
				chg_switchpoint = 0x1F;
			} else {
				chg_switchpoint += 1;
			}
		} else {
			if (0 == chg_switchpoint) {
				chg_switchpoint = 0x0;
			} else {
				chg_switchpoint -= 1;
			}
		}

    	ANA_REG_MSK_OR (ANA_CHGR_CTL1, (chg_switchpoint<<CHGR_SW_POINT_SHIFT_METAL), CHGR_SW_POINT_MSK_METAL);		
	} else {
#endif

	    chg_switchpoint = (ANA_REG_GET (ANA_CHGR_CTL1) & CHGR_SW_POINT_MSK) >> CHGR_SW_POINT_SHIFT;

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

    	ANA_REG_MSK_OR (ANA_CHGR_CTL1, (chg_switchpoint<<CHGR_SW_POINT_SHIFT), CHGR_SW_POINT_MSK);

#ifdef CHIP_VER_6531
}
#endif
    //SCI_TRACE_LOW:"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT,CHG_PHY_V5_186_112_2_17_23_3_34_495, (uint8 *) "d", chg_switchpoint);
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
    return EIC_HAL_GetValue (18);
#else
    return SCI_FALSE;
#endif
}

PUBLIC void CHG_PHY_SetChgCurrent (uint16 current)
{
    //CHG_PRINT:"CHGMNG:CHG_PHY_SetChgCurrent=%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT,CHG_PHY_V5_207_112_2_17_23_3_34_496, (uint8 *) "d", current);
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    {
        uint32 temp;

        if ( (current < CHARGER_CURRENT_300MA) || (current > CHARGER_CURRENT_1050MA) || (current%50))
        {
            SCI_ASSERT (0);/*assert verified*/
        }

        temp = (current - CHARGER_CURRENT_300MA) /50;

        ANA_REG_MSK_OR (ANA_CHGR_CTL0, (temp << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
    }
#else

    switch (current)
    {
        case CHARGER_CURRENT_300MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (0 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_400MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (1 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_500MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (2 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_600MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (3 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_800MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (4 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_1000MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTL0, (5 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
    }

#endif
}

PUBLIC CHGMNG_ADAPTER_TYPE_E CHG_PHY_IdentifyAdpType (void)
{
    uint32 ret;
    uint32 i;
    uint32 reg_save;

    CHIP_REG_OR (APB_MISC_CTL0, USB_STATE_REN);

    reg_save = CHIP_REG_GET (APB_MISC_CTL0);

    //Identify USB charger

    CHIP_REG_OR (APB_MISC_CTL0, USB_100KD_EN);      //don't pull down DM
    CHIP_REG_AND (APB_MISC_CTL0, (~ (USB_100K_EN|USB_1K5_EN))); //pull up DM,DP

    for (i = 0; i < 400; i++)
    {
        ;
    }

    ret = CHIP_REG_GET (APB_MISC_STS0) & USB_RX_DM;
    CHIP_REG_OR (APB_MISC_CTL0, (USB_100K_EN));     //don't pull up DM

    //normal charger
    if (ret)
    {
        ///Identify standard adapter
        CHIP_REG_AND (APB_MISC_CTL0, ~ (USB_100KD_EN|USB_1K5_EN));      //pull down DM,up DP

        for (i = 0; i < 400; i++)
        {
            ;
        }

        i = CHIP_REG_GET (APB_MISC_STS0) & (USB_RX_DM |USB_RX_DP) ;

        if ( (i == (USB_RX_DM |USB_RX_DP)) || (i == 0))
        {
            CHIP_REG_SET (APB_MISC_CTL0, reg_save);

            return CHGMNG_ADP_STANDARD;
        }
        else
        {
            CHIP_REG_SET (APB_MISC_CTL0, reg_save);

            return CHGMNG_ADP_NONSTANDARD;
        }
    }

    CHIP_REG_SET (APB_MISC_CTL0, reg_save);

    return CHGMNG_ADP_USB;

}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of chg_drv.c


