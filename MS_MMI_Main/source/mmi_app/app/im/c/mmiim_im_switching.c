/*==============================================================================
File Name: mmiim_im_swithing.c
Author: Haiwu.Chen
Date:
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/26   Haiwu.Chen      Create.
==============================================================================*/

#include "mmi_app_im_trc.h"
#include "window_parse.h"
#include "guistring.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "mmiim_text.h"
#include "mmiim_id.h"
#include "mmi_theme.h"

#include "mmiset_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "mmiim_im_switching.h"
#include "mmiim.h"
#include "mmiim_internal.h"
//#include "mmiset_func.h"
//#include "mmiim_menutable.h"
#include "guictrl_api.h"


/*
更新列表 -
20101018 增加输入语言设定显示，显示语言设置逻辑为
1 无限制输入
2 无指定输入语言
*/


/*****************************************************************************/
//  Description : to handle Switching input method
//  Global resource dependence :
//  Author:haiwu.chen
//  Note: 输入法切换窗口过程函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchingIMWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/*****************************************************************************/
//  Description : 获取列表各项值，包括对应索引表的构造
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 输入法切换窗口，列表项值构造
/*****************************************************************************/
LOCAL void InitIMDisplayValues(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr);

/*****************************************************************************/
//  Description : 动态创建输入法菜单项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateIMMenu(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr);

/*****************************************************************************/
//  Description : 初始化时，由method定listbox的焦点项，并存放在窗口数据中
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetIMFocusIndex(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr);

/*****************************************************************************/
//  Description : 输入法切换窗口，点击OK按键处理事件
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnIMClicked(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr
);

/*****************************************************************************/
//  Description : 设置当前的输入法方法，需要通知输入法控件
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SendChangeMethodMsg(GUIIM_METHOD_T method, MMI_HANDLE_T im_handle);

/*****************************************************************************/
//  Description : 只释放窗口内部释放资源
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnIMReleased(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr);


/*****************************************************************************/
//  Description : 当前输入语言切换窗口过程函数
//  Global resource dependence :
//  Author:haiwu.chen
//  Note: 窗口过程函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetInputLangWindow(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/*****************************************************************************/
//  Description : 动态创建输入法菜单项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateLangMenu(MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr);

/*****************************************************************************/
//  Description : 动态添加项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AppendLangItems(
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr
);

/*****************************************************************************/
//  Description : 点击OK按纽事件
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnLangClicked(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr
);

/*****************************************************************************/
//  Description : 只释放窗口内部释放资源
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnLangReleased(MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr);

/*****************************************************************************/
//  Description : get  valid lang index by lang index from s_lang_set
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
LOCAL uint32 GetValidIdxByLangIdx(uint32 lang_index);

/*****************************************************************************/
//  Description : 通过新的输入语言，获取默认的输入法，目前设为第一项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL GUIIM_METHOD_T GetNewMethodWhenLangWinClose(
    MMIIM_SET_INPUT_LANG_WIN_DATA_T *win_data_ptr,
    GUIIM_LANGUAGE_T language_type
);

////////////////////////////////////////////////////////////////////////////////
//input methods' name list
/*
各种输入法，不管是什么内核，什么模式下，只要是个输入法，common表示通用的
则其名字都可以在下表中查到。下面各项表示输入法的语言，可以考虑换成多国语
*/
static MMIIM_IM_NAME_T const s_abc_im_name_list[] =
{
    {GUIIM_M_ENGLISH_ABC_LEAD_UPPER,        IM_TXT_IM_SHORT_ABC_LEAD_UPPER,             FALSE},
    {GUIIM_M_ENGLISH_ABC_LOWER,             IM_TXT_IM_SHORT_ABC_LOWER,                  FALSE},
    {GUIIM_M_ENGLISH_ABC_UPPER,             IM_TXT_IM_SHORT_ABC_UPPER,                  FALSE},
};

static MMIIM_IM_NAME_T const s_123_im_name_list[] =
{
    {GUIIM_M_123,                           IM_TXT_IM_SHORT_123,                        FALSE}
};
static MMIIM_IM_NAME_T const s_sys_im_name_list[] =
{
    {GUIIM_M_SYS,                           IM_TXT_IM_DEFAULT,                        FALSE}
};


static MMIIM_IM_NAME_T const s_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER,     IM_TXT_IM_SHORT_ABC_LEAD_UPPER,             FALSE},
    {GUIIM_M_TP_ENGLISH_ABC_LOWER,          IM_TXT_IM_SHORT_ABC_LOWER,                  FALSE},
    {GUIIM_M_TP_ENGLISH_ABC_UPPER,          IM_TXT_IM_SHORT_ABC_UPPER,                  FALSE},
};
static MMIIM_IM_NAME_T const s_sys_im_tp_name_list[] =
{
    {GUIIM_M_TP_SYS,  IM_TXT_IM_DEFAULT,                  FALSE}
};

static MMIIM_IM_NAME_T const s_123_im_tp_name_list[] =
{
    {GUIIM_M_TP_123,                        IM_TXT_IM_SHORT_123,                        FALSE}
};


#if defined(MMIIM_USE_HANDWRITING)
static MMIIM_IM_NAME_T const s_hw_im_name_list[] =
{
    {GUIIM_M_HANDWRITING,                   IM_TXT_IM_SHORT_HW,                         FALSE}
};
#endif

//input language
static MMIIM_IM_NAME_T const s_cur_lang_name_list[] =
{
    {0,                                     TXT_INPUT_LANGUAGE,                 FALSE}
};

#if defined MMI_IM_PDA_SUPPORT
//input language
static MMIIM_IM_NAME_T const s_more_set_name_list[] =
{
    {0,                                     TXT_COMN_MORE,                 FALSE}
};
#endif

#ifdef IM_ENGLISH_SUPPORT
static MMIIM_IM_NAME_T const s_english_im_name_list[] =
{
    {GUIIM_M_ENGLISH_LEAD_UPPER,    IM_TXT_IM_SHORT_ENGLISH_LEAD_UPPER, FALSE},
    {GUIIM_M_ENGLISH_LOWER,         IM_TXT_IM_SHORT_ENGLISH_LOWER, FALSE},
    {GUIIM_M_ENGLISH_UPPER,         IM_TXT_IM_SHORT_ENGLISH_UPPER, FALSE}
};
#if defined(MMIIM_CORE_USE_SPIM)
static MMIIM_IM_NAME_T const s_english_im_tp_name_list[] =
{
    {GUIIM_M_TP_ENGLISH,                 IM_TXT_IM_SHORT_ENGLISH_LEAD_UPPER},
    {GUIIM_M_TP_ENGLISH,                 IM_TXT_IM_SHORT_ENGLISH_LOWER},
    {GUIIM_M_TP_ENGLISH,                 IM_TXT_IM_SHORT_ENGLISH_UPPER}
};
#else
static MMIIM_IM_NAME_T const s_english_im_tp_name_list[] =
{
    {GUIIM_M_TP_ENGLISH_LEAD_UPPER, IM_TXT_IM_SHORT_ENGLISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ENGLISH_LOWER,      IM_TXT_IM_SHORT_ENGLISH_LOWER, FALSE},
    {GUIIM_M_TP_ENGLISH_UPPER,      IM_TXT_IM_SHORT_ENGLISH_UPPER, FALSE}
};
#endif
#endif

#ifdef IM_SIMP_CHINESE_SUPPORT
static MMIIM_IM_NAME_T const s_simpchs_im_name_list[] =
{
    {GUIIM_M_PINYIN,                IM_TXT_IM_SHORT_PINYIN, FALSE},
    {GUIIM_M_STROKE,                IM_TXT_IM_SHORT_STROKE, FALSE}
};
static MMIIM_IM_NAME_T const s_simpchs_im_tp_name_list[] =
{
    {GUIIM_M_TP_PINYIN,             IM_TXT_IM_SHORT_PINYIN, FALSE},
    {GUIIM_M_TP_STROKE,             IM_TXT_IM_SHORT_STROKE, FALSE}
};
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
static MMIIM_IM_NAME_T const s_tradchs_im_name_list[] =
{
    {GUIIM_M_ZHUYIN,                IM_TXT_IM_SHORT_ZHUYIN, FALSE},
    {GUIIM_M_STROKE_CHT,           IM_TXT_IM_SHORT_STROKE, FALSE}
};
static MMIIM_IM_NAME_T const s_tradchs_im_tp_name_list[] =
{
    {GUIIM_M_TP_ZHUYIN,             IM_TXT_IM_SHORT_ZHUYIN, FALSE},
    {GUIIM_M_TP_STROKE_CHT,        IM_TXT_IM_SHORT_STROKE, FALSE}
};
#endif

#ifdef IM_ARABIC_SUPPORT
static MMIIM_IM_NAME_T const s_arabic_im_name_list[] =
{
    {GUIIM_M_ARABIC,                IM_TXT_IM_SHORT_ARABIC, FALSE}
};
static MMIIM_IM_NAME_T const s_arabic_abc_im_name_list[] =
{
    {GUIIM_M_ARABIC_ABC,            IM_TXT_IM_SHORT_ARABIC_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_arabic_im_tp_name_list[] =
{
    {GUIIM_M_TP_ARABIC,             IM_TXT_IM_SHORT_ARABIC, FALSE}
};
static MMIIM_IM_NAME_T const s_arabic_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ARABIC_ABC,         IM_TXT_IM_SHORT_ARABIC_ABC, FALSE}
};
#endif

