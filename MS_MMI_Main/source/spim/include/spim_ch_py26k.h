/*==============================================================================
File Name: spim_ch_py26k.h
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 26键拼音的对外函数
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_CH_PY26K_H_
#define _SPIM_CH_PY26K_H_

#include "spim_type.h"


//function list
/*==============================================================================
Function Name: SPIM_CH_PY26K_Init
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_CH_PY26K_Init(SPIM_INIT_PARAMETER_T const *para_ptr);

/*==============================================================================
Function Name: SPIM_CH_PY26K_term
Description: 释放实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void SPIM_CH_PY26K_term(SPIM_HANDLE_T handle);

/*==============================================================================
Function Name: SPIM_CH_PY26K_Handle
Description: 实例事件处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_RESULT_T SPIM_CH_PY26K_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
);




#endif //_SPIM_CH_PY26K_H_



