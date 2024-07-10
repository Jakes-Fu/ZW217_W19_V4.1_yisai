
/*****************************************************************************
** File Name:      mmiim_sp_ml26key_data.c                                   *
** Author:         hua.fang                                                  *
** Date:           07/22/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2010        hua.fang          Create                                   *
** 10/2010        hua.fang          Modify table struct
******************************************************************************/
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmiim_sp_ml26key_data.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/
//lint -e785

// 26键多国语言数据

// 数据的组织形式:
// 各国数据分为各自的语言数据，及公用的数据
// 公用数据指的是，如果该键位没有定位在语言数据表中，那它会再到公用数据区去找

// 数据的描述:
// MMIIM_SP_26KEY_KEYCHAR_T为描述键位键值的对应关系，即key_index对应的是键位，
// key_chars对应的是键值，key_chars的第一个值为键值的个数
// 每个语言都会有一个MMIIM_SP_26KEY_KEYCHAR_T的数组，以MMIIM_SP_26KEY_INDEX_MAX
// 结尾

// 公用按键对应的字母
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_common[MMIIM_SP_26KEY_COMMON_CNT] =
{
    { MMIIM_SP_26KEY_INDEX_DEL              , {1, '\b'} },
    { MMIIM_SP_26KEY_INDEX_COMMA            , {1, ','}  },
    { MMIIM_SP_26KEY_INDEX_PERIOD           , {1, '.'}  },
    { MMIIM_SP_26KEY_INDEX_ENTER            , {1, '\n'} },
    { MMIIM_SP_26KEY_INDEX_FN               , {0}       },
    { MMIIM_SP_26KEY_INDEX_SHIFT            , {0}       },
    { MMIIM_SP_26KEY_INDEX_AT_QWERTY        , {1, '@'}  },
    { MMIIM_SP_26KEY_INDEX_SPACE            , {1, ' '}  },
    { MMIIM_SP_26KEY_INDEX_AND              , {1, '&'}  },
    { MMIIM_SP_26KEY_INDEX_QUESTION         , {1, '?'}  },
    { MMIIM_SP_26KEY_INDEX_CTRL             , {0}       },

    { MMIIM_SP_26KEY_INDEX_PLUS             , {1, '+'}  },
    { MMIIM_SP_26KEY_INDEX_LEFT_PARENTHESIS , {1, '('}  },
    { MMIIM_SP_26KEY_INDEX_RIGHT_PARENTHESIS, {1, ')'}  },
    { MMIIM_SP_26KEY_INDEX_MINUS            , {1, '-'}  },
    { MMIIM_SP_26KEY_INDEX_DOUBLE_QUOTES    , {1, '\"'} },
    { MMIIM_SP_26KEY_INDEX_SEMICOLON        , {1, ';'}  },
    { MMIIM_SP_26KEY_INDEX_COLON            , {1, ':'}  },
    { MMIIM_SP_26KEY_INDEX_SLASH            , {1, '/'}  },
    { MMIIM_SP_26KEY_INDEX_SHIFT_AND        , {1, '\''} },
    { MMIIM_SP_26KEY_INDEX_EXCLAMATION      , {1, '!'}  },
    { MMIIM_SP_26KEY_INDEX_QUOTES, {1, '\''}  },
    { MMIIM_SP_26KEY_INDEX_EQUAL, {1, '='} },
    { MMIIM_SP_26KEY_INDEX_RIGHT_SLASH, {1, '\\'}  },
        

    { MMIIM_SP_26KEY_INDEX_0                , {1, '0'}  },           
    { MMIIM_SP_26KEY_INDEX_1                , {1, '1'}  },           
    { MMIIM_SP_26KEY_INDEX_2                , {1, '2'}  },           
    { MMIIM_SP_26KEY_INDEX_3                , {1, '3'}  },           
    { MMIIM_SP_26KEY_INDEX_4                , {1, '4'}  },           
    { MMIIM_SP_26KEY_INDEX_5                , {1, '5'}  },           
    { MMIIM_SP_26KEY_INDEX_6                , {1, '6'}  },           
    { MMIIM_SP_26KEY_INDEX_7                , {1, '7'}  },           
    { MMIIM_SP_26KEY_INDEX_8                , {1, '8'}  },           
    { MMIIM_SP_26KEY_INDEX_9                , {1, '9'}  },           
    { MMIIM_SP_26KEY_INDEX_STAR             , {1, '*'}  },           
    { MMIIM_SP_26KEY_INDEX_HASH             , {1, '#'}  },           
};


// 数字
#ifdef QWERTY_KEYPAD_BLACKBERRY
static MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_123[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, '*'} },
    { MMIIM_SP_26KEY_INDEX_B, {0} },
    { MMIIM_SP_26KEY_INDEX_C, {1, '9'} },
    { MMIIM_SP_26KEY_INDEX_D, {1, '5'} },
    { MMIIM_SP_26KEY_INDEX_E, {1, '2'} },
    { MMIIM_SP_26KEY_INDEX_F, {1, '6'} },
    { MMIIM_SP_26KEY_INDEX_G, {0} },
    { MMIIM_SP_26KEY_INDEX_H, {0} },
    { MMIIM_SP_26KEY_INDEX_I, {0} },
    { MMIIM_SP_26KEY_INDEX_J, {0} },
    { MMIIM_SP_26KEY_INDEX_K, {0} },
    { MMIIM_SP_26KEY_INDEX_L, {0} },
    { MMIIM_SP_26KEY_INDEX_M, {0} },
    { MMIIM_SP_26KEY_INDEX_N, {0} },
    { MMIIM_SP_26KEY_INDEX_O, {0} },
    { MMIIM_SP_26KEY_INDEX_P, {0} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, '#'} },
    { MMIIM_SP_26KEY_INDEX_R, {1, '3'} },
    { MMIIM_SP_26KEY_INDEX_S, {1, '4'} },
    { MMIIM_SP_26KEY_INDEX_T, {0} },
    { MMIIM_SP_26KEY_INDEX_U, {0} },
    { MMIIM_SP_26KEY_INDEX_V, {0} },
    { MMIIM_SP_26KEY_INDEX_W, {1, '1'} },
    { MMIIM_SP_26KEY_INDEX_X, {1, '8'} },
    { MMIIM_SP_26KEY_INDEX_Y, {0} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, '7'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
//#elif defined QWERTY_KEYPAD_NOKIA
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_123[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {0} },
    { MMIIM_SP_26KEY_INDEX_B, {1, '8'} },
    { MMIIM_SP_26KEY_INDEX_C, {0} },
    { MMIIM_SP_26KEY_INDEX_D, {0} },
    { MMIIM_SP_26KEY_INDEX_E, {0} },
    { MMIIM_SP_26KEY_INDEX_F, {1, '4'} },
    { MMIIM_SP_26KEY_INDEX_G, {1, '5'} },
    { MMIIM_SP_26KEY_INDEX_H, {1, '6'} },
    { MMIIM_SP_26KEY_INDEX_I, {0} },
    { MMIIM_SP_26KEY_INDEX_J, {1, '#'} },
    { MMIIM_SP_26KEY_INDEX_K, {0} },
    { MMIIM_SP_26KEY_INDEX_L, {0} },
    { MMIIM_SP_26KEY_INDEX_M, {1, '0'} },
    { MMIIM_SP_26KEY_INDEX_N, {1, '9'} },
    { MMIIM_SP_26KEY_INDEX_O, {0} },
    { MMIIM_SP_26KEY_INDEX_P, {0} },
    { MMIIM_SP_26KEY_INDEX_Q, {0} },
    { MMIIM_SP_26KEY_INDEX_R, {1, '1'} },
    { MMIIM_SP_26KEY_INDEX_S, {0} },
    { MMIIM_SP_26KEY_INDEX_T, {1, '2'} },
    { MMIIM_SP_26KEY_INDEX_U, {1, '*'} },
    { MMIIM_SP_26KEY_INDEX_V, {1, '7'} },
    { MMIIM_SP_26KEY_INDEX_W, {0} },
    { MMIIM_SP_26KEY_INDEX_X, {0} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, '3'} },
    { MMIIM_SP_26KEY_INDEX_Z, {0} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif

//小写字母
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_abc_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

//大写字母
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_abc_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x0041} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};


//以下多国语言

//繁体中文
#ifdef IM_TRAD_CHINESE_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_trad_chinese_zhuyin[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x3107} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x3119} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x3112} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x310E} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x310D} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x3111} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x3115} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x3118} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x311B, 0x311E} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x3128} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x311C} },
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x3120, 0x3124} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x311D} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x3129} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x311F, 0x3122} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x3123, 0x3126} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x3105, 0x3106} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x3110} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x310B} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x3113, 0x3114} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x3127, 0x311A} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x3116} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x3109, 0x310A} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x310F} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x3117} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x3108, 0x310C} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#ifdef IM_TRAD_CHINESE_QWERT_40KEY
static const MMIIM_SP_26KEY_KEYCHAR_T s_40key_table_trad_chinese_zhuyin[] =		//some customer like this layout
{
	//first line
    { MMIIM_SP_KEY_IRREGULAR_1, {1, 0x3105} },
    { MMIIM_SP_KEY_IRREGULAR_2, {1, 0x3109} },
    { MMIIM_SP_KEY_IRREGULAR_3, {1, 0x02c7} },
    { MMIIM_SP_KEY_IRREGULAR_4, {1, 0x02cb} },
    { MMIIM_SP_KEY_IRREGULAR_5, {1, 0x3113} },
    { MMIIM_SP_KEY_IRREGULAR_6, {1, 0x02ca} },
    { MMIIM_SP_KEY_IRREGULAR_7, {1, 0x02d9} },
    { MMIIM_SP_KEY_IRREGULAR_8, {1, 0x311a} },
    { MMIIM_SP_KEY_IRREGULAR_9, {1, 0x311e} },
    { MMIIM_SP_KEY_IRREGULAR_10, {1, 0x3122} },
    
    { MMIIM_SP_KEY_IRREGULAR_11, {1, 0x3106} },
    { MMIIM_SP_KEY_IRREGULAR_12, {1, 0x310a} },
    { MMIIM_SP_KEY_IRREGULAR_13, {1, 0x310d} },
    { MMIIM_SP_KEY_IRREGULAR_14, {1, 0x3110} },
    { MMIIM_SP_KEY_IRREGULAR_15, {1, 0x3114} },
    { MMIIM_SP_KEY_IRREGULAR_16, {1, 0x3117} },
    { MMIIM_SP_KEY_IRREGULAR_17, {1, 0x3127} },
    { MMIIM_SP_KEY_IRREGULAR_18, {1, 0x311c} },
    { MMIIM_SP_KEY_IRREGULAR_19, {1, 0x311f} },
    { MMIIM_SP_KEY_IRREGULAR_20, {1, 0x3123} },
    
    { MMIIM_SP_KEY_IRREGULAR_21, {1, 0x3107} },
    { MMIIM_SP_KEY_IRREGULAR_22, {1, 0x310b} },
    { MMIIM_SP_KEY_IRREGULAR_23, {1, 0x310e} },
    { MMIIM_SP_KEY_IRREGULAR_24, {1, 0x3111} },
    { MMIIM_SP_KEY_IRREGULAR_25, {1, 0x3115} },
    { MMIIM_SP_KEY_IRREGULAR_26, {1, 0x3118} },
    { MMIIM_SP_KEY_IRREGULAR_27, {1, 0x3128} },
    { MMIIM_SP_KEY_IRREGULAR_28, {1, 0x311c} },
    { MMIIM_SP_KEY_IRREGULAR_29, {1, 0x3120} },
    { MMIIM_SP_KEY_IRREGULAR_30, {1, 0x3124} },
    
    { MMIIM_SP_KEY_IRREGULAR_31, {1, 0x3108} },
    { MMIIM_SP_KEY_IRREGULAR_32, {1, 0x310c} },
    { MMIIM_SP_KEY_IRREGULAR_33, {1, 0x310f} },
    { MMIIM_SP_KEY_IRREGULAR_34, {1, 0x3112} },
    { MMIIM_SP_KEY_IRREGULAR_35, {1, 0x3116} },
    { MMIIM_SP_KEY_IRREGULAR_36, {1, 0x3119} },
    { MMIIM_SP_KEY_IRREGULAR_37, {1, 0x3129} },
    { MMIIM_SP_KEY_IRREGULAR_38, {1, 0x311d} },
    { MMIIM_SP_KEY_IRREGULAR_39, {1, 0x3121} },
    { MMIIM_SP_KEY_IRREGULAR_40, {1, 0x3125} },
        
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_trad_chinese_zhuyin[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x3106} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x3116} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x310D} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x310C} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x310B} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x310F} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x3112} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x3118, 0x3114} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x311E, 0x311F} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x311B, 0x3128} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x3120, 0x3121} },
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x3124, 0x3025} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x311C, 0x3129} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x3119, 0x3115} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x3122, 0x3123} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x311D, 0x3126} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x3105} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x310E} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x3109} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x3111} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x311A, 0x3127} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x3110} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x3108} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x310A} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x3117, 0x3113} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x3107} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_trad_chinese_zhuyin[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x3107} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x3119} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x3112} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x310E} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x310D} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x3111} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x3115} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x3118} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x311B, 0x311E} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x3128} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x311C} },
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x3120, 0x3124} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x311D} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x3129} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x311F, 0x3122} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x3123, 0x3126} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x3105, 0x3106} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x3110} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x310B} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x3113, 0x3114} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x3127, 0x311A} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x3116} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x3109, 0x310A} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x310F} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x3117} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x3108, 0x310C} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 阿拉伯语
#ifdef IM_ARABIC_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_arabic[] =
{
    {MMIIM_SP_26KEY_INDEX_A,	{1, 0x0634}},
    {MMIIM_SP_26KEY_INDEX_B,	{3, 0xFEFB, 0xFEF5, 0x0643}},
    {MMIIM_SP_26KEY_INDEX_C,	{1, 0x0624}},
    {MMIIM_SP_26KEY_INDEX_D,	{1, 0x064A}},
    {MMIIM_SP_26KEY_INDEX_E,	{1, 0x062B}},
    {MMIIM_SP_26KEY_INDEX_F,	{1, 0x0628}},
    {MMIIM_SP_26KEY_INDEX_G,	{2, 0x0644, 0xFEF7}},
    {MMIIM_SP_26KEY_INDEX_H,	{2, 0x0627, 0x0623}},
    {MMIIM_SP_26KEY_INDEX_I,	{1, 0x0647}},
    {MMIIM_SP_26KEY_INDEX_J,	{2, 0x062A, 0x0640}},
    {MMIIM_SP_26KEY_INDEX_K,	{1, 0x0646}},
    {MMIIM_SP_26KEY_INDEX_L,	{2, 0x0645, 0x0637}},
    {MMIIM_SP_26KEY_INDEX_M,	{2, 0x0629, 0x0648}},
    {MMIIM_SP_26KEY_INDEX_N,	{3, 0x0649, 0x0622, 0x061F}},
    {MMIIM_SP_26KEY_INDEX_O,	{2, 0x062E, 0x062C}},
    {MMIIM_SP_26KEY_INDEX_P,	{3, 0x062D, 0x062F, 0x061B}},
    {MMIIM_SP_26KEY_INDEX_Q,	{2, 0x0636, 0x0630}},
    {MMIIM_SP_26KEY_INDEX_R,	{1, 0x0642}},
    {MMIIM_SP_26KEY_INDEX_S,	{1, 0x0633}},
    {MMIIM_SP_26KEY_INDEX_T,	{2, 0x0641, 0xFEF9}},
    {MMIIM_SP_26KEY_INDEX_U,	{1, 0x0639}},
    {MMIIM_SP_26KEY_INDEX_V,	{1, 0x0631}},
    {MMIIM_SP_26KEY_INDEX_W,	{1, 0x0635}},
    {MMIIM_SP_26KEY_INDEX_X,	{1, 0x0621}},
    {MMIIM_SP_26KEY_INDEX_Y,	{2, 0x063A, 0x0625}},
    {MMIIM_SP_26KEY_INDEX_Z,	{1, 0x0626}},   
    {MMIIM_SP_26KEY_INDEX_PERIOD, {3, 0x0632, 0x0638, 0x060C} },    
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},        
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_arabic[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0633,  0x0634} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0621} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0647} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x064A} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x062B} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0628, 0x062A} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0644} },
    {MMIIM_SP_26KEY_INDEX_H, {4, 0x0627, 0x0622, 0x0625, 0x0623} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x062E} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0629} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0646} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0645} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0643} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0649} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x062D} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x062C} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0635, 0x0636} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0648, 0x0624} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0641} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0639} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0631, 0x0632} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0637, 0x0638} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x062f, 0x0630} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x063A} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x0626, 0x0640} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_arabic[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x633, 0x634} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x621} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x647} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64a} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x62b} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x628} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x644} },
    { MMIIM_SP_26KEY_INDEX_H, {4, 0x627, 0x622, 0x625, 0x623} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x62e} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x629, 0x62a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x646} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x645} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x643} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x649} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x62d} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x62c} },
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x636, 0x637, 0x638} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x642} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x648, 0x624} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x641} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x639} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x631, 0x632} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x635} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x630} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x63a} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x626} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 法语
#ifdef IM_FRENCH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_french_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {4, 0x0041, 0x00C0, 0x00C2, 0x00C6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {5, 0x0045, 0x00C9, 0x00C8, 0x00CA, 0x00CB} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0049, 0x00CE, 0x00CF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x00D1} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D4, 0x0152} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00D9, 0x00DB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x0178} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_french_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {4, 0x0061, 0x00E0, 0x00E2, 0x00E6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {5, 0x0065, 0x00E9, 0x00E8, 0x00EA, 0x00EB} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0069, 0x00EE, 0x00EF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x00F1} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F4, 0x0153} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00F9, 0x00FB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FF} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_french_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C0, 0x00C2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {5, 0x0045, 0x00C9, 0x00C8, 0x00CA, 0x00CB} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0049, 0x00CE, 0x00CF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00D9, 0x00DB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_french_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E0, 0x00E2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {5, 0x0065, 0x00E9, 0x00E8, 0x00EA, 0x00EB} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0069, 0x00EE, 0x00EF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00F9, 0x00FB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0077, 0x0027} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0078, 0x002D} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x002E} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x002F} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_french_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {4, 0x41, 0xc0, 0xc2, 0xc6} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {3, 0x43, 0xc7, 0x152} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {5, 0x45, 0xc9, 0xc8, 0xca, 0xcb} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x49, 0xce, 0xcf} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x4e, 0xd1} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x4f, 0xd4} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x55, 0xd9, 0xdb} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x59, 0x178} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_french_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {4, 0x61, 0xe0, 0xe2, 0xe6} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {3, 0x63, 0xe7, 0x153} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {5, 0x65, 0xe9, 0xe8, 0xea, 0xeb} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x69, 0xee, 0xef} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x6e, 0xf1} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x6f, 0xf4} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x75, 0xf9, 0xfb} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x79, 0xff} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 德语
#ifdef IM_GERMAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_german_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C4, 0x00C0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D6, 0x00DF} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_german_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E4, 0x00E0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F6, 0x00DF} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_german_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_german_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x00DF} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_german_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x41, 0xc0, 0xc4} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x42, 0x1e9e} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x43} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x45, 0xc9} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x49} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x4f, 0xd6} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x55, 0xdc} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_german_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x61, 0xe0, 0xe4} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x62, 0xdf} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x63} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x65, 0xe9} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x69} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x6f, 0xf6} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x75, 0xfc} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#endif
#endif

// 希腊语
#ifdef IM_GREEK_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_greek_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0391, 0x0386} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0392} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x03A8} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0394} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0395, 0x0388} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x03A6} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0393} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0397, 0x0389} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x0399, 0x038A, 0x03AA, 0x0390} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x039E} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x039A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x039B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x039C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x039D} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x039F, 0x038C} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x03A0} },
	{MMIIM_SP_26KEY_INDEX_Q, {0} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x03A1} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x03A3} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x03A4} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0398} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x03A9, 0x038F} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x03A3} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x03A7} },
    {MMIIM_SP_26KEY_INDEX_Y, {4, 0x03A5, 0x038E, 0x03AB, 0x03B0} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0396} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_greek_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x03B1, 0x03AC} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x03B2} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x03C8} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x03B4} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x03B5, 0x03AD} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x03C6} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x03B3} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x03B7, 0x03AE} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x03B9, 0x03AF,  0x03CA, 0x0390} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x03BE} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x03BA} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x03BB} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x03BC} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x03BD} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x03BF, 0x03CC} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x03C0} },
	{MMIIM_SP_26KEY_INDEX_Q, {0} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x03C1} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x03C3} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x03C4} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x03B8} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x03C9, 0x03CE} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x03C2} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x03C7} },
    {MMIIM_SP_26KEY_INDEX_Y, {4, 0x03C5, 0x03CD, 0x03CB, 0x03B0} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x03B6} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};


#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_greek_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0391, 0x0386} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0392} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x03A8} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0394} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0395, 0x0388} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x03A6} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0393} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0397, 0x0389} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0399, 0x038A, 0x03AA} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x039E} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x039A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x039B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x039C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x039D} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x039F, 0x038C} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0398} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x03A0} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x03A1} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x03A3} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x03A4} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x03AB} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x03A9, 0x038F} },
    {MMIIM_SP_26KEY_INDEX_W, {0} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x03A7} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x03A5, 0x038E} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0396} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_greek_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x03B1, 0x03AC} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x03B2} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x03C8} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x03B4} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x03B5, 0x03AD} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x03C6} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x03B3} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x03B7, 0x03AE} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x03B9, 0x03AF, 0x03CA, 0x0390} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x03BE} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x03BA} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x03BB} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x03BC} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x03BD} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x03BF, 0x03CC} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x03B8} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x03C0} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x03C1} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x03C3} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x03C4} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x03CB, 0x03B0} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x03C9, 0x03CE} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x03C2} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x03C7} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x03C5, 0x03CD} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x03B6} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_greek_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x391, 0x386} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x392} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x3a8} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x394} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x395, 0x388} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x3a6} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x393} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x397, 0x389} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x399, 0x38a, 0x3aa, 0x0390} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x39e} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x39a} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x39b} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x39c} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x39d} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x39f, 0x38c} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x398} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x3a0} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x3a1} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x3a3} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x3a4} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x3ab, 0x03b0} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x3a9, 0x38f} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x3c2} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x3a7} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x3a5, 0x38e} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x396} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_greek_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x3b1, 0x3ac} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x3b2} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x3c8} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x3b4} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x3b5, 0x3ad} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x3c6} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x3b3} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x3b7, 0x3ae} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x3b9, 0x3af, 0x3ca, 0x390} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x3be} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x3ba} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x3bb} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x3bc} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x3bd} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x3bf, 0x3cc} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x3b8} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x3c0} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x3c1} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x3c3} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x3c4} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x3cb, 0x3b0} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x3c9, 0x3ce} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x3c2} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x3c7} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x3c5, 0x3cd} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x3b6} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 印地语
#ifdef IM_HINDI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hindi[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {4, 0x094B, 0x0949, 0x0913, 0x0911}},
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0935, 0x0934}},
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x092E, 0x0923}},
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x094D, 0x0905}},
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x093E, 0x0906}},
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x093F, 0x0907}},
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0941, 0x0909}},
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x092A, 0x092B}},
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x0917, 0x091C, 0x0918, 0x091D}},
    {MMIIM_SP_26KEY_INDEX_J, {4, 0x092F, 0x0930, 0x095F, 0x0931}},
    {MMIIM_SP_26KEY_INDEX_K, {4, 0x0915, 0x091A, 0x0916, 0x091B}},
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0924, 0x091F, 0x0925, 0x0920}},
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x0938, 0x0936, 0x0937}},
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0932, 0x0933}},
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x0926, 0x0921, 0x0927, 0x0922}},
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x093C, 0x091E}},
    {MMIIM_SP_26KEY_INDEX_Q, {4, 0x094C, 0x094A, 0x0914, 0x0912}},
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0940, 0x0908}},
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0947, 0x090F}},
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0942, 0x090A}},
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0939, 0x0919}},
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0928, 0x0929}},
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0948, 0x0910}},
    {MMIIM_SP_26KEY_INDEX_X, {4, 0x0902, 0x0901, 0x0945, 0x0903}},
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x092C, 0x092D}},
    {MMIIM_SP_26KEY_INDEX_Z, {4, 0x0946, 0x090E,    0x0943, 0x0960}},	//因当前的虚拟键盘没有考虑0键，所以先将0键的数据放到z键上
    {MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x0964}}, 
    //{MMIIM_SP_26KEY_INDEX_0, {2, 0x0943, 0x0960}},	//因当前的虚拟键盘没有考虑0键，所以先将0键的数据放到z键上
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hindi[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {4, 0x094B, 0x0949, 0x0913, 0x0911} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0935} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x092E, 0x0923} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x094D, 0x0905} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x093E, 0x0906} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x093F, 0x0907} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0941, 0x0909} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x092A, 0x092B} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x0917, 0x091C, 0x0918, 0x091D} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x092F, 0x0930} },
    {MMIIM_SP_26KEY_INDEX_K, {4, 0x0915, 0x091A, 0x0916, 0x091B} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0924, 0x091F, 0x0925, 0x0920} },
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x0938, 0x0936, 0x0937} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0932} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x0926, 0x0921, 0x0927, 0x0922} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x093C, 0x091E} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x094C, 0x0914} },
    {MMIIM_SP_26KEY_INDEX_R, {3, 0x0940, 0x0908, 0x090B} },
    {MMIIM_SP_26KEY_INDEX_S, {4, 0x0947, 0x093D, 0x090D, 0x090F} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0942, 0x090A} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0939, 0x0919} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0928} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0948, 0x0910} },
    {MMIIM_SP_26KEY_INDEX_X, {4, 0x0902, 0x0901, 0x0945, 0x0903} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x092C, 0x092D} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0943} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hindi[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {4, 0x94b, 0x949, 0x913, 0x911} },
    { MMIIM_SP_26KEY_INDEX_B, {3, 0x935, 0x934, 0x964} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x92e, 0x923} },
    { MMIIM_SP_26KEY_INDEX_D, {3, 0x94d, 0x943, 0x905} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x93e, 0x906} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x93f, 0x907} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x941, 0x909} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x92a, 0x92b} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x917, 0x91c, 0x918, 0x91d} },
    { MMIIM_SP_26KEY_INDEX_J, {4, 0x92f, 0x930, 0x95f, 0x931} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x915, 0x91a, 0x916, 0x91b} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x924, 0x91f, 0x925, 0x920} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x938, 0x936, 0x937} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x932, 0x933} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x926, 0x921, 0x927, 0x922} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x93c, 0x91e} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x94c, 0x94a, 0x914, 0x912} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x940, 0x908} },
    { MMIIM_SP_26KEY_INDEX_S, {4, 0x947, 0x93d, 0x90d, 0x90f} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x942, 0x90a} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x939, 0x919} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x928, 0x929} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x948, 0x910} },
    { MMIIM_SP_26KEY_INDEX_X, {4, 0x902, 0x901, 0x945, 0x903} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x92c, 0x92d} },
    { MMIIM_SP_26KEY_INDEX_Z, {3, 0x946, 0x90e, 0x960} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

//add. MARATHI data. 此表与s_26key_table_hindi数据相同，但不写在一起是防止可能某一天修改数据不同
#if(defined IM_MARATHI_SUPPORT)	
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_marathi[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {4, 0x094B, 0x0949, 0x0913, 0x0911}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0935}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x092E, 0x0923}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x094D, 0x0905}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x093E, 0x0906}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x093F, 0x0907}},
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0941, 0x0909}},
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x092A, 0x092B}},
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x0917, 0x091C}},
    { MMIIM_SP_26KEY_INDEX_J, {4, 0x092F, 0x0930, 0x095F, 0x0931}},
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0915, 0x091A, 0x0916, 0x091B}},
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0924, 0x091F, 0x0925, 0x0920}},
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0938, 0x0936, 0x0937}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0932, 0x0933}},
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0926, 0x0921, 0x0927, 0x0922}},
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x093C, 0x091E}},
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x094C, 0x0914}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0940, 0x0908}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0947, 0x090F}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0942, 0x090A}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0939, 0x0919}},
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0928, 0x0929}},
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0948, 0x0910}},
    { MMIIM_SP_26KEY_INDEX_X, {4, 0x0902, 0x0901, 0x0945, 0x0903}},
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x092C, 0x092D}},
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0918, 0x091D}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x0964}},
    { MMIIM_SP_26KEY_INDEX_0, {1, 0x0943}},

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_marathi[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x094B, 0x0913} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0935} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x092E, 0x0923} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x094D, 0x0905} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x093E, 0x0906} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x093F, 0x0907} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0941, 0x0909} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x092A, 0x092B} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x0917, 0x091C, 0x0918, 0x091D} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x092F, 0x0930} },
    {MMIIM_SP_26KEY_INDEX_K, {4, 0x0915, 0x091A, 0x0916, 0x091B} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0924, 0x091F, 0x0925, 0x0920} },
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x0938, 0x0936, 0x0937} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0932, 0x0933} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x0926, 0x0921, 0x0927, 0x0922} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x093C, 0x091E} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x094C, 0x0914} },
    {MMIIM_SP_26KEY_INDEX_R, {3, 0x0940, 0x0908, 0x090B} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0947, 0x090F} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0942, 0x090A} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0939, 0x0919} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0928} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0948, 0x0910} },
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x0902, 0x0901, 0x0903} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x092C, 0x092D} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0943} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_marathi[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {4, 0x94b, 0x949, 0x913, 0x911} },
    { MMIIM_SP_26KEY_INDEX_B, {3, 0x935, 0x934, 0x964} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x92e, 0x923} },
    { MMIIM_SP_26KEY_INDEX_D, {3, 0x94d, 0x943, 0x905} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x93e, 0x906} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x93f, 0x907} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x941, 0x909} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x92a, 0x92b} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x917, 0x91c, 0x918, 0x91d} },
    { MMIIM_SP_26KEY_INDEX_J, {4, 0x92f, 0x930, 0x95f, 0x931} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x915, 0x91a, 0x916, 0x91b} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x924, 0x91f, 0x925, 0x920} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x938, 0x936, 0x937} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x932, 0x933} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x926, 0x921, 0x927, 0x922} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x93c, 0x91e} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x94c, 0x94a, 0x914, 0x912} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x940, 0x908} },
    { MMIIM_SP_26KEY_INDEX_S, {4, 0x947, 0x93d, 0x90d, 0x90f} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x942, 0x90a} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x939, 0x919} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x928, 0x929} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x948, 0x910} },
    { MMIIM_SP_26KEY_INDEX_X, {4, 0x902, 0x901, 0x945, 0x903} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x92c, 0x92d} },
    { MMIIM_SP_26KEY_INDEX_Z, {3, 0x946, 0x90e, 0x960} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif



// 匈牙利语
#ifdef IM_HUNGARIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hungarian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C1, 0x00C0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x004F, 0x00D6, 0x00D3, 0x0150} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0055, 0x00DA, 0x0170, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hungarian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E1, 0x00E0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x006F, 0x00F6, 0x00F3, 0x0151} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0075, 0x00FA, 0x0171, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hungarian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x41, 0xc1} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x43} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x45, 0xc9} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x49, 0xcd} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x4f, 0xd3, 0xd6, 0x150} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {4, 0x55, 0xda, 0xdc, 0x170} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hungarian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x61, 0xe1} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x63} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x65, 0xe9} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x69, 0xed} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x6f, 0xf3, 0xf6, 0x151} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {4, 0x75, 0xfa, 0xfc, 0x171} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 意大利语
#ifdef IM_ITALIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_italian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x00C8, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CC} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D2} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00D9} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_italian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x00E8, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00EC} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F2} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00F9} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_italian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x00C8, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CC} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D2} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00D9} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_italian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E0}},
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062}},
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063}},
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064}},
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x00E8, 0x00E9}},
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066}},
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067}},
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068}},
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00EC}},
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A}},
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B}},
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C}},
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D}},
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E}},
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F2}},
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070}},
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071}},
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072}},
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073}},
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074}},
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00F9}},
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076}},
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077}},
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078}},
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079}},
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A}},
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_italian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x41, 0xc0} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x43} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x45, 0xc9, 0xc8} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x49, 0xcc} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x4f, 0xd2} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x55, 0xd9} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_italian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x61, 0xe0} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x63} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x65, 0xe9, 0xe8} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x69, 0xec} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x6f, 0xf2} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x75, 0xf9} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 波斯语
#ifdef IM_PERSIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_persian[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0634}},
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0630, 0x0625}},
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0632, 0x0698}},
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x06CC}},
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x062B}},
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0628}},
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0644, 0x06C0}},
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0627, 0x0622}},
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0647}},
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x062A, 0x06A9, 0x0640}},
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0646, 0x06AF}},
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0645, 0x067E}},
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x0626, 0x0648, 0x0621}},
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x062F, 0x0623}},
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x062E, 0x062C}},
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x062D, 0x0686}},
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0636}},
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642}},
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0633}},
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0641, 0x060C}},
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0639}},
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0631, 0x0624}},
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0635}},
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0637, 0x064A}},
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x063A, 0x061B}},
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x0638, 0x0629}},
    {MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x061F}},
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_persian[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0633,  0x0634} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0698} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0647} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x067E} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x062B} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0628, 0x062A} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0644} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0627, 0x0622} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x062E} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0686} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0646} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0645} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x06A9} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x06AF} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x062D} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x062C} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0635, 0x0636} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0648} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0641} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0639} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0631, 0x0632} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0637, 0x0638} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x062f, 0x0630} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x063A} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x06CC} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_persian[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x634, 0x633} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x621} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x647} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64a} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x62b} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x628, 0x67e} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x644} },
    { MMIIM_SP_26KEY_INDEX_H, {4, 0x627, 0x622, 0x625, 0x623} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x62e} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x629, 0x62a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x646} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x645} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x643} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x649, 0x6cc} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x62d} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x62c} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x636, 0x637, 0x638, 0x6a9} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x642} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x624, 0x648} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x641} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x639} },
    { MMIIM_SP_26KEY_INDEX_V, {3, 0x632, 0x631, 0x698} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x635, 0x6af} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x630, 0x62f} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x63a} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x626} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 葡萄牙语
