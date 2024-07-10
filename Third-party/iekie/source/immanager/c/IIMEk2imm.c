/**************************************************************************
 IIMEk2imm.c

     Description:

     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   11/25/2005 10:15:38 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#include "IIMEstd.h"

#include "provider_com.h"
#include "IIMEk2imm.h"

#include "wrappingprovider.h"


#include "IIMEpvdmo.h"
#include "IIMEmtprovider.h"
#include "IIMEudmtprovider.h"

#include "IIMEpvdlatin4.h"

#include "kmxime_latin.h"

#include "IIMEpvdlatinfull.h"

#include "IIMEmemory.h"//IIMEMemory_SelfAlloc
#include "IIMEstring.h"//IIMEString_Length


#ifdef IIME_PINYIN_SUPPORT
#include "IIMEpvdchinesefull.h"
#include "IIMEfpyprovider.h"
#include "IIMEpvdstkfull.h"
#endif

#ifdef IIME_LATIN_INLINE
#include "IIMEpvdlatininline.h"
#endif

#ifdef KMX_LDB_AMHARIC
#include "IIMEpvdamharic_2.h"
#ifdef MSTAR_PRODUCT
//#include "IIMEpvdamharicfull.h"
#elif defined(SPREADTRUM_PRODUCT)
#include "IIMEpvdamharicfull_2.h"
#endif
#elif defined(SPREADTRUM_PRODUCT)
#include "IIMEpvdamharicfull_2.h"
#endif


/*
#ifdef _MSC_VER
#include "windows.h"
static wchar_t out[512];
#endif
*/

/*
 Macro define, typedef
 */


#define MIN_CACHE_BLOCK	(2* 1024)

#define GLUE(a, b, c) a ## b ## c

