/*****************************************************************************
** File Name:      mmiim_wintab.c                                                   *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIIM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_im_trc.h"
#include "mmk_type.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guires.h"
#include "guibutton.h"
#include "window_parse.h"
//#include "mmiim_menutable.h"
#include "mmiim_internal.h"
//#include "mmiim_nv.h"
#include "mmiim_id.h"
#include "mmiim_text.h"
#include "mmidisplay_data.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "guiform.h"
#include "guisetlist.h"
#include "wdp_update_wintab.h"

#if defined MMI_IM_PDA_SUPPORT
#include "mmiset_wintab.h"
#include "mmiset_func.h"
#include "mmiim_im_switching.h"
#include "mmiim.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if defined IM_HANDWRITING
#define MMIIM_SET_HW_COLOR_FRAME_COLOR  MMI_BLACK_COLOR//MMI_GRAY_WHITE_COLOR

#define MMIIM_SET_HW_COLOR_TOP_PER      10 //这是上边突出的比例 百分比
#define MMIIM_SET_HW_COLOR_LEFT_PER     8 //这是左边突出的比例
#define MMIIM_SET_HW_COLOR_HEIGHT       23 //高度是固定的
#define MMIIM_SET_HW_COLOR_GAP          2

#define MMIIM_SET_HW_COLOR_PRE_TOP_PER  60 //这是上边突出的比例
#define MMIIM_SET_HW_COLOR_PRE_HEIGHT   23 //高度是固定的
#define MMIIM_SET_HW_COLOR_FREE_HEIGHT  32 //高度是固定的

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIIM_HW_COLOR_BUTTON_T s_mmiim_hw_color_button[MMIIM_HW_COLOR_MAX] = 
{
    //button 1
    {
        {0},
        {0},
        MMI_DARK_BLUE_COLOR,
        MMI_DARK_BLUE_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON1_CTRL_ID
    },
    //button 2
    {
        {0},
        {0},
        MMI_DARK_RED_COLOR,
        MMI_DARK_RED_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON2_CTRL_ID
    },
    //button 3
    {
        {0},
        {0},
        MMI_DARK_CYAN_COLOR,
        MMI_DARK_CYAN_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON3_CTRL_ID
    },
    //button 4
    {
        {0},
        {0},
        MMI_DARK_GREEN_COLOR,
        MMI_DARK_GREEN_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON4_CTRL_ID
    },
    //button 5
    {
        {0},
        {0},
        MMI_BLUE_COLOR,
        MMI_BLUE_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON5_CTRL_ID
    },
    //button 6
    {
        {0},
        {0},
        MMI_CYAN_COLOR,
        MMI_CYAN_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON6_CTRL_ID
    },
    //button 7
    {
        {0},
        {0},
        MMI_GREEN_COLOR,
        MMI_GREEN_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON7_CTRL_ID
    },
    //button 8
    {
        {0},
        {0},
        MMI_DARK_GRAY_COLOR,
        MMI_DARK_GRAY_COLOR,
        MMIIM_SET_HW_COLOR_BUTTON8_CTRL_ID
    }
};
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

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
//  Description : handle im set menu window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetMenuWin(
                                      MMI_WIN_ID_T      win_id, 
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM            param
                                      );

/*****************************************************************************/
//  Description : handle im set pinyin fuzzy window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetPinyinFuzzyWin(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description :append im pinyin fuzzy item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AppendImPyFuzzyItem(
                               MMI_CTRL_ID_T    ctrl_id
                               );

/*****************************************************************************/
//  Description :set im pinyin fuzzy
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetImPyFuzzy(
                        MMI_CTRL_ID_T   ctrl_id
                        );

#if defined IM_HANDWRITING
/*****************************************************************************/
//  Description : handle im set handwriting speed window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetHwSpeedWin(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description :append im handwriting speed item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AppendImHwSpeedItem(
                               MMI_CTRL_ID_T    ctrl_id
                               );

/*****************************************************************************/
//  Description :set im handwriting speed
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetImHwSpeed(
                        MMI_CTRL_ID_T   ctrl_id
                        );

/*****************************************************************************/
//  Description : handle im set handwriting thickness window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetHwThicknessWin(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description :append im handwriting thickness item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AppendImHwThicknessItem(
                                   MMI_CTRL_ID_T    ctrl_id
                                   );

/*****************************************************************************/
//  Description :set im handwriting thickness
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetImHwThickness(
                            MMI_CTRL_ID_T   ctrl_id
                            );

/*****************************************************************************/
//  Description : handle im set handwriting color window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetHwColorWin(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : init handwriting color button
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void InitHwColorButton(
                             MMI_WIN_ID_T   win_id
                             );

/*****************************************************************************/
//  Description : get current handwriting color button index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetHwColorButtonIndex(
                                   GUI_COLOR_T  hw_color
                                   );

/*****************************************************************************/
//  Description : draw focus rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DrawHwColorButtonFocusRect(
                                      uint32    button_index
                                      );

/*****************************************************************************/
//  Description : set handwriting color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetHwColor(
                      uint32    button_index
                      );

/*****************************************************************************/
//  Description : handle handwriting color pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: if not click on color buttons, retrun false
/*****************************************************************************/
LOCAL BOOLEAN HandleHwColorPenOkMsg(//button index
                                    MMI_CTRL_ID_T    ctrl_id,
                                    uint32*          button_idx_ptr
                                    );
#endif
/*****************************************************************************/
//  Description : set py fuzzy label param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPYFuzzyParam(void);

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
LOCAL void SetSogouSettingParam(void);
#endif

#if defined MMI_IM_PDA_SUPPORT
LOCAL void SetInputLangParam(void);
#endif
#if defined IM_HANDWRITING
/*****************************************************************************/
//  Description : set handwritting speed param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetHandwriteSpeedParam(void);

/*****************************************************************************/
//  Description : set handwritting thick param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetHandwriteThickParam(void );

/*****************************************************************************/
//  Description : set handwritting color param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetHandwriteColorParam(void);
#endif
#ifdef MMI_IM_PDA_SUPPORT
LOCAL void SetKeyboardSizeParam(void);
LOCAL void SetImKeyboardSize(MMI_CTRL_ID_T ctrl_id);
#endif

/*****************************************************************************/
//  Description : save all setting(im)
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveIMSet(void);


/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//im set window
WINDOW_TABLE( MMIIM_SET_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleImSetMenuWin),
    WIN_ID(MMIIM_SET_WIN_ID),
    WIN_TITLE(TXT_SET_INPUT_MODE),
    //CREATE_MENU_CTRL(MMIIM_SET_MENU, MMIIM_SET_MENU_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIM_SET_FORM_CTRL_ID),

