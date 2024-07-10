/***************************************************************************
** File Name:      mmidm_wintab.c                                          *
** Author:         jian.ma                                                 *
** Date:           2009/07/02                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2009/07/02     jian.ma          Create                                  *
**                                                                         *
****************************************************************************/

#define _MMIDM_WINTAB_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
//#include "std_header.h"
#if (defined (DM_SUPPORT) || defined (DM_ONLY_SELFREG_SUPPORT))
#include "mmi_app_dm_trc.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmidm_text.h"
#include "mmidm_app.h"
#include "mmisms_app.h"
#include "mmicc_export.h"
#include "mmipub.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmidm_id.h"
//#include "guilabel.h"
#include "mmidm_id.h"
#include "mmidisplay_data.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiudisk_export.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmk_app.h"
#include "guiprgbox.h"
#include "mmidm_menutable.h"
#include "mmi_default.h"
#ifndef DM_ONLY_SELFREG_SUPPORT
#include "mmidm_internal.h"
#include "rdm_error.h"
#endif /*DM_ONLY_SELFREG_SUPPORT*/
#include "guitext.h"
#include "guiim_base.h"
#include "guiedit.h"
#include "mmidm_nv.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "gui_ucs2b_converter.h"
//#include "fota.h"
#include "mmicc_position.h"
#include "mmicc_image.h"
#include "guibutton.h"
#include "mmifm_export.h"
#include "guires.h"
#include "mmistk_export.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIDM_NOTIFY_VIBRA_TIMEROUT		3000
#define MMIDM_SESSION_PRG_TEXT_LEN      40
#define MMIDM_ONE_MIN_TIME              (60*1000) //1min
#define MMIDM_DL_CHECK_TIME             MMIDM_ONE_MIN_TIME //1min
#define MMIDM_STATE_CHECK_TIME          (5*MMIDM_ONE_MIN_TIME) //5min
#define MMIDM_DISPLAY_DELAY_TIME        (5*MMIDM_ONE_MIN_TIME) //5min
#define MMIDM_RUN_CHECK_TIME            (10*MMIDM_ONE_MIN_TIME) //10min
#define MMIDM_FOTA_DELAY_TIME           (60*MMIDM_ONE_MIN_TIME) //1 hous
#ifdef DM_SUPPORT_DFRESH
#define MMIDM_SOCKET_DELAY_TIME           (5*MMIDM_ONE_MIN_TIME) //5min
#endif
#define MMIDM_MAX_ALERT_DISPLAY_TIME    (5*1000)//5s
#define MMIDM_WAIT_DM_SESSION_TIME      1000
#ifdef WIN32
#define PROJECT_BUILD_TIME          "/2009/07/22"
#endif
#define MMIDM_DEBUG_MAX_INPUT_LEN   100
#define MMIDM_SELECT_SERVER_ITEM    3
#define MMIDM_DELAY_TIME_ITEM       4
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//typedef enum
//{
//    MMIDM_DL_STATE_NONE,
//    MMIDM_DL_STATE_CANCEL,
//    MMIDM_DL_STATE_OK,
//} MMIDM_DL_STATE_E;

typedef struct
{
    MMIDM_ALERT_TYPE_E mode;
    wchar  buffer[MMIFMM_FILE_FILE_NAME_MAX_LEN + 1];
    uint16 len;
} DM_DISPLAY_INFO_T;


/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL  uint8 s_mmidm_win_timerid = 0;      //dm win timer id
LOCAL  uint8 s_mmidm_fumo_update_timerid = 0;      //dm fumo update timer id
LOCAL  uint8 s_mmidm_check_timerid = 0; // dm state check timer id
//LOCAL  uint8 s_mmidm_download_timerid = 0; //check if need cancal dm session
LOCAL  uint8 s_mmidm_run_timerid = 0; //when dm run check if need cancal dm session
LOCAL  uint8 s_mmidm_no_signal_timerid = 0; //no signal timer
LOCAL  uint8 s_mmidm_delay_timerid = 0; //wap timer
LOCAL  uint8 s_mmidm_dis_confim_timerid = 0;//display confim timer
LOCAL  uint8 s_mmidm_fota_report_timerid = 0; //fota report timer
LOCAL  uint8 s_mmidm_choice_check_timerid = 0;//choice check timer
#ifdef DM_SUPPORT_DFRESH
LOCAL  uint8 s_mmidm_socket_timerid = 0;
#endif  

LOCAL  uint8 s_mmidm_broken_session_timerid = 0; //broke current session timer  /*+++ cr251729*/
LOCAL  MMIDM_DOWNLOAD_PROCESS_T s_download_process = {0};
LOCAL  MMIDM_DEBUG_TYPE_E  s_dm_type = DM_NONE_SET;
LOCAL  DM_DISPLAY_INFO_T   s_display_info ={0}; /*lint !e64*/
LOCAL  BOOLEAN s_is_need_handle_alert_timerout_event = FALSE;
LOCAL  BOOLEAN s_is_dl_cancel = FALSE;

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
#ifdef DM_SUPPORT_DFRESH
extern PUBLIC void MMIDM_NotifyAlertResult(BOOL iscontinue);
#endif
/*****************************************************************************/
//  Description : handle debug setting menu win msg
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDmDebugSettingMenuWinMsg(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );
/*****************************************************************************/
//  Description : DmParaSetWinHandleMsg
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DmParaSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : DmSelectServerWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DmSelectServerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmSIMSelectWinMsg(MMI_WIN_ID_T win_id,  MMI_MESSAGE_ID_E msg_id, DPARAM param);


#ifndef DM_ONLY_SELFREG_SUPPORT
/*****************************************************************************/
// 	Description : to handle dm main menu win message
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmMainMenuWinMsg( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );


/*****************************************************************************/
// 	Description : play ring for receiving a message from server
//	Global resource dependence : 
//  Author:yi.su
//	Note: 
/*****************************************************************************/
LOCAL void PlayDmNotifyRing(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : handle dm alert handle display 
//	Global resource dependence : 
//  Author:yi.su
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmAlertDisplayWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );

/*****************************************************************************/
// 	Description : to handle message in dm/dl alert confirm handle
//	Global resource dependence : 
//  Author:yi.su
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmAlertConfirmWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );

/*****************************************************************************/
// 	Description : to show software version info 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void ShowSoftwareVersionInfo(void);

/*****************************************************************************/
// 	Description : to handle message in download process from dm and user 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDMShowVersionWin( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );
/*****************************************************************************/
//  Description : show version info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void ShowVersionInfo(void);

/*****************************************************************************/
// 	Description : to update software
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/              
LOCAL void SoftWareUpdate(void);

/*****************************************************************************/
// 	Description : to handle dm install query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmInstallQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );

/*****************************************************************************/
// 	Description : to handle dm install succ win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmFumoUpdateSuccWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );
/*****************************************************************************/
//  Description : open wait win
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void FmmOpenWaitWin(MMI_TEXT_ID_T label);

/*****************************************************************************/
//  Description : wait window message         
//  Global resource dependence : 
//  Author: jian.ma
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

/*****************************************************************************/
//  Description : update dm waiting win info 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateDmWaitingWinInfo(MMI_TEXT_ID_T label);

/*****************************************************************************/
//  Description : handle dm session change state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleDmSessionChange(MMIDM_SESSION_CHANGE_T *data);

/*****************************************************************************/
//  Description : handle dl session change state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleDlSessionChange(MMIDM_SESSION_CHANGE_T *data);

