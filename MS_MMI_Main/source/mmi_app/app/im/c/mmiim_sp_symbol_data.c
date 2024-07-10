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

#include "mmiim_sp_symbol_data.h"
#include "mmk_type.h"

#include "os_api.h"


static __inline uint32 spim_wcslen(wchar *str)
{
    uint32 len = 0;

    if (NULL != str)
    {
        while (str[len]) len++;
    }

    return len;
}



static wchar const *symb_sign_str[] =
{
    L"http://", //MMIIM_SP_SYMB_SIGN_HTTP
    L"www.", //MMIIM_SP_SYMB_SIGN_WWW
    L".com", //MMIIM_SP_SYMB_SIGN_COM
    L".net", //MMIIM_SP_SYMB_SIGN_NET
    L".org", //MMIIM_SP_SYMB_SIGN_ORG
};

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
static wchar const *symb_face_str[] =
{
    L":)",      //MMIIM_SP_SYMB_FACE_EM00
    L":(",      //MMIIM_SP_SYMB_FACE_EM01
    L":D",      //MMIIM_SP_SYMB_FACE_EM02
    L":P",      //MMIIM_SP_SYMB_FACE_EM03
    L":O",      //MMIIM_SP_SYMB_FACE_EM04
    L":X",      //MMIIM_SP_SYMB_FACE_EM05
    L":|",      //MMIIM_SP_SYMB_FACE_EM06
    L":-)",     //MMIIM_SP_SYMB_FACE_EM07
    L":-(",     //MMIIM_SP_SYMB_FACE_EM08
    L":-D",     //MMIIM_SP_SYMB_FACE_EM09
    L":-P",     //MMIIM_SP_SYMB_FACE_EM10
    L":-O",     //MMIIM_SP_SYMB_FACE_EM11
    L":-X",     //MMIIM_SP_SYMB_FACE_EM12
    L":-|",     //MMIIM_SP_SYMB_FACE_EM13
    L":-#",     //MMIIM_SP_SYMB_FACE_EM14
    L"^_^",     //MMIIM_SP_SYMB_FACE_EM15
    L"-_-",     //MMIIM_SP_SYMB_FACE_EM16
    L"=_=",     //MMIIM_SP_SYMB_FACE_EM17
    L"+_+",     //MMIIM_SP_SYMB_FACE_EM18
    L"*_*",     //MMIIM_SP_SYMB_FACE_EM19
    L"$_$",     //MMIIM_SP_SYMB_FACE_EM20
    L"@_@",     //MMIIM_SP_SYMB_FACE_EM21
    L"#_#",     //MMIIM_SP_SYMB_FACE_EM22
    L"&_&",     //MMIIM_SP_SYMB_FACE_EM23
    L"!_!",     //MMIIM_SP_SYMB_FACE_EM24
    L"Q_Q",     //MMIIM_SP_SYMB_FACE_EM25
    L"T_T",     //MMIIM_SP_SYMB_FACE_EM26
    L"O_O",     //MMIIM_SP_SYMB_FACE_EM27
    L"O_o",     //MMIIM_SP_SYMB_FACE_EM28
    L"p_q",     //MMIIM_SP_SYMB_FACE_EM29
    L">_<",     //MMIIM_SP_SYMB_FACE_EM30
    L"^3^",     //MMIIM_SP_SYMB_FACE_EM31
    L"^o^",     //MMIIM_SP_SYMB_FACE_EM32
    L"orz",     //MMIIM_SP_SYMB_FACE_EM33
    L"Orz",     //MMIIM_SP_SYMB_FACE_EM34
    L"or2",     //MMIIM_SP_SYMB_FACE_EM35
    L"sto",     //MMIIM_SP_SYMB_FACE_EM36
    L"stO",     //MMIIM_SP_SYMB_FACE_EM37
    L"-_-+",    //MMIIM_SP_SYMB_FACE_EM38
    L"-_-||",   //MMIIM_SP_SYMB_FACE_EM39
    L"-_-;;",   //MMIIM_SP_SYMB_FACE_EM40
    L">_<||",   //MMIIM_SP_SYMB_FACE_EM41
    L"(-.-)",   //MMIIM_SP_SYMB_FACE_EM42
    L"{>~<}",   //MMIIM_SP_SYMB_FACE_EM43
    L"*^_^*",   //MMIIM_SP_SYMB_FACE_EM44
    L"#^_^#",   //MMIIM_SP_SYMB_FACE_EM45
    L"Y^o^Y",   //MMIIM_SP_SYMB_FACE_EM46
    L"=^.^=",   //MMIIM_SP_SYMB_FACE_EM47
    L"^(oo)^",  //MMIIM_SP_SYMB_FACE_EM48
    L"(T__T)",  //MMIIM_SP_SYMB_FACE_EM49
    L"(^_^)?",  //MMIIM_SP_SYMB_FACE_EM50
    L"(?_?)?",  //MMIIM_SP_SYMB_FACE_EM51
    L"(#^_^)",  //MMIIM_SP_SYMB_FACE_EM52
    L"(#_<-)",  //MMIIM_SP_SYMB_FACE_EM53
    L"q(^_^)p", //MMIIM_SP_SYMB_FACE_EM54
    L"\\{O.O)/", //MMIIM_SP_SYMB_FACE_EM55
    L"-_-)y-~", //MMIIM_SP_SYMB_FACE_EM56
    L"::>_<::", //MMIIM_SP_SYMB_FACE_EM57
    L"..>_<..", //MMIIM_SP_SYMB_FACE_EM58
    L"-.-)zZZ", //MMIIM_SP_SYMB_FACE_EM59
    L"(_r_)=3", //MMIIM_SP_SYMB_FACE_EM60
    L"..<{=..", //MMIIM_SP_SYMB_FACE_EM61
    L"{^o~o^}", //MMIIM_SP_SYMB_FACE_EM62
    L"|(-_-)|", //MMIIM_SP_SYMB_FACE_EM63
};
#endif

