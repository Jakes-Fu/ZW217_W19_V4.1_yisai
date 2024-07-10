/*! @file guitab.h
@brief 此文件介绍了tab控件的对外接口和数据类型 
@author Guanyong.Zhang
@version 1.0
@date 06/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 
-# 2007        Guanyong.Zhang     Create                                   *
-# 10/2008        Xiaoqing.Lu        Modified    
*******************************************************************************/

/*! @page page_Tab 选项卡(Tab)

-# @ref sec_tab_function
-# @ref sec_tab_def
-# @ref sec_tab_ctreate
    -# @ref ssec_tab_ctreate_win
    -# @ref ssec_tab_ctreate_cb
    -# @ref ssec_tab_ctreate_append
    -# @ref ssec_tab_ctreate_win2
    -# @ref ssec_tab_ctreate_run
    -# @ref ssec_tab_ctreate_function_list
-# @ref sec_tab_function_list
-# @ref sec_tab_dynamic_data
-# @ref sec_tab_app

@section sec_tab_function 控件功能

TAB控件又被称作选项卡控件。它将同类应用界面或窗口归类在一起为APP提供更为方便的界面设计。

主要功能描述如下：
-# 包含不同的选项卡（Item Card）；
-# 能通过切换选项卡切换不同的用户界面；
-# APP可根据需要动态增加选项卡的多少；
-# APP使用TAB控件区分不同的界面。

控件在设计时包括控件设计和选项卡设计，分别描述如下：
- 控件描述
    -# Tab控件依附于窗口；
    -# Tab控件原则上可以包含任意多的选项卡（Item Card，但目前平台最多支持20项）
    -# 通过Tab选项卡（Item Card）切换窗口（每个选项卡对应一个子窗口Child Window）；
    -# 可设置Tab控件的显示区域（Tab-rect）；
    -# Tab控件显示焦点Item card对应的文字（即选项卡的Caption）
    -# Tab控件可以左右切换Item，如果超过一页显示的项数，可以通过左右按键切换下一个选项。
    -# Tab控件可以设置左右切换的选项卡（Left-Card 和Right-Card），当项数不超过一页时，切换卡无效，当超过一页时，可通过触笔点击左右选项卡进行切换选项；
    -# 左右按键长按时能够快速切换选项卡；
    -# Tab控件的选项卡靠左侧显示；
    -# Tab控件显示附加的字符串，例如可以显示当前子窗口中的列表项数等，附加信息由应用设置。
- 控件选项卡描述
    -# 用户能够设置选项卡的缺省高度和宽度，这个缺省值是最小值；
    -# 选项卡的Caption，包括文字内容、文字颜色、显示区域、显示风格（左侧、居中、右侧）；
    -# 选项卡图片，包括系统资源图片ID、系统资源动画ID、图片数据buffer、图片存储路径；
    -# Item card在焦点状态下能够与非焦点状态时进行区分，当为item card非焦点状态下显示系统资源图片ID时，焦点状态显示对应的图片ID。其他情况下item card焦点状态下显示外围边框（边框颜色可自定义）
    -# Item card包含子窗口，Tab控件只显示焦点状态下的选项卡对应的子窗口；

@section sec_tab_def 宏定义解析

@code
#define CREATE_TAB_CTRL( _TAB_TYPE_, _TAB_STATE_, _CTRL_ID_) \
        MMK_CREATE_TAB, _TAB_TYPE_, _TAB_STATE_, _CTRL_ID_
@endcode

-用于创建选项卡控件。
    - _TAB_TYPE_ TAB的风格，实际上只支持靠左侧显示，该字段可以忽略
    - _TAB_STATE_ TAB的状态，一般为GUITAB_STATE_NULL，如果不需要左右选项卡，那么可设置GUITAB_STATE_NONEED_SCROLL状态。
    - _CTRL_ID_ 控件ID

@section sec_tab_ctreate 控件创建

本节我们讨论如何使用Tab控件，在创建控件的过程中，我们将逐步讨论父窗口、子窗口的概念。

本例的Tab控件包含两个选项卡（Item），当焦点在选项卡一时，显示“Hello SPRD1”；当焦点在选项卡二时，显示“Hello SPRD2”。

首先创建两个空文件，sample_tab_ctrl.c和sample_tab_ctrl.h，并分别放置在sample模块的相应c和h文件夹内。

@subsection ssec_tab_ctreate_win 第一步：创建窗口

-# TAB控件父窗口，注意需要添加窗口的ID和控件的ID
@code
WINDOW_TABLE( MMI_SAMPLE_TABCTRL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTabWinMsg),    
    WIN_ID( MMI_TEST_TAB_WIN_ID),

    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMI_TEST_TAB_CTRL_ID),

    END_WIN
};
@endcode
-# TAB1子窗口，即选项卡一的窗口，注意需要添加窗口的ID
@code
WINDOW_TABLE( MMI_SAMPLE_TAB1_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTab1ChildWindow),    
    WIN_ID( MMI_TEST_TAB1_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode
-# TAB2子窗口，即选项卡二的窗口，注意需要添加窗口的ID
@code
WINDOW_TABLE( MMI_SAMPLE_TAB2_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTab2ChildWindow),    
    WIN_ID( MMI_TEST_TAB2_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode

@subsection ssec_tab_ctreate_cb 第二步：实现窗口回调函数

-# 实现父窗口的回调函数
@code
LOCAL MMI_RESULT_E HandleTestTabWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E msg_id, 
                    DPARAM           param
                    )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_TAB_CTRL_ID);
        // 现Append TAB控件的选项卡
        AppendTabInfo();
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
@endcode
-# 实现Tab1子窗口回调函数
@code
LOCAL MMI_RESULT_E HandleTestTab1ChildWindow(
                        MMI_WIN_ID_T     win_id, 
                        MMI_MESSAGE_ID_E msg_id, 
                        DPARAM           param
                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUISOFTKEY_IMG_T    img_info = {0};
    
    switch(msg_id)
    {
case MSG_OPEN_WINDOW:
    // 这里是为了实现softkey的半透效果，用户可以暂时不必关注
        MMITHEME_GetHalfTransparentSoftkeyImg(&img_info);
        GUIWIN_SetHalfTransparent(TRUE, win_id, &img_info);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        {
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', '1', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);
            // 这里实现显示字符串
            DisplayString(w_text, len);
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMI_TEST_TAB_WIN_ID);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
@endcode
-# 实现Tab2子窗口的回调函数
@code
LOCAL MMI_RESULT_E HandleTestTab2ChildWindow(
                  MMI_WIN_ID_T     win_id, 
                  MMI_MESSAGE_ID_E msg_id, 
                  DPARAM           param
                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUISOFTKEY_IMG_T    img_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 这里是为了设置softkey的透明效果，可暂时不必关注
        MMITHEME_GetHalfTransparentSoftkeyImg(&img_info);
        GUIWIN_SetHalfTransparent(TRUE, win_id, &img_info);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        {
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', '2', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);
            // 这里实现显示字符串
            DisplayString(w_text, len);
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMI_TEST_TAB_WIN_ID);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
@endcode

@subsection ssec_tab_ctreate_append Append选项卡和显示字符串接口的实现

-# Append接口实现
@code
LOCAL void  AppendTabInfo(void)
{
MMI_STRING_T    str_text;
// 设置选项卡最大项数为2，申请选项卡内存
    GUITAB_SetMaxItemNum(MMI_TEST_TAB_CTRL_ID, 2);
    // 获取选项卡1的名称，注意字符串资源，需要添加
    MMI_GetLabelTextByLang(TXT_HELLO_TEST_TAB1_TEXT, &str_text);
    // Append选项卡1的信息，名称、选项卡按下和弹起时的图片，注意图片资源需要添加
    GUITAB_AppendSimpleItem(
        MMI_TEST_TAB_CTRL_ID,
        &str_text,
        IMAGE_TAB_SELECT1,
        IMAGE_TEST_TAB1_RELEASED);
    // 增加选项卡1对应的窗口ID
    GUITAB_AddChildWindow(MMI_TEST_TAB_CTRL_ID, MMI_TEST_TAB1_CHILD_WIN_ID, 0);
    // 获取选项卡2的名称，注意字符串资源，需要添加
MMI_GetLabelTextByLang(TXT_HELLO_TEST_TAB2_TEXT, &str_text);
// Append选项卡2的信息，名称、选项卡按下和弹起时的图片，注意图片资源需要添加
    GUITAB_AppendSimpleItem(
        MMI_TEST_TAB_CTRL_ID,
        &str_text,
        IMAGE_TAB_SELECT1,
        IMAGE_TEST_TAB2_RELEASED);
    // 增加选项卡2对应的窗口ID
    GUITAB_AddChildWindow(MMI_TEST_TAB_CTRL_ID, MMI_TEST_TAB2_CHILD_WIN_ID, 1);
    // 设置当前焦点的选项卡，这一步一定要设置，很重要
GUITAB_SetCurSel(MMI_TEST_TAB_CTRL_ID, 0);
// 设置焦点窗口，这个焦点窗口一定要和上面的当前选项卡对应
MMK_SetChildWinFocus(MMI_TEST_TAB_WIN_ID, MMI_TEST_TAB1_CHILD_WIN_ID);
}
@endcode
-# 显示字符串接口实现
@code
LOCAL void DisplayString(
                         uint16 *wstr_ptr, 
                         uint16 len
                         )
{
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T  start_point     = {1,140};
    GUI_RECT_T   rect            = {0, MMI_TAB_CHILD_WINDOW_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM};
    MMI_STRING_T    text_str  = {0};
    GUISTR_STYLE_T  text_style = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN;

    text_str.wstr_ptr = wstr_ptr;
    text_str.wstr_len = len;

    text_style.align = ALIGN_LEFT;
   text_style.font = SONG_FONT_16;
    text_style.font_color = MMI_BLACK_COLOR;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);

    GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T       *)& start_point,
                (const MMI_STRING_T     *)& text_str,
                &text_style,
                text_state
                );
}
@endcode

@subsection ssec_tab_ctreate_win2 第四步：创建窗口

@code
PUBLIC void MMIHELLO_CreateTabWin(void)
{
    // 创建父窗口
MMK_CreateParentWin((uint32*)MMI_SAMPLE_TABCTRL_WIN_TAB, PNULL);
// 创建子窗口1
MMK_CreateChildWin(MMI_TEST_TAB_WIN_ID, (uint32*)MMI_SAMPLE_TAB1_CHILD_WIN_TAB, NULL);
// 创建子窗口2
MMK_CreateChildWin(MMI_TEST_TAB_WIN_ID, (uint32*)MMI_SAMPLE_TAB2_CHILD_WIN_TAB, NULL);
// 设置当前的焦点窗口，这一步很重要，它必须和前面使用GUITAB_SetCurSel设置的当前项对应的窗口是同一个窗口，否则显示可能会错乱。
MMK_SetChildWinFocus(MMI_TEST_TAB_WIN_ID, MMI_TEST_TAB1_CHILD_WIN_ID);
}
@endcode

@subsection ssec_tab_ctreate_run 第五步：编译资源，编译运行模拟器

本例使用的资源以及控件和窗口的ID和以前的添加方式相同。

在hello sprd应用窗口增加消息处理：
@code
case MSG_APP_3:
        MMIHELLO_CreateTabWin();
        break;
@endcode
即在hello sprd窗口按软键3，打开tab测试窗口。结果如下图：
@image html tab_test_win.jpg
图 2 31 控件显示例图

当你在该窗口操作左右方向键的时候窗口将会显示“Hello SPRD1”和“Hello SPRD2”来回切换，你也可以用触笔点击选项卡来切换窗口。

@subsection ssec_tab_ctreate_function_list 第六步：外部接口介绍——子窗口和父窗口

在使用Tab控件时，我们用到了几个MMK的接口
- MMK_CreateParentWin是用来创建父窗口的函数，它和MMK_CreateWin的功能完全一样，只是名称不同，只是实现了MMK_CreateWin的封装。之所以取不同的名称，从目前平台来看这是为了更为清晰的来区分父子窗口的概念。
- MMK_CreateChildWin接口用来创建子窗口，和MMK_CreateWin相比较，它多了一个代表着父窗口的句柄，但是子窗口的创建是不会发送刷屏的消息的，换句话说，子窗口的刷新和父窗口的刷新密切相关，当父窗口接收Full Paint消息的时候，它会将该消息转发给处在焦点状态的子窗口，从而实现子窗口的刷新。
- MMK_SetChildWinFocus接口是用来设置父窗口下面的某一个子窗口处于焦点状态。
T
AB控件和两个子窗口都依附于父窗口，TAB控件的选项只记录了该选项卡对应的子窗口的ID，当切换选项卡的时候，TAB控件将发送调用MMK_WinGrabFocus接口来切换窗口的焦点，MMK_WinGrabFocus由控件调用，用户不必关心。用户关心的内容是当切换到某一个子窗口的时候，这个子窗口该做的事情。

@note TAB子窗口可以是同一个窗口也可以是不同的窗口，子窗口中可以创建任意控件。

@section sec_tab_function_list 关键接口介绍

-# @ref GUITAB_SetMaxItemNum
-# @ref GUITAB_AppendSimpleItem
-# @ref GUITAB_AddChildWindow
-# @ref GUITAB_SetCurSel
-# @ref GUITAB_AppendItem

@section sec_tab_dynamic_data 动态加载数据

TAB控件可以支持静态的资源图片来表现选项卡，就如同我们的事例程序一样，用户通过接口GUITAB_AppendSimpleItem来设置选项卡的信息，所传入的图片ID为静态资源图片。

TAB控件也可以支持一些动态图片数据，例如可以从u盘读取图片信息来作为选项卡的图片数据。
前文说过，TAB控件最多支持的选项卡数为20个，再加上选项卡的图片信息是动态的数据，这可能会造成同时申请的内存过来而导致调用GUITAB_AppendSimpleItem接口失败，为此我们提供了动态加载选项卡数据的方法，还曾记得Listbox控件吗？在Listbox控件的创建过程中也会遇到类似的问题，同样，我们也采用的动态加载数据的方法。

如果用户使用动态加载数据的方式，那么必须使用GUITAB_AppendItem接口添加Item的数据信息，该接口传入参数GUITAB_ITEM_T，数据结构表述如下：

-# GUITAB_IMAGE_DATA_U
-# GUITAB_IMAGE_DATA_T
-# GUITAB_ITEM_CONTENT_T
-# GUITAB_ITEM_T

Tab控件的Item项可以显示图片、文本或者两者都显示。

所谓的动态加载数据，实际上是动态加载图像数据，而文本数据如果用户要显示，则必须在append的时候就填入。

这里不得不讨论一下Item的两个状态：
- @ref GUITAB_ITEMSTATE_SHOW_IMG 仅仅显示图片
- @ref GUITAB_ITEMSTATE_SHOW_TEXT 仅仅显示文本

这两个状态设置了Item的显示类型，可以选择其一，也可以以“或”的关系都设置。

TAB控件是否需要动态加载不是由用户决定的，是由TAB控件自己决定的，即当图片的类型为TABITEM_DATA_ANIM_ID、TABITEM_DATA_ANIM_DATA或TABITEM_DATA_ANIM_PATH的时候，那么控件会使用动画控件来显示这些类型的图片，当控件发现其记录在GUITAB_ITEM_T结构中的add_data_ptr字段为0的时候，会自动发送MSG_CTL_TAB_NEED_ITEM_CONTENT消息给控件的父窗口，请注意，是父窗，和Item的子窗口没有关系。App在接收到这个消息之后，根据消息传递时所携带的参数，调用GUITAB_SetItemImage接口来设置图片的数据信息就可以了。

这样就实现了图片数据的动态加载。

TAB控件只记录当前页中Item的动画指针，当一个Item不在当前页的时候，控件会释放对应的动画指针和图片数据信息，以节省内存，当Item重新进入当前页的时候，控件会再次发送消息给APP，来重新获取图片数据信息，并创建动画。

@section sec_tab_app 控件应用
Tab控件主要使用在切换同种类型窗口的功能中，例如电话本、呼叫记录、信息的信箱、资源管理器等。如下图（在呼叫记录中的应用）：
@image html tab_app_example.jpg
图 2 32 控件显示例图
*******************************************************************************/

