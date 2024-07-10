/*******************************************************************************
** File Name: mmiim_base.h
** Author: Lianxiang.Zhou
** Date: 2009/08/08
** Copyright: 2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description: 应用部分的公用定义
********************************************************************************
** Edit History
**------------------------------------------------------------------------------
** DATE         NAME            DESCRIPTION
** 2009/08/08   Lianxiang.Zhou  Create.
*******************************************************************************/

#ifndef _MMIIM_BASE_H_
#define _MMIIM_BASE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafhelp.h"
#include "guiim_base.h"
#include "mmitheme_im.h"
#include "mmi_theme.h"
#if defined IM_ENGINE_SOGOU 
#include "guiimg.h"
#include "guiblock.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//触笔输入部分，首先必须是支持触屏的
#if defined(TOUCH_PANEL_SUPPORT)
    #define MMIIM_USE_TOUCH_PANEL

    //如果有手写内核支持，则支持手写
    #define MMIIM_USE_HANDWRITING
    #if defined(IM_HANDWRITING_HANWANG)
        #define MMIIM_USE_HANDWRITING_HANWANG
	#elif defined(IM_HANDWRITING_SOGOU)
		#define MMIIM_USE_HANDWRITING_XUNFEI
    #else
        #undef MMIIM_USE_HANDWRITING
    #endif

#if (defined MMIIM_VKB_FORBID)	//某些小屏工程需要配置为不支持虚拟键盘输入，但是支持手写

#else
#define MMIIM_VKB_SUPPORT
#endif
	
#endif

//此处需要根据系统的宏来决定开启那个ime的支持
#if defined IM_ENGINE_CSTAR
#define MMIIM_CORE_USE_CSTAR
#define MMIIM_ENGINE_EXIST
#elif defined IM_ENGINE_SOGOU 
#define MMIIM_CORE_USE_SOGOU
#define MMIIM_ENGINE_EXIST
#elif defined IM_ENGINE_T9 
#define MMIIM_CORE_USE_T9
#define MMIIM_ENGINE_EXIST
#elif defined IM_ENGINE_XT9 
#define MMIIM_CORE_USE_XT9
#define MMIIM_ENGINE_EXIST
#else
#define MMIIM_CORE_USE_SPIM
//#define MMIIM_ENGINE_EXIST
#endif

#define MMIIM_USE_MULTITAP

//multitap使用的数据，当前存在cstar的multitap数据和spread的数据，暂且固定使用spread的数据，
//因为发现cstar的部分数据存在缺失或其他的问题
#if (defined MMIIM_USE_MULTITAP)
//还是使用cstar的multitap数据吧，因智能输入字符使用按键如果与multitap数据不同，则PB 检索会有问题
#if (defined MMIIM_CORE_USE_CSTAR)      
#define MMIIM_CSTAR_MULTITAP
#elif (defined MMIIM_CORE_USE_SOGOU)
#define MMIIM_SOGOU_MULTITAP
#elif (defined MMIIM_CORE_USE_T9)
#define MMIIM_T9_MULTITAP
#elif (defined MMIIM_CORE_USE_XT9)
#define MMIIM_XT9_MULTITAP
#else
#define MMIIM_SPREAD_MULTITAP
#endif
#endif


#if defined(MMIIM_CORE_USE_CSTAR)
    #define MMIIM_USE_CSTAR

    #define MMIIM_USE_KB_CSTAR

    #if defined(MMIIM_USE_TOUCH_PANEL)
        #define MMIIM_USE_TP_CSTAR
        #define MMIIM_USE_ASSO_CSTAR
    #endif
#endif

#if defined(MMIIM_CORE_USE_SOGOU)
    #define MMIIM_USE_KB_SOGOU

    #if defined(MMIIM_USE_TOUCH_PANEL)
        #define MMIIM_USE_TP_SOGOU
    #endif
#endif

#if defined(MMIIM_CORE_USE_T9)
    #define MMIIM_USE_KB_T9

    #if defined(MMIIM_USE_TOUCH_PANEL)
        #define MMIIM_USE_TP_T9
    #endif
#endif

#if defined(MMIIM_CORE_USE_XT9)
    #define MMIIM_USE_KB_XT9

    #if defined(MMIIM_USE_TOUCH_PANEL)
        #define MMIIM_USE_TP_XT9
    #endif
#endif

#if defined(MMIIM_CORE_USE_SPIM)
        #define MMIIM_USE_SPIM_KEY
        #define MMIIM_USE_SPIM_QWERT

    #if defined(MMIIM_USE_TOUCH_PANEL)
        #define MMIIM_USE_SPIM_TP
    #endif
#endif




//字符输入法中使用的定时器id,可以修改为所需要的值,现在沿用t9定义的id
#define MMIIM_EXPL_TIMER_ID T9_EXPL_TIMER_ID
//字符输入法中定时器的定时时间
#define MMIIM_EXPL_TIMER_OUT_SHORT (500)
#define MMIIM_EXPL_TIMER_OUT_NORMAL (750)
#define MMIIM_EXPL_TIMER_OUT_LONG (1000)
#define MMIIM_EXPL_TIMER_OUT_DEFAULT MMIIM_EXPL_TIMER_OUT_NORMAL

#if ((defined IM_ENGINE_T9) || (defined IM_ENGINE_XT9))
typedef enum
{
    MMIIM_T9_CAP_SWITCH_LEAD2LOWER,
    MMIIM_T9_CAP_SWITCH_LOWER2LEAD,
    
    MMIIM_T9_CAP_SWITCH_MAX,
    
}MMIIM_T9_CAP_SWITCH_E;

enum
{
    MMIIM_MOVE_WORD_BY_WORD,
    MMIIM_MOVE_CHAR_BY_CHAR,
    MMIIM_MOVE_BY_MAX,
};
typedef uint8 MMIIM_MOVE_BY_TYPE;

#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//hindi characters
enum
{
    /*
    DEVA_ - DEVANAGARI_
    D_ - DIGIT_
    L_ - LETTER_
    LC_ - LETTER_CANDRA_
    LS_ - LETTER_SHORT_
    LV_ - LETTER_VOCALIC_
    S_ - SIGN_
    SS_ - STRESS_SIGN_
    VS_ - VOWEL_SIGN_
    VSC_ - VOWEL_SIGN_CANDRA_
    VSS_ - VOWEL_SIGN_SHORT_
    VSV_ - VOWEL_SIGN_VOCALIC_
    */

    /* VARIOUS SIGNS + INDEPENDENT VOWEL */
    DEVA_S_CANDRABINDU      = 0X0901,
    DEVA_S_ANUSVARA         = 0X0902,
    DEVA_S_VISARGA          = 0X0903,
    DEVA_LS_A               = 0X0904,
    DEVA_L_A                = 0X0905,
    DEVA_L_AA               = 0X0906,
    DEVA_L_I                = 0X0907,
    DEVA_L_II               = 0X0908,
    DEVA_L_U                = 0X0909,
    DEVA_L_UU               = 0X090A,
    DEVA_LV_R               = 0X090B,
    DEVA_LV_L               = 0X090C,
    DEVA_LC_E               = 0X090D,
    DEVA_LS_E               = 0X090E,
    DEVA_L_E                = 0X090F,
    DEVA_L_AI               = 0X0910,
    DEVA_LC_O               = 0X0911,
    DEVA_LS_O               = 0X0912,
    DEVA_L_O                = 0X0913,
    DEVA_L_AU               = 0X0914,

    /* CONSONANTS */
    DEVA_L_KA               = 0X0915,
    DEVA_L_KHA              = 0X0916,
    DEVA_L_GA               = 0X0917,
    DEVA_L_GHA              = 0X0918,
    DEVA_L_NGA              = 0X0919,
    DEVA_L_CA               = 0X091A,
    DEVA_L_CHA              = 0X091B,
    DEVA_L_JA               = 0X091C,
    DEVA_L_JHA              = 0X091D,
    DEVA_L_NYA              = 0X091E,
    DEVA_L_TTA              = 0X091F,
    DEVA_L_TTHA             = 0X0920,
    DEVA_L_DDA              = 0X0921,
    DEVA_L_DDHA             = 0X0922,
    DEVA_L_NNA              = 0X0923,
    DEVA_L_TA               = 0X0924,
    DEVA_L_THA              = 0X0925,
    DEVA_L_DA               = 0X0926,
    DEVA_L_DHA              = 0X0927,
    DEVA_L_NA               = 0X0928,
    DEVA_L_NNNA             = 0X0929,
    DEVA_L_PA               = 0X092A,
    DEVA_L_PHA              = 0X092B,
    DEVA_L_BA               = 0X092C,
    DEVA_L_BHA              = 0X092D,
    DEVA_L_MA               = 0X092E,
    DEVA_L_YA               = 0X092F,
    DEVA_L_RA               = 0X0930,
    DEVA_L_RRA              = 0X0931,
    DEVA_L_LA               = 0X0932,
    DEVA_L_LLA              = 0X0933,
    DEVA_L_LLLA             = 0X0934,
    DEVA_L_VA               = 0X0935,
    DEVA_L_SHA              = 0X0936,
    DEVA_L_SSA              = 0X0937,
    DEVA_L_SA               = 0X0938,
    DEVA_L_HA               = 0X0939,

    /* SIGNS */
    DEVA_S_NUKTA            = 0X093C,
    DEVA_S_AVAGRAHA         = 0X093D,

    /* DEPENDENT VOWELS */
    DEVA_VS_AA              = 0X093E,
    DEVA_VS_I               = 0X093F,
    DEVA_VS_II              = 0X0940,
    DEVA_VS_U               = 0X0941,
    DEVA_VS_UU              = 0X0942,
    DEVA_VSV_R              = 0X0943,
    DEVA_VSV_RR             = 0X0944,
    DEVA_VSC_E              = 0X0945,
    DEVA_VSS_E              = 0X0946,
    DEVA_VS_E               = 0X0947,
    DEVA_VS_AI              = 0X0948,
    DEVA_VSC_O              = 0X0949,
    DEVA_VSS_O              = 0X094A,
    DEVA_VS_O               = 0X094B,
    DEVA_VS_AU              = 0X094C,

    /* SIGNS */
    DEVA_S_VIRAMA           = 0X094D,
    DEVA_OM                 = 0X0950,
    DEVA_SS_UDATTA          = 0X0951,
    DEVA_SS_ANUDATTA        = 0X0952,
    DEVA_GRAVE_ACCENT       = 0X0953,
    DEVA_ACUTE_ACCENT       = 0X0954,

    /* ADDITIONAL CONSONANT */
    DEVA_L_QA               = 0X0958,
    DEVA_L_KHHA             = 0X0959,
    DEVA_L_GHHA             = 0X095A,
    DEVA_L_ZA               = 0X095B,
    DEVA_L_DDDHA            = 0X095C,
    DEVA_L_RHA              = 0X095D,
    DEVA_L_FA               = 0X095E,
    DEVA_L_YYA              = 0X095F,

