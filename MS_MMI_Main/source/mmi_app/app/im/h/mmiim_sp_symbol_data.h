/*==============================================================================
File Name: spim_symbol.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 符号输出的对外函数
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _MMIIM_SP_SYMBOL_DATA_H_
#define _MMIIM_SP_SYMBOL_DATA_H_

#include <stdlib.h>
#include "sci_types.h"
#include "caf.h"


#define MMIIM_SP_SYMBOL_SUPPORT_FACE (0)

#if defined(IM_HINDI_SUPPORT)
#define MMIIM_SP_SYMBOL_SUPPORT_HINDI (1)
#else
#define MMIIM_SP_SYMBOL_SUPPORT_HINDI (0)
#endif

/* 欧洲语言的符号是特定的集合，一般西欧语言都会启用相同的符号集合 */
#define MMIIM_SP_SYMBOL_SUPPORT_EUR (0)

#if (defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT))
#define MMIIM_SP_SYMBOL_SUPPORT_FULL (1)
#else
#define MMIIM_SP_SYMBOL_SUPPORT_FULL (0)
#endif

#define MMIIM_SP_SYMBOL_STRING_LEN_LIMIT (8)


/*
关于这个系列的定义，从0xe0000开始，是因为这段unicode是空白的，可以干净的无影响
这些特殊的character是输入法应用可以传给输入法控件处理的
因为在触笔输入的时候，一些特殊的按键或者按钮，如果统一处理的话，会简化程序
*/
/*
每一类的MAX都是下一类的起点，也就是说MAX在本类是无效的，是下一类的"零"
*/
#define MMIIM_SP_SYMB_DEF_STR_START (0xE000)
#define MMIIM_SP_SYMB_SIGN_START (MMIIM_SP_SYMB_DEF_STR_START)

enum
{
    MMIIM_SP_SYMB_SIGN_HTTP = MMIIM_SP_SYMB_SIGN_START,
    MMIIM_SP_SYMB_SIGN_WWW,
    MMIIM_SP_SYMB_SIGN_COM,
    MMIIM_SP_SYMB_SIGN_NET,
    MMIIM_SP_SYMB_SIGN_ORG,

    MMIIM_SP_SYMB_SIGN_MAX,
};

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
#define MMIIM_SP_SYMB_FACE_START (MMIIM_SP_SYMB_SIGN_MAX)

enum
{
    MMIIM_SP_SYMB_FACE_EM00 = MMIIM_SP_SYMB_FACE_START,
    MMIIM_SP_SYMB_FACE_EM01,
    MMIIM_SP_SYMB_FACE_EM02,
    MMIIM_SP_SYMB_FACE_EM03,
    MMIIM_SP_SYMB_FACE_EM04,
    MMIIM_SP_SYMB_FACE_EM05,
    MMIIM_SP_SYMB_FACE_EM06,
    MMIIM_SP_SYMB_FACE_EM07,
    MMIIM_SP_SYMB_FACE_EM08,
    MMIIM_SP_SYMB_FACE_EM09,
    MMIIM_SP_SYMB_FACE_EM10,
    MMIIM_SP_SYMB_FACE_EM11,
    MMIIM_SP_SYMB_FACE_EM12,
    MMIIM_SP_SYMB_FACE_EM13,
    MMIIM_SP_SYMB_FACE_EM14,
    MMIIM_SP_SYMB_FACE_EM15,
    MMIIM_SP_SYMB_FACE_EM16,
    MMIIM_SP_SYMB_FACE_EM17,
    MMIIM_SP_SYMB_FACE_EM18,
    MMIIM_SP_SYMB_FACE_EM19,
    MMIIM_SP_SYMB_FACE_EM20,
    MMIIM_SP_SYMB_FACE_EM21,
    MMIIM_SP_SYMB_FACE_EM22,
    MMIIM_SP_SYMB_FACE_EM23,
    MMIIM_SP_SYMB_FACE_EM24,
    MMIIM_SP_SYMB_FACE_EM25,
    MMIIM_SP_SYMB_FACE_EM26,
    MMIIM_SP_SYMB_FACE_EM27,
    MMIIM_SP_SYMB_FACE_EM28,
    MMIIM_SP_SYMB_FACE_EM29,
    MMIIM_SP_SYMB_FACE_EM30,
    MMIIM_SP_SYMB_FACE_EM31,
    MMIIM_SP_SYMB_FACE_EM32,
    MMIIM_SP_SYMB_FACE_EM33,
    MMIIM_SP_SYMB_FACE_EM34,
    MMIIM_SP_SYMB_FACE_EM35,
    MMIIM_SP_SYMB_FACE_EM36,
    MMIIM_SP_SYMB_FACE_EM37,
    MMIIM_SP_SYMB_FACE_EM38,
    MMIIM_SP_SYMB_FACE_EM39,
    MMIIM_SP_SYMB_FACE_EM40,
    MMIIM_SP_SYMB_FACE_EM41,
    MMIIM_SP_SYMB_FACE_EM42,
    MMIIM_SP_SYMB_FACE_EM43,
    MMIIM_SP_SYMB_FACE_EM44,
    MMIIM_SP_SYMB_FACE_EM45,
    MMIIM_SP_SYMB_FACE_EM46,
    MMIIM_SP_SYMB_FACE_EM47,
    MMIIM_SP_SYMB_FACE_EM48,
    MMIIM_SP_SYMB_FACE_EM49,
    MMIIM_SP_SYMB_FACE_EM50,
    MMIIM_SP_SYMB_FACE_EM51,
    MMIIM_SP_SYMB_FACE_EM52,
    MMIIM_SP_SYMB_FACE_EM53,
    MMIIM_SP_SYMB_FACE_EM54,
    MMIIM_SP_SYMB_FACE_EM55,
    MMIIM_SP_SYMB_FACE_EM56,
    MMIIM_SP_SYMB_FACE_EM57,
    MMIIM_SP_SYMB_FACE_EM58,
    MMIIM_SP_SYMB_FACE_EM59,
    MMIIM_SP_SYMB_FACE_EM60,
    MMIIM_SP_SYMB_FACE_EM61,
    MMIIM_SP_SYMB_FACE_EM62,
    MMIIM_SP_SYMB_FACE_EM63,