#ifdef IM_FRENCH_SUPPORT
static MMIIM_IM_NAME_T const s_french_im_name_list[] =
{
    {GUIIM_M_FRENCH_LEAD_UPPER,     IM_TXT_IM_SHORT_FRENCH_LEAD_UPPER, FALSE},
    {GUIIM_M_FRENCH_LOWER,          IM_TXT_IM_SHORT_FRENCH_LOWER, FALSE},
    {GUIIM_M_FRENCH_UPPER,          IM_TXT_IM_SHORT_FRENCH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_french_abc_im_name_list[] =
{
    {GUIIM_M_FRENCH_ABC_LEAD_UPPER,     IM_TXT_IM_SHORT_FRENCH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_FRENCH_ABC_LOWER,          IM_TXT_IM_SHORT_FRENCH_ABC_LOWER, FALSE},
    {GUIIM_M_FRENCH_ABC_UPPER,          IM_TXT_IM_SHORT_FRENCH_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_french_im_tp_name_list[] =
{
    {GUIIM_M_TP_FRENCH_LEAD_UPPER,      IM_TXT_IM_SHORT_FRENCH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_FRENCH_LOWER,           IM_TXT_IM_SHORT_FRENCH_LOWER, FALSE},
    {GUIIM_M_TP_FRENCH_UPPER,           IM_TXT_IM_SHORT_FRENCH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_french_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_FRENCH_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_FRENCH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_FRENCH_ABC_LOWER,           IM_TXT_IM_SHORT_FRENCH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_FRENCH_ABC_UPPER,           IM_TXT_IM_SHORT_FRENCH_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_GERMAN_SUPPORT
static MMIIM_IM_NAME_T const s_german_im_name_list[] =
{
    {GUIIM_M_GERMAN_LEAD_UPPER,     IM_TXT_IM_SHORT_GERMAN_LEAD_UPPER, FALSE},
    {GUIIM_M_GERMAN_LOWER,          IM_TXT_IM_SHORT_GERMAN_LOWER, FALSE},
    {GUIIM_M_GERMAN_UPPER,          IM_TXT_IM_SHORT_GERMAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_german_abc_im_name_list[] =
{
    {GUIIM_M_GERMAN_ABC_LEAD_UPPER,     IM_TXT_IM_SHORT_GERMAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_GERMAN_ABC_LOWER,          IM_TXT_IM_SHORT_GERMAN_ABC_LOWER, FALSE},
    {GUIIM_M_GERMAN_ABC_UPPER,          IM_TXT_IM_SHORT_GERMAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_german_im_tp_name_list[] =
{
    {GUIIM_M_TP_GERMAN_LEAD_UPPER,     IM_TXT_IM_SHORT_GERMAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_GERMAN_LOWER,          IM_TXT_IM_SHORT_GERMAN_LOWER, FALSE},
    {GUIIM_M_TP_GERMAN_UPPER,          IM_TXT_IM_SHORT_GERMAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_german_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_GERMAN_ABC_LEAD_UPPER,     IM_TXT_IM_SHORT_GERMAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_GERMAN_ABC_LOWER,          IM_TXT_IM_SHORT_GERMAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_GERMAN_ABC_UPPER,          IM_TXT_IM_SHORT_GERMAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_GREEK_SUPPORT
static MMIIM_IM_NAME_T const s_greek_im_name_list[] =
{
    {GUIIM_M_GREEK_LEAD_UPPER,      IM_TXT_IM_SHORT_GREEK_LEAD_UPPER, FALSE},
    {GUIIM_M_GREEK_LOWER,           IM_TXT_IM_SHORT_GREEK_LOWER, FALSE},
    {GUIIM_M_GREEK_UPPER,           IM_TXT_IM_SHORT_GREEK_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_greek_abc_im_name_list[] =
{
    {GUIIM_M_GREEK_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_GREEK_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_GREEK_ABC_LOWER,           IM_TXT_IM_SHORT_GREEK_ABC_LOWER, FALSE},
    {GUIIM_M_GREEK_ABC_UPPER,           IM_TXT_IM_SHORT_GREEK_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_greek_im_tp_name_list[] =
{
    {GUIIM_M_TP_GREEK_LEAD_UPPER,      IM_TXT_IM_SHORT_GREEK_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_GREEK_LOWER,           IM_TXT_IM_SHORT_GREEK_LOWER, FALSE},
    {GUIIM_M_TP_GREEK_UPPER,           IM_TXT_IM_SHORT_GREEK_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_greek_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_GREEK_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_GREEK_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_GREEK_ABC_LOWER,           IM_TXT_IM_SHORT_GREEK_ABC_LOWER, FALSE},
    {GUIIM_M_TP_GREEK_ABC_UPPER,           IM_TXT_IM_SHORT_GREEK_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_HINDI_SUPPORT
static MMIIM_IM_NAME_T const s_hidi_im_name_list[] =
{
    {GUIIM_M_HINDI,                     IM_TXT_IM_SHORT_HINDI, FALSE}
};
static MMIIM_IM_NAME_T const s_hidi_abc_im_name_list[] =
{
    {GUIIM_M_HINDI_ABC,                 IM_TXT_IM_SHORT_HINDI_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_hidi_im_tp_name_list[] =
{
    {GUIIM_M_TP_HINDI,                  IM_TXT_IM_SHORT_HINDI, FALSE}
};
static MMIIM_IM_NAME_T const s_hidi_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_HINDI_ABC,              IM_TXT_IM_SHORT_HINDI_ABC, FALSE}
};
#endif

#ifdef IM_HUNGARIAN_SUPPORT
static MMIIM_IM_NAME_T const s_hungarian_im_name_list[] =
{
    {GUIIM_M_HUNGARIAN_LEAD_UPPER,  IM_TXT_IM_SHORT_HUNGARIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_HUNGARIAN_LOWER,       IM_TXT_IM_SHORT_HUNGARIAN_LOWER, FALSE},
    {GUIIM_M_HUNGARIAN_UPPER,       IM_TXT_IM_SHORT_HUNGARIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_hungarian_abc_im_name_list[] =
{
    {GUIIM_M_HUNGARIAN_ABC_LEAD_UPPER,  IM_TXT_IM_SHORT_HUNGARIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_HUNGARIAN_ABC_LOWER,       IM_TXT_IM_SHORT_HUNGARIAN_ABC_LOWER, FALSE},
    {GUIIM_M_HUNGARIAN_ABC_UPPER,       IM_TXT_IM_SHORT_HUNGARIAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_hungarian_im_tp_name_list[] =
{
    {GUIIM_M_TP_HUNGARIAN_LEAD_UPPER,  IM_TXT_IM_SHORT_HUNGARIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_HUNGARIAN_LOWER,       IM_TXT_IM_SHORT_HUNGARIAN_LOWER, FALSE},
    {GUIIM_M_TP_HUNGARIAN_UPPER,       IM_TXT_IM_SHORT_HUNGARIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_hungarian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_HUNGARIAN_ABC_LEAD_UPPER,  IM_TXT_IM_SHORT_HUNGARIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_HUNGARIAN_ABC_LOWER,       IM_TXT_IM_SHORT_HUNGARIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_HUNGARIAN_ABC_UPPER,       IM_TXT_IM_SHORT_HUNGARIAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_INDONESIAN_SUPPORT
static MMIIM_IM_NAME_T const s_indonesian_im_name_list[] =
{
    {GUIIM_M_INDONESIAN_LEAD_UPPER, IM_TXT_IM_SHORT_INDONESIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_INDONESIAN_LOWER,      IM_TXT_IM_SHORT_INDONESIAN_LOWER, FALSE},
    {GUIIM_M_INDONESIAN_UPPER,      IM_TXT_IM_SHORT_INDONESIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_indonesian_abc_im_name_list[] =
{
    {GUIIM_M_INDONESIAN_ABC_LEAD_UPPER, IM_TXT_IM_SHORT_INDONESIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_INDONESIAN_ABC_LOWER,      IM_TXT_IM_SHORT_INDONESIAN_ABC_LOWER, FALSE},
    {GUIIM_M_INDONESIAN_ABC_UPPER,      IM_TXT_IM_SHORT_INDONESIAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_indonesian_im_tp_name_list[] =
{
    {GUIIM_M_TP_INDONESIAN_LEAD_UPPER, IM_TXT_IM_SHORT_INDONESIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_INDONESIAN_LOWER,      IM_TXT_IM_SHORT_INDONESIAN_LOWER, FALSE},
    {GUIIM_M_TP_INDONESIAN_UPPER,      IM_TXT_IM_SHORT_INDONESIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_indonesian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_INDONESIAN_ABC_LEAD_UPPER, IM_TXT_IM_SHORT_INDONESIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_INDONESIAN_ABC_LOWER,      IM_TXT_IM_SHORT_INDONESIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_INDONESIAN_ABC_UPPER,      IM_TXT_IM_SHORT_INDONESIAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_ITALIAN_SUPPORT
static MMIIM_IM_NAME_T const s_italian_im_name_list[] =
{
    {GUIIM_M_ITALIAN_LEAD_UPPER,    IM_TXT_IM_SHORT_ITALIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_ITALIAN_LOWER,         IM_TXT_IM_SHORT_ITALIAN_LOWER, FALSE},
    {GUIIM_M_ITALIAN_UPPER,         IM_TXT_IM_SHORT_ITALIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_italian_abc_im_name_list[] =
{
    {GUIIM_M_ITALIAN_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_ITALIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ITALIAN_ABC_LOWER,         IM_TXT_IM_SHORT_ITALIAN_ABC_LOWER, FALSE},
    {GUIIM_M_ITALIAN_ABC_UPPER,         IM_TXT_IM_SHORT_ITALIAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_italian_im_tp_name_list[] =
{
    {GUIIM_M_TP_ITALIAN_LEAD_UPPER,    IM_TXT_IM_SHORT_ITALIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ITALIAN_LOWER,         IM_TXT_IM_SHORT_ITALIAN_LOWER, FALSE},
    {GUIIM_M_TP_ITALIAN_UPPER,         IM_TXT_IM_SHORT_ITALIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_italian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ITALIAN_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_ITALIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ITALIAN_ABC_LOWER,         IM_TXT_IM_SHORT_ITALIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ITALIAN_ABC_UPPER,         IM_TXT_IM_SHORT_ITALIAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_MALAY_SUPPORT
static MMIIM_IM_NAME_T const s_malay_im_name_list[] =
{
    {GUIIM_M_MALAY_LEAD_UPPER,      IM_TXT_IM_SHORT_MALAY_LEAD_UPPER, FALSE},
    {GUIIM_M_MALAY_LOWER,           IM_TXT_IM_SHORT_MALAY_LOWER, FALSE},
    {GUIIM_M_MALAY_UPPER,           IM_TXT_IM_SHORT_MALAY_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_malay_abc_im_name_list[] =
{
    {GUIIM_M_MALAY_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_MALAY_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_MALAY_ABC_LOWER,           IM_TXT_IM_SHORT_MALAY_ABC_LOWER, FALSE},
    {GUIIM_M_MALAY_ABC_UPPER,           IM_TXT_IM_SHORT_MALAY_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_malay_im_tp_name_list[] =
{
    {GUIIM_M_TP_MALAY_LEAD_UPPER,      IM_TXT_IM_SHORT_MALAY_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_MALAY_LOWER,           IM_TXT_IM_SHORT_MALAY_LOWER, FALSE},
    {GUIIM_M_TP_MALAY_UPPER,           IM_TXT_IM_SHORT_MALAY_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_malay_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_MALAY_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_MALAY_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_MALAY_ABC_LOWER,           IM_TXT_IM_SHORT_MALAY_ABC_LOWER, FALSE},
    {GUIIM_M_TP_MALAY_ABC_UPPER,           IM_TXT_IM_SHORT_MALAY_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_PERSIAN_SUPPORT
static MMIIM_IM_NAME_T const s_persian_im_name_list[] =
{
    {GUIIM_M_PERSIAN,               IM_TXT_IM_SHORT_PERSIAN, FALSE}
};
static MMIIM_IM_NAME_T const s_persian_abc_im_name_list[] =
{
    {GUIIM_M_PERSIAN_ABC,               IM_TXT_IM_SHORT_PERSIAN_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_persian_im_tp_name_list[] =
{
    {GUIIM_M_TP_PERSIAN,            IM_TXT_IM_SHORT_PERSIAN, FALSE}
};
static MMIIM_IM_NAME_T const s_persian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_PERSIAN_ABC,            IM_TXT_IM_SHORT_PERSIAN_ABC, FALSE}
};
#endif

#ifdef IM_PORTUGUESE_SUPPORT
static MMIIM_IM_NAME_T const s_portuguese_im_name_list[] =
{
    {GUIIM_M_PORTUGUESE_LEAD_UPPER, IM_TXT_IM_SHORT_PORTUGUESE_LEAD_UPPER, FALSE},
    {GUIIM_M_PORTUGUESE_LOWER,      IM_TXT_IM_SHORT_PORTUGUESE_LOWER, FALSE},
    {GUIIM_M_PORTUGUESE_UPPER,      IM_TXT_IM_SHORT_PORTUGUESE_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_portuguese_abc_im_name_list[] =
{
    {GUIIM_M_PORTUGUESE_ABC_LEAD_UPPER, IM_TXT_IM_SHORT_PORTUGUESE_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_PORTUGUESE_ABC_LOWER,      IM_TXT_IM_SHORT_PORTUGUESE_ABC_LOWER, FALSE},
    {GUIIM_M_PORTUGUESE_ABC_UPPER,      IM_TXT_IM_SHORT_PORTUGUESE_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_portuguese_im_tp_name_list[] =
{
    {GUIIM_M_TP_PORTUGUESE_LEAD_UPPER,  IM_TXT_IM_SHORT_PORTUGUESE_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_PORTUGUESE_LOWER,       IM_TXT_IM_SHORT_PORTUGUESE_LOWER, FALSE},
    {GUIIM_M_TP_PORTUGUESE_UPPER,       IM_TXT_IM_SHORT_PORTUGUESE_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_portuguese_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_PORTUGUESE_ABC_LEAD_UPPER,  IM_TXT_IM_SHORT_PORTUGUESE_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_PORTUGUESE_ABC_LOWER,       IM_TXT_IM_SHORT_PORTUGUESE_ABC_LOWER, FALSE},
    {GUIIM_M_TP_PORTUGUESE_ABC_UPPER,       IM_TXT_IM_SHORT_PORTUGUESE_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_RUSSIAN_SUPPORT
static MMIIM_IM_NAME_T const s_russian_im_name_list[] =
{
    {GUIIM_M_RUSSIAN_LEAD_UPPER,    IM_TXT_IM_SHORT_RUSSIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_RUSSIAN_LOWER,         IM_TXT_IM_SHORT_RUSSIAN_LOWER, FALSE},
    {GUIIM_M_RUSSIAN_UPPER,         IM_TXT_IM_SHORT_RUSSIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_russian_abc_im_name_list[] =
{
    {GUIIM_M_RUSSIAN_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_RUSSIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_RUSSIAN_ABC_LOWER,         IM_TXT_IM_SHORT_RUSSIAN_ABC_LOWER, FALSE},
    {GUIIM_M_RUSSIAN_ABC_UPPER,         IM_TXT_IM_SHORT_RUSSIAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_russian_im_tp_name_list[] =
{
    {GUIIM_M_TP_RUSSIAN_LEAD_UPPER,    IM_TXT_IM_SHORT_RUSSIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_RUSSIAN_LOWER,         IM_TXT_IM_SHORT_RUSSIAN_LOWER, FALSE},
    {GUIIM_M_TP_RUSSIAN_UPPER,         IM_TXT_IM_SHORT_RUSSIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_russian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_RUSSIAN_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_RUSSIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_RUSSIAN_ABC_LOWER,         IM_TXT_IM_SHORT_RUSSIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_RUSSIAN_ABC_UPPER,         IM_TXT_IM_SHORT_RUSSIAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_SPANISH_SUPPORT
static MMIIM_IM_NAME_T const s_spanish_im_name_list[] =
{
    {GUIIM_M_SPANISH_LEAD_UPPER,    IM_TXT_IM_SHORT_SPANISH_LEAD_UPPER, FALSE},
    {GUIIM_M_SPANISH_LOWER,         IM_TXT_IM_SHORT_SPANISH_LOWER, FALSE},
    {GUIIM_M_SPANISH_UPPER,         IM_TXT_IM_SHORT_SPANISH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_spanish_abc_im_name_list[] =
{
    {GUIIM_M_SPANISH_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_SPANISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SPANISH_ABC_LOWER,         IM_TXT_IM_SHORT_SPANISH_ABC_LOWER, FALSE},
    {GUIIM_M_SPANISH_ABC_UPPER,         IM_TXT_IM_SHORT_SPANISH_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_spanish_im_tp_name_list[] =
{
    {GUIIM_M_TP_SPANISH_LEAD_UPPER,    IM_TXT_IM_SHORT_SPANISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SPANISH_LOWER,         IM_TXT_IM_SHORT_SPANISH_LOWER, FALSE},
    {GUIIM_M_TP_SPANISH_UPPER,         IM_TXT_IM_SHORT_SPANISH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_spanish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SPANISH_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_SPANISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SPANISH_ABC_LOWER,         IM_TXT_IM_SHORT_SPANISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SPANISH_ABC_UPPER,         IM_TXT_IM_SHORT_SPANISH_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_TAGALOG_SUPPORT
static MMIIM_IM_NAME_T const s_tagalog_im_name_list[] =
{
    {GUIIM_M_TAGALOG,                       IM_TXT_IM_SHORT_TAGALOG, FALSE}
};
static MMIIM_IM_NAME_T const s_tagalog_abc_im_name_list[] =
{
    {GUIIM_M_TAGALOG_ABC,                   IM_TXT_IM_SHORT_TAGALOG_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_tagalog_im_tp_name_list[] =
{
    {GUIIM_M_TP_TAGALOG,                    IM_TXT_IM_SHORT_TAGALOG, FALSE}
};
static MMIIM_IM_NAME_T const s_tagalog_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_TAGALOG_ABC,                IM_TXT_IM_SHORT_TAGALOG_ABC, FALSE}
};
#endif

#ifdef IM_THAI_SUPPORT
static MMIIM_IM_NAME_T const s_thai_im_name_list[] =
{
    {GUIIM_M_THAI,                      IM_TXT_IM_SHORT_THAI, FALSE}
};
static MMIIM_IM_NAME_T const s_thai_abc_im_name_list[] =
{
    {GUIIM_M_THAI_ABC,                  IM_TXT_IM_SHORT_THAI_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_thai_im_tp_name_list[] =
{
    {GUIIM_M_TP_THAI,                   IM_TXT_IM_SHORT_THAI, FALSE}
};
static MMIIM_IM_NAME_T const s_thai_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_THAI_ABC,               IM_TXT_IM_SHORT_THAI_ABC, FALSE}
};
#endif

#ifdef IM_TURKISH_SUPPORT
static MMIIM_IM_NAME_T const s_turkish_im_name_list[] =
{
    {GUIIM_M_TURKISH_LEAD_UPPER,    IM_TXT_IM_SHORT_TURKISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TURKISH_LOWER,         IM_TXT_IM_SHORT_TURKISH_LOWER, FALSE},
    {GUIIM_M_TURKISH_UPPER,         IM_TXT_IM_SHORT_TURKISH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_turkish_abc_im_name_list[] =
{
    {GUIIM_M_TURKISH_ABC_LEAD_UPPER,    IM_TXT_IM_SHORT_TURKISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TURKISH_ABC_LOWER,         IM_TXT_IM_SHORT_TURKISH_ABC_LOWER, FALSE},
    {GUIIM_M_TURKISH_ABC_UPPER,         IM_TXT_IM_SHORT_TURKISH_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_turkish_im_tp_name_list[] =
{
    {GUIIM_M_TP_TURKISH_LEAD_UPPER, IM_TXT_IM_SHORT_TURKISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_TURKISH_LOWER,      IM_TXT_IM_SHORT_TURKISH_LOWER, FALSE},
    {GUIIM_M_TP_TURKISH_UPPER,      IM_TXT_IM_SHORT_TURKISH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_turkish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_TURKISH_ABC_LEAD_UPPER, IM_TXT_IM_SHORT_TURKISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_TURKISH_ABC_LOWER,      IM_TXT_IM_SHORT_TURKISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_TURKISH_ABC_UPPER,      IM_TXT_IM_SHORT_TURKISH_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_URDU_SUPPORT
static MMIIM_IM_NAME_T const s_urdu_im_name_list[] =
{
    {GUIIM_M_URDU,                  IM_TXT_IM_SHORT_URDU, FALSE}
};
static MMIIM_IM_NAME_T const s_urdu_abc_im_name_list[] =
{
    {GUIIM_M_URDU_ABC,                  IM_TXT_IM_SHORT_URDU_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_urdu_im_tp_name_list[] =
{
    {GUIIM_M_TP_URDU,                  IM_TXT_IM_SHORT_URDU, FALSE}
};
static MMIIM_IM_NAME_T const s_urdu_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_URDU_ABC,                  IM_TXT_IM_SHORT_URDU_ABC, FALSE}
};
#endif

#ifdef IM_VIETNAMESE_SUPPORT
static MMIIM_IM_NAME_T const s_vietnamese_im_name_list[] =
{
    {GUIIM_M_VIETNAMESE_LEAD_UPPER, IM_TXT_IM_SHORT_VIETNAMESE_LEAD_UPPER, FALSE},
    {GUIIM_M_VIETNAMESE_LOWER,      IM_TXT_IM_SHORT_VIETNAMESE_LOWER, FALSE},
    {GUIIM_M_VIETNAMESE_UPPER,      IM_TXT_IM_SHORT_VIETNAMESE_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_vietnamese_abc_im_name_list[] =
{
    {GUIIM_M_VIETNAMESE_ABC_LEAD_UPPER, IM_TXT_IM_SHORT_VIETNAMESE_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_VIETNAMESE_ABC_LOWER,      IM_TXT_IM_SHORT_VIETNAMESE_ABC_LOWER, FALSE},
    {GUIIM_M_VIETNAMESE_ABC_UPPER,      IM_TXT_IM_SHORT_VIETNAMESE_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_vietnamese_im_tp_name_list[] =
{
    {GUIIM_M_TP_VIETNAMESE_LEAD_UPPER, IM_TXT_IM_SHORT_VIETNAMESE_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_VIETNAMESE_LOWER,      IM_TXT_IM_SHORT_VIETNAMESE_LOWER, FALSE},
    {GUIIM_M_TP_VIETNAMESE_UPPER,      IM_TXT_IM_SHORT_VIETNAMESE_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_vietnamese_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_VIETNAMESE_ABC_LEAD_UPPER, IM_TXT_IM_SHORT_VIETNAMESE_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_VIETNAMESE_ABC_LOWER,      IM_TXT_IM_SHORT_VIETNAMESE_ABC_LOWER, FALSE},
    {GUIIM_M_TP_VIETNAMESE_ABC_UPPER,      IM_TXT_IM_SHORT_VIETNAMESE_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_HEBREW_SUPPORT
static MMIIM_IM_NAME_T const s_hebrew_im_name_list[] =
{
    {GUIIM_M_HEBREW,                  IM_TXT_IM_SHORT_HEBREW, FALSE}
};
static MMIIM_IM_NAME_T const s_hebrew_abc_im_name_list[] =
{
    {GUIIM_M_HEBREW_ABC,                  IM_TXT_IM_SHORT_HEBREW_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_hebrew_im_tp_name_list[] =
{
    {GUIIM_M_TP_HEBREW,               IM_TXT_IM_SHORT_HEBREW, FALSE}
};
static MMIIM_IM_NAME_T const s_hebrew_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_HEBREW_ABC,               IM_TXT_IM_SHORT_HEBREW_ABC, FALSE}
};
#endif

#ifdef IM_BENGALI_SUPPORT
static MMIIM_IM_NAME_T const s_bengali_im_name_list[] =
{
    {GUIIM_M_BENGALI,                 IM_TXT_IM_SHORT_BENGALI, FALSE}
};
static MMIIM_IM_NAME_T const s_bengali_abc_im_name_list[] =
{
    {GUIIM_M_BENGALI_ABC,                 IM_TXT_IM_SHORT_BENGALI_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_bengali_im_tp_name_list[] =
{
    {GUIIM_M_TP_BENGALI,              IM_TXT_IM_SHORT_BENGALI, FALSE}
};
static MMIIM_IM_NAME_T const s_bengali_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_BENGALI_ABC,              IM_TXT_IM_SHORT_BENGALI_ABC, FALSE}
};
#endif

#ifdef IM_CZECH_SUPPORT
static MMIIM_IM_NAME_T const s_czech_im_name_list[] =
{
    {GUIIM_M_CZECH_LEAD_UPPER,      IM_TXT_IM_SHORT_CZECH_LEAD_UPPER, FALSE},
    {GUIIM_M_CZECH_LOWER,           IM_TXT_IM_SHORT_CZECH_LOWER, FALSE},
    {GUIIM_M_CZECH_UPPER,           IM_TXT_IM_SHORT_CZECH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_czech_abc_im_name_list[] =
{
    {GUIIM_M_CZECH_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_CZECH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_CZECH_ABC_LOWER,           IM_TXT_IM_SHORT_CZECH_ABC_LOWER, FALSE},
    {GUIIM_M_CZECH_ABC_UPPER,           IM_TXT_IM_SHORT_CZECH_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_czech_im_tp_name_list[] =
{
    {GUIIM_M_TP_CZECH_LEAD_UPPER,      IM_TXT_IM_SHORT_CZECH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_CZECH_LOWER,           IM_TXT_IM_SHORT_CZECH_LOWER, FALSE},
    {GUIIM_M_TP_CZECH_UPPER,           IM_TXT_IM_SHORT_CZECH_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_czech_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_CZECH_ABC_LEAD_UPPER,      IM_TXT_IM_SHORT_CZECH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_CZECH_ABC_LOWER,           IM_TXT_IM_SHORT_CZECH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_CZECH_ABC_UPPER,           IM_TXT_IM_SHORT_CZECH_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_SLOVENIAN_SUPPORT
static MMIIM_IM_NAME_T const s_slovenian_im_name_list[] =
{
    {GUIIM_M_SLOVENIAN_LEAD_UPPER,  IM_TXT_IM_SHORT_SLOVENIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_SLOVENIAN_LOWER,       IM_TXT_IM_SHORT_SLOVENIAN_LOWER, FALSE},
    {GUIIM_M_SLOVENIAN_UPPER,       IM_TXT_IM_SHORT_SLOVENIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_slovenian_abc_im_name_list[] =
{
    {GUIIM_M_SLOVENIAN_ABC_LEAD_UPPER,  IM_TXT_IM_SHORT_SLOVENIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SLOVENIAN_ABC_LOWER,       IM_TXT_IM_SHORT_SLOVENIAN_ABC_LOWER, FALSE},
    {GUIIM_M_SLOVENIAN_ABC_UPPER,       IM_TXT_IM_SHORT_SLOVENIAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_slovenian_im_tp_name_list[] =
{
    {GUIIM_M_TP_SLOVENIAN_LEAD_UPPER,  IM_TXT_IM_SHORT_SLOVENIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SLOVENIAN_LOWER,       IM_TXT_IM_SHORT_SLOVENIAN_LOWER, FALSE},
    {GUIIM_M_TP_SLOVENIAN_UPPER,       IM_TXT_IM_SHORT_SLOVENIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_slovenian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SLOVENIAN_ABC_LEAD_UPPER,  IM_TXT_IM_SHORT_SLOVENIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SLOVENIAN_ABC_LOWER,       IM_TXT_IM_SHORT_SLOVENIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SLOVENIAN_ABC_UPPER,       IM_TXT_IM_SHORT_SLOVENIAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_ROMANIAN_SUPPORT
static MMIIM_IM_NAME_T const s_romanian_im_name_list[] =
{
    {GUIIM_M_ROMANIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ROMANIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_ROMANIAN_LOWER,        IM_TXT_IM_SHORT_ROMANIAN_LOWER, FALSE},
    {GUIIM_M_ROMANIAN_UPPER,        IM_TXT_IM_SHORT_ROMANIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_romanian_abc_im_name_list[] =
{
    {GUIIM_M_ROMANIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ROMANIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ROMANIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ROMANIAN_ABC_LOWER, FALSE},
    {GUIIM_M_ROMANIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ROMANIAN_ABC_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_romanian_im_tp_name_list[] =
{
    {GUIIM_M_TP_ROMANIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ROMANIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ROMANIAN_LOWER,        IM_TXT_IM_SHORT_ROMANIAN_LOWER, FALSE},
    {GUIIM_M_TP_ROMANIAN_UPPER,        IM_TXT_IM_SHORT_ROMANIAN_UPPER, FALSE}
};
static MMIIM_IM_NAME_T const s_romanian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ROMANIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ROMANIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ROMANIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ROMANIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ROMANIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ROMANIAN_ABC_UPPER, FALSE}
};
#endif

#ifdef IM_TELUGU_SUPPORT
static MMIIM_IM_NAME_T const s_telugu_im_name_list[] =
{
    {GUIIM_M_TELUGU,   IM_TXT_IM_SHORT_TELUGU, FALSE},
};
static MMIIM_IM_NAME_T const s_telugu_abc_im_name_list[] =
{
    {GUIIM_M_TELUGU_ABC,   IM_TXT_IM_SHORT_TELUGU_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_telugu_im_tp_name_list[] =
{
    {GUIIM_M_TP_TELUGU,   IM_TXT_IM_SHORT_TELUGU, FALSE},
};
static MMIIM_IM_NAME_T const s_telugu_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_TELUGU_ABC,   IM_TXT_IM_SHORT_TELUGU_ABC, FALSE},
};
#endif

#ifdef IM_MARATHI_SUPPORT
static MMIIM_IM_NAME_T const s_marathi_im_name_list[] =
{
    {GUIIM_M_MARATHI,   IM_TXT_IM_SHORT_MARATHI, FALSE},
};
static MMIIM_IM_NAME_T const s_marathi_abc_im_name_list[] =
{
    {GUIIM_M_MARATHI_ABC,   IM_TXT_IM_SHORT_MARATHI_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_marathi_im_tp_name_list[] =
{
    {GUIIM_M_TP_MARATHI,   IM_TXT_IM_SHORT_MARATHI, FALSE},
};
static MMIIM_IM_NAME_T const s_marathi_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_MARATHI_ABC,   IM_TXT_IM_SHORT_MARATHI_ABC, FALSE},
};
#endif

#ifdef IM_TAMIL_SUPPORT
static MMIIM_IM_NAME_T const s_tamil_im_name_list[] =
{
    {GUIIM_M_TAMIL,   IM_TXT_IM_SHORT_TAMIL, FALSE},
};
static MMIIM_IM_NAME_T const s_tamil_abc_im_name_list[] =
{
    {GUIIM_M_TAMIL_ABC,   IM_TXT_IM_SHORT_TAMIL_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_tamil_im_tp_name_list[] =
{
    {GUIIM_M_TP_TAMIL,   IM_TXT_IM_SHORT_TAMIL, FALSE},
};
static MMIIM_IM_NAME_T const s_tamil_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_TAMIL_ABC,   IM_TXT_IM_SHORT_TAMIL_ABC, FALSE},
};
#endif

#ifdef IM_GUJARATI_SUPPORT
static MMIIM_IM_NAME_T const s_gujarati_im_name_list[] =
{
    {GUIIM_M_GUJARATI,   IM_TXT_IM_SHORT_GUJARATI, FALSE},
};
static MMIIM_IM_NAME_T const s_gujarati_abc_im_name_list[] =
{
    {GUIIM_M_GUJARATI_ABC,   IM_TXT_IM_SHORT_GUJARATI_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_gujarati_im_tp_name_list[] =
{
    {GUIIM_M_TP_GUJARATI,   IM_TXT_IM_SHORT_GUJARATI, FALSE},
};
static MMIIM_IM_NAME_T const s_gujarati_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_GUJARATI_ABC,   IM_TXT_IM_SHORT_GUJARATI_ABC, FALSE},
};
#endif

#ifdef IM_KANNADA_SUPPORT
//static MMIIM_IM_NAME_T const s_kannada_im_name_list[] =
//{
//    {GUIIM_M_KANNADA,   IM_TXT_IM_SHORT_KANNADA, FALSE},
//};
static MMIIM_IM_NAME_T const s_kannada_abc_im_name_list[] =
{
    {GUIIM_M_KANNADA_ABC,   IM_TXT_IM_SHORT_KANNADA_ABC, FALSE},
};
//static MMIIM_IM_NAME_T const s_kannada_im_tp_name_list[] =
//{
//    {GUIIM_M_TP_KANNADA,   IM_TXT_IM_SHORT_KANNADA, FALSE},
//};
static MMIIM_IM_NAME_T const s_kannada_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_KANNADA_ABC,   IM_TXT_IM_SHORT_KANNADA_ABC, FALSE},
};
#endif

#ifdef IM_MALAYALAM_SUPPORT
//static MMIIM_IM_NAME_T const s_malayalam_im_name_list[] =
//{
//    {GUIIM_M_MALAYALAM,   IM_TXT_IM_SHORT_MALAYALAM, FALSE},
//};
static MMIIM_IM_NAME_T const s_malayalam_abc_im_name_list[] =
{
    {GUIIM_M_MALAYALAM_ABC,   IM_TXT_IM_SHORT_MALAYALAM_ABC, FALSE},
};
//static MMIIM_IM_NAME_T const s_malayalam_im_tp_name_list[] =
//{
//    {GUIIM_M_TP_MALAYALAM,   IM_TXT_IM_SHORT_MALAYALAM, FALSE},
//};
static MMIIM_IM_NAME_T const s_malayalam_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_MALAYALAM_ABC,   IM_TXT_IM_SHORT_MALAYALAM_ABC, FALSE},
};
#endif

//缅甸
#ifdef IM_MYANMAR_SUPPORT
#if defined IM_ENGINE_SOGOU     //sogou存在智能输入法
static MMIIM_IM_NAME_T const s_myanmar_im_name_list[] =
{
    {GUIIM_M_MYANMAR,   IM_TXT_IM_SHORT_MYANMAR, FALSE},
};
static MMIIM_IM_NAME_T const s_myanmar_im_tp_name_list[] =
{
    {GUIIM_M_TP_MYANMAR,   IM_TXT_IM_SHORT_MYANMAR, FALSE},
};
#endif
static MMIIM_IM_NAME_T const s_myanmar_abc_im_name_list[] =
{
    {GUIIM_M_MYANMAR_ABC,   IM_TXT_IM_SHORT_MYANMAR_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_myanmar_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_MYANMAR_ABC,   IM_TXT_IM_SHORT_MYANMAR_ABC, FALSE},
};
#endif

#ifdef IM_ORIYA_SUPPORT
//static MMIIM_IM_NAME_T const s_oriya_im_name_list[] =
//{
//    {GUIIM_M_ORIYA,   IM_TXT_IM_SHORT_ORIYA, FALSE},
//};

//奥里亚multitap相关输入
static MMIIM_IM_NAME_T const s_oriya_abc_im_name_list[] =
{
    {GUIIM_M_ORIYA_ABC,   IM_TXT_IM_SHORT_ORIYA_ABC, FALSE},
};
//static MMIIM_IM_NAME_T const s_oriya_im_tp_name_list[] =
//{
//    {GUIIM_M_TP_ORIYA,   IM_TXT_IM_SHORT_ORIYA, FALSE},
//};
//奥里亚multitap相关输入
static MMIIM_IM_NAME_T const s_oriya_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ORIYA_ABC,   IM_TXT_IM_SHORT_ORIYA_ABC, FALSE},
};
#endif

#ifdef IM_PUNJABI_SUPPORT
static MMIIM_IM_NAME_T const s_punjabi_im_name_list[] =
{
    {GUIIM_M_PUNJABI,   IM_TXT_IM_SHORT_PUNJABI, FALSE},
};

//旁遮普语物理键multitap相关输入
static MMIIM_IM_NAME_T const s_punjabi_abc_im_name_list[] =
{
    {GUIIM_M_PUNJABI_ABC,   IM_TXT_IM_SHORT_PUNJABI_ABC, FALSE},
};

static MMIIM_IM_NAME_T const s_punjabi_im_tp_name_list[] =
{
    {GUIIM_M_TP_PUNJABI,   IM_TXT_IM_SHORT_PUNJABI, FALSE},
};

//旁遮普语虚拟键multitap相关输入
static MMIIM_IM_NAME_T const s_punjabi_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_PUNJABI_ABC,   IM_TXT_IM_SHORT_PUNJABI_ABC, FALSE},
};

#endif

#ifdef IM_AFRIKAANS_SUPPORT
static MMIIM_IM_NAME_T const s_afrikaans_im_name_list[] =
{
    {GUIIM_M_AFRIKAANS_LEAD_UPPER,   IM_TXT_IM_SHORT_AFRIKAANS_LEAD_UPPER, FALSE},
    {GUIIM_M_AFRIKAANS_LOWER,        IM_TXT_IM_SHORT_AFRIKAANS_LOWER, FALSE},
    {GUIIM_M_AFRIKAANS_UPPER,        IM_TXT_IM_SHORT_AFRIKAANS_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_afrikaans_im_tp_name_list[] =
{
    {GUIIM_M_TP_AFRIKAANS_LEAD_UPPER,   IM_TXT_IM_SHORT_AFRIKAANS_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_AFRIKAANS_LOWER,        IM_TXT_IM_SHORT_AFRIKAANS_LOWER, FALSE},
    {GUIIM_M_TP_AFRIKAANS_UPPER,        IM_TXT_IM_SHORT_AFRIKAANS_UPPER, FALSE},
};

//multitap	
static MMIIM_IM_NAME_T const s_afrikaans_abc_im_name_list[] =
{
    {GUIIM_M_AFRIKAANS_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_AFRIKAANS_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_AFRIKAANS_ABC_LOWER,        IM_TXT_IM_SHORT_AFRIKAANS_ABC_LOWER, FALSE},
    {GUIIM_M_AFRIKAANS_ABC_UPPER,        IM_TXT_IM_SHORT_AFRIKAANS_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_afrikaans_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_AFRIKAANS_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_AFRIKAANS_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_AFRIKAANS_ABC_LOWER,        IM_TXT_IM_SHORT_AFRIKAANS_ABC_LOWER, FALSE},
    {GUIIM_M_TP_AFRIKAANS_ABC_UPPER,        IM_TXT_IM_SHORT_AFRIKAANS_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_ALBANIAN_SUPPORT
static MMIIM_IM_NAME_T const s_albanian_im_name_list[] =
{
    {GUIIM_M_ALBANIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ALBANIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_ALBANIAN_LOWER,        IM_TXT_IM_SHORT_ALBANIAN_LOWER, FALSE},
    {GUIIM_M_ALBANIAN_UPPER,        IM_TXT_IM_SHORT_ALBANIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_albanian_im_tp_name_list[] =
{
    {GUIIM_M_TP_ALBANIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ALBANIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ALBANIAN_LOWER,        IM_TXT_IM_SHORT_ALBANIAN_LOWER, FALSE},
    {GUIIM_M_TP_ALBANIAN_UPPER,        IM_TXT_IM_SHORT_ALBANIAN_UPPER, FALSE},
};

//multitap	
static MMIIM_IM_NAME_T const s_albanian_abc_im_name_list[] =
{
    {GUIIM_M_ALBANIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ALBANIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ALBANIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ALBANIAN_ABC_LOWER, FALSE},
    {GUIIM_M_ALBANIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ALBANIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_albanian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ALBANIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ALBANIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ALBANIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ALBANIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ALBANIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ALBANIAN_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_ARMENIAN_SUPPORT
static MMIIM_IM_NAME_T const s_armenian_im_name_list[] =
{
    {GUIIM_M_ARMENIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ARMENIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_ARMENIAN_LOWER,        IM_TXT_IM_SHORT_ARMENIAN_LOWER, FALSE},
    {GUIIM_M_ARMENIAN_UPPER,        IM_TXT_IM_SHORT_ARMENIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_armenian_im_tp_name_list[] =
{
    {GUIIM_M_TP_ARMENIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ARMENIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ARMENIAN_LOWER,        IM_TXT_IM_SHORT_ARMENIAN_LOWER, FALSE},
    {GUIIM_M_TP_ARMENIAN_UPPER,        IM_TXT_IM_SHORT_ARMENIAN_UPPER, FALSE},
};

// 亚美尼亚语
static MMIIM_IM_NAME_T const s_armenian_abc_im_name_list[] =
{
    {GUIIM_M_ARMENIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ARMENIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ARMENIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ARMENIAN_ABC_LOWER, FALSE},
    {GUIIM_M_ARMENIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ARMENIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_armenian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ARMENIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ARMENIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ARMENIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ARMENIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ARMENIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ARMENIAN_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_AZERBAIJANI_SUPPORT
static MMIIM_IM_NAME_T const s_azerbaijani_im_name_list[] =
{
    {GUIIM_M_AZERBAIJANI_LEAD_UPPER,   IM_TXT_IM_SHORT_AZERBAIJANI_LEAD_UPPER, FALSE},
    {GUIIM_M_AZERBAIJANI_LOWER,        IM_TXT_IM_SHORT_AZERBAIJANI_LOWER, FALSE},
    {GUIIM_M_AZERBAIJANI_UPPER,        IM_TXT_IM_SHORT_AZERBAIJANI_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_azerbaijani_im_tp_name_list[] =
{
    {GUIIM_M_TP_AZERBAIJANI_LEAD_UPPER,   IM_TXT_IM_SHORT_AZERBAIJANI_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_AZERBAIJANI_LOWER,        IM_TXT_IM_SHORT_AZERBAIJANI_LOWER, FALSE},
    {GUIIM_M_TP_AZERBAIJANI_UPPER,        IM_TXT_IM_SHORT_AZERBAIJANI_UPPER, FALSE},
};

// 阿塞拜疆语
static MMIIM_IM_NAME_T const s_azerbaijani_abc_im_name_list[] =
{
    {GUIIM_M_AZERBAIJANI_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_AZERBAIJANI_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_AZERBAIJANI_ABC_LOWER,        IM_TXT_IM_SHORT_AZERBAIJANI_ABC_LOWER, FALSE},
    {GUIIM_M_AZERBAIJANI_ABC_UPPER,        IM_TXT_IM_SHORT_AZERBAIJANI_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_azerbaijani_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_AZERBAIJANI_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_AZERBAIJANI_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_AZERBAIJANI_ABC_LOWER,        IM_TXT_IM_SHORT_AZERBAIJANI_ABC_LOWER, FALSE},
    {GUIIM_M_TP_AZERBAIJANI_ABC_UPPER,        IM_TXT_IM_SHORT_AZERBAIJANI_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_BASQUE_SUPPORT
static MMIIM_IM_NAME_T const s_basque_im_name_list[] =
{
    {GUIIM_M_BASQUE_LEAD_UPPER,   IM_TXT_IM_SHORT_BASQUE_LEAD_UPPER, FALSE},
    {GUIIM_M_BASQUE_LOWER,        IM_TXT_IM_SHORT_BASQUE_LOWER, FALSE},
    {GUIIM_M_BASQUE_UPPER,        IM_TXT_IM_SHORT_BASQUE_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_basque_im_tp_name_list[] =
{
    {GUIIM_M_TP_BASQUE_LEAD_UPPER,   IM_TXT_IM_SHORT_BASQUE_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_BASQUE_LOWER,        IM_TXT_IM_SHORT_BASQUE_LOWER, FALSE},
    {GUIIM_M_TP_BASQUE_UPPER,        IM_TXT_IM_SHORT_BASQUE_UPPER, FALSE},
};

// 巴斯克multitap	
static MMIIM_IM_NAME_T const s_basque_abc_im_name_list[] =
{
    {GUIIM_M_BASQUE_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_BASQUE_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_BASQUE_ABC_LOWER,        IM_TXT_IM_SHORT_BASQUE_ABC_LOWER, FALSE},
    {GUIIM_M_BASQUE_ABC_UPPER,        IM_TXT_IM_SHORT_BASQUE_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_basque_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_BASQUE_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_BASQUE_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_BASQUE_ABC_LOWER,        IM_TXT_IM_SHORT_BASQUE_ABC_LOWER, FALSE},
    {GUIIM_M_TP_BASQUE_ABC_UPPER,        IM_TXT_IM_SHORT_BASQUE_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_BULGARIAN_SUPPORT
static MMIIM_IM_NAME_T const s_bulgarian_im_name_list[] =
{
    {GUIIM_M_BULGARIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_BULGARIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_BULGARIAN_LOWER,        IM_TXT_IM_SHORT_BULGARIAN_LOWER, FALSE},
    {GUIIM_M_BULGARIAN_UPPER,        IM_TXT_IM_SHORT_BULGARIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_bulgarian_im_tp_name_list[] =
{
    {GUIIM_M_TP_BULGARIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_BULGARIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_BULGARIAN_LOWER,        IM_TXT_IM_SHORT_BULGARIAN_LOWER, FALSE},
    {GUIIM_M_TP_BULGARIAN_UPPER,        IM_TXT_IM_SHORT_BULGARIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_bulgarian_abc_im_name_list[] =
{
    {GUIIM_M_BULGARIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_BULGARIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_BULGARIAN_ABC_LOWER,        IM_TXT_IM_SHORT_BULGARIAN_ABC_LOWER, FALSE},
    {GUIIM_M_BULGARIAN_ABC_UPPER,        IM_TXT_IM_SHORT_BULGARIAN_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_bulgarian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_BULGARIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_BULGARIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_BULGARIAN_ABC_LOWER,        IM_TXT_IM_SHORT_BULGARIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_BULGARIAN_ABC_UPPER,        IM_TXT_IM_SHORT_BULGARIAN_ABC_UPPER, FALSE},

};
#endif

#ifdef IM_CATALAN_SUPPORT
static MMIIM_IM_NAME_T const s_catalan_im_name_list[] =
{
    {GUIIM_M_CATALAN_LEAD_UPPER,   IM_TXT_IM_SHORT_CATALAN_LEAD_UPPER, FALSE},
    {GUIIM_M_CATALAN_LOWER,        IM_TXT_IM_SHORT_CATALAN_LOWER, FALSE},
    {GUIIM_M_CATALAN_UPPER,        IM_TXT_IM_SHORT_CATALAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_catalan_im_tp_name_list[] =
{
    {GUIIM_M_TP_CATALAN_LEAD_UPPER,   IM_TXT_IM_SHORT_CATALAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_CATALAN_LOWER,        IM_TXT_IM_SHORT_CATALAN_LOWER, FALSE},
    {GUIIM_M_TP_CATALAN_UPPER,        IM_TXT_IM_SHORT_CATALAN_UPPER, FALSE},
};

// 加泰罗尼亚语multitap	
static MMIIM_IM_NAME_T const s_catalan_abc_im_name_list[] =
{
    {GUIIM_M_CATALAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_CATALAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_CATALAN_ABC_LOWER,        IM_TXT_IM_SHORT_CATALAN_ABC_LOWER, FALSE},
    {GUIIM_M_CATALAN_ABC_UPPER,        IM_TXT_IM_SHORT_CATALAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_catalan_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_CATALAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_CATALAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_CATALAN_ABC_LOWER,        IM_TXT_IM_SHORT_CATALAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_CATALAN_ABC_UPPER,        IM_TXT_IM_SHORT_CATALAN_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_CROATIAN_SUPPORT
static MMIIM_IM_NAME_T const s_croatian_im_name_list[] =
{
    {GUIIM_M_CROATIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_CROATIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_CROATIAN_LOWER,        IM_TXT_IM_SHORT_CROATIAN_LOWER, FALSE},
    {GUIIM_M_CROATIAN_UPPER,        IM_TXT_IM_SHORT_CROATIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_croatian_im_tp_name_list[] =
{
    {GUIIM_M_TP_CROATIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_CROATIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_CROATIAN_LOWER,        IM_TXT_IM_SHORT_CROATIAN_LOWER, FALSE},
    {GUIIM_M_TP_CROATIAN_UPPER,        IM_TXT_IM_SHORT_CROATIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_croatian_abc_im_name_list[] =
{
    {GUIIM_M_CROATIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_CROATIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_CROATIAN_ABC_LOWER,        IM_TXT_IM_SHORT_CROATIAN_ABC_LOWER, FALSE},
    {GUIIM_M_CROATIAN_ABC_UPPER,        IM_TXT_IM_SHORT_CROATIAN_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_croatian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_CROATIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_CROATIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_CROATIAN_ABC_LOWER,        IM_TXT_IM_SHORT_CROATIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_CROATIAN_ABC_UPPER,        IM_TXT_IM_SHORT_CROATIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_DANISH_SUPPORT
static MMIIM_IM_NAME_T const s_danish_im_name_list[] =
{
    {GUIIM_M_DANISH_LEAD_UPPER,   IM_TXT_IM_SHORT_DANISH_LEAD_UPPER, FALSE},
    {GUIIM_M_DANISH_LOWER,        IM_TXT_IM_SHORT_DANISH_LOWER, FALSE},
    {GUIIM_M_DANISH_UPPER,        IM_TXT_IM_SHORT_DANISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_danish_im_tp_name_list[] =
{
    {GUIIM_M_TP_DANISH_LEAD_UPPER,   IM_TXT_IM_SHORT_DANISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_DANISH_LOWER,        IM_TXT_IM_SHORT_DANISH_LOWER, FALSE},
    {GUIIM_M_TP_DANISH_UPPER,        IM_TXT_IM_SHORT_DANISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_danish_abc_im_name_list[] =
{
    {GUIIM_M_DANISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_DANISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_DANISH_ABC_LOWER,        IM_TXT_IM_SHORT_DANISH_ABC_LOWER, FALSE},
    {GUIIM_M_DANISH_ABC_UPPER,        IM_TXT_IM_SHORT_DANISH_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_danish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_DANISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_DANISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_DANISH_ABC_LOWER,        IM_TXT_IM_SHORT_DANISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_DANISH_ABC_UPPER,        IM_TXT_IM_SHORT_DANISH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_DUTCH_SUPPORT
static MMIIM_IM_NAME_T const s_dutch_im_name_list[] =
{
    {GUIIM_M_DUTCH_LEAD_UPPER,   IM_TXT_IM_SHORT_DUTCH_LEAD_UPPER, FALSE},
    {GUIIM_M_DUTCH_LOWER,        IM_TXT_IM_SHORT_DUTCH_LOWER, FALSE},
    {GUIIM_M_DUTCH_UPPER,        IM_TXT_IM_SHORT_DUTCH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_dutch_im_tp_name_list[] =
{
    {GUIIM_M_TP_DUTCH_LEAD_UPPER,   IM_TXT_IM_SHORT_DUTCH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_DUTCH_LOWER,        IM_TXT_IM_SHORT_DUTCH_LOWER, FALSE},
    {GUIIM_M_TP_DUTCH_UPPER,        IM_TXT_IM_SHORT_DUTCH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_dutch_abc_im_name_list[] =
{
    {GUIIM_M_DUTCH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_DUTCH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_DUTCH_ABC_LOWER,        IM_TXT_IM_SHORT_DUTCH_ABC_LOWER, FALSE},
    {GUIIM_M_DUTCH_ABC_UPPER,        IM_TXT_IM_SHORT_DUTCH_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_dutch_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_DUTCH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_DUTCH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_DUTCH_ABC_LOWER,        IM_TXT_IM_SHORT_DUTCH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_DUTCH_ABC_UPPER,        IM_TXT_IM_SHORT_DUTCH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_ESTONIAN_SUPPORT
static MMIIM_IM_NAME_T const s_estonian_im_name_list[] =
{
    {GUIIM_M_ESTONIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ESTONIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_ESTONIAN_LOWER,        IM_TXT_IM_SHORT_ESTONIAN_LOWER, FALSE},
    {GUIIM_M_ESTONIAN_UPPER,        IM_TXT_IM_SHORT_ESTONIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_estonian_im_tp_name_list[] =
{
    {GUIIM_M_TP_ESTONIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_ESTONIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ESTONIAN_LOWER,        IM_TXT_IM_SHORT_ESTONIAN_LOWER, FALSE},
    {GUIIM_M_TP_ESTONIAN_UPPER,        IM_TXT_IM_SHORT_ESTONIAN_UPPER, FALSE},
};

// 爱沙尼亚语multitap	
static MMIIM_IM_NAME_T const s_estonian_abc_im_name_list[] =
{
    {GUIIM_M_ESTONIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ESTONIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ESTONIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ESTONIAN_ABC_LOWER, FALSE},
    {GUIIM_M_ESTONIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ESTONIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_estonian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ESTONIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ESTONIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ESTONIAN_ABC_LOWER,        IM_TXT_IM_SHORT_ESTONIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ESTONIAN_ABC_UPPER,        IM_TXT_IM_SHORT_ESTONIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_FILIPINO_SUPPORT
static MMIIM_IM_NAME_T const s_filipino_im_name_list[] =
{
    {GUIIM_M_FILIPINO_LEAD_UPPER,   IM_TXT_IM_SHORT_FILIPINO_LEAD_UPPER, FALSE},
    {GUIIM_M_FILIPINO_LOWER,        IM_TXT_IM_SHORT_FILIPINO_LOWER, FALSE},
    {GUIIM_M_FILIPINO_UPPER,        IM_TXT_IM_SHORT_FILIPINO_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_filipino_im_tp_name_list[] =
{
    {GUIIM_M_TP_FILIPINO_LEAD_UPPER,   IM_TXT_IM_SHORT_FILIPINO_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_FILIPINO_LOWER,        IM_TXT_IM_SHORT_FILIPINO_LOWER, FALSE},
    {GUIIM_M_TP_FILIPINO_UPPER,        IM_TXT_IM_SHORT_FILIPINO_UPPER, FALSE},
};

//菲律宾multitap	
static MMIIM_IM_NAME_T const s_filipino_abc_im_name_list[] =
{
    {GUIIM_M_FILIPINO_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_FILIPINO_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_FILIPINO_ABC_LOWER,        IM_TXT_IM_SHORT_FILIPINO_ABC_LOWER, FALSE},
    {GUIIM_M_FILIPINO_ABC_UPPER,        IM_TXT_IM_SHORT_FILIPINO_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_filipino_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_FILIPINO_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_FILIPINO_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_FILIPINO_ABC_LOWER,        IM_TXT_IM_SHORT_FILIPINO_ABC_LOWER, FALSE},
    {GUIIM_M_TP_FILIPINO_ABC_UPPER,        IM_TXT_IM_SHORT_FILIPINO_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_FINNISH_SUPPORT
static MMIIM_IM_NAME_T const s_finnish_im_name_list[] =
{
    {GUIIM_M_FINNISH_LEAD_UPPER,   IM_TXT_IM_SHORT_FINNISH_LEAD_UPPER, FALSE},
    {GUIIM_M_FINNISH_LOWER,        IM_TXT_IM_SHORT_FINNISH_LOWER, FALSE},
    {GUIIM_M_FINNISH_UPPER,        IM_TXT_IM_SHORT_FINNISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_finnish_im_tp_name_list[] =
{
    {GUIIM_M_TP_FINNISH_LEAD_UPPER,   IM_TXT_IM_SHORT_FINNISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_FINNISH_LOWER,        IM_TXT_IM_SHORT_FINNISH_LOWER, FALSE},
    {GUIIM_M_TP_FINNISH_UPPER,        IM_TXT_IM_SHORT_FINNISH_UPPER, FALSE},
};

//芬兰multitap	
static MMIIM_IM_NAME_T const s_finnish_abc_im_name_list[] =
{
    {GUIIM_M_FINNISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_FINNISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_FINNISH_ABC_LOWER,        IM_TXT_IM_SHORT_FINNISH_ABC_LOWER, FALSE},
    {GUIIM_M_FINNISH_ABC_UPPER,        IM_TXT_IM_SHORT_FINNISH_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_finnish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_FINNISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_FINNISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_FINNISH_ABC_LOWER,        IM_TXT_IM_SHORT_FINNISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_FINNISH_ABC_UPPER,        IM_TXT_IM_SHORT_FINNISH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_GALICIAN_SUPPORT
static MMIIM_IM_NAME_T const s_galician_im_name_list[] =
{
    {GUIIM_M_GALICIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_GALICIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_GALICIAN_LOWER,        IM_TXT_IM_SHORT_GALICIAN_LOWER, FALSE},
    {GUIIM_M_GALICIAN_UPPER,        IM_TXT_IM_SHORT_GALICIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_galician_im_tp_name_list[] =
{
    {GUIIM_M_TP_GALICIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_GALICIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_GALICIAN_LOWER,        IM_TXT_IM_SHORT_GALICIAN_LOWER, FALSE},
    {GUIIM_M_TP_GALICIAN_UPPER,        IM_TXT_IM_SHORT_GALICIAN_UPPER, FALSE},
};
// 加利西亚语
static MMIIM_IM_NAME_T const s_galician_abc_im_name_list[] =
{
    {GUIIM_M_GALICIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_GALICIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_GALICIAN_ABC_LOWER,        IM_TXT_IM_SHORT_GALICIAN_ABC_LOWER, FALSE},
    {GUIIM_M_GALICIAN_ABC_UPPER,        IM_TXT_IM_SHORT_GALICIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_galician_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_GALICIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_GALICIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_GALICIAN_ABC_LOWER,        IM_TXT_IM_SHORT_GALICIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_GALICIAN_ABC_UPPER,        IM_TXT_IM_SHORT_GALICIAN_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_GEORGIAN_SUPPORT
//static MMIIM_IM_NAME_T const s_georgian_im_name_list[] =
//{
//    {GUIIM_M_GEORGIAN,   IM_TXT_IM_SHORT_GEORGIAN, FALSE},
//};
//static MMIIM_IM_NAME_T const s_georgian_im_tp_name_list[] =
//{
//    {GUIIM_M_TP_GEORGIAN,   IM_TXT_IM_SHORT_GEORGIAN, FALSE},
//};

// 格鲁吉亚语multitap	
static MMIIM_IM_NAME_T const s_georgian_abc_im_name_list[] =
{
    {GUIIM_M_GEORGIAN_ABC,   IM_TXT_IM_SHORT_GEORGIAN_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_georgian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_GEORGIAN_ABC,   IM_TXT_IM_SHORT_GEORGIAN_ABC, FALSE},
};

#endif

#ifdef IM_HAUSA_SUPPORT
static MMIIM_IM_NAME_T const s_hausa_im_name_list[] =
{
    {GUIIM_M_HAUSA_LEAD_UPPER,   IM_TXT_IM_SHORT_HAUSA_LEAD_UPPER, FALSE},
    {GUIIM_M_HAUSA_LOWER,        IM_TXT_IM_SHORT_HAUSA_LOWER, FALSE},
    {GUIIM_M_HAUSA_UPPER,        IM_TXT_IM_SHORT_HAUSA_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_hausa_im_tp_name_list[] =
{
    {GUIIM_M_TP_HAUSA_LEAD_UPPER,   IM_TXT_IM_SHORT_HAUSA_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_HAUSA_LOWER,        IM_TXT_IM_SHORT_HAUSA_LOWER, FALSE},
    {GUIIM_M_TP_HAUSA_UPPER,        IM_TXT_IM_SHORT_HAUSA_UPPER, FALSE},
};

// 豪撒语multitap	
static MMIIM_IM_NAME_T const s_hausa_abc_im_name_list[] =
{
    {GUIIM_M_HAUSA_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_HAUSA_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_HAUSA_ABC_LOWER,        IM_TXT_IM_SHORT_HAUSA_ABC_LOWER, FALSE},
    {GUIIM_M_HAUSA_ABC_UPPER,        IM_TXT_IM_SHORT_HAUSA_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_hausa_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_HAUSA_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_HAUSA_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_HAUSA_ABC_LOWER,        IM_TXT_IM_SHORT_HAUSA_ABC_LOWER, FALSE},
    {GUIIM_M_TP_HAUSA_ABC_UPPER,        IM_TXT_IM_SHORT_HAUSA_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_ICELANDIC_SUPPORT
static MMIIM_IM_NAME_T const s_icelandic_im_name_list[] =
{
    {GUIIM_M_ICELANDIC_LEAD_UPPER,   IM_TXT_IM_SHORT_ICELANDIC_LEAD_UPPER, FALSE},
    {GUIIM_M_ICELANDIC_LOWER,        IM_TXT_IM_SHORT_ICELANDIC_LOWER, FALSE},
    {GUIIM_M_ICELANDIC_UPPER,        IM_TXT_IM_SHORT_ICELANDIC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_icelandic_im_tp_name_list[] =
{
    {GUIIM_M_TP_ICELANDIC_LEAD_UPPER,   IM_TXT_IM_SHORT_ICELANDIC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ICELANDIC_LOWER,        IM_TXT_IM_SHORT_ICELANDIC_LOWER, FALSE},
    {GUIIM_M_TP_ICELANDIC_UPPER,        IM_TXT_IM_SHORT_ICELANDIC_UPPER, FALSE},
};

//冰岛语multitap	
static MMIIM_IM_NAME_T const s_icelandic_abc_im_name_list[] =
{
    {GUIIM_M_ICELANDIC_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ICELANDIC_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ICELANDIC_ABC_LOWER,        IM_TXT_IM_SHORT_ICELANDIC_ABC_LOWER, FALSE},
    {GUIIM_M_ICELANDIC_ABC_UPPER,        IM_TXT_IM_SHORT_ICELANDIC_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_icelandic_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ICELANDIC_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ICELANDIC_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ICELANDIC_ABC_LOWER,        IM_TXT_IM_SHORT_ICELANDIC_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ICELANDIC_ABC_UPPER,        IM_TXT_IM_SHORT_ICELANDIC_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_IGBO_SUPPORT
static MMIIM_IM_NAME_T const s_igbo_im_name_list[] =
{
    {GUIIM_M_IGBO_LEAD_UPPER,   IM_TXT_IM_SHORT_IGBO_LEAD_UPPER, FALSE},
    {GUIIM_M_IGBO_LOWER,        IM_TXT_IM_SHORT_IGBO_LOWER, FALSE},
    {GUIIM_M_IGBO_UPPER,        IM_TXT_IM_SHORT_IGBO_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_igbo_im_tp_name_list[] =
{
    {GUIIM_M_TP_IGBO_LEAD_UPPER,   IM_TXT_IM_SHORT_IGBO_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_IGBO_LOWER,        IM_TXT_IM_SHORT_IGBO_LOWER, FALSE},
    {GUIIM_M_TP_IGBO_UPPER,        IM_TXT_IM_SHORT_IGBO_UPPER, FALSE},
};

//伊博语multitap	
static MMIIM_IM_NAME_T const s_igbo_abc_im_name_list[] =
{
    {GUIIM_M_IGBO_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_IGBO_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_IGBO_ABC_LOWER,        IM_TXT_IM_SHORT_IGBO_ABC_LOWER, FALSE},
    {GUIIM_M_IGBO_ABC_UPPER,        IM_TXT_IM_SHORT_IGBO_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_igbo_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_IGBO_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_IGBO_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_IGBO_ABC_LOWER,        IM_TXT_IM_SHORT_IGBO_ABC_LOWER, FALSE},
    {GUIIM_M_TP_IGBO_ABC_UPPER,        IM_TXT_IM_SHORT_IGBO_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_IRISH_SUPPORT
static MMIIM_IM_NAME_T const s_irish_im_name_list[] =
{
    {GUIIM_M_IRISH_LEAD_UPPER,   IM_TXT_IM_SHORT_IRISH_LEAD_UPPER, FALSE},
    {GUIIM_M_IRISH_LOWER,        IM_TXT_IM_SHORT_IRISH_LOWER, FALSE},
    {GUIIM_M_IRISH_UPPER,        IM_TXT_IM_SHORT_IRISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_irish_im_tp_name_list[] =
{
    {GUIIM_M_TP_IRISH_LEAD_UPPER,   IM_TXT_IM_SHORT_IRISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_IRISH_LOWER,        IM_TXT_IM_SHORT_IRISH_LOWER, FALSE},
    {GUIIM_M_TP_IRISH_UPPER,        IM_TXT_IM_SHORT_IRISH_UPPER, FALSE},
};

//爱尔兰语multitap	
static MMIIM_IM_NAME_T const s_irish_abc_im_name_list[] =
{
    {GUIIM_M_IRISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_IRISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_IRISH_ABC_LOWER,        IM_TXT_IM_SHORT_IRISH_ABC_LOWER, FALSE},
    {GUIIM_M_IRISH_ABC_UPPER,        IM_TXT_IM_SHORT_IRISH_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_irish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_IRISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_IRISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_IRISH_ABC_LOWER,        IM_TXT_IM_SHORT_IRISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_IRISH_ABC_UPPER,        IM_TXT_IM_SHORT_IRISH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_KAZAKH_SUPPORT
static MMIIM_IM_NAME_T const s_kazakh_im_name_list[] =
{
    {GUIIM_M_KAZAKH_LEAD_UPPER,   IM_TXT_IM_SHORT_KAZAKH_LEAD_UPPER, FALSE},
    {GUIIM_M_KAZAKH_LOWER,        IM_TXT_IM_SHORT_KAZAKH_LOWER, FALSE},
    {GUIIM_M_KAZAKH_UPPER,        IM_TXT_IM_SHORT_KAZAKH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_kazakh_im_tp_name_list[] =
{
    {GUIIM_M_TP_KAZAKH_LEAD_UPPER,   IM_TXT_IM_SHORT_KAZAKH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_KAZAKH_LOWER,        IM_TXT_IM_SHORT_KAZAKH_LOWER, FALSE},
    {GUIIM_M_TP_KAZAKH_UPPER,        IM_TXT_IM_SHORT_KAZAKH_UPPER, FALSE},
};

//哈萨克斯坦语multitap	
static MMIIM_IM_NAME_T const s_kazakh_abc_im_name_list[] =
{
    {GUIIM_M_KAZAKH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_KAZAKH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_KAZAKH_ABC_LOWER,        IM_TXT_IM_SHORT_KAZAKH_ABC_LOWER, FALSE},
    {GUIIM_M_KAZAKH_ABC_UPPER,        IM_TXT_IM_SHORT_KAZAKH_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_kazakh_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_KAZAKH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_KAZAKH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_KAZAKH_ABC_LOWER,        IM_TXT_IM_SHORT_KAZAKH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_KAZAKH_ABC_UPPER,        IM_TXT_IM_SHORT_KAZAKH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_LATVIAN_SUPPORT
static MMIIM_IM_NAME_T const s_latvian_im_name_list[] =
{
    {GUIIM_M_LATVIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_LATVIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_LATVIAN_LOWER,        IM_TXT_IM_SHORT_LATVIAN_LOWER, FALSE},
    {GUIIM_M_LATVIAN_UPPER,        IM_TXT_IM_SHORT_LATVIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_latvian_im_tp_name_list[] =
{
    {GUIIM_M_TP_LATVIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_LATVIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_LATVIAN_LOWER,        IM_TXT_IM_SHORT_LATVIAN_LOWER, FALSE},
    {GUIIM_M_TP_LATVIAN_UPPER,        IM_TXT_IM_SHORT_LATVIAN_UPPER, FALSE},
};

//拉脱维亚语multitap	
static MMIIM_IM_NAME_T const s_latvian_abc_im_name_list[] =
{
    {GUIIM_M_LATVIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_LATVIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_LATVIAN_ABC_LOWER,        IM_TXT_IM_SHORT_LATVIAN_ABC_LOWER, FALSE},
    {GUIIM_M_LATVIAN_ABC_UPPER,        IM_TXT_IM_SHORT_LATVIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_latvian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_LATVIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_LATVIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_LATVIAN_ABC_LOWER,        IM_TXT_IM_SHORT_LATVIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_LATVIAN_ABC_UPPER,        IM_TXT_IM_SHORT_LATVIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_LITHUANIAN_SUPPORT
static MMIIM_IM_NAME_T const s_lithuanian_im_name_list[] =
{
    {GUIIM_M_LITHUANIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_LITHUANIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_LITHUANIAN_LOWER,        IM_TXT_IM_SHORT_LITHUANIAN_LOWER, FALSE},
    {GUIIM_M_LITHUANIAN_UPPER,        IM_TXT_IM_SHORT_LITHUANIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_lithuanian_im_tp_name_list[] =
{
    {GUIIM_M_TP_LITHUANIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_LITHUANIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_LITHUANIAN_LOWER,        IM_TXT_IM_SHORT_LITHUANIAN_LOWER, FALSE},
    {GUIIM_M_TP_LITHUANIAN_UPPER,        IM_TXT_IM_SHORT_LITHUANIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_lithuanian_abc_im_name_list[] =
{
    {GUIIM_M_LITHUANIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_LITHUANIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_LITHUANIAN_ABC_LOWER,        IM_TXT_IM_SHORT_LITHUANIAN_ABC_LOWER, FALSE},
    {GUIIM_M_LITHUANIAN_ABC_UPPER,        IM_TXT_IM_SHORT_LITHUANIAN_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_lithuanian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_LITHUANIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_LITHUANIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_LITHUANIAN_ABC_LOWER,        IM_TXT_IM_SHORT_LITHUANIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_LITHUANIAN_ABC_UPPER,        IM_TXT_IM_SHORT_LITHUANIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_MACEDONIAN_SUPPORT
static MMIIM_IM_NAME_T const s_macedonian_im_name_list[] =
{
    {GUIIM_M_MACEDONIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_MACEDONIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_MACEDONIAN_LOWER,        IM_TXT_IM_SHORT_MACEDONIAN_LOWER, FALSE},
    {GUIIM_M_MACEDONIAN_UPPER,        IM_TXT_IM_SHORT_MACEDONIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_macedonian_im_tp_name_list[] =
{
    {GUIIM_M_TP_MACEDONIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_MACEDONIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_MACEDONIAN_LOWER,        IM_TXT_IM_SHORT_MACEDONIAN_LOWER, FALSE},
    {GUIIM_M_TP_MACEDONIAN_UPPER,        IM_TXT_IM_SHORT_MACEDONIAN_UPPER, FALSE},
};

// 马其顿语multitap	
static MMIIM_IM_NAME_T const s_macedonian_abc_im_name_list[] =
{
    {GUIIM_M_MACEDONIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_MACEDONIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_MACEDONIAN_ABC_LOWER,        IM_TXT_IM_SHORT_MACEDONIAN_ABC_LOWER, FALSE},
    {GUIIM_M_MACEDONIAN_ABC_UPPER,        IM_TXT_IM_SHORT_MACEDONIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_macedonian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_MACEDONIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_MACEDONIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_MACEDONIAN_ABC_LOWER,        IM_TXT_IM_SHORT_MACEDONIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_MACEDONIAN_ABC_UPPER,        IM_TXT_IM_SHORT_MACEDONIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_MOLDOVAN_SUPPORT
//static MMIIM_IM_NAME_T const s_moldovan_im_name_list[] =
//{
//    {GUIIM_M_MOLDOVAN_LEAD_UPPER,   IM_TXT_IM_SHORT_MOLDOVAN_LEAD_UPPER, FALSE},
//    {GUIIM_M_MOLDOVAN_LOWER,        IM_TXT_IM_SHORT_MOLDOVAN_LOWER, FALSE},
//    {GUIIM_M_MOLDOVAN_UPPER,        IM_TXT_IM_SHORT_MOLDOVAN_UPPER, FALSE},
//};
//static MMIIM_IM_NAME_T const s_moldovan_im_tp_name_list[] =
//{
//    {GUIIM_M_TP_MOLDOVAN_LEAD_UPPER,   IM_TXT_IM_SHORT_MOLDOVAN_LEAD_UPPER, FALSE},
//    {GUIIM_M_TP_MOLDOVAN_LOWER,        IM_TXT_IM_SHORT_MOLDOVAN_LOWER, FALSE},
//    {GUIIM_M_TP_MOLDOVAN_UPPER,        IM_TXT_IM_SHORT_MOLDOVAN_UPPER, FALSE},
//};

// 摩尔多瓦语
static MMIIM_IM_NAME_T const s_moldovan_abc_im_name_list[] =
{
    {GUIIM_M_MOLDOVAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_MOLDOVAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_MOLDOVAN_ABC_LOWER,        IM_TXT_IM_SHORT_MOLDOVAN_ABC_LOWER, FALSE},
    {GUIIM_M_MOLDOVAN_ABC_UPPER,        IM_TXT_IM_SHORT_MOLDOVAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_moldovan_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_MOLDOVAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_MOLDOVAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_MOLDOVAN_ABC_LOWER,        IM_TXT_IM_SHORT_MOLDOVAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_MOLDOVAN_ABC_UPPER,        IM_TXT_IM_SHORT_MOLDOVAN_ABC_UPPER, FALSE},
};

#endif

#ifdef IM_NORWEGIAN_SUPPORT
static MMIIM_IM_NAME_T const s_norwegian_im_name_list[] =
{
    {GUIIM_M_NORWEGIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_NORWEGIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_NORWEGIAN_LOWER,        IM_TXT_IM_SHORT_NORWEGIAN_LOWER, FALSE},
    {GUIIM_M_NORWEGIAN_UPPER,        IM_TXT_IM_SHORT_NORWEGIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_norwegian_im_tp_name_list[] =
{
    {GUIIM_M_TP_NORWEGIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_NORWEGIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_NORWEGIAN_LOWER,        IM_TXT_IM_SHORT_NORWEGIAN_LOWER, FALSE},
    {GUIIM_M_TP_NORWEGIAN_UPPER,        IM_TXT_IM_SHORT_NORWEGIAN_UPPER, FALSE},
};   
static MMIIM_IM_NAME_T const s_norwegian_abc_im_name_list[] =
{
    {GUIIM_M_NORWEGIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_NORWEGIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_NORWEGIAN_ABC_LOWER,        IM_TXT_IM_SHORT_NORWEGIAN_ABC_LOWER, FALSE},
    {GUIIM_M_NORWEGIAN_ABC_UPPER,        IM_TXT_IM_SHORT_NORWEGIAN_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_norwegian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_NORWEGIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_NORWEGIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_NORWEGIAN_ABC_LOWER,        IM_TXT_IM_SHORT_NORWEGIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_NORWEGIAN_ABC_UPPER,        IM_TXT_IM_SHORT_NORWEGIAN_ABC_UPPER, FALSE},
};    
#endif

#ifdef IM_POLISH_SUPPORT
static MMIIM_IM_NAME_T const s_polish_im_name_list[] =
{
    {GUIIM_M_POLISH_LEAD_UPPER,   IM_TXT_IM_SHORT_POLISH_LEAD_UPPER, FALSE},
    {GUIIM_M_POLISH_LOWER,        IM_TXT_IM_SHORT_POLISH_LOWER, FALSE},
    {GUIIM_M_POLISH_UPPER,        IM_TXT_IM_SHORT_POLISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_polish_im_tp_name_list[] =
{
    {GUIIM_M_TP_POLISH_LEAD_UPPER,   IM_TXT_IM_SHORT_POLISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_POLISH_LOWER,        IM_TXT_IM_SHORT_POLISH_LOWER, FALSE},
    {GUIIM_M_TP_POLISH_UPPER,        IM_TXT_IM_SHORT_POLISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_polish_abc_im_name_list[] =
{
    {GUIIM_M_POLISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_POLISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_POLISH_ABC_LOWER,        IM_TXT_IM_SHORT_POLISH_ABC_LOWER, FALSE},
    {GUIIM_M_POLISH_ABC_UPPER,        IM_TXT_IM_SHORT_POLISH_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_polish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_POLISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_POLISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_POLISH_ABC_LOWER,        IM_TXT_IM_SHORT_POLISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_POLISH_ABC_UPPER,        IM_TXT_IM_SHORT_POLISH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_SERBIAN_SUPPORT
static MMIIM_IM_NAME_T const s_serbian_im_name_list[] =
{
    {GUIIM_M_SERBIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_SERBIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_SERBIAN_LOWER,        IM_TXT_IM_SHORT_SERBIAN_LOWER, FALSE},
    {GUIIM_M_SERBIAN_UPPER,        IM_TXT_IM_SHORT_SERBIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_serbian_im_tp_name_list[] =
{
    {GUIIM_M_TP_SERBIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_SERBIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SERBIAN_LOWER,        IM_TXT_IM_SHORT_SERBIAN_LOWER, FALSE},
    {GUIIM_M_TP_SERBIAN_UPPER,        IM_TXT_IM_SHORT_SERBIAN_UPPER, FALSE},
};


// 塞尔维亚语multitap	
static MMIIM_IM_NAME_T const s_serbian_abc_im_name_list[] =
{
    {GUIIM_M_SERBIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SERBIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SERBIAN_ABC_LOWER,        IM_TXT_IM_SHORT_SERBIAN_ABC_LOWER, FALSE},
    {GUIIM_M_SERBIAN_ABC_UPPER,        IM_TXT_IM_SHORT_SERBIAN_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_serbian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SERBIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SERBIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SERBIAN_ABC_LOWER,        IM_TXT_IM_SHORT_SERBIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SERBIAN_ABC_UPPER,        IM_TXT_IM_SHORT_SERBIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_SESOTHO_SUPPORT
static MMIIM_IM_NAME_T const s_sesotho_im_name_list[] =
{
    {GUIIM_M_SESOTHO_LEAD_UPPER,   IM_TXT_IM_SHORT_SESOTHO_LEAD_UPPER, FALSE},
    {GUIIM_M_SESOTHO_LOWER,        IM_TXT_IM_SHORT_SESOTHO_LOWER, FALSE},
    {GUIIM_M_SESOTHO_UPPER,        IM_TXT_IM_SHORT_SESOTHO_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_sesotho_im_tp_name_list[] =
{
    {GUIIM_M_TP_SESOTHO_LEAD_UPPER,   IM_TXT_IM_SHORT_SESOTHO_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SESOTHO_LOWER,        IM_TXT_IM_SHORT_SESOTHO_LOWER, FALSE},
    {GUIIM_M_TP_SESOTHO_UPPER,        IM_TXT_IM_SHORT_SESOTHO_UPPER, FALSE},
};

// 塞索托语multitap	
static MMIIM_IM_NAME_T const s_sesotho_abc_im_name_list[] =
{
    {GUIIM_M_SESOTHO_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SESOTHO_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SESOTHO_ABC_LOWER,        IM_TXT_IM_SHORT_SESOTHO_ABC_LOWER, FALSE},
    {GUIIM_M_SESOTHO_ABC_UPPER,        IM_TXT_IM_SHORT_SESOTHO_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_sesotho_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SESOTHO_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SESOTHO_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SESOTHO_ABC_LOWER,        IM_TXT_IM_SHORT_SESOTHO_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SESOTHO_ABC_UPPER,        IM_TXT_IM_SHORT_SESOTHO_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_SLOVAK_SUPPORT
static MMIIM_IM_NAME_T const s_slovak_im_name_list[] =
{
    {GUIIM_M_SLOVAK_LEAD_UPPER,   IM_TXT_IM_SHORT_SLOVAK_LEAD_UPPER, FALSE},
    {GUIIM_M_SLOVAK_LOWER,        IM_TXT_IM_SHORT_SLOVAK_LOWER, FALSE},
    {GUIIM_M_SLOVAK_UPPER,        IM_TXT_IM_SHORT_SLOVAK_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_slovak_im_tp_name_list[] =
{
    {GUIIM_M_TP_SLOVAK_LEAD_UPPER,   IM_TXT_IM_SHORT_SLOVAK_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SLOVAK_LOWER,        IM_TXT_IM_SHORT_SLOVAK_LOWER, FALSE},
    {GUIIM_M_TP_SLOVAK_UPPER,        IM_TXT_IM_SHORT_SLOVAK_UPPER, FALSE},
}; 
static MMIIM_IM_NAME_T const s_slovak_abc_im_name_list[] =
{
    {GUIIM_M_SLOVAK_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SLOVAK_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SLOVAK_ABC_LOWER,        IM_TXT_IM_SHORT_SLOVAK_ABC_LOWER, FALSE},
    {GUIIM_M_SLOVAK_ABC_UPPER,        IM_TXT_IM_SHORT_SLOVAK_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_slovak_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SLOVAK_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SLOVAK_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SLOVAK_ABC_LOWER,        IM_TXT_IM_SHORT_SLOVAK_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SLOVAK_ABC_UPPER,        IM_TXT_IM_SHORT_SLOVAK_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_SWEDISH_SUPPORT
static MMIIM_IM_NAME_T const s_swedish_im_name_list[] =
{
    {GUIIM_M_SWEDISH_LEAD_UPPER,   IM_TXT_IM_SHORT_SWEDISH_LEAD_UPPER, FALSE},
    {GUIIM_M_SWEDISH_LOWER,        IM_TXT_IM_SHORT_SWEDISH_LOWER, FALSE},
    {GUIIM_M_SWEDISH_UPPER,        IM_TXT_IM_SHORT_SWEDISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_swedish_im_tp_name_list[] =
{
    {GUIIM_M_TP_SWEDISH_LEAD_UPPER,   IM_TXT_IM_SHORT_SWEDISH_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SWEDISH_LOWER,        IM_TXT_IM_SHORT_SWEDISH_LOWER, FALSE},
    {GUIIM_M_TP_SWEDISH_UPPER,        IM_TXT_IM_SHORT_SWEDISH_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_swedish_abc_im_name_list[] =
{
    {GUIIM_M_SWEDISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SWEDISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SWEDISH_ABC_LOWER,        IM_TXT_IM_SHORT_SWEDISH_ABC_LOWER, FALSE},
    {GUIIM_M_SWEDISH_ABC_UPPER,        IM_TXT_IM_SHORT_SWEDISH_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_swedish_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SWEDISH_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SWEDISH_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SWEDISH_ABC_LOWER,        IM_TXT_IM_SHORT_SWEDISH_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SWEDISH_ABC_UPPER,        IM_TXT_IM_SHORT_SWEDISH_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_UKRAINIAN_SUPPORT
static MMIIM_IM_NAME_T const s_ukrainian_im_name_list[] =
{
    {GUIIM_M_UKRAINIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_UKRAINIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_UKRAINIAN_LOWER,        IM_TXT_IM_SHORT_UKRAINIAN_LOWER, FALSE},
    {GUIIM_M_UKRAINIAN_UPPER,        IM_TXT_IM_SHORT_UKRAINIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_ukrainian_im_tp_name_list[] =
{
    {GUIIM_M_TP_UKRAINIAN_LEAD_UPPER,   IM_TXT_IM_SHORT_UKRAINIAN_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_UKRAINIAN_LOWER,        IM_TXT_IM_SHORT_UKRAINIAN_LOWER, FALSE},
    {GUIIM_M_TP_UKRAINIAN_UPPER,        IM_TXT_IM_SHORT_UKRAINIAN_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_ukrainian_abc_im_name_list[] =
{
    {GUIIM_M_UKRAINIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_UKRAINIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_UKRAINIAN_ABC_LOWER,        IM_TXT_IM_SHORT_UKRAINIAN_ABC_LOWER, FALSE},
    {GUIIM_M_UKRAINIAN_ABC_UPPER,        IM_TXT_IM_SHORT_UKRAINIAN_ABC_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_ukrainian_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_UKRAINIAN_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_UKRAINIAN_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_UKRAINIAN_ABC_LOWER,        IM_TXT_IM_SHORT_UKRAINIAN_ABC_LOWER, FALSE},
    {GUIIM_M_TP_UKRAINIAN_ABC_UPPER,        IM_TXT_IM_SHORT_UKRAINIAN_ABC_UPPER, FALSE},
};
#endif

#ifdef IM_YORUBA_SUPPORT
static MMIIM_IM_NAME_T const s_yoruba_im_name_list[] =
{
    {GUIIM_M_YORUBA_LEAD_UPPER,   IM_TXT_IM_SHORT_YORUBA_LEAD_UPPER, FALSE},
    {GUIIM_M_YORUBA_LOWER,        IM_TXT_IM_SHORT_YORUBA_LOWER, FALSE},
    {GUIIM_M_YORUBA_UPPER,        IM_TXT_IM_SHORT_YORUBA_UPPER, FALSE},
};
static MMIIM_IM_NAME_T const s_yoruba_im_tp_name_list[] =
{
    {GUIIM_M_TP_YORUBA_LEAD_UPPER,   IM_TXT_IM_SHORT_YORUBA_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_YORUBA_LOWER,        IM_TXT_IM_SHORT_YORUBA_LOWER, FALSE},
    {GUIIM_M_TP_YORUBA_UPPER,        IM_TXT_IM_SHORT_YORUBA_UPPER, FALSE},
};

// 约鲁巴语multitap	
static MMIIM_IM_NAME_T const s_yoruba_abc_im_name_list[] =
{
    {GUIIM_M_YORUBA_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_YORUBA_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_YORUBA_ABC_LOWER,        IM_TXT_IM_SHORT_YORUBA_ABC_LOWER, FALSE},
    {GUIIM_M_YORUBA_ABC_UPPER,        IM_TXT_IM_SHORT_YORUBA_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_yoruba_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_YORUBA_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_YORUBA_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_YORUBA_ABC_LOWER,        IM_TXT_IM_SHORT_YORUBA_ABC_LOWER, FALSE},
    {GUIIM_M_TP_YORUBA_ABC_UPPER,        IM_TXT_IM_SHORT_YORUBA_ABC_UPPER, FALSE},
};
#endif

//科萨语multitap	
#ifdef IM_XHOSA_SUPPORT

static MMIIM_IM_NAME_T const s_xhosa_im_name_list[] =
{
    {GUIIM_M_XHOSA_LEAD_UPPER,   IM_TXT_IM_SHORT_XHOSA_LEAD_UPPER, FALSE},
    {GUIIM_M_XHOSA_LOWER,        IM_TXT_IM_SHORT_XHOSA_LOWER, FALSE},
    {GUIIM_M_XHOSA_UPPER,        IM_TXT_IM_SHORT_XHOSA_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_xhosa_im_tp_name_list[] =
{
    {GUIIM_M_TP_XHOSA_LEAD_UPPER,   IM_TXT_IM_SHORT_XHOSA_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_XHOSA_LOWER,        IM_TXT_IM_SHORT_XHOSA_LOWER, FALSE},
    {GUIIM_M_TP_XHOSA_UPPER,        IM_TXT_IM_SHORT_XHOSA_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_xhosa_abc_im_name_list[] =
{
    {GUIIM_M_XHOSA_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_XHOSA_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_XHOSA_ABC_LOWER,        IM_TXT_IM_SHORT_XHOSA_ABC_LOWER, FALSE},
    {GUIIM_M_XHOSA_ABC_UPPER,        IM_TXT_IM_SHORT_XHOSA_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_xhosa_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_XHOSA_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_XHOSA_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_XHOSA_ABC_LOWER,        IM_TXT_IM_SHORT_XHOSA_ABC_LOWER, FALSE},
    {GUIIM_M_TP_XHOSA_ABC_UPPER,        IM_TXT_IM_SHORT_XHOSA_ABC_UPPER, FALSE},
};
#endif

//祖鲁语multitap	
#ifdef IM_ZULU_SUPPORT
static MMIIM_IM_NAME_T const s_zulu_im_name_list[] =
{
    {GUIIM_M_ZULU_LEAD_UPPER,   IM_TXT_IM_SHORT_ZULU_LEAD_UPPER, FALSE},
    {GUIIM_M_ZULU_LOWER,        IM_TXT_IM_SHORT_ZULU_LOWER, FALSE},
    {GUIIM_M_ZULU_UPPER,        IM_TXT_IM_SHORT_ZULU_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_zulu_im_tp_name_list[] =
{
    {GUIIM_M_TP_ZULU_LEAD_UPPER,   IM_TXT_IM_SHORT_ZULU_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ZULU_LOWER,        IM_TXT_IM_SHORT_ZULU_LOWER, FALSE},
    {GUIIM_M_TP_ZULU_UPPER,        IM_TXT_IM_SHORT_ZULU_UPPER, FALSE},
};


static MMIIM_IM_NAME_T const s_zulu_abc_im_name_list[] =
{
    {GUIIM_M_ZULU_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ZULU_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_ZULU_ABC_LOWER,        IM_TXT_IM_SHORT_ZULU_ABC_LOWER, FALSE},
    {GUIIM_M_ZULU_ABC_UPPER,        IM_TXT_IM_SHORT_ZULU_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_zulu_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ZULU_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_ZULU_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_ZULU_ABC_LOWER,        IM_TXT_IM_SHORT_ZULU_ABC_LOWER, FALSE},
    {GUIIM_M_TP_ZULU_ABC_UPPER,        IM_TXT_IM_SHORT_ZULU_ABC_UPPER, FALSE},
};
#endif

//(印度语支的)阿萨姆语	
#ifdef IM_ASSAMESE_SUPPORT
//static MMIIM_IM_NAME_T const s_assamese_im_name_list[] =
//{
//    {GUIIM_M_ASSAMESE,   IM_TXT_IM_SHORT_ASSAMESE, FALSE},
//};
//static MMIIM_IM_NAME_T const s_assamese_im_tp_name_list[] =
//{
//    {GUIIM_M_TP_ASSAMESE,   IM_TXT_IM_SHORT_ASSAMESE, FALSE},
//};

static MMIIM_IM_NAME_T const s_assamese_abc_im_name_list[] =
{
    {GUIIM_M_ASSAMESE_ABC,   IM_TXT_IM_SHORT_ASSAMESE_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_assamese_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_ASSAMESE_ABC,   IM_TXT_IM_SHORT_ASSAMESE_ABC, FALSE},
};
#endif

//斯瓦西里语	
#ifdef IM_SWAHILI_SUPPORT
static MMIIM_IM_NAME_T const s_swahili_im_name_list[] =
{
    {GUIIM_M_SWAHILI_LEAD_UPPER,   IM_TXT_IM_SHORT_SWAHILI_LEAD_UPPER, FALSE},
    {GUIIM_M_SWAHILI_LOWER,        IM_TXT_IM_SHORT_SWAHILI_LOWER, FALSE},
    {GUIIM_M_SWAHILI_UPPER,        IM_TXT_IM_SHORT_SWAHILI_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_swahili_im_tp_name_list[] =
{
    {GUIIM_M_TP_SWAHILI_LEAD_UPPER,   IM_TXT_IM_SHORT_SWAHILI_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SWAHILI_LOWER,        IM_TXT_IM_SHORT_SWAHILI_LOWER, FALSE},
    {GUIIM_M_TP_SWAHILI_UPPER,        IM_TXT_IM_SHORT_SWAHILI_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_swahili_abc_im_name_list[] =
{
    {GUIIM_M_SWAHILI_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SWAHILI_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_SWAHILI_ABC_LOWER,        IM_TXT_IM_SHORT_SWAHILI_ABC_LOWER, FALSE},
    {GUIIM_M_SWAHILI_ABC_UPPER,        IM_TXT_IM_SHORT_SWAHILI_ABC_UPPER, FALSE},
};

static MMIIM_IM_NAME_T const s_swahili_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SWAHILI_ABC_LEAD_UPPER,   IM_TXT_IM_SHORT_SWAHILI_ABC_LEAD_UPPER, FALSE},
    {GUIIM_M_TP_SWAHILI_ABC_LOWER,        IM_TXT_IM_SHORT_SWAHILI_ABC_LOWER, FALSE},
    {GUIIM_M_TP_SWAHILI_ABC_UPPER,        IM_TXT_IM_SHORT_SWAHILI_ABC_UPPER, FALSE},
};
#endif

//阿姆哈拉
#ifdef IM_AMHARIC_SUPPORT
static MMIIM_IM_NAME_T const s_amharic_abc_im_name_list[] =
{
    {GUIIM_M_AMHARIC_ABC,   IM_TXT_IM_SHORT_AMHARIC_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_amharic_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_AMHARIC_ABC,   IM_TXT_IM_SHORT_AMHARIC_ABC, FALSE},
};
#endif

#ifdef IM_KHMER_SUPPORT
static MMIIM_IM_NAME_T const s_khmer_im_tp_name_list[] =
{
    {GUIIM_M_KHMER,   IM_TXT_IM_SHORT_KHMER, FALSE},
};
static MMIIM_IM_NAME_T const s_khmer_im_name_list[] =
{
    {GUIIM_M_TP_KHMER,   IM_TXT_IM_SHORT_KHMER, FALSE},
};

static MMIIM_IM_NAME_T const s_khmer_abc_im_name_list[] =
{
    {GUIIM_M_KHMER_ABC,   IM_TXT_IM_SHORT_KHMER_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_khmer_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_KHMER_ABC,   IM_TXT_IM_SHORT_KHMER_ABC, FALSE},
};
#endif

#ifdef IM_LAO_SUPPORT
static MMIIM_IM_NAME_T const s_lao_im_name_list[] =
{
    {GUIIM_M_LAO,   IM_TXT_IM_SHORT_LAO, FALSE},
};

static MMIIM_IM_NAME_T const s_lao_im_tp_name_list[] =
{
    {GUIIM_M_TP_LAO,   IM_TXT_IM_SHORT_LAO, FALSE},
};

static MMIIM_IM_NAME_T const s_lao_abc_im_name_list[] =
{
    {GUIIM_M_LAO_ABC,   IM_TXT_IM_SHORT_LAO_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_lao_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_LAO_ABC,   IM_TXT_IM_SHORT_LAO_ABC, FALSE},
};
#endif

#ifdef IM_UYGHUR_SUPPORT
static MMIIM_IM_NAME_T const s_uyghur_im_name_list[] =
{
    {GUIIM_M_UYGHUR,   IM_TXT_IM_SHORT_UYGHUR, FALSE},
};

static MMIIM_IM_NAME_T const s_uyghur_im_tp_name_list[] =
{
    {GUIIM_M_TP_UYGHUR,   IM_TXT_IM_SHORT_UYGHUR, FALSE},
};

static MMIIM_IM_NAME_T const s_uyghur_abc_im_name_list[] =
{
    {GUIIM_M_UYGHUR_ABC,   IM_TXT_IM_SHORT_UYGHUR_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_uyghur_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_UYGHUR_ABC,   IM_TXT_IM_SHORT_UYGHUR_ABC, FALSE},
};
#endif

#ifdef IM_TIBETAN_SUPPORT  //tibetan 2012 0529
#if 0
static MMIIM_IM_NAME_T const s_tibetan_im_name_list[] =
{
	{GUIIM_M_TIBETAN, IM_TXT_IM_SHORT_TIBETAN, FALSE}
};
static MMIIM_IM_NAME_T const s_tibetan_im_tp_name_list[] =
{
	{GUIIM_M_TIBETAN, IM_TXT_IM_SHORT_TIBETAN, FALSE}
};
#endif

static MMIIM_IM_NAME_T const s_tibetan_abc_im_name_list[] =
{
	{GUIIM_M_TIBETAN_ABC, IM_TXT_IM_SHORT_TIBETAN_ABC, FALSE}
};
static MMIIM_IM_NAME_T const s_tibetan_abc_im_tp_name_list[] =
{
	{GUIIM_M_TP_TIBETAN_ABC, IM_TXT_IM_SHORT_TIBETAN_ABC, FALSE}
};

#endif

#ifdef IM_SINHALESE_SUPPORT
static MMIIM_IM_NAME_T const s_sinhalese_im_name_list[] =
{
    {GUIIM_M_SINHALESE,   IM_TXT_IM_SHORT_SINHALESE, FALSE},
};

static MMIIM_IM_NAME_T const s_sinhalese_im_tp_name_list[] =
{
    {GUIIM_M_TP_SINHALESE,   IM_TXT_IM_SHORT_SINHALESE, FALSE},
};

static MMIIM_IM_NAME_T const s_sinhalese_abc_im_name_list[] =
{
    {GUIIM_M_SINHALESE_ABC,   IM_TXT_IM_SHORT_SINHALESE_ABC, FALSE},
};
static MMIIM_IM_NAME_T const s_sinhalese_abc_im_tp_name_list[] =
{
    {GUIIM_M_TP_SINHALESE_ABC,   IM_TXT_IM_SHORT_SINHALESE_ABC, FALSE},
};
#endif


//这个配置表，表示设置系统语言中的语言系，与输入法中语言序的对应表
//此结构其实完可以不要的，只要输入法语言顺序与设置中系统语言的顺序一致的话
//这里做一个中间适配对应关系表
static uint16 s_SetLang_ImLang_list[][2] =
{
    {MMISET_LANGUAGE_ENGLISH,        GUIIM_LANG_ENGLISH},
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_SIMP_CHINESE,   GUIIM_LANG_CHS},
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_TRAD_CHINESE,   GUIIM_LANG_CHT},
#endif
#ifdef IM_ARABIC_SUPPORT
    {MMISET_LANGUAGE_ARABIC,         GUIIM_LANG_ARABIC},
#endif
#ifdef IM_FRENCH_SUPPORT
    {MMISET_LANGUAGE_FRENCH,         GUIIM_LANG_FRENCH},
#endif
#ifdef IM_HINDI_SUPPORT
    {MMISET_LANGUAGE_HINDI,          GUIIM_LANG_HINDI},
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {MMISET_LANGUAGE_HUNGARIAN,      GUIIM_LANG_HUNGARIAN},
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {MMISET_LANGUAGE_INDONESIAN,     GUIIM_LANG_INDONESIAN},
#endif
#ifdef IM_MALAY_SUPPORT
    {MMISET_LANGUAGE_MALAY,          GUIIM_LANG_MALAY},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {MMISET_LANGUAGE_PORTUGUESE,     GUIIM_LANG_PORTUGUESE},
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {MMISET_LANGUAGE_RUSSIAN,        GUIIM_LANG_RUSSIAN},
#endif
#ifdef IM_SPANISH_SUPPORT
    {MMISET_LANGUAGE_SPANISH,        GUIIM_LANG_SPANISH},
#endif
#ifdef IM_TAGALOG_SUPPORT
    {MMISET_LANGUAGE_TAGALOG,        GUIIM_LANG_TAGALOG},
#endif
#ifdef IM_THAI_SUPPORT
    {MMISET_LANGUAGE_THAI,           GUIIM_LANG_THAI},
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {MMISET_LANGUAGE_VIETNAMESE,     GUIIM_LANG_VIETNAMESE},
#endif
#ifdef IM_URDU_SUPPORT
    {MMISET_LANGUAGE_URDU,           GUIIM_LANG_URDU},
#endif
#ifdef IM_ITALIAN_SUPPORT
    {MMISET_LANGUAGE_ITALIAN,        GUIIM_LANG_ITALIAN},
#endif
#ifdef IM_PERSIAN_SUPPORT
    {MMISET_LANGUAGE_PERSIAN,        GUIIM_LANG_PERSIAN},
#endif
#ifdef IM_TURKISH_SUPPORT
    {MMISET_LANGUAGE_TURKISH,        GUIIM_LANG_TURKISH},
#endif
#ifdef IM_GERMAN_SUPPORT
    {MMISET_LANGUAGE_GERMAN,         GUIIM_LANG_GERMAN},
#endif
#ifdef IM_GREEK_SUPPORT
    {MMISET_LANGUAGE_GREEK,          GUIIM_LANG_GREEK},
#endif
#ifdef IM_HEBREW_SUPPORT
    {MMISET_LANGUAGE_HEBREW,         GUIIM_LANG_HEBREW},
#endif
#ifdef IM_BENGALI_SUPPORT
    {MMISET_LANGUAGE_BENGALI,        GUIIM_LANG_BENGALI},
#endif
#ifdef IM_CZECH_SUPPORT
    {MMISET_LANGUAGE_CZECH,          GUIIM_LANG_CZECH},
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    {MMISET_LANGUAGE_SLOVENIAN,      GUIIM_LANG_SLOVENIAN},
#endif
#ifdef IM_ROMANIAN_SUPPORT
    {MMISET_LANGUAGE_ROMANIAN,       GUIIM_LANG_ROMANIAN},
#endif
#ifdef IM_TELUGU_SUPPORT
    {MMISET_LANGUAGE_TELUGU,         GUIIM_LANG_TELUGU},
#endif
#ifdef IM_MARATHI_SUPPORT
    {MMISET_LANGUAGE_MARATHI,        GUIIM_LANG_MARATHI},
#endif
#ifdef IM_TAMIL_SUPPORT
    {MMISET_LANGUAGE_TAMIL,          GUIIM_LANG_TAMIL},
#endif
#ifdef IM_GUJARATI_SUPPORT
    {MMISET_LANGUAGE_GUJARATI,       GUIIM_LANG_GUJARATI},
#endif
#ifdef IM_KANNADA_SUPPORT
    {MMISET_LANGUAGE_KANNADA,        GUIIM_LANG_KANNADA},
#endif
#ifdef IM_MALAYALAM_SUPPORT
    {MMISET_LANGUAGE_MALAYALAM,      GUIIM_LANG_MALAYALAM},
#endif
#ifdef IM_ORIYA_SUPPORT
    {MMISET_LANGUAGE_ORIYA,          GUIIM_LANG_ORIYA},
#endif
#ifdef IM_PUNJABI_SUPPORT
    {MMISET_LANGUAGE_PUNJABI,        GUIIM_LANG_PUNJABI},
#endif
#ifdef IM_AFRIKAANS_SUPPORT
    {MMISET_LANGUAGE_AFRIKAANS,      GUIIM_LANG_AFRIKAANS},
#endif
#ifdef IM_ALBANIAN_SUPPORT
    {MMISET_LANGUAGE_ALBANIAN,       GUIIM_LANG_ALBANIAN},
#endif
#ifdef IM_ARMENIAN_SUPPORT
    {MMISET_LANGUAGE_ARMENIAN,       GUIIM_LANG_ARMENIAN},
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
    {MMISET_LANGUAGE_AZERBAIJANI,    GUIIM_LANG_AZERBAIJANI},
#endif
#ifdef IM_BASQUE_SUPPORT
    {MMISET_LANGUAGE_BASQUE,         GUIIM_LANG_BASQUE},
#endif
#ifdef IM_BULGARIAN_SUPPORT
    {MMISET_LANGUAGE_BULGARIAN,      GUIIM_LANG_BULGARIAN},
#endif
#ifdef IM_CATALAN_SUPPORT
    {MMISET_LANGUAGE_CATALAN,        GUIIM_LANG_CATALAN},
#endif
#ifdef IM_CROATIAN_SUPPORT
    {MMISET_LANGUAGE_CROATIAN,       GUIIM_LANG_CROATIAN},
#endif
#ifdef IM_DANISH_SUPPORT
    {MMISET_LANGUAGE_DANISH,         GUIIM_LANG_DANISH},
#endif
#ifdef IM_DUTCH_SUPPORT
    {MMISET_LANGUAGE_DUTCH,          GUIIM_LANG_DUTCH},
#endif
#ifdef IM_ESTONIAN_SUPPORT
    {MMISET_LANGUAGE_ESTONIAN,       GUIIM_LANG_ESTONIAN},
#endif
#ifdef IM_FILIPINO_SUPPORT
    {MMISET_LANGUAGE_FILIPINO,       GUIIM_LANG_FILIPINO},
#endif
#ifdef IM_FINNISH_SUPPORT
    {MMISET_LANGUAGE_FINNISH,        GUIIM_LANG_FINNISH},
#endif
#ifdef IM_GALICIAN_SUPPORT
    {MMISET_LANGUAGE_GALICIAN,       GUIIM_LANG_GALICIAN},
#endif
#ifdef IM_GEORGIAN_SUPPORT
    {MMISET_LANGUAGE_GEORGIAN,       GUIIM_LANG_GEORGIAN},
#endif
#ifdef IM_HAUSA_SUPPORT
    {MMISET_LANGUAGE_HAUSA,          GUIIM_LANG_HAUSA},
#endif
#ifdef IM_ICELANDIC_SUPPORT
    {MMISET_LANGUAGE_ICELANDIC,      GUIIM_LANG_ICELANDIC},
#endif
#ifdef IM_IGBO_SUPPORT
    {MMISET_LANGUAGE_IGBO,           GUIIM_LANG_IGBO},
#endif
#ifdef IM_IRISH_SUPPORT
    {MMISET_LANGUAGE_IRISH,          GUIIM_LANG_IRISH},
#endif
#ifdef IM_KAZAKH_SUPPORT
    {MMISET_LANGUAGE_KAZAKH,         GUIIM_LANG_KAZAKH},
#endif
#ifdef IM_LATVIAN_SUPPORT
    {MMISET_LANGUAGE_LATVIAN,        GUIIM_LANG_LATVIAN},
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    {MMISET_LANGUAGE_LITHUANIAN,     GUIIM_LANG_LITHUANIAN},
#endif
#ifdef IM_MACEDONIAN_SUPPORT
    {MMISET_LANGUAGE_MACEDONIAN,     GUIIM_LANG_MACEDONIAN},
#endif
#ifdef IM_MOLDOVAN_SUPPORT
    {MMISET_LANGUAGE_MOLDOVAN,       GUIIM_LANG_MOLDOVAN},
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    {MMISET_LANGUAGE_NORWEGIAN,      GUIIM_LANG_NORWEGIAN},
#endif
#ifdef IM_POLISH_SUPPORT
    {MMISET_LANGUAGE_POLISH,         GUIIM_LANG_POLISH},
#endif
#ifdef IM_SERBIAN_SUPPORT
    {MMISET_LANGUAGE_SERBIAN,        GUIIM_LANG_SERBIAN},
#endif
#ifdef IM_SESOTHO_SUPPORT
    {MMISET_LANGUAGE_SESOTHO,        GUIIM_LANG_SESOTHO},
#endif
#ifdef IM_SLOVAK_SUPPORT
    {MMISET_LANGUAGE_SLOVAK,         GUIIM_LANG_SLOVAK},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {MMISET_LANGUAGE_SWEDISH,        GUIIM_LANG_SWEDISH},
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    {MMISET_LANGUAGE_UKRAINIAN,      GUIIM_LANG_UKRAINIAN},
#endif
#ifdef IM_YORUBA_SUPPORT
    {MMISET_LANGUAGE_YORUBA,         GUIIM_LANG_YORUBA},
#endif
#ifdef IM_XHOSA_SUPPORT
    {MMISET_LANGUAGE_XHOSA,         GUIIM_LANG_XHOSA},    //科萨语	
#endif
#ifdef IM_ZULU_SUPPORT
    {MMISET_LANGUAGE_ZULU,         GUIIM_LANG_ZULU},    //祖鲁语	
#endif
#ifdef IM_ASSAMESE_SUPPORT
    {MMISET_LANGUAGE_ASSAMESE,         GUIIM_LANG_ASSAMESE},    //(印度语支的)阿萨姆语	
#endif
#ifdef IM_SWAHILI_SUPPORT
    {MMISET_LANGUAGE_SWAHILI,         GUIIM_LANG_SWAHILI},     //斯瓦西里	
#endif
#ifdef IM_MYANMAR_SUPPORT
    {MMISET_LANGUAGE_MYANMAR,         GUIIM_LANG_MYANMAR},     //缅甸语
#endif
#ifdef IM_AMHARIC_SUPPORT
    {MMISET_LANGUAGE_AMHARIC,         GUIIM_LANG_AMHARIC},     //阿姆哈拉
#endif
#ifdef IM_KHMER_SUPPORT
    {MMISET_LANGUAGE_KHMER,         GUIIM_LANG_KHMER},     //柬埔寨
#endif
#ifdef IM_LAO_SUPPORT
    {MMISET_LANGUAGE_LAO,         GUIIM_LANG_LAO},     //老挝
#endif
#ifdef IM_UYGHUR_SUPPORT
    {MMISET_LANGUAGE_UYGHUR,         GUIIM_LANG_UYGHUR},
#endif
#ifdef IM_TIBETAN_SUPPORT   
    {MMISET_LANGUAGE_TIBETAN, 	GUIIM_LANG_TIBETAN},  
#endif
#ifdef IM_SINHALESE_SUPPORT
    {MMISET_LANGUAGE_SINHALESE, 	GUIIM_LANG_SINHALESE},      
#endif

};    


//switch input method window
WINDOW_TABLE(MMIIM_SWITCH_IM_WIN_TAB) =
{
    WIN_ID(MMIIM_SWITCH_IM_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32) HandleSwitchingIMWinMsg),
    //WIN_STYLE(WS_HAS_TRANSPARENT),
    //CREATE_POPMENU_CTRL(MMIIM_SET_INPUTMETHOD_MENU, MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID),
    //WIN_TITLE(TXT_SET_INPUT_MODE),
    //CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIIM_SET_INPUTMETHOD_CTRL_ID),

//fixed by hongjun.jia for NEWMS00216056. 
//将切换输入法框更改为全屏显示；
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(TXT_SET_INPUT_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

//set current input language
WINDOW_TABLE(MMIIM_SET_INPUT_LANG_WIN_TAB) =
{
    WIN_ID(MMIIM_SET_INPUT_LANG_WIN_ID),
    WIN_FUNC((uint32) HandleSetInputLangWindow),
    WIN_TITLE(TXT_INPUT_LANGUAGE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIIM_SET_INPUT_LANGUAGE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

/*****************************************************************************/
//  Description : to handle Switching input method
//  Global resource dependence :
//  Author:haiwu.chen
//  Note: 输入法切换窗口过程函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchingIMWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E                    recode          =   MMI_RESULT_TRUE;
    MMIIM_SWITCHING_IM_WIN_DATA_T   *win_data_ptr   =   PNULL;
    // GUIIM_LANGUAGE_T                lang            =   GUIIM_LANG_NONE;

    // MMI_MENU_ID_T                   menu_id         =   0;
    // GUI_BOTH_RECT_T                 both_rect       =   {0};
    // GUIIM_LANGUAGE_T new_lang = GUIIM_LANG_NONE;

    win_data_ptr = MMK_GetWinAddDataPtr(win_id);
    if(PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleSwitchingIMWinMsg: win_data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2221_112_2_18_2_25_7_34,(uint8*)"");
        return recode;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //创建popmenu
            CreateIMMenu(win_data_ptr);

            //初始列表所有项的内容
            //InitIMDisplayValues(win_data_ptr);

            //设置焦点项
            SetIMFocusIndex(win_data_ptr);

            //添加list所有项
            MMIIM_AppendIMListItem(MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID, win_data_ptr);
        }
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            OnIMClicked(win_id, MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID, win_data_ptr);
        }
        break;

#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_CTRL:
#else
    case MSG_APP_HASH:
#endif
        MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        GUIIM_RemoveWin(win_data_ptr->im_kernel_ptr->im_handle, MMK_ConvertIdToHandle(win_id));
        
        //delete resource
        OnIMReleased(win_data_ptr);
        
        MMK_FreeWinAddData(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 动态创建输入法菜单项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateIMMenu(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr)
{
    GUI_BOTH_RECT_T both_rect = {0};
    GUIMENU_BUTTON_INFO_T menu_button_info = {0};
    MMI_STRING_T title_str = {0};

    both_rect.h_rect = win_data_ptr->disp_rect.h_rect;
    both_rect.v_rect = win_data_ptr->disp_rect.v_rect;

    //creat dynamic menu
    GUIMENU_CreatDynamic(&win_data_ptr->disp_rect, MMIIM_SWITCH_IM_WIN_ID,
                         MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);
    GUIMENU_SetEffectivePoint(MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID, (GUIMENU_EP_E)win_data_ptr->effective_point);

    menu_button_info.is_static = FALSE;
    menu_button_info.node_id = 0;
    menu_button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;

    GUIMENU_SetButtonStyle(MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID, &menu_button_info);

    MMK_SetWinMoveStyle(MMIIM_SWITCH_IM_WIN_ID, MOVE_FORBIDDEN);
/*
 * - fixed by hongjun.jia for NEWMS00216056. 
 * - 由于小分辨率下popmenu底层强制专成secondmenu；
 *   而此处又重新设置了popmenu的显示区域，
 *   导致锁屏开锁后刷新产生问题。
 * - 为达到平台统一，故此处将“切换输入法框”更改为全屏显示。
 */
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIAPICTRL_SetBothRect(MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID, &both_rect);
    GUIMENU_SetMaxWidth(MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID,
        both_rect.h_rect.right - both_rect.h_rect.left,
        both_rect.v_rect.right - both_rect.v_rect.left);
#endif   
    MMI_GetLabelTextByLang(TXT_SET_INPUT_MODE, &title_str);
    GUIMENU_SetMenuTitle(&title_str, MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID);

    MMK_SetAtvCtrl(MMIIM_SWITCH_IM_WIN_ID, MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID);
}

/*****************************************************************************/
//  Description : 初始化时，由method取得当前焦点的索引
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetIMFocusIndex(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr)
{
    GUIIM_METHOD_CFG_T *method_cfg_ptr = PNULL;
    GUIIM_METHOD_T method = GUIIM_M_NONE;
    uint i = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_data_ptr->im_kernel_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_data_ptr->menu_table_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_data_ptr->im_kernel_ptr->method_param_ptr); /*assert verified*/

    if(PNULL == win_data_ptr ||PNULL == win_data_ptr->im_kernel_ptr||PNULL == win_data_ptr->menu_table_ptr||PNULL == win_data_ptr->im_kernel_ptr->method_param_ptr)
    {
        return;
    }
    //焦点区初始化默认为第一项
    win_data_ptr->item_hot = 0;

    //输入核的method值，并设置当前焦点项索引
    for (i = 0; i < win_data_ptr->item_counts; i++)
    {
        method = win_data_ptr->im_kernel_ptr->method_param_ptr->im_set.method;
        if (method == win_data_ptr->menu_table_ptr[i].method)
        {
            //如果找到的话，就设置当前输入法为焦点
            win_data_ptr->item_hot = i;
            break;
        }

        method_cfg_ptr = MMITHEME_IMGetDefByMethod(method);
        if (PNULL == method_cfg_ptr)
            continue;

        if (GUIIM_INPUT_TYPE_TP_QWERT == method_cfg_ptr->input)
        {
            if (win_data_ptr->menu_table_ptr[i].method ==
                MMITHEME_IMGetMethodByInput(method, GUIIM_INPUT_TYPE_TOUCHPEN))
            {
                win_data_ptr->item_hot = i;
                break;
            }
        }

        if (GUIIM_INPUT_TYPE_QWERT == method_cfg_ptr->input)
        {
            /*这里有个小问题，在物理26key下，传入物理9key的输入方式去查找对应的method，主题下有个函
            会强制把GUIIM_INPUT_TYPE_KEYBOARD 改成GUIIM_INPUT_TYPE_QWERT故这时就会发生hot不成功的现象
            其它的都OK，已经涉及到26键下要找9键的问题了*/
//             if (win_data_ptr->menu_table_ptr[i].method ==
//                 MMITHEME_IMGetMethodByInput(method, GUIIM_INPUT_TYPE_KEYBOARD))
            if (method == MMITHEME_IMGetMethodByInput(
                win_data_ptr->menu_table_ptr[i].method, GUIIM_INPUT_TYPE_QWERT))
            {
                win_data_ptr->item_hot = i;
                break;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 完成设置当前输入语言窗口的创建
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIIM_SetLangCreateWin(
                                           GUIIM_INIT_KERNEL_T* im_kernel_ptr,
                                           GUI_BOTH_RECT_T disp_rect,
                                           MMIIM_POPMENU_EP_E effective_point
                                           )
{
    MMIIM_SET_INPUT_LANG_WIN_DATA_T *win_data_ptr = PNULL;    
    MMI_HANDLE_T win_handle = 0;

    if(PNULL == im_kernel_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] MMIIM_SetLangCreateWin: im_kernel_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2401_112_2_18_2_25_7_35,(uint8*)"");
        return win_handle;
    }

    win_data_ptr = (MMIIM_SET_INPUT_LANG_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SET_INPUT_LANG_WIN_DATA_T));
    SCI_MEMSET(win_data_ptr, 0, sizeof(MMIIM_SET_INPUT_LANG_WIN_DATA_T));


    win_data_ptr->im_kernel_ptr = im_kernel_ptr;
    win_data_ptr->user_data_ptr = PNULL;

    win_handle = MMK_CreateWin((uint32*)MMIIM_SET_INPUT_LANG_WIN_TAB, win_data_ptr);
    GUIIM_AppendWin(im_kernel_ptr->im_handle, win_handle);

    return win_handle;
}

/*****************************************************************************/
//  Description : 点击OK按纽事件
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnIMClicked(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SWITCHING_IM_WIN_DATA_T *win_data_ptr
)
{
   //  MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    uint32 cur_selection = 0;
    uint16 nMaxDisplayItem = 0; //用来标识最后一项输入语言
    int i = 0;

    //get the selected item
    menu_id = GUIMENU_GetCurNodeId(ctrl_id);
    cur_selection = menu_id - 1;    //从1开始
    if(cur_selection >= win_data_ptr->item_counts)
    {
        //SCI_TRACE_LOW:"[MMIIM] OnIMClicked: cur_selection >= win_data_ptr->item_counts."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2439_112_2_18_2_25_8_36,(uint8*)"");
        return;
    }

    //更新当前的焦点项位置
    win_data_ptr->item_hot = cur_selection;

    for (i = 0; i < win_data_ptr->item_counts; i++)
    {
        if (win_data_ptr->item_ptr[i].is_display)
        {
            nMaxDisplayItem++;
        }
    }

    //最后一项，设置当前语言，需要创建窗口
    if (cur_selection == nMaxDisplayItem - 1 &&
            0 == win_data_ptr->menu_table_ptr[cur_selection].method)
    {
        GUIIM_INIT_KERNEL_T* im_kernel_ptr = win_data_ptr->im_kernel_ptr;
/*
#ifndef MMI_IM_PDA_SUPPORT        
        GUI_BOTH_RECT_T both_rect = win_data_ptr->disp_rect;
        MMIIM_POPMENU_EP_E effective_point = win_data_ptr->effective_point;
        // uint16 width = 0;
#endif
*/

#if defined MMI_IM_PDA_SUPPORT

#if (defined MAINLCD_DEV_SIZE_320X240 && (defined KEYPAD_TYPE_QWERTY_KEYPAD))  //TK_QW版本不支持触摸屏输入
    {
        GUI_BOTH_RECT_T both_rect = win_data_ptr->disp_rect;
        MMIIM_POPMENU_EP_E effective_point = win_data_ptr->effective_point;
        //先创建语言选择窗口，后销毁输入法切换窗口
        MMIIM_SetLangCreateWin(im_kernel_ptr, both_rect, effective_point);
        
        //close win
        MMK_CloseWin(win_id);
    }
#else
    {
        MMI_HANDLE_T win_handle = 0;

        MMK_CloseWin(win_id);

        win_handle = MMIAPIIM_OpenSetWin();

         //如下情况不再处理消息
         // //1 数据校验信息被破坏
        //  //2 控件已经被销毁
        if (PNULL == im_kernel_ptr->im_handle) 
            return ;

        if (win_handle != 0)
        {
            GUIIM_AppendWin(im_kernel_ptr->im_handle, win_handle);
        }
    }
#endif
        
#else
    {
        GUI_BOTH_RECT_T both_rect = win_data_ptr->disp_rect;
        MMIIM_POPMENU_EP_E effective_point = win_data_ptr->effective_point;
        
        //先创建语言选择窗口，后销毁输入法切换窗口
        MMIIM_SetLangCreateWin(im_kernel_ptr, both_rect, effective_point);

        //close win
        MMK_CloseWin(win_id);
    }
#endif

    }
    else
    {
        GUIIM_METHOD_T method = win_data_ptr->menu_table_ptr[cur_selection].method;
        MMI_HANDLE_T im_handle = win_data_ptr->im_kernel_ptr->im_handle;

        //close win
        MMK_CloseWin(win_id);

        //change method
        SendChangeMethodMsg(method, im_handle);
    }
}

/*****************************************************************************/
//  Description : 释放释放资源
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnIMReleased(MMIIM_SWITCHING_IM_WIN_DATA_T *win_data_ptr)
{
    if(PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] OnIMReleased: win_data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2514_112_2_18_2_25_8_37,(uint8*)"");
        return;
    }

    //free data
    SCI_FREE(win_data_ptr->user_data_ptr);
    SCI_FREE(win_data_ptr->menu_table_ptr);
    SCI_FREE(win_data_ptr->item_ptr);
    win_data_ptr->item_hot = 0;
    win_data_ptr->item_counts = 0;
}

/*****************************************************************************/
//  Description : 完成窗口创建之前的准备
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 要显示的区域
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIIM_SwitchingCreateWin(
                                             GUIIM_INIT_KERNEL_T* im_kernel_ptr,
                                             GUI_BOTH_RECT_T disp_rect,
                                             MMIIM_POPMENU_EP_E effective_point
                                             )
{
    MMIIM_SWITCHING_IM_WIN_DATA_T *win_data_ptr = PNULL;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
    uint16 nMaxDisplayItem = 0;
    MMI_HANDLE_T win_handle = 0;
    uint16 i = 0;

    if (PNULL == im_kernel_ptr
        || PNULL == im_kernel_ptr->method_param_ptr)
    {
        return win_handle;
    }

    win_data_ptr = (MMIIM_SWITCHING_IM_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SWITCHING_IM_WIN_DATA_T));
    SCI_MEMSET(win_data_ptr, 0, sizeof(MMIIM_SWITCHING_IM_WIN_DATA_T));

    win_data_ptr->disp_rect.h_rect = disp_rect.h_rect;
    win_data_ptr->disp_rect.v_rect = disp_rect.v_rect;
    win_data_ptr->effective_point = effective_point;

    win_data_ptr->im_kernel_ptr = im_kernel_ptr;
    win_data_ptr->user_data_ptr = PNULL;
    win_data_ptr->default_method = GUIIM_M_NONE;

    //如果当前语言是什么语言都支持，则需要去读一下NV，取出之前保存的输入语言
    lang = im_kernel_ptr->method_param_ptr->input_lang;

    if (GUIIM_LANG_NONE == lang)
    { 
        lang = MMIAPIIM_GetLanguage();
    }

    if (GUIIM_LANG_NONE == lang)
    {
        lang = im_kernel_ptr->method_param_ptr->im_set.lang;

        if (GUIIM_LANG_NONE == lang)
        {
            MMIAPISET_GetLanguageType(&language_type);
            lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
        }
    }

    win_data_ptr->last_lang = lang;

    //先求取一下，是否总共有多少个可以切换的输入法，如果只有一个，则不切换
    InitIMDisplayValues(win_data_ptr);
    for (i = 0; i < win_data_ptr->item_counts; i++)
    {
        if (win_data_ptr->item_ptr[i].is_display)
        {
            nMaxDisplayItem++;
        }
    }
    if (1 >= nMaxDisplayItem)
    {
        //如果只有一项，则不需要弹出输入法切换窗口，但请务必释放空间
        OnIMReleased(win_data_ptr);

        SCI_FREE(win_data_ptr);
        win_data_ptr = PNULL;
            
        return 0;
    }

    win_handle = MMK_CreateWin((uint32*)MMIIM_SWITCH_IM_WIN_TAB, win_data_ptr);
    GUIIM_AppendWin(im_kernel_ptr->im_handle, win_handle);

    return win_handle;
}

/*****************************************************************************/
//  Description : 动态添加输入法项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_AppendIMListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr
)
{
    // uint16 cur_selection = 0;
    int32 i = 0;
    uint32 node_index = 0;
    // GUIMENU_POP_SEL_INFO_T select_pop_menu = {0};

    if(PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] MMIIM_AppendIMListItem: win_data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2623_112_2_18_2_25_8_38,(uint8*)"");
        return;
    }

    for (i = 0; i < win_data_ptr->item_counts; i++)
    {
        if (win_data_ptr->item_ptr[i].is_display)
        {
            GUIMENU_DYNA_ITEM_T node_item = {0};/*lint !e64*/
            MMI_STRING_T string = {0};          /*lint !e64*/

            MMITHEME_GetResText(win_data_ptr->item_ptr[i].txt_id,
                                win_data_ptr->im_kernel_ptr->win_handle, &string);
            node_item.is_grayed = FALSE;
            node_item.select_icon_id = 0;
            node_item.item_text_ptr = &string;

            GUIMENU_InsertNode(node_index, node_index + 1, 0, &node_item, MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID);
            node_index++;
        }
    }

    //设置第一次默认焦点项
    if (win_data_ptr->item_hot < node_index)
    {
        GUIMENU_SetFirstAndSelectedItem(0, win_data_ptr->item_hot, MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID);
    }
}

/*****************************************************************************/
//  Description : 设置当前的输入法方法，需要通知输入法控件
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SendChangeMethodMsg(GUIIM_METHOD_T method, MMI_HANDLE_T im_handle)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_CFG_T *method_cfg_ptr = PNULL;
    GUIIM_METHOD_T new_method = method;
    uint16 keyboard_type = 0;

    if(GUIIM_M_NONE == method)
    {
        //SCI_TRACE_LOW:"[MMIIM] SendChangeMethodMsg: method is GUIIM_M_NONE."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2665_112_2_18_2_25_8_39,(uint8*)"");
        return;
    }

    method_cfg_ptr = MMITHEME_IMGetDefByMethod(method);
    if(PNULL == method_cfg_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] SendChangeMethodMsg: method_cfg_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2672_112_2_18_2_25_8_40,(uint8*)"");
        return;
    }

    do
    {
        //tp_123，don't change, just break
        if (GUIIM_M_TP_123 == method)
        {
            break;
        }
        
        //如果是TP输入，则需要考虑转换，转换规则根据NV保存的9，26键键盘类型
        if (GUIIM_INPUT_TYPE_TOUCHPEN == method_cfg_ptr->input
            || GUIIM_INPUT_TYPE_TP_QWERT == method_cfg_ptr->input)
        {
            //如果不是multitap，则跳出 (注：智能输入暂时不用method区分9, 26，由应用自己处理)
            if (GUIIM_TYPE_MULTITAP != method_cfg_ptr->type)
            {
                break;
            }

            //是26键，则需要重新查一下method
            keyboard_type = MMIIM_GetKeyBoradType();
            if (1 == keyboard_type)
            {
                new_method = MMITHEME_IMGetMethodByInput(method, GUIIM_INPUT_TYPE_TP_QWERT);
            }

            /*又因为，目前multitap不支持9键输入，所以，这里遇到是0的，也统一改成26key，这里临时做法*/
            if (0 == keyboard_type)
            {
                //强制改成1
                MMIIM_SetKeyBoradType(1);
                new_method = MMITHEME_IMGetMethodByInput(method, GUIIM_INPUT_TYPE_TP_QWERT);
            }
        }
        else if (GUIIM_INPUT_TYPE_KEYBOARD == method_cfg_ptr->input
            || GUIIM_INPUT_TYPE_QWERT == method_cfg_ptr->input)
        {

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            new_method = MMITHEME_IMGetMethodByInput(method, GUIIM_INPUT_TYPE_QWERT);
#endif
        }

    } while(0);

    //SCI_TRACE_LOW:"[MMIIM] SendChangeMethodMsg: method=%04X, new_method=%04X."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2720_112_2_18_2_25_8_41,(uint8*)"dd", method, new_method);

    notify_pack.data.set_method = new_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(im_handle, &notify_pack);
}

/*****************************************************************************/
//  Description : 获取列表各项值，包括对应索引表的构造
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitIMDisplayValues(MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr)
{
#if defined MMI_IM_PDA_SUPPORT  //需求待定，是否需要区分大小写
//#define IM_KB_NO_CAPS
#endif  

#if (defined MMI_IM_PDA_SUPPORT && !defined MMIIM_VKB_FORBID)
#define IM_KB_NO_123            //从物理123进入触摸屏123，而tp123无法进行输入法切换异常    
#endif

    GUIIM_METHOD_PARAM_T *method_def_ptr = PNULL;
    uint16  curlang_im_count = 0;
    MMIIM_IM_NAME_T const *cur_lang_im_name_ptr = PNULL;
    uint16 curlang_abc_im_count = 0;
    MMIIM_IM_NAME_T const *cur_lang_abc_im_name_ptr = PNULL;
    uint16 i = 0;
    uint16 dest_idx = 0;
    BOOLEAN is_tp_support = FALSE;
    uint16 cnt_hw = 0;
    uint16 cnt_abc = 0;
    uint16 cnt_123 = 0;
    uint16 cnt_sys = 0;
    GUIIM_TYPE_T type_set = GUIIM_TYPE_NONE;
    uint16 display_idx = 0;

    if(PNULL == win_data_ptr
        || PNULL == win_data_ptr->im_kernel_ptr
        || PNULL == win_data_ptr->im_kernel_ptr->method_param_ptr
        || PNULL == win_data_ptr->im_kernel_ptr->method_param_ptr->im_def_ptr)
    {        
        //SCI_TRACE_LOW:"[MMIIM] InitIMDisplayValues: param is not valid."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_2752_112_2_18_2_25_8_42,(uint8*)"");
        return;
    }

    method_def_ptr = win_data_ptr->im_kernel_ptr->method_param_ptr;
    type_set = method_def_ptr->type_set;

    is_tp_support = (GUIIM_INPUT_TYPE_TOUCHPEN == method_def_ptr->im_def_ptr->input
        || GUIIM_INPUT_TYPE_TP_QWERT == method_def_ptr->im_def_ptr->input
        || GUIIM_INPUT_TYPE_HANDWRITING == method_def_ptr->im_def_ptr->input);

    switch (win_data_ptr->last_lang)
    {
#ifdef IM_ENGLISH_SUPPORT
    case GUIIM_LANG_ENGLISH:
        { 
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)            
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_english_im_tp_name_list : s_english_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif            
        }
        break;
#endif

#ifdef IM_SIMP_CHINESE_SUPPORT
    case GUIIM_LANG_CHS:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_simpchs_im_tp_name_list : s_simpchs_im_name_list;
                curlang_im_count = 2;
            }
#endif
        }
        break;
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    case GUIIM_LANG_CHT:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_tradchs_im_tp_name_list : s_tradchs_im_name_list;
                curlang_im_count = 2;
            }
#endif
        }
        break;
#endif

#ifdef IM_ARABIC_SUPPORT
    case GUIIM_LANG_ARABIC:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_arabic_im_tp_name_list : s_arabic_im_name_list;
                curlang_im_count = 1;
            }
#endif            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_arabic_abc_im_tp_name_list : s_arabic_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_FRENCH_SUPPORT
    case GUIIM_LANG_FRENCH:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_french_im_tp_name_list : s_french_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif            
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_french_abc_im_tp_name_list : s_french_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_GERMAN_SUPPORT
    case GUIIM_LANG_GERMAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_german_im_tp_name_list : s_german_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif            
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_german_abc_im_tp_name_list : s_german_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_GREEK_SUPPORT
    case GUIIM_LANG_GREEK:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_greek_im_tp_name_list : s_greek_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif            
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_greek_abc_im_tp_name_list : s_greek_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_HINDI_SUPPORT
    case GUIIM_LANG_HINDI:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_hidi_im_tp_name_list : s_hidi_im_name_list;
                curlang_im_count = 1;
            }
#endif            

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_hidi_abc_im_tp_name_list : s_hidi_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_HUNGARIAN_SUPPORT
    case GUIIM_LANG_HUNGARIAN:
        {
#if (defined IM_ENGINE_CSTAR)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_hungarian_im_tp_name_list : s_hungarian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif            

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_hungarian_abc_im_tp_name_list : s_hungarian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_INDONESIAN_SUPPORT
    case GUIIM_LANG_INDONESIAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_indonesian_im_tp_name_list : s_indonesian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_indonesian_abc_im_tp_name_list : s_indonesian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_ITALIAN_SUPPORT
    case GUIIM_LANG_ITALIAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_italian_im_tp_name_list : s_italian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_italian_abc_im_tp_name_list : s_italian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_MALAY_SUPPORT
    case GUIIM_LANG_MALAY:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_malay_im_tp_name_list : s_malay_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_malay_abc_im_tp_name_list : s_malay_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_PERSIAN_SUPPORT
    case GUIIM_LANG_PERSIAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_persian_im_tp_name_list : s_persian_im_name_list;
                curlang_im_count = 1;
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_persian_abc_im_tp_name_list : s_persian_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    case GUIIM_LANG_PORTUGUESE:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_portuguese_im_tp_name_list : s_portuguese_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_portuguese_abc_im_tp_name_list : s_portuguese_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_RUSSIAN_SUPPORT
    case GUIIM_LANG_RUSSIAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_russian_im_tp_name_list : s_russian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_russian_abc_im_tp_name_list : s_russian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_SPANISH_SUPPORT
    case GUIIM_LANG_SPANISH:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_spanish_im_tp_name_list : s_spanish_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_spanish_abc_im_tp_name_list : s_spanish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_TAGALOG_SUPPORT
    case GUIIM_LANG_TAGALOG:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_tagalog_im_tp_name_list : s_tagalog_im_name_list;;
                curlang_im_count = 1;
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_tagalog_abc_im_tp_name_list : s_tagalog_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_THAI_SUPPORT
    case GUIIM_LANG_THAI:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_thai_im_tp_name_list : s_thai_im_name_list;
                curlang_im_count = 1;
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_thai_abc_im_tp_name_list : s_thai_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_TURKISH_SUPPORT
    case GUIIM_LANG_TURKISH:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_turkish_im_tp_name_list : s_turkish_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_turkish_abc_im_tp_name_list : s_turkish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_URDU_SUPPORT
    case GUIIM_LANG_URDU:
        {
#if defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_urdu_im_tp_name_list : s_urdu_im_name_list;
                curlang_im_count = 1;
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_urdu_abc_im_tp_name_list : s_urdu_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    case GUIIM_LANG_VIETNAMESE:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_vietnamese_im_tp_name_list : s_vietnamese_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_vietnamese_abc_im_tp_name_list : s_vietnamese_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_HEBREW_SUPPORT
    case GUIIM_LANG_HEBREW:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_hebrew_im_tp_name_list : s_hebrew_im_name_list;
                curlang_im_count = 1;
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_hebrew_abc_im_tp_name_list : s_hebrew_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_BENGALI_SUPPORT
    case GUIIM_LANG_BENGALI:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_bengali_im_tp_name_list : s_bengali_im_name_list;
                curlang_im_count = 1;
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_bengali_abc_im_tp_name_list : s_bengali_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif
#ifdef IM_CZECH_SUPPORT
    case GUIIM_LANG_CZECH:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_czech_im_tp_name_list : s_czech_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_czech_abc_im_tp_name_list : s_czech_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_SLOVENIAN_SUPPORT
    case GUIIM_LANG_SLOVENIAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_slovenian_im_tp_name_list : s_slovenian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_slovenian_abc_im_tp_name_list : s_slovenian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_ROMANIAN_SUPPORT
    case GUIIM_LANG_ROMANIAN:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_romanian_im_tp_name_list : s_romanian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_romanian_abc_im_tp_name_list : s_romanian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_TELUGU_SUPPORT
    case GUIIM_LANG_TELUGU:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_telugu_im_tp_name_list : s_telugu_im_name_list;
                curlang_im_count = 1;
            }
#endif

	    //泰卢固语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_telugu_abc_im_tp_name_list: s_telugu_abc_im_name_list;
            curlang_abc_im_count = 1;

        }
        break;
#endif

#ifdef IM_MARATHI_SUPPORT
    case GUIIM_LANG_MARATHI:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_marathi_im_tp_name_list : s_marathi_im_name_list;
                curlang_im_count = 1;
            }
#endif
			
	    //马拉地语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_marathi_abc_im_tp_name_list : s_marathi_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_TAMIL_SUPPORT
    case GUIIM_LANG_TAMIL:
        {
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU || defined IM_ENGINE_T9)
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_tamil_im_tp_name_list : s_tamil_im_name_list;
                curlang_im_count = 1;
            }
#endif

	    //泰米尔语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_tamil_abc_im_tp_name_list : s_tamil_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_GUJARATI_SUPPORT
    case GUIIM_LANG_GUJARATI:
        {
/*#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_gujarati_im_tp_name_list : s_gujarati_im_name_list;
                curlang_im_count = 1;
            }
#endif*/
            
	    //古吉拉特语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_gujarati_abc_im_tp_name_list : s_gujarati_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_KANNADA_SUPPORT
    case GUIIM_LANG_KANNADA:
        {
	    //卡纳达multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_kannada_abc_im_tp_name_list : s_kannada_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_MALAYALAM_SUPPORT
    case GUIIM_LANG_MALAYALAM:
        {
	    //马拉亚拉姆multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_malayalam_abc_im_tp_name_list : s_malayalam_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

//缅甸
#ifdef IM_MYANMAR_SUPPORT
    case GUIIM_LANG_MYANMAR:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_myanmar_im_tp_name_list : s_myanmar_im_name_list;
                curlang_im_count = 1;
            }
#endif

	    //缅甸multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_myanmar_abc_im_tp_name_list : s_myanmar_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_ORIYA_SUPPORT
    case GUIIM_LANG_ORIYA:
        {
	    //奥里亚multitap相关输入
	    cur_lang_abc_im_name_ptr = (is_tp_support) ? s_oriya_abc_im_tp_name_list : s_oriya_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_PUNJABI_SUPPORT
    case GUIIM_LANG_PUNJABI:
        {
#if defined IM_ENGINE_SOGOU
            //{
            //    cur_lang_im_name_ptr = (is_tp_support) ? s_punjabi_im_tp_name_list : s_punjabi_im_name_list;
            //    curlang_im_count = 1;
            //}
#endif
            
	    //旁遮普multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_punjabi_abc_im_tp_name_list : s_punjabi_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_AFRIKAANS_SUPPORT
    case GUIIM_LANG_AFRIKAANS:
        {
	    //南非荷兰语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_afrikaans_abc_im_tp_name_list: s_afrikaans_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_ALBANIAN_SUPPORT
    case GUIIM_LANG_ALBANIAN:
        {
	    //阿尔巴尼亚语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_albanian_abc_im_tp_name_list: s_albanian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_ARMENIAN_SUPPORT
    case GUIIM_LANG_ARMENIAN:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_armenian_im_tp_name_list : s_armenian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            } 
#endif
            
	    // 亚美尼亚语语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_armenian_abc_im_tp_name_list: s_armenian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_AZERBAIJANI_SUPPORT
    case GUIIM_LANG_AZERBAIJANI:
        {
	    // 阿塞拜疆语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_azerbaijani_abc_im_tp_name_list: s_azerbaijani_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_BASQUE_SUPPORT
    case GUIIM_LANG_BASQUE:
        {
	    // 巴斯克语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_basque_abc_im_tp_name_list: s_basque_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_BULGARIAN_SUPPORT
    case GUIIM_LANG_BULGARIAN:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_bulgarian_im_tp_name_list : s_bulgarian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
           
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_bulgarian_abc_im_tp_name_list : s_bulgarian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_CATALAN_SUPPORT
    case GUIIM_LANG_CATALAN:
        {
	    // 加泰罗尼亚语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_catalan_abc_im_tp_name_list: s_catalan_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_CROATIAN_SUPPORT
    case GUIIM_LANG_CROATIAN:
        {
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_croatian_abc_im_tp_name_list : s_croatian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_DANISH_SUPPORT
    case GUIIM_LANG_DANISH:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_danish_im_tp_name_list : s_danish_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_danish_abc_im_tp_name_list : s_danish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_DUTCH_SUPPORT
    case GUIIM_LANG_DUTCH:
        {
#if defined IM_ENGINE_CSTAR
            cur_lang_im_name_ptr = (is_tp_support) ? s_dutch_im_tp_name_list : s_dutch_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_im_count = 1;
            #else
                curlang_im_count = 3;
            #endif
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_dutch_abc_im_tp_name_list : s_dutch_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_ESTONIAN_SUPPORT
    case GUIIM_LANG_ESTONIAN:
        {
	    // 爱沙尼亚语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_estonian_abc_im_tp_name_list: s_estonian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_FILIPINO_SUPPORT
    case GUIIM_LANG_FILIPINO:
        {
	    //菲律宾multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_filipino_abc_im_tp_name_list: s_filipino_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_FINNISH_SUPPORT
    case GUIIM_LANG_FINNISH:
        {
#if defined IM_ENGINE_CSTAR            
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_finnish_im_tp_name_list : s_finnish_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

	    //芬兰multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_finnish_abc_im_tp_name_list: s_finnish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_GALICIAN_SUPPORT
    case GUIIM_LANG_GALICIAN:
        {
	    // 加利西亚语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_galician_abc_im_tp_name_list: s_galician_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_GEORGIAN_SUPPORT
    case GUIIM_LANG_GEORGIAN:
        {
	    // 格鲁吉亚语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_georgian_abc_im_tp_name_list : s_georgian_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_HAUSA_SUPPORT
    case GUIIM_LANG_HAUSA:
        {
#if defined IM_ENGINE_SOGOU
            cur_lang_im_name_ptr = (is_tp_support) ? s_hausa_im_tp_name_list : s_hausa_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_im_count = 1;
            #else
                curlang_im_count = 3;
            #endif
#endif
	    // 豪撒语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_hausa_abc_im_tp_name_list: s_hausa_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_ICELANDIC_SUPPORT
    case GUIIM_LANG_ICELANDIC:
        {
	    // 冰岛语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_icelandic_abc_im_tp_name_list: s_icelandic_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_IGBO_SUPPORT
    case GUIIM_LANG_IGBO:
        {
	    // 伊博语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_igbo_abc_im_tp_name_list: s_igbo_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_IRISH_SUPPORT
    case GUIIM_LANG_IRISH:
        {
	    // 爱尔兰语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_irish_abc_im_tp_name_list: s_irish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_KAZAKH_SUPPORT
    case GUIIM_LANG_KAZAKH:
        {
	    // 哈萨克斯坦multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_kazakh_abc_im_tp_name_list: s_kazakh_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_LATVIAN_SUPPORT
    case GUIIM_LANG_LATVIAN:
        {
	    //拉脱维亚multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_latvian_abc_im_tp_name_list: s_latvian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_LITHUANIAN_SUPPORT
    case GUIIM_LANG_LITHUANIAN:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_lithuanian_im_tp_name_list : s_lithuanian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_lithuanian_abc_im_tp_name_list : s_lithuanian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_MACEDONIAN_SUPPORT
    case GUIIM_LANG_MACEDONIAN:
        {
	    //马其顿multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_macedonian_abc_im_tp_name_list: s_macedonian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_MOLDOVAN_SUPPORT
    case GUIIM_LANG_MOLDOVAN:
        {
	    // 摩尔多瓦语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_moldovan_abc_im_tp_name_list: s_moldovan_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_NORWEGIAN_SUPPORT
    case GUIIM_LANG_NORWEGIAN:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_norwegian_im_tp_name_list : s_norwegian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_norwegian_abc_im_tp_name_list : s_norwegian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_POLISH_SUPPORT
    case GUIIM_LANG_POLISH:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_polish_im_tp_name_list : s_polish_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_polish_abc_im_tp_name_list : s_polish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_SERBIAN_SUPPORT
    case GUIIM_LANG_SERBIAN:
        {
	    // 塞尔维亚语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_serbian_abc_im_tp_name_list: s_serbian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_SESOTHO_SUPPORT
    case GUIIM_LANG_SESOTHO:
        {
	    // 塞索托语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_sesotho_abc_im_tp_name_list: s_sesotho_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_SLOVAK_SUPPORT
    case GUIIM_LANG_SLOVAK:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_slovak_im_tp_name_list : s_slovak_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_slovak_abc_im_tp_name_list : s_slovak_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_SWEDISH_SUPPORT
    case GUIIM_LANG_SWEDISH:
        {
#if defined IM_ENGINE_CSTAR
            cur_lang_im_name_ptr = (is_tp_support) ? s_swedish_im_tp_name_list : s_swedish_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_im_count = 1;
            #else
                curlang_im_count = 3;
            #endif
#endif

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_swedish_abc_im_tp_name_list : s_swedish_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_UKRAINIAN_SUPPORT
    case GUIIM_LANG_UKRAINIAN:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_ukrainian_im_tp_name_list : s_ukrainian_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_ukrainian_abc_im_tp_name_list : s_ukrainian_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

#ifdef IM_YORUBA_SUPPORT
    case GUIIM_LANG_YORUBA:
        {
	    // 约鲁巴语multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_yoruba_abc_im_tp_name_list: s_yoruba_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

//科萨语 
#ifdef IM_XHOSA_SUPPORT
    case GUIIM_LANG_XHOSA:
        {
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_xhosa_abc_im_tp_name_list: s_xhosa_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

//祖鲁语 
#ifdef IM_ZULU_SUPPORT
    case GUIIM_LANG_ZULU:
        {
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_zulu_abc_im_tp_name_list: s_zulu_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

//(印度语支的)阿萨姆语	
#ifdef IM_ASSAMESE_SUPPORT
    case GUIIM_LANG_ASSAMESE:
        {
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_assamese_abc_im_tp_name_list: s_assamese_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

//斯瓦西里语	
#ifdef IM_SWAHILI_SUPPORT
    case GUIIM_LANG_SWAHILI:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_swahili_im_tp_name_list : s_swahili_im_name_list;
                #if defined IM_KB_NO_CAPS
                    curlang_im_count = 1;
                #else
                    curlang_im_count = 3;
                #endif
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_swahili_abc_im_tp_name_list: s_swahili_abc_im_name_list;
            #if defined IM_KB_NO_CAPS
                curlang_abc_im_count = 1;
            #else
                curlang_abc_im_count = 3;
            #endif
        }
        break;
#endif

//阿姆哈拉
#ifdef IM_AMHARIC_SUPPORT
    case GUIIM_LANG_AMHARIC:
        {
	    //multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_amharic_abc_im_tp_name_list : s_amharic_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

//柬埔寨
#ifdef IM_KHMER_SUPPORT
    case GUIIM_LANG_KHMER:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_khmer_im_tp_name_list : s_khmer_im_name_list;
                curlang_im_count = 1;
            }
#endif
            
	    //multitap相关输入
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_khmer_abc_im_tp_name_list : s_khmer_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

//老挝
#ifdef IM_LAO_SUPPORT
    case GUIIM_LANG_LAO:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_lao_im_tp_name_list : s_lao_im_name_list;
                curlang_im_count = 1;       //不存在大小写区分
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_lao_abc_im_tp_name_list: s_lao_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_UYGHUR_SUPPORT
    case GUIIM_LANG_UYGHUR:
        {
#if defined IM_ENGINE_CSTAR
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_uyghur_im_tp_name_list : s_uyghur_im_name_list;
                curlang_im_count = 1;       //不存在大小写区分
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_uyghur_abc_im_tp_name_list: s_uyghur_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_TIBETAN_SUPPORT   //tibetan 2012 0529
    case GUIIM_LANG_TIBETAN:
        {
      //      cur_lang_im_name_ptr = (is_tp_support) ? s_tibetan_im_tp_name_list : s_tibetan_im_name_list;
      //      curlang_im_count = 1;       //不存在大小写区分

            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_tibetan_abc_im_tp_name_list: s_tibetan_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

#ifdef IM_SINHALESE_SUPPORT
    case GUIIM_LANG_SINHALESE:
        {
#if defined IM_ENGINE_SOGOU
            {
                cur_lang_im_name_ptr = (is_tp_support) ? s_sinhalese_im_tp_name_list : s_sinhalese_im_name_list;
                curlang_im_count = 1;       //不存在大小写区分
            }
#endif
            
            cur_lang_abc_im_name_ptr = (is_tp_support) ? s_sinhalese_abc_im_tp_name_list: s_sinhalese_abc_im_name_list;
            curlang_abc_im_count = 1;
        }
        break;
#endif

    default:
        break;
    }

//sprd无智能输入法，从这里修改，免得每个地方需要用宏控制，并一个个的修改
#if defined IM_ENGINE_NONE
    curlang_im_count = 0;
#endif

    //当前输入语言支持的输入法+通用输入法+语言修改项
    if (is_tp_support)
    {
        #if defined IM_KB_NO_CAPS
            cnt_abc = 1;
        #else
            cnt_abc = ARR_SIZE(s_abc_im_tp_name_list);
        #endif

        cnt_123 = ARR_SIZE(s_123_im_tp_name_list);
        cnt_sys = ARR_SIZE(s_sys_im_tp_name_list);
    }
    else
    {
        #if defined IM_KB_NO_CAPS
            cnt_abc = 1;
        #else
            cnt_abc = ARR_SIZE(s_abc_im_name_list);
        #endif

        cnt_123 = ARR_SIZE(s_123_im_name_list);
        cnt_sys = ARR_SIZE(s_sys_im_name_list);
    }

#ifdef IM_KB_NO_123  //virtural keyboard version hasn't 123 in menu
    if (MMITHEME_IMTpAllow())
    {
        cnt_123 = 0;    //virtural keyboard version hasn't 123 in menu
    }
#endif

#if defined(MMIIM_USE_HANDWRITING)
    cnt_hw = ARR_SIZE(s_hw_im_name_list);
#endif

#ifdef IM_ENGINE_T9
    if (GUIIM_LANG_ENGLISH != win_data_ptr->last_lang)
    {
        cnt_abc = 0;
    }
#endif

    //+1, +1, 一个表示手写，一个表示支持输入语言设置
    win_data_ptr->item_counts = curlang_im_count + curlang_abc_im_count + cnt_hw + cnt_abc + cnt_123 + cnt_sys + 1;

    win_data_ptr->item_ptr =
        (MMIIM_IM_NAME_T*)SCI_ALLOC_APP(win_data_ptr->item_counts * sizeof(MMIIM_IM_NAME_T));
    SCI_MEMSET(win_data_ptr->item_ptr, 0, win_data_ptr->item_counts * sizeof(MMIIM_IM_NAME_T));
    win_data_ptr->menu_table_ptr =
        (MMIIM_IM_MENU_T*)SCI_ALLOC_APP(win_data_ptr->item_counts * sizeof(MMIIM_IM_MENU_T));
    SCI_MEMSET(win_data_ptr->menu_table_ptr, 0, win_data_ptr->item_counts * sizeof(MMIIM_IM_MENU_T));

    //初始化当前输入语言支持的输入法项，从0开始，智能的输入法
    for (i = 0; i < curlang_im_count; i++)  /*lint !e681*/
    {
        if (PNULL == cur_lang_im_name_ptr) break;
        
        win_data_ptr->item_ptr[i].txt_id = cur_lang_im_name_ptr[i].txt_id;
        win_data_ptr->item_ptr[i].method = cur_lang_im_name_ptr[i].method;
		/*
		 *  针对Bug 124554 - [sc7701_128X160BAR_32MB] dorado网址栏位编辑地址时,email帐号输入时,
		 *  还有彩信输入受检人时,查看输入法都没有En,en,EN这3种输入法.
		 *	根据lianxiang的注释：英文总是同时支持smart和abc的。      当前语言如果为english,
		 *  并且APP在对允许输入类型集合中设置了GUIIM_TYPE_ENGLISH，则显示英文的智能输入法。
		 */
        win_data_ptr->item_ptr[i].is_display = ((GUIIM_LIMIT_LOCAL & method_def_ptr->limit) == 0 || 
												(GUIIM_LANG_ENGLISH == win_data_ptr->last_lang &&
												 GUIIM_TYPE_ENGLISH & method_def_ptr->type_set));
    }

    dest_idx = curlang_im_count;

    //初始化当前输入语言支持的输入法项，从智能的最后一项接着，multitap输入法
    for (i = 0; i < curlang_abc_im_count; i++)/*lint !e681*/
    {
        if (PNULL == cur_lang_abc_im_name_ptr) break;
        
        win_data_ptr->item_ptr[dest_idx+i].txt_id = cur_lang_abc_im_name_ptr[i].txt_id;
        win_data_ptr->item_ptr[dest_idx+i].method = cur_lang_abc_im_name_ptr[i].method;

        win_data_ptr->item_ptr[dest_idx+i].is_display = ((GUIIM_LIMIT_LOCAL & method_def_ptr->limit) == 0);
    }

    dest_idx += curlang_abc_im_count;

    //初始化默认输入法项
    for (i = 0; i < cnt_sys; i++)
    {
        if (is_tp_support)
        {
            win_data_ptr->item_ptr[i+dest_idx].txt_id = s_sys_im_tp_name_list[i].txt_id;
            win_data_ptr->item_ptr[i+dest_idx].method = s_sys_im_tp_name_list[i].method;
        }
        else
        {
            win_data_ptr->item_ptr[i+dest_idx].txt_id = s_sys_im_name_list[i].txt_id;
            win_data_ptr->item_ptr[i+dest_idx].method = s_sys_im_name_list[i].method;
        }

        win_data_ptr->item_ptr[i+dest_idx].is_display = TRUE;
    }
    dest_idx += cnt_sys;

    //初始化通用abc输入法项
    for (i = 0; i < cnt_abc; i++)
    {
        if (is_tp_support)
        {
            win_data_ptr->item_ptr[i+dest_idx].txt_id = s_abc_im_tp_name_list[i].txt_id;
            win_data_ptr->item_ptr[i+dest_idx].method = s_abc_im_tp_name_list[i].method;
        }
        else
        {
            win_data_ptr->item_ptr[i+dest_idx].txt_id = s_abc_im_name_list[i].txt_id;
            win_data_ptr->item_ptr[i+dest_idx].method = s_abc_im_name_list[i].method;
        }

        win_data_ptr->item_ptr[i+dest_idx].is_display = ((GUIIM_LIMIT_ENGLISH & method_def_ptr->limit) == 0);
    }

    dest_idx += cnt_abc;

    //初始化123输入法项
    for (i = 0; i < cnt_123; i++)   //lint !e*/
    {
        if (is_tp_support)
        {
            win_data_ptr->item_ptr[i+dest_idx].txt_id = s_123_im_tp_name_list[i].txt_id;
            win_data_ptr->item_ptr[i+dest_idx].method = s_123_im_tp_name_list[i].method;
        }
        else
        {
            win_data_ptr->item_ptr[i+dest_idx].txt_id = s_123_im_name_list[i].txt_id;
            win_data_ptr->item_ptr[i+dest_idx].method = s_123_im_name_list[i].method;
        }

        win_data_ptr->item_ptr[i+dest_idx].is_display = ((GUIIM_LIMIT_DIGITAL & method_def_ptr->limit) == 0);
    }


    dest_idx += cnt_123;

#if defined(MMIIM_USE_HANDWRITING)

    //初始化手写
    for (i = 0; i < cnt_hw; i++)
    {
        win_data_ptr->item_ptr[dest_idx].txt_id = s_hw_im_name_list[i].txt_id;
        win_data_ptr->item_ptr[dest_idx].method = s_hw_im_name_list[i].method;
        win_data_ptr->item_ptr[dest_idx].is_display = FALSE;

        win_data_ptr->item_ptr[dest_idx].is_display = ((GUIIM_INPUT_TYPE_HANDWRITING & method_def_ptr->input_set) != 0);
    }

    dest_idx++;
#endif

    //初始化语言设置项，其实就是最后一行
    for (i = win_data_ptr->item_counts - 1; i < win_data_ptr->item_counts; i++)
    {
        BOOLEAN is_not_limit = ((GUIIM_LIMIT_LOCAL & method_def_ptr->limit) == 0);
        BOOLEAN is_none_set_input = (GUIIM_LANG_NONE == method_def_ptr->input_lang);

        #if defined MMI_IM_PDA_SUPPORT
            win_data_ptr->item_ptr[i].txt_id = s_more_set_name_list[0].txt_id;
            win_data_ptr->item_ptr[i].method = s_more_set_name_list[0].method;
        #else
            win_data_ptr->item_ptr[i].txt_id = s_cur_lang_name_list[0].txt_id;
            win_data_ptr->item_ptr[i].method = s_cur_lang_name_list[0].method;
        #endif
        
        win_data_ptr->item_ptr[i].is_display = (is_not_limit && is_none_set_input);
    }

    //列表所有项的字符初始化完成，接着建与输入核的索引
    display_idx = 0;

    for (i = 0; i < win_data_ptr->item_counts; i++)
    {
        if (win_data_ptr->item_ptr[i].is_display)
        {
            win_data_ptr->menu_table_ptr[display_idx].idx = display_idx;
            win_data_ptr->menu_table_ptr[display_idx].method = win_data_ptr->item_ptr[i].method;
            display_idx++;
        }
    }
}

/*****************************************************************************/
//  Description : to handle Switching input method
//  Global resource dependence :
//  Author:haiwu.chen
//  Note: 窗口过程函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetInputLangWindow(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIIM_SET_INPUT_LANG_WIN_DATA_T *win_data_ptr = PNULL;
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_MAX_LANGUAGE;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    win_data_ptr = MMK_GetWinAddDataPtr(win_id);
    if(PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleSetInputLangWindow: win_data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_4278_112_2_18_2_25_11_43,(uint8*)"");
        return recode;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            if(PNULL == win_data_ptr->im_kernel_ptr
                || PNULL == win_data_ptr->im_kernel_ptr->method_param_ptr)
            {
                MMK_CloseWin(win_id);
                //SCI_TRACE_LOW:"[MMIIM] HandleSetInputLangWindow: MSG_OPEN_WINDOW param is null."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_4290_112_2_18_2_25_11_44,(uint8*)"");
                return recode;
            }

            GUILIST_SetMaxItem(MMIIM_SET_INPUT_LANGUAGE_CTRL_ID, MMISET_MAX_LANGUAGE, FALSE);

            //有一些通用输入法，lang为none，则要去取nv语言值
            lang = MMIAPIIM_GetLanguage();

            if (GUIIM_LANG_NONE == lang)
            {
                lang = win_data_ptr->im_kernel_ptr->method_param_ptr->im_set.lang;
                if (GUIIM_LANG_NONE == lang) //如果输入法控件那边还是给none的话，则直接把焦点放在英语上
                {
                    lang = GUIIM_LANG_ENGLISH;
                }
            }

            language_type = MMIAPIIM_GetSetLangByGUILangIdx(lang); /*lint !e64*/
            win_data_ptr->hot_lang = language_type;
            MMIAPISET_AppendInputLanguageListItem(MMIIM_SET_INPUT_LANGUAGE_CTRL_ID, language_type);
            MMK_SetAtvCtrl(win_id, MMIIM_SET_INPUT_LANGUAGE_CTRL_ID);
        }
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (!GUILIST_IsCurrItemInvalid(MMIIM_SET_INPUT_LANGUAGE_CTRL_ID))
        {
            OnLangClicked(win_id, MMIIM_SET_INPUT_LANGUAGE_CTRL_ID, win_data_ptr);
        }

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_CTRL:
#else
    case MSG_APP_HASH:
#endif
        MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);
        break;

    case MSG_CLOSE_WINDOW:
        GUIIM_RemoveWin(win_data_ptr->im_kernel_ptr->im_handle, MMK_ConvertIdToHandle(win_id));
        OnLangReleased(win_data_ptr);
        MMK_FreeWinAddData(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 动态创建输入法菜单项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateLangMenu(MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr)
{
    GUI_BOTH_RECT_T both_rect = {0};

    both_rect.h_rect = win_data_ptr->disp_rect.h_rect;
    both_rect.v_rect = win_data_ptr->disp_rect.v_rect;

    //creat dynamic menu
    GUIMENU_CreatDynamic(&win_data_ptr->disp_rect, MMIIM_SET_INPUT_LANG_WIN_ID,
                         MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

    MMK_SetWinMoveStyle(MMIIM_SET_INPUT_LANG_WIN_ID, MOVE_FORBIDDEN);

    GUIAPICTRL_SetBothRect(MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID, &both_rect);
    GUIMENU_SetMaxWidth(MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID,
        both_rect.h_rect.right - both_rect.h_rect.left,
        both_rect.v_rect.right - both_rect.v_rect.left);
    GUIMENU_SetEffectivePoint(MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID, (GUIMENU_EP_E)win_data_ptr->effective_point);

    MMK_SetAtvCtrl(MMIIM_SET_INPUT_LANG_WIN_ID, MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID);
}

/*****************************************************************************/
//  Description : 动态添加项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AppendLangItems(
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr
)
{
    // uint16 cur_selection = 0;
    int32 i = 0;
    uint16 node_index = 0;
    uint16 hot_index = 0;

    if(PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] AppendLangItems: win_data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_4394_112_2_18_2_25_11_45,(uint8*)"");
        return;
    }

    for (i = 0; i < MMISET_MAX_LANGUAGE; i++)
    {
        if (MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E) i))
        {
            GUIMENU_DYNA_ITEM_T node_item = {0};/*lint !e64*/
            MMI_STRING_T string = {0};          /*lint !e64*/

            MMIAPISET_GetInputLanguageString((MMISET_LANGUAGE_TYPE_E) i, &string);
            node_item.is_grayed = FALSE;
            node_item.select_icon_id = 0;
            node_item.item_text_ptr = &string;

            //友情提示：node_id既为set_lang中的枚举值+1
            GUIMENU_InsertNode(node_index, i + 1, 0, &node_item, MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID);

            if (win_data_ptr->hot_lang == i)
            {
                hot_index = node_index;
            }

            node_index++;
        }
    }

    //设置第一次默认焦点项
    GUIMENU_SetFirstAndSelectedItem(0, hot_index, MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID);
}

/*****************************************************************************/
//  Description : 点击OK按纽事件
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnLangClicked(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id,
    MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr
)
{
    uint16 cur_selection = 0;
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_MAX_LANGUAGE;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    GUIIM_LANGUAGE_T old_lang = GUIIM_LANG_NONE;
    GUIIM_METHOD_T new_method = GUIIM_M_NONE;
    MMI_HANDLE_T im_handle = 0;
    uint32 i = 0;

    if(PNULL == win_data_ptr
        || PNULL == win_data_ptr->im_kernel_ptr
        || PNULL == win_data_ptr->im_kernel_ptr->method_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] OnLangClicked: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_IM_SWITCHING_4448_112_2_18_2_25_12_46,(uint8*)"");
        return;
    }

    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);

    for(i=0; i < MMISET_MAX_LANGUAGE; i++)
    {
        if(cur_selection == GetValidIdxByLangIdx(i))
        {
            language_type = i;
            break;
        }
    }

    //取得GUI语言序
    lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
    old_lang = MMIAPIIM_GetLanguage();
    if (lang != old_lang)
    {
        //把当前的输入语言写NV
        MMIAPIIM_SetLanguage(lang);
    }

    im_handle = win_data_ptr->im_kernel_ptr->im_handle;

    new_method = GetNewMethodWhenLangWinClose(win_data_ptr, lang);

    MMK_CloseWin(win_id);

    if (GUIIM_M_NONE !=new_method)
    {
        SendChangeMethodMsg(new_method, im_handle);
    }

    return;
}

/*****************************************************************************/
//  Description : 通过新的输入语言，获取默认的输入法，目前设为第一项
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL GUIIM_METHOD_T GetNewMethodWhenLangWinClose(
    MMIIM_SET_INPUT_LANG_WIN_DATA_T *win_data_ptr,
    GUIIM_LANGUAGE_T language_type
)
{
    GUIIM_METHOD_T new_method = GUIIM_M_NONE;
    MMIIM_SWITCHING_IM_WIN_DATA_T *im_win_data_ptr = PNULL;
    uint16 i = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_data_ptr->im_kernel_ptr); /*assert verified*/
    if(PNULL == win_data_ptr ||PNULL == win_data_ptr->im_kernel_ptr)
    {
        return GUIIM_M_NONE;
    }
    
    im_win_data_ptr = (MMIIM_SWITCHING_IM_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SWITCHING_IM_WIN_DATA_T));
    SCI_MEMSET(im_win_data_ptr, 0, sizeof(MMIIM_SWITCHING_IM_WIN_DATA_T));

    im_win_data_ptr->im_kernel_ptr = win_data_ptr->im_kernel_ptr;
    im_win_data_ptr->last_lang = language_type;

    //初始列表所有项的内容
    InitIMDisplayValues(im_win_data_ptr);

    for (i = 0; i < im_win_data_ptr->item_counts; i++)
    {
        if (im_win_data_ptr->item_ptr[i].is_display)
        {
            new_method = im_win_data_ptr->item_ptr[i].method;
            break;
        }
    }

    //release
    OnIMReleased(im_win_data_ptr);

    SCI_FREE(im_win_data_ptr);
    im_win_data_ptr = PNULL;

    return new_method;

}

/*****************************************************************************/
//  Description : 只释放窗口内部释放资源
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnLangReleased(MMIIM_SET_INPUT_LANG_WIN_DATA_T* win_data_ptr)
{
    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr )
    {
        return;
    }
    //free data
    SCI_FREE(win_data_ptr->user_data_ptr);
}

/*****************************************************************************/
//  Description : get  valid lang index by lang index from s_lang_set
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
LOCAL uint32 GetValidIdxByLangIdx(uint32 lang_index)
{
    uint32      i               =   0;
    uint32      valid_index     =   0;

    //SCI_ASSERT(lang_index < MMISET_MAX_LANGUAGE); /*assert verified*/
    if(lang_index >= MMISET_MAX_LANGUAGE )
    {
        return valid_index;
    }

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    i = 1;          // 输入语言不需要auto，跳过去
#endif

    for(;i <= lang_index; i++)
    {
        if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))
        {
            valid_index++;
        }
    }

    return (valid_index - 1);
}

/*****************************************************************************/
//  Description : 由set中的语言序，得到控件中的语言值
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetGUILangBySetLangIdx(MMISET_LANGUAGE_TYPE_E set_lang_index)
{
    uint16 i = 0;
    uint16 nRet = 0;

    for (i = 0; i < ARR_SIZE(s_SetLang_ImLang_list); i++)
    {
        if (set_lang_index == s_SetLang_ImLang_list[i][0])
        {
            nRet = s_SetLang_ImLang_list[i][1];
            break;
        }
    }

    return nRet;
}

/*****************************************************************************/
//  Description : 由控件中的语言值，得到set中的语言序
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetSetLangByGUILangIdx(GUIIM_LANGUAGE_T gui_lang_index)
{
    uint16 i = 0;
    uint16 nRet = 0;

    for (i = 0; i < ARR_SIZE(s_SetLang_ImLang_list); i++)
    {
        if (gui_lang_index == s_SetLang_ImLang_list[i][1])
        {
            nRet = s_SetLang_ImLang_list[i][0];
            break;
        }
    }

    return nRet;
}


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