#ifdef IM_PORTUGUESE_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_portuguese_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0041, 0x00C1, 0x00C0, 0x00C2, 0x00C3} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x00C9, 0x00CA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x004F, 0x00D3, 0x00D4, 0x00D5} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00DA, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_portuguese_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0061, 0x00E1, 0x00E0, 0x00E2, 0x00E3} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x00E9, 0x00EA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x006F, 0x00F3, 0x00F4, 0x00F5} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00FA, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_portuguese_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0041, 0x00C0, 0x00C1, 0x00C2, 0x00C3} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x00C9, 0x00CA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x004F, 0x00D3, 0x00D4, 0x00D5} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00DA, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_portuguese_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0061, 0x00E0, 0x00E1, 0x00E2, 0x00E3} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x00E9, 0x00EA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x006F, 0x00F3, 0x00F4, 0x00F5} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00FA, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_portuguese_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {5, 0x41, 0xc1, 0xc0, 0xc2, 0xc3} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x43, 0xc7} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x45, 0xc9, 0xca} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x49, 0xcd} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x4f} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x55, 0xda, 0xdc} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_portuguese_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {5, 0x61, 0xe1, 0xe0, 0xe2, 0xe3} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x63, 0xe7} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x65, 0xe9, 0xea} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x69, 0xed} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x6f} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x75, 0xfa, 0xfc} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 俄语
#ifdef IM_RUSSIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_russian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x0424}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0418}},
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0421}},
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0412}},
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0423}},
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0410}},
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x041F}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0420}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0428}},
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x041E}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x041B, 0x0416}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0414, 0x042D}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x042C, 0x042E}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0422, 0x0411}},
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x0429, 0x0425}},
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0417, 0x042A}},
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0419, 0x0401}},
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x041A}},
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x042B}},
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0415}},
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x0413}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x041C}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0426}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0427}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x041D}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x042F}},

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_russian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x0444}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0438}},
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0441}},
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0432}},
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0443}},
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0430}},
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x043F}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0440}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0448}},
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x043E}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x043B, 0x0436}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0434, 0x044D}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x044C, 0x044E}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0442, 0x0431}},
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x0449, 0x0445}},
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0437, 0x044A}},
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0439, 0x0451}},
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x043A}},
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x044B}},
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0435}},
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x0433}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x043C}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0446}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0447}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x043D}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x044F}},

    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};

#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_russian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0410} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0411} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0426} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0414} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0415, 0x0401} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0424} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0413} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0427} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0418, 0x0419} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x042C, 0x042A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x041A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x041B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x041C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x041D} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x041E} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x041F} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x042F, 0x042D} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0420} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0421} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0422} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0423, 0x042E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0412} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0428, 0x0429} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0425, 0x0416} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x042B} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0417} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_russian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0430} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0431} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0446} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0434} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0435, 0x0451} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0444} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0433} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0447} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0438, 0x0439} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x044C, 0x044A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x043A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x043B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x043C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x043D} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x043E} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x043F} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x044F, 0x044D} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0440} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0441} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0442} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0443, 0x044E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0432} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0448, 0x0449} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0445, 0x0436} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x044B} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0437} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_russian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x410, 0x41} },
    { MMIIM_SP_26KEY_INDEX_B, {3, 0x412, 0x42c, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x421, 0x43} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x42d, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x415, 0x401, 0x45} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0x411, 0x42a, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x424, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x41d, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x414, 0x49} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x41b, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x41a, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x428, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x41c, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x42e, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x41e, 0x4f} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x420, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x427, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {3, 0x42f, 0x416, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x417, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {3, 0x422, 0x413, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x41f, 0x426, 0x55} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x42b, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {3, 0x418, 0x419, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x425, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x423, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x429, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_russian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x430, 0x61} },
    { MMIIM_SP_26KEY_INDEX_B, {3, 0x432, 0x44c, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x441, 0x63} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x44d, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x435, 0x451, 0x65} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0x431, 0x44a, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x444, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x43d, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x434, 0x69} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x43b, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x43a, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x448, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x43c, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x44e, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x43e, 0x6f} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x440, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x447, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {3, 0x44f, 0x436, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x437, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {3, 0x442, 0x433, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x43f, 0x446, 0x75} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x44b, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {3, 0x438, 0x439, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x445, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x443, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x449, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 西班牙语
#ifdef IM_SPANISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_spanish_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C1, 0x00C0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x00C9, 0x00C8} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0049, 0x00CD, 0x00CF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x00D1} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D3, 0x00D2} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00DA, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_spanish_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E1, 0x00E0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x00E9, 0x00E8} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0069, 0x00ED, 0x00EF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x00F1} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F3, 0x00F2} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00FA, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_spanish_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x00D1} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00DA, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_spanish_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x00F1} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00FA, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_spanish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x41, 0xc1, 0xc0} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x43, 0xc7} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x45, 0xc9, 0xc8} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x49, 0xcd, 0xcf} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x4e, 0xd1} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x4f, 0xd3, 0xd2} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x55, 0xda, 0xdc} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_spanish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x61, 0xe1, 0xe0} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x63, 0xe7} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x65, 0xe9, 0xe8} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x69, 0xed, 0xef} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x6e, 0xf1} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x6f, 0xf3, 0xf2} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x75, 0xfa, 0xfc} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 他加禄语     // TODO: CHECK ME
#ifdef IM_TAGALOG_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tagalog[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
//0X1700~0X171F
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tagalog[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 泰语
#ifdef IM_THAI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_thai[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0E1F, 0x0E24}},
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0E34, 0x0E3A}},
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0E41, 0x0E09}},
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0E01, 0x0E0F}},
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0E33, 0x0E0E, 0x0E52}},
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0E14, 0x0E42}},
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0E40, 0x0E0C}},
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0E49, 0x0E47}},
    {MMIIM_SP_26KEY_INDEX_I, {5, 0x0E23, 0x0E13, 0x0E04, 0x0E1A, 0x0E55}},
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x0E48, 0x0E4B}},
    {MMIIM_SP_26KEY_INDEX_K, {3, 0x0E32, 0x0E29, 0x0E27}},
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0E2A, 0x0E28, 0x0E07, 0x0E0B}},
    {MMIIM_SP_26KEY_INDEX_M, {6, 0x0E17, 0x0E21, 0x0E1D, 0x0E12, 0x0E2C, 0x0E26}},
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0E37, 0x0E4C}},
    {MMIIM_SP_26KEY_INDEX_O, {7, 0x0E19, 0x0E2F, 0x0E15, 0x0E02, 0x0E25, 0x0E56, 0x0E58}},
    {MMIIM_SP_26KEY_INDEX_P, {9, 0x0E22, 0x0E0D, 0x0E08, 0x0E0A, 0x0E03, 0x0E10, 0x0E57, 0x0E59, 0x0E05}},
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x0E46, 0x0E50, 0x0E45}},
    {MMIIM_SP_26KEY_INDEX_R, {4, 0x0E1E, 0x0E11, 0x0E20, 0x0E53}},
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0E2B, 0x0E06}},
    {MMIIM_SP_26KEY_INDEX_T, {4, 0x0E30, 0x0E18, 0x0E16, 0x0E54}},
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0E35, 0x0E36, 0x0E4A, 0x0E3F}},
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0E2D, 0x0E2E}},
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0E44, 0x0E51}},
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0E1B}},
    {MMIIM_SP_26KEY_INDEX_Y, {4, 0x0E31, 0x0E38, 0x0E4D, 0x0E39}},
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x0E1C, 0x0E24}},
    {MMIIM_SP_26KEY_INDEX_PERIOD, {2, 0x0E43, 0x0E24}},
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_thai[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {4, 0x0E1F, 0x0E2B, 0x0E24, 0x0E26} },
    {MMIIM_SP_26KEY_INDEX_B, {3, 0x0E37, 0x0E3A, 0x0E4D} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0E2D, 0x0E09} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0E40, 0x0E42} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0E1E, 0x0E16, 0x0E0E} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0E49, 0x0E47} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x0E48, 0x0E4B, 0x0E4E} },
    {MMIIM_SP_26KEY_INDEX_H, {3, 0x0E32, 0x0E4A, 0x0E45} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0E19, 0x0E08, 0x0E2F} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x0E2A, 0x0E29, 0x0E25} },
    {MMIIM_SP_26KEY_INDEX_K, {3, 0x0E27, 0x0E28, 0x0E05} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0E07, 0x0E0A} },
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x0E43, 0x0E2C, 0x0E12} },
    {MMIIM_SP_26KEY_INDEX_N, {3, 0x0E17, 0x0E21, 0x0E4C} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0E22, 0x0E02, 0x0E0D} },
    {MMIIM_SP_26KEY_INDEX_P, {5, 0x0E1A, 0x0E0B, 0x0E10, 0x0E1B, 0x0E03} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0E44, 0x0E46} },
    {MMIIM_SP_26KEY_INDEX_R, {3, 0x0E30, 0x0E38, 0x0E18} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x0E01, 0x0E14, 0x0E0F} },
    {MMIIM_SP_26KEY_INDEX_T, {3, 0x0E31, 0x0E36, 0x0E39} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0E23, 0x0E15, 0x0E13} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0E34, 0x0E2E} },
    {MMIIM_SP_26KEY_INDEX_W, {3, 0x0E33, 0x0E11, 0x0E20} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0E41, 0x0E0C} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0E04, 0x0E35} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x0E1C, 0x0E06, 0x0E1D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_thai[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0xe1c, 0xe1d, 0xe1e} },
    { MMIIM_SP_26KEY_INDEX_B, {4, 0xe46, 0xe47, 0xe48, 0xe58} },
    { MMIIM_SP_26KEY_INDEX_C, {4, 0xe3f, 0xe40, 0xe41, 0xe42} },
    { MMIIM_SP_26KEY_INDEX_D, {3, 0xe22, 0xe23, 0xe24} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0xe07, 0xe08, 0xe09} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0xe25, 0xe26, 0xe54} },
    { MMIIM_SP_26KEY_INDEX_G, {3, 0xe27, 0xe28, 0xe55} },
    { MMIIM_SP_26KEY_INDEX_H, {3, 0xe29, 0xe2a, 0xe56} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0xe13, 0xe14, 0xe15} },
    { MMIIM_SP_26KEY_INDEX_J, {3, 0xe2b, 0xe2c, 0xe2d} },
    { MMIIM_SP_26KEY_INDEX_K, {3, 0xe2e, 0xe2f, 0xe30} },
    { MMIIM_SP_26KEY_INDEX_L, {3, 0xe31, 0xe32, 0xe33} },
    { MMIIM_SP_26KEY_INDEX_M, {4, 0xe4c, 0xe4d, 0xe4e, 0xe50} },
    { MMIIM_SP_26KEY_INDEX_N, {4, 0xe49, 0xe4a, 0xe4b, 0xe59} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0xe16, 0xe17, 0xe18} },
    { MMIIM_SP_26KEY_INDEX_P, {3, 0xe19, 0xe1a, 0xe1b} },
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0xe01, 0xe02, 0xe03} },
    { MMIIM_SP_26KEY_INDEX_R, {3, 0xe0a, 0xe0b, 0xe51} },
    { MMIIM_SP_26KEY_INDEX_S, {3, 0xe1f, 0xe20, 0xe21} },
    { MMIIM_SP_26KEY_INDEX_T, {3, 0xe0c, 0xe0d, 0xe52} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0xe10, 0xe11, 0xe12} },
    { MMIIM_SP_26KEY_INDEX_V, {4, 0xe43, 0xe44, 0xe45, 0xe57} },
    { MMIIM_SP_26KEY_INDEX_W, {3, 0xe04, 0xe05, 0xe06} },
    { MMIIM_SP_26KEY_INDEX_X, {4, 0xe37, 0xe38, 0xe39, 0xe3a} },
    { MMIIM_SP_26KEY_INDEX_Y, {3, 0xe0e, 0xe0f, 0xe53} },
    { MMIIM_SP_26KEY_INDEX_Z, {3, 0xe34, 0xe35, 0xe36} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 土耳其语
#ifdef IM_TURKISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_turkish_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0047, 0x011E} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x0130} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x015E} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00DC, 0x00DB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_turkish_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0067, 0x011F} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x0131} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x015F} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00FC, 0x00FB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_turkish_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0041} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0047, 0x011E} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0130, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x015E} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
}; 
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_turkish_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0067, 0x011F} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x0131} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x015F} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_turkish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x41, 0xc2} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x43, 0xc7} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x45} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x47, 0x11e} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x49, 0x130} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x4f, 0xd6} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x53, 0x15e} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x55, 0xdc, 0xdb} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_turkish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x61, 0xe2} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x63, 0xe7} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x65} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x67, 0x11f} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x69, 0x131} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x6f, 0xf6} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x73, 0x15f} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x75, 0xfc, 0xfb} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 乌尔都语
#ifdef IM_URDU_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_urdu[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0645, 0x0698}},
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0634, 0x0624}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x06D2, 0x06D3}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0631, 0x0691}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x06BE, 0x0630}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0646, 0x06BA}},
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0644, 0x06C2}},
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x06C1, 0x0621}},
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x0628, 0x0640}},
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0627, 0x0622}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x06A9, 0x06AF}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x06CC, 0x064A}},
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x0639}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x063A, 0x0626}},
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x062C, 0x0686}},
    { MMIIM_SP_26KEY_INDEX_P, {4, 0x062D, 0x061B, 0x062E, 0x0637}},
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x062A, 0x0638}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x062F, 0x0688}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0648, 0x0632}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0679, 0x062B}},
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x06C3}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0633}},
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0635, 0x0636}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0641}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x067E}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x0642}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {2, 0x060C, 0x061F}},

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_urdu[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0633,  0x0634} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0698} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0621, 0x06be} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x067E} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x062B, 0x0679} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0628, 0x062A} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0644} },
    {MMIIM_SP_26KEY_INDEX_H, {3, 0x0627, 0x0622, 0x0623} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x062E} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0686} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x06ba, 0x0646} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0645} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x06A9} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x06AF} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x062D, 0x062C} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x06d2, 0x06d3} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0635, 0x0636} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0648, 0x0624} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0641} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0639, 0x063A} },
    {MMIIM_SP_26KEY_INDEX_V, {3, 0x0631, 0x0632, 0x0691} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0637, 0x0638} },
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x062f, 0x0630, 0x0688} },
    {MMIIM_SP_26KEY_INDEX_Y, {3, 0x06c1, 0x06c2, 0x06c3} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x06CC, 0x0626} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_urdu[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x633, 0x634} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x621} },
    { MMIIM_SP_26KEY_INDEX_C, {3, 0x6c1, 0x6c2, 0x6c3} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x6d2, 0x6d3} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x62b} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x628, 0x67e} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x644} },
    { MMIIM_SP_26KEY_INDEX_H, {3, 0x627, 0x622, 0x623} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x62e} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x62a, 0x679} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x646, 0x6ba} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x645} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6be} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x6a9, 0x6af} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x62d} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x62c, 0x686} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x637, 0x638} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x642} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x648, 0x624} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x641} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x639} },
    { MMIIM_SP_26KEY_INDEX_V, {4, 0x631, 0x632, 0x698, 0x691} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x635, 0x636} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x630, 0x688} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x63a} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x626, 0x6cc} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 越南语
#ifdef IM_VIETNAMESE_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_vietnamese_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x0102, 0x00C2}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0042}},
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0043}},
    { MMIIM_SP_26KEY_INDEX_D, {3, 0x0044, 0x0110, 0x20AB}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00CA}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0300, 0x0046}},
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0047}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0048}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0049}},
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0323, 0x004A}},
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x004B}},
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x004C}},
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x004D}},
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x004E}},
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D4, 0x01A0}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0050}},
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0309, 0x0052}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0301, 0x0053}},
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0054}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x01AF}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0056}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0057}},
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x0303, 0x0058}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A}},
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_vietnamese_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x0103, 0x00E2}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0062}},
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x0063}},
    { MMIIM_SP_26KEY_INDEX_D, {3, 0x0064, 0x0111, 0x20AB}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00EA}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0300, 0x0066}},
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0067}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0068}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0069}},
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0323, 0x006A}},
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x006B}},
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x006C}},
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x006D}},
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x006E}},
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F4, 0x01A1}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0070}},
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0309, 0x0072}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0301, 0x0073}},
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0074}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x01B0}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0076}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0077}},
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x0303, 0x0078}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A}},
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_vietnamese_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0061, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x0103} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x0111} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x0065, 0x00E8, 0x00E9, 0x00EA} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0066, 0x0303} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0067, 0x0309} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0068, 0x0323} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0069, 0x00EC, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {6, 0x006F, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x01A1} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0072, 0x0300} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0301} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0075, 0x00F9, 0x00FA, 0x01B0} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FD} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_vietnamese_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0041, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x0102} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x0110} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x0045, 0x00C8, 0x00C9, 0x00CA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0049, 0x00CC, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {6, 0x004F, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x01A0} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0055, 0x00D9, 0x00DA, 0x01AF} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DD} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_vietnamese_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x41, 0xc2, 0x102} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x43} },
    { MMIIM_SP_26KEY_INDEX_D, {3, 0x44, 0x110, 0x0323} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x45, 0xca} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x49} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x4f, 0xd4, 0x1a0} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x51, 0x300, 0x301} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x53} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x55, 0x1af} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {3, 0x57, 0x303, 0x309} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_vietnamese_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x61, 0xe2, 0x103} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x63} },
    { MMIIM_SP_26KEY_INDEX_D, {3, 0x64, 0x111, 0x323} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x65, 0xea} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x69} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x6f, 0xf4, 0x1a1} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x71, 0x300, 0x301} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x73} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x75, 0x1b0} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {3, 0x77, 0x303, 0x309} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 希伯来语
#ifdef IM_HEBREW_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hebrew[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x05E9} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x05E0} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x05D1} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x05D2} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x05E7} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x05DB} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x05E2} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x05D9} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x05DF} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x05D7} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x05DC} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x05DA, 0x05E3} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x05E6, 0x05E5} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x05DE, 0x05EA} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x05DD} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x05E4} },
     { MMIIM_SP_26KEY_INDEX_Q, { 0} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x05E8} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x05D3} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x05D0} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x05D5} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x05D4} },
    { MMIIM_SP_26KEY_INDEX_W, { 0} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x05E1} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x05D8} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x05D6} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hebrew[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x05D0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x05D1} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x05B0, 0x05B1} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x05D3} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x05B2, 0x05B3, 0x05B4} },
    {MMIIM_SP_26KEY_INDEX_F, {3, 0x05B5, 0x05B6, 0x05B7} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x05D2} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x05D4, 0x05D7} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x05B8, 0x05B9, 0x05BB} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x05BC, 0x05BD, 0x05BF} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x05DA, 0x05DB} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x05DC} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x05DD, 0x05DE} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x05DF, 0x05E0} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x05C1, 0x05C2} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x05E3, 0x05E4} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x05E7} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x05E8} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x05E1} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x05D8} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x05E2, 0x05E5, 0x05E6} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x05D5} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x05EA} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x05E9} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x05D9} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x05D6} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hebrew[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x5d0} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x5d1} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x5b0, 0x5b1} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x5d3} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x5b2, 0x5b3, 0x5b4} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0x5b5, 0x5b6, 0x5b7} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x5d2} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x5d4, 0x5d7} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x5b8, 0x5b9, 0x5bb} },
    { MMIIM_SP_26KEY_INDEX_J, {3, 0x5bc, 0x5bd, 0x5bf} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x5da, 0x5db} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x5dc} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x5dd, 0x5de} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x5df, 0x5e0} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x5c1, 0x5c2} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x5e3, 0x5e4} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x5e7} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x5e8} },
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x5e1} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x5d8} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x5e2, 0x5e5, 0x5e6} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x5d5} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x5ea} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x5e9} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x5d9} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5d6} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 捷克语
#ifdef IM_CZECH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_czech_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E1, 0x00E4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x010F} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x011B, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x0148} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F3, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0072, 0x0159} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0165} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0075, 0x00FA, 0x016F, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_czech_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C1, 0x00C4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x010E} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x011A, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x0147} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D3, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0052, 0x0158} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0164} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0055, 0x00DA, 0x016E, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_czech_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x010E} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x00C9, 0x011A} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x0147} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0052, 0x0158} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0164} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x00DA, 0x016E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_czech_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x010F} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x00E9, 0x011B} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x0148} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0072, 0x0159} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0165} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x00FA, 0x016F} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076}},
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077}},
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078}},
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FD}},
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E}},
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_czech_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x61, 0xe1} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x63, 0x10d} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x64, 0x10f} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x65, 0xe9, 0x11b} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x69, 0xed} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x6e, 0x148} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x6f, 0xf3} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x72, 0x159} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x73, 0x161} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x74, 0x165} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x75, 0xfa, 0x16f} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x79, 0xfd} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x7a, 0x17e} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_czech_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x41, 0xc1} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x43, 0x10c} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x44, 0x10e} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x45, 0xc9, 0x11a} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x49, 0xcd} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x4e, 0x147} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x4f, 0xd3} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x52, 0x158} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x53, 0x160} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x54, 0x164} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x55, 0xda, 0x16e} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x59, 0xdd} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x5a, 0x17d} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 罗马尼亚语
#ifdef IM_ROMANIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_romanian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x0103, 0x00E2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00EE} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0219} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x021B} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_romanian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x0102, 0x00C2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CE} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0218} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x021A} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_romanian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x61, 0xe2, 0x103} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x63} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x65} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x69, 0xee} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x6f} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x73, 0x15f} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x74, 0x163} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x75} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x7a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_romanian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x41, 0xc2, 0x102} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x43} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x45} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x49, 0xce} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x4f} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x53, 0x15e} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x54, 0x162} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x55} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x5a} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 孟加拉语
#ifdef IM_BENGALI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bengali[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x09CB, 0x0993}},
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x09DC, 0x09DD}},
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x09AE, 0x09A3}},
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x09CD, 0x0985}},
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x09BE, 0x0986}},
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x09BF, 0x0987}},
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x09C1, 0x0989}},
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x09AA, 0x09AB}},
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x0997, 0x099C, 0x0998, 0x099D}},
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x09AF, 0x09B0, 0x09DF}},
    {MMIIM_SP_26KEY_INDEX_K, {4, 0x0995, 0x099A, 0x0996, 0x099B}},
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x09A4, 0x099F, 0x09A5, 0x09A0}},
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x09B8, 0x09B6, 0x09B7}},
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x09B2}},
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x09A6, 0x09A1, 0x09A7, 0x09A2}},
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x099E, 0x09CE}},
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x09CC, 0x0994}},
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x09C0, 0x0988}},
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x09C7, 0x098F}},
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x09C2, 0x098A}},
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x09B9, 0x0999}},
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x09A8}},
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x09C8, 0x0990}},
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x0982, 0x0981, 0x0983}},
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x09AC, 0x09AD}},
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x098B, 0x09E0}},
    {MMIIM_SP_26KEY_INDEX_PERIOD, {2, 0x0964, 0x0965}},
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bengali[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x985, 0x986, 0x9be} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x9ac, 0x9ad} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x99a, 0x99b} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x9a6, 0x9a7} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x98f, 0x990, 0x9c7} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0x9a4, 0x9a5, 0x9ce} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x997, 0x998} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x9b9, 0x983} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x987, 0x988, 0x9bf} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x99c, 0x99d} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x995, 0x996} },
    { MMIIM_SP_26KEY_INDEX_L, {3, 0x9b2, 0x982, 0x9cd} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x999, 0x9ae, 0x981} },
    { MMIIM_SP_26KEY_INDEX_N, {3, 0x9a3, 0x9a8, 0x99e} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x993, 0x994, 0x9cb} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x9aa, 0x9ab} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x9a1, 0x9a2} },
    { MMIIM_SP_26KEY_INDEX_R, {3, 0x9b0, 0x98b, 0x9c3} },
    { MMIIM_SP_26KEY_INDEX_S, {3, 0x9b6, 0x9b7, 0x9b8} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x99f, 0x9a0} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x989, 0x98a, 0x9c1} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x9f0, 0x9f1} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x9c2, 0x9c0} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x9dd, 0x9dc} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x9af, 0x9df} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x9c8, 0x9cc} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 斯洛文尼亚语
