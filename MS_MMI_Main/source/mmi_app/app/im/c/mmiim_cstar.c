/*******************************************************************************
** File Name: mmiim_cstar.c
** Author: Lianxiang.Zhou
** Date: 2009/08/08
** Copyright: 2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description:处理cstar键盘输入的应用
********************************************************************************
** Edit History
**------------------------------------------------------------------------------
** DATE         NAME            DESCRIPTION
** 2009/08/08   Lianxiang.Zhou  Create.
*******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmi_app_im_trc.h"
#include "guiim_base.h"

#include "mmiim.h"
#include "mmiim_cstar.h"
#include "mmitheme_pos.h"
#include "mmiim.h"
#include "mmiim_internal.h"

#if defined(MMIIM_USE_CSTAR)
#include "csk2api.h"
#include "kmx_all_ldb.h"
const CS_IME_INFO_PTR cs_input_method_config[] =
{

#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    &cs_ldb_english_107,
#else
    &cs_ldb_english_103,
#endif

#if defined(IM_SIMP_CHINESE_SUPPORT) //简体中文
#if defined(MMI_IM_PDA_SUPPORT)
#if defined(INPUT_METHOD_CSTAR_PLAIN)
    &cs_ldb_py_gb2312_c2_20k,
    &cs_ldb_stroke_chs,
#else
    &cs_ldb_py_gb2312_c2_10k,
    &cs_ldb_stroke_gb2312_mini,
#endif
#else
#if defined(INPUT_METHOD_CSTAR_PLAIN)
    &cs_ldb_py_gb2312_c2_20k,
    &cs_ldb_stroke_gb2312_plain,
#elif defined(INPUT_METHOD_CSTAR_GBK)
    &cs_ldb_py_gbk_c2_20k,
    &cs_ldb_stroke_gbk_plain,
#elif defined(INPUT_METHOD_CSTAR_MINI)
    &cs_ldb_dzpy_gb2312_20k,
    &cs_ldb_stroke_gb2312_mini,
#elif defined(INPUT_METHOD_CSTAR_MINI_GBK)
    &cs_ldb_dzpy_gbk_20k,
    &cs_ldb_stroke_gb2312_mini,
#elif defined(INPUT_METHOD_CSTAR_TINY)
    &cs_ldb_dzpy_gb2312_0k,
    &cs_ldb_stroke_gb2312_mini,
#endif
#endif  //endif MMI_PDA_SUPPORT
#endif  //endif IM_SIMP_CHINESE_SUPPORT

#if defined(IM_TRAD_CHINESE_SUPPORT) //繁体中文
#if defined(MMI_IM_PDA_SUPPORT)
    &cs_ldb_zy_big5_20k,
    &cs_ldb_stroke_big5_plain,
#else
#if defined(INPUT_METHOD_CSTAR_PLAIN)
    &cs_ldb_zy_big5_20k,
    &cs_ldb_stroke_big5_plain,
#elif defined(INPUT_METHOD_CSTAR_GBK)
    &cs_ldb_zy_big5_20k,
    &cs_ldb_stroke_big5_plain,
#elif defined(INPUT_METHOD_CSTAR_MINI)
    &cs_ldb_dzzy_big5normal_20k,
    &cs_ldb_stroke_big5_normal_mini,
#elif defined(INPUT_METHOD_CSTAR_MINI_GBK)
    &cs_ldb_dzzy_big5normal_20k,
    &cs_ldb_stroke_big5_normal_mini,
#elif defined(INPUT_METHOD_CSTAR_TINY)
    &cs_ldb_dzzy_big5normal_0k,
    &cs_ldb_stroke_big5_normal_mini,
#endif
#endif  //endif MMI_PDA_SUPPORT
#endif  //endif IM_TRAD_CHINESE_SUPPORT

#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
#if defined(IM_ARABIC_SUPPORT)//阿拉伯语
#if defined(SPECIAL_ARABIC_PERSIAN)
    &cs_ldb_arabic_809,
#else
    &cs_ldb_arabic_807,
#endif
#endif

#if defined(IM_HINDI_SUPPORT)//印地语
#if defined(SPECIAL_HINDI)
    &cs_ldb_hindi_908,
#else
    &cs_ldb_hindi_907,
#endif

#endif

#if defined(IM_THAI_SUPPORT)//泰语
#if defined(SPECIAL_THAI)
    &cs_ldb_thai_368,
#else
    &cs_ldb_thai_367,
#endif
#endif

#if defined(IM_RUSSIAN_SUPPORT)//俄语
    &cs_ldb_russian_207,
#endif

#if defined(IM_GERMAN_SUPPORT)//德语
    &cs_ldb_german_307,
#endif

#if defined(IM_VIETNAMESE_SUPPORT)//越南语
    &cs_ldb_vietnamese_317,
#endif

#if defined(IM_INDONESIAN_SUPPORT)//印度尼西亚语
    &cs_ldb_indonesian_327,
#endif

#if defined(IM_TURKISH_SUPPORT)//土耳其语
    &cs_ldb_turkish_337,
#endif

#if defined(IM_MALAY_SUPPORT)//马来语
    &cs_ldb_malay_347,
#endif

#if defined(IM_GREEK_SUPPORT)//希腊语
    &cs_ldb_greek_357,
#endif

#if defined(IM_DANISH_SUPPORT)//丹麦语
    &cs_ldb_danish_377,
#endif

#if defined(IM_DUTCH_SUPPORT)//荷兰语
    &cs_ldb_dutch_387,
#endif

#if defined(IM_SWAHILI_SUPPORT)//斯瓦西里语
    &cs_ldb_swahili_557,
#endif

#if defined(IM_POLISH_SUPPORT)//波兰语
    &cs_ldb_polish_397,
#endif

#if defined(IM_FRENCH_SUPPORT)//法语
    &cs_ldb_french_407,
#endif

#if defined(IM_URDU_SUPPORT)//乌尔都语
    &cs_ldb_urdu_957,
#endif

#if defined(IM_HUNGARIAN_SUPPORT)//匈牙利语
    &cs_ldb_hungarian_417,
#endif

#if defined(IM_HEBREW_SUPPORT)//希伯来语
    &cs_ldb_hebrew_427,
#endif

#if defined(IM_BENGALI_SUPPORT)//孟加拉语
    &cs_ldb_bengali_924,
#endif

#if defined(IM_BULGARIAN_SUPPORT)//保加利亚语
    &cs_ldb_bulgarian_437,
#endif

#if defined(IM_CZECH_SUPPORT)//捷克语
    &cs_ldb_czech_447,
#endif

#if defined(IM_FINNISH_SUPPORT)//芬兰语
    &cs_ldb_finnish_457,
#endif

#if defined(IM_PERSIAN_SUPPORT)//波斯语
#if defined(SPECIAL_ARABIC_PERSIAN) 
    &cs_ldb_persian_469,
#else
    &cs_ldb_persian_467,
#endif
#endif

#if defined(IM_SLOVAK_SUPPORT)//斯洛伐克语
    &cs_ldb_slovak_477,
#endif

#if defined(IM_NORWEGIAN_SUPPORT)//挪威语
    &cs_ldb_norwegian_487,
#endif

#if defined(IM_SWEDISH_SUPPORT)//瑞典语
    &cs_ldb_swedish_497,
#endif

#if defined(IM_SPANISH_SUPPORT)//西班牙语
    &cs_ldb_spanish_507,
#endif

#if defined(IM_ROMANIAN_SUPPORT)//罗马尼亚语
    &cs_ldb_romanian_527,
#endif

#if defined(IM_SLOVENIAN_SUPPORT)//斯诺文尼亚语
    &cs_ldb_slovenian_537,
#endif

#if defined(IM_PORTUGUESE_SUPPORT)//葡萄牙语
    &cs_ldb_portuguese_607,
#endif

#if defined(IM_ITALIAN_SUPPORT)//意大利语
    &cs_ldb_italian_707,
#endif

#if defined(IM_TAGALOG_SUPPORT)//他加诺语
    &cs_ldb_tagalog_587,
#endif

#if defined(IM_HINGLISH_SUPPORT)//印度英语
    &cs_ldb_hinglish_917,
#endif

#if defined(IM_LITHUANIAN_SUPPORT)//立陶宛语
    &cs_ldb_lithuanian_567,
#endif

#if defined(IM_UKRAINIAN_SUPPORT)//乌克兰语
    &cs_ldb_ukrainian_547,
#endif
#if defined(IM_TELUGU_SUPPORT)//泰卢固语
    &cs_ldb_telugu,
#endif

#if defined(IM_MARATHI_SUPPORT)//马拉地语
    &cs_ldb_marathi,
#endif

#if defined(IM_TAMIL_SUPPORT)//泰米尔语
    &cs_ldb_tamil,
#endif

#if defined(IM_PUNJABI_SUPPORT)//旁遮普语
    &cs_ldb_punjabi,
#endif

#if defined(IM_GUJARATI_SUPPORT)//吉吉拉特语
    &cs_ldb_gujarati,
#endif

#if defined(IM_ORIYA_SUPPORT)//奥里亚语
    &cs_ldb_oriya,
#endif

#if defined(IM_KANNADA_SUPPORT)//埃纳德语
    &cs_ldb_kannada,
#endif

#if defined(IM_MALAYALAM_SUPPORT)//马来亚兰语
    &cs_ldb_malayalam,
#endif

#if defined(IM_MYANMAR_SUPPORT)//缅甸语
    &cs_ldb_myanmar,
#endif

#if defined(IM_KHMER_SUPPORT)//高棉语
    &cs_ldb_khmer,
#endif

#if defined(IM_LAO_SUPPORT)//老挝语
    &cs_ldb_lao,
#endif

#if defined(IM_AMHARIC_SUPPORT)//阿姆哈拉语
    &cs_ldb_amharic,
#endif

#if defined(IM_ARMENIAN_SUPPORT)//亚美尼亚语
    &cs_ldb_armenian,
#endif

#if defined(IM_UYGHUR_SUPPORT)//维吾尔语
    &cs_ldb_uyghur,
#endif

#if defined(IM_TIBETAN_SUPPORT)   // 2012 0619 tibetan 
	//&cs_ldb_tibetan,
#endif
#else   /* INPUT_METHOD_CSTAR_COMMON_MINI */
#if defined(IM_ARABIC_SUPPORT)//阿拉伯语
#if defined(SPECIAL_ARABIC_PERSIAN)
    &cs_ldb_arabic_803,
