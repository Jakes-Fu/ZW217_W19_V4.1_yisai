
/*
 * cstar_ime.c
 *
 * Implementation of ilMETAR input method user interface.
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

/*
 * Platform headers
 *
 * TO DO:
 *
 * Replace code here to include platform dependent headers.
 */


#include "gui.h"
#include "sms.h"
#include "debug.h"

/*
 * ilMETAR input method private headers
 */

#include "csk2api.h"

#include "cstar_ime.h"
#include "update_data.h"

/*
 * Platform dependent data type wrappers
 *
 * TO DO:
 *
 * Replace code here to define platform dependent data types.
 */

#define ilME_UI_FONT              int
#define ilME_UI_COLOR             int
#define ilME_UI_RECT              RECT
#define ilME_UI_WCHAR             unsigned short

/*
 * Configurations
 */

#define ilME_IDT_WIDTH            8       /* Indicator pixel width            */
#define ilME_IDT_HEIGHT           11      /* Indicator pixel height           */

#define ilME_CAND_EXTRA_SPACE     0       /* Extra space between candidates   */
#define ilME_SIZE_CARET           1       /* Caret pixel width                */

#define ilME_IME_TIME_OUT         1500     /* Timer time-out value, milliseconds */

/*
 * Local definitions
 */

#define ilME_MAX_COMMIT           64
#define ilME_MAX_COMP             64
#define ilME_MAX_CAND             32
#define ilME_MAX_EUDW             64
#define ilME_MAX_INLINE           64
#define ilME_MAX_AUX              64

#define ilME_CTAB_FIRST           0x21
#define ilME_CTAB_LAST            0x7E
#define ilME_CTAB_SIZE            0x5E    /* ilME_CTAB_LAST - ilME_CTAB_FIRST + 1 */

#define ilME_UI_STATE_COMP        0
#define ilME_UI_STATE_CAND        1
#define ilME_UI_STATE_EUDW        2

#define ilME_UI_MAX_COMP_ITEM     3
#define ilME_UI_MAX_COMP_TEXT     64

#define ilME_UI_MAX_CAND_ITEM     18
#define ilME_UI_MAX_CAND_TEXT     64

#define ilME_UI_MAX_AUX_ITEM      3
#define ilME_UI_MAX_AUX_TEXT      64

typedef struct tagilME_UI_THEMES
{
    ilME_UI_FONT                  text_font;              /* Normal text font             */
    ilME_UI_FONT                  label_font;             /* Candidate label font         */
    ilME_UI_COLOR                 text_color;             /* Normal text color            */
    ilME_UI_COLOR                 bk_color;               /* Normal text background color */
    ilME_UI_COLOR                 hot_color1;             /* Extra hot color              */
    ilME_UI_COLOR                 hot_color2;             /* Extra hot color              */
    ilME_UI_COLOR                 hot_color3;             /* Extra hot color              */
} ilME_UI_THEMES;

typedef struct tagilME_UI_WNDPOS
{
    ilME_UI_RECT                  appwnd;                 /* application window position  */
    ilME_UI_RECT                  imewnd;                 /* current ime window position  */

    int                         comp_height;            /* Compisition area height      */
    ilME_UI_RECT                  comp_item;              /* Compisition item area        */
    ilME_UI_RECT                  comp_lidt;              /* Composition left indicator   */
    ilME_UI_RECT                  comp_ridt;              /* Composition right indicator  */

    int                         aux_height;             /* Aux area height              */
    ilME_UI_RECT                  aux_item;               /* Aux item area                */
    ilME_UI_RECT                  aux_lidt;               /* Aux left indicator           */
    ilME_UI_RECT                  aux_ridt;               /* Aux right indicator          */

    int                         cand_height;            /* Candidate area height        */
    ilME_UI_RECT                  cand_item;              /* Candidate item area          */
    ilME_UI_RECT                  cand_lidt;              /* Candidate left indicator     */
    ilME_UI_RECT                  cand_ridt;              /* Candidate right indicator    */
} ilME_UI_POS;

typedef struct tagilME_UI_METRIilME
{
    int                         text_height;            /* Normal text line height      */
    int                         label_width;            /* Label character width        */
    int                         min_cand_width;         /* Min candidate item width     */
    int                         cand_extra_space;       /* Extra space between cand     */
} ilME_UI_METRIilME;

typedef struct tagilME_UI_ITEM
{
    ilME_UI_RECT                  item_pos;               /* Background position          */
    ilME_UI_COLOR                 item_color;             /* Background color             */
    ilME_UI_RECT                  text_pos;               /* Text position                */
    ilME_UI_WCHAR*                text_str;               /* Text string                  */
    int                         text_len;               /* Text string length           */
    ilME_UI_FONT                  text_font;              /* Text font                    */
    ilME_UI_COLOR                 text_color;             /* Text color                   */
} ilME_UI_ITEM;

typedef struct tagilME_UI_CONTENT
{
    /* Composition content */
    int                         comp_item_count;
    ilME_UI_ITEM                  comp_item[ilME_UI_MAX_COMP_ITEM];
    ilME_UI_WCHAR                 comp_text[ilME_UI_MAX_COMP_TEXT];
    int                         comp_lidt_visible;
    ilME_UI_COLOR                 comp_lidt_color;
    int                         comp_ridt_visible;
    ilME_UI_COLOR                 comp_ridt_color;
    int                         comp_caret_visible;
    ilME_UI_RECT                  comp_caret_pos;
    ilME_UI_COLOR                 comp_caret_color;

    /* Aux content */
    int                         aux_item_count;
    ilME_UI_ITEM                  aux_item[ilME_UI_MAX_AUX_ITEM];
    ilME_UI_WCHAR                 aux_text[ilME_UI_MAX_AUX_TEXT];
    int                         aux_lidt_visible;
    ilME_UI_COLOR                 aux_lidt_color;
    int                         aux_ridt_visible;
    ilME_UI_COLOR                 aux_ridt_color;

    /* Candidate content */
    int                         cand_item_count;
    ilME_UI_ITEM                  cand_item[ilME_UI_MAX_CAND_ITEM];
    ilME_UI_WCHAR                 cand_text[ilME_UI_MAX_CAND_TEXT];
    int                         cand_lidt_visible;
    ilME_UI_COLOR                 cand_lidt_color;
    int                         cand_ridt_visible;
    ilME_UI_COLOR                 cand_ridt_color;
    int                         cand_caret_visible;
    ilME_UI_RECT                  cand_caret_pos;
    ilME_UI_COLOR                 cand_caret_color;

    /* Candidate 2 content */
    int                         cand_item_count2;
    ilME_UI_ITEM                  cand_item2[ilME_UI_MAX_CAND_ITEM];
    ilME_UI_WCHAR                 cand_text2[ilME_UI_MAX_CAND_TEXT];
} ilME_UI_CONTENT;

int                             cs_initialized = 0;

/* User interface related data */

static int                      cs_is_rtl = 0;

static ilME_UI_THEMES             cs_ui_themes;
static ilME_UI_METRIilME            cs_ui_metrics;
static ilME_UI_POS                cs_ui_pos;

static int                      cs_ui_visible = 0;
static int                      cs_ui_state;
static int                      cs_ui_timer_actived = 0;

static ilME_UI_CONTENT            cs_ui_content;

/* Platform persistent storage related data */

static void*                    cs_imm_user_data;
static int                      cs_imm_user_data_size;
static ilME_IMM_OPTIONS           cs_imm_options;

/* Input method kernel related data */

static ilME_HIMM                  cs_imm = 0;
static int                      cs_imm_mode;
static ilME_IMM_CONTENT           cs_imm_content;

/* for IM_USER_MULTITYPE */
static const MULTI_STRING long_multi_str[] = {
    L"00+0",
};

static const MULTI_STRING  multi_str[10] = {
    L"111",
    L"22abc2",
    L"33def3", 
    L"44hij4", 
    L"55jkl5", 
    L"66mno6", 
    L"77pqrs7", 
    L"88tuv8", 
    L"99wxyz9", 
    L"**PW*"
};

static const QWERTY_KEYMAP zhuyin_key2char[28] = 
{
    {'.', {0x02C9, 0x02CA, 0x02C7, 0x02CB, 0x02D9, 0}},
    {'a', {0x3107, 0x3108, 0}},
    {'b', {0x311D, 0}},
    {'c', {0x3119, 0}},
    {'d', {0x310E, 0x3111, 0}},
    {'e', {0x310D, 0x3110, 0}},
    {'f', {0x3115, 0}},
    {'g', {0x3118, 0}},
    {'h', {0x3128, 0}},
    {'i', {0x311E, 0x311F}},
    {'j', {0x311C, 0}},
    {'k', {0x3120, 0}},
    {'l', {0x3124, 0}},
    {'m', {0x3125, 0}},
    {'n', {0x3121, 0}},
    {'o', {0x3122, 0x3123}},
    {'p', {0x3126, 0}},
    {'q', {0x3105, 0x3106}},
    {'r', {0x3113, 0x3114}},
    {'s', {0x310B, 0x310C}},
    {'t', {0x3117, 0}},
    {'u', {0x311A, 0x311B}},
    {'v', {0x3129, 0}},
    {'w', {0x3109, 0x310A}},
    {'x', {0x3116, 0}},
    {'y', {0x3127, 0}},
    {'z', {0x310F, 0x3112}},
    {0, {0}}
};

static const ilME_MULTI_STRING_TABLE multi_string_table = 
{
    10,
    1,
    multi_str,
    long_multi_str
};


