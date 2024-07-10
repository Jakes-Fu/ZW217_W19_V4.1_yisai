/*****************************************************************************
** File Name:               mmiapwin_set.c                                   *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAPWIN_SET_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmiacc_event.h"
#include "guilistbox.h"
#include "mmialarm_export.h"
#include "mmipub.h"

#include "mmiap_list_control.h"
#include "mmiap_applet.h"
#include "mmiap_control.h"
#include "mmiapwin_set.h"
#include "mmiap_list.h"
#include "mmiap_play.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmiap_setting.h"

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#include "mmiap_common.h"

#ifdef BLUETOOTH_SUPPORT
#include "mmimp3_earphone.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
//PDA风格UI
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : get text_id by Repeat mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByRepeatMode(MMIMP3_REPEAT_MODE_E repeat_mode);

/*****************************************************************************/
//  Description : get text_id by shuffle mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByShuffleMode(MMIMP3_SHUFFLE_MODE_E shuffle_mode);

#else

/*****************************************************************************/
//  Description : get text_id by display style
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByDisplayMode(MMIMP3_DISPLAY_STYLE_E display_style);

#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : handle settings window message
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3SettingsWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : init settings list ctrl
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void InitSettingsListCtrl(void);

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendSettingsItem(
    MMI_CTRL_ID_T  ctrl_id,    //控件ID
    MMI_TEXT_ID_T  text_1,     //主文本
    MMI_TEXT_ID_T  text_2,     //副文本
    MMI_IMAGE_ID_T image_id,   //图标ID
    uint32         user_data   //用户数据
);

/*****************************************************************************/
//  Description : get text_id by EQ mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByEQMode(MMISRVAUD_EQ_MODE_E eq_mode);

/*****************************************************************************/
//  Description : get text_id by bt mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByBTMode(MMIMP3_BT_SETTING_TYPE bt_mode);

/*****************************************************************************/
//  Description : get text_id by bt mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByCodeType(MMIMP3_TXT_CODE_TYPE_E code_type);

/*****************************************************************************/
//  Description : handle eq set window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3EqSetWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : append mp3 eq set item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendEqSetItem(
    MMI_CTRL_ID_T    ctrl_id
);

/*****************************************************************************/
//  Description : Set mp3 as alarm ring
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3AsAlarmRing(
    uint16 alarm_id,
    MMIMP3_FILE_INFO_T *file_info_ptr
);

/*****************************************************************************/
//  Description : handle alarm ring set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3AlarmRingSetWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendRepeatModeListItem(
    MMI_CTRL_ID_T       ctrl_id
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendShuffleModeListItem(
    MMI_CTRL_ID_T       ctrl_id
);

/*****************************************************************************/
//  Description : handle play mode set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3RepeatModeWin(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : handle play mode set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3ShuffleModeWin(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : handle display set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3DisplaySetWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : append mp3 lyric set item
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendDisplaySetItem(
    MMI_CTRL_ID_T    ctrl_id
);

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : Append Bt Setting Item
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendBtSettingItem(
    MMI_CTRL_ID_T       ctrl_id
);

/*****************************************************************************/
//  Description : Handle Mp3 Bt Mode Set Win message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3BtModeSetWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : Handle Mp3 Bt Headset Win Msg
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3BtHeadsetWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : Set Bt Headset Name
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL void SetBtHeadsetName(
    BOOLEAN  is_update
);
#endif//#ifdef BLUETOOTH_SUPPORT  