    /* GENERIC ADDITIONS */
    DEVA_LV_RR              = 0X0960,
    DEVA_LV_LL              = 0X0961,
    DEVA_VSV_L              = 0X0962,
    DEVA_VSV_LL             = 0X0963,
    DEVA_DANDA              = 0X0964,
    DEVA_DOUBLE_DANDA       = 0X0965,

    /* DIGITS */
    DEVA_D_ZERO             = 0X0966,
    DEVA_D_ONE              = 0X0967,
    DEVA_D_TWO              = 0X0968,
    DEVA_D_THREE            = 0X0969,
    DEVA_D_FOUR             = 0X096A,
    DEVA_D_FIVE             = 0X096B,
    DEVA_D_SIX              = 0X096C,
    DEVA_D_SEVEN            = 0X096D,
    DEVA_D_EIGHT            = 0X096E,
    DEVA_D_NINE             = 0X096F,

    /* DEVANAGARI SPECIFIC ADDITOIN */
    DEVA_ABBREVIATION_SIGN  = 0X0970,
    DEVA_S_HIGH_SPACING_DOT = 0X0971,
    DEVA_LC_A               = 0X0972,
    DEVA_L_GGA              = 0X097B,
    DEVA_L_JJA              = 0X097C,
    DEVA_L_GLOTTAL_STOP     = 0X097D,
    DEVA_L_DDDA             = 0X097E,
    DEVA_L_BBA              = 0X097F,
};


enum
{
    /*
    C_ - CHARACTER_
    CS_ - CURRENCY_SYMBOL_
    D_ - DIGIT_
    */
    THAI_C_KO_KAI           = 0X0E01,
    THAI_C_KHO_KHAI         = 0X0E02,
    THAI_C_KHO_KHUAT        = 0X0E03,
    THAI_C_KHO_KHWAI        = 0X0E04,
    THAI_C_KHO_KHON         = 0X0E05,
    THAI_C_KHO_RAKHANG      = 0X0E06,
    THAI_C_NGO_NGU          = 0X0E07,
    THAI_C_CHO_CHAN         = 0X0E08,
    THAI_C_CHO_CHING        = 0X0E09,
    THAI_C_CHO_CHANG        = 0X0E0A,
    THAI_C_SO_SO            = 0X0E0B,
    THAI_C_CHO_CHOE         = 0X0E0C,
    THAI_C_YO_YING          = 0X0E0D,
    THAI_C_DO_CHADA         = 0X0E0E,
    THAI_C_TO_PATAK         = 0X0E0F,
    THAI_C_THO_THAN         = 0X0E10,
    THAI_C_THO_NANGMONTHO   = 0X0E11,
    THAI_C_THO_PHUTHAO      = 0X0E12,
    THAI_C_NO_NEN           = 0X0E13,
    THAI_C_DO_DEK           = 0X0E14,
    THAI_C_TO_TAO           = 0X0E15,
    THAI_C_THO_THUNG        = 0X0E16,
    THAI_C_THO_THAHAN       = 0X0E17,
    THAI_C_THO_THONG        = 0X0E18,
    THAI_C_NO_NU            = 0X0E19,
    THAI_C_BO_BAIMAI        = 0X0E1A,
    THAI_C_PO_PLA           = 0X0E1B,
    THAI_C_PHO_PHUNG        = 0X0E1C,
    THAI_C_FO_FA            = 0X0E1D,
    THAI_C_PHO_PHAN         = 0X0E1E,
    THAI_C_FO_FAN           = 0X0E1F,
    THAI_C_PHO_SAMPHAO      = 0X0E20,
    THAI_C_MO_MA            = 0X0E21,
    THAI_C_YO_YAK           = 0X0E22,
    THAI_C_RO_RUA           = 0X0E23,
    THAI_C_RU               = 0X0E24,
    THAI_C_LO_LING          = 0X0E25,
    THAI_C_LU               = 0X0E26,
    THAI_C_WO_WAEN          = 0X0E27,
    THAI_C_SO_SALA          = 0X0E28,
    THAI_C_SO_RUSI          = 0X0E29,
    THAI_C_SO_SUA           = 0X0E2A,
    THAI_C_HO_HIP           = 0X0E2B,
    THAI_C_LO_CHULA         = 0X0E2C,
    THAI_C_O_ANG            = 0X0E2D,
    THAI_C_HO_NOKHUK        = 0X0E2E,
    THAI_C_PAIYANNOI        = 0X0E2F,

    /* following vowels */
    THAI_C_SARA_A           = 0X0E30,
    THAI_C_MAI_HAN_AKAT     = 0X0E31,
    THAI_C_SARA_AA          = 0X0E32,
    THAI_C_SARA_AM          = 0X0E33,

    /* above vowels */
    THAI_C_SARA_I           = 0X0E34,
    THAI_C_SARA_II          = 0X0E35,
    THAI_C_SARA_UE          = 0X0E36,
    THAI_C_SARA_UEE         = 0X0E37,
    THAI_C_SARA_U           = 0X0E38,
    THAI_C_SARA_UU          = 0X0E39,

    /* below vowels */
    THAI_C_PHINTHU          = 0X0E3A,

    /* currency symbol */
    THAI_CS_BAHT            = 0X0E3F,

    /* leading vowels */
    THAI_C_SARA_E           = 0X0E40,
    THAI_C_SARA_AE          = 0X0E41,
    THAI_C_SARA_O           = 0X0E42,
    THAI_C_SARA_AI_MAIMUAN  = 0X0E43,
    THAI_C_SARA_AI_MAIMALAI = 0X0E44,

    /* signs */
    THAI_C_LAKKHANGYAO      = 0X0E45,

    /* signs repetition */
    THAI_C_MAIYAMOK         = 0X0E46,

    /* above vowels */
    THAI_C_MAITAIKHU        = 0X0E47,

    /* tone mark */
    THAI_C_MAI_EK           = 0X0E48,
    THAI_C_MAI_THO          = 0X0E49,
    THAI_C_MAI_TRI          = 0X0E4A,
    THAI_C_MAI_CHATTAWA     = 0X0E4B,

    /* sign - cancellation mark */
    THAI_C_THANTHAKHAT      = 0X0E4C,

    /* sign - final nasal */
    THAI_C_NIKHAHIT         = 0X0E4D,

    /* sign */
    THAI_C_YAMAKKAN         = 0X0E4E,

    /* sign - used as a bullet */
    THAI_C_FONGMAN          = 0X0E4F,

    /* Thai digital */
    THAI_D_ZERO             = 0X0E50,
    THAI_D_ONE              = 0X0E51,
    THAI_D_TWO              = 0X0E52,
    THAI_D_THREE            = 0X0E53,
    THAI_D_FOUR             = 0X0E54,
    THAI_D_FIVE             = 0X0E55,
    THAI_D_SIX              = 0X0E56,
    THAI_D_SEVEN            = 0X0E57,
    THAI_D_EIGHT            = 0X0E58,
    THAI_D_NINE             = 0X0E59,

    /* sign */
    THAI_C_ANGKHANKHU       = 0X0E5A,
    THAI_C_KHOMUT           = 0X0E5B,
};

#ifdef IM_TELUGU_SUPPORT
enum
{
	  /* DEVANAGARI SIGNS */
		TELUGU_DEVANAGARI_S_DANDA		= 0x0964,
		TELUGU_DEVANAGARI_S_DDANDA		= 0x0965,

    /* VARIOUS SIGNS */

    TELUGU_S_CHANDRA_BINDU = 0xc01,
    TELUGU_S_ANUSVARA = 0xc02,
    TELUGU_S_VISARGA = 0xc03,

    /* INDEPENDENT VOWEL */

    TELUGU_IV_A = 0xc05,
    TELUGU_IV_AA = 0xc06,
    TELUGU_IV_I = 0xc07,
    TELUGU_IV_II = 0xc08,
    TELUGU_IV_U = 0xc09,
    TELUGU_IV_UU = 0xc0a,
    TELUGU_IV_VR = 0xc0b,
    TELUGU_IV_VL = 0xc0c,
    TELUGU_IV_E = 0xc0e,
    TELUGU_IV_EE = 0xc0f,
    TELUGU_IV_AI = 0xc10,
    TELUGU_IV_O = 0xc12,
    TELUGU_IV_OO = 0xc13,
    TELUGU_IV_AU = 0xc14,

    /*  CONSONANTS  */

    TELUGU_C_KA = 0xc15,
    TELUGU_C_KHA = 0xc16,
    TELUGU_C_GA = 0xc17,
    TELUGU_C_GHA = 0xc18,
    TELUGU_C_NGA = 0xc19,
    TELUGU_C_CA = 0xc1a,
    TELUGU_C_CHA = 0xc1b,
    TELUGU_C_JA = 0xc1c,
    TELUGU_C_JHA = 0xc1d,
    TELUGU_C_NYA = 0xc1e,
    TELUGU_C_TTA = 0xc1f,
    TELUGU_C_TTHA = 0xc20,
    TELUGU_C_DDA = 0xc21,
    TELUGU_C_DDHA = 0xc22,
    TELUGU_C_NNA = 0xc23,
    TELUGU_C_TA = 0xc24,
    TELUGU_C_THA = 0xc25,
    TELUGU_C_DA = 0xc26,
    TELUGU_C_DHA = 0xc27,
    TELUGU_C_NA = 0xc28,
    TELUGU_C_PA = 0xc2a,
    TELUGU_C_PHA = 0xc2b,
    TELUGU_C_BA = 0xc2c,
    TELUGU_C_BHA = 0xc2d,
    TELUGU_C_MA = 0xc2e,
    TELUGU_C_YA = 0xc2f,
    TELUGU_C_RA = 0xc30,
    TELUGU_C_RRA = 0xc31,
    TELUGU_C_LA = 0xc32,
    TELUGU_C_LLA = 0xc33,
    TELUGU_C_VA = 0xc35,
    TELUGU_C_SHA = 0xc36,
    TELUGU_C_SSA = 0xc37,
    TELUGU_C_SA = 0xc38,
    TELUGU_C_HA = 0xc39,

    /*  DEPENDENT VOWELS  */

    TELUGU_DV_AA = 0xc3e,
    TELUGU_DV_I = 0xc3f,
    TELUGU_DV_II = 0xc40,
    TELUGU_DV_U = 0xc41,
    TELUGU_DV_UU = 0xc42,
    TELUGU_DV_VR = 0xc43,
    TELUGU_DV_VRR = 0xc44,
    TELUGU_DV_E = 0xc46,
    TELUGU_DV_EE = 0xc47,
    TELUGU_DV_AI = 0xc48,
    TELUGU_DV_O = 0xc4a,
    TELUGU_DV_OO = 0xc4b,
    TELUGU_DV_AU = 0xc4c,
    TELUGU_S_HALANT = 0xc4d,
    TELUGU_ADV_EE = 0xc55,
    TELUGU_ADV_AI = 0xc56,
    TELUGU_IV_VRR = 0xc60,
    TELUGU_IV_VLL = 0xc61,

    TELUGU_S_DANDA = 0xc64,
    TELUGU_S_DDANDA = 0xc65,
    /*  DIGITS  */

