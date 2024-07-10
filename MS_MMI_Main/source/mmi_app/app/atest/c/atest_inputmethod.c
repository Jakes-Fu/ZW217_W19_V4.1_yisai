/***************************************************************************************
** FileName    :  atest_inputmethod.c                                                          **
** Author      :  lin.chen                                                           **
** Date        :  20/12/2011                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The implementaion of the step and action in the multi-lang testing.        **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  20/12/2011     lin.chen         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/

#include "atest_inputmethod.h"
#include "sci_api.h"
#include "atest_signal.h"
#include "mmiim_sp_ml9key_data.h"
#include "mmiim_sp_ml26key_data.h"
#include "mmitheme_im.h"

#ifdef WIN32
/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/
LOCAL QWERTY_KEY_MAP_T QwertyKeyMap[MAX_ABC_NUM]=	/*lint !e808*/
{
	{"A",0xb1},		/*lint !e155*/
	{"B",0xb2},
	{"C",0xb3},
	{"D",0xb4},
	{"E",0xb5},
	{"F",0xb6},
	{"G",0xb7},
	{"H",0xb8},
	{"I",0xb9},
	{"J",0xba},
	{"K",0xbb},
	{"L",0xbc},
	{"M",0xbd},
	{"N",0xbe},
	{"O",0xbf},
	{"P",0xc0},
	{"Q",0xc1},
	{"R",0xc2},
	{"S",0xc3},
	{"T",0xc4},
	{"U",0xc5},
	{"V",0xc6},
	{"W",0xc7},
	{"X",0xc8},
	{"Y",0xc9},
	{"Z",0xca},
};


LOCAL ATEST_LANGUAGE_MAP_T s_sp_ml_list[]=	/*lint !e808*/
{
 	{GUIIM_M_TPQ_ENGLISH_ABC_LOWER,GUIIM_M_ENGLISH_ABC_LOWER,"English",	/*lint !e155*/
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    "english_107",
#else
    "english_103",
#endif
	"ENGLISH","英语"},
 //   {MMISET_LANGUAGE_SIMP_CHINESE,   GUIIM_LANG_CHS},
 //   {MMISET_LANGUAGE_TRAD_CHINESE,   GUIIM_LANG_CHT},
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
#ifdef IM_ARABIC_SUPPORT
    {MMISET_LANGUAGE_ARABIC,GUIIM_M_ARABIC_ABC,"Arabic",
#if defined(SPECIAL_ARABIC_PERSIAN)    
    "arabic_809",
#else
	"arabic_807",
#endif
	"ARABIC","阿拉伯语"},
#endif

#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_TPQ_FRENCH_ABC_LOWER,GUIIM_M_FRENCH_ABC_LOWER,"French","french_407","FRENCH","法语"},
#endif

#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_TPQ_HINDI_ABC,GUIIM_M_HINDI_ABC,"Hindi",
#if defined(SPECIAL_HINDI)
    "hindi_908",
#else
    "hindi_907",
#endif
	"HINDI","印地语"},
#endif

#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_TPQ_HUNGARIAN_ABC_LOWER,GUIIM_M_HUNGARIAN_ABC_LOWER,"Hungarian","hungarian_417","HUNGARIAN","匈牙利语"},
#endif

#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_TPQ_INDONESIAN_ABC_LOWER,GUIIM_M_INDONESIAN_ABC_LOWER,"Indonesian","indonesian_327","INDONESIAN","印度尼西亚语"},
#endif
        
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_TPQ_MALAY_ABC_LOWER,GUIIM_M_MALAY_ABC_LOWER,"Malay","malay_347","MALAY","马来语"},
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_TPQ_PORTUGUESE_ABC_LOWER,GUIIM_M_PORTUGUESE_ABC_LOWER,"Portuguese","portuguese_607","PORTUGUESE","葡萄牙语"},
#endif

#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_TPQ_RUSSIAN_ABC_LOWER,GUIIM_M_RUSSIAN_ABC_LOWER,"Russian","russian_207","RUSSIAN","俄语"},
#endif

#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_TPQ_SPANISH_ABC_LOWER,GUIIM_M_SPANISH_ABC_LOWER,"Spanish","spanish_507","SPANISH","西班牙语"},
#endif

#ifdef IM_TAGALOG_SUPPORT 
    {GUIIM_M_TPQ_TAGALOG_ABC,GUIIM_M_TAGALOG_ABC,"Tagalog","tagalog_587","TAGALOG","他加禄语"},
#endif

