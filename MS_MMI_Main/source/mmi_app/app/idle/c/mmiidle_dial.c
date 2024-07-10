/*****************************************************************************
** File Name:      mmiidle_dial.c                                            *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2009        Jassmine              Creat
******************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guiedit.h"
#ifdef TOUCH_PANEL_SUPPORT
#include "guibutton.h"
#include "mmiidle_dial.h"
#include "guitoolbar.h"
#endif
#ifndef MAINLCD_SIZE_320X480
#include "guictrl_api.h"
#endif
#include "guistatusbar.h"
#include "guires.h"
#include "mmicc_export.h"
#include "mmiidle_export.h"
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
#include "mmienvset_export.h"
#include "mmienvset_internal.h"
#endif
#include "mmi_id.h"
#include "mmidisplay_data.h"
#include "mmiidle_subwintab.h"
#include "mmi_appmsg.h"
#include "mmisms_export.h"
#include "mmi_menutable.h"
#include "mmipub.h"
#include "mmimms_export.h"
#include "mmicl_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmipb_export.h"
#if defined(TOUCH_PANEL_SUPPORT)
#include "guiform.h"
#endif
#if defined(TOUCH_PANEL_SUPPORT)&&!defined(MMI_PDA_SUPPORT) 
#include "guictrl_api.h"
#endif
#include "mmicc_image.h"
#include "mmipb_image.h"
#if defined(MMI_PDA_SUPPORT) 
#include "mmipb_view.h"
#include "mmicc_id.h"
#endif
#include "mmipb_id.h"
#include "guitab.h"
#include "mmipb_text.h"
#include "mmifdn_export.h"
#include "guiedit.h"
#if !defined(TOUCH_PANEL_SUPPORT)
#include "guitoolbar.h"
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
#include "mmisrvaud_util.h"
#include "mmisrvaud_api.h"
#endif
#include "mmi_resource.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "guicommon.h"
#ifdef ENG_SUPPORT
#include "mmieng_export.h"
#endif
#if defined(MMI_INDIAN_SOS_SUPPORT) && !defined(MMI_PDA_SUPPORT)
#include "mmiset_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIDLE_PB_FAST_SEACH_MAX_LEN   5
#define MMIIDLE_DIAL_VIBRATE_TIME       18  //拨号盘振动时间50
#define MMI_MAINAPP_SIM_MAX_ITEM     2
#define MMIIDLE_DIAL_MAX_LEN    40
#ifdef TOUCH_PANEL_SUPPORT        
#define MMIIDLE_DIAL_EDIT_RECT  {MMI_MAINAPP_DIALNUMBER_RECT_LEFT,MMI_MAINAPP_DIALNUMBER_RECT_TOP,MMI_MAINAPP_DIALNUMBER_RECT_RIGHT,MMI_MAINAPP_DIALNUMBER_RECT_BOTTOM}
#define MMIIDLE_DIAL_EDIT_RECT_H  {MMI_MAINAPP_DIALNUMBER_RECT_LEFT_H,MMI_MAINAPP_DIALNUMBER_RECT_TOP_H,MMI_MAINAPP_DIALNUMBER_RECT_RIGHT_H,MMI_MAINAPP_DIALNUMBER_RECT_BOTTOM_H}
#endif
#if defined(MAINLCD_SIZE_128X64)//@ alex added in 2010.11.27 --begin-->
#define MMIIDLE_DIAL_EDIT_RECT  {MMI_MAINAPP_DIALNUMBER_RECT_LEFT,MMI_MAINAPP_DIALNUMBER_RECT_TOP,MMI_MAINAPP_DIALNUMBER_RECT_RIGHT,MMI_MAINAPP_DIALNUMBER_RECT_BOTTOM}
#endif
//   <--end--
#if defined(TOUCH_PANEL_SUPPORT)
#define DIAL_NUM_BUTTON_ROW_MAX 4
#define DIAL_NUM_BUTTON_COLUMN_MAX 3
#else
#define DIAL_NUM_BUTTON_ROW_MAX 3//行
#define DIAL_NUM_BUTTON_COLUMN_MAX 4//列
#endif

#if defined(TOUCH_PANEL_SUPPORT)
#define DIAL_FUNC_BUTTON_NUM_MAX 3
#else
#define DIAL_FUNC_BUTTON_NUM_MAX 4
#endif

#define DIAL_BUTTON_ROW_H_MAX 3//行
#define DIAL_BUTTON_LINE_H_MAX 5//列

#ifdef DPHONE_SUPPORT
#define MMIIDLE_HANGUP_REMIND_TIME  30    //催挂音提示时间间隔
#define MMIIDLE_HANGUP_CHK_INTERVAL 1000    //催挂音检测间隔
#endif

#ifndef MMI_DIAL_MINI_SUPPORT
#define MMI_IDLE_DIAL_EDIT_NUM_COLOR		    GUI_RGB2RGB565(43,43,43)//0x4d60
#else
#define MMI_IDLE_DIAL_EDIT_NUM_COLOR		    0xffff
#endif
#define MMIIDLE_DIAL_LINE_WIDTH 1
#if defined(MAINLCD_SIZE_240X320)||defined(MAINLCD_SIZE_240X400)
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH 79
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_M 80
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT 31

#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_H 59
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H 30

#define MMIIDLE_DIAL_BUTTONS_HEIGHT 32
#if defined(TOUCH_PANEL_SUPPORT)
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT 40
#else
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT 45
#endif

#define MMI_DIAL_EDIT_LENGTH 198
#define MMI_DIAL_EDIT_LENGTH_H 146

#if defined(MMI_PDA_SUPPORT)
#define MMIIDLE_DIAL_BUTTONS_HEIGHT_H 31
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 35
#else
#define MMIIDLE_DIAL_BUTTONS_HEIGHT_H 35
#if defined(TOUCH_PANEL_SUPPORT)
#if defined(MAINLCD_SIZE_240X320)
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 35
#else
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 33
#endif
#else
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 45
#endif
#endif

#define MMICC_DTMF_NUMBER_BG_WIDTH_H 67
#define MMICC_DTMF_NUMBER_BG_HEIGHT_H 30
 #elif defined(MAINLCD_SIZE_320X480)
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH 106
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_M 106
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT 43

#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_H 79
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H 39

#define MMIIDLE_DIAL_BUTTONS_HEIGHT 56
#define MMIIDLE_DIAL_BUTTONS_HEIGHT_H 40

#define MMI_DIAL_EDIT_LENGTH 262
#define MMI_DIAL_EDIT_LENGTH_H 192

#define MMIIDLE_DIAL_EDIT_SHADOW_HEIGHT 9
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT 58
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 51

#define MMICC_DTMF_NUMBER_BG_WIDTH_H 93
#define MMICC_DTMF_NUMBER_BG_HEIGHT_H 41
#elif defined (MAINLCD_SIZE_176X220)
#define MMIIDLE_DIAL_BUTTONS_HEIGHT_H 31
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH 79
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_M 80
#define MMICC_DTMF_NUMBER_BG_WIDTH_H 67
#define MMICC_DTMF_NUMBER_BG_HEIGHT_H 30
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT 31
#define MMIIDLE_DIAL_BUTTONS_HEIGHT 32
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT 50
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 50//34
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_H 59
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H 30
#elif defined (MAINLCD_SIZE_128X160)
#define MMIIDLE_DIAL_BUTTONS_HEIGHT_H 31
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH 79
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_M 80
#define MMICC_DTMF_NUMBER_BG_WIDTH_H 67
#define MMICC_DTMF_NUMBER_BG_HEIGHT_H 30
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT 31
#define MMIIDLE_DIAL_BUTTONS_HEIGHT 32
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT 34//50
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 34
#define MMIIDLE_DIAL_NUMBER_BG_WIDTH_H 59
#define MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H 30
#else
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT 20
#define MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H 26
#endif
#define MMI_DIAL_LIST_RIGHT_H (MMI_MAINSCREEN_HEIGHT - 3*(MMIIDLE_DIAL_NUMBER_BG_WIDTH_H + MMIIDLE_DIAL_LINE_WIDTH) - 1)

#define MMIIDLE_DIAL_OFFSET_X (MMIIDLE_DIAL_NUMBER_BG_WIDTH + MMIIDLE_DIAL_LINE_WIDTH)
#define MMIIDLE_DIAL_OFFSET_M_X (MMIIDLE_DIAL_NUMBER_BG_WIDTH_M + MMIIDLE_DIAL_LINE_WIDTH)
#define MMIIDLE_DIAL_OFFSET_Y (MMIIDLE_DIAL_NUMBER_BG_HEIGHT + MMIIDLE_DIAL_LINE_WIDTH)
#define MMIIDLE_DIAL_OFFSET_X_H (MMIIDLE_DIAL_NUMBER_BG_WIDTH_H + MMIIDLE_DIAL_LINE_WIDTH)
#define MMIIDLE_DIAL_OFFSET_Y_H (MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH)
#define MMICC_DTMF_OFFSET_X_H (MMICC_DTMF_NUMBER_BG_WIDTH_H + MMIIDLE_DIAL_LINE_WIDTH)
#define MMICC_DTMF_OFFSET_Y_H (MMICC_DTMF_NUMBER_BG_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH)

#if defined(MMI_PDA_SUPPORT)
#define MMIIDLE_DIAL_NUMBER_TOP_Y (MMI_MAINSCREEN_HEIGHT - 3*(MMIIDLE_DIAL_NUMBER_BG_HEIGHT + MMIIDLE_DIAL_LINE_WIDTH) - MMIIDLE_DIAL_NUMBER_BG_HEIGHT - MMIIDLE_DIAL_BUTTONS_HEIGHT)
#define MMIIDLE_DIAL_NUMBER_TOP_Y_H (MMI_MAINSCREEN_WIDTH - 3*(MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH) - MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H - MMIIDLE_DIAL_BUTTONS_HEIGHT_H)
#define MMICC_DTMF_NUMBER_TOP_Y (MMI_MAINSCREEN_HEIGHT - 3*(MMIIDLE_DIAL_NUMBER_BG_HEIGHT + MMIIDLE_DIAL_LINE_WIDTH) - MMIIDLE_DIAL_NUMBER_BG_HEIGHT)
#define MMICC_DTMF_NUMBER_TOP_Y_H (MMI_MAINSCREEN_WIDTH - 3*(MMICC_DTMF_NUMBER_BG_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH) - MMICC_DTMF_NUMBER_BG_HEIGHT_H)
#else
#define MMIIDLE_DIAL_NUMBER_TOP_Y (MMI_MAINSCREEN_HEIGHT - 3*(MMIIDLE_DIAL_NUMBER_BG_HEIGHT + MMIIDLE_DIAL_LINE_WIDTH) - MMIIDLE_DIAL_NUMBER_BG_HEIGHT - MMIIDLE_DIAL_BUTTONS_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1)
#define MMIIDLE_DIAL_NUMBER_TOP_Y_H (MMI_MAINSCREEN_WIDTH - 3*(MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH) - MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H - MMIIDLE_DIAL_BUTTONS_HEIGHT_H - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMICC_DTMF_NUMBER_TOP_Y (MMI_MAINSCREEN_HEIGHT - 3*(MMIIDLE_DIAL_NUMBER_BG_HEIGHT + MMIIDLE_DIAL_LINE_WIDTH) - MMIIDLE_DIAL_NUMBER_BG_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1)
#define MMICC_DTMF_NUMBER_TOP_Y_H (MMI_MAINSCREEN_WIDTH - 3*(MMICC_DTMF_NUMBER_BG_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH) - MMICC_DTMF_NUMBER_BG_HEIGHT_H - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#endif
#define MMICC_DTMF_LEFT_H (MMI_MAINSCREEN_HEIGHT - 3*(MMICC_DTMF_NUMBER_BG_WIDTH_H + MMIIDLE_DIAL_LINE_WIDTH))

#undef MMI_DIAL_FORM_LEFT
#undef MMI_DIAL_FORM_TOP
#undef MMI_DIAL_OPTION_BUTTON_FORM_TOP
#undef MMI_DIAL_OPTION_BUTTON_FORM_TOP_H
#undef MMI_DIAL_FORM_LEFT_H
#undef MMI_DIAL_FORM_TOP_H

#define MMI_DIAL_FORM_LEFT 0
#define MMI_DIAL_FORM_TOP (MMIIDLE_DIAL_NUMBER_TOP_Y - MMIIDLE_DIAL_LINE_WIDTH - MMIIDLE_DIAL_EDIT_FORM_HEIGHT)
#define MMI_DIAL_OPTION_BUTTON_FORM_TOP (MMI_MAINSCREEN_HEIGHT - MMIIDLE_DIAL_BUTTONS_HEIGHT)
#define MMI_DIAL_OPTION_BUTTON_FORM_TOP_H (MMI_MAINSCREEN_WIDTH - MMIIDLE_DIAL_OFFSET_Y_H)
#define MMI_DIAL_FORM_LEFT_H (MMI_DIAL_LIST_RIGHT_H + MMIIDLE_DIAL_LINE_WIDTH + 1)

#define DIAL_NUM_MAX_LEN (PHONEBOOK_MAX_DN_NUMBER_LEN << 1)


#if defined(MAINLCD_SIZE_240X320)
#if defined(MMI_PDA_SUPPORT)
#define DIAL_EDIT_FONT_NORMAL  SONG_FONT_20
#define DIAL_EDIT_FONT_SMALL   SONG_FONT_16
#define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_18
#define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_14
#define DIAL_EDIT_FONT_ISTYLE_NORMAL  SONG_FONT_31
#define DIAL_EDIT_FONT_ISTYLE_SMALL   SONG_FONT_27
#else
#define DIAL_EDIT_FONT_NORMAL  SONG_FONT_22
#define DIAL_EDIT_FONT_SMALL   SONG_FONT_18
#define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_18
#define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_14
#endif
#define DIAL_EDIT_FORM_MARGIN        6
#define DIAL_EDIT_FORM_MARGIN_H      4
#elif defined(MAINLCD_SIZE_240X400)
#if defined(MMI_PDA_SUPPORT)
#define DIAL_EDIT_FONT_NORMAL  SONG_FONT_24
#define DIAL_EDIT_FONT_SMALL   SONG_FONT_20
#define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_19
#define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_15
#define DIAL_EDIT_FONT_ISTYLE_NORMAL  SONG_FONT_34
#define DIAL_EDIT_FONT_ISTYLE_SMALL   SONG_FONT_27
#else
#define DIAL_EDIT_FONT_NORMAL  SONG_FONT_24
#define DIAL_EDIT_FONT_SMALL   SONG_FONT_20
#define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_17
#define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_13
#endif
#define DIAL_EDIT_FORM_MARGIN        2
#define DIAL_EDIT_FORM_MARGIN_H      2
#elif defined(MAINLCD_SIZE_320X480)
#ifdef MMI_PDA_SUPPORT
#define DIAL_EDIT_FONT_NORMAL  SONG_FONT_31
#define DIAL_EDIT_FONT_SMALL   SONG_FONT_27
#define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_21
#define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_17
#define DIAL_EDIT_FONT_ISTYLE_NORMAL  SONG_FONT_38
#define DIAL_EDIT_FONT_ISTYLE_SMALL   SONG_FONT_34
#else
#define DIAL_EDIT_FONT_NORMAL  SONG_FONT_31
#define DIAL_EDIT_FONT_SMALL   SONG_FONT_27
#define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_21
#define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_17
#define DIAL_EDIT_FONT_ISTYLE_NORMAL  SONG_FONT_38
#define DIAL_EDIT_FONT_ISTYLE_SMALL   SONG_FONT_34
#endif
#define DIAL_EDIT_FORM_MARGIN        7
#define DIAL_EDIT_FORM_MARGIN_H      2
#else
#	ifdef MMI_PDA_SUPPORT
#		define DIAL_EDIT_FONT_NORMAL  SONG_FONT_22
#		define DIAL_EDIT_FONT_SMALL   SONG_FONT_18
#		define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_18
#		define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_14
#		define DIAL_EDIT_FONT_ISTYLE_NORMAL  SONG_FONT_31
#		define DIAL_EDIT_FONT_ISTYLE_SMALL   SONG_FONT_27
#	else
#		define DIAL_EDIT_FONT_NORMAL  SONG_FONT_22
#		define DIAL_EDIT_FONT_SMALL   SONG_FONT_18
#		define DIAL_EDIT_FONT_NORMAL_H  SONG_FONT_18
#		define DIAL_EDIT_FONT_SMALL_H   SONG_FONT_14
#	endif
#	define DIAL_EDIT_FORM_MARGIN        6
#	define DIAL_EDIT_FORM_MARGIN_H      4
#endif



/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//记录号码匹配搜索结果信息
typedef struct
{
    uint16 cl_match_item_num;
    uint16 pb_match_item_num;
    MMIPB_HANDLE_T handle;
}IDLE_SEARCH_INFO;

#ifdef TOUCH_PANEL_SUPPORT
typedef enum
{
    DIAL_TOOLBAR_TYPE_INIT,
    DIAL_TOOLBAR_SMS,
    DIAL_TOOLBAR_CONTACT,
    DIAL_TOOLBAR_CALLLOG
}DIAL_TOOLBAR_TYPE_E;

#define DIAL_TOOLBAR_AUTOSHRINK_TIMER 2000
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef TOUCH_PANEL_SUPPORT
/*拨号盘上的button在竖屏下分成2个form，第一个显示从0到9和* #这12个button，成3行4列排列，第二个显示3个功能button，成一行排列
横屏下所有button成3行5列排列*/
//竖屏
//'0'~'9' '*' '#' button
LOCAL MMIIDLE_DIAL_BUTTON_E s_mmiidle_dial_num_button_order[DIAL_NUM_BUTTON_ROW_MAX][DIAL_NUM_BUTTON_COLUMN_MAX] =
{
    {MMIIDLE_DIAL_BUTTON_1, MMIIDLE_DIAL_BUTTON_2, MMIIDLE_DIAL_BUTTON_3},
    {MMIIDLE_DIAL_BUTTON_4, MMIIDLE_DIAL_BUTTON_5, MMIIDLE_DIAL_BUTTON_6},
    {MMIIDLE_DIAL_BUTTON_7, MMIIDLE_DIAL_BUTTON_8, MMIIDLE_DIAL_BUTTON_9},
    {MMIIDLE_DIAL_BUTTON_STAR, MMIIDLE_DIAL_BUTTON_0, MMIIDLE_DIAL_BUTTON_HASH},
};

LOCAL MMI_CTRL_ID_T s_mmiidle_dial_num_button_ctrl_order[DIAL_NUM_BUTTON_ROW_MAX][DIAL_NUM_BUTTON_COLUMN_MAX] =
{
    {MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY2_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY3_BUTTON_CTRL_ID},
    {MMIIDLE_DIAL_KEY4_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY5_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY6_BUTTON_CTRL_ID},
    {MMIIDLE_DIAL_KEY7_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY8_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY9_BUTTON_CTRL_ID},
    {MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY0_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEYHASH_BUTTON_CTRL_ID},
}; 

LOCAL GUI_POINT_T s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX + 1][DIAL_NUM_BUTTON_COLUMN_MAX];
LOCAL GUI_POINT_T s_panel_rect_coordinate_h[DIAL_NUM_BUTTON_ROW_MAX + 1][DIAL_NUM_BUTTON_COLUMN_MAX];
#ifdef MMI_PDA_SUPPORT
MMI_HANDLE_T s_mmiidle_pb_applet_handle;
#endif
#ifdef MMI_PDA_SUPPORT//yao.chen
#ifdef MMI_ISTYLE_SUPPORT    
//save call del 
LOCAL MMIIDLE_DIAL_BUTTON_E s_mmiidle_dial_func_button_order_for_istyle[DIAL_FUNC_BUTTON_NUM_MAX] = 
{
    MMIIDLE_DIAL_BUTTON_SAVE,
    MMIIDLE_DIAL_BUTTON_CALL,
    MMIIDLE_DIAL_BUTTON_DEL,
};
LOCAL MMI_CTRL_ID_T s_mmiidle_dial_func_ctrl_order_for_istyle[DIAL_FUNC_BUTTON_NUM_MAX] = 
{
    MMIIDLE_DIAL_SAVE_BUTTON_CTRL_ID,
    MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID,
    MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID,
};