//sogou不支持模糊音
#if (defined IM_ENGINE_CSTAR) && (defined IM_SIMP_CHINESE_SUPPORT ||defined IM_TRAD_CHINESE_SUPPORT)   
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_PY_FUZZY_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIIM_SET_PY_FUZZY_LABEL_CTRL_ID, MMIIM_SET_PY_FUZZY_FORM_CTRL_ID),
#endif

#ifdef TOUCH_PANEL_SUPPORT
#if defined IM_HANDWRITING
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_HANDWRITE_SPEED_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIIM_SET_HANDWRITE_SPEED_LABEL_CTRL_ID, MMIIM_SET_HANDWRITE_SPEED_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE, MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID, MMIIM_SET_HANDWRITE_SPEED_FORM_CTRL_ID),


    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_HANDWRITE_THICK_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIIM_SET_HANDWRITE_THICK_LABEL_CTRL_ID, MMIIM_SET_HANDWRITE_THICK_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE, MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID, MMIIM_SET_HANDWRITE_THICK_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_HANDWRITE_COLOR_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIIM_SET_HANDWRITE_COLOR_LABEL_CTRL_ID, MMIIM_SET_HANDWRITE_COLOR_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIIM_SET_HANDWRITE_COLOR_SETTING_LABEL_CTRL_ID, MMIIM_SET_HANDWRITE_COLOR_FORM_CTRL_ID),
#endif
#endif

#ifdef MMI_IM_PDA_SUPPORT
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_KEYBOARD_SIZE_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIIM_SET_KEYBOARD_SIZE_LABEL_CTRL_ID, MMIIM_SET_KEYBOARD_SIZE_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE, MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, MMIIM_SET_KEYBOARD_SIZE_FORM_CTRL_ID),
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_SOGOU_SETTING_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIIM_SET_SOGOU_SETTING_LABEL_CTRL_ID, MMIIM_SET_SOGOU_SETTING_FORM_CTRL_ID),
#endif

    //这里有一个潜规则，当输入语言少于3个时，则该菜单项不会显示，所以使用时小心
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIIM_SET_INPUT_LANG_FORM_CTRL_ID, MMIIM_SET_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIIM_SET_INPUT_LANG_LABEL_CTRL_ID, MMIIM_SET_INPUT_LANG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIIM_SET_INPUT_LANG_CONTENT_LABEL_CTRL_ID, MMIIM_SET_INPUT_LANG_FORM_CTRL_ID),

#endif

    WIN_SOFTKEY(STXT_SELECT,TXT_NULL,STXT_FINISH),
    END_WIN
};

//im set pinyin fuzzy 
WINDOW_TABLE( MMIIM_SET_PINYIN_FUZZY_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleImSetPinyinFuzzyWin ),    
    WIN_ID(MMIIM_SET_PY_FUZZY_WIN_ID ),
#ifndef MMI_PDA_SUPPORT
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIIM_PINYIN_FUZZY_WIN_FORM_ID),
    WIN_TITLE(IM_TXT_INPUT_PY_FUZZY_SET),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMIIM_SET_PY_FUZZY_LIST_CTRL_ID),
    //如果有人需要有左右软键，则需要打开下面
    //CHILD_LIST_CTRL(TRUE, GUILIST_CHECKLIST_E, MMIIM_SET_PY_FUZZY_LIST_CTRL_ID, 
    //    MMIIM_PINYIN_FUZZY_WIN_FORM_ID),
    END_WIN
};
#if defined IM_HANDWRITING
//im set handwriting speed 
WINDOW_TABLE( MMIIM_SET_HW_SPEED_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleImSetHwSpeedWin ),    
    WIN_ID( MMIIM_SET_HW_SPEED_WIN_ID ),
    WIN_TITLE( IM_TXT_INPUT_HANDWRITING_SPEED_SET ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIIM_SET_HW_SPEED_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//im set handwriting thickness 
WINDOW_TABLE( MMIIM_SET_HW_THICKNESS_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleImSetHwThicknessWin ),    
    WIN_ID( MMIIM_SET_HW_THICKNESS_WIN_ID ),
    WIN_TITLE( IM_TXT_INPUT_HANDWRITING_THICKNESS_SET ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIIM_SET_HW_THICKNESS_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//im set handwriting color 
WINDOW_TABLE( MMIIM_SET_HW_COLOR_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleImSetHwColorWin ),    
    WIN_ID( MMIIM_SET_HW_COLOR_WIN_ID ),
    WIN_TITLE( IM_TXT_INPUT_HANDWRITING_COLOR_SET ),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIIM_SET_HW_COLOR_PREVIEW_LABEL_CTRL_ID),  
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIIM_SET_HW_COLOR_TEXT_LABEL_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open im set window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIIM_OpenSetWin(void)
{
    MMI_HANDLE_T win_handle = 0;

    //需不要创建菜单，菜单和"打开触屏，关掉TP输入法"无关，有TP无输入法，则会要求
    //按原来的逻辑创建fuzzy
#ifdef TOUCH_PANEL_SUPPORT
    #if (defined MAINLCD_DEV_SIZE_320X240) && (defined KEYPAD_TYPE_QWERTY_KEYPAD)
        #if (!defined IM_ENGINE_SOGOU) && (defined IM_SIMP_CHINESE_SUPPORT || (defined IM_TRAD_CHINESE_SUPPORT))
            win_handle = MMK_CreateWin((uint32 *)MMIIM_SET_PINYIN_FUZZY_WIN_TAB,PNULL);
        #endif
    #else
        win_handle = MMK_CreateWin((uint32 *)MMIIM_SET_WIN_TAB,PNULL);
    #endif
#else
//sogou不支持模糊音
#if (!defined IM_ENGINE_SOGOU) && (defined IM_SIMP_CHINESE_SUPPORT || (defined IM_TRAD_CHINESE_SUPPORT))
    win_handle = MMK_CreateWin((uint32 *)MMIIM_SET_PINYIN_FUZZY_WIN_TAB,PNULL);
#endif
#endif

    return win_handle;
}

/*****************************************************************************/
//  Description : handle im set menu window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetMenuWin(
                                      MMI_WIN_ID_T      win_id, 
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM            param
                                      )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIM_SET_PY_FUZZY_LABEL_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	#ifndef MMI_GUI_STYLE_TYPICAL
        	GUIFORM_SetType(MMIIM_SET_FORM_CTRL_ID,GUIFORM_TYPE_TP);
	#endif
#if (defined IM_ENGINE_CSTAR) && (defined IM_SIMP_CHINESE_SUPPORT ||defined IM_TRAD_CHINESE_SUPPORT)
        SetPYFuzzyParam();
