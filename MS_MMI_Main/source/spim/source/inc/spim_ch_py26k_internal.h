/*==============================================================================
File Name: spim_ch_py26k_internal.h
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _SPIM_CH_PY_26K_INTERNAL_H_
#define _SPIM_CH_PY_26K_INTERNAL_H_

#include "spim_type.h"

#include "spim_ch_py26k.h"


//候选字数据结构
typedef struct
{
    SPIM_DICT_IDX_T dict;
    SPIM_UINT8 page;
    SPIM_UINT16 idx;
} GUIME_CH_PYW_CAND_T;


//输入数据属性定义
typedef struct
{
    SPIM_INT8 input[SPIM_INPUT_LEN_MAX];
    SPIM_PY_IDX_T idx[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 idx_flag[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 idx_cnt;
    SPIM_UINT8 confirm_cnt;
    SPIM_UNICODE_T confirm[SPIM_INPUT_LEN_MAX];
} SPIM_CH_PYW_INPUT_T;


//输出数据区定义
typedef struct
{
    GUIME_CH_PYW_CAND_T cand[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 cand_cnt;
    SPIM_UINT16 cand_win_width;
    SPIM_INT8 is_searching;
} SPIM_CH_PYW_OUTPUT_T;

/*
first idx 缓存每个显示页的首个索引，其他索引不缓存，通过搜索得到
*/
typedef struct
{
    GUIME_CH_PYW_CAND_T first_idx[SPIM_IDX_BUF_PAGE_CNT];
    SPIM_UINT8 page_cnt;
    SPIM_UINT8 cur_page;
} SPIM_CH_PYW_BUFFER_T;

//core data(hide)
typedef struct
{
    SPIM_CH_DICT_CFG_T const *dict_cfg;
    SPIM_UINT32 dict_size;
    SPIM_INT16 const *py_cnt; //该拼音下当前字符集的字的个数

    SPIM_INIT_PARAMETER_T init_para;

    SPIM_CH_PYW_INPUT_T input;

    SPIM_CH_PYW_OUTPUT_T output;
    SPIM_CH_PYW_OUTPUT_T test;

    SPIM_CH_PYW_BUFFER_T buffer;
} SPIM_PY26K_INFO_T;



#endif //_SPIM_CH_PY_26K_INTERNAL_H_



