/****************************************************************************
** File Name:      mmimms_settingwin.c                                         *
** Author:         minghu.mao                                                 *
** Date:           05/26/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                             *
** 05/2009        minghu.mao          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmipub.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef MMI_GUI_STYLE_TYPICAL
#include "guilabel.h"
#include "mmimms_menutable.h"
#endif
#include "guidropdownlist.h"
#include "guisetlist.h"
//#include "guires.h"
//#ifdef MMS_SUPPORT
#include "mmk_type.h"
#include "mmiconnection_export.h"
#include "mmimms_id.h"
#include "mmimms_internal.h"
#ifdef MMI_GUI_STYLE_TYPICAL
#include "guiform.h"
#include "mmimms_menutable.h"
#endif
#include "mmimms_text.h"
#include "mmimms_setting.h"
#include "mmisd_export.h"
//#ifndef MMI_GUI_STYLE_TYPICAL
#include "mmi_appmsg.h"
//#endif
#include "mmifmm_export.h"
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
#include "mmisms_text.h"
#include "mmisms_app.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_LIST_STRNUM_MAX_LEN                    40

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
    MMISMS_PDA_MMS_SETTINGS_INIT,    
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
         MMISMS_PDA_MMS_SIM1_SC,
         MMISMS_PDA_MMS_SIM2_SC,   
         #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
         MMISMS_PDA_MMS_SIM3_SC,
         #endif     
         #if defined(MMI_MULTI_SIM_SYS_QUAD)
         MMISMS_PDA_MMS_SIM4_SC,
         #endif         
    #else
    MMISMS_PDA_MMS_SIM_SC,
    #endif
    
    MMISMS_PDA_MMS_SETTINGS_RECIEVE_OPT,    
    MMISMS_PDA_MMS_SETTINGS_SEND_OPT,
    MMISMS_PDA_MMS_SETTINGS_SAVE_POS,    
    MMISMS_PDA_MMS_SETTINGS_PLAY_AUDIO,
    MMISMS_PDA_MMS_SETTINGS_RESTORE,    
    MMISMS_PDA_MMS_SETTINGS_MAX
    
}MMISMS_PDA_MMS_SETTINGS_TYPE_E;

typedef enum
{
    MMISMS_PDA_MMS_SEND_SETTINGS_INIT,    
    MMISMS_PDA_MMS_SEND_VALID_PERIOD_OPT,    
    MMISMS_PDA_MMS_SEND_PRIORITY, 
    MMISMS_PDA_MMS_SEND_DELIVERY_REPORT,    
    MMISMS_PDA_MMS_SEND_READ_REPORT,    
    MMISMS_PDA_MMS_SEND_ANONYMOUS_REPORT,        
    MMISMS_PDA_MMS_SEND_SETTINGS_MAX
    
}MMISMS_PDA_MMS_SEND_TYPE_E;

LOCAL BOOLEAN is_update_settinglist = FALSE;

#endif

typedef enum
{
    MMISMS_PDA_MMS_RECIEVE_SETTINGS_INIT,    
    MMISMS_PDA_MMS_AUTO_DOWNLOAD_OPT,    
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
    MMISMS_MMS_ROAMING_AUTO_DOWNLOAD_OPT,
#endif
    MMISMS_PDA_MMS_ADV_OPT, 
    MMISMS_PDA_MMS_SEND_REPORT,        
    //////////////////////////////////////////////////////////////////////////
    // added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
    MMISMS_PDA_MMS_RETRIEVE_MODE_LOCAL,
    MMISMS_PDA_MMS_RETRIEVE_MODE_ROAM,
#endif
    MMISMS_PDA_MMS_RECIEVE_SETTINGS_MAX
    
}MMISMS_PDA_MMS_RECIEVE_TYPE_E;


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : update mms net setting when the index list item is selected  
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void MmsConnectionCallback(MN_DUAL_SYS_E dual_sys, uint32 selected_index);
#endif

/*****************************************************************************/
//  Description :mms setting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSSettingWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms setting sound list choose win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingSoundWinMsg(
                                         MMIMMS_WINDOW_ID_E win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                           );

/*****************************************************************************/
//  Description :mms receive setting  win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecevingSettingWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                           );

/*****************************************************************************/
//  Description :mms send setting  win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendingSettingWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                               );

/*****************************************************************************/
//  Description :mms setting reset win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: fun is equel with HandleRestoreDefaultWinMsg
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRestoreDefaultWinMsg(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :Set mms recieve setting dropdownlist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetRecvDropDownList( void );
#endif

/*****************************************************************************/
//  Description :Set mms send setting dropdownlist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetSendDropDownList( void );

/*****************************************************************************/
//  Description :mms save file position  win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveFilePosWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : append settings1 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaMMSSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 );


/*****************************************************************************/
//  Description : append settings 2 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaMMSSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data,   //用户数据
                                 BOOLEAN  is_need_replace
                                 );

/*****************************************************************************/
//  Description : mms send settings list
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMMSSendSettingsListCtrl(void);

/*****************************************************************************/
//  Description : init mms settings list
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMMSSettingsListCtrl(BOOLEAN is_need_replace);

/*****************************************************************************/
//  Description :mms send settings position  win handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendSettingsWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

#endif
//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description :handle retrieve mode win msg
//  Global resource dependence : 
//  Author: feng.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRetrieveModeWinMsg(MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param);
/*****************************************************************************/
//  Description : get cur retrieve mode from setting drop list
//  Global resource dependence : 
//  Author: feng.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_SETTING_RETRIEVE_MODE_E GetRetrieveModeFromDropList(uint16 index);
#endif
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//mms setting window
WINDOW_TABLE( MMIMMS_MMSSETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleMMSSettingWinMsg),    
    WIN_ID(MMIMMS_MMSSETTING_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_SETTING_MENU_CTRL_ID),
    #else
    CREATE_MENU_CTRL(MENU_MMS_SETTING, MMIMMS_SETTING_MENU_CTRL_ID),
    #endif
    END_WIN
};

WINDOW_TABLE( MMIMMS_SETTING_SOUND_LIST_WIN_TAB ) = 
{
    WIN_TITLE(TXT_MMS_PLAY_SOUND),
    WIN_FUNC((uint32)HandleSettingSoundWinMsg),    
    WIN_ID(MMIMMS_SETTING_SOUND_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMMS_SETTING_SOUND_LIST_CTRL_ID),
        END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMIMMS_RECEVINGSETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleRecevingSettingWinMsg),    
    WIN_ID(MMIMMS_RECEVINGSETTING_WIN_ID),
    WIN_TITLE(TXT_MMS_SETTING_RECV_OPT),    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_RECEVINGSETTING_CTRL_ID),

    END_WIN
};
#else
WINDOW_TABLE( MMIMMS_RECEVINGSETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleRecevingSettingWinMsg),    
    WIN_ID(MMIMMS_RECEVINGSETTING_WIN_ID),
    WIN_TITLE(TXT_MMS_SETTING_RECV_OPT),

    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_RECEVINGSETTING_CTRL_ID),

//////////////////////////////////////////////////////////////////////////
// modified by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMS_SETTING_RETRIEVEMODE_LOCAL_FORM_CTRL_ID, MMIMMS_RECEVINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMS_SETTING_RETRIEVEMODE_LOCAL_TITLE_LABEL_CTRL_ID, MMS_SETTING_RETRIEVEMODE_LOCAL_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMS_SETTING_RETRIEVEMODE_LOCAL_DROPDOWNLIST_CTRL_ID, MMS_SETTING_RETRIEVEMODE_LOCAL_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMS_SETTING_RETRIEVEMODE_ROAM_FORM_CTRL_ID, MMIMMS_RECEVINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMS_SETTING_RETRIEVEMODE_ROAM_TITLE_LABEL_CTRL_ID, MMS_SETTING_RETRIEVEMODE_ROAM_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMS_SETTING_RETRIEVEMODE_ROAM_DROPDOWNLIST_CTRL_ID, MMS_SETTING_RETRIEVEMODE_ROAM_FORM_CTRL_ID),
#else
        //aotodown
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_AOTODOWN_FORM_CTRL_ID,MMIMMS_RECEVINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_AOTODOWN_TITLE_LABEL_CTRL_ID,MMS_SETTING_AOTODOWN_FORM_CTRL_ID),
    #ifdef MMI_PDA_SUPPORT
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID,MMS_SETTING_AOTODOWN_FORM_CTRL_ID),
    #else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID,MMS_SETTING_AOTODOWN_FORM_CTRL_ID),
    #endif            
#endif
        //advertise
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_ADVERTISE_FORM_CTRL_ID,MMIMMS_RECEVINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_ADVERTISE_TITLE_LABEL_CTRL_ID,MMS_SETTING_ADVERTISE_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID,MMS_SETTING_ADVERTISE_FORM_CTRL_ID),
#else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID,MMS_SETTING_ADVERTISE_FORM_CTRL_ID),
#endif            
        //permit rpt
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_PERMIT_RPT_FORM_CTRL_ID,MMIMMS_RECEVINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_PERMIT_RPT_TITLE_LABEL_CTRL_ID,MMS_SETTING_PERMIT_RPT_FORM_CTRL_ID),
#ifdef  MMI_PDA_SUPPORT
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID,MMS_SETTING_PERMIT_RPT_FORM_CTRL_ID),
#else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID,MMS_SETTING_PERMIT_RPT_FORM_CTRL_ID),
#endif
    END_WIN
};
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMIMMS_SEND_SETTINGS_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSendSettingsWinMsg),    
    WIN_ID(MMIMMS_SEND_SETTINGS_WIN_ID),
    WIN_TITLE(TXT_MMS_DURATION),
