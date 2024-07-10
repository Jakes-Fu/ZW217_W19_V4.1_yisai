/*****************************************************************************
** File Name:      mmiim.c                                                   *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIIM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_im_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "window_parse.h"
#include "mmiim_menutable.h"
#include "mmiim_nv.h"
#include "mmiim_id.h"
//#include "mmiim_text.h"
#include "mmiim_internal.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiim_base.h"
#include "mmiim_sp_ml_data.h"
#include "mmiim_im_switching.h"
#include "mmitheme_im.h"

#if defined(MMIIM_CORE_USE_CSTAR)
#include "csk2api.h"
#elif defined(MMIIM_CORE_USE_SOGOU)
#include "WD_Def.h"//6_12_2
#include "wdp_update_wintab.h"
#include "mmiim_sogou.h"
#elif defined(MMIIM_CORE_USE_SPIM)
#include "spim_ch_py26k.h"
#endif

#include "mmiim.h"

#ifdef MMI_CSTAR_UDB_SUPPORT
#include "mmifmm_export.h"
#endif

#ifndef _WIN32
#include "version.h"
#endif

#ifdef IM_ENGINE_T9
#include "mmiim_t9_func.h"
#endif

#define MMI_FULLWIDTH_PUNC_SPACE 0x3000	
#define MMI_HALFWIDTH_PUNC_SPACE 0x0020
#define MMI_FULLWIDTH_PUNC_BASE 0xFEE0
#define MMI_FULLWIDTH_PUNC_START 0xFF00
#define MMI_FULLWIDTH_PUNC_END 0xFF5F	
#define MMI_CJK_PUNC_START 0x3000
#define MMI_CJK_PUNC_END 0x303F
#define MMIIM_CHINESE_FIRST (0x4e00)
#define MMIIM_CHINESE_LAST (0x9fa5)
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct index_char_mapping_tag
{
    uint16 key_index;
    uint16 key_char;
} INDEX_CHAR_MAPPING_T;

typedef struct physical_im_key_mapping_tag
{
    uint16 physical_key;
    uint16 im_vk;
} PHYSICAL_IM_KEY_MAPPING_T;

//根据GUIIM_LANGUAGE_T值排序
LOCAL S_LANG_CODE_STRING s_lang_code_table [] = 
{
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {GUIIM_LANG_CHS, L"Zh" },
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {GUIIM_LANG_CHT, L"Tw" },
#endif
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_LANG_ARABIC, L"Ar" },
#endif
#ifdef IM_ASSAMESE_SUPPORT
    {GUIIM_LANG_ASSAMESE, L"As" },   //有待验证
#endif
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_LANG_BENGALI, L"Be" },
#endif
#ifdef IM_GEORGIAN_SUPPORT
    {GUIIM_LANG_GEORGIAN, L"Ge" },
#endif
#ifdef IM_GUJARATI_SUPPORT
    {GUIIM_LANG_GUJARATI, L"Gu" },
#endif
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_LANG_HEBREW, L"He" },
#endif
#ifdef IM_HINDI_SUPPORT
    {GUIIM_LANG_HINDI, L"Hi" },
#endif
#ifdef IM_KANNADA_SUPPORT
    {GUIIM_LANG_KANNADA, L"Ka"},    //有待验证
#endif
#ifdef IM_MALAYALAM_SUPPORT
    {GUIIM_LANG_MALAYALAM, L"Ma"},
#endif
#ifdef IM_MARATHI_SUPPORT
    {GUIIM_LANG_MARATHI, L"Mr" },
#endif
#ifdef IM_ORIYA_SUPPORT
    {GUIIM_LANG_ORIYA, L"Or" },      //有待验证
#endif
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_LANG_PERSIAN, L"Ba" },    //有待验证
#endif
#ifdef IM_PUNJABI_SUPPORT
    {GUIIM_LANG_PUNJABI, L"Pa" },
#endif
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_LANG_TAGALOG, L"Tl" },      //有待验证
#endif
#ifdef IM_TAMIL_SUPPORT
    {GUIIM_LANG_TAMIL, L"Ta" },
#endif
#ifdef IM_TELUGU_SUPPORT
    {GUIIM_LANG_TELUGU, L"Te" },
#endif
#ifdef IM_THAI_SUPPORT
    {GUIIM_LANG_THAI, L"Th" },
#endif
#ifdef IM_URDU_SUPPORT
    {GUIIM_LANG_URDU, L"Ud"},     //有待验证
#endif
#ifdef IM_AFRIKAANS_SUPPORT
    {GUIIM_LANG_AFRIKAANS, L"Af"}, //有待验证
#endif
#ifdef IM_ALBANIAN_SUPPORT
    {GUIIM_LANG_ALBANIAN, L"Sq"},
#endif
#ifdef IM_ARMENIAN_SUPPORT
    {GUIIM_LANG_ARMENIAN, L"Hy"},
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
    {GUIIM_LANG_AZERBAIJANI, L"Az"},
#endif
#ifdef IM_BASQUE_SUPPORT
    {GUIIM_LANG_BASQUE, L"Bs"},        //有待验证
#endif
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_LANG_BULGARIAN, L"Bg"},
#endif
#ifdef IM_CATALAN_SUPPORT
    {GUIIM_LANG_CATALAN, L"Ct"},       //有待验证
#endif
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_LANG_CROATIAN, L"Hr"},
#endif
#ifdef IM_CZECH_SUPPORT
    {GUIIM_LANG_CZECH, L"Cz"},
#endif
#ifdef IM_DANISH_SUPPORT
    {GUIIM_LANG_DANISH, L"Dk"},
#endif
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_LANG_DUTCH, L"Nl"},
#endif
    {GUIIM_LANG_ENGLISH, L"En"},
#ifdef IM_ESTONIAN_SUPPORT
    {GUIIM_LANG_ESTONIAN, L"Et"},
#endif
#ifdef IM_FILIPINO_SUPPORT
    {GUIIM_LANG_FILIPINO, L"Ph"},
#endif
#ifdef IM_FINNISH_SUPPORT
    {GUIIM_LANG_FINNISH, L"Fi"},
#endif
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_LANG_FRENCH, L"Fr"},
#endif
#ifdef IM_GALICIAN_SUPPORT
    {GUIIM_LANG_GALICIAN, L"Gl"},    //有待验证
#endif
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_LANG_GERMAN, L"De"},
#endif
#ifdef IM_GREEK_SUPPORT
    {GUIIM_LANG_GREEK, L"Gr"},
#endif
#ifdef IM_HAUSA_SUPPORT
    {GUIIM_LANG_HAUSA, L"Hs"},   //有待验证
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_LANG_HUNGARIAN, L"Hu"},
#endif
#ifdef IM_ICELANDIC_SUPPORT
    {GUIIM_LANG_ICELANDIC, L"Is"},
#endif
#ifdef IM_IGBO_SUPPORT
    {GUIIM_LANG_IGBO, L"Ib"},        //有待验证
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_LANG_INDONESIAN, L"Id"},
#endif
#ifdef IM_IRISH_SUPPORT
    {GUIIM_LANG_IRISH, L"Ie"},
#endif
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_LANG_ITALIAN, L"It"},
#endif
#ifdef IM_KAZAKH_SUPPORT
    {GUIIM_LANG_KAZAKH, L"Kz"},
#endif
#ifdef IM_LATVIAN_SUPPORT
    {GUIIM_LANG_LATVIAN, L"Lv"},
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_LANG_LITHUANIAN, L"Lt"},
#endif
#ifdef IM_MACEDONIAN_SUPPORT
    {GUIIM_LANG_MACEDONIAN, L"Mk"},
#endif
#ifdef IM_MALAY_SUPPORT
    {GUIIM_LANG_MALAY, L"My"},
#endif
#ifdef IM_MOLDOVAN_SUPPORT
    {GUIIM_LANG_MOLDOVAN, L"Md"},
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_LANG_NORWEGIAN, L"No"},
#endif
#ifdef IM_POLISH_SUPPORT
    {GUIIM_LANG_POLISH, L"Pl"},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_LANG_PORTUGUESE, L"Pt"},
#endif
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_LANG_ROMANIAN, L"Ro"},
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_LANG_RUSSIAN     , L"Ru"},
#endif
#ifdef IM_SERBIAN_SUPPORT
    {GUIIM_LANG_SERBIAN, L"Se"},     //有待验证
#endif
#ifdef IM_SESOTHO_SUPPORT
    {GUIIM_LANG_SESOTHO, L"Xh"},     //版图语
#endif
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_LANG_SLOVAK, L"Sk"},
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_LANG_SLOVENIAN, L"Sl"},
#endif
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_LANG_SPANISH, L"Es"},
#endif
#ifdef IM_SWAHILI_SUPPORT
    {GUIIM_LANG_SWAHILI, L"Sw"},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_LANG_SWEDISH, L"Sv"},
#endif
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_LANG_TURKISH, L"Tr"},
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_LANG_UKRAINIAN, L"Uk"},
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_LANG_VIETNAMESE, L"Vi"},
#endif
#ifdef IM_XHOSA_SUPPORT
    {GUIIM_LANG_XHOSA, L"Xh"},
#endif
#ifdef IM_YORUBA_SUPPORT
    {GUIIM_LANG_YORUBA, L"Yo"},
#endif
#ifdef IM_ZULU_SUPPORT
    {GUIIM_LANG_ZULU, L"Zu"},
#endif
#ifdef IM_MYANMAR_SUPPORT
    {GUIIM_LANG_MYANMAR , L"Mm"},
#endif
#ifdef IM_AMHARIC_SUPPORT
    {GUIIM_LANG_AMHARIC , L"Am"},        //有待验证
#endif
#ifdef IM_KHMER_SUPPORT
    {GUIIM_LANG_KHMER, L"Kh"},
#endif
#ifdef IM_LAO_SUPPORT
    {GUIIM_LANG_LAO, L"La"},
#endif
#ifdef IM_UYGHUR_SUPPORT
    {GUIIM_LANG_UYGHUR, L"Uy"},
#endif
#ifdef IM_TIBETAN_SUPPORT
    {GUIIM_LANG_TIBETAN, L"Ti"},
#endif
#ifdef IM_SINHALESE_SUPPORT
    {GUIIM_LANG_SINHALESE, L"Sin"}, 	//有待验证
#endif
};
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)

LOCAL INDEX_CHAR_MAPPING_T s_index_char_mapping_26key[] =
{
    { MMIIM_SP_26KEY_INDEX_A, 'a' },
    { MMIIM_SP_26KEY_INDEX_B, 'b' },
    { MMIIM_SP_26KEY_INDEX_C, 'c' },
    { MMIIM_SP_26KEY_INDEX_D, 'd' },
    { MMIIM_SP_26KEY_INDEX_E, 'e' },
    { MMIIM_SP_26KEY_INDEX_F, 'f' },
    { MMIIM_SP_26KEY_INDEX_G, 'g' },
    { MMIIM_SP_26KEY_INDEX_H, 'h' },
    { MMIIM_SP_26KEY_INDEX_I, 'i' },
    { MMIIM_SP_26KEY_INDEX_J, 'j' },
    { MMIIM_SP_26KEY_INDEX_K, 'k' },
    { MMIIM_SP_26KEY_INDEX_L, 'l' },
    { MMIIM_SP_26KEY_INDEX_M, 'm' },
    { MMIIM_SP_26KEY_INDEX_N, 'n' },
    { MMIIM_SP_26KEY_INDEX_O, 'o' },
    { MMIIM_SP_26KEY_INDEX_P, 'p' },
    { MMIIM_SP_26KEY_INDEX_Q, 'q' },
    { MMIIM_SP_26KEY_INDEX_R, 'r' },
    { MMIIM_SP_26KEY_INDEX_S, 's' },
    { MMIIM_SP_26KEY_INDEX_T, 't' },
    { MMIIM_SP_26KEY_INDEX_U, 'u' },
    { MMIIM_SP_26KEY_INDEX_V, 'v' },
    { MMIIM_SP_26KEY_INDEX_W, 'w' },
    { MMIIM_SP_26KEY_INDEX_X, 'x' },
    { MMIIM_SP_26KEY_INDEX_Y, 'y' },
    { MMIIM_SP_26KEY_INDEX_Z, 'z' },

    { MMIIM_SP_26KEY_INDEX_DEL, '\b'},
    { MMIIM_SP_26KEY_INDEX_COMMA, ',' },
    { MMIIM_SP_26KEY_INDEX_PERIOD, '.' },
    { MMIIM_SP_26KEY_INDEX_ENTER, '\n'},
    { MMIIM_SP_26KEY_INDEX_AT_QWERTY, '@' },
    { MMIIM_SP_26KEY_INDEX_SPACE, ' ' },
    { MMIIM_SP_26KEY_INDEX_AND, '&' },
    { MMIIM_SP_26KEY_INDEX_QUESTION, '?' },

    { MMIIM_SP_26KEY_INDEX_PLUS, '+' },
    { MMIIM_SP_26KEY_INDEX_LEFT_PARENTHESIS, '(' },
    { MMIIM_SP_26KEY_INDEX_RIGHT_PARENTHESIS, ')' },
    { MMIIM_SP_26KEY_INDEX_MINUS, '-' },
    { MMIIM_SP_26KEY_INDEX_DOUBLE_QUOTES, '\"'},
    { MMIIM_SP_26KEY_INDEX_SEMICOLON, ';' },
    { MMIIM_SP_26KEY_INDEX_COLON, ':' },
    { MMIIM_SP_26KEY_INDEX_SLASH, '/' },
    { MMIIM_SP_26KEY_INDEX_SHIFT_AND, '\'' },
    { MMIIM_SP_26KEY_INDEX_EXCLAMATION, '!' },

    { MMIIM_SP_26KEY_INDEX_0, '0' },
    { MMIIM_SP_26KEY_INDEX_1, '1' },
    { MMIIM_SP_26KEY_INDEX_2, '2' },
    { MMIIM_SP_26KEY_INDEX_3, '3' },
    { MMIIM_SP_26KEY_INDEX_4, '4' },
    { MMIIM_SP_26KEY_INDEX_5, '5' },
    { MMIIM_SP_26KEY_INDEX_6, '6' },
    { MMIIM_SP_26KEY_INDEX_7, '7' },
    { MMIIM_SP_26KEY_INDEX_8, '8' },
    { MMIIM_SP_26KEY_INDEX_9, '9' },
    { MMIIM_SP_26KEY_INDEX_STAR, '*' },
    { MMIIM_SP_26KEY_INDEX_HASH, '#' },
};
#endif
LOCAL INDEX_CHAR_MAPPING_T s_index_char_mapping_9key[] =
{
    { MMIIM_SP_9KEY_INDEX_1, '1' },
    { MMIIM_SP_9KEY_INDEX_2, '2' },
    { MMIIM_SP_9KEY_INDEX_3, '3' },
    { MMIIM_SP_9KEY_INDEX_4, '4' },
    { MMIIM_SP_9KEY_INDEX_5, '5' },
    { MMIIM_SP_9KEY_INDEX_6, '6' },
    { MMIIM_SP_9KEY_INDEX_7, '7' },
    { MMIIM_SP_9KEY_INDEX_8, '8' },
    { MMIIM_SP_9KEY_INDEX_9, '9' },
    { MMIIM_SP_9KEY_INDEX_0, '0' },
    { MMIIM_SP_9KEY_INDEX_STAR, '*' },
    { MMIIM_SP_9KEY_INDEX_HASH, '#' },
};


//下面是MMK消息到输入法按键消息的映射，当前仅仅分shift状态和非shift状态
//如果某个按键不做处理，则将该按键的键码给注释掉，如cust中的asd等
//如果某个按键键码没有变化，则前后书写键码值基本一致并保留向下处理
//如果键码值发生改变，如cust中shift状态的q键等
#if defined QWERTY_KEYPAD_BLACKBERRY
LOCAL PHYSICAL_IM_KEY_MAPPING_T s_phy_im_key_map[] =    //正常按键映射表
{
    {KEY_0, MMIIM_SP_KEY_0}, 
    {KEY_1, MMIIM_SP_KEY_1}, 
    {KEY_2, MMIIM_SP_KEY_2}, 
    {KEY_3, MMIIM_SP_KEY_3}, 
    {KEY_4, MMIIM_SP_KEY_4}, 
    {KEY_5, MMIIM_SP_KEY_5}, 
    {KEY_6, MMIIM_SP_KEY_6}, 
    {KEY_7, MMIIM_SP_KEY_7}, 
    {KEY_8, MMIIM_SP_KEY_8}, 
    {KEY_9, MMIIM_SP_KEY_9}, 
    {KEY_STAR, MMIIM_SP_KEY_STAR}, 
    {KEY_HASH, MMIIM_SP_KEY_HASH}, 

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
    {KEY_Q, MMIIM_SP_KEY_Q}, 
    {KEY_W, MMIIM_SP_KEY_W}, 
    {KEY_E, MMIIM_SP_KEY_E}, 
    {KEY_R, MMIIM_SP_KEY_R}, 
    {KEY_T, MMIIM_SP_KEY_T}, 
    {KEY_Y, MMIIM_SP_KEY_Y}, 
    {KEY_U, MMIIM_SP_KEY_U}, 
    {KEY_I, MMIIM_SP_KEY_I}, 
    {KEY_O, MMIIM_SP_KEY_O}, 
    {KEY_P, MMIIM_SP_KEY_P}, 
    {KEY_A, MMIIM_SP_KEY_A}, 
    {KEY_S, MMIIM_SP_KEY_S}, 
    {KEY_D, MMIIM_SP_KEY_D}, 
    {KEY_F, MMIIM_SP_KEY_F}, 
    {KEY_G, MMIIM_SP_KEY_G}, 
    {KEY_H, MMIIM_SP_KEY_H}, 
    {KEY_J, MMIIM_SP_KEY_J}, 
    {KEY_K, MMIIM_SP_KEY_K}, 
    {KEY_L, MMIIM_SP_KEY_L}, 
    {KEY_Z, MMIIM_SP_KEY_Z}, 
    {KEY_X, MMIIM_SP_KEY_X}, 
    {KEY_C, MMIIM_SP_KEY_C}, 
    {KEY_V, MMIIM_SP_KEY_V}, 
    {KEY_B, MMIIM_SP_KEY_B}, 
    {KEY_N, MMIIM_SP_KEY_N}, 
    {KEY_M, MMIIM_SP_KEY_M}, 
#endif

    //字符按键，引擎基本上不处理        
    {KEY_DEL, MMIIM_SP_KEY_DEL},
    {KEY_SPACE, MMIIM_SP_KEY_SPACE},
    {KEY_SLASH, MMIIM_SP_KEY_SLASH},

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)    
    {KEY_COMMA, MMIIM_SP_KEY_COMMA},
    {KEY_PERIOD, MMIIM_SP_KEY_PERIOD},
    {KEY_ENTER, MMIIM_SP_KEY_ENTER},
    {KEY_AT_QWERTY, MMIIM_SP_KEY_AT_QWERTY},
    {KEY_LEFT_PARENTHESIS, MMIIM_SP_KEY_LEFT_PARENTHESIS },
    {KEY_RIGHT_PARENTHESIS, MMIIM_SP_KEY_RIGHT_PARENTHESIS},
    {KEY_AND, MMIIM_SP_KEY_AND},
    //{KEY_QUESTION, MMIIM_SP_KEY_QUESTION},
    {KEY_PLUS, MMIIM_SP_KEY_PLUS},
    
    {KEY_MINUS, MMIIM_SP_KEY_MINUS},
    {KEY_DOUBLE_QUOTES, MMIIM_SP_KEY_DOUBLE_QUOTES},
    {KEY_SEMICOLON, MMIIM_SP_KEY_SEMICOLON},
    {KEY_COLON, MMIIM_SP_KEY_COLON},
    {KEY_SHIFT_AND, MMIIM_SP_KEY_SHIFT_AND},
    {KEY_EXCLAMATION, MMIIM_SP_KEY_EXCLAMATION},
	{KEY_FN, MMIIM_SP_KEY_FN},
#endif

    //功能键
	{KEY_WEB, MMIIM_SP_KEY_OK},
    {KEY_OK, MMIIM_SP_KEY_OK},
    {KEY_CANCEL, MMIIM_SP_KEY_CANCEL},
    {KEY_UP, MMIIM_SP_KEY_UP},
    {KEY_DOWN, MMIIM_SP_KEY_DOWN},
    {KEY_LEFT, MMIIM_SP_KEY_LEFT},
    {KEY_RIGHT, MMIIM_SP_KEY_RIGHT},
    {KEY_CTRL, MMIIM_SP_KEY_CTRL},        
};


//按下shift key对应的映射表
LOCAL PHYSICAL_IM_KEY_MAPPING_T s_phy_im_shift_key_map[] =  //按下shift key对应的映射表
{
    {KEY_0, MMIIM_SP_KEY_M}, 
    {KEY_1, MMIIM_SP_KEY_R}, 
    {KEY_2, MMIIM_SP_KEY_T}, 
    {KEY_3, MMIIM_SP_KEY_Y}, 
    {KEY_4, MMIIM_SP_KEY_F}, 
    {KEY_5, MMIIM_SP_KEY_G}, 
    {KEY_6, MMIIM_SP_KEY_H}, 
    {KEY_7, MMIIM_SP_KEY_V}, 
    {KEY_8, MMIIM_SP_KEY_B}, 
    {KEY_9, MMIIM_SP_KEY_N}, 
    {KEY_STAR, MMIIM_SP_KEY_STAR}, 
    {KEY_HASH, MMIIM_SP_KEY_HASH}, 

    //下面是标准26键，但是按键的shift上键存在客户定制可能
#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
    {KEY_W, MMIIM_SP_KEY_1}, 
    {KEY_E, MMIIM_SP_KEY_2}, 
    {KEY_R, MMIIM_SP_KEY_3}, 
    {KEY_S, MMIIM_SP_KEY_4}, 
    {KEY_D, MMIIM_SP_KEY_5}, 
    {KEY_F, MMIIM_SP_KEY_6}, 
    {KEY_X, MMIIM_SP_KEY_7}, 
    {KEY_C, MMIIM_SP_KEY_8}, 
    {KEY_V, MMIIM_SP_KEY_9}, 

    {KEY_A, MMIIM_SP_KEY_STAR}, 
    {KEY_Q, MMIIM_SP_KEY_PLUS}, 
    {KEY_O, MMIIM_SP_KEY_PLUS},       
    {KEY_P, MMIIM_SP_KEY_RIGHT_PARENTHESIS},      
    {KEY_J, MMIIM_SP_KEY_HASH}, 
    {KEY_K, MMIIM_SP_KEY_MINUS}, 
    {KEY_B, MMIIM_SP_KEY_0},
    {KEY_I, MMIIM_SP_KEY_MINUS },
    {KEY_L, MMIIM_SP_KEY_DOUBLE_QUOTES },
    {KEY_T, MMIIM_SP_KEY_LEFT_PARENTHESIS },
    {KEY_Y, MMIIM_SP_KEY_RIGHT_PARENTHESIS },
    {KEY_H, MMIIM_SP_KEY_COLON },
    {KEY_G, MMIIM_SP_KEY_SLASH},
    {KEY_N, MMIIM_SP_KEY_COMMA}, 
    {KEY_M, MMIIM_SP_KEY_PERIOD}, 
#endif

    //附加按键配置                         
    {KEY_DEL, MMIIM_SP_KEY_DEL},

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)     
    {KEY_COMMA, MMIIM_SP_KEY_SEMICOLON},        //, ;
    {KEY_PERIOD, MMIIM_SP_KEY_COLON},           //. : 
    {KEY_ENTER, MMIIM_SP_KEY_ENTER},
    {KEY_SPACE, MMIIM_SP_KEY_SPACE},
    {KEY_AND, MMIIM_SP_KEY_AND},
    {KEY_AT_QWERTY, MMIIM_SP_KEY_AT_QWERTY},
	{KEY_EXCLAMATION, MMIIM_SP_KEY_QUESTION},	//! ?
#endif

    //功能键
	{KEY_WEB, MMIIM_SP_KEY_OK},
    {KEY_OK, MMIIM_SP_KEY_OK},
    {KEY_CANCEL, MMIIM_SP_KEY_CANCEL},
    {KEY_UP, MMIIM_SP_KEY_UP},
    {KEY_DOWN, MMIIM_SP_KEY_DOWN},
    {KEY_LEFT, MMIIM_SP_KEY_LEFT},
    {KEY_RIGHT, MMIIM_SP_KEY_RIGHT},
};


#elif defined QWERTY_KEYPAD_CUST        //客户自定义的按键序列
LOCAL PHYSICAL_IM_KEY_MAPPING_T s_phy_im_key_map[] =    //正常按键映射表
{
    {KEY_0, MMIIM_SP_KEY_0}, 
    {KEY_1, MMIIM_SP_KEY_1}, 
    {KEY_2, MMIIM_SP_KEY_2}, 
    {KEY_3, MMIIM_SP_KEY_3}, 
    {KEY_4, MMIIM_SP_KEY_4}, 
    {KEY_5, MMIIM_SP_KEY_5}, 
    {KEY_6, MMIIM_SP_KEY_6}, 
    {KEY_7, MMIIM_SP_KEY_7}, 
    {KEY_8, MMIIM_SP_KEY_8}, 
    {KEY_9, MMIIM_SP_KEY_9}, 
    {KEY_STAR, MMIIM_SP_KEY_STAR}, 
    {KEY_HASH, MMIIM_SP_KEY_HASH}, 

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
    {KEY_Q, MMIIM_SP_KEY_Q}, 
    {KEY_W, MMIIM_SP_KEY_W}, 
    {KEY_E, MMIIM_SP_KEY_E}, 
    {KEY_R, MMIIM_SP_KEY_R}, 
    {KEY_T, MMIIM_SP_KEY_T}, 
    {KEY_Y, MMIIM_SP_KEY_Y}, 
    {KEY_U, MMIIM_SP_KEY_U}, 
    {KEY_I, MMIIM_SP_KEY_I}, 
    {KEY_O, MMIIM_SP_KEY_O}, 
    {KEY_P, MMIIM_SP_KEY_P}, 
    {KEY_A, MMIIM_SP_KEY_A}, 
    {KEY_S, MMIIM_SP_KEY_S}, 
    {KEY_D, MMIIM_SP_KEY_D}, 
    {KEY_F, MMIIM_SP_KEY_F}, 
    {KEY_G, MMIIM_SP_KEY_G}, 
    {KEY_H, MMIIM_SP_KEY_H}, 
    {KEY_J, MMIIM_SP_KEY_J}, 
    {KEY_K, MMIIM_SP_KEY_K}, 
    {KEY_L, MMIIM_SP_KEY_L}, 
    {KEY_Z, MMIIM_SP_KEY_Z}, 
    {KEY_X, MMIIM_SP_KEY_X}, 
    {KEY_C, MMIIM_SP_KEY_C}, 
    {KEY_V, MMIIM_SP_KEY_V}, 
    {KEY_B, MMIIM_SP_KEY_B}, 
    {KEY_N, MMIIM_SP_KEY_N}, 
    {KEY_M, MMIIM_SP_KEY_M}, 
#endif

    //字符按键，引擎基本上不处理        
    {KEY_DEL, MMIIM_SP_KEY_DEL},
    {KEY_SPACE, MMIIM_SP_KEY_SPACE},
    {KEY_SLASH, MMIIM_SP_KEY_SLASH},

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)    
    {KEY_COMMA, MMIIM_SP_KEY_COMMA},
    {KEY_PERIOD, MMIIM_SP_KEY_PERIOD},
    {KEY_ENTER, MMIIM_SP_KEY_ENTER},
    {KEY_AT_QWERTY, MMIIM_SP_KEY_AT_QWERTY},
    {KEY_AT, MMIIM_SP_KEY_AT_QWERTY},
    {KEY_LEFT_PARENTHESIS, MMIIM_SP_KEY_LEFT_PARENTHESIS },
    {KEY_RIGHT_PARENTHESIS, MMIIM_SP_KEY_RIGHT_PARENTHESIS},
    {KEY_AND, MMIIM_SP_KEY_AND},
    {KEY_QUESTION, MMIIM_SP_KEY_QUESTION},
    {KEY_PLUS, MMIIM_SP_KEY_PLUS},
    
    {KEY_MINUS, MMIIM_SP_KEY_MINUS},
    {KEY_DOUBLE_QUOTES, MMIIM_SP_KEY_DOUBLE_QUOTES},
    {KEY_SEMICOLON, MMIIM_SP_KEY_SEMICOLON},
    {KEY_COLON, MMIIM_SP_KEY_COLON},
    {KEY_SHIFT_AND, MMIIM_SP_KEY_SHIFT_AND},
    {KEY_EXCLAMATION, MMIIM_SP_KEY_EXCLAMATION},
	{KEY_FN, MMIIM_SP_KEY_FN},
#endif

    //功能键
	{KEY_WEB, MMIIM_SP_KEY_OK},
    {KEY_OK, MMIIM_SP_KEY_OK},
    {KEY_CANCEL, MMIIM_SP_KEY_CANCEL},
    {KEY_UP, MMIIM_SP_KEY_UP},
    {KEY_DOWN, MMIIM_SP_KEY_DOWN},
    {KEY_LEFT, MMIIM_SP_KEY_LEFT},
    {KEY_RIGHT, MMIIM_SP_KEY_RIGHT},
    {KEY_CTRL, MMIIM_SP_KEY_CTRL},        
};

//按下shift key对应的映射表
LOCAL PHYSICAL_IM_KEY_MAPPING_T s_phy_im_shift_key_map[] =  //按下shift key对应的映射表
{
    {KEY_0, MMIIM_SP_KEY_M}, 
    {KEY_1, MMIIM_SP_KEY_R}, 
    {KEY_2, MMIIM_SP_KEY_T}, 
    {KEY_3, MMIIM_SP_KEY_Y}, 
    {KEY_4, MMIIM_SP_KEY_F}, 
    {KEY_5, MMIIM_SP_KEY_G}, 
    {KEY_6, MMIIM_SP_KEY_H}, 
    {KEY_7, MMIIM_SP_KEY_V}, 
    {KEY_8, MMIIM_SP_KEY_B}, 
    {KEY_9, MMIIM_SP_KEY_N}, 
    {KEY_STAR, MMIIM_SP_KEY_STAR}, 
    {KEY_HASH, MMIIM_SP_KEY_HASH}, 

    //下面是标准26键，但是按键的shift上键存在客户定制可能
#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
    {KEY_Q, MMIIM_SP_KEY_QUOTES},       // '  MMK不支持的按键
    {KEY_W, MMIIM_SP_KEY_LEFT_PARENTHESIS}, 
    {KEY_E, MMIIM_SP_KEY_RIGHT_PARENTHESIS}, 
    {KEY_R, MMIIM_SP_KEY_1}, 
    {KEY_T, MMIIM_SP_KEY_2}, 
    {KEY_Y, MMIIM_SP_KEY_3}, 
    {KEY_U, MMIIM_SP_KEY_STAR}, 
    {KEY_I, MMIIM_SP_KEY_PLUS}, 
    {KEY_O, MMIIM_SP_KEY_EQUAL},        // =  MMK不支持的按键
    {KEY_P, MMIIM_SP_KEY_RIGHT_SLASH},      // \  MMK不支持的按键
    //{KEY_A, MMIIM_SP_KEY_A}, 
    //{KEY_S, MMIIM_SP_KEY_S}, 
    //{KEY_D, MMIIM_SP_KEY_D}, 
    {KEY_F, MMIIM_SP_KEY_4}, 
    {KEY_G, MMIIM_SP_KEY_5}, 
    {KEY_H, MMIIM_SP_KEY_6}, 
    {KEY_J, MMIIM_SP_KEY_HASH}, 
    {KEY_K, MMIIM_SP_KEY_MINUS}, 
    {KEY_L, MMIIM_SP_KEY_DOUBLE_QUOTES}, 
    //{KEY_Z, MMIIM_SP_KEY_Z}, 
    //{KEY_X, MMIIM_SP_KEY_X}, 
    //{KEY_C, MMIIM_SP_KEY_C}, 
    {KEY_V, MMIIM_SP_KEY_7}, 
    {KEY_B, MMIIM_SP_KEY_8}, 
    {KEY_N, MMIIM_SP_KEY_9}, 
    {KEY_M, MMIIM_SP_KEY_0}, 
#endif

    //附加按键配置                         
    {KEY_DEL, MMIIM_SP_KEY_DEL},

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)     
    {KEY_COMMA, MMIIM_SP_KEY_SEMICOLON},        //, ;
    {KEY_PERIOD, MMIIM_SP_KEY_COLON},           //. :
    {KEY_ENTER, MMIIM_SP_KEY_ENTER},
    {KEY_SPACE, MMIIM_SP_KEY_SPACE},
    {KEY_AND, MMIIM_SP_KEY_AND},
    {KEY_AT_QWERTY, MMIIM_SP_KEY_SLASH},
    {KEY_AT, MMIIM_SP_KEY_SLASH},   
    {KEY_QUESTION, MMIIM_SP_KEY_EXCLAMATION},
#endif

    //功能键
	{KEY_WEB, MMIIM_SP_KEY_OK},
    {KEY_OK, MMIIM_SP_KEY_OK},
    {KEY_CANCEL, MMIIM_SP_KEY_CANCEL},
    {KEY_UP, MMIIM_SP_KEY_UP},
    {KEY_DOWN, MMIIM_SP_KEY_DOWN},
    {KEY_LEFT, MMIIM_SP_KEY_LEFT},
    {KEY_RIGHT, MMIIM_SP_KEY_RIGHT},
};

#else       //QWERTY_KEYPAD_NOKIA及默认配置
LOCAL PHYSICAL_IM_KEY_MAPPING_T s_phy_im_key_map[] =    //正常按键映射表
{
    {KEY_0, MMIIM_SP_KEY_0}, 
    {KEY_1, MMIIM_SP_KEY_1}, 
    {KEY_2, MMIIM_SP_KEY_2}, 
    {KEY_3, MMIIM_SP_KEY_3}, 
    {KEY_4, MMIIM_SP_KEY_4}, 
    {KEY_5, MMIIM_SP_KEY_5}, 
    {KEY_6, MMIIM_SP_KEY_6}, 
    {KEY_7, MMIIM_SP_KEY_7}, 
    {KEY_8, MMIIM_SP_KEY_8}, 
    {KEY_9, MMIIM_SP_KEY_9}, 
    {KEY_STAR, MMIIM_SP_KEY_STAR}, 
    {KEY_HASH, MMIIM_SP_KEY_HASH}, 

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
    {KEY_Q, MMIIM_SP_KEY_Q}, 
    {KEY_W, MMIIM_SP_KEY_W}, 
    {KEY_E, MMIIM_SP_KEY_E}, 
    {KEY_R, MMIIM_SP_KEY_R}, 
    {KEY_T, MMIIM_SP_KEY_T}, 
    {KEY_Y, MMIIM_SP_KEY_Y}, 
    {KEY_U, MMIIM_SP_KEY_U}, 
    {KEY_I, MMIIM_SP_KEY_I}, 
    {KEY_O, MMIIM_SP_KEY_O}, 
    {KEY_P, MMIIM_SP_KEY_P}, 
    {KEY_A, MMIIM_SP_KEY_A}, 
    {KEY_S, MMIIM_SP_KEY_S}, 
    {KEY_D, MMIIM_SP_KEY_D}, 
    {KEY_F, MMIIM_SP_KEY_F}, 
    {KEY_G, MMIIM_SP_KEY_G}, 
    {KEY_H, MMIIM_SP_KEY_H}, 
    {KEY_J, MMIIM_SP_KEY_J}, 
    {KEY_K, MMIIM_SP_KEY_K}, 
    {KEY_L, MMIIM_SP_KEY_L}, 
    {KEY_Z, MMIIM_SP_KEY_Z}, 
    {KEY_X, MMIIM_SP_KEY_X}, 
    {KEY_C, MMIIM_SP_KEY_C}, 
    {KEY_V, MMIIM_SP_KEY_V}, 
    {KEY_B, MMIIM_SP_KEY_B}, 
    {KEY_N, MMIIM_SP_KEY_N}, 
    {KEY_M, MMIIM_SP_KEY_M}, 
#endif

    //字符按键，引擎基本上不处理        
    {KEY_DEL, MMIIM_SP_KEY_DEL},
    {KEY_SPACE, MMIIM_SP_KEY_SPACE},
    {KEY_SLASH, MMIIM_SP_KEY_SLASH},

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD) 
    {KEY_COMMA, MMIIM_SP_KEY_COMMA},
    {KEY_PERIOD, MMIIM_SP_KEY_PERIOD},
    {KEY_ENTER, MMIIM_SP_KEY_ENTER},
    {KEY_AT_QWERTY, MMIIM_SP_KEY_AT_QWERTY},
    {KEY_LEFT_PARENTHESIS, MMIIM_SP_KEY_LEFT_PARENTHESIS },
    {KEY_RIGHT_PARENTHESIS, MMIIM_SP_KEY_RIGHT_PARENTHESIS},
    {KEY_AND, MMIIM_SP_KEY_AND},
    {KEY_QUESTION, MMIIM_SP_KEY_QUESTION},
    {KEY_PLUS, MMIIM_SP_KEY_PLUS},
    
    {KEY_MINUS, MMIIM_SP_KEY_MINUS},
    {KEY_DOUBLE_QUOTES, MMIIM_SP_KEY_DOUBLE_QUOTES},
    {KEY_SEMICOLON, MMIIM_SP_KEY_SEMICOLON},
    {KEY_COLON, MMIIM_SP_KEY_COLON},
    {KEY_SHIFT_AND, MMIIM_SP_KEY_SHIFT_AND},
    {KEY_EXCLAMATION, MMIIM_SP_KEY_EXCLAMATION},
	{KEY_FN, MMIIM_SP_KEY_FN},		//sym键
#endif

    //功能键
	{KEY_WEB, MMIIM_SP_KEY_OK},
    {KEY_OK, MMIIM_SP_KEY_OK},
    {KEY_CANCEL, MMIIM_SP_KEY_CANCEL},
    {KEY_UP, MMIIM_SP_KEY_UP},
    {KEY_DOWN, MMIIM_SP_KEY_DOWN},
    {KEY_LEFT, MMIIM_SP_KEY_LEFT},
    {KEY_RIGHT, MMIIM_SP_KEY_RIGHT},
    {KEY_CTRL, MMIIM_SP_KEY_CTRL},        
};

//按下shift key对应的映射表
LOCAL PHYSICAL_IM_KEY_MAPPING_T s_phy_im_shift_key_map[] =  //按下shift key对应的映射表
{
    {KEY_0, MMIIM_SP_KEY_M}, 
    {KEY_1, MMIIM_SP_KEY_R}, 
    {KEY_2, MMIIM_SP_KEY_T}, 
    {KEY_3, MMIIM_SP_KEY_Y}, 
    {KEY_4, MMIIM_SP_KEY_F}, 
    {KEY_5, MMIIM_SP_KEY_G}, 
    {KEY_6, MMIIM_SP_KEY_H}, 
    {KEY_7, MMIIM_SP_KEY_V}, 
    {KEY_8, MMIIM_SP_KEY_B}, 
    {KEY_9, MMIIM_SP_KEY_N}, 
    {KEY_STAR, MMIIM_SP_KEY_STAR}, 
    {KEY_HASH, MMIIM_SP_KEY_HASH}, 

    //下面是标准26键，但是按键的shift上键存在客户定制可能
#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
    {KEY_R, MMIIM_SP_KEY_1}, 
    {KEY_T, MMIIM_SP_KEY_2}, 
    {KEY_Y, MMIIM_SP_KEY_3}, 
    {KEY_U, MMIIM_SP_KEY_STAR}, 
    {KEY_I, MMIIM_SP_KEY_PLUS}, 
    {KEY_O, MMIIM_SP_KEY_LEFT_PARENTHESIS},       
    {KEY_P, MMIIM_SP_KEY_RIGHT_PARENTHESIS},      
    {KEY_F, MMIIM_SP_KEY_4}, 
    {KEY_G, MMIIM_SP_KEY_5}, 
    {KEY_H, MMIIM_SP_KEY_6}, 
    {KEY_J, MMIIM_SP_KEY_HASH}, 
    {KEY_K, MMIIM_SP_KEY_MINUS}, 
    {KEY_V, MMIIM_SP_KEY_7}, 
    {KEY_B, MMIIM_SP_KEY_8}, 
    {KEY_N, MMIIM_SP_KEY_9}, 
    {KEY_M, MMIIM_SP_KEY_0}, 
#endif

    //附加按键配置                         
    {KEY_DEL, MMIIM_SP_KEY_DEL},

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)     
    {KEY_COMMA, MMIIM_SP_KEY_SEMICOLON},        //, ;
    {KEY_PERIOD, MMIIM_SP_KEY_COLON},           //. : 
    {KEY_ENTER, MMIIM_SP_KEY_ENTER},
    {KEY_SPACE, MMIIM_SP_KEY_SPACE},
    {KEY_AND, MMIIM_SP_KEY_AND},
    {KEY_AT_QWERTY, MMIIM_SP_KEY_AT_QWERTY},
	{KEY_EXCLAMATION, MMIIM_SP_KEY_QUESTION},	//! ?
#endif

    //功能键
	{KEY_WEB, MMIIM_SP_KEY_OK},
    {KEY_OK, MMIIM_SP_KEY_OK},
    {KEY_CANCEL, MMIIM_SP_KEY_CANCEL},
    {KEY_UP, MMIIM_SP_KEY_UP},
    {KEY_DOWN, MMIIM_SP_KEY_DOWN},
    {KEY_LEFT, MMIIM_SP_KEY_LEFT},
    {KEY_RIGHT, MMIIM_SP_KEY_RIGHT},
};
#endif

LOCAL S_LANG_KEY_TABLE s_lang_keytable;

#if defined IM_ENGINE_CSTAR
static MMIIM_CSTAR_METHOD_RELATION_T const g_cstar_method_relation_list[] =
{
    //物理9键
#ifdef DPHONE_SUPPORT 
    #ifndef KEYBOARD_SUPPORT_LOW_MEMORY
    {GUIIM_M_PINYIN,                        IM_PINYIN},
    #else
    {GUIIM_M_PINYIN,                        IM_PINYIN_DZ},
    #endif
#else
#if defined(MMI_IM_PDA_SUPPORT)
    {GUIIM_M_PINYIN,                        IM_PINYIN},
#else
    #ifdef INPUT_METHOD_CSTAR_COMMON_MINI
    {GUIIM_M_PINYIN,                        IM_PINYIN_DZ},
    #else
    {GUIIM_M_PINYIN,                        IM_PINYIN},
    #endif
#endif
#endif
    {GUIIM_M_STROKE,                        IM_STROKE},
    {GUIIM_M_ZHUYIN,                        IM_ZHUYIN_MT},
    {GUIIM_M_STROKE_CHT,                    IM_STROKE_BIG5},
    {GUIIM_M_123,                           IM_123},
    {GUIIM_M_ENGLISH_LEAD_UPPER,            IM_ENGLISH_LEAD_UPPER},
    {GUIIM_M_ENGLISH_LOWER,                 IM_ENGLISH_LOWER},
    {GUIIM_M_ENGLISH_UPPER,                 IM_ENGLISH_UPPER},
    {GUIIM_M_ENGLISH_ABC_LEAD_UPPER,        IM_ABC_LEAD_UPPER},
    {GUIIM_M_ENGLISH_ABC_LOWER,             IM_ABC_LOWER},
    {GUIIM_M_ENGLISH_ABC_UPPER,             IM_ABC_UPPER},
    {GUIIM_M_ARABIC,                        IM_ARABIC},
    {GUIIM_M_FRENCH_LEAD_UPPER,             IM_FRENCH_LEAD_UPPER},
    {GUIIM_M_FRENCH_LOWER,                  IM_FRENCH_LOWER},
    {GUIIM_M_FRENCH_UPPER,                  IM_FRENCH_UPPER},
    {GUIIM_M_GERMAN_LEAD_UPPER,             IM_GERMAN_LEAD_UPPER},
    {GUIIM_M_GERMAN_LOWER,                  IM_GERMAN_LOWER},
    {GUIIM_M_GERMAN_UPPER,                  IM_GERMAN_UPPER},
    {GUIIM_M_GREEK_LEAD_UPPER,              IM_GREEK_LEAD_UPPER},
    {GUIIM_M_GREEK_LOWER,                   IM_GREEK_LOWER},
    {GUIIM_M_GREEK_UPPER,                   IM_GREEK_UPPER},
    {GUIIM_M_HINDI,                         IM_HINDI},
    {GUIIM_M_HUNGARIAN_LEAD_UPPER,          IM_HUNGARIAN_LEAD_UPPER},
    {GUIIM_M_HUNGARIAN_LOWER,               IM_HUNGARIAN_LOWER},
    {GUIIM_M_HUNGARIAN_UPPER,               IM_HUNGARIAN_UPPER},
    {GUIIM_M_INDONESIAN_LEAD_UPPER,         IM_INDONESIAN_LEAD_UPPER},
    {GUIIM_M_INDONESIAN_LOWER,              IM_INDONESIAN_LOWER},
    {GUIIM_M_INDONESIAN_UPPER,              IM_INDONESIAN_UPPER},
    {GUIIM_M_ITALIAN_LEAD_UPPER,            IM_ITALIAN_LEAD_UPPER},
    {GUIIM_M_ITALIAN_LOWER,                 IM_ITALIAN_LOWER},
    {GUIIM_M_ITALIAN_UPPER,                 IM_ITALIAN_UPPER},
    {GUIIM_M_MALAY_LEAD_UPPER,              IM_MALAY_LEAD_UPPER},
    {GUIIM_M_MALAY_LOWER,                   IM_MALAY_LOWER},
    {GUIIM_M_MALAY_UPPER,                   IM_MALAY_UPPER},
    {GUIIM_M_PERSIAN,                       IM_PERSIAN},
    {GUIIM_M_PORTUGUESE_LEAD_UPPER,         IM_PORTUGUESE_LEAD_UPPER},
    {GUIIM_M_PORTUGUESE_LOWER,              IM_PORTUGUESE_LOWER},
    {GUIIM_M_PORTUGUESE_UPPER,              IM_PORTUGUESE_UPPER},
    {GUIIM_M_RUSSIAN_LEAD_UPPER,            IM_RUSSIAN_LEAD_UPPER},
    {GUIIM_M_RUSSIAN_LOWER,                 IM_RUSSIAN_LOWER},
    {GUIIM_M_RUSSIAN_UPPER,                 IM_RUSSIAN_UPPER},
    {GUIIM_M_SPANISH_LEAD_UPPER,            IM_SPANISH_LEAD_UPPER},
    {GUIIM_M_SPANISH_LOWER,                 IM_SPANISH_LOWER},
    {GUIIM_M_SPANISH_UPPER,                 IM_SPANISH_UPPER},
    {GUIIM_M_TAGALOG,                       IM_TAGALOG},
    {GUIIM_M_THAI,                          IM_THAI},
    {GUIIM_M_TURKISH_LEAD_UPPER,            IM_TURKISH_LEAD_UPPER},
    {GUIIM_M_TURKISH_LOWER,                 IM_TURKISH_LOWER},
    {GUIIM_M_TURKISH_UPPER,                 IM_TURKISH_UPPER},
    {GUIIM_M_URDU,                          IM_URDU},
    {GUIIM_M_HEBREW,                        IM_HEBREW},
    {GUIIM_M_BENGALI,                       IM_BENGALI},
    {GUIIM_M_VIETNAMESE_LEAD_UPPER,         IM_VIETNAMESE_LEAD_UPPER},
    {GUIIM_M_VIETNAMESE_LOWER,              IM_VIETNAMESE_LOWER},
    {GUIIM_M_VIETNAMESE_UPPER,              IM_VIETNAMESE_UPPER},
    {GUIIM_M_CZECH_LEAD_UPPER,              IM_CZECH_LEAD_UPPER},
    {GUIIM_M_CZECH_LOWER,                   IM_CZECH_LOWER},
    {GUIIM_M_CZECH_UPPER,                   IM_CZECH_UPPER},
    {GUIIM_M_SLOVENIAN_LEAD_UPPER,          IM_SLOVENIAN_LEAD_UPPER},
    {GUIIM_M_SLOVENIAN_LOWER,               IM_SLOVENIAN_LOWER},
    {GUIIM_M_SLOVENIAN_UPPER,               IM_SLOVENIAN_UPPER},
    {GUIIM_M_ROMANIAN_LEAD_UPPER,           IM_ROMANIAN_LEAD_UPPER},
    {GUIIM_M_ROMANIAN_LOWER,                IM_ROMANIAN_LOWER},
    {GUIIM_M_ROMANIAN_UPPER,                IM_ROMANIAN_UPPER},
    {GUIIM_M_CROATIAN_LEAD_UPPER,           IM_CROATIAN_LEAD_UPPER},
    {GUIIM_M_CROATIAN_LOWER,                IM_CROATIAN_LOWER},
    {GUIIM_M_CROATIAN_UPPER,                IM_CROATIAN_UPPER},
    {GUIIM_M_DUTCH_LEAD_UPPER,              IM_DUTCH_LEAD_UPPER},
    {GUIIM_M_DUTCH_LOWER,                   IM_DUTCH_LOWER},
    {GUIIM_M_DUTCH_UPPER,                   IM_DUTCH_UPPER},
    {GUIIM_M_BULGARIAN_LEAD_UPPER,          IM_BULGARIAN_LEAD_UPPER},
    {GUIIM_M_BULGARIAN_LOWER,               IM_BULGARIAN_LOWER},
    {GUIIM_M_BULGARIAN_UPPER,               IM_BULGARIAN_UPPER},
    {GUIIM_M_POLISH_LEAD_UPPER,             IM_POLISH_LEAD_UPPER},
    {GUIIM_M_POLISH_LOWER,                  IM_POLISH_LOWER},
    {GUIIM_M_POLISH_UPPER,                  IM_POLISH_UPPER},
    {GUIIM_M_DANISH_LEAD_UPPER,             IM_DANISH_LEAD_UPPER},
    {GUIIM_M_DANISH_LOWER,                  IM_DANISH_LOWER},
    {GUIIM_M_DANISH_UPPER,                  IM_DANISH_UPPER},
    {GUIIM_M_SLOVAK_LEAD_UPPER,             IM_SLOVAK_LEAD_UPPER},
    {GUIIM_M_SLOVAK_LOWER,                  IM_SLOVAK_LOWER},
    {GUIIM_M_SLOVAK_UPPER,                  IM_SLOVAK_UPPER},
    {GUIIM_M_LITHUANIAN_LEAD_UPPER,         IM_LITHUANIAN_LEAD_UPPER},
    {GUIIM_M_LITHUANIAN_LOWER,              IM_LITHUANIAN_LOWER},
    {GUIIM_M_LITHUANIAN_UPPER,              IM_LITHUANIAN_UPPER},
    {GUIIM_M_NORWEGIAN_LEAD_UPPER,          IM_NORWEGIAN_LEAD_UPPER},
    {GUIIM_M_NORWEGIAN_LOWER,               IM_NORWEGIAN_LOWER},
    {GUIIM_M_NORWEGIAN_UPPER,               IM_NORWEGIAN_UPPER},
    {GUIIM_M_SWEDISH_LEAD_UPPER,            IM_SWEDISH_LEAD_UPPER},
    {GUIIM_M_SWEDISH_LOWER,                 IM_SWEDISH_LOWER},
    {GUIIM_M_SWEDISH_UPPER,                 IM_SWEDISH_UPPER},
    {GUIIM_M_UKRAINIAN_LEAD_UPPER,          IM_UKRAINIAN_LEAD_UPPER},
    {GUIIM_M_UKRAINIAN_LOWER,               IM_UKRAINIAN_LOWER},
    {GUIIM_M_UKRAINIAN_UPPER,               IM_UKRAINIAN_UPPER},
    {GUIIM_M_SWAHILI_LEAD_UPPER,            IM_SWAHILI_LEAD_UPPER},
    {GUIIM_M_SWAHILI_LOWER,                 IM_SWAHILI_LOWER},
    {GUIIM_M_SWAHILI_UPPER,                 IM_SWAHILI_UPPER},
    {GUIIM_M_FINNISH_LEAD_UPPER,            IM_FINNISH_LEAD_UPPER},
    {GUIIM_M_FINNISH_LOWER,                 IM_FINNISH_LOWER},
    {GUIIM_M_FINNISH_UPPER,                 IM_FINNISH_UPPER},
    {GUIIM_M_UYGHUR,                        IM_UYGHUR},
        

    //物理26键
#ifdef DPHONE_SUPPORT 
    #ifndef KEYBOARD_SUPPORT_LOW_MEMORY
    {GUIIM_M_Q_PINYIN,                        IM_FULLKEY_BASE + IM_PINYIN},
    #else
    {GUIIM_M_Q_PINYIN,                        IM_FULLKEY_BASE + IM_PINYIN_DZ},
    #endif
#else
#if defined(MMI_IM_PDA_SUPPORT)
    {GUIIM_M_Q_PINYIN,                        IM_FULLKEY_BASE + IM_PINYIN},
#else
    #ifdef INPUT_METHOD_CSTAR_COMMON_MINI
    {GUIIM_M_Q_PINYIN,                        IM_FULLKEY_BASE + IM_PINYIN_DZ},
    #else
    {GUIIM_M_Q_PINYIN,                        IM_FULLKEY_BASE + IM_PINYIN},
    #endif
#endif
#endif
    
    {GUIIM_M_Q_STROKE,                      IM_STROKE},//比较特殊
    {GUIIM_M_Q_ZHUYIN,                      IM_ZHUYIN + IM_FULLKEY_BASE},
    {GUIIM_M_Q_STROKE_CHT,                  IM_STROKE_BIG5},
    {GUIIM_M_Q_123,                         IM_FULLKEY_BASE + IM_123},
    {GUIIM_M_Q_ENGLISH_LEAD_UPPER,          IM_FULLKEY_BASE + IM_ENGLISH_LEAD_UPPER},
    {GUIIM_M_Q_ENGLISH_LOWER,               IM_FULLKEY_BASE + IM_ENGLISH_LOWER},
    {GUIIM_M_Q_ENGLISH_UPPER,               IM_FULLKEY_BASE + IM_ENGLISH_UPPER},
    {GUIIM_M_Q_ENGLISH_ABC_LEAD_UPPER,      IM_FULLKEY_BASE + IM_ABC_LEAD_UPPER},
    {GUIIM_M_Q_ENGLISH_ABC_LOWER,           IM_FULLKEY_BASE + IM_ABC_LOWER},
    {GUIIM_M_Q_ENGLISH_ABC_UPPER,           IM_FULLKEY_BASE + IM_ABC_UPPER},
    {GUIIM_M_Q_ARABIC,                      IM_FULLKEY_BASE + IM_ARABIC},
    {GUIIM_M_Q_FRENCH_LEAD_UPPER,           IM_FULLKEY_BASE + IM_FRENCH_LEAD_UPPER},
    {GUIIM_M_Q_FRENCH_LOWER,                IM_FULLKEY_BASE + IM_FRENCH_LOWER},
    {GUIIM_M_Q_FRENCH_UPPER,                IM_FULLKEY_BASE + IM_FRENCH_UPPER},
    {GUIIM_M_Q_GERMAN_LEAD_UPPER,           IM_FULLKEY_BASE + IM_GERMAN_LEAD_UPPER},
    {GUIIM_M_Q_GERMAN_LOWER,                IM_FULLKEY_BASE + IM_GERMAN_LOWER},
    {GUIIM_M_Q_GERMAN_UPPER,                IM_FULLKEY_BASE + IM_GERMAN_UPPER},
    {GUIIM_M_Q_GREEK_LEAD_UPPER,            IM_FULLKEY_BASE + IM_GREEK_LEAD_UPPER},
    {GUIIM_M_Q_GREEK_LOWER,                 IM_FULLKEY_BASE + IM_GREEK_LOWER},
    {GUIIM_M_Q_GREEK_UPPER,                 IM_FULLKEY_BASE + IM_GREEK_UPPER},
    {GUIIM_M_Q_HINDI,                       IM_FULLKEY_BASE + IM_HINDI},
    {GUIIM_M_Q_HUNGARIAN_LEAD_UPPER,        IM_FULLKEY_BASE + IM_HUNGARIAN_LEAD_UPPER},
    {GUIIM_M_Q_HUNGARIAN_LOWER,             IM_FULLKEY_BASE + IM_HUNGARIAN_LOWER},
    {GUIIM_M_Q_HUNGARIAN_UPPER,             IM_FULLKEY_BASE + IM_HUNGARIAN_UPPER},
    {GUIIM_M_Q_INDONESIAN_LEAD_UPPER,       IM_FULLKEY_BASE + IM_INDONESIAN_LEAD_UPPER},
    {GUIIM_M_Q_INDONESIAN_LOWER,            IM_FULLKEY_BASE + IM_INDONESIAN_LOWER},
    {GUIIM_M_Q_INDONESIAN_UPPER,            IM_FULLKEY_BASE + IM_INDONESIAN_UPPER},
    {GUIIM_M_Q_ITALIAN_LEAD_UPPER,          IM_FULLKEY_BASE + IM_ITALIAN_LEAD_UPPER},
    {GUIIM_M_Q_ITALIAN_LOWER,               IM_FULLKEY_BASE + IM_ITALIAN_LOWER},
    {GUIIM_M_Q_ITALIAN_UPPER,               IM_FULLKEY_BASE + IM_ITALIAN_UPPER},
    {GUIIM_M_Q_MALAY_LEAD_UPPER,            IM_FULLKEY_BASE + IM_MALAY_LEAD_UPPER},
    {GUIIM_M_Q_MALAY_LOWER,                 IM_FULLKEY_BASE + IM_MALAY_LOWER},
    {GUIIM_M_Q_MALAY_UPPER,                 IM_FULLKEY_BASE + IM_MALAY_UPPER},
    {GUIIM_M_Q_PERSIAN,                     IM_FULLKEY_BASE + IM_PERSIAN},
    {GUIIM_M_Q_PORTUGUESE_LEAD_UPPER,       IM_FULLKEY_BASE + IM_PORTUGUESE_LEAD_UPPER},
    {GUIIM_M_Q_PORTUGUESE_LOWER,            IM_FULLKEY_BASE + IM_PORTUGUESE_LOWER},
    {GUIIM_M_Q_PORTUGUESE_UPPER,            IM_FULLKEY_BASE + IM_PORTUGUESE_UPPER},
    {GUIIM_M_Q_RUSSIAN_LEAD_UPPER,          IM_FULLKEY_BASE + IM_RUSSIAN_LEAD_UPPER},
    {GUIIM_M_Q_RUSSIAN_LOWER,               IM_FULLKEY_BASE + IM_RUSSIAN_LOWER},
    {GUIIM_M_Q_RUSSIAN_UPPER,               IM_FULLKEY_BASE + IM_RUSSIAN_UPPER},
    {GUIIM_M_Q_SPANISH_LEAD_UPPER,          IM_FULLKEY_BASE + IM_SPANISH_LEAD_UPPER},
    {GUIIM_M_Q_SPANISH_LOWER,               IM_FULLKEY_BASE + IM_SPANISH_LOWER},
    {GUIIM_M_Q_SPANISH_UPPER,               IM_FULLKEY_BASE + IM_SPANISH_UPPER},
    {GUIIM_M_Q_TAGALOG,                     IM_FULLKEY_BASE + IM_TAGALOG},
    {GUIIM_M_Q_THAI,                        IM_FULLKEY_BASE + IM_THAI},
    {GUIIM_M_Q_TURKISH_LEAD_UPPER,          IM_FULLKEY_BASE + IM_TURKISH_LEAD_UPPER},
    {GUIIM_M_Q_TURKISH_LOWER,               IM_FULLKEY_BASE + IM_TURKISH_LOWER},
    {GUIIM_M_Q_TURKISH_UPPER,               IM_FULLKEY_BASE + IM_TURKISH_UPPER},
    {GUIIM_M_Q_URDU,                        IM_FULLKEY_BASE + IM_URDU},
    {GUIIM_M_Q_HEBREW,                      IM_FULLKEY_BASE + IM_HEBREW},
    {GUIIM_M_Q_BENGALI,                     IM_FULLKEY_BASE + IM_BENGALI},
    {GUIIM_M_Q_VIETNAMESE_LEAD_UPPER,       IM_FULLKEY_BASE + IM_VIETNAMESE_LEAD_UPPER},
    {GUIIM_M_Q_VIETNAMESE_LOWER,            IM_FULLKEY_BASE + IM_VIETNAMESE_LOWER},
    {GUIIM_M_Q_VIETNAMESE_UPPER,            IM_FULLKEY_BASE + IM_VIETNAMESE_UPPER},
    {GUIIM_M_Q_CZECH_LEAD_UPPER,            IM_FULLKEY_BASE + IM_CZECH_LEAD_UPPER},
    {GUIIM_M_Q_CZECH_LOWER,                 IM_FULLKEY_BASE + IM_CZECH_LOWER},
    {GUIIM_M_Q_CZECH_UPPER,                 IM_FULLKEY_BASE + IM_CZECH_UPPER},
    {GUIIM_M_Q_SLOVENIAN_LEAD_UPPER,        IM_FULLKEY_BASE + IM_SLOVENIAN_LEAD_UPPER},
    {GUIIM_M_Q_SLOVENIAN_LOWER,             IM_FULLKEY_BASE + IM_SLOVENIAN_LOWER},
    {GUIIM_M_Q_SLOVENIAN_UPPER,             IM_FULLKEY_BASE + IM_SLOVENIAN_UPPER},
    {GUIIM_M_Q_ROMANIAN_LEAD_UPPER,         IM_FULLKEY_BASE + IM_ROMANIAN_LEAD_UPPER},
    {GUIIM_M_Q_ROMANIAN_LOWER,              IM_FULLKEY_BASE + IM_ROMANIAN_LOWER},
    {GUIIM_M_Q_ROMANIAN_UPPER,              IM_FULLKEY_BASE + IM_ROMANIAN_UPPER},
    {GUIIM_M_Q_DUTCH_LEAD_UPPER,            IM_FULLKEY_BASE + IM_DUTCH_LEAD_UPPER},
    {GUIIM_M_Q_CROATIAN_LOWER,              IM_FULLKEY_BASE + IM_CROATIAN_LOWER},
    {GUIIM_M_Q_CROATIAN_UPPER,              IM_FULLKEY_BASE + IM_CROATIAN_UPPER},
    {GUIIM_M_Q_CROATIAN_LEAD_UPPER,         IM_FULLKEY_BASE + IM_CROATIAN_LEAD_UPPER},
    {GUIIM_M_Q_DUTCH_LOWER,                 IM_FULLKEY_BASE + IM_DUTCH_LOWER},
    {GUIIM_M_Q_DUTCH_UPPER,                 IM_FULLKEY_BASE + IM_DUTCH_UPPER},
    {GUIIM_M_Q_BULGARIAN_LEAD_UPPER,        IM_FULLKEY_BASE + IM_BULGARIAN_LEAD_UPPER},
    {GUIIM_M_Q_BULGARIAN_LOWER,             IM_FULLKEY_BASE + IM_BULGARIAN_LOWER},
    {GUIIM_M_Q_BULGARIAN_UPPER,             IM_FULLKEY_BASE + IM_BULGARIAN_UPPER},
    {GUIIM_M_Q_POLISH_LEAD_UPPER,           IM_FULLKEY_BASE + IM_POLISH_LEAD_UPPER},
    {GUIIM_M_Q_POLISH_LOWER,                IM_FULLKEY_BASE + IM_POLISH_LOWER},
    {GUIIM_M_Q_POLISH_UPPER,                IM_FULLKEY_BASE + IM_POLISH_UPPER},
    {GUIIM_M_Q_DANISH_LEAD_UPPER,           IM_FULLKEY_BASE + IM_DANISH_LEAD_UPPER},
    {GUIIM_M_Q_DANISH_LOWER,                IM_FULLKEY_BASE + IM_DANISH_LOWER},
    {GUIIM_M_Q_DANISH_UPPER,                IM_FULLKEY_BASE + IM_DANISH_UPPER},
    {GUIIM_M_Q_SLOVAK_LEAD_UPPER,           IM_FULLKEY_BASE + IM_SLOVAK_LEAD_UPPER},
    {GUIIM_M_Q_SLOVAK_LOWER,                IM_FULLKEY_BASE + IM_SLOVAK_LOWER},
    {GUIIM_M_Q_SLOVAK_UPPER,                IM_FULLKEY_BASE + IM_SLOVAK_UPPER},
    {GUIIM_M_Q_LITHUANIAN_LEAD_UPPER,       IM_FULLKEY_BASE + IM_LITHUANIAN_LEAD_UPPER},
    {GUIIM_M_Q_LITHUANIAN_LOWER,            IM_FULLKEY_BASE + IM_LITHUANIAN_LOWER},
    {GUIIM_M_Q_LITHUANIAN_UPPER,            IM_FULLKEY_BASE + IM_LITHUANIAN_UPPER},
    {GUIIM_M_Q_NORWEGIAN_LEAD_UPPER,        IM_FULLKEY_BASE + IM_NORWEGIAN_LEAD_UPPER},
    {GUIIM_M_Q_NORWEGIAN_LOWER,             IM_FULLKEY_BASE + IM_NORWEGIAN_LOWER},
    {GUIIM_M_Q_NORWEGIAN_UPPER,             IM_FULLKEY_BASE + IM_NORWEGIAN_UPPER},
    {GUIIM_M_Q_SWEDISH_LEAD_UPPER,          IM_FULLKEY_BASE + IM_SWEDISH_LEAD_UPPER},
    {GUIIM_M_Q_SWEDISH_LOWER,               IM_FULLKEY_BASE + IM_SWEDISH_LOWER},
    {GUIIM_M_Q_SWEDISH_UPPER,               IM_FULLKEY_BASE + IM_SWEDISH_UPPER},  
    {GUIIM_M_Q_UKRAINIAN_LEAD_UPPER,        IM_FULLKEY_BASE + IM_UKRAINIAN_LEAD_UPPER},
    {GUIIM_M_Q_UKRAINIAN_LOWER,             IM_FULLKEY_BASE + IM_UKRAINIAN_LOWER},
    {GUIIM_M_Q_UKRAINIAN_UPPER,             IM_FULLKEY_BASE + IM_UKRAINIAN_UPPER},
    {GUIIM_M_Q_SWAHILI_LEAD_UPPER,          IM_FULLKEY_BASE + IM_SWAHILI_LEAD_UPPER},
    {GUIIM_M_Q_SWAHILI_LOWER,               IM_FULLKEY_BASE + IM_SWAHILI_LOWER},
    {GUIIM_M_Q_SWAHILI_UPPER,               IM_FULLKEY_BASE + IM_SWAHILI_UPPER},
    {GUIIM_M_Q_FINNISH_LEAD_UPPER,          IM_FULLKEY_BASE + IM_FINNISH_LEAD_UPPER},
    {GUIIM_M_Q_FINNISH_LOWER,               IM_FULLKEY_BASE + IM_FINNISH_LOWER},
    {GUIIM_M_Q_FINNISH_UPPER,               IM_FULLKEY_BASE + IM_FINNISH_UPPER},
    {GUIIM_M_Q_UYGHUR,                      IM_FULLKEY_BASE + IM_UYGHUR},
};
#endif

#ifdef MMI_CSTAR_UDB_SUPPORT
#define MMIIM_UDB_CSTAR_LEN     (2048)
LOCAL UDB_ENTRY_T s_udb_entry = {0};
LOCAL const wchar s_cstar_udb_dir[] = {'@','c','s','t','a','r','d','a','t','a',0};
LOCAL const wchar s_cstar_udb_file[] = {'u','d','b','.','c','a','c','h','e', 0};
//记录当前手机编译时间,将编译时间与当前T卡上的配置文件时间绑定，防止使用非本机的版本文件导致死机
LOCAL const wchar s_cstar_compile_time[] = {'c','o','m', 'p', 'i', 'l', 'e', '.', 't', 'i', 'm', 'e', 0};	


//所有的输入法均需要添加UDB数据长度，虽然某些输入法没有UDB功能
//原因是:CSTAR的引擎库会检索cs_input_method_config来判断开启了那些输入法
//然后根据输入法种类为每种输入法分配UDB内存段空间，但是CSTAR的引擎并没有考虑那种语言没有UDB
//在调用函数 FindUserCacheByIM 时，发现内存空间不足而失败
//118049 - [ST][7701][320X480_PDA_32MB]输入法无记忆词组功能，MOCOR_W12.52版本上是支持的    #因为新版本开启了其他语言导致内存长度不足
LOCAL const UDB_LANG_CONFIG_T s_udb_lang_config[] =
{
    {GUIIM_LANG_ENGLISH, MMIIM_UDB_CSTAR_LEN},

#if defined(IM_SIMP_CHINESE_SUPPORT)
    {GUIIM_LANG_CHS, (3*MMIIM_UDB_CSTAR_LEN)},
#endif

#if defined(IM_TRAD_CHINESE_SUPPORT)
    {GUIIM_LANG_CHT, (5*MMIIM_UDB_CSTAR_LEN)},
#endif

#if defined(IM_ARABIC_SUPPORT)
    {GUIIM_LANG_ARABIC, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_HINDI_SUPPORT)
    {GUIIM_LANG_HINDI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_THAI_SUPPORT)
    {GUIIM_LANG_THAI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_RUSSIAN_SUPPORT)
    {GUIIM_LANG_RUSSIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_GERMAN_SUPPORT)
    {GUIIM_LANG_GERMAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_VIETNAMESE_SUPPORT)
    {GUIIM_LANG_VIETNAMESE, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_INDONESIAN_SUPPORT)
    {GUIIM_LANG_INDONESIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_TURKISH_SUPPORT)
    {GUIIM_LANG_TURKISH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_MALAY_SUPPORT)
    {GUIIM_LANG_MALAY, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_GREEK_SUPPORT)
    {GUIIM_LANG_GREEK, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_DANISH_SUPPORT)
    {GUIIM_LANG_DANISH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_DUTCH_SUPPORT)
    {GUIIM_LANG_DUTCH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_SWAHILI_SUPPORT)
    {GUIIM_LANG_SWAHILI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_POLISH_SUPPORT)
    {GUIIM_LANG_POLISH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_FRENCH_SUPPORT)
    {GUIIM_LANG_FRENCH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_URDU_SUPPORT)
    {GUIIM_LANG_URDU, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_HUNGARIAN_SUPPORT)
    {GUIIM_LANG_HUNGARIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_HEBREW_SUPPORT)
    {GUIIM_LANG_HEBREW, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_BENGALI_SUPPORT)
    {GUIIM_LANG_BENGALI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_BULGARIAN_SUPPORT)
    {GUIIM_LANG_BULGARIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_CZECH_SUPPORT)
    {GUIIM_LANG_CZECH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_FINNISH_SUPPORT)
    {GUIIM_LANG_FINNISH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_PERSIAN_SUPPORT)
    {GUIIM_LANG_PERSIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_SLOVAK_SUPPORT)
    {GUIIM_LANG_SLOVAK, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_NORWEGIAN_SUPPORT)
    {GUIIM_LANG_NORWEGIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_SWEDISH_SUPPORT)
    {GUIIM_LANG_SWEDISH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_SPANISH_SUPPORT)
    {GUIIM_LANG_SPANISH, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_ROMANIAN_SUPPORT)
    {GUIIM_LANG_ROMANIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_SLOVENIAN_SUPPORT)
    {GUIIM_LANG_SLOVENIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_PORTUGUESE_SUPPORT)
    {GUIIM_LANG_PORTUGUESE, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_ITALIAN_SUPPORT)
    {GUIIM_LANG_ITALIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_TAGALOG_SUPPORT)
    {GUIIM_LANG_TAGALOG, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_LITHUANIAN_SUPPORT)
    {GUIIM_LANG_LITHUANIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_UKRAINIAN_SUPPORT)
    {GUIIM_LANG_UKRAINIAN, MMIIM_UDB_CSTAR_LEN},
#endif
#if defined(IM_TELUGU_SUPPORT)
    {GUIIM_LANG_TELUGU, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_MARATHI_SUPPORT)
    {GUIIM_LANG_MARATHI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_TAMIL_SUPPORT)
    {GUIIM_LANG_TAMIL, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_PUNJABI_SUPPORT)
    {GUIIM_LANG_PUNJABI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_GUJARATI_SUPPORT)
    {GUIIM_LANG_GUJARATI, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_ORIYA_SUPPORT)
    {GUIIM_LANG_ORIYA, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_KANNADA_SUPPORT)
    {GUIIM_LANG_KANNADA, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_MALAYALAM_SUPPORT)
    {GUIIM_LANG_MALAYALAM, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_MYANMAR_SUPPORT)
    {GUIIM_LANG_MYANMAR, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_KHMER_SUPPORT)
    {GUIIM_LANG_KHMER, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_LAO_SUPPORT)
    {GUIIM_LANG_LAO, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_AMHARIC_SUPPORT)
   {GUIIM_LANG_AMHARIC, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_ARMENIAN_SUPPORT)
    {GUIIM_LANG_ARMENIAN, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_UYGHUR_SUPPORT)
    {GUIIM_LANG_UYGHUR, MMIIM_UDB_CSTAR_LEN},
#endif

#if defined(IM_TIBETAN_SUPPORT)
	{GUIIM_LANG_TIBETAN, MMIIM_UDB_CSTAR_LEN},
#endif

};
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
extern VOCABULARY_UPDATE_FREQUENCY_T g_sogou_vocabulary_update_frequency_data;
extern SYSTEM_UPDATE_SETTINGS_DATA_T g_sogou_update_settings_data;
extern MMISG_SORT_TYPE_E g_sg_pic_sort_type;
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
//输入法重入的时候，用于检查控制的控制量
#if ((defined IM_ENGINE_CSTAR) || (defined IM_ENGINE_T9))
LOCAL uint16 s_im_kernal_initialized = 0;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化多国语引擎
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitMlData(
                         MMISET_LANGUAGE_TYPE_E     set_lang_index, // [in] 输入语言
                         BOOLEAN                    is_26key,       // [in] 是否是26键键盘
                         MMIIM_SP_ML_DATA_T         *im_ml_data1_ptr, // [out]
                         MMIIM_SP_ML_DATA_T         *im_ml_data2_ptr  // [out]
                         );

/*****************************************************************************/
//  Description : 设置多国语的语言
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL GUIIM_METHOD_CFG_T* SetMultiLangMethod(
                                 MMIIM_SP_ML_DATA_T     *im_ml_data1_ptr,   // [in]
                                 uint16                 im_lang,            // [in] 输入语言
                                 BOOLEAN                is_26key,           // [in] 是否是26键键盘
                                 GUIIM_CAPITAL_MODE_T   cap_mode            // [in] 大小写
                                 );

