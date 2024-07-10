/*! @file guitips.h
@brief 此文件介绍了tips控件的对外接口和数据类型
@author Xiaoqing.Lu
@version 1.0
@date 02/20/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 2/2009         Xiaoqing.Lu      Create
*******************************************************************************/

/*! @page page_Tips Tips(Tips)

-# @ref sec_tips_function
-# @ref sec_tips_create
-# @ref sec_tips_app

@section sec_tips_function 控件功能

Tips是窗口内容信息的提示控件，它能够帮助窗口提示一些附加信息，使得窗口的某些属性更为清晰地显示给用户。在一个窗口中，只存在一个唯一的tips控件。Tips控件不响应任何消息。

@section sec_tips_create Tips控件创建

和状态栏类似的，Tips控件也被看为窗口的一个属性，它的创建是调用WIN_TIPS宏，与状态栏一样，放置在窗口的Table中。当Tips需要显示数据的时候，它会发送MSG_CTL_TIPS_NEED_DATA消息给用户，并携带一定的参数，例如当前listbox的项索引号等，用户接收到这个消息之后填入相关数据。给Tips控件显示。

Tips控件比较简单，它不处理任何消息，也不响应用户的输入，它的作用就是显示提示信息，因此用户使用起来也比较简单。

虽然Tips是窗口的一个属性，但目前来看它和列表菜单和列表控件存在着紧密的关系，因此也只有在列表菜单和列表控件的窗口中使用Tips。

当用户接收到Tips消息时，会接收到一个消息参数，这个参数的实际类型如下：
- @ref GUITIPS_DATA_T

消息来时，这个参数携带的信息有控件的ID，当前item的索引号，用户可以决定是否显示Tips，以及在Tips上显示的icon id和字符串，用户只需要修改这个参数就可以了，而不需要自己申请额外的内存。

@section sec_tips_app 控件应用

例如，在电话本中，使用tips控件显示姓名为“高级别”的电话号码为“123456789”：
@image html tips_display.jpg
图 2 38 控件显示例图
*******************************************************************************/

/*! @addtogroup tipsGrp Tips
@brief tips控件
@details tips控件
@{
*******************************************************************************/
#ifndef _GUITIPS_H_
#define _GUITIPS_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mmitheme_tips.h"
#include "guictrl.h"

#include "ctrltips_export.h"
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
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define	GUITIPS_Update              CTRLTIPS_Update      
#define GUITIPS_SetEdgeRect         CTRLTIPS_SetEdgeRect 
#define	GUITIPS_SetTipsText         CTRLTIPS_SetTipsText 
#define	GUITIPS_SetTipsValid        CTRLTIPS_SetTipsValid
#define	GUITIPS_GetTipsValid        CTRLTIPS_GetTipsValid
#define	GUITIPS_SetTextColor        CTRLTIPS_SetTextColor
#define	GUITIPS_SetLcdInfo          CTRLTIPS_SetLcdInfo  

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif//_GUITIPS_H_
/*@}*/