const ilME_QWERTY_INFO cs_qwerty_info[] = 
{
#ifndef INTERFACE_08_ONLY
    #ifdef KMX_LDB_THAI
    {ilME_LANG_THAI,   DefaultThaiMapping},
    #endif
    #ifdef KMX_LDB_ARMENIAN
    {ilME_LANG_ARMENIAN,   DefaultArmenianMapping},
    #endif
    #ifdef KMX_LDB_BULGARIAN
    {ilME_LANG_BULGARIAN,   DefaultBulgarianMapping},
    #endif
    #ifdef KMX_LDB_CROATIAN
    {ilME_LANG_CROATIAN,   DefaultCroatianMapping},
    #endif
    #ifdef KMX_LDB_CZECH
    {ilME_LANG_CZECH,   DefaultCzechMapping},
    #endif
    #ifdef KMX_LDB_DANISH
    {ilME_LANG_DANISH,   DefaultDanishMapping},
    #endif
    #ifdef KMX_LDB_DUTCH
    {ilME_LANG_DUTCH,   DefaultDutchMapping},
    #endif
    #ifdef KMX_LDB_FINNISH
    {ilME_LANG_FINNISH,   DefaultFinnishMapping},
    #endif
    #ifdef KMX_LDB_FRENCH
    {ilME_LANG_FRENCH,   DefaultFrenchMapping},
    #endif
    #ifdef KMX_LDB_GERMAN
    {ilME_LANG_GERMAN,   DefaultGermanMapping},
    #endif
    #ifdef KMX_LDB_GREEK
    {ilME_LANG_GREEK,   DefaultGreekMapping},
    #endif
    #ifdef KMX_LDB_HUNGARIAN
    {ilME_LANG_HUNGARIAN,   DefaultHungarianMapping},
    #endif
    #ifdef KMX_LDB_ITALIAN
    {ilME_LANG_ITALIAN,   DefaultItalianMapping},
    #endif
    #ifdef KMX_LDB_LITHUANIAN
    {ilME_LANG_LITHUANIAN,   DefaultLithuanianMapping},
    #endif
    #ifdef KMX_LDB_NORWEGIAN
    {ilME_LANG_NORWEGIAN,   DefaultNorwegianMapping},
    #endif
    #ifdef KMX_LDB_POLISH
    {ilME_LANG_POLISH,   DefaultPolishMapping},
    #endif
    #ifdef KMX_LDB_PORTUGUESE
    {ilME_LANG_PORTUGUESE,   DefaultPortugueseMapping},
    #endif
    #ifdef KMX_LDB_ROMANIAN
    {ilME_LANG_ROMANIAN,   DefaultRomanianMapping},
    #endif
    #ifdef KMX_LDB_RUSSIAN
    {ilME_LANG_RUSSIAN,   DefaultRussianMapping},
    #endif
    #ifdef KMX_LDB_SLOVAK
    {ilME_LANG_SLOVAK,   DefaultSlovakMapping},
    #endif
    #ifdef KMX_LDB_SLOVENIAN
    {ilME_LANG_SLOVENIAN,   DefaultSlovenianMapping},
    #endif
    #ifdef KMX_LDB_SPANISH
    {ilME_LANG_SPANISH,   DefaultSpanishMapping},
    #endif
    #ifdef KMX_LDB_SWEDISH
    {ilME_LANG_SWEDISH,   DefaultSwedishMapping},
    #endif
    #ifdef KMX_LDB_TURKISH
    {ilME_LANG_TURKISH,   DefaultTurkishMapping},
    #endif
    #ifdef KMX_LDB_UKRAINIAN
    {ilME_LANG_UKRAINIAN,   DefaultUkrainianMapping},
    #endif
    #ifdef KMX_LDB_ARABIC
    {ilME_LANG_ARABIC,   DefaultArabicMapping},
    #endif
    #ifdef KMX_LDB_HEBREW
    {ilME_LANG_HEBREW,   DefaultHebrewMapping},
    #endif
    #ifdef KMX_LDB_PERSIAN
    {ilME_LANG_PERSIAN,   DefaultPersianMapping},
    #endif
    #ifdef KMX_LDB_URDU
    {ilME_LANG_URDU,   DefaultUrduMapping},
    #endif
    #ifdef KMX_LDB_UYGHUR
    {ilME_LANG_UYGHUR,   DefaultUyghurMapping},
    #endif
    #ifdef KMX_LDB_HINDI
    {ilME_LANG_HINDI,   DefaultHindiMapping},
    #endif
    #ifdef KMX_LDB_NEPALI
    {ilME_LANG_NEPALI,   DefaultNepaliMapping},	
    #endif
    #ifdef KMX_LDB_SINHALA
    {ilME_LANG_SINHALA,   DefaultSinhalaMapping},	
    #endif
	//daodejing
    #ifdef KMX_LDB_BODO
    {ilME_LANG_BODO,   DefaultBodoMapping},
    #endif
	#ifdef KMX_LDB_DOGRI
    {ilME_LANG_DOGRI,   DefaultDogriMapping},
    #endif
	#ifdef KMX_LDB_SANSKRIT
    {ilME_LANG_SANSKRIT,   DefaultSanskritMapping},
    #endif
	#ifdef KMX_LDB_KASHMIRI
    {ilME_LANG_KASHMIRI,   DefaultKashmiriMapping},
    #endif
	#ifdef KMX_LDB_KONKANI
    {ilME_LANG_KONKANI,   DefaultKonkaniMapping},
    #endif
	#ifdef KMX_LDB_MAITHILI
    {ilME_LANG_MAITHILI,   DefaultMaithiliMapping},
    #endif
	#ifdef KMX_LDB_MANIPURI
    {ilME_LANG_MANIPURI,   DefaultManipuriMapping},
    #endif
	#ifdef KMX_LDB_SANTALI
    {ilME_LANG_SANTALI,   DefaultSantaliMapping},
    #endif
	#ifdef KMX_LDB_SOMALI
    {ilME_LANG_SOMALI,   DefaultSomaliMapping},
    #endif
	#ifdef KMX_LDB_SINDHI
    {ilME_LANG_SINDHI,   DefaultSindhiMapping},
    #endif
    #ifdef KMX_LDB_BENGALI
    {ilME_LANG_BENGALI,   DefaultBengaliMapping},
    #endif
    #ifdef KMX_LDB_TELUGU
    {ilME_LANG_TELUGU,   DefaultTeluguMapping},
    #endif
    #ifdef KMX_LDB_MARATHI
    {ilME_LANG_MARATHI,   DefaultMarathiMapping},
    #endif
    #ifdef KMX_LDB_TAMIL
    {ilME_LANG_TAMIL,   DefaultTamilMapping},
    #endif
    #ifdef KMX_LDB_PUNJABI
    {ilME_LANG_PUNJABI,   DefaultPunjabiMapping},
    #endif
    #ifdef KMX_LDB_GUJARATI
    {ilME_LANG_GUJARATI,   DefaultGujaratiMapping},
    #endif
    #ifdef KMX_LDB_ORIYA
    {ilME_LANG_ORIYA,   DefaultOriyaMapping},
    #endif
    #ifdef KMX_LDB_KANNADA
    {ilME_LANG_KANNADA,   DefaultKannadaMapping},
    #endif
    #ifdef KMX_LDB_MALAYALAM
    {ilME_LANG_MALAYALAM,   DefaultMalayalamMapping},
    #endif
    #ifdef KMX_LDB_KHMER
    {ilME_LANG_KHMER,   DefaultKhmerMapping},
    #endif
    #ifdef KMX_LDB_LAO
    {ilME_LANG_LAO,   DefaultLaoMapping},
    #endif
    #ifdef KMX_LDB_MYANMAR
    {ilME_LANG_MYANMAR,   DefaultMyanmarMapping},
    #endif
    #ifdef KMX_LDB_NEPALI
    {ilME_LANG_NEPALI,   DefaultNepaliMapping},
    #endif
    #ifdef KMX_LDB_SINHALA
    {ilME_LANG_SINHALA,   DefaultSinhalaMapping},
    #endif	
    #ifdef KMX_LDB_ASSAMESE
    {ilME_LANG_ASSAMESE,   DefaultAssameseMapping},
    #endif
#endif /* INTERFACE_08_ONLY */
    0
};


/* this value is used for vietnamese input */
/*static ilME_WCHAR                   cs_last_commit; */

/*
 * Prototype of local functions
 */

static int ilME_GuiMode2CstarMode(int input_method);

static void ilME_LoadUserData(void);
static void ilME_SaveUserData(void);

static void ilME_LoadOptions(void);
static void ilME_GetDefaultOptions(ilME_IMM_OPTIONS* pOptions);

static void ilME_InitUiThemes(void);
static void ilME_InitUiMetrics(void);

static void ilME_InitUiWindow(void);
static void ilME_CloseUiWindow(void);

static void ilME_InitUi(void);
static void ilME_CloseUi(void);

static void ilME_InitKernel(void);
static void ilME_CloseKernel(void);

static void ilME_ShowWnd(void);
static void ilME_HideWnd(void);

static void ilME_PaintWnd(void);
static void ilME_PaintLeftIndicator(const ilME_UI_RECT* r, ilME_UI_COLOR c);
static void ilME_PaintRightIndicator(const ilME_UI_RECT* r, ilME_UI_COLOR c);

static void ilME_CommitText(const ilME_UI_WCHAR* text, int len);
static void ilME_CommitOutput(void);

static void ilME_ParseCompContent(void);
static void ilME_ParseCandContent(void);
static void ilME_ParseEudwContent(void);
static void ilME_ParseAuxContent(void);
static void ilME_ParseInlineContent(void);
static void ilME_ParseCandContent2(void);
static void ilME_UpdateWnd(void);
static void ilME_ProcessContent(void);

static void ilME_OnTimerEvent(void);
static void ilME_StartTimer(void);
static void ilME_StopTimer(void);

static int ilME_OnNum0Key(int flags);
static int ilME_OnNum1Key(int flags);
static int ilME_OnNum2Key(int flags);
static int ilME_OnNum3Key(int flags);
static int ilME_OnNum4Key(int flags);
static int ilME_OnNum5Key(int flags);
static int ilME_OnNum6Key(int flags);
static int ilME_OnNum7Key(int flags);
static int ilME_OnNum8Key(int flags);
static int ilME_OnNum9Key(int flags);

static int ilME_OnLeftArrowKey(int flags);
static int ilME_OnRightArrowKey(int flags);
static int ilME_OnUpArrowKey(int flags);
static int ilME_OnDownArrowKey(int flags);

static int ilME_OnLeftSoftKey(int flags);
static int ilME_OnRightSoftKey(int flags);

static int ilME_OnStarKey(int flags);
static int ilME_OnPoundKey(int flags);

/*
 * Implementation of local functions
 */

static int ilME_GetContext(const unsigned short **context, int *length)
{
    return mle_get_context_string(&sms_editor, context, length);
}

static int ilME_CheckValidHanzi(unsigned short unichr)
{
	if (unichr == 0x517A || unichr == 0xFF0C)
		return 0;
	else
		return 1;
}

static int ilME_GuiMode2CstarMode(int input_method)
/*
 * Convert platform dependent input method ID to ilMETAR input method ID
 *
 */
{
    /*
     * TO DO:
     *
     * Replace code here to convert input method ID.
     *
     * List of ilMETAR input method ID:
     *
     *      IM_PINYIN     --> 拼音输入法
     *      IM_STROKE     --> 笔画输入法
     *      IM_ENGLISH    --> 智能英文输入法
     *      IM_ABC_LOWER  --> 小写英文Multitap输入法
     *      IM_ABC_UPPER  --> 大写英文Multitap输入法
     *      IM_123        --> 数字输入法
     */

    return input_method;
}

