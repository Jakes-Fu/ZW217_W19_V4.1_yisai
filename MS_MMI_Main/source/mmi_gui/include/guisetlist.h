/*! @file guisetlist.h
@brief  此文件介绍了setlist控件的对外接口和数据类型 
@author xiaoqing.lu  
@version 1.0
@date 11/04/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/04/2009     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_SetList SetList(SetList)

-# @ref sec_setlist_function
-# @ref sec_setlist_def
-# @ref sec_setlist_create
-# @ref sec_setlist_function_list
-# @ref sec_setlist_app

@section sec_setlist_function 控件功能

Setting List控件作为可以直接显示当前选项参数值及属性标题的控件，具有如下属性：
-# 单选功能：Settinglist能包含多个选项，但当前只显示一个选项参数值。
-# 选项切换：Settinglist可以通过左右键切换选项，并显示切换后的内容，如果支持触笔，则可通过触笔点击左右箭头切换选项。
-# 当只有两项功能选项时，按Web键，自动切换。
-# 显示内容：可显示选项内容，包括图片、文字
-# 显示风格：当行显示，高度可调
-# 选项内容：Settinglist的选项内容可以是文本、图片。控件可包含一个标题信息。
-# 控件的左右箭头可根据需要设置显示和不显示
-# 控件可以设置是否循环
-# 当选项超过2项时，可以打开窗口，全屏或者半屏窗口都是可以的，但需要用户设置。建议同一项目风格统一
-# 打开窗口以列表形式显示，有标题和softkey。

@section sec_setlist_def 宏定义解析

@code
#define CREATE_SETLIST_CTRL(_CTRL_ID_) \
        MMK_CREATE_SETLIST, _CTRL_ID_
@endcode
- 用于定义setlist控件
    - _CTRL_ID_ 控件ID

@section sec_setlist_create 控件创建

Setlist控件支持动态创建和静态创建两种形式，静态创建时，需要把上述宏定义增加到窗口的Table中：
@code
WINDOW_TABLE( MMISAMPLE_SETLIST_TEST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleTestSetlistWinMsg),
    WIN_ID( MMISETLIST_TEST_WIN_ID ),            CREATE_SETLIST_CTRL(MMISETLIST_TEST_CTRL_ID),
        END_WIN
};
@endcode

动态创建时，调用函数 @ref GUISETLIST_CreateCtrlDyn
    - 用户动态创建时需要需要传入窗口句柄或id，控件id，横竖屏区域，静态创建只需要设置控件ID。

@section sec_setlist_function_list 关键接口介绍

-# @ref GUISETLIST_AddItem
-# @ref GUISETLIST_SetBackGround
-# @ref GUISETLIST_CreateCtrlDyn
-# 标题设置函数
    - @ref GUISETLIST_SetTitleInfo
    - @ref GUISETLIST_SetTitleTextId

@section sec_setlist_app 控件应用
Setlist有很多种用途，例如闹钟的类型、日程的类型等，如下图，图中Type下的控件为setlist，左右键可切换单次、每周、每月、每年等等：
@image html setlist_display1.jpg
图 2 44 Setlist控件显示例图
当触笔单击控件或者web键时，可以弹出如下窗口：
@image html setlist_display2.jpg
图 2 45 控件弹出半屏窗口
@image html setlist_display3.jpg
图 2 46 控件弹出全屏窗口
*******************************************************************************/

/*! @addtogroup setlistGrp SetList
@brief setlist模块
@details setlist模块
@{
*******************************************************************************/


#ifndef _GUI_SETLIST_H_
#define _GUI_SETLIST_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guicommon.h"
#include "guictrl.h"
#include "mmi_link.h"
#include "mmitheme_setlist.h"
#include "ctrlsetlist_export.h"
//#include "..\\..\\mmi_ctrl\\source\\Setlist\\h\\ctrlsetlist.h"

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
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
#define GUISETLIST_AddItem            CTRLSETLIST_AddItem
#define GUISETLIST_AddItemById        CTRLSETLIST_AddItemById
#define GUISETLIST_InsertItem         CTRLSETLIST_InsertItem
#define GUISETLIST_ModifyItem         CTRLSETLIST_ModifyItem
#define GUISETLIST_DeleteItem         CTRLSETLIST_DeleteItem
#define GUISETLIST_DeleteAllItem      CTRLSETLIST_DeleteAllItem
#define GUISETLIST_SetBackGround      CTRLSETLIST_SetBackGround
#define GUISETLIST_SetCurIndex        CTRLSETLIST_SetCurIndex
#define GUISETLIST_GetCurIndex        CTRLSETLIST_GetCurIndex
#define GUISETLIST_CreateCtrlDyn      CTRLSETLIST_CreateCtrlDyn
#define GUISETLIST_SetDisplayRect     CTRLSETLIST_SetDisplayRect
#define GUISETLIST_GetDisplayRect     CTRLSETLIST_GetDisplayRect
#ifdef TOUCH_PANEL_SUPPORT
#define GUISETLIST_SetTpRect          CTRLSETLIST_SetTpRect
#endif
#define GUISETLIST_SetBorder          CTRLSETLIST_SetBorder
#define GUISETLIST_SetFont            CTRLSETLIST_SetFont
#define GUISETLIST_SetVisible         CTRLSETLIST_SetVisible
#define GUISETLIST_SetCircular        CTRLSETLIST_SetCircular
#define GUISETLIST_SetArrow           CTRLSETLIST_SetArrow
#define GUISETLIST_CallBack           CTRLSETLIST_CallBack
#define GUISETLIST_SetTitleInfo       CTRLSETLIST_SetTitleInfo
#define GUISETLIST_SetTitleTextId     CTRLSETLIST_SetTitleTextId
#define GUISETLIST_SetCtrlState       CTRLSETLIST_SetCtrlState
#define GUISETLIST_SetStrEffect       CTRLSETLIST_SetStrEffect
#define GUISETLIST_SetNeedArrow       CTRLSETLIST_SetNeedArrow
#define GUISETLIST_SetFontAlign       CTRLSETLIST_SetFontAlign
#define GUISETLIST_SetStyleById       CTRLSETLIST_SetStyleById
#define GUISETLIST_ResetAllItemState  CTRLSETLIST_ResetAllItemState
#define GUISETLIST_GetItemState       CTRLSETLIST_GetItemState
#define GUISETLIST_SetItemState       CTRLSETLIST_SetItemState
#define GUISETLIST_SetDispText        CTRLSETLIST_SetDispText
#define GUISETLIST_SetDefaultText     CTRLSETLIST_SetDefaultText
#define GUISETLIST_GetSelectItemIndex CTRLSETLIST_GetSelectItemIndex
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _GUI_SETLIST_H_
 

/*@}*/
