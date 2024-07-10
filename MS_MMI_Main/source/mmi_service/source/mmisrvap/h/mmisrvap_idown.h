/*****************************************************************************
** File Name:      mmisrvap_idown.h                                          *
** Author:                                                                   *
** Date:           11/07/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio file srv fsm                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012       robert.wang       Create                                    *
******************************************************************************/
#ifndef _MMISRVAP_IDOWN_H_
#define _MMISRVAP_IDOWN_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmi_signal.h"
#include "mmisrv.h"
#include "mmisrvmgr.h"

#include "mmisrvap_export.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMISRVAP_TASK_MSG_T MmiAudioCallbackMsgS

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    MMISRVAP_ERROR_E error_code;
} MMISRVAP_EVENT_PLAY_PARAM_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 用于处理其他task给srvap发送的消息
//  Global resource dependence :  none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AP_DispatchSignalToAPSrv(
    PWND app_ptr, //in
    uint16 msg_id, //in
    DPARAM param //in
);

/*****************************************************************************/
//  Description : audio srv call back
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AP_AudioSrvCallBack(
    MMISRV_HANDLE_T handle, //in
    MMISRVMGR_NOTIFY_PARAM_T *param //in
);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