#ifdef MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMMS_SEND_SETTING_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIMMS_SAVE_FILE_POS_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSaveFilePosWinMsg),    
    WIN_ID(MMIMMS_SAVE_FILE_POS_WIN_ID),
    WIN_TITLE(TXT_MMS_SAVE_FILE_POSITION),
#ifdef MMI_PDA_SUPPORT    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE( MMIMMS_SAVE_FILE_POS_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSaveFilePosWinMsg),    
    WIN_ID(MMIMMS_SAVE_FILE_POS_WIN_ID),
    WIN_TITLE(TXT_MMS_SAVE_FILE_POSITION),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_SAVE_FILE_POS_ID),
        //save fime position
        //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMMS_SETTING_SAVE_FILE_TITLE_CTRL_ID,MMIMMS_SAVE_FILE_POS_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID,MMIMMS_SAVE_FILE_POS_ID),
    END_WIN
};
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE( MMIMMS_SENDINGSETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSendingSettingWinMsg),    
    WIN_ID(MMIMMS_SENDINGSETTING_WIN_ID),
    WIN_TITLE(TXT_COMM_SEND_OPTION), 
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_SENDINGSETTING_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE( MMIMMS_SENDINGSETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSendingSettingWinMsg),    
    WIN_ID(MMIMMS_SENDINGSETTING_WIN_ID),
    WIN_TITLE(TXT_COMM_SEND_OPTION),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    #endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_SENDINGSETTING_CTRL_ID),
        //period
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_PERIOD_FORM_CTRL_ID,MMIMMS_SENDINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_PERIOD_TITLE_LABEL_CTRL_ID,MMS_SETTING_PERIOD_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT            
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID,MMS_SETTING_PERIOD_FORM_CTRL_ID),
#else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID,MMS_SETTING_PERIOD_FORM_CTRL_ID),
#endif            
        //priority
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_PRIORITY_FORM_CTRL_ID,MMIMMS_SENDINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_PRIORITY_TITLE_LABEL_CTRL_ID,MMS_SETTING_PRIORITY_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT 
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID,MMS_SETTING_PRIORITY_FORM_CTRL_ID),
#else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID,MMS_SETTING_PRIORITY_FORM_CTRL_ID),
#endif
        //send rpt
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_SENDRPT_FORM_CTRL_ID,MMIMMS_SENDINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_SENDRPT_TITLE_LABEL_CTRL_ID,MMS_SETTING_SENDRPT_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT 
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID,MMS_SETTING_SENDRPT_FORM_CTRL_ID),
#else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID,MMS_SETTING_SENDRPT_FORM_CTRL_ID),
#endif
        //read rpt
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_READRPT_FORM_CTRL_ID,MMIMMS_SENDINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_READRPT_TITLE_LABEL_CTRL_ID,MMS_SETTING_READRPT_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT 
            CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID,MMS_SETTING_READRPT_FORM_CTRL_ID),
#else
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID,MMS_SETTING_READRPT_FORM_CTRL_ID),
#endif
        //anonimity
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMS_SETTING_ANONYMITY_FORM_CTRL_ID,MMIMMS_SENDINGSETTING_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMS_SETTING_ANONYMITY_TITLE_LABEL_CTRL_ID,MMS_SETTING_ANONYMITY_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT         
           CHILD_SETLIST_CTRL(TRUE,MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID,MMS_SETTING_ANONYMITY_FORM_CTRL_ID),
#else            
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID,MMS_SETTING_ANONYMITY_FORM_CTRL_ID),
#endif
    END_WIN
};
#endif

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
WINDOW_TABLE(MMIMMS_SETTING_RETRIEVE_MODE_WIN_TBL) = 
{
    WIN_FUNC((uint32)HandleRetrieveModeWinMsg),    
    WIN_ID(MMIMMS_SETTING_RETRIEVE_MODE_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMMS_SETTING_RETRIEVE_MODE_CTRL_ID),
    #ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    #else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :create mms setting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSSettingWin(void)
{
    MMK_CreateWin((uint32 *)MMIMMS_MMSSETTING_WIN_TAB, PNULL);
    return;
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : MMICL_CreateAllLogChildWin
//	Global resource dependence : 
//  Author:rong.gu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CreateMMSSettingsChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIMMS_MMSSETTING_WIN_TAB, (ADD_DATA)parent_win_id);
}

/*****************************************************************************/
//  Description : append settings1 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaMMSSettingsItem1Line(
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


/*****************************************************************************/
//  Description : append settings 2 item
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void AppendPdaMMSSettingsItem2Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_STRING_T  text_str,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data,   //用户数据
                                 BOOLEAN is_need_replace
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar temp_wstr[MMIMMS_LIST_STRNUM_MAX_LEN + 2] = {0};
    wchar temp_len = 0;
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
        MMIAPICOM_Wstrncpy(temp_wstr,
            text_str.wstr_ptr,
            text_str.wstr_len);
        temp_len = text_str.wstr_len;
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    //text 2    
    if(text_str.wstr_ptr != PNULL && text_str.wstr_len != 0)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = temp_len;
    }
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;

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

/*****************************************************************************/
//  Description : mms send settings list
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMMSSendSettingsListCtrl(void)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    MMI_CTRL_ID_T       ctrl_id = MMIMMS_SENDINGSETTING_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    MMIMMS_SETTING_VALID_PERIOD_E valid_period                               = MMIMMS_SETTING_VALID_PERIOD_LONGEST;
    MMIMMS_SETTING_PRIORITY_E priority                                       = MMIMMS_SETTING_PRIORITY_NORMAL;
    BOOLEAN if_have_send_report                                              = FALSE;
    BOOLEAN if_have_read_report                                              = FALSE;
    BOOLEAN if_anonymity_send                                                = FALSE;
    MMI_TEXT_ID_T  period_text[MMS_SETTING_DROPPAGE_NUM3]                = {TXT_COMM_1_HOUR,TXT_COMM_12_HOURS, TXT_COMM_24_HOURS,TXT_MMS_ONE_WEEK,TXT_MMS_LONGGEST_DURATION};
    MMI_TEXT_ID_T  priority_text[MMS_SETTING_DROPPAGE_NUM2]              = {TXT_COMMON_HIGH, TXT_COMMON_MID,TXT_COMM_LOW};
    uint16 cur_select = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_MMS_SEND_SETTINGS_MAX, FALSE );   
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 

    valid_period = MMIMMS_Setting_GetValidPeriod();
    MMI_GetLabelTextByLang(period_text[valid_period] , &text_str);  
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_MMS_DURATION,
    text_str,
    IMAGE_COMMON_RIGHT_ARROW,
    (uint32)MMISMS_PDA_MMS_SEND_VALID_PERIOD_OPT,
    FALSE);    

    priority = MMIMMS_Setting_GetPriority();
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    MMI_GetLabelTextByLang(priority_text[priority] , &text_str);  
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_COMMON_PRIORITY,
    text_str,
    IMAGE_COMMON_RIGHT_ARROW,
    (uint32)MMISMS_PDA_MMS_SEND_PRIORITY,
    FALSE);

    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    if_have_send_report = MMIMMS_Setting_GetDeliveryReport();
    if(if_have_send_report)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_MMS_ARRIVE_REPORT,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_MMS_SEND_DELIVERY_REPORT,
    FALSE);

    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    if_have_read_report = MMIMMS_Setting_GetReadReport();
    if(if_have_read_report)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_MMS_READ_REPORT,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_MMS_SEND_READ_REPORT,
    FALSE);
        
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    if_anonymity_send = MMIMMS_Setting_GetAnonymous();
    if(if_anonymity_send)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_MMS_ANONYMITY,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_MMS_SEND_ANONYMOUS_REPORT,
    FALSE);        
        
    GUILIST_SetCurItemIndex(ctrl_id, cur_select);
//end    
}

/*****************************************************************************/
//  Description : mms recieve settings list
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMMSRecieveSettingsListCtrl(void)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    //uint8 i = 0;
    BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMIMMS_RECEVINGSETTING_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    uint16 cur_select = GUILIST_GetCurItemIndex(ctrl_id);

    GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_MMS_RECIEVE_SETTINGS_MAX, FALSE );   
    //////////////////////////////////////////////////////////////////////////
    // modified by feng.xiao for MS00207703 20120119
#ifdef DATA_ROAMING_SUPPORT
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
    AppendPdaMMSSettingsItem2Line(ctrl_id,
                                  TXT_MMS_SET_RETRIEVE_LOCAL,
                                  text_str,
                                  IMAGE_COMMON_RIGHT_ARROW,
                                  MMISMS_PDA_MMS_RETRIEVE_MODE_LOCAL,
                                  FALSE);

    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
    AppendPdaMMSSettingsItem2Line(ctrl_id,
                                  TXT_MMS_SET_RETRIEVE_ROAM,
                                  text_str,
                                  IMAGE_COMMON_RIGHT_ARROW,
                                  MMISMS_PDA_MMS_RETRIEVE_MODE_ROAM,
                                  FALSE);