/*****************************************************************************/
//  Description : 从字符索引转换到内部索引
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL MMIIM_SP_KEY_INDEX_T GetKeyIndexFromChar(
                                               BOOLEAN  is_26key,
                                               uint16   key_char
                                               );

/*****************************************************************************/
//  Description : 根据所设的语言及字符，获得该字符所在的键位
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN   GetKeyIndexByLan(         // [ret]输入字符是否在该语言中找到
                                  uint16                  *key_index,//[OUT] 键位值
								  MMISET_LANGUAGE_TYPE_E   set_lang_index, // [in] 输入语言
                                  BOOLEAN                  is_26key,       // [in] 是否是26键键盘
                                  uint16                   input_char,     // [in] 输入法需要查询的字符
								  MMIIM_SP_ML_DATA_T       *im_ml_data_ptr // [in] 输入法数据表
                                   );

/*****************************************************************************/
//  Description : 由内部索引转换到字符索引
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL MMIIM_SP_KEY_INDEX_T GetKeyCharFromIndex(
                                               BOOLEAN  is_26key,
                                               uint16   key_index
                                               );
/*****************************************************************************/
//  Description : 读写MMINV_IM_DEFAULT_METHOD时需要对当前操作的method做校验
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note: 因NV被改写导致仅显示数字VKB，这里需要对NV中存储的method做个检验，如果
//		是123类别，则返回false，以供后续处理
/*****************************************************************************/
LOCAL BOOLEAN IsDefaultMethodAllowedForPDA(
									GUIIM_METHOD_T method
                                               );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init im module
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_InitModule(void)
{
    //register menu
    MMIIM_RegMenuGroup();

    //register nv
    MMIIM_RegNv();
    
    //register win id name
    MMIIM_RegWinIdNameArr();    
}