#ifdef IM_SLOVENIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovenian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovenian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0041} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovenian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0041} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovenian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovenian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x61} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x62} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x63, 0x10d} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x64} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x65} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x66} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x67} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x68} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x69} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x6a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x6b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x6c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x6d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x6e} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x6f} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x70} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x71} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x72} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x73, 0x161} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x74} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x75} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x76} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x77} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x78} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x79} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x7a, 0x17e} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovenian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x41} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x42} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x43, 0x10c} },
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x44} },
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x45} },
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x46} },
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x47} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x48} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x49} },
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x4a} },
    { MMIIM_SP_26KEY_INDEX_K, {1, 0x4b} },
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x4c} },
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x4d} },
    { MMIIM_SP_26KEY_INDEX_N, {1, 0x4e} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x4f} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x50} },
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x51} },
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x52} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x53, 0x160} },
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x54} },
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x55} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x56} },
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x57} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x58} },
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x59} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x5a, 0x17d} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 泰卢固语
#ifdef IM_TELUGU_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_telugu[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0C4B} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0C35} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x0C2E, 0x0C23} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0x0C4D, 0x0C05} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x0C3E, 0x0C06} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0x0C3F, 0x0C07} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0x0C41, 0x0C09} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0x0C2A, 0x0C2B} },
    { MMIIM_SP_26KEY_INDEX_I, { 4, 0x0C17, 0x0C1C, 0x0C18, 0x0C1D} },
    { MMIIM_SP_26KEY_INDEX_J, { 3, 0x0C2F, 0x0C30, 0x0C31} },
    { MMIIM_SP_26KEY_INDEX_K, { 4, 0x0C15, 0x0C1A, 0x0C16, 0x0C1B} },
    { MMIIM_SP_26KEY_INDEX_L, { 4, 0x0C24, 0x0C1F, 0x0C25, 0x0C20} },
    { MMIIM_SP_26KEY_INDEX_M, { 3, 0x0C38, 0x0C36, 0x0C37} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0C32, 0x0C33} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 0x0C26, 0x0C21, 0x0C27, 0x0C22} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x0C1E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 4, 0x0C4C, 0x0C4A, 0x0C14, 0x0C12} },
    { MMIIM_SP_26KEY_INDEX_R, { 2, 0x0C40, 0x0C08} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0x0C47, 0x0C0F} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x0C42, 0x0C0A} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0x0C39, 0x0C19} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x0C28} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0x0C48, 0x0C10} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0C02} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 0x0C2C, 0x0C2D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0x0C46, 0x0C0E} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 1, 0x0964} },
    { MMIIM_SP_26KEY_INDEX_0, { 2, 0x0C43, 0x0C60} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_telugu[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0C05, 0x0C06, 0x0C10} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0C2C, 0x0C2D} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0C1A, 0x0C1B} },
    {MMIIM_SP_26KEY_INDEX_D, {4, 0x0C21, 0x0C22, 0x0C26, 0x0C27} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0C0E, 0x0C0F} },
    {MMIIM_SP_26KEY_INDEX_F, {6, 0x0C01, 0x0C02, 0x0C03, 0x0C4D, 0x0C55, 0x0C56} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0C17, 0x0C18} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0C39} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0C07, 0x0C08} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x0C1C, 0x0C1D} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0C15, 0x0C16} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0C0C, 0x0C61, 0x0C32, 0x0C33} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0C2E} },
    {MMIIM_SP_26KEY_INDEX_N, {4, 0x0C19, 0x0C1E, 0x0C23, 0x0C28} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0C12, 0x0C13, 0x0C14} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0C2A, 0x0C2B} },
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x0C3E, 0x0C3F, 0x0C40} },
    {MMIIM_SP_26KEY_INDEX_R, {4, 0x0C0B, 0x0C60, 0x0C30, 0x0C31} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x0C36, 0x0C37, 0x0C38} },
    {MMIIM_SP_26KEY_INDEX_T, {4, 0x0C1F, 0x0C20, 0x0C24, 0x0C25} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0C09, 0x0C0A} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0C35} },
    {MMIIM_SP_26KEY_INDEX_W, {4, 0x0C41, 0x0C42, 0x0C43, 0x0C44} },
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x0C46, 0x0C47, 0x0C48} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0C2F} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x0C4A, 0x0C4B, 0x0C4C} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_telugu[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0C4B, 0x0C13} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0C35, 0x0964/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0C2E, 0x0C23} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0C4D, 0x0C05} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0C3E, 0x0C06} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0C3F, 0x0C07} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0C41, 0x0C09} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0C2A, 0x0C2B} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0C17, 0x0C1C, 0x0C18, 0x0C1D} },
    { MMIIM_SP_26KEY_INDEX_J, {3, 0x0C2F, 0x0C30, 0x0C31} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0C15, 0x0C1A, 0x0C16, 0x0C1B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0C24, 0x0C1F, 0x0C25, 0x0C20} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0C38, 0x0C36, 0x0C37} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0C32, 0x0C33} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0C26, 0x0C21, 0x0C27, 0x0C22} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0C1E, 0x0C43/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x0C4C, 0x0C4A, 0x0C14, 0x0C12} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0C40, 0x0C08} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0C47, 0x0C0F} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0C42, 0x0C0A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0C39, 0x0C19} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0C28, 0x0C60/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0C48, 0x0C10} },
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0C02} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0C2C, 0x0C2D} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0C46, 0x0C0E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 泰米尔语
#ifdef IM_TAMIL_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tamil[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0BCB, 0x0B93}},
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0BB5, 0x0BB4}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0BAE, 0x0BA3}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0BCD, 0x0B85}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0BBE, 0x0B86}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0BBF, 0x0B87}},
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0BC1, 0x0B89}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0BAA}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0B9C}},
    { MMIIM_SP_26KEY_INDEX_J, {3, 0x0BAF, 0x0BB0, 0x0BB1}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x0B95, 0x0B9A}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0BA4, 0x0B9F}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x0BB8, 0x0BB7}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0BB2, 0x0BB3}},
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x0B9E}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0BCC}},
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x0BCA, 0x0B94, 0x0B92}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0BC0, 0x0B88}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0BC7, 0x0B8F}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0BC2, 0x0B8A}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0BB9, 0x0B99}},
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0BA8, 0x0BA9}},
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0BC8, 0x0B90}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0B83}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0BC6}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x0B8E}},

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tamil[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0BAF, 0x0BC8} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0BB6, 0x0BB7} },
    {MMIIM_SP_26KEY_INDEX_C, {4, 0x0B89, 0x0B8A, 0x0BC1, 0x0BC2} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0BA9, 0x0B83} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0BA8, 0x0BCD} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0B95, 0x0BF0} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0BAA, 0x0BF1} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0BB9, 0x0BBE} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0BB8, 0x0B90, 0x0B9C} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x0BA4, 0x0BF2} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0BAE, 0x0BF3} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0B9F, 0x0BF4} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x0B85, 0x0B86} },
    {MMIIM_SP_26KEY_INDEX_N, {4, 0x0BC6, 0x0BC7, 0x0BCA, 0x0BCB} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0BB4, 0x0BF5} },
    {MMIIM_SP_26KEY_INDEX_P, {4, 0x0B87, 0x0B88, 0x0BBF, 0x0BC0} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0B99, 0x0B9E} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0B9A, 0x0BF6} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0BB3, 0x0BF7} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0BB5, 0x0BF8} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0BB0, 0x0BF9} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0B8E, 0x0B8F} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0BB1, 0x0BFA} },
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x0B92, 0x0B93, 0x0B94} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0BB2} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x0BA3, 0x0BCC, 0x0BD7} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tamil[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0B93, 0x0BCB} },    //multitap使用该字符显示时，宽度过大，改为小些的字符显示
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0BB5, 0x0BB4} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0BAE, 0x0BA3} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0BCD, 0x0B85} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0BBE, 0x0B86} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0BBF, 0x0B87} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0BC1, 0x0B89} },
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0BAA} },
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0B9C} },
    { MMIIM_SP_26KEY_INDEX_J, {3, 0x0BAF, 0x0BB0, 0x0BB1} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x0B95, 0x0B9A} },
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0BA4, 0x0B9F} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x0BB8, 0x0BB7} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0BB2, 0x0BB3} },
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x0964/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0B9E} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x0B92, 0x0BCC, 0x0BCA, 0x0B94} },        //multitap使用该字符显示时，宽度过大，改为小些的字符显示
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0BC0, 0x0B88} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0BC7, 0x0B8F} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0BC2, 0x0B8A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0BB9, 0x0B99} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0BA8, 0x0BA9} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0B90, 0x0BC8} },    //multitap使用该字符显示时，宽度过大，改为小些的字符显示
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0B83} },
    { MMIIM_SP_26KEY_INDEX_Y, {0} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0BC6, 0x0B8E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 古吉拉特语
#ifdef IM_GUJARATI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_gujarati[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0ACB, 0X0A93} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0AB5} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0X0AAE, 0X0AA3} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0X0ACD, 0X0A85} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0X0ABE, 0X0A86} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0X0ABF, 0X0A87} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0X0AC1, 0X0A89} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0X0AAA, 0X0AAB} },
    { MMIIM_SP_26KEY_INDEX_I, { 4, 0X0A97, 0X0A9C, 0X0A98, 0X0A9D} },
    { MMIIM_SP_26KEY_INDEX_J, { 2, 0X0AAF, 0X0AB0} },
    { MMIIM_SP_26KEY_INDEX_K, { 4, 0X0A95, 0X0A9A, 0X0A96, 0X0A9B} },
    { MMIIM_SP_26KEY_INDEX_L, { 4, 0X0AA4, 0X0A9F, 0X0AA5, 0X0AA0} },
    { MMIIM_SP_26KEY_INDEX_M, { 3, 0X0AB8, 0X0AB6, 0X0AB7} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0X0AB2, 0X0AB3} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 0X0AA6, 0X0AA1, 0X0AA7, 0X0AA2} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0A9E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0X0ACC, 0X0A94} },
    { MMIIM_SP_26KEY_INDEX_R, { 2, 0X0AC0, 0X0A88} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0X0AC7, 0X0A8F} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0X0AC2, 0X0A8A} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0X0AB9, 0X0A99} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0AA8} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0X0AC8, 0X0A90} },
    { MMIIM_SP_26KEY_INDEX_X, { 3, 0X0A82, 0X0A81, 0X0A83} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 0X0AAC, 0X0AAD} },
    { MMIIM_SP_26KEY_INDEX_Z, { 0} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 1, 0X0994} },
    { MMIIM_SP_26KEY_INDEX_0, { 2, 0X0AC3, 0X0AE0} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_gujarati[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0A85, 0x0A86, 0x0A90} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0AAC, 0x0AAD} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0A9A, 0x0A9B} },
    {MMIIM_SP_26KEY_INDEX_D, {4, 0x0AA1, 0x0AA2, 0x0AA6, 0x0AA7} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0A8D, 0x0A8F} },
    {MMIIM_SP_26KEY_INDEX_F, {5, 0x0A81, 0x0A82, 0x0A83, 0x0ACD, 0x0ABC} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0A97, 0x0A98} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0AB9} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0A87, 0x0A88} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x0A9C, 0x0A9D} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0A95, 0x0A96} },
    {MMIIM_SP_26KEY_INDEX_L, {6, 0x0A8C, 0x0AB2, 0x0AB3, 0x0AE1, 0x0AE2, 0x0AE3} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0AAE} },
    {MMIIM_SP_26KEY_INDEX_N, {4, 0x0A99, 0x0A9E, 0x0AA3, 0x0AA8} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0A91, 0x0A93, 0x0A94} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0AAA, 0x0AAB} },
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x0ABE, 0x0ABF, 0x0AC0} },
    {MMIIM_SP_26KEY_INDEX_R, {4, 0x0AB0, 0x0A8B, 0x0AC3, 0x0AC4} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x0AB6, 0x0AB7, 0x0AB8} },
    {MMIIM_SP_26KEY_INDEX_T, {4, 0x0A9F, 0x0AA0, 0x0AA4, 0x0AA5} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0A89, 0x0A8A} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0AB5} },
    {MMIIM_SP_26KEY_INDEX_W, {4, 0x0AC1, 0x0AC2, 0x0AC5, 0x0AC7} },
    {MMIIM_SP_26KEY_INDEX_X, {4, 0x0AC8, 0x0AC9, 0x0ACB, 0x0ACC} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0AAF} },
    {MMIIM_SP_26KEY_INDEX_Z, {4, 0x0AD0, 0x0AE0, 0x0AF1, 0x0ABD} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_gujarati[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0ACB, 0x0A93} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0AB5} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0AAE, 0x0AA3} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0ACD, 0x0A85} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0ABE, 0x0A86} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0ABF, 0x0A87} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0AC1, 0x0A89} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0AAA, 0x0AAB} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0A97, 0x0A9C, 0x0A98, 0x0A9D} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0AAF, 0x0AB0} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0A95, 0x0A9A, 0x0A96, 0x0A9B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0AA4, 0x0A9F, 0x0AA5, 0x0AA0} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0AB8, 0x0AB6, 0x0AB7} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0AB2, 0x0AB3} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0AA6, 0x0AA1, 0x0AA7, 0x0AA2} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0A9E} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0ACC, 0x0A94} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0AC0, 0x0A88} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0AC7, 0x0ABF} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0AC2, 0x0A8A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0AB9, 0x0A99} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0AA8} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0AC8, 0x0A90} },
    { MMIIM_SP_26KEY_INDEX_X, {3, 0x0A82, 0x0A81, 0x0A83} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0AAC, 0x0AAD} },
    { MMIIM_SP_26KEY_INDEX_Z, {3, 0x0994/*TODO: CHECKME THIS KEY*/, 0x0AC3/*TODO: CHECKME THIS KEY*/, 0x0AE0/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 卡纳达语
#ifdef IM_KANNADA_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_kannada[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0CCB, 0x0C93}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0CB5}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0CAE, 0x0CA3}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0CCD, 0x0C85}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0CBE, 0x0C86}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0CBF, 0x0C87}},
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0CC1, 0x0C89}},
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0CAA, 0x0CAB}},
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0C97, 0x0C9C, 0x0C98, 0x0C9D}},
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0CB0, 0x0CAF}},
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0C95, 0x0C9A, 0x0C96, 0x0C9B}},
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0CA4, 0x0C9F, 0x0CA5, 0x0CA0}},
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0CB8, 0x0CB6, 0x0CB7}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0CB2, 0x0CB3}},
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0CA6, 0x0CA1, 0x0CA7, 0x0CA2}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0C9E}},
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x0CCC, 0x0CCA, 0x0C94, 0x0C92}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0CC0, 0x0C88}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0CC7, 0x0C8F}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0CC2, 0x0C8A}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0CB9, 0x0C99}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0CA8}},
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0CC8, 0x0C90}},
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x0C82, 0x0C83}},
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0CAC, 0x0CAD}},
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0CC6, 0x0C8E}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x0964}},
    { MMIIM_SP_26KEY_INDEX_0, { 3, 0x30, 0X0CC3, 0X0CE0} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_kannada[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0C85, 0x0C86, 0x0C90} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0CAC, 0x0CAD} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0C9A, 0x0C9B} },
    {MMIIM_SP_26KEY_INDEX_D, {4, 0x0CA1, 0x0CA2, 0x0CA6, 0x0CA7} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0C8E, 0x0C8F} },
    {MMIIM_SP_26KEY_INDEX_F, {5, 0x0C82, 0x0C83, 0x0CCD, 0x0CD5, 0x0CD6} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0C97, 0x0C98} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0CB9} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0C87, 0x0C88} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x0C9C, 0x0C9D} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0C95, 0x0C96} },
    {MMIIM_SP_26KEY_INDEX_L, {5, 0x0C8C, 0x0CB2, 0x0CB3, 0x0CDE, 0x0CE0} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0CAE} },
    {MMIIM_SP_26KEY_INDEX_N, {4, 0x0C99, 0x0C9E, 0x0CA3, 0x0CA8} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0C92, 0x0C93, 0x0C94} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0CAA, 0x0CAB} },
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x0CBE, 0x0CBF, 0x0CC0} },
    {MMIIM_SP_26KEY_INDEX_R, {4, 0x0CB0, 0x0CB1, 0x0C8B, 0x0CE1} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x0CB6, 0x0CB7, 0x0CB8} },
    {MMIIM_SP_26KEY_INDEX_T, {4, 0x0C9F, 0x0CA0, 0x0CA4, 0x0CA5} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0C89, 0x0C8A} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0CB5} },
    {MMIIM_SP_26KEY_INDEX_W, {4, 0x0CC1, 0x0CC2, 0x0CC3, 0x0CC4} },
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x0CC6, 0x0CC7, 0x0CC8} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0CAF} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x0CCA, 0x0CCB, 0x0CCC} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_kannada[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0CCB, 0x0C93} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0CB5, 0x0964/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0CAE, 0x0CA3} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0CCD, 0x0C85} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0CBE, 0x0C86} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0CBF, 0x0C87} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0CC1, 0x0C89} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0CAA, 0x0CAB} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0C97, 0x0C9C, 0x0C98, 0x0C9D} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0CAF, 0x0CB0} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0C95, 0x0C9A, 0x0C96, 0x0C9B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0CA4, 0x0C9F, 0x0CA5, 0x0CA0} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0CB8, 0x0CB6, 0x0CB7} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0CB2, 0x0CB3} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0CA6, 0x0CA1, 0x0CA7, 0x0CA2} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0C9E, 0x0CC3/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x0CCC, 0x0CCA, 0x0C94, 0x0C92} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0CC0, 0x0C88} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0CC7, 0x0C8F} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0CC2, 0x0C8A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0CB9, 0x0C99} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0CA8, 0x0CE0/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0CC8, 0x0C90} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x0C82, 0x0C83} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0CAC, 0x0CAD} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0CC6, 0x0C8E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 马拉雅拉姆语
#ifdef IM_MALAYALAM_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_malayalam[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0D4B, 0X0D13} },
    { MMIIM_SP_26KEY_INDEX_B, { 2, 0X0D35, 0X0D34} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0X0D2E, 0X0D23} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0X0D4D, 0X0D05} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0X0D3E, 0X0D06} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0X0D3F, 0X0D07} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0X0D41, 0X0D09} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0X0D2A, 0X0D2B} },
    { MMIIM_SP_26KEY_INDEX_I, { 4, 0X0D17, 0X0D1C, 0X0D18, 0X0D1D} },
    { MMIIM_SP_26KEY_INDEX_J, { 3, 0X0D2F, 0X0D30, 0X0D31} },
    { MMIIM_SP_26KEY_INDEX_K, { 4, 0X0D15, 0X0D1A, 0X0D16, 0X0D1B} },
    { MMIIM_SP_26KEY_INDEX_L, { 4, 0X0D24, 0X0D1F, 0X0D25, 0X0D20} },
    { MMIIM_SP_26KEY_INDEX_M, { 3, 0X0D38, 0X0D36, 0X0D37} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0X0D32, 0X0D33} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 0X0D26, 0X0D21, 0X0D27, 0X0D22} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0D1E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 4, 0X0D4C, 0X0D4A, 0X0D14, 0X0D12} },
    { MMIIM_SP_26KEY_INDEX_R, { 2, 0X0D40, 0X0D08} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0X0D47, 0X0D0E} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0X0D42, 0X0D0A} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0X0D39, 0X0D19} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0D28} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0X0D48, 0X0D10} },
    { MMIIM_SP_26KEY_INDEX_X, { 2, 0X0D02, 0X0D03} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 0X0D2C, 0X0D2D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0X0D46, 0X0D0E} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 1, 0X0964} },
    { MMIIM_SP_26KEY_INDEX_0, { 3, 0x30, 0X0D43, 0X0D60} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_malayalam[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x0D4B, 0x0D13, 0x0964/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0D35, 0x0D34} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0D2E, 0x0D23} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0D4D, 0x0D05} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0D3E, 0x0D06} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0D3F, 0x0D07} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0D41, 0x0D09} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0D2A, 0x0D2B} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0D17, 0x0D1C, 0x0D18, 0x0D1D} },
    { MMIIM_SP_26KEY_INDEX_J, {3, 0x0D2F, 0x0D30, 0x0D31} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0D15, 0x0D1A, 0x0D16, 0x0D1B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0D24, 0x0D1F, 0x0D25, 0x0D20} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0D38, 0x0D36, 0x0D37} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0D32, 0x0D33} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0D26, 0x0D21, 0x0D27, 0x0D22} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0D1E, 0x0D43/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x0D4C, 0x0D4A, 0x0D14, 0x0D12} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0D40, 0x0D08} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0D47, 0x0D0E} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0D42, 0x0D0A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0D39, 0x0D19} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0D28, 0x0D60/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0D48, 0x0D10} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x0D02, 0x0D03} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0D2C, 0x0D2D} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0D46, 0x0D0E} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 1, ','} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

//缅甸
#ifdef IM_MYANMAR_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_myanmar[] =		//因9键数据有误，使用了sogou的数据，故为了程序统一性，这里也是有sogou版本
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x1010, 0x1031} },
    {MMIIM_SP_26KEY_INDEX_B, {3, 0x101D, 0x102D, 0x103D} },
    {MMIIM_SP_26KEY_INDEX_C, {3, 0x101B, 0x1030, 0x104D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x1012, 0x102B} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x1002, 0x1032} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x1013, 0x102E} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x1014, 0x103A, 0x1085} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x1015, 0x1036} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x100A, 0x1025, 0x1009, 0x1026} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x1016, 0x1037} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x1017, 0x103B} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x1018, 0x103C, 0x1029, 0x102A} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x101F, 0x1021} },
    {MMIIM_SP_26KEY_INDEX_N, {3, 0x101E, 0x103F, 0x1038} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x100C, 0x100B} },
    {MMIIM_SP_26KEY_INDEX_P, {4, 0x100F, 0x100D, 0x100E, 0x104C} },
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x1000, 0x1023, 0x1024} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x1004, 0x1003} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x1011, 0x102C} },
    {MMIIM_SP_26KEY_INDEX_T, {3, 0x1005, 0x1027, 0x104F} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x1007, 0x1008} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x101C, 0x1020} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x1001, 0x1039} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x101A, 0x102F} },
    {MMIIM_SP_26KEY_INDEX_Y, {3, 0x1006, 0x104A, 0x104B} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x1019, 0x103E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_myanmar[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x1010, 0x1031} },
    {MMIIM_SP_26KEY_INDEX_B, {3, 0x101D, 0x102D, 0x103D} },
    {MMIIM_SP_26KEY_INDEX_C, {3, 0x101B, 0x1030, 0x104D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x1012, 0x102B} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x1002, 0x1032} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x1013, 0x102E} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x1014, 0x103A, 0x1085} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x1015, 0x1036} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x100A, 0x1025, 0x1009, 0x1026} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x1016, 0x1037} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x1017, 0x103B} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x1018, 0x103C, 0x1029, 0x102A} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x101F, 0x1021} },
    {MMIIM_SP_26KEY_INDEX_N, {3, 0x101E, 0x103F, 0x1038} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x100C, 0x100B} },
    {MMIIM_SP_26KEY_INDEX_P, {4, 0x100F, 0x100D, 0x100E, 0x104C} },
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x1000, 0x1023, 0x1024} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x1004, 0x1003} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x1011, 0x102C} },
    {MMIIM_SP_26KEY_INDEX_T, {3, 0x1005, 0x1027, 0x104F} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x1007, 0x1008} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x101C, 0x1020} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x1001, 0x1039} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x101A, 0x102F} },
    {MMIIM_SP_26KEY_INDEX_Y, {3, 0x1006, 0x104A, 0x104B} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x1019, 0x103E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else	
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_myanmar[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x1010, 0x1031} },
    { MMIIM_SP_26KEY_INDEX_B, {3, 0x101D, 0x102D, 0x103D} },
    { MMIIM_SP_26KEY_INDEX_C, {3, 0x101B, 0x1030, 0x104D} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x1012, 0x102B} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x1002, 0x1032} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x1013, 0x102E} },
    { MMIIM_SP_26KEY_INDEX_G, {3, 0x1014, 0x103A, 0x1085} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x1015, 0x1036} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x100A, 0x1025, 0x1009, 0x1026} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x1016, 0x1037} },
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x1017, 0x103B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x1018, 0x103C, 0x1029, 0x102A} },
    { MMIIM_SP_26KEY_INDEX_M, {0x101F, 0x1021} },
    { MMIIM_SP_26KEY_INDEX_N, {3, 0x101E, 0x103F, 0x1038} },
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x100C, 0x100B} },
    { MMIIM_SP_26KEY_INDEX_P, {4, 0x100F, 0x100D, 0x100E, 0x104C} },
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x1000, 0x1023, 0x1024} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x1003, 0x1004} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x1011, 0x102C} },
    { MMIIM_SP_26KEY_INDEX_T, {3, 0x1005, 0x1027, 0x104F} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x1007, 0x1008} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x101C, 0x1020} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x1001, 0x1039} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x101A, 0x102F} },
    { MMIIM_SP_26KEY_INDEX_Y, {3, 0x1006, 0x104A, 0x104B} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x1019, 0x103E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif//S9  
#endif//IM_MYANMAR_SUPPORT


// 奥利亚语
#ifdef IM_ORIYA_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_oriya[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0B4B, 0X0B13} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0B71} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0X0B2E, 0X0B23} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0X0B4D, 0X0B05} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0X0B3E, 0X0B06} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0X0B3F, 0X0B07} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0X0B41, 0X0B09} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0X0B2A, 0X0B2B} },
    { MMIIM_SP_26KEY_INDEX_I, { 4, 0X0B17, 0X0B1C, 0X0B18, 0X0B1D} },
    { MMIIM_SP_26KEY_INDEX_J, { 3, 0X0B5F, 0X0B30, 0X0B2F} },
    { MMIIM_SP_26KEY_INDEX_K, { 4, 0X0B15, 0X0B1A, 0X0B16, 0X0B1B} },
    { MMIIM_SP_26KEY_INDEX_L, { 4, 0X0B24, 0X0B1F, 0X0B25, 0X0B20} },
    { MMIIM_SP_26KEY_INDEX_M, { 3, 0X0B38, 0X0B36, 0X0B37} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0X0B32, 0X0B33} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 0X0B26, 0X0B21, 0X0B27, 0X0B22} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0B1E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0X0B4C, 0X0B14} },
    { MMIIM_SP_26KEY_INDEX_R, { 2, 0X0B40, 0X0B08} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0X0B47, 0X0B0F} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0X0B42, 0X0B0A} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0X0B39, 0X0B19} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0B28} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0X0B48, 0X0B10} },
    { MMIIM_SP_26KEY_INDEX_X, { 3, 0X0B02, 0X0B01, 0X0B03} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 0X0B2C, 0X0B2D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 0} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 1, 0X0964} },
    { MMIIM_SP_26KEY_INDEX_0, { 2, 0X0B43, 0X0B60} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_oriya[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0B4B, 0x0B13} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0B71} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0B2E, 0x0B23} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0B4D, 0x0B05} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0B3E, 0x0B06} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0B3F, 0x0B07} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0B41, 0x0B09} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0B2A, 0x0B2B} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0B17, 0x0B1C, 0x0B18, 0x0B1D} },
    { MMIIM_SP_26KEY_INDEX_J, {3, 0x0B5F, 0x0B30, 0x0B2F} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0B15, 0x0B1A, 0x0B16, 0x0B1B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0B24, 0x0B1F, 0x0B25, 0x0B20} },
    { MMIIM_SP_26KEY_INDEX_M, {3, 0x0B38, 0x0B36, 0x0B37} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0B32, 0x0B33} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0B26, 0x0B21, 0x0B27, 0x0B22} },
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0B1E} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0B4C, 0x0B14} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0B40, 0x0B08} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0B47, 0x0B0F} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0B42, 0x0B0A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0B39, 0x0B19} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0B28} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0B48, 0x0B10} },
    { MMIIM_SP_26KEY_INDEX_X, {3, 0x0B02, 0x0B01, 0x0B03} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0B2C, 0x0B2D} },
    { MMIIM_SP_26KEY_INDEX_Z, {3, 0x0964/*TODO: CHECKME THIS KEY*/, 0x0B43/*TODO: CHECKME THIS KEY*/, 0x0B60/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif

// 印度旁遮普语
#ifdef IM_PUNJABI_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_punjabi[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0A4B, 0x0A13}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0A35}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0A2E, 0x0A23}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0A4D, 0x0A05}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0A3E, 0x0A06}},
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0A3F, 0x0A07}},
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0A41, 0x0A09}},
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0A2A, 0x0A2B}},
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0A17, 0x0A1C, 0x0A18, 0x0A1D}},
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0A2F, 0x0A30}},
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0A15, 0x0A1A, 0x0A16, 0x0A1B}},
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0A24, 0x0A1F, 0x0A25, 0x0A20}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x0A38, 0x0A36}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0A32, 0x0A33}},
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0A26, 0x0A21, 0x0A27, 0x0A22}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0A1E}},
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0A4C, 0x0A14}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0A40, 0x0A08}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0A47, 0x0A0F}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0A42, 0x0A0A}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0A39, 0x0A19}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0A28}},
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0A48, 0x0A10}},
    { MMIIM_SP_26KEY_INDEX_X, {4, 0x0A70, 0x0A02, 0x0A71, 0x0A03}},
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0A42, 0x0A2D}},
    { MMIIM_SP_26KEY_INDEX_Z, {0}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x0964} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_punjabi[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0A05, 0x0A06, 0x0A3E} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0A2C, 0x0A2D} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0A1A, 0x0A1B} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0A26, 0x0A27} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0A0F, 0x0A10, 0x0A47} },
    {MMIIM_SP_26KEY_INDEX_F, {3, 0x0A24, 0x0A25, 0x0A5C} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x0A17, 0x0A5A, 0x0A18} },
    {MMIIM_SP_26KEY_INDEX_H, {2, 0x0A39, 0x0A03} },
    {MMIIM_SP_26KEY_INDEX_I, {3, 0x0A07, 0x0A08, 0x0A3F} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x0A1C, 0x0A5B, 0x0A1D} },
    {MMIIM_SP_26KEY_INDEX_K, {3, 0x0A15, 0x0A16, 0x0A59} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0A32, 0x0A33, 0x0A02, 0x0A4D} },
    {MMIIM_SP_26KEY_INDEX_M, {3, 0x0A19, 0x0A2E, 0x0A01} },
    {MMIIM_SP_26KEY_INDEX_N, {3, 0x0A23, 0x0A28, 0x0A1E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0A13, 0x0A14, 0x0A4B} },
    {MMIIM_SP_26KEY_INDEX_P, {3, 0x0A2A, 0x0A2B, 0x0A5E} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0A21, 0x0A22} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0A30, 0x0A3C} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0A38, 0x0A36} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0A1F, 0x0A20} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0A09, 0x0A0A, 0x0A41} },
    {MMIIM_SP_26KEY_INDEX_V, {3, 0x0A35, 0x0A70, 0x0A71} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0A42, 0x0A40} },
    {MMIIM_SP_26KEY_INDEX_X, {3, 0x0A73, 0x0A74, 0x0A72} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0A2F} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x0A48, 0x0A4C} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_punjabi[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x0A4B, 0x0A13} },
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0A35} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0A2E, 0x0A23} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0A4D, 0x0A05} },
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0A3E, 0x0A06} },
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0A3F, 0x0A07} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0A41, 0x0A09} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0A2A, 0x0A2B} },
    { MMIIM_SP_26KEY_INDEX_I, {4, 0x0A17, 0x0A1C, 0x0A18, 0x0A1D} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0A2F, 0x0A30} },
    { MMIIM_SP_26KEY_INDEX_K, {4, 0x0A15, 0x0A1A, 0x0A16, 0x0A1B} },
    { MMIIM_SP_26KEY_INDEX_L, {4, 0x0A24, 0x0A1F, 0x0A25, 0x0A20} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x0A38, 0x0A36} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0A32, 0x0A33} },
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x0A26, 0x0A21, 0x0A27, 0x0A22} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0A3C, 0x0A1E} },
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0A4C, 0x0A14} },
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0A40, 0x0A08} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0A47, 0x0A0F} },
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0A42, 0x0A0A} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0A39, 0x0A19} },
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0A28} },
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0A48, 0x0A10} },
    { MMIIM_SP_26KEY_INDEX_X, {4, 0x0A70, 0x0A02, 0x0A71, 0x0A03} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0A2C, 0x0A2D} },
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x0964/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 阿萨姆
#if defined(IM_ASSAMESE_SUPPORT)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_assamese[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x09CB, 0x0993} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0964/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x09AE, 0x09A3} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0x09CD, 0x0985} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x09BF, 0x0986} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0x09BF, 0x0987} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0x09C1, 0x0989} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0x09AA, 0x09AB} },
    { MMIIM_SP_26KEY_INDEX_I, { 4, 0x0997, 0x099C, 0x0998, 0x099D} },
    { MMIIM_SP_26KEY_INDEX_J, { 3, 0x09AF, 0x09F0, 0x09DF} },
    { MMIIM_SP_26KEY_INDEX_K, { 4, 0x0995, 0x099A, 0x0996, 0x099B} },
    { MMIIM_SP_26KEY_INDEX_L, { 4, 0x09A4, 0x099F, 0x09A5, 0x09A0} },
    { MMIIM_SP_26KEY_INDEX_M, { 3, 0x09B8, 0x09B6, 0x09B7} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0x09B2} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 0x09A6, 0x09A1, 0x09A7, 0x09A2} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x099E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0x09CC, 0x0994} },
    { MMIIM_SP_26KEY_INDEX_R, { 2, 0x09C0, 0x0988} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0x09C7, 0x098F} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x09C2, 0x098A} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0x09B9, 0x0999} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x09A8} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0x09C8, 0x0990} },
    { MMIIM_SP_26KEY_INDEX_X, { 3, 0x0982, 0x0981, 0x0983} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 0x09AC, 0x09AD} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0x09C3, 0x09E0/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 阿尔巴尼亚语
#ifdef IM_ALBANIAN_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_albanian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'C', 0x00C7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'E', 0x00CB} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_albanian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'c', 0x00E7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0x00EB} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 亚美尼亚语
#ifdef IM_ARMENIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_armenian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0531} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0532} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0533} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0534} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x0537, 0x0545} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0556} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0554} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0540} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x053B, 0x0555} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0543} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0539, 0x053F}},
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x053C, 0x0553}},
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x053E, 0x0544}},
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0546, 0x0547}},
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0548, 0x0549, 0x054C}},
    {MMIIM_SP_26KEY_INDEX_P, {3, 0x053A, 0x054A, 0x054B}},
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x053D}},
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0550}},
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x054D}},
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x054F}},
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0538, 0x0587}},
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x054E}},
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0541, 0x0552}},
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0551} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0535} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0536} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 3, 0x0542, 0x055F, 0x0589} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_armenian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0561}},
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0562}},
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0563}},
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0564}},
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0567, 0x0575}},
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0586}},
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0584}},
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0570}},
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x056B, 0x0585}},
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0573}},
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0569, 0x056F}},
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x056C, 0x0583}},
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x056E, 0x0574}},
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0576, 0x0577}},
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x0578, 0x0579, 0x057C}},
    {MMIIM_SP_26KEY_INDEX_P, {3, 0x056A, 0x057A, 0x057B}},
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x056D}},
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0580}},
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x057D}},
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x057F}},
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0568, 0x0587}},
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x057E}},
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0571, 0x0582}},
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0581}},
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0565}},
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0566}},
    {MMIIM_SP_26KEY_INDEX_PERIOD, { 3, 0x0572, 0x055F, 0x0589} },    
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_armenian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x0531, 0x0542/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0532} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0533} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0534} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0575} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0556} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0554} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0540} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x053B, 0x0555} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0543} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x053F, 0x0539} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x053C, 0x0553} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x0544, 0x053E} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0546, 0x0547} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x0548, 0x054C} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 0x054A, 0x053A, 0x055E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x053D} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0550} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x054D} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x054F} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0x0538, 0x0587} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x054E} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0x0552, 0x0541} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0551} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0535} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0536} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_armenian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x0561, 0x0572/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0562} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0563} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0564} },
    { MMIIM_SP_26KEY_INDEX_E, { 3, 0x0567, 0x0537, 0x0545} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0586} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0584} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0570} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x056B, 0x0585} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0573} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x056F, 0x0569} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x056C, 0x0583} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x0574, 0x056E} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0576, 0x0577} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 0x0578, 0x057C, 0x0579} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 0x057A, 0x056A, 0x057B} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 0x056D, 0x055D, 0x055C} },
    { MMIIM_SP_26KEY_INDEX_R, { 2, 0x0580, 0x055B} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x057D} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x057F} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0568} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x057E} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0x0582, 0x0571} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0581} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0565} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0566} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 阿塞拜疆语
#ifdef IM_AZERBAIJANI_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_azerbaijani_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'C', 0x00C7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'E', 0X018F} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 'G', 0X011E} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'I', 0X0130} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0X00D6} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 'S', 0X015E} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'U', 0X00DC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_azerbaijani_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'c', 0x00E7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0X0259} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 'g', 0X011F} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0X0131} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0X00F6} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 's', 0X015F} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'u', 0X00FC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 巴斯克语
#ifdef IM_BASQUE_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_basque_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'N', 0xD1} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_basque_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'n', 0xF1} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 保加利亚语
#ifdef IM_BULGARIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bulgarian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x042C}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0424}},
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x042A}},
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0410}},
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0415}},
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x041E}},
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0416}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0413}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0421}},
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x0422}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x041D, 0x041C}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0412, 0x0427}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x041F, 0x0411}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0425, 0x0420}},
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x0414, 0x0406}},
    { MMIIM_SP_26KEY_INDEX_P, {3, 0x0417, 0x0426, 0x00A7}},
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x002C, 0x0060, 0x042B}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0418, 0x0413}},
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x042F}},
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0428}},
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x041A}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x042D}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0423}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0419}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0429}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x042E}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x041B}},
        
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bulgarian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x044C}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0444}},
    { MMIIM_SP_26KEY_INDEX_C, {1, 0x044A}},
    { MMIIM_SP_26KEY_INDEX_D, {1, 0x0430}},
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0435}},
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x043E}},
    { MMIIM_SP_26KEY_INDEX_G, {1, 0x0436}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0433}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0441}},
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x0442}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x043D, 0x043C}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0432, 0x0447}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x043F, 0x0431}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0445, 0x0440}},
    { MMIIM_SP_26KEY_INDEX_O, {2, 0x0434, 0x0456}},
    { MMIIM_SP_26KEY_INDEX_P, {3, 0x0437, 0x0446, 0x00A7}},
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x002C, 0x0060, 0x044B}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0438, 0x0433}},
    { MMIIM_SP_26KEY_INDEX_S, {1, 0x044F}},
    { MMIIM_SP_26KEY_INDEX_T, {1, 0x0448}},
    { MMIIM_SP_26KEY_INDEX_U, {1, 0x043A}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x044D}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0443}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0439}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0449}},
    { MMIIM_SP_26KEY_INDEX_Z, {1, 0x044E}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x043B}},

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bulgarian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0410} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0411} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0426} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0414} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0415} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0424} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0413} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0427} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0418, 0x0419} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x042C, 0x042A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x041A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x041B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x041C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x041D} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x041E} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x041F} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x042F} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0420} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0421} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0422} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x042E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0412} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0428, 0x0429} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0425, 0x0416} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0423} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0417} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bulgarian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0430} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0431} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0446} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0434} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0435} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0444} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0433} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0447} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0438, 0x0439} },
    {MMIIM_SP_26KEY_INDEX_J, {2, 0x044C, 0x044A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x043A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x043B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x043C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x043D} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x043E} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x043F} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x044F} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0440} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0441} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0442} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x044E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0432} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0448, 0x0449} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0445, 0x0436} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0443} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0437} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bulgarian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x042C, 0x041B/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0424} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x042A} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0410} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0415} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x041E} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0416} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0413} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0421} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0422} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x041D, 0x041C} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x0412, 0x0427} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x041F, 0x0411} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0425, 0x0420} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x0414} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0x0417, 0x0426} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0418} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x042F} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x0428} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x041A} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x042D} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0423} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0419} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0429} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x042E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_bulgarian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x044C, 0x043B/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0444} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x044A} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0430} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0435} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x043E} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0436} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0433} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x0441, 0x2116} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0442} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x043D, 0x043C} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x0432, 0x0447} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x043F, 0x0431} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0445, 0x0440} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x0434, 0x0406} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 0x0437, 0x0446, 0x00A7} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 0x002C, 0x0060, 0x044B} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0438} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x044F} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x0448} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x043A} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x044D} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0443} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0439} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0449} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x044E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 加泰罗尼亚语