LOCAL const MMIIDLE_DIAL_BUTTON_T s_mmiidle_dial_button_for_istyle[] = 
{
    //key 1
    {
        MMIIDLE_DIAL_BUTTON_1,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY1,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY1_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 2
    {
        MMIIDLE_DIAL_BUTTON_2,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY2,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY2_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 3
    {
        MMIIDLE_DIAL_BUTTON_3,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY3,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY3_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 4
    {
        MMIIDLE_DIAL_BUTTON_4,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY4,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY4_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 5
    {
        MMIIDLE_DIAL_BUTTON_5,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY5,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY5_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 6
    {
        MMIIDLE_DIAL_BUTTON_6,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY6,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY6_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 7
    {
        MMIIDLE_DIAL_BUTTON_7,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY7,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY7_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 8
    {
        MMIIDLE_DIAL_BUTTON_8,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY8,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY8_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 9
    {
        MMIIDLE_DIAL_BUTTON_9,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY9,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY9_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

	//key *
    {
        MMIIDLE_DIAL_BUTTON_STAR,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEYSTAR,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEYSTAR_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key 0
    {
        MMIIDLE_DIAL_BUTTON_0,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY0,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEY0_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //key #
    {
        MMIIDLE_DIAL_BUTTON_HASH,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEYHASH,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_KEYHASH_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //save
    {
        MMIIDLE_DIAL_BUTTON_SAVE,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_SAVE,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_SAVE_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //call
    {
        MMIIDLE_DIAL_BUTTON_CALL,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_DIAL_ON,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_DIAL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //VT call
#ifdef VT_SUPPORT
    {
        MMIIDLE_DIAL_BUTTON_VT,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },
#endif

#ifdef MMI_IP_CALL_SUPPORT
    //IP call
    {
        MMIIDLE_DIAL_BUTTON_IPCALL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },
#endif

#if !(defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT))
    //SMS
    {
        MMIIDLE_DIAL_BUTTON_SMS,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },
#endif

    //delete
    {
        MMIIDLE_DIAL_BUTTON_DEL,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_DELKEY,
        IMAGE_ISTYLE_BUTTON_BLUE_BG_DELKEY_ON,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },
    
    //hide
    {
        MMIIDLE_DIAL_BUTTON_HIDE_PANEL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //show
    {
        MMIIDLE_DIAL_BUTTON_SHOW_PANEL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //hide dtmf
    {
        MMIIDLE_DIAL_BUTTON_HIDE_DTMF,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },
    
    //match num arrow
    {
        MMIIDLE_DIAL_BUTTON_SHOW_MATCH_ARROW,
        IMAGE_ISTYLE_DIAL_MATCH_NUM_ARROW,
        IMAGE_ISTYLE_DIAL_MATCH_NUM_ARROW_P,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_ISTYLE_DIAL_MATCH_NUM_ARROW_GRAY,
    },
};
#endif
#endif

//del ,dial ,video dial, IP dial button 
LOCAL MMIIDLE_DIAL_BUTTON_E s_mmiidle_dial_func_button_order[DIAL_FUNC_BUTTON_NUM_MAX] = 
{
#ifdef VT_SUPPORT
    MMIIDLE_DIAL_BUTTON_VT,
#elif defined(MMI_IP_CALL_SUPPORT)
    MMIIDLE_DIAL_BUTTON_IPCALL,
#else
    MMIIDLE_DIAL_BUTTON_SMS,
#endif
    MMIIDLE_DIAL_BUTTON_CALL,
    MMIIDLE_DIAL_BUTTON_HIDE_PANEL,
};

LOCAL MMI_CTRL_ID_T s_mmiidle_dial_func_ctrl_order[DIAL_FUNC_BUTTON_NUM_MAX] = 
{
#ifdef VT_SUPPORT
    MMIIDLE_DIAL_VIDEO_CALL_BUTTON_CTRL_ID,
#elif defined(MMI_IP_CALL_SUPPORT)
    MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID,
#else
    MMIIDLE_DIAL_SMS_BUTTON_CTRL_ID,
#endif
    MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID,
    MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID,
};
/*
LOCAL MMIIDLE_DIAL_BUTTON_E s_mmiidle_dial_func_button_h_order[DIAL_FUNC_BUTTON_NUM_MAX] = 
{
#if !defined(MMI_PDA_SUPPORT) && defined(TOUCH_PANEL_SUPPORT)
    MMIIDLE_DIAL_BUTTON_TOOLBAR,
#endif
    MMIIDLE_DIAL_BUTTON_DEL,
    MMIIDLE_DIAL_BUTTON_IPCALL,
    MMIIDLE_DIAL_BUTTON_CALL
};

LOCAL MMI_CTRL_ID_T s_mmiidle_dial_func_ctrl_h_order[DIAL_FUNC_BUTTON_NUM_MAX] = 
{
#if !defined(MMI_PDA_SUPPORT) && defined(TOUCH_PANEL_SUPPORT)
    MMIIDLE_DIAL_TOOLBAR_BUTTON_CTRL_ID,   
#endif
    MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID,
    MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID,
    MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID,
    
};
*/
//横屏
#if 0
LOCAL MMIIDLE_DIAL_BUTTON_E s_mmiidle_dial_button_h_order[DIAL_BUTTON_ROW_H_MAX][DIAL_BUTTON_LINE_H_MAX] =
{
    {MMIIDLE_DIAL_BUTTON_1, MMIIDLE_DIAL_BUTTON_2, MMIIDLE_DIAL_BUTTON_3, MMIIDLE_DIAL_BUTTON_STAR, MMIIDLE_DIAL_BUTTON_DEL},
    {MMIIDLE_DIAL_BUTTON_4, MMIIDLE_DIAL_BUTTON_5, MMIIDLE_DIAL_BUTTON_6, MMIIDLE_DIAL_BUTTON_0, MMIIDLE_DIAL_BUTTON_IPCALL},
    {MMIIDLE_DIAL_BUTTON_7, MMIIDLE_DIAL_BUTTON_8, MMIIDLE_DIAL_BUTTON_9, MMIIDLE_DIAL_BUTTON_HASH, MMIIDLE_DIAL_BUTTON_CALL},
};

LOCAL MMI_CTRL_ID_T s_mmiidle_dial_button_ctrl_h_order[DIAL_BUTTON_ROW_H_MAX][DIAL_BUTTON_LINE_H_MAX] =
{
    {MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY2_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY3_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID, MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID},
    {MMIIDLE_DIAL_KEY4_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY5_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY6_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY0_BUTTON_CTRL_ID, MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID},
    {MMIIDLE_DIAL_KEY7_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY8_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEY9_BUTTON_CTRL_ID, MMIIDLE_DIAL_KEYHASH_BUTTON_CTRL_ID, MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID},
};
#endif

LOCAL const MMIIDLE_DIAL_BUTTON_T s_mmiidle_dial_button[] = 
{
    //key 1
    {
        MMIIDLE_DIAL_BUTTON_1,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY1_ICON,
        IMAGE_NUMBER_KEY1_ICON_P,
        IMAGE_NULL,
    },
    
    //key 2
    {
        MMIIDLE_DIAL_BUTTON_2,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY2_ICON,
        IMAGE_NUMBER_KEY2_ICON_P,
        IMAGE_NULL,
    },

    //key 3
    {
        MMIIDLE_DIAL_BUTTON_3,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY3_ICON,
        IMAGE_NUMBER_KEY3_ICON_P,
        IMAGE_NULL,
    },

    //key 4
    {
        MMIIDLE_DIAL_BUTTON_4,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY4_ICON,
        IMAGE_NUMBER_KEY4_ICON_P,
        IMAGE_NULL,
    },

    //key 5
    {
        MMIIDLE_DIAL_BUTTON_5,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY5_ICON,
        IMAGE_NUMBER_KEY5_ICON_P,
        IMAGE_NULL,
    },

    //key 6
    {
        MMIIDLE_DIAL_BUTTON_6,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY6_ICON,
        IMAGE_NUMBER_KEY6_ICON_P,
        IMAGE_NULL,
    },

    //key 7
    {
        MMIIDLE_DIAL_BUTTON_7,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY7_ICON,
        IMAGE_NUMBER_KEY7_ICON_P,
        IMAGE_NULL,
    },

    //key 8
    {
        MMIIDLE_DIAL_BUTTON_8,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY8_ICON,
        IMAGE_NUMBER_KEY8_ICON_P,
        IMAGE_NULL,
    },

    //key 9
    {
        MMIIDLE_DIAL_BUTTON_9,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY9_ICON,
        IMAGE_NUMBER_KEY9_ICON_P,
        IMAGE_NULL,
    },

	//key *
    {
        MMIIDLE_DIAL_BUTTON_STAR,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEYSTAR_ICON,
        IMAGE_NUMBER_KEYSTAR_ICON_P,
        IMAGE_NULL,
    },

    //key 0
    {
        MMIIDLE_DIAL_BUTTON_0,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEY0_ICON,
        IMAGE_NUMBER_KEY0_ICON_P,
        IMAGE_NULL,
    },

    //key #
    {
        MMIIDLE_DIAL_BUTTON_HASH,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG_P,
        IMAGE_NUMBER_KEYHASH_ICON,
        IMAGE_NUMBER_KEYHASH_ICON_P,
        IMAGE_NULL,
    },

    //save
    {
        MMIIDLE_DIAL_BUTTON_SAVE,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

    //call
    {
        MMIIDLE_DIAL_BUTTON_CALL,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_BUTTON_DIAL,
        IMAGE_BUTTON_DIAL_P,
        IMAGE_NULL,
    },

#ifdef VT_SUPPORT
    //VT call
    {
        MMIIDLE_DIAL_BUTTON_VT,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_BUTTON_FUNC_BG_P,
        IMAGE_BUTTON_VIDEO_DIAL,
        IMAGE_BUTTON_VIDEO_DIAL,
        IMAGE_NULL,
    },
#endif

#ifdef MMI_IP_CALL_SUPPORT
    //IP call
    {
        MMIIDLE_DIAL_BUTTON_IPCALL,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_BUTTON_FUNC_BG_P,
        IMAGE_BUTTON_IP_DIAL,
        IMAGE_BUTTON_IP_DIAL_P,
        IMAGE_NULL,
    },
#endif

#if !(defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT))
    //SMS
    {
        MMIIDLE_DIAL_BUTTON_SMS,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_BUTTON_FUNC_BG_P,
        IMAGE_BUTTON_SMS,
        IMAGE_BUTTON_SMS_P,
        IMAGE_NULL,
    },
#endif

    //delete
    {
        MMIIDLE_DIAL_BUTTON_DEL,
        IMAGE_BUTTON_DELKEY,
        IMAGE_BUTTON_DELKEY_P,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },

#if defined(TOUCH_PANEL_SUPPORT) 
    //hide
    {
        MMIIDLE_DIAL_BUTTON_HIDE_PANEL,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_BUTTON_FUNC_BG_P,
        IMAGE_BUTTON_HIDE_KEYBOARD,
        IMAGE_BUTTON_HIDE_KEYBOARD_P,
        IMAGE_NULL,
    },

    //show
    {
        MMIIDLE_DIAL_BUTTON_SHOW_PANEL,
        IMAGE_DIAL_NUMBER_BG,
        IMAGE_BUTTON_FUNC_BG_P,
        IMAGE_BUTTON_SHOW_KEYBOARD,
        IMAGE_BUTTON_SHOW_KEYBOARD_P,
        IMAGE_NULL,
    },

    //hide dtmf
    {
        MMIIDLE_DIAL_BUTTON_HIDE_DTMF,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_BUTTON_HIDE_KEYBOARD_2,
        IMAGE_BUTTON_HIDE_KEYBOARD_P_2,
        IMAGE_NULL,
    },
    
#ifdef MMI_ISTYLE_SUPPORT
    //match num arrow
    {
        MMIIDLE_DIAL_BUTTON_SHOW_MATCH_ARROW,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
        IMAGE_NULL,
    },
#endif
#endif
};
LOCAL BOOLEAN s_mmiidle_is_button_panel_close;
#endif //TOUCH_PANEL_SUPPORT

//dial init input char

LOCAL BOOLEAN   s_mmiidle_is_init_dial;
LOCAL MMI_MESSAGE_ID_E  s_mmiidle_dial_init_key;

LOCAL wchar   *s_mmiidle_dial_init_tele = PNULL;  //初始化的电话号码
LOCAL BOOLEAN  s_mmiidle_is_init_dial_str; //是否初始化电话号码串

#ifdef TOUCH_PANEL_SUPPORT
LOCAL uint8 s_mmiidle_dial_timer_id;

LOCAL BOOLEAN   s_mmiidle_is_tp_del;
#endif
LOCAL IDLE_SEARCH_INFO s_num_search_info;

#ifdef DPHONE_SUPPORT
LOCAL uint8 s_hangup_remind_timer_id = 0;   //催挂音检测定时器
LOCAL uint32 s_hangup_remind_count = 0;     //时间计数
#endif

LOCAL BOOLEAN s_is_need_check_input = TRUE;

#ifdef MMI_MULTI_GREEN_KEY_SINGLE
LOCAL BOOLEAN s_dial_down_flag = FALSE;
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle dial win message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    );
/*****************************************************************************/
// 	Description : callback of pb change
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event);
                   
#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
// 	Description : PdaOpenDialWin
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CreateCtrlOfDialWin(MMI_WIN_ID_T        win_id);
/*****************************************************************************/
//  Description : handle dial list menu window msg
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialListMenuWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );
#endif

/*****************************************************************************/
//  Description : set dial window right softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDialWinRightSoftkey(
                                     wchar          *str_ptr,   //in:
                                     uint16         str_len,    //in:
                                     MMI_TEXT_ID_T  *text_id_ptr//in/out:
                                     );

/*****************************************************************************/
//  Description : set dial window intercept edit control cancel msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetWinInterceptEditMsg(
                                  MMI_CTRL_ID_T     edit_ctrl_id
                                  );

/*****************************************************************************/
//  Description : is lock phone
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLockPhone(
                          MMI_WIN_ID_T      win_id,
                          MMI_MESSAGE_ID_E  msg_id,
                          wchar             *str_ptr,
                          uint16            str_len
                          );

/*****************************************************************************/
//  Description : handle dial win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDialWinBg(
                            MMI_WIN_ID_T    win_id
                            );

/*****************************************************************************/
//  Description : handle dial win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayGrayButton(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id
                             );

/*****************************************************************************/
//  Description : handle dial window ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialOkMsg(
                           MMI_CTRL_ID_T    ctrl_id
                           );

/*****************************************************************************/
//  Description : handle dial menu window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialMenuWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : handle dial menu window msg
//  Global resource dependence : 
//  Author: chunjie liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialGreenMenuWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );
#endif

/*****************************************************************************/
//  Description : set dial menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDialMenuGray(
                           MMI_WIN_ID_T     win_id,
                           MMI_CTRL_ID_T    edit_ctrl_id
                           );

/*****************************************************************************/
//  Description : handle dial window web msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialWebMsg(
                            MMI_WIN_ID_T win_id,
                            MMI_CTRL_ID_T   ctrl_id
                            );

#if 0
/*****************************************************************************/
//  Description : 判断是否时号码快速查询
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPbFastSeach(
                            uint8   *str_ptr,
                            uint16  str_len
                            );
#endif
/*****************************************************************************/
//  Description : handle dial window green up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialGreenUpMsg(
                                MMI_CTRL_ID_T   ctrl_id,
                                MN_DUAL_SYS_E dual_sys
                                );

/*****************************************************************************/
//  Description : handle dial window video tel msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialVideoTelMsg(
                                 MMI_CTRL_ID_T  ctrl_id
                                 );
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
/*****************************************************************************/
//  Description : start vibrator timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartVibratorTimer(
                              MMI_WIN_ID_T  win_id
                              );

/*****************************************************************************/
//  Description : stop vibrator timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopVibratorTimer(void);

/*****************************************************************************/
//  Description : stop tp button vibrator
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTPButtonVibrator(uint8 timer_id, uint32 param);


/*****************************************************************************/
//  Description : handle dial window pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialPenOkMsg(
                              MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     edit_ctrl_id,   //in:edit ctrl
                              MMI_CTRL_ID_T     ctrl_id  //in:button ctrl
                              );

/*****************************************************************************/
//  Description : handle dial window pen long ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialPenLongOkMsg(
                                  MMI_WIN_ID_T      win_id,         //in:
                                  MMI_CTRL_ID_T     edit_ctrl_id,   //in:edit ctrl
                                  MMI_CTRL_ID_T     button_ctrl_id  //in:button ctrl
                                  );

#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : 设置Edit FORM的param
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void SetEditFormParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 隐藏显示拨号盘中的button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void SetNumberButtonsVisible(MMI_WIN_ID_T win_id, BOOLEAN is_visible);

/*****************************************************************************/
//  Description : 创建拨号盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialPanel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 创建拨号盘中的button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialPanelButton(MMI_WIN_ID_T win_id, MMI_HANDLE_T button_handle, MMIIDLE_DIAL_BUTTON_E button_type, const GUI_BOTH_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateEditForm(MMI_WIN_ID_T win_id, const GUI_BOTH_RECT_T *edit_form_rect_ptr);

#if !defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 重新放置拨号盘中的编辑框
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ArrangeEditRect(MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void CreateNumberButtons(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 创建拨号盘中的Dial button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialButtons(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 更新坐标值等。在发生窗口切换时，尤其是创建过dtmf键盘以后，坐标值可能发生了变化，需要重置
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void UpdateDialCoordinate(MMI_WIN_ID_T win_id);
#else
/*****************************************************************************/
//  Description : update win toolbar
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMIIdle_UpdateWinToolbar(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : create toolbar  button form
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void ConstructToolbarButtonFormCtrl(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : create toolbar control
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMIIdle_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void ConstructNumButtonFormCtrl(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 销毁拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void DestroyNumButtonFormCtrl(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 创建拨号盘中的option button form控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void ConstructOptionButtonFormCtrl(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 销毁拨号盘中的option button form控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void DestroyOptionButtonFormCtrl(MMI_WIN_ID_T win_id);


/*****************************************************************************/
// 	Description : set form param
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void SetFormParam(MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//  Description : 处理拨号盘按钮收起展开处理
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void HandleDialButtonPanelCloseOrSpread(MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//  Description : 创建拨号盘中的分割线
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DrawDialPanelLine(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_BOTH_RECT_T *rect_ptr, uint32 img_id);

/*****************************************************************************/
//  Description : 创建拨号盘中的输入框
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void InitDialEdit(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : Is IMEI Input
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsImeiIpnut(MMI_STRING_T *str_ptr);

/*****************************************************************************/
// 	Description : 根据新的号码搜索匹配号码更新显示
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void UpdateSearchResult(MMI_WIN_ID_T win_id, MMI_STRING_T *str_ptr, BOOLEAN is_fresh);
#if 0
/*****************************************************************************/
// 	Description : call back: list获取anim photo显示
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void DrawDialPanelListItemIconData(void *param);
#endif
#if defined(MMI_PDA_SUPPORT)
#ifdef MMI_ISTYLE_SUPPORT    
/*****************************************************************************/
//  Description : handle dial win message for istyle
//  Global resource dependence : 
//  Author: Jassmine
//  modify:chen yao modify for istyle 20110720
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Istyle_HandleDialWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    );
#endif
#endif
/*****************************************************************************/
// 	Description : 插入匹配的通话记录和电话簿信息
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void AppendMatchingInfo(MMI_WIN_ID_T win_id, void * param);
/*****************************************************************************/
// 	Description : 插入匹配的通话记录和电话簿信息
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
//LOCAL void AppendMatchingInfo(void * param);

/*****************************************************************************/
// 	Description : Is PIN/PIN2 Input
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPINInput(MMI_STRING_T *str_ptr);

#ifdef ENG_SUPPORT
/*****************************************************************************/
// 	Description : Is Eng Input
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEngInput(MMI_STRING_T *str_ptr);
#endif

/*****************************************************************************/
// 	Description : Is SIM ADN Input
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSIMADNInput(MMI_STRING_T *str_ptr);

/*****************************************************************************/
// 	Description : Check SIM ADN Input
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckSIMADNInput(MN_DUAL_SYS_E dual_sys, uint16 record_id);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : handle message of sim list window 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectADNCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif
#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : handle dial menu window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E Istyle_HandleIdleMatchlistWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );
#endif
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
//dial menu
WINDOW_TABLE( MMIIDLE_DIAL_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDialMenuWinMsg),
    WIN_ID(MMIIDLE_DIAL_MENU_WIN_ID),
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/28
   // WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_MENU_CTRL(MENU_PHONENUM_DIALUP_OPERATE,MMIIDLE_DIAL_POPMENU_CTRL_ID),
#else    
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_PHONENUM_DIALUP_OPERATE,MMIIDLE_DIAL_POPMENU_CTRL_ID),
#endif
#ifdef MMI_DIAL_MINI_SUPPORT
    WIN_TITLE(STXT_OPTION),
#endif  
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#ifdef BT_DIALER_SUPPORT
WINDOW_TABLE( MMIIDLE_DIAL_GREEN_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDialGreenMenuWinMsg),
    WIN_ID(MMIIDLE_DIAL_GREEN_MENU_WIN_ID),   
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_PHONENUM_GREEN_DIALUP_OPERATE,MMIIDLE_DIAL_GREEN_POPMENU_CTRL_ID),
#ifdef MMI_DIAL_MINI_SUPPORT
    WIN_TITLE(STXT_OPTION),
#endif  
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

#if defined(TOUCH_PANEL_SUPPORT)
WINDOW_TABLE( MMIIDLE_DIAL_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDialListMenuWinMsg),
    WIN_ID(MMIIDLE_DIAL_LIST_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT), 
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIIDLE_DIAL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDialWinMsg),    
    WIN_ID( MMIIDLE_DIAL_WIN_ID ),
    WIN_STYLE(WS_NO_DEFAULT_STYLE),     // 特殊处理，为了使iphone风格下，不计算title区域
//    WIN_STATUSBAR,                      // PDA默认都有statusbar，因为用了WS_NO_DEFAULT_STYLE风格，所以这里显示加上状态栏
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#ifdef MMI_ISTYLE_SUPPORT
WINDOW_TABLE( MMIIDLE_DIAL_MATCH_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) Istyle_HandleIdleMatchlistWinMsg ),    
    WIN_ID( MMIIDLE_DIAL_MATCH_LIST_WIN_ID ),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_COMMON_CONTACT),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIIDLE_DIAL_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 
#endif
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc 此窗无效
PUBLIC BOOLEAN MMIAPICC_IsDtmfPanel(MMI_WIN_ID_T win_id)
{
    return FALSE;
}
#endif
/*****************************************************************************/
//  Description : open dial window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_EditDialWin(
                                MMI_MESSAGE_ID_E    msg_id,     //in:
                                BOOLEAN             is_dial_key,     //in:
                                char                   *init_tele, 
                                BOOLEAN           is_dial_str
                                )
{
    MMI_WIN_ID_T                win_id = MMIIDLE_DIAL_WIN_ID;
#if !defined(TOUCH_PANEL_SUPPORT)
    MMI_WINDOW_CREATE_T         win_create = {0};
    MMI_CONTROL_CREATE_T        softkey_ctrl = {0};
    GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
#if defined(MAINLCD_SIZE_128X64)
#else
    GUI_BOTH_RECT_T client_rect = {0};   
#endif
#endif

    MMK_PBRegisterEvent(MMIIDLE_HandlePBEventFunc);
    
	if (is_dial_str)
	{
            s_mmiidle_is_init_dial_str = TRUE;
            if (PNULL != init_tele)
            {
                int nLen = SCI_STRLEN(init_tele)+1;
                if ( PNULL != s_mmiidle_dial_init_tele )
                {
                    SCI_FREE(s_mmiidle_dial_init_tele);
                }
                s_mmiidle_dial_init_tele = SCI_ALLOC_APP(nLen*2);
                if ( PNULL != s_mmiidle_dial_init_tele )
                {
                    SCI_MEMSET(s_mmiidle_dial_init_tele, 0, nLen*2);
                    MMIAPICOM_StrToWstr((uint8*)init_tele, s_mmiidle_dial_init_tele);
                }
            }
	}

        if (is_dial_key)
        {
            if (!MMK_IsOpenWin(win_id))
            {
                s_mmiidle_is_init_dial = TRUE;
                s_mmiidle_dial_init_key = msg_id;
            }
            else
            {
                if ((msg_id >= MSG_APP_1) && ( msg_id <= MSG_APP_HASH))
                {
                    MMK_PostMsg(win_id, msg_id, PNULL,0);
                }
            }
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
            MMIAPISET_StopAppRing();
#endif
        }
        
#if defined(MMI_PDA_SUPPORT)
            MMIAPIPB_OpenMainWin(
                        MMIPB_DIAL_APPLICATION,//
                        MMIPB_LIST_NORMAL_MAIN, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        0,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        PNULL
                        );
#elif defined(TOUCH_PANEL_SUPPORT)//TK临时解决方案
    if (!MMK_IsOpenWin(win_id))
    {
        MMK_CreateWin((uint32*)MMIIDLE_DIAL_WIN_TAB, PNULL);
    }
    else
    {
        MMK_WinGrabFocus(win_id);
    }
#else
    if (!MMK_IsOpenWin(win_id))
    {
        //create window
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id        = win_id;
        win_create.func          = HandleDialWinMsg;
        win_create.win_priority  = WIN_ONE_LEVEL;
#if !defined(MAINLCD_SIZE_128X64) 
        win_create.window_style |= WS_HAS_STATUSBAR;
#endif
        MMK_CreateWindow(&win_create);

        InitDialEdit(win_id);

#if !defined(MAINLCD_SIZE_128X64)
        client_rect = MMITHEME_GetWinClientBothRect(win_id);    
        
        //set listbox
        {
            MMI_CONTROL_CREATE_T        list_ctrl = {0};        
            GUILIST_INIT_DATA_T list_init = {0};

            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = client_rect.v_rect.right;
            list_init.both_rect.v_rect.top = client_rect.v_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT + MMIIDLE_DIAL_LINE_WIDTH;
            list_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
            
            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = client_rect.h_rect.right;
            list_init.both_rect.h_rect.top = client_rect.h_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H + MMIIDLE_DIAL_LINE_WIDTH;
            list_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
            list_init.type = GUILIST_TEXTLIST_E;

            GUILIST_CreateListBox(win_id, 0, MMIIDLE_DIAL_LISTBOX_CTRL_ID, &list_init);

            GUILIST_SetListState(MMIIDLE_DIAL_LISTBOX_CTRL_ID, GUILIST_STATE_NEED_PRGBAR, FALSE);

            //init list
            {
                MMI_STRING_T blank_str = {0};
                wchar blank_wchar= ' ';

                GUILIST_ITEM_DATA_T item_data = {0};
                GUILIST_ITEM_T item_t = {0};

                GUILIST_SetMaxItem(list_ctrl.ctrl_id, 1, TRUE );

                blank_str.wstr_len = 1;
                blank_str.wstr_ptr = &blank_wchar;
                item_data.item_content[0].item_data.text_buffer= blank_str;
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
                
                GUILIST_AppendItem(list_ctrl.ctrl_id, &item_t);
            }
            
        }
#endif
 
	if (is_dial_str)
	{
	       s_mmiidle_is_init_dial_str = TRUE;
	       if (PNULL != init_tele)
	      	{
      		       int nLen = SCI_STRLEN(init_tele)+1;
		       s_mmiidle_dial_init_tele = malloc(nLen*2);
			if (s_mmiidle_dial_init_tele != PNULL)
			{
				memset(s_mmiidle_dial_init_tele, 0, nLen*2);
				MMIAPICOM_StrToWstr((uint8*)init_tele, s_mmiidle_dial_init_tele);
			}
	      	}
	} 

        //init softkey
        softkey_init.leftsoft_id  = STXT_OPTION;
        softkey_init.midsoft_id   = TXT_NULL;
        softkey_init.rightsoft_id = TXT_DELETE;

        //create softkey control
        softkey_ctrl.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
        softkey_ctrl.guid              = SPRD_GUI_SOFTKEY_ID;
        softkey_ctrl.init_data_ptr     = &softkey_init;
        softkey_ctrl.parent_win_handle = win_id;
        MMK_CreateControl(&softkey_ctrl);
        MMK_SetWinSoftkeyCtrlId(win_id,softkey_ctrl.ctrl_id);
       //init keypad button
    }
    else
    {
        MMK_WinGrabFocus(win_id);
    }
#endif
}

/*****************************************************************************/
// 	Description : callback of pb change
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event)
{
#if defined(MMI_PDA_SUPPORT)
    MMI_WIN_ID_T win_id = MMK_GetFocusChildWinId();

    win_id = MMK_GetWinId(win_id);
#endif
    if((MMIEVENT_PB_ADD == event)
       || (MMIEVENT_PB_DELETE == event)
       || (MMIEVENT_PB_UPDATE == event))
    {
#if defined(MMI_PDA_SUPPORT)
        if(MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID) || (MMIIDLE_DIAL_WIN_ID == win_id))
        {
            MMK_PostMsg(MMK_GetWinHandle(s_mmiidle_pb_applet_handle, MMIIDLE_DIAL_WIN_ID), MSG_IDLE_DIAL_PB_UPDATE_IND, PNULL,0);
        }
#else
        if(MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID))
        {
            MMK_PostMsg(MMIIDLE_DIAL_WIN_ID, MSG_IDLE_DIAL_PB_UPDATE_IND, PNULL,0);
        }
#endif
    }
}

#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : handle dial list menu window msg
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialListMenuWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    uint8               phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
    uint16              phone_num_len = 0; 
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIDLE_DIAL_LIST_MENU_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    uint16   node_id     =   0;
    MMIPB_BCD_NUMBER_T  pb_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {0};/*lint !e64*/ 
    uint16 cur_list_index = 0;    
    MMI_STRING_T num_str = {0}; 
    wchar num_wchar[MMIPB_MAX_STRING_LEN] = {0};
    MMI_STRING_T title_str = {0}; 
    MMI_WIN_ID_T dial_win_id = (MMI_WIN_ID_T)MMK_GetWinAddDataPtr(win_id);
    uint32         group =0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
	BOOLEAN			is_num_valid = TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        cur_list_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(dial_win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID));
        title_str.wstr_ptr = num_wchar;
        if(cur_list_index < s_num_search_info.cl_match_item_num)
        {
            CC_CALL_MODE_E call_mode = CC_CALL_NORMAL;
            BOOLEAN is_emergency_call = FALSE;
            
            //call log匹配记录
            MMIAPICL_GetSearchCallType(&call_mode, &is_emergency_call, cur_list_index);

            if (is_emergency_call)
            {
                //name
                MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &title_str);
            }
            else
            {
                //name
                if (!MMIAPICL_GetSearchEntryName(&title_str, cur_list_index, TRUE))
                {
                    //num
                    MMIAPICL_GetSearchEntryNumber(&title_str, cur_list_index);
                }
            }
        }
        else
        {      
            //pb匹配记录
            uint32 pb_index = cur_list_index - s_num_search_info.cl_match_item_num;
            //name
            if(MMIAPIPB_GetSearchEntry(s_num_search_info.handle , &title_str, MMIPB_MAX_STRING_LEN, &pb_num,&group,pb_index) )
            {
                if(0 == title_str.wstr_len)
                {
                    //nanme is null, show number
                    MMIAPICOM_BcdToWchar(title_str.wstr_ptr, &title_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);
                }
            }
        }
        GUIMENU_SetMenuTitle(&title_str,ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //get list num string
        cur_list_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(dial_win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID));
        num_str.wstr_ptr = num_wchar;
        if(s_num_search_info.cl_match_item_num > cur_list_index)
        {
            MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);

            dual_sys = MMIAPICL_GetSearchEntrySim(cur_list_index);
        }
        else
        {
            MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, &group,(cur_list_index - s_num_search_info.cl_match_item_num));
            MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);
            //MMIAPIPB_GetSearchEntryNumber(&num_str, &num_type, MMIPB_MAX_STRING_LEN,(cur_list_index - s_num_search_info.cl_match_item_num));            

            dual_sys = MN_DUAL_SYS_MAX;
        }
        phone_num_len = num_str.wstr_len;
        MMI_WSTRNTOSTR((uint8 *)phone_num_str,MMIIDLE_DIAL_MAX_LEN,
            num_str.wstr_ptr,num_str.wstr_len,
            phone_num_len);
        
//        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        menu_id = (uint32)((MMI_MODULE_COMMON << 16) + node_id);
        switch (menu_id)
        {
        case ID_DIAL_ADD_PB:
        case ID_DIAL_ADD_EXISTNUM:

		    //+bug132483
			//号码有效才可以添加到contacts中
			is_num_valid = MMIAPICOM_IsValidNumberString((char*)phone_num_str, phone_num_len);
			SCI_TRACE_LOW("[mmiidle] HandleDialListMenuWinMsg is_num_valid:%d", is_num_valid);
			SCI_TRACE_BUF("[mmiidle] HandleDialListMenuWinMsg phone_num_str:%x", phone_num_str, phone_num_len);
			if (FALSE == is_num_valid)
			{
				//提示无效号码
				MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
				return MMI_RESULT_TRUE; 
			}
			//-bug132483

            MMIAPICOM_GenPartyNumber(phone_num_str,phone_num_len,&party_num);            
            pb_num.number_len = (uint8)party_num.num_len;
            pb_num.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type,MN_NUM_PLAN_UNKNOW);
            MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN, 
                party_num.bcd_num,party_num.num_len,
                party_num.num_len);

            if (ID_DIAL_ADD_PB == menu_id)
            {
                MMIAPIPB_AddContactWin(&pb_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                //MMIAPIPB_OpenAddMenu(&pb_num,PB_GROUP_ALL);
            }
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
            else
            {
                //if ((0 < pb_num.number_len)||(MN_NUM_TYPE_INTERNATIONAL == (pb_num.npi_ton >> 4)))
				if(MMIAPIPB_IsValidPhoneNumber(pb_num.npi_ton, pb_num.number_len))
                {
                   // MMIAPIPB_OpenEntryListForCallLog(&pb_num);
                    MMIAPIPB_AddToExistContact(&pb_num, MMIPB_ADD_TYPE_NUMBER, PNULL);
                }
                else
                {
                    //prompt no phone number
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
                }
            }
#endif            
            break;

        case ID_DIAL_SEND_SMS:
            MMIAPISMS_WriteNewMessage(dual_sys,
                PNULL,
                phone_num_str,
                (uint8)phone_num_len);
            break;

        case ID_DIAL_SEND_MMS:
            MMIAPIMMS_AnswerMMS(dual_sys,PNULL,&num_str);
            break;
            
        case ID_DIAL_CALL_AUDIO:
            MMIAPICC_MakeCall(dual_sys,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_NORMAL_CALL,
                PNULL);
            break;

#ifdef VT_SUPPORT
        case ID_DIAL_CALL_VIDEO:
            MMIAPICC_MakeCall(dual_sys,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_VIDEO_CALL,
                PNULL);
            break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
        case ID_DIAL_IPCALL:
            MMIAPICC_MakeCall(dual_sys,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_IP_CALL,
                PNULL);
            break;
#endif

        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        GUI_INVALID_PARAM(param);/*lint !e522*/
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : OpenPdaDialWin
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void CreateCtrlOfDialWin(MMI_WIN_ID_T        win_id)
{
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);   

#if defined(MMI_PDA_SUPPORT)
    s_mmiidle_is_button_panel_close = FALSE;
#else
    s_mmiidle_is_button_panel_close = TRUE;
#endif
    //set listbox
    {
        MMI_CONTROL_CREATE_T        list_ctrl = {0};        
        GUILIST_INIT_DATA_T list_init = {0};
        
        list_init.both_rect.v_rect.left = 0;
        list_init.both_rect.v_rect.right = client_rect.v_rect.right;
        list_init.both_rect.v_rect.top = client_rect.v_rect.top;
        
        if(s_mmiidle_is_button_panel_close)
        {
#if defined(MMI_PDA_SUPPORT)
            list_init.both_rect.v_rect.bottom = MMI_DIAL_OPTION_BUTTON_FORM_TOP - 1;
#else
            list_init.both_rect.v_rect.bottom = MMI_DIAL_OPTION_BUTTON_FORM_TOP - MMIIDLE_DIAL_EDIT_FORM_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1 - 1;
#endif
        }
        else
        {
            list_init.both_rect.v_rect.bottom = MMI_DIAL_FORM_TOP - 1;
        }

        list_init.both_rect.h_rect.left = 0;
        list_init.both_rect.h_rect.right = MMI_DIAL_LIST_RIGHT_H;
        list_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;//xuetao modify client_rect.h_rect.bottom - 1;
        list_init.both_rect.h_rect.top = client_rect.h_rect.top;
        
        list_init.type = GUILIST_TEXTLIST_E;
        GUILIST_CreateListBox(win_id, 0, MMIIDLE_DIAL_LISTBOX_CTRL_ID, &list_init);

        //init list
        {
            MMI_STRING_T blank_str = {0};
            wchar blank_wchar= ' ';

            GUILIST_ITEM_DATA_T item_data = {0};
            GUILIST_ITEM_T item_t = {0};

            GUILIST_SetMaxItem(MMK_GetCtrlHandleByWin(win_id,list_ctrl.ctrl_id), 1, TRUE );

            blank_str.wstr_len = 1;
            blank_str.wstr_ptr = &blank_wchar;
            item_data.item_content[0].item_data.text_buffer= blank_str;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
            
            GUILIST_AppendItem(MMK_GetCtrlHandleByWin(win_id,list_ctrl.ctrl_id), &item_t);
        }
    }    
    CreateDialPanel(win_id);
}

/*****************************************************************************/
// 	Description : MMICL_CreateDialWin
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CreateDialWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_PBRegisterEvent(MMIIDLE_HandlePBEventFunc);
#if defined(MMI_PDA_SUPPORT)
    s_mmiidle_pb_applet_handle = MMK_GetAppletHandleByWin(parent_win_id);
#endif
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIIDLE_DIAL_WIN_TAB, (ADD_DATA)parent_win_id);
}
#endif

/*****************************************************************************/
//  Description : open dial window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenDialWin(
                                MMI_MESSAGE_ID_E    msg_id,     //in:
                                BOOLEAN             is_dial_key //in:
                                )
{
    MMIAPIIDLE_EditDialWin(msg_id, is_dial_key, PNULL, FALSE);
}

/*****************************************************************************/
//  Description : open dial window without any parm
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_EnterDialWin(void)
{
#ifdef MMI_CALL_MINIMIZE_SUPPORT  
    if(!MMIAPICC_ActiveCCApplet())
#endif
    {
        MMIAPIIDLE_OpenDialWin(0,FALSE);
    }
}

#ifdef DPHONE_SUPPORT
static uint8 s_speeddial_timer = 0;//

/*****************************************************************************/
//  Description : 延时拨号timer处理
//  Global resource dependence : 
//  
//  Note:针对需要处理的键过滤，需要根据设置决定延时拨号TIMER
/*****************************************************************************/
LOCAL void HandleDialMsgForDelyDial(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	uint32 speeddial_delay = MMIAPISET_GetSpeedDialDelay();

	if (speeddial_delay == 0)
	{
            speeddial_delay = 0x0FFFFFFF;
	}

	if (s_speeddial_timer == 0 && ( speeddial_delay > 0))
	{
		s_speeddial_timer = MMK_CreateWinTimer(win_id, speeddial_delay, FALSE);
		MMK_StopTimer(s_speeddial_timer);
	}
	
    switch(msg_id)
	{
//	case MSG_OPEN_WINDOW:
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_STAR:
    case MSG_APP_0:
    case MSG_APP_HASH:
    case MSG_APP_CANCEL:
    case MSG_KEYLONG_0:
    case MSG_KEYUP_1:
    case MSG_KEYUP_2:
    case MSG_KEYUP_3:
    case MSG_KEYUP_4:
    case MSG_KEYUP_5:
    case MSG_KEYUP_6:
    case MSG_KEYUP_7:
    case MSG_KEYUP_8:
    case MSG_KEYUP_9:
    case MSG_KEYUP_STAR:
    case MSG_KEYUP_0:
    case MSG_KEYUP_HASH:
    case MSG_KEYUP_HEADSET_BUTTON:
    if ( MMK_IsTimerActive(s_speeddial_timer))
    {
        MMK_StopTimer(s_speeddial_timer);
    }
    //判断当前是否有效号码或是否已经在拨号中
    MMK_StartTimer(s_speeddial_timer, speeddial_delay, FALSE);		
    break;
		
    case MSG_CTL_CANCEL:
    case MSG_KEYUP_CANCEL:
    case MSG_CTL_OK:
    case MSG_KEYUP_GREEN:
    case MSG_APP_GREEN:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYLONG_1:
    case MSG_KEYLONG_2:
    case MSG_KEYLONG_3:
    case MSG_KEYLONG_4:
    case MSG_KEYLONG_5:
    case MSG_KEYLONG_6:
    case MSG_KEYLONG_7:
    case MSG_KEYLONG_8:
    case MSG_KEYLONG_9:
    case MSG_KEYLONG_CANCEL:
    case MSG_LOSE_FOCUS:
	if (MMK_IsTimerActive(s_speeddial_timer))
	{
            MMK_StopTimer(s_speeddial_timer);
	}
	break;

    case MSG_TIMER:
	if( (*(uint8*)param == s_speeddial_timer) && (FALSE == MMIAPICC_IsInState(CC_IN_CALL_STATE)) )
	{
		MMK_PostMsg(win_id, MSG_KEYUP_GREEN, PNULL, PNULL);
	}
	break;
    }
}
#endif
#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : InsertOptMenuNodeByLabel
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                                    MMI_CTRL_ID_T      ctrl_id,        //控件id
                                    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                                    uint16             node_id,        //节点编号
                                    uint16             parent_node_id, //父节点编号
                                    uint16             index           //位置
                                    )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  