#else
    &cs_ldb_arabic_800,
#endif
#endif

#if defined(IM_HINDI_SUPPORT)//印地语
#if defined(SPECIAL_HINDI)
    &cs_ldb_hindi_906,
#else
    &cs_ldb_hindi_900,
#endif
#endif

#if defined(IM_THAI_SUPPORT)//泰语
#if defined(SPECIAL_THAI)
    &cs_ldb_thai_364,
#else
    &cs_ldb_thai_360,
#endif
#endif

#if defined(IM_RUSSIAN_SUPPORT)//俄语
    &cs_ldb_russian_202,
#endif

#if defined(IM_GERMAN_SUPPORT)//德语
    &cs_ldb_german_300,
#endif

#if defined(IM_VIETNAMESE_SUPPORT)//越南语
    &cs_ldb_vietnamese_310,
#endif

#if defined(IM_INDONESIAN_SUPPORT)//印度尼西亚语
    &cs_ldb_indonesian_320,
#endif

#if defined(IM_TURKISH_SUPPORT)//土耳其语
    &cs_ldb_turkish_330,
#endif

#if defined(IM_MALAY_SUPPORT)//马来语
    &cs_ldb_malay_340,
#endif

#if defined(IM_GREEK_SUPPORT)//希腊语
    &cs_ldb_greek_350,
#endif

#if defined(IM_DANISH_SUPPORT)//丹麦语
    &cs_ldb_danish_370,
#endif

#if defined(IM_DUTCH_SUPPORT)//荷兰语
    &cs_ldb_dutch_380,
#endif

#if defined(IM_SWAHILI_SUPPORT)//斯瓦西里语
    &cs_ldb_swahili_550,
#endif

#if defined(IM_POLISH_SUPPORT)//波兰语
    &cs_ldb_polish_391,
#endif

#if defined(IM_FRENCH_SUPPORT)//法语
    &cs_ldb_french_400,
#endif

#if defined(IM_URDU_SUPPORT)//乌尔都语
    &cs_ldb_urdu_950,
#endif

#if defined(IM_HUNGARIAN_SUPPORT)//匈牙利语
    &cs_ldb_hungarian_410,
#endif

#if defined(IM_HEBREW_SUPPORT)//希伯来语
    &cs_ldb_hebrew_420,
#endif

#if defined(IM_BENGALI_SUPPORT)//孟加拉语
    &cs_ldb_bengali_924,
#endif

#if defined(IM_BULGARIAN_SUPPORT)//保加利亚语
    &cs_ldb_bulgarian_430,
#endif

#if defined(IM_CZECH_SUPPORT)//捷克语
    &cs_ldb_czech_440,
#endif

#if defined(IM_FINNISH_SUPPORT)//芬兰语
    &cs_ldb_finnish_450,
#endif

#if defined(IM_PERSIAN_SUPPORT)//波斯语
#if defined(SPECIAL_ARABIC_PERSIAN) 
    &cs_ldb_persian_463,
#else
    &cs_ldb_persian_460,
#endif
#endif

#if defined(IM_SLOVAK_SUPPORT)//斯洛伐克语
    &cs_ldb_slovak_470,
#endif

#if defined(IM_NORWEGIAN_SUPPORT)//挪威语
    &cs_ldb_norwegian_480,
#endif

#if defined(IM_SWEDISH_SUPPORT)//瑞典语
    &cs_ldb_swedish_490,
#endif

#if defined(IM_SPANISH_SUPPORT)//西班牙语
    &cs_ldb_spanish_500,
#endif

#if defined(IM_ROMANIAN_SUPPORT)//罗马尼亚语
    &cs_ldb_romanian_520,
#endif

#if defined(IM_SLOVENIAN_SUPPORT)//斯诺文尼亚语
    &cs_ldb_slovenian_530,
#endif

#if defined(IM_PORTUGUESE_SUPPORT)//葡萄牙语
    &cs_ldb_portuguese_600,
#endif

#if defined(IM_ITALIAN_SUPPORT)//意大利语
    &cs_ldb_italian_700,
#endif

#if defined(IM_TAGALOG_SUPPORT)//他加诺语
    &cs_ldb_tagalog_587,
#endif

#if defined(IM_TELUGU_SUPPORT)//泰卢固语
    &cs_ldb_telugu,
#endif

#if defined(IM_HINGLISH_SUPPORT)//印度英语
    &cs_ldb_hinglish_910,
#endif

#if defined(IM_LITHUANIAN_SUPPORT)//立陶宛语
    &cs_ldb_lithuanian_560,
#endif

#if defined(IM_UKRAINIAN_SUPPORT)//乌克兰语
    &cs_ldb_ukrainian_540,
#endif

#if defined(IM_MARATHI_SUPPORT)//马拉地语
    &cs_ldb_marathi,
#endif

#if defined(IM_TAMIL_SUPPORT)//泰米尔语
    &cs_ldb_tamil,
#endif

#if defined(IM_PUNJABI_SUPPORT)//旁遮普语
    &cs_ldb_punjabi,
#endif

#if defined(IM_GUJARATI_SUPPORT)//吉吉拉特语
    &cs_ldb_gujarati,
#endif

#if defined(IM_ORIYA_SUPPORT)//奥里亚语
    &cs_ldb_oriya,
#endif

#if defined(IM_KANNADA_SUPPORT)//埃纳德语
    &cs_ldb_kannada,
#endif

#if defined(IM_MALAYALAM_SUPPORT)//马来亚兰语
    &cs_ldb_malayalam,
#endif

#if defined(IM_MYANMAR_SUPPORT)//缅甸语
    &cs_ldb_myanmar,
#endif

#if defined(IM_KHMER_SUPPORT)//高棉语
    &cs_ldb_khmer,
#endif

#if defined(IM_LAO_SUPPORT)//老挝语
    &cs_ldb_lao,
#endif

#if defined(IM_AMHARIC_SUPPORT)//阿姆哈拉语
    &cs_ldb_amharic,
#endif

#if defined(IM_ARMENIAN_SUPPORT)//亚美尼亚语
    &cs_ldb_armenian,
#endif

#if defined(IM_UYGHUR_SUPPORT)//维吾尔语
    &cs_ldb_uyghur,
#endif
#if defined(IM_TIBETAN_SUPPORT)   // 2012 0619 tibetan 
	//&cs_ldb_tibetan,
#endif
#endif
    0   /* Required: marks the end of table */
};

// Add QWERTY keyboard layout
/*lint -save -e651 */
const CS_QWERTY_INFO cs_qwerty_info[] = 
{
    #ifdef IM_THAI_SUPPORT
    {CS_LANG_THAI,   DefaultThaiMapping},
    #endif
    #ifdef IM_ARMENIAN_SUPPORT 
    {CS_LANG_ARMENIAN,   DefaultArmenianMapping},
    #endif
    #ifdef IM_BULGARIAN_SUPPORT 
    {CS_LANG_BULGARIAN,   DefaultBulgarianMapping},
    #endif
    #ifdef IM_CZECH_SUPPORT 
    {CS_LANG_CZECH,   DefaultCzechMapping},
    #endif
    #ifdef IM_DANISH_SUPPORT 
    {CS_LANG_DANISH,   DefaultDanishMapping},
    #endif
    #ifdef IM_DUTCH_SUPPORT 
    {CS_LANG_DUTCH,   DefaultDutchMapping},
    #endif
    #ifdef IM_FINNISH_SUPPORT 
    {CS_LANG_FINNISH,   DefaultFinnishMapping},
    #endif
    #ifdef IM_FRENCH_SUPPORT 
    {CS_LANG_FRENCH,   DefaultFrenchMapping},
    #endif
    #ifdef IM_GERMAN_SUPPORT 
    {CS_LANG_GERMAN,   DefaultGermanMapping},
    #endif
    #ifdef IM_GREEK_SUPPORT 
    {CS_LANG_GREEK,   DefaultGreekMapping},
    #endif
    #ifdef IM_HUNGARIAN_SUPPORT 
    {CS_LANG_HUNGARIAN,   DefaultHungarianMapping},
    #endif
    #ifdef IM_ITALIAN_SUPPORT 
    {CS_LANG_ITALIAN,   DefaultItalianMapping},
    #endif
    #ifdef IM_LITHUANIAN_SUPPORT 
    {CS_LANG_LITHUANIAN,   DefaultLithuanianMapping},
    #endif
    #ifdef IM_NORWEGIAN_SUPPORT 
    {CS_LANG_NORWEGIAN,   DefaultNorwegianMapping},
    #endif
    #ifdef IM_POLISH_SUPPORT 
    {CS_LANG_POLISH,   DefaultPolishMapping},
    #endif
    #ifdef IM_PORTUGUESE_SUPPORT 
    {CS_LANG_PORTUGUESE,   DefaultPortugueseMapping},
    #endif
    #ifdef IM_ROMANIAN_SUPPORT 
    {CS_LANG_ROMANIAN,   DefaultRomanianMapping},
    #endif
    #ifdef IM_RUSSIAN_SUPPORT 
    {CS_LANG_RUSSIAN,   DefaultRussianMapping},
    #endif
    #ifdef IM_SLOVAK_SUPPORT 
    {CS_LANG_SLOVAK,   DefaultSlovakMapping},
    #endif
    #ifdef IM_SLOVENIAN_SUPPORT 
    {CS_LANG_SLOVENIAN,   DefaultSlovenianMapping},
    #endif
    #ifdef IM_SPANISH_SUPPORT 
    {CS_LANG_SPANISH,   DefaultSpanishMapping},
    #endif
    #ifdef IM_SWEDISH_SUPPORT 
    {CS_LANG_SWEDISH,   DefaultSwedishMapping},
    #endif
    #ifdef IM_TURKISH_SUPPORT 
    {CS_LANG_TURKISH,   DefaultTurkishMapping},
    #endif
    #ifdef IM_UKRAINIAN_SUPPORT 
    {CS_LANG_UKRAINIAN,   DefaultUkrainianMapping},
    #endif
    #ifdef IM_ARABIC_SUPPORT 
    {CS_LANG_ARABIC,   DefaultArabicMapping},
    #endif
    #ifdef IM_HEBREW_SUPPORT 
    {CS_LANG_HEBREW,   DefaultHebrewMapping},
    #endif
    #ifdef IM_PERSIAN_SUPPORT 
    {CS_LANG_PERSIAN,   DefaultPersianMapping},
    #endif
    #ifdef IM_URDU_SUPPORT 
    {CS_LANG_URDU,   DefaultUrduMapping},
    #endif
    #ifdef IM_UYGHUR_SUPPORT 
    {CS_LANG_UYGHUR,   DefaultUyghurMapping},
    #endif
    #ifdef IM_HINDI_SUPPORT 
    {CS_LANG_HINDI,   DefaultHindiMapping},
    #endif
    #ifdef IM_BENGALI_SUPPORT 
    {CS_LANG_BENGALI,   DefaultBengaliMapping},
    #endif
    #ifdef IM_TELUGU_SUPPORT 
    {CS_LANG_TELUGU,   DefaultTeluguMapping},
    #endif
    #ifdef IM_MARATHI_SUPPORT 
    {CS_LANG_MARATHI,   DefaultMarathiMapping},
    #endif
    #ifdef IM_TAMIL_SUPPORT 
    {CS_LANG_TAMIL,   DefaultTamilMapping},
    #endif
    #ifdef IM_PUNJABI_SUPPORT 
    {CS_LANG_PUNJABI,   DefaultPunjabiMapping},
    #endif
    #ifdef IM_GUJARATI_SUPPORT 
    {CS_LANG_GUJARATI,   DefaultGujaratiMapping},
    #endif
    #ifdef IM_ORIYA_SUPPORT 
    {CS_LANG_ORIYA,   DefaultOriyaMapping},
    #endif
    #ifdef IM_KANNADA_SUPPORT 
    {CS_LANG_KANNADA,   DefaultKannadaMapping},
    #endif
    #ifdef IM_MALAYALAM_SUPPORT 
    {CS_LANG_MALAYALAM,   DefaultMalayalamMapping},
    #endif
    #ifdef IM_KHMER_SUPPORT 
    {CS_LANG_KHMER,   DefaultKhmerMapping},
    #endif
    #ifdef IM_LAO_SUPPORT 
    {CS_LANG_LAO,   DefaultLaoMapping},
    #endif
    #ifdef IM_MYANMAR_SUPPORT 
    {CS_LANG_MYANMAR,   DefaultMyanmarMapping},
    #endif

    #ifdef IM_TIBETAN_SUPPORT     // 2012 0619 tibetan 
  //  {CS_LANG_TIBETAN, DefaultTibetanMapping}
    #endif
 
    0
};