/*****************************************************************************/
// 	Description : to handle DM server lock message 
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDMServerLockWinMsg( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
//	Description : dm handle user select option
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void MMIDM_HandleUserSelectOption(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : handle alert pl display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertPlDisplay(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : handle alert pl display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertPlConfirm(MMI_MESSAGE_ID_E msg_id);
/*****************************************************************************/
//  Description : handle alert nia display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertNIADisplay(void);
/*****************************************************************************/
//  Description : handle alert nia display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertNIAConfirm(MMI_MESSAGE_ID_E msg_id);
/*****************************************************************************/
//  Description : handle Fumo confirm alert
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertFUMOConfirm(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : handle alert scomo alert
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertScomoConfirm(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : to handle install update package
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void InstallUpdatePackage(void);

/*****************************************************************************/
// 	Description : to handle dm resume query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmResumeQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );
/*****************************************************************************/
// 	Description : to handle restart dm resume query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReDmResumeQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );
/*****************************************************************************/
// 	Description : DmSelectDelayTimeWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DmSelectDelayTimeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : open lock win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void OpenLockWin(void);


/*****************************************************************************/
// 	Description : to update software query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/ 
LOCAL void OpenSoftwareUpdateQueryWin(void);

/*****************************************************************************/
// 	Description : update software query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleDmSWQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     );

/*****************************************************************************/
// 	Description : open resume query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void MMIDM_OpenResumeQueryWin(MMIPUB_HANDLE_FUNC  handle_func);

/*****************************************************************************/
// 	Description : to show display info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void ShowDisplayInfo( MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : to create display confim check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CreateDispalyConfimCheckTimer(uint32 timer);

/*****************************************************************************/
// 	Description : to create choice list check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CreateChoiceListCheckTimer(uint32 timer);

/*****************************************************************************/
// 	Description : to close choice list check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CloseChoiceListCheckTimer(void);

/*****************************************************************************/
// 	Description : to close display confim check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CloseDisplayConfimCheckTimer(void);

/*****************************************************************************/
//  Description :handle event close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void MMIDM_HandDmEventClose(void);

/*****************************************************************************/
//  Description : get dial button rect
//  Global resource dependence : 
//  Author: xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDialButtonRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : creat dial button in call process
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void CreateDialButton(MMI_WIN_ID_T win_id );

/*****************************************************************************/
// 	Description : dm open UI ChoiceList Query win message
//	Global resource dependence : 
//  Author:jian.ma
/*****************************************************************************/
LOCAL void OpenDlChoiceListQueryWin(MMIDM_ALERT_TYPE_E mode);
/*****************************************************************************/
// 	Description : to handle ChiceList Query win message
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChiceListQueryWinMsg( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );
/*****************************************************************************/
// 	Description : add text item to list 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void AppendTextListItem(
                                MMI_CTRL_ID_T       ctrl_id,
                                const char*         string_ptr,
                                uint16              string_len,
                                GUIITEM_STYLE_E      item_style
                                );
#endif /*DM_ONLY_SELFREG_SUPPORT*/

/**--------------------------------------------------------------------------*
**                          WINDOW TABLE DECLARE                             *
**---------------------------------------------------------------------------*/
WINDOW_TABLE(MMIDM_DEBUG_SETTING_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE(TXT_DM_SETTING),
	WIN_FUNC((uint32)HandleDmDebugSettingMenuWinMsg),    
	WIN_ID(MMIDM_DEBUG_SETTING_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MENU_DM_PARAM_SET, MMIDM_DEBUG_SETTING_MENU_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIDM_PRAR_SET_WIN_TAB) = 
{
	WIN_FUNC((uint32)DmParaSetWinHandleMsg),
	WIN_ID(MMIDM_SET_WIN_ID),
	WIN_TITLE(TXT_DM_SETTING),
	CREATE_EDIT_TEXT_CTRL(DM_MAX_ID_STR,MMIDM_DEBUG_EDIT_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

WINDOW_TABLE(MMIDM_SELECT_TEST_SERVER_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)DmSelectServerWinHandleMsg),
	WIN_ID(MMIDM_SELECT_TEST_SERVER_WIN_ID),
	WIN_TITLE(TXT_DM_SETTING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDM_DEBUG_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE( MMIDM_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleDmSIMSelectWinMsg),    
    WIN_ID(MMIDM_SELECT_SIM_WIN_ID),
    WIN_TITLE(TXT_SIM_SEL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIDM_SELECT_SIM_CTRL_ID),
    END_WIN
};

#ifndef DM_ONLY_SELFREG_SUPPORT
//main menu win
WINDOW_TABLE(MMIDM_MAIN_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDmMainMenuWinMsg),    
    WIN_ID(MMIDM_MAIN_MENU_WIN_ID),
    WIN_TITLE(TXT_DM_SETTING),
    CREATE_MENU_CTRL(MENU_DM_UPDATE_MENU, MMIDM_MAIN_MENU_MENU_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//show version win
WINDOW_TABLE(MMIDM_SHOWVERSION_WIN_TAB) = 
{
	WIN_FUNC((uint32)HandleDMShowVersionWin),
	WIN_ID(MMIDM_SHOWVERSION_WIN_ID),
	WIN_TITLE(TXT_DM_SOFTWARE_VERSION),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(MMIDM_SHOW_VERSION_CTRL_ID),
    END_WIN
};

//DM lock win
WINDOW_TABLE(MMIDM_SERVER_LOCK_WINTAB) =
{
    WIN_FUNC((uint32)HandleDMServerLockWinMsg),
    WIN_ID(MMIDM_SERVER_LOCK_WIN_ID),
    WIN_TITLE(TXT_DM_HANDSET_LOCK),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    WIN_STYLE( WS_DISABLE_RETURN_WIN ),
    END_WIN
};



WINDOW_TABLE(MMIDM_SELECT_DELAYTIMER_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)DmSelectDelayTimeWinHandleMsg),
	WIN_ID(MMIDM_SELECT_DELAYTIMER_WIN_ID),
	WIN_TITLE(TXT_DM_DELAY_TIME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDM_DELAY_LISTBOX_CTRL_ID),
	END_WIN
};


WINDOW_TABLE(MMIDM_ALERT_DISPLAY_WIN_TAB) = 
{
	WIN_FUNC((uint32)HandleDmAlertDisplayWin),
	WIN_ID(MMIDM_ALERT_DISPLAY_WIN_ID),
	WIN_TITLE(TXT_DM_PUSH),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),	
    CREATE_TEXT_CTRL(MMIDM_DISPLAY_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIDM_ALERT_CONFIRM_WIN_TAB) = 
{
	WIN_FUNC((uint32)HandleDmAlertConfirmWin),
	WIN_ID(MMIDM_ALERT_CONFIRM_WIN_ID),
	WIN_TITLE(TXT_DM_PUSH),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),	
    CREATE_TEXT_CTRL(MMIDM_ALERT_CONFIRM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIDM_CHOICELIST_QUERY_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32) HandleChiceListQueryWinMsg ),    
        WIN_ID( MMIDM_CHOICELIST_QUERY_WIN_ID),
        WIN_TITLE(TXT_COMMUNICATION_MESSAGE),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDM_CHOICELIST_EDITBOX_CTRL_ID),
        END_WIN
};
#endif /*DM_ONLY_SELFREG_SUPPORT*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open dm debug win
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_OpenDebugMenuWin(void)
{
    MMK_CreateWin((uint32*)MMIDM_DEBUG_SETTING_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : handle debug setting menu win msg
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDmDebugSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    //SCI_TRACE_LOW:"MMIDM==> enter HandleDmDebugSettingMenuWinMsg "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_624_112_2_18_2_14_28_850,(uint8*)"");

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIDM_DEBUG_SETTING_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIDM_DEBUG_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
            case ID_DM_APN_SET:
                s_dm_type = DM_APN_SET;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_MON_SET:
                s_dm_type = DM_MON_SET;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_DEV_VERSION_SET:
                s_dm_type =DM_DEV_VERSION_SET;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_MAN_SET:
                s_dm_type = DM_MAN_SET;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_IMEI_SET:
                s_dm_type =DM_IMEI_SET;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_SRV_ADDR_SET:
                s_dm_type =DM_SRV_ADDR;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_SELF_REG_NUMBER:
                s_dm_type =DM_SELF_REG_NUM;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_SELF_REGPORT_NUMBER:
                s_dm_type =DM_SELF_REGPORT_NUM;
                MMK_CreateWin((uint32*)MMIDM_PRAR_SET_WIN_TAB, PNULL);
                break;
            case ID_DM_CNNECT_SERVER:
                if (MMIDM_IsDebugMode())
                {
                    MMIDM_SetIsSrReady(DM_SELF_REG_SUCC);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_SMS_REG,STXT_FINISH,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                }
                break;
            case ID_DM_DEBUG_MODE:
                if (MMIDM_IsDebugMode())
                {
                    MMIDM_SetDebugMode(FALSE);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMM_DEBUG_MODE,TXT_COMM_CLOSE,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                }
                else
                {
                    MMIDM_SetDebugMode(TRUE);
                    MMIDM_SetDmRegSwitchState(TRUE);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMM_DEBUG_MODE,TXT_OPEN_COMN,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                }
                break;
            case ID_DM_SMS_REG:
                MMIDM_SetNetReady(TRUE);
                MMIDM_SetSmsReady(TRUE);
                if(MMIDM_SendSelfRegisterSMS(MMIDM_GetMainSim()))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_SMS_REG,TXT_DM_SEND,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_SMS_REG,TXT_DM_SEND,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                }
                break;
            case ID_DM_TEST_SERVER:
                MMK_CreateWin((uint32*)MMIDM_SELECT_TEST_SERVER_WIN_TAB, PNULL);
                break;
            case ID_DM_SWITCH:
                if (MMIDM_GetDmRefSwitchState())
                {
                    MMIDM_SetDmRegSwitchState(FALSE);
                    MMIPUB_OpenAlertSuccessWin(TXT_COMM_CLOSE); 
                }
                else
                {
                    MMIDM_SetDmRegSwitchState(TRUE);
                    MMIPUB_OpenAlertSuccessWin(TXT_OPEN_COMN); 
                }
                break;
            #ifndef DM_ONLY_SELFREG_SUPPORT
                case ID_DM_LOG:
                    if (MMIDM_GetLogState())
                    {
                        MMIDM_SetLogState(FALSE);
                        MMIPUB_OpenAlertSuccessWin(TXT_COMM_CLOSE); 
                    }
                    else
                    {
                        MMIDM_SetLogState(TRUE);
                        MMIPUB_OpenAlertSuccessWin(TXT_OPEN_COMN); 
                    }
                    break;
//            case ID_DM_NULL_SESSION:
//                if (!MMIDM_IsDmRun())
//                {
//                    FmmOpenWaitWin(TXT_COMMON_SEARCHING);
//                    MMIDM_StartDm(DM_SESSION_USER,PNULL,0);
//                }
//                else
//                {
//                    MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
//                    MMIDM_DeleteAllResumeFile();
//                }
//                break;
//            case ID_DM_LOCK:
//                if (MMIDM_GetHandsetLock())
//                {
//                    MMIDM_CloseLockWin();
//                    MMIDM_SetDebugMode(FALSE);
//                }
//                else
//                {
//                    MMIDM_SetDebugMode(TRUE);
//                    OpenLockWin();
//                }
//                break;
            case ID_DM_FOTA:
                 MMIDM_FOTA_ImageArea_Open();
                 MMIDM_FOTA_SetUpdateFlag(DM_FOTA_OP_CLEAR_FLAG);
                 MMIDM_FOTA_ImageArea_Erase();
                 MMIDM_FOTA_ImageArea_Close();
                 if(MMIDM_InstallUpdatePackage())
                 {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_INSTALL_SUCC,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleDmFumoUpdateSuccWin);
                 }
                 else
                 {
                    MMIPUB_OpenAlertFailWin(TXT_UPDATE_FAIL);
                 }
				 break;
//                if (MMIDM_GetFotaState())
//                {
//                    MMIDM_SetFotaState(FALSE);
//                    MMIPUB_OpenAlertSuccessWin(TXT_COMM_CLOSE); 
//                }
//                else
//                {
//                    MMIDM_SetFotaState(TRUE);
//                    MMIPUB_OpenAlertSuccessWin(TXT_OPEN_COMN); 
//                }
//                break;
//            case ID_DM_TREE:
//                if (MMIDM_GetTreeState())
//                {
//                    MMIDM_SetTreeState(FALSE);
//                    MMIPUB_OpenAlertSuccessWin(TXT_COMM_CLOSE); 
//                }
//                else
//                {
//                    MMIDM_SetTreeState(TRUE);
//                    MMIPUB_OpenAlertSuccessWin(TXT_OPEN_COMN); 
//                }
//                break;
            case ID_DM_RESET:
                 MMIDM_DelDmFolderFiles();
                 MMIDM_CleanResumeNv();
                 MMIPUB_OpenAlertSuccessWin(STXT_FINISH); 
                 break;

            case ID_DM_SCOMO_INSTALL:
                 if(MMIDM_IsScomoFileExist())
                 {
                    MMIDM_ScomoInstall(FALSE);
                 }
                 else
                 {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_SCOMO_FILE_ABSENT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                 }
                 break;
            #endif /*DM_ONLY_SELFREG_SUPPORT*/

#ifndef  MMI_MULTI_SIM_SYS_SINGLE
            case ID_DM_MAIN_SIM:
            {
                MMI_TEXT_ID_T sim_name_id[MMI_DUAL_SYS_MAX] = {
                                    TXT_SIM_SEL_SIM1,
                                    TXT_SIM_SEL_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
                                    TXT_SIM_SEL_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
                                    TXT_SIM_SEL_SIM4,
#endif
#endif
                                    };
                
                MMIPUB_OpenAlertWinByTextId(PNULL,
                                                    sim_name_id[MMIDM_GetMainSim()],
                                                    TXT_NULL,
                                                    IMAGE_PUBWIN_SUCCESS,
                                                    PNULL,
                                                    PNULL,
                                                    MMIPUB_SOFTKEY_ONE,
                                                    PNULL);              
            }

                break;
#endif            
	        default:
		    break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(win_id);
		break;
			
	default:
		result = MMI_RESULT_FALSE;
		break;
    }
    
    return result;
}

MMI_RESULT_E HandleDmSIMSelectWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              cur_selection = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_MAX;
    MMI_CTRL_ID_T       ctrl_id = MMIDM_SELECT_SIM_CTRL_ID;
    uint32 item_data = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE );//max item 2
        MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SIM_ALL);
        GUILIST_SetCurItemIndex(ctrl_id, MMIAPIDM_GetMainSim());
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:    
    case MSG_APP_CALL4:     
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        dual_sys = MMIAPIDM_GetMainSim();
        if(cur_selection == dual_sys)
        {
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
        }
        else
        {
            #ifndef DM_ONLY_SELFREG_SUPPORT
            if(MMIDM_IsDmRun())
            {
                MMIPUB_OpenAlertSuccessWin(TXT_DM_SIMSEL_DMRUN_TIP);
               // MMK_CloseWin(win_id);
                 GUILIST_SetCurItemIndex(ctrl_id, MMIAPIDM_GetMainSim());
            }
            else
            #endif
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DM_SIMSEL_RESET_TIP,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
        }

        break;
    case MSG_PROMPTWIN_OK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        GUILIST_GetItemData(ctrl_id, cur_selection, &item_data);

        if(item_data < MMI_DUAL_SYS_MAX)
        {
            dual_sys = (MN_DUAL_SYS_E)item_data;
            
        }
        else
        {
            //SCI_TRACE_LOW:"HandleDmSIMSelectWinMsg,item_data[%d] >= MMI_DUAL_SYS_MAX,ERROR!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_886_112_2_18_2_14_28_851,(uint8*)"d",item_data);
        }
        MMIAPIDM_SetMainSim(dual_sys);
        MMIDM_CleanDmData();



    	MMIAPIPHONE_PowerReset();
    	break;
    	
   case MSG_PROMPTWIN_CANCEL:
	   	MMK_CloseWin(win_id);
	    //关闭询问窗口
	    MMIPUB_CloseQuerytWin(PNULL);
    break; 	
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}


