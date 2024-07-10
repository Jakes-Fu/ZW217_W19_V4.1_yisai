/****************************************************************************
** File Name:      window_parse.h                                           *
** Author:         Great.Tian                                              *
** Date:           03/23/2004                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 the listbox control.                                    *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2004        Great.Tian       Create
** 
****************************************************************************/
#ifndef  _WINDOW_PARSE_H_    
#define  _WINDOW_PARSE_H_   


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmk_msg.h" 
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmi_osbridge.h" 
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_applet_table.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#define WIN_DEF(_WINTAB_)     extern const uint32 _WINTAB_[];

#ifdef WIN_DEF
#include "win.def"
#endif

#define WINDOW_TABLE(_WINTAB_) const uint32 _WINTAB_[]

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define WIN_PRIO(_PRIO_)        MMK_WINPRIO, _PRIO_
#define WIN_FUNC(_FUNC_)        MMK_WINFUNC, _FUNC_
#define WIN_ID(_ID_)            MMK_WINID, _ID_

// create the menu control        
#define CREATE_MENU_CTRL(_GROUP_ID_, _CTRL_ID_) \
        MMK_CREATE_MENU, _GROUP_ID_, _CTRL_ID_

// create the menu control        
#define CREATE_MAINMENU_CTRL(_TYPE_, _CTRL_ID_) \
        MMK_CREATE_MAINMENU, _TYPE_, _CTRL_ID_

// create the pop_up menu control        
#define CREATE_POPMENU_CTRL(_GROUP_ID_, _CTRL_ID_) \
        MMK_CREATE_POPMENU, _GROUP_ID_, _CTRL_ID_

// create the msgbox control
#define CREATE_MSGBOX_CTRL(_STYLE_, _CTRL_ID_) \
        MMK_CREATE_MSGBOX, _STYLE_, _CTRL_ID_

// create the prgbox control
#define CREATE_PRGBOX_CTRL(_TYPE_, _CTRL_ID_) \
        MMK_CREATE_PRGBOX, _TYPE_, _CTRL_ID_

// create the text control
#define CREATE_TEXT_CTRL(_CTRL_ID_) \
        MMK_CREATE_TEXT, _CTRL_ID_

// create the listbox control
#define CREATE_LISTBOX_CTRL(_LIST_TYPE_, _CTRL_ID_) \
        MMK_CREATE_LISTBOX, _LIST_TYPE_, _CTRL_ID_

// create the text edit box control
#define CREATE_EDIT_TEXT_CTRL(_STR_MAX_LEN_, _CTRL_ID_) \
        MMK_CREATE_EDIT_TEXT, _STR_MAX_LEN_, _CTRL_ID_

// create the phone number edit box control
#define CREATE_EDIT_PHONENUM_CTRL(_STR_MAX_LEN_, _CTRL_ID_) \
        MMK_CREATE_EDIT_PHONENUM, _STR_MAX_LEN_, _CTRL_ID_

// create the digital edit box control
#define CREATE_EDIT_DIGITAL_CTRL(_STR_MAX_LEN_, _CTRL_ID_) \
        MMK_CREATE_EDIT_DIGITAL, _STR_MAX_LEN_, _CTRL_ID_

// create the password edit box control
#define CREATE_EDIT_PASSWORD_CTRL(_STR_MAX_LEN_, _CTRL_ID_) \
        MMK_CREATE_EDIT_PASSWORD, _STR_MAX_LEN_, _CTRL_ID_

// create the list edit box control
#define CREATE_EDIT_LIST_CTRL(_ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_) \
        MMK_CREATE_EDIT_LIST, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_

// create the date edit box control
#define CREATE_EDIT_DATE_CTRL(_CTRL_ID_) \
        MMK_CREATE_EDIT_DATE, _CTRL_ID_

// create the time edit box control
#define CREATE_EDIT_TIME_CTRL(_CTRL_ID_) \
        MMK_CREATE_EDIT_TIME, _CTRL_ID_

// create the date edit box control
#define CREATE_EDIT_TOUCH_DATE_CTRL(_CTRL_ID_) \
        MMK_CREATE_EDIT_TOUCH_DATE, _CTRL_ID_

