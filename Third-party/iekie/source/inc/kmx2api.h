
/***************************************************************************
 
Description: Input method library interface

Copyright (C) 2000-2007 Beijing Komoxo Inc.
All rights reserved.

****************************************************************************/

#ifndef KMX2API_H
#define KMX2API_H

#ifdef __cplusplus 
extern "C"{ 
#endif 

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Basic type defines                                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_VOID     void
#define KMX_PVOID    void*
#define KMX_PCVOID   const void*

typedef signed char             KMX_INT8;
typedef unsigned char           KMX_BYTE;
typedef signed short            KMX_SHORT;
typedef unsigned short          KMX_WORD;
typedef signed  int             KMX_INT;
typedef unsigned int            KMX_UINT;
typedef signed long             KMX_LONG;
typedef unsigned long           KMX_DWORD;
typedef float                   KMX_FLOAT32;
typedef double                  KMX_DOUBLE;

typedef unsigned char           KMX_CHAR;
typedef unsigned char*          KMX_PSTR;
typedef const unsigned char*    KMX_PCSTR;

typedef unsigned short          KMX_WCHAR;
typedef unsigned short*         KMX_PWSTR;
typedef const unsigned short*   KMX_PCWSTR;

typedef int                     KMX_BOOL;

typedef unsigned long           KMX_RESULT;
typedef void *                  KMX_HANDLE;
typedef KMX_HANDLE               KMX_HIMM;
typedef KMX_HANDLE               KMX_HCORE;

typedef unsigned short          KMX_POINTERID;

#define KMX_TRUE                 1
#define KMX_FALSE                0

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Virtual key defines                                                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_VK_NONE          0x0

#define KMX_VK_1             0x31
#define KMX_VK_2             0x32
#define KMX_VK_3             0x33
#define KMX_VK_4             0x34
#define KMX_VK_5             0x35
#define KMX_VK_6             0x36
#define KMX_VK_7             0x37
#define KMX_VK_8             0x38
#define KMX_VK_9             0x39
#define KMX_VK_L1            0x3A
#define KMX_VK_L2            0x3B
#define KMX_VK_L3            0x3C
#define KMX_VK_R1            0x3D
#define KMX_VK_R2            0x3E
#define KMX_VK_R3            0x3F

#define KMX_VK_A             0x61
#define KMX_VK_BACKSPACE     0x08
#define KMX_VK_LEFT          0x25
#define KMX_VK_UP            0x26
#define KMX_VK_RIGHT         0x27
#define KMX_VK_DOWN          0x28
#define KMX_VK_SELECT        0x29

/* Symbian    在拼音/笔画中作为KMX_VK_SPACE使用, 其他情况为KMX_VK_0 */
/* Smartphone 在拼音输入法中作为KMX_VK_AUX使用, 其他情况下为KMX_VK_0 */
#define KMX_VK_0             0x30

/* Symbian    在拼音输入法中作为KMX_VK_AUX使用, 其他情况下为KMX_VK_START */
/* Smartphone 作为KMX_VK_IME使用 */
#define KMX_VK_STAR          0x2a

/* Symbian    作为KMX_VK_IME使用 */
/* Smartphone 作为KMX_VK_SPACE使用 */
#define KMX_VK_POUND         0x23

/* 功能键, 没有与键盘直接对应, 如果输入法管理器收到以下键可以直接交给Provider使用 */
#define KMX_VK_SPACE         0x20  /* 空格\切分*/
#define KMX_VK_AUX           0x41  /* 显示AUX(目前拼音专用)*/
#define KMX_VK_IME           0x40  /* 切换输入法*/

#define KMX_VK_PLUS          0x42

#define KMX_VK_LONGCLICK     0x00010000      /* 表示长按键 */

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Error code defines                                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_ERROR_SUCCESS                    0x00000000
#define KMX_ERROR_UNKNOWN                    0x00000001

#define KMX_ERROR_USED                       0x00000700
#define KMX_ERROR_UNUSED                     0x00000701
#define KMX_ERROR_TIPS                       0x00000800

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

#define KMX_LANG_CHS             1
#define KMX_LANG_CHT             2
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
#define KMX_LANG_PORTUGUESE      60
#define KMX_LANG_ITALIAN         70
#define KMX_LANG_ARABIC          80
#define KMX_LANG_HINDI           90
#define KMX_LANG_HINGLISH        91
#define KMX_LANG_BENGALI         92
#define KMX_LANG_HANGUL          93
#define KMX_LANG_UYGHUR          94
#define KMX_LANG_NEPALI          95
#define KMX_LANG_SINHALA         96
#define KMX_LANG_BODO            97//daodejing  need
#define KMX_LANG_BODO            98
#define KMX_LANG_BODO            99
#define KMX_LANG_BODO            100
#define KMX_LANG_BODO            101
#define KMX_LANG_BODO            102
#define KMX_LANG_BODO            103
#define KMX_LANG_BODO            104
#define KMX_LANG_BODO            105
#define KMX_LANG_BODO            106


/* Input method identifiers */

#define IM_SURETYPE_BASE        10000

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

#define IM_NUMBER               20

#define IM_PINYIN_TC            30
#define IM_PINYIN_FULLKEY_TC    31
#define IM_ZHUYIN_MT_SC         32
#define IM_ZHUYIN_FULLKEY_SC    33

#define IM_ENGLISH              101
#define IM_ABC_LOWER            102
#define IM_ABC_UPPER            103
#define IM_ABC_LEAD_UPPER       104
#define IM_ABC                  105
#define IM_ENGLISH_UPPER        108
#define IM_ENGLISH_LOWER        109

#define IM_RUSSIAN              201
#define IM_ABC_LOWER_RUS        202
#define IM_ABC_UPPER_RUS        203
#define IM_ABC_LEAD_UPPER_RUS   204
#define IM_RUSSIAN_UPPER        208
#define IM_RUSSIAN_LOWER        209

#define IM_GERMAN               301
#define IM_ABC_LOWER_DE         302
#define IM_ABC_UPPER_DE         303
#define IM_ABC_LEAD_UPPER_DE    304
#define IM_GERMAN_UPPER         308
#define IM_GERMAN_LOWER         309

#define IM_VIETNAMESE           311
#define IM_ABC_LOWER_VI         312
#define IM_ABC_UPPER_VI         313
#define IM_ABC_LEAD_UPPER_VI    314
#define IM_VIETNAMESE_UPPER     318
#define IM_VIETNAMESE_LOWER     319

#define IM_INDONESIAN           321
#define IM_ABC_LOWER_ID         322
#define IM_ABC_UPPER_ID         323
#define IM_ABC_LEAD_UPPER_ID    324
#define IM_INDONESIAN_UPPER     328
#define IM_INDONESIAN_LOWER     329

#define IM_TURKISH              331
#define IM_ABC_LOWER_TR         332
#define IM_ABC_UPPER_TR         333
#define IM_ABC_LEAD_UPPER_TR    334
#define IM_TURKISH_UPPER        338
#define IM_TURKISH_LOWER        339

#define IM_MALAY                341
#define IM_ABC_LOWER_MS         342
#define IM_ABC_UPPER_MS         343
#define IM_ABC_LEAD_UPPER_MS    344
#define IM_MALAY_UPPER          348
#define IM_MALAY_LOWER          349

#define IM_GREEK                351
#define IM_ABC_LOWER_EL         352
#define IM_ABC_UPPER_EL         353
#define IM_ABC_LEAD_UPPER_EL    354
#define IM_GREEK_UPPER          358
#define IM_GREEK_LOWER          359

#define IM_THAI                 361
#define IM_THAI_MT              363
#define IM_THAI_123             364

#define IM_DANISH               371
#define IM_ABC_LOWER_DA         372
#define IM_ABC_UPPER_DA         373
#define IM_ABC_LEAD_UPPER_DA    374
#define IM_DANISH_UPPER         378
#define IM_DANISH_LOWER         379

#define IM_DUTCH                381
#define IM_ABC_LOWER_NL         382
#define IM_ABC_UPPER_NL         383
#define IM_ABC_LEAD_UPPER_NL    384
#define IM_DUTCH_UPPER          388
#define IM_DUTCH_LOWER          389

#define IM_POLISH               391
#define IM_ABC_LOWER_PL         392
#define IM_ABC_UPPER_PL         393
#define IM_ABC_LEAD_UPPER_PL    394
#define IM_POLISH_UPPER         398
#define IM_POLISH_LOWER         399

#define IM_FRENCH               401
#define IM_ABC_LOWER_FR         402
#define IM_ABC_UPPER_FR         403
#define IM_ABC_LEAD_UPPER_FR    404
#define IM_FRENCH_UPPER         408
#define IM_FRENCH_LOWER         409

#define IM_HUNGARIAN            411
#define IM_ABC_LOWER_HU         412
#define IM_ABC_UPPER_HU         413
#define IM_ABC_LEAD_UPPER_HU    414
#define IM_HUNGARIAN_UPPER      418
#define IM_HUNGARIAN_LOWER      419
 
#define IM_HEBREW               421
#define IM_HEBREW_MT            423

#define IM_BULGARIAN            431
#define IM_ABC_LOWER_BG         432
#define IM_ABC_UPPER_BG         433
#define IM_ABC_LEAD_UPPER_BG    434
#define IM_BULGARIAN_UPPER      438
#define IM_BULGARIAN_LOWER      439

#define IM_CZECH                441
#define IM_ABC_LOWER_IIME         442
#define IM_ABC_UPPER_IIME         443
#define IM_ABC_LEAD_UPPER_IIME    444
#define IM_CZECH_UPPER          448
#define IM_CZECH_LOWER          449

#define IM_FINNISH              451
#define IM_ABC_LOWER_FI         452
#define IM_ABC_UPPER_FI         453
#define IM_ABC_LEAD_UPPER_FI    454
#define IM_FINNISH_UPPER        458
#define IM_FINNISH_LOWER        459

#define IM_PERSIAN              461
#define IM_PERSIAN_MT           463
#define IM_PERSIAN_123          464

#define IM_SLOVAK               471
#define IM_ABC_LOWER_SK         472
#define IM_ABC_UPPER_SK         473
#define IM_ABC_LEAD_UPPER_SK    474
#define IM_SLOVAK_UPPER         478
#define IM_SLOVAK_LOWER         479

#define IM_NORWEGIAN            481
#define IM_ABC_LOWER_NO         482
#define IM_ABC_UPPER_NO         483
#define IM_ABC_LEAD_UPPER_NO    484
#define IM_NORWEGIAN_UPPER      488
#define IM_NORWEGIAN_LOWER      489

#define IM_SWEDISH              491
#define IM_ABC_LOWER_SV         492
#define IM_ABC_UPPER_SV         493
#define IM_ABC_LEAD_UPPER_SV    494
#define IM_SWEDISH_UPPER        498
#define IM_SWEDISH_LOWER        499

#define IM_SPANISH              501
#define IM_ABC_LOWER_ES         502
#define IM_ABC_UPPER_ES         503
#define IM_ABC_LEAD_UPPER_ES    504
#define IM_SPANISH_UPPER        508
#define IM_SPANISH_LOWER        509

#define IM_CROATIAN             511
#define IM_ABC_LOWER_HR         512
#define IM_ABC_UPPER_HR         513
#define IM_ABC_LEAD_UPPER_HR    514
#define IM_CROATIAN_UPPER       518
#define IM_CROATIAN_LOWER       519

#define IM_ROMANIAN             521
#define IM_ABC_LOWER_RO         522
#define IM_ABC_UPPER_RO         523
#define IM_ABC_LEAD_UPPER_RO    524
#define IM_ROMANIAN_UPPER       528
#define IM_ROMANIAN_LOWER       529

#define IM_SLOVENIAN            531
#define IM_ABC_LOWER_SL         532
#define IM_ABC_UPPER_SL         533
#define IM_ABC_LEAD_UPPER_SL    534
#define IM_SLOVENIAN_UPPER      538
#define IM_SLOVENIAN_LOWER      539

#define IM_UKRAINIAN            541
#define IM_ABC_LOWER_UK         542
#define IM_ABC_UPPER_UK         543
#define IM_ABC_LEAD_UPPER_UK    544
#define IM_UKRAINIAN_UPPER      548
#define IM_UKRAINIAN_LOWER      549

#define IM_SWAHILI              551
#define IM_ABC_LOWER_SW         552
#define IM_ABC_UPPER_SW         553
#define IM_ABC_LEAD_UPPER_SW    554
#define IM_SWAHILI_UPPER        558
#define IM_SWAHILI_LOWER        559
           
#define IM_LITHUANIAN           561
#define IM_ABC_LOWER_LT         562
#define IM_ABC_UPPER_LT         563
#define IM_ABC_LEAD_UPPER_LT    564
#define IM_LITHUANIAN_UPPER     568
#define IM_LITHUANIAN_LOWER     569

#define IM_ARMENIAN             571
#define IM_ABC_LOWER_HY         572
#define IM_ABC_UPPER_HY         573
#define IM_ABC_LEAD_UPPER_HY    574
#define IM_ARMENIAN_UPPER       578
#define IM_ARMENIAN_LOWER       579

#define IM_PORTUGUESE           601
#define IM_ABC_LOWER_PT         602
#define IM_ABC_UPPER_PT         603
#define IM_ABC_LEAD_UPPER_PT    604
#define IM_PORTUGUESE_UPPER     608
#define IM_PORTUGUESE_LOWER     609

#define IM_ITALIAN              701
#define IM_ABC_LOWER_IT         702
#define IM_ABC_UPPER_IT         703
#define IM_ABC_LEAD_UPPER_IT    704
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
#define IM_HINGLISH_UPPER       918
#define IM_HINGLISH_LOWER       919

#define IM_BENGALI              921
#define IM_BENGALI_MT           923
#define IM_BENGALI_123          924

#define IM_HANGUL               931
#define IM_HANGUL_MT            933

#define IM_UYGHUR               941
#define IM_UYGHUR_MT            943



/* client event type */
#define KMX_EVENT_KEYBOARD       0x0001
#define KMX_EVENT_TIMER          0x0002

/* event for full keyboard or touch panel */
#define	KMX_EVENT_APPENDCHAR		0x0010
#define KMX_EVENT_REMOVECHAR		0x0020
#define KMX_EVENT_SELECTCAND		0x0040
#define KMX_EVENT_REMOVEALL		0x0080
#define KMX_EVENT_PAGENEXT		0x0100
#define KMX_EVENT_PAGEPREVIOUS	0x0200

/* client window state */
#define KMX_FLAG_STATE_EDIT      1
#define KMX_FLAG_STATE_CAND      2
#define KMX_FLAG_STATE_USER      3
#define KMX_FLAG_STATE_ACTIVE    4

/* candidate and pychoice window PAGEUP/PAGEDOWN flags */
#define KMX_FLAG_PAGEUP          0x0001
#define KMX_FLAG_PAGEDOWN        0x0002

/* timer operator */
#define KMX_TIMER_SET            1
#define KMX_TIMER_CLEAR          2

/* association flags for stroke and pinyin input method */
#define KMX_ASSOCIATION_ON              1            /* switch on association*/
#define KMX_ASSOCIATION_OFF             0            /* switch off assocition (default)*/

/* keyboard layout for five-stroke input method */
#define KMX_STROKE_STANDARD_LAYOUT      0x0001
/* national standard layout
        1-5 key: hen shu pie na zhe
        6 key  : ? anything
        7 key : magic-one for symbols, digits and english
 */

#define KMX_STROKE_IIME_LAYOUT_ONE      0x0002
/* komoxo defined keyboad layout
        1 key: magic-one for symbols, digits and english
        2-6 keys: hen shu pie na zhe
        7 key: ? anything
 */

#define KMX_PINYIN_MODE_1                0x0001      // user can edit pinyin, the "star" key is iterator choice
#define KMX_PINYIN_MODE_2                0x0002      // user can't edit pinyin, user can't step candidates
#define KMX_PINYIN_MODE_3                0x0004      // user can't edit pinyin, user can step candidates

#define KMX_CANDIDATE_MODE_1                0x0001      // 1 line candidates
#define KMX_CANDIDATE_MODE_2                0x0002      // 2 line candidates

/* fuzzy phone flags for pinyin input method */
#define KMX_FUZZY_SHENGMU_NUM    10
#define KMX_FUZZY_Z_ZH           0x0001    /* 打开模糊音 z zh*/
#define KMX_FUZZY_C_CH           0x0002    /* 打开模糊音 c ch*/
#define KMX_FUZZY_S_SH           0x0004    /* 打开模糊音 s sh */
#define KMX_FUZZY_H_F            0x0008    /* 打开模糊音 h f */
#define KMX_FUZZY_N_L            0x0010    /* 打开模糊音 n l */

#define KMX_FUZZY_YUNMU_NUM      10
#define KMX_FUZZY_IN_ING         0x0100    /* 打开模糊音 in ing */
#define KMX_FUZZY_EN_ENG         0x0200    /* 打开模糊音 en eng */
#define KMX_FUZZY_AN_ANG         0x0400    /* 打开模糊音 an ang */
#define KMX_FUZZY_IAN_IANG       0x0800    /* 打开模糊音 ian iang */
#define KMX_FUZZY_UAN_UANG       0x1000    /* 打开模糊音 uan uang */

#define KMX_EUDW_DISABLED        0x10000   /* 取消西文的用户自定义词 */

/* Language database list */

typedef struct
{
    int             input_method;
    const void*     language_database;
} KMX_IME_INFO;

extern const KMX_IME_INFO IIME_ldb_pinyin_cizu;
extern const KMX_IME_INFO IIME_ldb_pinyin_suretype;
extern const KMX_IME_INFO IIME_ldb_pinyin_danzi;
extern const KMX_IME_INFO IIME_ldb_pinyin_tc;
extern const KMX_IME_INFO IIME_ldb_pinyin_tc_suretype;

extern const KMX_IME_INFO IIME_ldb_zhuyin;
extern const KMX_IME_INFO IIME_ldb_zhuyin_cizu;
extern const KMX_IME_INFO IIME_ldb_zhuyin_danzi;
extern const KMX_IME_INFO IIME_ldb_zhuyin_sc;

extern const KMX_IME_INFO IIME_ldb_stroke_chs;
extern const KMX_IME_INFO IIME_ldb_stroke_gb2312;
extern const KMX_IME_INFO IIME_ldb_stroke_gbk;
extern const KMX_IME_INFO IIME_ldb_stroke_gb18030;
extern const KMX_IME_INFO IIME_ldb_stroke_big5;

extern const KMX_IME_INFO IIME_ldb_english;

extern const KMX_IME_INFO IIME_ldb_armenian;
extern const KMX_IME_INFO IIME_ldb_arabic;
extern const KMX_IME_INFO IIME_ldb_bengali;
extern const KMX_IME_INFO IIME_ldb_bulgarian;
extern const KMX_IME_INFO IIME_ldb_czech;
extern const KMX_IME_INFO IIME_ldb_danish;
extern const KMX_IME_INFO IIME_ldb_dutch;
extern const KMX_IME_INFO IIME_ldb_finnish;
extern const KMX_IME_INFO IIME_ldb_french;
extern const KMX_IME_INFO IIME_ldb_german;
extern const KMX_IME_INFO IIME_ldb_greek;
extern const KMX_IME_INFO IIME_ldb_hebrew;
extern const KMX_IME_INFO IIME_ldb_hindi;
extern const KMX_IME_INFO IIME_ldb_hinglish;
extern const KMX_IME_INFO IIME_ldb_hungarian;
extern const KMX_IME_INFO IIME_ldb_indonesian;
extern const KMX_IME_INFO IIME_ldb_italian;
extern const KMX_IME_INFO IIME_ldb_lithuanian;
extern const KMX_IME_INFO IIME_ldb_malay;
extern const KMX_IME_INFO IIME_ldb_norwegian;
extern const KMX_IME_INFO IIME_ldb_persian;
extern const KMX_IME_INFO IIME_ldb_polish;
extern const KMX_IME_INFO IIME_ldb_portuguese;
extern const KMX_IME_INFO IIME_ldb_romanian;
extern const KMX_IME_INFO IIME_ldb_russian;
extern const KMX_IME_INFO IIME_ldb_slovak;
extern const KMX_IME_INFO IIME_ldb_slovenian;
extern const KMX_IME_INFO IIME_ldb_spanish;
extern const KMX_IME_INFO IIME_ldb_swahili;
extern const KMX_IME_INFO IIME_ldb_swedish;
extern const KMX_IME_INFO IIME_ldb_thai;
extern const KMX_IME_INFO IIME_ldb_turkish;
extern const KMX_IME_INFO IIME_ldb_ukrainian;
extern const KMX_IME_INFO IIME_ldb_vietnamese;
extern const KMX_IME_INFO IIME_ldb_hangul;
extern const KMX_IME_INFO IIME_ldb_uyghur;

extern const KMX_IME_INFO IIME_ldb_english_suretype;

/* Input method configuration table */

typedef const KMX_IME_INFO* KMX_IME_INFO_PTR;

extern const KMX_IME_INFO_PTR IIME_input_method_config[];

/* language params*/
typedef struct
{
    int             dwLangId;           /* [in]  language id*/
    int             nMethods;           /* [out] the input methods number of current language*/
    unsigned short  idIM[12];           /* [out] input methods id, max is 8, which is IM_PINYIN, IM_BIHUA, IM_ABC, etc.*/
} KMX_LANG_PARAMS, *PKMX_LANG_PARAMS;

typedef int (*KMX_GET_TEXT_WIDTH_FUNC)(const unsigned short* text, int length);

/* UI specified parameters */
typedef struct
{
    int             nMaxPYChoice;        /* [in] max display number of pinyin combination*/
    int             nWinWidth;           /* [in] Candidate window width*/
    int             nLableWidth;         /* [in] Lable width*/
    KMX_GET_TEXT_WIDTH_FUNC pGetTextWidthFunc;
}KMX_IMM_PARAMS, *PKMX_IMM_PARAMS;

/* user-defined-word and auto-frequence-adjustment data params*/
typedef struct
{
    int             iUserCacheSize;     /* [in] user-word buffer size*/
    char*           pUserCache;         /* [in] user-word buffer address*/
} KMX_USER_PARAMS, *PKMX_USER_PARAMS;

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
    unsigned short pCandBuffer[32];     /* buffer of candidate window*/
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
} KMX_IMM_CONTENT, *PKMX_IMM_CONTENT;