/*****************************************************************************/
// 	Description : DmParaSetWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DmParaSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_CTRL_ID_T		editbox_ctrl_id = MMIDM_DEBUG_EDIT_CTRL_ID;
	MMI_RESULT_E		result = MMI_RESULT_TRUE;
	MMI_STRING_T        string_info = {0};
    char                str[MMIDM_DEBUG_MAX_INPUT_LEN] = {0};
    wchar               wstr[MMIDM_DEBUG_MAX_INPUT_LEN] = {0};
    uint32              str_len = 0;
    MMI_TEXT_ID_T       text_id = TXT_NULL;
    uint32              text_max_len = MMIDM_DEBUG_MAX_INPUT_LEN;
    GUIIM_TYPE_T        allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;/*lint !e655*/
    GUIIM_TYPE_T        default_im = GUIIM_TYPE_ENGLISH;
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
        switch(s_dm_type) 
        {
            case DM_APN_SET:
        	    text_id = TXT_DM_APN_SET;
                text_max_len = MMIDM_APN_MAX_LEN-1;
                allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;/*lint !e655*/
                break;
            case DM_MON_SET:
        	    text_id = TXT_DM_MON_SET;
                text_max_len = DM_MAX_ID_STR-1;
                allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;/*lint !e655*/
                break;
            case DM_DEV_VERSION_SET:
        	    text_id = TXT_DM_DEV_VERSION_SET;
                text_max_len = DM_MAX_ID_STR-1;
                allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;/*lint !e655*/
                break;
            case DM_MAN_SET:
                text_id = TXT_DM_MAN_SET;
                text_max_len = DM_MAX_ID_STR-1;
                allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;/*lint !e655*/
        	    break;
            case DM_IMEI_SET:
                text_id = TXT_PHONE_IMEI;
                text_max_len = DM_MAX_ID_STR-6;
                allow_im = GUIIM_TYPE_DIGITAL;/*lint !e655*/
                default_im = GUIIM_TYPE_DIGITAL;/*lint !e655*/
        	    break;
            case DM_SRV_ADDR:
                text_id = TXT_DM_SRV_ADDR;
                text_max_len = MMIDM_DEBUG_MAX_INPUT_LEN-1;
                allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;/*lint !e655*/
                break;
            case DM_SELF_REG_NUM:
                text_id = TXT_DM_SELF_REG_NUMBER;
                text_max_len = MMIDM_SRNUM_MAX_LEN-1;
                allow_im = GUIIM_TYPE_DIGITAL;/*lint !e655*/
                default_im = GUIIM_TYPE_DIGITAL;/*lint !e655*/
                break;
            case DM_SELF_REGPORT_NUM:  
                text_id = TXT_DM_SELF_REGPORT_NUMBER;
                text_max_len = MMIDM_SRPORT_MAX_LEN-1;
                allow_im = GUIIM_TYPE_DIGITAL;/*lint !e655*/
                default_im = GUIIM_TYPE_DIGITAL;/*lint !e655*/
                break;
            default:
                text_id = TXT_DM_SETTING;
                text_max_len= MMIDM_DEBUG_MAX_INPUT_LEN;
                break;
        }
        GUIEDIT_SetTextMaxLen(editbox_ctrl_id,text_max_len/sizeof(wchar), text_max_len);
        GUIWIN_SetTitleTextId(win_id,text_id,FALSE); 
		//set im
        GUIEDIT_SetIm(editbox_ctrl_id, (GUIIM_TYPE_T)allow_im, default_im);
        MMIDM_GetDmParaInfo(s_dm_type,str);
        str_len = strlen(str);
        MMI_STRNTOWSTR( wstr, MMIDM_DEBUG_MAX_INPUT_LEN, (uint8*)str, str_len, str_len );

        GUIEDIT_SetString(editbox_ctrl_id,wstr,str_len);
		MMK_SetAtvCtrl(win_id,editbox_ctrl_id);
		break;
    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
		//save to nv
		SCI_MEMSET(&string_info,0,sizeof(MMI_STRING_T));
		GUIEDIT_GetString(editbox_ctrl_id,&string_info);

        MMI_WSTRNTOSTR((uint8*)str, MMIDM_DEBUG_MAX_INPUT_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len );

        MMIDM_SetDmParaInfo(s_dm_type,str,strlen(str));

		MMK_CloseWin(win_id);
		break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
// 	Description : DmSelectServerWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DmSelectServerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIDM_DEBUG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MN_IMSI_T imsi = {0};
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMIDM_SELECT_SERVER_ITEM, FALSE );//max item 2

        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_SPREADTRUM,TXT_OPEN_COMN,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_LG,TXT_COMM_CLOSE,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_NEWPOST,TXT_COMM_CLOSE,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  

        cur_selection = (MMIDM_TEST_SERVER_E)MMIDM_GetTestServerType();

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;	
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        MMIDM_SetTestServerType((MMIDM_TEST_SERVER_E)cur_selection);
        MMINV_WRITE(MMINV_DM_IMSI_INFO, &imsi);
        //MMIPUB_OpenAlertSuccessWin(STXT_FINISH );
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
PUBLIC void MMIDM_OpenDmSelSimWin(void)
{
    MMK_CreateWin((uint32 *)MMIDM_SELECT_SIM_WIN_TAB, PNULL);
}

 #ifndef DM_ONLY_SELFREG_SUPPORT
/*****************************************************************************/
// 	Description : open dm main menu win
//	Global resource dependence : 
//  Author:jian.ma
/*****************************************************************************/
PUBLIC void MMIDM_OpenDmMainMenu(void)
{
    MMK_CreateWin((uint32 *)MMIDM_MAIN_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : to handle dm main menu win message
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmMainMenuWinMsg( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
    MMI_MENU_ID_T	                    menu_id = 0;
    MMI_MENU_GROUP_ID_T	                group_id = 0;
    MMI_CTRL_ID_T                       ctrl_id = MMIDM_MAIN_MENU_MENU_CTRL_ID;
    
    //SCI_TRACE_LOW:"MMIDM==>  HandleDmMainMenuWinMsg: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1109_112_2_18_2_14_29_852,(uint8*)"d",msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch(menu_id) 
        {
            case ID_DM_SOFTWARE_VERSION:
                ShowSoftwareVersionInfo();
        	    break;
            case ID_DM_SOFTWARE_UPDATE:
                SoftWareUpdate();//OpenSoftwareUpdateQueryWin();
        	    break;
            case ID_DM_UPDATE_PACKAGE:
                InstallUpdatePackage();
        	    break;
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)               
           case ID_DM_SELECT_SIM:
           		MMIDM_OpenDmSelSimWin();
           		break;
#endif                    
            default:
                break;
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : dm open alert message
//	Global resource dependence : 
//  Author:jian.ma
/*****************************************************************************/
PUBLIC void MMIDM_OpenDmSessionAlertWin(
                                       MMIDM_ALERT_TYPE_E mode, 
                                       const uint8* text,
                                       uint16 len,
                                       uint32 max_dis_timer
                                       )
{

    //SCI_TRACE_LOW:"MMIDM==>  OpenDlAlerthandleWin  mode =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1162_112_2_18_2_14_29_853,(uint8*)"d",mode);
    
    MMIDM_CloseWaitingWin();
    
    if(MMK_IsOpenWin(MMIDM_ALERT_DISPLAY_WIN_ID))
    {
        MMK_CloseWin( MMIDM_ALERT_DISPLAY_WIN_ID );
    }
    if(MMK_IsOpenWin(MMIDM_ALERT_CONFIRM_WIN_ID))
    {
        MMK_CloseWin( MMIDM_ALERT_CONFIRM_WIN_ID );
    }
    
    SCI_MEMSET(&s_display_info,0,sizeof(s_display_info));
    
    s_display_info.mode = mode;
    
    if (PNULL!=text && len!=0)
    {
        GUI_UTF8ToWstr(s_display_info.buffer,len,text,len);
        s_display_info.len = len;
    }

    if (MMIDM_ALERT_SCOMO_CONFIRM ==mode )
    {
        if (MMIDM_GetHandsetLock())
        {
            return;
        }
    }
    PlayDmNotifyRing(MMIDM_GetMainSim());
    MMIDEFAULT_SetBackLight(TRUE);
    switch(mode) 
    {
        case MMIDM_ALERT_PL_DISPLAY:
             CreateDispalyConfimCheckTimer(max_dis_timer);
            if (!MMIDM_IsNeedDelayDMSession())
            {
                MMK_CreateWin((uint32 *)MMIDM_ALERT_DISPLAY_WIN_TAB, PNULL);
            }
            else
            {
                MMIDM_SetIsNeedRestart(TRUE);
                MMIDM_StopDmRunCheckTimer();
            }
             break;
        case MMIDM_ALERT_NIA_DISPLAY:
             MMK_CreateWin((uint32 *)MMIDM_ALERT_DISPLAY_WIN_TAB, PNULL);
    	     break;
        case MMIDM_ALERT_PL_CONFIRM:
        case MMIDM_ALERT_NIA_CONFIRM:
        case MMIDM_ALERT_FUMO_CONFIRM:
        case MMIDM_ALERT_SCOMO_CONFIRM:
            CreateDispalyConfimCheckTimer(max_dis_timer);
            if (!MMIDM_IsNeedDelayDMSession() && !MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))//maryxiao cr MS00245175
            {
#if defined(DM_SUPPORT_DFRESH)
                MMIDM_SetIsNeedReplay(TRUE);
#endif
                MMK_CreateWin((uint32 *)MMIDM_ALERT_CONFIRM_WIN_TAB, PNULL);
            }
            else
            {
                MMIDM_SetIsNeedRestart(TRUE);
                MMIDM_StopDmRunCheckTimer();
            }
            break;
        case MMIDM_ALERT_FUMO_UPDATE:
            if (!MMIDM_IsNeedDelayDMSession() && !MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))//maryxiao cr MS00245175
            {
                MMK_CreateWin((uint32 *)MMIDM_ALERT_CONFIRM_WIN_TAB, PNULL);
            }
            else
            {
                MMIDM_SetIsNeedRestart(TRUE);
                MMIDM_StopDmRunCheckTimer();
            }
            break;
        case MMIDM_ALERT_SINGLE_CHOICE:
        case MMIDM_ALERT_MULTI_CHOICE:
            CreateChoiceListCheckTimer(max_dis_timer);
            if (!MMIDM_IsNeedDelayDMSession())
            {
                OpenDlChoiceListQueryWin(mode);
            }
            else
            {
                MMIDM_SetIsNeedRestart(TRUE);
                MMIDM_StopDmRunCheckTimer();
            }
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Jiaoyou.wu
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsInConform(void)
{
    BOOLEAN ret = FALSE;

    if(MMK_IsOpenWin(MMIDM_ALERT_DISPLAY_WIN_ID) || MMK_IsOpenWin(MMIDM_ALERT_CONFIRM_WIN_ID))
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIDM] MMIDM_IsInConform ret = %d", ret); 
    return ret;
}

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandWapClose(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandWapClose"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1261_112_2_18_2_14_29_854,(uint8*)"");
    MMIDM_HandDmEventClose();
}

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandDmCallClose(void)
{
    MMIDM_HandDmEventClose();
}
/*****************************************************************************/
//  Description :handle event close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void MMIDM_HandDmEventClose(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandDmEventClose"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1279_112_2_18_2_14_29_855,(uint8*)"");
    if (MMIDM_GetIsNeedRestart())
    {
        if (0 != MMIDM_GetDelayTimerId())
        {
            MMIDM_SetNeedHandleDelayEvent(FALSE);
            if(MMIDM_ReStartDmSession())
            {
                MMIDM_CloseDelayTimer();
            }
            else
            {
                MMIDM_SetNeedHandleDelayEvent(TRUE);
                return;
            }
        }
        else if (0 != s_mmidm_dis_confim_timerid)
        {
            if(MMIDM_ALERT_PL_DISPLAY == s_display_info.mode)
            {
                MMK_CreateWin((uint32 *)MMIDM_ALERT_DISPLAY_WIN_TAB, PNULL);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMIDM_ALERT_CONFIRM_WIN_TAB, PNULL);
            }
        }
        else if (s_is_need_handle_alert_timerout_event)
        {
            if (MMIDM_ALERT_PL_DISPLAY == s_display_info.mode)
            {
                MMIDM_HandleUserSelectOption(MMIDM_ALERT_DISPLAY_WIN_ID,MSG_TIMER);
                MMK_CloseWin( MMIDM_ALERT_DISPLAY_WIN_ID );
            }
            else
            {
                MMIDM_HandleUserSelectOption(MMIDM_ALERT_CONFIRM_WIN_ID,MSG_TIMER);
                MMK_CloseWin( MMIDM_ALERT_CONFIRM_WIN_ID );
            }
            s_is_need_handle_alert_timerout_event = FALSE;
        }
        else if (MMIDM_ALERT_FUMO_UPDATE == s_display_info.mode)
        {
            MMK_CreateWin((uint32 *)MMIDM_ALERT_CONFIRM_WIN_TAB, PNULL);
        }
        MMIDM_SetIsNeedRestart(FALSE);
    }
    else if (s_is_need_handle_alert_timerout_event)
    {
        SCI_TRACE_LOW("MMIDM==> MMIDM_HandDmEventClose confirm=%d display=%d", MMK_IsOpenWin(MMIDM_ALERT_CONFIRM_WIN_ID), MMK_IsOpenWin(MMIDM_ALERT_DISPLAY_WIN_ID));
        s_is_need_handle_alert_timerout_event = FALSE;
        if(MMK_IsOpenWin(MMIDM_ALERT_CONFIRM_WIN_ID))
        {
            MMK_CloseWin( MMIDM_ALERT_CONFIRM_WIN_ID );
        }
        else if(MMK_IsOpenWin(MMIDM_ALERT_DISPLAY_WIN_ID))
        {
            MMK_CloseWin( MMIDM_ALERT_DISPLAY_WIN_ID );
        }
        if (MMIDM_IsDmRun())
        {
            MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
        }
    }
}

