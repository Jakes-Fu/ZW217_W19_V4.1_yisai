/*! @file guisoftkey.h
@brief 此文件介绍了softkey控件的对外接口和数据类型 
@author jibin
@version 1.0
@date 05/08/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 05/08/2004     jibin       		Create
*******************************************************************************/

/*! @page page_Softkey Softkey(Softkey)

-# @ref sec_softkey_function
-# @ref sec_softkey_design
-# @ref sec_softkey_def
-# @ref sec_softkey_create
-# @ref sec_softkey_function_list
-# @ref sec_softkey_app
-# @ref sec_softkey_tips

@section sec_softkey_function 控件功能

- 控制窗口的Softkey，控件分成三个区域，分别和键盘上的左软键、Web键、右软键一一对应。Softkey控件属于窗口中的属性之一，和其他控件不同的是每个窗口只有一个Softkey控件，并且它和窗口中的其他控件关系紧密，当窗口中不同的控件处在焦点状态时，触笔点击Softkey左中右三个位置，可以分别发送不同的消息(与键盘消息对应)给焦点控件和窗口，并且Softkey可动态显示不同的文本信息。
- 另外，Softkey控件不接收焦点。

@section sec_softkey_design Softkey显示问题及解决

设计时考虑到，在通常情况下softkey控件限于显示区域问题，会出现显示不下的情况。因此显示区域的设计如下：
-# 当只有一个（左或者右）或两个（左和右）功能区域时，以中间等分成两个区域。
-# 当具有三个功能区域时，首先计算中间的文本宽度，但中间键的最大宽度不超过softkey宽度的1/3；
-# 可以支持显示icon；
-# 可以支持显示文字省略点

@section sec_softkey_def 宏定义解析

@code
#define WIN_SOFTKEY(_LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_) \
        MMK_CREATE_SOFTKEY, _LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_
@endcode
- 用于创建Softkey控件
    - _LEFT_SOFT_ID_ Softkey左软键Text ID
    - _MID_SOFT_ID_ Softkey中软键Text ID
    - _RIGHT_SOFT_ID_ Softkey右软键Text ID

@section sec_softkey_create 控件创建

- WIN_SOFTKEY有三个参数，分别是三个区域显示的文本信息，静态创建使用该宏，如果用户需要设置初始化的文本，那么可以在窗口Table中初始化的时候就填入，如果用户需要动态修改文本信息，则可以调用相关的设置接口，如GUIWIN_SetSoftkeyTextId。对于softkey控件，只显示固定资源的文本，也就是说，只能设置固定资源的文本ID作为softkey显示的内容。
- Softkey的背景是固定不变的，每个区域分别由两张背景图片，分为按下与弹起的效果两种状态。
- Softkey控件和Title控件一样，在一个平台中，应该具有统一的风格，如果用户需要修改softkey显示的图片以及文本风格，可以直接修改MMITHEME_GetSoftkeyStyle接口的实现。这个接口是在创建控件的时候被调用的，所有的softkey控件在创建时都要调用这个接口，这也能够保证控件的默认风格的统一性。当然，为了实现个性化的控件，用户可以在具体的窗口中设置不同风格的Softkey，这些接口如GUIWIN_SetSoftkeyStyle，GUIWIN_SetSoftkeyBtnState等。
- 平台中，有很多的控件和Softkey关联紧密，例如Editbox控件，当输入框内容为空的时候左软键不显示文本，当有内容的时候左软键显示“确定”，右软键显示“删除”，这些都是需要Editbox控件自己设置的。

@section sec_softkey_function_list 关键接口介绍

-# @ref GUIWIN_SetSoftkeyTextId
-# @ref GUIWIN_SetSoftkeyBtnState
-# @ref GUIWIN_SetSoftkeyTPEnable

@section sec_softkey_app 控件应用

- Softkey用于显示窗口左软键、Web键、右软键的文本信息。
- Softkey控件处于窗口下部的一条形区域，这一区域分成左中右3个区域，分别对应左软键、中软键、右软键，每个区域上可以显示文字。一般情况下只用到左右两个软键。Softkey控件显示效果如图 2 43所示，左软键显示”Option”，右软键显示”Back”，中间显示“Contacts”，触笔按下时会显示按下效果，触笔提起时响应操作。
@image html softkey_display.jpg
图 2 43 控件显示例图

@section sec_softkey_tips Softkey控件显示说明

-# 在没有中间键的时候，左右显示的字符可以达到一半的宽度
-# 有中间键的时候，中间键优先计算区域，但最大宽度不超过屏屏宽的1/3
-# 当显示文字超出自己的区域时，用户可设置省略显示
*******************************************************************************/

/*! @addtogroup softkeyGrp Softkey
@brief softkey控件
@details softkey控件
@{
*******************************************************************************/

#ifndef _GUI_SOFTKEY_H_
#define _GUI_SOFTKEY_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guianim.h"
#include "ctrlsoftkey_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUISOFTKEY_SetTextId               CTRLSOFTKEY_SetTextId
#define GUISOFTKEY_SetButtonState          CTRLSOFTKEY_SetButtonState
#define GUISOFTKEY_SetButtonGray           CTRLSOFTKEY_SetButtonGray
#define GUISOFTKEY_GetTextId               CTRLSOFTKEY_GetTextId
#define GUISOFTKEY_SetStyle                CTRLSOFTKEY_SetStyle
#define GUISOFTKEY_SetPressedStyle         CTRLSOFTKEY_SetPressedStyle
#define GUISOFTKEY_SetBgType               CTRLSOFTKEY_SetBgType
#define GUISOFTKEY_Update                  CTRLSOFTKEY_Update
#define GUISOFTKEY_IsTPEnable              CTRLSOFTKEY_IsTPEnable
#define GUISOFTKEY_SetTPEnable             CTRLSOFTKEY_SetTPEnable
#define GUISOFTKEY_SetVisible              CTRLSOFTKEY_SetVisible
#define GUISOFTKEY_GetVisible              CTRLSOFTKEY_GetVisible
#define GUISOFTKEY_SetButtonIconId         CTRLSOFTKEY_SetButtonIconId
#define GUISOFTKEY_SetButtonTextId         CTRLSOFTKEY_SetButtonTextId
#define GUISOFTKEY_Setfont                 CTRLSOFTKEY_Setfont
#define GUISOFTKEY_CreateCtrlDyn           CTRLSOFTKEY_CreateCtrlDyn
#define GUISOFTKEY_SetButtonTextPtr        CTRLSOFTKEY_SetButtonTextPtr
#define GUIBUTTONSOFTKEY_GetStyleFromText  CTRLBUTTONSOFTKEY_GetStyleFromText
#define GUIBUTTONSOFTKEY_GetButtonHandle   CTRLBUTTONSOFTKEY_GetButtonHandle
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
/*! @} */