#endif
#if defined IM_HANDWRITING
        SetHandwriteSpeedParam();
        SetHandwriteThickParam();
        SetHandwriteColorParam();
#endif
#ifdef MMI_IM_PDA_SUPPORT
        SetKeyboardSizeParam();
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
	SetSogouSettingParam();
#endif

        //假设只有英文和一种本地语言，则不需要设置输入法语言，设置也是无效的
        if(MMISET_GetValidLanguageNum() >= 2)    
        {
            SetInputLangParam();
        }
        
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
        switch(ctrl_id)
        {
#if (defined IM_ENGINE_CSTAR) && (defined IM_SIMP_CHINESE_SUPPORT ||defined IM_TRAD_CHINESE_SUPPORT)
        case MMIIM_SET_PY_FUZZY_LABEL_CTRL_ID:
            MMK_CreateWin((uint32 *)MMIIM_SET_PINYIN_FUZZY_WIN_TAB,PNULL);
            break;
#endif
#ifdef TOUCH_PANEL_SUPPORT
#if defined IM_HANDWRITING
         case MMIIM_SET_HANDWRITE_COLOR_SETTING_LABEL_CTRL_ID:
             MMK_CreateWin((uint32*)MMIIM_SET_HW_COLOR_WIN_TAB,PNULL);
            break;
#endif
       #if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
        case MMIIM_SET_SOGOU_SETTING_LABEL_CTRL_ID:
		MMIAPIIM_OpenSogouIMSetInputModeWin();
            break;
	#endif

    #if defined MMI_IM_PDA_SUPPORT
        case MMIIM_SET_INPUT_LANG_LABEL_CTRL_ID:
            //假设只有英文和一种本地语言，则不需要设置输入法语言，设置也是无效的
            if(MMISET_GetValidLanguageNum() >= 2)
            {
                MMIAPISET_OpenImInputLangSettingWin();
            }
            
            break;
    #endif
    
#endif
            
        default:
            break;
        }   /*lint !e764*/      //这个在特定情况下，确实存在没有case的情况
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        SaveIMSet();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
        SaveIMSet();
        result = MMI_RESULT_FALSE;
        break;

#if defined MMI_IM_PDA_SUPPORT
    case MSG_GET_FOCUS:   
    {
        //假设只有英文和一种本地语言，则不需要设置输入法语言，设置也是无效的
        if(MMISET_GetValidLanguageNum() >= 2)
        {
            SetInputLangParam();
        }
        
        break;
    }
#endif

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIIM_CreateNewSoftKey(MMI_HANDLE_T win_handle,   MMI_HANDLE_T form_id ,MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    GUIFORM_DYNA_CHILD_T child_data = {0};

    child_data.is_softkey = TRUE;
    child_data.guid=SPRD_GUI_SOFTKEY_ID;
    child_data.child_handle=MMIIM_SOFTKEY_CTRL_ID;
    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;
    child_data.init_data_ptr=&softkey_data;
    GUIFORM_CreatDynaChildCtrl(win_handle,form_id,&child_data);
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : handle im set pinyin fuzzy window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetPinyinFuzzyWin(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIM_SET_PY_FUZZY_LIST_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_PermitChildBorder(MMIIM_PINYIN_FUZZY_WIN_FORM_ID, FALSE);
        GUIFORM_PermitChildBg(MMIIM_PINYIN_FUZZY_WIN_FORM_ID, FALSE);
        GUILIST_PermitBorder(ctrl_id, FALSE);
        AppendImPyFuzzyItem(ctrl_id);
#ifdef MMI_PDA_SUPPORT
        if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
        {
            MMIIM_CreateNewSoftKey(win_id,MMIIM_PINYIN_FUZZY_WIN_FORM_ID,TXT_COMMON_OK,TXT_NULL,STXT_CANCEL);
        }
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        SetImPyFuzzy(ctrl_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description :append im pinyin fuzzy item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AppendImPyFuzzyItem(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint16      max_item = MMIIM_MAX_PY_FUZZY;
    uint16      i = 0;
    uint16      pinyin_fuzzy = 0;

    //set list max item and selected max item
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);
    GUILIST_SetMaxSelectedItem(ctrl_id,max_item);

    //append item
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_Z_ZH,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_C_CH,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_S_SH,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_H_F,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_L_H,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_IN_ING,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_EN_ENG,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_AN_ANG,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_IAN_IANG,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(IM_TXT_INPUT_PY_FUZZY_UAN_UANG,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);


    //get current pinyin fuzzy set
    pinyin_fuzzy = MMIIM_GetPyFuzzy();

    for (i=0; i<max_item;i++)
    {
        if (0x00000001 == ((pinyin_fuzzy>>i) & 0x00000001))
        {
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id,i,TRUE);
        }
    }
}

/*****************************************************************************/
//  Description :set im pinyin fuzzy
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetImPyFuzzy(
                        MMI_CTRL_ID_T   ctrl_id
                        )
{
    uint16  selected_num = 0;
    uint16  selected_item[MMIIM_MAX_PY_FUZZY] = {0};
    uint16  i = 0;
    uint16  pinyin_fuzzy = 0;

    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,MMIIM_MAX_PY_FUZZY);

    for (i=0; i<selected_num; i++)
    {
        switch (selected_item[i])
        {
        case 0:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_Z_ZH);
            break;

        case 1:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_C_CH);
            break;

        case 2:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_S_SH);
            break;

        case 3:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_H_F);
            break;

        case 4:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_N_L);
            break;

        case 5:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_IN_ING);
            break;

        case 6:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_EN_ENG);
            break;

        case 7:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_AN_ANG);
            break;

        case 8:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_IAN_IANG);
            break;

        case 9:
            pinyin_fuzzy = (uint16)(pinyin_fuzzy | MMIIM_FUZZY_UAN_UANG);
            break;

        default:
            //SCI_TRACE_LOW:"SetImPyFuzzy:selected_item[%d] %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_739_112_2_18_2_29_28_288,(uint8*)"dd", selected_item[i]);
            break;
        }
    }

    //sest pinyin fuzzy
    MMIIM_SetPyFuzzy(pinyin_fuzzy);
}
#if defined IM_HANDWRITING
/*****************************************************************************/
//  Description : handle im set handwriting speed window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetHwSpeedWin(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIM_SET_HW_SPEED_LIST_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendImHwSpeedItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        SetImHwSpeed(ctrl_id);
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
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
//  Description :append im handwriting speed item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AppendImHwSpeedItem(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint16              max_item = MMIIM_HW_SPEED_MAX;
    uint16              cur_item = 0;
    MMIIM_HW_SPEED_E    hw_speed = MMIIM_HW_SPEED_NORMAL;

    //set list max item
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);

    MMIAPISET_AppendListItemByTextId(TXT_COMMON_FAST,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_NORMAL,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_SLOW,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

    hw_speed = MMIIM_GetHwSpeed();
    switch (hw_speed)
    {
    case MMIIM_HW_SPEED_FAST:
        cur_item = 0;
        break;

    case MMIIM_HW_SPEED_NORMAL:
        cur_item = 1;
        break;

    case MMIIM_HW_SPEED_SLOW:
        cur_item = 2;
        break;

    default:
        //SCI_TRACE_LOW:"AppendImHwSpeedItem:hw_speed %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_823_112_2_18_2_29_28_289,(uint8*)"d", hw_speed);
        break;
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_item,TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_item);
}