/*****************************************************************************/
// 	Description : play ring for receiving a message from server
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void PlayDmNotifyRing(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIDM==>  PlayDmNotifyRing  dual_sys =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1349_112_2_18_2_14_29_856,(uint8*)"d",dual_sys);
    MMIAPISMS_PlayNewSMSRing(dual_sys);
}

/*****************************************************************************/
// 	Description : to show software version info 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void ShowSoftwareVersionInfo(void)
{
    MMK_CreateWin((uint32 *)MMIDM_SHOWVERSION_WIN_TAB, PNULL);	
}
  

/*****************************************************************************/
// 	Description : to handle message in download process from dm and user 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDMShowVersionWin( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
	MMI_RESULT_E            result = MMI_RESULT_TRUE;
    //MMIDM_DOWNLOAD_PROCESS_T data;
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
        ShowVersionInfo();	
        MMK_SetAtvCtrl(win_id, MMIDM_SHOW_VERSION_CTRL_ID);
        break;

	case MSG_CTL_CANCEL:
         MMK_CloseWin(win_id);
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Description : show version info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void ShowVersionInfo(void)
{
    wchar           unicode_cr = 0x0d;
    MMI_STRING_T    cr_s = {0};
    MMI_STRING_T    text_s = {0};
    uint16          uint16_str[250] = {0};
    uint16          uint16_str_len = 0;
    char*           temp_ptr = PNULL;
    uint16          temp_len = 0;
    char            temp[10] = {0};

    //SCI_TRACE_LOW:"MMIDM==>  ShowVersionInfo "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1407_112_2_18_2_14_29_857,(uint8*)"");
    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;

//Software V:  
    temp_ptr = "Software V:";
    temp_len = (uint16)SCI_STRLEN((char*)temp_ptr);       
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    temp_ptr = MMIDM_GetSoftwareVersion();
    temp_len = (uint16)SCI_STRLEN((char*)temp_ptr);
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行         

    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//date :
    temp_ptr = "Build time:";
    temp_len =(uint16)SCI_STRLEN((char*)temp_ptr);       
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

    temp_ptr = PROJECT_BUILD_TIME;
    temp_len = (uint16)SCI_STRLEN((char*)temp_ptr); 
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );  
    MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

//reg times
    if (MMIDM_IsDebugMode())
    {
        temp_ptr = "reg times:";
        temp_len =(uint16)SCI_STRLEN((char*)temp_ptr);       
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

        temp_ptr = temp;
        sprintf(temp_ptr,"%d",MMIDM_GetRegTimes());
        temp_len = (uint16)SCI_STRLEN((char*)temp_ptr); 
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );  
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

        temp_ptr = "reg state";
        temp_len =(uint16)SCI_STRLEN((char*)temp_ptr);       
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

        SCI_MEMSET(temp,0,10);
        temp_ptr = temp;
        sprintf(temp_ptr,"%d",MMIDM_GetIsSrReady());
        temp_len = (uint16)SCI_STRLEN((char*)temp_ptr); 
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );  
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行

        temp_ptr = "Dm state:";
        temp_len =(uint16)SCI_STRLEN((char*)temp_ptr);       
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
        
        SCI_MEMSET(temp,0,10);
        temp_ptr = temp;
        sprintf(temp_ptr,"%d",MMIDM_IsDmRun());
        temp_len = (uint16)SCI_STRLEN((char*)temp_ptr); 
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (const uint8*)temp_ptr, temp_len );  
        MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
    }
    text_s.wstr_ptr = uint16_str;
    text_s.wstr_len = uint16_str_len;
    SCI_ASSERT(uint16_str_len<250);/*assert verified*///memory overlay

    GUITEXT_SetString(MMIDM_SHOW_VERSION_CTRL_ID, text_s.wstr_ptr, text_s.wstr_len , FALSE);
}

/*****************************************************************************/
// 	Description : to update software query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/ 
LOCAL void OpenSoftwareUpdateQueryWin(void)
{
    MMI_STRING_T text = {0};
    MMI_WIN_ID_T                    win_id = MMIDM_ALERTHANDLE_WIN_ID;
    MMI_GetLabelTextByLang(TXT_DM_SW_UPDATE_ALERT, &text);
    MMIPUB_OpenQueryWinByTextPtr(&text, PNULL, IMAGE_PUBWIN_QUERY, &win_id, HandleDmSWQueryWin);
}
/*****************************************************************************/
// 	Description : update software query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleDmSWQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==>  HandleDmSWQueryWin: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1504_112_2_18_2_14_30_858,(uint8*)"d",msg_id);

	switch (msg_id)
	{
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
            SoftWareUpdate();
		    MMK_CloseWin(win_id);    
		    break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;
        
	    default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
	        break;
	}
    return (result);
}
/*****************************************************************************/
// 	Description : to update software
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/              
LOCAL void SoftWareUpdate(void)
{
    MMIDM_PL_ALERT_TYPE  alert_type = DM_PL_ALERT_NONE;

    //SCI_TRACE_LOW:"MMIDM==> SoftWareUpdate "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1534_112_2_18_2_14_30_859,(uint8*)"");

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    if (!MMIAPIPHONE_GetSimExistedStatus(MMIDM_GetMainSim()))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);   	
        return;
    }
    if (!MMIDM_CheckIsDlSpaceEnouge())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMM_NO_SPACE);
        return;
    }
    if(MMIDM_IsDmRun())
    { 
        uint32  time = 2000; 
        MMIPUB_OpenAlertWinByTextId(&time,TXT_DM_SERVICE_RUN,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
        return;
    }

    if (DM_SELF_REG_SUCC==MMIDM_GetIsSrReady())
    {
	    alert_type = DM_PL_ALERT_1101;
	    MMINV_WRITE(MMINV_DM_ALERT_TYPE,&alert_type);
	    //SCI_TRACE_LOW:"MMIDM==> SoftWareUpdate alert_type = %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1563_112_2_18_2_14_30_860,(uint8*)"d",alert_type);

		
        if (!MMIDM_GetNVDownloadFinish())
        {
            FmmOpenWaitWin(TXT_COMMON_SEARCHING);
            if(RESUME_TYPE_FUMO==MMIDM_GetResumeType())
	        {
                MMIDM_OpenResumeQueryWin(HandleDmResumeQueryWin);
	        }
            else
            {
                MMIDM_StartDm(DM_SESSION_CLIENT,PNULL,0);
            }
        }
        else
        {
            if(MMIDM_IsResumeFileExist(RESUME_TYPE_FUMO_DATA))
	        {
                MMIDM_OpenDmSessionAlertWin(MMIDM_ALERT_FUMO_UPDATE,PNULL,0,0);
	        }
            else
            {
                MMIDM_SetNVDownloadFinish(FALSE);
                FmmOpenWaitWin(TXT_COMMON_SEARCHING);
                MMIDM_StartDm(DM_SESSION_CLIENT,PNULL,0);
            }
        }
        //SCI_TRACE_LOW:"MMIDM==> SoftWareUpdate start"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1591_112_2_18_2_14_30_861,(uint8*)"");
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_DM_SERVICE_UNREG);
    }
}

/*****************************************************************************/
// 	Description : to handle install update package
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void InstallUpdatePackage(void)
{
    MMI_WIN_ID_T win_id = MMIDM_ALERTHANDLE_WIN_ID;

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }

    if (MMIDM_CheckUpdateFile() && MMIDM_GetNVDownloadFinish())
    {
        MMI_STRING_T text = {0};
        MMI_GetLabelTextByLang(TXT_DM_DOWNLOAD_UPDATE, &text);
        MMIPUB_OpenQueryWinByTextPtr(&text, PNULL, IMAGE_PUBWIN_QUERY, &win_id, HandleDmInstallQueryWin);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_DM_UN_SEARCH_PACKAGE);
    }
    
}
/*****************************************************************************/
// 	Description : to handle dm install query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmInstallQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==>  HandleDmInstallQueryWin: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1638_112_2_18_2_14_30_862,(uint8*)"d",msg_id);
    
	switch (msg_id)
	{
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
            if(MMIDM_InstallUpdatePackage())
            {
                uint32 time_period = 5000;
                MMIPUB_OpenAlertWinByTextId(&time_period,TXT_DM_INSTALL_SUCC,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleDmFumoUpdateSuccWin);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_UPDATE_FAIL);
            }
		    MMK_CloseWin(win_id);    
		    break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CreateWin((uint32*)MMIDM_SELECT_DELAYTIMER_WIN_TAB, PNULL);
            MMK_CloseWin( win_id );
            break;
        
	    default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
	        break;
	}
    return (result);
}

/*****************************************************************************/
// 	Description : to handle dm install succ win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmFumoUpdateSuccWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==>  HandleDmFumoUpdateSuccWin: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1682_112_2_18_2_14_30_863,(uint8*)"d",msg_id);
    
	switch (msg_id)
	{
        case MSG_TIMER:
        case MSG_CLOSE_WINDOW:
            MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO_DATA);
            MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO);
            MMIAPIPHONE_PowerReset();
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
		    break;
	    default:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
	        break;
	}
    return (result);
}
/*****************************************************************************/
//  Description : open wait win
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void FmmOpenWaitWin(MMI_TEXT_ID_T label)
{
    MMI_STRING_T wait_text = {0};
    MMI_GetLabelTextByLang(label, &wait_text);
    MMIPUB_OpenWaitWin(1,
        &wait_text,
        PNULL,
        PNULL,
        MMIDM_WAITING_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_ONE,
        (MMIPUB_HANDLE_FUNC)HandleWaitWinMsg);
}