static void ilME_LoadUserData(void)
/*
 * Load/initialize user data from persistent storage
 */
{
    /*
     * TO DO:
     *
     * Replace code here to initialize user data.
     *
     *      1. Load user data from persistent storage.
     *      2. Initialize cs_imm_user_data to the address of user data.
     *      3. Initialize cs_imm_user_data_size to the byte size of user data.
     *
     * For debugging, the following code use non-persistent storage as user data:
     *
     *      static user_data[8192];
     *      cs_imm_user_data = user_data;
     *      cs_imm_user_data_size = 8192;
     */

    cs_imm_user_data = gui_load_ime_userdata(&cs_imm_user_data_size);
}

static void ilME_SaveUserData(void)
/*
 * Save user data to persistent storage
 */
{
    /*
     * TO DO:
     *
     * Replace code here to save user data back to persistent storage.
     */

    gui_save_ime_userdata(cs_imm_user_data, cs_imm_user_data_size);
}

static void ilME_LoadOptions(void)
/*
 * Load/initialize IME options from persistent storage
 */
{
    /*
     * TO DO: (Optional)
     *
     * If input method options can be set by end user, replace code
     * here to access persistent storage to load options data.
     * Otherwise, use build-in default options.
     *
     * Default options can be obtained by ilME_GetDefaultOptions()
     */

    void* imeoptiondata;
    int imeoptiondata_size;

    imeoptiondata = gui_load_ime_options(&imeoptiondata_size);

    if ( imeoptiondata && (imeoptiondata_size == sizeof(ilME_IMM_OPTIONS)) )
    {
        memcpy(&cs_imm_options, imeoptiondata, imeoptiondata_size);
    }
    else
    {
        ilME_GetDefaultOptions(&cs_imm_options);
    }

    cs_imm_options.pfnGetContext  = ilME_GetContext;
	cs_imm_options.pfnCheckValidHanzi = ilME_CheckValidHanzi;
    cs_imm_options.pQwertyInfo = cs_qwerty_info;
}

static void ilME_GetDefaultOptions(ilME_IMM_OPTIONS* pOptions)
/*
 * Returns default IME options
 */
{
    pOptions->dwPYFuzzy      = 0;
    pOptions->dwStrokeLayout = ilME_STROKE_STANDARD_LAYOUT;
    pOptions->dwAssoPinyin   = ilME_ASSOCIATION_ON;
    pOptions->dwAssoStroke   = ilME_ASSOCIATION_ON;
    pOptions->dwPinyinMode   = ilME_PINYIN_MODE_1;
    pOptions->dwMarScript    = 1;
}

static void ilME_InitUiThemes(void)
/*
 * Initialize user interface themes
 */
{
    cs_ui_themes.text_font  = GUI_SYSTEM_FONT;
    cs_ui_themes.label_font = GUI_SMALL_FONT;

    cs_ui_themes.bk_color   = RGB(240, 240, 255);
    cs_ui_themes.text_color = RGB(0, 0, 0);
    cs_ui_themes.hot_color1 = RGB(0, 0, 255);
    cs_ui_themes.hot_color2 = RGB(255, 255, 255);
    cs_ui_themes.hot_color3 = RGB(210, 210, 210);
}

static void ilME_InitUiMetrics(void)
/*
 * Initialize user interface metrics
 */
{
    cs_ui_metrics.text_height = gui_get_text_height(cs_ui_themes.text_font);
    cs_ui_metrics.label_width = gui_get_char_width(cs_ui_themes.label_font, 0x38);
    cs_ui_metrics.min_cand_width = gui_get_char_width(cs_ui_themes.text_font, 0x57);
    cs_ui_metrics.cand_extra_space = ilME_CAND_EXTRA_SPACE;
}

static void ilME_InitUiWindow(void)
/*
 * Initialize IME window
 */
{
    int width, height;

    cs_ui_visible = 0;

    cs_ui_pos.appwnd.left       = 0;
    cs_ui_pos.appwnd.top        = GUI_BAR_HEIGHT;
    cs_ui_pos.appwnd.right      = GUI_WIDTH;
    cs_ui_pos.appwnd.bottom     = GUI_HEIGHT - GUI_BAR_HEIGHT;

    cs_ui_pos.imewnd.left       = cs_ui_pos.appwnd.left;
    cs_ui_pos.imewnd.top        = cs_ui_pos.appwnd.bottom;
    cs_ui_pos.imewnd.right      = cs_ui_pos.appwnd.right;
    cs_ui_pos.imewnd.bottom     = cs_ui_pos.appwnd.bottom;

    width  = cs_ui_pos.appwnd.right - cs_ui_pos.appwnd.left;
    height = cs_ui_metrics.text_height;

    /* For composition content */

    cs_ui_pos.comp_height       = height;

    cs_ui_pos.comp_lidt.left    = 0;
    cs_ui_pos.comp_lidt.right   = ilME_IDT_WIDTH;
    cs_ui_pos.comp_lidt.top     = 0;
    cs_ui_pos.comp_lidt.bottom  = height;

    if (cs_imm_options.dwPinyinMode == ilME_PINYIN_MODE_5 && cs_imm_mode == IM_PINYIN)
    {
        cs_ui_pos.comp_ridt.left    = width / 2 - ilME_IDT_WIDTH;
        cs_ui_pos.comp_ridt.right   = width / 2;
        cs_ui_pos.comp_ridt.top     = 0;
        cs_ui_pos.comp_ridt.bottom  = height;

        cs_ui_pos.comp_item.left    = cs_ui_pos.comp_lidt.right;
        cs_ui_pos.comp_item.right   = cs_ui_pos.comp_ridt.left;
        cs_ui_pos.comp_item.top     = 0;
        cs_ui_pos.comp_item.bottom  = height;
    }
    else
    {
        cs_ui_pos.comp_ridt.left    = width - ilME_IDT_WIDTH;
        cs_ui_pos.comp_ridt.right   = width;
        cs_ui_pos.comp_ridt.top     = 0;
        cs_ui_pos.comp_ridt.bottom  = height;

        cs_ui_pos.comp_item.left    = cs_ui_pos.comp_lidt.right;
        cs_ui_pos.comp_item.right   = cs_ui_pos.comp_ridt.left;
        cs_ui_pos.comp_item.top     = 0;
        cs_ui_pos.comp_item.bottom  = height;
    }
    /* For aux content */

    cs_ui_pos.aux_height        = height;

    if (cs_imm_options.dwPinyinMode == ilME_PINYIN_MODE_5 && cs_imm_mode == IM_PINYIN)
    {
        cs_ui_pos.aux_lidt.left     = width / 2;
        cs_ui_pos.aux_lidt.right    = width / 2 + ilME_IDT_WIDTH;
        cs_ui_pos.aux_lidt.top      = 0;
        cs_ui_pos.aux_lidt.bottom   = height;
    }
    else
    {
        cs_ui_pos.aux_lidt.left     = 0;
        cs_ui_pos.aux_lidt.right    = ilME_IDT_WIDTH;
        cs_ui_pos.aux_lidt.top      = 0;
        cs_ui_pos.aux_lidt.bottom   = height;
    }
    cs_ui_pos.aux_ridt.left     = width - ilME_IDT_WIDTH;
    cs_ui_pos.aux_ridt.right    = width;
    cs_ui_pos.aux_ridt.top      = 0;
    cs_ui_pos.aux_ridt.bottom   = height;

    if (cs_imm_options.dwPinyinMode == ilME_PINYIN_MODE_5 && cs_imm_mode == IM_PINYIN)
    {
        cs_ui_pos.aux_item.left     = cs_ui_pos.aux_lidt.right;
        cs_ui_pos.aux_item.right    = cs_ui_pos.aux_ridt.left;
        cs_ui_pos.aux_item.top      = 0;
        cs_ui_pos.aux_item.bottom   = height;
    }
    else
    {
        cs_ui_pos.aux_item.left     = cs_ui_pos.aux_lidt.right;
        cs_ui_pos.aux_item.right    = cs_ui_pos.aux_ridt.left;
        cs_ui_pos.aux_item.top      = 0;
        cs_ui_pos.aux_item.bottom   = height;
    }
    /* For candidate content */

    cs_ui_pos.cand_height       = height;

    cs_ui_pos.cand_lidt.left    = 0;
    cs_ui_pos.cand_lidt.right   = ilME_IDT_WIDTH;
    cs_ui_pos.cand_lidt.top     = 0;
    cs_ui_pos.cand_lidt.bottom  = height;

    cs_ui_pos.cand_ridt.left    = width - ilME_IDT_WIDTH;
    cs_ui_pos.cand_ridt.right   = width;
    cs_ui_pos.cand_ridt.top     = 0;
    cs_ui_pos.cand_ridt.bottom  = height;

    cs_ui_pos.cand_item.left    = cs_ui_pos.cand_lidt.right;
    cs_ui_pos.cand_item.right   = cs_ui_pos.cand_ridt.left;
    cs_ui_pos.cand_item.top     = 0;
    cs_ui_pos.cand_item.bottom  = height;
}

static void ilME_CloseUiWindow(void)
/*
 * Close IME window
 */
{
    ilME_HideWnd();
}

static void ilME_InitUi(void)
/*
 * Setup user interface to initial state
 */
{
    cs_ui_timer_actived = 0;
    ilME_InitUiThemes();
    ilME_InitUiMetrics();
    ilME_InitUiWindow();
}

static void ilME_CloseUi(void)
/*
 * Cleanup when user interface is closed
 */
{
    ilME_StopTimer();
    ilME_CloseUiWindow();
    mle_end_composition(&sms_editor);
}

static int ilME_GetCandTextWidth(const unsigned short* text, int length)
{
    int width = gui_get_text_width(cs_ui_themes.text_font, text, length);

    if (width < cs_ui_metrics.min_cand_width)
        width  = cs_ui_metrics.min_cand_width;
    
    return width;
}