#ifdef IM_CATALAN_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_catalan_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'A', 0XC0} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'C', 0XC7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 3, 'E', 0xC8, 0xC9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'I', 0xCC} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0xD2} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'U', 0xDC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_catalan_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'a', 0xE0} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'c', 0xE7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 3, 'e', 0xE8, 0xE9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0xEC} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0xF2} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'u', 0xFC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_croatian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0X0041} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0042} },
    { MMIIM_SP_26KEY_INDEX_C, { 3, 0X0043, 0X010C, 0X0106} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0X0044, 0X0110} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0045} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0046} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0047} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0048} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X0049} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X004A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X004B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X004C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X004D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X004E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0X004F} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0050} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0X00B8, 0X0051} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0052} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0X0053, 0X0160} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0054} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X0055} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0056} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0X00A8, 0X0057} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0058} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0059} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0X005A, 0X017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_croatian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0X0061} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0062} },
    { MMIIM_SP_26KEY_INDEX_C, { 3, 0X0063, 0X010D, 0X0107} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0X0064, 0X0111} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0065} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0066} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0067} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0068} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X0069} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X006A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X006B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X006C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X006D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X006E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0X006F} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0070} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0X0071, 0X00B8} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0072} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0X0073, 0X0161} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0074} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X0075} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0076} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 0X0077, 0X00A8} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0078} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0079} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0X007A, 0X017E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_croatian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 3, 'C', 0x10C, 0x106} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 'D', 0x0110} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 'Q', 0x00B8} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 'S', 0x0160} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 'W', 0x00A8} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 'Z', 0x017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_croatian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 3, 'c', 0x10D, 0x107} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 'd', 0x0111} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 'q', 0x00B8} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 's', 0x0161} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 2, 'w', 0x00A8} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 'z', 0x017E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 丹麦语
#ifdef IM_DANISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_danish_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0041, 0x00C5, 0x00C6, 0x00C4, 0x00C0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x00D0} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x004C, 0x00D8} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D6, 0x00D3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_danish_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0061, 0x00E5, 0x00E6, 0x00E4, 0x00E0} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x00F0} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x006C, 0x00F8} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F6, 0x00F3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_danish_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C5, 0x00C6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D8} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_danish_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E5, 0x00E6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F8} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_danish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'A', 0x00C5} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 'K', 0x00C6} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 'L', 0x00D8} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 'O', 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'P', 0x00A8, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_danish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'a', 0x00E5} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 'k', 0x00E6} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 'l', 0x00F8} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 'o', 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'p', 0x00A8, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#endif
#endif


// 荷兰语
#ifdef IM_DUTCH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_dutch_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0041, 0x00C4, 0x00C1, 0x00C0, 0x00C2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x00C7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {5, 0x0045, 0x00CB, 0x00C9, 0x00C8, 0x00CA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {5, 0x0049, 0x00CF, 0x00CD, 0x00CC, 0x00CE} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {5, 0x004F, 0x00D6, 0x00D3, 0x00D2, 0x00D4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {5, 0x0055, 0x00DC, 0x00DA, 0x00D9, 0x00DB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_dutch_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {5, 0x0061, 0x00E4, 0x00E1, 0x00E0, 0x00E2} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x00E7} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {5, 0x0065, 0x00EB, 0x00E9, 0x00E8, 0x00EA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {5, 0x0069, 0x00EF, 0x00ED, 0x00EC, 0x00EE} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {5, 0x006F, 0x00F6, 0x00F3, 0x00F2, 0x00F4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {5, 0x0075, 0x00FC, 0x00FA, 0x00F9, 0x00FB} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_dutch_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0041} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x0045, 0x00C8, 0x00C9, 0x00CB} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_dutch_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x0065, 0x00E8, 0x00E9, 0x00EB} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00EF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_dutch_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 3, 'L', 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0x007E} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'P', 0x00A8, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_dutch_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 3, 'l', 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0x007E} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'p', 0x00A8, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#endif
#endif


// 爱沙尼亚语
#ifdef IM_ESTONIAN_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_estonian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0041, 0X00C4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0042} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X0043} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X0044} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0045} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0046} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0047} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0048} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X0049} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X004A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X004B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X004C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X004D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X004E} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 0X004F, 0X00D5, 0X00D6} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 0X00B4, 0X0050, 0X0060} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 0X02C7, 0X0051, 0X007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0052} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X0053} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0054} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0X0055, 0X00DC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0056} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X0057} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0058} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0059} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_estonian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0061, 0X00E4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0062} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X0063} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X0064} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0065} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0066} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0067} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0068} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X0069} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X006A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X006B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X006C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X006D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X006E} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 0X006F, 0X00F5, 0X00F6} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 0X0070, 0X00B4, 0X0060} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 0X0071, 0X02C7, 0X007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0072} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X0073} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0074} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 0X0075, 0X00FC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0076} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X0077} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0078} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0079} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_estonian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x00C4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D5, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_estonian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x00E4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F5, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_estonian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'A', 0x00C4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 'O', 0x00D5, 0x00D6} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'P', 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 'Q', 0x02C7, 0x007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'U', 0x00DC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_estonian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'a', 0x00E4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 'o', 0x00F5, 0x00F6} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'p', 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 'q', 0x02C7, 0x007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'u', 0x00FC} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 菲律宾语
#ifdef IM_FILIPINO_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_filipino_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'N', 0xD1} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_filipino_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'n', 0xF1} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 芬兰语
#ifdef IM_FINNISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_finnish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {5, 0x0041, 0x00C5, 0x00C4, 0x00C1, 0x00C2}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0042}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x0110}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9}},
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0046}},
    { MMIIM_SP_26KEY_INDEX_G, {3, 0x0047, 0x01E6, 0x01E4}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0048}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0049}},
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x004A}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x004B, 0x01E8}},
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x004C}},
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x004D}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x014A}},
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x004F, 0x00D6, 0x00D5, 0x00D8}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0050}},
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051}},
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x0052}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0166}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0056}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0057}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0058}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059}},
    { MMIIM_SP_26KEY_INDEX_Z, {4, 0x005A, 0x017D, 0x01B7, 0x01EE}},
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_finnish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {5, 0x0061, 0x00E5, 0x00E4, 0x00E1, 0x00E2}},
    { MMIIM_SP_26KEY_INDEX_B, {1, 0x0062}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x0111}},
    { MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9}},
    { MMIIM_SP_26KEY_INDEX_F, {1, 0x0066}},
    { MMIIM_SP_26KEY_INDEX_G, {3, 0x0067, 0x01E7, 0x01E5}},
    { MMIIM_SP_26KEY_INDEX_H, {1, 0x0068}},
    { MMIIM_SP_26KEY_INDEX_I, {1, 0x0069}},
    { MMIIM_SP_26KEY_INDEX_J, {1, 0x006A}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x006B, 0x01E9}},
    { MMIIM_SP_26KEY_INDEX_L, {1, 0x006C}},
    { MMIIM_SP_26KEY_INDEX_M, {1, 0x006D}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x014B}},
    { MMIIM_SP_26KEY_INDEX_O, {4, 0x006F, 0x00F6, 0x00F5, 0x00F8}},
    { MMIIM_SP_26KEY_INDEX_P, {1, 0x0070}},
    { MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071}},
    { MMIIM_SP_26KEY_INDEX_R, {1, 0x0072}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0167}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC}},
    { MMIIM_SP_26KEY_INDEX_V, {1, 0x0076}},
    { MMIIM_SP_26KEY_INDEX_W, {1, 0x0077}},
    { MMIIM_SP_26KEY_INDEX_X, {1, 0x0078}},
    { MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079}},
    { MMIIM_SP_26KEY_INDEX_Z, {4, 0x007A, 0x017E, 0x0292, 0x01EF}},
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};

#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_finnish_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C4, 0x00C5} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_finnish_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E4, 0x00E5} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_finnish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'A', 0x00C5, 0x00C4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 'O', 0x00D6, 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'P', 0x00A8, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_finnish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'a', 0x00E5, 0x00E4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 4, 'o', 0x00F6, 0x00B4, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'p', 0x00A8, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 加利西亚语
#ifdef IM_GALICIAN_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_galician_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'N', 0xD1} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_galician_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'n', 0xF1} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 格鲁吉亚语
#ifdef IM_GEORGIAN_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_georgian[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0X010D, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0X010E, 0X010F} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0X010D, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0X010E, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0X010D, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X010E} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, { 1, 0X010D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_georgian[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x10E4, 0x10D1/*TODO: CHECKME THIS KEY*/} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x10DB} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x10E7} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x10D5} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x10E3} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x10D7} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x10D0} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x10DE} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x10E8} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x10E0} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x10DD} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x10DA, 0x10DF} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x10E2, 0x10F0} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x10D8, 0x10E5} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x10EC, 0x10EE} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0x10D6, 0x10EA} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x10E6} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x10D9} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x10EB} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x10D4} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x10D2} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x10E1} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x10EF} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x10E9} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x10DC} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x10ED} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 豪撒语
#ifdef IM_HAUSA_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hausa_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0061} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0062, 0x0253} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x0257} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x006B, 0x0199} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0077, 0x0027} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x01B4} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hausa_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0041} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0042, 0x0181} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x018A} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x004B, 0x0198} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x01B3} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hausa_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 2, 'B', 0X0181} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 'D', 0X018A} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 'K', 0X0198} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'Y', 0X01B3} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_hausa_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 2, 'b', 0X0253} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 'd', 0X0257} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 'k', 0X0199} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'y', 0X01B4} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 冰岛语
#ifdef IM_ICELANDIC_SUPPORT
#if defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_icelandic_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C1, 0x00C6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x004F, 0x00D0, 0x00D3, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0050, 0x00DE} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DA} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DD} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_icelandic_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E1, 0x00E6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {4, 0x006F, 0x00F0, 0x00F3, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0070, 0x00FE} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FA} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FD} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_icelandic_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'A', 0x00C1, 0x00C6} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 'D', 0x00D0} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'E', 0x00C9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'I', 0x00CD} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 'O', 0x00D3, 0x00D6} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 'P', 0x00DE} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'Y', 0x00DD} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_icelandic_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 'd', 0x00F0} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0x00E9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0x00ED} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 3, 'o', 0x00F3, 0x00F6} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 'p', 0x00FE} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'y', 0x00FD} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 伊博语
#ifdef IM_IGBO_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_igbo_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'I', 0x1ECA} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'N', 0xD1} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0x1ECC} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'U', 0x1EE4} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_igbo_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0x1ECB} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'n', 0xF1} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0x1ECD} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'u', 0x1EE5} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 爱尔兰语
#ifdef IM_IRISH_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_irish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'A', 0xC1} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'E', 0xC9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'I', 0xCD} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0xD3} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'U', 0xDA} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_irish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'a', 0xE1} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0xE9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0xED} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0xF3} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'u', 0xFA} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 哈萨克斯坦
#ifdef IM_KAZAKH_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_kazakh_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x0424, 0x04D8} },
    { MMIIM_SP_26KEY_INDEX_B, { 2, 0x0418, 0x0406} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x0421, 0x04A2} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0x0412, 0x0492} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x0423, 0x04AE} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0x0410, 0x04B0} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0x041F, 0x049A} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0x0420, 0x04E8} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x0428, 0x04BA} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x041E} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x041B, 0x0416} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x0414, 0x042D} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x042C, 0x042E} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0422, 0x0411} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x0429, 0x0425} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0x0417, 0x042A} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0x0419, 0x0401} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x041A} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x042B} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x0415} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0413} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x041C} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0426} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0427} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x041D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x042F} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_kazakh_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x0444, 0x04D9} },
    { MMIIM_SP_26KEY_INDEX_B, { 2, 0x0438, 0x0456} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x0441, 0x04A3} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0x0432, 0x0493} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x0443, 0x04AF} },
    { MMIIM_SP_26KEY_INDEX_F, { 2, 0x0430, 0x04B1} },
    { MMIIM_SP_26KEY_INDEX_G, { 2, 0x043F, 0x049B} },
    { MMIIM_SP_26KEY_INDEX_H, { 2, 0x0440, 0x04E9} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x0448, 0x04BB} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x043E} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x043B, 0x0436} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x0434, 0x044D} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x044C, 0x044E} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0442, 0x0431} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x0449, 0x0445} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0x0437, 0x044A} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0x0439, 0x0451} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x043A} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x044B} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x0435} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0433} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x043C} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0446} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0447} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x043D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x044F} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 拉脱维亚语
#ifdef IM_LATVIAN_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_latvian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0X0041} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0042} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X0043} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X0044} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0045} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0046} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0047} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0048} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X0049} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X004A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X004B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X004C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X004D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X004E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0X004F} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0X0050, 0X00B0} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0X0051, 0X007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0052} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X0053} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0054} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X0055} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0056} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X0057} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0058} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0059} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_latvian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0X0061} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0062} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X0063} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X0064} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0065} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0066} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0067} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0068} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X0069} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X006A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X006B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X006C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X006D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X006E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0X006F} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0X0070, 0X00B0} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0X0071, 0X007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0072} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X0073} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0074} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X0075} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0076} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X0077} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0078} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0079} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_latvian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 'P', 0x00B0} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 'Q', 0x007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_latvian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 'p', 0x00B0} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 'q', 0x007E} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lithuanian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x0104} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0045, 0x0118, 0x0116} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x012E} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x004F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0055, 0x0172, 0x016A} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lithuanian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x0105} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {3, 0x0065, 0x0119, 0x0117} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x012F} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x0075, 0x0173, 0x016B} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lithuanian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0x0041, 0x0104} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0042} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x0043, 0x010C} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0044} },
    { MMIIM_SP_26KEY_INDEX_E, { 3, 0x0045, 0x0118, 0x0116} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0046} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0047} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0048} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 0x0049, 0x012E} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x004A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x004B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0x004C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0x004D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0x004E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x004F} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x0050} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x0051} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0052} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 0x0053, 0x0160} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0x0054} },
    { MMIIM_SP_26KEY_INDEX_U, { 3, 0x0055, 0x0172, 0x016A} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x0056} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0057} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0058} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0059} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0x0060, 0x017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lithuanian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x0105}},
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062}},
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D}},
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064}},
    { MMIIM_SP_26KEY_INDEX_E, { 3, 0x0065, 0x0119, 0x0117} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066}},
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067}},
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068}},
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x012F}},
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A}},
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B}},
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C}},
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D}},
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E}},
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x006F}},
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070}},
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071}},
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072}},
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161}},
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074}},
    { MMIIM_SP_26KEY_INDEX_U, { 3, 0x0075, 0x0173, 0x016B} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076}},
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077}},
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078}},
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079}},
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 0x0080, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0}},
};
#endif
#endif


// 马其顿
#ifdef IM_MACEDONIAN_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_macedonian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0410} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0412} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0406} },
    { MMIIM_SP_26KEY_INDEX_D, { 3, 0x0414, 0x0416, 0x0403} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0415} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0424} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0413} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0425} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0418} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0408} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x041A} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0x041B} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x041D, 0x041C} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0x0411} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x041E} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x041F} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x0409} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0420} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x0421} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x0422, 0x040C} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0423} },
    { MMIIM_SP_26KEY_INDEX_V, { 4, 0x0426, 0x0427, 0x0428, 0x040F} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x040A} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0417} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0405} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0401} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_macedonian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0430} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0432} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0456} },
    { MMIIM_SP_26KEY_INDEX_D, { 3, 0x0434, 0x0436, 0x0453} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0435} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0444} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0433} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0445} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0438} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0458} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x043A} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0x043B} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x043D, 0x043C} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0x0431} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x043E} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x043F} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x0459} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0440} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x0441} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x0442, 0x045C} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0443} },
    { MMIIM_SP_26KEY_INDEX_V, { 4, 0x0446, 0x0447, 0x0448, 0x045F} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x045A} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0437} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0455} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0451} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 摩尔多瓦语
#ifdef IM_MOLDOVAN_SUPPORT
#ifdef MMIIM_CSTAR_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_moldovan_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0102, 0X00C2} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0042} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X0043} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X0044} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0045} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0046} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0047} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0048} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X00CE} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X004A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X004B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X004C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X004D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X004E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0X004F} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0050} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0X0051} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0052} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X015E} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0162} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X0055} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0056} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X0057} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0058} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0059} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_moldovan_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 0X0103, 0X00E2} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0X0062} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0X0063} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0X0064} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0X0065} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0X0066} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0X0067} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0X0068} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0X00EE} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0X006A} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0X006B} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0X006C} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0X006D} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0X006E} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0X006F} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0X0070} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0X0071} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0X0072} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0X015F} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 0X0163} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0X0075} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0X0076} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0X0077} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0X0078} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0X0079} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0X007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_moldovan_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'A', 0x0102, 0x00C2} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'I', 0x00CE} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 'S', 0x015E} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 'T', 0x0162} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_moldovan_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'a', 0x0103, 0x00E2} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0x00EE} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 's', 0x015F} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 't', 0x0163} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 挪威语
#ifdef IM_NORWEGIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_norwegian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0041, 0x00C5, 0x00C6, 0x00C4, 0x00C0, 0x00C1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x0110} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x0045, 0x00C9, 0x00C8, 0x00CA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x014A} },
    {MMIIM_SP_26KEY_INDEX_O, {6, 0x004F, 0x00D8, 0x00D6, 0x00D3, 0x00D2, 0x00D4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0166} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DE} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_norwegian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0061, 0x00E5, 0x00E6, 0x00E4, 0x00E0, 0x00E1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x0111} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x0065, 0x00E9, 0x00E8, 0x00EA} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00EF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x014B} },
    {MMIIM_SP_26KEY_INDEX_O, {6, 0x006F, 0x00F8, 0x00F6, 0x00F3, 0x00F2, 0x00F4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0167} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FE} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_norwegian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C5, 0x00C6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D8} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
}; 
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_norwegian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E5, 0x00E6} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F8} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_norwegian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'A', 0x00C5, 0x00C6} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 'L', 0x00D8} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 4, 'P', 0x00A8, 0x005E, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_norwegian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'a', 0x00E5, 0x00E6} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 'l', 0x00F8} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'o'} },
    { MMIIM_SP_26KEY_INDEX_P, { 4, 'p', 0x00A8, 0x005E, 0x0060} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 波兰
#ifdef IM_POLISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_polish_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x0104} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x0106} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x0118} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x004C, 0x0141} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x0143} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x015A} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x005A, 0x0179, 0x017B} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_polish_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x0105} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x0107} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x0119} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x006C, 0x0142} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x0144} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x015B} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x007A, 0x017A, 0x017C} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_polish_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0041, 0x0104} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x0106} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x0118} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x004C, 0x0141} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x0143} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x015A} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x005A, 0x0179, 0x017B} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_polish_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0061, 0x0105} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x0107} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x0119} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x006C, 0x0142} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x0144} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F3} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x015B} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x007A, 0x017A, 0x017C} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_polish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 'L', 0x0141} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 'Q', 0x02DB, 0x00B7} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_polish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 2, 'a', 0x0105} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'c', 0x0107} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0x0119} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 'l', 0x0142} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'n', 0x0144} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0x00F3} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 3, 'q', 0x02DB, 0x00B7} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 's', 0x015B} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 3, 'z', 0x017C, 0x017A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 塞尔维亚语
#ifdef IM_SERBIAN_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_serbian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0410} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0411} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x0426, 0x0427} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0x0414, 0x0402} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x0415, 0x0416} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0424} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0413} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0425} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0418} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0408} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x041A} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0x041B} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0x041C} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0x041D} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x041E} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x041F} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0x0409, 0x0401} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0420} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x0421} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x0422, 0x040B} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0423} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x0412} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x040A} },
    { MMIIM_SP_26KEY_INDEX_X, { 2, 0x040F, 0x0428} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0417} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0405} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_serbian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0430} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0431} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 0x0446, 0x0447} },
    { MMIIM_SP_26KEY_INDEX_D, { 2, 0x0434, 0x0452} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 0x0435, 0x0436} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0444} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x0433} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0445} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0438} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x0458} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 0x043A} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 0x043B} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 0x043C} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 0x043D} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 0x043E} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 0x043F} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 0x0459, 0x0451} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x0440} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x0441} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x0442, 0x045B} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0443} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x0432} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x045A} },
    { MMIIM_SP_26KEY_INDEX_X, { 2, 0x045F, 0x0448} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x0437} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x0455} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif


// 斯洛伐克语
#ifdef IM_SLOVAK_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovak_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C1, 0x00C4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x010E} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {3, 0x004C, 0x0139, 0x013D} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x0147} },
    {MMIIM_SP_26KEY_INDEX_O, {5, 0x004F, 0x00D4, 0x00D3, 0x00D6, 0x0150} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0052, 0x0154} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0164} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0055, 0x00DA, 0x00DC, 0x0170} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovak_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E1, 0x00E4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x010F} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {3, 0x006C, 0x013A, 0x013E} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x0148} },
    {MMIIM_SP_26KEY_INDEX_O, {5, 0x006F, 0x00F4, 0x00F3, 0x00F6, 0x0151} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0072, 0x0155} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0165} },
    {MMIIM_SP_26KEY_INDEX_U, {4, 0x0075, 0x00FA, 0x00FC, 0x0171} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovak_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C1, 0x00C4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x010E} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CD} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {3, 0x004C, 0x0139, 0x013D} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x0147} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D3, 0x00D4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0052, 0x0154} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0164} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DA} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0059, 0x00DD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovak_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E1, 0x00E4} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x010F} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00ED} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {3, 0x006C, 0x013A, 0x013E} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x0148} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F3, 0x00F4} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0072, 0x0155} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0165} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FA} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0079, 0x00FD} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovak_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 1, 'O'} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'P', 0x00B4, 0x02C7} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 'Q', 0x00B0} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_slovak_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'a', 0x00E1, 0x00E4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 2, 'c', 0x010D} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0x00E9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 2, 'i', 0x00ED} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 'n', 0x0148} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0x00F4} },
    { MMIIM_SP_26KEY_INDEX_P, { 3, 'p', 0x00B4, 0x02C7} },
    { MMIIM_SP_26KEY_INDEX_Q, { 2, 'q', 0x00B0} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 's', 0x0161} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 't', 0x0165} },
    { MMIIM_SP_26KEY_INDEX_U, { 2, 'u', 0x00FA} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'y', 0x00FD} },
    { MMIIM_SP_26KEY_INDEX_Z, { 2, 'z', 0x017E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 瑞典语
#ifdef IM_SWEDISH_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_swedish_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0041, 0x00C5, 0x00C4, 0x00C0, 0x00C6, 0x00C1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0043, 0x010C} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0044, 0x0110} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0045, 0x00C9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0049, 0x00CF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x004E, 0x014A} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x004F, 0x00D6, 0x00D8} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0053, 0x0160} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0054, 0x0166} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0055, 0x00DC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x005A, 0x017D} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_swedish_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0061, 0x00E5, 0x00E4, 0x00E0, 0x00E6, 0x00E1} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0063, 0x010D} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0064, 0x0111} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0065, 0x00E9} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0069, 0x00EF} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x006E, 0x014B} },
    {MMIIM_SP_26KEY_INDEX_O, {3, 0x006F, 0x00F6, 0x00F8} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0073, 0x0161} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0074, 0x0167} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0075, 0x00FC} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x007A, 0x017E} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_swedish_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0041, 0x00C4, 0x00C5} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0042} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0043} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0044} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0045} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0046} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0047} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0048} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0049} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x004A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x004B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x004C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x004D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x004E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x004F, 0x00D6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0050} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0051} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0052} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0053} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0054} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0055} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0056} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0057} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0058} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0059} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x005A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_swedish_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {3, 0x0061, 0x00E4, 0x00E5} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0062} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0063} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0064} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0065} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0066} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0067} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0068} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0069} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x006A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x006B} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x006C} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x006D} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x006E} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x006F, 0x00F6} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0070} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0071} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0072} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0073} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0074} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0075} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0076} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0077} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0078} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0079} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x007A} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_swedish_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'A', 0x00C5, 0x00C4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'E'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0x00D6} },
    { MMIIM_SP_26KEY_INDEX_P, { 5, 'P', 0x00B4, 0x00A8, 0x0060, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 'S'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 'T'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'Y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_swedish_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 3, 'a', 0x00E5, 0x00E4} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 'e'} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0x00F6} },
    { MMIIM_SP_26KEY_INDEX_P, { 5, 'p', 0x00B4, 0x00A8, 0x0060, 0x005E} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 's'} },
    { MMIIM_SP_26KEY_INDEX_T, { 1, 't'} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 'y'} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 乌克兰语
#ifdef IM_UKRAINIAN_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_ukrainian_upper[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0424} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0418} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0421} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0412} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0423} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0410} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x041F} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0420} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0428} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x041E} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x041B, 0x0416} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0414, 0x0404} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x042C, 0x042E} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0422, 0x0411} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0429, 0x0425} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0417, 0x0407} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0419} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x041A} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0406} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0415} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0413, 0x0490} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x041C} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0426} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0427} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x041D} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x042F} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_ukrainian_lower[] =
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0444} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0438} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0441} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0432} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0443} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0430} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x043F} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0440} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0448} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x043E} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x043B, 0x0436} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0434, 0x0454} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x044C, 0x044E} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0442, 0x0431} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0449, 0x0445} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0437, 0x0457} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0439} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x043A} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0456} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0435} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0433, 0x0491} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x043C} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0446} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0447} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x043D} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x044F} },
    {MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_ukrainian_upper[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0410} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0411} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0426} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0414} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0415, 0x0404} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0424} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0413, 0x0490} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0427} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0418, 0x0419} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x042C} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x041A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x041B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x041C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x041D} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x041E} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x041F} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x042F} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0420} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0421} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0422} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0423, 0x042E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0412} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0428, 0x0429} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0425, 0x0416} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0406, 0x0407} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0417} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_ukrainian_lower[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0430} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0431} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0446} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0434} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0435, 0x0454} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0444} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0433, 0x0491} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0447} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0438, 0x0439} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x044C} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x043A} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x043B} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x043C} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x043D} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x043E} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x043F} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x044F} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0440} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0441} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0442} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0443, 0x044E} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0432} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0448, 0x0449} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0445, 0x0436} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0456, 0x0457} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0437} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};

#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_ukrainian_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0424} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0418} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0421} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0412} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0423} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0410} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x041F} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0420} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0428} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x041E} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x041B, 0x0416} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x0414, 0x0404} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x042C, 0x042E} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0422, 0x0411} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x0429, 0x0425} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0x0417, 0x0407} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x0419} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x041A} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x0406} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 0x0415, 0x0401} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0413} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x041C} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0426} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0427} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x041D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x042F} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_ukrainian_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 0x0444} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 0x0438} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 0x0441} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 0x0432} },
    { MMIIM_SP_26KEY_INDEX_E, { 1, 0x0443} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 0x0430} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 0x043F} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 0x0440} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 0x0448} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 0x043E} },
    { MMIIM_SP_26KEY_INDEX_K, { 2, 0x043B, 0x0436} },
    { MMIIM_SP_26KEY_INDEX_L, { 2, 0x0434, 0x0454} },
    { MMIIM_SP_26KEY_INDEX_M, { 2, 0x044C, 0x044E} },
    { MMIIM_SP_26KEY_INDEX_N, { 2, 0x0442, 0x0431} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 0x0449, 0x0445} },
    { MMIIM_SP_26KEY_INDEX_P, { 2, 0x0437, 0x0457} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 0x0439} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 0x043A} },
    { MMIIM_SP_26KEY_INDEX_S, { 1, 0x0456} },
    { MMIIM_SP_26KEY_INDEX_T, { 3, 0x0435, 0x0451, 0x2116} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 0x0433} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 0x043C} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 0x0446} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 0x0447} },
    { MMIIM_SP_26KEY_INDEX_Y, { 1, 0x043D} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 0x044F} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif
#endif


// 约鲁巴语
#ifdef IM_YORUBA_SUPPORT
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_yoruba_upper[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'A'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'B'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'C'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'D'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'E', 0X1EB8} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'F'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'G'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'H'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'I'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'J'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'K'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'L'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'M'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'N'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'O', 0X1ECC} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'P'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'Q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'R'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 'S', 0X1E62} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 'T', 0x0300} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'U'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'V'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'W'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'X'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'Y', 0x0301} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'Z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_yoruba_lower[] =
{
    { MMIIM_SP_26KEY_INDEX_A, { 1, 'a'} },
    { MMIIM_SP_26KEY_INDEX_B, { 1, 'b'} },
    { MMIIM_SP_26KEY_INDEX_C, { 1, 'c'} },
    { MMIIM_SP_26KEY_INDEX_D, { 1, 'd'} },
    { MMIIM_SP_26KEY_INDEX_E, { 2, 'e', 0X1EB9} },
    { MMIIM_SP_26KEY_INDEX_F, { 1, 'f'} },
    { MMIIM_SP_26KEY_INDEX_G, { 1, 'g'} },
    { MMIIM_SP_26KEY_INDEX_H, { 1, 'h'} },
    { MMIIM_SP_26KEY_INDEX_I, { 1, 'i'} },
    { MMIIM_SP_26KEY_INDEX_J, { 1, 'j'} },
    { MMIIM_SP_26KEY_INDEX_K, { 1, 'k'} },
    { MMIIM_SP_26KEY_INDEX_L, { 1, 'l'} },
    { MMIIM_SP_26KEY_INDEX_M, { 1, 'm'} },
    { MMIIM_SP_26KEY_INDEX_N, { 1, 'n'} },
    { MMIIM_SP_26KEY_INDEX_O, { 2, 'o', 0X1ECD} },
    { MMIIM_SP_26KEY_INDEX_P, { 1, 'p'} },
    { MMIIM_SP_26KEY_INDEX_Q, { 1, 'q'} },
    { MMIIM_SP_26KEY_INDEX_R, { 1, 'r'} },
    { MMIIM_SP_26KEY_INDEX_S, { 2, 's', 0X1E63} },
    { MMIIM_SP_26KEY_INDEX_T, { 2, 't', 0x0300} },
    { MMIIM_SP_26KEY_INDEX_U, { 1, 'u'} },
    { MMIIM_SP_26KEY_INDEX_V, { 1, 'v'} },
    { MMIIM_SP_26KEY_INDEX_W, { 1, 'w'} },
    { MMIIM_SP_26KEY_INDEX_X, { 1, 'x'} },
    { MMIIM_SP_26KEY_INDEX_Y, { 2, 'y', 0x0301} },
    { MMIIM_SP_26KEY_INDEX_Z, { 1, 'z'} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#endif

//阿姆哈拉
#ifdef IM_AMHARIC_SUPPORT
//#if defined(MMIIM_CSTAR_MULTITAP)     暂时仅仅cstar提供了数据表，故其他输入引擎暂且使用该数据
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_amharic[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {3, 0x1031, 0x1017, 0x102A} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x1018, 0x101A} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x1001, 0x1003} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x102D, 0x102E} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x1014, 0x1023, 0x1027} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0x103A, 0x1039, 0x1038} },
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x102B, 0x103D} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x1037, 0x1036} },
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x1004, 0x104D} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x103C, 0x1032} },
    { MMIIM_SP_26KEY_INDEX_K, {3, 0x102F, 0x1012, 0x100D} },
    { MMIIM_SP_26KEY_INDEX_L, {3, 0x1030, 0x1013, 0x100E} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x102C, 0x1026} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x100A, 0x1009} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x101E, 0x103F, 0x1029} },
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x1005, 0x100F} },
    { MMIIM_SP_26KEY_INDEX_Q, {3, 0x1006, 0x1008, 0x1002} },
    { MMIIM_SP_26KEY_INDEX_R, {3, 0x1019, 0x104E, 0x101B} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x103B, 0x103E} },
    { MMIIM_SP_26KEY_INDEX_T, {3, 0x1021, 0x1024, 0x100B} },
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x1000, 0x1025} },
    { MMIIM_SP_26KEY_INDEX_V, {3, 0x101C, 0x1020, 0x101F} },
    { MMIIM_SP_26KEY_INDEX_W, {3, 0x1010, 0x101D, 0x104F} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x1011, 0x100C} },
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x1015, 0x104C} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x1016, 0x1007} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