/*****************************************************************************/
//  Description : waiting win             
//  Global resource dependence : 
//  Author: jian.ma
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==> HandleWaitWinMsg, win_id =%d, msg_id = %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1731_112_2_18_2_14_30_864,(uint8*)"dd", win_id, msg_id);
    
    switch( msg_id)
    {
	case MSG_OPEN_WINDOW:
        s_mmidm_check_timerid = MMK_CreateWinTimer(win_id, MMIDM_STATE_CHECK_TIME, FALSE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
		break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIDM_SendSigToDmTask(DM_TASK_DM_CANCEL,MMIDM_GetDmTaskID(),PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_RED:
        MMIDM_SendSigToDmTask(DM_TASK_DM_CANCEL,MMIDM_GetDmTaskID(),PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_TIMER:
        if (s_mmidm_check_timerid == *(uint8*)param)
        {
            MMIPUB_OpenAlertWarningWin(TXT_DM_UN_SEARCH_PACKAGE);
            MMIDM_SendSigToDmTask(DM_TASK_DM_CANCEL,MMIDM_GetDmTaskID(),PNULL);
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_CLOSE_WINDOW:
        if ( 0 != s_mmidm_check_timerid)
        {
            MMK_StopTimer(s_mmidm_check_timerid);
            s_mmidm_check_timerid = 0;
        }
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : close waiting win             
//  Global resource dependence : 
//  Author: jian.ma
//  Note:   
/*****************************************************************************/
PUBLIC void MMIDM_CloseWaitingWin(void)
{
    MMK_CloseWin(MMIDM_WAITING_WIN_ID);
}

/*****************************************************************************/
//  Description : 处理进度条更新，在DM_TASK中将数据发给APP_TASK处理
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ProgressUpdateCallBack(MMIDM_DOWNLOAD_PROCESS_T *data)
{
    MMI_WIN_ID_T        progress_win_id = MMIDM_DLSESSION_PROGRESS_WIN_ID;
    MMIDM_CloseWaitingWin();

    SCI_MEMSET(&s_download_process,0x00,sizeof(MMIDM_DOWNLOAD_PROCESS_T));
    
    if (PNULL!=data 
        && !s_is_dl_cancel
        && !MMIAPICC_IsInState(CC_IN_CALL_STATE)
        && MMIDM_IsNeedDisplayProgresswin() //alert 1101 need display progress win
        )
    {
        s_download_process.inCurrentProgress = data->inCurrentProgress;
        s_download_process.inMaxProgress = data->inMaxProgress;
   
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ProgressUpdateCallBack inCurrentProgress=%d,inMaxProgress=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1801_112_2_18_2_14_30_865,(uint8*)"dd",s_download_process.inCurrentProgress,s_download_process.inMaxProgress);

        if (!MMIDM_IsNeedDelayDMSession())
        {
            if(!MMK_IsOpenWin(MMIDM_DLSESSION_PROGRESS_WIN_ID))
            {
    
                MMIPUB_OpenProgressWinByTextId(TXT_COMMON_DOWNLOADING_WAIT, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleProcessWinMsg);
                MMIPUB_SetProgressTotalSize(&progress_win_id, s_download_process.inMaxProgress);
            }
            else
            {
                MMIDM_CreateDmRunCheckTimer();
                MMIPUB_UpdateProgressBar(&progress_win_id, s_download_process.inCurrentProgress);
            }
        }
        else
        {
            MMIDM_CreateDelayTimer(0);
            MMIDM_StopDmRunCheckTimer();
            MMIDM_SetIsNeedRestart(TRUE);
            MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
        }
        SCI_FREE(data);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ProgressUpdateCallBack PNULL==data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1828_112_2_18_2_14_30_866,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_is_dl_cancel = FALSE;
        MMIDEFAULT_SetBackLight(TRUE);
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);	
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIDM_SendSigToDmTask(DM_TASK_DL_CANCEL,MMIDM_GetDmTaskID(),PNULL);
        s_is_dl_cancel = TRUE;
        MMK_CloseWin(win_id);
        break;
    case MSG_LOSE_FOCUS:
        if (MMIDM_IsNeedDelayDMSession())
        {
            MMIDM_CreateDelayTimer(0);
            MMIDM_StopDmRunCheckTimer();
            MMIDM_SetIsNeedRestart(TRUE);
            MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
            MMK_CloseWin(win_id);
        }
        else
        {
        	  result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        }
        break;
    case MSG_APP_RED:
        MMIDM_SendSigToDmTask(DM_TASK_DL_CANCEL,MMIDM_GetDmTaskID(),PNULL);
        s_is_dl_cancel = TRUE;
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIDM_SendSigToDmTask(DM_TASK_DL_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : handle server lock call back
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ServerLockCallBack(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ServerLockCallBack :LOCK"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1892_112_2_18_2_14_30_867,(uint8*)"");
    MMIDM_SetNVHandsetLock(TRUE);
    MMK_ReturnIdleWin();

    //挂断当前电话
//+CR126580    
 #ifdef VT_SUPPORT
 //-CR126580
    if(MMIAPIVT_IsVtCalling())
    {
        MMIAPIVT_ReleaseVTCallByRedkey();
    }
    else
 #endif
    if(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
    ||MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
    ||MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)
    )
    {
        MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
    }

    MMIAPIBT_OpenBlueTooth(FALSE);
    MMIAPIFM_Exit();
    MMIAPISET_StopAllRing(TRUE);
    OpenLockWin();
}
/*****************************************************************************/
//  Description : open lock win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void OpenLockWin(void)
{
    MMIAPISTK_CloseDisplayTextWin();//close stk for network flow
    MMK_ReturnIdleWin();
    if (!MMK_IsOpenWin(MMIDM_SERVER_LOCK_WIN_ID))
    {
        MMK_CreateWin((uint32 *)MMIDM_SERVER_LOCK_WINTAB, PNULL);
    }
}

/*****************************************************************************/
//  Description : deal with dm server unlock message
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ServerUnlockCallBack(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ServerUnlockCallBack: UNLOCK"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_1937_112_2_18_2_14_30_868,(uint8*)"");
    MMIDM_SetNVHandsetLock(FALSE);
    MMIDM_CloseLockWin();
}
/*****************************************************************************/
//  Description :close  lock win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CloseLockWin(void)
{
    if (MMK_IsOpenWin(MMIDM_SERVER_LOCK_WIN_ID))
    {
        MMK_CloseWin(MMIDM_SERVER_LOCK_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : get dial button rect
//  Global resource dependence : 
//  Author: xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDialButtonRect(MMI_WIN_ID_T win_id)
{
    uint16              width       = 0;
    uint16              height      = 0;
    GUI_RECT_T          rect        = CC_DIAL_BUTTON_RECT;
    MMI_IMAGE_ID_T      image_id    = IMAGE_CALL_DIAL;
    MMI_SOFTKEY_STYLE_T      softkey_style = {0};
    
    
    MMITHEME_GetSoftkeyStyle(&softkey_style);
    
    GUIRES_GetImgWidthHeight(&width,&height,image_id,win_id);
    
    rect.left  = softkey_style.second_icon_offset;
    rect.right = rect.left + width;
    
    return rect;
}

/*****************************************************************************/
//  Description : creat dial button in call process
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void CreateDialButton(MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T       ctrl_id = MMIDM_DIAL_BUTTON_CTRL_ID;
    GUI_RECT_T          rect = GetDialButtonRect( win_id );
    //MMI_IMAGE_ID_T      image_id = IMAGE_CALL_DIAL;
    GUIBUTTON_INIT_DATA_T  init_data = {0};
    
    //init
    init_data.both_rect.h_rect = init_data.both_rect.v_rect   = rect;
    init_data.bg.bg_type = GUI_BG_IMG;
    init_data.bg.img_id  = IMAGE_CALL_DIAL;

    //create
    GUIBUTTON_CreateDynamic(win_id,
        ctrl_id,
        &init_data);

}
/*****************************************************************************/
// 	Description : to handle DM server lock message 
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDMServerLockWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	    lcd_dev_info = {0};
    //GUI_RECT_T              prg_rect = {0, 0, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM};
    GUI_RECT_T              prg_rect = MMITHEME_GetClientRect();

#if defined KEYPAD_TYPE_QWERTY_KEYPAD   
    uint8 keycode = (uint8)(msg_id & 0x000000ff); 
    keycode = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(keycode);
    msg_id = (msg_id & 0xffffff00) | (keycode & 0x000000ff);
#endif

    //SCI_TRACE_LOW:"MMIDM==> HandleDMServerLockWinMsg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2012_112_2_18_2_14_31_869,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef TOUCH_PANEL_SUPPORT
        CreateDialButton(win_id);
#endif
        MMISRVAUD_ReqVirtualHandle("DM LOCK", MMISRVAUD_PRI_NORMAL);
        break;
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, prg_rect, MMI_WINDOW_BACKGROUND_COLOR);
        break;
    //键盘锁下允许拨打紧急拨号
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
        MMIAPICC_OpenEmergencyWin(msg_id);
        break;
    case MSG_KEYDOWN_HASH:
        if (MMIDM_IsDebugMode())
        {
            MMIDM_ServerUnlockCallBack();
        }
        break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_CLOSE_WINDOW:
        MMISRVAUD_FreeVirtualHandle("DM LOCK");
        break;
    default:
#ifdef TOUCH_PANEL_SUPPORT
        if (MSG_CTL_PENOK == msg_id&&MMIDM_DIAL_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id )
        {
            MMK_CreateWin((uint32*)MMICC_EMERGENCY_WIN_TAB, PNULL);
        }
#endif
        recode = MMI_RESULT_FALSE;
        break;

    }

    return recode;

}

/*****************************************************************************/
//  Description : update dm waiting win info 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateDmWaitingWinInfo(MMI_TEXT_ID_T label)
{
    MMI_STRING_T wait_text = {0};
    MMI_GetLabelTextByLang(label, &wait_text);

    MMIPUB_UpdateWaitWinText(TRUE,1,&wait_text,PNULL,PNULL,MMIDM_WAITING_WIN_ID);
}

/*****************************************************************************/
//  Description : handle dm session change state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleDmSessionChange(MMIDM_SESSION_CHANGE_T *data)
{
    if (PNULL!=data)
    {
        //SCI_TRACE_LOW:"MMIDM==>  HandleDmSessionChange %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2084_112_2_18_2_14_31_870,(uint8*)"d",data->inState);
        switch(data->inState) 
        {
            case DM_SESSION_START:
                break;
            case DM_SESSION_COMPLETE:
                if (DM_SESSION_FOTA == MMIDM_GetDmSessionType())
                {
                    MMIDM_SetFotaUpdateState(DM_FOTA_UPDATA_DONE);
                    MMIDM_SetFotaUpdateFlag(DM_FOTA_OP_CLEAR_FLAG);
                }
                break;
            case DM_SESSION_ABORT:
                if (RDM_ERR_COMMS_FATAL ==data->error||RDM_ERR_COMMS_NON_FATAL == data->error)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DM_NETWORK_DISCONNECT);
                }
                else
                {
                    if ( DM_SESSION_CLIENT== MMIDM_GetDmSessionType())
                    {
                         MMIPUB_OpenAlertWarningWin(TXT_DM_UN_SEARCH_PACKAGE);
                    }
                }
                if (DM_SESSION_FOTA == MMIDM_GetDmSessionType())
                {
                   MMIDM_SetFotaUpdateState(DM_FOTA_UPDATA_FAIL);
                }
                MMIDM_CloseWaitingWin();
                break;
            default:
                break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==>  HandleDmSessionChange PNULL==data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2120_112_2_18_2_14_31_871,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : close dl session progress win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/****************************************************************************/
PUBLIC void MMIDM_CloseDLSessionProgressWin(void)
{
    if (0 ==s_mmidm_no_signal_timerid)
    {
        MMK_CloseWin(MMIDM_DLSESSION_PROGRESS_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : handle dl session change state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleDlSessionChange(MMIDM_SESSION_CHANGE_T *data)
{
    if (PNULL!=data)
    {
        //SCI_TRACE_LOW:"MMIDM==>  HandleDlSessionChange %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2143_112_2_18_2_14_31_872,(uint8*)"d",data->inState);

        switch(data->inState) 
        {
            case DM_SESSION_START:
                s_is_dl_cancel = FALSE;
                UpdateDmWaitingWinInfo(TXT_DM_DL_WAIT);
                break;
            case DM_SESSION_COMPLETE:
                MMIDM_CloseDLSessionProgressWin();
                MMIDM_CleanResumeNv();
                MMIDM_RemoveResumeFile(MMIDM_GetResumeType());
                break;
            case DM_SESSION_ABORT:
                MMIDM_CloseDLSessionProgressWin();
                break;
            default:
                break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==>  HandleDlSessionChange PNULL==data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2165_112_2_18_2_14_31_873,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : deal with dm session state change
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SessionChangeCallBack(MMIDM_SESSION_CHANGE_T *data)
{ 
    if (PNULL == data)
    {
        //SCI_TRACE_LOW:"MMIDM==>  MMIDM_SessionChangeCallBack PNULL == data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2176_112_2_18_2_14_31_874,(uint8*)"");
        
        return;
    }
    //SCI_TRACE_LOW:"MMIDM==>  MMIDM_SessionChangeCallBack inState = %d,inType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2180_112_2_18_2_14_31_875,(uint8*)"dd",data->inState,data->inType);
    switch(data->inType)
    {
    case DM_SESSION_DM:
        HandleDmSessionChange(data);
        break;
    case DM_SESSION_DL:
        HandleDlSessionChange(data);
        break;
    default:
        break;
    }
      
    SCI_FREE(data);    
}

/*****************************************************************************/
//  Description : Notify alert call back
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_NotifyAlertCallBack(MMIDM_NOTIFY_ALERT_T *data)
{
    MMIDM_NOTIFY_ALERT_T *notify_alert_ptr = PNULL;

    if (PNULL == data)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_NotifyAlertCallBack PNULL == data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2206_112_2_18_2_14_31_876,(uint8*)"");
        return;
    }
    notify_alert_ptr = data;

    MMIDM_OpenDmSessionAlertWin(notify_alert_ptr->type,
        notify_alert_ptr->string,
        notify_alert_ptr->str_len,
        notify_alert_ptr->max_timer);
    SCI_FREE(data);
}

/*****************************************************************************/
//  Description : error alert
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ErrorAlert(uint32 error,MMIDM_SESSION_TYPE_E type)
{
    //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DOWNLOAD_SUCCEED,TXT_DOWNLOAD_FAILED,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
// 	Description : DmSelectDelayTimeWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DmSelectDelayTimeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIDM_DELAY_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    uint32 interval_timer = MMIDM_FOTA_DELAY_TIME;
    uint32  timers = 0;
   // MN_IMSI_T imsi = {0};
    if (MMIDM_IsDebugMode())
    {
        interval_timer = MMIDM_ONE_MIN_TIME;
    }

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        if (s_mmidm_fumo_update_timerid != 0)
        {
            MMK_StopTimer(s_mmidm_fumo_update_timerid);
            s_mmidm_fumo_update_timerid = 0;
        }
        GUILIST_SetMaxItem(ctrl_id,MMIDM_DELAY_TIME_ITEM, FALSE );//max item 2

        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_DELAY_TIME_1,TXT_NULL,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_DELAY_TIME_2,TXT_NULL,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_DELAY_TIME_4,TXT_NULL,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt(TXT_DM_DELAY_TIME_24,TXT_NULL,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
       
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
        timers = interval_timer*4;
        s_mmidm_fumo_update_timerid = MMK_CreateTimerCallback(timers, MMIAPIDM_HandleDmTimer,  NULL, FALSE);
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        switch(cur_selection)
        {
            case 0:
                timers = interval_timer;
        	    break;
            case 1:
                timers = interval_timer*2;
        	    break;
            case 2:
                timers = interval_timer*4;
        	    break;
            case 3:
                timers = interval_timer*24;
        	    break;
            default:
                break;
        }
        s_mmidm_fumo_update_timerid = MMK_CreateTimerCallback(timers, MMIAPIDM_HandleDmTimer,  NULL, FALSE);
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : to handle message in dm/dl alert confirm handle
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmAlertConfirmWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==>  HandleDmAlertConfirmWin: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2320_112_2_18_2_14_31_877,(uint8*)"d",msg_id);
    
	switch (msg_id)
	{
    case MSG_OPEN_WINDOW:
        ShowDisplayInfo(MMIDM_ALERT_CONFIRM_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIDM_ALERT_CONFIRM_CTRL_ID);
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        break;
        
    case MSG_LOSE_FOCUS://maryxiao cr245175
        if (MMIDM_IsNeedDelayDMSession())
        {
            MMIDM_StopDmRunCheckTimer();
            MMIDM_SetIsNeedRestart(TRUE);
        }
        break;     

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
		MMK_CloseWin(win_id);    
		break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        MMK_CloseWin( win_id );
        break;
       
	case MSG_TIMER:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        MMK_CloseWin( win_id );
        break;
    case MSG_APP_RED:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        MMK_CloseWin( win_id );
        break;
    case MSG_CLOSE_WINDOW:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        CloseDisplayConfimCheckTimer();
        break;
	default:
		result = MMI_RESULT_FALSE;
	    break;
	}
    return (result);
}
/*****************************************************************************/
// 	Description : handle dm alert display 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmAlertDisplayWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==>  HandleDmAlertDisplayWin msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2381_112_2_18_2_14_31_878,(uint8*)"d",msg_id);

	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
        if (MMIDM_ALERT_NIA_DISPLAY == s_display_info.mode)
        {
            s_mmidm_win_timerid = MMK_CreateWinTimer(win_id, MMIDM_MAX_ALERT_DISPLAY_TIME, FALSE);
        }
        ShowDisplayInfo(MMIDM_DISPLAY_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIDM_DISPLAY_CTRL_ID);
        break;
    case MSG_TIMER:
        if(s_mmidm_win_timerid == *(uint8*)param)	
        {
            MMK_StopTimer(s_mmidm_win_timerid);
            s_mmidm_win_timerid = 0; 
            MMIDM_HandleUserSelectOption(win_id,msg_id);
            MMK_CloseWin( win_id );
        }
        break;
    case MSG_APP_RED:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        MMK_CloseWin(win_id);
        break;
	case MSG_CTL_CANCEL:
        MMIDM_HandleUserSelectOption(win_id,msg_id);
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
        break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}


/*****************************************************************************/
//	Description : dm handle user select option
//	Global resource dependence : 
//	Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void MMIDM_HandleUserSelectOption(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{
	MMIDM_ALERT_TYPE_E alert_mode = s_display_info.mode;

	//SCI_TRACE_LOW:"MMIDM==>  MMIDM_HandleUserSelectOption alert_mode =%d msg_id=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2428_112_2_18_2_14_31_879,(uint8*)"dd",alert_mode,msg_id);

	switch(alert_mode)
	{
	case MMIDM_ALERT_PL_DISPLAY:
	    HandleAlertPlDisplay(msg_id);
		break;
	case MMIDM_ALERT_PL_CONFIRM:
        HandleAlertPlConfirm(msg_id);
		break;
	case MMIDM_ALERT_NIA_DISPLAY:
		HandleAlertNIADisplay();
		break;
	case MMIDM_ALERT_NIA_CONFIRM:
	    HandleAlertNIAConfirm(msg_id);
		break;
	case MMIDM_ALERT_FUMO_CONFIRM:
    case MMIDM_ALERT_FUMO_UPDATE:
		HandleAlertFUMOConfirm(win_id,msg_id);
		break;
	case MMIDM_ALERT_SCOMO_CONFIRM:
		HandleAlertScomoConfirm(win_id,msg_id);
		break;
	default:
		break;
	}
}

/*****************************************************************************/
//  Description : handle alert pl display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertPlDisplay(MMI_MESSAGE_ID_E msg_id)
{
	VDM_MMI_Observer_t* observer_ptr = MMIDM_GetObserverData();

    //SCI_TRACE_LOW:"MMIDM==> HandleAlertPlDisplay %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2464_112_2_18_2_14_31_880,(uint8*)"d",msg_id);

    if (PNULL !=observer_ptr &&MMIDM_IsDmRun())
    {
        switch(msg_id)
	    {
	        case MSG_APP_CANCEL:
            case MSG_CTL_CANCEL:
            case MSG_APP_RED:
		        observer_ptr->notifyInfoMsgClosed();
		        break;

	        case MSG_TIMER:
		        observer_ptr->notifyTimeout();
		        break;

	        default:
		        break;
	    }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> HandleAlertPlDisplay observer_ptr== PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2486_112_2_18_2_14_32_881,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle alert pl display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertPlConfirm(MMI_MESSAGE_ID_E msg_id)
{
	VDM_MMI_Observer_t* observer_ptr = MMIDM_GetObserverData();

    //SCI_TRACE_LOW:"MMIDM==> HandleAlertPlConfirm %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2498_112_2_18_2_14_32_882,(uint8*)"d",msg_id);

    if (PNULL!=observer_ptr&&MMIDM_IsDmRun())
    {
        switch(msg_id)
	    {
	        case MSG_APP_OK:
            case MSG_APP_WEB:
            case MSG_CTL_OK:
#ifdef DM_SUPPORT_REDBENCH
		        observer_ptr->notifyConfirmationResult(TRUE);
#elif defined(DM_SUPPORT_DFRESH)
                MMIDM_NotifyAlertResult(TRUE);
#endif
		        break;
	        case MSG_APP_CANCEL:
            case MSG_CTL_CANCEL:
            case MSG_APP_RED:
#ifdef DM_SUPPORT_REDBENCH
		        observer_ptr->notifyConfirmationResult(FALSE);
#elif defined(DM_SUPPORT_DFRESH)
                MMIDM_NotifyAlertResult(FALSE);
#endif
		        break;

	        case MSG_TIMER:
#ifdef DM_SUPPORT_REDBENCH
		        observer_ptr->notifyTimeout();
#elif defined(DM_SUPPORT_DFRESH)
                MMIDM_NotifyAlertResult(FALSE);
#endif
		        break;
	        default:
		        break;
	    }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> HandleAlertPlConfirm observer_ptr== PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2536_112_2_18_2_14_32_883,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : handle alert nia display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertNIADisplay(void)
{
    MMIDM_NotifyNIASessionProceed();
}
/*****************************************************************************/
//  Description : handle alert nia display 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertNIAConfirm(MMI_MESSAGE_ID_E msg_id)
{
    //SCI_TRACE_LOW:"MMIDM==> HandleAlertNIAConfirm"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2553_112_2_18_2_14_32_884,(uint8*)"");
	switch(msg_id)
	{
		case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
			MMIDM_NotifyNIASessionProceed();
#if defined(DM_SUPPORT_DFRESH)
            MMIDM_NotifyAlertResult(TRUE);
#endif

			break;
		case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_APP_RED:
		case MSG_TIMER:
            MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
#if defined(DM_SUPPORT_DFRESH)
            MMIDM_NotifyAlertResult(FALSE);
#endif

			break;
	    default:
		    break;
	}
}

/*****************************************************************************/
//  Description : handle Fumo confirm alert
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertFUMOConfirm(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{

    //SCI_TRACE_LOW:"MMIDM==> HandleAlertFUMOConfirm"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2579_112_2_18_2_14_32_885,(uint8*)"");

    switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
            MMK_CloseWin(MMIDM_DLSESSION_PROGRESS_WIN_ID);
			GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_DM_UPDATE_NOW, TXT_NULL, TXT_DM_UPDATE_LATER, FALSE);
			break;
		case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
            if(MMIDM_InstallUpdatePackage())
            {
                uint32 time_period = 5000;
                MMIPUB_OpenAlertWinByTextId(&time_period,TXT_DM_INSTALL_SUCC,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleDmFumoUpdateSuccWin);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_UPDATE_FAIL);
            }
			break;
		case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CreateWin((uint32*)MMIDM_SELECT_DELAYTIMER_WIN_TAB, PNULL);
            break;
        case MSG_CLOSE_WINDOW:
            MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO);
            break;
	    default:
		    break;
	}
}


/*****************************************************************************/
//  Description : handle alert scomo alert
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandleAlertScomoConfirm(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{
    //SCI_TRACE_LOW:"MMIDM==> HandleAlertScomoConfirm"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2619_112_2_18_2_14_32_886,(uint8*)"");

    switch(msg_id)
    {
	    case MSG_OPEN_WINDOW:
	        MMK_CloseWin(MMIDM_DLSESSION_PROGRESS_WIN_ID);
            MMIDM_RemoveResumeFile(RESUME_TYPE_SCOMO);
		    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_YES, TXT_NULL, TXT_COMM_NO, FALSE);
		    break;
	    case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
            MMIDM_ScomoInstall(FALSE);
		    break;
	    case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_CLOSE_WINDOW:
            break;
        default:
	        break;
    }  
}


/*****************************************************************************/
// 	Description : get fumo update timer id
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsDmTimerId(uint32 timer_id)
{
    if (  timer_id == s_mmidm_fumo_update_timerid 
        ||timer_id == s_mmidm_run_timerid
        ||timer_id == s_mmidm_no_signal_timerid
        ||timer_id == s_mmidm_delay_timerid
        ||timer_id == s_mmidm_fota_report_timerid
        ||timer_id == s_mmidm_dis_confim_timerid
        ||timer_id == s_mmidm_choice_check_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_IsDmTimerId "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2657_112_2_18_2_14_32_887,(uint8*)"");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
// 	Description : handle create no sig timer
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_CreateFotaReportTimer(void)
{
    if (s_mmidm_fota_report_timerid != 0)
    {
        MMK_StopTimer(s_mmidm_fota_report_timerid);
        s_mmidm_fota_report_timerid = 0;
    }
    s_mmidm_fota_report_timerid = MMK_CreateTimerCallback(MMIDM_DL_CHECK_TIME, MMIAPIDM_HandleDmTimer,  NULL, FALSE);
}
/*****************************************************************************/
// 	Description : handle create no sig timer
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_CreateNoSigTimer(void)
{
    if (s_mmidm_no_signal_timerid != 0)
    {
        MMK_StopTimer(s_mmidm_no_signal_timerid);
        s_mmidm_no_signal_timerid = 0;
    }
    s_mmidm_no_signal_timerid = MMK_CreateTimerCallback(MMIDM_DISPLAY_DELAY_TIME,MMIAPIDM_HandleDmTimer,  NULL,  FALSE);
}
/*****************************************************************************/
// 	Description : handle fumo update timer 
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandleDmTimer(uint8 timer_id, uint32 param)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2697_112_2_18_2_14_32_888,(uint8*)"d",timer_id);
    if (timer_id == s_mmidm_fumo_update_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_fumo_update_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2700_112_2_18_2_14_32_889,(uint8*)"");
        if (s_mmidm_fumo_update_timerid != 0)
        {
            MMK_StopTimer(s_mmidm_fumo_update_timerid);
            s_mmidm_fumo_update_timerid = 0;
        }
        if (MMIDM_GetNVDownloadFinish()&& MMIDM_IsResumeFileExist(RESUME_TYPE_FUMO_DATA))
        {
            MMIDM_OpenDmSessionAlertWin(MMIDM_ALERT_FUMO_UPDATE,PNULL,0,0);
        }
    }
    else if (timer_id == s_mmidm_run_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_run_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2713_112_2_18_2_14_32_890,(uint8*)"");
        if (MMIDM_IsDmRun())
        {
            MMIDM_ReSetDmPushInfo();
            MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
        }
        MMIDM_StopDmRunCheckTimer();
    }
    else if (timer_id == s_mmidm_no_signal_timerid)
    {
        MN_PHONE_PLMN_STATUS_E status = MMIAPIPHONE_GetPlmnStatus(MMIDM_GetMainSim());
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_no_signal_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2724_112_2_18_2_14_32_891,(uint8*)"");
        MMIDM_CloseNoSigTimer();
        MMIDM_CloseBrokenSessionTimer(); /*cr251729*/
        if (!MMIDM_IsNeedDelayDMSession()
            &&(PLMN_NORMAL_GSM_ONLY == status 
            || PLMN_NORMAL_GSM_GPRS_BOTH == status
            || PLMN_NORMAL_GPRS_ONLY == status))
        {
            MMIDM_ReStartDmSession();
        }
        else
        {
            MMIDM_ReSetDmPushInfo();
            MMIDM_StopDmRunCheckTimer();
            MMIDM_CloseDLSessionProgressWin();
            if (MMIDM_IsDmRun())
            {
                MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
            }
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM,TXT_DM_NETWORK_DISCONNECT,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        }
    }
    else if (timer_id == s_mmidm_delay_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_delay_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2747_112_2_18_2_14_32_892,(uint8*)"");
        MMIDM_CloseDelayTimer();
        if (!MMIDM_IsNeedDelayDMSession())
        {
            MMIDM_ReStartDmSession();
        }
        else
        {
            MMIDM_ReSetDmPushInfo();
            MMIDM_StopDmRunCheckTimer();
            if (MMIDM_IsDmRun())
            {
                MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
            }
        }
    }
    else if( timer_id== s_mmidm_dis_confim_timerid)	
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_dis_confim_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2765_112_2_18_2_14_32_893,(uint8*)"");
        CloseDisplayConfimCheckTimer();
        if (!MMIDM_IsNeedDelayDMSession())
        {
            if (MMIDM_ALERT_PL_DISPLAY == s_display_info.mode)
            {
                MMIDM_HandleUserSelectOption(MMIDM_ALERT_DISPLAY_WIN_ID,MSG_TIMER);
                MMK_CloseWin( MMIDM_ALERT_DISPLAY_WIN_ID );
            }
            else
            {
                MMIDM_HandleUserSelectOption(MMIDM_ALERT_CONFIRM_WIN_ID,MSG_TIMER);
                MMK_CloseWin( MMIDM_ALERT_CONFIRM_WIN_ID );
            }
        }
        else
        {
            s_is_need_handle_alert_timerout_event = TRUE;
            MMIDM_StopDmRunCheckTimer();
        }
    }
    else if ( timer_id == s_mmidm_fota_report_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_fota_report_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2788_112_2_18_2_14_32_894,(uint8*)"");
        if (s_mmidm_fota_report_timerid != 0)
        {
            MMK_StopTimer(s_mmidm_fota_report_timerid);
            s_mmidm_fota_report_timerid = 0;
        }
        MMIDM_SendFotaReport();
    }
    else if (timer_id == s_mmidm_choice_check_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_choice_check_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2798_112_2_18_2_14_32_895,(uint8*)"");
        CloseChoiceListCheckTimer();
        if (!MMIDM_IsNeedDelayDMSession())
        {
	        VDM_MMI_Observer_t*observer_ptr = MMIDM_GetObserverData();
            if (PNULL!=observer_ptr&&MMIDM_IsDmRun())
            {
                observer_ptr->notifyTimeout();
            }
        }
        else
        {
            MMIDM_StopDmRunCheckTimer();
        }
        MMK_CloseWin(MMIDM_CHOICELIST_QUERY_WIN_ID);
    }
#ifdef DM_SUPPORT_DFRESH   
    else if (timer_id == s_mmidm_socket_timerid)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmTimer s_mmidm_socket_timerid in"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2817_112_2_18_2_14_32_896,(uint8*)"");

        MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
        MMIDM_StopDmSocketCheckTimer();
    }
#endif
	/*+++ cr251729*/
    else if (timer_id == s_mmidm_broken_session_timerid)
    {
        SCI_TRACE_LOW("MMIDM==> MMIDM_HandleDmTimer s_mmidm_broken_session_timerid in");
        MMIDM_CloseBrokenSessionTimer();
        if (MMIDM_IsDmRun())
        {
            MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
        }
    }
	/*--- cr251729*/
}