static wchar const symb_img[] =
{
    ' ',                    //MMIIM_SP_SYMB_IMG_SPACE_HALF
    '\n',                   //MMIIM_SP_SYMB_IMG_ENTER
    
#if MMIIM_SP_SYMBOL_SUPPORT_FULL
    0x3000,                 //MMIIM_SP_SYMB_IMG_SPACE
#endif
};

//常用标点符号
//,.?!:;"'~|\/@$%#&^*_-+=<>()[]{}`
static wchar const symb_en[] =
{
    0x002c, 0x002e, 0x003f, 0x0021, 0x003a, 0x003b, 0x0022, 0x0027,
    0x007e, 0x007c, 0x005c, MMIIM_SP_SYMB_IMG_SPACE_HALF, 0x002f, 0x0040, 0x0024, MMIIM_SP_SYMB_IMG_ENTER, 
    0x0025, 0x0023, 0x0026, 0x005e, 0x002a, 0x005f, 0x002d, 0x002b, 
    0x003d, 0x003c, 0x003e, 0x0028, 0x0029, 0x005b, 0x005d, 0x007b, 
    0x007d, 0x00bf,
};

//sign
static wchar const symb_sign[] =
{
    MMIIM_SP_SYMB_SIGN_HTTP,
    MMIIM_SP_SYMB_SIGN_WWW,
    MMIIM_SP_SYMB_SIGN_COM,
    MMIIM_SP_SYMB_SIGN_NET,
    MMIIM_SP_SYMB_SIGN_ORG,
};

#if MMIIM_SP_SYMBOL_SUPPORT_FULL
//中文标点符号
//，。、；：？！% * # @ ”‘ ’｜…·ˉ（）《》〈〉～【】［］｛｝
static wchar const symb_ch[] =
{
    0xff0c, 0x3002, 0x3001, 0xff1b, 0xff1a, 0xff1f, 0xff01, 0x0025, 
    0x002a, 0x0023, 0x0040, 0x201c, 0x201d, 0x2018, MMIIM_SP_SYMB_IMG_SPACE, 0x2019, 
    0xff5c, 0x2026, MMIIM_SP_SYMB_IMG_ENTER,0x00b7, 0x02c9, 0xff08, 0xff09, 0x300a, 
    0x300b, 0x3008, 0x3009, 0xff5e, 0x3010, 0x3011, 0xff3b, 0xff3d, 
    0xff5b, 0xff5d,
};

//特殊符号
//№☆★○●◎◇◆□■△▲→←↑↓〓
static wchar const symb_special[] =
{
    0x2116, 0x2606, 0x2605, 0x25cb, 0x25cf, 0x25ce, 0x25c7, 0x25c6,
    0x25a1, 0x25a0, 0x25b3, 0x25b2, 0x2192, 0x2190, 0x2191, 0x2193, 
    0x3013,
};