#endif
/*****************************************************************************/
//  Description : handle dial win message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
#if defined(MMI_PDA_SUPPORT)
#ifdef MMI_ISTYLE_SUPPORT    
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle()) 
	{ 
		return Istyle_HandleDialWinMsg(win_id, msg_id,  param);
	} 
        else
#endif
#endif
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_STRING_T edit_str = {0};
    MMI_CTRL_ID_T edit_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID);
    MMI_CTRL_ID_T list_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID);
#if defined(TOUCH_PANEL_SUPPORT)
#if defined(MMI_PDA_SUPPORT) 
    GUITAB_ITEM_T  *tab_item_ptr = GUITAB_GetCurItem(MMK_GetCtrlHandleByWin(win_id,MMIPB_SEARCH_LIST_TAB_CTRL_ID));
#endif
    MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);
#endif
    LOCAL wchar temp_wstr[MMIIDLE_DIAL_MAX_LEN + 1] = {0};
    LOCAL wchar temp_wstr_len = 0;

#ifdef DPHONE_SUPPORT //自动拨号处理
    HandleDialMsgForDelyDial(win_id, msg_id, param);
    if ( MSG_KEYUP_HOOK == msg_id)
    {
        MMIDEFAULT_SetHookStates(TRUE);
        MMIDEFAULT_SetHandFreeStates(FALSE);
        if (!MMIDEFAULT_GetHandFreeStates())
        {
        }
    }
    else if (MSG_KEYDOWN_HANDSFREE == msg_id )//hand free down       
    {
    	if (MMIDEFAULT_GetHandFreeStates() && !MMIDEFAULT_GetHookStates())
    	{
    		 MMK_CloseWin(win_id);
    	}
    }
#endif

    msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);

    SCI_TRACE_LOW("[MMIIDLE] HandleDialWinMsg msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined(TOUCH_PANEL_SUPPORT)
        CreateCtrlOfDialWin(win_id);
        edit_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID);
        list_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID);  
#endif
        MMK_SetAtvCtrl(win_id,edit_ctrl_id);
        GUIEDIT_SetSoftkey(edit_ctrl_id,0,0,STXT_OPTION,STXT_OPTION,NULL); 
#ifdef DPHONE_SUPPORT //added by taiping.lai 2010/12/28
        GUIEDIT_SetSoftkey(edit_ctrl_id,2,1,STXT_RETURN,TXT_DELETE,NULL); 
#else
        GUIEDIT_SetSoftkey(edit_ctrl_id,2,1,STXT_RETURN,TXT_DELETE,SetDialWinRightSoftkey);
        
        SetWinInterceptEditMsg(edit_ctrl_id);
#endif
        IGUICTRL_SetCircularHandleUpDown(MMK_GetCtrlPtr(edit_ctrl_id), FALSE);
 
        GUILIST_SetBgImage(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID),IMAGE_COMMON_BG,FALSE);

        if (s_mmiidle_is_init_dial_str)
        {
          	GUIEDIT_SetString(edit_ctrl_id, s_mmiidle_dial_init_tele,  MMIAPICOM_Wstrlen(s_mmiidle_dial_init_tele));
        }

        //set init value
        if(s_mmiidle_is_init_dial)
        {
            s_mmiidle_is_init_dial = FALSE;
            MMK_SendMsg(edit_ctrl_id, s_mmiidle_dial_init_key, PNULL);

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
#ifdef TOUCH_PANEL_SUPPORT
            MMIAPIIDLE_PlayDialTone(s_mmiidle_dial_init_key, FALSE);
#else
            MMIAPIIDLE_PlayDialTone(s_mmiidle_dial_init_key, MMIAPICC_IsInState(CC_IN_CALL_STATE));
#endif
#endif
        }

        GUIEDIT_GetString(edit_ctrl_id, &edit_str);
#if defined(MMI_PDA_SUPPORT) 
	if(tab_item_ptr != PNULL && win_id == tab_item_ptr->child_win_handle)
#endif
	{
            UpdateSearchResult(win_id, &edit_str, FALSE);
	}
	
#ifdef DPHONE_SUPPORT
        s_hangup_remind_count = 0;
        s_hangup_remind_timer_id = MMK_CreateTimer(MMIIDLE_HANGUP_CHK_INTERVAL, TRUE);        
#endif

        s_is_need_check_input = TRUE;

        SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
        temp_wstr_len = MIN(MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_len);
        MMI_WSTRNCPY(temp_wstr, MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_ptr, edit_str.wstr_len, temp_wstr_len);
        break;

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
#ifdef TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T tp_point = (((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point);

            if (MMIAPIIDLE_IsPressInDialPanel(tp_point))
            {
                MMIAPISET_StopAppRing();
            }
        }
        break;
#endif
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_FULL_PAINT:

#ifdef MONOCHROME_LCD_SUPPORT //added by taiping.lai 2010/12/28
#else
        DisplayDialWinBg(win_id);
#endif 
#if defined(TOUCH_PANEL_SUPPORT)
//        SetEditFormParam(win_id);
#endif
#ifndef SUBLCD_SIZE_NONE
         //刷新小屏的显示,和主屏同步,主要是彩信
        MMISUB_UpdateDisplaySubLcd();
#endif
#ifdef BT_DIALER_SUPPORT
        if(MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            GUIEDIT_SetSoftkey(edit_ctrl_id,0,0,TXT_NULL,TXT_NULL,NULL); 
        }
        else
        {
            GUIEDIT_SetSoftkey(edit_ctrl_id,0,0,STXT_OPTION,STXT_OPTION,NULL); 
        }
#endif
        break;

    case MSG_BACKLIGHT_TURN_ON:           
        MMK_SendMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT, PNULL);
        break;

   case MSG_LCD_SWITCH:
#if defined(TOUCH_PANEL_SUPPORT)
        if(!MMITHEME_IsMainScreenLandscape())
        {
            s_mmiidle_is_button_panel_close = TRUE;
            HandleDialButtonPanelCloseOrSpread(win_id);
        }
        else
        {
            SetNumberButtonsVisible(win_id, TRUE);
        }
        SetEditFormParam(win_id);
        GUIFORM_ForceLayout(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID));
#endif
        MMK_WinInactiveCtrl(win_id, FALSE);
        MMK_SetAtvCtrl(win_id, edit_ctrl_id);
        break;

    case MSG_LOSE_FOCUS:
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
        break;

    case MSG_GET_FOCUS:
        GUIEDIT_GetString(edit_ctrl_id, &edit_str);
        UpdateSearchResult(win_id, &edit_str, TRUE);
        MMIAPISMS_CheckSMSState(FALSE);
#ifdef BT_DIALER_SUPPORT
        if(MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            GUIEDIT_SetSoftkey(edit_ctrl_id,0,0,TXT_NULL,TXT_NULL,NULL); 
        }
        else
        {
            GUIEDIT_SetSoftkey(edit_ctrl_id,0,0,STXT_OPTION,STXT_OPTION,NULL); 
        }
#endif
        MMK_SetAtvCtrl(win_id,edit_ctrl_id); 
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            BOOLEAN is_need_check = FALSE;
            MMI_STRING_T temp_str_info = {0};
            
            GUIEDIT_GetString(edit_ctrl_id, &edit_str);
            
            temp_str_info.wstr_ptr = (wchar *)temp_wstr;
            temp_str_info.wstr_len = temp_wstr_len;

            if (temp_str_info.wstr_len > 0 && edit_str.wstr_len > 0 && (edit_str.wstr_len == temp_str_info.wstr_len + 1)
#if !defined(TOUCH_PANEL_SUPPORT) // in touch panel support, dial win and DTMF win is not the same win
                && !MMIAPICC_IsInState(CC_IN_CALL_STATE)    //in call state do not check ADN 
#endif
            )
            {
                if ('#' == edit_str.wstr_ptr[edit_str.wstr_len - 1]
                    && 0 == memcmp(temp_str_info.wstr_ptr, edit_str.wstr_ptr, temp_str_info.wstr_len * sizeof(wchar)))//只有#在最后输入才做匹配
                {
                    is_need_check = TRUE;
                }
            }

            if (!s_is_need_check_input)
            {
                s_is_need_check_input = TRUE;

                is_need_check = FALSE;
            }

            SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
            temp_wstr_len = MIN(MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_len);
            MMI_WSTRNCPY(temp_wstr, MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_ptr, edit_str.wstr_len, temp_wstr_len);

#ifdef MMI_REFACTOR_SAMPLE            
            {
            uint8 temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};

            MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
                                edit_str.wstr_ptr, edit_str.wstr_len,
                                MIN(edit_str.wstr_len, MMIIDLE_DIAL_MAX_LEN));
            if (strncmp((char*)temp_str,"*#9527#",7)==0)
                {
                    extern void MMIAPIAPPSAMPLE_Enter(void);
                    MMIAPIAPPSAMPLE_Enter();
#if defined(TOUCH_PANEL_SUPPORT)
                    MMK_CloseParentWin(info_ptr);
#endif
                    MMK_CloseWin(win_id);                    
                    return;
                }        
            }    
#endif

#if defined(DPHONE_SUPPORT)//@ alex added in 2010.12.6
            if (7 < edit_str.wstr_len)
            {
                GUIEDIT_SetFont(edit_ctrl_id, CAF_FONT_14);
            }
#endif
            if (is_need_check && IsImeiIpnut(&edit_str))
            {
                MMIAPIPHONE_OpenIMEIDisplayWin();
#if defined(TOUCH_PANEL_SUPPORT)
                MMK_CloseParentWin(info_ptr);
#endif
                MMK_CloseWin(win_id);
            }
            else if (is_need_check && IsSIMADNInput(&edit_str))
            {
                s_is_need_check_input = FALSE;
            }
            else if (is_need_check && IsPINInput(&edit_str))
            {
#if defined(TOUCH_PANEL_SUPPORT)
                MMK_CloseParentWin(info_ptr);
#endif

                MMK_CloseWin(win_id);
            }
#ifdef ENG_SUPPORT
            else if (is_need_check && IsEngInput(&edit_str))
            {
#if defined(TOUCH_PANEL_SUPPORT)
                MMK_CloseParentWin(info_ptr);
#endif

                MMK_CloseWin(win_id);
            }
#endif
            else if (MMK_IsFocusWin(win_id))
            {
#if !defined(TOUCH_PANEL_SUPPORT)
                DisplayGrayButton(win_id,edit_ctrl_id);
#endif
            //cr209582 update softkey
            GUIWIN_UpdateSoftkey(win_id);
            }
        }
        if (MMK_IsFocusWin(win_id))
        {
            UpdateSearchResult(win_id, &edit_str, TRUE);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_MENU:
        HandleDialOkMsg(edit_ctrl_id);
        break;

    case MSG_CTL_MIDSK:
        HandleDialWebMsg(win_id, edit_ctrl_id);
        break;

#if defined(TOUCH_PANEL_SUPPORT)
    case MSG_APP_CANCEL:
#endif
    case MSG_CTL_CANCEL:
#if defined(TOUCH_PANEL_SUPPORT)
        MMK_CloseParentWin(info_ptr);
#endif
        MMK_CloseWin(win_id);
        break;

#ifdef MMI_MULTI_GREEN_KEY_SINGLE
	case MSG_KEYDOWN_GREEN:
		{
			s_dial_down_flag = TRUE;
			break;
		}
    case MSG_KEYUP_GREEN:
		{
			if (s_dial_down_flag)
			{
				HandleDialGreenUpMsg(edit_ctrl_id,MN_DUAL_SYS_MAX);
				s_dial_down_flag = FALSE;
			}
			break;
		}
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleDialGreenUpMsg(edit_ctrl_id,MN_DUAL_SYS_MAX);
        break;        
#else /* MMI_MULTI_GREEN_KEY_SINGLE */
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleDialGreenUpMsg(edit_ctrl_id,MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_MAX));
        break;
#endif /* MMI_MULTI_GREEN_KEY_SINGLE */

#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
        HandleDialVideoTelMsg(edit_ctrl_id);
        break;
#endif

    case MSG_APP_VIDEO_TEL:
        HandleDialVideoTelMsg(edit_ctrl_id);
        break;

#ifdef CMCC_UI_STYLE
    case MSG_KEYLONG_STAR:
        if (MMK_IsActiveCtrl(edit_ctrl_id) || MMK_SetAtvCtrl(win_id,edit_ctrl_id))
        {
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_KEYDOWN_PLUS,PNULL);
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_KEYUP_PLUS,PNULL);
        }
        break;
#endif

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    case MSG_KEYLONG_1:
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);
        if ((0 == edit_str.wstr_len) ||
            (1 == edit_str.wstr_len))
        {
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
            uint32 dual_sys_vm = MN_DUAL_SYS_1;
            BOOLEAN is_local_setting = FALSE;
            MMIPLMN_KEYBOARD_SHORTCUTS_T *keyboard_shortcuts_ptr = PNULL;
            uint32 i = 0; 
            
            for (dual_sys_vm = MN_DUAL_SYS_1; dual_sys_vm < MMI_DUAL_SYS_MAX; dual_sys_vm++)
            {
                //read keyboard settings from local setting list
                keyboard_shortcuts_ptr = (MMIPLMN_KEYBOARD_SHORTCUTS_T *)MMIAPIPLMN_GetLocalKeyboardShortcuts(dual_sys_vm);

                if (PNULL != keyboard_shortcuts_ptr)
                {
                    for (i = 0; i < MMIPLMN_KEYBOARD_SHORTCUTS_MAX; i++)
                    {
                        if (msg_id == keyboard_shortcuts_ptr->msg_id[i]
                            && PNULL != keyboard_shortcuts_ptr->keyboard_shorcut_func)
                        {
                            keyboard_shortcuts_ptr->keyboard_shorcut_func(msg_id);

                            is_local_setting = TRUE;

                            break;
                        }
                    }

                    if (is_local_setting)
                    {
                        break;
                    }
                }
            }

            if (!is_local_setting)
#endif
            {
                //cphs 有四个语音号码，暂时只呼叫LINE1语音信箱号码
                MMIAPISMS_OpenCallVMWin(MMI_DUAL_SYS_MAX);
            }
        }
        break;
#endif
    case MSG_KEYLONG_2:
    case MSG_KEYLONG_3:
    case MSG_KEYLONG_4:
    case MSG_KEYLONG_5:
    case MSG_KEYLONG_6:
    case MSG_KEYLONG_7:
    case MSG_KEYLONG_8:
    case MSG_KEYLONG_9:
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);
#if defined(MMI_INDIAN_SOS_SUPPORT) && !defined(MMI_PDA_SUPPORT)
		if ((1 == edit_str.wstr_len) && (MMISET_SOS_ENTRY_CODE == msg_id) && MMIAPISET_GetSOSSwitchState())
		{
			MMIAPISET_StartSOSProcedures();
			MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
		}
		else
#endif
        if ((0 == edit_str.wstr_len) ||
            (1 == edit_str.wstr_len))
        {
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
            uint32 dual_sys_other = MN_DUAL_SYS_1;
            BOOLEAN is_local_setting = FALSE;
            MMIPLMN_KEYBOARD_SHORTCUTS_T *keyboard_shortcuts_ptr = PNULL;
            uint32 i = 0; 

            for (dual_sys_other = MN_DUAL_SYS_1; dual_sys_other < MMI_DUAL_SYS_MAX; dual_sys_other++)
            {
                //read keyboard settings from local setting list
                keyboard_shortcuts_ptr = (MMIPLMN_KEYBOARD_SHORTCUTS_T *)MMIAPIPLMN_GetLocalKeyboardShortcuts(dual_sys_other);

                if (PNULL != keyboard_shortcuts_ptr)
                {
                    for (i = 0; i < MMIPLMN_KEYBOARD_SHORTCUTS_MAX; i++)
                    {
                        if (msg_id == keyboard_shortcuts_ptr->msg_id[i]
                            && PNULL != keyboard_shortcuts_ptr->keyboard_shorcut_func)
                        {
                            keyboard_shortcuts_ptr->keyboard_shorcut_func(msg_id);

                            is_local_setting = TRUE;

                            break;
                        }
                    }

                    if (is_local_setting)
                    {
                        break;
                    }
                }
            }

            if (!is_local_setting)
#endif
            {
#if defined(MMI_SPEEDDIAL_SUPPORT) 
                MMIAPICC_SpeedDial(MMI_DUAL_SYS_MAX, (uint16)(msg_id - MSG_KEYLONG_1 + 1));
#endif
            }
        }
        break;

#ifdef DPHONE_SUPPORT
    case MSG_TIMER:
    	  #ifdef DPHONE_SUPPORT
        if (s_hangup_remind_timer_id == *(uint8 *)param)
        {
            s_hangup_remind_count ++;
            if (MMIIDLE_HANGUP_REMIND_TIME == s_hangup_remind_count)
            {
                MMK_StopTimer(s_hangup_remind_timer_id);       
                s_hangup_remind_timer_id = 0;
                MMIAPISET_PlayRing(0, FALSE,0,1,MMISET_RING_TYPE_HANGUP_REMIND,PNULL);
            }
        }
        #endif
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
        HandleDialPenOkMsg(win_id,edit_ctrl_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENLONGOK:
        HandleDialPenLongOkMsg(win_id,edit_ctrl_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (0 < s_mmiidle_dial_timer_id)
        {
            StopTPButtonVibrator(s_mmiidle_dial_timer_id, 0);
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#if defined(TOUCH_PANEL_SUPPORT)&&!defined(MMI_PDA_SUPPORT)
    case MSG_APP_0:
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_HASH:
    case MSG_APP_STAR:
#ifdef DPHONE_SUPPORT
        s_hangup_remind_count = 0;
#endif 

        if (MMK_IsActiveCtrl(edit_ctrl_id) || MMK_SetAtvCtrl(win_id,edit_ctrl_id))
        {
            MMK_SendMsg(edit_ctrl_id, msg_id, PNULL);
        }
        else
        {
            //SCI_TRACE_LOW:"mmiidle_dial.c HandleDialWinMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_2247_112_2_18_2_23_48_22,(uint8*)"");
        }
        break;
#endif

    case MSG_APP_UP:
    case MSG_APP_DOWN:
        {
            BOOLEAN is_send = TRUE;

            if (TRUE != *(BOOLEAN*)param)
            {
                MMK_SendMsg(list_ctrl_id, msg_id, &is_send);
            }
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //将号码匹配信息插入到列表中
        AppendMatchingInfo(win_id, param);
        break;
        
    case MSG_CLOSE_WINDOW:
#ifdef TOUCH_PANEL_SUPPORT
        if (0 < s_mmiidle_dial_timer_id)
        {
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
        
            StopTPButtonVibrator(s_mmiidle_dial_timer_id, 0);
#endif
        }
#endif        
        #ifdef DPHONE_SUPPORT
        if (0 < s_hangup_remind_timer_id)
        {
            MMK_StopTimer(s_hangup_remind_timer_id);   
            s_hangup_remind_timer_id = 0;
        }
        #endif 
 
        if (s_mmiidle_is_init_dial_str)
        {
            if (PNULL != s_mmiidle_dial_init_tele)
            {
                SCI_FREE(s_mmiidle_dial_init_tele);
            }
            s_mmiidle_is_init_dial_str = FALSE;
        }
                
        MMIAPICL_SearchExit();
        MMIAPIPB_SearchExit(s_num_search_info.handle);
        s_num_search_info.handle = 0;
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
        s_dial_down_flag = FALSE;
#endif
        break;

    case MSG_TVOUT_CLOSE_IND:
        //结束搜索
        MMIAPICL_SearchExit();
        MMIAPIPB_SearchExit(s_num_search_info.handle);
        s_num_search_info.handle = 0;
#if defined(TOUCH_PANEL_SUPPORT)
        MMK_CloseParentWin(info_ptr);
#endif
        MMK_CloseWin(win_id);
        break;

    case MSG_IDLE_DIAL_PB_UPDATE_IND:
        GUILIST_RemoveAllItems(list_ctrl_id);
        GUIEDIT_GetString(edit_ctrl_id, &edit_str);
        UpdateSearchResult(win_id, &edit_str, TRUE); 
        break;
#if 0
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        DrawDialPanelListItemIconData(param);                
        break;
#endif
    case MSG_IDLE_DIAL_RESET_EDIT_IND:
        GUIEDIT_ClearAllStrEx(edit_ctrl_id, MMK_IsFocusWin(win_id));
        break;
        
#if defined(TOUCH_PANEL_SUPPORT)
    case MSG_CTL_LIST_LONGOK:
        {
            uint16 cur_list_index = 0;
            MMI_STRING_T num_str = {0};
            wchar num_wchar[MMIPB_MAX_STRING_LEN] = {0};
            MMIPB_BCD_NUMBER_T pb_num = {0};

            num_str.wstr_ptr = num_wchar;

            cur_list_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID));
            if(s_num_search_info.cl_match_item_num > cur_list_index)
            {
                MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);
            }
            else
            {
                //uint8 num_type = 0;

                MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, PNULL,(cur_list_index - s_num_search_info.cl_match_item_num));
                MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);

                //MMIAPIPB_GetSearchEntryNumber(&num_str, &num_type, MMIPB_MAX_STRING_LEN,(cur_list_index - s_num_search_info.cl_match_item_num));            
            }
            
            if (0 < num_str.wstr_len && PNULL != num_str.wstr_ptr)
            {
                MMK_CreateWin((uint32 * )MMIIDLE_DIAL_LIST_MENU_WIN_TAB,(ADD_DATA)win_id);
                {
                    MMI_CTRL_ID_T   ctrl_id = MMIIDLE_DIAL_LIST_MENU_CTRL_ID;
                    uint16           i = 0;

                    //creat dynamic menu
                    GUIMENU_CreatDynamic(PNULL,MMIIDLE_DIAL_LIST_MENU_WIN_ID, ctrl_id, GUIMENU_STYLE_POPUP_AUTO);

                    if(s_num_search_info.cl_match_item_num > cur_list_index)
                    {
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_CALL_AUDIO,(uint16)ID_DIAL_CALL_AUDIO, 0, i++);
#ifdef VT_SUPPORT
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_CALL_VIDEO,(uint16)ID_DIAL_CALL_VIDEO, 0, i++);
#endif
#ifdef MMI_IP_CALL_SUPPORT
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_CALL_IPCALL, (uint16)ID_DIAL_IPCALL, 0,i++);
#endif
                        #ifdef MMIMMS_SMS_IN_1_SUPPORT //短彩合一
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_COMMON_WRITE_MESSAGE, (uint16)ID_DIAL_SEND_SMS, 0,i++);   
                        #else
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_SMS, (uint16)ID_DIAL_SEND_SMS, 0,i++);                    
                        InsertOptMenuNodeByLabel(ctrl_id,TXT_MMS, (uint16)ID_DIAL_SEND_MMS, 0, i++);
                        #endif
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_ADDTOPB_NEW, (uint16)ID_DIAL_ADD_PB, 0, i++);
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_ADDTOPB_EXIST, (uint16)ID_DIAL_ADD_EXISTNUM, 0, i++);
#endif                        
                    }
                    else
                    {
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_CALL_AUDIO,(uint16)ID_DIAL_CALL_AUDIO, 0, i++);
#ifdef VT_SUPPORT
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_CALL_VIDEO,(uint16)ID_DIAL_CALL_VIDEO, 0, i++);
#endif
#ifdef MMI_IP_CALL_SUPPORT
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_CALL_IPCALL, (uint16)ID_DIAL_IPCALL, 0,i++);
#endif
                        #ifdef MMIMMS_SMS_IN_1_SUPPORT //短彩合一
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_COMMON_WRITE_MESSAGE, (uint16)ID_DIAL_SEND_SMS, 0,i++);               
                        #else                    
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_SMS, (uint16)ID_DIAL_SEND_SMS, 0,i++);                    
                        InsertOptMenuNodeByLabel(ctrl_id, TXT_MMS, (uint16)ID_DIAL_SEND_MMS, 0, i++); 
                        #endif
                    }

                }
            }
        }
        break;
                
    case MSG_IDLE_DIAL_GRAB_PARENT_WIN:
        MMK_WinGrabFocus(info_ptr);
        break;
        
    case MMI_PB_RELOAD_SEARCH_LIST_MSG:	
        GUIEDIT_GetString(edit_ctrl_id, &edit_str);
        UpdateSearchResult(win_id, &edit_str, TRUE);
        break;
        
    case MSG_CTL_TAB_SWITCH:
        GUIEDIT_GetString(edit_ctrl_id, &edit_str);
        UpdateSearchResult(win_id, &edit_str, TRUE);
        break;

    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        if (!s_mmiidle_is_button_panel_close && GUILIST_START_SLIDE == *(GUILIST_SLIDE_STATE_E *)param
            &&!MMITHEME_IsMainScreenLandscape())//滑动list操作，缩小dial panel
        {
            HandleDialButtonPanelCloseOrSpread(win_id);
        }
        break;

     case MSG_CTL_LIST_CHECK_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;

            switch (item_content_ptr->item_content_index)
            {
                case 5:
                    {
                        MMIPB_BCD_NUMBER_T bcd_num = {0};
                        MN_CALLED_NUMBER_T called_num= {0};
    
                        MMIAPICL_GetSearchEntryCalledNumber(&called_num, item_content_ptr->item_index);
                        {
                            SCI_MEMSET(&bcd_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));
                            bcd_num.number_len = called_num.num_len;
                            bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(called_num.number_type,
                              called_num.number_plan);
                            MMI_MEMCPY(bcd_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
                              called_num.party_num,MMIPB_BCD_NUMBER_MAX_LEN,
                              bcd_num.number_len);
                        }
                        //MMIAPIPB_OpenAddMenu(&bcd_num,PB_GROUP_ALL);
                        MMIAPIPB_AddContactWin(&bcd_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                    }
                    break;
                    
                default:
                    break;
            }
        }
        break;
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT    
    case MSG_NOTIFY_LIST_GET_EFFECT_STR:
        {
            GUILIST_EFFECT_STR_T *effect_str = (GUILIST_EFFECT_STR_T *)param;
            
            if (1 == effect_str->content_index)
            {               
                GUIEDIT_GetString(edit_ctrl_id,&edit_str);

                if( 0 < edit_str.wstr_len && s_num_search_info.cl_match_item_num <= effect_str->item_index)
                {
                    MMI_STRING_T name_str = {0};
                    MMI_STRING_T index_str = {0};
                    wchar name_wchar[MMIPB_MAX_STRING_LEN] = {0};
                    wchar index_wchar[MMIPB_MAX_STRING_LEN] = {0};
                    int index = 0;
                    
                    name_str.wstr_ptr = name_wchar;
                    index_str.wstr_ptr = index_wchar;
                    
                    MMIAPIPB_GetSearchEntry(s_num_search_info.handle ,&name_str, MMIPB_MAX_STRING_LEN, PNULL,PNULL,(effect_str->item_index - s_num_search_info.cl_match_item_num));
                    MMIAPIPB_Wstrstrpos(&name_str, &edit_str, &index_str, FALSE);
                    
                    if (0 < index_str.wstr_len)
                    {
                        effect_str->effect_type = GUILIST_EFFECT_REGION;
                        effect_str->region_num = index_str.wstr_len;
                        effect_str->region_ptr = SCI_ALLOC_APP((index_str.wstr_len) * sizeof(GUIFONT_REGION_STYLE_T));
                        
                        for (index = 0; index < index_str.wstr_len; index++)
                        {
                            effect_str->region_ptr[index].start = index_wchar[index];
                            effect_str->region_ptr[index].length = 1;
                            effect_str->region_ptr[index].state = GUISTR_STATE_SPEC_FONTCOLOR;
                            effect_str->region_ptr[index].font_color = MMI_DARK_GREEN_COLOR;
                        }
                    }
                }

            }
            else if (3 == effect_str->content_index)
            {
                effect_str->font_color = MMI_CYAN_COLOR;
                effect_str->state |= GUISTR_STATE_SPEC_FONTCOLOR;
                effect_str->effect_type = GUILIST_EFFECT_STR;
                
                GUIEDIT_GetString(edit_ctrl_id,&edit_str);
                effect_str->str_info.wstr_len = edit_str.wstr_len;
                effect_str->str_info.wstr_ptr = edit_str.wstr_ptr;
            }
        }
         break;
#endif         
    case MSG_NOTIFY_UPDATE:
        {
            GUI_RECT_T          *update_rect_ptr = (GUI_RECT_T*)param;
            GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};

            GUIRES_DisplayImg(PNULL,
                update_rect_ptr,
                update_rect_ptr,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
        }
        break;
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;

    }    
    return (result);
  }
}

/*****************************************************************************/
//  Description : set dial window right softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDialWinRightSoftkey(
                                     wchar          *str_ptr,   //in:
                                     uint16         str_len,    //in:
                                     MMI_TEXT_ID_T  *text_id_ptr//in/out:
                                     )
{
    BOOLEAN         result = FALSE;
    wchar           star_char = {'*'};

    if ((1 == str_len) &&
        (star_char == str_ptr[0]) &&
        (!MMIAPICC_IsInState(CC_IN_CALL_STATE)))
    {
        *text_id_ptr = TXT_IDLE_LOCK;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set dial window intercept edit control cancel msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetWinInterceptEditMsg(
                                  MMI_CTRL_ID_T     edit_ctrl_id
                                  )
{
    GUIEDIT_SetWinInterceptCtrlMsg(edit_ctrl_id,IsLockPhone);
}

/*****************************************************************************/
//  Description : is lock phone
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLockPhone(
                          MMI_WIN_ID_T      win_id,
                          MMI_MESSAGE_ID_E  msg_id,
                          wchar             *str_ptr,
                          uint16            str_len
                          )
{
    BOOLEAN     result = FALSE;
    char        star_char = {'*'};

    if ((MSG_APP_CANCEL == msg_id) && 
        (!MMIAPICC_IsInState(CC_IN_CALL_STATE)) && 
        (1 == str_len) &&
        (star_char == str_ptr[0])
#ifdef TOUCH_PANEL_SUPPORT
         &&(!s_mmiidle_is_tp_del)
#endif
        )
    {
#ifdef MMI_KEY_LOCK_SUPPORT
        MMIAPIKL_LockPhone();
#endif
        MMIAPIIDLE_CloseDialWin();
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display dial win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDialWinBg(
                            MMI_WIN_ID_T    win_id
                            )
{
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
#if defined(TOUCH_PANEL_SUPPORT)
    GUI_RECT_T img_rect = {0};

    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);   
    
    dis_point.x = client_rect.v_rect.left;

    dis_point.y = client_rect.v_rect.top;

    if(!MMITHEME_IsMainScreenLandscape())
        {
            img_rect.bottom=MMI_MAINSCREEN_HEIGHT;
            img_rect.right = MMI_MAINSCREEN_WIDTH;
        }
    else
        {
            img_rect.bottom=MMI_MAINSCREEN_WIDTH;
            img_rect.right = MMI_MAINSCREEN_HEIGHT;
        }
    
    img_rect.left=0;
    img_rect.top=client_rect.v_rect.top;
#endif

    //display bg
    GUIRES_DisplayImg(&dis_point,
        PNULL,
#if defined(TOUCH_PANEL_SUPPORT)
        &img_rect,
#else
        PNULL,
#endif
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
}

/*****************************************************************************/
//  Description : handle dial win background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayGrayButton(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  ctrl_id
                             )
{
#ifdef TOUCH_PANEL_SUPPORT
    MMI_STRING_T        str_info = {0};

    //get current string
    GUIEDIT_GetString(ctrl_id,&str_info);
    
    //button gray
    if (0 == str_info.wstr_len)
    {
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_SAVE_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
    }
    else
    {
        //呼出过程中不能拨打电话
        if (MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
        {
            GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
            GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
        }
        else
        {
            GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID),FALSE,PNULL,TRUE);

            if (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) > 0)
            {
                GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID),FALSE,PNULL,TRUE);
            }
            else
            {
                GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID),TRUE,PNULL,TRUE);
            }
        }

        //save not gray
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_SAVE_BUTTON_CTRL_ID),FALSE,PNULL,TRUE);
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID),FALSE,PNULL,TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description : handle dial window ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialOkMsg(
                           MMI_CTRL_ID_T    ctrl_id
                           )
{
    uint8           temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
    MMI_STRING_T    edit_str = {0};

    //get edit string
    GUIEDIT_GetString(ctrl_id,&edit_str);

    //convert string
    MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
        edit_str.wstr_ptr,edit_str.wstr_len,
        edit_str.wstr_len);

    //if imei
    if (MMIAPIPARSE_ProcessImeiStr(temp_str,edit_str.wstr_len))
    {
        MMIAPIPHONE_OpenIMEIDisplayWin();
    }
    else
    {
        if (MMK_IsOpenWin(MMIIDLE_DIAL_MENU_WIN_ID))
        {
            MMK_WinGrabFocus(MMIIDLE_DIAL_MENU_WIN_ID);
        }
        else
        {
            MMK_CreateWin((uint32 * )MMIIDLE_DIAL_MENU_WIN_TAB,(ADD_DATA)ctrl_id);
        }
    }
}

