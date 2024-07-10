/****************************************************************************
** File Name:      mmisms_delete.c                                          *
** Author:         liming deng                                                        *
** Date:           8/23/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2007        liming.deng      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#include "mmi_nv.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmipub.h"
#include "guitab.h"
#include "mmisms_filecontrol.h"
#include "mmisms_edit.h"
#include "mmisms_delete.h"
#include "mmisms_send.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmimms_id.h"
#include "mmimms_text.h"
#include "mmiota_id.h"
#include "mmisms_image.h"
#include "mmisms_order.h"
#include "mmisms_set.h"
#include "mmisms_export.h"
#include "mmimms_export.h"
#ifdef MMI_GUI_STYLE_TYPICAL
#include "mmisms_save.h"
#endif
#if defined(MMI_SMS_MULTI_PARAM_SUPPORT) || defined(MMI_GUI_STYLE_TYPICAL)
#include "mmisms_menutable.h"
#endif
#include "mmiudisk_export.h"
#ifdef TTS_SUPPORT
#include "mmicom_panel.h"
#endif
#include "mmisms_internal.h"
#include "mmi_nv.h"
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
#include "mmisms_app.h"
#endif
#include "mmi_pubmenuwin.h"
#ifdef MMI_SMS_EXPORT_SUPPORT
#include "mmisms_filewin.h"
#endif
#include "mmisms_appcontrol.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_SIM_MODE_ITEM 10
#define MMISMS_SIM_SEND_MODE_ITEM 7

#ifdef TTS_SUPPORT
#define MMISMS_TTS_MAX_NUM   2
#endif
#define MMISMS_VP_ITEM        6

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMISMS_PDA_SMS_SETTINGS_INIT,
    
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISMS_PDA_SMS_SETTINGS_SMS1,
    MMISMS_PDA_SMS_SETTINGS_SMS2,

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    MMISMS_PDA_SMS_SETTINGS_SMS3,
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMISMS_PDA_SMS_SETTINGS_SMS4,
#endif         
#else
    MMISMS_PDA_SMS_SETTINGS,
#endif
#else
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISMS_PDA_SMS_SETTINGS_SMS1_SC,
    MMISMS_PDA_SMS_SETTINGS_SMS2_SC,

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    MMISMS_PDA_SMS_SETTINGS_SMS3_SC,
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMISMS_PDA_SMS_SETTINGS_SMS4_SC,
#endif         
#else
    MMISMS_PDA_SMS_SETTINGS_SC,
#endif
    MMISMS_PDA_SMS_SETTINGS_VALID_PERIOD,    
#endif
    MMISMS_PDA_SMS_SETTINGS_STATUS_REPORT,
#ifdef MMI_MSG_RESEND
   MMISMS_PDA_SMS_SETTINGS_RESEND,
#endif
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    MMISMS_PDA_SMS_SETTINGS_REPLY_PATH,
#endif
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    MMISMS_PDA_SMS_SETTINGS_CONCATENATED_SETTING,
#endif
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    MMISMS_PDA_SMS_SETTINGS_ALPHABET_SETTING,
#endif
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
    MMISMS_PDA_SMS_SETTINGS_SAVE_REQUEST,   
#endif    
    MMISMS_PDA_SMS_SETTINGS_SAVE_POS,
#if defined(MMI_SMS_EXPORT_SUPPORT)
    MMISMS_PDA_SMS_SETTINGS_EXPORT,
#endif
    MMISMS_PDA_SMS_SETTINGS_TTS, 
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    MMISMS_PDA_SMS_SETTINGS_AUTO_SIGNATURE_FLAG,
    MMISMS_PDA_SMS_SETTINGS_AUTO_SIGNATURE,
#endif
#ifdef MMI_SMS_FIX_SEND_SIM
    MMISMS_PDA_SMS_SETTINGS_FIX_SIMCARD_SEND,
#endif
    MMISMS_PDA_SMS_SETTINGS_MAX
    
}MMISMS_PDA_SETTINGS_TYPE_E;

typedef enum
{
    MMISMS_PUSH_MSG_SETTING_RECEIVING_OPT,
    MMISMS_PUSH_MSG_SL_OPERATION,
    MMISMS_PUSH_MSG_SETTING_MAX
} MMISMS_PUSH_MSG_SETTING_E;

typedef enum
{
    MMISMS_PDA_SMS_TTS_INIT,
    MMISMS_PDA_SMS_TTS_STATUS,    
    MMISMS_PDA_SMS_TTS_VOLUME,   
    MMISMS_PDA_SMS_TTS_MAX    
}MMISMS_PDA_TTS_SETTING_TYPE_E;
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
typedef enum
{
    MMISMS_PDA_SMS_PARAM_SETTINGS_INIT,
    MMISMS_PDA_SMS_PARAM_SETTINGS_NAME,    
    MMISMS_PDA_SMS_PARAM_SETTINGS_SC,
    MMISMS_PDA_SMS_PARAM_SETTINGS_VALID_PERIOD,    
    MMISMS_PDA_SMS_PARAM_SETTINGS_SEND_MODE,   
    MMISMS_PDA_SMS_PARAM_SETTINGS_MAX
}MMISMS_PDA_SMS_PARAM_TYPE_E;
#endif
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
typedef enum
{
    MMISMS_BIG_FONT_SIZE,
    MMISMS_NORMAL_FONT_SIZE,
    MMISMS_SMALL_FONT_SIZE,
    MMISMS_FONT_SIZE_MAX
}MMISMS_FONT_SIZE_E;
#endif
typedef enum
{
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    MMISMS_MMS_FONT_SIZE,
#endif
    MMISMS_COMMON_SETTING_MAX
}MMISMS_COMMON_SETTING_E;

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
typedef struct
{
    MMISMS_SET_SIMMODE_T set_mode;
#if 0
    MMI_WIN_ID_T        win_id;
#endif
} MMISMS_SETTING_MAIN_WIN_PARAM_T;
#endif


/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
extern MMISMS_GLOBAL_T g_mmisms_global;
#endif
#endif

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to handle the SMS setting menu window message
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM            param
                                              );

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : init browser info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitSettingBrowser(
                         MMI_CTRL_ID_T  ctrl_id
                         );

/*****************************************************************************/
//  Description : init browser info service loading operation
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitSettingServiceLoadingOperation(
                         MMI_CTRL_ID_T  ctrl_id
                         );
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
/*****************************************************************************/
//  Description : init browser info
//  Global resource dependence : 
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void InitCommonSettingsListCtrl(
                         MMI_CTRL_ID_T  ctrl_id
                         );
#endif

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : to handle set browser information
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBrowserInfoWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

/*****************************************************************************/
//  Description : SetSmsSettingMenuListItemData
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetSmsSettingMenuListItemData(
                            GUILIST_ITEM_T               *item_t,
                            GUILIST_ITEM_DATA_T      *item_data,
                            MMI_TEXT_ID_T       txt_id_1,
                            MMI_TEXT_ID_T       txt_id_2
                            );

/*****************************************************************************/
//  Description : to handle set Service Loading Operation
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetServiceLoadingOperationWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : to handle set Push Message Settings
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePushMessagesSettingWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
#endif
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
/*****************************************************************************/
//  Description : to handle set common information for message
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetOthersInfoWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : add all tabs and all child windows to parent window.
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AddSettingsTabChildWindows(void);

#if defined(MMI_SMS_EXPORT_SUPPORT) || defined(TTS_SUPPORT)
/*****************************************************************************/
//  Description : append pda settings item 1 line
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendSmsPdaSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 );
#endif

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendSmsPdaSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data,   //用户数据
                                 BOOLEAN is_need_replace,
                                 GUIITEM_STYLE_E item_style
                                 );


#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsTTSListCtrl(void);
#endif

/*****************************************************************************/
//  Description :Init pda sms setting list
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void InitPdaSmsSettingsListCtrl(BOOLEAN is_need_replace);
#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description :HandleTTSListWindow
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTTSListWindow(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM            param
                                              );

#endif 

/*****************************************************************************/
//  Description :HandleSettingsMainWindow
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingsMainWindow(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM            param
                                              );

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
/*****************************************************************************/
//  Description :font size setting for sms_mms_in_1
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsMmsFontSizeSettingWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#endif

#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : to handle set TTS
//	Global resource dependence : 
//  Author:barry.peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsSetTTSWindow(
                                          MMI_WIN_ID_T       win_id, 
                                          MMI_MESSAGE_ID_E   msg_id, 
                                          DPARAM             param
                                          );
#endif 

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : to handle window message about setting Concatenated SMS
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingConcatenatedSMSWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               );
#endif
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
/*****************************************************************************/
//     Description : to handle window message about setting alphabet management
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingAlphabetManagementWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               );
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : to set the SMS status window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingSmsStatusWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM            param
                                                 );

/*****************************************************************************/
//  Description : to handle window message about setting reply path flag.
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingReplyPathFlagWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM            param
                                                    );

/*****************************************************************************/
//  Description : to open the window to set the flag of reply path
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void OpenSetReplyPathWin(void);
#endif
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jixin.xu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SettingwinMainCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : to select sim param
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectSIMParamWinMsg(
                                              MMI_WIN_ID_T        win_id,        //IN:
                                              MMI_MESSAGE_ID_E    msg_id,        //IN:
                                              DPARAM                param        //IN:
                                             );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : HandleSMSParamListLongOKMenuWinOKMsg
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSParamListLongOKMenuWinOKMsg( MMIPUB_MENU_NOTIFY_T* param );
#if 0
/*****************************************************************************/
// 	Description : HandleSMSParamListLongOKMenuWinMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSParamListLongOKMenuWinMsg(	
									 MMI_WIN_ID_T win_id, 
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param);
#endif
#endif

/*****************************************************************************/
//  Description : to set the sim mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetSIMMode(MN_DUAL_SYS_E dual_sys, BOOLEAN is_open, uint16 mode);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : InitPdaSmsParamSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsParamSettingsListCtrl(BOOLEAN is_need_replace, MMI_CTRL_ID_T ctrl_id, MN_DUAL_SYS_E dual_sys, uint8 mode_id);
#endif

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode menu window message
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingSIMModeMenuWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM            param
                                                    );

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode name window message
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingSIMModeNameWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM            param
                                                    );

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode send mode window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingSIMModeSendmodeWinMsg(
                                                         MMI_WIN_ID_T        win_id,        //IN:
                                                         MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                         DPARAM                param        //IN:
                                                        );

/*****************************************************************************/
//  Description : to set sim send mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetSIMSendmode(MMISMS_SET_SIMMODE_T set_mode);

/*****************************************************************************/
//  Description : to save the sim mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SaveSIMModeToGlobal(
                               MN_DUAL_SYS_E dual_sys,
                               MMISMS_SET_SIMMODE_T *set_mode_ptr
                              );

/*****************************************************************************/
//  Description : 将名称转化为字符串
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void ConvertSimToModeName(
                                MMI_STRING_T *mode_name,    //OUT
                                uint8           sim_alpha_len,  //IN:
                                uint8           *sim_alpha_ptr  //IN:
                               );
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
// Description : set volume during call
// Global resource dependence :
// Author: 
// Note:
/*****************************************************************************/
LOCAL void SetTTSVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description : Open Volume Panel
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenTTSVolumePanel(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description :  HOOK Key events 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E TTSVolumeWinHookFunc(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
#ifdef MMI_SMS_FIX_SEND_SIM

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode send fix sim window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFixSimSendingListWindow(
                                                         MMI_WIN_ID_T        win_id,        //IN:
                                                         MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                         DPARAM                param        //IN:
                                                        );
#endif
#if 1

/*****************************************************************************/
//     Description : to set the valid period
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void SetValidPeriod(
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
MMISMS_SET_SIMMODE_T set_mode
#else
void
#endif
);

/*****************************************************************************/
//     Description : to open set valid period win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetVPWin(
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
MMISMS_SET_SIMMODE_T set_mode
#else
void
#endif
);

/*****************************************************************************/
//     Description : to handle the valid period win message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectVPWinMsg(
                                        MMI_WIN_ID_T        win_id,        //IN:
                                        MMI_MESSAGE_ID_E    msg_id,        //IN:
                                        DPARAM                param        //IN:
                                        );

/*****************************************************************************/
//     Description : to open the window to set the save position
//    Global resource dependence : 
//  Author: Liqing Peng
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetSavePosWin(void);

/*****************************************************************************/
//  Description : to handle window message about setting save postion
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSavePosWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               );
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
/*****************************************************************************/
//     Description : to handle the input sms signature win message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputSignatureWinMsg(
                                               MMI_WIN_ID_T        win_id,    //IN:
                                               MMI_MESSAGE_ID_E msg_id,    //IN:
                                               DPARAM            param    //IN:
                                               );
#endif

/*****************************************************************************/
//     Description : to handle the input sc number win message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputSCWinMsg(
                                       MMI_WIN_ID_T        win_id,    //IN:
                                       MMI_MESSAGE_ID_E msg_id,    //IN:
                                       DPARAM            param    //IN:
                                       );

/******************* **********************************************************/
//     Description : to open set sc win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetSCWin(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleWriteSMSParamWaitWinMsg(
                                     MMI_WIN_ID_T win_id,    //IN:
                                     MMI_MESSAGE_ID_E msg_id,    //IN:
                                     DPARAM param    //IN:
                                     );


/*****************************************************************************/
//     Description : to handle the message of waiting window
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSMSWaitingWin(MMI_WIN_ID_T win_id,
                                                                                MMIPUB_SOFTKEY_STYLE_E softkey_style,
                                                                                MMIPUB_HANDLE_FUNC handle_func,
                                                                                uint32 user_data);
/*****************************************************************************/
//     Description : to open the window to set the flag of status report
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetStatusReportWin(void);

/*****************************************************************************/
//     Description : to set the MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_SetEnableWin( 
                        BOOLEAN is_enable 
                        );

#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to handle auto save window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoSaveWinMsg(
                                        MMI_WIN_ID_T        win_id,        //IN:
                                        MMI_MESSAGE_ID_E    msg_id,        //IN:
                                        DPARAM                param        //IN:
                                        );
/*****************************************************************************/
//     Description : to open the window to set the flag of autosave
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void    MMISMS_OpenSetAutoSaveWin( void );
#endif

/*****************************************************************************/
//     Description : to handle window message about setting status report flag.
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingStatusReportFlagWinMsg(
                                                       MMI_WIN_ID_T        win_id, 
                                                       MMI_MESSAGE_ID_E msg_id, 
                                                       DPARAM            param
                                                       );


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
// the window for SMS setting menu
#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISMS_SMS_SETTING_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsSettingMenuWinMsg),    
    WIN_ID( MMISMS_SMS_SETTING_MENU_WIN_ID ),

#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
#if !defined(MMS_SUPPORT) && !defined(BROWSER_SUPPORT)
    WIN_TITLE(TXT_MESSAGE_SETTING_SMS),   
#endif

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SMS_SETTING_MENU_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE( MMISMS_SMS_SETTING_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsSettingMenuWinMsg),    
    WIN_ID( MMISMS_SMS_SETTING_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SMS_SETTING, MMISMS_SMS_SETTING_MENU_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
// the window of setting preferred connection
WINDOW_TABLE(MMISMS_SETTING_PREFERRED_CONNECTION) = 
{
    WIN_TITLE( TXT_SMS_PREFERRED_CONNECTION),
    WIN_FUNC((uint32) HandleSettingPreferredConnectionWinMsg),    
    WIN_ID( MMISMS_PREFERRED_CONNECTION_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_PERFERRED_CONNECTION_LISTBOX_CTRL_ID ),
    END_WIN
};
#endif

// the window of setting status report flag
WINDOW_TABLE( MMISMS_SETTING_STATUS_REPORT_FLAG_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_STATUSREPORT),
    WIN_FUNC((uint32) HandleSettingStatusReportFlagWinMsg),    
    WIN_ID( MMISMS_SELECTSR_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID ),
    END_WIN
};

#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
// the window  to selecting the vp     2.9.2.
WINDOW_TABLE( MMISMS_SELECTSR_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_AUTO_SAVE ),
    WIN_FUNC((uint32) HandleAutoSaveWinMsg ),    
    WIN_ID( MMISMS_SELECTSR_WIN_ID ),
    #ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID ),
    END_WIN
};
#endif

#if 1
// the window  to selecting the vp     2.9.2.
WINDOW_TABLE( MMISMS_SELECTVP_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_SETTING_VP ),
    WIN_FUNC((uint32) HandleSelectVPWinMsg ),    
    WIN_ID( MMISMS_SELECTVP_WIN_ID ),
    #ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID ),
    END_WIN
};

// the window of setting save pos
WINDOW_TABLE( MMISMS_SETTING_SAVE_POS_WIN_TAB ) = 
{
    WIN_TITLE( TXT_PRIOR_STORE_MEDIUM),
    WIN_FUNC((uint32) HandleSettingSavePosWinMsg),    
    WIN_ID( MMISMS_SELECTSAVEPOS_WIN_ID ),
    #ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SELECTSAVEPOSWIN_LISTBOX_CTRL_ID ),
    END_WIN
};

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
// the window of  auto signature for message box
WINDOW_TABLE( MMISMS_INPUT_SIGNATURE_WIN_TAB ) = 
{
    WIN_TITLE( TXT_CMS_MMS_SIGN ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleInputSignatureWinMsg ),    
    WIN_ID( MMISMS_INPUT_SIGNATURE_WIN_ID ),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL ),
    CREATE_EDIT_TEXT_CTRL(MMISMS_MAX_SIGNATURE_NUMBER_LEN, 
                          MMISMS_INPUT_SIGNATURE_WIN_TEXT_CTRL_ID),
    END_WIN
};
#endif

// the window of  2.9.1.  for message box
WINDOW_TABLE( MMISMS_INPUTSC_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_SETTING_SC ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleInputSCWinMsg ),    
    WIN_ID( MMISMS_INPUTSC_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL ),
    CREATE_EDIT_PHONENUM_CTRL(MMISMS_MAX_SC_NUMBER_LEN, MMISMS_INPUTSCWIN_PHONENUM_CTRL_ID),
    END_WIN
};
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISMS_SETTINGS_TAB_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSettingsMainWindow),    
    WIN_ID( MMISMS_SETTINGS_TAB_WIN_ID),