/*****************************************************************************/
//  Description : init im nv
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_InitNv(void)
{
    uint16                  pinyin_fuzzy = MMIIM_DEFAULT_PY_FUZZY;
    GUI_COLOR_T             hw_color = MMI_DARK_BLUE_COLOR;
    MMIIM_HW_SPEED_E        hw_speed = MMIIM_HW_SPEED_NORMAL;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMIIM_HW_THICKNESS_E    hw_thickness = MMIIM_HW_THICKNESS_NORMAL;
    GUIIM_METHOD_T          method = GUIIM_M_NONE;
	GUIIM_LANGUAGE_T		input_language = GUIIM_LANG_NONE;
    BOOLEAN                 is_word_predic = FALSE;

	uint16					keyboard_type = 1;

#ifdef MMI_IME_USER_DATABASE
    uint8                   *alpha_udb_ptr = PNULL;
#ifdef IM_SIMP_CHINESE_SUPPORT
    uint8 *chs_udb_ptr = PNULL;
#endif
#ifdef IM_TRAD_CHINESE_SUPPORT
    uint8 *cht_udb_ptr = PNULL;
#endif 
#endif

    //init pinyin fuzzy
    MMINV_READ(MMINV_IM_PINYIN_FUZZY,&pinyin_fuzzy,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_PINYIN_FUZZY,&pinyin_fuzzy);
    }

    //init handwriting speed
    MMINV_READ(MMINV_IM_HW_SPEED,&hw_speed,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_HW_SPEED,&hw_speed);
    }

    //init handwriting thickness
    MMINV_READ(MMINV_IM_HW_THICKNESS,&hw_thickness,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_HW_THICKNESS,&hw_thickness);
    }

    //init handwriting color
    MMINV_READ(MMINV_IM_HW_COLOR,&hw_color,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_HW_COLOR,&hw_color);
    }

    //init default im method
    MMINV_READ(MMINV_IM_DEFAULT_METHOD, &method, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_DEFAULT_METHOD, &method);
    }

    //init default input language
    MMINV_READ(MMINV_IM_LANGUAGE, &input_language, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_LANGUAGE, &input_language);
    }

