
/***************************************************************************
 
Description: Input method library interface

Copyright (C) 2000-2007 Beijing Komoxo Inc.
All rights reserved.

****************************************************************************/

#ifndef IIMEK2API_H
#define IIMEK2API_H


#ifndef IEKIE_IM_LDB_TEST
//#define IEKIE_IM_LDB_TEST
#endif

//声明是展讯平台

//平台限定宏:---应该在本地文件中定义类似的:	
#ifndef SPREADTRUM_PRODUCT//此宏是从app.mk文件中来!
#define SPREADTRUM_PRODUCT
#endif

#ifndef NDEBUG
#define NDEBUG
#endif

#ifndef ARM_PLATFORM
#define ARM_PLATFORM
#endif





//ZHMCH	这些数值怎么确定?
#if defined(SPREADTRUM_PRODUCT)

/*
App.mk (make\app):#MCFLAG_OPT += -DNDEBUG -DARM_PLATFORM -DSPREADTRUM_PRODUCT -DKEYBOARD_IME_SHARP -DIIME_ALL_IN_ONE_DATABASE -DKMX_IME_THAI_KEY0_SPACE_MULTITAP -DIIME_ZHUYIN_NOKIA -DIIME_FULL_VERSION -DIIME_LEGACY_FULLKEY -DKMX_CFG_DISABLE_START_CHAR_CHECK 
App.mk (make\app):MCFLAG_OPT += -DNDEBUG -DARM_PLATFORM -DSPREADTRUM_PRODUCT -DKEYBOARD_IME_SHARP -DIIME_ALL_IN_ONE_DATABASE -DKMX_IME_THAI_KEY0_SPACE_MULTITAP -DIIME_FULL_VERSION -DIIME_LEGACY_FULLKEY -DKMX_CFG_DISABLE_START_CHAR_CHECK
*/

//App.mk (make\app):MCFLAG_OPT += 
/*
DNDEBUG
DARM_PLATFORM
DSPREADTRUM_PRODUCT
DKEYBOARD_IME_SHARP
DIIME_ALL_IN_ONE_DATABASE
DKMX_IME_THAI_KEY0_SPACE_MULTITAP
DIIME_FULL_VERSION
DIIME_LEGACY_FULLKEY
DKMX_CFG_DISABLE_START_CHAR_CHEC
*/

#ifndef KEYBOARD_IME_SHARP
#define KEYBOARD_IME_SHARP
#endif

//ZHMCH	这个宏与根据宏KMX_LDB_XXXXX	定义的ldb并不是对立关系!
#ifndef IIME_ALL_IN_ONE_DATABASE
#define IIME_ALL_IN_ONE_DATABASE
#endif

#ifndef KMX_IME_THAI_KEY0_SPACE_MULTITAP
#define KMX_IME_THAI_KEY0_SPACE_MULTITAP
#endif
#ifndef IIME_FULL_VERSION
#define IIME_FULL_VERSION
#endif
#ifndef IIME_LEGACY_FULLKEY
#define IIME_LEGACY_FULLKEY
#endif
#ifndef KMX_CFG_DISABLE_START_CHAR_CHECK
#define KMX_CFG_DISABLE_START_CHAR_CHECK
#endif





#ifndef IIME_PID_NAME0
#define IIME_PID_NAME0  (115)
#endif

#ifndef IIME_PID_NAME1
#define IIME_PID_NAME1  (112)
#endif


#ifndef IIME_PID_NAME2
#define IIME_PID_NAME2  (114)
#endif


#ifndef IIME_PID_NAME3
#define IIME_PID_NAME3  (101)
#endif


#ifndef IIME_PID_NAME4
#define IIME_PID_NAME4  (97)
#endif


#ifndef IIME_PID_NAME5
#define IIME_PID_NAME5  (100)
#endif


#ifndef IIME_PID_VERMAJOR
#define IIME_PID_VERMAJOR  (1)
#endif


#ifndef IIME_PID_VERMINOR
#define IIME_PID_VERMINOR  (05)
#endif


#ifndef IIME_PID_BUILDER
#define IIME_PID_BUILDER  (0120)	
#endif

//配置选项
//迫切需要的验证项目
//打开所有语言的所有ldb
#ifndef IEKIE_INPUT_ALL_LANGUAGE_ALL_LDB//等效于宏:IIME_ALL_IN_ONE_DATABASE
//#define IEKIE_INPUT_ALL_LANGUAGE_ALL_LDB
#endif
//打开所有语言，每种语言都只提供一种ldb
#ifndef IEKIE_INPUT_ALL_LANGUAGE_ONE_LDB
//#define IEKIE_INPUT_ALL_LANGUAGE_ONE_LDB
#endif
//打开某些语言，每种语言都提供对应的所有ldb文件
#ifndef IEKIE_INPUT_SAME_LANGUAGE_ALL_LDB
//#define IEKIE_INPUT_SAME_LANGUAGE_ALL_LDB
#endif
//打开某些语言，每种语言都只配置一个ldb文件
#ifndef IEKIE_INPUT_SAME_LANGUAGE_ONE_LDB
//#define IEKIE_INPUT_SAME_LANGUAGE_ONE_LDB
#endif





//以下宏，需要根据语言配置进行限定
//此处需要把以下宏全部注释掉，然后再IIMEtar源代码基础上进行限定!
//--------------------------------------------------//


//根据配置的语言使用有限的ldb资源
//测试:只打开英文---IIMEtar库文件大小
//不导入所有ldb数据时，根据语言配置ldb
#if 1//寸草不生
#ifndef IIME_ALL_IN_ONE_DATABASE

#if 1//chuanyin

#ifndef KMX_LDB_ENGLISH
#define KMX_LDB_ENGLISH  (107)
#endif

#ifndef KMX_LDB_FRENCH
#define KMX_LDB_FRENCH  (400)
#endif

#ifndef KMX_LDB_SWAHILI
#define KMX_LDB_SWAHILI  (550)
#endif

//HAUSA	IM_HAUSA_SUPPORT  没有针对此语言的支持，展讯已经提供了multitap

#ifndef KMX_LDB_PORTUGUESE
#define KMX_LDB_PORTUGUESE  (600)
#endif

#ifndef KMX_LDB_ARABIC
#define KMX_LDB_ARABIC  (807)
#endif

#ifndef KMX_LDB_AMHARIC
#define KMX_LDB_AMHARIC  (1)
#endif

#ifndef KMX_LDB_PERSIAN
#define KMX_LDB_PERSIAN  (460)
#endif

#ifndef KMX_LDB_BENGALI
#define KMX_LDB_BENGALI  (9201)//9200  编译不通过 by zmc
#endif

#ifndef KMX_LDB_MYANMAR
#define KMX_LDB_MYANMAR  (1)
#endif

#ifndef KMX_LDB_URDU
#define KMX_LDB_URDU  (950)
#endif


#ifndef KMX_LDB_SINHALA
#define KMX_LDB_SINHALA  (770)
#endif


#ifndef KMX_LDB_VIETNAMESE
#define KMX_LDB_VIETNAMESE  (310)
#endif


#ifndef KMX_LDB_THAI
#define KMX_LDB_THAI  (360)
#endif

#ifndef KMX_LDB_KHMER
#define KMX_LDB_KHMER  (1)
#endif

#ifndef KMX_LDB_INDONESIAN
#define KMX_LDB_INDONESIAN  (320)
#endif

#ifndef KMX_LDB_MALAY
#define KMX_LDB_MALAY  (340)
#endif

#ifndef KMX_LDB_TAGALOG
#define KMX_LDB_TAGALOG  (587)
#endif

#ifndef KMX_LDB_RUSSIAN
#define KMX_LDB_RUSSIAN  (202)
#endif

#ifndef KMX_LDB_RUSSIAN
#define KMX_LDB_RUSSIAN  (202)
#endif


#ifndef KMX_LDB_UKRAINIAN
#define KMX_LDB_UKRAINIAN  (540)
#endif

#ifndef KMX_LDB_GUJARATI
#define KMX_LDB_GUJARATI  (1)
#endif

#ifndef KMX_LDB_TELUGU
#define KMX_LDB_TELUGU  (1)
#endif

#ifndef KMX_LDB_PUNJABI
#define KMX_LDB_PUNJABI  (1)
#endif

#ifndef KMX_LDB_MARATHI
#define KMX_LDB_MARATHI  (1)
#endif

#ifndef KMX_LDB_TAMIL
#define KMX_LDB_TAMIL  (1)
#endif

#ifndef KMX_LDB_ASSAMESE
#define KMX_LDB_ASSAMESE  (710)
#endif

#ifndef KMX_LDB_KANNADA
#define KMX_LDB_KANNADA  (1)
#endif

#ifndef KMX_LDB_MALAYALAM
#define KMX_LDB_MALAYALAM  (1)
#endif

#ifndef KMX_LDB_KHMER
#define KMX_LDB_KHMER  (1)
#endif

#ifndef KMX_LDB_SPANISH
#define KMX_LDB_SPANISH  (500)
#endif

#ifndef KMX_LDB_TURKISH
#define KMX_LDB_TURKISH  (330)
#endif


#ifndef KMX_LDB_TAGALOG
#define KMX_LDB_TAGALOG  (587)
#endif

#endif//chuanyin


#if 0
#ifndef KMX_LDB_ENGLISH
#define KMX_LDB_ENGLISH  (107)
#endif

#ifndef KMX_LDB_NEPALI
//#define KMX_LDB_NEPALI  (720)
#endif

#ifndef KMX_LDB_SINHALA
#define KMX_LDB_SINHALA  (770)
#endif

//daodejing
#ifndef KMX_LDB_BODO
//#define KMX_LDB_BODO  (1100)
#endif

#ifndef KMX_LDB_DOGRI
//#define KMX_LDB_DOGRI  (1110)
#endif

#ifndef KMX_LDB_SANSKRIT
//#define KMX_LDB_SANSKRIT  (1120)
#endif

#ifndef KMX_LDB_KASHMIRI
//#define KMX_LDB_KASHMIRI  (1130)
#endif

#ifndef KMX_LDB_KONKANI
//#define KMX_LDB_KONKANI  (1140)
#endif

#ifndef KMX_LDB_MAITHILI
//#define KMX_LDB_MAITHILI  (1150)
#endif

#ifndef KMX_LDB_MANIPURI
//#define KMX_LDB_MANIPURI  (1160)
#endif

#ifndef KMX_LDB_SANTALI
//#define KMX_LDB_SANTALI  (1170)
#endif

#ifndef KMX_LDB_SOMALI
//#define KMX_LDB_SOMALI  (1180)
#endif

#ifndef KMX_LDB_SINDHI
//#define KMX_LDB_SINDHI  (1190)
#endif