/*****************************************************************************/
//  Description :set im handwriting speed
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetImHwSpeed(
                        MMI_CTRL_ID_T   ctrl_id
                        )
{
    uint16              cur_item = 0;
    MMIIM_HW_SPEED_E    hw_speed = MMIIM_HW_SPEED_NORMAL;

    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id,&cur_item,1);
    cur_item = GUISETLIST_GetCurIndex(ctrl_id);
    switch (cur_item)
    {
    case 0:
        hw_speed = MMIIM_HW_SPEED_FAST;
        break;

    case 1:
        hw_speed = MMIIM_HW_SPEED_NORMAL;
        break;

    case 2:
        hw_speed = MMIIM_HW_SPEED_SLOW;
        break;

    default:
        //SCI_TRACE_LOW:"SetImHwSpeed:cur_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_863_112_2_18_2_29_28_290,(uint8*)"d", cur_item);
        break;
    }

    //set handwriting speed
    MMIIM_SetHwSpeed(hw_speed);
}

/*****************************************************************************/
//  Description : handle im set handwriting thickness window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetHwThicknessWin(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIIM_SET_HW_THICKNESS_LIST_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendImHwThicknessItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        SetImHwThickness(ctrl_id);
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
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
//  Description :append im handwriting thickness item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AppendImHwThicknessItem(
                                   MMI_CTRL_ID_T    ctrl_id
                                   )
{
    uint16                  max_item = MMIIM_HW_THICKNESS_MAX;
    uint16                  cur_item = 0;
    MMIIM_HW_THICKNESS_E    hw_thickness = MMIIM_HW_THICKNESS_NORMAL;

    //set list max item
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);

    MMIAPISET_AppendListItemByTextId(IM_TXT_HANDWRITING_THICK,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_NORMAL,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(IM_TXT_HANDWRITING_THIN,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

    hw_thickness = MMIIM_GetHwThickness();
    switch (hw_thickness)
    {
    case MMIIM_HW_THICKNESS_THICK:
        cur_item = 0;
        break;

    case MMIIM_HW_THICKNESS_NORMAL:
        cur_item = 1;
        break;

    case MMIIM_HW_THICKNESS_THIN:
        cur_item = 2;
        break;

    default:
        //SCI_TRACE_LOW:"AppendImHwThicknessItem:hw_thickness %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_946_112_2_18_2_29_28_291,(uint8*)"d", hw_thickness);
        break;
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_item,TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_item);
}

/*****************************************************************************/
//  Description :set im handwriting thickness
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetImHwThickness(
                            MMI_CTRL_ID_T   ctrl_id
                            )
{
    uint16                  cur_item = 0;
    MMIIM_HW_THICKNESS_E    hw_thickness = MMIIM_HW_THICKNESS_NORMAL;

    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id,&cur_item,1);
    cur_item = GUISETLIST_GetCurIndex(ctrl_id);
    switch (cur_item)
    {
    case 0:
        hw_thickness = MMIIM_HW_THICKNESS_THICK;
        break;

    case 1:
        hw_thickness = MMIIM_HW_THICKNESS_NORMAL;
        break;

    case 2:
        hw_thickness = MMIIM_HW_THICKNESS_THIN;
        break;

    default:
        //SCI_TRACE_LOW:"SetImHwThickness:cur_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_986_112_2_18_2_29_28_292,(uint8*)"d", cur_item);
        break;
    }

    //set handwriting thickness
    MMIIM_SetHwThickness(hw_thickness);
}


/*****************************************************************************/
//  Description : SetHwColorFormParam
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetHwColorFormParam(void)
{
    /*
    GUIFORM_CHILD_WIDTH_T       width = {0};
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};

    MMI_STRING_T                city_string =   {0};
    MMI_STRING_T                local_str =     {0};  
    uint16                      form_space =    WT_VERTICAL_SPACE;
    GUI_RECT_T                  wt_form_rect =     {0,WT_VERTICAL_FORM_TOP , MMI_MAINSCREEN_WIDTH -1, MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT -1  };
    
    
    width.type = GUIFORM_CHILD_WIDTH_AUTO;
    width.add_data = 80;

    //buttons
    GUIFORM_SetChildWidth(MMIWT_CURRENT_DATETIME_FORM_CTRL_ID,MMIWT_CURRENT_TIME_LABEL_CTRL_ID,&width);

    //local time date
    GUIFORM_SetChildWidth(MMIWT_LOCAL_DATETIME_FORM_CTRL_ID,MMIWT_LOCAL_TIME_LABEL_CTRL_ID,&width);

    width.add_data = 10;
    //summer icon
    GUIFORM_SetChildWidth(MMIWT_CURRENT_DATETIME_FORM_CTRL_ID,MMIWT_CURRENT_SUMMER_ICON_LABEL_CTRL_ID,&width);
    GUIFORM_SetChildWidth(MMIWT_LOCAL_DATETIME_FORM_CTRL_ID,MMIWT_LOCAL_SUMMER_ICON_LABEL_CTRL_ID,&width);


    GUILABEL_SetIcon(MMIWT_CURRENT_SUMMER_ICON_LABEL_CTRL_ID,IMAGE_ACC_DAY_SUMMER_TIME_ICON);
    GUILABEL_SetIcon(MMIWT_LOCAL_SUMMER_ICON_LABEL_CTRL_ID,IMAGE_ACC_DAY_SUMMER_TIME_ICON);
    
    data_info.img_id = IMAGE_ACC_WORLDTIME_MAP;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIWT_MAP_ANI_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIWT_MAP_ANI_CTRL_ID,MapAnimalCtrlCallbackFun);
    #if defined MAINLCD_SIZE_320X480
    GUILABEL_SetFont(MMIWT_CURRENT_TIME_LABEL_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_CURRENT_DATE_LABEL_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_CURRENT_SUMMER_ICON_LABEL_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,WCLK_WORK_COLOR);

    GUILABEL_SetFont(MMIWT_LOCAL_TIME_LABEL_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_LOCAL_DATE_LABEL_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_LOCAL_SUMMER_ICON_LABEL_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,WCLK_WORK_COLOR);
    
    GUILABEL_SetFont(MMIWT_LABEL_LOCAL_CTRL_ID,MMI_DEFAULT_BIG_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_LABEL_LOCAL_CITY_CTRL_ID,MMI_DEFAULT_BIG_FONT,WCLK_WORK_COLOR);
	GUIFORM_SetRect(MMIWT_FORM_CTRL_ID,&wt_form_rect);
	#else
    GUILABEL_SetFont(MMIWT_CURRENT_TIME_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_CURRENT_DATE_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
	GUILABEL_SetFont(MMIWT_CURRENT_SUMMER_ICON_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
    
    GUILABEL_SetFont(MMIWT_LOCAL_TIME_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_LOCAL_DATE_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
	GUILABEL_SetFont(MMIWT_LOCAL_SUMMER_ICON_LABEL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
	
    GUILABEL_SetFont(MMIWT_LABEL_LOCAL_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
    GUILABEL_SetFont(MMIWT_LABEL_LOCAL_CITY_CTRL_ID,MMI_DEFAULT_SMALL_FONT,WCLK_WORK_COLOR);
	#endif

    GUILABEL_SetTextById(MMIWT_LABEL_LOCAL_CTRL_ID,TXT_ACC_WORLD_LOCAL,FALSE);
    GUIFORM_SetSpace(MMIWT_FORM_CTRL_ID,&form_space,&form_space);
    GUIFORM_SetMargin(MMIWT_FORM_CTRL_ID,0);*/
}

