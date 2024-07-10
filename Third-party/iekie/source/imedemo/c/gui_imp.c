
/*
 * gui.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "gui.h"
#include "gui_imp.h"
#include "mainmenu.h"
#include "guidebug.h"
#include "guizoom.h"
#include "userdata.h"
#include "IIMEk2api.h"
#include "kbd.h"
#include "ime_ldb.h"

#include "kmx_all_ldb.h"

#include "resource.h"

static int default_lang = 0;

#define GUI_IME_TIMER_ID        200
#define GUI_MAX_INPUT_METHOD    16

static int input_method_list[][GUI_MAX_INPUT_METHOD] = 
{
    /* albanian */
    {
        IM_ABC_LEAD_UPPER_SQ,
        IM_ABC_LOWER_SQ,
        IM_ABC_UPPER_SQ,
        IM_ABC_SQ,
    },

    {
        IM_AMHARIC_MT,
        IM_AMHARIC_MT + IM_FULLKEY_BASE,
    },

    {
        IM_ARABIC,
        IM_ARABIC_MT,
        IM_ARABIC_123,
        IM_FULLKEY_BASE + IM_ARABIC,
        IM_FULLKEY_BASE + IM_ARABIC_MT,
    },

    {
        IM_ARMENIAN,
        IM_ARMENIAN_UPPER,
        IM_ARMENIAN_LOWER,
        IM_ABC_LEAD_UPPER_HY,
        IM_ABC_LOWER_HY,
        IM_ABC_UPPER_HY,
        IM_ABC_HY,
		IM_LATINFULLKEY_BASE + IM_ARMENIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_ARMENIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_ARMENIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_HY,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_HY,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_HY,
        IM_LATINFULLKEY_BASE + IM_ABC_HY,
    },

    {
        IM_ASSAMESE_MT,
        IM_ASSAMESE_123,
        IM_LATINFULLKEY_BASE + IM_ASSAMESE_MT,
    },

    /* basque */
    {
        IM_ABC_LEAD_UPPER_EU,
        IM_ABC_LOWER_EU,
        IM_ABC_UPPER_EU,
        IM_ABC_EU,
    },

    {
        IM_BENGALI,
        IM_BENGALI_MT,
        IM_BENGALI_123,
        IM_LATINFULLKEY_BASE + IM_BENGALI,
        IM_LATINFULLKEY_BASE + IM_BENGALI_MT,
    },

    {
        IM_BULGARIAN,
        IM_BULGARIAN_UPPER,
        IM_BULGARIAN_LOWER,
        IM_ABC_LEAD_UPPER_BG,
        IM_ABC_LOWER_BG,
        IM_ABC_UPPER_BG,
        IM_ABC_BG,
		IM_LATINFULLKEY_BASE + IM_BULGARIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_BULGARIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_BULGARIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_BG,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_BG,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_BG,
        IM_LATINFULLKEY_BASE + IM_ABC_BG,
    },

    /* Catalan */
    {
        IM_ABC_LEAD_UPPER_CA,
        IM_ABC_LOWER_CA,
        IM_ABC_UPPER_CA,
        IM_ABC_CA,
    },

    /* Cebuano */
    {
        IM_ABC_LEAD_UPPER_CEB,
        IM_ABC_LOWER_CEB,
        IM_ABC_UPPER_CEB,
        IM_ABC_CEB,
    },

    {
#if defined(IIME_PINYIN_MINI)
        IM_PINYIN_DZ,
        IM_PINYIN_DZ + IM_FULLKEY_BASE,
#else
        IM_PINYIN,
        IM_PINYIN + IM_FULLKEY_BASE,
#endif
        IM_STROKE,
        IM_STROKE + IM_FULLKEY_BASE,
        IM_123,
        IM_USER_MULTITYPE_BASE + 1,
#if defined(LEADCORE_PRODUCT)
        IM_NUMBER,
#endif
    },

    {
        IM_ZHUYIN_MT,
        IM_ZHUYIN_MT_FULL,
        IM_ZHUYIN_FULLKEY,
        IM_ZHUYIN + IM_FULLKEY_BASE,
        IM_ZHUYIN_2 + IM_FULLKEY_BASE,

        IM_STROKE_BIG5,
        IM_STROKE_BIG5 + IM_FULLKEY_BASE,
        IM_123,
    },

    {
        IM_CROATIAN,
        IM_CROATIAN_UPPER,
        IM_CROATIAN_LOWER,
        IM_ABC_LEAD_UPPER_HR,
        IM_ABC_LOWER_HR,
        IM_ABC_UPPER_HR,
        IM_ABC_HR,
		IM_LATINFULLKEY_BASE + IM_CROATIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_CROATIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_CROATIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_HR,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_HR,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_HR,
        IM_LATINFULLKEY_BASE + IM_ABC_HR,
    },

    {
        IM_CZECH,
        IM_CZECH_UPPER,
        IM_CZECH_LOWER,
        IM_ABC_LEAD_UPPER_CS,
        IM_ABC_LOWER_CS,
        IM_ABC_UPPER_CS,
        IM_ABC_CS,
		IM_LATINFULLKEY_BASE + IM_CZECH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_CZECH_UPPER,
		IM_LATINFULLKEY_BASE + IM_CZECH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_CS,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_CS,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_CS,
        IM_LATINFULLKEY_BASE + IM_ABC_CS,
    },

    {
        IM_DANISH,
        IM_DANISH_UPPER,
        IM_DANISH_LOWER,
        IM_ABC_LEAD_UPPER_DA,
        IM_ABC_LOWER_DA,
        IM_ABC_UPPER_DA,
        IM_ABC_DA,
		IM_LATINFULLKEY_BASE + IM_DANISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_DANISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_DANISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_DA,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_DA,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_DA,
        IM_LATINFULLKEY_BASE + IM_ABC_DA,
    },

    {
        IM_DUTCH,
        IM_DUTCH_UPPER,
        IM_DUTCH_LOWER,
        IM_ABC_LEAD_UPPER_NL,
        IM_ABC_LOWER_NL,
        IM_ABC_UPPER_NL,
        IM_ABC_NL,
		IM_LATINFULLKEY_BASE + IM_DUTCH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_DUTCH_UPPER,
		IM_LATINFULLKEY_BASE + IM_DUTCH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_NL,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_NL,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_NL,
        IM_LATINFULLKEY_BASE + IM_ABC_NL,
    },

    {
        IM_ENGLISH,
        IM_ENGLISH_UPPER,
        IM_ENGLISH_LOWER,
        IM_ABC,
        IM_ABC_LEAD_UPPER,
        IM_ABC_LOWER,
        IM_ABC_UPPER,
        IM_FULLKEY_BASE + IM_ENGLISH,
		IM_LATINFULLKEY_BASE + IM_ENGLISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_ENGLISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_ENGLISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER,
    },

    {
        IM_FINNISH,
        IM_FINNISH_UPPER,
        IM_FINNISH_LOWER,
        IM_ABC_LEAD_UPPER_FI,
        IM_ABC_LOWER_FI,
        IM_ABC_UPPER_FI,
        IM_ABC_FI,
		IM_LATINFULLKEY_BASE + IM_FINNISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_FINNISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_FINNISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_FI,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_FI,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_FI,
        IM_LATINFULLKEY_BASE + IM_ABC_FI,
    },

    {
        IM_FRENCH,
        IM_FRENCH_UPPER,
        IM_FRENCH_LOWER,
        IM_ABC_LEAD_UPPER_FR,
        IM_ABC_LOWER_FR,
        IM_ABC_UPPER_FR,
        IM_ABC_FR,
		IM_LATINFULLKEY_BASE + IM_FRENCH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_FRENCH_UPPER,
		IM_LATINFULLKEY_BASE + IM_FRENCH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_FR,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_FR,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_FR,
        IM_LATINFULLKEY_BASE + IM_ABC_FR,
    },

    /* Galician */
    {
        IM_ABC_LEAD_UPPER_GL,
        IM_ABC_LOWER_GL,
        IM_ABC_UPPER_GL,
        IM_ABC_GL,
    },

    /* Georgian */
    {
        IM_ABC_LEAD_UPPER_KA,
        IM_ABC_LOWER_KA,
        IM_ABC_UPPER_KA,
        IM_ABC_KA,
    },

    {
        IM_GERMAN,
        IM_GERMAN_UPPER,
        IM_GERMAN_LOWER,
        IM_ABC_LEAD_UPPER_DE,
        IM_ABC_LOWER_DE,
        IM_ABC_UPPER_DE,
        IM_ABC_DE,
		IM_LATINFULLKEY_BASE + IM_GERMAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_GERMAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_GERMAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_DE,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_DE,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_DE,
        IM_LATINFULLKEY_BASE + IM_ABC_DE,
    },

    {
        IM_GREEK,
        IM_GREEK_UPPER,
        IM_GREEK_LOWER,
        IM_ABC_LEAD_UPPER_EL,
        IM_ABC_LOWER_EL,
        IM_ABC_UPPER_EL,
        IM_ABC_EL,
		IM_LATINFULLKEY_BASE + IM_GREEK_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_GREEK_UPPER,
		IM_LATINFULLKEY_BASE + IM_GREEK_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_EL,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_EL,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_EL,
        IM_LATINFULLKEY_BASE + IM_ABC_EL,
    },

    {
        IM_GUJARATI_MT,
        IM_GUJARATI_MT + IM_FULLKEY_BASE,
    },

    {
        IM_HANGUL_MT,
    },

    {
        IM_HEBREW,
        IM_HEBREW_MT,
        IM_HEBREW + IM_FULLKEY_BASE,
        IM_HEBREW_MT + IM_FULLKEY_BASE
    },

    {
        IM_HINDI,
        IM_HINDI_MT,
        IM_HINDI_123,
        IM_LATINFULLKEY_BASE + IM_HINDI,
        IM_LATINFULLKEY_BASE + IM_HINDI_MT,
        IM_FULLKEY_BASE + IM_HINDI_123,
    },

    {
        IM_HINGLISH,
        IM_HINGLISH_UPPER,
        IM_HINGLISH_LOWER,
        IM_ABC_LEAD_UPPER_HIN,
        IM_ABC_LOWER_HIN,
        IM_ABC_UPPER_HIN,
        IM_ABC_HIN,
		IM_LATINFULLKEY_BASE + IM_HINGLISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_HINGLISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_HINGLISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_HIN,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_HIN,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_HIN,
        IM_LATINFULLKEY_BASE + IM_ABC_HIN,
    },

    {
        IM_HUNGARIAN,
        IM_HUNGARIAN_UPPER,
        IM_HUNGARIAN_LOWER,
        IM_ABC_LEAD_UPPER_HU,
        IM_ABC_LOWER_HU,
        IM_ABC_UPPER_HU,
        IM_ABC_HU,
		IM_LATINFULLKEY_BASE + IM_HUNGARIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_HUNGARIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_HUNGARIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_HU,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_HU,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_HU,
        IM_LATINFULLKEY_BASE + IM_ABC_HU,
    },

    /* Igbo */
    {
        IM_ABC_LEAD_UPPER_IG,
        IM_ABC_LOWER_IG,
        IM_ABC_UPPER_IG,
        IM_ABC_IG,
    },

    {
        IM_INDONESIAN,
        IM_INDONESIAN_UPPER,
        IM_INDONESIAN_LOWER,
        IM_ABC_LEAD_UPPER_ID,
        IM_ABC_LOWER_ID,
        IM_ABC_UPPER_ID,
        IM_ABC_ID,
		IM_LATINFULLKEY_BASE + IM_INDONESIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_INDONESIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_INDONESIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_ID,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_ID,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_ID,
        IM_LATINFULLKEY_BASE + IM_ABC_ID,
    },

    {
        IM_ITALIAN,
        IM_ITALIAN_UPPER,
        IM_ITALIAN_LOWER,
        IM_ABC_LEAD_UPPER_IT,
        IM_ABC_LOWER_IT,
        IM_ABC_UPPER_IT,
        IM_ABC_IT,
		IM_LATINFULLKEY_BASE + IM_ITALIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_ITALIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_ITALIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_IT,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_IT,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_IT,
        IM_LATINFULLKEY_BASE + IM_ABC_IT,
    },

    {
        IM_KANNADA_MT,
        IM_KANNADA_MT + IM_FULLKEY_BASE,
    },

    /* Kazakh */
    {
        IM_ABC_LEAD_UPPER_KK,
        IM_ABC_LOWER_KK,
        IM_ABC_UPPER_KK,
        IM_ABC_KK,
    },

    {
        IM_KHMER_MT,
        IM_KHMER_MT + IM_FULLKEY_BASE,
        IM_KHMER_123,
    },

    {
        IM_ABC_LEAD_UPPER_KY,
        IM_ABC_LOWER_KY,
        IM_ABC_UPPER_KY,
        IM_ABC_KY,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_KY,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_KY,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_KY,
        IM_LATINFULLKEY_BASE + IM_ABC_KY,
    },

    {
        IM_LAO_MT,
        IM_LAO_MT + IM_FULLKEY_BASE,
    },

    {
        IM_LITHUANIAN,
        IM_LITHUANIAN_UPPER,
        IM_LITHUANIAN_LOWER,
        IM_ABC_LEAD_UPPER_LT,
        IM_ABC_LOWER_LT,
        IM_ABC_UPPER_LT,
        IM_ABC_LT,
		IM_LATINFULLKEY_BASE + IM_LITHUANIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_LITHUANIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_LITHUANIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_LT,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_LT,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_LT,
        IM_LATINFULLKEY_BASE + IM_ABC_LT,
    },

    {
        IM_MALAY,
        IM_MALAY_UPPER,
        IM_MALAY_LOWER,
        IM_ABC_LEAD_UPPER_MS,
        IM_ABC_LOWER_MS,
        IM_ABC_UPPER_MS,
        IM_ABC_MS,
		IM_LATINFULLKEY_BASE + IM_MALAY_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_MALAY_UPPER,
		IM_LATINFULLKEY_BASE + IM_MALAY_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_MS,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_MS,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_MS,
        IM_LATINFULLKEY_BASE + IM_ABC_MS,
    },

    {
        IM_MALAYALAM_MT,
        IM_MALAYALAM_MT + IM_FULLKEY_BASE,
    },

    {    
        IM_MARATHI_MT,
        IM_MARATHI_MT + IM_FULLKEY_BASE,
    },

    {    
        IM_MYANMAR_MT,
        IM_MYANMAR_MT + IM_FULLKEY_BASE,
    },

    {
        IM_NEPALI_MT,
        IM_NEPALI_123,
        IM_NEPALI_MT + IM_FULLKEY_BASE,
    },
	//daodejing
	{
		IM_BODO_MT,
		IM_BODO_123,
		IM_BODO_MT + IM_FULLKEY_BASE,
	},
	{
		IM_DOGRI_MT,
		IM_DOGRI_123,
		IM_DOGRI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_SANSKRIT_MT,
		IM_SANSKRIT_123,
		IM_SANSKRIT_MT + IM_FULLKEY_BASE,
	},
	{
		IM_KASHMIRI_MT,
		IM_KASHMIRI_123,
		IM_KASHMIRI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_KONKANI_MT,
		IM_KONKANI_123,
		IM_KONKANI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_MAITHILI_MT,
		IM_MAITHILI_123,
		IM_MAITHILI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_MANIPURI_MT,
		IM_MANIPURI_123,
		IM_MANIPURI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_SANTALI_MT,
		IM_SANTALI_123,
		IM_SANTALI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_SOMALI_MT,
		IM_SOMALI_123,
		IM_SOMALI_MT + IM_FULLKEY_BASE,
	},
	{
		IM_SINDHI_MT,
		IM_SINDHI_123,
		IM_SINDHI_MT + IM_FULLKEY_BASE,
	},
    {
        IM_NORWEGIAN,
        IM_NORWEGIAN_UPPER,
        IM_NORWEGIAN_LOWER,
        IM_ABC_LEAD_UPPER_RO,
        IM_ABC_LOWER_RO,
        IM_ABC_UPPER_RO,
        IM_ABC_RO,
		IM_LATINFULLKEY_BASE + IM_NORWEGIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_NORWEGIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_NORWEGIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_RO,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_RO,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_RO,
        IM_LATINFULLKEY_BASE + IM_ABC_RO,
    },

    {
        IM_ORIYA_MT,
        IM_ORIYA_MT + IM_FULLKEY_BASE,
    },

    /* Oromo */
    {
        IM_ABC_LEAD_UPPER_OM,
        IM_ABC_LOWER_OM,
        IM_ABC_UPPER_OM,
        IM_ABC_OM,
    },

    {
        IM_PERSIAN,
        IM_PERSIAN_MT,
        IM_PERSIAN_123,
        IM_PERSIAN + IM_FULLKEY_BASE,
        IM_PERSIAN_MT + IM_FULLKEY_BASE,
    },

    {
        IM_POLISH,
        IM_POLISH_UPPER,
        IM_POLISH_LOWER,
        IM_ABC_LEAD_UPPER_PL,
        IM_ABC_LOWER_PL,
        IM_ABC_UPPER_PL,
        IM_ABC_PL,
		IM_LATINFULLKEY_BASE + IM_POLISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_POLISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_POLISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_PL,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_PL,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_PL,
        IM_LATINFULLKEY_BASE + IM_ABC_PL,
    },

    {
        IM_PORTUGUESE,
        IM_PORTUGUESE_UPPER,
        IM_PORTUGUESE_LOWER,
        IM_ABC_LEAD_UPPER_PT,
        IM_ABC_LOWER_PT,
        IM_ABC_UPPER_PT,
        IM_ABC_PT,
		IM_LATINFULLKEY_BASE + IM_PORTUGUESE_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_PORTUGUESE_UPPER,
		IM_LATINFULLKEY_BASE + IM_PORTUGUESE_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_PT,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_PT,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_PT,
        IM_LATINFULLKEY_BASE + IM_ABC_PT,
    },

    {
        IM_PUNJABI_MT,
        IM_PUNJABI_MT + IM_FULLKEY_BASE,
    },

    {
        IM_ROMANIAN,
        IM_ROMANIAN_UPPER,
        IM_ROMANIAN_LOWER,
        IM_ABC_LEAD_UPPER_RO,
        IM_ABC_LOWER_RO,
        IM_ABC_UPPER_RO,
        IM_ABC_RO,
		IM_LATINFULLKEY_BASE + IM_ROMANIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_ROMANIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_ROMANIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_RO,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_RO,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_RO,
        IM_LATINFULLKEY_BASE + IM_ABC_RO,
    },

    {
        IM_RUSSIAN,
        IM_RUSSIAN_UPPER,
        IM_RUSSIAN_LOWER,
        IM_ABC_LEAD_UPPER_RUS,
        IM_ABC_LOWER_RUS,
        IM_ABC_UPPER_RUS,
        IM_ABC_RUS,
		IM_LATINFULLKEY_BASE + IM_RUSSIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_RUSSIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_RUSSIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_RUS,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_RUS,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_RUS,
        IM_LATINFULLKEY_BASE + IM_ABC_RUS,
    },

    /* serbian */
    {
        IM_ABC_LEAD_UPPER_SR,
        IM_ABC_LOWER_SR,
        IM_ABC_UPPER_SR,
        IM_ABC_SR,
    },

    /* serbian cyrillic */
    {
        IM_ABC_LEAD_UPPER_SRC,
        IM_ABC_LOWER_SRC,
        IM_ABC_UPPER_SRC,
        IM_ABC_SRC,
    },

    {
        IM_SLOVAK,
        IM_SLOVAK_UPPER,
        IM_SLOVAK_LOWER,
        IM_ABC_LEAD_UPPER_SK,
        IM_ABC_LOWER_SK,
        IM_ABC_UPPER_SK,
        IM_ABC_SK,
		IM_LATINFULLKEY_BASE + IM_SLOVAK_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_SLOVAK_UPPER,
		IM_LATINFULLKEY_BASE + IM_SLOVAK_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_SK,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_SK,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_SK,
        IM_LATINFULLKEY_BASE + IM_ABC_SK,
    },

    {
        IM_SLOVENIAN,
        IM_SLOVENIAN_UPPER,
        IM_SLOVENIAN_LOWER,
        IM_ABC_LEAD_UPPER_SL,
        IM_ABC_LOWER_SL,
        IM_ABC_UPPER_SL,
        IM_ABC_SL,
		IM_LATINFULLKEY_BASE + IM_SLOVENIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_SLOVENIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_SLOVENIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_SL,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_SL,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_SL,
        IM_LATINFULLKEY_BASE + IM_ABC_SL,
    },

    {
        IM_SPANISH,
        IM_SPANISH_UPPER,
        IM_SPANISH_LOWER,
        IM_ABC_LEAD_UPPER_ES,
        IM_ABC_LOWER_ES,
        IM_ABC_UPPER_ES,
        IM_ABC_ES,
		IM_LATINFULLKEY_BASE + IM_SPANISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_SPANISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_SPANISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_ES,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_ES,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_ES,
        IM_LATINFULLKEY_BASE + IM_ABC_ES,
    },

    {
        IM_SWAHILI,
        IM_SWAHILI_UPPER,
        IM_SWAHILI_LOWER,
        IM_ABC_LEAD_UPPER_SW,
        IM_ABC_LOWER_SW,
        IM_ABC_UPPER_SW,
        IM_ABC_SW,
		IM_LATINFULLKEY_BASE + IM_SWAHILI_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_SWAHILI_UPPER,
		IM_LATINFULLKEY_BASE + IM_SWAHILI_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_SW,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_SW,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_SW,
        IM_LATINFULLKEY_BASE + IM_ABC_SW,
    },

    {
        IM_SWEDISH,
        IM_SWEDISH_UPPER,
        IM_SWEDISH_LOWER,
        IM_ABC_LEAD_UPPER_SV,
        IM_ABC_LOWER_SV,
        IM_ABC_UPPER_SV,
        IM_ABC_SV,
		IM_LATINFULLKEY_BASE + IM_SWEDISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_SWEDISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_SWEDISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_SV,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_SV,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_SV,
        IM_LATINFULLKEY_BASE + IM_ABC_SV,
    },

    {
        IM_ABC_LOWER_TL,
        IM_ABC_UPPER_TL,
        IM_ABC_LEAD_UPPER_TL,
        IM_ABC_TL,
        IM_ABC_LOWER_TL + IM_FULLKEY_BASE,
        IM_ABC_UPPER_TL + IM_FULLKEY_BASE,
        IM_ABC_LEAD_UPPER_TL + IM_FULLKEY_BASE,
        IM_ABC_TL + IM_FULLKEY_BASE,
    },

    {
        IM_TAMIL_MT,
        IM_TAMIL_MT + IM_FULLKEY_BASE,
    },

    {
        IM_TELUGU_MT,
        IM_TELUGU_MT + IM_FULLKEY_BASE,
    },

    {
        IM_THAI,
        IM_THAI + IM_FULLKEY_BASE,
	    IM_THAI_MT,
	    IM_THAI_MT + IM_FULLKEY_BASE, 
    },

    {
        IM_TIBETAN_MT,        
    },

    {
        IM_TURKISH,
        IM_TURKISH_UPPER,
        IM_TURKISH_LOWER,
        IM_ABC_LEAD_UPPER_TR,
        IM_ABC_LOWER_TR,
        IM_ABC_UPPER_TR,
        IM_ABC_TR,
		IM_LATINFULLKEY_BASE + IM_TURKISH_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_TURKISH_UPPER,
		IM_LATINFULLKEY_BASE + IM_TURKISH_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_TR,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_TR,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_TR,
        IM_LATINFULLKEY_BASE + IM_ABC_TR,
    },

    {
        IM_UKRAINIAN,
        IM_UKRAINIAN_UPPER,
        IM_UKRAINIAN_LOWER,
        IM_ABC_LEAD_UPPER_UK,
        IM_ABC_LOWER_UK,
        IM_ABC_UPPER_UK,
        IM_ABC_UK,
		IM_LATINFULLKEY_BASE + IM_UKRAINIAN_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_UKRAINIAN_UPPER,
		IM_LATINFULLKEY_BASE + IM_UKRAINIAN_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_UK,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_UK,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_UK,
        IM_LATINFULLKEY_BASE + IM_ABC_UK,
    },

    {
        IM_URDU,
        IM_URDU_MT,
        IM_URDU + IM_FULLKEY_BASE,
        IM_URDU_MT + IM_FULLKEY_BASE,
    },

    {
        IM_UYGHUR,
        IM_UYGHUR_MT,
        IM_UYGHUR + IM_FULLKEY_BASE,
        IM_UYGHUR_MT + IM_FULLKEY_BASE,
    },

    /* Uzbek */
    {
        IM_ABC_LEAD_UPPER_UZ,
        IM_ABC_LOWER_UZ,
        IM_ABC_UPPER_UZ,
        IM_ABC_UZ,
    },

    {
        IM_VIETNAMESE,
        IM_VIETNAMESE_UPPER,
        IM_VIETNAMESE_LOWER,
        IM_ABC_LEAD_UPPER_VI,
        IM_ABC_LOWER_VI,
        IM_ABC_UPPER_VI,
        IM_ABC_VI,
		IM_LATINFULLKEY_BASE + IM_VIETNAMESE_LEAD_UPPER,
		IM_LATINFULLKEY_BASE + IM_VIETNAMESE_UPPER,
		IM_LATINFULLKEY_BASE + IM_VIETNAMESE_LOWER,
        IM_LATINFULLKEY_BASE + IM_ABC_LEAD_UPPER_VI,
        IM_LATINFULLKEY_BASE + IM_ABC_LOWER_VI,
        IM_LATINFULLKEY_BASE + IM_ABC_UPPER_VI,
        IM_LATINFULLKEY_BASE + IM_ABC_VI,
    },

    /* Zulu */
    {
        IM_ABC_LEAD_UPPER_ZU,
        IM_ABC_LOWER_ZU,
        IM_ABC_UPPER_ZU,
        IM_ABC_ZU,
    },