static void ilME_InitKernel(void)
/*
 * Initialize IME kernel
 */
{
/* for test some api */
/*
    ilME_UINT ret;
    unsigned short szAsso[5 * 32];
    unsigned short user_word[4];
    int number = 32;
*/

    ilME_IMM_PARAMS   ip;
    ilME_USER_PARAMS  up;


    memset(&ip, 0, sizeof(ip));
    ip.nMaxPYChoice  = 20;
    ip.nLableWidth   = (unsigned short) (cs_ui_metrics.label_width + cs_ui_metrics.cand_extra_space);
    ip.nWinWidth     = (unsigned short) (cs_ui_pos.cand_item.right - cs_ui_pos.cand_item.left);
    ip.pGetTextWidthFunc = ilME_GetCandTextWidth;
    ip.pfnReadUpdateData = Read_Update_Data;

    memset(&up, 0, sizeof(up));
    up.pUserCache = (char*)cs_imm_user_data;
    up.iUserCacheSize = cs_imm_user_data_size;

    cs_imm = ilME_IMM_Initialize(0, &ip, &up);
    ilME_IMM_SetOptionsEx(cs_imm, &cs_imm_options);
    ilME_IMM_SetInputMethod(cs_imm, cs_imm_mode);

    ilME_InitUi();

    ilME_IMM_AddMultiIM(cs_imm, IM_USER_MULTITYPE_BASE + 1, &multi_string_table);
/* For test some api */
/*
    ilME_IMM_SetInputMethod(cs_imm, IM_STROKE);
    ilME_IMM_QueryProperties(cs_imm, &number);

    user_word[0] = 0x4e00;
    user_word[1] = 0x4e00;
    user_word[2] = 0x4e00;
    user_word[3] = 0x4e00;

    ilME_IMM_SetInputMethod(cs_imm, IM_PINYIN);
    ilME_IMM_AddPhase(cs_imm, user_word, 4);
    ret = ilME_IMM_CheckAsso(cs_imm, 0x6D3B, szAsso, &number);


    ilME_IMM_SetInputMethod(cs_imm, IM_ZHUYIN_MT);
    ilME_IMM_CheckAsso(cs_imm, 0x4E00, szAsso, &number);
    ilME_IMM_AddPhase(cs_imm, user_word, 4);

    ilME_IMM_SetInputMethod(cs_imm, cs_imm_mode);
*/
}

static void ilME_CloseKernel(void)
/*
 * Close IME kernel
 */
{
    if (cs_imm)
    {
        ilME_IMM_Terminate(cs_imm);
        cs_imm = 0;
    }
}

static void ilME_ShowWnd(void)
/*
 * Switch IME window to visible state and show it's content
 */
{
    int h;
    int t;
    ilME_UI_RECT r;

    cs_ui_visible = 1;

    /* Calculate IME window height at current state */

    h = 0;

    if (cs_ui_content.comp_item_count > 0)
        h += cs_ui_pos.comp_height;

    if (cs_ui_content.aux_item_count > 0)
    {
        if (cs_imm_options.dwPinyinMode == ilME_PINYIN_MODE_5 && cs_imm_mode == IM_PINYIN)
        {
        }
        else
        {
            h += cs_ui_pos.aux_height;
        }
    }
    if ((cs_ui_content.cand_item_count2 > 0) || cs_imm_content.iHotUserWord == 0xFFFF)
        h += cs_ui_pos.cand_height;

    h += cs_ui_pos.cand_height;

    /* Set application window and IME window to the new position */

    t = cs_ui_pos.imewnd.bottom - h;
    if (cs_ui_pos.imewnd.top != t)
    {
        cs_ui_pos.imewnd.top = t;

        /* Adjust application window position */

        r = cs_ui_pos.appwnd;
        r.bottom = t;
        mle_set_pos(&sms_editor, &r);
    }

    /* Paint IME window content */

    ilME_PaintWnd();
}

static void ilME_HideWnd(void)
/*
 * Switch IME window to invisible state
 */
{
    if (cs_ui_visible)
    {
        cs_ui_visible = 0;

        /* Adjust IME window position */

        cs_ui_pos.imewnd.top = cs_ui_pos.imewnd.bottom;

        /* Restore application window position */

        mle_set_pos(&sms_editor, &cs_ui_pos.appwnd);
    }
}

static void ilME_PaintWnd(void)
/*
 * Paint IME window content
 */
{
    int i;
    int x, y;
    ilME_UI_RECT r;
    ilME_UI_ITEM* it;

    if (!cs_ui_visible)
        return;

    x = cs_ui_pos.imewnd.left;
    y = cs_ui_pos.imewnd.top;

    /* Paint background */

    gui_draw_rect(&cs_ui_pos.imewnd, cs_ui_themes.bk_color);

    /* Paint composition content */

    if (cs_ui_content.comp_item_count > 0)
    {
        if (cs_ui_content.comp_lidt_visible)
        {
            r = cs_ui_pos.comp_lidt;
            OffsetRect(&r, x, y);
            ilME_PaintLeftIndicator(&r, cs_ui_content.comp_lidt_color);
        }
        if (cs_ui_content.comp_ridt_visible)
        {
            r = cs_ui_pos.comp_ridt;
            OffsetRect(&r, x, y);
            ilME_PaintRightIndicator(&r, cs_ui_content.comp_ridt_color);
        }

        it = cs_ui_content.comp_item;
        for (i = 0; i < cs_ui_content.comp_item_count; i++, it++)
        {
            if (it->item_color != cs_ui_themes.bk_color)
            {
                r = it->item_pos;
                OffsetRect(&r, x, y);
                gui_draw_rect(&r, it->item_color);
            }
            if (it->text_len > 0)
            {
                gui_draw_text(
                    it->text_pos.left + x, it->text_pos.top + y, it->text_font,
                    it->text_str, it->text_len, it->text_color);
            }
        }

        if (cs_ui_content.comp_caret_visible)
        {
            r = cs_ui_content.comp_caret_pos;
            OffsetRect(&r, x, y);
            gui_draw_rect(&r, cs_ui_content.comp_caret_color);
        }

        if (cs_imm_options.dwPinyinMode == ilME_PINYIN_MODE_5 && cs_imm_mode == IM_PINYIN)
        {
        }
        else
        {
            y += cs_ui_pos.comp_height;
        }
    }

    /* Paint aux content */

    if (cs_ui_content.aux_item_count > 0)
    {
        if (cs_ui_content.aux_lidt_visible)
        {
            r = cs_ui_pos.aux_lidt;
            OffsetRect(&r, x, y);
            ilME_PaintLeftIndicator(&r, cs_ui_content.aux_lidt_color);
        }
        if (cs_ui_content.aux_ridt_visible)
        {
            r = cs_ui_pos.aux_ridt;
            OffsetRect(&r, x, y);
            ilME_PaintRightIndicator(&r, cs_ui_content.aux_ridt_color);
        }

        it = cs_ui_content.aux_item;
        for (i = 0; i < cs_ui_content.aux_item_count; i++, it++)
        {
            if (it->item_color != cs_ui_themes.bk_color)
            {
                r = it->item_pos;
                OffsetRect(&r, x, y);
                gui_draw_rect(&r, it->item_color);
            }
            if (it->text_len > 0)
            {
                gui_draw_text(
                    it->text_pos.left + x, it->text_pos.top + y, it->text_font,
                    it->text_str, it->text_len, it->text_color);
            }
        }

        y += cs_ui_pos.aux_height;
    }

    /* Paint Candidate content2 */
    if (cs_imm_content.iHotUserWord == 1 && cs_ui_content.cand_item_count2 > 0)
    {
        it = cs_ui_content.cand_item2;
        for (i = 0; i < cs_ui_content.cand_item_count2; i++, it++)
        {
            if (it->item_color != cs_ui_themes.bk_color)
            {
                r = it->item_pos;
                OffsetRect(&r, x, y);
                gui_draw_rect(&r, it->item_color);
            }
            if (it->text_len > 0)
            {
                gui_draw_text(
                    it->text_pos.left + x, it->text_pos.top + y, it->text_font,
                    it->text_str, it->text_len, it->text_color);
            }
        }

        y += cs_ui_pos.cand_height;
    }

    /* Paint candidate content */

    if (cs_ui_content.cand_item_count > 0)
    {
        if (cs_ui_content.cand_lidt_visible)
        {
            r = cs_ui_pos.cand_lidt;
            OffsetRect(&r, x, y);
            ilME_PaintLeftIndicator(&r, cs_ui_content.cand_lidt_color);
        }
        if (cs_ui_content.cand_ridt_visible)
        {
            r = cs_ui_pos.cand_ridt;
            OffsetRect(&r, x, y);
            ilME_PaintRightIndicator(&r, cs_ui_content.cand_ridt_color);
        }

        it = cs_ui_content.cand_item;
        for (i = 0; i < cs_ui_content.cand_item_count; i++, it++)
        {
            if (it->item_color != cs_ui_themes.bk_color)
            {
                r = it->item_pos;
                OffsetRect(&r, x, y);
                gui_draw_rect(&r, it->item_color);
            }
            if (it->text_len > 0)
            {
                gui_draw_text(
                    it->text_pos.left + x, it->text_pos.top + y, it->text_font,
                    it->text_str, it->text_len, it->text_color);
            }
        }
    }

    if (cs_ui_content.cand_caret_visible)
    {
        r = cs_ui_content.cand_caret_pos;
        OffsetRect(&r, x, y);
        gui_draw_rect(&r, cs_ui_content.cand_caret_color);
    }
    
    y += cs_ui_pos.cand_height;

    /* Paint Candidate content2 */
    if (cs_imm_content.iHotUserWord == 2 && cs_ui_content.cand_item_count2 > 0)
    {
        it = cs_ui_content.cand_item2;
        for (i = 0; i < cs_ui_content.cand_item_count2; i++, it++)
        {
            if (it->item_color != cs_ui_themes.bk_color)
            {
                r = it->item_pos;
                OffsetRect(&r, x, y);
                gui_draw_rect(&r, it->item_color);
            }
            if (it->text_len > 0)
            {
                gui_draw_text(
                    it->text_pos.left + x, it->text_pos.top + y, it->text_font,
                    it->text_str, it->text_len, it->text_color);
            }
        }
    }
}

static void ilME_PaintLeftIndicator(const ilME_UI_RECT* r, ilME_UI_COLOR c)
/*
 * Paint a left arrow indicator
 */
{
    int x, y;

    x = r->left + (r->right - r->left - ilME_IDT_WIDTH) / 2;
    y = r->top + (r->bottom - r->top - ilME_IDT_HEIGHT) / 2;

    gui_draw_pixel(x+5, y+3, c);

    gui_draw_pixel(x+4, y+4, c);
    gui_draw_pixel(x+5, y+4, c);

    gui_draw_pixel(x+3, y+5, c);
    gui_draw_pixel(x+4, y+5, c);
    gui_draw_pixel(x+5, y+5, c);

    gui_draw_pixel(x+2, y+6, c);
    gui_draw_pixel(x+3, y+6, c);
    gui_draw_pixel(x+4, y+6, c);
    gui_draw_pixel(x+5, y+6, c);

    gui_draw_pixel(x+3, y+7, c);
    gui_draw_pixel(x+4, y+7, c);
    gui_draw_pixel(x+5, y+7, c);

    gui_draw_pixel(x+4, y+8, c);
    gui_draw_pixel(x+5, y+8, c);

    gui_draw_pixel(x+5, y+9, c);
}

