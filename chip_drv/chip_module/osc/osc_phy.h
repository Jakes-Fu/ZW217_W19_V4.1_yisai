/******************************************************************************
 ** File Name:    osc_drvapi.h                                             *
 ** Author:       Kui.wang                                                     *
 ** DATE:         24/08/2012                                                  *
 ** Copyright:    2009 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *

 ******************************************************************************/

#ifndef _OSC_PHY_H_
#define _OSC_PHY_H_

#ifdef   __cplusplus
extern   "C"
{
#endif
/**-----------------------------------------------------------------------------*
**                                Macro for interface
**------------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                             Data Prototype                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                           Function Prototype                              **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableClkSource(CLOCK_SOURCE_E clk_src,BOOLEAN isEnable);
/*****************************************************************************/
//  Description:    This function is get current clock source
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_SOURCE_E OSC_PHY_GetCurrentClkSource(void);
/*****************************************************************************/
//  Description:    This function is switch Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SwitchClkSource(CLOCK_SOURCE_E clk_src);
/*****************************************************************************/
//  Description:    This function is switch master control
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_SwitchMaster(OSC_MASTER_E mastermcu);
/*****************************************************************************/
//  Description:    This function is Enable calibration
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_CaliEnable(BOOLEAN isEnable);
/*****************************************************************************/
//  Description:    This function is set calibration cycle
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_SetCaliCycle(uint32 cycleCount);
/*****************************************************************************/
//  Description:    This function is get divide value
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_GetDivValue(uint32 m,uint32 n,DIV_PART_T *div);
/*****************************************************************************/
//  Description:    This function is set divide integer part and fraction part
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_SetDivIF(DIV_PART_T *div);
/*****************************************************************************/
//  Description:    This function is get calibration result
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC uint32 OSC_PHY_SELF_GetCaliResult(void);

/*****************************************************************************/
//  Description:    This function is change dcxo mode
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_DCXO_ChangeMode(DCXO_WORKMODE_E dcxo_mode);
/*****************************************************************************/
//  Description:    This function is Enable DCXO auto power down
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableDcxoAutoPD(BOOLEAN isEnable);

/*****************************************************************************/
//  Description:    This function is Enable REF auto sleep
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableRefAutoSleep(DCXO_REF_E dcxo_ref,BOOLEAN isEnable);

/*****************************************************************************/
//  Description:    This function is Enable low power of clock 32k
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableLowPowerClock32k(BOOLEAN isEnable);

#ifdef   __cplusplus
}
#endif

#endif