/*****************************************************************************/
//  Description : 编码方式设置界面
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCodeType(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : 编码方式选项:1、GB(简体中文) 2、Big5(繁体中文)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendCodeTypeListItem(
    MMI_CTRL_ID_T      ctrl_id
);

/*****************************************************************************/
//  Description : 设置编码方式
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetCodeType(
    MMI_CTRL_ID_T      ctrl_id
);


/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
// the window for mp3 eq mode setting
LOCAL WINDOW_TABLE(MMIMP3_EQ_SET_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_COMMON_EQ_SET),
    WIN_FUNC((uint32)HandleMp3EqSetWin),
    WIN_ID(MMIMP3_EQ_SET_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_EQ_SET_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

// the window for mp3 alarm ring setting
LOCAL WINDOW_TABLE(MMIMP3_SET_ALARM_RING_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MP3_SET_ALARM_RING),
    WIN_FUNC((uint32)HandleMp3AlarmRingSetWinMsg),
    WIN_ID(MMIMP3_ALARM_RING_SET_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_ALARM_RING_SET_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

// the window for mp3 display setting
LOCAL WINDOW_TABLE(MMIMP3_DISPLAY_SET_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_DISPLAY_SET),
    WIN_FUNC((uint32)HandleMp3DisplaySetWin),
    WIN_ID(MMIMP3_DISPLAY_SET_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_DISPLAY_SET_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_REPEAT_MODE_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_COMMON_REPEAT),
    WIN_FUNC((uint32)HandleMp3RepeatModeWin),
    WIN_ID(MMIMP3_REPEAT_MODE_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_PLAY_MODE_REPEAT_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_SHUFFLE_MODE_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
#ifndef CMCC_UI_STYLE
    WIN_TITLE(TXT_SET_SHUFFLE),
#else
    WIN_TITLE(TXT_MP3_PLAY_SEQUENCE),
#endif
    WIN_FUNC((uint32)HandleMp3ShuffleModeWin),
    WIN_ID(MMIMP3_SHUFFLE_MODE_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_PLAY_MODE_SHUFFLE_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#ifdef BLUETOOTH_SUPPORT
LOCAL WINDOW_TABLE(MMIMP3_BT_SET_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MP3_BT_HEADSET_SET),
    WIN_FUNC((uint32)HandleMp3BtModeSetWin),
    WIN_ID(MMIMP3_BT_SET_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_BT_SET_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMP3_BT_HEADSET_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MP3_BT_HEADSET_LIST),
    WIN_FUNC((uint32)HandleMp3BtHeadsetWinMsg),
    WIN_ID(MMIMP3_BT_HEADSET_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, STXT_SELECT, TXT_NULL),
#endif

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMP3_BT_SET_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIMP3_BT_SET_LABEL_CTRL_ID, MMIMP3_BT_SET_FORM_CTRL_ID),

    END_WIN
};
#endif


LOCAL WINDOW_TABLE(MMIMP3_SET_TXT_CODE_TYPE_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_CODING_TYPE),
    WIN_FUNC((uint32)HandleSetCodeType),
    WIN_ID(MMIMP3_SET_CODE_TYPE_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMP3_SET_TXT_CODE_TYPE_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};


LOCAL WINDOW_TABLE(MMIMP3_SETTINGS_WIN_TAB) =
{
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_FUNC((uint32)HandleMp3SettingsWinMsg),
    WIN_ID(MMIMP3_SETTINGS_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMP3_SETTINGS_LIST_CTRL_ID),
    //CR NEWMS00118738
    //WIN_STYLE(WS_HAS_TRANSPARENT),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

//PDA风格UI
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : get text_id by Repeat mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByRepeatMode(MMIMP3_REPEAT_MODE_E repeat_mode)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    switch(repeat_mode)
    {
    case MMIMP3_REPEAT_OFF:
        text_id = TXT_CLOSE;
        break;

    case MMIMP3_REPEAT_ALL:
        text_id = TXT_COMMON_SET_REPEAT_ALL;
        break;

    case MMIMP3_REPEAT_ONE:
        text_id = TXT_COMM_REPEAT_ONE;
        break;

    default:
        break;
    }

    return text_id;
}

/*****************************************************************************/
//  Description : get text_id by shuffle mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByShuffleMode(MMIMP3_SHUFFLE_MODE_E shuffle_mode)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    switch(shuffle_mode)
    {
    case MMIMP3_SHUFFLE_ON:
        text_id = TXT_OPEN;
        break;

    case MMIMP3_SHUFFLE_OFF:
        text_id = TXT_CLOSE;
        break;

    default:
        break;
    }

    return text_id;
}

#else

/*****************************************************************************/
//  Description : get text_id by display style
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByDisplayMode(MMIMP3_DISPLAY_STYLE_E display_style)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    switch(display_style)
    {
    case MMIMP3_DISPLAY_NONE:
        text_id = TXT_COMMON_COMMON_NORMAL;
        break;
#ifdef MMIAP_LYRIC_SUPPORT

    case MMIMP3_DISPLAY_LYRIC_ON:
        text_id = TXT_MP3_LYRIC_ON;
        break;

    case MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT:
        text_id = TXT_MP3_LYRIC_ON_WITH_BACKLIGHT;
        break;
#endif
#ifdef MMIAP_SPECTRUM_SUPPORT

    case MMIMP3_DISPLAY_SPECTRUM:
        text_id = TXT_MP3_SPECTRUM;
        break;
#endif

#ifdef MMI_AUDIOPLAYER_ANIM_SUPPORT

    case MMIMP3_DISPLAY_ANIM:
        text_id = TXT_COMM_ANIMATION;
        break;
#endif

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT

    case MMIMP3_DISPLAY_ALBUM:
        text_id = TXT_MP3_ALBUM;
        break;
#endif

    default:
        break;
    }

    return text_id;
}

#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : handle settings window message
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3SettingsWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_SETTINGS_LIST_CTRL_ID;
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
#ifdef BLUETOOTH_SUPPORT
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;
    BOOLEAN is_ok = FALSE;
    MMIMP3_BT_SETTING_TYPE bt_setting = 0;
#endif

    uint16              index = 0;
    uint32              user_data = 0;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitSettingsListCtrl();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_GET_FOCUS:
    case MSG_MP3_UPDATE_LIST:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        InitSettingsListCtrl();
        GUILIST_SetCurItemIndex(ctrl_id, index);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &user_data);

        switch((MMIMP3_SETTINGS_TYPE_E)user_data)
        {
#ifdef MMI_PDA_SUPPORT

        case MMIMP3_SETTINGS_REPEAT:
            MMIAPSETWIN_OpenRepeatModeSetWin();
            break;

        case MMIMP3_SETTINGS_SHUFFLE:

            //MMIAPSETWIN_OpenShuffleModeSetWin();
            if(MMIMP3_SHUFFLE_ON == APSETTING_GetShuffleMode())
            {
                MMIMP3_SetShuffleMode(MMIMP3_SHUFFLE_OFF);
                MMK_PostMsg(win_id, MSG_MP3_UPDATE_LIST, PNULL, NULL);
            }
            else
            {
                MMIMP3_SetShuffleMode(MMIMP3_SHUFFLE_ON);
                MMK_PostMsg(win_id, MSG_MP3_UPDATE_LIST, PNULL, NULL);
            }

            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
#else

        case MMIMP3_SETTINGS_DISPLAY:
            MMIAPSETWIN_OpenDisplaySetWin();
            //MMIAPSETWIN_OpenRepeatModeSetWin();
            break;
#endif

        case MMIMP3_SETTINGS_EQ:
            MMIAPSETWIN_OpenEQSetWin();
            break;
#ifdef BLUETOOTH_SUPPORT

        case MMIMP3_SETTINGS_BT:

            bt_setting = MMIMP3_GetBtSettingMode();
            if(MMIMP3_BT_MODE_ON == bt_setting)
            {
                bt_setting = MMIMP3_BT_MODE_OFF;

                MMIMP3_SetBtSettingMode(bt_setting);

                SCI_TRACE_LOW("[MMIAP] HandleMp3SettingsWinMsg to OFF bt_setting=%d",\
                              bt_setting);

                MMIAP_TrySetBtAudio(FALSE);

                MMK_PostMsg(win_id, MSG_MP3_UPDATE_LIST, PNULL, NULL);
            }
            else
            {
                if(MMIAPIBT_GetActiveBTHeadset())
                {
                    is_ok = MMIAP_TrySetBtAudio(TRUE);
                    if (is_ok)
                    {
                        bt_setting = MMIMP3_BT_MODE_ON;
                    }

                    MMIMP3_SetBtSettingMode(bt_setting);

                    SCI_TRACE_LOW("[MMIAP] HandleMp3SettingsWinMsg to ON bt_setting=%d",\
                                  bt_setting);

                    MMK_PostMsg(win_id, MSG_MP3_UPDATE_LIST, PNULL, NULL);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_BT_HEADSET_MUST_SELECT, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                    break;
                }
            }

            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;

        case MMIMP3_SETTINGS_BT_HEADSET:
            MMIAPSETWIN_OpenBTHeadSetWin();
            break;
#endif

        case MMIMP3_SETTINGS_CODE_TYPE:
            MMIAPSETWIN_OpenTxtCodeTypeSetWin();
            break;

        default:
            break;
        }

        break;

    case MSG_APP_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_KEYDOWN_RED:
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init settings list ctrl
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void InitSettingsListCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMIMP3_SETTINGS_LIST_CTRL_ID;
    //GUILIST_ITEM_T      item_t    = {0};   /*lint !e64*/
    //GUILIST_ITEM_DATA_T item_data = {0};   /*lint !e64*/
