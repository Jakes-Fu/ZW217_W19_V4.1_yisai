
/***************************************************************************
 *                                                                         *
 * Copyright (C) 2000-2007 Beijing Komoxo Inc.                             *
 * All rights reserved.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef KMXIME_LATIN_H
#define KMXIME_LATIN_H

#ifdef _MSC_VER
    /* Disable Microsoft Visial C++ warning: unreferenced formal parameter */
    #pragma warning(disable: 4100)
    /* Disable Microsoft Visial C++ warning: named type definition in parentheses */
    #pragma warning(disable: 4115)
    /* Disable Microsoft Visial C++ warning: nonstandard extension used: nameless struct/union */
    #pragma warning(disable: 4201)
    /* Disable Microsoft Visial C++ warning: nonstandard extension used : bit field types other than int */
    #pragma warning(disable: 4214)
    /* Disable Microsoft Visial C++ warning: unreferenced local function has been removed */
    #pragma warning(disable: 4505)
    /* Disable Microsoft Visial C++ warning: conditional expression is constant */
    #pragma warning(disable: 4127)
#endif
#if 0
#include "inc/kmxime.h"
#else
#include "kmxime.h"
#endif
#include <string.h>
#include <stdlib.h>
#define NDEBUG
#ifdef NDEBUG
    #ifdef KMX_DEBUG
        #undef KMX_DEBUG
    #endif
    #define KMX_ASSERT(e)   ((void)0)
#else
    #ifndef KMX_DEBUG
        #define KMX_DEBUG
    #endif
    #include <stdio.h>
    #include <assert.h>
    #define KMX_ASSERT      assert
#endif

#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#ifndef KMX_TRUE
    #define KMX_TRUE        1
#endif

#ifndef KMX_FALSE
    #define KMX_FALSE       0
#endif