// create the time edit box control
#define CREATE_EDIT_TOUCH_TIME_CTRL(_CTRL_ID_) \
        MMK_CREATE_EDIT_TOUCH_TIME, _CTRL_ID_

// create the time edit box control
#define CREATE_EDIT_PICKER_CTRL(_CTRL_ID_) \
        MMK_CREATE_EDIT_PICKER, _CTRL_ID_

// create the ip edit box control
#define CREATE_EDIT_IP_CTRL(_CTRL_ID_) \
        MMK_CREATE_EDIT_IP, _CTRL_ID_

// create the animation
#define CREATE_ANIM_CTRL(_CTRL_ID_,_WIN_ID_) \
        MMK_CREATE_ANIM, _CTRL_ID_,_WIN_ID_

// window title id
#define WIN_TITLE(_TITEL_ID_)       MMK_CREATE_TITLE, _TITEL_ID_

// window background picture id
#define WIN_BACKGROUND_ID(_BACKGROUND_ID_)      MMK_WINBACKGROUNDID, _BACKGROUND_ID_

// create the label control
#define CREATE_LABEL_CTRL(_ALIGN_, _CTRL_ID_) \
        MMK_CREATE_LABEL, _ALIGN_, _CTRL_ID_

// create the button control
#define CREATE_BUTTON_CTRL(_BG_IMAGE_ID, _CTRL_ID_) \
        MMK_CREATE_BUTTON, _BG_IMAGE_ID, _CTRL_ID_

// create the checkbox control
#define CREATE_CHECKBOX_CTRL(_CTRL_ID_) \
        MMK_CREATE_CHECKBOX, _CTRL_ID_

//create the drop down list box
#define CREATE_DROPDOWNLIST_CTRL(PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_) \
        MMK_CREATE_DROPDOWNLIST, PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_
        
// create the iconlist control
#define CREATE_ICONLIST_CTRL(_CTRL_ID_) \
        MMK_CREATE_ICONLIST, _CTRL_ID_

// create the iconfolder control
#define CREATE_ICONFOLDER_CTRL(_CTRL_ID_) \
        MMK_CREATE_ICONFOLDER, _CTRL_ID_

//create the rich text control
#define CREATE_RICHTEXT_CTRL(_CTRL_ID_) \
        MMK_CREATE_RICHTEXT, _CTRL_ID_

// create the tab control
#define CREATE_TAB_CTRL( _TAB_TYPE_, _TAB_STATE_, _CTRL_ID_) \
        MMK_CREATE_TAB, _TAB_TYPE_, _TAB_STATE_, _CTRL_ID_

// create the setting list control
#define CREATE_SETLIST_CTRL(_CTRL_ID_) \
        MMK_CREATE_SETLIST, _CTRL_ID_

// create the combox control
#define CREATE_COMBOX_CTRL(PAGE_ITEM_NUM, _TYPE_,_CTRL_ID_) \
        MMK_CREATE_COMBOX, PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_

// create status bar control
#define WIN_STATUSBAR   MMK_CREATE_STATUSBAR

// not auto create status bar control
#define WIN_HIDE_STATUS   MMK_HIDE_STATUSBAR

// create tips control
#define WIN_TIPS        MMK_CREATE_TIPS

//create the softkey control
#define WIN_SOFTKEY(_LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_) \
        MMK_CREATE_SOFTKEY, _LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_

#define WIN_STYLE(_WIN_STYLE_)              MMK_WINDOW_STYLE , _WIN_STYLE_

#define WIN_MOVE_STYLE(_WIN_MOVE_STYLE_)    MMK_WINDOW_ANIM_MOVE_SYTLE , _WIN_MOVE_STYLE_

#define WIN_SUPPORT_ANGLE(_WIN_SUPPORT_ANGLE_) MMK_WINDOW_SUPPORT_ANGLE , _WIN_SUPPORT_ANGLE_