#else
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    flag = MMIMMS_Setting_GetRetrieveMode();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_COMMON_AUTO_DOWNLOAD,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_MMS_AUTO_DOWNLOAD_OPT,
    FALSE);
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    flag = MMIMMS_Setting_GetRoamingRetrieveMode();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_COMMON_ROAMING_AUTO_DOWNLOAD,
    text_str,
    image_id,
    (uint32)MMISMS_MMS_ROAMING_AUTO_DOWNLOAD_OPT,
    FALSE);
#endif    
#endif

    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    flag = MMIMMS_Setting_GetAdvertising();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_MMS_ADVERTISE_TITLE_NAME,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_MMS_ADV_OPT,
    FALSE);


    SCI_MEMSET(&text_str, 0x00, sizeof(text_str)); 
    flag = MMIMMS_Setting_GetRecvDeliveryReport();
    if(flag)
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {      
       image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }        
    AppendPdaMMSSettingsItem2Line(            
    ctrl_id,
    TXT_MMS_SEND_ARRIVE_REPORT,
    text_str,
    image_id,
    (uint32)MMISMS_PDA_MMS_SEND_REPORT,
    FALSE);

    GUILIST_SetCurItemIndex(ctrl_id, cur_select);
//end    
}

/*****************************************************************************/
//  Description : init mms settings list
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
LOCAL void  InitPdaMMSSettingsListCtrl(BOOLEAN is_need_replace)
{
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    //uint8 i = 0;
    BOOLEAN flag = FALSE;
    MMI_CTRL_ID_T       ctrl_id = MMIMMS_SETTING_MENU_CTRL_ID;
    MMI_STRING_T  text_str = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint8 curr_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;     
    MMIFILE_DEVICE_E  save_device = MMI_DEVICE_UDISK;
    MMI_TEXT_ID_T  text_id = TXT_NULL;
    uint16 index = 0;
    uint16 focus_index = 0;
    uint16 cur_select = GUILIST_GetCurItemIndex(ctrl_id);
    uint16 i = 0;
    MMI_TEXT_ID_T mms_setting_sim_sc[MMI_DUAL_SYS_MAX]=
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            TXT_MMS_USER1,
            TXT_MMS_USER2,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_MMS_USER3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_MMS_USER4,
#endif
#endif
#else
            TXT_MMS_USER
#endif    
    };
    uint32 mms_setting_sim_userdata[MMI_DUAL_SYS_MAX] = 
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMISMS_PDA_MMS_SIM1_SC,
            MMISMS_PDA_MMS_SIM2_SC,            
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_PDA_MMS_SIM3_SC,
#endif            
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            MMISMS_PDA_MMS_SIM4_SC,
#endif         
#else
            MMISMS_PDA_MMS_SIM_SC,
#endif
    };
    
    if(is_need_replace)
    { 
        focus_index = GUILIST_GetCurItemIndex(ctrl_id); 
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id, MMISMS_PDA_MMS_SETTINGS_MAX, FALSE );   
    }

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if( (is_need_replace && index == focus_index) ||!is_need_replace)
        {
            dual_sys = (MN_DUAL_SYS_E)i;
            curr_index = MMIMMS_Setting_GetNetSettingIndex(dual_sys);
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, curr_index); 
            text_str.wstr_ptr = link_setting_ptr->name;
            text_str.wstr_len  = link_setting_ptr->name_len;    
            AppendPdaMMSSettingsItem2Line(            
            ctrl_id,
            mms_setting_sim_sc[i],
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)mms_setting_sim_userdata[i],
            is_need_replace);
        }
        index++;
    }
    
    if(!is_need_replace)
    {
        AppendPdaMMSSettingsItem1Line(            
            ctrl_id,
            TXT_MMS_SETTING_RECV_OPT,
            (uint32)MMISMS_PDA_MMS_SETTINGS_RECIEVE_OPT);
    }
    index++;
    
    if(!is_need_replace)
    {
        AppendPdaMMSSettingsItem1Line(            
            ctrl_id,
            TXT_COMM_SEND_OPTION,
            (uint32)MMISMS_PDA_MMS_SETTINGS_SEND_OPT);
    }
    index++;
    
    if( (is_need_replace && index == focus_index) || !is_need_replace)
    {
        save_device = MMIMMS_Setting_GetPreferDisk();
        text_id = MMIAPIFMM_GetDeviceName(save_device);
        MMI_GetLabelTextByLang(text_id , &text_str);        
        AppendPdaMMSSettingsItem2Line(            
            ctrl_id,
            TXT_MMS_SAVE_FILE_POSITION,
            text_str,
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMISMS_PDA_MMS_SETTINGS_SAVE_POS,
            is_need_replace);
    }
    index++;
    SCI_MEMSET(&text_str, 0x00, sizeof(text_str));         
    if((is_need_replace && index == focus_index) ||!is_need_replace)
    {
        
        flag = MMIMMS_Setting_GetPlaysound();
        if(flag)
        {
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {      
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }        
        AppendPdaMMSSettingsItem2Line(            
            ctrl_id,
            TXT_MMS_PLAY_SOUND,
            text_str,
            image_id,
            (uint32)MMISMS_PDA_MMS_SETTINGS_PLAY_AUDIO,
            is_need_replace);
    }
    index++;

    if(!is_need_replace)
    {
        AppendPdaMMSSettingsItem1Line(            
            ctrl_id,
            TXT_MMS_SETTING_RE_DEFAULT,
            (uint32)MMISMS_PDA_MMS_SETTINGS_RESTORE);
    }

    GUILIST_SetCurItemIndex(ctrl_id, cur_select);
//end    
}

/*****************************************************************************/
//  Description :mms setting win handle fun pda
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSSettingWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    //uint32  group_id      = 0;
    //uint32  menu_id       = 0;
    MMI_CTRL_ID_T ctrl_id = MMIMMS_SETTING_MENU_CTRL_ID;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    uint16 index = 0;
    uint16 list_index = 0;
    uint32 user_data = 0;
    BOOLEAN play_sound = FALSE;    
    uint32 i = MN_DUAL_SYS_1;  
    switch(msg_id)
    {    
    case MSG_OPEN_WINDOW:
        {  
            InitPdaMMSSettingsListCtrl(FALSE);
            for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {                
                if (!MMIAPIPHONE_IsSimCardOk(i))
                {
                    GUILIST_SetItemGray(ctrl_id, i , TRUE);
                }
            }

            MMK_SetAtvCtrl(win_id, MMIMMS_SETTING_MENU_CTRL_ID);  
        }
        break;
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        if (is_update_settinglist)
        {
            InitPdaMMSSettingsListCtrl(FALSE);
            is_update_settinglist = FALSE;
        }
        else
        {
            InitPdaMMSSettingsListCtrl(TRUE);
        }
        
        if(!MMIAPIFMM_GetDeviceTypeStatus(MMIMMS_Setting_GetPreferDisk()))
        {
            SCI_TRACE_LOW("[MMIMMS:]HandleMMSSettingWinMsg current disk is invalid");
#ifdef NANDBOOT_SUPPORT
            MMIMMS_Setting_SetPreferDisk(MMI_DEVICE_UDISK);
#else
            MMIMMS_Setting_SetPreferDisk(MMIAPIFMM_GetDefaultDisk());
#endif
            InitPdaMMSSettingsListCtrl(FALSE);
        }
        
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {                
            if (!MMIAPIPHONE_IsSimCardOk(i))
            {
                GUILIST_SetItemGray(ctrl_id, i , TRUE);
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            //uint32 i = MN_DUAL_SYS_1;
            //BOOLEAN is_user_setting = FALSE;

            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &user_data);
            switch((MMISMS_PDA_MMS_SETTINGS_TYPE_E)user_data)
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                case MMISMS_PDA_MMS_SIM1_SC:
                case MMISMS_PDA_MMS_SIM2_SC:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
                case MMISMS_PDA_MMS_SIM3_SC:
#endif     
#if defined(MMI_MULTI_SIM_SYS_QUAD)
                case MMISMS_PDA_MMS_SIM4_SC:
#endif       
                list_index = MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + user_data - MMISMS_PDA_MMS_SIM1_SC),
                                                                     MMIMMS_Setting_GetNetSettingIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + user_data - MMISMS_PDA_MMS_SIM1_SC)));
                MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_MMS, list_index,
                                                   (MN_DUAL_SYS_1 + user_data - MMISMS_PDA_MMS_SIM1_SC), MmsConnectionCallback);
                break;  
                    
