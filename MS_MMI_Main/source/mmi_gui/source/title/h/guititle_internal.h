/*! @file guititle_internal.h
@brief 此文件介绍了title控件的对外接口和数据类型 
@author xiaoqing.lu
@version 1.0
@date 11/05/2011
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/05/2011     xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_Title 标题(Title)

-# @ref sec_title_function
-# @ref sec_title_create
    -# @ref ssec_title_create_def
    -# @ref ssec_title_create_function_list
-# @ref sec_title_style
-# @ref sec_title_app

@section sec_title_function 控件功能

- 标题控件（Title）是一种特殊的GUI控件，一般情况下，它显示在窗口区域的最上面，但是如果窗口包含状态栏控件，那么Title控件显示在状态栏的下面。标题控件显示一个文本或者图标和文本的组合，表示窗口的名字和属性。
- 标题控件也是窗口的属性之一，一个窗口只有一个标题控件，它的ID也是固定不变的。
- 标题控件显示的内容包括主文本信息、附加文本信息、Icon、用于窗口切换的图标（如左右方向箭头icon）。
- 标题控件的主文本信息可以显示在标题控件区域的左、中、右三个位置。用户可以设置文本的风格，如文本字体大小、字体色彩、背景图片等。
- 标题控件的附加文本信息一般显示在标题控件的右下角，用户可以设置文本的内容、显示区域以及文字的显示风格。
- 标题控件的图标一般显示在标题控件区域的左侧，用来更进一步说明窗口。
- 用于切换窗口的Icon显示在标题控件的左右两端，注意，一般情况下它不与标题控件的图标同时显示。
- 标题是窗口的属性，用户可以通过接口获得某个窗口标题的信息。

@section sec_title_create 控件创建

标题和状态栏一样，是控件的属性之一，不过它的创建要相对的复杂一些，因为要填写文本的ID。

@subsection ssec_title_create_def 宏定义解析

@code
#define WIN_TITLE(_TITEL_ID_) \
		MMK_CREATE_TITLE, _TITEL_ID_
@endcode
- 创建一个标题控件。
    - _TITEL_ID_ 窗口文本的ID

@note 如果一个窗口需要显示标题，而这个标题是需要动态设置的，比如菜单控件会设置标题控件显示的文本，并随时更新。这样我们在窗口的Table中增加WIN_TITLE(TXT_NULL)，也就是说你可以传入空的文本。虽然为本内容为空，但还是要创建，否则，如果Title控件不被创建，也就无法动态设置Title控件的文本，除非你动态创建Title控件。

@subsection ssec_title_create_function_list 关键接口

Title控件的作为窗口的属性之一，对外接口定义在guiwin.h中。
- @ref GUIWIN_SetTitleText
- @ref GUIWIN_SetTitleTextId

@section sec_title_style 控件风格

标题控件有自己的风格，但是对于一个平台，它的标题应该具有统一的风格，如果需要修改标题的风格信息，只需要修改MMITHEME_GetTitleStyle中的初始化信息，该接口在mmi_theme.c中实现。

@section sec_title_app 控件应用

标题控件的应用：
-# 标示窗口：
@image html title_display1.jpg
图 2 39 控件显示例图
-# 显示附加信息和窗口内容，图中“2/8”显示当前焦点处在第2条，总数第8条：
@image html title_display2.jpg
图 2 40 控件显示例图
-# 显示切换窗口的图标，图中左右箭头可以切换下一条消息：
@image html title_display3.jpg
图 2 41 控件显示例图
*******************************************************************************/

/*! @addtogroup titleGrp Title
@brief title控件
@details title控件
@{
*******************************************************************************/

#ifndef _GUI_TITLE_INTERNAL_H_
#define _GUI_TITLE_INTERNAL_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guifont.h"
#include "guititle.h"
#include "mmi_font.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUITITLE_SetTextByPtr           CTRLTITLE_SetTextByPtr
#define GUITITLE_SetSubTextByPtr        CTRLTITLE_SetSubTextByPtr
#define GUITITLE_SetSubTextParamByPtr   CTRLTITLE_SetSubTextParamByPtr
#define GUITITLE_SetFontColorByPtr      CTRLTITLE_SetFontColorByPtr  
#define GUITITLE_SetRectByPtr           CTRLTITLE_SetRectByPtr
#define GUITITLE_SetIsVisible           CTRLTITLE_SetIsVisible
#define GUITITLE_GetTitlePtr            CTRLTITLE_GetTitlePtr
#define GUITITLE_SetButtonState         CTRLTITLE_SetButtonState
#define GUITITLE_SetButtonTextId        CTRLTITLE_SetButtonTextId
#define GUITITLE_SetButtonTextByPtr     CTRLTITLE_SetButtonTextByPtr
#define GUITITLE_SetButtonImage         CTRLTITLE_SetButtonImage
#define GUITITLE_SetButtonHandleMsgInfo CTRLTITLE_SetButtonHandleMsgInfo
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_GUI_TITLE_INTERNAL_H_
/*@}*/