#ifdef IM_THAI_SUPPORT
    {GUIIM_M_TPQ_THAI_ABC,GUIIM_M_THAI_ABC,"Thai",
#if defined(SPECIAL_THAI)
    "thai_368",
#else
    "thai_367",
#endif
	"THAI","泰语"},
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_TPQ_VIETNAMESE_ABC_LOWER,GUIIM_M_VIETNAMESE_ABC_LOWER,"Vietnamese","vietnamese_317","VIETNAMESE","越南语"},
#endif

#ifdef IM_URDU_SUPPORT
    {GUIIM_M_TPQ_URDU_ABC,GUIIM_M_URDU_ABC,"Urdu","urdu_957","URDU","乌尔都语"},
#endif

#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_TPQ_ITALIAN_ABC_LOWER,GUIIM_M_ITALIAN_ABC_LOWER,"Italian","italian_707","ITALIAN","意大利语"},
#endif

#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_TPQ_PERSIAN_ABC,GUIIM_M_PERSIAN_ABC,"Persian",
#if defined(SPECIAL_ARABIC_PERSIAN) 
    "persian_469",
#else
    "persian_467",
#endif
	"PERSIAN","波斯语"},
#endif

#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_TPQ_TURKISH_ABC_LOWER,GUIIM_M_TURKISH_ABC_LOWER,"Turkish","turkish_337","TURKISH","土耳其语"},
#endif

#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_TPQ_GERMAN_ABC_LOWER,GUIIM_M_GERMAN_ABC_LOWER,"German","german_307","GERMAN","德语"},
#endif

#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_TPQ_GREEK_ABC_LOWER,GUIIM_M_GREEK_ABC_LOWER,"Greek","greek_357","GREEK","希腊语"},
#endif   

#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_TPQ_HEBREW_ABC,GUIIM_M_HEBREW_ABC,"Hebrew","hebrew_427","HEBREW","希伯来语"},
#endif
 
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_TPQ_BENGALI_ABC,GUIIM_M_BENGALI_ABC,"Bengali","bengali_927","BENGALI","孟加拉语"},
#endif
 
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_TPQ_CZECH_ABC_LOWER,GUIIM_M_CZECH_ABC_LOWER,"Czech","czech_447","CZECH","捷克语"},
#endif
 
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_TPQ_SLOVENIAN_ABC_LOWER,GUIIM_M_SLOVENIAN_ABC_LOWER,"Slovenian","slovenian_537","SLOVENIAN","斯洛文尼亚语"},
#endif
 
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_TPQ_ROMANIAN_ABC_LOWER,GUIIM_M_ROMANIAN_ABC_LOWER,"Romanian","romanian_527","ROMANIAN","罗马尼亚语"},
#endif
#else 
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_TPQ_ARABIC_ABC,GUIIM_M_ARABIC_ABC,"Arabic",
#if defined(SPECIAL_ARABIC_PERSIAN)    
    "arabic_803",
#else
	"arabic_800",
#endif
	"ARABIC","阿拉伯语"},
#endif

#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_TPQ_FRENCH_ABC_LOWER,GUIIM_M_FRENCH_ABC_LOWER,"French","french_400","FRENCH","法语"},
#endif

#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_TPQ_HINDI_ABC,GUIIM_M_HINDI_ABC,"Hindi",
#if defined(SPECIAL_HINDI)
    "hindi_906",
#else
    "hindi_900",
#endif
	"HINDI","印地语"},
#endif

#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_TPQ_HUNGARIAN_ABC_LOWER,GUIIM_M_HUNGARIAN_ABC_LOWER,"Hungarian","hungarian_410","HUNGARIAN","匈牙利语"},
#endif

#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_TPQ_INDONESIAN_ABC_LOWER,GUIIM_M_INDONESIAN_ABC_LOWER,"Indonesian","indonesian_320","INDONESIAN","印度尼西亚语"},
#endif
        
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_TPQ_MALAY_ABC_LOWER,GUIIM_M_MALAY_ABC_LOWER,"Malay","malay_340","MALAY","马来语"},
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_TPQ_PORTUGUESE_ABC_LOWER,GUIIM_M_PORTUGUESE_ABC_LOWER,"Portuguese","portuguese_600","PORTUGUESE","葡萄牙语"},
#endif

#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_TPQ_RUSSIAN_ABC_LOWER,GUIIM_M_RUSSIAN_ABC_LOWER,"Russian","russian_202","RUSSIAN","俄语"},
#endif

#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_TPQ_SPANISH_ABC_LOWER,GUIIM_M_SPANISH_ABC_LOWER,"Spanish","spanish_500","SPANISH","西班牙语"},
#endif

