/*****************************************************************************
** File Name:      mmiflashlight_export.h                                               
** Author:      
** Date:         
** Copyright:    
** Description: 
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
******************************************************************************/

#ifndef _MMIFLASHLIGHT_EXPORT_H_
#define _MMIFLASHLIGHT_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//parse window
typedef enum mmiacc_flashlight_mode_tag
{
    MMIACC_FLASHLIGHT_MODE_ON,
    MMIACC_FLASHLIGHT_MODE_OFF,
    MMIACC_FLASHLIGHT_MODE_SWITCH,

    MMIACC_FLASHLIGHT_MODE_MAX
} MMIACC_FLASHLIGHT_MODE_E;


 /*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIACC_OpenFlashlighWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_E mode);

/*****************************************************************************/
//  Description : MMIACC_CreateTorchTimer
//  Global resource dependence : 
//  Author: cheney.wu
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIACC_CreateTorchTimer(void);

/******************************************************************************/
// Description: MMIACC_StopTorchTimer
// Global resource dependence: 
// Author: cheney.wu
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIACC_StopTorchTimer(void);

/******************************************************************************/
// Description: MMIACC_ProcessTorchTimerOver
// Global resource dependence: 
// Author: cheney.wu
// Note: 超时保护机制
/******************************************************************************/
PUBLIC void MMIACC_ProcessTorchTimerOver(uint8 timer_id, uint32 param);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  