/*lint -restore */
#endif

#if defined(MMIIM_USE_KB_CSTAR)

#include "mmk_msg.h"
#include "mmi_custom_define.h"
#include "mmidisplay_data.h"
#include "guifont.h"

#include "spml_api.h"

#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
#define MMI_QWERTY_KEYPAD 
#endif
#include "mmiim_im_switching.h"
#include "mmiim_sp_symbol.h"
#include "mmi_default.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#ifdef MMI_QWERTY_KEYPAD
LOCAL const unsigned char MSG_COMMON_APP_KEY_MAP[] = 
{
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\b',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', CS_VK_NONE, 0x0F
};

LOCAL const unsigned short MSG_STROKE_APP_KEY_MAP[] = 
{
    CS_VK_NONE, CS_VK_NONE, CS_VK_NONE, CS_VK_1, CS_VK_2, CS_VK_3, CS_VK_STAR, CS_VK_NONE, CS_VK_NONE, CS_VK_NONE,
        CS_VK_NONE, CS_VK_NONE, CS_VK_NONE, CS_VK_4, CS_VK_5, CS_VK_6, CS_VK_NONE,CS_VK_NONE, CS_VK_NONE, CS_VK_NONE,
        CS_VK_NONE, CS_VK_NONE, CS_VK_NONE, CS_VK_7, CS_VK_8, CS_VK_9, CS_VK_0, CS_VK_NONE, CS_VK_NONE, CS_VK_NONE, CS_VK_NONE
};

#endif


/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: Initialize
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);

/******************************************************************************/
// Description: Terminate
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN Terminate(void *handle);

/******************************************************************************/
// Description: HandleMsg
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr);

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(MMIIM_CSTAR_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr);

/******************************************************************************/
// Description: SetMethod
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method);

/******************************************************************************/
// Description: 初始化输入法相关的数据
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void InitData(
                    GUIIM_INIT_KERNEL_T *param_ptr,
                    MMIIM_CSTAR_DATA_T *data_ptr
                    );

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitImKernel(
    MMIIM_CSTAR_DATA_T *data_ptr,
    CS_HIMM *init_Ker_ptr
);

/******************************************************************************/
// Description: 转化按键信息
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL uint32 ConvertKey(
                        MMIIM_CSTAR_DATA_T *data_ptr, 
                        GUIIM_EVENT_DATA_U *event_data_ptr,
                        CS_UINT *pEvent
                        );

/******************************************************************************/
// Description: 设置参数
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void SetOptionEx(void *handle);

/******************************************************************************/
// Description: 内核需要的回调，计算字符串的宽度
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL int get_text_width_cb(const unsigned short* text, int length);

/******************************************************************************/
// Description: checking can displaying char or not call back
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL int IsCanDisplayChar(wchar ch);

/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawPreEditWin(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT const *content_ptr);

/******************************************************************************/
// Description:IM_PINYIN_DZ库，画混淆区
// Global resource dependence: NONE
// Author: juan.wu
// Note:
/******************************************************************************/
LOCAL void DrawChoicebar(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT const *content_ptr);

/******************************************************************************/
// Description: 选字窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawCandWin(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT const *content_ptr);

/******************************************************************************/
// Description: 将系统设置的模糊音转化为cstar的设置
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 其实目前的设置是一样的
/******************************************************************************/
LOCAL uint32 GetCstarPyFuzzy(void);

/*****************************************************************************/
//  Description : 根据输入法确定是否需要画候选区或者编辑区
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrawCandAndEditBar(GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : 打开切换输入法窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSwitchIMWin(
MMIIM_CSTAR_DATA_T *data_ptr, 
CS_IMM_CONTENT *content_ptr,
GUI_BOTH_RECT_T *pop_menu_rect_ptr);

/*****************************************************************************/
//  Description : 打开符号窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSysbolWin(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT *content_ptr);

/*****************************************************************************/
//  Description : 得到输入法进入的次数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint8 GetImReloadCnt(void);

/*****************************************************************************/
//  Description : add输入法进入的次数计数器
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AddImReloadRefCnt(void);

/*****************************************************************************/
//  Description : add输入法进入的次数计数器
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void ReleaseImReloadRefCnt(void);

/*****************************************************************************/
//  Description : 设置输入法进入的次数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadCnt(uint8 im_reload_cnt);

/*****************************************************************************/
//  Description : 得到输入法是否重入
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsImReloadDirty(void);

/*****************************************************************************/
//  Description : 设置输入法是否重入
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadDirty(BOOLEAN is_dirty);

/******************************************************************************/
// Description: 创建动态
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 其实目前的设置是一样的
/******************************************************************************/
//LOCAL uint32 CreateIMDropdownlist(void);


/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
//需要在内核的回调函数里使用，用于计算宽度，但是不能作为参数传入
static GUI_FONT_T s_cand_font = 0;
static uint8 s_im_reload_cnt = 0;           //用于计数输入法进入次数
static BOOLEAN s_is_im_reload = FALSE;      //用于计数输入法进入次数

GUIIM_KERNEL_APIS_T const g_guiim_cstar_apis =
{
    Initialize,
        Terminate,
        HandleMsg,
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/******************************************************************************/
// Description: Initialize
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr)
{
    CS_HIMM init_result = PNULL;
    MMIIM_CSTAR_DATA_T *data_ptr = PNULL;
    
    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    SCI_TRACE_LOW ("[MMIIM] [cstar] Initialize: param_ptr=%08X", param_ptr);
    if(PNULL == param_ptr)
    {
        return data_ptr;
    }
    //SCI_TRACE_LOW:"[MMIIM] Initialize cstar: win_id=%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1125_112_2_18_2_24_58_22,(uint8*)"d", param_ptr->win_id);
    
    data_ptr = (MMIIM_CSTAR_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_CSTAR_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_CSTAR_DATA_T));

    //data
    data_ptr->ctrl_handle = param_ptr->im_handle;
    InitData(param_ptr, data_ptr);

    //kernel
    InitImKernel(data_ptr, &init_result);
    
    if(PNULL != init_result)
    {
        data_ptr->im_handle = init_result;
        SetOptionEx(init_result);
        SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);

        AddImReloadRefCnt();
    }
    else
    {
        SCI_FREE(data_ptr);
        return PNULL;
    }
    
    return data_ptr;
}


/******************************************************************************/
// Description: Terminate
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN Terminate(void *handle)
{
    MMIIM_CSTAR_DATA_T *data_ptr = (MMIIM_CSTAR_DATA_T*)handle;

    if (PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] Terminate: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1165_112_2_18_2_24_58_23,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIIM] Terminate cstar: cs_handle=%08X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1169_112_2_18_2_24_58_24,(uint8*)"d", data_ptr->im_handle);

    if (1 == MMIIM_GetImKerState())
    {
        CS_IMM_Terminate(data_ptr->im_handle);
        MMIIM_SetImKerState(0);
    }

    ReleaseImReloadRefCnt();
    if (0 != GetImReloadCnt())
    {
        //此时置脏
        SetImReloadDirty(TRUE);
    }    
   
    SCI_FREE(data_ptr);
    
    return TRUE;
}