#if defined(BLUETOOTH_SUPPORT) || defined(MMI_PDA_SUPPORT)
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
#endif


    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMIMP3_SETTINGS_MAX, FALSE);

    //append items
#ifndef MMI_AUDIOPLAYER_MINI_SUPPORT
#ifdef MMI_PDA_SUPPORT

    //REPEAT
    AppendSettingsItem(
        ctrl_id,
        TXT_COMMON_REPEAT,
        GetTextIdByRepeatMode(APSETTING_GetRepeatMode()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIMP3_SETTINGS_REPEAT);

    //SHUFFLE
    if(MMIMP3_SHUFFLE_ON == APSETTING_GetShuffleMode())
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    AppendSettingsItem(
        ctrl_id,
        TXT_SET_SHUFFLE,
        GetTextIdByShuffleMode(APSETTING_GetShuffleMode()),
        image_id,
        (uint32)MMIMP3_SETTINGS_SHUFFLE);
#else
    AppendSettingsItem(
        ctrl_id,
        TXT_DISPLAY_SET,
        GetTextIdByDisplayMode(APSETTING_GetDisplayStyle()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIMP3_SETTINGS_DISPLAY);
#endif
#endif

    //EQ
#ifdef MP3_EQ_SUPPORT
    AppendSettingsItem(
        ctrl_id,
        TXT_COMMON_EQ_SET,
        GetTextIdByEQMode(APSETTING_GetEqMode()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIMP3_SETTINGS_EQ);
#endif

#ifdef BLUETOOTH_SUPPORT

    //BT
    // 先检查bt连接状态，
    // 再设置显示图片
    MMIMP3_CheckBtSetting();

    if(MMIMP3_BT_MODE_ON == MMIMP3_GetBtSettingMode())
    {
        image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }

    AppendSettingsItem(
        ctrl_id,
        TXT_MP3_BT_HEADSET_SET,
        GetTextIdByBTMode(MMIMP3_GetBtSettingMode()),
        image_id,
        (uint32)MMIMP3_SETTINGS_BT);

    //BT_HEADSET
    AppendSettingsItem(
        ctrl_id,
        TXT_MP3_BT_HEADSET_LIST,
        TXT_NULL,
        IMAGE_NULL,
        (uint32)MMIMP3_SETTINGS_BT_HEADSET);
#endif

    //CODE_TYPE
#ifdef BIG5_SUPPORT
    AppendSettingsItem(
        ctrl_id,
        TXT_CODING_TYPE,
        GetTextIdByCodeType(APSETTING_GetCodeType()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIMP3_SETTINGS_CODE_TYPE);
#endif
}

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendSettingsItem(
    MMI_CTRL_ID_T  ctrl_id,    //控件ID
    MMI_TEXT_ID_T  text_1,     //主文本
    MMI_TEXT_ID_T  text_2,     //副文本
    MMI_IMAGE_ID_T image_id,   //图标ID
    uint32         user_data   //用户数据
)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    if(TXT_NULL != text_2)
    {
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;

        //text 1
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_1;
        //text 2
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = text_2;

        //anim
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.text_id = image_id;
    }
    else
    {
        //sigle line style
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_1;
    }

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : get text_id by EQ mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByEQMode(MMISRVAUD_EQ_MODE_E eq_mode)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    switch(eq_mode)
    {
    case MMISRVAUD_EQ_REGULAR:
        text_id = TXT_COMM_REGULAR;
        break;

    case MMISRVAUD_EQ_CLASSIC:
        text_id = TXT_COMMON_CLASSICAL;
        break;

    case MMISRVAUD_EQ_ODEUM:
        text_id = TXT_ODEUM_EQ;
        break;

    case MMISRVAUD_EQ_JAZZ:
        text_id = TXT_COMMON_EQ_JAZZ;
        break;

    case MMISRVAUD_EQ_ROCK:
        text_id = TXT_ROCK_EQ;
        break;

    case MMISRVAUD_EQ_SOFT_ROCK:
        text_id = TXT_COMMON_SOFT_ROCK_EQ;
        break;

    default:
        break;
    }

    return text_id;
}

/*****************************************************************************/
//  Description : get text_id by bt mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByBTMode(MMIMP3_BT_SETTING_TYPE bt_mode)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    switch(bt_mode)
    {
    case MMIMP3_BT_MODE_ON:
        text_id = TXT_OPEN;
        break;

    case MMIMP3_BT_MODE_OFF:
    case MMIMP3_BT_MODE_NONE:
        text_id = TXT_CLOSE;
        break;

    default:
        break;
    }

    return text_id;
}

/*****************************************************************************/
//  Description : get text_id by bt mode
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByCodeType(MMIMP3_TXT_CODE_TYPE_E code_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;

    switch(code_type)
    {
    case MMIMP3_TXT_CODE_TYPE_GB:
        text_id = TXT_CODING_TYPE_GB;
        break;

    case MMIMP3_TXT_CODE_TYPE_BIG5:
        text_id = TXT_CODING_TYPE_BIG5;
        break;

    default:
        break;
    }

    return text_id;
}


/*****************************************************************************/
//  Description : handle eq set window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3EqSetWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_EQ_SET_LIST_CTRL_ID;

#ifdef BLUETOOTH_SUPPORT
    MMI_WIN_ID_T    alert_win_id = MMIMP3_ALERT_WIN_ID;
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendEqSetItem(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

        //   case MSG_CTL_MIDSK: //masked for cr247655
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //    case MSG_APP_WEB:  //masked for cr247655
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
#ifdef BLUETOOTH_SUPPORT

        if(MMIAPIMP3_IsThroughBt())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_BT_NOT_SUPPORT_EQ, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

            cur_selection = (uint16)APSETTING_GetEqMode();
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        }
        else
#endif
        {
            APSETTING_SetEqMode((MMISRVAUD_EQ_MODE_E)cur_selection);
            MMIAPIMP3_EnableEq((MMISRVAUD_EQ_MODE_E)cur_selection);
            //xin.li_clean_popwin
            //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,
            // IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

            MMK_CloseWin(win_id);
        }

        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_NOTIFY_LIST_PREPAGE:
    case MSG_NOTIFY_LIST_NXTPAGE:
    case MSG_NOTIFY_LIST_MOVEBOTTOM:
    case MSG_NOTIFY_LIST_MOVETOP:
    case MSG_NOTIFY_LIST_MOVEDOWN:
    case MSG_NOTIFY_LIST_MOVEUP:
    case MSG_NOTIFY_LIST_HEAD:
    case MSG_NOTIFY_LIST_TAIL:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
#ifdef BLUETOOTH_SUPPORT

        if(MMIAPIMP3_IsThroughBt())
        {
            //nothing
        }
        else
#endif
        {
            //add for CR253535
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
            //add for CR253535
            MMIAPIMP3_EnableEq((MMISRVAUD_EQ_MODE_E)cur_selection);
        }

        break;

    case MSG_CLOSE_WINDOW:
#ifdef BLUETOOTH_SUPPORT
        if(MMIAPIMP3_IsThroughBt())
        {
            //nothing
        }
        else
#endif
        {
            MMIAPIMP3_EnableEq(APSETTING_GetEqMode());
        }

        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set mp3 as alarm ring
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMp3AsAlarmRing(
    uint16 alarm_id,
    MMIMP3_FILE_INFO_T *file_info_ptr
)
//uint16        *music_full_path_name,
//uint16        music_full_path_len,
{


    MMIMP3_LIST_FILE_BODY_T selected_file = {0};

    if(PNULL != file_info_ptr)
    {
        MMIAPIFMM_SetMusicToRing(
            MMIMULTIM_SET_ALARM_RING, (DPARAM)&alarm_id,
            file_info_ptr->file_name, file_info_ptr->file_name_len
        );
    }
    else
    {
        selected_file.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
        if(MMIMP3_GetCurPlayFileFullPath(selected_file.full_path_name,&(selected_file.full_path_len)))
        {
            MMIAPIFMM_SetMusicToRing(
                MMIMULTIM_SET_ALARM_RING, (DPARAM)&alarm_id,
                selected_file.full_path_name, selected_file.full_path_len
            );
        }
    }
}

/*****************************************************************************/
//  Description : handle alarm ring set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3AlarmRingSetWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_ALARM_RING_SET_LIST_CTRL_ID;
    uint16 i = 0;
    /*MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10
                                            };*/
    MMI_STRING_T str_ptr = {0};
    wchar   str_content[MMIACC_EVENT_TEXT_MAX_LEN + 1] = {0};
    MMIMP3_FILE_INFO_T  *file_info_ptr = (MMIMP3_FILE_INFO_T *)MMK_GetWinUserData(win_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, ALM_ALARM_NUM, FALSE);

        for(i = 0; i < ALM_ALARM_NUM; i++)
        {
            str_ptr.wstr_ptr = str_content;
            str_ptr.wstr_len = MMIACC_EVENT_TEXT_MAX_LEN;
            MMIAPIALM_GetAlarmName(i, &str_ptr);
            //MMIAPISET_AppendListItemByTextId(txt_alarm_id[i],TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
            MMIAPISET_AppendListItemByTextBuffer(str_ptr.wstr_ptr, str_ptr.wstr_len, 0, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
        SetMp3AsAlarmRing(cur_selection,file_info_ptr);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != file_info_ptr)
        {
            SCI_FREE(file_info_ptr);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : append mp3 eq set item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendEqSetItem(
    MMI_CTRL_ID_T    ctrl_id
)
{
    uint16  cur_selection = 0;

    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMISRVAUD_EQ_MODE_MAX, FALSE);

    //append items
    MMIAPISET_AppendListItemByTextId(TXT_COMM_REGULAR, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_CLASSICAL, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_ODEUM_EQ, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_EQ_JAZZ, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_ROCK_EQ, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_SOFT_ROCK_EQ, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

    //get current eq set
    cur_selection = (uint16)APSETTING_GetEqMode();

    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : handle play mode set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3RepeatModeWin(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_PLAY_MODE_REPEAT_CTRL_ID;
    uint16          cur_selection = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendRepeatModeListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

        MMIMP3_SetRepeatMode((MMIMP3_REPEAT_MODE_E)cur_selection);

        //success!
        //xin.li_clean_popwin
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle play mode set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3ShuffleModeWin(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_PLAY_MODE_SHUFFLE_CTRL_ID;
    uint16          cur_selection = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendShuffleModeListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

        MMIMP3_SetShuffleMode((MMIMP3_SHUFFLE_MODE_E)cur_selection);

        //success!
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        MMK_CloseWin(win_id);
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
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendRepeatModeListItem(
    MMI_CTRL_ID_T       ctrl_id
)
{
    uint16  cur_selection = 0;

    GUILIST_SetMaxItem(ctrl_id, MMIMP3_REPEAT_MAX, FALSE);

    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_SET_REPEAT_ALL, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_REPEAT_ONE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

    //get current lyric set
    switch(APSETTING_GetRepeatMode())
    {
    case MMIMP3_REPEAT_OFF:
        cur_selection = MMIMP3_MENU_INDEX_0;
        break;

    case MMIMP3_REPEAT_ALL:
        cur_selection = MMIMP3_MENU_INDEX_1;
        break;

    case MMIMP3_REPEAT_ONE:
        cur_selection = MMIMP3_MENU_INDEX_2;
        break;

    default:
        break;
    }

    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AppendShuffleModeListItem(
    MMI_CTRL_ID_T       ctrl_id
)
{
    uint16  cur_selection = 0;

    GUILIST_SetMaxItem(ctrl_id, MMIMP3_SHUFFLE_MAX, FALSE);

#ifndef CMCC_UI_STYLE
    MMIAPISET_AppendListItemByTextId(TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#else
    MMIAPISET_AppendListItemByTextId(TXT_SET_SHUFFLE,    TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_MP3_SET_ORDER_PLAY, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif

    switch(APSETTING_GetShuffleMode())
    {
    case MMIMP3_SHUFFLE_ON:
        cur_selection = MMIMP3_MENU_INDEX_0;
        break;

    case MMIMP3_SHUFFLE_OFF:
        cur_selection = MMIMP3_MENU_INDEX_1;
        break;

    default:
        break;
    }

    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : handle display set window message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3DisplaySetWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_DISPLAY_SET_CTRL_ID;
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    MMIMP3_DISPLAY_STYLE_E  style = MMIMP3_DISPLAY_ALBUM;
#else
    MMIMP3_DISPLAY_STYLE_E  style = MMIMP3_DISPLAY_NONE;
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendDisplaySetItem(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

        if(cur_selection < MMIMP3_DISPLAY_TYPE_MAX)
        {
            style = (MMIMP3_DISPLAY_STYLE_E)cur_selection;
        }

        MMIMP3_SetDisplayStyle(style);

        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,
        //IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        MMK_CloseWin(win_id);
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
//  Description : append mp3 lyric set item
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendDisplaySetItem(
    MMI_CTRL_ID_T    ctrl_id
)
{
    uint16  cur_selection = 0;
    MMIMP3_DISPLAY_STYLE_E  style = APSETTING_GetDisplayStyle();

    GUILIST_SetMaxItem(ctrl_id, MMIMP3_DISPLAY_TYPE_MAX, FALSE);

    MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_NORMAL,           TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#ifdef MMIAP_LYRIC_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_MP3_LYRIC_ON,                 TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_MP3_LYRIC_ON_WITH_BACKLIGHT,  TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_MP3_SPECTRUM,                 TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef MMI_AUDIOPLAYER_ANIM_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_COMM_ANIMATION,                     STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    MMIAPISET_AppendListItemByTextId(TXT_MP3_ALBUM,                    TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif

    switch(style)
    {
    case MMIMP3_DISPLAY_NONE:
#ifdef MMIAP_LYRIC_SUPPORT
    case MMIMP3_DISPLAY_LYRIC_ON:
    case MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT:
#endif
#ifdef MMIAP_SPECTRUM_SUPPORT
    case MMIMP3_DISPLAY_SPECTRUM:
#endif
#ifdef MMI_AUDIOPLAYER_ANIM_SUPPORT
    case MMIMP3_DISPLAY_ANIM:
#endif
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    case MMIMP3_DISPLAY_ALBUM:
#endif
        cur_selection = (uint16)style;
        break;

    default:
        break;
    }

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : Append Bt Setting Item
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendBtSettingItem(
    MMI_CTRL_ID_T       ctrl_id
)
{
    uint16  cur_selection = 0;

    GUILIST_SetMaxItem(ctrl_id, MMIMP3_BT_SETTING_MAX, FALSE);

    MMIAPISET_AppendListItemByTextId(TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);


    switch(MMIMP3_GetBtSettingMode())
    {
    case MMIMP3_BT_MODE_ON:
		if(MMIAPIBT_GetActiveBTHeadset())
		{
       		 cur_selection = MMIMP3_MENU_INDEX_0;
		}
		else
		{
			cur_selection = MMIMP3_MENU_INDEX_1;
		}
        break;

    case MMIMP3_BT_MODE_OFF:
    case MMIMP3_BT_MODE_NONE:
        cur_selection = MMIMP3_MENU_INDEX_1;
        break;

    default:
        cur_selection = MMIMP3_MENU_INDEX_1;
        break;
    }

    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : Handle Mp3 Bt Mode Set Win message
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3BtModeSetWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_BT_SET_CTRL_ID;
    MMI_WIN_ID_T    alert_win_id = MMIMP3_ALERT_WIN_ID;
    BOOLEAN is_ok = FALSE;
    MMIMP3_BT_SETTING_TYPE bt_setting = 0;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMP3_CheckBtSetting();

        AppendBtSettingItem(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

        switch(cur_selection)
        {
        case MMIMP3_MENU_INDEX_0:
            bt_setting = MMIMP3_GetBtSettingMode();
            if(MMIAPIBT_GetActiveBTHeadset())
            {
                //MMIMP3_InitAudioDevice(TRUE);  //声音切换到耳机需要先设置设备音量
                is_ok = MMIAP_TrySetBtAudio(TRUE);
                if (is_ok)
                {
                    bt_setting = MMIMP3_BT_MODE_ON;
                }

                MMIMP3_SetBtSettingMode(bt_setting);

                SCI_TRACE_LOW("[MMIAP HandleMp3BtModeSetWin MENU_INDEX_0 bt_setting=%d",\
                              bt_setting);

                MMK_CloseWin(win_id);
            }
            else
            {
                SCI_TRACE_LOW("[MMIAP HandleMp3BtModeSetWin bt_setting=%d",\
                              bt_setting);


                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_BT_HEADSET_MUST_SELECT, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                GUILIST_SetSelectedItem(ctrl_id, (uint16)MMIMP3_MENU_INDEX_1, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)MMIMP3_MENU_INDEX_1);
            }

            break;

        case MMIMP3_MENU_INDEX_1:
            bt_setting = MMIMP3_BT_MODE_OFF;

            MMIMP3_SetBtSettingMode(bt_setting);

            SCI_TRACE_LOW("[MMIAP HandleMp3BtModeSetWin MENU_INDEX_1 to OFF bt_setting=%d",\
                          bt_setting);

            MMIAP_TrySetBtAudio(FALSE);

            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }

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
//  Description : Handle Mp3 Bt Headset Win Msg
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3BtHeadsetWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_BT_SET_LABEL_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetBtHeadsetName(FALSE);
        GUIFORM_SetMargin(MMIMP3_BT_SET_FORM_CTRL_ID, MMI_TITLE_THEME_HEIGHT / 2);
        MMK_SetAtvCtrl(win_id, ctrl_id);

        /* 解决蓝牙耳机连接后，声音未从蓝牙耳机播放
        * 进入选择界面，先设置允许从蓝牙耳机出声音
        */
        if (MMIMP3_BT_MODE_NONE == MMIMP3_GetBtSettingMode())
        {
            MMIAP_TrySetBtAudio(TRUE);
        }
        break;

    case MSG_GET_FOCUS:
        SetBtHeadsetName(FALSE);
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_OpenBtHeadsetList();
#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                                || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                                || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left softkey处理
            }
            else if((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle softkey处理
#ifdef BLUETOOTH_SUPPORT
                MMIAPIBT_OpenBtHeadsetList();
#endif
            }
            else if((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right softkey处理
            }

            break;
        }

#endif //MMI_PDA_SUPPORT

        if((PNULL != param) && ctrl_id == ((MMI_NOTIFY_T*) param)->src_id)
        {
#ifdef BLUETOOTH_SUPPORT
            MMIAPIBT_OpenBtHeadsetList();
#endif
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
    {

        /* 在退出此界面时，检查蓝牙耳机
        *是否有连接
        */
        if (MMIMP3_BT_MODE_NONE == MMIMP3_GetBtSettingMode())
        {
            if(MMIAPIBT_GetActiveBTHeadset())
            {
                MMIMP3_SetBtSettingMode(MMIMP3_BT_MODE_ON);
            }
            else
            {
                MMIAP_TrySetBtAudio(FALSE);
            }
        }
    }
    break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Set Bt Headset Name
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL void SetBtHeadsetName(
    BOOLEAN  is_update
)
{
#ifdef BLUETOOTH_SUPPORT
    wchar                       file_name[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint16                      file_name_len = 0;
    MMI_STRING_T                title_string = {0};

    if(MMIAPIBT_GetActiveHeadsetName(file_name, &file_name_len))
    {
        title_string.wstr_ptr = file_name;
        title_string.wstr_len = file_name_len;
        GUILABEL_SetText(MMIMP3_BT_SET_LABEL_CTRL_ID, &title_string, is_update);
    }
    else
    {
        //MMI_GetLabelTextByLang( TXT_NULL, &title_string );
        //GUILABEL_SetText(MMIMP3_BT_SET_LABEL_CTRL_ID, &title_string, is_update);
        GUILABEL_SetTextById(MMIMP3_BT_SET_LABEL_CTRL_ID, TXT_BLANK, TRUE);
    }

#endif
}
#endif

/*****************************************************************************/
//  Description : 编码方式选项:1、GB(简体中文) 2、Big5(繁体中文)
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendCodeTypeListItem(
    MMI_CTRL_ID_T      ctrl_id
)
{
    uint16                      cur_selection = 0;
    MMIMP3_TXT_CODE_TYPE_E      code_type = MMIMP3_TXT_CODE_TYPE_GB;

    MMIAPISET_AppendListItemByTextId(TXT_CODING_TYPE_GB, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CODING_TYPE_BIG5, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

    //get current methed
    code_type = APSETTING_GetCodeType();

    //the status is open
    cur_selection = code_type;

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : 设置编码方式
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetCodeType(
    MMI_CTRL_ID_T      ctrl_id
)
{
    uint16      cur_selection = 0;
    MMIMP3_TXT_CODE_TYPE_E code_type = MMIMP3_TXT_CODE_TYPE_GB;

    GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

    switch(cur_selection)
    {
    case MMIMP3_TXT_CODE_TYPE_GB:
        code_type = MMIMP3_TXT_CODE_TYPE_GB;
        break;

    case MMIMP3_TXT_CODE_TYPE_BIG5:
        code_type = MMIMP3_TXT_CODE_TYPE_BIG5;
        break;

    default:
        break;
    }

    APSETTING_SetCodeType(code_type);
}

/*****************************************************************************/
//  Description : 编码方式设置界面
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCodeType(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_SET_TXT_CODE_TYPE_CTRL_ID;
    static uint16   s_cur_index = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIMP3_TXT_CODE_TYPE_MAX, FALSE);
        AppendCodeTypeListItem(ctrl_id);
        s_cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        SetCodeType(ctrl_id);
#ifdef MMIAP_LYRIC_SUPPORT

        if(GUILIST_GetCurItemIndex(ctrl_id) != s_cur_index)
        {
            if(MMIMP3_PLAYER_AUDIO_ACTIVE == MMIAPIMP3_GetPlayerState())
            {
                MMIMP3_OpenLyric();
            }
        }

#endif
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenSetAlarmWin(
    wchar *file_full_path_ptr,
    uint16 file_full_path_len
)
{
    MMIMP3_FILE_INFO_T  *file_info_ptr = PNULL;

    MMK_CreatePubListWinEx(SPRD_MP3_APPLET_ID, (uint32 *)MMIMP3_SET_ALARM_RING_WIN_TAB, PNULL);

    if (PNULL != file_full_path_ptr)
    {
        file_info_ptr = SCI_ALLOCA(sizeof(MMIMP3_FILE_INFO_T));
        if (PNULL != file_info_ptr)
        {
            SCI_MEMSET(file_info_ptr,0,sizeof(MMIMP3_FILE_INFO_T));
            file_info_ptr->file_name_len = MIN(file_full_path_len,sizeof(file_info_ptr->file_name));
            SCI_MEMSET(file_info_ptr->file_name,0,sizeof(file_info_ptr->file_name));
            MMIAPICOM_Wstrncpy(file_info_ptr->file_name,file_full_path_ptr,file_info_ptr->file_name_len);
            MMK_SetWinUserData(MMIMP3_ALARM_RING_SET_WIN_ID,file_info_ptr);
        }
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenEQSetWin(void)
{
    MMK_CreatePubListWinEx(SPRD_MP3_APPLET_ID, (uint32 *)MMIMP3_EQ_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenDisplaySetWin(void)
{
    MMK_CreatePubListWinEx(SPRD_MP3_APPLET_ID, (uint32 *)MMIMP3_DISPLAY_SET_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenRepeatModeSetWin(void)
{
    MMK_CreatePubListWinEx(SPRD_MP3_APPLET_ID, (uint32 *)MMIMP3_REPEAT_MODE_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenShuffleModeSetWin(void)
{
    MMK_CreatePubListWinEx(SPRD_MP3_APPLET_ID, (uint32 *)MMIMP3_SHUFFLE_MODE_WIN_TAB, PNULL);
}

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenBTSetWin(void)
{
    MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_BT_SET_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenBTHeadSetWin(void)
{
    MMIAPAPPLET_CreateWinTable((uint32 *)MMIMP3_BT_HEADSET_WIN_TAB, PNULL);
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenTxtCodeTypeSetWin(void)
{
    MMK_CreatePubListWinEx(SPRD_MP3_APPLET_ID, (uint32 *)MMIMP3_SET_TXT_CODE_TYPE_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenSettingWin(void)
{
    MMIAPAPPLET_CreateWinTable((uint32*)MMIMP3_SETTINGS_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E MMIAPSETWIN_GetCurrentEqMode(void)
{
    MMISRVAUD_EQ_MODE_E eq_mode = 0;

    if(MMK_IsOpenWin(MMIMP3_EQ_SET_WIN_ID))
    {
        eq_mode = (MMISRVAUD_EQ_MODE_E)GUILIST_GetCurItemIndex(MMIMP3_EQ_SET_LIST_CTRL_ID);
    }
    else
    {
        eq_mode = APSETTING_GetEqMode();
    }

    return eq_mode;
}