/*****************************************************************************/
//  Description : handle dial menu window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialMenuWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    uint8               phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
    uint16              phone_num_len = 0;
    wchar               char_p = 'P';
    wchar               char_w = 'W';
    MMI_STRING_T        edit_str = {0};
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIDLE_DIAL_POPMENU_CTRL_ID;
    uint32       add_data = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T       edit_ctrl_id = (MMI_CTRL_ID_T)add_data;
    MMI_MENU_ID_T       menu_id = 0;
    MMIPB_BCD_NUMBER_T  pb_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {0};/*lint !e64*/
    MMI_MENU_GROUP_ID_T group_id = 0;
	BOOLEAN				is_num_valid = TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MONOCHROME_LCD_SUPPORT //added by taiping.lai 2010/12/28
#else			
        //set dial menu gray
        SetDialMenuGray(win_id,edit_ctrl_id);
#endif 
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);

        //get uint8 string
        if((0 < edit_str.wstr_len) && (MMIIDLE_DIAL_MAX_LEN >= edit_str.wstr_len))
        {
            phone_num_len = edit_str.wstr_len;
            MMI_WSTRNTOSTR((uint8 *)phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                edit_str.wstr_ptr,edit_str.wstr_len,
                phone_num_len);
        }

        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_DIAL_ADD_PB:
        case ID_DIAL_ADD_EXISTNUM:

			//+bug132483
			//号码有效才可以添加到contacts中
			is_num_valid = MMIAPICOM_IsValidNumberString((char*)phone_num_str, phone_num_len);
			SCI_TRACE_LOW("[mmiidle] HandleDialMenuWinMsg is_num_valid:%d", is_num_valid);
			SCI_TRACE_BUF("[mmiidle] HandleDialMenuWinMsg phone_num_str:%x", phone_num_str, phone_num_len);
			if (FALSE == is_num_valid)
			{
				//提示无效号码
				MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
				return MMI_RESULT_TRUE; 
			}
			//-bug132483

            MMIAPICOM_GenPartyNumber(phone_num_str,phone_num_len,&party_num);
            
            pb_num.number_len = (uint8)party_num.num_len;
            pb_num.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type,MN_NUM_PLAN_UNKNOW);
            MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN, 
                party_num.bcd_num,party_num.num_len,
                party_num.num_len);

            if (ID_DIAL_ADD_PB == menu_id)
            {
                //MMIAPIPB_OpenAddMenu(&pb_num,PB_GROUP_ALL);
                MMIAPIPB_AddContactWin(&pb_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
            }
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
            else
            {
                //if ((0 < pb_num.number_len)||(MN_NUM_TYPE_INTERNATIONAL == (pb_num.npi_ton >> 4)))
				if(MMIAPIPB_IsValidPhoneNumber((pb_num.npi_ton >> 4), pb_num.number_len))
                {
                   // MMIAPIPB_OpenEntryListForCallLog(&pb_num);
                    MMIAPIPB_AddToExistContact(&pb_num, MMIPB_ADD_TYPE_NUMBER, PNULL);
                }
                else
                {
                    //prompt no phone number
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
                }
            }
#endif
            break;

        case ID_DIAL_SEND_SMS:
            MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX,
                PNULL,
                phone_num_str,
                (uint8)phone_num_len);
            break;

        case ID_DIAL_SEND_MMS:
            MMIAPIMMS_AnswerMMS(MN_DUAL_SYS_MAX,PNULL,&edit_str);
            break;

        case ID_DAIL_CALL_LOG:
            MMIAPICL_OpenMainMenuWindow();
            break;

        case ID_DIAL_INSERT_P:
            GUIEDIT_InsertString(edit_ctrl_id,&char_p,1);
            break;

        case ID_DIAL_INSERT_W:
            GUIEDIT_InsertString(edit_ctrl_id,&char_w,1);
            break;
            
        case ID_DIAL_CALL_AUDIO:
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_NORMAL_CALL,
                PNULL);
            break;

#ifdef BT_DIALER_SUPPORT
        case ID_DIAL_CALL_BT_AUDIO:
            MMIAPICC_MakeBtCall(phone_num_str,(uint8)phone_num_len);
            break;
#endif

#ifdef VT_SUPPORT
        case ID_DIAL_CALL_VIDEO:
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_VIDEO_CALL,
                PNULL);
            break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
        case ID_DIAL_IPCALL:
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_IP_CALL,
                PNULL);
            break;
#endif

        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        GUI_INVALID_PARAM(param);/*lint !e522*/
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : handle dial menu window msg
//  Global resource dependence : 
//  Author: chunjie liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDialGreenMenuWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    uint8               phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
    uint16              phone_num_len = 0;
    MMI_STRING_T        edit_str = {0};
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIDLE_DIAL_GREEN_POPMENU_CTRL_ID;
    uint32       add_data = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T       edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;
    CC_CALL_SIM_TYPE_E sim_type =  (MMI_CTRL_ID_T)add_data;
    MMI_MENU_ID_T       menu_id = 0;
    MMIPB_BCD_NUMBER_T  pb_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {0};/*lint !e64*/
    MMI_MENU_GROUP_ID_T group_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:			
        //set dial menu gray
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);
        if (0 == edit_str.wstr_len)
        {
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_GREEN_POPMENU_CTRL_ID,MENU_PHONENUM_GREEN_DIALUP_OPERATE,ID_DIAL_GREEN_CALL_AUDIO,TRUE);
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_GREEN_POPMENU_CTRL_ID,MENU_PHONENUM_GREEN_DIALUP_OPERATE,ID_DIAL_GREEN_CALL_BT_AUDIO,TRUE);
        }
        else
        {
            if(!MMIAPIBT_IsConnectDevice() || MMICC_IsExistActiveCall() || MMICC_IsExistHoldCall())
            {
                GUIMENU_SetItemGrayed(MMIIDLE_DIAL_GREEN_POPMENU_CTRL_ID,MENU_PHONENUM_GREEN_DIALUP_OPERATE,ID_DIAL_GREEN_CALL_BT_AUDIO,TRUE);
            }
            if (MMIAPISET_GetFlyMode())
            {
                GUIMENU_SetItemGrayed(MMIIDLE_DIAL_GREEN_POPMENU_CTRL_ID,MENU_PHONENUM_GREEN_DIALUP_OPERATE,ID_DIAL_GREEN_CALL_AUDIO,TRUE);
            }
        }

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);

        //get uint8 string
        if((0 < edit_str.wstr_len) && (MMIIDLE_DIAL_MAX_LEN >= edit_str.wstr_len))
        {
            phone_num_len = edit_str.wstr_len;
            MMI_WSTRNTOSTR((uint8 *)phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                edit_str.wstr_ptr,edit_str.wstr_len,
                phone_num_len);
        }

        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {                   
        case ID_DIAL_GREEN_CALL_AUDIO:
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                phone_num_str,
                (uint8)phone_num_len,
                PNULL,
                PNULL,
                sim_type,
                CC_CALL_NORMAL_CALL,
                PNULL);
            break;
        case ID_DIAL_GREEN_CALL_BT_AUDIO:
             MMIAPICC_MakeBtCall(phone_num_str,(uint8)phone_num_len);
            break;
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        GUI_INVALID_PARAM(param);/*lint !e522*/
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

/*****************************************************************************/
//  Description : set dial menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDialMenuGray(
                           MMI_WIN_ID_T     win_id,
                           MMI_CTRL_ID_T    edit_ctrl_id
                           )
{
    // BOOLEAN         is_ready_sim1 = FALSE;
    // BOOLEAN         is_ready_sim2 = FALSE;
    MMI_STRING_T    edit_str = {0};
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
#ifdef BT_DIALER_SUPPORT    
    BOOLEAN is_bt_call_gray = FALSE;
#endif

    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

    //get edit string
    GUIEDIT_GetString(edit_ctrl_id,&edit_str);


    if (0 == edit_str.wstr_len)
    {
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_OPT,TRUE);
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_ADD_PB_OPT,TRUE);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_SEND_SMS,TRUE);
#else    
#ifdef MMS_SUPPORT
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_SEND,TRUE);
#else
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_SEND_SMS,TRUE);
#endif
#endif
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_INSERT_P,TRUE);
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_INSERT_W,TRUE);
    }
    else
    {
#ifdef DPHONE_SUPPORT//@fen.xie MS00185274
       MMI_MENU_GROUP_ID_T  group_id = MENU_PHONENUM_DIALUP_OPERATE;
#else
#if defined(MMI_IP_CALL_SUPPORT) || defined(BT_DIALER_SUPPORT)
       MMI_MENU_GROUP_ID_T  group_id = MENU_DIAL_OPT;
#endif
#endif

#ifdef BT_DIALER_SUPPORT
        if(!MMIAPIBT_IsConnectDevice() || MMICC_IsExistActiveCall() || MMICC_IsExistHoldCall())
        {
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,group_id,ID_DIAL_CALL_BT_AUDIO,TRUE);
            is_bt_call_gray =TRUE;
        }
#endif
#ifdef BT_DIALER_SUPPORT
        if (MMIAPISET_GetFlyMode())
        {
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,group_id,ID_DIAL_CALL_AUDIO,TRUE);
        }
#endif

        if (MMIAPISET_GetFlyMode()
#ifdef BT_DIALER_SUPPORT            
            && is_bt_call_gray
#endif        
		)
        {
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_OPT,TRUE);
        }

#ifdef MMI_IP_CALL_SUPPORT
        if (0 == sim_ok_num)
        {
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,group_id,ID_DIAL_IPCALL,TRUE);
        }
#endif

        if(0 == GUIEDIT_GetCursorCurPos(edit_ctrl_id))
        {
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_INSERT_P,TRUE);
            GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_INSERT_W,TRUE);
        }
    }

#ifdef MMI_DUALMODE_ENABLE
    if (MMI_GMMREG_RAT_3G == MMIAPIPHONE_GetTDOrGsm(MMI_DUAL_SYS_MAX))   
    {
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_CALL_VIDEO,FALSE);
    }
    else
    {
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_CALL_VIDEO,TRUE);
    }
#endif

    //通话过程中不能操作彩信
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_SEND_MMS,TRUE);
    }

    //呼出过程中不能拨打电话
    if (MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    {
        GUIMENU_SetItemGrayed(MMIIDLE_DIAL_POPMENU_CTRL_ID,MENU_PHONENUM_DIALUP_OPERATE,ID_DIAL_OPT,TRUE);
    }
}

/*****************************************************************************/
//  Description : handle dial window web msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialWebMsg(
                            MMI_WIN_ID_T win_id,
                            MMI_CTRL_ID_T   ctrl_id
                            )
{
    uint8           temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
    MMI_STRING_T    edit_str = {0};
    uint8 list_phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
    uint16 list_phone_num_len = 0;    
    uint16 cur_list_index = 0;    
    uint16 edit_phone_num_len = 0;
    MMIPB_BCD_NUMBER_T  pb_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {0};/*lint !e64*/   
    MMI_STRING_T num_str = {0};
    wchar num_wchar[MMIPB_MAX_STRING_LEN] = {0};

    num_str.wstr_ptr = num_wchar;
    

    //get edit string
    GUIEDIT_GetString(ctrl_id,&edit_str);

    //convert string
    MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
        edit_str.wstr_ptr,edit_str.wstr_len,
        edit_str.wstr_len);

    edit_phone_num_len = edit_str.wstr_len;

    //get list num string
    {
        cur_list_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID));
        if(s_num_search_info.cl_match_item_num > cur_list_index)
        {
            MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);
        }
        else
        {
            MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, PNULL,(cur_list_index - s_num_search_info.cl_match_item_num));
            MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);

            //uint8 num_type = 0;
            //MMIAPIPB_GetSearchEntryNumber(&num_str, &num_type, MMIPB_MAX_STRING_LEN,(cur_list_index - s_num_search_info.cl_match_item_num));            
        }
        list_phone_num_len = num_str.wstr_len;
        MMI_WSTRNTOSTR((uint8 *)list_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
            num_str.wstr_ptr,num_str.wstr_len,
            list_phone_num_len);
    }

    if(0 == (s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num))
    {
         MMIAPICOM_GenPartyNumber(temp_str,edit_phone_num_len,&party_num);
            
        pb_num.number_len = (uint8)party_num.num_len;
        pb_num.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type,MN_NUM_PLAN_UNKNOW);
        MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN, 
            party_num.bcd_num,party_num.num_len,
            party_num.num_len);
        MMIAPIPB_AddContactWin(&pb_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        //MMIAPIPB_OpenAddMenu(&pb_num,PB_GROUP_ALL);
    }
    //make list contact num copy to edit
    else 
    {
        GUIEDIT_SetString(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), num_str.wstr_ptr, num_str.wstr_len);
        MMK_PostMsg(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), MSG_CTL_PAINT, PNULL, 0);
    }
}
#if 0
/*****************************************************************************/
//  Description : 判断是否时号码快速查询
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPbFastSeach(
                            uint8   *str_ptr,
                            uint16  str_len
                            )
{
    BOOLEAN     result = TRUE;
    uint8       seach_char = 0;
    uint16      i = 0;

    for (i=0; i<str_len; i++)
    {
        seach_char = *str_ptr++;
        if (('0' > seach_char) ||
            ('9' < seach_char))
        {
            result = FALSE;
            break;
        }
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : handle dial window green up msg
//  Global resource dependence : 
//  Author: Michael.Shi
//  Note: modified for multisim dial handle...
/*****************************************************************************/
LOCAL void HandleDialGreenUpMsg(
                                MMI_CTRL_ID_T   ctrl_id,
                                MN_DUAL_SYS_E dual_sys
                                )
{
    uint8           temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
    MMI_STRING_T    edit_str = {0};
    CC_CALL_SIM_TYPE_E sim_type = CC_SIM1_CALL;

    //get edit string
    GUIEDIT_GetString(ctrl_id,&edit_str);

    if (0 == edit_str.wstr_len)
    {
        MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
    }
    else
    {
        //convert string
        MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
            edit_str.wstr_ptr,edit_str.wstr_len,
            edit_str.wstr_len);
        
        sim_type = MMIAPICC_GetCallSimType(dual_sys);
#ifdef BT_DIALER_SUPPORT
        if(MMIAPIBT_IsConnectDevice())
        {
            MMK_CreateWin((uint32 * )MMIIDLE_DIAL_GREEN_MENU_WIN_TAB,(ADD_DATA)sim_type);
        }
        else
#endif
        {        
            MMIAPICC_MakeCall(dual_sys,
                temp_str,
                (uint8)edit_str.wstr_len,
                PNULL,
                PNULL,
                sim_type,
                CC_CALL_NORMAL_CALL,
                PNULL);
        }
    }
}

/*****************************************************************************/
//  Description : handle dial window video tel msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialVideoTelMsg(
                                 MMI_CTRL_ID_T  ctrl_id
                                 )
{
    uint8           temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
    MMI_STRING_T    edit_str = {0};

    //get edit string
    GUIEDIT_GetString(ctrl_id,&edit_str);

    if (0 == edit_str.wstr_len)
    {
        MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
    }
    else
    {
        //convert string
        MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
            edit_str.wstr_ptr,edit_str.wstr_len,
            edit_str.wstr_len);

        MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
            temp_str,
            (uint8)edit_str.wstr_len,
            PNULL,
            PNULL,
            CC_CALL_SIM_MAX,
            CC_CALL_VIDEO_CALL,
            PNULL);
    }
}
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
/*****************************************************************************/
//  Description : start vibrator timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartVibratorTimer(
                              MMI_WIN_ID_T  win_id
                              )
{
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
    if (0 == s_mmiidle_dial_timer_id)
    {
        s_mmiidle_dial_timer_id = MMK_CreateTimerCallback(MMIIDLE_DIAL_VIBRATE_TIME,StopTPButtonVibrator,NULL,FALSE);
    }
#endif
}

/*****************************************************************************/
//  Description : stop vibrator timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopVibratorTimer(void)
{
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
    if (0 < s_mmiidle_dial_timer_id)
    {
        MMK_StopTimer(s_mmiidle_dial_timer_id);
        s_mmiidle_dial_timer_id = 0;
    }
#endif
}
#endif
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
/*****************************************************************************/
//  Description : stop tp button vibrator
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTPButtonVibrator(uint8 timer_id, uint32 param)
{

    StopVibratorTimer();
    MMIAPISET_SetAutoStopVibrator(FALSE, MMISET_RING_TYPE_OTHER, MMIIDLE_DIAL_VIBRATE_TIME);


}
#endif
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
/*****************************************************************************/
//  Description : handle dial window pen long ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialPenLongOkMsg(
                                  MMI_WIN_ID_T      win_id,         //in:
                                  MMI_CTRL_ID_T     edit_ctrl_id,   //in:edit ctrl
                                  MMI_CTRL_ID_T     button_ctrl_id  //in:button ctrl
                                  )
{

    MMI_STRING_T        edit_str = {0};
    MMI_MESSAGE_ID_E    key_msg_id = 0;

    //start vibrator
    if (!MMIAPISET_GetIsTouchVibrate())//@juan.wu 将拨号盘触摸屏振动放到情景模式中，0值为开启，1为关闭，所以取反
    {
        //stop vibrator timer
        StopVibratorTimer();

        //start vibrator timer
        StartVibratorTimer(win_id);

        //start vibrator
        MMIAPISET_SetAutoStopVibrator(TRUE, MMISET_RING_TYPE_OTHER, MMIIDLE_DIAL_VIBRATE_TIME);
    }

    //get edit string
    GUIEDIT_GetString(edit_ctrl_id,&edit_str);

    switch (button_ctrl_id)
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
    case MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID:
        if (0 == edit_str.wstr_len
            || MMIIDLE_DIAL_KEYSTAR_BUTTON_CTRL_ID == button_ctrl_id)
        {
            key_msg_id = MSG_KEYLONG_1 + button_ctrl_id - MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID;
            //handle long number key
            MMK_SendMsg(win_id,key_msg_id,PNULL);
        }
        break;

    case MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID:
        key_msg_id = MSG_KEYLONG_CANCEL;
        if (MMK_IsActiveCtrl(edit_ctrl_id) || MMK_SetAtvCtrl(win_id,edit_ctrl_id))
        {
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_NOTIFY_IM_LONG_DEL,PNULL); // 发送给edit控件
        }
        break;

    default:
        //SCI_TRACE_LOW:"HandleDialPenLongOkMsg:button_ctrl_id %d don't handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_3233_112_2_18_2_23_50_23,(uint8*)"d",button_ctrl_id);
        break;
    }

}
#endif
/*****************************************************************************/
//  Description : close dial window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CloseDialWin(void)
{
#if defined(MMI_PDA_SUPPORT)
    MMK_PostMsg(MMK_GetWinHandle(s_mmiidle_pb_applet_handle, MMIIDLE_DIAL_WIN_ID), MSG_CTL_CANCEL, PNULL, 0);
#else
    MMK_CloseWin(MMIIDLE_DIAL_WIN_ID);
#endif
}

/*****************************************************************************/
//  Description : reset dial num in edit
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ResetDialNum(void)
{
#if defined(MMI_PDA_SUPPORT)
    MMK_PostMsg(MMK_GetWinHandle(s_mmiidle_pb_applet_handle, MMIIDLE_DIAL_WIN_ID), MSG_IDLE_DIAL_RESET_EDIT_IND, PNULL, 0);
#else
    MMK_PostMsg(MMIIDLE_DIAL_WIN_ID, MSG_IDLE_DIAL_RESET_EDIT_IND, PNULL, 0);
#endif
}

/*****************************************************************************/
//  Description : close dial menu window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CloseDialMenuWin(void)
{
    MMK_CloseWin(MMIIDLE_DIAL_MENU_WIN_ID);
}

/*****************************************************************************/
//  Description : 创建拨号盘中的输入框
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void InitDialEdit(MMI_WIN_ID_T win_id)
{
#if defined(MAINLCD_SIZE_128X64) //@ alex added in 2010.11.27
	//GUI_BG_T                bg_info = {0}; /* lint_lai */
	GUIEDIT_INIT_DATA_T         edit_init = {0};
	GUI_RECT_T                  edit_rect_ref = MMIIDLE_DIAL_EDIT_RECT;
	//GUIEDIT_INIT_DATA_T     edit_init = {0};   
	MMI_CTRL_ID_T           edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;
	MMI_CONTROL_CREATE_T    edit_ctrl = {0};
	
	edit_init.both_rect.h_rect.top = edit_rect_ref.top;
	edit_init.both_rect.h_rect.left = edit_rect_ref.left;
	edit_init.both_rect.h_rect.right = edit_rect_ref.right;
	edit_init.both_rect.h_rect.bottom = edit_rect_ref.bottom;
	edit_init.both_rect.v_rect.top = edit_rect_ref.top;
	edit_init.both_rect.v_rect.left = edit_rect_ref.left;
	edit_init.both_rect.v_rect.right = edit_rect_ref.right;
	edit_init.both_rect.v_rect.bottom = edit_rect_ref.bottom; 
	
    	edit_init.type        = GUIEDIT_TYPE_PHONENUM;
    	edit_init.str_max_len = MMIIDLE_DIAL_MAX_LEN;
    	//create phone number edit control
    	edit_ctrl.ctrl_id           = edit_ctrl_id;
    	edit_ctrl.guid              = SPRD_GUI_EDITBOX_ID;
    	edit_ctrl.init_data_ptr     = &edit_init;
    	edit_ctrl.parent_win_handle = win_id;
    	MMK_CreateControl(&edit_ctrl);	
#else
    // GUI_BG_T                bg_info = {0};
    MMI_CTRL_ID_T           edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;
    GUIEDIT_INIT_DATA_T     edit_init = {0};    
    MMI_CONTROL_CREATE_T    edit_ctrl = {0};     
    GUI_BOTH_RECT_T         client_rect = {0};

    client_rect = MMITHEME_GetWinClientBothRect(win_id);    
        
    edit_init.both_rect.v_rect.left = 0;
    edit_init.both_rect.v_rect.top = client_rect.v_rect.top;
    edit_init.both_rect.v_rect.right = client_rect.v_rect.right;
    edit_init.both_rect.v_rect.bottom = edit_init.both_rect.v_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT - MMIIDLE_DIAL_LINE_WIDTH;

    edit_init.both_rect.h_rect.left = 0;
    edit_init.both_rect.h_rect.top = client_rect.h_rect.top;
    edit_init.both_rect.h_rect.right = client_rect.h_rect.right;
    edit_init.both_rect.h_rect.bottom = edit_init.both_rect.h_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H - MMIIDLE_DIAL_LINE_WIDTH;

    edit_init.type        = GUIEDIT_TYPE_PHONENUM;
    edit_init.str_max_len = MMIIDLE_DIAL_MAX_LEN;
    
    //create phone number edit control
    edit_ctrl.ctrl_id           = edit_ctrl_id;
    edit_ctrl.guid              = SPRD_GUI_EDITBOX_ID;
    edit_ctrl.init_data_ptr     = &edit_init;

    edit_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&edit_ctrl);	
#endif
    //set im not handle tp
    GUIEDIT_SetImTp(edit_ctrl_id, FALSE);

    //set delete after full
    GUIEDIT_SetPhoneNumStyle(FALSE, edit_ctrl_id);
    
    //set display direction
    GUIEDIT_SetPhoneNumDir(edit_ctrl_id,GUIEDIT_DISPLAY_DIR_RB);
    //多行但没有滚动条
#ifdef MAINLCD_SIZE_128X64
    GUIEDIT_SetStyle(edit_ctrl_id, GUIEDIT_STYLE_MULTI);//@fen.xie
#else   
    GUIEDIT_SetStyle(edit_ctrl_id, GUIEDIT_STYLE_SINGLE);//@fen.xie
    
#endif 
    GUIEDIT_SetAlign(edit_ctrl_id, ALIGN_LVMIDDLE);
    //GUIEDIT_SetAlign(edit_ctrl_id, ALIGN_LVMIDDLE);
    //set border & font color & bg
    {
#ifdef MONOCHROME_LCD_SUPPORT
        GUI_BG_T        edit_bg = {GUI_BG_COLOR,/*GUI_SHAPE_SROUNDED_RECT*/GUI_SHAPE_RECT,IMAGE_NULL,MMI_WHITE_COLOR,FALSE};
        GUI_BORDER_T    edit_border = {2,/*MMI_BACKGROUND_COLOR*/MMI_BLACK_COLOR,/*GUI_BORDER_ROUNDED*/GUI_BORDER_NONE};//added by taiping.lai 2010/12/31 无需边框
        GUI_COLOR_T     edit_font_color = MMI_BLACK_COLOR;//MMI_BACKGROUND_COLOR;
#else
        GUI_BG_T        edit_bg = {GUI_BG_IMG,GUI_SHAPE_RECT,IMAGE_DIAL_EDIT_BG,MMI_WHITE_COLOR,FALSE};
        GUI_BORDER_T    edit_border = {1,MMI_BACKGROUND_COLOR,GUI_BORDER_NONE};
        GUI_COLOR_T     edit_font_color = MMI_IDLE_DIAL_EDIT_NUM_COLOR;
#endif

        GUIEDIT_SetBg(edit_ctrl_id, &edit_bg);
        GUIEDIT_SetBorder(edit_ctrl_id, &edit_border);
        GUIEDIT_SetFontColor(edit_ctrl_id, edit_font_color);
        {
            GUIEDIT_FONT_T  editfont ={0};
            editfont.is_valid = TRUE;
#if defined(MAINLCD_SIZE_128X160)
            editfont.big_font = SONG_FONT_32;
            editfont.mid_font = SONG_FONT_20;
            editfont.small_font = SONG_FONT_12;
#elif defined(MAINLCD_SIZE_176X220)
            editfont.big_font = SONG_FONT_32;
            editfont.mid_font = SONG_FONT_22;
            editfont.small_font = SONG_FONT_14;
#else
            editfont.big_font = SONG_FONT_34;
            editfont.mid_font = SONG_FONT_22;
            editfont.small_font = SONG_FONT_14;
#endif

            GUIEDIT_SetFontEx(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), &editfont);
            GUIEDIT_SetDialEditAdaptive(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), TRUE, TRUE);
        }
    }

    {
        GUI_BOTH_RECT_T line_rect = {0};

        line_rect.v_rect.left = 0;
        line_rect.v_rect.right = client_rect.v_rect.right;
        line_rect.v_rect.top = edit_init.both_rect.v_rect.bottom + 1;
        line_rect.v_rect.bottom = line_rect.v_rect.top;
        line_rect.h_rect.left =0;
        line_rect.h_rect.right = client_rect.h_rect.right;
        line_rect.h_rect.top = edit_init.both_rect.h_rect.bottom + 1;
        line_rect.h_rect.bottom = line_rect.h_rect.top;
        DrawDialPanelLine(win_id, MMIIDLE_DIAL_EDIT_BOX_LINE_CTRL_ID, &line_rect, IMAGE_DIAL_LINE);
    }
}