    TELUGU_D_ZERO = 0xc66,
    TELUGU_D_ONE = 0xc67,
    TELUGU_D_TWO = 0xc68,
    TELUGU_D_THREE = 0xc69,
    TELUGU_D_FOUR = 0xc6a,
    TELUGU_D_FIVE = 0xc6b,
    TELUGU_D_SIX = 0xc6c,
    TELUGU_D_SEVEN = 0xc6d,
    TELUGU_D_EIGHT = 0xc6e,
    TELUGU_D_NINE = 0xc6f
} ;
#endif

#ifdef IM_MARATHI_SUPPORT
enum
{   /* VARIOUS SIGNS + INDEPENDENT VOWEL */
    MARATHI_S_CHANDRA_BINDU		= 0x0901,
    MARATHI_S_BINDU				= 0x0902,
    MARATHI_S_VISARGA			= 0x0903,
    MARATHI_NIV_A				= 0x0904,
    MARATHI_IV_A				= 0x0905,
    MARATHI_IV_AA				= 0x0906,
    MARATHI_IV_I				= 0x0907,
    MARATHI_IV_II				= 0x0908,
    MARATHI_IV_U				= 0x0909,
    MARATHI_IV_UU				= 0x090a,
    MARATHI_IV_R				= 0x090b,
    MARATHI_NIV_L				= 0x090c,
    MARATHI_NIV_CE				= 0x090d,
    MARATHI_NIV_SE				= 0x090e,
    MARATHI_IV_E				= 0x090f,
    MARATHI_IV_AI				= 0x0910,
    MARATHI_NIV_CO				= 0x0911,
    MARATHI_NIV_SO				= 0x0912,
    MARATHI_IV_O				= 0x0913,
    MARATHI_IV_AU				= 0x0914,

    /* CONSONANTS */
    MARATHI_C_KA				= 0x0915,
    MARATHI_C_KHA				= 0x0916,
    MARATHI_C_GA				= 0x0917,
    MARATHI_C_GHA				= 0x0918,
    MARATHI_C_NGA				= 0x0919,
    MARATHI_C_CHA				= 0x091a,
    MARATHI_C_CHHA				= 0x091b,
    MARATHI_C_JA				= 0x091c,
    MARATHI_C_JHA				= 0x091d,
    MARATHI_C_NYA				= 0x091e,
    MARATHI_C_TTA				= 0x091f,
    MARATHI_C_TTHA				= 0x0920,
    MARATHI_C_DDA				= 0x0921,
    MARATHI_C_DDHA				= 0x0922,
    MARATHI_C_NNA				= 0x0923,
    MARATHI_C_TA				= 0x0924,
    MARATHI_C_THA				= 0x0925,
    MARATHI_C_DA				= 0x0926,
    MARATHI_C_DHA				= 0x0927,
    MARATHI_C_NA				= 0x0928,
    MARATHI_C_NNNA				= 0x0929,
    MARATHI_C_PA				= 0x092a,
    MARATHI_C_PHA				= 0x092b,
    MARATHI_C_BA				= 0x092c,
    MARATHI_C_BHA				= 0x092d,
    MARATHI_C_MA				= 0x092e,
    MARATHI_C_YA				= 0x092f,
    MARATHI_C_RA				= 0x0930,
    MARATHI_C_RRA				= 0x0931,
    MARATHI_C_LA				= 0x0932,
    MARATHI_C_LLA				= 0x0933,
    MARATHI_C_LLLA				= 0x0934,
    MARATHI_C_VA				= 0x0935,
    MARATHI_C_SHA				= 0x0936,
    MARATHI_C_SSA				= 0x0937,
    MARATHI_C_SA				= 0x0938,
    MARATHI_C_HA				= 0x0939,
    MARATHI_N_BLANK1			= 0x093a,
    MARATHI_N_BLANK2			= 0x093b,

    /* SIGNS */
    MARATHI_S_NUKTA				= 0x093c,
    MARATHI_NS_AVAGRAHA			= 0x093d,

    /* DEPENDENT VOWELS */
    MARATHI_DV_AA				= 0x093e,
    MARATHI_DV_I				= 0x093f,
    MARATHI_DV_II				= 0x0940,
    MARATHI_DV_U				= 0x0941,
    MARATHI_DV_UU				= 0x0942,
    MARATHI_DV_R				= 0x0943,
    MARATHI_NDV_RR				= 0x0944,
    MARATHI_DV_CE				= 0x0945,
    MARATHI_NDV_E				= 0x0946,
    MARATHI_DV_E				= 0x0947,
    MARATHI_DV_AI				= 0x0948,
    MARATHI_NDV_CO				= 0x0949,
    MARATHI_NDV_O				= 0x094a,
    MARATHI_DV_O				= 0x094b,
    MARATHI_DV_AU				= 0x094c,

    /* SIGNS */
    MARATHI_S_HALANT			= 0x094d,
    MARATHI_NS_RESERVED1		= 0x094e,
    MARATHI_NS_RESERVED2		= 0x094f,
    MARATHI_S_OM				= 0x0950,
    MARATHI_NS_UDATTA			= 0x0951,
    MARATHI_NS_ANUDATTA			= 0x0952,
    MARATHI_NS_GACCENT			= 0x0953,
    MARATHI_NS_AACCENT			= 0x0954,
    MARATHI_N_BLANK5			= 0x0955,
    MARATHI_N_BLANK6			= 0x0956,
    MARATHI_N_BLANK7			= 0x0957,

    /* ADDITIONAL CONSONANT */
    MARATHI_AC_QA				= 0x0958,
    MARATHI_AC_KHHA				= 0x0959,
    MARATHI_AC_GHHA				= 0x095a,
    MARATHI_AC_ZA				= 0x095b,
    MARATHI_AC_DDDHA			= 0x095c,
    MARATHI_AC_RHA				= 0x095d,
    MARATHI_AC_FA				= 0x095e,
    MARATHI_AC_YYA				= 0x095f,

    /* GENERIC ADDITIONS */
    MARATHI_NG_RR				= 0x0960,
    MARATHI_NG_LL				= 0x0961,
    MARATHI_NG_L				= 0x0962,
    MARATHI_NG_VLL				= 0x0963,
    MARATHI_NG_D				= 0x0964,
    MARATHI_NG_DD				= 0x0965,

    /* DIGITS */
    MARATHI_D_ZERO				= 0x0966,
    MARATHI_D_ONE				= 0x0967,
    MARATHI_D_TWO				= 0x0968,
    MARATHI_D_THREE				= 0x0969,
    MARATHI_D_FOUR				= 0x096a,
    MARATHI_D_FIVE				= 0x096b,
    MARATHI_D_SIX				= 0x096c,
    MARATHI_D_SEVEN				= 0x096d,
    MARATHI_D_EIGHT				= 0x096e,
    MARATHI_D_NINE				= 0x096f,
    /* DEVANAGARI SPECIFIC ADDITOIN */
    MARATHI_S_ABBR				= 0x0970
};
#endif



#ifdef IM_TAMIL_SUPPORT
enum
{
		/* DEVANAGARI SIGNS */
		TAMIL_DEVANAGARI_S_DANDA		= 0x0964,
		TAMIL_DEVANAGARI_S_DDANDA		= 0x0965,
	/* VARIOUS SIGNS + INDEPENDENT VOWEL */
		TAMIL_S_ANUSVARA = 0xb82,
    TAMIL_S_VISARGA = 0xb83,
    TAMIL_IV_A = 0xb85,
    TAMIL_IV_AA = 0xb86,
    TAMIL_IV_I = 0xb87,
    TAMIL_IV_II = 0xb88,
    TAMIL_IV_U = 0xb89,
    TAMIL_IV_UU = 0xb8a,

    TAMIL_IV_E = 0xb8e,
    TAMIL_IV_EE = 0xb8f,
    TAMIL_IV_AI = 0xb90,
    TAMIL_IV_O = 0xb92,
    TAMIL_IV_OO = 0xb93,
    TAMIL_IV_AU = 0xb94,

    /* CONSONANTS */
    TAMIL_C_KA = 0xb95,
    TAMIL_C_NGA = 0xb99,
    TAMIL_C_CHA = 0xb9a,
    TAMIL_C_JA = 0xb9c,
    TAMIL_C_NYA = 0xb9e,
    TAMIL_C_TTA = 0xb9f,
    TAMIL_C_NNA = 0xba3,
    TAMIL_C_TA = 0xba4,
    TAMIL_C_NA = 0xba8,
    TAMIL_C_NNNA = 0xba9,
    TAMIL_C_PA = 0xbaa,
    TAMIL_C_MA = 0xbae,
    TAMIL_C_YA = 0xbaf,
    TAMIL_C_RA = 0xbb0,
    TAMIL_C_RRA = 0xbb1,
    TAMIL_C_LA = 0xbb2,
    TAMIL_C_LLA = 0xbb3,
    TAMIL_C_LLLA = 0xbb4,
    TAMIL_C_VA = 0xbb5,
    TAMIL_C_SSA = 0xbb7,
    TAMIL_C_SA = 0xbb8,
    TAMIL_C_HA = 0xbb9,

    /* DEPENDENT VOWELS */
    TAMIL_DV_AA = 0xbbe,
    TAMIL_DV_I = 0xbbf,
    TAMIL_DV_II = 0xbc0,
    TAMIL_DV_U = 0xbc1,
    TAMIL_DV_UU = 0xbc2,
    TAMIL_DV_E = 0xbc6,
    TAMIL_DV_EE = 0xbc7,
    TAMIL_DV_AI = 0xbc8,
    TAMIL_DV_O = 0xbca,
    TAMIL_DV_OO = 0xbcb,
    TAMIL_DV_AU = 0xbcc,

    /* SIGNS */
    TAMIL_S_HALANT = 0xbcd,

    /* APPEND DEPENDENT VOWEL */
    TAMIL_AD_AU = 0xbd7,

    TAMIL_S_DANDA = 0xbe4,
    TAMIL_S_DDANDA = 0xbe5,
    /* DIGITS */
    TAMIL_D_ZERO = 0xbe6,
    TAMIL_D_ONE = 0xbe7,
    TAMIL_D_TWO = 0xbe8,
    TAMIL_D_THREE = 0xbe9,
    TAMIL_D_FOUR = 0xbea,
    TAMIL_D_FIVE = 0xbeb,
    TAMIL_D_SIX = 0xbec,
    TAMIL_D_SEVEN = 0xbed,
    TAMIL_D_EIGHT = 0xbee,
    TAMIL_D_NINE = 0xbef,

    /* TAMIL NUMERICS */
    TAMIL_D_TEN = 0xbf0,
    TAMIL_D_HUNDRED = 0xbf1,
    TAMIL_D_THOUSAND = 0xbf2,

    /* TAMIL SYMBOLS */
    TAMIL_OS_DAY = 0xbf3,
    TAMIL_OS_MONTH = 0xbf4,
    TAMIL_OS_YEAR = 0xbf5,
    TAMIL_OS_DEBIT = 0xbf6,
    TAMIL_OS_CREDIT = 0xbf7,
    TAMIL_OS_ABOVE = 0xbf8,