static const MMIIM_SP_26KEY_KEYCHAR_T s_28key_table_amharic[] =
{
    { MMIIM_SP_KEY_IRREGULAR_1, {7, 0x1200, 0x1201, 0x1202, 0x1203, 0x1204	, 0x1205	, 0x1206} },
    { MMIIM_SP_KEY_IRREGULAR_2, {8, 0x1208, 0x1209, 0x120A, 0x120B, 0x120C	, 0x120D	, 0x120E	, 0x120F} },
    { MMIIM_SP_KEY_IRREGULAR_3, {8, 0x1218, 0x1219, 0x121A, 0x121B, 0x121C	, 0x121D	, 0x121E	, 0x121F} },
    { MMIIM_SP_KEY_IRREGULAR_4, {8, 0x1228, 0x1229, 0x122A, 0x122B, 0x122C	, 0x122D	, 0x122E	, 0x122F} },
    { MMIIM_SP_KEY_IRREGULAR_5, {8, 0x1230, 0x1231, 0x1232, 0x1233, 0x1234	, 0x1235	, 0x1236	, 0x1237} },
    { MMIIM_SP_KEY_IRREGULAR_6, {8, 0x1238, 0x1239, 0x123A, 0x123B, 0x123C	, 0x123D	, 0x123E	, 0x123F} },
    { MMIIM_SP_KEY_IRREGULAR_7, {8, 0x1240, 0x1241, 0x1242, 0x1243, 0x1244	, 0x1245	, 0x1246	, 0x124B} },
    { MMIIM_SP_KEY_IRREGULAR_8, {8, 0x1260, 0x1261, 0x1262, 0x1263, 0x1264	, 0x1265	, 0x1266	, 0x1267} },
    { MMIIM_SP_KEY_IRREGULAR_9, {8, 0x1270, 0x1271, 0x1272, 0x1273, 0x1274	, 0x1275	, 0x1276	, 0x1277} },
    { MMIIM_SP_KEY_IRREGULAR_10, {8, 0x1278, 0x1279	, 0x127A	, 0x127B	, 0x127C	, 0x127D	, 0x127E	, 0x127F} },
    { MMIIM_SP_KEY_IRREGULAR_11, {8, 0x1290, 0x1291	, 0x1292	, 0x1293	, 0x1294	, 0x1295	, 0x1296	, 0x1297} },
    { MMIIM_SP_KEY_IRREGULAR_12, {8, 0x1298, 0x1299	, 0x129A	, 0x129B	, 0x129C	, 0x129D	, 0x129E	, 0x129F} },
    { MMIIM_SP_KEY_IRREGULAR_13, {7, 0x12A0, 0x12A1	, 0x12A2	, 0x12A3	, 0x12A4	, 0x12A5	, 0x12A6} },
    { MMIIM_SP_KEY_IRREGULAR_14, {7, 0x12A8, 0x12A9	, 0x12AA	, 0x12AB	, 0x12AC	, 0x12AD	, 0x12AE} },
    { MMIIM_SP_KEY_IRREGULAR_15, {7, 0x12C8, 0x12C9	, 0x12CA	, 0x12CB	, 0x12CC	, 0x12CD	, 0x12CE} },
    { MMIIM_SP_KEY_IRREGULAR_16, {8, 0x12D8, 0x12D9	, 0x12DA	, 0x12DB	, 0x12DC	, 0x12DD	, 0x12DE	, 0x12DF} },
    { MMIIM_SP_KEY_IRREGULAR_17, {8, 0x12E0, 0x12E1	, 0x12E2	, 0x12E3	, 0x12E4	, 0x12E5	, 0x12E6	, 0x12E7} },
    { MMIIM_SP_KEY_IRREGULAR_18, {7, 0x12E8, 0x12E9	, 0x12EA	, 0x12EB	, 0x12EC	, 0x12ED	, 0x12EE} },
    { MMIIM_SP_KEY_IRREGULAR_19, {8, 0x12F0, 0x12F1	, 0x12F2	, 0x12F3	, 0x12F4	, 0x12F5	, 0x12F6	, 0x12F7} },
    { MMIIM_SP_KEY_IRREGULAR_20, {7, 0x1300, 0x1301	, 0x1302	, 0x1303	, 0x1304	, 0x1305	, 0x1306} },
    { MMIIM_SP_KEY_IRREGULAR_21, {7, 0x1308, 0x1309	, 0x130A	, 0x130B	, 0x130C	, 0x130D	, 0x130E} },
    { MMIIM_SP_KEY_IRREGULAR_22, {8, 0x1320, 0x1321	, 0x1322	, 0x1323	, 0x1324	, 0x1325	, 0x1326	, 0x1327} },
    { MMIIM_SP_KEY_IRREGULAR_23, {8, 0x1328, 0x1329	, 0x132A	, 0x132B	, 0x132C	, 0x132D	, 0x132E	, 0x132F} },
    { MMIIM_SP_KEY_IRREGULAR_24, {7, 0x1330, 0x1331	, 0x1332	, 0x1333	, 0x1334	, 0x1335	, 0x1336} },
    { MMIIM_SP_KEY_IRREGULAR_25, {8, 0x1340, 0x1341	, 0x1342	, 0x1343	, 0x1344	, 0x1345	, 0x1346	, 0x133F} },
    { MMIIM_SP_KEY_IRREGULAR_26, {8, 0x1348, 0x1349	, 0x134A	, 0x134B	, 0x134C	, 0x134D	, 0x134E	, 0x134F} },
    { MMIIM_SP_KEY_IRREGULAR_27, {7, 0x1350, 0x1351	, 0x1352	, 0x1353	, 0x1354	, 0x1355	, 0x1356} },
    { MMIIM_SP_KEY_IRREGULAR_28, {7, 0x1268, 0x1269	, 0x126A	, 0x126B	, 0x126C	, 0x126D	, 0x126E} },
        
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};

//#endif//CSTAR
#endif//IM_AMHARIC_SUPPORT

//柬埔寨
#ifdef IM_KHMER_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)     
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_khmer[] =
{
    { MMIIM_SP_26KEY_INDEX_A, {2, 0x17B1, 0x17B6} },
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x1796, 0x1794} },
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x1787, 0x1785} },
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x178C, 0x178A} },
    { MMIIM_SP_26KEY_INDEX_E, {3, 0x17C2, 0x17C1, 0x17AF} },
    { MMIIM_SP_26KEY_INDEX_F, {3, 0x1792, 0x1790, 0x17CC} },
    { MMIIM_SP_26KEY_INDEX_G, {3, 0x17A2, 0x1784, 0x17CD} },
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x17C7, 0x17A0} },
    { MMIIM_SP_26KEY_INDEX_I, {3, 0x17B8, 0x17B7, 0x17A6} },
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x1789, 0x17D2} },
    { MMIIM_SP_26KEY_INDEX_K, {3, 0x1782, 0x1780, 0x17CE} },
    { MMIIM_SP_26KEY_INDEX_L, {3, 0x17A1, 0x179B, 0x17CF} },
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x17C6, 0x1798} },
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x178E, 0x1793} },
    { MMIIM_SP_26KEY_INDEX_O, {3, 0x17C5, 0x17C4, 0x17B3} },
    { MMIIM_SP_26KEY_INDEX_P, {3, 0x1797, 0x1795, 0x17B0} },
    { MMIIM_SP_26KEY_INDEX_Q, {4, 0x1788, 0x1786, 0x17BE, 0x17A5} },
    { MMIIM_SP_26KEY_INDEX_R, {3, 0x17AB, 0x179A, 0x17AC} },
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x17C3, 0x179F} },
    { MMIIM_SP_26KEY_INDEX_T, {3, 0x178F, 0x17BF, 0x17C0} },
    { MMIIM_SP_26KEY_INDEX_U, {3, 0x17BC, 0x17BB, 0x17A7} },
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x17CA, 0x179C} },
    { MMIIM_SP_26KEY_INDEX_W, {4, 0x17BA, 0x17B9, 0x17AD, 0x17AE} },
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x1783, 0x1781} },
    { MMIIM_SP_26KEY_INDEX_Y, {4, 0x17BD, 0x1799, 0x17A9, 0x17AA} },
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x178D, 0x178B} },
    { MMIIM_SP_26KEY_INDEX_PERIOD, {4, 0x17D4, 0x17D5, 0x17D6, 0x17D7} },    
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_khmer[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {4, 0x17A7, 0x17A9, 0x17AA, 0x17B6} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x1794, 0x1796} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x1785, 0x1787} },
    {MMIIM_SP_26KEY_INDEX_D, {3, 0x178A, 0x178C, 0x17CC} },
    {MMIIM_SP_26KEY_INDEX_E, {4, 0x17AF, 0x17C1, 0x17C2, 0x17C0} },
    {MMIIM_SP_26KEY_INDEX_F, {3, 0x1790, 0x1792, 0x17CD} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x1784, 0x17A2, 0x17CF} },
    {MMIIM_SP_26KEY_INDEX_H, {3, 0x17A0, 0x17C7, 0x17D0} },
    {MMIIM_SP_26KEY_INDEX_I, {4, 0x17B7, 0x17B8, 0x17A5, 0x17A6} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x1789, 0x17CE, 0x17D2} },
    {MMIIM_SP_26KEY_INDEX_K, {3, 0x1780, 0x1782, 0x17D4} },
    {MMIIM_SP_26KEY_INDEX_L, {3, 0x179B, 0x17A1, 0x17D5} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x1798, 0x17C6} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x1793, 0x178E} },
    {MMIIM_SP_26KEY_INDEX_O, {5, 0x17C4, 0x17C5, 0x17B1, 0x17B2, 0x17B3} },
    {MMIIM_SP_26KEY_INDEX_P, {4, 0x1795, 0x1797, 0x17AD, 0x17AE} },
    {MMIIM_SP_26KEY_INDEX_Q, {3, 0x1786, 0x1788, 0x17C8} },
    {MMIIM_SP_26KEY_INDEX_R, {3, 0x179A, 0x17AB, 0x17AC} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x179F, 0x17B0, 0x17C3} },
    {MMIIM_SP_26KEY_INDEX_T, {3, 0x178F, 0x1791, 0x17CA} },
    {MMIIM_SP_26KEY_INDEX_U, {3, 0x17BB, 0x17BC, 0x17CB} },
    {MMIIM_SP_26KEY_INDEX_V, {3, 0x179C, 0x17D6, 0x17D7} },
    {MMIIM_SP_26KEY_INDEX_W, {3, 0x17B9, 0x17BA, 0x17C9} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x1783, 0x1781} },
    {MMIIM_SP_26KEY_INDEX_Y, {4, 0x1799, 0x17BD, 0x17BE, 0x17BF} },
    {MMIIM_SP_26KEY_INDEX_Z, {2, 0x178B, 0x178D} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#endif//CSTAR
#endif//IM_KHMER_SUPPORT

//老挝
#ifdef IM_LAO_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lao[] =    
{
    { MMIIM_SP_26KEY_INDEX_A, {1, 0x0EB1}},        
    { MMIIM_SP_26KEY_INDEX_B, {2, 0x0EB8, 0x0EB6}},
    { MMIIM_SP_26KEY_INDEX_C, {2, 0x0EC2, 0x0EC1}},
    { MMIIM_SP_26KEY_INDEX_D, {2, 0x0E82, 0x0E81}},
    { MMIIM_SP_26KEY_INDEX_E, {1, 0x0EB3}},        
    { MMIIM_SP_26KEY_INDEX_F, {2, 0x0E95, 0x0E94}},
    { MMIIM_SP_26KEY_INDEX_G, {2, 0x0E87, 0x0EC0}},
    { MMIIM_SP_26KEY_INDEX_H, {2, 0x0ECA, 0x0EC9}},
    { MMIIM_SP_26KEY_INDEX_I, {2, 0x0EA3, 0x0EAE}},
    { MMIIM_SP_26KEY_INDEX_J, {2, 0x0ECB, 0x0EC8}},
    { MMIIM_SP_26KEY_INDEX_K, {2, 0x0E9A, 0x0EB2}},
    { MMIIM_SP_26KEY_INDEX_L, {2, 0x0EA5, 0x0EAA}},
    { MMIIM_SP_26KEY_INDEX_M, {2, 0x0EC6, 0x0E97}},
    { MMIIM_SP_26KEY_INDEX_N, {2, 0x0EB9, 0x0EB7}},
    { MMIIM_SP_26KEY_INDEX_O, {1, 0x0E99}},        
    { MMIIM_SP_26KEY_INDEX_P, {2, 0x0EBD, 0x0E8D}},
    { MMIIM_SP_26KEY_INDEX_Q, {2, 0x0EBC, 0x0EBB}},
    { MMIIM_SP_26KEY_INDEX_R, {2, 0x0E9D, 0x0E9E}},
    { MMIIM_SP_26KEY_INDEX_S, {2, 0x0E84, 0x0EAB}},
    { MMIIM_SP_26KEY_INDEX_T, {2, 0x0E88, 0x0EB0}},
    { MMIIM_SP_26KEY_INDEX_U, {2, 0x0ECD, 0x0EB5}},
    { MMIIM_SP_26KEY_INDEX_V, {2, 0x0EA7, 0x0EAD}},
    { MMIIM_SP_26KEY_INDEX_W, {2, 0x0EC3, 0x0EC4}},
    { MMIIM_SP_26KEY_INDEX_X, {2, 0x0EA2, 0x0E9B}},
    { MMIIM_SP_26KEY_INDEX_Y, {2, 0x0ECC, 0x0EB4}},
    { MMIIM_SP_26KEY_INDEX_Z, {2, 0x0E96, 0x0E9C}},
    { MMIIM_SP_26KEY_INDEX_PERIOD, {2, 0x0E8A, 0x0E9D}},    
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lao[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0EB0, 0x0EB1} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0E9A, 0x0E9B} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0E88, 0x0E81} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0E94, 0x0E95} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0EC0, 0x0EC1} },
    {MMIIM_SP_26KEY_INDEX_F, {3, 0x0E9F, 0x0E9D, 0x0EC8} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x0EC3, 0x0EC4, 0x0EC9} },
    {MMIIM_SP_26KEY_INDEX_H, {3, 0x0EAB, 0x0EAE, 0x0ECA} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0EB4, 0x0EB5} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x0EB2, 0x0EB3, 0x0ECB} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0E82, 0x0E84} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0EBC, 0x0EBD} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x0EA1, 0x0EA2} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0E87, 0x0E8D} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0EAD, 0x0EC2} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0E9C, 0x0E9E} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0EAF, 0x0EC6} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0EA3, 0x0EA5} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0EAA, 0x0E8A} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0E96, 0x0E97} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0EB8, 0x0EB9} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0EDC, 0x0EDD} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0EA7, 0x0E99} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0ECC, 0x0ECD} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0EB6, 0x0EB7} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0EBB} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else   //防止编译不过，使用sogou数据表示
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_lao[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0EB0, 0x0EB1} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0E9A, 0x0E9B} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0E88, 0x0E81} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x0E94, 0x0E95} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0EC0, 0x0EC1} },
    {MMIIM_SP_26KEY_INDEX_F, {3, 0x0E9F, 0x0E9D, 0x0EC8} },
    {MMIIM_SP_26KEY_INDEX_G, {3, 0x0EC3, 0x0EC4, 0x0EC9} },
    {MMIIM_SP_26KEY_INDEX_H, {3, 0x0EAB, 0x0EAE, 0x0ECA} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0EB4, 0x0EB5} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x0EB2, 0x0EB3, 0x0ECB} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0E82, 0x0E84} },
    {MMIIM_SP_26KEY_INDEX_L, {2, 0x0EBC, 0x0EBD} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x0EA1, 0x0EA2} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x0E87, 0x0E8D} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0EAD, 0x0EC2} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0E9C, 0x0E9E} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x0EAF, 0x0EC6} },
    {MMIIM_SP_26KEY_INDEX_R, {2, 0x0EA3, 0x0EA5} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0EAA, 0x0E8A} },
    {MMIIM_SP_26KEY_INDEX_T, {2, 0x0E96, 0x0E97} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0EB8, 0x0EB9} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0EDC, 0x0EDD} },
    {MMIIM_SP_26KEY_INDEX_W, {2, 0x0EA7, 0x0E99} },
    {MMIIM_SP_26KEY_INDEX_X, {2, 0x0ECC, 0x0ECD} },
    {MMIIM_SP_26KEY_INDEX_Y, {2, 0x0EB6, 0x0EB7} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0EBB} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#endif//
#endif//IM_LAO_SUPPORT

//维吾尔语
#ifdef IM_UYGHUR_SUPPORT
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_uyghur[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0634, 0x0633} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x06C6, 0x06C7} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x06BE} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x064A, 0x0649} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0698, 0x0EC1} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0628} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0644} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0627} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x062E} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x062A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0646} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0645} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0643} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x06C8, 0x06CB} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x06AF} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x062C} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x067E} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0648} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0641} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x06AD} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0632, 0x0631} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0686} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x062F} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x063A} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x0626, 0x06D0, 0x06D5} },
    {MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x06D0} },    

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#elif defined MMIIM_SOGOU_MULTITAP
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_uyghur[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0633, 0x0634} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0698} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x06Be} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x067e} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x064A, 0x06d0} },
    {MMIIM_SP_26KEY_INDEX_F, {2, 0x0628, 0x062a} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0644} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0627} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x062e} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0686} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0646} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0645} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x0643, 0x06ad} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0649} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x06af} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x062c} },
    {MMIIM_SP_26KEY_INDEX_Q, {2, 0x06c6, 0x06c7} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642} },
    {MMIIM_SP_26KEY_INDEX_S, {2, 0x0648, 0x06c8} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0641} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x06cb} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0631, 0x0632} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x06d5} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x062f} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x063a} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0626} },
};
#else
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_uyghur[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {2, 0x0634, 0x0633} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x06C6, 0x06C7} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x06BE} },
    {MMIIM_SP_26KEY_INDEX_D, {2, 0x064A, 0x0649} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0698, 0x0EC1} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0628} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0644} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0627} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x062E} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x062A} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0646} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0645} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0643} },
    {MMIIM_SP_26KEY_INDEX_N, {2, 0x06C8, 0x06CB} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x06AF} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x062C} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x067E} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0642} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0648} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0641} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x06AD} },
    {MMIIM_SP_26KEY_INDEX_V, {2, 0x0632, 0x0631} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0686} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x062F} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x063A} },
    {MMIIM_SP_26KEY_INDEX_Z, {3, 0x0626, 0x06D0, 0x06D5} },
    {MMIIM_SP_26KEY_INDEX_PERIOD, {1, 0x06D0} },    

    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#endif
#endif//IM_UYGHUR_SUPPORT

#if defined(IM_TIBETAN_SUPPORT) 
#if defined(MMIIM_CSTAR_MULTITAP)
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tibetan[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0f60} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0F56} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0F45} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0F51} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0F7A} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0F44} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0F42} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0F67} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0F72} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0F47} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0F40} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0F63} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0F58} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0F53} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x0F7C} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0F54} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0F84} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0F6A} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0F66} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0F4F} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0F74} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0F59} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0F49} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0F5B} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0F61} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0F60} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tibetan_shift_page[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0F68} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0F56} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0F46} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0F4C} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0F7B} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0F4B} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0F4A} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0F7F} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0F80} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0F47} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0F41} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0FB3} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0F7E} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0F4E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x0F7D} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0F55} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0F84} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0F82} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0F64} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0F5D} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0FAD} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0F5A} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0F5D} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0F65} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0FB1} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0F5E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#else	//使用cstar代替
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tibetan[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0f60} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0F56} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0F45} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0F51} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0F7A} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0F44} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0F42} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0F67} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0F72} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0F47} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0F40} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0F63} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0F58} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0F53} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x0F7C} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0F54} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0F84} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0F6A} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0F66} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0F4F} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0F74} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0F59} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0F49} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0F5B} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0F61} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0F60} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_tibetan_shift_page[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {1, 0x0F68} },
    {MMIIM_SP_26KEY_INDEX_B, {1, 0x0F56} },
    {MMIIM_SP_26KEY_INDEX_C, {1, 0x0F46} },
    {MMIIM_SP_26KEY_INDEX_D, {1, 0x0F4C} },
    {MMIIM_SP_26KEY_INDEX_E, {1, 0x0F7B} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0F4B} },
    {MMIIM_SP_26KEY_INDEX_G, {1, 0x0F4A} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0F7F} },
    {MMIIM_SP_26KEY_INDEX_I, {1, 0x0F80} },
    {MMIIM_SP_26KEY_INDEX_J, {1, 0x0F47} },
    {MMIIM_SP_26KEY_INDEX_K, {1, 0x0F41} },
    {MMIIM_SP_26KEY_INDEX_L, {1, 0x0FB3} },
    {MMIIM_SP_26KEY_INDEX_M, {1, 0x0F7E} },
    {MMIIM_SP_26KEY_INDEX_N, {1, 0x0F4E} },
    {MMIIM_SP_26KEY_INDEX_O, {1, 0x0F7D} },
    {MMIIM_SP_26KEY_INDEX_P, {1, 0x0F55} },
    {MMIIM_SP_26KEY_INDEX_Q, {1, 0x0F84} },
    {MMIIM_SP_26KEY_INDEX_R, {1, 0x0F82} },
    {MMIIM_SP_26KEY_INDEX_S, {1, 0x0F64} },
    {MMIIM_SP_26KEY_INDEX_T, {1, 0x0F5D} },
    {MMIIM_SP_26KEY_INDEX_U, {1, 0x0FAD} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0F5A} },
    {MMIIM_SP_26KEY_INDEX_W, {1, 0x0F5D} },
    {MMIIM_SP_26KEY_INDEX_X, {1, 0x0F65} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0FB1} },
    {MMIIM_SP_26KEY_INDEX_Z, {1, 0x0F5E} },
    { MMIIM_SP_26KEY_INDEX_MAX, {0} },    
};
#endif
#endif

//僧伽罗
#ifdef IM_SINHALESE_SUPPORT	
#if defined MMIIM_SOGOU_MULTITAP  //sogou版本
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_sinhalese[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0D85, 0x0D86, 0x0D87, 0x0D88, 0x0D93, 0x0D96} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0DB6, 0x0DB7} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0DA0, 0x0DA1} },
    {MMIIM_SP_26KEY_INDEX_D, {4, 0x0DA9, 0x0DAA, 0x0DAF, 0x0DB0} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0D91, 0x0D92} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0DC6} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0D9C, 0x0D9D} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0DC4} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0D89, 0x0D8A} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x0DA2, 0x0DA3, 0x0DA5} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0D9A, 0x0D9B} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0D8F, 0x0D90, 0x0DBD, 0x0DC5} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x0DB8, 0x0DB9} },
    {MMIIM_SP_26KEY_INDEX_N, {8, 0x0D9E, 0x0D9F, 0x0DA4, 0x0DA6, 0x0DAB, 0x0DAC, 0x0DB1, 0x0DB3} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0D94, 0x0D95} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0DB4, 0x0DB5} },
    {MMIIM_SP_26KEY_INDEX_Q, {6, 0x0D82, 0x0D83, 0x0DCA, 0x0DCF, 0x0DD0, 0x0DD1} },
    {MMIIM_SP_26KEY_INDEX_R, {3, 0x0D8D, 0x0D8E, 0x0DBB} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x0DC1, 0x0DC2, 0x0DC3} },
    {MMIIM_SP_26KEY_INDEX_T, {4, 0x0DA7, 0x0DA8, 0x0DAD, 0x0DAE} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0D8B, 0x0D8C} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0DC0} },
    {MMIIM_SP_26KEY_INDEX_W, {5, 0x0DD2, 0x0DD3, 0x0DD4, 0x0DD6, 0x0DD8} },
    {MMIIM_SP_26KEY_INDEX_X, {6, 0x0DD9, 0x0DDA, 0x0DDB, 0x0DDC, 0x0DDD, 0x0DDE} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0DBA} },
    {MMIIM_SP_26KEY_INDEX_Z, {4, 0x0DDF, 0x0DF2, 0x0DF3, 0x0DF4} },
};
#else	//用sogou版本代替
static const MMIIM_SP_26KEY_KEYCHAR_T s_26key_table_sinhalese[] =    
{
    {MMIIM_SP_26KEY_INDEX_A, {6, 0x0D85, 0x0D86, 0x0D87, 0x0D88, 0x0D93, 0x0D96} },
    {MMIIM_SP_26KEY_INDEX_B, {2, 0x0DB6, 0x0DB7} },
    {MMIIM_SP_26KEY_INDEX_C, {2, 0x0DA0, 0x0DA1} },
    {MMIIM_SP_26KEY_INDEX_D, {4, 0x0DA9, 0x0DAA, 0x0DAF, 0x0DB0} },
    {MMIIM_SP_26KEY_INDEX_E, {2, 0x0D91, 0x0D92} },
    {MMIIM_SP_26KEY_INDEX_F, {1, 0x0DC6} },
    {MMIIM_SP_26KEY_INDEX_G, {2, 0x0D9C, 0x0D9D} },
    {MMIIM_SP_26KEY_INDEX_H, {1, 0x0DC4} },
    {MMIIM_SP_26KEY_INDEX_I, {2, 0x0D89, 0x0D8A} },
    {MMIIM_SP_26KEY_INDEX_J, {3, 0x0DA2, 0x0DA3, 0x0DA5} },
    {MMIIM_SP_26KEY_INDEX_K, {2, 0x0D9A, 0x0D9B} },
    {MMIIM_SP_26KEY_INDEX_L, {4, 0x0D8F, 0x0D90, 0x0DBD, 0x0DC5} },
    {MMIIM_SP_26KEY_INDEX_M, {2, 0x0DB8, 0x0DB9} },
    {MMIIM_SP_26KEY_INDEX_N, {8, 0x0D9E, 0x0D9F, 0x0DA4, 0x0DA6, 0x0DAB, 0x0DAC, 0x0DB1, 0x0DB3} },
    {MMIIM_SP_26KEY_INDEX_O, {2, 0x0D94, 0x0D95} },
    {MMIIM_SP_26KEY_INDEX_P, {2, 0x0DB4, 0x0DB5} },
    {MMIIM_SP_26KEY_INDEX_Q, {6, 0x0D82, 0x0D83, 0x0DCA, 0x0DCF, 0x0DD0, 0x0DD1} },
    {MMIIM_SP_26KEY_INDEX_R, {3, 0x0D8D, 0x0D8E, 0x0DBB} },
    {MMIIM_SP_26KEY_INDEX_S, {3, 0x0DC1, 0x0DC2, 0x0DC3} },
    {MMIIM_SP_26KEY_INDEX_T, {4, 0x0DA7, 0x0DA8, 0x0DAD, 0x0DAE} },
    {MMIIM_SP_26KEY_INDEX_U, {2, 0x0D8B, 0x0D8C} },
    {MMIIM_SP_26KEY_INDEX_V, {1, 0x0DC0} },
    {MMIIM_SP_26KEY_INDEX_W, {5, 0x0DD2, 0x0DD3, 0x0DD4, 0x0DD6, 0x0DD8} },
    {MMIIM_SP_26KEY_INDEX_X, {6, 0x0DD9, 0x0DDA, 0x0DDB, 0x0DDC, 0x0DDD, 0x0DDE} },
    {MMIIM_SP_26KEY_INDEX_Y, {1, 0x0DBA} },
    {MMIIM_SP_26KEY_INDEX_Z, {4, 0x0DDF, 0x0DF2, 0x0DF3, 0x0DF4} },
};
#endif
#endif//IM_SINHALESE_SUPPORT


// 输入法信息
static MMIIM_SP_ML_26KEY_TABLE_CFG_T s_spim_26key_lang_data[] =
{
    {GUIIM_M_Q_123,                     s_26key_table_123},

    {GUIIM_M_TPQ_123,                   s_26key_table_123},

// 英语
    {GUIIM_M_Q_ENGLISH_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_ENGLISH_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_ENGLISH_ABC_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_ENGLISH_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_ENGLISH_ABC_UPPER,      s_26key_table_abc_upper},

// 南非的公用荷兰语
#ifdef IM_AFRIKAANS_SUPPORT
    {GUIIM_M_Q_AFRIKAANS_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_AFRIKAANS_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_AFRIKAANS_ABC_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_AFRIKAANS_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_AFRIKAANS_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_AFRIKAANS_ABC_UPPER,      s_26key_table_abc_upper},
#endif

// 阿尔巴尼亚语
#ifdef IM_ALBANIAN_SUPPORT
    {GUIIM_M_Q_ALBANIAN_ABC_LEAD_UPPER, s_26key_table_albanian_upper},
    {GUIIM_M_Q_ALBANIAN_ABC_LOWER,      s_26key_table_albanian_lower},
    {GUIIM_M_Q_ALBANIAN_ABC_UPPER,      s_26key_table_albanian_upper},

    {GUIIM_M_TPQ_ALBANIAN_ABC_LEAD_UPPER, s_26key_table_albanian_upper},
    {GUIIM_M_TPQ_ALBANIAN_ABC_LOWER,      s_26key_table_albanian_lower},
    {GUIIM_M_TPQ_ALBANIAN_ABC_UPPER,      s_26key_table_albanian_upper},
#endif

// 阿拉伯语
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_Q_ARABIC_ABC, s_26key_table_arabic},

    {GUIIM_M_TPQ_ARABIC_ABC, s_26key_table_arabic},
#endif

// 亚美尼亚语
#ifdef IM_ARMENIAN_SUPPORT
    {GUIIM_M_Q_ARMENIAN_ABC_LEAD_UPPER, s_26key_table_armenian_upper},
    {GUIIM_M_Q_ARMENIAN_ABC_LOWER,      s_26key_table_armenian_lower},
    {GUIIM_M_Q_ARMENIAN_ABC_UPPER,      s_26key_table_armenian_upper},

    {GUIIM_M_TPQ_ARMENIAN_ABC_LEAD_UPPER, s_26key_table_armenian_upper},
    {GUIIM_M_TPQ_ARMENIAN_ABC_LOWER,      s_26key_table_armenian_lower},
    {GUIIM_M_TPQ_ARMENIAN_ABC_UPPER,      s_26key_table_armenian_upper},
#endif

// 阿萨姆
#ifdef IM_ASSAMESE_SUPPORT
    {GUIIM_M_Q_ASSAMESE_ABC, s_26key_table_assamese},

    {GUIIM_M_TPQ_ASSAMESE_ABC, s_26key_table_assamese},
#endif

// 阿塞拜疆语
#ifdef IM_AZERBAIJANI_SUPPORT
    {GUIIM_M_Q_AZERBAIJANI_ABC_LEAD_UPPER, s_26key_table_azerbaijani_upper},
    {GUIIM_M_Q_AZERBAIJANI_ABC_LOWER,      s_26key_table_azerbaijani_lower},
    {GUIIM_M_Q_AZERBAIJANI_ABC_UPPER,      s_26key_table_azerbaijani_upper},

    {GUIIM_M_TPQ_AZERBAIJANI_ABC_LEAD_UPPER, s_26key_table_azerbaijani_upper},
    {GUIIM_M_TPQ_AZERBAIJANI_ABC_LOWER,      s_26key_table_azerbaijani_lower},
    {GUIIM_M_TPQ_AZERBAIJANI_ABC_UPPER,      s_26key_table_azerbaijani_upper},
#endif

// 巴斯克语
#ifdef IM_BASQUE_SUPPORT
    {GUIIM_M_Q_BASQUE_ABC_LEAD_UPPER, s_26key_table_basque_upper},
    {GUIIM_M_Q_BASQUE_ABC_LOWER,      s_26key_table_basque_lower},
    {GUIIM_M_Q_BASQUE_ABC_UPPER,      s_26key_table_basque_upper},

    {GUIIM_M_TPQ_BASQUE_ABC_LEAD_UPPER, s_26key_table_basque_upper},
    {GUIIM_M_TPQ_BASQUE_ABC_LOWER,      s_26key_table_basque_lower},
    {GUIIM_M_TPQ_BASQUE_ABC_UPPER,      s_26key_table_basque_upper},