#if defined(MMI_GUI_STYLE_TYPICAL) || defined(MMI_SMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMISMS_SETTINGS_TAB_CTRL_ID),
#else
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMISMS_SETTINGS_TAB_CTRL_ID),
#endif
    END_WIN
};

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
WINDOW_TABLE( MMISMS_MMS_FONTSIZE_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSmsMmsFontSizeSettingWinMsg),    
        WIN_ID(MMISMS_MMS_FONT_SIZE_WIN_ID),
        WIN_TITLE(TXT_COMMON_FONT_SIZE),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID),
        END_WIN
};
#endif

#endif

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TTS_SUPPORT
WINDOW_TABLE( MMISMS_SET_TTS_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTTSListWindow),
    WIN_ID(MMISMS_TTS_LIST_WIN_ID),
    WIN_TITLE( TXT_SET_TTS ),    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SMS_SETTING_TTSLIST_CTRL_ID),
#ifndef MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif
#endif

#ifdef BROWSER_SUPPORT
WINDOW_TABLE( MMISMS_BROWSER_SETTING_MENUWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetBrowserInfoWindow ),    
    WIN_ID( MMISMS_BROWSER_SETTING_MENUWIN_ID ),

    //#ifdef MMI_PDA_SUPPORT
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_BROWSER_SETTING_CTRL_ID),
    //#else
    //CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_BROWSER_SETTING_CTRL_ID),
    //#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_BROWSER_SETTING_CTRL_ID),
    

#ifndef MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif

    END_WIN
};

#ifdef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISMS_SMS_PUSH_MESSAGES_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePushMessagesSettingWindow),    
    WIN_ID( MMISMS_SMS_PUSH_MESSAGES_WIN_ID ),
    WIN_TITLE( TXT_SMS_BROWSER_INFO ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SMS_PUSH_MESSAGES_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE( MMISMS_SERIVE_LOADING_OPERATION_SETTING_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetServiceLoadingOperationWindow ),    
    WIN_ID( MMISMS_SERVICE_LOADING_OPERATION_SETTING_WIN_ID ),
    WIN_TITLE( TXT_SMS_SERVICE_LOADING_OPERATION ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SERVICE_LOADING_OPERATION_SETTING_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT) && !defined(MMI_GUI_STYLE_TYPICAL)
WINDOW_TABLE( MMISMS_OTHERS_SETTING_MENUWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetOthersInfoWindow ),    
    WIN_ID( MMISMS_OTHERS_SETTING_MENUWIN_ID ),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_COMMON_SETTING_MENU_CTRL_ID),

    END_WIN
};
#endif