    /* CURRENCY SYMBOL */
    TAMIL_OS_RUPEE = 0xbf9,

    /* TAMIL SYMBOLS */
    TAMIL_OS_NUMERNAL = 0xbfa
};
#endif


#ifdef IM_GUJARATI_SUPPORT
enum
{
	  /* DEVANAGARI SIGNS */
		GUJARATI_DEVANAGARI_S_DANDA		= 0x0964,
		GUJARATI_DEVANAGARI_S_DDANDA		= 0x0965,
		
	/* VARIOUS SIGNS + INDEPENDENT VOWEL */

  GUJARATI_S_CHANDRA_BINDU = 0xa81,
  GUJARATI_S_BINDU = 0xa82,
  GUJARATI_S_VISARGA = 0xa83,
	GUJARATI_IV_A		     =	0xa85,
	GUJARATI_IV_AA		     =	0xa86,
	GUJARATI_IV_I		     =	0xa87,
	GUJARATI_IV_II		     =	0xa88,
	GUJARATI_IV_U		     =	0xa89,
	GUJARATI_IV_UU		     =	0xa8a,
	GUJARATI_IV_VR		     =	0xa8b,
	GUJARATI_IV_VL		     =	0xa8c,
	GUJARATI_IV_CE		     =	0xa8d,
	GUJARATI_IV_E		     =	0xa8f,
	GUJARATI_IV_AI		     =	0xa90,
	GUJARATI_IV_CO		     =	0xa91,
	GUJARATI_IV_O		     =	0xa93,
	GUJARATI_IV_AU		     =	0xa94,

	/*  CONSONANTS  */

	GUJARATI_C_KA		     =	0xa95,
	GUJARATI_C_KHA		     =	0xa96,
	GUJARATI_C_GA		     =	0xa97,
	GUJARATI_C_GHA		     =	0xa98,
	GUJARATI_C_NGA		     =	0xa99,
	GUJARATI_C_CA		     =	0xa9a,
	GUJARATI_C_CHA		     =	0xa9b,
	GUJARATI_C_JA		     =	0xa9c,
	GUJARATI_C_JHA		     =	0xa9d,
	GUJARATI_C_NYA		     =	0xa9e,
	GUJARATI_C_TTA		     =	0xa9f,
	GUJARATI_C_TTHA		     =	0xaa0,
	GUJARATI_C_DDA		     =	0xaa1,
	GUJARATI_C_DDHA		     =	0xaa2,
	GUJARATI_C_NNA		     =	0xaa3,
	GUJARATI_C_TA		     =	0xaa4,
	GUJARATI_C_THA		     =	0xaa5,
	GUJARATI_C_DA		     =	0xaa6,
	GUJARATI_C_DHA		     =	0xaa7,
	GUJARATI_C_NA		     =	0xaa8,
	GUJARATI_C_PA		     =	0xaaa,
	GUJARATI_C_PHA		     =	0xaab,
	GUJARATI_C_BA		     =	0xaac,
	GUJARATI_C_BHA		     =	0xaad,
	GUJARATI_C_MA		     =	0xaae,
	GUJARATI_C_YA		     =	0xaaf,
	GUJARATI_C_RA		     =	0xab0,
	GUJARATI_C_LA		     =	0xab2,
	GUJARATI_C_LLA		     =	0xab3,
	GUJARATI_C_VA		     =	0xab5,
	GUJARATI_C_SHA		     =	0xab6,
	GUJARATI_C_SSA		     =	0xab7,
	GUJARATI_C_SA		     =	0xab8,
	GUJARATI_C_HA		     =	0xab9,

	GUJARATI_S_NUKTA		     =	0xabc,
	GUJARATI_S_AVAGRAHA		     =	0xabd,

	/*  DEPENDENT VOWELS  */

	GUJARATI_DV_AA		     =	0xabe,
	GUJARATI_DV_I		     =	0xabf,
	GUJARATI_DV_II		     =	0xac0,
	GUJARATI_DV_U		     =	0xac1,
	GUJARATI_DV_UU		     =	0xac2,
	GUJARATI_DV_VR		     =	0xac3,
	GUJARATI_DV_VRR		     =	0xac4,
	GUJARATI_DV_CE		     =	0xac5,
	GUJARATI_DV_E		     =	0xac7,
	GUJARATI_DV_AI		     =	0xac8,
	GUJARATI_DV_CO		     =	0xac9,
	GUJARATI_DV_O		     =	0xacb,
	GUJARATI_DV_AU		     =	0xacc,
	GUJARATI_S_HALANT		     =	0xacd,
	GUJARATI_S_OM		     =	0xad0,
	GUJARATI_IV_VRR		     =	0xae0,
	GUJARATI_IV_VLL		     =	0xae1,
	GUJARATI_S_VL		     =	0xae2,
	GUJARATI_S_VLL		     =	0xae3,
	GUJARATI_DEVANAGARI_D		     =	0xae4,
	GUJARATI_DEVANAGARI_DD		     =	0xae5,

	/*  DIGITS  */

	GUJARATI_D_ZERO		     =	0xae6,
	GUJARATI_D_ONE		     =	0xae7,
	GUJARATI_D_TWO		     =	0xae8,
	GUJARATI_D_THREE		     =	0xae9,
	GUJARATI_D_FOUR		     =	0xaea,
	GUJARATI_D_FIVE		     =	0xaeb,
	GUJARATI_D_SIX		     =	0xaec,
	GUJARATI_D_SEVEN		     =	0xaed,
	GUJARATI_D_EIGHT		     =	0xaee,
	GUJARATI_D_NINE		     =	0xaef,
	GUJARATI_D_CS		     =	0xaf1

};
#endif


#ifdef IM_KANNADA_SUPPORT
enum
{   
	/* DEVANAGARI SIGNS */
	KANNADA_DEVANAGARI_S_DANDA			= 0x0964,
	KANNADA_DEVANAGARI_S_DOUBLE_DANDA		= 0x0965,

	/* VARIOUS SIGNS */
    KANNADA_S_ANUSVARA			= 0x0C82,
	KANNADA_S_VISARGA			= 0x0C83,

	/* BLANK */
	//N_BLANK1					= 0x0C84,

	/* INDEPENDENT VOWELS */
	KANNADA_IV_A				= 0x0C85,
	KANNADA_IV_AA				= 0x0C86,
	KANNADA_IV_I				= 0x0C87,
	KANNADA_IV_II				= 0x0C88,
	KANNADA_IV_U				= 0x0C89,
	KANNADA_IV_UU				= 0x0C8A,
	KANNADA_IV_VR				= 0x0C8B,
	KANNADA_IV_VL				= 0x0C8C,
	//NIV_RESERVED1				= 0x0C8D,/* RESERVED */
	KANNADA_IV_E				= 0x0C8E,
	KANNADA_IV_EE				= 0x0C8F,
	KANNADA_IV_AI				= 0x0C90,
	//NIV_RESERVED2				= 0x0C91,/* RESERVED */
	KANNADA_IV_O				= 0x0C92,
	KANNADA_IV_OO				= 0x0C93,
	KANNADA_IV_AU				= 0x0C94,

    /* CONSONANTS */
    KANNADA_C_KA				= 0x0C95,
    KANNADA_C_KHA				= 0x0C96,
    KANNADA_C_GA				= 0x0C97,
    KANNADA_C_GHA				= 0x0C98,
    KANNADA_C_NGA				= 0x0C99,
    KANNADA_C_CA				= 0x0C9A,
    KANNADA_C_CHA				= 0x0C9B,
    KANNADA_C_JA				= 0x0C9C,
    KANNADA_C_JHA				= 0x0C9D,
    KANNADA_C_NYA				= 0x0C9E,
    KANNADA_C_TTA				= 0x0C9F,
    KANNADA_C_TTHA				= 0x0CA0,
    KANNADA_C_DDA				= 0x0CA1,
    KANNADA_C_DDHA				= 0x0CA2,
    KANNADA_C_NNA				= 0x0CA3,
    KANNADA_C_TA				= 0x0CA4,
    KANNADA_C_THA				= 0x0CA5,
    KANNADA_C_DA				= 0x0CA6,
    KANNADA_C_DHA				= 0x0CA7,
    KANNADA_C_NA				= 0x0CA8,
	//NC_RESERVED1				= 0x0CA9,/* RESERVED */
    KANNADA_C_PA				= 0x0CAA,
    KANNADA_C_PHA				= 0x0CAB,
    KANNADA_C_BA				= 0x0CAC,
    KANNADA_C_BHA				= 0x0CAD,
    KANNADA_C_MA				= 0x0CAE,
    KANNADA_C_YA				= 0x0CAF,
    KANNADA_C_RA				= 0x0CB0,
    KANNADA_C_RRA				= 0x0CB1,
    KANNADA_C_LA				= 0x0CB2,
    KANNADA_C_LLA				= 0x0CB3,
	//NC_RESERVED2				= 0x0CB4,/* RESERVED */
    KANNADA_C_VA				= 0x0CB5,
    KANNADA_C_SHA				= 0x0CB6,
    KANNADA_C_SSA				= 0x0CB7,
    KANNADA_C_SA				= 0x0CB8,
    KANNADA_C_HA				= 0x0CB9,
    
    /* BLANK */
	//N_BLANK2					= 0x0CBA,
	//N_BLANK3					= 0x0CBB,
	
	/* VARIOUS SIGNS */
    KANNADA_S_NUKTA				= 0x0CBC,
    KANNADA_S_AVAGRAHA			= 0x0CBD,

    /* DEPENDENT VOWELS */
    KANNADA_DV_AA				= 0x0CBE,
    KANNADA_DV_I				= 0x0CBF,
    KANNADA_DV_II				= 0x0CC0,
    KANNADA_DV_U				= 0x0CC1,
    KANNADA_DV_UU				= 0x0CC2,
    KANNADA_DV_VR				= 0x0CC3,
    KANNADA_DV_VRR				= 0x0CC4,
	//NDV_RESERVED1				= 0x0CC5,/* RESERVED */
    KANNADA_DV_E				= 0x0CC6,
    KANNADA_DV_EE				= 0x0CC7,
    KANNADA_DV_AI				= 0x0CC8,
	//NDV_RESERVED2				= 0x0CC9,/* RESERVED */
    KANNADA_DV_O				= 0x0CCA,
	KANNADA_DV_OO				= 0x0CCB,
    KANNADA_DV_AU				= 0x0CCC,

	/* SIGNS */
    KANNADA_S_HALANT			= 0x0CCD,

	/* BLANK */
    //N_BLANK4					= 0x0CCE,
	//N_BLANK5					= 0x0CCF,
	//N_BLANK6					= 0x0CD0,
	//N_BLANK7					= 0x0CD1,
	//N_BLANK8					= 0x0CD2,
	//N_BLANK9					= 0x0CD3,
	//N_BLANK10					= 0x0CD4,
	
	/* VARIOUS SIGNS */
	KANNADA_S_LENGTH_MARK		= 0x0CD5,
	KANNADA_S_AI_LENGTH_MARK	= 0x0CD6,

