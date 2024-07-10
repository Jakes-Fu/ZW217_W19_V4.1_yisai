/*****************************************************************************
** File Name:      mmikl_internal.h                                             *
** Author:         murphy.xie                                                 *
** Date:           02/06/2012                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe kelock internal interface            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012        murphy.xie	   Create
******************************************************************************/

#ifndef _MMI_KEYLOCK_INTERNAL_H_
#define _MMI_KEYLOCK_INTERNAL_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmikl_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//  Description : get win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMI_KL_WIN_STATUS_E MMIKL_GetWinStatus(void);

/*****************************************************************************/
//  Description : set win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKL_SetWinStatus(MMI_KL_WIN_STATUS_E status);

/*****************************************************************************/
//  Description : reset win status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKL_ReSetWinStatus(void);

/******************************************************************************/
//  Description : get unlock timer id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKL_GetUnlockTimerID(void);

/*****************************************************************************/
//  Description : get tips timer id
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKL_GetTipsTimerID(void);

/*****************************************************************************/
//  Description : get tips timer id
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKL_GetUnlockConfirmTimerID(void);

/*****************************************************************************/
//  Description : StopTipsShowTimer
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_StartTipsShowTimer(MMI_WIN_ID_T win_id );


/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_StopTipsShowTimer(void);

/******************************************************************************/
//  Description : start unlock confirm timer
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_StartUnlockConfirmTimer(
                                   MMI_WIN_ID_T     win_id
                                   );
/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_StopUnlockConfirmTimer(void);

/******************************************************************************/
//  Description : check prompt status (missed call, new sms, new mms)
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_CheckPromptStatus(void);

/*****************************************************************************/
//  Description : MMIKL_CreatWallpaperCtrl
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_CreatWallpaperCtrl(MMISET_KEY_LOCK_EFFECT_E lock_type);

/******************************************************************************/
//  Description : handle kl display window msg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN MMIKL_HandleKLDispWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        );

/*****************************************************************************/
//  Description : is preview state
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_IsPreviewState(void);
#ifdef   __cplusplus
    }
#endif

#endif

