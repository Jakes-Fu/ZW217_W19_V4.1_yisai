/****************************************************************************
** File Name:      mmicc_wintab.c                                          *
** Author:                                                                 *
** Date:           06/11/2004                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the window for cc         *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2004        louis.wei         Create
** 
****************************************************************************/
#define _MMICC_WINTAB_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"

#include "window_parse.h"
#include "cafcontrol.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmk_gsensor.h"

#include "mmi_appmsg.h"
#include "mmi_menutable.h"
#include "mmi_resource.h"
#include "mmi_id.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guibutton.h"
#include "guiform.h"
#include "guimenu.h"
#include "guistatusbar.h"
#include "guires.h"

#include "mmicc_export.h"
#include "mmicc_id.h"
#include "mmicc_menutable.h"
#include "mmicc_internal.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmicc_anim.h"
#include "mmicc_audio.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#include "mmiset_func.h"
#include "mmipub.h"
#ifdef RECORD_SUPPORT
#include "mmisrvrecord_export.h"
#endif

#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmidisplay_data.h"
#include "mmisms_text.h"
#include "mmiidle_export.h"
#include "mmicl_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmipb_export.h"
#include "mmimp_location.h"
#include "mmiidle_subwintab.h"
#include "mmidm_export.h"
//#include "mmiidle_statusbar.h"
#include "mmistk_export.h"
#include "mmiudisk_export.h"
#include "mmimtv_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmicountedtime_export.h"
#include "mmiussd_export.h"
#include "mmiidle_statusbar.h"
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_text.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#ifdef MV_SUPPORT
#include "agc_exp.h"//@fen.xie for magic voice
#ifndef WIN32
#include "magicvoice_app.h"
#endif
#endif

#ifdef TTS_SUPPORT
#include "mmitts_export.h"
//#include "mmiset_display.h"
#endif

#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
#include "mmiphonesearch_export.h"
#endif

#ifdef SCREENSAVER_SUPPORT 
#include "mmiss_export.h"
#endif

#ifndef WIN32
#include "gpio_prod_api.h"
#endif

#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#include "java_mmi_text.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#ifdef MET_MEX_AQ_SUPPORT
#include "met_mex_aq.h"
#endif
#endif

#include "mmimultim_image.h"
#include "mmicom_panel.h"
#if defined(TOUCH_PANEL_SUPPORT)
#include "mmiidle_export.h"
#endif
#include "guictrl_api.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif 
#include "mmidc_export.h"
#include "mmiatv_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif
#ifdef VT_SUPPORT
#include "mmivt_ui.h"//bug337268
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
//extern uint8 g_mmi_headset_time_id; // 耳机插入1秒以内，不响应button

#ifdef CMCC_UI_STYLE
LOCAL BOOLEAN s_is_cancel_reject = FALSE; //非PDA形态，只有右软键才短信拒接
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define	CC_REDIAL_TIMER_DUR1	5000        //第一次重拨间隔时间为5秒
#define	CC_REDIAL_TIMER_DUR2	60000       //第一次重拨间隔时间为1分
#define	CC_REDIAL_TIMER_DUR3	180000      //第一次重拨间隔时间为3分
#define	CC_REDIAL_TIMER_TIP	    1000        //重拨等待定时器周期

#define CC_DTMF_WAITING_TIME    3000

#define CC_NO_NUM_AUTODIAL_MAX_TIME     1

#define MMICC_PUBWIN_PROMPTEXT_MAX 2

#define MMICC_PUBWIN_TEXT_MAX_INDEX 4

#define CC_CONNECT_PROMPT_VIBRATE_DUARATION	200	//接通提示震动持续时间，500ms

#ifndef MMI_GUI_STYLE_TYPICAL
#define MMI_CALL_ACTIVE_COLOR MMI_WHITE_COLOR//0x6d22
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
#define MMI_CALL_HOLD_COLOR MMI_BLUE_COLOR //0xc364
#else
#define MMI_CALL_HOLD_COLOR RGB888TO565(0xFF6400) //0xc364
#endif
#define MMI_CALL_END_COLOR MMI_WHITE_COLOR//MMI_RED_COLOR
#define MMI_CALL_GRAY_COLOR MMI_WHITE_COLOR
#define MMI_CALL_ON_COLOR MMI_WHITE_COLOR
#define MMI_CALL_LOCK_MT_TEXT_COLOR MMI_WHITE_COLOR
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
#define MMI_CALL_STATE_TEXT_COLOR GUI_RGB2RGB565(232, 87, 0)
#else
#define MMI_CALL_INANDOUT_STATE_TEXT_COLOR RGB888TO565(0x00A8FF)
#define MMI_CALL_OTHER_STATE_TEXT_COLOR RGB888TO565(0xEF0000)
#define MMI_CALL_STATE_TEXT_COLOR RGB888TO565(0x00991F)
#define MMI_CALL_ANIM_BG_COLOR RGB888TO565(0x81bd00)
#endif
#if defined(MAINLCD_SIZE_240X320)
#define MMI_CALL_NAME_FONT SONG_FONT_14
#define MMI_CALL_INFO_FONT SONG_FONT_12
#define MMI_CALL_HOLD_TEXT_FONT SONG_FONT_14

#define MMI_MT_LOCK_TOP 200
#define MMI_MT_LOCK_TOP_H 120

#define MMI_DTMF_PANEL_HEIGHT 168
#define MMI_DTMF_PANEL_WIDTH 240
#define MMI_DTMF_PANEL_HEIGHT_H 160
#define MMI_DTMF_PANEL_WIDTH_H 204
#elif defined(MAINLCD_SIZE_240X400)
#define MMI_CALL_NAME_FONT SONG_FONT_16
#define MMI_CALL_INFO_FONT SONG_FONT_14
#define MMI_CALL_HOLD_TEXT_FONT SONG_FONT_14

#define MMI_MT_LOCK_TOP 255
#define MMI_MT_LOCK_TOP_H 120

#define MMI_DTMF_PANEL_HEIGHT 168
#define MMI_DTMF_PANEL_WIDTH 240
#define MMI_DTMF_PANEL_HEIGHT_H 160
#define MMI_DTMF_PANEL_WIDTH_H 204
#elif defined(MAINLCD_SIZE_128X160)
#define MMI_CALL_NAME_FONT SONG_FONT_12
#define MMI_CALL_INFO_FONT SONG_FONT_12
#define MMI_CALL_HOLD_TEXT_FONT SONG_FONT_12

#define MMI_MT_LOCK_TOP 255
#define MMI_MT_LOCK_TOP_H 120

#define MMI_DTMF_PANEL_HEIGHT 168
#define MMI_DTMF_PANEL_WIDTH 240
#define MMI_DTMF_PANEL_HEIGHT_H 160
#define MMI_DTMF_PANEL_WIDTH_H 204
#elif defined(MAINLCD_SIZE_176X220)
#define MMI_CALL_NAME_FONT SONG_FONT_12
#define MMI_CALL_INFO_FONT SONG_FONT_12
#define MMI_CALL_HOLD_TEXT_FONT SONG_FONT_12

#define MMI_MT_LOCK_TOP 255
#define MMI_MT_LOCK_TOP_H 120

#define MMI_DTMF_PANEL_HEIGHT 168
#define MMI_DTMF_PANEL_WIDTH 240
#define MMI_DTMF_PANEL_HEIGHT_H 160
#define MMI_DTMF_PANEL_WIDTH_H 204
#else
#define MMI_CALL_NAME_FONT SONG_FONT_22
#define MMI_CALL_INFO_FONT MMI_DEFAULT_NORMAL_FONT
#define MMI_CALL_HOLD_TEXT_FONT SONG_FONT_16

#define MMI_MT_LOCK_TOP 300
#define MMI_MT_LOCK_TOP_H 160

#define MMI_DTMF_PANEL_HEIGHT 228
#define MMI_DTMF_PANEL_WIDTH 320
#define MMI_DTMF_PANEL_HEIGHT_H 210
#define MMI_DTMF_PANEL_WIDTH_H 282
#endif
#endif

#define CC_CALL_STATE_STR_MAX_LEN       60      //当前呼叫状态显示的最大字节数

#ifdef MMI_CLEAR_CODE_SUPPORT
#define CC_CALL_CLEAR_CODE_STR_MAX_LEN       127      //当前呼叫状态显示的最大字节数
#endif

#define CC_WAITING_CALL_PLAY_TIMER      6   //呼叫等待时候，tone播放的次数
#define CC_WAITING_CALL_PERIOD          MMI_2SECONDS    //每次播放间隔时间    

#define CC_PLAY_CALL_END_HINT_TIME 3    //播放通话结束提醒音的次数

#define MMICC_BT_HF_RECORD_INDEX        6
#define MMICC_HF_RECORD_INDEX           5
#define MMICC_BT_RECORD_INDEX           MMICC_HF_RECORD_INDEX
#define MMICC_HOLD_RECORD_INDEX         4


#define MMICC_SELECT_SIM_MAX_NUM        (MMI_DUAL_SYS_MAX) 

#ifdef MV_SUPPORT
#define CC_MAGIC_CALL_VOLUME  7 //开启并设置魔音通话的音量
#endif

#define	CLEAR_AUTOREDIAL_INFO()  \
{\
    CC_INSTANCE.autodial_info.num = 0;\
    CC_INSTANCE.autodial_info.num_max = 0;\
}

#define CHECK_CC_APPLET_NOT_NULL \
if(PNULL == MMICC_GetAppletInstance()) \
{\
    SCI_TRACE_LOW("ccapp null applet");\
    return;\
}
#define CC_APPLET_HANDLE (MMICC_GetAppletInstance()->caf_applet.app_handle)

#define CC_INSTANCE (MMICC_GetAppletInstance()->cc_instance)

#define CC_TEXT_COLOR MMI_BLACK_COLOR

#if !defined(MMI_PDA_SUPPORT)  && defined(TOUCH_PANEL_SUPPORT) 
#define HB_HOLD_ROW 1 //hold/retrieve 按钮所在button行数
#else
#define HB_HOLD_ROW 0
#endif
#define HB_HOLD_COLUMN 2 //hold/retrieve 按钮所在button列数

typedef enum
{
    CC_DISPALY_INFO_STATE,
    CC_DISPALY_INFO_NAME,
    CC_DISPALY_INFO_NUMBER,
    CC_DISPALY_INFO_TIME,
    CC_DISPALY_INFO_LOCATION, 
    CC_DISPALY_INFO_AUTODIAL_TIMES, //自动重播次数
    CC_DISPALY_INFO_ASP_TIME, //自动答录机计时
    CC_DISPALY_INFO_MAX
}CC_DISPALY_INFO_E;


#ifndef MMI_GUI_STYLE_TYPICAL
typedef struct
{
    MMI_CTRL_ID_T            ctrl_id;
    MMI_TEXT_ID_T            text_id;//text on fg
    MMI_IMAGE_ID_T          img_icon_id;//fg img
    MMI_IMAGE_ID_T          img_icon_grayed_id;//grayed fg img
    MMI_IMAGE_ID_T          img_bg_id;
    MMI_IMAGE_ID_T          img_bg_pressed_id;
    MMI_IMAGE_ID_T          img_bg_on_id;
    MMI_IMAGE_ID_T          img_bg_on_pressed_id;
    BOOLEAN                     is_gray;
} CC_CC_BUTTON_T;
#else
typedef struct
{
    MMI_CTRL_ID_T            ctrl_id;
    MMI_IMAGE_ID_T          img_id;
    MMI_IMAGE_ID_T          img_on_id;
#ifndef TOUCH_PANEL_SUPPORT
    MMI_IMAGE_ID_T          focus_img_id;
    MMI_IMAGE_ID_T          focus_img_on_id;
#endif
    MMI_IMAGE_ID_T          gray_img_id;
    MMI_IMAGE_ID_T          gray_img_on_id;
    BOOLEAN                     is_gray;
} CC_CC_BUTTON_T;
#endif

typedef enum
{
    CHILD_FORM_ID,
    PHOTO_ANIM_ID,
    CHILD_CHILD_FORM_ID,
    NAME_LABEL_ID,
    TIME_LABEL_ID,
    CC_MULTI_CALL_DISPLAY_CTRL_MAX
} CC_MULTI_CALL_DISPLAY_CTRL_E; 

typedef enum
{
    CC_MULTI_CALL_ACTIVE,
    CC_MULTI_CALL_HOLD,
    CC_MULTI_CALL_MAX
} CC_MULTI_CALL_TYPE_E; //display multi call info, there are two lines, one is acive calls info, another is hold info

#ifdef MV_SUPPORT
typedef enum
{
    MMICC_MV_MENU_TYPE_NONE,
    MMICC_MV_MENU_TYPE_CC,//CC中设置魔音
    MMICC_MV_MENU_TYPE_SET//通话设置中的魔音
} MMICC_MV_MENU_TYPE_E;

typedef struct
{
    MMI_IMAGE_ID_T  image_id;
    MMI_TEXT_ID_T   text_id;
    uint32          value;
} MV_DATA_T;
#endif

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   

LOCAL const MMI_CTRL_ID_T s_multi_call_display_ctrl[CC_MULTI_CALL_MAX][CC_MULTI_CALL_DISPLAY_CTRL_MAX] = 
{
    {    
        MMICC_MULTI_ACTIVE_CALL_CHILD_FORM_CTRL_ID,
        MMICC_MULTI_ACTIVE_CALL_PHOTO_ANIM_CTRL_ID, 
        MMICC_MULTI_ACTIVE_CALL_CHILD_CHILD_FORM_CTRL_ID,
        MMICC_MULTI_ACTIVE_CALL_NAME_LABEL_CTRL_ID,  
        MMICC_MULTI_ACTIVE_CALL_TIME_LABEL_CTRL_ID,
    },
    {    
        MMICC_MULTI_HOLD_CALL_CHILD_FORM_CTRL_ID,
        MMICC_MULTI_HOLD_CALL_PHOTO_ANIM_CTRL_ID,
        MMICC_MULTI_HOLD_CALL_CHILD_CHILD_FORM_CTRL_ID,
        MMICC_MULTI_HOLD_CALL_NAME_LABEL_CTRL_ID,
        MMICC_MULTI_HOLD_CALL_TIME_LABEL_CTRL_ID,
    }
};

#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
//need be modify later
LOCAL const GUI_RECT_T s_connected_win_rect_coordinate[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
{
    {
        {CC_HB_ONE_ONE_LEFT, CC_HB_ONE_ONE_TOP, CC_HB_ONE_ONE_RIGHT, CC_HB_ONE_ONE_BOTTOM},
        {CC_HB_ONE_TWO_LEFT, CC_HB_ONE_TWO_TOP, CC_HB_ONE_TWO_RIGHT, CC_HB_ONE_TWO_BOTTOM},
        {CC_HB_ONE_THREE_LEFT, CC_HB_ONE_THREE_TOP, CC_HB_ONE_THREE_RIGHT, CC_HB_ONE_THREE_BOTTOM}
    },
    {
        {CC_HB_TWO_ONE_LEFT, CC_HB_TWO_ONE_TOP, CC_HB_TWO_ONE_RIGHT, CC_HB_TWO_ONE_BOTTOM},
        {CC_HB_TWO_TWO_LEFT, CC_HB_TWO_TWO_TOP, CC_HB_TWO_TWO_RIGHT, CC_HB_TWO_TWO_BOTTOM},
        {CC_HB_TWO_THREE_LEFT, CC_HB_TWO_THREE_TOP, CC_HB_TWO_THREE_RIGHT, CC_HB_TWO_THREE_BOTTOM}
    }
};

LOCAL const GUI_RECT_T s_connected_win_rect_coordinate_h[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
{
    {
        {CC_HB_ONE_ONE_LEFT_H, CC_HB_ONE_ONE_TOP_H, CC_HB_ONE_ONE_RIGHT_H, CC_HB_ONE_ONE_BOTTOM_H},
        {CC_HB_ONE_TWO_LEFT_H, CC_HB_ONE_TWO_TOP_H, CC_HB_ONE_TWO_RIGHT_H, CC_HB_ONE_TWO_BOTTOM_H},
        {CC_HB_ONE_THREE_LEFT_H, CC_HB_ONE_THREE_TOP_H, CC_HB_ONE_THREE_RIGHT_H, CC_HB_ONE_THREE_BOTTOM_H}
    },
    {
        {CC_HB_TWO_ONE_LEFT_H, CC_HB_TWO_ONE_TOP_H, CC_HB_TWO_ONE_RIGHT_H, CC_HB_TWO_ONE_BOTTOM_H},
        {CC_HB_TWO_TWO_LEFT_H, CC_HB_TWO_TWO_TOP_H, CC_HB_TWO_TWO_RIGHT_H, CC_HB_TWO_TWO_BOTTOM_H},
        {CC_HB_TWO_THREE_LEFT_H, CC_HB_TWO_THREE_TOP_H, CC_HB_TWO_THREE_RIGHT_H, CC_HB_TWO_THREE_BOTTOM_H}
    }
};
#endif
#else //MMI_GUI_STYLE_TYPICAL
LOCAL  CC_CC_BUTTON_E s_connected_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
{
    {CC_BUTTON_HANDFREE, CC_BUTTON_DIALPANEL, CC_BUTTON_MUTE},
    {CC_BUTTON_RECORD, CC_BUTTON_HOLD, CC_BUTTON_PB },
};

LOCAL  CC_CC_BUTTON_E s_mo_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
{
    {CC_BUTTON_HANDFREE, CC_BUTTON_DIALPANEL, CC_BUTTON_MUTE},
    {CC_BUTTON_RECORD, CC_BUTTON_HOLD, CC_BUTTON_PB },
};

LOCAL CC_CC_BUTTON_E s_mt_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
{
    {CC_BUTTON_HANDFREE, CC_BUTTON_DIALPANEL, CC_BUTTON_MUTE},
    {CC_BUTTON_RECORD, CC_BUTTON_HOLD, CC_BUTTON_PB },
};
#endif


#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
LOCAL CC_CC_BUTTON_T s_cc_win_button[] =
{
    //CC_BUTTON_HANDFREE
    {
        MMICC_BUTTON_HANDFREE_CTRL_ID,
        TXT_COMMON_HAND_FREE,
        IMG_CC_HANDFREE_ICON,
        IMG_CC_HANDFREE_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },   
    
    //CC_BUTTON_DIALPANEL
    {
        MMICC_BUTTON_DIALPANEL_CTRL_ID,
        TXT_CC_DTMF_PANEL,
        IMG_CC_DIALPANEL_ICON,
        IMG_CC_DIALPANEL_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_MUTE
    {
        MMICC_BUTTON_MUTE_CTRL_ID,
        TXT_TURNON_MUTE,
        IMG_CC_MUTE_ICON,
        IMG_CC_MUTE_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_RECORD
    {
        MMICC_BUTTON_RECORD_CTRL_ID,
        TXT_FM_RECORD,
        IMG_CC_RECORD_ICON,
        IMG_CC_RECORD_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_HOLD
    {
        MMICC_BUTTON_HOLD_CTRL_ID,
        TXT_CC_HOLD_PDA,
        IMG_CC_HOLD_ICON,
        IMG_CC_HOLD_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_PB
    {
        MMICC_BUTTON_PB_CTRL_ID,
        STXT_MAIN_PB,
        IMG_CC_PB_ICON,
        IMG_CC_PB_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },
#if !defined(MMI_GUI_STYLE_TYPICAL)
    //CC_BUTTON_SILENCE
    {
        MMICC_BUTTON_SILENCE_CTRL_ID,
        TXT_CC_DISABLE_RING,
        IMG_CC_SILENCE_ICON,
        IMG_CC_SILENCE_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_HANGUP
    {
        MMICC_BUTTON_HANGUP_CTRL_ID,
        TXT_CC_HANG_OFF,
        IMG_CC_HANGUP_ICON,
        IMG_CC_HANGUP_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HANGUP_BUTTON_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },
    
    //CC_BUTTON_ANSWER
    {
        MMICC_BUTTON_ANSWER_CTRL_ID,
        TXT_COMMON_ANSWER_CALL,
        IMG_CC_ANSWER_ICON,
        IMG_CC_ANSWER_GRAY_ICON,
        IMAGE_CALL_ANSWER_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_NEW_CALL
    {
        MMICC_BUTTON_NEW_CALL_CTRL_ID,
        TXT_CC_ADD_CALL,
        IMG_CC_ADD_CALL_ICON,
        IMG_CC_ADD_CALL_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_MERGE_CALLS
    {
        MMICC_BUTTON_MERGE_CALLS_CTRL_ID,
        TXT_CC_MERGE_CALLS,
        IMG_CC_MERGE_CALLS_ICON,
        IMG_CC_MERGE_CALLS_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_SWAP_CALLS
    {
        MMICC_BUTTON_SWAP_CALLS_CTRL_ID,
        TXT_CC_SWAP_PDA,
        IMG_CC_SWAP_CALLS_ICON,
        IMG_CC_SWAP_CALLS_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },

    //CC_BUTTON_RETRIEVE_CALLS
    {
        MMICC_BUTTON_RETRIVE_CALLS_CTRL_ID,
        TXT_CC_UNHOLD_PDA,
        IMG_CC_RETRIEVE_CALLS_ICON,
        IMG_CC_RETRIEVE_CALLS_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },
#endif //#if !defined(MMI_GUI_STYLE_TYPICAL)
//CC_BUTTON_ASP
#ifdef ASP_SUPPORT 
    {
        MMICC_BUTTON_ASP_CTRL_ID,
        TXT_CC_ASP_ANSWER_CALL,
        IMG_CC_ASP_ICON,
        IMG_CC_ASP_GRAY_ICON,
        IMAGE_CC_HB_BG_DEFAULT,
        IMAGE_CC_HB_BG_PRESSED,
        IMAGE_CC_HB_BG_SELECTED,
        IMAGE_CC_HB_BG_PRESSED,
        FALSE,
    },
#endif
};
#endif

#else
LOCAL CC_CC_BUTTON_T s_cc_win_button[] =
{
    //CC_BUTTON_HANDFREE
    {
        MMICC_BUTTON_HANDFREE_CTRL_ID,
        IMG_CC_HANDFREE,
        IMG_CC_HANDFREE_ON,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_HANDFREE_FOCUS,
        IMG_CC_HANDFREE_ON_FOCUS,
#endif
        IMG_CC_HANDFREE_GRAY,
        IMG_CC_HANDFREE_ON_GRAY,
        FALSE,
    },   
    
    //CC_BUTTON_DIALPANEL
    {
        MMICC_BUTTON_DIALPANEL_CTRL_ID,
        IMG_CC_DIALPANEL,
        IMAGE_NULL,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_DIALPANEL_FOCUS,
        IMAGE_NULL,
#endif
        IMG_CC_DIALPANEL_GRAY,
        IMAGE_NULL,
        FALSE,
    },

    //CC_BUTTON_MUTE
    {
        MMICC_BUTTON_MUTE_CTRL_ID,
        IMG_CC_MUTE,
        IMG_CC_MUTE_ON,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_MUTE_FOCUS,
        IMG_CC_MUTE_ON_FOCUS,
#endif
        IMG_CC_MUTE_GRAY,
        IMG_CC_MUTE_ON_GRAY,
        FALSE,
    },

    //CC_BUTTON_RECORD
    {
        MMICC_BUTTON_RECORD_CTRL_ID,
        IMG_CC_RECORD,
        IMG_CC_RECORD_ON,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_RECORD_FOCUS,
        IMG_CC_RECORD_ON_FOCUS,
#endif
        IMG_CC_RECORD_GRAY,
        IMG_CC_RECORD_ON_GRAY,
        FALSE,
    },

    //CC_BUTTON_HOLD
    {
        MMICC_BUTTON_HOLD_CTRL_ID,
        IMG_CC_HOLD,
        IMAGE_NULL,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_HOLD_FOCUS,
        IMAGE_NULL,
#endif
        IMG_CC_HOLD_GRAY,
        IMAGE_NULL,
        FALSE,
    },

    //CC_BUTTON_PB
    {
        MMICC_BUTTON_PB_CTRL_ID,
        IMG_CC_PB,
        IMAGE_NULL,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_PB_FOCUS,
        IMAGE_NULL,
#endif
        IMG_CC_PB_GRAY,
        IMAGE_NULL,
        FALSE,
    },
    
    //CC_BUTTON_ASP
#ifdef ASP_SUPPORT 
    {
        MMICC_BUTTON_ASP_CTRL_ID,
        IMG_CC_ASP,
        IMG_CC_ASP_ON,
#if !defined(TOUCH_PANEL_SUPPORT)
        IMG_CC_ASP_FOCUS,
        IMG_CC_ASP_ON_FOCUS,
#endif
        IMG_CC_ASP_GRAY,
        IMG_CC_ASP_ON_GRAY,
        FALSE,
    },
#endif
};
#endif

LOCAL const MMI_TEXT_ID_T s_cc_disc_cause[] =
{
    TXT_CC_USER_BUSY,
    TXT_CC_USER_NOT_RESPOND,
    TXT_CC_ERROR_NUMBER,
    TXT_CC_CALL_REJECTED,
    TXT_CC_NETWORK_ERROR,//TXT_CC_RES_UNAV,
    TXT_CC_HIDE_ID,
    TXT_CC_NETWORK_ERROR,//TXT_CC_SERVICE_UNAV,
    TXT_CC_NETWORK_ERROR,//TXT_CC_SERVICE_NOT_IMP,
    TXT_CC_NETWORK_ERROR,//TXT_CC_INVALID_MES,
    TXT_CC_NETWORK_ERROR,//TXT_CC_PROTOCOL_ERROR,
    TXT_CC_NETWORK_ERROR,
    TXT_CC_COST_EXCEED_LIMIT
};

static const uint32 s_redial_timeout[10] = {
    CC_REDIAL_TIMER_DUR1, CC_REDIAL_TIMER_DUR2, CC_REDIAL_TIMER_DUR2, CC_REDIAL_TIMER_DUR2,
    CC_REDIAL_TIMER_DUR3, CC_REDIAL_TIMER_DUR3, CC_REDIAL_TIMER_DUR3, CC_REDIAL_TIMER_DUR3,
    CC_REDIAL_TIMER_DUR3, CC_REDIAL_TIMER_DUR3};

//receiver of sms after mt call reject
LOCAL uint8 s_sms_name_arr[CC_MAX_TELE_NUM_LEN] = {0};
LOCAL uint16 s_sms_name_len;

#ifdef MV_SUPPORT
LOCAL MMICC_MV_MENU_TYPE_E s_mv_menu_open_type = MMICC_MV_MENU_TYPE_NONE;
LOCAL BOOLEAN s_is_mv_working = FALSE;//魔音功能是否正运行中
LOCAL const uint8 s_mv_item_num[MMISET_MV_MAX_NUM] = 
{
    7,  //change
    8,  //role
#ifdef MV_EFFECT_SUPPORT
    7,  //effect
#endif    
    13, //EQ
    5,  //BG
    7,  //BG_VOL
    7   //MV_VOL
};
//@fen.xie add MS00170872:the mv data that used in cc state(不影响通话设置中的mv设置参数)
LOCAL MV_DATA_T s_mv_data[MMISET_MV_MAX_NUM] = 
{
    {IMAGE_SECMENU_ICON_MULTIM_MP3, TXT_MV_CHANGE, 0},
    {IMAGE_SECMENU_ICON_LINKSET_NET, TXT_MV_ROLE, 0},
#ifdef MV_EFFECT_SUPPORT
    {IMAGE_SECMENU_ICON_MULTIM_KURO, TXT_MV_EFFECT, 0},
#endif    
    {IMAGE_SECMENU_ICON_SET_PROFILES, TXT_COMMON_EQ_SET, 0},
    {IMAGE_SECMENU_ICON_MULTIM_STREAM, TXT_MV_BG, 0},
    {IMAGE_SECMENU_ICON_MULTIM_MTV, TXT_MV_BGVOL, 0},
    {IMAGE_SECMENU_ICON_MULTIM_MTV, TXT_MV_MVVOL, 0},
};
//@fen.xie add MS00170872:used in mv setting window for recording the list item data
LOCAL MV_DATA_T s_mv_setting_data[MMISET_MV_MAX_NUM] = 
{
    {IMAGE_SECMENU_ICON_MULTIM_MP3, TXT_MV_CHANGE, 0},
    {IMAGE_SECMENU_ICON_LINKSET_NET, TXT_MV_ROLE, 0},
#ifdef MV_EFFECT_SUPPORT
   {IMAGE_SECMENU_ICON_MULTIM_KURO, TXT_MV_EFFECT, 0},
#endif   
    {IMAGE_SECMENU_ICON_SET_PROFILES, TXT_COMMON_EQ_SET, 0},
    {IMAGE_SECMENU_ICON_MULTIM_STREAM, TXT_MV_BG, 0},
    {IMAGE_SECMENU_ICON_MULTIM_MTV, TXT_MV_BGVOL, 0},
    {IMAGE_SECMENU_ICON_MULTIM_MTV, TXT_MV_MVVOL, 0},
};
/*@fen.xie MS00177206:MO/MT时选择取消键,则关闭魔音效果.该变量值只在MO/MT时设置并作为判断依据.
   附: 在in call status时,不作为判断依据.此时只有当魔音全部参数设置为空时,才能关闭魔音效果.*/
LOCAL BOOLEAN s_is_need_open_mv= FALSE;
#endif

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
LOCAL GUI_LCD_DEV_INFO s_animwin_dtmfpanel_layer = {0};
LOCAL GUI_LCD_DEV_INFO s_connectedwin_dtmfpanel_layer = {0};
#endif

LOCAL uint8 s_call_index[CC_MAX_CALL_NUM - 1];

#ifdef MMI_RECORD_SUPPORT
RECORD_SRV_HANDLE s_cc_record_handle = PNULL;
#endif

#ifdef BT_DIALER_SUPPORT
extern BOOLEAN g_call_disc_by_bt;
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//    Description : RGB888 translate to RGB565
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL  uint16 RGB888TO565(uint32 rgb888);

/*****************************************************************************/
//  Description : 创建多个电话的分割线
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
LOCAL void DrawCcPanelLine(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_BOTH_RECT_T *rect_ptr);
#endif

/*****************************************************************************/
// 	Description :Clear Auto Redial Info
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:CLEAR_AUTOREDIAL_INFO() 
/*****************************************************************************/
LOCAL void ClearAutoRedialInfo(void);

/*****************************************************************************/
// 	Description : open cc hold menu
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void  OpenCCHoldMenu(void);

/*****************************************************************************/
//  Description : 重拨次数超过限制添加该号码到黑名单中
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:参见GSM 02.07
/*****************************************************************************/
LOCAL void CC_AddItemtoBlackList(uint8* tele_num, uint8 tele_len);

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : get dial button rect
//  Global resource dependence : 
//  Author: xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDialButtonRect(MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
PUBLIC    MMI_RESULT_E HandleCallSIMSelectWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
//  Description : connect sim name and status string, copy to txt_str
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_CallStateDisplaying( uint16 str_len, wchar* state_str);

/*****************************************************************************/
//  Description : Free s_sim_status 
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_FreeSimStatusSpace(void);

/*****************************************************************************/
//  Description : set hanf free menu 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetHandFreeMenu(
    MMI_WIN_ID_T			win_id,
    MMI_TEXT_ID_T 		left_sk_id,
    MMI_TEXT_ID_T 		mid_sk_id,
    MMI_TEXT_ID_T 		right_sk_id,
    BOOLEAN is_need_fresh
    );

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set cc win RS key 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetConectedWinRightSoftkey(
    MMI_WIN_ID_T win_id,
    BOOLEAN is_need_fresh);
#endif
/*****************************************************************************/
//  Description : set mt win softkey
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_SetCallWinSoftkey(
    MMI_WIN_ID_T	        win_id,
    MMI_TEXT_ID_T 		left_sk_id,
    MMI_TEXT_ID_T 		mid_sk_id,
    MMI_TEXT_ID_T         right_sk_id,
    BOOLEAN               is_fresh
    );


/*****************************************************************************/
// 	Description : the process message function of the Missed call application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:modify by louis.wei
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICC_HandleInsertDTMFWinMsg( 
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param);

/*****************************************************************************/
// 	Description : the process message function of the sending dtmf 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICC_HandleSendingDTMFWinMsg( 
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param);

/*****************************************************************************/
// 	Description : the handle function of call list window message
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCCListWinMsg(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : the handle function of multi call list window message
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCCMultilistWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

/*****************************************************************************/
// 	Description : the process message function of the auot dialing  window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoDialWinMsg( 
				MMI_WIN_ID_T win_id, 
				MMI_MESSAGE_ID_E msg_id, 
				DPARAM param);


/*****************************************************************************/
// 	Description : the handle function of cc menu window message
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCCMenuWinMsg(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : the process message function of the Missed call application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMissedcallWinMsg( 
                                  MMI_WIN_ID_T win_id, 
                                  MMI_MESSAGE_ID_E msg_id, 
                                  DPARAM param);

/*****************************************************************************/
// 	Description : handle hold menu msg
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.9.29
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHoldMenuWinMsg(
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      );


/*****************************************************************************/
//  Description :to open the input win in calling
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenDialingWin( MMI_MESSAGE_ID_E msg_id );


#ifdef BROWSER_SUPPORT
/*****************************************************************************/
// 	Description : 通话中切换到网页前，询问用户是否要切换声音到扬声器
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchToHandfreeQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );
#endif

/*****************************************************************************/
// 	Description : 通过开始后提示发送dtmf
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void promptSendDtmfAfterCallSetup( void );

/*****************************************************************************/
//  Description :to open mo or mt win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void OpenCallingWin(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, MMI_ANIM_ID_T anim_id, PROCESSMSG_FUNC handle_func);  

/*****************************************************************************/
//  Description :to open connected win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void OpenConnectedWin(MMI_STRING_T* prompt_str_array, PROCESSMSG_FUNC handle_func
#ifdef MMI_CALLCONTROL_MINI_SUPPORT                                            
                                             ,BOOLEAN is_exist_time_lable,
                                             uint16 state_color
#endif
);


#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 创建通话界中面的Call button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructEndButton(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 创建通话界中面的Call button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructDialButton(MMI_WIN_ID_T win_id);

#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : 创建通话界中面的option button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructOptionButton(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 销毁通话界中面的option button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DestroyOptionButton(void);

/*****************************************************************************/
//  Description :CC create text label
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaCallCreateLabelByTextPtr(MMI_WIN_ID_T win_id,
                                                            MMI_CTRL_ID_T ctrl_id, 
                                                            GUILABEL_INIT_DATA_T label_init, 
                                                            MMI_STRING_T *text_ptr, 
                                                            GUI_FONT_T font, 
                                                            GUI_COLOR_T font_color);

/*****************************************************************************/
//  Description :CC create text label
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaCallCreateLabelByTextId(MMI_WIN_ID_T win_id,
                                                            MMI_CTRL_ID_T ctrl_id, 
                                                            GUILABEL_INIT_DATA_T label_init, 
                                                            MMI_TEXT_ID_T text_id, 
                                                            GUI_FONT_T font, 
                                                            GUI_COLOR_T font_color);

/*****************************************************************************/
//  Description :get "N People" string
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaGetConferenceNumStr(MMI_STRING_T *conference_people, BOOLEAN is_hold_specific);

/*****************************************************************************/
//  Description : create labels for MPTY cc screen
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL int32 GetCallInfoCtrlSpace(void);

/*****************************************************************************/
//  Description : create labels for single line cc screen, single line meas only exist single active/hold call or onle exist single MPTY, and no other calls
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplaySingleCallInfoForCommon(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, CC_PHOTO_TYPE_E photo_type
#ifdef MMI_CALLCONTROL_MINI_SUPPORT                                             
                                             ,BOOLEAN is_exist_time_lable,
                                             uint16 state_color
#endif
                                             );

/*****************************************************************************/
//  Description : create labels for MPTY cc screen
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayMPTYCallInfoForCommon(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, CC_PHOTO_TYPE_E photo_type
 #ifdef MMI_CALLCONTROL_MINI_SUPPORT
   , BOOLEAN is_exist_time_lable,
   uint16 state_color
 #endif
 );

/*****************************************************************************/
//  Description :create photo for common cc screen
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayCallPhotoForCommon( MMI_WIN_ID_T win_id, CC_PHOTO_TYPE_E photo_type);

/*****************************************************************************/
//  Description :add anim for 128X160/ 176X220 pro CC
//  Global resource dependence : none
//  Author:chunjie liu
//  Note:
/*****************************************************************************/
#ifdef MMI_CALLCONTROL_MINI_SUPPORT 
LOCAL void DisplayCallAnimPhotoForCommon( MMI_WIN_ID_T win_id,MMI_IMAGE_ID_T anim_id);
#endif

/*****************************************************************************/
//  Description :single line call, only a active or hold call, or a active/hold MPTY call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplaySingleCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array
#ifdef MMI_CALLCONTROL_MINI_SUPPORT                                             
                                             ,BOOLEAN is_exist_time_lable,
                                             uint16 state_color
#endif
);

/*****************************************************************************/
//  Description :multi line call, display active call in this cases: 
//                                                                                  active + hold call
//                                                                                  MPTY + hold call
//                                                                                  active + MPTY call
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayMultiActiveCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array);

/*****************************************************************************/
//  Description :multi line call, display hold call in this cases: 
//                                                                                  active + hold call
//                                                                                  MPTY + hold call
//                                                                                  active + MPTY call
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayCallInfoForMultiHoldCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array);

/*****************************************************************************/
//  Description :multi line call, display calls in this cases: 
//                                                                                  active + hold call
//                                                                                  MPTY + hold call
//                                                                                  active + MPTY call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayMultiCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array);

/*****************************************************************************/
//  Description :display disconneting win
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaOpenDisconnetingWin(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, PROCESSMSG_FUNC handle_func);

/*****************************************************************************/
//  Description :display disconneting win
//  Global resource dependence : none
//  Author:tao.xue
//  Note: modify by louis.wei 2004-01-14
/*****************************************************************************/
LOCAL void PdaDisplayDisconnectWin(
                                   uint8							call_index,
                                   MN_CALL_DISCONNECT_CALL_CAUSE_E	cause,
                                   uint32 call_time
                                   );
#else
/*****************************************************************************/
//  Description :to open processing win include disconnected win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void DisplayCallInfoForSingle(MMI_STRING_T* prompt_str_array, BOOLEAN is_flash);

/*****************************************************************************/
//  Description :when active call& hold call all being,display info include call state, name
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void DisplayCallInfoForMulti(void);

/*****************************************************************************/
//  Description :clean MMICC_CONNECTED_INFO_LIST_CTRL_ID items and append
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetMultiCallInfo(BOOLEAN is_flash);

/*****************************************************************************/
//  Description :to set mo or mt win text
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetCallingWinText(BOOLEAN is_update, MMI_STRING_T* prompt_str_array);

#endif
/*****************************************************************************/
// 	Description : process when timer is out
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallTimeDisplay(void);

/*****************************************************************************/
// 	Description : convert time count to str
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void CC_TimeCountToStr(MMI_STRING_T* time_str_ptr, uint32 time_count);
/*****************************************************************************/
//  Description :display when user want to make a call
//  Global resource dependence : none
//  Author:figo.feng
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenMoCallWin(void);

/*****************************************************************************/
//  Description :display when received a mt call
//  Global resource dependence : none
//  Author:figo.feng
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenMtCallWin(void);

/*****************************************************************************/
//  Description :display when received disconnect signal
//  Global resource dependence : none
//  Author:figo.feng
//  Note:
/*****************************************************************************/
LOCAL void CC_DisplayDisconnectWin(
    uint8							call_index,
    MN_CALL_DISCONNECT_CALL_CAUSE_E	cause,
    uint32 call_time
    );

/*****************************************************************************/
//  Description :display when there is missed call
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenMissedCallWin(void);

#if !defined(TOUCH_PANEL_SUPPORT)
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : find next hot button except gray ones
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL CC_CC_BUTTON_E CC_FindNextHotButton(CC_CC_BUTTON_E focus_button_type, MMI_MESSAGE_ID_E msg_id);
#endif
#endif

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : set image for specify hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_SetHotButtonImg(CC_CC_BUTTON_E button_type, BOOLEAN is_focus_button);

/*****************************************************************************/
// 	Description : CC_FreshConnectedWinButton
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_IsHotButtonDisplayOnImg(CC_CC_BUTTON_E button_type);

/*****************************************************************************/
// 	Description : CC_FreshConnectedWinButton
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_UpdateCCWinHotButton(CC_CC_BUTTON_E button_type, //the button image need changing
//                                                                                    BOOLEAN is_need_paint, //the button need painting now
                                                                                    BOOLEAN is_focus_button//is focus button changed
                                                                                    );
#endif

#if !defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : handle cc window pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinWebMsg(
                              MMI_WIN_ID_T      win_id         //in:
                              );
#endif


/*****************************************************************************/
// 	Description : the process KBD,timer message function of the CC application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL MMI_RESULT_E  CC_HandleCcWinMsg(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
    );
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle CC window botton down msg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinButtonDownMsg(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle CC window botton up msg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinButtonUpMsg(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : handle cc window pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinPenOkMsg(
                              MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     ctrl_id  //in:button ctrl
                              );
#endif


/*****************************************************************************/
// 	Description : the process message function of the Disconnect window
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleDisconnectWinMsg( 
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void PlayCallEndHint(uint8 timer_id, uint32 param);

    
/*****************************************************************************/
// 	Description : to process of animation window in the cc
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  CC_HandleCcAnimWinMsg(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
    );

/*****************************************************************************/
// 	Description : play new call ring
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  PlayNewCallRing(MN_DUAL_SYS_E dual_sys);

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
// 	Description : CC_SendSMSAfterRejectMTCall 
//	Global resource dependence : 
//  Author:bruce.chi
//	Note: 
/*****************************************************************************/
LOCAL void CC_SendSMSAfterRejectMTCall(uint8 *name_ptr, uint16 name_len);
#endif


/*****************************************************************************/
// 	Description : emergency call window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN EmergencyCallWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
 LOCAL void  CC_HandleDtmfSendingTimeout( uint8 timer_id, uint32 param);

/*****************************************************************************/
// 	Description : EnableMuteMode 
//	Global resource dependence : 
//  Author:bin.ji
//	Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN EnableMuteMode(
    BOOLEAN is_turnon,
    BOOLEAN  need_pubwin
    );


/*****************************************************************************/
// 	Description : EnableHandfreeMode 
//	Global resource dependence : 
//  Author:bin.ji
//	Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN EnableHandfreeMode(
    BOOLEAN is_turnon
    );


/*****************************************************************************/
//  Description :CC_FindLocationInforl 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL void CC_HandleAnimWinSoftkey( MMI_HANDLE_T win_handle, BOOLEAN is_need_fress);

/*****************************************************************************/
//  Description :CC_FindLocationInforl 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL void CC_FindLocationInfor( MMI_STRING_T* mmi_str, uint16* str, 
    uint8* tel_num, uint8 tel_num_len );

/*****************************************************************************/
// 	Description : create cc waiting menu
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void  CreateCCWaitingMenu(void);

/*****************************************************************************/
// 	Description : create cc menu (include record, handfree)
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void CreateCCMenu(void);

/*****************************************************************************/
// 	Description : insert dynamenu node by label
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
    MMI_CTRL_ID_T      ctrl_id,        //控件id
    MMI_TEXT_ID_T   	text_id,       //插入的节点的文本
    uint16          	node_id,        //节点编号
    uint16          	parent_node_id, //父节点编号
    uint16          	index,           //位置
    MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
    BOOLEAN    is_grayed   //is_grayed
    );

#ifdef MMI_CC_STK_MENU_SUPPORT
/*****************************************************************************/
// 	Description : insert dynamenu node by label
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByString(
                             MMI_CTRL_ID_T ctrl_id,        //控件id
                             MMI_STRING_T kstring,       //插入的节点的文本
                             uint16 node_id,        //节点编号
                             uint16 parent_node_id, //父节点编号
                             uint16 index,           //位置
                             MMI_IMAGE_ID_T select_icon_id, //item selected icon id
                             BOOLEAN is_grayed   //is_grayed
                             );
#endif
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : get label of record
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetRecordLabelForCCHoldMenu(void);
#endif
/*****************************************************************************/
// 	Description : 提示窗口消息处理
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHoldCnfPromptWindow(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
    );

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : HandleRecordEndMsg
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordEndMsg(MMIRECORD_SRV_RESULT_E result);

/*****************************************************************************/
// 	Description : NotifyCallback
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void NotifyCallback(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result);

/*****************************************************************************/
//  Description : 是否通话录音中
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCCRecording(void);

/*****************************************************************************/
//  Description : 开始录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL MMIRECORD_SRV_RESULT_E StartCCRecord(void);

/*****************************************************************************/
//  Description : GetCurrentDev
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void);

/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
/*****************************************************************************/
LOCAL void  ShowRecordErrorMsg(MMIFILE_ERROR_E fs_error);

/*****************************************************************************/
// 	Description : 开始录音或停止录音
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordOpt(
    MMI_WIN_ID_T win_id,BOOLEAN is_start_auto_record
    );
#endif
/*****************************************************************************/
// 	Description : the process KBD,timer message function of the CC application
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.8
/*****************************************************************************/
LOCAL MMI_RESULT_E  CC_HandleCcProcessingWinMsg(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
    );
    
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : create hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateHotButtonsWithoutForm(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : create one hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateNewHotButtonWithoutForm(MMI_WIN_ID_T win_id, int32 new_button_row, int32 new_button_column, CC_CC_BUTTON_E new_button_type);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set gray state of hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructHangupButton(MMI_HANDLE_T win_handle);
#endif

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: tao.xue
//	Note:   
/*****************************************************************************/
LOCAL void CC_HotButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);
#endif
/*****************************************************************************/
// 	Description :create cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_CreateDtmfPanel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description :display cc dtmf panel for sending dtmf & hide hot buttons & disable other buttons
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_DisplayDtmfPanel(MMI_WIN_ID_T win_id);

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
// 	Description :display hot buttons & enable other buttons & hide cc dtmf panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_DisplayHotButtons(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
// 	Description :show cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_ShowDtmfPanel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description :open cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_HideDtmfPanel(MMI_WIN_ID_T win_id);

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//    Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void CC_ReappendDtmfPanelLayer(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
// 	Description :desplay buttons of cc win by cc_display_type
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_DisplayByType(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description :update cc_display_type by cc state
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_UpdateCcDisplayType(CC_DISPLAY_TYPE_E   cancel_cc_display_type);
#else
/*****************************************************************************/
//  Description : create hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateHotButtons(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : create one hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateNewHotButton(MMI_WIN_ID_T win_id, int32 row, int32 column, CC_CC_BUTTON_E new_button_type);

/*****************************************************************************/
//  Description :destory cc hot buttons
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DestroyCCHotButtonFormCtrl(MMI_WIN_ID_T win_id);
#endif

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : update gray or not gray state of all hot buttons
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_UpdateHotButtonState(void);
/*****************************************************************************/
//  Description : set gray state of hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_GetHotButtonGrayState(CC_CC_BUTTON_E button_type);

/*****************************************************************************/
//  Description : set gray state of hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_SetHotButtonGrayState(CC_CC_BUTTON_E button_type, BOOLEAN is_gray);

#endif

/*****************************************************************************/
// 	Description : SetStatusWinSoftkeyByCallState
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void SetStatusWinSoftkeyByCallState(void);

/*****************************************************************************/
// Description : set volume during call
// Global resource dependence :
// Author: jixin.xu
// Note:
/*****************************************************************************/
LOCAL void SetCallVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description : Open Volume Panel
//  Global resource dependence :
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenVolumePanel(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description :to open the call processing window
//  Global resource dependence : none
//  Author:bin.ji
//  Note:2004.11.8
/*****************************************************************************/
LOCAL void CC_OpenCallProcessingWin(void);

/*****************************************************************************/
// 	Description : to open call list window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_CallList(
    uint32	index
    );

/*****************************************************************************/
//  Description :get the text of call state
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  GetCallStateText(
    MMI_STRING_T		*state_ptr
    );

/*****************************************************************************/
// 	Description : whether the call need to dial again
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_OpenAutoDialWindow ( 
    void
    );

/*****************************************************************************/
// 	Description : to append the info to listbox
//	Global resource dependence : 
//  Author:louis.wei
//	Note: 
/*****************************************************************************/
LOCAL void AppendListItem(									
    uint8 					call_id,
    MMI_CTRL_ID_T	ctrl_id
    );

/*****************************************************************************/
//  Description :to open processing win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void OpenProcessingWin(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, MMI_IMAGE_ID_T img_id, MMI_ANIM_ID_T anim_id, PROCESSMSG_FUNC handle_func);  

#ifdef MMI_GUI_STYLE_TYPICAL 
/*****************************************************************************/
//  Description :to set connected win text
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetConnectedWinText(BOOLEAN is_update, MMI_STRING_T* prompt_str_array);
#endif

/*****************************************************************************/
//  Description :to set processing win text
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetProcessingWinText(BOOLEAN is_update, MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array);

/*****************************************************************************/
//  Description :set mo or mt win form param
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetCallingWinTextFormParam(void);  

/*****************************************************************************/
//  Description : create photo anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatePhotoAnim( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T anim_ctrl_id, GUI_BOTH_RECT_T* both_rect_ptr,  CC_PHOTO_TYPE_E photo_type);

/*****************************************************************************/
//  Description : display win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWinBg( MMI_WIN_ID_T win_id , BOOLEAN is_need_blind);      

/*****************************************************************************/
// 	Description : to open call list window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_OpenMultiCallList(void);

/*****************************************************************************/
//  Description : Set browser app active (means:open the browser)
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBrowserActive(void);

/*****************************************************************************/
// 	Description : is conditions of anykey-answer op satisfied
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsAnyKeyAnswerThisCall(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : GetRedialedTimes
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void GetRedialedTimes(MMI_STRING_T *redialed_times_ptr);

/*****************************************************************************/
// 	Description : send DTMF when a call come into active state
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E SendStoredDtmf(void);

/*****************************************************************************/
// 	Description : handle headset on or off in call
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleHeadsetInCall(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : 创建状态栏
//	Global resource dependence : 
//  Author:xiaoqinglu
//	Note:
/*****************************************************************************/
LOCAL void CreateStatusBarCtrl(
                               MMI_WIN_ID_T win_id,
                               MMI_IMAGE_ID_T bg_image_id
                               );

/*****************************************************************************/
//  Description :to answer a incoming call automatically, assoicated with eng
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_AutoAnswerIncomingCall(void);


#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateCurrentASPCall(void);
#endif

#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : copy magic voice data when selection window open and close
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL void CopyMagicVoiceData(MV_DATA_T* dst_data_ptr, MV_DATA_T* src_data_ptr);

/*****************************************************************************/
// 	Description : for list control display
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL void SetMagicVoiceListItemData(
                                     GUILIST_ITEM_T* item_t_ptr,
                                     GUILIST_ITEM_DATA_T* item_data_ptr,
                                     MMI_IMAGE_ID_T image_id,
                                     MMI_TEXT_ID_T text_id0,
                                     MMI_TEXT_ID_T text_id1,
                                     MMI_TEXT_ID_T msk_test_id
                                     );

/*****************************************************************************/
// 	Description : magic voice item selection window callback function
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceItemSelectWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     );

/*****************************************************************************/
// 	Description : magic voice main selection window callback function
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceMainSelectWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     );
                                                     
/*****************************************************************************/
// 	Description : open main selection window when incoming outgoing or in call
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL void OpenMagicVoiceSelectWindow(ADD_DATA call_info_ptr);
#endif

/*****************************************************************************/
//  Description :Stop Call Control Timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopCCTimer(uint8* timer_id);

/*****************************************************************************/
//  Description :Start Call Control Timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  StartCCTimer(MMI_HANDLE_T win_id, uint8 *timer_id, uint32 timeout);


#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : open emergency menu win when dial emergency num(112, 911 etc) 
//	Global resource dependence : 
//  Author:zihai.lu
//	Note:
/*****************************************************************************/
LOCAL void  OpenCCEmergencyMenu(void);

/*****************************************************************************/
// 	Description : handle emergency menu win msg when dial emergency num
//	Global resource dependence : 
//  Author:zihai.lu
//	Note:
/*****************************************************************************/
MMI_RESULT_E  MMICC_HandleEmergencyMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

/*****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

// the window for call list
WINDOW_TABLE( MMICC_CALLLIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleCCListWinMsg ),    
    WIN_ID( MMICC_CALLLIST_WIN_ID ),
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //listbox控件CC_CALLLISTWIN_LISTBOX_CTRL_ID
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICC_CALLLISTWIN_LISTBOX_CTRL_ID),
    END_WIN
}; 

// the window for multi call list
WINDOW_TABLE( MMICC_MULTI_CALLLIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleCCMultilistWinMsg ),    
    WIN_ID( MMICC_MULTICALL_LIST_WIN_ID ),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_CC_MULTICALL_VIEW),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICC_MULTICALL_LIST_CTRL_ID),    
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
    END_WIN
}; 


// the window for missed call
WINDOW_TABLE( MMICC_MISSEDCALL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleMissedcallWinMsg ),    
    WIN_ID( MMICC_MISSCALL_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

//显示使用SIM卡拨出的选项                                                
WINDOW_TABLE( MMICC_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleCallSIMSelectWinMsg),    
    WIN_ID(MMICC_COM_SELECT_SIM_WIN_ID),
    WIN_TITLE(TXT_SIM_SEL),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICC_COM_SELECT_SIM_CTRL_ID),
    END_WIN
};

//紧急呼叫窗口
WINDOW_TABLE( MMICC_EMERGENCY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleEmergencyCallWinMsg),    
    WIN_ID(MMICC_EMERGENCY_WIN_ID),
    WIN_TITLE(TXT_KL_PSW_EMERGENCY_CALL),
    CREATE_EDIT_PHONENUM_CTRL(CC_EMERGENCY_CALL_NUM_MAX_LEN, MMICC_EMERGENCY_PHONENUM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_SOS, TXT_NULL, STXT_RETURN),
#else
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    
    WIN_SOFTKEY(TXT_SOS, TXT_NULL, TXT_NULL),
#else
    WIN_SOFTKEY(TXT_SOS, TXT_NULL, TXT_DELETE),
#endif
#endif
    END_WIN
};

//cc hold menu
WINDOW_TABLE(MMICC_HOLDMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleHoldMenuWinMsg),
	WIN_ID(MMICC_HOLDMENU_WIN_ID),
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    WIN_TITLE(TXT_NULL),
#endif
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

WINDOW_TABLE( MMICC_MENU_COMM_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCCMenuWinMsg),
    WIN_ID(MMICC_MENU_WIN_ID),   
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CC_COMM_OPT,MMICC_MENUWIN_MENU_CTRL_ID),
#else    
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CC_COMM_OPT,MMICC_MENUWIN_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMICC_MENU_COMM_OPT2_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCCMenuWinMsg),
    WIN_ID(MMICC_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CC_COMM_OPT2,MMICC_MENUWIN_MENU_CTRL_ID),
#else    
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CC_COMM_OPT2,MMICC_MENUWIN_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMICC_MENU_RECEIVE_WC_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCCMenuWinMsg),
    WIN_ID(MMICC_MENU_WIN_ID),
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    WIN_TITLE(TXT_NULL),
#endif
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CC_RECEIVE_WC,MMICC_MENUWIN_MENU_CTRL_ID),
#else    
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CC_RECEIVE_WC,MMICC_MENUWIN_MENU_CTRL_ID),
#endif 
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#ifdef CMCC_UI_STYLE
//紧急呼叫窗口选项菜单
WINDOW_TABLE(MMICC_MENU_EMERGENCY_OPT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)MMICC_HandleEmergencyMenuWinMsg),
    WIN_ID(MMICC_MENU_EMERGENCY_WIN_ID),   
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

WINDOW_TABLE( MMICC_MENU_RECEIVE_WC2_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleCCMenuWinMsg),
    WIN_ID(MMICC_MENU_WIN_ID),
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    WIN_TITLE(TXT_NULL),
#endif
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/29
    CREATE_MENU_CTRL(MENU_CC_RECEIVE_WC2,MMICC_MENUWIN_MENU_CTRL_ID),
#else 		
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_CC_RECEIVE_WC2,MMICC_MENUWIN_MENU_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#ifdef MV_SUPPORT
WINDOW_TABLE( MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleMagicVoiceMainSelectWinMsg),    
    WIN_ID(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID),
    WIN_TITLE(TXT_CC_MAGICVOICE),
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_MODIFY, STXT_RETURN),
#endif
    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMICC_MAGIC_VOICE_MAIN_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMICC_MAGIC_VOICE_ITEM_SELECT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleMagicVoiceItemSelectWinMsg),    
    WIN_ID(MMICC_MAGIC_VOICE_ITEM_SELECT_WIN_ID),
    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICC_MAGIC_VOICE_ITEM_CTRL_ID),
    END_WIN
};
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
#include "mmk_tp.h"

#define KEY_LOCK_IN_CALL_TP_MOVE_TIME   20

//来电锁屏，屏幕item的rect
typedef enum
{
    CC_MT_LOCK_BG_RECT,
    CC_MT_LOCK_ANSWER_ICON_RECT,
    CC_MT_LOCK_REJECT_ICON_RECT,
    CC_MT_LOCK_UNLOCK_ICON_RECT,
#if defined(TOUCH_PANEL_SUPPORT)
    CC_MT_LOCK_UP_ANIM_RECT,
#else
    CC_MT_LOCK_LEFT_ANIM_RECT,
    CC_MT_LOCK_RIGHT_ANIM_RECT,
    CC_MT_LOCK_DOWN_ANIM_RECT,
#endif
    CC_MT_LOCK_OTHER_RECT,//invalid rect
    CC_MT_LOCK_RECT_MAX
}CC_MT_LOCK_RECT_E;

//来电锁屏，屏幕显示状态
typedef enum
{
    CC_MT_LOCK_DISPLAY_INIT,//全部显示
    CC_MT_LOCK_DISPLAY_MOVE_ANSWER,//按下answer icon的显示
    CC_MT_LOCK_DISPLAY_MOVE_REJECT,
    CC_MT_LOCK_DISPLAY_MOVE_UNLOCK,
    CC_MT_LOCK_DISPLAY_MAX
}CC_MT_LOCK_DISPLAY_E;

//来电锁屏，屏幕icon
typedef enum
{
    CC_MT_LOCK_ANSWER_ICON,
    CC_MT_LOCK_REJECT_ICON,
    CC_MT_LOCK_UNLOCK_ICON,
    CC_MT_LOCK_ICON_MAX
}CC_MT_LOCK_ICON_E;

#if defined(TOUCH_PANEL_SUPPORT) && (!defined(MMI_CALLCONTROL_MINI_SUPPORT))
GUI_LCD_DEV_INFO s_cc_mt_lock_display_layer = {0}; //锁屏界面静态图片文字显示层
GUI_LCD_DEV_INFO s_cc_mt_lock_move_layer = {0};//滑动图片显示层
GUI_LCD_DEV_INFO s_cc_mt_lock_anim_layer = {0};//滑动方向指示动画显示层


#define CC_UNLOCK_DIRECT_ANIM_Y_MARGIN 5
#define CC_UNLOCK_DIRECT_ANIM_TIME 200
#define CC_UNLOCK_DERECT_ANIM_FRAME_MAX 5

/*****************************************************************************/
// 	Description : 获得来电屏锁界面的item的显示区域
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CC_GetMtLockItemRect(CC_MT_LOCK_RECT_E rect_type, MMI_WIN_ID_T win_id)
{
    uint16 img_w = 0, img_h = 0;
    uint16 img_bg_w = 0, img_bg_h = 0;
    GUI_RECT_T mt_lock_bg_rect = {0};
    GUI_RECT_T mt_lock_icon_rect = {0};
    GUI_RECT_T *return_rect = PNULL;    
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUI_RECT_T client_rect = {0};
    uint16 lock_top = 0;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        client_rect = client_both_rect.h_rect;
        lock_top = MMI_MT_LOCK_TOP_H;
    }
    else
    {
        client_rect = client_both_rect.v_rect;
        lock_top = MMI_MT_LOCK_TOP;        
    }
    
    img_bg_h = client_rect.bottom - lock_top + 1;
    img_bg_w = client_rect.right;
    
    mt_lock_bg_rect.top = lock_top + 1;
    mt_lock_bg_rect.bottom = client_rect.bottom - 1;
    mt_lock_bg_rect.left = 0;
    mt_lock_bg_rect.right = client_rect.right -1;

    switch (rect_type)
    {
        case CC_MT_LOCK_BG_RECT:
            return_rect = &mt_lock_bg_rect;
            break;
            
        case CC_MT_LOCK_ANSWER_ICON_RECT:
            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMG_CC_LOCK_ANSWER_ICON_P,win_id);
            //answer icon
            mt_lock_icon_rect.top = mt_lock_bg_rect.top + img_bg_h/2;
            mt_lock_icon_rect.bottom = mt_lock_icon_rect.top + img_h - 1;
            mt_lock_icon_rect.left = mt_lock_bg_rect.left + 20;
            mt_lock_icon_rect.right = mt_lock_icon_rect.left + img_w -1;

            return_rect = &mt_lock_icon_rect;
            break;
            
        case CC_MT_LOCK_REJECT_ICON_RECT:
            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMG_CC_LOCK_REJECT_ICON_P,win_id);
            //reject icon
            mt_lock_icon_rect.top = mt_lock_bg_rect.top + img_bg_h/2;
            mt_lock_icon_rect.bottom = mt_lock_icon_rect.top + img_h - 1;
            mt_lock_icon_rect.right = mt_lock_bg_rect.right -20;
            mt_lock_icon_rect.left = mt_lock_icon_rect.right - img_w + 1;

            return_rect = &mt_lock_icon_rect;
            break;
            
        case CC_MT_LOCK_UNLOCK_ICON_RECT:
            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMG_CC_LOCK_UNLOCK_ICON_P,win_id);
            //unlock icon
            mt_lock_icon_rect.top = mt_lock_bg_rect.top + img_bg_h/2;
            mt_lock_icon_rect.bottom = mt_lock_icon_rect.top + img_h - 1;
            mt_lock_icon_rect.left = (client_rect.right + mt_lock_bg_rect.left - img_w)/2;
            mt_lock_icon_rect.right = mt_lock_icon_rect.left + img_w -1;

            return_rect = &mt_lock_icon_rect;
            break;

        case CC_MT_LOCK_UP_ANIM_RECT:
            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMAGE_CC_LOCK_ANIMATION_UP,win_id);

            mt_lock_icon_rect.top = mt_lock_bg_rect.top + 25;
            mt_lock_icon_rect.bottom = mt_lock_icon_rect.top + img_h - 1;
            mt_lock_icon_rect.left = (client_rect.right + mt_lock_bg_rect.left - img_w)/2;
            mt_lock_icon_rect.right = mt_lock_icon_rect.left + img_w -1;

            return_rect = &mt_lock_icon_rect;
            break;

        default:
            return_rect = &mt_lock_bg_rect;
            break;
    }
    
    return *return_rect;
}

#define LABEL_MARGIN 4

/*****************************************************************************/
// 	Description : 根据image的icon显示该icon对应的功能描述字串(Y 方位偏移)
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL void CC_MtLockDisplayLabelByRect(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id, GUI_RECT_T image_rect, int offset_y)
{
    GUILABEL_INIT_DATA_T label_init = {0};
    int16 label_width = 0;
    MMI_STRING_T label_str= {0};
    GUI_RECT_T  client_rc = {0};
    //label_bg.bg_type = GUI_BG_IMG;
    //label_bg.img_id = IMAGE_COMMON_BG;

    MMI_GetLabelTextByLang(text_id,&label_str);
    client_rc = MMITHEME_GetClientRect();
    label_width = GUI_GetStringWidth(MMI_CALL_NAME_FONT,label_str.wstr_ptr , label_str.wstr_len);
    label_width = MIN(label_width + LABEL_MARGIN,(client_rc.right - client_rc.left + 1));
    label_init.both_rect.v_rect.left = image_rect.left - (label_width - (image_rect.right - image_rect.left))/2;
    label_init.both_rect.v_rect.left =  (label_init.both_rect.v_rect.left< 0)? 0:label_init.both_rect.v_rect.left;
    label_init.both_rect.v_rect.right = label_init.both_rect.v_rect.left + label_width + 4;
    if (0 < offset_y)
    {
        label_init.both_rect.v_rect.top = image_rect.bottom + offset_y;
    }
    else
    {
        label_init.both_rect.v_rect.top = image_rect.top + offset_y - GUIFONT_GetHeight(MMI_CALL_NAME_FONT);
    }
    label_init.both_rect.v_rect.bottom =  label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_NAME_FONT);

    label_init.both_rect.h_rect.left = image_rect.left - (label_width - (image_rect.right - image_rect.left))/2;
    label_init.both_rect.h_rect.right = label_init.both_rect.h_rect.left + label_width;
    if (0 < offset_y)
    {
        label_init.both_rect.h_rect.top = image_rect.bottom + offset_y;
    }
    else
    {
        label_init.both_rect.h_rect.top = image_rect.top + offset_y - GUIFONT_GetHeight(MMI_CALL_NAME_FONT);
    }

    label_init.both_rect.h_rect.bottom =  label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_NAME_FONT);

    PdaCallCreateLabelByTextId(win_id,
                                                    ctrl_id, 
                                                    label_init, 
                                                    text_id, 
                                                    MMI_CALL_NAME_FONT, 
                                                    MMI_CALL_LOCK_MT_TEXT_COLOR);

    //GUILABEL_SetBg(ctrl_id, &label_bg);
}
/*****************************************************************************/
// 	Description : to process of animation window in the cc
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  CC_StartUnlockDirectAnim(MMI_WIN_ID_T win_id, GUI_RECT_T tp_start_rect)
{
    uint16 layer_left = 0;
    uint16 layer_top = 0;
    uint16 img_w = 0;
    uint16 img_h = 0;
    GUI_RECT_T image_rect = {0};
    GUIRES_GetImgWidthHeight(&img_w,&img_h, IMAGE_CC_LOCK_ANIMATION_UP,win_id);
    //set layer position
    layer_left = (tp_start_rect.right + tp_start_rect.left)/2 - img_w/2;
    layer_top = tp_start_rect.top - img_h - CC_UNLOCK_DIRECT_ANIM_Y_MARGIN;
    UILAYER_SetLayerPosition(&s_cc_mt_lock_anim_layer, layer_left, layer_top);
    //display image in layer
    image_rect.left = layer_left;
    image_rect.right = layer_left + img_w;
    image_rect.top = layer_top;
    image_rect.bottom = layer_top + img_h;
    GUIRES_DisplayImg(PNULL, &image_rect, PNULL, win_id, IMAGE_CC_LOCK_ANIMATION_UP,&s_cc_mt_lock_anim_layer);
    //set frame num 
    CC_INSTANCE.unlock_anim_frame = 0;
    //start anim timer
    StartCCTimer(win_id, &CC_INSTANCE.mmicc_anim_unlock_icon_timer, CC_UNLOCK_DIRECT_ANIM_TIME);
}
/*****************************************************************************/
// 	Description : to process of animation window in the cc
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  CC_PlayUnlockDirectAnimNextFrame(MMI_WIN_ID_T win_id, GUI_RECT_T tp_start_rect)
{
    uint16 layer_left = 0;
    uint16 layer_top = 0;
    uint16 img_w = 0;
    uint16 img_h = 0;
    GUI_RECT_T image_rect = {0};
    GUI_RECT_T rect = {0};
    GUIRES_GetImgWidthHeight(&img_w,&img_h, IMAGE_CC_LOCK_ANIMATION_UP,win_id);

    //set next frame num
    CC_INSTANCE.unlock_anim_frame++;
    if(CC_INSTANCE.unlock_anim_frame >= CC_UNLOCK_DERECT_ANIM_FRAME_MAX)
    {
        CC_INSTANCE.unlock_anim_frame = 0;
    }
    //set layer position
    rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
    if(0 < tp_start_rect.right && 0 < tp_start_rect.bottom)
    {
        rect = tp_start_rect;
    }
    layer_left = (rect.right + rect.left)/2 - img_w/2;
    layer_top = rect.top - img_h - CC_UNLOCK_DIRECT_ANIM_Y_MARGIN - img_h*CC_INSTANCE.unlock_anim_frame/2;
    
    UILAYER_SetLayerPosition(&s_cc_mt_lock_anim_layer, layer_left, layer_top);
    //display image in layer
    image_rect.left = layer_left;
    image_rect.right = layer_left + img_w;
    image_rect.top = layer_top;
    image_rect.bottom = layer_top + img_h;
    GUIRES_DisplayImg(PNULL, &image_rect, PNULL, win_id, IMAGE_CC_LOCK_ANIMATION_UP,&s_cc_mt_lock_anim_layer);
    //start anim timer
    StartCCTimer(win_id, &CC_INSTANCE.mmicc_anim_unlock_icon_timer, CC_UNLOCK_DIRECT_ANIM_TIME);
}
/*****************************************************************************/
// 	Description : to process of animation window in the cc
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  CC_StopUnlockDirectAnim(void)
{
    //clear layer
    UILAYER_Clear(&s_cc_mt_lock_anim_layer);
    //clear frame num
    CC_INSTANCE.unlock_anim_frame = 0;
    //stop anim timer
    StopCCTimer(&CC_INSTANCE.mmicc_anim_unlock_icon_timer);
}
/*****************************************************************************/
// 	Description : 显示来电屏锁界面(根据不同的显示类型)
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL void CC_DisplayMtLock(MMI_WIN_ID_T win_id, CC_MT_LOCK_DISPLAY_E display_type, GUI_RECT_T *icon_rect_ptr)
{
    GUI_RECT_T item_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    UILAYER_Clear(&s_cc_mt_lock_display_layer);
    UILAYER_Clear(&s_cc_mt_lock_move_layer);
    UILAYER_Clear(&s_cc_mt_lock_anim_layer);
    
    if(s_cc_mt_lock_display_layer.block_id != 0)
    {
        //window是否是GET_FOCUS状态，是则在各自的层上画图；否则画在主层上。否则，在主层进行FULL_PAINT时，就会擦去已经blt的图片
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_cc_mt_lock_display_layer))
        {
            lcd_dev_info = s_cc_mt_lock_display_layer;
            //在window由LOSE_FOCUS状态转到GET_FOCUS时，需要将画图所用的层重新加入到layer队列中去(s_layer_blt)
            {
                UILAYER_APPEND_BLT_T append_layer = {0};
                append_layer.lcd_dev_info.lcd_id = s_cc_mt_lock_display_layer.lcd_id;
                append_layer.lcd_dev_info.block_id = s_cc_mt_lock_display_layer.block_id;
                append_layer.layer_level = UILAYER_LEVEL_NORMAL;
                UILAYER_AppendBltLayer(&append_layer);

                append_layer.lcd_dev_info.lcd_id = s_cc_mt_lock_move_layer.lcd_id;
                append_layer.lcd_dev_info.block_id = s_cc_mt_lock_move_layer.block_id;
                append_layer.layer_level = UILAYER_LEVEL_NORMAL;
                UILAYER_AppendBltLayer(&append_layer);

                append_layer.lcd_dev_info.lcd_id = s_cc_mt_lock_anim_layer.lcd_id;
                append_layer.lcd_dev_info.block_id = s_cc_mt_lock_anim_layer.block_id;
                append_layer.layer_level = UILAYER_LEVEL_NORMAL;
                UILAYER_AppendBltLayer(&append_layer);                
            }
        }
    }

    //move direction
    item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UP_ANIM_RECT,win_id);
    //GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMAGE_CC_LOCK_ANIMATION_UP,&lcd_dev_info);
    CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_TIPS_LABEL_CTRL_ID, TXT_CC_MT_LOCK_TIPS, item_rect, -5);
    
    switch (display_type)
    {
    case CC_MT_LOCK_DISPLAY_INIT:
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_ANSWER_ICON,&lcd_dev_info);
        CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_ANSWER_LABEL_CTRL_ID, TXT_COMMON_ANSWER_CALL, item_rect, 2);

        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_REJECT_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_REJECT_ICON,&lcd_dev_info);
        CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_REJECT_LABEL_CTRL_ID, TXT_CC_DECLINE, item_rect, 2);

        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_UNLOCK_ICON,&lcd_dev_info);
        CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_UNLOCK_LABEL_CTRL_ID, TXT_CC_MT_LOCK_UNLOCK, item_rect, 2);
        CC_StopUnlockDirectAnim();
        CC_StartUnlockDirectAnim(win_id, item_rect);
        break;
        
    case CC_MT_LOCK_DISPLAY_MOVE_ANSWER:       
        UILAYER_SetLayerPosition(&s_cc_mt_lock_move_layer, icon_rect_ptr->left , icon_rect_ptr->top );
        GUIRES_DisplayImg(PNULL, icon_rect_ptr, PNULL, win_id, IMG_CC_LOCK_ANSWER_ICON_P,&s_cc_mt_lock_move_layer);
        CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_ANSWER_LABEL_CTRL_ID, TXT_COMMON_ANSWER_CALL, *icon_rect_ptr, 2);
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
        CC_StopUnlockDirectAnim();
        CC_StartUnlockDirectAnim(win_id, item_rect);
        
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_REJECT_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_REJECT_ICON,&lcd_dev_info);

        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_UNLOCK_ICON,&lcd_dev_info);
        break;
        
    case CC_MT_LOCK_DISPLAY_MOVE_REJECT:
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_ANSWER_ICON,&lcd_dev_info);

        UILAYER_SetLayerPosition(&s_cc_mt_lock_move_layer, icon_rect_ptr->left , icon_rect_ptr->top );
        GUIRES_DisplayImg(PNULL, icon_rect_ptr, PNULL, win_id, IMG_CC_LOCK_REJECT_ICON_P,&s_cc_mt_lock_move_layer);
        CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_REJECT_LABEL_CTRL_ID, TXT_CC_DECLINE, *icon_rect_ptr, 2);
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_REJECT_ICON_RECT,win_id);
        CC_StopUnlockDirectAnim();
        CC_StartUnlockDirectAnim(win_id, item_rect);
        
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_UNLOCK_ICON,&lcd_dev_info);
        break;
        
    case CC_MT_LOCK_DISPLAY_MOVE_UNLOCK:
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_ANSWER_ICON,&lcd_dev_info);
    
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_REJECT_ICON_RECT,win_id);
        GUIRES_DisplayImg(PNULL, &item_rect, PNULL, win_id, IMG_CC_LOCK_REJECT_ICON,&lcd_dev_info);

        UILAYER_SetLayerPosition(&s_cc_mt_lock_move_layer, icon_rect_ptr->left , icon_rect_ptr->top );
        GUIRES_DisplayImg(PNULL, icon_rect_ptr, PNULL, win_id, IMG_CC_LOCK_UNLOCK_ICON_P,&s_cc_mt_lock_move_layer);
        CC_MtLockDisplayLabelByRect(win_id, MMICC_MT_LOCK_UNLOCK_LABEL_CTRL_ID, TXT_CC_MT_LOCK_UNLOCK, *icon_rect_ptr, 2);
        item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
        CC_StopUnlockDirectAnim();
        CC_StartUnlockDirectAnim(win_id, item_rect);
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : 创建来电屏锁界面所需的layer
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CC_CreateLockLayerInCall(MMI_WIN_ID_T win_id, GUI_RECT_T rect)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 width = 0, height = 0;
    
    if(rect.top < rect.bottom && rect.left < rect.right)
    {
        width = rect.right - rect.left + 1;
        height = rect.bottom - rect.top + 1;
        
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        result = UILAYER_CreateLayer(&create_info, &layer);
        SCI_TRACE_LOW("ccapp create lock layer res: %d", result);
        if(UILAYER_RESULT_SUCCESS == result)
        {
            UILAYER_APPEND_BLT_T append_layer = {0};
            append_layer.lcd_dev_info.lcd_id = layer.lcd_id;
            append_layer.lcd_dev_info.block_id = layer.block_id;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);
    
            UILAYER_SetLayerPosition(&layer, rect.left, rect.top);
            UILAYER_Clear(&layer);
        }
    }
    return layer;
}

/*****************************************************************************/
// 	Description : 获得point所在的来电屏锁item
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_MT_LOCK_RECT_E CC_GetMtLockItemByPoint(GUI_POINT_T pos, MMI_WIN_ID_T win_id)
{
    CC_MT_LOCK_RECT_E rect_type = CC_MT_LOCK_OTHER_RECT;
    GUI_RECT_T item_rect = {0};
    GUI_RECT_T image_rect = {0};
    GUI_RECT_T label_rect = {0};
    
    image_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
    GUILABEL_GetRect(MMICC_MT_LOCK_ANSWER_LABEL_CTRL_ID, &label_rect);
    item_rect.left = image_rect.left - 2;
    item_rect.top = image_rect.top;
    item_rect.right = image_rect.right + 2;
    item_rect.bottom = label_rect.bottom;
    if (GUI_PointIsInRect(pos, item_rect))
    {
        return CC_MT_LOCK_ANSWER_ICON_RECT;
    }
    
    image_rect = CC_GetMtLockItemRect(CC_MT_LOCK_REJECT_ICON_RECT,win_id);
    GUILABEL_GetRect(MMICC_MT_LOCK_REJECT_LABEL_CTRL_ID, &label_rect);
    item_rect.left = image_rect.left - 2;
    item_rect.top = image_rect.top;
    item_rect.right = image_rect.right + 2;
    item_rect.bottom = label_rect.bottom;    
    if (GUI_PointIsInRect(pos, item_rect))
    {
        return CC_MT_LOCK_REJECT_ICON_RECT;
    }

    image_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
    GUILABEL_GetRect(MMICC_MT_LOCK_UNLOCK_LABEL_CTRL_ID, &label_rect);
    item_rect.left = image_rect.left - 2;
    item_rect.top = image_rect.top;
    item_rect.right = image_rect.right + 2;
    item_rect.bottom = label_rect.bottom;    
    if (GUI_PointIsInRect(pos, item_rect))
    {
        return CC_MT_LOCK_UNLOCK_ICON_RECT;
    }

    return rect_type;
}


/*****************************************************************************/
// 	Description : to process of animation window in the cc
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL void  CC_HandleCcLockMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    static GUI_POINT_T tp_cusor_point = {0};//tp down/up 点
    static GUI_POINT_T tp_last_point = {0};//cursor移动过程中控制
    static GUI_POINT_T tp_start_point = {0};//按中可移动按钮时记录下此时的cursor坐标，icon移动的范围依据
    static GUI_RECT_T tp_start_rect = {0};//按中可移动按钮时记录下此时的rect区域，作为layer 移动的起始依据
    static GUI_RECT_T tp_target_rect = {0};//目标区域
    static BOOLEAN s_is_icon_moving = FALSE;
    static CC_MT_LOCK_ICON_E s_move_icon_type = CC_MT_LOCK_ICON_MAX;

    GUI_RECT_T accept_icon_rect = {0};

    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleCcLockMsg() msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_2276_112_2_18_2_10_44_309,(uint8*)"d", msg_id);
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T mt_lock_static_layer_rect = {0, MMI_MT_LOCK_TOP, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1};
            GUI_RECT_T mt_lock_move_layer_rect = {0, 0, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1};
            GUI_RECT_T mt_lock_anim_layer_rect = {0};
            uint16 img_w = 0, img_h = 0;
            GUI_BOTH_RECT_T full_rect = MMITHEME_GetFullScreenBothRect();
   
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_INIT);
            MMK_SetWinAngle(win_id, GUILCD_GetInitAngle(0), FALSE);
            if(MMITHEME_IsMainScreenLandscape())
            {
                mt_lock_move_layer_rect = mt_lock_static_layer_rect = full_rect.h_rect; 
                //mt_lock_static_layer_rect.top = MMI_MT_LOCK_TOP_H;
            }
            else
            {
                mt_lock_move_layer_rect = mt_lock_static_layer_rect = full_rect.v_rect; 
                //mt_lock_static_layer_rect.top = MMI_MT_LOCK_TOP;
            }
            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMG_CC_LOCK_ANSWER_ICON_P,win_id);
            mt_lock_move_layer_rect.bottom = img_h - 1;
            mt_lock_move_layer_rect.right = img_w - 1;

            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMAGE_CC_LOCK_ANIMATION_UP,win_id);
            mt_lock_anim_layer_rect.bottom = img_h - 1;
            mt_lock_anim_layer_rect.right = img_w - 1;

           accept_icon_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
           mt_lock_static_layer_rect.top = accept_icon_rect.top;
           mt_lock_static_layer_rect.bottom = accept_icon_rect.bottom;

            //init
            SCI_MEMSET((char*)&tp_cusor_point, 0, sizeof(GUI_POINT_T));
            SCI_MEMSET((char*)&tp_last_point, 0, sizeof(GUI_POINT_T));
            SCI_MEMSET((char*)&tp_start_point, 0, sizeof(GUI_POINT_T));
            SCI_MEMSET((char*)&tp_start_rect, 0, sizeof(GUI_RECT_T));
            SCI_MEMSET((char*)&tp_target_rect, 0, sizeof(GUI_RECT_T));
            s_is_icon_moving = FALSE;
            s_move_icon_type = CC_MT_LOCK_ICON_MAX;
            BLKMEM_GarbageClean();            
            s_cc_mt_lock_display_layer = CC_CreateLockLayerInCall(win_id, mt_lock_static_layer_rect);
            s_cc_mt_lock_move_layer = CC_CreateLockLayerInCall(win_id, mt_lock_move_layer_rect);
            s_cc_mt_lock_anim_layer = CC_CreateLockLayerInCall(win_id, mt_lock_anim_layer_rect);
            //set label text color
#ifndef MMI_MULTISIM_COLOR_SUPPORT
            GUILABEL_SetFont(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, MMI_CALL_LOCK_MT_TEXT_COLOR);
#endif
            GUILABEL_SetFont(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, MMI_CALL_LOCK_MT_TEXT_COLOR);
            GUILABEL_SetFont(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, MMI_CALL_LOCK_MT_TEXT_COLOR);
            //横屏时设置label 位置
            if(MMITHEME_IsMainScreenLandscape())
            {
#define LABEL_LOCK_MARGIN_Y (CC_MAIN_INFO_NAME_LABEL_TOP - CC_MAIN_INFO_ANIM_TOP_H)
            
                GUI_BOTH_RECT_T label_both_rect = {0};
                MMI_HANDLE_T ctrl_handle = 0;
                GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
                
                ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_NAME_LABEL_CTRL_ID);
                GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                label_both_rect.h_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT;
                label_both_rect.h_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT;
                label_both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                label_both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
                GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);

                ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID);
                GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                label_both_rect.h_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT;
                label_both_rect.h_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT;
                label_both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                label_both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);

                ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID);
                GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                label_both_rect.h_rect.left = CC_MAIN_INFO_LOCAT_LABEL_LEFT;
                label_both_rect.h_rect.right = CC_MAIN_INFO_LOCAT_LABEL_RIGHT;
                label_both_rect.h_rect.top = CC_MAIN_INFO_LOCAT_LABEL_TOP + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                label_both_rect.h_rect.bottom =  CC_MAIN_INFO_LOCAT_LABEL_BOTTOM + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);

                ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_STATE_LABEL_CTRL_ID);
                GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                label_both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT;
                label_both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT;
                label_both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                label_both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM + client_rect.v_rect.top - LABEL_LOCK_MARGIN_Y;/*lint !e737*/
                GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);                
            }
            
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_MT_LOCK)
        {
            GUI_RECT_T item_rect = {0};
                    
            tp_cusor_point.x = MMK_GET_TP_X(param);
            tp_cusor_point.y = MMK_GET_TP_Y(param);
            
            {
                MMI_TP_STATUS_E tp_status;
                GUI_POINT_T point = {0};
                MMK_GetLogicTPMsg(&tp_status, &point);

                tp_start_point.x = point.x;
                tp_start_point.y = point.y;
            }

            //icon移动目标rect
            item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UP_ANIM_RECT,win_id);
            MMI_MEMCPY (&tp_target_rect, sizeof(GUI_RECT_T), &item_rect, sizeof(GUI_RECT_T), sizeof(GUI_RECT_T));

            switch (CC_GetMtLockItemByPoint(tp_cusor_point, win_id))
            {              
            case CC_MT_LOCK_BG_RECT:
                break;
                
            case CC_MT_LOCK_ANSWER_ICON_RECT:
                {
                    item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_ANSWER_ICON_RECT,win_id);
                    MMI_MEMCPY (&tp_start_rect, sizeof(GUI_RECT_T), &item_rect, sizeof(GUI_RECT_T), sizeof(GUI_RECT_T));

                    CC_DisplayMtLock(win_id, CC_MT_LOCK_DISPLAY_MOVE_ANSWER, &tp_start_rect);

                    StartCCTimer(win_id, &CC_INSTANCE.mmicc_move_unlock_icon_timer, KEY_LOCK_IN_CALL_TP_MOVE_TIME);
                    s_is_icon_moving = TRUE;
                    s_move_icon_type = CC_MT_LOCK_ANSWER_ICON;
                }
                break;
                
            case CC_MT_LOCK_REJECT_ICON_RECT:
                {                 
                    item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_REJECT_ICON_RECT,win_id);
                    MMI_MEMCPY (&tp_start_rect, sizeof(GUI_RECT_T), &item_rect, sizeof(GUI_RECT_T), sizeof(GUI_RECT_T));
                    
                    CC_DisplayMtLock(win_id, CC_MT_LOCK_DISPLAY_MOVE_REJECT, &tp_start_rect);

                    StartCCTimer(win_id, &CC_INSTANCE.mmicc_move_unlock_icon_timer, KEY_LOCK_IN_CALL_TP_MOVE_TIME);
                    s_is_icon_moving = TRUE;
                    s_move_icon_type = CC_MT_LOCK_REJECT_ICON;
                }
                break;
                
            case CC_MT_LOCK_UNLOCK_ICON_RECT:
                {                                        
                    item_rect = CC_GetMtLockItemRect(CC_MT_LOCK_UNLOCK_ICON_RECT,win_id);
                    MMI_MEMCPY (&tp_start_rect, sizeof(GUI_RECT_T), &item_rect, sizeof(GUI_RECT_T), sizeof(GUI_RECT_T));

                    CC_DisplayMtLock(win_id, CC_MT_LOCK_DISPLAY_MOVE_UNLOCK, &tp_start_rect);

                    StartCCTimer(win_id, &CC_INSTANCE.mmicc_move_unlock_icon_timer, KEY_LOCK_IN_CALL_TP_MOVE_TIME);
                    s_is_icon_moving = TRUE;
                    s_move_icon_type = CC_MT_LOCK_UNLOCK_ICON;
                }
                break;

            case CC_MT_LOCK_OTHER_RECT:
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_MT_LOCK && s_is_icon_moving)
        {
            int16 offset_x = 0, offset_y = 0;
            uint16 img_w = 0, img_h = 0;
            
            static GUI_POINT_T move_icon_center_point = {0};
            
            s_is_icon_moving = FALSE;
            
            UILAYER_GetLayerPosition(&s_cc_mt_lock_move_layer, &offset_x , &offset_y );
            GUIRES_GetImgWidthHeight(&img_w,&img_h, IMG_CC_LOCK_ANSWER_ICON,win_id);
            move_icon_center_point.x = offset_x + img_w/2;
            move_icon_center_point.y = offset_y + img_h/2;
                                
            StopCCTimer(&CC_INSTANCE.mmicc_move_unlock_icon_timer);

            if (GUI_PointIsInRect(move_icon_center_point, tp_target_rect) ||move_icon_center_point.y < tp_target_rect.top)
            {
                switch (s_move_icon_type)
                {
                    case CC_MT_LOCK_ANSWER_ICON:
                        MMICC_AnswerCall();
                        break;
                        
                    case CC_MT_LOCK_REJECT_ICON:
                        MMIAPICC_ReleaseCallByRedkey();
                        MMICC_StopRingOrVibrateBeforeCCing();
                        break;
                        
                    case CC_MT_LOCK_UNLOCK_ICON:
                        //恢复通话界面动画控件正常背景色
                        GUIANIM_SetBgImg(MMK_GetCtrlHandleByWin(win_id, MMICC_CALLING_ANIM_FORM_CTRL_ID), IMAGE_COMMON_BG);
#ifdef MMI_KEY_LOCK_SUPPORT
                        MMIAPIKL_UnlockPhone();
#endif
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
                        if (MMIAPICC_IsMtCallPlayingRing())
                        {
                            CC_CreateNewHotButtonWithoutForm(win_id, CC_HOT_BUTTON_ROW_MAX - 1, 1, CC_BUTTON_SILENCE);
                        }
                        else
                        {
                            CC_CreateNewHotButtonWithoutForm(win_id, CC_HOT_BUTTON_ROW_MAX - 1, 1, CC_BUTTON_MUTE);
                        } 
#endif
                        CC_UpdateCcDisplayType(CC_DISPLAY_MT_LOCK);
                        CC_DisplayByType(win_id);
                        break;
                    
                    default:
                        break;
                }
            }
            SCI_MEMSET((char*)&tp_start_rect, 0, sizeof(GUI_RECT_T));
            CC_StopUnlockDirectAnim();

            if (CC_INSTANCE.cc_display_type == CC_DISPLAY_MT_LOCK)
            {
                CC_DisplayMtLock(win_id, CC_MT_LOCK_DISPLAY_INIT, PNULL);                
            }
            else
            {              
                UILAYER_ReleaseLayer(&s_cc_mt_lock_display_layer);
                UILAYER_ReleaseLayer(&s_cc_mt_lock_move_layer);
                UILAYER_ReleaseLayer(&s_cc_mt_lock_anim_layer);
                //解决因为窗口背景不一导致的label背景色问题
                {
                    GUI_BG_T label_bg = {0};
                    label_bg.bg_type = GUI_BG_IMG;
                    label_bg.img_id = IMAGE_COMMON_BG;
                    GUILABEL_SetBg(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &label_bg);
                    GUILABEL_SetBg(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, &label_bg);
                    GUILABEL_SetBg(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, &label_bg);
                    GUILABEL_SetBg(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &label_bg);
                    //MMK_SetWinBackgroundImageId(win_id, IMAGE_COMMON_BG);
                }
                //set label text color
#ifndef MMI_MULTISIM_COLOR_SUPPORT
                GUILABEL_SetFont(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, CC_TEXT_COLOR);
#endif
                GUILABEL_SetFont(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, CC_TEXT_COLOR);
                GUILABEL_SetFont(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, CC_TEXT_COLOR);

                MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_ALL);
                //横屏时设置label 位置
                if(MMITHEME_IsMainScreenLandscape())
                {
                    GUI_BOTH_RECT_T label_both_rect = {0};
                    MMI_HANDLE_T ctrl_handle = 0;
                    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
                    
                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_NAME_LABEL_CTRL_ID);
                    GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                    label_both_rect.h_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT_H;
                    label_both_rect.h_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT_H;
                    label_both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top;/*lint !e737*/
                    label_both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
                    GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);  
                    //GUILABEL_SetRect(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &(label_both_rect.h_rect), FALSE);

                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID);
                    GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                    label_both_rect.h_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT_H;
                    label_both_rect.h_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT_H;
                    label_both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
                    label_both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/
                    GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);  
                    //GUILABEL_SetRect(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, &(label_both_rect.h_rect), FALSE);

                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID);
                    GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                    label_both_rect.h_rect.left = CC_MAIN_INFO_LOCAT_LABEL_LEFT_H;
                    label_both_rect.h_rect.right = CC_MAIN_INFO_LOCAT_LABEL_RIGHT_H;
                    label_both_rect.h_rect.top = CC_MAIN_INFO_LOCAT_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
                    label_both_rect.h_rect.bottom =  CC_MAIN_INFO_LOCAT_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/
                    GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);  
                    //GUILABEL_SetRect(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, &(label_both_rect.h_rect), FALSE);

                    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_STATE_LABEL_CTRL_ID);
                    GUIAPICTRL_GetBothRect(ctrl_handle, &label_both_rect);
                    label_both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT_H;
                    label_both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT_H;
                    label_both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP_H + client_rect.v_rect.top;/*lint !e737*/
                    label_both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM_H + client_rect.v_rect.top;/*lint !e737*/
                    GUIAPICTRL_SetBothRect(ctrl_handle, &label_both_rect);    
                    //GUILABEL_SetRect(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &(label_both_rect.h_rect), FALSE);
                    
                }
                
                //destroy lock win ctrl
                MMK_DestroyControl(MMICC_MT_LOCK_TIPS_LABEL_CTRL_ID);    
                MMK_DestroyControl(MMICC_MT_LOCK_ANSWER_LABEL_CTRL_ID);   
                MMK_DestroyControl(MMICC_MT_LOCK_REJECT_LABEL_CTRL_ID);   
                MMK_DestroyControl(MMICC_MT_LOCK_UNLOCK_LABEL_CTRL_ID);
                
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_TIMER:
        if (CC_INSTANCE.mmicc_move_unlock_icon_timer == *(uint8*)param)
        {
            MMI_TP_STATUS_E tp_status;
            int16 offset_y = 0;
            GUI_POINT_T point = {0};
            int16 lock_top = MMI_MT_LOCK_TOP;
            MMK_GetLogicTPMsg(&tp_status, &point);
            offset_y = point.y - tp_cusor_point.y;

            if(MMITHEME_IsMainScreenLandscape())
            {
                lock_top = MMI_MT_LOCK_TOP_H;
            }
            if(tp_last_point.y != point.y)
            {
                if (point.y < tp_start_point.y && point.y >= lock_top)
                {
                    tp_last_point = point;
                    UILAYER_SetLayerPosition(&s_cc_mt_lock_move_layer, tp_start_rect.left, tp_start_rect.top + offset_y);
                }
            }

            StartCCTimer(win_id, &CC_INSTANCE.mmicc_move_unlock_icon_timer, KEY_LOCK_IN_CALL_TP_MOVE_TIME);
        }
        else if(CC_INSTANCE.mmicc_anim_unlock_icon_timer == *(uint8*)param)
        {
            CC_PlayUnlockDirectAnimNextFrame( win_id,  tp_start_rect);
        }
        break;

    default:
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleCcLockMsg() WRONG param! enter default branch!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_2594_112_2_18_2_10_45_310,(uint8*)"");
        break;
    }
}


#endif
#endif

/*****************************************************************************/
// 	Description :Clear Auto Redial Info
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:CLEAR_AUTOREDIAL_INFO() 
/*****************************************************************************/
LOCAL void ClearAutoRedialInfo(void)
{
    CC_INSTANCE.autodial_info.num = 0;
    CC_INSTANCE.autodial_info.num_max = 0;
}

/*****************************************************************************/
// 	Description : update cc status win
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_UpdateStatusWin(void)
{
    if (MMICC_STATUS_WIN_ID == MMK_GetFocusWinId())
    {
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
// 	Description : open cc hold menu
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void  OpenCCHoldMenu(void)
{
    MMI_WIN_ID_T	win_id = MMICC_HOLDMENU_WIN_ID;
    MMI_CTRL_ID_T	ctrl_id = MMICC_HOLDMENUWIN_MENU_CTRL_ID;
    MMI_TEXT_ID_T	label = TXT_NULL;
#ifdef MMI_GUI_STYLE_TYPICAL
#ifdef DPHONE_SUPPORT //不能通过菜单切换
#else    
    MMI_TEXT_ID_T	handhold_label = TXT_NULL;
#endif
#endif
    uint32			i = 0;
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address = {0};    
#endif
    
    MMK_CloseWin(win_id);

    CHECK_CC_APPLET_NOT_NULL
        
    MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32 *)MMICC_HOLDMENU_WIN_TAB,PNULL);

    //creat dynamic menu   
#ifdef DPHONE_SUPPORT 
    {
        GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

        GUIMENU_CreatDynamic(&both_rect, MMICC_HOLDMENU_WIN_ID,ctrl_id,GUIMENU_STYLE_THIRD);
    }
#else
    GUIMENU_CreatDynamic(PNULL,MMICC_HOLDMENU_WIN_ID,ctrl_id,GUIMENU_STYLE_POPUP);
#endif
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    {
        MMI_STRING_T            titlestr = {0};       
        MMI_GetLabelTextByLang(STXT_OPTION, &titlestr);
        GUIMENU_SetMenuTitle(&titlestr, ctrl_id);
    }
#endif

    i = 0;
    
    //SCI_TRACE_LOW:"mmiccapp:OpenCCHoldMenu, cur dev = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_2651_112_2_18_2_10_45_311,(uint8*)"d", MMICC_GetCurrentDev());

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef BT_DIALER_SUPPORT
    if(!MMICC_GetBtCallFlag())
#endif
{
#ifdef MV_SUPPORT
    if (IsCanUseMagicVoice(TRUE))
    {
        label = TXT_CC_MAGICVOICE;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_MAGICVOICE,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	    0,//IMAGE_NULL,
#else
            IMAGE_CC_OPTMENU_MAGIC_VOICE,
#endif            
            FALSE
            ); 
    }
#endif
}
#ifdef BT_DIALER_SUPPORT
     if(!MMICC_GetBtCallFlag())
#endif
{
    //handon
    if ((MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
#ifdef BLUETOOTH_SUPPORT
        &&MMIAPIBT_GetActivatedHS(&address)
#endif
        )
    {
        if (GPIO_CheckHeadsetStatus())
        {
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_CC_SEND_VOICE_TO_HEADSET,//text_ptr 
                CC_HOLDMENU_NODE_HANDHOLD,//node_id
                0, //parent_node_id
                i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	         0,//IMAGE_NULL,
#else                
                IMAGE_CC_OPTMENU_VOICE_TO_HEADSET,
#endif                
                FALSE
                ); 
        }
        else
        {
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_CC_SEND_VOICE_TO_PHONE,//text_ptr 
                CC_HOLDMENU_NODE_HANDHOLD,//node_id
                0, //parent_node_id
                i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	    	  0,//IMAGE_NULL,
#else                
                IMAGE_CC_OPTMENU_VOICE_TO_PHONE,
#endif                
                FALSE
                ); 
        }
    }
    
#ifdef BLUETOOTH_SUPPORT
    //bluetooth
    if (MMIAPIBT_GetActivatedHS(&address)&&(!(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())))
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_CC_SEND_VOICE_TO_BT,//text_ptr 
            CC_HOLDMENU_NODE_BLUETOOTH,//node_id
            0, //parent_node_id
            i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	    0,//IMAGE_NULL,
#else            
            IMAGE_CC_OPTMENU_VOICE_TO_BT,
#endif           
            FALSE
            );    
    }
    
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_BT_CONNECT_HF,//text_ptr 
        CC_HOLDMENU_NODE_BT_CONNECT_HF,//node_id
        0, //parent_node_id
        i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	    0,//IMAGE_NULL,
#else        
        IMAGE_CC_OPTMENU_CONNECT_BT,
#endif       
        FALSE
        ); 
#endif
#endif
}
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    //add new
#ifdef BT_DIALER_SUPPORT
    if(!MMICC_GetBtCallFlag())
#endif
{
    if ( MMICC_IsExistActiveAndHoldCalls())
    {
    	label= TXT_CC_MERGE_CALLS;
       InsertNodeByLabel(
        ctrl_id,//ctrl_id
        label,//text_ptr 
        CC_HOLDMENU_NODE_MERGE_CALLS,//node_id
        0, //parent_node_id
        i++,//index
        0,//IMAGE_NULL,
        FALSE
        );
    }
    else
    {
    	label= TXT_CC_ADD_CALL;    
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        label,//text_ptr
        CC_HOLDMENU_NODE_NEW_CALL,//node_id
        0, //parent_node_id
        i++,//index
        0,//IMAGE_NULL,
        FALSE
        );
    }
    
    //hold
    if ( MMICC_IsExistActiveAndHoldCalls())
    {
    	label= TXT_CC_SWAP_PDA;
    }
    else if(MMICC_IsExistHoldCall())
    {
    	label= TXT_CC_UNHOLD_PDA;
    }
    else
    {
        label = TXT_CC_HOLD_PDA;
    } 
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        label,//text_ptr 
        CC_HOLDMENU_NODE_SHUTTLE,//node_id
        0, //parent_node_id
        i++,//index
        0,//IMAGE_NULL,
        FALSE
        );
	 //-----record
#ifdef DPHONE_SUPPORT 
#else
#ifdef MMI_RECORD_SUPPORT
#ifdef MV_SUPPORT//魔音下不支持录音
    if (!IsMagicVoiceWorking())
#endif
    {
#if defined (BLUETOOTH_SUPPORT) && defined (BT_DIALER_SUPPORT)
        if((!MMIBT_IsConnectDevice(&address)) && (!(MMIAPIBT_GetActivatedHS(&address)&&((MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())))))
#endif
        {
            label = GetRecordLabelForCCHoldMenu();
            InsertNodeByLabel(
                ctrl_id,
                label,
                CC_HOLDMENU_NODE_RECORD,
                0,
                (uint16)i++,//index
                0,//IMAGE_NULL,
                FALSE
            );
        }
    }
#endif
#endif
}
	    //-----mute
	    label = (MMICC_IsCurrentCallMute())?TXT_TURNOFF_MUTE:TXT_TURNON_MUTE;
	    InsertNodeByLabel(
	        ctrl_id,//ctrl_id
	        label,//text_ptr 
	        CC_HOLDMENU_NODE_MUTE,//node_id
	        0, //parent_node_id
	        (uint16)i++,//index
	        0,//IMAGE_NULL,
	        FALSE
	        );
#endif
    //volume
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_RING_VOLUME,//text_ptr 
        CC_HOLDMENU_NODE_VOLUME,//node_id
        0, //parent_node_id
        i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT        
        0,//IMAGE_NULL,
#else        
        IMAGE_CC_OPTMENU_ADJUST_VOLUME,
#endif        
        FALSE
        );

#ifdef BT_DIALER_SUPPORT
    if(!MMICC_GetBtCallFlag())
#endif
{
#if defined(MMI_ECT_MENU_SUPPORT) && defined(MMI_CRSS_SUPPORT)
    //ECT Transfer: must : one active call+one hold call or one active call+one waiting call
    if (MMICC_ECTIsPermit())
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_CC_ECT_TRANSFER,//text_ptr 
            CC_HOLDMENU_NODE_ECT_TRANSFER,//node_id
            0, //parent_node_id
            i++,//index
            0,//IMAGE_NULL,
            FALSE
            ); 
    }
#endif
}
    //---pb
    InsertNodeByLabel(
        ctrl_id,
        STXT_MAIN_PB,
        CC_HOLDMENU_NODE_PB,
        0,
        (uint16)i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT        
        0,//IMAGE_NULL,
#else        
        IMAGE_CC_OPTMENU_PB,
#endif        
        FALSE
        );    

    
    //call log
    InsertNodeByLabel(
        ctrl_id,
        STXT_MAIN_CL,
        CC_HOLDMENU_NODE_CL,
        0,
        (uint16)i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT        
        0,//IMAGE_NULL,
#else
        IMAGE_CC_OPTMENU_CALL_LOG,
#endif
        FALSE
        );

    InsertNodeByLabel(
        ctrl_id,
        TXT_COMMUNICATION_MESSAGE,
        CC_HOLDMENU_NODE_SMS,
        0,
        (uint16)i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT        
        0,//IMAGE_NULL,
#else
        IMAGE_CC_OPTMENU_SMS,
#endif
        FALSE
        );

    //-----browser
#ifdef BROWSER_SUPPORT_NETFRONT
    if (MMI_GMMREG_RAT_3G == MMIAPIPHONE_GetTDOrGsm(MMIAPICC_GetCallDualSys()))
    {
        label = TXT_CC_TO_BROWSER;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_BROWSER,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	    0,//IMAGE_NULL,
#else            
            IMAGE_CC_OPTMENU_BROWSER,
#endif            
            FALSE
            ); 
    }
#endif

#ifdef BT_DIALER_SUPPORT
    if(!MMICC_GetBtCallFlag())
#endif
{
#ifdef MMI_CC_STK_MENU_SUPPORT
    if (MMIAPISTK_IsStkCard(MMIAPICC_GetCallDualSys()))//for orange
    {
        BOOLEAN is_stk_name_exist = FALSE;
        MMI_STRING_T stk_name = {0};
    
        is_stk_name_exist = MMIAPISTK_GetStkName(MMIAPICC_GetCallDualSys(), &stk_name);

        if (!is_stk_name_exist || 0 == stk_name.wstr_len)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, &stk_name);
        }

        InsertNodeByString(
            ctrl_id,//ctrl_id
            stk_name,//text_ptr 
            CC_HOLDMENU_NODE_STK,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
	    0,//IMAGE_NULL,
#else            
            IMAGE_CC_OPTMENU_SMS,
#endif            
            FALSE
            );
    }
#endif

#if !defined(TOUCH_PANEL_SUPPORT)
    //view multi call list
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    if(MMICC_IsExistActiveMPTY()
    || (MMICC_IsExistHoldMPTY() && !MMICC_IsExistActiveCall()))
    {
        label = TXT_CC_MULTICALL_VIEW;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_VIEW_MULTICALL,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_CC_OPTMENU_MPTY_AND_ANSWER,
            FALSE
            );
    }
#else
    if(MMICC_IsExistMPTY())
    {
        label = TXT_CC_MULTICALL_VIEW;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_VIEW_MULTICALL,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            0,//IMAGE_NULL,
            FALSE
            );
    }
#endif
#endif

    if (1 < MMICC_GetCallNum())
    {
        label = TXT_CC_END_ALL_CALLS;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_END_CALLS,//node_id
            0, //parent_node_id
            (uint16)i++,//index
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
            IMAGE_CC_OPTMENU_END_ALL_CALL,
#else
            0,//IMAGE_NULL,
#endif
            FALSE
            ); 
    }
}
#else/*MMI_GUI_STYLE_TYPICAL*/
#ifdef DPHONE_SUPPORT 
#else    

#ifdef MV_SUPPORT
    if (IsCanUseMagicVoice(TRUE))
    {
        label = TXT_CC_MAGICVOICE;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_MAGICVOICE,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_NULL,
            FALSE
            ); 
    }
#endif

    //handfree
#ifdef MMI_ENABLE_HANDFREE
#ifdef MV_SUPPORT//目前系统不支持魔音功能和免提同时开启
    if (!MMIAPICC_IsHandFree() && !IsMagicVoiceWorking())
#else
    if (!MMIAPICC_IsHandFree())
#endif
    {
        label = TXT_TURNON_HANDFREE;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_HANDFREE,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_NULL,
            FALSE
            ); 
    }
#endif

    //bluetooth
#ifndef BT_DIALER_SUPPORT
#ifdef BLUETOOTH_SUPPORT
    if (MMIAPIBT_GetActivatedHS(&address)&&(!(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())))
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_BLUETOOTH,//text_ptr 
            CC_HOLDMENU_NODE_BLUETOOTH,//node_id
            0, //parent_node_id
            i++,//index
            IMAGE_NULL,
            FALSE
            );    
    }
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_BT_CONNECT_HF,//text_ptr 
        CC_HOLDMENU_NODE_BT_CONNECT_HF,//node_id
        0, //parent_node_id
        i++,//index
        IMAGE_NULL,
        FALSE
        ); 
    #endif
#endif
#endif

    //handon
    if (MMICC_AUDIO_DEV_HH != MMICC_GetCurrentDev() && MMICC_AUDIO_DEV_EP != MMICC_GetCurrentDev())
    {
        if (GPIO_CheckHeadsetStatus())
        {
            handhold_label = TXT_HAND_ON;//如果以后有合适的名字添加为耳机之类的
        }
        else
        {
            handhold_label = TXT_HAND_ON;
        }
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            handhold_label,//text_ptr 
            CC_HOLDMENU_NODE_HANDHOLD,//node_id
            0, //parent_node_id
            i++,//index
            IMAGE_NULL,
            FALSE
            ); 
    }
#endif 

#if defined(MMI_ECT_MENU_SUPPORT) && defined(MMI_CRSS_SUPPORT)
    //ECT Transfer: must : one active call+one hold call or one active call+one waiting call
    if (MMICC_ECTIsPermit())
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_CC_ECT_TRANSFER,//text_ptr 
            CC_HOLDMENU_NODE_ECT_TRANSFER,//node_id
            0, //parent_node_id
            i++,//index
            IMAGE_NULL,
            FALSE
            ); 
    }
#endif

    //-----shuttle
    InsertNodeByLabel(
        ctrl_id,
        TXT_CC_SHUTTLE,
        CC_HOLDMENU_NODE_SHUTTLE,
        0,
        (uint16)i++,//index
        IMAGE_NULL,
        FALSE
        );

    //-----dtmf
    //当前电话不在多方通话中,并且处于通话过程中，才可以设置多方通话
    if ((CC_CALL_STATE == MMICC_GetCurrentCallStatus())
        &&(!MMICC_IsCallIndexInMPTY(MMICC_GetCurrentCallIndex())))
    {
        if (CC_INSTANCE.is_dtmf_switch)
        {
            label = TXT_DTMF_CLOSE;
        }
        else
        {
            label = TXT_DTMF_OPEN;
        }

        InsertNodeByLabel(
            ctrl_id,
            label,
            CC_HOLDMENU_NODE_DTMF,
            0,
            (uint16)i++,//index
            IMAGE_NULL,
            FALSE
            );
    }

    //---pb
    InsertNodeByLabel(
        ctrl_id,
        STXT_MAIN_PB,
        CC_HOLDMENU_NODE_PB,
        0,
        (uint16)i++,//index
        IMAGE_NULL,
        FALSE
        );    
    
    //call log
    InsertNodeByLabel(
        ctrl_id,
        STXT_MAIN_CL,
        CC_HOLDMENU_NODE_CL,
        0,
        (uint16)i++,//index
        IMAGE_NULL,
        FALSE
        );
        
    InsertNodeByLabel(
        ctrl_id,
        TXT_COMMUNICATION_MESSAGE,
        CC_HOLDMENU_NODE_SMS,
        0,
        (uint16)i++,//index
        IMAGE_NULL,
        FALSE
        );

    //-----record
#ifdef MMI_RECORD_SUPPORT
#ifdef DPHONE_SUPPORT 
#else    
#ifdef MV_SUPPORT//魔音下不支持录音
    if (!IsMagicVoiceWorking())
#endif
   {
#if defined (BLUETOOTH_SUPPORT) && defined (BT_DIALER_SUPPORT)
        if((!MMIBT_IsConnectDevice(&address)) && (!(MMIAPIBT_GetActivatedHS(&address)&&((MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())))))
#endif
        {
            label = GetRecordLabelForCCHoldMenu();
            InsertNodeByLabel(
                ctrl_id,
                label,
                CC_HOLDMENU_NODE_RECORD,
                0,
                (uint16)i++,//index
                IMAGE_NULL,
                FALSE
            );
        }
    }

#endif
#endif
    //-----mute
    label = (MMICC_IsCurrentCallMute())?TXT_TURNOFF_MUTE:TXT_TURNON_MUTE;
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        label,//text_ptr 
        CC_HOLDMENU_NODE_MUTE,//node_id
        0, //parent_node_id
        (uint16)i++,//index
        IMAGE_NULL,
        FALSE
        );

    //-----browser
#ifdef BROWSER_SUPPORT
    if (MMI_GMMREG_RAT_3G == MMIAPIPHONE_GetTDOrGsm(MMIAPICC_GetCallDualSys()))
    {
        label = TXT_CC_TO_BROWSER;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_BROWSER,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_NULL,
            FALSE
            ); 
    }
#endif

#ifdef MMI_CC_STK_MENU_SUPPORT
    if (MMIAPISTK_IsStkCard(MMIAPICC_GetCallDualSys()))//for orange
    {
        BOOLEAN is_stk_name_exist = FALSE;
        MMI_STRING_T stk_name = {0};
    
        is_stk_name_exist = MMIAPISTK_GetStkName(MMIAPICC_GetCallDualSys(), &stk_name);

        if (!is_stk_name_exist || 0 == stk_name.wstr_len)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, &stk_name);
        }

        InsertNodeByString(
            ctrl_id,//ctrl_id
            stk_name,//text_ptr 
            CC_HOLDMENU_NODE_STK,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_NULL,
            FALSE
            );
    }
#endif

    //build multiparty call
    if (MMICC_IsExistActiveAndHoldCalls())
    {
        if (MMICC_IsMptySetupAvailable())
        {
            label = TXT_CC_BUILDMPTY;
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                label,//text_ptr
                CC_HOLDMENU_NODE_BUILDMPTY,//node_id
                0, //parent_node_id
                (uint16)i++,//index
                IMAGE_NULL,
                FALSE
                );            
        }
    }    

    //view multi call list
    if(MMICC_IsExistMPTY())
    {
        label = TXT_CC_MULTICALL_VIEW;
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            label,//text_ptr 
            CC_HOLDMENU_NODE_VIEW_MULTICALL,//node_id
            0, //parent_node_id
            (uint16)i++,//index
            IMAGE_NULL,
            FALSE
            );
    }
#endif  /*MMI_GUI_STYLE_TYPICAL*/
}

#ifdef MMI_GUI_STYLE_TYPICAL
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
#endif

/*****************************************************************************/
//  Description : 重拨次数超过限制添加该号码到黑名单中
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:参见GSM 02.07
/*****************************************************************************/
LOCAL void CC_AddItemtoBlackList(uint8* tele_num, uint8 tele_len)
{
    uint32 i = 0;

    CHECK_CC_APPLET_NOT_NULL
    //把号码加入到合适的位置
    for (i = 0; i < CC_MAX_AUTOCALL_BLACKLIST; i++)
    {
        if (!CC_INSTANCE.call_repeat_info.autocall_blacklist[i].is_blacklisted)
        {
            CC_INSTANCE.call_repeat_info.autocall_blacklist[i].is_blacklisted = TRUE;   //是否加入黑名单
            CC_INSTANCE.call_repeat_info.autocall_blacklist[i].failed_time = MMIAPICOM_GetCurTime();//黑名单中失败的时间
            SCI_MEMCPY(CC_INSTANCE.call_repeat_info.autocall_blacklist[i].tele_num, tele_num, tele_len);
            CC_INSTANCE.call_repeat_info.autocall_blacklist[i].tele_len = tele_len;
            break;
        }
    }
    //达到名单的最大数目，黑名单已满
    if (i >= CC_MAX_AUTOCALL_BLACKLIST)
    {
        CC_INSTANCE.call_repeat_info.is_full = TRUE;
    }
    
    //清空记录拨叫次数的值等
    ClearAutoRedialInfo();
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
MMI_RESULT_E HandleCallSIMSelectWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              cur_selection = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_MAX;
    MMI_CTRL_ID_T       ctrl_id = MMICC_COM_SELECT_SIM_CTRL_ID;
    CC_MAKE_CALL_INFO *make_call_info_ptr = (CC_MAKE_CALL_INFO *)MMK_GetWinAddDataPtr(win_id);
    uint32 item_data = 0;
    uint16 cur_index = 0;
    uint16 top_index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        GUILIST_SetMaxItem(ctrl_id, MMICC_SELECT_SIM_MAX_NUM, FALSE );//max item 2
        MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMICC_SELECT_SIM_MAX_NUM, MMISET_APPEND_SYS_OK);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        GUILIST_SetTopItemIndex(ctrl_id, top_index);
        break;
        
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:    
    case MSG_APP_CALL4:     
    case MSG_APP_OK:
#if !defined(MMI_PDA_SUPPORT) || defined(MMI_CALLCONTROL_MINI_SUPPORT)
    case MSG_CTL_OK:
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(PNULL != make_call_info_ptr)
        {
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
            if(GUILIST_IsCurrItemInvalid(ctrl_id))
            {
                break;
            }
        
            GUILIST_GetItemData(ctrl_id, cur_selection, &item_data);

            if(item_data < MMI_DUAL_SYS_MAX)
            {
                dual_sys = (MN_DUAL_SYS_E)item_data;
            
                MMICC_ProcessMakeCall(dual_sys, make_call_info_ptr);
            }
            else
            {
                //SCI_TRACE_LOW:"ccapp HandleCallSIMSelectWinMsg,item_data[%d] >= MMI_DUAL_SYS_MAX,ERROR!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3240_112_2_18_2_10_46_312,(uint8*)"d",item_data);
            }
        
            MMK_CloseWin(win_id);
        }
        else
        {
            //SCI_TRACE_LOW:"ccapp HandleCallSIMSelectWinMsg make info ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3247_112_2_18_2_10_46_313,(uint8*)"");
        }
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_CANCEL:
#endif
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        //free memory
        if (PNULL != make_call_info_ptr)
        {
            if (PNULL != make_call_info_ptr->disp_ptr)
            {
                if (PNULL != make_call_info_ptr->disp_ptr->wstr_ptr)
                {
                    SCI_FREE(make_call_info_ptr->disp_ptr->wstr_ptr);
                    make_call_info_ptr->disp_ptr->wstr_ptr = PNULL;
                }
                
                SCI_FREE(make_call_info_ptr->disp_ptr);
                make_call_info_ptr->disp_ptr = PNULL;
            }
            
            if (PNULL != make_call_info_ptr->name_ptr)
            {
                if (PNULL != make_call_info_ptr->name_ptr->wstr_ptr)
                {
                    SCI_FREE(make_call_info_ptr->name_ptr->wstr_ptr);
                    make_call_info_ptr->name_ptr->wstr_ptr = PNULL;
                }
                
                SCI_FREE(make_call_info_ptr->name_ptr);
                make_call_info_ptr->name_ptr = PNULL;
            }
            
            SCI_FREE(make_call_info_ptr);
            make_call_info_ptr = PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : connect sim name and status string, copy to txt_str
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_CallStateDisplaying( uint16 str_len, wchar* state_str)
{
#ifdef MMI_GUI_STYLE_TYPICAL

    uint8 sim_str_len = 0;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T   sim_name = {0};
    wchar sim_str[MMISET_MAX_SIM_NAME_LEN + 1] = {0};//字符串sim1 GB转成ucs2后长度*2加上两个终结符总共10
#endif 
    uint8 line_str_len = 0;
#ifdef MMI_CSP_SUPPORT
    MMI_STRING_T line_str = {0};
#endif
    wchar space_ch = ' ';
    
    if(str_len > CC_CALL_STATE_STR_MAX_LEN)
    {
        //SCI_TRACE_LOW:"ccapp str is too long"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3326_112_2_18_2_10_47_315,(uint8*)"");
        return;
    }

    //get sim str
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && !defined(MMI_MULTI_SIM_SYS_NONE) 
    //add for 双卡双待呼叫界面提示呼叫使用的sim卡
    sim_name = MMIAPISET_GetSimName(MMIAPICC_GetCallDualSys());
    
    //SCI_TRACE_LOW:"mmicc: CC_CallStateDisplaying, s_call_context.dual_sys = %d, sim_name.str_arr = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3318_112_2_18_2_10_47_314,(uint8*)"ds", MMIAPICC_GetCallDualSys(), sim_name.wstr_arr);
    
    sim_str_len = sim_name.wstr_len + 1;
    MMI_WSTRNCPY(sim_str, sim_str_len, sim_name.wstr_arr, sim_str_len, sim_str_len);
    MMI_WSTRNCPY(sim_str + sim_str_len, 1, &space_ch, 1, 1);
#endif//MMI_MULTI_SIM_SYS_SINGLE

#ifdef MMI_CSP_SUPPORT
    line = MMIAPISET_GetCallLineInfo(MMIAPICC_GetCallDualSys());
    if(0 == line)
    {
        MMI_GetLabelTextByLang(TXT_LINE_1, &line_str);
    }
    else if(1 == line)
    {
        MMI_GetLabelTextByLang(TXT_LINE_2, &line_str);
    }
    line_str_len = line_str.wstr_len;
#endif//MMI_CSP_SUPPORT

    //free CC_INSTANCE.sim_status
    CC_FreeSimStatusSpace();
    
    CC_INSTANCE.sim_status.wstr_len = (line_str_len > 0)?(sim_str_len + line_str_len + 1 + str_len):(sim_str_len + str_len);

    CC_INSTANCE.sim_status.wstr_ptr = SCI_ALLOC_APPZ((CC_INSTANCE.sim_status.wstr_len + 1)* sizeof(wchar));

    MMI_WSTRNCPY(CC_INSTANCE.sim_status.wstr_ptr, sim_str_len, sim_str, sim_str_len, sim_str_len);
    if(line_str_len > 0)
    {
 #ifdef MMI_CSP_SUPPORT   
        MMI_WSTRNCPY(CC_INSTANCE.sim_status.wstr_ptr + sim_str_len, line_str_len, line_str.wstr_ptr, line_str_len, line_str_len);
 #endif
        MMI_WSTRNCPY(CC_INSTANCE.sim_status.wstr_ptr + sim_str_len + line_str_len, 1, &space_ch, 1, 1);
        MMI_WSTRNCPY(CC_INSTANCE.sim_status.wstr_ptr + sim_str_len + line_str_len + 1, str_len, state_str, str_len, str_len);
    }
    else
    {
        MMI_WSTRNCPY(CC_INSTANCE.sim_status.wstr_ptr + sim_str_len , str_len, state_str, str_len, str_len);
    }
    
#else //MMI_GUI_STYLE_TYPICAL
    if(str_len > CC_CALL_STATE_STR_MAX_LEN)
    {
        //SCI_TRACE_LOW:"ccapp str is too long"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3326_112_2_18_2_10_47_315,(uint8*)"");
        return;
    }
    
    //free CC_INSTANCE.sim_status
    CC_FreeSimStatusSpace();
    
    CC_INSTANCE.sim_status.wstr_len = str_len;
    CC_INSTANCE.sim_status.wstr_ptr = SCI_ALLOC_APPZ((CC_INSTANCE.sim_status.wstr_len + 1)* sizeof(wchar));
    MMI_WSTRNCPY(CC_INSTANCE.sim_status.wstr_ptr, str_len, state_str, str_len, str_len);
#endif //MMI_GUI_STYLE_TYPICAL
}

/*****************************************************************************/
//  Description : Free s_sim_status 
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_FreeSimStatusSpace(void)
{
    if (PNULL != CC_INSTANCE.sim_status.wstr_ptr)
    {
        SCI_FREE(CC_INSTANCE.sim_status.wstr_ptr);/*lint !e666*/
        CC_INSTANCE.sim_status.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : set mt win softkey
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_SetCallWinSoftkey(
    MMI_WIN_ID_T	        win_id,
    MMI_TEXT_ID_T 		left_sk_id,
    MMI_TEXT_ID_T 		mid_sk_id,
    MMI_TEXT_ID_T         right_sk_id,
    BOOLEAN               is_fresh
    )
{    
    if (MMK_IsOpenWin(win_id))
    {
        // GUIWIN_SetSoftkeyTextId(win_id,  left_sk_id, mid_sk_id, right_sk_id, is_fresh);
        GUIWIN_SeSoftkeytButtonTextId(win_id, left_sk_id, LEFT_BUTTON, is_fresh);

        if (!MMI_IsTextNull(mid_sk_id))
        {
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
            GUIWIN_SeSoftkeytButtonIconId(win_id, mid_sk_id, MIDDLE_BUTTON, is_fresh);
#else        	
            GUIWIN_SeSoftkeytButtonTextId(win_id, mid_sk_id, MIDDLE_BUTTON, is_fresh);
#endif            
        }

        GUIWIN_SeSoftkeytButtonTextId(win_id, right_sk_id, RIGHT_BUTTON, is_fresh);
    }
    else
    {
        //SCI_TRACE_LOW:"CC_SetCallWinSoftkey:win_id %d is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3391_112_2_18_2_10_47_316,(uint8*)"d",win_id);
    }
}

/*****************************************************************************/
//  Description : set hanf free menu 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetHandFreeMenu(
    MMI_WIN_ID_T			win_id,
    MMI_TEXT_ID_T 		left_sk_id,
    MMI_TEXT_ID_T 		mid_sk_id,
    MMI_TEXT_ID_T 		right_sk_id,
    BOOLEAN is_need_fresh)
{
    if (MMICC_IsExistIncommingCall())
    {
        CC_SetCallWinSoftkey(win_id, left_sk_id, mid_sk_id, right_sk_id, is_need_fresh);
    }
    else
    {
        CC_SetCallWinSoftkey(win_id,left_sk_id,mid_sk_id,TXT_CC_HANG_OFF,is_need_fresh);
    }
}

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set cc win RS key 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetConectedWinRightSoftkey(
    MMI_WIN_ID_T win_id,
    BOOLEAN is_need_fresh)
{
    MMI_TEXT_ID_T rs_text_id = TXT_NULL;
    
#if !defined(MMI_GUI_STYLE_TYPICAL)
    if (CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL)
    {
        rs_text_id = STXT_RETURN;
    }
    else
#endif
    {
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        if(MMIAPICC_IsHandFree())
        {
            rs_text_id = TXT_HAND_ON;
        }
        else
        {
            rs_text_id = TXT_COMMON_HAND_FREE;
        }
 #else
	rs_text_id = TXT_CC_HANG_OFF;
 #endif
    }

    GUIWIN_SeSoftkeytButtonTextId(win_id, rs_text_id, RIGHT_BUTTON, is_need_fresh);
}
#endif
/*****************************************************************************/
//  Description : only dial emergency call when locked
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_OpenEmergencyWin(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN is_mergencycall = FALSE;
    MMI_STRING_T key_str = {0};
    wchar key_code = 0;
    uint16 str_len = 0;

    key_str.wstr_ptr = &key_code;
    
    switch(msg_id)
    {
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_0:
        key_code = MMIAPICC_MsgidToAscii(msg_id);
        key_str.wstr_len = sizeof(key_code)/sizeof(wchar);
        if(MMIAPICC_IsInputEmergencyCall(&key_str, &str_len, MMI_DUAL_SYS_MAX))
        {
            is_mergencycall = TRUE;
            MMK_CreatePubEditWin((uint32*)MMICC_EMERGENCY_WIN_TAB, PNULL);
            
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
            {
                uint32 i = 0;
                uint32 j = 0;
                uint8 emergency_call_len = 0;
                const MMIPLMN_EMERGENCY_CALL_T *local_emergency_list_ptr = PNULL;

                for (j = 0; j < MMI_DUAL_SYS_MAX; j++)
                {
                    local_emergency_list_ptr = MMIAPIPLMN_GetLocalEmergency(i);

                    if (PNULL != local_emergency_list_ptr)
                    {
                        for (i = 0; ; i++)
                        {
                            if (PNULL != local_emergency_list_ptr[i].emergency_call_ptr)
                            {
                                uint8 temp_tele_len = strlen(local_emergency_list_ptr[i].emergency_call_ptr);

                                if (emergency_call_len < temp_tele_len)
                                {
                                    emergency_call_len = temp_tele_len;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                if (0 == emergency_call_len)
                {
                    emergency_call_len = CC_EMERGENCY_CALL_NUM_MAX_LEN;
                }

                GUIEDIT_SetMaxLen(MMICC_EMERGENCY_PHONENUM_CTRL_ID, emergency_call_len);
            }
#endif

            if (CC_INVALID_DTMF_VALUE != key_code)
            {
                GUIEDIT_SetString(
                    MMICC_EMERGENCY_PHONENUM_CTRL_ID,
                    &key_code,
                    1);
            }
        }
        break;
        
    default:
        is_mergencycall = FALSE;
        break;
    }
    
    return is_mergencycall;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_PostMsgToDetachCnfWaitWin(void)
{
    return MMK_PostMsg(MMICC_DETACHCNF_WAIT_WIN_ID, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, PNULL, 0);
}

/*****************************************************************************/
// 	Description : send DTMF when a call come into active state
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E SendStoredDtmf(void)
{
    uint32          i = 0;
    uint32          dtmf_len = 0;
    uint32          remain_len = 0;
    uint8           copy_len = 0;
    uint8           dtmf_str[CC_MAX_TELE_NUM_LEN+1] = {0};
    BOOLEAN         is_end = FALSE;
    MMI_WIN_ID_T    alert_win_id = MMICC_SEND_DTMF_WIN_ID;
    MMI_STRING_T    alert_str = {0};
    wchar           temp_alert_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
    MMI_STRING_T    promt_str = {0};
    CC_RESULT_E     is_dtmf = CC_RESULT_SUCCESS;
    BOOLEAN         is_valid_dtmf = FALSE;
    CC_DTMF_STATE_T* dtmf_info_ptr = MMICC_GetStoredDtmfInfo();
    //MMI_STRING_T    total_alert_str = {0};
    
    //check the param
    if( !MMICC_IsCurrentCallIDValid()) 
    {
        //SCI_TRACE_LOW("ccapp.c: MMICC_SendStoredDtmf , cur_call_id = %d", s_call_context.current_call);
        return CC_RESULT_ERROR;
    }
    alert_str.wstr_ptr = temp_alert_str;
    //total_alert_str.wstr_ptr = &(s_call_context.dtmf_state.dtmf_str[1]);
    // total_alert_str.wstr_len = s_call_context.dtmf_state.dtmf_len - 1;
    
    MMI_MEMCPY(
        (char*)dtmf_info_ptr->dtmf_str,
        dtmf_info_ptr->dtmf_len,
        &(dtmf_info_ptr->dtmf_str[1]),
        (dtmf_info_ptr->dtmf_len - 1),
        (dtmf_info_ptr->dtmf_len - 1)
        );
    dtmf_info_ptr->dtmf_len--;
    //s_call_context.dtmf_state.dtmf_len = total_alert_str.wstr_len;
    SCI_MEMSET((char*)(&dtmf_info_ptr->dtmf_str[dtmf_info_ptr->dtmf_len]),0, 1);
    
    //if the extension number exist
    if(dtmf_info_ptr->is_dtmf)
    {
        //从有效的DTMF位置开始检查dtmf字符串
        for(i = 0; i < dtmf_info_ptr->dtmf_len; i++)
        {
            is_valid_dtmf = ('P' == dtmf_info_ptr->dtmf_str[i] || 'W' == dtmf_info_ptr->dtmf_str[i] ||
                'p' == dtmf_info_ptr->dtmf_str[i] || 'w' == dtmf_info_ptr->dtmf_str[i]);
            
            if(is_valid_dtmf)
            {
                //update s_call_context and store extension number			
                dtmf_len = i;
                
                if(dtmf_len > 0)
                {
                    dtmf_info_ptr->is_dtmf = TRUE;
                    copy_len = ( dtmf_len > CC_MAX_TELE_NUM_LEN ) ? CC_MAX_TELE_NUM_LEN : dtmf_len;
                    
                    MMI_MEMCPY(
                        (char*)dtmf_str,
                        dtmf_len,
                        dtmf_info_ptr->dtmf_str,
                        dtmf_info_ptr->dtmf_len,
                        copy_len
                        );
                    
                    //发送有效的DTMF
                    MMICC_SendDTMFStr(dtmf_str, dtmf_len);
                }
                is_end = FALSE;
                
                break;
            }
        }
        
        //如果字符串全部监测完毕没有发现w,p则发送后面所有的DTMF
        if (i >= dtmf_info_ptr->dtmf_len)
        {
            is_end = TRUE;
        }
        
        //如果字符串中不再包含p,w,n则直接呼出
        if (is_end)
        {
            //alert_str.wstr_ptr = s_call_context.dtmf_state.dtmf_str;
            MMI_STRNTOWSTR(alert_str.wstr_ptr, 
                dtmf_info_ptr->dtmf_len, 
                (uint8 *)dtmf_info_ptr->dtmf_str,
                dtmf_info_ptr->dtmf_len, 
                dtmf_info_ptr->dtmf_len);
            alert_str.wstr_len = dtmf_info_ptr->dtmf_len;
            MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
            
            MMIPUB_SetAlertWinTextByPtr(alert_win_id, &promt_str, &alert_str, TRUE);
            MMK_PostMsg(alert_win_id, MSG_FULL_PAINT, PNULL,0);
            
            //DTMF字符串第一个元素为字母需要去掉字母发送
            MMICC_SendDTMFStr(dtmf_info_ptr->dtmf_str, dtmf_info_ptr->dtmf_len);
            
            //clear dtmf infomation in s_call_context
            MMICC_ClearStoredDtmf();
            
            MMK_CloseWin(MMICC_SEND_DTMF_WIN_ID);
            MMK_CloseWin(MMICC_SEND_DTMF_W_WIN_ID);
        }
        else
        {
            //获取下一个有效的DTMF字符串，去掉已经发送的DTMF和第一个字符p或w
            dtmf_info_ptr->dtmf_len = dtmf_info_ptr->dtmf_len - copy_len;
            remain_len = ( dtmf_info_ptr->dtmf_len > CC_MAX_TELE_NUM_LEN ) ? CC_MAX_TELE_NUM_LEN : dtmf_info_ptr->dtmf_len;
            
            if(copy_len < CC_MAX_TELE_NUM_LEN)
            {
                MMI_MEMCPY(
                    (char*)dtmf_str,
                    remain_len,
                    (dtmf_info_ptr->dtmf_str + copy_len),
                    dtmf_info_ptr->dtmf_len,
                    remain_len
                );
            }
            
            //去除DTMF已经发送的部分，重新设置dtmf字符串
            dtmf_info_ptr->dtmf_len = remain_len;
            SCI_MEMSET(dtmf_info_ptr->dtmf_str, NULL, sizeof(dtmf_info_ptr->dtmf_str));
            MMI_MEMCPY(
                (char*)dtmf_info_ptr->dtmf_str,
                sizeof(dtmf_info_ptr->dtmf_str),
                dtmf_str,
                remain_len,
                remain_len
                );
            
            //判断是否已经只有字母存在，如果是则DTMF已经无效
            is_dtmf = MMICC_IsDTMF(dtmf_info_ptr->dtmf_str, dtmf_info_ptr->dtmf_len);
            if (is_dtmf)
            {
                return CC_RESULT_SUCCESS;
            }
            promptSendDtmfAfterCallSetup();
        }
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : the process message function of insert dtmf
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICC_HandleInsertDTMFWinMsg( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        {
            MMI_STRING_T    title_str = {0};
            MMIRES_GetText(TXT_CC_COMMUN, PNULL, &title_str);
            MMIPUB_SetWinTitleText(win_id, &title_str, FALSE);

            MMIPUB_SetWinSoftkey(win_id, TXT_SEND, STXT_CANCEL, FALSE);
        }
#endif
        //if (!MMICC_IsSendingStoredDtmf())
        {
            MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
            CC_INSTANCE.dtmf_timer_id = MMI_NONE_TIMER_ID;
        }
#ifdef MMI_GUI_STYLE_TYPICAL
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
#endif
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (CC_INSTANCE.dtmf_timer_id == MMI_NONE_TIMER_ID)
        {
            CC_INSTANCE.dtmf_timer_id = MMK_CreateWinTimer(MMICC_STATUS_WIN_ID, CC_DTMF_WAITING_TIME, FALSE);
        }
        {
            wchar temp_alert_wchar[CC_MAX_TELE_NUM_LEN + 1] = {0};
            MMI_STRING_T alert_str = {0};
            MMI_STRING_T promt_str = {0};
            uint32 alert_time = 0;
            MMI_WIN_ID_T alert_win_id = MMICC_SEND_DTMF_WIN_ID;
            CC_DTMF_STATE_T dtmf_info = *MMICC_GetStoredDtmfInfo();
           
            alert_str.wstr_ptr = temp_alert_wchar;
            alert_str.wstr_len = dtmf_info.dtmf_len - 1;
            MMI_STRNTOWSTR(alert_str.wstr_ptr,alert_str.wstr_len, (uint8 *)&(dtmf_info.dtmf_str[1]), alert_str.wstr_len,alert_str.wstr_len);
            MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
            if (MMK_IsOpenWin(alert_win_id))
            {
                MMK_CloseWin(alert_win_id);
            }
            MMIPUB_OpenAlertWinByTextPtrEx(CC_APPLET_HANDLE,&alert_time, &promt_str, &alert_str,IMAGE_PUBWIN_WAIT, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, MMICC_HandleSendingDTMFWinMsg,0);
        }
        
        MMK_CloseWin(win_id);
        
        if(CC_RESULT_SUCCESS != SendStoredDtmf())
        {
            //SCI_TRACE_LOW:"ccapp.c:MMICC_HandleInsertDTMFWinMsg() send dtmf failure"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_3718_112_2_18_2_10_47_317,(uint8*)"");
        }

        break;
        
    case MSG_APP_CANCEL:
        MMICC_ClearStoredDtmf();
        MMK_CloseWin(MMICC_SEND_DTMF_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}
/*****************************************************************************/
// 	Description : the process message function of the sending dtmf 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICC_HandleSendingDTMFWinMsg( 
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id) {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        {
            MMI_STRING_T    title_str = {0};
            MMIRES_GetText(TXT_CC_COMMUN, PNULL, &title_str);
            MMIPUB_SetWinTitleText(win_id, &title_str, FALSE);
        }
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    case MSG_APP_CANCEL:
        MMICC_ClearStoredDtmf();
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SendDtmf( uint32 msg_id)
{
#if !defined(MMI_GUI_STYLE_TYPICAL)&&defined(TOUCH_PANEL_SUPPORT)
    BOOLEAN is_cc_win_focus = FALSE;//MMICC_STATUS_WIN_ID,MMICC_ANIMATION_WIN_ID
    CC_RESULT_E is_dtmf_send_ok = CC_RESULT_ERROR;
#else
    BOOLEAN is_process_win_focus = FALSE;
    BOOLEAN is_dialWin_focus = FALSE;
#endif

#if !defined(MMI_GUI_STYLE_TYPICAL)&&defined(TOUCH_PANEL_SUPPORT)
    //dtmf should send if cc window was on focus.
    is_cc_win_focus = ((MMICC_STATUS_WIN_ID == MMK_GetFocusWinId()
                                    ||MMICC_ANIMATION_WIN_ID == MMK_GetFocusWinId())
                                    && MMIAPICC_IsInState(CC_IN_CALL_STATE));

    if (is_cc_win_focus)
    {
        //if sending of dtmf is permitted, we send it. dtmf panel should open if this op is success. 
        if ((CC_INSTANCE.cc_display_type == CC_DISPLAY_HOT_BUTTONS
            ||CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL
#ifdef MET_MEX_SUPPORT
            || MMIMEX_IsOutCalling()
#endif
            )
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
            && !CC_GetHotButtonGrayState(CC_BUTTON_DIALPANEL)
#endif            
            )
        {           
            is_dtmf_send_ok = MMICC_SendSingleDtmf(msg_id);
            if (is_dtmf_send_ok == CC_RESULT_SUCCESS && CC_INSTANCE.cc_display_type == CC_DISPLAY_HOT_BUTTONS)
            {
                CC_ShowDtmfPanel(MMK_GetFocusWinId());
            }
        }
    }

#else
    is_process_win_focus = (MMICC_PROCESSING_WIN_ID == MMK_GetFocusWinId());
    is_dialWin_focus = (MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID)
                         && MMIAPICC_IsInState(CC_IN_CALL_STATE));
    
    if(is_process_win_focus || is_dialWin_focus
#ifdef MET_MEX_SUPPORT
        || MMIMEX_IsOutCalling()
#endif	
        ) 
    {        
        //send the dtmf
        MMICC_SendSingleDtmf(msg_id);
    }
#endif

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    {
        MMI_WIN_ID_T win_id = MMK_GetFocusChildWinId();
        win_id = MMK_GetWinId(win_id);

        if (MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID)
            || (MMIIDLE_DIAL_WIN_ID == win_id)
#if !defined(MMI_GUI_STYLE_TYPICAL)
#if defined(TOUCH_PANEL_SUPPORT)
            || ((MMICC_ANIMATION_WIN_ID == MMK_GetFocusWinId()
                    || MMICC_STATUS_WIN_ID == MMK_GetFocusWinId())
                && PNULL != MMICC_GetAppletInstance()
                && CC_DISPLAY_DTMF_PANEL == CC_INSTANCE.cc_display_type)
#endif
#endif
                )
        {
#if !defined(MMI_GUI_STYLE_TYPICAL)
#if defined(TOUCH_PANEL_SUPPORT)
            BOOLEAN is_dtmf_input = ((MMICC_ANIMATION_WIN_ID == MMK_GetFocusWinId()
                                                            || MMICC_STATUS_WIN_ID == MMK_GetFocusWinId())
                                                        && PNULL != MMICC_GetAppletInstance()
                                                        && CC_DISPLAY_DTMF_PANEL == CC_INSTANCE.cc_display_type);
#else
            BOOLEAN is_dtmf_input = MMIAPICC_IsInState(CC_IN_CALL_STATE);
#endif
#else
            BOOLEAN is_dtmf_input = MMIAPICC_IsInState(CC_IN_CALL_STATE);
#endif
            if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
                    || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION))
            {
                msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
            }

            MMIAPIIDLE_PlayDialTone(msg_id, is_dtmf_input);
        }
    }
#endif
}


/*****************************************************************************/
//  Description :to open the input win in calling
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenDialingWin(
                             MMI_MESSAGE_ID_E msg_id
                             )
{
    BOOLEAN is_dialkey =FALSE;
//     MMI_WIN_ID_T win_id = MMIIDLE_DIAL_WIN_ID;
    
    if ((msg_id >= MSG_APP_1) && ( msg_id <= MSG_APP_HASH))
    {
        is_dialkey = TRUE;
    }
    MMIAPIIDLE_OpenDialWin(msg_id, is_dialkey);
}

/*****************************************************************************/
// 	Description : s_is_dtmf_switch
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_SetDtmfSwitch ( BOOLEAN is_enable )
{
    CHECK_CC_APPLET_NOT_NULL
    CC_INSTANCE.is_dtmf_switch = is_enable;
}

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
// 	Description : 通话中切换到网页前，询问用户是否要切换声音到扬声器
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchToHandfreeQueryWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMICC_AUDIO_DEV_E audio_device = MMICC_GetCurrentDev();
            if ((MMICC_AUDIO_DEV_BT != audio_device)
                &&(MMICC_AUDIO_DEV_EP != audio_device)
                &&(MMICC_AUDIO_DEV_HF != audio_device))
            {
                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_HF);
            }
            SetBrowserActive();            
        }

        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        SetBrowserActive();  
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

#endif

/*****************************************************************************/
// 	Description : 通过开始后提示发送dtmf
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void promptSendDtmfAfterCallSetup( void )
{
    uint32          alert_time = 0;
    MMI_WIN_ID_T    alert_win_id = MMICC_SEND_DTMF_WIN_ID;
    MMI_WIN_ID_T    dtmf_query_win_id = MMICC_SEND_DTMF_W_WIN_ID;
    MMI_STRING_T    alert_str = {0};
    wchar           temp_alert_wchar[CC_MAX_TELE_NUM_LEN + 1] = {0};
    MMI_STRING_T    promt_str = {0};
    CC_DTMF_STATE_T dtmf_info = *MMICC_GetStoredDtmfInfo();

    alert_str.wstr_ptr = temp_alert_wchar;
    alert_str.wstr_len = dtmf_info.dtmf_len;

    alert_str.wstr_len = dtmf_info.dtmf_len - 1;
    MMI_STRNTOWSTR(alert_str.wstr_ptr, alert_str.wstr_len, (uint8 *)&(dtmf_info.dtmf_str[1]), alert_str.wstr_len, alert_str.wstr_len);
    
    if (('W' == dtmf_info.dtmf_str[0] ||  'w' == dtmf_info.dtmf_str[0])&& dtmf_info.dtmf_len != 1)
    {
        MMI_GetLabelTextByLang(TXT_CC_SEND_DTMF, &promt_str);
        if (MMK_IsOpenWin(dtmf_query_win_id))
        {
            MMK_CloseWin(dtmf_query_win_id);
        }
        MMIPUB_OpenAlertWinByTextPtrEx(CC_APPLET_HANDLE, &alert_time, &promt_str,&alert_str,IMAGE_PUBWIN_QUERY, &dtmf_query_win_id, PNULL, MMIPUB_SOFTKEY_OKCANCEL, MMICC_HandleInsertDTMFWinMsg,0);
    }
    else
    {  
        MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
        if (MMK_IsOpenWin(alert_win_id))
        {
            MMK_CloseWin(alert_win_id);
        }
        MMIPUB_OpenAlertWinByTextPtrEx(CC_APPLET_HANDLE, &alert_time, &promt_str, &alert_str,IMAGE_PUBWIN_WAIT, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, MMICC_HandleSendingDTMFWinMsg,0);
    }    
}

/*****************************************************************************/
//  Description :to open mo or mt win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void OpenCallingWin(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, MMI_ANIM_ID_T anim_id, PROCESSMSG_FUNC handle_func)
{
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    GUISOFTKEY_INIT_DATA_T softkey_init = {0};
    //MMI_CONTROL_CREATE_T anim_create = {0};
#ifdef MMI_GUI_STYLE_TYPICAL
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
#endif    
    //GUIANIM_INIT_DATA_T anim_init = {0};

#ifndef MMI_GUI_STYLE_TYPICAL
    //create window
#if !defined(DPHONE_SUPPORT)
    CHECK_CC_APPLET_NOT_NULL
#endif
    
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    CreateStatusBarCtrl(win_id, IMAGE_NULL);
#endif
    PdaDisplaySingleCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_CURRENT
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    
    ,FALSE,
    MMI_CALL_INANDOUT_STATE_TEXT_COLOR
#endif     
    );
#else

#if defined(DPHONE_SUPPORT)//@ alex added in 2010.11.29 --begin-->
    MMI_CTRL_ID_T text_form_id = MMICC_CALLING_TEXT_FORM_CTRL_ID;
    MMI_CONTROL_CREATE_T text_form_create = {0};
    GUIFORM_INIT_DATA_T text_form_init = {0};
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    GUI_BOTH_RECT_T anim_both_rect = {0};

    //create window
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);
    {
        uint16 ver_space = 0;
        uint16 hor_space = 0;
        GUIFORM_SetSpace(text_form_id, &ver_space, &hor_space);
    }
    //create text form
    text_form_init.both_rect.v_rect.left = 1;//client_rect.v_rect.left;
    text_form_init.both_rect.v_rect.top = 1;//MMICC_CALLING_TEXT_FORM_TOP -MMI_STATUSBAR_HEIGHT;
    text_form_init.both_rect.v_rect.right = 126;//client_rect.v_rect.right;
    text_form_init.both_rect.v_rect.bottom = 64-14;//client_rect.v_rect.bottom;//MMICC_CALLING_TEXT_FORM_BOTTOM;// -MMI_STATUSBAR_HEIGHT;
    
    text_form_init.both_rect.h_rect.left = 1;//client_rect.h_rect.left;
    text_form_init.both_rect.h_rect.top = 1;//MMICC_CALLING_TEXT_FORM_TOP_H -MMI_STATUSBAR_HEIGHT;
    text_form_init.both_rect.h_rect.right = 126;//client_rect.h_rect.right;
    text_form_init.both_rect.h_rect.bottom = 64-14;//client_rect.h_rect.bottom;//MMICC_CALLING_TEXT_FORM_BOTTOM_H ;//-MMI_STATUSBAR_HEIGHT;
    text_form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    text_form_create.ctrl_id           = text_form_id;
    text_form_create.guid              = SPRD_GUI_FORM_ID;
    text_form_create.init_data_ptr     = &text_form_init;
    text_form_create.parent_win_handle = win_id;
    MMK_CreateControl(&text_form_create);
    
    //not slide
    GUIFORM_IsSlide(text_form_id,FALSE);

    //create text label
    text_label_init.align = GUILABEL_ALIGN_MIDDLE;
    text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = FALSE;
    text_form_child_ctrl.init_data_ptr = &text_label_init;
	//@ alex made a judge to divide that it whether has relative name or no relative name in PB, and depend on its judge to display contents in form
	 text_form_child_ctrl.child_handle = MMICC_CALLING_STATE_LABEL_CTRL_ID + 0;
        GUIFORM_CreatDynaChildCtrl(win_id, text_form_id,  &text_form_child_ctrl);
        if(prompt_str_array[CC_DISPALY_INFO_STATE].wstr_len > 0 && (PNULL != prompt_str_array[CC_DISPALY_INFO_STATE].wstr_ptr))
        {
            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[CC_DISPALY_INFO_STATE], FALSE);
	     GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
        }
	if(prompt_str_array[CC_DISPALY_INFO_NAME].wstr_len > 0){
		 text_form_child_ctrl.child_handle = MMICC_CALLING_STATE_LABEL_CTRL_ID + 1;
	        GUIFORM_CreatDynaChildCtrl(win_id, text_form_id,  &text_form_child_ctrl);
	        if(prompt_str_array[CC_DISPALY_INFO_NAME].wstr_len > 0 && (PNULL != prompt_str_array[CC_DISPALY_INFO_NAME].wstr_ptr))
	        {
	            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[CC_DISPALY_INFO_NAME], FALSE);
		     GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_TEXT_FONT, CC_TEXT_COLOR);
	        }
	}else{
		 text_form_child_ctrl.child_handle = MMICC_CALLING_STATE_LABEL_CTRL_ID + 2;
	        GUIFORM_CreatDynaChildCtrl(win_id, text_form_id,  &text_form_child_ctrl);
	        if(prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len > 0 && (PNULL != prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr))
	        {
	            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[CC_DISPALY_INFO_NUMBER], FALSE);
		     GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_TEXT_FONT, CC_TEXT_COLOR);
        }	
	}
	 text_form_child_ctrl.child_handle = MMICC_CALLING_STATE_LABEL_CTRL_ID + 3;
        GUIFORM_CreatDynaChildCtrl(win_id, text_form_id,  &text_form_child_ctrl);
        if(prompt_str_array[CC_DISPALY_INFO_LOCATION].wstr_len > 0 && (PNULL != prompt_str_array[CC_DISPALY_INFO_LOCATION].wstr_ptr))
        {
            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[CC_DISPALY_INFO_LOCATION], FALSE);
	     GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_TEXT_FONT, CC_TEXT_COLOR);
        } 

    //create photo anim
    anim_both_rect.v_rect.left = MMICC_CALLING_PHOTO_ANIM_LEFT;
    anim_both_rect.v_rect.right = MMICC_CALLING_PHOTO_ANIM_RIGHT;
    anim_both_rect.v_rect.top = MMICC_CALLING_PHOTO_ANIM_TOP;
    anim_both_rect.v_rect.bottom = MMICC_CALLING_PHOTO_ANIM_BOTTOM;

    anim_both_rect.h_rect.left = MMICC_CALLING_PHOTO_ANIM_LEFT_H;
    anim_both_rect.h_rect.right = MMICC_CALLING_PHOTO_ANIM_RIGHT_H;
    anim_both_rect.h_rect.top = MMICC_CALLING_PHOTO_ANIM_TOP_H;
    anim_both_rect.h_rect.bottom = MMICC_CALLING_PHOTO_ANIM_BOTTOM_H;

    //set photo anim
    CreatePhotoAnim(win_id, MMICC_PHOTO_ANIM_CTRL_ID, &anim_both_rect, PHOTO_TYPE_CURRENT);
#elif defined(MAINLCD_SIZE_320X480)
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    
    GUIFORM_INIT_DATA_T info_form_init = {0};
    MMI_CTRL_ID_T info_form_id = MMICC_CONNECTED_INFO_FORM_CTRL_ID;
    MMI_CONTROL_CREATE_T info_form_create = {0};
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
    CC_DISPALY_INFO_E info_index = CC_DISPALY_INFO_STATE;
    GUIANIM_INIT_DATA_T anim_init = {0};
#if !defined(DPHONE_SUPPORT)
    CHECK_CC_APPLET_NOT_NULL
#endif

    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);

    CreateStatusBarCtrl(win_id, IMAGE_NULL);

    //create info form
    info_form_init.both_rect.v_rect.left = client_rect.v_rect.left;
    info_form_init.both_rect.v_rect.top = MMICC_CALLING_TEXT_FORM_TOP;
    info_form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    info_form_init.both_rect.v_rect.bottom = MMICC_CALLING_TEXT_FORM_BOTTOM;
    
    info_form_init.both_rect.h_rect.left = client_rect.h_rect.left;
    info_form_init.both_rect.h_rect.top = MMICC_CALLING_TEXT_FORM_TOP_H;
    info_form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    info_form_init.both_rect.h_rect.bottom = MMICC_CALLING_TEXT_FORM_BOTTOM_H;
    info_form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    info_form_create.ctrl_id = info_form_id;
    info_form_create.guid = SPRD_GUI_FORM_ID;
    info_form_create.init_data_ptr = &info_form_init;
    info_form_create.parent_win_handle = win_id;
    MMK_CreateControl(&info_form_create);

    //not slide
    GUIFORM_IsSlide(info_form_id,FALSE);
    //!!!NOT MAINLCD_SIZE_128X64 and MAINLCD_SIZE_320X480
    text_label_init.align = GUILABEL_ALIGN_MIDDLE;
    text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = FALSE;
    text_form_child_ctrl.init_data_ptr = &text_label_init;

    text_form_child_ctrl.child_handle = MMICC_CONNECTED_STATE_LABEL_CTRL_ID;
    GUIFORM_CreatDynaChildCtrl(win_id, info_form_id,  &text_form_child_ctrl);
    GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[info_index], FALSE);
    GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_NORMAL_FONT, CC_TEXT_COLOR);

    {
        MMI_HANDLE_T anim_handle = MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID;
        GUIANIM_CTRL_INFO_T control_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        //GUIANIM_DISPLAY_INFO_T display_info = {0};
        MMIPB_BCD_NUMBER_T pb_num = {0};
        GUIANIM_FILE_INFO_T file_info = {0};
        wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
        uint32 custom_photo_name_len = 0;
        uint32 photo_file_size = 0;
        GUIFORM_CHILD_WIDTH_T anim_child_width = {0};
        GUIFORM_CHILD_HEIGHT_T anim_child_height = {0};
        BOOLEAN is_want_mo_call = (MMICC_AUTO_DIAL_WIN_ID == win_id) || (MMICC_IsExistOutgoingCall());

        text_form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
        text_form_child_ctrl.is_bg = FALSE;
        text_form_child_ctrl.is_get_active = FALSE;
        text_form_child_ctrl.init_data_ptr = &anim_init;
        text_form_child_ctrl.child_handle = anim_handle;
        GUIFORM_CreatDynaChildCtrl(win_id, info_form_id,  &text_form_child_ctrl);
        anim_child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        anim_child_height.add_data = MMICC_CALLING_PHOTO_ANIM_HEIGHT;
        anim_child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        anim_child_width.add_data = MMICC_CALLING_PHOTO_ANIM_WIDTH;
        GUIFORM_SetChildHeight(info_form_id, anim_handle, &anim_child_height);
        GUIFORM_SetChildWidth(info_form_id, anim_handle, &anim_child_width);
        GUIFORM_SetChildAlign(info_form_id, anim_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);

        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = anim_handle;
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

        MMICC_GetCurrentCallNumber(&pb_num, TRUE, is_want_mo_call);

        if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
        {
            GUIANIM_SetDefaultIcon(anim_handle,IMAGE_COMMON_PIC_ICON,IMAGE_COMMON_PIC_ICON);
            file_info.full_path_wstr_ptr = custom_photo_name;
            file_info.full_path_wstr_len = custom_photo_name_len;
            display_info.bg.bg_type = GUI_BG_COLOR;
            display_info.bg.color   = MMI_BACKGROUND_COLOR;
            GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);
        }
        else
        {
            data_info.img_id = IAMGE_CALL_DEFAULT_PHOTO;
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
        }
        
    }

    text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = FALSE;
    text_form_child_ctrl.init_data_ptr = &text_label_init;

    for(info_index = CC_DISPALY_INFO_STATE; info_index <= CC_DISPALY_INFO_LOCATION; info_index++)
    {
        if(prompt_str_array[info_index].wstr_len > 0 && (PNULL != prompt_str_array[info_index].wstr_ptr))
        {
            text_form_child_ctrl.child_handle = MMICC_CONNECTED_STATE_LABEL_CTRL_ID + info_index;
            GUIFORM_CreatDynaChildCtrl(win_id, info_form_id,  &text_form_child_ctrl);
            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[info_index], FALSE);
            GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_NORMAL_FONT, CC_TEXT_COLOR);
        }
    }
    {
        uint16 ver_space = 0;
        uint16 hor_space = 0;
        GUIFORM_SetSpace(info_form_id, &ver_space, &hor_space);
    }

    //create button of calling/incoming/waiting win
    CC_CreateHotButtons(win_id);
#else
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};

    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
    int32 info_index = CC_DISPALY_INFO_STATE; 
    
    MMI_CONTROL_CREATE_T text_child_form_create = {0};
    MMI_CTRL_ID_T text_child_form_id = MMICC_CALLING_TEXT_FORM_CTRL_ID;
    GUIFORM_INIT_DATA_T text_child_form_init = {0};
    GUI_BOTH_RECT_T both_rect = {0};
#if !defined(DPHONE_SUPPORT)
    CHECK_CC_APPLET_NOT_NULL
#endif
    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);

    //create status bar
    CreateStatusBarCtrl(win_id, IMAGE_NULL);

    //create photo anim
    both_rect.v_rect.left = MMICC_CONNECTED_PHOTO_ANIM_LEFT;
    both_rect.v_rect.right = MMICC_CONNECTED_PHOTO_ANIM_RIGHT;
    both_rect.v_rect.top = MMICC_CONNECTED_PHOTO_ANIM_TOP;
    both_rect.v_rect.bottom = MMICC_CONNECTED_PHOTO_ANIM_BOTTOM;
    both_rect.h_rect.left = MMICC_CONNECTED_PHOTO_ANIM_LEFT_H;
    both_rect.h_rect.right = MMICC_CONNECTED_PHOTO_ANIM_RIGHT_H;
    both_rect.h_rect.top = MMICC_CONNECTED_PHOTO_ANIM_TOP_H;
    both_rect.h_rect.bottom = MMICC_CONNECTED_PHOTO_ANIM_BOTTOM_H;

    CreatePhotoAnim(win_id, MMICC_PHOTO_ANIM_CTRL_ID, &both_rect, PHOTO_TYPE_CURRENT);

    //create text form
    text_child_form_init.both_rect.v_rect.left = MMICC_CONNECTED_TEXT_FORM_LEFT; 
    text_child_form_init.both_rect.v_rect.top = MMICC_CONNECTED_TEXT_FORM_TOP; 
    text_child_form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    text_child_form_init.both_rect.v_rect.bottom = MMICC_CONNECTED_TEXT_FORM_BOTTOM;
    
    text_child_form_init.both_rect.h_rect.left = MMICC_CONNECTED_TEXT_FORM_LEFT_H; 
    text_child_form_init.both_rect.h_rect.top = MMICC_CONNECTED_TEXT_FORM_TOP_H; 
    text_child_form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    text_child_form_init.both_rect.h_rect.bottom = MMICC_CONNECTED_TEXT_FORM_BOTTOM_H;
    text_child_form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    text_child_form_create.ctrl_id = text_child_form_id;
    text_child_form_create.guid = SPRD_GUI_FORM_ID;
    text_child_form_create.init_data_ptr = &text_child_form_init;
    text_child_form_create.parent_win_handle = win_id;
    MMK_CreateControl(&text_child_form_create);

    {
        uint16 ver_space = 0;
        uint16 hor_space = 0;
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
        GUI_BG_T bg_info = {0};
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_COMMON_BG;

        GUIFORM_SetBg(text_child_form_id, &bg_info);
        GUIFORM_PermitChildFont(text_child_form_id,FALSE);
#endif
        GUIFORM_SetSpace(text_child_form_id, &ver_space, &hor_space);
    }


    //create text label
    text_label_init.align = GUILABEL_ALIGN_LEFT;
    text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = FALSE;
    text_form_child_ctrl.init_data_ptr = &text_label_init;

    for(info_index = CC_DISPALY_INFO_STATE; info_index <= CC_DISPALY_INFO_LOCATION; info_index++)
    {
        if(prompt_str_array[info_index].wstr_len > 0 && (PNULL != prompt_str_array[info_index].wstr_ptr))
        {
            if (CC_DISPALY_INFO_LOCATION == info_index)
            {
                text_form_child_ctrl.child_handle = MMICC_CALLING_STATE_LABEL_CTRL_ID + info_index - 1;
            }
            else
            {
                text_form_child_ctrl.child_handle = MMICC_CALLING_STATE_LABEL_CTRL_ID + info_index;
            }
            GUIFORM_CreatDynaChildCtrl(win_id, text_child_form_id,  &text_form_child_ctrl);
            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[info_index], FALSE);
#ifdef MAINLCD_SIZE_176X220
            GUILABEL_SetFont(text_form_child_ctrl.child_handle, SONG_FONT_14, CC_TEXT_COLOR);
#else
            GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, CC_TEXT_COLOR);
#endif
        }
    }
    
    //create button of calling/incoming/waiting win
    CC_CreateHotButtons(win_id);
#endif

#endif
    //create softkey control
    softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
    softkey_create.init_data_ptr     = &softkey_init;
    softkey_create.parent_win_handle = win_id;
 
    MMK_CreateControl(&softkey_create);
    MMK_SetWinSoftkeyCtrlId(win_id,softkey_create.ctrl_id);

}

/*****************************************************************************/
//  Description :to open connected win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void OpenConnectedWin(MMI_STRING_T* prompt_str_array, PROCESSMSG_FUNC handle_func
#ifdef MMI_CALLCONTROL_MINI_SUPPORT                                             
                                             ,BOOLEAN is_exist_time_lable,
                                             uint16 state_color
#endif
)
{
    MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    GUISOFTKEY_INIT_DATA_T softkey_init = {0};

    CHECK_CC_APPLET_NOT_NULL
    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);

#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    //create status bar
    CreateStatusBarCtrl(win_id, IMAGE_NULL);  
#endif        
    //set call info
    PdaDisplaySingleCall(win_id, prompt_str_array
#ifdef MMI_CALLCONTROL_MINI_SUPPORT   
    ,is_exist_time_lable,state_color
#endif
    );
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    //create button of connected win
    CC_CreateHotButtonsWithoutForm(win_id);
#endif    
#else
    
    CHECK_CC_APPLET_NOT_NULL
    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);
#ifndef MAINLCD_SIZE_128X64 
    //create status bar
    CreateStatusBarCtrl(win_id, IMAGE_NULL);  
    //set call info
    if (MMICC_IsExistActiveAndHoldCalls())
    {
        DisplayCallInfoForMulti();
    }
    else
#endif        
    {
        DisplayCallInfoForSingle(prompt_str_array, FALSE);
    }
#ifndef MAINLCD_SIZE_128X64 
    //create button of connected win
    CC_CreateHotButtons(win_id);
#endif // MAINLCD_SIZE_128X64 

#endif
    //create softkey control
    softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
    softkey_create.init_data_ptr     = &softkey_init;
    softkey_create.parent_win_handle = win_id;
 
    MMK_CreateControl(&softkey_create);
    MMK_SetWinSoftkeyCtrlId(win_id,softkey_create.ctrl_id);

}

/*****************************************************************************/
//  Description :to open the mo call window
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenMoCallWin(void)
{
    MMI_STRING_T	prompt_str_array[CC_DISPALY_INFO_MAX] = {0};
    uint8			tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			tel_num_len = 0;
    uint16			city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH] = {0};
    MMI_STRING_T    state_str = {0};
    BOOLEAN         is_emergency_call = FALSE;

    wchar temp_tel_num[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
    
        
    //判断cc window是否存在
#ifndef MMI_GUI_STYLE_TYPICAL 
    MMK_CloseWin( MMICC_STATUS_WIN_ID );
#else
    MMK_CloseWin( MMICC_ANIMATION_WIN_ID );
#endif

    if(PNULL == MMICC_GetAppletInstance())
    {
        MMICC_StartCCApplet();
    }
    
    SCI_MEMSET(&prompt_str_array[CC_DISPALY_INFO_STATE], 0, sizeof(prompt_str_array));

    MMICC_GetWantCallNumStr(tel_num, &tel_num_len);
    
    //bug 186648 modify is_only_for_display(the last parameter of MMIAPICC_IsEmergencyNum) true to false,is_only_for_display is setted true is to some requirement,
    //but in different countries in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num(no sim emergency) ,
    //it is not normal display emergency call in window
#ifdef BT_DIALER_SUPPORT
    if(!MMICC_GetBtCallFlag())
#endif
{
    is_emergency_call = MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, MMICC_GetWantCallDualSys(), FALSE);
}
    if(is_emergency_call)
    {
        MMI_GetLabelTextByLang(
            TXT_KL_PSW_EMERGENCY_CALL,
            &state_str
            );
    }
    else
    {
        MMI_GetLabelTextByLang(
            TXT_CALLING,
            &state_str
            );
    }

#ifdef BT_DIALER_SUPPORT    
     MMIAPICL_DeleteAllPbapSyncData();
#endif

    if(is_emergency_call)
    {
        MMI_GetLabelTextByLang(
            TXT_KL_PSW_EMERGENCY_CALL,
            &state_str
            );

#ifndef MMI_GUI_STYLE_TYPICAL
        MMICC_GetWantCallSpecifiedStr(&prompt_str_array[CC_DISPALY_INFO_STATE]);
        prompt_str_array[CC_DISPALY_INFO_NAME].wstr_len = state_str.wstr_len;
        prompt_str_array[CC_DISPALY_INFO_NAME].wstr_ptr = state_str.wstr_ptr;
#endif        
    }
    else
    {
        if(!MMICC_GetWantCallSpecifiedStr(&prompt_str_array[CC_DISPALY_INFO_STATE]))
        {
            CC_CallStateDisplaying( state_str.wstr_len, state_str.wstr_ptr);
            prompt_str_array[CC_DISPALY_INFO_STATE].wstr_len = CC_INSTANCE.sim_status.wstr_len;
            prompt_str_array[CC_DISPALY_INFO_STATE].wstr_ptr = CC_INSTANCE.sim_status.wstr_ptr;
        }
    
        if(0 < tel_num_len)
        {

            if(!MMICC_GetWantCallNameStr(&prompt_str_array[CC_DISPALY_INFO_NAME]))
            {
                prompt_str_array[CC_DISPALY_INFO_NAME].wstr_len = 0;
                prompt_str_array[CC_DISPALY_INFO_NAME].wstr_ptr = PNULL;
            }

            prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr = temp_tel_num;
            prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len = tel_num_len;
            //prompt_str_array[2].wstr_ptr = tel_num;
            MMI_STRNTOWSTR(prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr,
                prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len,
                (uint8 *)tel_num,
                tel_num_len,
                tel_num_len);
        }
        else if(MMICC_IsExistOutgoingCall())
        {    
            if(CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
            {

                if(MMICC_GetCallNameStrByIndex(&prompt_str_array[CC_DISPALY_INFO_NAME], MMICC_GetCurrentCallIndex()))
                {
#ifndef SUBLCD_SIZE_NONE                    
                    MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_str_array[CC_DISPALY_INFO_NAME]);//@fen.xie MS00172355
#endif
                }
                else
                {
                    prompt_str_array[CC_DISPALY_INFO_NAME].wstr_len = 0;
                    prompt_str_array[CC_DISPALY_INFO_NAME].wstr_ptr = PNULL;
#ifndef SUBLCD_SIZE_NONE                     
				    //刷新小屏，显示去电号码
                    MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_str_array[CC_DISPALY_INFO_NUMBER]);//@fen.xie MS00172355
#endif 
                } 
               
                MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, MMICC_GetCurrentCallIndex());
                
#ifdef BT_DIALER_SUPPORT  
                if(MMICC_GetBtCallFlag() && 0 == tel_num_len)
                {
                    //clcc is not support
                    MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &prompt_str_array[CC_DISPALY_INFO_NUMBER]);
                }
                else
#endif
{
                
                prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr = temp_tel_num;
                prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len = tel_num_len;
        
                //prompt_str_array[2].wstr_ptr = tel_num;
                MMI_STRNTOWSTR(prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr,
                    prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len,
                    (uint8 *)tel_num,
                    tel_num_len,
                    tel_num_len);
}				
            }
        }

        // find the gui shu di information

        CC_FindLocationInfor(&(prompt_str_array[CC_DISPALY_INFO_LOCATION]), city_ucs2_str, tel_num, tel_num_len );

    }
#ifndef MMI_GUI_STYLE_TYPICAL
        //判断cc window是否存在
    if( !MMK_IsOpenWin( MMICC_STATUS_WIN_ID ) )
    {
        OpenConnectedWin(prompt_str_array, CC_HandleCcWinMsg
#ifdef MMI_CALLCONTROL_MINI_SUPPORT           
            ,FALSE,
            MMI_CALL_INANDOUT_STATE_TEXT_COLOR         
#endif            
            );

 #ifdef MMI_CALLCONTROL_MINI_SUPPORT
    DisplayCallAnimPhotoForCommon(MMICC_STATUS_WIN_ID,IMAGE_CC_CALLING_PHOTO_ANIM);
 #endif
 
    }
    else
    { 
        if (0 < MMICC_GetCallNum())
        {
            PdaDisplayMultiCall(MMICC_STATUS_WIN_ID, prompt_str_array);
#ifndef MMI_CALLCONTROL_MINI_SUPPORT             
            CC_UpdateHotButtonState();
#endif
            if (!MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
            {
                MMK_WinGrabFocus(MMICC_STATUS_WIN_ID);
            }
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT            
            if (MMK_IsOpenWin(MMIKL_PSW_WIN_ID))
            {
                MMK_CloseWin(MMIKL_PSW_WIN_ID);
            }
#endif            
        }
        else         
        {
            SCI_ASSERT(0);/* assert verified*/
        }
    }
#else
    OpenCallingWin(MMICC_ANIMATION_WIN_ID, prompt_str_array, IMAGE_MO_CALL_ANIMA, CC_HandleCcAnimWinMsg);
#endif
    
    CC_FreeSimStatusSpace();
}

/*****************************************************************************/
// 	Description : give a hint to user that there is a missed call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_ShowMCHint(void)
{
    //判断是否有MISSED CALL存在
    if(( 0== MMICC_GetCallNum())  && ( MMIAPICC_GetMissedCallNumber() > 0) 
        && MMICC_GetIsNewMissedCall())
    {
        MMICC_SetIsNewMissedCall(FALSE);
        //CC_SetMissedcallWinString(&g_ccapp_data.missedcalwin_data, (uint8*)"Missed Call");
#ifdef MMIWIDGET_SUPPORT
	if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
	{
            MMK_CloseWin( MMICC_MISSCALL_WIN_ID );//MS00232341
	    MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_CALLLOG_ID, FALSE, TRUE);
	}
	else
#endif		
        {
            CC_OpenMissedCallWin();
        }
    }
}

/*****************************************************************************/
//  Description :to open mo or mt win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetCallingWinTextFormParam(void)
{
    uint16 ver_space = MMICC_CALLING_TEXT_FORM_SPACE;
    uint16 hor_space = MMICC_CALLING_TEXT_FORM_SPACE;
    //set text label font
#ifdef MAINLCD_SIZE_320X480
    GUILABEL_SetFont(MMICC_CALLING_STATE_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
    GUILABEL_SetFont(MMICC_CALLING_NAME_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
    GUILABEL_SetFont(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
    GUILABEL_SetFont(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
#elif defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
    if(MMITHEME_IsMainScreenLandscape())
    {
        GUILABEL_SetFont(MMICC_CALLING_STATE_LABEL_CTRL_ID, SONG_FONT_14, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NAME_LABEL_CTRL_ID, SONG_FONT_12, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, SONG_FONT_12, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, SONG_FONT_12, CC_TEXT_COLOR);
    }
    else
    {
        GUILABEL_SetFont(MMICC_CALLING_STATE_LABEL_CTRL_ID, SONG_FONT_16, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NAME_LABEL_CTRL_ID, SONG_FONT_14, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, SONG_FONT_12, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, SONG_FONT_14, CC_TEXT_COLOR);
    }
#else
    if(MMITHEME_IsMainScreenLandscape())
    {
        GUILABEL_SetFont(MMICC_CALLING_STATE_LABEL_CTRL_ID, MMI_DEFAULT_NORMAL_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NAME_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, CC_TEXT_COLOR);
    }
    else
    {
    #if defined(MAINLCD_SIZE_128X64)
        GUILABEL_SetFont(MMICC_CALLING_STATE_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NAME_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, /*MMI_DEFAULT_BIG_FONT*/12, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
    #else        
        GUILABEL_SetFont(MMICC_CALLING_STATE_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NAME_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
        GUILABEL_SetFont(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, MMI_DEFAULT_BIG_FONT, CC_TEXT_COLOR);
    #endif //MAINLCD_SIZE_128X64        
    }
#endif

    //set text form space
    if(MMITHEME_IsMainScreenLandscape())
    {
        ver_space = MMICC_CALLING_TEXT_FORM_SPACE_H;
        hor_space = MMICC_CALLING_TEXT_FORM_SPACE_H;
    }
    GUIFORM_SetSpace(MMICC_CALLING_TEXT_FORM_CTRL_ID, &ver_space, &hor_space);
} 

#ifdef MMI_CLEAR_CODE_SUPPORT
LOCAL MMI_STRING_T GetClearCodeString(MN_CALL_DISCONNECT_CALL_CAUSE_E cause){
	MMI_STRING_T res_string = {0};
    uint8 default_string[CC_CALL_CLEAR_CODE_STR_MAX_LEN] = {0};
	
	switch(cause)
	{
		case MN_CAUSE_UNASSIGNED_NO://1
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_01, &res_string);
			break;
		case MN_CAUSE_NUMBER_CHANGED://22
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_22, &res_string);
			break;
		case MN_CAUSE_INVALID_NO_FORMAT://28
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_28, &res_string);
			break;
		    
		case MN_CAUSE_NO_ROUTE_TO_DEST://3
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_03, &res_string);
			break;		
		case MN_CAUSE_OPER_DETERM_BARRING://8
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_08, &res_string);
			break;
		case MN_CAUSE_CALL_REJECTED://21
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_21, &res_string);
			break;			
		case MN_CAUSE_DEST_OUT_OF_ORDER://27
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_27, &res_string);
			break;
		    
		case MN_CAUSE_NO_USER_RESPONDING://18
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_18, &res_string);
			break;		
		case MN_CAUSE_ALERTING_NO_ANSWER://19
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_19, &res_string);
			break;	
		    
		case MN_CAUSE_NORMAL_CLEARING://16
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_16, &res_string);
			break;	
		    
		case MN_CAUSE_USER_BUSY://17
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_17, &res_string);
			break;	

		default://other case
			MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_CLEARCODE_DEFAULT,  &res_string);
            MMI_WSTRNTOSTR((uint8*)default_string, CC_CALL_CLEAR_CODE_STR_MAX_LEN,
                res_string.wstr_ptr, res_string.wstr_len, res_string.wstr_len);
			sprintf((char*)default_string,"%s -%02d-",default_string, cause);
            res_string.wstr_len = strlen((char*)default_string);
            MMI_STRNTOWSTR(res_string.wstr_ptr,
                CC_CALL_CLEAR_CODE_STR_MAX_LEN,
                default_string,
                CC_CALL_CLEAR_CODE_STR_MAX_LEN,
                res_string.wstr_len);  
			break;
	}
	
	return res_string;
}

#endif

#ifndef MMI_GUI_STYLE_TYPICAL 
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 创建通话界中面的Call button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructDialButton(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T        dial_button_ctrl = {0};        
    GUIBUTTON_INIT_DATA_T    dial_button_init = {0};
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_BG_T button_bg = {0};
    uint16		img_width =0;
    uint16		img_height =0;
    GUI_FONT_ALL_T     font = {0};

    if(0 != MMK_GetCtrlHandleByWin(win_id, MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID))
    {
        return;
    }
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);
    
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMG_CC_BUTTON_ANSWER_CALL,win_id);

    dial_button_init.both_rect.v_rect.left   = MMICC_CC_END_BUTTON_MARGIN;
    dial_button_init.both_rect.v_rect.right  = dial_button_init.both_rect.v_rect.left + img_width - 1;
    dial_button_init.both_rect.v_rect.top    = client_rect.v_rect.bottom - MMI_DIAL_OPTION_BUTTON_FORM_HEIGHT;
    dial_button_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom - 10;

    dial_button_init.both_rect.h_rect.left   = CC_HB_ONE_ONE_LEFT_H;
    dial_button_init.both_rect.h_rect.right  = (CC_HB_ONE_THREE_RIGHT_H+CC_HB_ONE_ONE_LEFT_H)/2 -4;
    dial_button_init.both_rect.h_rect.top    = client_rect.h_rect.bottom - MMI_DIAL_OPTION_BUTTON_FORM_HEIGHT;
    dial_button_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom - 10;


    button_bg.bg_type = GUI_BG_COLOR;
    button_bg.color = GUI_RGB2RGB565(17,171,21);
    
    dial_button_ctrl.ctrl_id           = MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID;
    dial_button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    dial_button_ctrl.init_data_ptr     = &dial_button_init;
    dial_button_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&dial_button_ctrl);
    
    button_bg.bg_type = GUI_BG_COLOR;
    button_bg.color = GUI_RGB2RGB565(17,171,21);
    button_bg.shape = GUI_SHAPE_RECT;
    GUIBUTTON_SetBg(MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, &button_bg);

//    button_bg.img_id = IMG_CC_BUTTON_ANSWER_CALL_P;
//    GUIBUTTON_SetPressedBg(MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, &button_bg);
//    GUIBUTTON_SetRunSheen(MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, FALSE);
    
    GUIBUTTON_SetTextId(MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, TXT_CALL);

    font.color = CC_TEXT_COLOR;
    font.font = MMI_DEFAULT_NORMAL_FONT;
    GUIBUTTON_SetFont(MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, &font);
    GUIBUTTON_SetTextAlign(MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID, ALIGN_HVMIDDLE);
}

/*****************************************************************************/
//  Description : 创建通话界中面的End button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructEndButton(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T        end_button_ctrl = {0};        
    GUIBUTTON_INIT_DATA_T    end_button_init = {0};
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_BG_T button_bg = {0};
    uint16		img_width =0;
    uint16		img_height =0;
    GUI_FONT_ALL_T     font;

    if(0 != MMK_GetCtrlHandleByWin(win_id, MMICC_BUTTON_PDA_END_CALL_CTRL_ID))
    {
        return;
    }
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMG_CC_BUTTON_END_CALL,win_id);

    end_button_init.both_rect.v_rect.left   = client_rect.v_rect.right/2 + 4;
    end_button_init.both_rect.v_rect.right  = end_button_init.both_rect.v_rect.left + img_width - 1;
    end_button_init.both_rect.v_rect.top    = client_rect.v_rect.bottom - MMI_DIAL_OPTION_BUTTON_FORM_HEIGHT ;
    end_button_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom - 10;

    end_button_init.both_rect.h_rect.left   = (CC_HB_ONE_THREE_RIGHT_H+CC_HB_ONE_ONE_LEFT_H)/2+4;
    end_button_init.both_rect.h_rect.right  = CC_HB_ONE_THREE_RIGHT_H-MMICC_CC_END_BUTTON_MARGIN/2;
    end_button_init.both_rect.h_rect.top    = client_rect.h_rect.bottom - MMI_DIAL_OPTION_BUTTON_FORM_HEIGHT;
    end_button_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom - 10;

    end_button_init.bg.bg_type = GUI_BG_COLOR;
    end_button_init.bg.color = GUI_RGB2RGB565(17,171,21);
    
    end_button_ctrl.ctrl_id           = MMICC_BUTTON_PDA_END_CALL_CTRL_ID;
    end_button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    end_button_ctrl.init_data_ptr     = &end_button_init;
    end_button_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&end_button_ctrl);
    
    button_bg.bg_type = GUI_BG_COLOR;
    button_bg.color = GUI_RGB2RGB565(255,78,0); /*lint !e572*/
    button_bg.shape = GUI_SHAPE_RECT;
    
    GUIBUTTON_SetBg(MMICC_BUTTON_PDA_END_CALL_CTRL_ID, &button_bg);
    
    GUIBUTTON_SetTextId(MMICC_BUTTON_PDA_END_CALL_CTRL_ID, TXT_CC_HANG_OFF);

    font.color = CC_TEXT_COLOR;
    font.font = MMI_DEFAULT_NORMAL_FONT;
    GUIBUTTON_SetFont(MMICC_BUTTON_PDA_END_CALL_CTRL_ID, &font);

    GUIBUTTON_SetTextAlign(MMICC_BUTTON_PDA_END_CALL_CTRL_ID, ALIGN_HVMIDDLE);
}

#endif //MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 创建通话界中面的option button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructOptionButton(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T        hold_button_ctrl = {0};        
    GUIBUTTON_INIT_DATA_T    hold_button_init = {0};
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_BG_T button_bg = {0};
    //uint16		img_width =0;
    //uint16		img_height =0;
    //GUILABEL_INIT_DATA_T label_init = {0};    
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    if(0 != MMK_GetCtrlHandleByWin(win_id, MMICC_BUTTON_PDA_OPTION_CTRL_ID))
    {
        return;
    }

    client_rect = MMITHEME_GetWinClientBothRect(win_id);
    
    //GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_COMMON_BTN_OPTION,win_id);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if (MMITHEME_IsMainScreenLandscape())
    {
        lcd_width = lcd_height;
    }
    hold_button_init.both_rect.v_rect.right   = CC_OPTION_BUTTON_RIGHT;
    hold_button_init.both_rect.v_rect.left  = CC_OPTION_BUTTON_LEFT;
    hold_button_init.both_rect.v_rect.top    = CC_OPTION_BUTTON_TOP + client_rect.v_rect.top;
    hold_button_init.both_rect.v_rect.bottom = CC_OPTION_BUTTON_BOTTOM + client_rect.v_rect.top;

    hold_button_init.both_rect.h_rect.right   = CC_OPTION_BUTTON_RIGHT_H;
    hold_button_init.both_rect.h_rect.left  = CC_OPTION_BUTTON_LEFT_H;
    hold_button_init.both_rect.h_rect.top    = CC_OPTION_BUTTON_TOP_H + client_rect.h_rect.top;
    hold_button_init.both_rect.h_rect.bottom = CC_OPTION_BUTTON_BOTTOM_H + client_rect.h_rect.top;

    hold_button_init.bg.bg_type = GUI_BG_IMG;
    hold_button_init.bg.img_id  = IMAGE_CC_BTN_OPTION;
    
    hold_button_ctrl.ctrl_id           = MMICC_BUTTON_PDA_OPTION_CTRL_ID;
    hold_button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    hold_button_ctrl.init_data_ptr     = &hold_button_init;
    hold_button_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&hold_button_ctrl);
    
    button_bg.bg_type = GUI_BG_IMG;
    
    button_bg.img_id = IMAGE_CC_BTN_OPTION;
    GUIBUTTON_SetBg(MMICC_BUTTON_PDA_OPTION_CTRL_ID, &button_bg);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_OPTION_CTRL_ID, FALSE, &button_bg, FALSE);
    button_bg.img_id = IMAGE_CC_BTN_OPTION_FOCUS;
    GUIBUTTON_SetPressedBg(MMICC_BUTTON_PDA_OPTION_CTRL_ID, &button_bg);
    GUIBUTTON_SetRunSheen(MMICC_BUTTON_PDA_OPTION_CTRL_ID, FALSE);
    
#if 0
    label_init.both_rect.v_rect.left = CC_OPTION_LABEL_LEFT;
    label_init.both_rect.v_rect.right = CC_OPTION_LABEL_RIGHT;
    label_init.both_rect.v_rect.top = CC_OPTION_LABEL_TOP + client_rect.v_rect.top;
    label_init.both_rect.v_rect.bottom =  CC_OPTION_LABEL_BOTTOM + client_rect.v_rect.top;

    label_init.both_rect.h_rect.left = CC_OPTION_LABEL_LEFT_H;
    label_init.both_rect.h_rect.right = CC_OPTION_LABEL_RIGHT_H;
    label_init.both_rect.h_rect.top = CC_OPTION_LABEL_TOP_H + client_rect.h_rect.top;
    label_init.both_rect.h_rect.bottom =  CC_OPTION_LABEL_BOTTOM_H + client_rect.h_rect.top;

    PdaCallCreateLabelByTextId(win_id,
                                                    MMICC_CONNECTED_PDA_OPTION_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    STXT_OPTION, 
                                                    MMI_CALL_HOLD_TEXT_FONT, 
                                                    CC_TEXT_COLOR);
#endif    
}

/*****************************************************************************/
//  Description : 销毁通话界中面的option button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DestroyOptionButton(void)
{
    MMK_DestroyDynaCtrl(MMICC_BUTTON_PDA_OPTION_CTRL_ID);
    //MMK_DestroyDynaCtrl(MMICC_CONNECTED_PDA_OPTION_LABEL_CTRL_ID);
}
 
/*****************************************************************************/
//  Description :CC create text label
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaCallCreateLabelByTextPtr(MMI_WIN_ID_T win_id,
                                                            MMI_CTRL_ID_T ctrl_id, 
                                                            GUILABEL_INIT_DATA_T label_init, 
                                                            MMI_STRING_T *text_ptr, 
                                                            GUI_FONT_T font, 
                                                            GUI_COLOR_T font_color)
{

    MMI_CONTROL_CREATE_T    create = {0};

    if (0 == win_id || 0 == ctrl_id || text_ptr == 0)
    {
        return ;
    }

    //text is null, create with TXT_NULL
    if (0 == text_ptr->wstr_len || PNULL == text_ptr->wstr_ptr)
    {
        PdaCallCreateLabelByTextId(win_id, ctrl_id, label_init, TXT_NULL, font, font_color);
        return ;
    }
    
    if (GUILABEL_ALIGN_NONE == label_init.align)
    {
        label_init.align = GUILABEL_ALIGN_LEFT;
    }

    if (0 != MMK_GetCtrlHandleByWin( win_id, ctrl_id ))
    {
        GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, ctrl_id), &label_init.both_rect);
    }
    else
    {
        create.ctrl_id = ctrl_id;
        create.guid    = SPRD_GUI_LABEL_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &label_init;
        MMK_CreateControl(&create); 
    }

    if (CAF_FONT_0 == font)
    {
        font = MMI_CALL_INFO_FONT;
    }

    if (0 == font_color)
    {
        font_color = MMI_BLACK_COLOR;
    }
    GUILABEL_SetText(ctrl_id, text_ptr, FALSE);
    GUILABEL_SetFont(ctrl_id, font, font_color);

}

/*****************************************************************************/
//  Description :CC create text label
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaCallCreateLabelByTextId(MMI_WIN_ID_T win_id,
                                                            MMI_CTRL_ID_T ctrl_id, 
                                                            GUILABEL_INIT_DATA_T label_init, 
                                                            MMI_TEXT_ID_T text_id, 
                                                            GUI_FONT_T font, 
                                                            GUI_COLOR_T font_color)
{

    MMI_CONTROL_CREATE_T    create = {0};
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin( win_id, ctrl_id );
    
    if (0 == win_id || 0 == ctrl_id || text_id == 0)
    {
        label_init.align = GUILABEL_ALIGN_LEFT;
    }

    if (0 != ctrl_handle)
    {
        GUIAPICTRL_SetBothRect(ctrl_handle, &label_init.both_rect);
    }
    else
    {
        create.ctrl_id = ctrl_id;
        create.guid    = SPRD_GUI_LABEL_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &label_init;
        MMK_CreateControl(&create); 
    }

    if (CAF_FONT_0 == font)
    {
        font = MMI_CALL_INFO_FONT;
    }

    if (0 == font_color)
    {
        font_color = MMI_BLACK_COLOR;
    }
    GUILABEL_SetTextById(ctrl_id, text_id, FALSE);
    GUILABEL_SetFont(ctrl_id, font, font_color);
}

/*****************************************************************************/
//  Description :get "N People" string
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaGetConferenceNumStr(MMI_STRING_T *conference_people, BOOLEAN is_hold_specific)
{
    MMI_STRING_T           people_str = {0};
    uint16                        conference_number = 0;
    char                 temp_char = 0;
    wchar               temp_wchar = 0;
    wchar               space_ch = ' ';
    wchar               temp2[10] = {0};  

    if (PNULL == conference_people || PNULL == conference_people->wstr_ptr)
    {
        return ;
    }

    if (is_hold_specific)
    {
        conference_number = MMIAPICC_GetHoldCallCount();
    }
    else
    {
        if (MMICC_IsExistActiveCall())
        {
            conference_number = MMIAPICC_GetActiveCallCount();
        }
        else
        {
            conference_number = MMIAPICC_GetHoldCallCount();
        }
    }

    conference_people->wstr_len = 8;
    
    temp_char = conference_number + '0';
    
    people_str.wstr_ptr = temp2;
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_CC_PEOPLE), &people_str);

    MMI_STRNTOWSTR(&temp_wchar,1, (uint8 *)&temp_char,1, 1);
    MMI_WSTRNCPY(conference_people->wstr_ptr, 1, &temp_wchar, 1, 1);
    MMI_WSTRNCPY((conference_people->wstr_ptr + 1), 1, &space_ch, 1, 1);
    MMI_WSTRNCPY((conference_people->wstr_ptr + 2), 6, people_str.wstr_ptr, 6, 6);
}

/*****************************************************************************/
//  Description : create labels for single line cc screen, single line meas this is a single active/hold call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplaySingleCallInfoForCommon(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, CC_PHOTO_TYPE_E photo_type
#ifdef MMI_CALLCONTROL_MINI_SUPPORT                                             
                                             ,BOOLEAN is_exist_time_lable,
                                             uint16 state_color
#endif
                                             )
{
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint16 emergency_call_len = 0;
#endif
    int32 space = 0;
    GUI_COLOR_T font_color = 0;
    GUILABEL_INIT_DATA_T label_init = {0};
    
    if (PNULL == prompt_str_array)
    {
        //SCI_TRACE_LOW:"ccapp err DisplayCallInfoForSingle, null ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_5067_112_2_18_2_10_50_318,(uint8*)"");
        return;
    }

    CHECK_CC_APPLET_NOT_NULL

    space = GetCallInfoCtrlSpace();
    
    MMK_DestroyControl(MMICC_CONNECTED_NAME_LABEL_CTRL_ID);
    MMK_DestroyControl(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID);
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        MMK_DestroyControl(MMICC_CONNECTED_TIME_LABEL_CTRL_ID);
        MMK_DestroyControl(MMICC_CONNECTED_SIM_LABEL_CTRL_ID);
#endif

    label_init.both_rect.v_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT;
    label_init.both_rect.v_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT;
    label_init.both_rect.v_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
    label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*/

    label_init.both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT_H;
    label_init.both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT_H;
#ifndef MMI_CALLCONTROL_MINI_SUPPORT   
    label_init.both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
    label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/
#else
    label_init.both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP_H + client_rect.h_rect.top - space;/*lint !e737*/
    label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM_H + client_rect.h_rect.top - space;/*lint !e737*/
#endif  

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_STATE_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &prompt_str_array[CC_DISPALY_INFO_STATE], //when it has time string, state is time 
                                                    MMI_CALL_INFO_FONT,
                                                    MMI_CALL_STATE_TEXT_COLOR /*lint !e572*/
                                                    );
#else
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && !defined(MMI_MULTI_SIM_SYS_NONE) 
    if (!MMIAPICC_IsInputEmergencyCall(&prompt_str_array[CC_DISPALY_INFO_NUMBER], &emergency_call_len, MMI_DUAL_SYS_MAX)
#ifdef BT_DIALER_SUPPORT        
        && !MMICC_GetBtCallFlag()
#endif     
	)    
    {
        MMI_STRING_T display_string = {0};
        wchar* display_str_ptr = PNULL;
        uint8 sim_str_len = 0;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMISET_SIM_NAME_T   sim_name = {0};
#endif
        wchar sim_str[MMISET_MAX_SIM_NAME_LEN + 1] = {0};

        uint8 line_str_len = 0;
#ifdef MMI_CSP_SUPPORT
        uint8 line = 0;
        MMI_STRING_T line_str = {0};
#endif
        const wchar space_ch = {' '};
        const size_t space_ch_len = 1;

        //get sim str
        sim_name = MMIAPISET_GetSimName(MMIAPICC_GetCallDualSys());
        
        sim_str_len = sim_name.wstr_len + space_ch_len;
        MMI_WSTRNCPY(sim_str, sim_name.wstr_len, sim_name.wstr_arr, sim_name.wstr_len, sim_name.wstr_len);
        MMI_WSTRNCPY(sim_str + sim_name.wstr_len, space_ch_len, &space_ch, space_ch_len, space_ch_len); 

#ifdef MMI_CSP_SUPPORT
        if(MMISET_IsAlsValid(MMIAPICC_GetCallDualSys()))
        {
            line = MMIAPISET_GetCallLineInfo(MMIAPICC_GetCallDualSys());
            if(0 == line)
            {
                MMI_GetLabelTextByLang(TXT_LINE_1, &line_str);
            }
            else if(1 == line)
            {
                MMI_GetLabelTextByLang(TXT_LINE_2, &line_str);
            }
            line_str_len = line_str.wstr_len;
        }
#endif//MMI_CSP_SUPPORT

        display_string.wstr_len = (line_str_len > 0)?(sim_str_len + line_str_len + space_ch_len ):(sim_str_len );
        display_str_ptr = SCI_ALLOC_APPZ((display_string.wstr_len + 1)* sizeof(wchar));
        display_string.wstr_ptr = display_str_ptr;
        
        MMI_WSTRNCPY(display_str_ptr, sim_str_len, sim_str, sim_str_len, sim_str_len);
#ifdef MMI_CSP_SUPPORT
        if(line_str_len > 0)
        {
            MMI_WSTRNCPY(display_str_ptr + sim_str_len, line_str_len, line_str.wstr_ptr, line_str_len, line_str_len);
            MMI_WSTRNCPY(display_str_ptr + sim_str_len + line_str_len, space_ch_len, &space_ch, space_ch_len, space_ch_len);
        }
#endif //#ifdef MMI_CSP_SUPPORT
        label_init.both_rect.v_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_MAIN_INFO_STATE_LABEL_LEFT + CC_MAIN_INFO_STATE_LABEL_OFFSET;

        label_init.both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_LEFT_H + CC_MAIN_INFO_STATE_LABEL_OFFSET_H;
        PdaCallCreateLabelByTextPtr(win_id,
                                                            MMICC_CONNECTED_SIM_LABEL_CTRL_ID, 
                                                            label_init, 
                                                            &display_string, //when it has time string, state is time 
                                                            MMI_CALL_INFO_FONT, 
                                                            state_color);

        label_init.both_rect.v_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT + CC_MAIN_INFO_STATE_LABEL_OFFSET;
        label_init.both_rect.v_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT;

        label_init.both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT_H + CC_MAIN_INFO_STATE_LABEL_OFFSET_H;
        label_init.both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT_H;

        PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_STATE_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &prompt_str_array[CC_DISPALY_INFO_STATE],  
                                                    MMI_CALL_INFO_FONT,                                               
                                                    state_color                                                  
                                                    );/*lint !e572*/
                                                    
        SCI_FREE(display_str_ptr);
    }
    else
#endif
    {
        PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_STATE_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &prompt_str_array[CC_DISPALY_INFO_STATE],  
                                                    MMI_CALL_INFO_FONT,                                               
                                                    state_color                                                  
                                                    );/*lint !e572*/        
    }
#endif
    


#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    //create call location
    label_init.both_rect.v_rect.left = CC_MAIN_INFO_LOCAT_LABEL_LEFT;
    label_init.both_rect.v_rect.right = CC_MAIN_INFO_LOCAT_LABEL_RIGHT;
    label_init.both_rect.v_rect.top = CC_MAIN_INFO_LOCAT_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
    label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_LOCAT_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*/

    label_init.both_rect.h_rect.left = CC_MAIN_INFO_LOCAT_LABEL_LEFT_H;
    label_init.both_rect.h_rect.right = CC_MAIN_INFO_LOCAT_LABEL_RIGHT_H;
    label_init.both_rect.h_rect.top = CC_MAIN_INFO_LOCAT_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
    label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_LOCAT_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsInputEmergencyCall(&prompt_str_array[CC_DISPALY_INFO_NUMBER], &emergency_call_len, MMI_DUAL_SYS_MAX))
    {
        MMI_STRING_T display_string = {0};
        wchar* display_str_ptr = PNULL;
        uint8 sim_str_len = 0;
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && !defined(MMI_MULTI_SIM_SYS_NONE) 
        MMISET_SIM_NAME_T   sim_name = {0};
#endif
        wchar sim_str[MMISET_MAX_SIM_NAME_LEN + 1] = {0};

#ifdef MMI_CSP_SUPPORT
        uint8 line_str_len = 0;
        uint8 line = 0;
        MMI_STRING_T line_str = {0};
#endif
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && !defined(MMI_MULTI_SIM_SYS_NONE) 
        const wchar space_ch = {' '};
        const size_t space_ch_len = 1;
#endif
        MMI_STRING_T location_str = {0};

        SCI_MEMCPY(&location_str, &(prompt_str_array[CC_DISPALY_INFO_LOCATION]), sizeof(MMI_STRING_T));
        //location_str.wstr_ptr = (MMI_STRING_T*)(&prompt_str_array[CC_DISPALY_INFO_LOCATION])->wstr_ptr;
        //location_str.wstr_len = prompt_str_array[CC_DISPALY_INFO_LOCATION].wstr_len;

        //get sim str
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && !defined(MMI_MULTI_SIM_SYS_NONE) 
        sim_name = MMIAPISET_GetSimName(MMIAPICC_GetCallDualSys());
        
        sim_str_len = sim_name.wstr_len + space_ch_len;
        MMI_WSTRNCPY(sim_str, sim_name.wstr_len, sim_name.wstr_arr, sim_name.wstr_len, sim_name.wstr_len);
        MMI_WSTRNCPY(sim_str + sim_name.wstr_len, space_ch_len, &space_ch, space_ch_len, space_ch_len);
#endif//MMI_MULTI_SIM_SYS_SINGLE  

#ifdef MMI_CSP_SUPPORT
        if(MMISET_IsAlsValid(MMIAPICC_GetCallDualSys()))
        {
            line = MMIAPISET_GetCallLineInfo(MMIAPICC_GetCallDualSys());
            if(0 == line)
            {
                MMI_GetLabelTextByLang(TXT_LINE_1, &line_str);
            }
            else if(1 == line)
            {
                MMI_GetLabelTextByLang(TXT_LINE_2, &line_str);
            }
            line_str_len = line_str.wstr_len;
        }
        if(line_str_len > 0)
        {
            display_string.wstr_len = sim_str_len + line_str_len + space_ch_len + location_str.wstr_len;
        }
        else
#endif//MMI_CSP_SUPPORT
        {
           display_string.wstr_len = sim_str_len + location_str.wstr_len; 
        }
        display_str_ptr = SCI_ALLOC_APPZ((display_string.wstr_len + 1)* sizeof(wchar));
        display_string.wstr_ptr = display_str_ptr;

/*#ifndef MMI_MULTI_SIM_SYS_SINGLE*/
        MMI_WSTRNCPY(display_str_ptr, sim_str_len, sim_str, sim_str_len, sim_str_len);
/*#endif*/

#ifdef MMI_CSP_SUPPORT
        if(line_str_len > 0)
        {
            MMI_WSTRNCPY(display_str_ptr + sim_str_len, line_str_len, line_str.wstr_ptr, line_str_len, line_str_len);
            MMI_WSTRNCPY(display_str_ptr + sim_str_len + line_str_len, space_ch_len, &space_ch, space_ch_len, space_ch_len);
            MMI_WSTRNCPY(display_str_ptr + sim_str_len + line_str_len + space_ch_len, location_str.wstr_len, location_str.wstr_ptr, location_str.wstr_len, location_str.wstr_len);
        }
        else
#endif //#ifdef MMI_CSP_SUPPORT
        {
            MMI_WSTRNCPY(display_str_ptr + sim_str_len , location_str.wstr_len, location_str.wstr_ptr, location_str.wstr_len, location_str.wstr_len);
        }
        PdaCallCreateLabelByTextPtr(win_id,
                                                            MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, 
                                                            label_init, 
                                                            &display_string, //when it has time string, state is time 
                                                            MMI_CALL_INFO_FONT, 
                                                            font_color);
        SCI_FREE(display_str_ptr);
    }
    else
#endif
    {
    PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &prompt_str_array[CC_DISPALY_INFO_LOCATION], 
                                                    MMI_CALL_INFO_FONT, 
                                                    font_color);
    }
#if !defined MMI_MULTI_SIM_SYS_SINGLE && !defined( MMI_MULTI_SIM_SYS_NONE )
#ifdef MMI_MULTISIM_COLOR_SUPPORT
        if (!MMIAPICC_IsInputEmergencyCall(&prompt_str_array[CC_DISPALY_INFO_NUMBER], &emergency_call_len, MMI_DUAL_SYS_MAX))
        {
            GUI_COLOR_T color = MMIAPISET_GetSimColor(MMIAPICC_GetCallDualSys());
            GUILABEL_SetFont(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, MMI_CALL_INFO_FONT, color);
        }
#endif
#endif
#else
	 if(is_exist_time_lable)
        {
        	label_init.both_rect.v_rect.left = CC_MAIN_INFO_TIME_LABEL_LEFT;
	       label_init.both_rect.v_rect.right = CC_MAIN_INFO_TIME_LABEL_RIGHT;
	       label_init.both_rect.v_rect.top = CC_MAIN_INFO_TIME_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
	       label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_TIME_LABEL_BOTTOM+ client_rect.v_rect.top - space;/*lint !e737*/

	       label_init.both_rect.h_rect.left = CC_MAIN_INFO_TIME_LABEL_LEFT_H;
	       label_init.both_rect.h_rect.right = CC_MAIN_INFO_TIME_LABEL_RIGHT_H;
	       label_init.both_rect.h_rect.top = CC_MAIN_INFO_TIME_LABEL_TOP_H + client_rect.h_rect.top - space;/*lint !e737*/
	       label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_TIME_LABEL_BOTTOM_H + client_rect.h_rect.top - space;/*lint !e737*/

	       PdaCallCreateLabelByTextPtr(win_id,
	                                                        MMICC_CONNECTED_TIME_LABEL_CTRL_ID, 
	                                                        label_init, 
	                                                        &prompt_str_array[CC_DISPALY_INFO_TIME], 
	                                                        MMI_CALL_INFO_FONT, 
	                                                        font_color); 
        }       
#endif

    if (prompt_str_array[CC_DISPALY_INFO_NAME].wstr_len > 0)
    {
        //create call name
        label_init.both_rect.v_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.v_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
            label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }
        else
#endif
        {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }           

        label_init.both_rect.h_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT_H;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H+ client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }
        else
         {
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top - space;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H+ client_rect.h_rect.top - space;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }
#else
	 {
	 	label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top;/*lint !e737*/
        	label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
	 }
#endif
        PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_NAME_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &prompt_str_array[CC_DISPALY_INFO_NAME], 
                                                    MMI_CALL_NAME_FONT, 
                                                    font_color);

        //create call number
        label_init.both_rect.v_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.v_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
            label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
        }  
        else
#endif  
        {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - space;/*lint !e737*/
        }
        label_init.both_rect.h_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT_H;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H+ client_rect.h_rect.top  - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H+ client_rect.h_rect.top - space  - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
        }  
        else
        {
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H+ client_rect.h_rect.top  - space;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H+ client_rect.h_rect.top - space;/*lint !e737*/
        }  
#else
	{
		label_init.both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
        	label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/
        }
#endif 

        PdaCallCreateLabelByTextPtr(win_id,
                                                        MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, 
                                                        label_init, 
                                                        &prompt_str_array[CC_DISPALY_INFO_NUMBER], 
                                                        MMI_CALL_INFO_FONT, 
                                                        font_color);        
    }
    else
    {
        //create call number
        label_init.both_rect.v_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.v_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
            label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }
        else
#endif
        {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }   

        label_init.both_rect.h_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT_H;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H + client_rect.h_rect.top  - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H+ client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }
        else
        {
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H + client_rect.h_rect.top  - space;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H+ client_rect.h_rect.top - space;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
        }
#else
	{
		label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top;/*lint !e737*/
        	label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;

	}
#endif  
        PdaCallCreateLabelByTextPtr(win_id,
                                                        MMICC_CONNECTED_NAME_LABEL_CTRL_ID, 
                                                        label_init, 
                                                        &prompt_str_array[CC_DISPALY_INFO_NUMBER], 
                                                        MMI_CALL_NAME_FONT, 
                                                        font_color);

        //create call number
        label_init.both_rect.v_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if(!is_exist_time_lable)
        {
            label_init.both_rect.v_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
            label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
        } 
        else
#endif
        {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - space;/*lint !e737*/
        }

        PdaCallCreateLabelByTextId(win_id,
                                                        MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, 
                                                        label_init, 
                                                        TXT_NULL, 
                                                        MMI_CALL_INFO_FONT, 
                                                        font_color);        
    }


    //create auto redial times or asp time
    
    /*if (MMICC_AUTO_DIAL_WIN_ID == win_id)
    {
#ifdef MAINLCD_SIZE_240X320
    //create redial times
        label_init.both_rect.v_rect.left = client_rect.v_rect.left;
        label_init.both_rect.v_rect.right = client_rect.v_rect.right;
        label_init.both_rect.v_rect.top = MMICC_CC_PDA_CALL_INFO_TOP + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) + (GUIFONT_GetHeight(MMI_CALL_INFO_FONT) + 5);
        label_init.both_rect.v_rect.bottom =  label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
#else
        label_init.both_rect.v_rect.left = client_rect.v_rect.left;
        label_init.both_rect.v_rect.right = client_rect.v_rect.right;
        label_init.both_rect.v_rect.top = MMICC_CC_PDA_CALL_INFO_TOP + 3*(GUIFONT_GetHeight(MMI_CALL_INFO_FONT) + 3);
        label_init.both_rect.v_rect.bottom =  label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
#endif
    
        PdaCallCreateLabelByTextPtr(win_id,
                                                        MMICC_CONNECTED_REDIAL_TIMES_LABEL_CTRL_ID, 
                                                        label_init, 
                                                        &prompt_str_array[CC_DISPALY_INFO_AUTODIAL_TIMES], 
                                                        MMI_CALL_INFO_FONT, 
                                                        font_color);
    }*/
    MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), MMK_ConvertIdToHandle(MMICC_CONNECTED_STATE_LABEL_CTRL_ID));
    //create call photo
    PdaDisplayCallPhotoForCommon(win_id, photo_type);  
}

/*****************************************************************************/
//  Description : create labels for MPTY cc screen
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL int32 GetCallInfoCtrlSpace(void)
{
    int32 space = 0;

    if(MMICC_IsExistOutgoingCall() || MMICC_IsExistIncommingCall() || !MMICC_IsExistActiveAndHoldCalls())
    {
        space = SINGLE_CALL_SPACE;
    }

    return space;
}


/*****************************************************************************/
//  Description : create labels for MPTY cc screen
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayMPTYCallInfoForCommon(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, CC_PHOTO_TYPE_E photo_type
 #ifdef MMI_CALLCONTROL_MINI_SUPPORT
   , BOOLEAN is_exist_time_lable,
   uint16 state_color
 #endif
)
{
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);

    GUILABEL_INIT_DATA_T label_init = {0};
    GUI_COLOR_T font_color = 0;
    MMI_STRING_T		 conference_people = {0};
    wchar               temp_arr[20] = {0};
    int32 space = GetCallInfoCtrlSpace();
    conference_people.wstr_ptr = temp_arr;
    
    MMK_DestroyControl(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID);

#ifdef MMI_CALLCONTROL_MINI_SUPPORT 
    MMK_DestroyControl(MMICC_CONNECTED_TIME_LABEL_CTRL_ID);
    MMK_DestroyControl(MMICC_CONNECTED_SIM_LABEL_CTRL_ID);
#endif

    //create state info or call time
    if (MMICC_IsExistHoldCall())
    {
        font_color = MMI_CALL_HOLD_COLOR;
    }
    else 
    {
        font_color = CC_TEXT_COLOR;
    }
    
    //create state info or call time
    label_init.both_rect.v_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT;
    label_init.both_rect.v_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT;
    label_init.both_rect.v_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP + client_rect.v_rect.top -space;/*lint !e737*/
    label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*/

    label_init.both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT_H;
    label_init.both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT_H;
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    label_init.both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
    label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/
#else
    label_init.both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP_H + client_rect.h_rect.top -space;/*lint !e737*/
    label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM_H + client_rect.h_rect.top -space;/*lint !e737*/
#endif
    
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
     PdaCallCreateLabelByTextPtr(win_id,
                                            MMICC_CONNECTED_STATE_LABEL_CTRL_ID, 
                                            label_init, 
                                            &prompt_str_array[CC_DISPALY_INFO_STATE], //when it has time string, state is time 
                                            MMI_CALL_INFO_FONT, 
                                            state_color);/*lint !e572*/      
#else
    PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_STATE_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &prompt_str_array[CC_DISPALY_INFO_STATE], //when it has time string, state is time 
                                                    MMI_CALL_INFO_FONT, 
                                                    MMI_CALL_STATE_TEXT_COLOR);/*lint !e572*/
#endif

#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    //create time
    if(is_exist_time_lable)
    {
        label_init.both_rect.v_rect.left = CC_MAIN_INFO_TIME_LABEL_LEFT;
	    label_init.both_rect.v_rect.right = CC_MAIN_INFO_TIME_LABEL_RIGHT;
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_TIME_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_TIME_LABEL_BOTTOM+ client_rect.v_rect.top - space;/*lint !e737*/
       
        label_init.both_rect.h_rect.left = CC_MAIN_INFO_TIME_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_MAIN_INFO_TIME_LABEL_RIGHT_H;
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_TIME_LABEL_TOP_H + client_rect.h_rect.top - space;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_TIME_LABEL_BOTTOM_H + client_rect.h_rect.top - space;/*lint !e737*/
       
        PdaCallCreateLabelByTextPtr(win_id,
            MMICC_CONNECTED_TIME_LABEL_CTRL_ID, 
            label_init, 
            &prompt_str_array[CC_DISPALY_INFO_TIME], 
            MMI_CALL_INFO_FONT, 
            CC_TEXT_COLOR);
    } 
#endif
    
    //create call name
    label_init.both_rect.v_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT;
    label_init.both_rect.v_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    if(!is_exist_time_lable)
    {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top -space - SINGLE_CALL_HEIGHT;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
    }
    else
#endif
    {    
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP + client_rect.v_rect.top -space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
    }

    label_init.both_rect.h_rect.left = CC_MAIN_INFO_NAME_LABEL_LEFT_H;
    label_init.both_rect.h_rect.right = CC_MAIN_INFO_NAME_LABEL_RIGHT_H;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    if(!is_exist_time_lable)
    {
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H + client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
    }
    else 
    {
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top - space;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H + client_rect.h_rect.top - space;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
    }
#else
    {    
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_NAME_LABEL_TOP_H+ client_rect.h_rect.top;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NAME_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*///label_init.both_rect.v_rect.top + GUIFONT_GetHeight(MMI_CALL_INFO_FONT) ;
    }  
#endif

    PdaCallCreateLabelByTextId(win_id,
                                                    MMICC_CONNECTED_NAME_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    TXT_CC_MULTICALL, 
                                                    MMI_CALL_NAME_FONT, 
                                                    CC_TEXT_COLOR);
                                                       
    //create call people number
    label_init.both_rect.v_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT;
    label_init.both_rect.v_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    if(!is_exist_time_lable)
    {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - space - SINGLE_CALL_HEIGHT;/*lint !e737*/
    }
    else
#endif
    {
        label_init.both_rect.v_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
        label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM+ client_rect.v_rect.top - space;/*lint !e737*/
    }

    label_init.both_rect.h_rect.left = CC_MAIN_INFO_NUM_LABEL_LEFT_H;
    label_init.both_rect.h_rect.right = CC_MAIN_INFO_NUM_LABEL_RIGHT_H;
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    if(!is_exist_time_lable)
    {
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H + client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H + client_rect.h_rect.top - space - SINGLE_CALL_HEIGHT_H;/*lint !e737*/
    }
    else
    {
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H + client_rect.h_rect.top - space;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H + client_rect.h_rect.top- space;/*lint !e737*/
    }
#else
    {
        label_init.both_rect.h_rect.top = CC_MAIN_INFO_NUM_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
        label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_NUM_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/
    }
#endif 

    PdaGetConferenceNumStr(&conference_people, FALSE);        
    PdaCallCreateLabelByTextPtr(win_id,
                                                    MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, 
                                                    label_init, 
                                                    &conference_people,
                                                    MMI_CALL_INFO_FONT, 
                                                    CC_TEXT_COLOR);
                                                    
    PdaDisplayCallPhotoForCommon(win_id, photo_type);
}

/*****************************************************************************/
//  Description :create photo for common cc screen
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayCallPhotoForCommon( MMI_WIN_ID_T win_id, CC_PHOTO_TYPE_E photo_type)
{
    GUIANIM_INIT_DATA_T anim_init = {0};
    MMI_CTRL_ID_T anim_ctrl_id = 0;
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    int32 space = GetCallInfoCtrlSpace();

    anim_init.both_rect.v_rect.left = CC_MAIN_INFO_ANIM_LEFT;
    anim_init.both_rect.v_rect.right = CC_MAIN_INFO_ANIM_RIGHT;
    anim_init.both_rect.v_rect.top = CC_MAIN_INFO_ANIM_TOP + client_both_rect.v_rect.top - space;/*lint !e737*/
    anim_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_ANIM_BOTTOM  + client_both_rect.v_rect.top - space;/*lint !e737*/

    anim_init.both_rect.h_rect.left = CC_MAIN_INFO_ANIM_LEFT_H;
    anim_init.both_rect.h_rect.right = CC_MAIN_INFO_ANIM_RIGHT_H;
#ifndef MMI_CALLCONTROL_MINI_SUPPORT   
    anim_init.both_rect.h_rect.top = CC_MAIN_INFO_ANIM_TOP_H + client_both_rect.h_rect.top;/*lint !e737*/
    anim_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_ANIM_BOTTOM_H+ client_both_rect.h_rect.top;/*lint !e737*/
#else
    anim_init.both_rect.h_rect.top = CC_MAIN_INFO_ANIM_TOP_H + client_both_rect.h_rect.top - space;/*lint !e737*/
    anim_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_ANIM_BOTTOM_H+ client_both_rect.h_rect.top - space;/*lint !e737*/
#endif

    switch (win_id)
    {
        case MMICC_DISCONNECT_WIN_ID:
            anim_ctrl_id = MMICC_DISCONNECTED_PHOTO_ANIM_CTRL_ID;
            break;
            
        case MMICC_STATUS_WIN_ID:
            anim_ctrl_id = MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID;
            break;

        case MMICC_ANIMATION_WIN_ID:
        case MMICC_AUTO_DIAL_WIN_ID:
            anim_ctrl_id = MMICC_CALLING_ANIM_FORM_CTRL_ID;
            break;
            
        default:
            anim_ctrl_id = MMICC_CALLING_ANIM_FORM_CTRL_ID;
            break;
    }

    MMK_DestroyControl(anim_ctrl_id);
            
    CreatePhotoAnim(win_id, anim_ctrl_id, &anim_init.both_rect, photo_type);
}
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description :add anim for 128X160/ 176X220 pro CC
//  Global resource dependence : none
//  Author:chunjie liu
//  Note:
/*****************************************************************************/
LOCAL void DisplayCallAnimPhotoForCommon( MMI_WIN_ID_T win_id,MMI_IMAGE_ID_T anim_id)
{
    GUIANIM_INIT_DATA_T anim_init = {0};
    MMI_CTRL_ID_T anim_ctrl_id = MMICC_BOTTOM_PHOTO_ANIM_CTRL_ID;
    MMI_CONTROL_CREATE_T anim_create = {0};
    //CC_CALL_STATE_E call_state = CC_NULL_STATE;
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    anim_init.both_rect.v_rect.left = CC_MAIN_INFO_BOTTOM_ANIM_LEFT;
    anim_init.both_rect.v_rect.right = CC_MAIN_INFO_BOTTOM_ANIM_RIGHT;
    anim_init.both_rect.v_rect.top = CC_MAIN_INFO_BOTTOM_ANIM_TOP + client_both_rect.v_rect.top;/*lint !e737*/
    anim_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_BOTTOM_ANIM_BOTTOM  + client_both_rect.v_rect.top;/*lint !e737*/

    anim_init.both_rect.h_rect.left = CC_MAIN_INFO_BOTTOM_ANIM_LEFT_H;
    anim_init.both_rect.h_rect.right = CC_MAIN_INFO_BOTTOM_ANIM_RIGHT_H;
    anim_init.both_rect.h_rect.top = CC_MAIN_INFO_BOTTOM_ANIM_TOP_H + client_both_rect.h_rect.top;/*lint !e737*/
    anim_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_BOTTOM_ANIM_BOTTOM_H+ client_both_rect.h_rect.top;/*lint !e737*/

    //create bottom photo anim
    anim_create.guid = SPRD_GUI_ANIM_ID;
    anim_create.ctrl_id = anim_ctrl_id;
    anim_create.parent_win_handle = win_id;
    anim_create.init_data_ptr = &anim_init;
    MMK_CreateControl(&anim_create);
     //set photo anim
    {
        GUIANIM_CTRL_INFO_T control_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};

        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = anim_ctrl_id;

        data_info.img_id = anim_id;
        
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;  
        display_info.bg.bg_type = GUI_BG_COLOR;
        display_info.bg.color = MMI_CALL_ANIM_BG_COLOR;              
        GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    }
        
}
#endif

/*****************************************************************************/
//  Description :single call, only a active or hold call, or a active/hold MPTY call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplaySingleCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array
#ifdef MMI_CALLCONTROL_MINI_SUPPORT                                             
                                             ,BOOLEAN is_exist_time_lable,
                                             uint16 state_color
#endif
)
{
    MMI_STRING_T		 conference_people = {0};
    wchar               temp_arr[12] = {0};
    conference_people.wstr_ptr = temp_arr;
    
    if (PNULL == prompt_str_array)
    {
        //SCI_TRACE_LOW:"ccapp err DisplayCallInfoForSingle, null ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_5428_112_2_18_2_10_51_319,(uint8*)"");
        return;
    }
    
    //destroy multi call ctrl, in case of return from multi call
    MMK_DestroyControl(MMICC_CONNECTED_HOLD_STATE_LABEL_CTRL_ID);
    MMK_DestroyControl(MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID);
    MMK_DestroyControl(MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID);
    MMK_DestroyControl(MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID);
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    MMK_DestroyControl(MMICC_MULTI_CALL_LINE_CTRL_ID);
#endif
    if (MMICC_IsExistActiveCall() || MMICC_IsExistHoldCall())
    {
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            ConstructOptionButton(win_id);
        }
		else
#endif		
		{
			DestroyOptionButton();
		}
    }

    if (MMICC_IsExistMPTY() &&!( MMICC_IsExistOutgoingCall() || MMICC_IsExistIncommingCall()))
    {
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    
        if(!is_exist_time_lable)
        {
            PdaDisplayMPTYCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_CURRENT,is_exist_time_lable,MMI_CALL_HOLD_COLOR);
        }
        else
        {
            PdaDisplayMPTYCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_CURRENT,is_exist_time_lable,MMI_CALL_STATE_TEXT_COLOR);
        }
#else
        PdaDisplayMPTYCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_CURRENT);
#endif
    }
    else
    {
        PdaDisplaySingleCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_CURRENT
#ifdef MMI_CALLCONTROL_MINI_SUPPORT   
    ,is_exist_time_lable,
    state_color
#endif 
            );
    }
}

/*****************************************************************************/
//  Description :multi line call, display active call in this cases: 
//                                                                                  active + hold call
//                                                                                  MPTY + hold call
//                                                                                  active + MPTY call
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayMultiActiveCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array)
{  
    MMI_STRING_T		 conference_people = {0};
    wchar               temp_arr[12] = {0};
    conference_people.wstr_ptr = temp_arr;
    
    if (PNULL == prompt_str_array)
    {
        //SCI_TRACE_LOW:"ccapp err DisplayCallInfoForSingle, null ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_5472_112_2_18_2_10_51_320,(uint8*)"");
        return;
    }
    
    if (MMICC_IsExistActiveMPTY())
    {
        PdaDisplayMPTYCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_ACTIVE
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    
    ,TRUE,
    MMI_CALL_STATE_TEXT_COLOR
#endif 
        );
    }
    else
    {
        PdaDisplaySingleCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_ACTIVE
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    
    ,TRUE,
    MMI_CALL_STATE_TEXT_COLOR
#endif            
            );
    }
}

/*****************************************************************************/
//  Description :multi line call, display hold call in this cases: 
//                                                                                  active + hold call
//                                                                                  MPTY + hold call
//                                                                                  active + MPTY call
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayCallInfoForMultiHoldCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array)
{
    MMI_STRING_T call_name_str = {0};
    uint8 call_id = 0;

    wchar temp_tel_num[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
    uint8 tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8 tel_num_len = 0;
    MMI_STRING_T num_str = {0};
    
    GUILABEL_INIT_DATA_T label_init = {0};
    GUIANIM_INIT_DATA_T anim_init = {0};
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    call_id = MMICC_FindStateCall(CC_HOLD_STATE);

    //draw line
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    if (1 < MMICC_GetCallNum())
    {
        GUI_BOTH_RECT_T line_rect = {0};

        line_rect.v_rect.left = 0;
        line_rect.v_rect.right = client_both_rect.v_rect.right;
        line_rect.v_rect.top = CC_MULTI_CALL_LINE_TOP;
        line_rect.v_rect.bottom = line_rect.v_rect.top;
        line_rect.h_rect.left =0;
        line_rect.h_rect.right = client_both_rect.h_rect.right;
        line_rect.h_rect.top = CC_MULTI_CALL_LINE_TOP_H;
        line_rect.h_rect.bottom = line_rect.h_rect.top;
        DrawCcPanelLine(win_id, MMICC_MULTI_CALL_LINE_CTRL_ID, &line_rect);
    }
#endif
    //get hold call number
    MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, call_id);
    //if (MMIAPICOM_IsValidNumberString((char*)tel_num, tel_num_len))
    if (0 != tel_num_len)
    {
        num_str.wstr_len = tel_num_len;
        num_str.wstr_ptr = temp_tel_num;
        MMI_STRNTOWSTR(num_str.wstr_ptr,
            num_str.wstr_len,
            (uint8 *)tel_num,
            tel_num_len,
            tel_num_len);
    }
    else
    {
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &num_str);
    }
    
    /*if (0 != MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID))
    {
        //create call name
        if(MMICC_IsExistHoldMPTY())
        {
            GUILABEL_SetTextById(MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID, TXT_CC_MULTICALL, FALSE);
        }
        else
        {
            MMICC_GetCallNameStrByIndex(&call_name_str, call_id);

            GUILABEL_SetText(MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID, &call_name_str, FALSE);
        }

        GUILABEL_SetText(MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID, &num_str, FALSE);
    }
    else*/
    {
        //create state info
        label_init.both_rect.v_rect.left = CC_SEC_INFO_STATE_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_SEC_INFO_STATE_LABEL_RIGHT;
        label_init.both_rect.v_rect.top = CC_SEC_INFO_STATE_LABEL_TOP + client_both_rect.v_rect.top;
        label_init.both_rect.v_rect.bottom =  CC_SEC_INFO_STATE_LABEL_BOTTOM+ client_both_rect.v_rect.top;

        label_init.both_rect.h_rect.left = CC_SEC_INFO_STATE_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_SEC_INFO_STATE_LABEL_RIGHT_H;
        label_init.both_rect.h_rect.top = CC_SEC_INFO_STATE_LABEL_TOP_H + client_both_rect.h_rect.top;
        label_init.both_rect.h_rect.bottom =  CC_SEC_INFO_STATE_LABEL_BOTTOM_H + client_both_rect.h_rect.top;

        PdaCallCreateLabelByTextId(win_id,
                                                        MMICC_CONNECTED_HOLD_STATE_LABEL_CTRL_ID, 
                                                        label_init, 
                                                        TXT_CC_HOLD, 
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)                                                          
                                                        SONG_FONT_16, 
 #else
 								MMI_CALL_INFO_FONT,
 #endif                                                        
                                                        MMI_CALL_HOLD_COLOR);
        
        label_init.both_rect.v_rect.left = CC_SEC_INFO_NAME_LABEL_LEFT;
        label_init.both_rect.v_rect.right = CC_SEC_INFO_NAME_LABEL_RIGHT;
        label_init.both_rect.v_rect.top = CC_SEC_INFO_NAME_LABEL_TOP + client_both_rect.v_rect.top;
        label_init.both_rect.v_rect.bottom =  CC_SEC_INFO_NAME_LABEL_BOTTOM+ client_both_rect.v_rect.top;

        label_init.both_rect.h_rect.left = CC_SEC_INFO_NAME_LABEL_LEFT_H;
        label_init.both_rect.h_rect.right = CC_SEC_INFO_NAME_LABEL_RIGHT_H;
        label_init.both_rect.h_rect.top = CC_SEC_INFO_NAME_LABEL_TOP_H + client_both_rect.h_rect.top;
        label_init.both_rect.h_rect.bottom =  CC_SEC_INFO_NAME_LABEL_BOTTOM_H + client_both_rect.h_rect.top;
        //create call name
        if(MMICC_IsExistHoldMPTY())
        {            
            MMI_STRING_T conference_people = {0};
            wchar temp_arr[20] = {0};

            PdaCallCreateLabelByTextId(win_id,
                                                            MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID, 
                                                            label_init, 
                                                            TXT_CC_MULTICALL, 
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)
                                                            SONG_FONT_15, 
#else
								    MMI_CALL_INFO_FONT,
#endif
                                                            CC_TEXT_COLOR);

            //create num info
            label_init.both_rect.v_rect.left = CC_SEC_INFO_NUM_LABEL_LEFT;
            label_init.both_rect.v_rect.right = CC_SEC_INFO_NUM_LABEL_RIGHT;
            label_init.both_rect.v_rect.top = CC_SEC_INFO_NUM_LABEL_TOP + client_both_rect.v_rect.top;
            label_init.both_rect.v_rect.bottom =  CC_SEC_INFO_NUM_LABEL_BOTTOM+ client_both_rect.v_rect.top;

            label_init.both_rect.h_rect.left = CC_SEC_INFO_NUM_LABEL_LEFT_H;
            label_init.both_rect.h_rect.right = CC_SEC_INFO_NUM_LABEL_RIGHT_H;
            label_init.both_rect.h_rect.top = CC_SEC_INFO_NUM_LABEL_TOP_H + client_both_rect.h_rect.top;
            label_init.both_rect.h_rect.bottom =  CC_SEC_INFO_NUM_LABEL_BOTTOM_H + client_both_rect.h_rect.top;

            conference_people.wstr_ptr = temp_arr;
            PdaGetConferenceNumStr(&conference_people, TRUE);        
            PdaCallCreateLabelByTextPtr(win_id,
                                                            MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID, 
                                                            label_init, 
                                                            &conference_people,
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)
                                                            SONG_FONT_15, 
#else
								    MMI_CALL_INFO_FONT,
#endif
                                                            CC_TEXT_COLOR);
        }
        else //if(!MMICC_IsExistHoldMPTY())//exist a single hold call
        {
            if(MMICC_GetCallNameStrByIndex(&call_name_str, call_id))
            {
                PdaCallCreateLabelByTextPtr(win_id,
                                                                MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID, 
                                                                label_init, 
                                                                &call_name_str, 
                                                                SONG_FONT_12, 
                                                                CC_TEXT_COLOR);
                //create num info
                label_init.both_rect.v_rect.left = CC_SEC_INFO_NUM_LABEL_LEFT;
                label_init.both_rect.v_rect.right = CC_SEC_INFO_NUM_LABEL_RIGHT;
                label_init.both_rect.v_rect.top = CC_SEC_INFO_NUM_LABEL_TOP + client_both_rect.v_rect.top;
                label_init.both_rect.v_rect.bottom =  CC_SEC_INFO_NUM_LABEL_BOTTOM+ client_both_rect.v_rect.top;

                label_init.both_rect.h_rect.left = CC_SEC_INFO_NUM_LABEL_LEFT_H;
                label_init.both_rect.h_rect.right = CC_SEC_INFO_NUM_LABEL_RIGHT_H;
                label_init.both_rect.h_rect.top = CC_SEC_INFO_NUM_LABEL_TOP_H + client_both_rect.h_rect.top;
                label_init.both_rect.h_rect.bottom =  CC_SEC_INFO_NUM_LABEL_BOTTOM_H + client_both_rect.h_rect.top;
                
                PdaCallCreateLabelByTextPtr(win_id,
                                                                MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID, 
                                                                label_init, 
                                                                &num_str, 
                                                                SONG_FONT_12, 
                                                                CC_TEXT_COLOR);                
            }
            else
            {
                //create num info
                label_init.both_rect.v_rect.left = CC_SEC_INFO_NAME_LABEL_LEFT;
                label_init.both_rect.v_rect.right = CC_SEC_INFO_NAME_LABEL_RIGHT;
                label_init.both_rect.v_rect.top = CC_SEC_INFO_NAME_LABEL_TOP + client_both_rect.v_rect.top;
                label_init.both_rect.v_rect.bottom =  CC_SEC_INFO_NAME_LABEL_BOTTOM+ client_both_rect.v_rect.top;

                label_init.both_rect.h_rect.left = CC_SEC_INFO_NAME_LABEL_LEFT_H;
                label_init.both_rect.h_rect.right = CC_SEC_INFO_NAME_LABEL_RIGHT_H;
                label_init.both_rect.h_rect.top = CC_SEC_INFO_NAME_LABEL_TOP_H + client_both_rect.h_rect.top;
                label_init.both_rect.h_rect.bottom =  CC_SEC_INFO_NAME_LABEL_BOTTOM_H + client_both_rect.h_rect.top;
                
                PdaCallCreateLabelByTextPtr(win_id,
                                                                MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID, 
                                                                label_init, 
                                                                &num_str, 
                                                                SONG_FONT_12, 
                                                                CC_TEXT_COLOR);

                //create num info
                label_init.both_rect.v_rect.left = CC_SEC_INFO_NUM_LABEL_LEFT;
                label_init.both_rect.v_rect.right = CC_SEC_INFO_NUM_LABEL_RIGHT;
                label_init.both_rect.v_rect.top = CC_SEC_INFO_NUM_LABEL_TOP + client_both_rect.v_rect.top;
                label_init.both_rect.v_rect.bottom =  CC_SEC_INFO_NUM_LABEL_BOTTOM+ client_both_rect.v_rect.top;

                label_init.both_rect.h_rect.left = CC_SEC_INFO_NUM_LABEL_LEFT_H;
                label_init.both_rect.h_rect.right = CC_SEC_INFO_NUM_LABEL_RIGHT_H;
                label_init.both_rect.h_rect.top = CC_SEC_INFO_NUM_LABEL_TOP_H + client_both_rect.h_rect.top;
                label_init.both_rect.h_rect.bottom =  CC_SEC_INFO_NUM_LABEL_BOTTOM_H + client_both_rect.h_rect.top;
                
                PdaCallCreateLabelByTextId(win_id,
                                                                MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID, 
                                                                label_init, 
                                                                TXT_NULL, 
                                                                SONG_FONT_12, 
                                                                CC_TEXT_COLOR);                
            }
        }
    }
    
    MMK_DestroyControl(MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID);
    
    anim_init.both_rect.v_rect.left = CC_SEC_INFO_ANIM_LEFT;
    anim_init.both_rect.v_rect.right = CC_SEC_INFO_ANIM_RIGHT;
    anim_init.both_rect.v_rect.top = CC_SEC_INFO_ANIM_TOP + client_both_rect.v_rect.top;
    anim_init.both_rect.v_rect.bottom =  CC_SEC_INFO_ANIM_BOTTOM + client_both_rect.v_rect.top;

    anim_init.both_rect.h_rect.left = CC_SEC_INFO_ANIM_LEFT_H;
    anim_init.both_rect.h_rect.right = CC_SEC_INFO_ANIM_RIGHT_H;
    anim_init.both_rect.h_rect.top = CC_SEC_INFO_ANIM_TOP_H + client_both_rect.h_rect.top;
    anim_init.both_rect.h_rect.bottom =  CC_SEC_INFO_ANIM_BOTTOM_H + client_both_rect.h_rect.top;
    
    CreatePhotoAnim(win_id, MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID, &anim_init.both_rect, PHOTO_TYPE_HOLD);
}
/*****************************************************************************/
//  Description :multi line call, display calls in this cases: 
//                                                                                  active + hold call
//                                                                                  MPTY + hold call
//                                                                                  active + MPTY call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaDisplayMultiCall(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array)
{         
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    
    MMK_DestroyControl(MMICC_BOTTOM_PHOTO_ANIM_CTRL_ID);
#endif	
    PdaDisplayMultiActiveCall(win_id, prompt_str_array);
    PdaDisplayCallInfoForMultiHoldCall(win_id, prompt_str_array);
}

/*****************************************************************************/
//  Description :display disconneting win
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PdaOpenDisconnetingWin(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, PROCESSMSG_FUNC handle_func)
{
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    GUISOFTKEY_INIT_DATA_T softkey_init = {0};

    CHECK_CC_APPLET_NOT_NULL
    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    CreateStatusBarCtrl(win_id, IMAGE_NULL);
#endif
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    if(CC_CALL_STATE == MMICC_GetCurrentCallStatus() ||CC_HOLD_STATE == MMICC_GetCurrentCallStatus())
    {
        PdaDisplaySingleCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_OTHER,TRUE,MMI_CALL_OTHER_STATE_TEXT_COLOR);
    }
    else
    {
        PdaDisplaySingleCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_OTHER,FALSE,MMI_CALL_OTHER_STATE_TEXT_COLOR);
    }
#else
    PdaDisplaySingleCallInfoForCommon(win_id, prompt_str_array, PHOTO_TYPE_OTHER);
#endif

 #ifdef MMI_CALLCONTROL_MINI_SUPPORT
    DisplayCallAnimPhotoForCommon(win_id,IMAGE_CC_DISCONNECTED_PHOTO_ANIM);
 #endif
    
 
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
//    CC_CreateHotButtonsWithoutForm(win_id);
#endif
    //create softkey control
    softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
    softkey_create.init_data_ptr     = &softkey_init;
    softkey_create.parent_win_handle = win_id;
 
    MMK_CreateControl(&softkey_create);
    MMK_SetWinSoftkeyCtrlId(win_id,softkey_create.ctrl_id);
    
}
    

/*****************************************************************************/
//  Description :display disconneting win
//  Global resource dependence : none
//  Author:tao.xue
//  Note: modify by louis.wei 2004-01-14
/*****************************************************************************/
LOCAL void PdaDisplayDisconnectWin(
                                   uint8							call_index,
                                   MN_CALL_DISCONNECT_CALL_CAUSE_E	cause,
                                   uint32 call_time
                                   )
{
    MMI_STRING_T			prompt_string[CC_DISPALY_INFO_MAX] = {0};
    wchar                   temp_wchar[CC_DISPLAY_TIME_LEN + 3] = {0};
    //wchar                   temp_wchar1[CC_MAX_PBNAME_LEN + 1] = {0};
    CC_MN_CALL_DISC_CAUSE_E cause_type = CC_DISC_CAUSE_NORMAL;

    uint16			city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH] = {0};

    wchar                      temp_tel_num[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
    uint8			tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			tel_num_len = 0;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function PdaDisplayDisconnectWin():call_index=%d, cause=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_5735_112_2_18_2_10_52_321,(uint8*)"dd", call_index, cause);

    if (MMICC_IsExistActiveCall())//this is a call splited from MPTY
    {
        //SCI_TRACE_LOW:"ccapp disc win not open as active call"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_5739_112_2_18_2_10_52_322,(uint8*)"");
        return ;
    }

    if (CC_INSTANCE.cc_display_type == CC_DISPLAY_MT_LOCK)//s_is_original_phone_lock
    {
        CC_UpdateCcDisplayType(CC_DISPLAY_MT_LOCK);
        
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
        MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
        MMK_CloseWin(MMICC_ADJUSTVOLUME_WIN_ID);
        MMK_CloseWin(MMICC_DISCONNECT_WIN_ID);
        
        //MMIAPIKL_LockPhone();
        return ;
    }


    cause_type = MMICC_ShowCallDiscCause(cause);
    
    //非本方挂断,需要发挂断音提示用户
    if (CC_RELEASE_OPER != MMICC_GetCallOperatedByIndex(call_index)
        && 0 != call_time)
    {
        MMICC_SetIsPlayCallEndHintFlag(TRUE);
    }
    else
    {
        MMICC_SetIsPlayCallEndHintFlag(FALSE);
    }
#ifdef MMI_CLEAR_CODE_SUPPORT
    //merge telcel
    if(cause <= MN_CAUSE_INTERWORKING)
    {
//lzk mark
#ifdef BT_DIALER_SUPPORT
        uint8					disc_str[CC_CALL_CLEAR_CODE_STR_MAX_LEN] = {0};
        wchar                   promp_disc_wchar[CC_CALL_CLEAR_CODE_STR_MAX_LEN + 1] = {0};
        switch(cause)
        {
        case MN_CAUSE_UNASSIGNED_NO://1
        case MN_CAUSE_NUMBER_CHANGED://22
        case MN_CAUSE_INVALID_NO_FORMAT://28
            sprintf((char*)disc_str,"MARCACION NO DISPNOIBLE -%02d-",cause);
            break;
            
        case MN_CAUSE_NO_ROUTE_TO_DEST://3
        case MN_CAUSE_OPER_DETERM_BARRING://8
        case MN_CAUSE_CALL_REJECTED://21
        case MN_CAUSE_DEST_OUT_OF_ORDER://27
            sprintf((char*)disc_str,"USUARIO NO DISPONIBLE -%02d-",cause);
            break;
            
        case MN_CAUSE_NO_USER_RESPONDING://18
        case MN_CAUSE_ALERTING_NO_ANSWER://19
            sprintf((char*)disc_str,"USUARIO NO RESPONDE -%02d-",cause);
            break;
            
        case MN_CAUSE_NORMAL_CLEARING://16
            sprintf((char*)disc_str,"--sin texto--");
            break;
            
        case MN_CAUSE_USER_BUSY://17
            sprintf((char*)disc_str,"USUARIO OCUPADO");
            break;
            
        default://other case
            sprintf((char*)disc_str,"LLAMADA TERMINADA -%02d-",cause);
            break;
        }
        
        prompt_string[CC_DISPALY_INFO_NAME].wstr_ptr = promp_disc_wchar;
        prompt_string[CC_DISPALY_INFO_NAME].wstr_len = strlen((char*)disc_str);
        MMI_STRNTOWSTR(prompt_string[CC_DISPALY_INFO_NAME].wstr_ptr,
            CC_CALL_CLEAR_CODE_STR_MAX_LEN,
            disc_str,
            CC_CALL_CLEAR_CODE_STR_MAX_LEN,
            prompt_string[CC_DISPALY_INFO_NAME].wstr_len);    
#else
     prompt_string[CC_DISPALY_INFO_NAME] = GetClearCodeString(cause);    
#endif    
    }
    else
#endif
    if (CC_CALL_STATE == MMICC_GetCallStatusByIndex( call_index)
        || CC_HOLD_STATE == MMICC_GetCallStatusByIndex( call_index))
    {
        //更新显示
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        prompt_string[CC_DISPALY_INFO_STATE].wstr_ptr = temp_wchar;
        prompt_string[CC_DISPALY_INFO_STATE].wstr_len = sizeof(temp_wchar)/sizeof(wchar);        
        CC_TimeCountToStr(&prompt_string[CC_DISPALY_INFO_STATE], call_time);
#else
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_END,  &prompt_string[CC_DISPALY_INFO_STATE]);
#endif
    }
    else if ((CC_DISC_CAUSE_NORMAL == cause_type)//normal cause
        ||(0 != call_time)  // call time not zero
        ||(CC_RELEASE_OPER == MMICC_GetCallOperatedByIndex(call_index)) //release by user
        ||(CC_MT_CALL_TYPE == MMICC_GetCallDirection(call_index))) //only MO display disconnect cause
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_END,  &prompt_string[CC_DISPALY_INFO_STATE]);
    }
    else if (((cause_type > CC_DISC_CAUSE_NORMAL) && (cause_type <= CC_DISC_CAUSE_BEING_EXCEED_LIMIT)))
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)s_cc_disc_cause[cause_type - 1],  &prompt_string[CC_DISPALY_INFO_STATE]);
    }

#ifdef MMI_CALLCONTROL_MINI_SUPPORT 
    prompt_string[CC_DISPALY_INFO_TIME].wstr_ptr = temp_wchar;
    prompt_string[CC_DISPALY_INFO_TIME].wstr_len = sizeof(temp_wchar)/sizeof(wchar);
    CC_TimeCountToStr(&prompt_string[CC_DISPALY_INFO_TIME], call_time);
#endif
    MMICC_GetCallNameStrByIndex(&prompt_string[CC_DISPALY_INFO_NAME], call_index);

#ifdef BT_DIALER_SUPPORT
    if (g_call_disc_by_bt)
    {
        MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, MMICC_GetLastActiveCallIndex());

        MMICC_SetLastActiveCallIndex();

        g_call_disc_by_bt = FALSE;
    }
    else
    {
#endif

        MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, call_index);

#ifdef BT_DIALER_SUPPORT
    }
#endif

    //bug 186648 modify is_only_for_display(the last parameter of MMIAPICC_IsEmergencyNum) true to false,is_only_for_display is setted true is to some requirement,
    //but in different countries in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num(no sim emergency) ,
    //it is not normal display emergency call in window       
    if (
#ifdef BT_DIALER_SUPPORT
        !MMICC_GetBtCallFlag() &&
#endif
        MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, MMIAPICC_GetCallDualSys(), FALSE))    
    {
        MMI_STRING_T    state_str = {0};
        MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &state_str);

        prompt_string[CC_DISPALY_INFO_NAME].wstr_len = state_str.wstr_len;
        prompt_string[CC_DISPALY_INFO_NAME].wstr_ptr = state_str.wstr_ptr;    
    }
    else
    {    
        //get info for display call number
        //if (MMIAPICOM_IsValidNumberString((char*)tel_num, tel_num_len))
        if (0 != tel_num_len)
        {
            prompt_string[CC_DISPALY_INFO_NUMBER].wstr_ptr = temp_tel_num;
            prompt_string[CC_DISPALY_INFO_NUMBER].wstr_len = tel_num_len;
            MMI_STRNTOWSTR(prompt_string[CC_DISPALY_INFO_NUMBER].wstr_ptr,
                prompt_string[CC_DISPALY_INFO_NUMBER].wstr_len,
                (uint8 *)tel_num,
                tel_num_len,
                tel_num_len);
        }
        else
        {
            //the number wstr_len is 0.display unknown number info
            MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &prompt_string[CC_DISPALY_INFO_NUMBER]);
        }

        CC_FindLocationInfor(&(prompt_string[CC_DISPALY_INFO_LOCATION]), city_ucs2_str, tel_num, tel_num_len);
    }

    //判断disconnect window是否存在
    if ( MMK_IsOpenWin( MMICC_DISCONNECT_WIN_ID ) )
    {
        MMK_CloseWin(MMICC_DISCONNECT_WIN_ID);
        GUIWIN_UpdateStbIcon();
    }
    
    //when usb is running, not display disc win
    if (MMIAPIUDISK_UdiskIsRun()&&MMIAPICC_IsVideoCall())
    {
        return;
    }
    else
    {
        CC_INSTANCE.call_end_index = call_index;
        PdaOpenDisconnetingWin(MMICC_DISCONNECT_WIN_ID, prompt_string, CC_HandleDisconnectWinMsg);
    }
    MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
    
    //@samboo.shen add 20070727 vt电话回落时不能关闭呼出窗口
        MMK_CloseWin( MMICC_ANIMATION_WIN_ID );
    MMK_CloseWin(MMICC_ADJUSTVOLUME_WIN_ID);

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    if( 0 == CC_INSTANCE.mmicc_disconnect_timer_for_display )
    {
        CC_INSTANCE.mmicc_disconnect_timer_for_display = MMK_CreateWinTimer(MMICC_DISCONNECT_WIN_ID, CC_DISCONNECT_TIMER_OUT_FOR_DISPLAY, FALSE);
    }
    else
    {
        MMK_StartWinTimer(
            MMICC_DISCONNECT_WIN_ID,
            CC_INSTANCE.mmicc_disconnect_timer_for_display,
            CC_DISCONNECT_TIMER_OUT_FOR_DISPLAY, FALSE
            );
    }
#endif

    if( 0 == CC_INSTANCE.mmicc_disconnect_timer )
    {
        CC_INSTANCE.mmicc_disconnect_timer = MMK_CreateWinTimer(MMICC_DISCONNECT_WIN_ID, CC_DISCONNECT_TIMER_OUT, FALSE);
    }
    else
    {
        MMK_StartWinTimer(
            MMICC_DISCONNECT_WIN_ID,
            CC_INSTANCE.mmicc_disconnect_timer,
            CC_DISCONNECT_TIMER_OUT, FALSE
            );
    }
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏，显示总共通话时间
    MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_string[CC_DISPALY_INFO_TIME]);
#endif
}

/*****************************************************************************/
//  Description : check if need lock in call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsNeedTurnOffBLQuicklyInCall(void)
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();
    
    if (MMK_IsFocusWin(MMICC_STATUS_WIN_ID)
        && MMIAPICC_IsInState(CC_IN_CALL_STATE)
        && (!MMICC_IsExistOutgoingCall() && MMICC_ANIMATION_WIN_ID != win_id)
        && !GPIO_CheckHeadsetStatus()//未插耳机
        && !MMIAPICC_IsHandFree()//不是免提
        && (!(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev()))//未使用蓝牙耳机通话
        && CC_INSTANCE.cc_display_type == CC_DISPLAY_HOT_BUTTONS
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#else
/*****************************************************************************/
//  Description :just only active call or hold call being, need show photo, state, name& time
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void DisplayCallInfoForSingle(MMI_STRING_T* prompt_str_array, BOOLEAN is_flash)
{
    MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
#ifdef MAINLCD_SIZE_128X64  /* lint_lai */
	int32 index = 0;
#else		   
    GUIFORM_INIT_DATA_T info_form_init = {0};
    MMI_CTRL_ID_T info_form_id = MMICC_CONNECTED_INFO_FORM_CTRL_ID;
    MMI_CONTROL_CREATE_T info_form_create = {0};
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif 
    CC_DISPALY_INFO_E info_index = CC_DISPALY_INFO_STATE;
#if defined(MAINLCD_SIZE_320X480)
    GUIANIM_INIT_DATA_T anim_init = {0};
#else
    GUI_BOTH_RECT_T both_rect = {0};
    MMI_CONTROL_CREATE_T text_child_form_create = {0};
    MMI_CTRL_ID_T text_child_form_id = MMICC_CONNECTED_TEXT_CHILD_FORM_CTRL_ID;
    GUIFORM_INIT_DATA_T text_child_form_init = {0};
#endif
    if (PNULL == prompt_str_array)
    {
        //SCI_TRACE_LOW:"ccapp err DisplayCallInfoForSingle, null ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_5931_112_2_18_2_10_52_323,(uint8*)"");
        return;
    }
#if defined(MAINLCD_SIZE_128X64)  //@ alex added in 2010.11.30
    //MAINLCD_SIZE_128X64
    //create text form
    text_child_form_init.both_rect.v_rect.left = 1;//MMICC_CONNECTED_TEXT_FORM_LEFT;
    text_child_form_init.both_rect.v_rect.top = 1;//MMICC_CONNECTED_TEXT_FORM_TOP;
    text_child_form_init.both_rect.v_rect.right = 126;//client_rect.v_rect.right;
    text_child_form_init.both_rect.v_rect.bottom = 64-14;//MMICC_CONNECTED_TEXT_FORM_BOTTOM;
    
    text_child_form_init.both_rect.h_rect.left = 1;//MMICC_CONNECTED_TEXT_FORM_LEFT_H;
    text_child_form_init.both_rect.h_rect.top = 1;//MMICC_CONNECTED_TEXT_FORM_TOP_H;
    text_child_form_init.both_rect.h_rect.right = 126;//client_rect.h_rect.right;
    text_child_form_init.both_rect.h_rect.bottom = 64-14;//MMICC_CONNECTED_TEXT_FORM_BOTTOM_H;
    text_child_form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    text_child_form_create.ctrl_id = text_child_form_id;
    text_child_form_create.guid = SPRD_GUI_FORM_ID;
    text_child_form_create.init_data_ptr = &text_child_form_init;
    text_child_form_create.parent_win_handle = win_id;
    MMK_CreateControl(&text_child_form_create);

    //not slide
    GUIFORM_IsSlide(text_child_form_id,FALSE);

    {
        uint16 ver_space = 0;
        uint16 hor_space = 0;
        GUIFORM_SetSpace(text_child_form_id, &ver_space, &hor_space);
    }

    //create text label
    text_label_init.align = GUILABEL_ALIGN_MIDDLE;
    text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = FALSE;
    text_form_child_ctrl.init_data_ptr = &text_label_init;

    for(index=0; index<=(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID - MMICC_CONNECTED_STATE_LABEL_CTRL_ID); index++)
    {
        if(prompt_str_array[index].wstr_len > 0 && (PNULL != prompt_str_array[index].wstr_ptr))
        {
            text_form_child_ctrl.child_handle = MMICC_CONNECTED_STATE_LABEL_CTRL_ID + index;
            GUIFORM_CreatDynaChildCtrl(win_id, text_child_form_id,  &text_form_child_ctrl);
            GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[index], FALSE);
            GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, CC_TEXT_COLOR);
        }
    }
#else
    //!!!NOT MAINLCD_SIZE_128X64
    if(0 != MMK_GetCtrlHandleByWin(win_id, info_form_id)) //list ctrl is not being means call state not change, just update label
    {
        if (CC_CALL_STATE == MMICC_GetCurrentCallStatus())
        {
            SetConnectedWinText(is_flash, prompt_str_array);
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏，显示当前通话时间
            MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_str_array[2]);
#endif
        }
        else
        {
            prompt_str_array[CC_DISPALY_INFO_TIME].wstr_len = 0;
            SetConnectedWinText(is_flash, prompt_str_array);
        }
        //刷新状态栏
        if (is_flash)
        {
            GUIWIN_UpdateStbIcon();   
        }        
    }
    else 
    {
        //destroy multi call ctrl
        MMK_DestroyControl(MMICC_MULTI_CALL_FORM_CTRL_ID);

        MMK_DestroyControl(info_form_id);
        //create info form
        info_form_init.both_rect.v_rect.left = client_rect.v_rect.left;
        info_form_init.both_rect.v_rect.top = MMICC_CALLING_TEXT_FORM_TOP;
        info_form_init.both_rect.v_rect.right = client_rect.v_rect.right;
        info_form_init.both_rect.v_rect.bottom = MMICC_CALLING_TEXT_FORM_BOTTOM;

        info_form_init.both_rect.h_rect.left = client_rect.h_rect.left;
        info_form_init.both_rect.h_rect.top = MMICC_CALLING_TEXT_FORM_TOP_H;
        info_form_init.both_rect.h_rect.right = client_rect.h_rect.right;
        info_form_init.both_rect.h_rect.bottom = MMICC_CALLING_TEXT_FORM_BOTTOM_H;
        info_form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

        info_form_create.ctrl_id = info_form_id;
        info_form_create.guid = SPRD_GUI_FORM_ID;
        info_form_create.init_data_ptr = &info_form_init;
        info_form_create.parent_win_handle = win_id;
        MMK_CreateControl(&info_form_create);

//@for light function modify
#if defined MAINLCD_SIZE_240X320 
        {
        GUI_BG_T bg_info = {0};
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_COMMON_BG;

        GUIFORM_SetBg(info_form_id, &bg_info);
        GUIFORM_PermitChildFont(info_form_id, FALSE);
        }
#endif
        //not slide
        GUIFORM_IsSlide(info_form_id,FALSE);

#if defined(MAINLCD_SIZE_320X480)
        //!!!NOT MAINLCD_SIZE_128X64 and MAINLCD_SIZE_320X480
        text_label_init.align = GUILABEL_ALIGN_MIDDLE;
        text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        text_form_child_ctrl.is_bg = FALSE;
        text_form_child_ctrl.is_get_active = FALSE;
        text_form_child_ctrl.init_data_ptr = &text_label_init;

        text_form_child_ctrl.child_handle = MMICC_CONNECTED_STATE_LABEL_CTRL_ID;
        GUIFORM_CreatDynaChildCtrl(win_id, info_form_id,  &text_form_child_ctrl);
        GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[info_index], FALSE);
        GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_NORMAL_FONT, CC_TEXT_COLOR);

        {
            MMI_HANDLE_T anim_handle = MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID;
            GUIANIM_CTRL_INFO_T control_info = {0};
            GUIANIM_DATA_INFO_T data_info = {0};
            GUIANIM_DISPLAY_INFO_T display_info = {0};
            MMIPB_BCD_NUMBER_T pb_num = {0};
            GUIANIM_FILE_INFO_T file_info = {0};
            wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
            uint32 custom_photo_name_len = 0;
            uint32 photo_file_size = 0;
            GUIFORM_CHILD_WIDTH_T anim_child_width = {0};
            GUIFORM_CHILD_HEIGHT_T anim_child_height = {0};

            text_form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
            text_form_child_ctrl.is_bg = FALSE;
            text_form_child_ctrl.is_get_active = FALSE;
            text_form_child_ctrl.init_data_ptr = &anim_init;
            text_form_child_ctrl.child_handle = anim_handle;
            GUIFORM_CreatDynaChildCtrl(win_id, info_form_id,  &text_form_child_ctrl);
            anim_child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            anim_child_height.add_data = MMICC_CALLING_PHOTO_ANIM_HEIGHT;
            anim_child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            anim_child_width.add_data = MMICC_CALLING_PHOTO_ANIM_WIDTH;
            GUIFORM_SetChildHeight(info_form_id, anim_handle, &anim_child_height);
            GUIFORM_SetChildWidth(info_form_id, anim_handle, &anim_child_width);
            GUIFORM_SetChildAlign(info_form_id, anim_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);

            control_info.is_ctrl_id = TRUE;
            control_info.ctrl_id = anim_handle;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

            MMICC_GetCurrentCallNumber(&pb_num, TRUE, FALSE);

            if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
            {
                GUIANIM_SetDefaultIcon(anim_handle,IMAGE_COMMON_PIC_ICON,IMAGE_COMMON_PIC_ICON);
                file_info.full_path_wstr_ptr = custom_photo_name;
                file_info.full_path_wstr_len = custom_photo_name_len;
                display_info.bg.bg_type = GUI_BG_COLOR;
                display_info.bg.color   = MMI_BACKGROUND_COLOR;
                GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);
            }
            else
            {
                data_info.img_id = IAMGE_CALL_DEFAULT_PHOTO;
                GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            }
        
        }

        text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        text_form_child_ctrl.is_bg = FALSE;
        text_form_child_ctrl.is_get_active = FALSE;
        text_form_child_ctrl.init_data_ptr = &text_label_init;

        for(info_index=CC_DISPALY_INFO_NAME; info_index < CC_DISPALY_INFO_MAX; info_index++)
        {
            if(prompt_str_array[info_index].wstr_len > 0 && (PNULL != prompt_str_array[info_index].wstr_ptr))
            {
                text_form_child_ctrl.child_handle = MMICC_CONNECTED_STATE_LABEL_CTRL_ID + info_index;
                GUIFORM_CreatDynaChildCtrl(win_id, info_form_id,  &text_form_child_ctrl);
                GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[info_index], FALSE);
                GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_NORMAL_FONT, CC_TEXT_COLOR);
            }
        }

        {
            uint16 ver_space = 0;
            uint16 hor_space = 0;
            GUIFORM_SetSpace(info_form_id, &ver_space, &hor_space);
        }
#else //MAINLCD_SIZE_320X480
        //!!!NOT MAINLCD_SIZE_128X64 and !!!NOT MAINLCD_SIZE_320X480
        //create photo anim
        both_rect.v_rect.left = MMICC_CONNECTED_PHOTO_ANIM_LEFT;
        both_rect.v_rect.right = MMICC_CONNECTED_PHOTO_ANIM_RIGHT;
        both_rect.v_rect.top = MMICC_CONNECTED_PHOTO_ANIM_TOP;
        both_rect.v_rect.bottom = MMICC_CONNECTED_PHOTO_ANIM_BOTTOM;

        both_rect.h_rect.left = MMICC_CONNECTED_PHOTO_ANIM_LEFT_H;
        both_rect.h_rect.right = MMICC_CONNECTED_PHOTO_ANIM_RIGHT_H;
        both_rect.h_rect.top = MMICC_CONNECTED_PHOTO_ANIM_TOP_H;
        both_rect.h_rect.bottom = MMICC_CONNECTED_PHOTO_ANIM_BOTTOM_H;

        CreatePhotoAnim(win_id, MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID, &both_rect, PHOTO_TYPE_CURRENT);

        //create text form
        text_child_form_init.both_rect.v_rect.left = MMICC_CONNECTED_TEXT_FORM_LEFT;
#ifdef MAINLCD_SIZE_128X64
        text_child_form_init.both_rect.v_rect.top = /*MMICC_CONNECTED_TEXT_FORM_TOP*/0;
#else    
        text_child_form_init.both_rect.v_rect.top = MMICC_CONNECTED_TEXT_FORM_TOP;
#endif    
        text_child_form_init.both_rect.v_rect.right = client_rect.v_rect.right;
        text_child_form_init.both_rect.v_rect.bottom = MMICC_CONNECTED_TEXT_FORM_BOTTOM;

        text_child_form_init.both_rect.h_rect.left = MMICC_CONNECTED_TEXT_FORM_LEFT_H;
#ifdef MAINLCD_SIZE_128X64
        text_child_form_init.both_rect.h_rect.top = /*MMICC_CONNECTED_TEXT_FORM_TOP_H*/0;
#else    
        text_child_form_init.both_rect.h_rect.top = MMICC_CONNECTED_TEXT_FORM_TOP_H;
#endif    
        text_child_form_init.both_rect.h_rect.right = client_rect.h_rect.right;
        text_child_form_init.both_rect.h_rect.bottom = MMICC_CONNECTED_TEXT_FORM_BOTTOM_H;
        text_child_form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

        text_child_form_create.ctrl_id = text_child_form_id;
        text_child_form_create.guid = SPRD_GUI_FORM_ID;
        text_child_form_create.init_data_ptr = &text_child_form_init;
        text_child_form_create.parent_win_handle = win_id;
        MMK_CreateControl(&text_child_form_create);

        //not slide
        GUIFORM_IsSlide(text_child_form_id,FALSE);

        {
            uint16 ver_space = 0;
            uint16 hor_space = 0;
            GUIFORM_SetSpace(text_child_form_id, &ver_space, &hor_space);
        }

#if defined MAINLCD_SIZE_240X320 
        {
        GUI_BG_T bg_info = {0};
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_COMMON_BG;

        GUIFORM_SetBg(text_child_form_id, &bg_info);
        GUIFORM_PermitChildFont(text_child_form_id, FALSE);
        }
#endif
        

        //create text label
        text_label_init.align = GUILABEL_ALIGN_LEFT;
        text_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        text_form_child_ctrl.is_bg = FALSE;
        text_form_child_ctrl.is_get_active = FALSE;
        text_form_child_ctrl.init_data_ptr = &text_label_init;

        for(info_index=0; info_index<=((int32)MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID - MMICC_CONNECTED_STATE_LABEL_CTRL_ID); info_index++)
        {
            if(prompt_str_array[info_index].wstr_len > 0 && (PNULL != prompt_str_array[info_index].wstr_ptr))
            {
                text_form_child_ctrl.child_handle = MMICC_CONNECTED_STATE_LABEL_CTRL_ID + info_index;
                GUIFORM_CreatDynaChildCtrl(win_id, text_child_form_id,  &text_form_child_ctrl);
                GUILABEL_SetText(text_form_child_ctrl.child_handle, &prompt_str_array[info_index], FALSE);
#if defined(MAINLCD_SIZE_176X220) || defined(MAINLCD_SIZE_128X160)
                GUILABEL_SetFont(text_form_child_ctrl.child_handle, SONG_FONT_14, CC_TEXT_COLOR);
#else
                GUILABEL_SetFont(text_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, CC_TEXT_COLOR);
#endif
            }
        }
#endif //MAINLCD_SIZE_320X480 

    }

#endif // MAINLCD_SIZE_128X64 
}

/*****************************************************************************/
//  Description :when active call& hold call all being,display info include call state, name
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void DisplayCallInfoForMulti(void)
{
#if defined(MAINLCD_SIZE_128X64)

#else
    MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;
    MMI_CTRL_ID_T parent_form_id = MMICC_MULTI_CALL_FORM_CTRL_ID;
    MMI_CONTROL_CREATE_T form_ctrl = {0};        
    GUIFORM_INIT_DATA_T form_init = {0};
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};
    MMI_HANDLE_T child_form_handle = 0;
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    uint32 i = 0;

    // multi form ctrl is being means call state not change, just update list
    if (0 != MMK_GetCtrlHandleByWin(win_id, MMICC_MULTI_CALL_FORM_CTRL_ID))
    {
        
        SetMultiCallInfo(FALSE);
    }
    else
    {
        //destroy single info display ctrl
        MMI_CTRL_ID_T ctrl_id = 0;
        for (ctrl_id = MMICC_CONNECTED_INFO_FORM_CTRL_ID; ctrl_id <= MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID; ctrl_id++)
        {
            MMK_DestroyControl(ctrl_id);
        }
        MMK_DestroyControl(MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID);
        MMK_DestroyControl(MMICC_CONNECTED_INFO_FORM_CTRL_ID);
        MMK_DestroyControl(parent_form_id);

        
        //create list
        form_init.both_rect.v_rect.left = MMICC_MULTI_FORM_MARGIN_X;
        form_init.both_rect.v_rect.right = client_both_rect.v_rect.right - MMICC_MULTI_FORM_MARGIN_X;
        form_init.both_rect.v_rect.top = MMI_STATUSBAR_HEIGHT + MMICC_MULTI_FORM_MARGIN_Y;
        form_init.both_rect.v_rect.bottom = MMICC_CONNECTED_BUTTON_FORM_TOP - MMICC_MULTI_FORM_MARGIN_Y;

        form_init.both_rect.h_rect.left = MMICC_MULTI_FORM_MARGIN_X_H;
        form_init.both_rect.h_rect.right = client_both_rect.h_rect.right - MMICC_MULTI_FORM_MARGIN_X_H;
        form_init.both_rect.h_rect.top = MMI_STATUSBAR_HEIGHT + MMICC_MULTI_FORM_MARGIN_Y_H;
        form_init.both_rect.h_rect.bottom = MMICC_CONNECTED_BUTTON_FORM_TOP_H - MMICC_MULTI_FORM_MARGIN_Y_H;

        form_init.layout_type = GUIFORM_LAYOUT_ORDER;
        
        form_ctrl.ctrl_id = parent_form_id;
        form_ctrl.guid = SPRD_GUI_FORM_ID;
        form_ctrl.init_data_ptr = &form_init;
        form_ctrl.parent_win_handle = win_id;
        MMK_CreateControl(&form_ctrl);     

        for(i=0; i < CC_MULTI_CALL_MAX; i++)
        {
            MMI_HANDLE_T child_handle = 0;
            GUIANIM_INIT_DATA_T anim_init = {0};
            GUILABEL_INIT_DATA_T label_init = {0};
            GUIFORM_CHILD_WIDTH_T child_anim_width = {0};
            
            //create child form
            child_form_handle = s_multi_call_display_ctrl[i][CHILD_FORM_ID];
            form_init.layout_type = GUIFORM_LAYOUT_SBS;
            
            form_child_ctrl.child_handle = child_form_handle;
            form_child_ctrl.guid = SPRD_GUI_FORM_ID;
            form_child_ctrl.init_data_ptr = &form_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, parent_form_id,  &form_child_ctrl);

            //create anim handle in child form
            anim_init.is_form_bg = FALSE;
            child_handle = s_multi_call_display_ctrl[i][PHOTO_ANIM_ID];
            form_child_ctrl.child_handle = child_handle;
            form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
            form_child_ctrl.init_data_ptr = &anim_init; /*lint !e733*/
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, child_form_handle,  &form_child_ctrl);  

            //create child child form handle in child form
            form_init.layout_type = GUIFORM_LAYOUT_ORDER;
            child_handle = s_multi_call_display_ctrl[i][CHILD_CHILD_FORM_ID];
            form_child_ctrl.child_handle = child_handle;
            form_child_ctrl.guid = SPRD_GUI_FORM_ID;
            form_child_ctrl.init_data_ptr = &form_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, child_form_handle,  &form_child_ctrl);             
            
            child_form_handle = s_multi_call_display_ctrl[i][CHILD_CHILD_FORM_ID];
            
            //create name label handle in child child form
            label_init.align = GUILABEL_ALIGN_LEFT;
            child_handle = s_multi_call_display_ctrl[i][NAME_LABEL_ID];
            form_child_ctrl.child_handle = child_handle;
            form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
            form_child_ctrl.init_data_ptr = &label_init; /*lint !e733*/
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, child_form_handle,  &form_child_ctrl);       
            
            //create time label handle in child child form
            label_init.align = GUILABEL_ALIGN_LEFT;
            child_handle = s_multi_call_display_ctrl[i][TIME_LABEL_ID];
            form_child_ctrl.child_handle = child_handle;
            form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
            form_child_ctrl.init_data_ptr = &label_init; /*lint !e733*/
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, child_form_handle,  &form_child_ctrl);    

            //set param
            child_anim_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            child_anim_width.add_data = (uint16)MMICC_MULTI_CALL_PHOTO_WIDTH;
            GUIFORM_SetChildWidth(s_multi_call_display_ctrl[i][CHILD_FORM_ID], s_multi_call_display_ctrl[i][PHOTO_ANIM_ID], &child_anim_width);
        }
        
        SetMultiCallInfo(FALSE);
    }

#endif //MAINLCD_SIZE_128X64
}

/*****************************************************************************/
//  Description :clean MMICC_CONNECTED_INFO_LIST_CTRL_ID items and append
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetMultiCallInfo(BOOLEAN is_flash)
{
    MMI_STRING_T call_name_str = {0};
    MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;
    GUILIST_ITEM_DATA_T item_data = {0};
    GUILIST_ITEM_T item_t = {0};
    uint8 call_id = 0;
    MMI_STRING_T time_str = {0};
    wchar temp_wchar[CC_DISPLAY_TIME_LEN + 3] = {0};
    uint32 cur_time_count = 0;
    GUI_FONT_T font = MMI_DEFAULT_SMALL_FONT;
    uint16 v_space = MMICC_MULTI_CALL_FORM_SPACE;

    time_str.wstr_ptr = temp_wchar;
    time_str.wstr_len = sizeof(temp_wchar)/sizeof(wchar);
    item_t.item_data_ptr = &item_data;

    if(MMK_IsWindowLandscape(win_id))
    {
        font = MMI_DEFAULT_SMALL_FONT;
    }
    else
    {
        font = MMI_DEFAULT_NORMAL_FONT;
    }
    GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][NAME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);
    GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][TIME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);
    GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][NAME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);
    GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][TIME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);

    GUIFORM_SetSpace(MMICC_MULTI_CALL_FORM_CTRL_ID, PNULL, &v_space);

    //set anim param
    CreatePhotoAnim(win_id, s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][PHOTO_ANIM_ID], PNULL, PHOTO_TYPE_ACTIVE);
    CreatePhotoAnim(win_id, s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][PHOTO_ANIM_ID], PNULL, PHOTO_TYPE_HOLD);
    //get call name, photo, call duration
    if(MMICC_IsExistActiveMPTY())
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_MULTICALL, &call_name_str);
    }
    else if(1 == MMIAPICC_GetActiveCallCount())
    {
        call_id = MMICC_FindStateCall(CC_CALL_STATE);
        MMICC_GetCallNameStrByIndex(&call_name_str, call_id);
    }
    else
    {
        //do nothing
    }

    cur_time_count = MMICC_GetCurrentCallTime();
    CC_TimeCountToStr(&time_str, cur_time_count);
    
    GUILABEL_SetText(s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][NAME_LABEL_ID], &call_name_str, is_flash);
    GUILABEL_SetText(s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][TIME_LABEL_ID], &time_str, is_flash);

    //set hold call item
    SCI_MEMSET(&call_name_str, 0, sizeof(call_name_str));
    SCI_MEMSET(&temp_wchar, 0, sizeof(temp_wchar));
    
    if(MMICC_IsExistHoldMPTY())
    {
        call_id = MMICC_FindStateCall(CC_HOLD_STATE);
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_MULTICALL, &call_name_str);
    }
    else if(MMICC_IsExistHoldCall())
    {
        call_id = MMICC_FindStateCall(CC_HOLD_STATE);
        MMICC_GetCallNameStrByIndex(&call_name_str, call_id);    }
    else
    {
        //do nothing
    }

    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_HOLD, &time_str);

    GUILABEL_SetText(s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][NAME_LABEL_ID], &call_name_str, is_flash);
    GUILABEL_SetText(s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][TIME_LABEL_ID], &time_str, is_flash);

}

/*****************************************************************************/
//  Description :to set connected win text
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetConnectedWinText(BOOLEAN is_update, MMI_STRING_T* prompt_str_array)
{
   CC_DISPALY_INFO_E info_index = CC_DISPALY_INFO_STATE;
   MMI_CTRL_ID_T label_ctrl_id = MMICC_CONNECTED_STATE_LABEL_CTRL_ID;

   for (info_index=CC_DISPALY_INFO_STATE; info_index < CC_DISPALY_INFO_MAX; info_index++)
   {
       if (0 < prompt_str_array[info_index].wstr_len && PNULL != prompt_str_array[info_index].wstr_ptr)
       {
           label_ctrl_id = MMICC_CONNECTED_STATE_LABEL_CTRL_ID + info_index;
           GUILABEL_SetText(label_ctrl_id, &prompt_str_array[info_index], is_update);
       }
       else
       {
           GUILABEL_SetTextById(label_ctrl_id+1, TXT_NULL, is_update);
       }
   }
}

#endif
/*****************************************************************************/
// 	Description : process when timer is out
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallTimeDisplay()
{
    MMI_STRING_T	prompt_str = {0};
    wchar temp_wchar[CC_DISPLAY_TIME_LEN + 3]={0};
    MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;
    uint32 cur_time_count = MMICC_GetCurrentCallTime();
    
    prompt_str.wstr_len = sizeof(temp_wchar)/sizeof(wchar);
    prompt_str.wstr_ptr = temp_wchar;
    if (MMK_IsFocusWin(win_id))
    {
        CC_TimeCountToStr(&prompt_str, cur_time_count);
        //刷新主屏和副屏
        if (0 != MMK_GetCtrlHandleByWin(win_id, MMICC_MULTI_CALL_FORM_CTRL_ID))
        {
            //fresh list time
            GUILABEL_SetText(MMICC_MULTI_ACTIVE_CALL_TIME_LABEL_CTRL_ID, &prompt_str, TRUE);   
        }
        else
        {
            //fresh label time
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &prompt_str, TRUE);   
#else
	    GUILABEL_SetText(MMICC_CONNECTED_TIME_LABEL_CTRL_ID, &prompt_str, TRUE);
#endif
#else
            GUILABEL_SetText(MMICC_CONNECTED_TIME_LABEL_CTRL_ID, &prompt_str, TRUE);   
#endif
        }
    }
    else
    {
        //只刷新副屏
        CC_TimeCountToStr(&prompt_str, cur_time_count);
        
        {
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏，显示通话时间
            MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_str);
#endif
        }
    }
}


/*****************************************************************************/
// 	Description : convert time count to str
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void CC_TimeCountToStr(MMI_STRING_T* time_str_ptr, uint32 time_count)
{
    uint8 time_str_arr[CC_DISPLAY_TIME_LEN + 2] = {0};
    if (PNULL != time_str_ptr && PNULL != time_str_ptr->wstr_ptr)
    {
        sprintf((char*)time_str_arr,
            "%02d:%02d:%02d",
            (int)(time_count / CC_HOUR_TO_SECOND),
            (int)((time_count % CC_HOUR_TO_SECOND) / CC_MINUTE_TO_SECOND),
            (int)(time_count % CC_MINUTE_TO_SECOND)
            );
        
        time_str_ptr->wstr_len = MIN(time_str_ptr->wstr_len, strlen((char*)time_str_arr));  /*lint !e666 */ 
        //prompt_str.wstr_ptr = time_str_arr;
        MMI_STRNTOWSTR(time_str_ptr->wstr_ptr,
            time_str_ptr->wstr_len,
            (uint8 *)time_str_arr,
            time_str_ptr->wstr_len,
            time_str_ptr->wstr_len);
    }
}

/*****************************************************************************/
//  Description :get the text of call state
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  GetCallStateText(
                                MMI_STRING_T		*state_ptr
                                )
{
    CC_OPERATED_BY_USER_E  operated = MMICC_GetCallOperatedByIndex(MMICC_GetCurrentCallIndex());
    BOOLEAN			    result = TRUE;
    
    if(PNULL == state_ptr)
    {
        //SCI_TRACE_LOW:"GetCallStateText state_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_6510_112_2_18_2_10_53_324,(uint8*)"");
        return FALSE;
    }
        
    if( CC_NO_OPER != operated )
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_CC_HOLDING + (uint16)operated - 1),state_ptr);
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏显示，显示call状态
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,state_ptr);
#endif
    }
    else
    {
        if( CC_CALL_STATE ==MMICC_GetCurrentCallStatus())
        {

            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_COMMUN, state_ptr);
#ifndef SUBLCD_SIZE_NONE           
            //刷新小屏显示，显示call状态
            MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,state_ptr);            
#endif
            
        }
        else if( CC_HOLD_STATE == MMICC_GetCurrentCallStatus() )
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_HOLD, state_ptr);
#ifndef SUBLCD_SIZE_NONE           
            //刷新小屏显示，显示call状态
            MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,state_ptr);
#endif
        }
        else if( MMICC_IsExistIncommingCall())
        {
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_INCOMING, state_ptr );
#else
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_MTCALLING, state_ptr );
#endif
            CC_OpenMtCallWin();
            result = FALSE;
        }
        else
        {
            if( MMICC_IsExistOutgoingCall() )
            {
                CC_OpenMoCallWin();
            }
            result = FALSE;
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : stop ring at incoming call
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_StopMtCallPlayingRing(void)
{
    MMI_WIN_ID_T win_id = MMICC_ANIMATION_WIN_ID;

    if (!MMIAPICC_IsMtCallPlayingRing())
    {
        return FALSE;
    }

    MMIAPICC_StopRingOrVibrate();
#ifdef TTS_SUPPORT
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_CALL);
#endif

    if (MMK_IsFocusWin(win_id))
    {
        CC_HandleAnimWinSoftkey(win_id, TRUE);
    }
#ifndef MMI_GUI_STYLE_TYPICAL//@tao.xue
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
    CC_CreateNewHotButtonWithoutForm(win_id, CC_HOT_BUTTON_ROW_MAX - 1, 1, CC_BUTTON_MUTE);
#endif
#endif

    return TRUE;
}
/*****************************************************************************/
// 	Description : to process of animation window in the cc
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  CC_HandleCcAnimWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E				recode = MMI_RESULT_TRUE;
#ifdef FLIP_PHONE_SUPPORT    
    BOOLEAN						is_flip_answer = FALSE;
#endif
    GUI_LCD_DEV_INFO	        lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0}; 
    LOCAL BOOLEAN               need_playring = FALSE;
    LOCAL uint32                s_waiting_call_ring_play_times = 0;
#ifdef BT_DIALER_SUPPORT
    BT_STATUS bt_status = BT_ERROR;
#endif
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    //check the param
    if (MSG_TIMER != msg_id)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleCcAnimWinMsg() msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_6606_112_2_18_2_10_54_325,(uint8*)"d", msg_id);
    }
    
#ifdef DPHONE_SUPPORT
    if(MSG_KEYDOWN_HANDSFREE == msg_id)
    {
        if(MMIDEFAULT_GetHandFreeStates())
        {
            msg_id = MSG_APP_OK;
        }
    }
    else if(MSG_KEYUP_HOOK == msg_id)
    {
        msg_id = MSG_APP_OK;
    }
#endif

    //CC界面，全键盘工程，默认需对共用的数字按键进行消息转换。
    if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
        || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION))
    {
        msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
    }
    
#if defined(MMI_GUI_STYLE_TYPICAL)||(!defined(MMI_GUI_STYLE_TYPICAL)&&!defined(TOUCH_PANEL_SUPPORT))
    //send dtmf when press key in call state
    if(!CC_IsAnyKeyAnswerThisCall(msg_id)
        && ((msg_id >= MSG_KEYDOWN_1 && msg_id <= MSG_KEYDOWN_HASH) 
        || (msg_id >= MSG_APP_1 && msg_id <= MSG_APP_HASH) 
#ifdef TOUCH_PANEL_SUPPORT
        || (MSG_APP_WEB == msg_id) 
#endif
        )
        )
    {
        if (MMICC_IsAllowSendDtmf()) 
        {
            MMICC_SendSingleDtmf(msg_id);
            CC_OpenDialingWin(msg_id); 
        }
        else if(MMICC_IsAllowAddNewCall()
#if defined(MMI_ECT_MENU_SUPPORT) && defined(MMI_CRSS_SUPPORT)
                    ||(  (1 == MMIAPICC_GetHoldCallCount())
                        && (CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
                        && ((MSG_KEYDOWN_4 == msg_id)||(MSG_APP_4 == msg_id))   
                        )
#endif            
            )
        {
            CC_OpenDialingWin(msg_id); 
        }
    }
#endif
    if (MSG_KEYDOWN_HEADSET_BUTTON == msg_id)      //耳机button按下，对应ok或cancel操作        
    {
#if 0    
        if (GPIO_CheckHeadsetStatus() && MMI_NONE_TIMER_ID == g_mmi_headset_time_id)  //确认耳机是插入的，并且不在耳机定时器保护区间内
        {
            if (CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
                
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                msg_id = MSG_APP_GREEN;
#else
                msg_id = MSG_APP_OK;
#endif
            }
            else if (CC_WAITING_STATE == MMICC_GetCurrentCallStatus())
            {
                //fix for CR82049,button按下保持并应答
                msg_id = MSG_CC_ACCEPT_WAIT_CALL_MSG;								
            }
            else
            {
                msg_id = MSG_APP_RED;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ccapp.c CC_HandleCcAnimWinMsg() headset is plugout!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_6678_112_2_18_2_10_54_326,(uint8*)"");
        }
#endif        
    }
    else if (CC_IsAnyKeyAnswerThisCall(msg_id))//any key to accept call, beside RIGHT_SOFT_KEY and POWER_KEY
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        msg_id = MSG_APP_GREEN;
#else
        msg_id = MSG_APP_OK;
#endif
    }

    SCI_TRACE_LOW("[MMICC] CC_HandleCcAnimWinMsg msg_id = 0x%x", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

#ifndef MMI_GUI_STYLE_TYPICAL
        //MMK_SetWinBackgroundImageId(win_id, IMAGE_COMMON_BG);
#endif
        MMK_OpenGsensor( win_id );

        if (MMICC_GetCallNum() <= 1)//first call
        {
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                CC_INSTANCE.is_original_phone_lock = TRUE;
                //MMIAPIKL_UnlockPhone();//MS235544 unlock the phone if entry cc window
            }
            else
#endif
            {
                CC_INSTANCE.is_original_phone_lock = FALSE;
            }
        }
        
        if (MMICC_IsExistIncommingCall())
        {
            //显示来电界面允许播放铃声，cr116652
            need_playring = TRUE;
            
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
#ifdef ASP_SUPPORT
            if(MMIAPIASP_IsMachineTurnOn())
            {
                MMIAPIASP_CreateWaitTimer();
            }
#endif   
}      
            //not need operate handfree
#ifndef MMI_CALLCONTROL_MINI_SUPPORT            
            CC_SetHotButtonGrayState(CC_BUTTON_HANDFREE, TRUE);
#endif
        }

        //need not display custom photo,should play mt call ring
        if (MMICC_IsExistIncommingCall() && need_playring )
        {
            //显示来电界面允许播放铃声，cr116652
            need_playring = FALSE;
            PlayNewCallRing(MMIAPICC_GetCallDualSys());
            s_waiting_call_ring_play_times ++;
        }
        
        //状态栏显示通话状态图标 
        MAIN_SetCallingState(TRUE);

#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(TOUCH_PANEL_SUPPORT) && (!defined MMI_CALLCONTROL_MINI_SUPPORT)
        //display incoming call lock screen when phone is locked
        if (MMICC_IsExistIncommingCall() && MMICC_GetCallNum() == 1 && CC_INSTANCE.is_original_phone_lock)
        {
            CC_INSTANCE.cc_display_type = CC_DISPLAY_MT_LOCK;
            GUIWIN_SetSoftkeyVisible(win_id,FALSE);
            CC_HandleCcLockMsg(win_id, msg_id, param);
        }
        else
#endif            
        {
            CC_INSTANCE.cc_display_type = CC_DISPLAY_HOT_BUTTONS;
            GUIWIN_SetSoftkeyVisible(win_id,TRUE);
        }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT        
        CC_CreateHotButtonsWithoutForm(win_id);
#endif
        CC_CreateDtmfPanel(win_id);
        
        CC_INSTANCE.is_need_light_on_cc_win = TRUE;
        
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_HOT_BUTTONS) 
#endif
#endif
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        {
            BT_ADDRESS address = {0};

            CC_UpdateHotButtonState();
            
            if (MMICC_IsExistIncommingCall()
                && 1 == MMICC_GetCallNum()
                && MMIAPIBT_GetActivatedHS(&address))
            {
                //not need operate handfree
                CC_SetHotButtonGrayState(CC_BUTTON_HANDFREE, TRUE);
            }
        }
#endif

#ifdef CMCC_UI_STYLE
        s_is_cancel_reject = FALSE;
#endif
#ifdef BT_DIALER_SUPPORT
        if (CC_INSTANCE.is_need_light_on_cc_win)
        {
            CC_INSTANCE.is_need_light_on_cc_win = FALSE;
            MMIDEFAULT_TurnOnBackLight();
        }
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#endif

        break;

    case MSG_FULL_PAINT:
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
        DisplayWinBg(win_id, FALSE);        
#endif
        CC_HandleAnimWinSoftkey(win_id, FALSE);
        //自动接听功能，工程模式里面需要进行设置
        CC_AutoAnswerIncomingCall();

#ifndef MMI_GUI_STYLE_TYPICAL
        if (CC_INSTANCE.is_need_light_on_cc_win)
        {
            CC_INSTANCE.is_need_light_on_cc_win = FALSE;
            MMIDEFAULT_TurnOnBackLight();
        }
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        DisplayWinBg(win_id, FALSE);        
#if defined(TOUCH_PANEL_SUPPORT) && (!defined MMI_CALLCONTROL_MINI_SUPPORT)
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_MT_LOCK)
        {                               
            CC_DisplayMtLock(win_id, CC_MT_LOCK_DISPLAY_INIT, PNULL);
        }
        else
#endif            
        {
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
            CC_UpdateCCWinHotButton(CC_BUTTON_MAX, FALSE);
#endif
            GUIWIN_SetSoftkeyVisible(win_id,TRUE);
        }
#else
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_UpdateCCWinHotButton(CC_BUTTON_MAX, FALSE);
#endif
#endif
        break;

    case MSG_NOTIFY_UPDATE:
        {
            GUI_RECT_T* update_rect_ptr = (GUI_RECT_T*)param;
#ifndef MMI_GUI_STYLE_TYPICAL
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_THEME_BG,
                &lcd_dev_info);
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_CC_COMMON_BG_PDA,
                &lcd_dev_info);
#else
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
#endif
        }
        break;

    case MSG_TIMER:
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
#ifdef ASP_SUPPORT
        if(MMIAPIASP_IsWaitTimeout(param))
        {
            MMIAPIASP_StopWaitTimer();
            if(!MMICC_AnswerASPCall())
            {
                //SCI_TRACE_LOW:"MMIASP WAIT TIME OUT, asp answer fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_6846_112_2_18_2_10_54_327,(uint8*)"");
            }
        }
        else 
#endif
        if (CC_INSTANCE.waiting_call_ring_timer == *(uint8*)param)
        {
            if (s_waiting_call_ring_play_times <= CC_WAITING_CALL_PLAY_TIMER)
            {
                MMIAPISET_PlayRing(MMIAPICC_GetCallDualSys(), FALSE,0,1,MMISET_RING_TYPE_CALL_IN_CALL,PNULL);
                MMK_StartWinTimer( win_id, CC_INSTANCE.waiting_call_ring_timer, CC_WAITING_CALL_PERIOD, FALSE);
                s_waiting_call_ring_play_times ++;
            }
            else
            {
                MMK_StopTimer(CC_INSTANCE.waiting_call_ring_timer);
                CC_INSTANCE.waiting_call_ring_timer = 0;
                s_waiting_call_ring_play_times = 0;
            }
        }
#ifndef MMI_GUI_STYLE_TYPICAL
#if defined(TOUCH_PANEL_SUPPORT) && (!defined MMI_CALLCONTROL_MINI_SUPPORT)
        else if ((CC_INSTANCE.mmicc_move_unlock_icon_timer == *(uint8*)param)
            ||(CC_INSTANCE.mmicc_anim_unlock_icon_timer == *(uint8*)param))
        {
            CC_HandleCcLockMsg(win_id, msg_id, param);            
        }
#endif        
#endif
}
        break;

#if defined(TOUCH_PANEL_SUPPORT)        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    //case MSG_APP_UPSIDE:
    //case MSG_APP_DOWNSIDE: 
    //case MSG_APP_VOL_UP:
    //case MSG_APP_VOL_DOWN:  
        //拨出情况调解音量        
        if (CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
        { 
            OpenVolumePanel(win_id, msg_id);
        }
        else if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            &&(MMIDEFAULT_GetFlipStatus()))
        {
            MMIAPICC_StopMtCallPlayingRing();
        }
        else 
        {
            recode = MMI_RESULT_FALSE;
        }
        break; 
#endif

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
#if !defined(TOUCH_PANEL_SUPPORT)
    case MSG_APP_UP:
    case MSG_APP_DOWN:
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        if (!MMK_IsOpenWin(MMICOMMON_PANEL_WIN_ID))
        {
            CC_UpdateCCWinHotButton(CC_INSTANCE.cc_win_focus_button_type, FALSE);
            CC_INSTANCE.cc_win_focus_button_type = CC_FindNextHotButton(CC_INSTANCE.cc_win_focus_button_type, msg_id);
            CC_UpdateCCWinHotButton(CC_INSTANCE.cc_win_focus_button_type, TRUE);        
        }
        else
#endif
        {
            if(MSG_APP_UP == msg_id || MSG_APP_DOWN == msg_id)
            {
                OpenVolumePanel(win_id, msg_id);
            }
        }
#else
        //拨出情况调解音量        
        if (CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
        { 
#if 0        
            if(MSG_APP_UP == msg_id || MSG_APP_DOWN == msg_id)
            {
                OpenVolumePanel(win_id, msg_id);
            }
#endif			
        }
        else if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            &&(MMIDEFAULT_GetFlipStatus()))
        {
            MMIAPICC_StopMtCallPlayingRing();
        }
        else 
        {
            recode = MMI_RESULT_FALSE;
        }
#endif
        break; 

#ifndef PRINTSCREEN_SUPPORT
    case MSG_APP_CAMERA:
        if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            &&(MMIDEFAULT_GetFlipStatus()))
        {
            MMIAPICC_StopMtCallPlayingRing();
        }
        else 
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
#endif

    case MSG_CC_ACCEPT_WAIT_CALL_MSG:
        MMIAPICC_StopRingOrVibrate();
        
        if(!MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall() )
        {
            //can not do other handle in this state
            return recode;
        }
        if( 1 != MMICC_GetCallNum() )
        {
            //对waiting call进行保持并应答
            MMICC_HoldAndAnswerCW();
        }
        break;

    case MSG_BT_ACCEPT_INCOMING_CALL: //blue tooth headset accept the incoming call
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4: 
    case MSG_APP_OK://user press OK
	case MSG_CTL_OK:
#ifdef DPHONE_SUPPORT
    if (!MMIDEFAULT_GetHookStates())
    {
    	MMIDEFAULT_SetHandFreeStatesAndLed(TRUE);
    }
#endif

#ifdef LCD_SLIDE_SUPPORT   
    case MSG_KEYUP_SLIDE:     //打开滑盖
        if (MSG_KEYUP_SLIDE == msg_id) 
        {
            /*注:此时只能调用MMIKL_UnlockPhone来锁键盘!不能使用MMIDEFAULT_HandleSlideKey(MSG_KEYUP_SLIDE)，
            因为它会调用MMIAPISET_PlayRing，导致当开盖接听功能关闭时不只解锁了同时还关闭了来电铃声*/
            MMIKL_UnlockPhone();/*@fen.xie 开盖总是要解锁的*/

            //开盖接听若打开则继续处理接听操作，否则do nothing
            if (MMISET_RECEIVE_PHONE_TYPE != MMIAPISET_GetSlideOpenType())
            {
                break;/*开盖接听关闭，do nothing*/
            }
        }
#endif
        if(!MMICC_AnswerCall())
        {
            return recode;
        }
        break;

    case MSG_LCD_SWITCH:
//        SetCallingWinTextFormParam();      
#if defined(TOUCH_PANEL_SUPPORT)
        MMIAPIIDLE_SetDtmfEditFormParam(win_id);
#endif

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
        if(MMICC_ANIMATION_WIN_ID == win_id)
        {
            MMIAPIIDLE_SetDtmfPanelLayerPosition(win_id, s_animwin_dtmfpanel_layer);
            //SCI_MEMSET(&s_animwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));
        }
        else if(MMICC_STATUS_WIN_ID == win_id)
        {
            MMIAPIIDLE_SetDtmfPanelLayerPosition(win_id, s_connectedwin_dtmfpanel_layer);
            //SCI_MEMSET(&s_connectedwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));
        }
#endif
        break;

    case MSG_GSENSOR: //来电时屏幕晃动关闭来电铃声和振动
        if(MMK_GSENSOR_STATUS_BACKWARD_SIDE == (uint32 )param)
        {
#ifdef CMCC_UI_STYLE
            s_is_cancel_reject = TRUE;
#endif
            if (MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_CALL_SILENCE))
            {
                MMIAPICC_StopMtCallPlayingRing();
            }
        }            
        break;
        
    case MSG_BT_CANCEL_OUTGOING_CALL: //blue tooth headset cancel outgoing call	
    case MSG_BT_REJECT_INCOMING_CALL: //blue tooth headset reject the incoming call           
    case MSG_APP_RED://该消息是由耳机按键转化而来
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/17
        MMIAPICC_StopRingOrVibrate();   //Modified by yanqi.2010-Apr-20 静音无效
        if (msg_id == MSG_APP_CANCEL && MMIAPICC_IsMtCallPlayingRing())
        {
            CC_HandleAnimWinSoftkey(win_id, TRUE);
            break;
        }
#else

#ifndef MMI_GUI_STYLE_TYPICAL
        if (CC_DISPLAY_DTMF_PANEL == CC_INSTANCE.cc_display_type
            && (MSG_APP_CANCEL == msg_id || MSG_CTL_CANCEL == msg_id))
        {
            CC_HideDtmfPanel(win_id);

            break;
        }
#endif

#ifdef MMI_PDA_SUPPORT
        if (MSG_APP_CANCEL == msg_id || MSG_CTL_CANCEL == msg_id)//PDA下不允许返回
        {
            break;
        }
#else
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        if (MSG_APP_CANCEL == msg_id || MSG_CTL_CANCEL == msg_id)
        {
#ifdef CMCC_UI_STYLE
        s_is_cancel_reject = TRUE;
#else
            if (MMIAPICC_IsMtCallPlayingRing())/*lint !e527*/
            {
                MMIAPICC_StopMtCallPlayingRing();
                break;
            }
#endif
#if !defined(MMI_PDA_SUPPORT)&&defined(TOUCH_PANEL_SUPPORT)     
            if(MMICC_IsExistOutgoingCall())
            {
                EnableHandfreeMode(!MMIAPICC_IsHandFree()); /*lint !e730*/
                SetConectedWinRightSoftkey(win_id, TRUE);
                break;
            }
#endif  //TOUCH_PANEL_SUPPORT
        }
#endif
#endif
   
#endif// DPHONE_SUPPORT
#ifdef TTS_SUPPORT
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_CALL);
#endif

        MMIAPICC_StopRingOrVibrate();
#ifdef BT_DIALER_SUPPORT
        if(MMICC_GetBtCallFlag())
        {
			// no bt call exist now
			if(0 == MMICC_GetCurrentCallIndex())
			{
				bt_status = MMIAPIBT_HfuHangup();
				if(BT_PENDING !=  bt_status&& BT_SUCCESS != bt_status)
				{
					SCI_TRACE_LOW("ccwintab.c:CC_HandleCcAnimWinMsg() MMIAPIBT_HfuHangup fail");
				}
			}
			// bt call exist, reject current call
			else
			{
				if(CC_RESULT_ERROR == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(),MN_CAUSE_USER_BUSY ))
				{
					SCI_TRACE_LOW("CC_HandleCcAnimWinMsg MMMICC_ReleaseCallReq fail");
				}
			}
        }
        else
#endif
{

        //根据当前CALL的状态进行相应的处理
        switch(MMICC_GetCurrentCallStatus())
        {
        case CC_CALLING_STATE:
            MMICC_ClearWantCallInfo();
            ClearAutoRedialInfo();
            //hang up call
            if(CC_RESULT_ERROR == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(),MN_CAUSE_NORMAL_CLEARING ))
            {
                //显示失败
            }
            break;
            
        case CC_INCOMING_CALL_STATE:
            ClearAutoRedialInfo();
            //hang up call
            if(CC_RESULT_ERROR == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(),MN_CAUSE_USER_BUSY ))
            {
                //显示失败
            }
            break;
            
        case CC_WAITING_STATE:
            ClearAutoRedialInfo();
            //hang up call
            if(CC_RESULT_ERROR == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(),MN_CAUSE_USER_BUSY))
            {
                //显示失败
            }
            break;
            
        default:
            //SCI_TRACE_LOW:"ccapp.c: CC_HandleCcWinMsg() call_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_7102_112_2_18_2_10_55_329,(uint8*)"d", MMICC_GetCurrentCallStatus());
            break;
        }
}
        
//        MMK_CloseWin(win_id);
        break;
            
    case MSG_KEYLONG_UPSIDE:
    case MSG_KEYLONG_DOWNSIDE:      //长按挂断
        if( SCI_FALSE == MMIDEFAULT_GetFlipStatus()) 
        {
            MMIDEFAULT_TurnOnBackLight();
            
            MMIAPICC_StopMtCallPlayingRing();
            ClearAutoRedialInfo();

            if(CC_RESULT_ERROR == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(),MN_CAUSE_USER_BUSY))
            {
                //显示失败
            }
            MMK_CloseWin(win_id);
        }			
        break;
        
    case MSG_CLOSE_WINDOW:
         if (CC_INSTANCE.waiting_call_ring_timer != 0)
        {
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
            MMK_StopTimer(CC_INSTANCE.waiting_call_ring_timer);
            CC_INSTANCE.waiting_call_ring_timer = 0;
            s_waiting_call_ring_play_times = 0;
        }

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
        MMIAPICC_StopRingOrVibrate();

#ifdef TTS_SUPPORT
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_CALL);
#endif


#ifdef MV_SUPPORT
        MMK_CloseWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID);
#endif

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
        if(MMICC_ANIMATION_WIN_ID == win_id)
        {
            SCI_MEMSET(&s_animwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));//UILAYER_ReleaseLayer(&s_animwin_dtmfpanel_layer);
        }
        else if(MMICC_STATUS_WIN_ID == win_id)
        {
            SCI_MEMSET(&s_connectedwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));//UILAYER_ReleaseLayer(&s_connectedwin_dtmfpanel_layer);
        }
#endif

        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMK_CloseGsensor( win_id );
        break;
#ifdef DPHONE_SUPPORT
    case MSG_KEYDOWN_HOOK:
    case MSG_KEYDOWN_HANDSFREE:
        recode = FALSE;
        break;
#endif

#if !defined(TOUCH_PANEL_SUPPORT)
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        HandleCCWinWebMsg(win_id);
#else
	if(MMICC_IsExistIncommingCall())
       {
       	if (MMIAPICC_IsMtCallPlayingRing())/*lint !e527*/
              {
                  MMIAPICC_StopMtCallPlayingRing();
              }       	

       }
       if(MMICC_IsExistOutgoingCall())
        {
        	EnableHandfreeMode(!MMIAPICC_IsHandFree()); /*lint !e730*/        
        }
	
#endif
        break;
#endif

    case MSG_LOSE_FOCUS:
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
        break;

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
    case MSG_GET_FOCUS:
        CC_ReappendDtmfPanelLayer(win_id);
#ifdef BT_DIALER_SUPPORT
        MMICC_UpdateCurrentCallStatus(FALSE);
#endif
        break;
#else

#ifdef BT_DIALER_SUPPORT
    case MSG_GET_FOCUS:
        MMICC_UpdateCurrentCallStatus(FALSE);
        break;
#endif
#endif

#ifdef TOUCH_PANEL_SUPPORT
   case MSG_CTL_BUTTON_DOWN:
        HandleCCWinButtonDownMsg(win_id, ((MMI_NOTIFY_T*)param)->src_id);
        break;

   case MSG_CTL_BUTTON_UP:
        HandleCCWinButtonUpMsg(((MMI_NOTIFY_T*)param)->src_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
        HandleCCWinPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef DRM_SUPPORT
    case MSG_CTL_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T	display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
            MMIPB_BCD_NUMBER_T pb_num = {0};
            wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
            uint32 custom_photo_name_len = 0;
            uint32 photo_file_size = 0;
            if(GUIANIM_RESULT_SUCC == display_ind.result)
            {
                MMICC_GetCurrentCallNumber(&pb_num, TRUE, MMICC_IsExistOutgoingCall());
                if(MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
                {
                    if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE, custom_photo_name))
                    {                
                        MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, custom_photo_name);
                    }                        
                }
            }
        }       
        break;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL        
    case MSG_CC_ALLOW_SEND_DEMF_MSG:
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        if (!MMICC_IsEmergencyCall())
        {
            CC_SetHotButtonGrayState(CC_BUTTON_DIALPANEL, FALSE);
            CC_UpdateCCWinHotButton(CC_BUTTON_DIALPANEL, FALSE);
        }
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
#if defined(TOUCH_PANEL_SUPPORT) && (!defined MMI_CALLCONTROL_MINI_SUPPORT)        
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_MT_LOCK)
        {
            CC_HandleCcLockMsg(win_id, msg_id, param);
        }
        else 
#endif            
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL && msg_id == MSG_TP_PRESS_UP)
        {
            GUI_POINT_T	    tp_point = {0};
            
            GUI_BOTH_RECT_T                  dtmf_both_rect;
            BOOLEAN is_dtmf_rect = FALSE;
            
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            dtmf_both_rect.v_rect.left = 0;
            dtmf_both_rect.v_rect.top = MMI_MAINSCREEN_HEIGHT - MMI_DTMF_PANEL_HEIGHT - 1;
            dtmf_both_rect.v_rect.right = MMI_MAINSCREEN_WIDTH;
            dtmf_both_rect.v_rect.bottom = MMI_MAINSCREEN_HEIGHT;

            dtmf_both_rect.h_rect.left = MMI_MAINSCREEN_HEIGHT - MMI_DTMF_PANEL_WIDTH_H - 1;
            dtmf_both_rect.h_rect.top = MMI_MAINSCREEN_WIDTH - MMI_DTMF_PANEL_HEIGHT_H - 1;
            dtmf_both_rect.h_rect.right = MMI_MAINSCREEN_HEIGHT;
            dtmf_both_rect.h_rect.bottom = MMI_MAINSCREEN_WIDTH;

            if(!MMITHEME_IsMainScreenLandscape())
            {
                is_dtmf_rect = GUI_PointIsInRect(tp_point,dtmf_both_rect.v_rect);
            }
            else
            {
                is_dtmf_rect = GUI_PointIsInRect(tp_point,dtmf_both_rect.h_rect);
            }
            
            if (!is_dtmf_rect)
            {
                CC_HideDtmfPanel(win_id);
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    //crNEWMS00114847,响应关机键
    case MSG_KEYLONG_POWER:
    	recode = MMI_RESULT_FALSE;
    	break;

    case MSG_APP_POWER:
        if (MMICC_IsExistIncommingCall())
        {
            MMIAPICC_StopMtCallPlayingRing(); 
        }
    	recode = MMI_RESULT_FALSE;
        break;

#endif

    default:
        recode = MMI_RESULT_FALSE;
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleCcWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_7267_112_2_18_2_10_55_330,(uint8*)"d", msg_id);
        break;
    }
    
    return ( recode );
}

/*****************************************************************************/
//  Description :CC_FindLocationInforl 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL void CC_HandleAnimWinSoftkey( MMI_HANDLE_T win_handle, BOOLEAN is_need_fresh )
{
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
#ifdef CMCC_UI_STYLE
    MMISET_ANYKEY_REJECT_SET_T   reject_info = MMIAPISET_GetAnykeyRejectMode();
#endif
#endif

#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/17  去掉右软键免提/ 听筒的切换功能
    if (MMICC_IsExistIncommingCall())
    {
         if( !MMICC_IsExistActiveCall() && !MMICC_IsExistHoldCall())
        {
            // set the softkey
            if (MMIAPICC_IsMtCallPlayingRing())
            {
                CC_SetCallWinSoftkey(win_handle,STXT_ANSWER,TXT_NULL,TXT_RING_SILENT,TRUE);
            }
            else
            {
                CC_SetCallWinSoftkey(win_handle,STXT_ANSWER,TXT_NULL,STXT_REJECT,TRUE);
            }
        }
        else
        {
            CC_SetCallWinSoftkey(win_handle,STXT_MENU,TXT_NULL,STXT_REJECT,TRUE);
        }
    }
    else
    {       
        // set the softkey according the status 
        CC_SetCallWinSoftkey(win_handle,TXT_NULL,TXT_NULL,STXT_EXIT,TRUE);  		
    }
#else		

    if (MMICC_IsExistIncommingCall())
    {
         if( !MMICC_IsExistActiveCall() && !MMICC_IsExistHoldCall())
        {
            // set the softkey
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
            if (MMIAPICC_IsMtCallPlayingRing())
            {
                SetHandFreeMenu(win_handle,STXT_ANSWER,TXT_NULL,TXT_RING_SILENT,is_need_fresh);
                GUIWIN_SeSoftkeytButtonTextId(win_handle, TXT_NULL, MIDDLE_BUTTON, is_need_fresh);
            }
            else
            {

                {
                    SetHandFreeMenu(win_handle,STXT_ANSWER,TXT_NULL,STXT_REJECT,is_need_fresh);
                }
    
                GUIWIN_SeSoftkeytButtonTextId(win_handle, TXT_NULL, MIDDLE_BUTTON, is_need_fresh);
            }
#else
            if (MMIAPICC_IsMtCallPlayingRing())
            {
#ifdef GUI_MID_SOFTKEY_USE_IMG
                SetHandFreeMenu(win_handle,STXT_ANSWER, IMAGE_COMMON_SOFTKEY_SILENT_ICON, STXT_REJECT,is_need_fresh);
#else
                SetHandFreeMenu(win_handle, STXT_ANSWER, TXT_RING_SILENT, STXT_REJECT, is_need_fresh);
#endif
                
                //GUIWIN_SeSoftkeytButtonTextId(win_handle, TXT_NULL, MIDDLE_BUTTON, is_need_fresh);
            }
            else
            {

                {
                    SetHandFreeMenu(win_handle,STXT_ANSWER,TXT_NULL,STXT_REJECT,is_need_fresh);
                }
    
                GUIWIN_SeSoftkeytButtonTextId(win_handle, TXT_NULL, MIDDLE_BUTTON, is_need_fresh);
            }

#endif
        }
        else
        {
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)

#endif
        {
            if(!MMIAPIDM_GetHandsetLock())
            {
#ifdef BT_DIALER_SUPPORT
				if(MMICC_GetBtCallFlag())
				{
					SetHandFreeMenu(win_handle,STXT_ANSWER,TXT_NULL, STXT_REJECT,is_need_fresh);
				}
				else
#endif
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
				{
				    SetHandFreeMenu(win_handle,STXT_MENU,TXT_NULL, STXT_REJECT,is_need_fresh);   
				}
#else
				{
		  			SetHandFreeMenu(win_handle,STXT_MENU,TXT_NULL, STXT_REJECT,is_need_fresh);
		  		}
#endif 
            }
            else
            {
                SetHandFreeMenu(win_handle, TXT_NULL, TXT_NULL, STXT_REJECT, is_need_fresh);
            }    
        }

            GUIWIN_SeSoftkeytButtonTextId(win_handle, TXT_NULL, MIDDLE_BUTTON, is_need_fresh);
        }
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
#ifdef CMCC_UI_STYLE
        if(reject_info.is_sms_reply_open && !MMIAPIDM_GetHandsetLock())
        {
            SetHandFreeMenu(win_handle, TXT_NULL, TXT_NULL, STXT_SMS_REJECT, is_need_fresh);
        }
#endif
#endif
    }
    else
    {      
#if !defined(MMI_PDA_SUPPORT)&&defined(TOUCH_PANEL_SUPPORT)  
        SetConectedWinRightSoftkey(win_handle, is_need_fresh);
		//CC_SetCallWinSoftkey(win_handle, TXT_NULL, TXT_NULL, TXT_CC_HANG_OFF, is_need_fresh);
#else
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_SetCallWinSoftkey(win_handle, TXT_NULL, TXT_NULL, TXT_CC_HANG_OFF, is_need_fresh);
#else
	if(MMIAPICC_IsHandFree())
       {
    #ifdef GUI_MID_SOFTKEY_USE_IMG
            CC_SetCallWinSoftkey(win_handle, TXT_NULL,IMAGE_COMMON_SOFTKEY_EARPIECE_ICON, TXT_CC_HANG_OFF, is_need_fresh);
    #else
            CC_SetCallWinSoftkey(win_handle, TXT_NULL,TXT_HAND_ON, TXT_CC_HANG_OFF, is_need_fresh);
    #endif
       }
       else
       {
    #ifdef GUI_MID_SOFTKEY_USE_IMG
            CC_SetCallWinSoftkey(win_handle, TXT_NULL, IMAGE_COMMON_SOFTKEY_HANDFREE_ICON, TXT_CC_HANG_OFF, is_need_fresh);
    #else
            CC_SetCallWinSoftkey(win_handle, TXT_NULL, TXT_COMMON_HAND_FREE, TXT_CC_HANG_OFF, is_need_fresh);
    #endif
       }	
#endif
#endif

#ifdef CMCC_UI_STYLE
        {
            MN_NUMBER_TYPE_E    number_type = MN_NUM_TYPE_UNKNOW;
            uint8               tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
            uint8               tel_num_len = 0;
            MMIPB_BCD_NUMBER_T  pb_num = {0};

            MMICC_GetCurrentCallNumber(&pb_num, TRUE, TRUE);
            number_type = MMIPB_GetNumberTypeFromUint8(pb_num.npi_ton);

            if (MN_NUM_TYPE_ALPHANUMERIC == number_type)
            {
                number_type = MN_NUM_TYPE_UNKNOW;
            }

            tel_num_len = MMIAPICOM_GenDispNumber(number_type, pb_num.number_len, pb_num.number, tel_num, (uint8)sizeof(tel_num));
            if (MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, MMICC_GetWantCallDualSys(), TRUE)
                && MMICC_IsExistOutgoingCall())
             {
                GUIWIN_SeSoftkeytButtonTextId(win_handle, STXT_OPTION, LEFT_BUTTON, is_need_fresh);
             }
             else
             {
                GUIWIN_SeSoftkeytButtonTextId(win_handle, TXT_NULL, LEFT_BUTTON, is_need_fresh);
             }

        }
#endif
    }
#endif 
}

/*****************************************************************************/
//  Description :CC_FindLocationInforl 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL void CC_FindLocationInfor( MMI_STRING_T* mmi_str, uint16* str, 
                                uint8* tel_num, uint8 tel_num_len )
{
#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    uint16  temp_str[MMIFINDMP_CITYNAME_LENGTH] = {0};
    uint16*  temp_ptr = PNULL;
    uint16  str_length = 0;
    MMI_STRING_T	temp_mmi_str = {0};
#endif    
    uint8   origin_number_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    int16   origin_num_len  = 0;
    
    
    if(MMIAPICC_RemoveIpNumber(tel_num, tel_num_len, origin_number_arr,CC_MAX_TELE_NUM_LEN))
    {
        origin_num_len = strlen((char*)origin_number_arr);
    }
    else
    {
        MMI_MEMCPY(origin_number_arr, CC_MAX_TELE_NUM_LEN + 2, tel_num, tel_num_len, tel_num_len);
        origin_num_len = tel_num_len;
    }
//============== MET AQ  start ===================
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
    if(MMIMEX_AQCCFindLocalInfo(origin_number_arr,origin_num_len,mmi_str,str))
    {
        return;
    }
#endif
	//============== MET AQ  end ===================
	
#ifdef HOME_LOCATION_INQUIRE_SUPPORT  
    str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_arr, origin_num_len, temp_str);

#ifndef MMI_GUI_STYLE_TYPICAL
    if( str_length == 0 )
    {
        //MMI_GetLabelTextByLang(TXT_UNKNOWN, &temp_mmi_str);
    }
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
    {
        MMI_GetLabelTextByLang(TXT_LOCAL, &temp_mmi_str);
    }
    else
    {
        //MMI_GetLabelTextByLang( TXT_GUISHUDI_NOCOMMA, &temp_mmi_str );
    }
#else
    if( str_length == 0 )
    {
        MMI_GetLabelTextByLang(TXT_GUISHUDIUNKNOWN, &temp_mmi_str);
    }
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER )
    {
        MMI_GetLabelTextByLang(TXT_GUISHUDILOCAL, &temp_mmi_str);
    }
    else
    {
        MMI_GetLabelTextByLang( TXT_GUISHUDI, &temp_mmi_str );
    }
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
    if (0 == str_length || str_length == MMIPHONESEARCH_LOCALNUMBER)
#endif
    {
        MMIAPICOM_Wstrncpy( str, temp_mmi_str.wstr_ptr, temp_mmi_str.wstr_len );
        mmi_str->wstr_len = temp_mmi_str.wstr_len;
    }
    
    
    if ((0 != str_length)&&(MMIPHONESEARCH_LOCALNUMBER != str_length))
    {
        
        temp_ptr = str;
#ifdef MMI_GUI_STYLE_TYPICAL
        temp_ptr += temp_mmi_str.wstr_len;
        //增加“：”
        {
            wchar colon_str[] = {':', 0};
            MMIAPICOM_Wstrncpy( temp_ptr, colon_str, 1 );
            temp_ptr += 1;
            mmi_str->wstr_len += 1;
        }        
#endif
        MMIAPICOM_Wstrncpy( temp_ptr, temp_str, str_length );
        mmi_str->wstr_len += str_length;
    }
    
    mmi_str->wstr_ptr = str;

#endif

}




/*****************************************************************************/
//  Description :to open processing win include disconnected win
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void OpenProcessingWin(MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array, MMI_IMAGE_ID_T img_id, MMI_ANIM_ID_T anim_id, PROCESSMSG_FUNC handle_func)
{
#if defined(MAINLCD_SIZE_128X64) //@ alex added in 2010.11.29
    MMI_HANDLE_T form_id = MMICC_PROCESSING_FORM_CTRL_ID;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T form_create = {0};
    GUIFORM_INIT_DATA_T form_init = {0};
    GUIFORM_DYNA_CHILD_T form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    //GUIANIM_INIT_DATA_T anim_init_data = {0};  /* lint_lai */
    MMI_CONTROL_CREATE_T softkey_create = {0};
    GUISOFTKEY_INIT_DATA_T softkey_init = {0};
    int32 index = 0;
    //void *anim_ctrl_ptr = PNULL;  /* lint_lai */
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);

    CHECK_CC_APPLET_NOT_NULL
    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);

    //create form
    form_init.both_rect.v_rect.left = client_rect.v_rect.left;
    form_init.both_rect.v_rect.top = 0;//MMICC_PROCESS_FORM_TOP;
    form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom;
    
    form_init.both_rect.h_rect.left = client_rect.h_rect.left;
    form_init.both_rect.h_rect.top = 0;//MMICC_PROCESS_FORM_TOP_H;
    form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    form_create.ctrl_id = form_id;
    form_create.guid = SPRD_GUI_FORM_ID;
    form_create.init_data_ptr = &form_init;
    form_create.parent_win_handle = win_id;
    MMK_CreateControl(&form_create);

    //not slide
    GUIFORM_IsSlide(form_id,FALSE);

    //create text label
    text_label_init.align = GUILABEL_ALIGN_MIDDLE;
    form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    form_child_ctrl.is_bg = FALSE;
    form_child_ctrl.is_get_active = FALSE;
    form_child_ctrl.init_data_ptr = &text_label_init;

    for(index=0; index<=(int32)(MMICC_PROCESSING_TEXT_LABEL_1_CTRL_ID - MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID); index++)
    {
        form_child_ctrl.child_handle = MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID + index;
        GUIFORM_CreatDynaChildCtrl(win_id, form_id,  &form_child_ctrl);
        if(prompt_str_array[index].wstr_len > 0 && (PNULL != prompt_str_array[index].wstr_ptr))
        {
            GUILABEL_SetText(form_child_ctrl.child_handle, &prompt_str_array[index], FALSE);
	     GUILABEL_SetFont(form_child_ctrl.child_handle, MMI_DEFAULT_NORMAL_FONT, CC_TEXT_COLOR);		
        }
    }
#else    
    MMI_HANDLE_T form_id = MMICC_PROCESSING_FORM_CTRL_ID;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T form_create = {0};
    GUIFORM_INIT_DATA_T form_init = {0};
    GUIFORM_DYNA_CHILD_T form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    GUIANIM_INIT_DATA_T anim_init_data = {0};
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_CONTROL_CREATE_T softkey_create = {0};
    GUISOFTKEY_INIT_DATA_T softkey_init = {0};
#endif

    uint32 index = 0;
//     void *anim_ctrl_ptr = PNULL;
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);

    CHECK_CC_APPLET_NOT_NULL
    //create window
    win_create.applet_handle = CC_APPLET_HANDLE;
    win_create.win_id = win_id;
    win_create.func = handle_func;
    win_create.win_priority = WIN_ONE_LEVEL;
    MMK_CreateWindow(&win_create);
    
    CreateStatusBarCtrl(win_id, IMAGE_NULL);

    //create form
    form_init.both_rect.v_rect.left = client_rect.v_rect.left;
    form_init.both_rect.v_rect.top = MMICC_PROCESS_FORM_TOP;
    form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom;
    
    form_init.both_rect.h_rect.left = client_rect.h_rect.left;
    form_init.both_rect.h_rect.top = MMICC_PROCESS_FORM_TOP_H;
    form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    form_create.ctrl_id = form_id;
    form_create.guid = SPRD_GUI_FORM_ID;
    form_create.init_data_ptr = &form_init;
    form_create.parent_win_handle = win_id;
    MMK_CreateControl(&form_create);

    //not slide
    GUIFORM_IsSlide(form_id,FALSE);

//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    {
        GUI_BG_T bg_info = {0};
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_COMMON_BG;

        GUIFORM_SetBg(form_id, &bg_info);
        GUIFORM_PermitChildFont(form_id,FALSE);
    }
#endif
    //create text label
    text_label_init.align = GUILABEL_ALIGN_MIDDLE;
    form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    form_child_ctrl.is_bg = FALSE;
    form_child_ctrl.is_get_active = FALSE;
    form_child_ctrl.init_data_ptr = &text_label_init;

    for(index=0; index<=((uint32)MMICC_PROCESSING_TEXT_LABEL_1_CTRL_ID - (uint32)MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID); index++)
    {
        form_child_ctrl.child_handle = MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID + index;
        GUIFORM_CreatDynaChildCtrl(win_id, form_id,  &form_child_ctrl);
        if(prompt_str_array[index].wstr_len > 0 && (PNULL != prompt_str_array[index].wstr_ptr))
        {
            GUILABEL_SetText(form_child_ctrl.child_handle, &prompt_str_array[index], FALSE);
        }
    }

    //create anim

    form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
    form_child_ctrl.child_handle = MMICC_PROCESSING_ANIM_CTRL_ID;
    form_child_ctrl.is_bg = FALSE;
    form_child_ctrl.is_get_active = FALSE;
    form_child_ctrl.init_data_ptr = &anim_init_data;
    GUIFORM_CreatDynaChildCtrl(win_id, form_id,  &form_child_ctrl);
    
    //set anim
    {
        GUIANIM_CTRL_INFO_T     control_info = {0};
        GUIANIM_DATA_INFO_T     data_info = {0};
        GUIANIM_DISPLAY_INFO_T  display_info = {0};
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = form_child_ctrl.child_handle;
        
        if (IMAGE_NULL != anim_id)
        {
            data_info.anim_id = anim_id;
        }
        else
        {
            data_info.img_id = img_id;    
        }
        
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_play_once = FALSE;
        GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    }
#endif  // MAINLCD_SIZE_128X64

#ifdef MMI_GUI_STYLE_TYPICAL//pda大多数场景不显示softkey
    //create softkey control
    softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
    softkey_create.init_data_ptr     = &softkey_init;
    softkey_create.parent_win_handle = win_id;
 
    MMK_CreateControl(&softkey_create);
    MMK_SetWinSoftkeyCtrlId(win_id,softkey_create.ctrl_id);
#endif
    
//    SetCallingWinTextFormParam();  
}

/*****************************************************************************/
//  Description :to set mo or mt win text
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetCallingWinText(BOOLEAN is_update, MMI_STRING_T* prompt_str_array)
{
   uint32 index = 0;
   MMI_CTRL_ID_T label_ctrl_id = 0;
   
   for (index = 0; index < ((uint32)MMICC_CALLING_LOCAT_LABEL_CTRL_ID - (uint32)MMICC_CALLING_STATE_LABEL_CTRL_ID); index++)
   {
       if (0 < prompt_str_array[index].wstr_len && PNULL != prompt_str_array[index].wstr_ptr)
       {
           label_ctrl_id = MMICC_CALLING_STATE_LABEL_CTRL_ID + index;
           GUILABEL_SetText(label_ctrl_id, &prompt_str_array[index], is_update);
       }
   }
}


/*****************************************************************************/
//  Description :to set processing win text
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void SetProcessingWinText(BOOLEAN is_update, MMI_HANDLE_T win_id, MMI_STRING_T* prompt_str_array)
{
   uint32 index = 0;
   MMI_CTRL_ID_T label_ctrl_id = 0;
   
   for (index = 0; index < ((uint32)MMICC_PROCESSING_ANIM_CTRL_ID - (uint32)MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID); index++)
   {
       if (0 < prompt_str_array[index].wstr_len && PNULL != prompt_str_array[index].wstr_ptr)
       {
           label_ctrl_id = MMICC_PROCESSING_TEXT_LABEL_0_CTRL_ID + index;
           GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id, label_ctrl_id), &prompt_str_array[index], is_update);
       }
   }
}


/*****************************************************************************/
//  Description : display win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatePhotoAnim( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T anim_ctrl_id, GUI_BOTH_RECT_T* both_rect_ptr, CC_PHOTO_TYPE_E photo_type)
{
    MMI_CONTROL_CREATE_T anim_create = {0};
    GUIANIM_INIT_DATA_T anim_init_data = {0};
    void *anim_ctrl_ptr = PNULL;
    BOOLEAN is_want_mo_call = (MMICC_AUTO_DIAL_WIN_ID == win_id) || (MMICC_IsExistOutgoingCall());

    //if anim ctrl is not being , create
    if(0 == MMK_GetCtrlHandleByWin(win_id, anim_ctrl_id)) 
    {
        if(PNULL == both_rect_ptr)
        {
            //SCI_TRACE_LOW:"ccapp CreatePhotoAnim null ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_7751_112_2_18_2_10_56_331,(uint8*)"");
            return;
        }
        //create photo anim
        anim_init_data.both_rect = *both_rect_ptr;

        anim_create.guid = SPRD_GUI_ANIM_ID;
        anim_create.ctrl_id = anim_ctrl_id;
        anim_create.parent_win_handle = win_id;
        anim_create.init_data_ptr = &anim_init_data;
        anim_ctrl_ptr = MMK_CreateControl(&anim_create);
    }

    //set photo anim
    {
        GUIANIM_CTRL_INFO_T control_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};
        MMIPB_BCD_NUMBER_T pb_num = {0};
        GUIANIM_FILE_INFO_T file_info = {0};
        wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
        uint32 custom_photo_name_len = 0;
        uint32 photo_file_size = 0;
        BOOLEAN is_need_find_pb = FALSE;
        uint8 call_id = 0;
        MMI_IMAGE_ID_T default_image_id = IAMGE_CALL_DEFAULT_PHOTO;

        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = anim_ctrl_id;
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

        switch(photo_type)
        {
        case PHOTO_TYPE_CURRENT:
            MMICC_GetCurrentCallNumber(&pb_num, TRUE, is_want_mo_call);
            if(MMICC_IsExistMPTY()
            && (!MMICC_IsExistIncommingCall() 
                && !MMICC_IsExistOutgoingCall()))
            {
                is_need_find_pb = FALSE;
#ifndef MMI_GUI_STYLE_TYPICAL
                default_image_id = IMAGE_CC_CONFERENCE;
#else
                default_image_id = IAMGE_CALL_DEFAULT_PHOTO;
#endif                
            }
            else
            {
                is_need_find_pb = TRUE;
                default_image_id = IAMGE_CALL_DEFAULT_PHOTO;
            }
            break;

        case PHOTO_TYPE_ACTIVE:
            if(1 == MMIAPICC_GetActiveCallCount())
            {
                is_need_find_pb = TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
                default_image_id = IAMGE_CALL_DEFAULT_PHOTO;
#else
                default_image_id = IAMGE_CC_DEFAULT_PHOTO_SMALL;
#endif
                call_id = MMICC_FindStateCall(CC_CALL_STATE);
                pb_num = MMICC_GetPbNumByIndex(call_id);
            }
#ifndef MMI_GUI_STYLE_TYPICAL
            else if (MMICC_IsExistOutgoingCall())
            {
                is_need_find_pb = TRUE;
                default_image_id = IAMGE_CALL_DEFAULT_PHOTO;
                MMICC_GetCurrentCallNumber(&pb_num, TRUE, FALSE);
            }
#endif
            else if(MMICC_IsExistActiveMPTY())
            {
                is_need_find_pb = FALSE;
                default_image_id = IMAGE_CC_CONFERENCE;
            }
            else
            {
                is_need_find_pb = FALSE;
            }
            break;

        case PHOTO_TYPE_HOLD:
            if(MMICC_IsExistHoldCall() && !MMICC_IsExistHoldMPTY())
            {
                is_need_find_pb = TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT  
                default_image_id = IAMGE_CALL_HOLD_DEFAULT_PHOTO_PDA;
#else
		  		default_image_id = IAMGE_CALL_DEFAULT_PHOTO;
#endif
#else
                default_image_id = IAMGE_CC_DEFAULT_PHOTO_SMALL;
#endif
                call_id = MMICC_FindStateCall(CC_HOLD_STATE);
                pb_num = MMICC_GetPbNumByIndex(call_id);
            }
            else if(MMICC_IsExistHoldMPTY())
            {
                is_need_find_pb = FALSE;
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT  
                default_image_id = IMAGE_CC_CONFERENCE_HOLD_PDA; 
#else
		  		default_image_id = IMAGE_CC_CONFERENCE;
#endif
#else
                default_image_id = IMAGE_CC_CONFERENCE;
#endif
            }
            else
            {
                is_need_find_pb = FALSE;
            }
            break;
#ifndef MMI_GUI_STYLE_TYPICAL
        case PHOTO_TYPE_OTHER:
            if (MMICC_DISCONNECT_WIN_ID == win_id)
            {
                is_need_find_pb = TRUE;
                default_image_id = IAMGE_CALL_DEFAULT_PHOTO;       
                pb_num = MMICC_GetPbNumByIndex(CC_INSTANCE.call_end_index);
            }
            break;
#endif
        default:
            //do nothing
            break;
        }
        

        if(is_need_find_pb && MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
        {
            GUIANIM_SetDefaultIconByPtr(anim_ctrl_ptr,IMAGE_COMMON_PIC_ICON,IMAGE_COMMON_PIC_ICON);
            file_info.full_path_wstr_ptr = custom_photo_name;
            file_info.full_path_wstr_len = custom_photo_name_len;
            display_info.bg.bg_type = GUI_BG_IMG;
            display_info.bg.img_id = IMAGE_COMMON_BG;
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_GUI_STYLE_TYPICAL)
            if(MMICC_ANIMATION_WIN_ID == win_id
#ifdef MMI_KEY_LOCK_SUPPORT
                && MMIAPIKL_IsPhoneLocked()
#endif
               )
            {
                display_info.bg.bg_type = GUI_BG_NONE;
            }
#endif
            GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);
        }
        else
        {
#ifdef CMCC_UI_STYLE
            MN_NUMBER_TYPE_E    number_type = MN_NUM_TYPE_UNKNOW;
            uint8                           tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
            uint8                           tel_num_len = 0;

            number_type = MMIPB_GetNumberTypeFromUint8(pb_num.npi_ton);

            if (MN_NUM_TYPE_ALPHANUMERIC == number_type)
            {
                number_type = MN_NUM_TYPE_UNKNOW;
            }

            tel_num_len = MMIAPICOM_GenDispNumber(number_type, pb_num.number_len, pb_num.number, tel_num, (uint8)sizeof(tel_num));
            
            //如果是紧急呼叫号码
            if (MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, MMICC_GetWantCallDualSys(), TRUE))
            {
                data_info.img_id = IAMGE_CALL_EMERGENCY_PHOTO;
            }
            else
            {
				data_info.img_id = default_image_id;
            }
#else
            data_info.img_id = default_image_id;
#endif
            display_info.bg.bg_type = GUI_BG_IMG;
            display_info.bg.img_id = IMAGE_COMMON_BG;
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_GUI_STYLE_TYPICAL)
            if(MMICC_ANIMATION_WIN_ID == win_id
#ifdef MMI_KEY_LOCK_SUPPORT
                && MMIAPIKL_IsPhoneLocked()
#endif
               )
            {
                display_info.bg.bg_type = GUI_BG_NONE;
            }
#endif            
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
        }
    }
}

/*****************************************************************************/
//  Description : display win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayWinBg( MMI_WIN_ID_T win_id, BOOLEAN is_need_blind )
{
    GUI_POINT_T dis_point = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
    //display bg
#ifndef MMI_GUI_STYLE_TYPICAL    
    GUI_RECT_T dis_rect = {0};
    CHECK_CC_APPLET_NOT_NULL

    if (MMITHEME_IsMainScreenLandscape())
    {
        dis_rect = client_both_rect.h_rect;
    }
    else
    {
        dis_rect = client_both_rect.v_rect;
    }
    //锁屏界面需要显示idle下的背景图
  if(CC_DISPLAY_MT_LOCK == CC_INSTANCE.cc_display_type)
    {
        GUIRES_DisplayImg(&dis_point,
            &dis_rect,
            PNULL,
            win_id,
            IMAGE_THEME_BG,
            &lcd_dev_info);
        GUIRES_DisplayImg(&dis_point,
            &dis_rect,
            PNULL,
            win_id,
            IMAGE_CC_COMMON_BG_PDA,
            &lcd_dev_info);
    }
    else
    {
        GUIRES_DisplayImg(&dis_point,
            &dis_rect,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
    }
#else

    GUI_RECT_T topper_layer_rect = {0};
    GUI_RECT_T lower_layer_rect = {0};
    
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

    if (is_need_blind)
    {
        if (MMITHEME_IsMainScreenLandscape())
        {
            topper_layer_rect.left = client_both_rect.h_rect.left;
            topper_layer_rect.right = client_both_rect.h_rect.right;
            topper_layer_rect.top = MMICC_CALLING_TEXT_FORM_TOP_H;
            topper_layer_rect.bottom = MMICC_CALLING_TEXT_FORM_BOTTOM_H;

            lower_layer_rect.left = client_both_rect.h_rect.left;
            lower_layer_rect.right = client_both_rect.h_rect.right;
            lower_layer_rect.top = MMICC_CALLING_ANIM_FORM_TOP_H;
            lower_layer_rect.bottom = client_both_rect.h_rect.bottom ;
        }
        else
        {
            topper_layer_rect.left = client_both_rect.v_rect.left;
            topper_layer_rect.right = client_both_rect.v_rect.right;
            topper_layer_rect.top = MMICC_CALLING_TEXT_FORM_TOP;
            topper_layer_rect.bottom = MMICC_CALLING_TEXT_FORM_BOTTOM;

            lower_layer_rect.left = client_both_rect.v_rect.left;
            lower_layer_rect.right = client_both_rect.v_rect.right;
            lower_layer_rect.top = MMICC_CALLING_ANIM_FORM_TOP;
            lower_layer_rect.bottom = client_both_rect.v_rect.bottom ;
        }

        //display blind layer bg
        GUIRES_DisplayImg(PNULL,
            &topper_layer_rect,
            PNULL,
            win_id,
            IMG_CC_TEXT_FORM_BG,
            &lcd_dev_info);

        GUIRES_DisplayImg(PNULL,
            &lower_layer_rect,
            PNULL,
            win_id,
            IMG_CC_ANIM_FORM_BG,
            &lcd_dev_info);
    }
#endif
}


/*****************************************************************************/
//  Description :to open the widow when mt call 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL void CC_OpenMtCallWin(void)
{
    MMI_STRING_T	prompt_str_array[CC_DISPALY_INFO_MAX] = {0};	//array[0]: 正在呼叫, array[1]: 号码, array[2]:PB中的号码
    //array[3]: 归属地
    wchar           temp_wchar[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
    uint8			tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};    
    uint8			tel_num_len = 0;
    uint16			city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH] = {0};
    wchar           temp_state_wchar[CC_CALL_STATE_STR_MAX_LEN] = {0};
    MMI_STRING_T state_str = {0};

    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenMtCallWin()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8015_112_2_18_2_10_57_332,(uint8*)"");

    //协议规定不能同时存在两个MT,如果之前有MO,MO没有接通时会把MO挂断，接通了不会提示MT
    if (MMK_IsOpenWin( MMICC_ANIMATION_WIN_ID ) )
    {
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
    }

#ifdef BT_DIALER_SUPPORT    
    if(!MMICC_GetBtCallFlag())
#endif 
{
    if( MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID))
    {
        ClearAutoRedialInfo();
        MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
    }
}

#ifdef BT_DIALER_SUPPORT    
    MMIAPICL_DeleteAllPbapSyncData();
#endif

    if(PNULL == MMICC_GetAppletInstance())
    {
        MMICC_StartCCApplet();
    }    
    
    SCI_MEMSET(&prompt_str_array[CC_DISPALY_INFO_STATE], 0, sizeof(prompt_str_array));
    
    prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr = temp_wchar;
    
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    MMI_GetLabelTextByLang(TXT_CC_INCOMING, &state_str);
#else
    MMI_GetLabelTextByLang(TXT_CC_MTCALLING, &state_str);
#endif
    CC_CallStateDisplaying( state_str.wstr_len, state_str.wstr_ptr);
    
    prompt_str_array[CC_DISPALY_INFO_STATE].wstr_len = CC_INSTANCE.sim_status.wstr_len;
    prompt_str_array[CC_DISPALY_INFO_STATE].wstr_ptr = temp_state_wchar;
    MMI_WSTRNCPY(temp_state_wchar, 
                                CC_CALL_STATE_STR_MAX_LEN, 
                                (const wchar *) CC_INSTANCE.sim_status.wstr_ptr, 
                                CC_INSTANCE.sim_status.wstr_len, 
                                CC_INSTANCE.sim_status.wstr_len);

    MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, MMICC_GetCurrentCallIndex());

    
    //get info for display call number
    //if (MMIAPICOM_IsValidNumberString((char*)tel_num, tel_num_len))
    if (0 != tel_num_len)
    {
        prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len = tel_num_len;
        MMI_STRNTOWSTR(prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_ptr,
            prompt_str_array[CC_DISPALY_INFO_NUMBER].wstr_len,
            (uint8 *)tel_num,
            tel_num_len,
            tel_num_len);
    }
    else
    {
        //the number wstr_len is 0.display unknown number info
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &prompt_str_array[CC_DISPALY_INFO_NUMBER]);
    }
    
    if (MMICC_GetCallNameStrByIndex(PNULL, MMICC_GetCurrentCallIndex()))
    {
        MMICC_GetCallNameStrByIndex(&prompt_str_array[CC_DISPALY_INFO_NAME], MMICC_GetCurrentCallIndex());
 #ifndef SUBLCD_SIZE_NONE       
        //刷新小屏，显示来电号码或者名称
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_str_array[CC_DISPALY_INFO_NAME]);
#endif
    }
#ifndef SUBLCD_SIZE_NONE
    else
    {
        //刷新小屏，显示来电号码或者名称
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_str_array[CC_DISPALY_INFO_NUMBER]);
    }
#endif   
    //@samboo add 2007-07-02 open different wins to handle vt call or cc call
      
    CC_FindLocationInfor(&(prompt_str_array[CC_DISPALY_INFO_LOCATION]), city_ucs2_str, tel_num, tel_num_len );

#ifndef MMI_GUI_STYLE_TYPICAL
    OpenCallingWin(MMICC_ANIMATION_WIN_ID, prompt_str_array, IMAGE_NULL, CC_HandleCcAnimWinMsg);
 #ifdef MMI_CALLCONTROL_MINI_SUPPORT
    DisplayCallAnimPhotoForCommon(MMICC_ANIMATION_WIN_ID,IMAGE_CC_INCOMING_PHOTO_ANIM);
 #endif
 
#else
    OpenCallingWin(MMICC_ANIMATION_WIN_ID, prompt_str_array, IMAGE_MT_CALL_ANIMA, CC_HandleCcAnimWinMsg);
#endif
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_CC, FALSE);
    
    // @Great.Tian, Fix CR24994
    //MMITHEME_SetUpdateDelayCount(2);
    
    CC_FreeSimStatusSpace();

#ifdef BLUETOOTH_SUPPORT
    MMIAPIBT_IncomingCall(prompt_str_array[CC_DISPALY_INFO_NUMBER]);
#endif

}

/*****************************************************************************/
//  Description :to open the call processing window
//  Global resource dependence : none
//  Author:bin.ji
//  Note:2004.11.8
/*****************************************************************************/
LOCAL void CC_OpenCallProcessingWin(void)
{
    MMI_STRING_T    str = {0};

    CHECK_CC_APPLET_NOT_NULL
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenCallProcessingWin()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8111_112_2_18_2_10_57_333,(uint8*)"");
#ifdef MMI_CALL_MINIMIZE_SUPPORT
    if(CC_INSTANCE.is_minimize)
    {
        return;
    }
 #endif   
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str);
    
    //判断cc window是否存在
    if( !MMK_IsOpenWin( MMICC_PROCESSING_WIN_ID ) )
    {
//        OpenProcessingWin(MMICC_PROCESSING_WIN_ID, prompt_str, IMAGE_NULL, ANIM_PUBWIN_WAIT, CC_HandleCcProcessingWinMsg);
        MMIPUB_OpenWaitWinEx(CC_APPLET_HANDLE, 1, &str, PNULL, PNULL, MMICC_PROCESSING_WIN_ID,IMAGE_NULL,
                                                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,(MMIPUB_HANDLE_FUNC)CC_HandleCcProcessingWinMsg, PNULL);
        
        // init the softkey
        SetHandFreeMenu(MMICC_PROCESSING_WIN_ID,TXT_NULL,TXT_NULL,TXT_NULL,FALSE);        
    }
}

/*****************************************************************************/
//  Description :display when received disconnect signal
//  Global resource dependence : none
//  Author:figo.feng
//  Note: modify by louis.wei 2004-01-14
/*****************************************************************************/
LOCAL void CC_DisplayDisconnectWin(
                                   uint8							call_index,
                                   MN_CALL_DISCONNECT_CALL_CAUSE_E	cause,
                                   uint32 call_time
                                   )
{
    MMI_STRING_T			prompt_string[2] = {0};
    wchar                   temp_wchar[CC_DISPLAY_TIME_LEN + 3] = {0};
    CC_MN_CALL_DISC_CAUSE_E cause_type = CC_DISC_CAUSE_NORMAL;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_DisplayDisconnectWin():call_index=%d, cause=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8146_112_2_18_2_10_57_334,(uint8*)"dd", call_index, cause);
    SCI_MEMSET( &prompt_string[0], 0, sizeof(prompt_string) );
    
    //============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT            
    if(MMIMEX_IsOutCalling())
    {
        MMIMEX_OutCallEndedCB(NULL);
        //process started by mex application, should not go on.
        return;
    }
#endif 
    
    cause_type = MMICC_ShowCallDiscCause(cause);

    //非本方挂断,需要发挂断音提示用户
    if (CC_RELEASE_OPER != MMICC_GetCallOperatedByIndex(call_index)
        && CC_INCOMING_CALL_STATE != MMICC_GetCallStatusByIndex(call_index)
        && CC_WAITING_STATE != MMICC_GetCallStatusByIndex(call_index))
    {
        MMICC_SetIsPlayCallEndHintFlag(TRUE);
    }
    else
    {
        MMICC_SetIsPlayCallEndHintFlag(FALSE);
    }
    
#ifdef MMI_CLEAR_CODE_SUPPORT
    //merge telcel
    if(cause <= MN_CAUSE_INTERWORKING)
    {
//lzk mark
#ifdef BT_DIALER_SUPPORT
        uint8					disc_str[CC_CALL_CLEAR_CODE_STR_MAX_LEN] = {0};
        wchar                   promp_disc_wchar[CC_CALL_CLEAR_CODE_STR_MAX_LEN + 1] = {0};
        switch(cause)
        {
        case MN_CAUSE_UNASSIGNED_NO://1
        case MN_CAUSE_NUMBER_CHANGED://22
        case MN_CAUSE_INVALID_NO_FORMAT://28
            sprintf((char*)disc_str,"MARCACION NO DISPNOIBLE -%02d-",cause);
            break;
            
        case MN_CAUSE_NO_ROUTE_TO_DEST://3
        case MN_CAUSE_OPER_DETERM_BARRING://8
        case MN_CAUSE_CALL_REJECTED://21
        case MN_CAUSE_DEST_OUT_OF_ORDER://27
            sprintf((char*)disc_str,"USUARIO NO DISPONIBLE -%02d-",cause);
            break;
            
        case MN_CAUSE_NO_USER_RESPONDING://18
        case MN_CAUSE_ALERTING_NO_ANSWER://19
            sprintf((char*)disc_str,"USUARIO NO RESPONDE -%02d-",cause);
            break;
            
        case MN_CAUSE_NORMAL_CLEARING://16
            sprintf((char*)disc_str,"--sin texto--");
            break;
            
        case MN_CAUSE_USER_BUSY://17
            sprintf((char*)disc_str,"USUARIO OCUPADO");
            break;
            
        default://other case
            sprintf((char*)disc_str,"LLAMADA TERMINADA -%02d-",cause);
            break;
        }
        
        prompt_string[1].wstr_ptr = promp_disc_wchar;
        prompt_string[1].wstr_len = strlen((char*)disc_str);
        MMI_STRNTOWSTR(prompt_string[1].wstr_ptr,
            CC_CALL_CLEAR_CODE_STR_MAX_LEN,
            disc_str,
            CC_CALL_CLEAR_CODE_STR_MAX_LEN,
            prompt_string[1].wstr_len);       
#else
    prompt_string[CC_DISPALY_INFO_NAME] = GetClearCodeString(cause); 
#endif 
    }
    else
#endif
    {
        //更新显示
        if ((CC_DISC_CAUSE_NORMAL == cause_type)//normal cause
            ||(0 != call_time)  // call time not zero
            ||(CC_RELEASE_OPER == MMICC_GetCallOperatedByIndex(call_index)) //release by user
            ||(CC_MT_CALL_TYPE == MMICC_GetCallDirection( call_index))) //only MO display disconnect cause
        {
            //SCI_TRACE_LOW:"CC_DisplayDisconnectWin call_state = %d, call_time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8179_112_2_18_2_10_57_335,(uint8*)"dd", MMICC_GetCallStatusByIndex( call_index), call_time);
            if (CC_CALL_STATE == MMICC_GetCallStatusByIndex(call_index)
                || CC_HOLD_STATE == MMICC_GetCallStatusByIndex(call_index))
            {
                //更新显示
                prompt_string[1].wstr_ptr = temp_wchar;
                prompt_string[1].wstr_len = sizeof(temp_wchar)/sizeof(wchar);
                CC_TimeCountToStr(&prompt_string[1], call_time);
            }
            else //havn't set up call yet, just prompt without call time
            {
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_END,  &prompt_string[1]);
            }
        }
        else
        {
            if (((cause_type > CC_DISC_CAUSE_NORMAL) && (cause_type <= CC_DISC_CAUSE_BEING_EXCEED_LIMIT)))
            {
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)s_cc_disc_cause[cause_type - 1],  &prompt_string[1]);
            }
        }
    }    
        
    MMICC_GetCallNameStrByIndex(&prompt_string[0], call_index);
    
    //判断disconnect window是否存在
    if ( MMK_IsOpenWin( MMICC_DISCONNECT_WIN_ID ) )
    {
        SetProcessingWinText(TRUE, MMICC_DISCONNECT_WIN_ID, prompt_string);
        GUIWIN_UpdateStbIcon();
    }
    //when usb is running, not display disc win
    else if (MMIAPIUDISK_UdiskIsRun()&&MMIAPICC_IsVideoCall())
    {
        return;
    }
    else
    {
#if (!defined MMI_GUI_STYLE_TYPICAL)// && (!defined MMI_CALLCONTROL_MINI_SUPPORT)
        OpenProcessingWin(MMICC_DISCONNECT_WIN_ID, prompt_string, IMAGE_NULL, IMAGE_NULL, CC_HandleDisconnectWinMsg);
#else
        OpenProcessingWin(MMICC_DISCONNECT_WIN_ID, prompt_string, IMAGE_CALL_END, IMAGE_NULL, CC_HandleDisconnectWinMsg);
#endif
    }
    MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
    
    //@samboo.shen add 20070727 vt电话回落时不能关闭呼出窗口
    MMK_CloseWin( MMICC_ANIMATION_WIN_ID );
    MMK_CloseWin(MMICC_ADJUSTVOLUME_WIN_ID);
    
    if( 0 == CC_INSTANCE.mmicc_disconnect_timer )
    {
        CC_INSTANCE.mmicc_disconnect_timer = MMK_CreateWinTimer(MMICC_DISCONNECT_WIN_ID, CC_DISCONNECT_TIMER_OUT, FALSE);
    }
    else
    {
        MMK_StartWinTimer(
            MMICC_DISCONNECT_WIN_ID,
            CC_INSTANCE.mmicc_disconnect_timer,
            CC_DISCONNECT_TIMER_OUT, FALSE
            );
    }
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏，显示总共通话时间
    MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_CALL,&prompt_string[1]);
#endif
}

/*****************************************************************************/
//  Description : check is in cc is on top
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsCcOnTopWin(void)
{
    if ((MMIAPICC_IsInState(CC_IN_CALL_STATE)
            &&(MMK_IsFocusWin(MMICC_STATUS_WIN_ID)
                ||MMK_IsFocusWin(MMICC_ANIMATION_WIN_ID)
                ||MMK_IsFocusWin(MMICC_PROCESSING_WIN_ID)
                ||MMK_IsFocusWin(MMICC_MENU_WIN_ID)
                ||MMK_IsFocusWin(MMICC_CALLLIST_WIN_ID)
                ||MMK_IsFocusWin(MMICC_HOLDMENU_WIN_ID)
                ||MMK_IsFocusWin(MMICC_ADJUSTVOLUME_WIN_ID)
                ||MMK_IsFocusWin(MMICC_HOLDCNF_PROMPT_WIN_ID)
                ||MMK_IsFocusWin(MMICC_SEND_DTMF_WIN_ID)
                ||MMK_IsFocusWin(MMICC_SEND_DTMF_W_WIN_ID)
                ||MMK_IsFocusWin(MMICC_EMERGENCY_WIN_ID)
                ||MMK_IsFocusWin(MMICC_MULTICALL_LIST_WIN_ID)
                ||MMK_IsFocusWin(MMICC_AUTO_DIAL_WIN_ID)
                ))
        ||(!MMIAPICC_IsInState(CC_IN_CALL_STATE)
            &&(MMK_IsFocusWin(MMICC_DISCONNECT_WIN_ID)
                ||MMK_IsFocusWin(MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID)
                ))
        ||(MMK_IsFocusWin(MMICC_QUERY_SWITCH_TO_HANDFREE_WIN_ID)
                ||MMK_IsFocusWin(MMICC_DETACHCNF_WAIT_WIN_ID
                ))
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : is allow Pormpt Win displayed on top
//  Global resource dependence : 
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsPermitPromptWinOnTop(BOOLEAN is_lockwin_show_prompt)
{
    if( MMIAPIUdisk_IsOpenPcCamera() 
#ifdef MMI_RECORD_SUPPORT
        || IsCCRecording() 
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif  
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
        || MMIAPIDM_GetHandsetLock()        
        || MMIAPIDC_IsOpened()
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_CALLCONTROL_MINI_SUPPORT)        
        || (MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID) && !is_lockwin_show_prompt)
#elif MMI_KEY_LOCK_SUPPORT
        || (MMIAPIKL_IsPhoneLocked() && !is_lockwin_show_prompt)
#endif
        || MMIAPIATV_IsRecording()
#if defined(MMI_PDA_SUPPORT)
        || (MMIAPICC_IsInState(CC_IN_CALL_STATE))
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT        
	|| MMK_IsFocusWin(MMIKL_PSW_WIN_ID)
#endif
#endif
        )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description :display when there is missed call
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenMissedCallWin(void)
{
    uint8				missed_call_str[32] = {0};
    uint8				char_num_len = 0;
    uint8				copy_len = 0;
    uint32              time_out = 0;
    MMI_STRING_T		tmp_prompt = {0};
    MMI_STRING_T		prompt_str = {0};
    wchar               temp_wchar[33] = {0};
    MMI_WIN_PRIORITY_E	win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T		alert_win_id = MMICC_MISSCALL_WIN_ID;
    
    prompt_str.wstr_ptr = temp_wchar;
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenMissedCallWin()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8301_112_2_18_2_10_58_336,(uint8*)"");
    
    SCI_MEMSET(&prompt_str, 0, sizeof(prompt_str));
    SCI_MEMSET(&tmp_prompt, 0, sizeof(tmp_prompt));
    
    sprintf((char*)missed_call_str, "%d", (int)MMIAPICC_GetMissedCallNumber());
    char_num_len = strlen((char*)missed_call_str);
    if(char_num_len >= sizeof(missed_call_str))
    {
        //SCI_TRACE_LOW:"ccapp error char_num_len = %d, missed_call_str = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8310_112_2_18_2_10_58_337,(uint8*)"dd", char_num_len, sizeof(missed_call_str));
        return;
    }

    if(1 == MMIAPICC_GetMissedCallNumber())
    {
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_CC_MISS_CALL , &tmp_prompt);
    }
    else
    {
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_COMMON_MISS_CALLS , &tmp_prompt);
    }    
    
    {
        const wchar blank_char =' ';
        const uint8 blank_len = 1;
        uint8 wstr_len =  char_num_len + tmp_prompt.wstr_len + blank_len;
        
        prompt_str.wstr_ptr = SCI_ALLOCAZ((wstr_len + 1)*sizeof(wchar));
        if( PNULL != prompt_str.wstr_ptr)
        {
            prompt_str.wstr_len = 0;
        
            copy_len = ( char_num_len > wstr_len) ? (wstr_len ): char_num_len;
            MMI_STRNTOWSTR(prompt_str.wstr_ptr,wstr_len,(uint8 *)missed_call_str,sizeof(missed_call_str),copy_len);
            prompt_str.wstr_len = copy_len;

        
            MMI_WSTRNCPY(prompt_str.wstr_ptr + prompt_str.wstr_len, blank_len, &blank_char, blank_len, blank_len);
            prompt_str.wstr_len += blank_len;
        
            copy_len = ( (tmp_prompt.wstr_len + prompt_str.wstr_len + blank_len) > wstr_len) ? (wstr_len - prompt_str.wstr_len ): tmp_prompt.wstr_len;
            SCI_ASSERT(wstr_len >= char_num_len);/* assert verified*/
            MMI_MEMCPY(
                (void*)(prompt_str.wstr_ptr + prompt_str.wstr_len),
                (wstr_len - char_num_len + 1)*sizeof(wchar),
                tmp_prompt.wstr_ptr,
                (tmp_prompt.wstr_len + 1)*sizeof(wchar),
                (copy_len + 1)*sizeof(wchar)
                );
            prompt_str.wstr_len += copy_len;
        }
        else
        {
            //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenMissedCallWin() mem alloc FAILED"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8354_112_2_18_2_10_58_338,(uint8*)"");
            return;
        }
    }

    
    //判断missed call window是否存在
    
    MMK_CloseWin( MMICC_MISSCALL_WIN_ID );
#ifdef SCREENSAVER_SUPPORT 
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif
    if(MMIAPICC_IsPermitPromptWinOnTop(FALSE))
    {
        win_priority = WIN_ONE_LEVEL;
    }
    
    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWCALL,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,HandleMissedcallWinMsg);
    MMIAPICC_SendMissedCallNumberToKeylock();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏，显示未接来电
    MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_MISSED_CALL,&prompt_str);
#endif    
    SCI_FREE( prompt_str.wstr_ptr );
}

/*****************************************************************************/
// 	Description : to append the info to listbox
//	Global resource dependence : 
//  Author:louis.wei
//	Note: 
/*****************************************************************************/
LOCAL void AppendListItem(									
                          uint8 					call_id,
                          MMI_CTRL_ID_T	ctrl_id
                          )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMI_STRING_T		    item_str = {0};
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};    
    uint8 tel_num_len = 0;
    wchar temp_wchar[CC_MAX_PBNAME_LEN+1] = {0};
    
    if(!MMICC_GetCallNameStrByIndex(&item_str, call_id))
    {
        if(MMICC_GetCallNumStrByIndex(tele_num, &tel_num_len, call_id))
        {
            if (0 != tel_num_len)
            {
                item_str.wstr_ptr = temp_wchar;
                item_str.wstr_len = tel_num_len;
                MMI_STRNTOWSTR(item_str.wstr_ptr, item_str.wstr_len, tele_num,  tel_num_len, tel_num_len);
                item_str.wstr_len = MMIAPICOM_Wstrlen(item_str.wstr_ptr);            
            }
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_UNKNOW_NUM, &item_str);
        }
    }

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = item_str.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = item_str.wstr_ptr;
    item_data.softkey_id[0] = TXT_CC_HANG_OFF;
    if (MMICC_IsMptySplitAvailable())     
    {
#ifdef GUI_MID_SOFTKEY_USE_IMG
         item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_SPLIT_ICON;
#else
        item_data.softkey_id[1] = TXT_CC_PRIVATE;
#endif
    }
    else
    {
#ifdef MMI_GUI_STYLE_MINISCREEN
        item_data.softkey_id[1] = IMAGE_NULL;
#else
        item_data.softkey_id[1] = TXT_NULL;
#endif  
    }
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}


/*****************************************************************************/
//  Description :update a call processing text (releasing, holding, spliting, etc)
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.8
/*****************************************************************************/
PUBLIC void MMICC_UpdateCurrentCallProcessing(void)
{
    MMI_STRING_T    state_str = {0};
    
    if(!MMICC_IsCurrentCallIDValid())
    {
        return;
    }
#ifdef MET_MEX_SUPPORT
	if(MMIMEX_IsOutCalling())
	{
	    //process started by mex application, should not go on.            
         return;
	}
#endif 	
#ifdef MMI_GUI_STYLE_TYPICAL    
    if(GetCallStateText(&state_str))
    {
        CC_OpenCallProcessingWin(); //open CallProcessingWin
//        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);  //关闭ANIMATION_WIN窗口
        
        CC_CallStateDisplaying( state_str.wstr_len, state_str.wstr_ptr);
        
        GUIWIN_UpdateStbIcon();
        
        CC_FreeSimStatusSpace();
    }
#else //MMI_GUI_STYLE_TYPICAL
    if((CC_CALL_STATE != MMICC_GetCurrentCallStatus()) && GetCallStateText(&state_str))
    {
        //刷新主屏和副屏
        if (0 != MMK_GetCtrlHandleByWin(MMICC_STATUS_WIN_ID, MMICC_CONNECTED_STATE_LABEL_CTRL_ID))
        {
            //fresh list time
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &state_str, TRUE);   
        }
    }
#endif //MMI_GUI_STYLE_TYPICAL
}

/*****************************************************************************/
//  Description :update a call status text (hold, active)
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.8
/*****************************************************************************/
PUBLIC void MMICC_UpdateCurrentCallStatus(BOOLEAN	is_flash)
{
    MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;
    MMI_STRING_T    state_str = {0};
    MMI_STRING_T    name_str = {0};
    MMI_STRING_T	text_arr[CC_DISPALY_INFO_MAX] = {0};
    wchar temp_wchar[CC_DISPLAY_TIME_LEN + 3] = {0};
    uint32 cur_time_count = 0;

#ifndef MMI_GUI_STYLE_TYPICAL
    uint16			city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH] = {0};
    wchar                      temp_tel_num[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
    uint8			tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			tel_num_len = 0;
#endif

#ifdef MET_MEX_SUPPORT
    if (MMIMEX_IsOutCalling())
    {
        //process started by mex application, should not go on.            
         return;
    }
#endif

    if(!MMICC_IsCurrentCallIDValid())
    {
        return;
    }
    cur_time_count = MMICC_GetCurrentCallTime();

    if(GetCallStateText(&state_str))
    {        
        CC_CallStateDisplaying( state_str.wstr_len, state_str.wstr_ptr);
        text_arr[CC_DISPALY_INFO_STATE].wstr_len = CC_INSTANCE.sim_status.wstr_len;
        text_arr[CC_DISPALY_INFO_STATE].wstr_ptr = CC_INSTANCE.sim_status.wstr_ptr;

        SCI_MEMSET(&name_str, 0, sizeof(name_str));
#ifndef MMI_GUI_STYLE_TYPICAL
        if (MMICC_IsExistActiveMPTY())
#else
        if (MMICC_IsExistMPTY())
#endif
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_MULTICALL, &name_str);
        }
        else
        {
            MMICC_GetCallNameStrByIndex(&name_str, MMICC_GetCurrentCallIndex());
        }

#ifndef MMI_GUI_STYLE_TYPICAL
        if (MMICC_GetCallNameStrByIndex(PNULL, MMICC_GetCurrentCallIndex()))
#endif
        {
            text_arr[CC_DISPALY_INFO_NAME].wstr_len = name_str.wstr_len;
            text_arr[CC_DISPALY_INFO_NAME].wstr_ptr = name_str.wstr_ptr;
        }

        text_arr[CC_DISPALY_INFO_TIME].wstr_ptr = temp_wchar;
        text_arr[CC_DISPALY_INFO_TIME].wstr_len = sizeof(temp_wchar)/sizeof(wchar);
        CC_TimeCountToStr(&text_arr[CC_DISPALY_INFO_TIME], cur_time_count);
        
#ifndef MMI_GUI_STYLE_TYPICAL
        MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, MMICC_GetCurrentCallIndex());
        
        //get info for display call number
        if (tel_num_len >0)
        {
            text_arr[CC_DISPALY_INFO_NUMBER].wstr_ptr = temp_tel_num;
            text_arr[CC_DISPALY_INFO_NUMBER].wstr_len = tel_num_len;
            MMI_STRNTOWSTR(text_arr[CC_DISPALY_INFO_NUMBER].wstr_ptr,
                text_arr[CC_DISPALY_INFO_NUMBER].wstr_len,
                (uint8 *)tel_num,
                tel_num_len,
                tel_num_len);
        }
        else
        {
            //the number wstr_len is 0.display unknown number info
            MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &text_arr[CC_DISPALY_INFO_NUMBER]);
        }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT         
        if (MMICC_IsExistActiveCall())
        {
            text_arr[CC_DISPALY_INFO_STATE].wstr_ptr = temp_wchar;
            text_arr[CC_DISPALY_INFO_STATE].wstr_len = sizeof(temp_wchar)/sizeof(wchar);
            CC_TimeCountToStr(&text_arr[CC_DISPALY_INFO_STATE], cur_time_count);
        }
        else if (MMICC_IsExistHoldCall())
        {
            //do nothing
        }

        text_arr[CC_DISPALY_INFO_TIME].wstr_ptr = PNULL;
        text_arr[CC_DISPALY_INFO_TIME].wstr_len = 0;
#endif

        if (1 == MMICC_GetCallNum()
        || (MMICC_IsExistActiveCall() && !MMICC_IsExistActiveMPTY()))
        {
            CC_FindLocationInfor(&(text_arr[CC_DISPALY_INFO_LOCATION]), city_ucs2_str, tel_num, tel_num_len );
        }
#endif
            //判断cc window是否存在
#ifdef PDA_UI_DROPDOWN_WIN
#ifdef MMI_CALL_MINIMIZE_SUPPORT
        //mo
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_CALLING);
#endif
#endif              
        if( !MMK_IsOpenWin( MMICC_STATUS_WIN_ID ) )
        {

            OpenConnectedWin(text_arr, CC_HandleCcWinMsg
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_CALLCONTROL_MINI_SUPPORT               
                ,TRUE,
                MMI_CALL_STATE_TEXT_COLOR
#endif
#endif
                );
#ifndef MMI_GUI_STYLE_TYPICAL
 #ifdef MMI_CALLCONTROL_MINI_SUPPORT
    DisplayCallAnimPhotoForCommon(MMICC_STATUS_WIN_ID,IMAGE_CC_CALL_PHOTO_ANIM);
    GUIANIM_PauseAnim(MMICC_BOTTOM_PHOTO_ANIM_CTRL_ID);
 #endif
 #endif
        }
        else
        {
#ifndef  MAINLCD_SIZE_128X64      
            if (MMICC_IsExistActiveAndHoldCalls())
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                PdaDisplayMultiCall(win_id, text_arr);
#else
                DisplayCallInfoForMulti();
#endif
            }
            else
#endif                
            {
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
		  if(CC_HOLD_STATE == MMICC_GetCurrentCallStatus())
           	 {
           	    PdaDisplaySingleCall(win_id, text_arr,FALSE,MMI_CALL_HOLD_COLOR);
           	 }
          	else
          	{
          	    PdaDisplaySingleCall(win_id, text_arr,TRUE,MMI_CALL_STATE_TEXT_COLOR);
          	}
            DisplayCallAnimPhotoForCommon(win_id,IMAGE_CC_CALL_PHOTO_ANIM);
            GUIANIM_PauseAnim(MMICC_BOTTOM_PHOTO_ANIM_CTRL_ID);
 #else  
                PdaDisplaySingleCall(win_id, text_arr);
 #endif
 
#else
                DisplayCallInfoForSingle(text_arr, is_flash);
#endif
            }

#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
            //update hold hot button
         {
                CC_CC_BUTTON_E button_type = CC_BUTTON_HOLD;

                if ( MMICC_IsExistActiveAndHoldCalls())
                {
                    button_type = CC_BUTTON_SWAP_CALLS;
                }
                else if(MMICC_IsExistHoldCall())
                {
                    button_type = CC_BUTTON_RETRIEVE_CALLS;
                }
                else
                {
                    button_type = CC_BUTTON_HOLD;
                }                
                CC_CreateNewHotButtonWithoutForm(win_id, HB_HOLD_ROW, HB_HOLD_COLUMN, button_type);            
            }
#endif
            
#endif

            //刷新状态栏
            if (is_flash)
            {
                GUIWIN_UpdateStbIcon();  
                if(MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                }
            }
        
            if(MMICC_IsExistOperatedCall())
            {
                MMIPUB_SetWinSoftkey(win_id,TXT_NULL,TXT_NULL,FALSE);

                //return的时候释放内存
                CC_FreeSimStatusSpace();
                return;
            }
        }
        MMK_CloseWin(MMICC_PROCESSING_WIN_ID);  //关闭processing窗口
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);  //关闭ANIMATION_WIN窗口
    
        CC_FreeSimStatusSpace();
    }

    SetStatusWinSoftkeyByCallState();
    
    //if only hold calls being, enable downlink mute to avoid some unnessary network volume
    MMICC_HandleDownlinkChanel(); //cr 126367 cr 134974
}


/*****************************************************************************/
// 	Description : SetStatusWinSoftkeyByCallState
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void SetStatusWinSoftkeyByCallState(void)
{
MMI_WIN_ID_T win_id = MMICC_STATUS_WIN_ID;

    switch( MMICC_GetCurrentCallStatus()) 
    {
    case CC_CALLING_STATE:        
        SetHandFreeMenu(win_id,TXT_NULL,TXT_NULL,TXT_NULL,FALSE);
        break;

    case CC_WAITING_STATE:
        if( 1 == MMICC_GetCallNum())
        {
            SetHandFreeMenu(win_id,STXT_ANSWER,TXT_NULL,TXT_NULL,FALSE);
        }
        else
        {
            if(!MMIAPIDM_GetHandsetLock())
            {
                SetHandFreeMenu(win_id,STXT_MENU,TXT_NULL,TXT_NULL,FALSE);
            }
            else
            {
                SetHandFreeMenu(win_id,TXT_NULL,TXT_NULL,TXT_NULL,FALSE);
            }
        }
        break;

    default:
        if(!MMIAPIDM_GetHandsetLock())
        {
            SetHandFreeMenu(win_id,STXT_MENU,TXT_NULL,TXT_NULL,FALSE);
        }
        else
        {
            SetHandFreeMenu(win_id,TXT_NULL,TXT_NULL,TXT_NULL,FALSE);
        }
        break;
    }

}
/*****************************************************************************/
// 	Description : the process message function of the auot dialing  window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoDialWinMsg( 
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static 	uint8	s_timer_id = 0;
    static  uint8	s_count = 0;
    uint8		tele_len = 0;
    uint8		tele_num[ CC_MAX_TELE_NUM_LEN + 2] = {0};
    MMI_STRING_T prompt_arr[CC_DISPALY_INFO_MAX] = {0};
    
    
    if (MSG_TIMER != msg_id)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function MMICC_HandleAutoDialWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8709_112_2_18_2_10_59_339,(uint8*)"d", msg_id);
    }
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        DisplayWinBg(win_id, FALSE);
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_UpdateCCWinHotButton(CC_BUTTON_MAX, FALSE);
#endif
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    case MSG_NOTIFY_UPDATE:
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            GUI_RECT_T* update_rect_ptr = (GUI_RECT_T*)param;
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
#endif
        }
        break;
        
    case MSG_TIMER:
        //received signal of timer when timer of dialing is time out
        if( s_timer_id == *(uint8*)param)
        {
            s_count++;
#ifndef MMI_GUI_STYLE_TYPICAL
            MMICC_GetWantCallNameStr(&prompt_arr[CC_DISPALY_INFO_NAME]);
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_CC_REDIALING1+(s_count)%3), &prompt_arr[CC_DISPALY_INFO_STATE]);
            
            GetRedialedTimes(&prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES]);

            GUILABEL_SetText(MMICC_CONNECTED_REDIAL_TIMES_LABEL_CTRL_ID, &prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES], TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &prompt_arr[CC_DISPALY_INFO_STATE], TRUE);
            GUIWIN_UpdateStbIcon();

            if (PNULL != prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES].wstr_ptr)
            {
                SCI_FREE(prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES].wstr_ptr);
                prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES].wstr_ptr = PNULL;
            }
#else
            MMICC_GetWantCallNameStr(&prompt_arr[0]);
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_CC_REDIALING1+(s_count)%3), &prompt_arr[1]);
            
            GetRedialedTimes(&prompt_arr[2]);
 
            SetCallingWinText(TRUE, prompt_arr);
            GUIWIN_UpdateStbIcon();
            
            if (PNULL != prompt_arr[2].wstr_ptr)
            {
                SCI_FREE(prompt_arr[2].wstr_ptr);
                prompt_arr[2].wstr_ptr = PNULL;
            }
#endif
            
            if (CC_INSTANCE.autodial_info.num >= 10)
            {
                ClearAutoRedialInfo();
                MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
            }
            else if (s_count == (s_redial_timeout[CC_INSTANCE.autodial_info.num] / CC_REDIAL_TIMER_TIP))
            {   
                s_count = 0;
                CC_INSTANCE.autodial_info.num++;
                
                SCI_MEMSET( (void*)tele_num, 0, sizeof(tele_num));

                MMICC_GetWantCallNumStr(tele_num, &tele_len);
                //@samboo.shen add 20070726 cr88717
                if(MMI_RESULT_FALSE == MMICC_MakeRedialCall(MMICC_GetWantCallDualSys(), tele_num, tele_len))
                {
                    recode = MMI_RESULT_FALSE;
                    MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
                    
                    return recode;		            
                }
                MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        SCI_MEMSET( (void*)tele_num, 0, sizeof(tele_num));
        
        {          
            s_count = 0;
            CC_INSTANCE.autodial_info.num++;
            
            SCI_MEMSET( (void*)tele_num, 0, sizeof(tele_num));

            MMICC_GetWantCallNumStr(tele_num, &tele_len);
            //@samboo.shen add 20070726 cr88717
            if(MMI_RESULT_FALSE == MMICC_MakeRedialCall(MMICC_GetWantCallDualSys(), tele_num, tele_len))
            {
                recode = MMI_RESULT_FALSE;
                MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
                
                return recode;		            
            }
            MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer( s_timer_id );
        s_timer_id = 0;
        s_count = 0;
        //s_is_open_auto_dial_win = FALSE;
        
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        
        MMIAPISMS_CheckTimerMSG();

        if (0 == MMICC_GetCallNum()
            && !MMICC_IsExistOutgoingCall())
        {
            //MMICC_StopCCApplet();
            MMICC_DestroyAudioHandle();
            MMK_PostMsg( MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
        }
        
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );		        
        break;
        
    case MSG_CTL_CANCEL: 
    case MSG_APP_CANCEL:
        MMICC_ClearWantCallInfo();     
        ClearAutoRedialInfo();
        
        if (0 == MMICC_GetCallNum())
        {
            //最后一通电话挂断, 弹出被延迟的闹钟
            if (!MMIAPIALM_CheckAndStartDelayedAlarm())
            {//@zhaohui,counted time
                MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
            }
            // @baokun fix 93989 end
            
            // add by shengjie.liu for cr99360
            MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(MMICC_GetWantCallDualSys()));
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        MMICC_ClearWantCallInfo();
        ClearAutoRedialInfo();
        
        if( 0 == MMICC_GetCallNum())
        {
            //最后一通电话挂断, 弹出被延迟的闹钟
            if (!MMIAPIALM_CheckAndStartDelayedAlarm())
            {//@zhaohui,counted time
                MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
            }
            // @baokun fix 93989 end
            
            // add by shengjie.liu for cr99360
            MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(MMICC_GetWantCallDualSys()));
        }
        MMK_CloseWin(win_id);
        MMK_ReturnIdleWin();
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
        break;
        
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        s_timer_id = MMK_CreateTimer(CC_REDIAL_TIMER_TIP, TRUE);
        break;        

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
        HandleCCWinPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif        
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
        ConstructDialButton(win_id);
        ConstructEndButton(win_id);
#endif        
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_CreateHotButtonsWithoutForm(win_id);
        CC_UpdateHotButtonState();
#endif
#endif
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, FALSE);
        if (CC_INSTANCE.autodial_info.num < CC_INSTANCE.autodial_info.num_max)
        {
            s_count = 0;
            s_timer_id = MMK_CreateTimer(CC_REDIAL_TIMER_TIP, TRUE);
        }
        //s_is_open_auto_dial_win = TRUE;
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR        
        //增加状态栏
        CreateStatusBarCtrl(win_id, IMAGE_NULL);
#endif
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );	
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        //SCI_TRACE_LOW:"ccapp.c: MMICC_HandleAutoDialWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8916_112_2_18_2_10_59_340,(uint8*)"d", msg_id);
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : GetRedialedTimes
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void GetRedialedTimes(MMI_STRING_T *redialed_times_ptr)
{
    MMI_STRING_T redialed_str = {0};
    MMI_STRING_T times_str = {0};
    uint8 redialed_times[10] = {0};
    uint16 length = 0;
    
    if(PNULL == redialed_times_ptr)
    {
        //SCI_TRACE_LOW:"GetRedialedTimes redialed_times_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8935_112_2_18_2_11_0_341,(uint8*)"");
        return;
    }
    
    MMI_GetLabelTextByLang(TXT_CC_REDIALED, &redialed_str);
    MMI_GetLabelTextByLang(TXT_CC_REDIAL_TIMES, &times_str);
    
    sprintf((char *)redialed_times, " %d ", CC_INSTANCE.autodial_info.num + 1);
    
    length = strlen((char *)redialed_times);
    
    redialed_times_ptr->wstr_ptr = SCI_ALLOCAZ((redialed_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar));
    if (PNULL == redialed_times_ptr->wstr_ptr)
    {
        return;
    }
    
    //SCI_MEMSET(redialed_times_ptr->wstr_ptr, 0x00, ((redialed_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar)));
    
    MMI_WSTRNCPY(redialed_times_ptr->wstr_ptr, redialed_str.wstr_len, redialed_str.wstr_ptr, redialed_str.wstr_len, redialed_str.wstr_len);
    redialed_times_ptr->wstr_len = redialed_str.wstr_len;
    
    MMI_STRNTOWSTR(redialed_times_ptr->wstr_ptr+redialed_times_ptr->wstr_len, length, (uint8 *)redialed_times, 10, length);
    redialed_times_ptr->wstr_len += length;
    
    MMI_WSTRNCPY((redialed_times_ptr->wstr_ptr+redialed_times_ptr->wstr_len), times_str.wstr_len, times_str.wstr_ptr, times_str.wstr_len, times_str.wstr_len);
    redialed_times_ptr->wstr_len += times_str.wstr_len;
}

/*****************************************************************************/
// 	Description : whether the call need to dial again
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_OpenAutoDialWindow ( 
                                  void
                                  )
{
    MMI_STRING_T prompt_arr[CC_DISPALY_INFO_MAX] = {0};
#ifndef MMI_GUI_STYLE_TYPICAL
    uint8       tele_num[ CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8       tele_len = 0;
    wchar   temp_tel_num[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
    uint16			city_ucs2_str[MMIFINDMP_CITYNAME_LENGTH] = {0};
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenAutoDialWindow()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_8979_112_2_18_2_11_0_342,(uint8*)"");
    
    //判断disconnect window是否存在
    MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );

    MMICC_GetWantCallNameStr(&prompt_arr[CC_DISPALY_INFO_NAME]);


    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_REDIALING1, &prompt_arr[CC_DISPALY_INFO_STATE]);
    
    //s_is_open_auto_dial_win = TRUE;
    
    GetRedialedTimes(&prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES]);

    MMICC_GetWantCallNumStr(tele_num, &tele_len);

    prompt_arr[CC_DISPALY_INFO_NUMBER].wstr_ptr = temp_tel_num;
    prompt_arr[CC_DISPALY_INFO_NUMBER].wstr_len = tele_len;

    //prompt_str_array[2].wstr_ptr = tel_num;
    MMI_STRNTOWSTR(prompt_arr[CC_DISPALY_INFO_NUMBER].wstr_ptr,
        prompt_arr[CC_DISPALY_INFO_NUMBER].wstr_len,
        (uint8 *)tele_num,
        tele_len,
        tele_len);
        
    CC_FindLocationInfor(&(prompt_arr[CC_DISPALY_INFO_LOCATION]), city_ucs2_str, tele_num, tele_len);

    OpenCallingWin(MMICC_AUTO_DIAL_WIN_ID, prompt_arr, IMAGE_NULL, HandleAutoDialWinMsg);

#ifdef MMI_CALLCONTROL_MINI_SUPPORT
    DisplayCallAnimPhotoForCommon(MMICC_AUTO_DIAL_WIN_ID,IMAGE_CC_CALLING_PHOTO_ANIM);
#endif
 
    if (PNULL != prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES].wstr_ptr)
    {
        SCI_FREE(prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES].wstr_ptr);
        prompt_arr[CC_DISPALY_INFO_AUTODIAL_TIMES].wstr_ptr = PNULL;
    }
#else 
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenAutoDialWindow()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9015_112_2_18_2_11_0_343,(uint8*)"");
    
    //判断disconnect window是否存在
    MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );

    MMICC_GetWantCallNameStr(&prompt_arr[0]);
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_REDIALING1, &prompt_arr[1]);
    
    //s_is_open_auto_dial_win = TRUE;
    
    GetRedialedTimes(&prompt_arr[2]);

    OpenCallingWin(MMICC_AUTO_DIAL_WIN_ID, prompt_arr, IMAGE_REPEAT_MO_CALL_PIC, HandleAutoDialWinMsg);

    if (PNULL != prompt_arr[2].wstr_ptr)
    {
        SCI_FREE(prompt_arr[2].wstr_ptr);
        prompt_arr[2].wstr_ptr = PNULL;
    }
#endif
   
}

/*****************************************************************************/
//  Description : 停止录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_StopCCRecord(void)
{
#ifdef MMI_RECORD_SUPPORT
	if (PNULL != s_cc_record_handle)
	{
		MMIRECORDSRV_StopRecord(s_cc_record_handle);
		MMIRECORDSRV_FreeRecordHandle(s_cc_record_handle);
		s_cc_record_handle = 0;
	}
#endif

}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : NotifyCallback
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void NotifyCallback(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result)
{

	SCI_TRACE_LOW("[CC RECORD] NotifyCallback record_srv_handle = %d, s_record_info.record_srv_handle = %d, result = %d, support_type = %d",record_srv_handle,s_cc_record_handle,result, support_type);
	if (record_srv_handle == s_cc_record_handle
		&& MMIRECORD_SRV_SUPPORT_RECORD == support_type)
	{			
		
		if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED ,GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
		{
			result = MMIRECORD_SRV_RESULT_NO_SPACE;
		}
		HandleRecordEndMsg(result);//最好post msg到相应的窗口处理		
	}
}

/*****************************************************************************/
// 	Description : HandleRecordEndMsg
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordEndMsg(MMIRECORD_SRV_RESULT_E result)
{
	MMIFILE_ERROR_E    fs_ret = SFS_ERROR_NONE;
	
	MMIDEFAULT_TurnOnBackLight();
	switch (result)
    {
    case MMIRECORD_SRV_RESULT_SUCCESS:  
        break;
        
    case MMIRECORD_SRV_RESULT_MAX_FILE:
        //提示电话录音数目已满
        MMICC_OpenAlertWarningWin(TXT_COMMON_RECORD_MAX_FILE);
        break;
        
    case MMIRECORD_SRV_RESULT_NO_SPACE:
        //提示空间不足
        fs_ret = SFS_ERROR_NO_SPACE;
        ShowRecordErrorMsg(fs_ret);
        break;
    case MMIRECORD_SRV_RESULT_FOLDER_FULL:
        fs_ret = SFS_ERROR_FOLD_FULL;
        ShowRecordErrorMsg(fs_ret);
        break;
        
    default:
        //提示录音失败
        MMICC_OpenAlertFailWin(TXT_RECORD_FAIL);
		MMIRECORDSRV_DeleteRecordFile(s_cc_record_handle,TRUE);
        break;
    }
#ifndef MMI_GUI_STYLE_TYPICAL
     MMICC_SetAlertWinTitleTextId( TXT_FM_RECORD);
#endif
    if(MMIRECORD_SRV_RESULT_SUCCESS != result)
    {
        MMIAPICC_StopCCRecord();
    }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT       
    CC_UpdateCCWinHotButton(CC_BUTTON_RECORD, MMK_IsFocusWin(MMICC_STATUS_WIN_ID));
#endif
}

/*****************************************************************************/
//  Description : 开始录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL MMIRECORD_SRV_RESULT_E StartCCRecord(void)
{    
	MMIRECORD_SRV_RECORD_PARAM_T record_param = {0};
	wchar         telname[CC_MAX_PBNAME_LEN + 1]   = {0};
    wchar         name[CC_MAX_PBNAME_LEN + 1]   = {0};
    uint8         cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8         tel_num_len = CC_MAX_TELE_NUM_LEN + 2;
    MMI_STRING_T  call_name_str = {0};
	MMIFILE_DEVICE_E record_dev = MMIAPIRECORD_GetRecordStorage();
   
	
	if (PNULL != s_cc_record_handle)
	{
		return MMIRECORD_SRV_RESULT_ERROR;
	}
		
	s_cc_record_handle = MMIRECORDSRV_RequestRecordHandle(NotifyCallback);
	if (PNULL == s_cc_record_handle)
	{
		return MMIRECORD_SRV_RESULT_ERROR;
	}
    
	/* Get call number */
    MMIAPICC_GetCallName(&call_name_str);
    tel_num_len = MMIAPICC_GetCallNumber(cur_call_num, tel_num_len); 
    // 如果是隐藏号码来电...
    if(0 == tel_num_len)
    {
        MMI_GetLabelTextByLang(TXT_UNKNOW_NUM,  &call_name_str);
    }
    // 如果是未命名的情况，则取号码作为......
    else if(MMIAPIPB_IsUseDefaultName(call_name_str.wstr_ptr)||!MMIAPICOM_FileNameIsValid(&call_name_str))
    {
        MMIAPICOM_StrToWstr(cur_call_num, telname);
        if(MMIAPICOM_GetValidFileName(FALSE, telname, tel_num_len, name, CC_MAX_PBNAME_LEN))
        {
            call_name_str.wstr_ptr = name;
            call_name_str.wstr_len = MMIAPICOM_Wstrlen(name);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_DC_NAMEINVALID);//TXT_DC_NAMEINVALID
        }     
    }   
        
#ifdef MMI_RECORD_INCALL_FORMAT_ALL 
    record_param.fmt = MMIAPIRECORD_GetRecordFmt();       
#else
    record_param.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
#endif
        
    record_param.is_create_file_name = TRUE;
    record_param.prefix_ptr = PNULL;
    record_param.record_dev_ptr = &record_dev;
    record_param.record_file_id_ptr = PNULL;
    record_param.record_name_ptr = &call_name_str;
    record_param.source = MMISRVAUD_RECORD_SOURCE_CALL;
    record_param.record_param_ptr = PNULL;
    record_param.volume = MMIAPISET_GetMultimVolume();
    return MMIRECORDSRV_StartRecord(s_cc_record_handle,record_param);   
}


/*****************************************************************************/
//  Description : GetCurrentDev
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;
    wchar *file_path_ptr = MMIRECORDSRV_GetRecordFilePath(s_cc_record_handle);

    if(PNULL != file_path_ptr)
    {
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(file_path_ptr, MMIFILE_DEVICE_LEN);
    }    

    if(MMI_DEVICE_NUM <= file_dev)
    {
        file_dev= MMIAPIRECORD_GetRecordStorage();
    }

    SCI_TRACE_LOW("[CC RECORD] GetCurrentDev file_dev = %d",file_dev);
    return file_dev;
}

/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
/*****************************************************************************/
LOCAL void  ShowRecordErrorMsg(MMIFILE_ERROR_E fs_error)
{
    MMIFILE_DEVICE_E   filedev ;
    BOOLEAN  ret = TRUE;

    filedev = GetCurrentDev();
    
    ret= MMIAPIFMM_CheckMemoryCard(filedev);

    //除磁盘格式错误外的
    if(ret)
    {
        if(SFS_ERROR_ABORT == fs_error)
        {
            MMICC_OpenAlertWarningWin(TXT_ERROR);                      
        }
        else
        {
            MMIAPIFMM_PromptFileErrorType(PNULL,fs_error, filedev);
        }
    }
}

/*****************************************************************************/
//  Description : 是否通话录音中
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCCRecording(void)
{
	if (PNULL != s_cc_record_handle)
	{
		return TRUE;
	}
	return FALSE;
}
/*****************************************************************************/
// 	Description : 开始录音或停止录音
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordOpt(
                           MMI_WIN_ID_T win_id,
                           BOOLEAN is_start_auto_record
                           )
{
    MMIRECORD_SRV_RESULT_E error_code = MMIRECORD_SRV_RESULT_SUCCESS;
    MMIFILE_ERROR_E    fs_ret = SFS_ERROR_NONE;
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMICC_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
#ifndef MMI_GUI_STYLE_TYPICAL
        MMICC_SetAlertWinTitleTextId(TXT_FM_RECORD);
#endif
        return;
    }
    else if(MMIAPIMTV_IsMTVRunning()) //CMMB播放中
    {
        MMICC_OpenAlertWarningWin(TXT_EXIT_MOBILETV);
#ifndef MMI_GUI_STYLE_TYPICAL
        MMICC_SetAlertWinTitleTextId(TXT_FM_RECORD);
#endif
        return;
    }
#ifdef MV_SUPPORT//魔音下不支持录音
    else if (IsMagicVoiceWorking())
    {
        MMICC_OpenAlertWarningWin(TXT_MV_RUNNING);
#ifndef MMI_GUI_STYLE_TYPICAL
        MMICC_SetAlertWinTitleTextId( TXT_FM_RECORD);
#endif
        return;
    }
#endif
   
    if (IsCCRecording())
    {  
        if (is_start_auto_record)
        {
            //不做停止录音处理
            return;
        }
        //停止录音
        if (!MMIAPIENG_GetIQModeStatus())
        {
            MMIAPICC_StopCCRecord();
        }
        else
        {
            MMIAPIENG_AlertIQMode();
            return;
        }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT       
        CC_UpdateCCWinHotButton(CC_BUTTON_RECORD, MMK_IsFocusWin(MMICC_STATUS_WIN_ID));
#endif
#ifndef TOUCH_PANEL_SUPPORT
        if(0 == s_cc_record_handle)
        {
            MMICC_OpenAlertSuccessWin(TXT_CC_STOPRECORD);
        }
#endif        
        //录音完毕！
#if defined MMI_GUI_STYLE_TYPICAL || defined CMCC_UI_STYLE
        MMICC_OpenAlertSuccessWin(TXT_CC_RECORDCOMPLETE);
#endif
        if (MMICC_STATUS_WIN_ID != win_id)
        {
            MMK_CloseWin(win_id);
        }
	}
	else
	{    
        //开始录音
        if (!MMIAPIENG_GetIQModeStatus())
        {
            error_code = StartCCRecord();
            if(MMIRECORD_SRV_RESULT_SUCCESS != error_code)
            {
                MMIAPICC_StopCCRecord();
            }
        }
        else
        {
            MMIAPIENG_AlertIQMode();
            return;
        }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT       
        CC_UpdateCCWinHotButton(CC_BUTTON_RECORD, MMK_IsFocusWin(MMICC_STATUS_WIN_ID));
#endif
        switch (error_code)
        {
        case MMIRECORD_SRV_RESULT_SUCCESS:
            //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_RESUME, MMICC_AUDIO_DEV_MAX);
#ifndef TOUCH_PANEL_SUPPORT
            MMICC_OpenAlertSuccessWin(TXT_CC_STARTRECORD);
#endif
            if (MMICC_STATUS_WIN_ID != win_id)
            {
                MMK_CloseWin(win_id);
            }
            break;
            
        case MMIRECORD_SRV_RESULT_MAX_FILE:
            //提示电话录音数目已满
            MMICC_OpenAlertWarningWin(TXT_COMMON_RECORD_MAX_FILE);
            break;
            
        case MMIRECORD_SRV_RESULT_NO_SPACE:
            //提示空间不足
            fs_ret = SFS_ERROR_NO_SPACE;
            ShowRecordErrorMsg(fs_ret);
            break;
        case MMIRECORD_SRV_RESULT_FOLDER_FULL:
            fs_ret = SFS_ERROR_FOLD_FULL;
            ShowRecordErrorMsg(fs_ret);
            break;
            
        default:
            //提示录音失败
            MMICC_OpenAlertFailWin(TXT_RECORD_FAIL);
            break;
        }
#ifndef MMI_GUI_STYLE_TYPICAL
            MMICC_SetAlertWinTitleTextId( TXT_FM_RECORD);
#endif
       
    }

}
#endif
/*****************************************************************************/
// 	Description : the handle function of cc menu window message
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCCMenuWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E		recode = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	menu_id = 0;
    MMI_MENU_ID_T		group_id = 0;
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS					address = {0};
#endif
    //SCI_TRACE_LOW:"---mmiccapp:MMICC_HandleCCMenuWinMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9191_112_2_18_2_11_0_344,(uint8*)"");
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
    {
        MMI_STRING_T title_str = {0}; 
        MMI_GetLabelTextByLang(TXT_COMMON_ANSWER_CALL, &title_str);
        GUIMENU_SetMenuTitle(&title_str,MMICC_MENUWIN_MENU_CTRL_ID);
    }
#endif
        MMK_SetAtvCtrl(win_id, MMICC_MENUWIN_MENU_CTRL_ID);
        break;

#ifdef DPHONE_SUPPORT        //add by weidong 2011-1-5
	case MSG_KEYDOWN_HANDSFREE:
	case MSG_KEYDOWN_HOOK:
		if (!(MMIDEFAULT_GetHandFreeStates() || MMIDEFAULT_GetHookStates()))
        {
            // 需要把Call释放掉
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);//CC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
        }
        break;
#endif

    case MSG_FULL_PAINT:
        //蓝牙选项灰化
#ifdef BLUETOOTH_SUPPORT
        {
            BOOLEAN earphone_menu_gray = FALSE;
            //有激活的蓝牙耳机
            if (MMIAPIBT_GetActivatedHS(&address))
            {
                if (MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())//正在使用蓝牙耳机进行通话，将“传送到耳机选项灰显”
                {
                    earphone_menu_gray = TRUE;
                }
                else
                {
                    //正在使用手机通话，将“传送到手机”选项灰显
                    earphone_menu_gray = FALSE;
                }
            }
            else
            {
                //没有激活的蓝牙耳机则不能传送声音，均灰显
                earphone_menu_gray = TRUE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_BLUETOOTH, earphone_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_BLUETOOTH, earphone_menu_gray);            
        }
        
#endif
        //免提选项灰化
        {
            BOOLEAN handfree_menu_gray = FALSE;
            if (MMIAPICC_IsHandFree())
            {
                handfree_menu_gray = TRUE;
            }
            else
            {
                handfree_menu_gray = FALSE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_HANDFREE_SWITCH, handfree_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_HANDFREE_SWITCH, handfree_menu_gray);
        }
        
        //听筒选项灰化
        {
            BOOLEAN handhold_menu_gray = FALSE;
            if (MMICC_AUDIO_DEV_EP == MMICC_GetCurrentDev() || MMICC_AUDIO_DEV_HH == MMICC_GetCurrentDev())
            {
                handhold_menu_gray = TRUE;
            }
            else
            {
                handhold_menu_gray = FALSE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_HANDHOLD, handhold_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_HANDHOLD, handhold_menu_gray);
        }
        
        //释放保留电话选项灰化
        {
            BOOLEAN release_hold_menu_gray = FALSE;
            if (!MMICC_IsExistHoldCall())
            {
                release_hold_menu_gray = TRUE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_RELEASE_HOLD, release_hold_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_RELEASE_HOLD, release_hold_menu_gray);            
        }
        
        //释放激活电话选项灰化
        {
            BOOLEAN release_active_menu_gray = FALSE;
            if (!MMICC_IsExistActiveCall())
            {
                release_active_menu_gray = TRUE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_RELEASE_ACTIVE, release_active_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_RELEASE_ACTIVE, release_active_menu_gray);
        }
        
        //建立多方通话选项灰化
        {
            BOOLEAN setup_mpty_menu_gray = FALSE;
            if (!MMICC_IsMptySetupAvailable())
            {
                setup_mpty_menu_gray = TRUE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_BUILDMPTY, setup_mpty_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_BUILDMPTY, setup_mpty_menu_gray);
        }
        
        //分离多方通话选项灰化
        {
            BOOLEAN split_mpty_menu_gray = FALSE;
            if (!MMICC_IsMptySplitAvailable())
            {
                split_mpty_menu_gray = TRUE;
            }
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_SPLIT, split_mpty_menu_gray);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_SPLIT, split_mpty_menu_gray);
        }

#if defined(MMI_ECT_MENU_SUPPORT) && defined(MMI_CRSS_SUPPORT)
        //ECT 菜单灰化
        if (!MMICC_ECTIsPermit())
        {
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT2, ID_CC_ECT, TRUE);
            GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_COMM_OPT, ID_CC_ECT, TRUE);
        }
#endif

        GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_RECEIVE_WC2, ID_CC_HOLD_ANSWER, FALSE);
        GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_RECEIVE_WC2, ID_CC_BUILDMPTY_ANSWER, FALSE);
        if (MMICC_IsExistActiveCall() && MMICC_IsExistHoldCall())
        {
            if (CC_MAX_MPTY_NUM <= MMIAPICC_GetMPTYCallCount())
            {
                GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_RECEIVE_WC2, ID_CC_HOLD_ANSWER, TRUE);
                GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_RECEIVE_WC2, ID_CC_BUILDMPTY_ANSWER, TRUE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMICC_MENUWIN_MENU_CTRL_ID,MENU_CC_RECEIVE_WC2, ID_CC_HOLD_ANSWER, TRUE);
            }
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId( MMICC_MENUWIN_MENU_CTRL_ID, &group_id, &menu_id);
        switch( menu_id )
        {
#ifdef MMI_RECORD_SUPPORT
        case ID_CC_RECORD:
            HandleRecordOpt(win_id,FALSE);
            break;
#endif            
        case  ID_CC_SPLIT:        	        		
        case  ID_CC_RELEASE_CALL:
            CC_CallList(menu_id);
            break;
        case  ID_CC_HOLD_ANSWER:
            MMICC_HoldAndAnswerCW();
            break;
            
        case  ID_CC_RELEASE_ANSWER:
            MMICC_ReleaseAndAnswerCW();            
            break;
            
        case  ID_CC_RELEASE_HOLD:
            MMICC_ReleaseHoldCall();
            break;
            
        case  ID_CC_RELEASE_ACTIVE:
            MMICC_ReleaseActiveCall();
            break;
            
        case  ID_CC_RELEASE_ALL:
            if (CC_RESULT_SUCCESS == MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL))
            {
                //SCI_TRACE_LOW:"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_RELEASE_ALL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9362_112_2_18_2_11_1_345,(uint8*)"");
            }
            break;
            
        case  ID_CC_SHUTTLE:
            MMICC_HoldCall();
            break;
            
        case  ID_CC_BUILDMPTY:
            if (CC_RESULT_SUCCESS == MMICC_BuildMPTYCall())
            {
                //SCI_TRACE_LOW:"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_BUILDMPTY"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9373_112_2_18_2_11_1_346,(uint8*)"");
            }
            break;
            
        case  ID_CC_BUILDMPTY_ANSWER:
            MMICC_BuildMPTYAndAnswerCW();
            break;
          
        case ID_CC_HANDHOLD:
            if (MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE,MMICC_AUDIO_DEV_HH))
            {
                //SCI_TRACE_LOW:"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_HANDHOLD"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9384_112_2_18_2_11_1_347,(uint8*)"");
            }
            else //if (!MMIAPIBT_IsHeadsetUsedInCall())
            {
                //fail!
                MMICC_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
          
#ifdef BLUETOOTH_SUPPORT
        case ID_CC_BLUETOOTH:
            if (MMIAPIBT_GetActivatedHS(&address))
            {
#ifdef MMI_RECORD_SUPPORT
                if (IsCCRecording()) 
                {
                    MMICC_OpenAlertWarningWin(TXT_RECORD_CALL_ALERT);
                }
                else
#endif
                {
                    MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_BT);
                }
            }
            else
            {
                MMICC_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
#endif
            
        case ID_HANDFREE_SWITCH:
            if (EnableHandfreeMode(TRUE))
            {
                //SCI_TRACE_LOW:"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_HANDFREE_SWITCH"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9418_112_2_18_2_11_1_348,(uint8*)"");
            }
            else //if (!MMIAPIBT_IsHeadsetUsedInCall())
            {
                //fail!
                MMICC_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
            
            
#ifdef BLUETOOTH_SUPPORT
        case ID_SEND_VOICE_TO_PHONE:
            MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_DISABLE, MMICC_AUDIO_DEV_BT);
#ifdef MMI_GUI_STYLE_TYPICAL
            //success!
            MMICC_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            break;
            
        case ID_SEND_VOICE_TO_EARPHONE:	
            //record call,should not to send voice
#ifdef MMI_RECORD_SUPPORT
            if (IsCCRecording()) 
            {
                MMICC_OpenAlertWarningWin(TXT_RECORD_CALL_ALERT);
            }
            else
#endif
            {
                // @baokun fix cr87278 end
                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_BT);
#ifdef MMI_GUI_STYLE_TYPICAL
                //success!
                MMICC_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            MMK_CloseWin(MMICC_MENU_WIN_ID);
            break;
#endif

#if defined(MMI_ECT_MENU_SUPPORT) && defined(MMI_CRSS_SUPPORT)
        case ID_CC_ECT:
            //ECT
            MMICC_ECTTransfer();
            break;
#endif

        default:
            //SCI_TRACE_MID:"MMICC_HandleCCMenuWinMsg: the menu id is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9465_112_2_18_2_11_1_349,(uint8*)"");
            MMK_CloseWin( win_id );
            break;
        }

        if (menu_id != ID_CC_SPLIT && menu_id != ID_CC_RELEASE_CALL)
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_RED:
        recode =MMI_RESULT_FALSE;
        //CC_ReleaseAllCallWithCause(CC_RED_CAUSE);
        //MMK_CloseWin(win_id ) ;
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        //SCI_TRACE_LOW:"MMICC_HandleCCMenuWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9492_112_2_18_2_11_1_350,(uint8*)"d", msg_id);
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : to open cc menu
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void  MMICC_OpenCCMenu(void)
{
    //SCI_TRACE_LOW:"ccapp.c:enter function OpenCCMenu()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9504_112_2_18_2_11_1_351,(uint8*)"");
    
    //判断disconnect window是否存在
    MMK_CloseWin(MMICC_MENU_WIN_ID );
    
    //MMK_CreateWin((uint32*) MMICC_CALLMENU_WIN_TAB, PNULL );
    
    //	以下部分需要根据控件进行初始化
    if( CC_WAITING_STATE == MMICC_GetCurrentCallStatus())
    {
        CreateCCWaitingMenu();
    }
    else
    {
        CreateCCMenu();
    }
}

/*****************************************************************************/
// 	Description : insert dynamenu node by label
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,       //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,           //位置
                             MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
                             BOOLEAN    is_grayed   //is_grayed
                             )
{
    MMI_STRING_T kstring = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.select_icon_id = select_icon_id;
    node_item.is_grayed = is_grayed;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

#ifdef MMI_CC_STK_MENU_SUPPORT
/*****************************************************************************/
// 	Description : insert dynamenu node by label
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByString(
                             MMI_CTRL_ID_T ctrl_id,        //控件id
                             MMI_STRING_T kstring,       //插入的节点的文本
                             uint16 node_id,        //节点编号
                             uint16 parent_node_id, //父节点编号
                             uint16 index,           //位置
                             MMI_IMAGE_ID_T select_icon_id, //item selected icon id
                             BOOLEAN is_grayed   //is_grayed
                             )
{
    GUIMENU_DYNA_ITEM_T node_item = {0};
    
    node_item.item_text_ptr = &kstring;
    node_item.select_icon_id = select_icon_id;
    node_item.is_grayed = is_grayed;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
#endif
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : get label of record
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetRecordLabelForCCHoldMenu(void)
{
    if (IsCCRecording())
    {
        return TXT_CC_STOPRECORD;
    }
    else
    {
        return TXT_CC_STARTRECORD;
    }
}
#endif
/*****************************************************************************/
// 	Description : create cc menu (include record, handfree)
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void CreateCCMenu(void)
{   
#ifdef MMI_RECORD_SUPPORT
    CHECK_CC_APPLET_NOT_NULL
    if (IsCCRecording())
    {   
        //停止录音
        MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32 *)MMICC_MENU_COMM_OPT2_WIN_TAB, PNULL);
	}
	else
	{
        //录音
        MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32 *)MMICC_MENU_COMM_OPT_WIN_TAB, PNULL);
       
    }
#else
    MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32 *)MMICC_MENU_COMM_OPT_WIN_TAB, PNULL);
#endif
}

/*****************************************************************************/
// 	Description : create cc waiting menu
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void  CreateCCWaitingMenu(void)
{
    CHECK_CC_APPLET_NOT_NULL
    if(!MMICC_IsExistActiveCall() || !MMICC_IsExistHoldCall())
    {
        MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32 *)MMICC_MENU_RECEIVE_WC_WIN_TAB, PNULL);
    }
    else
    {
        MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32 *)MMICC_MENU_RECEIVE_WC2_WIN_TAB, PNULL);
    }
}

/*****************************************************************************/
// 	Description : the handle function of call list window message
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCCListWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E		recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T		index = (uint32)MMK_GetWinAddDataPtr(win_id);
    uint16				list_index = 0;
    
    switch(msg_id)
    {
        
    case MSG_OPEN_WINDOW:
        break;
		 
 #ifdef DPHONE_SUPPORT  
	case MSG_KEYDOWN_HANDSFREE:
	case MSG_KEYDOWN_HOOK:
		if (!(MMIDEFAULT_GetHandFreeStates() || MMIDEFAULT_GetHookStates()))
        {
            // 需要把Call释放掉
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
        }
        break;
 #endif
 		case MSG_CTL_OK:
    case MSG_APP_OK:
        list_index = GUILIST_GetCurItemIndex( MMICC_CALLLISTWIN_LISTBOX_CTRL_ID );
        MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(),  s_call_index[list_index] ,MN_CAUSE_NORMAL_CLEARING );
        MMK_CloseWin(MMICC_MENU_WIN_ID);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        list_index = GUILIST_GetCurItemIndex( MMICC_CALLLISTWIN_LISTBOX_CTRL_ID );

        switch( index )
        {
        case  CC_HOLDMENU_NODE_RELEASE_SPECIFIED_CALL:
            MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(),  s_call_index[list_index] ,MN_CAUSE_NORMAL_CLEARING );
            MMK_CloseWin(MMICC_MENU_WIN_ID);
            break;
            
        case  CC_HOLDMENU_NODE_SPLIT:
            MMK_ReturnAppointedWin( MMICC_STATUS_WIN_ID );
            MMIPUB_SetWinSoftkey(MMICC_PROCESSING_WIN_ID,TXT_NULL,STXT_EXIT,TRUE);
            if( CC_RESULT_ERROR == MMICC_SplitSpecCall( s_call_index[list_index]))
            {
                MMICC_OpenAlertFailWin(TXT_CC_OPER_FAIL);
            }
            MMK_CloseWin(MMICC_MENU_WIN_ID);
            MMK_CloseWin( win_id );
            break;
            
        default:
            MMK_CloseWin( win_id );
            break;
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

/*****************************************************************************/
// 	Description : the handle function of multi call list window message
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCCMultilistWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 list_index = 0;
    uint16 i = 0;
    MMI_CTRL_ID_T list_id = MMICC_MULTICALL_LIST_CTRL_ID;
    GUI_RECT_T cur_item_rect = {0};
#ifndef MMI_GUI_STYLE_TYPICAL
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    wchar temp_tel_num[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
#endif

    switch(msg_id)
    {
        
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        //MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        //MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
#endif
        GUILIST_SetMaxItem(list_id, CC_MAX_CALL_NUM - 1, FALSE);
        MMK_SetAtvCtrl(win_id, list_id);
        break;

    case MSG_FULL_PAINT:
        SCI_MEMSET(&s_call_index, CC_INVALID_INDEX, sizeof(s_call_index));
        GUILIST_RemoveAllItems(list_id);
        for(i = 0; i < MMICC_GetCallNum(); i++)
        {
            if(MMICC_IsCallIndexInMPTY(i))
            {
                GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
                GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
                MMI_STRING_T name_str = {0};        
#ifndef MMI_GUI_STYLE_TYPICAL
                if (MMICC_IsMptySplitAvailable())
                {
                    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON;
                }
                else
                {
                    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT; //GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                }

                item_t.item_data_ptr = &item_data;     
                item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
                
                if(!MMICC_GetCallNameStrByIndex(&name_str, i))
                {
                    uint8 tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
                    uint8 tel_num_len = 0;                    
                    MMICC_GetCallNumStrByIndex(tel_num, &tel_num_len, i);
                    
                    //get info for display call number
                    if (tel_num_len >0)
                    {
                        name_str.wstr_ptr = temp_tel_num;
                        name_str.wstr_len = tel_num_len;
                        MMI_STRNTOWSTR(name_str.wstr_ptr,
                            name_str.wstr_len,
                            (uint8 *)tel_num,
                            tel_num_len,
                            tel_num_len);
                    }
                    else
                    {
                        //the number wstr_len is 0.display unknown number info
                        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &name_str);
                    }                
                }
                
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;       
                item_data.item_content[0].item_data.image_id = IMAGE_CC_SINGLE_HANGUP_PDA;
                
                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;       
                if(MMICC_IsMptySplitAvailable())
                {
                    item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;     
                    item_data.item_content[2].item_data.image_id = IMAGE_CC_SINGLE_SPLIT_PDA;
                }

                GUILIST_AppendItem(list_id, &item_t);    
                s_call_index[list_index++] = i;

                GUILIST_GetItemRect(list_id, list_index, &cur_item_rect);
#else
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                item_t.item_data_ptr = &item_data;     
                
                MMICC_GetCallNameStrByIndex(&name_str, i);
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;                
                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;                    
                if(CC_CALL_STATE == MMICC_GetCallStatusByIndex(i))
                {
                    item_data.item_content[0].item_data.image_id = IMAGE_CC_ACTIVE;

                    if(0 < MMIAPICC_GetHoldCallCount())
                    {
                        item_data.softkey_id[0] = TXT_CC_HANG_OFF;
                    }
                    else
                    {
                        item_data.softkey_id[0] = TXT_CC_PRIVATE;
                    }
                }
                else if(CC_HOLD_STATE == MMICC_GetCallStatusByIndex(i))
                {
                    item_data.item_content[0].item_data.image_id = IMAGE_CC_HOLD;
                    item_data.softkey_id[0] = TXT_CC_HANG_OFF;
                }
                else
                {
                    //SCI_TRACE_LOW:"ccapp err index: %d, state : %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9823_112_2_18_2_11_2_352,(uint8*)"dd", i, MMICC_GetCallStatusByIndex(i));
                }
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;

                GUILIST_AppendItem(list_id, &item_t);    
                s_call_index[list_index++] = i;

                GUILIST_GetItemRect(list_id, list_index, &cur_item_rect);
#endif
            }
        }
        break;
		 
 #ifdef DPHONE_SUPPORT  
	case MSG_KEYDOWN_HANDSFREE:
	case MSG_KEYDOWN_HOOK:
		if (!(MMIDEFAULT_GetHandFreeStates() || MMIDEFAULT_GetHookStates()))
        {
            // 需要把Call释放掉
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
        }
        break;
 #endif
 
#if defined MMI_PDA_SUPPORT || defined MMI_CALLCONTROL_MINI_SUPPORT || defined MMI_MULTI_SIM_SYS_NONE
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
//    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        list_index = GUILIST_GetCurItemIndex( list_id );
        if(MMICC_GetCallNum() > s_call_index[list_index])
        {
            BOOLEAN is_only_active_call = FALSE;
            uint8 cur_call_index = s_call_index[list_index];
            is_only_active_call = ((CC_CALL_STATE == MMICC_GetCallStatusByIndex(cur_call_index))
                                    &&(!MMICC_IsExistHoldCall())
                                    &&(MMICC_IsCallIndexInMPTY(cur_call_index)));
#if defined MMI_MULTI_SIM_SYS_NONE			            
            if(is_only_active_call)
            {
                MMIAPICC_HandleMPTY_ByIndex(TRUE , cur_call_index); // True : split, False : disconnect
            }
            else
            {
                MMIAPICC_HandleMPTY_ByIndex(FALSE , cur_call_index); // True : split, False : disconnect
            }
            MMK_CloseWin(win_id);        
#else
            if(is_only_active_call)
            {
                 if( CC_RESULT_ERROR == MMICC_SplitSpecCall(cur_call_index))
                {
                    MMICC_OpenAlertFailWin(TXT_CC_OPER_FAIL);
                }               
            }
            else
            {
                MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), cur_call_index,MN_CAUSE_NORMAL_CLEARING );
            }
            MMK_CloseWin(win_id);
#endif
        }
        break;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
     case MSG_CTL_LIST_CHECK_CONTENT:
        list_index = GUILIST_GetCurItemIndex( list_id );
        item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;

        if(MMICC_GetCallNum() > s_call_index[list_index])
        {
            BOOLEAN is_only_active_call = FALSE;
            uint8 cur_call_index = s_call_index[list_index];
            is_only_active_call = ((CC_CALL_STATE == MMICC_GetCallStatusByIndex(cur_call_index))
                                    &&(0 == MMIAPICC_GetHoldCallCount())
                                    &&(MMICC_IsCallIndexInMPTY(cur_call_index)));

            switch (item_content_ptr->item_content_index)
            {
                case 0:
#if defined MMI_MULTI_SIM_SYS_NONE	
                    MMIAPICC_HandleMPTY_ByIndex(FALSE , cur_call_index); // True : split, False : disconnect
#else
                    MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(),  cur_call_index,MN_CAUSE_NORMAL_CLEARING );
#endif
                    break;
                    
                case 1:
                    break;
                    
                case 2:
                    if(is_only_active_call)
                    {
#if defined MMI_MULTI_SIM_SYS_NONE	
						MMIAPICC_HandleMPTY_ByIndex(TRUE , cur_call_index); // True : split, False : disconnect
#else
                        if( CC_RESULT_ERROR == MMICC_SplitSpecCall(cur_call_index))
                        {
                            MMICC_OpenAlertFailWin(TXT_CC_OPER_FAIL);
                        }               
#endif               
                    }
                    break;
                    
                default:
                    break;
            }
            MMK_CloseWin(win_id);
        }
        break;
#endif


#ifndef MMI_PDA_SUPPORT
        case MSG_APP_CANCEL:
#endif
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : to open call list window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_OpenMultiCallList(void)
{
    CHECK_CC_APPLET_NOT_NULL
    if(MMICC_IsExistMPTY())
    {
        if(MMK_IsOpenWin(MMICC_MULTICALL_LIST_WIN_ID))
        {
            MMK_CloseWin(MMICC_MULTICALL_LIST_WIN_ID);
        }
        MMK_CreatePubListWinEx(CC_APPLET_HANDLE, (uint32*) MMICC_MULTI_CALLLIST_WIN_TAB, PNULL);
    }
}


/*****************************************************************************/
// 	Description : to open call list window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
void CC_OpenCallList(MMI_MENU_GROUP_ID_T menu_id)
{
    CHECK_CC_APPLET_NOT_NULL
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_OpenCallList()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_9958_112_2_18_2_11_2_353,(uint8*)"");
    
    MMK_CloseWin( MMICC_CALLLIST_WIN_ID ) ;            
    
    MMK_CreateWinByApplet(CC_APPLET_HANDLE, (uint32*) MMICC_CALLLIST_WIN_TAB, (ADD_DATA)menu_id);
    
    // set the param for listbox 
    GUILIST_SetMaxItem(MMICC_CALLLISTWIN_LISTBOX_CTRL_ID, 7, FALSE );//max item 7
    
    switch(menu_id)
    {
    case  ID_CC_SPLIT:
        GUIWIN_SetTitleTextId(MMICC_CALLLIST_WIN_ID, TXT_CC_SPLIT, FALSE);
        break;
        
    case  ID_CC_RELEASE_CALL:
        GUIWIN_SetTitleTextId(MMICC_CALLLIST_WIN_ID, TXT_CC_RELEASE_CALL, FALSE);
        break;

    case CC_HOLDMENU_NODE_RELEASE_SPECIFIED_CALL:
        GUIWIN_SetTitleTextId(MMICC_CALLLIST_WIN_ID, TXT_CC_MULTICALL_VIEW, FALSE);
        break;
        
    case CC_HOLDMENU_NODE_SPLIT:
        GUIWIN_SetTitleTextId(MMICC_CALLLIST_WIN_ID, TXT_CC_MULTICALL_VIEW, FALSE);
        break;
        
    default:
        GUIWIN_SetTitleTextId(MMICC_CALLLIST_WIN_ID, STXT_OPTION, FALSE);
        break;
    }
        
    //set the active control id
    MMK_SetAtvCtrl( MMICC_CALLLIST_WIN_ID, MMICC_CALLLISTWIN_LISTBOX_CTRL_ID);
    
}


/*****************************************************************************/
// 	Description : to open call list window
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_CallList( 
                       uint32	index
                       )
{
    uint8   i = 0;
    BOOLEAN is_mpty = FALSE;
    uint8 list_index = 0;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CallList()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_10007_112_2_18_2_11_3_354,(uint8*)"");
        
    if(  ID_CC_SPLIT == index )
    {
        is_mpty = TRUE;
    }
    
    if( !MMICC_IsExistActiveCall() && !MMICC_IsExistHoldCall())
    {
        return;
    }
    
    //open the call list winodw
    if(
        ( !is_mpty ) ||
        (    
        MMICC_IsExistMPTY()
        && !MMICC_IsExistHoldCall())    
        )
    {
        CC_OpenCallList(index);
    }
    //提示需要先切换到通话状态
    else if(  
        MMICC_IsExistMPTY()
        && !MMICC_IsExistActiveCall()
        )
    {
        //Open prompt that there have not built mpty call
        MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,TXT_CC_EST_SHUTTLE_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL,0); 
        return;
    }
    //提示先建多方通话
    else
    {
        //Open prompt that there have not built mpty call
#ifndef MMI_GUI_STYLE_TYPICAL
        MMICC_OpenAlertWarningWin(TXT_CC_MERGE_CALLS_FIRST);
        MMICC_SetAlertWinTitleTextId(TXT_CC_COMMUN);
#else
        MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,TXT_CC_EST_MPTY_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL,0);
#endif
        return ;
    }
    
    for( i = 0; i < MMICC_GetCallNum() ; i++ )
    {
        if(MMICC_IsCallIndexInMPTY(i))
        {
            AppendListItem( i, MMICC_CALLLISTWIN_LISTBOX_CTRL_ID);
            s_call_index[list_index ++] = i;
        }
    }
    
}

/*****************************************************************************/
// 	Description : open adjust volume win
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_OpenAdjustVolumeWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    OpenVolumePanel(win_id, msg_id);
}

/*****************************************************************************/
// Description : set volume during call
// Global resource dependence :
// Author: jixin.xu
// Note:
/*****************************************************************************/
LOCAL void SetCallVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(PNULL != para_ptr)
    {
        MMIAPICC_SetCallVolume((uint8)para_ptr->cur_value);
#ifdef TTS_SUPPORT
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_CALL))
        {
           MMIAPITTS_SetTTSVolume((uint16)para_ptr->cur_value); 
        }
        
#endif
    } 
}

/*****************************************************************************/
//  Description : Open Volume Panel
//  Global resource dependence :
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenVolumePanel(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    if (!MMK_IsOpenWin(MMICOMMON_PANEL_WIN_ID))
    {
        MMICOM_PANEL_INFO_T panel_info = {0};

        panel_info.x = -1;
        panel_info.y = -1;
        //panel_info.open_type = turn_vol_type;
        panel_info.panel_type = MMICOM_PANEL_VOLUME;
        panel_info.cur_value = MMIAPICC_GetCallVolume();
        panel_info.last_value = MMIAPICC_GetCallVolume();//恢复默认值的时候用的
        panel_info.min_value = MMISET_VOL_ONE;
        panel_info.max_value = MMISET_VOL_MAX;
        panel_info.set_callback = SetCallVolumeCallback;
#ifndef UI_MULTILAYER_SUPPORT
		panel_info.createwin_style = MMICOM_PLAYER_PANEL_CREATE_WIN;
#endif        
        MMIAPICOM_OpenPanelChildWin(win_id, &panel_info);
    }
    else
    {
        MMK_SendMsg(MMICOMMON_PANEL_WIN_ID, msg_id, PNULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : EnableMuteMode 
//	Global resource dependence : 
//  Author:bin.ji
//	Note: TRUE to turnon Mute, TRUE to turnoff Mute
/*****************************************************************************/
LOCAL BOOLEAN EnableMuteMode(
                             BOOLEAN  is_turnon,
                             BOOLEAN  need_pubwin
                             )
{
    if (MMICC_SetCurrentCallMute(is_turnon))
    {
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_UpdateCCWinHotButton(CC_BUTTON_MUTE, TRUE);
#endif

        if (need_pubwin)
        {
            if (is_turnon)
            {
                MMICC_OpenAlertSuccessWin(TXT_CC_ENABLE_MUTE);
            }
            else
            {
                MMICC_OpenAlertSuccessWin(TXT_CC_DISABLE_MUTE);
            }
#ifndef MMI_GUI_STYLE_TYPICAL
            MMICC_SetAlertWinTitleTextId(TXT_TURNON_MUTE);
#endif
        } 
        return TRUE;
    }
    else
    {
        if (need_pubwin)
        {
            //fail!
            MMICC_OpenAlertFailWin(TXT_CC_OPER_FAIL);
#ifndef MMI_GUI_STYLE_TYPICAL
            if (MMICC_IsExistActiveAndHoldCalls())
            {

                MMICC_SetAlertWinTitleTextId(TXT_CC_SWAP_PDA);
            }
            else if (MMICC_IsExistActiveCall())
            {
                MMICC_SetAlertWinTitleTextId(TXT_CC_HOLD_PDA);
            }
            else
            {
                MMICC_SetAlertWinTitleTextId(TXT_CC_UNHOLD_PDA);
            }
#endif
        }
        return FALSE;
    }
}


/*****************************************************************************/
// 	Description : EnableHandfreeMode 
//	Global resource dependence : 
//  Author:bin.ji
//	Note: TRUE to turnon Handfree, FALSE to change into handhold/earphone
/*****************************************************************************/
LOCAL BOOLEAN EnableHandfreeMode(
                                 BOOLEAN is_turnon
                                 )
{
    MMICC_AUDIO_DEV_E dev = is_turnon?MMICC_AUDIO_DEV_HF:MMICC_AUDIO_DEV_HH;
    MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, dev);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle hold menu msg
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.9.29
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHoldMenuWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    uint16				        node_id     = 0;
    MMI_RESULT_E		        recode      = MMI_RESULT_TRUE;
    uint16                      node_index = 0;
#ifdef MMI_RECORD_SUPPORT
    MMI_STRING_T                record_str = {0};
    MMI_TEXT_ID_T               label = TXT_NULL;
#endif
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address = {0};
#endif
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        break;
                
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,  MMICC_HOLDMENUWIN_MENU_CTRL_ID);
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_GET_FOCUS:
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
#ifdef MMI_ENABLE_HANDFREE
#ifdef BLUETOOTH_SUPPORT
        if (!(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev()))
        {
            node_index = MMICC_BT_HF_RECORD_INDEX;
        }
        else
        {
            node_index = MMICC_BT_RECORD_INDEX;
        }
#else
        node_index = MMICC_HF_RECORD_INDEX;
#endif  //BLUETOOTH_SUPPORT
#else 
#ifdef BLUETOOTH_SUPPORT
        if (!(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev()))
        {
            node_index = MMICC_BT_RECORD_INDEX;
        }
        else
        {
            node_index = MMICC_HOLD_RECORD_INDEX;
        }
#else
        node_index = MMICC_HOLD_RECORD_INDEX;
#endif  //BLUETOOTH_SUPPORT
#endif  //MMI_ENABLE_HANDFREE
#ifdef MMI_RECORD_SUPPORT
        label = GetRecordLabelForCCHoldMenu();
        MMI_GetLabelTextByLang(label, &record_str);
        GUIMENU_ReplaceNode(node_index, CC_HOLDMENU_NODE_RECORD , 0, &record_str,  MMICC_HOLDMENUWIN_MENU_CTRL_ID);
#endif
}
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        node_id = GUIMENU_GetCurNodeId(MMICC_HOLDMENUWIN_MENU_CTRL_ID);
        switch( node_id )
        {
#ifdef MMI_CALLCONTROL_MINI_SUPPORT 
	   case CC_HOLDMENU_NODE_NEW_CALL:
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
#if defined(MMI_KEY_LOCK_EFFECT_PSW_SUPPORT)
        if (MMIAPIKL_IsNeedPsw())
        {
            MMIAPIKL_OpenPswWin(TRUE, MMIAPIIDLE_EnterDialWin);    
        }
        else
#endif
        {
            MMIAPIIDLE_EnterDialWin();
        }
	 MMK_CloseWin(win_id);
}
        break;
        case CC_HOLDMENU_NODE_MERGE_CALLS:
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            MMICC_BuildMPTYCall();
            MMK_CloseWin(win_id);
}
        break;
 #endif
#ifdef MMI_RECORD_SUPPORT
        case CC_HOLDMENU_NODE_RECORD://ID_CC_RECORD:		
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            HandleRecordOpt(win_id,FALSE);
            MMK_CloseWin(win_id);
}
            break;
#endif            
        case CC_HOLDMENU_NODE_SHUTTLE://ID_CC_HOLD:            
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            MMICC_HoldCall();
            MMK_CloseWin(win_id);
}
            break;
            
        case CC_HOLDMENU_NODE_PB://ID_PB:
            MMIAPIPB_OpenListAllGroupWin();
            MMK_CloseWin(win_id);  
            break;
            
        case CC_HOLDMENU_NODE_CL:
#if defined(JAVA_SUPPORT) && defined(LOW_MEMORY_SUPPORT)
            if (MMIAPIJAVA_IsJavaRuning())
            {
                MMICC_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
                MMK_CloseWin(win_id);
                break;
            }
#endif            
#if !defined(MMI_GUI_STYLE_TYPICAL)&&defined(MMI_KEY_LOCK_EFFECT_PSW_SUPPORT)
            if (MMIAPIKL_IsNeedPsw())
            {
                MMIAPIKL_OpenPswWin(TRUE, (MMI_PSW_FUNC)MMIAPICL_OpenMainMenuWindow);    
            }
            else
#endif
            {
                MMIAPICL_OpenMainMenuWindow();
            }
            MMK_CloseWin(win_id);
            break;
            
        case CC_HOLDMENU_NODE_SMS://ID_MESSAGE:
#if !defined(MMI_GUI_STYLE_TYPICAL)&&defined(MMI_KEY_LOCK_EFFECT_PSW_SUPPORT)
        if (MMIAPIKL_IsNeedPsw())
        {
            MMIAPIKL_OpenPswWin(TRUE, MMIAPISMS_OpenMainMenuWin);    
        }
        else
#endif
        {
            MMIAPISMS_OpenMainMenuWin();
        }
            MMK_CloseWin(win_id);
            break;
            
#ifdef BLUETOOTH_SUPPORT
        case CC_HOLDMENU_NODE_BLUETOOTH: //ID_CC_SEND_VOICE
            if (MMIAPIBT_GetActivatedHS(&address))
            {
#if 0            
#ifdef MMI_RECORD_SUPPORT
                if (IsCCRecording()) 
                {
                    MMICC_OpenAlertWarningWin(TXT_RECORD_CALL_ALERT);
                }
                else
#endif //cr 134806
#endif
                {
                    MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_BT);
                }
            }
            else
            {
                MMICC_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
#endif
        case CC_HOLDMENU_NODE_HANDHOLD:
            if (MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_HH))
            {
                //SCI_TRACE_LOW:"mmiccapp: CC_HOLDMENU_NODE_HANDHOLD"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_10536_112_2_18_2_11_4_357,(uint8*)"");
            }
            else //if (!MMIAPIBT_IsHeadsetUsedInCall())
            {
                //fail!
                MMICC_OpenAlertFailWin(TXT_ERROR);
            }

            MMK_CloseWin(win_id);
            break;
            
#ifdef MV_SUPPORT
        case CC_HOLDMENU_NODE_MAGICVOICE:
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
            {
                OpenMagicVoiceSelectWindow(NULL);
            }
            break;
#endif
            
        case CC_HOLDMENU_NODE_HANDFREE:
            if (MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_HF))
            {
                //SCI_TRACE_LOW:"mmiccapp: CC_HOLDMENU_NODE_HANDFREE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_10558_112_2_18_2_11_4_358,(uint8*)"");
            }
            else //if (!MMIAPIBT_IsHeadsetUsedInCall())
            {
                //fail!
                MMICC_OpenAlertFailWin(TXT_ERROR);
            }

            MMK_CloseWin(win_id);
            break;
            
        case CC_HOLDMENU_NODE_MUTE:
            {
                EnableMuteMode((BOOLEAN)(!MMICC_IsCurrentCallMute()),TRUE);
                MMK_CloseWin(win_id);
            }
            break;
            
        case CC_HOLDMENU_NODE_BUILDMPTY:
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            MMICC_BuildMPTYCall();
}
            break;

        case CC_HOLDMENU_NODE_VIEW_MULTICALL:
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
#if !defined(TOUCH_PANEL_SUPPORT)
            if (MMICC_IsMptySplitAvailable())    
            {
                CC_CallList(CC_HOLDMENU_NODE_SPLIT);
            }
            else
            {
                CC_CallList(CC_HOLDMENU_NODE_RELEASE_SPECIFIED_CALL);
            }
#else
            CC_OpenMultiCallList();
#endif
}
            break;
        
        case CC_HOLDMENU_NODE_DTMF:
            MMICC_SetDtmfSwitch(!CC_INSTANCE.is_dtmf_switch); /*lint !e730*/
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,TXT_CC_OPER_SUCC,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL,0);
#endif
            MMK_CloseWin(win_id);
            break;
#ifdef BROWSER_SUPPORT
        case CC_HOLDMENU_NODE_BROWSER:
            //@fen.xie MS00178832:先提示U盘使用中
            if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
            {
                MMICC_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {
                MMIAPICC_SwitchToBrowser();
            }
            MMK_CloseWin(win_id);
            break;
        #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
        case CC_HOLDMENU_NODE_BT_CONNECT_HF:
            MMIAPIBT_PariedHandFree();
            break;
        #endif        
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
        case CC_HOLDMENU_NODE_END_CALLS:     
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            MMICC_ReleaseActiveCall();
            MMICC_ReleaseHoldCall();
}
            break;

        case CC_HOLDMENU_NODE_VOLUME:
            MMK_CloseWin(win_id);
            MMIAPICC_AdjustCallVolume(TRUE);
            break;
#endif

#ifdef MMI_CC_STK_MENU_SUPPORT
        case CC_HOLDMENU_NODE_STK://for orange
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            if (MMIAPISTK_IsStkCard(MMIAPICC_GetCallDualSys()))
            {
                MMIAPISTK_EnterToplevelMenu(MMIAPICC_GetCallDualSys());
            }

            MMK_CloseWin(win_id);
}
            break;
#endif

#if defined(MMI_ECT_MENU_SUPPORT) && defined(MMI_CRSS_SUPPORT)
        case CC_HOLDMENU_NODE_ECT_TRANSFER:
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            //ECT
            MMICC_ECTTransfer();
}
            break;
#endif

        default:
            MMK_CloseWin(win_id);
            break;
        }
        break;

        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
        
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}


/*****************************************************************************/
// 	Description : the process KBD,timer message function of the CC application
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.8
/*****************************************************************************/
LOCAL MMI_RESULT_E  CC_HandleCcProcessingWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleCcProcessingWinMsg() msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_10672_112_2_18_2_11_4_359,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {	
    case MSG_OPEN_WINDOW:
        //增加状态栏
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
        
    case MSG_APP_RED:
        MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_NORMAL_CLEARING);
        recode = MMI_RESULT_TRUE;
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //fix for cr 74433
        break;
        
    default:
        //do nothing
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description :if is dtmf panel is on focus
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsDtmfPanel(MMI_WIN_ID_T win_id)
{ 
#if defined(TOUCH_PANEL_SUPPORT)
    if(PNULL == MMICC_GetAppletInstance())
    {
        return FALSE;
    }
    
    if (MMICC_STATUS_WIN_ID == win_id || MMICC_ANIMATION_WIN_ID == win_id)
    {
        return TRUE;
    }
    
#endif

    return FALSE;
}

#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: tao.xue
//	Note:   
/*****************************************************************************/
LOCAL void CC_HotButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id= 0;
    GUI_RECT_T img_rect={0};//图片裁剪区
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T    win_id=0;
    uint16 image_w=0;
    uint16 image_h=0;
    uint16 rect_width=0;
    uint16 rect_height=0;
    CC_CC_BUTTON_E button_type = CC_BUTTON_HANDFREE; 

    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
    MMI_STRING_T        str_data = {0};
//#ifdef MAINLCD_SIZE_240X320
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
//#else
    //GUISTR_STATE_T      state = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK;
//#endif
    GUISTR_INFO_T       text_info = {0};
    uint16 text_w=0;
    uint16 text_h=0;

    MMI_IMAGE_ID_T image_id;
    MMI_TEXT_ID_T text_id;
    
    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id=MMK_GetWinId(win_handle);

    for (button_type = CC_BUTTON_HANDFREE; button_type < CC_BUTTON_MAX; button_type++)
    {
        if (ctrl_id == s_cc_win_button[button_type].ctrl_id)
        {
            break;

        }
    }

    if (button_type == CC_BUTTON_MAX)
    {
        return ;
    }
    
    if (s_cc_win_button[button_type].is_gray)
    {
        text_style.font_color = MMI_CALL_GRAY_COLOR;
        image_id = s_cc_win_button[button_type].img_icon_grayed_id;
    }
    else if(CC_IsHotButtonDisplayOnImg(button_type)
#if !defined(TOUCH_PANEL_SUPPORT)
        || button_type == CC_INSTANCE.cc_win_focus_button_type
#endif
        )
    {
        text_style.font_color = MMI_CALL_ON_COLOR;
        image_id = s_cc_win_button[button_type].img_icon_grayed_id;
    }
    else
    {
        if (button_type == CC_BUTTON_ANSWER)
        {
            text_style.font_color = MMI_CALL_ACTIVE_COLOR;
        }
        else if (button_type == CC_BUTTON_HANGUP)
        {
            text_style.font_color = MMI_CALL_END_COLOR;
        }
        else
        {
            text_style.font_color = CC_TEXT_COLOR;
        }
        image_id = s_cc_win_button[button_type].img_icon_id;
    }

    text_id = s_cc_win_button[button_type].text_id;
    
    rect_width = (uint16)(owner_draw_ptr->display_rect.right - owner_draw_ptr->display_rect.left + 1);
    rect_height = (uint16)(owner_draw_ptr->display_rect.bottom - owner_draw_ptr->display_rect.top + 1);

    {
    	GUIRES_GetImgWidthHeight(&image_w, &image_h, image_id, win_id);
    	
    	img_rect.left=owner_draw_ptr->display_rect.left + (rect_width - image_w)/2;
        /*if (button_type == CC_BUTTON_ANSWER 
            || button_type == CC_BUTTON_NEW_CALL 
            || button_type == CC_BUTTON_DIALPANEL 
            || button_type == CC_BUTTON_HANGUP 
            || button_type == CC_BUTTON_MERGE_CALLS)
        {
    	    img_rect.top=owner_draw_ptr->display_rect.top + (rect_height - image_h)/2 - 8;//row 0
        }
        else*/
        {
//#if defined(MAINLCD_SIZE_240X400)||defined(MAINLCD_SIZE_240X320)//@tao.xue
    	    img_rect.top=owner_draw_ptr->display_rect.top + (rect_height - image_h)/2 - 12;//row 1
/*#else
    	    img_rect.top=owner_draw_ptr->display_rect.top + (rect_height - image_h)/2 - 17;//row 1
#endif*/
        }
    	img_rect.right=img_rect.left+image_w - 1;
    	img_rect.bottom=img_rect.top+image_h - 1;
    }

    {  
        text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
        text_style.angle = ANGLE_0;
        text_style.char_space = 0;
        text_style.effect = FONT_EFFECT_CUSTOM;
        text_style.edge_color = 0;
#if defined(MAINLCD_SIZE_240X400)||defined(MAINLCD_SIZE_240X320)//@tao.xue
        text_style.font = SONG_FONT_14;
#elif defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
        text_style.font = SONG_FONT_8;
#else
        text_style.font = SONG_FONT_16;
#endif
        text_style.line_space = 0;
        text_style.region_num = 0;
        text_style.region_ptr = PNULL;

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)text_id, &str_data);
        GUISTR_GetStringInfo(&text_style, &str_data, state, &text_info);

        text_w = rect_width;

        text_h = text_info.height;
        
        text_display_rect.left = owner_draw_ptr->display_rect.left + (rect_width - text_w)/2;
        text_display_rect.top = img_rect.bottom + 4;
        text_display_rect.right=text_display_rect.left+text_w - 1;
        text_display_rect.bottom=text_display_rect.top+text_h - 1;
    }

    GUIRES_DisplayImg(PNULL, &img_rect, PNULL, win_handle, image_id, &owner_draw_ptr->lcd_dev);
    GUISTR_DrawTextToLCDInRect( 
        MMITHEME_GetDefaultLcdDev(),
        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&str_data,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  Description : create hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateHotButtonsWithoutForm(MMI_WIN_ID_T win_id)
{
    int32 row = 0;
    int32 column = 0;
    GUIBUTTON_INIT_DATA_T button_init = {0};
    CC_CC_BUTTON_E new_button_type = CC_BUTTON_MAX;
    MMI_HANDLE_T    button_handle = 0;    
    MMI_CONTROL_CREATE_T        button_ctrl = {0};      
    int32 index = 0;
    GUI_BG_T button_bg = {0};
    CC_CC_BUTTON_E hot_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] = {0};
#ifdef MMI_PDA_SUPPORT    
    CC_CC_BUTTON_E connected_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
    {
        {CC_BUTTON_NEW_CALL, CC_BUTTON_DIALPANEL, CC_BUTTON_HOLD},
        {CC_BUTTON_RECORD, CC_BUTTON_MUTE, CC_BUTTON_HANDFREE },
    };
    CC_CC_BUTTON_E mt_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
    {
        {CC_BUTTON_ANSWER, CC_BUTTON_DIALPANEL, CC_BUTTON_HOLD},
        {CC_BUTTON_RECORD, CC_BUTTON_SILENCE, CC_BUTTON_HANDFREE },
    };
#else  //MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
    CC_CC_BUTTON_E connected_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
    {
        {CC_BUTTON_NEW_CALL, CC_BUTTON_DIALPANEL, CC_BUTTON_HANGUP},
        {CC_BUTTON_RECORD, CC_BUTTON_MUTE, CC_BUTTON_HOLD },
    };
    CC_CC_BUTTON_E mt_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
    {
        {CC_BUTTON_ANSWER, CC_BUTTON_DIALPANEL, CC_BUTTON_HANGUP},
        {CC_BUTTON_RECORD, CC_BUTTON_SILENCE, CC_BUTTON_HOLD },
    };

#else //TOUCH_PANEL_SUPPORT
    CC_CC_BUTTON_E connected_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
    {
        {CC_BUTTON_PB, CC_BUTTON_DIALPANEL, CC_BUTTON_HOLD},
        {CC_BUTTON_RECORD, CC_BUTTON_MUTE, CC_BUTTON_HANDFREE },
    };
    CC_CC_BUTTON_E mt_win_button_order[CC_HOT_BUTTON_ROW_MAX][CC_HOT_BUTTON_COLUMN_MAX] =
    {
        {CC_BUTTON_PB, CC_BUTTON_DIALPANEL, CC_BUTTON_HOLD},
        {CC_BUTTON_RECORD, CC_BUTTON_SILENCE, CC_BUTTON_HANDFREE },
    };
#endif //TOUCH_PANEL_SUPPORT
#endif//MMI_PDA_SUPPORT

    switch (win_id)
    {
    case MMICC_STATUS_WIN_ID:
    case MMICC_DISCONNECT_WIN_ID:
    case MMICC_AUTO_DIAL_WIN_ID:        
        SCI_MEMCPY((void *)&hot_button_order, (void *)&connected_win_button_order, sizeof(hot_button_order));
        if ( MMICC_IsExistActiveAndHoldCalls())
        {
            hot_button_order[0][0] = CC_BUTTON_MERGE_CALLS;
        }
        else
        {
#ifdef TOUCH_PANEL_SUPPORT            
            hot_button_order[0][0] = CC_BUTTON_NEW_CALL;
#else
            hot_button_order[0][0] = CC_BUTTON_PB;
#endif
        }

        {
            if ( MMICC_IsExistActiveAndHoldCalls())
            {
                hot_button_order[HB_HOLD_ROW][HB_HOLD_COLUMN] = CC_BUTTON_SWAP_CALLS;
            }
            else if(MMICC_IsExistHoldCall())
            {
                hot_button_order[HB_HOLD_ROW][HB_HOLD_COLUMN] = CC_BUTTON_RETRIEVE_CALLS;
            }
            else
            {
                hot_button_order[HB_HOLD_ROW][HB_HOLD_COLUMN] = CC_BUTTON_HOLD;
            }
        }
        break;
        
    case MMICC_ANIMATION_WIN_ID:
        SCI_MEMCPY((void *)&hot_button_order, (void *)&mt_win_button_order, sizeof(hot_button_order));
        
#ifdef ASP_SUPPORT
        if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus() && !MMIAPIASP_IsMachineTurnOn() && !MMIAPIUDISK_UdiskIsRun())
        {
            hot_button_order[CC_HOT_BUTTON_ROW_MAX - 1][0] = CC_BUTTON_ASP;
        }
        else
        {
            hot_button_order[CC_HOT_BUTTON_ROW_MAX - 1][0] = CC_BUTTON_RECORD;
        }
#endif
        if (MMIAPICC_IsMtCallPlayingRing())
        {
            hot_button_order[CC_HOT_BUTTON_ROW_MAX - 1][1] = CC_BUTTON_SILENCE;
        }
        else
        {
            hot_button_order[CC_HOT_BUTTON_ROW_MAX - 1][1] = CC_BUTTON_MUTE;
        }
#ifdef TOUCH_PANEL_SUPPORT            
        hot_button_order[0][0] = CC_BUTTON_ANSWER;
#endif
        break;
    default :
        return ;
    }

    //destroy all the hot buttons existed in this window
    for (index = 0; index < CC_BUTTON_MAX; index ++)
    {
        MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, s_cc_win_button[index].ctrl_id);
        if(0 != ctrl_handle)
        {
            MMK_DestroyDynaCtrl(ctrl_handle);
        }
    }

    for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
    {
        for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
        {
            GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
            new_button_type = hot_button_order[row][column];
            button_handle = s_cc_win_button[new_button_type].ctrl_id;
            if (CC_BUTTON_MAX != new_button_type)
            {
                //set handle long press msg
                button_init.both_rect.v_rect.left   = s_connected_win_rect_coordinate[row][column].left;
                button_init.both_rect.v_rect.right  = s_connected_win_rect_coordinate[row][column].right;
                button_init.both_rect.v_rect.top    = s_connected_win_rect_coordinate[row][column].top + client_both_rect.v_rect.top;
                button_init.both_rect.v_rect.bottom = s_connected_win_rect_coordinate[row][column].bottom + client_both_rect.v_rect.top;

                button_init.both_rect.h_rect.left   = s_connected_win_rect_coordinate_h[row][column].left;
                button_init.both_rect.h_rect.right  = s_connected_win_rect_coordinate_h[row][column].right;
                button_init.both_rect.h_rect.top    = s_connected_win_rect_coordinate_h[row][column].top + client_both_rect.h_rect.top;
                button_init.both_rect.h_rect.bottom = s_connected_win_rect_coordinate_h[row][column].bottom + client_both_rect.h_rect.top;
                
                button_init.bg.bg_type = GUI_BG_IMG;
                
                button_ctrl.ctrl_id           = button_handle;
                button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                button_ctrl.init_data_ptr     = &button_init;
                button_ctrl.parent_win_handle = win_id;
                MMK_CreateControl(&button_ctrl);
                
                GUIBUTTON_SetOwnerDraw(button_handle, CC_HotButtonOwnerDrawFun);
                
                button_bg.bg_type = GUI_BG_IMG;
                button_bg.img_id = s_cc_win_button[new_button_type].img_bg_id;
                GUIBUTTON_SetBg(button_handle, &button_bg);

                button_bg.img_id = s_cc_win_button[new_button_type].img_bg_pressed_id;
                GUIBUTTON_SetPressedBg(button_handle, &button_bg);
            }
        }
    } 
    
    SCI_MEMCPY((void *)&(CC_INSTANCE.cc_win_button_order), (void *)&hot_button_order, sizeof(hot_button_order));

    if (CC_INSTANCE.cc_display_type != CC_DISPLAY_HOT_BUTTONS)
    {
        for (index = 0; index < CC_BUTTON_MAX; index ++)
        {
            GUIBUTTON_SetVisible(s_cc_win_button[index].ctrl_id, FALSE, FALSE);
        }
    }
    else
    {
        if (MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }
    
    CC_UpdateHotButtonState();
#ifdef MMI_PDA_SUPPORT    
    if((MMICC_DISCONNECT_WIN_ID != win_id) && (MMICC_AUTO_DIAL_WIN_ID != win_id))
    {
        ConstructHangupButton(win_id);
    }
#endif
}

/*****************************************************************************/
//  Description : create one hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateNewHotButtonWithoutForm(MMI_WIN_ID_T win_id, int32 new_button_row, int32 new_button_column, CC_CC_BUTTON_E new_button_type)
{
    int32 row = 0;
    int32 column = 0;
    GUIBUTTON_INIT_DATA_T button_init = {0};
    CC_CC_BUTTON_E old_button_type = CC_BUTTON_MAX;
    MMI_HANDLE_T    button_handle = 0;    
    MMI_CONTROL_CREATE_T        button_ctrl = {0};      
    MMI_HANDLE_T ctrl_handle = 0;
    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //Hots buttons array should be created first, otherwise, creating new hot button is not allow. 
    if (0 == MMK_GetCtrlHandleByWin(win_id, s_cc_win_button[1].ctrl_id))
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_CreateNewHotButtonWithoutForm() PNULL == s_current_win_button_order_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11057_112_2_18_2_11_5_360,(uint8*)"");
        return ;
    }
    
    old_button_type = CC_INSTANCE.cc_win_button_order[new_button_row][new_button_column];
    if (new_button_type == old_button_type)
    {
        return ;
    }
    else if((ctrl_handle = MMK_GetCtrlHandleByWin(win_id, s_cc_win_button[old_button_type].ctrl_id)) != 0)
    {
        MMK_DestroyDynaCtrl(ctrl_handle);
    }

    button_handle = s_cc_win_button[new_button_type].ctrl_id;

    row = new_button_row;
    column = new_button_column;
    button_init.both_rect.v_rect.left   = s_connected_win_rect_coordinate[row][column].left;
    button_init.both_rect.v_rect.right  = s_connected_win_rect_coordinate[row][column].right;
    button_init.both_rect.v_rect.top    = s_connected_win_rect_coordinate[row][column].top + client_both_rect.v_rect.top;
    button_init.both_rect.v_rect.bottom = s_connected_win_rect_coordinate[row][column].bottom + client_both_rect.v_rect.top;

    button_init.both_rect.h_rect.left   = s_connected_win_rect_coordinate_h[row][column].left;
    button_init.both_rect.h_rect.right  = s_connected_win_rect_coordinate_h[row][column].right;
    button_init.both_rect.h_rect.top    = s_connected_win_rect_coordinate_h[row][column].top + client_both_rect.h_rect.top;
    button_init.both_rect.h_rect.bottom = s_connected_win_rect_coordinate_h[row][column].bottom + client_both_rect.h_rect.top;
    button_init.bg.bg_type = GUI_BG_IMG;
    
    button_ctrl.ctrl_id           = button_handle;
    button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    button_ctrl.init_data_ptr     = &button_init;
    button_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&button_ctrl);

    GUIBUTTON_SetOwnerDraw(button_handle, CC_HotButtonOwnerDrawFun);
                
    CC_INSTANCE.cc_win_button_order[row][column] = new_button_type;

    CC_SetHotButtonImg(new_button_type, FALSE);
    
    if (CC_INSTANCE.cc_display_type != CC_DISPLAY_HOT_BUTTONS)
    {
        GUIBUTTON_SetVisible(button_handle, FALSE, FALSE);
        GUIBUTTON_SetVisible(MMICC_BUTTON_HANGUP_CTRL_ID, FALSE, FALSE);
    }
    else
    {
        if (MMK_IsFocusWin(win_id))
        {
            GUIBUTTON_SetVisible(button_handle, TRUE, TRUE);
            GUIBUTTON_SetVisible(MMICC_BUTTON_HANGUP_CTRL_ID, TRUE, TRUE);
        }
        else
        {
            GUIBUTTON_SetVisible(button_handle, TRUE, FALSE);
            GUIBUTTON_SetVisible(MMICC_BUTTON_HANGUP_CTRL_ID, TRUE, FALSE);            
        }
    }
}
#endif

/*****************************************************************************/
// 	Description :create cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_CreateDtmfPanel(MMI_WIN_ID_T win_id)
{
#if defined(TOUCH_PANEL_SUPPORT)
    MMIAPIIDLE_CreateDtmfPanel(win_id);
    MMIAPIIDLE_HideDtmfPanel(win_id);

#ifndef MAINLCD_SIZE_320X480
    if(MMICC_ANIMATION_WIN_ID == win_id 
        && 0 == s_animwin_dtmfpanel_layer.block_id)
    {
        s_animwin_dtmfpanel_layer = MMIAPIIDLE_CreateDtmfPanelLayer(win_id);
    }
    else if(MMICC_STATUS_WIN_ID == win_id
        && 0 == s_connectedwin_dtmfpanel_layer.block_id)
    {
        s_connectedwin_dtmfpanel_layer = MMIAPIIDLE_CreateDtmfPanelLayer(win_id);
    }
#endif//MAINLCD_SIZE_320X480
#endif//TOUCH_PANEL_SUPPORT
}

/*****************************************************************************/
// 	Description :display cc dtmf panel for sending dtmf & hide hot buttons & disable other buttons
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_DisplayDtmfPanel(MMI_WIN_ID_T win_id)
{
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
#if defined(TOUCH_PANEL_SUPPORT)
    int32 index = 0;

    MMIAPIIDLE_ShowDtmfPanel(win_id);
    
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_ACTIVE_CALL_CTRL_ID,TRUE,PNULL,FALSE);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_HOLD_CALL_CTRL_ID,TRUE,PNULL,FALSE);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_SWAP_CALL_CTRL_ID,TRUE,PNULL,FALSE);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_OPTION_CTRL_ID,TRUE,PNULL,FALSE);
    
    for (index = 0; index < CC_BUTTON_MAX; index ++)
    {
        GUIBUTTON_SetVisible(s_cc_win_button[index].ctrl_id, FALSE, FALSE);
    }
    //display hangup button
    GUIBUTTON_SetVisible(MMICC_BUTTON_HANGUP_CTRL_ID, FALSE, FALSE);    
#endif
#endif
}

/*****************************************************************************/
// 	Description :show cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_ShowDtmfPanel(MMI_WIN_ID_T win_id)
{
#if defined(TOUCH_PANEL_SUPPORT)
    if (CC_INSTANCE.cc_display_type == CC_DISPLAY_HOT_BUTTONS)
    {
#if !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
		CC_ReappendDtmfPanelLayer(win_id);
#endif
        CC_UpdateCcDisplayType(CC_DISPLAY_HOT_BUTTONS);
        CC_DisplayByType(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function MMICC_CloseDtmfPanel() else WARRNING!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11193_112_2_18_2_11_5_361,(uint8*)"");
    }
#endif
}

/*****************************************************************************/
// 	Description :open cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_HideDtmfPanel(MMI_WIN_ID_T win_id)
{
    SCI_TRACE_LOW("ccapp.c : CC_HideDtmfPanel cc_display_type=%d", CC_INSTANCE.cc_display_type);

#if defined(TOUCH_PANEL_SUPPORT)
    if (CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL)
    {
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
        if(MMICC_ANIMATION_WIN_ID == win_id)
        {
            UILAYER_Clear(&s_animwin_dtmfpanel_layer);
        }
        else if(MMICC_STATUS_WIN_ID == win_id)
        {
            UILAYER_Clear(&s_connectedwin_dtmfpanel_layer);
        }
#endif
        CC_UpdateCcDisplayType(CC_DISPLAY_DTMF_PANEL);
        CC_DisplayByType(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function MMICC_CloseDtmfPanel() else WARRNING!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11213_112_2_18_2_11_5_362,(uint8*)"");
    }
#endif
}

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//    Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void CC_ReappendDtmfPanelLayer(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    if(MMICC_ANIMATION_WIN_ID == win_id)
    {
        lcd_dev_info = s_animwin_dtmfpanel_layer;
    }
    else if(MMICC_STATUS_WIN_ID == win_id)
    {
        lcd_dev_info = s_connectedwin_dtmfpanel_layer;
    }
    if (0 != lcd_dev_info.block_id)
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        append_layer.lcd_dev_info.lcd_id = lcd_dev_info.lcd_id;
        append_layer.lcd_dev_info.block_id = lcd_dev_info.block_id;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_AppendBltLayer(&append_layer);
    }
}
#endif

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
// 	Description :display hot buttons & enable other buttons & hide cc dtmf panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_DisplayHotButtons(MMI_WIN_ID_T win_id)
{
    int32 index = 0;
    
 #if defined(TOUCH_PANEL_SUPPORT)
    MMIAPIIDLE_HideDtmfPanel(win_id);
#endif

    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_ACTIVE_CALL_CTRL_ID,FALSE,PNULL,FALSE);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_HOLD_CALL_CTRL_ID,FALSE,PNULL,FALSE);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_SWAP_CALL_CTRL_ID,FALSE,PNULL,FALSE);
    GUIBUTTON_SetGrayed(MMICC_BUTTON_PDA_OPTION_CTRL_ID,FALSE,PNULL,FALSE);
    
    for (index = 0; index < CC_BUTTON_MAX; index ++)
    {
        GUIBUTTON_SetVisible(s_cc_win_button[index].ctrl_id, TRUE, FALSE);
    }

    //display hangup button
    GUIBUTTON_SetVisible(MMICC_BUTTON_HANGUP_CTRL_ID, TRUE, FALSE);    
}
#endif

/*****************************************************************************/
// 	Description :desplay buttons of cc win by cc_display_type
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_DisplayByType(MMI_WIN_ID_T win_id)
{
    switch (CC_INSTANCE.cc_display_type)
    {
        case CC_DISPLAY_MT_LOCK: 
            break;
            
        case CC_DISPLAY_HOT_BUTTONS:
#ifndef MMI_CALLCONTROL_MINI_SUPPORT            
            CC_DisplayHotButtons(win_id);
            CC_UpdateHotButtonState();
#endif            
            break;

        case CC_DISPLAY_DTMF_PANEL:
            CC_DisplayDtmfPanel(win_id);
            break;

        case CC_DISPLAY_NONE:
        case CC_DISPLAY_TYPE_MAX:
        default:
            break;
    }
}

/*****************************************************************************/
// 	Description :update cc_display_type by cc state
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
LOCAL void CC_UpdateCcDisplayType(CC_DISPLAY_TYPE_E   cancel_cc_display_type)
{
    //if cc_display_type is cancel_cc_display_type ,change it
    if (CC_INSTANCE.cc_display_type == cancel_cc_display_type)
    {
        switch (CC_INSTANCE.cc_display_type)
        {
            case CC_DISPLAY_MT_LOCK:
                if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
                {
                    CC_INSTANCE.cc_display_type = CC_DISPLAY_HOT_BUTTONS;
                }
                else
                {   
                    CC_INSTANCE.cc_display_type = CC_DISPLAY_NONE;
                }
                break;
                
            case CC_DISPLAY_HOT_BUTTONS:
#if defined(TOUCH_PANEL_SUPPORT)
                if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
                {
                    CC_INSTANCE.cc_display_type = CC_DISPLAY_DTMF_PANEL;
                }
                else
#endif
                {   
                    CC_INSTANCE.cc_display_type = CC_DISPLAY_NONE;
                }
                break;
                
            case CC_DISPLAY_DTMF_PANEL:
                if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
                {
                    CC_INSTANCE.cc_display_type = CC_DISPLAY_HOT_BUTTONS;
                }
                else
                {   
                    CC_INSTANCE.cc_display_type = CC_DISPLAY_NONE;
                }
                break;

            case CC_DISPLAY_NONE:
                break;
                
            case CC_DISPLAY_TYPE_MAX:
            default:
                //SCI_TRACE_LOW:"ccapp.c:enter function CC_UpdateCcDisplayType() default case WARRNING!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11322_112_2_18_2_11_5_363,(uint8*)"");
                break;
        }
    }
}
#else
/*****************************************************************************/
//  Description : create hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateHotButtons(MMI_WIN_ID_T win_id)
{
    int32 row = 0;
    int32 column = 0;
    MMI_CTRL_ID_T button_form_id = MMICC_CONNECTED_BUTTON_FORM_CTRL_ID;
    GUIBUTTON_INIT_DATA_T button_init = {0};
    GUIFORM_INIT_DATA_T button_form_init = {0};
    GUIFORM_DYNA_CHILD_T button_child_form_ctrl= {0};
    GUIFORM_INIT_DATA_T button_child_form_init = {0};
    MMI_CONTROL_CREATE_T button_form_create = {0};
    CC_CC_BUTTON_E button_type = CC_BUTTON_MAX;
    MMI_HANDLE_T    button_handle = 0;    
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
    CC_CC_BUTTON_E (*current_win_button_order_ptr)[CC_HOT_BUTTON_COLUMN_MAX] = PNULL;

    switch (win_id)
    {
        case MMICC_STATUS_WIN_ID:
            current_win_button_order_ptr= (CC_CC_BUTTON_E (*)[CC_HOT_BUTTON_COLUMN_MAX])&s_connected_win_button_order;
            break;

        case MMICC_ANIMATION_WIN_ID:
            if (MMICC_IsExistOutgoingCall())
            {
                current_win_button_order_ptr = (CC_CC_BUTTON_E (*)[CC_HOT_BUTTON_COLUMN_MAX])&s_mo_win_button_order;
            }
            else
            {
                current_win_button_order_ptr = (CC_CC_BUTTON_E (*)[CC_HOT_BUTTON_COLUMN_MAX])&s_mt_win_button_order;

#ifdef ASP_SUPPORT
                if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus() && !MMIAPIASP_IsMachineTurnOn() && !MMIAPIUDISK_UdiskIsRun())
                {
                    s_mt_win_button_order[CC_HOT_BUTTON_ROW_MAX - 1][0] = CC_BUTTON_ASP;
                }
#endif
            }
            break;
            
        default :
            return ;
    }

    button_form_init.both_rect.v_rect.left = MMICC_CONNECTED_BUTTON_FORM_MARGIN;
    button_form_init.both_rect.v_rect.right = client_rect.v_rect.right - MMICC_CONNECTED_BUTTON_FORM_MARGIN;
    button_form_init.both_rect.v_rect.top = MMICC_CONNECTED_BUTTON_FORM_TOP;
    button_form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom - 1;

    button_form_init.both_rect.h_rect.left = MMICC_CONNECTED_BUTTON_FORM_MARGIN_H;
    button_form_init.both_rect.h_rect.right = client_rect.h_rect.right - MMICC_CONNECTED_BUTTON_FORM_MARGIN_H;
    button_form_init.both_rect.h_rect.top = MMICC_CONNECTED_BUTTON_FORM_TOP_H;
    button_form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom - 1;
    button_form_init.layout_type = GUIFORM_LAYOUT_ORDER;

    button_form_create.ctrl_id = button_form_id;
    button_form_create.guid = SPRD_GUI_FORM_ID;
    button_form_create.init_data_ptr = &button_form_init;
    button_form_create.parent_win_handle = win_id;
    MMK_CreateControl(&button_form_create);

    //not slide
    GUIFORM_IsSlide(button_form_id,FALSE);
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    {
        GUI_BG_T bg_info = {0};
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_COMMON_BG;

        GUIFORM_SetBg(button_form_id, &bg_info);
    }
#endif
    for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
    {
        MMI_HANDLE_T child_form_handle = MMICC_CONNECTED_BUTTON_CHILD_FORM_0_CRTL_ID + row;
        
        button_child_form_init.layout_type = GUIFORM_LAYOUT_SBS;
        button_child_form_ctrl.child_handle = child_form_handle;
        button_child_form_ctrl.guid = SPRD_GUI_FORM_ID;
        button_child_form_ctrl.init_data_ptr = &button_child_form_init;
        button_child_form_ctrl.is_bg = FALSE;
        button_child_form_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, button_form_id,  &button_child_form_ctrl);

        for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
        {
            button_type = current_win_button_order_ptr[row][column];
            button_handle = s_cc_win_button[button_type].ctrl_id;
            if (CC_BUTTON_MAX != button_type)
            {
                //phone number edit init data
                button_init.bg.bg_type = GUI_BG_IMG;

                //set button(status on) bg
                if (CC_IsHotButtonDisplayOnImg(button_type))
                {
                    button_init.bg.img_id  = s_cc_win_button[button_type].img_on_id;
                }
                else
                {
                    button_init.bg.img_id  = s_cc_win_button[button_type].img_id;
                }
                
                //create phone number edit control
                button_child_form_ctrl.child_handle           = button_handle;
                button_child_form_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                button_child_form_ctrl.init_data_ptr     = &button_init;
                button_child_form_ctrl.is_bg = FALSE;
                button_child_form_ctrl.is_get_active = FALSE;
                GUIFORM_CreatDynaChildCtrl(win_id, child_form_handle,  &button_child_form_ctrl);

            }
        }
    } 

    CC_INSTANCE.current_win_button_order_ptr = current_win_button_order_ptr;
}

/*****************************************************************************/
//  Description : create one hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_CreateNewHotButton(MMI_WIN_ID_T win_id, int32 new_button_row, int32 new_button_column, CC_CC_BUTTON_E new_button_type)
{
    int32 row = 0;
    int32 column = 0;
    MMI_CTRL_ID_T button_form_id = MMICC_CONNECTED_BUTTON_FORM_CTRL_ID;
    GUIBUTTON_INIT_DATA_T button_init = {0};
    GUIFORM_INIT_DATA_T button_form_init = {0};
    GUIFORM_DYNA_CHILD_T button_child_form_ctrl= {0};
    GUIFORM_INIT_DATA_T button_child_form_init = {0};
    MMI_CONTROL_CREATE_T button_form_create = {0};
    CC_CC_BUTTON_E button_type = CC_BUTTON_MAX;
    MMI_HANDLE_T    button_handle = 0;    
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);

    DestroyCCHotButtonFormCtrl(win_id);
    
    button_form_init.both_rect.v_rect.left = MMICC_CONNECTED_BUTTON_FORM_MARGIN;
    button_form_init.both_rect.v_rect.right = client_rect.v_rect.right - MMICC_CONNECTED_BUTTON_FORM_MARGIN;
    button_form_init.both_rect.v_rect.top = MMICC_CONNECTED_BUTTON_FORM_TOP;
    button_form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom - 1;

    button_form_init.both_rect.h_rect.left = MMICC_CONNECTED_BUTTON_FORM_MARGIN_H;
    button_form_init.both_rect.h_rect.right = client_rect.h_rect.right - MMICC_CONNECTED_BUTTON_FORM_MARGIN_H;
    button_form_init.both_rect.h_rect.top = MMICC_CONNECTED_BUTTON_FORM_TOP_H;
    button_form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom - 1;
    button_form_init.layout_type = GUIFORM_LAYOUT_ORDER;

    button_form_create.ctrl_id = button_form_id;
    button_form_create.guid = SPRD_GUI_FORM_ID;
    button_form_create.init_data_ptr = &button_form_init;
    button_form_create.parent_win_handle = win_id;
    MMK_CreateControl(&button_form_create);

    //not slide
    GUIFORM_IsSlide(button_form_id,FALSE);
        
    for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
    {
        MMI_HANDLE_T child_form_handle = MMICC_CONNECTED_BUTTON_CHILD_FORM_0_CRTL_ID + row;
        
        button_child_form_init.layout_type = GUIFORM_LAYOUT_SBS;
        button_child_form_ctrl.child_handle = child_form_handle;
        button_child_form_ctrl.guid = SPRD_GUI_FORM_ID;
        button_child_form_ctrl.init_data_ptr = &button_child_form_init;
        button_child_form_ctrl.is_bg = FALSE;
        button_child_form_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, button_form_id,  &button_child_form_ctrl);

        for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
        {
            if (row == new_button_row && column == new_button_column)
            {
                button_type = new_button_type;
            }
            else
            {
                button_type = CC_INSTANCE.current_win_button_order_ptr[row][column];
            }
            button_handle = s_cc_win_button[button_type].ctrl_id;
            
            if (CC_BUTTON_MAX != button_type)
            {
                //phone number edit init data
                button_init.bg.bg_type = GUI_BG_IMG;

                //set button(status on) bg
                if (CC_IsHotButtonDisplayOnImg(button_type))
                {
                    button_init.bg.img_id  = s_cc_win_button[button_type].img_on_id;
                }
                else
                {
                    button_init.bg.img_id  = s_cc_win_button[button_type].img_id;
                }

                //create phone number edit control
                button_child_form_ctrl.child_handle           = button_handle;
                button_child_form_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                button_child_form_ctrl.init_data_ptr     = &button_init;
                button_child_form_ctrl.is_bg = FALSE;
                button_child_form_ctrl.is_get_active = FALSE;
                GUIFORM_CreatDynaChildCtrl(win_id, child_form_handle,  &button_child_form_ctrl);

                //set handle long press msg
            }
        }
    } 

    CC_UpdateCCWinHotButton(CC_BUTTON_MAX, FALSE); 
}

/*****************************************************************************/
//  Description :destory cc hot buttons
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DestroyCCHotButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    int32 cow = 0;
    int32 column = 0;
    // int32 index = 0;
    MMI_CTRL_ID_T ctrl_id = 0;
    CC_CC_BUTTON_E button_type = CC_BUTTON_MAX;
    if(0 != MMK_GetCtrlHandleByWin(win_id, MMICC_CONNECTED_BUTTON_FORM_CTRL_ID))
    {
        for (cow=0; cow<CC_HOT_BUTTON_ROW_MAX; cow++)
        {
            for(column=0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
            {
                button_type = CC_INSTANCE.current_win_button_order_ptr[cow][column];
                MMK_DestroyDynaCtrl(s_cc_win_button[button_type].ctrl_id);
            }
            ctrl_id = MMICC_CONNECTED_BUTTON_CHILD_FORM_0_CRTL_ID + cow;
            MMK_DestroyDynaCtrl(ctrl_id);
        }
        MMK_DestroyDynaCtrl(MMICC_CONNECTED_BUTTON_FORM_CTRL_ID);
    }
}
#endif

#ifdef MMI_PDA_SUPPORT    
/*****************************************************************************/
//  Description : set gray state of hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ConstructHangupButton(MMI_HANDLE_T win_handle)
{
#ifndef MMI_GUI_STYLE_TYPICAL
    GUIBUTTON_INIT_DATA_T button_init = {0};
    MMI_HANDLE_T    button_handle = 0;    
    MMI_CONTROL_CREATE_T        button_ctrl = {0};
    GUI_BG_T button_bg = {0};
    GUI_FONT_ALL_T  text_font={0};

    GUI_BOTH_RECT_T client_both_rect = MMITHEME_GetWinClientBothRect(win_handle);
    button_handle = MMICC_BUTTON_HANGUP_CTRL_ID;
    button_init.both_rect.v_rect.left   = CC_HANGUP_BUTTON_LEFT;
    button_init.both_rect.v_rect.right  = CC_HANGUP_BUTTON_RIGHT;
    button_init.both_rect.v_rect.top    = CC_HANGUP_BUTTON_TOP+ client_both_rect.v_rect.top;
    button_init.both_rect.v_rect.bottom = CC_HANGUP_BUTTON_BOTTOM+ client_both_rect.v_rect.top;

    button_init.both_rect.h_rect.left   = CC_HANGUP_BUTTON_LEFT_H;
    button_init.both_rect.h_rect.right  = CC_HANGUP_BUTTON_RIGHT_H;
    button_init.both_rect.h_rect.top    = CC_HANGUP_BUTTON_TOP_H + client_both_rect.h_rect.top;
    button_init.both_rect.h_rect.bottom = CC_HANGUP_BUTTON_BOTTOM_H + client_both_rect.h_rect.top;

    button_init.bg.bg_type = GUI_BG_IMG;
    
    button_ctrl.ctrl_id           = button_handle;
    button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    button_ctrl.init_data_ptr     = &button_init;
    button_ctrl.parent_win_handle = win_handle;
    MMK_CreateControl(&button_ctrl);

	text_font.color=MMI_CALL_ON_COLOR;
#ifdef MAINLCD_SIZE_320X480
    text_font.font=SONG_FONT_20;
#else
    text_font.font=SONG_FONT_16;
#endif
    GUIBUTTON_SetFont(button_handle, &text_font);
    GUIBUTTON_SetTextId(button_handle, TXT_CC_HANG_OFF);

    button_bg.bg_type = GUI_BG_IMG;
    button_bg.img_id = IMAGE_CC_HANGUP_BUTTON_BG_DEFAULT;
    GUIBUTTON_SetBg(button_handle, &button_bg);

    button_bg.img_id = IMAGE_CC_HANGUP_BUTTON_BG_PRESSED;
    GUIBUTTON_SetPressedBg(button_handle, &button_bg);    

    //HIDE hangup button
    if (CC_INSTANCE.cc_display_type != CC_DISPLAY_HOT_BUTTONS)
    {
        GUIBUTTON_SetVisible(button_handle, FALSE, FALSE);
    }
#endif
}
#endif //mmi_pda_support

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : update gray or not gray state of all hot buttons
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_UpdateHotButtonState(void)
{
    CC_CC_BUTTON_E button_index = CC_BUTTON_MAX;

    if(MMK_IsFocusWin(MMICC_DISCONNECT_WIN_ID) || MMK_IsFocusWin(MMICC_AUTO_DIAL_WIN_ID))
    {
        for (button_index=CC_BUTTON_HANDFREE; button_index<CC_BUTTON_MAX; button_index++)
        {
            s_cc_win_button[button_index].is_gray = TRUE;
        }
        return;
    }
    
    for (button_index=CC_BUTTON_HANDFREE; button_index<CC_BUTTON_MAX; button_index++)
    {
        s_cc_win_button[button_index].is_gray = FALSE;
    }
    
#ifdef MV_SUPPORT//魔音下不能设置免提
    if (IsMagicVoiceWorking() || MMICC_IsMagicVoiceOpenLater())
    {
        s_cc_win_button[CC_BUTTON_HANDFREE].is_gray = TRUE;
        s_cc_win_button[CC_BUTTON_RECORD].is_gray = TRUE;
#if !defined(TOUCH_PANEL_SUPPORT)
        CC_INSTANCE.cc_win_focus_button_type = CC_BUTTON_DIALPANEL;
#endif
    }
    else
#endif
    {
#if !defined(TOUCH_PANEL_SUPPORT)
        CC_INSTANCE.cc_win_focus_button_type = CC_BUTTON_DIALPANEL;
#endif
    }

    if (MMICC_IsAllowSendDtmf()
#if !defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_GUI_STYLE_TYPICAL)
    || MMICC_IsAllowAddNewCall()
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
    || !MMICC_IsEmergencyCall()
#endif
    )
    {
        s_cc_win_button[CC_BUTTON_DIALPANEL].is_gray = FALSE;
    }
    else
    {
        s_cc_win_button[CC_BUTTON_DIALPANEL].is_gray = TRUE;
    }

    if (MMICC_IsExistOutgoingCall())
    {
        s_cc_win_button[CC_BUTTON_MUTE].is_gray = TRUE;
        s_cc_win_button[CC_BUTTON_RECORD].is_gray = TRUE;
        s_cc_win_button[CC_BUTTON_HOLD].is_gray = TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL        
        s_cc_win_button[CC_BUTTON_MERGE_CALLS].is_gray = TRUE;
        s_cc_win_button[CC_BUTTON_SWAP_CALLS].is_gray = TRUE;
        s_cc_win_button[CC_BUTTON_RETRIEVE_CALLS].is_gray = TRUE;
        s_cc_win_button[CC_BUTTON_NEW_CALL].is_gray = TRUE;        
#endif        
        s_cc_win_button[CC_BUTTON_PB].is_gray = TRUE;
    }
    else
    {
        //根据当前CALL的状态进行相应的处理
        switch(MMICC_GetCurrentCallStatus())
        {
            case CC_CALL_STATE:
            case CC_HOLD_STATE:
#if !defined(MMI_GUI_STYLE_TYPICAL)
                if (CC_MAX_MPTY_NUM <= MMIAPICC_GetMPTYCallCount())
                {
                    s_cc_win_button[CC_BUTTON_MERGE_CALLS].is_gray = TRUE;
                }
#endif
                break;

            case CC_INCOMING_CALL_STATE:
            case CC_WAITING_STATE:
                s_cc_win_button[CC_BUTTON_MUTE].is_gray = TRUE;
                s_cc_win_button[CC_BUTTON_RECORD].is_gray = TRUE;
                s_cc_win_button[CC_BUTTON_HOLD].is_gray = TRUE;
                s_cc_win_button[CC_BUTTON_PB].is_gray = TRUE;
                break;

            case CC_INIT_STATE:
            case CC_CALLING_STATE:
            default:
                //SCI_TRACE_LOW:"ccapp.c:enter function CC_InitHotButtonState() default WARRNING!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11703_112_2_18_2_11_6_364,(uint8*)"");
                break;
        }
    }

#ifdef BT_DIALER_SUPPORT
    if(MMICC_GetBtCallFlag())
    {
        s_cc_win_button[CC_BUTTON_RECORD].is_gray = TRUE;
    }
#endif
}
/*****************************************************************************/
//  Description : get gray state of hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_GetHotButtonGrayState(CC_CC_BUTTON_E button_type)
{
    return s_cc_win_button[button_type].is_gray;
}
/*****************************************************************************/
//  Description : set gray state of hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_SetHotButtonGrayState(CC_CC_BUTTON_E button_type, BOOLEAN is_gray)
{
    s_cc_win_button[button_type].is_gray = is_gray;
}
#endif

#if !defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : handle cc window pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinWebMsg(
                              MMI_WIN_ID_T      win_id        //in:
                              )
{
    //SCI_TRACE_LOW:"ccapp HandleCCWinWebMsg button id: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11733_112_2_18_2_11_6_365,(uint8*)"d",CC_INSTANCE.cc_win_focus_button_type);

    if (CC_BUTTON_HANDFREE != CC_INSTANCE.cc_win_focus_button_type && MMIAPIDM_GetHandsetLock())//phone locked by dm,  forbid hot buttons except handfree
    {
        return ;
    }
    
    switch(CC_INSTANCE.cc_win_focus_button_type)
    {
    case CC_BUTTON_HANDFREE:
#ifdef MV_SUPPORT//魔音下不能设置免提
        if (IsMagicVoiceWorking() || MMICC_IsMagicVoiceOpenLater())
        {
            MMICC_OpenAlertWarningWin(TXT_MV_RUNNING);
        }
        else
#endif
        {
            if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            {
                if(0 == MMICC_GetMtPresetCodecRoute())
                {
                    MMICC_SetMtPresetCodecRoute(MMISRVAUD_ROUTE_SPEAKER);
                }
                else
                {
                    MMICC_SetMtPresetCodecRoute(0);
                }
            }
            else if(MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev())
            {
                EnableHandfreeMode(FALSE);
            }
            else
            {
                EnableHandfreeMode(TRUE);
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    	break;

    case CC_BUTTON_DIALPANEL:
        MMIAPIIDLE_EnterDialWin();
    	break;

    case CC_BUTTON_MUTE:
        EnableMuteMode((BOOLEAN)(!MMICC_IsCurrentCallMute()),TRUE);
        break;
#ifdef MMI_RECORD_SUPPORT
    case CC_BUTTON_RECORD:
        HandleRecordOpt(win_id,FALSE);
        break;
#endif
    case CC_BUTTON_HOLD:
#ifndef MMI_GUI_STYLE_TYPICAL        
    case CC_BUTTON_RETRIEVE_CALLS:
    case CC_BUTTON_SWAP_CALLS:
#endif        
        MMICC_HoldCall();
    	break;
        
#ifndef MMI_GUI_STYLE_TYPICAL
    case CC_BUTTON_MERGE_CALLS:
        MMICC_BuildMPTYCall();
        break;

    case CC_BUTTON_SILENCE:
        MMIAPICC_StopMtCallPlayingRing();
        break;
#endif

    case CC_BUTTON_PB:
        MMIAPIPB_OpenListAllGroupWin();
    	break;
        
#ifdef ASP_SUPPORT 
    case CC_BUTTON_ASP:
        if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            && !MMIAPIASP_IsMachineTurnOn())
        {
            if(!MMIAPIASP_WorkOneTime())
            {
                //SCI_TRACE_LOW:"ccapp HandleCCWinWebMsg !MMIAPIASP_WorkOneTime()"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11815_112_2_18_2_11_6_366,(uint8*)"");
            }
            if(!MMICC_AnswerASPCall())
            {
                //SCI_TRACE_LOW:"ccapp HandleCCWinWebMsg !AnswerASPCall()"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11819_112_2_18_2_11_6_367,(uint8*)"");
            }
        }
        break;
#endif
        
    default:
        //do nothing
        break;
    }
}

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : find next hot button except gray ones
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL CC_CC_BUTTON_E CC_FindNextHotButton(CC_CC_BUTTON_E focus_button_type, MMI_MESSAGE_ID_E msg_id)
{
    int32 row = 0;
    int32 column = 0;
    int32 current_row = 0;
    int32 current_column = 0;
    int32 next_row = 0;
    int32 next_column = 0;
    CC_CC_BUTTON_E next_focus_button_type = CC_BUTTON_DIALPANEL;
    BOOLEAN is_find_current_button = FALSE;
    BOOLEAN is_find_next_button = FALSE;

#ifdef MMI_GUI_STYLE_TYPICAL
    if ( PNULL == CC_INSTANCE.current_win_button_order_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_UpdateCCWinHotButton(), PNULL == s_current_win_button_order_ptr ERROR!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11850_112_2_18_2_11_6_368,(uint8*)"");
        return CC_BUTTON_DIALPANEL;
    }
#endif    
    
    for (row=0; row<CC_HOT_BUTTON_ROW_MAX && !is_find_current_button; row++)
    {
        for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX && !is_find_current_button; column++)
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            if ((CC_CC_BUTTON_E)CC_INSTANCE.cc_win_button_order[row][column] == focus_button_type)
#else
            if ((CC_CC_BUTTON_E)CC_INSTANCE.current_win_button_order_ptr[row][column] == focus_button_type)
#endif
            {
                is_find_current_button = TRUE;
                current_row = row;
                current_column = column;
            }
        }
    }

    if (!is_find_current_button)
    {
        //SCI_TRACE_LOW:"ccapp CC_FindNextHotButton didn't find the current focus button!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11874_112_2_18_2_11_6_369,(uint8*)"");
        return CC_BUTTON_HANDFREE;
    }
    
    switch (msg_id)
    {
    case MSG_APP_LEFT:
        if(current_column == 0 && current_row > 0)
        {
            next_row = current_row - 1;
            next_column = CC_HOT_BUTTON_COLUMN_MAX - 1;
        }
        else if(current_column == 0 && current_row == 0)
        {
            next_row = CC_HOT_BUTTON_ROW_MAX - 1;
            next_column = CC_HOT_BUTTON_COLUMN_MAX - 1;
        }
        else
        {
            next_row = current_row;
            next_column = current_column - 1;
        }
        
        for (row = next_row; row>=0; row--)
        {
            for (column = next_column; column>=0; column--)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
            next_column = CC_HOT_BUTTON_COLUMN_MAX - 1;
        }

        for (row=CC_HOT_BUTTON_ROW_MAX - 1; row>=0; row--)
        {
            for (column=CC_HOT_BUTTON_COLUMN_MAX - 1; column>=0; column--)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
        }
        //SCI_TRACE_LOW:"ccapp CC_FindNextHotButton didn't find the next focus button!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11931_112_2_18_2_11_6_370,(uint8*)"");
        break;
        
    case MSG_APP_RIGHT:
        if(current_column == CC_HOT_BUTTON_COLUMN_MAX - 1 && current_row < CC_HOT_BUTTON_ROW_MAX - 1)
        {
            next_row = current_row + 1;
            next_column = 0;
        }
        else if(current_column == CC_HOT_BUTTON_COLUMN_MAX - 1 && current_row == CC_HOT_BUTTON_ROW_MAX - 1)
        {
            next_row = 0;
            next_column = 0;
        }
        else
        {
            next_row = current_row;
            next_column = current_column + 1;
        }
        
        for (row = next_row; row<CC_HOT_BUTTON_ROW_MAX; row++)
        {
            for (column = next_column; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
            next_column = 0;
        }

        for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
        {
            for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
        }
        //SCI_TRACE_LOW:"ccapp CC_FindNextHotButton didn't find the next focus button!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_11985_112_2_18_2_11_6_371,(uint8*)"");
        break;

    case MSG_APP_UP:
        if(0 == current_row)
        {
            next_row = CC_HOT_BUTTON_ROW_MAX - 1;
        }
        else
        {
            next_row = current_row - 1;
        }
        next_column = current_column;
        
        for (row = next_row; row>=0; row--)
        {
            for (column = next_column; column>=0; column--)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
            next_column = CC_HOT_BUTTON_COLUMN_MAX - 1;
        }

        for (row=CC_HOT_BUTTON_ROW_MAX - 1; row>=0; row--)
        {
            for (column=CC_HOT_BUTTON_COLUMN_MAX - 1; column>=0; column--)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
        }
        //SCI_TRACE_LOW:"ccapp CC_FindNextHotButton didn't find the next focus button!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12033_112_2_18_2_11_7_372,(uint8*)"");
        break;

    case MSG_APP_DOWN:
        if(CC_HOT_BUTTON_ROW_MAX - 1 == current_row)
        {
            next_row = 0;
        }
        else
        {
            next_row = current_row + 1;
        }
        next_column = current_column;
        
        for (row = next_row; row<CC_HOT_BUTTON_ROW_MAX; row++)
        {
            for (column = next_column; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
            next_column = 0;
        }

        for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
        {
            for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                uint32 index = CC_INSTANCE.cc_win_button_order[row][column];
#else                
                uint32 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
#endif
                if (!s_cc_win_button[index].is_gray)
                {
                    is_find_next_button = TRUE;
                    return index;
                }
            }
        }
        //SCI_TRACE_LOW:"ccapp CC_FindNextHotButton didn't find the next focus button!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12081_112_2_18_2_11_7_373,(uint8*)"");
        break;

    default :
        break;
    }

    return next_focus_button_type;
}
#endif
#endif

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//  Description : set image for specify hot button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CC_SetHotButtonImg(CC_CC_BUTTON_E button_type, BOOLEAN is_focus_button)
{
    GUI_BG_T button_bg = {0};
        
    button_bg.bg_type = GUI_BG_IMG;
    button_bg.shape = GUI_SHAPE_RECT;

    if (s_cc_win_button[button_type].is_gray)
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        if (CC_IsHotButtonDisplayOnImg(button_type))
        {
            button_bg.img_id = s_cc_win_button[button_type].img_bg_on_id;
        }
        else
        {
            button_bg.img_id = s_cc_win_button[button_type].img_bg_id;
        }

        GUIBUTTON_SetGrayed(s_cc_win_button[button_type].ctrl_id,TRUE,&button_bg,FALSE);
#else
        if (CC_IsHotButtonDisplayOnImg(button_type))
        {
            button_bg.img_id = s_cc_win_button[button_type].gray_img_on_id;
        }
        else
        {
            button_bg.img_id = s_cc_win_button[button_type].gray_img_id;
        }

        GUIBUTTON_SetGrayed(s_cc_win_button[button_type].ctrl_id,TRUE,&button_bg,FALSE);
#endif
    }
#if !defined(TOUCH_PANEL_SUPPORT)
    else if (is_focus_button)
    {
#if defined(MMI_GUI_STYLE_TYPICAL)    
        if (CC_IsHotButtonDisplayOnImg(button_type))
        {
            button_bg.img_id = s_cc_win_button[button_type].focus_img_on_id;
        }
        else
        {
            button_bg.img_id = s_cc_win_button[button_type].focus_img_id;
        }
#else //MMI_GUI_STYLE_TYPICAL
        button_bg.img_id = IMAGE_CC_HANGUP_BUTTON_BG_DEFAULT; //设置焦点按钮的背景
#endif
        GUIBUTTON_SetGrayed(s_cc_win_button[button_type].ctrl_id,FALSE, PNULL,FALSE);
        GUIBUTTON_SetBg (s_cc_win_button[button_type].ctrl_id,  &button_bg);
        GUIBUTTON_SetPressedBg(s_cc_win_button[button_type].ctrl_id, &button_bg);
    }
#endif //TOUCH_PANEL_SUPPORT
    else
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIBUTTON_SetGrayed(s_cc_win_button[button_type].ctrl_id,FALSE,PNULL,FALSE);

        if (CC_IsHotButtonDisplayOnImg(button_type))
        {
            button_bg.img_id = s_cc_win_button[button_type].img_bg_on_id;
            GUIBUTTON_SetBg(s_cc_win_button[button_type].ctrl_id, &button_bg);
            button_bg.img_id = s_cc_win_button[button_type].img_bg_on_pressed_id;
            GUIBUTTON_SetPressedBg(s_cc_win_button[button_type].ctrl_id, &button_bg);
        }
        else
        {
            button_bg.img_id = s_cc_win_button[button_type].img_bg_id;
            GUIBUTTON_SetBg(s_cc_win_button[button_type].ctrl_id, &button_bg);
            button_bg.img_id = s_cc_win_button[button_type].img_bg_pressed_id;
            GUIBUTTON_SetPressedBg(s_cc_win_button[button_type].ctrl_id, &button_bg);
        }

        GUIBUTTON_SetRunSheen(s_cc_win_button[button_type].ctrl_id, FALSE);
#else
    if (CC_IsHotButtonDisplayOnImg(button_type))
        {
            button_bg.img_id = s_cc_win_button[button_type].img_on_id;
        }
        else
        {
            button_bg.img_id = s_cc_win_button[button_type].img_id;
        }

        GUIBUTTON_SetGrayed(s_cc_win_button[button_type].ctrl_id,FALSE, PNULL,FALSE);
        GUIBUTTON_SetBg (s_cc_win_button[button_type].ctrl_id, &button_bg);
        GUIBUTTON_SetPressedBg(s_cc_win_button[button_type].ctrl_id, &button_bg);
#endif
    }
}

/*****************************************************************************/
// 	Description : CC_FreshConnectedWinButton
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_IsHotButtonDisplayOnImg(CC_CC_BUTTON_E button_type)
{
    BOOLEAN result = FALSE;
    
    if (s_cc_win_button[button_type].is_gray)
    {
        result = FALSE;
    }
    else if ((CC_BUTTON_HANDFREE == button_type && MMIAPICC_IsHandFree())
        || (CC_BUTTON_MUTE == button_type && MMICC_IsCurrentCallMute() && !MMICC_IsExistOutgoingCall())
#ifdef MMI_RECORD_SUPPORT
        || (CC_BUTTON_RECORD == button_type && IsCCRecording())
#endif
        || (CC_BUTTON_HANGUP == button_type && !MMK_IsFocusWin(MMICC_DISCONNECT_WIN_ID))
        )
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : CC_FreshConnectedWinButton
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_UpdateCCWinHotButton(CC_CC_BUTTON_E button_type, BOOLEAN is_focus_button)
{
    uint32 row = 0; 
    uint32 column = 0;
    
#ifdef MMI_GUI_STYLE_TYPICAL
    if ( PNULL == CC_INSTANCE.current_win_button_order_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_UpdateCCWinHotButton(), PNULL == s_current_win_button_order_ptr ERROR!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12215_112_2_18_2_11_7_374,(uint8*)"");
        return ;
    }
#endif

    //update all buttons image
    if (CC_BUTTON_MAX == button_type)
    {
        for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
        {
            for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
            {
#ifndef MMI_GUI_STYLE_TYPICAL
                CC_SetHotButtonImg(CC_INSTANCE.cc_win_button_order[row][column], FALSE);
#else
                    {
                        uint16 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
                        MMK_SendMsg(s_cc_win_button[index].ctrl_id, MSG_CTL_PAINT, PNULL);                        
                    }
#endif
            }
        }
        
#if !defined(TOUCH_PANEL_SUPPORT)
        CC_SetHotButtonImg(CC_INSTANCE.cc_win_focus_button_type, TRUE);
#endif

        //paint all buttons(maybe it is a option window, so paint will be done after return to cc window)
        if (MMK_IsFocusWin(MMICC_STATUS_WIN_ID) ||MMK_IsFocusWin(MMICC_ANIMATION_WIN_ID) )
        {
            for (row=0; row<CC_HOT_BUTTON_ROW_MAX; row++)
            {
                for (column = 0; column<CC_HOT_BUTTON_COLUMN_MAX; column++)
                {
#ifndef MMI_GUI_STYLE_TYPICAL
                    MMK_SendMsg(s_cc_win_button[CC_INSTANCE.cc_win_button_order[row][column]].ctrl_id, MSG_CTL_PAINT, PNULL);
#else
                    {
                        uint16 index = CC_INSTANCE.current_win_button_order_ptr[row][column];
                        MMK_SendMsg(s_cc_win_button[index].ctrl_id, MSG_CTL_PAINT, PNULL);                        
                    }
#endif
                }
            }
        }
    }
    else
    {
        //update focus button image
        CC_SetHotButtonImg(button_type, is_focus_button);
        //paint all buttons(maybe it is a option window, so paint will be done after return to cc window)
        if (MMK_IsFocusWin(MMICC_STATUS_WIN_ID) ||MMK_IsFocusWin(MMICC_ANIMATION_WIN_ID) )
        {
            MMK_PostMsg(s_cc_win_button[button_type].ctrl_id, MSG_CTL_PAINT, PNULL, 0);
        }
    }
    //MMK_PostMsg(MMICC_BUTTON_HANGUP_CTRL_ID, MSG_CTL_PAINT, PNULL, 0);
}
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle CC window botton down msg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinButtonDownMsg(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_MESSAGE_ID_E keydown_msg_id = 0;
    MMI_MESSAGE_ID_E keyup_msg_id = 0;
    uint8 dtmf_ch = 0;
    MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID);

    switch (ctrl_id)
    {
    case MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY2_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY3_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY4_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY5_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY6_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY7_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY8_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY9_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY0_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEYHASH_BUTTON_CTRL_ID:
        //send dtmf
        keydown_msg_id = MSG_APP_1 + ctrl_id - MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID;

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_PlayDialTone(keydown_msg_id, TRUE);
#endif

        dtmf_ch = MMIAPICC_MsgidToAscii(keydown_msg_id);
        MMIAPICC_SendDtmfByChar(MN_DUAL_SYS_MAX, dtmf_ch);

        if (MMK_IsActiveCtrl(edit_ctrl_handle) || MMK_SetAtvCtrl(win_id,edit_ctrl_handle))
        {
            MMK_SendMsg(edit_ctrl_handle,keydown_msg_id,PNULL);
            MMK_SendMsg(edit_ctrl_handle,keyup_msg_id,PNULL);
        }
        else
        {
            //SCI_TRACE_LOW:"mmicc_app.c HandleCCWinPenOkMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12533_112_2_18_2_11_8_377,(uint8*)"");
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle CC window botton up msg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinButtonUpMsg(MMI_CTRL_ID_T ctrl_id)
{
    switch (ctrl_id)
    {
    case MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY2_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY3_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY4_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY5_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY6_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY7_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY8_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY9_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY0_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEYHASH_BUTTON_CTRL_ID:
        //Stop DTMF
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif

        MMIAPICC_StopDtmf(MN_DUAL_SYS_MAX);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle cc window pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleCCWinPenOkMsg(
                              MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     ctrl_id  //in:button ctrl
                              )
{
#ifdef BT_DIALER_SUPPORT
    BT_STATUS bt_status = BT_ERROR;
#endif
    
	if (MMICC_BUTTON_HANDFREE_CTRL_ID != ctrl_id && MMICC_BUTTON_ANSWER_CTRL_ID != ctrl_id && MMIAPIDM_GetHandsetLock())//window locked by dm,  forbid some hot button
    {
        return ;
    }

    switch(ctrl_id)
    {
    case MMICC_BUTTON_HANDFREE_CTRL_ID:
#ifdef MV_SUPPORT//魔音下不能设置免提
        if (IsMagicVoiceWorking() || MMICC_IsMagicVoiceOpenLater())
        {
            MMICC_OpenAlertWarningWin(TXT_MV_RUNNING);
        }
        else
#endif
        {
            if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            {
                if(0 == MMICC_GetMtPresetCodecRoute())
                {
                    MMICC_SetMtPresetCodecRoute(MMISRVAUD_ROUTE_SPEAKER);
                }
                else
                {
                    MMICC_SetMtPresetCodecRoute(0);
                }
            }        
            else if(MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev())
            {
                EnableHandfreeMode(FALSE);
            }
            else
            {
                EnableHandfreeMode(TRUE);
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }

    	break;

    case MMICC_BUTTON_DIALPANEL_CTRL_ID:
#if defined(JAVA_SUPPORT) && defined(LOW_MEMORY_SUPPORT)
        if (MMIAPIJAVA_IsJavaRuning())
        {
            MMICC_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
            break;
        }
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        if (MMIAPIKL_IsNeedPsw())
        {
            MMIAPIKL_OpenPswWin(TRUE, MMIAPIIDLE_EnterDialWin);    
        }
        else
#endif //MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        {
            CC_ShowDtmfPanel( win_id);
        }
#else
        {
            MMIAPIIDLE_EnterDialWin();
        }
#endif //MMI_GUI_STYLE_TYPICAL
        break;

    case MMICC_BUTTON_MUTE_CTRL_ID:
	    EnableMuteMode((BOOLEAN)(!MMICC_IsCurrentCallMute()),FALSE);
        break;
#ifdef MMI_RECORD_SUPPORT
    case MMICC_BUTTON_RECORD_CTRL_ID:
        HandleRecordOpt(win_id,FALSE);
        break;
#endif
    case MMICC_BUTTON_HOLD_CTRL_ID:
    case MMICC_BUTTON_RETRIVE_CALLS_CTRL_ID:
    case MMICC_BUTTON_SWAP_CALLS_CTRL_ID:
        MMICC_HoldCall();
        break;

    case MMICC_BUTTON_PB_CTRL_ID:
#if defined(JAVA_SUPPORT) && defined(LOW_MEMORY_SUPPORT)
        if (MMIAPIJAVA_IsJavaRuning())
        {
            MMICC_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
            break;
        }
#endif

#if !defined(MMI_GUI_STYLE_TYPICAL)&&defined(MMI_KEY_LOCK_EFFECT_PSW_SUPPORT)    
        if (MMIAPIKL_IsNeedPsw())
        {
            MMIAPIKL_OpenPswWin(TRUE, (MMI_PSW_FUNC)MMIAPIPB_OpenListAllGroupWin);    
        }
        else
#endif        
        {
            MMIAPIPB_OpenListAllGroupWin(); 
        }
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
    case MMICC_BUTTON_SILENCE_CTRL_ID:
        MMIAPICC_StopMtCallPlayingRing();
        break;
        
    case MMICC_BUTTON_HANGUP_CTRL_ID:
#ifdef BT_DIALER_SUPPORT   		
        if(MMICC_GetBtCallFlag())
        {
#ifdef MMI_MULTI_SIM_SYS_NONE        
		    MMIAPICC_HandleBtHangupButton();
#else
		    bt_status = MMIAPIBT_HfuHangup();
		    SCI_TRACE_LOW("ccapp.c: HandleCCWinPenOk  MMIAPIBT_HfuHangup  !!!!!");		
		    
            if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c:HandleCCWinPenOk() MMIAPIBT_HfuHangup fail");
            }
#endif
        }
        else
#endif
        {
            MMIAPICC_ReleaseCallByRedkey();
        }
        MMICC_StopRingOrVibrateBeforeCCing();
        break;
        
    case MMICC_BUTTON_ANSWER_CTRL_ID:
        MMICC_AnswerCall();
        break;

    case MMICC_BUTTON_NEW_CALL_CTRL_ID:
#if defined(MMI_KEY_LOCK_EFFECT_PSW_SUPPORT)
        if (MMIAPIKL_IsNeedPsw())
        {
            MMIAPIKL_OpenPswWin(TRUE, MMIAPIIDLE_EnterDialWin);    
        }
        else
#endif
        {
            MMIAPIIDLE_EnterDialWin();
        }
        break;

    case MMICC_BUTTON_MERGE_CALLS_CTRL_ID: 
        MMICC_BuildMPTYCall();
        break;
#endif

#ifdef ASP_SUPPORT 
    case MMICC_BUTTON_ASP_CTRL_ID:
        if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            && !MMIAPIASP_IsMachineTurnOn())
        {
            if(!MMIAPIASP_WorkOneTime())
            {
                //SCI_TRACE_LOW:"ccapp HandleCCWinWebMsg !MMIAPIASP_WorkOneTime()"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12422_112_2_18_2_11_7_375,(uint8*)"");
            }
            if(!MMICC_AnswerASPCall())
            {
                //SCI_TRACE_LOW:"ccapp HandleCCWinWebMsg !AnswerASPCall()"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12426_112_2_18_2_11_7_376,(uint8*)"");
            }
        }
        break;
#endif

        
#ifdef MMI_GUI_STYLE_TYPICAL
    case MMICC_MULTI_ACTIVE_CALL_PHOTO_ANIM_CTRL_ID:
    case MMICC_MULTI_ACTIVE_CALL_NAME_LABEL_CTRL_ID:
    case MMICC_MULTI_ACTIVE_CALL_TIME_LABEL_CTRL_ID:
        if(MMICC_IsExistActiveMPTY())
        {
            CC_OpenMultiCallList();
        }
        break;

    case MMICC_MULTI_HOLD_CALL_PHOTO_ANIM_CTRL_ID:
    case MMICC_MULTI_HOLD_CALL_NAME_LABEL_CTRL_ID:
    case MMICC_MULTI_HOLD_CALL_TIME_LABEL_CTRL_ID:
        if(MMICC_IsExistHoldMPTY())
        {
            CC_OpenMultiCallList();
        }
        break;

    case MMICC_CONNECTED_STATE_LABEL_CTRL_ID:
    case MMICC_CONNECTED_NAME_LABEL_CTRL_ID:
    case MMICC_CONNECTED_TIME_LABEL_CTRL_ID:
    case MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID:
    case MMICC_CONNECTED_INFO_FORM_CTRL_ID:  
        if(1 < MMICC_GetCallNum())
        {
            CC_OpenMultiCallList();
        }
        break;
#else
    case MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID:
        if(MMICC_IsExistActiveMPTY() 
          || (MMICC_IsExistHoldMPTY() && !MMICC_IsExistActiveAndHoldCalls()))
        {
#ifdef MMI_MULTI_SIM_SYS_NONE
            BOOLEAN bSupport_Split = FALSE;
            BOOLEAN bSupport_end_specific_call = FALSE;		
            
            MMIAPIBT_HfuIsAGSupportRelSpecCallIndex(&bSupport_end_specific_call);
            MMIAPIBT_HfuIsAGSupportPrivateConsultMode(&bSupport_Split);
            
            SCI_TRACE_LOW("ccapp.c: HandleCCWinPenOk  bSupport_Split %d ,  bSupport_end_specific_call %d  !!!!!" , bSupport_Split , bSupport_end_specific_call);
            if(bSupport_Split && bSupport_end_specific_call)
            {
                CC_OpenMultiCallList();
            }
#else
            CC_OpenMultiCallList();
#endif
        }
        break;

    case MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID:
        if(MMICC_IsExistHoldMPTY())
        {
            CC_OpenMultiCallList();
        }
        break;
        
    case MMICC_BUTTON_PDA_HOLD_CALL_CTRL_ID:
    case MMICC_BUTTON_PDA_ACTIVE_CALL_CTRL_ID:
    case MMICC_BUTTON_PDA_SWAP_CALL_CTRL_ID:
        MMICC_HoldCall();
        break;

    case MMICC_BUTTON_PDA_OPTION_CTRL_ID:
        MMK_PostMsg(win_id, MSG_APP_MENU, PNULL, 0);
        break; 

    case MMICC_BUTTON_PDA_DIAL_CALL_CTRL_ID:
        if (MMICC_AUTO_DIAL_WIN_ID == win_id)
        {
            MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
        }
        break;
        
    case MMICC_BUTTON_PDA_END_CALL_CTRL_ID:
        if (MMICC_AUTO_DIAL_WIN_ID == win_id)
        {
            MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
        }
        break;

#if 0
    case MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY2_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY3_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY4_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY5_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY6_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY7_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY8_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY9_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEY0_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_KEYHASH_BUTTON_CTRL_ID:
        {
            uint8 dtmf_ch = 0; 
            MMI_MESSAGE_ID_E    keydown_msg_id = 0;
            MMI_MESSAGE_ID_E    keyup_msg_id = 0;
            MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID);
            
            //send dtmf
            keydown_msg_id = MSG_APP_1 + ctrl_id - MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID;
            keyup_msg_id = MSG_KEYUP_1 + ctrl_id - MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID; 
            MMIAPIIDLE_PlayDialTone(keydown_msg_id, FALSE);
            dtmf_ch = MMIAPICC_MsgidToAscii(keydown_msg_id);
            MMIAPICC_SendDtmfByChar(MN_DUAL_SYS_MAX,dtmf_ch);

            if (MMK_IsActiveCtrl(edit_ctrl_handle) || MMK_SetAtvCtrl(win_id,edit_ctrl_handle))
            {
                MMK_SendMsg(edit_ctrl_handle,keydown_msg_id,PNULL);
                MMK_SendMsg(edit_ctrl_handle,keyup_msg_id,PNULL);
            }
            else
            {
                //SCI_TRACE_LOW:"mmicc_app.c HandleCCWinPenOkMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12533_112_2_18_2_11_8_377,(uint8*)"");
            }
        }
        break;
#endif
        
    case MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID:
    case MMICC_CONNECTED_STATE_LABEL_CTRL_ID:
    case MMICC_CONNECTED_NAME_LABEL_CTRL_ID:
    case MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID:
#ifdef MMI_CALLCONTROL_MINI_SUPPORT    	
    case MMICC_CONNECTED_TIME_LABEL_CTRL_ID:
#endif    	
    case MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID:
    case MMICC_CONNECTED_REDIAL_TIMES_LABEL_CTRL_ID:
    case MMICC_CONNECTED_HOLD_STATE_LABEL_CTRL_ID:
    case MMICC_CONNECTED_HOLD_NAME_LABEL_CTRL_ID:
    case MMICC_CONNECTED_HOLD_NUMBER_LABEL_CTRL_ID:
    case MMICC_CONNECTED_PDA_HOLD_LABEL_CTRL_ID:
    case MMICC_CONNECTED_PDA_ACTIVE_LABEL_CTRL_ID:
    case MMICC_CONNECTED_PDA_SWAP_LABEL_CTRL_ID:
    case MMICC_CONNECTED_PDA_SIM_LABEL_CTRL_ID:
    case MMICC_CONNECTED_PDA_OPTION_LABEL_CTRL_ID:
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL)
        {
            CC_HideDtmfPanel(win_id);
        }
        break;
#endif

    default:
        //do nothing
        break;
    }
}
#endif

/*****************************************************************************/
// 	Description : the process KBD,timer message function of the CC application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL MMI_RESULT_E  CC_HandleCcWinMsg(
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    uint8 call_reminder_time = CC_CALL_REMINDER_TIME_DEFAULT;
#ifdef BT_DIALER_SUPPORT
    BT_STATUS bt_status = BT_ERROR;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
    if (MMICC_IsExistOutgoingCall())
    {
        return CC_HandleCcAnimWinMsg(win_id, msg_id, param);
    }
#endif

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    //check the param
    if (MSG_TIMER != msg_id || CC_INSTANCE.call_time_count_timer != *(uint8*)param)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleCcWinMsg() msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12594_112_2_18_2_11_8_378,(uint8*)"d", msg_id);
    }

    //CC界面，全键盘工程，默认需对共用的数字按键进行消息转换。
    if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
        || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION))
    {
        msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
    }
    
#if defined(MMI_GUI_STYLE_TYPICAL)||(!defined(MMI_GUI_STYLE_TYPICAL)&&!defined(TOUCH_PANEL_SUPPORT))
    //send dtmf when press right key in call state
    if(((msg_id >= MSG_KEYDOWN_1) && (msg_id <= MSG_KEYDOWN_HASH)) 
        || ((msg_id >= MSG_APP_1) && ( msg_id <= MSG_APP_HASH)) 
#if 0
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        || (MSG_CTL_MIDSK == msg_id)
#endif
#ifdef TOUCH_PANEL_SUPPORT
        || (MSG_APP_WEB == msg_id)
#endif 
#endif
     )
    {
        if ( !MMICC_IsExistOperatedCall() && MMICC_IsAllowSendDtmf())
        {
            MMICC_SendSingleDtmf(msg_id);
            CC_OpenDialingWin(msg_id); 
        }
        else if(MMICC_IsAllowAddNewCall())
        {
            CC_OpenDialingWin(msg_id); 
        }
    }
#endif
#if 0
    if (MSG_KEYDOWN_HEADSET_BUTTON == msg_id)      //耳机button按下，对应ok或cancel操作        
    {
        if (GPIO_CheckHeadsetStatus() && MMI_NONE_TIMER_ID == g_mmi_headset_time_id)  //确认耳机是插入的，并且不在耳机定时器保护区间内
        {
            if (CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            {
                msg_id = MSG_APP_OK;
            }
            else if (CC_WAITING_STATE == MMICC_GetCurrentCallStatus())
            {
                if (!MMICC_IsExistActiveAndHoldCalls() && !MMICC_IsExistMPTY())
                {
                    //fix for CR82049,button按下保持并应答
                    msg_id = MSG_CC_ACCEPT_WAIT_CALL_MSG;	
                }
                else
                {
                    msg_id = MSG_APP_OK;                
                }
            }
            else
            {
                msg_id = MSG_BT_CANCEL_CALL;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ccapp.c CC_HandleCcWinMsg() headset is plugout!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12652_112_2_18_2_11_8_379,(uint8*)"");
        }
    }
#endif
    
    SCI_TRACE_LOW("[MMICC] CC_HandleCcWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        //MMK_SetWinBackgroundImageId(win_id, IMAGE_COMMON_BG);        
#endif
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
        if (MMICC_IsSendingStoredDtmf())
        {
            promptSendDtmfAfterCallSetup();
        }
        else
        {
            //do nothing
        }
}  
        
#ifdef MMI_PDA_SUPPORT
        if (MMICC_GetCallNum() <= 1)//first call
        {
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_UnlockPhone();
#endif
        }
#endif        

        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
        CC_CreateDtmfPanel(win_id);
#endif
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        CC_UpdateHotButtonState();
#endif
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CC_CONNECT_OPEN_WINDOW:
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif  
{
        if (MMICC_IsSendingStoredDtmf())
        {
            promptSendDtmfAfterCallSetup();
        }
}

        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);

#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif 
{
#ifdef MV_SUPPORT
        //@fen.xie MS00177206:MO/MT时选择取消键则不开启魔音效果
        MMIAPICC_EnableMV();
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
        if (MMIAPISET_GetAutoRecordSetting()
        #ifdef ASP_SUPPORT            
        &&(!MMIAPIASP_IsMachineWorking())
        #endif
        )
        {
#if 0        
#ifdef BLUETOOTH_SUPPORT
            BT_ADDRESS		address={0};
            if(!MMIAPIBT_GetActivatedHS(&address))
#endif
#endif
#ifdef MMI_RECORD_SUPPORT
            {        
                HandleRecordOpt(win_id,TRUE);
            }  
#else
            {
            }
#endif            
        }
#endif
}
        break;

    case MSG_CC_CONNECT_UPDATE_BUTTON:
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_CreateHotButtonsWithoutForm(win_id);
#endif
#ifndef MMI_GUI_STYLE_TYPICAL        
        //处理呼出界面打开拨号盘接通后，时间显示区域显示重叠问题
        if(
        CC_DISPLAY_DTMF_PANEL == CC_INSTANCE.cc_display_type
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        || (CC_DISPLAY_HOT_BUTTONS == CC_INSTANCE.cc_display_type && win_id == MMK_GetFocusWinId())
#endif
            )
        {
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
#endif        
        break;
#endif

    case MSG_NOTIFY_UPDATE:
        {
            GUI_RECT_T* update_rect_ptr = (GUI_RECT_T*)param;
#ifndef MMI_GUI_STYLE_TYPICAL
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_THEME_BG,
                &lcd_dev_info);
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_CC_COMMON_BG_PDA,
                &lcd_dev_info);
#else
            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
#endif
        }
        break;
      
    case MSG_MMICC_STARTTIMER:
        if ( MMI_NONE_TIMER_ID == CC_INSTANCE.call_time_count_timer )
        {
            CC_INSTANCE.call_time_count_timer = MMK_CreateWinTimer(CC_APPLET_HANDLE, CC_TIMER_OUT, TRUE );
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/17
        CC_SetCallWinSoftkey(win_id,STXT_MENU,TXT_NULL,STXT_EXIT,FALSE);
#else			
        if(!MMIAPIDM_GetHandsetLock())
        {
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
            SetHandFreeMenu(win_id, STXT_MENU, TXT_NULL, TXT_NULL,FALSE);
#else 
    {
        if(MMIAPICC_IsHandFree())
        {
#ifdef GUI_MID_SOFTKEY_USE_IMG
            SetHandFreeMenu(win_id, STXT_MENU, IMAGE_COMMON_SOFTKEY_EARPIECE_ICON, TXT_NULL, FALSE);
#else
            SetHandFreeMenu(win_id, STXT_MENU, TXT_HAND_ON, TXT_NULL, FALSE);
#endif
        }
        else
        {
#ifdef GUI_MID_SOFTKEY_USE_IMG
            SetHandFreeMenu(win_id, STXT_MENU, IMAGE_COMMON_SOFTKEY_HANDFREE_ICON, TXT_NULL, FALSE);
#else
            SetHandFreeMenu(win_id, STXT_MENU, TXT_COMMON_HAND_FREE, TXT_NULL, FALSE);
#endif
        }
    }
#endif
        }
        else
        {
            SetHandFreeMenu(win_id, TXT_NULL, TXT_NULL, TXT_NULL,FALSE);
        }	
#endif 
#ifndef MMI_PDA_SUPPORT
        SetConectedWinRightSoftkey(win_id, FALSE);
#endif
        DisplayWinBg(win_id, FALSE);
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
        if(MMICC_ANIMATION_WIN_ID == win_id)
        {
            UILAYER_Clear(&s_animwin_dtmfpanel_layer);
        }
        else if(MMICC_STATUS_WIN_ID == win_id)
        {
            UILAYER_Clear(&s_connectedwin_dtmfpanel_layer);
        }
#endif

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        CC_UpdateHotButtonState();
        CC_UpdateCCWinHotButton(CC_BUTTON_MAX, FALSE);
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
        if (CC_INSTANCE.is_need_light_on_cc_win)
        {
            CC_INSTANCE.is_need_light_on_cc_win = FALSE;
            MMIDEFAULT_TurnOnBackLight();
        }
        MMIDEFAULT_StopLcdBackLightTimer();
        MMIDEFAULT_StartLcdBackLightTimer();
#ifdef MMI_CALLCONTROL_MINI_SUPPORT
        if (MMK_GetFocusWinHandle() == MMK_ConvertIdToHandle(win_id))
        {
            GUIANIM_ResumeAnim(MMICC_BOTTOM_PHOTO_ANIM_CTRL_ID);
        }
#endif        
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
        /*if(MMI_NONE_TIMER_ID !=  CC_INSTANCE.call_time_count_timer )
        {
            MMK_StopTimer( CC_INSTANCE.call_time_count_timer );
            CC_INSTANCE.call_time_count_timer = MMI_NONE_TIMER_ID;
        }*/

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
        if(MMICC_ANIMATION_WIN_ID == win_id)
        {
            SCI_MEMSET(&s_animwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));//UILAYER_ReleaseLayer(&s_animwin_dtmfpanel_layer);
        }
        else if(MMICC_STATUS_WIN_ID == win_id)
        {
            SCI_MEMSET(&s_connectedwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));//UILAYER_ReleaseLayer(&s_connectedwin_dtmfpanel_layer);
        }
#endif
        if (MMI_NONE_TIMER_ID != CC_INSTANCE.dtmf_timer_id)
        {
            MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
            CC_INSTANCE.dtmf_timer_id = MMI_NONE_TIMER_ID;
        }
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;
        
    //cr NEWMS00116928    
    case MSG_BACKLIGHT_TURN_ON:
    case MSG_GET_FOCUS:
#ifndef MMI_GUI_STYLE_TYPICAL
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        if (!MMICC_IsExistActiveCall() && CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL)
        {
            CC_UpdateCcDisplayType(CC_DISPLAY_DTMF_PANEL);
        } 
        CC_DisplayByType(win_id);
#endif
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        MMICC_UpdateCurrentCallStatus(FALSE);
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif 
{
        if (MMICC_IsSendingStoredDtmf())
        {
            promptSendDtmfAfterCallSetup();
        }
}
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
		CC_ReappendDtmfPanelLayer(win_id);
#endif        
        break;

    case MSG_TIMER:
        //received signal of timer when timer of cc is time out
        if(CC_INSTANCE.call_time_count_timer == *(uint8*)param)
        {
            MMICC_SetCallTimeCount(MMICC_GetCallTimeCount() + 1);//s_call_context.call_time_count++;
            if ((!MMICC_IsExistOperatedCall())
                && (CC_CALL_STATE == MMICC_GetCurrentCallStatus()))
            {
                CC_CallTimeDisplay();
            }
            call_reminder_time = MMIAPISET_GetMinuteReminderTime();
            //判断此时是否需要发提示音
            if(call_reminder_time > 0
                && ((call_reminder_time == MMICC_GetCallTimeCount() % CC_MINUTE_TO_SECOND) // 1-59秒提醒
                || (call_reminder_time == CC_MINUTE_TO_SECOND && MMICC_GetCallTimeCount() % CC_MINUTE_TO_SECOND == 0)))// 60秒提醒
            {
                //发提示音
                MMIAPISET_PlayRing(MMIAPICC_GetCallDualSys(), FALSE,0,1,MMISET_RING_TYPE_MINHINT_IN_CALL,PNULL);
            }
        }
        else if (
#ifdef BT_DIALER_SUPPORT
            !MMICC_GetBtCallFlag() &&
#endif
            CC_INSTANCE.dtmf_timer_id == *(uint8*)param)
        {
            //send dtmf if exist extension number
            if(CC_RESULT_SUCCESS != SendStoredDtmf())
            {
                //SCI_TRACE_LOW:"ccapp.c:CC_SetupCompleteCnf() send dtmf failure"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12817_112_2_18_2_11_8_380,(uint8*)"");
            }

            if (MMICC_IsSendingStoredDtmf())
            {
                MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
                CC_INSTANCE.dtmf_timer_id = MMI_NONE_TIMER_ID;
                CC_INSTANCE.dtmf_timer_id = MMK_CreateWinTimer(win_id, CC_DTMF_WAITING_TIME, FALSE);
            }
        }
        break;

    case MSG_LCD_SWITCH:
        {
            GUI_FONT_T font = MMI_DEFAULT_NORMAL_FONT;
            if(MMK_IsWindowLandscape(win_id))
            {
                font = MMI_DEFAULT_SMALL_FONT;
            }
            else
            {
                font = MMI_DEFAULT_NORMAL_FONT;
            }
            GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][NAME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);
            GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_ACTIVE][TIME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);
            GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][NAME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);
            GUILABEL_SetFont(s_multi_call_display_ctrl[CC_MULTI_CALL_HOLD][TIME_LABEL_ID], font, MMI_DEFAULT_TEXT_COLOR);        
        }
#if defined(TOUCH_PANEL_SUPPORT)
        MMIAPIIDLE_SetDtmfEditFormParam(win_id);
#endif
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MAINLCD_SIZE_320X480) && !defined(MMI_GUI_STYLE_TYPICAL)
        if(MMICC_ANIMATION_WIN_ID == win_id)
        {
            MMIAPIIDLE_SetDtmfPanelLayerPosition(win_id, s_animwin_dtmfpanel_layer);
            //SCI_MEMSET(&s_animwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));
        }
        else if(MMICC_STATUS_WIN_ID == win_id)
        {
            MMIAPIIDLE_SetDtmfPanelLayerPosition(win_id, s_connectedwin_dtmfpanel_layer);
            //SCI_MEMSET(&s_connectedwin_dtmfpanel_layer, 0x0, sizeof(GUI_LCD_DEV_INFO));
        }
#endif
        break;

#if defined(TOUCH_PANEL_SUPPORT)        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    //case MSG_APP_UPSIDE:
    //case MSG_APP_DOWNSIDE:
    //case MSG_APP_VOL_UP:
    //case MSG_APP_VOL_DOWN:    
        {
            OpenVolumePanel(win_id, msg_id);
        }
        break;
#endif

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:        
#if !defined(TOUCH_PANEL_SUPPORT)
    case MSG_APP_UP:
    case MSG_APP_DOWN:
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        if (!MMK_IsOpenWin(MMICOMMON_PANEL_WIN_ID))
        {
            CC_UpdateCCWinHotButton(CC_INSTANCE.cc_win_focus_button_type, FALSE);
            CC_INSTANCE.cc_win_focus_button_type = CC_FindNextHotButton(CC_INSTANCE.cc_win_focus_button_type, msg_id);
            CC_UpdateCCWinHotButton(CC_INSTANCE.cc_win_focus_button_type, TRUE);        
        }
        else
#endif
        {
            if(MSG_APP_UP == msg_id || MSG_APP_DOWN == msg_id)
            {
                OpenVolumePanel(win_id, msg_id);
            }
            
        }
#else
#if 0
        {
            if(MSG_APP_UP == msg_id || MSG_APP_DOWN == msg_id)
            {
                OpenVolumePanel(win_id, msg_id);
            }
        }
#endif
#endif
        break; 
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
#if !defined(TOUCH_PANEL_SUPPORT)
    case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        HandleCCWinWebMsg(win_id);
        break;
#endif
#else
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        EnableHandfreeMode(!MMIAPICC_IsHandFree()); /*lint !e730*/
        SetConectedWinRightSoftkey(win_id, TRUE);
        break;

#endif

#ifndef PRINTSCREEN_SUPPORT 
#ifdef MMI_RECORD_SUPPORT
    case MSG_APP_CAMERA:
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
        HandleRecordOpt(win_id,FALSE);
}
        break;
#endif
#endif

    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
        MMICC_HoldCall();
}
        break;
        
        
    case MSG_CC_ACCEPT_WAIT_CALL_MSG:
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
        if( !MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall() )
        {
            //can not do other handle in this state
            return recode;
        }
        MMICC_HoldAndAnswerCW();
}
        break;

    case MSG_APP_OK://user press OK
    case MSG_CTL_OK:	
	case MSG_APP_MENU:
        if(!MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall())
        {
            //can not do other handle in this state
            return recode;
        }
        //根据当前CALL的状态进行相应的处理
        switch(MMICC_GetCurrentCallStatus())
        {
        case CC_INIT_STATE:
            break;
            
        case CC_CALLING_STATE:
            ClearAutoRedialInfo();
            break;

        case CC_CALL_STATE:
        case CC_HOLD_STATE:
            if(!MMIAPIDM_GetHandsetLock())
            {
                OpenCCHoldMenu();
            }
            break;

        case CC_WAITING_STATE:
            //显示菜单
            CreateCCWaitingMenu();
            break;

        case CC_INCOMING_CALL_STATE:
            //accept call
            if(!MMICC_AnswerCall())			
            {
                //显示失败
            }
            break;
        default:
            //SCI_TRACE_LOW:"ccapp.c: CC_HandleCcWinMsg() call_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_12941_112_2_18_2_11_8_381,(uint8*)"d", MMICC_GetCurrentCallStatus());
            break;
        }
        
        break;
        
    case MSG_APP_RED:
        //正在保持，正在分离多方通话等状态，底层消息尚未处理
        if(!MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall() )
        {
            recode = MMI_RESULT_TRUE;
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }            
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:
        recode =MMI_RESULT_FALSE;
        break;
#endif
        
    case MSG_BT_CANCEL_CALL: //blue tooth headset cancel the call
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
        if(!MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall() )
        {
            //can not do other handle in this state
            return recode;
        }
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
        //BQB测试中要求先释放active的电话
        if(MMICC_IsExistActiveCall())
        {
            MMICC_ReleaseActiveCall();     //释放active的所有电话
            return recode;
        }
#endif //BT_BQB_SUPPORT
        
        //根据当前CALL的状态进行相应的处理
        switch(MMICC_GetCurrentCallStatus())
        {
        case CC_INIT_STATE:
            break;
            
        case CC_CALL_STATE:
            ClearAutoRedialInfo();
            MMICC_ReleaseActiveCall();     //释放active的所有电话
            break;
            
        case CC_HOLD_STATE:
            ClearAutoRedialInfo();
            MMICC_ReleaseHoldCall();       //释放hold的所有电话
            break;
            
        case CC_CALLING_STATE:
            MMICC_ClearWantCallInfo();
            ClearAutoRedialInfo();
            //hang up call
            if(CC_RESULT_ERROR == MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING))
            {
                //显示失败
            }
            break;
            
        case CC_INCOMING_CALL_STATE:
            ClearAutoRedialInfo();
            //hang up call
            if(CC_RESULT_ERROR == MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY ))
            {
                //显示失败
            }
            break;
            
        case CC_WAITING_STATE:
            ClearAutoRedialInfo();
            //hang up call
            if(CC_RESULT_ERROR == MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY ))
            {
                //显示失败
            }
            break;
            
        default:
            //SCI_TRACE_LOW:"ccapp.c: CC_HandleCcWinMsg() call_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13025_112_2_18_2_11_8_382,(uint8*)"d", MMICC_GetCurrentCallStatus());
            break;
        }
}
        break;

    case MSG_LOSE_FOCUS:
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT
   case MSG_CTL_BUTTON_DOWN:
        HandleCCWinButtonDownMsg(win_id, ((MMI_NOTIFY_T*)param)->src_id);
        break;

   case MSG_CTL_BUTTON_UP:
        HandleCCWinButtonUpMsg(((MMI_NOTIFY_T*)param)->src_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
        HandleCCWinPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef DPHONE_SUPPORT
   case MSG_KEYDOWN_HOOK:
   case MSG_KEYUP_HOOK:
   case MSG_KEYDOWN_HANDSFREE:
   	    recode = MMI_RESULT_FALSE;
   	    break;    
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_point = {0};
            
            GUI_RECT_T active_anim_rect = {0};
            BOOLEAN is_active_anim_rect = FALSE;
            GUI_RECT_T hold_anim_rect = {0};
            BOOLEAN is_hold_anim_rect = FALSE;
            GUI_BOTH_RECT_T dtmf_both_rect = {0};
            BOOLEAN is_dtmf_rect = FALSE;
            
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            GUIANIM_GetDisplayRect(MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID, &active_anim_rect);
            is_active_anim_rect = GUI_PointIsInRect(tp_point,active_anim_rect);
            GUIANIM_GetDisplayRect(MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID, &hold_anim_rect);
            is_hold_anim_rect = GUI_PointIsInRect(tp_point,hold_anim_rect);

            dtmf_both_rect.v_rect.left = 0;
            dtmf_both_rect.v_rect.top = MMI_MAINSCREEN_HEIGHT - MMI_DTMF_PANEL_HEIGHT - 1;
            dtmf_both_rect.v_rect.right = MMI_MAINSCREEN_WIDTH;
            dtmf_both_rect.v_rect.bottom = MMI_MAINSCREEN_HEIGHT;

            dtmf_both_rect.h_rect.left = MMI_MAINSCREEN_HEIGHT - MMI_DTMF_PANEL_WIDTH_H - 1;
            dtmf_both_rect.h_rect.top = MMI_MAINSCREEN_WIDTH - MMI_DTMF_PANEL_HEIGHT_H - 1;
            dtmf_both_rect.h_rect.right = MMI_MAINSCREEN_HEIGHT;
            dtmf_both_rect.h_rect.bottom = MMI_MAINSCREEN_WIDTH;
            
            if(!MMITHEME_IsMainScreenLandscape())
            {
                is_dtmf_rect = GUI_PointIsInRect(tp_point,dtmf_both_rect.v_rect);
            }
            else
            {
                is_dtmf_rect = GUI_PointIsInRect(tp_point,dtmf_both_rect.h_rect);
            }
            
            if (CC_INSTANCE.cc_display_type != CC_DISPLAY_HOT_BUTTONS)
            {
                if (!is_dtmf_rect)
                {
                    CC_HideDtmfPanel(win_id);
                }
            }
            else if (is_active_anim_rect)
            {
                HandleCCWinPenOkMsg(win_id, MMICC_CONNECTED_PHOTO_ANIM_CTRL_ID);
            }
            else if (is_hold_anim_rect)
            {
                HandleCCWinPenOkMsg(win_id, MMICC_CONNECTED_HOLD_PHOTO_ANIM_CTRL_ID);
            }
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    //crNEWMS00114847,响应关机键
    case MSG_KEYLONG_POWER:
    	recode = MMI_RESULT_FALSE;
    	break;	

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_GUI_STYLE_TYPICAL
        if (CC_INSTANCE.cc_display_type == CC_DISPLAY_DTMF_PANEL)
        {
            CC_HideDtmfPanel(win_id);
        }
        else
#endif
#if defined(MMI_PDA_SUPPORT)
        {
#if defined(TOUCH_PANEL_SUPPORT)
#if defined(MMI_CALL_MINIMIZE_SUPPORT)
            MMIAPICC_HideCCApplet();
            MMK_ReturnIdleWin();
#else        
            recode = MMI_RESULT_FALSE;
#endif//MMI_CALL_MINIMIZE_SUPPORT
#else
            MMIAPICC_ReleaseCallByRedkey();
#endif//TOUCH_PANEL_SUPPORT
        }
#else
        {
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
            EnableHandfreeMode(!MMIAPICC_IsHandFree()); /*lint !e730*/
            SetConectedWinRightSoftkey(win_id, TRUE);
 #else
 #ifdef BT_DIALER_SUPPORT
            if(MMICC_GetBtCallFlag())
            {
                if(MMICC_GetBtConnWaitCallFlag())
                {
                    MMICC_SetBtConnWaitCallFlag(FALSE);
                    bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_HELD_CALLS);
                    if(BT_SUCCESS !=  bt_status && BT_PENDING != bt_status)
                    {
                        SCI_TRACE_LOW("ccapp.c:CC_BtCallCcwaInd() MMIAPIBT_HfuCallHold fail");
                    }
                    return MMI_RESULT_TRUE; 
                }
                bt_status = MMIAPIBT_HfuHangup();
                if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
                {
                    SCI_TRACE_LOW("ccapp.c:CC_HandleCcWinMsg() MMIAPIBT_HfuHangup fail");
                }
            }
            else
 #endif
            {
                MMIAPICC_ReleaseCallByRedkey();       
            }
#endif
        }
#endif
        break;
        
   default:
        recode = MMI_RESULT_FALSE;
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleCcWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13135_112_2_18_2_11_9_383,(uint8*)"d", msg_id);
        break;
    }
    
    return recode;
}


/*****************************************************************************/
// 	Description : the process message function of the Disconnect window
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleDisconnectWinMsg( 
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint8       tele_num[ CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8       tele_len = 0;
#ifdef BT_DIALER_SUPPORT
    BT_STATUS bt_status = BT_ERROR;
#endif

    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleDisconnectWinMsg(),msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13157_112_2_18_2_11_9_384,(uint8*)"d", msg_id);
    
    SCI_TRACE_LOW("[MMICC] CC_HandleDisconnectWinMsg, msg_id = 0x%x", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

#ifndef MMI_GUI_STYLE_TYPICAL
//        MMK_SetWinBackgroundImageId(win_id, IMAGE_COMMON_BG); 
#ifndef MMI_CALLCONTROL_MINI_SUPPORT        
        {
            int32 index = 0;
            CC_CreateHotButtonsWithoutForm(win_id);
            for (index = 0; index < CC_BUTTON_MAX; index ++)
            {
                GUIBUTTON_SetVisible(s_cc_win_button[index].ctrl_id, TRUE, FALSE);
            }
            CC_UpdateCCWinHotButton(CC_BUTTON_MAX, FALSE);
        }
#endif
#endif
        MMIDEFAULT_TurnOnBackLight();
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        CC_UpdateHotButtonState();
#endif
        if (MMICC_GetIsPlayCallEndHintFlag())
        {
             //启动定时器，开始通话挂断音提示，CC_PLAY_CALL_END_HINT_TIME指定提示音次数
            CC_INSTANCE.call_end_hint_timer = MMK_CreateTimerCallback(MMI_100MSECONDS, PlayCallEndHint, CC_PLAY_CALL_END_HINT_TIME, FALSE);
        }
        if (!MMK_IsOpenWin(MMICC_STATUS_WIN_ID) && !MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))
        {
            MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        }
        break;

    case MSG_FULL_PAINT:
        DisplayWinBg(win_id, FALSE);
        break;

    case MSG_TIMER:
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT 
        if( CC_INSTANCE.mmicc_disconnect_timer_for_display == *(uint8*)param)
        {
            int32 space = 0;
            GUILABEL_INIT_DATA_T label_init = {0};
            GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
            space = GetCallInfoCtrlSpace();
            //create state info
            label_init.both_rect.v_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT;
            label_init.both_rect.v_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT;
            label_init.both_rect.v_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP + client_rect.v_rect.top - space;/*lint !e737*/
            label_init.both_rect.v_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM + client_rect.v_rect.top - space;/*lint !e737*/

            label_init.both_rect.h_rect.left = CC_MAIN_INFO_STATE_LABEL_LEFT_H;
            label_init.both_rect.h_rect.right = CC_MAIN_INFO_STATE_LABEL_RIGHT_H;
            label_init.both_rect.h_rect.top = CC_MAIN_INFO_STATE_LABEL_TOP_H + client_rect.h_rect.top;/*lint !e737*/
            label_init.both_rect.h_rect.bottom =  CC_MAIN_INFO_STATE_LABEL_BOTTOM_H + client_rect.h_rect.top;/*lint !e737*/

            PdaCallCreateLabelByTextId(win_id,
                                                            MMICC_CONNECTED_STATE_LABEL_CTRL_ID, 
                                                            label_init, 
                                                            TXT_CC_END, 
#if defined MAINLCD_SIZE_240X320                                                            
                                                            MMI_CALL_INFO_FONT,
#else
                                                            MMI_DEFAULT_NORMAL_FONT, 
#endif
                                                            CC_TEXT_COLOR);

            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
        }
#endif        
#endif

        //received signal of timer when timer of disconnect is time out
        if(CC_INSTANCE.mmicc_disconnect_timer == *(uint8*)param)
        {
            //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleDisconnectWinMsg(), MSG_TIMER"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13216_112_2_18_2_11_9_385,(uint8*)"");
            //clear disconnect window
            //@samboo comment 20071001 use dummy mode
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
            {
                //获取自动重拨的号码
                MMICC_GetWantCallNumStr(tele_num, &tele_len);
                
                //SCI_TRACE_LOW:"ccapp.c: CC_HandleDisconnectWinMsg, tele_num = %s, tele_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13223_112_2_18_2_11_9_386,(uint8*)"sd", tele_num, tele_len);
                //clear disconnect window
                //判断当前是否设置自动重拨，黑名单是否已满（满不能重拨），该号码是否在黑名单中
                if (MMICC_IsWantCallExist() && (MMIAPISET_GetIsAutoRedail() || MMIAPISTK_GetSetupCallDuration()) )//cr131457
                {
                    //SCI_TRACE_LOW:"ccapp.c: s_call_repeat_info.disc_cause = %d, s_autodial_num = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13228_112_2_18_2_11_9_387,(uint8*)"dd", CC_INSTANCE.call_repeat_info.disc_cause, CC_INSTANCE.autodial_info.num);
                    //无效号码只能重拨一次
                    if (MMIAPICC_IsValideDest(CC_INSTANCE.call_repeat_info.disc_cause) && (CC_INSTANCE.autodial_info.num  < CC_NO_NUM_AUTODIAL_MAX_TIME))//断开条件，以后整理
                    {
                        CC_OpenAutoDialWindow();
                    }
                    //用户忙的情况能够重拨十次
                    else if(MMIAPICC_IsUserBusy(CC_INSTANCE.call_repeat_info.disc_cause) && CC_INSTANCE.autodial_info.num < CC_INSTANCE.autodial_info.num_max)
                    {
                        CC_OpenAutoDialWindow();
                    }
                    else if ((MMIAPICC_IsValideDest(CC_INSTANCE.call_repeat_info.disc_cause) && (CC_INSTANCE.autodial_info.num >= CC_NO_NUM_AUTODIAL_MAX_TIME)) ||
                        (MMIAPICC_IsUserBusy(CC_INSTANCE.call_repeat_info.disc_cause) && CC_INSTANCE.autodial_info.num >= CC_INSTANCE.autodial_info.num_max))
                    {
                        //添加到自动重拨黑名单中
                        CC_AddItemtoBlackList(tele_num, tele_len);
                    }
                }
            }   
            
            //显示未接来电	
            //@samboo cr102289 20071206 根本原因是mmk_closewin里面MMK_GetWinPtrBaseWinId获得的win_ptr不是disconnect_win的，造成assert
            MMK_CloseWin( win_id );
#ifdef DPHONE_SUPPORT
            if(MMIDEFAULT_GetHookStates()||MMIDEFAULT_GetHandFreeStates())
            {
                MMIAPIIDLE_OpenDialWin(msg_id, FALSE);
            }
#endif
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
#ifndef MMI_PDA_SUPPORT
    case MSG_APP_POWER: 
#endif
    case MSG_APP_CANCEL://user press CANCEL 
        //still exited MO call,should not delete info
        if (!MMICC_IsExistOutgoingCall())
        {
            MMICC_ClearWantCallInfo();
        }     
        
        ClearAutoRedialInfo();
        //clear disconnect window
        MMK_CloseWin( MMICC_DISCONNECT_WIN_ID );
        break;
        
    case MSG_GET_FOCUS:
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        if( 0 == CC_INSTANCE.mmicc_disconnect_timer_for_display )
        {
            CC_INSTANCE.mmicc_disconnect_timer_for_display = MMK_CreateWinTimer(MMICC_DISCONNECT_WIN_ID, CC_DISCONNECT_TIMER_OUT_FOR_DISPLAY, FALSE);
        }
        else
        {
            MMK_StartWinTimer(
                MMICC_DISCONNECT_WIN_ID,
                CC_INSTANCE.mmicc_disconnect_timer_for_display,
                CC_DISCONNECT_TIMER_OUT_FOR_DISPLAY, FALSE
                );
        }
#endif        
#endif

        if( 0 == CC_INSTANCE.mmicc_disconnect_timer )
        {
            CC_INSTANCE.mmicc_disconnect_timer = MMK_CreateWinTimer(MMICC_DISCONNECT_WIN_ID, CC_DISCONNECT_TIMER_OUT, FALSE);
        }
        else
        {
            MMK_StartWinTimer(
                MMICC_DISCONNECT_WIN_ID,
                CC_INSTANCE.mmicc_disconnect_timer,
                CC_DISCONNECT_TIMER_OUT, FALSE
                );
        }
        break;
 
    case MSG_LOSE_FOCUS:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            ClearAutoRedialInfo();
            MMK_CloseWin(win_id);
        }
#endif
        break;
  
    case MSG_APP_RED:
        ClearAutoRedialInfo();
        MMK_CloseWin(win_id);
#ifdef DPHONE_SUPPORT
        if(MMIDEFAULT_GetHookStates()||MMIDEFAULT_GetHandFreeStates())
        {
            MMIAPIIDLE_OpenDialWin(msg_id, FALSE);
        }
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIAPIUSSD_WinGrabFocus();

#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_CALLCONTROL_MINI_SUPPORT
        MMK_StopTimer( CC_INSTANCE.mmicc_disconnect_timer_for_display );
        CC_INSTANCE.mmicc_disconnect_timer_for_display = 0;
#endif        
#endif
        MMK_StopTimer( CC_INSTANCE.mmicc_disconnect_timer );
        CC_INSTANCE.mmicc_disconnect_timer = 0;
        if (MMICC_GetIsPlayCallEndHintFlag())
        {
            MMICC_SetIsPlayCallEndHintFlag(FALSE);
            
            if (0 != CC_INSTANCE.call_end_hint_timer)
            {
                //可能这时挂断音正在播放，需要停止
                MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);

                MMK_StopTimer(CC_INSTANCE.call_end_hint_timer);
                CC_INSTANCE.call_end_hint_timer = 0;
            }
        }
        
        if (MMICC_IsCurrentCallIDValid()
#ifdef BT_DIALER_SUPPORT
            && !MMICC_GetBtCallFlag()
#endif
#ifdef ASP_SUPPORT            
            && !MMIAPIASP_IsMachineWorking()
#endif       
        )
        {
            if (!MMICC_IsExistOperatedCall())
            {
                MMICC_UpdateCurrentCallStatus(TRUE);    //call status
            }
            else
            {
                MMICC_UpdateCurrentCallProcessing();
            }
        }
        MMICC_HandleOperAfterCallEnd();
        break;
        
    case MSG_APP_WEB:
        if ( 
            (!MMICC_IsExistActiveCall())&& 
            (!MMICC_IsExistHoldCall()) &&
            CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
        {
            MMIAPICC_StopRingOrVibrate();
            ClearAutoRedialInfo();
            
            //hang up call
#ifdef BT_DIALER_SUPPORT
            if(MMICC_GetBtCallFlag())
            {
                bt_status = MMIAPIBT_HfuHangup();
                if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
               {
                    SCI_TRACE_LOW("ccapp.c:CC_HandleDisconnectWinMsg() MMIAPIBT_HfuHangup fail");
               }
            }
            else
#endif
{
            if(CC_RESULT_ERROR == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_USER_BUSY ))
            {
                //显示失败
            }
}
            MMK_CloseWin(win_id);
        }
        break;
        
        //	case MSG_APP_WEB:
    case MSG_APP_OK:
        //fix for cr 74433
        break;
        
    default:
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleDisconnectWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13396_112_2_18_2_11_9_388,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : 通话结束嘟嘟音，根据传入的param，自减1做嘟嘟音计数
//	Global resource dependence : 
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void PlayCallEndHint(uint8 timer_id, uint32 param)
{
    uint32 hint_volume = MMISET_VOL_FOUR;

    CHECK_CC_APPLET_NOT_NULL
    
    //SCI_TRACE_LOW:"ccapp:PlayCallEndHint"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13411_112_2_18_2_11_9_389,(uint8*)"");
    
    if (0 != CC_INSTANCE.call_end_hint_timer)
    {                
        MMK_StopTimer(CC_INSTANCE.call_end_hint_timer);
        CC_INSTANCE.call_end_hint_timer = 0;
        
        if (param > 0)
        { 
            //可能这时tone音正在播放，需要停止
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
            MMISRVAUD_SetVolumeNoHandle( hint_volume);
            MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, MMI_NT_RING_DIS_ID, MMI_200MSECONDS, MMISRVAUD_TONE_FREQ_DEFAULT);
            param --;
            CC_INSTANCE.call_end_hint_timer = MMK_CreateTimerCallback(MMI_200MSECONDS, PlayCallEndHint, param, FALSE);
        }
        else if (0 == param)//挂断音播放结束之后，需要把音量值设回原来的值
        {
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
            MMISRVAUD_SetVolumeNoHandle( MMIAPICC_GetCallVolume());
        }
    }
}
/*****************************************************************************/
// 	Description : the process message function of the Missed call application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:modify by louis.wei
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMissedcallWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (!MMIAPIKL_IsPhoneLocked())
#endif
        {
            //关闭通话记录列表和未接电话之间的所有窗口
            //MMK_CloseMiddleWin(MMICL_LOG_LIST_WIN_ID,win_id);
            MMK_CloseParentWin(MMICL_LOG_LIST_WIN_ID);
            
            //close the window of call log list menu and detail win
            MMK_CloseWin(MMICL_LOG_LIST_MENU_WIN_ID);
            MMK_CloseWin(MMICL_REFUSED_LOG_LIST_MENU_WIN_ID);
            MMK_CloseWin(MMICL_LOG_LIST_DETAIIL_WIN_ID);
            
            MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
            MMK_CloseWin(win_id);
        }
        break;
        
    //case MSG_GET_FOCUS:
        //MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_FULL_PAINT, PNULL);
        //MMITHEME_SetUpdateDelayCount(1);		
        //break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIPUB_SetWinSoftkey(MMICC_MISSCALL_WIN_ID, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
#endif
        {
            MMIPUB_SetWinSoftkey(MMICC_MISSCALL_WIN_ID, STXT_DETAIL, STXT_EXIT, FALSE);
        }
        
        //MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_LOSE_FOCUS, PNULL);
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;    

    case MSG_APP_CANCEL:
    case MSG_APP_RED:        
        MMK_CloseWin( win_id );
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:
        recode = MMI_RESULT_FALSE;
        break;
#endif        
        
    case MSG_CLOSE_WINDOW:
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏，清除未接来电显示
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_MISSED_CALL,PNULL);
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        //SCI_TRACE_LOW:"ccapp.c: MMICC_HandleMissedcallWinMsg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13506_112_2_18_2_11_9_390,(uint8*)"d", msg_id);
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : play new call ring
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  PlayNewCallRing(MN_DUAL_SYS_E dual_sys)
{
    //play ring or vibrate
    if (MMICC_IsExistActiveCall()||MMICC_IsExistHoldCall() || MMICC_IsExistOperatedCall())
    {
        MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_CALL_IN_CALL,PNULL);
        if (0 == CC_INSTANCE.waiting_call_ring_timer)
        {
            CC_INSTANCE.waiting_call_ring_timer = MMK_CreateWinTimer(MMICC_ANIMATION_WIN_ID, CC_WAITING_CALL_PERIOD, FALSE);
        }
    }
    else
    {
        MMIAPICC_StartRingOrVibrate(FALSE);
        MMICC_SetMtPresetCodecRoute(0);
    }
    
    return TRUE;
}
/*****************************************************************************/
// 	Description : is conditions of anykey-answer op satisfied
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsAnyKeyAnswerThisCall(MMI_MESSAGE_ID_E msg_id)
{    
#ifndef MMI_GUI_STYLE_TYPICAL
	//pda should not support this function
	return FALSE;
#else
    MMISET_ANYKEY_REJECT_SET_T	reject_set = {0};

    if (((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            || (CC_WAITING_STATE == MMICC_GetCurrentCallStatus()))
        && ((msg_id > MSG_KEYDOWN) && (msg_id <= MSG_KEYDOWN_MAX_NUM))
        && (msg_id != MSG_APP_CANCEL)
        && (msg_id != MSG_APP_RED)
        && !((msg_id == MSG_KEYDOWN_HEADSET_DETECT)||(msg_id == MSG_KEYDOWN_SDCARD_DETECT))
        && !((msg_id == MSG_APP_UP)||(msg_id == MSG_APP_DOWN)||(msg_id == MSG_APP_LEFT)||(msg_id == MSG_APP_RIGHT))
        && (msg_id != MSG_APP_WEB)
        )
        {
            reject_set = MMIAPISET_GetAnykeyRejectMode();
            if (reject_set.is_anykey_answer)
            {
                return TRUE;
            }
        }

    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : 调节音量，并打开音量调节
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_AdjustCallVolume(BOOLEAN is_increase)
{
    BOOLEAN is_open_win = FALSE;
    
    if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) ||
        (MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) && MMICC_IsVoiceCodecEnable()))
    {
        MMI_MESSAGE_ID_E msg_id = is_increase ? MSG_APP_UP : MSG_APP_DOWN;

        MMIAPICC_OpenAdjustVolumeWin(MMK_GetFocusWinId(), msg_id);
        is_open_win = TRUE;
    }
    else
    {
        return TRUE;
    }

    return is_open_win;
}

/*****************************************************************************/
//  Description : Set browser app active (means:open the browser)
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBrowserActive(void)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};
    
    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.is_browser_type_customized = TRUE;
#ifdef BROWSER_SUPPORT_DORADO
    entry_param.browser_type = MMIBROWSER_TYPE_DORADO;
#else
	entry_param.browser_type = MMIBROWSER_TYPE_OPERA_MINI;
#endif
    
    MMIAPIBROWSER_Entry(&entry_param);
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : when missed call number change,send messege to keylock window if is phone locked.
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SendMissedCallNumberToKeylock(void)
{
#ifdef MMI_KEY_LOCK_SUPPORT
    if (MMIAPIKL_IsPhoneLocked()&& MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
    {
        MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_MISSED_CALL,PNULL,0);
    }
#endif
}

/*****************************************************************************/
// 	Description : whether the current mode is hand free
//	Global resource dependence : 
//  Author:gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsHandFree(void)
{
    BOOLEAN is_handfree = FALSE;
    if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
    {
       is_handfree = (MMISRVAUD_ROUTE_SPEAKER == MMICC_GetMtPresetCodecRoute());
    }
    else
    {
        is_handfree = (MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev());
    }
    return is_handfree;
}

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
// 	Description : 关机闹钟后，是否开机询问窗口处理函数
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAnswerSMSOrNotWindow( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MMIAPISMS_OpenFreqWordListForCc(MMIAPICC_GetCallDualSys(), (uint8)s_sms_name_len, s_sms_name_arr);
        MMK_CloseWin(MMICC_DISCONNECT_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(MMICC_DISCONNECT_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : CC_SendSMSAfterRejectMTCall 
//	Global resource dependence : 
//  Author:bruce.chi
//	Note: 
/*****************************************************************************/
LOCAL void CC_SendSMSAfterRejectMTCall(uint8 *name_ptr, uint16 name_len)
{
    MMI_WIN_ID_T win_id = MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID;
    
    if(PNULL == name_ptr)
    {
        //SCI_TRACE_LOW:"CC_SendSMSAfterRejectMTCall name_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13676_112_2_18_2_11_10_391,(uint8*)"");
        return;
    }
           
    s_sms_name_len = MIN(name_len, CC_MAX_TELE_NUM_LEN);
    SCI_MEMCPY(s_sms_name_arr, name_ptr, s_sms_name_len);

    MMIPUB_OpenQueryWinByTextId(TXT_REPLY,IMAGE_PUBWIN_QUERY,&win_id,HandleAnswerSMSOrNotWindow);
}

/*****************************************************************************/
// 	Description : MMIAPICC_SendSMSAfterRejectMTCall 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: only export to VT
/*****************************************************************************/
PUBLIC void MMIAPICC_SendSMSAfterRejectMTCall(uint8 *name_ptr, uint16 name_len)
{
    MMISET_ANYKEY_REJECT_SET_T  reject_mode = MMIAPISET_GetAnykeyRejectMode();
    
#ifdef CMCC_UI_STYLE
    if (!s_is_cancel_reject)
    {
        return;
    }
#endif

    if(!MMIAPIDM_GetHandsetLock())
    {
        if (reject_mode.is_sms_reply_open)
        {
            //sms拒接窗口存在，则关闭
            if (MMK_IsOpenWin(MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID))
            {
                MMK_WinGrabFocus(MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID);
            }
            
            CC_SendSMSAfterRejectMTCall(name_ptr, name_len);
        }
    }
}
#endif

/*****************************************************************************/
// 	Description : emergency call win msg
//	Global resource dependence : 
//  Author:bownzhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleEmergencyCallWinMsg(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id, 
    DPARAM param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMICC_EMERGENCY_PHONENUM_CTRL_ID;
    MMI_STRING_T str_info = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint32 multi_sys_num = 0;
    uint16 multi_sys = MN_DUAL_SYS_1;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetFont(ctrl_id, SONG_FONT_29);

        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,CC_EMERGENCY_CALL_NUM_MAX_LEN + 1,TXT_NULL,TXT_SOS,EmergencyCallWinSoftkey);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_EDITBOX_UPDATE_STRNUM:
        GUIEDIT_GetString(ctrl_id, &str_info);
        
        if (0 == str_info.wstr_len)
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            BOOLEAN is_input_emergency_call = FALSE;
            uint16 emergency_call_len = 0;
            
            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMI_DUAL_SYS_MAX);
            
            if (!is_input_emergency_call)
            {
                MMK_CloseWin(win_id);
            }
#if 0//def MMIPUBWIN_PDASTYLE_SUPPORT
            else if (str_info.wstr_len == emergency_call_len)
            {    
                MMIPUB_SetWinSoftkeyGray(win_id, FALSE, TRUE, FALSE, TRUE);
            }
            else
            {
                MMIPUB_SetWinSoftkeyGray(win_id, TRUE, TRUE, FALSE, TRUE);
            }
#endif
        }
        break;
    case MSG_CTL_MIDSK:    
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:    
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint8 temp_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
            BOOLEAN is_input_emergency_call = FALSE;
            uint16 emergency_call_len = 0;
            
            GUIEDIT_GetString(ctrl_id, &str_info);
            
            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMI_DUAL_SYS_MAX);
            
            if (is_input_emergency_call && (emergency_call_len == str_info.wstr_len))
            {
                MMI_WSTRNTOSTR((uint8 *)temp_str, 
                    str_info.wstr_len,
                    str_info.wstr_ptr,
                    str_info.wstr_len,
                    str_info.wstr_len);
                multi_sys_num = MMIAPISET_GetMultiSysSettingNum(&multi_sys, 1);

                if( 0 == multi_sys_num)
                {
                    dual_sys = MN_DUAL_SYS_1;
                }
                else
                {
                    dual_sys = (MN_DUAL_SYS_E)multi_sys;
                }
                
                MMIAPICC_MakeCall(
                    dual_sys,
                    temp_str,
                    (uint8)(str_info.wstr_len),
                    PNULL,
                    PNULL,
                    MMIAPICC_GetCallSimType(dual_sys),
                    CC_CALL_NORMAL_CALL,
                    PNULL
                    );
                MMK_CloseWin(win_id);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    //@fen.xie MS00177896:锁键盘后，按住POWER，一秒内再按紧急呼叫号码开头的数字键,会关机
    case MSG_KEYLONG_RED:
        break;        
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        //SCI_TRACE_LOW:"mmiccapp: HandleEmergencyCallWinMsg, msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13824_112_2_18_2_11_10_392,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : emergency call window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN EmergencyCallWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_input_emergency_call = FALSE;
    uint16          emergency_call_len = 0;
    MMI_STRING_T    str_info = {0};

    //set string info
    str_info.wstr_ptr = str_ptr;
    str_info.wstr_len = str_len;

    is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMI_DUAL_SYS_MAX);
    if ((is_input_emergency_call) &&
        (str_len == emergency_call_len))
    {
        *text_id_ptr = TXT_SOS;
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StartDtmfTimerId(void)
{
    MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
    CC_INSTANCE.dtmf_timer_id = MMK_CreateWinTimer(WATCHCC_CONNECTED_WIN_ID, CC_DTMF_WAITING_TIME, FALSE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StopDtmfTimerId(void)
{
    CHECK_CC_APPLET_NOT_NULL
    if (MMI_NONE_TIMER_ID != CC_INSTANCE.dtmf_timer_id)
    {
        MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
        CC_INSTANCE.dtmf_timer_id = MMI_NONE_TIMER_ID;
    }    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StartDtmfSendingTimer(void)
{
    CHECK_CC_APPLET_NOT_NULL

    MMK_StopTimer(CC_INSTANCE.dtmf_sending_timer);

    CC_INSTANCE.dtmf_sending_timer = MMK_CreateTimerCallback(CC_DTMF_SENDING_TIMER_OUT, CC_HandleDtmfSendingTimeout,(uint32)MN_DUAL_SYS_MAX,FALSE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StopDtmfSendingTimer(void)
{
    CHECK_CC_APPLET_NOT_NULL

    if (MMI_NONE_TIMER_ID != CC_INSTANCE.dtmf_sending_timer)
    {
        MMK_StopTimer(CC_INSTANCE.dtmf_sending_timer);

        CC_INSTANCE.dtmf_sending_timer = MMI_NONE_TIMER_ID;
    }    
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
 LOCAL void  CC_HandleDtmfSendingTimeout( uint8 timer_id, uint32 param)
{
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)param;
    MMIAPICC_StopDtmf(dual_sys);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void  MMIICC_StarCallTimingTimer(void)
{
    if( MMI_NONE_TIMER_ID ==  CC_INSTANCE.call_time_count_timer )
    {
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_MMICC_STARTTIMER , PNULL );
    }
}

/*****************************************************************************/
// 	Description : write disconnect call cause to blacklist
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_SetRepeatInfoCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    CHECK_CC_APPLET_NOT_NULL
    CC_INSTANCE.call_repeat_info.disc_cause = cause;
    //SCI_TRACE_LOW:"ccapp.c: MMICC_SetRepeatInfoCause, s_call_repeat_info.disc_cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_13906_112_2_18_2_11_10_393,(uint8*)"d", CC_INSTANCE.call_repeat_info.disc_cause);
}

/*****************************************************************************/
// Description : set auto dial times max
//Global resource dependence : 
//  Author: samboo.shen
//Note:
/*****************************************************************************/
PUBLIC void MMICC_SetAutoDialTimesMax(uint8 times_max)
{
    CHECK_CC_APPLET_NOT_NULL   
    CC_INSTANCE.autodial_info.num_max = times_max;
}

/*****************************************************************************/
// 	Description : the operation after call absolute end, including the disconnected win closed
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleOperAfterCallEnd(void)
{
    BOOLEAN is_last_call = (0 == MMICC_GetCallNum());
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏，关闭小屏关于call的信息
    MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_CALL,PNULL);
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
    if (MMICC_IsExistActiveCall())//this is a call splited from MPTY
    {
        MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
        if (MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
        {
            MMICC_UpdateCurrentCallStatus(FALSE);
            MMK_PostMsg(MMICC_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }
        else
        {
            MMK_ReturnAppointedWin( MMICC_STATUS_WIN_ID );
        }
    }
#endif

    if(is_last_call)
    {
        //没有通话的情况下需要取消状态栏上正在通话的标志
        MMI_WIN_ID_T win_id = MMICC_QUERY_SWITCH_TO_HANDFREE_WIN_ID;
        MMIPUB_CloseQuerytWin(&win_id);        
        MAIN_SetCallingState(FALSE);
    }
    if( is_last_call && !MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID))
    {
#ifdef PDA_UI_DROPDOWN_WIN
#ifdef MMI_CALL_MINIMIZE_SUPPORT
    MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_RUNNING_CALLING);
#endif
#endif

#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{

#ifdef MV_SUPPORT//全部通话都结束，处理魔音相关操作
        if (IsMagicVoiceWorking())
        {
            CloseMagicVoice();//关闭魔音
        }

        if (MMIAPISET_GetIsMagicVoice())
        {
            MMK_CloseWin(MMICC_MAGIC_VOICE_ITEM_SELECT_WIN_ID);
            MMK_CloseWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID);
            /*魔音开启情况下,当挂断最后一通电话时,需要重置魔音的参数值;
                以保证通话中对魔音的设置操作只是临时的*/
            ResetCCMagicVoiceValue();

            //@fen.xie MS00185659 当所有通话都结束,不再需要使用魔音时，此时允许重新开启冲突的3D特效
            //MMI_Enable3DMMI(TRUE);
#ifdef UI_P3D_SUPPORT
            MMI_Enable3DEffect(MMI_3D_EFFECT_MAGIC_VOICE);
#endif                 
        }
#endif
}

//@fen.xie MS00173072 begin: 最后一通电话挂断，处理合盖键,回idle锁键盘
#ifdef LCD_SLIDE_SUPPORT
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIKL_GetIsLaterHandleSlideKey())
        {
            MMIKL_SetIsLaterHandleSlideKey(FALSE);
            MMK_ReturnIdleWin();
            if (MMISET_KEYLOCK_SWITCH_ON_TYPE == MMIAPISET_GetKeylockSwitchType())     //键盘锁开启
            {
                MMIAPIKL_LockPhone();
            }
        }
#endif
#endif /*LCD_SLIDE_SUPPORT*/            
//@fen.xie MS00173072 end   

        // @baokun fix 93989 begin: change function order

        //最后一通电话挂断, 弹出被延迟的闹钟
        if (!MMIAPIALM_CheckAndStartDelayedAlarm())
        {//@zhaohui,counted time
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
        }
        // @baokun fix 93989 end
        
        // add by shengjie.liu for cr99360
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
{
        MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(MMIAPICC_GetCallDualSys()));
}
        MMIAPISMS_CheckTimerMSG();

        if(PNULL != MMICC_GetAppletInstance() )
        {
            if(CC_INSTANCE.is_original_phone_lock)
            {
                CC_INSTANCE.is_original_phone_lock = FALSE;     
            }
               
            //stop cc applet
            MMICC_DestroyAudioHandle();
            MMK_PostMsg( MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
        }
    }	
    //显示未接来电	
    MMIAPICC_ShowMCHint();

    MMIAPIDM_HandDmCallClose();
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : tao.xue
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetIsPlayCallEndHintFlag(void)
{
    if(PNULL != MMICC_GetAppletInstance())
    {
        return CC_INSTANCE.is_play_call_end_hint;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : tao.xue
//	Note:   
/*****************************************************************************/
PUBLIC void MMICC_SetIsPlayCallEndHintFlag(BOOLEAN is_enable)
{
    if(PNULL != MMICC_GetAppletInstance())
    {
        CC_INSTANCE.is_play_call_end_hint = is_enable;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:   
/*****************************************************************************/
PUBLIC void MMICC_ClearAutoRedialInfo(void)
{
    CHECK_CC_APPLET_NOT_NULL
    ClearAutoRedialInfo();
}
/*****************************************************************************/
// 	Description : 打开/关闭免提模式
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_EnableHandfreeMode(BOOLEAN is_turnon)
{
    return  EnableHandfreeMode( is_turnon);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:   
/*****************************************************************************/
PUBLIC void MMICC_OpenDisconnectWin(
                                   uint8							call_index,
                                   MN_CALL_DISCONNECT_CALL_CAUSE_E	cause,
                                   uint32 call_time
                                   )
{
    CHECK_CC_APPLET_NOT_NULL

    SCI_TRACE_LOW("[MMICC] MMICC_OpenDisconnectWin call_index = %d, cause = %d, call_time = %d", call_index, cause, call_time);
#ifdef MMI_CALL_MINIMIZE_SUPPORT        
    if(CC_INSTANCE.is_minimize)
    {
        return;
    }
#endif    
#if (!defined MMI_GUI_STYLE_TYPICAL)// && (!defined MMI_CALLCONTROL_MINI_SUPPORT)
    PdaDisplayDisconnectWin(call_index, cause, call_time);
#else
    CC_DisplayDisconnectWin(call_index, cause, call_time);
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenSelectSimWin(ADD_DATA add_data_ptr)
{
    if (MMK_IsOpenWin(MMICC_COM_SELECT_SIM_WIN_ID))
    {
        MMK_WinGrabFocus(MMICC_COM_SELECT_SIM_WIN_ID);
    } 
    else
    {
        MMK_CreatePubListWin((uint32*)MMICC_SELECT_SIM_WIN_TAB, (ADD_DATA) add_data_ptr);  
    }
}

/*****************************************************************************/
//  Description : start vibrate for connect call prompt
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_VibrateForConnectPrompt(void)
{
    if (MMIAPISET_GetIsConnectPrompt())
    {
        MMISRVAUD_StartVibrate(MMISRVAUD_PRI_HIGH, CC_CONNECT_PROMPT_VIBRATE_DUARATION, 0, 1, PNULL);        
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle headset on or off in call
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleHeadsetInCall(MMI_MESSAGE_ID_E msg_id)
{
    MMICC_AUDIO_OPER_E oper = MMICC_AUDIO_OPER_ENABLE;
    BOOLEAN is_headset_on = TRUE;
    if (MSG_KEYUP_HEADSET_DETECT == msg_id)
    {
        oper = MMICC_AUDIO_OPER_DISABLE;
        is_headset_on = FALSE;
    }
    MMIAPICC_SwitchAudioDevice(oper, MMICC_AUDIO_DEV_EP);

    //设置呼叫窗口和通话窗口右软键
    if (MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
    {
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL);	
    }
    else if (
#ifndef MMI_GUI_STYLE_TYPICAL
    MMK_IsFocusWin(MMICC_STATUS_WIN_ID) 
#else
    MMK_IsFocusWin(MMICC_ANIMATION_WIN_ID) 
#endif
    && MMICC_IsExistOutgoingCall()
    )
    {
        MMK_SendMsg(MMICC_ANIMATION_WIN_ID, MSG_FULL_PAINT, PNULL);	
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL);	
#endif
    }
    else
    {
        //do nothing
    }
}

/*****************************************************************************/
// 	Description : handle headset on or off in call
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_HandleHeadsetInCall(MMI_MESSAGE_ID_E msg_id)
{
    HandleHeadsetInCall(msg_id);
}

/*****************************************************************************/
// 	Description : 创建状态栏
//	Global resource dependence : 
//  Author:xiaoqinglu
//	Note:
/*****************************************************************************/
LOCAL void CreateStatusBarCtrl(
                               MMI_WIN_ID_T win_id,
                               MMI_IMAGE_ID_T bg_image_id
                               )
{
#ifdef DPHONE_SUPPORT /* lint_lai */
#else
    GUI_BG_T  bg_info = {0};

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = bg_image_id;//IMAGE_COMMON_BG;

    GUIWIN_CreateStbDyn(win_id);
    
    if (IMAGE_NULL != bg_image_id)
    {
        GUIWIN_SetStbBg(win_id, bg_info);
    }
    /*else
    {
        GUISTATUSBAR_SetBgIsDesplay(win_id, FALSE);
    }*/

    return;
#endif 
}

/*****************************************************************************/
//  Description :to answer a incoming call automatically, assoicated with eng
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void CC_AutoAnswerIncomingCall(void)
{
    if(MMIAPIENG_GetAutoAnswerCallSet()
        &&(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()))
    {
        //SCI_TRACE_LOW:"ccapp:AutoAnswerIncomingCall"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14206_112_2_18_2_11_11_394,(uint8*)"");
        MMK_PostMsg(MMICC_ANIMATION_WIN_ID, MSG_APP_GREEN, PNULL, 0);
    }
}
#if 0
/*****************************************************************************/
// 	Description : UPDATE CALL STATUS
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_UpdateCallStatusDisplay(MMICC_UPDATESTATUS_TYPE_E type)
{
    BOOLEAN result = TRUE;
    MMI_WIN_ID_T	 alert_win_id = MMICC_HOLDCNF_PROMPT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;    

    if(type >= MMICC_UPDATESTATUS_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"cc app MMICC_UpdateCallStatusDisplay error param: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14223_112_2_18_2_11_11_395,(uint8*)"d", type);
        return FALSE;
    }

    switch(type)
    {
    case MMICC_MO_START_IND:
        CC_OpenMoCallWin();
#ifndef MMI_GUI_STYLE_TYPICAL
            MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
#endif            
        break;

    case MMICC_MT_START_IND:
        MMIDEFAULT_TurnOnBackLight();
        if(PNULL != MMICC_GetAppletInstance())
        {
            CC_INSTANCE.is_need_light_on_cc_win = FALSE;
        }
#ifdef MMI_RECORD_SUPPORT
        if(MMIAPIRECORD_IsRecording())
        {
            if (MMICC_GetCallNum()==1)  //if it is the first call
            {
                MMIAPIRECORD_StopRecordOrPlay(); //停止进入前的录音
            }    
        }
#endif
#ifdef FM_SUPPORT      
        if (MMICC_GetCallNum()==1)  //if it is the first call
        {
            MMIAPIFM_StopRecord();//NEWMS00232750
            MMIAPIFM_StopAutoSearch();
        }    
#endif
        CC_OpenMtCallWin();
        MMK_CloseWin(MMICC_MENU_WIN_ID );
        MMK_CloseWin( MMICC_CALLLIST_WIN_ID );            
        break;

    case MMICC_MO_CONNECTED_IND:
#ifndef MMI_GUI_STYLE_TYPICAL

#ifndef MMI_CALLCONTROL_MINI_SUPPORT
             MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_OPEN_WINDOW , PNULL );
#else
             MMK_PostMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_OPEN_WINDOW, PNULL,0);

#endif

#endif
            //修改CALL LOG resolve in the future
            MMICC_UpdateCurrentCallStatus(FALSE);    //call status
            MMK_WinGrabFocus(MMICC_STATUS_WIN_ID);
            MMIICC_StarCallTimingTimer();
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON , PNULL);
#endif
        MMIAPIUSSD_WinGrabFocus();//电话接通后需要回到USSR界面
        break;

    case MMICC_MT_CONNECTED_IND:
        MMICC_UpdateCurrentCallStatus(TRUE);    //call status
        MMK_WinGrabFocus(MMICC_STATUS_WIN_ID);
#ifndef MMI_GUI_STYLE_TYPICAL
        if(CC_DISPLAY_MT_LOCK == CC_INSTANCE.cc_display_type)
        {
            CC_UpdateCcDisplayType(CC_DISPLAY_MT_LOCK);
        }
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON , PNULL);
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
        if (MMIAPISET_GetAutoRecordSetting()
        #ifdef ASP_SUPPORT            
        &&(!MMIAPIASP_IsMachineWorking())
        #endif
        )
        {
#if 0        
#ifdef BLUETOOTH_SUPPORT
            BT_ADDRESS		address={0};
            if(!MMIAPIBT_GetActivatedHS(&address))
#endif
#endif
#ifdef MMI_RECORD_SUPPORT
#ifdef BT_DIALER_SUPPORT
                if(FALSE == MMICC_GetBtCallFlag())
#endif
                {            
                    HandleRecordOpt(MMICC_STATUS_WIN_ID,TRUE);
                }
#else
                {
                }
#endif
        }
#endif
        //start timer
        MMIICC_StarCallTimingTimer();
        MMK_CloseWin(MMICC_MENU_WIN_ID );
        MMK_CloseWin( MMICC_CALLLIST_WIN_ID);
        MMIAPIUSSD_WinGrabFocus();//电话接通后需要回到USSR界面    
        break;

    case MMICC_DISCONNECTED_IND:
        //sms reject disconnect窗口和sms询问窗口存在于更高优先级的窗口的下面，则关闭
        if (MMK_IsOpenWin(MMICC_DISCONNECT_WIN_ID)&&MMK_IsOpenWin(MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID)
#ifdef BT_DIALER_SUPPORT
           && !MMICC_GetBtCallFlag()
#endif  
		)
        {
            MMK_WinGrabFocus(MMICC_DISCONNECT_WIN_ID);
        }
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON , PNULL);
        if(MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
        {
            MMK_PostMsg(MMICC_STATUS_WIN_ID, MSG_FULL_PAINT, PNULL,0);
        }
#endif
        MMK_CloseWin(MMICC_MENU_WIN_ID  );
        MMK_CloseWin( MMICC_CALLLIST_WIN_ID );
        break;

    case MMICC_HOLD_SUC_IND:
    case MMICC_RETRIEVE_SUC_IND:
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;
        
    case MMICC_RETRIEVE_FAIL_IND:
        //利用msgbox提示出错，并修改CC window的处理
        MMICC_OpenAlertFailWin(TXT_NETWORK_FAIL_INFO);
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;

    case MMICC_HOLD_FAIL_IND:
        //use the msgbox to prompt the operated is fail
        {
            MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,TXT_NETWORK_FAIL_INFO,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,HandleHoldCnfPromptWindow,0);
        }
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;

    case MMICC_CALL_AFTER_HOLD_FAIL_IND:
        //use the msgbox to prompt the operated is fail
        MMICC_OpenAlertFailWin(TXT_NETWORK_FAIL_INFO);
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;
        
    case MMICC_BUILD_MPTY_SUC_IND:
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON , PNULL);
#else
        MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,TXT_CC_MPTY_ACTIVE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,HandleHoldCnfPromptWindow,0);
#endif            
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;

    case MMICC_BUILD_MPTY_FAIL_IND:
        //use the MSGBOX to prompt the operation is fail
        MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,TXT_NETWORK_FAIL_INFO,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,HandleHoldCnfPromptWindow,0);
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;

    case MMICC_SPLIT_MPTY_SUC_IND:
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON , PNULL);
#else
        {
            MMI_STRING_T private_prompt_str = {0};
            MMI_STRING_T name_str = {0};

            MMI_GetLabelTextByLang(TXT_CC_PRIVATE_PROMPT, &private_prompt_str);
            MMICC_GetCallNameStrByIndex(&name_str, MMICC_GetCurrentCallIndex());
            MMIPUB_OpenAlertWinByTextPtrEx(CC_APPLET_HANDLE,PNULL, &private_prompt_str, &name_str, IMAGE_PUBWIN_SUCCESS, &alert_win_id, & win_priority, MMIPUB_SOFTKEY_ONE,PNULL,0);
        }
#endif
       
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;         

    case MMICC_SPLIT_MPTY_FAIL_IND:
        //use the MSGBOX to prompt the operation is fail
        MMICC_OpenAlertFailWin(TXT_NETWORK_FAIL_INFO);
        MMICC_UpdateCurrentCallStatus(TRUE); 
        break;

    default:
        //do nothing
        break;
    }

    return result;
}
#endif
/*****************************************************************************/
// 	Description : 提示窗口消息处理
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHoldCnfPromptWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMICC_UpdateCurrentCallStatus(TRUE);    //call status 
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;
}

#ifdef ASP_SUPPORT
/*****************************************************************************/
// 	Description : 此函数专供在答录界面用户接听时调用....
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_GotoRealCallState(uint32 asp_connected_time)
{
    BT_ADDRESS		address = {0};
    MMIAPIBT_CallConnect(&address);
    // 答录机已经工作的时间接过来....
    MMICC_SetCallTimeCount(asp_connected_time);
#ifdef MV_SUPPORT
    //只有第一通电话时才主动打开魔音选择窗口
    if (IsCanUseMagicVoice(TRUE) && !MMK_IsFocusWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID) && (1 == MMICC_GetCallNum()))
    {
        MMICC_OpenMVBeforeCallConnected(NULL);
    }
    else
    {
        UpdateCurrentASPCall();
    }
#else
    UpdateCurrentASPCall();
#endif
#if defined(MMI_RECORD_SUPPORT) && defined(MMI_AUTO_RECORD_SUPPORT)
    if (MMIAPISET_GetAutoRecordSetting())
    {
        HandleRecordOpt(MMICC_STATUS_WIN_ID,TRUE);
    }
#endif    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
LOCAL void UpdateCurrentASPCall(void)
{
    MMICC_UpdateCurrentCallStatus(TRUE);
    MMK_WinGrabFocus(MMICC_STATUS_WIN_ID);
#ifndef MMI_GUI_STYLE_TYPICAL
    CC_INSTANCE.cc_display_type = CC_DISPLAY_HOT_BUTTONS;
    CC_DisplayByType(MMICC_STATUS_WIN_ID);
    MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON , PNULL);
#endif    
    //start timer
    MMIICC_StarCallTimingTimer();
}

#endif


#ifdef BROWSER_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:louis.wei
//	Note:从电话界面切换到浏览器界面
/*****************************************************************************/
PUBLIC void MMIAPICC_SwitchToBrowser( void )
{
    MMI_WIN_ID_T win_id = MMICC_QUERY_SWITCH_TO_HANDFREE_WIN_ID;
    MMICC_AUDIO_DEV_E audio_device = MMICC_GetCurrentDev();
    SCI_TRACE_LOW("MMIAPICC_SwitchToBrowser audio_device= %d",audio_device);
    if ((MMICC_AUDIO_DEV_BT == audio_device)
        ||(MMICC_AUDIO_DEV_EP == audio_device)
        ||(MMICC_AUDIO_DEV_HF == audio_device))
    {
        SetBrowserActive();
    }
    else
    {
#ifdef VT_SUPPORT
        if (MMIAPIVT_IsVtCalling())//bug337268
        {
            MMIVT_APPLET_T* vt_applet_ptr = MMIVT_GetAppletInstance();
            SCI_TRACE_LOW("MMIAPICC_SwitchToBrowser vt calling");
            if (PNULL == vt_applet_ptr)
            {
                SCI_TRACE_LOW("MMIAPICC_SwitchToBrowser vt applet is null");
                MMIPUB_OpenAlertFailWin(TXT_COMM_ERROR);
            }
            else
            {
               MMI_HANDLE_T applet_handle = PNULL;
               applet_handle = vt_applet_ptr->caf_applet.app_handle;
               MMIPUB_OpenQueryWinByTextIdEx(applet_handle,TXT_CC_IS_SWITCH_TO_HANDFREE, IMAGE_PUBWIN_QUERY, &win_id, HandleSwitchToHandfreeQueryWindow,0);
            }
        }
        else
#endif
        MMIPUB_OpenQueryWinByTextIdEx(CC_APPLET_HANDLE,TXT_CC_IS_SWITCH_TO_HANDFREE, IMAGE_PUBWIN_QUERY, &win_id, HandleSwitchToHandfreeQueryWindow,0);
    }

}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:louis.wei
//	Note: 从浏览器切换到电话界面
/*****************************************************************************/
PUBLIC void MMIAPICC_SwitchFromBrowser( void )
{
#ifdef VT_SUPPORT
    if (!MMIAPIVT_ActiveVT())
#endif
    {
        if (MMK_IsOpenWin(MMICC_STATUS_WIN_ID))
        {
            MMK_WinGrabFocus(MMICC_STATUS_WIN_ID);
        }
        else
        {
            MMIAPIIDLE_EnterDialWin();
        }
    }
}
#endif
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//+++ mdy by qing.yu@for cr238867
/*****************************************************************************/
//  Description : 收到蓝牙调节音量的消息，显示音量调节窗口
//  Global resource dependence : 
//  Author: Qing.Yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_BTSpeakerVolChange(void)
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();

    MMIAPICC_SetCallVolume(MMIAPICC_GetBtSpeakerVol());

    MMK_CloseWin(MMICOMMON_PANEL_WIN_ID);

    MMIAPICC_OpenAdjustVolumeWin(win_id, 0);
    return TRUE;
}
//--- mdy by qing.yu@for cr238867
#endif

#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : only for simulator
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
#ifdef WIN32
PUBLIC void MV_DEMO_Open(BOOLEAN isSwitch)
{
}
PUBLIC void MV_DEMO_Close(BOOLEAN isSwitch)
{
}
PUBLIC void MV_DEMO_SetParam(uint32  paramid, int32 paramvalue)
{
}
PUBLIC uint32 MV_DEMO_Get_ResourceStatus(void)
{
    return 0;
}
#endif

/*****************************************************************************/
// 	Description : open magic voice when MMICC_STATUS_WIN_ID open if switch is on
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void OpenMagicVoice(void)
{
    uint32 i = 0;
    BOOLEAN is_need_mute = !MMICC_IsMute();//如果已经本端静音功能已经打开，则这里不需要打开/关闭静音功能

    if (!IsCanUseMagicVoice(TRUE))
    {
        //SCI_TRACE_LOW:"ccapp OpenMagicVoice mv cannot open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14532_112_2_18_2_11_11_396,(uint8*)"");
        return;
    }
    
    if(s_is_mv_working)
    {
        //SCI_TRACE_LOW:"ccapp OpenMagicVoice mv is working"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14538_112_2_18_2_11_11_397,(uint8*)"");
        return;
    }
    
    if (is_need_mute)
    {
        MMICC_EnableMute(TRUE); 
    }
#if 0    
    MMIAUDIO_StopAGC();
#ifndef WIN32
    AUD_AGC_LockSwitch(SCI_TRUE);
#endif
#endif
    
    for (i = 0; i < (MMISET_MV_MAX_NUM - 2); i ++)
    {
        // 在MV_DEMO_OPEN(FALSE)之前设置好参数
        MV_DEMO_SetParam(i, s_mv_data[i].value);
    }
    MV_DEMO_Open(FALSE);
    s_is_mv_working = TRUE;
    SCI_Sleep(200);
    if (is_need_mute)
    {
        MMICC_EnableMute(FALSE);
    }
}

/*****************************************************************************/
// Description : enable magic voice
//Global resource dependence : 
//  Author: samboo.shen
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_EnableMV(void)
{
    if (MMICC_IsMagicVoiceOpenLater())
    {
        OpenMagicVoice();
        s_is_need_open_mv = FALSE;/*set to FALSE,because it only is usefull before call has been connnected*/
    }
}

/*****************************************************************************/
// 	Description : set magic voice parameter when MMICC_STATUS_WIN_ID open
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void SetMagicVoiceParam(void)
{
    uint32 i = 0;
    BOOLEAN is_need_mute = !MMICC_IsMute();//如果已经本端静音功能已经打开，则这里不需要打开/关闭静音功能

    if(!s_is_mv_working)
    {
        //SCI_TRACE_LOW:"ccapp SetMagicVoiceParam mv is not working"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14591_112_2_18_2_11_12_398,(uint8*)"");
        return;
    }

    if (is_need_mute)
    {
        MMICC_EnableMute(TRUE);
    }
    // 设置参数之前要先关掉后设置再打开(TRUE)
    MV_DEMO_Close(TRUE);
    for (i = 0; i < MMISET_MV_MAX_NUM - 2; i ++)
    {
        MV_DEMO_SetParam(i, s_mv_data[i].value);
    }
    MV_DEMO_Open(TRUE);
    SCI_Sleep(200);
    if (is_need_mute)
    {
        MMICC_EnableMute(FALSE);
    }
}

/*****************************************************************************/
// 	Description : copy magic voice data when selection window open and close
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL void CopyMagicVoiceData(MV_DATA_T* dst_data_ptr, MV_DATA_T* src_data_ptr)
{
    uint32 i = 0;

    if(PNULL == dst_data_ptr ||PNULL == src_data_ptr)
    {
        //SCI_TRACE_LOW:"CopyMagicVoiceData dst_data_ptr OR src_data_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14623_112_2_18_2_11_12_399,(uint8*)"");
        return;
    }
     
    for (i = 0; i < MMISET_MV_MAX_NUM; i ++)
    {
        dst_data_ptr[i].value = NULL != src_data_ptr ? src_data_ptr[i].value : 0;
    }
}

/*****************************************************************************/
// 	Description : close magic voice when call is disconnected
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void CloseMagicVoice(void)
{
    if(!s_is_mv_working)
    {
        //SCI_TRACE_LOW:"ccapp CloseMagicVoice mv is not working"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14641_112_2_18_2_11_12_400,(uint8*)"");
        return;
    }
    
    // 注意是FALSE
    MV_DEMO_Close(FALSE);
    s_is_mv_working = FALSE;
    
#ifndef WIN32
    AUD_AGC_LockSwitch(SCI_FALSE);
#endif

}

/*****************************************************************************/
// 	Description : magic voice needs to open later(when call has been connected)
//	Global resource dependence : 
//  Author : fen.xie
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMagicVoiceOpenLater(void)
{
    return (s_is_need_open_mv && IsCanUseMagicVoice(FALSE));
}
/*****************************************************************************/
// 	Description : when handfree or bluetooth active, can't use magic voice
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsCanUseMagicVoice(
                        BOOLEAN is_for_open_select_win/*IN,TRUE:判断是否需要打开魔音选择窗口*/
                                                      /*FALSE:是否需要打开魔音效果*/
                        )
{
    BT_ADDRESS  address = {0};
    uint32 i = 0;
    
    if (!MMIAPIBT_GetActivatedHS(&address) && !MMIAPICC_IsHandFree() 
#ifdef MMI_RECORD_SUPPORT
        && !IsCCRecording() 
#endif
        && MMIAPISET_GetIsMagicVoice())
    {
        if (is_for_open_select_win)
        {
            // 不考虑魔音参数是否全为0
            return TRUE;
        }
        else
        {
            for (i = 0; i < (MMISET_MV_MAX_NUM - 2); i ++)
            {
                // 只要有一个参数不为0，就可以打开魔音
                if (s_mv_data[i].value > 0)
                {
                    return TRUE;
                }
            }
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : whether magic voice is working
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsMagicVoiceWorking(void)
{
    return IsMagicVoiceWorking();
}

/*****************************************************************************/
// 	Description : whether magic voice is working
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsMagicVoiceWorking(void)
{
    return s_is_mv_working;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author : 
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_SetIsNeedOpenMv(BOOLEAN is_need_open)
{
    s_is_need_open_mv = is_need_open;
}


/*****************************************************************************/
// 	Description : reset magic voice value(s_mv_data) used in cc
//	Global resource dependence : 
//  Author : fen.xie
//	Note:MS00170872
/*****************************************************************************/
PUBLIC void ResetCCMagicVoiceValue(void)
{
    uint32 i = 0;
    MMISET_MV_INFO_T mv_info = {0};
    
    MMIAPISET_GetMagicVoiceInfo(&mv_info);
    
    for (i = 0; i < MMISET_MV_MAX_NUM; i ++)
    {
        s_mv_data[i].value = mv_info.mv_value[i];
    }
}

/*****************************************************************************/
// 	Description : reset magic voice value in setting window with nv value
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void ResetMagicVoiceValue(void)
{
    uint32 i = 0;
    MMISET_MV_INFO_T mv_info = {0};
    
    MMIAPISET_GetMagicVoiceInfo(&mv_info);
    
    for (i = 0; i < MMISET_MV_MAX_NUM; i ++)
    {
        s_mv_setting_data[i].value = mv_info.mv_value[i];
    }
}


/*****************************************************************************/
// 	Description : get magic voice display text by item and item value
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetMagicVoiceItemText(MMISET_MV_ITEM_E mv_item, uint32 value)
{
    MMI_TEXT_ID_T text_id = 0;
    
    switch(mv_item)
    {
    case MMISET_MV_CHANGE:
        text_id = TXT_MV_CHANGE_NONE + value;
        break;
        
    case MMISET_MV_ROLE:
        text_id = TXT_MV_ROLE_NONE + value;
        break;

#ifdef MV_EFFECT_SUPPORT
    case MMISET_MV_EFFECT:
        text_id = TXT_MV_EFFECT_NONE + value;
        break;
#endif        

    case MMISET_MV_EQ:
        text_id = TXT_MV_EQ_NONE + value;
        break;
        
    case MMISET_MV_BG:
        text_id = TXT_MV_BGID_NONE + value;
        break;
        
    case MMISET_MV_BG_VOL:
        text_id = TXT_NULL;//TXT_MV_VOLUME_LEVEL0 + value;
        break;
        
    case MMISET_MV_MV_VOL:
        text_id = TXT_NULL;//TXT_MV_VOLUME_LEVEL0 + value;
        break;
        
    default:
        text_id = TXT_NULL;
        break;
    }
    
    return text_id;
}

/*****************************************************************************/
// 	Description : for list control display
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL void SetMagicVoiceListItemData(
                                     GUILIST_ITEM_T* item_t_ptr,
                                     GUILIST_ITEM_DATA_T* item_data_ptr,
                                     MMI_IMAGE_ID_T image_id,
                                     MMI_TEXT_ID_T text_id0,
                                     MMI_TEXT_ID_T text_id1,
                                     MMI_TEXT_ID_T msk_test_id
                                     )
{
    if(PNULL == item_t_ptr ||PNULL == item_data_ptr)
    {
        //SCI_TRACE_LOW:"CopyMagicVoiceData item_t_ptr OR item_data_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_WINTAB_14828_112_2_18_2_11_12_401,(uint8*)"");
        return;
    }

#ifndef MMI_GUI_STYLE_TYPICAL
    item_t_ptr->item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM;
    item_t_ptr->item_data_ptr = item_data_ptr;
    
    item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data_ptr->item_content[0].item_data.text_id = image_id;
    item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data_ptr->item_content[1].item_data.image_id = text_id0;
    item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data_ptr->item_content[2].item_data.text_id = IMAGE_COMMON_RIGHT_ARROW;
    item_data_ptr->item_content[3].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data_ptr->item_content[3].item_data.text_id = text_id1;
#else
    item_t_ptr->item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
    item_t_ptr->item_data_ptr = item_data_ptr;
    
    item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data_ptr->item_content[0].item_data.image_id = image_id;
    item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data_ptr->item_content[1].item_data.text_id = text_id0;
    item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data_ptr->item_content[2].item_data.text_id = text_id1;
    
    item_data_ptr->softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
    item_data_ptr->softkey_id[1] = (MMI_TEXT_ID_T)msk_test_id;
    item_data_ptr->softkey_id[2] = (MMI_TEXT_ID_T)STXT_CANCEL;
#endif
}

/*****************************************************************************/
// 	Description : magic voice item selection window callback function
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceItemSelectWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMICC_MAGIC_VOICE_ITEM_CTRL_ID;
    param = param;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined(MMI_PDA_SUPPORT)
        if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            //MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
            //MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
        }
#endif
        {
            uint32 i = 0;
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            // 通过主选择窗口的选择项来决定子选择窗口的选择内容
            uint16 item_index = GUILIST_GetCurItemIndex(MMICC_MAGIC_VOICE_MAIN_CTRL_ID);
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUILIST_SetMaxItem(ctrl_id, s_mv_item_num[item_index], FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            for (i = 0; i < s_mv_item_num[item_index]; i ++)
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = GetMagicVoiceItemText((MMISET_MV_ITEM_E)item_index, i);
                GUILIST_AppendItem(ctrl_id, &item_t);
            }
            
            GUILIST_SetSelectedItem(ctrl_id, (uint16)s_mv_setting_data[item_index].value, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, (uint16)s_mv_setting_data[item_index].value);              
            GUILIST_ValidCurItemPos(ctrl_id);
            
            GUIWIN_SetTitleTextId(win_id, s_mv_setting_data[item_index].text_id, FALSE);
        }
        break;

    case MSG_GET_FOCUS:
        if (!IsCanUseMagicVoice(TRUE))
        {
            MMK_CloseWin(win_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
#if !defined(MMI_PDA_SUPPORT)
    case MSG_CTL_OK:
#endif
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        {
            uint16 item_index = GUILIST_GetCurItemIndex(MMICC_MAGIC_VOICE_MAIN_CTRL_ID);
            s_mv_setting_data[item_index].value = GUILIST_GetCurItemIndex(ctrl_id);
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : magic voice main selection window callback function
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMagicVoiceMainSelectWinMsg(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMICC_MAGIC_VOICE_MAIN_CTRL_ID;
    CC_MAKE_CALL_INFO* make_call_info_ptr = (CC_MAKE_CALL_INFO*)MMK_GetWinAddDataPtr(win_id);
    //static MV_DATA_T s_old_mv_data[MMISET_MV_MAX_NUM] = {0};
    static BOOLEAN s_is_resource_exist = FALSE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            //MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
            //MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
        }
#endif
        {
            uint32 i = 0;

            if(MMICC_MV_MENU_TYPE_CC == s_mv_menu_open_type)
            {
                s_is_resource_exist = (BOOLEAN)MV_DEMO_Get_ResourceStatus();
                CopyMagicVoiceData(s_mv_setting_data, s_mv_data);

                //@fen.xie MS00177206 begin:MO/MT时魔音效果默认是开启

                if (!MMK_IsOpenWin(MMICC_STATUS_WIN_ID)
#ifndef MMI_GUI_STYLE_TYPICAL
                ||(MMK_IsOpenWin(MMICC_STATUS_WIN_ID) && MMICC_IsExistOutgoingCall())
#endif
                )
                {                
                    s_is_need_open_mv = TRUE;
                }
                else/*在通话中进入该菜单时，无需设置s_is_need_open_mv的默认值*/
                {
                }
                 //@fen.xie MS00177206 end
            }
            else if(MMICC_MV_MENU_TYPE_SET == s_mv_menu_open_type)
            {
                GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_CC_MAGICVOICE_PARAM_SET, FALSE);//@fen.xie MS00185592
                s_is_resource_exist = TRUE;
                ResetMagicVoiceValue();
            }

            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUILIST_SetMaxItem(ctrl_id, MMISET_MV_MAX_NUM, FALSE);
            // MV_MAX_NUM - 2 暂时去掉调节音量的功能
            for (i = 0; i < MMISET_MV_MAX_NUM - 2; i ++)
            {
                GUILIST_ITEM_T item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                BOOLEAN is_gray = ((MMISET_MV_BG == i) && !s_is_resource_exist) ? TRUE : FALSE;
                
                SetMagicVoiceListItemData(&item_t,
                    &item_data,
                    s_mv_setting_data[i].image_id,
                    s_mv_setting_data[i].text_id,
                    GetMagicVoiceItemText(i, s_mv_setting_data[i].value),
                    (is_gray ? TXT_NULL : TXT_MODIFY));
                
                GUILIST_AppendItem(ctrl_id, &item_t);
                GUILIST_SetItemGray(ctrl_id, i, is_gray);
            }
        }
        break;
        
    case MSG_GET_FOCUS:
        if (!IsCanUseMagicVoice(TRUE))
        {
            MMK_CloseWin(win_id);
        }        
        {
            GUILIST_ITEM_T item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            uint16 item_index = GUILIST_GetCurItemIndex(ctrl_id);
            BOOLEAN is_gray = ((MMISET_MV_BG == item_index) && !s_is_resource_exist) ? TRUE : FALSE;
            
            SetMagicVoiceListItemData(&item_t,
                &item_data,
                s_mv_setting_data[item_index].image_id,
                s_mv_setting_data[item_index].text_id,
                GetMagicVoiceItemText((MMISET_MV_ITEM_E)item_index, s_mv_setting_data[item_index].value),
                (is_gray ? TXT_NULL : TXT_MODIFY));
            
            GUILIST_ReplaceItem(ctrl_id, &item_t, (uint16)item_index);
            GUILIST_SetItemGray(ctrl_id, item_index, is_gray);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16 item_index = GUILIST_GetCurItemIndex(ctrl_id);
            BOOLEAN is_gray = ((MMISET_MV_BG == item_index) && !s_is_resource_exist) ? TRUE : FALSE;
            
            if (!is_gray)
            {   
                //mv win may be opened in setting module, not in calling
                MMI_HANDLE_T applet_handle = 0;
                if(PNULL != MMICC_GetAppletInstance())
                {
                    applet_handle = CC_APPLET_HANDLE;
                }
                MMK_CreatePubListWinEx(applet_handle, (uint32*)MMICC_MAGIC_VOICE_ITEM_SELECT_WIN_TAB, NULL);
            }
        }
        break;
        
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        {
            uint16 item_index = GUILIST_GetCurItemIndex(ctrl_id);
            BOOLEAN is_gray = ((MMISET_MV_BG == item_index) && !s_is_resource_exist) ? TRUE : FALSE;

            if (!is_gray)
            {
                GUILIST_ITEM_T item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                
                if(MSG_APP_LEFT == msg_id)
                {
                    if(s_mv_setting_data[item_index].value > 0)
                    {
                        s_mv_setting_data[item_index].value --;
                    }
                    else if(0 == s_mv_setting_data[item_index].value)
                    {
                        if (1 <= s_mv_item_num[item_index])
                        {
                            s_mv_setting_data[item_index].value = s_mv_item_num[item_index]-1;
                        }
                    }
                }
                else if(MSG_APP_RIGHT == msg_id)
                {
                    if ((s_mv_setting_data[item_index].value + 1) < s_mv_item_num[item_index])
                    {
                        s_mv_setting_data[item_index].value ++;
                    }
                    else if((s_mv_setting_data[item_index].value + 1) == s_mv_item_num[item_index])
                    {
                        s_mv_setting_data[item_index].value = 0;
                    }
                }

                SetMagicVoiceListItemData(&item_t,
                    &item_data,
                    s_mv_setting_data[item_index].image_id,
                    s_mv_setting_data[item_index].text_id,
                    GetMagicVoiceItemText((MMISET_MV_ITEM_E)item_index, s_mv_setting_data[item_index].value),
                    TXT_MODIFY);
                
                GUILIST_ReplaceItem(ctrl_id, &item_t, (uint16)item_index);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, 0, 0);
            }
        }
        break;
#if 0        
    case MSG_APP_HEADSET_BUTTON:
        //来电过程中按耳机按键可以接听电话
        if (MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
        {
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
        break;
#endif        
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            if(MMICC_MV_MENU_TYPE_CC == s_mv_menu_open_type)
            {
                CopyMagicVoiceData(s_mv_data, s_mv_setting_data);/*@fen.xie add:save the setting date to mv for cc*/
                
                if (MMK_IsOpenWin(MMICC_STATUS_WIN_ID)
#ifndef MMI_GUI_STYLE_TYPICAL
                &&(!MMICC_IsExistOutgoingCall())
#endif
                )
                {
                    if (IsMagicVoiceWorking())
                    {
                        if (IsCanUseMagicVoice(FALSE))
                        {
                            SetMagicVoiceParam();
                        }
                        else
                        {
                            CloseMagicVoice();
                            //if magic voice is open, HandFree & Record button should be set to active.
#ifndef MMI_CALLCONTROL_MINI_SUPPORT                             
                            CC_SetHotButtonGrayState(CC_BUTTON_HANDFREE, FALSE);
                            CC_SetHotButtonGrayState(CC_BUTTON_RECORD, FALSE);
#endif                            
                        }
                    }
                    else
                    {
                        if (IsCanUseMagicVoice(FALSE))
                        {
                            OpenMagicVoice();
                            //if magic voice is open, HandFree & Record button should be set to gray.
#ifndef MMI_CALLCONTROL_MINI_SUPPORT                             
                            CC_SetHotButtonGrayState(CC_BUTTON_HANDFREE, TRUE);
                            CC_SetHotButtonGrayState(CC_BUTTON_RECORD, TRUE);
#endif                            
                        }
                    }
                    
                }
#ifdef ASP_SUPPORT
                // 如果答录通话在处在被用户接听的状态....
                else if(MMIAPIASP_lsAspCallSpeaking())
                {
                    UpdateCurrentASPCall();
#ifdef MV_SUPPORT
                    //when mt call is asp handling, user accept, need open mv 
                    MMIAPICC_EnableMV();
#endif                    
                }
#endif
                else if (MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))
                {
                    // 来电话时，模拟按绿键
                    MMK_SendMsg(MMICC_ANIMATION_WIN_ID, MSG_APP_GREEN, NULL);
                }
                else
                {
                    // 拨电话时，继续拨电话的流程
                    MMICC_ProcessMakeCall(make_call_info_ptr->dual_sys, make_call_info_ptr);
                }
            }
            else if(MMICC_MV_MENU_TYPE_SET == s_mv_menu_open_type)
            {
                //@fen.xie add begin:set the setting date to nv
                uint32  i = 0;   
                MMISET_MV_INFO_T mv_info = {0};

                for(i=0; i<MMISET_MV_MAX_NUM; i++)
                {
                    mv_info.mv_value[i] = s_mv_setting_data[i].value;
                }
                MMIAPISET_SetMagicVoiceInfo(mv_info);
                //@fen.xie add end
#ifdef MMI_GUI_STYLE_TYPICAL
                MMICC_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            if (!MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))//xuetao MS228957
            {
                MMK_CloseWin(win_id);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if(MMICC_MV_MENU_TYPE_CC == s_mv_menu_open_type)
        {
                //@fen.xie MS00177206:MO/MT时选择取消键,则取消魔音效果
                s_is_need_open_mv = FALSE;
                
#ifdef ASP_SUPPORT
            // 如果答录通话在处在被用户接听的状态....
            if(MMIAPIASP_lsAspCallSpeaking())
            {
                UpdateCurrentASPCall();
            }
            else if (!MMK_IsOpenWin(MMICC_STATUS_WIN_ID))
#else
            if (!MMK_IsOpenWin(MMICC_STATUS_WIN_ID))
#endif
            {                               
                if (MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))
                {
                    MMK_SendMsg(MMICC_ANIMATION_WIN_ID, MSG_APP_GREEN, NULL);
                }
                else
                {
                    MMICC_ProcessMakeCall(make_call_info_ptr->dual_sys, make_call_info_ptr);
                }
            }
        }
        if (!MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))//xuetao MS228957
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        {
            uint32  i = 0;   

            for(i = 0; i < MMISET_MV_MAX_NUM; i++)
            {
                s_mv_setting_data[i].value = 0;
            }
        }
        
        if(MMICC_MV_MENU_TYPE_CC == s_mv_menu_open_type)
        {
            // 拨电话时，make_call_info_ptr记录了电话的信息，所以在此处清除
            if (PNULL != make_call_info_ptr)
            {
                if (PNULL != make_call_info_ptr->disp_ptr)
                {
                    if (PNULL != make_call_info_ptr->disp_ptr->wstr_ptr)
                    {
                        SCI_FREE(make_call_info_ptr->disp_ptr->wstr_ptr);
                        make_call_info_ptr->disp_ptr->wstr_ptr = PNULL;
                    }
                    
                    SCI_FREE(make_call_info_ptr->disp_ptr);
                    make_call_info_ptr->disp_ptr = PNULL;
                }
                
                if (PNULL != make_call_info_ptr->name_ptr)
                {
                    if (PNULL != make_call_info_ptr->name_ptr->wstr_ptr)
                    {
                        SCI_FREE(make_call_info_ptr->name_ptr->wstr_ptr);
                        make_call_info_ptr->name_ptr->wstr_ptr = PNULL;
                    }
                    
                    SCI_FREE(make_call_info_ptr->name_ptr);
                    make_call_info_ptr->name_ptr = PNULL;
                }
                
                SCI_FREE(make_call_info_ptr);
                make_call_info_ptr = PNULL;
            }
            
            //当去电时打开魔音选择窗口,选择red键关闭窗口并结束呼出时,也需要及时处理MMI_Enable3DMMI
            if (!MMICC_IsExistOutgoingCall() && (0 == MMICC_GetCallNum()))
            {
#ifdef UI_P3D_SUPPORT
                MMI_Enable3DEffect(MMI_3D_EFFECT_MAGIC_VOICE);
#endif             
                //MMI_Enable3DMMI(TRUE);
            }
        }
        s_mv_menu_open_type = MMICC_MV_MENU_TYPE_NONE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : open main selection window when incoming outgoing or in call
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
LOCAL void OpenMagicVoiceSelectWindow(ADD_DATA call_info_ptr)
{
    MMI_HANDLE_T applet_handle = 0;
    if((PNULL != MMICC_GetAppletInstance())&&(!MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID)))
    {
        applet_handle = CC_APPLET_HANDLE;
    }
    MMK_CloseWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID);
    MMK_CloseWin(MMICC_MAGIC_VOICE_ITEM_SELECT_WIN_ID);

    s_mv_menu_open_type = MMICC_MV_MENU_TYPE_CC;
    MMK_CreatePubListWinEx(applet_handle, (uint32*)MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_TAB, call_info_ptr);
}

/*****************************************************************************/
// 	Description : open main selection window when calling or just setting.
//	Global resource dependence : 
//  Author : 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_OpenMagicVoiceSetMenu(void)
{
    MMI_HANDLE_T applet_handle = 0;
    if(PNULL != MMICC_GetAppletInstance())
    {
        applet_handle = CC_APPLET_HANDLE;
    }

    if(!MMK_IsOpenWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID))
    {
        s_mv_menu_open_type = MMICC_MV_MENU_TYPE_SET;
        MMK_CreatePubListWinEx(applet_handle, (uint32*)MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_TAB, NULL);
    }
}

/*****************************************************************************/
// 	Description : open main selection window when incoming or outgoing
//	Global resource dependence : 
//  Author : fen.xie
//	Note:   MS00185659
/*****************************************************************************/
PUBLIC void MMICC_OpenMVBeforeCallConnected(ADD_DATA call_info_ptr)
{
    /*
        block冲突,关闭3D特效;只要魔音的开关处于开启,无论参数是否为空都关闭冲突的block;
        因为在通话中有可能反复开启关闭魔音的背景音效果,从而使用到mv 的 block
    */
#if  0   
    if(MMI_Is3DMMIEnable())
    {
        //MMI_Enable3DMMI(FALSE);             
    }
#endif    
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_MAGIC_VOICE);
#endif 

    //初始化mv的数据
    ResetCCMagicVoiceValue();

    OpenMagicVoiceSelectWindow(call_info_ptr);
}
#endif

/*****************************************************************************/
//  Discription: Handle ECT alert
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_HandleECTAlertWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMICC_UpdateCurrentCallStatus(TRUE);

        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;     
}

/*****************************************************************************/
//  Description : open success window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_OpenAlertSuccessWin(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T win_id = MMICC_HOLDCNF_PROMPT_WIN_ID;
    CHECK_CC_APPLET_NOT_NULL
    MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0);
}
/*****************************************************************************/
//  Description : open warning window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMICC_OpenAlertWarningWin(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T win_id = MMICC_HOLDCNF_PROMPT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
    CHECK_CC_APPLET_NOT_NULL
    MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,&win_id,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL,0);
}
/*****************************************************************************/
//  Description :open fail window by text_id 
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMICC_OpenAlertFailWin(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T win_id = MMICC_HOLDCNF_PROMPT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

    CHECK_CC_APPLET_NOT_NULL

    MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,&win_id,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL,0);
}

/*****************************************************************************/
//  Description :open waiting window by text_id 
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMICC_OpenAlertWaitingWin(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T win_id = MMICC_HOLDCNF_PROMPT_WIN_ID;
    CHECK_CC_APPLET_NOT_NULL
    MMIPUB_OpenAlertWinByTextIdEx(CC_APPLET_HANDLE,PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WAIT,&win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0);
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :SET PUBWIN TITLE
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetAlertWinTitleTextId(MMI_TEXT_ID_T text_id)
{
    MMIPUB_SetWinTitleTextId( MMK_GetWinHandle(CC_APPLET_HANDLE, MMICC_HOLDCNF_PROMPT_WIN_ID), text_id, FALSE );
}
#endif

/*****************************************************************************/
//  Description :Stop Call Control Timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopCCTimer(uint8* timer_id)
{
    if (PNULL != timer_id && 0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);

        *timer_id = 0;
    }
}

/*****************************************************************************/
//  Description :Start Call Control Timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  StartCCTimer(MMI_HANDLE_T win_id, uint8 *timer_id, uint32 timeout)
{
    if (PNULL != timer_id)
    {
        StopCCTimer(timer_id);

        *timer_id = MMK_CreateWinTimer(win_id, timeout, FALSE);
    }
}

#ifdef MMI_CALLCONTROL_MINI_SUPPORT
/*****************************************************************************/
//    Description : RGB888 translate to RGB565
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL  uint16 RGB888TO565(uint32 rgb888)
{
    unsigned int r888 = (rgb888 & 0xff0000) >> 16;
    unsigned int g888 = (rgb888 & 0x00ff00) >> 8;
    unsigned int b888 = (rgb888 & 0x0000ff);
    unsigned int r565 = r888 >> 3;
    unsigned int g565 = g888 >> 2;
    unsigned int b565 = b888 >> 3;
    unsigned int rgb565 = (r565 << (5 + 6)) | (g565 << 5) | b565;

    return rgb565;
}

/*****************************************************************************/
//  Description : 创建多个电话的分割线
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
LOCAL void DrawCcPanelLine(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_BOTH_RECT_T *rect_ptr)
{
    GUIBUTTON_INIT_DATA_T     line_init = {0};    
    MMI_CONTROL_CREATE_T button_ctrl = {0};
    GUI_BG_T bg_info = {0};
    
    line_init.both_rect.v_rect.left = rect_ptr->v_rect.left;
    line_init.both_rect.v_rect.right = rect_ptr->v_rect.right;
    line_init.both_rect.v_rect.top = rect_ptr->v_rect.top;
    line_init.both_rect.v_rect.bottom = rect_ptr->v_rect.bottom;

    line_init.both_rect.h_rect.left = rect_ptr->h_rect.left;
    line_init.both_rect.h_rect.right = rect_ptr->h_rect.right;
    line_init.both_rect.h_rect.top = rect_ptr->h_rect.top;
    line_init.both_rect.h_rect.bottom = rect_ptr->h_rect.bottom;

    if(0 != MMK_GetCtrlHandleByWin(win_id, ctrl_id))
    {
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id,ctrl_id));
    }
    
    button_ctrl.ctrl_id           = ctrl_id;
    button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    button_ctrl.init_data_ptr     = &line_init;
    button_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&button_ctrl);

    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = MMI_GREEN_COLOR;
    GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, ctrl_id), TRUE, &bg_info, FALSE);            
}
#endif

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : open emergency menu win when dial emergency num(112, 911 etc) 
//	Global resource dependence : 
//  Author:zihai.lu
//	Note:
/*****************************************************************************/
LOCAL void  OpenCCEmergencyMenu(void)
{
    MMI_WIN_ID_T    win_id = MMICC_MENU_EMERGENCY_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMICC_EMERGENCY_WIN_MENU_CTRL_ID;
    MMI_TEXT_ID_T   label = TXT_NULL; 
    uint            cnt = 0;
    //creat dynamic menu   
    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);

    //handfree
    label = MMIAPICC_IsHandFree() ? TXT_HAND_ON: TXT_COMMON_HAND_FREE;
    InsertNodeByLabel(ctrl_id, label, CC_EMERGENCY_MENU_NODE_HANDFREE, 0, cnt++, IMAGE_NULL, FALSE); 

    //-----mute
    label = (MMICC_IsMute()) ? TXT_TURNOFF_MUTE : TXT_TURNON_MUTE;
    InsertNodeByLabel(ctrl_id, label, CC_EMERGENCY_MENU_NODE_MUTE, 0, cnt++, IMAGE_NULL, FALSE);

    //hung up
    label = TXT_CC_HANG_OFF;
    InsertNodeByLabel(ctrl_id, label, CC_EMERGENCY_MENU_NODE_HUNG_UP, 0, cnt++, IMAGE_NULL, FALSE);
}

/*****************************************************************************/
// 	Description : handle emergency menu win msg when dial emergency num
//	Global resource dependence : 
//  Author:zihai.lu
//	Note:
/*****************************************************************************/
MMI_RESULT_E  MMICC_HandleEmergencyMenuWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16          node_id = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        OpenCCEmergencyMenu();
        MMK_SetAtvCtrl(win_id, MMICC_EMERGENCY_WIN_MENU_CTRL_ID);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        node_id = GUIMENU_GetCurNodeId(MMICC_EMERGENCY_WIN_MENU_CTRL_ID);
        switch(node_id)
        {
        case CC_EMERGENCY_MENU_NODE_HANDFREE:
            if (MMIAPICC_IsHandFree())
            {
                EnableHandfreeMode(FALSE);
            }
            else
            {
                EnableHandfreeMode(TRUE);
            }
            MMK_CloseWin(win_id);
            break;
            
        case CC_EMERGENCY_MENU_NODE_MUTE:
            {
                EnableMuteMode((BOOLEAN)(!MMICC_IsMute()), TRUE);
                MMK_CloseWin(win_id);
            }
            break;

        case CC_EMERGENCY_MENU_NODE_HUNG_UP:
            MMIAPICC_ReleaseCallByRedkey();
            MMICC_StopRingOrVibrateBeforeCCing();
            MMK_CloseWin(win_id);
            break;

        default:
            MMK_CloseWin(win_id);
            break;
        }
        break;

        case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : to open cc emergency menu
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void  MMICC_OpenEmergency_OptionMenu(void)
{
    MMK_CreateWin((uint32 *)MMICC_MENU_EMERGENCY_OPT_WIN_TAB, PNULL);
}

#endif

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
/*****************************************************************************/
// 	Description : Stop panel Play DTMF Tone
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsPlayDialTone(MMI_MESSAGE_ID_E msg_id)
{
    MMI_WIN_ID_T win_id = MMK_GetFocusChildWinId();
    BOOLEAN ret = FALSE;

    win_id = MMK_GetWinId(win_id);

    if (MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID)
    || (MMIIDLE_DIAL_WIN_ID == win_id)
    || MMK_IsFocusWin(MAIN_IDLE_WIN_ID) // Bug-897462: 待机界面首次按0，#,*3个按键中的一个时，会先响应按键音再响应拨号音
#if !defined(MMI_GUI_STYLE_TYPICAL)
#if defined(TOUCH_PANEL_SUPPORT)
    || ((MMICC_ANIMATION_WIN_ID == MMK_GetFocusWinId() || MMICC_STATUS_WIN_ID == MMK_GetFocusWinId())
        && PNULL != MMICC_GetAppletInstance()
        && CC_DISPLAY_DTMF_PANEL == CC_INSTANCE.cc_display_type)
#endif
#endif
    )
    {
        if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
                || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION))
        {
            msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
        }

        if (MSG_KEYDOWN_1 <= msg_id && msg_id <= MSG_KEYDOWN_HASH)
        {
            ret = TRUE;
        }
    }

    SCI_TRACE_LOW("MMIAPIIDLE_IsPlayDialTone ret = %d", ret);
    return ret;
}
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : Open SIM Calling Prompt
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_OpenSIMCallingPrompt(void)
{
    MMI_STRING_T string_calling = {0};
    MMI_STRING_T string_info = {0};
    MMISET_SIM_NAME_T sim_name = {0};
    MN_DUAL_SYS_E dual_sys = MMIAPICC_GetCallingSIM();
    wchar ch_space = ' ';
    uint16 ch_space_len = 1;
    MMI_WIN_ID_T win_id = MMICC_SIM_CALLING_PROMPT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return FALSE;
    }

    sim_name = MMIAPISET_GetSimName(dual_sys);

    MMI_GetLabelTextByLang(TXT_CALL_USING, &string_calling);

    string_info.wstr_len = sim_name.wstr_len + string_calling.wstr_len + ch_space_len;
    string_info.wstr_ptr = SCI_ALLOCAZ((string_info.wstr_len + 1)*sizeof(wchar));

    if (PNULL == string_info.wstr_ptr)
    {
        return FALSE;
    }

    //SIM name
    MMI_WSTRNCPY(string_info.wstr_ptr, string_info.wstr_len, sim_name.wstr_arr, sim_name.wstr_len, sim_name.wstr_len);

    //Space
    MMI_WSTRNCPY(string_info.wstr_ptr + sim_name.wstr_len, string_calling.wstr_len + ch_space_len, &ch_space, ch_space_len, ch_space_len);

    //Calling
    MMI_WSTRNCPY(string_info.wstr_ptr + sim_name.wstr_len + 1, string_calling.wstr_len, string_calling.wstr_ptr, string_calling.wstr_len, string_calling.wstr_len);

    MMIPUB_OpenAlertWinByTextPtr(PNULL, &string_info, PNULL, IMAGE_PUBWIN_WARNING, &win_id, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);

    SCI_FREE(string_info.wstr_ptr);

    return TRUE;
}
#endif

/*Edit by script, ignore 5 case. Thu Apr 26 19:00:51 2012*/ //IGNORE9527


/*Edit by script, ignore 15 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