#ifdef TTS_SUPPORT
WINDOW_TABLE( MMISMS_SET_TTS_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSmsSetTTSWindow ),    
    WIN_ID( MMISMS_SET_TTS_WIN_ID ),
    WIN_TITLE( TXT_SET_TTS_AUTO_READ ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SET_TTS_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
#ifndef MMI_PDA_SUPPORT
// the window of setting concatenated sms
WINDOW_TABLE( MMISMS_SETTING_CONCATENATED_SMS_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_CONCATENATED_SMS),
    WIN_FUNC((uint32) HandleSettingConcatenatedSMSWinMsg),    
    WIN_ID( MMISMS_CONCATENATED_SMS_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_CONCATENATED_SMS_LISTBOX_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
// the window of setting alphabet management
WINDOW_TABLE( MMISMS_SETTING_ALPHABET_MANAGEMENT_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_ALPHABET_MANAGEMENT),
    WIN_FUNC((uint32) HandleSettingAlphabetManagementWinMsg),    
    WIN_ID( MMISMS_ALPHABET_MANAGEMENT_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_ALPHABET_MANAGEMENT_LISTBOX_CTRL_ID ),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
#ifdef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMISMS_SETTING_STATUS_TAB ) = 
{
    WIN_FUNC((uint32) HandleSettingSmsStatusWinMsg),
    WIN_ID( MMISMS_SETTING_STATUS_WIN_ID ),
    WIN_TITLE(TXT_SMS_SETTING_STATUS),
    CREATE_MENU_CTRL(MENU_SETTING_STATUS_OPT, MMISMS_SETTING_STATUS_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

WINDOW_TABLE( MMISMS_SETTING_REPLY_PATH_FLAG_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSettingReplyPathFlagWinMsg),    
    WIN_ID( MMISMS_SETTING_REPLY_PATH_FLAG_WIN_ID ),
    WIN_TITLE(TXT_SMS_REPLY_PATH),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
WINDOW_TABLE( MMISMS_SELECTSIMMODE_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSelectSIMParamWinMsg ),    
    WIN_ID( MMISMS_SELECTSIMMODE_WIN_ID ),
    WIN_TITLE( TXT_COMMON_PARAM_SETTINGS ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID ),
    WIN_SOFTKEY(STXT_SELECT, TXT_EDIT, STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
#if 0
WINDOW_TABLE(MMISMS_SELECTSIMMODE_LONGOK_WIN_TAB)=
{
    WIN_FUNC( (uint32)HandleSMSParamListLongOKMenuWinMsg),
    WIN_ID(MMISMS_SELECTSIMMODE_LONGOK_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_SMS_PARAM_LINKLIST_LONGOK, MMISMS_SELECTSIMMODE_LISTBOX_LONGOK_CTRL_ID),
    END_WIN
};
#endif
#endif

WINDOW_TABLE( MMISMS_SETTING_SIMMODE_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsSettingSIMModeMenuWinMsg),    
    WIN_ID( MMISMS_SETTING_SIMMODE_MENUWIN_ID ),
    WIN_TITLE(TXT_COMMON_PARAM_SETTINGS),
#ifdef MMI_PDA_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SETTING_SIMMODE_MENU_CTRL_ID),
#else
    CREATE_MENU_CTRL(MENU_SMS_SETTING_SIMMODE, MMISMS_SETTING_SIMMODE_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(STXT_SAVE, TXT_EDIT, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMISMS_SET_SIMMODE_NAME_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsSettingSIMModeNameWinMsg ),
    WIN_TITLE( TXT_SMS_SIM_MODE_NAME ),
    WIN_ID( MMISMS_SET_SIMMODE_NAME_WIN_ID ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_EDIT_TEXT_CTRL(00,MMISMS_SETTING_SIMMODE_NAME_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMISMS_SET_SIMMODE_SENDMODE_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_SIM_SEND_MODE ),
    WIN_FUNC((uint32) HandleSmsSettingSIMModeSendmodeWinMsg ),    
    WIN_ID( MMISMS_SET_SIMMODE_SENDMODE_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID ),
    END_WIN
};
#endif
#ifdef MMI_SMS_FIX_SEND_SIM

WINDOW_TABLE( MMISMS_SET_FIXSIM_SENDING_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFixSimSendingListWindow),
    WIN_ID(MMISMS_FIXSIM_SENDING_LIST_WIN_ID),
    WIN_TITLE( TXT_SMS_FIX_SIM_SEND ),    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SMS_SETTING_FIXSIMLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create setting win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateSmsSettingWin(void)
{
#ifdef MMI_GUI_STYLE_TYPICAL   
    MMK_CreateWin((uint32 *)MMISMS_SMS_SETTING_MENUWIN_TAB, PNULL);
#else
    MMISMS_EnterSettingMainWin();
#endif
}

/*****************************************************************************/
//  Description : create setting win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateBrowserSettingWin(void)
{
#ifdef MMI_GUI_STYLE_TYPICAL
    MMK_CreateWin((uint32 *)MMISMS_SMS_PUSH_MESSAGES_WIN_TAB, PNULL);
#endif
}
#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : create setting win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CreateServiceLoadingOperationSettingWin(MMI_CTRL_ID_T ctrl_id)
{
    MMK_CreatePubListWin((uint32 *)MMISMS_SERIVE_LOADING_OPERATION_SETTING_TAB, (ADD_DATA)ctrl_id);
}
#endif
#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : to handle set TTS
//	Global resource dependence : 
//  Author:barry.peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsSetTTSWindow(
                                          MMI_WIN_ID_T    win_id, 
                                          MMI_MESSAGE_ID_E   msg_id, 
                                          DPARAM             param
                                          )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISMS_SET_TTS_CTRL_ID,MMISMS_TTS_MAX_NUM, FALSE );
        MMISMS_AppendTTSListItem(MMISMS_SET_TTS_CTRL_ID);        
        MMK_SetAtvCtrl(win_id,MMISMS_SET_TTS_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (!GUILIST_IsCurrItemInvalid(MMISMS_SET_TTS_CTRL_ID))
        {
            MMISMS_SetTTSInfo(MMISMS_SET_TTS_CTRL_ID); 
            //success!
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
// Description : set volume during call
// Global resource dependence :
// Author: 
// Note:
/*****************************************************************************/
LOCAL void SetTTSVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if (PNULL != para_ptr)
    {
        MMISMS_SetTTSVol((uint8)para_ptr->cur_value);

#ifndef MMI_GUI_STYLE_TYPICAL
        if (MMK_IsOpenWin(MMISMS_TTS_LIST_WIN_ID))
        {
            MMK_PostMsg(MMISMS_TTS_LIST_WIN_ID, MMISMS_UPDATE_LIST, PNULL, NULL);
            MMK_PostMsg(MMISMS_TTS_LIST_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
        }
#else
        if (MMK_IsOpenWin(MMISMS_SMS_SETTING_MENU_WIN_ID))
        {
            MMK_PostMsg(MMISMS_SMS_SETTING_MENU_WIN_ID, MMISMS_UPDATE_LIST, PNULL, NULL);
            MMK_PostMsg(MMISMS_SMS_SETTING_MENU_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
        }
#endif
    } 
}

/*****************************************************************************/
//  Description : Open Volume Panel
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenTTSVolumePanel(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    if (!MMK_IsOpenWin(MMICOMMON_PANEL_WIN_ID))
    {
        MMICOM_PANEL_INFO_T panel_info = {0};

        panel_info.x = -1;
        panel_info.y = -1;
        //panel_info.open_type = turn_vol_type;
        panel_info.panel_type = MMICOM_PANEL_VOLUME;
        panel_info.cur_value = MMISMS_GetTTSVol();
        panel_info.last_value = MMISMS_GetTTSVol();//恢复默认值的时候用的
        panel_info.min_value = MMISET_VOL_ZERO;
        panel_info.max_value = MMISET_VOL_MAX;
        panel_info.set_callback = SetTTSVolumeCallback;
        MMIAPICOM_OpenPanelChildWin(win_id, &panel_info);
    }
    else
    {
        MMK_SendMsg(MMICOMMON_PANEL_WIN_ID, msg_id, PNULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :  HOOK Key events 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E TTSVolumeWinHookFunc(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    switch(msg_id)
    {
    case MSG_APP_UP:
    case MSG_APP_LEFT:
    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
        if (MMK_IsOpenWin(MMICOMMON_PANEL_WIN_ID))
        {
            result = OpenTTSVolumePanel(win_id, msg_id);
        }
        break;
    
    default:
        break;
    }
    
    return  result;
}
#endif

#if 1
/*****************************************************************************/
//     Description : to set the sc addr called by window
//    Global resource dependence : none
//  Author:louis.wei
//    Note:设置信息服务中心号码
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_AppSetSCAddr(                    //RETURN:
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
                                      MMISMS_SET_SIMMODE_T *set_mode_ptr,
#endif
                                      MN_DUAL_SYS_E   dual_sys,
                                      uint8  *sc_addr,    //IN:
                                      uint8  addr_len        //IN:
                                      )
{
    BOOLEAN ret_val = FALSE;
    MN_CALLED_NUMBER_T  sc_addr_t = {MN_NUM_TYPE_UNKNOW, MN_NUM_PLAN_UNKNOW, 0, 0};
    MMI_PARTY_NUMBER_T  party_num_t = {MN_NUM_TYPE_UNKNOW,0,0};
#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
    ERR_MNSMS_CODE_E    retcode = ERR_MNSMS_NONE;    
    MN_RETURN_RESULT_E result  = MN_RETURN_FAILURE;
#endif

    if (PNULL != sc_addr && addr_len > 0)
    {
        // translate the string number to bcd code
        SCI_MEMSET(sc_addr_t.party_num, 0xFF, MN_MAX_ADDR_BCD_LEN);
        MMIAPICOM_GenPartyNumber(sc_addr, addr_len, &party_num_t);
        
        if (0 < party_num_t.num_len)
        {
            MMIPARTYNUM_2_MNCALLEDNUM((&party_num_t), (&sc_addr_t));
            sc_addr_t.number_plan  = MN_NUM_PLAN_ISDN_TELE;
            
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
            set_mode_ptr->tp_sc_addr_present = TRUE;
            SCI_MEMSET(&set_mode_ptr->tp_sc_addr_t,0x0,sizeof(MN_CALLED_NUMBER_T));
            SCI_MEMCPY(&set_mode_ptr->tp_sc_addr_t,&sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
            MMISMS_Set_Setmode(set_mode_ptr);
#if 0
            g_mmisms_global.set_mode.tp_sc_addr_present = TRUE;
            SCI_MEMSET(&g_mmisms_global.set_mode.tp_sc_addr_t,0x0,sizeof(MN_CALLED_NUMBER_T));
            SCI_MEMCPY(&g_mmisms_global.set_mode.tp_sc_addr_t,&sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
            MMISMS_SaveSIMModeToMN();
#endif
            ret_val = TRUE;
#else
            retcode = MNSMS_SetScAddrEx(dual_sys, &sc_addr_t);
            if (ERR_MNSMS_NONE == retcode)
            {
                result  = MN_RETURN_SUCCESS;
            }
            if (MN_RETURN_SUCCESS == result)
            {
                ret_val = TRUE;
            }
#endif
        }
    }
    
    return (ret_val);
}


/*****************************************************************************/
//     Description : to set the valid period
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void SetValidPeriod(
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
MMISMS_SET_SIMMODE_T set_mode
#else
void
#endif
)
{
    uint8 i = 0;
    uint16 cur_selection = 0;
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MN_SMS_TIME_SECOND_T valid_period = MMISMS_VALID_PERIOD_MAX;
#else
    MMINV_SMS_VALID_PERIOD_T valid_period = MMISMS_VALID_PERIOD_MAX;
#endif
    
    GUILIST_SetMaxItem(MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID, MMISMS_VP_ITEM, FALSE );//max item 6
    // add the item to listbox
    for (i = 0; i < MMISMS_VP_ITEM; i++)
    {
        MMIAPISET_AppendListItemByTextId( 
                                (TXT_SMS_VP_1HOUR + i),
                                TXT_COMMON_OK, 
                                MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO
                                );
    }
    
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    if(set_mode.tp_vp_present)
    {
        valid_period = set_mode.vp_second;
    }
    else
    {
        valid_period = MMISMS_VALID_PERIOD_MAX;
    }
#else
    valid_period = MMISMS_GetValidPeriod();
#endif

    switch (valid_period)
    {
    case MMISMS_VALID_PERIOD_3600:
        cur_selection = 0;
        break;

    case MMISMS_VALID_PERIOD_21600:
        cur_selection = 1;
        break;

    case MMISMS_VALID_PERIOD_86400:
        cur_selection = 2;
        break;

    case MMISMS_VALID_PERIOD_259200:
        cur_selection = 3;
        break;

    case MMISMS_VALID_PERIOD_604800:
        cur_selection = 4;
        break;

    case MMISMS_VALID_PERIOD_MAX:
        cur_selection = 5;
        break;

    default:
        valid_period = MMISMS_VALID_PERIOD_MAX;
        cur_selection = 5;
        break;
    }
        
    GUILIST_SetSelectedItem(MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID, cur_selection, TRUE);
    GUILIST_SetCurItemIndex( MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID, cur_selection);
}

/*****************************************************************************/
//     Description : to open set valid period win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetVPWin(
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
MMISMS_SET_SIMMODE_T set_mode
#else
void
#endif
)
{

    MMK_CreatePubListWin((uint32*) MMISMS_SELECTVP_WIN_TAB, PNULL );

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    SetValidPeriod(set_mode);
#else
    SetValidPeriod();
#endif
    
}

/*****************************************************************************/
//     Description : to handle the valid period win message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectVPWinMsg(
                                        MMI_WIN_ID_T        win_id,        //IN:
                                        MMI_MESSAGE_ID_E    msg_id,        //IN:
                                        DPARAM                param        //IN:
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MN_SMS_TIME_SECOND_T valid_period = 0;
#else
    MMINV_SMS_VALID_PERIOD_T valid_period = 0;
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT    
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        MMK_SetAtvCtrl(win_id, MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID);
        #ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        #endif
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID);
        switch (cur_selection)
        {
        case 0:
            valid_period = MMISMS_VALID_PERIOD_3600;
            break;

        case 1:
            valid_period = MMISMS_VALID_PERIOD_21600;
            break;

        case 2:
            valid_period = MMISMS_VALID_PERIOD_86400;
            break;

        case 3:
            valid_period = MMISMS_VALID_PERIOD_259200;
            break;

        case 4:
            valid_period = MMISMS_VALID_PERIOD_604800;
            break;

        case 5:
            valid_period = MMISMS_VALID_PERIOD_MAX;
            break;

        default:
            valid_period = MMISMS_VALID_PERIOD_MAX;
            break;
        }

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
        //g_mmisms_global.set_mode.tp_vp_present = TRUE;
        //g_mmisms_global.set_mode.vp_second = valid_period;
        param_to_create_setting_main_win->set_mode.tp_vp_present = TRUE;
        param_to_create_setting_main_win->set_mode.vp_second = valid_period;
        MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);

        MMISMS_SaveSIMModeToMN();

        MMISMS_OpenSMSWaitingWin(MMISMS_WRITE_SMS_PARAM_WAITING_WIN_ID,
                                                            MMIPUB_SOFTKEY_NONE,
                                                            MMISMS_HandleWriteSMSParamWaitWinMsg,
                                                            win_id);
#else
        MMISMS_SetValidPeriod(valid_period);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin(win_id);
#endif
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//     Description : to open the window to set the save position
//    Global resource dependence : 
//  Author: Liqing Peng
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetSavePosWin(void)
{
	MMK_CreatePubListWin((uint32*)MMISMS_SETTING_SAVE_POS_WIN_TAB, PNULL);
}

/*****************************************************************************/
//     Description : to handle window message about setting save postion
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSavePosWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SELECTSAVEPOSWIN_LISTBOX_CTRL_ID;
    MMISMS_SMS_SAVEPOS_TYPE_E   pos = MMISMS_SAVEPOS_NONE;

    //SCI_TRACE_LOW:"HandleSettingSavePosWinMsg msg_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_20660_112_2_18_2_55_3_812,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        // add the item to listbox, SIM and Phone
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextId( TXT_COMM_SIM, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId( TXT_PHONE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        pos = MMISMS_GetSavePos();
        //SCI_TRACE_LOW:"HandleSettingSavePosWinMsg pos is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_20671_112_2_18_2_55_3_813,(uint8*)"d",pos);

        switch(pos)
        {
        case MMISMS_SAVEPOS_SIM:
            cur_selection = 0;
            break;

        case MMISMS_SAVEPOS_PHONE:
            cur_selection = 1;
            break;

        default:
            cur_selection = 0;
            break;
        }
        
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch (cur_selection)
        {
        case 0:
            pos = MMISMS_SAVEPOS_SIM;
            break;

        case 1:
            pos = MMISMS_SAVEPOS_PHONE;
            break;
            
        default:
            pos = MMISMS_SAVEPOS_SIM;
            break;
        }
        
        MMISMS_SetSavePos(pos);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
/*****************************************************************************/
//     Description : to handle the input sms signature win message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputSignatureWinMsg(
                                               MMI_WIN_ID_T     win_id, //IN:
                                               MMI_MESSAGE_ID_E msg_id, //IN:
                                               DPARAM           param   //IN:
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id= MMISMS_INPUT_SIGNATURE_WIN_TEXT_CTRL_ID;
    MMI_STRING_T num_string = {0};
    
    //SCI_TRACE_LOW:"MMISMS: HandleInputSignatureWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6096_112_2_18_2_54_32_737,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //to get the input string
        SCI_MEMSET(&num_string, 0, sizeof (MMI_STRING_T));
        GUIEDIT_GetString(ctrl_id, &num_string);
            
        MMISMS_SetAutoSignatureContent(num_string.wstr_ptr, num_string.wstr_len);

#ifdef MMI_GUI_STYLE_TYPICAL
        //提示保存成功
        MMIPUB_OpenAlertWinByTextId(PNULL,STXT_SAVE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
#endif

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

#endif
/*****************************************************************************/
// 	Description : is sc addr valid(only number & '+')
//	Global resource dependence : 
//  Author:louis.wei
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IsSCAddrValid(MMI_STRING_T *num_str_ptr)
{
    uint32 i = 0;
    BOOLEAN is_valid = TRUE;

    if (PNULL == num_str_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:IsSCAddrValid num_str_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6052_112_2_18_2_54_32_736,(uint8*)"");
        return FALSE;
    }
    if (0 < num_str_ptr->wstr_len)
    {
        for (i = 0; i < num_str_ptr->wstr_len; i++)
        {
            if (('+' != num_str_ptr->wstr_ptr[i] &&
                ('0' > num_str_ptr->wstr_ptr[i] ||
                '9' < num_str_ptr->wstr_ptr[i])
                )//输入必须是0-9或者'+'
                ||
                ('+' == num_str_ptr->wstr_ptr[i]
                && (0 < i))//'+'只能在最前面
                )
            {
                is_valid = FALSE;
                break;
            }
        }
    }
    else
    {
        is_valid = FALSE;
    }

    return is_valid;
}


/*****************************************************************************/
//     Description : to handle the input sc number win message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputSCWinMsg(
                                       MMI_WIN_ID_T        win_id,    //IN:
                                       MMI_MESSAGE_ID_E msg_id,    //IN:
                                       DPARAM            param    //IN:
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id= MMISMS_INPUTSCWIN_PHONENUM_CTRL_ID;
    MMI_STRING_T num_string = {0};
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT    
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif

    //SCI_TRACE_LOW:"MMISMS: HandleInputSCWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_6150_112_2_18_2_54_32_738,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/17
        {
            BOOLEAN is_disp_im_icon = FALSE;
            BOOLEAN is_disp_num_info = FALSE;

            GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
        }
#endif
        GUIEDIT_SetHandleRedKey(FALSE, ctrl_id); //cr192180
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //to get the input number
        SCI_MEMSET(&num_string, 0, sizeof (MMI_STRING_T));
        GUIEDIT_GetString(ctrl_id, &num_string);
        
        if (num_string.wstr_len > 0)
        {
            if (IsSCAddrValid(&num_string))
            {
                uint8 num_str[MMISMS_MAX_SC_NUMBER_LEN] = {0};

                MMI_WSTRNTOSTR((uint8 *)num_str,
                                            MMISMS_MAX_SC_NUMBER_LEN,
                                            num_string.wstr_ptr,
                                            MMISMS_MAX_SC_NUMBER_LEN,
                                            num_string.wstr_len
                                        );

                // set sc address
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
                if (MMISMS_AppSetSCAddr(&param_to_create_setting_main_win->set_mode, dual_sys, (uint8 *)num_str, (uint8)num_string.wstr_len))
#else
                if (MMISMS_AppSetSCAddr(dual_sys, (uint8 *)num_str, (uint8)num_string.wstr_len))
#endif
                {
#if defined(MMI_SMS_MULTI_PARAM_SUPPORT) && defined(MMI_GUI_STYLE_TYPICAL)
                    MMK_CloseWin(win_id);
#else
                    MMISMS_OpenSMSWaitingWin(MMISMS_WRITE_SMS_PARAM_WAITING_WIN_ID,
                                                                        MMIPUB_SOFTKEY_NONE,
                                                                        MMISMS_HandleWriteSMSParamWaitWinMsg,
                                                                        win_id);
#endif
                }
                else
                {
                    //提示输入错误
                    MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                }
            }
            else
            {
                //提示输入错误
                MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
            }
        }
        else
        {
            // prompt: empty number
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/******************* **********************************************************/
//     Description : to open set sc win
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetSCWin(MN_DUAL_SYS_E dual_sys)
{
    uint8 sc_num[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    uint8 sc_num_len = 0;
    wchar sc_num_wstr[MMISMS_STRNUM_MAX_LEN + 2] = {0};

    MMK_CreatePubEditWin((uint32 *)MMISMS_INPUTSC_WIN_TAB, (ADD_DATA)dual_sys);
#ifdef DPHONE_SUPPORT 
#else
    GUIEDIT_SetFont(MMISMS_INPUTSCWIN_PHONENUM_CTRL_ID, SONG_FONT_29);
#endif
    if (MMISMS_AppGetSCAddr(
                            dual_sys,
                            sc_num, 
                            &sc_num_len, 
                            MMISMS_STRNUM_MAX_LEN + 2 
                            )
        )
    {
        if (sc_num_len > 0)
        {
            MMI_STRNTOWSTR( sc_num_wstr,
                            MMISMS_STRNUM_MAX_LEN,
                            (const uint8 *)sc_num,
                            MMISMS_STRNUM_MAX_LEN,
                            sc_num_len);

        
            GUIEDIT_SetString(MMISMS_INPUTSCWIN_PHONENUM_CTRL_ID,
                sc_num_wstr,
                (uint16)MMIAPICOM_Wstrlen(sc_num_wstr)
                );
        }
    }
}

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
/*****************************************************************************/
//     Description : to open set auto signature win
//    Global resource dependence : 
//  Author:bown.zhang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetAutoSignatureWin(void)
{
    wchar signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
    uint8 signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;

    MMK_CreatePubEditWin((uint32 *)MMISMS_INPUT_SIGNATURE_WIN_TAB, PNULL);

    //GUIEDIT_SetFont(MMISMS_INPUT_SIGNATURE_WIN_TEXT_CTRL_ID, SONG_FONT_26);

    signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr);

    if (0 == signature_str_len)
    {
        SCI_MEMSET(signature_wstr, 0, MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2);
    }

    GUIEDIT_SetString(MMISMS_INPUT_SIGNATURE_WIN_TEXT_CTRL_ID, signature_wstr, (uint16)MMIAPICOM_Wstrlen(signature_wstr));
}
#endif
#endif

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : SetSmsSettingMenuListItemData
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetSmsSettingMenuListItemData(
                            GUILIST_ITEM_T               *item_t,
                            GUILIST_ITEM_DATA_T      *item_data,
                            MMI_TEXT_ID_T       txt_id_1,
                            MMI_TEXT_ID_T       txt_id_2
                            )
{
    item_t->item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t->item_data_ptr = item_data;

    item_data->item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data->item_content[0].item_data.text_id = txt_id_1;
    item_data->item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data->item_content[1].item_data.text_id = txt_id_2;
    //set softkey
    item_data->softkey_id[0] = TXT_COMMON_OK;
    item_data->softkey_id[1] = TXT_NULL;
    item_data->softkey_id[2] = STXT_RETURN;
}

/*****************************************************************************/
//  Description : to handle set Service Loading Operation
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetServiceLoadingOperationWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 list_index = 0;
    MMISMS_SL_OPERATION_E oper = MMISMS_SL_OPERATION_ALWAYS;
    uint32 pre_ctrl_id = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitSettingServiceLoadingOperation(MMISMS_SERVICE_LOADING_OPERATION_SETTING_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISMS_SERVICE_LOADING_OPERATION_SETTING_CTRL_ID);
        break;    
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        list_index = GUILIST_GetCurItemIndex(MMISMS_SERVICE_LOADING_OPERATION_SETTING_CTRL_ID);

        if (!GUILIST_IsCurrItemInvalid(MMISMS_SERVICE_LOADING_OPERATION_SETTING_CTRL_ID))
        {
            GUILIST_ITEM_T item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};

            oper = (MMISMS_SL_OPERATION_E)list_index;

            MMISMS_SetPushSmsSLOperation(oper);

            //service loading operation
            if (MMISMS_SL_OPERATION_ALWAYS == oper)
            {
                SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_SMS_SERVICE_LOADING_OPERATION,TXT_COMMON_ALWAYS);
            }
            else if (MMISMS_SL_OPERATION_PROMPT == oper)
            {
                SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_SMS_SERVICE_LOADING_OPERATION,TXT_COMMON_PROMPT);
            }
            else
            {
                SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_SMS_SERVICE_LOADING_OPERATION,TXT_COMMON_NEVER);
            }

            GUILIST_ReplaceItem((MMI_CTRL_ID_T)pre_ctrl_id, &item_t, 1);

            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : add all tabs and all child windows to parent window.
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AddSettingsTabChildWindows(void)
{
    uint32 i = 0;
    MMI_STRING_T str_text = {0};
    MMI_IMAGE_ID_T sms_setting_pda_tab[][2] = 
        {{IMAGE_SETTING_SMS_TAB_ON_ICON, IMAGE_SETTING_SMS_TAB_OFF_ICON},
        {IMAGE_SETTING_MMS_TAB_ON_ICON, IMAGE_SETTING_MMS_TAB_OFF_ICON},
#ifdef BROWSER_SUPPORT
        {IMAGE_SETTING_PUSH_TAB_ON_ICON, IMAGE_SETTING_PUSH_TAB_OFF_ICON},
#endif
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
        {IMAGE_CONTROL_TAB_OTHER_HIGHLIGHT, IMAGE_CONTROL_TAB_OTHER}
#endif
        };

#ifdef MMI_ISTYLE_SUPPORT
    MMI_IMAGE_ID_T istyle_sms_setting_pda_tab[][2] = 
        {{ISTYLE_IMAGE_SETTING_SMS_TAB_ON_ICON, ISTYLE_IMAGE_SETTING_SMS_TAB_OFF_ICON},
        {ISTYLE_IMAGE_SETTING_MMS_TAB_ON_ICON, ISTYLE_IMAGE_SETTING_MMS_TAB_OFF_ICON},
#ifdef BROWSER_SUPPORT
        {ISTYLE_IMAGE_SETTING_PUSH_TAB_ON_ICON, ISTYLE_IMAGE_SETTING_PUSH_TAB_OFF_ICON},
#endif
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
        {ISTYLE_IMAGE_OTHERS_TAB_ON_ICON, ISTYLE_IMAGE_OTHERS_TAB_OFF_ICON}
#endif
        };
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#ifdef MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_sms_setting_pda_tab;
    }
    else
#endif        
    {
        pda_tab_ptr = sms_setting_pda_tab;
    }

    GUITAB_SetMaxItemNum(MMISMS_SETTINGS_TAB_CTRL_ID, 4);
    
    MMI_GetLabelTextByLang(TXT_SMS , &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[0][0],
        pda_tab_ptr[0][1]);        
    GUITAB_AddChildWindow(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        MMISMS_SMS_SETTING_MENU_WIN_ID, 
        i++);

#ifdef MMS_SUPPORT
    MMI_GetLabelTextByLang(TXT_MMS, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[1][0],
        pda_tab_ptr[1][1]);        
    GUITAB_AddChildWindow(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        MMIMMS_MMSSETTING_WIN_ID,
        i++);
#endif

#ifdef BROWSER_SUPPORT
    MMI_GetLabelTextByLang(TXT_COMM_PUSH_MSG, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[2][0],
        pda_tab_ptr[2][1]);        
    GUITAB_AddChildWindow(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        MMISMS_BROWSER_SETTING_MENUWIN_ID,
        i++);
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    MMI_GetLabelTextByLang(TXT_TOOLS_OTHERS, &str_text);
    GUITAB_AppendSimpleItem(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[3][0],
        pda_tab_ptr[3][1]);        
    GUITAB_AddChildWindow(
        MMISMS_SETTINGS_TAB_CTRL_ID,
        MMISMS_OTHERS_SETTING_MENUWIN_ID,
        i++);
#endif                
    GUITAB_SetCurSel(MMISMS_SETTINGS_TAB_CTRL_ID, 0);
}

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
/*****************************************************************************/
//  Description :font size setting for sms_mms_in_1
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsMmsFontSizeSettingWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint16 index = 0;
    uint16 cur_index = 0;
    GUI_FONT_T font = MMI_DEFAULT_NORMAL_FONT;
    MMI_TEXT_ID_T text_array[] = {TXT_COMM_FONT_BIG,TXT_COMMON_NORMAL,TXT_FONT_SMALL};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        font = MMISMS_GetWinFont();
        switch(font)
        {
        case MMI_DEFAULT_BIG_FONT:
            cur_index = MMISMS_BIG_FONT_SIZE;
            break;
            
        case MMI_DEFAULT_NORMAL_FONT:
            cur_index = MMISMS_NORMAL_FONT_SIZE;
            break;
            
        case MMI_DEFAULT_SMALL_FONT:
            cur_index = MMISMS_SMALL_FONT_SIZE;
            break;
            
        default:
            cur_index = MMISMS_NORMAL_FONT_SIZE;
            break;
        }
        
        GUILIST_SetMaxItem(MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID, 3, FALSE);
        
        for (index = 0; index < MMISMS_FONT_SIZE_MAX; index++)
        {
            MMI_STRING_T text_str = {0};
            AppendSmsPdaSettingsItem2Line(MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID,
            text_array[index], text_str, 0, 0, FALSE, GUIITEM_STYLE_ONE_LINE_RADIO);
        }
        
        GUILIST_SetSelectedItem(MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID, cur_index, TRUE);
        GUILIST_SetCurItemIndex(MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID,cur_index);	
        MMK_SetAtvCtrl(win_id, MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID);                
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
            cur_index = GUILIST_GetCurItemIndex(MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID);
            if(cur_index < MMISMS_FONT_SIZE_MAX)
            {
                switch(cur_index)
                {
                case 0:
                    font = MMI_DEFAULT_BIG_FONT;
                    break;
                case 1:
                    font = MMI_DEFAULT_NORMAL_FONT;
                    break;
                case 2:
                    font = MMI_DEFAULT_SMALL_FONT;
                    break;
                default:
                    font = MMI_DEFAULT_NORMAL_FONT;
                    break;
                } 
                MMIAPISMS_SetWinFont(font);
            }     
            MMK_CloseWin(win_id);
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default :
            err_code = MMI_RESULT_FALSE;
            break;
    } 
    return err_code;
}
#endif


/*****************************************************************************/
//  Description :HandleSettingsMainWindow
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingsMainWindow(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM            param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISMS_SETTINGS_TAB_CTRL_ID);
        AddSettingsTabChildWindows();
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
        SettingwinMainCloseCtrlFunc(win_id);
#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jixin.xu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SettingwinMainCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMISMS_SETTING_MAIN_WIN_PARAM_T *win_data_p = (MMISMS_SETTING_MAIN_WIN_PARAM_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}
#endif
/*****************************************************************************/
// 	Description : create sms setting main window
//	Global resource dependence : 
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterSettingMainWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMI_HANDLE_T win_handle = 0;
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = PNULL;
#endif
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    if (MMK_IsOpenWin(MMISMS_SETTINGS_TAB_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SETTINGS_TAB_WIN_ID);
    }
    win_handle = MMK_CreateWin((uint32 *)MMISMS_SETTINGS_TAB_WIN_TAB, PNULL);
#else
    if (MMK_IsOpenWin(MMISMS_SMS_SETTING_MENU_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SMS_SETTING_MENU_WIN_ID);
    }
    win_handle = MMK_CreateWin((uint32 *)MMISMS_SMS_SETTING_MENUWIN_TAB, PNULL);
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    param_to_create_setting_main_win = (MMISMS_SETTING_MAIN_WIN_PARAM_T *) SCI_ALLOC_APPZ(sizeof (MMISMS_SETTING_MAIN_WIN_PARAM_T));
    param_to_create_setting_main_win->set_mode = MMISMS_Get_Setmode();

    MMK_SetWinUserData(win_handle, (void*)param_to_create_setting_main_win);
#endif

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
        SCI_FREE (param_to_create_setting_main_win);
#endif
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : open tab win func
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSettingTabWin(void)
{
    //MMK_CreateParentWin((uint32 *)MMISMS_SETTINGS_TAB_WIN_TAB, PNULL);
    MMISMS_EnterSettingMainWin();

    MMK_CreateChildWin(MMISMS_SETTINGS_TAB_WIN_ID, (uint32 *)MMISMS_SMS_SETTING_MENUWIN_TAB, NULL);

#ifdef MMS_SUPPORT
    MMIAPIMMS_CreateMMSSettingsChildWin(MMISMS_SETTINGS_TAB_WIN_ID);
#endif
    
#ifdef BROWSER_SUPPORT
    MMK_CreateChildWin(MMISMS_SETTINGS_TAB_WIN_ID, (uint32 *)MMISMS_BROWSER_SETTING_MENUWIN_TAB, NULL);
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT) && !defined(MMI_GUI_STYLE_TYPICAL)
    MMK_CreateChildWin(MMISMS_SETTINGS_TAB_WIN_ID, (uint32 *)MMISMS_OTHERS_SETTING_MENUWIN_TAB, NULL);
#endif
    MMK_SetChildWinFocus(MMISMS_SETTINGS_TAB_WIN_ID, MMISMS_SMS_SETTING_MENU_WIN_ID);    
}

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : to handle set browser information
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBrowserInfoWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    uint16			list_index = 0;
    BOOLEAN          flag = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUILIST_SetOwnSofterKey(MMISMS_BROWSER_SETTING_CTRL_ID, TRUE);
#endif
        InitSettingBrowser(MMISMS_BROWSER_SETTING_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISMS_BROWSER_SETTING_CTRL_ID);
        break;    
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitSettingBrowser(MMISMS_BROWSER_SETTING_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        list_index = GUILIST_GetCurItemIndex(MMISMS_BROWSER_SETTING_CTRL_ID);

        switch ((MMISMS_PUSH_MSG_SETTING_E)list_index)
        {
        case MMISMS_PUSH_MSG_SETTING_RECEIVING_OPT:
            flag = !MMISMS_GetPushSmsBrowserInfo();
            MMISMS_SetPushSmsBrowserInfo(flag);

            MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;

        case MMISMS_PUSH_MSG_SL_OPERATION:
            MMISMS_CreateServiceLoadingOperationSettingWin(MMISMS_BROWSER_SETTING_CTRL_ID);
            break;

        default:
            break;
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
/*****************************************************************************/
//  Description : to handle set common information for message
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetOthersInfoWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_COMMON_SETTING_MENU_CTRL_ID;
    uint16 index = 0;
    switch(msg_id)
    {    
    case MSG_OPEN_WINDOW:
        {  
            InitCommonSettingsListCtrl(ctrl_id);

            MMK_SetAtvCtrl(win_id, ctrl_id);  
        }
        break;
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitCommonSettingsListCtrl(ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            index = GUILIST_GetCurItemIndex(ctrl_id);
            switch(index)
            {
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
            case MMISMS_MMS_FONT_SIZE:
                MMK_CreatePubListWin((uint32*)MMISMS_MMS_FONTSIZE_SETTING_WIN_TAB, PNULL);
                break;
#endif
            default:
                break;
            }
            break;
        }  
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
/*****************************************************************************/
//  Description : init browser info
//  Global resource dependence : 
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void InitCommonSettingsListCtrl(
                         MMI_CTRL_ID_T  ctrl_id
                         )
{
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    MMISMS_FONT_E font_size = MMISMS_GetFontSize();/*lint !e64 */
    MMI_TEXT_ID_T text_array[] = {TXT_COMM_FONT_BIG,TXT_COMMON_NORMAL,TXT_FONT_SMALL};
    MMI_STRING_T  text_str = {0};
	uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
	uint16 top_index = GUILIST_GetTopItemIndex(ctrl_id);

    GUILIST_SetMaxItem(ctrl_id,MMISMS_COMMON_SETTING_MAX,FALSE);
    MMI_GetLabelTextByLang(text_array[font_size] , &text_str);   
    AppendSmsPdaSettingsItem2Line(
                                ctrl_id,
                                TXT_COMMON_FONT_SIZE,
                                text_str,
                                IMAGE_COMMON_RIGHT_ARROW,
                                0,
                                FALSE,
                                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
                                );

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);
#endif
}
#endif

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : init browser info
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void InitSettingBrowser(
                         MMI_CTRL_ID_T  ctrl_id
                         )
{
    BOOLEAN     is_open  =   FALSE;
    MMI_IMAGE_ID_T image_id = 0;
    MMI_STRING_T  text_str = {0};
    MMISMS_SL_OPERATION_E oper = MMISMS_GetPushSmsSLOperation();
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    uint16 top_index = GUILIST_GetTopItemIndex(ctrl_id);
    MMI_TEXT_ID_T text_array[] = {TXT_COMMON_ALWAYS,TXT_COMMON_PROMPT,TXT_COMMON_NEVER};
    
    //set max item
    GUILIST_SetMaxItem(ctrl_id,MMISMS_PUSH_MSG_SETTING_MAX,FALSE);

    is_open = MMISMS_GetPushSmsBrowserInfo();   // status report
    if(is_open)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
#ifdef MMI_GUI_STYLE_MINISCREEN
        //GUIWIN_SetSoftkeyTextId(MMISMS_BROWSER_SETTING_MENUWIN_ID,  STXT_CANCEL, TXT_NULL, STXT_RETURN, TRUE);
#endif
    }
    else
    {
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
#ifdef MMI_GUI_STYLE_MINISCREEN
       //GUIWIN_SetSoftkeyTextId(MMISMS_BROWSER_SETTING_MENUWIN_ID,  STXT_SELECT, TXT_NULL, STXT_RETURN, TRUE);
#endif
    }        

    AppendSmsPdaSettingsItem2Line(            
                                    ctrl_id,
                                    TXT_CMSBRW_ACCEPTPUSHMSGSETTING,
                                    text_str,
                                    image_id,
                                    MMISMS_PUSH_MSG_SETTING_RECEIVING_OPT,
                                    FALSE,
                                    GUIITEM_STYLE_ONE_LINE_TEXT_ICON
                                    );

    MMI_GetLabelTextByLang(text_array[oper] , &text_str);   
    AppendSmsPdaSettingsItem2Line(
                                ctrl_id,
                                TXT_SMS_SERVICE_LOADING_OPERATION,
                                text_str,
                                IMAGE_COMMON_RIGHT_ARROW,
                                MMISMS_PUSH_MSG_SL_OPERATION,
                                FALSE,
                                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
                                );

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    GUILIST_SetTopItemIndex(ctrl_id, top_index);

    //SCI_TRACE_LOW:"InitSettingBrowser:is_open is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_1015_112_2_18_2_53_32_466,(uint8*)"d",is_open);
}

#endif

#if defined(MMI_SMS_EXPORT_SUPPORT) || defined(TTS_SUPPORT)
/*****************************************************************************/
//  Description : append pda settings item 1 line
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendSmsPdaSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}
#endif

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendSmsPdaSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data,   //用户数据
                                 BOOLEAN is_need_replace,
                                 GUIITEM_STYLE_E item_style
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        MMIAPICOM_Wstrncpy(temp_wstr,
                                          text_str.wstr_ptr,
                                          text_str.wstr_len);
        temp_len = text_str.wstr_len;
    }
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0 && GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT != item_style)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }
 
    if (GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM == item_style || GUIITEM_STYLE_ONE_LINE_TEXT_ICON == item_style)
    {
        //anim
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = image_id;
    }
    else if (GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT == item_style && text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = temp_len;
    }

    if(is_need_replace)
    {
        uint16 index = 0;
        index = GUILIST_GetCurItemIndex(ctrl_id); 
        GUILIST_ReplaceItem(ctrl_id,&item_t,index);      
    }
    else
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
    
}
#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsTTSListCtrl(void)
{
    MMI_CTRL_ID_T ctrl_id = MMISMS_SMS_SETTING_TTSLIST_CTRL_ID;
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT        
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    BOOLEAN flag = FALSE;
#endif

    MMI_STRING_T text_str= {0};
    uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);

    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SMS_SETTINGS_MAX, FALSE );   
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT        
    flag = MMISMS_GetTTSInfo();   // status report
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        

    AppendSmsPdaSettingsItem2Line(            
    ctrl_id,
    TXT_SET_TTS_AUTO_READ,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_SMS_TTS_STATUS,
    FALSE,
    GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);