//init predictive text on/off
    MMINV_READ(MMINV_IM_WORD_PREDIC, &is_word_predic, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_WORD_PREDIC, &is_word_predic);
    }

#if defined MMI_IM_PDA_SUPPORT
    if (input_language == GUIIM_LANG_NONE)      //首次开机，可能设置为空
    {
        GUIIM_LANGUAGE_T guiim_disp_lang = GUIIM_LANG_NONE;
        MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;

        MMIAPISET_GetLanguageType(&language_type);
        guiim_disp_lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);

        //可能当前显示语言也为英文
        if (guiim_disp_lang != GUIIM_LANG_NONE && guiim_disp_lang != GUIIM_LANG_ENGLISH)
        {
            MMIAPIIM_SetPrevLanguage(guiim_disp_lang);
        }

		MMIAPIIM_SetLanguage (guiim_disp_lang);		//设置默认输入语言
    }
#endif
	//init 9KEY or 26KEY
	MMINV_READ(MMINV_IM_KEYBOARD_TYPE, &keyboard_type, return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
		MMINV_WRITE(MMINV_IM_KEYBOARD_TYPE, &keyboard_type);
	}

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
	return_value = MMI_ReadNVItem(IMNV_SOGOU_UPDATE_FREQUENCY, &g_sogou_vocabulary_update_frequency_data.frequency);

	if (MN_RETURN_SUCCESS != return_value)
	{
		g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_SEVEN;
	}
	return_value = MMI_ReadNVItem(IMNV_SOGOU_UPDATE_SETTING, &g_sogou_update_settings_data);
	if (MN_RETURN_SUCCESS != return_value)
	{
		//SCI_TRACE_LOW:"MMIAPIIM_InitNv read IMNV_SOGOU_UPDATE_SETTING error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_419_112_2_18_2_24_46_0,(uint8*)"");
		g_sogou_update_settings_data.is_need_connect_remind = TRUE;
		g_sogou_update_settings_data.is_need_update_notif = TRUE;
	}
	//init IMNV_SOGOU_UPDATE_TIME
	return_value = MMI_ReadNVItem(IMNV_SOGOU_UPDATE_TIME, &g_sogou_vocabulary_update_frequency_data.time);

	if (MN_RETURN_SUCCESS != return_value)
	{
		SCI_MEMSET(&g_sogou_vocabulary_update_frequency_data.time,0,sizeof(SCI_TIME_T));
	}
    // skin sort type
    return_value = MMI_ReadNVItem(IMNV_SOGOU_SKIN_SORT_TYPE, &g_sg_pic_sort_type);
    if (MN_RETURN_SUCCESS != return_value || g_sg_pic_sort_type >= MMISG_SORT_TYPE_MAX)
    {
        g_sg_pic_sort_type = MMISG_SORT_TYPE_TIME;
    }
    
    MMIAPIPHONE_SetEventDelaied (MMI_SOGOU_INIT_DELAY);//需要打开该延时处理，仅仅在延时消息后处理，否则sogou相关程序没有初始化
