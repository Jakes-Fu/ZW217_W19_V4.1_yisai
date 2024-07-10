/****************************************************************************
** File Name:      mmiazan_export.h                                         *
** Author:         Jesse.Yu                                                *
** Date:           10/01/2011                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 10/01/2011     Jesse.Yu         Create
** 
****************************************************************************/
#ifndef MMIAZAN_API_H
#define MMIAZAN_API_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_time.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
**                          DEPENDANCY                                       *
*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MMI_AZAN_SUPPORT
#define MMIAZAN_ALARM_COUNT              (6)

typedef struct
{
	SCI_TIME_T m_Time[MMIAZAN_ALARM_COUNT];
}AZAN_ALARM_T;

/*****************************************************************************/
// 	Description : Open azan pray win
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIAZAN_OpenMuSiLinPrayWin(void);

/*****************************************************************************/
// 	Description : Set azan alarm status
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIAZAN_SetAzanAlarmFlg(BOOLEAN bStatus);

/*****************************************************************************/
// 	Description : Open azan alarm win
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIAZAN_OpenAlarmExpiredWin(BOOLEAN bRepeat);

/*****************************************************************************/
// 	Description : Azan get repeat timer id
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIAZAN_GetAzanRepeatTimerId(void);
/*****************************************************************************/
// 	Description : MMIAPIAZAN_StopRepeatWinTimer
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIAZAN_StopRepeatWinTimer(void);

/*****************************************************************************/
// 	Description : 恢复azan的出厂设置
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIAZAN_ResetFactorySetting(void);
/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIAZAN_OpenMuSiLinPrayWin				    MMIAPIAZAN_OpenMuSiLinPrayWin
#define MMIAZAN_SetAzanAlarmFlg				        MMIAPIAZAN_SetAzanAlarmFlg
#define MMIAZAN_OpenAlarmExpiredWin				    MMIAPIAZAN_OpenAlarmExpiredWin
#define MMIAZAN_GetAzanRepeatTimerId				    MMIAPIAZAN_GetAzanRepeatTimerId
#define MMIAZAN_StopRepeatWinTimer				    MMIAPIAZAN_StopRepeatWinTimer
#define MMMIAZAN_ResetFactorySetting				    MMIAPIAZAN_ResetFactorySetting

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /*MMI_AZAN_SUPPORT*/
#endif  /* MMIAZAN_API_H */


