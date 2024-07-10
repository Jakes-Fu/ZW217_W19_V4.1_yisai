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

//缓冲的索引页数
#define SPIM_IDX_BUF_PAGE_CNT (100)


//类型定义
//unicode是16位无符号数
typedef unsigned short SPIM_UNICODE_T;
//通用的handle类型
typedef void* SPIM_HANDLE_T;


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
typedef unsigned int SPIM_RESULT_T;


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
    SPIM_EVENT_GET_CONTENT,

    SPIM_EVENT_MAX
};
typedef unsigned int SPIM_EVENT_T;


//事件参数
typedef union
{
    char append;
    char select;
} SPIM_EVENT_PARA_T;


//翻页标识定义
enum
{
    SPIM_PAGE_FLAG_NEXT = 0x01,
    SPIM_PAGE_FLAG_PREVIOUS = 0x02,
};
typedef unsigned int SPIM_PAGE_FLAG_T;


#endif //_SPIM_TYPE_H_