#define DECLARE_LANGUAGE(name, short_name)  \
    {IM_ ## name,			            IM_ ## name, 	    IIMELatin4Provider_Create, IIMELatin4Provider_Delete}, \
    {GLUE(IM_, name, _LEAD_UPPER),	    IM_ ## name, 	    IIMELatin4Provider_Create, IIMELatin4Provider_Delete}, \
    {GLUE(IM_, name, _UPPER),	        IM_ ## name, 	    IIMELatin4Provider_Create, IIMELatin4Provider_Delete}, \
    {GLUE(IM_, name, _LOWER),	        IM_ ## name, 	    IIMELatin4Provider_Create, IIMELatin4Provider_Delete}, \
    {IM_ABC_LOWER_ ## short_name,		IM_ ## name,        IIMEMTProvider_Create, IIMEMTProvider_Delete}, \
    {IM_ABC_UPPER_ ## short_name,		IM_ ## name,        IIMEMTProvider_Create, IIMEMTProvider_Delete}, \
    {IM_ABC_LEAD_UPPER_ ## short_name,	IM_ ## name,        IIMEMTProvider_Create, IIMEMTProvider_Delete}, \
    {IM_ABC_ ## short_name,	            IM_ ## name,        IIMEMTProvider_Create, IIMEMTProvider_Delete}, \
    {IM_FULLKEY_BASE + IM_ ## name,			            IM_ ## name, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + GLUE(IM_, name, _LEAD_UPPER),    IM_ ## name,    IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + GLUE(IM_, name, _UPPER),	        IM_ ## name, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + GLUE(IM_, name, _LOWER),	        IM_ ## name, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + IM_ABC_LOWER_ ## short_name,		IM_ ## name,    IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + IM_ABC_UPPER_ ## short_name,		IM_ ## name,    IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + IM_ABC_LEAD_UPPER_ ## short_name,IM_ ## name,    IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete}, \
    {IM_FULLKEY_BASE + IM_ABC_ ## short_name,           IM_ ## name,    IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},

static const ProviderAttribute arrProvider[] = 
{
#ifdef IIME_PINYIN
	{IM_PINYIN,			                IM_PINYIN,		                    WrappingProvider_Create, WrappingProvider_Delete},
    {IM_PINYIN_SURETYPE,                IM_PINYIN_SURETYPE,                 WrappingProvider_Create, WrappingProvider_Delete},
    {IM_PINYIN_FULLKEY,                 IM_PINYIN,                          IIMEFPYProvider_Create,    IIMEFPYProvider_Delete},
	{IM_PINYIN_TC,	                    IM_PINYIN_TC,                       WrappingProvider_Create, WrappingProvider_Delete},
    {IM_PINYIN_TC + IM_SURETYPE_BASE,   IM_PINYIN_TC + IM_SURETYPE_BASE,    WrappingProvider_Create, WrappingProvider_Delete},
    {IM_PINYIN_FULLKEY_TC,              IM_PINYIN_TC,                       IIMEFPYProvider_Create,    IIMEFPYProvider_Delete},

    {IM_PINYIN + IM_FULLKEY_BASE,       IM_PINYIN,                          ChineseFullProvider_Create, ChineseFullProvider_Delete},

#endif

#ifdef IIME_PINYIN_MINI
	{IM_PINYIN_DZ, IM_PINYIN_DZ, WrappingProvider_Create, WrappingProvider_Delete},
#ifdef IIME_PINYIN_SUPPORT
	{IM_PINYIN_DZ + IM_FULLKEY_BASE, IM_PINYIN_DZ, ChineseFullProvider_Create, ChineseFullProvider_Delete},
#endif
/*
#if !defined(IIME_PINYIN)
    {IM_PINYIN_FULLKEY, IM_PINYIN_DZ,   IIMEFPYProvider_Create,   IIMEFPYProvider_Delete},
    {IM_PINYIN + IM_FULLKEY_BASE,    IM_PINYIN_DZ,           ChineseFullProvider_Create, ChineseFullProvider_Delete},
#endif
*/
#endif

#ifdef IIME_ZHUYIN
	{IM_ZHUYIN,			            IM_ZHUYIN,          WrappingProvider_Create, WrappingProvider_Delete},

	{IM_ZHUYIN_MT,			        IM_ZHUYIN_MT,       WrappingProvider_Create, WrappingProvider_Delete},
	{IM_ZHUYIN_MT_FULL,	            IM_ZHUYIN_MT,       WrappingProvider_Create, WrappingProvider_Delete},
	{IM_ZHUYIN_FULLKEY,		        IM_ZHUYIN_MT,       IIMEFPYProvider_Create, IIMEFPYProvider_Delete},
	{IM_ZHUYIN_MT_SC,		        IM_ZHUYIN_MT_SC,    WrappingProvider_Create, WrappingProvider_Delete},
	{IM_ZHUYIN_FULLKEY_SC,	        IM_ZHUYIN_MT_SC,    IIMEFPYProvider_Create, IIMEFPYProvider_Delete},

    {IM_ZHUYIN + IM_FULLKEY_BASE,   IM_ZHUYIN_MT,   ChineseFullProvider_Create, ChineseFullProvider_Delete},
    {IM_ZHUYIN_2 + IM_FULLKEY_BASE, IM_ZHUYIN_MT,   ChineseFullProvider_Create, ChineseFullProvider_Delete},
#endif

#ifdef IIME_ZHUYIN_MINI
	{IM_ZHUYIN_MT,			IM_ZHUYIN_MT,	    WrappingProvider_Create, WrappingProvider_Delete},
    #ifdef IIME_PINYIN_SUPPORT
	    {IM_ZHUYIN_FULLKEY,		IM_ZHUYIN_MT,	    IIMEFPYProvider_Create, IIMEFPYProvider_Delete},
        #if !defined(IIME_ZHUYIN)
            {IM_ZHUYIN + IM_FULLKEY_BASE,   IM_ZHUYIN_MT,   ChineseFullProvider_Create, ChineseFullProvider_Delete},
            {IM_ZHUYIN_2 + IM_FULLKEY_BASE, IM_ZHUYIN_MT,   ChineseFullProvider_Create, ChineseFullProvider_Delete},
        #endif
    #endif
#endif

#ifdef IIME_STROKE
	{IM_STROKE,				IM_STROKE,		WrappingProvider_Create, WrappingProvider_Delete},

	{IM_STROKE_BIG5,		IM_STROKE_BIG5, WrappingProvider_Create, WrappingProvider_Delete},

    #ifdef IIME_PINYIN_SUPPORT
        {IM_STROKE + IM_FULLKEY_BASE,		IM_STROKE,		StrokeFullProvider_Create, StrokeFullProvider_Delete},

	    {IM_STROKE_BIG5 + IM_FULLKEY_BASE,	IM_STROKE_BIG5, StrokeFullProvider_Create, StrokeFullProvider_Delete},
    #endif
#endif


#ifdef IIME_LATIN_INLINE
    {IM_ENGLISH,            IM_ENGLISH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
    {IM_ENGLISH_LEAD_UPPER, IM_ENGLISH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
	{IM_ENGLISH_UPPER,		IM_ENGLISH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
	{IM_ENGLISH_LOWER,		IM_ENGLISH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
#else
    {IM_ENGLISH,	        IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_ENGLISH_LEAD_UPPER, IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_ENGLISH_UPPER,		IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_ENGLISH_LOWER,		IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
#endif
	{IM_ABC,			IM_ENGLISH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
	#ifdef KMX_THI_MT_CANDIDATE_BOX
	{IM_ABC_LOWER,			IM_ENGLISH,     IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_ABC_UPPER,			IM_ENGLISH,     IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_ABC_LEAD_UPPER,		IM_ENGLISH,     IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	#else
	{IM_ABC_LOWER,			IM_ENGLISH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
	{IM_ABC_UPPER,			IM_ENGLISH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
	{IM_ABC_LEAD_UPPER,		IM_ENGLISH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
	#endif
	{IM_SURETYPE_BASE + IM_ENGLISH,			IM_SURETYPE_BASE + IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_SURETYPE_BASE + IM_ENGLISH_UPPER,	IM_SURETYPE_BASE + IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_SURETYPE_BASE + IM_ENGLISH_LOWER,	IM_SURETYPE_BASE + IM_ENGLISH, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
	{IM_SURETYPE_BASE + IM_ABC_LOWER,		IM_SURETYPE_BASE + IM_ENGLISH,  IIMEMTProvider_Create, IIMEMTProvider_Delete},
	{IM_SURETYPE_BASE + IM_ABC_UPPER,		IM_SURETYPE_BASE + IM_ENGLISH,  IIMEMTProvider_Create, IIMEMTProvider_Delete},
	{IM_SURETYPE_BASE + IM_ABC_LEAD_UPPER,	IM_SURETYPE_BASE + IM_ENGLISH,  IIMEMTProvider_Create, IIMEMTProvider_Delete},
    
    #ifndef IIME_DIGIT_ONLY
	    {IM_LATINFULLKEY_BASE + IM_ENGLISH,	            IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	    {IM_LATINFULLKEY_BASE + IM_ENGLISH_LEAD_UPPER,	IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	    {IM_LATINFULLKEY_BASE + IM_ENGLISH_UPPER,		IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	    {IM_LATINFULLKEY_BASE + IM_ENGLISH_LOWER,		IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},

	    {IM_LATINFULLKEY_BASE + IM_ABC_LOWER,			IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	    {IM_LATINFULLKEY_BASE + IM_ABC_UPPER,			IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	    {IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER,		IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	    {IM_LATINFULLKEY_BASE + IM_ABC,		            IM_ENGLISH, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    #endif

#if defined(KMX_LDB_RUSSIAN)
    DECLARE_LANGUAGE(RUSSIAN, RUS)
#endif

#if defined(KMX_LDB_GERMAN)
    DECLARE_LANGUAGE(GERMAN, DE)
#endif

#if defined(KMX_LDB_VIETNAMESE)
    DECLARE_LANGUAGE(VIETNAMESE, VI)
#endif

#if defined(KMX_LDB_INDONESIAN)
    DECLARE_LANGUAGE(INDONESIAN, ID)      
#endif

#if defined(KMX_LDB_TURKISH)
    DECLARE_LANGUAGE(TURKISH, TR)
#endif

#if defined(KMX_LDB_MALAY)
    DECLARE_LANGUAGE(MALAY, MS)
#endif

#if defined(KMX_LDB_GREEK)
    DECLARE_LANGUAGE(GREEK, EL)
#endif

#if defined(KMX_LDB_THAI)
    {IM_THAI,				IM_THAI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
     #ifdef KMX_THI_MT_CANDIDATE_BOX
    {IM_THAI_MT,			IM_THAI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    #else
    {IM_THAI_MT,			IM_THAI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    #endif
    {IM_THAI_123,			IM_THAI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_LATINFULLKEY_BASE + IM_THAI,	IM_THAI, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_LATINFULLKEY_BASE + IM_THAI_MT, IM_THAI, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_LATINFULLKEY_BASE + IM_THAI_123,IM_THAI, IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_DANISH)
    DECLARE_LANGUAGE(DANISH, DA)
#endif

#if defined(KMX_LDB_DUTCH)
    DECLARE_LANGUAGE(DUTCH, NL)
#endif

#if defined(KMX_LDB_POLISH)
    DECLARE_LANGUAGE(POLISH, PL)
#endif

#if defined(KMX_LDB_FRENCH)
#ifdef IIME_LATIN_INLINE
    {IM_FRENCH,             IM_FRENCH,  IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
    {IM_FRENCH_LEAD_UPPER,  IM_FRENCH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
	{IM_FRENCH_UPPER,		IM_FRENCH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
	{IM_FRENCH_LOWER,		IM_FRENCH, 	IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
	{IM_ABC_LOWER_FR,			IM_FRENCH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
	{IM_ABC_UPPER_FR,			IM_FRENCH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
	{IM_ABC_LEAD_UPPER_FR,		IM_FRENCH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
#else
    DECLARE_LANGUAGE(FRENCH, FR)
#endif
#endif

#if defined(KMX_LDB_HUNGARIAN)
    DECLARE_LANGUAGE(HUNGARIAN, HU)
#endif
 
#if defined(KMX_LDB_HEBREW)
    {IM_HEBREW,				IM_HEBREW, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_HEBREW_MT,			IM_HEBREW, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_HEBREW + IM_FULLKEY_BASE,		IM_HEBREW, 		IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_HEBREW_MT + IM_FULLKEY_BASE,    IM_HEBREW, 		IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_BULGARIAN)
    DECLARE_LANGUAGE(BULGARIAN, BG)
#endif

#if defined(KMX_LDB_CZECH)
    DECLARE_LANGUAGE(CZECH, IIME)
#endif

#if defined(KMX_LDB_FINNISH)
    DECLARE_LANGUAGE(FINNISH, FI)
#endif

#if defined(KMX_LDB_PERSIAN)
    {IM_PERSIAN,			IM_PERSIAN, 	IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_PERSIAN_MT,			IM_PERSIAN, 	IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_PERSIAN_123,		IM_PERSIAN, 	IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_PERSIAN + IM_FULLKEY_BASE,		IM_PERSIAN, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_PERSIAN_MT + IM_FULLKEY_BASE,   IM_PERSIAN, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SLOVAK)
    DECLARE_LANGUAGE(SLOVAK, SK)
#endif

#if defined(KMX_LDB_NORWEGIAN)
    DECLARE_LANGUAGE(NORWEGIAN, NO)
#endif

#if defined(KMX_LDB_SWEDISH)
    DECLARE_LANGUAGE(SWEDISH, SV)
#endif

#if defined(KMX_LDB_SPANISH)
    DECLARE_LANGUAGE(SPANISH, ES)
#endif

#if defined(KMX_LDB_CROATIAN)
    DECLARE_LANGUAGE(CROATIAN, HR)
#endif

#if defined(KMX_LDB_ROMANIAN)
    DECLARE_LANGUAGE(ROMANIAN, RO)
#endif

#if defined(KMX_LDB_SLOVENIAN)
    DECLARE_LANGUAGE(SLOVENIAN, SL)
#endif

#if defined(KMX_LDB_UKRAINIAN)
    DECLARE_LANGUAGE(UKRAINIAN, UK)
#endif

#if defined(KMX_LDB_SWAHILI)
    DECLARE_LANGUAGE(SWAHILI, SW)
#endif
               
#if defined(KMX_LDB_LITHUANIAN)
    DECLARE_LANGUAGE(LITHUANIAN, LT)
#endif

#if defined(KMX_LDB_ARMENIAN)
    DECLARE_LANGUAGE(ARMENIAN, HY)
#endif

#if defined(KMX_LDB_TAGALOG)
    DECLARE_LANGUAGE(TAGALOG, TL)
#endif

#if defined(KMX_LDB_KYRGYZ)
    DECLARE_LANGUAGE(KYRGYZ, KY)
#endif

#if defined(KMX_LDB_ALBANIAN)
    DECLARE_LANGUAGE(ALBANIAN, SQ)
#endif

#if defined(KMX_LDB_CATALAN)
    DECLARE_LANGUAGE(CATALAN, CA)
#endif

#if defined(KMX_LDB_BASQUE)
    DECLARE_LANGUAGE(BASQUE, EU)
#endif

#if defined(KMX_LDB_GALICIAN)
    DECLARE_LANGUAGE(GALICIAN, GL)
#endif

#if defined(KMX_LDB_IGBO)
    DECLARE_LANGUAGE(IGBO, IG)
#endif

#if defined(KMX_LDB_OROMO)
    DECLARE_LANGUAGE(OROMO, OM)
#endif

#if defined(KMX_LDB_CEBUANO)
    DECLARE_LANGUAGE(CEBUANO, CEB)
#endif

#if defined(KMX_LDB_ZULU)
    DECLARE_LANGUAGE(ZULU, ZU)
#endif

#if defined(KMX_LDB_KAZAKH)
    DECLARE_LANGUAGE(KAZAKH, KK)
#endif

#if defined(KMX_LDB_UZBEK)
    DECLARE_LANGUAGE(UZBEK, UZ)
#endif

#if defined(KMX_LDB_GEORGIAN)
    DECLARE_LANGUAGE(GEORGIAN, KA)
#endif

#if defined(KMX_LDB_PORTUGUESE)
    DECLARE_LANGUAGE(PORTUGUESE, PT)
#endif

#if defined(KMX_LDB_ITALIAN)
    DECLARE_LANGUAGE(ITALIAN, IT)
#endif

#if defined(KMX_LDB_ARABIC)
#ifdef IIME_LATIN_INLINE
    {IM_ARABIC,				IM_ARABIC, 		IIMELatinInlineProvider_Create, IIMELatinInlineProvider_Delete},
#else
    {IM_ARABIC,				IM_ARABIC, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
#endif
    {IM_ARABIC_MT,			IM_ARABIC, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_ARABIC_123,			IM_ARABIC, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_FULLKEY_BASE + IM_ARABIC,		IM_ARABIC, 		IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_ARABIC_MT,    IM_ARABIC, 		IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},

#endif

#if defined(KMX_LDB_HINDI)
    {IM_HINDI,				IM_HINDI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_HINDI_MT,			IM_HINDI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_HINDI_123,			IM_HINDI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_HINDI,		IM_HINDI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_HINDI_MT,    IM_HINDI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_HINDI_123,	IM_HINDI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_HINGLISH)
    DECLARE_LANGUAGE(HINGLISH, HIN)
#endif

#if defined(KMX_LDB_NEPALI)
    {IM_NEPALI,				IM_NEPALI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_NEPALI_MT,			IM_NEPALI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_NEPALI_123,			IM_NEPALI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_NEPALI,		IM_NEPALI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_NEPALI_MT,    IM_NEPALI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_NEPALI_123,	IM_NEPALI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SINHALA)
    {IM_SINHALA,				IM_SINHALA, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_SINHALA_MT,			IM_SINHALA, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SINHALA_123,			IM_SINHALA, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_SINHALA,		IM_SINHALA,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_SINHALA_MT,    IM_SINHALA,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_SINHALA_123,	IM_SINHALA, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

//daodejing
#if defined(KMX_LDB_BODO)
    {IM_BODO,				IM_BODO, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_BODO_MT,			IM_BODO, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_BODO_123,			IM_BODO, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_BODO,		IM_BODO,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_BODO_MT,    IM_BODO,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_BODO_123,	IM_BODO, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_DOGRI)
    {IM_DOGRI,				IM_DOGRI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_DOGRI_MT,			IM_DOGRI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_DOGRI_123,			IM_DOGRI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_DOGRI,		IM_DOGRI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_DOGRI_MT,    IM_DOGRI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_DOGRI_123,	IM_DOGRI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SANSKRIT)
    {IM_SANSKRIT,				IM_SANSKRIT, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_SANSKRIT_MT,			IM_SANSKRIT, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SANSKRIT_123,			IM_SANSKRIT, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_SANSKRIT,		IM_SANSKRIT,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_SANSKRIT_MT,    IM_SANSKRIT,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_SANSKRIT_123,	IM_SANSKRIT, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_KASHMIRI)
    {IM_KASHMIRI,				IM_KASHMIRI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_KASHMIRI_MT,			IM_KASHMIRI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_KASHMIRI_123,			IM_KASHMIRI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_KASHMIRI,		IM_KASHMIRI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_KASHMIRI_MT,    IM_KASHMIRI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_KASHMIRI_123,	IM_KASHMIRI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_KONKANI)
    {IM_KONKANI,				IM_KONKANI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_KONKANI_MT,			IM_KONKANI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_KONKANI_123,			IM_KONKANI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_KONKANI,		IM_KONKANI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_KONKANI_MT,    IM_KONKANI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_KONKANI_123,	IM_KONKANI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_MAITHILI)
    {IM_MAITHILI,				IM_MAITHILI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_MAITHILI_MT,			IM_MAITHILI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MAITHILI_123,			IM_MAITHILI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_MAITHILI,		IM_MAITHILI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_MAITHILI_MT,    IM_MAITHILI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_MAITHILI_123,	IM_MAITHILI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_MANIPURI)
    {IM_MANIPURI,				IM_MANIPURI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_MANIPURI_MT,			IM_MANIPURI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MANIPURI_123,			IM_MANIPURI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_MANIPURI,		IM_MANIPURI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_MANIPURI_MT,    IM_MANIPURI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_MANIPURI_123,	IM_MANIPURI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SANTALI)
    {IM_SANTALI,				IM_SANTALI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_SANTALI_MT,			IM_SANTALI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SANTALI_123,			IM_SANTALI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_SANTALI,		IM_SANTALI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_SANTALI_MT,    IM_SANTALI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_SANTALI_123,	IM_SANTALI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SOMALI)
    {IM_SOMALI,				IM_SOMALI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_SOMALI_MT,			IM_SOMALI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SOMALI_123,			IM_SOMALI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_SOMALI,		IM_SOMALI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_SOMALI_MT,    IM_SOMALI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_SOMALI_123,	IM_SOMALI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SINDHI)
    {IM_SINDHI,				IM_SINDHI, 		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_SINDHI_MT,			IM_SINDHI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SINDHI_123,			IM_SINDHI, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_FULLKEY_BASE + IM_SINDHI,		IM_SINDHI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
	{IM_FULLKEY_BASE + IM_SINDHI_MT,    IM_SINDHI,   IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_FULLKEY_BASE + IM_SINDHI_123,	IM_SINDHI, 	IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_BENGALI)
    {IM_BENGALI,			IM_BENGALI,		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_BENGALI_MT,			IM_BENGALI,		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_BENGALI_123,		IM_BENGALI,		IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_BENGALI + IM_FULLKEY_BASE,		IM_BENGALI,		IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_BENGALI_MT + IM_FULLKEY_BASE,   IM_BENGALI,		IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_HANGUL)
    {IM_HANGUL_MT,          IM_HANGUL,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
#endif

#if defined(KMX_LDB_UYGHUR)
    {IM_UYGHUR,             IM_UYGHUR,      IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_UYGHUR_MT,          IM_UYGHUR,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_UYGHUR + IM_FULLKEY_BASE,   IM_UYGHUR,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_UYGHUR_MT + IM_FULLKEY_BASE,IM_UYGHUR,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_URDU)
    {IM_URDU,               IM_URDU,        IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_URDU_MT,            IM_URDU,        IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_URDU + IM_FULLKEY_BASE,     IM_URDU,        IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_URDU_MT + IM_FULLKEY_BASE,  IM_URDU,        IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_TELUGU)
    {IM_TELUGU_MT,                      IM_TELUGU,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_TELUGU_MT + IM_FULLKEY_BASE,    IM_TELUGU,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_MARATHI)
    {IM_MARATHI_MT,                     IM_MARATHI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MARATHI_MT + IM_FULLKEY_BASE,   IM_MARATHI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_TAMIL)
    {IM_TAMIL_MT,                       IM_TAMIL,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_TAMIL_MT + IM_FULLKEY_BASE,     IM_TAMIL,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_PUNJABI)
    {IM_PUNJABI_MT,                       IM_PUNJABI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_PUNJABI_MT + IM_FULLKEY_BASE,     IM_PUNJABI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_PUNJABI)
    {IM_GUJARATI_MT,                       IM_GUJARATI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_GUJARATI_MT + IM_FULLKEY_BASE,     IM_GUJARATI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_ORIYA )
    {IM_ORIYA_MT,                       IM_ORIYA,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_ORIYA_MT + IM_FULLKEY_BASE,     IM_ORIYA,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_KANNADA )
    {IM_KANNADA_MT,                       IM_KANNADA,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_KANNADA_MT + IM_FULLKEY_BASE,     IM_KANNADA,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_MALAYALAM)
    {IM_MALAYALAM_MT,                       IM_MALAYALAM,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MALAYALAM_MT + IM_FULLKEY_BASE,     IM_MALAYALAM,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_KHMER)
    {IM_KHMER_MT,                       IM_KHMER,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_KHMER_MT + IM_FULLKEY_BASE,     IM_KHMER,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
    {IM_KHMER_123,			IM_KHMER, 		IIMEMTProvider_Create, IIMEMTProvider_Delete},
#endif

#if defined(KMX_LDB_LAO)
    {IM_LAO_MT,                       IM_LAO,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_LAO_MT + IM_FULLKEY_BASE,     IM_LAO,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_AMHARIC)
    {IM_AMHARIC_MT,                       IM_AMHARIC,      Amharic2Provider_Create, Amharic2Provider_Delete},
#ifdef SPREADTRUM_PRODUCT
    {IM_AMHARIC_MT + IM_FULLKEY_BASE,     IM_AMHARIC,      IIMEAmharicFullProvider2_Create, IIMEAmharicFullProvider2_Delete},
#else                                                                                     
//#ifndef MSTAR_PRODUCT
    {IM_AMHARIC_MT + IM_FULLKEY_BASE,     IM_AMHARIC,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
//#else
    //{IM_AMHARIC_MT + IM_FULLKEY_BASE,     IM_AMHARIC,      IIMEAmharicFullProvider_Create, IIMEAmharicFullProvider_Delete},
//#endif
#endif
#endif

#if defined(KMX_LDB_MYANMAR)
    {IM_MYANMAR_MT,                       IM_MYANMAR,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MYANMAR_MT + IM_FULLKEY_BASE,     IM_MYANMAR,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_ASSAMESE)
    {IM_ASSAMESE_MT,                       IM_ASSAMESE,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_ASSAMESE_123,                      IM_ASSAMESE,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_ASSAMESE_MT + IM_FULLKEY_BASE,     IM_ASSAMESE,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_NEPALI)
    {IM_NEPALI_MT,                       IM_NEPALI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_NEPALI_123,                      IM_NEPALI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_NEPALI_MT + IM_FULLKEY_BASE,     IM_NEPALI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SINHALA)
    {IM_SINHALA_MT,                       IM_SINHALA,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SINHALA_123,                      IM_SINHALA,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_SINHALA_MT + IM_FULLKEY_BASE,     IM_SINHALA,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

//daodejing
#if defined(KMX_LDB_BODO)
    {IM_BODO_MT,                       IM_BODO,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_BODO_123,                      IM_BODO,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_BODO_MT + IM_FULLKEY_BASE,     IM_BODO,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_DOGRI)
    {IM_DOGRI_MT,                       IM_DOGRI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_DOGRI_123,                      IM_DOGRI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_DOGRI_MT + IM_FULLKEY_BASE,     IM_DOGRI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SANSKRIT)
    {IM_SANSKRIT_MT,                       IM_SANSKRIT,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SANSKRIT_123,                      IM_SANSKRIT,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_SANSKRIT_MT + IM_FULLKEY_BASE,     IM_SANSKRIT,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_KASHMIRI)
    {IM_KASHMIRI_MT,                       IM_KASHMIRI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_KASHMIRI_123,                      IM_KASHMIRI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_KASHMIRI_MT + IM_FULLKEY_BASE,     IM_KASHMIRI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_KONKANI)
    {IM_KONKANI_MT,                       IM_KONKANI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_KONKANI_123,                      IM_KONKANI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_KONKANI_MT + IM_FULLKEY_BASE,     IM_KONKANI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_MAITHILI)
    {IM_MAITHILI_MT,                       IM_MAITHILI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MAITHILI_123,                      IM_MAITHILI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_MAITHILI_MT + IM_FULLKEY_BASE,     IM_MAITHILI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_MANIPURI)
    {IM_MANIPURI_MT,                       IM_MANIPURI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_MANIPURI_123,                      IM_MANIPURI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_MANIPURI_MT + IM_FULLKEY_BASE,     IM_MANIPURI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SANTALI)
    {IM_SANTALI_MT,                       IM_SANTALI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SANTALI_123,                      IM_SANTALI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_SANTALI_MT + IM_FULLKEY_BASE,     IM_SANTALI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SOMALI)
    {IM_SOMALI_MT,                       IM_SOMALI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SOMALI_123,                      IM_SOMALI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_SOMALI_MT + IM_FULLKEY_BASE,     IM_SOMALI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_SINDHI)
    {IM_SINDHI_MT,                       IM_SINDHI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_SINDHI_123,                      IM_SINDHI,      IIMEMTProvider_Create, IIMEMTProvider_Delete},

    {IM_SINDHI_MT + IM_FULLKEY_BASE,     IM_SINDHI,      IIMELatinFullProvider_Create, IIMELatinFullProvider_Delete},
#endif

#if defined(KMX_LDB_TIBETAN)
    {IM_TIBETAN,			IM_TIBETAN,		IIMELatin4Provider_Create, IIMELatin4Provider_Delete},
    {IM_TIBETAN_MT,			IM_TIBETAN,		IIMEMTProvider_Create, IIMEMTProvider_Delete},
    {IM_TIBETAN_123,		IM_TIBETAN,		IIMEMTProvider_Create, IIMEMTProvider_Delete},
#endif

#if defined(KMX_LDB_SERBIAN)
    DECLARE_LANGUAGE(SERBIAN, SR)
#endif

#if defined(KMX_LDB_SERBIANC)
    DECLARE_LANGUAGE(SERBIANC, SRC)
#endif

	{IM_123,			    IM_ENGLISH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},

#if defined(LEADCORE_PRODUCT)
    {IM_NUMBER,             IM_ENGLISH,     IIMEMTProvider_Create, IIMEMTProvider_Delete},
#endif

    {IM_USER_MULTITYPE_BASE, IM_ENGLISH,    IIMEUDMTProvider_Create, IIMEUDMTProvider_Delete},

	0
};

#define IMM_NUMBER (sizeof(arrProvider) / sizeof(arrProvider[0]) - 1)

extern unsigned int iekie_im_init_yet;
extern unsigned int iekie_im_get_init_state(void);
extern IIME_BOOL checkLdbDef();


static IIME_INT FindProvider(IIME_INT iMethod)
{
	IIME_INT i;

	//拦截非multitap
	//if (!iekie_im_get_init_state())
	//{
	//  return -1;
	//}
	if(!checkLdbDef())
	{
		return -1;
	}
	
    if (iMethod >= IM_USER_MULTITYPE_BASE && iMethod < IM_LATINFULLKEY_BASE)
        iMethod = IM_USER_MULTITYPE_BASE;

	for (i = 0; i < IMM_NUMBER; i++){
		if (arrProvider[i].m_idIM == iMethod)
			return i;
	}

	return -1;	/* don't find method*/
}

static int FindBaseInputMethod(int input_method)
{
	int i, iMethod;
	iMethod = 0;
	for (i = 0; i < sizeof(arrProvider) / sizeof(arrProvider[0]) - 1; i++){
		if (arrProvider[i].m_idIM == input_method){
            iMethod = arrProvider[i].m_idBaseIM;
			break;
		}
	}

    return iMethod;
}

static const void * FindDataBase(int input_method)
{
	int i, iMethod;


    if (input_method >= IM_USER_MULTITYPE_BASE && input_method < IM_LATINFULLKEY_BASE)
        input_method = IM_USER_MULTITYPE_BASE;

	iMethod = 0;
	for (i = 0; i < sizeof(arrProvider) / sizeof(arrProvider[0]) - 1; i++){
		if (arrProvider[i].m_idIM == input_method){
            iMethod = arrProvider[i].m_idBaseIM;
			break;
		}
	}

	if (iMethod == 0)
		return IIME_NULL;

	i = 0;
	while(IIME_input_method_config[i]){
		if (IIME_input_method_config[i]->input_method == iMethod)
			return IIME_input_method_config[i]->language_database;

		i++;
	}

    /* if input_method is IM_PINYIN_FULLKEY, use IM_PINYIN_SURETYPE database */
    if (input_method == IM_PINYIN_FULLKEY)
    {
        i = 0;
        while(IIME_input_method_config[i]){
            if (IIME_input_method_config[i]->input_method == IM_PINYIN_SURETYPE)
                return IIME_input_method_config[i]->language_database;

            i++;
        }
    }

	return IIME_NULL;
}

#define MAX_USER_DEFINED_MULTI_IM   8

typedef struct tagIIME_MULTI_IM
{
    int iMethod;
    const IIME_MULTI_STRING_TABLE* multi_table;
} IIME_MULTI_IM;

IIME_IMM_OPTIONS g_Options;

typedef struct tagIIME_IMM
{
	IIME_IMM_PARAMS		m_ImmParam;
	IIME_LANG_PARAMS		m_LangParam;
    IIME_IMM_OPTIONS		*m_pOptions;

	IIME_USER_PARAMS		m_UserParam;
    IIME_MULTI_IM         m_user_multi_im[MAX_USER_DEFINED_MULTI_IM];

	/*const void *      m_pdbMO;*/

	IIME_INT				m_idMethod;			/* method id, such as IM_PINYIN, IM_ZHUYIN, IM_ENGLISH, etc.*/
	Provider_Com*		m_pProvider;

    IIME_UINT             m_nOutput;
}IIME_IMM;

/*
 Declaration and Macro
 */

#define PROVIDER_PRECALL() 	IIME_RESULT ret;\
	Provider_Com *pProvider;\
\
	IIME_IMM *pImm = (IIME_IMM*)hImm;\
	if (pImm == IIME_NULL)\
		return IIME_ERROR_UNKNOWN;\
\
	pProvider = pImm->m_pProvider;\
	if (pProvider == IIME_NULL)\
		return IIME_ERROR_UNKNOWN;

static void SetOutputNumber(IIME_IMM *pImm);

int FindUserCacheByIM(int iMethod, const IIME_USER_PARAMS *pUserTotal, IIME_USER_PARAMS *pUserIM);

static IIME_UINT IMM_PreTranslateKey(IIME_IMM *pImm, IIME_UINT iKey);

#if defined(IIME_MARS_SCRIPT)
static void IMM_PostTranslateCand(IIME_IMM *pImm, IIME_IMM_CONTENT *pContent);
#endif

IIME_HIMM IIME_IMM_Initialize(PIIME_LANG_PARAMS pLangParams, 
						  PIIME_IMM_PARAMS pImmParams,
						  PIIME_USER_PARAMS pUserParams)
{
	IIME_INT i;
	IIME_IMM *pImm;

#ifdef SELFALLOC_PRODUCT
	IIME_MRESET(IIME_NULL);
#endif

	pImm = (IIME_IMM *)IIME_MALLOC(sizeof(IIME_IMM));
	if (pImm == IIME_NULL)
		return IIME_NULL;

    memset(pImm, 0, sizeof(IIME_IMM));

	/* save ImmParams*/
	memcpy(&(pImm->m_ImmParam), pImmParams, sizeof(IIME_IMM_PARAMS));

	if(pLangParams){/* initialize LangParams */
		pLangParams->nMethods = IMM_NUMBER;
		for (i = 0; i < IMM_NUMBER && i < sizeof(pLangParams->idIM) / sizeof(pLangParams->idIM[0]); i++)
			pLangParams->idIM[i] = (IIME_WORD)(arrProvider[i].m_idIM);
		
		memcpy(&(pImm->m_LangParam), pLangParams, sizeof(IIME_LANG_PARAMS));
	}

	/* initialize Options */
    pImm->m_pOptions = &(g_Options);

	pImm->m_pOptions->dwPYFuzzy = 0;
	pImm->m_pOptions->dwStrokeLayout = 1;
	pImm->m_pOptions->dwAssoPinyin = IIME_ASSOCIATION_OFF;
	pImm->m_pOptions->dwAssoStroke = IIME_ASSOCIATION_ON;
    pImm->m_pOptions->dwPinyinMode = IIME_PINYIN_MODE_1;
    pImm->m_pOptions->dwCandidateMode = IIME_CANDIDATE_MODE_1;

	/* initialize user cache */
    pImm->m_UserParam.pUserCache = pUserParams->pUserCache;
    pImm->m_UserParam.iUserCacheSize = pUserParams->iUserCacheSize;

	/* SetCurrent input method */
	pImm->m_pProvider = IIME_NULL;
	pImm->m_idMethod = 0;
    pImm->m_nOutput = 0;
	
    if (IIME_input_method_config[0] == 0)
        return IIME_NULL;

	/* set the current im */
	i = IM_123;//IIME_input_method_config[0]->input_method;

	IIME_IMM_SetInputMethod((IIME_HIMM)pImm, i);

    return pImm;
}

IIME_RESULT IIME_IMM_Terminate(IIME_HIMM hImm)
{
	IIME_IMM *pImm = (IIME_IMM*)hImm;
	if (pImm == IIME_NULL)
		return IIME_ERROR_UNKNOWN;

	if (pImm->m_pProvider != IIME_NULL){
		IIME_INT i = FindProvider(pImm->m_idMethod);
		IIME_ASSERT(i != -1);

		(*(arrProvider[i].m_pfnDelete))(pImm->m_pProvider);

		pImm->m_pProvider = IIME_NULL;
	}

	IIME_FREE(pImm);

	return IIME_ERROR_SUCCESS;
}

IIME_RESULT IIME_IMM_AddMultiIM(IIME_HIMM hImm, IIME_UINT iMethod, const IIME_MULTI_STRING_TABLE* multi_table)
{
    int i;
    IIME_IMM *pImm = (IIME_IMM*)hImm;

    if (pImm == IIME_NULL)
        return IIME_ERROR_UNKNOWN;

    if (iMethod < IM_USER_MULTITYPE_BASE || multi_table == IIME_NULL)
        return IIME_ERROR_UNKNOWN;
    
    for (i = 0; i < MAX_USER_DEFINED_MULTI_IM; i++)
    {
        IIME_MULTI_IM *pUserMultiIM = pImm->m_user_multi_im + i;
        if (pUserMultiIM->iMethod == 0)
        {
            pUserMultiIM->iMethod = iMethod;
            pUserMultiIM->multi_table = multi_table;
            return IIME_ERROR_SUCCESS;
        }
    }

    return IIME_ERROR_UNKNOWN;
}
const IIME_MULTI_STRING_TABLE* FindMultiTable(IIME_HIMM hImm, int iMethod)
{
    int i;
    IIME_IMM *pImm = (IIME_IMM*)hImm;
    if (iMethod < IM_USER_MULTITYPE_BASE || iMethod >= IM_LATINFULLKEY_BASE)
        return IIME_NULL;

    for (i = 0; i < MAX_USER_DEFINED_MULTI_IM; i++)
    {
        IIME_MULTI_IM *pUserMultiIM = pImm->m_user_multi_im + i;
        if (pUserMultiIM->iMethod == iMethod)
        {
            return pUserMultiIM->multi_table;
        }
    }

    return IIME_NULL;
}

/* Set/Get current input method*/
IIME_UINT IIME_IMM_GetInputMethod(IIME_HIMM hImm)
{
	IIME_IMM *pImm = (IIME_IMM*)hImm;
	if (pImm == IIME_NULL)
		return IIME_ERROR_UNKNOWN;

	return (pImm->m_idMethod);
}
#ifdef KMX_THI_MT_CANDIDATE_BOX
IIME_UINT iMethod;
int IIME_ime_is_thai_mt(void)
{
    if(iMethod == IM_THAI_MT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int IIME_ime_is_en_mt(void)
{
    if(iMethod == IM_ABC_LOWER||iMethod == IM_ABC_UPPER||iMethod == IM_ABC_LEAD_UPPER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int IIME_ime_is_en_Abc_mt(void)
{
    if(iMethod == IM_ABC_LEAD_UPPER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int IIME_ime_is_en_abc_mt(void)
{
    if(iMethod == IM_ABC_LOWER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int IIME_ime_is_thai(void)
{
    if(iMethod == IM_THAI_MT||iMethod == IM_THAI||iMethod == IM_THAI_123)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#else
int IIME_ime_is_thai_mt(void)
{
    return 0;
}
int IIME_ime_is_thai(void)
{
    return 0;
}
#endif
IIME_RESULT IIME_IMM_SetInputMethod(IIME_HIMM hImm,
                                IIME_UINT iNewMethod)
{
	IIME_INT iProvider;
	IIME_USER_PARAMS user_param1, user_param2;
	const void *pdbMisc = IIME_NULL;
    const void *pdbMagicOne = IIME_NULL;

	IIME_IMM *pImm = (IIME_IMM*)hImm;
	if (pImm == IIME_NULL)
		return IIME_ERROR_UNKNOWN;

	if (pImm->m_pProvider != 0){
		iProvider = FindProvider(pImm->m_idMethod);
		IIME_ASSERT(iProvider != -1);

		(*(arrProvider[iProvider].m_pfnDelete))(pImm->m_pProvider);
		pImm->m_pProvider = IIME_NULL;
	}

	iProvider = FindProvider(iNewMethod); 
	if (iProvider == -1)
		return IIME_ERROR_UNKNOWN;

#ifdef SELFALLOC_PRODUCT
	IIME_MRESET((IIME_BYTE*)pImm + sizeof(IIME_IMM));
#endif

    FindUserCacheByIM(iNewMethod, &(pImm->m_UserParam), &user_param1);
    FindUserCacheByIM(IM_ENGLISH, &(pImm->m_UserParam), &user_param2);

    pdbMisc = FindDataBase(iNewMethod);
    if (iNewMethod > IM_SURETYPE_BASE && iNewMethod < IM_USER_MULTITYPE_BASE)
        pdbMagicOne = FindDataBase(IM_SURETYPE_BASE + IM_ENGLISH);
    else
        pdbMagicOne = FindDataBase(IM_ENGLISH);

	if (pdbMisc){
        const IIME_MULTI_STRING_TABLE* pMulti_string_table;
        pMulti_string_table = FindMultiTable(hImm, iNewMethod);

        switch (iNewMethod)
        {
        case IM_PINYIN_TC:
            iNewMethod = IM_PINYIN;
            break;
        case IM_PINYIN_TC + IM_SURETYPE_BASE:
            iNewMethod = IM_PINYIN + IM_SURETYPE_BASE;
            break;
        case IM_PINYIN_FULLKEY_TC:
            iNewMethod = IM_PINYIN_FULLKEY;
            break;
        case IM_ZHUYIN_MT_SC:
            iNewMethod = IM_ZHUYIN_MT;
            break;
        case IM_ZHUYIN_FULLKEY_SC:
            iNewMethod = IM_ZHUYIN_FULLKEY;
            break;
        }
#ifdef KMX_THI_MT_CANDIDATE_BOX
    iMethod = iNewMethod;
#endif
        pImm->m_pProvider = (*(arrProvider[iProvider].m_pfnCreate))(&(pImm->m_ImmParam),
																	&user_param1,
																	&user_param2,
																	pdbMisc,/*idMiscDB,*/
																	pdbMagicOne, /*pImm->m_pdbMO,*/
                                                                    pMulti_string_table,
																	iNewMethod
																	);
		pImm->m_idMethod = iNewMethod;

		IIME_IMM_SetOptionsEx(hImm, pImm->m_pOptions);

        SetOutputNumber(pImm);

		return IIME_ERROR_SUCCESS;
	}
	else
		return IIME_ERROR_UNKNOWN;
}

IIME_RESULT IIME_IMM_SetInputMethodEx(IIME_HIMM hImm,
                                  IIME_UINT iNewMethod,
                                  const QWERTY_KEYMAP* keymap)
{
	IIME_INT iProvider;
	IIME_USER_PARAMS user_param1, user_param2;
	const void *pdbMisc = IIME_NULL;
    const void *pdbMagicOne = IIME_NULL;

	IIME_IMM *pImm = (IIME_IMM*)hImm;
	if (pImm == IIME_NULL)
		return IIME_ERROR_UNKNOWN;

    if (iNewMethod < IM_FULLKEY_BASE)
        return IIME_ERROR_UNKNOWN;

	if (pImm->m_pProvider != 0){
		iProvider = FindProvider(pImm->m_idMethod);
		IIME_ASSERT(iProvider != -1);

		(*(arrProvider[iProvider].m_pfnDelete))(pImm->m_pProvider);
		pImm->m_pProvider = IIME_NULL;
	}

	iProvider = FindProvider(iNewMethod);
	if (iProvider == -1)
		return IIME_ERROR_UNKNOWN;

#ifdef SELFALLOC_PRODUCT
	IIME_MRESET((IIME_BYTE*)pImm + sizeof(IIME_IMM));
#endif

    FindUserCacheByIM(iNewMethod, &(pImm->m_UserParam), &user_param1);
    FindUserCacheByIM(IM_ENGLISH, &(pImm->m_UserParam), &user_param2);

    pdbMisc = FindDataBase(iNewMethod);
    if (iNewMethod > IM_SURETYPE_BASE && iNewMethod < IM_USER_MULTITYPE_BASE)
        pdbMagicOne = FindDataBase(IM_SURETYPE_BASE + IM_ENGLISH);
    else
        pdbMagicOne = FindDataBase(IM_ENGLISH);

	if (pdbMisc){
        const IIME_MULTI_STRING_TABLE* pMulti_string_table;
        pMulti_string_table = (IIME_MULTI_STRING_TABLE*)keymap;

        pImm->m_pProvider = (*(arrProvider[iProvider].m_pfnCreate))(&(pImm->m_ImmParam),
																	&user_param1,
																	&user_param2,
																	pdbMisc,/*idMiscDB,*/
																	pdbMagicOne, /*pImm->m_pdbMO,*/
                                                                    pMulti_string_table,
																	iNewMethod
																	);
		pImm->m_idMethod = iNewMethod;

		IIME_IMM_SetOptionsEx(hImm, pImm->m_pOptions);

        SetOutputNumber(pImm);

		return IIME_ERROR_SUCCESS;
	}
	else
		return IIME_ERROR_UNKNOWN;
}

/* event handler*/
IIME_RESULT IIME_IMM_EventHandle(IIME_HIMM hImm,
											IIME_UINT hEvent,
											IIME_UINT iMsg,
											PIIME_IMM_CONTENT pContent)
{
	PROVIDER_PRECALL();

	if (hEvent == IIME_EVENT_KEYBOARD)
		iMsg = IMM_PreTranslateKey(pImm, iMsg);

	if ( hEvent == IIME_EVENT_KEYBOARD && 
		 ((iMsg & 0xFFFF) == IIME_VK_NONE) )
		ret = IIME_ERROR_UNUSED;
	else
    {
        unsigned short iCaretPos = pContent->iCaretPos;
        memset(pContent, 0, sizeof(IIME_IMM_CONTENT));
        pContent->iCaretPos = iCaretPos;

		ret = (*(pProvider->m_pfnEventHandle))(pImm->m_pProvider, hEvent, iMsg, pContent);
    }
	#if 0//ZHMCH	IIMEstring.c	文件已经去掉!
    if (pImm->m_idMethod == IM_PINYIN)
    {
        int nLen = IIMEString_Length(pContent->pCommit);
        pImm->m_nOutput += nLen;
    }
	#endif

#if defined(IIME_MARS_SCRIPT)
    IMM_PostTranslateCand(pImm, pContent);
#endif

#ifdef IIME_LATIN_INLINE
    if (ret >> 16)
    {
        pImm->m_idMethod = (ret >> 16);
        ret = (ret & 0xFFFF);
    }
#endif
	return ret;
}


/* Get/ Set Options */
IIME_RESULT IIME_IMM_SetOptionsEx(IIME_HIMM hImm,
											 const IIME_IMM_OPTIONS* pOptions)
{
	PROVIDER_PRECALL();

	memcpy(pImm->m_pOptions, pOptions, sizeof(IIME_IMM_OPTIONS));

	ret = (*(pProvider->m_pfnSetOptions))(pProvider, pImm->m_pOptions);

	return ret;
}

IIME_RESULT IIME_IMM_GetOptionsEx(IIME_HIMM hImm,
											 PIIME_IMM_OPTIONS pOptions)
{
	PROVIDER_PRECALL();

    ret = (*(pProvider->m_pfnGetOptions))(pProvider, pOptions);

	return ret;
}

/*
 Implementions
 */

static int SpliteCache(void)
{
    IIME_BOOL bHasPinyin, bHasZhuyin;
    IIME_BOOL bHasPinyin_tc, bHasZhuyin_sc;
    int i, nBlock;

    nBlock = 0;
    i = 0;
    bHasPinyin = 0;
    bHasZhuyin = 0;
    bHasPinyin_tc = 0;
    bHasZhuyin_sc = 0;
    while(IIME_input_method_config[i])
    {
        switch (IIME_input_method_config[i]->input_method)
        {
        case IM_PINYIN_DZ:
            break;
        case IM_PINYIN:
        case IM_PINYIN_SURETYPE:
        case IM_PINYIN_FULLKEY:
            if (!bHasPinyin)
            {
                nBlock += 2;
                bHasPinyin = 1;
            }
            break;
        case IM_PINYIN_TC:
        case IM_PINYIN_TC + IM_SURETYPE_BASE:
        case IM_PINYIN_FULLKEY_TC:
            if (!bHasPinyin_tc)
            {
                nBlock += 2;
                bHasPinyin_tc = 1;
            }
            break;
        case IM_ZHUYIN_MT:
        case IM_ZHUYIN_FULLKEY:
            if (!bHasZhuyin)
            {
                nBlock += 2;
                bHasZhuyin = 1;
            }
            break;
        case IM_ZHUYIN_MT_SC:
        case IM_ZHUYIN_FULLKEY_SC:
            if (!bHasZhuyin_sc)
            {
                nBlock += 2;
                bHasZhuyin_sc = 1;
            }
            break;
        default:
           nBlock += 1;
        }
        i++;
    }

    return nBlock;
}

int FindUserCacheByIM(int iMethod, const IIME_USER_PARAMS *pUserTotal, IIME_USER_PARAMS *pUserIM)
{
    IIME_BOOL bHasPinyin, bHasZhuyin;
    IIME_BOOL bHasPinyin_tc, bHasZhuyin_sc;
    int iBlockPinyin, iBlockZhuyin;
    int iBlockPinyin_tc, iBlockZhuyin_sc;

    int i, iBlock, nBlock, szBlock;

    iMethod = FindBaseInputMethod(iMethod);

    pUserIM->iUserCacheSize = 0;
    pUserIM->pUserCache = 0;

    nBlock = SpliteCache();
    if (nBlock)
        szBlock = (pUserTotal->iUserCacheSize / nBlock) / MIN_CACHE_BLOCK * MIN_CACHE_BLOCK;
    else
        szBlock = 0;

    if (szBlock == 0)
        return 0;

/*
#ifdef _MSC_VER
    wsprintf(out, L"iMethod = %d, nBlock = %d, szBlock = %d\n", iMethod, nBlock, szBlock);
    OutputDebugString(out);
#endif
*/
    i = 0;
    iBlock = 0;
    bHasPinyin = 0;
    bHasZhuyin = 0;
    bHasPinyin_tc = 0;
    bHasZhuyin_sc = 0;
    iBlockPinyin = -1;
    iBlockZhuyin = -1;
    iBlockPinyin_tc = -1;
    iBlockZhuyin_sc = -1;

    if (iMethod >= IM_USER_MULTITYPE_BASE)
    {
        pUserIM->iUserCacheSize = 0;
        pUserIM->pUserCache = IIME_NULL;
        return 1;
    }

    if (iMethod == IM_PINYIN_SURETYPE || iMethod == IM_PINYIN_FULLKEY)
        iMethod = IM_PINYIN;
    if (iMethod == IM_PINYIN_TC + IM_SURETYPE_BASE || iMethod == IM_PINYIN_FULLKEY_TC)
        iMethod = IM_PINYIN_TC;

    if (iMethod == IM_ZHUYIN_FULLKEY)
        iMethod = IM_ZHUYIN_MT;
    if (iMethod == IM_ZHUYIN_FULLKEY_SC)
        iMethod = IM_ZHUYIN_MT_SC;

    while(IIME_input_method_config[i])
    {
        int orgBlock = iBlock;
/*
#ifdef _MSC_VER
        wsprintf(out, L"IIME_input_method_config[%d]->input_method = %d\n", i, IIME_input_method_config[i]->input_method);
        OutputDebugString(out);
#endif
*/
        switch (IIME_input_method_config[i]->input_method)
        {
        case IM_PINYIN_DZ:
            break;
        case IM_PINYIN:
        case IM_PINYIN + IM_SURETYPE_BASE:
        case IM_PINYIN_FULLKEY:
            if (!bHasPinyin)
            {
                iBlockPinyin = iBlock;
                iBlock += 2;
                bHasPinyin = 1;
            }
            break;
        case IM_PINYIN_TC:
        case IM_PINYIN_TC + IM_SURETYPE_BASE:
        case IM_PINYIN_FULLKEY_TC:
            if (!bHasPinyin_tc)
            {
                iBlockPinyin_tc = iBlock;
                iBlock += 2;
                bHasPinyin_tc = 1;
            }
            break;
        case IM_ZHUYIN:
        case IM_ZHUYIN_MT:
        case IM_ZHUYIN_FULLKEY:
            if (!bHasZhuyin)
            {
                iBlockZhuyin = iBlock;
                iBlock += 2;
                bHasZhuyin = 1;
            }
            break;
        case IM_ZHUYIN_MT_SC:
        case IM_ZHUYIN_FULLKEY_SC:
            if (!bHasZhuyin_sc)
            {
                iBlockZhuyin_sc = iBlock;
                iBlock += 2;
                bHasZhuyin_sc = 1;
            }
            break;
        default:
           iBlock += 1;
        }

        if (iMethod == IIME_input_method_config[i]->input_method)
        {
            iBlock = orgBlock;
            break;
        }

        i++;
    }

    if (IIME_input_method_config[i] == 0)
        return 0;

    switch(iMethod)
    {
    case IM_PINYIN_DZ:
        pUserIM->pUserCache = 0;
        pUserIM->iUserCacheSize = 0;
    case IM_PINYIN:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlockPinyin * szBlock;
        pUserIM->iUserCacheSize = 2 * szBlock;
        break;
    case IM_PINYIN_TC:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlockPinyin_tc * szBlock;
        pUserIM->iUserCacheSize = 2 * szBlock;
        break;
    case IM_ZHUYIN_MT:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlockZhuyin * szBlock;
        pUserIM->iUserCacheSize = 2 * szBlock;
        break;
    case IM_ZHUYIN_MT_SC:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlockZhuyin_sc * szBlock;
        pUserIM->iUserCacheSize = 2 * szBlock;
        break;

    default:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlock * szBlock;
        pUserIM->iUserCacheSize = szBlock;
    }
/*
#ifdef _MSC_VER
    wsprintf(out, L"iBlock = %d, iBlockPinyin = %d, iBlockZhuyin = %d\n", iBlock, iBlockPinyin, iBlockZhuyin);
    OutputDebugString(out);
    wsprintf(out, L"pCache = 0x%08x, szCache = 0x%08x\n", pUserIM->pUserCache, pUserIM->iUserCacheSize);
    OutputDebugString(out);
    wsprintf(out, L"pTotalCache = 0x%08x, szTotalCache = 0x%08x\n", pUserTotal->pUserCache, pUserTotal->iUserCacheSize);
    OutputDebugString(out);
#endif
*/
    return 1;
}


static IIME_BOOL IsMultiTap(IIME_INT idMethod)
{
    int i, j;

    i = idMethod / 10;
    j = idMethod % 10;

    if (idMethod >= IM_USER_MULTITYPE_BASE)
        return IIME_TRUE;

    if (idMethod == IM_123)
        return IIME_TRUE;

    if (i > 0 && 2 <= j && j <= 5)
        return IIME_TRUE;

    return IIME_FALSE;

}

static IIME_UINT	IMM_PreTranslateKey(IIME_IMM *pImm, IIME_UINT iKey)
{
    IIME_UINT idMethod = pImm->m_idMethod;

	/* 当输入法不是multitap时，非 backspace 的长按键被过滤掉 */
	if ((iKey & IIME_VK_LONGCLICK) && (iKey & 0xFFFF) != IIME_VK_BACKSPACE && !IsMultiTap(idMethod))
		iKey = iKey & 0xFFFF;


#ifdef KEYBOARD_IME_STAR
	if (idMethod == IM_ZHUYIN && iKey == IIME_VK_POUND)
		iKey = IIME_VK_SPACE;
#endif

	return iKey;
}

#if defined(IIME_MARS_SCRIPT)

#include "core/ldbapi2/kmxime_stroke.h"
#include "core/digitpinyin/digitpinyinconst.h"

static int binary_search_earth(const unsigned short *table, int len, unsigned short value)
{
    if (table != 0 && len > 0)
    {
        int beg, end, mid;

        beg = 0;
        end = len - 1;

        while (beg <= end)
        {
            mid = (beg + end) / 2;
            if (value < table[mid])
                end = mid - 1;
            else if (value > table[mid])
                beg = mid + 1;
            else
                return mid;
        }
    }

    return -1;
}

static int IIME_Earth2Mars(unsigned short *buf, const unsigned short* earth, const unsigned short *mars, int table_len)
{
    int len = 0;
    while(*buf)
    {
        int i = binary_search_earth(earth, table_len, *buf);
        if (i > 0)
            *buf = mars[i];
        buf++;
        len++;
    }

    return len;
}

static void IMM_PostTranslateCand(IIME_IMM *pImm, IIME_IMM_CONTENT *pContent)
{
    int code = 0;
    unsigned short *pBuffer;
    const unsigned short *earth, *mars;
    int len;

    if (pImm->m_pOptions->dwMarScript == 0)
    {
        return;
    }

    if (pImm->m_idMethod == IM_PINYIN || 
        pImm->m_idMethod == IM_PINYIN_SURETYPE || 
        pImm->m_idMethod == IM_PINYIN_FULLKEY ||
        pImm->m_idMethod == IM_PINYIN_DZ)
    {
        const void *pdbMisc = FindDataBase(pImm->m_idMethod);
        const KMX_PINYIN_DB *pdb = (const KMX_PINYIN_DB *)pdbMisc;


        if ((pdb->nFormat & KMX_DB_FLAG_GB2312) || (pdb->nFormat & KMX_DB_FLAG_COMPACT))
        {
            code = 1;
        }
        else
        {
            code = 2;
        }
    }
    else if (pImm->m_idMethod == IM_STROKE)
    {
        const void *pdbMisc = FindDataBase(pImm->m_idMethod);
        const LDB_HEADER* ldb = (const LDB_HEADER*)pdbMisc;

        if (ldb->did == 1000 || ldb->did == 1010 || ldb->did == 1030)
        {
            code = 1;
        }
        else if (ldb->did == 1001 || ldb->did == 1031)
        {
            code = 2;
        }
    }

    if (code == 1)
    {
        earth = earth_gb2312;
        mars = mars_gb2312;
        len = earth_mars_gb2312_length;
    }
    else if (code == 2)
    {
        earth = earth_gbk;
        mars = mars_gbk;
        len = earth_mars_gbk_length;
    }
    else
        return;

    pBuffer = pContent->pCommit;
    IIME_Earth2Mars(pBuffer, earth, mars, len);
}

#endif /* defined(IIME_MARS_SCRIPT) */

static void SetOutputNumber(IIME_IMM *pImm)
{
#if defined(KMX_SP)
    if (pImm->m_nOutput)
    {
        IIME_IMM_SetOutputNumber(pImm->m_nOutput);
        pImm->m_nOutput = 0;
    }
#endif
}

/*
 Get Version
 */

#ifndef IIME_PID_VERMAJOR
#define IIME_PID_VERMAJOR	0
#endif

#ifndef IIME_PID_VERMINOR
#define IIME_PID_VERMINOR 00
#endif

#ifndef IIME_PID_BUILDER
#define IIME_PID_BUILDER 0000
#endif

#define STR(x) # x
#define XSTR(x) STR(x)

#ifdef IIME_PID_VERMAJOR
#undef IIME_PID_VERMAJOR
#endif

#define IIME_PID_VERMAJOR  (1)



#ifdef IIME_PID_VERMINOR
#undef IIME_PID_VERMINOR
#endif

#define IIME_PID_VERMINOR  (05)



#ifdef IIME_PID_BUILDER
#undef IIME_PID_BUILDER
#endif

#define IIME_PID_BUILDER  (0120)	



static const char pVersion[] = "IIME Version " XSTR(IIME_PID_VERMAJOR) "." 
							XSTR(IIME_PID_VERMINOR) " [Build " XSTR(IIME_PID_BUILDER) "]";

const char* IIME_IMM_GetVersion(void)
{
    return pVersion;
}
//#ifndef IIME_DIGIT_ONLY
#ifdef IIME_PINYIN_SUPPORT

#include "digitpinyinconst.h"

static const char *pinyin_charset[] = 
{
    "gb2312",
    "gbk",
    "gb18030"
};

const char* IIME_IMM_GetPinyinCharset(const IIME_IME_INFO* ime_info)
{
    if (ime_info->input_method == IM_PINYIN || ime_info->input_method == IM_PINYIN + IM_SURETYPE_BASE)
    {
        const KMX_PINYIN_DB *pdb = ime_info->language_database;
        if (pdb->nFormat & KMX_DB_FLAG_CHARSET_GB2312)
        {
            return pinyin_charset[0];
        }
        else if (pdb->nFormat & KMX_DB_FLAG_CHARSET_GBK)
        {
            return pinyin_charset[1];
        }
        else if (pdb->nFormat & KMX_DB_FLAG_CHARSET_GB18030)
        {
            return pinyin_charset[2];
        }
    }

    return IIME_NULL;
}

/* 
  Query Association 
  */
#include "core/digitpinyin/digitpinyinconst.h"

extern IIME_RESULT   HanziAsso_QueryAsso_ByDB(const KMX_PINYIN_DB *pdb,
                                            IIME_WCHAR wContext, 
                                            IIME_PWSTR pwszAsso,
                                            IIME_INT *pnAsso);

static const void* search_database(int input_method)
{
	int i;
	for (i = 0; IIME_input_method_config[i]; i++)
    {
		if (IIME_input_method_config[i]->input_method == input_method)
			return IIME_input_method_config[i]->language_database;
	}

	return IIME_NULL;
}

static const KMX_PINYIN_DB* HanziAsso_QueryDB(IIME_BOOL isPinyin)
{
	IIME_UINT idMethod;
	const KMX_PINYIN_DB* pdb = IIME_NULL;

	if (isPinyin){
		idMethod = IM_PINYIN;
		pdb = search_database(idMethod);

        if (!pdb){
            idMethod = IM_PINYIN_SURETYPE;
            pdb = search_database(idMethod);
        }

        if (!pdb){
            idMethod = IM_PINYIN_DZ;
            pdb = search_database(idMethod);
        }
	}
	else{
		idMethod = IM_ZHUYIN;

		pdb = search_database(idMethod);

		if (!pdb){
			idMethod = IM_ZHUYIN_MT;
			pdb = search_database(idMethod);
		}

	}

    return pdb;
}

#if defined(IIME_PINYIN) || defined(IIME_ZHUYIN) || defined(IIME_PINYIN_MINI) || defined(IIME_ZHUYIN_MINI)

IIME_RESULT   HanziAsso_QueryAsso(IIME_BOOL  isPinyin,
                                IIME_WCHAR wContext, 
                                IIME_PWSTR pwszAsso,
                                IIME_INT *pnAsso)
{
    const void *pdb = HanziAsso_QueryDB(isPinyin);
    if (pdb == IIME_NULL)
        return IIME_ERROR_UNKNOWN;

    return HanziAsso_QueryAsso_ByDB(pdb, wContext, pwszAsso, pnAsso);
}

/* 
  Query pinyin zhuyin 
  */
extern IIME_BOOL QueryPinyinZhuyinByDB(const KMX_PINYIN_DB *pdb, IIME_WCHAR wCode, IIME_WCHAR *wszPinyin, int maxDyz);

IIME_BOOL IIME_IMM_QueryCodePinyin(IIME_WORD wCode, IIME_WCHAR *wszPinyin)
{
    const void *pdb = HanziAsso_QueryDB(IIME_TRUE);
    if (pdb == IIME_NULL)
        return IIME_ERROR_UNKNOWN;

    if(QueryPinyinZhuyinByDB(pdb, wCode, wszPinyin, 1))
#if defined(LEADCORE_PRODUCT)
        return IIME_TRUE;
#else
        return IIME_ERROR_SUCCESS;
#endif
    else
#if defined(LEADCORE_PRODUCT)
        return IIME_FALSE;
#else
        return IIME_ERROR_UNKNOWN;
#endif
}

IIME_BOOL IIME_IMM_QueryCodeZhuyin(IIME_WORD wCode, IIME_WCHAR *wszZhuyin)
{
    const void *pdb = HanziAsso_QueryDB(IIME_FALSE);
    if (pdb == IIME_NULL)
        return IIME_ERROR_UNKNOWN;

    if(QueryPinyinZhuyinByDB(pdb, wCode, wszZhuyin, 1))
#if defined(LEADCORE_PRODUCT)
        return IIME_TRUE;
#else
        return IIME_ERROR_SUCCESS;
#endif
    else
#if defined(LEADCORE_PRODUCT)
        return IIME_FALSE;
#else
        return IIME_ERROR_UNKNOWN;
#endif
}
#endif
/*
 Query Stroke
 */

#include "inc/kmxime.h"
static int query_stroke(
    const void*     database,
    unsigned short  char_code,
    char*           buffer,
    int             buffer_length)
{
    int result;
    int i;

    result = kmx_chinese_query_stroke(database, char_code, buffer, (unsigned short)buffer_length);

    for (i = 0; i < result; i++)
        buffer[i] = (char)(buffer[i] - 0x30);

    return result;
}

int IIME_IMM_QueryStroke_gb(
    unsigned short  char_code,
    char*           buffer,
    int             buffer_length)
{
    const void* database = search_database(IM_STROKE);

    return query_stroke(database, char_code, buffer, buffer_length);
}

int IIME_IMM_QueryStroke_big5(
    unsigned short  char_code,
    char*           buffer,
    int             buffer_length)
{
    const void* database = search_database(IM_STROKE_BIG5);

    return query_stroke(database, char_code, buffer, buffer_length);
}

#endif


/*
 Add Phrase
 */
IIME_RESULT IIME_IMM_AddPhase(IIME_HIMM hImm, unsigned short *szPhrase, int nLen)
{
	PROVIDER_PRECALL();
/*
    if (pImm->m_idMethod != IM_PINYIN && pImm->m_idMethod != IM_ZHUYIN_MT) 
        return IIME_ERROR_UNKNOWN;
*/
    ret = (*(pProvider->m_pfnAddPhrase))(pProvider, szPhrase, nLen);

    return ret;
}

IIME_RESULT IIME_IMM_CheckAsso(IIME_HIMM hImm, unsigned short wContext, unsigned short *szAsso, int *pNumber)
{
	PROVIDER_PRECALL();

    if (pImm->m_idMethod != IM_PINYIN && pImm->m_idMethod != IM_ZHUYIN_MT)
        return IIME_ERROR_UNKNOWN;

    ret = (*(pProvider->m_pfnCheckAsso))(pProvider, wContext, szAsso, pNumber);

    return ret;
}

IIME_RESULT IIME_IMM_QueryProperties(IIME_HIMM hImm, int *pProperties)
{
	PROVIDER_PRECALL();

    if (pImm->m_idMethod != IM_STROKE && pImm->m_idMethod != IM_STROKE_BIG5)
        return IIME_ERROR_UNKNOWN;

    ret = (*(pProvider->m_pfnQueryProperties))(pProvider, pProperties);

    return ret;
}

IIME_BOOL IIME_IME_IsShowPreEditWin(IIME_UINT method)
{
    return IIME_FALSE;
}