//    IM_123,
#if defined(LEADCORE_PRODUCT)
//    IM_NUMBER,
#endif
//    IM_USER_MULTITYPE_BASE + 1,
};

IIME_IME_INFO_PTR IIME_input_method_config[] =
{
#if defined(IIME_PINYIN) 
    //&IIME_ldb_pinyin_dynamic,
    //&IIME_ldb_qpy_gb2312_c1_20k,
    &IIME_ldb_pinyin_cizu,
#endif
#if defined(PY_GB2312_LEVEL1_20K_SURETYPE) || defined(PY_GBK_LEVEL1_20K_SURETYPE) || defined(PY_GB18030_LEVEL1_20K_SURETYPE)
    &IIME_ldb_pinyin_suretype,
#endif

#if defined(PY_BIG5_LEVEL1_20K_DIGIT)
    &IIME_ldb_pinyin_tc,
#endif
#if defined(PY_BIG5_LEVEL1_20K_SURETYPE)
    &IIME_ldb_pinyin_tc_suretype,
#endif

#if defined(ZY_BIG5_LEVEL1_20K) || defined(ZY_BIG5NORMAL_LEVEL1_20K) || defined(ZY_BIG5HKSIIME_LEVEL1_20K)
    &IIME_ldb_zhuyin_cizu,
#endif
#if defined(ZY_GB18030_LEVEL1_20K) || defined(ZY_GB2312_LEVEL1_20K)
    &IIME_ldb_zhuyin_sc,
#endif

#if defined(IIME_PINYIN_MINI)
	&IIME_ldb_pinyin_danzi,
#endif

#if defined(DZZY_BIG5NORMAL_LEVEL0_20K) || defined(DZZY_BIG5NORMAL_LEVEL0_10K) || defined(DZZY_BIG5NORMAL_LEVEL0_0K)
    &IIME_ldb_zhuyin_danzi,
#endif

#if defined(KMX_LDB_STROKE_GB18030) || defined(KMX_LDB_STROKE_GBK) || defined(KMX_LDB_STROKE_GB2312)
    &IIME_ldb_stroke_chs,
#endif

#if defined(KMX_LDB_STROKE_BIG5)
    &IIME_ldb_stroke_big5,
#endif

#if defined(KMX_LDB_STROKE_BIG5HKSCS)
    &IIME_ldb_stroke_big5hkscs,
#endif

#if defined(KMX_LDB_ENGLISH)
    &IIME_ldb_english,
#endif

#if defined(KMX_LDB_ENGLISH_SURETYPE)
    &IIME_ldb_english_suretype,
#endif

#if defined(KMX_LDB_ARMENIAN)
    &IIME_ldb_armenian,
#endif
#if defined(KMX_LDB_ARABIC)
    &IIME_ldb_arabic,
#endif
#if defined(KMX_LDB_BENGALI)
    &IIME_ldb_bengali,
#endif
#if defined(KMX_LDB_BULGARIAN)
    &IIME_ldb_bulgarian,
#endif
#if defined(KMX_LDB_CROATIAN)
    &IIME_ldb_croatian,
#endif
#if defined(KMX_LDB_CZECH)
    &IIME_ldb_czech,
#endif
#if defined(KMX_LDB_DANISH)
    &IIME_ldb_danish,
#endif
#if defined(KMX_LDB_DUTCH)
    &IIME_ldb_dutch,
#endif
#if defined(KMX_LDB_FINNISH)
    &IIME_ldb_finnish,
#endif
#if defined(KMX_LDB_FRENCH)
    &IIME_ldb_french,
#endif
#if defined(KMX_LDB_GERMAN)
    &IIME_ldb_german,
#endif
#if defined(KMX_LDB_GREEK)
    &IIME_ldb_greek,
#endif
#if defined(KMX_LDB_HEBREW)
    &IIME_ldb_hebrew,
#endif
#if defined(KMX_LDB_HINDI)
    &IIME_ldb_hindi,
#endif
#if defined(KMX_LDB_NEPALI)//IM_NEPALI_SUPPORT
    &IIME_ldb_nepali,
#endif
#if defined(KMX_LDB_SINHALA)//IM_SINHALA_SUPPORT
    &IIME_ldb_sinhala,
#endif
//daodejing
#if defined(KMX_LDB_BODO)
    &IIME_ldb_bodo,
#endif
#if defined(KMX_LDB_DOGRI)
    &IIME_ldb_dogri,
#endif
#if defined(KMX_LDB_SANSKRIT)
    &IIME_ldb_sanskrit,
#endif
#if defined(KMX_LDB_KASHMIRI)
    &IIME_ldb_kashmiri,
#endif
#if defined(KMX_LDB_KONKANI)
    &IIME_ldb_konkani,
#endif
#if defined(KMX_LDB_MAITHILI)
    &IIME_ldb_maithili,
#endif
#if defined(KMX_LDB_MANIPURI)
    &IIME_ldb_manipuri,
#endif
#if defined(KMX_LDB_SANTALI)
    &IIME_ldb_santali,
#endif
#if defined(KMX_LDB_SOMALI)
    &IIME_ldb_somali,
#endif
#if defined(KMX_LDB_SINDHI)
    &IIME_ldb_sindhi,
#endif
#if defined(KMX_LDB_HINGLISH)
    &IIME_ldb_hinglish,
#endif
#if defined(KMX_LDB_HUNGARIAN)
    &IIME_ldb_hungarian,
#endif
#if defined(KMX_LDB_INDONESIAN)
    &IIME_ldb_indonesian,
#endif
#if defined(KMX_LDB_ITALIAN)
    &IIME_ldb_italian,
#endif
#if defined(KMX_LDB_LITHUANIAN)
    &IIME_ldb_lithuanian,
#endif
#if defined(KMX_LDB_MALAY)
    &IIME_ldb_malay,
#endif
#if defined(KMX_LDB_NORWEGIAN)
    &IIME_ldb_norwegian,
#endif
#if defined(KMX_LDB_PERSIAN)
    &IIME_ldb_persian,
#endif
#if defined(KMX_LDB_POLISH)
    &IIME_ldb_polish,
#endif
#if defined(KMX_LDB_PORTUGUESE)
    &IIME_ldb_portuguese,
#endif
#if defined(KMX_LDB_PUNJABI)
    &IIME_ldb_punjabi,
#endif
#if defined(KMX_LDB_GUJARATI)
    &IIME_ldb_gujarati,
#endif
#if defined(KMX_LDB_ORIYA)
    &IIME_ldb_oriya,
#endif
#if defined(KMX_LDB_KANNADA)
    &IIME_ldb_kannada,
#endif
#if defined(KMX_LDB_MALAYALAM)
    &IIME_ldb_malayalam,
#endif
#if defined(KMX_LDB_KHMER)
    &IIME_ldb_khmer,
#endif
#if defined(KMX_LDB_KYRGYZ)
    &IIME_ldb_kyrgyz,
#endif
#if defined(KMX_LDB_LAO)
    &IIME_ldb_lao,
#endif
#if defined(KMX_LDB_ROMANIAN)
    &IIME_ldb_romanian,
#endif
#if defined(KMX_LDB_RUSSIAN)
    &IIME_ldb_russian,
#endif
#if defined(KMX_LDB_SLOVAK)
    &IIME_ldb_slovak,
#endif
#if defined(KMX_LDB_SLOVENIAN)
    &IIME_ldb_slovenian,
#endif
#if defined(KMX_LDB_SPANISH)
    &IIME_ldb_spanish,
#endif
#if defined(KMX_LDB_SWAHILI)
    &IIME_ldb_swahili,
#endif
#if defined(KMX_LDB_SWEDISH)
    &IIME_ldb_swedish,
#endif
#if defined(KMX_LDB_THAI)
    &IIME_ldb_thai,
#endif
#if defined(KMX_LDB_TIBETAN)
    &IIME_ldb_tibetan,
#endif
#if defined(KMX_LDB_TURKISH)
    &IIME_ldb_turkish,
#endif
#if defined(KMX_LDB_UKRAINIAN)
    &IIME_ldb_ukrainian,
#endif
#if defined(KMX_LDB_VIETNAMESE)
    &IIME_ldb_vietnamese,
#endif
#if defined(KMX_LDB_MYANMAR)
    &IIME_ldb_myanmar,
#endif
#if defined(KMX_LDB_URDU)
    &IIME_ldb_urdu,
#endif
#if defined(KMX_LDB_UYGHUR)
    &IIME_ldb_uyghur,
#endif
#if defined(KMX_LDB_HANGUL)
    &IIME_ldb_hangul,
#endif
#if defined(KMX_LDB_TELUGU)
    &IIME_ldb_telugu,
#endif
#if defined(KMX_LDB_MARATHI)
    &IIME_ldb_marathi,
#endif
#if defined(KMX_LDB_TAMIL)
    &IIME_ldb_tamil,
#endif
#if defined(KMX_LDB_TAGALOG)
    &IIME_ldb_tagalog,
#endif
#if defined(KMX_LDB_AMHARIC)
    &IIME_ldb_amharic,
#endif
#if defined(KMX_LDB_NEPALI)
    &IIME_ldb_nepali,
#endif
//daodejing
#if defined(KMX_LDB_BODO)
    &IIME_ldb_bodo,
#endif
#if defined(KMX_LDB_DOGRI)
    &IIME_ldb_dogri,
#endif
#if defined(KMX_LDB_SANSKRIT)
    &IIME_ldb_sanskrit,
#endif
#if defined(KMX_LDB_KASHMIRI)
    &IIME_ldb_kashmiri,
#endif
#if defined(KMX_LDB_KONKANI)
    &IIME_ldb_konkani,
#endif
#if defined(KMX_LDB_MAITHILI)
    &IIME_ldb_maithili,
#endif
#if defined(KMX_LDB_MANIPURI)
    &IIME_ldb_manipuri,
#endif
#if defined(KMX_LDB_SANTALI)
    &IIME_ldb_santali,
#endif
#if defined(KMX_LDB_SOMALI)
    &IIME_ldb_somali,
#endif
#if defined(KMX_LDB_SINDHI)
    &IIME_ldb_sindhi,
#endif
#if defined(KMX_LDB_ASSAMESE)
    &IIME_ldb_assamese,
#endif
#if defined(KMX_LDB_ALBANIAN)
    &IIME_ldb_albanian,
#endif
#if defined(KMX_LDB_CATALAN)
    &IIME_ldb_catalan,
#endif
#if defined(KMX_LDB_BASQUE)
    &IIME_ldb_basque,
#endif
#if defined(KMX_LDB_GALICIAN)
    &IIME_ldb_galician,
#endif
#if defined(KMX_LDB_IGBO)
    &IIME_ldb_igbo,
#endif
#if defined(KMX_LDB_OROMO)
    &IIME_ldb_oromo,
#endif
#if defined(KMX_LDB_SERBIAN)
    &IIME_ldb_serbian,
#endif
#if defined(KMX_LDB_SERBIANC)
    &IIME_ldb_serbianc,
#endif

#if defined(KMX_LDB_CEBUANO)
    &IIME_ldb_cebuano,
#endif
#if defined(KMX_LDB_ZULU)
    &IIME_ldb_zulu,
#endif

#if defined(KMX_LDB_KAZAKH)
    &IIME_ldb_kazakh,
#endif
#if defined(KMX_LDB_UZBEK)
    &IIME_ldb_uzbek,
#endif
#if defined(KMX_LDB_GEORGIAN)
    &IIME_ldb_georgian,
#endif

	0
};