#ifdef GUIF_SCROLLKEY
#define WIN_SCROLLKEY(_LEFT_SCROLL_KEY_ID_, _RIGHT_SCROLL_KEY_ID_, _LEFT_SCROLL_TEXT_ID_, _RIGHT_SCROLL_TEXT_ID_, _LEFT_SCROLL_SKIN_IDX_, _RIGHT_SCROLL_SKIN_IDX_) \
        MMK_CREATE_SCROLLKEY, _LEFT_SCROLL_KEY_ID_, _RIGHT_SCROLL_KEY_ID_, _LEFT_SCROLL_TEXT_ID_, _RIGHT_SCROLL_TEXT_ID_, _LEFT_SCROLL_SKIN_IDX_, _RIGHT_SCROLL_SKIN_IDX_
#endif

//create the form control
#define CREATE_FORM_CTRL(_LAYOUT_, _CTRL_ID_) \
        MMK_CREATE_FORM, _LAYOUT_, _CTRL_ID_

//create the owner draw control
#define CREATE_OWNDRAW_CTRL(_CTRL_ID_, _CALL_BACK_) \
        MMK_CREATE_OWNDRAW, _CTRL_ID_, _CALL_BACK_

#define CREATE_TOOLBAR_CTRL(_CTRL_ID_ ) \
        MMK_CREATE_TOOLBAR, _CTRL_ID_ 

#define CREATE_SLIDER_CTRL(_CTRL_ID_ ) \
        MMK_CREATE_SLIDER, _CTRL_ID_ 

#define CREATE_PROGRESS_CTRL(_CTRL_ID_ ) \
        MMK_CREATE_PROGRESS, _CTRL_ID_ 

#define CREATE_LETTERBARS_CTRL(_CTRL_ID_ ) \
        MMK_CREATE_LETTERSBAR, _CTRL_ID_ 

//create scroll control
#define CREATE_SCROLL_CTRL(_CTRL_ID_) \
        MMK_CREATE_SCROLL, _CTRL_ID_ 

#define END_WIN                             MMK_END_WIN

// create child text
#define CHILD_TEXT_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_TEXT, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child text edit
#define CHILD_EDIT_TEXT_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_EDIT_TEXT, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child phone number edit
#define CHILD_EDIT_PHONENUM_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PHONENUM, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child digital edit
#define CHILD_EDIT_DIGITAL_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_DIGITAL, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child password edit
#define CHILD_EDIT_PASSWORD_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PASSWORD, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child list edit
#define CHILD_EDIT_LIST_CTRL(_IS_GET_ACTIVE_, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_LIST, _IS_GET_ACTIVE_, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child date edit
#define CHILD_EDIT_DATE_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_DATE, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child time edit
#define CHILD_EDIT_TIME_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_TIME, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child touch date edit
#define CHILD_EDIT_TOUCH_DATE_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_TOUCH_DATE, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child touch time edit
#define CHILD_EDIT_TOUCH_TIME_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_TOUCH_TIME, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child touch time edit
#define CHILD_EDIT_PICKER_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PICKER, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child ip edit
#define CHILD_EDIT_IP_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_IP, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child animation
#define CHILD_ANIM_CTRL(_IS_BG,_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_ANIM, _IS_BG, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child label
#define CHILD_LABEL_CTRL(_ALIGN_, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_LABEL, _ALIGN_, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create child button
#define CHILD_BUTTON_CTRL(_IS_GET_ACTIVE_, _BG_IMAGE_ID, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_BUTTON, _IS_GET_ACTIVE_, _BG_IMAGE_ID, _CTRL_ID_, _PARENT_CTRL_ID_

// create child checkbox
#define CHILD_CHECKBOX_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_CHECKBOX, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

//create the drop down list box
#define CHILD_DROPDOWNLIST_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_DROPDOWNLIST, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_

// create child setlist
#define CHILD_SETLIST_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_SETLIST, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_

// create child owner draw
#define CHILD_OWNDRAW_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_, _CALL_BACK_) \
        CREATE_CHILD_OWNDRAW, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_, _CALL_BACK_

// create the form control
#define CHILD_FORM_CTRL(_IS_GET_ACTIVE_, _LAYOUT_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_FORM, _IS_GET_ACTIVE_, _LAYOUT_, _CTRL_ID_, _PARENT_CTRL_ID_

