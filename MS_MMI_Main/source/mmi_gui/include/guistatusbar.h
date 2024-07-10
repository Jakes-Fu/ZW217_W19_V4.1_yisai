/*! @file guistatusbar.h
@brief 此文件介绍了statusbar控件的对外接口和数据类型
@author xiaoqing.lu
@version 1.0
@date 12/23/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 12/23/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_StatusBar 状态栏(StatusBar)

-# @ref sec_statusbar_function
-# @ref sec_statusbar_create
-# @ref sec_statusbar_function_list
-# @ref sec_statusbar_app

@section sec_statusbar_function 控件功能

StatusBar即状态栏控件（Status bar control）。它将平台中各功能模块的状态显示给用户，使用户能够更为清晰、方便地查看手机各功能模块当前的状态，状态栏所显示的状态为用户最为关心的模块的状态，例如闹钟是否启动、蓝牙是否开启、是否有短信或彩信、充电状态等。

状态栏控件和其他控件有所不同，它是窗口的属性之一，每个窗口中只能同时存在一个状态栏控件。

状态栏主要功能如下：
-# 状态栏显示不同模块的标示，显示内容为图片、文字、图片与文字；每个标示作为一个Item。
-# 每个标示可根据需求，在触笔点击时显示对应的模块内容；
-# 状态栏可以显示在不同的窗口中；
-# 状态栏所显示的手机模块的状态一定是模块当前的实际状态；
-# 状态栏状态更新具有及时性；
-# 状态栏具有统一的特性，即所有窗口所显示的状态栏的状态都是相同的。在某一个窗口改变一个item状态时，其他窗口显示的状态栏对应的item状态随之改变。
-# 状态栏控件的item状态改变是依赖于标示的变化呈现给用户的，改变状态由APP控制，如果焦点窗口包含状态栏控件，那么会立即显示，控件在刷新的时候调用控件的刷新接口及时显示。
-# 在我们的平台的设计中，状态栏通常情况下位于窗口的顶部。

@section sec_statusbar_create 状态栏的创建

状态栏是用来显示平台模块状态的控件，每个窗口最多只能有一个状态栏控件，它实际上是作为窗口的一个属性存在于窗口的实例中。

状态栏作为窗口的属性，创建起来很简单，你只需要在窗口Table中增加一个宏就可以了，例如，我们在九宫格的主菜单中增加状态栏。
@code
WINDOW_TABLE(MAINMENU_ICON_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_WIN_ID),
    WIN_STATUSBAR,    CREATE_MENU_CTRL(MENU_MAINMENU_ICON,MAINMENUWIN_MENU_CTRL_ID),
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode

只要你在窗口的Table中增加了WIN_STATUSBAR属性，这个窗口在显示的时候就会显示状态栏。

当然，用户可以动态创建状态栏，例如，idle窗口是一个动态创建的窗口，那么它使用状态栏的时候，在Open消息时调用了GUIWIN_CreateStbDyn接口，该接口之需要传递窗口的ID即可。

状态栏作为一个GUI控件，它有固定的区域（窗口的最上端）和固定的控件ID，都是通过接口获取的，接口定义在mmitheme_statusbar.c文件中，实现如下：
- 获取控件的ID
@code
MMI_CTRL_ID_T MMITHEME_GetStatusBarCtrlId(void)
{
	return MMICOMMON_STATUSBAR_CTRL_ID;
}
@endcode

- 获取控件的显示区域
    - 函数GUI_BOTH_RECT_T MMITHEME_GetStatusBarBothRect (void)用来获取横竖屏两种情况的区域，用于设置给状态栏控件。控件会根据LCD的旋转角度自行显示在正确的位置。

其实固定的ID也决定了一个窗口最多只能有一个状态栏控件。这里也验证了，不同窗口是可以使用同一控件ID的。

@section sec_statusbar_function_list 关键接口介绍

状态栏在设计时作为窗口的属性，对外接口定义在guiwin.h中，用户如果需要修改状态栏的属性，可以调用guiwin接口。关键接口如下：
-# @ref GUIWIN_CreateStbDyn
-# @ref GUIWIN_UpdateStb
-# @ref GUIWIN_InitStbData
-# @ref GUIWIN_AppendStbItemData
-# @ref GUIWIN_SetStbItemVisible
-# GUIWIN_StbItemIsVisible
-# @ref GUIWIN_SetStbItemHandleFunc
-# @ref GUIWIN_SetStbItemIcon
-# @ref GUIWIN_SetStbItemIconCurFrame
-# @ref GUIWIN_SetStbItemIconValid
-# @ref GUIWIN_SetStbItemIconStyle
-# @ref GUIWIN_SetStbItemIconId

@section sec_statusbar_app 控件应用
状态栏控件可以存在于任何一个窗口，例如下图中在主菜单窗口中显示状态栏（窗口顶部）。
@image html statusbar_app_example.jpg
图 2 37 控件显示例图


*******************************************************************************/

/*! @addtogroup statusbarGrp StatusBar
@brief statusbar模块
@details 状态栏控件的头文件，该文件不对外开放，所有app都应该调用guiwin.h中的封装接口
@{
*******************************************************************************/

#ifndef _GUI_STATUSBAR_H_
#define _GUI_STATUSBAR_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "ctrlstatusbar_export.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                                                                                     */
/*----------------------------------------------------------------------------*/
#define GUISTATUSBAR_CreateCtrlDyn CTRLSTATUSBAR_CreateCtrlDyn
#define GUISTATUSBAR_SetBgImageId CTRLSTATUSBAR_SetBgImageId
#define GUISTATUSBAR_SetBg CTRLSTATUSBAR_SetBg
#define GUISTATUSBAR_UpdateCtrl CTRLSTATUSBAR_UpdateCtrl
#define GUISTATUSBAR_SetBgIsDesplay CTRLSTATUSBAR_SetBgIsDesplay
#define GUISTATUSBAR_ClearCustomerBg CTRLSTATUSBAR_ClearCustomerBg
#define GUISTATUSBAR_CustomCreate CTRLSTATUSBAR_CustomCreate
#define GUISTATUSBAR_SetCommonBg CTRLSTATUSBAR_SetCommonBg
#define GUISTATUSBAR_SetState CTRLSTATUSBAR_SetState
#define GUISTATUSBAR_GetState CTRLSTATUSBAR_GetState
#define GUISTATUSBAR_SetItemBg CTRLSTATUSBAR_SetItemBg
#define GUISTATUSBAR_GetCtrlLayer CTRLSTATUSBAR_GetCtrlLayer
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
#define GUISTATUSBAR_SetDropDownState CTRLSTATUSBAR_SetDropDownState
#define GUISTATUSBAR_IsDropDown CTRLSTATUSBAR_IsDropDown
#define GUISTATUSBAR_SetPressed CTRLSTATUSBAR_SetPressed
#define GUISTATUSBAR_IsPressed CTRLSTATUSBAR_IsPressed
#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                                                                                          */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _GUI_STATUSBAR_H_


/*@}*/
