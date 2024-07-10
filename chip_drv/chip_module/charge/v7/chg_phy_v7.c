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
#include "rda2720_glb.h"
#include "uws6121e_reg_analog.h"


#include "ldo_drvapi.h"

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
    ANA_REG_OR (ANA_CHGR_CTRL1, (CHGR_CC_EN));
}

/*****************************************************************************/
//  Description:    This function is used to shut down the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_ShutDown (void)
{
    ANA_REG_OR (ANA_CHGR_CTRL0,CHGR_PD);
}

/*****************************************************************************/
//  Description:    This function is used to turn on the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_TurnOn (void)
{
    ANA_REG_AND (ANA_CHGR_CTRL0,~CHGR_PD);
    //CHG_PRINT ( ("CHGMNG:CHG_TurnOn"));
}

/*****************************************************************************/
//  Description:    This function restarts the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetRecharge (void)
{
#if 0
    ANA_REG_OR (ANA_CHGR_CTL2, CHGR_RECHG);
#endif
}

/*****************************************************************************/
//  Description:    This function stops the recharge mode.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_StopRecharge (void)
{
#if 0
    ANA_REG_AND (ANA_CHGR_CTL2, ~CHGR_RECHG);
    SCI_TRACE_ID (TRACE_TOOL_CONVERT,CHG_PHY_V5_114_112_2_17_23_3_33_494, (uint8 *) "");
#endif
}

/*****************************************************************************/
//  Description:    This function sets the lowest switchover point between constant-current
//                      and constant-voltage modes.
//  Author:         Ryan.Liao
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetSwitchoverPoint (CHG_SWITPOINT_E eswitchpoint)
{
    SCI_ASSERT (eswitchpoint <= 63);/*assert verified*/
    ANA_REG_MSK_OR (ANA_CHGR_CTRL0, (eswitchpoint<<CHGR_SW_POINT_SHIFT), CHGR_SW_POINT_MSK);
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

    chg_switchpoint = (ANA_REG_GET (ANA_CHGR_CTRL0) & CHGR_SW_POINT_MSK) >> CHGR_SW_POINT_SHIFT;
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
    ANA_REG_MSK_OR (ANA_CHGR_CTRL0, (chg_switchpoint<<CHGR_SW_POINT_SHIFT), CHGR_SW_POINT_MSK);
    //SCI_TRACE_LOW:"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHG_PHY_V5_186_112_2_17_23_3_34_495,(uint8*)"d", chg_switchpoint);
    return chg_switchpoint;
}

PUBLIC void CHG_PHY_SetChargeEndVolt(uint32 mv)
{
    // 0x00: 4.2v, 0x01: 4.3v, 0x02: 4.4v, 0x03: 4.5v
    #define CHGR_END_V_4200                 4200
    #define CHGR_END_V_4500                 4500

    uint32 b;
    
    SCI_ASSERT ((mv >= CHGR_END_V_4200) && (mv <= CHGR_END_V_4500)); /*assert verified*/
    b = (mv - CHGR_END_V_4200) / 100;

    if (b > (CHGR_END_V_MSK >> CHGR_END_V_SHIFT)) {
        b = CHGR_END_V_MSK >> CHGR_END_V_SHIFT;
    }

    ANA_REG_MSK_OR(ANA_CHGR_CTRL1, (b << CHGR_END_V_SHIFT), CHGR_END_V_MSK);
}

PUBLIC void CHG_PHY_SetVBatOvpVolt(uint32 mv)
{
#if 0
    // 4b'0000: 4.25v, 4b'0001: 4.275v, ..., 4b'1111: 4.625v
    #define CHGR_VBAT_OVP_V_4250            4250
    #define CHGR_VBAT_OVP_V_4625            4625

    uint32 b;
    
    SCI_ASSERT ((mv >= CHGR_VBAT_OVP_V_4250) && (mv <= CHGR_VBAT_OVP_V_4625)); /*assert verified*/
    b = (mv - CHGR_VBAT_OVP_V_4250) / 25;

    if (b > (VBAT_OVP_V_MSK >> VBAT_OVP_V_SHIFT)) {
        b = VBAT_OVP_V_MSK >> VBAT_OVP_V_SHIFT;
    }
   
    ANA_REG_MSK_OR(ANA_MIXED_CTRL0, (b << VBAT_OVP_V_SHIFT), VBAT_OVP_V_MSK);
#endif
}

