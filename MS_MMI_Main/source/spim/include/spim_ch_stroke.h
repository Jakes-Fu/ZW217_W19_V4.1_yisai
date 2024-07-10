/*==============================================================================
File Name: spim_ch_stroke.h
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 笔划输入对外函数
尚未区分简体繁体
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_CH_STROKE_H_
#define _SPIM_CH_STROKE_H_

#include "spim_type.h"


//function list
//init
/*==============================================================================
Function Name: SPIM_CH_STROKE_Init
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_CH_STROKE_Init(SPIM_INIT_PARAMETER_T const *para_ptr);
//term
/*==============================================================================
Function Name: SPIM_CH_STROKE_term
Description: 释放实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void SPIM_CH_STROKE_term(SPIM_HANDLE_T handle);
//handle
/*==============================================================================
Function Name: SPIM_CH_STROKE_Handle
Description: 事件的处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_RESULT_T SPIM_CH_STROKE_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
);




#endif //_SPIM_CH_STROKE_H_