#endif

#ifdef MMI_IM_PDA_SUPPORT
    {
#if defined MAINLCD_SIZE_320X480
        MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_BIG;
#elif defined MAINLCD_SIZE_240x320
        MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_SMALL;
#else
        MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_NORMAL;
#endif
        
        MMINV_READ(MMINV_IM_KEYBOARD_SIZE, &kb_size, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            MMINV_WRITE(MMINV_IM_KEYBOARD_SIZE, &kb_size);
        }
    }
#endif

#ifdef MMI_IME_USER_DATABASE
    alpha_udb_ptr = (uint8*)SCI_ALLOC_APP(T9_UDB_SIZE);
    SCI_MEMSET(alpha_udb_ptr, 0, T9_UDB_SIZE);
    MMINV_READ(MMINV_IM_ALPHA_UDB, alpha_udb_ptr, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_ALPHA_UDB, alpha_udb_ptr);
    }
    SCI_FREE(alpha_udb_ptr);

#ifdef IM_SIMP_CHINESE_SUPPORT
    chs_udb_ptr = (uint8*)SCI_ALLOC_APP(T9_CAUDB_SIZE);
    SCI_MEMSET(chs_udb_ptr, 0, T9_CAUDB_SIZE);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_CHS_UDB, chs_udb_ptr);
    }
    SCI_FREE(chs_udb_ptr);
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    cht_udb_ptr = (uint8*)SCI_ALLOC_APP(T9_CAUDB_SIZE);
    SCI_MEMSET(cht_udb_ptr, 0, T9_CAUDB_SIZE);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_CHT_UDB, cht_udb_ptr);
    }
    SCI_FREE(cht_udb_ptr);
#endif 

#endif

    return;
}

/*****************************************************************************/
//  Description : get word predictive state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_GetWordPredictiveState(void)
{
    BOOLEAN is_word_predic = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_IM_WORD_PREDIC, &is_word_predic, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_WORD_PREDIC, &is_word_predic);
    }
    return (is_word_predic);
}

/*****************************************************************************/
//  Description : set word predictive state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetWordPredictiveState(BOOLEAN is_word_predic)
{
    MMINV_WRITE(MMINV_IM_WORD_PREDIC, &is_word_predic);

    return;
}

/*****************************************************************************/
//  Description : get im pinyin fuzzy
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetPyFuzzy(void)
{
    uint16              pinyin_fuzzy = MMIIM_DEFAULT_PY_FUZZY;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    //get pinyin fuzzy
    MMINV_READ(MMINV_IM_PINYIN_FUZZY,&pinyin_fuzzy,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_PINYIN_FUZZY,&pinyin_fuzzy);
    }

    return (pinyin_fuzzy);
}

/*****************************************************************************/
//  Description : set im pinyin fuzzy
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetPyFuzzy(
                             uint16     pinyin_fuzzy
                             )
{
    //write pinyin fuzzy
    MMINV_WRITE(MMINV_IM_PINYIN_FUZZY,&pinyin_fuzzy);
}

/*****************************************************************************/
//  Description : get im handwriting speed
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_HW_SPEED_E MMIIM_GetHwSpeed(void)
{
    MMIIM_HW_SPEED_E    hw_speed = MMIIM_HW_SPEED_NORMAL;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    //get handwriting speed
    MMINV_READ(MMINV_IM_HW_SPEED,&hw_speed,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_HW_SPEED,&hw_speed);
    }

    return (hw_speed);
}

/*****************************************************************************/
//  Description : set im handwriting speed
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetHwSpeed(
                             MMIIM_HW_SPEED_E   hw_speed
                             )
{
    //write handwriting speed
    MMINV_WRITE(MMINV_IM_HW_SPEED,&hw_speed);
}

/*****************************************************************************/
//  Description : get im handwriting thickness
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_HW_THICKNESS_E MMIIM_GetHwThickness(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMIIM_HW_THICKNESS_E    hw_thickness = MMIIM_HW_THICKNESS_NORMAL;

    //get handwriting thickness
    MMINV_READ(MMINV_IM_HW_THICKNESS,&hw_thickness,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_HW_THICKNESS,&hw_thickness);
    }

    return (hw_thickness);
}

/*****************************************************************************/
//  Description : set im handwriting thickness
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetHwThickness(
                                 MMIIM_HW_THICKNESS_E   hw_thickness
                                 )
{
    //write handwriting thickness
    MMINV_WRITE(MMINV_IM_HW_THICKNESS,&hw_thickness);
}

/*****************************************************************************/
//  Description : get im handwriting color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIIM_GetHwColor(void)
{
    GUI_COLOR_T         hw_color = MMI_DARK_BLUE_COLOR;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    //get handwriting color
    MMINV_READ(MMINV_IM_HW_COLOR,&hw_color,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_HW_COLOR,&hw_color);
    }

    return (hw_color);
}

/*****************************************************************************/
//  Description : set im handwriting color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_SetHwColor(
                             GUI_COLOR_T    hw_color
                             )
{
    //write handwriting color
    MMINV_WRITE(MMINV_IM_HW_COLOR,&hw_color);
}


#ifdef MMI_IM_PDA_SUPPORT
/*==============================================================================
Description: MMIIM_GetHwThickness
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMIIM_KEYBOARD_SIZE_T MMIIM_GetKeyboardSize(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
#if defined MAINLCD_SIZE_320X480
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_BIG;
#elif defined MAINLCD_SIZE_240x320
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_SMALL;
#else
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_NORMAL;
#endif

    MMINV_READ(MMINV_IM_KEYBOARD_SIZE, &kb_size, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_KEYBOARD_SIZE,&kb_size);
    }

    return (kb_size);
}


/*==============================================================================
Description: MMIIM_SetHwThickness
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void MMIIM_SetKeyboardSize(MMIIM_KEYBOARD_SIZE_T kb_size)
{
    MMINV_WRITE(MMINV_IM_KEYBOARD_SIZE,&kb_size);

    return;
}
#endif


#if defined MMIIM_HW_FULLSCREEN_SUPPORT
/*==============================================================================
Description: MMIAPIIM_HWGetAreaType
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC MMIIM_HW_AREA_TYPE_E MMIAPIIM_HWGetAreaType (void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMIIM_HW_AREA_TYPE_E area_type = MMIIM_HW_AREA_TYPE_HALF_SCREEN;

    MMINV_READ(MMINV_IM_HW_AREA_TYPE, &area_type, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        //SCI_TRACE_LOW:"MMIAPIIM_HWGetAreaType Err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_637_112_2_18_2_24_47_1,(uint8*)"");
        MMINV_WRITE(MMINV_IM_HW_AREA_TYPE, &area_type);
    }

    return (area_type);
}


/*==============================================================================
Description: MMIAPIIM_HWSetAreaType
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void MMIAPIIM_HWSetAreaType (MMIIM_HW_AREA_TYPE_E area_type)
{
    if(area_type < MMIIM_HW_AREA_TYPE_MAX && area_type >= MMIIM_HW_AREA_TYPE_HALF_SCREEN)
    {
        MMINV_WRITE(MMINV_IM_HW_AREA_TYPE, &area_type);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIIM_HWSetAreaType Err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_658_112_2_18_2_24_47_2,(uint8*)"");
    }

    return;
}
#endif


/*****************************************************************************/
//  Description : 获取输入法的默认设置
//  Global resource dependence : 
//  Author: Lianxiang.Zhou
//  Note: 输入方式，输入法，大小写模式
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetDefaultType(GUIIM_METHOD_T *method_ptr)
{
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    GUIIM_METHOD_T method = GUIIM_M_NONE;

    MMINV_READ(MMINV_IM_DEFAULT_METHOD, method_ptr, return_value);

	//force to reset default method:
	//case1:nv read error
	//case2:current default method is the one PDA doesn't allow
    if (MN_RETURN_SUCCESS != return_value || !IsDefaultMethodAllowedForPDA(*method_ptr))
    {
        MMINV_WRITE(MMINV_IM_DEFAULT_METHOD, &method);
        *method_ptr = method;
    }

    return;
}

/*****************************************************************************/
//  Description : 设置输入法的默认设置
//  Global resource dependence : 
//  Author: Lianxiang.Zhou
//  Note: 输入方式，输入法，大小写模式
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetDefaultType(GUIIM_METHOD_T method)
{
	//If we set 123_type to nv for PDA, the im_def_ptr got from
	//s_im_def is incorrect and vkb is limited to the digital mode 
	//Hence, we should check the method before setting

	if(IsDefaultMethodAllowedForPDA(method))
	{
		MMINV_WRITE(MMINV_IM_DEFAULT_METHOD, &method);
	}    

    return;
} 

#if defined MMI_IM_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获取前一次的非英语输入语言
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC GUIIM_LANGUAGE_T MMIAPIIM_GetPrevLanguage(void)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_IM_PREV_LANGUAGE, &lang, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_PREV_LANGUAGE, &lang);
    }

    return (lang);
}

/*****************************************************************************/
//  Description : 设置前一次的非英语输入语言
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetPrevLanguage(GUIIM_LANGUAGE_T lang)
{
    if(lang != GUIIM_LANG_NONE && lang != GUIIM_LANG_ENGLISH)
    {
        MMINV_WRITE(MMINV_IM_PREV_LANGUAGE, &lang);
    }

    return;
}
#endif
/*****************************************************************************/
//  Description : 获取当前输入语言的默认设置
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC GUIIM_LANGUAGE_T MMIAPIIM_GetLanguage(void)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_IM_LANGUAGE, &lang, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IM_LANGUAGE, &lang);
    }

    return (lang);
}

/*****************************************************************************/
//  Description : 设置当前输入语言的默认设置
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetLanguage(GUIIM_LANGUAGE_T lang)
{
    MMINV_WRITE(MMINV_IM_LANGUAGE, &lang);

#if defined MMI_IM_PDA_SUPPORT
    if(lang != GUIIM_LANG_ENGLISH && lang != GUIIM_LANG_NONE)
    {
        MMIAPIIM_SetPrevLanguage(lang);
    }
#endif


    return;
}

/*****************************************************************************/
//  Description : 获取键盘类型
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 0: 9key; 1: 26key
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetKeyBoradType(void)
{
    uint16 kb_type = 1;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	MMINV_READ(MMINV_IM_KEYBOARD_TYPE, &kb_type, return_value);
	if (MN_RETURN_SUCCESS != return_value)
	{
		MMINV_WRITE(MMINV_IM_KEYBOARD_TYPE, &kb_type);
	}

    return (kb_type);
}

/*****************************************************************************/
//  Description : 存储键盘类型
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 0: 9key; 1: 26key
/*****************************************************************************/
PUBLIC void MMIIM_SetKeyBoradType(uint16 kb_type)
{
    //write cstar tp is fullpad
    MMINV_WRITE(MMINV_IM_KEYBOARD_TYPE, &kb_type);

    return;
}

#ifdef MMI_IME_USER_DATABASE
/*****************************************************************************/
//  Description : get the alphabetic udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetAlphaUDBData(int8 * alpha_udb_data_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_IM_ALPHA_UDB, alpha_udb_data_ptr, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(alpha_udb_data_ptr, 0, T9_UDB_SIZE);
        MMINV_WRITE(MMINV_IM_ALPHA_UDB, alpha_udb_data_ptr);
    }
    return;
}

/*****************************************************************************/
//  Description : set the alphabetic udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetAlphaUDBData(int8 * alpha_udb_data_ptr)
{
    MMINV_WRITE(MMINV_IM_ALPHA_UDB, alpha_udb_data_ptr);
    return;
}

#ifdef IM_SIMP_CHINESE_SUPPORT  
/*****************************************************************************/
//  Description : get the chs udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetChsUDBData(int8 * chs_udb_data_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_IM_CHS_UDB, chs_udb_data_ptr, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(chs_udb_data_ptr, 0, T9_CAUDB_SIZE);
        MMINV_WRITE(MMINV_IM_CHS_UDB, chs_udb_data_ptr);
    }
    return;
}

/*****************************************************************************/
//  Description : set the chs udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetChsUDBData(int8 * chs_udb_data_ptr)
{
    MMINV_WRITE(MMINV_IM_CHS_UDB, chs_udb_data_ptr);
    return;
}
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT  
/*****************************************************************************/
//  Description : get the cht udb data
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetChtUDBData(int8 * cht_udb_data_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_IM_CHT_UDB, cht_udb_data_ptr, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(cht_udb_data_ptr, 0, T9_CAUDB_SIZE);
        MMINV_WRITE(MMINV_IM_CHT_UDB, cht_udb_data_ptr);
    }
    return;
}

/*****************************************************************************/
//  Description : set the cht udb data
//  Global resource dependence : 
//  Author: haiwu.chenaa
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIM_SetChtUDBData(int8 * cht_udb_data_ptr)
{
    MMINV_WRITE(MMINV_IM_CHT_UDB, cht_udb_data_ptr);
    return;
}
#endif
#endif

/*****************************************************************************/
//  Description : reset im nv
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_ResetNv(void)
{
    uint16                  pinyin_fuzzy = MMIIM_DEFAULT_PY_FUZZY;
    GUI_COLOR_T             hw_color = MMI_DARK_BLUE_COLOR;
    MMIIM_HW_SPEED_E        hw_speed = MMIIM_HW_SPEED_NORMAL;
    MMIIM_HW_THICKNESS_E    hw_thickness = MMIIM_HW_THICKNESS_NORMAL;
    GUIIM_METHOD_T          method = GUIIM_M_NONE;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    uint16 kb_type = 0;
#ifdef MMI_IM_PDA_SUPPORT    
#if defined MAINLCD_SIZE_320X480
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_BIG;
#elif defined MAINLCD_SIZE_240x320
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_SMALL;
#else
    MMIIM_KEYBOARD_SIZE_T kb_size = MMIIM_KEYBOARD_SIZE_NORMAL;
#endif
#endif
    //reset pinyin fuzzy
    MMIIM_SetPyFuzzy(pinyin_fuzzy);

    //reset handwriting speed
    MMIIM_SetHwSpeed(hw_speed);

    //reset handwriting thickness
    MMIIM_SetHwThickness(hw_thickness);

    //reset handwriting color
    MMIIM_SetHwColor(hw_color);

    //reset method
#ifdef CMCC_UI_STYLE     
    #if defined (IM_SIMP_CHINESE_SUPPORT)
	#if defined (KEYPAD_TYPE_QWERTY_KEYPAD)		//qwerty, sogou, first time entry, will entry 9key input, but the engine not work.
    method = GUIIM_M_Q_PINYIN;
	#else
    method = GUIIM_M_PINYIN;
	#endif
    #endif    
#endif    
    MMIAPIIM_SetDefaultType(method);

    //reset input language
#ifdef CMCC_UI_STYLE     
    #if defined (IM_SIMP_CHINESE_SUPPORT)
    lang = GUIIM_LANG_CHS;
    #endif    
#endif
	/*Here, we should do the same thing as MMIAPIIM_InitNv so as to be consistent in
	  initiating MMINV_IM_LANGUAGE on both init and reset option.*/
#if defined MMI_IM_PDA_SUPPORT
    if (lang == GUIIM_LANG_NONE)
    {
        MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
		
        MMIAPISET_GetLanguageType(&language_type);
        lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
    }
#endif
    MMIAPIIM_SetLanguage(lang);

    //reset keyboard type, 9key 26key
    MMIIM_SetKeyBoradType(kb_type);

#ifdef MMI_IM_PDA_SUPPORT
    MMIIM_SetKeyboardSize(kb_size);
#endif

#ifdef IM_ENGINE_SOGOU
    MMIAPIIM_SGResetNv();
#endif
    return;
}