#ifndef KMX_LDB_ARABIC
#define KMX_LDB_ARABIC  (807)
#endif

#ifndef KMX_LDB_ARMENIAN
#define KMX_LDB_ARMENIAN  (570)
#endif


#ifndef KMX_LDB_ALBANIAN
#define KMX_LDB_ALBANIAN  (611)
#endif

#ifndef KMX_LDB_AMHARIC
#define KMX_LDB_AMHARIC  (1)
#endif

#ifndef KMX_LDB_ASSAMESE
#define KMX_LDB_ASSAMESE  (710)
#endif

#ifndef KMX_LDB_BASQUE
#define KMX_LDB_BASQUE  (631)
#endif

#ifndef KMX_LDB_BENGALI
#define KMX_LDB_BENGALI  (9201)//9200  ZHMCH	编译不通过
#endif

#ifndef KMX_LDB_BULGARIAN
#define KMX_LDB_BULGARIAN  (430)
#endif

#ifndef KMX_LDB_CATALAN
#define KMX_LDB_CATALAN  (621)
#endif

#ifndef KMX_LDB_CROATIAN
#define KMX_LDB_CROATIAN  (510)
#endif


#ifndef KMX_LDB_DANISH
#define KMX_LDB_DANISH  (370)
#endif

#ifndef KMX_LDB_DUTCH
#define KMX_LDB_DUTCH  (380)
#endif

#ifndef KMX_LDB_ENGLISH
#define KMX_LDB_ENGLISH  (103)
#endif

#ifndef KMX_LDB_FINNISH
#define KMX_LDB_FINNISH  (450)
#endif

#ifndef KMX_LDB_FRENCH
#define KMX_LDB_FRENCH  (400)
#endif

#ifndef KMX_LDB_GALICIAN
#define KMX_LDB_GALICIAN  (641)
#endif

#ifndef KMX_LDB_GERMAN
#define KMX_LDB_GERMAN  (300)
#endif

#ifndef KMX_LDB_GREEK
#define KMX_LDB_GREEK  (350)
#endif

#ifndef KMX_LDB_GUJARATI
#define KMX_LDB_GUJARATI  (1)
#endif

#ifndef KMX_LDB_HANGUL
#define KMX_LDB_HANGUL  (1)
#endif

#ifndef KMX_LDB_HEBREW
#define KMX_LDB_HEBREW  (420)
#endif

#ifndef KMX_LDB_HINDI
#define KMX_LDB_HINDI  (900)
#endif

#ifndef KMX_LDB_HINGLISH
#define KMX_LDB_HINGLISH  (910)
#endif

#ifndef KMX_LDB_HUNGARIAN
#define KMX_LDB_HUNGARIAN  (410)
#endif

#ifndef KMX_LDB_IGBO
#define KMX_LDB_IGBO  (651)
#endif

#ifndef KMX_LDB_INDONESIAN
#define KMX_LDB_INDONESIAN  (320)
#endif

#ifndef KMX_LDB_ITALIAN
#define KMX_LDB_ITALIAN  (700)
#endif

#ifndef KMX_LDB_KANNADA
#define KMX_LDB_KANNADA  (1)
#endif

#ifndef KMX_LDB_KHMER
#define KMX_LDB_KHMER  (1)
#endif

#ifndef KMX_LDB_KYRGYZ
#define KMX_LDB_KYRGYZ  (747)
#endif

#ifndef KMX_LDB_LAO
#define KMX_LDB_LAO  (1)
#endif

#ifndef KMX_LDB_LITHUANIAN
#define KMX_LDB_LITHUANIAN  (560)
#endif

#ifndef KMX_LDB_MALAY
#define KMX_LDB_MALAY  (340)
#endif

#ifndef KMX_LDB_MALAYALAM
#define KMX_LDB_MALAYALAM  (1)
#endif

#ifndef KMX_LDB_MARATHI
#define KMX_LDB_MARATHI  (1)
#endif

#ifndef KMX_LDB_MYANMAR
#define KMX_LDB_MYANMAR  (1)
#endif

#ifndef KMX_LDB_NEPALI
#define KMX_LDB_NEPALI  (720)
#endif

#ifndef KMX_LDB_SINHALA
#define KMX_LDB_SINHALA  (770)
#endif

//daodejing
#ifndef KMX_LDB_BODO
#define KMX_LDB_BODO  (1100)
#endif

#ifndef KMX_LDB_DOGRI
#define KMX_LDB_DOGRI  (1110)
#endif

#ifndef KMX_LDB_SANSKRIT
#define KMX_LDB_SANSKRIT  (1120)
#endif

#ifndef KMX_LDB_KASHMIRI
#define KMX_LDB_KASHMIRI  (1130)
#endif

#ifndef KMX_LDB_KONKANI
#define KMX_LDB_KONKANI  (1140)
#endif

#ifndef KMX_LDB_MAITHILI
#define KMX_LDB_MAITHILI  (1150)
#endif

#ifndef KMX_LDB_MANIPURI
#define KMX_LDB_MANIPURI  (1160)
#endif

#ifndef KMX_LDB_SANTALI
#define KMX_LDB_SANTALI  (1170)
#endif

#ifndef KMX_LDB_SOMALI
#define KMX_LDB_SOMALI  (1180)
#endif

#ifndef KMX_LDB_SINDHI
#define KMX_LDB_SINDHI  (1190)
#endif

#ifndef KMX_LDB_NORWEGIAN
#define KMX_LDB_NORWEGIAN  (480)
#endif

#ifndef KMX_LDB_ORIYA
#define KMX_LDB_ORIYA  (1)
#endif

#ifndef KMX_LDB_OROMO
#define KMX_LDB_OROMO  (661)
#endif

#ifndef KMX_LDB_PERSIAN
#define KMX_LDB_PERSIAN  (460)
#endif

#ifndef KMX_LDB_POLISH
#define KMX_LDB_POLISH  (391)
#endif

#ifndef KMX_LDB_PORTUGUESE
#define KMX_LDB_PORTUGUESE  (600)
#endif

#ifndef KMX_LDB_PUNJABI
#define KMX_LDB_PUNJABI  (1)
#endif

#ifndef KMX_LDB_ROMANIAN
#define KMX_LDB_ROMANIAN  (520)
#endif

#ifndef KMX_LDB_RUSSIAN
#define KMX_LDB_RUSSIAN  (202)
#endif

#ifndef KMX_LDB_SERBIAN
#define KMX_LDB_SERBIAN  (233)
#endif

#ifndef KMX_LDB_SERBIANC
#define KMX_LDB_SERBIANC  (243)
#endif

#ifndef KMX_LDB_SLOVAK
#define KMX_LDB_SLOVAK  (470)
#endif

#ifndef KMX_LDB_SLOVENIAN
#define KMX_LDB_SLOVENIAN  (530)
#endif

#ifndef KMX_LDB_SPANISH
#define KMX_LDB_SPANISH  (500)
#endif

#ifndef KMX_LDB_SWAHILI
#define KMX_LDB_SWAHILI  (550)
#endif

#ifndef KMX_LDB_SWEDISH
#define KMX_LDB_SWEDISH  (490)
#endif

#ifndef KMX_LDB_TAGALOG
#define KMX_LDB_TAGALOG  (587)
#endif

#ifndef KMX_LDB_TAMIL
#define KMX_LDB_TAMIL  (1)
#endif

#ifndef KMX_LDB_TELUGU
#define KMX_LDB_TELUGU  (1)
#endif

#ifndef KMX_LDB_THAI
#define KMX_LDB_THAI  (360)
#endif

#ifndef KMX_LDB_TURKISH
#define KMX_LDB_TURKISH  (330)
#endif

#ifndef KMX_LDB_UKRAINIAN
#define KMX_LDB_UKRAINIAN  (540)
#endif

#ifndef KMX_LDB_URDU
#define KMX_LDB_URDU  (950)
#endif

#ifndef KMX_LDB_UYGHUR
#define KMX_LDB_UYGHUR  (940)
#endif

#ifndef KMX_LDB_VIETNAMESE
#define KMX_LDB_VIETNAMESE  (310)
#endif



#ifndef KMX_LDB_TIBETAN
#define KMX_LDB_TIBETAN  (730)
#endif
#ifndef KMX_LDB_CZECH
#define KMX_LDB_CZECH  (442)
#endif


#ifndef KMX_LDB_ZULU
#define KMX_LDB_ZULU  (730)
#endif

#ifndef KMX_LDB_KAZAKH
#define	KMX_LDB_KAZAKH	(257)
#endif
#ifndef KMX_LDB_UZBEK
#define	KMX_LDB_UZBEK	(267)
#endif

#ifndef KMX_LDB_GEORGIAN
#define	KMX_LDB_GEORGIAN	(277)
#endif

#endif



//以下配置，以下的宏限定使用的ldb数据，与打开IIME_ALL_IN_ONE_DATABASE导入所有的ldb是一样的
#else//IIME_ALL_IN_ONE_DATABASE

#if 1//IIME_ALL_IN_ONE_DATABASE

#ifndef KMX_LDB_CATALAN
#define KMX_LDB_CATALAN  (621)
#endif

#ifndef KMX_LDB_ALBANIAN
#define KMX_LDB_ALBANIAN  (611)
#endif

#ifndef KMX_LDB_AMHARIC
#define KMX_LDB_AMHARIC  (1)
#endif

#ifndef KMX_LDB_ARABIC
#define KMX_LDB_ARABIC  (800)
#endif

#ifndef KMX_LDB_ARMENIAN
#define KMX_LDB_ARMENIAN  (570)
#endif

#ifndef KMX_LDB_ASSAMESE
#define KMX_LDB_ASSAMESE  (710)
#endif

#ifndef KMX_LDB_BASQUE
#define KMX_LDB_BASQUE  (631)
#endif

#ifndef KMX_LDB_BENGALI
#define KMX_LDB_BENGALI  (9201)
#endif

#ifndef KMX_LDB_BULGARIAN
#define KMX_LDB_BULGARIAN  (430)
#endif

#ifndef KMX_LDB_CROATIAN
#define KMX_LDB_CROATIAN  (510)
#endif


#ifndef KMX_LDB_CEBUANO
#define KMX_LDB_CEBUANO  (687)
#endif



#ifndef KMX_LDB_DANISH
#define KMX_LDB_DANISH  (370)
#endif

#ifndef KMX_LDB_DUTCH
#define KMX_LDB_DUTCH  (380)
#endif

#ifndef KMX_LDB_ENGLISH
#define KMX_LDB_ENGLISH  (109)
#endif

#ifndef KMX_LDB_FINNISH
#define KMX_LDB_FINNISH  (450)
#endif