// create the list control
#define CHILD_LIST_CTRL(_IS_GET_ACTIVE_, _LIST_TYPE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_LIST, _IS_GET_ACTIVE_, _LIST_TYPE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create the softkey control
#define CHILD_SOFTKEY_CTRL(_LEFTSOFT_ID_, _MIDSOFT_ID_, _RIGHT_SOFT_ID_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_SOFTKEY, _LEFTSOFT_ID_, _MIDSOFT_ID_, _RIGHT_SOFT_ID_, _CTRL_ID_, _PARENT_CTRL_ID_

// create the slider control
#define CHILD_SLIDER_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_SLIDER, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

// create the progress control
#define CHILD_PROGRESS_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_PROGRESS, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_

//create the drop down list box
#define CHILD_COMBOX_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_COMBOX, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_
        

//兼容窗口表方式创建pub win
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
#define MMK_CreatePubListWin( _WIN_TABLE_, _ADD_DATA_ )  MMK_CreatePubwinTab( SPRD_GUI_LIST_ID, _WIN_TABLE_, _ADD_DATA_ )
#define MMK_CreatePubEditWin( _WIN_TABLE_, _ADD_DATA_ )  MMK_CreatePubwinTab( SPRD_GUI_EDITBOX_ID, _WIN_TABLE_, _ADD_DATA_ )
#define MMK_CreatePubFormWin( _WIN_TABLE_, _ADD_DATA_ )  MMK_CreatePubwinTab( SPRD_GUI_FORM_ID, _WIN_TABLE_, _ADD_DATA_ )

#define MMK_CreatePubListWinEx(_APPLET_HANDLE_, _WIN_TABLE_, _ADD_DATA_ )  MMK_CreatePubwinTabEx(_APPLET_HANDLE_, SPRD_GUI_LIST_ID, _WIN_TABLE_, _ADD_DATA_ )
#define MMK_CreatePubEditWinEx(_APPLET_HANDLE_, _WIN_TABLE_, _ADD_DATA_ )  MMK_CreatePubwinTabEx(_APPLET_HANDLE_, SPRD_GUI_EDITBOX_ID, _WIN_TABLE_, _ADD_DATA_ )
#define MMK_CreatePubFormWinEx(_APPLET_HANDLE_, _WIN_TABLE_, _ADD_DATA_ )  MMK_CreatePubwinTabEx(_APPLET_HANDLE_, SPRD_GUI_FORM_ID, _WIN_TABLE_, _ADD_DATA_ )
#else
#define MMK_CreatePubListWin   MMK_CreateWin
#define MMK_CreatePubEditWin   MMK_CreateWin
#define MMK_CreatePubFormWin   MMK_CreateWin

#define MMK_CreatePubListWinEx   MMK_CreateWinByApplet
#define MMK_CreatePubEditWinEx   MMK_CreateWinByApplet
#define MMK_CreatePubFormWinEx   MMK_CreateWinByApplet
#endif


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//parse window
typedef enum parse_win_tag
{
    FIRST_PARSE                 = 0xF000, // 对应处理函数
    MMK_WINPRIO                 = FIRST_PARSE,    
    MMK_WINFUNC,
    MMK_WINID,

    MMK_CREATE_MENU,
    MMK_CREATE_MAINMENU,
    MMK_CREATE_POPMENU,
    MMK_CREATE_MSGBOX,
    MMK_CREATE_PRGBOX,
    MMK_CREATE_TEXT,
    MMK_CREATE_LISTBOX,

    MMK_CREATE_EDIT_TEXT,
    MMK_CREATE_EDIT_PHONENUM,
    MMK_CREATE_EDIT_DIGITAL,
    MMK_CREATE_EDIT_PASSWORD,
    MMK_CREATE_EDIT_LIST,
    MMK_CREATE_EDIT_DATE,
    MMK_CREATE_EDIT_TIME,
    MMK_CREATE_EDIT_TOUCH_DATE,
    MMK_CREATE_EDIT_TOUCH_TIME,
    MMK_CREATE_EDIT_PICKER,
    MMK_CREATE_EDIT_IP,
    
    MMK_CREATE_ANIM,
    MMK_WINBACKGROUNDID,
    MMK_CREATE_LABEL,
    MMK_CREATE_BUTTON,
    MMK_CREATE_CHECKBOX,

    MMK_WINDOW_STYLE,
    MMK_CREATE_DROPDOWNLIST,
    MMK_CREATE_ICONLIST,
    MMK_CREATE_RICHTEXT,
    MMK_CREATE_TAB,
    MMK_CREATE_STATUSBAR,
    MMK_CREATE_SOFTKEY,
    MMK_CREATE_TITLE,
    MMK_WINDOW_ANIM_MOVE_SYTLE,
    MMK_CREATE_TIPS,

    MMK_CREATE_SETLIST,
    MMK_CREATE_FORM,
    MMK_CREATE_OWNDRAW,
    MMK_CREATE_TOOLBAR,
    MMK_CREATE_ICONFOLDER,
    MMK_HIDE_STATUSBAR,
    
    MMK_WINDOW_SUPPORT_ANGLE,
    MMK_CREATE_SCROLLKEY,
    MMK_CREATE_SLIDER,
    MMK_CREATE_PROGRESS,
    MMK_CREATE_LETTERSBAR,

    MMK_CREATE_SCROLL,
    
    MMK_CREATE_COMBOX,    

    MMK_END_WIN,
    LAST_PARSE
} MMK_PARSE_WIN_E;

