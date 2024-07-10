/*==============================================================================
File Name: spim_ch_asso_internal.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 生成联想字集合。从联想字数据库中检索当前字的联想字
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_CH_ASSO_INTERNAL_H_
#define _SPIM_CH_ASSO_INTERNAL_H_

#include "spim_type.h"



typedef struct
{
    SPIM_UNICODE_T ch;
    SPIM_UNICODE_T const *asso;
} SPIM_CH_ASSO_DICT_T;





#endif //_SPIM_CH_ASSO_INTERNAL_H_



