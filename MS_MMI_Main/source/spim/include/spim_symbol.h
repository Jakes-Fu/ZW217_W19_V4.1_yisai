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
#ifndef _SPIM_SYMBOL_H_
#define _SPIM_SYMBOL_H_

#include "spim_type.h"


#define SPIM_SYMBOL_STRING_LEN_LIMIT (8)


/*
关于这个系列的定义，从0xe0000开始，是因为这段unicode是空白的，可以干净的无影响
这些特殊的character是输入法应用可以传给输入法控件处理的
因为在触笔输入的时候，一些特殊的按键或者按钮，如果统一处理的话，会简化程序
*/
/*
每一类的MAX都是下一类的起点，也就是说MAX在本类是无效的，是下一类的"零"
*/
#define SPIM_SYMB_DEF_STR_START (0xE000)
#define SPIM_SYMB_SIGN_START (SPIM_SYMB_DEF_STR_START)

enum
{
    SPIM_SYMB_SIGN_HTTP = SPIM_SYMB_SIGN_START,
    SPIM_SYMB_SIGN_WWW,
    SPIM_SYMB_SIGN_COM,
    SPIM_SYMB_SIGN_NET,
    SPIM_SYMB_SIGN_ORG,

    SPIM_SYMB_SIGN_MAX,
};

#define SPIM_SYMB_FACE_START (SPIM_SYMB_SIGN_MAX)

enum
{
    SPIM_SYMB_FACE_EM00 = SPIM_SYMB_FACE_START,
    SPIM_SYMB_FACE_EM01,
    SPIM_SYMB_FACE_EM02,
    SPIM_SYMB_FACE_EM03,
    SPIM_SYMB_FACE_EM04,
    SPIM_SYMB_FACE_EM05,
    SPIM_SYMB_FACE_EM06,
    SPIM_SYMB_FACE_EM07,
    SPIM_SYMB_FACE_EM08,
    SPIM_SYMB_FACE_EM09,
    SPIM_SYMB_FACE_EM10,
    SPIM_SYMB_FACE_EM11,
    SPIM_SYMB_FACE_EM12,
    SPIM_SYMB_FACE_EM13,
    SPIM_SYMB_FACE_EM14,
    SPIM_SYMB_FACE_EM15,
    SPIM_SYMB_FACE_EM16,
    SPIM_SYMB_FACE_EM17,
    SPIM_SYMB_FACE_EM18,
    SPIM_SYMB_FACE_EM19,
    SPIM_SYMB_FACE_EM20,
    SPIM_SYMB_FACE_EM21,
    SPIM_SYMB_FACE_EM22,
    SPIM_SYMB_FACE_EM23,
    SPIM_SYMB_FACE_EM24,
    SPIM_SYMB_FACE_EM25,
    SPIM_SYMB_FACE_EM26,
    SPIM_SYMB_FACE_EM27,
    SPIM_SYMB_FACE_EM28,
    SPIM_SYMB_FACE_EM29,
    SPIM_SYMB_FACE_EM30,
    SPIM_SYMB_FACE_EM31,
    SPIM_SYMB_FACE_EM32,
    SPIM_SYMB_FACE_EM33,
    SPIM_SYMB_FACE_EM34,
    SPIM_SYMB_FACE_EM35,
    SPIM_SYMB_FACE_EM36,
    SPIM_SYMB_FACE_EM37,
    SPIM_SYMB_FACE_EM38,
    SPIM_SYMB_FACE_EM39,
    SPIM_SYMB_FACE_EM40,
    SPIM_SYMB_FACE_EM41,
    SPIM_SYMB_FACE_EM42,
    SPIM_SYMB_FACE_EM43,
    SPIM_SYMB_FACE_EM44,
    SPIM_SYMB_FACE_EM45,
    SPIM_SYMB_FACE_EM46,
    SPIM_SYMB_FACE_EM47,
    SPIM_SYMB_FACE_EM48,
    SPIM_SYMB_FACE_EM49,
    SPIM_SYMB_FACE_EM50,
    SPIM_SYMB_FACE_EM51,
    SPIM_SYMB_FACE_EM52,
    SPIM_SYMB_FACE_EM53,
    SPIM_SYMB_FACE_EM54,
    SPIM_SYMB_FACE_EM55,
    SPIM_SYMB_FACE_EM56,
    SPIM_SYMB_FACE_EM57,
    SPIM_SYMB_FACE_EM58,
    SPIM_SYMB_FACE_EM59,
    SPIM_SYMB_FACE_EM60,
    SPIM_SYMB_FACE_EM61,
    SPIM_SYMB_FACE_EM62,
    SPIM_SYMB_FACE_EM63,

    SPIM_SYMB_FACE_MAX,
};

#define SPIM_SYMB_DEF_STR_END (SPIM_SYMB_FACE_MAX)


//符号类型
//通常，某一类符号的给出顺序按照普遍的使用频率
typedef enum
{
    SPIM_SYMBOL_TYPE_EN,
    SPIM_SYMBOL_TYPE_CH,
    SPIM_SYMBOL_TYPE_SPECIAL,
    SPIM_SYMBOL_TYPE_NUM,
    SPIM_SYMBOL_TYPE_MATH,
    SPIM_SYMBOL_TYPE_UNIT,
    SPIM_SYMBOL_TYPE_PYIN,
    SPIM_SYMBOL_TYPE_SIGN,
    SPIM_SYMBOL_TYPE_FACE,

    SPIM_SYMBOL_TYPE_MAX
} SPIM_SYMBOL_TYPE_T;


/*==============================================================================
Function Name: SPIM_SYMB_GetSymbols
Description: 按照类型获取符号列表，返回指定数量之内的符号
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
    type - symbol type
    *cnt_ptr - [in\out]want get symbol count and the get count
    *symb_ptr - [out]get symbols
    return - get count
==============================================================================*/
SPIM_INT32 SPIM_SYMB_GetSymbols(
    SPIM_SYMBOL_TYPE_T type,
    SPIM_UINT32 *cnt_ptr,
    SPIM_UNICODE_T *symb_ptr
);

/*==============================================================================
Function Name: SPIM_SYMB_GetSymbStr
Description: SPIM_SYMB_GetSymbStr
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
SPIM_INT32 SPIM_SYMB_GetSymbStr(
    SPIM_UNICODE_T symb,
    SPIM_UNICODE_T *str_ptr,
    SPIM_UINT32 *str_len_ptr
);

/*==============================================================================
Function Name: SPIM_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
SPIM_UNICODE_T const * SPIM_SYMB_GetSymbStrPtr(SPIM_UNICODE_T symb);


#endif //_SPIM_SYMBOL_H_