/******************************************************************************/
// Description: HandleMsg
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(void *handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *event_data_ptr)
{
    MMIIM_CSTAR_DATA_T *data_ptr = (MMIIM_CSTAR_DATA_T*)handle;
    BOOLEAN result = FALSE;
    BOOLEAN is_qwert_inputing = FALSE;
    BOOLEAN has_associate = FALSE;
    
    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleMsg: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1209_112_2_18_2_24_59_25,(uint8*)"");
        return FALSE;
    }
    
    if(PNULL == data_ptr->im_handle)
    {
        return FALSE;
    }
    
    switch(event)
    {
    case GUIIM_EVENT_SYS_MSG:
        result = HandleSysMsg(data_ptr, event_data_ptr);
        break;
        
    case GUIIM_EVENT_SET_METHOD:
        result = SetMethod(handle, event_data_ptr->set_method);
        break;
        
    case GUIIM_EVENT_GET_METHOD:
        event_data_ptr->get_method = data_ptr->method;
        result = TRUE;
        break;
        
    case GUIIM_EVENT_PAINT:
        if (IsDrawCandAndEditBar(data_ptr->method))
        {
            if (0!=data_ptr->cand_win.count ||0!=data_ptr->edit_win.count||0!=data_ptr->choice_win.choice_count)
            {
                
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
                MMIIM_DrawChoiceWin(data_ptr->lcd_info_ptr,
                    &(data_ptr->choice_win));
#else
                MMIIM_DrawPreEditWin(data_ptr->lcd_info_ptr,
                    &(data_ptr->edit_win));
#endif
                if(MMIIM_WIN_STATE_USER == data_ptr->cand_win.state)
                {
                    MMIIM_DrawUserWin(data_ptr->lcd_info_ptr,
                        &(data_ptr->cand_win));
                }
                else
                {
                    MMIIM_DrawCandWin(data_ptr->lcd_info_ptr,
                        &(data_ptr->cand_win));
                }
            }
        }
        result = TRUE;
        break;
        
    case GUIIM_EVENT_SLEEP:
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        result = TRUE;
        break;
        
    case GUIIM_EVENT_GET_CTRL_RECT:
    case GUIIM_EVENT_GET_RECT:
        if (0!=data_ptr->cand_win.count ||0!=data_ptr->edit_win.count||0!=data_ptr->choice_win.choice_count)
        {
            event_data_ptr->get_rect = data_ptr->rect;
        }
        
		result = TRUE;
        break;
        
    case GUIIM_EVENT_GET_EDIT_STATE:

        /*
        9keys逻辑比较清晰
        26keys
            1 当无联想字的时候，统一就是全程delete
            2 有联想字的时候，联想字改成cancel，其他编辑过程，都是inputing
        */

        is_qwert_inputing = (GUIIM_INPUT_TYPE_QWERT == data_ptr->init_param.method_param_ptr->im_def_ptr->input);

        if (is_qwert_inputing)
        {
            if (0 != data_ptr->cand_win.count)
            {
                has_associate = ((GUIIM_LANG_CHS == data_ptr->init_param.method_param_ptr->im_def_ptr->lang)
                    || (GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_def_ptr->lang));

                if (has_associate)
                {
                    if (0 != data_ptr->edit_win.count)
                    {
                        event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                    }
                    else
                    {
                        event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
                    }
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_DEALING;
                }
            }
            else
            {
                event_data_ptr->edit_state = GUIIM_EDIT_NONE;
            }
        }
        else
        {
            if (MMIIM_WIN_STATE_USER == data_ptr->edit_win.state ||
                MMIIM_WIN_STATE_USER == data_ptr->cand_win.state)
            {
                if (0 == data_ptr->cand_win.count)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
            }
#ifdef DPHONE_SUPPORT 
			else if (0 != data_ptr->cand_win.count)
			{
                event_data_ptr->edit_state = GUIIM_EDIT_DEALING;
			}
#else
            else if (0 != data_ptr->edit_win.count)
            {
                if (data_ptr->cand_win.has_lable)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
            }
            else if (0 != data_ptr->cand_win.count)
            {
                //edit 没有内容，也可能是在编写
                if (MMIIM_WIN_STATE_EDIT == data_ptr->cand_win.state)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
                else if (data_ptr->cand_win.has_lable)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
                }
            }
#endif
            else
            {
                event_data_ptr->edit_state = GUIIM_EDIT_NONE;
            }
        }
        
        result = TRUE;

        break;
        
    default:
        break;
    }
    
    return result;
}

/******************************************************************************/
// Description: 处理TP消息
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL void HandleTpKeySelect(MMIIM_CSTAR_DATA_T *data_ptr,uint16 tp_index,CS_IMM_CONTENT *content_ptr)
{
    CS_UINT input_method = 0;
    CS_HIMM im_handle = PNULL;
    
    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleTpKeySelect: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1383_112_2_18_2_24_59_26,(uint8*)"");
        return;
    }
    
    input_method = CS_IMM_GetInputMethod(data_ptr->im_handle);
    im_handle = data_ptr->im_handle;
    
    if (PNULL == content_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleTpKeySelect PNULL == content!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1392_112_2_18_2_24_59_27,(uint8*)"");
        return;
    }
#ifdef MMI_QWERTY_KEYPAD
    if(IM_STROKE != input_method && IM_STROKE_BIG5 !=input_method)
    {
        (void)CS_IMM_EventHandle(im_handle, CS_EVENT_KEYBOARD, CS_VK_1 + tp_index, content_ptr);
    }
    else
    {
        if(MMIIM_WIN_STATE_CAND != data_ptr->cand_win.state)
            (void)CS_IMM_EventHandle(im_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, content_ptr);
        
        (void)CS_IMM_EventHandle(im_handle, CS_EVENT_KEYBOARD, CS_VK_1 + tp_index, content_ptr);
    }
#else
    if(MMIIM_WIN_STATE_CAND != data_ptr->cand_win.state)
        (void)CS_IMM_EventHandle(im_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, content_ptr);
    
    (void)CS_IMM_EventHandle(im_handle, CS_EVENT_KEYBOARD, CS_VK_1 + tp_index, content_ptr);
    
    //如果有候选字的话，要接着送一个down消息，使其进入候选字状态，修改En下，按1键输入符号，点不上屏
    if (content_ptr->nCand > 0 && CS_FLAG_STATE_USER != content_ptr->flagState)
    {
        if (IM_PINYIN != input_method && IM_STROKE != input_method
            &&IM_ZHUYIN_MT != input_method && IM_STROKE_BIG5 != input_method && IM_PINYIN_DZ != input_method)
        {
            (void)CS_IMM_EventHandle(im_handle, CS_EVENT_KEYBOARD, CS_VK_SELECT, content_ptr);
        }
    }
#endif
    
}

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(MMIIM_CSTAR_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr)
{
    static BOOLEAN is_down_pass = FALSE; //这个值是动态临时值，所以可以用static
    uint32 cs_vk = CS_VK_NONE;
    CS_RESULT handle_result = CS_ERROR_UNUSED;
    CS_IMM_CONTENT content = {0};
    int32 i = 0;
    GUI_POINT_T tp_point = {0};
    BOOLEAN is_tp_cand = FALSE;
    uint16 tp_index = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    CS_UINT event = CS_EVENT_KEYBOARD;
    GUI_BOTH_RECT_T pop_menu_rect = {0};    
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
    CS_UINT     input_method = 0;
#endif
    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleSysMsg: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1448_112_2_18_2_24_59_28,(uint8*)"");
        return FALSE;
    }

    
    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        //发生了重入现象，需要重置一下输入法
        if (IsImReloadDirty() || 0 == MMIIM_GetImKerState())
        {            
            CS_HIMM init_result = PNULL;
            SetImReloadDirty(FALSE);

            //kernel
            InitImKernel(data_ptr, &init_result);
    
            if(init_result)
            {
                data_ptr->im_handle = init_result;
                SetOptionEx(init_result);
                SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);
                
                //SCI_TRACE_LOW:"[MMIIM] HandleSysMsg: IsImReloadDirty, method=%04X"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1471_112_2_18_2_24_59_29,(uint8*)"d", data_ptr->init_param.method_param_ptr->im_set.method);
            }
            
            return TRUE;
        }
    }
    
    switch(event_data_ptr->sys_msg.msg_id)
    {
    case MSG_TIMER:
        if(MMIIM_EXPL_TIMER_ID == *(uint8*)(event_data_ptr->sys_msg.param))
        {
            handle_result = CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_TIMER, 0, &content);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        tp_point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);
        
        if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_cand))
        {
            if (MMIIM_WIN_STATE_USER == data_ptr->cand_win.state)
            {
                return TRUE;
            }

            is_tp_cand = MMIIM_TpDownCandAsKey(&data_ptr->cand_win,
                tp_point,
                &tp_index);
            
            if(is_tp_cand)
            {
                HandleTpKeySelect(data_ptr,tp_index,&content);
            }
            else
            {
                return TRUE;
            }
        }
        else if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_idc_left))
        {
            if(data_ptr->cand_win.idc_flag & MMIIM_IDC_FLAG_LEFT)
            {
                if(MMIIM_WIN_STATE_CAND != data_ptr->cand_win.state)
                {
                    handle_result = CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);
                    if(CS_ERROR_USED !=  handle_result)
                    {
                        return FALSE;
                    }
                }
                
                handle_result = CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_KEYBOARD, CS_VK_LEFT, &content);                
                if(CS_ERROR_USED !=  handle_result)
                {
                    return FALSE;
                }
            }
            else
            {
                return TRUE;
            }
        }
        else if(GUI_PointIsInRect(tp_point, data_ptr->cand_win.rect_idc_right))
        {
            if(data_ptr->cand_win.idc_flag & MMIIM_IDC_FLAG_RIGHT)
            {
                if(MMIIM_WIN_STATE_CAND != data_ptr->cand_win.state)
                {
                    handle_result = CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);
                    if(CS_ERROR_USED !=  handle_result)
                    {
                        return FALSE;
                    }
                }
                
                handle_result = CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_KEYBOARD, CS_VK_RIGHT, &content);                
                if(CS_ERROR_USED !=  handle_result)
                {
                    return FALSE;
                }
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
            return FALSE;
        }
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	case MSG_KEYLONG_DEL:        
    case MSG_KEYLONG_CANCEL:
        {
            if (MMIIM_WIN_STATE_USER == data_ptr->edit_win.state)
            {
                handle_result = 
                    CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE | CS_VK_LONGCLICK, &content);
            }
            else
            {
                //我们这里需要重新设置输入法，其实最主要目的就是要把引擎的内容清干净
                if (PNULL != data_ptr->im_handle)
                {
                    CS_IMM_SetInputMethod(data_ptr->im_handle, 
                        CS_IMM_GetInputMethod(data_ptr->im_handle));
                }

                notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            }
        }
        break;
        
    default:
        cs_vk = ConvertKey(data_ptr, event_data_ptr, &event);
        
        if(CS_VK_NONE == cs_vk)
        {
            return FALSE;
        }