#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : handle dial window pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDialPenOkMsg(
                              MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     edit_ctrl_id,   //in:edit ctrl
                              MMI_CTRL_ID_T     ctrl_id  //in:button ctrl
                              )
{
#if !defined(TOUCH_PANEL_SUPPORT)
    uint8 dtmf_ch = 0;
#endif
    uint8 edit_phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
    uint16 edit_phone_num_len = 0;
    uint8 list_phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
    uint16 list_phone_num_len = 0;    
    MMI_STRING_T        edit_str = {0};
    MMI_MESSAGE_ID_E    keydown_msg_id = 0;
    MMI_MESSAGE_ID_E    keyup_msg_id = 0;
    MMIPB_BCD_NUMBER_T  pb_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {0};/*lint !e64*/
    uint16 cur_list_index = 0;
    MMI_STRING_T num_str = {0};
    wchar num_wchar[MMIPB_MAX_STRING_LEN] = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    num_str.wstr_ptr = num_wchar;

    //start vibrator
    if (!MMIAPISET_GetIsTouchVibrate())//@juan.wu 将拨号盘触摸屏振动放到情景模式中，0值为开启，1为关闭，所以取反
    {
        //stop vibrator timer
        StopVibratorTimer();

        //start vibrator timer
        StartVibratorTimer(win_id);

        //start vibrator
        //MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetAutoStopVibrator(TRUE, MMISET_RING_TYPE_OTHER, MMIIDLE_DIAL_VIBRATE_TIME);
    }

    //get edit string
    GUIEDIT_GetString(edit_ctrl_id,&edit_str);

    //get uint8 string
    if (edit_str.wstr_len > MMIIDLE_DIAL_MAX_LEN)
    {
        edit_str.wstr_len = MMIIDLE_DIAL_MAX_LEN;
    }
    edit_phone_num_len = edit_str.wstr_len;
    MMI_WSTRNTOSTR((uint8 *)edit_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
        edit_str.wstr_ptr,edit_str.wstr_len,
        edit_phone_num_len);

    //get list num string
    {
        cur_list_index = GUILIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID));
        if(s_num_search_info.cl_match_item_num > cur_list_index)
        {
            MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);
            dual_sys = MMIAPICL_GetSearchEntrySim(cur_list_index);
        }
        else
        {
            dual_sys = MN_DUAL_SYS_MAX;
            MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, PNULL,(cur_list_index - s_num_search_info.cl_match_item_num));
            MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);
            //uint8 num_type = 0;
            //MMIAPIPB_GetSearchEntryNumber(&num_str, &num_type, MMIPB_MAX_STRING_LEN,(cur_list_index - s_num_search_info.cl_match_item_num));            
        }
        list_phone_num_len = num_str.wstr_len;
        MMI_WSTRNTOSTR((uint8 *)list_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
            num_str.wstr_ptr,num_str.wstr_len,
            list_phone_num_len);
    }

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
        keyup_msg_id = MSG_KEYUP_1 + ctrl_id - MMIIDLE_DIAL_KEY1_BUTTON_CTRL_ID;

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_PlayDialTone(keydown_msg_id, FALSE);
#endif

        //handle key
        if (MMK_IsActiveCtrl(edit_ctrl_id) || MMK_SetAtvCtrl(win_id,edit_ctrl_id))
        {
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),keydown_msg_id,PNULL);
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),keyup_msg_id,PNULL);
        }
        else
        {
            //SCI_TRACE_LOW:"mmiidle_dial.c HandleDialWinMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_3496_112_2_18_2_23_51_24,(uint8*)"");
        }
        break;

    case MMIIDLE_DIAL_SAVE_BUTTON_CTRL_ID:
        MMIAPICOM_GenPartyNumber(edit_phone_num_str,edit_phone_num_len,&party_num);
            
        pb_num.number_len = (uint8)party_num.num_len;
        pb_num.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type,MN_NUM_PLAN_UNKNOW);
        MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN, 
            party_num.bcd_num,party_num.num_len,
            party_num.num_len);
        MMIAPIPB_AddContactWin(&pb_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        //MMIAPIPB_OpenAddMenu(&pb_num,PB_GROUP_ALL);
        break;

    case MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID:
        SCI_TRACE_LOW(" HandleDialPenOkMsg press dial button cl:%d, pb:%d",s_num_search_info.cl_match_item_num, s_num_search_info.pb_match_item_num);

        if (0 == edit_phone_num_len)//edit box is null
        {
            if(0 == s_num_search_info.cl_match_item_num)//list is null
            {
                return ;
            }
            else
            {
                //get first num of index string
                {
                    cur_list_index = 0;
                    if(s_num_search_info.cl_match_item_num > cur_list_index)
                    {
                        MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);
                    }
                    else
                    {
                        MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, PNULL,(cur_list_index - s_num_search_info.cl_match_item_num));
                        MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);
                        //uint8 num_type = 0;
                        //MMIAPIPB_GetSearchEntryNumber(&num_str, &num_type, MMIPB_MAX_STRING_LEN,(cur_list_index - s_num_search_info.cl_match_item_num));            
                    }
                    
                    list_phone_num_len = num_str.wstr_len;
                    MMI_WSTRNTOSTR((uint8 *)list_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                        num_str.wstr_ptr,num_str.wstr_len,
                        list_phone_num_len);
                }
                
                if (0 == num_str.wstr_len)//num is null
                {
                    //prompt no phone number
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
                    return ;
                }
                else
                {
                    //bug 138642
                    //convert string
                    /*MMI_WSTRNTOSTR((uint8 *)edit_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                        num_str.wstr_ptr,num_str.wstr_len,
                        num_str.wstr_len);

                    edit_phone_num_len = num_str.wstr_len;*/
                    GUIEDIT_SetString(edit_ctrl_id,num_str.wstr_ptr,num_str.wstr_len);
                    MMK_PostMsg(MMK_ConvertIdToHandle(edit_ctrl_id), MSG_CTL_PAINT, PNULL, 0);
                }
            }
        }
        else
        {
            MMIAPICC_MakeCall(dual_sys,
            edit_phone_num_str,
            (uint8)edit_phone_num_len,
            PNULL,
            PNULL,
            CC_CALL_SIM_MAX,
            CC_CALL_NORMAL_CALL,
            PNULL);
        }
        
        break;

#ifdef VT_SUPPORT
    case MMIIDLE_DIAL_VIDEO_CALL_BUTTON_CTRL_ID:
#endif
#if defined(MMI_IP_CALL_SUPPORT)
    case MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID:
#endif
    case MMIIDLE_DIAL_SMS_BUTTON_CTRL_ID:
        if (0 == edit_phone_num_len)//edit box is null
        {
            if(0 == (s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num))//list is null
            {
                return ;
            }
            else
            {
                //get first num of index string
                {
                    cur_list_index = 0;
                    if(s_num_search_info.cl_match_item_num > cur_list_index)
                    {
                        MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);
                    }
                    else
                    {
                        MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, PNULL,(cur_list_index - s_num_search_info.cl_match_item_num));
                        MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);
                        //uint8 num_type = 0;
                        //MMIAPIPB_GetSearchEntryNumber(&num_str, &num_type, MMIPB_MAX_STRING_LEN,(cur_list_index - s_num_search_info.cl_match_item_num));            
                    }
                    
                    list_phone_num_len = num_str.wstr_len;
                    MMI_WSTRNTOSTR((uint8 *)list_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                        num_str.wstr_ptr,num_str.wstr_len,
                        list_phone_num_len);
                }
                
                if (0 == num_str.wstr_len)//num is null
                {
                    //prompt no phone number
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
                    return ;
                }
                else
                {
                    //convert string
                    //                     MMI_WSTRNTOSTR((uint8 *)edit_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                    //                         num_str.wstr_ptr,num_str.wstr_len,
                    //                         num_str.wstr_len);
                    //                     
                    //                     edit_phone_num_len = num_str.wstr_len;
                    
                    GUIEDIT_SetString(edit_ctrl_id,num_str.wstr_ptr,num_str.wstr_len);
                    MMK_PostMsg(MMK_ConvertIdToHandle(edit_ctrl_id), MSG_CTL_PAINT, PNULL, 0);
                }
            }
        }
		else
#ifdef VT_SUPPORT
        if (MMIIDLE_DIAL_VIDEO_CALL_BUTTON_CTRL_ID == ctrl_id)
        {
            MMIAPICC_MakeCall(dual_sys,
                    edit_phone_num_str,
                    (uint8)edit_phone_num_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_VIDEO_CALL,
                    PNULL);
        }
        else
#endif
#if defined(MMI_IP_CALL_SUPPORT)
        if (MMIIDLE_DIAL_IP_CALL_BUTTON_CTRL_ID == ctrl_id)
        {
            MMIAPICC_MakeCall(dual_sys,
                    edit_phone_num_str,
                    (uint8)edit_phone_num_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_IP_CALL,
                    PNULL);
        }
        else
#endif
        {
            MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX,
                PNULL,
                edit_phone_num_str,
                (uint8)edit_phone_num_len);
            
        }
        break;

    case MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID:
        //handle cancel
        s_mmiidle_is_tp_del = TRUE;
        if (MMK_IsActiveCtrl(edit_ctrl_id) || MMK_SetAtvCtrl(win_id,edit_ctrl_id))
        {
            MMK_SendMsg(MMK_GetActiveCtrl(win_id),MSG_NOTIFY_IM_BACKSPACE,PNULL);
        }
        s_mmiidle_is_tp_del = FALSE;
        break;

    case MMIIDLE_DIAL_LISTBOX_CTRL_ID:
        //add to pb
        if(0 == (s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num))
        {
             MMIAPICOM_GenPartyNumber(edit_phone_num_str,edit_phone_num_len,&party_num);
                
            pb_num.number_len = (uint8)party_num.num_len;
            pb_num.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type,MN_NUM_PLAN_UNKNOW);
            MMI_MEMCPY(pb_num.number,MMIPB_BCD_NUMBER_MAX_LEN, 
                party_num.bcd_num,party_num.num_len,
                party_num.num_len);
            MMIAPIPB_AddContactWin(&pb_num,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
            //MMIAPIPB_OpenAddMenu(&pb_num,PB_GROUP_ALL);
        }
        //make list contact num copy to edit
        else 
        {
#if defined(MMI_PDA_SUPPORT)
            if (0 == num_str.wstr_len)
            {
                //prompt no phone number
                MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
            }
            else
            {
                uint8           temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
                
                //convert string
                MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
                    num_str.wstr_ptr,num_str.wstr_len,
                    num_str.wstr_len);
                                    
                MMIAPICC_MakeCall(dual_sys,
                    temp_str,
                    (uint8)num_str.wstr_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_NORMAL_CALL,
                    PNULL);
            }
#else
            GUIEDIT_SetString(edit_ctrl_id, num_str.wstr_ptr, num_str.wstr_len);
            MMK_SetAtvCtrl(win_id,edit_ctrl_id);
            MMK_PostMsg(edit_ctrl_id, MSG_CTL_PAINT, PNULL, 0);
#endif
        }
        break; 

    case MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID:
    case MMIIDLE_DIAL_FUNC_SHOW_BUTTON_CTRL_ID:
        HandleDialButtonPanelCloseOrSpread(win_id);
        break;

#ifdef MMI_ISTYLE_SUPPORT
    case MMIIDLE_DIAL_MATCH_NUM_ARROW_CTRL_ID:
        MMK_CreatePubListWin((uint32*) MMIIDLE_DIAL_MATCH_LIST_WIN_TAB, (ADD_DATA)edit_ctrl_id);
        break;
#endif

    default:
        break;
    }
}

//*****************************************************************************/
//  Description : 设置Edit FORM的param
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void SetEditFormParam(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    MMI_CTRL_ID_T           edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;
    GUI_BG_T        edit_bg = {GUI_BG_NONE,GUI_SHAPE_RECT,IMAGE_NULL,MMI_WHITE_COLOR,FALSE};
    GUI_BORDER_T    edit_border = {1,MMI_BACKGROUND_COLOR,GUI_BORDER_NONE};
    GUI_FONT_ALL_T  all_font = {SONG_FONT_16, MMI_IDLE_DIAL_EDIT_NUM_COLOR};
    GUIEDIT_FONT_T  editfont ={0};
 
    //set im not handle tp
    GUIEDIT_SetImTp(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), FALSE);
    //set delete after full
    GUIEDIT_SetPhoneNumStyle(TRUE, MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id));
    //set display direction
    GUIEDIT_SetPhoneNumDir(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id),GUIEDIT_DISPLAY_DIR_LT);
    GUIEDIT_SetStyle(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), GUIEDIT_STYLE_SINGLE);//@fen.xie
    GUIEDIT_SetAlign(MMK_GetCtrlHandleByWin(win_id,edit_ctrl_id), ALIGN_HVMIDDLE);
    
    GUIFORM_SetChildTheme(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &edit_bg, &edit_border, &all_font);
    GUIFORM_SetActiveChildTheme(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &edit_bg, &edit_border, &all_font);

    ver_space = 0;
    hor_space = 1;
    GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &hor_space, &ver_space);

    child_height.type = GUIFORM_CHILD_HEIGHT_PERCENT;
    child_height.add_data = 100;
    GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), &child_height);

    child_height.type = GUIFORM_CHILD_HEIGHT_PERCENT;
    child_height.add_data = 100;
    GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID), &child_height);

    if (!MMITHEME_IsMainScreenLandscape())
    {
        editfont.is_valid = TRUE;
        editfont.big_font = DIAL_EDIT_FONT_NORMAL;
        if (MMIAPICC_IsDtmfPanel(win_id))
        {
            editfont.small_font = DIAL_EDIT_FONT_NORMAL;
        }
        else
        {
            editfont.small_font = DIAL_EDIT_FONT_SMALL;
        }
        GUIEDIT_SetFontEx(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), &editfont);
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), DIAL_EDIT_FORM_MARGIN);

        child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
        child_width.add_data = 85;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), &child_width);

        child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
        child_width.add_data = 15;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID), &child_width);
     }
    else 
    {
        editfont.is_valid = TRUE;
        editfont.big_font = DIAL_EDIT_FONT_NORMAL_H;
        if (MMIAPICC_IsDtmfPanel(win_id))
        {
            editfont.small_font = DIAL_EDIT_FONT_NORMAL_H;
        }
        else
        {
            editfont.small_font = DIAL_EDIT_FONT_SMALL_H;
        }
        GUIEDIT_SetFontEx(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), &editfont);
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), DIAL_EDIT_FORM_MARGIN_H);

        child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
        child_width.add_data = 80;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), &child_width);

        child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
        child_width.add_data = 20;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID), &child_width);
     } 
}

/*****************************************************************************/
//  Description : 隐藏显示拨号盘中的button
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void SetNumberButtonsVisible(MMI_WIN_ID_T win_id, BOOLEAN is_visible)
{
    int32 row = 0;
    int32 column = 0;

#if !defined(MMI_PDA_SUPPORT)
    if (MMIAPICC_IsDtmfPanel(win_id))
#endif
    {
        GUIFORM_SetVisible(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), is_visible, FALSE);
    }
    if (is_visible)
    {
        MMK_SetAtvCtrl(win_id,MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_EDIT_CTRL_ID));
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_SHOW_BUTTON_CTRL_ID), FALSE, FALSE);
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID), TRUE, FALSE);
    }
    else
    {
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_SHOW_BUTTON_CTRL_ID), TRUE, FALSE);
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID), FALSE, FALSE);
    }

    GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_DIAL_PANEL_COLUMN_LINE_H_CTRL_ID), is_visible, FALSE);
    for (row=0; row<DIAL_NUM_BUTTON_ROW_MAX; row++)
    {
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_ROW_LINE0_CTRL_ID + row), is_visible, FALSE);
        for (column = 0; column<DIAL_NUM_BUTTON_COLUMN_MAX; column++)
        {
            if (row == 0 && column > 0)
            {
                GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_COLUMN_LINE0_CTRL_ID + column), is_visible, FALSE);
            }
            
            if (MMIIDLE_DIAL_BUTTON_MAX != s_mmiidle_dial_num_button_order[row][column])
            {
                GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, s_mmiidle_dial_num_button_ctrl_order[row][column]), is_visible, FALSE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : 创建拨号盘中的button 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialPanelButton(MMI_WIN_ID_T win_id, MMI_HANDLE_T button_handle, MMIIDLE_DIAL_BUTTON_E button_type, const GUI_BOTH_RECT_T *rect_ptr)
{
    GUI_BG_T bg_info = {0};
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    MMI_CONTROL_CREATE_T button_ctrl = {0};
    
    bg_info.bg_type = GUI_BG_IMG;
    SCI_MEMCPY((void*)&button_init.both_rect, (void*)rect_ptr, sizeof(GUI_BOTH_RECT_T));

    button_ctrl.ctrl_id           = button_handle;
    button_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    button_ctrl.init_data_ptr     = &button_init;
    button_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&button_ctrl);
    
    GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id,button_handle), TRUE);
    
    bg_info.img_id = s_mmiidle_dial_button[button_type].bg_img;
    GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);
    bg_info.img_id = s_mmiidle_dial_button[button_type].bg_img_p;
    GUIBUTTON_SetPressedBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);

    bg_info.img_id = s_mmiidle_dial_button[button_type].icon_img;
    GUIBUTTON_SetFg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);    
    bg_info.img_id = s_mmiidle_dial_button[button_type].icon_img_p;
    GUIBUTTON_SetPressedFg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);  
    
    GUIBUTTON_SetRunSheen(MMK_GetCtrlHandleByWin(win_id,button_handle), FALSE);
}

/*****************************************************************************/
//  Description : 创建拨号盘中的编辑框
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateEditForm(MMI_WIN_ID_T win_id, const GUI_BOTH_RECT_T *edit_form_rect_ptr)
{
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    MMI_CTRL_ID_T           edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;
    GUIEDIT_INIT_DATA_T     edit_init = {0};    
    GUI_BOTH_RECT_T client_rect = {0};
    MMI_HANDLE_T    button_handle = 0;
    GUI_BG_T bg_info = {0};
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    SCI_MEMCPY((void*)&form_init.both_rect, (void*)edit_form_rect_ptr, sizeof(GUI_BOTH_RECT_T));

    form_init.layout_type = GUIFORM_LAYOUT_SBS;
    form_ctrl.ctrl_id           = MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_DIAL_EDIT_BG;
    GUIFORM_SetBg( MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &bg_info);

    GUIFORM_IsSlide( MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), FALSE);
    
    SCI_MEMCPY((void*)&form_init.both_rect, (void*)edit_form_rect_ptr, sizeof(GUI_BOTH_RECT_T));

    //edit box
    {
        //phone number edit init data
        edit_init.type        = GUIEDIT_TYPE_PHONENUM;
        edit_init.str_max_len = MMIIDLE_DIAL_MAX_LEN;

         //create phone number edit control
        form_child_ctrl.child_handle           = edit_ctrl_id;
        form_child_ctrl.guid              = SPRD_GUI_EDITBOX_ID;
        form_child_ctrl.init_data_ptr     = &edit_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = TRUE;//an active ctrl!!!
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID),  &form_child_ctrl);

        if (MMIAPICC_IsDtmfPanel(win_id))
        {
            GUIEDIT_SetDtmfNum(edit_ctrl_id, TRUE);
        }
    }

    //dtmf panel not need clipboard
    if (MMIAPICC_IsDtmfPanel(win_id))
    {
        GUIEDIT_SetClipboardEnabled(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), FALSE);
    }

    if (MMIAPICC_IsDtmfPanel(win_id))
    //hide button
    {
        //phone number edit init data
        button_init.bg.bg_type = GUI_BG_IMG;
        button_init.bg.img_id  = s_mmiidle_dial_button[MMIIDLE_DIAL_BUTTON_HIDE_DTMF].icon_img;

        //create phone number edit control
        button_handle = MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID;
        form_child_ctrl.child_handle           = button_handle;
        form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
        form_child_ctrl.init_data_ptr     = &button_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID),  &form_child_ctrl);

        //set handle long press msg
        GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);

        bg_info.img_id = s_mmiidle_dial_button[MMIIDLE_DIAL_BUTTON_HIDE_DTMF].icon_img_p;
        GUIBUTTON_SetPressedFg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);  
        GUIBUTTON_SetRunSheen(MMK_GetCtrlHandleByWin(win_id, button_handle), FALSE);
    }
    else
    //del button
    {
        //phone number edit init data
        button_init.bg.bg_type = GUI_BG_IMG;
        button_init.bg.img_id  = s_mmiidle_dial_button[MMIIDLE_DIAL_BUTTON_DEL].bg_img;

        //create phone number edit control
        button_handle = MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID;
        form_child_ctrl.child_handle           = button_handle;
        form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
        form_child_ctrl.init_data_ptr     = &button_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID),  &form_child_ctrl);

        GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);
 
        bg_info.img_id = s_mmiidle_dial_button[MMIIDLE_DIAL_BUTTON_DEL].bg_img_p;
        GUIBUTTON_SetPressedBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);
        GUIBUTTON_SetRunSheen(MMK_GetCtrlHandleByWin(win_id,button_handle), FALSE);
    }

    SetEditFormParam(win_id);

#if !defined(MMI_PDA_SUPPORT)
    ArrangeEditRect(win_id);
#endif 
#if 0
    if (MMIAPICC_IsDtmfPanel(win_id))
    {
        UILAYER_RESULT_E result  = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO dtmf_panel_layer = {0};
    GUI_BOTH_RECT_T both_rect = {0};

        GUIAPICTRL_GetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &both_rect);
        //if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_dtmf_panel_layer))
        {
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = win_id;
            create_info.offset_x = 0;
            create_info.offset_y = 0;
            create_info.width = MAX((both_rect.v_rect.right - both_rect.v_rect.left), (both_rect.h_rect.right - both_rect.h_rect.left));
            create_info.height = MAX((both_rect.v_rect.bottom - both_rect.v_rect.top), (both_rect.h_rect.bottom - both_rect.h_rect.top));
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE;
            result = UILAYER_CreateLayer(&create_info, &dtmf_panel_layer);

            if(UILAYER_RESULT_SUCCESS == result)
            {
                UILAYER_APPEND_BLT_T append_layer = {0};
                uint16 layer_left = 0;
                uint16 layer_top = 0;
                append_layer.lcd_dev_info.lcd_id = dtmf_panel_layer.lcd_id;
                append_layer.lcd_dev_info.block_id = dtmf_panel_layer.block_id;
                append_layer.layer_level = UILAYER_LEVEL_HIGH;
                UILAYER_AppendBltLayer(&append_layer);

                if(MMITHEME_IsMainScreenLandscape())
                {
                    layer_left = both_rect.h_rect.left;
                    layer_top = both_rect.h_rect.top;
                }
                else
                {
                    layer_left = both_rect.v_rect.left;
                    layer_top = both_rect.v_rect.top;
                }
                   
                UILAYER_SetLayerPosition(&dtmf_panel_layer, layer_left, layer_top);
                UILAYER_WeakLayerAlpha(&dtmf_panel_layer, 0xFF);
                UILAYER_Clear(&dtmf_panel_layer);
                MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &dtmf_panel_layer);
                MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), &dtmf_panel_layer);
                MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID), &dtmf_panel_layer);
            }
        }
    }
    #endif
}

/**240X320 通话中dtmf panel会和其它控件有重合，需要多层来处理**/
#ifndef MAINLCD_SIZE_320X480
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIAPIIDLE_CreateDtmfPanelLayer(MMI_WIN_ID_T win_id)
{

    UILAYER_RESULT_E result  = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO dtmf_panel_layer = {0};
    GUI_BOTH_RECT_T both_rect = {0};
#if 1
    if (MMIAPICC_IsDtmfPanel(win_id))
    {
        GUIAPICTRL_GetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &both_rect);
        //if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_dtmf_panel_layer))
        {
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = win_id;
            create_info.offset_x = 0;
            create_info.offset_y = 0;
            create_info.width = MAX((both_rect.v_rect.right - both_rect.v_rect.left), (both_rect.h_rect.right - both_rect.h_rect.left));
            create_info.height = MAX((both_rect.v_rect.bottom - both_rect.v_rect.top), (both_rect.h_rect.bottom - both_rect.h_rect.top));
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE;
            result = UILAYER_CreateLayer(&create_info, &dtmf_panel_layer);

            if(UILAYER_RESULT_SUCCESS == result)
            {
                UILAYER_APPEND_BLT_T append_layer = {0};
                uint16 layer_left = 0;
                uint16 layer_top = 0;
                append_layer.lcd_dev_info.lcd_id = dtmf_panel_layer.lcd_id;
                append_layer.lcd_dev_info.block_id = dtmf_panel_layer.block_id;
                append_layer.layer_level = UILAYER_LEVEL_HIGH;
                UILAYER_AppendBltLayer(&append_layer);

                if(MMITHEME_IsMainScreenLandscape())
                {
                    layer_left = both_rect.h_rect.left;
                    layer_top = both_rect.h_rect.top;
                }
                else
                {
                    layer_left = both_rect.v_rect.left;
                    layer_top = both_rect.v_rect.top;
                }
                   
                UILAYER_SetLayerPosition(&dtmf_panel_layer, layer_left, layer_top);
                UILAYER_WeakLayerAlpha(&dtmf_panel_layer, 0xFF);
                UILAYER_Clear(&dtmf_panel_layer);
                MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &dtmf_panel_layer);
                MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), &dtmf_panel_layer);
                MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID), &dtmf_panel_layer);
            }
        }
    }
    #endif
    return dtmf_panel_layer;
}    

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetDtmfPanelLayerPosition(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO dtmf_panel_layer )
{
    GUI_BOTH_RECT_T both_rect = {0};
    uint16 layer_left = 0;
    uint16 layer_top = 0;

    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dtmf_panel_layer))
    {
        GUIAPICTRL_GetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &both_rect);
        if(MMITHEME_IsMainScreenLandscape())
        {
            layer_left = both_rect.h_rect.left;
            layer_top = both_rect.h_rect.top;
        }
        else
        {
            layer_left = both_rect.v_rect.left;
            layer_top = both_rect.v_rect.top;
        }
           
        UILAYER_SetLayerPosition(&dtmf_panel_layer, layer_left, layer_top);
    }
}

#endif //MAINLCD_SIZE_320X480


#if !defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 重新放置拨号盘中的编辑框
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void ArrangeEditRect(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T edit_form_rect = {0};
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);

    UpdateDialCoordinate(win_id);
    
    if (!MMITHEME_IsMainScreenLandscape())
    {
        if (s_mmiidle_is_button_panel_close && !MMIAPICC_IsDtmfPanel(win_id))
        {
            edit_form_rect.left = s_panel_rect_coordinate[0][0].x;
            edit_form_rect.right = client_rect.v_rect.right;
            edit_form_rect.top = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][0].y - MMIIDLE_DIAL_LINE_WIDTH - MMIIDLE_DIAL_EDIT_FORM_HEIGHT;
            edit_form_rect.bottom = edit_form_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT - 1;
        }
        else
        { 
            edit_form_rect.left = s_panel_rect_coordinate[0][0].x;
            edit_form_rect.right = client_rect.v_rect.right;
            edit_form_rect.top = s_panel_rect_coordinate[0][0].y - MMIIDLE_DIAL_LINE_WIDTH - MMIIDLE_DIAL_EDIT_FORM_HEIGHT;
            edit_form_rect.bottom = edit_form_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT - 1;
        }
    }
    else
    {
        edit_form_rect.left = s_panel_rect_coordinate_h[0][0].x;
        edit_form_rect.right = client_rect.h_rect.right;
        edit_form_rect.top = s_panel_rect_coordinate_h[0][0].y - MMIIDLE_DIAL_LINE_WIDTH - MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H;
        edit_form_rect.bottom = edit_form_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H - 1;
    }

    GUIFORM_SetRect(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &edit_form_rect);
}
#endif

/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void CreateNumberButtons(MMI_WIN_ID_T win_id)
{
    GUI_BG_T bg_info = {0};
    int32 row = 0;
    int32 column = 0;
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_BOTH_RECT_T line_rect = {0};
    GUI_BOTH_RECT_T edit_form_rect = {0};
    GUI_BOTH_RECT_T     button_both_rect = {0};

    bg_info.bg_type = GUI_BG_IMG;
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);  
    
    edit_form_rect.v_rect.left = s_panel_rect_coordinate[0][0].x;
    edit_form_rect.v_rect.right = client_rect.v_rect.right;
    edit_form_rect.v_rect.top = s_panel_rect_coordinate[0][0].y - MMIIDLE_DIAL_LINE_WIDTH - MMIIDLE_DIAL_EDIT_FORM_HEIGHT;
    edit_form_rect.v_rect.bottom = edit_form_rect.v_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT - 1;
    edit_form_rect.h_rect.left = s_panel_rect_coordinate_h[0][0].x;
    edit_form_rect.h_rect.right = client_rect.h_rect.right;
    edit_form_rect.h_rect.top = s_panel_rect_coordinate_h[0][0].y - MMIIDLE_DIAL_LINE_WIDTH - MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H;
    edit_form_rect.h_rect.bottom = edit_form_rect.h_rect.top + MMIIDLE_DIAL_EDIT_FORM_HEIGHT_H - 1;
    CreateEditForm(win_id, &edit_form_rect);

    //left side line when in landscape case
    SCI_MEMSET(&line_rect, 0x00, sizeof (GUI_BOTH_RECT_T));
    line_rect.h_rect.left = edit_form_rect.h_rect.left - 1;
    line_rect.h_rect.right = line_rect.h_rect.left;
    line_rect.h_rect.top = edit_form_rect.h_rect.top;
    line_rect.h_rect.bottom = client_rect.h_rect.bottom;
    DrawDialPanelLine(win_id, MMIIDLE_DIAL_DIAL_PANEL_COLUMN_LINE_H_CTRL_ID, &line_rect, IMAGE_DIAL_LINE);

    for (row=0; row<DIAL_NUM_BUTTON_ROW_MAX; row++)
    {
        //row line
        {            
            line_rect.v_rect.left = s_panel_rect_coordinate[row][0].x;
            line_rect.v_rect.right = client_rect.v_rect.right;
            line_rect.v_rect.top = s_panel_rect_coordinate[row][0].y - 1;
            line_rect.v_rect.bottom = line_rect.v_rect.top;
            line_rect.h_rect.left = s_panel_rect_coordinate_h[row][0].x;
            line_rect.h_rect.right = client_rect.h_rect.right;
            line_rect.h_rect.top = s_panel_rect_coordinate_h[row][0].y - 1;
            line_rect.h_rect.bottom = line_rect.h_rect.top;
#if !defined(MMI_PDA_SUPPORT)
            if (0 == row)
            {
                DrawDialPanelLine(win_id, MMIIDLE_DIAL_INPUT_ROW_LINE0_CTRL_ID + row, &line_rect, IMAGE_DIAL_LINE);
            }
            else
#endif
            {
                DrawDialPanelLine(win_id, MMIIDLE_DIAL_INPUT_ROW_LINE0_CTRL_ID + row, &line_rect, IMAGE_DIAL_NUMBER_BG);
            }
        }

        for (column = 0; column<DIAL_NUM_BUTTON_COLUMN_MAX; column++)
        {
            //column line
            if (row == 0 && column > 0)
            {
                line_rect.v_rect.left = s_panel_rect_coordinate[0][column].x - 1;
                line_rect.v_rect.right = line_rect.v_rect.left;
                line_rect.v_rect.top = s_panel_rect_coordinate[0][column].y;
                line_rect.v_rect.bottom = client_rect.v_rect.bottom;
                line_rect.h_rect.left = s_panel_rect_coordinate_h[0][column].x - 1;
                line_rect.h_rect.right = line_rect.h_rect.left;
                line_rect.h_rect.top = s_panel_rect_coordinate_h[0][column].y;
                line_rect.h_rect.bottom = client_rect.h_rect.bottom;
                DrawDialPanelLine(win_id, MMIIDLE_DIAL_INPUT_COLUMN_LINE0_CTRL_ID + column, &line_rect, IMAGE_DIAL_NUMBER_BG);
            }

            button_both_rect.v_rect.left = s_panel_rect_coordinate[row][column].x;
            button_both_rect.v_rect.top = s_panel_rect_coordinate[row][column].y;
            if (column < DIAL_NUM_BUTTON_COLUMN_MAX - 1)
            {
                button_both_rect.v_rect.right = s_panel_rect_coordinate[row][column + 1].x - MMIIDLE_DIAL_LINE_WIDTH - 1;
            }
            else
            {
                button_both_rect.v_rect.right = client_rect.v_rect.right;
            }
            button_both_rect.v_rect.bottom = button_both_rect.v_rect.top + MMIIDLE_DIAL_NUMBER_BG_HEIGHT - 1;

            button_both_rect.h_rect.left = s_panel_rect_coordinate_h[row][column].x;
            button_both_rect.h_rect.top = s_panel_rect_coordinate_h[row][column].y;
            if (column < DIAL_NUM_BUTTON_COLUMN_MAX- 1)
            {
                button_both_rect.h_rect.right = s_panel_rect_coordinate_h[row][column + 1].x - MMIIDLE_DIAL_LINE_WIDTH - 1;
            }
            else
            {
                button_both_rect.h_rect.right = client_rect.h_rect.right;
            }
            
            if (MMIAPICC_IsDtmfPanel(win_id))
            {
                button_both_rect.h_rect.bottom = button_both_rect.h_rect.top + MMICC_DTMF_NUMBER_BG_HEIGHT_H - 1;
            }
            else
            {
                button_both_rect.h_rect.bottom = button_both_rect.h_rect.top + MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H - 1;
            }
            
            CreateDialPanelButton(win_id, s_mmiidle_dial_num_button_ctrl_order[row][column], s_mmiidle_dial_num_button_order[row][column], &button_both_rect);
        }
    }     
    if (!MMITHEME_IsMainScreenLandscape()&&!MMIAPICC_IsDtmfPanel(win_id))
    {
        SetNumberButtonsVisible(win_id, !s_mmiidle_is_button_panel_close); /*lint !e730*/
    }
}

