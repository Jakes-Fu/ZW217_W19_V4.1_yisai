/*****************************************************************************
** File Name:      mmiset_call_internal.h                                           *
** Author:                                                                   *
** Date:           2012/02/10                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li            Create
** 2012/02/10    xiyuan.ma           Edit
******************************************************************************/

#ifndef _MMISET_CALL_INTERNAL_H
#define _MMISET_CALL_INTERNAL_H

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiset_security.h"
#include "mmiset_call_export.h"

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init call settings
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CallInit(void);

/*****************************************************************************/
// 	Description : append reject and any key list item
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendAnykeyRejectListItem(
                                              MMI_CTRL_ID_T ctrl_id
                                              );

/*****************************************************************************/ 
// 	Description : set if is r answer
//	Global resource dependence : none
//  Author: scv
//	Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetMinuteReminderTime( 
                                       uint8  call_reminder_time
                                       );

/*****************************************************************************/
// 	Description : set if is connect prompt open
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIsConnectPrompt(
                                      BOOLEAN  is_connect_prompt
                                      );
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
// 	Description : set is flipanswer
//	Global resource dependence : none
//  Author: jibin
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIsFlipAnswer(
                                   MMISET_FLIP_ANSWER_TYPE_T* flip_answer
                                   );
#endif
/*****************************************************************************/ 
// 	Description : set if is auto redial
//	Global resource dependence : none
//  Author: scv
//	Note: 
/*****************************************************************************/ 
//PUBLIC void MMISET_SetIsAutoRedial( 
//                            BOOLEAN  is_auto_redial
//                             );
/*****************************************************************************/
// 	Description : Set default value
//	Global resource dependence : none
//  Author: lin.lin
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetIPNumberDefaultValue(
                                           MN_DUAL_SYS_E           dual_sys,
                                           MMISET_IP_NUMBER_INFO_T *info_ptr
                                           );

/*****************************************************************************/
// 	Description : send new call bar password
//	Global resource dependence : 
//  Author: maql
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SendNewBarPassword(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CallForwarding(
                                  MN_DUAL_SYS_E     dual_sys,
                                  MMI_STRING_T    *voice_number_ptr
                                  );

/*****************************************************************************/
// 	Description : start call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_StartCallForwarding(
                                       MN_DUAL_SYS_E     dual_sys,
                                       MMI_STRING_T    *voice_number_ptr
                                       );

/*****************************************************************************/
// 	Description : 添加无应答时间的单选list items
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendNoAnswerTimeListItem(
                                              MMI_CTRL_ID_T	    ctrl_id
                                              );

/*****************************************************************************/
// 	Description : close call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseCallForwarding(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : interrogate status of ss
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_InterrogateStatus(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set clir type by the hide id menu
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetClirType(MN_DUAL_SYS_E dual_sys, MN_CALL_CLIR_E clir_type);

/*****************************************************************************/
// 	Description : close all call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseAllCallForwarding(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : start call waiting or call bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_StartCallWaitOrBar(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : send call bar password
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SendBarPassword(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
// 	Description : close call waiting or call bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseCallWaitOrBar(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// 	Description : close all call bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseAllCallBar(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Set IP number
//	Global resource dependence : none
//  Author: lin.lin
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetIPNumberInfo(
                                   MMISET_IP_NUMBER_INFO_T *info_ptr
                                   );

/*****************************************************************************/
// 	Description : Get IP number info
//	Global resource dependence : none
//  Author: lin.lin
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_GetIPNumberInfo(
                                   MN_DUAL_SYS_E dual_sys,
                                   MMISET_IP_NUMBER_INFO_T *info_ptr
                                   );

/*****************************************************************************/
// 	Description : register new pw 
//	Global resource dependence : 
//  Author: maql
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ChangeCallBarPassWord(MN_DUAL_SYS_E dual_sys);

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
// 	Description : register call forward 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : interrogate status of ss
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_InterrogateAllCFStatus(MN_DUAL_SYS_E dual_sys);

#ifdef VT_SUPPORT
/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendFallbackListItem(MMI_CTRL_ID_T ctrl_id);  

/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectFallback(MMI_CTRL_ID_T ctrl_id); 

/*****************************************************************************/
// 	Description : MMISET_WriteReplaceInfoToNv
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_WriteReplaceInfoToNv(MMISET_VT_REPLACE_FILE_INFO_T* replace_file_info_ptr);


/*****************************************************************************/
// 	Description : MMISET_SetVTRecordRVDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT录制远端视频和混音的位置，存入nv
/*****************************************************************************/
PUBLIC void MMISET_SetVTRecordRVDAPath(MMIFILE_DEVICE_E record_path);

/*****************************************************************************/
// 	Description : MMISET_SetVTRecordDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT录制混音的位置，存入nv
/*****************************************************************************/
PUBLIC void MMISET_SetVTRecordDAPath(MMIFILE_DEVICE_E device);


/*****************************************************************************/
// 	Description : MMISET_SetVTRecordRVPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT录制远端视频的位置，存入nv
/*****************************************************************************/
PUBLIC void MMISET_SetVTRecordRVPath(MMIFILE_DEVICE_E device);

/*****************************************************************************/
// 	Description : MMISET_ResetVTSetting
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:reset nv里面的VT相关设置
/*****************************************************************************/
PUBLIC void MMISET_ResetVTSetting(void);
#endif

/*****************************************************************************/
// 	Description : is ss processing
//	Global resource dependence : none
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsSSProcessing(void);

/*****************************************************************************/
// 	Description : set is cfu actived
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIsCfuActived(MN_DUAL_SYS_E dual_sys, MMISET_SS_TYPE_E ss_type, BOOLEAN is_actived);

/*****************************************************************************/
// 	Description :get is cfu actived
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetIsCfuActived(MN_DUAL_SYS_E dual_sys, MMISET_SS_TYPE_E ss_type);
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : IP Number context init.
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_IPNumberContextInit(void);

/*****************************************************************************/
// 	Description : Set default IP number info
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDefaultIPNumberInfo(void);

/*****************************************************************************/
// 	Description : Set default IP number info
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDefaultIPNumberInfoToNV(void);

/*****************************************************************************/
// 	Description : Set IP number info pointer
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC MMISET_IP_NUMBER_INFO_T* MMISET_GetIPNumberInfoPtr(MN_DUAL_SYS_E dual_sys);
#endif
#endif