/* options for input method*/
typedef struct
{
    unsigned long   dwPYFuzzy;      /* 模糊音设置*/
    unsigned long   dwStrokeLayout; /* 笔画键盘布局*/
    unsigned long   dwAssoPinyin;   /* 拼音联想设置*/
    unsigned long   dwAssoStroke;   /* 笔画联想设置*/
    unsigned long   dwPinyinMode;   /* 拼音操作模式 */
    unsigned long   dwCandidateMode;/* 候选项模式 */
} KMX_IMM_OPTIONS, *PKMX_IMM_OPTIONS;

/* Initialization/Cleanup */
KMX_HIMM KMX_IMM_Initialize(PKMX_LANG_PARAMS pLangParams, PKMX_IMM_PARAMS pImmParams, PKMX_USER_PARAMS pUserParam);
KMX_RESULT KMX_IMM_Terminate(KMX_HIMM hImm);

/* Input method selection */
KMX_UINT KMX_IMM_GetInputMethod(KMX_HIMM hImm);
KMX_RESULT KMX_IMM_SetInputMethod(KMX_HIMM hImm, KMX_UINT iNewMethod);

/* Event handler */
KMX_RESULT KMX_IMM_EventHandle(KMX_HIMM hImm, KMX_UINT hEvent, KMX_UINT iMsg, PKMX_IMM_CONTENT pContent);