#if defined(MMI_QWERTY_KEYPAD)
        if (KEY_CTRL == cs_vk)
        {
            OpenSwitchIMWin(data_ptr, &content, &pop_menu_rect);
            return TRUE;
        }
        if (KEY_FN == cs_vk)
        {
            OpenSysbolWin(data_ptr, &content);
            return TRUE;
        }
#else 
        if (CS_VK_POUND == cs_vk)
        {
            OpenSwitchIMWin(data_ptr, &content, &pop_menu_rect);
            return TRUE;
        }

#ifndef DPHONE_SUPPORT 
        //* 键用于弹出符号窗口
        if (CS_VK_STAR == cs_vk)
        {
            OpenSysbolWin(data_ptr, &content);
            return TRUE;
        }
#endif

#endif
        
        //非编辑状态的标志是没有编辑输入，此时的一个简明有效的判别方式是edit的0、2位都空
        if((0 == data_ptr->edit_win.buffer[0]) && (0 == data_ptr->edit_win.buffer[2]))
        {
            wchar num_char = 0;
            
            switch(event_data_ptr->sys_msg.msg_id & MMIIM_CSTAR_KEY_MASK)
            {
                /* key */
            case KEY_0:
                num_char = '0';
                break;
            case KEY_1:
                num_char = '1';
                break;
            case KEY_2:
                num_char = '2';
                break;
            case KEY_3:
                num_char = '3';
                break;
            case KEY_4:
                num_char = '4';
                break;
            case KEY_5:
                num_char = '5';
                break;
            case KEY_6:
                num_char = '6';
                break;
            case KEY_7:
                num_char = '7';
                break;
            case KEY_8:
                num_char = '8';
                break;
            case KEY_9:
                num_char = '9';
                break;
                
            default:
                break;
            }
            
            //按中的是数字键
            if(0 != num_char)
            {
                //非输入情况下长按数字键上数字
                if((cs_vk & 0xffff0000) == CS_VK_LONGCLICK && is_down_pass)
                {
                    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
                    notify_pack.data.commit_data.commit[0] = num_char;
                    notify_pack.data.commit_data.commit_len = 1;
                    
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                    
                    return TRUE;
                }
#if defined(MMI_QWERTY_KEYPAD)
                else if(CS_VK_NONE == event && 0 == data_ptr->cand_win.count)
                {
                    if ((cs_vk & 0xffff0000) == MMIIM_CSTAR_VKEY_PRESSSED)
                    {
                        //针对此现象，只处理down消息，这样防止多上屏一个字符
                        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
                        notify_pack.data.commit_data.commit[0] = cs_vk;
                        notify_pack.data.commit_data.commit_len = 1;
                    
                        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                    }
                    
                    return TRUE;
                }
#endif
                else if((cs_vk & 0xffff0000) == MMIIM_CSTAR_VKEY_PRESSSED)
                {
                    is_down_pass = TRUE;
                    return TRUE;
                }
                else if((cs_vk & 0xffff0000) == MMIIM_CSTAR_VKEY_RELEASED && is_down_pass)
                {
                    //不返回，修改键值
                    cs_vk &= 0x0000ffff;
                    cs_vk |= MMIIM_CSTAR_VKEY_PRESSSED;
                }
            }
        }
        
        //不处理up消息
        if((cs_vk & MMIIM_CSTAR_VKEY_RELEASED))
        {
            return FALSE;
        }
        //处理方向键的repeat消息
        if(0 != (cs_vk & CS_VK_REPEAT)) 
        {
            cs_vk &= 0x0000ffff;;
        }
        is_down_pass = FALSE;
        
#ifdef DPHONE_SUPPORT 
        if ((CS_VK_STAR == cs_vk )&&((0 == data_ptr->edit_win.buffer[0]) && (0 == data_ptr->edit_win.buffer[2]))&&
        	data_ptr->cand_win.state!=MMIIM_WIN_STATE_CAND)
		{
			if ( IM_STROKE == CS_IMM_GetInputMethod(data_ptr->im_handle))
			{
				cs_vk = CS_VK_7;
			}
			else
			{
				cs_vk = CS_VK_1;
			}
		}
        else if ( (CS_VK_1 == cs_vk && data_ptr->cand_win.state!=MMIIM_WIN_STATE_CAND && IM_STROKE != CS_IMM_GetInputMethod(data_ptr->im_handle)) ||
			 (CS_VK_7 == cs_vk && data_ptr->cand_win.state!=MMIIM_WIN_STATE_CAND && IM_STROKE == CS_IMM_GetInputMethod(data_ptr->im_handle)))
		{
 		    cs_vk = CS_VK_STAR;
		}
#endif
        if(cs_vk == (CS_VK_SELECT | CS_VK_LONGCLICK))//在有候选词时不处理OK的长按键 cr239843
        {
            return FALSE;
        }
        if(CS_VK_NONE ==event)
        {
            content.pCommit[0] = cs_vk;
        }
        else
        {
            handle_result = CS_IMM_EventHandle(data_ptr->im_handle, event, cs_vk, &content);

            //联想字状态，按ok键后，应该直接上屏，而cstar在该版本处，会先切换到lable状态，
            //此处帮用户操作一步吧
#ifdef  INPUT_METHOD_CSTAR_COMMON_MINI           
            if(content.flagState == CS_FLAG_STATE_CAND 
                && cs_vk == CS_VK_SELECT
                && (GUIIM_M_PINYIN == data_ptr->method))        //26键不需要这样处理，因26键不会切换到lable状态
            {
                SCI_MEMSET(&content, 0, sizeof(content));

                if(PNULL == data_ptr->im_handle)
                {
                    return FALSE;
                }
                else
                {
                    handle_result = CS_IMM_EventHandle(data_ptr->im_handle, event, CS_VK_SELECT, &content);
                }
            }
#endif            

            // * 号会带来数据的变化。按说，带来数据变化的都该返回"真"的
            if(CS_ERROR_USED !=  handle_result
                && CS_VK_STAR != cs_vk 
                && CS_ERROR_COMMIT != handle_result)
            {
                return FALSE;
            }
            
            break;
        }
        
        
    }
    
    /*
    定时器消息处理：
    该函数调用完成后CS_IMM_CONTENT的flagTimer字段为CS_TIMER_SET表示引擎需要UI
    开启一个Multitap输入法（即多次击键的字母输入法）的超时定时器，如该定时器
    已经启动表示需要复位该定时器；flagTimer字段为CS_TIMER_CLEAR表示引擎要求关
    闭该定时器；定时器到时，需要UI调用该函数向引擎发送定时器到时消息。
    定时器状态:
    CS_TIMER_SET，开启或重新开启multitap定时器
    CS_TIMER_CLEAR，关闭multitap定时器
    */
    if(CS_TIMER_SET == content.flagTimer)
    {
        MMIIM_StartTimer(&(data_ptr->is_timer_active), data_ptr->ctrl_handle);
    }
    else if(CS_TIMER_CLEAR == content.flagTimer || 0 == content.flagTimer)
    {
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
    }
    
    // 将pcommit返回给调用者
    i = 0;
    while(i < GUIIM_COMMIT_BUFFER_MAX && content.pCommit[i] != 0)
    {
        notify_pack.data.commit_data.commit[i] = (wchar)(content.pCommit[i]);
        i++;
    }
    if(CS_ERROR_COMMIT == handle_result && CS_VK_0 == cs_vk)
    {
        notify_pack.data.commit_data.commit[i] = '0';
        i++;
    }
    notify_pack.data.commit_data.commit_len = i;
    
    if(content.nUnderline > 0)
    {
        content.nUnderline = MIN( content.nUnderline, GUIIM_COMMIT_BUFFER_MAX );
        
        for(i = 0; i <= content.nUnderline; i++)
        {
            notify_pack.data.commit_data.underline[i] = (wchar)(content.pUnderlineBuffer[i]);
        }
        
        notify_pack.data.commit_data.underline_len = (uint8)(content.nUnderline);
    }

	if (notify_pack.data.commit_data.commit_len != 0 || notify_pack.data.commit_data.underline_len != 0)
    {
	    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
	    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
	}    
    
    if (IsDrawCandAndEditBar(data_ptr->method) && CS_VK_NONE != event)
    {
        GUIIM_NOTIFY_PACK_T notify_pack = {0};
        if (0 !=content.nChoice || 0 !=content.nPreEdit||0 !=content.nCand)
        {
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI//小版本 用DANZI的库，choice 区域要画出来
            input_method = MMIIM_ConvertMethod(data_ptr->method);
            if(IM_PINYIN_DZ == input_method)
            {
                DrawChoicebar(data_ptr, &content);
            }
            else
            {
                DrawPreEditWin(data_ptr, &content);
            }
#else
            // 更新编辑窗口内容
            DrawPreEditWin(data_ptr, &content);
#endif
            // 更新候选词窗口内容
            DrawCandWin(data_ptr, &content);
        }
        else
        {
            data_ptr->choice_win.choice_count= content.nChoice;
            data_ptr->cand_win.count= content.nCand;
            data_ptr->edit_win.count= content.nPreEdit;
        }
        
        notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
	
    }
    
    return TRUE;
}