#ifdef IM_TAGALOG_SUPPORT 
    {GUIIM_M_TPQ_TAGALOG_ABC,GUIIM_M_TAGALOG_ABC,"Tagalog","tagalog_587","TAGALOG","他加禄语"},
#endif

#ifdef IM_THAI_SUPPORT
    {GUIIM_M_TPQ_THAI_ABC,GUIIM_M_THAI_ABC,"Thai",
#if defined(SPECIAL_THAI)
    "thai_364",
#else
    "thai_360",
#endif
	"THAI","泰语"},
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_TPQ_VIETNAMESE_ABC_LOWER,GUIIM_M_VIETNAMESE_ABC_LOWER,"Vietnamese","vietnamese_310","VIETNAMESE","越南语"},
#endif

#ifdef IM_URDU_SUPPORT
    {GUIIM_M_TPQ_URDU_ABC,GUIIM_M_URDU_ABC,"Urdu","urdu_950","URDU","乌尔都语"},
#endif

#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_TPQ_ITALIAN_ABC_LOWER,GUIIM_M_ITALIAN_ABC_LOWER,"Italian","italian_700","ITALIAN","意大利语"},
#endif

#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_TPQ_PERSIAN_ABC,GUIIM_M_PERSIAN_ABC,"Persian",
#if defined(SPECIAL_ARABIC_PERSIAN) 
    "persian_463",
#else
    "persian_460",
#endif
	"PERSIAN","波斯语"},
#endif

#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_TPQ_TURKISH_ABC_LOWER,GUIIM_M_TURKISH_ABC_LOWER,"Turkish","turkish_330","TURKISH","土耳其语"},
#endif

#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_TPQ_GERMAN_ABC_LOWER,GUIIM_M_GERMAN_ABC_LOWER,"German","german_300","GERMAN","德语"},
#endif

#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_TPQ_GREEK_ABC_LOWER,GUIIM_M_GREEK_ABC_LOWER,"Greek","greek_350","GREEK","希腊语"},
#endif   