/*! @addtogroup tabGrp Tab
@brief tab控件
@details tab控件
@{
*******************************************************************************/

#ifndef _GUITAB_H_
#define _GUITAB_H_ 

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "mmk_app.h"
#include "guifont.h"
#include "guicommon.h"
#include "guictrl.h"
#include "guianim.h"
#include "guiprgbox.h"
#include "mmitheme_tab.h"
#include "ctrltab_export.h"
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#define GUITAB_SetMaxItemNum       CTRLTAB_SetMaxItemNum
#define GUITAB_AddChildWindow      CTRLTAB_AddChildWindow
#define GUITAB_AppendSimpleItem    CTRLTAB_AppendSimpleItem
#define GUITAB_InsertItem          CTRLTAB_InsertItem
#define GUITAB_DeleteItem          CTRLTAB_DeleteItem
#define GUITAB_DeleteAllItems      CTRLTAB_DeleteAllItems
//#define GUITAB_GetRect             CTRLTAB_GetRect
//#define GUITAB_SetRect             CTRLTAB_SetRect
#define GUITAB_GetItemNum          CTRLTAB_GetItemNum
#define GUITAB_SetSimpleItem       CTRLTAB_SetSimpleItem
#define GUITAB_SetCurSel           CTRLTAB_SetCurSel
#define GUITAB_GetCurSel           CTRLTAB_GetCurSel
#define GUITAB_GetCurItem          CTRLTAB_GetCurItem
#define GUITAB_SetBackGroundImage  CTRLTAB_SetBackGroundImage
#define GUITAB_SetBackGroundColor  CTRLTAB_SetBackGroundColor
#define GUITAB_SetFontParam        CTRLTAB_SetFontParam
#define GUITAB_SetAddedString      CTRLTAB_SetAddedString
#define GUITAB_GetAddedString      CTRLTAB_GetAddedString
#define GUITAB_SetState            CTRLTAB_SetState
#define GUITAB_GetState            CTRLTAB_GetState
#define GUITAB_SetItemState        CTRLTAB_SetItemState
#define GUITAB_GetItemState        CTRLTAB_GetItemState
#define GUITAB_SetImageState       CTRLTAB_SetImageState
#define GUITAB_GetImageState       CTRLTAB_GetImageState
#define GUITAB_SetItemContent      CTRLTAB_SetItemContent
#define GUITAB_SetItemImage        CTRLTAB_SetItemImage
#define GUITAB_SetItemText         CTRLTAB_SetItemText
#define GUITAB_SetAutoSwitch       CTRLTAB_SetAutoSwitch
#define GUITAB_GetAutoSwitch       CTRLTAB_GetAutoSwitch
#define GUITAB_SwitchToNextItem    CTRLTAB_SwitchToNextItem
#define GUITAB_AppendItem          CTRLTAB_AppendItem
#define GUITAB_SetItemSpace        CTRLTAB_SetItemSpace
#define GUITAB_SetItemWidth        CTRLTAB_SetItemWidth
#define GUITAB_SetTabRotative      CTRLTAB_SetTabRotative
#define GUITAB_SetDefaultIcon      CTRLTAB_SetDefaultIcon
#define GUITAB_SetItemValid        CTRLTAB_SetItemValid
#define GUITAB_SetFocusItemBgColor CTRLTAB_SetFocusItemBgColor
#define GUITAB_SetFocusItemTextBgImage CTRLTAB_SetFocusItemTextBgImage
#define GUITAB_SetStartIndex       CTRLTAB_SetStartIndex
#define GUITAB_GetStartIndex       CTRLTAB_GetStartIndex
#define GUITAB_SetSyncDecodeAnim   CTRLTAB_SetSyncDecodeAnim
#define GUITAB_SetItemDefaultHeight CTRLTAB_SetItemDefaultHeight
#define GUITAB_SetItemSelBgImage   CTRLTAB_SetItemSelBgImage
#define GUITAB_SetTitleInfo        CTRLTAB_SetTitleInfo
#define GUITAB_SetTitleTextId      CTRLTAB_SetTitleTextId
#define GUITAB_SetTitleIcon        CTRLTAB_SetTitleIcon
#define GUITAB_SetDividingLine     CTRLTAB_SetDividingLine
#define GUITAB_SetTpDownBg         CTRLTAB_SetTpDownBg
#define GUITAB_SetIsDisplayPrg     CTRLTAB_SetIsDisplayPrg

/***************************************************************************//*!
@brief 获取控件的区域
@author guanyong.zhang
@param ctrl_id [in] 控件ID
@retval 区域
@return 获得控件区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T GUITAB_GetRect(
                                 MMI_CTRL_ID_T  ctrl_id
                                 );

/***************************************************************************//*!
@brief set tab rect
@author Jassmine
@param ctrl_handle [in] 控件ID
@param rect_ptr [in] 控件ID
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void GUITAB_SetRect(
                           MMI_HANDLE_T     ctrl_handle,
                           GUI_RECT_T       *rect_ptr
                           );

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

/*@}*/