/*****************************************************************************/
// 	Description : open resume query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void MMIDM_OpenResumeQueryWin(MMIPUB_HANDLE_FUNC  handle_func)
{
    MMI_STRING_T text = {0};
    MMI_WIN_ID_T                    win_id = MMIDM_ALERTHANDLE_WIN_ID;
    MMI_GetLabelTextByLang(TXT_DM_DL_CONTINUE, &text);
    MMIPUB_OpenQueryWinByTextPtr(&text, PNULL, IMAGE_PUBWIN_QUERY, &win_id, handle_func);
}


/*****************************************************************************/
// 	Description : to handle dm resume query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDmResumeQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMIDM==>  HandleDmResumeQueryWin: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2852_112_2_18_2_14_32_897,(uint8*)"d",msg_id);

	switch (msg_id)
	{
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
		    MMK_CloseWin(win_id);  
            MMIDM_StartDm(DM_SESSION_FUMO_RESUME,PNULL,0);
		    break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO_DATA);
            MMIDM_RemoveResumeFile(RESUME_TYPE_FUMO);
            MMIDM_StartDm(DM_SESSION_CLIENT,PNULL,0);
            break;
        
	    default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
	        break;
	}
    return (result);
}

/*****************************************************************************/
// 	Description : to handle restart dm resume query win
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReDmResumeQueryWin(
									     MMI_WIN_ID_T win_id, 
									     MMI_MESSAGE_ID_E msg_id, 
									     DPARAM param
									     )
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIDM_RESUME_TYPE   resume_type = MMIDM_GetResumeType();
    //SCI_TRACE_LOW:"MMIDM==>  HandleReDmResumeQueryWin: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2891_112_2_18_2_14_32_898,(uint8*)"d",msg_id);

	switch (msg_id)
	{
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
            if (RESUME_TYPE_FUMO == resume_type)
            {
                MMIDM_StartDm(DM_SESSION_FUMO_RESUME,PNULL,0);
            }
            else if (RESUME_TYPE_SCOMO == resume_type)
            {
                MMIDM_StartDm(DM_SESSION_SCOMO_RESUME,PNULL,0);
            }
		    MMK_CloseWin(win_id);  
		    break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;
        
	    default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
	        break;
	}
    return (result);
}