/*****************************************************************************/
//  Description : clear user data im nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_ClearUserDataNv(void)
{
#ifdef MMI_IME_USER_DATABASE
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    uint8 *alpha_udb_ptr = PNULL;
#ifdef IM_SIMP_CHINESE_SUPPORT
    uint8 *chs_udb_ptr = PNULL;
#endif
#ifdef IM_TRAD_CHINESE_SUPPORT
    uint8 *cht_udb_ptr = PNULL;
#endif

    alpha_udb_ptr = (uint8*)SCI_ALLOC_APP(T9_UDB_SIZE);
    SCI_MEMSET(alpha_udb_ptr, 0, T9_UDB_SIZE);
    MMINV_WRITE(MMINV_IM_ALPHA_UDB, alpha_udb_ptr);
    SCI_FREE(alpha_udb_ptr);
    
#ifdef IM_SIMP_CHINESE_SUPPORT
    chs_udb_ptr = (uint8*)SCI_ALLOC_APP(T9_CAUDB_SIZE);
    SCI_MEMSET(chs_udb_ptr, 0, T9_CAUDB_SIZE);
    MMINV_WRITE(MMINV_IM_CHS_UDB, chs_udb_ptr);
    SCI_FREE(chs_udb_ptr);
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    cht_udb_ptr = (uint8*)SCI_ALLOC_APP(T9_CAUDB_SIZE);
    SCI_MEMSET(cht_udb_ptr, 0, T9_CAUDB_SIZE);
    MMINV_WRITE(MMINV_IM_CHT_UDB, cht_udb_ptr);
    SCI_FREE(cht_udb_ptr);
#endif    
    
#endif

    return;
}

/*****************************************************************************/
//  Description : im init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_Init()
{    
#ifdef MMI_CSTAR_UDB_SUPPORT
    //分析自造词空间
    MMIIM_InitUdbEntry();
    MMIIM_LoadUdbEntry();
#endif

#ifdef IM_ENGINE_T9
    //samsung use csc then use this function, but main branch not support csc, so comment this.
    //MMIIM_T9_InitLDBInfo();
#endif

#ifdef MMI_IME_USER_DATABASE
    MMIIM_T9_LoadUserData();
#endif
}


/*==============================================================================
Description: 判断是否是符号
Global resource dependence: NONE
Author: 
Note:
判断的可能不全
==============================================================================*/
PUBLIC BOOLEAN MMIAPIIM_IsPunct(uint16 text_unicode)
{
	BOOLEAN ispunct_letter = FALSE;
	
	if (MMI_FULLWIDTH_PUNC_START < text_unicode && text_unicode < MMI_FULLWIDTH_PUNC_END)
	{
		//change fullwidth punctuation to halfwidth punctuation
		text_unicode = text_unicode - MMI_FULLWIDTH_PUNC_BASE;
	}
	if(text_unicode <= 0x00FF)
	{
		//acsii
		if(('A' <= text_unicode && text_unicode <= 'Z') 
			|| ('a' <= text_unicode && text_unicode <= 'z')
			||('0' <= text_unicode && text_unicode <= '9')
			)
		{
			ispunct_letter = FALSE;
		}
		else
		{
			ispunct_letter = TRUE;
		}
	}
	else if (MMI_FULLWIDTH_PUNC_SPACE == text_unicode)
	{
		//fullwidth " " 
		ispunct_letter = TRUE;
	} 
	else if(MMI_CJK_PUNC_START <= text_unicode && text_unicode <= MMI_CJK_PUNC_END)
	{
		//CJK Symbals and Punctuation
		ispunct_letter = TRUE;
	}
	   
	return ispunct_letter;
}
/*==============================================================================
Description: 获取字符的字母表示
Global resource dependence: NONE
Author: lianxiang.zhou refactory
Note:
1 如果是[A-Z]，则返回相应的[a-z]
2 如果是[a-z0-9]，则不变返返回
3 如果是全角空格，则返回半角空格
4 如果是半角空格，则不变返返回
5 如果是全角形式字符[0xff00-0xff5f]，则转化成相应的半角(-0xfee0)
6 如果是汉字[0x4e00-0x9fa5]，则返回拼音串
7 否则，返回原unicode值
==============================================================================*/
PUBLIC void MMIAPIIM_GetTextLetters(uint16 text_unicode, uint16 *letters_ptr)
{
    // BOOLEAN is_simple_chinese = FALSE;
    // int16   gb_offset = 0;
    // uint8   char_type = 2;

    if (PNULL == letters_ptr)
    {
        return;
    }

    
    if ('A' <= text_unicode && text_unicode <= 'Z')
    {
        //upper case change to low case
        *letters_ptr = text_unicode + ('a' - 'A');
    }
    else if ((text_unicode >= 0x00C0 && text_unicode <= 0x00D6)
        || (text_unicode >= 0x00D8 && text_unicode <= 0x00DE)
        || (text_unicode >= 0x0410 && text_unicode <= 0x042F)
        || (text_unicode >= 0x0391 && text_unicode <= 0x03AB))
    {
        *letters_ptr = text_unicode + 0x20;
    }
    else if (text_unicode >= 0x0400 && text_unicode <= 0x040F)
    {
        *letters_ptr = text_unicode + 0x50;
    }
    else if (text_unicode >= 0x0531 && text_unicode <= 0x0556)
    {
        *letters_ptr = text_unicode + 0x30;
    }
    else if (text_unicode == 0x0386)
    {
        *letters_ptr = (text_unicode + 0x26);
    }
    else if (text_unicode >= 0x0388 && text_unicode <= 0x038A)
    {
        *letters_ptr = (text_unicode + 0x25);
    }
    else if (text_unicode == 0x038C)
    {
        *letters_ptr = (text_unicode + 0x40);
    }
    else if (text_unicode == 0x038E || text_unicode == 0x038F)
    {
        *letters_ptr = (text_unicode + 0x3F);
    }
    else if ((text_unicode == 0x0198)
        || (text_unicode == 0x01AC)
        || (text_unicode == 0x01B8)
        || (text_unicode == 0x01BC)
        || (text_unicode == 0x01F4) 
        || (text_unicode == 0x0187)
        || (text_unicode == 0x018B)
        || (text_unicode == 0x0191)
        || (text_unicode == 0x01A7)
        || (text_unicode == 0x01AF)
        || (text_unicode == 0x01B3)
        || (text_unicode == 0x01B5)
        )
    {
        *letters_ptr = text_unicode + 1;
    }
    else if (((text_unicode >= 0x0100 && text_unicode <= 0x0136)
        || (text_unicode >= 0x014A && text_unicode <= 0x0176)
        || (text_unicode >= 0x0182 && text_unicode <= 0x0184)
        || (text_unicode >= 0x01A0 && text_unicode <= 0x01A4)
        || (text_unicode >= 0x01DE && text_unicode <= 0x01EE)
        || (text_unicode >= 0x01F8 && text_unicode <= 0x0232)
        || (text_unicode >= 0x0460 && text_unicode <= 0x0480)
        || (text_unicode >= 0x048A && text_unicode <= 0x04BE)
        || (text_unicode >= 0x04D0 && text_unicode <= 0x0524)
        || (text_unicode >= 0x1E00 && text_unicode <= 0x1E94)
        || (text_unicode >= 0x1EA0 && text_unicode <= 0x1EF8)
        )
        && (text_unicode & 0x0001) == 0)
    {
        *letters_ptr = text_unicode + 1;
    }
    else if (((text_unicode >= 0x0139 && text_unicode <= 0x0147)
        || (text_unicode >= 0x0179 && text_unicode <= 0x017D)
        || (text_unicode >= 0x01CD && text_unicode <= 0x01DB)
        || (text_unicode >= 0x04C1 && text_unicode <= 0x04CD)
        )
        && (text_unicode & 0x0001) == 1)
    {
        *letters_ptr = text_unicode + 1;
    }
    else if (text_unicode == 0x0181)
    {
        *letters_ptr = 0x0253; 
    }
    else if (text_unicode == 0x018A )
    {
        *letters_ptr = 0x0257; 
    }
    else if (text_unicode ==  0x018F)
    {
        *letters_ptr = 0x0259 ; 
    }    
    else if (MMI_FULLWIDTH_PUNC_SPACE == text_unicode)
    {
        //change fullwidth " " to halfwidth " "
        *letters_ptr = MMI_HALFWIDTH_PUNC_SPACE;
    }
    else if (MMI_FULLWIDTH_PUNC_START < text_unicode && text_unicode < MMI_FULLWIDTH_PUNC_END)
    {
        //change fullwidth punctuation to halfwidth punctuation
        *letters_ptr = text_unicode - MMI_FULLWIDTH_PUNC_BASE;
    }
    else if(MMI_CJK_PUNC_START <= text_unicode && text_unicode <= MMI_CJK_PUNC_END)
	{
		//CJK Symbals and Punctuation，为了把CJK符号排在前面,空字符的后面，把所有的CJK码都转换为0x01
        *letters_ptr = 0x01;
	}
    else if (MMIIM_CHINESE_FIRST <= text_unicode && text_unicode <= MMIIM_CHINESE_LAST)
    {
        //get pinyin
#if defined(MMIIM_CORE_USE_CSTAR) && (defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT))
        if (CS_IMM_QueryCodePinyin((CS_WORD)text_unicode, (CS_WCHAR *)letters_ptr))
        {
            //not find its character, use its unicode
            *letters_ptr = text_unicode;
        }
#elif defined(MMIIM_CORE_USE_SOGOU) && (defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT))
        {
		WD_UINT16 hzChar = text_unicode;
		WD_UINT8 codetype = 2;		//unicode
		WD_hzCharProp HzCharProp;
		WD_UINT8 retvalue = 0;
		
		SCI_MEMSET(&HzCharProp, 0, sizeof(HzCharProp));
		retvalue = WD_GetPropertyByHz(hzChar, codetype, &HzCharProp);	/*lint  !e628 !e718 !e746*/
		if(retvalue == 0)		//成功,函数返回值头文件注释错误
		{
			uint16 iloop = 0;

			for(iloop = 0; iloop < 6 && iloop < (uint16)strlen(HzCharProp.pinyin[0]); iloop ++)		/*lint  !e64*/
			{
				*letters_ptr ++ = HzCharProp.pinyin[0][iloop];
			}

			//letters_ptr -= iloop;	//还原该指针值,虽然可以不用还原的
		}
		else
		{
			*letters_ptr = text_unicode;
		}
        }		
#elif defined(MMIIM_CORE_USE_SPIM)
        //test code...
        *letters_ptr = text_unicode;
#else
        *letters_ptr = text_unicode;
#endif
    }
    else
    {
        //return unicode
        //include acsii and number, and others...
        *letters_ptr = text_unicode;
    }

    return;
}

/*****************************************************************************/
//  Description : 根据所设的语言及字符，获得该字符所在的键位
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetKeyIndex(                                         // [ret] 键位以字符形式给出，如"1"代表1键，如果返回0，则代表没找到相应的键位
                                   MMISET_LANGUAGE_TYPE_E   set_lang_index, // [in] 输入语言
                                   BOOLEAN                  is_26key,       // [in] 是否是26键键盘
                                   uint16                   input_char      // [in] 输入法需要查询的字符
                                   )
{
    uint16 i = 0;
    uint16 key_index = 0;
    uint16 key_char = 0;
    //BOOLEAN result = FALSE;
    MMIIM_SP_ML_DATA_T im_ml_data = {0};

    // translate to internal language id
   

    MMIIM_SP_ML_Init(&im_ml_data);
    //first get current language keyindex
    if(GetKeyIndexByLan(&key_index, set_lang_index, is_26key, input_char, &im_ml_data))
	{
		key_char = GetKeyCharFromIndex(is_26key, key_index);
	}
	else
	{
		//not find in current language, find in other language table
		for(i = MMISET_LANGUAGE_ENGLISH; i < MMISET_MAX_LANGUAGE; i++)
		{
			if(i != set_lang_index)
			{
				if(GetKeyIndexByLan(&key_index, (MMISET_LANGUAGE_TYPE_E)i, is_26key, input_char, &im_ml_data))
				{
					key_char = GetKeyCharFromIndex(is_26key, key_index);
					break;
				}
			}
		}
	}

    MMIIM_SP_ML_Term(&im_ml_data);

    return key_char;
}



/*****************************************************************************/
//  Description : 根据所设的语言及字符，获得该字符所在的键位
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN   GetKeyIndexByLan(         //[ret]输入字符是否在该语言中找到
                                  uint16                  *key_index,//[OUT] 键位值
								  MMISET_LANGUAGE_TYPE_E   set_lang_index, // [in] 输入语言
                                  BOOLEAN                  is_26key,       // [in] 是否是26键键盘
                                  uint16                   input_char,     // [in] 输入法需要查询的字符
								  MMIIM_SP_ML_DATA_T       *im_ml_data_ptr // [in] 输入法数据表
                                   )
{
	uint32 i = 0;
    GUIIM_METHOD_CFG_T *im_method_ptr = PNULL;
    GUIIM_CAPITAL_MODE_T cap_mode = GUIIM_MODE_LOWER;
	BOOLEAN result = FALSE;
	uint16 im_lang = MMIAPIIM_GetGUILangBySetLangIdx(set_lang_index);
	 
	if (0 == im_lang || im_ml_data_ptr == PNULL)
    {
        return FALSE;
    }
    for (i = 0; i < 2; i++)
    {
        im_method_ptr = SetMultiLangMethod(im_ml_data_ptr, im_lang, is_26key, cap_mode);
        if (PNULL == im_method_ptr)
        {
            break;
        }

        result = MMIIM_SP_ML_GetKeyIndexByChar(im_ml_data_ptr, input_char, key_index);
        if (result)
        {
            break;
        }
        else
        {   // 当小写找不到后，再看有没有大写属性，如果有，就从大写表里找一遍
            if (GUIIM_MODE_MAX != im_method_ptr->capital)
            {
                cap_mode = GUIIM_MODE_UPPER;
            }
            else
            {
                break;
            }
        }
    }
	return result;
}

/*****************************************************************************/
//  Description : 返回按键时对应的字符
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetKeyChar(                                          // [ret] 返回对应的字符
                                  MMISET_LANGUAGE_TYPE_E    set_lang_index, // [in] 输入语言
                                  BOOLEAN                   is_26key,       // [in] 是否是26键键盘
                                  uint16                    key_index,      // [in] 键位，以字符形式给出，如"1"代表1键
                                  uint16                    input_count     // [in] 按该键位的次数
                                  )
{
    uint16 ret_value = 0;
    uint16 total_char_count = 0;
    uint16 actual_input_count = input_count;
    BOOLEAN result = FALSE;
    MMIIM_SP_KEY_INDEX_T sp_key_index = 0;
    MMIIM_SP_ML_DATA_T im_ml_data = {0};

    result = InitMlData(set_lang_index, is_26key, &im_ml_data, PNULL);
    if (!result)
    {
        return 0;
    }

    sp_key_index = GetKeyIndexFromChar(is_26key, key_index);

    total_char_count = MMIIM_SP_ML_GetCount(&im_ml_data, sp_key_index);
    if (0 == total_char_count)
    {
        return 0;
    }

    if (actual_input_count > total_char_count)
    {
        actual_input_count = actual_input_count % total_char_count;
    }

    if (0 == actual_input_count)
    {
        actual_input_count = total_char_count;
    }

    ret_value = MMIIM_SP_ML_GetChar(
                                    &im_ml_data, 
                                    sp_key_index, 
                                    actual_input_count);

    MMIIM_SP_ML_Term(&im_ml_data);

    return ret_value;

#if 0   // 以下代码是区分大小写
    uint16 ret_value = 0;
    uint16 char_count1 = 0;
    uint16 char_count2 = 0;
    uint16 total_char_count = 0;
    uint16 actual_input_count = input_count;
    BOOLEAN result = FALSE;
    MMIIM_SP_KEY_INDEX_T sp_key_index = 0;
    MMIIM_SP_ML_DATA_T im_ml_data1 = {0};   // lower method
    MMIIM_SP_ML_DATA_T im_ml_data2 = {0};   // upper method

    result = InitMlData(set_lang_index, is_26key, &im_ml_data1, &im_ml_data2);
    if (!result)
    {
        return 0;
    }

    sp_key_index = GetKeyIndexFromChar(is_26key, key_index);

    char_count1 = MMIIM_SP_ML_GetCount(&im_ml_data1, sp_key_index);
    char_count2 = MMIIM_SP_ML_GetCount(&im_ml_data2, sp_key_index);

    total_char_count = char_count1 + char_count2;
    if (0 == total_char_count)
    {
        return 0;
    }

    if (actual_input_count > total_char_count)
    {
        actual_input_count = actual_input_count % total_char_count;
    }

    if (0 == actual_input_count)
    {
        actual_input_count = total_char_count;
    }

    if (actual_input_count <= char_count1)
    {
        ret_value = MMIIM_SP_ML_GetChar(
                                        &im_ml_data1, 
                                        sp_key_index, 
                                        actual_input_count);
    }
    else
    {
        ret_value = MMIIM_SP_ML_GetChar(
                                        &im_ml_data2, 
                                        sp_key_index, 
                                        actual_input_count - char_count1);
    }

    MMIIM_SP_ML_Term(&im_ml_data1);
    MMIIM_SP_ML_Term(&im_ml_data2);

    return ret_value;
#endif
}

/*****************************************************************************/
//  Description : 设置多国语的语言
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL GUIIM_METHOD_CFG_T* SetMultiLangMethod(
                                 MMIIM_SP_ML_DATA_T     *im_ml_data_ptr,   // [in]
                                 uint16                 im_lang,            // [in] 输入语言
                                 BOOLEAN                is_26key,           // [in] 是否是26键键盘
                                 GUIIM_CAPITAL_MODE_T   cap_mode            // [in] 大小写
                                 )
{
    BOOLEAN             result          = FALSE;
    GUIIM_METHOD_CFG_T  *im_method_ptr  = PNULL;
    GUIIM_INPUT_TYPE_T  input_type      = GUIIM_INPUT_TYPE_TOUCHPEN;
    //SCI_ASSERT(PNULL != im_ml_data_ptr);
    if (PNULL == im_ml_data_ptr)
    {
        //SCI_TRACE_LOW:"InitMlData: im_ml_data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1335_112_2_18_2_24_48_3,(uint8*)"");
        return FALSE;
    }
    
    

    if (is_26key)
    {
        input_type = GUIIM_INPUT_TYPE_TP_QWERT;
    }

    im_method_ptr = MMITHEME_IMGetDefBySet(
            (GUIIM_LANGUAGE_T)im_lang, 
            input_type, 
            GUIIM_TYPE_MULTITAP,
            cap_mode);
    if (PNULL == im_method_ptr)
    {
        //SCI_TRACE_LOW("== SetMultiLangMethod == can not get def method %d, type %d, cap %d", im_lang, input_type, cap_mode);
        goto _Error_;
    }

    result = MMIIM_SP_ML_SetMethod(im_ml_data_ptr, im_method_ptr->method);
    if (!result)
    {
        //SCI_TRACE_LOW:"== SetMultiLangMethod == can not find method %d in multi-lang data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1360_112_2_18_2_24_48_4,(uint8*)"d", im_method_ptr->method);
        goto _Error_;
    }

    return im_method_ptr;

_Error_:
    MMIIM_SP_ML_Term(im_ml_data_ptr);
    return PNULL;
}

/*****************************************************************************/
//  Description : 初始化多国语引擎
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitMlData(
                         MMISET_LANGUAGE_TYPE_E     set_lang_index, // [in] 输入语言
                         BOOLEAN                    is_26key,       // [in] 是否是26键键盘
                         MMIIM_SP_ML_DATA_T         *im_ml_data1_ptr, // [out]
                         MMIIM_SP_ML_DATA_T         *im_ml_data2_ptr  // [out]
                         )
{
    uint16  im_lang = 0;
    GUIIM_METHOD_CFG_T  *im_method_ptr  = PNULL;

    if (PNULL == im_ml_data1_ptr)
    {
        return FALSE;
    }

    im_lang = MMIAPIIM_GetGUILangBySetLangIdx(set_lang_index);
    if (0 == im_lang)
    {
        //SCI_TRACE_LOW:"== InitMlData == can get lang index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1393_112_2_18_2_24_48_5,(uint8*)"d", set_lang_index);
        return FALSE;
    }

    MMIIM_SP_ML_Init(im_ml_data1_ptr);

    im_method_ptr = SetMultiLangMethod(im_ml_data1_ptr, im_lang, is_26key, GUIIM_MODE_LOWER);
    if (PNULL != im_ml_data2_ptr && PNULL != im_method_ptr
        && GUIIM_MODE_MAX != im_method_ptr->capital)
    {
        MMIIM_SP_ML_Init(im_ml_data2_ptr);
        SetMultiLangMethod(im_ml_data2_ptr, im_lang, is_26key, GUIIM_MODE_UPPER);
    }

    return (PNULL != im_method_ptr);
}

