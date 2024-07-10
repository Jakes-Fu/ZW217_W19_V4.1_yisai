/*==============================================================================
File Name: spim_spells9k_internal.h
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

#ifndef _SPIM_SPELLS9K_INTERNAL_H_
#define _SPIM_SPELLS9K_INTERNAL_H_

#include "spim_type.h"

#include "spim_spells9k.h"

//每个键上的可选字符数
#define SPIM_SPELLS9K_KEYMAP_CNT (10)


//输入数据属性
typedef struct
{
    SPIM_INT8 input[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 input_len;
    SPIM_UNICODE_T confirm[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 confirm_len;
    SPIM_UINT8 has_commit;
    SPIM_DICT_INDEX_T commit;
    SPIM_UINT8 no_cand;
    SPIM_UNICODE_T keymap[SPIM_SPELLS9K_KEYMAP_CNT];
    SPIM_UINT8 keymap_cnt;
} SPIM_SPELLS9K_INPUT_T;


//输出数据属性
typedef struct
{
    SPIM_DICT_INDEX_T cand[SPIM_CANDIDATE_CNT_MAX];
    SPIM_UINT8 cand_cnt;
    SPIM_UINT16 cand_win_width;
    SPIM_INT8 is_searching;
} SPIM_SPELLS9K_OUTPUT_T;


/*
存储部分关键的临时数据，以提高检索速度
first idx 缓存每个显示页的首个索引，其他索引不缓存，通过搜索得到
*/
typedef struct
{
    SPIM_DICT_INDEX_T first_idx[SPIM_IDX_BUF_PAGE_CNT];
    SPIM_UINT8 page_cnt;
    SPIM_UINT8 cur_page;
} SPIM_SPELLS9K_BUFFER_T;

//core data(hide)
typedef struct
{
    SPIM_UNICODE_T const *alphabet_ptr;
    SPIM_INT32 alphabet_size;
    SPIM_INT8 const **vocabulary_ptr;
    SPIM_INT32 vocabulary_size;
    SPIM_INT8 const *keymap_ptr; //按照index定义的键值，这个是预先计算好的值，数据

    SPIM_INIT_PARAMETER_T init_para;
    SPIM_CAPITAL_MODE_T cap;

    SPIM_SPELLS9K_INPUT_T input;

    SPIM_SPELLS9K_OUTPUT_T output;
    //尝试输出数据
    SPIM_SPELLS9K_OUTPUT_T test;

    SPIM_SPELLS9K_BUFFER_T buffer;
} SPIM_SPELLS9K_INFO_T;






#endif //_SPIM_SPELLS9K_INTERNAL_H_