#endif

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsSettingsListCtrl(BOOLEAN is_need_replace)
{
    MMI_CTRL_ID_T       ctrl_id = MMISMS_SMS_SETTING_MENU_CTRL_ID;
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
    uint8 temp_str[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_wstr[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    uint8 temp_str_len = 0;
    MMINV_SMS_VALID_PERIOD_T valid_period = 0;
    uint32 cur_selection  = 0;
#endif
    MMI_STRING_T text_str= {0};
    BOOLEAN    flag  = FALSE;
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT 
    MMINV_SMS_AUTOSAVE_T status = 0;
#endif
    MMISMS_SMS_SAVEPOS_TYPE_E pos = MMISMS_SAVEPOS_NONE;
    uint16 index = 0;
    uint16 focus_index = 0;
    uint16 i = 0;
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT

    MMI_TEXT_ID_T sms_setting_sim[MMI_DUAL_SYS_MAX]=
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            TXT_SMS_SIM1_SET,
            TXT_SMS_SIM2_SET,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_SMS_SIM3_SET,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_SMS_SIM4_SET,
#endif
#endif
#else
            TXT_SMS_SIM_SET
#endif    
    };

    uint32 sms_setting_sim_userdata[MMI_DUAL_SYS_MAX] = 
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMISMS_PDA_SMS_SETTINGS_SMS1,
            MMISMS_PDA_SMS_SETTINGS_SMS2,            
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_PDA_SMS_SETTINGS_SMS3,
#endif            
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_PDA_SMS_SETTINGS_SMS4,
#endif         
#else
            MMISMS_PDA_SMS_SETTINGS,
#endif
    };

#else//MMI_SMS_MULTI_PARAM_SUPPORT
    MMI_TEXT_ID_T sms_setting_sim_sc[MMI_DUAL_SYS_MAX]=
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            TXT_SMS_SETTING_SIM1_SC,
            TXT_SMS_SETTING_SIM2_SC,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_SMS_SETTING_SIM3_SC,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_SMS_SETTING_SIM4_SC,
#endif
#endif
#else
            TXT_SMS_SETTING_SC
#endif    
    };

    uint32 sms_setting_sim_userdata[MMI_DUAL_SYS_MAX] = 
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMISMS_PDA_SMS_SETTINGS_SMS1_SC,
            MMISMS_PDA_SMS_SETTINGS_SMS2_SC,            
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_PDA_SMS_SETTINGS_SMS3_SC,
#endif            
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_PDA_SMS_SETTINGS_SMS4_SC,
#endif         
#else
            MMISMS_PDA_SMS_SETTINGS_SC,
#endif
    };
#endif

    //set max item
    if(is_need_replace)
    { 
        focus_index = GUILIST_GetCurItemIndex(ctrl_id); 
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SMS_SETTINGS_MAX, FALSE );   
    }

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
       if ( (is_need_replace && index == focus_index) ||!is_need_replace )
       {
            MMI_STRING_T temp_text_str = {0};
            wchar temp_wstr[MMISMS_SET_SIMMODE_NAME_LEN + 2] = {0};

            SCI_MEMSET(&temp_text_str, 0x00, sizeof(temp_text_str));
            temp_text_str.wstr_ptr = temp_wstr;

            if (MMIPHONE_IsSimCardOk((MN_DUAL_SYS_E)i))
            {
                ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
                uint8 smsp_id = 0;
                MN_SMS_PARAM_T sms_param = {0};

                smsp_id = MMISMS_GetSIMModeFlag((MN_DUAL_SYS_E)i);
                err_code = MNSMS_GetSmsParamEx((MN_DUAL_SYS_E)i, smsp_id + 1, &sms_param);

                if (ERR_MNSMS_NONE == err_code
                    && sms_param.alpha_id_present)
                {
                    ConvertSimToModeName(
                                     &temp_text_str,
                                     sms_param.alpha_id_t.id_len,
                                     sms_param.alpha_id_t.id
                                    );
                }

                //Not exist, default name
                if (0 == temp_text_str.wstr_len)
                {
                    MMI_GetLabelTextByLang(TXT_SMS_SIM_MODE1 + smsp_id, &temp_text_str);
                }
            }

            //Append Item
            AppendSmsPdaSettingsItem2Line(
            ctrl_id,
            sms_setting_sim[i],
            temp_text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)sms_setting_sim_userdata[i],
            is_need_replace,
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
            );       
        }

        index++;
    }
#else
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
       if( (is_need_replace && index == focus_index) ||!is_need_replace )
       {
            if(MMISMS_AppGetSCAddr(
                                (MN_DUAL_SYS_E)i,
                                temp_str, 
                                &temp_str_len, 
                                MMISMS_STRNUM_MAX_LEN + 2 
                                ))
            {
                if (temp_str_len > 0)
                {
                    MMI_STRNTOWSTR( temp_wstr,
                                    MMISMS_STRNUM_MAX_LEN,
                                    (const uint8 *)temp_str,
                                    MMISMS_STRNUM_MAX_LEN,
                                    temp_str_len);     
                }
            }
            
            text_str.wstr_ptr = temp_wstr;
            text_str.wstr_len = temp_str_len;
            AppendSmsPdaSettingsItem2Line(
            ctrl_id,
            sms_setting_sim_sc[i],
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)sms_setting_sim_userdata[i],
            is_need_replace,
            GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
            );       
        }
        index++;
        SCI_MEMSET(temp_str, 0x00, sizeof(temp_str));
    }
    
    if((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        valid_period = MMISMS_GetValidPeriod();   // valid peroid    
        switch (valid_period)
        {
        case MMISMS_VALID_PERIOD_3600:
            cur_selection = 0;
            break;
            
        case MMISMS_VALID_PERIOD_21600:
            cur_selection = 1;
            break;
            
        case MMISMS_VALID_PERIOD_86400:
            cur_selection = 2;
            break;
            
        case MMISMS_VALID_PERIOD_259200:
            cur_selection = 3;
            break;
            
        case MMISMS_VALID_PERIOD_604800:
            cur_selection = 4;
            break;
            
        case MMISMS_VALID_PERIOD_MAX:
            cur_selection = 5;
            break;
            
        default:
            valid_period = MMISMS_VALID_PERIOD_MAX;
            cur_selection = 5;
            break;
        }
        
        MMI_GetLabelTextByLang(TXT_SMS_VP_1HOUR + cur_selection, &text_str); 
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_SETTING_VP,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_SMS_SETTINGS_VALID_PERIOD,
            is_need_replace,
            GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);
    }    
    index++;
#endif
    
    if((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        flag = MMISMS_GetStatusReportFlag();   // status report
        if(flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }        
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_STATUSREPORT,
            text_str,
            image_id,
            (uint32)MMISMS_PDA_SMS_SETTINGS_STATUS_REPORT,
            is_need_replace,
			GUIITEM_STYLE_ONE_LINE_TEXT_ICON);
    }
    index++;

#ifdef MMI_MSG_RESEND
    if((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        flag = MMISMS_GetResendFlag();   // status report
        if(flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }        
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_AUTO_RESEND,
            text_str,
            image_id,
            (uint32)MMISMS_PDA_SMS_SETTINGS_RESEND,
            is_need_replace,
			GUIITEM_STYLE_ONE_LINE_TEXT_ICON);
    }
    index++;
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    if ((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        flag = MMISMS_GetReplyPathFlag();   // Reply Path

        if (flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }

        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_REPLY_PATH,
            text_str,
            image_id,
            (uint32)MMISMS_PDA_SMS_SETTINGS_REPLY_PATH,
            is_need_replace,
			GUIITEM_STYLE_ONE_LINE_TEXT_ICON);
    }
    index++;
#endif

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    if ((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        flag = MMISMS_GetConcatenatedSMS();   // Concatenated setting

        if (flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }

        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_CONCATENATED_SMS,
            text_str,
            image_id,
            (uint32)MMISMS_PDA_SMS_SETTINGS_CONCATENATED_SETTING,
            is_need_replace,
			GUIITEM_STYLE_ONE_LINE_TEXT_ICON);
    }
    index++;
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    if ((is_need_replace && index == focus_index) || !is_need_replace)
    {
        MMISMS_ALPHABET_MANAGEMENT_E alphabet_management = MMISMS_GetAlphabetManagement();

        switch(alphabet_management)
        {
        case MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET:
            MMI_GetLabelTextByLang(TXT_SMS_ALPHABET_GSM_ALPHABET, &text_str);
            break;
            
        case MMISMS_ALPHABET_MANAGEMENT_8BIT:
            MMI_GetLabelTextByLang(TXT_SMS_ALPHABET_8BIT, &text_str);
            break;
            
        case MMISMS_ALPHABET_MANAGEMENT_UCS2:
            MMI_GetLabelTextByLang(TXT_SMS_ALPHABET_UCS2, &text_str);
            break;

        case MMISMS_ALPHABET_MANAGEMENT_AUTO:
            MMI_GetLabelTextByLang(TXT_AUTO, &text_str);
            break;

        default:
            MMI_GetLabelTextByLang(TXT_AUTO, &text_str);
            break;
        }        
        AppendSmsPdaSettingsItem2Line(
            ctrl_id,
            TXT_SMS_ALPHABET_MANAGEMENT,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_SMS_SETTINGS_ALPHABET_SETTING,
            is_need_replace,
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);               
    }
    index++;