static void ilME_PaintRightIndicator(const ilME_UI_RECT* r, ilME_UI_COLOR c)
/*
 * Paint a right arrow indicator
 */
{
    int x, y;

    x = r->left + (r->right - r->left - ilME_IDT_WIDTH) / 2;
    y = r->top + (r->bottom - r->top - ilME_IDT_HEIGHT) / 2;

    gui_draw_pixel(x+2, y+3, c);

    gui_draw_pixel(x+2, y+4, c);
    gui_draw_pixel(x+3, y+4, c);

    gui_draw_pixel(x+2, y+5, c);
    gui_draw_pixel(x+3, y+5, c);
    gui_draw_pixel(x+4, y+5, c);

    gui_draw_pixel(x+2, y+6, c);
    gui_draw_pixel(x+3, y+6, c);
    gui_draw_pixel(x+4, y+6, c);
    gui_draw_pixel(x+5, y+6, c);

    gui_draw_pixel(x+2, y+7, c);
    gui_draw_pixel(x+3, y+7, c);
    gui_draw_pixel(x+4, y+7, c);

    gui_draw_pixel(x+2, y+8, c);
    gui_draw_pixel(x+3, y+8, c);

    gui_draw_pixel(x+2, y+9, c);
}

static void ilME_CommitText(const ilME_UI_WCHAR* text, int len)
/*
 * Generated input characters into application
 */
{
    mle_insert_text(&sms_editor, text, len);
    /*cs_last_commit = text[len - 1];*/
}

static void ilME_CommitOutput(void)
/*
 * Submit IME generated text content into application
 */
{
    int len;
    const ilME_UI_WCHAR* text;
    const ilME_UI_WCHAR* p;

    mle_set_composition(&sms_editor, 0, 0);

    len = 0;
    text = (const ilME_UI_WCHAR*) (cs_imm_content.pCommit);
    if (text[0] == '\b'){
        text++;
        mle_delete_char(&sms_editor);
    }

    p = text;
    while ( (*p++) && (len < ilME_MAX_COMMIT) )
        len++;

    if (len > 0)
    {
//        text = (const ilME_UI_WCHAR*) (cs_imm_content.pCommit);
        ilME_CommitText(text, len);
    }
}

static void ilME_ParseCompContent(void)
/*
 * Parse composition content from IME kernel generated content
 */
{
    ilME_UI_CONTENT*          uc;
    ilME_IMM_CONTENT*         ic;

    ilME_UI_WCHAR*            scan;
    ilME_UI_WCHAR*            scan_end;

    ilME_UI_ITEM*             it;

    ilME_UI_RECT              r;
    int                     x;
    int                     cx_wnd;
    int                     cx_text;
    int                     cx_caret;

    int                     first;
    int                     last;

    int                     text_len;

    int                     pre_index;
    int                     pre_hot;
    int                     pre_count;

    int                     hot_start;
    int                     hot_len;
    int                     caret;

    /* Initialize content to empty */

    uc = &cs_ui_content;
    ic = &cs_imm_content;
    r  = cs_ui_pos.comp_item;

    uc->comp_item_count   = 0;
    uc->comp_lidt_visible = 0;
    uc->comp_ridt_visible = 0;

    /* Do not show caret in stroke input method */

    if (cs_imm_mode == IM_STROKE)
    {
        uc->comp_caret_visible   = 0;
    }
    else
    {
        uc->comp_caret_visible   = 1;
        uc->comp_caret_pos       = r;
        if (cs_is_rtl)
            uc->comp_caret_pos.left = r.right - ilME_SIZE_CARET;
        else
            uc->comp_caret_pos.right = r.left + ilME_SIZE_CARET;
        uc->comp_caret_color     = cs_ui_themes.text_color;
    }

    if (ic->iHotPreEdit != 0 && ic->pPreBuffer[0] == 0)
    {
        int i;
        for (i = 0; i < ilME_MAX_COMP - 1; i++)
            ic->pPreBuffer[i] = ic->pPreBuffer[i + 1];
        ic->nPreEdit--;
        ic->iHotPreEdit--;
    }

    /*
     * Scan composition content twice to:
     *      - Generate composition text
     *      - Locate hot text start and end position
     */

    scan = (ilME_UI_WCHAR*)(ic->pPreBuffer);
    scan_end = scan + ilME_MAX_COMP;

    text_len = 0;
    pre_count = 0;

    while (scan < scan_end)
    {
        if (pre_count >= ic->nPreEdit)
            break;

        while ( (scan < scan_end) && (*scan != 0) )
        {
            text_len++;
            scan++;
        }

        pre_count++;

        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    if ( (text_len == 0) || (pre_count == 0) )
        return;     /* composition content is empty */

    pre_hot = ic->iHotPreEdit;
    if (pre_hot >= pre_count)
        pre_hot = pre_count - 1;
    if (pre_hot < 0)
        pre_hot = 0;

    hot_start = 0;
    hot_len = 0;

    text_len = 0;
    pre_index = 0;

    scan = (ilME_UI_WCHAR*)(ic->pPreBuffer);

    while (scan < scan_end)
    {
        if (pre_index >= ic->nPreEdit)
            break;

        if (pre_index > 0)
        {
            /* insert space between items */
            if ( (text_len == 0) ||
                 ((text_len > 0) && (uc->comp_text[text_len-1] != 0x20)) )
            {
                uc->comp_text[text_len++] = 0x20;
            }
        }

        if (pre_index == pre_hot)
            hot_start = text_len;

        while ( (scan < scan_end) && (*scan != 0) )
            uc->comp_text[text_len++] = *scan++;

        if (pre_index == pre_hot)
            hot_len = text_len - hot_start;

        pre_index++;

        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    /* Place caret at the end of hot text */

    caret = hot_start + hot_len;

    /* Split composition text into multiple items to ensure hot text visible */

    cx_wnd = r.right - r.left;

    first = 0;
    for (;;)
    {
        cx_caret = gui_get_text_width(
            cs_ui_themes.text_font,
            uc->comp_text + first,
            caret - first);
        if (cx_caret <= cx_wnd)
            break;
        first++;
    }

    if (uc->comp_caret_visible)
    {
        if (cs_is_rtl)
        {
            uc->comp_caret_pos.left -= cx_caret;
            uc->comp_caret_pos.right -= cx_caret;
        }
        else
        {
            uc->comp_caret_pos.left += cx_caret;
            uc->comp_caret_pos.right += cx_caret;
        }
    }

    last = text_len;
    for (;;)
    {
        cx_text = gui_get_text_width(
            cs_ui_themes.text_font,
            uc->comp_text + first,
            last - first);
        if (cx_text <= cx_wnd)
            break;
        last--;
    }

    it = uc->comp_item;
    if (cs_is_rtl)
        x = r.right;
    else
        x = r.left;

    if (hot_len > 0)
    {
        if (hot_start > first)
        {
            uc->comp_item_count++;
            it->text_str       = uc->comp_text + first;
            it->text_len       = hot_start - first;
            it->text_font      = cs_ui_themes.text_font;
            cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
            it->text_pos       = r;
            if (cs_is_rtl)
            {
                it->text_pos.right = x;
                it->text_pos.left  = x - cx_text;
            }
            else
            {
                it->text_pos.left  = x;
                it->text_pos.right = x + cx_text;
            }
            it->item_pos       = it->text_pos;
            it->text_color     = cs_ui_themes.text_color;
            it->item_color     = cs_ui_themes.bk_color;
            it++;
            if (cs_is_rtl)
                x -= cx_text;
            else
                x += cx_text;
        }

        uc->comp_item_count++;
        if (hot_start >= first)
        {
            it->text_str = uc->comp_text + hot_start;
            it->text_len = hot_len;
        }
        else
        {
            it->text_str = uc->comp_text + first;
            it->text_len = caret - first;
        }
        it->text_font      = cs_ui_themes.text_font;
        cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
        it->text_pos       = r;
        if (cs_is_rtl)
        {
            it->text_pos.right = x;
            it->text_pos.left  = x - cx_text;
        }
        else
        {
            it->text_pos.left  = x;
            it->text_pos.right = x + cx_text;
        }
        it->item_pos       = it->text_pos;
        if (cs_ui_state == ilME_UI_STATE_COMP)
            it->text_color = cs_ui_themes.hot_color1;
        else
            it->text_color = cs_ui_themes.text_color;
        it->item_color = cs_ui_themes.bk_color;
        it++;
        if (cs_is_rtl)
            x -= cx_text;
        else
            x += cx_text;

        if (caret < last)
        {
            uc->comp_item_count++;
            it->text_str       = uc->comp_text + caret;
            it->text_len       = last - caret;
            it->text_font      = cs_ui_themes.text_font;
            cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
            it->text_pos       = r;
            if (cs_is_rtl)
            {
                it->text_pos.right = x;
                it->text_pos.left  = x - cx_text;
            }
            else
            {
                it->text_pos.left  = x;
                it->text_pos.right = x + cx_text;
            }
            it->item_pos       = it->text_pos;
            it->text_color     = cs_ui_themes.text_color;
            it->item_color     = cs_ui_themes.bk_color;
            it++;
        }
    }
    else
    {
        uc->comp_item_count++;
        it->text_str       = uc->comp_text + first;
        it->text_len       = last - first;
        it->text_font      = cs_ui_themes.text_font;
        it->text_pos       = r;
        if (cs_is_rtl)
            it->text_pos.left = r.right - cx_text;
        else
            it->text_pos.right = r.left + cx_text;
        it->item_pos       = it->text_pos;
        it->text_color     = cs_ui_themes.text_color;
        it->item_color     = cs_ui_themes.bk_color;
    }

    if (first > 0)
    {
        if (cs_is_rtl)
        {
            uc->comp_ridt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_COMP)
                uc->comp_ridt_color = cs_ui_themes.hot_color1;
            else
                uc->comp_ridt_color = cs_ui_themes.text_color;
        }
        else
        {
            uc->comp_lidt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_COMP)
                uc->comp_lidt_color = cs_ui_themes.hot_color1;
            else
                uc->comp_lidt_color = cs_ui_themes.text_color;
        }
    }

    if (last < text_len)
    {
        if (cs_is_rtl)
        {
            uc->comp_lidt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_COMP)
                uc->comp_lidt_color = cs_ui_themes.hot_color1;
            else
                uc->comp_lidt_color = cs_ui_themes.text_color;
        }
        else
        {
            uc->comp_ridt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_COMP)
                uc->comp_ridt_color = cs_ui_themes.hot_color1;
            else
                uc->comp_ridt_color = cs_ui_themes.text_color;
        }
    }
}

