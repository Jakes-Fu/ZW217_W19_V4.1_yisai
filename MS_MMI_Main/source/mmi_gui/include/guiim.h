/*! @file guiim.h
@brief 输入法控件的创建和操作相关的函数与类型 
@author Lianxiang.Zhou
@version 1.0
@date 2011/02/16
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@note 2009/08/08 - Lianxiang.Zhou - Create.
@sa imGrp
@sa @ref page_Im
@details 输入法控件针对控件使用层所提供的函数和类型。输入法控件是一个中间层的概念，管理和调用输入法应用，供需要输入的控件或者应用通过简单的接口统一的使用输入法。
- 统一，即对于所有的输入法应用无差别的使用
- 简单，将调用接口尽可能的简化
*******************************************************************************/

/*! @page page_Im 输入法(IM)

- 输入法控件非常用控件。
- 在输入法控件的调用上，通常由系统的edit控件封装了。
- 在输入法应用的制作上，一些高级用户会使用到，但通常用户不会涉及输入法应用的开发与维护。
- 本着文档在紧要处着笔的原则，此处重点是原理介绍，而本控件实现简单，原理相对复杂，所以，不可能再这样的文档中完成全部的细节。所以，如果有特别需要，请参考专业文档。

-# @ref sec_im_mode
-# @ref sec_im_relationship
-# @ref sec_im_usgae
-# @ref sec_im_add_app

@section sec_im_mode 系统目前所采用的输入法模式(Mode of Input Module)

我们系统现在的输入法通过输入法控件管理输入法应用，而输入法应用则是依赖输入法引擎来完成。输入法引擎也可以叫做输入法内核，或者简单的叫做api。总之，就是提供一些输入法内部字符串转化的函数库。

这样说来，我们必须弄清楚3个概念：
- 输入法控件
- 输入法应用
- 输入法内核
另外，我们还必须理清楚2个概念：
- 输入方式
- 输入方法

好，下面我们来简单的叙述一下这5个概念
-# 输入法控件(IMC - Input Method Controller)
    - 输入法控件是我们平台上的一个普通控件，与其他控件没什么不同。该控件只支持动态创建。通常由编辑应用即需要启动输入法的应用来创建。输入法控件的主要功能就是连接编辑应用和输入法应用，使得编辑应用无需关心输入法应用的任何细节，同样，输入法应用也完全不要关心编辑应用的需求。这样一来，就使得模块化增强了。
    - 显然，编辑应用不是我们输入法模块中的一部分，他只是我们的最终客户而已。而要对这个最终客户负责的，有且仅有输入法控件。
    - 为了使得这样一套机制得以实现，就必须要求输入法应用按照既定的格式去建立。即按照输入法控件提供的格式去完成一套api，并且处理相应的交互事件。细节容后再禀。
-# 输入法应用(IMA - Input Method Application)
    - 这里说的输入法应用，是在当前这套体制下的输入法应用，需要按照一定的格式来完成。这个格式是输入法控件所制定的，为了方便输入法控件的管理。
    - 一个输入法应用会依赖一些输入法的引擎去完成，比如获得拼音、笔划、联想字等。
    - 而这些引擎通常是由第三方提供的。不同的引擎的集成度是不一样的，我们必须根据具体的内核来安排我们的应用，同时，还必须遵循UI需求规定的样式去完成。
    - 所以，输入法应用工程师是很委屈的一个角色，必须兼顾一下三方面的制约：
        -# 输入法控件
        -# 输入法引擎
        -# UI需求
-# 输入法内核(IME - Input Method Engine)
    - 也可以叫做输入法引擎，或者其他什么名字。总之，就是一个库，提高一系列的api，可以完成字符串到字符串的转化功能。通常是第三方提供，并且都会有详细的设计文档。一般来讲，要在某个内核上完成输入法应用，其user guide是肯定要看的。常见的有T9、S9、Cstar、点讯、搜狗和百度等等。
-# 输入方式(Input Type)
    - 我们称触笔输入、手写输入和按键输入等为输入方式。
-# 输入方法(Imput Method)
    - 我们成9键拼音按键输入、9键拼音触笔输入、手写输入或multi-tap ABC输入等等为输入方法，即具体到输入的时候所采用的方法的一些方法。

@section sec_im_relationship 各类关系(Relationship)

线性的关系是最好的，也是我们所追求的。

-# 输入法控件和输入方法之间的关系(IMC vs. IM)
    - 我们的系统会支持一些输入方法。我们称之为输入方法的集合S。
    - 输入法控件必须给集合S中的每一个输入方法指定一个且唯一一个输入法应用来完成该方法。
    - 也就是说，输入法控件并不关注输入方法的个体，而是针对于整个输入方法的集合S。并针对每一个输入方法配置相应的内容。
    - 这个配置，我们的系统是在mmitheme_im.c中来完成的，打开这个文件，就能看到一个很大的配置表格。
-# 输入法应用和输入方法之间的关系(IMA vs. IM)
    - 一个输入法应用至少要完成一个输入方法的输入，否则，这个输入法应用就没有意义了。因为输入法应用就是完成输入方法的程序。
    - 当然，一个输入法应用可以完成一个或者多个输入方法。
    - 同样，两个不同的输入法应用是可以完成同一个输入方法的，但是两个输入法应用之间是没有关系的，独立的。
    - 可以说，输入法应用和输入方法之间是n-n的映射关系。
-# 输入法应用和输入法内核之间的关系(IMA vs. IME)
    - 输入法应用为了完成一些特殊的输入方法，必须依赖输入法内核才能完成。通常，输入法内核会完成复杂的运算，而输入法应用更多的关注ui显示及用户交互、用户体检部分的功能。一个输入法应用可能会用到一个或者多个输入法内核，比如手写输入法应用，需要用到手写输入的内核和联想字的内核。一个输入法应用也可能不需要特殊的输入法内核，当其输入不是很复杂的时候，比如我们系统中的multi-tap输入法应用，就是自身完成了并不是很复杂的内核功能，而无需依赖其他的内核。
-# 输入法控件和输入法应用之间的关系(IMC vs. IMA)
    - 输入法控件和输入法应用之间是通过输入方法来连接的。输入法控件必须为每一个输入方法制定唯一的输入法应用，在编辑应用要求启动输入法的时候去启动特定的输入法应用。这样一来，就会出现一些输入法应用所完成的输入方法没有被使用到的情况，这样是很正常的。
-# 用图表来表示的输入法各部分之间的关系(Diagram)
@image html im_relationship.jpg

@section sec_im_usgae 使用输入法控件

输入法控件的调用通常都是系统集成在edit控件的，其调用细节，请参考相关的文档。

@section sec_im_add_app 添加输入法应用
添加输入法应用是一部分高级用户的需求，需要很多的篇幅来叙述，请参考相关的文档。
*******************************************************************************/