    MMIIM_SP_SYMB_FACE_MAX,
};

#define MMIIM_SP_SYMB_DEF_STR_END (MMIIM_SP_SYMB_FACE_MAX)
#else
#define MMIIM_SP_SYMB_DEF_STR_END (MMIIM_SP_SYMB_SIGN_MAX)
#endif

#define MMIIM_SP_SYMB_IMG_START (MMIIM_SP_SYMB_DEF_STR_END)

enum
{
    MMIIM_SP_SYMB_IMG_SPACE_HALF = MMIIM_SP_SYMB_IMG_START,
    MMIIM_SP_SYMB_IMG_ENTER,

#if MMIIM_SP_SYMBOL_SUPPORT_FULL
    MMIIM_SP_SYMB_IMG_SPACE,
#endif

    MMIIM_SP_SYMB_IMG_MAX,
};

#define MMIIM_SP_SYMB_DEF_IMG_END (MMIIM_SP_SYMB_IMG_MAX)



//符号类型
//通常，某一类符号的给出顺序按照普遍的使用频率
typedef enum
{
    MMIIM_SP_SYMBOL_TYPE_EN,
    MMIIM_SP_SYMBOL_TYPE_SIGN,

#if MMIIM_SP_SYMBOL_SUPPORT_FULL
    MMIIM_SP_SYMBOL_TYPE_CH,
    MMIIM_SP_SYMBOL_TYPE_SPECIAL,
    MMIIM_SP_SYMBOL_TYPE_NUM,
    MMIIM_SP_SYMBOL_TYPE_MATH,
    MMIIM_SP_SYMBOL_TYPE_UNIT,
    MMIIM_SP_SYMBOL_TYPE_PYIN,
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
    MMIIM_SP_SYMBOL_TYPE_FACE,
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_HINDI
    MMIIM_SP_SYMBOL_TYPE_HINDI,
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_EUR
    MMIIM_SP_SYMBOL_TYPE_EUR,
#endif

    MMIIM_SP_SYMBOL_TYPE_MAX
} MMIIM_SP_SYMBOL_TYPE_T;


//符号分类的配置
typedef struct
{
    wchar const *symb_list;
    uint32 symb_cnt;
}MMIIM_SP_SYMBOL_CFG_T;



/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbols
Description: 按照类型获取符号列表，返回指定数量之内的符号
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    type - symbol type
    *cnt_ptr - [in\out]want get symbol count and the get count
    *symb_ptr - [out]get symbols
    return - get count
==============================================================================*/
int32 MMIIM_SP_SYMB_GetSymbols(
    MMIIM_SP_SYMBOL_TYPE_T type,
    uint32 *cnt_ptr,
    wchar *symb_ptr
);

/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
wchar const * MMIIM_SP_SYMB_GetSymbStrPtr(wchar symb);


#endif /*_MMIIM_SP_SYMBOL_DATA_H_*/