static void ilME_ParseCandContent(void)
/*
 * Parse candidate content from IME kernel generated content
 */
{
    ilME_UI_CONTENT*          uc;
    ilME_IMM_CONTENT*         ic;

    ilME_UI_WCHAR*            scan;
    ilME_UI_WCHAR*            scan_end;

    ilME_UI_ITEM*             it;

    ilME_UI_RECT              r;
    int                     x;
    int                     cx_max;
    int                     cx_text;
    int                     cx_item;

    int                     first;
    int                     last;

    int                     text_len;

    int                     cad_index;
    int                     cad_hot;
    int                     cad_count;

    /* Initialize content to empty */

    uc = &cs_ui_content;
    ic = &cs_imm_content;

    uc->cand_item_count    = 0;
    uc->cand_lidt_visible  = 0;
    uc->cand_ridt_visible  = 0;
    uc->cand_caret_visible = 0;

    /*
     * Scan candidate content twice to:
     *      - Generate candidate text
     *      - Locate hot text start and end position
     */

    scan = (ilME_UI_WCHAR*)(ic->pCandBuffer);
    scan_end = scan + ilME_MAX_CAND;

    cad_count = 0;
    while (scan < scan_end)
    {
        if (cad_count >= ic->nCand)
            break;

        while ( (scan < scan_end) && (*scan != 0) )
            scan++;

        cad_count++;
        if (cad_count == 9)
            break;

        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    if (cad_count == 0)
        return;     /* candidate content is empty */

    cad_hot = ic->iHotCand;
    if (cad_hot >= cad_count)
        cad_hot = cad_count - 1;
    if (cad_hot < 0)
        cad_hot = 0;

    text_len = 0;
    it = uc->cand_item;

    r = cs_ui_pos.cand_item;
    if (cs_is_rtl)
        x = r.right;
    else
        x = r.left;

    cad_index = 0;
    scan = (ilME_UI_WCHAR*)(ic->pCandBuffer);
    while (scan < scan_end)
    {
        if (cad_index >= ic->nCand)
            break;

        cx_text = cs_ui_metrics.label_width;
        if (cs_ui_state == ilME_UI_STATE_CAND)
        {
            uc->cand_item_count++;
            it->text_str       = uc->cand_text + text_len;
            it->text_len       = 1;
            it->text_font      = cs_ui_themes.label_font;
            uc->cand_text[text_len++] = (ilME_UI_WCHAR)(cad_index + 0x31);
            it->text_pos       = r;
            if (cs_is_rtl)
            {
                it->text_pos.right  = x;
                it->text_pos.left = x - cx_text;
            }
            else
            {
                it->text_pos.left  = x;
                it->text_pos.right = x + cx_text;
            }
            it->text_pos.top   = r.top - 1;
            it->item_pos       = it->text_pos;
            it->text_color     = cs_ui_themes.hot_color1;
            it->item_color     = cs_ui_themes.bk_color;
            it++;
        }
        if (cs_is_rtl)
            x -= cx_text;
        else
            x += cx_text;

        first = text_len;
        while ( (scan < scan_end) && (*scan != 0) )
            uc->cand_text[text_len++] = *scan++;
        last = text_len;

        if (cs_is_rtl)
            cx_max = x - r.left;
        else
            cx_max = r.right - x;
        if (cx_max < 0)
            cx_max = 0;

        for (;;)
        {
            cx_text = gui_get_text_width(
                cs_ui_themes.text_font,
                uc->cand_text + first,
                last - first);
            if (cx_text <= cx_max)
                break;
            first++;
        }

        cx_item = cx_text;
        if (cx_item < cs_ui_metrics.min_cand_width)
            cx_item = cs_ui_metrics.min_cand_width;

        uc->cand_item_count++;
        it->text_str       = uc->cand_text + first;
        it->text_len       = last - first;
        it->text_font      = cs_ui_themes.text_font;
        it->text_pos       = r;
        if (cs_is_rtl)
        {
            it->text_pos.right  = x - (cx_item - cx_text) / 2;
            it->text_pos.left = it->text_pos.right - cx_text;
        }
        else
        {
            it->text_pos.left  = x + (cx_item - cx_text) / 2;
            it->text_pos.right = it->text_pos.left + cx_text;
        }
        it->item_pos       = r;
        if (cs_is_rtl)
        {
            it->item_pos.right  = x;
            it->item_pos.left = x - cx_item;
        }
        else
        {
            it->item_pos.left  = x;
            it->item_pos.right = x + cx_item;
        }

        if (cad_index != cad_hot)
        {
            it->text_color = cs_ui_themes.text_color;
            it->item_color = cs_ui_themes.bk_color;
        }
        else if (cs_ui_state == ilME_UI_STATE_CAND)
        {
            it->text_color = cs_ui_themes.hot_color2;
            it->item_color = cs_ui_themes.hot_color1;
        }
        else
        {
            it->text_color = cs_ui_themes.text_color;
            it->item_color = cs_ui_themes.hot_color3;
        }
        it++;

        if (cs_is_rtl)
        {
            x -= cx_item + cs_ui_metrics.cand_extra_space;
            if (x <= r.left)
                break;
        }
        else
        {
            x += cx_item + cs_ui_metrics.cand_extra_space;
            if (x >= r.right)
                break;
        }

        cad_index++;
        if (cad_index == 9)
            break;

        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    if (ic->flagPageCand & ilME_FLAG_PAGEDOWN)
    {
        if (cs_is_rtl)
        {
            uc->cand_ridt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_CAND)
                uc->cand_ridt_color = cs_ui_themes.hot_color1;
            else
                uc->cand_ridt_color = cs_ui_themes.text_color;
        }
        else
        {
            uc->cand_lidt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_CAND)
                uc->cand_lidt_color = cs_ui_themes.hot_color1;
            else
                uc->cand_lidt_color = cs_ui_themes.text_color;
        }
    }

    if (ic->flagPageCand & ilME_FLAG_PAGEUP)
    {
        if (cs_is_rtl)
        {
            uc->cand_lidt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_CAND)
                uc->cand_lidt_color = cs_ui_themes.hot_color1;
            else
                uc->cand_lidt_color = cs_ui_themes.text_color;
        }
        else
        {
            uc->cand_ridt_visible = 1;
            if (cs_ui_state == ilME_UI_STATE_CAND)
                uc->cand_ridt_color = cs_ui_themes.hot_color1;
            else
                uc->cand_ridt_color = cs_ui_themes.text_color;
        }
    }
}