int gui_input_method_count = 0;
int* gui_input_method_list = input_method_list[10];     // 10 is english

HBITMAP lcd_bitmap = 0;

static const RECT lcd_pos = {16, 10, 304, 346};

static HFONT lcd_system_font = 0;
static HFONT lcd_small_font = 0;

static BOOL lcd_dirty = FALSE;
static RECT lcd_dirty_pos;

static GUI_KEY_HANDLER gui_key_handler = 0;
static GUI_TIMER_HANDLER gui_timer_handler = 0;

static GUI_TIMER_HANDLER gui_timer_handler1 = 0;
static GUI_TIMER_HANDLER gui_timer_handler2 = 0;

static GUI_CHAR_HANDLER gui_char_handler = 0;

static void set_dirty(const RECT* r)
{
    RECT r1, r2;

    SetRect(&r1, 0, 0, GUI_WIDTH, GUI_HEIGHT);
    if (IntersectRect(&r2, &r1, r))
    {
        if (lcd_dirty)
        {
            r1 = lcd_dirty_pos;
            UnionRect(&lcd_dirty_pos, &r1, &r2);
        }
        else
        {
            lcd_dirty = TRUE;
            lcd_dirty_pos = r2;
        }
        UpdateGuiDebugWindow(&r2);
    }
}