	/* BLANK */
   // N_BLANK11					= 0x0CD7,
	//N_BLANK12					= 0x0CD8,
	//N_BLANK13					= 0x0CD9,
	//N_BLANK14					= 0x0CDA,
	//N_BLANK15					= 0x0CDB,
	//N_BLANK16					= 0x0CDC,
	//N_BLANK17					= 0x0CDD,

	/* ADDITIONAL CONSONANT */
    KANNADA_AC_FA				= 0x0CDE,/*obslete historic letter */

	/* BLANK */
    //N_BLANK18					= 0x0CDF,
   

    /* GENERIC ADDITION */
    KANNADA_IV_VRR				= 0x0CE0, 
    KANNADA_IV_VLL				= 0x0CE1, 
	KANNADA_IV_VVL				= 0x0CE2, 
	KANNADA_IV_VVLL				= 0x0CE3, 

	/* BLANK */
    KANNADA_S_DANDA				= 0x0CE4,
	KANNADA_S_DOUBLE_DANDA		= 0x0CE5,
    
    /* DIGITS */
    KANNADA_D_ZERO				= 0x0CE6,
    KANNADA_D_ONE				= 0x0CE7,
    KANNADA_D_TWO				= 0x0CE8,
    KANNADA_D_THREE				= 0x0CE9,
    KANNADA_D_FOUR				= 0x0CEA,
    KANNADA_D_FIVE				= 0x0CEB,
    KANNADA_D_SIX				= 0x0CEC,
    KANNADA_D_SEVEN				= 0x0CED,
    KANNADA_D_EIGHT				= 0x0CEE,
    KANNADA_D_NINE				= 0x0CEF,
    
	/* BLANK */
    //N_BLANK21					= 0x0CF0,

	/* VARIOUS SIGNS */
	KANNADA_S_JIHVAMULIYA		= 0x0CF1,
	KANNADA_S_UPADHMANIYA		= 0x0CF2,

	/* END OF ENUM */
    KANNADA_DUMMY_END
};
#endif


#ifdef IM_MALAYALAM_SUPPORT
enum
{
	/* MALAYALAM SIGNS */
    MALAYALAM_DEVANAGARI_S_DANDA = 0x0964,
    MALAYALAM_DEVANAGARI_S_DOUBLE_DANDA = 0x0965,
	
	/* VARIOUS SIGNS */
	MALAYALAM_S_ANUSVARA	     =	0x0D02,
	MALAYALAM_S_VISARGA		     =	0x0D03,
	
	/* INDEPENDENT VOWEL */
	MALAYALAM_IV_A		         =	0x0D05,
	MALAYALAM_IV_AA		         =	0x0D06,
	MALAYALAM_IV_I		         =	0x0D07,
	MALAYALAM_IV_II		         =	0x0D08,
	MALAYALAM_IV_U		         =	0x0D09,
	MALAYALAM_IV_UU		         =	0x0D0A,
	MALAYALAM_IV_VR		         =	0x0D0B,
	MALAYALAM_IV_VL		         =	0x0D0C,
	MALAYALAM_IV_E		         =	0x0D0E,
	MALAYALAM_IV_EE		         =	0x0D0F,
	MALAYALAM_IV_AI		         =	0x0D10,
	MALAYALAM_IV_O		         =	0x0D12,
	MALAYALAM_IV_OO		         =	0x0D13,
	MALAYALAM_IV_AU		         =	0x0D14,
	
	/* CONSONANTS */
	MALAYALAM_C_KA		         =	0x0D15,
	MALAYALAM_C_KHA		         =	0x0D16,
	MALAYALAM_C_GA		         =	0x0D17,
	MALAYALAM_C_GHA		         =	0x0D18,
	MALAYALAM_C_NGA		         =	0x0D19,
	MALAYALAM_C_CA		         =	0x0D1A,
	MALAYALAM_C_CHA		         =	0x0D1B,
	MALAYALAM_C_JA		         =	0x0D1C,
	MALAYALAM_C_JHA		         =	0x0D1D,
	MALAYALAM_C_NYA		         =	0x0D1E,
	MALAYALAM_C_TTA		         =	0x0D1F,
	MALAYALAM_C_TTHA		     =	0x0D20,
	MALAYALAM_C_DDA		         =	0x0D21,
	MALAYALAM_C_DDHA		     =	0x0D22,
	MALAYALAM_C_NNA		         =	0x0D23,
	MALAYALAM_C_TA		         =	0x0D24,
	MALAYALAM_C_THA		         =	0x0D25,
	MALAYALAM_C_DA		         =	0x0D26,
	MALAYALAM_C_DHA		         =	0x0D27,
	MALAYALAM_C_NA		         =	0x0D28,
	MALAYALAM_C_PA		         =	0x0D2A,
	MALAYALAM_C_PHA		         =	0x0D2B,
	MALAYALAM_C_BA		         =	0x0D2C,
	MALAYALAM_C_BHA		         =	0x0D2D,
	MALAYALAM_C_MA		         =	0x0D2E,
	MALAYALAM_C_YA		         =	0x0D2F,
	MALAYALAM_C_RA		         =	0x0D30,
	MALAYALAM_C_RRA		         =	0x0D31,
	MALAYALAM_C_LA		         =	0x0D32,
	MALAYALAM_C_LLA		         =	0x0D33,
	MALAYALAM_C_LLLA		     =	0x0D34,
	MALAYALAM_C_VA		         =	0x0D35,
	MALAYALAM_C_SHA		         =	0x0D36,
	MALAYALAM_C_SSA		         =	0x0D37,
	MALAYALAM_C_SA		         =	0x0D38,
	MALAYALAM_C_HA		         =	0x0D39,
	
	/* VARIOUS SIGNS */ 
	MALAYALAM_S_AVAGRAHA         =	0x0D3D,
	
	/* DEPENDENT VOWELS */
	MALAYALAM_DV_AA		         =	0x0D3E,
	MALAYALAM_DV_I		         =	0x0D3F,
	MALAYALAM_DV_II		         =	0x0D40,
	MALAYALAM_DV_U		         =	0x0D41,
	MALAYALAM_DV_UU		         =	0x0D42,
	MALAYALAM_DV_VR		         =	0x0D43,
	MALAYALAM_DV_VRR	         =	0x0D44,
	MALAYALAM_DV_E		         =	0x0D46,
	MALAYALAM_DV_EE		         =	0x0D47,
	MALAYALAM_DV_AI		         =	0x0D48,
	MALAYALAM_DV_O		         =	0x0D4A,
	MALAYALAM_DV_OO		         =	0x0D4B,
	MALAYALAM_DV_AU		         =	0x0D4C,
	
	/* VARIOUS SIGNS */ 
	MALAYALAM_S_HALANT		     =	0x0D4D,
	MALAYALAM_S_AU_LENGTH_MARK	 =	0x0D57,
	
	/* INDEPENDENT VOWELS */
	MALAYALAM_IV_VRR		     =	0x0D60,
	MALAYALAM_IV_VLL		     =	0x0D61,
	
	/* DEPENDENT VOWELS */
	MALAYALAM_DV_VL			     =	0x0D62,
	MALAYALAM_DV_VLL		     =	0x0D63,

	/* MALAYALAM SIGNS */
    MALAYALAM_S_DANDA 			 = 0x0D64,
    MALAYALAM_S_DOUBLE_DANDA 	 = 0x0D65,

	/*  DIGITS  */
	MALAYALAM_D_ZERO		     =	0x0D66,
	MALAYALAM_D_ONE		         =	0x0D67,
	MALAYALAM_D_TWO		         =	0x0D68,
	MALAYALAM_D_THREE		     =	0x0D69,
	MALAYALAM_D_FOUR		     =	0x0D6A,
	MALAYALAM_D_FIVE		     =	0x0D6B,
	MALAYALAM_D_SIX		         =	0x0D6C,
	MALAYALAM_D_SEVEN		     =	0x0D6D,
	MALAYALAM_D_EIGHT		     =	0x0D6E,
	MALAYALAM_D_NINE		     =	0x0D6F,
	MALAYALAM_D_TEN			     =	0x0D70,
	MALAYALAM_D_HUNDERED	     =	0x0D71,
	MALAYALAM_D_THOUSAND	     =	0x0D72,
	
	/* FRACTION */
	MALAYALAM_F_QUARTER		     =	0x0D73,
	MALAYALAM_F_HALF		     =	0x0D74,
	MALAYALAM_F_THREE_QUARTER    =	0x0D75,
	
	/* DATE MARK */
	MALAYALAM_S_DATE_MARK		 =  0x0D79,
	
	/* CHILLU LETTERS */
	MALAYALAM_C_CHILLU_NN		 =  0x0D7A,
	MALAYALAM_C_CHILLU_N		 =  0x0D7B,
	MALAYALAM_C_CHILLU_RR		 =  0x0D7C,
	MALAYALAM_C_CHILLU_L		 =  0x0D7D,
	MALAYALAM_C_CHILLU_LL		 =  0x0D7E,
	MALAYALAM_C_CHILLU_K		 =  0x0D7F,
	
	/* END OF ENUM */
	MALAYALAM_DUMMY_END
};
#endif


#ifdef IM_ORIYA_SUPPORT
enum
{   
	/* ORIYA SIGNS */
    ORIYA_DEVANAGARI_S_DANDA = 0x0964,
    ORIYA_DEVANAGARI_S_DOUBLE_DANDA = 0x0965,
	
	/* VARIOUS SIGNS + INDEPENDENT VOWEL */
    ORIYA_S_CHANDRA_BINDU = 0x0B01,
    ORIYA_S_ANUSVARA = 0x0B02,
    ORIYA_S_VISARGA = 0x0B03,

    ORIYA_IV_A = 0x0B05,
    ORIYA_IV_AA = 0x0B06,
    ORIYA_IV_I = 0x0B07,
    ORIYA_IV_II = 0x0B08,
    ORIYA_IV_U = 0x0B09,
    ORIYA_IV_UU = 0x0B0A,
    ORIYA_IV_R = 0x0B0B,
    ORIYA_IV_L = 0x0B0C,
    ORIYA_IV_E = 0x0B0F,
    ORIYA_IV_AI = 0x0B10,
    ORIYA_IV_O = 0x0B13,
    ORIYA_IV_AU = 0x0B14,

    /* CONSONANTS */
    ORIYA_C_KA = 0x0B15,
    ORIYA_C_KHA = 0x0B16,
    ORIYA_C_GA = 0x0B17,
    ORIYA_C_GHA = 0x0B18,
    ORIYA_C_NGA = 0x0B19,
    ORIYA_C_CHA = 0x0B1A,
    ORIYA_C_CHHA = 0x0B1B,
    ORIYA_C_JA = 0x0B1C,
    ORIYA_C_JHA = 0x0B1D,
    ORIYA_C_NYA = 0x0B1E,
    ORIYA_C_TTA = 0x0B1F,
    ORIYA_C_TTHA = 0x0B20,
    ORIYA_C_DDA = 0x0B21,
    ORIYA_C_DDHA = 0x0B22,
    ORIYA_C_NNA = 0x0B23,
    ORIYA_C_TA = 0x0B24,
    ORIYA_C_THA = 0x0B25,
    ORIYA_C_DA = 0x0B26,
    ORIYA_C_DHA = 0x0B27,
    ORIYA_C_NA = 0x0B28,