#else
                case MMISMS_PDA_MMS_SIM_SC:

                list_index = MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_E)MN_DUAL_SYS_1,
                                                                     MMIMMS_Setting_GetNetSettingIndex((MN_DUAL_SYS_E)MN_DUAL_SYS_1));
                MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_MMS, list_index, MN_DUAL_SYS_1, MmsConnectionCallback);
          
                break;
                #endif
                
                case MMISMS_PDA_MMS_SETTINGS_RECIEVE_OPT:
                MMK_CreateWin((uint32*)MMIMMS_RECEVINGSETTING_WIN_TAB, PNULL);
                break;   

                case MMISMS_PDA_MMS_SETTINGS_SEND_OPT:
                MMK_CreateWin((uint32*)MMIMMS_SENDINGSETTING_WIN_TAB, PNULL);
                break;   

                case MMISMS_PDA_MMS_SETTINGS_SAVE_POS:
                MMK_CreatePubListWin((uint32*)MMIMMS_SAVE_FILE_POS_WIN_TAB, PNULL);
                break;   

                case MMISMS_PDA_MMS_SETTINGS_PLAY_AUDIO:                
                play_sound = !MMIMMS_Setting_GetPlaysound();
                MMIMMS_Setting_SetPlaysound(play_sound);                  
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
                break;

                case MMISMS_PDA_MMS_SETTINGS_RESTORE:
                MMIPUB_OpenQueryWinByTextId(TXT_MMS_RESTORE_DEFAULT,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleRestoreDefaultWinMsg);
                break;
                
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
        //todo 关闭窗口,处理自动签名的全局变量
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}
#else
/*****************************************************************************/
//  Description :mms setting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSSettingWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint32  group_id      = 0;
    uint32  menu_id       = 0;
    uint16 list_index = 0;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    //Menu ID的声明不需要通过多卡宏控制!!
    MMI_MENU_ID_T mms_user_setting[]=
    {
        ID_MMS_SETTING_USER1,
        ID_MMS_SETTING_USER2,
        ID_MMS_SETTING_USER3,
        ID_MMS_SETTING_USER4
    };
    
    switch(msg_id)
    {    
    case MSG_OPEN_WINDOW:
        {            
            uint32 i = MN_DUAL_SYS_1;
            
            MMK_SetAtvCtrl(win_id, MMIMMS_SETTING_MENU_CTRL_ID);
            
            for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {                
                if (MMIAPIPHONE_IsSimCardOk(i))
                {
                    MMIAPICOM_EnableGrayed(win_id, MENU_MMS_SETTING,mms_user_setting[i],FALSE);
                }
                else
                {
                    MMIAPICOM_EnableGrayed(win_id, MENU_MMS_SETTING,mms_user_setting[i],TRUE);
                }
            }
            
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);      
        }
        break;
    case MSG_FULL_PAINT:        
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //根据menuid进行不同设置，主要有link设置，声音设置，是否回复默认配置，发送选项，接收选项
        {
            uint32 i = MN_DUAL_SYS_1;
            BOOLEAN is_user_setting = FALSE;

            GUIMENU_GetId(MMIMMS_SETTING_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id), (MMI_MENU_ID_T *)(&menu_id));

            // for multsim could use one handling method...
            for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if(menu_id == mms_user_setting[i])
                {   
                    is_user_setting = TRUE;
                    break;
                }
            }
            
            // is mms_setting_user1/user2/user3/user4
            if(is_user_setting) 
            {   
                list_index = MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_E)MN_DUAL_SYS_1, MMIMMS_Setting_GetNetSettingIndex((MN_DUAL_SYS_E)MN_DUAL_SYS_1));
#ifdef MMI_PDA_SUPPORT
                MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_MMS, list_index, (MN_DUAL_SYS_E)i, MmsConnectionCallback);
#else
                MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_MMS, list_index, (MN_DUAL_SYS_E)i, MmsConnectionCallback);
#endif
            }
            else
            {
                switch(menu_id)
                {
                case ID_MMS_SETTING_RECV:
                    MMK_CreateWin((uint32*)MMIMMS_RECEVINGSETTING_WIN_TAB, PNULL);
                    break;
                case ID_MMS_SETTING_SEND:
                    MMK_CreateWin((uint32*)MMIMMS_SENDINGSETTING_WIN_TAB, PNULL);
                    break;
                case ID_MMS_SETTING_SAVE_FILE_POS:
                    MMK_CreateWin((uint32*)MMIMMS_SAVE_FILE_POS_WIN_TAB, PNULL);
                    break;
                case ID_MMS_SETTING_SOUND:
                    #ifdef  MMI_PDA_SUPPORT
                    MMK_CreatePubListWin((uint32*)MMIMMS_SETTING_SOUND_LIST_WIN_TAB, PNULL);
                    #else
                    MMK_CreateWin((uint32*)MMIMMS_SETTING_SOUND_LIST_WIN_TAB, PNULL);
                    #endif
                	break;
                case ID_MMS_SETTING_DEFAULT:
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_RESTORE_DEFAULT,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleRestoreDefaultWinMsg);
                    break;
                default:
                    err_code = MMI_RESULT_FALSE;
                    break;
                }
            }
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        //todo 关闭窗口,处理自动签名的全局变量
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}
#endif

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : update mms net setting when the index list item is selected  
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void MmsConnectionCallback(MN_DUAL_SYS_E dual_sys, uint32 selected_index)
{
    MMIMMS_Setting_SetNetSettingIndex(dual_sys, selected_index);
#ifndef MMI_PDA_SUPPORT
    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
}
#endif

/*****************************************************************************/
//  Description :mms setting reset win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: fun is equel with HandleRestoreDefaultWinMsg
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRestoreDefaultWinMsg(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMIMMS_ResetMMSFactorySetting();
#ifndef MMI_GUI_STYLE_TYPICAL        
        is_update_settinglist = TRUE;
#endif
        MMK_PostMsg(MMIMMS_MMSSETTING_WIN_ID, MMISMS_UPDATE_LIST, PNULL, NULL);
        MMK_PostMsg(MMIMMS_MMSSETTING_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);         
        MMK_CloseWin(win_id);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMSMMI_OpenMMSAlertWin(TXT_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :mms setting sound list choose win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingSoundWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                           )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMIMMS_CONTROL_ID_E  list_ctrl_id = MMIMMS_SETTING_SOUND_LIST_CTRL_ID;
    uint16            list_index = 0;
    BOOLEAN           play_sound = TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //从g_nv_setting_mirror.is_play_sound读取是否允许播放声音，显示出来
        GUILIST_SetMaxItem(list_ctrl_id, NUM_MMS_MSG_SETTINGS_ONOFF, FALSE );//max item 2
        
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMMON_ENABLE_ON, STXT_OK, TXT_NULL, STXT_EXIT, list_ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );//open
        MMIAPISET_AppendListItemByTextIdExt(TXT_CLOSE, STXT_OK, TXT_NULL, STXT_EXIT, list_ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );//close
        
        play_sound = MMIMMS_Setting_GetPlaysound();
        if (TRUE == play_sound)
        {
            list_index = 0;
        }
        else
        {
            list_index = 1;
        }
        GUILIST_SetSelectedItem(list_ctrl_id, list_index, TRUE);
        GUILIST_SetCurItemIndex(list_ctrl_id, list_index);
        MMK_SetAtvCtrl(win_id, list_ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //根据选择的配置，设置g_nv_setting_mirror.is_play_sound，提示设置完成
        list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if (1 == list_index)
        {
            play_sound = FALSE;
        }
        else 
        {
            play_sound = TRUE;
        }
        if(list_index < NUM_MMS_MSG_SETTINGS_ONOFF)
        {
            MMIMMS_Setting_SetPlaysound(play_sound);  
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS); 
            MMK_CloseWin(win_id);
        }
        else
        {
            //SCI_TRACE_MID:"list_index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTINGWIN_1297_112_2_18_2_39_0_425,(uint8*)"d",list_index);
        }        
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

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :mms receive setting  win handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecevingSettingWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                           )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIMMS_RECEVINGSETTING_CTRL_ID;
    uint16 index = 0;
    uint32 user_data = 0;
    BOOLEAN flag  = TRUE;
#ifdef DATA_ROAMING_SUPPORT
	BOOLEAN is_need_update = TRUE;
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaMMSRecieveSettingsListCtrl();
        MMK_SetAtvCtrl(win_id, MMIMMS_RECEVINGSETTING_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
         {
            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &user_data);
            switch((MMISMS_PDA_MMS_RECIEVE_TYPE_E)user_data)
            {           
#ifndef DATA_ROAMING_SUPPORT
                case MMISMS_PDA_MMS_AUTO_DOWNLOAD_OPT:
                flag = !MMIMMS_Setting_GetRetrieveMode();
                MMIMMS_Setting_SetRetrieveMode((MMIMMS_SETTING_RETRIEVE_E)flag);                  
                break;   
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
                case MMISMS_MMS_ROAMING_AUTO_DOWNLOAD_OPT:
                    flag = !MMIMMS_Setting_GetRoamingRetrieveMode();
                    MMIMMS_Setting_SetRoamingRetrieveMode((MMIMMS_SETTING_RETRIEVE_E)flag);                                      
                    break;
#endif                    
#endif

                case MMISMS_PDA_MMS_ADV_OPT:
                flag = !MMIMMS_Setting_GetAdvertising();
                MMIMMS_Setting_SetAdvertising(flag);                  
                break;   

                case MMISMS_PDA_MMS_SEND_REPORT:
                flag = !MMIMMS_Setting_GetRecvDeliveryReport();
                MMIMMS_Setting_SetRecvDeliveryReport(flag);                  
                break;   
                //////////////////////////////////////////////////////////////////////////
                // added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
                case MMISMS_PDA_MMS_RETRIEVE_MODE_LOCAL:
                case MMISMS_PDA_MMS_RETRIEVE_MODE_ROAM:
                    {
                        MMK_CreatePubListWin((uint32 *)MMIMMS_SETTING_RETRIEVE_MODE_WIN_TBL, (ADD_DATA)user_data);
                        is_need_update = FALSE;
                        break;
                    }
#endif
                default:
                break;
            }
#ifdef DATA_ROAMING_SUPPORT            
            if(is_need_update)
#endif               
            {
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);            
            }
        }
        break;
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitPdaMMSRecieveSettingsListCtrl();
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}
#else
/*****************************************************************************/
//  Description :mms receive setting  win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecevingSettingWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                           )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev;
    // MMI_CTRL_ID_T  ctrl_id               = 0;
#ifdef DATA_ROAMING_SUPPORT
    uint16 retrieve_mode_index = 0;
    MMIMMS_SETTING_RETRIEVE_MODE_E retrieve_mode_e = MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