static void update_dirty(void)
{
    RECT r;

    kbd_update_layout();

    if (lcd_dirty)
    {
        r = lcd_dirty_pos;
        OffsetRect(&r, lcd_pos.left, lcd_pos.top);
        InvalidateRect(g_main_window, &r, FALSE);
        UpdateGuiZoomWindow(&lcd_dirty_pos);
    }
}

static void init_input_method_list(void)
{
    int i = 0;
    for (i = 0; i < GUI_MAX_INPUT_METHOD; i++)
    {
        if (gui_input_method_list[i] == 0)
            break;
    }

    gui_input_method_count = i;//sizeof(input_method_list) / sizeof(input_method_list[0]);
}

void gui_set_default_language(int lang)
{
    default_lang = lang - MMI_LANG_FIRST;
    gui_input_method_list = input_method_list[default_lang];

    init_input_method_list();
}


void gui_init(void)
{
    HDC hdc;
    LOGFONT lf;

    memset(&lf, 0, sizeof(lf));
    lf.lfHeight = 28;
    lstrcpy(lf.lfFaceName,  L"Myanmar3"); //L"Microsoft YaHei");   /* L"Simsun-18030" */ /* L"Gulim" */  /* L"Myanmar3" */
    lcd_system_font = CreateFontIndirect(&lf);

    memset(&lf, 0, sizeof(lf));
    lf.lfHeight = 20;
    lstrcpy(lf.lfFaceName, L"Tahoma");
    lcd_small_font = CreateFontIndirect(&lf);

    hdc = GetDC(g_main_window);
    lcd_bitmap = CreateCompatibleBitmap(hdc, GUI_WIDTH, GUI_HEIGHT);
    ReleaseDC(g_main_window, hdc);

    lcd_dirty = FALSE;

    init_input_method_list();

    gui_key_handler = 0;
    gui_timer_handler = 0;
    gui_char_handler = 0;

    mainmenu_show();
}