#endif
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT  
    if((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        status = MMISMS_GetAutoSave();      //send save
        switch(status)
        {
        case MMINV_SMS_SETTING_PROMPT:
            MMI_GetLabelTextByLang(TXT_SMS_PROMPT, &text_str);
            break;
            
        case MMINV_SMS_SETTING_SAVE:
            MMI_GetLabelTextByLang(STXT_SAVE, &text_str);
            break;
            
        case MMINV_SMS_SETTING_NOTSAVE:
            MMI_GetLabelTextByLang(TXT_SMS_NOT_SAVE, &text_str);
            break;
            
        default:
            MMI_GetLabelTextByLang(TXT_SMS_PROMPT, &text_str);
            break;
        }        
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_AUTO_SAVE,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_SMS_SETTINGS_SAVE_REQUEST,
            is_need_replace,
            GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);               
    }    
    index++;
#endif    
    if((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        pos = MMISMS_GetSavePos();   // save pos
        switch(pos)
        {
        case MMISMS_SAVEPOS_SIM:
            MMI_GetLabelTextByLang(TXT_COMM_SIM, &text_str);
            break;
            
        case MMISMS_SAVEPOS_PHONE:
            MMI_GetLabelTextByLang(TXT_PHONE, &text_str);
            break;
            
        default:
            break;
        }        
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_PRIOR_STORE_MEDIUM,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_SMS_SETTINGS_SAVE_POS,
            is_need_replace,
            GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);
    }
    index++;

#if defined(MMI_SMS_EXPORT_SUPPORT)
    if(!is_need_replace)
    {
        AppendSmsPdaSettingsItem1Line(
            ctrl_id,
            TXT_SMS_EXPORT,
            (uint32)MMISMS_PDA_SMS_SETTINGS_EXPORT);
    }
    index++;
#endif

#ifdef TTS_SUPPORT
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT 
    if(!is_need_replace)
    {
        AppendSmsPdaSettingsItem1Line(            
            ctrl_id,
            TXT_SET_TTS,
            (uint32)MMISMS_PDA_SMS_SETTINGS_TTS);

    }
    index++;
#endif	
#endif

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    /*message auto signature*/
    if((is_need_replace && index == focus_index) || !is_need_replace)
    { 
        flag = MMISMS_GetAutoSignatureFlag();   // status report
        if(flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_CMS_MMS_SIGN,
            text_str,
            image_id,
            (uint32)MMISMS_PDA_SMS_SETTINGS_AUTO_SIGNATURE_FLAG,
            is_need_replace,
            GUIITEM_STYLE_ONE_LINE_TEXT_ICON);
    }
    index++;

    if((is_need_replace && index == focus_index) || !is_need_replace)
    {
        MMI_STRING_T temp_text_str = {0};
        wchar signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
        uint8 signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
        
        signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr);
        
        temp_text_str.wstr_ptr = signature_wstr;
        temp_text_str.wstr_len = signature_str_len;
        
        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_COMMON_SIGNATURE,
            temp_text_str,
            image_id,
            (uint32)MMISMS_PDA_SMS_SETTINGS_AUTO_SIGNATURE,
            is_need_replace,
            GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT); 
    }
        //gray the signature edit item
    if (!MMISMS_GetAutoSignatureFlag())
    {
        GUILIST_SetItemGray(ctrl_id, index, TRUE);
    }
    else
    {
        GUILIST_SetItemGray(ctrl_id, index, FALSE);
    }
    index++;

    
#endif
#ifdef MMI_SMS_FIX_SEND_SIM

    if(!is_need_replace)
    {
        AppendSmsPdaSettingsItem1Line(
            ctrl_id,
            TXT_SMS_FIX_SIM_SEND,
            (uint32)MMISMS_PDA_SMS_SETTINGS_FIX_SIMCARD_SEND);
        
    }
#endif
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTTSListWindow(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM            param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SMS_SETTING_TTSLIST_CTRL_ID;
    uint16 index = 0;
    uint32 user_data = 0;
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT         
    BOOLEAN is_tts_open = FALSE;
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetWinHookFunc(win_id, TTSVolumeWinHookFunc);
        InitPdaSmsTTSListCtrl();
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitPdaSmsTTSListCtrl();
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT 
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &user_data);
        switch((MMISMS_PDA_TTS_SETTING_TYPE_E)user_data)
        {

#ifndef MMISMS_CHATMODE_ONLY_SUPPORT         
        case MMISMS_PDA_SMS_TTS_STATUS:
            MMK_CloseWin(MMICOMMON_PANEL_WIN_ID);

            is_tts_open = MMISMS_GetTTSInfo();   // status report
            is_tts_open = !is_tts_open;
            MMINV_WRITE(MMINV_SMS_TTS_FLAG,&is_tts_open);
            MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
#endif

        case MMISMS_PDA_SMS_TTS_VOLUME:
            OpenTTSVolumePanel(win_id, 0);
            break;

        default:
            break;
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
//     Description : to handle the SMS setting menu window message
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM            param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SMS_SETTING_MENU_CTRL_ID;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint32 i = 0;
    uint16 index = 0;
    uint32 user_data = 0;
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT    
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaSmsSettingsListCtrl(FALSE);
        for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
        {          
			GUILIST_SetItemGray(ctrl_id, (i - MN_DUAL_SYS_1), TRUE);          
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);        
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitPdaSmsSettingsListCtrl(TRUE);
        for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
        {          
			GUILIST_SetItemGray(ctrl_id, (i - MN_DUAL_SYS_1), TRUE);           
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
	#ifndef MMI_PDA_SUPPORT 
    case MSG_CTL_OK:
    case MSG_APP_OK:
	#endif
    case MSG_APP_WEB:     
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &user_data);
        switch((MMISMS_PDA_SETTINGS_TYPE_E)user_data)
        {
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
        #ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMISMS_PDA_SMS_SETTINGS_SMS1:
        case MMISMS_PDA_SMS_SETTINGS_SMS2:
        #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_PDA_SMS_SETTINGS_SMS3:
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_PDA_SMS_SETTINGS_SMS4:
        #endif           
            dual_sys = MN_DUAL_SYS_1 + (user_data - MMISMS_PDA_SMS_SETTINGS_SMS1);
            //g_mmisms_global.set_mode.dual_sys = dual_sys;
            param_to_create_setting_main_win->set_mode.dual_sys = dual_sys;
            MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
            MMK_CreateWin((uint32*)MMISMS_SELECTSIMMODE_WIN_TAB, (ADD_DATA)dual_sys);
            break;
        #else
        case MMISMS_PDA_SMS_SETTINGS:
            dual_sys = MN_DUAL_SYS_1;
            //g_mmisms_global.set_mode.dual_sys = dual_sys;
            param_to_create_setting_main_win->set_mode.dual_sys = dual_sys;
            MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
            MMK_CreateWin((uint32*)MMISMS_SELECTSIMMODE_WIN_TAB, (ADD_DATA)dual_sys);
            break;
        #endif
#else
        #ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMISMS_PDA_SMS_SETTINGS_SMS1_SC:
        case MMISMS_PDA_SMS_SETTINGS_SMS2_SC:
        #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_PDA_SMS_SETTINGS_SMS3_SC:
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_PDA_SMS_SETTINGS_SMS4_SC:
        #endif           
            dual_sys = MN_DUAL_SYS_1+(user_data-MMISMS_PDA_SMS_SETTINGS_SMS1_SC);
            MMISMS_OpenSetSCWin(dual_sys);
            break;
        #else
        case MMISMS_PDA_SMS_SETTINGS_SC:
            dual_sys = MN_DUAL_SYS_1;
            MMISMS_OpenSetSCWin(dual_sys);
            break;
        #endif
            
        case MMISMS_PDA_SMS_SETTINGS_VALID_PERIOD:
            MMISMS_OpenSetVPWin();
            break;
#endif
            
        case MMISMS_PDA_SMS_SETTINGS_STATUS_REPORT:
            {
                BOOLEAN is_flag = !MMISMS_GetStatusReportFlag();

                MMISMS_SetStatusReportFlag(is_flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, 0);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            break;

#ifdef MMI_MSG_RESEND
        case MMISMS_PDA_SMS_SETTINGS_RESEND:
            {
                BOOLEAN is_flag = !MMISMS_GetResendFlag();

                MMISMS_SetResendFlag(is_flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, 0);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                MMISMS_ResendDelayed();
            }
            break;
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
        case MMISMS_PDA_SMS_SETTINGS_REPLY_PATH:
            {
                BOOLEAN is_flag = !MMISMS_GetReplyPathFlag();

                MMISMS_SetReplyPathFlag(is_flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, 0);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            break;
#endif

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
        case MMISMS_PDA_SMS_SETTINGS_CONCATENATED_SETTING:
            {
                BOOLEAN is_flag = !MMISMS_GetConcatenatedSMS();

                MMISMS_SetConcatenatedSMS(is_flag);

                MMISMS_FullOrder();

                MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);    

                MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);

                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, 0);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            break;
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
        case MMISMS_PDA_SMS_SETTINGS_ALPHABET_SETTING:
            MMK_CreatePubListWin((uint32 *)MMISMS_SETTING_ALPHABET_MANAGEMENT_WIN_TAB, PNULL);
            break;
#endif

#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
        case MMISMS_PDA_SMS_SETTINGS_SAVE_REQUEST:
            MMISMS_OpenSetAutoSaveWin();
            break;
#endif

        case MMISMS_PDA_SMS_SETTINGS_SAVE_POS:
            MMISMS_OpenSetSavePosWin();
            break;
            
#if defined(MMI_SMS_EXPORT_SUPPORT)
        case MMISMS_PDA_SMS_SETTINGS_EXPORT:
             if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
            }	        
            MMISMS_EnterExportMainWin();
            break;
#endif

        case MMISMS_PDA_SMS_SETTINGS_TTS:
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TTS_SUPPORT    
            MMK_CreateWin((uint32*)MMISMS_SET_TTS_LIST_WIN_TAB, PNULL);
#endif
#endif
            break;
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
        /*sms auto signature*/
        case MMISMS_PDA_SMS_SETTINGS_AUTO_SIGNATURE_FLAG:
            {
                BOOLEAN is_flag = !MMISMS_GetAutoSignatureFlag();
                
                MMISMS_SetAutoSignatureFlag(is_flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, 0);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
            break;
        case MMISMS_PDA_SMS_SETTINGS_AUTO_SIGNATURE:
            {
                if (MMISMS_GetAutoSignatureFlag())
                {
                    MMISMS_OpenSetAutoSignatureWin();
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_ENABLE_ON, TXT_CMS_MMS_SIGN,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                }
            }
            break;    
#endif            
#ifdef MMI_SMS_FIX_SEND_SIM
        case MMISMS_PDA_SMS_SETTINGS_FIX_SIMCARD_SEND:
        {
            MMK_CreateWin((uint32*)MMISMS_SET_FIXSIM_SENDING_LIST_WIN_TAB, PNULL);
            
        }
        break;
#endif
        default:
            break;
        }
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(MMK_IsChildWin(win_id))
        {
        MMK_CloseParentWin(MMK_GetParentWinHandle(win_id));
        }
        else
        {
            MMK_CloseWin(win_id);
        }        
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
        SettingwinMainCloseCtrlFunc(win_id);
#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#else
#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : init browser info
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitSettingBrowser(
                         MMI_CTRL_ID_T  ctrl_id
                         )
{
    uint16                  cur_index   =   0;
    BOOLEAN     is_open  =   FALSE;

    //set max item
    GUILIST_SetMaxItem(ctrl_id,2,FALSE);

    //append item
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_OPEN,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_CLOSE,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    
    is_open = MMISMS_GetPushSmsBrowserInfo();
    //SCI_TRACE_LOW:"InitSettingBrowser:is_open is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_1992_112_2_18_2_53_34_467,(uint8*)"d",is_open);
    
    if(is_open)
    {
        cur_index = 0;
    }

    else
    {
        cur_index = 1;
    }
    
    //set current and selected item
    GUILIST_SetSelectedItem(ctrl_id,cur_index,TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_index);

    return;
}

/*****************************************************************************/
//  Description : to handle set Push Message Settings
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePushMessagesSettingWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 list_index = 0;
    BOOLEAN receiving_flag = FALSE;
    MMISMS_SL_OPERATION_E oper = MMISMS_SL_OPERATION_ALWAYS;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SMS_PUSH_MESSAGES_CTRL_ID;
    uint16 cur_selection = 0;
    uint16 cur_status = 0;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	 GUILIST_SetMaxItem(ctrl_id, 2, FALSE );//max item 3

        receiving_flag = MMISMS_GetPushSmsBrowserInfo();

        //receiving option
        if (receiving_flag)
        {
            SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_COMMON_RECEIVING_OPTION,TXT_COMMON_RECEIVING_OPTION);
        }
        else
        {
            SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_COMMON_RECEIVING_OPTION,TXT_COMMON_RECEIVING_OPTION);
        }

        GUILIST_AppendItem(ctrl_id, &item_t);   

        oper = MMISMS_GetPushSmsSLOperation();

        //service loading operation
        if (MMISMS_SL_OPERATION_ALWAYS == oper)
        {
            SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_SMS_SERVICE_LOADING_OPERATION,TXT_COMMON_ALWAYS);
        }
        else if (MMISMS_SL_OPERATION_PROMPT == oper)
        {
            SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_SMS_SERVICE_LOADING_OPERATION,TXT_COMMON_PROMPT);
        }
        else
        {
            SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_SMS_SERVICE_LOADING_OPERATION,TXT_COMMON_NEVER);
        }

        GUILIST_AppendItem(ctrl_id, &item_t);   

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;    
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        if (GUILIST_IsCurrItemInvalid(ctrl_id))
        {
            break;
        }      

        switch(cur_selection)
        {
        case 0:
            receiving_flag = MMISMS_GetPushSmsBrowserInfo();

            if (receiving_flag)
            {
                SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_COMMON_RECEIVING_OPTION,TXT_COMMON_RECEIVING_OPTION);
            }
            else
            {
                SetSmsSettingMenuListItemData(&item_t,&item_data,TXT_COMMON_RECEIVING_OPTION,TXT_COMMON_RECEIVING_OPTION);
            }

            MMISMS_SetPushSmsBrowserInfo((BOOLEAN)!receiving_flag);

            GUILIST_ReplaceItem(ctrl_id, &item_t, MMISMS_PUSH_MSG_SETTING_RECEIVING_OPT);

            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
            break;

        case 1:
            MMISMS_CreateServiceLoadingOperationSettingWin(ctrl_id);
            break;

        default:
            break;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//     Description : to handle the SMS setting menu window message
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM            param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SMS_SETTING_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint32 i = 0;
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
        {
            if (!MMIAPIPHONE_IsSimCardOk(i))
            {
            #ifdef MMI_MULTI_SIM_SYS_SINGLE
                MMIAPICOM_EnableGrayed(win_id,MENU_SMS_SETTING,MMISMS_MENU_SETTING_SC_ITEM_ID,TRUE);
            #else
                MMIAPICOM_EnableGrayed(win_id,MENU_SMS_SETTING,MMISMS_MENU_SETTING_SIM1_SC_ITEM_ID+(i - MN_DUAL_SYS_1),TRUE);
            #endif
            }
        }
#ifdef TTS_SUPPORT
        MMK_SetWinHookFunc(win_id, TTSVolumeWinHookFunc);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
	#ifndef MMI_PDA_SUPPORT 
    case MSG_CTL_OK:
    case MSG_APP_OK:
	#endif
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMISMS_MENU_SETTING_SIM1_PARAM_ITEM_ID:
        case MMISMS_MENU_SETTING_SIM2_PARAM_ITEM_ID:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_SETTING_SIM3_PARAM_ITEM_ID:
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_SETTING_SIM4_PARAM_ITEM_ID:
#endif
            SCI_MEMSET(&g_mmisms_global.set_mode,0x0,sizeof(MMISMS_SET_SIMMODE_T));
            g_mmisms_global.set_mode.dual_sys = MN_DUAL_SYS_1+(menu_id-MMISMS_MENU_SETTING_SIM1_PARAM_ITEM_ID);
            //MMISMS_OpenSetSCWin(dual_sys);
            MMK_CreateWin((uint32*)MMISMS_SELECTSIMMODE_WIN_TAB, (ADD_DATA)g_mmisms_global.set_mode.dual_sys);
            break;        
#else
        case MMISMS_MENU_SETTING_SC_ITEM_ID:
            g_mmisms_global.set_mode.dual_sys = MN_DUAL_SYS_1;
            MMK_CreateWin((uint32*)MMISMS_SELECTSIMMODE_WIN_TAB, (ADD_DATA)g_mmisms_global.set_mode.dual_sys);
            break;
#endif
#else //MMI_SMS_MULTI_PARAM_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMISMS_MENU_SETTING_SIM1_SC_ITEM_ID:
        case MMISMS_MENU_SETTING_SIM2_SC_ITEM_ID:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_SETTING_SIM3_SC_ITEM_ID:
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMISMS_MENU_SETTING_SIM4_SC_ITEM_ID:
#endif
            dual_sys = MN_DUAL_SYS_1+(menu_id-MMISMS_MENU_SETTING_SIM1_SC_ITEM_ID);
            MMISMS_OpenSetSCWin(dual_sys);
            break;        
#else
        case MMISMS_MENU_SETTING_SC_ITEM_ID:
            dual_sys = MN_DUAL_SYS_1;
            MMISMS_OpenSetSCWin(dual_sys);
            break;
#endif
#endif//MMI_SMS_MULTI_PARAM_SUPPORT

        case MMISMS_MENU_SETTING_SR_ITEM_ID:
            MMISMS_OpenSetAutoSaveWin();
            break;

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
        case MMISMS_MENU_SETTING_STATUS_ITEM_ID:
            MMK_CreateWin((uint32*)MMISMS_SETTING_STATUS_TAB, PNULL);
            break;
#else
        case MMISMS_MENU_SETTING_STATUS_REPORT_ITEM_ID:
            MMISMS_OpenSetStatusReportWin();
            break;
#endif

#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
        case MMISMS_MENU_SETTING_VP_ITEM_ID:
            MMISMS_OpenSetVPWin();
            break;