/* Options */
KMX_RESULT KMX_IMM_SetOptionsEx(KMX_HIMM hImm, const KMX_IMM_OPTIONS* pOptions);
KMX_RESULT KMX_IMM_GetOptionsEx(KMX_HIMM hImm, PKMX_IMM_OPTIONS pOptions);

/* add user phase */
/* the input method must IM_PINYIN */
KMX_RESULT KMX_IMM_AddPhase(KMX_HIMM hImm, unsigned short *szPhrase, int nLen);

KMX_RESULT KMX_IMM_CheckAsso(KMX_HIMM hImm, unsigned short wContext, unsigned short *szAsso, int *pNumber);

KMX_RESULT KMX_IMM_QueryProperties(KMX_HIMM hImm, int *pProperties);

const char* KMX_IMM_GetVersion(void);

/* Query Hanzi Association */
KMX_RESULT   HanziAsso_QueryAsso(KMX_BOOL  isPinyin,
								KMX_WCHAR wContext,	/* 上下文内容unicode字符串，如“开始” */
                                KMX_PWSTR pwzAsso,		/* 返回的联想候选序列*/
								KMX_INT  *pnAsso);		/* 输入为所要的联想最大个数，返回的是实际候选个数*/

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

kmx_active_state KMX_IMM_GetActiveState(void);