void gui_cleanup(void)
{
    gui_key_handler = 0;

    if (gui_timer_handler)
    {
        KillTimer(g_main_window, GUI_IME_TIMER_ID);
        gui_timer_handler = 0;
    }

    gui_char_handler = 0;

    DeleteObject(lcd_bitmap);
    DeleteObject(lcd_small_font);
    DeleteObject(lcd_system_font);
}

void gui_handle_key(int msg, int key, int time)
{
    if (gui_key_handler)
    {
        gui_key_handler(msg, key, time);
        update_dirty();
    }
}

void gui_handle_timer(int timer_id)
{
    if (timer_id == GUI_IME_TIMER_ID)
    {
        if (gui_timer_handler)
        {
            gui_timer_handler();
            update_dirty();
        }
        else
        {
            KillTimer(g_main_window, GUI_IME_TIMER_ID);
        }
    }
}

void gui_handle_char(int key)
{
    if (gui_char_handler)
    {
        gui_char_handler(key);
        update_dirty();
    }
}

void gui_paint(HDC hdc)
{
    RECT r;
    HDC mem_hdc;
    HBITMAP save_bitmap;

    lcd_dirty = FALSE;

    r = lcd_pos;
    InflateRect(&r, 1, 1);
    DrawEdge(hdc, &r, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);

    mem_hdc = CreateCompatibleDC(hdc);
    save_bitmap = (HBITMAP)SelectObject(mem_hdc, lcd_bitmap);

    BitBlt(hdc, lcd_pos.left, lcd_pos.top, GUI_WIDTH, GUI_HEIGHT,
        mem_hdc, 0, 0, SRCCOPY);

    SelectObject(mem_hdc, save_bitmap);
    DeleteDC(mem_hdc);
}

void gui_set_key_handler(
    GUI_KEY_HANDLER     handler)
{
    gui_key_handler = handler;
}

void gui_enable_ime_timer(
    int                 time,
    GUI_TIMER_HANDLER   handler)
{
    if (handler)
    {
        gui_timer_handler = handler;
        SetTimer(g_main_window, GUI_IME_TIMER_ID, time, NULL);
    }
    else if (gui_timer_handler)
    {
        gui_timer_handler = 0;
        KillTimer(g_main_window, GUI_IME_TIMER_ID);
    }
}

void gui_enable_timer(int time,
                      int timer_id,
                      GUI_TIMER_HANDLER handler)
{
    if (timer_id == GUI_IME_TIMER_ID_1)
    {
        if (handler)
        {
            gui_timer_handler1 = handler;
            SetTimer(g_main_window, timer_id, time, NULL);
        }
        else if (gui_timer_handler1)
        {
            gui_timer_handler1 = 0;
            KillTimer(g_main_window, timer_id);
        }
    }
    else
    {
        if (handler)
        {
            gui_timer_handler2 = handler;
            SetTimer(g_main_window, timer_id, time, NULL);
        }
        else if (gui_timer_handler2)
        {
            gui_timer_handler2 = 0;
            KillTimer(g_main_window, timer_id);
        }
    }
}

void gui_set_char_handler(
    GUI_CHAR_HANDLER handler)
{
    gui_char_handler = handler;
}

void gui_disable_ime_timer(void)
{
    if (gui_timer_handler)
    {
        gui_timer_handler = 0;
        KillTimer(g_main_window, GUI_IME_TIMER_ID);
    }
}

void gui_disable_timer(int timer_id)
{
    if (timer_id == GUI_IME_TIMER_ID_1)
    {
        if (gui_timer_handler1)
        {
            gui_timer_handler1 = 0;
            KillTimer(g_main_window, GUI_IME_TIMER_ID_1);
        }
    }
    else
    {
        if (gui_timer_handler2)
        {
            gui_timer_handler2 = 0;
            KillTimer(g_main_window, GUI_IME_TIMER_ID_2);
        }
    }
}

void gui_draw_pixel(
    int                 x,
    int                 y,
    int                 color)
{
    RECT d;
    HDC hdc;
    HBITMAP save_bitmap;

    hdc = CreateCompatibleDC(NULL);
    save_bitmap = (HBITMAP)SelectObject(hdc, lcd_bitmap);

    SetPixel(hdc, x, y, color);

    SelectObject(hdc, save_bitmap);
    DeleteDC(hdc);

    SetRect(&d, x - 1, y - 1, x + 1, y + 1);
    set_dirty(&d);
}

void gui_draw_line(
    int                 x1,
    int                 y1,
    int                 x2,
    int                 y2,
    int                 color)
{
    RECT d;
    HDC hdc;
    HBITMAP save_bitmap;
    HPEN pen;
    HPEN save_pen;

    hdc = CreateCompatibleDC(NULL);
    save_bitmap = (HBITMAP)SelectObject(hdc, lcd_bitmap);

    pen = CreatePen(PS_SOLID, 0, color);
    save_pen = (HPEN)SelectObject(hdc, pen);

    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    SelectObject(hdc, save_pen);
    DeleteObject(pen);
    SelectObject(hdc, save_bitmap);
    DeleteDC(hdc);

    d.left   = ((x1 <= x2) ? x1 : x2) - 1;
    d.top    = ((y1 <= y2) ? y1 : y2) - 1;
    d.right  = ((x1 >= x2) ? x1 : x2) + 1;
    d.bottom = ((y1 >= y2) ? y1 : y2) + 1;

    set_dirty(&d);
}

void gui_draw_rect(
    const RECT*         r,
    int                 color)
{
    RECT d;
    HDC hdc;
    HBITMAP save_bitmap;
    HBRUSH brush;

    hdc = CreateCompatibleDC(NULL);
    save_bitmap = (HBITMAP)SelectObject(hdc, lcd_bitmap);

    brush = CreateSolidBrush(color);
    FillRect(hdc, r, brush);
    DeleteObject(brush);

    SelectObject(hdc, save_bitmap);
    DeleteDC(hdc);

    SetRect(&d, r->left - 1, r->top - 1, r->right + 1, r->bottom + 1);
    set_dirty(&d);
}

void gui_draw_button(int x, 
                     int y,
                     int id)
{
    RECT d;
    HDC hdc;
    HDC hMemDC;

    HBITMAP hImage;
    HBITMAP hOldBitmap;
    HBITMAP save_bitmap;
    BITMAP  bm;
    
    hdc = CreateCompatibleDC(NULL);
    save_bitmap = (HBITMAP)SelectObject(hdc, lcd_bitmap);

    hImage = LoadBitmap(g_instance, MAKEINTRESOURCE(id));
    GetObject(hImage, sizeof(BITMAP), &bm);

    hMemDC = CreateCompatibleDC(hdc);
    hOldBitmap = SelectObject(hMemDC, hImage);

    BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hImage);
    DeleteDC(hMemDC);

    SelectObject(hdc, save_bitmap);
    DeleteDC(hdc);

    SetRect(&d, x - 1, y -1, x + bm.bmWidth + 1, y + bm.bmHeight + 1);
    set_dirty(&d);
}


void gui_draw_text(
    int                 x,
    int                 y,
    int                 font,
    const WCHAR*        text,
    int                 text_len,
    int                 color)
{
    RECT d;
    HDC hdc;
    HBITMAP save_bitmap;
    HFONT save_font;
    SIZE s;

    hdc = CreateCompatibleDC(NULL);
    save_bitmap = (HBITMAP)SelectObject(hdc, lcd_bitmap);

    if (font == GUI_SMALL_FONT)
        save_font = (HFONT)SelectObject(hdc, lcd_small_font);
    else
        save_font = (HFONT)SelectObject(hdc, lcd_system_font);

    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    SetTextAlign(hdc, TA_LEFT | TA_TOP);

    GetTextExtentPoint32(hdc, text, text_len, &s);
    TextOut(hdc, x, y, text, text_len);

    SelectObject(hdc, save_font);
    SelectObject(hdc, save_bitmap);
    DeleteDC(hdc);

    SetRect(&d, x - 1, y - 1, x + s.cx + 1, y + s.cy + 1);
    set_dirty(&d);
}

int gui_get_text_height(
    int                 font)
{
    HDC hdc;
    HFONT save_font;
    SIZE s;

    hdc = GetDC(g_main_window);

    if (font == GUI_SMALL_FONT)
        save_font = (HFONT)SelectObject(hdc, lcd_small_font);
    else
        save_font = (HFONT)SelectObject(hdc, lcd_system_font);

    GetTextExtentPoint32(hdc, L"��", 1, &s);

    SelectObject(hdc, save_font);

    ReleaseDC(g_main_window, hdc);

    return s.cy;
}

int gui_get_char_width(
    int                 font,
    WCHAR               chr)
{
    return gui_get_text_width(font, &chr, 1);
}

int gui_get_text_width(
    int                 font,
    const WCHAR*        text,
    int                 text_len)
{
    HDC hdc;
    HFONT save_font;
    SIZE s;

    hdc = GetDC(g_main_window);

    if (font == GUI_SMALL_FONT)
        save_font = (HFONT)SelectObject(hdc, lcd_small_font);
    else
        save_font = (HFONT)SelectObject(hdc, lcd_system_font);

    GetTextExtentPoint32(hdc, text, text_len, &s);

    SelectObject(hdc, save_font);

    ReleaseDC(g_main_window, hdc);

    return s.cx;
}

void* gui_load_ime_userdata(
    int*                size)
{
    if (size)
        *size = userdata_size;
    return userdata;
}

void gui_save_ime_userdata(
    void*               data,
    int                 size)
{
    userdata_save();
}

void* gui_load_ime_options(
    int*                size)
{
    if (size)
        *size = imeoptiondata_size;
    return imeoptiondata;
}

void gui_save_ime_options(
    void*               data,
    int                 size)
{
    userdata_save();
}