/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_HandleDmIdleOption(void)
{
    if (MMIDM_GetHandsetLock())
    {
    	//SCI_TRACE_LOW:"MMIDM==> lock session\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2929_112_2_18_2_14_32_899,(uint8*)"");
        OpenLockWin();
    }
}

/*****************************************************************************/
// 	Description : to handle dm option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_HandeDmOption(void)
{
    BOOLEAN  is_handle = FALSE;
    is_handle = MMIDM_SendFotaReport();

    if (MMIDM_GetHandsetLock())
    {
        return;
    }
    
    if (MMIDM_GetNVDownloadFinish()
        &&0 == s_mmidm_fumo_update_timerid
        &&MMIDM_IsResumeFileExist(RESUME_TYPE_FUMO_DATA)
        &&!is_handle)
    {
    	//SCI_TRACE_LOW:"MMIDM==> update session\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2953_112_2_18_2_14_32_900,(uint8*)"");
        MMIDM_OpenDmSessionAlertWin(MMIDM_ALERT_FUMO_UPDATE,PNULL,0,0);
        is_handle = TRUE;
    }

    if (!is_handle  
        &&(RESUME_TYPE_FUMO==MMIDM_GetResumeType()||RESUME_TYPE_SCOMO ==MMIDM_GetResumeType())
        &&!MMIDM_IsDmRun()) 
    {
        MMIDM_OpenResumeQueryWin(HandleReDmResumeQueryWin);
    }
}

/*****************************************************************************/
// 	Description : to create display confim check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CreateDispalyConfimCheckTimer(uint32 timer)
{
    uint32 timer_num = MMIDM_DISPLAY_DELAY_TIME;
    //SCI_TRACE_LOW:"MMIDM==> CreateDispalyConfimCheckTimer timer=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2973_112_2_18_2_14_32_901,(uint8*)"d",timer);
    CloseDisplayConfimCheckTimer();
    if (0 != timer 
        && timer_num>timer)
    {
        if (timer<MMIDM_ONE_MIN_TIME)
        {
            timer_num = MMIDM_ONE_MIN_TIME;
        }
        else
        {
            timer_num = timer;
        }
    }
    s_mmidm_dis_confim_timerid = MMK_CreateTimerCallback(timer_num, MMIAPIDM_HandleDmTimer,  NULL,FALSE);
    //SCI_TRACE_LOW:"MMIDM==> CreateDispalyConfimCheckTimer timer_num=%d s_mmidm_dis_confim_timerid=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_2988_112_2_18_2_14_33_902,(uint8*)"dd",timer_num,s_mmidm_dis_confim_timerid);
}

/*****************************************************************************/
// 	Description : to close display confim check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CloseDisplayConfimCheckTimer(void)
{
    if ( 0 != s_mmidm_dis_confim_timerid)
    {
        MMK_StopTimer(s_mmidm_dis_confim_timerid);
        s_mmidm_dis_confim_timerid = 0;
    }
}

/*****************************************************************************/
// 	Description : to create choice list check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CreateChoiceListCheckTimer(uint32 timer)
{
    uint32 timer_num = MMIDM_DISPLAY_DELAY_TIME;
    //SCI_TRACE_LOW:"MMIDM==> CreateChoiceListCheckTimer timer=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_3011_112_2_18_2_14_33_903,(uint8*)"d",timer);
    CloseChoiceListCheckTimer();
    if (0 != timer 
        && timer_num>timer)
    {
        if (timer<MMIDM_ONE_MIN_TIME)
        {
            timer_num = MMIDM_ONE_MIN_TIME;
        }
        else
        {
            timer_num = timer;
        }
    }
    s_mmidm_choice_check_timerid = MMK_CreateTimerCallback(timer_num, MMIAPIDM_HandleDmTimer,  NULL, FALSE);
    //SCI_TRACE_LOW:"MMIDM==> CreateDispalyConfimCheckTimer timer_num=%d s_mmidm_choice_check_timerid=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_3026_112_2_18_2_14_33_904,(uint8*)"dd",timer_num,s_mmidm_choice_check_timerid);
}