//parse form child control
//必须和FormChildCtrlParseFunc[]的定义相对应
typedef enum parse_child_ctrl_tag
{
    CHILD_CTRL_FIRST            = 0xF100,
    CREATE_CHILD_TEXT           = CHILD_CTRL_FIRST,
    CREATE_CHILD_EDIT_TEXT,
    CREATE_CHILD_EDIT_PHONENUM,
    CREATE_CHILD_EDIT_DIGITAL,
    CREATE_CHILD_EDIT_PASSWORD,
    CREATE_CHILD_EDIT_LIST,
    CREATE_CHILD_EDIT_DATE,
    CREATE_CHILD_EDIT_TIME,
    CREATE_CHILD_EDIT_IP,
    CREATE_CHILD_ANIM,
    CREATE_CHILD_LABEL,
    CREATE_CHILD_BUTTON,
    CREATE_CHILD_CHECKBOX,
    CREATE_CHILD_DROPDOWNLIST,
    CREATE_CHILD_SETLIST,
    CREATE_CHILD_OWNDRAW,
    CREATE_CHILD_FORM,
    CREATE_CHILD_LIST,
    CREATE_CHILD_SOFTKEY,
    CREATE_CHILD_TAB,
    CREATE_CHILD_EDIT_TOUCH_DATE,
    CREATE_CHILD_EDIT_TOUCH_TIME,
    CREATE_CHILD_EDIT_PICKER,
    CREATE_CHILD_SLIDER,
    CREATE_CHILD_PROGRESS,
    CREATE_CHILD_COMBOX,
    LAST_CHILD
} MMK_PARSE_CHILD_CTRL_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : parse window tab
//	Global resource dependence : 
//  Author: Jasmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ParseWinTab(    
                               MMI_HANDLE_T  win_handle,
                               uint32       *win_tab_ptr
                               );

/*****************************************************************************/
//  Description : create control
//  Global resource dependence : 
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CreateCtrlByWinTab(    
                                      MMI_WIN_ID_T   win_id,
                                      uint32         *win_tab_ptr
                                      );

/*****************************************************************************/
// 	Description : create control table
//	Global resource dependence : 
//  Author:James Zhang
//	Note:destroy all control first
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CreateCtrlByWinTabEx(    
                                        MMI_WIN_ID_T	win_id,
                                        uint32			*win_tab_ptr
                                        );

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
// 	Description : create pub list win tab
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreatePubwinTabEx(
                                     MMI_HANDLE_T applet_handle,
                                     CAF_GUID_T   guid,
                                     uint32		  *win_tab_ptr,
                                     ADD_DATA     add_data_ptr 
                                     );

/*****************************************************************************/
// 	Description : create pub list win tab
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreatePubwinTab(
                                   CAF_GUID_T   guid,
                                   uint32		*win_tab_ptr,
                                   ADD_DATA     add_data_ptr 
                                   );

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _MMK_APP_H_  */