static void ilME_ParseCandContent2(void)
/*
 * Parse candidate content from IME kernel generated content
 */
{
    ilME_UI_CONTENT*          uc;
    ilME_IMM_CONTENT*         ic;

    ilME_UI_WCHAR*            scan;
    ilME_UI_WCHAR*            scan_end;

    ilME_UI_ITEM*             it;

    ilME_UI_RECT              r;
    int                     x;
    int                     cx_max;
    int                     cx_text;
    int                     cx_item;

    int                     first;
    int                     last;

    int                     text_len;

    int                     cad_index;
    int                     cad_hot;
    int                     cad_count;

    /* Initialize content to empty */

    uc = &cs_ui_content;
    ic = &cs_imm_content;

    uc->cand_item_count2    = 0;

    /*
     * Scan candidate content twice to:
     *      - Generate candidate text
     *      - Locate hot text start and end position
     */

    scan = (ilME_UI_WCHAR*)(ic->pUserWordBuffer);
    scan_end = scan + ilME_MAX_CAND;

    cad_count = 0;
    while (scan < scan_end)
    {
        if (cad_count >= ic->nUserWord)
            break;

        while ( (scan < scan_end) && (*scan != 0) )
            scan++;

        cad_count++;
        if (cad_count == 9)
            break;

        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    if (cad_count == 0)
        return;     /* candidate content is empty */

    cad_hot = ic->iHotCand;
    if (cad_hot >= cad_count)
        cad_hot = cad_count - 1;
    if (cad_hot < 0)
        cad_hot = 0;

    text_len = 0;
    it = uc->cand_item2;

    r = cs_ui_pos.cand_item;
    if (cs_is_rtl)
        x = r.right;
    else
        x = r.left;

    cad_index = 0;
    scan = (ilME_UI_WCHAR*)(ic->pUserWordBuffer);
    while (scan < scan_end)
    {
        if (cad_index >= ic->nUserWord)
            break;

        cx_text = cs_ui_metrics.label_width;
/*
        if (cs_ui_state == ilME_UI_STATE_CAND)
        {
            uc->cand_item_count2++;
            it->text_str       = uc->cand_text2 + text_len;
            it->text_len       = 1;
            it->text_font      = cs_ui_themes.label_font;
            uc->cand_text2[text_len++] = (ilME_UI_WCHAR)(cad_index + 0x31);
            it->text_pos       = r;
            if (cs_is_rtl)
            {
                it->text_pos.right  = x;
                it->text_pos.left = x - cx_text;
            }
            else
            {
                it->text_pos.left  = x;
                it->text_pos.right = x + cx_text;
            }
            it->text_pos.top   = r.top - 1;
            it->item_pos       = it->text_pos;
            it->text_color     = cs_ui_themes.hot_color1;
            it->item_color     = cs_ui_themes.bk_color;
            it++;
        }
*/
        if (cs_is_rtl)
            x -= cx_text;
        else
            x += cx_text;

        first = text_len;
        while ( (scan < scan_end) && (*scan != 0) )
            uc->cand_text2[text_len++] = *scan++;
        last = text_len;

        if (cs_is_rtl)
            cx_max = x - r.left;
        else
            cx_max = r.right - x;
        if (cx_max < 0)
            cx_max = 0;

        for (;;)
        {
            cx_text = gui_get_text_width(
                cs_ui_themes.text_font,
                uc->cand_text2 + first,
                last - first);
            if (cx_text <= cx_max)
                break;
            first++;
        }

        cx_item = cx_text;
        if (cx_item < cs_ui_metrics.min_cand_width)
            cx_item = cs_ui_metrics.min_cand_width;

        uc->cand_item_count2++;
        it->text_str       = uc->cand_text2 + first;
        it->text_len       = last - first;
        it->text_font      = cs_ui_themes.text_font;
        it->text_pos       = r;
        if (cs_is_rtl)
        {
            it->text_pos.right  = x - (cx_item - cx_text) / 2;
            it->text_pos.left = it->text_pos.right - cx_text;
        }
        else
        {
            it->text_pos.left  = x + (cx_item - cx_text) / 2;
            it->text_pos.right = it->text_pos.left + cx_text;
        }
        it->item_pos       = r;
        if (cs_is_rtl)
        {
            it->item_pos.right  = x;
            it->item_pos.left = x - cx_item;
        }
        else
        {
            it->item_pos.left  = x;
            it->item_pos.right = x + cx_item;
        }

        it->text_color = cs_ui_themes.text_color;
        it->item_color = cs_ui_themes.bk_color;
/*
        else if (cs_ui_state == ilME_UI_STATE_CAND)
        {
            it->text_color = cs_ui_themes.hot_color2;
            it->item_color = cs_ui_themes.hot_color1;
        }
        else
        {
            it->text_color = cs_ui_themes.text_color;
            it->item_color = cs_ui_themes.hot_color3;
        }
*/
        it++;

        if (cs_is_rtl)
        {
            x -= cx_item + cs_ui_metrics.cand_extra_space;
            if (x <= r.left)
                break;
        }
        else
        {
            x += cx_item + cs_ui_metrics.cand_extra_space;
            if (x >= r.right)
                break;
        }

        cad_index++;
        if (cad_index == 9)
            break;

        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

}

static void ilME_ParseEudwContent(void)
/*
 * Parse EUDW content from IME kernel generated content
 */
{
    ilME_UI_CONTENT*          uc;
    ilME_IMM_CONTENT*         ic;

    ilME_UI_WCHAR*            scan;
    ilME_UI_WCHAR*            scan_end;

    ilME_UI_ITEM*             it;

    ilME_UI_RECT              r;
    int                     cx_wnd;
    int                     cx_text;
    int                     cx_caret;

    int                     first;
    int                     last;

    int                     text_len;
    int                     caret;

    uc = &cs_ui_content;
    ic = &cs_imm_content;

    /* Set prompt text content */

    r = cs_ui_pos.comp_item;

    uc->comp_lidt_visible  = 0;
    uc->comp_ridt_visible  = 0;
    uc->comp_caret_visible = 0;

    uc->comp_item_count = 1;
    it = uc->comp_item;

    scan = (ilME_UI_WCHAR*)(ic->pPreBuffer);
    scan_end = scan + ilME_MAX_COMP;

    text_len = 0;
    while ( (scan < scan_end) && (*scan != 0) )
        uc->comp_text[text_len++] = *scan++;

    it->text_str       = uc->comp_text;
    it->text_len       = text_len;
    it->text_font      = cs_ui_themes.text_font;
    cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
    it->text_pos       = r;
    if (cs_is_rtl)
        it->text_pos.left = r.right - cx_text;
    else
        it->text_pos.right = r.left + cx_text;
    it->item_pos       = it->text_pos;
    it->text_color     = cs_ui_themes.text_color;
    it->item_color     = cs_ui_themes.bk_color;

    /* Set EUDW text */

    r = cs_ui_pos.cand_item;

    uc->cand_item_count   = 0;
    uc->cand_lidt_visible = 0;
    uc->cand_ridt_visible = 0;

    uc->cand_caret_visible   = 1;
    uc->cand_caret_pos       = r;
    if (cs_is_rtl)
        uc->cand_caret_pos.left = r.right - ilME_SIZE_CARET;
    else
        uc->cand_caret_pos.right = r.left + ilME_SIZE_CARET;
    uc->cand_caret_color     = cs_ui_themes.text_color;

    scan = (ilME_UI_WCHAR*)(ic->pUserWordBuffer);
    scan_end = scan + ilME_MAX_EUDW;

    text_len = 0;
    while ( (scan < scan_end) && (*scan != 0) && (text_len < ic->nUserWord) )
        uc->cand_text[text_len++] = *scan++;

    if (text_len == 0)
        return;     /* EUDW text is empty */

    caret = ic->iHotUserWord;
    if (caret > text_len)
        caret = text_len;
    if (caret < 0)
        caret = 0;

    cx_wnd = r.right - r.left;

    first = 0;
    for (;;)
    {
        cx_caret = gui_get_text_width(
            cs_ui_themes.text_font,
            uc->cand_text + first,
            caret - first);
        if (cx_caret <= cx_wnd)
            break;
        first++;
    }

    if (cs_is_rtl)
    {
        uc->cand_caret_pos.left  -= cx_caret;
        uc->cand_caret_pos.right -= cx_caret;
    }
    else
    {
        uc->cand_caret_pos.left  += cx_caret;
        uc->cand_caret_pos.right += cx_caret;
    }

    last = text_len;
    for (;;)
    {
        cx_text = gui_get_text_width(
            cs_ui_themes.text_font,
            uc->cand_text + first,
            last - first);
        if (cx_text <= cx_wnd)
            break;
        last--;
    }

    uc->cand_item_count = 1;
    it = uc->cand_item;

    it->text_str       = uc->cand_text + first;
    it->text_len       = last - first;
    it->text_font      = cs_ui_themes.text_font;
    it->text_pos       = r;
    if (cs_is_rtl)
        it->text_pos.left = r.right - cx_text;
    else
        it->text_pos.right = r.left + cx_text;
    it->item_pos       = it->text_pos;
    it->text_color     = cs_ui_themes.text_color;
    it->item_color     = cs_ui_themes.bk_color;

    if (first > 0)
    {
        if (cs_is_rtl)
        {
            uc->cand_ridt_visible = 1;
            uc->cand_ridt_color   = cs_ui_themes.hot_color1;
        }
        else
        {
            uc->cand_lidt_visible = 1;
            uc->cand_lidt_color   = cs_ui_themes.hot_color1;
        }
    }

    if (last < text_len)
    {
        if (cs_is_rtl)
        {
            uc->cand_lidt_visible = 1;
            uc->cand_lidt_color   = cs_ui_themes.hot_color1;
        }
        else
        {
            uc->cand_ridt_visible = 1;
            uc->cand_ridt_color   = cs_ui_themes.hot_color1;
        }
    }
}

static void ilME_ParseAuxContent(void)
/*
 * Parse aux content from IME kernel generated content
 */
{
    ilME_UI_CONTENT*          uc;
    ilME_IMM_CONTENT*         ic;

    ilME_UI_WCHAR*            scan;
    ilME_UI_WCHAR*            scan_end;

    ilME_UI_ITEM*             it;

    ilME_UI_RECT              r;
    int                     x;
    int                     cx_wnd;
    int                     cx_text;

    int                     first;
    int                     last;

    int                     text_len;

    int                     index;
    int                     hot;
    int                     count;

    int                     hot_visible;
    int                     hot_start;
    int                     hot_len;
    int                     hot_end;

    /* Initialize content to empty */

    uc = &(cs_ui_content);
    ic = &(cs_imm_content);
    r  = cs_ui_pos.aux_item;

    uc->aux_item_count   = 0;
    uc->aux_lidt_visible = 0;
    uc->aux_ridt_visible = 0;

    /*
     * Scan content twice to:
     *    - Generate text
     *    - Locate hot text start and end position
     */

    scan = (ilME_UI_WCHAR*)(ic->pChoiceBuffer);
    scan_end = scan + ilME_MAX_AUX;

    text_len = 0;
    count = 0;

    while (scan < scan_end)
    {
        if (count >= ic->nChoice)
            break;
        while ( (scan < scan_end) && (*scan != 0) )
        {
            text_len++;
            scan++;
        }
        count++;
        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    if ( (text_len == 0) || (count == 0) )
        return;     /* content is empty */

    hot = ic->iHotChoice;
    if ( (hot >= 0) && (hot < count) )
    {
        hot_visible = 1;
    }
    else
    {
        hot_visible = 0;
        hot = 0;
    }

    hot_start = 0;
    hot_end = 0;
    hot_len = 0;

    text_len = 0;
    index = 0;

    scan = (ilME_UI_WCHAR*)(ic->pChoiceBuffer);
    while (scan < scan_end)
    {
        if (index >= ic->nChoice)
            break;
        if (index > 0)
        {
            /* insert space between items */
            if ( (text_len == 0) ||
                 ((text_len > 0) && (uc->aux_text[text_len-1] != 0x20)) )
            {
                uc->aux_text[text_len++] = 0x20;
            }
        }
        if (index == hot)
            hot_start = text_len;
        while ( (scan < scan_end) && (*scan != 0) )
            uc->aux_text[text_len++] = *scan++;
        if (index == hot)
            hot_end = text_len;
        index++;
        if ( (scan < scan_end) && (*scan == 0) )
            scan++;
    }

    hot_len = hot_end - hot_start;

    /* Split text into multiple items to ensure hot text visible */

    cx_wnd = r.right - r.left;

    first = 0;
    for (;;)
    {
        cx_text = gui_get_text_width(
            cs_ui_themes.text_font,
            uc->aux_text + first,
            hot_end - first);
        if (cx_text <= cx_wnd)
            break;
        while (uc->aux_text[first] != 0x20)
            first++;
        first++;
    }

    last = text_len;
    for (;;)
    {
        cx_text = gui_get_text_width(
            cs_ui_themes.text_font,
            uc->aux_text + first,
            last - first);
        if (cx_text <= cx_wnd)
            break;
        while (uc->aux_text[last-1] != 0x20)
            last--;
        last--;
    }

    it = uc->aux_item;
    x = r.left;

    if (hot_start > first)
    {
        uc->aux_item_count++;
        it->text_str       = uc->aux_text + first;
        it->text_len       = hot_start - first;
        it->text_font      = cs_ui_themes.text_font;
        cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
        it->text_pos       = r;
        it->text_pos.left  = x;
        it->text_pos.right = x + cx_text;
        it->item_pos       = it->text_pos;
        it->text_color     = cs_ui_themes.text_color;
        it->item_color     = cs_ui_themes.bk_color;
        it++;
        x += cx_text;
    }

    uc->aux_item_count++;
    it->text_str       = uc->aux_text + hot_start;
    it->text_len       = hot_len;
    it->text_font      = cs_ui_themes.text_font;
    cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
    it->text_pos       = r;
    it->text_pos.left  = x;
    it->text_pos.right = x + cx_text;
    it->item_pos       = it->text_pos;
    it->text_color = cs_ui_themes.text_color;
    if (hot_visible)
        it->item_color = cs_ui_themes.hot_color3;
    else
        it->item_color = cs_ui_themes.bk_color;
    it++;
    x += cx_text;

    if (hot_end < last)
    {
        uc->aux_item_count++;
        it->text_str       = uc->aux_text + hot_end;
        it->text_len       = last - hot_end;
        it->text_font      = cs_ui_themes.text_font;
        cx_text = gui_get_text_width(it->text_font, it->text_str, it->text_len);
        it->text_pos       = r;
        it->text_pos.left  = x;
        it->text_pos.right = x + cx_text;
        it->item_pos       = it->text_pos;
        it->text_color     = cs_ui_themes.text_color;
        it->item_color     = cs_ui_themes.bk_color;
        it++;
    }

    if (first > 0)
    {
        uc->aux_lidt_visible = 1;
        uc->aux_lidt_color = cs_ui_themes.text_color;
    }

    if (last < text_len)
    {
        uc->aux_ridt_visible = 1;
        uc->aux_ridt_color = cs_ui_themes.text_color;
    }
}

static void ilME_ParseInlineContent(void)
/*
 * Parse inline content from IME kernel generated content
 */
{
    ilME_IMM_CONTENT*         ic;
    ilME_UI_WCHAR*            text;
    int                     len;

    ic = &cs_imm_content;

    if (ic->nUnderline > 0)
    {
        text = (ilME_UI_WCHAR*)(ic->pUnderlineBuffer);
        len = ic->nUnderline;
    }
    else
    {
        text = 0;
        len = 0;
    }

    mle_set_composition(&sms_editor, text, len);
}

static void ilME_UpdateWnd(void)
/*
 * Update user interface from IME kernel generated content
 */
{
    int visible;

    visible = 0;

    cs_ui_content.comp_item_count = 0;
    cs_ui_content.cand_item_count = 0;
    cs_ui_content.aux_item_count  = 0;

    if (cs_imm_content.flagState == ilME_FLAG_STATE_CAND || cs_imm_content.flagState == ilME_FLAG_STATE_ACTIVE)
        cs_ui_state = ilME_UI_STATE_CAND;
    else if (cs_imm_content.flagState == ilME_FLAG_STATE_USER)
        cs_ui_state = ilME_UI_STATE_EUDW;
    else
        cs_ui_state = ilME_UI_STATE_COMP;

    if (cs_ui_state == ilME_UI_STATE_EUDW)
    {
        ilME_ParseEudwContent();
        visible = 1;
    }
    else
    {
        ilME_ParseCompContent();
        if (cs_ui_content.comp_item_count > 0)
            visible = 1;

        ilME_ParseAuxContent();
        if (cs_ui_content.aux_item_count > 0)
            visible = 1;

        ilME_ParseCandContent();
        if (cs_ui_content.cand_item_count > 0)
            visible = 1;

        ilME_ParseCandContent2();
        if (cs_ui_content.cand_item_count2 > 0)
            visible = 1;

    }

    ilME_ParseInlineContent();

    if (visible)
        ilME_ShowWnd();
    else
        ilME_HideWnd();
}

static void ilME_ProcessContent(void)
/*
 * Process IME kernel generated content as event handling result
 */
{
    ilME_StopTimer();

    ilME_CommitOutput();

    ilME_UpdateWnd();

    if (cs_imm_content.flagTimer == ilME_TIMER_SET)
        ilME_StartTimer();

    sms_update_status();
}

static void ilME_OnTimerEvent(void)
/*
 * Timer event handler
 */
{
    memset(&cs_imm_content, 0, sizeof(ilME_IMM_CONTENT));
    cs_imm_content.iCaretPos = mle_get_context(&sms_editor);
	if (cs_imm_content.iCaretPos == 0)
		cs_imm_content.iCaretPos = ' ';

    ilME_IMM_EventHandle(cs_imm, ilME_EVENT_TIMER, 0, &cs_imm_content);
    ilME_ProcessContent();
}

static void ilME_StartTimer(void)
/*
 * Active timer
 */
{
    if (!cs_ui_timer_actived)
    {
        gui_enable_ime_timer(ilME_IME_TIME_OUT, ilME_OnTimerEvent);
        cs_ui_timer_actived = 1;
    }
}

static void ilME_StopTimer(void)
/*
 * Stop/Cancel timer
 */
{
    if (cs_ui_timer_actived)
    {
        gui_disable_ime_timer();
        cs_ui_timer_actived = 0;
    }
}

#if !defined(INTERFACE_08_ONLY)

/*
 * Implementation of global functions
 */

void cstar_ime_set_input_method(int input_method)
/*
 * Initialize input method
 */
{
    if (cs_initialized)
    {
        ilME_HideWnd();

        cs_imm_mode = ilME_GuiMode2CstarMode(input_method);
        if (input_method == IM_ZHUYIN + IM_FULLKEY_BASE)
            ilME_IMM_SetInputMethodEx(cs_imm, input_method, zhuyin_key2char);
        else
            ilME_IMM_SetInputMethod(cs_imm, cs_imm_mode);

        ilME_InitUi();
    }
    else
    {
        ilME_LoadUserData();
        ilME_LoadOptions();

        ilME_InitUi();

        cs_imm_mode = ilME_GuiMode2CstarMode(input_method);
        ilME_InitKernel();

        cs_initialized = 1;
    }

    switch (cs_imm_mode)
    {
        case IM_HEBREW:
        case IM_ARABIC:
        case IM_PERSIAN:
        case IM_URDU:
        case IM_UYGHUR:
        case IM_FULLKEY_BASE + IM_HEBREW:
        case IM_FULLKEY_BASE + IM_ARABIC:
        case IM_FULLKEY_BASE + IM_PERSIAN:
        case IM_FULLKEY_BASE + IM_URDU:
        case IM_FULLKEY_BASE+ IM_UYGHUR:
            cs_is_rtl = 1;
            break;
        default:
            cs_is_rtl = 0;
            break;
    }
}

void cstar_ime_close_input_method(void)
/*
 * Close input method
 */
{
    if (cs_initialized)
    {
        ilME_CloseKernel();
        ilME_CloseUi();

        ilME_SaveUserData();

        cs_initialized = 0;
    }
}

int cstar_ime_is_ui_visible(void)
{
    if (cs_initialized)
        return cs_ui_visible;
    else
        return 0;
}


int cstar_ime_handle_key(int key, int presshold)
/*
 * Key event handler
 */
{
    int flags;
    int status;

    unsigned int vk;

    if (!cs_initialized)
        return 0;

    if (cs_imm_mode == IM_PINYIN_FULLKEY || cs_imm_mode == IM_ZHUYIN_FULLKEY)
        if (key >= GUI_KEY_1 && key <= GUI_KEY_9)
            return cstar_ime_handle_char(ilME_EVENT_SELECTCAND, key);

    if (presshold)
        flags = ilME_VK_LONGCLICK;
    else
        flags = 0;

    memset(&cs_imm_content, 0, sizeof(ilME_IMM_CONTENT));
    cs_imm_content.iCaretPos = mle_get_context(&sms_editor);
    if (cs_imm_content.iCaretPos == 0)
        cs_imm_content.iCaretPos = ' ';

    switch (key)
    {
        case GUI_KEY_0:
        case GUI_KEY_1:
        case GUI_KEY_2:
        case GUI_KEY_3:
        case GUI_KEY_4:
        case GUI_KEY_5:
        case GUI_KEY_6:
        case GUI_KEY_7:
        case GUI_KEY_8:
        case GUI_KEY_9:
            vk = (key - GUI_KEY_0) + ilME_VK_0;
            break;
        case GUI_KEY_L1:
        case GUI_KEY_L2:
        case GUI_KEY_L3:
        case GUI_KEY_R1:
        case GUI_KEY_R2:
        case GUI_KEY_R3:
            vk = (key - GUI_KEY_L1) + ilME_VK_L1;
            break;
        case GUI_KEY_STAR:
            vk = ilME_VK_STAR;
            break;
        case GUI_KEY_POUND:
            vk = ilME_VK_POUND;
            break;
        case GUI_KEY_LEFT:
            vk = ilME_VK_LEFT;
            break;
        case GUI_KEY_RIGHT:
            vk = ilME_VK_RIGHT;
            break;
        case GUI_KEY_UP:
            vk = ilME_VK_UP;
            break;
        case GUI_KEY_DOWN:
            vk = ilME_VK_DOWN;
            break;
        case GUI_KEY_OK:
        case GUI_KEY_LSK:
            vk = ilME_VK_SELECT;
            break;
        case GUI_KEY_RSK:
            vk = ilME_VK_BACKSPACE;
            break;
        case ilME_VK_A:
        case ilME_VK_A + 1:
        case ilME_VK_A + 2:
        case ilME_VK_A + 3:
        case ilME_VK_A + 4:
        case ilME_VK_A + 5:
        case ilME_VK_A + 6:
        case ilME_VK_A + 7:
        case ilME_VK_A + 8:
        case ilME_VK_A + 9:
        case ilME_VK_A + 10:
        case ilME_VK_A + 11:
        case ilME_VK_A + 12:
        case ilME_VK_A + 13:
        case ilME_VK_A + 14:
        case ilME_VK_A + 15:
        case ilME_VK_A + 16:
        case ilME_VK_A + 17:
        case ilME_VK_A + 18:
        case ilME_VK_A + 19:
        case ilME_VK_A + 20:
        case ilME_VK_A + 21:
        case ilME_VK_A + 22:
        case ilME_VK_A + 23:
        case ilME_VK_A + 24:
        case ilME_VK_A + 25:
        case ilME_VK_A + 26:
            vk = key;
            break;
        default:
            return 0;
    }

    status = ilME_IMM_EventHandle(cs_imm, ilME_EVENT_KEYBOARD, vk | flags, &cs_imm_content);
    ilME_ProcessContent();

    if (vk == ilME_VK_POUND && flags == ilME_VK_LONGCLICK)
        return 0;

    return status == ilME_ERROR_USED;
}

int cstar_ime_handle_char(int event, int msg)
{
    int status;

    memset(&cs_imm_content, 0, sizeof(ilME_IMM_CONTENT));
	if (cs_imm_content.iCaretPos == 0)
		cs_imm_content.iCaretPos = ' ';
    cs_imm_content.iCaretPos = mle_get_context(&sms_editor);

    status = ilME_IMM_EventHandle(cs_imm, event, msg, &cs_imm_content);
    ilME_ProcessContent();

	if (status == ilME_ERROR_COMMIT)
	{
		unsigned short text = (unsigned short)msg;
        ilME_CommitText(&text, 1);
	}

    return 0;
}

#endif /* !defined(INTERFACE_08_ONLY) */

#if defined(KMX_SP)
extern int g_active_state;
kmx_active_state ilME_IMM_GetActiveState(void)
{
    return g_active_state;
}

void ilME_IMM_SetOutputNumber(unsigned int i)
{

}

int mmi_get_candidate_window_metrics(CANDIDATE_WINDOW_METRICS* metrics)
{
    ilME_WCHAR szHanzi[2] = {0x4E00, 0};
    metrics->label_width = (unsigned short) (cs_ui_metrics.label_width + cs_ui_metrics.cand_extra_space);
    metrics->window_width = (unsigned short) (cs_ui_pos.cand_item.right - cs_ui_pos.cand_item.left);
    metrics->char_width = ilME_GetTextWidth(szHanzi, 1);

    return 1;
}

#endif /* KMX_SP */

