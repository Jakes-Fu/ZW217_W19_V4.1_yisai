/*==============================================================================
File Name: spim_symbol.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: 符号
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/


#include "spim_symbol.h"
#include "spim_symbol_internal.h"

static __inline SPIM_UINT32 spim_wcslen(SPIM_UNICODE_T *str)
{
    SPIM_UINT32 len = 0;

    if (NULL != str)
    {
        while (str[len]) len++;
    }

    return len;
}



static SPIM_UNICODE_T const *symb_sign_str[] =
{
    L"http://", //SPIM_SYMB_SIGN_HTTP
    L"www.", //SPIM_SYMB_SIGN_WWW
    L".com", //SPIM_SYMB_SIGN_COM
    L".net", //SPIM_SYMB_SIGN_NET
    L".org", //SPIM_SYMB_SIGN_ORG
};

static SPIM_UNICODE_T const *symb_face_str[] =
{
    L":)",      //SPIM_SYMB_FACE_EM00
    L":(",      //SPIM_SYMB_FACE_EM01
    L":D",      //SPIM_SYMB_FACE_EM02
    L":P",      //SPIM_SYMB_FACE_EM03
    L":O",      //SPIM_SYMB_FACE_EM04
    L":X",      //SPIM_SYMB_FACE_EM05
    L":|",      //SPIM_SYMB_FACE_EM06
    L":-)",     //SPIM_SYMB_FACE_EM07
    L":-(",     //SPIM_SYMB_FACE_EM08
    L":-D",     //SPIM_SYMB_FACE_EM09
    L":-P",     //SPIM_SYMB_FACE_EM10
    L":-O",     //SPIM_SYMB_FACE_EM11
    L":-X",     //SPIM_SYMB_FACE_EM12
    L":-|",     //SPIM_SYMB_FACE_EM13
    L":-#",     //SPIM_SYMB_FACE_EM14
    L"^_^",     //SPIM_SYMB_FACE_EM15
    L"-_-",     //SPIM_SYMB_FACE_EM16
    L"=_=",     //SPIM_SYMB_FACE_EM17
    L"+_+",     //SPIM_SYMB_FACE_EM18
    L"*_*",     //SPIM_SYMB_FACE_EM19
    L"$_$",     //SPIM_SYMB_FACE_EM20
    L"@_@",     //SPIM_SYMB_FACE_EM21
    L"#_#",     //SPIM_SYMB_FACE_EM22
    L"&_&",     //SPIM_SYMB_FACE_EM23
    L"!_!",     //SPIM_SYMB_FACE_EM24
    L"Q_Q",     //SPIM_SYMB_FACE_EM25
    L"T_T",     //SPIM_SYMB_FACE_EM26
    L"O_O",     //SPIM_SYMB_FACE_EM27
    L"O_o",     //SPIM_SYMB_FACE_EM28
    L"p_q",     //SPIM_SYMB_FACE_EM29
    L">_<",     //SPIM_SYMB_FACE_EM30
    L"^3^",     //SPIM_SYMB_FACE_EM31
    L"^o^",     //SPIM_SYMB_FACE_EM32
    L"orz",     //SPIM_SYMB_FACE_EM33
    L"Orz",     //SPIM_SYMB_FACE_EM34
    L"or2",     //SPIM_SYMB_FACE_EM35
    L"sto",     //SPIM_SYMB_FACE_EM36
    L"stO",     //SPIM_SYMB_FACE_EM37
    L"-_-+",    //SPIM_SYMB_FACE_EM38
    L"-_-||",   //SPIM_SYMB_FACE_EM39
    L"-_-;;",   //SPIM_SYMB_FACE_EM40
    L">_<||",   //SPIM_SYMB_FACE_EM41
    L"(-.-)",   //SPIM_SYMB_FACE_EM42
    L"{>~<}",   //SPIM_SYMB_FACE_EM43
    L"*^_^*",   //SPIM_SYMB_FACE_EM44
    L"#^_^#",   //SPIM_SYMB_FACE_EM45
    L"Y^o^Y",   //SPIM_SYMB_FACE_EM46
    L"=^.^=",   //SPIM_SYMB_FACE_EM47
    L"^(oo)^",  //SPIM_SYMB_FACE_EM48
    L"(T__T)",  //SPIM_SYMB_FACE_EM49
    L"(^_^)?",  //SPIM_SYMB_FACE_EM50
    L"(?_?)?",  //SPIM_SYMB_FACE_EM51
    L"(#^_^)",  //SPIM_SYMB_FACE_EM52
    L"(#_<-)",  //SPIM_SYMB_FACE_EM53
    L"q(^_^)p", //SPIM_SYMB_FACE_EM54
    L"\\{O.O)/", //SPIM_SYMB_FACE_EM55
    L"-_-)y-~", //SPIM_SYMB_FACE_EM56
    L"::>_<::", //SPIM_SYMB_FACE_EM57
    L"..>_<..", //SPIM_SYMB_FACE_EM58
    L"-.-)zZZ", //SPIM_SYMB_FACE_EM59
    L"(_r_)=3", //SPIM_SYMB_FACE_EM60
    L"..<{=..", //SPIM_SYMB_FACE_EM61
    L"{^o~o^}", //SPIM_SYMB_FACE_EM62
    L"|(-_-)|", //SPIM_SYMB_FACE_EM63
};


//常用标点符号
//,.?!:;"'~|\/@$%#&^*_-+=<>()[]{}`
static SPIM_UNICODE_T const symb_en[] =
{
    0x002c, 0x002e, 0x003f, 0x0021, 0x003a, 0x003b, 0x0022, 0x0027,
    0x007e, 0x007c, 0x005c, 0x002f, 0x0040, 0x0024, 0x0025, 0x0023,
    0x0026, 0x005e, 0x002a, 0x005f, 0x002d, 0x002b, 0x003d, 0x003c,
    0x003e, 0x0028, 0x0029, 0x005b, 0x005d, 0x007b, 0x007d, 0x0060,
};

//中文标点符号
// 。，、；：？！“”‘’（）…—ˉ《》〈〉·．～『』「」〖〗【】〔〕［］｛｝々‖∶＂＇｀｜〃ˇ¨
static SPIM_UNICODE_T const symb_ch[] =
{
    0x3002, 0xff0c, 0x3001, 0xff1b, 0xff1a, 0xff1f, 0xff01, 0x201c,
    0x201d, 0x2018, 0x2019, 0xff08, 0xff09, 0x2026, 0x2014, 0x02c9,
    0x300a, 0x300b, 0x3008, 0x3009, 0x00b7, 0xff0e, 0xff5e, 0x300e,
    0x300f, 0x300c, 0x300d, 0x3016, 0x3017, 0x3010, 0x3011, 0x3014,
    0x3015, 0xff3b, 0xff3d, 0xff5b, 0xff5d, 0x3005, 0x2016, 0x2236,
    0xff02, 0xff07, 0xff40, 0xff5c, 0x3003, 0x02c7, 0x00a8,
};

//特殊符号
//§№☆★○●◎◇◆□■△▲※→←↑↓〓＃＆＠＼＾＿￣
static SPIM_UNICODE_T const symb_special[] =
{
    0x00a7, 0x2116, 0x2606, 0x2605, 0x25cb, 0x25cf, 0x25ce, 0x25c7,
    0x25c6, 0x25a1, 0x25a0, 0x25b3, 0x25b2, 0x203b, 0x2192, 0x2190,
    0x2191, 0x2193, 0x3013, 0xff03, 0xff06, 0xff20, 0xff3c, 0xff3e,
    0xff3f, 0xffe3,
};

//数字序号
//ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫ⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑㈠㈡㈢㈣㈤㈥㈦㈧㈨㈩⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽
static SPIM_UNICODE_T const symb_num[] =
{
    0x2160, 0x2161, 0x2162, 0x2163, 0x2164, 0x2165, 0x2166, 0x2167,
    0x2168, 0x2169, 0x216a, 0x216b, 0x2488, 0x2489, 0x248a, 0x248b,
    0x248c, 0x248d, 0x248e, 0x248f, 0x2490, 0x2491, 0x3220, 0x3221,
    0x3222, 0x3223, 0x3224, 0x3225, 0x3226, 0x3227, 0x3228, 0x3229,
    0x2474, 0x2475, 0x2476, 0x2477, 0x2478, 0x2479, 0x247a, 0x247b,
    0x247c, 0x247d,
};

//数学符号
//≈≡≠＝≤≥＜＞≮≯∷±＋－×÷／∫∮∝∞∧∨∑∏∪∩∈∵∴⊥∥∠⌒⊙≌∽√
static SPIM_UNICODE_T const symb_math[] =
{
    0x2248, 0x2261, 0x2260, 0xff1d, 0x2264, 0x2265, 0xff1c, 0xff1e,
    0x226e, 0x226f, 0x2237, 0x00b1, 0xff0b, 0xff0d, 0x00d7, 0x00f7,
    0xff0f, 0x222b, 0x222e, 0x221d, 0x221e, 0x2227, 0x2228, 0x2211,
    0x220f, 0x222a, 0x2229, 0x2208, 0x2235, 0x2234, 0x22a5, 0x2225,
    0x2220, 0x2312, 0x2299, 0x224c, 0x223d, 0x221a,
};

//单位符号
//°′″＄￡￥‰％℃¤￠
//〇一二三四五六七八九十百千万亿兆吉太拍艾分厘毫微零壹贰叁肆伍陆柒捌玖拾佰仟
static SPIM_UNICODE_T const symb_unit[] =
{
    0x00b0, 0x2032, 0x2033, 0xff04, 0xffe1, 0xffe5, 0x2030, 0xff05,
    0x2103, 0x00a4, 0xffe0,
#if 0
    // 〇 会导致显示错误，而死机。
    0x3007, 0x4e00, 0x4e8c, 0x4e09, 0x56db,
    0x4e94, 0x516d, 0x4e03, 0x516b, 0x4e5d, 0x5341, 0x767e, 0x5343,
    0x4e07, 0x4ebf, 0x5146, 0x5409, 0x592a, 0x62cd, 0x827e, 0x5206,
    0x5398, 0x6beb, 0x5fae, 0x96f6, 0x58f9, 0x8d30, 0x53c1, 0x8086,
    0x4f0d, 0x9646, 0x67d2, 0x634c, 0x7396, 0x62fe, 0x4f70, 0x4edf,
#endif
};

//拼音符号
//āáǎàōóǒòêēéěèīíǐìūúǔùǖǘǚǜüa-z
static SPIM_UNICODE_T const symb_pyin[] =
{
    0x0101, 0x00e1, 0x01ce, 0x00e0, 0x014d, 0x00f3, 0x01d2, 0x00f2,
    0x00ea, 0x0113, 0x00e9, 0x011b, 0x00e8, 0x012b, 0x00ed, 0x01d0,
    0x00ec, 0x016b, 0x00fa, 0x01d4, 0x00f9, 0x01d6, 0x01d8, 0x01da,
    0x01dc, 0x00fc, 0x0062, 0x0070, 0x006d, 0x0066, 0x0064, 0x0074,
    0x006e, 0x006c, 0x0067, 0x006b, 0x0068, 0x006a, 0x0071, 0x0078,
    0x0072, 0x007a, 0x0073, 0x0063, 0x0079, 0x0077,
};

//sign
static SPIM_UNICODE_T const symb_sign[] =
{
    SPIM_SYMB_SIGN_HTTP,
    SPIM_SYMB_SIGN_WWW,
    SPIM_SYMB_SIGN_COM,
    SPIM_SYMB_SIGN_NET,
    SPIM_SYMB_SIGN_ORG,
};

//face
static SPIM_UNICODE_T const symb_face[] =
{
    SPIM_SYMB_FACE_EM00,
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
};




//配置
static SPIM_SYMBOL_CFG_T const symb_cfg[] =
{
    {symb_en, SPIM_ARR_SIZE(symb_en)},
    {symb_ch, SPIM_ARR_SIZE(symb_ch)},
    {symb_special, SPIM_ARR_SIZE(symb_special)},
    {symb_num, SPIM_ARR_SIZE(symb_num)},
    {symb_math, SPIM_ARR_SIZE(symb_math)},
    {symb_unit, SPIM_ARR_SIZE(symb_unit)},
    {symb_pyin, SPIM_ARR_SIZE(symb_pyin)},
    {symb_sign, SPIM_ARR_SIZE(symb_sign)},
    {symb_face, SPIM_ARR_SIZE(symb_face)},
};



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
)
{
    SPIM_INT32 is_error_param = 0;
    SPIM_UNICODE_T const *symb_list_ptr = NULL;
    SPIM_UINT32 symb_cnt = 0;
    SPIM_INT32 symb_left = 0;

    if (NULL == cnt_ptr) return 0;

    if (NULL == symb_ptr) return 0;

    if (type >= SPIM_ARR_SIZE(symb_cfg)) return 0;

    if (0 == *cnt_ptr) return 0;

    symb_list_ptr = symb_cfg[type].symb_list;

    if (PNULL == symb_list_ptr) return 0;

    symb_cnt = symb_cfg[type].symb_cnt;

    if (symb_cnt > *cnt_ptr) symb_cnt = *cnt_ptr;

    SPIM_MEMCPY(symb_ptr, symb_list_ptr, symb_cnt * sizeof(SPIM_UNICODE_T));
    *cnt_ptr = symb_cnt;

    return symb_cnt;
}




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
)
{
    SPIM_UNICODE_T const *symb_str_ptr = NULL;
    BOOLEAN is_string = TRUE;

    if (NULL == str_ptr || NULL == str_len_ptr)
    {
        return 0;
    }

    if (SPIM_SYMB_SIGN_START <= symb && symb < SPIM_SYMB_SIGN_START + SPIM_ARR_SIZE(symb_sign_str))
    {
        symb_str_ptr = symb_sign_str[symb - SPIM_SYMB_SIGN_START];
    }
    else if (SPIM_SYMB_FACE_START <= symb && symb < SPIM_SYMB_FACE_START + SPIM_ARR_SIZE(symb_sign_str))
    {
        symb_str_ptr = symb_face_str[symb - SPIM_SYMB_FACE_START];
    }
    else
    {
        str_ptr[0] = symb;
        *str_len_ptr = 1;
        is_string = FALSE;
    }

    if (is_string && PNULL != symb_str_ptr)
    {
        *str_len_ptr = spim_wcslen(symb_str_ptr);
        SPIM_MEMCPY(str_ptr, symb_str_ptr, (*str_len_ptr) * sizeof(SPIM_UNICODE_T));
    }

    return 0;
}


/*==============================================================================
Function Name: SPIM_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
SPIM_UNICODE_T const * SPIM_SYMB_GetSymbStrPtr(SPIM_UNICODE_T symb)
{
    SPIM_UNICODE_T const *symb_str_ptr = NULL;

    if (SPIM_SYMB_SIGN_START <= symb && symb < SPIM_SYMB_SIGN_MAX)
    {
        symb_str_ptr = symb_sign_str[symb - SPIM_SYMB_SIGN_START];
    }
    else if (SPIM_SYMB_FACE_START <= symb && symb < SPIM_SYMB_FACE_MAX)
    {
        symb_str_ptr = symb_face_str[symb - SPIM_SYMB_FACE_START];
    }

    return symb_str_ptr;
}