#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_TPQ_HEBREW_ABC,GUIIM_M_HEBREW_ABC,"Hebrew","hebrew_420","HEBREW","希伯来语"},
 #endif
 
 #ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_TPQ_BENGALI_ABC,GUIIM_M_BENGALI_ABC,"Bengali","bengali_920","BENGALI","孟加拉语"},
 #endif
 
 #ifdef IM_CZECH_SUPPORT
    {GUIIM_M_TPQ_CZECH_ABC_LOWER,GUIIM_M_CZECH_ABC_LOWER,"Czech","czech_440","CZECH","捷克语"},
 #endif
 
 #ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_TPQ_SLOVENIAN_ABC_LOWER,GUIIM_M_SLOVENIAN_ABC_LOWER,"Slovenian","slovenian_530","SLOVENIAN","斯洛文尼亚语"},
 #endif
 
 #ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_TPQ_ROMANIAN_ABC_LOWER,GUIIM_M_ROMANIAN_ABC_LOWER,"Romanian","romanian_520","ROMANIAN","罗马尼亚语"},
 #endif 
 #endif
 
 #ifdef IM_TELUGU_SUPPORT
    {GUIIM_M_TPQ_TELUGU_ABC,GUIIM_M_TELUGU_ABC,"Telugu","telugu","TELUGU","泰卢固语"},
 #endif
 
 #ifdef IM_MARATHI_SUPPORT
    {GUIIM_M_TPQ_MARATHI_ABC,GUIIM_M_MARATHI_ABC,"Marathi","marathi","MARATHI","马拉地语"},
 #endif
 
 #ifdef IM_TAMIL_SUPPORT
    {GUIIM_M_TPQ_TAMIL_ABC,GUIIM_M_TAMIL_ABC,"Tamil","tamil","TAMIL","泰米尔语"},
 #endif
 
 #ifdef IM_GUJARATI_SUPPORT
    {GUIIM_M_TPQ_GUJARATI_ABC,GUIIM_M_GUJARATI_ABC,"Gujarati","gujarati","GUJARATI","吉古拉特语"},
 #endif
 
 #ifdef IM_KANNADA_SUPPORT
    {GUIIM_M_TPQ_KANNADA_ABC,GUIIM_M_KANNADA_ABC,"Kannada","kannada","KANNADA","卡纳达语"},
 #endif
 
 #ifdef IM_MALAYALAM_SUPPORT
    {GUIIM_M_TPQ_MALAYALAM_ABC,GUIIM_M_MALAYALAM_ABC,"Malayalam","malayalam","MALAYALAM","马来亚语"},
 #endif
 
 #ifdef IM_ORIYA_SUPPORT
    {GUIIM_M_TPQ_ORIYA_ABC,GUIIM_M_ORIYA_ABC,"Oriya","oriya","ORIYA","奥里亚语"},
 #endif
 
 #ifdef IM_PUNJABI_SUPPORT
    {GUIIM_M_TPQ_PUNJABI_ABC,GUIIM_M_PUNJABI_ABC,"Punjabi","punjabi","PUNJABI","旁遮普语"},
 #endif
 
 #ifdef IM_AFRIKAANS_SUPPORT
    {GUIIM_M_TPQ_AFRIKAANS_ABC_LOWER,GUIIM_M_AFRIKAANS_ABC_LOWER,"Afrikaans","afrikaans","AFRIKAANS","南非荷兰语"},
 #endif
 
 #ifdef IM_ALBANIAN_SUPPORT
    {GUIIM_M_TPQ_ALBANIAN_ABC_LOWER,GUIIM_M_ALBANIAN_ABC_LOWER,"Albanian","albanian","ALBANIAN","阿尔巴尼亚语"},
 #endif
 
 #ifdef IM_ARMENIAN_SUPPORT
    {GUIIM_M_TPQ_ARMENIAN_ABC_LOWER,GUIIM_M_ARMENIAN_ABC_LOWER,"Armenian","armenian","ARMENIAN","亚美尼亚语"},
 #endif
 
 #ifdef IM_AZERBAIJANI_SUPPORT
    {GUIIM_M_TPQ_AZERBAIJANI_ABC_LOWER,GUIIM_M_AZERBAIJANI_ABC_LOWER,"Azerbaijani","azerbaijani","AZERBAIJANI","阿塞拜疆语"},
 #endif
 
 #ifdef IM_BASQUE_SUPPORT
    {GUIIM_M_TPQ_BASQUE_ABC_LOWER,GUIIM_M_BASQUE_ABC_LOWER,"Basque","basque","BASQUE","巴斯克语"},
 #endif

 #ifdef IM_BULGARIAN_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_BULGARIAN_ABC_LOWER,GUIIM_M_BULGARIAN_ABC_LOWER,"Bulgarian","bulgarian_437","BULGARIAN","保加利亚语"},
 #else
    {GUIIM_M_TPQ_BULGARIAN_ABC_LOWER,GUIIM_M_BULGARIAN_ABC_LOWER,"Bulgarian","bulgarian_430","BULGARIAN","保加利亚语"},
 #endif
 #endif
 
 #ifdef IM_CATALAN_SUPPORT
    {GUIIM_M_TPQ_CATALAN_ABC_LOWER,GUIIM_M_CATALAN_ABC_LOWER,"Catalan","catalan","CATALAN","加泰罗尼亚语"},
 #endif
 
 #ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_TPQ_CROATIAN_ABC_LOWER,GUIIM_M_CROATIAN_ABC_LOWER,"Croatian","croatian","CROATIAN","克罗地亚语"},
 #endif

 #ifdef IM_DANISH_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_DANISH_ABC_LOWER,GUIIM_M_DANISH_ABC_LOWER,"Danish","danish_377","DANISH","丹麦语"},
 #else
    {GUIIM_M_TPQ_DANISH_ABC_LOWER,GUIIM_M_DANISH_ABC_LOWER,"Danish","danish_370","DANISH","丹麦语"},
 #endif
 #endif
 
 #ifdef IM_DUTCH_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
   {GUIIM_M_TPQ_DUTCH_ABC_LOWER,GUIIM_M_DUTCH_ABC_LOWER,"Dutch","dutch_387","DUTCH","荷兰语"},
 #else
   {GUIIM_M_TPQ_DUTCH_ABC_LOWER,GUIIM_M_DUTCH_ABC_LOWER,"Dutch","dutch_380","DUTCH","荷兰语"},
 #endif
 #endif
 
 #ifdef IM_ESTONIAN_SUPPORT
    {GUIIM_M_TPQ_ESTONIAN_ABC_LOWER,GUIIM_M_ESTONIAN_ABC_LOWER,"Estonian","estonian","ESTONIAN","爱沙尼亚语"},
 #endif
 
 #ifdef IM_FILIPINO_SUPPORT
    {GUIIM_M_TPQ_FILIPINO_ABC_LOWER,GUIIM_M_FILIPINO_ABC_LOWER,"Filipino","filipino","FILIPINO","菲律宾语"},
 #endif
 
 #ifdef IM_FINNISH_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_FINNISH_ABC_LOWER,GUIIM_M_FINNISH_ABC_LOWER,"Finnish","finnish_457","FINNISH","芬兰语"},
 #else
    {GUIIM_M_TPQ_FINNISH_ABC_LOWER,GUIIM_M_FINNISH_ABC_LOWER,"Finnish","finnish_450","FINNISH","芬兰语"},
 #endif
 #endif
 
 #ifdef IM_GALICIAN_SUPPORT
    {GUIIM_M_TPQ_GALICIAN_ABC_LOWER,GUIIM_M_GALICIAN_ABC_LOWER,"Galician","galician","GALICIAN","加利西亚语"},
 #endif
 
 #ifdef IM_GEORGIAN_SUPPORT
    {GUIIM_M_TPQ_GEORGIAN_ABC,GUIIM_M_GEORGIAN_ABC,"Georgian","georgian","GEORGIAN","乔治亚语"},
 #endif
 
 #ifdef IM_HAUSA_SUPPORT
    {GUIIM_M_TPQ_HAUSA_ABC_LOWER,GUIIM_M_HAUSA_ABC_LOWER,"Hausa","hausa","HAUSA","豪萨语"},
 #endif
 
 #ifdef IM_ICELANDIC_SUPPORT
    {GUIIM_M_TPQ_ICELANDIC_ABC_LOWER,GUIIM_M_ICELANDIC_ABC_LOWER,"Icelandic","icelandic","ICELANDIC","冰岛语"},
 #endif
 
 #ifdef IM_IGBO_SUPPORT
    {GUIIM_M_TPQ_IGBO_ABC_LOWER,GUIIM_M_IGBO_ABC_LOWER,"Igbo","igbo","IGBO","伊博语"},
 #endif
 
 #ifdef IM_IRISH_SUPPORT
    {GUIIM_M_TPQ_IRISH_ABC_LOWER,GUIIM_M_IRISH_ABC_LOWER,"Irish","irish","IRISH","Irish语"},
 #endif
 
 #ifdef IM_KAZAKH_SUPPORT
    {GUIIM_M_TPQ_KAZAKH_ABC_LOWER,GUIIM_M_KAZAKH_ABC_LOWER,"Kazakh","kazakh","KAZAKH","哈萨克语"},
 #endif
 
 #ifdef IM_LATVIAN_SUPPORT
    {GUIIM_M_TPQ_LATVIAN_ABC_LOWER,GUIIM_M_LATVIAN_ABC_LOWER,"Latvian","latvian","LATVIAN","拉脱维亚语"},
 #endif
 
 #ifdef IM_LITHUANIAN_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_LITHUANIAN_ABC_LOWER,GUIIM_M_LITHUANIAN_ABC_LOWER,"Lithuanian","lithuanian_567","LITHUANIAN","立陶宛语"},
 #else
    {GUIIM_M_TPQ_LITHUANIAN_ABC_LOWER,GUIIM_M_LITHUANIAN_ABC_LOWER,"Lithuanian","lithuanian_560","LITHUANIAN","立陶宛语"},
 #endif
 #endif
 
 #ifdef IM_MACEDONIAN_SUPPORT
    {GUIIM_M_TPQ_MACEDONIAN_ABC_LOWER,GUIIM_M_MACEDONIAN_ABC_LOWER,"Macedonian","macedonian","MACEDONIAN","马其顿语"},
 #endif
 
 #ifdef IM_MOLDOVAN_SUPPORT
    {GUIIM_M_TPQ_MOLDOVAN_ABC_LOWER,GUIIM_M_MOLDOVAN_ABC_LOWER,"Moldovan","moldovan","MOLDOVAN","摩尔多瓦语"},
 #endif
 
 #ifdef IM_NORWEGIAN_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_NORWEGIAN_ABC_LOWER,GUIIM_M_NORWEGIAN_ABC_LOWER,"Norwegian","norwegian_487","NORWEGIAN","挪威语"},
 #else
    {GUIIM_M_TPQ_NORWEGIAN_ABC_LOWER,GUIIM_M_NORWEGIAN_ABC_LOWER,"Norwegian","norwegian_480","NORWEGIAN","挪威语"},
 #endif
 #endif
 
 #ifdef IM_POLISH_SUPPORT
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_POLISH_ABC_LOWER,GUIIM_M_POLISH_ABC_LOWER,"Polish","polish_397","POLISH","波兰语"},
 #else
     {GUIIM_M_TPQ_POLISH_ABC_LOWER,GUIIM_M_POLISH_ABC_LOWER,"Polish","polish_391","POLISH","波兰语"},
 #endif
 #endif
 
 #ifdef IM_SERBIAN_SUPPORT
    {GUIIM_M_TPQ_SERBIAN_ABC_LOWER,GUIIM_M_SERBIAN_ABC_LOWER,"Serbian","serbian","SERBIAN","塞尔维亚语"},
 #endif
 
 #ifdef IM_SESOTHO_SUPPORT
    {GUIIM_M_TPQ_SESOTHO_ABC_LOWER,GUIIM_M_SESOTHO_ABC_LOWER,"Sesotho","sesotho","SESOTHO","塞索托语"},
 #endif
 
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
 #ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_TPQ_SLOVAK_ABC_LOWER,GUIIM_M_SLOVAK_ABC_LOWER,"Slovak","slovak_477","SLOVAK","斯洛伐克语"},
 #endif
 
 #ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_TPQ_SWEDISH_ABC_LOWER,GUIIM_M_SWEDISH_ABC_LOWER,"Swedish","swedish_497","SWEDISH","瑞典语"},
 #endif
 
 #ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_TPQ_UKRAINIAN_ABC_LOWER,GUIIM_M_UKRAINIAN_ABC_LOWER,"Ukrainian","ukrainian_547","UKRAINIAN","乌克兰语"},
 #endif
 #else
 #ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_TPQ_SLOVAK_ABC_LOWER,GUIIM_M_SLOVAK_ABC_LOWER,"Slovak","slovak_470","SLOVAK","斯洛伐克语"},
 #endif
 
 #ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_TPQ_SWEDISH_ABC_LOWER,GUIIM_M_SWEDISH_ABC_LOWER,"Swedish","swedish_490","SWEDISH","瑞典语"},
 #endif
 
 #ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_TPQ_UKRAINIAN_ABC_LOWER,GUIIM_M_UKRAINIAN_ABC_LOWER,"Ukrainian","ukrainian_540","UKRAINIAN","乌克兰语"},
 #endif
 #endif
 
 #ifdef IM_YORUBA_SUPPORT
    {GUIIM_M_TPQ_YORUBA_ABC_LOWER,GUIIM_M_YORUBA_ABC_LOWER,"Yoruba","yoruba","YORUBA","约鲁巴语"},
 #endif
 
 #ifdef IM_XHOSA_SUPPORT
    {GUIIM_M_TPQ_XHOSA_ABC_LOWER,GUIIM_M_XHOSA_ABC_LOWER,"Xhosa","xhosa","XHOSA","科萨语"},
 #endif
 
 #ifdef IM_ZULU_SUPPORT	
    {GUIIM_M_TPQ_ZULU_ABC_LOWER,GUIIM_M_ZULU_ABC_LOWER,"Zulu","zulu","ZULU","祖鲁语"},
 #endif
 
 #ifdef IM_ASSAMESE_SUPPORT	
    {GUIIM_M_TPQ_ASSAMESE_ABC,GUIIM_M_ASSAMESE_ABC,"Assamese","assamese","ASSAMESE","阿萨姆语"},
 #endif
  
 #ifdef IM_SWAHILI_SUPPORT	
 #if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {GUIIM_M_TPQ_SWAHILI_ABC_LOWER,GUIIM_M_SWAHILI_ABC_LOWER,"Swahili","swahili_557","SWAHILI","斯瓦西里语"},
 #else
    {GUIIM_M_TPQ_SWAHILI_ABC_LOWER,GUIIM_M_SWAHILI_ABC_LOWER,"Swahili","swahili_550","SWAHILI","斯瓦西里语"},
 #endif
 #endif
 
 #ifdef IM_MYANMAR_SUPPORT	
    {GUIIM_M_TPQ_MYANMAR_ABC,GUIIM_M_MYANMAR_ABC,"Myanmar","myanmar","MYANMAR","缅甸语"},
 #endif

 #ifdef IM_AMHARIC_SUPPORT	
    {GUIIM_M_TPQ_AMHARIC_ABC,GUIIM_M_AMHARIC_ABC,"Amharic","amharic","AMHARIC","阿姆哈拉语"},
 #endif
 
  #ifdef IM_KHMER_SUPPORT	
    {GUIIM_M_TPQ_KHMER_ABC,GUIIM_M_KHMER_ABC,"Khmer","khmer","KHMER","高棉语"},
 #endif
 
  #ifdef IM_LAO_SUPPORT	
    {GUIIM_M_TPQ_LAO_ABC,GUIIM_M_LAO_ABC,"Lao","lao","LAO","老挝语"},
 #endif
};

 

