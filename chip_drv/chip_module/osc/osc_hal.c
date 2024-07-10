/******************************************************************************
 ** File Name:      osc_hal.c                                                 *
 ** Author:         kui.wang                                                    *
 ** DATE:           08/21/2012                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                 Edit          History                                     *
 **---------------------------------------------------------------------------*
 ** DATE            NAME          DESCRIPTION                                 *
 **                                                                           *
 ******************************************************************************/

#include "os_api.h"
#include "osc_drvapi.h"
#include "osc_phy.h"
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableClkSource(CLOCK_SOURCE_E clk_src,BOOLEAN isSelect)
{
	if(isSelect)
	{	
		OSC_PHY_EnableClkSource(clk_src,SCI_TRUE);
	}
	else
	{
		OSC_PHY_EnableClkSource(clk_src,SCI_FALSE);
	}
}

/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_SelectClkSource(CLOCK_SOURCE_E clk_src)
{
    OSC_PHY_SwitchClkSource(clk_src);
}
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_SOURCE_E OSC_HAL_GetCurrentClkSource(void)
{
    return OSC_PHY_GetCurrentClkSource();
}
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_DcxoChangeMode(DCXO_WORKMODE_E dcxo_mode)
{
    OSC_PHY_DCXO_ChangeMode(dcxo_mode);
}
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/

PUBLIC void OSC_HAL_SELF_EnableCalibrate(BOOLEAN isEnable)
{
	if(isEnable)
	{
		OSC_PHY_EnableClkSource(CLOCK_SELF_32K,SCI_TRUE);
		OSC_PHY_SELF_SwitchMaster(MASTER_ARM);
		OSC_PHY_SELF_CaliEnable(SCI_TRUE);
	}
	else
	{
		OSC_PHY_EnableClkSource(CLOCK_SELF_32K,SCI_FALSE);
		OSC_PHY_SELF_SwitchMaster(MASTER_DSP);
		OSC_PHY_SELF_CaliEnable(SCI_FALSE);
	}
}

/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_SELF_StartCalibrate(uint32 cycleCount)
{
	uint32 cal_result;
	DIV_PART_T divider;
	
	OSC_PHY_SELF_SetCaliCycle(cycleCount);
	cal_result = OSC_PHY_SELF_GetCaliResult();
	OSC_PHY_SELF_GetDivValue(cal_result,cycleCount,&divider);
	OSC_PHY_SELF_SetDivIF(&divider);
}
/*****************************************************************************/
//  Description:    This function is Enable DCXO auto power down
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableDcxoAutoPD(BOOLEAN isEnable)
{
	if(isEnable)
	{
		OSC_PHY_EnableDcxoAutoPD(SCI_TRUE);
	}
	else
	{
		OSC_PHY_EnableDcxoAutoPD(SCI_FALSE);
	}
}
/*****************************************************************************/
//  Description:    This function is Enable REF auto sleep
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableRefAutoSleep(DCXO_REF_E dcxo_ref,BOOLEAN isEnable)
{
	if(isEnable)
	{
		OSC_PHY_EnableRefAutoSleep(dcxo_ref,SCI_TRUE);
	}
	else
	{
		OSC_PHY_EnableRefAutoSleep(dcxo_ref,SCI_FALSE);
	}
}

/*****************************************************************************/
//  Description:    This function is Enable low power of clock 32k
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableLowPowerClock32k(BOOLEAN isEnable)
{
	if(isEnable)
	{
		OSC_PHY_EnableLowPowerClock32k(SCI_TRUE);
	}
	else
	{
		OSC_PHY_EnableLowPowerClock32k(SCI_FALSE);
	}
}