#ifdef __cplusplus 
extern "C"{ 
#endif 

typedef int                 LDB_BOOL;
typedef unsigned char       LDB_BYTE;
typedef unsigned short      LDB_WORD;
typedef unsigned short      LDB_CHAR;

#define LDB_WID             LDB_WORD

/****************************************************************************
 *
 * Language database structure.
 *
 ***************************************************************************/

#define KMX_LANG_ENGLISH         10
#define KMX_LANG_RUSSIAN         20
#define KMX_LANG_GERMAN          30
#define KMX_LANG_VIETNAMESE      31
#define KMX_LANG_INDONESIAN      32
#define KMX_LANG_TURKISH         33
#define KMX_LANG_MALAY           34
#define KMX_LANG_GREEK           35
#define KMX_LANG_THAI            36
#define KMX_LANG_DANISH          37
#define KMX_LANG_DUTCH           38
#define KMX_LANG_POLISH          39
#define KMX_LANG_FRENCH          40
#define KMX_LANG_HUNGARIAN       41
#define KMX_LANG_HEBREW          42
#define KMX_LANG_BULGARIAN       43
#define KMX_LANG_CZECH           44
#define KMX_LANG_FINNISH         45
#define KMX_LANG_PERSIAN         46
#define KMX_LANG_SLOVAK          47
#define KMX_LANG_NORWEGIAN       48
#define KMX_LANG_SWEDISH         49
#define KMX_LANG_SPANISH         50
#define KMX_LANG_CROATIAN        51
#define KMX_LANG_ROMANIAN        52
#define KMX_LANG_SLOVENIAN       53
#define KMX_LANG_UKRAINIAN       54
#define KMX_LANG_SWAHILI         55
#define KMX_LANG_LITHUANIAN      56
#define KMX_LANG_ARMENIAN        57
#define KMX_LANG_TAGALOG         58
#define KMX_LANG_PORTUGUESE      60
#define KMX_LANG_ITALIAN         70
#define KMX_LANG_ARABIC          80
#define KMX_LANG_HINDI           90
#define KMX_LANG_HINGLISH        91
#define KMX_LANG_BENGALI         92
#define KMX_LANG_NEPALI          95//IM_NEPALI_SUPPORT
#define KMX_LANG_TELUGU          96
#define KMX_LANG_MARATHI         97
#define KMX_LANG_TAMIL           98
#define KMX_LANG_MYANMAR         99
#define KMX_LANG_BODO            110//daodejing
#define KMX_LANG_DOGRI           111
#define KMX_LANG_SANSKRIT        112
#define KMX_LANG_KASHMIRI        113
#define KMX_LANG_KONKANI         114
#define KMX_LANG_MAITHILI        115
#define KMX_LANG_MANIPURI        116
#define KMX_LANG_SANTALI         117
#define KMX_LANG_SOMALI          118
#define KMX_LANG_SINDHI          119
#define KMX_LANG_PUNJABI         81
#define KMX_LANG_GUJARATI        82
#define KMX_LANG_ORIYA           83
#define KMX_LANG_KANNADA         84
#define KMX_LANG_MALAYALAM       85

#define KMX_LANG_KHMER           86
#define KMX_LANG_LAO             87
#define KMX_LANG_SINHALA         77//IM_SINHALA_SUPPORT


#define MAX_WORD_LENGTH     KMX_MAX_LATIN_WORD_LENGTH

#define MIN_LATIN_DID       100
#define MAX_LATIN_DID       99999
#define IS_SURE_TYPE(did)   ( (did) >= 10000 ? 1 : 0 )

/* Start From Russian, Extend DID range [KMX_LANG * 100, (KMX_LANG) + 1 * 100) */
#define IS_LATIN_EXTEND_DID(did) ( ((did) >= 2000 && (did) < 10000) ? 1 : 0 )
#define LATIN_EXTEND_DID_TO_LANG(did) ( (did) / 100 )

#define KMX_LDB_ATTR_USE_KEY_STAR        1
#define KMX_LDB_ATTR_USE_KEY_POUND       2

typedef struct tagLDB_INFO
{
    int                     language;
    int                     database_id;
    int                     is_sure_type;
    int                     major_version;
    int                     minor_version;
    int                     build_number;
    const LDB_CHAR*         copyright;
    int                     copyright_length;
    int                     engine_object_size;
    int                     max_input_length;
    const LDB_CHAR*         eudw_prompt;        /* Prompt for eudw candidate */
    int                     eudw_prompt_length;
    const LDB_CHAR*         eudw_title;         /* Prompt for eudw input box */
    int                     eudw_title_length;
} LDB_INFO;

int kmx_get_db_info(
    IN const void*          database,
    OUT LDB_INFO*           info);

typedef struct
{
    int                     did;
    int                     major_version;
    int                     minor_version;
    int                     build_number;

    const LDB_CHAR*         copyright;              /* NULL-terminated string */
    int                     copyright_length;

    const LDB_CHAR*         eudw_prompt;            /* NULL-terminated string */
    int                     eudw_prompt_length;

    const LDB_CHAR*         eudw_title;             /* NULL-terminated string */
    int                     eudw_title_length;

    const LDB_CHAR*         alphabet;
    int                     alphabet_length;

    const LDB_CHAR*         lc2ucm;
    int                     lc2ucm_length;

    const LDB_CHAR*         uc2lcm;
    int                     uc2lcm_length;

    const LDB_CHAR*         sos_table;
    int                     sos_table_length;

    const LDB_CHAR*         sou_table;
    int                     sou_table_length;

    const LDB_WORD*         mt_index;               /* 1 extra item at the end */
    const LDB_CHAR*         mt_string;
    int                     mt_string_length;

    const LDB_WORD*         legram_index;           /* 1 extra item at the end */
    const LDB_CHAR*         legram_string;
    int                     legram_string_length;

    int                     word_count;
    int                     word_id_bits;
    LDB_WID                 wid_oov;
    LDB_WID                 wid_sos;

    const LDB_BYTE*         word_index;             /* 1 extra item at the end */
    int                     max_word_length;

    const LDB_BYTE*         word_table;
    int                     word_level_bits;
    int                     word_index_bits;

    int                     char_code_count;
    int                     char_code_bits;

    const LDB_CHAR*         char_table;
    const LDB_BYTE*         ckey_table;

    const LDB_BYTE*         string_table;
    int                     string_table_length;

    const LDB_BYTE*         bigram_index;           /* 1 extra item at the end */
    int                     bigram_index_count;
    int                     bigram_index_bits;
    const LDB_BYTE*         bigram;
    int                     bigram_count;

    const LDB_BYTE*         trigram;
    int                     trigram_count;

    const LDB_BYTE*         exgram_index;
    int                     exgram_index_count;
    int                     exgram_index_bits;
    const LDB_BYTE*         exgram;
} LDB;

int kmx_latin_initialize_ex(
    IN KMX_IME_ENGINE*              engine,
    IN const KMX_IME_ENGINE_INIT*   init_info);

int kmx_latin_is_chartable(
    IN KMX_IME_ENGINE*      engine,
    IN const LDB_CHAR		chr);

int kmx_latin_is_alphabeta(
    IN KMX_IME_ENGINE*      engine,
    IN const LDB_CHAR		chr);

void kmx_latin_lower_case(
    IN KMX_IME_ENGINE*      engine,
    IN unsigned short*		str,
	IN unsigned short		len);

void kmx_latin_upper_case(
    IN KMX_IME_ENGINE*      engine,
    IN unsigned short*		str,
	IN unsigned short		len);

void kmx_latin_initial_case(
    IN KMX_IME_ENGINE*      engine,
    IN unsigned short*		str,
	IN unsigned short		len);

int kmx_latin_to_lower_case(
    IN const void*          database,
    IN unsigned short*		str,
	IN unsigned short		len);

int kmx_latin_to_upper_case(
    IN const void*          database,
    IN unsigned short*		str,
	IN unsigned short		len);

int kmx_latin_to_initial_case(
    IN const void*          database,
    IN unsigned short*		str,
	IN unsigned short		len);

#ifdef __cplusplus 
} 
#endif 

#endif /* KMXIME_LATIN_H */