LOCAL char  s_LangSheet[ATEST_STRING_MAX_LEN] = {0};
LOCAL char  s_LangSection[ATEST_STRING_MAX_LEN] = {0};
LOCAL BOOLEAN b_KeyPadFlag = FALSE;


/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description      : send the message which notify the control panel 
//                    that the language multitap testing 
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendInputMethodTestedToCPMsg();



/**-----------------------------------------------------------------------------------*
 **                                Function Implementation                            *
 **-----------------------------------------------------------------------------------*/
 
/**************************************************************************************/
// Description      : send the message which notify the control panel 
//                    that the language multitap testing 
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendInputMethodTestedToCPMsg()
{
    ATEST_APP_SIG_T *signal_ptr = PNULL; 

    // create signal
    SCI_CREATE_SIGNAL(signal_ptr,
        CPANEL_ATEST_REQ,
        sizeof(ATEST_APP_SIG_T),
        SCI_IDENTIFY_THREAD() );
    
    // set the sig_ptr's param
    signal_ptr->signal_id = ATEST_MC_INPUTMETHOD_INFO;
   	strncat(signal_ptr->record_data,s_LangSheet,ATEST_STRING_MAX_LEN);
	strncat(signal_ptr->test_data,s_LangSection,ATEST_STRING_MAX_LEN);
	signal_ptr->check_type = b_KeyPadFlag;

   // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
}