#ifndef KMX_LDB_FRENCH
#define KMX_LDB_FRENCH  (400)
#endif

#ifndef KMX_LDB_GALICIAN
#define KMX_LDB_GALICIAN  (641)
#endif

#ifndef KMX_LDB_GERMAN
#define KMX_LDB_GERMAN  (300)
#endif

#ifndef KMX_LDB_GREEK
#define KMX_LDB_GREEK  (350)
#endif

#ifndef KMX_LDB_GUJARATI
#define KMX_LDB_GUJARATI  (1)
#endif

#ifndef KMX_LDB_HANGUL
#define KMX_LDB_HANGUL  (1)
#endif

#ifndef KMX_LDB_HEBREW
#define KMX_LDB_HEBREW  (420)
#endif

#ifndef KMX_LDB_HINDI
#define KMX_LDB_HINDI  (900)
#endif

#ifndef KMX_LDB_HINGLISH
#define KMX_LDB_HINGLISH  (910)
#endif

#ifndef KMX_LDB_HUNGARIAN
#define KMX_LDB_HUNGARIAN  (410)
#endif

#ifndef KMX_LDB_IGBO
#define KMX_LDB_IGBO  (651)
#endif

#ifndef KMX_LDB_INDONESIAN
#define KMX_LDB_INDONESIAN  (320)
#endif

#ifndef KMX_LDB_ITALIAN
#define KMX_LDB_ITALIAN  (700)
#endif

#ifndef KMX_LDB_KANNADA
#define KMX_LDB_KANNADA  (1)
#endif

#ifndef KMX_LDB_KHMER
#define KMX_LDB_KHMER  (1)
#endif

#ifndef KMX_LDB_KYRGYZ
#define KMX_LDB_KYRGYZ  (747)
#endif

#ifndef KMX_LDB_LAO
#define KMX_LDB_LAO  (1)
#endif

#ifndef KMX_LDB_LITHUANIAN
#define KMX_LDB_LITHUANIAN  (560)
#endif

#ifndef KMX_LDB_MALAY
#define KMX_LDB_MALAY  (340)
#endif

#ifndef KMX_LDB_MALAYALAM
#define KMX_LDB_MALAYALAM  (1)
#endif

#ifndef KMX_LDB_MARATHI
#define KMX_LDB_MARATHI  (1)
#endif

#ifndef KMX_LDB_MYANMAR
#define KMX_LDB_MYANMAR  (1)
#endif

#ifndef KMX_LDB_NEPALI
#define KMX_LDB_NEPALI  (720)
#endif

#ifndef KMX_LDB_SINHALA
#define KMX_LDB_SINHALA  (770)
#endif

//daodejing
#ifndef KMX_LDB_BODO
#define KMX_LDB_BODO  (1100)
#endif

#ifndef KMX_LDB_DOGRI
#define KMX_LDB_DOGRI  (1110)
#endif

#ifndef KMX_LDB_SANSKRIT
#define KMX_LDB_SANSKRIT  (1120)
#endif

#ifndef KMX_LDB_KASHMIRI
#define KMX_LDB_KASHMIRI  (1130)
#endif

#ifndef KMX_LDB_KONKANI
#define KMX_LDB_KONKANI  (1140)
#endif

#ifndef KMX_LDB_MAITHILI
#define KMX_LDB_MAITHILI  (1150)
#endif

#ifndef KMX_LDB_MANIPURI
#define KMX_LDB_MANIPURI  (1160)
#endif

#ifndef KMX_LDB_SANTALI
#define KMX_LDB_SANTALI  (1170)
#endif

#ifndef KMX_LDB_SOMALI
#define KMX_LDB_SOMALI  (1180)
#endif

#ifndef KMX_LDB_SINDHI
#define KMX_LDB_SINDHI  (1190)
#endif

#ifndef KMX_LDB_NORWEGIAN
#define KMX_LDB_NORWEGIAN  (480)
#endif

#ifndef KMX_LDB_ORIYA
#define KMX_LDB_ORIYA  (1)
#endif

#ifndef KMX_LDB_OROMO
#define KMX_LDB_OROMO  (661)
#endif

#ifndef KMX_LDB_PERSIAN
#define KMX_LDB_PERSIAN  (460)
#endif

#ifndef KMX_LDB_POLISH
#define KMX_LDB_POLISH  (391)
#endif

#ifndef KMX_LDB_PORTUGUESE
#define KMX_LDB_PORTUGUESE  (600)
#endif

#ifndef KMX_LDB_PUNJABI
#define KMX_LDB_PUNJABI  (1)
#endif

#ifndef KMX_LDB_ROMANIAN
#define KMX_LDB_ROMANIAN  (520)
#endif

#ifndef KMX_LDB_RUSSIAN
#define KMX_LDB_RUSSIAN  (202)
#endif

#ifndef KMX_LDB_SERBIAN
#define KMX_LDB_SERBIAN  (233)
#endif

#ifndef KMX_LDB_SERBIANC
#define KMX_LDB_SERBIANC  (243)
#endif

#ifndef KMX_LDB_SLOVAK
#define KMX_LDB_SLOVAK  (470)
#endif

#ifndef KMX_LDB_SLOVENIAN
#define KMX_LDB_SLOVENIAN  (530)
#endif

#ifndef KMX_LDB_SPANISH
#define KMX_LDB_SPANISH  (500)
#endif

#ifndef KMX_LDB_SWAHILI
#define KMX_LDB_SWAHILI  (550)
#endif

#ifndef KMX_LDB_SWEDISH
#define KMX_LDB_SWEDISH  (490)
#endif

#ifndef KMX_LDB_TAGALOG
#define KMX_LDB_TAGALOG  (587)
#endif

#ifndef KMX_LDB_TAMIL
#define KMX_LDB_TAMIL  (1)
#endif

#ifndef KMX_LDB_TELUGU
#define KMX_LDB_TELUGU  (1)
#endif

#ifndef KMX_LDB_THAI
#define KMX_LDB_THAI  (360)
#endif

#ifndef KMX_LDB_TURKISH
#define KMX_LDB_TURKISH  (330)
#endif

#ifndef KMX_LDB_UKRAINIAN
#define KMX_LDB_UKRAINIAN  (540)
#endif

#ifndef KMX_LDB_URDU
#define KMX_LDB_URDU  (950)
#endif

#ifndef KMX_LDB_UYGHUR
#define KMX_LDB_UYGHUR  (940)
#endif

#ifndef KMX_LDB_VIETNAMESE
#define KMX_LDB_VIETNAMESE  (310)
#endif



#ifndef KMX_LDB_TIBETAN
#define KMX_LDB_TIBETAN  (730)
#endif
#ifndef KMX_LDB_CZECH
#define KMX_LDB_CZECH  (442)
#endif


#ifndef KMX_LDB_ZULU
#define KMX_LDB_ZULU  (730)
#endif

#ifndef KMX_LDB_KAZAKH
#define	KMX_LDB_KAZAKH	(257)
#endif
#ifndef KMX_LDB_UZBEK
#define	KMX_LDB_UZBEK	(267)
#endif

#ifndef KMX_LDB_GEORGIAN
#define	KMX_LDB_GEORGIAN	(277)
#endif

#endif

#endif//IIME_ALL_IN_ONE_DATABASE
#endif//寸草不生
#endif/*SPREADTRUM_PRODUCT*/