/*****************************************************************************/
//  Description : handle im set handwriting color window
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImSetHwColorWin(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    static uint32       s_cur_button_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_COLOR_T         hw_color = 0;
    GUI_POINT_T         dis_point = {0};
    MMI_STRING_T        text_str = {0};
    MMI_CTRL_ID_T       ctrl_id1 = MMIIM_SET_HW_COLOR_PREVIEW_LABEL_CTRL_ID;
    MMI_CTRL_ID_T       ctrl_id2 = MMIIM_SET_HW_COLOR_TEXT_LABEL_CTRL_ID;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
    int16               total_width = client_rect.right - client_rect.left;
    int16               total_height = client_rect.bottom - client_rect.top;
    int16               left_space = total_width*MMIIM_SET_HW_COLOR_LEFT_PER/100;
    int16               top_space_prew = client_rect.top + total_height*MMIIM_SET_HW_COLOR_PRE_TOP_PER/100;
    uint32              new_button_idx = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T lab_prev_rect = {0};
            GUI_RECT_T lab_free_rect = {0};
            lab_prev_rect.left = left_space;
            lab_prev_rect.top = top_space_prew;
            lab_prev_rect.right = total_width - left_space;
            lab_prev_rect.bottom = top_space_prew+MMIIM_SET_HW_COLOR_PRE_HEIGHT;
            lab_free_rect.left = left_space;
            lab_free_rect.top = lab_prev_rect.bottom + 4;
            lab_free_rect.right = total_width - left_space;
            lab_free_rect.bottom = lab_prev_rect.bottom+4+MMIIM_SET_HW_COLOR_FREE_HEIGHT;
            //GUI_RECT_T lab_prev_rect = {left_space, top_space_prew, total_width - left_space, top_space_prew+MMIIM_SET_HW_COLOR_PRE_HEIGHT};
            //GUI_RECT_T lab_free_rect = {left_space, lab_prev_rect.bottom + 4, total_width - left_space, lab_prev_rect.bottom+4+MMIIM_SET_HW_COLOR_FREE_HEIGHT};
            if(lab_free_rect.bottom > client_rect.bottom)
            {
                lab_free_rect.left = left_space;
                lab_free_rect.right = total_width - left_space;
                lab_free_rect.bottom = client_rect.bottom - 4;
                lab_free_rect.top = lab_prev_rect.bottom -MMIIM_SET_HW_COLOR_FREE_HEIGHT;
                lab_prev_rect.left = left_space;
                lab_prev_rect.right = total_width - left_space;
                lab_prev_rect.bottom = lab_free_rect.top;   
                lab_prev_rect.top = lab_prev_rect.bottom - MMIIM_SET_HW_COLOR_PRE_HEIGHT;
            }
            GUILABEL_SetRect(MMIIM_SET_HW_COLOR_PREVIEW_LABEL_CTRL_ID, &lab_prev_rect, FALSE);
            GUILABEL_SetRect(MMIIM_SET_HW_COLOR_TEXT_LABEL_CTRL_ID, &lab_free_rect, FALSE);
        }
    
        //init color button
        InitHwColorButton(win_id);

        //SetHwColorFormParam();

        //set preview
        MMI_GetLabelTextByLang(STXT_PREVIEW,&text_str);
        GUILABEL_SetText(ctrl_id1,&text_str,FALSE);

        //set text
        MMI_GetLabelTextByLang(TXT_FREEDOM_COMMUNICATION,&text_str);
        GUILABEL_SetText(ctrl_id2,&text_str,FALSE);

        //get handwriting color
        hw_color = MMIIM_GetHwColor();

        //get current button index
        s_cur_button_index = GetHwColorButtonIndex(hw_color);

        //set background and text color
        GUILABEL_SetBackgroundColor(ctrl_id2,MMI_GRAY_WHITE_COLOR);
        GUILABEL_SetFont(ctrl_id2,MMI_DEFAULT_TEXT_FONT,hw_color);
        break;

    case MSG_LCD_SWITCH:
        {
            GUI_RECT_T lab_prev_rect = {0};
            GUI_RECT_T lab_free_rect = {0};
            lab_prev_rect.left = left_space;
            lab_prev_rect.top = top_space_prew;
            lab_prev_rect.right = total_width - left_space;
            lab_prev_rect.bottom = top_space_prew+MMIIM_SET_HW_COLOR_PRE_HEIGHT;
            lab_free_rect.left = left_space;
            lab_free_rect.top = lab_prev_rect.bottom + 4;
            lab_free_rect.right = total_width - left_space;
            lab_free_rect.bottom = lab_prev_rect.bottom+4+MMIIM_SET_HW_COLOR_FREE_HEIGHT;
            if(lab_free_rect.bottom > client_rect.bottom)
            {
                lab_free_rect.left = left_space;
                lab_free_rect.right = total_width - left_space;
                lab_free_rect.bottom = client_rect.bottom - 4;
                lab_free_rect.top = lab_prev_rect.bottom -MMIIM_SET_HW_COLOR_FREE_HEIGHT;
                lab_prev_rect.left = left_space;
                lab_prev_rect.right = total_width - left_space;
                lab_prev_rect.bottom = lab_free_rect.top;   
                lab_prev_rect.top = lab_prev_rect.bottom - MMIIM_SET_HW_COLOR_PRE_HEIGHT;
            }
            //GUI_RECT_T lab_prev_rect = {left_space, top_space_prew, total_width - left_space, top_space_prew+MMIIM_SET_HW_COLOR_PRE_HEIGHT};
            //GUI_RECT_T lab_free_rect = {left_space, lab_prev_rect.bottom + 4, total_width - left_space, lab_prev_rect.bottom+4+MMIIM_SET_HW_COLOR_FREE_HEIGHT};
            GUILABEL_SetRect(MMIIM_SET_HW_COLOR_PREVIEW_LABEL_CTRL_ID, &lab_prev_rect, FALSE);
            GUILABEL_SetRect(MMIIM_SET_HW_COLOR_TEXT_LABEL_CTRL_ID, &lab_free_rect, FALSE);
        }
        break;
    

    case MSG_FULL_PAINT:
        //display bg
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);

        //draw focus rect
        DrawHwColorButtonFocusRect(s_cur_button_index);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_OK:
    case MSG_NOTIFY_MIDSK:
    case MSG_APP_MENU:
        //set handwriting color
        SetHwColor(s_cur_button_index);

        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_LEFT:
        if (0 < s_cur_button_index)
        {
            s_cur_button_index--;
        }
        else
        {
            s_cur_button_index = (uint32)(MMIIM_HW_COLOR_MAX-1);
        }

        //set text color
        hw_color = s_mmiim_hw_color_button[s_cur_button_index].pressed_color;
        GUILABEL_SetFont(ctrl_id2,MMI_DEFAULT_TEXT_FONT,hw_color);

        //update
        MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        break;

    case MSG_APP_RIGHT:
        if (s_cur_button_index < (MMIIM_HW_COLOR_MAX-1))
        {
            s_cur_button_index++;
        }
        else
        {
            s_cur_button_index = 0;
        }

        //set text color
        hw_color = s_mmiim_hw_color_button[s_cur_button_index].pressed_color;
        GUILABEL_SetFont(ctrl_id2,MMI_DEFAULT_TEXT_FONT,hw_color);

        //update
        MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
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
                SetHwColor(s_cur_button_index);

                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMK_CloseWin(win_id);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_1264_112_2_18_2_29_29_293,(uint8*)"");
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);
                break;            
            default:
                break;
            }
            if (HandleHwColorPenOkMsg(((MMI_NOTIFY_T*)param)->src_id, &new_button_idx))
            {
                s_cur_button_index = new_button_idx;

                //set text color
                hw_color = s_mmiim_hw_color_button[s_cur_button_index].pressed_color;
                GUILABEL_SetFont(ctrl_id2,MMI_DEFAULT_TEXT_FONT,hw_color);
            }
        }

        
        //update
        MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (HandleHwColorPenOkMsg(((MMI_NOTIFY_T*)param)->src_id, &new_button_idx))
        {
            s_cur_button_index = new_button_idx;

            //set text color
            hw_color = s_mmiim_hw_color_button[s_cur_button_index].pressed_color;
            GUILABEL_SetFont(ctrl_id2,MMI_DEFAULT_TEXT_FONT,hw_color);
        }
        
        //update
        MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        break;
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 


    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init handwriting color button
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void InitHwColorButton(
                             MMI_WIN_ID_T   win_id
                             )
{
    int16 i = 0;
    GUIBUTTON_INIT_DATA_T init_data = {0};
    GUI_BOTH_RECT_T client_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUI_RECT_T client_rect_v = client_rect.v_rect;
    GUI_RECT_T client_rect_h = client_rect.h_rect;
    int16 total_width = 0;
    int16 total_height = 0;
    int16 button_w = 0;
    int16 button_h = 0;
    int16 left_space = 0;
    int16 top_space = 0;
    int16 button_gap = 0;

    //init rect values
    //坚屏下的
    total_width = client_rect_v.right - client_rect_v.left;
    total_height = client_rect_v.bottom - client_rect_v.top;
    
    top_space = client_rect_v.top + total_height * MMIIM_SET_HW_COLOR_TOP_PER / 100;
    left_space = client_rect_v.left + total_width * MMIIM_SET_HW_COLOR_LEFT_PER / 100;
    button_w = (total_width - left_space*2)/MMIIM_HW_COLOR_MAX;
    button_h = MMIIM_SET_HW_COLOR_HEIGHT;
    button_gap = MMIIM_SET_HW_COLOR_GAP;

    for (i=0; i<MMIIM_HW_COLOR_MAX; i++)
    {
        s_mmiim_hw_color_button[i].v_rect.left = left_space + (button_w + button_gap)*i;
        s_mmiim_hw_color_button[i].v_rect.top= top_space;
        s_mmiim_hw_color_button[i].v_rect.right = s_mmiim_hw_color_button[i].v_rect.left + button_w;
        s_mmiim_hw_color_button[i].v_rect.bottom= s_mmiim_hw_color_button[i].v_rect.top + button_h;
    }

    //横屏下的
    total_width = client_rect_h.right - client_rect_h.left;
    total_height = client_rect_h.bottom - client_rect_h.top;
    
    top_space = client_rect_h.top + total_height * MMIIM_SET_HW_COLOR_TOP_PER / 100;
    left_space = client_rect_h.left + total_width * MMIIM_SET_HW_COLOR_LEFT_PER / 100;
    button_w = (total_width - left_space*2)/MMIIM_HW_COLOR_MAX;
    button_h = MMIIM_SET_HW_COLOR_HEIGHT;
    button_gap = MMIIM_SET_HW_COLOR_GAP;

    for (i=0; i<MMIIM_HW_COLOR_MAX; i++)
    {
        s_mmiim_hw_color_button[i].h_rect.left = left_space + (button_w + button_gap)*i;
        s_mmiim_hw_color_button[i].h_rect.top= top_space;
        s_mmiim_hw_color_button[i].h_rect.right = s_mmiim_hw_color_button[i].h_rect.left + button_w;
        s_mmiim_hw_color_button[i].h_rect.bottom= s_mmiim_hw_color_button[i].h_rect.top + button_h;
    }   

    for (i=0; i<MMIIM_HW_COLOR_MAX; i++)
    {
        //init
        init_data.both_rect.v_rect = s_mmiim_hw_color_button[i].v_rect;
        init_data.both_rect.h_rect = s_mmiim_hw_color_button[i].h_rect;
        init_data.bg.bg_type = GUI_BG_COLOR;
        init_data.bg.color   = s_mmiim_hw_color_button[i].pressed_color;

        //create
        GUIBUTTON_CreateDynamic(win_id,
            s_mmiim_hw_color_button[i].ctrl_id,
            &init_data);
    }
}