    ORIYA_C_PA = 0x0B2A,
    ORIYA_C_PHA = 0x0B2B,
    ORIYA_C_BA = 0x0B2C,
    ORIYA_C_BHA = 0x0B2D,
    ORIYA_C_MA = 0x0B2E,
    ORIYA_C_YA = 0x0B2F,
    ORIYA_C_RA = 0x0B30,

    ORIYA_C_LA = 0x0B32,
    ORIYA_C_LLA = 0x0B33,

    ORIYA_C_VA = 0x0B35,
    ORIYA_C_SHA = 0x0B36,
    ORIYA_C_SSA = 0x0B37,
    ORIYA_C_SA = 0x0B38,
    ORIYA_C_HA = 0x0B39,

    /* SIGNS */
    ORIYA_S_NUKTA = 0x0B3C,
    ORIYA_NS_AVAGRAHA = 0x0B3D,

    /* DEPENDENT VOWELS */
    ORIYA_DV_AA = 0x0B3E,
    ORIYA_DV_I = 0x0B3F,
    ORIYA_DV_II = 0x0B40,
    ORIYA_DV_U = 0x0B41,
    ORIYA_DV_UU = 0x0B42,
    ORIYA_DV_R = 0x0B43,
    ORIYA_DV_RR = 0x0B44,

    ORIYA_DV_E = 0x0B47,
    ORIYA_DV_AI = 0x0B48,

		ORIYA_SAN_AU = 0x0B4A, /* extra: to handle 0x0B4C becouse its pre,above and post base dependent vowle*/
    ORIYA_DV_O = 0x0B4B,
    ORIYA_DV_AU = 0x0B4C,

    /* SIGNS */
    ORIYA_S_HALANT = 0x0B4D,

    ORIYA_S_AI = 0x0B56,
    ORIYA_S_AU = 0x0B57,

    /* ADDITIONAL CONSONANT */
    ORIYA_AC_RRA = 0x0B5C,
    ORIYA_AC_RHA = 0x0B5D,
    ORIYA_AC_YYA = 0x0B5F,

    /* GENERIC ADDITIONS */
    ORIYA_NG_RR = 0x0B60,
    ORIYA_NG_LL = 0x0B61,
    ORIYA_NG_L = 0x0B62,
    ORIYA_NG_VLL = 0x0B63,
    ORIYA_NG_D = 0x0B64,
    ORIYA_NG_DD = 0x0B65,

    /* DIGITS */
    ORIYA_D_ZERO = 0x0B66,
    ORIYA_D_ONE = 0x0B67,
    ORIYA_D_TWO = 0x0B68,
    ORIYA_D_THREE = 0x0B69,
    ORIYA_D_FOUR = 0x0B6A,
    ORIYA_D_FIVE = 0x0B6B,
    ORIYA_D_SIX = 0x0B6C,
    ORIYA_D_SEVEN = 0x0B6D,
    ORIYA_D_EIGHT = 0x0B6E,
    ORIYA_D_NINE = 0x0B6F,
    /* ORIYA SPECIFIC ADDITOIN */
    ORIYA_S_ISSHAR = 0x0B70,
    ORIYA_S_WA = 0x0B71
};
#endif


#ifdef IM_PUNJABI_SUPPORT
enum
{
    /* DEVANAGARI SIGNS */
	PUNJABI_DEVANAGARI_S_DANDA			= 0x0964,
	PUNJABI_DEVANAGARI_S_DOUBLE_DANDA		= 0x0965,

	/* VARIOUS SIGNS + INDEPENDENT VOWEL */
    PUNJABI_S_ADAK_BINDI = 0x0A01,
    PUNJABI_S_BINDI = 0x0A02,
    PUNJABI_S_VISARGA = 0x0A03,
    PUNJABI_IV_A = 0x0A05,
    PUNJABI_IV_AA = 0x0A06,
    PUNJABI_IV_I = 0x0A07,
    PUNJABI_IV_II = 0x0A08,
    PUNJABI_IV_U = 0x0A09,
    PUNJABI_IV_UU = 0x0A0a,
    PUNJABI_IV_EE = 0x0A0f,
    PUNJABI_IV_AI = 0x0A10,
    PUNJABI_IV_OO = 0x0A13,
    PUNJABI_IV_AU = 0x0A14,

    /* CONSONANTS */
    PUNJABI_C_KA = 0x0A15,
    PUNJABI_C_KHA = 0x0A16,
    PUNJABI_C_GA = 0x0A17,
    PUNJABI_C_GHA = 0x0A18,
    PUNJABI_C_NGA = 0x0A19,
    PUNJABI_C_CA = 0x0A1a,
    PUNJABI_C_CHA = 0x0A1b,
    PUNJABI_C_JA = 0x0A1c,
    PUNJABI_C_JHA = 0x0A1d,
    PUNJABI_C_NYA = 0x0A1e,
    PUNJABI_C_TTA = 0x0A1f,
    PUNJABI_C_TTHA = 0x0A20,
    PUNJABI_C_DDA = 0x0A21,
    PUNJABI_C_DDHA = 0x0A22,
    PUNJABI_C_NNA = 0x0A23,
    PUNJABI_C_TA = 0x0A24,
    PUNJABI_C_THA = 0x0A25,
    PUNJABI_C_DA = 0x0A26,
    PUNJABI_C_DHA = 0x0A27,
    PUNJABI_C_NA = 0x0A28,
    PUNJABI_C_PA = 0x0A2a,
    PUNJABI_C_PHA = 0x0A2b,
    PUNJABI_C_BA = 0x0A2c,
    PUNJABI_C_BHA = 0x0A2d,
    PUNJABI_C_MA = 0x0A2e,
    PUNJABI_C_YA = 0x0A2f,
    PUNJABI_C_RA = 0x0A30,
    PUNJABI_C_LA = 0x0A32,
    PUNJABI_C_LLA = 0x0A33,
    PUNJABI_C_VA = 0x0A35,
    PUNJABI_C_SHA = 0x0A36,
    PUNJABI_C_SA = 0x0A38,
    PUNJABI_C_HA = 0x0A39,

    /* SIGNS */
    PUNJABI_S_NUKTA = 0x0A3c,

    /* DEPENDENT VOWELS */
    PUNJABI_DV_AA = 0x0A3e,
    PUNJABI_DV_I = 0x0A3f,
    PUNJABI_DV_II = 0x0A40,
    PUNJABI_DV_U = 0x0A41,
    PUNJABI_DV_UU = 0x0A42,
    PUNJABI_DV_EE = 0x0A47,
    PUNJABI_DV_AI = 0x0A48,
    PUNJABI_DV_OO = 0x0A4b,
    PUNJABI_DV_AU = 0x0A4c,

    /* SIGNS */
    PUNJABI_S_HALANT = 0x0A4d,
    PUNJABI_S_UDAAT = 0x0A51,

    /* ADDITIONAL CONSONANT */
    PUNJABI_AC_KHHA = 0x0A59,
    PUNJABI_AC_GHHA = 0x0A5a,
    PUNJABI_AC_ZA = 0x0A5b,
    PUNJABI_AC_RRA = 0x0A5c,
    PUNJABI_AC_FA = 0x0A5e,
    PUNJABI_S_DANDA = 0x0A64,
	PUNJABI_S_DOUBLE_DANDA = 0x0A65,
    
    /* DIGITS */
    PUNJABI_D_ZERO = 0x0A66,
    PUNJABI_D_ONE = 0x0A67,
    PUNJABI_D_TWO = 0x0A68,
    PUNJABI_D_THREE = 0x0A69,
    PUNJABI_D_FOUR = 0x0A6a,
    PUNJABI_D_FIVE = 0x0A6b,
    PUNJABI_D_SIX = 0x0A6c,
    PUNJABI_D_SEVEN = 0x0A6d,
    PUNJABI_D_EIGHT = 0x0A6e,
    PUNJABI_D_NINE = 0x0A6f,

    /* PUNJABI SPECIFIC ADDITOIN */
    PUNJABI_AC_TIPPI = 0x0A70,
    PUNJABI_AC_ADDAK = 0x0A71,
    PUNJABI_AC_IRI = 0x0A72,
    PUNJABI_AC_URA = 0x0A73,
    PUNJABI_AC_ONKAR = 0x0A74,
    
    /* SIGN */
    PUNJABI_S_YAKASH = 0x0A75,
    
    /* END OF ENUM */
	PUNJABI_DUMMY_END
};
#endif