#endif

// 孟加拉语
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_Q_BENGALI_ABC, s_26key_table_bengali},

    {GUIIM_M_TPQ_BENGALI_ABC, s_26key_table_bengali},
#endif

// 保加利亚语
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_Q_BULGARIAN_ABC_LEAD_UPPER, s_26key_table_bulgarian_upper},
    {GUIIM_M_Q_BULGARIAN_ABC_LOWER,      s_26key_table_bulgarian_lower},
    {GUIIM_M_Q_BULGARIAN_ABC_UPPER,      s_26key_table_bulgarian_upper},

    {GUIIM_M_TPQ_BULGARIAN_ABC_LEAD_UPPER, s_26key_table_bulgarian_upper},
    {GUIIM_M_TPQ_BULGARIAN_ABC_LOWER,      s_26key_table_bulgarian_lower},
    {GUIIM_M_TPQ_BULGARIAN_ABC_UPPER,      s_26key_table_bulgarian_upper},
#endif

// 加泰罗尼亚语
#ifdef IM_CATALAN_SUPPORT
    {GUIIM_M_Q_CATALAN_ABC_LEAD_UPPER, s_26key_table_catalan_upper},
    {GUIIM_M_Q_CATALAN_ABC_LOWER,      s_26key_table_catalan_lower},
    {GUIIM_M_Q_CATALAN_ABC_UPPER,      s_26key_table_catalan_upper},

    {GUIIM_M_TPQ_CATALAN_ABC_LEAD_UPPER, s_26key_table_catalan_upper},
    {GUIIM_M_TPQ_CATALAN_ABC_LOWER,      s_26key_table_catalan_lower},
    {GUIIM_M_TPQ_CATALAN_ABC_UPPER,      s_26key_table_catalan_upper},
#endif

// 克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_Q_CROATIAN_ABC_LEAD_UPPER, s_26key_table_croatian_upper},
    {GUIIM_M_Q_CROATIAN_ABC_LOWER,      s_26key_table_croatian_lower},
    {GUIIM_M_Q_CROATIAN_ABC_UPPER,      s_26key_table_croatian_upper},

    {GUIIM_M_TPQ_CROATIAN_ABC_LEAD_UPPER, s_26key_table_croatian_upper},
    {GUIIM_M_TPQ_CROATIAN_ABC_LOWER,      s_26key_table_croatian_lower},
    {GUIIM_M_TPQ_CROATIAN_ABC_UPPER,      s_26key_table_croatian_upper},
#endif

// 捷克语
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_Q_CZECH_ABC_LEAD_UPPER, s_26key_table_czech_upper},
    {GUIIM_M_Q_CZECH_ABC_LOWER,      s_26key_table_czech_lower},
    {GUIIM_M_Q_CZECH_ABC_UPPER,      s_26key_table_czech_upper},

    {GUIIM_M_TPQ_CZECH_ABC_LEAD_UPPER, s_26key_table_czech_upper},
    {GUIIM_M_TPQ_CZECH_ABC_LOWER,      s_26key_table_czech_lower},
    {GUIIM_M_TPQ_CZECH_ABC_UPPER,      s_26key_table_czech_upper},
#endif

// 丹麦语
#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_Q_DANISH_ABC_LEAD_UPPER, s_26key_table_danish_upper},
    {GUIIM_M_Q_DANISH_ABC_LOWER,      s_26key_table_danish_lower},
    {GUIIM_M_Q_DANISH_ABC_UPPER,      s_26key_table_danish_upper},

    {GUIIM_M_TPQ_DANISH_ABC_LEAD_UPPER, s_26key_table_danish_upper},
    {GUIIM_M_TPQ_DANISH_ABC_LOWER,      s_26key_table_danish_lower},
    {GUIIM_M_TPQ_DANISH_ABC_UPPER,      s_26key_table_danish_upper},
#endif

// 荷兰语
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_Q_DUTCH_ABC_LEAD_UPPER, s_26key_table_dutch_upper},
    {GUIIM_M_Q_DUTCH_ABC_LOWER,      s_26key_table_dutch_lower},
    {GUIIM_M_Q_DUTCH_ABC_UPPER,      s_26key_table_dutch_upper},

    {GUIIM_M_TPQ_DUTCH_ABC_LEAD_UPPER, s_26key_table_dutch_upper},
    {GUIIM_M_TPQ_DUTCH_ABC_LOWER,      s_26key_table_dutch_lower},
    {GUIIM_M_TPQ_DUTCH_ABC_UPPER,      s_26key_table_dutch_upper},
#endif

// 爱沙尼亚语
#ifdef IM_ESTONIAN_SUPPORT
    {GUIIM_M_Q_ESTONIAN_ABC_LEAD_UPPER, s_26key_table_estonian_upper},
    {GUIIM_M_Q_ESTONIAN_ABC_LOWER,      s_26key_table_estonian_lower},
    {GUIIM_M_Q_ESTONIAN_ABC_UPPER,      s_26key_table_estonian_upper},

    {GUIIM_M_TPQ_ESTONIAN_ABC_LEAD_UPPER, s_26key_table_estonian_upper},
    {GUIIM_M_TPQ_ESTONIAN_ABC_LOWER,      s_26key_table_estonian_lower},
    {GUIIM_M_TPQ_ESTONIAN_ABC_UPPER,      s_26key_table_estonian_upper},
#endif

// 菲律宾语
#ifdef IM_FILIPINO_SUPPORT
    {GUIIM_M_Q_FILIPINO_ABC_LEAD_UPPER, s_26key_table_filipino_upper},
    {GUIIM_M_Q_FILIPINO_ABC_LOWER,      s_26key_table_filipino_lower},
    {GUIIM_M_Q_FILIPINO_ABC_UPPER,      s_26key_table_filipino_upper},

    {GUIIM_M_TPQ_FILIPINO_ABC_LEAD_UPPER, s_26key_table_filipino_upper},
    {GUIIM_M_TPQ_FILIPINO_ABC_LOWER,      s_26key_table_filipino_lower},
    {GUIIM_M_TPQ_FILIPINO_ABC_UPPER,      s_26key_table_filipino_upper},
#endif

// 芬兰语
#ifdef IM_FINNISH_SUPPORT
    {GUIIM_M_Q_FINNISH_ABC_LEAD_UPPER, s_26key_table_finnish_upper},
    {GUIIM_M_Q_FINNISH_ABC_LOWER,      s_26key_table_finnish_lower},
    {GUIIM_M_Q_FINNISH_ABC_UPPER,      s_26key_table_finnish_upper},

    {GUIIM_M_TPQ_FINNISH_ABC_LEAD_UPPER, s_26key_table_finnish_upper},
    {GUIIM_M_TPQ_FINNISH_ABC_LOWER,      s_26key_table_finnish_lower},
    {GUIIM_M_TPQ_FINNISH_ABC_UPPER,      s_26key_table_finnish_upper},
#endif

// 法语
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_Q_FRENCH_ABC_LEAD_UPPER, s_26key_table_french_upper},
    {GUIIM_M_Q_FRENCH_ABC_LOWER,      s_26key_table_french_lower},
    {GUIIM_M_Q_FRENCH_ABC_UPPER,      s_26key_table_french_upper},

    {GUIIM_M_TPQ_FRENCH_ABC_LEAD_UPPER, s_26key_table_french_upper},
    {GUIIM_M_TPQ_FRENCH_ABC_LOWER,      s_26key_table_french_lower},
    {GUIIM_M_TPQ_FRENCH_ABC_UPPER,      s_26key_table_french_upper},
#endif

// 加利西亚语
#ifdef IM_GALICIAN_SUPPORT
    {GUIIM_M_Q_GALICIAN_ABC_LEAD_UPPER, s_26key_table_galician_upper},
    {GUIIM_M_Q_GALICIAN_ABC_LOWER,      s_26key_table_galician_lower},
    {GUIIM_M_Q_GALICIAN_ABC_UPPER,      s_26key_table_galician_upper},

    {GUIIM_M_TPQ_GALICIAN_ABC_LEAD_UPPER, s_26key_table_galician_upper},
    {GUIIM_M_TPQ_GALICIAN_ABC_LOWER,      s_26key_table_galician_lower},
    {GUIIM_M_TPQ_GALICIAN_ABC_UPPER,      s_26key_table_galician_upper},
#endif

// 格鲁吉亚语
#ifdef IM_GEORGIAN_SUPPORT
    {GUIIM_M_Q_GEORGIAN_ABC, s_26key_table_georgian},

    {GUIIM_M_TPQ_GEORGIAN_ABC, s_26key_table_georgian},
#endif

// 德语
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_Q_GERMAN_ABC_LEAD_UPPER, s_26key_table_german_upper},
    {GUIIM_M_Q_GERMAN_ABC_LOWER,      s_26key_table_german_lower},
    {GUIIM_M_Q_GERMAN_ABC_UPPER,      s_26key_table_german_upper},

    {GUIIM_M_TPQ_GERMAN_ABC_LEAD_UPPER, s_26key_table_german_upper},
    {GUIIM_M_TPQ_GERMAN_ABC_LOWER,      s_26key_table_german_lower},
    {GUIIM_M_TPQ_GERMAN_ABC_UPPER,      s_26key_table_german_upper},
#endif

// 希腊语
#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_Q_GREEK_ABC_LEAD_UPPER, s_26key_table_greek_upper},
    {GUIIM_M_Q_GREEK_ABC_LOWER,      s_26key_table_greek_lower},
    {GUIIM_M_Q_GREEK_ABC_UPPER,      s_26key_table_greek_upper},

    {GUIIM_M_TPQ_GREEK_ABC_LEAD_UPPER, s_26key_table_greek_upper},
    {GUIIM_M_TPQ_GREEK_ABC_LOWER,      s_26key_table_greek_lower},
    {GUIIM_M_TPQ_GREEK_ABC_UPPER,      s_26key_table_greek_upper},
#endif

// 古吉拉特语（印度的一种语言）
#ifdef IM_GUJARATI_SUPPORT
    {GUIIM_M_Q_GUJARATI_ABC, s_26key_table_gujarati},

    {GUIIM_M_TPQ_GUJARATI_ABC, s_26key_table_gujarati},
#endif

// 豪撒语
#ifdef IM_HAUSA_SUPPORT
    {GUIIM_M_Q_HAUSA_ABC_LEAD_UPPER, s_26key_table_hausa_upper},
    {GUIIM_M_Q_HAUSA_ABC_LOWER,      s_26key_table_hausa_lower},
    {GUIIM_M_Q_HAUSA_ABC_UPPER,      s_26key_table_hausa_upper},

    {GUIIM_M_TPQ_HAUSA_ABC_LEAD_UPPER, s_26key_table_hausa_upper},
    {GUIIM_M_TPQ_HAUSA_ABC_LOWER,      s_26key_table_hausa_lower},
    {GUIIM_M_TPQ_HAUSA_ABC_UPPER,      s_26key_table_hausa_upper},
#endif

// 希伯莱语
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_Q_HEBREW_ABC, s_26key_table_hebrew},

    {GUIIM_M_TPQ_HEBREW_ABC, s_26key_table_hebrew},
#endif

// 印地语
#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_Q_HINDI_ABC,               s_26key_table_hindi},

    {GUIIM_M_TPQ_HINDI_ABC,             s_26key_table_hindi},
#endif

// 匈牙利语
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_Q_HUNGARIAN_ABC_LEAD_UPPER, s_26key_table_hungarian_upper},
    {GUIIM_M_Q_HUNGARIAN_ABC_LOWER,      s_26key_table_hungarian_lower},
    {GUIIM_M_Q_HUNGARIAN_ABC_UPPER,      s_26key_table_hungarian_upper},

    {GUIIM_M_TPQ_HUNGARIAN_ABC_LEAD_UPPER, s_26key_table_hungarian_upper},
    {GUIIM_M_TPQ_HUNGARIAN_ABC_LOWER,      s_26key_table_hungarian_lower},
    {GUIIM_M_TPQ_HUNGARIAN_ABC_UPPER,      s_26key_table_hungarian_upper},
#endif

// 冰岛语
#ifdef IM_ICELANDIC_SUPPORT
    {GUIIM_M_Q_ICELANDIC_ABC_LEAD_UPPER, s_26key_table_icelandic_upper},
    {GUIIM_M_Q_ICELANDIC_ABC_LOWER,      s_26key_table_icelandic_lower},
    {GUIIM_M_Q_ICELANDIC_ABC_UPPER,      s_26key_table_icelandic_upper},

    {GUIIM_M_TPQ_ICELANDIC_ABC_LEAD_UPPER, s_26key_table_icelandic_upper},
    {GUIIM_M_TPQ_ICELANDIC_ABC_LOWER,      s_26key_table_icelandic_lower},
    {GUIIM_M_TPQ_ICELANDIC_ABC_UPPER,      s_26key_table_icelandic_upper},
#endif

// 伊博语
#ifdef IM_IGBO_SUPPORT
    {GUIIM_M_Q_IGBO_ABC_LEAD_UPPER, s_26key_table_igbo_upper},
    {GUIIM_M_Q_IGBO_ABC_LOWER,      s_26key_table_igbo_lower},
    {GUIIM_M_Q_IGBO_ABC_UPPER,      s_26key_table_igbo_upper},

    {GUIIM_M_TPQ_IGBO_ABC_LEAD_UPPER, s_26key_table_igbo_upper},
    {GUIIM_M_TPQ_IGBO_ABC_LOWER,      s_26key_table_igbo_lower},
    {GUIIM_M_TPQ_IGBO_ABC_UPPER,      s_26key_table_igbo_upper},
#endif

// 印度尼西亚语
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_Q_INDONESIAN_ABC_LEAD_UPPER,   s_26key_table_abc_upper},
    {GUIIM_M_Q_INDONESIAN_ABC_LOWER,        s_26key_table_abc_lower},
    {GUIIM_M_Q_INDONESIAN_ABC_UPPER,        s_26key_table_abc_upper},

    {GUIIM_M_TPQ_INDONESIAN_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_INDONESIAN_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_INDONESIAN_ABC_UPPER,      s_26key_table_abc_upper},
#endif

// 爱尔兰语
#ifdef IM_IRISH_SUPPORT
    {GUIIM_M_Q_IRISH_ABC_LEAD_UPPER, s_26key_table_irish_upper},
    {GUIIM_M_Q_IRISH_ABC_LOWER,      s_26key_table_irish_lower},
    {GUIIM_M_Q_IRISH_ABC_UPPER,      s_26key_table_irish_upper},

    {GUIIM_M_TPQ_IRISH_ABC_LEAD_UPPER, s_26key_table_irish_upper},
    {GUIIM_M_TPQ_IRISH_ABC_LOWER,      s_26key_table_irish_lower},
    {GUIIM_M_TPQ_IRISH_ABC_UPPER,      s_26key_table_irish_upper},
#endif

// 意大利语
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_Q_ITALIAN_ABC_LEAD_UPPER, s_26key_table_italian_upper},
    {GUIIM_M_Q_ITALIAN_ABC_LOWER,      s_26key_table_italian_lower},
    {GUIIM_M_Q_ITALIAN_ABC_UPPER,      s_26key_table_italian_upper},

    {GUIIM_M_TPQ_ITALIAN_ABC_LEAD_UPPER, s_26key_table_italian_upper},
    {GUIIM_M_TPQ_ITALIAN_ABC_LOWER,      s_26key_table_italian_lower},
    {GUIIM_M_TPQ_ITALIAN_ABC_UPPER,      s_26key_table_italian_upper},
#endif

// 卡纳达语（印度南部的一种方言）
#ifdef IM_KANNADA_SUPPORT
    {GUIIM_M_Q_KANNADA_ABC, s_26key_table_kannada},

    {GUIIM_M_TPQ_KANNADA_ABC, s_26key_table_kannada},
#endif

// 哈萨克斯坦
#ifdef IM_KAZAKH_SUPPORT
    {GUIIM_M_Q_KAZAKH_ABC_LEAD_UPPER, s_26key_table_kazakh_upper},
    {GUIIM_M_Q_KAZAKH_ABC_LOWER,      s_26key_table_kazakh_lower},
    {GUIIM_M_Q_KAZAKH_ABC_UPPER,      s_26key_table_kazakh_upper},

    {GUIIM_M_TPQ_KAZAKH_ABC_LEAD_UPPER, s_26key_table_kazakh_upper},
    {GUIIM_M_TPQ_KAZAKH_ABC_LOWER,      s_26key_table_kazakh_lower},
    {GUIIM_M_TPQ_KAZAKH_ABC_UPPER,      s_26key_table_kazakh_upper},
#endif

// 拉脱维亚语
#ifdef IM_LATVIAN_SUPPORT
    {GUIIM_M_Q_LATVIAN_ABC_LEAD_UPPER, s_26key_table_latvian_upper},
    {GUIIM_M_Q_LATVIAN_ABC_LOWER,      s_26key_table_latvian_lower},
    {GUIIM_M_Q_LATVIAN_ABC_UPPER,      s_26key_table_latvian_upper},

    {GUIIM_M_TPQ_LATVIAN_ABC_LEAD_UPPER, s_26key_table_latvian_upper},
    {GUIIM_M_TPQ_LATVIAN_ABC_LOWER,      s_26key_table_latvian_lower},
    {GUIIM_M_TPQ_LATVIAN_ABC_UPPER,      s_26key_table_latvian_upper},
#endif

// 立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_Q_LITHUANIAN_ABC_LEAD_UPPER, s_26key_table_lithuanian_upper},
    {GUIIM_M_Q_LITHUANIAN_ABC_LOWER,      s_26key_table_lithuanian_lower},
    {GUIIM_M_Q_LITHUANIAN_ABC_UPPER,      s_26key_table_lithuanian_upper},

    {GUIIM_M_TPQ_LITHUANIAN_ABC_LEAD_UPPER, s_26key_table_lithuanian_upper},
    {GUIIM_M_TPQ_LITHUANIAN_ABC_LOWER,      s_26key_table_lithuanian_lower},
    {GUIIM_M_TPQ_LITHUANIAN_ABC_UPPER,      s_26key_table_lithuanian_upper},
#endif

// 马其顿
#ifdef IM_MACEDONIAN_SUPPORT
    {GUIIM_M_Q_MACEDONIAN_ABC_LEAD_UPPER, s_26key_table_macedonian_upper},
    {GUIIM_M_Q_MACEDONIAN_ABC_LOWER,      s_26key_table_macedonian_lower},
    {GUIIM_M_Q_MACEDONIAN_ABC_UPPER,      s_26key_table_macedonian_upper},

    {GUIIM_M_TPQ_MACEDONIAN_ABC_LEAD_UPPER, s_26key_table_macedonian_upper},
    {GUIIM_M_TPQ_MACEDONIAN_ABC_LOWER,      s_26key_table_macedonian_lower},
    {GUIIM_M_TPQ_MACEDONIAN_ABC_UPPER,      s_26key_table_macedonian_upper},
#endif

// 马拉雅拉姆语（印度的一种语言）
#ifdef IM_MALAYALAM_SUPPORT
    {GUIIM_M_Q_MALAYALAM_ABC,           s_26key_table_malayalam},

    {GUIIM_M_TPQ_MALAYALAM_ABC,         s_26key_table_malayalam},
#endif

// 缅甸
#ifdef IM_MYANMAR_SUPPORT
    {GUIIM_M_Q_MYANMAR_ABC,           s_26key_table_myanmar},

    {GUIIM_M_TPQ_MYANMAR_ABC,         s_26key_table_myanmar},
#endif

// 马来语
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_Q_MALAY_ABC_LEAD_UPPER,    s_26key_table_abc_upper},
    {GUIIM_M_Q_MALAY_ABC_LOWER,         s_26key_table_abc_lower},
    {GUIIM_M_Q_MALAY_ABC_UPPER,         s_26key_table_abc_upper},

    {GUIIM_M_TPQ_MALAY_ABC_LEAD_UPPER,  s_26key_table_abc_upper},
    {GUIIM_M_TPQ_MALAY_ABC_LOWER,       s_26key_table_abc_lower},
    {GUIIM_M_TPQ_MALAY_ABC_UPPER,       s_26key_table_abc_upper},
#endif

// 马拉地语 
#ifdef IM_MARATHI_SUPPORT
    {GUIIM_M_Q_MARATHI_ABC,             s_26key_table_marathi},

    {GUIIM_M_TPQ_MARATHI_ABC,           s_26key_table_marathi},
#endif

// 摩尔多瓦语
#ifdef IM_MOLDOVAN_SUPPORT
    {GUIIM_M_Q_MOLDOVAN_ABC_LEAD_UPPER, s_26key_table_moldovan_upper},
    {GUIIM_M_Q_MOLDOVAN_ABC_LOWER,      s_26key_table_moldovan_lower},
    {GUIIM_M_Q_MOLDOVAN_ABC_UPPER,      s_26key_table_moldovan_upper},

    {GUIIM_M_TPQ_MOLDOVAN_ABC_LEAD_UPPER, s_26key_table_moldovan_upper},
    {GUIIM_M_TPQ_MOLDOVAN_ABC_LOWER,      s_26key_table_moldovan_lower},
    {GUIIM_M_TPQ_MOLDOVAN_ABC_UPPER,      s_26key_table_moldovan_upper},
#endif

// 挪威语
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_Q_NORWEGIAN_ABC_LEAD_UPPER, s_26key_table_norwegian_upper},
    {GUIIM_M_Q_NORWEGIAN_ABC_LOWER,      s_26key_table_norwegian_lower},
    {GUIIM_M_Q_NORWEGIAN_ABC_UPPER,      s_26key_table_norwegian_upper},

    {GUIIM_M_TPQ_NORWEGIAN_ABC_LEAD_UPPER, s_26key_table_norwegian_upper},
    {GUIIM_M_TPQ_NORWEGIAN_ABC_LOWER,      s_26key_table_norwegian_lower},
    {GUIIM_M_TPQ_NORWEGIAN_ABC_UPPER,      s_26key_table_norwegian_upper},
#endif

// 奥利亚语
#ifdef IM_ORIYA_SUPPORT
    {GUIIM_M_Q_ORIYA_ABC, s_26key_table_oriya},

    {GUIIM_M_TPQ_ORIYA_ABC, s_26key_table_oriya},
#endif

// 波斯语
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_Q_PERSIAN_ABC, s_26key_table_persian},

    {GUIIM_M_TPQ_PERSIAN_ABC, s_26key_table_persian},
#endif

// 波兰
#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_Q_POLISH_ABC_LEAD_UPPER, s_26key_table_polish_upper},
    {GUIIM_M_Q_POLISH_ABC_LOWER,      s_26key_table_polish_lower},
    {GUIIM_M_Q_POLISH_ABC_UPPER,      s_26key_table_polish_upper},

    {GUIIM_M_TPQ_POLISH_ABC_LEAD_UPPER, s_26key_table_polish_upper},
    {GUIIM_M_TPQ_POLISH_ABC_LOWER,      s_26key_table_polish_lower},
    {GUIIM_M_TPQ_POLISH_ABC_UPPER,      s_26key_table_polish_upper},
#endif

// 葡萄牙语
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_Q_PORTUGUESE_ABC_LEAD_UPPER, s_26key_table_portuguese_upper},
    {GUIIM_M_Q_PORTUGUESE_ABC_LOWER,      s_26key_table_portuguese_lower},
    {GUIIM_M_Q_PORTUGUESE_ABC_UPPER,      s_26key_table_portuguese_upper},

    {GUIIM_M_TPQ_PORTUGUESE_ABC_LEAD_UPPER, s_26key_table_portuguese_upper},
    {GUIIM_M_TPQ_PORTUGUESE_ABC_LOWER,      s_26key_table_portuguese_lower},
    {GUIIM_M_TPQ_PORTUGUESE_ABC_UPPER,      s_26key_table_portuguese_upper},
#endif

// 印度旁遮普语
#ifdef IM_PUNJABI_SUPPORT
    {GUIIM_M_Q_PUNJABI_ABC, s_26key_table_punjabi},

    {GUIIM_M_TPQ_PUNJABI_ABC, s_26key_table_punjabi},
#endif

// 罗马尼亚语
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_Q_ROMANIAN_ABC_LEAD_UPPER, s_26key_table_romanian_upper},
    {GUIIM_M_Q_ROMANIAN_ABC_LOWER,      s_26key_table_romanian_lower},
    {GUIIM_M_Q_ROMANIAN_ABC_UPPER,      s_26key_table_romanian_upper},

    {GUIIM_M_TPQ_ROMANIAN_ABC_LEAD_UPPER, s_26key_table_romanian_upper},
    {GUIIM_M_TPQ_ROMANIAN_ABC_LOWER,      s_26key_table_romanian_lower},
    {GUIIM_M_TPQ_ROMANIAN_ABC_UPPER,      s_26key_table_romanian_upper},
#endif

// 俄语
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_Q_RUSSIAN_ABC_LEAD_UPPER, s_26key_table_russian_upper},
    {GUIIM_M_Q_RUSSIAN_ABC_LOWER,      s_26key_table_russian_lower},
    {GUIIM_M_Q_RUSSIAN_ABC_UPPER,      s_26key_table_russian_upper},

    {GUIIM_M_TPQ_RUSSIAN_ABC_LEAD_UPPER, s_26key_table_russian_upper},
    {GUIIM_M_TPQ_RUSSIAN_ABC_LOWER,      s_26key_table_russian_lower},
    {GUIIM_M_TPQ_RUSSIAN_ABC_UPPER,      s_26key_table_russian_upper},
#endif

// 塞尔维亚语
#ifdef IM_SERBIAN_SUPPORT
    {GUIIM_M_Q_SERBIAN_ABC_LEAD_UPPER, s_26key_table_serbian_upper},
    {GUIIM_M_Q_SERBIAN_ABC_LOWER,      s_26key_table_serbian_lower},
    {GUIIM_M_Q_SERBIAN_ABC_UPPER,      s_26key_table_serbian_upper},

    {GUIIM_M_TPQ_SERBIAN_ABC_LEAD_UPPER, s_26key_table_serbian_upper},
    {GUIIM_M_TPQ_SERBIAN_ABC_LOWER,      s_26key_table_serbian_lower},
    {GUIIM_M_TPQ_SERBIAN_ABC_UPPER,      s_26key_table_serbian_upper},
#endif

// 塞索托语
#ifdef IM_SESOTHO_SUPPORT
    {GUIIM_M_Q_SESOTHO_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_SESOTHO_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_SESOTHO_ABC_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_SESOTHO_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_SESOTHO_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_SESOTHO_ABC_UPPER,      s_26key_table_abc_upper},
#endif

// 斯洛伐克语
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_Q_SLOVAK_ABC_LEAD_UPPER, s_26key_table_slovak_upper},
    {GUIIM_M_Q_SLOVAK_ABC_LOWER,      s_26key_table_slovak_lower},
    {GUIIM_M_Q_SLOVAK_ABC_UPPER,      s_26key_table_slovak_upper},

    {GUIIM_M_TPQ_SLOVAK_ABC_LEAD_UPPER, s_26key_table_slovak_upper},
    {GUIIM_M_TPQ_SLOVAK_ABC_LOWER,      s_26key_table_slovak_lower},
    {GUIIM_M_TPQ_SLOVAK_ABC_UPPER,      s_26key_table_slovak_upper},
#endif

// 斯洛文尼亚语
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_Q_SLOVENIAN_ABC_LEAD_UPPER, s_26key_table_slovenian_upper},
    {GUIIM_M_Q_SLOVENIAN_ABC_LOWER,      s_26key_table_slovenian_lower},
    {GUIIM_M_Q_SLOVENIAN_ABC_UPPER,      s_26key_table_slovenian_upper},

    {GUIIM_M_TPQ_SLOVENIAN_ABC_LEAD_UPPER, s_26key_table_slovenian_upper},
    {GUIIM_M_TPQ_SLOVENIAN_ABC_LOWER,      s_26key_table_slovenian_lower},
    {GUIIM_M_TPQ_SLOVENIAN_ABC_UPPER,      s_26key_table_slovenian_upper},
#endif

// 西班牙语
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_Q_SPANISH_ABC_LEAD_UPPER, s_26key_table_spanish_upper},
    {GUIIM_M_Q_SPANISH_ABC_LOWER,      s_26key_table_spanish_lower},
    {GUIIM_M_Q_SPANISH_ABC_UPPER,      s_26key_table_spanish_upper},

    {GUIIM_M_TPQ_SPANISH_ABC_LEAD_UPPER, s_26key_table_spanish_upper},
    {GUIIM_M_TPQ_SPANISH_ABC_LOWER,      s_26key_table_spanish_lower},
    {GUIIM_M_TPQ_SPANISH_ABC_UPPER,      s_26key_table_spanish_upper},
#endif

// 斯瓦西里语
#ifdef IM_SWAHILI_SUPPORT       //cstar, sogou等均使用abc数据表
    {GUIIM_M_Q_SWAHILI_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_SWAHILI_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_SWAHILI_ABC_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_SWAHILI_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_SWAHILI_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_SWAHILI_ABC_UPPER,      s_26key_table_abc_upper},
#endif

// 瑞典语
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_Q_SWEDISH_ABC_LEAD_UPPER, s_26key_table_swedish_upper},
    {GUIIM_M_Q_SWEDISH_ABC_LOWER,      s_26key_table_swedish_lower},
    {GUIIM_M_Q_SWEDISH_ABC_UPPER,      s_26key_table_swedish_upper},

    {GUIIM_M_TPQ_SWEDISH_ABC_LEAD_UPPER, s_26key_table_swedish_upper},
    {GUIIM_M_TPQ_SWEDISH_ABC_LOWER,      s_26key_table_swedish_lower},
    {GUIIM_M_TPQ_SWEDISH_ABC_UPPER,      s_26key_table_swedish_upper},
#endif

// 他加禄语
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_Q_TAGALOG_ABC, s_26key_table_tagalog},

    {GUIIM_M_TPQ_TAGALOG_ABC, s_26key_table_tagalog},
#endif

// 泰米尔语
#ifdef IM_TAMIL_SUPPORT
    {GUIIM_M_Q_TAMIL_ABC, s_26key_table_tamil},

    {GUIIM_M_TPQ_TAMIL_ABC, s_26key_table_tamil},
#endif

// 泰卢固语
#ifdef IM_TELUGU_SUPPORT
    {GUIIM_M_Q_TELUGU_ABC, s_26key_table_telugu},

    {GUIIM_M_TPQ_TELUGU_ABC, s_26key_table_telugu},
#endif

// 泰语
#ifdef IM_THAI_SUPPORT
    {GUIIM_M_Q_THAI_ABC, s_26key_table_thai},

    {GUIIM_M_TPQ_THAI_ABC, s_26key_table_thai},
#endif

// 土耳其语
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_Q_TURKISH_ABC_LEAD_UPPER, s_26key_table_turkish_upper},
    {GUIIM_M_Q_TURKISH_ABC_LOWER,      s_26key_table_turkish_lower},
    {GUIIM_M_Q_TURKISH_ABC_UPPER,      s_26key_table_turkish_upper},

    {GUIIM_M_TPQ_TURKISH_ABC_LEAD_UPPER, s_26key_table_turkish_upper},
    {GUIIM_M_TPQ_TURKISH_ABC_LOWER,      s_26key_table_turkish_lower},
    {GUIIM_M_TPQ_TURKISH_ABC_UPPER,      s_26key_table_turkish_upper},
#endif

// 乌克兰语
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_Q_UKRAINIAN_ABC_LEAD_UPPER, s_26key_table_ukrainian_upper},
    {GUIIM_M_Q_UKRAINIAN_ABC_LOWER,      s_26key_table_ukrainian_lower},
    {GUIIM_M_Q_UKRAINIAN_ABC_UPPER,      s_26key_table_ukrainian_upper},

    {GUIIM_M_TPQ_UKRAINIAN_ABC_LEAD_UPPER, s_26key_table_ukrainian_upper},
    {GUIIM_M_TPQ_UKRAINIAN_ABC_LOWER,      s_26key_table_ukrainian_lower},
    {GUIIM_M_TPQ_UKRAINIAN_ABC_UPPER,      s_26key_table_ukrainian_upper},
#endif

// 乌尔都语
#ifdef IM_URDU_SUPPORT
    {GUIIM_M_Q_URDU_ABC, s_26key_table_urdu},

    {GUIIM_M_TPQ_URDU_ABC, s_26key_table_urdu},
#endif