/*****************************************************************************/
//  Description : 创建拨号盘中的Dial buttons 控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialButtons(MMI_WIN_ID_T win_id)
{
    GUI_BG_T bg_info = {0};
    int32 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_BOTH_RECT_T line_rect = {0};
    GUI_BOTH_RECT_T     button_both_rect = {0};
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    bg_info.bg_type = GUI_BG_IMG;
        
    //row line 1
    line_rect.v_rect.left = MMI_DIAL_FORM_LEFT;
    line_rect.v_rect.right = client_rect.v_rect.right;
    line_rect.v_rect.top = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][0].y - 1;
    line_rect.v_rect.bottom = line_rect.v_rect.top;
    line_rect.h_rect.left = MMI_DIAL_FORM_LEFT_H;
    line_rect.h_rect.right = client_rect.h_rect.right;
    line_rect.h_rect.top = s_panel_rect_coordinate_h[DIAL_NUM_BUTTON_ROW_MAX][0].y - 1;
    line_rect.h_rect.bottom = line_rect.h_rect.top;
    DrawDialPanelLine(win_id, MMIIDLE_DIAL_INPUT_ROW_LINE5_CTRL_ID, &line_rect, IMAGE_DIAL_LINE);

    for(index = 0; index < DIAL_FUNC_BUTTON_NUM_MAX - 1; index++)
    {   
        button_both_rect.v_rect.left = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][index].x;
        button_both_rect.v_rect.top = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][index].y;
        button_both_rect.v_rect.right = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][index+1].x - 1;
        button_both_rect.v_rect.bottom = button_both_rect.v_rect.top + MMIIDLE_DIAL_BUTTONS_HEIGHT - 1;

        if (0 == index)
        {
            button_both_rect.h_rect.left = s_panel_rect_coordinate_h[DIAL_NUM_BUTTON_ROW_MAX][index].x;
            button_both_rect.h_rect.top = s_panel_rect_coordinate_h[DIAL_NUM_BUTTON_ROW_MAX][index].y;
            button_both_rect.h_rect.right = button_both_rect.h_rect.left + MMIIDLE_DIAL_NUMBER_BG_WIDTH_H - 1;
            button_both_rect.h_rect.bottom = button_both_rect.h_rect.top + MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H - 1;
        }
        else if (1 == index)
        {
            button_both_rect.h_rect.left = s_panel_rect_coordinate_h[DIAL_NUM_BUTTON_ROW_MAX][index].x;
            button_both_rect.h_rect.top = s_panel_rect_coordinate_h[DIAL_NUM_BUTTON_ROW_MAX][index].y;
            button_both_rect.h_rect.right = client_rect.h_rect.right;
            button_both_rect.h_rect.bottom = button_both_rect.h_rect.top + MMIIDLE_DIAL_NUMBER_BG_HEIGHT_H - 1;
        }

#if !defined(MMI_PDA_SUPPORT)
        if (0 == index)
        {
            button_both_rect.h_rect.right += 5;
        }
        else if (1 == index)
        {
            button_both_rect.h_rect.left += 4;
        }
        button_both_rect.h_rect.bottom += 4;
#endif
        CreateDialPanelButton(win_id, s_mmiidle_dial_func_ctrl_order[index], s_mmiidle_dial_func_button_order[index], &button_both_rect);
    }   

    button_both_rect.v_rect.left = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][DIAL_NUM_BUTTON_COLUMN_MAX - 1].x;
    button_both_rect.v_rect.top = s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][DIAL_NUM_BUTTON_COLUMN_MAX - 1].y;
    button_both_rect.v_rect.right = MMI_MAINSCREEN_WIDTH - 1;
    button_both_rect.v_rect.bottom = button_both_rect.v_rect.top + MMIIDLE_DIAL_BUTTONS_HEIGHT - 1;
    button_both_rect.h_rect.left = 0;
    button_both_rect.h_rect.top = 0;
    button_both_rect.h_rect.right = 0;
    button_both_rect.h_rect.bottom = 0;
    CreateDialPanelButton(win_id, MMIIDLE_DIAL_FUNC_SHOW_BUTTON_CTRL_ID, MMIIDLE_DIAL_BUTTON_SHOW_PANEL, &button_both_rect);
    CreateDialPanelButton(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID, MMIIDLE_DIAL_BUTTON_HIDE_PANEL, &button_both_rect);

    if (s_mmiidle_is_button_panel_close)
    {
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_SHOW_BUTTON_CTRL_ID), TRUE, FALSE);
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID), FALSE, FALSE);
    }
    else
    {
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_SHOW_BUTTON_CTRL_ID), FALSE, FALSE);
        GUIBUTTON_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_HIDE_BUTTON_CTRL_ID), TRUE, FALSE);
    }
}

/*****************************************************************************/
//  Description : 更新坐标值等。在发生窗口切换时，尤其是创建过dtmf键盘以后，坐标值可能发生了变化，需要重置
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void UpdateDialCoordinate(MMI_WIN_ID_T win_id)
{
    uint32 row = 0;
    uint32 column = 0;

    if (MMIAPICC_IsDtmfPanel(win_id))
    {
        for (row = 0; row < DIAL_NUM_BUTTON_ROW_MAX; row++)
        {
            for (column = 0; column < DIAL_NUM_BUTTON_COLUMN_MAX; column++)
            {
                if (column == 2)
                {
                    s_panel_rect_coordinate[row][column].x = MMIIDLE_DIAL_OFFSET_X + MMIIDLE_DIAL_OFFSET_M_X;
                }
                else
                {
                    s_panel_rect_coordinate[row][column].x = column*MMIIDLE_DIAL_OFFSET_X;
                }
                s_panel_rect_coordinate[row][column].y = MMICC_DTMF_NUMBER_TOP_Y + row*MMIIDLE_DIAL_OFFSET_Y;

                s_panel_rect_coordinate_h[row][column].x = MMICC_DTMF_LEFT_H+ column*MMICC_DTMF_OFFSET_X_H;
                s_panel_rect_coordinate_h[row][column].y = MMICC_DTMF_NUMBER_TOP_Y_H + row*MMICC_DTMF_OFFSET_Y_H;
            }
        }
    }
    else
    {
        //number buttons
        for (row = 0; row < DIAL_NUM_BUTTON_ROW_MAX + 1; row++)
        {
            for (column = 0; column < DIAL_NUM_BUTTON_COLUMN_MAX; column++)
            {
                if (column == 2)
                {
                    s_panel_rect_coordinate[row][column].x = MMIIDLE_DIAL_OFFSET_X + MMIIDLE_DIAL_OFFSET_M_X;
                }
                else
                {
                    s_panel_rect_coordinate[row][column].x = column*MMIIDLE_DIAL_OFFSET_X;
                }
                s_panel_rect_coordinate[row][column].y = MMIIDLE_DIAL_NUMBER_TOP_Y + row*MMIIDLE_DIAL_OFFSET_Y;

                s_panel_rect_coordinate_h[row][column].x = MMI_DIAL_FORM_LEFT_H+ column*MMIIDLE_DIAL_OFFSET_X_H;
                s_panel_rect_coordinate_h[row][column].y = MMIIDLE_DIAL_NUMBER_TOP_Y_H + row*MMIIDLE_DIAL_OFFSET_Y_H;
            }
        }

        //dial buttons
        {
            uint16 img_height = 0;
            uint16 img_width = 0;
    
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_BUTTON_DIAL, win_id);
            s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][0].x = 0;
            s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][1].x = (MMI_MAINSCREEN_WIDTH - img_width)/2;
            s_panel_rect_coordinate[DIAL_NUM_BUTTON_ROW_MAX][2].x = (MMI_MAINSCREEN_WIDTH - img_width)/2 + img_width;
        }
    }
}

/*****************************************************************************/
//  Description : 创建拨号盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void CreateDialPanel(MMI_WIN_ID_T win_id)
{
    UpdateDialCoordinate(win_id);

    CreateNumberButtons(win_id);
    CreateDialButtons(win_id);
}

/*****************************************************************************/
//  Description : 处理拨号盘按钮收起展开处理
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void HandleDialButtonPanelCloseOrSpread(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T list_rect = {0};
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);   

    s_mmiidle_is_button_panel_close = !s_mmiidle_is_button_panel_close;
     
    list_rect.left = 0;
    list_rect.right = client_rect.v_rect.right;
    list_rect.top = client_rect.v_rect.top;
    if(s_mmiidle_is_button_panel_close)
    {
#if defined(MMI_PDA_SUPPORT)
        list_rect.bottom = MMI_DIAL_OPTION_BUTTON_FORM_TOP - 1;
#else
        list_rect.bottom = MMI_DIAL_OPTION_BUTTON_FORM_TOP - MMIIDLE_DIAL_EDIT_FORM_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1 - 1;
#endif
     }
    else
    { 
         list_rect.bottom = MMI_DIAL_FORM_TOP - 1;
    }
    GUILIST_SetRect(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID), &list_rect);
    
#if !defined(MMI_PDA_SUPPORT)
    ArrangeEditRect(win_id);
#endif

    SetNumberButtonsVisible(win_id, !s_mmiidle_is_button_panel_close); /*lint !e730*/
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

/*****************************************************************************/
//  Description : 创建DTMF键盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CreateDtmfPanel(MMI_WIN_ID_T win_id)
{
    UpdateDialCoordinate(win_id);
    
    CreateNumberButtons(win_id);
}

/*****************************************************************************/
//  Description : 显示DTMF键盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ShowDtmfPanel(MMI_WIN_ID_T win_id)
{
    SetNumberButtonsVisible(win_id, TRUE);
}

/*****************************************************************************/
//  Description : 隐藏DTMF键盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_HideDtmfPanel(MMI_WIN_ID_T win_id)
{
    SetNumberButtonsVisible(win_id, FALSE);
}
/*****************************************************************************/
//  Description : 设置DTMF键盘中的编辑框
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetDtmfEditFormParam(MMI_WIN_ID_T win_id)
{
    SetEditFormParam(win_id);
#ifdef TOUCH_PANEL_SUPPORT    
    GUIFORM_ForceLayout(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID));
#endif
    MMK_WinInactiveCtrl(win_id, FALSE);
    MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID));
}
#else
#if 0
/*****************************************************************************/
//  Description :create  toolbar form ctrl
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void ConstructToolbarButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg_info = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    MMI_HANDLE_T    button_handle = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    form_init.both_rect.v_rect.left = 0;
    form_init.both_rect.v_rect.top = 0;
    form_init.both_rect.v_rect.right = 0;
    form_init.both_rect.v_rect.bottom = 0;

    form_init.both_rect.h_rect.left = MMI_DIAL_TOOLBAR_BUTTON_FORM_LEFT_H;//MMI_DIAL_TOOLBAR_BUTTON_FORM_LEFT_H 252
    form_init.both_rect.h_rect.top = MMI_DIAL_TOOLBAR_BUTTON_FORM_TOP_H ;//MMI_DIAL_TOOLBAR_BUTTON_FORM_TOP_H 54
    form_init.both_rect.h_rect.right = MMI_DIAL_TOOLBAR_BUTTON_FORM_RIGHT_H ;//MMI_DIAL_TOOLBAR_BUTTON_FORM_RIGHT_H 312
    form_init.both_rect.h_rect.bottom = MMI_DIAL_TOOLBAR_BUTTON_FORM_BOTTOM_H ;//MMI_DIAL_TOOLBAR_BUTTON_FORM_BOTTOM_H 92
    
    form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    form_ctrl.ctrl_id           = MMIIDLE_DIAL_TOOLBAR_BUTTON_FORM_CTRL_ID;  //只对竖屏有效 
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);
    
    //not slide
    GUIFORM_IsSlide(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_TOOLBAR_BUTTON_FORM_CTRL_ID),FALSE);

    button_init.bg.bg_type = GUI_BG_IMG;
    button_init.bg.img_id  = s_mmiidle_dial_button[MMIIDLE_DIAL_BUTTON_TOOLBAR].bg_img;

    //create phone number edit control
    form_child_ctrl.child_handle           = MMIIDLE_DIAL_TOOLBAR_FORM_BUTTON_CTRL_ID;
    form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
    form_child_ctrl.init_data_ptr     = &button_init;
    form_child_ctrl.is_bg = FALSE;
    form_child_ctrl.is_get_active = FALSE;
    GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_BUTTON_FORM_CTRL_ID),  &form_child_ctrl);
    GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_TOOLBAR_FORM_BUTTON_CTRL_ID), TRUE);
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = s_mmiidle_dial_button[MMIIDLE_DIAL_BUTTON_TOOLBAR].bg_img_gray;
    //set the gray data to button
    if (bg_info.img_id != NULL)
    {
        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id,button_handle), FALSE, &bg_info, FALSE);            
    }
}

/*****************************************************************************/
//  Description : 创建拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void ConstructNumButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg_info = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    MMIIDLE_DIAL_BUTTON_E button_index = MMIIDLE_DIAL_BUTTON_MAX;
    MMI_HANDLE_T    child_form_handle = 0;
    MMI_HANDLE_T    button_handle = 0;
    int32 row = 0;
    int32 column = 0;
    int32 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    bg_info.bg_type = GUI_BG_IMG;
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    form_init.both_rect.v_rect.left = MMI_DIAL_FORM_LEFT;
    form_init.both_rect.v_rect.top = MMI_DIAL_FORM_TOP;
    form_init.both_rect.v_rect.right = client_rect.v_rect.right - MMI_DIAL_FORM_LEFT;
    form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom;

    form_init.both_rect.h_rect.left = MMI_DIAL_FORM_LEFT_H ;
    form_init.both_rect.h_rect.top = MMI_DIAL_FORM_TOP_H ;
    form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;

    form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    form_ctrl.ctrl_id           = MMIIDLE_DIAL_FORM_CTRL_ID;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);
    
    GUIFORM_IsSlide(MMK_GetCtrlHandleByWin(win_id,  MMIIDLE_DIAL_FORM_CTRL_ID),FALSE);
//@for light function modify
#if defined(MAINLCD_SIZE_240X320) 
    bg_info.img_id = IMAGE_COMMON_BG;
#endif
    GUIFORM_SetBg(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID), &bg_info);

    if(!MMITHEME_IsMainScreenLandscape())
    {
        //number button of dial panel
        child_form_handle = MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_ORDER;
        form_child_ctrl.child_handle           = child_form_handle;
        form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr     = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        for (row=0; row<DIAL_NUM_BUTTON_ROW_MAX; row++)
        {
            child_form_handle = MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID + row;
            
            //SCI_MEMCPY(&form_init.rect, &rect, sizeof(GUI_RECT_T));
            form_init.layout_type = GUIFORM_LAYOUT_SBS;
            //create child form control
            form_child_ctrl.child_handle           = child_form_handle;
            form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
            form_child_ctrl.init_data_ptr     = &form_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID),  &form_child_ctrl);

            for (column = 0; column<DIAL_NUM_BUTTON_COLUMN_MAX; column++)
            {
                button_index = s_mmiidle_dial_num_button_order[row][column];
                button_handle = s_mmiidle_dial_num_button_ctrl_order[row][column];
                if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
                {
                    //phone number edit init data
                    button_init.bg.bg_type = GUI_BG_IMG;
                    button_init.bg.img_id  = s_mmiidle_dial_button[button_index].bg_img;

                    //create phone number edit control
                    form_child_ctrl.child_handle           = button_handle;
                    form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                    form_child_ctrl.init_data_ptr     = &button_init;
                    form_child_ctrl.is_bg = FALSE;
                    form_child_ctrl.is_get_active = FALSE;
                    GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

                    bg_info.img_id = s_mmiidle_dial_button[button_index].bg_img_gray;
                    //set handle long press msg
                    GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);

                    //set the gray data to button
                    if (bg_info.img_id != NULL)
                    {
                        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, button_handle), FALSE, &bg_info, FALSE);            
                    }
                }
            }
        }     

    //create MMIIDLE_DIAL_FUNC_FORM_CTRL_ID and its BUTTON
        child_form_handle = MMIIDLE_DIAL_FUNC_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_SBS;        
        form_child_ctrl.child_handle           = child_form_handle;
        form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr     = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        for(index = 0; index < DIAL_FUNC_BUTTON_NUM_MAX; index++)
        {
            button_index = s_mmiidle_dial_func_button_order[index];
            button_handle = s_mmiidle_dial_func_ctrl_order[index];           

            if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
            {
                //phone number edit init data
                button_init.bg.bg_type = GUI_BG_IMG;
                button_init.bg.img_id  = s_mmiidle_dial_button[button_index].bg_img;

                //create phone number edit control
                form_child_ctrl.child_handle           = button_handle;
                form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                form_child_ctrl.init_data_ptr     = &button_init;
                form_child_ctrl.is_bg = FALSE;
                form_child_ctrl.is_get_active = FALSE;
                GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

                bg_info.img_id = s_mmiidle_dial_button[button_index].bg_img_gray;
                //set handle long press msg
                GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);

                //set the gray data to button
                if (bg_info.img_id != NULL)
                {
                    GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, button_handle), FALSE, &bg_info, FALSE);            
                }
            }        
        }
    }
    else
    {
        for (row=0; row<DIAL_BUTTON_ROW_H_MAX; row++)
        {
            child_form_handle = MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID + row;
            
            //SCI_MEMCPY(&form_init.rect, &rect, sizeof(GUI_RECT_T));
            form_init.layout_type = GUIFORM_LAYOUT_SBS;
            //create child form control
            form_child_ctrl.child_handle           = child_form_handle;
            form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
            form_child_ctrl.init_data_ptr     = &form_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

            for (column = 0; column<DIAL_BUTTON_LINE_H_MAX; column++)
            {
                button_index = s_mmiidle_dial_button_h_order[row][column];
                button_handle = s_mmiidle_dial_button_ctrl_h_order[row][column];
                if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
                {
                    //phone number edit init data
                    button_init.bg.bg_type = GUI_BG_IMG;
                    button_init.bg.img_id  = s_mmiidle_dial_button[button_index].bg_img;

                    //create phone number edit control
                    form_child_ctrl.child_handle           = button_handle;
                    form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                    form_child_ctrl.init_data_ptr     = &button_init;
                    form_child_ctrl.is_bg = FALSE;
                    form_child_ctrl.is_get_active = FALSE;
                    GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

                    bg_info.img_id = s_mmiidle_dial_button[button_index].bg_img_gray;
                    //set handle long press msg
                    GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);

                    //set the gray data to button
                    if (bg_info.img_id != NULL)
                    {
                        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, button_handle), FALSE, &bg_info, FALSE);            
                    }
                }
            }
        }     
    }
}

/*****************************************************************************/
//  Description : 销毁拨号盘中的号码输入控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void DestroyNumButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    int32 row = 0;
    int32 column = 0;
    // int32 index = 0;
    MMI_CTRL_ID_T ctrl_id = 0;
    if(0 != MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FORM_CTRL_ID))
    {
        for (row=0; row<DIAL_BUTTON_ROW_H_MAX; row++)
        {
            for(column=0; column<DIAL_BUTTON_LINE_H_MAX; column++)
            {
                ctrl_id = MMK_GetCtrlHandleByWin(win_id,s_mmiidle_dial_button_ctrl_h_order[row][column]);
                MMK_DestroyDynaCtrl(ctrl_id);
            }
            ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID + row);
            MMK_DestroyDynaCtrl(ctrl_id);
        } 
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID));
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID));
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID));
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_BUTTON_FORM_CTRL_ID));
    }
}
 
/*****************************************************************************/
//  Description : update toolbar
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMIIdle_UpdateWinToolbar(MMI_WIN_ID_T win_id)
{
    GUI_POINT_T pt_bar = {0};

    if(!MMITHEME_IsMainScreenLandscape())
    {
        pt_bar.x = MMIIDLE_TOOLBAR_START_POINT_X;
        pt_bar.y = MMIIDLE_TOOLBAR_START_POINT_Y;    
    }
    else
    {
        pt_bar.x = MMIIDLE_TOOLBAR_START_POINT_X_H;
        pt_bar.y = MMIIDLE_TOOLBAR_START_POINT_Y_H;    
    }

    
    GUITOOLBAR_SetPanelPopForm(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_CTRL_ID), GUITOOLBAR_DIR_UP);
    GUITOOLBAR_SetPanelBasePt(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_CTRL_ID), &pt_bar, FALSE);

    GUITOOLBAR_SetVisible(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_CTRL_ID),TRUE);
    GUITOOLBAR_SetShowType(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_CTRL_ID), GUITOOLBAR_EXPAND);   
         
}

/*****************************************************************************/
//  Description : create toolbar
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMIIdle_CreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    GUITOOLBAR_ITEM_T cl_toolbar_idle_item_a [] = 
    {
        {DIAL_TOOLBAR_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_DIAL_SMS, IMAGE_TOOLBAR_DIAL_SMS_GRAY,0},/*lint !e785*/
        {DIAL_TOOLBAR_CONTACT, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_DIAL_CONTACT, IMAGE_TOOLBAR_DIAL_CONTACT_GRAY,0},/*lint !e785*/
        {DIAL_TOOLBAR_CALLLOG, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_DIAL_CALLLOG, IMAGE_TOOLBAR_DIAL_CALLLOG_GRAY,0},/*lint !e785*/
    };

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);

    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);

    GUITOOLBAR_SetAllItem(MMK_GetCtrlHandleByWin(win_id,ctrl_id), 
        cl_toolbar_idle_item_a, 
        sizeof(cl_toolbar_idle_item_a)/sizeof(GUITOOLBAR_ITEM_T));

    GUITOOLBAR_SetAutoShrinkInterval(MMK_GetCtrlHandleByWin(win_id, ctrl_id),DIAL_TOOLBAR_AUTOSHRINK_TIMER);

} 
/*****************************************************************************/
//  Description : 创建拨号盘中的option button form控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void ConstructOptionButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg_info = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    MMIIDLE_DIAL_BUTTON_E button_index = MMIIDLE_DIAL_BUTTON_MAX;
    MMI_HANDLE_T    child_form_handle = 0;
    MMI_HANDLE_T    button_handle = 0;
    // int32 row = 0;
    // int32 column = 0;
    int32 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    form_init.both_rect.v_rect.left = MMI_DIAL_OPTION_BUTTON_FORM_LEFT;
    form_init.both_rect.v_rect.top = MMI_DIAL_OPTION_BUTTON_FORM_TOP;
    form_init.both_rect.v_rect.right = MMI_DIAL_OPTION_BUTTON_FORM_RIGHT;
    form_init.both_rect.v_rect.bottom = MMI_DIAL_OPTION_BUTTON_FORM_BOTTOM;
    
    form_init.both_rect.h_rect.left = MMI_DIAL_OPTION_BUTTON_FORM_LEFT_H;
    form_init.both_rect.h_rect.top = MMI_DIAL_OPTION_BUTTON_FORM_TOP_H;
    form_init.both_rect.h_rect.right = MMI_DIAL_OPTION_BUTTON_FORM_RIGHT_H;
    form_init.both_rect.h_rect.bottom = MMI_DIAL_OPTION_BUTTON_FORM_BOTTOM_H;
    form_init.layout_type = GUIFORM_LAYOUT_ORDER;    
    form_ctrl.ctrl_id           = MMIIDLE_DIAL_FORM_CTRL_ID;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);

    //not slide
    GUIFORM_IsSlide(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FORM_CTRL_ID),FALSE);

    bg_info.bg_type = GUI_BG_IMG;
    if(!MMITHEME_IsMainScreenLandscape())
    {   

    //create MMIIDLE_DIAL_FUNC_FORM_CTRL_ID and its BUTTON
        child_form_handle = MMIIDLE_DIAL_FUNC_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_SBS;        
        form_child_ctrl.child_handle = child_form_handle;
        form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        for(index = 0; index < DIAL_FUNC_BUTTON_NUM_MAX; index++)
        {
            button_index = s_mmiidle_dial_func_button_order[index];
            button_handle = s_mmiidle_dial_func_ctrl_order[index];         
            
            if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
            {
                //phone number edit init data
                button_init.bg.bg_type = GUI_BG_IMG;
                button_init.bg.img_id  = s_mmiidle_dial_button[button_index].bg_img;

                //create phone number edit control
                form_child_ctrl.child_handle           = button_handle;
                form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                form_child_ctrl.init_data_ptr     = &button_init;
                form_child_ctrl.is_bg = FALSE;
                form_child_ctrl.is_get_active = FALSE;
                GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

                bg_info.img_id = s_mmiidle_dial_button[button_index].bg_img_gray;
                //set handle long press msg
                GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);

                //set the gray data to button
                if (bg_info.img_id != NULL)
                {
                    GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, button_handle), FALSE, &bg_info, FALSE);            
                }
            }        
        }
    }
    else
    {
        child_form_handle = MMIIDLE_DIAL_FUNC_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_ORDER;        
        form_child_ctrl.child_handle = child_form_handle;
        form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        for(index = 0; index < DIAL_FUNC_BUTTON_NUM_MAX; index++)
        {
            button_index = s_mmiidle_dial_func_button_h_order[index];
            button_handle = s_mmiidle_dial_func_ctrl_h_order[index];           

            if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
            {
                //phone number edit init data
                button_init.bg.bg_type = GUI_BG_IMG;
                button_init.bg.img_id  = s_mmiidle_dial_button[button_index].bg_img;

                //create phone number edit control
                form_child_ctrl.child_handle           = button_handle;
                form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                form_child_ctrl.init_data_ptr     = &button_init;
                form_child_ctrl.is_bg = FALSE;
                form_child_ctrl.is_get_active = FALSE;
                GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

                bg_info.img_id = s_mmiidle_dial_button[button_index].bg_img_gray;
                //set handle long press msg
                GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id, button_handle), TRUE);

                //set the gray data to button
                if (bg_info.img_id != NULL)
                {
                    GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, button_handle), FALSE, &bg_info, FALSE);            
                }
            }        
        }
    
    }

}

/*****************************************************************************/
//  Description : 销毁拨号盘中的option button form控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void DestroyOptionButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    int32 index = 0;
    // MMI_CTRL_ID_T ctrl_id = 0;
    if(0 != MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FORM_CTRL_ID))
    {
        for(index = 0; index < DIAL_FUNC_BUTTON_NUM_MAX; index++)
        {
            MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id, s_mmiidle_dial_func_ctrl_order[index]));         
        }      
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FUNC_FORM_CTRL_ID));
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FORM_CTRL_ID));
    }
}

/*****************************************************************************/
// 	Description : set form param
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void SetFormParam(MMI_WIN_ID_T win_id)
{
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    uint16 num_button_width = 0;//0~9,*,#key button image width
    uint16 num_button_height = 0;
    uint16 func_button_width = 0;//ipcall call,del key button image width
    uint16 func_button_height = 0;
    MMI_HANDLE_T button_handle = 0;
    MMI_HANDLE_T form_handle = 0;
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    
    GUIRES_GetImgWidthHeight(&num_button_width, &num_button_height, IMAGE_BUTTON_BLUE_BG_KEY1, win_id);
    GUIRES_GetImgWidthHeight(&func_button_width, &func_button_height, IMAGE_BUTTON_BLUE_BG_DIAL, win_id);
 
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID), 0);
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID), 0);
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID), 0);
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID), 0);
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM1_CTRL_ID), 0);
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM2_CTRL_ID), 0);
    GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_TOOLBAR_BUTTON_FORM_CTRL_ID), 0); 
    
    if (!MMITHEME_IsMainScreenLandscape())
    { 
        ver_space = MMI_DIAL_FORM_VER_SPACE;
        hor_space = MMI_DIAL_FORM_HOR_SPACE;

        //set space
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID), &hor_space, &ver_space);   
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM1_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM2_CTRL_ID), &hor_space, &ver_space); 
    }
    else 
    { 
        int32 row = 0;
        int32 column = 0;    
        ver_space = MMI_DIAL_FORM_VER_SPACE_H;
        hor_space = MMI_DIAL_FORM_HOR_SPACE_H; 

        //set space
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM1_CTRL_ID), &hor_space, &ver_space);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM2_CTRL_ID), &hor_space, &ver_space);

        //set align
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;

        for (row=0; row<DIAL_BUTTON_ROW_H_MAX; row++)
        {
            form_handle = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID + row);
            for (column = 0; column<DIAL_BUTTON_LINE_H_MAX; column++)
            {
                button_handle = MMK_GetCtrlHandleByWin(win_id,s_mmiidle_dial_button_ctrl_h_order[row][column]);
                if((DIAL_BUTTON_LINE_H_MAX - 1) == column)
                {
                    //last button is func button
                    child_width.add_data = func_button_width;
                }
                else
                {
                    child_width.add_data = num_button_width;
                }
                GUIFORM_SetChildWidth(form_handle, button_handle, &child_width);
            }
        }      
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : 创建拨号盘中的分割线
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DrawDialPanelLine(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_BOTH_RECT_T *rect_ptr, uint32 img_id)
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

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = img_id;
    GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id, ctrl_id), TRUE, &bg_info, FALSE);            
}

/*****************************************************************************/
// 	Description : Is IMEI Input
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsImeiIpnut(MMI_STRING_T *str_ptr)
{
    BOOLEAN is_imei_parsered = FALSE;
    uint8 temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};

    if (PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || 0 == str_ptr->wstr_len)
    {
        return FALSE;
    }

    MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
                                str_ptr->wstr_ptr, str_ptr->wstr_len,
                                MIN(str_ptr->wstr_len, MMIIDLE_DIAL_MAX_LEN));


    is_imei_parsered = MMIAPIPARSE_ProcessImeiStr((uint8 *)temp_str, MIN(str_ptr->wstr_len, MMIIDLE_DIAL_MAX_LEN));

    return is_imei_parsered;
}

/*****************************************************************************/
// 	Description : 根据新的号码搜索匹配号码更新显示
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void UpdateSearchResult(MMI_WIN_ID_T win_id, MMI_STRING_T *str_ptr, BOOLEAN is_fresh)
{
    uint16 item_num = 0;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_CTRL_ID_T list_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID);