#endif

        case MMISMS_MENU_SETTING_SAVE_POS_ITEM_ID:
            MMISMS_OpenSetSavePosWin();
            break;

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
        case MMISMS_MENU_SETTING_CONCATENATED_SMS_ID:
            MMK_CreateWin((uint32*)MMISMS_SETTING_CONCATENATED_SMS_WIN_TAB, PNULL);
            break;
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
        case MMISMS_MENU_SETTING_ALPHABET_MANAGEMENT_ID:
            MMK_CreateWin((uint32*)MMISMS_SETTING_ALPHABET_MANAGEMENT_WIN_TAB, PNULL);
            break;
#endif

#if defined(MMI_SMS_EXPORT_SUPPORT)
        case MMISMS_MENU_SETTING_EXPORT_ITEM_ID:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                break;
            }	        
            MMISMS_EnterExportMainWin();
            break;
#endif

#ifdef TTS_SUPPORT
        /*
        case MMISMS_MENU_SETTING_TTS_ITEM_ID:
            MMK_CreateWin((uint32*)MMISMS_SET_TTS_WIN_TAB, PNULL);
            break;
            */
        case MMISMS_MENU_SETTING_TTS_OPEN_CLOSE_ID:
            #ifdef MMI_PDA_SUPPORT
            MMK_CreatePubListWin((uint32*)MMISMS_SET_TTS_WIN_TAB, PNULL);
            #else
            MMK_CreateWin((uint32*)MMISMS_SET_TTS_WIN_TAB, PNULL);            
            #endif
            
            break;
        case MMISMS_MENU_SETTING_TTS_VOL_ID:
            OpenTTSVolumePanel(win_id, 0);
            break;
#endif
        case MMISMS_MENU_SETTING_PERFERRED_CONNECTION_ITEM_ID:
            //MMISMS_OpenSetPreferredConnection();
            break;

        default:
            break;
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
} 

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : to handle set browser information
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBrowserInfoWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    uint16			list_index = 0;
    BOOLEAN          flag = FALSE;
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            InitSettingBrowser(MMISMS_BROWSER_SETTING_CTRL_ID);
            MMK_SetAtvCtrl(win_id,MMISMS_BROWSER_SETTING_CTRL_ID);
            break;    
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            list_index = GUILIST_GetCurItemIndex(MMISMS_BROWSER_SETTING_CTRL_ID);

            if (!GUILIST_IsCurrItemInvalid(MMISMS_BROWSER_SETTING_CTRL_ID))
            {
                flag = !((BOOLEAN)list_index);
                MMISMS_SetPushSmsBrowserInfo(flag);
                 
                MMK_CloseWin( win_id );
            }
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
        }
        
        return recode;
}
#endif
#endif
#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : init browser info service loading operation
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitSettingServiceLoadingOperation(
                         MMI_CTRL_ID_T  ctrl_id
                         )
{
    uint16 cur_index = 0;
    MMISMS_SL_OPERATION_E oper = MMISMS_SL_OPERATION_ALWAYS;

    //set max item
    GUILIST_SetMaxItem(ctrl_id,MMISMS_SL_OPERATION_MAX,FALSE);

    //append item
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_ALWAYS,STXT_SAVE,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_PROMPT,STXT_SAVE,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_NEVER,STXT_SAVE,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        
    oper = MMISMS_GetPushSmsSLOperation();
    
    cur_index = (uint16)oper;
    
    //set current and selected item
    GUILIST_SetSelectedItem(ctrl_id,cur_index,TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_index);

    return;
}
#endif
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : to set the SMS status window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingSmsStatusWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM            param
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SETTING_STATUS_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMISMS_MENU_SETTING_STATUS_REPORT_ITEM_ID:
            MMISMS_OpenSetStatusReportWin();
            break;
            
        case MMISMS_MENU_SETTING_REPLY_PATH_ITEM_ID:
            OpenSetReplyPathWin();
            break;
        
        default:
            //SCI_TRACE_LOW:"MMISMS: HandleSettingSmsStatusWinMsg: menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_2277_112_2_18_2_53_34_468,(uint8*)"d",menu_id);
            break;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle window message about setting reply path flag.
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingReplyPathFlagWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM            param
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    BOOLEAN status = FALSE;
    uint16 cur_selection = 0;
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        MMK_SetAtvCtrl(win_id, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID);
        switch (cur_selection)
        {
        case 0:
            status = TRUE;;
            break;
        case 1:
            status = FALSE;
            break;
            
        default:
            status = FALSE;
            break;
        }
        
        MMISMS_SetReplyPathFlag(status);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to open the window to set the flag of reply path
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void OpenSetReplyPathWin(void)
{
    MMK_CreateWin((uint32*)MMISMS_SETTING_REPLY_PATH_FLAG_WIN_TAB, PNULL);
    
    MMISMS_SetEnableWin(MMISMS_GetReplyPathFlag());
}
#endif
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
/*****************************************************************************/
//  Description : to select sim param
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectSIMParamWinMsg(
                                              MMI_WIN_ID_T        win_id,        //IN:
                                              MMI_MESSAGE_ID_E    msg_id,        //IN:
                                              DPARAM                param        //IN:
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        {
            SetSIMMode(dual_sys, TRUE, param_to_create_setting_main_win->set_mode.mode);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            GUIWIN_SetTitleTextId(win_id, TXT_SMS_SIM1_SET + dual_sys, FALSE);
#endif

            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_EDIT, STXT_RETURN, FALSE);

            MMK_SetAtvCtrl(win_id, MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID);
        }
        break;

    case MSG_GET_FOCUS:
        SetSIMMode(dual_sys, FALSE, param_to_create_setting_main_win->set_mode.mode);
        break;
    
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID);
        MMISMS_SetSIMModeFlag(dual_sys, (uint8)cur_selection);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_LIST_LONGOK:
        //MMK_CreateWin((uint32*)MMISMS_SELECTSIMMODE_LONGOK_WIN_TAB, (ADD_DATA)dual_sys);
        {
            MMIPUB_MENU_PARAM_T menuparam = {0};
            menuparam.group_id = MENU_SMS_PARAM_LINKLIST_LONGOK;
            menuparam.win_id =   MMISMS_SELECTSIMMODE_LONGOK_WIN_ID;
            menuparam.ctrl_id =  MMISMS_SELECTSIMMODE_LISTBOX_LONGOK_CTRL_ID;    
            menuparam.dst_handle = win_id;
            menuparam.func.OnMenuOk = HandleSMSParamListLongOKMenuWinOKMsg;                    
            MMIPUB_OpenPubMenuWin( &menuparam ); 
        }
        break;
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID);
        //g_mmisms_global.set_mode.mode = cur_selection;
        param_to_create_setting_main_win->set_mode.mode = cur_selection;
        MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
        SaveSIMModeToGlobal(dual_sys, &param_to_create_setting_main_win->set_mode);
        MMK_CreateWin((uint32*)MMISMS_SETTING_SIMMODE_MENUWIN_TAB, (ADD_DATA)dual_sys);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID);
        MMISMS_SetSIMModeFlag(dual_sys, (uint8)cur_selection);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin(win_id);
        break;
#endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : HandleSMSParamListLongOKMenuWinOKMsg
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSParamListLongOKMenuWinOKMsg( MMIPUB_MENU_NOTIFY_T* param )
{
   
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 menu_id = 0;
    uint16 list_index = 0;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(param->win_id);/*lint !e64*/
    uint32 add_data = 0;
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif

    menu_id = param->menu_id;
    switch(menu_id)
    {
    case MMISMS_MENU_SETTING_PARAM_EDIT_ID:            
        list_index = GUILIST_GetCurItemIndex(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID);

        //g_mmisms_global.set_mode.mode = list_index;
        param_to_create_setting_main_win->set_mode.mode = list_index;
        MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
        SaveSIMModeToGlobal(dual_sys, &param_to_create_setting_main_win->set_mode);

        add_data = (uint32)((dual_sys & 0xFF) | ((list_index & 0xFF) << 8));

        MMK_CreateWin((uint32*)MMISMS_SETTING_SIMMODE_MENUWIN_TAB, (ADD_DATA)add_data);

        MMK_CloseWin(param->win_id);
        break;
    
    default:
        break;
    }

    return recode;
}

#if 0
/*****************************************************************************/
// 	Description : HandleSMSParamListLongOKMenuWinMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSMSParamListLongOKMenuWinMsg(	
									 MMI_WIN_ID_T win_id, 
									 MMI_MESSAGE_ID_E msg_id,
									 DPARAM param)
{
   
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 group_id = 0, menu_id = 0;
    uint16 list_index = 0;
    MN_DUAL_SYS_E dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    uint32 add_data = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISMS_SELECTSIMMODE_LISTBOX_LONGOK_CTRL_ID);            
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(MMISMS_SELECTSIMMODE_LISTBOX_LONGOK_CTRL_ID,&group_id,&menu_id);
        switch(menu_id)
        {
        case MMISMS_MENU_SETTING_PARAM_EDIT_ID:            
            list_index = GUILIST_GetCurItemIndex(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID);

            g_mmisms_global.set_mode.mode = list_index;
            SaveSIMModeToGlobal(dual_sys, g_mmisms_global.set_mode.mode);

            add_data = (uint32)((dual_sys & 0xFF) | ((list_index & 0xFF) << 8));

            MMK_CreateWin((uint32*)MMISMS_SETTING_SIMMODE_MENUWIN_TAB, (ADD_DATA)add_data);

            MMK_CloseWin(win_id);
            break;
        
        default:
            break;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}
#endif
#endif

/*****************************************************************************/
//  Description : to save the sim mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SaveSIMModeToGlobal(
                               MN_DUAL_SYS_E dual_sys,
                               MMISMS_SET_SIMMODE_T *set_mode_ptr
                              )
{
    MN_SMS_PARAM_T *sms_param_ptr = PNULL;

    sms_param_ptr = SCI_ALLOCAZ(sizeof(MN_SMS_PARAM_T));

    if (PNULL == sms_param_ptr)
    {
        return;
    }

    MNSMS_GetSmsParamEx(
                        dual_sys,
                        set_mode_ptr->mode + 1,
                        sms_param_ptr
                       );

    set_mode_ptr->alpha_id_present =  sms_param_ptr->alpha_id_present;
    if(set_mode_ptr->alpha_id_present)
    {
        SCI_MEMCPY(&set_mode_ptr->alpha_id_t,&sms_param_ptr->alpha_id_t,sizeof(MN_SMS_SUPPORT_LANG_T));
    }

    set_mode_ptr->tp_sc_addr_present = sms_param_ptr->tp_sc_addr_present;
    if(set_mode_ptr->tp_sc_addr_present)
    {
        SCI_MEMCPY(&set_mode_ptr->tp_sc_addr_t,&sms_param_ptr->tp_sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
    }

    set_mode_ptr->tp_pid_present = sms_param_ptr->tp_pid_present;
    if(set_mode_ptr->tp_pid_present)
    {
        set_mode_ptr->tp_pid = sms_param_ptr->tp_pid;
    }   

    set_mode_ptr->tp_vp_present = sms_param_ptr->tp_vp_present;
    if(set_mode_ptr->tp_vp_present)
    {
        set_mode_ptr->vp_second = sms_param_ptr->vp_second;
    } 
#if 0
    g_mmisms_global.set_mode.alpha_id_present =  sms_param_ptr->alpha_id_present;
    if(g_mmisms_global.set_mode.alpha_id_present)
    {
        SCI_MEMCPY(&g_mmisms_global.set_mode.alpha_id_t,&sms_param_ptr->alpha_id_t,sizeof(MN_SMS_SUPPORT_LANG_T));
    }

    g_mmisms_global.set_mode.tp_sc_addr_present = sms_param_ptr->tp_sc_addr_present;
    if(g_mmisms_global.set_mode.tp_sc_addr_present)
    {
        SCI_MEMCPY(&g_mmisms_global.set_mode.tp_sc_addr_t,&sms_param_ptr->tp_sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
    }

    g_mmisms_global.set_mode.tp_pid_present = sms_param_ptr->tp_pid_present;
    if(g_mmisms_global.set_mode.tp_pid_present)
    {
        g_mmisms_global.set_mode.tp_pid = sms_param_ptr->tp_pid;
    }   

    g_mmisms_global.set_mode.tp_vp_present = sms_param_ptr->tp_vp_present;
    if(g_mmisms_global.set_mode.tp_vp_present)
    {
        g_mmisms_global.set_mode.vp_second = sms_param_ptr->vp_second;
    }
#endif
    MMISMS_Set_Setmode(set_mode_ptr);
    
    SCI_FREE(sms_param_ptr);	
    sms_param_ptr = PNULL;
}
#if 0
/*****************************************************************************/
//  Description : to save the sim MN
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveSIMModeToMN(void)
{
    MN_SMS_PARAM_T *sms_param_ptr = PNULL;  

    sms_param_ptr = SCI_ALLOCAZ(sizeof(MN_SMS_PARAM_T));

    if (PNULL == sms_param_ptr)
    {
        return;
    } 

    MNSMS_GetSmsParamEx(
                        g_mmisms_global.set_mode.dual_sys,
                        g_mmisms_global.set_mode.mode + 1,
                        sms_param_ptr
                       );

    sms_param_ptr->alpha_id_present = g_mmisms_global.set_mode.alpha_id_present;
    if(g_mmisms_global.set_mode.alpha_id_present)
    {
        SCI_MEMCPY(&sms_param_ptr->alpha_id_t,&g_mmisms_global.set_mode.alpha_id_t,sizeof(MN_SMS_SUPPORT_LANG_T));
    }
    else
    {
        SCI_MEMSET(&sms_param_ptr->alpha_id_t,0x00,sizeof(MN_SMS_SUPPORT_LANG_T));
    }

    sms_param_ptr->tp_sc_addr_present = g_mmisms_global.set_mode.tp_sc_addr_present;
    if(g_mmisms_global.set_mode.tp_sc_addr_present)
    {
        SCI_MEMCPY(&sms_param_ptr->tp_sc_addr_t,&g_mmisms_global.set_mode.tp_sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
    }
    else
    {
        SCI_MEMSET(&sms_param_ptr->tp_sc_addr_t,0x00,sizeof(MN_CALLED_NUMBER_T));
    }

    sms_param_ptr->tp_pid_present = g_mmisms_global.set_mode.tp_pid_present;
    if(g_mmisms_global.set_mode.tp_pid_present)
    {
        sms_param_ptr->tp_pid = g_mmisms_global.set_mode.tp_pid;
    }
    else
    {
        sms_param_ptr->tp_pid = MN_SMS_PID_DEFAULT_TYPE;
    }

    sms_param_ptr->tp_vp_present = g_mmisms_global.set_mode.tp_vp_present;
    if(g_mmisms_global.set_mode.tp_vp_present)
    {
        sms_param_ptr->vp_second = g_mmisms_global.set_mode.vp_second;
    }
    else
    {
        sms_param_ptr->vp_second = MMISMS_VALID_PERIOD_MAX;
    }

    MNSMS_SetSmsParamEx(
                        g_mmisms_global.set_mode.dual_sys,
                        g_mmisms_global.set_mode.mode + 1,
                        sms_param_ptr);
    
    SCI_FREE(sms_param_ptr);	
}
#endif
/*****************************************************************************/
//  Description : to set the sim mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetSIMMode(MN_DUAL_SYS_E dual_sys, BOOLEAN is_open, uint16 mode)
{
    uint8 i = 0;
    uint16 cur_selection = 0;
    uint8 mode_num = 0;
    MN_SMS_PARAM_T *sms_param_ptr = PNULL;
    MMI_STRING_T mode_mame = {0};
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/

    sms_param_ptr = SCI_ALLOCAZ(sizeof(MN_SMS_PARAM_T));

    if (PNULL == sms_param_ptr)
    {
        return;
    }

    mode_mame.wstr_ptr = SCI_ALLOCAZ(MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar));

    if(PNULL == mode_mame.wstr_ptr)
    {
        SCI_FREE(sms_param_ptr);

        return;
    } 
    
    MNSMS_GetSmsParamMaxNumEx(dual_sys,&mode_num);
    
    GUILIST_SetMaxItem(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID, mode_num, FALSE );//max item 10

    // add the item to listbox
    for (i = 0; i < mode_num; i++)
    {
        GUILIST_ITEM_DATA_T item_data ={0};/*lint !e64*/
        uint8 temp_str[MMISMS_STRNUM_MAX_LEN + 2] = {0};
        wchar temp_wstr[MMISMS_STRNUM_MAX_LEN + 2] = {0};
        uint8 temp_str_len = 0;

        item_t.item_style = GUIITEM_STYLE_TWO_LINE_RADIO;
        item_t.item_data_ptr = &item_data;/*lint !e733*/

        SCI_MEMSET(sms_param_ptr,0x0,sizeof(MN_SMS_PARAM_T));
        SCI_MEMSET(mode_mame.wstr_ptr,0x0,MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar));

        MNSMS_GetSmsParamEx(
                            dual_sys,
                            i + 1,
                            sms_param_ptr
                           );

        //SCI_TRACE_LOW:"SetSIMMode:i = %d,alpha_id_present = %d,len = %d,"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_2686_112_2_18_2_53_35_469,(uint8*)"ddd",i,sms_param_ptr->alpha_id_present,sms_param_ptr->alpha_id_t.id_len);

        ConvertSimToModeName(
                             &mode_mame,
                             sms_param_ptr->alpha_id_t.id_len,
                             sms_param_ptr->alpha_id_t.id
                            );

        if (sms_param_ptr->tp_sc_addr_present)
        {
            // translate the BCD code to string number
            temp_str_len = MMIAPICOM_GenDispNumber( 
                sms_param_ptr->tp_sc_addr_t.number_type, 
                sms_param_ptr->tp_sc_addr_t.num_len, 
                sms_param_ptr->tp_sc_addr_t.party_num,
                temp_str,
                MMISMS_STRNUM_MAX_LEN + 2 
                );

            if (temp_str_len > 0)
            {
                MMI_STRNTOWSTR( temp_wstr,
                                MMISMS_STRNUM_MAX_LEN,
                                (const uint8 *)temp_str,
                                MMISMS_STRNUM_MAX_LEN,
                                temp_str_len);     
            }
        }

        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_EDIT;
        item_data.softkey_id[2] = STXT_RETURN;

        if(sms_param_ptr->alpha_id_present &&
           (0 != sms_param_ptr->alpha_id_t.id_len) &&
           (0 != mode_mame.wstr_len))
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = mode_mame.wstr_ptr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = mode_mame.wstr_len;
        }
        else
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id= TXT_SMS_SIM_MODE1 + i;
        }

        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = temp_str_len;

        GUILIST_AppendItem( MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID, &item_t );
    }
    
    GUILIST_SetOwnSofterKey(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID, TRUE);

    cur_selection = (uint8)MMISMS_GetSIMModeFlag(dual_sys);
    GUILIST_SetSelectedItem(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID, cur_selection, TRUE);
        
    if(is_open)
    {
        cur_selection = (uint8)MMISMS_GetSIMModeFlag(dual_sys);
        GUILIST_SetSelectedItem(MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID, cur_selection, TRUE);

    }
    else
    {
        cur_selection = mode;
    }
        
    GUILIST_SetCurItemIndex( MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID, cur_selection);

    SCI_FREE(mode_mame.wstr_ptr);	

    SCI_FREE(sms_param_ptr);	
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : InitPdaSmsParamSettingsListCtrl
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaSmsParamSettingsListCtrl(BOOLEAN is_need_replace, MMI_CTRL_ID_T ctrl_id, MN_DUAL_SYS_E dual_sys, uint8 mode_id)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint8 temp_str[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_wstr[MMISMS_STRNUM_MAX_LEN + 2] = {0};
    uint8 temp_str_len = 0;
    MMI_STRING_T text_str= {0};
    MMINV_SMS_VALID_PERIOD_T valid_period = 0;
    MN_SMS_PID_E sms_pid = 0;
    BOOLEAN    flag  = FALSE;
    MMINV_SMS_AUTOSAVE_T status = 0;
    MMISMS_SMS_SAVEPOS_TYPE_E pos = MMISMS_SAVEPOS_NONE;
    uint32 cur_selection  = 0;
    uint16 index = 0;
    uint16 focus_index = 0;
    uint16 i = 0;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    uint8 smsp_id = 0;
    MN_SMS_PARAM_T sms_param = {0};

    text_str.wstr_ptr = temp_wstr;

    smsp_id = mode_id + 1;
    err_code = MNSMS_GetSmsParamEx(dual_sys, smsp_id, &sms_param);

    //set max item
    if(is_need_replace)
    { 
        focus_index = GUILIST_GetCurItemIndex(ctrl_id); 
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_SMS_PARAM_SETTINGS_MAX, FALSE );   
    }

    if ( (is_need_replace && index == focus_index) ||!is_need_replace )
    {
        if (ERR_MNSMS_NONE == err_code)
        {
            ConvertSimToModeName(
                             &text_str,
                             sms_param.alpha_id_t.id_len,
                             sms_param.alpha_id_t.id
                            );
        }

        //Not exist, default name
        if (0 == text_str.wstr_len)
        {
            MMI_GetLabelTextByLang(TXT_SMS_SIM_MODE1 + dual_sys, &text_str);
        }

        //Append Item
        AppendSmsPdaSettingsItem2Line(
        ctrl_id,
        TXT_SMS_SIM_MODE_NAME,
        text_str,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMISMS_PDA_SMS_PARAM_SETTINGS_NAME,
        is_need_replace,
		GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
        );
    }
    index++;
    SCI_MEMSET(temp_str, 0x00, sizeof(temp_str));

    if( (is_need_replace && index == focus_index) ||!is_need_replace )
    {
        if (ERR_MNSMS_NONE == err_code
            && sms_param.tp_sc_addr_present)
        {
            // translate the BCD code to string number
            temp_str_len = MMIAPICOM_GenDispNumber( 
                sms_param.tp_sc_addr_t.number_type, 
                sms_param.tp_sc_addr_t.num_len, 
                sms_param.tp_sc_addr_t.party_num,
                temp_str,
                MMISMS_STRNUM_MAX_LEN + 2 
                );

            if (temp_str_len > 0)
            {
                MMI_STRNTOWSTR( temp_wstr,
                                MMISMS_STRNUM_MAX_LEN,
                                (const uint8 *)temp_str,
                                MMISMS_STRNUM_MAX_LEN,
                                temp_str_len);     
            }
        } 

        text_str.wstr_ptr = temp_wstr;
        text_str.wstr_len = temp_str_len;
        AppendSmsPdaSettingsItem2Line(
        ctrl_id,
        TXT_SMS_SETTING_SC,
        text_str,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMISMS_PDA_SMS_PARAM_SETTINGS_SC,
        is_need_replace,
		GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
        );       
    }
    index++;
    SCI_MEMSET(temp_str, 0x00, sizeof(temp_str));

    if((is_need_replace && index == focus_index) || !is_need_replace)
    {
        if (ERR_MNSMS_NONE == err_code
            && sms_param.tp_vp_present)
        {
            valid_period = sms_param.vp_second;
        }

        switch (valid_period)
        {
        case MMISMS_VALID_PERIOD_3600:
            cur_selection = 0;
            break;
            
        case MMISMS_VALID_PERIOD_21600:
            cur_selection = 1;
            break;
            
        case MMISMS_VALID_PERIOD_86400:
            cur_selection = 2;
            break;
            
        case MMISMS_VALID_PERIOD_259200:
            cur_selection = 3;
            break;
            
        case MMISMS_VALID_PERIOD_604800:
            cur_selection = 4;
            break;
            
        case MMISMS_VALID_PERIOD_MAX:
            cur_selection = 5;
            break;
            
        default:
            valid_period = MMISMS_VALID_PERIOD_MAX;
            cur_selection = 5;
            break;
        }
        
        MMI_GetLabelTextByLang(TXT_SMS_VP_1HOUR + cur_selection, &text_str);

        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_SETTING_VP,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_SMS_PARAM_SETTINGS_VALID_PERIOD,
            is_need_replace,
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);
    }    
    index++;
    SCI_MEMSET(temp_str, 0x00, sizeof(temp_str));
    
    if((is_need_replace && index == focus_index) || !is_need_replace)
    {
        if (ERR_MNSMS_NONE == err_code
            && sms_param.tp_pid_present)
        {
            sms_pid = sms_param.tp_pid;
        }

        switch (sms_pid)
        {
        case MN_SMS_PID_DEFAULT_TYPE:
            cur_selection = 0;
            break;

        case MN_SMS_PID_VOICE_TYPE:
            cur_selection = 1;
            break;

        case MN_SMS_PID_PAGING_TYPE:
            cur_selection = 2;
            break;

        case MN_SMS_PID_EMAIL_TYPE:
            cur_selection = 3;
            break;

        case MN_SMS_PID_ERMES_TYPE:
            cur_selection = 4;
            break;

        case MN_SMS_PID_X400_TYPE:
            cur_selection = 5;
            break;

        case MN_SMS_PID_GROUP3_FAX:
        case MN_SMS_PID_GROUP4_FAX:
            cur_selection = 6;
            break;
            
        default:
            cur_selection = 0;
            break;
        }
        
        MMI_GetLabelTextByLang(TXT_SMS_SIM_SEND_MODE_TEXT + cur_selection, &text_str);

        AppendSmsPdaSettingsItem2Line(            
            ctrl_id,
            TXT_SMS_SIM_SEND_MODE,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_SMS_PARAM_SETTINGS_SEND_MODE,
            is_need_replace,
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM);
    }
}
#endif

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode menu window message
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingSIMModeMenuWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM            param
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SETTING_SIMMODE_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T menu_id = 0;
#ifdef MMI_PDA_SUPPORT
    uint32 add_data = (uint32)MMK_GetWinAddDataPtr(win_id);
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)(add_data & 0xFF);
    uint8 mode_id = (uint8)((add_data & 0xFF00) >> 8);
    uint16 index = 0;
    uint32 user_data = 0;