// 越南语
#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_Q_VIETNAMESE_ABC_LEAD_UPPER, s_26key_table_vietnamese_upper},
    {GUIIM_M_Q_VIETNAMESE_ABC_LOWER,      s_26key_table_vietnamese_lower},
    {GUIIM_M_Q_VIETNAMESE_ABC_UPPER,      s_26key_table_vietnamese_upper},

    {GUIIM_M_TPQ_VIETNAMESE_ABC_LEAD_UPPER, s_26key_table_vietnamese_upper},
    {GUIIM_M_TPQ_VIETNAMESE_ABC_LOWER,      s_26key_table_vietnamese_lower},
    {GUIIM_M_TPQ_VIETNAMESE_ABC_UPPER,      s_26key_table_vietnamese_upper},
#endif

// 科萨语（南非的一种方言）
#ifdef IM_XHOSA_SUPPORT
    {GUIIM_M_Q_XHOSA_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_XHOSA_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_XHOSA_ABC_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_XHOSA_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_XHOSA_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_XHOSA_ABC_UPPER,      s_26key_table_abc_upper},
#endif

// 约鲁巴语
#ifdef IM_YORUBA_SUPPORT
    {GUIIM_M_Q_YORUBA_ABC_LEAD_UPPER, s_26key_table_yoruba_upper},
    {GUIIM_M_Q_YORUBA_ABC_LOWER,      s_26key_table_yoruba_lower},
    {GUIIM_M_Q_YORUBA_ABC_UPPER,      s_26key_table_yoruba_upper},

    {GUIIM_M_TPQ_YORUBA_ABC_LEAD_UPPER, s_26key_table_yoruba_upper},
    {GUIIM_M_TPQ_YORUBA_ABC_LOWER,      s_26key_table_yoruba_lower},
    {GUIIM_M_TPQ_YORUBA_ABC_UPPER,      s_26key_table_yoruba_upper},
#endif

// 祖鲁语（非洲的语种）
#ifdef IM_ZULU_SUPPORT
    {GUIIM_M_Q_ZULU_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_ZULU_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_ZULU_ABC_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_ZULU_ABC_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_ZULU_ABC_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_ZULU_ABC_UPPER,      s_26key_table_abc_upper},
#endif

#ifdef IM_AMHARIC_SUPPORT
#if defined MMI_IM_PDA_SUPPORT
    {GUIIM_M_Q_AMHARIC_ABC, s_28key_table_amharic},
    {GUIIM_M_TPQ_AMHARIC_ABC, s_28key_table_amharic},
#else
    {GUIIM_M_Q_AMHARIC_ABC, s_26key_table_amharic},
    {GUIIM_M_TPQ_AMHARIC_ABC, s_26key_table_amharic},
#endif
#endif

// 柬埔寨
#ifdef IM_KHMER_SUPPORT
    {GUIIM_M_Q_KHMER_ABC, s_26key_table_khmer},
    {GUIIM_M_TPQ_KHMER_ABC,      s_26key_table_khmer},
#endif

// 老挝
#ifdef IM_LAO_SUPPORT
    {GUIIM_M_Q_LAO_ABC, s_26key_table_lao},
    {GUIIM_M_TPQ_LAO_ABC,      s_26key_table_lao},
#endif

#ifdef IM_UYGHUR_SUPPORT
    {GUIIM_M_Q_UYGHUR_ABC, s_26key_table_uyghur},
    {GUIIM_M_TPQ_UYGHUR_ABC,      s_26key_table_uyghur},
#endif

#ifdef IM_TIBETAN_SUPPORT
    {GUIIM_M_Q_TIBETAN_ABC, s_26key_table_tibetan},
    {GUIIM_M_TPQ_TIBETAN_ABC,      s_26key_table_tibetan},
#endif

#ifdef IM_SINHALESE_SUPPORT
    {GUIIM_M_Q_SINHALESE_ABC, s_26key_table_sinhalese},
    {GUIIM_M_TPQ_SINHALESE_ABC,      s_26key_table_sinhalese},
#endif
};

// 输入法信息，smart输入虚拟键盘需要的按键字符显示对应数据，为了程序的可维护性，
//不采用与表s_spim_26key_lang_data共用的方式
#if 1   //def MMIIM_VIRTUAL_KB_TEXT_SUPPORT     //强制打开，部分程序宏未完全控制，不打开，将导致虚拟键盘背景图为空白
static MMIIM_SP_ML_26KEY_TABLE_CFG_T s_spim_smart_26key_lang_data[] =
{
    {GUIIM_M_Q_123,                     s_26key_table_123},

    {GUIIM_M_TPQ_123,                   s_26key_table_123},

// 英语
    {GUIIM_M_Q_ENGLISH_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_ENGLISH_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_ENGLISH_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_ENGLISH_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_ENGLISH_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_ENGLISH_UPPER,      s_26key_table_abc_upper},

     {GUIIM_M_TPQ_PINYIN,                s_26key_table_abc_upper},

//繁体中文丝印数据
#ifdef IM_TRAD_CHINESE_SUPPORT
#ifdef IM_TRAD_CHINESE_QWERT_40KEY
    {GUIIM_M_TPQ_ZHUYIN, s_40key_table_trad_chinese_zhuyin},	
#else
    {GUIIM_M_TPQ_ZHUYIN, s_26key_table_trad_chinese_zhuyin},
#endif    
#endif
     
// 南非的公用荷兰语
#ifdef IM_AFRIKAANS_SUPPORT
    {GUIIM_M_Q_AFRIKAANS_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_AFRIKAANS_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_AFRIKAANS_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_AFRIKAANS_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_AFRIKAANS_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_AFRIKAANS_UPPER,      s_26key_table_abc_upper},
#endif

// 阿尔巴尼亚语
#ifdef IM_ALBANIAN_SUPPORT
    {GUIIM_M_Q_ALBANIAN_LEAD_UPPER, s_26key_table_albanian_upper},
    {GUIIM_M_Q_ALBANIAN_LOWER,      s_26key_table_albanian_lower},
    {GUIIM_M_Q_ALBANIAN_UPPER,      s_26key_table_albanian_upper},

    {GUIIM_M_TPQ_ALBANIAN_LEAD_UPPER, s_26key_table_albanian_upper},
    {GUIIM_M_TPQ_ALBANIAN_LOWER,      s_26key_table_albanian_lower},
    {GUIIM_M_TPQ_ALBANIAN_UPPER,      s_26key_table_albanian_upper},
#endif

// 阿拉伯语
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_Q_ARABIC, s_26key_table_arabic},

    {GUIIM_M_TPQ_ARABIC, s_26key_table_arabic},
#endif

// 亚美尼亚语
#ifdef IM_ARMENIAN_SUPPORT
    {GUIIM_M_Q_ARMENIAN_LEAD_UPPER, s_26key_table_armenian_upper},
    {GUIIM_M_Q_ARMENIAN_LOWER,      s_26key_table_armenian_lower},
    {GUIIM_M_Q_ARMENIAN_UPPER,      s_26key_table_armenian_upper},

    {GUIIM_M_TPQ_ARMENIAN_LEAD_UPPER, s_26key_table_armenian_upper},
    {GUIIM_M_TPQ_ARMENIAN_LOWER,      s_26key_table_armenian_lower},
    {GUIIM_M_TPQ_ARMENIAN_UPPER,      s_26key_table_armenian_upper},
#endif

// 阿萨姆
#ifdef IM_ASSAMESE_SUPPORT
    //{GUIIM_M_Q_ASSAMESE, s_26key_table_assamese},

    //{GUIIM_M_TPQ_ASSAMESE, s_26key_table_assamese},
#endif

// 阿塞拜疆语
#ifdef IM_AZERBAIJANI_SUPPORT
    {GUIIM_M_Q_AZERBAIJANI_LEAD_UPPER, s_26key_table_azerbaijani_upper},
    {GUIIM_M_Q_AZERBAIJANI_LOWER,      s_26key_table_azerbaijani_lower},
    {GUIIM_M_Q_AZERBAIJANI_UPPER,      s_26key_table_azerbaijani_upper},

    {GUIIM_M_TPQ_AZERBAIJANI_LEAD_UPPER, s_26key_table_azerbaijani_upper},
    {GUIIM_M_TPQ_AZERBAIJANI_LOWER,      s_26key_table_azerbaijani_lower},
    {GUIIM_M_TPQ_AZERBAIJANI_UPPER,      s_26key_table_azerbaijani_upper},
#endif

// 巴斯克语
#ifdef IM_BASQUE_SUPPORT
    {GUIIM_M_Q_BASQUE_LEAD_UPPER, s_26key_table_basque_upper},
    {GUIIM_M_Q_BASQUE_LOWER,      s_26key_table_basque_lower},
    {GUIIM_M_Q_BASQUE_UPPER,      s_26key_table_basque_upper},

    {GUIIM_M_TPQ_BASQUE_LEAD_UPPER, s_26key_table_basque_upper},
    {GUIIM_M_TPQ_BASQUE_LOWER,      s_26key_table_basque_lower},
    {GUIIM_M_TPQ_BASQUE_UPPER,      s_26key_table_basque_upper},
#endif

// 孟加拉语
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_Q_BENGALI, s_26key_table_bengali},

    {GUIIM_M_TPQ_BENGALI, s_26key_table_bengali},
#endif

// 保加利亚语
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_Q_BULGARIAN_LEAD_UPPER, s_26key_table_bulgarian_upper},
    {GUIIM_M_Q_BULGARIAN_LOWER,      s_26key_table_bulgarian_lower},
    {GUIIM_M_Q_BULGARIAN_UPPER,      s_26key_table_bulgarian_upper},

    {GUIIM_M_TPQ_BULGARIAN_LEAD_UPPER, s_26key_table_bulgarian_upper},
    {GUIIM_M_TPQ_BULGARIAN_LOWER,      s_26key_table_bulgarian_lower},
    {GUIIM_M_TPQ_BULGARIAN_UPPER,      s_26key_table_bulgarian_upper},
#endif

// 加泰罗尼亚语
#ifdef IM_CATALAN_SUPPORT
    {GUIIM_M_Q_CATALAN_LEAD_UPPER, s_26key_table_catalan_upper},
    {GUIIM_M_Q_CATALAN_LOWER,      s_26key_table_catalan_lower},
    {GUIIM_M_Q_CATALAN_UPPER,      s_26key_table_catalan_upper},

    {GUIIM_M_TPQ_CATALAN_LEAD_UPPER, s_26key_table_catalan_upper},
    {GUIIM_M_TPQ_CATALAN_LOWER,      s_26key_table_catalan_lower},
    {GUIIM_M_TPQ_CATALAN_UPPER,      s_26key_table_catalan_upper},
#endif

// 克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_Q_CROATIAN_LEAD_UPPER, s_26key_table_croatian_upper},
    {GUIIM_M_Q_CROATIAN_LOWER,      s_26key_table_croatian_lower},
    {GUIIM_M_Q_CROATIAN_UPPER,      s_26key_table_croatian_upper},

    {GUIIM_M_TPQ_CROATIAN_LEAD_UPPER, s_26key_table_croatian_upper},
    {GUIIM_M_TPQ_CROATIAN_LOWER,      s_26key_table_croatian_lower},
    {GUIIM_M_TPQ_CROATIAN_UPPER,      s_26key_table_croatian_upper},
#endif

// 捷克语
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_Q_CZECH_LEAD_UPPER, s_26key_table_czech_upper},
    {GUIIM_M_Q_CZECH_LOWER,      s_26key_table_czech_lower},
    {GUIIM_M_Q_CZECH_UPPER,      s_26key_table_czech_upper},

    {GUIIM_M_TPQ_CZECH_LEAD_UPPER, s_26key_table_czech_upper},
    {GUIIM_M_TPQ_CZECH_LOWER,      s_26key_table_czech_lower},
    {GUIIM_M_TPQ_CZECH_UPPER,      s_26key_table_czech_upper},
#endif

// 丹麦语
#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_Q_DANISH_LEAD_UPPER, s_26key_table_danish_upper},
    {GUIIM_M_Q_DANISH_LOWER,      s_26key_table_danish_lower},
    {GUIIM_M_Q_DANISH_UPPER,      s_26key_table_danish_upper},

    {GUIIM_M_TPQ_DANISH_LEAD_UPPER, s_26key_table_danish_upper},
    {GUIIM_M_TPQ_DANISH_LOWER,      s_26key_table_danish_lower},
    {GUIIM_M_TPQ_DANISH_UPPER,      s_26key_table_danish_upper},
#endif

// 荷兰语
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_Q_DUTCH_LEAD_UPPER, s_26key_table_dutch_upper},
    {GUIIM_M_Q_DUTCH_LOWER,      s_26key_table_dutch_lower},
    {GUIIM_M_Q_DUTCH_UPPER,      s_26key_table_dutch_upper},

    {GUIIM_M_TPQ_DUTCH_LEAD_UPPER, s_26key_table_dutch_upper},
    {GUIIM_M_TPQ_DUTCH_LOWER,      s_26key_table_dutch_lower},
    {GUIIM_M_TPQ_DUTCH_UPPER,      s_26key_table_dutch_upper},
#endif

// 爱沙尼亚语
#ifdef IM_ESTONIAN_SUPPORT
    {GUIIM_M_Q_ESTONIAN_LEAD_UPPER, s_26key_table_estonian_upper},
    {GUIIM_M_Q_ESTONIAN_LOWER,      s_26key_table_estonian_lower},
    {GUIIM_M_Q_ESTONIAN_UPPER,      s_26key_table_estonian_upper},

    {GUIIM_M_TPQ_ESTONIAN_LEAD_UPPER, s_26key_table_estonian_upper},
    {GUIIM_M_TPQ_ESTONIAN_LOWER,      s_26key_table_estonian_lower},
    {GUIIM_M_TPQ_ESTONIAN_UPPER,      s_26key_table_estonian_upper},
#endif

// 菲律宾语
#ifdef IM_FILIPINO_SUPPORT
    {GUIIM_M_Q_FILIPINO_LEAD_UPPER, s_26key_table_filipino_upper},
    {GUIIM_M_Q_FILIPINO_LOWER,      s_26key_table_filipino_lower},
    {GUIIM_M_Q_FILIPINO_UPPER,      s_26key_table_filipino_upper},

    {GUIIM_M_TPQ_FILIPINO_LEAD_UPPER, s_26key_table_filipino_upper},
    {GUIIM_M_TPQ_FILIPINO_LOWER,      s_26key_table_filipino_lower},
    {GUIIM_M_TPQ_FILIPINO_UPPER,      s_26key_table_filipino_upper},
#endif

// 芬兰语
#ifdef IM_FINNISH_SUPPORT
    {GUIIM_M_Q_FINNISH_LEAD_UPPER, s_26key_table_finnish_upper},
    {GUIIM_M_Q_FINNISH_LOWER,      s_26key_table_finnish_lower},
    {GUIIM_M_Q_FINNISH_UPPER,      s_26key_table_finnish_upper},

    {GUIIM_M_TPQ_FINNISH_LEAD_UPPER, s_26key_table_finnish_upper},
    {GUIIM_M_TPQ_FINNISH_LOWER,      s_26key_table_finnish_lower},
    {GUIIM_M_TPQ_FINNISH_UPPER,      s_26key_table_finnish_upper},
#endif

// 法语
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_Q_FRENCH_LEAD_UPPER, s_26key_table_french_upper},
    {GUIIM_M_Q_FRENCH_LOWER,      s_26key_table_french_lower},
    {GUIIM_M_Q_FRENCH_UPPER,      s_26key_table_french_upper},

    {GUIIM_M_TPQ_FRENCH_LEAD_UPPER, s_26key_table_french_upper},
    {GUIIM_M_TPQ_FRENCH_LOWER,      s_26key_table_french_lower},
    {GUIIM_M_TPQ_FRENCH_UPPER,      s_26key_table_french_upper},
#endif

// 加利西亚语
#ifdef IM_GALICIAN_SUPPORT
    {GUIIM_M_Q_GALICIAN_LEAD_UPPER, s_26key_table_galician_upper},
    {GUIIM_M_Q_GALICIAN_LOWER,      s_26key_table_galician_lower},
    {GUIIM_M_Q_GALICIAN_UPPER,      s_26key_table_galician_upper},

    {GUIIM_M_TPQ_GALICIAN_LEAD_UPPER, s_26key_table_galician_upper},
    {GUIIM_M_TPQ_GALICIAN_LOWER,      s_26key_table_galician_lower},
    {GUIIM_M_TPQ_GALICIAN_UPPER,      s_26key_table_galician_upper},
#endif

// 格鲁吉亚语
#ifdef IM_GEORGIAN_SUPPORT
    //{GUIIM_M_Q_GEORGIAN, s_26key_table_georgian},

    //{GUIIM_M_TPQ_GEORGIAN, s_26key_table_georgian},
#endif

// 德语
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_Q_GERMAN_LEAD_UPPER, s_26key_table_german_upper},
    {GUIIM_M_Q_GERMAN_LOWER,      s_26key_table_german_lower},
    {GUIIM_M_Q_GERMAN_UPPER,      s_26key_table_german_upper},

    {GUIIM_M_TPQ_GERMAN_LEAD_UPPER, s_26key_table_german_upper},
    {GUIIM_M_TPQ_GERMAN_LOWER,      s_26key_table_german_lower},
    {GUIIM_M_TPQ_GERMAN_UPPER,      s_26key_table_german_upper},
#endif

// 希腊语
#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_Q_GREEK_LEAD_UPPER, s_26key_table_greek_upper},
    {GUIIM_M_Q_GREEK_LOWER,      s_26key_table_greek_lower},
    {GUIIM_M_Q_GREEK_UPPER,      s_26key_table_greek_upper},

    {GUIIM_M_TPQ_GREEK_LEAD_UPPER, s_26key_table_greek_upper},
    {GUIIM_M_TPQ_GREEK_LOWER,      s_26key_table_greek_lower},
    {GUIIM_M_TPQ_GREEK_UPPER,      s_26key_table_greek_upper},
#endif

// 古吉拉特语（印度的一种语言）
#ifdef IM_GUJARATI_SUPPORT
    {GUIIM_M_Q_GUJARATI, s_26key_table_gujarati},

    {GUIIM_M_TPQ_GUJARATI, s_26key_table_gujarati},
#endif

// 豪撒语
#ifdef IM_HAUSA_SUPPORT
    {GUIIM_M_Q_HAUSA_LEAD_UPPER, s_26key_table_hausa_upper},
    {GUIIM_M_Q_HAUSA_LOWER,      s_26key_table_hausa_lower},
    {GUIIM_M_Q_HAUSA_UPPER,      s_26key_table_hausa_upper},

    {GUIIM_M_TPQ_HAUSA_LEAD_UPPER, s_26key_table_hausa_upper},
    {GUIIM_M_TPQ_HAUSA_LOWER,      s_26key_table_hausa_lower},
    {GUIIM_M_TPQ_HAUSA_UPPER,      s_26key_table_hausa_upper},
#endif

// 希伯莱语
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_Q_HEBREW, s_26key_table_hebrew},

    {GUIIM_M_TPQ_HEBREW, s_26key_table_hebrew},
#endif

// 印地语
#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_Q_HINDI,               s_26key_table_hindi},

    {GUIIM_M_TPQ_HINDI,             s_26key_table_hindi},
#endif

// 匈牙利语
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_Q_HUNGARIAN_LEAD_UPPER, s_26key_table_hungarian_upper},
    {GUIIM_M_Q_HUNGARIAN_LOWER,      s_26key_table_hungarian_lower},
    {GUIIM_M_Q_HUNGARIAN_UPPER,      s_26key_table_hungarian_upper},

    {GUIIM_M_TPQ_HUNGARIAN_LEAD_UPPER, s_26key_table_hungarian_upper},
    {GUIIM_M_TPQ_HUNGARIAN_LOWER,      s_26key_table_hungarian_lower},
    {GUIIM_M_TPQ_HUNGARIAN_UPPER,      s_26key_table_hungarian_upper},
#endif

// 冰岛语
#ifdef IM_ICELANDIC_SUPPORT
    {GUIIM_M_Q_ICELANDIC_LEAD_UPPER, s_26key_table_icelandic_upper},
    {GUIIM_M_Q_ICELANDIC_LOWER,      s_26key_table_icelandic_lower},
    {GUIIM_M_Q_ICELANDIC_UPPER,      s_26key_table_icelandic_upper},

    {GUIIM_M_TPQ_ICELANDIC_LEAD_UPPER, s_26key_table_icelandic_upper},
    {GUIIM_M_TPQ_ICELANDIC_LOWER,      s_26key_table_icelandic_lower},
    {GUIIM_M_TPQ_ICELANDIC_UPPER,      s_26key_table_icelandic_upper},
#endif

// 伊博语
#ifdef IM_IGBO_SUPPORT
    {GUIIM_M_Q_IGBO_LEAD_UPPER, s_26key_table_igbo_upper},
    {GUIIM_M_Q_IGBO_LOWER,      s_26key_table_igbo_lower},
    {GUIIM_M_Q_IGBO_UPPER,      s_26key_table_igbo_upper},

    {GUIIM_M_TPQ_IGBO_LEAD_UPPER, s_26key_table_igbo_upper},
    {GUIIM_M_TPQ_IGBO_LOWER,      s_26key_table_igbo_lower},
    {GUIIM_M_TPQ_IGBO_UPPER,      s_26key_table_igbo_upper},
#endif

// 印度尼西亚语
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_Q_INDONESIAN_LEAD_UPPER,   s_26key_table_abc_upper},
    {GUIIM_M_Q_INDONESIAN_LOWER,        s_26key_table_abc_lower},
    {GUIIM_M_Q_INDONESIAN_UPPER,        s_26key_table_abc_upper},

    {GUIIM_M_TPQ_INDONESIAN_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_INDONESIAN_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_INDONESIAN_UPPER,      s_26key_table_abc_upper},
#endif

// 爱尔兰语
#ifdef IM_IRISH_SUPPORT
    {GUIIM_M_Q_IRISH_LEAD_UPPER, s_26key_table_irish_upper},
    {GUIIM_M_Q_IRISH_LOWER,      s_26key_table_irish_lower},
    {GUIIM_M_Q_IRISH_UPPER,      s_26key_table_irish_upper},

    {GUIIM_M_TPQ_IRISH_LEAD_UPPER, s_26key_table_irish_upper},
    {GUIIM_M_TPQ_IRISH_LOWER,      s_26key_table_irish_lower},
    {GUIIM_M_TPQ_IRISH_UPPER,      s_26key_table_irish_upper},
#endif

// 意大利语
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_Q_ITALIAN_LEAD_UPPER, s_26key_table_italian_upper},
    {GUIIM_M_Q_ITALIAN_LOWER,      s_26key_table_italian_lower},
    {GUIIM_M_Q_ITALIAN_UPPER,      s_26key_table_italian_upper},

    {GUIIM_M_TPQ_ITALIAN_LEAD_UPPER, s_26key_table_italian_upper},
    {GUIIM_M_TPQ_ITALIAN_LOWER,      s_26key_table_italian_lower},
    {GUIIM_M_TPQ_ITALIAN_UPPER,      s_26key_table_italian_upper},
#endif

// 卡纳达语（印度南部的一种方言）
#ifdef IM_KANNADA_SUPPORT
    //{GUIIM_M_Q_KANNADA, s_26key_table_kannada},

    //{GUIIM_M_TPQ_KANNADA, s_26key_table_kannada},
#endif

// 哈萨克斯坦
#ifdef IM_KAZAKH_SUPPORT
    {GUIIM_M_Q_KAZAKH_LEAD_UPPER, s_26key_table_kazakh_upper},
    {GUIIM_M_Q_KAZAKH_LOWER,      s_26key_table_kazakh_lower},
    {GUIIM_M_Q_KAZAKH_UPPER,      s_26key_table_kazakh_upper},

    {GUIIM_M_TPQ_KAZAKH_LEAD_UPPER, s_26key_table_kazakh_upper},
    {GUIIM_M_TPQ_KAZAKH_LOWER,      s_26key_table_kazakh_lower},
    {GUIIM_M_TPQ_KAZAKH_UPPER,      s_26key_table_kazakh_upper},
#endif

// 拉脱维亚语
#ifdef IM_LATVIAN_SUPPORT
    {GUIIM_M_Q_LATVIAN_LEAD_UPPER, s_26key_table_latvian_upper},
    {GUIIM_M_Q_LATVIAN_LOWER,      s_26key_table_latvian_lower},
    {GUIIM_M_Q_LATVIAN_UPPER,      s_26key_table_latvian_upper},

    {GUIIM_M_TPQ_LATVIAN_LEAD_UPPER, s_26key_table_latvian_upper},
    {GUIIM_M_TPQ_LATVIAN_LOWER,      s_26key_table_latvian_lower},
    {GUIIM_M_TPQ_LATVIAN_UPPER,      s_26key_table_latvian_upper},
#endif

// 立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_Q_LITHUANIAN_LEAD_UPPER, s_26key_table_lithuanian_upper},
    {GUIIM_M_Q_LITHUANIAN_LOWER,      s_26key_table_lithuanian_lower},
    {GUIIM_M_Q_LITHUANIAN_UPPER,      s_26key_table_lithuanian_upper},

    {GUIIM_M_TPQ_LITHUANIAN_LEAD_UPPER, s_26key_table_lithuanian_upper},
    {GUIIM_M_TPQ_LITHUANIAN_LOWER,      s_26key_table_lithuanian_lower},
    {GUIIM_M_TPQ_LITHUANIAN_UPPER,      s_26key_table_lithuanian_upper},
#endif

// 马其顿
#ifdef IM_MACEDONIAN_SUPPORT
    {GUIIM_M_Q_MACEDONIAN_LEAD_UPPER, s_26key_table_macedonian_upper},
    {GUIIM_M_Q_MACEDONIAN_LOWER,      s_26key_table_macedonian_lower},
    {GUIIM_M_Q_MACEDONIAN_UPPER,      s_26key_table_macedonian_upper},

    {GUIIM_M_TPQ_MACEDONIAN_LEAD_UPPER, s_26key_table_macedonian_upper},
    {GUIIM_M_TPQ_MACEDONIAN_LOWER,      s_26key_table_macedonian_lower},
    {GUIIM_M_TPQ_MACEDONIAN_UPPER,      s_26key_table_macedonian_upper},
#endif

// 马拉雅拉姆语（印度的一种语言）
#ifdef IM_MALAYALAM_SUPPORT
    //{GUIIM_M_Q_MALAYALAM,           s_26key_table_malayalam},

    //{GUIIM_M_TPQ_MALAYALAM,         s_26key_table_malayalam},
#endif

// 缅甸
#ifdef IM_MYANMAR_SUPPORT
    {GUIIM_M_Q_MYANMAR,           s_26key_table_myanmar},

    {GUIIM_M_TPQ_MYANMAR,         s_26key_table_myanmar},
#endif

// 马来语
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_Q_MALAY_LEAD_UPPER,    s_26key_table_abc_upper},
    {GUIIM_M_Q_MALAY_LOWER,         s_26key_table_abc_lower},
    {GUIIM_M_Q_MALAY_UPPER,         s_26key_table_abc_upper},

    {GUIIM_M_TPQ_MALAY_LEAD_UPPER,  s_26key_table_abc_upper},
    {GUIIM_M_TPQ_MALAY_LOWER,       s_26key_table_abc_lower},
    {GUIIM_M_TPQ_MALAY_UPPER,       s_26key_table_abc_upper},
#endif

// 马拉地语
#ifdef IM_MARATHI_SUPPORT
    {GUIIM_M_Q_MARATHI,             s_26key_table_marathi},

    {GUIIM_M_TPQ_MARATHI,           s_26key_table_marathi},
#endif

// 摩尔多瓦语
#ifdef IM_MOLDOVAN_SUPPORT
    //{GUIIM_M_Q_MOLDOVAN_LEAD_UPPER, s_26key_table_moldovan_upper},
    //{GUIIM_M_Q_MOLDOVAN_LOWER,      s_26key_table_moldovan_lower},
    //{GUIIM_M_Q_MOLDOVAN_UPPER,      s_26key_table_moldovan_upper},

    //{GUIIM_M_TPQ_MOLDOVAN_LEAD_UPPER, s_26key_table_moldovan_upper},
    //{GUIIM_M_TPQ_MOLDOVAN_LOWER,      s_26key_table_moldovan_lower},
    //{GUIIM_M_TPQ_MOLDOVAN_UPPER,      s_26key_table_moldovan_upper},
#endif

// 挪威语
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_Q_NORWEGIAN_LEAD_UPPER, s_26key_table_norwegian_upper},
    {GUIIM_M_Q_NORWEGIAN_LOWER,      s_26key_table_norwegian_lower},
    {GUIIM_M_Q_NORWEGIAN_UPPER,      s_26key_table_norwegian_upper},

    {GUIIM_M_TPQ_NORWEGIAN_LEAD_UPPER, s_26key_table_norwegian_upper},
    {GUIIM_M_TPQ_NORWEGIAN_LOWER,      s_26key_table_norwegian_lower},
    {GUIIM_M_TPQ_NORWEGIAN_UPPER,      s_26key_table_norwegian_upper},
#endif

// 奥利亚语
#ifdef IM_ORIYA_SUPPORT
    {GUIIM_M_Q_ORIYA, s_26key_table_oriya},

    {GUIIM_M_TPQ_ORIYA, s_26key_table_oriya},
#endif

// 波斯语
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_Q_PERSIAN, s_26key_table_persian},

    {GUIIM_M_TPQ_PERSIAN, s_26key_table_persian},
#endif

// 波兰
#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_Q_POLISH_LEAD_UPPER, s_26key_table_polish_upper},
    {GUIIM_M_Q_POLISH_LOWER,      s_26key_table_polish_lower},
    {GUIIM_M_Q_POLISH_UPPER,      s_26key_table_polish_upper},

    {GUIIM_M_TPQ_POLISH_LEAD_UPPER, s_26key_table_polish_upper},
    {GUIIM_M_TPQ_POLISH_LOWER,      s_26key_table_polish_lower},
    {GUIIM_M_TPQ_POLISH_UPPER,      s_26key_table_polish_upper},
#endif

// 葡萄牙语
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_Q_PORTUGUESE_LEAD_UPPER, s_26key_table_portuguese_upper},
    {GUIIM_M_Q_PORTUGUESE_LOWER,      s_26key_table_portuguese_lower},
    {GUIIM_M_Q_PORTUGUESE_UPPER,      s_26key_table_portuguese_upper},

    {GUIIM_M_TPQ_PORTUGUESE_LEAD_UPPER, s_26key_table_portuguese_upper},
    {GUIIM_M_TPQ_PORTUGUESE_LOWER,      s_26key_table_portuguese_lower},
    {GUIIM_M_TPQ_PORTUGUESE_UPPER,      s_26key_table_portuguese_upper},
#endif

// 印度旁遮普语
#ifdef IM_PUNJABI_SUPPORT
    {GUIIM_M_Q_PUNJABI, s_26key_table_punjabi},

    {GUIIM_M_TPQ_PUNJABI, s_26key_table_punjabi},
#endif

// 罗马尼亚语
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_Q_ROMANIAN_LEAD_UPPER, s_26key_table_romanian_upper},
    {GUIIM_M_Q_ROMANIAN_LOWER,      s_26key_table_romanian_lower},
    {GUIIM_M_Q_ROMANIAN_UPPER,      s_26key_table_romanian_upper},

    {GUIIM_M_TPQ_ROMANIAN_LEAD_UPPER, s_26key_table_romanian_upper},
    {GUIIM_M_TPQ_ROMANIAN_LOWER,      s_26key_table_romanian_lower},
    {GUIIM_M_TPQ_ROMANIAN_UPPER,      s_26key_table_romanian_upper},
#endif

// 俄语
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_Q_RUSSIAN_LEAD_UPPER, s_26key_table_russian_upper},
    {GUIIM_M_Q_RUSSIAN_LOWER,      s_26key_table_russian_lower},
    {GUIIM_M_Q_RUSSIAN_UPPER,      s_26key_table_russian_upper},

    {GUIIM_M_TPQ_RUSSIAN_LEAD_UPPER, s_26key_table_russian_upper},
    {GUIIM_M_TPQ_RUSSIAN_LOWER,      s_26key_table_russian_lower},
    {GUIIM_M_TPQ_RUSSIAN_UPPER,      s_26key_table_russian_upper},
#endif

// 塞尔维亚语
#ifdef IM_SERBIAN_SUPPORT
    {GUIIM_M_Q_SERBIAN_LEAD_UPPER, s_26key_table_serbian_upper},
    {GUIIM_M_Q_SERBIAN_LOWER,      s_26key_table_serbian_lower},
    {GUIIM_M_Q_SERBIAN_UPPER,      s_26key_table_serbian_upper},

    {GUIIM_M_TPQ_SERBIAN_LEAD_UPPER, s_26key_table_serbian_upper},
    {GUIIM_M_TPQ_SERBIAN_LOWER,      s_26key_table_serbian_lower},
    {GUIIM_M_TPQ_SERBIAN_UPPER,      s_26key_table_serbian_upper},
