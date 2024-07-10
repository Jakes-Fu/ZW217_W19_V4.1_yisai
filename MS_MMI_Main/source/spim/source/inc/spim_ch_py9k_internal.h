/*==============================================================================
File Name: spim_ch_py9k_internal.h
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

#ifndef _SPIM_CH_PY_9K_INTERNAL_H_
#define _SPIM_CH_PY_9K_INTERNAL_H_

#include "spim_type.h"

#include "spim_ch_py9k.h"


// idx_flag 中用来保存匹配数字长度的部分
#define SPIM_CH_PYW_9K_IDX_LEN_MASK (0x1F)
// 该 idx 是拼音索引号，还是匹配的数字串， 0 - 拼音索引 1-匹配数字
#define SPIM_CH_PYW_9K_IDX_NUM_FLAG_MASK (0x20)


/*
因为只能匹配9k的时候，长度是未知的，且不可确定的
match_idx_count - 匹配索引的个数，所有的数字算一个
*/
typedef struct
{
    SPIM_DICT_IDX_T dict;
    SPIM_UINT8 page;            // 其实是词长 - 1
    SPIM_UINT16 idx;
    SPIM_UINT8 match_idx_count;
    SPIM_UINT8 match_num_count;
} GUIME_CH_PYW_CAND_9K_T;

//core data(hide)
typedef struct
{
    SPIM_INT8 input[SPIM_INPUT_LEN_MAX];  //数字串，输入的原始数字
    SPIM_UINT8 input_len; //输入长度，即input的len
    SPIM_PY_IDX_T idx[SPIM_INPUT_LEN_MAX]; //已经确定的数字，即已经转化成了拼音。两种 1 指定的，拼音索引号 2 匹配的，数字起点
    SPIM_UINT8 idx_flag[SPIM_INPUT_LEN_MAX]; // 是通过py cand指定的，还是匹配的串所覆盖的数字
    SPIM_UINT8 idx_cnt; //索引个数，指定的一个算一个，匹配的一次算一个
    SPIM_UINT8 idx_confirm_len; //索引包含的输入长度
    GUIME_CH_PYW_CAND_9K_T confirm[SPIM_INPUT_LEN_MAX]; //提交索引  // 已经确认为汉字的候选字
    SPIM_UINT8 confirm_cnt; //确认的汉字的单位数，一个词算一个单位  // 已经确认为汉字的个数
    SPIM_UINT8 confirm_idx_cnt; //确认的索引的数目，一个拼音算一个，一段模糊匹配的字符也算一个
} SPIM_CH_PYW_INPUT_9K_T;

typedef struct
{
    GUIME_CH_PYW_CAND_9K_T cand[SPIM_INPUT_LEN_MAX];
    SPIM_UINT8 cand_cnt;        // 候选字的个数
    SPIM_UINT16 cand_win_width;
    SPIM_INT8 is_searching;     // 0代表已经找遍了所有的字，1代表还有字没有被选择出来，下次可以继续
} SPIM_CH_PYW_OUTPUT_9K_T;


typedef struct
{
    SPIM_PY_IDX_T idx[SPIM_CH_PY9K_COMBINE_MAX];    // 候选拼音在9键拼音表中的索引
    SPIM_UINT8 cnt;                                 // 已找到的索引个数，也代表上述数组的偏移
} SPIM_CH_PYW_9K_PY_CAND_T;

/*
first idx 缓存每个显示页的首个索引，其他索引不缓存，通过搜索得到
*/
typedef struct
{
    GUIME_CH_PYW_CAND_9K_T first_idx[SPIM_IDX_BUF_PAGE_CNT];
    SPIM_UINT8 page_cnt;
    SPIM_UINT8 cur_page;
} SPIM_CH_PYW_BUFFER_9K_T;

typedef struct
{
    SPIM_INIT_PARAMETER_T init_para;        //初始化参数

    SPIM_CH_PYW_INPUT_9K_T input;           //输入

    SPIM_CH_PYW_9K_PY_CAND_T py_cand;       //候选

    SPIM_CH_PYW_OUTPUT_9K_T output;         //输出
    SPIM_CH_PYW_OUTPUT_9K_T test;

    SPIM_CH_PYW_BUFFER_9K_T buffer;         //翻页用

    SPIM_CH_DICT_CFG_T const *dict_cfg;     // 字典数组
    SPIM_UINT32 dict_size;                  // 字典数

    SPIM_INT16 const *py_cnt;               //当前字符集的各拼音下【字的个数】
    SPIM_INT16 const **ch_pronc_pptr;       //当前字符集的各读音
    SPIM_INT8 const **ch_key_list_pptr;     //当前字符集下的各按键位
    SPIM_UINT32 ch_pronc_size;              //有多少的读音，以上三者的个数都应相同

    SPIM_INT8 const *key_name_ptr;          //键值
    SPIM_UINT32 key_name_size;              //键值个数

    SPIM_INT16 const **ch_ic_pptr;          //辅音集
    SPIM_UINT32 ch_ic_size;                 //辅音个数

    SPIM_CH_DICT_CFG_T const *ch_key_dict_cfg_ptr;  //字符集的键位
    SPIM_UINT32 ch_dict_cfg_size;                   //个数

    SPIM_INT8 separator_char;               //分隔符

} SPIM_PY9K_INFO_T;






#endif //_SPIM_CH_PY_9K_INTERNAL_H_



