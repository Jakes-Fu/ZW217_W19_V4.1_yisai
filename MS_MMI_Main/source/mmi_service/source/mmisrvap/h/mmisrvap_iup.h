/*****************************************************************************
** File Name:      mmisrvap_iup.h                                            *
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
#ifndef _MMISRVAP_IUP_H_
#define _MMISRVAP_IUP_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmi_slist.h"

#include "mmisrvap_idown.h"
#include "mmisrvap_fsm.h"
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

/**--------------------------------------------------------------------------*
**                         TYPE AND STRUCT                                  *
**--------------------------------------------------------------------------*/

typedef struct
{
    MMISRVAP_HANDLE cur_handle;
    MMI_SLISTNODEHANDLE_T cur_node;
}MMISRVAP_INFO_T;
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ���ô�����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:ʹ���ڲ�����
/*****************************************************************************/
PUBLIC void AP_SetErrorCode(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_ERROR_E error_code //in
);

/*****************************************************************************/
//  Description : ��ô�����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:ʹ���ڲ�����
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E AP_GetErrorCode(
    MMISRVAP_FSM_T *fsm_ptr //in
);

/*****************************************************************************/
//  Description : ͨ��ap handle��ô�����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E AP_GetErrorCodeByHandle(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : ͨ��ap handle���ô�����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_SetErrorCodeByHandle(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_ERROR_E error_code //in
);

/*****************************************************************************/
//  Description : ����audio handle���ap srv �Ļص�����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AP_GetAPInfoByAudioHandle(
    MMISRV_HANDLE_T audio_handle, //in
    MMISRVAP_HANDLE *ap_out_ptr //out
);

/*****************************************************************************/
//  Description : ����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_Lock(void);

/*****************************************************************************/
//  Description :����
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_UnLock(void);

/*****************************************************************************/
//  Description : �ж�handle�Ƿ���Ч
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AP_IsValidHandle(
    MMISRVAP_HANDLE ap_handle //in
);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif






