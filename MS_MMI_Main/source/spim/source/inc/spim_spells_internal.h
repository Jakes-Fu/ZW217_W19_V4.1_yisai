/*==============================================================================
File Name: spim_spells_internal.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _SPIM_SPELLS_INTERNAL_H_
#define _SPIM_SPELLS_INTERNAL_H_

#include "spim_type.h"
#include "spim_spells.h"


//输入数据属性
typedef struct
{
    SPIM_UNICODE_T input[SPIM_INPUT_LEN_MAX];
    SPIM_INT8 index[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 input_len;
    SPIM_UINT8 has_commit;
    SPIM_DICT_INDEX_T commit;
    SPIM_UINT8 no_cand;
} SPIM_SPELLS_INPUT_T;



//输出数据属性
typedef struct
{
    SPIM_DICT_INDEX_T cand[SPIM_CANDIDATE_CNT_MAX];
    SPIM_UINT8 cand_cnt;
    SPIM_UINT16 cand_win_width;
    SPIM_INT8 is_searching;
} SPIM_SPELLS_OUTPUT_T;


/*
存储输出的部分关键临时数据，加快检索所读
first idx 缓存每个显示页的首个索引，其他索引不缓存，通过搜索得到
*/
typedef struct
{
    SPIM_DICT_INDEX_T first_idx[SPIM_IDX_BUF_PAGE_CNT];
    SPIM_UINT8 page_cnt;
    SPIM_UINT8 cur_page;
} SPIM_SPELLS_BUFFER_T;

//core data(hide)
typedef struct
{
    SPIM_UNICODE_T const *alphabet_ptr;
    SPIM_INT32 alphabet_size;
    SPIM_INT8 const **vocabulary_ptr;
    SPIM_INT32 vocabulary_size;

    SPIM_INIT_PARAMETER_T init_para;
    SPIM_CAPITAL_MODE_T cap;

    SPIM_SPELLS_INPUT_T input;

    SPIM_SPELLS_OUTPUT_T output;
    //尝试输出结果
    SPIM_SPELLS_OUTPUT_T test;

    SPIM_SPELLS_BUFFER_T buffer;
} SPIM_SPELLS_INFO_T;



#endif //_SPIM_SPELLS_INTERNAL_H_