#if defined (IM_ASSAMESE_SUPPORT)
typedef enum
{
    /* DEVANAGARI SIGNS */
    ASSAMESE_DEVANAGARI_S_DANDA		= 0x0964,
    ASSAMESE_DEVANAGARI_S_DDANDA		= 0x0965,
			
    /* VARIOUS SIGNS + INDEPENDENT VOWEL */
    ASSAMESE_S_CHANDRA_BINDU = 0x981,
    ASSAMESE_S_ANUSVARA = 0x982,
    ASSAMESE_S_VISARGA = 0x983,

    ASSAMESE_IV_A = 0x985,
    ASSAMESE_IV_AA = 0x986,
    ASSAMESE_IV_I = 0x987,
    ASSAMESE_IV_II = 0x988,
    ASSAMESE_IV_U = 0x989,
    ASSAMESE_IV_UU = 0x98a,
    ASSAMESE_IV_VR = 0x98b,
    ASSAMESE_IV_VL = 0x98c,
    ASSAMESE_IV_E = 0x98f,
    ASSAMESE_IV_AI = 0x990,

    ASSAMESE_IV_O = 0x993,
    ASSAMESE_IV_AU = 0x994,

    /* CONSONANTS */
    ASSAMESE_C_KA = 0x995,
    ASSAMESE_C_KHA = 0x996,
    ASSAMESE_C_GA = 0x997,
    ASSAMESE_C_GHA = 0x998,
    ASSAMESE_C_NGA = 0x999,
    ASSAMESE_C_CA = 0x99a,
    ASSAMESE_C_CHA = 0x99b,
    ASSAMESE_C_JA = 0x99c,
    ASSAMESE_C_JHA = 0x99d,
    ASSAMESE_C_NYA = 0x99e,
    ASSAMESE_C_TTA = 0x99f,
    ASSAMESE_C_TTHA = 0x9a0,
    ASSAMESE_C_DDA = 0x9a1,
    ASSAMESE_C_DDHA = 0x9a2,
    ASSAMESE_C_NNA = 0x9a3,
    ASSAMESE_C_TA = 0x9a4,
    ASSAMESE_C_THA = 0x9a5,
    ASSAMESE_C_DA = 0x9a6,
    ASSAMESE_C_DHA = 0x9a7,
    ASSAMESE_C_NA = 0x9a8,

    ASSAMESE_C_PA = 0x9aa,
    ASSAMESE_C_PHA = 0x9ab,
    ASSAMESE_C_BA = 0x9ac,
    ASSAMESE_C_BHA = 0x9ad,
    ASSAMESE_C_MA = 0x9ae,
    ASSAMESE_C_YA = 0x9af,
    ASSAMESE_C_RA = 0x9b0,

    ASSAMESE_C_LA = 0x9b2,

    ASSAMESE_C_SHA = 0x9b6,
    ASSAMESE_C_SSA = 0x9b7,
    ASSAMESE_C_SA = 0x9b8,
    ASSAMESE_C_HA = 0x9b9,
    ASSAMESE_C_INV = 0x9ba,

    /* VARIOUS SIGNS */
    ASSAMESE_S_NUKTA = 0x9bc,
    ASSAMESE_S_AVAGRAH = 0x9bd,

    /* DEPENDENT VOWELS */
    ASSAMESE_DV_AA = 0x9be,
    ASSAMESE_DV_I = 0x9bf,
    ASSAMESE_DV_II = 0x9c0,
    ASSAMESE_DV_U = 0x9c1,
    ASSAMESE_DV_UU = 0x9c2,
    ASSAMESE_DV_VR = 0x9c3,
    ASSAMESE_DV_VRR = 0x9c4,

    ASSAMESE_DV_E = 0x9c7,
    ASSAMESE_DV_AI = 0x9c8,

    ASSAMESE_DV_O = 0x9cb,
    ASSAMESE_DV_AU = 0x9cc,

    /* SIGNS */
    ASSAMESE_S_HALANT = 0x9cd,

    /* APPEND DEPENDENT VOWEL */
    ASSAMESE_AD_TA = 0x9ce,
    ASSAMESE_AD_AU = 0x9d7,

    /* CONSONANTS */
    ASSAMESE_C_RRA = 0x9dc,
    ASSAMESE_C_RHA = 0x9dd,
    ASSAMESE_C_YYA = 0x9df,

    ASSAMESE_C_VRR = 0x9e0,
    ASSAMESE_C_VLL = 0x9e1,

    ASSAMESE_S_VRR = 0x9e2,
    ASSAMESE_S_VLL = 0x9e3,
    /* PMT START MAUI_00027782 */
    ASSAMESE_S_DANDA = 0x9e4,
    ASSAMESE_S_DDANDA = 0x9e5,
    /* PMT END MAUI_00027782 */

    /* DIGITS */
    ASSAMESE_D_ZERO = 0x9e6,
    ASSAMESE_D_ONE = 0x9e7,
    ASSAMESE_D_TWO = 0x9e8,
    ASSAMESE_D_THREE = 0x9e9,
    ASSAMESE_D_FOUR = 0x9ea,
    ASSAMESE_D_FIVE = 0x9eb,
    ASSAMESE_D_SIX = 0x9ec,
    ASSAMESE_D_SEVEN = 0x9ed,
    ASSAMESE_D_EIGHT = 0x9ee,
    ASSAMESE_D_NINE = 0x9ef,

    ASSAMESE_C_MRA = 0x9f0,
    ASSAMESE_C_LRA = 0x9f1,
    ASSAMESE_OS_RUPEE = 0x9f2,
    ASSAMESE_C_RUPEE_SIGN = 0x9f3,
    ASSAMESE_C_CURONE = 0x9f4,
    ASSAMESE_C_CURTWO = 0x9f5,

    ASSAMESE_C_CURTHREE = 0x9f6,
    ASSAMESE_C_CURFOUR = 0x9f7,
    ASSAMESE_OS_CURLESSDEN = 0x9f8,
    ASSAMESE_OS_CURDENSIXTEEN = 0x9f9,

    ASSAMESE_OS_ISSHAR = 0x9fa
};
#endif


/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

/******************************************************************************/
// Description: 启动默认定时器
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_StartTimer(BOOLEAN *is_active, CAF_HANDLE_T ctrl_handle);

/******************************************************************************/
// Description: 启动默认定时器
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_StartTimerByTick(BOOLEAN *is_active, CAF_HANDLE_T ctrl_handle, uint32 ticks);

/******************************************************************************/
// Description: 关闭默认定时器
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_StopTimer(BOOLEAN *is_active);

/*****************************************************************************/
//  Description : 根据窗口的大小来设置popmenu的大小
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 注意物理键盘是从最上角往下弹出
/*****************************************************************************/
PUBLIC void MMIIM_SetPopMenuRect(MMI_HANDLE_T win_handle, GUI_BOTH_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description : 根据窗口的大小来设置popmenu的大小
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 注意虚拟键盘是从左下角往上弹出，目前暂定方案，以后应该弄成传入参数
/*****************************************************************************/
PUBLIC void MMIIM_SetTpPopMenuRect(MMI_HANDLE_T win_handle, GUI_BOTH_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description : MMIIM_GetDisplayDirection
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_DIRECTION_SET_E MMIIM_GetDisplayDirection(GUIIM_METHOD_T method);

#ifdef __cplusplus 
}
#endif


//general enum, hasn't relation with cstar or sogou or t9
enum
{
    MMIIM_DRAW_LEFT2RIGHT = 0x01,
    MMIIM_DRAW_RIGHT2LEFT = 0x02,
};
typedef uint8 MMIIM_DRAW_DIRECTION_T;

typedef struct
{
    /*显示可选的字和字符相关*/
    GUI_FONT_T font_default;
    GUI_FONT_T font_pre_edit;
    GUI_FONT_T font_candidate;
    GUI_FONT_T font_pre_choice;
    GUI_FONT_T font_lable;
    GUI_COLOR_T color_font_default;
    GUI_COLOR_T color_font_pre_edit;
    GUI_COLOR_T color_font_candidate;
    GUI_COLOR_T color_font_pre_choice;
    GUI_COLOR_T color_font_lable;
    GUI_COLOR_T color_bg_default;
    GUI_COLOR_T color_bg_pre_edit;
    GUI_COLOR_T color_bg_candidate;
    GUI_COLOR_T color_bg_pre_choice;
    GUI_COLOR_T color_bg_lable;
    GUI_COLOR_T color_hot_default;
    GUI_COLOR_T color_hot_pre_edit;
    GUI_COLOR_T color_hot_candidate;
    GUI_COLOR_T color_hot_pre_choice;
    GUI_COLOR_T color_hot_lable;
    
    GUI_COLOR_T color_border;
}MMIIM_UI_CONFIG_T;

extern MMIIM_UI_CONFIG_T const g_mmiim_ui_config;


#if defined(MMIIM_CORE_USE_CSTAR)
enum
{
    //indicator size
    MMIIM_IDC_WIDTH = 4,
    MMIIM_IDC_HEIGHT = 2*MMIIM_IDC_WIDTH -1,
    MMIIM_IDC_MARGIN = 4,
    MMIIM_IDC_FULL_WIDTH = MMIIM_IDC_WIDTH + 2*MMIIM_IDC_MARGIN,
    MMIIM_PRE_BUFFER_LEN = 64,
    MMIIM_CAND_WORDS_SPACE = 1,
    MMIIM_EDIT_WIN_SPACE = 2,

    MMIIM_ASSO_WORD_MAX_LEN = 32,
    MMIIM_WIN_BUFFER_LEN = 2*MMIIM_ASSO_WORD_MAX_LEN,
    //cand
    MMIIM_CAND_COUNT_MAX = 10,
};

enum
{
    MMIIM_IDC_FLAG_NONE = 0,
    MMIIM_IDC_FLAG_LEFT = 0x01,
    MMIIM_IDC_FLAG_RIGHT = 0x02,
};
typedef uint8 MMIIM_IDC_FLAG_T;

typedef struct
{
    uint16 guiim_method;
    uint32 cstar_method;
}MMIIM_CSTAR_METHOD_RELATION_T;


enum
{
    MMIIM_WIN_STATE_NONE,
    MMIIM_WIN_STATE_EDIT,
    MMIIM_WIN_STATE_CAND,
    MMIIM_WIN_STATE_USER,
};
typedef uint8 MMIIM_WIN_STATE_T;

typedef struct
{
    GUI_RECT_T rect_cand;
    GUI_RECT_T rect_idc_left;
    GUI_RECT_T rect_idc_right;
    uint8 lable_width;

    MMIIM_WIN_STATE_T state;

	MMIIM_DRAW_DIRECTION_T draw_direction; //显示方向，取决定于语言，MMIIM_DRAW_LEFT2RIGHT, MMIIM_IDC_FLAG_RIGHT
    
    wchar buffer[MMIIM_WIN_BUFFER_LEN];
    int32 count;
	
    MMIIM_IDC_FLAG_T idc_flag; // 0 - none, 1 - right, 2 - left, 3 - all

    BOOLEAN has_lable; //显示数字标签
    
    BOOLEAN has_hot; //显示选定块
    uint8 hot_index;

    BOOLEAN is_self_idc; //不能显示所提供的所有字符，记录最后一个显示的字符索引
    uint8 disp_first_index;
    uint8 disp_per_page;

    BOOLEAN is_not_asso;
}MMIIM_CAND_WIN_T;

typedef struct
{
    MMIIM_WIN_STATE_T state;

    GUI_RECT_T rect;

	MMIIM_DRAW_DIRECTION_T draw_direction; //显示方向，取决定于语言，MMIIM_DRAW_LEFT2RIGHT, MMIIM_IDC_FLAG_RIGHT
    
    wchar buffer[MMIIM_WIN_BUFFER_LEN];
    uint8 count;

    BOOLEAN has_cursor; //显示光标
    uint8 cursor_index;
}MMIIM_PRE_EDIT_WIN_T;

typedef struct
{
    MMIIM_WIN_STATE_T state;
    
    GUI_RECT_T rect;

    MMIIM_DRAW_DIRECTION_T draw_direction;
    
    wchar buffer[MMIIM_WIN_BUFFER_LEN];
    
    uint8 choice_count;
    
    BOOLEAN has_cursor; //显示光标
    uint8 cursor_index;
}MMIIM_CHOICE_WIN_T;


/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

/******************************************************************************/
// Description: 初始化候选字窗口中的变量
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetCandWin(MMIIM_CAND_WIN_T *cand_win_ptr);

/******************************************************************************/
// Description: 初始化预编译窗口中的变量
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetPreEditWin(MMIIM_PRE_EDIT_WIN_T *edit_win_ptr);


/******************************************************************************/
// Description: 绘制候选字窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawCandWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *cand_win_ptr
                              );

/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawPreEditWin(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 MMIIM_PRE_EDIT_WIN_T *edit_win_ptr
                                 );

/******************************************************************************/
// Description: 绘制用户自造词窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawUserWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *cand_win_ptr
                              );
                              
/******************************************************************************/
// Description: 点击选择窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 如果选中，则同时处理返回值
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_TpDownCand(
    MMIIM_CAND_WIN_T *cand_win_ptr,
    GUI_POINT_T point,
    MMI_HANDLE_T im_handle
);

/******************************************************************************/
// Description: 根据上下文设置候选窗口的候选字
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 如果选中，则同时处理返回值
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_AssoCand(
                              MMIIM_CAND_WIN_T *cand_win_ptr,
                              wchar const * context_ptr,
                              uint16 context_len,
                              uint16 context_pos
                              );

