/*==============================================================================
File Name: mmiim_touch_ime_cstar.h
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/
/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#ifndef _MMIIM_TOUCH_IME_CSTAR_H_
#define _MMIIM_TOUCH_IME_CSTAR_H_

#include "mmiim_touch_ime_common.h"

#if defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_CSTAR)

#include "kmx_all_ldb.h"
#include "kmxime.h"



typedef struct KERNEL_DATA_LATIN_TAG
{
    KMX_IME_ENGINE engine;
    char input_string[KMX_MAX_LATIN_WORD_LENGTH];
    uint16 input_string_len;
    uint16 match_flags;
    KMX_LATIN_CANDIDATE cand[LATIN_CANDIDATE_COUNT];
    uint8 cand_cnt;
    uint8 page[CAND_PAGE_LIMIT];
    uint8 cur_page;
    uint8 has_next_page;
} KERNEL_DATA_LATIN_T;

typedef struct KERNEL_DATA_Q_LATIN_TAG
{
    KMX_IME_ENGINE engine;
    wchar input_string[KMX_MAX_LATIN_WORD_LENGTH];
    uint16 input_string_len;
    uint16 match_flags;
    KMX_LATIN_CANDIDATE cand[LATIN_CANDIDATE_COUNT];
    uint8 cand_cnt;
    uint8 page[CAND_PAGE_LIMIT];
    uint8 cur_page;
    uint8 has_next_page;
} KERNEL_DATA_Q_LATIN_T;

#if defined (IM_SIMP_CHINESE_SUPPORT)
typedef struct KERNEL_DATA_PINYIN_TAG
{
    KMX_IME_ENGINE engine;
    char input_string[KMX_MAX_PINYIN_INPUT];
    uint16 input_string_len;
    KMX_PINYIN_COMPOSITION pinyin_composition;
    uint16 pinyin_match_index;
    KMX_PINYIN_MATCH_RESULT pinyin_match_result;
    KMX_PINYIN_CANDIDATE cand[CH_CANDIDATE_COUNT];
    uint8 cand_cnt;
    uint8 cust_pinyin_first;
    uint8 expand_pinyin_page;
    uint8 cur_page;
    uint8 has_next_page;
    EDIT_STATE_T edit_state;
    wchar last_char;
    uint8 confirm_cnt;
    KMX_PINYIN_CANDIDATE confirm[KMX_MAX_PINYIN_INPUT];
} KERNEL_DATA_PINYIN_T;

typedef struct KERNEL_DATA_Q_PINYIN_TAG
{
    KMX_IME_ENGINE engine;
    char input_string[KMX_MAX_PINYIN_INPUT];
    uint16 input_string_len;
    KMX_PINYIN_CANDIDATE cand[CH_CANDIDATE_COUNT_E];
    uint8 cand_cnt;
    uint8 cur_page;
    uint8 has_next_page;
    EDIT_STATE_T edit_state;
    wchar last_char;
    uint8 confirm_cnt;
    KMX_PINYIN_CANDIDATE confirm[KMX_MAX_PINYIN_INPUT];
} KERNEL_DATA_Q_PINYIN_T;
#endif

#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))
typedef struct KERNEL_DATA_STROKE_TAG
{
    KMX_IME_ENGINE engine;
    char input_string[KMX_MAX_STROKE_INPUT_LENGTH];
    uint16 input_string_len;
    KMX_STROKE_CANDIDATE cand[CH_CANDIDATE_COUNT_E];
    uint8 cand_cnt;
    uint8 cur_page;
    uint8 has_next_page;
    EDIT_STATE_T edit_state;
    wchar last_char;
} KERNEL_DATA_STROKE_T;
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
typedef struct KERNEL_DATA_ZHUYIN_TAG
{
    KMX_IME_ENGINE engine;
    uint16 input_string[KMX_MAX_ZHUYIN_INPUT];
    uint16 input_string_len;
    //uint16 zhuyin_match_index;
    KMX_ZHUYIN_WORD_MATCH_RESULT zhuyin_match_result;
    KMX_ZHUYIN_CANDIDATE cand[CH_CANDIDATE_COUNT];
    uint8 cand_cnt;
    uint8 cust_zhuyin_first;
    uint8 expand_zhuyin_page;
    uint8 cur_page;
    uint8 has_next_page;
    EDIT_STATE_T edit_state;
    wchar last_char;
    KEY_INFO_T      *last_key;
    uint16      last_key_index;
    uint8           commit_timer;   //同一个按键字符切换定时器
    uint8 confirm_cnt;
    KMX_ZHUYIN_CANDIDATE confirm[KMX_MAX_ZHUYIN_INPUT];
} KERNEL_DATA_ZHUYIN_T;
typedef struct KERNEL_DATA_Q_ZHUYIN_TAG
{
    KMX_IME_ENGINE engine;
    uint16 input_string[KMX_MAX_ZHUYIN_INPUT];
    uint16 input_string_len;
    KMX_ZHUYIN_CANDIDATE cand[CH_CANDIDATE_COUNT_E];
    uint8 cand_cnt;
    uint8 cur_page;
    uint8 has_next_page;
    EDIT_STATE_T edit_state;
    wchar last_char;
    uint8 confirm_cnt;
    KMX_ZHUYIN_CANDIDATE confirm[KMX_MAX_ZHUYIN_INPUT];
} KERNEL_DATA_Q_ZHUYIN_T;
#endif

#if defined IM_AMHARIC_SUPPORT
//此处假设amharic语按当前的约定使用:28个辅音对应的28组元音；28个辅音分布在2~9这8个键上
//当按下2 ~9后，会要求先选择辅音，辅音选择后，会出现辅音能够组合的元音，然后选择
//元音上屏；
// 2~9每个按键对应的辅音个数不会超过5个，否则需要考虑翻页功能
//为了程序的简单，使用了上述约定，如果约定修改，则需要对应的修改程序
//
#define KMX_THUMB_AMHARIC_INPUT_STRING_MAX 15
#define KMX_THUMB_AMHARIC_CANDIDATE_MAX 8
#define KMX_THUMB_AMHARIC_CONSONANT_MAX 4
#define THUMB_AMHARIC_CAND_WIDTH_MIN (32)
typedef struct KERNEL_DATA_AMHARIC_TAG
{
    uint16 input_string[KMX_THUMB_AMHARIC_INPUT_STRING_MAX + 1];        //当前输入的按键
    uint16 input_string_len;

    KMX_LATIN_CANDIDATE candidate[KMX_THUMB_AMHARIC_CANDIDATE_MAX];     //候选字
    KMX_LATIN_CANDIDATE consonant[KMX_THUMB_AMHARIC_CONSONANT_MAX];     //候选辅音
    
    uint16 cand_cnt;       //当前候选字个数
    uint16 consonant_cnt;       //当前候选辅音个数
    uint16 consonant_index;     //当前辅音选择的索引
} KERNEL_DATA_AMHARIC_T;
#endif

#if defined IM_BENGALI_SUPPORT
#if defined IM_ENGINE_CSTAR		//cstar才有的
typedef struct BENGALI_MULTITAP_DATA_TAG
{
    KMX_IME_ENGINE engine;	//虽然使用multi-tap输入法，但是因为我们借用cstar检测字符输入规则，所以还是要使用cstar的输入法引擎

	//下面的数据为某些语言特有的信息，当某些语言存在输入规则时，需要这些字段，为了程序的统一性，全部使用这些字段表示multi-tap输入
	wchar 			cursor_char;		//准备输入时，编辑器光标前一个字符，用来检测即将输入的字符是否有效
	wchar 			text[TEXT_LEN_LIMIT];	//能够输入的有效字符，有时候，一个按键的所有字符并不是全部符合输入规则
	uint8 			count;					//有效字符个数
} BENGALI_MULTITAP_DATA_T;
#endif

typedef enum _BENGALI_CHAR_TYPE_E
{
	BENGALI_CONSONANT_E,
	BENGALI_INDEPENDENT_VOWLE_E,
	BENGALI_DEPENDENT_VOWLE_E,
	BENGALI_NUMBER_E,
	
	BENGALI_NOT_DEFINE_E,	//没有使用的字符

	BENGALI_OUT_E,	//不在孟加拉范围内
}BENGALI_CHAR_TYPE_E;

#endif

/* thumb en */
PUBLIC BOOLEAN ThumbLatinInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbLatinTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbLatinDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbLatinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbLatinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbLatinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbLatinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbLatinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbLatinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbLatinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC void ThumbLatinSetMatch(IM_DATA_T *data_ptr);
PUBLIC void ThumbLatinClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbLatinUpdate(IM_DATA_T *data_ptr);