/*****************************************************************************/
//  Description : 由内部索引转换到字符索引
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL MMIIM_SP_KEY_INDEX_T GetKeyCharFromIndex(
                                               BOOLEAN  is_26key,
                                               uint16   key_index
                                               )
{
    uint16 i = 0;
    uint16 key_char = 0;
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    if (is_26key)
    {
        for (i = 0; i < sizeof(s_index_char_mapping_26key) / sizeof(INDEX_CHAR_MAPPING_T); i++)
        {
            if (key_index == s_index_char_mapping_26key[i].key_index)
            {
                key_char = s_index_char_mapping_26key[i].key_char;
                break;
            }
        }
    }
    else
    {
        for (i = 0; i < sizeof(s_index_char_mapping_9key) / sizeof(INDEX_CHAR_MAPPING_T); i++)
        {
            if (key_index == s_index_char_mapping_9key[i].key_index)
            {
                key_char = s_index_char_mapping_9key[i].key_char;
                break;
            }
        }
    }
#else
        for (i = 0; i < sizeof(s_index_char_mapping_9key) / sizeof(INDEX_CHAR_MAPPING_T); i++)
        {
            if (key_index == s_index_char_mapping_9key[i].key_index)
            {
                key_char = s_index_char_mapping_9key[i].key_char;
                break;
            }
        }

#endif

    return key_char;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note: 因NV被改写导致仅显示数字VKB，这里需要对NV中存储的method做个检验，如果
//  是123类别，则返回false，以供后续处理(手写暂时不放在这里，s_im_def中TP下手写
//	是作为默认输入配置)
/*****************************************************************************/
LOCAL BOOLEAN IsDefaultMethodAllowedForPDA(
									GUIIM_METHOD_T method
                                               )
{
	BOOLEAN result = TRUE;

#if !defined(KEYPAD_TYPE_TYPICAL_KEYPAD) && !defined(KEYPAD_TYPE_QWERTY_KEYPAD) && defined(MMI_IM_PDA_SUPPORT)
	switch(method)
	{
		case GUIIM_M_TP_123:
		case GUIIM_M_TPQ_123:
		case GUIIM_M_Q_123:
		case GUIIM_M_123:
		//case GUIIM_M_HANDWRITING:
			result = FALSE;
			break;

		default:
			result = TRUE;
			break;
	}
#endif

	SCI_TRACE_LOW("[arvinzhang]IsDefaultMethodAllowedForPDA: method is %d", method);
	return result;
}

/*****************************************************************************/
//  Description : 从字符索引转换到内部索引
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
LOCAL MMIIM_SP_KEY_INDEX_T GetKeyIndexFromChar(
                                               BOOLEAN  is_26key,
                                               uint16   key_char
                                               )
{
    uint16 i = 0;
    MMIIM_SP_KEY_INDEX_T key_index = 0;

#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
    if (is_26key)
    {
        key_index = MMIIM_SP_26KEY_INDEX_MAX;

        for (i = 0; i < sizeof(s_index_char_mapping_26key) / sizeof(INDEX_CHAR_MAPPING_T); i++)
        {
            if (key_char == s_index_char_mapping_26key[i].key_char)
            {
                key_index = s_index_char_mapping_26key[i].key_index;
                break;
            }
        }
    }
    else
    {
        key_index = MMIIM_SP_9KEY_INDEX_MAX;

        for (i = 0; i < sizeof(s_index_char_mapping_9key) / sizeof(INDEX_CHAR_MAPPING_T); i++)
        {
            if (key_char == s_index_char_mapping_9key[i].key_char)
            {
                key_index = s_index_char_mapping_9key[i].key_index;
                break;
            }
        }
    }
#else
        key_index = MMIIM_SP_9KEY_INDEX_MAX;

        for (i = 0; i < sizeof(s_index_char_mapping_9key) / sizeof(INDEX_CHAR_MAPPING_T); i++)
        {
            if (key_char == s_index_char_mapping_9key[i].key_char)
            {
                key_index = s_index_char_mapping_9key[i].key_index;
                break;
            }
        }

#endif

    return key_index;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_CapsExist

Func:   判断语言是否存在大小写

Input:  
            lang:  输入法对应的语言

Return: TRUE:存在大小写，FALSE:不存在大小写

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIIM_CapsExist(GUIIM_LANGUAGE_T lang)
{
    switch(lang)
    {
        case GUIIM_LANG_CHS:
        case GUIIM_LANG_CHT:		
        case GUIIM_LANG_ARABIC:
        case GUIIM_LANG_ASSAMESE:  
        case GUIIM_LANG_BENGALI:  
        case GUIIM_LANG_GEORGIAN:  
        case GUIIM_LANG_GUJARATI:    
        case GUIIM_LANG_HEBREW:
        case GUIIM_LANG_HINDI:    
        case GUIIM_LANG_KANNADA:   
        case GUIIM_LANG_MALAYALAM:     
        case GUIIM_LANG_MARATHI:     
        case GUIIM_LANG_PERSIAN: 
        case GUIIM_LANG_PUNJABI:    
        case GUIIM_LANG_TAGALOG:
        case GUIIM_LANG_TAMIL:
        case GUIIM_LANG_TELUGU:
        case GUIIM_LANG_THAI:
        case GUIIM_LANG_URDU:    
        case GUIIM_LANG_ORIYA:
        case GUIIM_LANG_MYANMAR: 
        case GUIIM_LANG_AMHARIC:  
        case GUIIM_LANG_KHMER: 
        case GUIIM_LANG_LAO: 
        case GUIIM_LANG_UYGHUR:
	    case GUIIM_LANG_TIBETAN:
            return FALSE;
           
        default:
            return TRUE;
    }
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_InitLangKeyTable

Func:   1.初始化语言对应的输入法按键丝印数据表，用于电话本查找；因电话本查找时，过于
             频繁调用字符对应按键查找函数，如果每次都重新查找按键丝印数据表的话，太浪费
             资源，速度慢
	    2.此处有一个约束，即不支持笔画等输入法对应的按键数据，只支持9键和26键
	    
Input:  
            lang:  当前系统设置的语言

Return: 

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void MMIAPIIM_InitLangKeyTable(MMISET_LANGUAGE_TYPE_E  language_type)
{
	GUIIM_METHOD_T method = GUIIM_M_NONE;
	GUIIM_LANGUAGE_T im_lang = GUIIM_LANG_NONE;

	im_lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
	
	//存在大小写
	if (MMIAPIIM_CapsExist(im_lang))
	{
		s_lang_keytable.has_caps = TRUE;
		//9
		method = MMITHEME_IMSearchMethod (im_lang, GUIIM_INPUT_TYPE_KEYBOARD, GUIIM_MODE_UPPER);	//得到9键输入法
		if(method != GUIIM_M_NONE)
		{
			s_lang_keytable.pupper_9key = (MMIIM_SP_9KEY_TABLE_T *)MMIIM_SP_Smart9Key_GetKeyTable (method);
		}
		else
		{
			//SCI_TRACE_LOW:"MMIAPIIM_InitLangKeyTable"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1571_112_2_18_2_24_49_6,(uint8*)"");
			s_lang_keytable.pupper_9key = PNULL;
		}

		method = MMITHEME_IMSearchMethod (im_lang, GUIIM_INPUT_TYPE_KEYBOARD, GUIIM_MODE_LOWER);	//得到9键输入法
		if(method != GUIIM_M_NONE)
		{
			s_lang_keytable.plower_9key = (MMIIM_SP_9KEY_TABLE_T *)MMIIM_SP_Smart9Key_GetKeyTable (method);
		}
		else
		{
			//SCI_TRACE_LOW:"MMIAPIIM_InitLangKeyTable"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1582_112_2_18_2_24_49_7,(uint8*)"");
			s_lang_keytable.plower_9key = PNULL;
		}

#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
		//26
		method = MMITHEME_IMSearchMethod (im_lang, GUIIM_INPUT_TYPE_QWERT, GUIIM_MODE_UPPER);	//得到26键输入法
		if(method != GUIIM_M_NONE)
		{
			s_lang_keytable.pupper_26key = (MMIIM_SP_26KEY_KEYCHAR_T *)MMIIM_SP_SMART26KEY_GetKeyTable (method);
		}
		else
		{
			//SCI_TRACE_LOW:"MMIAPIIM_InitLangKeyTable"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1594_112_2_18_2_24_49_8,(uint8*)"");
			s_lang_keytable.pupper_26key = PNULL;
		}

		method = MMITHEME_IMSearchMethod (im_lang, GUIIM_INPUT_TYPE_QWERT, GUIIM_MODE_LOWER);	//得到26键输入法
		if(method != GUIIM_M_NONE)
		{
			s_lang_keytable.pupper_26key = (MMIIM_SP_26KEY_KEYCHAR_T *)MMIIM_SP_SMART26KEY_GetKeyTable (method);
		}
		else
		{
			//SCI_TRACE_LOW:"MMIAPIIM_InitLangKeyTable"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1605_112_2_18_2_24_49_9,(uint8*)"");
			s_lang_keytable.pupper_26key = PNULL;
		}
#endif        
	}
	else
	{
		s_lang_keytable.has_caps = FALSE;

		method = MMITHEME_IMSearchMethod (im_lang, GUIIM_INPUT_TYPE_KEYBOARD, GUIIM_MODE_MAX);	//得到9键输入法
		if (method != GUIIM_M_NONE)
		{
			s_lang_keytable.pupper_9key = s_lang_keytable.plower_9key = (MMIIM_SP_9KEY_TABLE_T *)MMIIM_SP_Smart9Key_GetKeyTable (method);
		}
		else
		{
			//SCI_TRACE_LOW:"MMIAPIIM_InitLangKeyTable"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1620_112_2_18_2_24_49_10,(uint8*)"");
			s_lang_keytable.pupper_9key = s_lang_keytable.plower_9key = PNULL;
		}
		
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
		method = MMITHEME_IMSearchMethod (im_lang, GUIIM_INPUT_TYPE_QWERT, GUIIM_MODE_MAX);	//得到26键输入法
		if (method != GUIIM_M_NONE)
		{
			s_lang_keytable.pupper_26key = s_lang_keytable.plower_26key = (MMIIM_SP_26KEY_KEYCHAR_T *)MMIIM_SP_SMART26KEY_GetKeyTable (method);
		}
		else
		{
			//SCI_TRACE_LOW:"MMIAPIIM_InitLangKeyTable"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1631_112_2_18_2_24_49_11,(uint8*)"");
			s_lang_keytable.pupper_26key = s_lang_keytable.plower_26key = PNULL;
		}
#endif        
	}
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetLangKeyTable

Func:   1.获取语言对应的按键丝印数据表
	    2.此处有一个约束，即不支持笔画等输入法对应的按键数据，只支持9键和26键
	    
Input:  

Return: 

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC S_LANG_KEY_TABLE *pMMIAPIIM_GetLangKeyTable (void)
{
	return &s_lang_keytable;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetKeyIndexByKeytable

Func:   1.查找指定的字符在按键数据表中的索引，MMIIM_SP_9KEY_INDEX_1~MMIIM_SP_9KEY_INDEX_0
		或MMIIM_SP_9KEY_INDEX_A~MMIIM_SP_9KEY_INDEX_Z

Input:  
	    pkey_table: 要查找的按键数据表，为MMIIM_SP_26KEY_KEYCHAR_T或MMIIM_SP_9KEY_TABLE_DEF_T
    	    input_char:	要查找的字符
    	    is_26key:  因9键和26键数据表的组织形式不同，没有办法通过pkey_table自身来区分
    	      故添加一个区分参数
Return: 
		查找成功，则a~z或1~9返回；否则返回  < 0数据

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIAPIIM_GetKeyIndexByKeytable(               // [ret] 键位以字符形式给出，如"1"代表1键，如果返回0，则代表没找到相应的键位                          
                                   wchar const *pkey_table,
                                   uint16                   input_char,      // [in] 输入法需要查询的字符
					BOOLEAN                  is_26key       // [in] 是否是26键键盘
                                   )
{
	uint32 iloop1 = 0, iloop2 = 0;
		
	if(pkey_table == PNULL)
	{
		//SCI_TRACE_LOW:"MMIAPIIM_GetKeyIndexByKeytable ERR PARAM"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1682_112_2_18_2_24_49_12,(uint8*)"");
		return -1;
	}
	
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
	//判断指针类型
	if(is_26key)		//26键盘数据表
	{
		MMIIM_SP_26KEY_KEYCHAR_T *p26key = (MMIIM_SP_26KEY_KEYCHAR_T *)pkey_table;

		for(iloop1 = 0; iloop1 < 26; iloop1 ++)
		{
			if(p26key == PNULL || p26key->key_index == MMIIM_SP_26KEY_INDEX_MAX)
			{
				//SCI_TRACE_LOW:"MMIAPIIM_GetKeyIndexByKeytable"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1695_112_2_18_2_24_49_13,(uint8*)"");
				return -1;
			}

			for (iloop2 = 1; iloop2 < p26key->key_chars[0]; iloop2 ++)	//第一个字符表示数据个数
			{
				if (p26key->key_chars[iloop2] == input_char)		//找到对应的字符
				{
					return (iloop1 - MMIIM_SP_26KEY_INDEX_A + 'a');
				}
			}
			
			p26key ++;
		}
	}
	else
#endif        
	{
		MMIIM_SP_9KEY_TABLE_DEF_T *p9key = (MMIIM_SP_9KEY_TABLE_DEF_T *)pkey_table;		//二维数组指针[MMIIM_SP_9KEY_INDEX_MAX][MMIIM_SP_9KEY_CHAR_MAX+1]
		wchar *pcur_key = PNULL;

		pcur_key = (wchar *)p9key;
		for(iloop1 = 0; iloop1 < 10; iloop1 ++)	//10个数字
		{
			if(pcur_key == PNULL || pcur_key[0] == MMIIM_SP_9KEY_INDEX_MAX)
			{
				//SCI_TRACE_LOW:"MMIAPIIM_GetKeyIndexByKeytable"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1720_112_2_18_2_24_49_14,(uint8*)"");
				return -1;
			}

			for (iloop2 = 1; iloop2 < pcur_key[0]; iloop2 ++)	//第一个字符表示数据个数
			{
				if (pcur_key[iloop2] == input_char)		//找到对应的字符
				{
					return (iloop1 - MMIIM_SP_9KEY_INDEX_1 + '1');
				}
			}
			
			pcur_key += MMIIM_SP_9KEY_CHAR_MAX + 1;
		}
		
	}

	return -1;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_GetLangCodeString_ptr

Func:   1.获取语言标志码字符串数据

Input:  im_lang:输入法语言

Return: 
            成功返回该语言对应的标志码字符串；否则返回error字符串
            
Create: yaoguangchen@spread. 2012-01-26

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC const wchar *MMIAPIIM_GetLangCodeString_ptr (GUIIM_LANGUAGE_T im_lang)
{
    uint16 iloop = 0;
    
    for (iloop = 0; iloop < ARR_SIZE (s_lang_code_table); iloop ++)
    {
        if (s_lang_code_table[iloop].im_lang == im_lang)
            return s_lang_code_table[iloop].code_str;
    }

    return L"Error";
}


/*****************************************************************************/
//  Description : get wval's key value
//  Global resource dependence : none
//  Author: 
//  retrun: key value char: '1': key_1, '2' : key_2,....'0':key_0
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_GetWcharKeyValue(wchar *wstr, MMISET_LANGUAGE_TYPE_E lan_type, wchar *key_str)
{
    uint32 k = 0; 
    BOOLEAN   is_exist = FALSE;
    uint16    wstr_len = 0;

    if(PNULL == wstr || PNULL == key_str)
    {
        return FALSE;
    }
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
	if(MMISET_LANGUAGE_SIMP_CHINESE == lan_type || MMISET_LANGUAGE_TRAD_CHINESE == lan_type)
	{
		lan_type = MMISET_LANGUAGE_ENGLISH;
	}
#endif
    wstr_len = MMIAPICOM_Wstrlen(wstr);
    //get key value
    for(k = 0; k < wstr_len; k++)
    {
		
		if(MMIAPIIM_IsPunct(wstr[k]))
		{
			//所有的符号都影射到'1'
			key_str[k] = '1';
		}
		else
		{
			key_str[k] =  MMIAPIIM_GetKeyIndex(                                     
				lan_type, // [in] 输入语言
				FALSE,       // [in] 是否是26键键盘
				wstr[k]    // [in] 输入法需要查询的字符
				);
		}
    }
	if(wstr_len > 0)
	{
		is_exist = TRUE;
	}
    key_str[k] = 0;
    return is_exist;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIIM_GetWcharKeyValueByTable

Func:   1.根据输入字符，转换为字符对应的按键序列，为a~z或1~9
	    2.此处依赖于当前系统显示语言及设置的输入法按键丝印数据
	    
Input:  wstr:输入字符串
	    key_str:输出字符串
	    is_26key:是否为26键，即返回为a~z or 1~9
    	    
Return: 转换成功TRUE，else FALSE;

Create: yaoguangchen@spread. 2011-12-02

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIIM_GetWcharKeyValueByTable (wchar *wstr, wchar *key_str, BOOLEAN is_26key)
{
	uint32 k = 0; 
	uint16    wstr_len = 0;
	S_LANG_KEY_TABLE *plang_table = PNULL;

	if(PNULL == wstr || PNULL == key_str)
	{
		//SCI_TRACE_LOW:"MMIIM_GetWcharKeyValueByTable"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1836_112_2_18_2_24_49_15,(uint8*)"");
		return FALSE;
	}

	wstr_len = MMIAPICOM_Wstrlen(wstr);
	//get key value
	for(k = 0; k < wstr_len; k++)
	{
		if(MMIAPIIM_IsPunct(wstr[k]))
		{
			//所有的符号都影射到'1'
			key_str[k] = '1';
		}
		else
		{
			int32 out_char = -1;

			plang_table = pMMIAPIIM_GetLangKeyTable ();     /*lint *!746*/

#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
			if(is_26key)
			{
			
				out_char =  MMIAPIIM_GetKeyIndexByKeytable (                                     
				plang_table->plower_26key, 	/*lint !e64*/     // 26键小写数据表
				wstr[k],    // [in] 输入法需要查询的字符
				TRUE       // [in] 是否是26键键盘
				);
				if(out_char > 0)
				{
					key_str[k] = out_char;
				}
				else
				{
					out_char =  MMIAPIIM_GetKeyIndexByKeytable (                                     
					plang_table->pupper_26key, 	/*lint !e64*/   // 26键小写数据表
					wstr[k],    // [in] 输入法需要查询的字符
					TRUE       // [in] 是否是26键键盘
					);

					if(out_char > 0)
					{
						key_str[k] = out_char;
					}
					else
					{
						//SCI_TRACE_LOW:"MMIIM_GetWcharKeyValueByTable"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1880_112_2_18_2_24_49_16,(uint8*)"");
						return FALSE;
					}
				}
			}
			else
#endif                
			{
				out_char =  MMIAPIIM_GetKeyIndexByKeytable (                                     
				plang_table->plower_9key, 	/*lint !e64*/   // 9键小写数据表
				wstr[k],    // [in] 输入法需要查询的字符
				FALSE       // [in] 是否是26键键盘
				);
				if(out_char > 0)
				{
					key_str[k] = out_char;
				}
				else
				{
					out_char =  MMIAPIIM_GetKeyIndexByKeytable (                                     
					plang_table->pupper_9key, 	/*lint !e64*/  // 9键小写数据表
					wstr[k],    // [in] 输入法需要查询的字符
					FALSE       // [in] 是否是26键键盘
					);

					if(out_char > 0)
					{
						key_str[k] = out_char;
					}
					else
					{
						key_str[k] = 0;
						//SCI_TRACE_LOW:"MMIIM_GetWcharKeyValueByTable"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_1911_112_2_18_2_24_49_17,(uint8*)"");
						return FALSE;
					}
				}
			}
		}
	}

	key_str[k] = 0;

	return TRUE;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIIM_PhysicalKeyToImKey

Func:   将物理按键消息转换为输入法按键消息对应的按键码
        
Input:
            msg_id:  按键消息
            
Return:

Thoughtway:
            1. 当前mmk存在一些按键码不够使用的情况，如 +等按键码；
            2. 我们的客户是多样化的，什么样的键盘形态都能配置出来，
                既然MMK是无法满足用户的，那还是上层应用来实现好了
            3. 这里已经可以确定的是，MMK的所有物理按键能够将消息作为msg-id传到上层。
                但是这个shift按键配合使用的配置就需要自定义配置了
            4. MMIDEFAULT_ConvertKeyCode MMIDEFAULT_GetShiftKey MMIDEFAULT_ConvertTypicalKeyToQwertyKey等均依赖于
                MMK按键，所以输入法这边尽量与这些函数分离开来
            5. 这个函数与函数 MMIIM_SP_TransMsg的按键处理部分相似
            6. 当前输入法使用了多套vkey，实在是不好维护，大动作会影响客户的维护
                暂时保留这些多余的代码吧:虚拟键盘VKEY_xxx，sp_key_xxx, sp_26key_index_xxx等
Notes:

Create: yaoguangchen@spread. 2012-05-31

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC MMIIM_SP_VKEY_T MMIIM_PhysicalKeyMsgToImKey (MMI_MESSAGE_ID_E msg_id)
{
    uint8 key_code = msg_id & MMIIM_SP_KEY_MASK;
    MMIIM_SP_VKEY_T im_vk = MMIIM_SP_KEY_NONE;
    uint8 key_cnt = 0;
    uint32 iloop = 0;

    if (MMK_IsKeyMsg (msg_id))
    {
        //im_vk = key_code;

#if (defined KEYPAD_TYPE_QWERTY_KEYPAD)
        if (MMK_IsShiftKeyPress())
        {
            key_cnt = ARR_SIZE (s_phy_im_shift_key_map);

            for (iloop  = 0; iloop < key_cnt; iloop ++)
            {
                if (key_code == s_phy_im_shift_key_map[iloop].physical_key)
                {
                    im_vk = s_phy_im_shift_key_map[iloop].im_vk;
                    break;
                }
            }
        }
        else
#endif            
        {
            key_cnt = ARR_SIZE (s_phy_im_key_map);

            for (iloop  = 0; iloop < key_cnt; iloop ++)
            {
                if (key_code == s_phy_im_key_map[iloop].physical_key)
                {
                    im_vk = s_phy_im_key_map[iloop].im_vk;
                    break;
                }
            }
        }
    }
    
    return im_vk;
}



#ifdef IM_ENGINE_CSTAR
/******************************************************************************/
// Description: 转化输入法类型
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
PUBLIC CS_UINT MMIIM_ConvertMethod(GUIIM_METHOD_T method)
{
    uint16 i = 0;
    uint16 im_cnt = ARR_SIZE(g_cstar_method_relation_list);
    CS_UINT cs_im = IM_NONE;

    if(GUIIM_M_NONE == method)
        return cs_im;

    for(i = 0; i < im_cnt; i++)
    {
        if(g_cstar_method_relation_list[i].guiim_method == method)
        {
            cs_im = g_cstar_method_relation_list[i].cstar_method;
            break;
        }
    }
    
    return cs_im;
}
#endif

#ifdef MMI_CSTAR_UDB_SUPPORT
/*****************************************************************************/
//  Description : MMIIM_GetUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC UDB_ENTRY_T MMIIM_GetUdbEntry()
{
    return s_udb_entry;
}

/*****************************************************************************/
//  Description : MMIIM_GetUdbTotalSize
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetUdbTotalSize()
{
    uint16 i = 0;
    uint total_size = 0;

    for (i = 0; i < ARR_SIZE(s_udb_lang_config); i++)
    {
        total_size += s_udb_lang_config[i].lang_len;
    }

    return total_size;
}

/*****************************************************************************/
//  Description : MMIIM_InitUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_InitUdbEntry()
{
    BOOLEAN is_ret = FALSE;
    uint total_size = MMIIM_GetUdbTotalSize();
    
    MMIIM_FreeUdbEntry();

    //必须得有配置大小，理论上英语会是一直开的，能得到大小
    if (0 == total_size)
    {
        return is_ret;
    }
    
    s_udb_entry.udb_ptr = SCI_ALLOCA(total_size);
    if (PNULL == s_udb_entry.udb_ptr)
    {
        return is_ret;
    }
    SCI_MEMSET(s_udb_entry.udb_ptr, 0x0, total_size);
    s_udb_entry.udb_len = total_size;

    is_ret = TRUE;
    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIM_FreeUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_FreeUdbEntry()
{
    if (PNULL != s_udb_entry.udb_ptr)
    {
        SCI_FREE(s_udb_entry.udb_ptr);
    }
    s_udb_entry.udb_len = 0;

    return TRUE;
}

/*****************************************************************************/
//  Description : MMIIM_LoadUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
//	Modify:	需要检测当前的udb数据是否为当前手机使用的版本，如使用其他手机上的
//			udb数据，会引起异常
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_LoadUdbEntry()
{
    BOOLEAN is_ret = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_SDCARD;
    wchar *device_ptr = PNULL;
    uint16 device_len = 0;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    uint32 file_size = 0;
    uint32 read_len = 0;
	BOOLEAN adapt_udb_file = TRUE;		//仅仅在版本不对时，才为FALSE
	BOOLEAN re_create_compile_time_file = FALSE;		
	char*          pcompile_time = PNULL;
#ifndef _WIN32
	pcompile_time = VERSION_GetInfo(BUILD_TIME);	/*lint !e605*/
#else
	pcompile_time = "2012-07-09";
#endif	
	
    if (PNULL == s_udb_entry.udb_ptr
        || 0 == s_udb_entry.udb_len)
    {
        return is_ret;
    }
    
    //先找到有效的盘
    device_type = MMIAPIFMM_GetFirstValidSD();
    if(device_type < MMI_DEVICE_NUM)
    {
        device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(device_type);
        device_len = MMIAPIFMM_GetDevicePathLen(device_type);
    }
    else
    {    
        device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
        device_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    
    MMIAPIFMM_CombineFullPath(
        device_ptr,
        device_len,
        s_cstar_udb_dir,
        MMIAPICOM_Wstrlen(s_cstar_udb_dir),
        PNULL,
        0,
        full_path_name,
        &full_path_len
    );

	//创建文件夹
    if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
    {
        MMIAPIFMM_CreateDir(full_path_name, full_path_len);
		re_create_compile_time_file = TRUE;
    }
	else	//文件夹已经存在，判断该文件夹是否与当前手机版本匹配
	{
		char read_compile_time[50 + 1] = {0};	
		
		MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
		
		//根据当前手机编译时间，创建编译时间文件，来记录当前cstar使用的udb数据
	    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
	    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	    
	    MMIAPIFMM_CombineFullPath(
	        device_ptr,
	        device_len,
	        s_cstar_udb_dir,
	        MMIAPICOM_Wstrlen(s_cstar_udb_dir),
	        s_cstar_compile_time,
	        MMIAPICOM_Wstrlen(s_cstar_compile_time),
	        full_path_name,
	        &full_path_len
	    );

		file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);
		if (file_handle)
		{
			uint32  read_size  = 0;
			
			SCI_MEMSET (read_compile_time, 0, sizeof(read_compile_time));
		    file_error = MMIAPIFMM_ReadFile (file_handle, read_compile_time, sizeof (read_compile_time) - 1, &read_size, NULL);
		    if (file_error)
		    {
				MMIAPIFMM_CloseFile (file_handle);		    
				SCI_TRACE_LOW ("[MMIIM] MMIIM_LoadUdbEntry file_error");
				return FALSE;
			}
			
			MMIAPIFMM_CloseFile (file_handle);		  

			//检查时间是否正常			
			if (strcmp (read_compile_time, pcompile_time))
			{
				adapt_udb_file = FALSE;
				re_create_compile_time_file = TRUE;
				MMIFILE_DeleteFile (full_path_name, NULL);
			}
		}
		else
		{
			re_create_compile_time_file = TRUE;
		}
	}

	//根据当前手机编译时间，创建编译时间文件，来记录当前cstar使用的udb数据
	if (re_create_compile_time_file)
    {
	    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
	    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	    
	    MMIAPIFMM_CombineFullPath(
	        device_ptr,
	        device_len,
	        s_cstar_udb_dir,
	        MMIAPICOM_Wstrlen(s_cstar_udb_dir),
	        s_cstar_compile_time,
	        MMIAPICOM_Wstrlen(s_cstar_compile_time),
	        full_path_name,
	        &full_path_len
	    );

	    file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, 0, 0);
	    if (SFS_INVALID_HANDLE != file_handle)
	    {
	    	MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
			uint32  write_size  = 0;
			
			file_error = MMIAPIFMM_WriteFile (file_handle, pcompile_time, strlen(pcompile_time), &write_size, NULL);
			if (file_error)
			{
				SCI_TRACE_LOW ("[MMIIM] MMIIM_LoadUdbEntry file_error");
				return FALSE;
			}

			MMIAPIFMM_CloseFile(file_handle);
	    }
	}

	//读取or创建文件
    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    MMIAPIFMM_CombineFullPath(
        device_ptr,
        device_len,
        s_cstar_udb_dir,
        MMIAPICOM_Wstrlen(s_cstar_udb_dir),
        s_cstar_udb_file,
        MMIAPICOM_Wstrlen(s_cstar_udb_file),
        full_path_name,
        &full_path_len
    );

	if (adapt_udb_file == FALSE)	//版本不匹配，删除udb文件
	{
		MMIFILE_DeleteFile (full_path_name, NULL);
	}
	
    file_handle = MMIAPIFMM_CreateFile(full_path_name, 
        SFS_MODE_OPEN_ALWAYS | SFS_MODE_READ, 0, 0);

    if (SFS_INVALID_HANDLE != file_handle)
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);

        //有可能是从别的地方拷贝过来的文件，或者文件已经被坏，则不读此文件
        if (file_size == s_udb_entry.udb_len)
        {
            MMIAPIFMM_ReadFile(file_handle, s_udb_entry.udb_ptr,
                file_size, &read_len, PNULL);
            is_ret = TRUE;
        }
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIM_SaveUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_SaveUdbEntry()
{
    BOOLEAN is_ret = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_SDCARD;
    wchar *device_ptr = PNULL;
    uint16 device_len = 0;
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    uint32 write_len = 0;

    if (PNULL == s_udb_entry.udb_ptr
        || 0 == s_udb_entry.udb_len)
    {
        return is_ret;
    }
    
    //先找到有效的盘
    device_type = MMIAPIFMM_GetFirstValidSD();
    if(device_type < MMI_DEVICE_NUM)
    {
        device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(device_type);
        device_len = MMIAPIFMM_GetDevicePathLen(device_type);
    }
    else
    {    
        device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
        device_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    
    MMIAPIFMM_CombineFullPath(
        device_ptr,
        device_len,
        s_cstar_udb_dir,
        MMIAPICOM_Wstrlen(s_cstar_udb_dir),
        PNULL,
        0,
        full_path_name,
        &full_path_len
    );
    
    if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
    {
        MMIAPIFMM_CreateDir(full_path_name, full_path_len);
    }

    //set hide dir attribute
    MMIAPIFMM_SetAttr(full_path_name, full_path_len, 
        TRUE, FALSE, FALSE, FALSE);

    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    MMIAPIFMM_CombineFullPath(
        device_ptr,
        device_len,
        s_cstar_udb_dir,
        MMIAPICOM_Wstrlen(s_cstar_udb_dir),
        s_cstar_udb_file,
        MMIAPICOM_Wstrlen(s_cstar_udb_file),
        full_path_name,
        &full_path_len
    );
    
    if (MMIAPIFMM_IsFileExist(full_path_name, full_path_len))
    {
        MMIAPIFMM_DeleteFile(full_path_name, PNULL);
    }
    
    file_handle = MMIAPIFMM_CreateFile(full_path_name, 
        SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, 0, 0);

    if (SFS_INVALID_HANDLE != file_handle)
    {
        file_error = MMIAPIFMM_WriteFile(file_handle, 
            s_udb_entry.udb_ptr, s_udb_entry.udb_len, &write_len, PNULL);
    }

    MMIAPIFMM_CloseFile(file_handle);

    if (SFS_ERROR_NONE == file_error)
    {
        is_ret = TRUE;
    }
    return is_ret;
}

extern int FindUserCacheByIM(int iMethod, const CS_USER_PARAMS *pUserTotal, 
    CS_USER_PARAMS *pUserIM);

#endif

#ifdef IM_ENGINE_CSTAR
/*****************************************************************************/
//  Description : MMIIM_SaveUdbEntry
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIM_IME_FindUserCacheByIM(
    GUIIM_METHOD_T method,
    void **user_data,
    uint *user_data_size
)
{
#ifdef MMI_CSTAR_UDB_SUPPORT
    CS_USER_PARAMS UserTotal = {0};
    CS_USER_PARAMS UserIM = {0};
    GUIIM_METHOD_T new_method = GUIIM_M_NONE;
    CS_UINT imm_mode = IM_NONE;

    if (PNULL == s_udb_entry.udb_ptr
        || 0 == s_udb_entry.udb_len)
    {
        return;
    }

    new_method = MMITHEME_IMGetMethodByInput(method, GUIIM_INPUT_TYPE_KEYBOARD);
    imm_mode = MMIIM_ConvertMethod(new_method);

    if (IM_NONE == imm_mode)
    {
        return;
    }

    UserTotal.pUserCache = (char*)s_udb_entry.udb_ptr;
    UserTotal.iUserCacheSize = s_udb_entry.udb_len;
    if(FindUserCacheByIM(imm_mode, &UserTotal, &UserIM))
    {
        *user_data = UserIM.pUserCache;
        *user_data_size = UserIM.iUserCacheSize;
    }
    else
    {
        *user_data = PNULL;
        *user_data_size = 0;
    }
#else    
    *user_data = PNULL;
    *user_data_size = 0;
#endif
}
#endif

#if ((defined IM_ENGINE_CSTAR) || (defined IM_ENGINE_T9))
/*****************************************************************************/
//  Description : MMIIM_GetImKerState
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIIM_GetImKerState(void)
{
    return s_im_kernal_initialized;
}

/*****************************************************************************/
//  Description : MMIIM_SetImKerState
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIM_SetImKerState(uint16 im_initialized)
{
    s_im_kernal_initialized = im_initialized;
}
#endif
/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_DialogSeperateChar

Func:   判断是否是. !?

Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIIM_DialogSeperateChar (wchar c)
{
    if (c == '.' || c == '?' || c == '!' )
        return TRUE;

    return FALSE;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   MMIAPIIM_WordSeperateChar

Func:   判断是否是SeperateChar

Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIIM_WordSeperateChar (wchar c)
{
    if (c == '\n' || c == 0x3000)  
        return TRUE;

    return MMIAPIIM_DialogSeperateChar (c);
}
/*****************************************************************************/
//  Description : 判断将要输入的字符的前一个字符是否是.?!
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsPreUnderlineWordSeperateChar (MMI_HANDLE_T edit_handle)
{
	wchar cursor_char = 0;
	uint16 cursor_pos = 0;
	MMI_STRING_T string_info = {0};

	cursor_pos = GUIEDIT_GetCursorCurPos (edit_handle);
	GUIEDIT_GetString (edit_handle, &string_info);
	if (cursor_pos == 0)
	{
		return TRUE;
	}

	if (cursor_pos > 0 && cursor_pos <= string_info.wstr_len)
	{
		cursor_char = string_info.wstr_ptr[cursor_pos - 1];
	}

	while( cursor_pos > 1)
	{
	    if (MMIAPIIM_WordSeperateChar(cursor_char))
		{
			return TRUE;
		}
		else if(cursor_char == ' ')
		{
			cursor_pos--;
			if (cursor_pos <= string_info.wstr_len)
			{
				cursor_char = string_info.wstr_ptr[cursor_pos - 1];
			}
		}
		else
		{
			return FALSE;
		}
	}
	
    return FALSE;
}


/*****************************************************************************/
//  Description : 判断将要输入的字符的前一个字符是否是.?!
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_IsPreCurrentWordSeperateChar (MMI_HANDLE_T edit_handle)
{
	wchar cursor_char = 0;
	uint16 cursor_pos = 0;
	MMI_STRING_T string_info = {0};
	uint16 highlight_min_pos = 0;

    GUIEDIT_GetHighlightInfo(edit_handle,&highlight_min_pos,PNULL);

	cursor_pos = GUIEDIT_GetCursorCurPos (edit_handle);
	GUIEDIT_GetString (edit_handle, &string_info);
	if ((cursor_pos == 0)||(0 == highlight_min_pos))
	{
		return TRUE;
	}
	if (highlight_min_pos == cursor_pos)
	{		
		if (cursor_pos > 0 && cursor_pos <= string_info.wstr_len)
		{
			cursor_char = string_info.wstr_ptr[cursor_pos - 1];
		}
	}
	else
	{
		if (cursor_pos > 1 && cursor_pos <= string_info.wstr_len)
		{
			cursor_char = string_info.wstr_ptr[cursor_pos - 2];
		}
	}

	while( cursor_pos > 1)
	{
	    if (MMIAPIIM_WordSeperateChar(cursor_char))
		{
			return TRUE;
		}
		else if(cursor_char == ' ')
		{
			cursor_pos--;
			highlight_min_pos--;
			if (highlight_min_pos == cursor_pos)//???????λ???????????????underline????????????????ж??Сд
			{		
				if (cursor_pos <= string_info.wstr_len)
				{
					cursor_char = string_info.wstr_ptr[cursor_pos - 1];
				}
			}
			else
			{
				if (cursor_pos <= string_info.wstr_len)
				{
					cursor_char = string_info.wstr_ptr[cursor_pos - 2];
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	
    return FALSE;
}
/*****************************************************************************/
//  Description : 得到编辑器当前光标前一个字符，本来想将underline字符长度去掉的，
//				因不需要判断underline字符，故这个函数就不需要考虑underline字符了
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note: 
/*****************************************************************************/
PUBLIC wchar MMIIM_GetEditCursorChar (MMI_HANDLE_T edit_handle)
{
	wchar cursor_char = 0;
	uint16 cursor_pos = 0, underline_len = 0;
	MMI_STRING_T string_info = {0};
	
	cursor_pos = GUIEDIT_GetCursorCurPos (edit_handle);
	GUIEDIT_GetString (edit_handle, &string_info);

	if (cursor_pos > 0 && cursor_pos <= string_info.wstr_len)
	{
		cursor_char = string_info.wstr_ptr[cursor_pos - 1];
	}
	
	return cursor_char;
}