#ifdef __cplusplus 
extern "C"{ 
#endif 

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Basic type defines                                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define IIME_VOID     void
#define IIME_PVOID    void*
#define IIME_PCVOID   const void*

typedef signed char             IIME_INT8;
typedef unsigned char           IIME_BYTE;
typedef signed short            IIME_SHORT;
typedef unsigned short          IIME_WORD;
typedef signed  int             IIME_INT;
typedef unsigned int            IIME_UINT;
typedef signed long             IIME_LONG;
typedef unsigned long           IIME_DWORD;
typedef float                   IIME_FLOAT32;
typedef double                  IIME_DOUBLE;

typedef unsigned char           IIME_CHAR;
typedef unsigned char*          IIME_PSTR;
typedef const unsigned char*    IIME_PCSTR;

typedef unsigned short          IIME_WCHAR;
typedef unsigned short*         IIME_PWSTR;
typedef const unsigned short*   IIME_PCWSTR;

typedef int                     IIME_BOOL;

typedef unsigned long           IIME_RESULT;
typedef void *                  IIME_HANDLE;
typedef IIME_HANDLE               IIME_HIMM;
typedef IIME_HANDLE               IIME_HCORE;

typedef unsigned short          IIME_POINTERID;

#define IIME_TRUE                 1
#define IIME_FALSE                0

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Virtual key defines                                                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define IIME_VK_NONE          0x0

#define IIME_VK_1             0x31
#define IIME_VK_2             0x32
#define IIME_VK_3             0x33
#define IIME_VK_4             0x34
#define IIME_VK_5             0x35
#define IIME_VK_6             0x36
#define IIME_VK_7             0x37
#define IIME_VK_8             0x38
#define IIME_VK_9             0x39
#define IIME_VK_L1            0x3A
#define IIME_VK_L2            0x3B
#define IIME_VK_L3            0x3C
#define IIME_VK_R1            0x3D
#define IIME_VK_R2            0x3E
#define IIME_VK_R3            0x3F

#define IIME_VK_A             0x61
#define IIME_VK_BACKSPACE     0x08
#define IIME_VK_LEFT          0x25
#define IIME_VK_UP            0x26
#define IIME_VK_RIGHT         0x27
#define IIME_VK_DOWN          0x28
#define IIME_VK_SELECT        0x29

/* Symbian    在拼音/笔画中作为IIME_VK_SPACE使用, 其他情况为IIME_VK_0 */
/* Smartphone 在拼音输入法中作为IIME_VK_AUX使用, 其他情况下为IIME_VK_0 */
#define IIME_VK_0             0x30

/* Symbian    在拼音输入法中作为IIME_VK_AUX使用, 其他情况下为IIME_VK_START */
/* Smartphone 作为IIME_VK_IME使用 */
#define IIME_VK_STAR          0x2a

/* Symbian    作为IIME_VK_IME使用 */
/* Smartphone 作为IIME_VK_SPACE使用 */
#define IIME_VK_POUND         0x23

/* 功能键, 没有与键盘直接对应, 如果输入法管理器收到以下键可以直接交给Provider使用 */
#define IIME_VK_SPACE         0x20  /* 空格\切分*/
#define IIME_VK_AUX           0x41  /* 显示AUX(目前拼音专用)*/
#define IIME_VK_IME           0x40  /* 切换输入法*/

#define IIME_VK_PLUS          0x42

#define IIME_VK_LONGCLICK     0x00010000      /* 表示长按键 */
#define IIME_VK_REPEAT        0x00020000      /* 表示长按键 */

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Error code defines                                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define IIME_ERROR_SUCCESS                    0x00000000
#define IIME_ERROR_UNKNOWN                    0x00000001

#define IIME_ERROR_USED                       0x00000700
#define IIME_ERROR_UNUSED                     0x00000701
#define IIME_ERROR_COMMIT						0x00000702
#define IIME_ERROR_TIPS                       0x00000800

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Input method defines                                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/*  language id */
#define CHS_CHARSET     936
#define CHT_CHARSET     950
#define ENG_CHARSET     1252

/* Language identifiers */

#define IIME_LANG_CHS             1
#define IIME_LANG_CHT             2
#define IIME_LANG_ENGLISH         10
#define IIME_LANG_RUSSIAN         20
#define IIME_LANG_GERMAN          30
#define IIME_LANG_VIETNAMESE      31
#define IIME_LANG_INDONESIAN      32
#define IIME_LANG_TURKISH         33
#define IIME_LANG_MALAY           34
#define IIME_LANG_GREEK           35
#define IIME_LANG_THAI            36
#define IIME_LANG_DANISH          37
#define IIME_LANG_DUTCH           38
#define IIME_LANG_POLISH          39
#define IIME_LANG_FRENCH          40
#define IIME_LANG_HUNGARIAN       41
#define IIME_LANG_HEBREW          42
#define IIME_LANG_BULGARIAN       43
#define IIME_LANG_CZECH           44
#define IIME_LANG_FINNISH         45
#define IIME_LANG_PERSIAN         46
#define IIME_LANG_SLOVAK          47
#define IIME_LANG_NORWEGIAN       48
#define IIME_LANG_SWEDISH         49
#define IIME_LANG_SPANISH         50
#define IIME_LANG_CROATIAN        51
#define IIME_LANG_ROMANIAN        52
#define IIME_LANG_SLOVENIAN       53
#define IIME_LANG_UKRAINIAN       54
#define IIME_LANG_SWAHILI         55
#define IIME_LANG_LITHUANIAN      56
#define IIME_LANG_ARMENIAN        57
#define IIME_LANG_TAGALOG         58
#define IIME_LANG_PORTUGUESE      60
#define IIME_LANG_ITALIAN         70
#define IIME_LANG_ARABIC          80
#define IIME_LANG_HINDI           90
#define IIME_LANG_HINGLISH        91
#define IIME_LANG_BENGALI         92
#define IIME_LANG_HANGUL          93
#define IIME_LANG_UYGHUR          94
#define IIME_LANG_URDU            95
#define IIME_LANG_TELUGU          96
#define IIME_LANG_MARATHI         97
#define IIME_LANG_TAMIL           98
#define IIME_LANG_MYANMAR         99

#define IIME_LANG_PUNJABI         81
#define IIME_LANG_GUJARATI        82
#define IIME_LANG_ORIYA           83
#define IIME_LANG_KANNADA         84
#define IIME_LANG_MALAYALAM       85

#define IIME_LANG_KHMER           86
#define IIME_LANG_LAO             87

#define IIME_LANG_AMHARIC         88
#define IIME_LANG_ASSAMESE        71
#define IIME_LANG_NEPALI          72
#define IIME_LANG_SINHALA         77
#define IIME_LANG_BODO            110//daodejing
#define IIME_LANG_DOGRI           111
#define IIME_LANG_SANSKRIT        112
#define IIME_LANG_KASHMIRI        113
#define IIME_LANG_KONKANI         114
#define IIME_LANG_MAITHILI        115
#define IIME_LANG_MANIPURI        116
#define IIME_LANG_SANTALI         117
#define IIME_LANG_SOMALI          118
#define IIME_LANG_SINDHI          119
#define IIME_LANG_TIBETAN         73
#define IIME_LANG_KYRGYZ          74

#define IIME_LANG_TIGRE           75
#define IIME_LANG_TIGRINYA        76

#define IIME_LANG_ALBANIAN        61
#define IIME_LANG_CATALAN         62
#define IIME_LANG_BASQUE          63
#define IIME_LANG_GALICIAN        64
#define IIME_LANG_IGBO            65
#define IIME_LANG_OROMO           66
#define IIME_LANG_ZULU            67
#define IIME_ALNG_CEBUANO         68

#define IIME_LANG_SERBIAN         23
#define IIME_LANG_SERBIANC        24
#define IIME_LANG_KAZAKH          25
#define IIME_LANG_UZBEK           26
#define IIME_LANG_GEORGIAN        27


/* Input method identifiers */

#define IM_SURETYPE_BASE        10000
#define IM_USER_MULTITYPE_BASE  20000

#define IM_LATINFULLKEY_BASE	0x10000
#define IM_FULLKEY_BASE         0x10000

#define IM_NONE                 0
#define IM_PINYIN               1
#define IM_STROKE               2
#define IM_MAGIC_ONE            3
#define IM_STROKE_BIG5          4
#define IM_PINYIN_DZ            5
#define IM_123                  7
#define IM_ZHUYIN_MT            8
#define IM_ZHUYIN               9
#define IM_PINYIN_SURETYPE      (IM_PINYIN + IM_SURETYPE_BASE)
#define IM_PINYIN_FULLKEY       11
#define IM_ZHUYIN_FULLKEY       12

#define IM_ZHUYIN_MT_FULL       13
#define IM_ZHUYIN_2             14      /* for second zhuyin qwerty keyboard definition */
                                        /* there is not IM_ZHUYIN_2, must use (IM_ZHUYIN_2 + IM_FULLKEY_BASE) */

#define IM_NUMBER               22

#define IM_PINYIN_TC            30
#define IM_PINYIN_FULLKEY_TC    31
#define IM_ZHUYIN_MT_SC         32
#define IM_ZHUYIN_FULLKEY_SC    33

#define IM_ENGLISH              101
#define IM_ABC_LOWER            102
#define IM_ABC_UPPER            103
#define IM_ABC_LEAD_UPPER       104
#define IM_ABC                  105
#define IM_ENGLISH_LEAD_UPPER	107
#define IM_ENGLISH_UPPER        108
#define IM_ENGLISH_LOWER        109

#define IM_RUSSIAN              201
#define IM_ABC_LOWER_RUS        202
#define IM_ABC_UPPER_RUS        203
#define IM_ABC_LEAD_UPPER_RUS   204
#define IM_ABC_RUS              205
#define IM_RUSSIAN_LEAD_UPPER   207
#define IM_RUSSIAN_UPPER        208
#define IM_RUSSIAN_LOWER        209

#define IM_GERMAN               301
#define IM_ABC_LOWER_DE         302
#define IM_ABC_UPPER_DE         303
#define IM_ABC_LEAD_UPPER_DE    304
#define IM_ABC_DE               305
#define IM_GERMAN_LEAD_UPPER    307
#define IM_GERMAN_UPPER         308
#define IM_GERMAN_LOWER         309

#define IM_VIETNAMESE           311
#define IM_ABC_LOWER_VI         312
#define IM_ABC_UPPER_VI         313
#define IM_ABC_LEAD_UPPER_VI    314
#define IM_ABC_VI               315
#define IM_VIETNAMESE_LEAD_UPPER    317
#define IM_VIETNAMESE_UPPER     318
#define IM_VIETNAMESE_LOWER     319

#define IM_INDONESIAN           321
#define IM_ABC_LOWER_ID         322
#define IM_ABC_UPPER_ID         323
#define IM_ABC_LEAD_UPPER_ID    324
#define IM_ABC_ID               325
#define IM_INDONESIAN_LEAD_UPPER     327
#define IM_INDONESIAN_UPPER     328
#define IM_INDONESIAN_LOWER     329

#define IM_TURKISH              331
#define IM_ABC_LOWER_TR         332
#define IM_ABC_UPPER_TR         333
#define IM_ABC_LEAD_UPPER_TR    334
#define IM_ABC_TR               335
#define IM_TURKISH_LEAD_UPPER        337
#define IM_TURKISH_UPPER        338
#define IM_TURKISH_LOWER        339

#define IM_MALAY                341
#define IM_ABC_LOWER_MS         342
#define IM_ABC_UPPER_MS         343
#define IM_ABC_LEAD_UPPER_MS    344
#define IM_ABC_MS               345
#define IM_MALAY_LEAD_UPPER          347
#define IM_MALAY_UPPER          348
#define IM_MALAY_LOWER          349

#define IM_GREEK                351
#define IM_ABC_LOWER_EL         352
#define IM_ABC_UPPER_EL         353
#define IM_ABC_LEAD_UPPER_EL    354
#define IM_ABC_EL               355
#define IM_GREEK_LEAD_UPPER          357
#define IM_GREEK_UPPER          358
#define IM_GREEK_LOWER          359

#define IM_THAI                 361
#define IM_THAI_MT              363
#define IM_THAI_123             364

#define IM_DANISH               371
#define IM_ABC_LOWER_DA         372
#define IM_ABC_UPPER_DA         373
#define IM_ABC_LEAD_UPPER_DA    374
#define IM_ABC_DA               375
#define IM_DANISH_LEAD_UPPER         377
#define IM_DANISH_UPPER         378
#define IM_DANISH_LOWER         379

#define IM_DUTCH                381
#define IM_ABC_LOWER_NL         382
#define IM_ABC_UPPER_NL         383
#define IM_ABC_LEAD_UPPER_NL    384
#define IM_ABC_NL               385
#define IM_DUTCH_LEAD_UPPER          387
#define IM_DUTCH_UPPER          388
#define IM_DUTCH_LOWER          389

#define IM_POLISH               391
#define IM_ABC_LOWER_PL         392
#define IM_ABC_UPPER_PL         393
#define IM_ABC_LEAD_UPPER_PL    394
#define IM_ABC_PL               395
#define IM_POLISH_LEAD_UPPER         397
#define IM_POLISH_UPPER         398
#define IM_POLISH_LOWER         399

#define IM_FRENCH               401
#define IM_ABC_LOWER_FR         402
#define IM_ABC_UPPER_FR         403
#define IM_ABC_LEAD_UPPER_FR    404
#define IM_ABC_FR               405
#define IM_FRENCH_LEAD_UPPER         407
#define IM_FRENCH_UPPER         408
#define IM_FRENCH_LOWER         409

#define IM_HUNGARIAN            411
#define IM_ABC_LOWER_HU         412
#define IM_ABC_UPPER_HU         413
#define IM_ABC_LEAD_UPPER_HU    414
#define IM_ABC_HU               415
#define IM_HUNGARIAN_LEAD_UPPER      417
#define IM_HUNGARIAN_UPPER      418
#define IM_HUNGARIAN_LOWER      419
 
#define IM_HEBREW               421
#define IM_HEBREW_MT            423

#define IM_BULGARIAN            431
#define IM_ABC_LOWER_BG         432
#define IM_ABC_UPPER_BG         433
#define IM_ABC_LEAD_UPPER_BG    434
#define IM_ABC_BG               435
#define IM_BULGARIAN_LEAD_UPPER      437
#define IM_BULGARIAN_UPPER      438
#define IM_BULGARIAN_LOWER      439

#define IM_CZECH                441
#define IM_ABC_LOWER_IIME         442
#define IM_ABC_UPPER_IIME         443
#define IM_ABC_LEAD_UPPER_IIME    444
#define IM_ABC_IIME               445
#define IM_CZECH_LEAD_UPPER          447
#define IM_CZECH_UPPER          448
#define IM_CZECH_LOWER          449

#define IM_FINNISH              451
#define IM_ABC_LOWER_FI         452
#define IM_ABC_UPPER_FI         453
#define IM_ABC_LEAD_UPPER_FI    454
#define IM_ABC_FI               455
#define IM_FINNISH_LEAD_UPPER        457
#define IM_FINNISH_UPPER        458
#define IM_FINNISH_LOWER        459

#define IM_PERSIAN              461
#define IM_PERSIAN_MT           463
#define IM_PERSIAN_123          464

#define IM_SLOVAK               471
#define IM_ABC_LOWER_SK         472
#define IM_ABC_UPPER_SK         473
#define IM_ABC_LEAD_UPPER_SK    474
#define IM_ABC_SK               475
#define IM_SLOVAK_LEAD_UPPER         477
#define IM_SLOVAK_UPPER         478
#define IM_SLOVAK_LOWER         479

#define IM_NORWEGIAN            481
#define IM_ABC_LOWER_NO         482
#define IM_ABC_UPPER_NO         483
#define IM_ABC_LEAD_UPPER_NO    484
#define IM_ABC_NO               485
#define IM_NORWEGIAN_LEAD_UPPER      487
#define IM_NORWEGIAN_UPPER      488
#define IM_NORWEGIAN_LOWER      489

#define IM_SWEDISH              491
#define IM_ABC_LOWER_SV         492
#define IM_ABC_UPPER_SV         493
#define IM_ABC_LEAD_UPPER_SV    494
#define IM_ABC_SV               495
#define IM_SWEDISH_LEAD_UPPER        497
#define IM_SWEDISH_UPPER        498
#define IM_SWEDISH_LOWER        499

#define IM_SPANISH              501
#define IM_ABC_LOWER_ES         502
#define IM_ABC_UPPER_ES         503
#define IM_ABC_LEAD_UPPER_ES    504
#define IM_ABC_ES               505
#define IM_SPANISH_LEAD_UPPER        507
#define IM_SPANISH_UPPER        508
#define IM_SPANISH_LOWER        509

#define IM_CROATIAN             511
#define IM_ABC_LOWER_HR         512
#define IM_ABC_UPPER_HR         513
#define IM_ABC_LEAD_UPPER_HR    514
#define IM_ABC_HR               515
#define IM_CROATIAN_LEAD_UPPER       517
#define IM_CROATIAN_UPPER       518
#define IM_CROATIAN_LOWER       519

#define IM_ROMANIAN             521
#define IM_ABC_LOWER_RO         522
#define IM_ABC_UPPER_RO         523
#define IM_ABC_LEAD_UPPER_RO    524
#define IM_ABC_RO               525
#define IM_ROMANIAN_LEAD_UPPER       527
#define IM_ROMANIAN_UPPER       528
#define IM_ROMANIAN_LOWER       529

#define IM_SLOVENIAN            531
#define IM_ABC_LOWER_SL         532
#define IM_ABC_UPPER_SL         533
#define IM_ABC_LEAD_UPPER_SL    534
#define IM_ABC_SL               535
#define IM_SLOVENIAN_LEAD_UPPER      537
#define IM_SLOVENIAN_UPPER      538
#define IM_SLOVENIAN_LOWER      539

#define IM_UKRAINIAN            541
#define IM_ABC_LOWER_UK         542
#define IM_ABC_UPPER_UK         543
#define IM_ABC_LEAD_UPPER_UK    544
#define IM_ABC_UK               545
#define IM_UKRAINIAN_LEAD_UPPER      547
#define IM_UKRAINIAN_UPPER      548
#define IM_UKRAINIAN_LOWER      549

#define IM_SWAHILI              551
#define IM_ABC_LOWER_SW         552
#define IM_ABC_UPPER_SW         553
#define IM_ABC_LEAD_UPPER_SW    554
#define IM_ABC_SW               555
#define IM_SWAHILI_LEAD_UPPER        557
#define IM_SWAHILI_UPPER        558
#define IM_SWAHILI_LOWER        559
           
#define IM_LITHUANIAN           561
#define IM_ABC_LOWER_LT         562
#define IM_ABC_UPPER_LT         563
#define IM_ABC_LEAD_UPPER_LT    564
#define IM_ABC_LT               565
#define IM_LITHUANIAN_LEAD_UPPER     567
#define IM_LITHUANIAN_UPPER     568
#define IM_LITHUANIAN_LOWER     569

#define IM_ARMENIAN             571
#define IM_ABC_LOWER_HY         572
#define IM_ABC_UPPER_HY         573
#define IM_ABC_LEAD_UPPER_HY    574
#define IM_ABC_HY               575
#define IM_ARMENIAN_LEAD_UPPER       577
#define IM_ARMENIAN_UPPER       578
#define IM_ARMENIAN_LOWER       579

/* tagalog only has multitap, IM_TAGALOG can't used now!!! */
#define IM_TAGALOG              581
#define IM_ABC_LOWER_TL         582
#define IM_ABC_UPPER_TL         583
#define IM_ABC_LEAD_UPPER_TL    584
#define IM_ABC_TL               585
#define IM_TAGALOG_LEAD_UPPER       587
#define IM_TAGALOG_UPPER       588
#define IM_TAGALOG_LOWER       589

#define IM_PORTUGUESE           601
#define IM_ABC_LOWER_PT         602
#define IM_ABC_UPPER_PT         603
#define IM_ABC_LEAD_UPPER_PT    604
#define IM_ABC_PT               605
#define IM_PORTUGUESE_LEAD_UPPER     607
#define IM_PORTUGUESE_UPPER     608
#define IM_PORTUGUESE_LOWER     609

#define IM_ITALIAN              701
#define IM_ABC_LOWER_IT         702
#define IM_ABC_UPPER_IT         703
#define IM_ABC_LEAD_UPPER_IT    704
#define IM_ABC_IT               705
#define IM_ITALIAN_LEAD_UPPER        707
#define IM_ITALIAN_UPPER        708
#define IM_ITALIAN_LOWER        709

#define IM_ARABIC               801
#define IM_ARABIC_MT            803
#define IM_ARABIC_123           804

#define IM_HINDI                901
#define IM_HINDI_MT             903
#define IM_HINDI_123            904

#define IM_HINGLISH             911
#define IM_ABC_LOWER_HIN        912
#define IM_ABC_UPPER_HIN        913
#define IM_ABC_LEAD_UPPER_HIN   914
#define IM_ABC_HIN              915
#define IM_HINGLISH_LEAD_UPPER       917
#define IM_HINGLISH_UPPER       918
#define IM_HINGLISH_LOWER       919

#define IM_BENGALI              921
#define IM_BENGALI_MT           923
#define IM_BENGALI_123          924

#define IM_HANGUL               931
#define IM_HANGUL_MT            933

#define IM_UYGHUR               941
#define IM_UYGHUR_MT            943

#define IM_URDU                 951
#define IM_URDU_MT              953

#define IM_TELUGU               961
#define IM_TELUGU_MT            963

#define IM_MARATHI              971
#define IM_MARATHI_MT           973

#define IM_TAMIL                981
#define IM_TAMIL_MT             983

#define IM_MYANMAR              991
#define IM_MYANMAR_MT           993

#define IM_PUNJABI              811
#define IM_PUNJABI_MT           813

#define IM_GUJARATI             821
#define IM_GUJARATI_MT          823

#define IM_ORIYA                831
#define IM_ORIYA_MT             833

#define IM_KANNADA              841
#define IM_KANNADA_MT           843

#define IM_MALAYALAM            851
#define IM_MALAYALAM_MT         853

#define IM_KHMER                861
#define IM_KHMER_MT             863
#define IM_KHMER_123            864

#define IM_LAO                  871
#define IM_LAO_MT               873

#define IM_AMHARIC              881
#define IM_AMHARIC_MT           883

#define IM_ASSAMESE             711
#define IM_ASSAMESE_MT          713
#define IM_ASSAMESE_123         714

#define IM_NEPALI               721
#define IM_NEPALI_MT            723
#define IM_NEPALI_123           724

//daodejing
#define IM_BODO               1101
#define IM_BODO_MT            1103
#define IM_BODO_123           1104

#define IM_DOGRI               1111
#define IM_DOGRI_MT            1113
#define IM_DOGRI_123           1114

#define IM_SANSKRIT            1121
#define IM_SANSKRIT_MT         1123
#define IM_SANSKRIT_123        1124

#define IM_KASHMIRI            1131
#define IM_KASHMIRI_MT         1133
#define IM_KASHMIRI_123        1134

#define IM_KONKANI             1141
#define IM_KONKANI_MT          1143
#define IM_KONKANI_123         1144

#define IM_MAITHILI            1151
#define IM_MAITHILI_MT         1153
#define IM_MAITHILI_123        1154

#define IM_MANIPURI            1161
#define IM_MANIPURI_MT         1163
#define IM_MANIPURI_123        1164

#define IM_SANTALI             1171
#define IM_SANTALI_MT          1173
#define IM_SANTALI_123         1174

#define IM_SOMALI              1181
#define IM_SOMALI_MT           1183
#define IM_SOMALI_123          1184

#define IM_SINDHI              1191
#define IM_SINDHI_MT           1193
#define IM_SINDHI_123          1194

#define IM_TIBETAN              731
#define IM_TIBETAN_MT           733
#define IM_TIBETAN_123          734

/* Kyrgyz only has multitap, IM_KYRGYZ can't used now!!! */
#define IM_KYRGYZ               741
#define IM_ABC_LOWER_KY         742
#define IM_ABC_UPPER_KY         743
#define IM_ABC_LEAD_UPPER_KY    744
#define IM_ABC_KY               745
#define IM_KYRGYZ_LEAD_UPPER    747
#define IM_KYRGYZ_UPPER         748
#define IM_KYRGYZ_LOWER         749

/* Albanian only has multitap, IM_ALBANIAN can't used now!!! */
#define IM_ALBANIAN             611
#define IM_ABC_LOWER_SQ         612
#define IM_ABC_UPPER_SQ         613
#define IM_ABC_LEAD_UPPER_SQ    614
#define IM_ABC_SQ               615
#define IM_ALBANIAN_LEAD_UPPER  617
#define IM_ALBANIAN_UPPER       618
#define IM_ALBANIAN_LOWER       619

/* Catalan only has multitap, IM_CATALAN can't used now!!! */
#define IM_CATALAN              621
#define IM_ABC_LOWER_CA         622
#define IM_ABC_UPPER_CA         623
#define IM_ABC_LEAD_UPPER_CA    624
#define IM_ABC_CA               625
#define IM_CATALAN_LEAD_UPPER   627
#define IM_CATALAN_UPPER        628
#define IM_CATALAN_LOWER        629

/* Basque only has multitap, IM_BASQUE can't used now!!! */
#define IM_BASQUE               631
#define IM_ABC_LOWER_EU         632
#define IM_ABC_UPPER_EU         633
#define IM_ABC_LEAD_UPPER_EU    634
#define IM_ABC_EU               635
#define IM_BASQUE_LEAD_UPPER    637
#define IM_BASQUE_UPPER         638
#define IM_BASQUE_LOWER         639

/* Galician only has multitap, IM_GALICIAN can't used now!!! */
#define IM_GALICIAN             641
#define IM_ABC_LOWER_GL         642
#define IM_ABC_UPPER_GL         643
#define IM_ABC_LEAD_UPPER_GL    644
#define IM_ABC_GL               645
#define IM_GALICIAN_LEAD_UPPER  647
#define IM_GALICIAN_UPPER       648
#define IM_GALICIAN_LOWER       649

/* Igbo only has multitap, IM_IGBO can't used now!!! */
#define IM_IGBO                 651
#define IM_ABC_LOWER_IG         652
#define IM_ABC_UPPER_IG         653
#define IM_ABC_LEAD_UPPER_IG    654
#define IM_ABC_IG               655
#define IM_IGBO_LEAD_UPPER      657
#define IM_IGBO_UPPER           658
#define IM_IGBO_LOWER           659

/* Oromo only has multitap, IM_OROMO can't used now!!! */
#define IM_OROMO                661
#define IM_ABC_LOWER_OM         662
#define IM_ABC_UPPER_OM         663
#define IM_ABC_LEAD_UPPER_OM    664
#define IM_ABC_OM               665
#define IM_OROMO_LEAD_UPPER     667
#define IM_OROMO_UPPER          668
#define IM_OROMO_LOWER          669

/* Serbian only has multitap, IM_SERBIAN can't used now!!! */
#define IM_SERBIAN             231
#define IM_ABC_LOWER_SR        232
#define IM_ABC_UPPER_SR        233
#define IM_ABC_LEAD_UPPER_SR   234
#define IM_ABC_SR              235
#define IM_SERBIAN_LEAD_UPPER  237
#define IM_SERBIAN_UPPER       238
#define IM_SERBIAN_LOWER       239

/* Serbian cyrillic only has multitap, IM_SERBIANC can't used now!!! */
#define IM_SERBIANC             241
#define IM_ABC_LOWER_SRC        242
#define IM_ABC_UPPER_SRC        243
#define IM_ABC_LEAD_UPPER_SRC   244
#define IM_ABC_SRC              245
#define IM_SERBIANC_LEAD_UPPER  247
#define IM_SERBIANC_UPPER       248
#define IM_SERBIANC_LOWER       249

/* Kazakh only has multitap, IM_KAZAKH can't used now!!! */
#define IM_KAZAKH               251
#define IM_ABC_LOWER_KK         252
#define IM_ABC_UPPER_KK         253
#define IM_ABC_LEAD_UPPER_KK    254
#define IM_ABC_KK               255
#define IM_KAZAKH_LEAD_UPPER    257
#define IM_KAZAKH_UPPER         258
#define IM_KAZAKH_LOWER         259

/* Uzbek only has multitap, IM_UZBEK can't used now!!! */
#define IM_UZBEK               261
#define IM_ABC_LOWER_UZ        262
#define IM_ABC_UPPER_UZ        263
#define IM_ABC_LEAD_UPPER_UZ   264
#define IM_ABC_UZ              265
#define IM_UZBEK_LEAD_UPPER    267
#define IM_UZBEK_UPPER         268
#define IM_UZBEK_LOWER         269

/* Georgian only has multitap, IM_GEORGIAN can't used now!!! */
#define IM_GEORGIAN             271
#define IM_ABC_LOWER_KA         272
#define IM_ABC_UPPER_KA         273
#define IM_ABC_LEAD_UPPER_KA    274
#define IM_ABC_KA               275
#define IM_GEORGIAN_LEAD_UPPER  277
#define IM_GEORGIAN_UPPER       278
#define IM_GEORGIAN_LOWER       279

/* Zulu only has multitap, IM_ZULU can't used now!!! */
#define IM_ZULU                 671
#define IM_ABC_LOWER_ZU         672
#define IM_ABC_UPPER_ZU         673
#define IM_ABC_LEAD_UPPER_ZU    674
#define IM_ABC_ZU               675
#define IM_ZULU_LEAD_UPPER      677
#define IM_ZULU_UPPER           678
#define IM_ZULU_LOWER           679

/* Cebuano only has multitap, IM_CEBUANO can't used now!!! */
#define IM_CEBUANO              681
#define IM_ABC_LOWER_CEB        682
#define IM_ABC_UPPER_CEB        683
#define IM_ABC_LEAD_UPPER_CEB   684
#define IM_ABC_CEB              685
#define IM_CEBUANO_LEAD_UPPER   687
#define IM_CEBUANO_UPPER        688
#define IM_CEBUANO_LOWER        689

#define IM_SINHALA               771
#define IM_SINHALA_MT            772
#define IM_SINHALA_123           773


#define IIME_BASE_IM(im_id)                   ((im_id) & 0xFFFF)
#define IIME_LANG_FROM_LATIN_IM(im_id)        ((im_id) / 10)
#define IIME_BASE_LATIN_IM_FROM_LANG(lang_id) ((lang_id) * 10 + 1)
#define IIME_IS_LATIN_IM(im_id)               ((im_id) >= IM_ENGLISH && (im_id) < IM_SURETYPE_BASE)
#define IIME_IM_TO_BASE_LATIN_IM(im_id)       (IIME_BASE_LATIN_IM_FROM_LANG(IIME_LANG_FROM_LATIN_IM(im_id)))

/* IM Identifiers */
#define IIME_TYPE_FROM_LATIN_IM(im_id)        ((im_id) % 10)
#define IIME_IS_LATIN_MULTITAP_TYPE(tp)       ((tp) >= 2 && (tp) <= 5)
#define IIME_IS_LATIN_SMART_TYPE(tp)          (((tp) == 1) || ((tp) >= 7 && (tp) <= 9))


/* client event type */
#define IIME_EVENT_KEYBOARD       0x0001
#define IIME_EVENT_TIMER          0x0002
#define IIME_EVENT_CHAR			0x0003
#define IIME_EVENT_CHAR_APPEND    0x0003
#define IIME_EVENT_CHAR_REPLACE   0x0004

/* event for full keyboard or touch panel */
#define	IIME_EVENT_APPENDCHAR		0x0010
#define IIME_EVENT_REMOVECHAR		0x0020
#define IIME_EVENT_SELECTCAND		0x0040
#define IIME_EVENT_REMOVEALL		0x0080
#define IIME_EVENT_PAGENEXT		0x0100
#define IIME_EVENT_PAGEPREVIOUS	0x0200

/* client window state */
#define IIME_FLAG_STATE_EDIT      1
#define IIME_FLAG_STATE_CAND      2
#define IIME_FLAG_STATE_USER      3
#define IIME_FLAG_STATE_ACTIVE    4

/* candidate and pychoice window PAGEUP/PAGEDOWN flags */
#define IIME_FLAG_PAGEUP          0x0001
#define IIME_FLAG_PAGEDOWN        0x0002

/* timer operator */
#define IIME_TIMER_SET            1
#define IIME_TIMER_CLEAR          2

/* Language database list */

typedef struct
{
    int             input_method;
    const void*     language_database;
} IIME_IME_INFO;

extern const IIME_IME_INFO IIME_ldb_pinyin_cizu;
extern const IIME_IME_INFO IIME_ldb_pinyin_suretype;
extern const IIME_IME_INFO IIME_ldb_pinyin_danzi;
extern const IIME_IME_INFO IIME_ldb_pinyin_tc;
extern const IIME_IME_INFO IIME_ldb_pinyin_tc_suretype;

extern const IIME_IME_INFO IIME_ldb_zhuyin;
extern const IIME_IME_INFO IIME_ldb_zhuyin_cizu;
extern const IIME_IME_INFO IIME_ldb_zhuyin_danzi;
extern const IIME_IME_INFO IIME_ldb_zhuyin_sc;

extern const IIME_IME_INFO IIME_ldb_stroke_chs;
extern const IIME_IME_INFO IIME_ldb_stroke_gb2312;
extern const IIME_IME_INFO IIME_ldb_stroke_gbk;
extern const IIME_IME_INFO IIME_ldb_stroke_gb18030;
extern const IIME_IME_INFO IIME_ldb_stroke_big5;
extern const IIME_IME_INFO IIME_ldb_stroke_big5hksIIME;

extern const IIME_IME_INFO IIME_ldb_english;

extern const IIME_IME_INFO IIME_ldb_albanian;
extern const IIME_IME_INFO IIME_ldb_armenian;
extern const IIME_IME_INFO IIME_ldb_arabic;
extern const IIME_IME_INFO IIME_ldb_basque;
extern const IIME_IME_INFO IIME_ldb_bengali;
extern const IIME_IME_INFO IIME_ldb_bulgarian;
extern const IIME_IME_INFO IIME_ldb_catalan;
extern const IIME_IME_INFO IIME_ldb_croatian;
extern const IIME_IME_INFO IIME_ldb_czech;
extern const IIME_IME_INFO IIME_ldb_danish;
extern const IIME_IME_INFO IIME_ldb_dutch;
extern const IIME_IME_INFO IIME_ldb_finnish;
extern const IIME_IME_INFO IIME_ldb_french;
extern const IIME_IME_INFO IIME_ldb_galician;
extern const IIME_IME_INFO IIME_ldb_german;
extern const IIME_IME_INFO IIME_ldb_greek;
extern const IIME_IME_INFO IIME_ldb_hebrew;
extern const IIME_IME_INFO IIME_ldb_hindi;
extern const IIME_IME_INFO IIME_ldb_hinglish;
extern const IIME_IME_INFO IIME_ldb_hungarian;
extern const IIME_IME_INFO IIME_ldb_igbo;
extern const IIME_IME_INFO IIME_ldb_indonesian;
extern const IIME_IME_INFO IIME_ldb_italian;
extern const IIME_IME_INFO IIME_ldb_lithuanian;
extern const IIME_IME_INFO IIME_ldb_malay;
extern const IIME_IME_INFO IIME_ldb_norwegian;
extern const IIME_IME_INFO IIME_ldb_oromo;
extern const IIME_IME_INFO IIME_ldb_persian;
extern const IIME_IME_INFO IIME_ldb_polish;
extern const IIME_IME_INFO IIME_ldb_portuguese;
extern const IIME_IME_INFO IIME_ldb_romanian;
extern const IIME_IME_INFO IIME_ldb_russian;
extern const IIME_IME_INFO IIME_ldb_slovak;
extern const IIME_IME_INFO IIME_ldb_slovenian;
extern const IIME_IME_INFO IIME_ldb_spanish;
extern const IIME_IME_INFO IIME_ldb_swahili;
extern const IIME_IME_INFO IIME_ldb_swedish;
extern const IIME_IME_INFO IIME_ldb_tagalog;
extern const IIME_IME_INFO IIME_ldb_kyrgyz;
extern const IIME_IME_INFO IIME_ldb_thai;
extern const IIME_IME_INFO IIME_ldb_turkish;
extern const IIME_IME_INFO IIME_ldb_ukrainian;
extern const IIME_IME_INFO IIME_ldb_vietnamese;
extern const IIME_IME_INFO IIME_ldb_hangul;
extern const IIME_IME_INFO IIME_ldb_uyghur;
extern const IIME_IME_INFO IIME_ldb_urdu;
extern const IIME_IME_INFO IIME_ldb_telugu;
extern const IIME_IME_INFO IIME_ldb_marathi;
extern const IIME_IME_INFO IIME_ldb_tamil;
extern const IIME_IME_INFO IIME_ldb_punjabi;
extern const IIME_IME_INFO IIME_ldb_gujarati;
extern const IIME_IME_INFO IIME_ldb_oriya;
extern const IIME_IME_INFO IIME_ldb_kannada;
extern const IIME_IME_INFO IIME_ldb_malayalam;
extern const IIME_IME_INFO IIME_ldb_myanmar;
extern const IIME_IME_INFO IIME_ldb_khmer;
extern const IIME_IME_INFO IIME_ldb_lao;
extern const IIME_IME_INFO IIME_ldb_amharic;
extern const IIME_IME_INFO IIME_ldb_nepali;
extern const IIME_IME_INFO IIME_ldb_sinhala;
extern const IIME_IME_INFO IIME_ldb_bodo;//daodejing
extern const IIME_IME_INFO IIME_ldb_dogri;
extern const IIME_IME_INFO IIME_ldb_sanskrit;
extern const IIME_IME_INFO IIME_ldb_kashmiri;
extern const IIME_IME_INFO IIME_ldb_konkani;
extern const IIME_IME_INFO IIME_ldb_maithili;
extern const IIME_IME_INFO IIME_ldb_manipuri;
extern const IIME_IME_INFO IIME_ldb_santali;
extern const IIME_IME_INFO IIME_ldb_somali;
extern const IIME_IME_INFO IIME_ldb_sindhi;
extern const IIME_IME_INFO IIME_ldb_assamese;
extern const IIME_IME_INFO IIME_ldb_tibetan;
extern const IIME_IME_INFO IIME_ldb_serbian;
extern const IIME_IME_INFO IIME_ldb_serbianc;
extern const IIME_IME_INFO IIME_ldb_zulu;
extern const IIME_IME_INFO IIME_ldb_cebuano;
extern const IIME_IME_INFO IIME_ldb_kazakh;
extern const IIME_IME_INFO IIME_ldb_uzbek;
extern const IIME_IME_INFO IIME_ldb_georgian;
extern const IIME_IME_INFO IIME_ldb_english_suretype;

/* Input method configuration table */

typedef const IIME_IME_INFO* IIME_IME_INFO_PTR;

extern const IIME_IME_INFO_PTR IIME_input_method_config[];

/* qwerty input method configuration */
typedef struct
{
    unsigned short key;
    unsigned short value[11];
}QWERTY_KEYMAP;

typedef struct
{
    int                     lang;
    const QWERTY_KEYMAP*    keymap;
} IIME_QWERTY_INFO;

extern const QWERTY_KEYMAP DefaultThaiMapping[];
extern const QWERTY_KEYMAP DefaultArmenianMapping[];
extern const QWERTY_KEYMAP DefaultBulgarianMapping[];
extern const QWERTY_KEYMAP DefaultCroatianMapping[];
extern const QWERTY_KEYMAP DefaultCzechMapping[];
extern const QWERTY_KEYMAP DefaultDanishMapping[];
extern const QWERTY_KEYMAP DefaultDutchMapping[];
extern const QWERTY_KEYMAP DefaultFinnishMapping[];
extern const QWERTY_KEYMAP DefaultFrenchMapping[];
extern const QWERTY_KEYMAP DefaultGermanMapping[];
extern const QWERTY_KEYMAP DefaultGreekMapping[];
extern const QWERTY_KEYMAP DefaultHungarianMapping[];
extern const QWERTY_KEYMAP DefaultItalianMapping[];
extern const QWERTY_KEYMAP DefaultLithuanianMapping[];
extern const QWERTY_KEYMAP DefaultNorwegianMapping[];
extern const QWERTY_KEYMAP DefaultPolishMapping[];
extern const QWERTY_KEYMAP DefaultPortugueseMapping[];
extern const QWERTY_KEYMAP DefaultRomanianMapping[];
extern const QWERTY_KEYMAP DefaultRussianMapping[];
extern const QWERTY_KEYMAP DefaultSlovakMapping[];
extern const QWERTY_KEYMAP DefaultSlovenianMapping[];
extern const QWERTY_KEYMAP DefaultSpanishMapping[];
extern const QWERTY_KEYMAP DefaultSwedishMapping[];
extern const QWERTY_KEYMAP DefaultTurkishMapping[];
extern const QWERTY_KEYMAP DefaultUkrainianMapping[];
extern const QWERTY_KEYMAP DefaultArabicMapping[];
extern const QWERTY_KEYMAP DefaultHebrewMapping[];
extern const QWERTY_KEYMAP DefaultPersianMapping[];
extern const QWERTY_KEYMAP DefaultUrduMapping[];
extern const QWERTY_KEYMAP DefaultHindiMapping[];
extern const QWERTY_KEYMAP DefaultBengaliMapping[];
extern const QWERTY_KEYMAP DefaultTeluguMapping[];
extern const QWERTY_KEYMAP DefaultMarathiMapping[];
extern const QWERTY_KEYMAP DefaultTamilMapping[];
extern const QWERTY_KEYMAP DefaultPunjabiMapping[];
extern const QWERTY_KEYMAP DefaultGujaratiMapping[];
extern const QWERTY_KEYMAP DefaultOriyaMapping[];
extern const QWERTY_KEYMAP DefaultKannadaMapping[];
extern const QWERTY_KEYMAP DefaultMalayalamMapping[];
extern const QWERTY_KEYMAP DefaultKhmerMapping[];
extern const QWERTY_KEYMAP DefaultLaoMapping[];
extern const QWERTY_KEYMAP DefaultMyanmarMapping[];
extern const QWERTY_KEYMAP DefaultUyghurMapping[];
extern const QWERTY_KEYMAP DefaultNepaliMapping[];
extern const QWERTY_KEYMAP DefaultSinhalaMapping[];
extern const QWERTY_KEYMAP DefaultBodoMapping[];//daodejing
extern const QWERTY_KEYMAP DefaultDogriMapping[];
extern const QWERTY_KEYMAP DefaultSanskritMapping[];
extern const QWERTY_KEYMAP DefaultKashmiriMapping[];
extern const QWERTY_KEYMAP DefaultKonkaniMapping[];
extern const QWERTY_KEYMAP DefaultMaithiliMapping[];
extern const QWERTY_KEYMAP DefaultManipuriMapping[];
extern const QWERTY_KEYMAP DefaultSantaliMapping[];
extern const QWERTY_KEYMAP DefaultSomaliMapping[];
extern const QWERTY_KEYMAP DefaultSindhiMapping[];
extern const QWERTY_KEYMAP DefaultAssameseMapping[];

/* language params*/
typedef struct
{
    int             dwLangId;           /* [in]  language id*/
    int             nMethods;           /* [out] the input methods number of current language*/
    unsigned short  idIM[12];           /* [out] input methods id, max is 8, which is IM_PINYIN, IM_BIHUA, IM_ABC, etc.*/
} IIME_LANG_PARAMS, *PIIME_LANG_PARAMS;

typedef int (*IIME_GET_TEXT_WIDTH_FUNC)(const unsigned short* text, int length);
typedef int (*CALLBACK_READ_UPDATE_DATA)(unsigned char *buffer, int pos, int length);

typedef int (*CALLBACK_CHECK_VALID_HANZI)(unsigned short unichr);
typedef int (*CALLBACK_GET_CONTEXT)(const unsigned short **text, int *length);

/* UI specified parameters */
typedef struct
{
    int             nMaxPYChoice;        /* [in] max display number of pinyin combination*/
    int             nWinWidth;           /* [in] Candidate window width*/
    int             nLableWidth;         /* [in] Lable width*/
    
	IIME_GET_TEXT_WIDTH_FUNC		pGetTextWidthFunc;	/* 获取字符串宽度的回调函数 */
	CALLBACK_READ_UPDATE_DATA	pfnReadUpdateData;	/* 读取更新词库回调函数，如果为空，则不使用更新词库 */
}IIME_IMM_PARAMS, *PIIME_IMM_PARAMS;

/* user-defined-word and auto-frequence-adjustment data params*/
typedef struct
{
    int             iUserCacheSize;     /* [in] user-word buffer size*/
    char*           pUserCache;         /* [in] user-word buffer address*/
} IIME_USER_PARAMS, *PIIME_USER_PARAMS;

/* UI client content for basic API */
typedef struct
{
    /* Commit content*/
    unsigned short pCommit[64];

    /* pre-edit content*/
    unsigned short pPreBuffer[64];      /* buffer of pre-edit window*/
    unsigned short nPreEdit;            /* number of pre-edit content*/
    unsigned short iHotPreEdit;         /* the hotspot of pre-edit*/

    /* Candidate content*/
    unsigned short pCandBuffer[64];     /* buffer of candidate window*/
    unsigned short nCand;               /* number of candidate content*/
    unsigned short iHotCand;            /* the hotspot of candidate*/

    /* pychoice content*/
    unsigned short pChoiceBuffer[64];
    unsigned short nChoice;
    unsigned short iHotChoice;

    /* underline content*/
    unsigned short pUnderlineBuffer[64];
    unsigned short nUnderline;
    unsigned short iHotUnderline;

    /* user word content*/
    unsigned short pUserWordBuffer[64];
    unsigned short nUserWord;
    unsigned short iHotUserWord;

    /* underline caret pos begin from 0,*/
    unsigned short iCaretPos;
    unsigned short bShowCaret;

    /* Page up/down flag of Candidate and Pinyin-Choice window*/
    unsigned short flagPageCand;
    unsigned short flagPageChoice;

    /* state*/
    unsigned short flagState;

    /* Timer operator*/
    unsigned short flagTimer;
} IIME_IMM_CONTENT, *PIIME_IMM_CONTENT;

typedef unsigned short MULTI_STRING[16];

typedef struct 
{
    unsigned short multi_key_number;
    unsigned short long_multi_key_number;
    const MULTI_STRING *multi_table;
    const MULTI_STRING *long_multi_table;
} IIME_MULTI_STRING_TABLE;

/* association flags for stroke and pinyin input method */
#define IIME_ASSOCIATION_ON              1            /* switch on association*/
#define IIME_ASSOCIATION_OFF             0            /* switch off assocition (default)*/

/* keyboard layout for five-stroke input method (default) */
#define IIME_STROKE_STANDARD_LAYOUT      0x0000       
/* national standard layout
        1-5 key: hen shu pie na zhe
        6 key  : ? anything
        7 key : magic-one for symbols, digits and english
 */

#define IIME_STROKE_IIMETAR_LAYOUT_ONE      0x0001
/* IIME defined keyboad layout
        1 key: magic-one for symbols, digits and english
        2-6 keys: hen shu pie na zhe
        7 key: ? anything
 */

/* config whether disable wildcard or not. default is not disable wildcard */
#define IIME_STROKE_DISABLE_WILDCARD_OFF  0  /* Enable stroke wildcard. (default) */
#define IIME_STROKE_DISABLE_WILDCARD_ON   1  /* Disable stroke wildcard. */ 


#define IIME_PINYIN_MODE_1                0x0000      /* user can edit pinyin, the "star" key is iterator choice */
#define IIME_PINYIN_MODE_2                0x0001      /* user can't edit pinyin, user can't step candidates */
#define IIME_PINYIN_MODE_3                0x0002      /* user can't edit pinyin, user can step candidates */
#define IIME_PINYIN_MODE_4                0x0003      /* user can't edit pinyin, user can step candidates */
#define IIME_PINYIN_MODE_5                0x0004      /* bird mode */

#define IIME_CANDIDATE_MODE_1                0x0001      /* 1 line candidates */
#define IIME_CANDIDATE_MODE_2                0x0002      /* 2 line candidates */

#define IIME_FULLKEY_PINYIN_MODE_1        0x0001      /* 全键盘输入时，候选字自动加入标签 */
#define IIME_FULLKEY_PINYIN_MODE_2        0x0002      /* 全键盘输入时，需要按方向键后，才有候选字标签 */

/* fuzzy phone flags for pinyin input method */
#define IIME_FUZZY_SHENGMU_NUM    10
#define IIME_FUZZY_Z_ZH           0x0001    /* 打开模糊音 z zh*/
#define IIME_FUZZY_C_CH           0x0002    /* 打开模糊音 c ch*/
#define IIME_FUZZY_S_SH           0x0004    /* 打开模糊音 s sh */
#define IIME_FUZZY_H_F            0x0008    /* 打开模糊音 h f */
#define IIME_FUZZY_N_L            0x0010    /* 打开模糊音 n l */

#define IIME_FUZZY_YUNMU_NUM      10
#define IIME_FUZZY_IN_ING         0x0100    /* 打开模糊音 in ing */
#define IIME_FUZZY_EN_ENG         0x0200    /* 打开模糊音 en eng */
#define IIME_FUZZY_AN_ANG         0x0400    /* 打开模糊音 an ang */
#define IIME_FUZZY_IAN_IANG       0x0800    /* 打开模糊音 ian iang */
#define IIME_FUZZY_UAN_UANG       0x1000    /* 打开模糊音 uan uang */
#define IIME_EUDW_DISABLED        0x8000   /* 取消西文的用户自定义词 */

/* Alphabetic language input mdoe flags */
#define IIME_ALPHABETIC_PHRASE_INPUT     0            /* Phrase input for alphabetic language*/
#define IIME_ALPHABETIC_WORD_INPUT       1            /* Word input for alphabetic language */

/* magic one on star key */
#define IIME_MAGICONE_DEFAULT             0
#define IIME_MAGICONE_STAR                1

/* zhuyin symbols as candidate */
#define IIME_ZHUYIN_SYMBOL_AS_CAND_OFF    0
#define IIME_ZHUYIN_SYMBOL_AS_CAND_ON    1

/* options for input method*/
typedef struct
{
    CALLBACK_CHECK_VALID_HANZI  pfnCheckValidHanzi;
    CALLBACK_GET_CONTEXT        pfnGetContext;

    const IIME_QWERTY_INFO              *pQwertyInfo;

    unsigned int     dwPYFuzzy:16;       /* 模糊音设置*/
    unsigned int     dwStrokeLayout:1;   /* 笔画键盘布局*/
    unsigned int     dwStrokeWildCard:1; /* Stroke wildcard config */
    unsigned int     dwAssoPinyin:1;     /* 拼音联想设置*/
    unsigned int     dwAssoStroke:1;     /* 笔画联想设置*/
    unsigned int     dwPinyinMode:4;     /* 拼音操作模式 */
    unsigned int     dwCandidateMode:2;  /* 候选项模式 */
    unsigned int     dwMarScript:2;      /* 火星文选项 */
    unsigned int     dwAlphaBeticInputMode:2;    /* 字母语言输入模式 整句/单词 */
    unsigned int     dwFullkeyPinyinMode : 2;
    unsigned int     dwMagicOne : 1;
    unsigned int     dwZhuyinSymbolAsCand: 1;
    unsigned int     dwPad: 30;
} IIME_IMM_OPTIONS, *PIIME_IMM_OPTIONS;


/* Initialization/Cleanup */
IIME_HIMM IIME_IMM_Initialize(PIIME_LANG_PARAMS pLangParams, PIIME_IMM_PARAMS pImmParams, PIIME_USER_PARAMS pUserParam);
IIME_RESULT IIME_IMM_Terminate(IIME_HIMM hImm);

/* Input method selection */
IIME_UINT IIME_IMM_GetInputMethod(IIME_HIMM hImm);
IIME_RESULT IIME_IMM_SetInputMethod(IIME_HIMM hImm, IIME_UINT iNewMethod);

IIME_RESULT IIME_IMM_SetInputMethodEx(IIME_HIMM hImm, IIME_UINT iNewMethod, const QWERTY_KEYMAP* keymap);

/* Add User defined multitype input method */
IIME_RESULT IIME_IMM_AddMultiIM(IIME_HIMM hImm, IIME_UINT iMethod, const IIME_MULTI_STRING_TABLE* multi_table);

/* Event handler */
IIME_RESULT IIME_IMM_EventHandle(IIME_HIMM hImm, IIME_UINT hEvent, IIME_UINT iMsg, PIIME_IMM_CONTENT pContent);

/* Options */
IIME_RESULT IIME_IMM_SetOptionsEx(IIME_HIMM hImm, const IIME_IMM_OPTIONS* pOptions);
IIME_RESULT IIME_IMM_GetOptionsEx(IIME_HIMM hImm, PIIME_IMM_OPTIONS pOptions);

/* add user phase */
/* the input method must IM_PINYIN */
IIME_RESULT IIME_IMM_AddPhase(IIME_HIMM hImm, unsigned short *szPhrase, int nLen);

IIME_RESULT IIME_IMM_CheckAsso(IIME_HIMM hImm, unsigned short wContext, unsigned short *szAsso, int *pNumber);

IIME_RESULT IIME_IMM_QueryProperties(IIME_HIMM hImm, int *pProperties);

const char* IIME_IMM_GetVersion(void);

const char* IIME_IMM_GetPinyinCharset(const IIME_IME_INFO* ime_info);

/* Query Hanzi Association */
IIME_RESULT   HanziAsso_QueryAsso(IIME_BOOL  isPinyin,
								IIME_WCHAR wContext,	/* 上下文内容unicode字符串，如“开始” */
                                IIME_PWSTR pwzAsso,		/* 返回的联想候选序列*/
								IIME_INT  *pnAsso);		/* 输入为所要的联想最大个数，返回的是实际候选个数*/

/*
 *
 * service provider interface 
 *
 */
typedef enum KMX_ACTIVE_STATE
{
	kmx_en_try,
	kmx_en_notice,
	kmx_en_actived
} kmx_active_state;

typedef struct
{
    int window_width;
    int label_width;
    int char_width;
    int reserved;
} CANDIDATE_WINDOW_METRICS;

kmx_active_state IIME_IMM_GetActiveState(void);

void IIME_IMM_SetOutputNumber(unsigned int);

int mmi_get_candidate_window_metriIIME(CANDIDATE_WINDOW_METRICS* metriIIME);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese pinyin and bpmf query                                           */
/*                                                                         */
/*-------------------------------------------------------------------------*/

IIME_BOOL IIME_IMM_QueryCodePinyin(IIME_WORD wCode, IIME_WCHAR *wszPinyin);

IIME_BOOL IIME_IMM_QueryCodeZhuyin(IIME_WORD wCode, IIME_WCHAR *wszZhuyin);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese stroke query                                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Stroke code constants */

#define IIME_IMM_STROKE_H     1   /* Heng */
#define IIME_IMM_STROKE_S     2   /* Shu  */
#define IIME_IMM_STROKE_P     3   /* Pie  */
#define IIME_IMM_STROKE_D     4   /* Dian */
#define IIME_IMM_STROKE_Z     5   /* Zhe  */

/* Maximum stroke count of Chinese character */

#define IIME_IMM_MAX_STROKE   63

int IIME_IMM_QueryStroke_gb(
    unsigned short char_code,
    char* buffer,
    int buffer_length);

int IIME_IMM_QueryStroke_gbk(
    unsigned short char_code,
    char* buffer,
    int buffer_length);

int IIME_IMM_QueryStroke_big5(
    unsigned short char_code,
    char* buffer,
    int buffer_length);

IIME_BOOL IIME_IME_IsShowPreEditWin(IIME_UINT method);


#ifdef __cplusplus 
} 
#endif 

#endif /* IIMEK2API_H */