/******************************************************************************/
// Description: SetMethod
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method)
{
    MMIIM_CSTAR_DATA_T *data_ptr = (MMIIM_CSTAR_DATA_T*)handle;
    CS_RESULT result = CS_ERROR_SUCCESS;
    CS_UINT     input_method = 0;
    //GUIIM_NOTIFY_PACK_T notify_pack = {0};    
    
    if(PNULL == handle)
    {
        //SCI_TRACE_LOW:"[MMIIM] SetMethod: handle is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1875_112_2_18_2_25_0_30,(uint8*)"");
        return FALSE;
    }
    
    data_ptr->method = method;
    
    input_method = MMIIM_ConvertMethod(method); //此函数中会做完是否物理全键盘的输入法转换操作

    result = CS_IMM_SetInputMethod(data_ptr->im_handle, input_method);

    //SCI_TRACE_LOW:"[MMIIM] SetMethod: cs_handle=%8X, cs_method=%04X, method=%04X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_1886_112_2_18_2_25_0_31,(uint8*)"ddd", data_ptr->im_handle, input_method, method);
    
    do 
    {
        //set the drawing direction of candidate bar and edit bar.
        switch (input_method)
        {
        case IM_HEBREW:
        case IM_HEBREW_MT:
        case IM_ARABIC:
        case IM_ARABIC_MT:
        case IM_URDU:
        case IM_URDU_MT:
        case IM_PERSIAN:
        case IM_PERSIAN_MT:
        case IM_UYGHUR:
        case IM_UYGHUR_MT:
        case IM_HEBREW + IM_FULLKEY_BASE:
        case IM_HEBREW_MT + IM_FULLKEY_BASE:
        case IM_ARABIC + IM_FULLKEY_BASE:
        case IM_ARABIC_MT + IM_FULLKEY_BASE:
        case IM_URDU + IM_FULLKEY_BASE:
        case IM_URDU_MT + IM_FULLKEY_BASE:
        case IM_PERSIAN + IM_FULLKEY_BASE:
        case IM_PERSIAN_MT + IM_FULLKEY_BASE:
        case IM_UYGHUR + IM_FULLKEY_BASE:
        case IM_UYGHUR_MT + IM_FULLKEY_BASE:
            data_ptr->edit_win.draw_direction  = MMIIM_DRAW_RIGHT2LEFT;
            data_ptr->cand_win.draw_direction  = MMIIM_DRAW_RIGHT2LEFT;
            data_ptr->choice_win.draw_direction = MMIIM_DRAW_RIGHT2LEFT;
            break;
        default:
            data_ptr->edit_win.draw_direction = MMIIM_DRAW_LEFT2RIGHT;
            data_ptr->cand_win.draw_direction = MMIIM_DRAW_LEFT2RIGHT;
            data_ptr->choice_win.draw_direction = MMIIM_DRAW_LEFT2RIGHT;
            break;
        }
    } while(0);
    
    do 
    {
        //下面这修改必须注意：目前的物理UI都是会在下方画两条栏的，不论是什么输入法
        //现在需求要求在abc, 123下，不要画这些栏，由于没有UI状态值，所有暂时如下修改
        if (!IsDrawCandAndEditBar(method))
        {
            data_ptr->rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);
            
            //notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
            //GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }
        else
        {
            //reset state
            data_ptr->edit_win.state = MMIIM_WIN_STATE_NONE;
            data_ptr->cand_win.state = MMIIM_WIN_STATE_NONE;
            MMIIM_ResetPreEditWin(&(data_ptr->edit_win));
            MMIIM_ResetCandWin(&data_ptr->cand_win);

        //返回时，该ui没有更新状态
        #ifdef INPUT_METHOD_CSTAR_COMMON_MINI//小版本 用DANZI的库，choice 区域刷新
            if(IM_PINYIN_DZ == input_method)
            {
                MMIIM_ResetChoiceWin(&data_ptr->choice_win);      
            }
        #endif            
            
            //notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
            //GUIIM_CommitData (data_ptr->init_param.im_handle, &notify_pack);
        }
    } while(0);
    
    return (CS_ERROR_SUCCESS == result);
}


/******************************************************************************/
// Description: 初始化输入法相关的数据
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 多处的+1/-1是边界处理
/******************************************************************************/
LOCAL void InitData(
                    GUIIM_INIT_KERNEL_T *param_ptr,
                    MMIIM_CSTAR_DATA_T *data_ptr
                    )
{
    uint8 pre_edit_height = 0;
    uint8 cand_height = 0;
    uint8 pre_choice_height = 0;
    GUI_RECT_T win_rect = {0};
    
    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == param_ptr || PNULL == data_ptr)
    {
        return;
    }
    
    data_ptr->init_param = *param_ptr;
    
    data_ptr->lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;

    //有些字顶天立地的，微调两个像素是优化显示效果必须的
    pre_edit_height = GUI_GetFontHeight(g_mmiim_ui_config.font_pre_edit, UNICODE_HANZI) + 2;
    pre_choice_height = GUI_GetFontHeight(g_mmiim_ui_config.font_pre_choice, UNICODE_HANZI) + 2;
    cand_height = GUI_GetFontHeight(g_mmiim_ui_config.font_candidate, UNICODE_HANZI) + 2;
    
    data_ptr->cand_win.lable_width = GUI_GetFontWidth(g_mmiim_ui_config.font_lable, UNICODE_ASCII);
    
    win_rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);
	//begin 20101202
    #ifdef MAINLCD_SIZE_128X64
	win_rect.bottom += MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
	#endif
	//end 20101202    
    MMIIM_InitCandWin(&data_ptr->cand_win, &win_rect, cand_height);
    
    win_rect.bottom = data_ptr->cand_win.rect_cand.top - MMIIM_EDIT_WIN_SPACE;
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
    MMIIM_InitChoiceWin(&data_ptr->choice_win, &win_rect, pre_choice_height);
    MMIIM_InitPreEditWin(&data_ptr->edit_win, &win_rect, pre_edit_height);
    data_ptr->rect.top = data_ptr->choice_win.rect.top - 1; //顶部的装饰线    
#else
    MMIIM_InitPreEditWin(&data_ptr->edit_win, &win_rect, pre_edit_height);
    data_ptr->rect.top = data_ptr->edit_win.rect.top - 1; //顶部的装饰线
#endif    

    data_ptr->rect.bottom = data_ptr->cand_win.rect_cand.bottom;
    data_ptr->rect.left = data_ptr->cand_win.rect_idc_left.left;
    data_ptr->rect.right = data_ptr->cand_win.rect_idc_right.right;
    s_cand_font = g_mmiim_ui_config.font_candidate;
    
    
    return;
}

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitImKernel(
    MMIIM_CSTAR_DATA_T *data_ptr,
    CS_HIMM *init_Ker_ptr
)
{
    CS_IMM_PARAMS im_params = {0};
    CS_USER_PARAMS user_params = {0};

#ifdef MMI_CSTAR_UDB_SUPPORT
    UDB_ENTRY_T udb_entry = MMIIM_GetUdbEntry();
#endif

    im_params.nMaxPYChoice = 1;
    im_params.nWinWidth  = data_ptr->cand_win.rect_cand.right - data_ptr->cand_win.rect_cand.left + 1;
    im_params.nLableWidth = data_ptr->cand_win.lable_width + MMIIM_CAND_WORDS_SPACE;
    im_params.pGetTextWidthFunc = get_text_width_cb;
    
#ifdef MMI_CSTAR_UDB_SUPPORT
    if(PNULL != udb_entry.udb_ptr
        && 0 != udb_entry.udb_len)
    {
        user_params.iUserCacheSize = udb_entry.udb_len;
        user_params.pUserCache = udb_entry.udb_ptr;
    }
#endif
    *init_Ker_ptr = CS_IMM_Initialize(PNULL, &im_params, &user_params);

    MMIIM_SetImKerState(1);

    return;
}    


/******************************************************************************/
// Description: 设置参数
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void SetOptionEx(void *handle)
{
    CS_IMM_OPTIONS option = {0};
    
    if (0 == handle)
    {
        return;
    }
    
    option.pfnCheckValidHanzi = IsCanDisplayChar;
    option.dwPYFuzzy = GetCstarPyFuzzy();
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    option.pQwertyInfo = cs_qwerty_info;
#else
    option.pQwertyInfo = PNULL;
#endif
    option.dwStrokeLayout = CS_STROKE_STANDARD_LAYOUT;
    option.dwAssoPinyin = CS_ASSOCIATION_ON;
    option.dwAssoStroke = CS_ASSOCIATION_ON;
    option.dwPinyinMode = CS_PINYIN_MODE_3;
    option.dwCandidateMode = CS_CANDIDATE_MODE_1;
    option.dwAlphaBeticInputMode = CS_ALPHABETIC_WORD_INPUT;
    CS_IMM_SetOptionsEx(handle, &option);
    
    return;
}


/******************************************************************************/
// Description: 内核需要的回调，计算字符串的宽度
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL int get_text_width_cb(const unsigned short* text, int length)
{
    uint16 len = (length >= 0) ? length : 0;
    
    return GUI_GetStringWidth(s_cand_font, text, len);
}

/******************************************************************************/
// Description: checking can displaying char or not call back
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL int IsCanDisplayChar(wchar ch)
{
    if (SPMLAPI_TestChar(ch))
        return 1;
    else
        return 0;
}