//    uint16 total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
    uint16 index = 0;
    
    item_t.item_data_ptr = &item_data;

    if(PNULL == str_ptr)
    {
        //SCI_TRACE_LOW:"UpdateSearchResult err para"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_5070_112_2_18_2_23_54_25,(uint8*)"");
        return;
    }

    MMIAPIPB_SearchExit(s_num_search_info.handle);
    SCI_MEMSET(&s_num_search_info, 0x00, sizeof(IDLE_SEARCH_INFO));
    //搜索匹配号码
    s_num_search_info.cl_match_item_num = MMIAPICL_SearchList(str_ptr);
    s_num_search_info.handle = MMIAPIPB_SearchList(str_ptr, MMIPB_LIST_FIELD_NUMBER , &s_num_search_info.pb_match_item_num);
    item_num = s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num;

    //初始化列表
    if(0 == item_num)
    {
        GUILIST_RemoveAllItems(list_ctrl_id);
    
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        GUILIST_SetMaxItem(list_ctrl_id, 1, TRUE );
        GUILIST_AppendItem(list_ctrl_id, &item_t);
//      font.font = MMI_DEFAULT_NORMAL_FONT;
//      font.color = MMI_BLACK_COLOR;
//      GUILIST_SetTextFont(list_ctrl_id, font.font, font.color);
    }
    else
    {
        GUILIST_RemoveAllItems(list_ctrl_id);

        GUILIST_SetMaxItem(list_ctrl_id, item_num, TRUE );
         
		GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_EFFECT_STR, TRUE);
#ifndef MMI_DIAL_MINI_SUPPORT 
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2;
#else
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
 #endif           
        for(index=0; index<item_num; index++)
        {
            GUILIST_AppendItem(list_ctrl_id, &item_t);
        }
     }
    
    if(is_fresh)
    {
//        MMK_PostMsg(list_ctrl_id, MSG_CTL_PAINT, PNULL, 0);
        MMK_SendMsg(list_ctrl_id, MSG_CTL_PAINT, PNULL);
    }
    
}
#if 0
/*****************************************************************************/
// 	Description : call back: list获取anim photo显示
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void DrawDialPanelListItemIconData(void *param)
{    
    MMIPB_BCD_NUMBER_T bcd_num = {0};
    wchar custom_photo_name[MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 2] = {0};
    uint32 custom_photo_name_len = 0;
    uint32 photo_file_size = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    MN_CALLED_NUMBER_T called_num= {0};

    GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
    if(need_item_content_ptr == PNULL )
    {
        return;
    }

    //contact photo
    if (need_item_content_ptr->item_index < s_num_search_info.cl_match_item_num)
    {
        MMIAPICL_GetSearchEntryCalledNumber(&called_num, need_item_content_ptr->item_index);
        {
            SCI_MEMSET(&bcd_num,0x00, sizeof(MMIPB_BCD_NUMBER_T));
            bcd_num.number_len = called_num.num_len;
            bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(called_num.number_type,
              called_num.number_plan);
            MMI_MEMCPY(bcd_num.number,MMIPB_BCD_NUMBER_MAX_LEN,
              called_num.party_num,MMIPB_BCD_NUMBER_MAX_LEN,
              bcd_num.number_len);
        }
    }
    else
    {
        MMIAPIPB_GetSearchEntry(s_num_search_info.handle, PNULL,  MMIPB_MAX_STRING_LEN, &bcd_num, PNULL,(need_item_content_ptr->item_index - s_num_search_info.cl_match_item_num));
    }
   
    if(MMIAPIPB_GetCustomPhotoPathByNumber(&bcd_num,custom_photo_name, &custom_photo_name_len, &photo_file_size))
    {
       item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;     
       anim_path.full_path_wstr_ptr = custom_photo_name;
       anim_path.full_path_wstr_len = custom_photo_name_len;
       item_content.item_data.anim_path_ptr = &anim_path;
       GUILIST_SetItemContent(need_item_content_ptr->ctrl_handle, &item_content, need_item_content_ptr->item_index, need_item_content_ptr->item_content_index); 
    }
}
#endif
/*****************************************************************************/
// 	Description : 插入匹配的通话记录和电话簿信息
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL void AppendMatchingInfo(MMI_WIN_ID_T win_id, void * param)
{
    uint16 index = 0;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
    MMIPB_BCD_NUMBER_T bcd_num = {0};
    MMI_STRING_T num_str = {0};
    MMI_STRING_T name_str = {0};
    MMI_STRING_T temp_name_str = {0};
    wchar num_wchar[MMIPB_MAX_STRING_LEN] = {0};
    wchar name_wchar[MMIPB_MAX_STRING_LEN] = {0};
    wchar temp_name_wchar[MMIPB_MAX_STRING_LEN] = {0};
    MMI_STRING_T edit_str = {0};
    BOOLEAN is_num_valid = FALSE;
    CC_CALL_MODE_E call_mode = CC_CALL_NORMAL;
    BOOLEAN is_emergency_call = FALSE;
#ifndef MMI_DIAL_MINI_SUPPORT 
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T sim_name_str  = {0};
    wchar sim_name_wstr[MMIPB_MAX_STRING_LEN + 1]={0};
#endif
#endif
#if 0
    MMI_STRING_T datetime_str       ={0};
    wchar datetime_wstr[MMIPB_MAX_STRING_LEN + 1] = {0};
    MMI_STRING_T time_str       ={0};
    wchar wstr_time_str[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint16                   call_times = 0;
    uint32              *call_start_time_ptr = PNULL;
#endif
    MMI_CTRL_ID_T list_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID);
    uint16 display_content_index = 0; 

    if (PNULL == param)
    {
        return;
    }

    num_str.wstr_ptr = num_wchar;
    name_str.wstr_ptr = name_wchar;
    temp_name_str.wstr_ptr = temp_name_wchar;
    index = need_item_data_ptr->item_index;

    if(index >= (s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num)
        && (index != 0))
    {
        //SCI_TRACE_LOW:"AppendMatchingInfo err append idx:%d, total idx: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_5223_112_2_18_2_23_54_26,(uint8*)"dd", index, (s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num));
        return;
    }

    item_t.item_data_ptr = &item_data;

    if(0 == (s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num))
    {
        item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_ID;
        GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_EDIT_CTRL_ID), &edit_str);
        if(1 > edit_str.wstr_len)
        {
            //如果搜索字串没有匹配内容，显示"请输入号码"
            item_data.item_content[display_content_index].item_data.text_id= TXT_NO_MATCH_CONTRACT;
        }
        else
        {
            //显示"加入电话簿"
            item_data.item_content[display_content_index].item_data.text_id= TXT_ADD_TO_CONTRACT;
        }
    }
    else
    {
        if(index < s_num_search_info.cl_match_item_num)
        {
            //call log匹配记录
            is_num_valid = MMIAPICL_GetSearchEntryNumber(&num_str, index);
            MMIAPICL_GetSearchCallType(&call_mode, &is_emergency_call, index);

            if(!is_num_valid)
            {
                //SCI_TRACE_LOW:"AppendMatchingInfo err num is invalid"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_5255_112_2_18_2_23_54_27,(uint8*)"");
                return;
            }

            if (is_emergency_call)
            {           
                //icon1
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[display_content_index].item_data.image_id = MMIAPICL_GetCallTypeImageID(is_emergency_call, FALSE, MMIAPICL_GetSearchEntryCallType(index));
                display_content_index++;
                
                //name
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[display_content_index].item_data.text_id = TXT_KL_PSW_EMERGENCY_CALL;
                display_content_index++;
                
                display_content_index++;

                //num
//                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
//                item_data.item_content[display_content_index].item_data.text_buffer= num_str;
            }
            else
            {            
                //call type icon
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[display_content_index].item_data.image_id = MMIAPICL_GetCallTypeImageID(is_emergency_call, FALSE, MMIAPICL_GetSearchEntryCallType(index));
                display_content_index++;
                //name
                if (MMIAPICL_GetSearchEntryName(&name_str, index, TRUE))
                {
                    item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    item_data.item_content[display_content_index].item_data.text_buffer= name_str;
                }
                else
                {
                    if (0 < name_str.wstr_len)//call times
                    {
                        temp_name_str.wstr_len = name_str.wstr_len;
                        SCI_MEMCPY(temp_name_str.wstr_ptr, name_str.wstr_ptr, MMIPB_MAX_STRING_LEN);
                        MMIAPICOM_MergeTwoString(&num_str,&temp_name_str,&name_str,MMIPB_MAX_STRING_LEN);

                        item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                        item_data.item_content[display_content_index].item_data.text_buffer= name_str;
                    }
                    else
                    {
                        item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                        item_data.item_content[display_content_index].item_data.text_buffer= num_str;
                    }
                }
                display_content_index++;
#ifndef MMI_DIAL_MINI_SUPPORT 
                //sim name
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
                SCI_MEMSET(sim_name_wstr,0,sizeof(sim_name_wstr));
                sim_name_str.wstr_ptr = sim_name_wstr;
                sim_name_str.wstr_len = 0;
                MMIAPICL_GetSimNameContent(sim_name_wstr,MMIPB_MAX_STRING_LEN,MMIAPICL_GetSearchEntrySim(index));
                sim_name_str.wstr_len = MMIAPICOM_Wstrlen(sim_name_wstr);
                
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
                item_data.item_content[display_content_index].font_color_id=MMIAPISET_GetSimColorID(MMIAPICL_GetSearchEntrySim(index));
                item_data.item_content[display_content_index].is_default =TRUE;
#endif                 
                
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr =sim_name_str.wstr_ptr;
                item_data.item_content[display_content_index].item_data.text_buffer.wstr_len =sim_name_str.wstr_len;
#else
                //Single don't need sim name
#endif
                display_content_index++;
#endif                
                //num
                item_data.item_content[display_content_index].font_color_id = MMITHEME_COLOR_9;
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[display_content_index].item_data.text_buffer= num_str;
/*
                SCI_MEMSET(&datetime_str,0,sizeof(MMI_STRING_T));
                SCI_MEMSET(&datetime_wstr,0,sizeof(datetime_wstr));            
                datetime_str.wstr_ptr = datetime_wstr;
                SCI_MEMSET(&time_str,0,sizeof(MMI_STRING_T));
                SCI_MEMSET(&wstr_time_str,0,sizeof(wstr_time_str));            
                time_str.wstr_ptr = wstr_time_str;
                MMIAPICL_GetSearchEntryTimes(index, &call_times, &call_start_time_ptr);
                if (0 < call_times)
                {
                    //Add date and time content
                    if (call_start_time_ptr[call_times -1])
                    {
                        GetDateContent(&datetime_str,MMIPB_MAX_STRING_LEN, call_times -1, call_start_time_ptr,TRUE);
                        item_data.item_content[display_content_index].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                        item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = datetime_str.wstr_len;
                        item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = datetime_str.wstr_ptr;
                        display_content_index++;

                        GetTimeContent(&time_str,MMIPB_MAX_STRING_LEN,call_times -1, call_start_time_ptr);
                        item_data.item_content[display_content_index].item_data_type =GUIITEM_DATA_TEXT_BUFFER;
                        item_data.item_content[display_content_index].item_data.text_buffer.wstr_len = time_str.wstr_len;
                        item_data.item_content[display_content_index].item_data.text_buffer.wstr_ptr = time_str.wstr_ptr;
                    }
                }
                
                if (!is_name_valid)
                {
                    //add icon
                    item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data.item_content[display_content_index].item_data.image_id = IMAGE_COMMON_OPTMENU_ADD_CONTACT;
                    GUILIST_SetItemState((GUILIST_ITEM_T*)GUILIST_GetItemPtrByIndex(need_item_data_ptr->ctrl_handle, need_item_data_ptr->item_index),GUIITEM_STATE_CONTENT_CHECK, TRUE);
                }
                else
                {
                    GUILIST_SetItemState((GUILIST_ITEM_T*)GUILIST_GetItemPtrByIndex(need_item_data_ptr->ctrl_handle, need_item_data_ptr->item_index),GUIITEM_STATE_CONTENT_CHECK, FALSE);
                }
*/
            }
        }
        else
        {      
            //pb匹配记录
            uint32 pb_index = index - s_num_search_info.cl_match_item_num;
            uint32 group = 0;
            //is_num_valid = MMIAPIPB_GetSearchEntryNumber(&num_str, &number_type, MMIPB_MAX_STRING_LEN,pb_index);
            MMIAPIPB_GetSearchEntry(s_num_search_info.handle, &name_str,  MMIPB_MAX_STRING_LEN,&bcd_num,&group,pb_index);
            MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &bcd_num);
            if(num_str.wstr_len == 0)
            {
                //SCI_TRACE_LOW:"AppendMatchingInfo err num is invalid"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_5403_112_2_18_2_23_55_28,(uint8*)"");
                return;
            }

            //icon
            item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[display_content_index].item_data.image_id = MMIAPIPB_GetSearchEntryIcon(group);
            display_content_index++;
            
            //name
            if(0 != name_str.wstr_len && PNULL != name_str.wstr_ptr)
            {
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[display_content_index].item_data.text_buffer= name_str;
            }
            else
            {
                item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[display_content_index].item_data.text_id = TXT_NO_NAME;
            }           
            display_content_index++;
#ifndef MMI_DIAL_MINI_SUPPORT  
            //text 2
            display_content_index++;
 #endif           
            //number
            item_data.item_content[display_content_index].font_color_id = MMITHEME_COLOR_9;
            item_data.item_content[display_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[display_content_index].item_data.text_buffer= num_str;
        }
    }

    GUILIST_SetItemData( list_ctrl_id, &item_data, index);    
    GUILIST_SetListState( list_ctrl_id, GUILIST_STATE_ACTIVE, TRUE );
}

#ifdef MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : handle dial menu window msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E Istyle_HandleIdleMatchlistWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       list_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_LISTBOX_CTRL_ID);
    MMI_CTRL_ID_T       edit_ctrl_id = (MMI_CTRL_ID_T )MMK_GetWinAddDataPtr(win_id);
    MMI_STRING_T    edit_str = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 item_num = s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num;
            GUILIST_ITEM_T item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            uint16 index = 0;
            
            item_t.item_data_ptr = &item_data;

            GUILIST_RemoveAllItems(list_ctrl_id);

            GUILIST_SetMaxItem(list_ctrl_id, item_num, TRUE );
             
            GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_EFFECT_STR, TRUE);
#ifndef MMI_DIAL_MINI_SUPPORT 
            item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2;
#else
            item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
#endif           
            for(index=0; index<item_num; index++)
            {
                GUILIST_AppendItem(list_ctrl_id, &item_t);
            }
        
            MMK_SetAtvCtrl(win_id,list_ctrl_id);
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        AppendMatchingInfo(win_id, param);
        break;
        
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT    
    case MSG_NOTIFY_LIST_GET_EFFECT_STR:
        {
            GUILIST_EFFECT_STR_T *effect_str = (GUILIST_EFFECT_STR_T *)param;
            
            if (1 == effect_str->content_index)
            {               
                GUIEDIT_GetString(edit_ctrl_id,&edit_str);

                if( 0 < edit_str.wstr_len && s_num_search_info.cl_match_item_num <= effect_str->item_index)
                {
                    MMI_STRING_T name_str = {0};
                    MMI_STRING_T index_str = {0};
                    wchar name_wchar[MMIPB_MAX_STRING_LEN] = {0};
                    wchar index_wchar[MMIPB_MAX_STRING_LEN] = {0};
                    int index = 0;
                    
                    name_str.wstr_ptr = name_wchar;
                    index_str.wstr_ptr = index_wchar;
                    
                    MMIAPIPB_GetSearchEntry(s_num_search_info.handle ,&name_str, MMIPB_MAX_STRING_LEN, PNULL,PNULL,(effect_str->item_index - s_num_search_info.cl_match_item_num));
                    MMIAPIPB_Wstrstrpos(&name_str, &edit_str, &index_str, FALSE);
                    
                    if (0 < index_str.wstr_len)
                    {
                        effect_str->effect_type = GUILIST_EFFECT_REGION;
                        effect_str->region_num = index_str.wstr_len;
                        effect_str->region_ptr = SCI_ALLOC_APP((index_str.wstr_len) * sizeof(GUIFONT_REGION_STYLE_T));
                        
                        for (index = 0; index < index_str.wstr_len; index++)
                        {
                            effect_str->region_ptr[index].start = index_wchar[index];
                            effect_str->region_ptr[index].length = 1;
                            effect_str->region_ptr[index].state = GUISTR_STATE_SPEC_FONTCOLOR;
                            effect_str->region_ptr[index].font_color = MMI_DARK_GREEN_COLOR;
                        }
                    }
                }

            }
            else if (3 == effect_str->content_index)
            {
                effect_str->font_color = MMI_CYAN_COLOR;
                effect_str->state |= GUISTR_STATE_SPEC_FONTCOLOR;
                effect_str->effect_type = GUILIST_EFFECT_STR;
                
                GUIEDIT_GetString(edit_ctrl_id,&edit_str);
                effect_str->str_info.wstr_len = edit_str.wstr_len;
                effect_str->str_info.wstr_ptr = edit_str.wstr_ptr;
            }
        }
         break;
#endif         

    case MSG_CTL_PENOK:
        //get list num string
        {
            uint16 cur_list_index = 0;
            MMI_STRING_T num_str = {0};
            wchar num_wchar[MMIPB_MAX_STRING_LEN] = {0};
            uint8 list_phone_num_str[MMIIDLE_DIAL_MAX_LEN+1] = {0};
            uint16 list_phone_num_len = 0;    
            MMIPB_BCD_NUMBER_T  pb_num = {0};
    
            num_str.wstr_ptr = num_wchar;
            
            cur_list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            if(s_num_search_info.cl_match_item_num > cur_list_index)
            {
                MMIAPICL_GetSearchEntryNumber(&num_str, cur_list_index);
            }
            else
            {
                MMIAPIPB_GetSearchEntry(s_num_search_info.handle , PNULL, MMIPB_MAX_STRING_LEN, &pb_num, PNULL,(cur_list_index - s_num_search_info.cl_match_item_num));
                MMIAPICOM_BcdToWchar(num_str.wstr_ptr, &num_str.wstr_len, MMIPB_MAX_STRING_LEN, &pb_num);
            }
            list_phone_num_len = num_str.wstr_len;
            MMI_WSTRNTOSTR((uint8 *)list_phone_num_str,MMIIDLE_DIAL_MAX_LEN,
                num_str.wstr_ptr,num_str.wstr_len,
                list_phone_num_len);

            GUIEDIT_SetString(edit_ctrl_id, num_str.wstr_ptr, num_str.wstr_len);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        GUI_INVALID_PARAM(param);/*lint !e522*/
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author: tao.xue
//	Note: 
/*****************************************************************************/
LOCAL uint16 Istyle_GetMatchNum(MMI_WIN_ID_T win_id, MMI_STRING_T *str_ptr)
{
    uint16 item_num = 0;

    if(PNULL == str_ptr || 0 == str_ptr->wstr_len)
    {
        return 0;
    }

    MMIAPIPB_SearchExit(s_num_search_info.handle);
    SCI_MEMSET(&s_num_search_info, 0x00, sizeof(IDLE_SEARCH_INFO));
    //搜索匹配号码
    s_num_search_info.cl_match_item_num = MMIAPICL_SearchList(str_ptr);
    s_num_search_info.handle = MMIAPIPB_SearchList(str_ptr, MMIPB_LIST_FIELD_NUMBER , &s_num_search_info.pb_match_item_num);
    item_num = s_num_search_info.cl_match_item_num + s_num_search_info.pb_match_item_num;

    return item_num;
}

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author: tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_UpdateMatchNum(MMI_WIN_ID_T win_id, MMI_STRING_T *str_ptr)
{
    uint16 item_num = 0;
    MMI_STRING_T match_num_str = {0};
    wchar temp_wchar[5 + 1]={0};
    char temp_char[5 + 1] = {0};
    MMI_CTRL_ID_T arrow_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_MATCH_NUM_ARROW_CTRL_ID);
    match_num_str.wstr_ptr = temp_wchar;
    
    item_num = Istyle_GetMatchNum(win_id, str_ptr);

    if (0 == item_num)
    {
        GUIBUTTON_SetTextId(arrow_ctrl_id, TXT_NULL);
        GUIBUTTON_SetGrayed(arrow_ctrl_id, TRUE, PNULL, FALSE); 
    }
    else
    {
        GUI_FONT_ALL_T font = {SONG_FONT_12, MMI_WHITE_COLOR};
        
        sprintf((char*)temp_char, "%d", item_num);
        match_num_str.wstr_len = strlen((char*)temp_char);
        MMI_STRNTOWSTR(match_num_str.wstr_ptr, match_num_str.wstr_len, (uint8 *)temp_char, match_num_str.wstr_len, match_num_str.wstr_len);
        GUIBUTTON_SetFont(arrow_ctrl_id, &font);
        GUIBUTTON_SetTextAlign(arrow_ctrl_id,  ALIGN_HVMIDDLE);
        GUIBUTTON_SetText(arrow_ctrl_id, match_num_str.wstr_ptr, match_num_str.wstr_len);
        GUIBUTTON_SetGrayed(arrow_ctrl_id, FALSE, PNULL, FALSE); 
    }
}
            
/*****************************************************************************/
// 	Description : Istyle_MMITHEME_GetWinClientBothRect
//	Global resource dependence : 
//    Author:chen yao for istyle 20110720
//	Note: 由于istyle风格的TAB移动到屏幕下方，客户区有所变动
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T Istyle_MMITHEME_GetWinClientBothRect(
                                                     MMI_HANDLE_T win_handle
                                                     )
{
	GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_handle);

    // MMITHEME_GetWinClientBothRect不会扣掉title区域，所以这里不用减
//	client_rect.v_rect.top -= MMI_ISTYLE_TITLE_HEIGHT;
//	client_rect.h_rect.top -= MMI_ISTYLE_TITLE_HEIGHT; 
    
	return client_rect;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: tao.xue
//	Note:   
/*****************************************************************************/
LOCAL void Istyle_HotButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id= 0;
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
    MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    GUISTR_INFO_T       text_info = {0};
    
    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);

    {  
        text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
        text_style.font_color = MMI_WHITE_COLOR;
        text_style.angle = ANGLE_0;
        text_style.char_space = 0;
        text_style.effect = FONT_EFFECT_CUSTOM;
        text_style.edge_color = 0;
#if defined(MAINLCD_SIZE_240X400)||defined(MAINLCD_SIZE_240X320)//@tao.xue
        text_style.font = SONG_FONT_22;
#else
        text_style.font = SONG_FONT_26;
#endif
        text_style.line_space = 0;
        text_style.region_num = 0;
        text_style.region_ptr = PNULL;

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CALL, &str_data);
        GUISTR_GetStringInfo(&text_style, &str_data, state, &text_info);
        
        text_display_rect.left = owner_draw_ptr->display_rect.left + 30;
        text_display_rect.top = owner_draw_ptr->display_rect.top;
        text_display_rect.right=owner_draw_ptr->display_rect.right;
        text_display_rect.bottom=owner_draw_ptr->display_rect.bottom;
    }

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
//  Description : 创建拨号盘中的form控件
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void Istyle_ConstructDialButtonFormCtrl(MMI_WIN_ID_T win_id)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg_info = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    GUIEDIT_INIT_DATA_T     edit_init = {0};    
    MMIIDLE_DIAL_BUTTON_E button_index = MMIIDLE_DIAL_BUTTON_MAX;
    MMI_HANDLE_T    child_form_handle = 0;
    MMI_HANDLE_T    button_handle = 0;
    int32 row = 0;
    int32 column = 0;
    int32 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};
    GUIEDIT_FONT_T  editfont={0};
    
    client_rect = Istyle_MMITHEME_GetWinClientBothRect(win_id);

    //create MMIIDLE_DIAL_FORM_CTRL_ID control
    form_init.both_rect.v_rect = client_rect.v_rect;
    
    form_init.layout_type = GUIFORM_LAYOUT_ORDER;    

    form_ctrl.ctrl_id           = MMIIDLE_DIAL_FORM_CTRL_ID;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);

    GUIFORM_IsSlide(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FORM_CTRL_ID),FALSE);
	
    bg_info.bg_type = GUI_BG_IMG;

    {
    //create MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID and its child form
        child_form_handle = MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_SBS;
        form_child_ctrl.child_handle           = child_form_handle;
        form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr     = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_ISTYLE_DIAL_NUM_BG;
        GUIFORM_SetBg( MMK_GetCtrlHandleByWin(win_id,child_form_handle), &bg_info);
    
        //edit box
        {
            MMI_CTRL_ID_T           edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;

            //phone number edit init data
            edit_init.type        = GUIEDIT_TYPE_PHONENUM;
            edit_init.str_max_len = MMIIDLE_DIAL_MAX_LEN;

            //create phone number edit control
            form_child_ctrl.child_handle           = edit_ctrl_id;
            form_child_ctrl.guid              = SPRD_GUI_EDITBOX_ID;
            form_child_ctrl.init_data_ptr     = &edit_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = TRUE;//an active ctrl!!!
            GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);
                        
            //set im not handle tp
            GUIEDIT_SetImTp(MMK_GetCtrlHandleByWin(win_id,edit_ctrl_id), FALSE);

            //set delete after full
            GUIEDIT_SetPhoneNumStyle(TRUE, MMK_GetCtrlHandleByWin(win_id,edit_ctrl_id));
            
            //set display direction
            GUIEDIT_SetPhoneNumDir(MMK_GetCtrlHandleByWin(win_id,edit_ctrl_id),GUIEDIT_DISPLAY_DIR_LT);            
            GUIEDIT_SetStyle(MMK_GetCtrlHandleByWin(win_id,edit_ctrl_id), GUIEDIT_STYLE_SINGLE);//@fen.xie
            GUIEDIT_SetAlign(MMK_GetCtrlHandleByWin(win_id,edit_ctrl_id), ALIGN_HVMIDDLE);
            editfont.is_valid = TRUE;
            editfont.big_font = DIAL_EDIT_FONT_ISTYLE_NORMAL;
            editfont.small_font = DIAL_EDIT_FONT_ISTYLE_SMALL;
            GUIEDIT_SetFontEx(MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), &editfont);

            //set border & font color & bg
            {
                GUI_BG_T        edit_bg = {GUI_BG_NONE,GUI_SHAPE_RECT,IMAGE_NULL,MMI_WHITE_COLOR,FALSE};
                GUI_BORDER_T    edit_border = {0,MMI_BACKGROUND_COLOR,GUI_BORDER_NONE};
                GUI_FONT_ALL_T  all_font = {CAF_FONT_28, MMI_WHITE_COLOR};
    
                GUIFORM_SetChildTheme(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &edit_bg, &edit_border, &all_font);
                GUIFORM_SetActiveChildTheme(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID), &edit_bg, &edit_border, &all_font);
            }
        }

        {
            MMI_HANDLE_T match_num_arrow_handle = MMIIDLE_DIAL_MATCH_NUM_ARROW_CTRL_ID;
            button_init.bg.bg_type = GUI_BG_IMG;
            button_init.bg.img_id  = s_mmiidle_dial_button_for_istyle[MMIIDLE_DIAL_BUTTON_SHOW_MATCH_ARROW].bg_img;
        
            form_child_ctrl.child_handle           = match_num_arrow_handle;
            form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
            form_child_ctrl.init_data_ptr     = &button_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

            bg_info.img_id  = s_mmiidle_dial_button_for_istyle[MMIIDLE_DIAL_BUTTON_SHOW_MATCH_ARROW].bg_img_p;
            GUIBUTTON_SetPressedBg(MMK_GetCtrlHandleByWin(win_id,match_num_arrow_handle),&bg_info);
            bg_info.img_id  = s_mmiidle_dial_button_for_istyle[MMIIDLE_DIAL_BUTTON_SHOW_MATCH_ARROW].bg_img_gray;
            GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id,match_num_arrow_handle), FALSE, &bg_info, FALSE); 
            GUIBUTTON_SetRunSheen(MMK_GetCtrlHandleByWin(win_id, match_num_arrow_handle), FALSE);
        }
        child_form_handle = MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_ORDER;
        form_child_ctrl.child_handle           = child_form_handle;
        form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr     = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        for (row=0; row<DIAL_NUM_BUTTON_ROW_MAX; row++)
        {
            child_form_handle = MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID + row;
            
            //SCI_MEMCPY(&form_init.rect, &rect, sizeof(GUI_RECT_T));
            form_init.layout_type = GUIFORM_LAYOUT_SBS;
            //create child form control
            form_child_ctrl.child_handle           = child_form_handle;
            form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
            form_child_ctrl.init_data_ptr     = &form_init;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = FALSE;
            GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID),  &form_child_ctrl);

            for (column = 0; column<DIAL_NUM_BUTTON_COLUMN_MAX; column++)
            {
                button_index = s_mmiidle_dial_num_button_order[row][column];
                button_handle = s_mmiidle_dial_num_button_ctrl_order[row][column];
                if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
                {
                    //phone number edit init data
                    button_init.bg.bg_type = GUI_BG_IMG;
                    button_init.bg.img_id  = s_mmiidle_dial_button_for_istyle[button_index].bg_img;

                    //create phone number edit control
                    form_child_ctrl.child_handle           = button_handle;
                    form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                    form_child_ctrl.init_data_ptr     = &button_init;
                    form_child_ctrl.is_bg = FALSE;
                    form_child_ctrl.is_get_active = FALSE;
                    GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,child_form_handle),  &form_child_ctrl);

                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].bg_img_gray;
                    //set handle long press msg
                    GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id,button_handle), TRUE);

                    //set the gray data to button
                    if (bg_info.img_id != NULL)
                    {
                        GUIBUTTON_SetGrayed(MMK_GetCtrlHandleByWin(win_id,button_handle), FALSE, &bg_info, FALSE);            
                    }

                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].bg_img_p;
                    GUIBUTTON_SetPressedBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);     
                    GUIBUTTON_SetRunSheen(MMK_GetCtrlHandleByWin(win_id,button_handle), FALSE);
                }
            }
        }    
       
        child_form_handle = MMIIDLE_DIAL_FUNC_FORM_CTRL_ID;
        form_init.layout_type = GUIFORM_LAYOUT_SBS;
        form_child_ctrl.child_handle           = child_form_handle;
        form_child_ctrl.guid              = SPRD_GUI_FORM_ID;
        form_child_ctrl.init_data_ptr     = &form_init;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FORM_CTRL_ID),  &form_child_ctrl);

        //not slide
        GUIFORM_IsSlide(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID),FALSE);

        bg_info.bg_type = GUI_BG_IMG;
        for(index = 0; index < DIAL_FUNC_BUTTON_NUM_MAX; index++)
        {
            button_index = s_mmiidle_dial_func_button_order_for_istyle[index];
            button_handle = s_mmiidle_dial_func_ctrl_order_for_istyle[index];         
            
            if (MMIIDLE_DIAL_BUTTON_MAX != button_index)
            {
                //phone number edit init data
                //button_init.bg.bg_type = GUI_BG_IMG;
                button_init.bg.img_id  = s_mmiidle_dial_button_for_istyle[button_index].bg_img;

                //create phone number edit control
                form_child_ctrl.child_handle           = button_handle;
                form_child_ctrl.guid              = SPRD_GUI_BUTTON_ID;
                form_child_ctrl.init_data_ptr     = &button_init;
                form_child_ctrl.is_bg = FALSE;
                form_child_ctrl.is_get_active = FALSE;
                GUIFORM_CreatDynaChildCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID),  &form_child_ctrl);

                //set handle long press msg
                GUIBUTTON_SetHandleLong(MMK_GetCtrlHandleByWin(win_id,button_handle), TRUE);

                if (MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID == button_handle)
                {
                    GUI_FONT_ALL_T dial_font = {0};
                    
                    dial_font.font = MMI_DEFAULT_BIG_FONT;
                    dial_font.color = MMI_WHITE_COLOR;

                    GUIBUTTON_SetOwnerDraw(MMK_GetCtrlHandleByWin(win_id,button_handle), Istyle_HotButtonOwnerDrawFun);
                    GUIFORM_PermitChildFont(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID), TRUE);
                    GUIFORM_SetChildTheme(MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_FUNC_FORM_CTRL_ID), PNULL, PNULL, &dial_font);

                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].bg_img;
                    GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);  
                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].bg_img_p;
                    GUIBUTTON_SetPressedBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);
                }
                else
                {
                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].icon_img;
                    GUIBUTTON_SetPressedFg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);  
                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].bg_img_p;
                    GUIBUTTON_SetPressedBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);
                    
                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].icon_img;
                    GUIBUTTON_SetFg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);    
                    bg_info.img_id = s_mmiidle_dial_button_for_istyle[button_index].bg_img;
                    GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id,button_handle),&bg_info);  
                }
                
                GUIBUTTON_SetRunSheen(MMK_GetCtrlHandleByWin(win_id,button_handle), FALSE);
            }        
        }
    
    }
}