/******************************************************************************/
// Description: 点击选择窗口，当按键处理，不处理结果
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_TpDownCandAsKey(
                                     MMIIM_CAND_WIN_T *cand_win_ptr,
                                     GUI_POINT_T point,
                                     uint16 *index_ptr
                                     );

/******************************************************************************/
// Description: 获取联想汉字
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC int32 MMIIM_QueryAsso(
                             wchar const * context_ptr,
                             uint16 context_len,
                             uint16 context_pos,
                             wchar *asso_ptr,
                             int32 *asso_len_ptr
                             );

/******************************************************************************/
// Description: 初始化候选字框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitCandWin(MMIIM_CAND_WIN_T *cand_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height);

/******************************************************************************/
// Description: 初始化预编辑框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitPreEditWin(MMIIM_PRE_EDIT_WIN_T *edit_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height);

/******************************************************************************/
// Description: 单字库用到的CHOICE窗口
// Global resource dependence: NONE
// Author: juan.wu
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawChoiceWin(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 MMIIM_CHOICE_WIN_T *choice_win_ptr
                                 );

/******************************************************************************/
// Description: 初始化混淆区域
// Global resource dependence: NONE
// Author: juan.wu
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitChoiceWin(MMIIM_CHOICE_WIN_T *choice_win_ptr, GUI_RECT_T *win_rect_ptr,uint16 height);

/******************************************************************************/
// Description: 初始化choice窗口中的变量
// Global resource dependence: NONE
// Author: juan.wu
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetChoiceWin(MMIIM_CHOICE_WIN_T *choice_win_ptr);
/******************************************************************************/
// Description: 绘制空格字符
// Global resource dependence: NONE
// Author: yaoguang.chen
// Note: 
/******************************************************************************/
PUBLIC int32 MMIIM_DrawSpace(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *prect_draw, GUI_COLOR_T color);

#ifdef __cplusplus 
}
#endif

#elif defined IM_ENGINE_SOGOU//MMIIM_CORE_USE_SOGOU
enum
{
    //indicator size
    MMIIM_IDC_WIDTH = 4,
    MMIIM_IDC_HEIGHT = 2*MMIIM_IDC_WIDTH -1,
    MMIIM_IDC_MARGIN = 4,
    MMIIM_IDC_FULL_WIDTH = MMIIM_IDC_WIDTH + 2*MMIIM_IDC_MARGIN,
    MMIIM_PRE_BUFFER_LEN = 64,
    MMIIM_CAND_WORDS_SPACE = 5,
    MMIIM_EDIT_WIN_SPACE = 2,

    MMIIM_ASSO_WORD_MAX_LEN = 32,
    MMIIM_WIN_BUFFER_LEN = 2*MMIIM_ASSO_WORD_MAX_LEN,
    //cand
    MMIIM_CAND_COUNT_MAX = 10,
};

enum
{
    MMIIM_IDC_FLAG_NONE = 0,
    MMIIM_IDC_FLAG_LEFT = 0x01,
    MMIIM_IDC_FLAG_RIGHT = 0x02,
};
typedef uint8 MMIIM_IDC_FLAG_T;

typedef struct
{
    uint16 guiim_method;
    uint32 sogou_method;//低16位代表搜狗输入法的输入法类型，高16为代表有大小写的语言的大小写状态（1-全小写，2-首字母大写，3-全大写）
} MMIIM_SOGOU_METHOD_RELATION_T;


enum
{
    MMIIM_WIN_STATE_NONE,
    MMIIM_WIN_STATE_EDIT,
    MMIIM_WIN_STATE_CAND,
    MMIIM_WIN_STATE_USER,
};
typedef uint8 MMIIM_WIN_STATE_T;


typedef struct
{
    GUI_RECT_T rect_cand;
    GUI_RECT_T rect_idc_left;
    GUI_RECT_T rect_idc_right;
    uint8 lable_width;

    MMIIM_WIN_STATE_T state;

	MMIIM_DRAW_DIRECTION_T draw_direction; //显示方向，取决定于语言，MMIIM_DRAW_LEFT2RIGHT, MMIIM_IDC_FLAG_RIGHT
    
    wchar buffer[MMIIM_WIN_BUFFER_LEN];
    int32 count;
	
    MMIIM_IDC_FLAG_T idc_flag; // 0 - none, 1 - right, 2 - left, 3 - all

    BOOLEAN has_lable; //显示数字标签
    
    BOOLEAN has_hot; //显示选定块
    uint8 hot_index;

    BOOLEAN is_self_idc; //不能显示所提供的所有字符，记录最后一个显示的字符索引
    uint8 disp_first_index;
    uint8 disp_per_page;

    BOOLEAN is_not_asso;
}MMIIM_CAND_WIN_T;

typedef struct
{
    MMIIM_WIN_STATE_T state;

    GUI_RECT_T rect;

	MMIIM_DRAW_DIRECTION_T draw_direction; //显示方向，取决定于语言，MMIIM_DRAW_LEFT2RIGHT, MMIIM_IDC_FLAG_RIGHT
    
    wchar buffer[MMIIM_WIN_BUFFER_LEN];
    uint8 count;

    BOOLEAN has_cursor; //显示光标
    uint8 cursor_index;
}MMIIM_PRE_EDIT_WIN_T;

typedef struct
{
    MMIIM_WIN_STATE_T state;
    
    GUI_RECT_T rect;
	
    MMIIM_DRAW_DIRECTION_T draw_direction;
    
    wchar buffer[MMIIM_WIN_BUFFER_LEN];
    
    uint8 choice_count;
    
    BOOLEAN has_cursor; //显示光标
    uint8 cursor_index;
}MMIIM_CHOICE_WIN_T;


/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

/******************************************************************************/
// Description: 初始化候选字窗口中的变量
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetCandWin(MMIIM_CAND_WIN_T *cand_win_ptr);

/******************************************************************************/
// Description: 初始化预编译窗口中的变量
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetPreEditWin(MMIIM_PRE_EDIT_WIN_T *edit_win_ptr);

/******************************************************************************/
// Description: 绘制候选字窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawCandWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *cand_win_ptr,
                              uint8 *lastdisp,
							  MMI_WIN_ID_T win_id,
                              void *imeResult
                              );

/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawPreEditWin(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 MMIIM_PRE_EDIT_WIN_T *edit_win_ptr,
								 MMI_WIN_ID_T   win_id
								 );

/******************************************************************************/
// Description: 绘制用户自造词窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawUserWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *cand_win_ptr
                              );
                              
/******************************************************************************/
// Description: 点击选择窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 如果选中，则同时处理返回值
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_TpDownCand(
    MMIIM_CAND_WIN_T *cand_win_ptr,
    GUI_POINT_T point,
    MMI_HANDLE_T im_handle
);

/******************************************************************************/
// Description: 根据上下文设置候选窗口的候选字
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 如果选中，则同时处理返回值
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_AssoCand(
                              MMIIM_CAND_WIN_T *cand_win_ptr,
                              wchar const * context_ptr,
                              uint16 context_len,
                              uint16 context_pos
                              );

/******************************************************************************/
// Description: 点击选择窗口，当按键处理，不处理结果
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_TpDownCandAsKey(
                                     MMIIM_CAND_WIN_T *cand_win_ptr,
                                     GUI_POINT_T point,
                                     uint16 *index_ptr,
                                     uint8 lastdisplay
                                     );

/******************************************************************************/
// Description: 获取联想汉字
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC int32 MMIIM_QueryAsso(
                             wchar const * context_ptr,
                             uint16 context_len,
                             uint16 context_pos,
                             wchar *asso_ptr,
                             int32 *asso_len_ptr
                             );

/******************************************************************************/
// Description: 初始化候选字框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitCandWin(MMIIM_CAND_WIN_T *cand_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height);

/******************************************************************************/
// Description: 初始化预编辑框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitPreEditWin(MMIIM_PRE_EDIT_WIN_T *edit_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height);

/******************************************************************************/
// Description: 初始化备选音的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitLetterWin(MMIIM_CAND_WIN_T *edit_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height);

/******************************************************************************/
// Description: 绘制备选音区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_DrawLetterWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *edit_win_ptr,
                              uint8 *lastdisp,
                              void *imeResult_in,
							  MMI_WIN_ID_T   win_id
							  );

/******************************************************************************/
// Description: 绘制空格字符
// Global resource dependence: NONE
// Author: yaoguang.chen
// Note: 
/******************************************************************************/
PUBLIC int32 MMIIM_DrawSpace(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *prect_draw, GUI_COLOR_T color);
#ifdef __cplusplus 
}
#endif

#elif defined MMIIM_CORE_USE_T9

enum
{
    //indicator size
    MMIIM_IDC_WIDTH = 4,
    MMIIM_IDC_HEIGHT = 4,
    MMIIM_IDC_MARGIN = 4,
    MMIIM_IDC_FULL_WIDTH = MMIIM_IDC_WIDTH + 2*MMIIM_IDC_MARGIN,
    MMIIM_IDC_COLOR_INACTIVE = 0x041f,
    MMIIM_IDC_COLOR_ACTIVE = 0xef9d,
    MMIIM_IDC_COLOR_PRESSED = 0x34df,
    
    
    MMIIM_PRE_BUFFER_LEN = 64,
    MMIIM_CAND_WORDS_SPACE = 1,
    MMIIM_EDIT_WIN_SPACE = 2,

    MMIIM_ASSO_WORD_MAX_LEN = 32,
    MMIIM_WIN_BUFFER_LEN = 2*MMIIM_ASSO_WORD_MAX_LEN,
    //cand
    MMIIM_CAND_COUNT_MAX = 10,
    MMIIM_COMMIT_LEN_MAX = 36,
};

enum
{
    MMIIM_IDC_FLAG_NONE = 0,
    MMIIM_IDC_FLAG_LEFT = 0x01,
    MMIIM_IDC_FLAG_RIGHT = 0x02,
};
typedef uint8 MMIIM_IDC_FLAG_T;

enum
{
    MMIIM_WIN_STATE_NONE,
    MMIIM_WIN_STATE_EDIT,
    MMIIM_WIN_STATE_CAND,
    MMIIM_WIN_STATE_USER,
};
typedef uint8 MMIIM_WIN_STATE_T;


typedef struct
{
    wchar * word_ptr;                           //注意不以'/0'结束，只是一个指示
    uint16 len;                                 //表示当前段的字符个数
    uint16 width;                               //表示当前段的字符长度
    uint16 page_index;                          //本单词在第几页
} MMIIM_TEXT_BAR_SEG_INFO_T;

enum
{
    MMIIM_IDC_NONE,
    MMIIM_IDC_INACTIVE,
    MMIIM_IDC_ACTIVE,
    MMIIM_IDC_PRESSED,

    MMIIM_IDC_MAX,
};
typedef uint8 MMIIM_IDC_STATE_T;


#endif //MMIIM_CORE_USE_CSTAR


#endif //_MMIIM_BASE_H_