//数字序号
//ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫ⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑㈠㈡㈢㈣㈤㈥㈦㈧㈨㈩⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽
static wchar const symb_num[] =
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
static wchar const symb_math[] =
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
static wchar const symb_unit[] =
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
static wchar const symb_pyin[] =
{
    0x0101, 0x00e1, 0x01ce, 0x00e0, 0x014d, 0x00f3, 0x01d2, 0x00f2,
    0x00ea, 0x0113, 0x00e9, 0x011b, 0x00e8, 0x012b, 0x00ed, 0x01d0,
    0x00ec, 0x016b, 0x00fa, 0x01d4, 0x00f9, 
            0x00fc, 0x0062, 0x0070, 0x006d, 0x0066, 0x0064, 0x0074,
    0x006e, 0x006c, 0x0067, 0x006b, 0x0068, 0x006a, 0x0071, 0x0078,
    0x0072, 0x007a, 0x0073, 0x0063, 0x0079, 0x0077,
};
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_HINDI
static wchar const symb_hindi[] =
{
    0x0950, 0x0964, 0x0965, 0x0970, 0x093d, 0x20a8, 0x20b9,
};
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_EUR
static wchar const symb_eur[] =
{
    0xa1, 0xa2, 0xa3,
};
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
//face
static wchar const symb_face[] =
{
    MMIIM_SP_SYMB_FACE_EM00,
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
};
#endif



//配置
static MMIIM_SP_SYMBOL_CFG_T const symb_cfg[] =
{
    {symb_en, ARR_SIZE(symb_en)},
    {symb_sign, ARR_SIZE(symb_sign)},

#if MMIIM_SP_SYMBOL_SUPPORT_FULL
    {symb_ch, ARR_SIZE(symb_ch)},
    {symb_special, ARR_SIZE(symb_special)},
    {symb_num, ARR_SIZE(symb_num)},
    {symb_math, ARR_SIZE(symb_math)},
    {symb_unit, ARR_SIZE(symb_unit)},
    {symb_pyin, ARR_SIZE(symb_pyin)},
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
    {symb_face, ARR_SIZE(symb_face)},
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_HINDI
    {symb_hindi, ARR_SIZE(symb_hindi)},
#endif

#if MMIIM_SP_SYMBOL_SUPPORT_EUR
    {symb_eur, ARR_SIZE(symb_eur)},
#endif
};


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
)
{
    // int32 is_error_param = 0;
    wchar const *symb_list_ptr = NULL;
    uint32 symb_cnt = 0;
    // int32 symb_left = 0;

    if (NULL == cnt_ptr) return 0;

    if (NULL == symb_ptr) return 0;

    if (type >= ARR_SIZE(symb_cfg)) return 0;

    if (*cnt_ptr == 0) return 0;

    symb_list_ptr = symb_cfg[type].symb_list;

    if (PNULL == symb_list_ptr) return 0;

    symb_cnt = symb_cfg[type].symb_cnt;

    if (symb_cnt > *cnt_ptr) symb_cnt = *cnt_ptr;

    SCI_MEMCPY(symb_ptr, symb_list_ptr, symb_cnt * sizeof(wchar));
    *cnt_ptr = symb_cnt;

    return symb_cnt;
}


/*==============================================================================
Function Name: MMIIM_SP_SYMB_GetSymbStrPtr
Description: 获取字符串的指针
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 宽字符，0结尾
==============================================================================*/
wchar const * MMIIM_SP_SYMB_GetSymbStrPtr(wchar symb)
{
    wchar const *symb_str_ptr = NULL;

    if (MMIIM_SP_SYMB_SIGN_START <= symb && symb < MMIIM_SP_SYMB_SIGN_MAX)
    {
        symb_str_ptr = symb_sign_str[symb - MMIIM_SP_SYMB_SIGN_START];
    }
#if MMIIM_SP_SYMBOL_SUPPORT_FACE
    else if (MMIIM_SP_SYMB_FACE_START <= symb && symb < MMIIM_SP_SYMB_FACE_MAX)
    {
        symb_str_ptr = symb_face_str[symb - MMIIM_SP_SYMB_FACE_START];
    }
#endif
    else if (MMIIM_SP_SYMB_IMG_START <= symb && symb < MMIIM_SP_SYMB_IMG_MAX)
    {
        symb_str_ptr = &symb_img[symb - MMIIM_SP_SYMB_IMG_START];
    }

    return symb_str_ptr;
}