/*****************************************************************************/
// 	Description : set form param
//	Global resource dependence : 
//  Author: fen.xie
// chen yao modify for istyle 20110720
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_SetFormParam(MMI_WIN_ID_T        win_id)
{
    uint16 ver_space = 0;
    uint16 hor_space = 0;
    MMI_CTRL_ID_T       ctrl_id; 
    MMI_CTRL_ID_T       child_ctrl_id; 
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    uint16 img_height = 0;
    uint16 img_width = 0;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_ISTYLE_DIAL_NUM_BG, win_id);

    //set MMIIDLE_DIAL_FORM_CTRL_ID
    {
        ctrl_id = MMIIDLE_DIAL_FORM_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, ctrl_id), 0);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, ctrl_id), &hor_space, &ver_space);

        child_ctrl_id = MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID;
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = MMI_MAINSCREEN_WIDTH;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, ctrl_id),MMK_GetCtrlHandleByWin(win_id, child_ctrl_id) , &child_width);

        child_ctrl_id = MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID;
        child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        child_height.add_data = MMI_MAINSCREEN_HEIGHT-(MMI_ISTYLE_TAB_HEIGHT+MMI_ISTYLE_DIAL_CALL_BUTTON_HEIGHT+MMI_ISTYLE_STATUSBAR_HEIGHT+img_height);
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID, &child_height);
    }

    //set MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID
    {
        MMI_CTRL_ID_T form_ctrl_id = MMIIDLE_DIAL_INPUT_EDIT_FORM_CTRL_ID;
        MMI_CTRL_ID_T edit_ctrl_id = MMIIDLE_DIAL_EDIT_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), 0);            
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), &hor_space, &ver_space);

        child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        child_height.add_data = img_height;
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), &child_height);

        child_height.type = GUIFORM_CHILD_HEIGHT_PERCENT;
        child_height.add_data = 100;
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), &child_height);
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_MATCH_NUM_ARROW_CTRL_ID), &child_height);

        child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
 #if defined(MAINLCD_SIZE_320X480)
         child_width.add_data = 88;
 #else
          child_width.add_data = 80;
 #endif
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id), &child_width);
 #if defined(MAINLCD_SIZE_320X480)
        child_width.add_data = 12;
#else
        child_width.add_data = 20;
#endif
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, form_ctrl_id), MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_MATCH_NUM_ARROW_CTRL_ID), &child_width);
    } 

    //set MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID
    {
        ctrl_id = MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, ctrl_id), 0);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, ctrl_id), &hor_space, &ver_space);

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = MMI_MAINSCREEN_WIDTH;       
        child_ctrl_id = MMIIDLE_DIAL_INPUT_NUM_FORM0_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), 0);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &hor_space, &ver_space);
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_width);
        
        child_ctrl_id = MMIIDLE_DIAL_INPUT_NUM_FORM1_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), 0);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &hor_space, &ver_space);
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_width);
        
        child_ctrl_id = MMIIDLE_DIAL_INPUT_NUM_FORM2_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), 0);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &hor_space, &ver_space);
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_width);
        
        child_ctrl_id = MMIIDLE_DIAL_INPUT_NUM_FORM3_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), 0);                    
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &hor_space, &ver_space);	
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_width);
    }

    //set MMIIDLE_DIAL_FUNC_FORM_CTRL_ID
    {
        ctrl_id = MMIIDLE_DIAL_FUNC_FORM_CTRL_ID;
        GUIFORM_SetMargin(MMK_GetCtrlHandleByWin(win_id, ctrl_id), 0);                
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id, ctrl_id), &hor_space, &ver_space);

        child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        child_ctrl_id = MMIIDLE_DIAL_CALL_BUTTON_CTRL_ID;
        child_height.add_data = MMI_ISTYLE_DIAL_CALL_BUTTON_HEIGHT;
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_height);

        child_height.add_data = MMI_ISTYLE_DIAL_CALL_BUTTON_HEIGHT;
        child_ctrl_id = MMIIDLE_DIAL_SAVE_BUTTON_CTRL_ID;
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_height);
        
        child_ctrl_id = MMIIDLE_DIAL_DEL_BUTTON_CTRL_ID;
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, child_ctrl_id), &child_height);

        GUIFORM_SetAlign(MMK_GetCtrlHandleByWin(win_id, ctrl_id), GUIFORM_CHILD_ALIGN_HMIDDLE); 
        GUIFORM_PermitChildFont(MMK_GetCtrlHandleByWin(win_id, ctrl_id),TRUE);
    }
}

/*****************************************************************************/
// 	Description : OpenPdaDialWin
//	Global resource dependence : 
//  Author:tao.xue
// chen yao modify for istyle 20110720
//	Note: 
/*****************************************************************************/
LOCAL void Istyle_OpenPdaDialWin(MMI_WIN_ID_T        win_id)
{
	Istyle_ConstructDialButtonFormCtrl(win_id);
	Istyle_SetFormParam(win_id);
}
/*****************************************************************************/
//  Description : handle dial win message for istyle
//  Global resource dependence : 
//  Author: Jassmine
//  modify:chen yao modify for istyle 20110720
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Istyle_HandleDialWinMsg(
                                    MMI_WIN_ID_T        win_id,
                                    MMI_MESSAGE_ID_E    msg_id,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_STRING_T edit_str = {0};
    MMI_CTRL_ID_T edit_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_EDIT_CTRL_ID);
    MMI_CTRL_ID_T list_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID);
    LOCAL wchar temp_wstr[MMIIDLE_DIAL_MAX_LEN + 1] = {0};
    LOCAL wchar temp_wstr_len = 0;

    MMI_WIN_ID_T info_ptr = (MMI_WIN_ID_T )MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
        Istyle_OpenPdaDialWin(win_id);
		MMK_SetWinSupportAngle( MMK_ConvertIdToHandle(win_id), WIN_SUPPORT_ANGLE_0 );
        MMK_SetAtvCtrl(win_id,edit_ctrl_id);
        
        edit_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_EDIT_CTRL_ID);
        list_ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIIDLE_DIAL_LISTBOX_CTRL_ID);
        SetWinInterceptEditMsg(edit_ctrl_id);
        //Init edit str
        if (s_mmiidle_is_init_dial_str)
        {
            GUIEDIT_SetString(edit_ctrl_id, s_mmiidle_dial_init_tele,  MMIAPICOM_Wstrlen(s_mmiidle_dial_init_tele));
        }

        GUIEDIT_GetString(edit_ctrl_id, &edit_str);
        Istyle_UpdateMatchNum(win_id, &edit_str);
        s_is_need_check_input = TRUE;

        SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
        temp_wstr_len = MIN(MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_len);
        MMI_WSTRNCPY(temp_wstr, MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_ptr, edit_str.wstr_len, temp_wstr_len);
        break;

   case MSG_LCD_SWITCH:
        break;    
        
    case MSG_BACKLIGHT_TURN_ON:           
        MMK_SendMsg(MMK_GetParentWinHandle(win_id), MSG_FULL_PAINT, PNULL);
        break;  
        
    case MSG_IDLE_DIAL_GRAB_PARENT_WIN:
        MMK_WinGrabFocus(info_ptr);
    break;

    case MSG_LOSE_FOCUS:
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIAPIIDLE_StopDialTone();
#endif
        break;


    case MSG_GET_FOCUS:
        {
            GUI_BOTH_RECT_T client_rect = Istyle_MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
            MMK_SetWinRect(MMK_ConvertIdToHandle(win_id), &client_rect.v_rect);
        }
        MMIAPISMS_CheckSMSState(FALSE);
        GUIEDIT_GetString(edit_ctrl_id, &edit_str); 

#ifdef MAINLCD_SIZE_240X320
        if (0 != s_mmiidle_dial_init_key)
        {
            wchar key_code = 0;
            key_code = MMIAPICC_MsgidToAscii(s_mmiidle_dial_init_key);
            
            GUIEDIT_InsertString(edit_ctrl_id,
                                    &key_code,
                                    1);
            s_mmiidle_dial_init_key = 0;
        }
#endif

        MMK_SetAtvCtrl(win_id,edit_ctrl_id);
        GUIWIN_SetTitleIsVisible(win_id, FALSE, FALSE);
        Istyle_UpdateMatchNum(win_id, &edit_str);
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            BOOLEAN is_need_check = FALSE;
            MMI_STRING_T temp_str_info = {0};
            
            GUIEDIT_GetString(edit_ctrl_id, &edit_str);
            
            temp_str_info.wstr_ptr = (wchar *)temp_wstr;
            temp_str_info.wstr_len = temp_wstr_len;

            if (temp_str_info.wstr_len > 0 && edit_str.wstr_len > 0 && (edit_str.wstr_len == temp_str_info.wstr_len + 1))
            {
                if ('#' == edit_str.wstr_ptr[edit_str.wstr_len - 1]
                    && 0 == memcmp(temp_str_info.wstr_ptr, edit_str.wstr_ptr, temp_str_info.wstr_len * sizeof(wchar)))//只有#在最后输入才做匹配
                {
                    is_need_check = TRUE;
                }
            }

            if (!s_is_need_check_input)
            {
                s_is_need_check_input = TRUE;

                is_need_check = FALSE;
            }

            SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
            temp_wstr_len = MIN(MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_len);
            MMI_WSTRNCPY(temp_wstr, MMIIDLE_DIAL_MAX_LEN, edit_str.wstr_ptr, edit_str.wstr_len, temp_wstr_len);

#ifdef MMI_REFACTOR_SAMPLE            
            {
            uint8 temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};

            MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
                                edit_str.wstr_ptr, edit_str.wstr_len,
                                MIN(edit_str.wstr_len, MMIIDLE_DIAL_MAX_LEN));
            if (strncmp((char*)temp_str,"*#9527#",7)==0)
                {                    
                    extern void MMIAPIAPPSAMPLE_Enter(void);
                    MMIAPIAPPSAMPLE_Enter();
#if defined(TOUCH_PANEL_SUPPORT)
                    MMK_CloseParentWin(info_ptr);
#endif
                    MMK_CloseWin(win_id);                    
                    return;
                }        
            }    
#endif

#if defined(DPHONE_SUPPORT)//@ alex added in 2010.12.6
            if (7 < edit_str.wstr_len)
            {
                GUIEDIT_SetFont(edit_ctrl_id, CAF_FONT_14);
            }
#endif
            if (is_need_check && IsImeiIpnut(&edit_str))
            {
                MMIAPIPHONE_OpenIMEIDisplayWin();
#if defined(TOUCH_PANEL_SUPPORT)
                MMK_CloseParentWin(info_ptr);
#endif
                MMK_CloseWin(win_id);
            }
            else if (is_need_check && IsSIMADNInput(&edit_str))
            {
                s_is_need_check_input = FALSE;
            }
            else if (is_need_check && IsPINInput(&edit_str))
            {
#if defined(TOUCH_PANEL_SUPPORT)
                MMK_CloseParentWin(info_ptr);
#endif

                MMK_CloseWin(win_id);
            }
#ifdef ENG_SUPPORT
            else if (is_need_check && IsEngInput(&edit_str))
            {
#if defined(TOUCH_PANEL_SUPPORT)
                MMK_CloseParentWin(info_ptr);
#endif

                MMK_CloseWin(win_id);
            }
#endif
            else if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            } 
            if (MMK_IsFocusWin(win_id))
            {
                Istyle_UpdateMatchNum(win_id, &edit_str);
            }
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_MENU:
        HandleDialOkMsg(edit_ctrl_id);
        break;

    case MSG_CTL_MIDSK:
        HandleDialWebMsg(win_id, edit_ctrl_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(info_ptr);
        MMK_CloseWin(win_id);
        break;

#ifdef MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleDialGreenUpMsg(edit_ctrl_id,MN_DUAL_SYS_MAX);
        break;        
#else /* MMI_MULTI_GREEN_KEY_SINGLE */
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        HandleDialGreenUpMsg(edit_ctrl_id,MMIAPICC_CheckDualSysType( msg_id, MN_DUAL_SYS_MAX));
        break;
#endif /* MMI_MULTI_GREEN_KEY_SINGLE */

#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
        HandleDialVideoTelMsg(edit_ctrl_id);
        break;
#endif

    case MSG_APP_VIDEO_TEL:
        HandleDialVideoTelMsg(edit_ctrl_id);
        break;
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    case MSG_KEYLONG_1:
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);
        if ((0 == edit_str.wstr_len) ||
            (1 == edit_str.wstr_len))
        {
            //cphs 有四个语音号码，暂时只呼叫LINE1语音信箱号码
            MMIAPISMS_OpenCallVMWin(MMI_DUAL_SYS_MAX);
        }
        break;
#endif
    case MSG_KEYLONG_2:
    case MSG_KEYLONG_3:
    case MSG_KEYLONG_4:
    case MSG_KEYLONG_5:
    case MSG_KEYLONG_6:
    case MSG_KEYLONG_7:
    case MSG_KEYLONG_8:
    case MSG_KEYLONG_9:
        //get edit string
        GUIEDIT_GetString(edit_ctrl_id,&edit_str);
        if ((0 == edit_str.wstr_len) ||
            (1 == edit_str.wstr_len))
        {
#if defined(MMI_SPEEDDIAL_SUPPORT) 
            MMIAPICC_SpeedDial(MMI_DUAL_SYS_MAX, (uint16)(msg_id - MSG_KEYLONG_1 + 1));
#endif
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
        HandleDialPenOkMsg(win_id,edit_ctrl_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENLONGOK:
        HandleDialPenLongOkMsg(win_id,edit_ctrl_id,((MMI_NOTIFY_T*)param)->src_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T tp_point = (((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point);

            if (MMIAPIIDLE_IsPressInDialPanel(tp_point))
            {
                MMIAPISET_StopAppRing();
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (0 < s_mmiidle_dial_timer_id)
        {
            StopTPButtonVibrator(s_mmiidle_dial_timer_id, 0);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
        MMK_SendMsg(list_ctrl_id, msg_id, PNULL);
        break;

    case MSG_CLOSE_WINDOW:
        if (0 < s_mmiidle_dial_timer_id)
        {
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */        
            StopTPButtonVibrator(s_mmiidle_dial_timer_id, 0);
#endif
        }
#ifdef DPHONE_SUPPORT
        if (0 < s_hangup_remind_timer_id)
        {
            MMK_StopTimer(s_hangup_remind_timer_id);   
            s_hangup_remind_timer_id = 0;
        }
#endif 

        if (s_mmiidle_is_init_dial_str)
        {
            if (PNULL != s_mmiidle_dial_init_tele)
            {
                SCI_FREE(s_mmiidle_dial_init_tele);
                s_mmiidle_dial_init_tele=NULL;
            }
            s_mmiidle_is_init_dial_str = FALSE;
        } 
                
        MMIAPICL_SearchExit();
        MMIAPIPB_SearchExit(s_num_search_info.handle);
        s_num_search_info.handle = 0;
        break;

    case MSG_TVOUT_CLOSE_IND:
        //结束搜索
        MMIAPICL_SearchExit();
        MMIAPIPB_SearchExit(s_num_search_info.handle);
        s_num_search_info.handle = 0;
#if defined(MMI_PDA_SUPPORT)
        MMK_CloseParentWin(info_ptr);
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_IDLE_DIAL_RESET_EDIT_IND:
        GUIEDIT_ClearAllStrEx(edit_ctrl_id, MMK_IsFocusWin(win_id));
        break;
        
    case MSG_FULL_PAINT:
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif  

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : handle message of sim list window 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SIMSelectADNCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        uint32 record_id = (uint32)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());

        if (CheckSIMADNInput((MN_DUAL_SYS_E)dual_sys, (uint16)record_id))
        {
            s_is_need_check_input = FALSE;
        }
    }

    return recode;
}
#endif

/*****************************************************************************/
// 	Description : Is PIN/PIN2 Input
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPINInput(MMI_STRING_T *str_ptr)
{
    BOOLEAN is_pin_input = FALSE;
    uint8 temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
    
    if (PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || str_ptr->wstr_len <= 5)
    {
        return FALSE;
    }

    //以#结束的号码，如果是PIN操作号码直接执行操作
    if ('#' == str_ptr->wstr_ptr[str_ptr->wstr_len - 1])
    {
        MMI_PIN_INFO_T pin_info = {0};

        MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
                                    str_ptr->wstr_ptr, str_ptr->wstr_len,
                                    MIN(str_ptr->wstr_len, MMIIDLE_DIAL_MAX_LEN));

        if (0 == strncmp((char *)temp_str, "**05*", 5)//**05*PUK*NEW_PIN*NEW_PIN#
            || 0 == strncmp((char *)temp_str, "**04*", 5)//**04*PIN*NEW_PIN*NEW_PIN#
            || 0 == strncmp((char *)temp_str, "**052*", 6)//**052*PUK2*NEW_PIN2*NEW_PIN2#
            || 0 == strncmp((char *)temp_str, "**042*", 6))//**042*PIN2*NEW_PIN2*NEW_PIN2#
        {
            parsErrorCodeE pars_result = MMIAPIPARSE_ParsePukAndPinStr((char *)temp_str, &pin_info);

            if (PARS_RESULT_SUCCESS == pars_result
                || PARS_RESULT_PIN_CONTRADICT == pars_result)
            {
                MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                    temp_str,
                    (uint8)str_ptr->wstr_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_NORMAL_CALL,
                    PNULL);

                is_pin_input = TRUE;
            }
        }
    }

    return is_pin_input;
}

#ifdef ENG_SUPPORT
/*****************************************************************************/
// 	Description : Is Eng Input
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEngInput(MMI_STRING_T *str_ptr)
{
    BOOLEAN is_pin_input = FALSE;
    uint8 temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};

    if (PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || str_ptr->wstr_len < 5)
    {
        return FALSE;
    }

    if ('*' == str_ptr->wstr_ptr[0] || '#' == str_ptr->wstr_ptr[0])
    {
        MMIENG_IDLE_DIAL_NUM_TYPE_E dial_num_type = MMIENG_IDLE_DIAL_NUM_MIN;
        parsCtrlCodeE ctrl_code = PARS_CHANGE_PIN1;
        uint16 temp_str_len = MIN(str_ptr->wstr_len, MMIIDLE_DIAL_MAX_LEN);

        MMI_WSTRNTOSTR((uint8 *)temp_str, MMIIDLE_DIAL_MAX_LEN,
                                    str_ptr->wstr_ptr, str_ptr->wstr_len, temp_str_len
                                    );

        if (MMIAPIENG_ParseIdleDialNumString(temp_str, temp_str_len, &dial_num_type, &ctrl_code))
        {
            MMIAPIENG_DoIdleDialNumOpt(dial_num_type);
            MMIAPIIDLE_CloseDialWin();
            MMIAPIIDLE_CloseDialMenuWin();
            is_pin_input = TRUE;
        }
    }

    return is_pin_input;
}
#endif

/*****************************************************************************/
// 	Description : Is SIM ADN Input
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSIMADNInput(MMI_STRING_T *str_ptr)
{
    BOOLEAN is_sim_adn = FALSE;

    uint8 temp_str[MMIIDLE_DIAL_MAX_LEN + 1]  = {0};
    
    if (PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || str_ptr->wstr_len <= 1 || str_ptr->wstr_len > 4)// 1#---999#
    {
        return FALSE;
    }

    if ('#' == str_ptr->wstr_ptr[str_ptr->wstr_len - 1])
    {
        uint16 i = 0;

        MMI_WSTRNTOSTR((uint8 *)temp_str,MMIIDLE_DIAL_MAX_LEN,
                                    str_ptr->wstr_ptr, str_ptr->wstr_len,
                                    MIN(str_ptr->wstr_len - 1, MMIIDLE_DIAL_MAX_LEN));

        is_sim_adn = TRUE;

        for (i = 0; i < str_ptr->wstr_len - 1; i++)
        {
            if (temp_str[i] < '0' || temp_str[i] > '9')
            {
                is_sim_adn = FALSE;
                break;
            }
        }

        if (is_sim_adn)
        {
            uint16 record_id = 0;
            uint16 sim_card_ok = 0 ;
            uint32 sim_card_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_card_ok, 1);

            if (!MMIAPIPB_IsPbReady())
            {
                MMIPUB_OpenAlertWarningWin(TXT_PB_NOT_READY);
                
                return FALSE;
            }

            record_id = atoi((char *)temp_str);

            if (1 == sim_card_ok_num)
            {
                return CheckSIMADNInput((MN_DUAL_SYS_E)sim_card_ok, record_id);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else if (1 < sim_card_ok_num)
            {
                MMI_WIN_ID_T selectSimWinID = MAIN_SIM_SELECT_WIN_ID;
                MMIPHONE_SELECT_SIM_DATA_T caller_data={0};

                caller_data.append_type = (uint32)MMISET_APPEND_SIM_OK;
                caller_data.callback_func =SIMSelectADNCallback;
                caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;
                caller_data.publist_id = MAIN_SIM_SELECT_CTRL_ID;
                caller_data.win_id_ptr = &selectSimWinID;
                caller_data.user_data = (ADD_DATA)record_id;
                caller_data.extra_data = PNULL;

                MMIAPIPHONE_OpenSelectSimWin(&caller_data);
            }
#endif
            else
            {
                return FALSE;
            }
        }
    }

    return is_sim_adn;
}

/*****************************************************************************/
// 	Description : Check SIM ADN Input
//	Global resource dependence : 
//  Author: fen.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckSIMADNInput(MN_DUAL_SYS_E dual_sys, uint16 record_id)
{
    if (record_id > 0 && record_id < 1000)// 1-999 valid
    {
        ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;
        PHONEBOOK_ENTRY_T *adn_entry_ptr = PNULL;
        uint8 number_str[DIAL_NUM_MAX_LEN + 2] = {0};
        wchar number_wstr[DIAL_NUM_MAX_LEN + 2] = {0};
        uint16 num_len = 0;
        MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;

        fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);

        if (fdn_context_ptr->is_fdn_enable && !fdn_context_ptr->is_usim)
        {
            MMIPUB_OpenAlertWarningWin(TXT_FDN_ACTIVATED);
            
            return FALSE;
        }

        if (!MMIAPIPB_IsPbReady())
        {
            MMIPUB_OpenAlertWarningWin(TXT_PB_NOT_READY);
            
            return FALSE;
        }

        adn_entry_ptr = SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));

        if (PNULL == adn_entry_ptr)
        {
            //SCI_TRACE_LOW:"mainapp: CheckSIMADNInput adn_entry_ptr No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_6280_112_2_18_2_23_56_29,(uint8*)"");

            return FALSE;
        }

        SCI_MEMSET(adn_entry_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));

        adn_entry_ptr->entry_id = record_id;

        err_code = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_SIM_STORAGE, adn_entry_ptr);

        if (ERR_MNDATAMAG_NO_ERR != err_code)
        {
            //SCI_TRACE_LOW:"mainapp: CheckSIMADNInput err_code=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_DIAL_6293_112_2_18_2_23_56_30,(uint8*)"d", err_code);

            MMIPUB_OpenAlertWarningWin(TXT_NO_ADN_LOCATION_ENTRY);

            SCI_FREE(adn_entry_ptr);

            return FALSE;
        }

        num_len = MMIAPICOM_GenDispNumber(adn_entry_ptr->ton,
                                                                    adn_entry_ptr->number_len,
                                                                    adn_entry_ptr->number, 
                                                                    number_str,
                                                                    DIAL_NUM_MAX_LEN + 2);

        num_len = MIN(DIAL_NUM_MAX_LEN, num_len);

        MMI_STRNTOWSTR(number_wstr, num_len, number_str, num_len, num_len);

#if defined(MMI_PDA_SUPPORT)
        {
            MMI_HANDLE_T win_handle = MMK_GetWinHandle(s_mmiidle_pb_applet_handle, MMIIDLE_DIAL_WIN_ID);

            GUIEDIT_SetString(MMK_GetCtrlHandleByWin(win_handle, MMIIDLE_DIAL_EDIT_CTRL_ID), number_wstr, num_len);

            MMK_PostMsg(win_handle, MSG_FULL_PAINT, PNULL, 0);
        }
#else
        GUIEDIT_SetString(MMIIDLE_DIAL_EDIT_CTRL_ID, number_wstr, num_len);

        MMK_PostMsg(MMIIDLE_DIAL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
#endif

        SCI_FREE(adn_entry_ptr);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
/*****************************************************************************/
// 	Description : Character to DTMF Tone Id
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL MMISRVAUD_DTMF_TONE_E Char2DtmfToneId(uint8 dtmf_ch)
{
    MMISRVAUD_DTMF_TONE_E dtmf_tone_id = MMISRVAUD_DTMF_TONE_MAX;

    if(!((dtmf_ch >= '0' && dtmf_ch <= '9') || 
#ifdef DTMF_LETTER_A_B_C_D_SUPPORT
         (dtmf_ch >= 'A' && dtmf_ch <= 'D') ||
#endif
          dtmf_ch == '#' ||
          dtmf_ch =='*')
        )
    {
        return dtmf_tone_id;
    }

    switch(dtmf_ch)
    {
    case '1':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_ONE;
        break;

    case '2':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_TWO;
        break;

    case '3':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_THREE;
        break;

#ifdef DTMF_LETTER_A_B_C_D_SUPPORT
    case 'A':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_LETTERA;
        break;
#endif

    case '4':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_FOUR;
        break;

    case '5':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_FIVE;
        break;

    case '6':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_SIX;
        break;

#ifdef DTMF_LETTER_A_B_C_D_SUPPORT
    case 'B':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_LETTERB;
        break;
#endif

    case '7':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_SEVEN;
        break;

    case '8':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_EIGHT;
        break;

    case '9':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_NINE;
        break;

#ifdef DTMF_LETTER_A_B_C_D_SUPPORT
    case 'C':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_LETTERC;
        break;
#endif

    case '*':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_STAR;
        break;

    case '0':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_ZERO;
        break;

    case '#':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_POND;
        break;

#ifdef DTMF_LETTER_A_B_C_D_SUPPORT
    case 'D':
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_LETTERD;
        break;
#endif

    default:
        dtmf_tone_id = MMISRVAUD_DTMF_TONE_MAX;
        break;
    }

    return dtmf_tone_id;
}

/*****************************************************************************/
// 	Description : Dial panel Play DTMF Tone
//	Global resource dependence : none
//	Author:
//	Note:is_dtmf_input TRUE:需要Press抬起后再关闭Tone
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_PlayDialTone(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_dtmf_input)
{
    uint8 dtmf_ch = 0;
    MMISRVAUD_DTMF_TONE_E dtmf_tone_id = MMISRVAUD_DTMF_TONE_MAX;
    uint8 dial_tone_status = 0;
    MMISRVAUD_ROUTE_T route = 0;
    
    dtmf_ch = MMIAPICC_MsgidToAscii(msg_id);

    dtmf_tone_id = Char2DtmfToneId(dtmf_ch);

    if (MMISRVAUD_DTMF_TONE_MAX == dtmf_tone_id)
    {
        return FALSE;
    }

    dial_tone_status = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, ENVSET_DIAL_TONE_SWITCH);

    if (!dial_tone_status)
    {
        uint32 duration = 200;

#if 0 //暂时不考虑
        if (is_dtmf_input)
        {
            duration = 0xFFFFFFFF;
        }
#endif

        MMIAPISET_StopAppRing();
    
        MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_DTMF);

        //通话中不需要设置模式
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_DTMF, dtmf_tone_id, duration, 0);
        }
        else
        {
            if (MMISRVAUD_IsHeadSetPlugIn())
            {
                route = MMISRVAUD_ROUTE_EARPHONE;
            }
            else
            {
                route = MMISRVAUD_ROUTE_SPEAKER;
            }

            MMISRVAUD_PlayToneWithRoute(route, MMISRVAUD_TYPE_TONE_DTMF, dtmf_tone_id, duration, 0);
        }

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : Stop panel Play DTMF Tone
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_StopDialTone(void)
{
    MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_DTMF);

    return TRUE;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : Is Press In Dial Panel
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsPressInDialPanel(GUI_POINT_T point)
{
    MMI_WIN_ID_T win_id = MMK_GetFocusChildWinId();
    BOOLEAN is_in_dial_rect = FALSE;
#if defined(MMI_PDA_SUPPORT)
    GUI_RECT_T rect_v = {MMI_DIAL_FORM_LEFT, MMIIDLE_DIAL_NUMBER_TOP_Y, MMI_MAINSCREEN_WIDTH - 1, MMI_DIAL_OPTION_BUTTON_FORM_TOP - 1};
    GUI_RECT_T rect_h = {MMI_DIAL_FORM_LEFT_H, MMIIDLE_DIAL_NUMBER_TOP_Y_H, MMI_MAINSCREEN_HEIGHT - 1, MMI_DIAL_OPTION_BUTTON_FORM_TOP_H - 1};
#else
    GUI_RECT_T rect = {0};
#endif

#if defined(MMI_PDA_SUPPORT)
    if(!MMITHEME_IsMainScreenLandscape())
    {
        is_in_dial_rect = GUI_PointIsInRect(point, rect_v);
    }
    else
    {
        is_in_dial_rect = GUI_PointIsInRect(point, rect_h);
    }
#else
    GUIAPICTRL_GetRect(MMK_GetCtrlHandleByWin(win_id, MMIIDLE_DIAL_INPUT_NUM_FORM_CTRL_ID), &rect);
    is_in_dial_rect = GUI_PointIsInRect(point, rect);
#endif

    win_id = MMK_GetWinId(win_id);

    if ((MMK_IsFocusWin(MMIIDLE_DIAL_WIN_ID) || (MMIIDLE_DIAL_WIN_ID == win_id))
           && !s_mmiidle_is_button_panel_close
           && is_in_dial_rect
        )
    {
        return TRUE;
    }

    return FALSE;
}
#endif
#endif



/*Edit by script, ignore 4 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