#else
    uint16 mms_setting_if_aotodown       = 0;
#endif
    uint16 mms_setting_if_permit_adv     = 0;
    uint16 mms_setting_if_permit_sendrpt = 0;
    // uint16 save_pos_index                = 0;
    
    lcd_dev.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev.block_id = GUI_BLOCK_MAIN; 
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        #ifndef  MMI_PDA_SUPPORT
    //////////////////////////////////////////////////////////////////////////
    // modified by feng.xiao
    #ifdef DATA_ROAMING_SUPPORT
        GUIFORM_SetStyle(MMS_SETTING_RETRIEVEMODE_LOCAL_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMS_SETTING_RETRIEVEMODE_ROAM_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    #else
        GUIFORM_SetStyle(MMS_SETTING_AOTODOWN_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    #endif
        GUIFORM_SetStyle(MMS_SETTING_ADVERTISE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMS_SETTING_PERMIT_RPT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
        
#ifdef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMIMMS_RECEVINGSETTING_CTRL_ID,GUIFORM_TYPE_TP);
#ifndef DATA_ROAMING_SUPPORT
        GUISETLIST_SetFontAlign(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
        GUISETLIST_SetFontAlign(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
        //从g_nv_setting_mirror读取自动下载和接收广告的配置通过LIST控件显示        
    //////////////////////////////////////////////////////////////////////////
    // modified by feng.xiao
    #ifdef  DATA_ROAMING_SUPPORT
        GUILABEL_SetTextById(MMS_SETTING_RETRIEVEMODE_LOCAL_TITLE_LABEL_CTRL_ID, TXT_MMS_SET_RETRIEVE_LOCAL, FALSE);
        GUILABEL_SetTextById(MMS_SETTING_RETRIEVEMODE_ROAM_TITLE_LABEL_CTRL_ID, TXT_MMS_SET_RETRIEVE_ROAM, FALSE);
    #else
        GUILABEL_SetTextById(MMS_SETTING_AOTODOWN_TITLE_LABEL_CTRL_ID, TXT_COMMON_AUTO_DOWNLOAD,FALSE);
    #endif
        GUILABEL_SetTextById(MMS_SETTING_ADVERTISE_TITLE_LABEL_CTRL_ID, TXT_MMS_ADVERTISE_TITLE_NAME,FALSE);
        GUILABEL_SetTextById(MMS_SETTING_PERMIT_RPT_TITLE_LABEL_CTRL_ID, TXT_MMS_SEND_ARRIVE_REPORT,FALSE);
        SetRecvDropDownList();
    #ifdef DATA_ROAMING_SUPPORT
        MMK_SetAtvCtrl(win_id, MMS_SETTING_RETRIEVEMODE_LOCAL_DROPDOWNLIST_CTRL_ID);
    #else
        MMK_SetAtvCtrl(win_id, MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID);
    #endif
        break;
#ifdef MMI_MMS_MINI_SUPPORT
    case MSG_CTL_MIDSK:   
    case MSG_APP_WEB:
        {            
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(SPRD_GUI_DROPDOWNLIST_ID == IGUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(ctrl_id)))
            {
                GUIDROPDOWNLIST_OpenList(win_id,ctrl_id);
            }    
        }
        break;
#endif
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
#ifndef DATA_ROAMING_SUPPORT                
                mms_setting_if_aotodown = GUISETLIST_GetCurIndex(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetRetrieveMode((MMIMMS_SETTING_RETRIEVE_E)mms_setting_if_aotodown);
#endif                
                mms_setting_if_permit_adv = GUISETLIST_GetCurIndex(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetAdvertising(mms_setting_if_permit_adv);
                mms_setting_if_permit_sendrpt = GUISETLIST_GetCurIndex(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetRecvDeliveryReport(mms_setting_if_permit_sendrpt);
                MMK_CloseWin(win_id);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifndef MMI_PDA_SUPPORT        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef DATA_ROAMING_SUPPORT
        retrieve_mode_index = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_RETRIEVEMODE_LOCAL_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetRetrieveMode(GetRetrieveModeFromDropList(retrieve_mode_index), MMIMMS_SETTING_TYPE_LOCAL);
        retrieve_mode_index = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_RETRIEVEMODE_ROAM_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetRetrieveMode(GetRetrieveModeFromDropList(retrieve_mode_index), MMIMMS_SETTING_TYPE_ROAM);
#else
        mms_setting_if_aotodown = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetRetrieveMode((MMIMMS_SETTING_RETRIEVE_E)mms_setting_if_aotodown);
#endif        
        mms_setting_if_permit_adv = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetAdvertising(mms_setting_if_permit_adv);
        mms_setting_if_permit_sendrpt = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetRecvDeliveryReport(mms_setting_if_permit_sendrpt);
        MMK_CloseWin(win_id);
        break;
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :mms receive setting  win handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendingSettingWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                           )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIMMS_SENDINGSETTING_CTRL_ID;
    uint16 index = 0;
    uint32 user_data = 0;
    BOOLEAN flag  = TRUE;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitPdaMMSSendSettingsListCtrl();
        MMK_SetAtvCtrl(win_id, MMIMMS_SENDINGSETTING_CTRL_ID);
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
            GUILIST_GetItemData(ctrl_id, index, &user_data);
            switch((MMISMS_PDA_MMS_SEND_TYPE_E)user_data)
            {                
                case MMISMS_PDA_MMS_SEND_VALID_PERIOD_OPT:
                case MMISMS_PDA_MMS_SEND_PRIORITY:                
                MMK_CreatePubListWin((uint32*)MMIMMS_SEND_SETTINGS_WIN_TAB, (ADD_DATA)user_data);                                             
                break;   

                case MMISMS_PDA_MMS_SEND_DELIVERY_REPORT:
                flag = !MMIMMS_Setting_GetDeliveryReport();
                MMIMMS_Setting_SetDeliveryReport(flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);  
                break;   
 
                case MMISMS_PDA_MMS_SEND_READ_REPORT:
                flag = !MMIMMS_Setting_GetReadReport();
                MMIMMS_Setting_SetReadReport(flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);  
                break;

                case MMISMS_PDA_MMS_SEND_ANONYMOUS_REPORT:
                flag = !MMIMMS_Setting_GetAnonymous();
                MMIMMS_Setting_SetAnonymous(flag);
                MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, NULL);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
                break;
                
                default:
                break;
            }          
        }
        break;
        
    case MSG_GET_FOCUS:
    case MMISMS_UPDATE_LIST:
        InitPdaMMSSendSettingsListCtrl();
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;
}

#else
/*****************************************************************************/
//  Description :mms send setting  win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendingSettingWinMsg(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E err_code                   = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev;
    // MMI_CTRL_ID_T  ctrl_id                  = 0;
    uint16  mms_setting_valid_period        = 0;
    uint16  mms_setting_priority            = 0;
    uint16  mms_setting_if_have_send_report = 0;
    uint16  mms_setting_if_have_read_report = 0;
    uint16  mms_setting_if_anonymity_send   = 0;
    
    lcd_dev.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev.block_id = GUI_BLOCK_MAIN; 
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef  MMI_PDA_SUPPORT        
        GUIFORM_SetStyle(MMS_SETTING_PERIOD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMS_SETTING_PRIORITY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMS_SETTING_SENDRPT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMS_SETTING_READRPT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMS_SETTING_ANONYMITY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif

#ifdef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMIMMS_SENDINGSETTING_CTRL_ID,GUIFORM_TYPE_TP);
        GUISETLIST_SetFontAlign(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
        //从g_nv_setting_mirror读取有效期，优先级，发送报告，阅读报告，匿名发送选项通过LIST控件显示       
        GUILABEL_SetTextById(MMS_SETTING_PERIOD_TITLE_LABEL_CTRL_ID, TXT_MMS_DURATION,FALSE);
        GUILABEL_SetTextById(MMS_SETTING_PRIORITY_TITLE_LABEL_CTRL_ID, TXT_COMMON_PRIORITY,FALSE);
        GUILABEL_SetTextById(MMS_SETTING_SENDRPT_TITLE_LABEL_CTRL_ID, TXT_MMS_ARRIVE_REPORT,FALSE);
        GUILABEL_SetTextById(MMS_SETTING_READRPT_TITLE_LABEL_CTRL_ID, TXT_MMS_READ_REPORT,FALSE);
        GUILABEL_SetTextById(MMS_SETTING_ANONYMITY_TITLE_LABEL_CTRL_ID, TXT_MMS_ANONYMITY,FALSE);
        SetSendDropDownList();
        MMK_SetAtvCtrl(win_id, MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID);
        break;

#ifdef MMI_MMS_MINI_SUPPORT
    case MSG_CTL_MIDSK:   
    case MSG_APP_WEB:
        {            
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(SPRD_GUI_DROPDOWNLIST_ID == IGUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(ctrl_id)))
            {
                GUIDROPDOWNLIST_OpenList(win_id,ctrl_id);
            }    
        }
        break;
#endif
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                mms_setting_valid_period = GUISETLIST_GetCurIndex(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetValidPeriod((MMIMMS_SETTING_VALID_PERIOD_E)mms_setting_valid_period);
                mms_setting_priority = GUISETLIST_GetCurIndex(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetPriority((MMIMMS_SETTING_PRIORITY_E)mms_setting_priority);
                mms_setting_if_have_send_report = GUISETLIST_GetCurIndex(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetDeliveryReport(mms_setting_if_have_send_report);
                mms_setting_if_have_read_report = GUISETLIST_GetCurIndex(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetReadReport(mms_setting_if_have_read_report);
                mms_setting_if_anonymity_send = GUISETLIST_GetCurIndex(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID);
                MMIMMS_Setting_SetAnonymous(mms_setting_if_anonymity_send);
                MMK_CloseWin(win_id);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                break;            
            default:
                break;
            }
        }
        break;
#endif

#ifndef MMI_PDA_SUPPORT        
    case MSG_CTL_OK:
    case MSG_APP_OK: 
        mms_setting_valid_period = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetValidPeriod((MMIMMS_SETTING_VALID_PERIOD_E)mms_setting_valid_period);
        mms_setting_priority = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetPriority((MMIMMS_SETTING_PRIORITY_E)mms_setting_priority);
        mms_setting_if_have_send_report = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetDeliveryReport(mms_setting_if_have_send_report);
        mms_setting_if_have_read_report = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetReadReport(mms_setting_if_have_read_report);
        mms_setting_if_anonymity_send = GUIDROPDOWNLIST_GetCurItemIndex(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID);
        MMIMMS_Setting_SetAnonymous(mms_setting_if_anonymity_send);
        MMK_CloseWin(win_id);
        break;
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    } 
    return err_code;    
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :mms send settings position  win handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendSettingsWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    uint16 index                                                     = 0;
    uint16 cur_index                                                 = 0;
    MMISMS_PDA_MMS_SEND_TYPE_E add_data = 0;
    MMI_TEXT_ID_T  period_text[MMS_SETTING_DROPPAGE_NUM3]                = {TXT_COMM_1_HOUR,TXT_COMM_12_HOURS, TXT_COMM_24_HOURS,TXT_MMS_ONE_WEEK,TXT_MMS_LONGGEST_DURATION};
    MMI_TEXT_ID_T  priority_text[MMS_SETTING_DROPPAGE_NUM2]              = {TXT_COMMON_HIGH, TXT_COMMON_MID,TXT_COMM_LOW};
    add_data = ((uint32)MMK_GetWinAddDataPtr(win_id));

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
        GUILIST_SetMaxItem(MMIMMS_SEND_SETTING_CTRL_ID, MMS_SETTING_DROPPAGE_NUM3, FALSE);
        if(MMISMS_PDA_MMS_SEND_VALID_PERIOD_OPT == add_data)
        {
            GUIWIN_SetTitleTextId(win_id,TXT_MMS_DURATION,FALSE);
            for (index = 0; index < MMS_SETTING_DROPPAGE_NUM3; index++)
            {                
                MMI_STRING_T text_str = {0};
                MMI_GetLabelTextByLang(period_text[index], &text_str);
                MMIMMS_AppendOneLineTextListItem(MMIMMS_SEND_SETTING_CTRL_ID,
                GUIITEM_STYLE_ONE_LINE_RADIO,
                TXT_COMMON_OK,  0,  text_str.wstr_ptr, 
                text_str.wstr_len, (uint16)index, FALSE, 0); 
            }
            cur_index = (uint16)MMIMMS_Setting_GetValidPeriod();
        }
        else if(MMISMS_PDA_MMS_SEND_PRIORITY == add_data)
        {
            GUIWIN_SetTitleTextId(win_id,TXT_COMMON_PRIORITY,FALSE);
            for (index = 0; index< MMS_SETTING_DROPPAGE_NUM2; index++)
            {                
                MMI_STRING_T text_str = {0};
                MMI_GetLabelTextByLang(priority_text[index], &text_str);
                MMIMMS_AppendOneLineTextListItem(MMIMMS_SEND_SETTING_CTRL_ID,
                GUIITEM_STYLE_ONE_LINE_RADIO,
                TXT_COMMON_OK,  0,  text_str.wstr_ptr, 
                text_str.wstr_len, (uint16)index, FALSE, 0); 
            }
             cur_index = (uint16)MMIMMS_Setting_GetPriority();
        }

        GUILIST_SetSelectedItem(MMIMMS_SEND_SETTING_CTRL_ID, cur_index, TRUE);
        GUILIST_SetCurItemIndex(MMIMMS_SEND_SETTING_CTRL_ID,cur_index);	
        MMK_SetAtvCtrl(win_id, MMIMMS_SEND_SETTING_CTRL_ID);    
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        cur_index = GUILIST_GetCurItemIndex(MMIMMS_SEND_SETTING_CTRL_ID);
        if(MMISMS_PDA_MMS_SEND_VALID_PERIOD_OPT == add_data)
        {
            if(cur_index < MMIMMS_SETTING_VALID_PERIOD_MAX)
            {
                MMIMMS_Setting_SetValidPeriod((MMIMMS_SETTING_VALID_PERIOD_E)cur_index);
            }
        }
        else if(MMISMS_PDA_MMS_SEND_PRIORITY == add_data )
        {
            if(cur_index < MMIMMS_SETTING_PRIORITY_MAX)
            {
                MMIMMS_Setting_SetPriority((MMIMMS_SETTING_PRIORITY_E)cur_index);
            }            
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

/*****************************************************************************/
//  Description :mms save file position  win handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveFilePosWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    static MMIFILE_DEVICE_E  s_device_table[MMI_DEVICE_NUM]                   = {0};
    MMIFILE_DEVICE_E  save_device                                    = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E device_type                                     = MMI_DEVICE_UDISK;
    uint16 index                                                     = 0;
    uint16 cur_index                                                 = 0;
    uint32 user_data = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        save_device = MMIMMS_Setting_GetPreferDisk();
        GUILIST_SetMaxItem(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, MMI_DEVICE_NUM, FALSE);
        for (device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(device_type))
            {
                MMI_TEXT_ID_T text_id = 0;
                MMI_STRING_T text_str = {0};
                s_device_table[index] = device_type;
                if(save_device == device_type)
                {
                    cur_index = index;
                }
                text_id = MMIAPIFMM_GetDeviceName(device_type);
                MMI_GetLabelTextByLang(text_id, &text_str);
                MMIMMS_AppendOneLineTextListItem(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID,
                    GUIITEM_STYLE_ONE_LINE_RADIO,
                    TXT_COMMON_OK,  0,  text_str.wstr_ptr, 
                    text_str.wstr_len, (uint16)index, FALSE, index);
                index++;
            }
        }
        GUILIST_SetSelectedItem(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, cur_index, TRUE);
        GUILIST_SetCurItemIndex(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID,cur_index);	
        MMK_SetAtvCtrl(win_id, MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID);                
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        cur_index = GUILIST_GetCurItemIndex(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID);
        GUILIST_GetItemData(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, cur_index, &user_data);
        if(user_data < MMI_DEVICE_NUM)
        {
            MMIMMS_Setting_SetPreferDisk(s_device_table[user_data]);
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
#else
/*****************************************************************************/
//  Description :mms save file position  win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveFilePosWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E err_code                = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev;
    // MMI_CTRL_ID_T  ctrl_id               = 0;
    //MMI_STRING_T save_pos_dropdownlist[MMS_SETTING_DROPPAGE_NUM1] = {0};
    //MMI_STRING_T save_pos_dropdownlist1[MMS_SETTING_DROPPAGE_NUM1 - 1] = {0};
    //MMI_TEXT_ID_T  save_pos_text[MMS_SETTING_DROPPAGE_NUM1]      = {TXT_MMS_SAVE_UDISK, TXT_COMMON_SD_CARD};
    //MMI_TEXT_ID_T  save_pos_text1[MMS_SETTING_DROPPAGE_NUM1 - 1] = {TXT_MMS_SAVE_UDISK};
    static MMIFILE_DEVICE_E  s_device_table[MMI_DEVICE_NUM]                   = {0};
    MMIFILE_DEVICE_E  save_device                                    = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E device_type                                     = MMI_DEVICE_UDISK;
    uint16 index                                                     = 0;
    uint16 cur_index                                                 = 0;
    
    lcd_dev.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev.block_id = GUI_BLOCK_MAIN; 
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUILABEL_SetTextById(MMIMMS_SETTING_SAVE_FILE_TITLE_CTRL_ID, TXT_MMS_SAVE_FILE_POSITION,FALSE);
		GUIDROPDOWNLIST_SetMaxItem(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, MMI_DEVICE_NUM);
		index = 0;
		save_device = MMIMMS_Setting_GetPreferDisk();
        for (device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(device_type))
            {
                //set to setting list
                s_device_table[index] = device_type;
                if(save_device == device_type)
                {
                    cur_index = index;
                }
                #ifdef MMI_PDA_SUPPORT
                GUISETLIST_AddItemById(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, MMIAPIFMM_GetDeviceName(device_type));
                #else
                GUIDROPDOWNLIST_AppendItemById(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, MMIAPIFMM_GetDeviceName(device_type));
                #endif
                index++; 
            }
        }
        #ifdef MMI_PDA_SUPPORT
        GUISETLIST_SetCurIndex(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, cur_index);
        #else
        GUIDROPDOWNLIST_SetCurItemIndex(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID, cur_index);
        #endif
        
        MMK_SetAtvCtrl(win_id, MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID);        
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:            
                cur_index = GUISETLIST_GetCurIndex(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID);
                if(cur_index < MMI_DEVICE_NUM)
                {
                    MMIMMS_Setting_SetPreferDisk(s_device_table[cur_index]);
                }
                MMK_CloseWin(win_id);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifndef MMI_PDA_SUPPORT        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        cur_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID);
        if(cur_index < MMI_DEVICE_NUM)
        {
            MMIMMS_Setting_SetPreferDisk(s_device_table[cur_index]);
        }
        MMK_CloseWin(win_id);
        break;
#endif
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
//  Description :add list item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_AppendOneLineTextListItem(MMI_CTRL_ID_T       ctrl_id,
                                      GUIITEM_STYLE_E    item_style,
                                      MMI_TEXT_ID_T       left_softkey_id,
                                      MMI_IMAGE_ID_T      icon_id,
                                      wchar*        wstr_ptr,
                                      uint16              wstr_len,
                                      uint16              pos,
                                      BOOLEAN             is_update,
                                      uint32 user_data
                                      )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16   i                    = 0;
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = user_data;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    if( 0 != icon_id )
    {
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = icon_id;
        i++;
    }
    
    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer.wstr_len = wstr_len;
    
    item_data.item_content[i].item_data.text_buffer.wstr_ptr = wstr_ptr;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//  Description :Set mms recieve setting dropdownlist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef DATA_ROAMING_SUPPORT
LOCAL void SetRecvDropDownList( void )
{
    uint8 index                                                             = 0;
    MMIMMS_SETTING_RETRIEVE_E if_aotodown                                   = MMIMMS_SETTING_RETRIEVE_MAX;
    BOOLEAN if_permit_adv                                                   = FALSE;
    BOOLEAN if_permit_rpt                                                   = TRUE;
    uint16 if_autodown_cur_index                                            = 0;
    uint16 if_permit_adv_cur_index                                          = 0;
    uint16 if_permit_rpt_cur_index                                          = 0;
    //MMI_STRING_T aotodown_dropdownlist[MMS_SETTING_DROPPAGE_NUM1] = {0};
    //MMI_STRING_T adv_dropdownlist[MMS_SETTING_DROPPAGE_NUM1]      = {0};
    //MMI_STRING_T rpt_dropdownlist[MMS_SETTING_DROPPAGE_NUM1]      = {0};
    MMI_TEXT_ID_T  aoto_text[MMS_SETTING_DROPPAGE_NUM1]                 = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  adv_text[MMS_SETTING_DROPPAGE_NUM1]                  = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  sendrpt_text[MMS_SETTING_DROPPAGE_NUM1]              = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};

    if_aotodown = MMIMMS_Setting_GetRetrieveMode();
    if (MMIMMS_SETTING_RETRIEVE_AUTO == if_aotodown)
    {
        if_autodown_cur_index = 1;
    }
    else
    {
        if_autodown_cur_index = 0;
    }
    if_permit_adv = MMIMMS_Setting_GetAdvertising();
    if (TRUE == if_permit_adv)
    {
        if_permit_adv_cur_index = 1;
    }
    else
    {
        if_permit_adv_cur_index = 0;
    }
    if_permit_rpt = MMIMMS_Setting_GetRecvDeliveryReport();
    if (TRUE == if_permit_rpt)
    {
        if_permit_rpt_cur_index = 1;
    }
    else
    {
        if_permit_rpt_cur_index = 0;
    }
    //todo get if_permit_rpt
    
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID,aoto_text[index]);
    }
    
    GUISETLIST_SetCurIndex(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID, if_autodown_cur_index);

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
           GUISETLIST_AddItemById(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID,adv_text[index]);
        
    }
    GUISETLIST_SetCurIndex(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID, if_permit_adv_cur_index);

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID,sendrpt_text[index]);
    }

    GUISETLIST_SetCurIndex(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID, if_permit_rpt_cur_index);

    
}
#endif
#else
//////////////////////////////////////////////////////////////////////////
// modified by feng.xiao
LOCAL void SetRecvDropDownList( void )
{
    uint8 index                                                             = 0;
#ifdef DATA_ROAMING_SUPPORT
    MMIMMS_SETTING_RETRIEVE_MODE_E cur_mode = MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
#else
    MMIMMS_SETTING_RETRIEVE_E if_aotodown                                   = MMIMMS_SETTING_RETRIEVE_MAX;
#endif
    BOOLEAN if_permit_adv                                                   = FALSE;
    BOOLEAN if_permit_rpt                                                   = TRUE;
#ifdef DATA_ROAMING_SUPPORT
    uint16 cur_mode_local = 0;
    uint16 cur_mode_roam = 0;
#else
    uint16 if_autodown_cur_index                                            = 0;
#endif
    uint16 if_permit_adv_cur_index                                          = 0;
    uint16 if_permit_rpt_cur_index                                          = 0;
#ifdef DATA_ROAMING_SUPPORT
    MMI_STRING_T mode_dropdownlist[MMS_SETTING_DROPPAGE_NUM2] = {0};
#else
    MMI_STRING_T aotodown_dropdownlist[MMS_SETTING_DROPPAGE_NUM1] = {0};
#endif
    MMI_STRING_T adv_dropdownlist[MMS_SETTING_DROPPAGE_NUM1]      = {0};
    MMI_STRING_T rpt_dropdownlist[MMS_SETTING_DROPPAGE_NUM1]      = {0};
#ifdef DATA_ROAMING_SUPPORT
    MMI_TEXT_ID_T  mode_text[MMS_SETTING_DROPPAGE_NUM2] = {TXT_MMS_SET_RETRIEVE_MODE_IMMEDIATE,
                                                               TXT_MMS_RETRIEVE_MANUAL,
                                                               TXT_MMS_SET_RETRIEVE_MODE_DENY};
#else
    MMI_TEXT_ID_T  aoto_text[MMS_SETTING_DROPPAGE_NUM1]                 = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
#endif
    MMI_TEXT_ID_T  adv_text[MMS_SETTING_DROPPAGE_NUM1]                  = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  sendrpt_text[MMS_SETTING_DROPPAGE_NUM1]              = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
#ifdef DATA_ROAMING_SUPPORT
    cur_mode = MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_LOCAL);
    if (MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE == cur_mode)
    {
        cur_mode_local = 0;
    }
    else if (MMIMMS_SETTING_RETRIEVE_MODE_DEFER == cur_mode)
    {
        cur_mode_local = 1;
    }
    else
    {
        cur_mode_local = 2;
    }
    cur_mode = MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_ROAM);
    if (MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE == cur_mode)
    {
        cur_mode_roam = 0;
    } 
    else if (MMIMMS_SETTING_RETRIEVE_MODE_DEFER == cur_mode)
    {
        cur_mode_roam = 1;
    }
    else
    {
        cur_mode_roam = 2;
    }