/******************************************************************************/
// Description: 转化按键信息
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL uint32 ConvertKey(MMIIM_CSTAR_DATA_T *data_ptr, GUIIM_EVENT_DATA_U *event_data_ptr,CS_UINT *pEvent)
{
    uint32 cs_vk = CS_VK_NONE;
    MMIIM_SP_VKEY_T sp_vk = MMIIM_SP_KEY_NONE;
    uint32 cs_vk_type = 0xffff0000;
    CS_UINT event = CS_EVENT_KEYBOARD;
    MMI_MESSAGE_ID_E msg = 0;
    GUIIM_METHOD_T  input_method = 0;
    
    if (PNULL == data_ptr || PNULL == event_data_ptr || PNULL == pEvent )
    {
        //SCI_TRACE_LOW:"[MMIIM] ConvertKey PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_2122_112_2_18_2_25_1_32,(uint8*)"");
        return CS_VK_NONE;
    }

    msg = event_data_ptr->sys_msg.msg_id;
    input_method = data_ptr->init_param.method_param_ptr->im_set.method;
    switch(msg & MMIIM_CSTAR_KEY_TYPE_MASK)
    {
    case KEY_PRESSED:
        cs_vk_type = MMIIM_CSTAR_VKEY_PRESSSED;
        break;
        
    case KEY_RELEASED:
        cs_vk_type = MMIIM_CSTAR_VKEY_RELEASED;
        break;
        
    case KEY_LONG_PRESSED:
        cs_vk_type = CS_VK_LONGCLICK;
        break;

    case KEY_REPEATED:
        switch(msg & MMIIM_CSTAR_KEY_MASK)
        {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
                cs_vk_type = CS_VK_REPEAT;
            break;

            default:
                break;
        }
        break;
        
    default:
        break;
    }
    
    if(0xffff0000 == cs_vk_type)
    {
        return CS_VK_NONE;
    }
    
    /* 键值转换 */
    sp_vk = MMIIM_PhysicalKeyMsgToImKey (msg);
    switch(sp_vk)
    {
#if !defined(MMI_QWERTY_KEYPAD)
        /* key */
    case MMIIM_SP_KEY_0:
        cs_vk = CS_VK_0;
        break;
    case MMIIM_SP_KEY_1:
        cs_vk = CS_VK_1;
        break;
    case MMIIM_SP_KEY_2:
        cs_vk = CS_VK_2;
        break;
    case MMIIM_SP_KEY_3:
        cs_vk = CS_VK_3;
        break;
    case MMIIM_SP_KEY_4:
        cs_vk = CS_VK_4;
        break;
    case MMIIM_SP_KEY_5:
        cs_vk = CS_VK_5;
        break;
    case MMIIM_SP_KEY_6:
        cs_vk = CS_VK_6;
        break;
    case MMIIM_SP_KEY_7:
        cs_vk = CS_VK_7;
        break;
    case MMIIM_SP_KEY_8:
        cs_vk = CS_VK_8;
        break;
    case MMIIM_SP_KEY_9:
        cs_vk = CS_VK_9;
        break;
    case MMIIM_SP_KEY_STAR:
        cs_vk = CS_VK_STAR;
        break;
    case MMIIM_SP_KEY_HASH:
        cs_vk = CS_VK_POUND;
        break; 
#endif
    case MMIIM_SP_KEY_CANCEL:
        cs_vk = CS_VK_BACKSPACE;
        break;
    case MMIIM_SP_KEY_LEFT:
        cs_vk = CS_VK_LEFT;
        break;
    case MMIIM_SP_KEY_UP:
        cs_vk = CS_VK_UP;
        break;
    case MMIIM_SP_KEY_RIGHT:
        cs_vk = CS_VK_RIGHT;
        break;
    case MMIIM_SP_KEY_DOWN:
        cs_vk = CS_VK_DOWN;
        break;
    case MMIIM_SP_KEY_OK:
        cs_vk = CS_VK_SELECT;
        break;
        
#if defined(MMI_QWERTY_KEYPAD)
    case MMIIM_SP_KEY_Q:    
    case MMIIM_SP_KEY_W:
    case MMIIM_SP_KEY_E:
    case MMIIM_SP_KEY_R:
    case MMIIM_SP_KEY_T:
    case MMIIM_SP_KEY_Y:
    case MMIIM_SP_KEY_U:
    case MMIIM_SP_KEY_I:
    case MMIIM_SP_KEY_O:
    case MMIIM_SP_KEY_P:
    case MMIIM_SP_KEY_A:        
    case MMIIM_SP_KEY_S:
    case MMIIM_SP_KEY_D:
    case MMIIM_SP_KEY_F:
    case MMIIM_SP_KEY_G:
    case MMIIM_SP_KEY_H:
    case MMIIM_SP_KEY_K:
    case MMIIM_SP_KEY_L:
    case MMIIM_SP_KEY_J:
    case MMIIM_SP_KEY_Z:
    case MMIIM_SP_KEY_X:
    case MMIIM_SP_KEY_C:
    case MMIIM_SP_KEY_V:
    case MMIIM_SP_KEY_B:
    case MMIIM_SP_KEY_N:
    case MMIIM_SP_KEY_M:
        if (GUIIM_M_Q_STROKE == input_method ||GUIIM_M_Q_123 == input_method)
        {
            cs_vk = MSG_STROKE_APP_KEY_MAP[msg - MSG_APP_Q];
        }
        else
        {
            cs_vk = 'a' + sp_vk - MMIIM_SP_KEY_A;     //按A~Z的顺序排列//MSG_COMMON_APP_KEY_MAP[sp_vk - MMIIM_SP_KEY_Q];
            event = CS_EVENT_CHAR;
        }
        break;    
    case MMIIM_SP_KEY_FN:
        cs_vk = KEY_FN;
        break;
        
    case MMIIM_SP_KEY_0:
        cs_vk = CS_VK_0;
        /*如果全键盘编辑处于无输入状态，则送这些值应该直接commit*/
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        else if(MMIIM_WIN_STATE_CAND == data_ptr->cand_win.state)
        {
            event = CS_EVENT_CHAR;
        }
        break;
    case MMIIM_SP_KEY_1:
        cs_vk = CS_VK_1;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_2:
        cs_vk = CS_VK_2;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_3:
        cs_vk = CS_VK_3;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_4:
        cs_vk = CS_VK_4;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_5:
        cs_vk = CS_VK_5;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_6:
        cs_vk = CS_VK_6;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_7:
        cs_vk = CS_VK_7;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_8:
        cs_vk = CS_VK_8;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_9:
        cs_vk = CS_VK_9;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MMIIM_SP_KEY_AT_QWERTY:
        cs_vk = (uint16)'@';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_AND:
        cs_vk = (uint16)'&';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_SPACE:
        cs_vk = (uint16)' ';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_ENTER: 
        cs_vk = (uint16)'\n';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_SHIFT:
        cs_vk = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_DEL:
        cs_vk = CS_VK_BACKSPACE;
        if (0 == data_ptr->cand_win.count)
        {
            event = CS_VK_NONE;
        }
        break;
    case MSG_KEYLONG_DEL:
        cs_vk = (CS_VK_BACKSPACE|CS_VK_LONGCLICK);
        break;
    case MMIIM_SP_KEY_CTRL:
        cs_vk = KEY_CTRL;
        break;

    //MMK不支持的按键
    case MMIIM_SP_KEY_QUOTES:
        cs_vk = (uint16)'\'';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_EQUAL:
        cs_vk = (uint16)'=';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_RIGHT_SLASH:
        cs_vk = (uint16)'\\';
        event = CS_VK_NONE;
        break;
        
    case MMIIM_SP_KEY_PLUS:
        cs_vk = (uint16)'+';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_MINUS:
        cs_vk = (uint16)'-';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_DOUBLE_QUOTES:
        cs_vk = (uint16)'"';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_SLASH:
        cs_vk = (uint16)'/';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_EXCLAMATION:
        cs_vk = (uint16)'!';
        event = CS_VK_NONE;
        break;
#if 0   //不再与MMK系统shift相关，前面的physicaltoim已经做了转换  
    case MMIIM_SP_KEY_SHIFT_AND:
        cs_vk = (uint16)'\'';
        event = CS_VK_NONE;
        break;
#endif        
    case MMIIM_SP_KEY_STAR:
        cs_vk = (uint16)'*';
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_HASH:
        cs_vk = (uint16)'#';
        event = CS_VK_NONE;
        break;

    //下面这些符号存在全角和半角之分
    case MMIIM_SP_KEY_COMMA:
        cs_vk = (uint16)',';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_PERIOD:
        cs_vk = (uint16)'.';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_QUESTION:
        cs_vk = (uint16)'?';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_LEFT_PARENTHESIS:
        cs_vk = (uint16)'(';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_RIGHT_PARENTHESIS:
        cs_vk = (uint16)')';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_SEMICOLON:
        cs_vk = (uint16)';';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;
    case MMIIM_SP_KEY_COLON:
        cs_vk = (uint16)':';
#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
        if (MMIIM_GetSymbType (input_method) == MMIIM_SP_SYMB_TYPE_FULL)
        {
            cs_vk = MMIIM_ConvertToFullSymbol (cs_vk);
        }
#endif
        event = CS_VK_NONE;
        break;

#endif
    default:
        break;
    }
    
    if(CS_VK_NONE != cs_vk)
    {
        cs_vk |= cs_vk_type;
    }
    *pEvent = event;
    
    //SCI_TRACE_LOW:"[MMIIM] ConvertKey: cs_vk=%d, cs_vk_type=%X, event=%d, msg=%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_CSTAR_2464_112_2_18_2_25_1_33,(uint8*)"dddd",cs_vk, cs_vk_type, event, msg);
    
    return cs_vk;
}


/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawPreEditWin(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT const *content_ptr)
{    
    MMIIM_ResetPreEditWin(&(data_ptr->edit_win));
    
    /*
    flagState 当前输入法状态：
    1 编辑状态，候选词标签不用显示
    2 选择状态，显示候选词标签
    3 英文自造词状态，显示自造词窗口
    */
    if(CS_FLAG_STATE_EDIT == content_ptr->flagState)
    {
        data_ptr->edit_win.state = MMIIM_WIN_STATE_EDIT;
    }
    else if(CS_FLAG_STATE_CAND == content_ptr->flagState)
    {
        data_ptr->edit_win.state = MMIIM_WIN_STATE_CAND;
    }
    else if(CS_FLAG_STATE_USER == content_ptr->flagState)
    {
        data_ptr->edit_win.state = MMIIM_WIN_STATE_USER;
    }
    else
    {
        //这一步是多余的，因为在模式3的状态下，cstar他们返回的flagstate值不对，有内容也是0，应该是edit状态。
        if (0 != content_ptr->nPreEdit)
        {
            data_ptr->edit_win.state = MMIIM_WIN_STATE_CAND;
        }
        else
        {
            data_ptr->edit_win.state = MMIIM_WIN_STATE_NONE;
        }
    }
    /*
    pPreBuffer  需要在Pre-Edit窗口显示的字符串，使用'\0'分割多个项
    nPreEdit    Pre-Edit共有多少项
    iHotPreEdit 需要显示光标的项id，从0开始计算
    */
    SCI_MEMCPY(data_ptr->edit_win.buffer, content_ptr->pPreBuffer, sizeof(data_ptr->edit_win.buffer));
    data_ptr->edit_win.count = content_ptr->nPreEdit;
    
    data_ptr->edit_win.has_cursor = TRUE;
    data_ptr->edit_win.cursor_index = content_ptr->iHotPreEdit;
    
    
    if(0 != data_ptr->edit_win.count )
    {
        MMIIM_DrawPreEditWin(data_ptr->lcd_info_ptr,
            &(data_ptr->edit_win));
    }
    
    return;
}
/******************************************************************************/
// Description:IM_PINYIN_DZ库，画混淆区
// Global resource dependence: NONE
// Author: juan.wu
// Note:
/******************************************************************************/
LOCAL void DrawChoicebar(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT const *content_ptr)
{
    MMIIM_ResetChoiceWin(&(data_ptr->choice_win));
    
    if(CS_FLAG_STATE_EDIT == content_ptr->flagState)
    {
        data_ptr->choice_win.state = MMIIM_WIN_STATE_EDIT;
    }
    else if(CS_FLAG_STATE_CAND == content_ptr->flagState)
    {
        data_ptr->choice_win.state = MMIIM_WIN_STATE_CAND;
    }
    else if(CS_FLAG_STATE_USER == content_ptr->flagState)
    {
        data_ptr->choice_win.state = MMIIM_WIN_STATE_USER;
    }
    else
    {
        //这一步是多余的，因为在模式3的状态下，cstar他们返回的flagstate值不对，有内容也是0，应该是edit状态。
        if (0 != content_ptr->nChoice)
        {
            data_ptr->choice_win.state = MMIIM_WIN_STATE_CAND;
        }
        else
        {
            data_ptr->choice_win.state = MMIIM_WIN_STATE_NONE;
        }
    }
    
    SCI_MEMCPY(data_ptr->choice_win.buffer, content_ptr->pChoiceBuffer, sizeof(data_ptr->choice_win.buffer));
    data_ptr->choice_win.choice_count= content_ptr->nChoice;
    
    data_ptr->choice_win.has_cursor = TRUE;
    data_ptr->choice_win.cursor_index = content_ptr->iHotChoice;
    
    
    if (0 != data_ptr->choice_win.choice_count)
    {
        MMIIM_DrawChoiceWin(data_ptr->lcd_info_ptr,
            &(data_ptr->choice_win));
    }

}