/**************************************************************************************/
// Description      : get sp ml chars and testing
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
PUBLIC void Atest_InputMethodTesting(BOOLEAN  KeyPadFlag,char *LangTestingFilePath)
{
	FILE *Spd_Multitap_ptr;
	char Spd_Multitap_ResFile[MAX_MULTI_PATH];
	char KeyPadMark[ATEST_STRING_MAX_LEN];

	int atest_language;
	uint16 keychar;
	int key;
	int keyflag;
	int keychartotalnum;
	int keynum;
	int actualkeynum;

	MMIIM_SP_9KEY_TABLE_DEF_T *SP_9key_table_ptr = PNULL;
	MMIIM_SP_26KEY_KEYCHAR_T *SP_26key_table_ptr = PNULL;
	wchar *cur_char_ptr = PNULL;

	SCI_MEMSET(Spd_Multitap_ResFile,0,MAX_MULTI_PATH);
	SCI_MEMSET(KeyPadMark,0,ATEST_STRING_MAX_LEN);
	SCI_MEMSET(s_LangSection,0,ATEST_STRING_MAX_LEN);
	SCI_MEMSET(s_LangSheet,0,ATEST_STRING_MAX_LEN);

	b_KeyPadFlag = KeyPadFlag;
	//设置键盘类型
	if(!b_KeyPadFlag)//b_KeyPadFlag=0 为9键盘
	{
		strncpy(KeyPadMark,"Typical",strlen("Typical"));
	}
	else
	{
		strncpy(KeyPadMark,"Qwerty",strlen("Qwerty"));
	}

#ifdef IM_ENGINE_CSTAR //CStar
	strncpy(s_LangSection,"CStar",strlen("CStar"));
#elif defined IM_ENGINE_SOGOU//Sogou
	strncpy(s_LangSection,"Sogou",strlen("Sogou"));
#else
	strncpy(s_LangSection,"None",strlen("None"));
#endif	

#if defined IM_ENGINE_CSTAR || IM_ENGINE_SOGOU
	//构造Sprd multitap数据结果路径加文件名
	strncpy(Spd_Multitap_ResFile,LangTestingFilePath,strlen(LangTestingFilePath));
	strncat(Spd_Multitap_ResFile,"\\",strlen("\\"));
	strncat(Spd_Multitap_ResFile,s_LangSection,strlen(s_LangSection));
	strncat(Spd_Multitap_ResFile,"_",strlen("_"));
	strncat(Spd_Multitap_ResFile,KeyPadMark,strlen(KeyPadMark));
	strncat(Spd_Multitap_ResFile,"_Sp_Multitap.ini",strlen("_Sp_Multitap.ini"));

	strncat(s_LangSection,"&",strlen("&"));
	Spd_Multitap_ptr = fopen(Spd_Multitap_ResFile,"w+");
	if(NULL == Spd_Multitap_ptr)
	{
		return;
	}

	for (atest_language = 0; atest_language < ATEST_MAX_LANGUAGE; atest_language++)
	{	
		if(!b_KeyPadFlag)//universe: b_KeyPadFlag=0 为9键盘
		{
			SP_9key_table_ptr = (MMIIM_SP_9KEY_TABLE_DEF_T *)MMIIM_SP_ML9KEY_GetKeyTable(s_sp_ml_list[atest_language].gui_im_T_language);//获得对应语言的数据指针
			fprintf(Spd_Multitap_ptr,"[%s]\n",s_sp_ml_list[atest_language].language_up_name);		/*lint !e560*/

			strncat(s_LangSection,s_sp_ml_list[atest_language].language_up_name,strlen(s_sp_ml_list[atest_language].language_up_name));
			strncat(s_LangSection,"&",strlen("&"));
		#ifdef IM_ENGINE_CSTAR //CStar
			strncat(s_LangSheet,s_sp_ml_list[atest_language].cstar_typical_sheet_name,strlen(s_sp_ml_list[atest_language].cstar_typical_sheet_name));
		#else//Sogou
			strncat(s_LangSheet,s_sp_ml_list[atest_language].language_Chi_name,strlen(s_sp_ml_list[atest_language].language_Chi_name));
		#endif			
			strncat(s_LangSheet,"&",strlen("&"));
		
			for (key = MMIIM_SP_9KEY_INDEX_1; key <= MMIIM_SP_9KEY_INDEX_0; key++)
			{
				actualkeynum = 0;
				cur_char_ptr = (wchar *)SP_9key_table_ptr + key * (MMIIM_SP_9KEY_CHAR_MAX + 1);
				keychartotalnum = cur_char_ptr[0];      //字符个数
				for (keynum = 1; keynum <= keychartotalnum; keynum++)
				{
					keychar = cur_char_ptr[keynum];
					if(!((keychar <= 0x0040) 
					|| (keychar >= 0x005b && keychar <= 0x0060) 
					|| (keychar >= 0x007b && keychar <= 0x00bf)
					|| (keychar >= 0x2000 && keychar <= 0x20b9) 
					|| (keychar >= 0x2400 && keychar <= 0x2426))) 
					{
						actualkeynum++;
						if(MMIIM_SP_9KEY_INDEX_0 == key)
						{
							keyflag = MMIIM_SP_9KEY_INDEX_1;
						}
						else
						{
							keyflag = key+1;
						}
						fprintf(Spd_Multitap_ptr,"key%x%x =0x%04x\n",keyflag,actualkeynum,keychar);
					}
				}
			}
		}
		else //universe:全键盘
		{
			SP_26key_table_ptr = (MMIIM_SP_26KEY_KEYCHAR_T *)MMIIM_SP_ML26KEY_GetKeyTable(s_sp_ml_list[atest_language].gui_im_Q_language);//获得对应语言的数据指针
			fprintf(Spd_Multitap_ptr,"[%s]\n",s_sp_ml_list[atest_language].language_up_name);		/*lint !e560*/

			strncat(s_LangSection,s_sp_ml_list[atest_language].language_up_name,strlen(s_sp_ml_list[atest_language].language_up_name));
			strncat(s_LangSection,"&",strlen("&"));

		#ifdef IM_ENGINE_CSTAR //CStar
			strncat(s_LangSheet,s_sp_ml_list[atest_language].cstar_qwerty_sheet_name,strlen(s_sp_ml_list[atest_language].cstar_qwerty_sheet_name));
		#else//Sogou
			strncat(s_LangSheet,s_sp_ml_list[atest_language].language_Chi_name,strlen(s_sp_ml_list[atest_language].language_Chi_name));
		#endif
			strncat(s_LangSheet,"&",strlen("&"));

			for (key = MMIIM_SP_26KEY_INDEX_A; key <= MMIIM_SP_26KEY_INDEX_Z; key++)
			{
				actualkeynum = 0;
				cur_char_ptr = SP_26key_table_ptr[key].key_chars;
				keychartotalnum = cur_char_ptr[0];      //字符个数
				for (keynum = 1; keynum <= keychartotalnum; keynum++)
				{
					keychar = cur_char_ptr[keynum];
					if(!((keychar <= 0x0040) 
					|| (keychar >= 0x005b && keychar <= 0x0060) 
					|| (keychar >= 0x007b && keychar <= 0x00bf)
					|| (keychar >= 0x2000 && keychar <= 0x20b9) 
					|| (keychar >= 0x2400 && keychar <= 0x2426))) 
					{
						actualkeynum++;
						fprintf(Spd_Multitap_ptr,"key%s%x =0x%04x\n",QwertyKeyMap[key].Qwerty_key,actualkeynum,keychar);	/*lint !e560*/
					}
				}
			}
		}
	}

	fclose(Spd_Multitap_ptr);
#endif

	//发送消息通知controlpanel开始进行数据比较
	ATEST_SendInputMethodTestedToCPMsg();

}
#endif






