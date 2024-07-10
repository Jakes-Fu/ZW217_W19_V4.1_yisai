/*****************************************************************************
** File Name:      mmidc_main.h                                              *
** Author:                                                                   *
** Date:           08/13/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera pda                           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_MAIN_H_
#define _MMIDC_MAIN_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "dal_dcamera.h"
#include "dal_recorder.h"

#include "mmidc_setting.h"
#include "mmicom_trace.h"
#include "watch_commonwin_export.h"
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
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef struct 
{
    CAF_APPLET_T caf_applet;
    BOOLEAN  is_first_preview; //first preview 
    BOOLEAN is_running;
    MMIDC_CMD_E cmd; 
}MMIDC_APPLET_T;

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIDC_RequestHandle(void);
                        
/*****************************************************************************/
//  Description : Get current audio handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIDC_GetAudioHandle(void);

/*****************************************************************************/
//  Description : Set current audio handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIDC_SetAudioHandle(MMISRV_HANDLE_T handle);

 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
 /*****************************************************************************/
//Description : create win by applet
//Global resource dependence :none
//Author:robert.wang
//Note: 
/*****************************************************************************/
PUBLIC void MMIDC_CreateWinByApplet(uint32* win_table_ptr, //in
                                             ADD_DATA add_data_ptr //in
                                             );
                                             
/*****************************************************************************/
//Description : save photo 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_Saving(void);

/*****************************************************************************/
//Description : get first preview flag
//Global resource dependence :none
//Author:robert.wang
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetFirstPreviewFlag(void);

/*****************************************************************************/
//Description : set first preview flag
//Global resource dependence :none
//Author:robert.wang
//Note: 
/*****************************************************************************/
PUBLIC void MMIDC_SetFirstPreviewFlag(BOOLEAN is_first_preview);

/*****************************************************************************/
//Description : get dc running flag
//Global resource dependence :none
//Author:robert.wang
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetDCModuleRunningFlag(void);

/*****************************************************************************/
//Description : set dc running flag
//Global resource dependence :none
//Author:robert.wang
//Note: 
/*****************************************************************************/
PUBLIC void MMIDC_SetDCModuleRunningFlag(BOOLEAN is_running);

/*****************************************************************************/
//Description : get capture status by user
//Global resource dependence :none
//Author:robert.wang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetCaptureStatusByUser(void);

/*****************************************************************************/
//Description : process msg cmd
//Global resource dependence :none
//Author:robert.wang
//Note: 
/****************************************************************************/
PUBLIC int32 MMIDC_ProcessMsgCmd(MMIDC_CMD_PARAM_T cmd_param);

/*****************************************************************************/
//  Description : post create file handle msg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PostCreateFileHandleMsg(void);

/*****************************************************************************/
//  Description : post rename file handle msg
//  Global resource dependence : none
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PostRenameFileHandleMsg(void);

/*****************************************************************************/
//  Description : check 2m sensor entry environment
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Check2MSensorEntryEnv(MMIDCSRV_WORK_PARAM_T *work_param //in
                                                          );
                                                          
/*****************************************************************************/
//  Description : check 2m sensor focus enviroment
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_Check2MSensorFocusEnv(void);

/*****************************************************************************/
//Description : get dc module cmd
//Global resource dependence :none
//Author:chunyou
//Note: 
/*****************************************************************************/
PUBLIC MMIDC_CMD_E MMIDC_GetDCModuleCmd(void);

/*****************************************************************************/
//Description : set dc module cmd
//Global resource dependence :none
//Author:chunyou
//Note: 
/*****************************************************************************/
PUBLIC void MMIDC_SetDCModuleCmd(MMIDC_CMD_E cmd);


PUBLIC void MMIDC_CloseTipDisplayTimer(void);
 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#define _MMIDC_MAIN_H_ 
