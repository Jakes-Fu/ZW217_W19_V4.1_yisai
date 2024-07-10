/*****************************************************************************
** File Name:      mmiacc_id.h                                               *
** Author:         haiyang.hu                                                *
** Date:           2006/09/22                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/22     haiayng.hu        Create                                   *
******************************************************************************/

#ifndef _MMIACC_ID_H_
#define _MMIACC_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmiwclk_export.h"

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

#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
    MMIACC_WIN_ID_START = (MMI_MODULE_ACC << 16),

#include "mmiacc_id.def"
    //计时器主窗口ID
    MMI_TIMER_PARENT_WIN_ID,
	//stopwatch
	MMISTOPWATCH_MAIN_WIN_ID,		// stop watch main win
	//COUNTDOWN
	MMICOUNTDOWN_MAIN_WIN_ID,			// COUNTDOWN main win
	MMICOUNTDOWN_SETTIME_WIN_ID,		// COUNTDOWN set time win
	MMICOUNTDOWN_RINGTYPE_WIN_ID,       // COUNTDOWN ring types win
	MMICOUNTDOWN_ALERT_WIN_ID,          // COUNTDOWN alert win

	//phonesearch
	MMIPHONESEARCH_MAIN_WIN_ID,
	MMIPHONESEARCH_INPUT_WIN_ID,
	MMIPHONESEARCH_POP_QUERY_WIN_ID,
    MMIPHONESEARCH_POP_DOWNLOAD_WIN_ID,

//=============================================================//
//                     For watch                               //
//=============================================================//
    WATCH_STOPWATCH_WIN_ID,
//=============================================================//
//                    For watch                                //
//=============================================================//

#ifdef ADULT_WATCH_SUPPORT
//=============================================================//
//                     For Adult watch                         //
//=============================================================//
    WATCH_FLASHLITHT_WIN_ID,
//=============================================================//
//                    For Adult watch                          //
//=============================================================//
#endif

    MMIACC_MAX_WIN_ID
}MMIACC_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIACC_CTRL_ID_ID_START = MMIACC_MAX_WIN_ID,//(MMI_MODULE_ACC << 16),
    
    MMI_CALC_POPMENU_CTRL_ID,
    MMIACC_CALC_1_CTRL_ID, 
    MMIACC_CALC_2_CTRL_ID, 
    MMIACC_CALC_3_CTRL_ID, 
    MMIACC_CALC_4_CTRL_ID, 
    MMIACC_CALC_5_CTRL_ID, 
    MMIACC_CALC_6_CTRL_ID,
    MMIACC_CALC_7_CTRL_ID, 
    MMIACC_CALC_8_CTRL_ID, 
    MMIACC_CALC_9_CTRL_ID, 
    MMIACC_CALC_STAR_CTRL_ID, 
    MMIACC_CALC_0_CTRL_ID, 
    MMIACC_CALC_HASH_CTRL_ID, 
    MMIACC_CALC_PLUS_CTRL_ID, 
    MMIACC_CALC_SUB_CTRL_ID, 
    MMIACC_CALC_MUL_CTRL_ID, 
    MMIACC_CALC_DIV_CTRL_ID, 
    MMIACC_CALC_EQUAL_CTRL_ID, 

    MMIACC_CALC_CLEAR_CTRL_ID, 
    MMIACC_CALC_BACKSPACE_CTRL_ID, 
    MMIACC_CALC_MPLUS_CTRL_ID, 
    MMIACC_CALC_MMINUS_CTRL_ID, 
    MMIACC_CALC_MCLEAR_CTRL_ID, 
    MMIACC_CALC_MRECALL_CTRL_ID, 

    //由于FORM无法使子控件居中，加入几个空白的子控件
    MMIACC_CALC_EMPTY_CTRL_ID_1,
    MMIACC_CALC_EMPTY_CTRL_ID_2,
    MMIACC_CALC_EMPTY_CTRL_ID_3,
    MMIACC_CALC_EMPTY_CTRL_ID_4,

    MMI_CALC_FORMCTRL_ID,	 
    MMI_CALC_CHILDFORMCTRL1_ID,
    MMI_CALC_CHILDFORMCTRL2_ID,
    MMI_CALC_CHILDFORMCTRL3_ID,
    MMI_CALC_CHILDFORMCTRL4_ID,
    MMI_CALC_CHILDFORMCTRL5_ID,
    MMI_CALC_CHILDFORMCTRL6_ID,
    MMI_CALC_CHILDFORMCTRL7_ID,
    MMI_CALC_CHILDFORMCTRR1_ID,
    MMI_CALC_CHILDFORMCTRR2_ID,
    MMI_CALC_CHILDFORMCTRR3_ID,
    MMI_CALC_CHILDFORMCTRR4_ID,
    MMI_CALC_CHILDFORMCTRR5_ID,
    MMI_CALC_CHILDFORMCTRR6_ID,
	//world clock
    MMIACC_WCLK_OPTION_CTRL_ID,
    MMIACC_WCLK_OPTION_DELETE_FORM_CTRL_ID,
        MMIACC_WCLK_OPTION_DELETE_LIST_CTRL_ID,
        MMIACC_WCLK_OPTION_DELETE_SOFTKEY_CTRL_ID, 
    MMIACC_WCLK_OPTION_SET_DST_FORM_CTRL_ID,
        MMIACC_WCLK_OPTION_SET_DST_LIST_CTRL_ID,
        MMIACC_WCLK_OPTION_SET_DST_SOFTKEY_CTRL_ID, 
    MMIACC_WCLK_ADD_CITY_OPTION_CTRL_ID,
    MMIACC_WCLK_ORDER_LIST_CTRL_ID,
    