PUBLIC void ThumbLatinSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbLatinDispEdit(IM_DATA_T *data_ptr);


#if defined (IM_SIMP_CHINESE_SUPPORT)
/* thumb pinyin */
PUBLIC BOOLEAN ThumbPinyinInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbPinyinTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbPinyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbPinyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC void ThumbPinyinClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinUpdate(IM_DATA_T *data_ptr);

PUBLIC void ThumbPinyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinDispEdit(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinSetCust(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinSetPyCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinAsso(IM_DATA_T *data_ptr);
#endif


#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))
/* thumb stroke */
PUBLIC BOOLEAN ThumbStrokeInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbStrokeTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbStrokeInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbStrokeCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC void ThumbStrokeClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeUpdate(IM_DATA_T *data_ptr);

PUBLIC void ThumbStrokeSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeDispEdit(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeAsso(IM_DATA_T *data_ptr);
#endif


/* qwert en */
PUBLIC BOOLEAN QwertLatinInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertLatinTerm(IM_DATA_T *data_ptr);
PUBLIC void QwertLatinDraw(IM_DATA_T * data_ptr);
PUBLIC void QwertLatinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertLatinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertLatinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertLatinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertLatinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertLatinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC void QwertLatinSetMatch(IM_DATA_T *data_ptr);
PUBLIC void QwertLatinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertLatinUpdate(IM_DATA_T *data_ptr);

PUBLIC void QwertLatinSetCand(IM_DATA_T *data_ptr);


#if defined (IM_SIMP_CHINESE_SUPPORT)
/* Qwert pinyin */
PUBLIC BOOLEAN QwertPinyinInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertPinyinTerm(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinDraw(IM_DATA_T * data_ptr);
PUBLIC void QwertPinyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC void QwertPinyinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinUpdate(IM_DATA_T *data_ptr);

PUBLIC void QwertPinyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinDispEdit(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinAsso(IM_DATA_T *data_ptr);
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
PUBLIC BOOLEAN ThumbZhuyinInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbZhuyinTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbZhuyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbZhuyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void ThumbZhuyinClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinUpdate(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinDispEdit(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinSetCust(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinSetZyCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbZhuyinAsso(IM_DATA_T *data_ptr);
#endif
#if defined (IM_TRAD_CHINESE_SUPPORT)
PUBLIC BOOLEAN QwertZhuyinInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertZhuyinTerm(IM_DATA_T *data_ptr);
PUBLIC void QwertZhuyinDraw(IM_DATA_T * data_ptr);
PUBLIC void QwertZhuyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void QwertZhuyinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertZhuyinUpdate(IM_DATA_T *data_ptr);
PUBLIC void QwertZhuyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void QwertZhuyinDispEdit(IM_DATA_T *data_ptr);
PUBLIC void QwertZhuyinAsso(IM_DATA_T *data_ptr);
#endif


PUBLIC const void *GetKmxLdb_ptr(GUIIM_LANGUAGE_T lang, BOOLEAN is_stoke_ldb);

PUBLIC BOOLEAN ThumbSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void ThumbSmartInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN SmartInitEngine(KMX_IME_ENGINE *kmx_save_ptr, GUIIM_LANGUAGE_T lang, GUIIM_METHOD_T method);
PUBLIC BOOLEAN ThumbSmartInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbSmartTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbSmartDraw(IM_DATA_T * data_ptr);
/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCommitDefaultCharUp

Func:   先提交默认的候选字，再处理自身按键

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的按键，会先检测该按键是否为字符按键，否则不做处理

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-10-26

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCommitDefaultCharUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC BOOLEAN ThumbSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbSmartCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void SmartSetMatch(IM_DATA_T *data_ptr);
PUBLIC void ThumbSmartClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbSmartUpdate(IM_DATA_T *data_ptr);
PUBLIC void ThumbSmartSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbSmartDispEdit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertSmartInit(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertSmartTerm(IM_DATA_T *data_ptr);
PUBLIC void QwertSmartDraw(IM_DATA_T * data_ptr);
PUBLIC void QwertSmartInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void QwertSmartClear(IM_DATA_T *data_ptr);
PUBLIC int32 QwertSmartConfigPeriod (KEY_INFO_T *key_ptr, MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr);
PUBLIC const void *GetKmxLdb_ptr(GUIIM_LANGUAGE_T lang, BOOLEAN is_stoke_ldb);

/* kernel相关函数 */
PUBLIC uint32 GetPyFuzzy(void);
PUBLIC int32 Cstar_SetCapsLock (IM_DATA_T *data_ptr);

PUBLIC int32 UpdateUdbCache(KMX_IME_ENGINE* engine, const uint16* string, uint16 length, GUIIM_METHOD_T method);

#if defined IM_AMHARIC_SUPPORT
    PUBLIC BOOLEAN ThumbAmharicInit(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbAmharicTerm (IM_DATA_T *data_ptr);
    PUBLIC void ThumbAmharicInitKb(IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbAmharicCustUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAmharicCharLong (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAmharicCharUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAmharicCharLock (IM_DATA_T *data_ptr, uint8 locked);
    PUBLIC void ThumbAmharicClear (IM_DATA_T *data_ptr);
    PUBLIC void ThumbAmharicUpdate (IM_DATA_T *data_ptr);
    PUBLIC BOOLEAN ThumbAmharicCtrlLong (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAmharicCtrlUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAmharicCandUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC BOOLEAN ThumbAmharicSysUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
    PUBLIC void ThumbAmharicSetCust(IM_DATA_T *data_ptr);
    PUBLIC void ThumbAmharicDraw (IM_DATA_T *data_ptr);
    PUBLIC void ThumbAmharicDispEdit(IM_DATA_T *data_ptr);
    PUBLIC void ThumbAmharicSetCandidate(IM_DATA_T *data_ptr);
    PUBLIC uint16 kmx_amharic_digit_match (uint16 asc_digit, KMX_LATIN_CANDIDATE *consonant_ptr);
    PUBLIC uint16 kmx_amharic_consonant_match (wchar consonant_char, KMX_LATIN_CANDIDATE *candidate_ptr);    
#endif

//孟加拉语特殊处理
#if defined IM_BENGALI_SUPPORT
/*----------------------------------------------------------------------------------------------------------------------
Func:   9键智能输入法初始化，无法复用ThumbSmartInit 函数，因键盘初始化不同

Input:
        data_ptr:    输入法指针

Output:

Return: TRUE SUC; ELSE FALSE

Thoughtway:

Create: yaoguangchen@spread. 2012-07-12

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbBengaliInit (IM_DATA_T *data_ptr);

/*----------------------------------------------------------------------------------------------------------------------

Func:   9键智能输入法键盘初始化

Input:
        data_ptr:    输入法指针

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-07-12

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliInitKb(IM_DATA_T *data_ptr);

/*==============================================================================
Description: 
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliVkeyupStar(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

/*==============================================================================
Description: 
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliTerm (IM_DATA_T *data_ptr);

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliMultitapInitKb

Func:   9键multi-tap输入法键盘初始化, 界面上的东西与ThumbSmartBengaliInitKb基本相同，仅仅字符按键消息处理函数不同

Input:
            data_ptr:    输入法指针

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-08-02

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliMultitapInitKb(IM_DATA_T *data_ptr);

/*==============================================================================
Description: 9键孟加拉语multi-tap输入法，不走通用版本，是因为孟加拉语的输入规则BTRC
			导致我们自己的multi-tap输入法无法满足该项需求，所以这里也使用到了cstar的
			输入法引擎，故初始化，关闭等，均走smart相关流程，仅仅在按键消息处理那块
			需要再做处理
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliMultitapInit (IM_DATA_T *data_ptr);

/*==============================================================================
Description: 孟加拉私有字符分拆

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC void BengaliSeparatePrivateChar (wchar private_char, wchar *separate_str);

/*==============================================================================
Description: 孟加拉语字符提交，与一般的提交不同，需要检测是否存在私有字符，
			并将私有字符给分拆开来

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC void BengaliCommitChar (IM_DATA_T *data_ptr, wchar c, BOOLEAN real_commit);

/*==============================================================================
Description: 孟加拉语，检测即将输入的一个字符是否与前面的字符一起，符合孟加拉BTRC
			输入规范

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC int32 BengaliCheckInputCharValid (wchar cursor_char, wchar input_char);

/*==============================================================================
Description: 孟加拉语字符分类

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC BENGALI_CHAR_TYPE_E BengaliCharType (wchar c);

/*==============================================================================
Description: 因当前按键中的所有按键码均有效，而multi-tap输入时，为减少每次计算
			按键码中有效的字符，故一次将所有的有效按键字符抽取出来
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 BengaliGetValidChars (wchar cursor_char, wchar *psrc_text, wchar *ptar_text);

/*==============================================================================
Description: ThumbAbcCommitLast
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliMultiTapCommitLast(IM_DATA_T *data_ptr);

/*==============================================================================
Description: ThumbAbcCommitCB
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void BengaliMultiTapCommitCB(uint8 timer_id, uint32 param);

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliMultitapCharUp

Func:   9键智能输入法，字符键0 ~ 9消息处理

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的按键，会先检测该按键是否为字符按键，否则不做处理

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-08-02

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbBengaliMultitapCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

/*==============================================================================
Description: 

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliMultitapTerm (IM_DATA_T *data_ptr);

#endif

#endif //defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_CSTAR)


#endif /*_MMIIM_TOUCH_IME_CSTAR_H_*/