/*****************************************************************************/
//  Description : get current handwriting color button index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetHwColorButtonIndex(
                                   GUI_COLOR_T  hw_color
                                   )
{
    uint32              i = 0;
    uint32              button_index = 0;

    //get focus button index
    for (i=0; i<MMIIM_HW_COLOR_MAX; i++)
    {
        if (hw_color == s_mmiim_hw_color_button[i].pressed_color)
        {
            button_index = i;
            break;
        }
    }

    return (button_index);
}

/*****************************************************************************/
//  Description : draw focus rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DrawHwColorButtonFocusRect(
                                      uint32    button_index
                                      )
{
    GUI_RECT_T          button_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    //draw focus rect
    if (MMITHEME_IsMainScreenLandscape())
    {
        button_rect = s_mmiim_hw_color_button[button_index].h_rect;
    } 
    else
    {
        button_rect = s_mmiim_hw_color_button[button_index].v_rect;
    }
    button_rect.left--;
    button_rect.top--;
    button_rect.right++;
    button_rect.bottom++;

    //top,bottom
    LCD_DrawHLine(&lcd_dev_info,button_rect.left,button_rect.top,button_rect.right,MMIIM_SET_HW_COLOR_FRAME_COLOR);
    LCD_DrawHLine(&lcd_dev_info,button_rect.left,button_rect.bottom,button_rect.right,MMIIM_SET_HW_COLOR_FRAME_COLOR);

    //left,right
    LCD_DrawVLine(&lcd_dev_info,button_rect.left,button_rect.top,button_rect.bottom,MMIIM_SET_HW_COLOR_FRAME_COLOR);
    LCD_DrawVLine(&lcd_dev_info,button_rect.right,button_rect.top,button_rect.bottom,MMIIM_SET_HW_COLOR_FRAME_COLOR);
}