//计时器主窗口tab控件
MMI_TIMER_PARENT_TAB_ID,

    //stop watch
    MMIST_BUTTON1_CTRL_ID,
    MMIST_BUTTON2_CRTL_ID,
    MMIST_LIST_CRTL_ID,

//============================================================================//
//                                   For watch                                //
//============================================================================//
    WATCH_STOPWATCH_RESET_BTN_CTRL_ID,
    WATCH_STOPWATCH_PLAY_BTN_CTRL_ID,
    WATCH_STOPWATCH_PAUSE_BTN_CTRL_ID,
//============================================================================//
//                                   For watch                                //
//============================================================================//

    //counted time
    MMICT_TIME_TIMEEDITBOX_CTRL_ID,//@zhaohui,counted time
    MMICT_BUTTON_KEY1_CTRL_ID,
    MMICT_BUTTON_KEY2_CTRL_ID,
    MMICT_BUTTON_KEY3_CTRL_ID,
    MMICT_BUTTON_KEY4_CTRL_ID,
    MMICT_BUTTON_KEY5_CTRL_ID,
    MMICT_BUTTON_KEY6_CTRL_ID,
    MMICT_BUTTON_KEY7_CTRL_ID,
    MMICT_BUTTON_KEY8_CTRL_ID,
    MMICT_BUTTON_KEY9_CTRL_ID,
    MMICT_BUTTON_KEY0_CTRL_ID,
    //MMICT_BUTTON_UP_CTRL_ID,
    //MMICT_BUTTON_DOWN_CTRL_ID,
    MMICT_TIME_BUTTON_CTRL_ID,
    //tvout

	//udisk & sd card managment
	MMIUDISK_MEM_DETAIL_FORM_CTRL_ID,
	MMIUDISK_MEM_DETAIL_LABEL_CTRL_ID,
	MMIUDISK_MEM_DETAIL_ANIM_CTRL_ID,
    //MMIUDISK_MEM_DETAIL_TEXT_CTRL_ID,
    MMIUDISK_DEVICE_SELECT_CTRL_ID,
    MMIUDISK_USB_OPER_SELECT_CTRL_ID,         //Usb operation select control id
	MMIUDISK_UPCC_MAIN_MENU_CTRL_ID,     //usb pcc main menu control id
    MMIUDISK_UPCC_SET_BRIGHTNESS_CTRL_ID,              //pc camera set brightness control id
    MMIUDISK_UPCC_SET_CONTRAST_CTRL_ID,              //pc camera set contrast control id
    MMIUDISK_UPCC_SET_EFFECT_CTRL_ID,              //pc camera set effect control id

	// world time ctrl id
	MMIWT_FORM_CTRL_ID,
        MMIWT_DROPDOWNLIST_GMT_CTRL_ID,
	    MMIWT_DROPDOWNLIST_CITY_CTRL_ID,
        MMIWT_CURRENT_DROPDOWNLIST_FORM_CTRL_ID,
        MMIWT_CURRENT_DATETIME_FORM_CTRL_ID,
            MMIWT_CURRENT_TIME_LABEL_CTRL_ID,
            MMIWT_CURRENT_DATE_LABEL_CTRL_ID,
            MMIWT_CURRENT_SUMMER_ICON_LABEL_CTRL_ID,
        MMIWT_MAP_ANI_CTRL_ID,
        MMIWT_LOCAL_CITY_FORM_CTRL_ID,
            MMIWT_LABEL_LOCAL_CTRL_ID,
            MMIWT_LABEL_LOCAL_CITY_CTRL_ID,
        MMIWT_LOCAL_DATETIME_FORM_CTRL_ID,
            MMIWT_LOCAL_TIME_LABEL_CTRL_ID,
            MMIWT_LOCAL_DATE_LABEL_CTRL_ID,
            MMIWT_LOCAL_SUMMER_ICON_LABEL_CTRL_ID,

    //new worldtime ctrl id
        MMIWT_ADD_CITY_FORM_CTRL_ID,
            MMIWT_BOTTON_ADD_CITY_ID,
            MMIWT_LABEL_ADD_CITY_ID,
           
        MMIWT_CITY_1_FORM_CTRL_ID,
            MMIWT_CITY_1_INFO_FORM_CTRL_ID,
                MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID,
                    MMIWT_CITY_1_INFO_CITY_CTRL_ID,
                    MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,                    
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT 
                    MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID,
                MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID,
