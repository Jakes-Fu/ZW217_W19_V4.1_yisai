/*==============================================================================
File Name: spim_symbol_internal.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 符号输出的内部数据定义
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_SYMBOL_INTERNAL_H_
#define _SPIM_SYMBOL_INTERNAL_H_

#include "spim_type.h"



//符号分类的配置
typedef struct
{
    SPIM_UNICODE_T const *symb_list;
    SPIM_UINT32 symb_cnt;
}SPIM_SYMBOL_CFG_T;



#endif /*_SPIM_SYMBOL_INTERNAL_H_*/