/*****************************************************************************/
// 	Description : to close choice list check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void CloseChoiceListCheckTimer(void)
{
    if ( 0 != s_mmidm_choice_check_timerid)
    {
        MMK_StopTimer(s_mmidm_choice_check_timerid);
        s_mmidm_choice_check_timerid = 0;
    }
}
/*****************************************************************************/
// 	Description : to create dm wap check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateDelayTimer(uint32 timer)
{
    uint32 timer_num = MMIDM_DISPLAY_DELAY_TIME;

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_CreateDelayTimer timer=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_3049_112_2_18_2_14_33_905,(uint8*)"d",timer);
    
    if ( 0 != s_mmidm_delay_timerid)
    {
        MMK_StopTimer(s_mmidm_delay_timerid);
        s_mmidm_delay_timerid = 0;
    }
    if (0 != timer)
    {
        timer_num = timer;
    }
    s_mmidm_delay_timerid = MMK_CreateTimerCallback(timer_num, MMIAPIDM_HandleDmTimer,  NULL,FALSE);
}
/*****************************************************************************/
// 	Description : to close delay check  timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CloseDelayTimer(void)
{
    if (s_mmidm_delay_timerid != 0)
    {
        MMK_StopTimer(s_mmidm_delay_timerid);
        s_mmidm_delay_timerid = 0;
    }
}

/*****************************************************************************/
// 	Description : to get delay timerid
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDelayTimerId(void)
{
    return s_mmidm_delay_timerid;
}
/*****************************************************************************/
// 	Description : to close NoSig check  timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CloseNoSigTimer(void)
{
    if (s_mmidm_no_signal_timerid != 0)
    {
        MMK_StopTimer(s_mmidm_no_signal_timerid);
        s_mmidm_no_signal_timerid = 0;
    }
}

/*****************************************************************************/
// 	Description : to get NoSig check  timerid
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetNoSigTimerId(void)
{
    return s_mmidm_no_signal_timerid;
}

#ifdef DM_SUPPORT_DFRESH
/*****************************************************************************/
// 	Description : to create dm open csoket check timer
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateDmSocketCheckTimer(void)
{
    if ( 0 != s_mmidm_socket_timerid)
    {
        MMK_StopTimer(s_mmidm_socket_timerid);
        s_mmidm_socket_timerid = 0;
    }
    s_mmidm_socket_timerid = MMK_CreateTimerCallback(MMIDM_SOCKET_DELAY_TIME,MMIAPIDM_HandleDmTimer,  NULL, FALSE);
}
/*****************************************************************************/
// 	Description : to stop dm run check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_StopDmSocketCheckTimer(void)
{
    if ( 0 != s_mmidm_socket_timerid)
    {
        MMK_StopTimer(s_mmidm_socket_timerid);
        s_mmidm_socket_timerid = 0;
    }
}
#endif
/*****************************************************************************/
// 	Description : to create dm run check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateDmRunCheckTimer(void)
{
    if ( 0 != s_mmidm_run_timerid)
    {
        MMK_StopTimer(s_mmidm_run_timerid);
        s_mmidm_run_timerid = 0;
    }
    s_mmidm_run_timerid = MMK_CreateTimerCallback(MMIDM_RUN_CHECK_TIME,MMIAPIDM_HandleDmTimer,  NULL, FALSE);
}
/*****************************************************************************/
// 	Description : to stop dm run check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_StopDmRunCheckTimer(void)
{
    if ( 0 != s_mmidm_run_timerid)
    {
        MMK_StopTimer(s_mmidm_run_timerid);
        s_mmidm_run_timerid = 0;
    }
}

/*****************************************************************************/
// 	Description : to show display info
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void ShowDisplayInfo( MMI_CTRL_ID_T ctrl_id)
{
    MMI_STRING_T    string = {0};
    wchar newline[]=L"\n";
    wchar display_info[MMIFMM_FILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 display_len = 0;
    MMIDM_ALERT_TYPE_E mode =s_display_info.mode;
    if (MMIDM_ALERT_FUMO_CONFIRM == mode)
    {
        MMI_GetLabelTextByLang(TXT_DM_DOWNLOAD_UPDATE, &string);//是否升级？
        MMIDM_SetNVDownloadFinish(TRUE);
    }
    else if (MMIDM_ALERT_FUMO_UPDATE == mode)
    {
        MMI_GetLabelTextByLang(TXT_DM_DOWNLOAD_UPDATE_ALERT, &string);//终端存有需要更新的固件升级包，是否需要升级？
        MMIDM_SetNVDownloadFinish(TRUE);
    }
    else if (MMIDM_ALERT_SCOMO_CONFIRM == mode)
    {
        MMI_GetLabelTextByLang(TXT_DM_INSTALL_APP_PAG, &string);//是否安装
        //MMIDM_SetNVDownloadFinish(FALSE);
    }
    else if (MMIDM_ALERT_NIA_DISPLAY == mode || MMIDM_ALERT_PL_DISPLAY == mode)
    {
        MMI_GetLabelTextByLang(TXT_DM_EXT_SERVICE, &string);//增强性服务
    }
    else if (MMIDM_ALERT_NIA_CONFIRM == mode || MMIDM_ALERT_PL_CONFIRM == mode)
    {
        MMI_GetLabelTextByLang(TXT_DM_EXT_SERVICES, &string);//是否接受增强性服务
    }

    MMI_WSTRNCPY(display_info,MMIFMM_FILE_FILE_NAME_MAX_LEN,string.wstr_ptr,string.wstr_len,string.wstr_len);
    display_len +=string.wstr_len;
    if (s_display_info.len!=0)
    {
        MMIAPICOM_Wstrcat(display_info,newline);
        display_len +=1;//sizeof(newline);
        MMIAPICOM_Wstrcat(display_info,s_display_info.buffer);
        display_len +=s_display_info.len;
    }
    
    GUITEXT_SetString(ctrl_id,display_info,display_len,FALSE);
}


/*****************************************************************************/
// 	Description : dm close all win
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_CloseAllWin(void)
{
    MMIDM_CloseWaitingWin();
}
/*****************************************************************************/
// 	Description : dm open UI ChoiceList Query win message
//	Global resource dependence : 
//  Author:jian.ma
/*****************************************************************************/
LOCAL void OpenDlChoiceListQueryWin(MMIDM_ALERT_TYPE_E mode)
{
    //SCI_TRACE_LOW:"MMIDM==> OpenDlChoiceListQueryWin mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_3220_112_2_18_2_14_33_906,(uint8*)"d",mode);

    if (MMIDM_ALERT_MULTI_CHOICE == mode)
    {
        MMK_CreateWin((uint32 *)MMIDM_CHOICELIST_QUERY_TAB, PNULL);
    }
    else if (MMIDM_ALERT_SINGLE_CHOICE == mode)
    {
        MMK_CreateWin((uint32 *)MMIDM_CHOICELIST_QUERY_TAB, PNULL);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> OpenDlChoiceListQueryWin error mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_3232_112_2_18_2_14_33_907,(uint8*)"");
    }
	
}
/*****************************************************************************/
// 	Description : to handle MulChiceList Query win message
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChiceListQueryWinMsg( 
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{ 
    VDM_MMI_ListContext_t*      list_context_ptr =PNULL;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    uint16                      i =0;
    uint32                      select_item=0;
	VDM_MMI_Observer_t*         observer_ptr = MMIDM_GetObserverData();
    
    //SCI_TRACE_LOW:"MMIDM: HandleChiceListQueryWinMsg msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_WINTAB_3252_112_2_18_2_14_33_908,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW: 
        GUIWIN_SetTitleText(win_id,s_display_info.buffer,s_display_info.len,FALSE);
        //create radio list box
        list_context_ptr=MMIDM_GetChoiceListInfo();

        GUILIST_SetMaxItem(MMIDM_CHOICELIST_EDITBOX_CTRL_ID, (uint16)list_context_ptr->itemsCount, FALSE);
        for(i = 0; i < list_context_ptr->itemsCount; i++)
        {
           AppendTextListItem(  MMIDM_CHOICELIST_EDITBOX_CTRL_ID,
                                (char*)(list_context_ptr->items)[i],
                                (uint16)strlen((char*)(list_context_ptr->items)[i]),
                                GUIITEM_STYLE_ONE_LINE_RADIO
                                );
           if(list_context_ptr->defaultSelection &(1 << i))
           {
               select_item = i;
           }
        }

        GUILIST_SetSelectedItem(MMIDM_CHOICELIST_EDITBOX_CTRL_ID, (uint16)select_item, TRUE);

        MMK_SetAtvCtrl( win_id,  MMIDM_CHOICELIST_EDITBOX_CTRL_ID);
        break;
    case MSG_CTL_OK:
        select_item = GUILIST_GetCurItemIndex(MMIDM_CHOICELIST_EDITBOX_CTRL_ID);
#ifndef WIN32
        observer_ptr->notifyChoiceListResult(select_item);
#endif
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#ifndef WIN32
        observer_ptr->notifyCanceled();
#endif
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIDM_CleanChoiceListInfo();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : add text item to list 
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void AppendTextListItem(
                                MMI_CTRL_ID_T       ctrl_id,
                                const char*         string_ptr,
                                uint16              string_len,
                                GUIITEM_STYLE_E      item_style
                                )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    //uint16                  i         = 0;
    wchar*              w_string_ptr =PNULL;
    
    w_string_ptr = SCI_ALLOCAZ(sizeof(wchar)*(string_len+1));
    if(PNULL == w_string_ptr)
    {
        return;
    }
    
    //SCI_MEMSET(w_string_ptr,0x00,(sizeof(wchar)*(string_len+1)));
    MMIAPICOM_StrToWstr((uint8*)string_ptr,w_string_ptr);

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = STXT_SELECT;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
   
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len =(uint16)MIN( string_len, GUILIST_STRING_MAX_NUM );
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = w_string_ptr;
//    MMIAPICOM_Wstrncpy( item_data.item_content[1].item_data.text_buffer.wstr_ptr, 
//        w_string_ptr,
//         item_data.item_content[1].item_data.text_buffer.wstr_len);
    
    GUILIST_AppendItem( ctrl_id, &item_t );
    SCI_FREE(w_string_ptr);
    w_string_ptr = PNULL;
}

/*+++ cr251729*/
/*****************************************************************************/
// 	Description : handle create broken session timer
//	Global resource dependence : none
//  Author:qing.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_CreateBrokenSessionTimer(void)
{
    if (s_mmidm_broken_session_timerid != 0)
    {
        MMK_StopTimer(s_mmidm_broken_session_timerid);
        s_mmidm_broken_session_timerid = 0;
    }
    s_mmidm_broken_session_timerid = MMK_CreateTimerCallback(3*MMIDM_ONE_MIN_TIME,MMIAPIDM_HandleDmTimer,  NULL,  FALSE);
}

/*****************************************************************************/
// 	Description : to close broken session timer
//	Global resource dependence : 
//  Author:qing.yu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CloseBrokenSessionTimer(void)
{
    SCI_TRACE_LOW("MMIDM==> MMIDM_CloseBrokenSessionTimer");
    if (s_mmidm_broken_session_timerid != 0)
    {
        MMK_StopTimer(s_mmidm_broken_session_timerid);
        s_mmidm_broken_session_timerid = 0;
    }
}

/*****************************************************************************/
// 	Description : to broken session timer
//	Global resource dependence : 
//  Author:qing.yu
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetBrokenTimerId(void)
{
    SCI_TRACE_LOW("MMIDM==> s_mmidm_broken_session_timerid=%d", s_mmidm_broken_session_timerid);
    return s_mmidm_broken_session_timerid;
}
/*--- cr251729*/

#endif /*DM_ONLY_SELFREG_SUPPORT*/

#endif 


/*Edit by script, ignore 5 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