#endif

// 塞索托语
#ifdef IM_SESOTHO_SUPPORT
    {GUIIM_M_Q_SESOTHO_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_SESOTHO_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_SESOTHO_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_SESOTHO_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_SESOTHO_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_SESOTHO_UPPER,      s_26key_table_abc_upper},
#endif

// 斯洛伐克语
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_Q_SLOVAK_LEAD_UPPER, s_26key_table_slovak_upper},
    {GUIIM_M_Q_SLOVAK_LOWER,      s_26key_table_slovak_lower},
    {GUIIM_M_Q_SLOVAK_UPPER,      s_26key_table_slovak_upper},

    {GUIIM_M_TPQ_SLOVAK_LEAD_UPPER, s_26key_table_slovak_upper},
    {GUIIM_M_TPQ_SLOVAK_LOWER,      s_26key_table_slovak_lower},
    {GUIIM_M_TPQ_SLOVAK_UPPER,      s_26key_table_slovak_upper},
#endif

// 斯洛文尼亚语
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_Q_SLOVENIAN_LEAD_UPPER, s_26key_table_slovenian_upper},
    {GUIIM_M_Q_SLOVENIAN_LOWER,      s_26key_table_slovenian_lower},
    {GUIIM_M_Q_SLOVENIAN_UPPER,      s_26key_table_slovenian_upper},

    {GUIIM_M_TPQ_SLOVENIAN_LEAD_UPPER, s_26key_table_slovenian_upper},
    {GUIIM_M_TPQ_SLOVENIAN_LOWER,      s_26key_table_slovenian_lower},
    {GUIIM_M_TPQ_SLOVENIAN_UPPER,      s_26key_table_slovenian_upper},
#endif

// 西班牙语
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_Q_SPANISH_LEAD_UPPER, s_26key_table_spanish_upper},
    {GUIIM_M_Q_SPANISH_LOWER,      s_26key_table_spanish_lower},
    {GUIIM_M_Q_SPANISH_UPPER,      s_26key_table_spanish_upper},

    {GUIIM_M_TPQ_SPANISH_LEAD_UPPER, s_26key_table_spanish_upper},
    {GUIIM_M_TPQ_SPANISH_LOWER,      s_26key_table_spanish_lower},
    {GUIIM_M_TPQ_SPANISH_UPPER,      s_26key_table_spanish_upper},
#endif

// 斯瓦西里语
#ifdef IM_SWAHILI_SUPPORT       //cstar, sogou等均使用abc数据表
    {GUIIM_M_Q_SWAHILI_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_SWAHILI_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_SWAHILI_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_SWAHILI_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_SWAHILI_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_SWAHILI_UPPER,      s_26key_table_abc_upper},
#endif

// 瑞典语
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_Q_SWEDISH_LEAD_UPPER, s_26key_table_swedish_upper},
    {GUIIM_M_Q_SWEDISH_LOWER,      s_26key_table_swedish_lower},
    {GUIIM_M_Q_SWEDISH_UPPER,      s_26key_table_swedish_upper},

    {GUIIM_M_TPQ_SWEDISH_LEAD_UPPER, s_26key_table_swedish_upper},
    {GUIIM_M_TPQ_SWEDISH_LOWER,      s_26key_table_swedish_lower},
    {GUIIM_M_TPQ_SWEDISH_UPPER,      s_26key_table_swedish_upper},
#endif

// 他加禄语
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_Q_TAGALOG, s_26key_table_tagalog},

    {GUIIM_M_TPQ_TAGALOG, s_26key_table_tagalog},
#endif

// 泰米尔语
#ifdef IM_TAMIL_SUPPORT
    {GUIIM_M_Q_TAMIL, s_26key_table_tamil},

    {GUIIM_M_TPQ_TAMIL, s_26key_table_tamil},
#endif

// 泰卢固语
#ifdef IM_TELUGU_SUPPORT
    {GUIIM_M_Q_TELUGU, s_26key_table_telugu},

    {GUIIM_M_TPQ_TELUGU, s_26key_table_telugu},
#endif

// 泰语
#ifdef IM_THAI_SUPPORT
    {GUIIM_M_Q_THAI, s_26key_table_thai},

    {GUIIM_M_TPQ_THAI, s_26key_table_thai},
#endif

// 土耳其语
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_Q_TURKISH_LEAD_UPPER, s_26key_table_turkish_upper},
    {GUIIM_M_Q_TURKISH_LOWER,      s_26key_table_turkish_lower},
    {GUIIM_M_Q_TURKISH_UPPER,      s_26key_table_turkish_upper},

    {GUIIM_M_TPQ_TURKISH_LEAD_UPPER, s_26key_table_turkish_upper},
    {GUIIM_M_TPQ_TURKISH_LOWER,      s_26key_table_turkish_lower},
    {GUIIM_M_TPQ_TURKISH_UPPER,      s_26key_table_turkish_upper},
#endif

// 乌克兰语
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_Q_UKRAINIAN_LEAD_UPPER, s_26key_table_ukrainian_upper},
    {GUIIM_M_Q_UKRAINIAN_LOWER,      s_26key_table_ukrainian_lower},
    {GUIIM_M_Q_UKRAINIAN_UPPER,      s_26key_table_ukrainian_upper},

    {GUIIM_M_TPQ_UKRAINIAN_LEAD_UPPER, s_26key_table_ukrainian_upper},
    {GUIIM_M_TPQ_UKRAINIAN_LOWER,      s_26key_table_ukrainian_lower},
    {GUIIM_M_TPQ_UKRAINIAN_UPPER,      s_26key_table_ukrainian_upper},
#endif

// 乌尔都语
#ifdef IM_URDU_SUPPORT
    {GUIIM_M_Q_URDU, s_26key_table_urdu},

    {GUIIM_M_TPQ_URDU, s_26key_table_urdu},
#endif

// 越南语
#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_Q_VIETNAMESE_LEAD_UPPER, s_26key_table_vietnamese_upper},
    {GUIIM_M_Q_VIETNAMESE_LOWER,      s_26key_table_vietnamese_lower},
    {GUIIM_M_Q_VIETNAMESE_UPPER,      s_26key_table_vietnamese_upper},

    {GUIIM_M_TPQ_VIETNAMESE_LEAD_UPPER, s_26key_table_vietnamese_upper},
    {GUIIM_M_TPQ_VIETNAMESE_LOWER,      s_26key_table_vietnamese_lower},
    {GUIIM_M_TPQ_VIETNAMESE_UPPER,      s_26key_table_vietnamese_upper},
#endif

// 科萨语（南非的一种方言）
#ifdef IM_XHOSA_SUPPORT
    {GUIIM_M_Q_XHOSA_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_XHOSA_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_XHOSA_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_XHOSA_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_XHOSA_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_XHOSA_UPPER,      s_26key_table_abc_upper},
#endif

// 约鲁巴语
#ifdef IM_YORUBA_SUPPORT
    {GUIIM_M_Q_YORUBA_LEAD_UPPER, s_26key_table_yoruba_upper},
    {GUIIM_M_Q_YORUBA_LOWER,      s_26key_table_yoruba_lower},
    {GUIIM_M_Q_YORUBA_UPPER,      s_26key_table_yoruba_upper},

    {GUIIM_M_TPQ_YORUBA_LEAD_UPPER, s_26key_table_yoruba_upper},
    {GUIIM_M_TPQ_YORUBA_LOWER,      s_26key_table_yoruba_lower},
    {GUIIM_M_TPQ_YORUBA_UPPER,      s_26key_table_yoruba_upper},
#endif

// 祖鲁语（非洲的语种）
#ifdef IM_ZULU_SUPPORT
    {GUIIM_M_Q_ZULU_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_Q_ZULU_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_Q_ZULU_UPPER,      s_26key_table_abc_upper},

    {GUIIM_M_TPQ_ZULU_LEAD_UPPER, s_26key_table_abc_upper},
    {GUIIM_M_TPQ_ZULU_LOWER,      s_26key_table_abc_lower},
    {GUIIM_M_TPQ_ZULU_UPPER,      s_26key_table_abc_upper},
#endif

// 阿姆哈拉，暂不支持智能输入
#ifdef IM_AMHARIC_SUPPORT 
#if defined MMI_IM_PDA_SUPPORT
    //{GUIIM_M_Q_AMHARIC, s_28key_table_amharic},
    //{GUIIM_M_TPQ_AMHARIC, s_28key_table_amharic},
#else
    //{GUIIM_M_Q_AMHARIC, s_26key_table_amharic},
    //{GUIIM_M_TPQ_AMHARIC, s_26key_table_amharic},
#endif
#endif

// 柬埔寨，暂不支持智能输入
#ifdef IM_KHMER_SUPPORT
    //{GUIIM_M_Q_KHMER, s_26key_table_khmer},
    //{GUIIM_M_TPQ_KHMER,      s_26key_table_khmer},
#endif

// 老挝
#ifdef IM_LAO_SUPPORT
    {GUIIM_M_Q_LAO, s_26key_table_lao},
    {GUIIM_M_TPQ_LAO,      s_26key_table_lao},
#endif

#ifdef IM_UYGHUR_SUPPORT
    {GUIIM_M_Q_UYGHUR, s_26key_table_uyghur},
    {GUIIM_M_TPQ_UYGHUR,      s_26key_table_uyghur},
#endif

#ifdef IM_TIBETAN_SUPPORT      
	//{GUIIM_M_Q_TIBETAN, s_26key_table_tibetan},
    //{GUIIM_M_TPQ_TIBETAN, s_26key_table_tibetan},
#endif

#ifdef IM_SINHALESE_SUPPORT
    {GUIIM_M_Q_SINHALESE, s_26key_table_sinhalese},
    {GUIIM_M_TPQ_SINHALESE,      s_26key_table_sinhalese},
#endif

};
#endif

//某些语言输入法存在多页虚拟键盘的问题
static MMIIM_SP_ML_26KEY_TABLE_CFG_T s_spim_smart_26key_lang_shift_page_data[] =
{
#ifdef IM_TIBETAN_SUPPORT      
	//{GUIIM_M_Q_TIBETAN, s_26key_table_tibetan_shift_page},
    //{GUIIM_M_TPQ_TIBETAN, s_26key_table_tibetan_shift_page},

    {GUIIM_M_Q_TIBETAN_ABC, s_26key_table_tibetan_shift_page},
    {GUIIM_M_TPQ_TIBETAN_ABC,      s_26key_table_tibetan_shift_page},
#endif

	{GUIIM_M_TPQ_ENGLISH_ABC_UPPER,      s_26key_table_abc_upper},	//加一个，关闭某些宏 编译不过
};

//某些语言输入法存在多页虚拟键盘的问题
static MMIIM_SP_ML_26KEY_TABLE_CFG_T s_spim_multitap_26key_lang_shift_page_data[] =
{
#ifdef IM_TIBETAN_SUPPORT      
	//{GUIIM_M_Q_TIBETAN, s_26key_table_tibetan_shift_page},
    //{GUIIM_M_TPQ_TIBETAN, s_26key_table_tibetan_shift_page},

    {GUIIM_M_Q_TIBETAN_ABC, s_26key_table_tibetan_shift_page},
    {GUIIM_M_TPQ_TIBETAN_ABC,      s_26key_table_tibetan_shift_page},
#endif

	{GUIIM_M_TPQ_ENGLISH_ABC_LOWER,      s_26key_table_abc_lower},	//加一个，关闭某些宏 编译不过
};

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
    case GUIIM_CUSTOM_KEY_CHR_A:
        key_idx = MMIIM_SP_26KEY_INDEX_A;
        break;

    case GUIIM_CUSTOM_KEY_CHR_B:
        key_idx = MMIIM_SP_26KEY_INDEX_B;
        break;

    case GUIIM_CUSTOM_KEY_CHR_C:
        key_idx = MMIIM_SP_26KEY_INDEX_C;
        break;

    case GUIIM_CUSTOM_KEY_CHR_D:
        key_idx = MMIIM_SP_26KEY_INDEX_D;
        break;

    case GUIIM_CUSTOM_KEY_CHR_E:
        key_idx = MMIIM_SP_26KEY_INDEX_E;
        break;

    case GUIIM_CUSTOM_KEY_CHR_F:
        key_idx = MMIIM_SP_26KEY_INDEX_F;
        break;

    case GUIIM_CUSTOM_KEY_CHR_G:
        key_idx = MMIIM_SP_26KEY_INDEX_G;
        break;

    case GUIIM_CUSTOM_KEY_CHR_H:
        key_idx = MMIIM_SP_26KEY_INDEX_H;
        break;

    case GUIIM_CUSTOM_KEY_CHR_I:
        key_idx = MMIIM_SP_26KEY_INDEX_I;
        break;

    case GUIIM_CUSTOM_KEY_CHR_J:
        key_idx = MMIIM_SP_26KEY_INDEX_J;
        break;

    case GUIIM_CUSTOM_KEY_CHR_K:
        key_idx = MMIIM_SP_26KEY_INDEX_K;
        break;

    case GUIIM_CUSTOM_KEY_CHR_L:
        key_idx = MMIIM_SP_26KEY_INDEX_L;
        break;

    case GUIIM_CUSTOM_KEY_CHR_M:
        key_idx = MMIIM_SP_26KEY_INDEX_M;
        break;

    case GUIIM_CUSTOM_KEY_CHR_N:
        key_idx = MMIIM_SP_26KEY_INDEX_N;
        break;

    case GUIIM_CUSTOM_KEY_CHR_O:
        key_idx = MMIIM_SP_26KEY_INDEX_O;
        break;

    case GUIIM_CUSTOM_KEY_CHR_P:
        key_idx = MMIIM_SP_26KEY_INDEX_P;
        break;

    case GUIIM_CUSTOM_KEY_CHR_Q:
        key_idx = MMIIM_SP_26KEY_INDEX_Q;
        break;

    case GUIIM_CUSTOM_KEY_CHR_R:
        key_idx = MMIIM_SP_26KEY_INDEX_R;
        break;

    case GUIIM_CUSTOM_KEY_CHR_S:
        key_idx = MMIIM_SP_26KEY_INDEX_S;
        break;

    case GUIIM_CUSTOM_KEY_CHR_T:
        key_idx = MMIIM_SP_26KEY_INDEX_T;
        break;

    case GUIIM_CUSTOM_KEY_CHR_U:
        key_idx = MMIIM_SP_26KEY_INDEX_U;
        break;

    case GUIIM_CUSTOM_KEY_CHR_V:
        key_idx = MMIIM_SP_26KEY_INDEX_V;
        break;

    case GUIIM_CUSTOM_KEY_CHR_W:
        key_idx = MMIIM_SP_26KEY_INDEX_W;
        break;

    case GUIIM_CUSTOM_KEY_CHR_X:
        key_idx = MMIIM_SP_26KEY_INDEX_X;
        break;

    case GUIIM_CUSTOM_KEY_CHR_Y:
        key_idx = MMIIM_SP_26KEY_INDEX_Y;
        break;

    case GUIIM_CUSTOM_KEY_CHR_Z:
        key_idx = MMIIM_SP_26KEY_INDEX_Z;
        break;

    case GUIIM_CUSTOM_KEY_DEL:
        key_idx = MMIIM_SP_26KEY_INDEX_DEL;
        break;

    case GUIIM_CUSTOM_KEY_COMMA:
        key_idx = MMIIM_SP_26KEY_INDEX_COMMA;
        break;

    case GUIIM_CUSTOM_KEY_PERIOD:
        key_idx = MMIIM_SP_26KEY_INDEX_PERIOD;
        break;

    case GUIIM_CUSTOM_KEY_ENTER:
        key_idx = MMIIM_SP_26KEY_INDEX_ENTER;
        break;

    case GUIIM_CUSTOM_KEY_FN:
        key_idx = MMIIM_SP_26KEY_INDEX_FN;
        break;

    case GUIIM_CUSTOM_KEY_SHIFT:
        key_idx = MMIIM_SP_26KEY_INDEX_SHIFT;
        break;

    case GUIIM_CUSTOM_KEY_AT_QWERTY:
        key_idx = MMIIM_SP_26KEY_INDEX_AT_QWERTY;
        break;

    case GUIIM_CUSTOM_KEY_SPACE:
        key_idx = MMIIM_SP_26KEY_INDEX_SPACE;
        break;

    case GUIIM_CUSTOM_KEY_AND:
        key_idx = MMIIM_SP_26KEY_INDEX_AND;
        break;

    case GUIIM_CUSTOM_KEY_QUESTION:
        key_idx = MMIIM_SP_26KEY_INDEX_QUESTION;
        break;

    case GUIIM_CUSTOM_KEY_CTRL:
        key_idx = MMIIM_SP_26KEY_INDEX_CTRL;
        break;

        
    //一般的shift key不支持
    case GUIIM_CUSTOM_KEY_PLUS:
    case GUIIM_CUSTOM_KEY_LEFT_PARENTHESIS:
    case GUIIM_CUSTOM_KEY_RIGHT_PARENTHESIS:
    case GUIIM_CUSTOM_KEY_MINUS:
    case GUIIM_CUSTOM_KEY_DOUBLE_QUOTES:
    case GUIIM_CUSTOM_KEY_SEMICOLON:
    case GUIIM_CUSTOM_KEY_COLON:
    case GUIIM_CUSTOM_KEY_SLASH:
    case GUIIM_CUSTOM_KEY_SHIFT_AND:
    case GUIIM_CUSTOM_KEY_EXCLAMATION:
        break;

    case GUIIM_CUSTOM_KEY_0:
        key_idx = MMIIM_SP_26KEY_INDEX_M;
        break;

    case GUIIM_CUSTOM_KEY_1:
        key_idx = MMIIM_SP_26KEY_INDEX_R;
        break;

    case GUIIM_CUSTOM_KEY_2:
        key_idx = MMIIM_SP_26KEY_INDEX_T;
        break;

    case GUIIM_CUSTOM_KEY_3:
        key_idx = MMIIM_SP_26KEY_INDEX_Y;
        break;

    case GUIIM_CUSTOM_KEY_4:
        key_idx = MMIIM_SP_26KEY_INDEX_F;
        break;

    case GUIIM_CUSTOM_KEY_5:
        key_idx = MMIIM_SP_26KEY_INDEX_G;
        break;

    case GUIIM_CUSTOM_KEY_6:
        key_idx = MMIIM_SP_26KEY_INDEX_H;
        break;

    case GUIIM_CUSTOM_KEY_7:
        key_idx = MMIIM_SP_26KEY_INDEX_V;
        break;

    case GUIIM_CUSTOM_KEY_8:
        key_idx = MMIIM_SP_26KEY_INDEX_B;
        break;

    case GUIIM_CUSTOM_KEY_9:
        key_idx = MMIIM_SP_26KEY_INDEX_N;
        break;

    case GUIIM_CUSTOM_KEY_STAR:
        key_idx = MMIIM_SP_26KEY_INDEX_U;
        break;

    case GUIIM_CUSTOM_KEY_HASH:
        key_idx = MMIIM_SP_26KEY_INDEX_J;
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
PUBLIC void* MMIIM_SP_ML26KEY_GetKeyTable(
    GUIIM_METHOD_T method   // [in] method type
)
{
    uint16 i = 0;
    const MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;

    for (i = 0; i < sizeof(s_spim_26key_lang_data) / sizeof(s_spim_26key_lang_data[0]); i++)
    {
        if (s_spim_26key_lang_data[i].method == method)
        {
            key_table_ptr = s_spim_26key_lang_data[i].key_tab_ptr;
            break;
        }
    }

    return (void*)key_table_ptr;
}
/*==============================================================================
Description: 获取智能26键按键对应的multi-tap值
Global resource dependence: NONE
Author:yaoguangchen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_SMART26KEY_GetKeyTable(
    GUIIM_METHOD_T method   // [in] method type
)
{
    uint16 i = 0;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;

    //26键布局
    for (i = 0; i < sizeof(s_spim_smart_26key_lang_data) / sizeof(s_spim_smart_26key_lang_data[0]); i++)
    {
        if (s_spim_smart_26key_lang_data[i].method == method)
        {
            key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T *)s_spim_smart_26key_lang_data[i].key_tab_ptr;
            break;
        }
    }

    //如果没有键盘表，则用默认键盘表
    if(key_table_ptr == NULL)
    {
        key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T *)&s_26key_table_abc_upper;
    }
        
    return key_table_ptr;
}


/*==============================================================================
Description: get key table by method index
Author: yaoguang.chen
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_ML26KEY_GetShiftKeyTable(
    GUIIM_METHOD_T method   // [in] method type
)
{
    uint16 i = 0;
    const MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;

    for (i = 0; i < sizeof(s_spim_multitap_26key_lang_shift_page_data) / sizeof(s_spim_multitap_26key_lang_shift_page_data[0]); i++)
    {
        if (s_spim_multitap_26key_lang_shift_page_data[i].method == method)
        {
            key_table_ptr = s_spim_multitap_26key_lang_shift_page_data[i].key_tab_ptr;
            break;
        }
    }

	if (key_table_ptr == PNULL)
	{
		SCI_TRACE_LOW ("MMIIM_SP_ML26KEY_GetShiftKeyTable");
	}

    return (void*)key_table_ptr;
}

/*==============================================================================
Description: 获取智能26键按键对应的SMART值
Global resource dependence: NONE
Author:yaoguang.chen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_SMART26KEY_GetShiftKeyTable(
    GUIIM_METHOD_T method   // [in] method type
)
{
    uint16 i = 0;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;

    //26键布局
    for (i = 0; i < sizeof(s_spim_smart_26key_lang_shift_page_data) / sizeof(s_spim_smart_26key_lang_shift_page_data[0]); i++)
    {
        if (s_spim_smart_26key_lang_shift_page_data[i].method == method)
        {
            key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T *)s_spim_smart_26key_lang_shift_page_data[i].key_tab_ptr;
            break;
        }
    }

	if (key_table_ptr == PNULL)
	{
		SCI_TRACE_LOW ("MMIIM_SP_SMART26KEY_GetShiftKeyTable");
	}
	
    return key_table_ptr;
}


#if defined MMI_IM_PDA_SUPPORT	//仅仅存在虚拟键盘时，才需要配置该项虚拟键盘丝印功能

/*==============================================================================
Description: 获取智能26键按键对应的虚拟键盘显示字符，暂时与按键码unicode值相同，
			但是仅仅取第一个字符显示
Global resource dependence: NONE
Author:yaoguang.chen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_ML26KEY_GetPrintKeyTable (
    GUIIM_METHOD_T method   // [in] method type
)
{
	return MMIIM_SP_ML26KEY_GetKeyTable (method);
}

/*==============================================================================
Description: 获取multi-tap 26键按键对应的虚拟键盘显示字符，暂时与按键码unicode值相同，
			但是仅仅取第一个字符显示
Global resource dependence: NONE
Author:yaoguang.chen
Note:
==============================================================================*/
PUBLIC void* MMIIM_SP_SMART26KEY_GetPrintKeyTable (
    GUIIM_METHOD_T method   // [in] method type
)
{
	return MMIIM_SP_SMART26KEY_GetKeyTable (method);
}
#endif

/*==============================================================================
Description: 获取指定索引下的配置键值个数
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
uint16 MMIIM_SP_ML26KEY_GetChrCnt(void const * table_ptr, uint16 key_idx)
{
    MMIIM_SP_26KEY_KEYCHAR_T const *key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T*)table_ptr;
    uint16 i = 0;

    if (PNULL == key_table_ptr)
    {
        return 0;
    }

    // 1. find in its owner table
    while (MMIIM_SP_26KEY_INDEX_MAX != key_table_ptr[i].key_index)
    {
        if (key_table_ptr[i].key_index == key_idx)
        {
            return key_table_ptr[i].key_chars[0];
        }

        // we assume key table count is smaller than MMIIM_SP_26KEY_INDEX_MAX,
        // otherwise, the key table is wrong.
        SCI_ASSERT(i <= MMIIM_SP_26KEY_INDEX_MAX); /*assert verified*/

        i++;
    }

    // 2. when it is common index
    // we have common key only from MMIIM_SP_26KEY_COUNT to MMIIM_SP_26KEY_INDEX_MAX
    if (MMIIM_SP_26KEY_COUNT <= key_idx && key_idx < MMIIM_SP_26KEY_INDEX_MAX)
    {
        // data in common key table is in order, so we don't check key index
        // one by one
        return s_26key_table_common[key_idx - MMIIM_SP_26KEY_COUNT].key_chars[0];
    }

    // 3. not find, return 0
    return 0;
}


/*==============================================================================
Description: 获取指定索引下的指定键值
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: char_index从1开始计数
==============================================================================*/
uint16 MMIIM_SP_ML26KEY_GetChr(void const * table_ptr, uint16 key_idx, uint16 chr_idx)
{
    const MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T*)table_ptr;
    uint16 i = 0;

    if (PNULL == key_table_ptr)
    {
        return 0;
    }

    // 1. find in its owner table
    while (MMIIM_SP_26KEY_INDEX_MAX != key_table_ptr[i].key_index)
    {
        if (key_table_ptr[i].key_index == key_idx)
        {
            // check index
            SCI_ASSERT(chr_idx <= key_table_ptr[i].key_chars[0]); /*assert verified*/
            return key_table_ptr[i].key_chars[chr_idx];
        }

        // we assume key table count is smaller than MMIIM_SP_26KEY_INDEX_MAX,
        // otherwise, the key table is wrong.
        SCI_ASSERT(i <= MMIIM_SP_26KEY_INDEX_MAX); /*assert verified*/

        i++;
    }

    // 2. when it is common index
    if (MMIIM_SP_26KEY_COUNT <= key_idx && key_idx < MMIIM_SP_26KEY_INDEX_MAX)
    {
        // check index
        // data in common key table is in order, so we don't check key index
        // one by one
        SCI_ASSERT(chr_idx <= s_26key_table_common[key_idx - MMIIM_SP_26KEY_COUNT].key_chars[0]); /*assert verified*/
        return s_26key_table_common[key_idx - MMIIM_SP_26KEY_COUNT].key_chars[chr_idx];
    }

    // 3. not find, return 0
    return 0;
}

/*==============================================================================
Description: 根据字符得到对应的键值
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML26KEY_GetKeyIndexByChar(
                                                  void const *table_ptr,     // [in]
                                                  wchar     key_char,       // [in]
                                                  uint16    *key_index_ptr  // [out]
                                                  )
{
    uint16 i = 0;
    uint16 j = 0;
    MMIIM_SP_26KEY_KEYCHAR_T const *key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T*)table_ptr;

    if (PNULL == key_index_ptr || PNULL == key_table_ptr)
    {
        return FALSE;
    }

    // 1. find letter table
    while (MMIIM_SP_26KEY_INDEX_MAX != key_table_ptr[i].key_index)
    {
        for (j = 1; j <= key_table_ptr[i].key_chars[0]; j++)
        {
            if (key_char == key_table_ptr[i].key_chars[j])
            {
                *key_index_ptr = key_table_ptr[i].key_index;
                return TRUE;
            }
        }

        // we assume key table count is smaller than MMIIM_SP_26KEY_INDEX_MAX,
        // otherwise, the key table is wrong.
        SCI_ASSERT(i <= MMIIM_SP_26KEY_INDEX_MAX); /*assert verified*/

        i++;
    }

    // 2. find common table
    for (i = 0; i < MMIIM_SP_26KEY_INDEX_MAX - MMIIM_SP_26KEY_COUNT; i++)
    {
        for (j = 1; j <= s_26key_table_common[i].key_chars[0]; j++)
        {
            if (key_char == s_26key_table_common[i].key_chars[j])
            {
                *key_index_ptr = MMIIM_SP_26KEY_COUNT + i;
                return TRUE;
            }
        }
    }

    // 3. not found, just return
    return FALSE;
}

/*==============================================================================
Description: alloc memory for custom key table
Author: hua.fang
Global resource dependence:
Note: 
==============================================================================*/
PUBLIC BOOLEAN MMIIM_SP_ML26KEY_AllocCustomKb(
                                              void const *table_ptr,
                                              void **custom_pptr
                                              )
{
    uint16 i = 0;
    MMIIM_SP_26KEY_CUSTOM_CHAR_T *custom_ptr = PNULL;
    const MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T*)table_ptr;

    if (PNULL == key_table_ptr)
    {
        return FALSE;
    }

    custom_ptr = (MMIIM_SP_26KEY_CUSTOM_CHAR_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_26KEY_CUSTOM_CHAR_T));
    SCI_MEMSET(custom_ptr, 0, sizeof(MMIIM_SP_26KEY_CUSTOM_CHAR_T));

    // set all key index as MMIIM_SP_26KEY_INDEX_MAX
    for (i = 0; i < MMIIM_SP_26KEY_INDEX_MAX + 1; i++)
    {
        (*custom_ptr)[i].key_index = MMIIM_SP_26KEY_INDEX_MAX;
    }

    // 1. set common key data to custom key table
    // because lang key data may redefine some key index, so copy common data first
    for (i = MMIIM_SP_26KEY_COUNT; i < MMIIM_SP_26KEY_INDEX_MAX; i++)
    {
        // set data to custom table in order
        (*custom_ptr)[i] = s_26key_table_common[i - MMIIM_SP_26KEY_COUNT];
    }

    // 2. set lang key data to custom key data
    i = 0;

    while (MMIIM_SP_26KEY_INDEX_MAX != key_table_ptr[i].key_index)
    {
        // set data to custom table in order
        (*custom_ptr)[key_table_ptr[i].key_index] = key_table_ptr[i];

        SCI_ASSERT(MMIIM_SP_26KEY_INDEX_MAX >= i); /*assert verified*/
        i++;
    }

    // fn and ctrl key is used to switch ime if they are not defined
    if (0 == (*custom_ptr)[MMIIM_SP_26KEY_INDEX_FN].key_chars[0])
    {
        (*custom_ptr)[MMIIM_SP_26KEY_INDEX_FN].key_chars[0] = 1;
        (*custom_ptr)[MMIIM_SP_26KEY_INDEX_FN].key_chars[1] = MMIIM_SP_KEY_OP_SYMBOL_PANEL;
    }

    if (0 == (*custom_ptr)[MMIIM_SP_26KEY_INDEX_CTRL].key_chars[0])
    {
        (*custom_ptr)[MMIIM_SP_26KEY_INDEX_CTRL].key_chars[0] = 1;
        (*custom_ptr)[MMIIM_SP_26KEY_INDEX_CTRL].key_chars[1] = MMIIM_SP_KEY_OP_SWITCH_MENU;
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
PUBLIC BOOLEAN MMIIM_SP_ML26KEY_FreeCustomKb(
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
Description: MMIIM_SP_ML26KEY_CustomKb
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ML26KEY_CustomKb(
    GUIIM_TYPE_T cur_im_type,
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr,
    void *custom_kb_ptr
)
{
    uint32 i = 0;
    int32 key_idx = 0;
    MMIIM_SP_26KEY_CUSTOM_CHAR_T *custom_ptr = (MMIIM_SP_26KEY_CUSTOM_CHAR_T*)custom_kb_ptr;
    
    if (PNULL == custom_ptr || PNULL == key_info_ptr)
    {
        return FALSE;
    }

    if (cur_im_type != key_info_ptr->im_type)
    {
        return FALSE;
    }

    if (key_info_ptr->key_count > MMIIM_SP_26KEY_CHAR_MAX)
    {
        key_info_ptr->key_count = MMIIM_SP_26KEY_CHAR_MAX;
    }

    key_idx = ConvertKeyIdx(key_info_ptr->key);

    if (key_idx < 0)
    {
        return FALSE;
    }

    //特别处理配置星号井号键的情况
    //此处是根据当前系统的潜规则来做的，即9键应用简单移植至全键盘
    if (GUIIM_CUSTOM_KEY_STAR == key_info_ptr->key)
    {
        (*custom_ptr)[GUIIM_CUSTOM_KEY_FN].key_chars[0] = 0;
    }
    else if (GUIIM_CUSTOM_KEY_HASH == key_info_ptr->key)
    {
        (*custom_ptr)[GUIIM_CUSTOM_KEY_CTRL].key_chars[0] = 0;
    }

    (*custom_ptr)[key_idx].key_chars[0] = key_info_ptr->key_count;

    for (i = 0; i < key_info_ptr->key_count; i++)
    {
        (*custom_ptr)[key_idx].key_chars[i+1] = key_info_ptr->key_values[i];
    }

    return TRUE;
}

#endif