#endif
                MMIWT_CITY_1_INFO_TIME_CTRL_ID,
                MMIWT_CITY_1_INFO_DATE_CTRL_ID,
            MMIWT_CITY_1_CLOCK_CTRL_ID,
        #if 0    
        MMIWT_CITY_2_FORM_CTRL_ID,
            MMIWT_CITY_2_INFO_FORM_CTRL_ID,
                MMIWT_CITY_2_INFO_CITY_FORM_CTRL_ID,
                    MMIWT_CITY_2_INFO_CITY_CTRL_ID,
                    MMIWT_CITY_2_INFO_SUMMER_ICON_CTRL_ID,                    
                MMIWT_CITY_2_INFO_TIME_CTRL_ID,
                MMIWT_CITY_2_INFO_DATE_CTRL_ID,
            MMIWT_CITY_2_CLOCK_CTRL_ID,

       MMIWT_CITY_3_FORM_CTRL_ID,
          MMIWT_CITY_3_INFO_FORM_CTRL_ID,
                MMIWT_CITY_3_INFO_CITY_FORM_CTRL_ID,
                    MMIWT_CITY_3_INFO_CITY_CTRL_ID,
                    MMIWT_CITY_3_INFO_SUMMER_ICON_CTRL_ID,                    
                MMIWT_CITY_3_INFO_TIME_CTRL_ID,
                MMIWT_CITY_3_INFO_DATE_CTRL_ID,
          MMIWT_CITY_3_CLOCK_CTRL_ID,

       MMIWT_CITY_4_FORM_CTRL_ID,
          MMIWT_CITY_4_INFO_FORM_CTRL_ID,
                MMIWT_CITY_4_INFO_CITY_FORM_CTRL_ID,
                    MMIWT_CITY_4_INFO_CITY_CTRL_ID,
                    MMIWT_CITY_4_INFO_SUMMER_ICON_CTRL_ID,                    
                MMIWT_CITY_4_INFO_TIME_CTRL_ID,
                MMIWT_CITY_4_INFO_DATE_CTRL_ID,
          MMIWT_CITY_4_CLOCK_CTRL_ID,
       #endif
    MMIWT_ADD_CITY_LIST_CTRL_ID =MMIWT_CITY_1_CLOCK_CTRL_ID+(MMIWT_CITY_1_CLOCK_CTRL_ID-MMIWT_CITY_1_FORM_CTRL_ID+1)*WT_DEFAULT_CITY_NUM,
#ifdef STOPWATCH_SUPPORT
	// stop watch ctrl id
	MMISTOPWATCH_MAIN_WIN_LABEL_CTRL_ID,    // stop watch main wnd label id
	MMISTOPWATCH_MAIN_WIN_LIST_CTRL_ID,    // stop watch main wnd list id
	MMISTOPWATCH_MAIN_WIN_LAP_CTRL_MIN_ID,     // stop watch main wnd lap label id
	MMISTOPWATCH_MAIN_WIN_LAP_CTRL_MAX_ID=MMISTOPWATCH_MAIN_WIN_LAP_CTRL_MIN_ID+8,     // stop watch main wnd lap label id
	MMISTOPWATCH_MAIN_WIN_DOT_CTRL_ID,     // stop watch main wnd dot label id
#endif
	MMICOUNTDOWN_SETTIME_EDIT_ID,			// countdown set time edit ID
	MMICOUNTDOWN_RINGTYPE_LIST_ID,			// countdown ring type list ID
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
    MMICT_TIME_TITLE_CTRL_ID,
#endif
  
	MMIPHONESEARCH_MAIN_LIST_ID,			// phone search list ID
	MMIPHONESEARCH_MENU_CTRL_ID,
	MMIPHONESEARCH_INPUT_EDIT_ID,			// phone search input edit ID
	MMIPHONESEARCH_POP_MENU_ID,

	//unit conversion ctrl id
        MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID,
        MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID,
        MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,

	MMIUNITCONV_UNIT_LABEL1_CTRL_ID,
	MMIUNITCONV_UNIT_LABEL2_CTRL_ID,
        MMIUNITCONV_EDIT1_CTRL_ID,
        MMIUNITCONV_EDIT2_CTRL_ID,
        MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID,
        MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID,
        MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID,
        MMIUNITCONV_EDIT_PICKER1_CTRL_ID,
        MMIUNITCONV_EDIT_PICKER2_CTRL_ID,
        MMIUNITCONV_CHANGE_TYPE_CTRL_ID,
        MMIUNITCONV_CHANGE_CONVERSION_TYPE_CTRL_ID,

#if defined(ZDT_TOOLS_MENU_SUPPORT)// wuxx add.
        MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID,
#endif

    MMIACC_MAX_CTRL_ID
}MMIACC_CONTROL_ID_E;

//menu id
typedef enum
{
    MMI_PHONESEARCH_POP_START_MENU_ID,
    MMI_PHONESEARCH_POP_QUERY_MENU_ID,
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    MMI_PHONESEARCH_POP_DOWNLOAD_MENU_ID,    
#endif
    MMI_PHONESEARCH_MAX_MENU_ID
}MMI_PHONESEARCH_MENU_ID_E;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIACC_RegWinIdNameArr(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIACC_ID_H_