PUBLIC void CHG_PHY_SetVChgOvpVolt(uint32 mv)
{
    // 2b'00: 6.0v, 2b'01: 6.5v, ..., 2b'10: 7.0v, 2b'11: 9.7v
    #define CHGR_VCHG_OVP_V_6000            6000
    #define CHGR_VCHG_OVP_V_9700            9700

    uint32 b;
    
    SCI_ASSERT ((mv >= CHGR_VCHG_OVP_V_6000) && (mv <= CHGR_VCHG_OVP_V_9700)); /*assert verified*/
    b = (mv - CHGR_VCHG_OVP_V_6000) / 50;
    if (b > 0x3) {
        b = 0x3;
    }
    
    ANA_REG_MSK_OR(ANA_CHGR_CTRL1, (b << VCHG_OVP_V_SHIFT), VCHG_OVP_V_MSK);
}

extern uint8 PH_GetAdaptiveGpioValue(void);

PUBLIC void CHG_PHY_SetChgCurrent (uint16 current)
{
    //CHG_PRINT:"CHGMNG:CHG_PHY_SetChgCurrent=%d"
    uint32 temp;

    SCI_TRACE_ID (TRACE_TOOL_CONVERT,CHG_PHY_V5_207_112_2_17_23_3_34_496, (uint8 *) "d", current);
    if ( (current < CHARGER_CURRENT_300MA) || (current > CHARGER_CURRENT_MAX) || (current%50))
    {
        SCI_ASSERT (0);/*assert verified*/
    }

#if defined(RF_BAND_SELFADAPTIVE)
    if(0x02 == (PH_GetAdaptiveGpioValue()&0x02)) // the HW is different, the DVT phone's current is 1/2.2 of the 8910FF
    {
        SCI_TRACE_LOW("CHGMNG CHG_PHY_SetChgCurrent is DVT");
        switch (current)
        {
            case CHARGER_CURRENT_300MA:
                current = CHARGER_CURRENT_650MA;
                break;
            case CHARGER_CURRENT_350MA:
                current = CHARGER_CURRENT_750MA;
                break;
            case CHARGER_CURRENT_400MA:
                current = CHARGER_CURRENT_900MA;
                break;
            case CHARGER_CURRENT_450MA:
                current = CHARGER_CURRENT_1000MA;
                break;
            case CHARGER_CURRENT_500MA:
                current = CHARGER_CURRENT_1100MA;
                break;
            case CHARGER_CURRENT_550MA:
                current = CHARGER_CURRENT_1200MA;
                break;
            default:
                SCI_ASSERT (0);/*assert verified*/
        }
    }
#endif

//current = CHARGER_CURRENT_700MA;
    switch (current)
    {
        case CHARGER_CURRENT_300MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x0 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_350MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x1 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_400MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x2 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_450MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x3 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_500MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x4 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_550MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x5 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_600MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x6 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_650MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x7 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_700MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x8 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;            
        case CHARGER_CURRENT_750MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0x9 << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;            
        case CHARGER_CURRENT_800MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0xA << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_900MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0xB << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;
        case CHARGER_CURRENT_1000MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0xC << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;            
        case CHARGER_CURRENT_1100MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0xD << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;  
        case CHARGER_CURRENT_1200MA:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0xE << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;  
        case CHARGER_CURRENT_MAX:
            ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (0xF << CHGR_CHG_CUR_SHIFT), CHGR_CHG_CUR_MSK);
            break;              
        default:
            SCI_ASSERT (0);/*assert verified*/
    }
}

