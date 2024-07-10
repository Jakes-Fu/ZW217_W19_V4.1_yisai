/*==============================================================================
File Name: spim_type.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: spim的基础数据和数据结构定义，此处定义使得spim不依赖系统
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_TYPE_H_
#define _SPIM_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "spim_general.h"

/* 常数定义 */
enum
{
    //缓冲的索引页数
    SPIM_IDX_BUF_PAGE_CNT = 100,

    //可输入的字符数
    SPIM_INPUT_LEN_MAX = 64,

    //可提交的字符数
    SPIM_COMIT_LEN_MAX = SPIM_INPUT_LEN_MAX,
    //预编译字符拼接最大长度
    SPIM_PRE_EDIT_LEN_MAX = 2*SPIM_INPUT_LEN_MAX,
    //候选字拼接最大长度
    SPIM_CANDIDATE_LEN_MAX = 64,
    //候选拼音拼接最大长度
    SPIM_PY_CHOICE_LEN_MAX = 140,
    
    //候选字个数
    SPIM_CANDIDATE_CNT_MAX = 16,

    //拼音分割显示符
    SPIM_PY_SEG_SYMBOL = '\'',

    //提交输入字符的标识
    SPIM_COMMIT_INPUT_FLAG = -1,
};

// 输入法类型
typedef enum
{
    SPIM_TYPE_PINYIN_9KEY = 0,
    SPIM_TYPE_PINYIN_26KEY,
    SPIM_TYPE_ZHUYIN_9KEY,
    SPIM_TYPE_ZHUYIN_26KEY
} SPIM_TYPE_E;


//类型定义
//unicode是16位无符号数
typedef SPIM_UINT16 SPIM_UNICODE_T;

//通用的handle类型
typedef void* SPIM_HANDLE_T;

//字典索引类型
typedef SPIM_INT32 SPIM_DICT_INDEX_T;



//普通的spim返回值
enum
{
    //常用返回值
    SPIM_NO_ERROR,
    SPIM_ERROR,

    //特定信息的返回值
    SPIM_PARA_ERROR,

    SPIM_UNKNOW_ERROR
};
typedef SPIM_UINT32 SPIM_RESULT_T;


enum
{
    SPIM_CAPITAL_LOWER,
    SPIM_CAPITAL_LEAD_UPPER,
    SPIM_CAPITAL_UPPER,
};
typedef SPIM_UINT8 SPIM_CAPITAL_MODE_T;

//spim的普通事件列表
enum
{
    SPIM_EVENT_CHAR_APPEND,
    SPIM_EVENT_CHAR_REMOVE,
    SPIM_EVENT_CHAR_REMOVE_ALL,
    SPIM_EVENT_CAND_SELECT,
    SPIM_EVENT_PINYIN_SELECT,
    SPIM_EVENT_PAGE_PREVIOUS,
    SPIM_EVENT_PAGE_NEXT,
    SPIM_EVENT_COMMIT_INPUT,
    SPIM_EVENT_SET_CAPITAL,
    SPIM_EVENT_GET_CONTENT,

    SPIM_EVENT_MAX
};
typedef SPIM_UINT32 SPIM_EVENT_T;


//事件参数
typedef union
{
    SPIM_INT8 append;
    SPIM_INT8 select;
    SPIM_CAPITAL_MODE_T capital;
} SPIM_EVENT_PARA_T;


//翻页标识定义
enum
{
    SPIM_PAGE_FLAG_PREVIOUS = 0x01,
    SPIM_PAGE_FLAG_NEXT = 0x02
};
typedef SPIM_UINT32 SPIM_PAGE_FLAG_T;

/*
简单的初始化参数模式
处理不等宽字符时，根据应用提供的获取字符串宽度的函数来计算字符串宽度
处理等宽字符时，为了效率，一般优先使用字符宽度
!!! 通常认为只有汉字是等宽的
*/
typedef SPIM_UINT32 (*SPIM_STRING_WIDTH_F)(SPIM_UNICODE_T *ptr, SPIM_UINT32 len);

typedef struct
{
    SPIM_UINT16 width_win;
    SPIM_UINT8 width_chr;
    SPIM_UINT8 width_spc;
    SPIM_STRING_WIDTH_F string_width_f;
    SPIM_TYPE_E type;
} SPIM_INIT_PARAMETER_T;

/*
通用的输入法上下文结构
会冗余各种输入法中可能会出现的域

*/
typedef struct
{
    /* 明确标识是否有数据输出 */
    SPIM_INT32 has_data;

    /* 提交数据 */
    SPIM_UNICODE_T commit[SPIM_COMIT_LEN_MAX];
    SPIM_UINT32 commit_len;

    /* 预编译部分，用'\0'分割 */
    SPIM_UNICODE_T pre_edit[SPIM_PRE_EDIT_LEN_MAX];
    SPIM_UINT32 pre_edit_cnt;

    /* 候选项，用'\0'分割 */
    SPIM_UNICODE_T candidate[SPIM_CANDIDATE_LEN_MAX];
    SPIM_UINT32 cand_cnt;

    /* 拼音候选项，用'\0'分割 */
    /* !!!此处为了内核的简单处理，将拼音候选项一次给出 */
    SPIM_UNICODE_T py_choice[SPIM_PY_CHOICE_LEN_MAX];
    SPIM_UINT32 py_cnt;

    /* 候选字的翻页标志 */
    SPIM_PAGE_FLAG_T page_flag;
} SPIM_CONTENT_T;



#endif //_SPIM_TYPE_H_