/*! @addtogroup imGrp Im
@brief 输入法模块
@sa @ref page_Im
@details 这个模块涉及两个方向，三个概念。
- 两个方向
    -# 向上，针对输入法控件的使用者
    -# 向下，针对输入法控件的调用对象——输入法应用
- 三个概念
    -# 输入法控件的使用者
    -# 输入法控件本身
    -# 输入法控件的调用对象——输入法应用

这个模块只包含输入法控件本身，对于使用者和调用对象，则是制定统一的函数接口。

@{
*******************************************************************************/
#ifndef _GUIIM_H_
#define _GUIIM_H_

/*------------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guiim_base.h"
#include "mmitheme_im.h"
#include "ctrlim_export.h"

/*------------------------------------------------------------------------------
    Macro Declaration
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    TYPE AND STRUCT
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif

/*------------------------------------------------------------------------------
    FUNCTION DEFINITION
------------------------------------------------------------------------------*/
#define GUIIM_ImGetText         CTRLIM_ImGetText
#define GUIIM_GetImRect         CTRLIM_GetImRect
#define GUIIM_GetInput          CTRLIM_GetInput
#define GUIIM_GetType           CTRLIM_GetType
#define GUIIM_GetCapital        CTRLIM_GetCapital
#define GUIIM_SetCapital        CTRLIM_SetCapital
#define GUIIM_SetInput          CTRLIM_SetInput
#define GUIIM_SetType           CTRLIM_SetType
#define GUIIM_SetInputSet       CTRLIM_SetInputSet
#define GUIIM_SetTypeSet        CTRLIM_SetTypeSet
#define GUIIM_ClearState        CTRLIM_ClearState
#define GUIIM_DisplaySoftkey    CTRLIM_DisplaySoftkey
#define GUIIM_HandleMsg         CTRLIM_HandleMsg
#define GUIIM_SetCustomKey      CTRLIM_SetCustomKey
#define GUIIM_IsOnlyInputAscii  CTRLIM_IsOnlyInputAscii
#define GUIIM_CommitLast        CTRLIM_CommitLast
#define GUIIM_SetInputSymbol    CTRLIM_SetInputSymbol
#define GUIIM_SetImTag          CTRLIM_SetImTag
#define GUIIM_CanSwithToTp      CTRLIM_CanSwithToTp
#define GUIIM_GetImMethodParam  CTRLIM_GetImMethodParam
#define GUIIM_GetImHandle       CTRLIM_GetImHandle

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef __cplusplus 
} 
#endif 


#endif //_GUIIM_H_

/*@}*/