PUBLIC void CHG_PHY_SetChargeEndCurrent(uint32 ma) 
{
    // 0x00: 0.9full, 0x01: 0.4full, 0x2: 0.2full, 0x3: 0.1full,
    #define CHGR_ITERM_0P9                  9
    #define CHGR_ITERM_0P4                  4
    #define CHGR_ITERM_0P2                  2
    #define CHGR_ITERM_0P1                  1

    uint32 b=0, full, end, iterm;

    b = (ANA_REG_GET(ANA_CHGR_CTRL1) & CHGR_CHG_CUR_MSK) >> CHGR_CHG_CUR_SHIFT;

    if ( b <= 0xA )
    {
        full = b * 50 + CHARGER_CURRENT_300MA;
    }
    else if ( b <= 0xF)
    {
        full = (b-0xA) * 100 + CHARGER_CURRENT_800MA;
    }

    iterm = ma * 10 / full;
    
    if (iterm >= 9) {
        b = 0;
    } else if (iterm >= 4) {
        b = 1;
    } else if (iterm >= 2) {
        b = 2;
    } else {
        b = 3;
    }
    ANA_REG_MSK_OR (ANA_CHGR_CTRL1, (b << CHGR_ITERM_SHIFT), CHGR_ITERM_MSK);   
}

static BOOLEAN getChgStatus(uint32 status) 
{
    return (ANA_REG_GET(ANA_CHGR_STATUS) & status);
}

PUBLIC BOOLEAN CHG_PHY_IsChargerCvStatus(void) 
{
    return getChgStatus(CHGR_CV_STATUS);
} 

PUBLIC BOOLEAN CHG_PHY_IsChargerPresent(void) 
{
    return getChgStatus(CHGR_INT);
} 

PUBLIC BOOLEAN CHG_PHY_IsChargerReady(void) 
{
    return getChgStatus(CHGR_ON);
} 

PUBLIC BOOLEAN CHG_PHY_IsVBatOv(void) 
{
    return 0;
#if 0
    return getChgStatus(VBAT_OVI);
#endif
} 

PUBLIC BOOLEAN CHG_PHY_IsVChgOv(void) 
{
    return getChgStatus(VCHG_OVI);
} 

uint32 CHG_PHY_IsBatDetOK(void)
{
    return ((ANA_REG_GET(ANA_MIXED_CTRL) & BATDET_OK_MASK) >> BATDET_OK_SHIFT);
}

#if 0
uint32 CHG_PHY_ChgCtl1(void)
{
    return ANA_REG_GET(ANA_CHGR_CTL1);
}

PUBLIC uint32 CHG_PHY_ChgCtl0(void)
{
    return ANA_REG_GET(ANA_CHGR_CTL0);
}

PUBLIC uint32 CHG_PHY_ChgStatus (void)
{
    return ANA_REG_GET(ANA_CHGR_STATUS);
}
#endif

PUBLIC CHGMNG_ADAPTER_TYPE_E CHG_PHY_IdentifyAdpType (void)
{
	uint32 ret = CHGMNG_ADP_UNKNOW;
	uint32 charger_status, cnt = 40;

	while ((!(ANA_REG_GET(ANA_CHGR_STATUS) & BIT_CHG_DET_DONE)) && cnt--) {
		/* adding delay is to read the register chg_status value successfully.
		  * changing the delay from 200ms to 50ms is to speed up charging icon display
		  */
		SCI_SLEEP (50);
	}

	charger_status = ANA_REG_GET(ANA_CHGR_STATUS);
	charger_status &= (CDP_INT | DCP_INT | SDP_INT);

	switch (charger_status) {
	case CDP_INT:
		ret = CHGMNG_ADP_NONSTANDARD;
		break;
	case DCP_INT:
		ret = CHGMNG_ADP_STANDARD;
		break;
	case SDP_INT:
		ret = CHGMNG_ADP_USB;
		break;
	default:
		ret = CHGMNG_ADP_UNKNOW;
	}
	return ret;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of chg_drv.c