/******************************************************************************/
// Description: 选字窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawCandWin(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT const *content_ptr)
{
    MMIIM_ResetCandWin(&data_ptr->cand_win);
    
    if(CS_FLAG_STATE_USER == content_ptr->flagState)
    {
    /*
    pUserWordBuffer 英文自造词状态时，自造词窗口的内容
    nUserWord 英文自造词窗口的字符个数+1
    iHotUserWord 英文自造词窗口的光标位置
        */
        data_ptr->cand_win.state = MMIIM_WIN_STATE_USER;
        data_ptr->cand_win.count = content_ptr->nUserWord - 1;
        SCI_MEMCPY(data_ptr->cand_win.buffer,
            content_ptr->pUserWordBuffer,
            ((uint32)(data_ptr->cand_win.count)*sizeof(data_ptr->cand_win.buffer[0])));
        data_ptr->cand_win.hot_index = content_ptr->iHotUserWord;
        
        MMIIM_DrawUserWin(data_ptr->lcd_info_ptr,
            &(data_ptr->cand_win));
    }
    else
    {
    /*
    pCandBuffer 候选词窗口的字符串，使用'\0'分割多个候选词
    nCand 当前候选词的个数
    iHotCand 被选中的候选词id，从0开始计算，最少总有一个被选中
        */
        //SCI_ASSERT(sizeof(data_ptr->cand_win.buffer) >= sizeof(content_ptr->pCandBuffer)); /*assert verified*/
        if(sizeof(data_ptr->cand_win.buffer) < sizeof(content_ptr->pCandBuffer))    /*lint !e506 !e774*/    //虽然该值在当前宏定义情况下，恒为真，当防止宏定义错误
        {
            return;
        }
        
        data_ptr->cand_win.count = content_ptr->nCand;
        
        if(content_ptr->nCand == 0)
        {
            data_ptr->cand_win.state = MMIIM_WIN_STATE_NONE;
        }
        else
        {
            
            SCI_MEMCPY(data_ptr->cand_win.buffer, content_ptr->pCandBuffer, sizeof(content_ptr->pCandBuffer));
            
            data_ptr->cand_win.has_hot = TRUE;
            data_ptr->cand_win.hot_index = content_ptr->iHotCand;
            
            /*
            flagState 当前输入法状态：
            1 编辑状态，候选词标签不用显示
            2 选择状态，显示候选词标签
            3 英文自造词状态，显示自造词窗口
            */
            if(CS_FLAG_STATE_EDIT == content_ptr->flagState)
            {
                data_ptr->cand_win.state = MMIIM_WIN_STATE_EDIT;
            }
            else if(CS_FLAG_STATE_CAND == content_ptr->flagState)
            {
                data_ptr->cand_win.state = MMIIM_WIN_STATE_CAND;
                data_ptr->cand_win.has_lable = TRUE;
            }
            else
            {
                data_ptr->cand_win.state = MMIIM_WIN_STATE_NONE;
            }
            
            /*
            flagPageCand 候选词窗口是否需要显示上下翻页图标：
            0不显示，1显示上翻页，2显示下翻页，3显示上下翻页
            注意还要考虑一些从右往左显示的语言的处理
            */
            if(content_ptr->flagPageCand & CS_FLAG_PAGEDOWN)
            {
                if (MMIIM_DRAW_RIGHT2LEFT == data_ptr->cand_win.draw_direction)
                {
                    data_ptr->cand_win.idc_flag |= MMIIM_IDC_FLAG_RIGHT;
                }
                else
                {
                    data_ptr->cand_win.idc_flag |= MMIIM_IDC_FLAG_LEFT;
                }
            }
            
            if(content_ptr->flagPageCand & CS_FLAG_PAGEUP)
            {
                if (MMIIM_DRAW_RIGHT2LEFT == data_ptr->cand_win.draw_direction)
                {
                    data_ptr->cand_win.idc_flag |= MMIIM_IDC_FLAG_LEFT;
                }
                else
                {
                    data_ptr->cand_win.idc_flag |= MMIIM_IDC_FLAG_RIGHT;
                }
            }
        }
        
		if(0 != data_ptr->cand_win.count)
        {
            MMIIM_DrawCandWin(data_ptr->lcd_info_ptr,
                &(data_ptr->cand_win));
		}
    }
    
    return;
}

/******************************************************************************/
// Description: 将系统设置的模糊音转化为cstar的设置
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 其实目前的设置是一样的
/******************************************************************************/
LOCAL uint32 GetCstarPyFuzzy(void)
{
    uint32 fuzzy_set_cstar = 0;
    uint32 fuzzy_set_sys = MMIIM_GetPyFuzzy();
    
    if(fuzzy_set_sys & MMIIM_FUZZY_Z_ZH)
    {
        fuzzy_set_cstar |= CS_FUZZY_Z_ZH;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_C_CH)
    {
        fuzzy_set_cstar |= CS_FUZZY_C_CH;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_S_SH)
    {
        fuzzy_set_cstar |= CS_FUZZY_S_SH;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_H_F)
    {
        fuzzy_set_cstar |= CS_FUZZY_H_F;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_N_L)
    {
        fuzzy_set_cstar |= CS_FUZZY_N_L;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_IN_ING)
    {
        fuzzy_set_cstar |= CS_FUZZY_IN_ING;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_EN_ENG)
    {
        fuzzy_set_cstar |= CS_FUZZY_EN_ENG;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_AN_ANG)
    {
        fuzzy_set_cstar |= CS_FUZZY_AN_ANG;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_IAN_IANG)
    {
        fuzzy_set_cstar |= CS_FUZZY_IAN_IANG;
    }
    
    if(fuzzy_set_sys & MMIIM_FUZZY_UAN_UANG)
    {
        fuzzy_set_cstar |= CS_FUZZY_UAN_UANG;
    }
    
    return fuzzy_set_cstar;
}

/*****************************************************************************/
//  Description : 根据输入法确定是否需要画候选区或者编辑区
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrawCandAndEditBar(GUIIM_METHOD_T method)
{
    if (GUIIM_M_ENGLISH_ABC_LEAD_UPPER == method ||
        GUIIM_M_ENGLISH_ABC_LOWER == method ||
        GUIIM_M_ENGLISH_ABC_UPPER == method ||
        GUIIM_M_123 == method ||
        GUIIM_M_Q_ENGLISH_ABC_LEAD_UPPER == method ||
        GUIIM_M_Q_ENGLISH_ABC_LOWER == method ||
        GUIIM_M_Q_ENGLISH_ABC_UPPER == method ||
        GUIIM_M_Q_123 == method)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : 打开切换输入法窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSwitchIMWin(
MMIIM_CSTAR_DATA_T *data_ptr, 
CS_IMM_CONTENT *content_ptr,
GUI_BOTH_RECT_T *pop_menu_rect_ptr)
{
    if (PNULL == data_ptr
        || PNULL == content_ptr 
        || PNULL == pop_menu_rect_ptr)
    {
        return;
    }
    
    //在开启新窗口前，关掉定时器，并通知引擎
    if (data_ptr->is_timer_active)
    {
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_TIMER, 0, content_ptr);
    }            
    
    //输入法控件统一处理切换输入法
    MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, pop_menu_rect_ptr);
    MMIIM_SwitchingCreateWin(&(data_ptr->init_param), *pop_menu_rect_ptr, MMIIM_EP_TOP_LEFT);
}

/*****************************************************************************/
//  Description : 打开符号窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenSysbolWin(MMIIM_CSTAR_DATA_T *data_ptr, CS_IMM_CONTENT *content_ptr)
{
    if (PNULL == data_ptr || PNULL == content_ptr)
    {
        return;
    }
    
    //在开启新窗口前，关掉定时器，并通知引擎
    if (data_ptr->is_timer_active)
    {
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        CS_IMM_EventHandle(data_ptr->im_handle, CS_EVENT_TIMER, 0, content_ptr);
    }
    
    //输入法控件统一处理切换输入法
    if(PNULL == data_ptr->init_param.method_param_ptr->im_set.method)
    {
        return;
    }
    
    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle, 
        data_ptr->init_param.method_param_ptr->im_set.lang);
}

/*****************************************************************************/
//  Description : 得到输入法进入的次数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint8 GetImReloadCnt(void)
{
    return s_im_reload_cnt;
}

/*****************************************************************************/
//  Description : 设置输入法进入的次数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadCnt(uint8 im_reload_cnt)
{
    s_im_reload_cnt = im_reload_cnt;
}

/*****************************************************************************/
//  Description : add输入法进入的次数计数器
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AddImReloadRefCnt(void)
{
    s_im_reload_cnt++;
}

/*****************************************************************************/
//  Description : add输入法进入的次数计数器
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void ReleaseImReloadRefCnt(void)
{
    if (0 != s_im_reload_cnt)
    {
        s_im_reload_cnt--;
    }
}

/*****************************************************************************/
//  Description : 得到输入法是否重入
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsImReloadDirty(void)
{
    return s_is_im_reload;
}

/*****************************************************************************/
//  Description : 设置输入法是否重入
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadDirty(BOOLEAN is_dirty)
{
    s_is_im_reload = is_dirty;
}

#else


//define a fake variable
GUIIM_KERNEL_APIS_T const g_guiim_cstar_apis ={0};

#endif //MMIIM_USE_KB_CSTAR



/*Edit by script, ignore 1 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
