/*==============================================================================
File Name: spim_english9k.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 英文9键的外部函数
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_SP_ENGLISH9K_H_
#define _SPIM_SP_ENGLISH9K_H_

#include "spim_spells9k.h"

//init
/*==============================================================================
Function Name: SPIM_SPELLS9K_InitEnglish
Description: 初始化实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_HANDLE_T SPIM_SPELLS9K_InitEnglish(SPIM_INIT_PARAMETER_T const *para_ptr);
//term
/*==============================================================================
Function Name: SPIM_SPELLS9K_Term
Description: 释放实例
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 此处所有的拼写式外语是一致的
==============================================================================*/
void SPIM_SPELLS9K_Term(SPIM_HANDLE_T handle);
//handle
/*==============================================================================
Function Name: SPIM_SPELLS9K_Handle
Description: 实例事件处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 此处所有的拼写式外语是一致的
==============================================================================*/
SPIM_RESULT_T SPIM_SPELLS9K_Handle(
    SPIM_HANDLE_T * handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    SPIM_CONTENT_T * content_ptr
);


#endif //_SPIM_SP_ENGLISH9K_H_





