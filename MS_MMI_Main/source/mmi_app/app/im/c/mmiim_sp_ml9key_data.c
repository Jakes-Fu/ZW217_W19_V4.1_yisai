
/*****************************************************************************
** File Name:      mmiim_sp_ml9key_data.c                                    *
** Author:         hua.fang                                                  *
** Date:           07/06/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2010        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmiim_sp_ml9key_data.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
//将宏定义成 inline 函数
LOCAL __inline BOOLEAN IsThaiConsonant(wchar wc)
{
    return (wc >= THAI_C_KO_KAI && wc <= THAI_C_HO_NOKHUK);
}

LOCAL __inline BOOLEAN IsThaiToneMark(wchar wc)
{
    return (wc == THAI_C_MAI_EK
            || wc == THAI_C_MAI_THO
            || wc == THAI_C_MAI_TRI
            || wc == THAI_C_MAI_CHATTAWA);
}

LOCAL __inline BOOLEAN IsThaiUpDownVowel(wchar wc)
{
    return (wc == THAI_C_MAI_HAN_AKAT
            || (wc >= THAI_C_SARA_I && wc <= THAI_C_PHINTHU)
            || wc == THAI_C_MAITAIKHU);
}

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// multitap language data type
typedef struct
{
    GUIIM_METHOD_T              method;             // method name
    MMIIM_SP_9KEY_TABLE_DEF_T   *key_tab_ptr;       // key table. when lead upper, its is upper table first, then set by lower_method below
} const MMIIM_SP_9KEY_TABLE_CFG_T;


/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/
//lint -e785

// 9键多国语言数据
// 数据组织形式: 每行的第一个存放可选字母的个数，后面紧跟对应的字母

//小写字母与数字
//这里的*#号等必须是按顺序排列的，即1~9, 0, * #，因为所有调用地方均已经假设按此顺序排列，减少程序计算
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_lower =
{
#if defined MMI_IM_PDA_SUPPORT
    /*1*/{1, '1'},
#else
    /*1*/{13, ',', '.', '?', '!', '\'', '"', '-', '@', ':', ';', '/', '_', '1'},
#endif    
    /*2*/{4, 'a', 'b', 'c', '2'},
    /*3*/{4, 'd', 'e', 'f', '3'},
    /*4*/{4, 'g', 'h', 'i', '4'},
    /*5*/{4, 'j', 'k', 'l', '5'},
    /*6*/{4, 'm', 'n', 'o', '6'},
    /*7*/{5, 'p', 'q', 'r', 's', '7'},
    /*8*/{4, 't', 'u', 'v', '8'},
    /*9*/{5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

//大写字母与数字
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_upper =
{
#if defined MMI_IM_PDA_SUPPORT
    /*1*/{1, '1'},
#else
    /*1*/{13, ',', '.', '?', '!', '\'', '"', '-', '@', ':', ';', '/', '_', '1'},
#endif    
    /*2*/{4, 'A', 'B', 'C', '2'},
    /*3*/{4, 'D', 'E', 'F', '3'},
    /*4*/{4, 'G', 'H', 'I', '4'},
    /*5*/{4, 'J', 'K', 'L', '5'},
    /*6*/{4, 'M', 'N', 'O', '6'},
    /*7*/{5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{4, 'T', 'U', 'V', '8'},
    /*9*/{5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_lower =
{
    /*1*/{5, 0x0027, 0x002D, 0x002E, 0x002F, '1'},
    /*2*/{4, 'a', 'b', 'c', '2'},
    /*3*/{4, 'd', 'e', 'f', '3'},
    /*4*/{4, 'g', 'h', 'i', '4'},
    /*5*/{4, 'j', 'k', 'l', '5'},
    /*6*/{4, 'm', 'n', 'o', '6'},
    /*7*/{5, 'p', 'q', 'r', 's', '7'},
    /*8*/{4, 't', 'u', 'v', '8'},
    /*9*/{5, 'w', 'x', 'y', 'z', '9'},
#if defined MMI_IM_PDA_SUPPORT    
    /*0*/{2, ' ', '0'},     //PDA版本存在enter键
#else
    /*0*/{3, ' ',0x0a ,'0'},
#endif
    /***/{0},
    /*#*/{0}
};

//大写字母与数字
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_upper =
{
    /*1*/{5, 0x0027, 0x002D, 0x002E, 0x002F, '1'},
    /*2*/{4, 'A', 'B', 'C', '2'},
    /*3*/{4, 'D', 'E', 'F', '3'},
    /*4*/{4, 'G', 'H', 'I', '4'},
    /*5*/{4, 'J', 'K', 'L', '5'},
    /*6*/{4, 'M', 'N', 'O', '6'},
    /*7*/{5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{4, 'T', 'U', 'V', '8'},
    /*9*/{5, 'W', 'X', 'Y', 'Z', '9'},
#if defined MMI_IM_PDA_SUPPORT    
    /*0*/{2, ' ', '0'},     //PDA版本存在enter键
#else
    /*0*/{3, ' ',0x0a ,'0'},
#endif
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_lower =
{
#if defined MMI_IM_PDA_SUPPORT
    /*1*/{1, '1'},
#else
    /*1*/{13, ',', '.', '?', '!', '\'', '"', '-', '@', ':', ';', '/', '_', '1'},
#endif    
    /*2*/{4, 'a', 'b', 'c', '2'},
    /*3*/{4, 'd', 'e', 'f', '3'},
    /*4*/{4, 'g', 'h', 'i', '4'},
    /*5*/{4, 'j', 'k', 'l', '5'},
    /*6*/{4, 'm', 'n', 'o', '6'},
    /*7*/{5, 'p', 'q', 'r', 's', '7'},
    /*8*/{4, 't', 'u', 'v', '8'},
    /*9*/{5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

//大写字母与数字
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_upper =
{
#if defined MMI_IM_PDA_SUPPORT
    /*1*/{1, '1'},
#else
    /*1*/{13, ',', '.', '?', '!', '\'', '"', '-', '@', ':', ';', '/', '_', '1'},
#endif    
    /*2*/{4, 'A', 'B', 'C', '2'},
    /*3*/{4, 'D', 'E', 'F', '3'},
    /*4*/{4, 'G', 'H', 'I', '4'},
    /*5*/{4, 'J', 'K', 'L', '5'},
    /*6*/{4, 'M', 'N', 'O', '6'},
    /*7*/{5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{4, 'T', 'U', 'V', '8'},
    /*9*/{5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif

//数字
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_123 =
{
    /*1*/{1, '1'},
    /*2*/{1, '2'},
    /*3*/{1, '3'},
    /*4*/{1, '4'},
    /*5*/{1, '5'},
    /*6*/{1, '6'},
    /*7*/{1, '7'},
    /*8*/{1, '8'},
    /*9*/{1, '9'},
    /*0*/{1, '0'},
    /***/{0},
    /*#*/{0}
};

//以下多国语言，mtk，default

//繁体中文丝印数据
#ifdef IM_TRAD_CHINESE_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_9key_table_trad_chinese_zhuyin =
{
    /*1*/{ 4, 0x3105, 0x3106, 0x3107, 0x3108},
    /*2*/{ 4, 0x3109, 0x310A, 0x310B, 0x310C},
    /*3*/{ 3, 0x310D, 0x310E, 0x310F},
    /*4*/{ 3, 0x3110, 0x3111, 0x3112},
    /*5*/{ 4, 0x3113, 0x3114, 0x3115, 0x3116},
    /*6*/{ 3, 0x3117, 0x3118, 0x3119},
    /*7*/{ 4, 0x311A, 0x311B, 0x311C, 0x311D},
    /*8*/{ 4, 0x311E, 0x311F, 0x3120, 0x3121},
    /*9*/{ 5, 0x3122, 0x3123, 0x3124, 0x3125, 0x3126},
    /*0*/{ 3, 0x3127, 0x3128, 0x3129},
    /***/{0},
    /*#*/{0}
};    
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_9key_table_trad_chinese_zhuyin =
{
    /*1*/{ 4, 0x3105, 0x3106, 0x3107, 0x3108},
    /*2*/{ 4, 0x3109, 0x310A, 0x310B, 0x310C},
    /*3*/{ 3, 0x310D, 0x310E, 0x310F},
    /*4*/{ 3, 0x3110, 0x3111, 0x3112},
    /*5*/{ 4, 0x3113, 0x3114, 0x3115, 0x3116},
    /*6*/{ 3, 0x3117, 0x3118, 0x3119},
    /*7*/{ 4, 0x311A, 0x311B, 0x311C, 0x311D},
    /*8*/{ 4, 0x311E, 0x311F, 0x3120, 0x3121},
    /*9*/{ 5, 0x3122, 0x3123, 0x3124, 0x3125, 0x3126},
    /*0*/{ 3, 0x3127, 0x3128, 0x3129},
    /***/{0},
    /*#*/{0}
};    
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_9key_table_trad_chinese_zhuyin =
{
    /*1*/{ 4, 0x3105, 0x3106, 0x3107, 0x3108},
    /*2*/{ 4, 0x3109, 0x310A, 0x310B, 0x310C},
    /*3*/{ 3, 0x310D, 0x310E, 0x310F},
    /*4*/{ 3, 0x3110, 0x3111, 0x3112},
    /*5*/{ 4, 0x3113, 0x3114, 0x3115, 0x3116},
    /*6*/{ 3, 0x3117, 0x3118, 0x3119},
    /*7*/{ 4, 0x311A, 0x311B, 0x311C, 0x311D},
    /*8*/{ 4, 0x311E, 0x311F, 0x3120, 0x3121},
    /*9*/{ 5, 0x3122, 0x3123, 0x3124, 0x3125, 0x3126},
    /*0*/{ 3, 0x3127, 0x3128, 0x3129},
    /***/{0},
    /*#*/{0}
};    
#endif
#endif

// 阿拉伯语
#ifdef IM_ARABIC_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_arabic =
{
    /*1*/{ 6, 0x002E, 0x060C, 0x061B, 0x061F, 0x0021, 0x0031},
    /*2*/{ 6, 0x0628, 0x062A, 0x0629, 0x062B, 0x0640, 0x0032},
    /*3*/{ 9, 0x0627, 0x0623, 0x0625, 0x0622, 0x0649, 0x0624, 0x0626, 0x0621, 0x0033},
    /*4*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0034},
    /*5*/{ 5, 0x062F, 0x0630, 0x0631, 0x0632, 0x0035},
    /*6*/{ 4, 0x062C, 0x062D, 0x062E, 0x0036},
    /*7*/{ 5, 0x0646, 0x0647, 0x0648, 0x064A, 0x0037},
    /*8*/{ 6, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0038},
    /*9*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_arabic =
{
    /*1*/  {2, 0x0661, 0x0031},
    /*2*/  {7, 0x0628, 0x062A, 0x0629, 0x062B, 0x0640, 0x0662, 0x0032},
    /*3*/  {10, 0x0627, 0x0623, 0x0625, 0x0622, 0x0649, 0x0624, 0x0626, 0x0621, 0x0663, 0x0033},
    /*4*/  {6, 0x0633, 0x0634, 0x0635, 0x0636, 0x0664, 0x0034},
    /*5*/  {6, 0x062F, 0x0630, 0x0631, 0x0632, 0x0665, 0x0035},
    /*6*/  {5, 0x062C, 0x062D, 0x062E, 0x0666, 0x0036},
    /*7*/  {6, 0x0646, 0x0647, 0x0648, 0x064A, 0x0667, 0x0037},
    /*8*/  {7, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0668, 0x0038},
    /*9*/  {6, 0x0637, 0x0638, 0x0639, 0x063A, 0x0669, 0x0039},
    /*0*/  {2, 0x0660, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_arabic =
{
    /*1*/{14, 0x002E, 0x061F, 0x060C, 0x0021, 0x061B, 0x0022, 0x0040, 0x003A, 0x0031, 0x003B, 0x0028, 0x0029, 0x002A, 0x002D},
    /*2*/{ 4, 0x062C, 0x062D, 0x062E, 0x0032},
    /*3*/{ 8, 0x0627, 0x0623, 0x0625, 0x0622, 0x0628, 0x062A, 0x062B, 0x0033},
    /*4*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0034},
    /*5*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0035},
    /*6*/{ 5, 0x062F, 0x0630, 0x0631, 0x0632, 0x0036},
    /*7*/{ 7, 0x0648, 0x0624, 0x0649, 0x064A, 0x0626, 0x0621, 0x0037},
    /*8*/{ 5, 0x0645, 0x0646, 0x0647, 0x0629, 0x0038},
    /*9*/{ 5, 0x0641, 0x0642, 0x0643, 0x0644, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 法语
#ifdef IM_FRENCH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_french_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C0, 0x00C2, 0x00C7, 0x00C6},
    /*3*/{ 8, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x00C8, 0x00CA, 0x00CB},
    /*4*/{ 6, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CE, 0x00CF},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D4, 0x0152, 0x00D1},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00D9, 0x00DB},
    /*9*/{ 6, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x0178},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_french_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E0, 0x00E2, 0x00E7, 0x00E6},
    /*3*/{ 8, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x00E8, 0x00EA, 0x00EB},
    /*4*/{ 6, 0x0067, 0x0068, 0x0069, 0x0034, 0x00EE, 0x00EF},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F4, 0x0153, 0x00F1},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00F9, 0x00FB},
    /*9*/{ 6, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x00FF},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_french_upper =
{
    /*1*/  {5, 0x0027, 0x002D, 0x002E, 0x002F, 0x0031},
    /*2*/  {7, 0x0041, 0x00C0, 0x00C2, 0x0042, 0x0043, 0x00C7, 0x0032},
    /*3*/  {8, 0x0044, 0x0045, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x0046, 0x0033},
    /*4*/  {6, 0x0047, 0x0048, 0x0049, 0x00CE, 0x00CF, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D4, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {6, 0x0054, 0x0055, 0x00D9, 0x00DB, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_french_lower =
{
    /*1*/  {5, 0x0027, 0x002D, 0x002E, 0x002F, 0x0031},
    /*2*/  {7, 0x0061, 0x00E0, 0x00E2, 0x0062, 0x0063, 0x00E7, 0x0032},
    /*3*/  {8, 0x0064, 0x0065, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x0066, 0x0033},
    /*4*/  {6, 0x0067, 0x0068, 0x0069, 0x00EE, 0x00EF, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F4, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {6, 0x0074, 0x0075, 0x00F9, 0x00FB, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_french_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 'A', 'B', 'C', '2', 0x00C0, 0x00C2, 0x00C7, 0x00C6},
    /*3*/{ 8, 'D', 'E', 'F', '3', 0x00C9, 0x00C8, 0x00CA, 0x00CB},
    /*4*/{ 6, 'G', 'H', 'I', '4', 0x00CE, 0x00CF},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 7, 'M', 'N', 'O', '6', 0x00D4, 0x0152, 0x00D1},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 6, 'T', 'U', 'V', '8', 0x00D9, 0x00DB},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', '9', 0x0178},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_french_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 'a', 'b', 'c', '2', 0x00E0, 0x00E2, 0x00E7, 0x00E6},
    /*3*/{ 8, 'd', 'e', 'f', '3', 0x00E9, 0x00E8, 0x00EA, 0x00EB},
    /*4*/{ 6, 'g', 'h', 'i', '4', 0x00EE, 0x00EF},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 7, 'm', 'n', 'o', '6', 0x00F4, 0x0153, 0x00F1},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 6, 't', 'u', 'v', '8', 0x00F9, 0x00FB},
    /*9*/{ 6, 'w', 'x', 'y', 'z', '9', 0x00FF},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 德语
#ifdef IM_GERMAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_german_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C4, 0x00C0},
    /*3*/{ 5, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 5, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D6},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x00DF},
    /*8*/{ 5, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_german_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E4, 0x00E0},
    /*3*/{ 5, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 5, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F6},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x00DF},
    /*8*/{ 5, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_german_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0041, 0x00C4, 0x0042, 0x0043, 0x0032},
    /*3*/  {4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D6, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {5, 0x0054, 0x0055, 0x00DC, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_german_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0061, 0x00E4, 0x0062, 0x0063, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F6, 0x0036},
    /*7*/  {6, 0x0070, 0x0071, 0x0072, 0x0073, 0x00DF, 0x0037},
    /*8*/  {5, 0x0074, 0x0075, 0x00FC, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_german_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x00C4, 0x00C0},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x00C9},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', '6', 0x00D6},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x00DC},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_german_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x00E4, 0x00E0},
    /*3*/{ 5, 'd', 'e', 'f', '3', 0x00E9},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', '6', 0x00F6},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x00DF},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x00FC},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 希腊语
#ifdef IM_GREEK_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_greek_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ';', '(', ')', '*'},
    /*2*/{ 8, 0x0391, 0x0392, 0x0393, 0x0032, 0x0386, 0x0041, 0x0042, 0x0043},
    /*3*/{ 8, 0x0394, 0x0395, 0x0396, 0x0033, 0x0388, 0x0044, 0x0045, 0x0046},
    /*4*/{10, 0x0397, 0x0398, 0x0399, 0x0034, 0x0389, 0x038A, 0x03AA, 0x0047, 0x0048, 0x0049},
    /*5*/{ 7, 0x039A, 0x039B, 0x039C, 0x0035, 0x004A, 0x004B, 0x004C},
    /*6*/{ 8, 0x039D, 0x039E, 0x039F, 0x0036, 0x038C, 0x004D, 0x004E, 0x004F},
    /*7*/{ 8, 0x03A0, 0x03A1, 0x03A3, 0x0037, 0x0050, 0x0051, 0x0052, 0x0053},
    /*8*/{ 9, 0x03A4, 0x03A5, 0x03A6, 0x0038, 0x038E, 0x03AB, 0x0054, 0x0055, 0x0056},
    /*9*/{ 9, 0x03A7, 0x03A8, 0x03A9, 0x0039, 0x038F, 0x0057, 0x0058, 0x0059, 0x005A},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_greek_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ';', '(', ')', '*'},
    /*2*/{ 8, 0x03B1, 0x03B2, 0x03B3, 0x0032, 0x03AC, 0x0061, 0x0062, 0x0063},
    /*3*/{ 8, 0x03B4, 0x03B5, 0x03B6, 0x0033, 0x03AD, 0x0064, 0x0065, 0x0066},
    /*4*/{11, 0x03B7, 0x03B8, 0x03B9, 0x0034, 0x03AE, 0x03AF, 0x03CA, 0x0390, 0x0067, 0x0068, 0x0069},
    /*5*/{ 7, 0x03BA, 0x03BB, 0x03BC, 0x0035, 0x006A, 0x006B, 0x006C},
    /*6*/{ 8, 0x03BD, 0x03BE, 0x03BF, 0x0036, 0x03CC, 0x006D, 0x006E, 0x006F},
    /*7*/{ 9, 0x03C0, 0x03C1, 0x03C3, 0x03C2, 0x0037, 0x0070, 0x0071, 0x0072, 0x0073},
    /*8*/{10, 0x03C4, 0x03C5, 0x03C6, 0x0038, 0x03CD, 0x03CB, 0x03B0, 0x0074, 0x0075, 0x0076},
    /*9*/{ 9, 0x03C7, 0x03C8, 0x03C9, 0x0039, 0x03CE, 0x0077, 0x0078, 0x0079, 0x007A},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_greek_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0391, 0x0386, 0x0392, 0x0393, 0x0032},
    /*3*/  {5, 0x0394, 0x0395, 0x0388, 0x0396, 0x0033},
    /*4*/  {7, 0x0397, 0x0389, 0x0398, 0x0399, 0x038A, 0x03AA, 0x0034},
    /*5*/  {4, 0x039A, 0x039B, 0x039C, 0x0035},
    /*6*/  {5, 0x039D, 0x039E, 0x039F, 0x038C, 0x0036},
    /*7*/  {4, 0x03A0, 0x03A1, 0x03A3, 0x0037},
    /*8*/  {6, 0x03A4, 0x03A5, 0x038E, 0x03AB, 0x03A6, 0x0038},
    /*9*/  {5, 0x03A7, 0x03A8, 0x03A9, 0x038F, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_greek_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x03B1, 0x03AC, 0x03B2, 0x03B3, 0x0032},
    /*3*/  {5, 0x03B4, 0x03B5, 0x03AD, 0x03B6, 0x0033},
    /*4*/  {8, 0x03B7, 0x03AE, 0x03B8, 0x03B9, 0x03AF, 0x03CA, 0x0390, 0x0034},
    /*5*/  {4, 0x03BA, 0x03BB, 0x03BC, 0x0035},
    /*6*/  {5, 0x03BD, 0x03BE, 0x03BF, 0x03CC, 0x0036},
    /*7*/  {5, 0x03C0, 0x03C1, 0x03C3, 0x03C2, 0x0037},
    /*8*/  {7, 0x03C4, 0x03C5, 0x03CD, 0x03CB, 0x03B0, 0x03C6, 0x0038},
    /*9*/  {5, 0x03C7, 0x03C8, 0x03C9, 0x03CE, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_greek_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ';', '(', ')', '*'},
    /*2*/{ 8, 0x0391, 0x0392, 0x0393, '2', 0x0386, 'A', 'B', 'C'},
    /*3*/{ 8, 0x0394, 0x0395, 0x0396, '3', 0x0388, 'D', 'E', 'F'},
    /*4*/{11, 0x0397, 0x0398, 0x0399, '4', 0x0389, 0x038a, 0x03aa, 0x0390, 'G', 'H', 'I'},
    /*5*/{ 7, 0x039a, 0x039b, 0x039c, '5', 'J', 'K', 'L'},
    /*6*/{ 8, 0x039d, 0x039e, 0x039f, '6', 0x038c, 'M', 'N', 'O'},
    /*7*/{ 8, 0x03a0, 0x03a1, 0x03a3, '7', 'P', 'Q', 'R', 'S'},
    /*8*/{10, 0x03a4, 0x03a5, 0x03a6, '8', 0x038e, 0x03ab, 0x03b0, 'T', 'U', 'V'},
    /*9*/{ 9, 0x03a7, 0x03a8, 0x03a9, '9', 0x038f, 'W', 'X', 'Y', 'Z'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_greek_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ';', '(', ')', '*'},
    /*2*/{ 8, 0x03b1, 0x03b2, 0x03b3, '2', 0x03ac, 'a', 'b', 'c'},
    /*3*/{ 8, 0x03b4, 0x03b5, 0x03b6, '3', 0x03ad, 'd', 'e', 'f'},
    /*4*/{11, 0x03b7, 0x03b8, 0x03b9, '4', 0x03ae, 0x03af, 0x03ca, 0x0390, 'g', 'h', 'i'},
    /*5*/{ 7, 0x03ba, 0x03bb, 0x03bc, '5', 'j', 'k', 'l'},
    /*6*/{ 8, 0x03bd, 0x03be, 0x03bf, '6', 0x03cc, 'm', 'n', 'o'},
    /*7*/{ 9, 0x03c0, 0x03c1, 0x03c3, 0x03c2, '7', 'p', 'q', 'r', 's'},
    /*8*/{10, 0x03c4, 0x03c5, 0x03c6, '8', 0x03cd, 0x03cb, 0x03b0, 't', 'u', 'v'},
    /*9*/{ 9, 0x03c7, 0x03c8, 0x03c9, '9', 0x03ce, 'w', 'x', 'y', 'z'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 印地语
#ifdef IM_HINDI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)

//mini版
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)

#if defined(SPECIAL_HINDI)  //special   908
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindi = 
{
    /*1*/ {8, 0x0905, 0x0907, 0x0909, 0x090B, 0x090F, 0x0911, 0x0913, 0x0031},
    /*2*/ {6, 0x0906, 0x0908, 0x090A, 0x0910, 0x0914, 0x0032},
    /*3*/ {12, 0x093E, 0x093F, 0x0940, 0x0941, 0x0942, 0x0943, 0x0945, 0x0947, 0x0948, 0x0949, 0x094B, 0x0033},
    /*4*/ {10, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x091A, 0x091B, 0x091C, 0x091D, 0x0034},
    /*5*/ {7, 0x091E, 0x091F, 0x0920, 0x0921, 0x0922, 0x0923, 0x0035},
    /*6*/ {6, 0x0924, 0x0925, 0x0926, 0x0927, 0x0928, 0x0036},
    /*7*/ {6, 0x092A, 0x092B, 0x092C, 0x092D, 0x092E, 0x0037},
    /*8*/ {9, 0x092F, 0x0930, 0x0932, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x0038},
    /*9*/ {8, 0x0901, 0x0902, 0x0903, 0x093C, 0x093D, 0x094C, 0x094D, 0x0039},
    /*0*/ {2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else   //907
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindi = 
{
    /*1*/ {15, 0x0901, 0x0902, 0x0903, 0x094D, 0x0970, 0x0950, 0x0964, 0x0965, 0x002E, 0x002C, 0x003F, 0x0021, 0x0027, 0x002D, 0x0031},
    /*2*/ {14, 0x093E, 0x093F, 0x0940, 0x0941, 0x0942, 0x0943, 0x0905, 0x0906, 0x0907, 0x0908, 0x0909, 0x090A, 0x090B, 0x0032},
    /*3*/ {13, 0x0947, 0x0948, 0x0945, 0x094B, 0x094C, 0x0949, 0x090F, 0x0910, 0x090D, 0x0913, 0x0914, 0x0911, 0x0033},
    /*4*/ {9, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x0958, 0x0959, 0x095A, 0x0034},
    /*5*/ {7, 0x091A, 0x091B, 0x091C, 0x091D, 0x091E, 0x095B, 0x0035},
    /*6*/ {8, 0x091F, 0x0920, 0x0921, 0x0922, 0x0923, 0x095C, 0x095D, 0x0036},
    /*7*/ {7, 0x0924, 0x0925, 0x0926, 0x0927, 0x0928, 0x0929, 0x0037},
    /*8*/ {7, 0x092A, 0x092B, 0x092C, 0x092D, 0x092E, 0x095E, 0x0038},
    /*9*/ {13, 0x092F, 0x0930, 0x0932, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x0933, 0x0931, 0x0934, 0x095F, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

#endif      //INPUT_METHOD_CSTAR_COMMON_MINI

#else       //非INPUT_METHOD_CSTAR_COMMON_MINI mini版

#if defined(SPECIAL_HINDI)  //special   906, 与908相同
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindi = 
{
    /*1*/ {8, 0x0905, 0x0907, 0x0909, 0x090B, 0x090F, 0x0911, 0x0913, 0x0031},
    /*2*/ {6, 0x0906, 0x0908, 0x090A, 0x0910, 0x0914, 0x0032},
    /*3*/ {12, 0x093E, 0x093F, 0x0940, 0x0941, 0x0942, 0x0943, 0x0945, 0x0947, 0x0948, 0x0949, 0x094B, 0x0033},
    /*4*/ {10, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x091A, 0x091B, 0x091C, 0x091D, 0x0034},
    /*5*/ {7, 0x091E, 0x091F, 0x0920, 0x0921, 0x0922, 0x0923, 0x0035},
    /*6*/ {6, 0x0924, 0x0925, 0x0926, 0x0927, 0x0928, 0x0036},
    /*7*/ {6, 0x092A, 0x092B, 0x092C, 0x092D, 0x092E, 0x0037},
    /*8*/ {9, 0x092F, 0x0930, 0x0932, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x0038},
    /*9*/ {8, 0x0901, 0x0902, 0x0903, 0x093C, 0x093D, 0x094C, 0x094D, 0x0039},
    /*0*/ {2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else   //900，与907相同
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindi = 
{
    /*1*/ {15, 0x0901, 0x0902, 0x0903, 0x094D, 0x0970, 0x0950, 0x0964, 0x0965, 0x002E, 0x002C, 0x003F, 0x0021, 0x0027, 0x002D, 0x0031},
    /*2*/ {14, 0x093E, 0x093F, 0x0940, 0x0941, 0x0942, 0x0943, 0x0905, 0x0906, 0x0907, 0x0908, 0x0909, 0x090A, 0x090B, 0x0032},
    /*3*/ {13, 0x0947, 0x0948, 0x0945, 0x094B, 0x094C, 0x0949, 0x090F, 0x0910, 0x090D, 0x0913, 0x0914, 0x0911, 0x0033},
    /*4*/ {9, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x0958, 0x0959, 0x095A, 0x0034},
    /*5*/ {7, 0x091A, 0x091B, 0x091C, 0x091D, 0x091E, 0x095B, 0x0035},
    /*6*/ {8, 0x091F, 0x0920, 0x0921, 0x0922, 0x0923, 0x095C, 0x095D, 0x0036},
    /*7*/ {7, 0x0924, 0x0925, 0x0926, 0x0927, 0x0928, 0x0929, 0x0037},
    /*8*/ {7, 0x092A, 0x092B, 0x092C, 0x092D, 0x092E, 0x095E, 0x0038},
    /*9*/ {13, 0x092F, 0x0930, 0x0932, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x0933, 0x0931, 0x0934, 0x095F, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

#endif

#endif      //cstar end

#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindi =
{
    /*1*/  {8, 0x0901, 0x0902, 0x0903, 0x093C, 0x093D, 0x094D, 0x0967, 0x0031},
    /*2*/  {9, 0x0905, 0x0906, 0x0907, 0x0908, 0x0909, 0x090A, 0x090B, 0x0968, 0x0032},
    /*3*/  {8, 0x090D, 0x090F, 0x0910, 0x0911, 0x0913, 0x0914, 0x0969, 0x0033},
    /*4*/  {9, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x091A, 0x091B, 0x096A, 0x0034},
    /*5*/  {9, 0x091C, 0x091D, 0x091E, 0x091F, 0x0920, 0x0921, 0x0922, 0x096B, 0x0035},
    /*6*/  {8, 0x0923, 0x0924, 0x0925, 0x0926, 0x0927, 0x0928, 0x096C, 0x0036},
    /*7*/  {8, 0x092A, 0x092B, 0x092C, 0x092D, 0x092E, 0x092F, 0x096D, 0x0037},
    /*8*/  {9, 0x0930, 0x0932, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x096E, 0x0038},
    /*9*/  {14, 0x093E, 0x093F, 0x0940, 0x0941, 0x0942, 0x0943, 0x0945, 0x0947, 0x0948, 0x0949, 0x094B, 0x094C, 0x096F, 0x0039},
    /*0*/  {2, 0x0960, 0x0030},
};
#else       //非cstar
//hindi, 4个表
//<--hindi begin
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindi =
{
    /*1*/{10, DEVA_L_KA, DEVA_L_KHA, DEVA_L_GA, DEVA_L_GHA, DEVA_L_NGA, DEVA_L_QA, DEVA_L_KHHA, DEVA_L_GHHA, DEVA_D_ONE, '1'},
    /*2*/{ 8, DEVA_L_CA, DEVA_L_CHA, DEVA_L_JA, DEVA_L_JHA, DEVA_L_NYA, DEVA_L_ZA, DEVA_D_TWO, '2'},
    /*3*/{ 9, DEVA_L_TTA, DEVA_L_TTHA, DEVA_L_DDA, DEVA_L_DDHA, DEVA_L_NNA, DEVA_L_DDDHA, DEVA_L_RHA, DEVA_D_THREE, '3'},
    /*4*/{ 8, DEVA_L_TA, DEVA_L_THA, DEVA_L_DA, DEVA_L_DHA, DEVA_L_NA, DEVA_L_NNNA, DEVA_D_FOUR, '4'},
    /*5*/{ 8, DEVA_L_PA, DEVA_L_PHA, DEVA_L_BA, DEVA_L_BHA, DEVA_L_MA, DEVA_L_FA, DEVA_D_FIVE, '5'},
    /*6*/{10, DEVA_L_YA, DEVA_L_RA, DEVA_L_LA, DEVA_L_LLA, DEVA_L_VA, DEVA_L_YYA, DEVA_L_RRA, DEVA_L_LLLA, DEVA_D_SIX, '6'},
    /*7*/{ 6, DEVA_L_SHA, DEVA_L_SSA, DEVA_L_SA, DEVA_L_HA, DEVA_D_SEVEN, '7'},
    /*8*/{15, DEVA_L_A, DEVA_L_AA, DEVA_L_I, DEVA_L_II, DEVA_L_U, DEVA_L_UU, DEVA_LV_R, DEVA_L_E, DEVA_L_AI, DEVA_LC_E, DEVA_L_O, DEVA_L_AU, DEVA_LC_O, DEVA_D_EIGHT, '8'},
    /*9*/{ 2, DEVA_D_NINE, '9'},
    /*0*/{ 7, ' ', '.', DEVA_DANDA, DEVA_OM, DEVA_DOUBLE_DANDA, DEVA_D_ZERO, 0x20a8, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindimatra =
{
    /*1*/{10, DEVA_L_KA, DEVA_L_KHA, DEVA_L_GA, DEVA_L_GHA, DEVA_L_NGA, DEVA_L_QA, DEVA_L_KHHA, DEVA_L_GHHA, DEVA_D_ONE, '1'},
    /*2*/{ 8, DEVA_L_CA, DEVA_L_CHA, DEVA_L_JA, DEVA_L_JHA, DEVA_L_NYA, DEVA_L_ZA, DEVA_D_TWO, '2'},
    /*3*/{ 9, DEVA_L_TTA, DEVA_L_TTHA, DEVA_L_DDA, DEVA_L_DDHA, DEVA_L_NNA, DEVA_L_DDDHA, DEVA_L_RHA, DEVA_D_THREE, '3'},
    /*4*/{ 8, DEVA_L_TA, DEVA_L_THA, DEVA_L_DA, DEVA_L_DHA, DEVA_L_NA, DEVA_L_NNNA, DEVA_D_FOUR, '4'},
    /*5*/{ 8, DEVA_L_PA, DEVA_L_PHA, DEVA_L_BA, DEVA_L_BHA, DEVA_L_MA, DEVA_L_FA, DEVA_D_FIVE, '5'},
    /*6*/{10, DEVA_L_YA, DEVA_L_RA, DEVA_L_LA, DEVA_L_LLA, DEVA_L_VA, DEVA_L_YYA, DEVA_L_RRA, DEVA_L_LLLA, DEVA_D_SIX, '6'},
    /*7*/{ 6, DEVA_L_SHA, DEVA_L_SSA, DEVA_L_SA, DEVA_L_HA, DEVA_D_SEVEN, '7'},
    /*8*/{15, DEVA_L_A, DEVA_L_AA, DEVA_L_I, DEVA_L_II, DEVA_L_U, DEVA_L_UU, DEVA_LV_R, DEVA_L_E, DEVA_L_AI, DEVA_LC_E, DEVA_L_O, DEVA_L_AU, DEVA_LC_O, DEVA_D_EIGHT, '8'},
    /*9*/{14, DEVA_VS_AA, DEVA_VS_I, DEVA_VS_II, DEVA_VS_U, DEVA_VS_UU, DEVA_VSV_R, DEVA_VS_E, DEVA_VS_AI, DEVA_VSC_E, DEVA_VS_O, DEVA_VS_AU, DEVA_VSC_O, DEVA_D_NINE, '9'},
    /*0*/{12, ' ', DEVA_S_VIRAMA, DEVA_S_CANDRABINDU, DEVA_S_ANUSVARA, DEVA_S_VISARGA, DEVA_OM, DEVA_DANDA, DEVA_ABBREVIATION_SIGN, DEVA_DOUBLE_DANDA, '.', DEVA_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindimatra1 =
{
    /*1*/{10, DEVA_L_KA, DEVA_L_KHA, DEVA_L_GA, DEVA_L_GHA, DEVA_L_NGA, DEVA_L_QA, DEVA_L_KHHA, DEVA_L_GHHA, DEVA_D_ONE, '1'},
    /*2*/{ 8, DEVA_L_CA, DEVA_L_CHA, DEVA_L_JA, DEVA_L_JHA, DEVA_L_NYA, DEVA_L_ZA, DEVA_D_TWO, '2'},
    /*3*/{ 9, DEVA_L_TTA, DEVA_L_TTHA, DEVA_L_DDA, DEVA_L_DDHA, DEVA_L_NNA, DEVA_L_DDDHA, DEVA_L_RHA, DEVA_D_THREE, '3'},
    /*4*/{ 8, DEVA_L_TA, DEVA_L_THA, DEVA_L_DA, DEVA_L_DHA, DEVA_L_NA, DEVA_L_NNNA, DEVA_D_FOUR, '4'},
    /*5*/{ 8, DEVA_L_PA, DEVA_L_PHA, DEVA_L_BA, DEVA_L_BHA, DEVA_L_MA, DEVA_L_FA, DEVA_D_FIVE, '5'},
    /*6*/{10, DEVA_L_YA, DEVA_L_RA, DEVA_L_LA, DEVA_L_LLA, DEVA_L_VA, DEVA_L_YYA, DEVA_L_RRA, DEVA_L_LLLA, DEVA_D_SIX, '6'},
    /*7*/{ 6, DEVA_L_SHA, DEVA_L_SSA, DEVA_L_SA, DEVA_L_HA, DEVA_D_SEVEN, '7'},
    /*8*/{15, DEVA_L_A, DEVA_L_AA, DEVA_L_I, DEVA_L_II, DEVA_L_U, DEVA_L_UU, DEVA_LV_R, DEVA_L_E, DEVA_L_AI, DEVA_LC_E, DEVA_L_O, DEVA_L_AU, DEVA_LC_O, DEVA_D_EIGHT, '8'},
    /*9*/{ 2, DEVA_D_NINE, '9'},
    /*0*/{11, ' ', DEVA_S_CANDRABINDU, DEVA_S_ANUSVARA, DEVA_S_VISARGA, DEVA_OM, DEVA_DANDA, DEVA_ABBREVIATION_SIGN, DEVA_DOUBLE_DANDA, '.', DEVA_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hindimatra2 =
{
    /*1*/{10, DEVA_L_KA, DEVA_L_KHA, DEVA_L_GA, DEVA_L_GHA, DEVA_L_NGA, DEVA_L_QA, DEVA_L_KHHA, DEVA_L_GHHA, DEVA_D_ONE, '1'},
    /*2*/{ 8, DEVA_L_CA, DEVA_L_CHA, DEVA_L_JA, DEVA_L_JHA, DEVA_L_NYA, DEVA_L_ZA, DEVA_D_TWO, '2'},
    /*3*/{ 9, DEVA_L_TTA, DEVA_L_TTHA, DEVA_L_DDA, DEVA_L_DDHA, DEVA_L_NNA, DEVA_L_DDDHA, DEVA_L_RHA, DEVA_D_THREE, '3'},
    /*4*/{ 8, DEVA_L_TA, DEVA_L_THA, DEVA_L_DA, DEVA_L_DHA, DEVA_L_NA, DEVA_L_NNNA, DEVA_D_FOUR, '4'},
    /*5*/{ 8, DEVA_L_PA, DEVA_L_PHA, DEVA_L_BA, DEVA_L_BHA, DEVA_L_MA, DEVA_L_FA, DEVA_D_FIVE, '5'},
    /*6*/{10, DEVA_L_YA, DEVA_L_RA, DEVA_L_LA, DEVA_L_LLA, DEVA_L_VA, DEVA_L_YYA, DEVA_L_RRA, DEVA_L_LLLA, DEVA_D_SIX, '6'},
    /*7*/{ 6, DEVA_L_SHA, DEVA_L_SSA, DEVA_L_SA, DEVA_L_HA, DEVA_D_SEVEN, '7'},
    /*8*/{15, DEVA_L_A, DEVA_L_AA, DEVA_L_I, DEVA_L_II, DEVA_L_U, DEVA_L_UU, DEVA_LV_R, DEVA_L_E, DEVA_L_AI, DEVA_LC_E, DEVA_L_O, DEVA_L_AU, DEVA_LC_O, DEVA_D_EIGHT, '8'},
    /*9*/{ 2, DEVA_D_NINE, '9'},
    /*0*/{10, ' ', '.', DEVA_S_ANUSVARA, DEVA_S_VISARGA, DEVA_OM, DEVA_DANDA, DEVA_ABBREVIATION_SIGN, DEVA_DOUBLE_DANDA, DEVA_D_ZERO, 0x20a8, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
//-->hindi end
#endif

// 匈牙利语
#ifdef IM_HUNGARIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hungarian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 6, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C1, 0x00C0},
    /*3*/{ 5, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CD},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D3, 0x00D6, 0x0150},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 7, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DA, 0x00DC, 0x0170},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hungarian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 6, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E1, 0x00E0},
    /*3*/{ 5, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00ED},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F3, 0x00F6, 0x0151},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 7, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FA, 0x00FC, 0x0171},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hungarian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{11, 'A', 'B', 'C', 0x00C1, 0x00C4, 0x0102, 0x00C2, 0x0104, 0x0106, 0x010C, '2'},
    /*3*/{10, 'D', 'E', 'F', 0x00C9, 0x00CB, 0x011A, 0x0118, 0x010E, 0x0110, '3'},
    /*4*/{ 6, 'G', 'H', 'I', 0x00CD, 0x00CE, '4'},
    /*5*/{ 7, 'J', 'K', 'L', 0x0139, 0x013D, 0x0141, '5'},
    /*6*/{10, 'M', 'N', 'O', 0x00D3, 0x00D6, 0x0150, 0x00D4, 0x0143, 0x0147, '6'},
    /*7*/{10, 'P', 'Q', 'R', 'S', 0x0154, 0x0158, 0x015A, 0x0160, 0x015E, '7'},
    /*8*/{10, 'T', 'U', 'V', 0x00DA, 0x00DC, 0x0170, 0x0164, 0x0162, 0x016E, '8'},
    /*9*/{ 9, 'W', 'X', 'Y', 'Z', 0x00DD, 0x0179, 0x017D, 0x017B, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hungarian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{11, 'a', 'b', 'c', 0x00E1, 0x00E4, 0x0103, 0x00E2, 0x0105, 0x0107, 0x010D, '2'},
    /*3*/{10, 'd', 'e', 'f', 0x00E9, 0x00EB, 0x011B, 0x0119, 0x010F, 0x0111, '3'},
    /*4*/{ 6, 'g', 'h', 'i', 0x00ED, 0x00EE, '4'},
    /*5*/{ 7, 'j', 'k', 'l', 0x013A, 0x013E, 0x0142, '5'},
    /*6*/{10, 'm', 'n', 'o', 0x00F3, 0x00F6, 0x0151, 0x00F4, 0x0144, 0x0148, '6'},
    /*7*/{11, 'p', 'q', 'r', 's', 0x0155, 0x0159, 0x015B, 0x0161, 0x015F, 0x00DF, '7'},
    /*8*/{10, 't', 'u', 'v', 0x00FA, 0x00FC, 0x0171, 0x0165, 0x0163, 0x016F, '8'},
    /*9*/{ 9, 'w', 'x', 'y', 'z', 0x00FD, 0x017A, 0x017E, 0x017C, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 印度尼西亚语
#ifdef IM_INDONESIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_indonesian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 0x0041, 0x0042, 0x0043, 0x0032},
    /*3*/{ 4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_indonesian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 0x0061, 0x0062, 0x0063, 0x0032},
    /*3*/{ 4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined(MMIIM_SOGOU_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_indonesian_upper =
{
#if defined MMI_IM_PDA_SUPPORT
    /*1*/{0},
#else
    /*1*/{13, ',', '.', '?', '!', '\'', '"', '-', '@', ':', ';', '/', '_', '1'},
#endif    
    /*2*/{4, 'A', 'B', 'C', '2'},
    /*3*/{4, 'D', 'E', 'F', '3'},
    /*4*/{4, 'G', 'H', 'I', '4'},
    /*5*/{4, 'J', 'K', 'L', '5'},
    /*6*/{4, 'M', 'N', 'O', '6'},
    /*7*/{5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{4, 'T', 'U', 'V', '8'},
    /*9*/{5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_indonesian_lower =
{
#if defined MMI_IM_PDA_SUPPORT
    /*1*/{0},
#else
    /*1*/{13, ',', '.', '?', '!', '\'', '"', '-', '@', ':', ';', '/', '_', '1'},
#endif    
    /*2*/{4, 'a', 'b', 'c', '2'},
    /*3*/{4, 'd', 'e', 'f', '3'},
    /*4*/{4, 'g', 'h', 'i', '4'},
    /*5*/{4, 'j', 'k', 'l', '5'},
    /*6*/{4, 'm', 'n', 'o', '6'},
    /*7*/{5, 'p', 'q', 'r', 's', '7'},
    /*8*/{4, 't', 'u', 'v', '8'},
    /*9*/{5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_indonesian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 'B', 'C', '2', 0x00C6},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x00C9},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 6, 'M', 'N', 'O', '6', 0x00D1, 0x00D6},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_indonesian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 'a', 'b', 'c', '2', 0x00E0, 0x00E6, 0x00E7},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x00E9, 0x00E8},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 6, 'm', 'n', 'o', '6', 0x00F1, 0x00F6},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 意大利语
#ifdef IM_ITALIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_italian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C0},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x00C8},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CC},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 5, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D2},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 5, 0x0054, 0x0055, 0x0056, 0x0038, 0x00D9},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_italian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E0},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x00E8},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00EC},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 5, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F2},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 5, 0x0074, 0x0075, 0x0076, 0x0038, 0x00F9},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_italian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0041, 0x00C0, 0x0042, 0x0043, 0x0032},
    /*3*/  {6, 0x0044, 0x0045, 0x00C8, 0x00C9, 0x0046, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CC, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D2, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {5, 0x0054, 0x0055, 0x00D9, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_italian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0061, 0x00E0, 0x0062, 0x0063, 0x0032},
    /*3*/  {6, 0x0064, 0x0065, 0x00E8, 0x00E9, 0x0066, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00EC, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F2, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {5, 0x0074, 0x0075, 0x00F9, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_italian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 'B', 'C', '2', 0x00C0},
    /*3*/{ 6, 'D', 'E', 'F', '3', 0x00C8, 0x00C9},
    /*4*/{ 5, 'G', 'H', 'I', '4', 0x00CC},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', '6', 0x00D2},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x00D9},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_italian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'a', 'b', 'c', '2', 0x00E0},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x00E8, 0x00E9},
    /*4*/{ 5, 'g', 'h', 'i', '4', 0x00EC},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', '6', 0x00F2},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x00F9},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 波斯语
#ifdef IM_PERSIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)     //mini版本，使用469 or 467
#if defined(SPECIAL_ARABIC_PERSIAN) 
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_persian =      //469
{
    /*1*/{14, 0x002E, 0x061F, 0x060C, 0x0021, 0x061B, 0x0022, 0x0040, 0x003A, 0x0031, 0x003B, 0x0028, 0x0029, 0x002A, 0x002D},
    /*2*/{ 5, 0x062C, 0x062D, 0x062E, 0x0686, 0x0032},
    /*3*/{ 9, 0x0627, 0x0623, 0x0625, 0x0622, 0x0628, 0x062A, 0x062B, 0x067E, 0x0033},
    /*4*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0034},
    /*5*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0035},
    /*6*/{ 6, 0x062F, 0x0630, 0x0631, 0x0632, 0x0698, 0x0036},
    /*7*/{ 8, 0x0648, 0x0624, 0x0649, 0x064A, 0x0626, 0x0621, 0x06CC, 0x0037},
    /*8*/{ 5, 0x0645, 0x0646, 0x0647, 0x0629, 0x0038},
    /*9*/{ 7, 0x0641, 0x0642, 0x0643, 0x0644, 0x06A9, 0x06AF, 0x0039},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_persian =      
{
    /*1*/{29, 0x002E, 0x060C, 0x061F, 0x0021, 0x003A, 0x061B, 0x06F1, 0x0040, 0x002F, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x0025, 0x003D, 0x002B, 0x002D, 0x0023, 0x002A, 0x003C, 0x003E, 0x005B, 0x005D, 0x007B, 0x007D, 0x0026, 0x005C, 0x005F, 0x007C},
    /*2*/{ 6, 0x0628, 0x067E, 0x062A, 0x0629, 0x062B, 0x06F2},
    /*3*/{ 8, 0x0627, 0x0623, 0x0625, 0x0622, 0x0624, 0x0626, 0x0621, 0x06F3},
    /*4*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x06F4},
    /*5*/{ 6, 0x062F, 0x0630, 0x0631, 0x0632, 0x0698, 0x06F5},
    /*6*/{ 5, 0x062C, 0x0686, 0x062D, 0x062E, 0x06F6},
    /*7*/{ 5, 0x0646, 0x0647, 0x0648, 0x06CC, 0x06F7},
    /*8*/{ 7, 0x0641, 0x0642, 0x06A9, 0x06AF, 0x0644, 0x0645, 0x06F8},
    /*9*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x06F9},
    /*0*/{ 2, 0x0020, 0x06F0},
    /***/{0},
    /*#*/{0}
};
#endif
#else           //正常版本，使用463 or 460
#if defined(SPECIAL_ARABIC_PERSIAN)         //463与469相同
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_persian =
{
    /*1*/{14, 0x002E, 0x061F, 0x060C, 0x0021, 0x061B, 0x0022, 0x0040, 0x003A, 0x0031, 0x003B, 0x0028, 0x0029, 0x002A, 0x002D},
    /*2*/{ 5, 0x062C, 0x062D, 0x062E, 0x0686, 0x0032},
    /*3*/{ 9, 0x0627, 0x0623, 0x0625, 0x0622, 0x0628, 0x062A, 0x062B, 0x067E, 0x0033},
    /*4*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0034},
    /*5*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0035},
    /*6*/{ 6, 0x062F, 0x0630, 0x0631, 0x0632, 0x0698, 0x0036},
    /*7*/{ 8, 0x0648, 0x0624, 0x0649, 0x064A, 0x0626, 0x0621, 0x06CC, 0x0037},
    /*8*/{ 5, 0x0645, 0x0646, 0x0647, 0x0629, 0x0038},
    /*9*/{ 7, 0x0641, 0x0642, 0x0643, 0x0644, 0x06A9, 0x06AF, 0x0039},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_persian =      //460与467相同
{
    /*1*/{29, 0x002E, 0x060C, 0x061F, 0x0021, 0x003A, 0x061B, 0x06F1, 0x0040, 0x002F, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x0025, 0x003D, 0x002B, 0x002D, 0x0023, 0x002A, 0x003C, 0x003E, 0x005B, 0x005D, 0x007B, 0x007D, 0x0026, 0x005C, 0x005F, 0x007C},
    /*2*/{ 6, 0x0628, 0x067E, 0x062A, 0x0629, 0x062B, 0x06F2},
    /*3*/{ 8, 0x0627, 0x0623, 0x0625, 0x0622, 0x0624, 0x0626, 0x0621, 0x06F3},
    /*4*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x06F4},
    /*5*/{ 6, 0x062F, 0x0630, 0x0631, 0x0632, 0x0698, 0x06F5},
    /*6*/{ 5, 0x062C, 0x0686, 0x062D, 0x062E, 0x06F6},
    /*7*/{ 5, 0x0646, 0x0647, 0x0648, 0x06CC, 0x06F7},
    /*8*/{ 7, 0x0641, 0x0642, 0x06A9, 0x06AF, 0x0644, 0x0645, 0x06F8},
    /*9*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x06F9},
    /*0*/{ 2, 0x0020, 0x06F0},
    /***/{0},
    /*#*/{0}
};
#endif

#endif
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_persian =
{
    /*1*/  {2, 0x0661, 0x0031},
    /*2*/  {6, 0x0628, 0x067E, 0x062A, 0x062B, 0x0662, 0x0032},
    /*3*/  {5, 0x0627, 0x0622, 0x06CC, 0x0663, 0x0033},
    /*4*/  {6, 0x0633, 0x0634, 0x0635, 0x0636, 0x0664, 0x0034},
    /*5*/  {7, 0x062F, 0x0630, 0x0631, 0x0632, 0x0698, 0x0665, 0x0035},
    /*6*/  {6, 0x062C, 0x0686, 0x062D, 0x062E, 0x0666, 0x0036},
    /*7*/  {5, 0x0646, 0x0648, 0x0647, 0x0667, 0x0037},
    /*8*/  {8, 0x0641, 0x0642, 0x06AF, 0x0644, 0x0645, 0x06A9, 0x0668, 0x0038},
    /*9*/  {6, 0x0637, 0x0638, 0x0639, 0x063A, 0x0669, 0x0039},
    /*0*/  {2, 0x0660, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_persian =
{
    /*1*/{14, 0x002E, 0x061F, 0x060C, 0x0021, 0x061B, 0x0022, 0x0040, 0x003A, 0x0031, 0x003B, 0x0028, 0x0029, 0x002A, 0x002D},
    /*2*/{ 5, 0x062C, 0x062D, 0x062E, 0x0686, 0x0032},
    /*3*/{ 9, 0x0627, 0x0623, 0x0625, 0x0622, 0x0628, 0x062A, 0x062B, 0x067E, 0x0033},
    /*4*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0034},
    /*5*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0035},
    /*6*/{ 6, 0x062F, 0x0630, 0x0631, 0x0632, 0x0698, 0x0036},
    /*7*/{ 8, 0x0648, 0x0624, 0x0649, 0x064A, 0x0626, 0x0621, 0x06CC, 0x0037},
    /*8*/{ 5, 0x0645, 0x0646, 0x0647, 0x0629, 0x0038},
    /*9*/{ 7, 0x0641, 0x0642, 0x0643, 0x0644, 0x06A9, 0x06AF, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 葡萄牙语
#ifdef IM_PORTUGUESE_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_portuguese_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C1, 0x00C0, 0x00C2, 0x00C3, 0x00C7},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x00CA},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CD},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D3, 0x00D4, 0x00D5},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DA, 0x00DC},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_portuguese_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E1, 0x00E0, 0x00E2, 0x00E3, 0x00E7},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x00EA},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00ED},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F3, 0x00F4, 0x00F5},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FA, 0x00FC},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_portuguese_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {9, 0x0041, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x0042, 0x0043, 0x00C7, 0x0032},
    /*3*/  {6, 0x0044, 0x0045, 0x00C9, 0x00CA, 0x0046, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CD, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {7, 0x004D, 0x004E, 0x004F, 0x00D3, 0x00D4, 0x00D5, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {6, 0x0054, 0x0055, 0x00DA, 0x00DC, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_portuguese_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {9, 0x0061, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x0062, 0x0063, 0x00E7, 0x0032},
    /*3*/  {6, 0x0064, 0x0065, 0x00E9, 0x00EA, 0x0066, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00ED, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {7, 0x006D, 0x006E, 0x006F, 0x00F3, 0x00F4, 0x00F5, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {6, 0x0074, 0x0075, 0x00FA, 0x00FC, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_portuguese_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 'A', 'B', 'C', '2', 0x00C1, 0x00C0, 0x00C2, 0x00C3, 0x00C7},
    /*3*/{ 6, 'D', 'E', 'F', '3', 0x00C9, 0x00CA},
    /*4*/{ 5, 'G', 'H', 'I', '4', 0x00CD},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 7, 'M', 'N', 'O', '6', 0x00D3, 0x00D4, 0x00D5},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 6, 'T', 'U', 'V', '8', 0x00DA, 0x00DC},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_portuguese_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 'a', 'b', 'c', '2', 0x00E1, 0x00E0, 0x00E2, 0x00E3, 0x00E7},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x00E9, 0x00EA},
    /*4*/{ 5, 'g', 'h', 'i', '4', 0x00ED},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 7, 'm', 'n', 'o', '6', 0x00F3, 0x00F4, 0x00F5},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 6, 't', 'u', 'v', '8', 0x00FA, 0x00FC},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 俄语
#ifdef IM_RUSSIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)    
 //mini版本，使用207，这个1键某些字符被去掉，因MMIIM_SP_9KEY_TABLE_DEF_T长度限制原因，
 //这个长度不合理，但现在来不及修改
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_upper =
{
    /*1*/{37, 0x002E, 0x002C, 0x003F, 0x0021, 0x0027, 0x002D, 0x0031, 0x003A, 0x003B, 0x0022, 0x00B7, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0023, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x005B, 0x005D, 0x007B, 0x007D, 0x003C, 0x003E, 0x0024, 0x007C, 0x002B, 0x003D, 0x002A, 0x007E, 0x005E, 0x005F, 0x0060, 0x00A7},
    /*2*/{ 8, 0x0410, 0x0411, 0x0412, 0x0413, 0x0032, 0x0041, 0x0042, 0x0043},
    /*3*/{ 9, 0x0414, 0x0415, 0x0401, 0x0416, 0x0417, 0x0033, 0x0044, 0x0045, 0x0046},
    /*4*/{ 8, 0x0418, 0x0419, 0x041A, 0x041B, 0x0034, 0x0047, 0x0048, 0x0049},
    /*5*/{ 8, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035, 0x004A, 0x004B, 0x004C},
    /*6*/{ 8, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036, 0x004D, 0x004E, 0x004F},
    /*7*/{ 9, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037, 0x0050, 0x0051, 0x0052, 0x0053},
    /*8*/{ 8, 0x0428, 0x0429, 0x042A, 0x042B, 0x0038, 0x0054, 0x0055, 0x0056},
    /*9*/{ 9, 0x042C, 0x042D, 0x042E, 0x042F, 0x0039, 0x0057, 0x0058, 0x0059, 0x005A},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_lower =
{
    /*1*/{37, 0x002E, 0x002C, 0x003F, 0x0021, 0x0027, 0x002D, 0x0031, 0x003A, 0x003B, 0x0022, 0x00B7, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0023, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x005B, 0x005D, 0x007B, 0x007D, 0x003C, 0x003E, 0x0024, 0x007C, 0x002B, 0x003D, 0x002A, 0x007E, 0x005E, 0x005F, 0x0060, 0x00A7},
    /*2*/{ 8, 0x0430, 0x0431, 0x0432, 0x0433, 0x0032, 0x0061, 0x0062, 0x0063},
    /*3*/{ 9, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437, 0x0033, 0x0064, 0x0065, 0x0066},
    /*4*/{ 8, 0x0438, 0x0439, 0x043A, 0x043B, 0x0034, 0x0067, 0x0068, 0x0069},
    /*5*/{ 8, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035, 0x006A, 0x006B, 0x006C},
    /*6*/{ 8, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036, 0x006D, 0x006E, 0x006F},
    /*7*/{ 9, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037, 0x0070, 0x0071, 0x0072, 0x0073},
    /*8*/{ 8, 0x0448, 0x0449, 0x044A, 0x044B, 0x0038, 0x0074, 0x0075, 0x0076},
    /*9*/{ 9, 0x044C, 0x044D, 0x044E, 0x044F, 0x0039, 0x0077, 0x0078, 0x0079, 0x007A},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else
//202与207相同
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_upper =
{
    /*1*/{37, 0x002E, 0x002C, 0x003F, 0x0021, 0x0027, 0x002D, 0x0031, 0x003A, 0x003B, 0x0022, 0x00B7, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0023, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x005B, 0x005D, 0x007B, 0x007D, 0x003C, 0x003E, 0x0024, 0x007C, 0x002B, 0x003D, 0x002A, 0x007E, 0x005E, 0x005F, 0x0060, 0x00A7},
    /*2*/{ 8, 0x0410, 0x0411, 0x0412, 0x0413, 0x0032, 0x0041, 0x0042, 0x0043},
    /*3*/{ 9, 0x0414, 0x0415, 0x0401, 0x0416, 0x0417, 0x0033, 0x0044, 0x0045, 0x0046},
    /*4*/{ 8, 0x0418, 0x0419, 0x041A, 0x041B, 0x0034, 0x0047, 0x0048, 0x0049},
    /*5*/{ 8, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035, 0x004A, 0x004B, 0x004C},
    /*6*/{ 8, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036, 0x004D, 0x004E, 0x004F},
    /*7*/{ 9, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037, 0x0050, 0x0051, 0x0052, 0x0053},
    /*8*/{ 8, 0x0428, 0x0429, 0x042A, 0x042B, 0x0038, 0x0054, 0x0055, 0x0056},
    /*9*/{ 9, 0x042C, 0x042D, 0x042E, 0x042F, 0x0039, 0x0057, 0x0058, 0x0059, 0x005A},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_lower =
{
    /*1*/{37, 0x002E, 0x002C, 0x003F, 0x0021, 0x0027, 0x002D, 0x0031, 0x003A, 0x003B, 0x0022, 0x00B7, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0023, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x005B, 0x005D, 0x007B, 0x007D, 0x003C, 0x003E, 0x0024, 0x007C, 0x002B, 0x003D, 0x002A, 0x007E, 0x005E, 0x005F, 0x0060, 0x00A7},
    /*2*/{ 8, 0x0430, 0x0431, 0x0432, 0x0433, 0x0032, 0x0061, 0x0062, 0x0063},
    /*3*/{ 9, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437, 0x0033, 0x0064, 0x0065, 0x0066},
    
    /*4*/{ 8, 0x0438, 0x0439, 0x043A, 0x043B, 0x0034, 0x0067, 0x0068, 0x0069},
    /*5*/{ 8, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035, 0x006A, 0x006B, 0x006C},
    
    /*6*/{ 8, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036, 0x006D, 0x006E, 0x006F},
    /*7*/{ 9, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037, 0x0070, 0x0071, 0x0072, 0x0073},
    
    /*8*/{ 8, 0x0448, 0x0449, 0x044A, 0x044B, 0x0038, 0x0074, 0x0075, 0x0076},
    /*9*/{ 9, 0x044C, 0x044D, 0x044E, 0x044F, 0x0039, 0x0077, 0x0078, 0x0079, 0x007A},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};
#endif
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0410, 0x0411, 0x0412, 0x0413, 0x0032},
    /*3*/  {6, 0x0414, 0x0415, 0x0416, 0x0417, 0x0401, 0x0033},
    /*4*/  {5, 0x0418, 0x0419, 0x041A, 0x041B, 0x0034},
    /*5*/  {5, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035},
    /*6*/  {5, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036},
    /*7*/  {5, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037},
    /*8*/  {5, 0x0428, 0x0429, 0x042A, 0x042B, 0x0038},
    /*9*/  {5, 0x042C, 0x042D, 0x042E, 0x042F, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0430, 0x0431, 0x0432, 0x0433, 0x0032},
    /*3*/  {6, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437, 0x0033},
    /*4*/  {5, 0x0438, 0x0439, 0x043A, 0x043B, 0x0034},
    /*5*/  {5, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035},
    /*6*/  {5, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036},
    /*7*/  {5, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037},
    /*8*/  {5, 0x0448, 0x0449, 0x044A, 0x044B, 0x0038},
    /*9*/  {5, 0x044C, 0x044D, 0x044E, 0x044F, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0410, 0x0411, 0x0412, 0x0413, '2', 'A', 'B', 'C'},
    /*3*/{ 9, 0x0414, 0x0415, 0x0401, 0x0416, 0x0417, '3', 'D', 'E', 'F'},
    /*4*/{ 8, 0x0418, 0x0419, 0x041a, 0x041b, '4', 'G', 'H', 'I'},
    /*5*/{ 8, 0x041c, 0x041d, 0x041e, 0x041f, '5', 'J', 'K', 'L'},
    /*6*/{ 8, 0x0420, 0x0421, 0x0422, 0x0423, '6', 'M', 'N', 'O'},
    /*7*/{ 9, 0x0424, 0x0425, 0x0426, 0x0427, '7', 'P', 'Q', 'R', 'S'},
    /*8*/{ 8, 0x0428, 0x0429, 0x042a, 0x042b, '8', 'T', 'U', 'V'},
    /*9*/{ 9, 0x042c, 0x042d, 0x042e, 0x042f, '9', 'W', 'X', 'Y', 'Z'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_russian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0430, 0x0431, 0x0432, 0x0433, '2', 'a', 'b', 'c'},
    /*3*/{ 9, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437, '3', 'd', 'e', 'f'},
    /*4*/{ 8, 0x0438, 0x0439, 0x043a, 0x043b, '4', 'g', 'h', 'i'},
    /*5*/{ 8, 0x043c, 0x043d, 0x043e, 0x043f, '5', 'j', 'k', 'l'},
    /*6*/{ 8, 0x0440, 0x0441, 0x0442, 0x0443, '6', 'm', 'n', 'o'},
    /*7*/{ 9, 0x0444, 0x0445, 0x0446, 0x0447, '7', 'p', 'q', 'r', 's'},
    /*8*/{ 8, 0x0448, 0x0449, 0x044a, 0x044b, '8', 't', 'u', 'v'},
    /*9*/{ 9, 0x044c, 0x044d, 0x044e, 0x044f, '9', 'w', 'x', 'y', 'z'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 西班牙语
#ifdef IM_SPANISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_spanish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C1, 0x00C0, 0x00C7},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x00C8},
    /*4*/{ 6, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CD, 0x00CF},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D1, 0x00D3, 0x00D2},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DA, 0x00DC},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_spanish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E1, 0x00E0, 0x00E7},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x00E8},
    /*4*/{ 6, 0x0067, 0x0068, 0x0069, 0x0034, 0x00ED, 0x00EF},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F1, 0x00F3, 0x00F2},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FA, 0x00FC},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_spanish_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0041, 0x00C1, 0x0042, 0x0043, 0x0032},
    /*3*/  {5, 0x0044, 0x0045, 0x00C9, 0x0046, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CD, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {6, 0x004D, 0x004E, 0x00D1, 0x004F, 0x00D3, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {6, 0x0054, 0x0055, 0x00DA, 0x00DC, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_spanish_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0061, 0x00E1, 0x0062, 0x0063, 0x0032},
    /*3*/  {5, 0x0064, 0x0065, 0x00E9, 0x0066, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00ED, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {6, 0x006D, 0x006E, 0x00F1, 0x006F, 0x00F3, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {6, 0x0074, 0x0075, 0x00FA, 0x00FC, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_spanish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 'A', 'B', 'C', '2', 0x00C1, 0x00C0, 0x00C7},
    /*3*/{ 6, 'D', 'E', 'F', '3', 0x00C9, 0x00C8},
    /*4*/{ 6, 'G', 'H', 'I', '4', 0x00CD, 0x00CF},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 7, 'M', 'N', 'O', '6', 0x00D1, 0x00D3, 0x00D2},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 6, 'T', 'U', 'V', '8', 0x00DA, 0x00DC},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_spanish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 'a', 'b', 'c', '2', 0x00E1, 0x00E0, 0x00E7},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x00E9, 0x00E8},
    /*4*/{ 6, 'g', 'h', 'i', '4', 0x00ED, 0x00EF},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 7, 'm', 'n', 'o', '6', 0x00F1, 0x00F3, 0x00F2},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 6, 't', 'u', 'v', '8', 0x00FA, 0x00FC},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 他加禄语
#ifdef IM_TAGALOG_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tagalog =
{
    /*1*/  {1, 0x0031},
    /*2*/  {4, 0x0061, 0x0062, 0x0063, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
//0X1700~0X171F
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tagalog =
{
    /*1*/  {1, 0x0031},
    /*2*/  {4, 0x0061, 0x0062, 0x0063, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#endif
#endif

// 泰语
#ifdef IM_THAI_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)

#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
#if defined(SPECIAL_THAI)  //special   368, 364
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai = 
{
    /*1*/ {11, 0x0E01, 0x0E02, 0x0E03, 0x0E04, 0x0E05, 0x0E06, 0x0E07, 0x0E08, 0x0031, 0x0E30, 0x0E31},
    /*2*/ {8, 0x0E09, 0x0E0A, 0x0E0B, 0x0E0C, 0x0E0D, 0x0032, 0x0E32, 0x0E33},
    /*3*/ {9, 0x0E0E, 0x0E0F, 0x0E10, 0x0E11, 0x0E12, 0x0E13, 0x0033, 0x0E34, 0x0E35},
    /*4*/ {9, 0x0E14, 0x0E15, 0x0E16, 0x0E17, 0x0E18, 0x0E19, 0x0034, 0x0E36, 0x0E37},
    /*5*/ {7, 0x0E48, 0x0E49, 0x0E4A, 0x0E4B, 0x0E4C, 0x0E47, 0x0035},
    /*6*/ {9, 0x0E1A, 0x0E1B, 0x0E1C, 0x0E1D, 0x0E1E, 0x0E1F, 0x0036, 0x0E38, 0x0E39},
    /*7*/ {13, 0x0E20, 0x0E21, 0x0E22, 0x0E23, 0x0E25, 0x0E2F, 0x0E45, 0x0E4D, 0x0E26, 0x0E24, 0x0037, 0x0E40, 0x0E41},
    /*8*/ {7, 0x0E27, 0x0E28, 0x0E29, 0x0E2A, 0x0038, 0x0E42, 0x0E46},
    /*9*/ {7, 0x0E2B, 0x0E2C, 0x0E2D, 0x0E2E, 0x0039, 0x0E43, 0x0E44},
    /*0*/ {2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else   //367,360
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai = 
{
    /*1*/ {7, 0x0E01, 0x0E02, 0x0E04, 0x0E30, 0x0E32, 0x0E33, 0x0031},
    /*2*/ {7, 0x0E06, 0x0E07, 0x0E08, 0x0E09, 0x0E40, 0x0E41, 0x0032},
    /*3*/ {8, 0x0E0A, 0x0E0B, 0x0E0C, 0x0E0D, 0x0E42, 0x0E43, 0x0E44, 0x0033},
    /*4*/ {8, 0x0E0E, 0x0E0F, 0x0E10, 0x0E11, 0x0E12, 0x0E13, 0x0E31, 0x0034},
    /*5*/ {8, 0x0E14, 0x0E15, 0x0E16, 0x0E17, 0x0E18, 0x0E34, 0x0E35, 0x0035},
    /*6*/ {8, 0x0E19, 0x0E1A, 0x0E1B, 0x0E1C, 0x0E1D, 0x0E36, 0x0E37, 0x0036},
    /*7*/ {8, 0x0E1E, 0x0E1F, 0x0E20, 0x0E21, 0x0E22, 0x0E38, 0x0E39, 0x0037},
    /*8*/ {8, 0x0E23, 0x0E24, 0x0E25, 0x0E27, 0x0E28, 0x0E29, 0x0E47, 0x0038},
    /*9*/ {8, 0x0E2A, 0x0E2B, 0x0E2C, 0x0E2D, 0x0E2E, 0x0E2F, 0x0E46, 0x0039},
    /*0*/ {6, 0x0E48, 0x0E49, 0x0E4A, 0x0E4B, 0x0E4C, 0x0030},
    /***/{0},
    /*#*/{0}
};

#endif      //INPUT_METHOD_CSTAR_COMMON_MINI

#else       //非INPUT_METHOD_CSTAR_COMMON_MINI mini版

#if defined(SPECIAL_THAI)  //special   368, 364
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai = 
{
    /*1*/ {11, 0x0E01, 0x0E02, 0x0E03, 0x0E04, 0x0E05, 0x0E06, 0x0E07, 0x0E08, 0x0031, 0x0E30, 0x0E31},
    /*2*/ {8, 0x0E09, 0x0E0A, 0x0E0B, 0x0E0C, 0x0E0D, 0x0032, 0x0E32, 0x0E33},
    /*3*/ {9, 0x0E0E, 0x0E0F, 0x0E10, 0x0E11, 0x0E12, 0x0E13, 0x0033, 0x0E34, 0x0E35},
    /*4*/ {9, 0x0E14, 0x0E15, 0x0E16, 0x0E17, 0x0E18, 0x0E19, 0x0034, 0x0E36, 0x0E37},
    /*5*/ {7, 0x0E48, 0x0E49, 0x0E4A, 0x0E4B, 0x0E4C, 0x0E47, 0x0035},
    /*6*/ {9, 0x0E1A, 0x0E1B, 0x0E1C, 0x0E1D, 0x0E1E, 0x0E1F, 0x0036, 0x0E38, 0x0E39},
    /*7*/ {13, 0x0E20, 0x0E21, 0x0E22, 0x0E23, 0x0E25, 0x0E2F, 0x0E45, 0x0E4D, 0x0E26, 0x0E24, 0x0037, 0x0E40, 0x0E41},
    /*8*/ {7, 0x0E27, 0x0E28, 0x0E29, 0x0E2A, 0x0038, 0x0E42, 0x0E46},
    /*9*/ {7, 0x0E2B, 0x0E2C, 0x0E2D, 0x0E2E, 0x0039, 0x0E43, 0x0E44},
    /*0*/ {2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else   //367,360
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai = 
{
    /*1*/ {7, 0x0E01, 0x0E02, 0x0E04, 0x0E30, 0x0E32, 0x0E33, 0x0031},
    /*2*/ {7, 0x0E06, 0x0E07, 0x0E08, 0x0E09, 0x0E40, 0x0E41, 0x0032},
    /*3*/ {8, 0x0E0A, 0x0E0B, 0x0E0C, 0x0E0D, 0x0E42, 0x0E43, 0x0E44, 0x0033},
    /*4*/ {8, 0x0E0E, 0x0E0F, 0x0E10, 0x0E11, 0x0E12, 0x0E13, 0x0E31, 0x0034},
    /*5*/ {8, 0x0E14, 0x0E15, 0x0E16, 0x0E17, 0x0E18, 0x0E34, 0x0E35, 0x0035},
    /*6*/ {8, 0x0E19, 0x0E1A, 0x0E1B, 0x0E1C, 0x0E1D, 0x0E36, 0x0E37, 0x0036},
    /*7*/ {8, 0x0E1E, 0x0E1F, 0x0E20, 0x0E21, 0x0E22, 0x0E38, 0x0E39, 0x0037},
    /*8*/ {8, 0x0E23, 0x0E24, 0x0E25, 0x0E27, 0x0E28, 0x0E29, 0x0E47, 0x0038},
    /*9*/ {8, 0x0E2A, 0x0E2B, 0x0E2C, 0x0E2D, 0x0E2E, 0x0E2F, 0x0E46, 0x0039},
    /*0*/ {6, 0x0E48, 0x0E49, 0x0E4A, 0x0E4B, 0x0E4C, 0x0030},
    /***/{0},
    /*#*/{0}
};

#endif      //INPUT_METHOD_CSTAR_COMMON_MINI

#endif      //cstar end

#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai =
{
    /*1*/  {12, 0x0E01, 0x0E02, 0x0E03, 0x0E04, 0x0E05, 0x0E06, 0x0E07, 0x0E08, 0x0E30, 0x0E31, 0x0E51, 0x0031},
    /*2*/  {9, 0x0E09, 0x0E0A, 0x0E0B, 0x0E0C, 0x0E0D, 0x0E32, 0x0E33, 0x0E52, 0x0032},
    /*3*/  {10, 0x0E0E, 0x0E0F, 0x0E10, 0x0E11, 0x0E12, 0x0E13, 0x0E34, 0x0E35, 0x0E53, 0x0033},
    /*4*/  {10, 0x0E14, 0x0E15, 0x0E16, 0x0E17, 0x0E18, 0x0E19, 0x0E36, 0x0E37, 0x0E54, 0x0034},
    /*5*/  {11, 0x0E48, 0x0E49, 0x0E4A, 0x0E4B, 0x0E4C, 0x0E47, 0x0E3A, 0x0E4D, 0x0E4E, 0x0E55, 0x0035},
    /*6*/  {10, 0x0E1A, 0x0E1B, 0x0E1C, 0x0E1D, 0x0E1E, 0x0E1F, 0x0E38, 0x0E39, 0x0E56, 0x0036},
    /*7*/  {13, 0x0E20, 0x0E21, 0x0E22, 0x0E23, 0x0E25, 0x0E2F, 0x0E45, 0x0E26, 0x0E24, 0x0E40, 0x0E41, 0x0E57, 0x0037},
    /*8*/  {8, 0x0E27, 0x0E28, 0x0E29, 0x0E2A, 0x0E42, 0x0E46, 0x0E58, 0x0038},
    /*9*/  {8, 0x0E2B, 0x0E2C, 0x0E2D, 0x0E2E, 0x0E43, 0x0E44, 0x0E59, 0x0039},
    /*0*/  {2, 0x0E50, 0x0030},
};
#else
//thai,5 tables
//<--thai begin
/* the begining key mapping only leading vowel, sign, consonant are allowed */
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai_normal =
{
    /*1*/{ 8, THAI_D_ONE, THAI_C_KO_KAI, THAI_C_KHO_KHAI, THAI_C_KHO_KHWAI, THAI_C_KHO_RAKHANG, THAI_C_NGO_NGU, THAI_C_CHO_CHAN, 0x0031},
    /*2*/{ 7, THAI_C_CHO_CHING, THAI_C_CHO_CHANG, THAI_C_SO_SO, THAI_C_CHO_CHOE, THAI_C_YO_YING, THAI_D_TWO, 0x0032},
    /*3*/{ 8, THAI_C_DO_CHADA, THAI_C_TO_PATAK, THAI_C_THO_THAN, THAI_C_THO_NANGMONTHO, THAI_C_THO_PHUTHAO, THAI_C_NO_NEN, THAI_D_THREE, 0x0033},
    /*4*/{ 8, THAI_C_DO_DEK, THAI_C_TO_TAO, THAI_C_THO_THUNG, THAI_C_THO_THAHAN, THAI_C_THO_THONG, THAI_C_NO_NU, THAI_D_FOUR, 0x0034},
    /*5*/{ 2, THAI_D_FIVE, 0x0035},
    /*6*/{ 8, THAI_C_BO_BAIMAI, THAI_C_PO_PLA, THAI_C_PHO_PHUNG, THAI_C_FO_FA, THAI_C_PHO_PHAN, THAI_C_FO_FAN, THAI_D_SIX, 0x0036},
    /*7*/{10, THAI_C_PHO_SAMPHAO, THAI_C_MO_MA, THAI_C_YO_YAK, THAI_C_RO_RUA, THAI_C_RU, THAI_C_LO_LING, THAI_C_SARA_E, THAI_C_SARA_AE, THAI_D_SEVEN, 0x0037},
    /*8*/{ 7, THAI_C_WO_WAEN, THAI_C_SO_SALA, THAI_C_SO_RUSI, THAI_C_SO_SUA, THAI_C_SARA_O, THAI_D_EIGHT, 0x0038},
    /*9*/{ 8, THAI_C_HO_HIP, THAI_C_LO_CHULA, THAI_C_O_ANG, THAI_C_HO_NOKHUK, THAI_C_SARA_AI_MAIMUAN, THAI_C_SARA_AI_MAIMALAI, THAI_D_NINE, 0x0039},
    /*0*/{ 3, THAI_D_ZERO, ' ', 0x0030},
    /***/{0},
    /*#*/{0}
};

/* after a consonant is input all are allowed */
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai_con =
{
    /*1*/{10, THAI_D_ONE, THAI_C_KO_KAI, THAI_C_KHO_KHAI, THAI_C_KHO_KHWAI, THAI_C_KHO_RAKHANG, THAI_C_NGO_NGU, THAI_C_CHO_CHAN, THAI_C_SARA_A, THAI_C_MAI_HAN_AKAT, 0x0031},
    /*2*/{10, THAI_C_CHO_CHING, THAI_C_CHO_CHANG, THAI_C_SO_SO, THAI_C_CHO_CHOE, THAI_C_YO_YING, THAI_C_SARA_AA, THAI_C_SARA_AM, THAI_C_LAKKHANGYAO, THAI_D_TWO, 0x0032},
    /*3*/{10, THAI_C_DO_CHADA, THAI_C_TO_PATAK, THAI_C_THO_THAN, THAI_C_THO_NANGMONTHO, THAI_C_THO_PHUTHAO, THAI_C_NO_NEN, THAI_C_SARA_I, THAI_C_SARA_II, THAI_D_THREE, 0x0033},
    /*4*/{10, THAI_C_DO_DEK, THAI_C_TO_TAO, THAI_C_THO_THUNG, THAI_C_THO_THAHAN, THAI_C_THO_THONG, THAI_C_NO_NU, THAI_C_SARA_UE, THAI_C_SARA_UEE, THAI_D_FOUR, 0x0034},
    /*5*/{ 8, THAI_C_MAI_EK, THAI_C_MAI_THO, THAI_C_MAI_TRI, THAI_C_MAI_CHATTAWA, THAI_C_MAITAIKHU, THAI_C_THANTHAKHAT, THAI_D_FIVE, 0x0035},
    /*6*/{10, THAI_C_BO_BAIMAI, THAI_C_PO_PLA, THAI_C_PHO_PHUNG, THAI_C_FO_FA, THAI_C_PHO_PHAN, THAI_C_FO_FAN, THAI_C_SARA_U, THAI_C_SARA_UU, THAI_D_SIX, 0x0036},
    /*7*/{10, THAI_C_PHO_SAMPHAO, THAI_C_MO_MA, THAI_C_YO_YAK, THAI_C_RO_RUA, THAI_C_RU, THAI_C_LO_LING, THAI_C_SARA_E, THAI_C_SARA_AE, THAI_D_SEVEN, 0x0037},
    /*8*/{10, THAI_C_WO_WAEN, THAI_C_SO_SALA, THAI_C_SO_RUSI, THAI_C_SO_SUA, THAI_C_SARA_O, THAI_C_MAIYAMOK, THAI_D_EIGHT, 0x0038},
    /*9*/{ 8, THAI_C_HO_HIP, THAI_C_LO_CHULA, THAI_C_O_ANG, THAI_C_HO_NOKHUK, THAI_C_SARA_AI_MAIMUAN, THAI_C_SARA_AI_MAIMALAI, THAI_D_NINE, 0x0039},
    /*0*/{ 3, THAI_D_ZERO, ' ', 0x0030},
    /***/{0},
    /*#*/{0}
};
/* after tone mark no other tone mark, above and below vowel */
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai_tone =
{
    /*1*/{ 9, THAI_D_ONE, THAI_C_KO_KAI, THAI_C_KHO_KHAI, THAI_C_KHO_KHWAI, THAI_C_KHO_RAKHANG, THAI_C_NGO_NGU, THAI_C_CHO_CHAN, THAI_C_SARA_A, 0x0031},
    /*2*/{10, THAI_C_CHO_CHING, THAI_C_CHO_CHANG, THAI_C_SO_SO, THAI_C_CHO_CHOE, THAI_C_YO_YING, THAI_C_SARA_AA, THAI_C_SARA_AM, THAI_C_LAKKHANGYAO, THAI_D_TWO, 0x0032},
    /*3*/{ 8, THAI_C_DO_CHADA, THAI_C_TO_PATAK, THAI_C_THO_THAN, THAI_C_THO_NANGMONTHO, THAI_C_THO_PHUTHAO, THAI_C_NO_NEN, THAI_D_THREE, 0x0033},
    /*4*/{ 8, THAI_C_DO_DEK, THAI_C_TO_TAO, THAI_C_THO_THUNG, THAI_C_THO_THAHAN, THAI_C_THO_THONG, THAI_C_NO_NU, THAI_D_FOUR, 0x0034},
    /*5*/{ 3,  THAI_C_THANTHAKHAT, THAI_D_FIVE, 0x0035},
    /*6*/{ 8, THAI_C_BO_BAIMAI, THAI_C_PO_PLA, THAI_C_PHO_PHUNG, THAI_C_FO_FA, THAI_C_PHO_PHAN, THAI_C_FO_FAN, THAI_D_SIX, 0x0036},
    /*7*/{10, THAI_C_PHO_SAMPHAO, THAI_C_MO_MA, THAI_C_YO_YAK, THAI_C_RO_RUA, THAI_C_RU, THAI_C_LO_LING, THAI_C_SARA_E, THAI_C_SARA_AE, THAI_D_SEVEN, 0x0037},
    /*8*/{ 8, THAI_C_WO_WAEN, THAI_C_SO_SALA, THAI_C_SO_RUSI, THAI_C_SO_SUA, THAI_C_SARA_O, THAI_C_MAIYAMOK, THAI_D_EIGHT, 0x0038},
    /*9*/{ 8, THAI_C_HO_HIP, THAI_C_LO_CHULA, THAI_C_O_ANG, THAI_C_HO_NOKHUK, THAI_C_SARA_AI_MAIMUAN, THAI_C_SARA_AI_MAIMALAI, THAI_D_NINE, 0x0039},
    /*0*/{ 3, THAI_D_ZERO, ' ', 0x0030},
    /***/{0},
    /*#*/{0}
};
/* After Above and Below vowel no above and below vowel is allowed */
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai_vowel =
{
    /*1*/{10, THAI_D_ONE, THAI_C_KO_KAI, THAI_C_KHO_KHAI, THAI_C_KHO_KHWAI, THAI_C_KHO_RAKHANG, THAI_C_NGO_NGU, THAI_C_CHO_CHAN, THAI_C_SARA_A, THAI_C_MAI_HAN_AKAT, 0x0031},
    /*2*/{10, THAI_C_CHO_CHING, THAI_C_CHO_CHANG, THAI_C_SO_SO, THAI_C_CHO_CHOE, THAI_C_YO_YING, THAI_C_SARA_AA, THAI_C_SARA_AM, THAI_C_LAKKHANGYAO, THAI_D_TWO, 0x0032},
    /*3*/{10, THAI_C_DO_CHADA, THAI_C_TO_PATAK, THAI_C_THO_THAN, THAI_C_THO_NANGMONTHO, THAI_C_THO_PHUTHAO, THAI_C_NO_NEN, THAI_C_SARA_I, THAI_C_SARA_II, THAI_D_THREE, 0x0033},
    /*4*/{10, THAI_C_DO_DEK, THAI_C_TO_TAO, THAI_C_THO_THUNG, THAI_C_THO_THAHAN, THAI_C_THO_THONG, THAI_C_NO_NU, THAI_C_SARA_UE, THAI_C_SARA_UEE, THAI_D_FOUR, 0x0034},
    /*5*/{ 8, THAI_C_MAI_EK, THAI_C_MAI_THO, THAI_C_MAI_TRI, THAI_C_MAI_CHATTAWA, THAI_C_MAITAIKHU, THAI_C_THANTHAKHAT, THAI_D_FIVE, 0x0035},
    /*6*/{10, THAI_C_BO_BAIMAI, THAI_C_PO_PLA, THAI_C_PHO_PHUNG, THAI_C_FO_FA, THAI_C_PHO_PHAN, THAI_C_FO_FAN, THAI_C_SARA_U, THAI_C_SARA_UU, THAI_D_SIX, 0x0036},
    /*7*/{10, THAI_C_PHO_SAMPHAO, THAI_C_MO_MA, THAI_C_YO_YAK, THAI_C_RO_RUA, THAI_C_RU, THAI_C_LO_LING, THAI_C_SARA_E, THAI_C_SARA_AE, THAI_D_SEVEN, 0x0037},
    /*8*/{ 8, THAI_C_WO_WAEN, THAI_C_SO_SALA, THAI_C_SO_RUSI, THAI_C_SO_SUA, THAI_C_SARA_O, THAI_C_MAIYAMOK, THAI_D_EIGHT, 0x0038},
    /*9*/{ 8, THAI_C_HO_HIP, THAI_C_LO_CHULA, THAI_C_O_ANG, THAI_C_HO_NOKHUK, THAI_C_SARA_AI_MAIMUAN, THAI_C_SARA_AI_MAIMALAI, THAI_D_NINE, 0x0039},
    /*0*/{ 3, THAI_D_ZERO, ' ', 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_thai_sara_aa =
{
    /*1*/{ 9, THAI_D_ONE, THAI_C_KO_KAI, THAI_C_KHO_KHAI, THAI_C_KHO_KHWAI, THAI_C_KHO_RAKHANG, THAI_C_NGO_NGU, THAI_C_CHO_CHAN, THAI_C_SARA_A, 0x0031},
    /*2*/{ 7, THAI_C_CHO_CHING, THAI_C_CHO_CHANG, THAI_C_SO_SO, THAI_C_CHO_CHOE, THAI_C_YO_YING, THAI_D_TWO, 0x0032},
    /*3*/{ 8, THAI_C_DO_CHADA, THAI_C_TO_PATAK, THAI_C_THO_THAN, THAI_C_THO_NANGMONTHO, THAI_C_THO_PHUTHAO, THAI_C_NO_NEN, THAI_D_THREE, 0x0033},
    /*4*/{ 8, THAI_C_DO_DEK, THAI_C_TO_TAO, THAI_C_THO_THUNG, THAI_C_THO_THAHAN, THAI_C_THO_THONG, THAI_C_NO_NU, THAI_D_FOUR, 0x0034},
    /*5*/{ 2, THAI_D_FIVE, 0x0035},
    /*6*/{ 8, THAI_C_BO_BAIMAI, THAI_C_PO_PLA, THAI_C_PHO_PHUNG, THAI_C_FO_FA, THAI_C_PHO_PHAN, THAI_C_FO_FAN, THAI_D_SIX, 0x0036},
    /*7*/{10, THAI_C_PHO_SAMPHAO, THAI_C_MO_MA, THAI_C_YO_YAK, THAI_C_RO_RUA, THAI_C_RU, THAI_C_LO_LING, THAI_C_SARA_E, THAI_C_SARA_AE, THAI_D_SEVEN, 0x0037},
    /*8*/{ 7, THAI_C_WO_WAEN, THAI_C_SO_SALA, THAI_C_SO_RUSI, THAI_C_SO_SUA, THAI_C_SARA_O, THAI_D_EIGHT, 0x0038},
    /*9*/{ 8, THAI_C_HO_HIP, THAI_C_LO_CHULA, THAI_C_O_ANG, THAI_C_HO_NOKHUK, THAI_C_SARA_AI_MAIMUAN, THAI_C_SARA_AI_MAIMALAI, THAI_D_NINE, 0x0039},
    /*0*/{ 3, THAI_D_ZERO, ' ', 0x0030},
    /***/{0},
    /*#*/{0}

};
#endif
//thai end -->
#endif

// 土耳其语
#ifdef IM_TURKISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_turkish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C7, 0x00C2},
    /*3*/{ 4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/{ 6, 0x0047, 0x0048, 0x0130, 0x0034, 0x011E, 0x0049},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 5, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D6},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x015E},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC, 0x00DB},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_turkish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E7, 0x00E2},
    /*3*/{ 4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/{ 6, 0x0067, 0x0068, 0x0069, 0x0034, 0x011F, 0x0131},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 5, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F6},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x015F},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC, 0x00FB},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_turkish_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0041, 0x0042, 0x0043, 0x00C7, 0x0032},
    /*3*/  {4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/  {6, 0x0047, 0x0048, 0x0130, 0x011E, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D6, 0x0036},
    /*7*/  {5, 0x0050, 0x0052, 0x0053, 0x015E, 0x0037},
    /*8*/  {5, 0x0054, 0x0055, 0x0056, 0x00DC, 0x0038},
    /*9*/  {3, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_turkish_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0061, 0x0062, 0x0063, 0x00E7, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {6, 0x0067, 0x0068, 0x0069, 0x011F, 0x0131, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F6, 0x0036},
    /*7*/  {5, 0x0070, 0x0072, 0x0073, 0x015F, 0x0037},
    /*8*/  {5, 0x0074, 0x0075, 0x0076, 0x00FC, 0x0038},
    /*9*/  {3, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_turkish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x00C7, 0x00C2},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 6, 'G', 'H', 'I', '4', 0x011E, 0x0130},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', '6', 0x00D6},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', '7', 0x015E},
    /*8*/{ 6, 'T', 'U', 'V', '8', 0x00DC, 0x00DB},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_turkish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x00E7, 0x00E2},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 6, 'g', 'h', 'i', '4', 0x011F, 0x0131},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', '6', 0x00F6},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x015F},
    /*8*/{ 6, 't', 'u', 'v', '8', 0x00FC, 0x00FB},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 乌尔都语
#ifdef IM_URDU_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)     //mini版本，使用957
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_urdu =
{
    /*1*/{29, 0x002E, 0x060C, 0x061F, 0x0021, 0x003A, 0x061B, 0x0031, 0x0040, 0x002F, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x0025, 0x003D, 0x002B, 0x002D, 0x0023, 0x002A, 0x003C, 0x003E, 0x005B, 0x005D, 0x007B, 0x007D, 0x0026, 0x005C, 0x005F, 0x007C},
    /*2*/{ 7, 0x0628, 0x067E, 0x062A, 0x0679, 0x062B, 0x06C3, 0x0032},
    /*3*/{ 7, 0x0627, 0x0622, 0x0624, 0x0626, 0x06D3, 0x0621, 0x0033},
    /*4*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0034},
    /*5*/{ 8, 0x062F, 0x0688, 0x0630, 0x0631, 0x0691, 0x0632, 0x0698, 0x0035},
    /*6*/{ 5, 0x062C, 0x0686, 0x062D, 0x062E, 0x0036},
    /*7*/{ 9, 0x0646, 0x06BA, 0x0648, 0x06BE, 0x06C1, 0x06C2, 0x06CC, 0x06D2, 0x0037},
    /*8*/{ 7, 0x0641, 0x0642, 0x06A9, 0x06AF, 0x0644, 0x0645, 0x0038},
    /*9*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0039},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_urdu =     //950与957相同
{
    /*1*/{29, 0x002E, 0x060C, 0x061F, 0x0021, 0x003A, 0x061B, 0x0031, 0x0040, 0x002F, 0x0028, 0x0029, 0x00AB, 0x00BB, 0x0025, 0x003D, 0x002B, 0x002D, 0x0023, 0x002A, 0x003C, 0x003E, 0x005B, 0x005D, 0x007B, 0x007D, 0x0026, 0x005C, 0x005F, 0x007C},
    /*2*/{ 7, 0x0628, 0x067E, 0x062A, 0x0679, 0x062B, 0x06C3, 0x0032},
    /*3*/{ 7, 0x0627, 0x0622, 0x0624, 0x0626, 0x06D3, 0x0621, 0x0033},
    /*4*/{ 5, 0x0633, 0x0634, 0x0635, 0x0636, 0x0034},
    /*5*/{ 8, 0x062F, 0x0688, 0x0630, 0x0631, 0x0691, 0x0632, 0x0698, 0x0035},
    /*6*/{ 5, 0x062C, 0x0686, 0x062D, 0x062E, 0x0036},
    /*7*/{ 9, 0x0646, 0x06BA, 0x0648, 0x06BE, 0x06C1, 0x06C2, 0x06CC, 0x06D2, 0x0037},
    /*8*/{ 7, 0x0641, 0x0642, 0x06A9, 0x06AF, 0x0644, 0x0645, 0x0038},
    /*9*/{ 5, 0x0637, 0x0638, 0x0639, 0x063A, 0x0039},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#endif
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_urdu =     //950与957相同
{
    /*1*/{3, 0x002E, '-', 0x007C},
    /*2*/{ 3, 0x0628, '-', 0x06C3},
    /*3*/{ 3, 0x0627, '-', 0x0621},
    /*4*/{ 3, 0x0633, '-', 0x0636},
    /*5*/{ 3, 0x062F, '-', 0x0698},
    /*6*/{ 3, 0x062C, '-', 0x062E},
    /*7*/{ 3, 0x0646, '-', 0x06D2},
    /*8*/{ 3, 0x0641, '-', 0x0645},
    /*9*/{ 3, 0x0637, '-', 0x063A},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#endif
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_urdu =
{
    /*1*/  {2, 0x0661, 0x0031},
    /*2*/  {8, 0x0628, 0x067e, 0x062a, 0x0679, 0x062b, 0x06c3, 0x0662, 0x0032},
    /*3*/  {9, 0x0627, 0x0622, 0x0623, 0x0624, 0x0626, 0x06d3, 0x0621, 0x0663, 0x0033},
    /*4*/  {6, 0x0633, 0x0634, 0x0635, 0x0636, 0x0664, 0x0034},
    /*5*/  {9, 0x062f, 0x0688, 0x0630, 0x0631, 0x0691, 0x0632, 0x0698, 0x0665, 0x0035},
    /*6*/  {6, 0x062c, 0x0686, 0x062d, 0x062e, 0x0666, 0x0036},
    /*7*/  {10, 0x0646, 0x06ba, 0x0648, 0x06be, 0x06c1, 0x06c2, 0x06cc, 0x06d2, 0x0667, 0x0037},
    /*8*/  {8, 0x0641, 0x0642, 0x06a9, 0x06af, 0x0644, 0x0645, 0x0668, 0x0038},
    /*9*/  {6, 0x0637, 0x0638, 0x0639, 0x063a, 0x0669, 0x0039},
    /*0*/  {2, 0x0660, 0x0030},
};
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_urdu =     //950与957相同
{
    /*1*/{3, 0x002E, '-', 0x007C},
    /*2*/{ 3, 0x0628, '-', 0x06C3},
    /*3*/{ 3, 0x0627, '-', 0x0621},
    /*4*/{ 3, 0x0633, '-', 0x0636},
    /*5*/{ 3, 0x062F, '-', 0x0698},
    /*6*/{ 3, 0x062C, '-', 0x062E},
    /*7*/{ 3, 0x0646, '-', 0x06D2},
    /*8*/{ 3, 0x0641, '-', 0x0645},
    /*9*/{ 3, 0x0637, '-', 0x063A},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{ 0},
    /*#*/{0}
};
#endif
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_urdu =
{
    /*1*/{14, 0x002E, 0x061F, 0x060C, 0x0021, 0x061B, 0x0022, 0x0040, 0x003A, 0x0031, 0x003B, 0x0028, 0x0029, 0x002A, 0x002D},
    /*2*/{ 8, 0x062C, 0x0686, 0x062D, 0x062E, 0x062F, 0x0688, 0x0630, 0x0032},
    /*3*/{ 8, 0x0627, 0x0622, 0x0628, 0x067E, 0x062A, 0x0679, 0x062B, 0x0033},
    /*4*/{ 6, 0x063A, 0x0641, 0x0642, 0x06A9, 0x06AF, 0x0034},
    /*5*/{ 5, 0x0636, 0x0637, 0x0638, 0x0639, 0x0035},
    /*6*/{ 8, 0x0631, 0x0691, 0x0632, 0x0698, 0x0633, 0x0634, 0x0635, 0x0036},
    /*7*/{ 6, 0x0621, 0x06CC, 0x0626, 0x06D2, 0x06D3, 0x0037},
    /*8*/{ 5, 0x06C2, 0x06C1, 0x06BE, 0x06C3, 0x0038},
    /*9*/{ 7, 0x0644, 0x0645, 0x0646, 0x06BA, 0x0648, 0x0624, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_urdu =     //950与957相同
{
    /*1*/{3, 0x002E, '-', 0x007C},
    /*2*/{ 3, 0x0628, '-', 0x06C3},
    /*3*/{ 3, 0x0627, '-', 0x0621},
    /*4*/{ 3, 0x0633, '-', 0x0636},
    /*5*/{ 3, 0x062F, '-', 0x0698},
    /*6*/{ 3, 0x062C, '-', 0x062E},
    /*7*/{ 3, 0x0646, '-', 0x06D2},
    /*8*/{ 3, 0x0641, '-', 0x0645},
    /*9*/{ 3, 0x0637, '-', 0x063A},
    /*0*/{ 2, 0x0020, 0x0030},
    /***/{0},
    /*#*/{0}
};
#endif
#endif
#endif

// 越南语
#ifdef IM_VIETNAMESE_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)     //mini版本，使用317
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_upper =
{
    /*1*/{26, 0x002E, 0x002C, 0x003F, 0x0021, 0x0031, 0x003A, 0x003B, 0x0027, 0x002D, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0028, 0x0029, 0x003C, 0x003E, 0x0022, 0x20AB, 0x0024, 0x00A5, 0x002B, 0x003D, 0x0023, 0x002A},
    /*2*/{ 5, 0x0041, 0x0102, 0x00C2, 0x0042, 0x0043, 0x0032},
    /*3*/{ 5, 0x0044, 0x0110, 0x0045, 0x00CA, 0x0046, 0x0033},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 6, 0x004D, 0x004E, 0x004F, 0x00D4, 0x01A0, 0x0036},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 5, 0x0054, 0x0055, 0x01AF, 0x0056, 0x0038},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_lower =
{
    /*1*/{26, 0x002E, 0x002C, 0x003F, 0x0021, 0x0031, 0x003A, 0x003B, 0x0027, 0x002D, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0028, 0x0029, 0x003C, 0x003E, 0x0022, 0x20AB, 0x0024, 0x00A5, 0x002B, 0x003D, 0x0023, 0x002A},
    /*2*/{ 5, 0x0061, 0x0103, 0x00E2, 0x0062, 0x0063, 0x0032},
    /*3*/{ 5, 0x0064, 0x0111, 0x0065, 0x00EA, 0x0066, 0x0033},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 6, 0x006D, 0x006E, 0x006F, 0x00F4, 0x01A1, 0x0036},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 5, 0x0074, 0x0075, 0x01B0, 0x0076, 0x0038},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_abc_upper =		//cstar的按键码并没有将所有的按键字符给写出来，而multi-tap输入法必须将所有字符写出来
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 21, 0x0041, 0x0102, 0x00C0,0x00C1,0x001EA2,0x00C2,0x00C3,0x1EA0, 0x1EB0,0x1EAE,0x1EB2,0x1EB4,0x1EB6,0X1EA6,0x1EA4,0x1EA8,0x1EAA, 0x1EAC,0x0042, 0x0043, 0x0032},//
    /*3*/{ 16, 0x0044, 0x0110, 0x0045, 0x00C8,0x00C9,0x1EBA,0x1EBC,0x1EB8, 0x00CA,0x1EC0,0x1EBE, 0x1EC2,0x1EC4,0x1EC6,0x0046, 0x0033},
    /*4*/{ 9, 0x0047, 0x0048, 0x0049, 0x00CC,0x00CD,0x1EC8,0x0128,0x1ECA,0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 21, 0x004D, 0x004E, 0x004F,0x00D2,0x00D3,0x1ECE,0x00D5,0x1ECC, 0x00D4, 0x1ED2,0x1ED0,0x1ED4,0x1ED6,0x1ED8, 0x01A0,0x1EDC,0x1EDA,0x1EDE,0x1EE0,0x1EE0, 0x0036},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 15, 0x0054, 0x0055, 0x01AF, 0x00D9,0x00DA,0x1EE6,0x0168,0x1EE4, 0x1EEA,0x1EE8,0x1EEC,0x1EEE,0x1EF0,0x0056, 0x0038},
    /*9*/{ 10, 0x0057, 0x0058, 0x0059,0x1EF2,0x00DD,0x1EF6,0x1EF8,0x1EF4, 0x005A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_abc_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 21, 0x0061, 0x0103,0x00E0,0x00E1, 0x1EA3, 0x00E2, 0x00E3,0x1EA1, 0x1EB1,0x1EAF,0x1EB3,0x1EB5,0x1EB7, 0x1EA7,0x1EA5,0x1EA9,0x1EAB,0x1EAD,0x0062, 0x0063, 0x0032}, //  
    /*3*/{ 16, 0x0064, 0x0111, 0x0065,0x00E8,0x00E9,0x1EBB,0x1EBD,0x1EB9, 0x00EA, 0x1EC1,0x1EBF,0x1EC3,0x1EC5,0x1EC7, 0x0066, 0x0033},
    /*4*/{ 9, 0x0067, 0x0068, 0x0069,0x00EC,0x00ED,0x1EC9,0x0129,0x1CB, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 21, 0x006D, 0x006E, 0x006F,0x00F2,0x00F3,0x1ECF,0x00F5,0x1ECD, 0x00F4,0x1ED3,0x1ED1,0x1ED5,0x1ED7,0x1ED9, 0x01A1,0x1EDD,0x1EDB,0x1EDF,0x1EE1,0x1EE3, 0x0036},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 15, 0x0074, 0x0075, 0x01B0, 0x00F9,0x00FA,0x1EE7,0x0169,0x1EE5,0x1EEB,0x1EE9,0x1EED,0x1EEF,0x1EF1,0x0076, 0x0038},
    /*9*/{ 10, 0x0077, 0x0078, 0x0079,0x1EF3,0x00FD,0x1EF7,0x1EF9,0x1EF5, 0x007A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

#else   //310与317相同
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_upper =
{
    /*1*/{26, 0x002E, 0x002C, 0x003F, 0x0021, 0x0031, 0x003A, 0x003B, 0x0027, 0x002D, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0028, 0x0029, 0x003C, 0x003E, 0x0022, 0x20AB, 0x0024, 0x00A5, 0x002B, 0x003D, 0x0023, 0x002A},
    /*2*/{ 5, 0x0041, 0x0102, 0x00C2, 0x0042, 0x0043, 0x0032},
    /*3*/{ 5, 0x0044, 0x0110, 0x0045, 0x00CA, 0x0046, 0x0033},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 6, 0x004D, 0x004E, 0x004F, 0x00D4, 0x01A0, 0x0036},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 5, 0x0054, 0x0055, 0x01AF, 0x0056, 0x0038},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_lower =
{
    /*1*/{26, 0x002E, 0x002C, 0x003F, 0x0021, 0x0031, 0x003A, 0x003B, 0x0027, 0x002D, 0x005C, 0x0040, 0x002F, 0x0025, 0x0026, 0x0028, 0x0029, 0x003C, 0x003E, 0x0022, 0x20AB, 0x0024, 0x00A5, 0x002B, 0x003D, 0x0023, 0x002A},
    /*2*/{ 5, 0x0061, 0x0103, 0x00E2, 0x0062, 0x0063, 0x0032},
    /*3*/{ 5, 0x0064, 0x0111, 0x0065, 0x00EA, 0x0066, 0x0033},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 6, 0x006D, 0x006E, 0x006F, 0x00F4, 0x01A1, 0x0036},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 5, 0x0074, 0x0075, 0x01B0, 0x0076, 0x0038},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_abc_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 21, 0x0041, 0x0102, 0x00C0,0x00C1,0x001EA2,0x00C2,0x00C3,0x1EA0, 0x1EB0,0x1EAE,0x1EB2,0x1EB4,0x1EB6,0X1EA6,0x1EA4,0x1EA8,0x1EAA, 0x1EAC,0x0042, 0x0043, 0x0032},//
    /*3*/{ 16, 0x0044, 0x0110, 0x0045, 0x00C8,0x00C9,0x1EBA,0x1EBC,0x1EB8, 0x00CA,0x1EC0,0x1EBE, 0x1EC2,0x1EC4,0x1EC6,0x0046, 0x0033},
    /*4*/{ 9, 0x0047, 0x0048, 0x0049, 0x00CC,0x00CD,0x1EC8,0x0128,0x1ECA,0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 21, 0x004D, 0x004E, 0x004F,0x00D2,0x00D3,0x1ECE,0x00D5,0x1ECC, 0x00D4, 0x1ED2,0x1ED0,0x1ED4,0x1ED6,0x1ED8, 0x01A0,0x1EDC,0x1EDA,0x1EDE,0x1EE0,0x1EE0, 0x0036},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 15, 0x0054, 0x0055, 0x01AF, 0x00D9,0x00DA,0x1EE6,0x0168,0x1EE4, 0x1EEA,0x1EE8,0x1EEC,0x1EEE,0x1EF0,0x0056, 0x0038},
    /*9*/{ 10, 0x0057, 0x0058, 0x0059,0x1EF2,0x00DD,0x1EF6,0x1EF8,0x1EF4, 0x005A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_abc_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 21, 0x0061, 0x0103,0x00E0,0x00E1, 0x1EA3, 0x00E2, 0x00E3,0x1EA1, 0x1EB1,0x1EAF,0x1EB3,0x1EB5,0x1EB7, 0x1EA7,0x1EA5,0x1EA9,0x1EAB,0x1EAD,0x0062, 0x0063, 0x0032}, //  
    /*3*/{ 16, 0x0064, 0x0111, 0x0065,0x00E8,0x00E9,0x1EBB,0x1EBD,0x1EB9, 0x00EA, 0x1EC1,0x1EBF,0x1EC3,0x1EC5,0x1EC7, 0x0066, 0x0033},
    /*4*/{ 9, 0x0067, 0x0068, 0x0069,0x00EC,0x00ED,0x1EC9,0x0129,0x1CB, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 21, 0x006D, 0x006E, 0x006F,0x00F2,0x00F3,0x1ECF,0x00F5,0x1ECD, 0x00F4,0x1ED3,0x1ED1,0x1ED5,0x1ED7,0x1ED9, 0x01A1,0x1EDD,0x1EDB,0x1EDF,0x1EE1,0x1EE3, 0x0036},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 15, 0x0074, 0x0075, 0x01B0, 0x00F9,0x00FA,0x1EE7,0x0169,0x1EE5,0x1EEB,0x1EE9,0x1EED,0x1EEF,0x1EF1,0x0076, 0x0038},
    /*9*/{ 10, 0x0077, 0x0078, 0x0079,0x1EF3,0x00FD,0x1EF7,0x1EF9,0x1EF5, 0x007A, 0x0039},
    /*0*/{ 1, 0x0030},
    /***/{0},
    /*#*/{0}
};

#endif
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_lower =
{
    /*1*/  {6, 0x0300, 0x0301, 0x0303, 0x0309, 0x0323, 0x0031},
    /*2*/  {9, 0x0061, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x0103, 0x0062, 0x0063, 0x0032},
    /*3*/  {7, 0x0064, 0x0111, 0x0065, 0x00E8, 0x00E9, 0x00EA, 0x0033},
    /*4*/  {6, 0x0067, 0x0068, 0x0069, 0x00EC, 0x00ED, 0x0034},
    /*5*/  {3, 0x006B, 0x006C, 0x0035},
    /*6*/  {9, 0x006D, 0x006E, 0x006F, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x01A1, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {7, 0x0074, 0x0075, 0x00F9, 0x00FA, 0x01B0, 0x0076, 0x0038},
    /*9*/  {4, 0x0078, 0x0079, 0x00FD, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_upper =
{
    /*1*/  {6, 0x0300, 0x0301, 0x0303, 0x0309, 0x0323, 0x0031},
    /*2*/  {9, 0x0041, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x0102, 0x0042, 0x0043, 0x0032},
    /*3*/  {7, 0x0044, 0x0110, 0x0045, 0x00C8, 0x00C9, 0x00CA, 0x0033},
    /*4*/  {6, 0x0047, 0x0048, 0x0049, 0x00CC, 0x00CD, 0x0034},
    /*5*/  {3, 0x004B, 0x004C, 0x0035},
    /*6*/  {9, 0x004D, 0x004E, 0x004F, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x01A0, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {7, 0x0054, 0x0055, 0x00D9, 0x00DA, 0x01AF, 0x0056, 0x0038},
    /*9*/  {4, 0x0058, 0x0059, 0x00DD, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_upper =
{
    /*1*/{18, 0x0300, 0x0309, 0x0303, 0x0301, 0x0323, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x0102, 0x00C2},
    /*3*/{ 6, 'D', 'E', 'F', '3', 0x0110, 0x00CA},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 6, 'M', 'N', 'O', '6', 0x00D4, 0x01A0},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x01AF},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_vietnamese_lower =
{
    /*1*/{18, 0x0300, 0x0309, 0x0303, 0x0301, 0x0323, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x0103, 0x00E2},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x0111, 0x00EA},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 6, 'm', 'n', 'o', '6', 0x00F4, 0x01A1},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x01B0},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 希伯来语
#ifdef IM_HEBREW_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hebrew =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 4, 0x05D3, 0x05D4, 0x05D5, 0x0032},
    /*3*/{ 4, 0x05D0, 0x05D1, 0x05D2, 0x0033},
    /*4*/{ 5, 0x05DE, 0x05DD, 0x05E0, 0x05DF, 0x0034},
    /*5*/{ 5, 0x05D9, 0x05DB, 0x05DA, 0x05DC, 0x0035},
    /*6*/{ 4, 0x05D6, 0x05D7, 0x05D8, 0x0036},
    /*7*/{ 4, 0x05E8, 0x05E9, 0x05EA, 0x0037},
    /*8*/{ 4, 0x05E6, 0x05E5, 0x05E7, 0x0038},
    /*9*/{ 5, 0x05E1, 0x05E2, 0x05E4, 0x05E3, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{ 0},
    /*#*/{ 0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hebrew =
{
    /*1*/  {17, 0x05B0, 0x05B1, 0x05B2, 0x05B3, 0x05B4, 0x05B5, 0x05B6, 0x05B7, 0x05B8, 0x05B9, 0x05BB, 0x05BC, 0x05BD, 0x05BF, 0x05C1, 0x05C2, 0x0031},
    /*2*/  {4, 0x05D0, 0x05D1, 0x05D2, 0x0032},
    /*3*/  {4, 0x05D3, 0x05D4, 0x05D5, 0x0033},
    /*4*/  {4, 0x05D6, 0x05D7, 0x05D8, 0x0034},
    /*5*/  {4, 0x05D9, 0x05DA, 0x05DB, 0x0035},
    /*6*/  {4, 0x05DC, 0x05DD, 0x05DE, 0x0036},
    /*7*/  {5, 0x05DF, 0x05E0, 0x05E1, 0x05E2, 0x0037},
    /*8*/  {5, 0x05E3, 0x05E4, 0x05E5, 0x05E6, 0x0038},
    /*9*/  {5, 0x05E7, 0x05E8, 0x05E9, 0x05EA, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hebrew =
{
    /*1*/{17, 0x05b0, 0x05b1, 0x05b2, 0x05b3, 0x05b4, 0x05b5, 0x05b6, 0x05b7, 0x05b8, 0x05b9, 0x05bb, 0x05bc, 0x05bd, 0x05bf, 0x05c1, 0x05c2, '1'},
    /*2*/{ 4, 0x05d0, 0x05d1, 0x05d2, '2'},
    /*3*/{ 4, 0x05d3, 0x05d4, 0x05d5, '3'},
    /*4*/{ 4, 0x05d6, 0x05d7, 0x05d8, '4'},
    /*5*/{ 4, 0x05d9, 0x05da, 0x05db, '5'},
    /*6*/{ 4, 0x05dc, 0x05dd, 0x05de, '6'},
    /*7*/{ 5, 0x05df, 0x05e0, 0x05e1, 0x05e2, '7'},
    /*8*/{ 5, 0x05e3, 0x05e4, 0x05e5, 0x05e6, '8'},
    /*9*/{ 5, 0x05e7, 0x05e8, 0x05e9, 0x05ea, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 捷克语
#ifdef IM_CZECH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_czech_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 7, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E1, 0x010D, 0x00E4},
    /*3*/{ 7, 0x0064, 0x0065, 0x0066, 0x0033, 0x010F, 0x00E9, 0x011B},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00ED},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x0148, 0x00F3, 0x00F6},
    /*7*/{ 7, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0159, 0x0161},
    /*8*/{ 8, 0x0074, 0x0075, 0x0076, 0x0038, 0x0165, 0x00FA, 0x016F, 0x00FC},
    /*9*/{ 7, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x00FD, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_czech_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 7, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C1, 0x010C, 0x00C4},
    /*3*/{ 7, 0x0044, 0x0045, 0x0046, 0x0033, 0x010E, 0x00C9, 0x011A},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CD},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x0147, 0x00D3, 0x00D6},
    /*7*/{ 7, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0158, 0x0160},
    /*8*/{ 8, 0x0054, 0x0055, 0x0056, 0x0038, 0x0164, 0x00DA, 0x016E, 0x00DC},
    /*9*/{ 7, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x00DD, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_czech_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x00C1, 0x010C, 0x0032},
    /*3*/  {7, 0x0044, 0x0045, 0x0046, 0x00C9, 0x010E, 0x011A, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CD, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {6, 0x004D, 0x004E, 0x004F, 0x00D3, 0x0147, 0x0036},
    /*7*/  {7, 0x0050, 0x0051, 0x0052, 0x0053, 0x0158, 0x0160, 0x0037},
    /*8*/  {7, 0x0054, 0x0055, 0x0056, 0x00DA, 0x0164, 0x016E, 0x0038},
    /*9*/  {7, 0x0057, 0x0058, 0x0059, 0x005A, 0x00DD, 0x017D, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_czech_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x00E1, 0x010D, 0x0032},
    /*3*/  {7, 0x0064, 0x0065, 0x0066, 0x010F, 0x00E9, 0x011B, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00ED, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {6, 0x006D, 0x006E, 0x006F, 0x0148, 0x00F3, 0x0036},
    /*7*/  {7, 0x0070, 0x0071, 0x0072, 0x0073, 0x0159, 0x0161, 0x0037},
    /*8*/  {7, 0x0074, 0x0075, 0x0076, 0x0165, 0x00FA, 0x016F, 0x0038},
    /*9*/  {7, 0x0077, 0x0078, 0x0079, 0x007A, 0x00FD, 0x017E, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_czech_lower =
{
    /*1*/{13, 0x0021, 0x0024, 0x0027, 0x002b, 0x002C, 0x002D, 0x002E, '1', 0x003a, 0x003b, 0x003f, 0x0040, 0x20ac},
    /*2*/{ 6, 'a', 'b', 'c', 0x00E1, 0x010D, '2'},
    /*3*/{ 7, 'd', 'e', 'f', 0x00E9, 0x010F, 0x011B, '3'},
    /*4*/{ 5, 'g', 'h', 'i', 0x00ED, '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 6, 'm', 'n', 'o', 0x00F3, 0x0148, '6'},
    /*7*/{ 7, 'p', 'q', 'r', 's', 0x0159, 0x0161, '7'},
    /*8*/{ 7, 't', 'u', 'v', 0x00FA, 0x0165, 0x016F, '8'},
    /*9*/{ 7, 'w', 'x', 'y', 'z', 0x00FD, 0x017E, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_czech_upper =
{
    /*1*/{13, 0x0021, 0x0024, 0x0027, 0x002b, 0x002C, 0x002D, 0x002E, '1', 0x003a, 0x003b, 0x003f, 0x0040, 0x20ac},
    /*2*/{ 6, 'A', 'B', 'C', 0x00C1, 0x010C, '2'},
    /*3*/{ 7, 'D', 'E', 'F', 0x00C9, 0x010E, 0x011A, '3'},
    /*4*/{ 5, 'G', 'H', 'I', 0x00CD, '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 6, 'M', 'N', 'O', 0x00D3, 0x0147, '6'},
    /*7*/{ 7, 'P', 'Q', 'R', 'S', 0x0158, 0x0160, '7'},
    /*8*/{ 7, 'T', 'U', 'V', 0x00DA, 0x0164, 0x016E, '8'},
    /*9*/{ 7, 'W', 'X', 'Y', 'Z', 0x00DD, 0x017D, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 罗马尼亚语
#ifdef IM_ROMANIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_romanian_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 6, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C2, 0x0102},
    /*3*/{ 4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CE},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0218},
    /*8*/{ 5, 0x0054, 0x0055, 0x0056, 0x0038, 0x021A},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{ 0},
    /*#*/{ 0},
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_romanian_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 6, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E2, 0x0103},
    /*3*/{ 4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00EE},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0219},
    /*8*/{ 5, 0x0074, 0x0075, 0x0076, 0x0038, 0x021B},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{ 0},
    /*#*/{ 0},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_romanian_lower =
{
    /*1*/{10, 0x002C, 0x002D, 0x002E, 0x002f, '1', 0x003A, 0x003f, 0x0040, 0x005f, 0x007e},
    /*2*/{ 6, 'a', 'b', 'c', 0x00E2, 0x0103, '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 5, 'g', 'h', 'i', 0x00EE, '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 6, 'p', 'q', 'r', 's', 0x015F, '7'},
    /*8*/{ 5, 't', 'u', 'v', 0x0163, '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_romanian_upper =
{
    /*1*/{10, 0x002C, 0x002D, 0x002E, 0x002f, '1', 0x003A, 0x003f, 0x0040, 0x005f, 0x007e},
    /*2*/{ 6, 'A', 'B', 'C', 0x00C2, 0x0102, '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 5, 'G', 'H', 'I', 0x00CE, '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', 0x015E, '7'},
    /*8*/{ 5, 'T', 'U', 'V', 0x0162, '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 孟加拉语
//如果使用孟加拉语，请使用ETRUMP字库，这个条件至今有效，具体什么时候无效，得看字库的支持程度  2012-08-02
//孟加拉BTRC规范1键的前3个字符特殊处理:sogou, etrump使用0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd
//而cstar使用0x09FC = 0x09cd + 0x09b0, 0x09FD = 0x09cd + 0x09af, 0x09FE = 0x09b0 + 0x09cd
#ifdef IM_BENGALI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)	//CSTAR按孟加拉电信规范第一版来处理，1键上的自定义字符unicode码与sogou使用不同0x09FC, 0x09FD, 0x09FE
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bengali =
{
    /*1*/  {7, 0x09FC, 0x09FD, 0x09FE, 0x0982, 0x0983, 0x0981, 0x09E7},	//0x09FC = 0x09cd + 0x09b0, 0x09FD = 0x09cd + 0x09af, 0x09FE = 0x09b0 + 0x09cd三个字符为组合字符，是自造字
    /*2*/  {14, 0x0985, 0x0986, 0x0987, 0x0988, 0x0989, 0x098A, 0x098B, 0x09BE, 0x09BF, 0x09C0, 0x09C1, 0x09C2, 0x09C3, 0x09E8},
    /*3*/  {9, 0x098F, 0x0990, 0x0993, 0x0994, 0x09C7, 0x09C8, 0x09CB, 0x09CC, 0x09E9},
    /*4*/  {6, 0x0995, 0x0996, 0x0997, 0x0998, 0x0999, 0x09EA},
    /*5*/  {6, 0x099A, 0x099B, 0x099C, 0x099D, 0x099E, 0x09EB},
    /*6*/  {6, 0x099F, 0x09A0, 0x09A1, 0x09A2, 0x09A3, 0x09EC},
    /*7*/  {6, 0x09A4, 0x09A5, 0x09A6, 0x09A7, 0x09A8, 0x09ED},
    /*8*/  {6, 0x09AA, 0x09AB, 0x09AC, 0x09AD, 0x09AE, 0x09EE},
    /*9*/  {7, 0x09AF, 0x09B0, 0x09B2, 0x09B6, 0x09B7, 0x09B8, 0x09EF},
    /*0*/  {7, 0x0020, 0x09b9, 0x09dc, 0x09dd, 0x09df, 0x09ce, 0x09e6},	
    /***/  {1, 0x09cd},
    /*#*/  {8, 0x0964, 0x002e, ',', '?', '!', 0x09F3, '@', '\n'}, 
};
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_bengali =
{
    /*1*/  {3, 0xeac1, '-', 0x0981},	//0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd三个字符为组合字符，是自造字
    /*2*/  {3, 0x0985, '-', 0x098b},
    /*3*/  {3, 0x098f, '-', 0x0994},
    /*4*/  {3, 0x0995, '-', 0x0999},
    /*5*/  {3, 0x099a, '-', 0x099e},
    /*6*/  {3, 0x099f, '-', 0x09a3},
    /*7*/  {3, 0x09a4, '-', 0x09a8},
    /*8*/  {3, 0x09aa, '-', 0x09ae},
    /*9*/  {3, 0x09af, '-', 0x09b8},
    /*0*/  {3, 0x09b9, '-', 0x09ce},
    /***/  {1, 0x09CD},
    /*#*/  {3, 0x0964, '-', 0x21b5},	//0x21b5 为enter键的图形unicode码
};
#endif
#elif defined MMIIM_SOGOU_MULTITAP		//SOGOU按孟加拉电信规范第四版来处理
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bengali =
{
    /*1*/  {5, 0x0982, 0x0983, 0x0981, 0x09E7, 0x0031},	//0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd三个字符为组合字符，是自造字
    /*2*/  {15, 0x0985, 0x0986, 0x0987, 0x0988, 0x0989, 0x098A, 0x098B, 0x09BE, 0x09BF, 0x09C0, 0x09C1, 0x09C2, 0x09C3, 0x09E8, 0x0032},
    /*3*/  {10, 0x098F, 0x0990, 0x0993, 0x0994, 0x09C7, 0x09C8, 0x09CB, 0x09CC, 0x09E9, 0x0033},
    /*4*/  {7, 0x0995, 0x0996, 0x0997, 0x0998, 0x0999, 0x09EA, 0x0034},
    /*5*/  {7, 0x099A, 0x099B, 0x099C, 0x099D, 0x099E, 0x09EB, 0x0035},
    /*6*/  {7, 0x099F, 0x09A0, 0x09A1, 0x09A2, 0x09A3, 0x09EC, 0x0036},
    /*7*/  {7, 0x09A4, 0x09A5, 0x09A6, 0x09A7, 0x09A8, 0x09ED, 0x0037},
    /*8*/  {7, 0x09AA, 0x09AB, 0x09AC, 0x09AD, 0x09AE, 0x09EE, 0x0038},
    /*9*/  {8, 0x09AF, 0x09B0, 0x09B2, 0x09B6, 0x09B7, 0x09B8, 0x09EF, 0x0039},
    /*0*/  {8, ' ', 0x09B9, 0x09DC, 0x09DD, 0x09DF, 0x09CE, 0x09E6, 0x0030},
    /***/  {8, 0x09CD, 0x0964, 0x09BC, ',', '?', '!', 0x09F3, '@'},
    /*#*/  {0},
};
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_bengali =
{
    /*1*/  {3, 0xEAC1, '-', 0x0981},	//0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd三个字符为组合字符，是自造字
    /*2*/  {3, 0x0985, '-', 0x09E8},
    /*3*/  {3, 0x098F, '-', 0x09E9},
    /*4*/  {3, 0x0995, '-', 0x09EA},
    /*5*/  {3, 0x099A, '-', 0x09EB},
    /*6*/  {3, 0x099F, '-', 0x09EC},
    /*7*/  {3, 0x09A4, '-', 0x09ED},
    /*8*/  {3, 0x09AA, '-', 0x09EE},
    /*9*/  {3, 0x09AF, '-', 0x09EF},
    /*0*/  {3, 0x09B9, '-', 0x09E6},
    /***/  {1, 0x09CD},
    /*#*/  {0}
};
#endif
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bengali =
{
    /*1*/{ 8, 0x0981, 0x0982, 0x0983, 0x09cd, 0x09ce, 0x09f0, 0x09f1, '1'},
    /*2*/{14, 0x0985, 0x0986, 0x0987, 0x0988, 0x0989, 0x098a, 0x098b, 0x09be, 0x09bf, 0x09c0, 0x09c1, 0x09c2, 0x09c3, '2'},
    /*3*/{ 9, 0x098f, 0x0990, 0x0993, 0x0994, 0x09c7, 0x09c8, 0x09cb, 0x09cc, '3'},
    /*4*/{ 6, 0x0995, 0x0996, 0x0997, 0x0998, 0x0999, '4'},
    /*5*/{ 6, 0x099a, 0x099b, 0x099c, 0x099d, 0x099e, '5'},
    /*6*/{ 8, 0x099f, 0x09a0, 0x09a1, 0x09a2, 0x09a3, 0x09dc, 0x09dd, '6'},
    /*7*/{ 6, 0x09a4, 0x09a5, 0x09a6, 0x09a7, 0x09a8, '7'},
    /*8*/{ 6, 0x09aa, 0x09ab, 0x09ac, 0x09ad, 0x09ae, '8'},
    /*9*/{ 9, 0x09af, 0x09b0, 0x09b2, 0x09b6, 0x09b7, 0x09b8, 0x09b9, 0x09df, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_bengali =	//以sogou版本代替
{
    /*1*/  {3, 0xEAC1, '-', 0x0981},	//0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd三个字符为组合字符，是自造字
    /*2*/  {3, 0x0985, '-', 0x09E8},
    /*3*/  {3, 0x098F, '-', 0x09E9},
    /*4*/  {3, 0x0995, '-', 0x09EA},
    /*5*/  {3, 0x099A, '-', 0x09EB},
    /*6*/  {3, 0x099F, '-', 0x09EC},
    /*7*/  {3, 0x09A4, '-', 0x09ED},
    /*8*/  {3, 0x09AA, '-', 0x09EE},
    /*9*/  {3, 0x09AF, '-', 0x09EF},
    /*0*/  {3, 0x09B9, '-', 0x09E6},
    /***/  {1, 0x09CD},
    /*#*/  {0}
};
#endif
#endif
#endif

// 斯洛文尼亚语
#ifdef IM_SLOVENIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovenian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ')', '(', ';', '*'},
    /*2*/{ 5, 0x0061, 0x0062, 0x0063, 0x0032, 0x010D},
    /*3*/{ 4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0161},
    /*8*/{ 4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/{ 6, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovenian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ')', '(', ';', '*'},
    /*2*/{ 5, 0x0041, 0x0042, 0x0043, 0x0032, 0x010C},
    /*3*/{ 4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0160},
    /*8*/{ 4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/{ 6, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovenian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0041, 0x0042, 0x0043, 0x010C, 0x0032},
    /*3*/  {4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/  {6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0160, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {6, 0x0057, 0x0058, 0x0059, 0x005A, 0x017D, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovenian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0061, 0x0062, 0x0063, 0x010D, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/  {6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0161, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {6, 0x0077, 0x0078, 0x0079, 0x007A, 0x017E, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovenian_lower =
{
    /*1*/{13, 0x002E, 0x002C, 0x002D, 0x003f, 0x0021, 0x0040, 0x003A, 0x003B, 0x0027, 0x002b, 0x20ac, 0x0024, '1'},
    /*2*/{ 5, 'a', 'b', 'c', 0x010D, '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 6, 'p', 'q', 'r', 's', 0x0161, '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 6, 'w', 'x', 'y', 'z', 0x017E, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovenian_upper =
{
    /*1*/{13, 0x002E, 0x002C, 0x002D, 0x003f, 0x0021, 0x0040, 0x003A, 0x003B, 0x0027, 0x002b, 0x20ac, 0x0024, '1'},
    /*2*/{ 5, 'A', 'B', 'C', 0x010C, '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', 0x0160, '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', 0x017D, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 印度泰卢固语
#ifdef IM_TELUGU_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_telugu = 
{
    /*1*/ {5, 0x0031, 0x0C4D, 0x0C02, 0x0C03, 0x0C01},
    /*2*/ {6, 0x0C15, 0x0C16, 0x0C05, 0x0C06, 0x0032, 0x0C3E},
    /*3*/ {8, 0x0C17, 0x0C18, 0x0C19, 0x0C07, 0x0C08, 0x0033, 0x0C3F, 0x0C40},
    /*4*/ {10, 0x0C1A, 0x0C1B, 0x0C1C, 0x0C1D, 0x0C1E, 0x0C09, 0x0C0A, 0x0034, 0x0C41, 0x0C42},
    /*5*/ {8, 0x0C1F, 0x0C20, 0x0C21, 0x0C22, 0x0C23, 0x0C0B, 0x0035, 0x0C43},
    /*6*/ {9, 0x0C24, 0x0C25, 0x0C26, 0x0c27, 0x0C0E, 0x0C0F, 0x0036, 0x0C46, 0x0C47},
    /*7*/ {8, 0x0C28, 0x0C2A, 0x0C2B, 0x0C2C, 0x0C2D, 0x0C10, 0x0037, 0x0C48},
    /*8*/ {8, 0x0C2E, 0x0C2F, 0x0C30, 0x0C12, 0x0C13, 0x0038, 0x0C4A, 0x0C4B},
    /*9*/ {10, 0x0C32, 0x0C35, 0x0C36, 0x0C37, 0x0C38, 0x0C39, 0x0C33, 0x0C14, 0x0039, 0x0C4C},
    /*0*/ {2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_telugu =
{
    /*1*/  {10, 0x0C05, 0x0C06, 0x0C07, 0x0C08, 0x0C09, 0x0C0A, 0x0C0B, 0x0C0C, 0x0C67, 0x0031},
    /*2*/  {10, 0x0C0E, 0x0C0F, 0x0C10, 0x0C12, 0x0C13, 0x0C14, 0x0C60, 0x0C61, 0x0C68, 0x0032},
    /*3*/  {11, 0x0C01, 0x0C02, 0x0C03, 0x0C3E, 0x0C3F, 0x0C40, 0x0C41, 0x0C42, 0x0C43, 0x0C69, 0x0033},
    /*4*/  {12, 0x0C44, 0x0C46, 0x0C47, 0x0C48, 0x0C4A, 0x0C4B, 0x0C4C, 0x0C4D, 0x0C55, 0x0C56, 0x0C6A, 0x0034},
    /*5*/  {9, 0x0C15, 0x0C16, 0x0C17, 0x0C18, 0x0C19, 0x0C1A, 0x0C1B, 0x0C6B, 0x0035},
    /*6*/  {9, 0x0C1C, 0x0C1D, 0x0C1E, 0x0C1F, 0x0C20, 0x0C21, 0x0C22, 0x0C6C, 0x0036},
    /*7*/  {9, 0x0C23, 0x0C24, 0x0C25, 0x0C26, 0x0C27, 0x0C28, 0x0C2A, 0x0C6D, 0x0037},
    /*8*/  {9, 0x0C2B, 0x0C2C, 0x0C2D, 0x0C2E, 0x0C2F, 0x0C30, 0x0C31, 0x0C6E, 0x0038},
    /*9*/  {9, 0x0C32, 0x0C33, 0x0C35, 0x0C36, 0x0C37, 0x0C38, 0x0C39, 0x0C6F, 0x0039},
    /*0*/  {2, 0x0C66, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_telugu =
{
    /*1*/{ 7, TELUGU_C_KA, TELUGU_C_KHA, TELUGU_C_GA, TELUGU_C_GHA, TELUGU_C_NGA, TELUGU_D_ONE, '1'},
    /*2*/{ 7, TELUGU_C_CA, TELUGU_C_CHA, TELUGU_C_JA, TELUGU_C_JHA, TELUGU_C_NYA, TELUGU_D_TWO, '2'},
    /*3*/{ 7, TELUGU_C_TTA, TELUGU_C_TTHA, TELUGU_C_DDA, TELUGU_C_DDHA, TELUGU_C_NNA, TELUGU_D_THREE, '3'},
    /*4*/{ 7, TELUGU_C_TA, TELUGU_C_THA, TELUGU_C_DA, TELUGU_C_DHA, TELUGU_C_NA, TELUGU_D_FOUR, '4'},
    /*5*/{ 7, TELUGU_C_PA, TELUGU_C_PHA, TELUGU_C_BA, TELUGU_C_BHA, TELUGU_C_MA, TELUGU_D_FIVE, '5'},
    /*6*/{12, TELUGU_C_YA, TELUGU_C_RA, TELUGU_C_RRA, TELUGU_C_LA, TELUGU_C_LLA, TELUGU_C_VA, TELUGU_C_SHA, TELUGU_C_SSA, TELUGU_C_SA, TELUGU_C_HA, TELUGU_D_SIX, '6'},
    /*7*/{16, TELUGU_IV_A, TELUGU_IV_AA, TELUGU_IV_I, TELUGU_IV_II, TELUGU_IV_U, TELUGU_IV_UU, TELUGU_IV_VR, TELUGU_IV_VRR, TELUGU_IV_E, TELUGU_IV_EE, TELUGU_IV_AI, TELUGU_IV_O, TELUGU_IV_OO, TELUGU_IV_AU, TELUGU_D_SEVEN, '7'},
    /*8*/{ 2, TELUGU_D_EIGHT, '8'},
    /*9*/{ 2, TELUGU_D_NINE, '9'},
    /*0*/{ 5, ' ', TELUGU_D_ZERO, TELUGU_DEVANAGARI_S_DANDA, TELUGU_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_telugumatra =
{
    /*1*/{ 7, TELUGU_C_KA, TELUGU_C_KHA, TELUGU_C_GA, TELUGU_C_GHA, TELUGU_C_NGA, TELUGU_D_ONE, '1'},
    /*2*/{ 7, TELUGU_C_CA, TELUGU_C_CHA, TELUGU_C_JA, TELUGU_C_JHA, TELUGU_C_NYA, TELUGU_D_TWO, '2'},
    /*3*/{ 7, TELUGU_C_TTA, TELUGU_C_TTHA, TELUGU_C_DDA, TELUGU_C_DDHA, TELUGU_C_NNA, TELUGU_D_THREE, '3'},
    /*4*/{ 7, TELUGU_C_TA, TELUGU_C_THA, TELUGU_C_DA, TELUGU_C_DHA, TELUGU_C_NA, TELUGU_D_FOUR, '4'},
    /*5*/{ 7, TELUGU_C_PA, TELUGU_C_PHA, TELUGU_C_BA, TELUGU_C_BHA, TELUGU_C_MA, TELUGU_D_FIVE, '5'},
    /*6*/{12, TELUGU_C_YA, TELUGU_C_RA, TELUGU_C_RRA, TELUGU_C_LA, TELUGU_C_LLA, TELUGU_C_VA, TELUGU_C_SHA, TELUGU_C_SSA, TELUGU_C_SA, TELUGU_C_HA, TELUGU_D_SIX, '6'},
    /*7*/{16, TELUGU_IV_A, TELUGU_IV_AA, TELUGU_IV_I, TELUGU_IV_II, TELUGU_IV_U, TELUGU_IV_UU, TELUGU_IV_VR, TELUGU_IV_VRR, TELUGU_IV_E, TELUGU_IV_EE, TELUGU_IV_AI, TELUGU_IV_O, TELUGU_IV_OO, TELUGU_IV_AU, TELUGU_D_SEVEN, '7'},
    /*8*/{15, TELUGU_DV_AA, TELUGU_DV_I, TELUGU_DV_II, TELUGU_DV_U, TELUGU_DV_UU, TELUGU_DV_VR, TELUGU_DV_VRR, TELUGU_DV_E, TELUGU_DV_EE, TELUGU_DV_AI, TELUGU_DV_O, TELUGU_DV_OO, TELUGU_DV_AU, TELUGU_D_EIGHT, '8'},
    /*9*/{ 6, TELUGU_S_CHANDRA_BINDU, TELUGU_S_ANUSVARA, TELUGU_S_VISARGA, TELUGU_S_HALANT, TELUGU_D_NINE, '9'},
    /*0*/{ 5, ' ', TELUGU_D_ZERO, TELUGU_DEVANAGARI_S_DANDA, TELUGU_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_telugumatra1 =
{
    /*1*/{ 7, TELUGU_C_KA, TELUGU_C_KHA, TELUGU_C_GA, TELUGU_C_GHA, TELUGU_C_NGA, TELUGU_D_ONE, '1'},
    /*2*/{ 7, TELUGU_C_CA, TELUGU_C_CHA, TELUGU_C_JA, TELUGU_C_JHA, TELUGU_C_NYA, TELUGU_D_TWO, '2'},
    /*3*/{ 7, TELUGU_C_TTA, TELUGU_C_TTHA, TELUGU_C_DDA, TELUGU_C_DDHA, TELUGU_C_NNA, TELUGU_D_THREE, '3'},
    /*4*/{ 7, TELUGU_C_TA, TELUGU_C_THA, TELUGU_C_DA, TELUGU_C_DHA, TELUGU_C_NA, TELUGU_D_FOUR, '4'},
    /*5*/{ 7, TELUGU_C_PA, TELUGU_C_PHA, TELUGU_C_BA, TELUGU_C_BHA, TELUGU_C_MA, TELUGU_D_FIVE, '5'},
    /*6*/{12, TELUGU_C_YA, TELUGU_C_RA, TELUGU_C_RRA, TELUGU_C_LA, TELUGU_C_LLA, TELUGU_C_VA, TELUGU_C_SHA, TELUGU_C_SSA, TELUGU_C_SA, TELUGU_C_HA, TELUGU_D_SIX, '6'},
    /*7*/{16, TELUGU_IV_A, TELUGU_IV_AA, TELUGU_IV_I, TELUGU_IV_II, TELUGU_IV_U, TELUGU_IV_UU, TELUGU_IV_VR, TELUGU_IV_VRR, TELUGU_IV_E, TELUGU_IV_EE, TELUGU_IV_AI, TELUGU_IV_O, TELUGU_IV_OO, TELUGU_IV_AU, TELUGU_D_SEVEN, '7'},
    /*8*/{ 2, TELUGU_D_EIGHT, '8'},
    /*9*/{ 5, TELUGU_S_CHANDRA_BINDU, TELUGU_S_ANUSVARA, TELUGU_S_VISARGA, TELUGU_D_NINE, '9'},
    /*0*/{ 5, ' ', TELUGU_D_ZERO, TELUGU_DEVANAGARI_S_DANDA, TELUGU_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 印度马拉地语
#ifdef IM_MARATHI_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_marathi = 
{
    /*1*/ {9, 0x0031, 0x0967, 0x0964, 0x0950, 0x094D, 0x093C, 0x0902, 0x0903, 0x0901},
    /*2*/ {5, 0x0905, 0x0906, 0x0032, 0x0968, 0x093E},
    /*3*/ {12, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x0907, 0x0958, 0x0959, 0x095A, 0x0033, 0x0969, 0x093F},
    /*4*/ {12, 0x091A, 0x091B, 0x091C, 0x091D, 0x091E, 0x091F, 0x0920, 0x0908, 0x095B, 0x0034, 0x096A, 0x0940},
    /*5*/ {11, 0x0921, 0x0922, 0x0923, 0x0924, 0x0925, 0x0909, 0x095C, 0x095D, 0x0035, 0x096B, 0x0941},
    /*6*/ {14, 0x0926, 0x0927, 0x0928, 0x092A, 0x092B, 0x090A, 0x090B, 0x0929, 0x095E, 0x0036, 0x096C, 0x0942, 0x0943, 0x0945},
    /*7*/ {11, 0x092C, 0x092D, 0x092E, 0x092F, 0x090F, 0x0910, 0x095F, 0x0037, 0x096D, 0x0947, 0x0948},
    /*8*/ {10, 0x0930, 0x0932, 0x0933, 0x0911, 0x0913, 0x0931, 0x0038, 0x096E, 0x0949, 0x094B},
    /*9*/ {9, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x0914, 0x0039, 0x096F, 0x094C},
    /*0*/ {1, 0x0966},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_marathi =
{
    /*1*/  {2, 0x0967, 0x0031},
    /*2*/  {8, 0x0905, 0x0906, 0x0907, 0x0908, 0x0909, 0x090A, 0x0968, 0x0032},
    /*3*/  {7, 0x090B, 0x090F, 0x0910, 0x0913, 0x0914, 0x0969, 0x0033},
    /*4*/  {11, 0x0915, 0x0916, 0x0917, 0x0918, 0x0919, 0x091A, 0x091B, 0x091C, 0x091D, 0x096A, 0x0034},
    /*5*/  {10, 0x091E, 0x091F, 0x0920, 0x0921, 0x0922, 0x0923, 0x0924, 0x0925, 0x096B, 0x0035},
    /*6*/  {11, 0x0926, 0x0927, 0x0928, 0x092A, 0x092B, 0x092C, 0x092D, 0x092E, 0x092F, 0x096C, 0x0036},
    /*7*/  {10, 0x0930, 0x0932, 0x0933, 0x0935, 0x0936, 0x0937, 0x0938, 0x0939, 0x096D, 0x0037},
    /*8*/  {9, 0x0901, 0x0902, 0x0903, 0x093C, 0x093E, 0x093F, 0x0940, 0x096E, 0x0038},
    /*9*/  {10, 0x0941, 0x0942, 0x0943, 0x0947, 0x0948, 0x094B, 0x094C, 0x094D, 0x096F, 0x0039},
    /*0*/  {2, 0x0966, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_marathi =
{
    /*1*/{10, MARATHI_C_KA, MARATHI_C_KHA, MARATHI_C_GA, MARATHI_C_GHA, MARATHI_C_NGA, MARATHI_AC_QA, MARATHI_AC_KHHA, MARATHI_AC_GHHA, MARATHI_D_ONE, '1'},
    /*2*/{ 8, MARATHI_C_CHA, MARATHI_C_CHHA, MARATHI_C_JA, MARATHI_C_JHA, MARATHI_C_NYA, MARATHI_AC_ZA, MARATHI_D_TWO, '2'},
    /*3*/{ 9, MARATHI_C_TTA, MARATHI_C_TTHA, MARATHI_C_DDA, MARATHI_C_DDHA, MARATHI_C_NNA, MARATHI_AC_DDDHA, MARATHI_AC_RHA, MARATHI_D_THREE, '3'},
    /*4*/{ 8, MARATHI_C_TA, MARATHI_C_THA, MARATHI_C_DA, MARATHI_C_DHA, MARATHI_C_NA, MARATHI_C_NNNA, MARATHI_D_FOUR, '4'},
    /*5*/{ 8, MARATHI_C_PA, MARATHI_C_PHA, MARATHI_C_BA, MARATHI_C_BHA, MARATHI_C_MA, MARATHI_AC_FA, MARATHI_D_FIVE, '5'},
    /*6*/{10, MARATHI_C_YA, MARATHI_C_RA, MARATHI_C_LA, MARATHI_C_LLA, MARATHI_C_VA, MARATHI_C_LLLA, MARATHI_AC_YYA, MARATHI_C_RRA, MARATHI_D_SIX, '6'},
    /*7*/{ 6, MARATHI_C_SHA, MARATHI_C_SSA, MARATHI_C_SA, MARATHI_C_HA, MARATHI_D_SEVEN, '7'},
    /*8*/{14, MARATHI_IV_A, MARATHI_IV_AA, MARATHI_IV_I, MARATHI_IV_II, MARATHI_IV_U, MARATHI_IV_UU, MARATHI_IV_R, MARATHI_IV_E, MARATHI_IV_AI, MARATHI_IV_O, MARATHI_IV_AU, MARATHI_NIV_CO, MARATHI_D_EIGHT, '8'},
    /*9*/{ 2, MARATHI_D_NINE, '9'},
    /*0*/{ 7, ' ', '.', MARATHI_NG_D, MARATHI_S_OM, MARATHI_NG_DD, MARATHI_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_marathimatra =
{
    /*1*/{10, MARATHI_C_KA, MARATHI_C_KHA, MARATHI_C_GA, MARATHI_C_GHA, MARATHI_C_NGA, MARATHI_AC_QA, MARATHI_AC_KHHA, MARATHI_AC_GHHA, MARATHI_D_ONE, '1'},
    /*2*/{ 8, MARATHI_C_CHA, MARATHI_C_CHHA, MARATHI_C_JA, MARATHI_C_JHA, MARATHI_C_NYA, MARATHI_AC_ZA, MARATHI_D_TWO, '2'},
    /*3*/{ 9, MARATHI_C_TTA, MARATHI_C_TTHA, MARATHI_C_DDA, MARATHI_C_DDHA, MARATHI_C_NNA, MARATHI_AC_DDDHA, MARATHI_AC_RHA, MARATHI_D_THREE, '3'},
    /*4*/{ 8, MARATHI_C_TA, MARATHI_C_THA, MARATHI_C_DA, MARATHI_C_DHA, MARATHI_C_NA, MARATHI_C_NNNA, MARATHI_D_FOUR, '4'},
    /*5*/{ 8, MARATHI_C_PA, MARATHI_C_PHA, MARATHI_C_BA, MARATHI_C_BHA, MARATHI_C_MA, MARATHI_AC_FA, MARATHI_D_FIVE, '5'},
    /*6*/{10, MARATHI_C_YA, MARATHI_C_RA, MARATHI_C_LA, MARATHI_C_LLA, MARATHI_C_VA, MARATHI_C_LLLA, MARATHI_AC_YYA, MARATHI_C_RRA, MARATHI_D_SIX, '6'},
    /*7*/{ 6, MARATHI_C_SHA, MARATHI_C_SSA, MARATHI_C_SA, MARATHI_C_HA, MARATHI_D_SEVEN, '7'},
    /*8*/{14, MARATHI_IV_A, MARATHI_IV_AA, MARATHI_IV_I, MARATHI_IV_II, MARATHI_IV_U, MARATHI_IV_UU, MARATHI_IV_R, MARATHI_IV_E, MARATHI_IV_AI, MARATHI_IV_O, MARATHI_IV_AU, MARATHI_NIV_CO, MARATHI_D_EIGHT, '8'},
    /*9*/{14, MARATHI_DV_AA, MARATHI_DV_I, MARATHI_DV_II, MARATHI_DV_U, MARATHI_DV_UU, MARATHI_DV_R, MARATHI_DV_E, MARATHI_DV_AI, MARATHI_DV_CE, MARATHI_DV_O, MARATHI_DV_AU, MARATHI_NDV_CO, MARATHI_D_NINE, '9'},
    /*0*/{12, ' ', MARATHI_S_HALANT, MARATHI_S_CHANDRA_BINDU, MARATHI_S_BINDU, MARATHI_S_VISARGA, MARATHI_S_OM, MARATHI_NG_D, MARATHI_S_ABBR, MARATHI_NG_DD, '.', MARATHI_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_marathimatra1 =
{
    /*1*/{10, MARATHI_C_KA, MARATHI_C_KHA, MARATHI_C_GA, MARATHI_C_GHA, MARATHI_C_NGA, MARATHI_AC_QA, MARATHI_AC_KHHA, MARATHI_AC_GHHA, MARATHI_D_ONE, '1'},
    /*2*/{ 8, MARATHI_C_CHA, MARATHI_C_CHHA, MARATHI_C_JA, MARATHI_C_JHA, MARATHI_C_NYA, MARATHI_AC_ZA, MARATHI_D_TWO, '2'},
    /*3*/{ 9, MARATHI_C_TTA, MARATHI_C_TTHA, MARATHI_C_DDA, MARATHI_C_DDHA, MARATHI_C_NNA, MARATHI_AC_DDDHA, MARATHI_AC_RHA, MARATHI_D_THREE, '3'},
    /*4*/{ 8, MARATHI_C_TA, MARATHI_C_THA, MARATHI_C_DA, MARATHI_C_DHA, MARATHI_C_NA, MARATHI_C_NNNA, MARATHI_D_FOUR, '4'},
    /*5*/{ 8, MARATHI_C_PA, MARATHI_C_PHA, MARATHI_C_BA, MARATHI_C_BHA, MARATHI_C_MA, MARATHI_AC_FA, MARATHI_D_FIVE, '5'},
    /*6*/{10, MARATHI_C_YA, MARATHI_C_RA, MARATHI_C_LA, MARATHI_C_LLA, MARATHI_C_VA, MARATHI_C_LLLA, MARATHI_AC_YYA, MARATHI_C_RRA, MARATHI_D_SIX, '6'},
    /*7*/{ 6, MARATHI_C_SHA, MARATHI_C_SSA, MARATHI_C_SA, MARATHI_C_HA, MARATHI_D_SEVEN, '7'},
    /*8*/{14, MARATHI_IV_A, MARATHI_IV_AA, MARATHI_IV_I, MARATHI_IV_II, MARATHI_IV_U, MARATHI_IV_UU, MARATHI_IV_R, MARATHI_IV_E, MARATHI_IV_AI, MARATHI_IV_O, MARATHI_IV_AU, MARATHI_NIV_CO, MARATHI_D_EIGHT, '8'},
    /*9*/{ 2, MARATHI_D_NINE, '9'},
    /*0*/{11, ' ', MARATHI_S_CHANDRA_BINDU, MARATHI_S_BINDU, MARATHI_S_VISARGA, MARATHI_S_OM, MARATHI_NG_D, MARATHI_S_ABBR, MARATHI_NG_DD, '.', MARATHI_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_marathimatra2 =
{
    /*1*/{10, MARATHI_C_KA, MARATHI_C_KHA, MARATHI_C_GA, MARATHI_C_GHA, MARATHI_C_NGA, MARATHI_AC_QA, MARATHI_AC_KHHA, MARATHI_AC_GHHA, MARATHI_D_ONE, '1'},
    /*2*/{ 8, MARATHI_C_CHA, MARATHI_C_CHHA, MARATHI_C_JA, MARATHI_C_JHA, MARATHI_C_NYA, MARATHI_AC_ZA, MARATHI_D_TWO, '2'},
    /*3*/{ 9, MARATHI_C_TTA, MARATHI_C_TTHA, MARATHI_C_DDA, MARATHI_C_DDHA, MARATHI_C_NNA, MARATHI_AC_DDDHA, MARATHI_AC_RHA, MARATHI_D_THREE, '3'},
    /*4*/{ 8, MARATHI_C_TA, MARATHI_C_THA, MARATHI_C_DA, MARATHI_C_DHA, MARATHI_C_NA, MARATHI_C_NNNA, MARATHI_D_FOUR, '4'},
    /*5*/{ 8, MARATHI_C_PA, MARATHI_C_PHA, MARATHI_C_BA, MARATHI_C_BHA, MARATHI_C_MA, MARATHI_AC_FA, MARATHI_D_FIVE, '5'},
    /*6*/{10, MARATHI_C_YA, MARATHI_C_RA, MARATHI_C_LA, MARATHI_C_LLA, MARATHI_C_VA, MARATHI_C_LLLA, MARATHI_AC_YYA, MARATHI_C_RRA, MARATHI_D_SIX, '6'},
    /*7*/{ 6, MARATHI_C_SHA, MARATHI_C_SSA, MARATHI_C_SA, MARATHI_C_HA, MARATHI_D_SEVEN, '7'},
    /*8*/{14, MARATHI_IV_A, MARATHI_IV_AA, MARATHI_IV_I, MARATHI_IV_II, MARATHI_IV_U, MARATHI_IV_UU, MARATHI_IV_R, MARATHI_IV_E, MARATHI_IV_AI, MARATHI_IV_O, MARATHI_IV_AU, MARATHI_NIV_CO, MARATHI_D_EIGHT, '8'},
    /*9*/{ 2, MARATHI_D_NINE, '9'},
    /*0*/{10, ' ', '.', MARATHI_S_BINDU, MARATHI_S_VISARGA, MARATHI_S_OM, MARATHI_NG_D, MARATHI_S_ABBR, MARATHI_NG_DD, MARATHI_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 泰米尔语
#ifdef IM_TAMIL_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tamil = 
{
    /*1*/ {3, 0x0031, 0x0B83, 0x0BCD},
    /*2*/ {12, 0x0B85, 0x0B86, 0x0B87, 0x0B88, 0x0B89, 0x0B8A, 0x0032, 0x0BBE, 0x0BBF, 0x0BC0, 0x0BC1, 0x0BC2},
    /*3*/ {13, 0x0B8E, 0x0B8F, 0x0B90, 0x0B92, 0x0B93, 0x0B94, 0x0033, 0x0BC6, 0x0BC7, 0x0BC8, 0x0BCA, 0x0BCB, 0x0BCC},
    /*4*/ {5, 0x0B95, 0x0B99, 0x0B9A, 0x0B9E, 0x0034},
    /*5*/ {5, 0x0B9F, 0x0BA3, 0x0BA4, 0x0BA8, 0x0035},
    /*6*/ {4, 0x0BAA, 0x0BAE, 0x0BAF, 0x0036},
    /*7*/ {4, 0x0BB0, 0x0BB2, 0x0BB5, 0x0037},
    /*8*/ {5, 0x0BB4, 0x0BB3, 0x0BB1, 0x0BA9, 0x0038},
    /*9*/ {5, 0x0B9C, 0x0BB7, 0x0BB8, 0x0BB9, 0x0039},    
    /*0*/ {2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tamil =
{
    /*1*/  {13, 0x0BF0, 0x0BF1, 0x0BF2, 0x0BF3, 0x0BF4, 0x0BF5, 0x0BF6, 0x0BF7, 0x0BF8, 0x0BF9, 0x0BFA, 0x0BE7, 0x0031},
    /*2*/  {8, 0x0B85, 0x0B86, 0x0B87, 0x0B88, 0x0B89, 0x0B8A, 0x0BE8, 0x0032},
    /*3*/  {8, 0x0B8E, 0x0B8F, 0x0B90, 0x0B92, 0x0B93, 0x0B94, 0x0BE9, 0x0033},
    /*4*/  {8, 0x0B95, 0x0B99, 0x0B9A, 0x0B9C, 0x0B9E, 0x0B9F, 0x0BEA, 0x0034},
    /*5*/  {8, 0x0BA3, 0x0BA4, 0x0BA8, 0x0BA9, 0x0BAA, 0x0BAE, 0x0BEB, 0x0035},
    /*6*/  {8, 0x0BAF, 0x0BB0, 0x0BB1, 0x0BB2, 0x0BB3, 0x0BB4, 0x0BEC, 0x0036},
    /*7*/  {7, 0x0BB5, 0x0BB6, 0x0BB7, 0x0BB8, 0x0BB9, 0x0BED, 0x0037},
    /*8*/  {8, 0x0BBE, 0x0BBF, 0x0BC0, 0x0BC1, 0x0BC2, 0x0B83, 0x0BEE, 0x0038},
    /*9*/  {10, 0x0BC6, 0x0BC7, 0x0BC8, 0x0BCA, 0x0BCB, 0x0BCC, 0x0BCD, 0x0BD7, 0x0BEF, 0x0039},
    /*0*/  {2, 0x0BE6, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tamil =
{
    /*1*/{ 4, TAMIL_C_KA, TAMIL_C_NGA, TAMIL_D_ONE, '1'},
    /*2*/{ 5, TAMIL_C_CHA, TAMIL_C_JA, TAMIL_C_NYA, TAMIL_D_TWO, '2'},
    /*3*/{ 4, TAMIL_C_TTA, TAMIL_C_NNA, TAMIL_D_THREE, '3'},
    /*4*/{ 5, TAMIL_C_TA, TAMIL_C_NA, TAMIL_C_NNNA, TAMIL_D_FOUR, '4'},
    /*5*/{ 4, TAMIL_C_PA, TAMIL_C_MA, TAMIL_D_FIVE, '5'},
    /*6*/{12, TAMIL_C_YA, TAMIL_C_RA, TAMIL_C_RRA, TAMIL_C_LA, TAMIL_C_LLA, TAMIL_C_LLLA, TAMIL_C_VA, TAMIL_C_SSA, TAMIL_C_SA, TAMIL_C_HA, TAMIL_D_SIX, '6'},
    /*7*/{14, TAMIL_IV_A, TAMIL_IV_AA, TAMIL_IV_I, TAMIL_IV_II, TAMIL_IV_U, TAMIL_IV_UU, TAMIL_IV_E, TAMIL_IV_EE, TAMIL_IV_AI, TAMIL_IV_O, TAMIL_IV_OO, TAMIL_IV_AU, TAMIL_D_SEVEN, '7'},
    /*8*/{ 2, TAMIL_D_EIGHT, '8'},
    /*9*/{ 3, TAMIL_S_VISARGA, TAMIL_D_NINE, '9'},
    /*0*/{ 5, ' ', TAMIL_D_ZERO, TAMIL_DEVANAGARI_S_DANDA, TAMIL_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tamilmatra =
{
    /*1*/{ 4, TAMIL_C_KA, TAMIL_C_NGA, TAMIL_D_ONE, '1'},
    /*2*/{ 5, TAMIL_C_CHA, TAMIL_C_JA, TAMIL_C_NYA, TAMIL_D_TWO, '2'},
    /*3*/{ 4, TAMIL_C_TTA, TAMIL_C_NNA, TAMIL_D_THREE, '3'},
    /*4*/{ 5, TAMIL_C_TA, TAMIL_C_NA, TAMIL_C_NNNA, TAMIL_D_FOUR, '4'},
    /*5*/{ 4, TAMIL_C_PA, TAMIL_C_MA, TAMIL_D_FIVE, '5'},
    /*6*/{12, TAMIL_C_YA, TAMIL_C_RA, TAMIL_C_RRA, TAMIL_C_LA, TAMIL_C_LLA, TAMIL_C_LLLA, TAMIL_C_VA, TAMIL_C_SSA, TAMIL_C_SA, TAMIL_C_HA, TAMIL_D_SIX, '6'},
    /*7*/{14, TAMIL_IV_A, TAMIL_IV_AA, TAMIL_IV_I, TAMIL_IV_II, TAMIL_IV_U, TAMIL_IV_UU, TAMIL_IV_E, TAMIL_IV_EE, TAMIL_IV_AI, TAMIL_IV_O, TAMIL_IV_OO, TAMIL_IV_AU, TAMIL_D_SEVEN, '7'},
    /*8*/{13, TAMIL_DV_AA, TAMIL_DV_I, TAMIL_DV_II, TAMIL_DV_U, TAMIL_DV_UU, TAMIL_DV_E, TAMIL_DV_EE, TAMIL_DV_AI, TAMIL_DV_O, TAMIL_DV_OO, TAMIL_DV_AU, TAMIL_D_EIGHT, '8'},
    /*9*/{ 4, TAMIL_S_VISARGA, TAMIL_S_HALANT, TAMIL_D_NINE, '9'},
    /*0*/{ 5, ' ', TAMIL_D_ZERO, TAMIL_DEVANAGARI_S_DANDA, TAMIL_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_tamilmatra1 =
{
    /*1*/{ 4, TAMIL_C_KA, TAMIL_C_NGA, TAMIL_D_ONE, '1'},
    /*2*/{ 5, TAMIL_C_CHA, TAMIL_C_JA, TAMIL_C_NYA, TAMIL_D_TWO, '2'},
    /*3*/{ 4, TAMIL_C_TTA, TAMIL_C_NNA, TAMIL_D_THREE, '3'},
    /*4*/{ 5, TAMIL_C_TA, TAMIL_C_NA, TAMIL_C_NNNA, TAMIL_D_FOUR, '4'},
    /*5*/{ 4, TAMIL_C_PA, TAMIL_C_MA, TAMIL_D_FIVE, '5'},
    /*6*/{12, TAMIL_C_YA, TAMIL_C_RA, TAMIL_C_RRA, TAMIL_C_LA, TAMIL_C_LLA, TAMIL_C_LLLA, TAMIL_C_VA, TAMIL_C_SSA, TAMIL_C_SA, TAMIL_C_HA, TAMIL_D_SIX, '6'},
    /*7*/{14, TAMIL_IV_A, TAMIL_IV_AA, TAMIL_IV_I, TAMIL_IV_II, TAMIL_IV_U, TAMIL_IV_UU, TAMIL_IV_E, TAMIL_IV_EE, TAMIL_IV_AI, TAMIL_IV_O, TAMIL_IV_OO, TAMIL_IV_AU, TAMIL_D_SEVEN, '7'},
    /*8*/{ 2, TAMIL_D_EIGHT, '8'},
    /*9*/{ 3, TAMIL_S_VISARGA, TAMIL_D_NINE, '9'},
    /*0*/{ 5, ' ', TAMIL_D_ZERO, TAMIL_DEVANAGARI_S_DANDA, TAMIL_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 印度古吉拉特语
#ifdef IM_GUJARATI_SUPPORT
#if defined MMIIM_CSTAR_MULTITAP
#if 0
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujarati =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{ 2, GUJARATI_D_NINE, '9'},
    /*0*/{ 5, ' ', '.', GUJARATI_S_OM, GUJARATI_D_ZERO, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujarati =
{
    /*1*/{ 7, 0x0A95, 0x0A96, 0x0A97, 0x0A98, 0x0A99, 0x0AE7, 0x0031},
    /*2*/{ 7, 0x0A9A, 0x0A9B, 0x0A9C, 0x0A9D, 0x0A9E, 0x0AE8, 0x0032},
    /*3*/{ 7, 0x0A9F, 0x0AA0, 0x0AA1, 0x0AA2, 0x0AA3, 0x0AE9, 0x0033},
    /*4*/{ 7, 0x0AA4, 0x0AA5, 0x0AA6, 0x0AA7, 0x0AA8, 0x0AEA, 0x0034},
    /*5*/{ 7, 0x0AAA, 0x0AAB, 0x0AAC, 0x0AAD, 0x0AAE, 0x0AEB, 0x0035},
    /*6*/{ 7, 0x0AAF, 0x0AB0, 0x0AB2, 0x0AB3, 0x0AB5, 0x0AEC, 0x0036},
    /*7*/{ 6, 0x0AB6, 0x0AB7, 0x0AB8, 0x0AB9, 0x0AED, 0x0037},
    /*8*/{16, 0x0A85, 0x0A86, 0x0A87, 0x0A88, 0x0A89, 0x0A8A, 0x0A8B, 0x0A8C, 0x0A8D, 0x0A8F, 0x0A90, 0x0A91, 0x0A93, 0x0A94, 0x0AEE, 0x0038},
    /*9*/{15, 0x0AEF, 0x0039, 0x0ABE, 0x0ABF, 0x0AC0, 0x0AC1, 0x0AC2, 0x0AC3, 0x0AC4, 0x0AC5, 0x0AC7, 0x0AC8, 0x0AC9, 0x0ACB, 0x0ACC},
    /*0*/{13, 0x0020, 0x002E, 0x0AD0, 0x0AF1, 0x0AE6, 0x0964, 0x0965, 0x0030, 0x0ACD, 0x0A81, 0x0A82, 0x0A83, 0x0ABC},
    /***/{0},
    /*#*/{0}
};


static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{15, GUJARATI_DV_AA, GUJARATI_DV_I, GUJARATI_DV_II, GUJARATI_DV_U, GUJARATI_DV_UU, GUJARATI_DV_VR, GUJARATI_DV_VRR, GUJARATI_DV_CE, GUJARATI_DV_E, GUJARATI_DV_AI, GUJARATI_DV_CO, GUJARATI_DV_O, GUJARATI_DV_AU, GUJARATI_D_NINE, '9'},
    /*0*/{12, ' ', GUJARATI_S_HALANT, GUJARATI_S_CHANDRA_BINDU, GUJARATI_S_BINDU, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra1 =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{ 2, GUJARATI_D_NINE, '9'},
    /*0*/{11, ' ', GUJARATI_S_CHANDRA_BINDU, GUJARATI_S_BINDU, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra2 =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{ 2, GUJARATI_D_NINE, '9'},
    /*0*/{10, ' ', GUJARATI_S_BINDU, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra3 =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{15, GUJARATI_DV_AA, GUJARATI_DV_I, GUJARATI_DV_II, GUJARATI_DV_U, GUJARATI_DV_UU, GUJARATI_DV_VR, GUJARATI_DV_VRR, GUJARATI_DV_CE, GUJARATI_DV_E, GUJARATI_DV_AI, GUJARATI_DV_CO, GUJARATI_DV_O, GUJARATI_DV_AU, GUJARATI_D_NINE, '9'},
    /*0*/{13, ' ', GUJARATI_S_HALANT, GUJARATI_S_CHANDRA_BINDU, GUJARATI_S_BINDU, GUJARATI_S_NUKTA, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujarati =
{
    /*1*/  {9, 0x0A85, 0x0A86, 0x0A87, 0x0A88, 0x0A89, 0x0A8A, 0x0A8B, 0x0A8C, 0x0AE7},
    /*2*/  {9, 0x0A8D, 0x0A8F, 0x0A90, 0x0A91, 0x0A93, 0x0A94, 0x0AE0, 0x0AE1, 0x0AE8},
    /*3*/  {9, 0x0A81, 0x0A82, 0x0A83, 0x0ABC, 0x0ABE, 0x0ABF, 0x0AC0, 0x0AC1, 0x0AC2},
    /*4*/  {9, 0x0AC4, 0x0AC5, 0x0AC7, 0x0AC8, 0x0AC9, 0x0ACB, 0x0ACC, 0x0ACD, 0x0AE2},
    /*5*/  {9, 0x0A95, 0x0A96, 0x0A97, 0x0A98, 0x0A99, 0x0A9A, 0x0A9B, 0x0AEB, 0x0035},
    /*6*/  {9, 0x0A9C, 0x0A9D, 0x0A9E, 0x0A9F, 0x0AA0, 0x0AA1, 0x0AA2, 0x0AEC, 0x0036},
    /*7*/  {8, 0x0AA3, 0x0AA4, 0x0AA5, 0x0AA6, 0x0AA7, 0x0AA8, 0x0AED, 0x0037},
    /*8*/  {2, 0x0AEE, 0x0038},
    /*9*/  {1, 0x0AB2},
    /*0*/  {0},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujarati =
{
    /*1*/{ 7, 0x0A95, 0x0A96, 0x0A97, 0x0A98, 0x0A99, 0x0AE7, 0x0031},
    /*2*/{ 7, 0x0A9A, 0x0A9B, 0x0A9C, 0x0A9D, 0x0A9E, 0x0AE8, 0x0032},
    /*3*/{ 7, 0x0A9F, 0x0AA0, 0x0AA1, 0x0AA2, 0x0AA3, 0x0AE9, 0x0033},
    /*4*/{ 7, 0x0AA4, 0x0AA5, 0x0AA6, 0x0AA7, 0x0AA8, 0x0AEA, 0x0034},
    /*5*/{ 7, 0x0AAA, 0x0AAB, 0x0AAC, 0x0AAD, 0x0AAE, 0x0AEB, 0x0035},
    /*6*/{ 7, 0x0AAF, 0x0AB0, 0x0AB2, 0x0AB3, 0x0AB5, 0x0AEC, 0x0036},
    /*7*/{ 6, 0x0AB6, 0x0AB7, 0x0AB8, 0x0AB9, 0x0AED, 0x0037},
    /*8*/{16, 0x0A85, 0x0A86, 0x0A87, 0x0A88, 0x0A89, 0x0A8A, 0x0A8B, 0x0A8C, 0x0A8D, 0x0A8F, 0x0A90, 0x0A91, 0x0A93, 0x0A94, 0x0AEE, 0x0038},
    /*9*/{15, 0x0AEF, 0x0039, 0x0ABE, 0x0ABF, 0x0AC0, 0x0AC1, 0x0AC2, 0x0AC3, 0x0AC4, 0x0AC5, 0x0AC7, 0x0AC8, 0x0AC9, 0x0ACB, 0x0ACC},
    /*0*/{13, 0x0020, 0x002E, 0x0AD0, 0x0AF1, 0x0AE6, 0x0964, 0x0965, 0x0030, 0x0ACD, 0x0A81, 0x0A82, 0x0A83, 0x0ABC},
    /***/{0},
    /*#*/{0}
};


static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{15, GUJARATI_DV_AA, GUJARATI_DV_I, GUJARATI_DV_II, GUJARATI_DV_U, GUJARATI_DV_UU, GUJARATI_DV_VR, GUJARATI_DV_VRR, GUJARATI_DV_CE, GUJARATI_DV_E, GUJARATI_DV_AI, GUJARATI_DV_CO, GUJARATI_DV_O, GUJARATI_DV_AU, GUJARATI_D_NINE, '9'},
    /*0*/{12, ' ', GUJARATI_S_HALANT, GUJARATI_S_CHANDRA_BINDU, GUJARATI_S_BINDU, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra1 =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{ 2, GUJARATI_D_NINE, '9'},
    /*0*/{11, ' ', GUJARATI_S_CHANDRA_BINDU, GUJARATI_S_BINDU, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra2 =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{ 2, GUJARATI_D_NINE, '9'},
    /*0*/{10, ' ', GUJARATI_S_BINDU, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_gujaratimatra3 =
{
    /*1*/{ 7, GUJARATI_C_KA, GUJARATI_C_KHA, GUJARATI_C_GA, GUJARATI_C_GHA, GUJARATI_C_NGA, GUJARATI_D_ONE, '1'},
    /*2*/{ 7, GUJARATI_C_CA, GUJARATI_C_CHA, GUJARATI_C_JA, GUJARATI_C_JHA, GUJARATI_C_NYA, GUJARATI_D_TWO, '2'},
    /*3*/{ 7, GUJARATI_C_TTA, GUJARATI_C_TTHA, GUJARATI_C_DDA, GUJARATI_C_DDHA, GUJARATI_C_NNA, GUJARATI_D_THREE, '3'},
    /*4*/{ 7, GUJARATI_C_TA, GUJARATI_C_THA, GUJARATI_C_DA, GUJARATI_C_DHA, GUJARATI_C_NA, GUJARATI_D_FOUR, '4'},
    /*5*/{ 7, GUJARATI_C_PA, GUJARATI_C_PHA, GUJARATI_C_BA, GUJARATI_C_BHA, GUJARATI_C_MA, GUJARATI_D_FIVE, '5'},
    /*6*/{ 7, GUJARATI_C_YA, GUJARATI_C_RA, GUJARATI_C_LA, GUJARATI_C_LLA, GUJARATI_C_VA, GUJARATI_D_SIX, '6'},
    /*7*/{ 6, GUJARATI_C_SHA, GUJARATI_C_SSA, GUJARATI_C_SA, GUJARATI_C_HA, GUJARATI_D_SEVEN, '7'},
    /*8*/{16, GUJARATI_IV_A, GUJARATI_IV_AA, GUJARATI_IV_I, GUJARATI_IV_II, GUJARATI_IV_U, GUJARATI_IV_UU, GUJARATI_IV_VR, GUJARATI_IV_VL, GUJARATI_IV_CE, GUJARATI_IV_E, GUJARATI_IV_AI, GUJARATI_IV_CO, GUJARATI_IV_O, GUJARATI_IV_AU, GUJARATI_D_EIGHT, '8'},
    /*9*/{15, GUJARATI_DV_AA, GUJARATI_DV_I, GUJARATI_DV_II, GUJARATI_DV_U, GUJARATI_DV_UU, GUJARATI_DV_VR, GUJARATI_DV_VRR, GUJARATI_DV_CE, GUJARATI_DV_E, GUJARATI_DV_AI, GUJARATI_DV_CO, GUJARATI_DV_O, GUJARATI_DV_AU, GUJARATI_D_NINE, '9'},
    /*0*/{13, ' ', GUJARATI_S_HALANT, GUJARATI_S_CHANDRA_BINDU, GUJARATI_S_BINDU, GUJARATI_S_NUKTA, GUJARATI_S_VISARGA, GUJARATI_S_OM, GUJARATI_D_CS, '.', GUJARATI_D_ZERO, GUJARATI_DEVANAGARI_S_DANDA, GUJARATI_DEVANAGARI_S_DDANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 印度卡纳达语
#ifdef IM_KANNADA_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kannada = 
{
    /*1*/ {7, 0x0c95, 0x0c96, 0x0c97, 0x0c98, 0x0c99, 0x0ce7, 0x0031},
    /*2*/ {7, 0x0c9a, 0x0c9b, 0x0c9c, 0x0c9d, 0x0c9e, 0x0ce8, 0x0032},
    /*3*/ {7, 0x0c9f, 0x0ca0, 0x0ca1, 0x0ca2, 0x0ca3, 0x0ce9, 0x0033},
    /*4*/ {7, 0x0ca4, 0x0ca5, 0x0ca6, 0x0ca7, 0x0ca8, 0x0cea, 0x0034},
    /*5*/ {7, 0x0caa, 0x0cab, 0x0cac, 0x0cad, 0x0cae, 0x0ceb, 0x0035},
    /*6*/ {11, 0x0caf, 0x0cb0, 0x0cb2, 0x0cb5, 0x0cb6, 0x0cb7, 0x0cb8, 0x0cb9, 0x0cb3, 0x0cec, 0x0036},
    /*7*/ {15, 0x0c85, 0x0c86, 0x0c87, 0x0c88, 0x0c89, 0x0c8a, 0x0c8b, 0x0c8e, 0x0c8f, 0x0c90, 0x0c92, 0x0c93, 0x0c94, 0x0ced, 0x0037},
    /*8*/ {15, 0x0cee, 0x0038, 0x0cbe, 0x0cbf, 0x0cc0, 0x0cc1, 0x0cc2, 0x0cc3, 0x0cc4, 0x0cc6, 0x0cc7, 0x0cc8, 0x0cca, 0x0ccb, 0x0ccc},
    /*9*/ {5, 0x0cef, 0x0039, 0x0c82, 0x0c83, 0x0ccd},
    /*0*/ {7, 0x0020, 0x0cf1, 0x0cf2, 0x0ce6, 0x0964, 0x0965, 0x0030},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kannada =
{
    /*1*/  {10, 0x0C85, 0x0C86, 0x0C87, 0x0C88, 0x0C89, 0x0C8A, 0x0C8B, 0x0C8C, 0x0CE7, 0x0031},
    /*2*/  {10, 0x0C8E, 0x0C8F, 0x0C90, 0x0C92, 0x0C93, 0x0C94, 0x0CE0, 0x0CE1, 0x0CE8, 0x0032},
    /*3*/  {11, 0x0C82, 0x0C83, 0x0CBE, 0x0CBF, 0x0CC0, 0x0CC1, 0x0CC2, 0x0CC3, 0x0CC4, 0x0CE9, 0x0033},
    /*4*/  {11, 0x0CC6, 0x0CC7, 0x0CC8, 0x0CCA, 0x0CCB, 0x0CCC, 0x0CCD, 0x0CD5, 0x0CD6, 0x0CEA, 0x0034},
    /*5*/  {9, 0x0C95, 0x0C96, 0x0C97, 0x0C98, 0x0C99, 0x0C9A, 0x0C9B, 0x0CEB, 0x0035},
    /*6*/  {9, 0x0C9C, 0x0C9D, 0x0C9E, 0x0C9F, 0x0CA0, 0x0CA1, 0x0CA2, 0x0CEC, 0x0036},
    /*7*/  {8, 0x0CA3, 0x0CA4, 0x0CA5, 0x0CA6, 0x0CA7, 0x0CA8, 0x0CED, 0x0037},
    /*8*/  {10, 0x0CAA, 0x0CAB, 0x0CAC, 0x0CAD, 0x0CAE, 0x0CAF, 0x0CB0, 0x0CB1, 0x0CEE, 0x0038},
    /*9*/  {10, 0x0CB2, 0x0CB3, 0x0CB5, 0x0CB6, 0x0CB7, 0x0CB8, 0x0CB9, 0x0CDE, 0x0CEF, 0x0039},
    /*0*/  {2, 0x0CE6, 0x0030},
};
#else

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kannada =
{
    /*1*/{ 7, KANNADA_C_KA, KANNADA_C_KHA, KANNADA_C_GA, KANNADA_C_GHA, KANNADA_C_NGA, KANNADA_D_ONE, '1'},
    /*2*/{ 7, KANNADA_C_CA, KANNADA_C_CHA, KANNADA_C_JA, KANNADA_C_JHA, KANNADA_C_NYA, KANNADA_D_TWO, '2'},
    /*3*/{ 7, KANNADA_C_TTA, KANNADA_C_TTHA, KANNADA_C_DDA, KANNADA_C_DDHA, KANNADA_C_NNA, KANNADA_D_THREE, '3'},
    /*4*/{ 7, KANNADA_C_TA, KANNADA_C_THA, KANNADA_C_DA, KANNADA_C_DHA, KANNADA_C_NA, KANNADA_D_FOUR, '4'},
    /*5*/{ 7, KANNADA_C_PA, KANNADA_C_PHA, KANNADA_C_BA, KANNADA_C_BHA, KANNADA_C_MA, KANNADA_D_FIVE, '5'},
    /*6*/{11, KANNADA_C_YA, KANNADA_C_RA, KANNADA_C_LA, KANNADA_C_VA, KANNADA_C_SHA, KANNADA_C_SSA, KANNADA_C_SA, KANNADA_C_HA, KANNADA_C_LLA, KANNADA_D_SIX, '6'},
    /*7*/{15, KANNADA_IV_A, KANNADA_IV_AA, KANNADA_IV_I, KANNADA_IV_II, KANNADA_IV_U, KANNADA_IV_UU, KANNADA_IV_VR, KANNADA_IV_E, KANNADA_IV_EE, KANNADA_IV_AI, KANNADA_IV_O, KANNADA_IV_OO, KANNADA_IV_AU, KANNADA_D_SEVEN, '7'},
    /*8*/{ 2, KANNADA_D_EIGHT, '8'},
    /*9*/{ 2, KANNADA_D_NINE, '9'},
    /*0*/{ 7, ' ', KANNADA_S_JIHVAMULIYA, KANNADA_S_UPADHMANIYA, KANNADA_D_ZERO, KANNADA_DEVANAGARI_S_DANDA, KANNADA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kannadamatra =
{
    /*1*/{ 7, KANNADA_C_KA, KANNADA_C_KHA, KANNADA_C_GA, KANNADA_C_GHA, KANNADA_C_NGA, KANNADA_D_ONE, '1'},
    /*2*/{ 7, KANNADA_C_CA, KANNADA_C_CHA, KANNADA_C_JA, KANNADA_C_JHA, KANNADA_C_NYA, KANNADA_D_TWO, '2'},
    /*3*/{ 7, KANNADA_C_TTA, KANNADA_C_TTHA, KANNADA_C_DDA, KANNADA_C_DDHA, KANNADA_C_NNA, KANNADA_D_THREE, '3'},
    /*4*/{ 7, KANNADA_C_TA, KANNADA_C_THA, KANNADA_C_DA, KANNADA_C_DHA, KANNADA_C_NA, KANNADA_D_FOUR, '4'},
    /*5*/{ 7, KANNADA_C_PA, KANNADA_C_PHA, KANNADA_C_BA, KANNADA_C_BHA, KANNADA_C_MA, KANNADA_D_FIVE, '5'},
    /*6*/{11, KANNADA_C_YA, KANNADA_C_RA, KANNADA_C_LA, KANNADA_C_VA, KANNADA_C_SHA, KANNADA_C_SSA, KANNADA_C_SA, KANNADA_C_HA, /*KANNADA_C_RRA, */ KANNADA_C_LLA, KANNADA_D_SIX, '6'},
    /*7*/{15, KANNADA_IV_A, KANNADA_IV_AA, KANNADA_IV_I, KANNADA_IV_II, KANNADA_IV_U, KANNADA_IV_UU, KANNADA_IV_VR, /*KANNADA_IV_VL, */ KANNADA_IV_E, KANNADA_IV_EE, KANNADA_IV_AI, KANNADA_IV_O, KANNADA_IV_OO, KANNADA_IV_AU, KANNADA_D_SEVEN, '7'},
    /*8*/{15, KANNADA_DV_AA, KANNADA_DV_I, KANNADA_DV_II, KANNADA_DV_U, KANNADA_DV_UU, KANNADA_DV_VR, KANNADA_DV_VRR, KANNADA_DV_E, KANNADA_DV_EE, KANNADA_DV_AI, KANNADA_DV_O, KANNADA_DV_OO, KANNADA_DV_AU, KANNADA_D_EIGHT, '8'},
    /*9*/{ 5, KANNADA_S_ANUSVARA, KANNADA_S_VISARGA, KANNADA_S_HALANT, KANNADA_D_NINE, '9'},
    /*0*/{ 7, ' ', KANNADA_S_JIHVAMULIYA, KANNADA_S_UPADHMANIYA, KANNADA_D_ZERO, KANNADA_DEVANAGARI_S_DANDA, KANNADA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kannadamatra1 =
{
    /*1*/{ 7, KANNADA_C_KA, KANNADA_C_KHA, KANNADA_C_GA, KANNADA_C_GHA, KANNADA_C_NGA, KANNADA_D_ONE, '1'},
    /*2*/{ 7, KANNADA_C_CA, KANNADA_C_CHA, KANNADA_C_JA, KANNADA_C_JHA, KANNADA_C_NYA, KANNADA_D_TWO, '2'},
    /*3*/{ 7, KANNADA_C_TTA, KANNADA_C_TTHA, KANNADA_C_DDA, KANNADA_C_DDHA, KANNADA_C_NNA, KANNADA_D_THREE, '3'},
    /*4*/{ 7, KANNADA_C_TA, KANNADA_C_THA, KANNADA_C_DA, KANNADA_C_DHA, KANNADA_C_NA, KANNADA_D_FOUR, '4'},
    /*5*/{ 7, KANNADA_C_PA, KANNADA_C_PHA, KANNADA_C_BA, KANNADA_C_BHA, KANNADA_C_MA, KANNADA_D_FIVE, '5'},
    /*6*/{11, KANNADA_C_YA, KANNADA_C_RA, KANNADA_C_LA, KANNADA_C_VA, KANNADA_C_SHA, KANNADA_C_SSA, KANNADA_C_SA, KANNADA_C_HA, /*KANNADA_C_RRA, */ KANNADA_C_LLA, KANNADA_D_SIX, '6'},
    /*7*/{15, KANNADA_IV_A, KANNADA_IV_AA, KANNADA_IV_I, KANNADA_IV_II, KANNADA_IV_U, KANNADA_IV_UU, KANNADA_IV_VR, /*KANNADA_IV_VL, */ KANNADA_IV_E, KANNADA_IV_EE, KANNADA_IV_AI, KANNADA_IV_O, KANNADA_IV_OO, KANNADA_IV_AU, KANNADA_D_SEVEN, '7'},
    /*8*/{ 2, KANNADA_D_EIGHT, '8'},
    /*9*/{ 4, KANNADA_S_ANUSVARA, KANNADA_S_VISARGA, KANNADA_D_NINE, '9'},
    /*0*/{ 7, ' ', KANNADA_S_JIHVAMULIYA, KANNADA_S_UPADHMANIYA, KANNADA_D_ZERO, KANNADA_DEVANAGARI_S_DANDA, KANNADA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 印度马拉雅拉姆语
#ifdef IM_MALAYALAM_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_malayalam = 
{
    /*1*/ {8, 0x0D15, 0x0D16, 0x0D17, 0x0D18, 0x0D19, 0x0D67, 0x0031, 0x0D7F},
    /*2*/ {7, 0x0D1A, 0x0D1B, 0x0D1C, 0x0D1D, 0x0D1E, 0x0D68, 0x0032},
    /*3*/ {8, 0x0D1F, 0x0D20, 0x0D21, 0x0D22, 0x0D23, 0x0D69, 0x0033, 0x0D7A},
    /*4*/ {8, 0x0D24, 0x0D25, 0x0D26, 0x0D27, 0x0D28, 0x0D6A, 0x0034, 0x0D7B},
    /*5*/ {7, 0x0D2A, 0x0D2B, 0x0D2C, 0x0D2D, 0x0D2E, 0x0D6B, 0x0035},
    /*6*/ {16, 0x0D2F, 0x0D30, 0x0D32, 0x0D35, 0x0D36, 0x0D37, 0x0D38, 0x0D39, 0x0D31, 0x0D33, 0x0D34, 0x0D6C, 0x0036, 0x0D7C, 0x0D7D, 0x0D7E},
    /*7*/ {15, 0x0D05, 0x0D06, 0x0D07, 0x0D08, 0x0D09, 0x0D0A, 0x0D0B, 0x0D0E, 0x0D0F, 0x0D10, 0x0D12, 0x0D13, 0x0D14, 0x0D6D, 0x0037},
    /*8*/ {15, 0x0D6E, 0x0038, 0x0D3E, 0x0D3F, 0x0D40, 0x0D41, 0x0D42, 0x0D43, 0x0D44, 0x0D46, 0x0D47, 0x0D48, 0x0D4A, 0x0D4B, 0x0D4C},
    /*9*/ {5, 0x0D6F, 0x0039, 0x0D02, 0x0D03, 0x0D4D},
    /*0*/ {5, 0x0020, 0x0D66, 0x0964, 0x0965, 0x0030},
    /***/{0},
    /*#*/{0}
};
#else

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_malayalam =
{
    /*1*/{ 8, MALAYALAM_C_KA, MALAYALAM_C_KHA, MALAYALAM_C_GA, MALAYALAM_C_GHA, MALAYALAM_C_NGA, MALAYALAM_D_ONE, '1', MALAYALAM_C_CHILLU_K},
    /*2*/{ 7, MALAYALAM_C_CA, MALAYALAM_C_CHA, MALAYALAM_C_JA, MALAYALAM_C_JHA, MALAYALAM_C_NYA, MALAYALAM_D_TWO, '2'},
    /*3*/{ 8, MALAYALAM_C_TTA, MALAYALAM_C_TTHA, MALAYALAM_C_DDA, MALAYALAM_C_DDHA, MALAYALAM_C_NNA, MALAYALAM_D_THREE, '3', MALAYALAM_C_CHILLU_NN},
    /*4*/{ 8, MALAYALAM_C_TA, MALAYALAM_C_THA, MALAYALAM_C_DA, MALAYALAM_C_DHA, MALAYALAM_C_NA, MALAYALAM_D_FOUR, '4', MALAYALAM_C_CHILLU_N},
    /*5*/{ 7, MALAYALAM_C_PA, MALAYALAM_C_PHA, MALAYALAM_C_BA, MALAYALAM_C_BHA, MALAYALAM_C_MA, MALAYALAM_D_FIVE, '5'},
    /*6*/{16, MALAYALAM_C_YA, MALAYALAM_C_RA, MALAYALAM_C_LA, MALAYALAM_C_VA, MALAYALAM_C_SHA, MALAYALAM_C_SSA, MALAYALAM_C_SA, MALAYALAM_C_HA, MALAYALAM_C_RRA, MALAYALAM_C_LLA, MALAYALAM_C_LLLA, MALAYALAM_D_SIX, '6', MALAYALAM_C_CHILLU_RR, MALAYALAM_C_CHILLU_L, MALAYALAM_C_CHILLU_LL},
    /*7*/{15, MALAYALAM_IV_A, MALAYALAM_IV_AA, MALAYALAM_IV_I, MALAYALAM_IV_II, MALAYALAM_IV_U, MALAYALAM_IV_UU, MALAYALAM_IV_VR, MALAYALAM_IV_E, MALAYALAM_IV_EE, MALAYALAM_IV_AI, MALAYALAM_IV_O, MALAYALAM_IV_OO, MALAYALAM_IV_AU, MALAYALAM_D_SEVEN, '7'},
    /*8*/{ 2, MALAYALAM_D_EIGHT, '8'},
    /*9*/{ 2, MALAYALAM_D_NINE, '9'},
    /*0*/{ 5, ' ', MALAYALAM_D_ZERO, MALAYALAM_DEVANAGARI_S_DANDA, MALAYALAM_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_malayalammatra =
{
    /*1*/{ 8, MALAYALAM_C_KA, MALAYALAM_C_KHA, MALAYALAM_C_GA, MALAYALAM_C_GHA, MALAYALAM_C_NGA, MALAYALAM_D_ONE, '1', MALAYALAM_C_CHILLU_K},
    /*2*/{ 7, MALAYALAM_C_CA, MALAYALAM_C_CHA, MALAYALAM_C_JA, MALAYALAM_C_JHA, MALAYALAM_C_NYA, MALAYALAM_D_TWO, '2'},
    /*3*/{ 8, MALAYALAM_C_TTA, MALAYALAM_C_TTHA, MALAYALAM_C_DDA, MALAYALAM_C_DDHA, MALAYALAM_C_NNA, MALAYALAM_D_THREE, '3', MALAYALAM_C_CHILLU_NN},
    /*4*/{ 8, MALAYALAM_C_TA, MALAYALAM_C_THA, MALAYALAM_C_DA, MALAYALAM_C_DHA, MALAYALAM_C_NA, MALAYALAM_D_FOUR, '4', MALAYALAM_C_CHILLU_N},
    /*5*/{ 7, MALAYALAM_C_PA, MALAYALAM_C_PHA, MALAYALAM_C_BA, MALAYALAM_C_BHA, MALAYALAM_C_MA, MALAYALAM_D_FIVE, '5'},
    /*6*/{16, MALAYALAM_C_YA, MALAYALAM_C_RA, MALAYALAM_C_LA, MALAYALAM_C_VA, MALAYALAM_C_SHA, MALAYALAM_C_SSA, MALAYALAM_C_SA, MALAYALAM_C_HA, MALAYALAM_C_RRA, MALAYALAM_C_LLA, MALAYALAM_C_LLLA, MALAYALAM_D_SIX, '6', MALAYALAM_C_CHILLU_RR, MALAYALAM_C_CHILLU_L, MALAYALAM_C_CHILLU_LL},
    /*7*/{15, MALAYALAM_IV_A, MALAYALAM_IV_AA, MALAYALAM_IV_I, MALAYALAM_IV_II, MALAYALAM_IV_U, MALAYALAM_IV_UU, MALAYALAM_IV_VR, MALAYALAM_IV_E, MALAYALAM_IV_EE, MALAYALAM_IV_AI, MALAYALAM_IV_O, MALAYALAM_IV_OO, MALAYALAM_IV_AU, MALAYALAM_D_SEVEN, '7'},
    /*8*/{15, MALAYALAM_DV_AA, MALAYALAM_DV_I, MALAYALAM_DV_II, MALAYALAM_DV_U, MALAYALAM_DV_UU, MALAYALAM_DV_VR, MALAYALAM_DV_VRR, MALAYALAM_DV_E, MALAYALAM_DV_EE, MALAYALAM_DV_AI, MALAYALAM_DV_O, MALAYALAM_DV_OO, MALAYALAM_DV_AU, MALAYALAM_D_EIGHT, '8'},
    /*9*/{ 5, MALAYALAM_S_ANUSVARA, MALAYALAM_S_VISARGA, MALAYALAM_S_HALANT, MALAYALAM_D_NINE, '9'},
    /*0*/{ 5, ' ', MALAYALAM_D_ZERO, MALAYALAM_DEVANAGARI_S_DANDA, MALAYALAM_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_malayalammatra1 =
{
    /*1*/{ 8, MALAYALAM_C_KA, MALAYALAM_C_KHA, MALAYALAM_C_GA, MALAYALAM_C_GHA, MALAYALAM_C_NGA, MALAYALAM_D_ONE, '1', MALAYALAM_C_CHILLU_K},
    /*2*/{ 7, MALAYALAM_C_CA, MALAYALAM_C_CHA, MALAYALAM_C_JA, MALAYALAM_C_JHA, MALAYALAM_C_NYA, MALAYALAM_D_TWO, '2'},
    /*3*/{ 8, MALAYALAM_C_TTA, MALAYALAM_C_TTHA, MALAYALAM_C_DDA, MALAYALAM_C_DDHA, MALAYALAM_C_NNA, MALAYALAM_D_THREE, '3', MALAYALAM_C_CHILLU_NN},
    /*4*/{ 8, MALAYALAM_C_TA, MALAYALAM_C_THA, MALAYALAM_C_DA, MALAYALAM_C_DHA, MALAYALAM_C_NA, MALAYALAM_D_FOUR, '4', MALAYALAM_C_CHILLU_N},
    /*5*/{ 7, MALAYALAM_C_PA, MALAYALAM_C_PHA, MALAYALAM_C_BA, MALAYALAM_C_BHA, MALAYALAM_C_MA, MALAYALAM_D_FIVE, '5'},
    /*6*/{16, MALAYALAM_C_YA, MALAYALAM_C_RA, MALAYALAM_C_LA, MALAYALAM_C_VA, MALAYALAM_C_SHA, MALAYALAM_C_SSA, MALAYALAM_C_SA, MALAYALAM_C_HA, MALAYALAM_C_RRA, MALAYALAM_C_LLA, MALAYALAM_C_LLLA, MALAYALAM_D_SIX, '6', MALAYALAM_C_CHILLU_RR, MALAYALAM_C_CHILLU_L, MALAYALAM_C_CHILLU_LL},
    /*7*/{15, MALAYALAM_IV_A, MALAYALAM_IV_AA, MALAYALAM_IV_I, MALAYALAM_IV_II, MALAYALAM_IV_U, MALAYALAM_IV_UU, MALAYALAM_IV_VR, MALAYALAM_IV_E, MALAYALAM_IV_EE, MALAYALAM_IV_AI, MALAYALAM_IV_O, MALAYALAM_IV_OO, MALAYALAM_IV_AU, MALAYALAM_D_SEVEN, '7'},
    /*8*/{ 2, MALAYALAM_D_EIGHT, '8'},
    /*9*/{ 4, MALAYALAM_S_ANUSVARA, MALAYALAM_S_VISARGA, MALAYALAM_D_NINE, '9'},
    /*0*/{ 5, ' ', MALAYALAM_D_ZERO, MALAYALAM_DEVANAGARI_S_DANDA, MALAYALAM_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

//缅甸
#ifdef IM_MYANMAR_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_myanmar = 	//cstar没有提供智能输入法，切给出的键码表存在较大问题，所以直接使用sogou的键码表
{
    /*1*/  {14, 0x1031, 0x102C, 0x102D, 0x102E, 0x102B, 0x103A, 0x1036, 0x1032, 0x1039, 0x104A, 0x104B, 0x1085, 0x1041, 0x0031},
    /*2*/  {7, 0x1000, 0x1001, 0x1004, 0x1002, 0x1003, 0x1042, 0x0032},
    /*3*/  {8, 0x1005, 0x1006, 0x100A, 0x1008, 0x1007, 0x1009, 0x1043, 0x0033},
    /*4*/  {7, 0x100F, 0x100C, 0x100B, 0x100D, 0x100E, 0x1044, 0x0034},
    /*5*/  {7, 0x1014, 0x1010, 0x1011, 0x1012, 0x1013, 0x1045, 0x0035},
    /*6*/  {7, 0x1019, 0x1015, 0x1016, 0x1017, 0x1018, 0x1046, 0x0036},
    /*7*/  {9, 0x101C, 0x101A, 0x101B, 0x101D, 0x104C, 0x104D, 0x104F, 0x1047, 0x0037},
    /*8*/  {14, 0x101E, 0x101F, 0x1021, 0x1025, 0x1020, 0x103F, 0x1023, 0x1024, 0x1026, 0x1027, 0x1029, 0x102A, 0x1048, 0x0038},
    /*9*/  {10, 0x103E, 0x102F, 0x1030, 0x103B, 0x103C, 0x103D, 0x1037, 0x1038, 0x1049, 0x0039},
    /*0*/  {2, 0x1040, 0x0030},
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_myanmar =
{
    /*1*/  {14, 0x1031, 0x102C, 0x102D, 0x102E, 0x102B, 0x103A, 0x1036, 0x1032, 0x1039, 0x104A, 0x104B, 0x1085, 0x1041, 0x0031},
    /*2*/  {7, 0x1000, 0x1001, 0x1004, 0x1002, 0x1003, 0x1042, 0x0032},
    /*3*/  {8, 0x1005, 0x1006, 0x100A, 0x1008, 0x1007, 0x1009, 0x1043, 0x0033},
    /*4*/  {7, 0x100F, 0x100C, 0x100B, 0x100D, 0x100E, 0x1044, 0x0034},
    /*5*/  {7, 0x1014, 0x1010, 0x1011, 0x1012, 0x1013, 0x1045, 0x0035},
    /*6*/  {7, 0x1019, 0x1015, 0x1016, 0x1017, 0x1018, 0x1046, 0x0036},
    /*7*/  {9, 0x101C, 0x101A, 0x101B, 0x101D, 0x104C, 0x104D, 0x104F, 0x1047, 0x0037},
    /*8*/  {14, 0x101E, 0x101F, 0x1021, 0x1025, 0x1020, 0x103F, 0x1023, 0x1024, 0x1026, 0x1027, 0x1029, 0x102A, 0x1048, 0x0038},
    /*9*/  {10, 0x103E, 0x102F, 0x1030, 0x103B, 0x103C, 0x103D, 0x1037, 0x1038, 0x1049, 0x0039},
    /*0*/  {2, 0x1040, 0x0030},
};
#else	
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_myanmar = 
{
    /*1*/  {14, 0x1031, 0x102C, 0x102D, 0x102E, 0x102B, 0x103A, 0x1036, 0x1032, 0x1039, 0x104A, 0x104B, 0x1085, 0x1041, 0x0031},
    /*2*/  {7, 0x1000, 0x1001, 0x1004, 0x1002, 0x1003, 0x1042, 0x0032},
    /*3*/  {8, 0x1005, 0x1006, 0x100A, 0x1008, 0x1007, 0x1009, 0x1043, 0x0033},
    /*4*/  {7, 0x100F, 0x100C, 0x100B, 0x100D, 0x100E, 0x1044, 0x0034},
    /*5*/  {7, 0x1014, 0x1010, 0x1011, 0x1012, 0x1013, 0x1045, 0x0035},
    /*6*/  {7, 0x1019, 0x1015, 0x1016, 0x1017, 0x1018, 0x1046, 0x0036},
    /*7*/  {9, 0x101C, 0x101A, 0x101B, 0x101D, 0x104C, 0x104D, 0x104F, 0x1047, 0x0037},
    /*8*/  {14, 0x101E, 0x101F, 0x1021, 0x1025, 0x1020, 0x103F, 0x1023, 0x1024, 0x1026, 0x1027, 0x1029, 0x102A, 0x1048, 0x0038},
    /*9*/  {10, 0x103E, 0x102F, 0x1030, 0x103B, 0x103C, 0x103D, 0x1037, 0x1038, 0x1049, 0x0039},
    /*0*/  {2, 0x1040, 0x0030},
};
#endif//S9  
#endif//IM_MYANMAR_SUPPORT

// 奥里雅印度语
#ifdef IM_ORIYA_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_oriya = 
{
    /*1*/ {7, 0x0b15, 0x0b16, 0x0b17, 0x0b18, 0x0b19, 0x0b67, 0x0031},
    /*2*/ {7, 0x0b1a, 0x0b1b, 0x0b1c, 0x0b1d, 0x0b1e, 0x0b68, 0x0032},
    /*3*/ {7, 0x0b1f, 0x0b20, 0x0b21, 0x0b22, 0x0b23, 0x0b69, 0x0033},
    /*4*/ {7, 0x0b24, 0x0b25, 0x0b26, 0x0b27, 0x0b28, 0x0b6a, 0x0034},
    /*5*/ {7, 0x0b2a, 0x0b2b, 0x0b2c, 0x0b2d, 0x0b2e, 0x0b6b, 0x0035},
    /*6*/ {11, 0x0b2f, 0x0b30, 0x0b32, 0x0b33, 0x0b35, 0x0b5f, 0x0b5c, 0x0b5d, 0x0b71, 0x0b6c, 0x0036},
    /*7*/ {7, 0x0b36, 0x0b37, 0x0b38, 0x0b39, 0x0b70, 0x0b6d, 0x0037},
    /*8*/ {14, 0x0b05, 0x0b06, 0x0b07, 0x0b08, 0x0b09, 0x0b0a, 0x0b0b, 0x0b0c, 0x0b0f, 0x0b10, 0x0b13, 0x0b14, 0x0b6e, 0x0038},
    /*9*/ {13, 0x0b6f, 0x0039, 0x0b3e, 0x0b3f, 0x0b40, 0x0b41, 0x0b42, 0x0b43, 0x0b44, 0x0b47, 0x0b48, 0x0b4b, 0x0b4c},
    /*0*/ {11, 0x0020, 0x002e, 0x0b66, 0x0964, 0x0965, 0x0030, 0x0b4d, 0x0b01, 0x0b02, 0x0b03, 0x0b3d},
    /***/{0},
    /*#*/{0}
};
#else

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_oriya =
{
    /*1*/{ 7, ORIYA_C_KA, ORIYA_C_KHA, ORIYA_C_GA, ORIYA_C_GHA, ORIYA_C_NGA, ORIYA_D_ONE, '1'},
    /*2*/{ 7, ORIYA_C_CHA, ORIYA_C_CHHA, ORIYA_C_JA, ORIYA_C_JHA, ORIYA_C_NYA, ORIYA_D_TWO, '2'},
    /*3*/{ 7, ORIYA_C_TTA, ORIYA_C_TTHA, ORIYA_C_DDA, ORIYA_C_DDHA, ORIYA_C_NNA, ORIYA_D_THREE, '3'},
    /*4*/{ 7, ORIYA_C_TA, ORIYA_C_THA, ORIYA_C_DA, ORIYA_C_DHA, ORIYA_C_NA, ORIYA_D_FOUR, '4'},
    /*5*/{ 7, ORIYA_C_PA, ORIYA_C_PHA, ORIYA_C_BA, ORIYA_C_BHA, ORIYA_C_MA, ORIYA_D_FIVE, '5'},
    /*6*/{11, ORIYA_C_YA, ORIYA_C_RA, ORIYA_C_LA, ORIYA_C_LLA, ORIYA_C_VA, ORIYA_AC_YYA, ORIYA_AC_RRA, ORIYA_AC_RHA, ORIYA_S_WA, ORIYA_D_SIX, '6'},
    /*7*/{ 7, ORIYA_C_SHA, ORIYA_C_SSA, ORIYA_C_SA, ORIYA_C_HA, ORIYA_S_ISSHAR, ORIYA_D_SEVEN, '7'},
    /*8*/{14, ORIYA_IV_A, ORIYA_IV_AA, ORIYA_IV_I, ORIYA_IV_II, ORIYA_IV_U, ORIYA_IV_UU, ORIYA_IV_R, ORIYA_IV_L, ORIYA_IV_E, ORIYA_IV_AI, ORIYA_IV_O, ORIYA_IV_AU, ORIYA_D_EIGHT, '8'},
    /*9*/{ 2, ORIYA_D_NINE, '9'},
    /*0*/{ 6, ' ', '.', ORIYA_D_ZERO, ORIYA_DEVANAGARI_S_DANDA, ORIYA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_oriyamatra =
{
    /*1*/{ 7, ORIYA_C_KA, ORIYA_C_KHA, ORIYA_C_GA, ORIYA_C_GHA, ORIYA_C_NGA, ORIYA_D_ONE, '1'},
    /*2*/{ 7, ORIYA_C_CHA, ORIYA_C_CHHA, ORIYA_C_JA, ORIYA_C_JHA, ORIYA_C_NYA, ORIYA_D_TWO, '2'},
    /*3*/{ 7, ORIYA_C_TTA, ORIYA_C_TTHA, ORIYA_C_DDA, ORIYA_C_DDHA, ORIYA_C_NNA, ORIYA_D_THREE, '3'},
    /*4*/{ 7, ORIYA_C_TA, ORIYA_C_THA, ORIYA_C_DA, ORIYA_C_DHA, ORIYA_C_NA, ORIYA_D_FOUR, '4'},
    /*5*/{ 7, ORIYA_C_PA, ORIYA_C_PHA, ORIYA_C_BA, ORIYA_C_BHA, ORIYA_C_MA, ORIYA_D_FIVE, '5'},
    /*6*/{11, ORIYA_C_YA, ORIYA_C_RA, ORIYA_C_LA, ORIYA_C_LLA, ORIYA_C_VA, ORIYA_AC_YYA, ORIYA_AC_RRA, ORIYA_AC_RHA, ORIYA_S_WA, ORIYA_D_SIX, '6'},
    /*7*/{ 7, ORIYA_C_SHA, ORIYA_C_SSA, ORIYA_C_SA, ORIYA_C_HA, ORIYA_S_ISSHAR, ORIYA_D_SEVEN, '7'},
    /*8*/{14, ORIYA_IV_A, ORIYA_IV_AA, ORIYA_IV_I, ORIYA_IV_II, ORIYA_IV_U, ORIYA_IV_UU, ORIYA_IV_R, ORIYA_IV_L, ORIYA_IV_E, ORIYA_IV_AI, ORIYA_IV_O, ORIYA_IV_AU, ORIYA_D_EIGHT, '8'},
    /*9*/{13, ORIYA_DV_AA, ORIYA_DV_I, ORIYA_DV_II, ORIYA_DV_U, ORIYA_DV_UU, ORIYA_DV_R, ORIYA_DV_RR, ORIYA_DV_E, ORIYA_DV_AI, ORIYA_DV_O, ORIYA_DV_AU, ORIYA_D_NINE, '9'},
    /*0*/{11, ' ', ORIYA_S_HALANT, ORIYA_S_CHANDRA_BINDU, ORIYA_S_ANUSVARA, ORIYA_S_VISARGA, ORIYA_NS_AVAGRAHA, '.', ORIYA_D_ZERO, ORIYA_DEVANAGARI_S_DANDA, ORIYA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_oriyamatra1 =
{
    /*1*/{ 7, ORIYA_C_KA, ORIYA_C_KHA, ORIYA_C_GA, ORIYA_C_GHA, ORIYA_C_NGA, ORIYA_D_ONE, '1'},
    /*2*/{ 7, ORIYA_C_CHA, ORIYA_C_CHHA, ORIYA_C_JA, ORIYA_C_JHA, ORIYA_C_NYA, ORIYA_D_TWO, '2'},
    /*3*/{ 7, ORIYA_C_TTA, ORIYA_C_TTHA, ORIYA_C_DDA, ORIYA_C_DDHA, ORIYA_C_NNA, ORIYA_D_THREE, '3'},
    /*4*/{ 7, ORIYA_C_TA, ORIYA_C_THA, ORIYA_C_DA, ORIYA_C_DHA, ORIYA_C_NA, ORIYA_D_FOUR, '4'},
    /*5*/{ 7, ORIYA_C_PA, ORIYA_C_PHA, ORIYA_C_BA, ORIYA_C_BHA, ORIYA_C_MA, ORIYA_D_FIVE, '5'},
    /*6*/{11, ORIYA_C_YA, ORIYA_C_RA, ORIYA_C_LA, ORIYA_C_LLA, ORIYA_C_VA, ORIYA_AC_YYA, ORIYA_AC_RRA, ORIYA_AC_RHA, ORIYA_S_WA, ORIYA_D_SIX, '6'},
    /*7*/{ 7, ORIYA_C_SHA, ORIYA_C_SSA, ORIYA_C_SA, ORIYA_C_HA, ORIYA_S_ISSHAR, ORIYA_D_SEVEN, '7'},
    /*8*/{14, ORIYA_IV_A, ORIYA_IV_AA, ORIYA_IV_I, ORIYA_IV_II, ORIYA_IV_U, ORIYA_IV_UU, ORIYA_IV_R, ORIYA_IV_L, ORIYA_IV_E, ORIYA_IV_AI, ORIYA_IV_O, ORIYA_IV_AU, ORIYA_D_EIGHT, '8'},
    /*9*/{ 2, ORIYA_D_NINE, '9'},
    /*0*/{10, ' ', ORIYA_S_CHANDRA_BINDU, ORIYA_S_ANUSVARA, ORIYA_S_VISARGA, ORIYA_NS_AVAGRAHA, '.', ORIYA_D_ZERO, ORIYA_DEVANAGARI_S_DANDA, ORIYA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_oriyamatra2 =
{
    /*1*/{ 7, ORIYA_C_KA, ORIYA_C_KHA, ORIYA_C_GA, ORIYA_C_GHA, ORIYA_C_NGA, ORIYA_D_ONE, '1'},
    /*2*/{ 7, ORIYA_C_CHA, ORIYA_C_CHHA, ORIYA_C_JA, ORIYA_C_JHA, ORIYA_C_NYA, ORIYA_D_TWO, '2'},
    /*3*/{ 7, ORIYA_C_TTA, ORIYA_C_TTHA, ORIYA_C_DDA, ORIYA_C_DDHA, ORIYA_C_NNA, ORIYA_D_THREE, '3'},
    /*4*/{ 7, ORIYA_C_TA, ORIYA_C_THA, ORIYA_C_DA, ORIYA_C_DHA, ORIYA_C_NA, ORIYA_D_FOUR, '4'},
    /*5*/{ 7, ORIYA_C_PA, ORIYA_C_PHA, ORIYA_C_BA, ORIYA_C_BHA, ORIYA_C_MA, ORIYA_D_FIVE, '5'},
    /*6*/{11, ORIYA_C_YA, ORIYA_C_RA, ORIYA_C_LA, ORIYA_C_LLA, ORIYA_C_VA, ORIYA_AC_YYA, ORIYA_AC_RRA, ORIYA_AC_RHA, ORIYA_S_WA, ORIYA_D_SIX, '6'},
    /*7*/{ 7, ORIYA_C_SHA, ORIYA_C_SSA, ORIYA_C_SA, ORIYA_C_HA, ORIYA_S_ISSHAR, ORIYA_D_SEVEN, '7'},
    /*8*/{14, ORIYA_IV_A, ORIYA_IV_AA, ORIYA_IV_I, ORIYA_IV_II, ORIYA_IV_U, ORIYA_IV_UU, ORIYA_IV_R, ORIYA_IV_L, ORIYA_IV_E, ORIYA_IV_AI, ORIYA_IV_O, ORIYA_IV_AU, ORIYA_D_EIGHT, '8'},
    /*9*/{ 2, ORIYA_D_NINE, '9'},
    /*0*/{ 9, ' ', ORIYA_S_ANUSVARA, ORIYA_S_VISARGA, ORIYA_NS_AVAGRAHA, '.', ORIYA_D_ZERO, ORIYA_DEVANAGARI_S_DANDA, ORIYA_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 印度旁遮普语
#ifdef IM_PUNJABI_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_punjabi = 
{
    /*1*/ {9, 0x0a15, 0x0a16, 0x0a17, 0x0a18, 0x0a19, 0x0a59, 0x0a5a, 0x0a67, 0x0031},
    /*2*/ {8, 0x0a1a, 0x0a1b, 0x0a1c, 0x0a1d, 0x0a1e, 0x0a5b, 0x0a68, 0x0032},
    /*3*/ {7, 0x0a1f, 0x0a20, 0x0a21, 0x0a22, 0x0a23, 0x0a69, 0x0033},
    /*4*/ {7, 0x0a24, 0x0a25, 0x0a26, 0x0a27, 0x0a28, 0x0a6a, 0x0034},
    /*5*/ {8, 0x0a2a, 0x0a2b, 0x0a2c, 0x0a2d, 0x0a2e, 0x0a5e, 0x0a6b, 0x0035},
    /*6*/ {11, 0x0a2f, 0x0a30, 0x0a32, 0x0a35, 0x0a5c, 0x0a38, 0x0a39, 0x0a33, 0x0a36, 0x0a6c, 0x0036},
    /*7*/ {14, 0x0a05, 0x0a06, 0x0a07, 0x0a08, 0x0a09, 0x0a0a, 0x0a0f, 0x0a10, 0x0a13, 0x0a14, 0x0a72, 0x0a73, 0x0a6d, 0x0037},
    /*8*/ {11, 0x0a6e, 0x0038, 0x0a3e, 0x0a3f, 0x0a40, 0x0a41, 0x0a42, 0x0a47, 0x0a48, 0x0a4b, 0x0a4c},
    /*9*/ {8, 0x0a74, 0x0a6f, 0x0039, 0x0a03, 0x0a4d, 0x0a70, 0x0a71, 0x0a02},
    /*0*/ {5, 0x0020, 0x0964, 0x0a66, 0x0965, 0x0030},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_punjabi =
{
    /*1*/  {9, 0x0A01, 0x0A02, 0x0A03, 0x0A3C, 0x0A4D, 0x0A70, 0x0A71, 0x0A67, 0x0031},
    /*2*/  {9, 0x0A05, 0x0A06, 0x0A07, 0x0A08, 0x0A09, 0x0A72, 0x0A73, 0x0A68, 0x0032},
    /*3*/  {8, 0x0A0A, 0x0A0F, 0x0A10, 0x0A13, 0x0A14, 0x0A69, 0x0A74, 0x0033},
    /*4*/  {9, 0x0A15, 0x0A16, 0x0A17, 0x0A18, 0x0A19, 0x0A59, 0x0A5A, 0x0A6A, 0x0034},
    /*5*/  {10, 0x0A1A, 0x0A1B, 0x0A1C, 0x0A1D, 0x0A1E, 0x0A1F, 0x0A20, 0x0A5B, 0x0A6B, 0x0035},
    /*6*/  {10, 0x0A21, 0x0A22, 0x0A23, 0x0A24, 0x0A25, 0x0A26, 0x0A27, 0x0A28, 0x0A6C, 0x0036},
    /*7*/  {10, 0x0A2A, 0x0A2B, 0x0A2C, 0x0A2D, 0x0A2E, 0x0A2F, 0x0A30, 0x0A5E, 0x0A6D, 0x0037},
    /*8*/  {9, 0x0A32, 0x0A33, 0x0A35, 0x0A36, 0x0A38, 0x0A39, 0x0A5C, 0x0A6E, 0x0038},
    /*9*/  {11, 0x0A3E, 0x0A3F, 0x0A40, 0x0A41, 0x0A42, 0x0A47, 0x0A48, 0x0A4B, 0x0A4C, 0x0A6F, 0x0039},
    /*0*/  {2, 0x0A66, 0x0030},
};
#else

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_punjabi =
{
    /*1*/{ 9, PUNJABI_C_KA, PUNJABI_C_KHA, PUNJABI_C_GA, PUNJABI_C_GHA, PUNJABI_C_NGA, PUNJABI_AC_KHHA, PUNJABI_AC_GHHA, PUNJABI_D_ONE, '1'},
    /*2*/{ 8, PUNJABI_C_CA, PUNJABI_C_CHA, PUNJABI_C_JA, PUNJABI_C_JHA, PUNJABI_C_NYA, PUNJABI_AC_ZA, PUNJABI_D_TWO, '2'},
    /*3*/{ 7, PUNJABI_C_TTA, PUNJABI_C_TTHA, PUNJABI_C_DDA, PUNJABI_C_DDHA, PUNJABI_C_NNA, PUNJABI_D_THREE, '3'},
    /*4*/{ 7, PUNJABI_C_TA, PUNJABI_C_THA, PUNJABI_C_DA, PUNJABI_C_DHA, PUNJABI_C_NA, PUNJABI_D_FOUR, '4'},
    /*5*/{ 8, PUNJABI_C_PA, PUNJABI_C_PHA, PUNJABI_C_BA, PUNJABI_C_BHA, PUNJABI_C_MA, PUNJABI_AC_FA, PUNJABI_D_FIVE, '5'},
    /*6*/{11, PUNJABI_C_YA, PUNJABI_C_RA, PUNJABI_C_LA, PUNJABI_C_VA, PUNJABI_AC_RRA, PUNJABI_C_SA, PUNJABI_C_HA, PUNJABI_C_LLA, PUNJABI_C_SHA, PUNJABI_D_SIX, '6'},
    /*7*/{14, PUNJABI_IV_A, PUNJABI_IV_AA, PUNJABI_IV_I, PUNJABI_IV_II, PUNJABI_IV_U, PUNJABI_IV_UU, PUNJABI_IV_EE, PUNJABI_IV_AI, PUNJABI_IV_OO, PUNJABI_IV_AU, PUNJABI_AC_IRI, PUNJABI_AC_URA, PUNJABI_D_SEVEN, '7'},
    /*8*/{ 2, PUNJABI_D_EIGHT, '8'},
    /*9*/{ 3, PUNJABI_AC_ONKAR, PUNJABI_D_NINE, '9'},
    /*0*/{ 5, ' ', PUNJABI_DEVANAGARI_S_DANDA, PUNJABI_D_ZERO, PUNJABI_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_punjabimatra =
{
    /*1*/{ 9, PUNJABI_C_KA, PUNJABI_C_KHA, PUNJABI_C_GA, PUNJABI_C_GHA, PUNJABI_C_NGA, PUNJABI_AC_KHHA, PUNJABI_AC_GHHA, PUNJABI_D_ONE, '1'},
    /*2*/{ 8, PUNJABI_C_CA, PUNJABI_C_CHA, PUNJABI_C_JA, PUNJABI_C_JHA, PUNJABI_C_NYA, PUNJABI_AC_ZA, PUNJABI_D_TWO, '2'},
    /*3*/{ 7, PUNJABI_C_TTA, PUNJABI_C_TTHA, PUNJABI_C_DDA, PUNJABI_C_DDHA, PUNJABI_C_NNA, PUNJABI_D_THREE, '3'},
    /*4*/{ 7, PUNJABI_C_TA, PUNJABI_C_THA, PUNJABI_C_DA, PUNJABI_C_DHA, PUNJABI_C_NA, PUNJABI_D_FOUR, '4'},
    /*5*/{ 8, PUNJABI_C_PA, PUNJABI_C_PHA, PUNJABI_C_BA, PUNJABI_C_BHA, PUNJABI_C_MA, PUNJABI_AC_FA, PUNJABI_D_FIVE, '5'},
    /*6*/{11, PUNJABI_C_YA, PUNJABI_C_RA, PUNJABI_C_LA, PUNJABI_C_VA, PUNJABI_AC_RRA, PUNJABI_C_SA, PUNJABI_C_HA, PUNJABI_C_LLA, PUNJABI_C_SHA, PUNJABI_D_SIX, '6'},
    /*7*/{14, PUNJABI_IV_A, PUNJABI_IV_AA, PUNJABI_IV_I, PUNJABI_IV_II, PUNJABI_IV_U, PUNJABI_IV_UU, PUNJABI_IV_EE, PUNJABI_IV_AI, PUNJABI_IV_OO, PUNJABI_IV_AU, PUNJABI_AC_IRI, PUNJABI_AC_URA, PUNJABI_D_SEVEN, '7'},
    /*8*/{11, PUNJABI_DV_AA, PUNJABI_DV_I, PUNJABI_DV_II, PUNJABI_DV_U, PUNJABI_DV_UU, PUNJABI_DV_EE, PUNJABI_DV_AI, PUNJABI_DV_OO, PUNJABI_DV_AU, PUNJABI_D_EIGHT, '8'},
    /*9*/{ 7, PUNJABI_S_VISARGA, PUNJABI_S_HALANT, PUNJABI_AC_TIPPI, PUNJABI_AC_ADDAK, PUNJABI_AC_ONKAR, PUNJABI_D_NINE, '9'},
    /*0*/{ 5, ' ', PUNJABI_DEVANAGARI_S_DANDA, PUNJABI_D_ZERO, PUNJABI_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_punjabimatra1 =
{
    /*1*/{ 9, PUNJABI_C_KA, PUNJABI_C_KHA, PUNJABI_C_GA, PUNJABI_C_GHA, PUNJABI_C_NGA, PUNJABI_AC_KHHA, PUNJABI_AC_GHHA, PUNJABI_D_ONE, '1'},
    /*2*/{ 8, PUNJABI_C_CA, PUNJABI_C_CHA, PUNJABI_C_JA, PUNJABI_C_JHA, PUNJABI_C_NYA, PUNJABI_AC_ZA, PUNJABI_D_TWO, '2'},
    /*3*/{ 7, PUNJABI_C_TTA, PUNJABI_C_TTHA, PUNJABI_C_DDA, PUNJABI_C_DDHA, PUNJABI_C_NNA, PUNJABI_D_THREE, '3'},
    /*4*/{ 7, PUNJABI_C_TA, PUNJABI_C_THA, PUNJABI_C_DA, PUNJABI_C_DHA, PUNJABI_C_NA, PUNJABI_D_FOUR, '4'},
    /*5*/{ 8, PUNJABI_C_PA, PUNJABI_C_PHA, PUNJABI_C_BA, PUNJABI_C_BHA, PUNJABI_C_MA, PUNJABI_AC_FA, PUNJABI_D_FIVE, '5'},
    /*6*/{11, PUNJABI_C_YA, PUNJABI_C_RA, PUNJABI_C_LA, PUNJABI_C_VA, PUNJABI_AC_RRA, PUNJABI_C_SA, PUNJABI_C_HA, PUNJABI_C_LLA, PUNJABI_C_SHA, PUNJABI_D_SIX, '6'},
    /*7*/{14, PUNJABI_IV_A, PUNJABI_IV_AA, PUNJABI_IV_I, PUNJABI_IV_II, PUNJABI_IV_U, PUNJABI_IV_UU, PUNJABI_IV_EE, PUNJABI_IV_AI, PUNJABI_IV_OO, PUNJABI_IV_AU, PUNJABI_AC_IRI, PUNJABI_AC_URA, PUNJABI_D_SEVEN, '7'},
    /*8*/{ 2, PUNJABI_D_EIGHT, '8'},
    /*9*/{ 6, PUNJABI_S_BINDI, PUNJABI_S_VISARGA, PUNJABI_AC_ADDAK, PUNJABI_AC_ONKAR, PUNJABI_D_NINE, '9'},
    /*0*/{ 5, ' ', PUNJABI_DEVANAGARI_S_DANDA, PUNJABI_D_ZERO, PUNJABI_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_punjabimatra2 =
{
    /*1*/{ 9, PUNJABI_C_KA, PUNJABI_C_KHA, PUNJABI_C_GA, PUNJABI_C_GHA, PUNJABI_C_NGA, PUNJABI_AC_KHHA, PUNJABI_AC_GHHA, PUNJABI_D_ONE, '1'},
    /*2*/{ 8, PUNJABI_C_CA, PUNJABI_C_CHA, PUNJABI_C_JA, PUNJABI_C_JHA, PUNJABI_C_NYA, PUNJABI_AC_ZA, PUNJABI_D_TWO, '2'},
    /*3*/{ 7, PUNJABI_C_TTA, PUNJABI_C_TTHA, PUNJABI_C_DDA, PUNJABI_C_DDHA, PUNJABI_C_NNA, PUNJABI_D_THREE, '3'},
    /*4*/{ 7, PUNJABI_C_TA, PUNJABI_C_THA, PUNJABI_C_DA, PUNJABI_C_DHA, PUNJABI_C_NA, PUNJABI_D_FOUR, '4'},
    /*5*/{ 8, PUNJABI_C_PA, PUNJABI_C_PHA, PUNJABI_C_BA, PUNJABI_C_BHA, PUNJABI_C_MA, PUNJABI_AC_FA, PUNJABI_D_FIVE, '5'},
    /*6*/{11, PUNJABI_C_YA, PUNJABI_C_RA, PUNJABI_C_LA, PUNJABI_C_VA, PUNJABI_AC_RRA, PUNJABI_C_SA, PUNJABI_C_HA, PUNJABI_C_LLA, PUNJABI_C_SHA, PUNJABI_D_SIX, '6'},
    /*7*/{14, PUNJABI_IV_A, PUNJABI_IV_AA, PUNJABI_IV_I, PUNJABI_IV_II, PUNJABI_IV_U, PUNJABI_IV_UU, PUNJABI_IV_EE, PUNJABI_IV_AI, PUNJABI_IV_OO, PUNJABI_IV_AU, PUNJABI_AC_IRI, PUNJABI_AC_URA, PUNJABI_D_SEVEN, '7'},
    /*8*/{ 2, PUNJABI_D_EIGHT, '8'},
    /*9*/{ 6, PUNJABI_S_VISARGA, PUNJABI_AC_TIPPI, PUNJABI_AC_ADDAK, PUNJABI_AC_ONKAR, PUNJABI_D_NINE, '9'},
    /*0*/{ 5, ' ', PUNJABI_DEVANAGARI_S_DANDA, PUNJABI_D_ZERO, PUNJABI_DEVANAGARI_S_DOUBLE_DANDA, '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 阿萨姆
#if defined (IM_ASSAMESE_SUPPORT)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_assamese =
{
    /*1*/{ 7, ASSAMESE_C_KA, ASSAMESE_C_KHA, ASSAMESE_C_GA, ASSAMESE_C_GHA, ASSAMESE_C_NGA, ASSAMESE_D_ONE, '1'},
    /*2*/{ 7, ASSAMESE_C_CA, ASSAMESE_C_CHA, ASSAMESE_C_JA, ASSAMESE_C_JHA, ASSAMESE_C_NYA, ASSAMESE_D_TWO, '2'},
    /*3*/{ 7, ASSAMESE_C_TTA, ASSAMESE_C_TTHA, ASSAMESE_C_DDA, ASSAMESE_C_DDHA, ASSAMESE_C_NNA, ASSAMESE_D_THREE, '3'},
    /*4*/{ 7, ASSAMESE_C_TA, ASSAMESE_C_THA, ASSAMESE_C_DA, ASSAMESE_C_DHA, ASSAMESE_C_NA, ASSAMESE_D_FOUR, '4'},
    /*5*/{ 7, ASSAMESE_C_PA, ASSAMESE_C_PHA, ASSAMESE_C_BA, ASSAMESE_C_BHA, ASSAMESE_C_MA, ASSAMESE_D_FIVE, '5'},
    /*6*/{ 6, ASSAMESE_C_YA, ASSAMESE_C_MRA, ASSAMESE_C_LA, ASSAMESE_C_LRA, ASSAMESE_D_SIX, '6'},
    /*7*/{10, ASSAMESE_C_SHA, ASSAMESE_C_SSA, ASSAMESE_C_SA, ASSAMESE_C_HA, ASSAMESE_C_RRA, ASSAMESE_C_RHA, ASSAMESE_C_YYA, ASSAMESE_AD_TA, ASSAMESE_D_SEVEN, '7'},
    /*8*/{14, ASSAMESE_IV_A, ASSAMESE_IV_AA, ASSAMESE_IV_I, ASSAMESE_IV_II, ASSAMESE_IV_U, ASSAMESE_IV_UU, ASSAMESE_IV_VR, ASSAMESE_IV_VL, ASSAMESE_IV_E, ASSAMESE_IV_AI, ASSAMESE_IV_O, ASSAMESE_IV_AU, ASSAMESE_D_EIGHT, '8'},
    /*9*/{ 2, ASSAMESE_D_NINE, '9'},
    /*0*/{ 5, ' ', ASSAMESE_DEVANAGARI_S_DANDA, ASSAMESE_DEVANAGARI_S_DDANDA, ASSAMESE_D_ZERO, '0'},
    /***/{ 0},
    /*#*/{ 0},
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_assamesematra =
{
    /*1*/{ 7, ASSAMESE_C_KA, ASSAMESE_C_KHA, ASSAMESE_C_GA, ASSAMESE_C_GHA, ASSAMESE_C_NGA, ASSAMESE_D_ONE, '1'},
    /*2*/{ 7, ASSAMESE_C_CA, ASSAMESE_C_CHA, ASSAMESE_C_JA, ASSAMESE_C_JHA, ASSAMESE_C_NYA, ASSAMESE_D_TWO, '2'},
    /*3*/{ 7, ASSAMESE_C_TTA, ASSAMESE_C_TTHA, ASSAMESE_C_DDA, ASSAMESE_C_DDHA, ASSAMESE_C_NNA, ASSAMESE_D_THREE, '3'},
    /*4*/{ 7, ASSAMESE_C_TA, ASSAMESE_C_THA, ASSAMESE_C_DA, ASSAMESE_C_DHA, ASSAMESE_C_NA, ASSAMESE_D_FOUR, '4'},
    /*5*/{ 7, ASSAMESE_C_PA, ASSAMESE_C_PHA, ASSAMESE_C_BA, ASSAMESE_C_BHA, ASSAMESE_C_MA, ASSAMESE_D_FIVE, '5'},
    /*6*/{ 6, ASSAMESE_C_YA, ASSAMESE_C_MRA, ASSAMESE_C_LA, ASSAMESE_C_LRA, ASSAMESE_D_SIX, '6'},
    /*7*/{10, ASSAMESE_C_SHA, ASSAMESE_C_SSA, ASSAMESE_C_SA, ASSAMESE_C_HA, ASSAMESE_C_RRA, ASSAMESE_C_RHA, ASSAMESE_C_YYA, ASSAMESE_AD_TA, ASSAMESE_D_SEVEN, '7'},
    /*8*/{14, ASSAMESE_IV_A, ASSAMESE_IV_AA, ASSAMESE_IV_I, ASSAMESE_IV_II, ASSAMESE_IV_U, ASSAMESE_IV_UU, ASSAMESE_IV_VR, ASSAMESE_IV_VL, ASSAMESE_IV_E, ASSAMESE_IV_AI, ASSAMESE_IV_O, ASSAMESE_IV_AU, ASSAMESE_D_EIGHT, '8'},
    /*9*/{12, ASSAMESE_DV_AA, ASSAMESE_DV_I, ASSAMESE_DV_II, ASSAMESE_DV_U, ASSAMESE_DV_UU, ASSAMESE_DV_VR, ASSAMESE_DV_E, ASSAMESE_DV_AI, ASSAMESE_DV_O, ASSAMESE_DV_AU, ASSAMESE_D_NINE, '9'},
    /*0*/{10, ' ', ASSAMESE_S_HALANT, ASSAMESE_S_CHANDRA_BINDU, ASSAMESE_S_ANUSVARA, ASSAMESE_S_VISARGA, ASSAMESE_S_AVAGRAH, ASSAMESE_DEVANAGARI_S_DANDA, ASSAMESE_DEVANAGARI_S_DDANDA, ASSAMESE_D_ZERO, '0'},
    /***/{ 0},
    /*#*/{ 0},
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_assamesematra1 =
{
    /*1*/{ 7, ASSAMESE_C_KA, ASSAMESE_C_KHA, ASSAMESE_C_GA, ASSAMESE_C_GHA, ASSAMESE_C_NGA, ASSAMESE_D_ONE, '1'},
    /*2*/{ 7, ASSAMESE_C_CA, ASSAMESE_C_CHA, ASSAMESE_C_JA, ASSAMESE_C_JHA, ASSAMESE_C_NYA, ASSAMESE_D_TWO, '2'},
    /*3*/{ 7, ASSAMESE_C_TTA, ASSAMESE_C_TTHA, ASSAMESE_C_DDA, ASSAMESE_C_DDHA, ASSAMESE_C_NNA, ASSAMESE_D_THREE, '3'},
    /*4*/{ 7, ASSAMESE_C_TA, ASSAMESE_C_THA, ASSAMESE_C_DA, ASSAMESE_C_DHA, ASSAMESE_C_NA, ASSAMESE_D_FOUR, '4'},
    /*5*/{ 7, ASSAMESE_C_PA, ASSAMESE_C_PHA, ASSAMESE_C_BA, ASSAMESE_C_BHA, ASSAMESE_C_MA, ASSAMESE_D_FIVE, '5'},
    /*6*/{ 6, ASSAMESE_C_YA, ASSAMESE_C_MRA, ASSAMESE_C_LA, ASSAMESE_C_LRA, ASSAMESE_D_SIX, '6'},
    /*7*/{10, ASSAMESE_C_SHA, ASSAMESE_C_SSA, ASSAMESE_C_SA, ASSAMESE_C_HA, ASSAMESE_C_RRA, ASSAMESE_C_RHA, ASSAMESE_C_YYA, ASSAMESE_AD_TA, ASSAMESE_D_SEVEN, '7'},
    /*8*/{14, ASSAMESE_IV_A, ASSAMESE_IV_AA, ASSAMESE_IV_I, ASSAMESE_IV_II, ASSAMESE_IV_U, ASSAMESE_IV_UU, ASSAMESE_IV_VR, ASSAMESE_IV_VL, ASSAMESE_IV_E, ASSAMESE_IV_AI, ASSAMESE_IV_O, ASSAMESE_IV_AU, ASSAMESE_D_EIGHT, '8'},
    /*9*/{ 2, ASSAMESE_D_NINE, '9'},
    /*0*/{ 9, ' ', ASSAMESE_S_CHANDRA_BINDU, ASSAMESE_S_ANUSVARA, ASSAMESE_S_VISARGA, ASSAMESE_S_AVAGRAH, ASSAMESE_DEVANAGARI_S_DANDA, ASSAMESE_DEVANAGARI_S_DDANDA, ASSAMESE_D_ZERO, '0'},
    /***/{ 0},
    /*#*/{ 0},
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_assamesematra2 =
{
    /*1*/{ 7, ASSAMESE_C_KA, ASSAMESE_C_KHA, ASSAMESE_C_GA, ASSAMESE_C_GHA, ASSAMESE_C_NGA, ASSAMESE_D_ONE, '1'},
    /*2*/{ 7, ASSAMESE_C_CA, ASSAMESE_C_CHA, ASSAMESE_C_JA, ASSAMESE_C_JHA, ASSAMESE_C_NYA, ASSAMESE_D_TWO, '2'},
    /*3*/{ 7, ASSAMESE_C_TTA, ASSAMESE_C_TTHA, ASSAMESE_C_DDA, ASSAMESE_C_DDHA, ASSAMESE_C_NNA, ASSAMESE_D_THREE, '3'},
    /*4*/{ 7, ASSAMESE_C_TA, ASSAMESE_C_THA, ASSAMESE_C_DA, ASSAMESE_C_DHA, ASSAMESE_C_NA, ASSAMESE_D_FOUR, '4'},
    /*5*/{ 7, ASSAMESE_C_PA, ASSAMESE_C_PHA, ASSAMESE_C_BA, ASSAMESE_C_BHA, ASSAMESE_C_MA, ASSAMESE_D_FIVE, '5'},
    /*6*/{ 6, ASSAMESE_C_YA, ASSAMESE_C_MRA, ASSAMESE_C_LA, ASSAMESE_C_LRA, ASSAMESE_D_SIX, '6'},
    /*7*/{10, ASSAMESE_C_SHA, ASSAMESE_C_SSA, ASSAMESE_C_SA, ASSAMESE_C_HA, ASSAMESE_C_RRA, ASSAMESE_C_RHA, ASSAMESE_C_YYA, ASSAMESE_AD_TA, ASSAMESE_D_SEVEN, '7'},
    /*8*/{14, ASSAMESE_IV_A, ASSAMESE_IV_AA, ASSAMESE_IV_I, ASSAMESE_IV_II, ASSAMESE_IV_U, ASSAMESE_IV_UU, ASSAMESE_IV_VR, ASSAMESE_IV_VL, ASSAMESE_IV_E, ASSAMESE_IV_AI, ASSAMESE_IV_O, ASSAMESE_IV_AU, ASSAMESE_D_EIGHT, '8'},
    /*9*/{ 2, ASSAMESE_D_NINE, '9'},
    /*0*/{ 8, ' ', ASSAMESE_S_ANUSVARA, ASSAMESE_S_VISARGA, ASSAMESE_S_AVAGRAH, ASSAMESE_DEVANAGARI_S_DANDA, ASSAMESE_DEVANAGARI_S_DDANDA, ASSAMESE_D_ZERO, '0'},
    /***/{ 0},
    /*#*/{ 0},
};
#endif

// 南非的公用荷兰语
#ifdef IM_AFRIKAANS_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_afrikaans_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{12, 'A', 'B', 'C', '2', 0x00C4, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C5, 0x00C6, 0x00C7},
    /*3*/{ 9, 'D', 'E', 'F', '3', 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00D0},
    /*4*/{ 8, 'G', 'H', 'I', '4', 0x00CC, 0x00CD, 0x00CE, 0x00CF},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{11, 'M', 'N', 'O', '6', 0x00D6, 0x00D8, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D1},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 8, 'T', 'U', 'V', '8', 0x00D9, 0x00DA, 0x00DB, 0x00DC},
    /*9*/{ 7, 'W', 'X', 'Y', 'Z', '9', 0x00DD, 0x00DE},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_afrikaans_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{12, 'a', 'b', 'c', '2', 0x00E4, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E5, 0x00E6, 0x00E7},
    /*3*/{ 9, 'd', 'e', 'f', '3', 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00F0},
    /*4*/{ 8, 'g', 'h', 'i', '4', 0x00EC, 0x00ED, 0x00EE, 0x00EF},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{11, 'm', 'n', 'o', '6', 0x00F6, 0x00F8, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F1},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 8, 't', 'u', 'v', '8', 0x00F9, 0x00FA, 0x00FB, 0x00FC},
    /*9*/{ 7, 'w', 'x', 'y', 'z', '9', 0x00FD, 0x00FE},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 阿尔巴尼亚语
#ifdef IM_ALBANIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_albanian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 'B', 'C', 0x00C7, '2'},
    /*3*/{ 5, 'D', 'E', 0x00CB, 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_albanian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'a', 'b', 'c', 0x00E7, '2'},
    /*3*/{ 5, 'd', 'e', 0x00EB, 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 亚美尼亚语
#ifdef IM_ARMENIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_armenian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{10, 0x0041, 0x0042, 0x0043, 0x0032, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536},
    /*3*/{ 9, 0x0044, 0x0045, 0x0046, 0x0033, 0x0537, 0x0538, 0x0539, 0x053A, 0x053B},
    /*4*/{10, 0x0047, 0x0048, 0x0049, 0x0034, 0x053C, 0x053D, 0x053E, 0x053F, 0x0540, 0x0541},
    /*5*/{10, 0x004A, 0x004B, 0x004C, 0x0035, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547},
    /*6*/{ 9, 0x004D, 0x004E, 0x004F, 0x0036, 0x0548, 0x0549, 0x054A, 0x054B, 0x054C},
    /*7*/{11, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x054D, 0x054E, 0x054F, 0x0550, 0x0551, 0x0552},
    /*8*/{ 8, 0x0054, 0x0055, 0x0056, 0x0038, 0x0553, 0x0554, 0x0555, 0x0556},
    /*9*/{12, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x0589, 0x055D, 0x055E, 0x055B, 0x055C, 0x058A, 0x055F},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_armenian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{10, 0x0061, 0x0062, 0x0063, 0x0032, 0x0561, 0x0562, 0x0563, 0x0564, 0x0565, 0x0566},
    /*3*/{ 9, 0x0064, 0x0065, 0x0066, 0x0033, 0x0567, 0x0568, 0x0569, 0x056A, 0x056B},
    /*4*/{10, 0x0067, 0x0068, 0x0069, 0x0034, 0x056C, 0x056D, 0x056E, 0x056F, 0x0570, 0x0571},
    /*5*/{10, 0x006A, 0x006B, 0x006C, 0x0035, 0x0572, 0x0573, 0x0574, 0x0575, 0x0576, 0x0577},
    /*6*/{ 9, 0x006D, 0x006E, 0x006F, 0x0036, 0x0578, 0x0579, 0x057A, 0x057B, 0x057C},
    /*7*/{11, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x057D, 0x057E, 0x057F, 0x0580, 0x0581, 0x0582},
    /*8*/{ 9, 0x0074, 0x0075, 0x0076, 0x0038, 0x0583, 0x0584, 0x0585, 0x0586, 0x0587},
    /*9*/{12, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x0589, 0x055D, 0x055E, 0x055B, 0x055C, 0x058A, 0x055F},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_armenian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 0x0531, 0x0532, 0x0533, 0x0534, '2', 0x0559, 0x055A},
    /*3*/{ 7, 0x0535, 0x0536, 0x0537, 0x0538, '3', 0x055B, 0x055C},
    /*4*/{ 8, 0x0539, 0x053A, 0x053B, 0x053C, 0x053D, '4', 0x055D, 0x055E},
    /*5*/{ 8, 0x053E, 0x053F, 0x0540, 0x0541, 0x0542, '5', 0x055F, 0x0589},
    /*6*/{ 8, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547, '6', 0x058A, 0xFB13},
    /*7*/{ 8, 0x0548, 0x0549, 0x054A, 0x054B, 0x054C, '7', 0xFB14, 0xFB15},
    /*8*/{ 8, 0x054D, 0x054E, 0x054F, 0x0550, 0x0551, '8', 0xFB16, 0xFB17},
    /*9*/{ 6, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_armenian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 0x0561, 0x0562, 0x0563, 0x0564, '2', 0x0559, 0x055A},
    /*3*/{ 7, 0x0565, 0x0566, 0x0567, 0x0568, '3', 0x055B, 0x055C},
    /*4*/{ 8, 0x0569, 0x056A, 0x056B, 0x056C, 0x056D, '4', 0x055D, 0x055E},
    /*5*/{ 8, 0x056E, 0x056F, 0x0570, 0x0571, 0x0572, '5', 0x055F, 0x0589},
    /*6*/{ 8, 0x0573, 0x0574, 0x0575, 0x0576, 0x0577, '6', 0x058A, 0xFB13},
    /*7*/{ 8, 0x0578, 0x0579, 0x057A, 0x057B, 0x057C, '7', 0xFB14, 0xFB15},
    /*8*/{ 8, 0x057D, 0x057E, 0x057F, 0x0580, 0x0581, '8', 0xFB16, 0xFB17},
    /*9*/{ 6, 0x0582, 0x0583, 0x0584, 0x0585, 0x0586, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 阿塞拜疆语
#ifdef IM_AZERBAIJANI_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_azerbaijani_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 'B', 'C', 0x00C7, '2'},
    /*3*/{ 5, 'D', 'E', 0x018F, 'F', '3'},
    /*4*/{ 7, 'G', 0x011E, 'H', 'X', 'I', 0x0130, '4'},
    /*5*/{ 5, 'J', 'K', 'Q', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0x00D6, '6'},
    /*7*/{ 5, 'P', 'R', 'S', 0x015E, '7'},
    /*8*/{ 5, 'T', 'U', 0x00DC, 'V', '8'},
    /*9*/{ 4, 'Y', 'Z', 'W', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_azerbaijani_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'a', 'b', 'c', 0x00E7, '2'},
    /*3*/{ 5, 'd', 'e', 0x0259, 'f', '3'},
    /*4*/{ 7, 'g', 0x011F, 'h', 'x', 0x0131, 'i', '4'},
    /*5*/{ 5, 'j', 'k', 'q', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0x00F6, '6'},
    /*7*/{ 5, 'p', 'r', 's', 0x015F, '7'},
    /*8*/{ 5, 't', 'u', 0x00FC, 'v', '8'},
    /*9*/{ 4, 'y', 'z', 'w', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 巴斯克语
#ifdef IM_BASQUE_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_basque_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'A', 'B', 'C', '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0xD1, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_basque_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'a', 'b', 'c', '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0xF1, '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 保加利亚语
#ifdef IM_BULGARIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bulgarian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{ 8, 0x0410, 0x0411, 0x0412, 0x0413, 0x0032, 0x0041, 0x0042, 0x0043},
    /*3*/{ 8, 0x0414, 0x0415, 0x0416, 0x0417, 0x0033, 0x0044, 0x0045, 0x0046},
    /*4*/{ 8, 0x0418, 0x0419, 0x041A, 0x041B, 0x0034, 0x0047, 0x0048, 0x0049},
    /*5*/{ 8, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035, 0x004A, 0x004B, 0x004C},
    /*6*/{ 8, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036, 0x004D, 0x004E, 0x004F},
    /*7*/{ 9, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037, 0x0050, 0x0051, 0x0052, 0x0053},
    /*8*/{ 7, 0x0428, 0x0429, 0x042A, 0x0038, 0x0054, 0x0055, 0x0056},
    /*9*/{ 8, 0x042C, 0x042E, 0x042F, 0x0039, 0x0057, 0x0058, 0x0059, 0x005A},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bulgarian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{ 8, 0x0430, 0x0431, 0x0432, 0x0433, 0x0032, 0x0061, 0x0062, 0x0063},
    /*3*/{ 8, 0x0434, 0x0435, 0x0436, 0x0437, 0x0033, 0x0064, 0x0065, 0x0066},
    /*4*/{ 8, 0x0438, 0x0439, 0x043A, 0x043B, 0x0034, 0x0067, 0x0068, 0x0069},
    /*5*/{ 8, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035, 0x006A, 0x006B, 0x006C},
    /*6*/{ 8, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036, 0x006D, 0x006E, 0x006F},
    /*7*/{ 9, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037, 0x0070, 0x0071, 0x0072, 0x0073},
    /*8*/{ 7, 0x0448, 0x0449, 0x044A, 0x0038, 0x0074, 0x0075, 0x0076},
    /*9*/{ 8, 0x044C, 0x044E, 0x044F, 0x0039, 0x0077, 0x0078, 0x0079, 0x007A},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bulgarian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0410, 0x0411, 0x0412, 0x0413, 0x0032},
    /*3*/  {5, 0x0414, 0x0415, 0x0416, 0x0417, 0x0033},
    /*4*/  {5, 0x0418, 0x0419, 0x041A, 0x041B, 0x0034},
    /*5*/  {5, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035},
    /*6*/  {5, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036},
    /*7*/  {5, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037},
    /*8*/  {4, 0x0428, 0x0429, 0x042A, 0x0038},
    /*9*/  {4, 0x042C, 0x042E, 0x042F, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bulgarian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0430, 0x0431, 0x0432, 0x0433, 0x0032},
    /*3*/  {5, 0x0434, 0x0435, 0x0436, 0x0437, 0x0033},
    /*4*/  {5, 0x0438, 0x0439, 0x043A, 0x043B, 0x0034},
    /*5*/  {5, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035},
    /*6*/  {5, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036},
    /*7*/  {5, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037},
    /*8*/  {4, 0x0448, 0x0449, 0x044A, 0x0038},
    /*9*/  {4, 0x044C, 0x044E, 0x044F, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bulgarian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{ 6, 0x0410, 0x0411, 0x0412, 0x0413, 0x0490, '2'},
    /*3*/{ 7, 0x0414, 0x0415, 0x0416, 0x0417, 0x0401, 0x0404, '3'},
    /*4*/{ 7, 0x0418, 0x0419, 0x041A, 0x041B, 0x0406, 0x0407, '4'},
    /*5*/{ 4, 0x041C, 0x041D, 0x041E, '5'},
    /*6*/{ 4, 0x041F, 0x0420, 0x0421, '6'},
    /*7*/{ 5, 0x0422, 0x0423, 0x0424, 0x0425, '7'},
    /*8*/{ 6, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, '8'},
    /*9*/{ 6, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_bulgarian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{ 6, 0x0430, 0x0431, 0x0432, 0x0433, 0x0491, '2'},
    /*3*/{ 7, 0x0434, 0x0435, 0x0436, 0x0437, 0x0451, 0x0454, '3'},
    /*4*/{ 7, 0x0438, 0x0439, 0x043A, 0x043B, 0x0456, 0x0457, '4'},
    /*5*/{ 4, 0x043C, 0x043D, 0x043E, '5'},
    /*6*/{ 4, 0x043F, 0x0440, 0x0441, '6'},
    /*7*/{ 5, 0x0442, 0x0443, 0x0444, 0x0445, '7'},
    /*8*/{ 6, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, '8'},
    /*9*/{ 6, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif

// 加泰罗尼亚语
#ifdef IM_CATALAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_catalan_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', 0X00C0, 0X00C7, '2'},
    /*3*/{ 6, 'D', 'E', 'F', 0X00C9, 0X00C8, '3'},
    /*4*/{ 5, 'G', 'H', 'I', 0X00CC, '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0X00D2, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', 0X00DC, '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_catalan_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', 0X00E0, 0X00E7, '2'},
    /*3*/{ 6, 'd', 'e', 'f', 0X00E9, 0X00E8, '3'},
    /*4*/{ 5, 'g', 'h', 'i', 0X00EC, '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0X00F2, '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 5, 't', 'u', 'v', 0X00FC, '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_croatian_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x010c, 0x0106},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x0110},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', '7', 0x0160},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', '9', 0x017d},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_croatian_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x010d, 0x0107},
    /*3*/{ 5, 'd', 'e', 'f', '3', 0x0111},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x0161},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 6, 'w', 'x', 'y', 'z', '9', 0x017e},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 丹麦语
#ifdef IM_DANISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_danish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C6, 0x00C5, 0x00C4, 0x00C0},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x00D0},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D8, 0x00D6, 0x00D3},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 5, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_danish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E6, 0x00E5, 0x00E4, 0x00E0},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x00F0},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F8, 0x00F6, 0x00F3},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 5, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_danish_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x00C5, 0x00C6, 0x0032},
    /*3*/  {5, 0x0044, 0x0045, 0x0046, 0x00C9, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D8, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_danish_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x00E5, 0x00E6, 0x0032},
    /*3*/  {5, 0x0064, 0x0065, 0x0066, 0x00E9, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F8, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_danish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 'A', 'B', 'C', '2', 0x00C1, 0x00C5, 0x00C6},
    /*3*/{ 6, 'D', 'E', 'F', '3', 0x00C9, 0x00C8},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 8, 'M', 'N', 'O', '6', 0x00D3, 0x00D6, 0x00D8, 0x00D2},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 6, 'T', 'U', 'V', '8', 0x00DA, 0x00DC},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', 0x00DD, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_danish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 'a', 'b', 'c', '2', 0x00E4, 0x00E5, 0x00E6, 0x00E1},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x00E9, 0x00E8},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 8, 'm', 'n', 'o', '6', 0x00F3, 0x00F6, 0x00F8, 0x00F2},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x00DF},
    /*8*/{ 6, 't', 'u', 'v', '8', 0x00FA, 0x00FC},
    /*9*/{ 6, 'w', 'x', 'y', 'z', '9', 0x00FD},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 荷兰语
#ifdef IM_DUTCH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_dutch_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 9, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C4, 0x00C1, 0x00C0, 0x00C2, 0x00C7},
    /*3*/{ 8, 0x0044, 0x0045, 0x0046, 0x0033, 0x00CB, 0x00C9, 0x00C8, 0x00CA},
    /*4*/{ 8, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CF, 0x00CD, 0x00CC, 0x00CE},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 8, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D6, 0x00D3, 0x00D2, 0x00D4},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 8, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC, 0x00DA, 0x00D9, 0x00DB},
    /*9*/{ 5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_dutch_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 9, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E4, 0x00E1, 0x00E0, 0x00E2, 0x00E7},
    /*3*/{ 8, 0x0064, 0x0065, 0x0066, 0x0033, 0x00EB, 0x00E9, 0x00E8, 0x00EA},
    /*4*/{ 8, 0x0067, 0x0068, 0x0069, 0x0034, 0x00EF, 0x00ED, 0x00EC, 0x00EE},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 8, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F6, 0x00F3, 0x00F2, 0x00F4},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 8, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC, 0x00FA, 0x00F9, 0x00FB},
    /*9*/{ 5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_dutch_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {4, 0x0041, 0x0042, 0x0043, 0x0032},
    /*3*/  {7, 0x0044, 0x0045, 0x0046, 0x00C8, 0x00C9, 0x00CB, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CF, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_dutch_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {4, 0x0061, 0x0062, 0x0063, 0x0032},
    /*3*/  {7, 0x0064, 0x0065, 0x0066, 0x00E8, 0x00E9, 0x00EB, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00EF, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_dutch_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 9, 'A', 'B', 'C', '2', 0x00C7, 0x00C4, 0x00C1, 0x00C0, 0x00C2},
    /*3*/{ 8, 'D', 'E', 'F', '3', 0x00CB, 0x00C9, 0x00C8, 0x00CA},
    /*4*/{ 8, 'G', 'H', 'I', '4', 0x00CF, 0x00CD, 0x00CC, 0x00CE},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 8, 'M', 'N', 'O', '6', 0x00D6, 0x00D3, 0x00D2, 0x00D4},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 8, 'T', 'U', 'V', '8', 0x00DC, 0x00DA, 0x00DB, 0x00D9},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_dutch_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ';', '(', ')', '*'},
    /*2*/{ 9, 'a', 'b', 'c', '2', 0x00E7, 0x00E4, 0x00E1, 0x00E0, 0x00E2},
    /*3*/{ 8, 'd', 'e', 'f', '3', 0x00EB, 0x00E9, 0x00E8, 0x00EA},
    /*4*/{ 8, 'g', 'h', 'i', '4', 0x00EF, 0x00ED, 0x00EC, 0x00EE},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 8, 'm', 'n', 'o', '6', 0x00F6, 0x00F3, 0x00F2, 0x00F4},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x00DF},
    /*8*/{ 8, 't', 'u', 'v', '8', 0x00FC, 0x00FA, 0x00FB, 0x00F9},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 爱沙尼亚语
#ifdef IM_ESTONIAN_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_estonian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0041, 0x0042, 0x0043, 0x00C4, 0x0032},
    /*3*/  {4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {6, 0x004D, 0x004E, 0x004F, 0x00D5, 0x00D6, 0x0036},
    /*7*/  {6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0160, 0x0037},
    /*8*/  {5, 0x0054, 0x0055, 0x0056, 0x00DC, 0x0038},
    /*9*/  {6, 0x0057, 0x0058, 0x0059, 0x005A, 0x017D, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_estonian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {5, 0x0061, 0x0062, 0x0063, 0x00E4, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {6, 0x006D, 0x006E, 0x006F, 0x00F5, 0x00F6, 0x0036},
    /*7*/  {6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0161, 0x0037},
    /*8*/  {5, 0x0074, 0x0075, 0x0076, 0x00FC, 0x0038},
    /*9*/  {6, 0x0077, 0x0078, 0x0079, 0x007A, 0x017E, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_estonian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 'B', 'C', '2', 0x00C4},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 6, 'M', 'N', 'O', '6', 0x00D5, 0x00D6},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', '7', 0x0160},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x00DC},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', '9', 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_estonian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'a', 'b', 'c', '2', 0x00E4},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 6, 'm', 'n', 'o', '6', 0x00F5, 0x00F6},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x0161},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x00FC},
    /*9*/{ 6, 'w', 'x', 'y', 'z', '9', 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 菲律宾语
#ifdef IM_FILIPINO_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_filipino_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'A', 'B', 'C', '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0xD1, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_filipino_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'a', 'b', 'c', '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0xF1, '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 芬兰语
#ifdef IM_FINNISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_finnish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C4, 0x00C5, 0x00C1, 0x00C2, 0x010C},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x0110},
    /*4*/{ 6, 0x0047, 0x0048, 0x0049, 0x0034, 0x01E6, 0x01E4},
    /*5*/{ 5, 0x004A, 0x004B, 0x004C, 0x0035, 0x01E8},
    /*6*/{ 8, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D6, 0x00D8, 0x014A, 0x00D5},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0160},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC, 0x0166},
    /*9*/{ 8, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x017D, 0x01B7, 0x01EE},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_finnish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E4, 0x00E5, 0x00E1, 0x00E2, 0x010D},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x0111},
    /*4*/{ 6, 0x0067, 0x0068, 0x0069, 0x0034, 0x01E7, 0x01E5},
    /*5*/{ 5, 0x006A, 0x006B, 0x006C, 0x0035, 0x01E9},
    /*6*/{ 8, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F6, 0x00F8, 0x014B, 0x00F5},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0161},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC, 0x0167},
    /*9*/{ 8, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x017E, 0x0292, 0x01EF},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_finnish_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x00C4, 0x00C5, 0x0032},
    /*3*/  {4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D6, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_finnish_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x00E4, 0x00E5, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F6, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_finnish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x00C4, 0x00C5},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x00C9},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', '6', 0x00D6},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x00DC},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_finnish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x00E4, 0x00E5},
    /*3*/{ 5, 'd', 'e', 'f', '3', 0x00E9},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', '6', 0x00F6},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x00DF},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x00FC},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 加利西亚语
#ifdef IM_GALICIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_galician_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'A', 'B', 'C', '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0xD1, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_galician_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'a', 'b', 'c', '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0xF1, '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 格鲁吉亚语
#ifdef IM_GEORGIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_georgian =
{
    /*1*/{14, '.', ',', '-', '?', '!', '\'', '@', ':', '#', ')', '*', '(', ';', '1'},
    /*2*/{ 5, 0x10D0, 0x10D1, 0x10D2, 0x10D3, '2'},
    /*3*/{ 5, 0x10D4, 0x10D5, 0x10D6, 0x10D7, '3'},
    /*4*/{ 5, 0x10D8, 0x10D9, 0x10DA, 0x10DB, '4'},
    /*5*/{ 5, 0x10DC, 0x10DD, 0x10DE, 0x10DF, '5'},
    /*6*/{ 5, 0x10E0, 0x10E1, 0x10E2, 0x10E3, '6'},
    /*7*/{ 5, 0x10E4, 0x10E5, 0x10E6, 0x10E7, '7'},
    /*8*/{ 5, 0x10E8, 0x10E9, 0x10EA, 0x10EB, '8'},
    /*9*/{ 6, 0x10EC, 0x10ED, 0x10EE, 0x10EF, 0x10F0, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 豪撒语
#ifdef IM_HAUSA_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hausa_lower =
{
    /*1*/  {2, 0x0027, 0x0031},
    /*2*/  {5, 0x0061, 0x0062, 0x0063, 0x0253, 0x0032},
    /*3*/  {5, 0x0064, 0x0065, 0x0066, 0x0257, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {5, 0x006A, 0x006B, 0x006C, 0x0199, 0x0035},
    /*6*/  {4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {6, 0x0077, 0x0078, 0x0079, 0x007A, 0x01B4, 0x0039},
    /*0*/  {1, 0x0030},
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hausa_upper =
{
    /*1*/  {2, 0x0027, 0x0031},
    /*2*/  {5, 0x0041, 0x0042, 0x0043, 0x0181, 0x0032},
    /*3*/  {5, 0x0044, 0x0045, 0x0046, 0x018A, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {5, 0x004A, 0x004B, 0x004C, 0x0198, 0x0035},
    /*6*/  {4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {6, 0x0057, 0x0058, 0x0059, 0x005A, 0x01B3, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hausa_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 'B', 0x0181, 'C', '2'},
    /*3*/{ 5, 'D', 0x018A, 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 5, 'J', 'K', 0x0198, 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 6, 'W', 'X', 'Y', 0x01B3, 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_hausa_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'a', 'b', 0x0253, 'c', '2'},
    /*3*/{ 5, 'd', 0x0257, 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 5, 'j', 'k', 0x0199, 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 6, 'w', 'x', 'y', 0x01B4, 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 冰岛语
#ifdef IM_ICELANDIC_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_icelandic_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x00C1, 0x00C6, 0x0032},
    /*3*/  {5, 0x0044, 0x0045, 0x0046, 0x00C9, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CD, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {7, 0x004D, 0x004E, 0x004F, 0x00D0, 0x00D3, 0x00D6, 0x0036},
    /*7*/  {6, 0x0050, 0x0051, 0x0052, 0x0053, 0x00DE, 0x0037},
    /*8*/  {5, 0x0054, 0x0055, 0x0056, 0x00DA, 0x0038},
    /*9*/  {6, 0x0057, 0x0058, 0x0059, 0x005A, 0x00DD, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_icelandic_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x00E1, 0x00E6, 0x0032},
    /*3*/  {5, 0x0064, 0x0065, 0x0066, 0x00E9, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00ED, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {7, 0x006D, 0x006E, 0x006F, 0x00F0, 0x00F3, 0x00F6, 0x0036},
    /*7*/  {6, 0x0070, 0x0071, 0x0072, 0x0073, 0x00FE, 0x0037},
    /*8*/  {5, 0x0074, 0x0075, 0x0076, 0x00FA, 0x0038},
    /*9*/  {6, 0x0077, 0x0078, 0x0079, 0x007A, 0x00FD, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_icelandic_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C6, 0x00C1, 0x00C4, 0x00C5},
    /*3*/{ 7, 0x0044, 0x0045, 0x0046, 0x0033, 0x00D0, 0x00C9, 0x00CB},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CD},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D3, 0x00D6, 0x00D8},
    /*7*/{ 5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DA, 0x00DC},
    /*9*/{ 7, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x00DD, 0x00DE},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_icelandic_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 8, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E6, 0x00E1, 0x00E4, 0x00E5},
    /*3*/{ 7, 0x0064, 0x0065, 0x0066, 0x0033, 0x00F0, 0x00E9, 0x00EB},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00ED},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F3, 0x00F6, 0x00F8},
    /*7*/{ 5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FA, 0x00FC},
    /*9*/{ 7, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x00FD, 0x00FE},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 伊博语
#ifdef IM_IGBO_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_igbo_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'A', 'B', 'C', '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 5, 'G', 'H', 'I', 0x1ECA, '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 6, 'M', 'N', 0xD1, 'O', 0x1ECC, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 0x1EE4, 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_igbo_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'a', 'b', 'c', '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 5, 'g', 'h', 'i', 0x1ECB, '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 6, 'm', 'n', 0xF1, 'o', 0x1ECD, '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 5, 't', 'u', 0x1EE5, 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 爱尔兰语
#ifdef IM_IRISH_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_irish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'A', 0xC1, 'B', 'C', '2'},
    /*3*/{ 5, 'D', 'E', 0xC9, 'F', '3'},
    /*4*/{ 5, 'G', 'H', 'I', 0xCD, '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0xD3, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 0xDA, 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_irish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 'a', 0xE1, 'b', 'c', '2'},
    /*3*/{ 5, 'd', 'e', 0xE9, 'f', '3'},
    /*4*/{ 5, 'g', 'h', 'i', 0xED, '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0xF3, '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 5, 't', 'u', 0xFA, 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif



// 哈萨克斯坦
#ifdef IM_KAZAKH_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kazakh_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 0x0410, 0x04D8, 0x0411, 0x0412, 0x0413, 0x0492, '2'},
    /*3*/{ 6, 0x0414, 0x0415, 0x0401, 0x0416, 0x0417, '3'},
    /*4*/{ 7, 0x0418, 0x0419, 0x041A, 0x049A, 0x041B, 0x0406, '4'},
    /*5*/{ 6, 0x041C, 0x041D, 0x04A2, 0x041E, 0x04E8, '5'},
    /*6*/{ 4, 0x041F, 0x0420, 0x0421, '6'},
    /*7*/{ 7, 0x0422, 0x0423, 0x04B0, 0x04AE, 0x0424, 0x0425, '7'},
    /*8*/{ 7, 0x04BA, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, '8'},
    /*9*/{ 6, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_kazakh_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 7, 0x0430, 0x04D9, 0x0431, 0x0432, 0x0433, 0x0493, '2'},
    /*3*/{ 6, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437, '3'},
    /*4*/{ 7, 0x0438, 0x0439, 0x043A, 0x049B, 0x043B, 0x0456, '4'},
    /*5*/{ 6, 0x043C, 0x043D, 0x04A3, 0x043E, 0x04E9, '5'},
    /*6*/{ 4, 0x043F, 0x0440, 0x0441, '6'},
    /*7*/{ 7, 0x0442, 0x0443, 0x04B1, 0x04AF, 0x0444, 0x0445, '7'},
    /*8*/{ 7, 0x04BB, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, '8'},
    /*9*/{ 6, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 拉脱维亚语
#ifdef IM_LATVIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_latvian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x0100, 0x010C},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x0112},
    /*4*/{ 6, 'G', 'H', 'I', '4', 0x0122, 0x012A},
    /*5*/{ 6, 'J', 'K', 'L', '5', 0x0136, 0x013B},
    /*6*/{ 6, 'M', 'N', 'O', '6', 0x0145, 0x014C},
    /*7*/{ 7, 'P', 'Q', 'R', 'S', '7', 0x0156, 0x0160},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x016A},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', '9', 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_latvian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x0101, 0x010D},
    /*3*/{ 5, 'd', 'e', 'f', '3', 0x0113},
    /*4*/{ 6, 'g', 'h', 'i', '4', 0x0123, 0x012B},
    /*5*/{ 6, 'j', 'k', 'l', '5', 0x0137, 0x013C},
    /*6*/{ 6, 'm', 'n', 'o', '6', 0x0146, 0x014D},
    /*7*/{ 7, 'p', 'q', 'r', 's', '7', 0x0157, 0x0161},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x016B},
    /*9*/{ 6, 'w', 'x', 'y', 'z', '9', 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_lithuanian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0041, 0x0042, 0x0043, 0x0032, 0x0104, 0x010C},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x0118, 0x0116},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x012E},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 4, 0x004D, 0x004E, 0x004F, 0x0036},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0160},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x0172, 0x016A},
    /*9*/{ 6, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_lithuanian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0061, 0x0062, 0x0063, 0x0032, 0x0105, 0x010D},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x0119, 0x0117},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x012F},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 4, 0x006D, 0x006E, 0x006F, 0x0036},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0161},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x0173, 0x016B},
    /*9*/{ 6, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_lithuanian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x0104, 0x010C},
    /*3*/{ 6, 'D', 'E', 'F', '3', 0x0118, 0x0116},
    /*4*/{ 5, 'G', 'H', 'I', '4', 0x012E},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', '7', 0x0160},
    /*8*/{ 6, 'T', 'U', 'V', '8', 0x0172, 0x016A},
    /*9*/{ 6, 'W', 'X', 'Y', 'Z', '9', 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_lithuanian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x0105, 0x010D},
    /*3*/{ 6, 'd', 'e', 'f', '3', 0x0119, 0x0117},
    /*4*/{ 5, 'g', 'h', 'i', '4', 0x012F},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x0161},
    /*8*/{ 6, 't', 'u', 'v', '8', 0x0173, 0x016B},
    /*9*/{ 6, 'w', 'x', 'y', 'z', '9', 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 马其顿
#ifdef IM_MACEDONIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_macedonian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 0x0410, 0x0411, 0x0412, 0x0413, '2'},
    /*3*/{ 7, 0x0414, 0x0403, 0x0415, 0x0416, 0x0417, 0x0405, '3'},
    /*4*/{ 6, 0x0418, 0x0408, 0x041A, 0x041B, 0x0409, '4'},
    /*5*/{ 5, 0x041C, 0x041D, 0x040A, 0x041E, '5'},
    /*6*/{ 4, 0x041F, 0x0420, 0x0421, '6'},
    /*7*/{ 6, 0x0422, 0x040C, 0x0423, 0x0424, 0x0425, '7'},
    /*8*/{ 5, 0x0426, 0x0427, 0x040F, 0x0428, '8'},
    /*9*/{ 1, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_macedonian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 5, 0x0430, 0x0431, 0x0432, 0x0433, '2'},
    /*3*/{ 7, 0x0434, 0x0453, 0x0435, 0x0436, 0x0437, 0x0455, '3'},
    /*4*/{ 6, 0x0438, 0x0458, 0x043A, 0x043B, 0x0459, '4'},
    /*5*/{ 5, 0x043C, 0x043D, 0x045A, 0x043E, '5'},
    /*6*/{ 4, 0x043F, 0x0440, 0x0441, '6'},
    /*7*/{ 6, 0x0442, 0x045C, 0x0443, 0x0444, 0x0445, '7'},
    /*8*/{ 5, 0x0446, 0x0447, 0x045F, 0x0448, '8'},
    /*9*/{ 1, '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 摩尔多瓦语
#ifdef IM_MOLDOVAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_moldovan_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 8, 'A', 'B', 'C', '2', 0x0102, 0x00C2, 0x00C1, 0x00C4},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x00C9},
    /*4*/{ 6, 'G', 'H', 'I', '4', 0x00CE, 0x00CD},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 7, 'M', 'N', 'O', '6', 0x00D3, 0x00D6, 0x0150},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', '7', 0x015E},
    /*8*/{ 8, 'T', 'U', 'V', '8', 0x0162, 0x00DA, 0x00DC, 0x0170},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_moldovan_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 8, 'a', 'b', 'c', '2', 0x0103, 0x00E2, 0x00E1, 0x00E4},
    /*3*/{ 5, 'd', 'e', 'f', '3', 0x00E9},
    /*4*/{ 6, 'g', 'h', 'i', '4', 0x00EE, 0x00ED},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 7, 'm', 'n', 'o', '6', 0x00F3, 0x00F6, 0x0151},
    /*7*/{ 7, 'p', 'q', 'r', 's', '7', 0x015F, 0x00DF},
    /*8*/{ 8, 't', 'u', 'v', '8', 0x0163, 0x00FA, 0x00FC, 0x0171},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 挪威语
#ifdef IM_NORWEGIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_norwegian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ')', '(', ';', '*'},
    /*2*/{10, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C6, 0x00C5, 0x00C4, 0x00C0, 0x00C1, 0x010C},
    /*3*/{ 8, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x00C8, 0x00CA, 0x0110},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CF},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{10, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D8, 0x00D6, 0x00D3, 0x00D2, 0x00D4, 0x014A},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0160},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC, 0x0166},
    /*9*/{ 7, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x00DE, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_norwegian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ')', '(', ';', '*'},
    /*2*/{10, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E6, 0x00E5, 0x00E4, 0x00E0, 0x00E1, 0x010D},
    /*3*/{ 8, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x00E8, 0x00EA, 0x0111},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00EF},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{10, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F8, 0x00F6, 0x00F3, 0x00F2, 0x00F4, 0x014B},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0161},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC, 0x0167},
    /*9*/{ 7, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x00FE, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_norwegian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x00C5, 0x00C6, 0x0032},
    /*3*/  {5, 0x0044, 0x0045, 0x0046, 0x00C9, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D8, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_norwegian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x00E5, 0x00E6, 0x0032},
    /*3*/  {5, 0x0064, 0x0065, 0x0066, 0x00E9, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F8, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_norwegian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ')', '(', ';', '*'},
    /*2*/{ 9, 'A', 'B', 'C', 0x00C5, 0x00C6, 0x00C1, 0x00C0, 0x00C4, '2'},
    /*3*/{ 6, 'D', 'E', 'F', 0x00C9, 0x00C8, '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 9, 'M', 'N', 'O', 0x00D8, 0x00D3, 0x00D2, 0x00D4, 0x00D6, '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', 0x00DC, '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_norwegian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '1', ')', '(', ';', '*'},
    /*2*/{ 9, 'a', 'b', 'c', 0x00E5, 0x00E6, 0x00E1, 0x00E0, 0x00E4, '2'},
    /*3*/{ 6, 'd', 'e', 'f', 0x00E9, 0x00E8, '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 9, 'm', 'n', 'o', 0x00F8, 0x00F3, 0x00F2, 0x00F4, 0x00F6, '6'},
    /*7*/{ 6, 'p', 'q', 'r', 's', 0x00DF, '7'},
    /*8*/{ 5, 't', 'u', 'v', 0x00FC, '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 波兰语
#ifdef IM_POLISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_polish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0041, 0x0042, 0x0043, 0x0032, 0x0104, 0x0106},
    /*3*/{ 5, 0x0044, 0x0045, 0x0046, 0x0033, 0x0118},
    /*4*/{ 4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/{ 5, 0x004A, 0x004B, 0x004C, 0x0035, 0x0141},
    /*6*/{ 6, 0x004D, 0x004E, 0x004F, 0x0036, 0x0143, 0x00D3},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x015A},
    /*8*/{ 4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/{ 7, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x0179, 0x017B},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_polish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 0x0061, 0x0062, 0x0063, 0x0032, 0x0105, 0x0107},
    /*3*/{ 5, 0x0064, 0x0065, 0x0066, 0x0033, 0x0119},
    /*4*/{ 4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/{ 5, 0x006A, 0x006B, 0x006C, 0x0035, 0x0142},
    /*6*/{ 6, 0x006D, 0x006E, 0x006F, 0x0036, 0x0144, 0x00F3},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x015B},
    /*8*/{ 4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/{ 7, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x017A, 0x017C},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_polish_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0032},
    /*3*/  {5, 0x0046, 0x0047, 0x0048, 0x0049, 0x0033},
    /*4*/  {4, 0x004A, 0x004B, 0x004C, 0x0034},
    /*5*/  {5, 0x004D, 0x004E, 0x004F, 0x0050, 0x0035},
    /*6*/  {6, 0x0052, 0x0053, 0x0054, 0x0055, 0x0057, 0x0036},
    /*7*/  {5, 0x0059, 0x005A, 0x00D3, 0x0104, 0x0037},
    /*8*/  {3, 0x0106, 0x0118, 0x0038},
    /*9*/  {6, 0x0141, 0x0143, 0x015A, 0x0179, 0x017B, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_polish_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x0105, 0x0107, 0x0032},
    /*3*/  {5, 0x0064, 0x0065, 0x0066, 0x0119, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {5, 0x006A, 0x006B, 0x006C, 0x0142, 0x0035},
    /*6*/  {6, 0x006D, 0x006E, 0x006F, 0x0144, 0x00F3, 0x0036},
    /*7*/  {5, 0x0070, 0x0072, 0x0073, 0x015B, 0x0037},
    /*8*/  {3, 0x0074, 0x0075, 0x0038},
    /*9*/  {6, 0x0077, 0x0079, 0x007A, 0x017A, 0x017C, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_polish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 'A', 'B', 'C', '2', 0x00C4, 0x00C1, 0x0104, 0x10C, 0x106},
    /*3*/{ 8, 'D', 'E', 'F', '3', 0x010E, 0x00C9, 0x0114, 0x118},
    /*4*/{ 5, 'G', 'H', 'I', '4', 0x00CD},
    /*5*/{ 7, 'J', 'K', 'L', '5', 0x013D, 0x0139, 0x0141},
    /*6*/{ 9, 'M', 'N', 'O', '6', 0x0143, 0x0147, 0x00D3, 0x00D6, 0x00D4},
    /*7*/{10, 'P', 'Q', 'R', 'S', '7', 0x0154, 0x0158, 0x015A, 0x0160, 0x00DF},
    /*8*/{ 8, 'T', 'U', 'V', '8', 0x0164, 0x016E, 0x00DA, 0x00DC},
    /*9*/{ 9, 'W', 'X', 'Y', 'Z', '9', 0x00DD, 0X0179, 0x017D, 0x017B},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_polish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 9, 'a', 'b', 'c', '2', 0x00E1, 0x00E4, 0x0105, 0x010D, 0x107},
    /*3*/{ 8, 'd', 'e', 'f', '3', 0x010F, 0x00E9, 0x0115, 0x0119},
    /*4*/{ 5, 'g', 'h', 'i', '4', 0x00ED},
    /*5*/{ 7, 'j', 'k', 'l', '5', 0x013A, 0x013E, 0x0142},
    /*6*/{ 9, 'm', 'n', 'o', '6', 0x0144, 0x0148, 0x00F3, 0x00F4, 0x00F6},
    /*7*/{10, 'p', 'q', 'r', 's', '7', 0x0155, 0x00159, 0x015B, 0x0161, 0x00DF},
    /*8*/{ 8, 't', 'u', 'v', '8', 0x0165, 0x016F, 0x00FA, 0x00FC},
    /*9*/{ 9, 'w', 'x', 'y', 'z', '9', 0x00FD, 0x017A, 0x017C, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 塞尔维亚语
#ifdef IM_SERBIAN_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_serbian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{10, 0x0410, 0x0411, 0x0412, 0x0413, '2', 0x0041, 0x0042, 0x0043, 0x010C, 0x0106},
    /*3*/{ 8, 0x0414, 0x0402, 0x0415, '3', 0x0044, 0x0110, 0x0045, 0x0046},
    /*4*/{ 7, 0x0416, 0x0417, 0x0418, '4', 0x0047, 0x0048, 0x0049},
    /*5*/{ 8, 0x0408, 0x041A, 0x041B, 0x0409, '5', 0x004A, 0x004B, 0x004C},
    /*6*/{ 8, 0x041C, 0x041D, 0x040A, 0x041E, '6', 0x004D, 0x004E, 0x004F},
    /*7*/{ 9, 0x041F, 0x0420, 0x0421, '7', 0x0050, 0x0051, 0x0052, 0x0053, 0x0160},
    /*8*/{ 8, 0x0422, 0x040B, 0x0423, 0x0424, '8', 0x0054, 0x0055, 0x0056},
    /*9*/{11, 0x0425, 0x0426, 0x0427, 0x040F, 0x0428, '9', 0x0057, 0x0058, 0x0059, 0x005A, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_serbian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{10, 0x0430, 0x0431, 0x0432, 0x0433, '2', 0x0061, 0x0062, 0x0063, 0x010D, 0x0107},
    /*3*/{ 8, 0x0434, 0x0452, 0x0435, '3', 0x0064, 0x0111, 0x0065, 0x0066},
    /*4*/{ 7, 0x0436, 0x0437, 0x0438, '4', 0x0067, 0x0068, 0x0069},
    /*5*/{ 8, 0x0458, 0x043A, 0x043B, 0x0459, '5', 0x006A, 0x006B, 0x006C},
    /*6*/{ 8, 0x043C, 0x043D, 0x045A, 0x043E, '6', 0x006D, 0x006E, 0x006F},
    /*7*/{ 9, 0x043F, 0x0440, 0x0441, '7', 0x0070, 0x0071, 0x0072, 0x0073, 0x0161},
    /*8*/{ 8, 0x0442, 0x045B, 0x0443, 0x0444, '8', 0x0074, 0x0075, 0x0076},
    /*9*/{11, 0x0445, 0x0446, 0x0447, 0x045F, 0x0448, '9', 0x0077, 0x0078, 0x0079, 0x007A, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 塞索托语
#ifdef IM_SESOTHO_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_sesotho_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'A', 'B', 'C', '2'},
    /*3*/{ 4, 'D', 'E', 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 4, 'M', 'N', 'O', '6'},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_sesotho_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'a', 'b', 'c', '2'},
    /*3*/{ 4, 'd', 'e', 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 4, 'm', 'n', 'o', '6'},
    /*7*/{ 5, 'p', 'q', 'r', 's', '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif


// 斯洛伐克语
#ifdef IM_SLOVAK_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovak_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 7, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C1, 0x010C, 0x00C4},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x010E, 0x00C9},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CD},
    /*5*/{ 6, 0x004A, 0x004B, 0x004C, 0x0035, 0x0139, 0x013D},
    /*6*/{ 9, 0x004D, 0x004E, 0x004F, 0x0036, 0x0147, 0x00D3, 0x00D4, 0x00D6, 0x0150},
    /*7*/{ 7, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0154, 0x0160},
    /*8*/{ 8, 0x0054, 0x0055, 0x0056, 0x0038, 0x0164, 0x00DA, 0x00DC, 0x0170},
    /*9*/{ 7, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x00DD, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovak_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 7, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E1, 0x010D, 0x00E4},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x010F, 0x00E9},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00ED},
    /*5*/{ 6, 0x006A, 0x006B, 0x006C, 0x0035, 0x013A, 0x013E},
    /*6*/{ 9, 0x006D, 0x006E, 0x006F, 0x0036, 0x0148, 0x00F3, 0x00F4, 0x00F6, 0x0151},
    /*7*/{ 7, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0155, 0x0161},
    /*8*/{ 8, 0x0074, 0x0075, 0x0076, 0x0038, 0x0165, 0x00FA, 0x00FC, 0x0171},
    /*9*/{ 7, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x00FD, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovak_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {7, 0x0041, 0x0042, 0x0043, 0x00C1, 0x00C4, 0x010C, 0x0032},
    /*3*/  {6, 0x0044, 0x0045, 0x0046, 0x00C9, 0x010E, 0x0033},
    /*4*/  {5, 0x0047, 0x0048, 0x0049, 0x00CD, 0x0034},
    /*5*/  {6, 0x004A, 0x004B, 0x004C, 0x0139, 0x013D, 0x0035},
    /*6*/  {7, 0x004D, 0x004E, 0x004F, 0x00D3, 0x00D4, 0x0147, 0x0036},
    /*7*/  {7, 0x0050, 0x0051, 0x0052, 0x0053, 0x0154, 0x0160, 0x0037},
    /*8*/  {6, 0x0054, 0x0055, 0x0056, 0x00DA, 0x0164, 0x0038},
    /*9*/  {7, 0x0057, 0x0058, 0x0059, 0x005A, 0x00DD, 0x017D, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovak_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {7, 0x0061, 0x0062, 0x0063, 0x00E1, 0x00E4, 0x010D, 0x0032},
    /*3*/  {6, 0x0064, 0x0065, 0x0066, 0x010F, 0x00E9, 0x0033},
    /*4*/  {5, 0x0067, 0x0068, 0x0069, 0x00ED, 0x0034},
    /*5*/  {6, 0x006A, 0x006B, 0x006C, 0x013A, 0x013E, 0x0035},
    /*6*/  {7, 0x006D, 0x006E, 0x006F, 0x0148, 0x00F3, 0x00F4, 0x0036},
    /*7*/  {7, 0x0070, 0x0071, 0x0072, 0x0073, 0x0155, 0x0161, 0x0037},
    /*8*/  {6, 0x0074, 0x0075, 0x0076, 0x0165, 0x00FA, 0x0038},
    /*9*/  {7, 0x0077, 0x0078, 0x0079, 0x007A, 0x00FD, 0x017E, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovak_upper =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 9, 'A', 'B', 'C', '2', 0x00C4, 0x00C1, 0x0104, 0x010c, 0x0106},
    /*3*/{ 8, 'D', 'E', 'F', '3', 0x010e, 0x00C9, 0x011a, 0x0118},
    /*4*/{ 5, 'G', 'H', 'I', '4', 0x00CD},
    /*5*/{ 7, 'J', 'K', 'L', '5', 0x0139, 0x013d, 0x0141},
    /*6*/{10, 'M', 'N', 'O', '6', 0x0143, 0x0147, 0x00D3, 0x00D4, 0x00D6, 0x0150},
    /*7*/{ 9, 'P', 'Q', 'R', 'S', '7', 0x0154, 0x0158, 0x015a, 0x0160},
    /*8*/{ 9, 'T', 'U', 'V', '8', 0x0164, 0x00DA, 0x00DC, 0x016e, 0x0170},
    /*9*/{ 9, 'W', 'X', 'Y', 'Z', '9', 0x00DD, 0x017b, 0x017d, 0x0179},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_slovak_lower =
{
    /*1*/{17, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1', ')', '(', ';', '*'},
    /*2*/{ 9, 'a', 'b', 'c', '2', 0x00E4, 0x00E1, 0x0105, 0x010d, 0x0107},
    /*3*/{ 8, 'd', 'e', 'f', '3', 0x010f, 0x00E9, 0x011b, 0x0119},
    /*4*/{ 5, 'g', 'h', 'i', '4', 0x00ED},
    /*5*/{ 7, 'j', 'k', 'l', '5', 0x013a, 0x013e, 0x0142},
    /*6*/{10, 'm', 'n', 'o', '6', 0x0144, 0x0148, 0x00F3, 0x00F4, 0x00F6, 0x0151},
    /*7*/{10, 'p', 'q', 'r', 's', '7', 0x00DF, 0x0155, 0x0159, 0x015b, 0x0161},
    /*8*/{ 9, 't', 'u', 'v', '8', 0x0165, 0x00FA, 0x00FC, 0x016f, 0x0171},
    /*9*/{ 9, 'w', 'x', 'y', 'z', '9', 0x00FD, 0x017a, 0x017c, 0x017e},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 瑞典语
#ifdef IM_SWEDISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_swedish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{10, 0x0041, 0x0042, 0x0043, 0x0032, 0x00C5, 0x00C4, 0x00C0, 0x00C6, 0x00C1, 0x010C},
    /*3*/{ 6, 0x0044, 0x0045, 0x0046, 0x0033, 0x00C9, 0x0110},
    /*4*/{ 5, 0x0047, 0x0048, 0x0049, 0x0034, 0x00CF},
    /*5*/{ 4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/{ 7, 0x004D, 0x004E, 0x004F, 0x0036, 0x00D6, 0x00D8, 0x014A},
    /*7*/{ 6, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0160},
    /*8*/{ 6, 0x0054, 0x0055, 0x0056, 0x0038, 0x00DC, 0x0166},
    /*9*/{ 6, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x017D},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_swedish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{10, 0x0061, 0x0062, 0x0063, 0x0032, 0x00E5, 0x00E4, 0x00E0, 0x00E6, 0x00E1, 0x010D},
    /*3*/{ 6, 0x0064, 0x0065, 0x0066, 0x0033, 0x00E9, 0x0111},
    /*4*/{ 5, 0x0067, 0x0068, 0x0069, 0x0034, 0x00EF},
    /*5*/{ 4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/{ 7, 0x006D, 0x006E, 0x006F, 0x0036, 0x00F6, 0x00F8, 0x014B},
    /*7*/{ 6, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0161},
    /*8*/{ 6, 0x0074, 0x0075, 0x0076, 0x0038, 0x00FC, 0x0167},
    /*9*/{ 6, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x017E},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_swedish_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0041, 0x0042, 0x0043, 0x00C4, 0x00C5, 0x0032},
    /*3*/  {4, 0x0044, 0x0045, 0x0046, 0x0033},
    /*4*/  {4, 0x0047, 0x0048, 0x0049, 0x0034},
    /*5*/  {4, 0x004A, 0x004B, 0x004C, 0x0035},
    /*6*/  {5, 0x004D, 0x004E, 0x004F, 0x00D6, 0x0036},
    /*7*/  {5, 0x0050, 0x0051, 0x0052, 0x0053, 0x0037},
    /*8*/  {4, 0x0054, 0x0055, 0x0056, 0x0038},
    /*9*/  {5, 0x0057, 0x0058, 0x0059, 0x005A, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_swedish_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0061, 0x0062, 0x0063, 0x00E4, 0x00E5, 0x0032},
    /*3*/  {4, 0x0064, 0x0065, 0x0066, 0x0033},
    /*4*/  {4, 0x0067, 0x0068, 0x0069, 0x0034},
    /*5*/  {4, 0x006A, 0x006B, 0x006C, 0x0035},
    /*6*/  {5, 0x006D, 0x006E, 0x006F, 0x00F6, 0x0036},
    /*7*/  {5, 0x0070, 0x0071, 0x0072, 0x0073, 0x0037},
    /*8*/  {4, 0x0074, 0x0075, 0x0076, 0x0038},
    /*9*/  {5, 0x0077, 0x0078, 0x0079, 0x007A, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_swedish_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'A', 'B', 'C', '2', 0x00C4, 0x00C5},
    /*3*/{ 5, 'D', 'E', 'F', '3', 0x00C9},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', '6', 0x00D6},
    /*7*/{ 5, 'P', 'Q', 'R', 'S', '7'},
    /*8*/{ 5, 'T', 'U', 'V', '8', 0x00DC},
    /*9*/{ 5, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_swedish_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 6, 'a', 'b', 'c', '2', 0x00E4, 0x00E5},
    /*3*/{ 5, 'd', 'e', 'f', '3', 0x00E9},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', '6', 0x00F6},
    /*7*/{ 6, 'p', 'q', 'r', 's', '7', 0x00DF},
    /*8*/{ 5, 't', 'u', 'v', '8', 0x00FC},
    /*9*/{ 5, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 乌克兰语
#ifdef IM_UKRAINIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_ukrainian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{ 9, 0x0410, 0x0411, 0x0412, 0x0413, 0x0490, 0x0032, 0x0041, 0x0042, 0x0043},
    /*3*/{ 9, 0x0414, 0x0415, 0x0404, 0x0416, 0x0417, 0x0033, 0x0044, 0x0045, 0x0046},
    /*4*/{10, 0x0418, 0x0406, 0x0407, 0x0419, 0x041A, 0x041B, 0x0034, 0x0047, 0x0048, 0x0049},
    /*5*/{ 8, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035, 0x004A, 0x004B, 0x004C},
    /*6*/{ 8, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036, 0x004D, 0x004E, 0x004F},
    /*7*/{ 9, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037, 0x0050, 0x0051, 0x0052, 0x0053},
    /*8*/{ 6, 0x0428, 0x0429, 0x0038, 0x0054, 0x0055, 0x0056},
    /*9*/{ 8, 0x042C, 0x042E, 0x042F, 0x0039, 0x0057, 0x0058, 0x0059, 0x005A},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_ukrainian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{ 9, 0x0430, 0x0431, 0x0432, 0x0433, 0x0491, 0x0032, 0x0061, 0x0062, 0x0063},
    /*3*/{ 9, 0x0434, 0x0435, 0x0454, 0x0436, 0x0437, 0x0033, 0x0064, 0x0065, 0x0066},
    /*4*/{10, 0x0438, 0x0456, 0x0457, 0x0439, 0x043A, 0x043B, 0x0034, 0x0067, 0x0068, 0x0069},
    /*5*/{ 8, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035, 0x006A, 0x006B, 0x006C},
    /*6*/{ 8, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036, 0x006D, 0x006E, 0x006F},
    /*7*/{ 9, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037, 0x0070, 0x0071, 0x0072, 0x0073},
    /*8*/{ 6, 0x0448, 0x0449, 0x0038, 0x0074, 0x0075, 0x0076},
    /*9*/{ 8, 0x044C, 0x044E, 0x044F, 0x0039, 0x0077, 0x0078, 0x0079, 0x007A},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_ukrainian_upper =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0410, 0x0411, 0x0412, 0x0413, 0x0490, 0x0032},
    /*3*/  {6, 0x0414, 0x0415, 0x0416, 0x0417, 0x0404, 0x0033},
    /*4*/  {7, 0x0418, 0x0419, 0x041A, 0x041B, 0x0406, 0x0407, 0x0034},
    /*5*/  {5, 0x041C, 0x041D, 0x041E, 0x041F, 0x0035},
    /*6*/  {5, 0x0420, 0x0421, 0x0422, 0x0423, 0x0036},
    /*7*/  {5, 0x0424, 0x0425, 0x0426, 0x0427, 0x0037},
    /*8*/  {4, 0x0428, 0x0429, 0x042C, 0x0038},
    /*9*/  {3, 0x042E, 0x042F, 0x0039},
    /*0*/  {1, 0x0030},
};
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_ukrainian_lower =
{
    /*1*/  {1, 0x0031},
    /*2*/  {6, 0x0430, 0x0431, 0x0432, 0x0433, 0x0491, 0x0032},
    /*3*/  {6, 0x0434, 0x0435, 0x0454, 0x0436, 0x0437, 0x0033},
    /*4*/  {7, 0x0438, 0x0456, 0x0457, 0x0439, 0x043A, 0x043B, 0x0034},
    /*5*/  {5, 0x043C, 0x043D, 0x043E, 0x043F, 0x0035},
    /*6*/  {5, 0x0440, 0x0441, 0x0442, 0x0443, 0x0036},
    /*7*/  {5, 0x0444, 0x0445, 0x0446, 0x0447, 0x0037},
    /*8*/  {4, 0x0448, 0x0449, 0x044C, 0x0038},
    /*9*/  {3, 0x044E, 0x044F, 0x0039},
    /*0*/  {1, 0x0030},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_ukrainian_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{10, 0x0410, 0x0411, 0x0412, 0x0413, 0x0403, 0x0490, '2', 'A', 'B', 'C'},
    /*3*/{12, 0x0414, 0x0415, 0x0401, 0x0416, 0x0417, 0x0402, 0x0404, 0x0405, '3', 'D', 'E', 'F'},
    /*4*/{13, 0x0418, 0x0419, 0x041A, 0x041B, 0x0406, 0x0407, 0x0408, 0x040C, 0x0409, '4', 'G', 'H', 'I'},
    /*5*/{ 9, 0x041C, 0x041D, 0x041E, 0x041f, 0x040A, '5', 'J', 'K', 'L'},
    /*6*/{10, 0x0420, 0x0421, 0x0422, 0x0423, 0x040B, 0x040E, '6', 'M', 'N', 'O'},
    /*7*/{10, 0x0424, 0x0425, 0x0426, 0x0427, 0x040F, '7', 'P', 'Q', 'R', 'S'},
    /*8*/{ 8, 0x0428, 0x0429, 0x042A, 0x042B, '8', 'T', 'U', 'V'},
    /*9*/{ 9, 0x042C, 0x042D, 0x042E, 0x042F, '9', 'W', 'X', 'Y', 'Z'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_ukrainian_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', ';', '(', ')', '*', '1'},
    /*2*/{10, 0x0430, 0x0431, 0x0432, 0x0433, 0x0453, 0x0491, '2', 'a', 'b', 'c'},
    /*3*/{12, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437, 0x0452, 0x0454, 0x0455, '3', 'd', 'e', 'f'},
    /*4*/{13, 0x0438, 0x0439, 0x043A, 0x043B, 0x0456, 0x0457, 0x0458, 0x045C, 0x0459, '4', 'g', 'h', 'i'},
    /*5*/{ 9, 0x043C, 0x043D, 0x043E, 0x043F, 0x045A, '5', 'j', 'k', 'l'},
    /*6*/{10, 0x0440, 0x0441, 0x0442, 0x0443, 0x045B, 0x045E, '6', 'm', 'n', 'o'},
    /*7*/{10, 0x0444, 0x0445, 0x0446, 0x0447, 0x045F, '7', 'p', 'q', 'r', 's'},
    /*8*/{ 8, 0x0448, 0x0449, 0x044A, 0x044B, '8', 't', 'u', 'v'},
    /*9*/{ 9, 0x044C, 0x044D, 0x044E, 0x044F, '9', 'w', 'x', 'y', 'z'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif
#endif


// 约鲁巴语
#ifdef IM_YORUBA_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_yoruba_upper =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'A', 'B', 'C', '2'},
    /*3*/{ 5, 'D', 'E', 0x1EB8, 'F', '3'},
    /*4*/{ 4, 'G', 'H', 'I', '4'},
    /*5*/{ 4, 'J', 'K', 'L', '5'},
    /*6*/{ 5, 'M', 'N', 'O', 0x1ECC, '6'},
    /*7*/{ 6, 'P', 'Q', 'R', 'S', 0x1E62, '7'},
    /*8*/{ 4, 'T', 'U', 'V', '8'},
    /*9*/{ 7, 0x0300, 0x0301, 'W', 'X', 'Y', 'Z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};

static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_yoruba_lower =
{
    /*1*/{13, '.', ',', '-', '?', '!', '\'', '@', ':', '#', '$', '/', '_', '1'},
    /*2*/{ 4, 'a', 'b', 'c', '2'},
    /*3*/{ 5, 'd', 'e', 0x1EB9, 'f', '3'},
    /*4*/{ 4, 'g', 'h', 'i', '4'},
    /*5*/{ 4, 'j', 'k', 'l', '5'},
    /*6*/{ 5, 'm', 'n', 'o', 0x1ECD, '6'},
    /*7*/{ 6, 'p', 'q', 'r', 's', 0x1E63, '7'},
    /*8*/{ 4, 't', 'u', 'v', '8'},
    /*9*/{ 7, 0x0300, 0x0301, 'w', 'x', 'y', 'z', '9'},
    /*0*/{ 2, ' ', '0'},
    /***/{0},
    /*#*/{0}
};
#endif

//阿姆哈拉语数据表还没有提供，暂时使用abc替代
#ifdef IM_AMHARIC_SUPPORT
//#if defined(MMIIM_CSTAR_MULTITAP)     暂时仅仅cstar提供了数据表，故其他输入引擎暂且使用该数据
static  MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_amharic = 
{
    /*1*/{7, 0x1362, 0x1363, 0x1364, 0x1365, '?', '!', '1'}, 
    /*2*/{3, 0x1200, 0x1208, 0x1218},
    /*3*/{4, 0x1228, 0x1230, 0x1238, 0x1240},
    /*4*/{4, 0x1260, 0x1270, 0x1278, 0x1290},
    /*5*/{3, 0x1298, 0x12A0, 0x12A8}, 
    /*6*/{3, 0x12C8, 0x12D8, 0x12E0},
    /*7*/{4, 0x12E8, 0x12F0, 0x1300, 0x1308},
    /*8*/{3, 0x1320, 0x1328, 0x1330},
    /*9*/{4, 0x1340, 0x1348, 0x1350, 0x1268},
    /*0*/{2, ' ', '0'},
};

//#endif//CSTAR
#endif//IM_AMHARIC_SUPPORT

#ifdef IM_KHMER_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)     
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_khmer = 
{
    /*1*/{4, 0x17D4, 0x17D5, 0x17D6, 0x17D7},
    /*2*/{6, 0x17B1, 0x17B6, 0x1796, 0x1794, 0x1787, 0x1785},
    /*3*/{8, 0x178C, 0x178A, 0x17C2, 0x17C1, 0x17AF, 0x1792, 0x1790, 0x17CC},
    /*4*/{8, 0x17A2, 0x1784, 0x17CD, 0x17C7, 0x17A0, 0x17B8, 0x17B7, 0x17A6},
    /*5*/{8, 0x1789, 0x17D2, 0x1782, 0x1780, 0x17CE, 0x17A1, 0x179B, 0x17CF},
    /*6*/{7, 0x17C6, 0x1798, 0x178E, 0x1793, 0x17C5, 0x17C4, 0x17B3},
    /*7*/{12, 0x1797, 0x1795, 0x17B0, 0x1788, 0x1786, 0x17BE, 0x17A5, 0x17AB, 0x179A, 0x17AC, 0x17C3, 0x179F},
    /*8*/{8, 0x178F, 0x17BF, 0x17C0, 0x17BC, 0x17BB, 0x17A7, 0x17CA, 0x179C},
    /*9*/{12, 0x17BA, 0x17B9, 0x17AD, 0x17AE, 0x1783, 0x1781, 0x17BD, 0x1799, 0x17A9, 0x17AA, 0x178D, 0x178B},
    /*0*/{3, 0x17C9, 0x17CB, 0x17C8},
    /***/{0},
    /*#*/{0}
};    
#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_khmer =
{
    /*1*/  {16, 0x17A5, 0x17A6, 0x17A7, 0x17A9, 0x17AA, 0x17AB, 0x17AC, 0x17AD, 0x17AE, 0x17AF, 0x17B0, 0x17B1, 0x17B2, 0x17B3, 0x17E1, 0x0031},
    /*2*/  {13, 0x17B6, 0x17B7, 0x17B8, 0x17B9, 0x17BA, 0x17BB, 0x17BC, 0x17BD, 0x17BE, 0x17BF, 0x17C0, 0x17E2, 0x0032},
    /*3*/  {9, 0x17C1, 0x17C2, 0x17C3, 0x17C4, 0x17C5, 0x17C9, 0x17CA, 0x17E3, 0x0033},
    /*4*/  {11, 0x1780, 0x1781, 0x1782, 0x1783, 0x1784, 0x1785, 0x1786, 0x1787, 0x1788, 0x17E4, 0x0034},
    /*5*/  {10, 0x1789, 0x178A, 0x178B, 0x178C, 0x178D, 0x178E, 0x178F, 0x1790, 0x17E5, 0x0035},
    /*6*/  {10, 0x1791, 0x1792, 0x1793, 0x1794, 0x1795, 0x1796, 0x1797, 0x1798, 0x17E6, 0x0036},
    /*7*/  {10, 0x1799, 0x179A, 0x179B, 0x179C, 0x179F, 0x17A0, 0x17A1, 0x17A2, 0x17E7, 0x0037},
    /*8*/  {9, 0x17C6, 0x17C7, 0x17C8, 0x17CB, 0x17CC, 0x17CD, 0x17CE, 0x17E8, 0x0038},
    /*9*/  {9, 0x17CF, 0x17D0, 0x17D2, 0x17D4, 0x17D5, 0x17D6, 0x17D7, 0x17E9, 0x0039},
    /*0*/  {2, 0x17E0, 0x0030},
};
#endif//CSTAR
#endif//IM_KHMER_SUPPORT

//老挝
#ifdef IM_LAO_SUPPORT
#if defined MMIIM_CSTAR_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_lao =
{
    /*1*/  {11, 0x0EC0, 0x0EC1, 0x0EC2, 0x0EC3, 0x0EC4, 0x0031, 0x0EB0, 0x0EB2, 0x0EB3, 0x0EBB, 0x0EBC},
    /*2*/  {4, 0x0E81, 0x0E82, 0x0E84, 0x0032},
    /*3*/  {3, 0x0E87, 0x0E88, 0x0033},
    /*4*/  {2, /*0x0EBA,*/ 0x0EBD, 0x0034},
    /*5*/  {5, 0x0E94, 0x0E95, 0x0E96, 0x0E97, 0x0035},
    /*6*/  {6, 0x0E99, 0x0E9A, 0x0E9B, 0x0E9C, 0x0E9D, 0x0036},
    /*7*/  {5, 0x0E9E, 0x0E9F, 0x0EA1, 0x0EA2, 0x0037},
    /*8*/  {4, 0x0EA3, 0x0EA5, 0x0EA7, 0x0038},
    /*9*/  {5, 0x0EAA, 0x0EAB, 0x0EAD, 0x0EAE, 0x0039},
    /*0*/  {17, 0x0020, 0x0030, 0x0EAF, 0x0EB1, 0x0EB4, 0x0EB5, 0x0EB6, 0x0EB7, 0x0EB8, 0x0EB9, 0x0EC6, 0x0EC8, 0x0EC9, 0x0ECA, 0x0ECB, 0x0ECC, 0x0ECD},
};

#elif defined MMIIM_SOGOU_MULTITAP
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_lao =
{
    /*1*/  {2, 0x0ED1, 0x0031},
    /*2*/  {9, 0x0E81, 0x0E82, 0x0E84, 0x0E87, 0x0E88, 0x0E8A, 0x0E8D, 0x0ED2, 0x0032},
    /*3*/  {10, 0x0E94, 0x0E95, 0x0E96, 0x0E97, 0x0E99, 0x0E9A, 0x0E9B, 0x0E9C, 0x0ED3, 0x0033},
    /*4*/  {9, 0x0E9D, 0x0E9E, 0x0E9F, 0x0EA1, 0x0EA2, 0x0EA3, 0x0EA5, 0x0ED4, 0x0034},
    /*5*/  {9, 0x0EA7, 0x0EAA, 0x0EAB, 0x0EAD, 0x0EAE, 0x0EDC, 0x0EDD, 0x0ED5, 0x0035},
    /*6*/  {8, 0x0EAF, 0x0EBC, 0x0EBD, 0x0EC6, 0x0ECC, 0x0ECD, 0x0ED6, 0x0036},
    /*7*/  {8, 0x0EB0, 0x0EB1, 0x0EB2, 0x0EB3, 0x0EB4, 0x0EB5, 0x0ED7, 0x0037},
    /*8*/  {9, 0x0EB6, 0x0EB7, 0x0EB8, 0x0EB9, 0x0EBB, 0x0EC0, 0x0EC1, 0x0ED8, 0x0038},
    /*9*/  {9, 0x0EC2, 0x0EC3, 0x0EC4, 0x0EC8, 0x0EC9, 0x0ECA, 0x0ECB, 0x0ED9, 0x0039},
    /*0*/  {2, 0x0ED0, 0x0030},
};
#else   //使用sogou数据替代
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_lao =
{
    /*1*/  {2, 0x0ED1, 0x0031},
    /*2*/  {9, 0x0E81, 0x0E82, 0x0E84, 0x0E87, 0x0E88, 0x0E8A, 0x0E8D, 0x0ED2, 0x0032},
    /*3*/  {10, 0x0E94, 0x0E95, 0x0E96, 0x0E97, 0x0E99, 0x0E9A, 0x0E9B, 0x0E9C, 0x0ED3, 0x0033},
    /*4*/  {9, 0x0E9D, 0x0E9E, 0x0E9F, 0x0EA1, 0x0EA2, 0x0EA3, 0x0EA5, 0x0ED4, 0x0034},
    /*5*/  {9, 0x0EA7, 0x0EAA, 0x0EAB, 0x0EAD, 0x0EAE, 0x0ED5, 0x0EDC, 0x0EDD, 0x0035},
    /*6*/  {8, 0x0EAF, 0x0EBC, 0x0EBD, 0x0EC6, 0x0ECC, 0x0ECD, 0x0ED6, 0x0036},
    /*7*/  {8, 0x0EB0, 0x0EB1, 0x0EB2, 0x0EB3, 0x0EB4, 0x0EB5, 0x0ED7, 0x0037},
    /*8*/  {9, 0x0EB6, 0x0EB7, 0x0EB8, 0x0EB9, 0x0EBB, 0x0EC0, 0x0EC1, 0x0ED8, 0x0038},
    /*9*/  {9, 0x0EC2, 0x0EC3, 0x0EC4, 0x0EC8, 0x0EC9, 0x0ECA, 0x0ECB, 0x0ED9, 0x0039},
    /*0*/  {2, 0x0ED0, 0x0030},
};
#endif
#endif

#ifdef IM_UYGHUR_SUPPORT
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_uyghur =
{
    /*1*/  {7, 0x0626, 0x002E, 0x060C, 0x061F, 0x0021, 0x061B, 0x0031},
    /*2*/  {6, 0x0627, 0x06D5, 0x0628, 0x067E, 0x062A, 0x0032},
    /*3*/  {4, 0x062C, 0x0686, 0x062E, 0x0033},
    /*4*/  {5, 0x062F, 0x0631, 0x0632, 0x0698, 0x0034},
    /*5*/  {6, 0x0633, 0x0634, 0x063A, 0x0641, 0x0642, 0x0035},
    /*6*/  {5, 0x0643, 0x06AF, 0x06AD, 0x0644, 0x0036},
    /*7*/  {4, 0x0645, 0x0646, 0x06BE, 0x0037},
    /*8*/  {5, 0x0648, 0x06C7, 0x06C6, 0x06C8, 0x0038},
    /*9*/  {5, 0x06CB, 0x06D0, 0x0649, 0x064A, 0x0039},
    /*0*/  {1, 0x0030},
};
#endif

#ifdef  IM_TIBETAN_SUPPORT    
#if defined(MMIIM_CSTAR_MULTITAP)
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_tibetan =
{
    /*1*/  {3, 0x0F0B, 0x0F0D, 0x0F21},
    /*2*/  {5, 0x0F72, 0x0F74, 0x0F7A, 0x0F7C, 0x022,},
    /*3*/  {5, 0x0F40, 0x0F41, 0x0F42, 0x0F44,0x0F23},
    /*4*/  {5, 0x0F45, 0x0F46, 0x0F47, 0x0F49, 0x0F24},
    /*5*/  {5, 0x0F4F, 0x0F50, 0x0F51, 0x0F53, 0x0F25},
    /*6*/  {5, 0x0F54, 0x0F55, 0x0F56, 0x0F58, 0x0F26},
    /*7*/  {5, 0x0F59, 0x0F5A, 0x0F5B, 0x0F5C,0x0F27},
    /*8*/  {5, 0x0F5E, 0x0f5F, 0x0F60, 0x0F61, 0x0F28},
    /*9*/  {7, 0x0F62, 0x0F63, 0x0F64, 0x0F66, 0x0F67,0x0F68,0x0F29},
    /*0*/  {2, 0x0F20,0x0020},
};
#else
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_tibetan =
{
    /*1*/  {3, 0x0F0B, 0x0F0D, 0x0F21},
    /*2*/  {5, 0x0F72, 0x0F74, 0x0F7A, 0x0F7C, 0x022,},
    /*3*/  {5, 0x0F40, 0x0F41, 0x0F42, 0x0F44,0x0F23},
    /*4*/  {5, 0x0F45, 0x0F46, 0x0F47, 0x0F49, 0x0F24},
    /*5*/  {5, 0x0F4F, 0x0F50, 0x0F51, 0x0F53, 0x0F25},
    /*6*/  {5, 0x0F54, 0x0F55, 0x0F56, 0x0F58, 0x0F26},
    /*7*/  {5, 0x0F59, 0x0F5A, 0x0F5B, 0x0F5C,0x0F27},
    /*8*/  {5, 0x0F5E, 0x0f5F, 0x0F60, 0x0F61, 0x0F28},
    /*9*/  {7, 0x0F62, 0x0F63, 0x0F64, 0x0F66, 0x0F67,0x0F68,0x0F29},
    /*0*/  {2, 0x0F20,0x0020},
};
#endif
#if defined MMI_IM_PDA_SUPPORT	//仅仅cstar存在相关语言，故统一使用统一种数据
static MMIIM_SP_9KEY_TABLE_DEF_T s_print_key_table_abc_tibetan =
{
    /*1*/  {3, 0x0F0B, 0x0F0D, 0x0F21},
    /*2*/  {3, 0x0F72, '-', 0x0F7C},
    /*3*/  {3, 0x0F40, '-', 0x0F44},
    /*4*/  {3, 0x0F45, '-', 0x0F49},
    /*5*/  {3, 0x0F4F, '-', 0x0F53},
    /*6*/  {3, 0x0F54, '-', 0x0F58},
    /*7*/  {3, 0x0F59, '-', 0x0F5C},
    /*8*/  {3, 0x0F5E, '-', 0x0F61},
    /*9*/  {3, 0x0F62, '-',0x0F68},
    /*0*/  {2, 0x0F20,0x0020},
};
#endif
#endif

#ifdef IM_SINHALESE_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP  //sogou版本
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_sinhalese =
{
    /*1*/  {11, 0x0D85, 0x0D86, 0x0D87, 0x0D88, 0x0D89, 0x0D8A, 0x0D8B, 0x0D8C, 0x0D8D, 0x0D8E, 0x0031},
    /*2*/  {9, 0x0D8F, 0x0D90, 0x0D91, 0x0D92, 0x0D93, 0x0D94, 0x0D95, 0x0D96, 0x0032},
    /*3*/  {12, 0x0D82, 0x0D83, 0x0DCA, 0x0DCF, 0x0DD0, 0x0DD1, 0x0DD2, 0x0DD3, 0x0DD4, 0x0DD6, 0x0DD8, 0x0033},
    /*4*/  {11, 0x0DD9, 0x0DDA, 0x0DDB, 0x0DDC, 0x0DDD, 0x0DDE, 0x0DDF, 0x0DF2, 0x0DF3, 0x0DF4, 0x0034},
    /*5*/  {10, 0x0D9A, 0x0D9B, 0x0D9C, 0x0D9D, 0x0D9E, 0x0D9F, 0x0DA0, 0x0DA1, 0x0DA2, 0x0035},
    /*6*/  {9, 0x0DA3, 0x0DA4, 0x0DA5, 0x0DA6, 0x0DA7, 0x0DA8, 0x0DA9, 0x0DAA, 0x0036},
    /*7*/  {9, 0x0DAB, 0x0DAC, 0x0DAD, 0x0DAE, 0x0DAF, 0x0DB0, 0x0DB1, 0x0DB3, 0x0037},
    /*8*/  {10, 0x0DB4, 0x0DB5, 0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA, 0x0DBB, 0x0DBD, 0x0038},
    /*9*/  {8, 0x0DC0, 0x0DC1, 0x0DC2, 0x0DC3, 0x0DC4, 0x0DC5, 0x0DC6, 0x0039},
    /*0*/  {1, 0x0030},
};
#else	//sogou版本代替
static MMIIM_SP_9KEY_TABLE_DEF_T s_key_table_abc_sinhalese =
{
    /*1*/  {11, 0x0D85, 0x0D86, 0x0D87, 0x0D88, 0x0D89, 0x0D8A, 0x0D8B, 0x0D8C, 0x0D8D, 0x0D8E, 0x0031},
    /*2*/  {9, 0x0D8F, 0x0D90, 0x0D91, 0x0D92, 0x0D93, 0x0D94, 0x0D95, 0x0D96, 0x0032},
    /*3*/  {12, 0x0D82, 0x0D83, 0x0DCA, 0x0DCF, 0x0DD0, 0x0DD1, 0x0DD2, 0x0DD3, 0x0DD4, 0x0DD6, 0x0DD8, 0x0033},
    /*4*/  {11, 0x0DD9, 0x0DDA, 0x0DDB, 0x0DDC, 0x0DDD, 0x0DDE, 0x0DDF, 0x0DF2, 0x0DF3, 0x0DF4, 0x0034},
    /*5*/  {10, 0x0D9A, 0x0D9B, 0x0D9C, 0x0D9D, 0x0D9E, 0x0D9F, 0x0DA0, 0x0DA1, 0x0DA2, 0x0035},
    /*6*/  {9, 0x0DA3, 0x0DA4, 0x0DA5, 0x0DA6, 0x0DA7, 0x0DA8, 0x0DA9, 0x0DAA, 0x0036},
    /*7*/  {9, 0x0DAB, 0x0DAC, 0x0DAD, 0x0DAE, 0x0DAF, 0x0DB0, 0x0DB1, 0x0DB3, 0x0037},
    /*8*/  {10, 0x0DB4, 0x0DB5, 0x0DB6, 0x0DB7, 0x0DB8, 0x0DB9, 0x0DBA, 0x0DBB, 0x0DBD, 0x0038},
    /*9*/  {8, 0x0DC0, 0x0DC1, 0x0DC2, 0x0DC3, 0x0DC4, 0x0DC5, 0x0DC6, 0x0039},
    /*0*/  {1, 0x0030},
};
#endif
#endif

// 输入法信息
static MMIIM_SP_9KEY_TABLE_CFG_T s_spim_mt_lang_data[] =
{
    {GUIIM_M_123, &s_key_table_123},

    {GUIIM_M_TP_123, &s_key_table_123},
    {GUIIM_M_SYS, &s_key_table_abc_lower},

// 英语
    {GUIIM_M_ENGLISH_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_ENGLISH_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_ENGLISH_ABC_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_ENGLISH_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_ENGLISH_ABC_UPPER,      &s_key_table_abc_upper},

// 南非的公用荷兰语
#ifdef IM_AFRIKAANS_SUPPORT
    {GUIIM_M_AFRIKAANS_ABC_LEAD_UPPER, &s_key_table_afrikaans_upper},
    {GUIIM_M_AFRIKAANS_ABC_LOWER,      &s_key_table_afrikaans_lower},
    {GUIIM_M_AFRIKAANS_ABC_UPPER,      &s_key_table_afrikaans_upper},

    {GUIIM_M_TP_AFRIKAANS_ABC_LEAD_UPPER, &s_key_table_afrikaans_upper},
    {GUIIM_M_TP_AFRIKAANS_ABC_LOWER,      &s_key_table_afrikaans_lower},
    {GUIIM_M_TP_AFRIKAANS_ABC_UPPER,      &s_key_table_afrikaans_upper},
#endif

// 阿尔巴尼亚语
#ifdef IM_ALBANIAN_SUPPORT
    {GUIIM_M_ALBANIAN_ABC_LEAD_UPPER, &s_key_table_albanian_upper},
    {GUIIM_M_ALBANIAN_ABC_LOWER,      &s_key_table_albanian_lower},
    {GUIIM_M_ALBANIAN_ABC_UPPER,      &s_key_table_albanian_upper},

    {GUIIM_M_TP_ALBANIAN_ABC_LEAD_UPPER, &s_key_table_albanian_upper},
    {GUIIM_M_TP_ALBANIAN_ABC_LOWER,      &s_key_table_albanian_lower},
    {GUIIM_M_TP_ALBANIAN_ABC_UPPER,      &s_key_table_albanian_upper},
#endif

// 阿拉伯语
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_ARABIC_ABC, &s_key_table_arabic},
    {GUIIM_M_TP_ARABIC_ABC, &s_key_table_arabic},
#endif

// 亚美尼亚语
#ifdef IM_ARMENIAN_SUPPORT
    {GUIIM_M_ARMENIAN_ABC_LEAD_UPPER, &s_key_table_armenian_upper},
    {GUIIM_M_ARMENIAN_ABC_LOWER,      &s_key_table_armenian_lower},
    {GUIIM_M_ARMENIAN_ABC_UPPER,      &s_key_table_armenian_upper},

    {GUIIM_M_TP_ARMENIAN_ABC_LEAD_UPPER, &s_key_table_armenian_upper},
    {GUIIM_M_TP_ARMENIAN_ABC_LOWER,      &s_key_table_armenian_lower},
    {GUIIM_M_TP_ARMENIAN_ABC_UPPER,      &s_key_table_armenian_upper},
#endif

// 阿萨姆
#ifdef IM_ASSAMESE_SUPPORT
    {GUIIM_M_ASSAMESE_ABC, &s_key_table_assamese},
    {GUIIM_M_TP_ASSAMESE_ABC, &s_key_table_assamese},
#endif

// 阿塞拜疆语
#ifdef IM_AZERBAIJANI_SUPPORT
    {GUIIM_M_AZERBAIJANI_ABC_LEAD_UPPER, &s_key_table_azerbaijani_upper},
    {GUIIM_M_AZERBAIJANI_ABC_LOWER,      &s_key_table_azerbaijani_lower},
    {GUIIM_M_AZERBAIJANI_ABC_UPPER,      &s_key_table_azerbaijani_upper},

    {GUIIM_M_TP_AZERBAIJANI_ABC_LEAD_UPPER, &s_key_table_azerbaijani_upper},
    {GUIIM_M_TP_AZERBAIJANI_ABC_LOWER,      &s_key_table_azerbaijani_lower},
    {GUIIM_M_TP_AZERBAIJANI_ABC_UPPER,      &s_key_table_azerbaijani_upper},
#endif

// 巴斯克语
#ifdef IM_BASQUE_SUPPORT
    {GUIIM_M_BASQUE_ABC_LEAD_UPPER, &s_key_table_basque_upper},
    {GUIIM_M_BASQUE_ABC_LOWER,      &s_key_table_basque_lower},
    {GUIIM_M_BASQUE_ABC_UPPER,      &s_key_table_basque_upper},

    {GUIIM_M_TP_BASQUE_ABC_LEAD_UPPER, &s_key_table_basque_upper},
    {GUIIM_M_TP_BASQUE_ABC_LOWER,      &s_key_table_basque_lower},
    {GUIIM_M_TP_BASQUE_ABC_UPPER,      &s_key_table_basque_upper},
#endif

// 孟加拉语
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_BENGALI_ABC, &s_key_table_bengali},

    {GUIIM_M_TP_BENGALI_ABC, &s_key_table_bengali},
#endif

// 保加利亚语
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_BULGARIAN_ABC_LEAD_UPPER, &s_key_table_bulgarian_upper},
    {GUIIM_M_BULGARIAN_ABC_LOWER,      &s_key_table_bulgarian_lower},
    {GUIIM_M_BULGARIAN_ABC_UPPER,      &s_key_table_bulgarian_upper},

    {GUIIM_M_TP_BULGARIAN_ABC_LEAD_UPPER, &s_key_table_bulgarian_upper},
    {GUIIM_M_TP_BULGARIAN_ABC_LOWER,      &s_key_table_bulgarian_lower},
    {GUIIM_M_TP_BULGARIAN_ABC_UPPER,      &s_key_table_bulgarian_upper},
#endif

// 加泰罗尼亚语
#ifdef IM_CATALAN_SUPPORT
    {GUIIM_M_CATALAN_ABC_LEAD_UPPER, &s_key_table_catalan_upper},
    {GUIIM_M_CATALAN_ABC_LOWER,      &s_key_table_catalan_lower},
    {GUIIM_M_CATALAN_ABC_UPPER,      &s_key_table_catalan_upper},

    {GUIIM_M_TP_CATALAN_ABC_LEAD_UPPER, &s_key_table_catalan_upper},
    {GUIIM_M_TP_CATALAN_ABC_LOWER,      &s_key_table_catalan_lower},
    {GUIIM_M_TP_CATALAN_ABC_UPPER,      &s_key_table_catalan_upper},
#endif

// 克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_CROATIAN_ABC_LEAD_UPPER, &s_key_table_croatian_upper},
    {GUIIM_M_CROATIAN_ABC_LOWER,      &s_key_table_croatian_lower},
    {GUIIM_M_CROATIAN_ABC_UPPER,      &s_key_table_croatian_upper},

    {GUIIM_M_TP_CROATIAN_ABC_LEAD_UPPER, &s_key_table_croatian_upper},
    {GUIIM_M_TP_CROATIAN_ABC_LOWER,      &s_key_table_croatian_lower},
    {GUIIM_M_TP_CROATIAN_ABC_UPPER,      &s_key_table_croatian_upper},
#endif

// 捷克语
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_CZECH_ABC_LEAD_UPPER, &s_key_table_czech_upper},
    {GUIIM_M_CZECH_ABC_LOWER,      &s_key_table_czech_lower},
    {GUIIM_M_CZECH_ABC_UPPER,      &s_key_table_czech_upper},

    {GUIIM_M_TP_CZECH_ABC_LEAD_UPPER, &s_key_table_czech_upper},
    {GUIIM_M_TP_CZECH_ABC_LOWER,      &s_key_table_czech_lower},
    {GUIIM_M_TP_CZECH_ABC_UPPER,      &s_key_table_czech_upper},
#endif

// 丹麦语
#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_DANISH_ABC_LEAD_UPPER, &s_key_table_danish_upper},
    {GUIIM_M_DANISH_ABC_LOWER,      &s_key_table_danish_lower},
    {GUIIM_M_DANISH_ABC_UPPER,      &s_key_table_danish_upper},

    {GUIIM_M_TP_DANISH_ABC_LEAD_UPPER, &s_key_table_danish_upper},
    {GUIIM_M_TP_DANISH_ABC_LOWER,      &s_key_table_danish_lower},
    {GUIIM_M_TP_DANISH_ABC_UPPER,      &s_key_table_danish_upper},
#endif

// 荷兰语
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_DUTCH_ABC_LEAD_UPPER, &s_key_table_dutch_upper},
    {GUIIM_M_DUTCH_ABC_LOWER,      &s_key_table_dutch_lower},
    {GUIIM_M_DUTCH_ABC_UPPER,      &s_key_table_dutch_upper},

    {GUIIM_M_TP_DUTCH_ABC_LEAD_UPPER, &s_key_table_dutch_upper},
    {GUIIM_M_TP_DUTCH_ABC_LOWER,      &s_key_table_dutch_lower},
    {GUIIM_M_TP_DUTCH_ABC_UPPER,      &s_key_table_dutch_upper},
#endif

// 爱沙尼亚语
#ifdef IM_ESTONIAN_SUPPORT
    {GUIIM_M_ESTONIAN_ABC_LEAD_UPPER, &s_key_table_estonian_upper},
    {GUIIM_M_ESTONIAN_ABC_LOWER,      &s_key_table_estonian_lower},
    {GUIIM_M_ESTONIAN_ABC_UPPER,      &s_key_table_estonian_upper},

    {GUIIM_M_TP_ESTONIAN_ABC_LEAD_UPPER, &s_key_table_estonian_upper},
    {GUIIM_M_TP_ESTONIAN_ABC_LOWER,      &s_key_table_estonian_lower},
    {GUIIM_M_TP_ESTONIAN_ABC_UPPER,      &s_key_table_estonian_upper},
#endif

// 菲律宾语
#ifdef IM_FILIPINO_SUPPORT
    {GUIIM_M_FILIPINO_ABC_LEAD_UPPER, &s_key_table_filipino_upper},
    {GUIIM_M_FILIPINO_ABC_LOWER,      &s_key_table_filipino_lower},
    {GUIIM_M_FILIPINO_ABC_UPPER,      &s_key_table_filipino_upper},

    {GUIIM_M_TP_FILIPINO_ABC_LEAD_UPPER, &s_key_table_filipino_upper},
    {GUIIM_M_TP_FILIPINO_ABC_LOWER,      &s_key_table_filipino_lower},
    {GUIIM_M_TP_FILIPINO_ABC_UPPER,      &s_key_table_filipino_upper},
#endif

// 芬兰语
#ifdef IM_FINNISH_SUPPORT
    {GUIIM_M_FINNISH_ABC_LEAD_UPPER, &s_key_table_finnish_upper},
    {GUIIM_M_FINNISH_ABC_LOWER,      &s_key_table_finnish_lower},
    {GUIIM_M_FINNISH_ABC_UPPER,      &s_key_table_finnish_upper},

    {GUIIM_M_TP_FINNISH_ABC_LEAD_UPPER, &s_key_table_finnish_upper},
    {GUIIM_M_TP_FINNISH_ABC_LOWER,      &s_key_table_finnish_lower},
    {GUIIM_M_TP_FINNISH_ABC_UPPER,      &s_key_table_finnish_upper},
#endif

// 法语
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_FRENCH_ABC_LEAD_UPPER, &s_key_table_french_upper},
    {GUIIM_M_FRENCH_ABC_LOWER,      &s_key_table_french_lower},
    {GUIIM_M_FRENCH_ABC_UPPER,      &s_key_table_french_upper},

    {GUIIM_M_TP_FRENCH_ABC_LEAD_UPPER, &s_key_table_french_upper},
    {GUIIM_M_TP_FRENCH_ABC_LOWER,      &s_key_table_french_lower},
    {GUIIM_M_TP_FRENCH_ABC_UPPER,      &s_key_table_french_upper},
#endif

// 加利西亚语
#ifdef IM_GALICIAN_SUPPORT
    {GUIIM_M_GALICIAN_ABC_LEAD_UPPER, &s_key_table_galician_upper},
    {GUIIM_M_GALICIAN_ABC_LOWER,      &s_key_table_galician_lower},
    {GUIIM_M_GALICIAN_ABC_UPPER,      &s_key_table_galician_upper},

    {GUIIM_M_TP_GALICIAN_ABC_LEAD_UPPER, &s_key_table_galician_upper},
    {GUIIM_M_TP_GALICIAN_ABC_LOWER,      &s_key_table_galician_lower},
    {GUIIM_M_TP_GALICIAN_ABC_UPPER,      &s_key_table_galician_upper},
#endif

// 格鲁吉亚语
#ifdef IM_GEORGIAN_SUPPORT
    {GUIIM_M_GEORGIAN_ABC, &s_key_table_georgian},

    {GUIIM_M_TP_GEORGIAN_ABC, &s_key_table_georgian},
#endif

// 德语
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_GERMAN_ABC_LEAD_UPPER, &s_key_table_german_upper},
    {GUIIM_M_GERMAN_ABC_LOWER,      &s_key_table_german_lower},
    {GUIIM_M_GERMAN_ABC_UPPER,      &s_key_table_german_upper},

    {GUIIM_M_TP_GERMAN_ABC_LEAD_UPPER, &s_key_table_german_upper},
    {GUIIM_M_TP_GERMAN_ABC_LOWER,      &s_key_table_german_lower},
    {GUIIM_M_TP_GERMAN_ABC_UPPER,      &s_key_table_german_upper},
#endif

// 希腊语
#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_GREEK_ABC_LEAD_UPPER, &s_key_table_greek_upper},
    {GUIIM_M_GREEK_ABC_LOWER,      &s_key_table_greek_lower},
    {GUIIM_M_GREEK_ABC_UPPER,      &s_key_table_greek_upper},

    {GUIIM_M_TP_GREEK_ABC_LEAD_UPPER, &s_key_table_greek_upper},
    {GUIIM_M_TP_GREEK_ABC_LOWER,      &s_key_table_greek_lower},
    {GUIIM_M_TP_GREEK_ABC_UPPER,      &s_key_table_greek_upper},
#endif

// 印度古吉拉特语
#ifdef IM_GUJARATI_SUPPORT
    {GUIIM_M_GUJARATI_ABC, &s_key_table_gujarati},

    {GUIIM_M_TP_GUJARATI_ABC, &s_key_table_gujarati},
#endif

// 豪撒语
#ifdef IM_HAUSA_SUPPORT
    {GUIIM_M_HAUSA_ABC_LEAD_UPPER, &s_key_table_hausa_upper},
    {GUIIM_M_HAUSA_ABC_LOWER,      &s_key_table_hausa_lower},
    {GUIIM_M_HAUSA_ABC_UPPER,      &s_key_table_hausa_upper},

    {GUIIM_M_TP_HAUSA_ABC_LEAD_UPPER, &s_key_table_hausa_upper},
    {GUIIM_M_TP_HAUSA_ABC_LOWER,      &s_key_table_hausa_lower},
    {GUIIM_M_TP_HAUSA_ABC_UPPER,      &s_key_table_hausa_upper},
#endif

// 希伯来语
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_HEBREW_ABC, &s_key_table_hebrew},

    {GUIIM_M_TP_HEBREW_ABC, &s_key_table_hebrew},
#endif

// 印地语
#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_HINDI_ABC, &s_key_table_hindi},

    {GUIIM_M_TP_HINDI_ABC, &s_key_table_hindi},
#endif

// 匈牙利语
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_HUNGARIAN_ABC_LEAD_UPPER, &s_key_table_hungarian_upper},
    {GUIIM_M_HUNGARIAN_ABC_LOWER,      &s_key_table_hungarian_lower},
    {GUIIM_M_HUNGARIAN_ABC_UPPER,      &s_key_table_hungarian_upper},

    {GUIIM_M_TP_HUNGARIAN_ABC_LEAD_UPPER, &s_key_table_hungarian_upper},
    {GUIIM_M_TP_HUNGARIAN_ABC_LOWER,      &s_key_table_hungarian_lower},
    {GUIIM_M_TP_HUNGARIAN_ABC_UPPER,      &s_key_table_hungarian_upper},
#endif

// 冰岛语
#ifdef IM_ICELANDIC_SUPPORT
    {GUIIM_M_ICELANDIC_ABC_LEAD_UPPER, &s_key_table_icelandic_upper},
    {GUIIM_M_ICELANDIC_ABC_LOWER,      &s_key_table_icelandic_lower},
    {GUIIM_M_ICELANDIC_ABC_UPPER,      &s_key_table_icelandic_upper},

    {GUIIM_M_TP_ICELANDIC_ABC_LEAD_UPPER, &s_key_table_icelandic_upper},
    {GUIIM_M_TP_ICELANDIC_ABC_LOWER,      &s_key_table_icelandic_lower},
    {GUIIM_M_TP_ICELANDIC_ABC_UPPER,      &s_key_table_icelandic_upper},
#endif

// 伊博语
#ifdef IM_IGBO_SUPPORT
    {GUIIM_M_IGBO_ABC_LEAD_UPPER, &s_key_table_igbo_upper},
    {GUIIM_M_IGBO_ABC_LOWER,      &s_key_table_igbo_lower},
    {GUIIM_M_IGBO_ABC_UPPER,      &s_key_table_igbo_upper},

    {GUIIM_M_TP_IGBO_ABC_LEAD_UPPER, &s_key_table_igbo_upper},
    {GUIIM_M_TP_IGBO_ABC_LOWER,      &s_key_table_igbo_lower},
    {GUIIM_M_TP_IGBO_ABC_UPPER,      &s_key_table_igbo_upper},
#endif

// 印度尼西亚语
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_INDONESIAN_ABC_LEAD_UPPER, &s_key_table_indonesian_upper},
    {GUIIM_M_INDONESIAN_ABC_LOWER,      &s_key_table_indonesian_lower},
    {GUIIM_M_INDONESIAN_ABC_UPPER,      &s_key_table_indonesian_upper},

    {GUIIM_M_TP_INDONESIAN_ABC_LEAD_UPPER, &s_key_table_indonesian_upper},
    {GUIIM_M_TP_INDONESIAN_ABC_LOWER,      &s_key_table_indonesian_lower},
    {GUIIM_M_TP_INDONESIAN_ABC_UPPER,      &s_key_table_indonesian_upper},
#endif

// 爱尔兰语
#ifdef IM_IRISH_SUPPORT
    {GUIIM_M_IRISH_ABC_LEAD_UPPER, &s_key_table_irish_upper},
    {GUIIM_M_IRISH_ABC_LOWER,      &s_key_table_irish_lower},
    {GUIIM_M_IRISH_ABC_UPPER,      &s_key_table_irish_upper},

    {GUIIM_M_TP_IRISH_ABC_LEAD_UPPER, &s_key_table_irish_upper},
    {GUIIM_M_TP_IRISH_ABC_LOWER,      &s_key_table_irish_lower},
    {GUIIM_M_TP_IRISH_ABC_UPPER,      &s_key_table_irish_upper},
#endif

// 意大利语
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_ITALIAN_ABC_LEAD_UPPER, &s_key_table_italian_upper},
    {GUIIM_M_ITALIAN_ABC_LOWER,      &s_key_table_italian_lower},
    {GUIIM_M_ITALIAN_ABC_UPPER,      &s_key_table_italian_upper},

    {GUIIM_M_TP_ITALIAN_ABC_LEAD_UPPER, &s_key_table_italian_upper},
    {GUIIM_M_TP_ITALIAN_ABC_LOWER,      &s_key_table_italian_lower},
    {GUIIM_M_TP_ITALIAN_ABC_UPPER,      &s_key_table_italian_upper},
#endif

// 印度卡纳达语
#ifdef IM_KANNADA_SUPPORT
    {GUIIM_M_KANNADA_ABC, &s_key_table_kannada},

    {GUIIM_M_TP_KANNADA_ABC, &s_key_table_kannada},
#endif

// 哈萨克斯坦
#ifdef IM_KAZAKH_SUPPORT
    {GUIIM_M_KAZAKH_ABC_LEAD_UPPER, &s_key_table_kazakh_upper},
    {GUIIM_M_KAZAKH_ABC_LOWER,      &s_key_table_kazakh_lower},
    {GUIIM_M_KAZAKH_ABC_UPPER,      &s_key_table_kazakh_upper},

    {GUIIM_M_TP_KAZAKH_ABC_LEAD_UPPER, &s_key_table_kazakh_upper},
    {GUIIM_M_TP_KAZAKH_ABC_LOWER,      &s_key_table_kazakh_lower},
    {GUIIM_M_TP_KAZAKH_ABC_UPPER,      &s_key_table_kazakh_upper},
#endif

// 拉脱维亚语
#ifdef IM_LATVIAN_SUPPORT
    {GUIIM_M_LATVIAN_ABC_LEAD_UPPER, &s_key_table_latvian_upper},
    {GUIIM_M_LATVIAN_ABC_LOWER,      &s_key_table_latvian_lower},
    {GUIIM_M_LATVIAN_ABC_UPPER,      &s_key_table_latvian_upper},

    {GUIIM_M_TP_LATVIAN_ABC_LEAD_UPPER, &s_key_table_latvian_upper},
    {GUIIM_M_TP_LATVIAN_ABC_LOWER,      &s_key_table_latvian_lower},
    {GUIIM_M_TP_LATVIAN_ABC_UPPER,      &s_key_table_latvian_upper},
#endif

// 立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_LITHUANIAN_ABC_LEAD_UPPER, &s_key_table_lithuanian_upper},
    {GUIIM_M_LITHUANIAN_ABC_LOWER,      &s_key_table_lithuanian_lower},
    {GUIIM_M_LITHUANIAN_ABC_UPPER,      &s_key_table_lithuanian_upper},

    {GUIIM_M_TP_LITHUANIAN_ABC_LEAD_UPPER, &s_key_table_lithuanian_upper},
    {GUIIM_M_TP_LITHUANIAN_ABC_LOWER,      &s_key_table_lithuanian_lower},
    {GUIIM_M_TP_LITHUANIAN_ABC_UPPER,      &s_key_table_lithuanian_upper},
#endif

// 马其顿
#ifdef IM_MACEDONIAN_SUPPORT
    {GUIIM_M_MACEDONIAN_ABC_LEAD_UPPER, &s_key_table_macedonian_upper},
    {GUIIM_M_MACEDONIAN_ABC_LOWER,      &s_key_table_macedonian_lower},
    {GUIIM_M_MACEDONIAN_ABC_UPPER,      &s_key_table_macedonian_upper},

    {GUIIM_M_TP_MACEDONIAN_ABC_LEAD_UPPER, &s_key_table_macedonian_upper},
    {GUIIM_M_TP_MACEDONIAN_ABC_LOWER,      &s_key_table_macedonian_lower},
    {GUIIM_M_TP_MACEDONIAN_ABC_UPPER,      &s_key_table_macedonian_upper},
#endif

// 印度马拉雅拉姆语
#ifdef IM_MALAYALAM_SUPPORT
    {GUIIM_M_MALAYALAM_ABC, &s_key_table_malayalam},

    {GUIIM_M_TP_MALAYALAM_ABC, &s_key_table_malayalam},
#endif

// 缅甸
#ifdef IM_MYANMAR_SUPPORT
    {GUIIM_M_MYANMAR_ABC, &s_key_table_myanmar},

    {GUIIM_M_TP_MYANMAR_ABC, &s_key_table_myanmar},
#endif

// 马来语
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_MALAY_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_MALAY_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_MALAY_ABC_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_MALAY_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_MALAY_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_MALAY_ABC_UPPER,      &s_key_table_abc_upper},
#endif

// 印度马拉地语
#ifdef IM_MARATHI_SUPPORT
    {GUIIM_M_MARATHI_ABC, &s_key_table_marathi},

    {GUIIM_M_TP_MARATHI_ABC, &s_key_table_marathi},
#endif

// 摩尔多瓦语
#ifdef IM_MOLDOVAN_SUPPORT
    {GUIIM_M_MOLDOVAN_ABC_LEAD_UPPER, &s_key_table_moldovan_upper},
    {GUIIM_M_MOLDOVAN_ABC_LOWER,      &s_key_table_moldovan_lower},
    {GUIIM_M_MOLDOVAN_ABC_UPPER,      &s_key_table_moldovan_upper},

    {GUIIM_M_TP_MOLDOVAN_ABC_LEAD_UPPER, &s_key_table_moldovan_upper},
    {GUIIM_M_TP_MOLDOVAN_ABC_LOWER,      &s_key_table_moldovan_lower},
    {GUIIM_M_TP_MOLDOVAN_ABC_UPPER,      &s_key_table_moldovan_upper},
#endif

// 挪威语
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_NORWEGIAN_ABC_LEAD_UPPER, &s_key_table_norwegian_upper},
    {GUIIM_M_NORWEGIAN_ABC_LOWER,      &s_key_table_norwegian_lower},
    {GUIIM_M_NORWEGIAN_ABC_UPPER,      &s_key_table_norwegian_upper},

    {GUIIM_M_TP_NORWEGIAN_ABC_LEAD_UPPER, &s_key_table_norwegian_upper},
    {GUIIM_M_TP_NORWEGIAN_ABC_LOWER,      &s_key_table_norwegian_lower},
    {GUIIM_M_TP_NORWEGIAN_ABC_UPPER,      &s_key_table_norwegian_upper},
#endif

// 奥里雅印度语
#ifdef IM_ORIYA_SUPPORT
    {GUIIM_M_ORIYA_ABC, &s_key_table_oriya},

    {GUIIM_M_TP_ORIYA_ABC, &s_key_table_oriya},
#endif

// 波斯语
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_PERSIAN_ABC, &s_key_table_persian},

    {GUIIM_M_TP_PERSIAN_ABC, &s_key_table_persian},
#endif

// 波兰语
#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_POLISH_ABC_LEAD_UPPER, &s_key_table_polish_upper},
    {GUIIM_M_POLISH_ABC_LOWER,      &s_key_table_polish_lower},
    {GUIIM_M_POLISH_ABC_UPPER,      &s_key_table_polish_upper},

    {GUIIM_M_TP_POLISH_ABC_LEAD_UPPER, &s_key_table_polish_upper},
    {GUIIM_M_TP_POLISH_ABC_LOWER,      &s_key_table_polish_lower},
    {GUIIM_M_TP_POLISH_ABC_UPPER,      &s_key_table_polish_upper},
#endif

// 葡萄牙语
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_PORTUGUESE_ABC_LEAD_UPPER, &s_key_table_portuguese_upper},
    {GUIIM_M_PORTUGUESE_ABC_LOWER,      &s_key_table_portuguese_lower},
    {GUIIM_M_PORTUGUESE_ABC_UPPER,      &s_key_table_portuguese_upper},

    {GUIIM_M_TP_PORTUGUESE_ABC_LEAD_UPPER, &s_key_table_portuguese_upper},
    {GUIIM_M_TP_PORTUGUESE_ABC_LOWER,      &s_key_table_portuguese_lower},
    {GUIIM_M_TP_PORTUGUESE_ABC_UPPER,      &s_key_table_portuguese_upper},
#endif

// 印度旁遮普语
#ifdef IM_PUNJABI_SUPPORT
    {GUIIM_M_PUNJABI_ABC, &s_key_table_punjabi},

    {GUIIM_M_TP_PUNJABI_ABC, &s_key_table_punjabi},
#endif

// 罗马尼亚语
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_ROMANIAN_ABC_LEAD_UPPER, &s_key_table_romanian_upper},
    {GUIIM_M_ROMANIAN_ABC_LOWER,      &s_key_table_romanian_lower},
    {GUIIM_M_ROMANIAN_ABC_UPPER,      &s_key_table_romanian_upper},

    {GUIIM_M_TP_ROMANIAN_ABC_LEAD_UPPER, &s_key_table_romanian_upper},
    {GUIIM_M_TP_ROMANIAN_ABC_LOWER,      &s_key_table_romanian_lower},
    {GUIIM_M_TP_ROMANIAN_ABC_UPPER,      &s_key_table_romanian_upper},
#endif

// 俄语
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_RUSSIAN_ABC_LEAD_UPPER, &s_key_table_russian_upper},
    {GUIIM_M_RUSSIAN_ABC_LOWER,      &s_key_table_russian_lower},
    {GUIIM_M_RUSSIAN_ABC_UPPER,      &s_key_table_russian_upper},

    {GUIIM_M_TP_RUSSIAN_ABC_LEAD_UPPER, &s_key_table_russian_upper},
    {GUIIM_M_TP_RUSSIAN_ABC_LOWER,      &s_key_table_russian_lower},
    {GUIIM_M_TP_RUSSIAN_ABC_UPPER,      &s_key_table_russian_upper},
#endif

// 塞尔维亚语
#ifdef IM_SERBIAN_SUPPORT
    {GUIIM_M_SERBIAN_ABC_LEAD_UPPER, &s_key_table_serbian_upper},
    {GUIIM_M_SERBIAN_ABC_LOWER,      &s_key_table_serbian_lower},
    {GUIIM_M_SERBIAN_ABC_UPPER,      &s_key_table_serbian_upper},

    {GUIIM_M_TP_SERBIAN_ABC_LEAD_UPPER, &s_key_table_serbian_upper},
    {GUIIM_M_TP_SERBIAN_ABC_LOWER,      &s_key_table_serbian_lower},
    {GUIIM_M_TP_SERBIAN_ABC_UPPER,      &s_key_table_serbian_upper},
#endif

// 塞索托语
#ifdef IM_SESOTHO_SUPPORT
    {GUIIM_M_SESOTHO_ABC_LEAD_UPPER, &s_key_table_sesotho_upper},
    {GUIIM_M_SESOTHO_ABC_LOWER,      &s_key_table_sesotho_lower},
    {GUIIM_M_SESOTHO_ABC_UPPER,      &s_key_table_sesotho_upper},

    {GUIIM_M_TP_SESOTHO_ABC_LEAD_UPPER, &s_key_table_sesotho_upper},
    {GUIIM_M_TP_SESOTHO_ABC_LOWER,      &s_key_table_sesotho_lower},
    {GUIIM_M_TP_SESOTHO_ABC_UPPER,      &s_key_table_sesotho_upper},
#endif

// 斯洛伐克语
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_SLOVAK_ABC_LEAD_UPPER, &s_key_table_slovak_upper},
    {GUIIM_M_SLOVAK_ABC_LOWER,      &s_key_table_slovak_lower},
    {GUIIM_M_SLOVAK_ABC_UPPER,      &s_key_table_slovak_upper},

    {GUIIM_M_TP_SLOVAK_ABC_LEAD_UPPER, &s_key_table_slovak_upper},
    {GUIIM_M_TP_SLOVAK_ABC_LOWER,      &s_key_table_slovak_lower},
    {GUIIM_M_TP_SLOVAK_ABC_UPPER,      &s_key_table_slovak_upper},
#endif

// 斯洛文尼亚语
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_SLOVENIAN_ABC_LEAD_UPPER, &s_key_table_slovenian_upper},
    {GUIIM_M_SLOVENIAN_ABC_LOWER,      &s_key_table_slovenian_lower},
    {GUIIM_M_SLOVENIAN_ABC_UPPER,      &s_key_table_slovenian_upper},

    {GUIIM_M_TP_SLOVENIAN_ABC_LEAD_UPPER, &s_key_table_slovenian_upper},
    {GUIIM_M_TP_SLOVENIAN_ABC_LOWER,      &s_key_table_slovenian_lower},
    {GUIIM_M_TP_SLOVENIAN_ABC_UPPER,      &s_key_table_slovenian_upper},
#endif

// 西班牙语
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_SPANISH_ABC_LEAD_UPPER, &s_key_table_spanish_upper},
    {GUIIM_M_SPANISH_ABC_LOWER,      &s_key_table_spanish_lower},
    {GUIIM_M_SPANISH_ABC_UPPER,      &s_key_table_spanish_upper},

    {GUIIM_M_TP_SPANISH_ABC_LEAD_UPPER, &s_key_table_spanish_upper},
    {GUIIM_M_TP_SPANISH_ABC_LOWER,      &s_key_table_spanish_lower},
    {GUIIM_M_TP_SPANISH_ABC_UPPER,      &s_key_table_spanish_upper},
#endif

// 斯瓦西里语
#ifdef IM_SWAHILI_SUPPORT       //cstar, sogou等均使用abc数据表
    {GUIIM_M_SWAHILI_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_SWAHILI_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_SWAHILI_ABC_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_SWAHILI_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_SWAHILI_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_SWAHILI_ABC_UPPER,      &s_key_table_abc_upper},
#endif

// 瑞典语
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_SWEDISH_ABC_LEAD_UPPER, &s_key_table_swedish_upper},
    {GUIIM_M_SWEDISH_ABC_LOWER,      &s_key_table_swedish_lower},
    {GUIIM_M_SWEDISH_ABC_UPPER,      &s_key_table_swedish_upper},

    {GUIIM_M_TP_SWEDISH_ABC_LEAD_UPPER, &s_key_table_swedish_upper},
    {GUIIM_M_TP_SWEDISH_ABC_LOWER,      &s_key_table_swedish_lower},
    {GUIIM_M_TP_SWEDISH_ABC_UPPER,      &s_key_table_swedish_upper},
#endif

// 他加禄语
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_TAGALOG_ABC, &s_key_table_tagalog},

    {GUIIM_M_TP_TAGALOG_ABC, &s_key_table_tagalog},
#endif

// 泰米尔语
#ifdef IM_TAMIL_SUPPORT
    {GUIIM_M_TAMIL_ABC, &s_key_table_tamil},

    {GUIIM_M_TP_TAMIL_ABC, &s_key_table_tamil},
#endif

// 印度泰卢固语
#ifdef IM_TELUGU_SUPPORT
    {GUIIM_M_TELUGU_ABC, &s_key_table_telugu},

    {GUIIM_M_TP_TELUGU_ABC, &s_key_table_telugu},
#endif

// 泰语
#ifdef IM_THAI_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)
    {GUIIM_M_THAI_ABC, &s_key_table_thai},
    {GUIIM_M_TP_THAI_ABC, &s_key_table_thai},
#elif defined (MMIIM_SOGOU_MULTITAP)
    {GUIIM_M_THAI_ABC, &s_key_table_thai},
    {GUIIM_M_TP_THAI_ABC, &s_key_table_thai},
#else
    {GUIIM_M_THAI_ABC, &s_key_table_thai_normal},

    {GUIIM_M_TP_THAI_ABC, &s_key_table_thai_normal},
#endif    
#endif

// 土耳其语
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_TURKISH_ABC_LEAD_UPPER, &s_key_table_turkish_upper},
    {GUIIM_M_TURKISH_ABC_LOWER,      &s_key_table_turkish_lower},
    {GUIIM_M_TURKISH_ABC_UPPER,      &s_key_table_turkish_upper},

    {GUIIM_M_TP_TURKISH_ABC_LEAD_UPPER, &s_key_table_turkish_upper},
    {GUIIM_M_TP_TURKISH_ABC_LOWER,      &s_key_table_turkish_lower},
    {GUIIM_M_TP_TURKISH_ABC_UPPER,      &s_key_table_turkish_upper},
#endif

// 乌克兰语
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_UKRAINIAN_ABC_LEAD_UPPER, &s_key_table_ukrainian_upper},
    {GUIIM_M_UKRAINIAN_ABC_LOWER,      &s_key_table_ukrainian_lower},
    {GUIIM_M_UKRAINIAN_ABC_UPPER,      &s_key_table_ukrainian_upper},

    {GUIIM_M_TP_UKRAINIAN_ABC_LEAD_UPPER, &s_key_table_ukrainian_upper},
    {GUIIM_M_TP_UKRAINIAN_ABC_LOWER,      &s_key_table_ukrainian_lower},
    {GUIIM_M_TP_UKRAINIAN_ABC_UPPER,      &s_key_table_ukrainian_upper},
#endif

// 乌尔都语
#ifdef IM_URDU_SUPPORT
    {GUIIM_M_URDU_ABC, &s_key_table_urdu},

    {GUIIM_M_TP_URDU_ABC, &s_key_table_urdu},
#endif

// 越南语
#ifdef IM_VIETNAMESE_SUPPORT
#if defined IM_ENGINE_CSTAR	//cstar的按键码并没有将所有的按键字符给写出来，而multi-tap输入法必须将所有字符写出来
    {GUIIM_M_VIETNAMESE_ABC_LEAD_UPPER, &s_key_table_vietnamese_abc_upper},
    {GUIIM_M_VIETNAMESE_ABC_LOWER,      &s_key_table_vietnamese_abc_lower},
    {GUIIM_M_VIETNAMESE_ABC_UPPER,      &s_key_table_vietnamese_abc_upper},

    {GUIIM_M_TP_VIETNAMESE_ABC_LEAD_UPPER, &s_key_table_vietnamese_abc_upper},
    {GUIIM_M_TP_VIETNAMESE_ABC_LOWER,      &s_key_table_vietnamese_abc_lower},
    {GUIIM_M_TP_VIETNAMESE_ABC_UPPER,      &s_key_table_vietnamese_abc_upper},
#else
    {GUIIM_M_VIETNAMESE_ABC_LEAD_UPPER, &s_key_table_vietnamese_upper},
    {GUIIM_M_VIETNAMESE_ABC_LOWER,      &s_key_table_vietnamese_lower},
    {GUIIM_M_VIETNAMESE_ABC_UPPER,      &s_key_table_vietnamese_upper},

    {GUIIM_M_TP_VIETNAMESE_ABC_LEAD_UPPER, &s_key_table_vietnamese_upper},
    {GUIIM_M_TP_VIETNAMESE_ABC_LOWER,      &s_key_table_vietnamese_lower},
    {GUIIM_M_TP_VIETNAMESE_ABC_UPPER,      &s_key_table_vietnamese_upper},
#endif    
#endif

// 科萨语（南非的一种方言）
#ifdef IM_XHOSA_SUPPORT
    {GUIIM_M_XHOSA_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_XHOSA_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_XHOSA_ABC_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_XHOSA_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_XHOSA_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_XHOSA_ABC_UPPER,      &s_key_table_abc_upper},
#endif

// 约鲁巴语
#ifdef IM_YORUBA_SUPPORT
    {GUIIM_M_YORUBA_ABC_LEAD_UPPER, &s_key_table_yoruba_upper},
    {GUIIM_M_YORUBA_ABC_LOWER,      &s_key_table_yoruba_lower},
    {GUIIM_M_YORUBA_ABC_UPPER,      &s_key_table_yoruba_upper},

    {GUIIM_M_TP_YORUBA_ABC_LEAD_UPPER, &s_key_table_yoruba_upper},
    {GUIIM_M_TP_YORUBA_ABC_LOWER,      &s_key_table_yoruba_lower},
    {GUIIM_M_TP_YORUBA_ABC_UPPER,      &s_key_table_yoruba_upper},
#endif

// 祖鲁语（非洲的语种）
#ifdef IM_ZULU_SUPPORT
    {GUIIM_M_ZULU_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_ZULU_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_ZULU_ABC_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_ZULU_ABC_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_ZULU_ABC_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_ZULU_ABC_UPPER,      &s_key_table_abc_upper},
#endif

//阿姆哈拉语数据表还没有提供，暂时使用abc替代
#ifdef IM_AMHARIC_SUPPORT
    {GUIIM_M_AMHARIC_ABC,      &s_key_table_abc_amharic},
    {GUIIM_M_TP_AMHARIC_ABC,      &s_key_table_abc_amharic},
#endif

//柬埔寨
#ifdef IM_KHMER_SUPPORT
    {GUIIM_M_KHMER_ABC,      &s_key_table_abc_khmer},
    {GUIIM_M_TP_KHMER_ABC,      &s_key_table_abc_khmer},
#endif

//老挝
#ifdef IM_LAO_SUPPORT
    {GUIIM_M_LAO_ABC,      &s_key_table_abc_lao},
    {GUIIM_M_TP_LAO_ABC,      &s_key_table_abc_lao},
#endif

#ifdef IM_UYGHUR_SUPPORT
    {GUIIM_M_UYGHUR_ABC,      &s_key_table_abc_uyghur},
    {GUIIM_M_TP_UYGHUR_ABC,      &s_key_table_abc_uyghur},
#endif

#ifdef IM_TIBETAN_SUPPORT   
	{GUIIM_M_TIBETAN_ABC, &s_key_table_abc_tibetan},
	{GUIIM_M_TP_TIBETAN_ABC, &s_key_table_abc_tibetan},
#endif

#ifdef IM_SINHALESE_SUPPORT
    {GUIIM_M_SINHALESE_ABC,      &s_key_table_abc_sinhalese},
    {GUIIM_M_TP_SINHALESE_ABC,      &s_key_table_abc_sinhalese},
#endif
};

//仅仅在虚拟键盘时，才需要绘制丝印
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_CFG_T s_spim_mt_lang_print_data[] =
{
    {GUIIM_M_TP_123, &s_key_table_123},		//保留一个，防止pclint

#if defined IM_URDU_SUPPORT
    {GUIIM_M_TP_URDU_ABC, &s_print_key_table_urdu},
#endif

#if defined IM_BENGALI_SUPPORT
    {GUIIM_M_TP_BENGALI_ABC, &s_print_key_table_bengali},
#endif

#if defined IM_TIBETAN_SUPPORT
    {GUIIM_M_TP_TIBETAN_ABC, &s_print_key_table_abc_tibetan},
#endif
};
#endif

//使用字符串的方式显示虚拟键盘字符
// 输入法信息
static MMIIM_SP_9KEY_TABLE_CFG_T s_spim_smart_lang_data[] =
{
    {GUIIM_M_123, &s_key_table_123},

    {GUIIM_M_TP_123, &s_key_table_123},

// 英语
    {GUIIM_M_ENGLISH_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_ENGLISH_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_ENGLISH_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_ENGLISH_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_ENGLISH_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_ENGLISH_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_PINYIN, &s_key_table_abc_lower},	
    {GUIIM_M_TP_PINYIN, &s_key_table_abc_lower},

//繁体中文丝印数据
#ifdef IM_TRAD_CHINESE_SUPPORT
    {GUIIM_M_TP_ZHUYIN, &s_9key_table_trad_chinese_zhuyin},
#endif

// 南非的公用荷兰语
#ifdef IM_AFRIKAANS_SUPPORT
    {GUIIM_M_AFRIKAANS_LEAD_UPPER, &s_key_table_afrikaans_upper},
    {GUIIM_M_AFRIKAANS_LOWER,      &s_key_table_afrikaans_lower},
    {GUIIM_M_AFRIKAANS_UPPER,      &s_key_table_afrikaans_upper},

    {GUIIM_M_TP_AFRIKAANS_LEAD_UPPER, &s_key_table_afrikaans_upper},
    {GUIIM_M_TP_AFRIKAANS_LOWER,      &s_key_table_afrikaans_lower},
    {GUIIM_M_TP_AFRIKAANS_UPPER,      &s_key_table_afrikaans_upper},
#endif

// 阿尔巴尼亚语
#ifdef IM_ALBANIAN_SUPPORT
    {GUIIM_M_ALBANIAN_LEAD_UPPER, &s_key_table_albanian_upper},
    {GUIIM_M_ALBANIAN_LOWER,      &s_key_table_albanian_lower},
    {GUIIM_M_ALBANIAN_UPPER,      &s_key_table_albanian_upper},

    {GUIIM_M_TP_ALBANIAN_LEAD_UPPER, &s_key_table_albanian_upper},
    {GUIIM_M_TP_ALBANIAN_LOWER,      &s_key_table_albanian_lower},
    {GUIIM_M_TP_ALBANIAN_UPPER,      &s_key_table_albanian_upper},
#endif

// 阿拉伯语
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_ARABIC, &s_key_table_arabic},

    {GUIIM_M_TP_ARABIC, &s_key_table_arabic},
#endif

// 亚美尼亚语
#ifdef IM_ARMENIAN_SUPPORT
    {GUIIM_M_ARMENIAN_LEAD_UPPER, &s_key_table_armenian_upper},
    {GUIIM_M_ARMENIAN_LOWER,      &s_key_table_armenian_lower},
    {GUIIM_M_ARMENIAN_UPPER,      &s_key_table_armenian_upper},

    {GUIIM_M_TP_ARMENIAN_LEAD_UPPER, &s_key_table_armenian_upper},
    {GUIIM_M_TP_ARMENIAN_LOWER,      &s_key_table_armenian_lower},
    {GUIIM_M_TP_ARMENIAN_UPPER,      &s_key_table_armenian_upper},
#endif

// 阿萨姆
#ifdef IM_ASSAMESE_SUPPORT
    //{GUIIM_M_ASSAMESE, &s_key_table_assamese},
    //{GUIIM_M_TP_ASSAMESE, &s_key_table_assamese},
#endif

// 阿塞拜疆语
#ifdef IM_AZERBAIJANI_SUPPORT
    {GUIIM_M_AZERBAIJANI_LEAD_UPPER, &s_key_table_azerbaijani_upper},
    {GUIIM_M_AZERBAIJANI_LOWER,      &s_key_table_azerbaijani_lower},
    {GUIIM_M_AZERBAIJANI_UPPER,      &s_key_table_azerbaijani_upper},

    {GUIIM_M_TP_AZERBAIJANI_LEAD_UPPER, &s_key_table_azerbaijani_upper},
    {GUIIM_M_TP_AZERBAIJANI_LOWER,      &s_key_table_azerbaijani_lower},
    {GUIIM_M_TP_AZERBAIJANI_UPPER,      &s_key_table_azerbaijani_upper},
#endif

// 巴斯克语
#ifdef IM_BASQUE_SUPPORT
    {GUIIM_M_BASQUE_LEAD_UPPER, &s_key_table_basque_upper},
    {GUIIM_M_BASQUE_LOWER,      &s_key_table_basque_lower},
    {GUIIM_M_BASQUE_UPPER,      &s_key_table_basque_upper},

    {GUIIM_M_TP_BASQUE_LEAD_UPPER, &s_key_table_basque_upper},
    {GUIIM_M_TP_BASQUE_LOWER,      &s_key_table_basque_lower},
    {GUIIM_M_TP_BASQUE_UPPER,      &s_key_table_basque_upper},
#endif

// 孟加拉语
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_BENGALI, &s_key_table_bengali},

    {GUIIM_M_TP_BENGALI, &s_key_table_bengali},
#endif

// 保加利亚语
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_BULGARIAN_LEAD_UPPER, &s_key_table_bulgarian_upper},
    {GUIIM_M_BULGARIAN_LOWER,      &s_key_table_bulgarian_lower},
    {GUIIM_M_BULGARIAN_UPPER,      &s_key_table_bulgarian_upper},

    {GUIIM_M_TP_BULGARIAN_LEAD_UPPER, &s_key_table_bulgarian_upper},
    {GUIIM_M_TP_BULGARIAN_LOWER,      &s_key_table_bulgarian_lower},
    {GUIIM_M_TP_BULGARIAN_UPPER,      &s_key_table_bulgarian_upper},
#endif

// 加泰罗尼亚语
#ifdef IM_CATALAN_SUPPORT
    {GUIIM_M_CATALAN_LEAD_UPPER, &s_key_table_catalan_upper},
    {GUIIM_M_CATALAN_LOWER,      &s_key_table_catalan_lower},
    {GUIIM_M_CATALAN_UPPER,      &s_key_table_catalan_upper},

    {GUIIM_M_TP_CATALAN_LEAD_UPPER, &s_key_table_catalan_upper},
    {GUIIM_M_TP_CATALAN_LOWER,      &s_key_table_catalan_lower},
    {GUIIM_M_TP_CATALAN_UPPER,      &s_key_table_catalan_upper},
#endif

// 克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_CROATIAN_LEAD_UPPER, &s_key_table_croatian_upper},
    {GUIIM_M_CROATIAN_LOWER,      &s_key_table_croatian_lower},
    {GUIIM_M_CROATIAN_UPPER,      &s_key_table_croatian_upper},

    {GUIIM_M_TP_CROATIAN_LEAD_UPPER, &s_key_table_croatian_upper},
    {GUIIM_M_TP_CROATIAN_LOWER,      &s_key_table_croatian_lower},
    {GUIIM_M_TP_CROATIAN_UPPER,      &s_key_table_croatian_upper},
#endif

// 捷克语
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_CZECH_LEAD_UPPER, &s_key_table_czech_upper},
    {GUIIM_M_CZECH_LOWER,      &s_key_table_czech_lower},
    {GUIIM_M_CZECH_UPPER,      &s_key_table_czech_upper},

    {GUIIM_M_TP_CZECH_LEAD_UPPER, &s_key_table_czech_upper},
    {GUIIM_M_TP_CZECH_LOWER,      &s_key_table_czech_lower},
    {GUIIM_M_TP_CZECH_UPPER,      &s_key_table_czech_upper},
#endif

// 丹麦语
#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_DANISH_LEAD_UPPER, &s_key_table_danish_upper},
    {GUIIM_M_DANISH_LOWER,      &s_key_table_danish_lower},
    {GUIIM_M_DANISH_UPPER,      &s_key_table_danish_upper},

    {GUIIM_M_TP_DANISH_LEAD_UPPER, &s_key_table_danish_upper},
    {GUIIM_M_TP_DANISH_LOWER,      &s_key_table_danish_lower},
    {GUIIM_M_TP_DANISH_UPPER,      &s_key_table_danish_upper},
#endif

// 荷兰语
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_DUTCH_LEAD_UPPER, &s_key_table_dutch_upper},
    {GUIIM_M_DUTCH_LOWER,      &s_key_table_dutch_lower},
    {GUIIM_M_DUTCH_UPPER,      &s_key_table_dutch_upper},

    {GUIIM_M_TP_DUTCH_LEAD_UPPER, &s_key_table_dutch_upper},
    {GUIIM_M_TP_DUTCH_LOWER,      &s_key_table_dutch_lower},
    {GUIIM_M_TP_DUTCH_UPPER,      &s_key_table_dutch_upper},
#endif

// 爱沙尼亚语
#ifdef IM_ESTONIAN_SUPPORT
    {GUIIM_M_ESTONIAN_LEAD_UPPER, &s_key_table_estonian_upper},
    {GUIIM_M_ESTONIAN_LOWER,      &s_key_table_estonian_lower},
    {GUIIM_M_ESTONIAN_UPPER,      &s_key_table_estonian_upper},

    {GUIIM_M_TP_ESTONIAN_LEAD_UPPER, &s_key_table_estonian_upper},
    {GUIIM_M_TP_ESTONIAN_LOWER,      &s_key_table_estonian_lower},
    {GUIIM_M_TP_ESTONIAN_UPPER,      &s_key_table_estonian_upper},
#endif

// 菲律宾语
#ifdef IM_FILIPINO_SUPPORT
    {GUIIM_M_FILIPINO_LEAD_UPPER, &s_key_table_filipino_upper},
    {GUIIM_M_FILIPINO_LOWER,      &s_key_table_filipino_lower},
    {GUIIM_M_FILIPINO_UPPER,      &s_key_table_filipino_upper},

    {GUIIM_M_TP_FILIPINO_LEAD_UPPER, &s_key_table_filipino_upper},
    {GUIIM_M_TP_FILIPINO_LOWER,      &s_key_table_filipino_lower},
    {GUIIM_M_TP_FILIPINO_UPPER,      &s_key_table_filipino_upper},
#endif

// 芬兰语
#ifdef IM_FINNISH_SUPPORT
    {GUIIM_M_FINNISH_LEAD_UPPER, &s_key_table_finnish_upper},
    {GUIIM_M_FINNISH_LOWER,      &s_key_table_finnish_lower},
    {GUIIM_M_FINNISH_UPPER,      &s_key_table_finnish_upper},

    {GUIIM_M_TP_FINNISH_LEAD_UPPER, &s_key_table_finnish_upper},
    {GUIIM_M_TP_FINNISH_LOWER,      &s_key_table_finnish_lower},
    {GUIIM_M_TP_FINNISH_UPPER,      &s_key_table_finnish_upper},
#endif

// 法语
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_FRENCH_LEAD_UPPER, &s_key_table_french_upper},
    {GUIIM_M_FRENCH_LOWER,      &s_key_table_french_lower},
    {GUIIM_M_FRENCH_UPPER,      &s_key_table_french_upper},

    {GUIIM_M_TP_FRENCH_LEAD_UPPER, &s_key_table_french_upper},
    {GUIIM_M_TP_FRENCH_LOWER,      &s_key_table_french_lower},
    {GUIIM_M_TP_FRENCH_UPPER,      &s_key_table_french_upper},
#endif

// 加利西亚语
#ifdef IM_GALICIAN_SUPPORT
    {GUIIM_M_GALICIAN_LEAD_UPPER, &s_key_table_galician_upper},
    {GUIIM_M_GALICIAN_LOWER,      &s_key_table_galician_lower},
    {GUIIM_M_GALICIAN_UPPER,      &s_key_table_galician_upper},

    {GUIIM_M_TP_GALICIAN_LEAD_UPPER, &s_key_table_galician_upper},
    {GUIIM_M_TP_GALICIAN_LOWER,      &s_key_table_galician_lower},
    {GUIIM_M_TP_GALICIAN_UPPER,      &s_key_table_galician_upper},
#endif

// 格鲁吉亚语
#ifdef IM_GEORGIAN_SUPPORT
    //{GUIIM_M_GEORGIAN, &s_key_table_georgian},

    //{GUIIM_M_TP_GEORGIAN, &s_key_table_georgian},
#endif

// 德语
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_GERMAN_LEAD_UPPER, &s_key_table_german_upper},
    {GUIIM_M_GERMAN_LOWER,      &s_key_table_german_lower},
    {GUIIM_M_GERMAN_UPPER,      &s_key_table_german_upper},

    {GUIIM_M_TP_GERMAN_LEAD_UPPER, &s_key_table_german_upper},
    {GUIIM_M_TP_GERMAN_LOWER,      &s_key_table_german_lower},
    {GUIIM_M_TP_GERMAN_UPPER,      &s_key_table_german_upper},
#endif

// 希腊语
#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_GREEK_LEAD_UPPER, &s_key_table_greek_upper},
    {GUIIM_M_GREEK_LOWER,      &s_key_table_greek_lower},
    {GUIIM_M_GREEK_UPPER,      &s_key_table_greek_upper},

    {GUIIM_M_TP_GREEK_LEAD_UPPER, &s_key_table_greek_upper},
    {GUIIM_M_TP_GREEK_LOWER,      &s_key_table_greek_lower},
    {GUIIM_M_TP_GREEK_UPPER,      &s_key_table_greek_upper},
#endif

// 印度古吉拉特语
#ifdef IM_GUJARATI_SUPPORT
    {GUIIM_M_GUJARATI, &s_key_table_gujarati},

    {GUIIM_M_TP_GUJARATI, &s_key_table_gujarati},
#endif

// 豪撒语
#ifdef IM_HAUSA_SUPPORT
    {GUIIM_M_HAUSA_LEAD_UPPER, &s_key_table_hausa_upper},
    {GUIIM_M_HAUSA_LOWER,      &s_key_table_hausa_lower},
    {GUIIM_M_HAUSA_UPPER,      &s_key_table_hausa_upper},

    {GUIIM_M_TP_HAUSA_LEAD_UPPER, &s_key_table_hausa_upper},
    {GUIIM_M_TP_HAUSA_LOWER,      &s_key_table_hausa_lower},
    {GUIIM_M_TP_HAUSA_UPPER,      &s_key_table_hausa_upper},
#endif

// 希伯来语
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_HEBREW, &s_key_table_hebrew},

    {GUIIM_M_TP_HEBREW, &s_key_table_hebrew},
#endif

// 印地语
#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_HINDI, &s_key_table_hindi},

    {GUIIM_M_TP_HINDI, &s_key_table_hindi},
#endif

// 匈牙利语
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_HUNGARIAN_LEAD_UPPER, &s_key_table_hungarian_upper},
    {GUIIM_M_HUNGARIAN_LOWER,      &s_key_table_hungarian_lower},
    {GUIIM_M_HUNGARIAN_UPPER,      &s_key_table_hungarian_upper},

    {GUIIM_M_TP_HUNGARIAN_LEAD_UPPER, &s_key_table_hungarian_upper},
    {GUIIM_M_TP_HUNGARIAN_LOWER,      &s_key_table_hungarian_lower},
    {GUIIM_M_TP_HUNGARIAN_UPPER,      &s_key_table_hungarian_upper},
#endif

// 冰岛语
#ifdef IM_ICELANDIC_SUPPORT
    {GUIIM_M_ICELANDIC_LEAD_UPPER, &s_key_table_icelandic_upper},
    {GUIIM_M_ICELANDIC_LOWER,      &s_key_table_icelandic_lower},
    {GUIIM_M_ICELANDIC_UPPER,      &s_key_table_icelandic_upper},

    {GUIIM_M_TP_ICELANDIC_LEAD_UPPER, &s_key_table_icelandic_upper},
    {GUIIM_M_TP_ICELANDIC_LOWER,      &s_key_table_icelandic_lower},
    {GUIIM_M_TP_ICELANDIC_UPPER,      &s_key_table_icelandic_upper},
#endif

// 伊博语
#ifdef IM_IGBO_SUPPORT
    {GUIIM_M_IGBO_LEAD_UPPER, &s_key_table_igbo_upper},
    {GUIIM_M_IGBO_LOWER,      &s_key_table_igbo_lower},
    {GUIIM_M_IGBO_UPPER,      &s_key_table_igbo_upper},

    {GUIIM_M_TP_IGBO_LEAD_UPPER, &s_key_table_igbo_upper},
    {GUIIM_M_TP_IGBO_LOWER,      &s_key_table_igbo_lower},
    {GUIIM_M_TP_IGBO_UPPER,      &s_key_table_igbo_upper},
#endif

// 印度尼西亚语
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_INDONESIAN_LEAD_UPPER, &s_key_table_indonesian_upper},
    {GUIIM_M_INDONESIAN_LOWER,      &s_key_table_indonesian_lower},
    {GUIIM_M_INDONESIAN_UPPER,      &s_key_table_indonesian_upper},

    {GUIIM_M_TP_INDONESIAN_LEAD_UPPER, &s_key_table_indonesian_upper},
    {GUIIM_M_TP_INDONESIAN_LOWER,      &s_key_table_indonesian_lower},
    {GUIIM_M_TP_INDONESIAN_UPPER,      &s_key_table_indonesian_upper},
#endif

// 爱尔兰语
#ifdef IM_IRISH_SUPPORT
    {GUIIM_M_IRISH_LEAD_UPPER, &s_key_table_irish_upper},
    {GUIIM_M_IRISH_LOWER,      &s_key_table_irish_lower},
    {GUIIM_M_IRISH_UPPER,      &s_key_table_irish_upper},

    {GUIIM_M_TP_IRISH_LEAD_UPPER, &s_key_table_irish_upper},
    {GUIIM_M_TP_IRISH_LOWER,      &s_key_table_irish_lower},
    {GUIIM_M_TP_IRISH_UPPER,      &s_key_table_irish_upper},
#endif

// 意大利语
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_ITALIAN_LEAD_UPPER, &s_key_table_italian_upper},
    {GUIIM_M_ITALIAN_LOWER,      &s_key_table_italian_lower},
    {GUIIM_M_ITALIAN_UPPER,      &s_key_table_italian_upper},

    {GUIIM_M_TP_ITALIAN_LEAD_UPPER, &s_key_table_italian_upper},
    {GUIIM_M_TP_ITALIAN_LOWER,      &s_key_table_italian_lower},
    {GUIIM_M_TP_ITALIAN_UPPER,      &s_key_table_italian_upper},
#endif

// 印度卡纳达语
#ifdef IM_KANNADA_SUPPORT
    //{GUIIM_M_KANNADA, &s_key_table_kannada},

    //{GUIIM_M_TP_KANNADA, &s_key_table_kannada},
#endif

// 哈萨克斯坦
#ifdef IM_KAZAKH_SUPPORT
    {GUIIM_M_KAZAKH_LEAD_UPPER, &s_key_table_kazakh_upper},
    {GUIIM_M_KAZAKH_LOWER,      &s_key_table_kazakh_lower},
    {GUIIM_M_KAZAKH_UPPER,      &s_key_table_kazakh_upper},

    {GUIIM_M_TP_KAZAKH_LEAD_UPPER, &s_key_table_kazakh_upper},
    {GUIIM_M_TP_KAZAKH_LOWER,      &s_key_table_kazakh_lower},
    {GUIIM_M_TP_KAZAKH_UPPER,      &s_key_table_kazakh_upper},
#endif

// 拉脱维亚语
#ifdef IM_LATVIAN_SUPPORT
    {GUIIM_M_LATVIAN_LEAD_UPPER, &s_key_table_latvian_upper},
    {GUIIM_M_LATVIAN_LOWER,      &s_key_table_latvian_lower},
    {GUIIM_M_LATVIAN_UPPER,      &s_key_table_latvian_upper},

    {GUIIM_M_TP_LATVIAN_LEAD_UPPER, &s_key_table_latvian_upper},
    {GUIIM_M_TP_LATVIAN_LOWER,      &s_key_table_latvian_lower},
    {GUIIM_M_TP_LATVIAN_UPPER,      &s_key_table_latvian_upper},
#endif

// 立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_LITHUANIAN_LEAD_UPPER, &s_key_table_lithuanian_upper},
    {GUIIM_M_LITHUANIAN_LOWER,      &s_key_table_lithuanian_lower},
    {GUIIM_M_LITHUANIAN_UPPER,      &s_key_table_lithuanian_upper},

    {GUIIM_M_TP_LITHUANIAN_LEAD_UPPER, &s_key_table_lithuanian_upper},
    {GUIIM_M_TP_LITHUANIAN_LOWER,      &s_key_table_lithuanian_lower},
    {GUIIM_M_TP_LITHUANIAN_UPPER,      &s_key_table_lithuanian_upper},
#endif

// 马其顿
#ifdef IM_MACEDONIAN_SUPPORT
    {GUIIM_M_MACEDONIAN_LEAD_UPPER, &s_key_table_macedonian_upper},
    {GUIIM_M_MACEDONIAN_LOWER,      &s_key_table_macedonian_lower},
    {GUIIM_M_MACEDONIAN_UPPER,      &s_key_table_macedonian_upper},

    {GUIIM_M_TP_MACEDONIAN_LEAD_UPPER, &s_key_table_macedonian_upper},
    {GUIIM_M_TP_MACEDONIAN_LOWER,      &s_key_table_macedonian_lower},
    {GUIIM_M_TP_MACEDONIAN_UPPER,      &s_key_table_macedonian_upper},
#endif

// 印度马拉雅拉姆语
#ifdef IM_MALAYALAM_SUPPORT
    //{GUIIM_M_MALAYALAM, &s_key_table_malayalam},

    //{GUIIM_M_TP_MALAYALAM, &s_key_table_malayalam},
#endif

// 缅甸
#ifdef IM_MYANMAR_SUPPORT
    {GUIIM_M_MYANMAR, &s_key_table_myanmar},

    {GUIIM_M_TP_MYANMAR, &s_key_table_myanmar},
#endif

// 马来语
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_MALAY_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_MALAY_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_MALAY_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_MALAY_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_MALAY_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_MALAY_UPPER,      &s_key_table_abc_upper},
#endif

// 印度马拉地语
#ifdef IM_MARATHI_SUPPORT
    {GUIIM_M_MARATHI, &s_key_table_marathi},

    {GUIIM_M_TP_MARATHI, &s_key_table_marathi},
#endif

// 摩尔多瓦语
#ifdef IM_MOLDOVAN_SUPPORT
    //{GUIIM_M_MOLDOVAN_LEAD_UPPER, &s_key_table_moldovan_upper},
    //{GUIIM_M_MOLDOVAN_LOWER,      &s_key_table_moldovan_lower},
    //{GUIIM_M_MOLDOVAN_UPPER,      &s_key_table_moldovan_upper},

    //{GUIIM_M_TP_MOLDOVAN_LEAD_UPPER, &s_key_table_moldovan_upper},
    //{GUIIM_M_TP_MOLDOVAN_LOWER,      &s_key_table_moldovan_lower},
    //{GUIIM_M_TP_MOLDOVAN_UPPER,      &s_key_table_moldovan_upper},
#endif

// 挪威语
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_NORWEGIAN_LEAD_UPPER, &s_key_table_norwegian_upper},
    {GUIIM_M_NORWEGIAN_LOWER,      &s_key_table_norwegian_lower},
    {GUIIM_M_NORWEGIAN_UPPER,      &s_key_table_norwegian_upper},

    {GUIIM_M_TP_NORWEGIAN_LEAD_UPPER, &s_key_table_norwegian_upper},
    {GUIIM_M_TP_NORWEGIAN_LOWER,      &s_key_table_norwegian_lower},
    {GUIIM_M_TP_NORWEGIAN_UPPER,      &s_key_table_norwegian_upper},
#endif

// 奥里雅印度语
#ifdef IM_ORIYA_SUPPORT
    {GUIIM_M_ORIYA, &s_key_table_oriya},

    {GUIIM_M_TP_ORIYA, &s_key_table_oriya},
#endif

// 波斯语
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_PERSIAN, &s_key_table_persian},

    {GUIIM_M_TP_PERSIAN, &s_key_table_persian},
#endif

// 波兰语
#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_POLISH_LEAD_UPPER, &s_key_table_polish_upper},
    {GUIIM_M_POLISH_LOWER,      &s_key_table_polish_lower},
    {GUIIM_M_POLISH_UPPER,      &s_key_table_polish_upper},

    {GUIIM_M_TP_POLISH_LEAD_UPPER, &s_key_table_polish_upper},
    {GUIIM_M_TP_POLISH_LOWER,      &s_key_table_polish_lower},
    {GUIIM_M_TP_POLISH_UPPER,      &s_key_table_polish_upper},
#endif

// 葡萄牙语
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_PORTUGUESE_LEAD_UPPER, &s_key_table_portuguese_upper},
    {GUIIM_M_PORTUGUESE_LOWER,      &s_key_table_portuguese_lower},
    {GUIIM_M_PORTUGUESE_UPPER,      &s_key_table_portuguese_upper},

    {GUIIM_M_TP_PORTUGUESE_LEAD_UPPER, &s_key_table_portuguese_upper},
    {GUIIM_M_TP_PORTUGUESE_LOWER,      &s_key_table_portuguese_lower},
    {GUIIM_M_TP_PORTUGUESE_UPPER,      &s_key_table_portuguese_upper},
#endif

// 印度旁遮普语
#ifdef IM_PUNJABI_SUPPORT
    {GUIIM_M_PUNJABI, &s_key_table_punjabi},

    {GUIIM_M_TP_PUNJABI, &s_key_table_punjabi},
#endif

// 罗马尼亚语
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_ROMANIAN_LEAD_UPPER, &s_key_table_romanian_upper},
    {GUIIM_M_ROMANIAN_LOWER,      &s_key_table_romanian_lower},
    {GUIIM_M_ROMANIAN_UPPER,      &s_key_table_romanian_upper},

    {GUIIM_M_TP_ROMANIAN_LEAD_UPPER, &s_key_table_romanian_upper},
    {GUIIM_M_TP_ROMANIAN_LOWER,      &s_key_table_romanian_lower},
    {GUIIM_M_TP_ROMANIAN_UPPER,      &s_key_table_romanian_upper},
#endif

// 俄语
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_RUSSIAN_LEAD_UPPER, &s_key_table_russian_upper},
    {GUIIM_M_RUSSIAN_LOWER,      &s_key_table_russian_lower},
    {GUIIM_M_RUSSIAN_UPPER,      &s_key_table_russian_upper},

    {GUIIM_M_TP_RUSSIAN_LEAD_UPPER, &s_key_table_russian_upper},
    {GUIIM_M_TP_RUSSIAN_LOWER,      &s_key_table_russian_lower},
    {GUIIM_M_TP_RUSSIAN_UPPER,      &s_key_table_russian_upper},
#endif

// 塞尔维亚语
#ifdef IM_SERBIAN_SUPPORT
    {GUIIM_M_SERBIAN_LEAD_UPPER, &s_key_table_serbian_upper},
    {GUIIM_M_SERBIAN_LOWER,      &s_key_table_serbian_lower},
    {GUIIM_M_SERBIAN_UPPER,      &s_key_table_serbian_upper},

    {GUIIM_M_TP_SERBIAN_LEAD_UPPER, &s_key_table_serbian_upper},
    {GUIIM_M_TP_SERBIAN_LOWER,      &s_key_table_serbian_lower},
    {GUIIM_M_TP_SERBIAN_UPPER,      &s_key_table_serbian_upper},
#endif

// 塞索托语
#ifdef IM_SESOTHO_SUPPORT
    {GUIIM_M_SESOTHO_LEAD_UPPER, &s_key_table_sesotho_upper},
    {GUIIM_M_SESOTHO_LOWER,      &s_key_table_sesotho_lower},
    {GUIIM_M_SESOTHO_UPPER,      &s_key_table_sesotho_upper},

    {GUIIM_M_TP_SESOTHO_LEAD_UPPER, &s_key_table_sesotho_upper},
    {GUIIM_M_TP_SESOTHO_LOWER,      &s_key_table_sesotho_lower},
    {GUIIM_M_TP_SESOTHO_UPPER,      &s_key_table_sesotho_upper},
#endif

// 斯洛伐克语
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_SLOVAK_LEAD_UPPER, &s_key_table_slovak_upper},
    {GUIIM_M_SLOVAK_LOWER,      &s_key_table_slovak_lower},
    {GUIIM_M_SLOVAK_UPPER,      &s_key_table_slovak_upper},

    {GUIIM_M_TP_SLOVAK_LEAD_UPPER, &s_key_table_slovak_upper},
    {GUIIM_M_TP_SLOVAK_LOWER,      &s_key_table_slovak_lower},
    {GUIIM_M_TP_SLOVAK_UPPER,      &s_key_table_slovak_upper},
#endif

// 斯洛文尼亚语
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_SLOVENIAN_LEAD_UPPER, &s_key_table_slovenian_upper},
    {GUIIM_M_SLOVENIAN_LOWER,      &s_key_table_slovenian_lower},
    {GUIIM_M_SLOVENIAN_UPPER,      &s_key_table_slovenian_upper},

    {GUIIM_M_TP_SLOVENIAN_LEAD_UPPER, &s_key_table_slovenian_upper},
    {GUIIM_M_TP_SLOVENIAN_LOWER,      &s_key_table_slovenian_lower},
    {GUIIM_M_TP_SLOVENIAN_UPPER,      &s_key_table_slovenian_upper},
#endif

// 西班牙语
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_SPANISH_LEAD_UPPER, &s_key_table_spanish_upper},
    {GUIIM_M_SPANISH_LOWER,      &s_key_table_spanish_lower},
    {GUIIM_M_SPANISH_UPPER,      &s_key_table_spanish_upper},

    {GUIIM_M_TP_SPANISH_LEAD_UPPER, &s_key_table_spanish_upper},
    {GUIIM_M_TP_SPANISH_LOWER,      &s_key_table_spanish_lower},
    {GUIIM_M_TP_SPANISH_UPPER,      &s_key_table_spanish_upper},
#endif

// 斯瓦西里语
#ifdef IM_SWAHILI_SUPPORT   //cstar, sogou等均使用abc数据表
    {GUIIM_M_SWAHILI_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_SWAHILI_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_SWAHILI_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_SWAHILI_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_SWAHILI_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_SWAHILI_UPPER,      &s_key_table_abc_upper},
#endif

// 瑞典语
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_SWEDISH_LEAD_UPPER, &s_key_table_swedish_upper},
    {GUIIM_M_SWEDISH_LOWER,      &s_key_table_swedish_lower},
    {GUIIM_M_SWEDISH_UPPER,      &s_key_table_swedish_upper},

    {GUIIM_M_TP_SWEDISH_LEAD_UPPER, &s_key_table_swedish_upper},
    {GUIIM_M_TP_SWEDISH_LOWER,      &s_key_table_swedish_lower},
    {GUIIM_M_TP_SWEDISH_UPPER,      &s_key_table_swedish_upper},
#endif

// 他加禄语
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_TAGALOG, &s_key_table_tagalog},

    {GUIIM_M_TP_TAGALOG, &s_key_table_tagalog},
#endif

// 泰米尔语
#ifdef IM_TAMIL_SUPPORT
    {GUIIM_M_TAMIL, &s_key_table_tamil},

    {GUIIM_M_TP_TAMIL, &s_key_table_tamil},
#endif

// 印度泰卢固语
#ifdef IM_TELUGU_SUPPORT
    {GUIIM_M_TELUGU, &s_key_table_telugu},

    {GUIIM_M_TP_TELUGU, &s_key_table_telugu},
#endif

// 泰语
#ifdef IM_THAI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
    {GUIIM_M_THAI, &s_key_table_thai},
    {GUIIM_M_TP_THAI, &s_key_table_thai},
#elif defined (MMIIM_SOGOU_MULTITAP)
    {GUIIM_M_THAI, &s_key_table_thai},
    {GUIIM_M_TP_THAI, &s_key_table_thai},
#else
    {GUIIM_M_THAI, &s_key_table_thai_normal},

    {GUIIM_M_TP_THAI, &s_key_table_thai_normal},
#endif    
#endif

// 土耳其语
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_TURKISH_LEAD_UPPER, &s_key_table_turkish_upper},
    {GUIIM_M_TURKISH_LOWER,      &s_key_table_turkish_lower},
    {GUIIM_M_TURKISH_UPPER,      &s_key_table_turkish_upper},

    {GUIIM_M_TP_TURKISH_LEAD_UPPER, &s_key_table_turkish_upper},
    {GUIIM_M_TP_TURKISH_LOWER,      &s_key_table_turkish_lower},
    {GUIIM_M_TP_TURKISH_UPPER,      &s_key_table_turkish_upper},
#endif

// 乌克兰语
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_UKRAINIAN_LEAD_UPPER, &s_key_table_ukrainian_upper},
    {GUIIM_M_UKRAINIAN_LOWER,      &s_key_table_ukrainian_lower},
    {GUIIM_M_UKRAINIAN_UPPER,      &s_key_table_ukrainian_upper},

    {GUIIM_M_TP_UKRAINIAN_LEAD_UPPER, &s_key_table_ukrainian_upper},
    {GUIIM_M_TP_UKRAINIAN_LOWER,      &s_key_table_ukrainian_lower},
    {GUIIM_M_TP_UKRAINIAN_UPPER,      &s_key_table_ukrainian_upper},
#endif

// 乌尔都语
#ifdef IM_URDU_SUPPORT
    {GUIIM_M_URDU, &s_key_table_urdu},

    {GUIIM_M_TP_URDU, &s_key_table_urdu},
#endif

// 越南语
#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_VIETNAMESE_LEAD_UPPER, &s_key_table_vietnamese_upper},
    {GUIIM_M_VIETNAMESE_LOWER,      &s_key_table_vietnamese_lower},
    {GUIIM_M_VIETNAMESE_UPPER,      &s_key_table_vietnamese_upper},

    {GUIIM_M_TP_VIETNAMESE_LEAD_UPPER, &s_key_table_vietnamese_upper},
    {GUIIM_M_TP_VIETNAMESE_LOWER,      &s_key_table_vietnamese_lower},
    {GUIIM_M_TP_VIETNAMESE_UPPER,      &s_key_table_vietnamese_upper},
#endif

// 科萨语（南非的一种方言）
#ifdef IM_XHOSA_SUPPORT
    {GUIIM_M_XHOSA_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_XHOSA_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_XHOSA_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_XHOSA_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_XHOSA_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_XHOSA_UPPER,      &s_key_table_abc_upper},
#endif

// 约鲁巴语
#ifdef IM_YORUBA_SUPPORT
    {GUIIM_M_YORUBA_LEAD_UPPER, &s_key_table_yoruba_upper},
    {GUIIM_M_YORUBA_LOWER,      &s_key_table_yoruba_lower},
    {GUIIM_M_YORUBA_UPPER,      &s_key_table_yoruba_upper},

    {GUIIM_M_TP_YORUBA_LEAD_UPPER, &s_key_table_yoruba_upper},
    {GUIIM_M_TP_YORUBA_LOWER,      &s_key_table_yoruba_lower},
    {GUIIM_M_TP_YORUBA_UPPER,      &s_key_table_yoruba_upper},
#endif

// 祖鲁语（非洲的语种）
#ifdef IM_ZULU_SUPPORT
    {GUIIM_M_ZULU_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_ZULU_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_ZULU_UPPER,      &s_key_table_abc_upper},

    {GUIIM_M_TP_ZULU_LEAD_UPPER, &s_key_table_abc_upper},
    {GUIIM_M_TP_ZULU_LOWER,      &s_key_table_abc_lower},
    {GUIIM_M_TP_ZULU_UPPER,      &s_key_table_abc_upper},
#endif

//阿姆哈拉语数据表还没有提供，暂时使用abc替代
#ifdef IM_AMHARIC_SUPPORT
    //{GUIIM_M_AMHARIC, &s_key_table_abc_amharic},
    //{GUIIM_M_TP_AMHARIC, &s_key_table_abc_amharic},
#endif

//柬埔寨智能输入暂不支持
#ifdef IM_KHMER_SUPPORT
    //{GUIIM_M_KHMER, &s_key_table_abc_khmer},
    //{GUIIM_M_TP_KHMER, &s_key_table_abc_khmer},
#endif

//老挝
#ifdef IM_LAO_SUPPORT
    {GUIIM_M_LAO, &s_key_table_abc_lao},
    {GUIIM_M_TP_LAO, &s_key_table_abc_lao},
#endif

#ifdef IM_UYGHUR_SUPPORT
    {GUIIM_M_UYGHUR, &s_key_table_abc_uyghur},
    {GUIIM_M_TP_UYGHUR, &s_key_table_abc_uyghur},
#endif

#ifdef IM_TIBETAN_SUPPORT    
//	{GUIIM_M_TIBETAN, &s_key_table_abc_tibetan},
//	{GUIIM_M_TP_TIBETAN, &s_key_table_abc_tibetan},
#endif

#ifdef IM_SINHALESE_SUPPORT
    {GUIIM_M_SINHALESE, &s_key_table_abc_sinhalese},
    {GUIIM_M_TP_SINHALESE, &s_key_table_abc_sinhalese},
#endif
};

//仅仅在虚拟键盘时，才需要绘制丝印
#if defined MMI_IM_PDA_SUPPORT
static MMIIM_SP_9KEY_TABLE_CFG_T s_spim_smart_lang_print_data[] =
{
    {GUIIM_M_TP_123, &s_key_table_123},		//保留一个，防止pclint

#if defined IM_URDU_SUPPORT
    {GUIIM_M_TP_URDU, &s_print_key_table_urdu},
#endif

#if defined IM_BENGALI_SUPPORT
    {GUIIM_M_TP_BENGALI, &s_print_key_table_bengali},
#endif

#if defined IM_TIBETAN_SUPPORT
    {GUIIM_M_TP_TIBETAN, &s_print_key_table_abc_tibetan},
#endif
};
#endif

//lint +e785

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*==============================================================================
Description: thai语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T * GetThaiKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: hindi语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetHindiKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: Telugu语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetTeluguKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: MARATHI语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetMarathiKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: Tamil语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetTamilKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: GUJARATI语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetGujaratiKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: KANNADA语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetKannadaKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: MALAYALAM语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetMalayalamKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: ORIYA语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetOriyaKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: PUNJABI语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetPunjabiKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: Assamese语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetAssameseKeyTable(
    wchar previous_char   // [in] prev char
);

/*==============================================================================
Description: ConvertKeyIdx
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 ConvertKeyIdx(GUIIM_CUSTOM_KEY_T custom_key);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*==============================================================================
Description: hindi语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetHindiKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_HINDI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
    return &s_key_table_hindi;
#elif defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_hindi;
#else
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    switch (previous_char)
    {
    case(DEVA_VS_AA):
    case(DEVA_VS_U):
    case(DEVA_VS_UU):
    case(DEVA_VSV_R):
    case(DEVA_L_A):
    case(DEVA_L_AA):
    case(DEVA_L_I):
    case(DEVA_L_U):
    case(DEVA_L_UU):
    case(DEVA_LV_R):
    case(DEVA_L_E):
        key_table = &s_key_table_hindimatra1;
        break;

    case(DEVA_VS_I):
    case(DEVA_VS_II):
    case(DEVA_VSC_E):
    case(DEVA_VS_E):
    case(DEVA_VS_AI):
    case(DEVA_VSC_O):
    case(DEVA_VS_O):
    case(DEVA_VS_AU):
    case(DEVA_L_II):
    case(DEVA_LC_E):
    case(DEVA_L_AI):
    case(DEVA_LC_O):
    case(DEVA_L_O):
    case(DEVA_L_AU):
        key_table = &s_key_table_hindimatra2;
        break;

    default:

        if ((previous_char >= DEVA_L_KA && previous_char <= DEVA_L_HA) ||
                (previous_char >= DEVA_L_QA && previous_char <= DEVA_L_YYA))
        {
            key_table = &s_key_table_hindimatra;
        }
        else
        {
            key_table = &s_key_table_hindi;
        }

        break;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: thai语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T * GetThaiKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_THAI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
    return &s_key_table_thai;
#elif defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_thai;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (THAI_C_SARA_AA == previous_char)
    {
        /* THAI_C_SARA_A + THAI_C_SARA_A is a special case for two following vowel combination. */
        key_table = &s_key_table_thai_sara_aa;
    }
    else if (IsThaiConsonant(previous_char))
    {
        key_table = &s_key_table_thai_con;
    }
    else  if (IsThaiToneMark(previous_char))
    {
        key_table = &s_key_table_thai_tone;
    }
    else if (IsThaiUpDownVowel(previous_char))
    {
        key_table = &s_key_table_thai_vowel;
    }
    else
    {
        key_table = &s_key_table_thai_normal;
    }

    return key_table;
#endif    
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: Telugu语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetTeluguKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_TELUGU_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP) || defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_telugu;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if (previous_char >= TELUGU_C_KA && previous_char <= TELUGU_C_HA)
        {
            key_table = &s_key_table_telugumatra;
        }
        else
        {
            switch (previous_char)
            {
            case(TELUGU_IV_A):
            case(TELUGU_IV_AA):
            case(TELUGU_IV_I):
            case(TELUGU_IV_II):
            case(TELUGU_IV_U):
            case(TELUGU_IV_UU):
            case(TELUGU_IV_VR):
            case(TELUGU_IV_VL):
            case(TELUGU_IV_E):
            case(TELUGU_IV_EE):
            case(TELUGU_IV_AI):
            case(TELUGU_IV_O):
            case(TELUGU_IV_OO):
            case(TELUGU_IV_AU):
            case(TELUGU_DV_AA):
            case(TELUGU_DV_I):
            case(TELUGU_DV_II):
            case(TELUGU_DV_U):
            case(TELUGU_DV_UU):
            case(TELUGU_DV_VR):
            case(TELUGU_DV_VRR):
            case(TELUGU_DV_E):
            case(TELUGU_DV_EE):
            case(TELUGU_DV_AI):
            case(TELUGU_DV_O):
            case(TELUGU_DV_OO):
            case(TELUGU_DV_AU):
                {
                    key_table = &s_key_table_telugumatra1;
                    break;
                }
            default:
                {
                    key_table = &s_key_table_telugu;
                    break;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_telugu;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: MARATHI语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetMarathiKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_MARATHI_SUPPORT

#if (defined(MMIIM_CSTAR_MULTITAP) || defined(MMIIM_SOGOU_MULTITAP))
    return &s_key_table_marathi;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if ((previous_char >= MARATHI_C_KA && previous_char <= MARATHI_C_HA) ||
                (previous_char >= MARATHI_AC_QA && previous_char <= MARATHI_AC_YYA))
        {
            key_table = &s_key_table_marathimatra;
        }
        else
        {
            switch (previous_char)
            {
            case(MARATHI_DV_AA):
            case(MARATHI_DV_U):
            case(MARATHI_DV_UU):
            case(MARATHI_DV_R):
            case(MARATHI_IV_A):
            case(MARATHI_IV_AA):
            case(MARATHI_IV_I):
            case(MARATHI_IV_U):
            case(MARATHI_IV_UU):
            case(MARATHI_IV_R):
            case(MARATHI_IV_E):
                {
                    key_table = &s_key_table_marathimatra1;
                    break;
                }

            case(MARATHI_DV_I):
            case(MARATHI_DV_II):
            case(MARATHI_DV_CE):
            case(MARATHI_DV_E):
            case(MARATHI_DV_AI):
            case(MARATHI_NDV_CO):
            case(MARATHI_DV_O):
            case(MARATHI_DV_AU):
            case(MARATHI_IV_II):
            case(MARATHI_IV_AI):
            case(MARATHI_NIV_CO):
            case(MARATHI_IV_O):
            case(MARATHI_IV_AU):
                {
                    key_table = &s_key_table_marathimatra2;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_marathi;
                    break;
                }
            }
        }

    }
    else
    {
        key_table = &s_key_table_marathi;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: Tamil语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetTamilKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_TAMIL_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP) || defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_tamil;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if (previous_char >= TAMIL_C_KA && previous_char <= TAMIL_C_HA)
        {
            key_table = &s_key_table_tamilmatra;
        }
        else
        {
            switch (previous_char)
            {
            case(TAMIL_DV_AA):
            case(TAMIL_DV_I):
            case(TAMIL_DV_II):
            case(TAMIL_DV_U):
            case(TAMIL_DV_UU):
            case(TAMIL_DV_E):
            case(TAMIL_DV_EE):
            case(TAMIL_DV_AI):
            case(TAMIL_DV_O):
            case(TAMIL_DV_OO):
            case(TAMIL_DV_AU):
            case(TAMIL_IV_A):
            case(TAMIL_IV_AA):
            case(TAMIL_IV_I):
            case(TAMIL_IV_II):
            case(TAMIL_IV_U):
            case(TAMIL_IV_UU):
            case(TAMIL_IV_E):
            case(TAMIL_IV_EE):
            case(TAMIL_IV_O):
            case(TAMIL_IV_OO):
            case(TAMIL_IV_AU):
                {
                    key_table = &s_key_table_tamilmatra1;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_tamil;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_tamil;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: GUJARATI语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetGujaratiKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_GUJARATI_SUPPORT

#if defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_gujarati;
#else
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if ((previous_char >= GUJARATI_C_KA && previous_char <= GUJARATI_C_HA))
        {
            switch (previous_char)
            {
            case(GUJARATI_C_KA):
            case(GUJARATI_C_KHA):
            case(GUJARATI_C_GA):
            case(GUJARATI_C_JA):
            case(GUJARATI_C_DDA):
            case(GUJARATI_C_DDHA):
            case(GUJARATI_C_NA):
            case(GUJARATI_C_PHA):
            case(GUJARATI_C_YA):
            case(GUJARATI_C_RA):
            case(GUJARATI_C_LLA):
                {
                    key_table = &s_key_table_gujaratimatra3;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_gujaratimatra;
                }
            }
        }
        else
        {
            switch (previous_char)
            {
            case(GUJARATI_IV_A):
            case(GUJARATI_IV_AA):
            case(GUJARATI_IV_VR):
            case(GUJARATI_IV_VL):
            case(GUJARATI_DV_AA):
            case(GUJARATI_DV_U):
            case(GUJARATI_DV_UU):
            case(GUJARATI_DV_VR):
            case(GUJARATI_DV_VRR):
                {
                    key_table = &s_key_table_gujaratimatra1;
                    break;
                }

            case(GUJARATI_DV_I):
            case(GUJARATI_DV_II):
            case(GUJARATI_DV_CE):
            case(GUJARATI_DV_E):
            case(GUJARATI_DV_AI):
            case(GUJARATI_DV_CO):
            case(GUJARATI_DV_O):
            case(GUJARATI_DV_AU):
            case(GUJARATI_IV_I):
            case(GUJARATI_IV_II):
            case(GUJARATI_IV_U):
            case(GUJARATI_IV_UU):
            case(GUJARATI_IV_CE):
            case(GUJARATI_IV_E):
            case(GUJARATI_IV_AI):
            case(GUJARATI_IV_CO):
            case(GUJARATI_IV_O):
            case(GUJARATI_IV_AU):
                {
                    key_table = &s_key_table_gujaratimatra2;
                    break;
                }

            case(GUJARATI_S_NUKTA):
                {
                    key_table = &s_key_table_gujaratimatra;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_gujarati;
                    break;
                }
            }
        }

    }
    else
    {
        key_table = &s_key_table_gujarati;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: KANNADA语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetKannadaKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_KANNADA_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP) || defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_kannada;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if (previous_char >= KANNADA_C_KA && previous_char <= KANNADA_C_HA)
        {
            key_table = &s_key_table_kannadamatra;
        }
        else
        {
            switch (previous_char)
            {
            case(KANNADA_IV_A):
            case(KANNADA_IV_AA):
            case(KANNADA_IV_I):
            case(KANNADA_IV_II):
            case(KANNADA_IV_U):
            case(KANNADA_IV_UU):
            case(KANNADA_IV_VR):
            case(KANNADA_IV_VL):
            case(KANNADA_IV_E):
            case(KANNADA_IV_EE):
            case(KANNADA_IV_AI):
            case(KANNADA_IV_O):
            case(KANNADA_IV_OO):
            case(KANNADA_IV_AU):
            case(KANNADA_DV_AA):
            case(KANNADA_DV_I):
            case(KANNADA_DV_II):
            case(KANNADA_DV_U):
            case(KANNADA_DV_UU):
            case(KANNADA_DV_VR):
            case(KANNADA_DV_VRR):
            case(KANNADA_DV_E):
            case(KANNADA_DV_EE):
            case(KANNADA_DV_AI):
            case(KANNADA_DV_O):
            case(KANNADA_DV_OO):
            case(KANNADA_DV_AU):
                {
                    key_table = &s_key_table_kannadamatra1;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_kannada;
                    break;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_kannada;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: MALAYALAM语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetMalayalamKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_MALAYALAM_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)  || defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_malayalam;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if ((previous_char >= MALAYALAM_C_KA) && (previous_char <= MALAYALAM_C_HA))
        {
            key_table = &s_key_table_malayalammatra;
        }
        else
        {
            switch (previous_char)
            {
            case(MALAYALAM_IV_A):
            case(MALAYALAM_IV_AA):
            case(MALAYALAM_IV_I):
            case(MALAYALAM_IV_II):
            case(MALAYALAM_IV_U):
            case(MALAYALAM_IV_UU):
            case(MALAYALAM_IV_VR):
            case(MALAYALAM_IV_VL):
            case(MALAYALAM_IV_E):
            case(MALAYALAM_IV_EE):
            case(MALAYALAM_IV_AI):
            case(MALAYALAM_IV_O):
            case(MALAYALAM_IV_OO):
            case(MALAYALAM_IV_AU):
            case(MALAYALAM_DV_AA):
            case(MALAYALAM_DV_I):
            case(MALAYALAM_DV_II):
            case(MALAYALAM_DV_U):
            case(MALAYALAM_DV_UU):
            case(MALAYALAM_DV_VR):
            case(MALAYALAM_DV_VRR):
            case(MALAYALAM_DV_E):
            case(MALAYALAM_DV_EE):
            case(MALAYALAM_DV_AI):
            case(MALAYALAM_DV_O):
            case(MALAYALAM_DV_OO):
            case(MALAYALAM_DV_AU):
            case(MALAYALAM_IV_VRR):
            case(MALAYALAM_IV_VLL):
            case(MALAYALAM_DV_VL):
            case(MALAYALAM_DV_VLL):
                {
                    key_table = &s_key_table_malayalammatra1;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_malayalam;
                    break;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_malayalam;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: ORIYA语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetOriyaKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_ORIYA_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)  || defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_oriya;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if ((previous_char >= ORIYA_C_KA && previous_char <= ORIYA_C_HA)
                || (previous_char >= ORIYA_AC_RRA && previous_char <= ORIYA_AC_YYA)
                || (previous_char >= ORIYA_S_ISSHAR && previous_char <= ORIYA_S_WA))
        {
            key_table = &s_key_table_oriyamatra;
        }
        else
        {
            switch (previous_char)
            {
            case(ORIYA_IV_A):
            case(ORIYA_IV_AA):
            case(ORIYA_IV_I):
            case(ORIYA_IV_II):
            case(ORIYA_IV_U):
            case(ORIYA_IV_UU):
            case(ORIYA_IV_R):
            case(ORIYA_IV_L):
            case(ORIYA_IV_E):
            case(ORIYA_IV_O):
            case(ORIYA_DV_AA):
            case(ORIYA_DV_U):
            case(ORIYA_DV_UU):
            case(ORIYA_DV_R):
            case(ORIYA_DV_RR):
            case(ORIYA_DV_E):
            case(ORIYA_DV_O):
            case(ORIYA_NG_L):
            case(ORIYA_NG_VLL):
                {
                    key_table = &s_key_table_oriyamatra1;
                    break;
                }

            case(ORIYA_IV_AI):
            case(ORIYA_IV_AU):
            case(ORIYA_DV_I):
            case(ORIYA_DV_II):
            case(ORIYA_DV_AI):
            case(ORIYA_DV_AU):
                {
                    key_table = &s_key_table_oriyamatra2;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_oriya;
                    break;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_oriya;
    }

    return key_table;
#endif
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: PUNJABI语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetPunjabiKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_PUNJABI_SUPPORT

#if defined(MMIIM_CSTAR_MULTITAP)  || defined (MMIIM_SOGOU_MULTITAP)
    return &s_key_table_punjabi;
#else

    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if ((previous_char >= PUNJABI_C_KA && previous_char <= PUNJABI_C_HA)
                || (previous_char >= PUNJABI_AC_KHHA && previous_char <= PUNJABI_AC_FA))
        {
            key_table = &s_key_table_punjabimatra;
        }
        else
        {
            switch (previous_char)
            {
            case(PUNJABI_DV_AA):
            case(PUNJABI_DV_II):
            case(PUNJABI_DV_EE):
            case(PUNJABI_DV_AI):
            case(PUNJABI_DV_OO):
            case(PUNJABI_DV_AU):
            case(PUNJABI_IV_AA):
            case(PUNJABI_IV_EE):
            case(PUNJABI_IV_II):
            case(PUNJABI_IV_AI):
            case(PUNJABI_IV_AU):
            case(PUNJABI_IV_U):
            case(PUNJABI_IV_UU):
            case(PUNJABI_IV_OO):
                {
                    key_table = &s_key_table_punjabimatra1;
                    break;
                }

            case(PUNJABI_DV_I):
            case(PUNJABI_DV_U):
            case(PUNJABI_DV_UU):
            case(PUNJABI_IV_A):
            case(PUNJABI_IV_I):
                {
                    key_table = &s_key_table_punjabimatra2;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_punjabi;
                    break;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_punjabi;
    }

    return key_table;		//函数没有返回值，实际却用到了返回值.fix it. 
#endif	
#else
    return PNULL;
#endif
}

/*==============================================================================
Description: Assamese语的输入规则，根据前一个字符切换输入表
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_9KEY_TABLE_DEF_T* GetAssameseKeyTable(
    wchar previous_char   // [in] prev char
)
{
#ifdef IM_ASSAMESE_SUPPORT
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table = PNULL;

    if (previous_char)
    {
        if ((previous_char >= ASSAMESE_C_KA && previous_char <= ASSAMESE_C_HA) ||
                (previous_char >= ASSAMESE_C_RRA && previous_char <= ASSAMESE_C_YYA) ||
                (previous_char >= ASSAMESE_C_MRA && previous_char <= ASSAMESE_C_LRA))
        {
            key_table = &s_key_table_assamesematra;
        }
        else
        {
            switch (previous_char)
            {
            case(ASSAMESE_IV_A):
            case(ASSAMESE_IV_AA):
            case(ASSAMESE_IV_VR):
            case(ASSAMESE_IV_VL):
            case(ASSAMESE_IV_E):
            case(ASSAMESE_IV_O):
            case(ASSAMESE_DV_AA):
            case(ASSAMESE_DV_I):
            case(ASSAMESE_DV_II):
            case(ASSAMESE_DV_U):
            case(ASSAMESE_DV_UU):
            case(ASSAMESE_DV_VR):
            case(ASSAMESE_DV_VRR):
            case(ASSAMESE_DV_E):
            case(ASSAMESE_DV_AI):
            case(ASSAMESE_DV_O):
            case(ASSAMESE_DV_AU):
                {
                    key_table = &s_key_table_assamesematra1;
                    break;
                }

            case(ASSAMESE_IV_I):
            case(ASSAMESE_IV_II):
            case(ASSAMESE_IV_U):
            case(ASSAMESE_IV_UU):
            case(ASSAMESE_IV_AI):
            case(ASSAMESE_IV_AU):
                {
                    key_table = &s_key_table_assamesematra2;
                    break;
                }

            default:
                {
                    key_table = &s_key_table_assamese;
		    break;
                }
            }
        }
    }
    else
    {
        key_table = &s_key_table_assamese;
    }

    return key_table;

#else
    return PNULL;
#endif
}


/*==============================================================================
Description: ConvertKeyIdx
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 ConvertKeyIdx(GUIIM_CUSTOM_KEY_T custom_key)
{
    int32 key_idx = -1;

    switch(custom_key)
    {
    case GUIIM_CUSTOM_KEY_0:
        key_idx = MMIIM_SP_9KEY_INDEX_0;
        break;

    case GUIIM_CUSTOM_KEY_1:
        key_idx = MMIIM_SP_9KEY_INDEX_1;
        break;

    case GUIIM_CUSTOM_KEY_2:
        key_idx = MMIIM_SP_9KEY_INDEX_2;
        break;

    case GUIIM_CUSTOM_KEY_3:
        key_idx = MMIIM_SP_9KEY_INDEX_3;
        break;

    case GUIIM_CUSTOM_KEY_4:
        key_idx = MMIIM_SP_9KEY_INDEX_4;
        break;

    case GUIIM_CUSTOM_KEY_5:
        key_idx = MMIIM_SP_9KEY_INDEX_5;
        break;

    case GUIIM_CUSTOM_KEY_6:
        key_idx = MMIIM_SP_9KEY_INDEX_6;
        break;

    case GUIIM_CUSTOM_KEY_7:
        key_idx = MMIIM_SP_9KEY_INDEX_7;
        break;

    case GUIIM_CUSTOM_KEY_8:
        key_idx = MMIIM_SP_9KEY_INDEX_8;
        break;

    case GUIIM_CUSTOM_KEY_9:
        key_idx = MMIIM_SP_9KEY_INDEX_9;
        break;

    case GUIIM_CUSTOM_KEY_STAR:
        key_idx = MMIIM_SP_9KEY_INDEX_STAR;
        break;

    case GUIIM_CUSTOM_KEY_HASH:
        key_idx = MMIIM_SP_9KEY_INDEX_HASH;
        break;
        
    default:
        break;
    }

    return key_idx;
}



/*==============================================================================
Description: get key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void const * MMIIM_SP_ML9KEY_GetKeyTable(
    GUIIM_METHOD_T method      // [in] method type
)
{
    uint16 i = 0;
    MMIIM_SP_9KEY_TABLE_DEF_T *table_cfg_ptr = PNULL;

    for (i = 0; i < ARR_SIZE(s_spim_mt_lang_data); i++)
    {
        if (s_spim_mt_lang_data[i].method == method)
        {
            table_cfg_ptr = s_spim_mt_lang_data[i].key_tab_ptr;
            break;
        }
    }

    return table_cfg_ptr;
}



//仅仅在虚拟键盘时，才需要绘制丝印
#if defined MMI_IM_PDA_SUPPORT
/*==============================================================================
Description: get key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void const * MMIIM_SP_ML9KEY_GetPrintKeyTable(
    GUIIM_METHOD_T method      // [in] method type
)
{
    uint16 i = 0;
    MMIIM_SP_9KEY_TABLE_DEF_T *table_cfg_ptr = PNULL;

    for (i = 0; i < ARR_SIZE(s_spim_mt_lang_print_data); i++)
    {
        if (s_spim_mt_lang_print_data[i].method == method)
        {
            table_cfg_ptr = s_spim_mt_lang_print_data[i].key_tab_ptr;
            break;
        }
    }

	if (table_cfg_ptr == PNULL)
	{
		table_cfg_ptr = MMIIM_SP_ML9KEY_GetKeyTable (method);
	}

    return table_cfg_ptr;
}
#endif

//使用字符串的方式显示虚拟键盘字符
/*==============================================================================
Description: get key table by method index
Author: yaoguang.chen
Global resource dependence: NONE
Note:   通过输入法索引获取当前的数据区，应用于智能输入，不跟multitap
    使用同一张表，是为了后期程序维护的灵活性
==============================================================================*/
PUBLIC void const * MMIIM_SP_Smart9Key_GetKeyTable(
    GUIIM_METHOD_T method      // [in] method type
)
{
    uint16 i = 0;
    MMIIM_SP_9KEY_TABLE_DEF_T *table_cfg_ptr = PNULL;

    for (i = 0; i < ARR_SIZE(s_spim_smart_lang_data); i++)
    {
        if (s_spim_smart_lang_data[i].method == method)
        {
            table_cfg_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T *)s_spim_smart_lang_data[i].key_tab_ptr;
            break;
        }
    }

    //如果没有键盘表，则用默认键盘表
    if(table_cfg_ptr == NULL)
    {
    	SCI_TRACE_LOW ("[MMIIM] MMIIM_SP_Smart9Key_GetKeyTable None table err");
        table_cfg_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T *)&s_key_table_abc_upper;
    }

    return table_cfg_ptr;
}


//仅仅在虚拟键盘时，才需要绘制丝印
#if defined MMI_IM_PDA_SUPPORT
/*==============================================================================
Description: get print key table by method index
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void const * MMIIM_SP_Smart9KEY_GetPrintKeyTable(
    GUIIM_METHOD_T method      // [in] method type
)
{
    uint16 i = 0;
    MMIIM_SP_9KEY_TABLE_DEF_T *table_cfg_ptr = PNULL;

    for (i = 0; i < ARR_SIZE(s_spim_smart_lang_print_data); i++)
    {
        if (s_spim_smart_lang_print_data[i].method == method)
        {
            table_cfg_ptr = s_spim_smart_lang_print_data[i].key_tab_ptr;
            break;
        }
    }

	if (table_cfg_ptr == PNULL)
	{
		table_cfg_ptr = MMIIM_SP_Smart9Key_GetKeyTable (method);
	}

    return table_cfg_ptr;
}
#endif


/*==============================================================================
Description: get function pointer which checks special language letter.
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC CHECK_TABLE_FUNC_T MMIIM_SP_ML9KEY_GetCheckingFunc(
    GUIIM_METHOD_T method // [in] method type
)
{
    CHECK_TABLE_FUNC_T checking_func_ptr = PNULL;

#ifdef IM_HINDI_SUPPORT
    if (GUIIM_M_HINDI_ABC == method)
    {
        checking_func_ptr = GetHindiKeyTable;
    }
#endif

#ifdef IM_THAI_SUPPORT
    if (GUIIM_M_THAI_ABC == method)
    {
        checking_func_ptr = GetThaiKeyTable;
    }
#endif

#ifdef IM_TELUGU_SUPPORT
    if (GUIIM_M_TELUGU_ABC == method)
    {
        checking_func_ptr = GetTeluguKeyTable;
    }
#endif

#ifdef IM_MARATHI_SUPPORT
    if (GUIIM_M_MARATHI_ABC == method)
    {
        checking_func_ptr = GetMarathiKeyTable;
    }
#endif

#ifdef IM_TAMIL_SUPPORT
    if (GUIIM_M_TAMIL_ABC == method)
    {
        checking_func_ptr = GetTamilKeyTable;
    }
#endif

#ifdef IM_GUJARATI_SUPPORT
    if (GUIIM_M_GUJARATI_ABC == method)
    {
        checking_func_ptr = GetGujaratiKeyTable;
    }
#endif

#ifdef IM_KANNADA_SUPPORT
    if (GUIIM_M_KANNADA_ABC == method)
    {
        checking_func_ptr = GetKannadaKeyTable;
    }
#endif

#ifdef IM_MALAYALAM_SUPPORT
    if (GUIIM_M_MALAYALAM_ABC == method)
    {
        checking_func_ptr = GetMalayalamKeyTable;
    }
#endif

#ifdef IM_ORIYA_SUPPORT
    if (GUIIM_M_ORIYA_ABC == method)
    {
        checking_func_ptr = GetOriyaKeyTable;
    }
#endif

#ifdef IM_PUNJABI_SUPPORT
    if (GUIIM_M_PUNJABI_ABC == method)
    {
        checking_func_ptr = GetPunjabiKeyTable;
    }
#endif

#ifdef IM_ASSAMESE_SUPPORT
    if (GUIIM_M_ASSAMESE_ABC == method)
    {
        checking_func_ptr = GetAssameseKeyTable;
    }
#endif  //else cstar
    return checking_func_ptr;
}

/*==============================================================================
Function Name: MMIIM_SP_Check9KeyTable
Description: 校验键值表的正确性
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_Check9KeyTable(
    void *table_ptr   // [in] key table
)
{
    int32 key_index = 0;
    int32 char_index = 0;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T*)table_ptr;

    if (PNULL == key_table_ptr)
    {
        return TRUE;
    }

    for (key_index = 0; key_index < MMIIM_SP_9KEY_INDEX_MAX; key_index++)
    {
        if ((*key_table_ptr)[key_index][0] > GUIIM_CUSTOM_CHAR_LIMIT)
        {
            return FALSE;
        }

        for (char_index = 1; char_index <= (*key_table_ptr)[key_index][0]; char_index++)
        {
            if (0 == (*key_table_ptr)[key_index][char_index])
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}


/*==============================================================================
Description: 获取指定索引下的配置键值个数
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
uint16 MMIIM_SP_ML9KEY_GetChrCnt(void const * table_ptr, uint16 key_idx)
{
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T *)table_ptr;

    if (PNULL == key_table_ptr)
    {
        return 0;
    }

    if (key_idx >= MMIIM_SP_9KEY_INDEX_MAX)
    {
        return 0;
    }

    return ((*key_table_ptr)[key_idx][0]);
}


/*==============================================================================
Description: 获取指定索引下的指定键值
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: char_index从1开始计数
==============================================================================*/
uint16 MMIIM_SP_ML9KEY_GetChr(void const * table_ptr, uint16 key_idx, uint16 chr_idx)
{
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T *)table_ptr;

    if (PNULL == key_table_ptr)
    {
        return 0;
    }

    if (key_idx >= MMIIM_SP_9KEY_INDEX_MAX)
    {
        return 0;
    }

    return ((*key_table_ptr)[key_idx][chr_idx]);
}


/*==============================================================================
Description: 根据字符得到对应的键值
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML9KEY_GetKeyIndexByChar(
                                                 void const *table_ptr,     // [in]
                                                 wchar  key_char,       // [in]
                                                 uint16 *key_index_ptr  // [out]
                                                 )
{
    uint16 i = 0;
    uint16 j = 0;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T *)table_ptr;

    if (PNULL == key_index_ptr)
    {
        return FALSE;
    }

#ifdef IM_ENGINE_SOGOU
    for (i = 0; i < GUIIM_CUSTOM_KEY_MAX; i++)
#else
    for (i = 0; i < MMIIM_SP_9KEY_INDEX_MAX; i++)
#endif    
    {
        for (j = 0; j < (*key_table_ptr)[i][0]; j++)
        {
            if (key_char == (*key_table_ptr)[i][j + 1])
            {
                *key_index_ptr = i;
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*==============================================================================
Description: alloc memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML9KEY_AllocCustomKb(
                                             void const *table_ptr,
                                             void  **custom_pptr
                                             )
{
    uint32 key_table_size = sizeof(MMIIM_SP_9KEY_TABLE_T);
    MMIIM_SP_9KEY_TABLE_T *custom_ptr = PNULL;

    custom_ptr = (MMIIM_SP_9KEY_TABLE_T*)SCI_ALLOC_APP(key_table_size);

    SCI_MEMCPY(custom_ptr, table_ptr, key_table_size);

    if (0 == (*custom_ptr)[MMIIM_SP_9KEY_INDEX_STAR][0])
    {
        (*custom_ptr)[MMIIM_SP_9KEY_INDEX_STAR][0] = 1;
        (*custom_ptr)[MMIIM_SP_9KEY_INDEX_STAR][1] = MMIIM_SP_KEY_OP_SYMBOL_PANEL;
    }

    if (0 == (*custom_ptr)[MMIIM_SP_9KEY_INDEX_HASH][0])
    {
        (*custom_ptr)[MMIIM_SP_9KEY_INDEX_HASH][0] = 1;
        (*custom_ptr)[MMIIM_SP_9KEY_INDEX_HASH][1] = MMIIM_SP_KEY_OP_SWITCH_MENU;
    }

    *custom_pptr = custom_ptr;

    return TRUE;
}

/*==============================================================================
Description: free memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML9KEY_FreeCustomKb(
                                            void *custom_ptr
                                            )
{
    if (PNULL != custom_ptr)
    {
        SCI_FREE(custom_ptr);
    }

    return TRUE;
}

/*==============================================================================
Description: MMIIM_SP_ML9KEY_CustomKb
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ML9KEY_CustomKb(
    GUIIM_TYPE_T cur_im_type,
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr,
    void *custom_kb_ptr
)
{
    uint32 i = 0;
    int32 key_idx = 0;
    MMIIM_SP_9KEY_TABLE_T *custom_ptr = (MMIIM_SP_9KEY_TABLE_T*)custom_kb_ptr;
    
    if (PNULL == custom_ptr || PNULL == key_info_ptr)
    {
        return FALSE;
    }

    if (cur_im_type != key_info_ptr->im_type)
    {
        return FALSE;
    }

    if (key_info_ptr->key_count > MMIIM_SP_9KEY_CHAR_MAX)
    {
        key_info_ptr->key_count = MMIIM_SP_9KEY_CHAR_MAX;
    }

    key_idx = ConvertKeyIdx(key_info_ptr->key);

    if (key_idx < 0)
    {
        return FALSE;
    }

    (*custom_ptr)[key_idx][0] = key_info_ptr->key_count;

    for (i = 0; i < key_info_ptr->key_count; i++)
    {
        (*custom_ptr)[key_idx][i+1] = key_info_ptr->key_values[i];
    }


    return TRUE;
}