/*****************************************************************************/
//  Description : set handwriting color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetHwColor(
                      uint32    button_index
                      )
{
    GUI_COLOR_T     hw_color = 0;

    //get current color
    hw_color = s_mmiim_hw_color_button[button_index].pressed_color;
    MMIIM_SetHwColor(hw_color);
}

/*****************************************************************************/
//  Description : handle handwriting color pen ok msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: if not click on color buttons, retrun false
/*****************************************************************************/
LOCAL BOOLEAN HandleHwColorPenOkMsg(//button index
                                   MMI_CTRL_ID_T    ctrl_id,
                                   uint32*          button_idx_ptr
                                   )
{
    uint32              i = 0;
    BOOLEAN             is_ret = FALSE;
    
    if (PNULL == button_idx_ptr)
    {
        //SCI_TRACE_LOW:"HandleHwColorPenOkMsg: button_idx_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_1471_112_2_18_2_29_29_294,(uint8*)"");
        return FALSE;
    }

    //get focus button index
    for (i=0; i<MMIIM_HW_COLOR_MAX; i++)
    {
        if (ctrl_id == s_mmiim_hw_color_button[i].ctrl_id)
        {
            *button_idx_ptr = i;
            is_ret = TRUE;
            break;
        }
    }

    return is_ret;
}
#endif
/*****************************************************************************/
//  Description : set py fuzzy label param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPYFuzzyParam(void)
{
    MMI_STRING_T title = {0};


    GUIFORM_SetStyle(MMIIM_SET_PY_FUZZY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(IM_TXT_INPUT_PY_FUZZY_SET, &title);
    GUILABEL_SetText(MMIIM_SET_PY_FUZZY_LABEL_CTRL_ID, &title, FALSE);
}
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
LOCAL void SetSogouSettingParam(void)
{
    MMI_STRING_T title = {0};

    GUIFORM_SetStyle(MMIIM_SET_SOGOU_SETTING_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SOGOU_SETTING, &title);
    GUILABEL_SetText(MMIIM_SET_SOGOU_SETTING_LABEL_CTRL_ID, &title, FALSE);
}
#endif

#if defined MMI_IM_PDA_SUPPORT
//extern int32 MMIAPISET_GetInputLanguageContent(uint16 *str_len_ptr, wchar *str_content_ptr);
LOCAL void SetInputLangParam(void)
{
    wchar str_content_ptr[30] = {0};    //长度不可小于MMISET_LANG_NAME_MAX_LEN
    MMI_STRING_T title = {0};

    GUIFORM_SetStyle(MMIIM_SET_INPUT_LANG_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_INPUT_LANGUAGE, &title);
    GUILABEL_SetText(MMIIM_SET_INPUT_LANG_LABEL_CTRL_ID, &title, FALSE);

    SCI_MEMSET(str_content_ptr, 0, sizeof(str_content_ptr));
    MMIAPISET_GetInputLanguageContent(&title.wstr_len, str_content_ptr);    //lint !e628 !e718 !e746 
    title.wstr_ptr = str_content_ptr;
    GUILABEL_SetText(MMIIM_SET_INPUT_LANG_CONTENT_LABEL_CTRL_ID, &title, FALSE);
}
#endif
#if defined IM_HANDWRITING
/*****************************************************************************/
//  Description : set handwritting speed param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetHandwriteSpeedParam(void)
{
    uint16         loop      = 0;
    uint16         cur_index = 0;
    MMI_STRING_T   title     = {0};
    MMIIM_HW_SPEED_E    hw_speed = MMIIM_HW_SPEED_NORMAL;
    MMI_TEXT_ID_T  speed_str[MMIIM_HW_SPEED_MAX] = 
    {
        TXT_COMMON_FAST,
        TXT_NORMAL,
        TXT_COMMON_SLOW
    };
#ifndef MMI_GUI_STYLE_TYPICAL
    	GUISETLIST_SetFontAlign(MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID,ALIGN_LEFT);
#endif
    GUIFORM_SetStyle(MMIIM_SET_HANDWRITE_SPEED_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(IM_TXT_INPUT_HANDWRITING_SPEED, &title);
    GUILABEL_SetText(MMIIM_SET_HANDWRITE_SPEED_LABEL_CTRL_ID, &title, FALSE);

    for(loop = 0; loop < MMIIM_HW_SPEED_MAX; loop++)
    {
	    GUISETLIST_AddItemById(MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID, speed_str[loop]);
    }

    hw_speed = MMIIM_GetHwSpeed();
    switch (hw_speed)
    {
    case MMIIM_HW_SPEED_FAST:
        cur_index = 0;
        break;

    case MMIIM_HW_SPEED_NORMAL:
        cur_index = 1;
        break;

    case MMIIM_HW_SPEED_SLOW:
        cur_index = 2;
        break;

    default:
        //SCI_TRACE_LOW:"AppendImHwSpeedItem: hw_speed %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_1574_112_2_18_2_29_30_295,(uint8*)"d", hw_speed);
        break;
    }
    
    GUISETLIST_SetCtrlState(MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID, hw_speed);
    GUISETLIST_SetTitleTextId(MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID,IM_TXT_INPUT_HANDWRITING_SPEED_SET);	
}
/*****************************************************************************/
//  Description : set handwritting thick param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetHandwriteThickParam(void )
{
    MMI_STRING_T   title     = {0};
    uint16         loop      = 0;
    uint16         cur_index = 0;
    MMIIM_HW_THICKNESS_E    hw_thickness = MMIIM_HW_THICKNESS_NORMAL;    
    MMI_TEXT_ID_T  thickness_str[MMIIM_HW_THICKNESS_MAX] = 
    {
        IM_TXT_HANDWRITING_THICK,
        TXT_COMMON_COMMON_NORMAL,
        IM_TXT_HANDWRITING_THIN
    };
    GUIFORM_SetStyle(MMIIM_SET_HANDWRITE_THICK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifndef MMI_GUI_STYLE_TYPICAL
    	GUISETLIST_SetFontAlign(MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID,ALIGN_LEFT);
#endif
    MMI_GetLabelTextByLang(IM_TXT_INPUT_HANDWRITING_THICKNESS, &title);
    GUILABEL_SetText(MMIIM_SET_HANDWRITE_THICK_LABEL_CTRL_ID, &title, FALSE);

    for(loop = 0; loop < MMIIM_HW_THICKNESS_MAX; loop++)
    {
	    GUISETLIST_AddItemById(MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID, thickness_str[loop]);
    }
    hw_thickness = MMIIM_GetHwThickness();
    switch (hw_thickness)
    {
    case MMIIM_HW_THICKNESS_THICK:
        cur_index = 0;
        break;

    case MMIIM_HW_THICKNESS_NORMAL:
        cur_index = 1;
        break;

    case MMIIM_HW_THICKNESS_THIN:
        cur_index = 2;
        break;

    default:
        //SCI_TRACE_LOW:"AppendImHwThicknessItem: hw_thickness %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_1625_112_2_18_2_29_30_296,(uint8*)"d", hw_thickness);
        break;
    }
    
    GUISETLIST_SetCtrlState(MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID, hw_thickness);
    GUISETLIST_SetTitleTextId(MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID,IM_TXT_INPUT_HANDWRITING_THICKNESS_SET);	
}
/*****************************************************************************/
//  Description : set handwritting color param 
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetHandwriteColorParam(void)
{
    MMI_STRING_T   title1     = {0};
    MMI_STRING_T   title2     = {0};

    GUIFORM_SetStyle(MMIIM_SET_HANDWRITE_COLOR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(IM_TXT_INPUT_HANDWRITING_COLOR, &title1);
    GUILABEL_SetText(MMIIM_SET_HANDWRITE_COLOR_LABEL_CTRL_ID, &title1, FALSE);
    MMI_GetLabelTextByLang(IM_TXT_INPUT_HANDWRITING_COLOR_SET, &title2);
    GUILABEL_SetText(MMIIM_SET_HANDWRITE_COLOR_SETTING_LABEL_CTRL_ID, &title2, FALSE);
}
#endif
#ifdef MMI_IM_PDA_SUPPORT
/*==============================================================================
Description: SetKeyboardSizeParam
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetKeyboardSizeParam(void)
{
    MMI_STRING_T title = {0};
    uint16 cur_index = 0;
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_SMALL;

    GUIFORM_SetStyle(MMIIM_SET_KEYBOARD_SIZE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUISETLIST_SetFontAlign(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, ALIGN_LEFT);
    MMI_GetLabelTextByLang(IM_TXT_KEYBOARD_SIZE_SET, &title);
    GUILABEL_SetText(MMIIM_SET_KEYBOARD_SIZE_LABEL_CTRL_ID, &title, FALSE);

    GUISETLIST_AddItemById(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, TXT_COMM_FONT_BIG);
    GUISETLIST_AddItemById(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, TXT_COMMON_NORMAL);
    GUISETLIST_AddItemById(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, TXT_FONT_SMALL);

    kb_size = MMIIM_GetKeyboardSize();

    switch (kb_size)
    {
    case MMIIM_KEYBOARD_SIZE_BIG:
        cur_index = 0;
        break;

    case MMIIM_KEYBOARD_SIZE_NORMAL:
        cur_index = 1;
        break;

    case MMIIM_KEYBOARD_SIZE_SMALL:
        cur_index = 2;
        break;

    default:
        //SCI_TRACE_LOW:"AppendImHwThicknessItem: hw_thickness %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_1687_112_2_18_2_29_30_297,(uint8*)"d", kb_size);
        break;
    }
    
    GUISETLIST_SetCtrlState(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID, kb_size);
    GUISETLIST_SetTitleTextId(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID,IM_TXT_KEYBOARD_SIZE_SET);

    return;
}


/*==============================================================================
Description: SetImKeyboardSize
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetImKeyboardSize(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_item = 0;
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_SMALL;

    cur_item = GUISETLIST_GetCurIndex(ctrl_id);
    
    switch (cur_item)
    {
    case 0:
        kb_size = MMIIM_KEYBOARD_SIZE_BIG;
        break;

    case 1:
        kb_size = MMIIM_KEYBOARD_SIZE_NORMAL;
        break;

    case 2:
        kb_size = MMIIM_KEYBOARD_SIZE_SMALL;
        break;

    default:
        //SCI_TRACE_LOW:"SetImKeyboardSize:cur_item %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_WINTAB_1726_112_2_18_2_29_30_298,(uint8*)"d", cur_item);
        break;
    }

    MMIIM_SetKeyboardSize(kb_size);

    return;
}
#endif


/*****************************************************************************/
//  Description : save all setting(im)
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveIMSet(void)
{
#if defined IM_HANDWRITING
    SetImHwSpeed(MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID);
    SetImHwThickness(MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID);
#endif
#ifdef MMI_IM_PDA_SUPPORT
    SetImKeyboardSize(MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID);
#endif

    return;
}



/*Edit by script, ignore 5 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
