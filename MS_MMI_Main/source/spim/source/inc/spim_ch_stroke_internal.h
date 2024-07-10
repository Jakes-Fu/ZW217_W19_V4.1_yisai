/*==============================================================================
File Name: spim_ch_stroke_internal.h
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
#ifndef _SPIM_CH_STROKE_INTERNAL_H_
#define _SPIM_CH_STROKE_INTERNAL_H_

#include "spim_type.h"
#include "spim_ch_stroke.h"

//core data(hide)
typedef struct
{
    SPIM_INT8 input[SPIM_INPUT_LEN_MAX];  //数字串
    SPIM_UINT8 input_len; //输入长度
    SPIM_UNICODE_T commit;
} SPIM_CH_STROKE_INPUT_T;

typedef struct
{
    SPIM_DICT_INDEX_T cand[SPIM_CANDIDATE_CNT_MAX];
    SPIM_UINT8 cand_cnt;
    SPIM_INT8 is_searching;
} SPIM_CH_STROKE_OUTPUT_T;

/*
first idx 缓存每个显示页的首个索引，其他索引不缓存，通过搜索得到
*/
typedef struct
{
    SPIM_DICT_INDEX_T first_idx[SPIM_IDX_BUF_PAGE_CNT];
    SPIM_UINT8 page_cnt;
    SPIM_UINT8 cur_page;
} SPIM_CH_STROKE_BUFFER_T;

//字段数据结构
typedef struct
{
    SPIM_UNICODE_T ch;
    SPIM_INT8 const *stroke;
} SPIM_CH_STROKE_DATA_T;


//实例数据
typedef struct
{
    SPIM_CH_STROKE_DATA_T const *data_ptr;
    SPIM_INT32 data_size;

    SPIM_CH_STROKE_INPUT_T input;
    SPIM_UINT8 cand_cnt_per_page; //每次能给出的最大候选字数

    SPIM_CH_STROKE_OUTPUT_T output;
    SPIM_CH_STROKE_OUTPUT_T test;

    SPIM_CH_STROKE_BUFFER_T buffer;
} SPIM_CH_STROKE_INFO_T;






#endif //_SPIM_CH_STROKE_INTERNAL_H_