#endif
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        InitPdaSmsParamSettingsListCtrl(FALSE, ctrl_id, dual_sys, mode_id);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, FALSE);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitPdaSmsParamSettingsListCtrl(TRUE, ctrl_id, dual_sys, mode_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT 
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
    case MSG_APP_WEB:     
        index = GUILIST_GetCurItemIndex(ctrl_id);

        GUILIST_GetItemData(ctrl_id, index, &user_data);

        switch((MMISMS_PDA_SMS_PARAM_TYPE_E)user_data)
        {
        case MMISMS_PDA_SMS_PARAM_SETTINGS_NAME:
            {
                GUIEDIT_TEXT_MAX_T max_len = {0};
                
                MMK_CreatePubEditWin((uint32 *)MMISMS_SET_SIMMODE_NAME_WIN_TAB, PNULL);

                //set max len
                max_len.ascii    = MMISMS_SET_SIMMODE_NAME_LEN;
                max_len.ucs2     = MMISMS_SET_SIMMODE_NAME_LEN/sizeof(wchar);
                max_len.r8_1byte = MMISMS_SET_SIMMODE_NAME_LEN -1;
                max_len.r8_2byte = MMISMS_SET_SIMMODE_NAME_LEN -2;
                GUIEDIT_SetTextMaxLenEx(MMISMS_SETTING_SIMMODE_NAME_CTRL_ID,&max_len);
            }
            break;
            
        case MMISMS_PDA_SMS_PARAM_SETTINGS_SC:
            MMISMS_OpenSetSCWin(dual_sys);
            break;
            
        case MMISMS_PDA_SMS_PARAM_SETTINGS_VALID_PERIOD:
            MMISMS_OpenSetVPWin(param_to_create_setting_main_win->set_mode);
            break;
            
        case MMISMS_PDA_SMS_PARAM_SETTINGS_SEND_MODE:
            MMK_CreateWin((uint32 *)MMISMS_SET_SIMMODE_SENDMODE_WIN_TAB, PNULL);
            SetSIMSendmode(param_to_create_setting_main_win->set_mode);
            break;
            
        default:
            break;
        }
        break;
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //save SIM mode to MN
        MMISMS_SaveSIMModeToMN();

        MMISMS_OpenSMSWaitingWin(MMISMS_WRITE_SMS_PARAM_WAITING_WIN_ID,
                                                            MMIPUB_SOFTKEY_NONE,
                                                            MMISMS_HandleWriteSMSParamWaitWinMsg,
                                                            win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMISMS_MENU_SETTING_SIM_MODENAME_ITEM_ID:
            {
                GUIEDIT_TEXT_MAX_T  max_len = {0};
                
                MMK_CreateWin((uint32 *)MMISMS_SET_SIMMODE_NAME_WIN_TAB, PNULL);

                //set max len
                max_len.ascii    = MMISMS_SET_SIMMODE_NAME_LEN;
                max_len.ucs2     = MMISMS_SET_SIMMODE_NAME_LEN/sizeof(wchar);
                max_len.r8_1byte = MMISMS_SET_SIMMODE_NAME_LEN -1;
                max_len.r8_2byte = MMISMS_SET_SIMMODE_NAME_LEN -2;
                GUIEDIT_SetTextMaxLenEx(MMISMS_SETTING_SIMMODE_NAME_CTRL_ID,&max_len);
            }
            break;

        case MMISMS_MENU_SETTING_SIM_SC_ITEM_ID:
            //MMISMS_OpenSetSCWin(g_mmisms_global.set_mode.dual_sys);
            MMISMS_OpenSetSCWin(param_to_create_setting_main_win->set_mode.dual_sys);
            break;

        case MMISMS_MENU_SETTING_VP_ITEM_ID:
            MMISMS_OpenSetVPWin(param_to_create_setting_main_win->set_mode);
            break;

        case MMISMS_MENU_SETTING_SIM_SENDTYPE_ITEM_ID:
            MMK_CreateWin((uint32 *)MMISMS_SET_SIMMODE_SENDMODE_WIN_TAB, PNULL);
            SetSIMSendmode(param_to_create_setting_main_win->set_mode);
            break;

        default:
            break;
        }
        break;
#endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode name window message
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingSIMModeNameWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM            param
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SETTING_SIMMODE_NAME_CTRL_ID;
    MMI_STRING_T edit_string = {0};
    BOOLEAN is_ucs2 = FALSE;
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        if (0 != param_to_create_setting_main_win->set_mode.alpha_id_t.id_len)
        {
            edit_string.wstr_ptr = SCI_ALLOCAZ(MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar));

            if (PNULL == edit_string.wstr_ptr)
            {
                return MMI_RESULT_TRUE;
            }

            ConvertSimToModeName(
                                 &edit_string,
                                 param_to_create_setting_main_win->set_mode.alpha_id_t.id_len,
                                 &(param_to_create_setting_main_win->set_mode.alpha_id_t.id[0])
                                );

            GUIEDIT_SetString(ctrl_id,edit_string.wstr_ptr,edit_string.wstr_len);

            SCI_FREE(edit_string.wstr_ptr);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString(ctrl_id, &edit_string);

        if(0 == edit_string.wstr_len)
        {
            //g_mmisms_global.set_mode.alpha_id_present = FALSE;
            //SCI_MEMSET(&g_mmisms_global.set_mode.alpha_id_t,0x0,sizeof(MN_SMS_SUPPORT_LANG_T));
            param_to_create_setting_main_win->set_mode.alpha_id_present = FALSE;
            SCI_MEMSET(&param_to_create_setting_main_win->set_mode.alpha_id_t,0x0,sizeof(MN_SMS_SUPPORT_LANG_T));
            MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
        }
        else
        {
            uint16 out_len = 0;
            
            PHONEBOOK_ALPHABET_TYPE_E alpha_type = PHONEBOOK_ALPHABET_ASCII;
            uint8 input_name[MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar) + 1] = {0};
            uint8 input_len = 0;
            uint8 output_name[MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar) + 1] = {0}; 
            
            //g_mmisms_global.set_mode.alpha_id_present = TRUE;
            param_to_create_setting_main_win->set_mode.alpha_id_present = TRUE;
            MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
            is_ucs2 = !(MMIAPICOM_IsASCIIString(edit_string.wstr_ptr, edit_string.wstr_len));
            if(is_ucs2)
            {
                alpha_type = PHONEBOOK_ALPHABET_UCS2;
                SCI_MEMCPY(input_name,(uint8 *)edit_string.wstr_ptr,edit_string.wstr_len *sizeof(wchar));
                input_len = edit_string.wstr_len *sizeof(wchar);
            }
            else
            {
                MMI_WSTRNTOSTR(input_name, MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar), edit_string.wstr_ptr, MN_SMS_MAX_SUPPORT_LANG_LEN * sizeof(wchar), edit_string.wstr_len * sizeof(wchar));
                input_len = edit_string.wstr_len;
            }

            MMIAPICOM_EncodeGSMAlphabet(
                                        alpha_type,
                                        input_len,
                                        input_name,
                                        &out_len,
                                        output_name,
                                        MMISMS_SET_SIMMODE_NAME_LEN + 1
                                       );
            //g_mmisms_global.set_mode.alpha_id_t.id_len = (uint8)out_len;
            //SCI_MEMCPY(g_mmisms_global.set_mode.alpha_id_t.id,output_name,out_len);
            param_to_create_setting_main_win->set_mode.alpha_id_t.id_len = (uint8)out_len;
            SCI_MEMCPY(param_to_create_setting_main_win->set_mode.alpha_id_t.id,output_name,out_len);
            MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
        }

#ifdef MMI_PDA_SUPPORT
        MMISMS_SaveSIMModeToMN();

        MMISMS_OpenSMSWaitingWin(MMISMS_WRITE_SMS_PARAM_WAITING_WIN_ID,
                                                            MMIPUB_SOFTKEY_NONE,
                                                            MMISMS_HandleWriteSMSParamWaitWinMsg,
                                                            win_id);
#else
        MMK_CloseWin(win_id);