const void gui_get_input_method_name(int input_method, WCHAR *pIMEName)
{
    const WCHAR *pName = NULL;
    const WCHAR *pFirst = NULL;
	if (input_method > IM_LATINFULLKEY_BASE)
    {
        pFirst = L"Q:";
        input_method -= IM_LATINFULLKEY_BASE;
    }
    else
        pFirst = L"\0";

	if (input_method > IM_SURETYPE_BASE)
        input_method -= IM_SURETYPE_BASE;

    switch (input_method)
    {
        case IM_PINYIN_DZ:              pName = L"ƴ��DZ"; break;
        case IM_PINYIN:                 pName = L"ƴ��"; break;
        case IM_PINYIN_FULLKEY:         pName = L"ƴ��TP"; break;
        case IM_PINYIN_TC:              pName = L"ƴ��(��)"; break;
        case IM_PINYIN_FULLKEY_TC:      pName = L"ƴ��TP(��)"; break;
        case IM_STROKE:                 pName = L"�ʻ�"; break;
        case IM_STROKE_BIG5:            pName = L"�ʻ�(��)"; break;

        case IM_NUMBER:                 pName = L"����"; break;
        case IM_123:                    pName = L"����"; break;
        case IM_ZHUYIN:                 pName = L"ע��"; break;
        case IM_ZHUYIN_2:               pName = L"ע��2"; break;
        case IM_ZHUYIN_MT:              pName = L"ע��MT"; break;
        case IM_ZHUYIN_MT_FULL:         pName = L"ע��MTf"; break;
        case IM_ZHUYIN_FULLKEY:         pName = L"ע��TP"; break;
        case IM_ZHUYIN_MT_SC:           pName = L"ע��MT(��)"; break;
        case IM_ZHUYIN_FULLKEY_SC:      pName = L"ע��TP(��)"; break;

        case IM_ENGLISH:                pName = L"Ӣ��"; break;
        case IM_ABC_LOWER:              pName = L"Ӣ��(abc)"; break;
        case IM_ABC_UPPER:              pName = L"Ӣ��(ABC)"; break;
        case IM_ABC_LEAD_UPPER:         pName = L"Ӣ��(Abc)"; break;
        case IM_ABC:                    pName = L"Ӣ��(Abc*)"; break;
		case IM_ENGLISH_LEAD_UPPER:		pName = L"Ӣ��(Ul)"; break;
        case IM_ENGLISH_UPPER:          pName = L"Ӣ��(U)"; break;
        case IM_ENGLISH_LOWER:          pName = L"Ӣ��(L)"; break;

        case IM_RUSSIAN:                pName = L"����"; break;
        case IM_ABC_LOWER_RUS:          pName = L"����(abc)"; break;
        case IM_ABC_UPPER_RUS:          pName = L"����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_RUS:     pName = L"����(Abc)"; break;
        case IM_ABC_RUS:                pName = L"����(Abc*)"; break;
        case IM_RUSSIAN_UPPER:          pName = L"����(U)"; break;
        case IM_RUSSIAN_LOWER:          pName = L"����(L)"; break;

        case IM_GERMAN:                 pName = L"����"; break;
        case IM_ABC_LOWER_DE:           pName = L"����(abc)"; break;
        case IM_ABC_UPPER_DE:           pName = L"����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_DE:      pName = L"����(Abc)"; break;
        case IM_ABC_DE:                 pName = L"����(Abc*)"; break;
        case IM_GERMAN_UPPER:           pName = L"����(U)"; break;
        case IM_GERMAN_LOWER:           pName = L"����(L)"; break;

        case IM_VIETNAMESE:             pName = L"Խ����"; break;
        case IM_ABC_LOWER_VI:           pName = L"Խ����(abc)"; break;
        case IM_ABC_UPPER_VI:           pName = L"Խ����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_VI:      pName = L"Խ����(Abc)"; break;
        case IM_ABC_VI:                 pName = L"Խ����(Abc*)"; break;
        case IM_VIETNAMESE_LEAD_UPPER:  pName = L"Խ����(Ul)"; break;
        case IM_VIETNAMESE_UPPER:       pName = L"Խ����(U)"; break;
        case IM_VIETNAMESE_LOWER:       pName = L"Խ����(L)"; break;

        case IM_INDONESIAN:             pName = L"ӡ����"; break;
        case IM_ABC_LOWER_ID:           pName = L"ӡ����(abc)"; break;
        case IM_ABC_UPPER_ID:           pName = L"ӡ����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_ID:      pName = L"ӡ����(Abc)"; break;
        case IM_ABC_ID:                 pName = L"ӡ����(Abc*)"; break;
		case IM_INDONESIAN_LEAD_UPPER:  pName = L"ӡ����(Ul)"; break;
        case IM_INDONESIAN_UPPER:       pName = L"ӡ����(U)"; break;
        case IM_INDONESIAN_LOWER:       pName = L"ӡ����(L)"; break;

        case IM_TURKISH:                pName = L"��������"; break;
        case IM_ABC_LOWER_TR:           pName = L"��������(abc)"; break;
        case IM_ABC_UPPER_TR:           pName = L"��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_TR:      pName = L"��������(Abc)"; break;
        case IM_ABC_TR:                 pName = L"��������(Abc*)"; break;
        case IM_TURKISH_UPPER:          pName = L"��������(U)"; break;
        case IM_TURKISH_LOWER:          pName = L"��������(L)"; break;

        case IM_MALAY:                  pName = L"������"; break;
        case IM_ABC_LOWER_MS:           pName = L"������(abc)"; break;
        case IM_ABC_UPPER_MS:           pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_MS:      pName = L"������(Abc)"; break;
        case IM_ABC_MS:                 pName = L"������(Abc*)"; break;
        case IM_MALAY_UPPER:            pName = L"������(U)"; break;
        case IM_MALAY_LOWER:            pName = L"������(L)"; break;

        case IM_GREEK:                  pName = L"ϣ����"; break;
        case IM_ABC_LOWER_EL:           pName = L"ϣ����(abc)"; break;
        case IM_ABC_UPPER_EL:           pName = L"ϣ����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_EL:      pName = L"ϣ����(Abc)"; break;
        case IM_ABC_EL:                 pName = L"ϣ����(Abc*)"; break;
        case IM_GREEK_UPPER:            pName = L"ϣ����(U)"; break;
        case IM_GREEK_LOWER:            pName = L"ϣ����(L)"; break;

        case IM_THAI:                   pName = L"̩��"; break;
        case IM_THAI_MT:                pName = L"̩��(MT)"; break;
        case IM_THAI_123:               pName = L"̩��(123)"; break;
        
        case IM_TIBETAN_MT:             pName = L"����(MT)"; break;
        case IM_TIBETAN_123:            pName = L"����(123)"; break;

        case IM_DANISH:                 pName = L"������"; break;
        case IM_ABC_LOWER_DA:           pName = L"������(abc)"; break;
        case IM_ABC_UPPER_DA:           pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_DA:      pName = L"������(Abc)"; break;
        case IM_ABC_DA:                 pName = L"������(Abc*)"; break;
        case IM_DANISH_UPPER:           pName = L"������(U)"; break;
        case IM_DANISH_LOWER:           pName = L"������(L)"; break;

        case IM_DUTCH:                  pName = L"������"; break;
        case IM_ABC_LOWER_NL:           pName = L"������(abc)"; break;
        case IM_ABC_UPPER_NL:           pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_NL:      pName = L"������(Abc)"; break;
        case IM_ABC_NL:                 pName = L"������(Abc*)"; break;
        case IM_DUTCH_UPPER:            pName = L"������(U)"; break;
        case IM_DUTCH_LOWER:            pName = L"������(L)"; break;

        case IM_POLISH:                 pName = L"������"; break;
        case IM_ABC_LOWER_PL:           pName = L"������(abc)"; break;
        case IM_ABC_UPPER_PL:           pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_PL:      pName = L"������(Abc)"; break;
        case IM_ABC_PL:                 pName = L"������(Abc*)"; break;
        case IM_POLISH_UPPER:           pName = L"������(U)"; break;
        case IM_POLISH_LOWER:           pName = L"������(L)"; break;

        case IM_FRENCH:                 pName = L"����"; break;
        case IM_ABC_LOWER_FR:           pName = L"����(abc)"; break;
        case IM_ABC_UPPER_FR:           pName = L"����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_FR:      pName = L"����(Abc)"; break;
        case IM_ABC_FR:                 pName = L"����(Abc*)"; break;
        case IM_FRENCH_LEAD_UPPER:      pName = L"����(Ul)"; break;
        case IM_FRENCH_UPPER:           pName = L"����(U)"; break;
        case IM_FRENCH_LOWER:           pName = L"����(L)"; break;

        case IM_SPANISH:                pName = L"��������"; break;
        case IM_ABC_LOWER_ES:           pName = L"��������(abc)"; break;
        case IM_ABC_UPPER_ES:           pName = L"��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_ES:      pName = L"��������(Abc)"; break;
        case IM_ABC_ES:                 pName = L"��������(Abc*)"; break;
        case IM_SPANISH_UPPER:          pName = L"��������(U)"; break;
        case IM_SPANISH_LOWER:          pName = L"��������(L)"; break;

        case IM_FINNISH:                pName = L"����"; break;
        case IM_ABC_LOWER_FI:           pName = L"����(abc)"; break;
        case IM_ABC_UPPER_FI:           pName = L"����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_FI:      pName = L"����(Abc)"; break;
        case IM_ABC_FI:                 pName = L"����(Abc*)"; break;
        case IM_FINNISH_UPPER:          pName = L"����(U)"; break;
        case IM_FINNISH_LOWER:          pName = L"����(L)"; break;

        case IM_SLOVENIAN:              pName = L"˹��������"; break;
        case IM_ABC_LOWER_SL:           pName = L"˹��������(abc)"; break;
        case IM_ABC_UPPER_SL:           pName = L"˹��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SL:      pName = L"˹��������(Abc)"; break;
        case IM_ABC_SL:                 pName = L"˹��������(Abc*)"; break;
        case IM_SLOVENIAN_UPPER:        pName = L"˹��������(U)"; break;
        case IM_SLOVENIAN_LOWER:        pName = L"˹��������(L)"; break;

        case IM_UKRAINIAN:              pName = L"�ڿ���"; break;
        case IM_ABC_LOWER_UK:           pName = L"�ڿ���(abc)"; break;
        case IM_ABC_UPPER_UK:           pName = L"�ڿ���(ABC)"; break;
        case IM_ABC_LEAD_UPPER_UK:      pName = L"�ڿ���(Abc)"; break;
        case IM_ABC_UK:                 pName = L"�ڿ���(Abc*)"; break;
        case IM_UKRAINIAN_UPPER:        pName = L"�ڿ���(U)"; break;
        case IM_UKRAINIAN_LOWER:        pName = L"�ڿ���(L)"; break;

        case IM_SWAHILI:                pName = L"˹������"; break;
        case IM_ABC_LOWER_SW:           pName = L"˹������(abc)"; break;
        case IM_ABC_UPPER_SW:           pName = L"˹������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SW:      pName = L"˹������(Abc)"; break;
        case IM_ABC_SW:                 pName = L"˹������(Abc*)"; break;
        case IM_SWAHILI_UPPER:          pName = L"˹������(U)"; break;
        case IM_SWAHILI_LOWER:          pName = L"˹������(L)"; break;
                   
        case IM_LITHUANIAN:             pName = L"������"; break;
        case IM_ABC_LOWER_LT:           pName = L"������(abc)"; break;
        case IM_ABC_UPPER_LT:           pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_LT:      pName = L"������(Abc)"; break;
        case IM_ABC_LT:                 pName = L"������(Abc*)"; break;
        case IM_LITHUANIAN_UPPER:       pName = L"������(U)"; break;
        case IM_LITHUANIAN_LOWER:       pName = L"������(L)"; break;

        case IM_ARMENIAN:               pName = L"�������"; break;
        case IM_ABC_LOWER_HY:           pName = L"�������(abc)"; break;
        case IM_ABC_UPPER_HY:           pName = L"�������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_HY:      pName = L"�������(Abc)"; break;
        case IM_ABC_HY:                 pName = L"�������(Abc*)"; break;
        case IM_ARMENIAN_UPPER:         pName = L"�������(U)"; break;
        case IM_ARMENIAN_LOWER:         pName = L"�������(L)"; break;
        case IM_ARMENIAN_LEAD_UPPER:    pName = L"�������(Ul)"; break;
		
        case IM_TAGALOG:                pName = L"����ŵ"; break;
        case IM_ABC_LOWER_TL:           pName = L"����ŵ(abc)"; break;
        case IM_ABC_UPPER_TL:           pName = L"����ŵ(ABC)"; break;
        case IM_ABC_LEAD_UPPER_TL:      pName = L"����ŵ(Abc)"; break;
        case IM_ABC_TL:                 pName = L"����ŵ(Abc*)"; break;
        case IM_TAGALOG_UPPER:          pName = L"����ŵ(U)"; break;
        case IM_TAGALOG_LOWER:          pName = L"����ŵ(L)"; break;
		
        case IM_KYRGYZ:                pName = L"������˹"; break;
        case IM_ABC_LOWER_KY:           pName = L"������˹(abc)"; break;
        case IM_ABC_UPPER_KY:           pName = L"������˹(ABC)"; break;
        case IM_ABC_LEAD_UPPER_KY:      pName = L"������˹(Abc)"; break;
        case IM_ABC_KY:                 pName = L"������˹(Abc*)"; break;
        case IM_KYRGYZ_UPPER:          pName = L"������˹(U)"; break;
        case IM_KYRGYZ_LOWER:          pName = L"������˹(L)"; break;
		
        case IM_ALBANIAN:               pName = L"����������"; break;
        case IM_ABC_LOWER_SQ:           pName = L"����������(abc)"; break;
        case IM_ABC_UPPER_SQ:           pName = L"����������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SQ:      pName = L"����������(Abc)"; break;
        case IM_ABC_SQ:                 pName = L"����������(Abc*)"; break;
        case IM_ALBANIAN_UPPER:         pName = L"����������(U)"; break;
        case IM_ALBANIAN_LOWER:         pName = L"����������(L)"; break;

        case IM_CATALAN:                pName = L"��̩������"; break;
        case IM_ABC_LOWER_CA:           pName = L"��̩������(abc)"; break;
        case IM_ABC_UPPER_CA:           pName = L"��̩������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_CA:      pName = L"��̩������(Abc)"; break;
        case IM_ABC_CA:                 pName = L"��̩������(Abc*)"; break;
        case IM_CATALAN_UPPER:          pName = L"��̩������(U)"; break;
        case IM_CATALAN_LOWER:          pName = L"��̩������(L)"; break;

        case IM_BASQUE:                 pName = L"��˹��"; break;
        case IM_ABC_LOWER_EU:           pName = L"��˹��(abc)"; break;
        case IM_ABC_UPPER_EU:           pName = L"��˹��(ABC)"; break;
        case IM_ABC_LEAD_UPPER_EU:      pName = L"��˹��(Abc)"; break;
        case IM_ABC_EU:                 pName = L"��˹��(Abc*)"; break;
        case IM_BASQUE_UPPER:           pName = L"��˹��(U)"; break;
        case IM_BASQUE_LOWER:           pName = L"��˹��(L)"; break;

        case IM_GALICIAN:               pName = L"��������"; break;
        case IM_ABC_LOWER_GL:           pName = L"��������(abc)"; break;
        case IM_ABC_UPPER_GL:           pName = L"��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_GL:      pName = L"��������(Abc)"; break;
        case IM_ABC_GL:                 pName = L"��������(Abc*)"; break;
        case IM_GALICIAN_UPPER:         pName = L"��������(U)"; break;
        case IM_GALICIAN_LOWER:         pName = L"��������(L)"; break;

        case IM_IGBO:                   pName = L"������"; break;
        case IM_ABC_LOWER_IG:           pName = L"������(abc)"; break;
        case IM_ABC_UPPER_IG:           pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_IG:      pName = L"������(Abc)"; break;
        case IM_ABC_IG:                 pName = L"������(Abc*)"; break;
        case IM_IGBO_UPPER:             pName = L"������(U)"; break;
        case IM_IGBO_LOWER:             pName = L"������(L)"; break;

        case IM_OROMO:                   pName = L"����Ī��"; break;
        case IM_ABC_LOWER_OM:           pName = L"����Ī��(abc)"; break;
        case IM_ABC_UPPER_OM:           pName = L"����Ī��(ABC)"; break;
        case IM_ABC_LEAD_UPPER_OM:      pName = L"����Ī��(Abc)"; break;
        case IM_ABC_OM:                 pName = L"����Ī��(Abc*)"; break;
        case IM_OROMO_UPPER:             pName = L"����Ī��(U)"; break;
        case IM_OROMO_LOWER:             pName = L"����Ī��(L)"; break;

        case IM_SERBIAN:                pName = L"����ά��"; break;
        case IM_ABC_LOWER_SR:           pName = L"����ά��(abc)"; break;
        case IM_ABC_UPPER_SR:           pName = L"����ά��(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SR:      pName = L"����ά��(Abc)"; break;
        case IM_ABC_SR:                 pName = L"����ά��(Abc*)"; break;
        case IM_SERBIAN_UPPER:          pName = L"����ά��(U)"; break;
        case IM_SERBIAN_LOWER:          pName = L"����ά��(L)"; break;

        case IM_SERBIANC:                pName = L"����ά��C"; break;
        case IM_ABC_LOWER_SRC:           pName = L"����ά��C(abc)"; break;
        case IM_ABC_UPPER_SRC:           pName = L"����ά��C(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SRC:      pName = L"����ά��C(Abc)"; break;
        case IM_ABC_SRC:                 pName = L"����ά��C(Abc*)"; break;
        case IM_SERBIANC_UPPER:          pName = L"����ά��C(U)"; break;
        case IM_SERBIANC_LOWER:          pName = L"����ά��C(L)"; break;

        case IM_CEBUANO:                pName = L"������"; break;
        case IM_ABC_LOWER_CEB:          pName = L"������(abc)"; break;
        case IM_ABC_UPPER_CEB:          pName = L"������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_CEB:     pName = L"������(Abc)"; break;
        case IM_ABC_CEB:                pName = L"������(Abc*)"; break;
        case IM_CEBUANO_UPPER:          pName = L"������(U)"; break;
        case IM_CEBUANO_LOWER:          pName = L"������(L)"; break;

        case IM_ZULU:                   pName = L"��³��"; break;
        case IM_ABC_LOWER_ZU:           pName = L"��³��(abc)"; break;
        case IM_ABC_UPPER_ZU:           pName = L"��³��(ABC)"; break;
        case IM_ABC_LEAD_UPPER_ZU:      pName = L"��³��(Abc)"; break;
        case IM_ABC_ZU:                 pName = L"��³��(Abc*)"; break;
        case IM_ZULU_UPPER:             pName = L"��³��(U)"; break;
        case IM_ZULU_LOWER:             pName = L"��³��(L)"; break;

        case IM_KAZAKH:                pName = L"��������"; break;
        case IM_ABC_LOWER_KK:          pName = L"��������(abc)"; break;
        case IM_ABC_UPPER_KK:          pName = L"��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_KK:     pName = L"��������(Abc)"; break;
        case IM_ABC_KK:                pName = L"��������(Abc*)"; break;
        case IM_KAZAKH_UPPER:          pName = L"��������(U)"; break;
        case IM_KAZAKH_LOWER:          pName = L"��������(L)"; break;

        case IM_UZBEK:                  pName = L"���ȱ����"; break;
        case IM_ABC_LOWER_UZ:           pName = L"���ȱ����(abc)"; break;
        case IM_ABC_UPPER_UZ:           pName = L"���ȱ����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_UZ:      pName = L"���ȱ����(Abc)"; break;
        case IM_ABC_UZ:                 pName = L"���ȱ����(Abc*)"; break;
        case IM_UZBEK_UPPER:            pName = L"���ȱ����(U)"; break;
        case IM_UZBEK_LOWER:            pName = L"���ȱ����(L)"; break;

        case IM_GEORGIAN:               pName = L"��³������"; break;
        case IM_ABC_LOWER_KA:           pName = L"��³������(abc)"; break;
        case IM_ABC_UPPER_KA:           pName = L"��³������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_KA:      pName = L"��³������(Abc)"; break;
        case IM_ABC_KA:                 pName = L"��³������(Abc*)"; break;
        case IM_GEORGIAN_UPPER:         pName = L"��³������(U)"; break;
        case IM_GEORGIAN_LOWER:         pName = L"��³������(L)"; break;

        case IM_NORWEGIAN:              pName = L"Ų��"; break;
        case IM_ABC_LOWER_NO:           pName = L"Ų��(abc)"; break;
        case IM_ABC_UPPER_NO:           pName = L"Ų��(ABC)"; break;
        case IM_ABC_LEAD_UPPER_NO:      pName = L"Ų��(Abc)"; break;
        case IM_ABC_NO:                 pName = L"Ų��(Abc*)"; break;
        case IM_NORWEGIAN_UPPER:        pName = L"Ų��(U)"; break;
        case IM_NORWEGIAN_LOWER:        pName = L"Ų��(L)"; break;

        case IM_SWEDISH:                pName = L"���"; break;
        case IM_ABC_LOWER_SV:           pName = L"���(abc)"; break;
        case IM_ABC_UPPER_SV:           pName = L"���(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SV:      pName = L"���(Abc)"; break;
        case IM_ABC_SV:                 pName = L"���(Abc*)"; break;
        case IM_SWEDISH_UPPER:          pName = L"���(U)"; break;
        case IM_SWEDISH_LOWER:          pName = L"���(L)"; break;

        case IM_ROMANIAN:               pName = L"����������"; break;
        case IM_ABC_LOWER_RO:           pName = L"����������(abc)"; break;
        case IM_ABC_UPPER_RO:           pName = L"����������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_RO:      pName = L"����������(Abc)"; break;
        case IM_ABC_RO:                 pName = L"����������(Abc*)"; break;
        case IM_ROMANIAN_UPPER:         pName = L"����������(U)"; break;
        case IM_ROMANIAN_LOWER:         pName = L"����������(L)"; break;

        case IM_PORTUGUESE:             pName = L"��������"; break;
        case IM_ABC_LOWER_PT:           pName = L"��������(abc)"; break;
        case IM_ABC_UPPER_PT:           pName = L"��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_PT:      pName = L"��������(Abc)"; break;
        case IM_ABC_PT:                 pName = L"��������(Abc*)"; break;
        case IM_PORTUGUESE_UPPER:       pName = L"��������(U)"; break;
        case IM_PORTUGUESE_LOWER:       pName = L"��������(L)"; break;

        case IM_ITALIAN:                pName = L"�������"; break;
        case IM_ABC_LOWER_IT:           pName = L"�������(abc)"; break;
        case IM_ABC_UPPER_IT:           pName = L"�������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_IT:      pName = L"�������(Abc)"; break;
        case IM_ABC_IT:                 pName = L"�������(Abc*)"; break;
        case IM_ITALIAN_UPPER:          pName = L"�������(U)"; break;
        case IM_ITALIAN_LOWER:          pName = L"�������(L)"; break;

        case IM_ARABIC:                 pName = L"��������"; break;
        case IM_ARABIC_MT:              pName = L"��������(MT)"; break;
        case IM_ARABIC_123:             pName = L"��������(123)"; break;

        case IM_PERSIAN:                pName = L"��˹��"; break;
        case IM_PERSIAN_MT:             pName = L"��˹��(MT)"; break;
        case IM_PERSIAN_123:            pName = L"��˹��(123)"; break;

        case IM_HINDI:                  pName = L"ӡ����"; break;
        case IM_HINDI_MT:               pName = L"ӡ����(MT)"; break;
        case IM_HINDI_123:              pName = L"ӡ����(123)"; break;
		
		case IM_HINGLISH:               pName = L"Hinglish"; break;
        case IM_ABC_LOWER_HIN:          pName = L"Hinglish(abc)"; break;
        case IM_ABC_UPPER_HIN:          pName = L"Hinglish(ABC)"; break;
        case IM_ABC_LEAD_UPPER_HIN:     pName = L"Hinglish(Abc)"; break;
        case IM_ABC_HIN:                pName = L"Hinglish(Abc*)"; break;
        case IM_HINGLISH_UPPER:         pName = L"Hinglish(U)"; break;
        case IM_HINGLISH_LOWER:         pName = L"Hinglish(L)"; break;

		case IM_BENGALI:                pName = L"�ϼ�����"; break;
        case IM_BENGALI_MT:             pName = L"�ϼ�����(MT)"; break;
        case IM_BENGALI_123:            pName = L"�ϼ�����(123)"; break;
		
        case IM_HEBREW:                 pName = L"ϣ����"; break;
        case IM_HEBREW_MT:              pName = L"ϣ����(MT)"; break;

        case IM_HUNGARIAN:              pName = L"��������"; break;
        case IM_ABC_LOWER_HU:           pName = L"��������(abc)"; break;
        case IM_ABC_UPPER_HU:           pName = L"��������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_HU:      pName = L"��������(Abc)"; break;
        case IM_ABC_HU:                 pName = L"��������(Abc*)"; break;
        case IM_HUNGARIAN_UPPER:        pName = L"��������(U)"; break;
        case IM_HUNGARIAN_LOWER:        pName = L"��������(L)"; break;

        case IM_CROATIAN:					pName = L"���޵�����"; break;
        case IM_ABC_LOWER_HR:           pName = L"���޵�����(abc)"; break;
        case IM_ABC_UPPER_HR:           pName = L"���޵�����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_HR:      pName = L"���޵�����(Abc)"; break;
        case IM_ABC_HR:                 pName = L"���޵�����(Abc*)"; break;
        case IM_CROATIAN_UPPER:			pName = L"���޵�����(U)"; break;
        case IM_CROATIAN_LOWER:			pName = L"���޵�����(L)"; break;

        case IM_CZECH:					pName = L"�ݿ���"; break;
        case IM_ABC_LOWER_CS:           pName = L"�ݿ���(abc)"; break;
        case IM_ABC_UPPER_CS:           pName = L"�ݿ���(ABC)"; break;
        case IM_ABC_LEAD_UPPER_CS:      pName = L"�ݿ���(Abc)"; break;
        case IM_ABC_CS:                 pName = L"�ݿ���(Abc*)"; break;
        case IM_CZECH_UPPER:			pName = L"�ݿ���(U)"; break;
        case IM_CZECH_LOWER:			pName = L"�ݿ���(L)"; break;

        case IM_BULGARIAN:				pName = L"����������"; break;
        case IM_ABC_LOWER_BG:           pName = L"����������(abc)"; break;
        case IM_ABC_UPPER_BG:           pName = L"����������(ABC)"; break;
        case IM_ABC_LEAD_UPPER_BG:		pName = L"����������(Abc)"; break;
        case IM_ABC_BG:		            pName = L"����������(Abc*)"; break;
        case IM_BULGARIAN_UPPER:		pName = L"����������(U)"; break;
        case IM_BULGARIAN_LOWER:		pName = L"����������(L)"; break;

        case IM_SLOVAK:				    pName = L"˹�工����"; break;
        case IM_ABC_LOWER_SK:           pName = L"˹�工����(abc)"; break;
        case IM_ABC_UPPER_SK:           pName = L"˹�工����(ABC)"; break;
        case IM_ABC_LEAD_UPPER_SK:		pName = L"˹�工����(Abc)"; break;
        case IM_ABC_SK:		            pName = L"˹�工����(Abc*)"; break;
        case IM_SLOVAK_UPPER:		    pName = L"˹�工����(U)"; break;
        case IM_SLOVAK_LOWER:		    pName = L"˹�工����(L)"; break;

        case IM_HANGUL_MT:		        pName = L"������"; break;    
        
        case IM_URDU:                   pName = L"�ڶ�����"; break;
        case IM_URDU_MT:                pName = L"�ڶ�����(MT)"; break;
        case IM_TELUGU_MT:              pName = L"̩¬��(MT)"; break;
        case IM_MARATHI_MT:             pName = L"������(MT)"; break;
        case IM_TAMIL_MT:               pName = L"̩�׶�(MT)"; break;
        case IM_PUNJABI_MT:             pName = L"������(MT)"; break;
        case IM_GUJARATI_MT:            pName = L"�ż�����"; break;
        case IM_ORIYA_MT:               pName = L"������"; break;
        case IM_KANNADA_MT:             pName = L"���ɴ�"; break;
        case IM_MALAYALAM_MT:           pName = L"��������ķ"; break;
        case IM_KHMER_MT:               pName = L"����կ��"; break;
        case IM_KHMER_123:              pName = L"����կ��(123)"; break;
        case IM_LAO_MT:                 pName = L"������"; break;
        case IM_AMHARIC_MT:             pName = L"��ķ������"; break;

        case IM_MYANMAR_MT:             pName = L"�����"; break;
        case IM_UYGHUR:                 pName = L"ά�����"; break;
        case IM_UYGHUR_MT:              pName = L"ά�����(MT)"; break;

        case IM_NEPALI_MT:              pName = L"�Ჴ����(MT)"; break;
        case IM_NEPALI_123:             pName = L"�Ჴ����(123)"; break;
		//daodejing
        case IM_BODO_MT:                pName = L"������(MT)"; break;
        case IM_BODO_123:               pName = L"������(123)"; break;
		case IM_DOGRI_MT:               pName = L"�������(MT)"; break;
        case IM_DOGRI_123:              pName = L"�������(123)"; break;
		case IM_SANSKRIT_MT:            pName = L"����(MT)"; break;
        case IM_SANSKRIT_123:           pName = L"����(123)"; break;
		case IM_KASHMIRI_MT:            pName = L"��ʲ�׶���(MT)"; break;
        case IM_KASHMIRI_123:           pName = L"��ʲ�׶���(123)"; break;
		case IM_KONKANI_MT:             pName = L"�׿�����(MT)"; break;
        case IM_KONKANI_123:            pName = L"�׿�����(123)"; break;
		case IM_MAITHILI_MT:            pName = L"��������(MT)"; break;
        case IM_MAITHILI_123:           pName = L"��������(123)"; break;
		case IM_MANIPURI_MT:            pName = L"�����ն���(MT)"; break;
        case IM_MANIPURI_123:           pName = L"�����ն���(123)"; break;
		case IM_SANTALI_MT:             pName = L"ɣ������(MT)"; break;
        case IM_SANTALI_123:            pName = L"ɣ������(123)"; break;
		case IM_SOMALI_MT:              pName = L"��������(MT)"; break;
        case IM_SOMALI_123:             pName = L"��������(123)"; break;
		case IM_SINDHI_MT:              pName = L"�ŵ���(MT)"; break;
        case IM_SINDHI_123:             pName = L"�ŵ���(123)"; break;
        case IM_ASSAMESE_MT:            pName = L"����ķ��(MT)"; break;
        case IM_ASSAMESE_123:           pName = L"����ķ��(123)"; break;

        default:                        pName = L"����!"; break;
    }

    wcscpy(pIMEName, pFirst);
    wcscat(pIMEName, pName);

    return;
}