#else
    if_aotodown = MMIMMS_Setting_GetRetrieveMode();
    if (MMIMMS_SETTING_RETRIEVE_AUTO == if_aotodown)
    {
        if_autodown_cur_index = 1;
    }
    else
    {
        if_autodown_cur_index = 0;
    }
#endif
    if_permit_adv = MMIMMS_Setting_GetAdvertising();
    if (TRUE == if_permit_adv)
    {
        if_permit_adv_cur_index = 1;
    }
    else
    {
        if_permit_adv_cur_index = 0;
    }
    if_permit_rpt = MMIMMS_Setting_GetRecvDeliveryReport();
    if (TRUE == if_permit_rpt)
    {
        if_permit_rpt_cur_index = 1;
    }
    else
    {
        if_permit_rpt_cur_index = 0;
    }
    //todo get if_permit_rpt

#ifdef DATA_ROAMING_SUPPORT
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM2; index++)
    {
        MMI_GetLabelTextByLang(mode_text[index], &mode_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_RETRIEVEMODE_LOCAL_DROPDOWNLIST_CTRL_ID, mode_dropdownlist, MMS_SETTING_DROPPAGE_NUM2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_RETRIEVEMODE_LOCAL_DROPDOWNLIST_CTRL_ID, cur_mode_local);
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_RETRIEVEMODE_ROAM_DROPDOWNLIST_CTRL_ID, mode_dropdownlist, MMS_SETTING_DROPPAGE_NUM2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_RETRIEVEMODE_ROAM_DROPDOWNLIST_CTRL_ID, cur_mode_roam);
#else
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        MMI_GetLabelTextByLang(aoto_text[index], &aotodown_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID, aotodown_dropdownlist, MMS_SETTING_DROPPAGE_NUM1);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID, if_autodown_cur_index);
#endif

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        MMI_GetLabelTextByLang(adv_text[index], &adv_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID, adv_dropdownlist, MMS_SETTING_DROPPAGE_NUM1);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID, if_permit_adv_cur_index);

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        MMI_GetLabelTextByLang(sendrpt_text[index], &rpt_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID, rpt_dropdownlist, MMS_SETTING_DROPPAGE_NUM1);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID, if_permit_rpt_cur_index);    
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :Set mms send setting dropdownlist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetSendDropDownList( void )
{
    uint8 index                                                              = 0;
    MMIMMS_SETTING_VALID_PERIOD_E valid_period                               = MMIMMS_SETTING_VALID_PERIOD_LONGEST;
    MMIMMS_SETTING_PRIORITY_E priority                                       = MMIMMS_SETTING_PRIORITY_NORMAL;
    BOOLEAN if_have_send_report                                              = FALSE;
    BOOLEAN if_have_read_report                                              = FALSE;
    BOOLEAN if_anonymity_send                                                = FALSE;
    MMI_TEXT_ID_T  period_text[MMS_SETTING_DROPPAGE_NUM3]                = {TXT_COMM_1_HOUR,TXT_COMM_12_HOURS, TXT_COMM_24_HOURS,TXT_MMS_ONE_WEEK,TXT_MMS_LONGGEST_DURATION};
    MMI_TEXT_ID_T  priority_text[MMS_SETTING_DROPPAGE_NUM2]              = {TXT_COMMON_HIGH, TXT_COMMON_MID,TXT_COMM_LOW};
    MMI_TEXT_ID_T  sendrpt_text[MMS_SETTING_DROPPAGE_NUM1]               = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  readrpt_text[MMS_SETTING_DROPPAGE_NUM1]               = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  anonymity_text[MMS_SETTING_DROPPAGE_NUM1]             = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};

    valid_period = MMIMMS_Setting_GetValidPeriod();
    priority = MMIMMS_Setting_GetPriority();
    if_have_send_report = MMIMMS_Setting_GetDeliveryReport();
    if_have_read_report = MMIMMS_Setting_GetReadReport();
    if_anonymity_send = MMIMMS_Setting_GetAnonymous();
    
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM3; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID,period_text[index]);
    }
    GUISETLIST_SetCurIndex(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID, valid_period);
   
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM2; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID,priority_text[index]);
    }
    GUISETLIST_SetCurIndex(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID, priority);

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID,sendrpt_text[index]);
    }
    GUISETLIST_SetCurIndex(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID, if_have_send_report);
   
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID,readrpt_text[index]);
    }
    GUISETLIST_SetCurIndex(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID, if_have_read_report);

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        GUISETLIST_AddItemById(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID,anonymity_text[index]);
    }
    GUISETLIST_SetCurIndex(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID, if_anonymity_send);
}
#else
/*****************************************************************************/
//  Description :Set mms send setting dropdownlist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetSendDropDownList( void )
{
    uint8 index                                                              = 0;
    MMIMMS_SETTING_VALID_PERIOD_E valid_period                               = MMIMMS_SETTING_VALID_PERIOD_LONGEST;
    MMIMMS_SETTING_PRIORITY_E priority                                       = MMIMMS_SETTING_PRIORITY_NORMAL;
    BOOLEAN if_have_send_report                                              = FALSE;
    BOOLEAN if_have_read_report                                              = FALSE;
    BOOLEAN if_anonymity_send                                                = FALSE;
    MMI_STRING_T period_dropdownlist[MMS_SETTING_DROPPAGE_NUM3]    = {0};
    MMI_STRING_T priority_dropdownlist[MMS_SETTING_DROPPAGE_NUM2]  = {0};
    MMI_STRING_T sendrpt_dropdownlist[MMS_SETTING_DROPPAGE_NUM1]   = {0};
    MMI_STRING_T readrpt_dropdownlist[MMS_SETTING_DROPPAGE_NUM1]   = {0};
    MMI_STRING_T anonymity_dropdownlist[MMS_SETTING_DROPPAGE_NUM1] = {0};
    MMI_TEXT_ID_T  period_text[MMS_SETTING_DROPPAGE_NUM3]                = {TXT_COMM_1_HOUR,TXT_COMM_12_HOURS, TXT_COMM_24_HOURS,TXT_MMS_ONE_WEEK,TXT_MMS_LONGGEST_DURATION};
    MMI_TEXT_ID_T  priority_text[MMS_SETTING_DROPPAGE_NUM2]              = {TXT_COMMON_HIGH,TXT_COMMON_MID,TXT_COMM_LOW};
    MMI_TEXT_ID_T  sendrpt_text[MMS_SETTING_DROPPAGE_NUM1]               = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  readrpt_text[MMS_SETTING_DROPPAGE_NUM1]               = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};
    MMI_TEXT_ID_T  anonymity_text[MMS_SETTING_DROPPAGE_NUM1]             = {TXT_CLOSE, TXT_COMMON_ENABLE_ON};

    valid_period = MMIMMS_Setting_GetValidPeriod();
    priority = MMIMMS_Setting_GetPriority();
    if_have_send_report = MMIMMS_Setting_GetDeliveryReport();
    if_have_read_report = MMIMMS_Setting_GetReadReport();
    if_anonymity_send = MMIMMS_Setting_GetAnonymous();
    
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM3; index++)
    {
        MMI_GetLabelTextByLang(period_text[index], &period_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID, period_dropdownlist, MMS_SETTING_DROPPAGE_NUM3);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID, (uint16)valid_period );
    
    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM2; index++)
    {
        MMI_GetLabelTextByLang(priority_text[index], &priority_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID, priority_dropdownlist, MMS_SETTING_DROPPAGE_NUM2);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID, (uint16)priority );

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        MMI_GetLabelTextByLang(sendrpt_text[index], &sendrpt_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID, sendrpt_dropdownlist, MMS_SETTING_DROPPAGE_NUM1);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID, (uint16)if_have_send_report );

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        MMI_GetLabelTextByLang(readrpt_text[index], &readrpt_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID, readrpt_dropdownlist, MMS_SETTING_DROPPAGE_NUM1);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID, (uint16)if_have_read_report );

    for (index = 0; index < MMS_SETTING_DROPPAGE_NUM1; index++)
    {
        MMI_GetLabelTextByLang(anonymity_text[index], &anonymity_dropdownlist[index]);
    }
    GUIDROPDOWNLIST_AppendItemArray(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID, anonymity_dropdownlist, MMS_SETTING_DROPPAGE_NUM1);
    GUIDROPDOWNLIST_SetCurItemIndex(MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID, (uint16)if_anonymity_send );
}
#endif
//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description :handle retrieve mode win msg
//  Global resource dependence : 
//  Author: feng.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRetrieveModeWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E	res = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	ctrl_id = MMIMMS_SETTING_RETRIEVE_MODE_CTRL_ID;
    MMI_TEXT_ID_T  mode_text[MMS_SETTING_DROPPAGE_NUM2] = {TXT_MMS_SET_RETRIEVE_MODE_IMMEDIATE,
                                                               TXT_MMS_RETRIEVE_MANUAL,
                                                               TXT_MMS_SET_RETRIEVE_MODE_DENY};
    uint16 cur_index = 0;
    uint32 add_data = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMIMMS_SETTING_RETRIEVE_MODE_E mode = MMIMMS_SETTING_RETRIEVE_MODE_DEFER;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int i = 0;
            MMI_STRING_T text_str = {0};
            
            GUILIST_SetMaxItem(ctrl_id, MMS_SETTING_DROPPAGE_NUM2, FALSE);
            for (i = 0; i < MMS_SETTING_DROPPAGE_NUM2; i++)
            {                
                MMI_GetLabelTextByLang(mode_text[i], &text_str);
                MMIMMS_AppendOneLineTextListItem(ctrl_id,
                                                 GUIITEM_STYLE_ONE_LINE_RADIO,
                                                 TXT_COMMON_OK,  
                                                 0,  
                                                 text_str.wstr_ptr, 
                                                 text_str.wstr_len, 
                                                 (uint16)i, 
                                                 FALSE, 0); 
            }

            if(MMISMS_PDA_MMS_RETRIEVE_MODE_LOCAL == add_data)
            {   
                GUIWIN_SetTitleTextId(win_id, TXT_MMS_SET_RETRIEVE_LOCAL, FALSE);
                mode = MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_LOCAL);
            }
            else if(MMISMS_PDA_MMS_RETRIEVE_MODE_ROAM == add_data)
            {
                GUIWIN_SetTitleTextId(win_id, TXT_MMS_SET_RETRIEVE_ROAM, FALSE);
                mode = MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_ROAM);
            }

            if (MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE == mode)
            {
                cur_index = 0;
            }
            else if (MMIMMS_SETTING_RETRIEVE_MODE_DEFER == mode )
            {
                cur_index = 1;
            }
            else if (MMIMMS_SETTING_RETRIEVE_MODE_DENY == mode)
            {
                cur_index = 2;
            }
            else
            {
                cur_index = 0;
            }

            GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_index);	
            break;
        }
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
		{
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            if (cur_index < MMIMMS_SETTING_RETRIEVE_MODE_MAX)
            {
                if (0 == cur_index)
                {
                    mode = MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE;
                }
                else if (1 == cur_index)
                {
                    mode = MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
                }
                else if (2 == cur_index)
                {
                    mode = MMIMMS_SETTING_RETRIEVE_MODE_DENY;
                }

                if(MMISMS_PDA_MMS_RETRIEVE_MODE_LOCAL == add_data)
                {
                    MMIMMS_Setting_SetRetrieveMode(mode, MMIMMS_SETTING_TYPE_LOCAL);
                }
                else if(MMISMS_PDA_MMS_RETRIEVE_MODE_ROAM == add_data)
                {
                    MMIMMS_Setting_SetRetrieveMode(mode, MMIMMS_SETTING_TYPE_ROAM);
                }
            }
            MMK_CloseWin(win_id);
			break;
		}
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
    default:
        {
            res = MMI_RESULT_FALSE;
            break;
        }
    }
    return res;
}
/*****************************************************************************/
//  Description : get cur retrieve mode from setting drop list
//  Global resource dependence : 
//  Author: feng.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_SETTING_RETRIEVE_MODE_E GetRetrieveModeFromDropList(uint16 index)
{
    if (0 == index)
    {
        return MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE;
    }
    else if (1 == index)
    {
        return MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
    }
    else
    {
        return MMIMMS_SETTING_RETRIEVE_MODE_DENY;
    }
}
#endif


/*Edit by script, ignore 11 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