void KMX_IMM_SetOutputNumber(unsigned int);

int mmi_get_candidate_window_metriIIME(CANDIDATE_WINDOW_METRIIIME* metriIIME);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese pinyin and bpmf query                                           */
/*                                                                         */
/*-------------------------------------------------------------------------*/

KMX_BOOL KMX_IMM_QueryCodePinyin(KMX_WORD wCode, KMX_WCHAR *wszPinyin);

KMX_BOOL KMX_IMM_QueryCodeZhuyin(KMX_WORD wCode, KMX_WCHAR *wszZhuyin);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese stroke query                                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Stroke code constants */

#define KMX_IMM_STROKE_H     1   /* Heng */
#define KMX_IMM_STROKE_S     2   /* Shu  */
#define KMX_IMM_STROKE_P     3   /* Pie  */
#define KMX_IMM_STROKE_D     4   /* Dian */
#define KMX_IMM_STROKE_Z     5   /* Zhe  */

/* Maximum stroke count of Chinese character */

#define KMX_IMM_MAX_STROKE   63

int KMX_IMM_QueryStroke_gb(
    unsigned short char_code,
    char* buffer,
    int buffer_length);

int KMX_IMM_QueryStroke_gbk(
    unsigned short char_code,
    char* buffer,
    int buffer_length);

int KMX_IMM_QueryStroke_big5(
    unsigned short char_code,
    char* buffer,
    int buffer_length);

#ifdef __cplusplus 
} 
#endif 

#endif /* IIMEK2API_H */
