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

#ifndef _OSC_DRVAPI_H_
#define _OSC_DRVAPI_H_

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

typedef enum
{
	MASTER_ARM,
	MASTER_DSP,
	MASTER_MAX
}OSC_MASTER_E;

typedef struct
{
	uint32 integer;
	uint32 fraction;
	uint32 precision;
}DIV_PART_T;

typedef enum
{
	CLOCK_SELF_32K,
	CLOCK_DCXO_32K,
	CLOCK_EXTE_32K
}CLOCK_SOURCE_E;

typedef enum
{
    NORMALCURRENT_MODE,
    LOWCURRENT_MODE
}DCXO_WORKMODE_E;

typedef enum
{
	DCXO_REF1,
	DCXO_REF2
}DCXO_REF_E;
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
PUBLIC void OSC_HAL_EnableClkSource(CLOCK_SOURCE_E clk_src,BOOLEAN isSelect);
/*****************************************************************************/
//  Description:    This function is select Source 32k clock
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_SelectClkSource(CLOCK_SOURCE_E clk_src);
/*****************************************************************************/
//  Description:    This function is change dcxo mode in low current and normal
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_DcxoChangeMode(DCXO_WORKMODE_E dcxo_mode);
/*****************************************************************************/
//  Description:    This function is Enable calibration module
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_SELF_EnableCalibrate(BOOLEAN isEnable);
/*****************************************************************************/
//  Description:    This function is start calibration
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_SELF_StartCalibrate(uint32 cycleCount);
/*****************************************************************************/
//  Description:    This function is get current clock 32k source
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_SOURCE_E OSC_HAL_GetCurrentClkSource(void);
/*****************************************************************************/
//  Description:    This function is Enable DCXO auto power down
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableDcxoAutoPD(BOOLEAN isEnable);
/*****************************************************************************/
//  Description:    This function is Enable REF auto sleep
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableRefAutoSleep(DCXO_REF_E dcxo_ref,BOOLEAN isEnable);

/*****************************************************************************/
//  Description:    This function is Enable low power of clock 32k
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_HAL_EnableLowPowerClock32k(BOOLEAN isEnable);

#ifdef   __cplusplus
}
#endif

#endif