#endif
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode send mode window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSmsSettingSIMModeSendmodeWinMsg(
                                                         MMI_WIN_ID_T        win_id,        //IN:
                                                         MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                         DPARAM                param        //IN:
                                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MN_SMS_PID_E send_mode = MN_SMS_PID_DEFAULT_TYPE;//need modify
#if defined (MMS_SUPPORT) || defined (BROWSER_SUPPORT)    
    MMI_WIN_ID_T main_win_id = MMISMS_SETTINGS_TAB_WIN_ID;
#else
    MMI_WIN_ID_T main_win_id = MMISMS_SMS_SETTING_MENU_WIN_ID;
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SETTING_MAIN_WIN_PARAM_T * param_to_create_setting_main_win = MMK_GetWinUserData(main_win_id);
#endif
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        MMK_SetAtvCtrl(win_id, MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID);
        switch (cur_selection)
        {
        case 0:
            send_mode = MN_SMS_PID_DEFAULT_TYPE;
            break;

        case 1:
            send_mode = MN_SMS_PID_VOICE_TYPE;
            break;

        case 2:
            send_mode = MN_SMS_PID_PAGING_TYPE;
            break;

        case 3:
            send_mode = MN_SMS_PID_EMAIL_TYPE;
            break;
            
        case 4:
            send_mode = MN_SMS_PID_ERMES_TYPE;
            break;

        case 5:
            send_mode = MN_SMS_PID_X400_TYPE;
            break;

        case 6:
            send_mode = MN_SMS_PID_GROUP3_FAX;
            break;

        default:
            send_mode = MN_SMS_PID_DEFAULT_TYPE;
            break;
        }

        //g_mmisms_global.set_mode.tp_pid_present = TRUE;
        //g_mmisms_global.set_mode.tp_pid = send_mode;
        param_to_create_setting_main_win->set_mode.tp_pid_present = TRUE;
        param_to_create_setting_main_win->set_mode.tp_pid = send_mode;
        MMISMS_Set_Setmode(&param_to_create_setting_main_win->set_mode);
#ifdef MMI_PDA_SUPPORT
        MMISMS_SaveSIMModeToMN();

        MMISMS_OpenSMSWaitingWin(MMISMS_WRITE_SMS_PARAM_WAITING_WIN_ID,
                                                            MMIPUB_SOFTKEY_NONE,
                                                            MMISMS_HandleWriteSMSParamWaitWinMsg,
                                                            win_id);
#else
        MMK_CloseWin(win_id);
#endif
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : to set sim send mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetSIMSendmode(MMISMS_SET_SIMMODE_T set_mode)
{
    uint8 i = 0;
    uint16 cur_selection = 0;
    MN_SMS_PID_E send_mode = MN_SMS_PID_DEFAULT_TYPE;//need modify
    
    GUILIST_SetMaxItem(MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID, MMISMS_SIM_SEND_MODE_ITEM, FALSE );//max item 6
    // add the item to listbox
    for (i = 0; i < MMISMS_SIM_SEND_MODE_ITEM; i++)
    {
        MMIAPISET_AppendListItemByTextId( 
                                        (TXT_SMS_SIM_SEND_MODE_TEXT + i),
                                        TXT_COMMON_OK, 
                                        MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID,
                                        GUIITEM_STYLE_ONE_LINE_RADIO
                                        );
    }

    if(set_mode.tp_pid_present)
    {
        send_mode = set_mode.tp_pid;
    }
    else
    {
        send_mode = MN_SMS_PID_DEFAULT_TYPE;
    }
    
    switch (send_mode)
    {
    case MN_SMS_PID_DEFAULT_TYPE:
        cur_selection = 0;
        break;

    case MN_SMS_PID_VOICE_TYPE:
        cur_selection = 1;
        break;

    case MN_SMS_PID_PAGING_TYPE:
        cur_selection = 2;
        break;

    case MN_SMS_PID_EMAIL_TYPE:
        cur_selection = 3;
        break;

    case MN_SMS_PID_ERMES_TYPE:
        cur_selection = 4;
        break;

    case MN_SMS_PID_X400_TYPE:
        cur_selection = 5;
        break;

    case MN_SMS_PID_GROUP3_FAX:
    case MN_SMS_PID_GROUP4_FAX:
        cur_selection = 6;
        break;
        
    default:
        cur_selection = 0;
        break;
    }
        
    GUILIST_SetSelectedItem(MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID, cur_selection, TRUE);
    GUILIST_SetCurItemIndex( MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID, cur_selection);
}

/*****************************************************************************/
//  Description : 将名称转化为字符串
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void ConvertSimToModeName(
                                MMI_STRING_T *mode_name,    //OUT
                                uint8           sim_alpha_len,  //IN:
                                uint8           *sim_alpha_ptr  //IN:
                               )
{
    BOOLEAN is_ucs2 =FALSE;
    uint16   output_len = 0;
    uint8   output_str[MMISMS_SET_SIMMODE_NAME_LEN + 1] = {0};
    uint8   ascii_data[MMISMS_SET_SIMMODE_NAME_LEN + 1] = {0};
    uint16  data_length = 0;
    PHONEBOOK_ALPHABET_TYPE_E alpha_type = PHONEBOOK_ALPHABET_ASCII;
    
    if(PNULL == sim_alpha_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:ConvertSimToModeName sim_alpha_ptr 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_3428_112_2_18_2_53_36_470,(uint8*)"d", sim_alpha_ptr);
        return;
    } 
    
    if (0 < sim_alpha_len)
    {
        uint8   input_len = (uint8)MIN(sim_alpha_len, MMISMS_SET_SIMMODE_NAME_LEN + 1);
        if (!MMIAPICOM_DecodeGSMAlphabet(
            &alpha_type, 
            input_len,
            sim_alpha_ptr,
			MMISMS_SET_SIMMODE_NAME_LEN + 1,
            &output_len,
            output_str
            ))
        {
            //SCI_TRACE_LOW:"MMISMS:ConvertSimToModeName decode alphabet with sim standard is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_3444_112_2_18_2_53_36_471,(uint8*)"");
        }

        is_ucs2 = (PHONEBOOK_ALPHABET_UCS2 == alpha_type) ? TRUE : FALSE;
        
        if (PHONEBOOK_ALPHABET_ASCII == alpha_type)
        {
            
            data_length = MMIAPICOM_Default2Ascii( 
                                                  output_str,
                                                  ascii_data,
                                                  output_len
                                                 );
            
            output_len = MIN(data_length, MMISMS_SET_SIMMODE_NAME_LEN);
            
            SCI_MEMCPY(output_str, ascii_data, output_len);	
        }
    }
    else
    {
        is_ucs2   = FALSE;
    }

    if(is_ucs2)
    {
        MMIAPICOM_Wstrncpy(mode_name->wstr_ptr, (const wchar *)output_str, output_len);
        mode_name->wstr_len = output_len/sizeof(wchar);
    }
    else
    {
        mode_name->wstr_len = output_len;
        MMI_STRNTOWSTR(
                       mode_name->wstr_ptr,
                       MMISMS_SET_SIMMODE_NAME_LEN * sizeof(wchar),
                       output_str,
                       MMISMS_SET_SIMMODE_NAME_LEN,
                       output_len);	
    }
}
#endif//MMI_SMS_MULTI_PARAM_SUPPORT

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : to handle window message about setting Concatenated SMS
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingConcatenatedSMSWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMISMS_CONCATENATED_SMS_LISTBOX_CTRL_ID;
    BOOLEAN is_contenated_sms = TRUE;

    //SCI_TRACE_LOW:"HandleSettingConcatenatedSMSWinMsg msg_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_3503_112_2_18_2_53_37_472,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        // add the item to listbox
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextId( TXT_COMMON_ENABLE_ON, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        is_contenated_sms = MMISMS_GetConcatenatedSMS();
        //SCI_TRACE_LOW:"HandleSettingConcatenatedSMSWinMsg is_contenated_sms=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_3514_112_2_18_2_53_37_473,(uint8*)"d", is_contenated_sms);

        if (is_contenated_sms)
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch (cur_selection)
        {
        case 0:
            is_contenated_sms = TRUE;
            break;

        case 1:
            is_contenated_sms = FALSE;
            break;
            
        default:
            break;
        }

        if (is_contenated_sms != MMISMS_GetConcatenatedSMS())
        {
            MMISMS_SetConcatenatedSMS(is_contenated_sms);

            MMISMS_FullOrder();

            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);    

            MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
        }

#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#endif

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
/*****************************************************************************/
//     Description : to handle window message about setting alphabet management
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingAlphabetManagementWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMISMS_ALPHABET_MANAGEMENT_LISTBOX_CTRL_ID;
    MMISMS_ALPHABET_MANAGEMENT_E alpha_management = MMISMS_ALPHABET_MANAGEMENT_DEFAULT;

    //SCI_TRACE_LOW:"HandleSettingAlphabetManagementWinMsg msg_id is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_3598_112_2_18_2_53_37_474,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        // add the item to listbox
        GUILIST_SetMaxItem(ctrl_id, MMISMS_ALPHABET_MANAGEMENT_MAX, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextId( TXT_SMS_ALPHABET_GSM_ALPHABET, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId( TXT_SMS_ALPHABET_8BIT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId( TXT_SMS_ALPHABET_UCS2, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId( TXT_AUTO, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        alpha_management = MMISMS_GetAlphabetManagement();
        //SCI_TRACE_LOW:"HandleSettingAlphabetManagementWinMsg alpha_management=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SETTINGWIN_3611_112_2_18_2_53_37_475,(uint8*)"d", alpha_management);

        cur_selection = (uint16)alpha_management;
        
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        alpha_management = (MMISMS_ALPHABET_MANAGEMENT_E)cur_selection;

        MMISMS_SetAlphabetManagement(alpha_management);

#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#ifdef MMI_SMS_FIX_SEND_SIM

LOCAL void SetFixSIMSendingListCtrl(MMI_CTRL_ID_T ctrl_id)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    MMI_STRING_T text_str= {0};
    uint32  index = 0;
    MMI_TEXT_ID_T    text_id = 0;
    BOOLEAN sim_sending_off = FALSE;

    //If fix sending is Open
    if(MMISMS_GetFixSIMSendingOnOFF())
     {
            image_id = IMAGE_CHECK_SELECTED_ICON;
            text_id = TXT_COMMON_ON;
     }
     else
     {
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
            text_id = TXT_COMMON_OFF;
     }      
    GUILIST_SetMaxItem(ctrl_id, 2,FALSE);
    MMI_GetLabelTextByLang(text_id, &text_str);
    AppendSmsPdaSettingsItem2Line(
                                ctrl_id,
                                TXT_SMS_FIX_SIM_SEND_ONOFF,
                                text_str,
                                image_id,
                                0,
                                FALSE,
                                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
                                );

    index++;

    MMI_GetLabelTextByLang(TXT_SIM_SEL_SIM1 + MMISMS_GetFixSIMSendingSIMSYS(), &text_str);
    AppendSmsPdaSettingsItem2Line(
                                ctrl_id,
                                TXT_SMS_FIX_SIM_SEND,
                                text_str,
                                0,
                                1,
                                FALSE,
                                GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM
                                );
    sim_sending_off = !MMISMS_GetFixSIMSendingOnOFF();
    GUILIST_SetItemGray(ctrl_id, index, sim_sending_off);
}


LOCAL MMI_RESULT_E  FixSimSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        MMISMS_SetFixSIMSendingSIMSYS(dual_sys);
    }

    return recode;

}

LOCAL void MMISMS_OpenFixSimSendingSettingWin(void)
{
    // MMI_STRING_T wait_text = {0};
    uint16 sim_ok = 0;
    uint32 sim_ok_num = 0;

    {
        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

        if (0 == sim_ok_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_ok_num)
        {
            MMISMS_SetFixSIMSendingSIMSYS(sim_ok);
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(FixSimSelectSimCallback,0,TRUE);
        }
#endif
    }
}

/*****************************************************************************/
//  Description : to handle the SMS setting SIM mode send mode window message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFixSimSendingListWindow(
                                                         MMI_WIN_ID_T        win_id,        //IN:
                                                         MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                         DPARAM                param        //IN:
                                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T    ctrl_id = MMISMS_SMS_SETTING_FIXSIMLIST_CTRL_ID;
    BOOLEAN sim_sending_off = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
    case MSG_FULL_PAINT:        
        SetFixSIMSendingListCtrl(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        if (MMISMS_GetFixSIMSendingOnOFF())
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_CLOSE, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OPEN, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if (cur_selection == 0)
        {
            sim_sending_off = !MMISMS_GetFixSIMSendingOnOFF();
            MMISMS_SetFixSIMSendingOnOFF(sim_sending_off);
            MMK_PostMsg(win_id,MSG_FULL_PAINT , 0, 0);
        }
        else if (cur_selection ==1)
        {
            //Open SelectSIM
            MMISMS_OpenFixSimSendingSettingWin();
        }
        
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

PUBLIC BOOLEAN MMISMS_SetFixSIMSendingSIMSYS(uint32 simsys)
{
    MMINV_WRITE(MMINV_SMS_FIX_SIM_SEND_SIMSYS,&simsys);

    return TRUE;
}

PUBLIC uint32 MMISMS_GetFixSIMSendingSIMSYS(void)
{
    uint32 return_value =0 ;
    uint32 fix_simsys = 0;
    MMINV_READ(MMINV_SMS_FIX_SIM_SEND_SIMSYS, &fix_simsys, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_TRACE_LOW("MMISMS_GetFixSIMSendingSIMSYS , Read Nv Error");
        return 0;
    }
    return fix_simsys;
}

PUBLIC void MMISMS_SetFixSIMSendingOnOFF(BOOLEAN is_on)
{
    MMINV_WRITE(MMINV_SMS_FIX_SIM_SEND_ON,&is_on);
    return;
}

PUBLIC BOOLEAN MMISMS_GetFixSIMSendingOnOFF(void)
{
    uint32 return_value =0 ;
    BOOLEAN is_fix_on = 0;
    MMINV_READ(MMINV_SMS_FIX_SIM_SEND_ON, &is_fix_on, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_TRACE_LOW("MMISMS_GetFixSIMSendingOnOFF , Read Nv Error");
        return FALSE;
    }
    return is_fix_on;
}
#endif

/*****************************************************************************/
//     Description : to handle the message of waiting window    
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISMS_HandleWriteSMSParamWaitWinMsg(
                                     MMI_WIN_ID_T win_id,    //IN:
                                     MMI_MESSAGE_ID_E msg_id,    //IN:
                                     DPARAM param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T* add_data = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_WIN_ID_T pre_win_id = 0;

    if (PNULL != add_data)
    {
        pre_win_id = (MMI_WIN_ID_T)(add_data->user_data);
    }

    switch (msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;

    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(pre_win_id);

#ifdef MMI_GUI_STYLE_TYPICAL
        //提示保存成功
#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
#endif

        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : to handle the message of waiting window
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSMSWaitingWin(MMI_WIN_ID_T win_id,
                                                                                MMIPUB_SOFTKEY_STYLE_E softkey_style,
                                                                                MMIPUB_HANDLE_FUNC handle_func,
                                                                                uint32 user_data)
{
    MMI_STRING_T prompt_str = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);

    MMIPUB_OpenWaitWinEx(MMK_GetFirstAppletHandle(), 1, &prompt_str, PNULL, PNULL, win_id, IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE, handle_func, user_data);
}

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//     Description : to open the window to set the preferred connection
//    Global resource dependence : 
//  Author: Liqing Peng
//    Note: 
/*****************************************************************************/
void MMISMS_OpenSetPreferredConnection(void)
{
    MMK_CreateWin((uint32*)MMISMS_SETTING_PREFERRED_CONNECTION, PNULL);
}

/*****************************************************************************/
//     Description : to handle window message about setting preferred connection
//    Global resource dependence : 
//  Author: liqing.peng
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingPreferredConnectionWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM            param
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMISMS_PERFERRED_CONNECTION_LISTBOX_CTRL_ID;
    MMISMS_PREFERRED_PATH_E preferred_connection = MMISMS_PREFERRED_PATH_GSM_PREFERRED;

    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        // add the item to listbox, SIM and Phone
        GUILIST_SetMaxItem(ctrl_id, MMISMS_PREFERRED_PATH_MAX, FALSE);//max item 4
        MMIAPISET_AppendListItemByTextId(TXT_COMM_GSM_PREFER, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SMS_PREFERRED_GPRS, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SMS_PREFERRED_GSM_ONLY, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SMS_PREFERRED_GPRS_ONLY, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        
        preferred_connection = MMISMS_GetPreferredConnection();

        cur_selection = (uint16)preferred_connection;
        
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        preferred_connection = (MMISMS_PREFERRED_PATH_E)cur_selection;

        MMISMS_SetPreferredConnection(preferred_connection);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//     Description : to set the MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_SetEnableWin( 
                        BOOLEAN is_enable 
                        )
{
    uint16 cur_selection = 0;
    
    GUILIST_SetMaxItem(MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID, 2, FALSE );//max item 2
    // add the item to listbox
    MMIAPISET_AppendListItemByTextId( TXT_COMMON_ENABLE_ON, STXT_OK, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_OK, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
        
    switch(is_enable)
    {
    case FALSE:
        cur_selection = 1;
        break;
    case TRUE:
        cur_selection = 0;
        break;
    default:
        cur_selection = 1;
        break;
    }
    
    GUILIST_SetSelectedItem( MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID, cur_selection, TRUE);
    GUILIST_SetCurItemIndex( MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID, cur_selection);
}


/*****************************************************************************/
//     Description : to open the window to set the flag of status report
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetStatusReportWin(void)
{
#ifdef MMI_PDA_SUPPORT
    MMK_CreatePubListWin((uint32*)MMISMS_SETTING_STATUS_REPORT_FLAG_WIN_TAB, PNULL);
#else
    MMK_CreateWin((uint32*)MMISMS_SETTING_STATUS_REPORT_FLAG_WIN_TAB, PNULL);
#endif    
    MMISMS_SetEnableWin(MMISMS_GetStatusReportFlag());
}

/*****************************************************************************/
//     Description : to handle window message about setting status report flag.
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingStatusReportFlagWinMsg(
                                                       MMI_WIN_ID_T        win_id, 
                                                       MMI_MESSAGE_ID_E msg_id, 
                                                       DPARAM            param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    BOOLEAN status = FALSE;
    uint16 cur_selection = 0;
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        MMK_SetAtvCtrl(win_id, MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID);
        switch (cur_selection)
        {
        case 0:
            status = TRUE;;
            break;
        case 1:
            status = FALSE;
            break;
            
        default:
            status = FALSE;
            break;
        }
        
        MMISMS_SetStatusReportFlag(status);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to open the window to set the flag of autosave
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_OpenSetAutoSaveWin(void)
{
    MMK_CreatePubListWin((uint32 *)MMISMS_SELECTSR_WIN_TAB, PNULL);
}

/*****************************************************************************/
//     Description : to handle auto save window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoSaveWinMsg(
                                        MMI_WIN_ID_T        win_id,        //IN:
                                        MMI_MESSAGE_ID_E    msg_id,        //IN:
                                        DPARAM                param        //IN:
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID;
    MMINV_SMS_AUTOSAVE_T status = MMINV_SMS_SETTING_PROMPT;
    uint16 cur_selection = 0;
    
    switch (msg_id)
    {   
    case MSG_OPEN_WINDOW:      
        GUILIST_SetMaxItem(ctrl_id, 3, FALSE );//max item 3
        MMIAPISET_AppendListItemByTextId(TXT_SMS_PROMPT, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(STXT_SAVE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_SMS_NOT_SAVE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

        status = MMISMS_GetAutoSave();
        GUILIST_SetSelectedItem(ctrl_id, (uint16)status, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, (uint16)status);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch( cur_selection )
        {
        case 0:
        case 1:
        case 2:
            status = (MMINV_SMS_AUTOSAVE_T)cur_selection;
            break;
            
        default:
            status = MMINV_SMS_SETTING_PROMPT;
            break;
        }
        MMISMS_SetAutoSave(status);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
#endif

/*Edit by script, ignore 19 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
