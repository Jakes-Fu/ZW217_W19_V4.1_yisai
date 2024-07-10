/*****************************************************************************
** File Name:      mmiim_tp_cstar.c                                          *
** Author:                                                                   *
** Date:           2009/09/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle pb menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/9/16     rui.xu            Create                                    *
** 2010/07/07    haiwu.chen        Edit                                      *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_im_trc.h"
#include "mmiim_base.h"
#include "guiim_base.h"


#if defined(IM_ENGINE_CSTAR) && defined (TOUCH_PANEL_SUPPORT)

#include "os_api.h"
//#include "sci_types.h"

#include "csk2api.h"

#include "mmiim_tp_cstar_keyboard_cfg.h"
#include "mmiim_tp_cstar_position.h"
#include "mmiim_tp_cstar_internal.h"

#include "guires.h"
#include "mmiim_internal.h"
#include "mmitheme_im.h"

//#include "mmiim_image.h"
//#include "guistring.h"
#include "mmiim_im_switching.h"
//#include "mmitheme_pos.h"
#include "mmiim_sp_symbol.h"
//#include "ui_layer.h"
#include "mmiim_tp_ui.h"
#include "mmk_timer.h"
#include "spml_api.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 初始化输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void* Initialize(
    GUIIM_INIT_KERNEL_T* param_ptr
);

/*****************************************************************************/
//  Description : 析构输入法
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Terminate(
    void* handle_ptr
);

/*****************************************************************************/
//  Description : 输入法的处理函数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void* handle_ptr,
    GUIIM_EVENT_T event,
    GUIIM_EVENT_DATA_U* event_data_ptr
);

/******************************************************************************/
// Description: 设置参数
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void SetOptionEx(
    void *handle
);

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr
);

/*****************************************************************************/
//  Description : 外部消息转化为内部消息类型
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TransMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_TP_CSTAR_MSG_T *msg_ptr
);

/*****************************************************************************/
//  Description : 在无输入状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : 在输入状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : 在联想状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : 在自造词状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgUserWord(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : TpDownProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpDownProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpMoveProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpMoveProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpUpNoInputProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpUpNoInputProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpUpInputingProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpUpInputingProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpUpAssociateProcess
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 屏上有内容时，up处理
/*****************************************************************************/
LOCAL BOOLEAN TpUpAssociateProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpUpUserWordProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpUpUserWordProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpLongProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpLongProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : 由cstar返回的内容设置当前UI状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetUIStateByContent(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    CS_IMM_CONTENT* content_ptr
);

/*****************************************************************************/
//  Description : 处理cstar事件
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleCstarEvent(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    CS_UINT hEvent,
    CS_UINT iMsg
);

/*****************************************************************************/
//  Description : 触笔点击候选筐操作
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: simulator 是否是由键盘模拟TP按在了候选区
/*****************************************************************************/
LOCAL void TpCandPressUp(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUI_POINT_T tp_point,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    BOOLEAN is_simulator
);

/*****************************************************************************/
//  Description : 当触笔在choice bar上up，对应处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpUpOnChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx,
    BOOLEAN *dirty_flg_ptr
);

/*****************************************************************************/
//  Description : 模拟混淆区的拼音翻页功能
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 内核引擎只能一个一个翻，我们自己来模拟翻页
/*****************************************************************************/
LOCAL BOOLEAN HandleChoicePageSimulator(
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr,
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx
);

/*****************************************************************************/
//  Description : 设置输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 依据cstar输入法及是否全键盘给引擎设置正确方法,state用于更新当前状态
/*****************************************************************************/
LOCAL void SetMethod(
    MMIIM_TP_CSTAR_HANDLE_T* andle_ptr,
    GUIIM_METHOD_T method,
    MMIIM_TP_CSTAR_STATE_ID_T kb_state
);

/*****************************************************************************/
//  Description : 把外部的输入法项转换成内部的状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: cstar不管9/26，内部键盘状态关心9/26, 内部输入法 < cstar输入法*2
/*****************************************************************************/
LOCAL uint32 ConvertMethodtoState(
    GUIIM_METHOD_T method,
    BOOLEAN is_full_keypad
);

/******************************************************************************/
// Description: get text width call back
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL int GetTextWidthCallback(
    const unsigned short* text,
    int length
);

/******************************************************************************/
// Description: checking can displaying char or not call back
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL int IsCanDisplayChar(wchar ch);

/******************************************************************************/
// Description: 处理TP消息
// Global resource dependence: NONE
// Author:
// Note:
/******************************************************************************/
LOCAL void HandleTpKeySelect(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    uint16 tp_index
);

/*==============================================================================
Description: SetToNoInput
Global resource dependence: NONE
Author: haiwu.chen
Note:
==============================================================================*/
LOCAL void SetToNoInput(MMIIM_TP_CSTAR_HANDLE_T *data_ptr);

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitImKernel(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    CS_HIMM *init_Ker_ptr
);

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

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
static uint8 s_im_reload_cnt = 0;           //用于计数输入法进入次数
static BOOLEAN s_is_im_reload = FALSE;      //用于计数输入法进入次数

#if defined (KEYPAD_TYPE_QWERTY_KEYPAD) || defined(TOUCH_PANEL_SUPPORT)
extern const CS_QWERTY_INFO cs_qwerty_info[];
#endif

//guiim使用的函数表
GUIIM_KERNEL_APIS_T const g_guiim_tp_cstar_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

extern const uint16 s_sym_del[];
extern const uint16 s_sym_space[];

extern MMIIM_TP_CSTAR_STATE_T g_keyboard_state[MMIIM_TP_CSTAR_KB_STATE_MAX];
	
static MMIIM_CSTAR_METHOD_RELATION_T const g_tp_cstar_method_relation_list[] =
{        
    //TP 9键
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
    {GUIIM_M_TP_PINYIN,                        IM_PINYIN_DZ},
#else
    {GUIIM_M_TP_PINYIN,                        IM_PINYIN},
#endif
    {GUIIM_M_TP_STROKE,                     IM_STROKE},
    {GUIIM_M_TP_ZHUYIN,                     IM_ZHUYIN_MT},
    {GUIIM_M_TP_STROKE_CHT,                 IM_STROKE_BIG5},
    {GUIIM_M_TP_123,                        IM_123},
    {GUIIM_M_TP_ENGLISH_LEAD_UPPER,         IM_ENGLISH_LEAD_UPPER},
    {GUIIM_M_TP_ENGLISH_LOWER,              IM_ENGLISH_LOWER},
    {GUIIM_M_TP_ENGLISH_UPPER,              IM_ENGLISH_UPPER},
    {GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER,     IM_ABC_LEAD_UPPER},
    {GUIIM_M_TP_ENGLISH_ABC_LOWER,          IM_ABC_LOWER},
    {GUIIM_M_TP_ENGLISH_ABC_UPPER,          IM_ABC_UPPER},
    {GUIIM_M_TP_ARABIC,                     IM_ARABIC},
    {GUIIM_M_TP_FRENCH_LEAD_UPPER,          IM_FRENCH_LEAD_UPPER},
    {GUIIM_M_TP_FRENCH_LOWER,               IM_FRENCH_LOWER},
    {GUIIM_M_TP_FRENCH_UPPER,               IM_FRENCH_UPPER},
    {GUIIM_M_TP_GERMAN_LEAD_UPPER,          IM_GERMAN_LEAD_UPPER},
    {GUIIM_M_TP_GERMAN_LOWER,               IM_GERMAN_LOWER},
    {GUIIM_M_TP_GERMAN_UPPER,               IM_GERMAN_UPPER},
    {GUIIM_M_TP_GREEK_LEAD_UPPER,           IM_GREEK_LEAD_UPPER},
    {GUIIM_M_TP_GREEK_LOWER,                IM_GREEK_LOWER},
    {GUIIM_M_TP_GREEK_UPPER,                IM_GREEK_UPPER},
    {GUIIM_M_TP_HINDI,                      IM_HINDI},
    {GUIIM_M_TP_HUNGARIAN_LEAD_UPPER,       IM_HUNGARIAN_LEAD_UPPER},
    {GUIIM_M_TP_HUNGARIAN_LOWER,            IM_HUNGARIAN_LOWER},
    {GUIIM_M_TP_HUNGARIAN_UPPER,            IM_HUNGARIAN_UPPER},
    {GUIIM_M_TP_INDONESIAN_LEAD_UPPER,      IM_INDONESIAN_LEAD_UPPER},
    {GUIIM_M_TP_INDONESIAN_LOWER,           IM_INDONESIAN_LOWER},
    {GUIIM_M_TP_INDONESIAN_UPPER,           IM_INDONESIAN_UPPER},
    {GUIIM_M_TP_ITALIAN_LEAD_UPPER,         IM_ITALIAN_LEAD_UPPER},
    {GUIIM_M_TP_ITALIAN_LOWER,              IM_ITALIAN_LOWER},
    {GUIIM_M_TP_ITALIAN_UPPER,              IM_ITALIAN_UPPER},
    {GUIIM_M_TP_MALAY_LEAD_UPPER,           IM_MALAY_LEAD_UPPER},
    {GUIIM_M_TP_MALAY_LOWER,                IM_MALAY_LOWER},
    {GUIIM_M_TP_MALAY_UPPER,                IM_MALAY_UPPER},
    {GUIIM_M_TP_PERSIAN,                    IM_PERSIAN},
    {GUIIM_M_TP_PORTUGUESE_LEAD_UPPER,      IM_PORTUGUESE_LEAD_UPPER},
    {GUIIM_M_TP_PORTUGUESE_LOWER,           IM_PORTUGUESE_LOWER},
    {GUIIM_M_TP_PORTUGUESE_UPPER,           IM_PORTUGUESE_UPPER},
    {GUIIM_M_TP_RUSSIAN_LEAD_UPPER,         IM_RUSSIAN_LEAD_UPPER},
    {GUIIM_M_TP_RUSSIAN_LOWER,              IM_RUSSIAN_LOWER},
    {GUIIM_M_TP_RUSSIAN_UPPER,              IM_RUSSIAN_UPPER},
    {GUIIM_M_TP_SPANISH_LEAD_UPPER,         IM_SPANISH_LEAD_UPPER},
    {GUIIM_M_TP_SPANISH_LOWER,              IM_SPANISH_LOWER},
    {GUIIM_M_TP_SPANISH_UPPER,              IM_SPANISH_UPPER},
    {GUIIM_M_TP_TAGALOG,                    IM_TAGALOG},
    {GUIIM_M_TP_THAI,                       IM_THAI},
    {GUIIM_M_TP_TURKISH_LEAD_UPPER,         IM_TURKISH_LEAD_UPPER},
    {GUIIM_M_TP_TURKISH_LOWER,              IM_TURKISH_LOWER},
    {GUIIM_M_TP_TURKISH_UPPER,              IM_TURKISH_UPPER},
    {GUIIM_M_TP_URDU,                       IM_URDU},
    {GUIIM_M_TP_HEBREW,                     IM_HEBREW},
    {GUIIM_M_TP_BENGALI,                    IM_BENGALI},
    {GUIIM_M_TP_VIETNAMESE_LEAD_UPPER,      IM_VIETNAMESE_LEAD_UPPER},
    {GUIIM_M_TP_VIETNAMESE_LOWER,           IM_VIETNAMESE_LOWER},
    {GUIIM_M_TP_VIETNAMESE_UPPER,           IM_VIETNAMESE_UPPER},
    {GUIIM_M_TP_CZECH_LEAD_UPPER,           IM_CZECH_LEAD_UPPER},
    {GUIIM_M_TP_CZECH_LOWER,                IM_CZECH_LOWER},
    {GUIIM_M_TP_CZECH_UPPER,                IM_CZECH_UPPER},
    {GUIIM_M_TP_SLOVENIAN_LEAD_UPPER,       IM_SLOVENIAN_LEAD_UPPER},
    {GUIIM_M_TP_SLOVENIAN_LOWER,            IM_SLOVENIAN_LOWER},
    {GUIIM_M_TP_SLOVENIAN_UPPER,            IM_SLOVENIAN_UPPER},
    {GUIIM_M_TP_ROMANIAN_LEAD_UPPER,        IM_ROMANIAN_LEAD_UPPER},
    {GUIIM_M_TP_ROMANIAN_LOWER,             IM_ROMANIAN_LOWER},
    {GUIIM_M_TP_ROMANIAN_UPPER,             IM_ROMANIAN_UPPER},
    {GUIIM_M_TP_CROATIAN_LEAD_UPPER,        IM_CROATIAN_LEAD_UPPER},
    {GUIIM_M_TP_CROATIAN_LOWER,             IM_CROATIAN_LOWER},
    {GUIIM_M_TP_CROATIAN_UPPER,             IM_CROATIAN_UPPER},
    {GUIIM_M_TP_DUTCH_LEAD_UPPER,           IM_DUTCH_LEAD_UPPER},
    {GUIIM_M_TP_DUTCH_LOWER,                IM_DUTCH_LOWER},
    {GUIIM_M_TP_DUTCH_UPPER,                IM_DUTCH_UPPER},
    {GUIIM_M_TP_BULGARIAN_LEAD_UPPER,       IM_BULGARIAN_LEAD_UPPER},
    {GUIIM_M_TP_BULGARIAN_LOWER,            IM_BULGARIAN_LOWER},
    {GUIIM_M_TP_BULGARIAN_UPPER,            IM_BULGARIAN_UPPER},
    {GUIIM_M_TP_POLISH_LEAD_UPPER,          IM_POLISH_LEAD_UPPER},
    {GUIIM_M_TP_POLISH_LOWER,               IM_POLISH_LOWER},
    {GUIIM_M_TP_POLISH_UPPER,               IM_POLISH_UPPER},
    {GUIIM_M_TP_DANISH_LEAD_UPPER,          IM_DANISH_LEAD_UPPER},
    {GUIIM_M_TP_DANISH_LOWER,               IM_DANISH_LOWER},
    {GUIIM_M_TP_DANISH_UPPER,               IM_DANISH_UPPER},
    {GUIIM_M_TP_SLOVAK_LEAD_UPPER,          IM_SLOVAK_LEAD_UPPER},
    {GUIIM_M_TP_SLOVAK_LOWER,               IM_SLOVAK_LOWER},
    {GUIIM_M_TP_SLOVAK_UPPER,               IM_SLOVAK_UPPER},
    {GUIIM_M_TP_LITHUANIAN_LEAD_UPPER,      IM_LITHUANIAN_LEAD_UPPER},
    {GUIIM_M_TP_LITHUANIAN_LOWER,           IM_LITHUANIAN_LOWER},
    {GUIIM_M_TP_LITHUANIAN_UPPER,           IM_LITHUANIAN_UPPER},
    {GUIIM_M_TP_NORWEGIAN_LEAD_UPPER,       IM_NORWEGIAN_LEAD_UPPER},
    {GUIIM_M_TP_NORWEGIAN_LOWER,            IM_NORWEGIAN_LOWER},
    {GUIIM_M_TP_NORWEGIAN_UPPER,            IM_NORWEGIAN_UPPER},
    {GUIIM_M_TP_SWEDISH_LEAD_UPPER,         IM_SWEDISH_LEAD_UPPER},
    {GUIIM_M_TP_SWEDISH_LOWER,              IM_SWEDISH_LOWER},
    {GUIIM_M_TP_SWEDISH_UPPER,              IM_SWEDISH_UPPER},
    {GUIIM_M_TP_UKRAINIAN_LEAD_UPPER,       IM_UKRAINIAN_LEAD_UPPER},
    {GUIIM_M_TP_UKRAINIAN_LOWER,            IM_UKRAINIAN_LOWER},
    {GUIIM_M_TP_UKRAINIAN_UPPER,            IM_UKRAINIAN_UPPER},
    {GUIIM_M_TP_SWAHILI_LEAD_UPPER,         IM_SWAHILI_LEAD_UPPER},
    {GUIIM_M_TP_SWAHILI_LOWER,              IM_SWAHILI_LOWER},
    {GUIIM_M_TP_SWAHILI_UPPER,              IM_SWAHILI_UPPER},
    {GUIIM_M_TP_FINNISH_LEAD_UPPER,         IM_FINNISH_LEAD_UPPER},
    {GUIIM_M_TP_FINNISH_LOWER,              IM_FINNISH_LOWER},
    {GUIIM_M_TP_FINNISH_UPPER,              IM_FINNISH_UPPER},

    //tp 26键
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
    {GUIIM_M_TPQ_PINYIN,                    IM_PINYIN_DZ},
#else
    {GUIIM_M_TPQ_PINYIN,                    IM_PINYIN},
#endif
    {GUIIM_M_TPQ_STROKE,                    IM_STROKE},
    {GUIIM_M_TPQ_ZHUYIN,                    IM_ZHUYIN_MT},
    {GUIIM_M_TPQ_STROKE_CHT,                IM_STROKE_BIG5},
    {GUIIM_M_TPQ_123,                       IM_123},
    {GUIIM_M_TPQ_ENGLISH_LEAD_UPPER,        IM_ENGLISH_LEAD_UPPER},
    {GUIIM_M_TPQ_ENGLISH_LOWER,             IM_ENGLISH_LOWER},
    {GUIIM_M_TPQ_ENGLISH_UPPER,             IM_ENGLISH_UPPER},
    {GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER,    IM_ABC_LEAD_UPPER},
    {GUIIM_M_TPQ_ENGLISH_ABC_LOWER,         IM_ABC_LOWER},
    {GUIIM_M_TPQ_ENGLISH_ABC_UPPER,         IM_ABC_UPPER},
    {GUIIM_M_TPQ_ARABIC,                    IM_ARABIC},
    {GUIIM_M_TPQ_FRENCH_LEAD_UPPER,         IM_FRENCH_LEAD_UPPER},
    {GUIIM_M_TPQ_FRENCH_LOWER,              IM_FRENCH_LOWER},
    {GUIIM_M_TPQ_FRENCH_UPPER,              IM_FRENCH_UPPER},
    {GUIIM_M_TPQ_GERMAN_LEAD_UPPER,         IM_GERMAN_LEAD_UPPER},
    {GUIIM_M_TPQ_GERMAN_LOWER,              IM_GERMAN_LOWER},
    {GUIIM_M_TPQ_GERMAN_UPPER,              IM_GERMAN_UPPER},
    {GUIIM_M_TPQ_GREEK_LEAD_UPPER,          IM_GREEK_LEAD_UPPER},
    {GUIIM_M_TPQ_GREEK_LOWER,               IM_GREEK_LOWER},
    {GUIIM_M_TPQ_GREEK_UPPER,               IM_GREEK_UPPER},
    {GUIIM_M_TPQ_HINDI,                     IM_HINDI},
    {GUIIM_M_TPQ_HUNGARIAN_LEAD_UPPER,      IM_HUNGARIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_HUNGARIAN_LOWER,           IM_HUNGARIAN_LOWER},
    {GUIIM_M_TPQ_HUNGARIAN_UPPER,           IM_HUNGARIAN_UPPER},
    {GUIIM_M_TPQ_INDONESIAN_LEAD_UPPER,     IM_INDONESIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_INDONESIAN_LOWER,          IM_INDONESIAN_LOWER},
    {GUIIM_M_TPQ_INDONESIAN_UPPER,          IM_INDONESIAN_UPPER},
    {GUIIM_M_TPQ_ITALIAN_LEAD_UPPER,        IM_ITALIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_ITALIAN_LOWER,             IM_ITALIAN_LOWER},
    {GUIIM_M_TPQ_ITALIAN_UPPER,             IM_ITALIAN_UPPER},
    {GUIIM_M_TPQ_MALAY_LEAD_UPPER,          IM_MALAY_LEAD_UPPER},
    {GUIIM_M_TPQ_MALAY_LOWER,               IM_MALAY_LOWER},
    {GUIIM_M_TPQ_MALAY_UPPER,               IM_MALAY_UPPER},
    {GUIIM_M_TPQ_PERSIAN,                   IM_PERSIAN},
    {GUIIM_M_TPQ_PORTUGUESE_LEAD_UPPER,     IM_PORTUGUESE_LEAD_UPPER},
    {GUIIM_M_TPQ_PORTUGUESE_LOWER,          IM_PORTUGUESE_LOWER},
    {GUIIM_M_TPQ_PORTUGUESE_UPPER,          IM_PORTUGUESE_UPPER},
    {GUIIM_M_TPQ_RUSSIAN_LEAD_UPPER,        IM_RUSSIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_RUSSIAN_LOWER,             IM_RUSSIAN_LOWER},
    {GUIIM_M_TPQ_RUSSIAN_UPPER,             IM_RUSSIAN_UPPER},
    {GUIIM_M_TPQ_SPANISH_LEAD_UPPER,        IM_SPANISH_LEAD_UPPER},
    {GUIIM_M_TPQ_SPANISH_LOWER,             IM_SPANISH_LOWER},
    {GUIIM_M_TPQ_SPANISH_UPPER,             IM_SPANISH_UPPER},
    {GUIIM_M_TPQ_TAGALOG,                   IM_TAGALOG},
    {GUIIM_M_TPQ_THAI,                      IM_THAI},
    {GUIIM_M_TPQ_TURKISH_LEAD_UPPER,        IM_TURKISH_LEAD_UPPER},
    {GUIIM_M_TPQ_TURKISH_LOWER,             IM_TURKISH_LOWER},
    {GUIIM_M_TPQ_TURKISH_UPPER,             IM_TURKISH_UPPER},
    {GUIIM_M_TPQ_URDU,                      IM_URDU},
    {GUIIM_M_TPQ_HEBREW,                    IM_HEBREW},
    {GUIIM_M_TPQ_BENGALI,                   IM_BENGALI},
    {GUIIM_M_TPQ_VIETNAMESE_LEAD_UPPER,     IM_VIETNAMESE_LEAD_UPPER},
    {GUIIM_M_TPQ_VIETNAMESE_LOWER,          IM_VIETNAMESE_LOWER},
    {GUIIM_M_TPQ_VIETNAMESE_UPPER,          IM_VIETNAMESE_UPPER},
    {GUIIM_M_TPQ_CZECH_LEAD_UPPER,          IM_CZECH_LEAD_UPPER},
    {GUIIM_M_TPQ_CZECH_LOWER,               IM_CZECH_LOWER},
    {GUIIM_M_TPQ_CZECH_UPPER,               IM_CZECH_UPPER},
    {GUIIM_M_TPQ_SLOVENIAN_LEAD_UPPER,      IM_SLOVENIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_SLOVENIAN_LOWER,           IM_SLOVENIAN_LOWER},
    {GUIIM_M_TPQ_SLOVENIAN_UPPER,           IM_SLOVENIAN_UPPER},
    {GUIIM_M_TPQ_ROMANIAN_LEAD_UPPER,       IM_ROMANIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_ROMANIAN_LOWER,            IM_ROMANIAN_LOWER},
    {GUIIM_M_TPQ_ROMANIAN_UPPER,            IM_ROMANIAN_UPPER},
    {GUIIM_M_TPQ_CROATIAN_LEAD_UPPER,       IM_CROATIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_CROATIAN_LOWER,            IM_CROATIAN_LOWER},
    {GUIIM_M_TPQ_CROATIAN_UPPER,            IM_CROATIAN_UPPER},
    {GUIIM_M_TPQ_DUTCH_LEAD_UPPER,          IM_DUTCH_LEAD_UPPER},
    {GUIIM_M_TPQ_DUTCH_LOWER,               IM_DUTCH_LOWER},
    {GUIIM_M_TPQ_DUTCH_UPPER,               IM_DUTCH_UPPER},
    {GUIIM_M_TPQ_BULGARIAN_LEAD_UPPER,      IM_BULGARIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_BULGARIAN_LOWER,           IM_BULGARIAN_LOWER},
    {GUIIM_M_TPQ_BULGARIAN_UPPER,           IM_BULGARIAN_UPPER},
    {GUIIM_M_TPQ_POLISH_LEAD_UPPER,         IM_POLISH_LEAD_UPPER},
    {GUIIM_M_TPQ_POLISH_LOWER,              IM_POLISH_LOWER},
    {GUIIM_M_TPQ_POLISH_UPPER,              IM_POLISH_UPPER},
    {GUIIM_M_TPQ_DANISH_LEAD_UPPER,         IM_DANISH_LEAD_UPPER},
    {GUIIM_M_TPQ_DANISH_LOWER,              IM_DANISH_LOWER},
    {GUIIM_M_TPQ_DANISH_UPPER,              IM_DANISH_UPPER},
    {GUIIM_M_TPQ_SLOVAK_LEAD_UPPER,         IM_SLOVAK_LEAD_UPPER},
    {GUIIM_M_TPQ_SLOVAK_LOWER,              IM_SLOVAK_LOWER},
    {GUIIM_M_TPQ_SLOVAK_UPPER,              IM_SLOVAK_UPPER}, 
    {GUIIM_M_TPQ_LITHUANIAN_LEAD_UPPER,     IM_LITHUANIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_LITHUANIAN_LOWER,          IM_LITHUANIAN_LOWER},
    {GUIIM_M_TPQ_LITHUANIAN_UPPER,          IM_LITHUANIAN_UPPER},
    {GUIIM_M_TPQ_NORWEGIAN_LEAD_UPPER,      IM_NORWEGIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_NORWEGIAN_LOWER,           IM_NORWEGIAN_LOWER},
    {GUIIM_M_TPQ_NORWEGIAN_UPPER,           IM_NORWEGIAN_UPPER},
    {GUIIM_M_TPQ_SWEDISH_LEAD_UPPER,        IM_SWEDISH_LEAD_UPPER},
    {GUIIM_M_TPQ_SWEDISH_LOWER,             IM_SWEDISH_LOWER},
    {GUIIM_M_TPQ_SWEDISH_UPPER,             IM_SWEDISH_UPPER},
    {GUIIM_M_TPQ_UKRAINIAN_LEAD_UPPER,      IM_UKRAINIAN_LEAD_UPPER},
    {GUIIM_M_TPQ_UKRAINIAN_LOWER,           IM_UKRAINIAN_LOWER},
    {GUIIM_M_TPQ_UKRAINIAN_UPPER,           IM_UKRAINIAN_UPPER},
    {GUIIM_M_TPQ_SWAHILI_LEAD_UPPER,        IM_SWAHILI_LEAD_UPPER},
    {GUIIM_M_TPQ_SWAHILI_LOWER,             IM_SWAHILI_LOWER},
    {GUIIM_M_TPQ_SWAHILI_UPPER,             IM_SWAHILI_UPPER},
    {GUIIM_M_TPQ_FINNISH_LEAD_UPPER,        IM_FINNISH_LEAD_UPPER},
    {GUIIM_M_TPQ_FINNISH_LOWER,             IM_FINNISH_LOWER},
    {GUIIM_M_TPQ_FINNISH_UPPER,             IM_FINNISH_UPPER},
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 初始化输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T* param_ptr)
{
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr = PNULL;
    CS_HIMM init_result = PNULL;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = 0;                     //应用程序键盘状态

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param_ptr->lcd_info_ptr); /*assert verified*/

    if(PNULL == param_ptr ||PNULL == param_ptr->lcd_info_ptr )
    {
        return PNULL;
    }
    
    //SCI_TRACE_LOW:"[MMIIM] Initialize tpcstar: win_id=%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_616_112_2_18_2_28_18_200,(uint8*)"d", param_ptr->win_id);

    //应用句柄各参数初始化，不初始化的参数即messet为0
    handle_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) SCI_ALLOC_APP(sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    if (PNULL == handle_ptr)
        return PNULL;

    SCI_MEMSET(handle_ptr, 0, sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    //cstar初始化
    handle_ptr->ctrl_handle = param_ptr->im_handle;
    handle_ptr->lcd_info_ptr = param_ptr->lcd_info_ptr;
    handle_ptr->init_param = *param_ptr;

    //创建tips层
    MMIIMUI_CreateTipsLayer(handle_ptr, param_ptr);

    handle_ptr->stateId = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD - MMIIM_TP_CSTAR_KB_START_ID;           //重要参数，初始化默认为手写
    handle_ptr->is_full_keypad = TRUE;
    handle_ptr->is_need_lock = FALSE;
    handle_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    //kernel
    InitImKernel(handle_ptr, &init_result);    

    if(PNULL != init_result)
    {
        handle_ptr->cstar_handle = init_result;

        //设置option
        SetOptionEx(handle_ptr->cstar_handle);

        //根据当前输入法核及handle中是否全键盘开关获取当前键盘状态
        if (MMITHEME_IsMainScreenLandscape()) //横屏的话，立马改成26键
        {
            handle_ptr->is_full_keypad = TRUE;
        }
        else if (GUIIM_INPUT_TYPE_TOUCHPEN == handle_ptr->init_param.method_param_ptr->im_set.input)
        {
            handle_ptr->is_full_keypad = FALSE;
        }
        
        kb_state = MMIIMUI_GetVKboardState(handle_ptr->init_param.method_param_ptr->im_set.method, handle_ptr->is_full_keypad);

        //设置默认的状态
        SetMethod(handle_ptr, handle_ptr->init_param.method_param_ptr->im_set.method, kb_state);

        //设置数
        MMIIMUI_InitDisplayData(handle_ptr);

        //根据当前键盘状态刷图
        //应用程序接着会收到paint消息，所有在paint中做
        
        AddImReloadRefCnt();
    }
    else
    {
        SCI_FREE(handle_ptr);
        return PNULL;
    }

#if 1   //def MMIIM_VIRTUAL_KB_TEXT_SUPPORT     //强制打开，部分程序宏未完全控制，不打开，将导致虚拟键盘背景图为空白
	//为空表示未配置
	//默认配置，默认配置需要重写，因切换语言可能修改该值，而初始化配置不需要该值
	if(g_keyboard_state[handle_ptr->stateId].p_disp_text == NULL		
		|| g_keyboard_state[handle_ptr->stateId].p_disp_text == MMIIMUI_MulKb26KeyGetDefData()      //lint !e746
		|| g_keyboard_state[handle_ptr->stateId].p_disp_text == MMIIMUI_MulKb9KeyGetDefData())	        //lint !e746
	{
		if(handle_ptr->is_full_keypad)
		{
			g_keyboard_state[handle_ptr->stateId].p_disp_text = MMIIMUI_MulKb26KeyGetDefData();
			MMIIMUI_SetSmartKb26KeyTextData(handle_ptr->init_param.method_param_ptr->im_set.method, 
				g_keyboard_state[handle_ptr->stateId].p_disp_text );
		}
		else
		{
			g_keyboard_state[handle_ptr->stateId].p_disp_text = MMIIMUI_MulKb9KeyGetDefData();
			MMIIMUI_SetSmartKb9KeyTextData(handle_ptr->init_param.method_param_ptr->im_set.method, 
				g_keyboard_state[handle_ptr->stateId].p_disp_text );
		}
	}
#endif	

    return (void*) handle_ptr;
}

/*****************************************************************************/
//  Description : 析构输入法
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Terminate(void* handle_ptr)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;

    if(data_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIIM] Terminate tpcstar: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_713_112_2_18_2_28_18_201,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIIM] Terminate tpcstar: cs_handle=%08X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_717_112_2_18_2_28_18_202,(uint8*)"d", data_ptr->cstar_handle);

    if (PNULL != data_ptr->cstar_handle)
    {
        CS_IMM_Terminate(data_ptr->cstar_handle);

        ReleaseImReloadRefCnt();
        if (0 != GetImReloadCnt())
        {
            //此时置脏
            SetImReloadDirty(TRUE);
        }
    }

    /*这个检查是有点多余的，如果以后横屏也支持9键的话，那就不必此条件，现在的逻辑就是横屏不保存也不修改9，26状态，均设置为26键*/
    if (!MMITHEME_IsMainScreenLandscape())
    {
        MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);
    }

    if (0 != data_ptr->tips_timer_id)
    {
        //SCI_TRACE_LOW:"[MMIIM] Terminate tpcstar: stop timer id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_739_112_2_18_2_28_18_203,(uint8*)"d", data_ptr->tips_timer_id);
        
        MMK_StopTimer(data_ptr->tips_timer_id);
        data_ptr->tips_timer_id = 0;
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        //SCI_TRACE_LOW:"[MMIIM] Terminate tpcstar: release layer."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_747_112_2_18_2_28_18_204,(uint8*)"");
        
        UILAYER_RELEASELAYER(&(data_ptr->tips_lcd_info));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(data_ptr->tips_lcd_info));
//        data_ptr->tips_lcd_info.block_id = UILAYER_NULL_HANDLE;
    }

    SCI_FREE(data_ptr);
    handle_ptr = PNULL;

    return TRUE;
}

/*****************************************************************************/
//  Description : 输入法的处理函数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void *handle_ptr,                   //in :输入法句柄
    GUIIM_EVENT_T event,                //in :输入事件
    GUIIM_EVENT_DATA_U *event_data_ptr  //in&out: 输入和输出参数
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    BOOLEAN result = FALSE;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD;

    if (PNULL == data_ptr
        || data_ptr->stateId >= MMIIM_TP_CSTAR_KB_STATE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleMsg: data_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_777_112_2_18_2_28_18_205,(uint8*)"");
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"[MMIIM] HandleMsg:event:%d,stateId:%d,stateId_bak:%d,stateId_bakii:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_782_112_2_18_2_28_18_206,(uint8*)"dddd", event, data_ptr->stateId, data_ptr->bak_stateId, data_ptr->bak_stateIdii);
    
    data_ptr->event_data_ptr = event_data_ptr;    

    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:
        {
            result = HandleSysMsg(data_ptr, event_data_ptr);
        }
        break;

    case GUIIM_EVENT_PAINT:
        {
            MMIIMUI_DrawAllBoard(data_ptr);
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_SLEEP:
        {
            //sleep时，应用也会被guiim销毁
            MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);
            result = TRUE;
        }
        break;          

    case GUIIM_EVENT_SET_METHOD:
        {
            kb_state = MMIIMUI_GetVKboardState(event_data_ptr->set_method, data_ptr->is_full_keypad);
            SetMethod(data_ptr, event_data_ptr->set_method, kb_state);
            MMIIMUI_InitDisplayData(data_ptr);
            MMIIMUI_DrawAllBoard(data_ptr);
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_METHOD:
        {
            if(PNULL != data_ptr->init_param.method_param_ptr)
            {
                event_data_ptr->get_method = data_ptr->init_param.method_param_ptr->im_set.method;
                result = TRUE;
            }
        }
        break;

    case GUIIM_EVENT_GET_RECT:
        {
            MMIIMUI_GetRect(data_ptr, &(event_data_ptr->get_rect));
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:
        {
            if (MMIIM_TP_CSTAR_STATE_USER == data_ptr->ui_state)
            {
                if (0 == data_ptr->nUserWord)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SELECTING;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
            }            
            else if (data_ptr->nPreEdit > 0)
            {
                event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
            }
            else if (data_ptr->result_len > 0)
            {
                event_data_ptr->edit_state = GUIIM_EDIT_CANDITATE;
            }
            else
            {
                event_data_ptr->edit_state = GUIIM_EDIT_NONE;
            }

            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_CTRL_RECT: //接收触笔响应的区域
        {
            MMIIMUI_GetRect(data_ptr, &(event_data_ptr->get_ctrl_rect));
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_CUSTOM_KEY:
        {
            result = TRUE;
        }
        break;

    default:
        break;

    }

    return result;
}

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    BOOLEAN result = TRUE;
    BOOLEAN is_dealt = FALSE;
    MMIIM_TP_CSTAR_MSG_T msg = {0};

    if (PNULL == data_ptr
        || data_ptr->stateId >= MMIIM_TP_CSTAR_KB_STATE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleSysMsg: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_904_112_2_18_2_28_18_207,(uint8*)"");
        return FALSE;
    }

    //添加一个定时器处理消息，解决用户自造词状态，误认为连续按键问题
    if(event_data_ptr->sys_msg.msg_id == MSG_TIMER)
    {
        if(MMIIM_EXPL_TIMER_ID == *(uint8*)(event_data_ptr->sys_msg.param))
        {
            CS_IMM_CONTENT content = {0};
            CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_TIMER, 0, &content);
        }

        return TRUE;
    }

    if (MSG_LCD_SWITCH == event_data_ptr->sys_msg.msg_id
        && MMIIM_TP_CSTAR_STATE_DIGSYM == data_ptr->stateId + MMIIM_TP_CSTAR_KB_START_ID)
    {
        //横竖屏切换时，lock信息丢掉
        handle_ptr->is_need_lock = FALSE;

        MMIIMUI_InitDisplayData(data_ptr);
        
        return TRUE;
    }

    //如果有tips时，此时来电
    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        MMIIMUI_HideKeyTips(data_ptr);

        if (0 != data_ptr->tips_timer_id)
        {
            MMK_StopTimer(data_ptr->tips_timer_id);
            data_ptr->tips_timer_id = 0;
        }
        
        return FALSE;
    }

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        //发生了重入现象，需要重置一下输入法
        if (IsImReloadDirty())
        {
            CS_HIMM init_result = PNULL;
            SetImReloadDirty(FALSE);
            
            //kernel
            InitImKernel(handle_ptr, &init_result);    
            
            if(PNULL != init_result)
            {
                handle_ptr->cstar_handle = init_result;
                
                //设置option
                SetOptionEx(handle_ptr->cstar_handle);
                
                //设置默认的状态
                SetMethod(handle_ptr, handle_ptr->init_param.method_param_ptr->im_set.method, 
                    handle_ptr->stateId);
                
                //SCI_TRACE_LOW:"[MMIIM] HandleSysMsg: IsImReloadDirty, method=%04X, stateId=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_968_112_2_18_2_28_18_208,(uint8*)"dd", handle_ptr->init_param.method_param_ptr->im_set.method, handle_ptr->stateId);
            }
            
            return TRUE;
        }
    }

    
    //把外部消息转为内部消息，检查是down,up,long,repeat消息
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回，无此消息类型，直接return
    if (MMIIM_TP_CSTAR_MSG_NONE == msg.id)
    {
        return FALSE;
    }   

    switch (data_ptr->ui_state)
    {
    case MMIIM_TP_CSTAR_STATE_NOINPUT:
        is_dealt = HandleSysMsgNoInput(data_ptr, &msg);
        break;

    case MMIIM_TP_CSTAR_STATE_INPUTING:
        is_dealt = HandleSysMsgInputing(data_ptr, &msg);
        break;

    case MMIIM_TP_CSTAR_STATE_ASSOCIATE:
        is_dealt = HandleSysMsgAssociate(data_ptr, &msg);
        break;

    case MMIIM_TP_CSTAR_STATE_USER:
        is_dealt = HandleSysMsgUserWord(data_ptr, &msg);
        break;

    default:
        break;
    }

    if (!is_dealt)
    {
        return FALSE;
    }

    result = is_dealt;

    if (MMIIMUI_ChangeRect(data_ptr))
    {
        return result;
    }

    return result;
}

/******************************************************************************/
// Description: 设置参数
// Global resource dependence: NONE
// Author: haiwu.chen
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
    option.dwPYFuzzy = (int16)MMIIM_GetPyFuzzy();
#if defined (KEYPAD_TYPE_QWERTY_KEYPAD) || defined(TOUCH_PANEL_SUPPORT)
    option.pQwertyInfo = cs_qwerty_info;
#else
    option.pQwertyInfo = PNULL;
#endif
    option.dwStrokeLayout = CS_STROKE_STANDARD_LAYOUT;
    option.dwAssoPinyin = CS_ASSOCIATION_ON;
    option.dwAssoStroke = CS_ASSOCIATION_ON;
    option.dwPinyinMode = CS_PINYIN_MODE_3;//HandleChoicePageSimulator与此模式有依赖关系
    option.dwCandidateMode = CS_CANDIDATE_MODE_1;
    option.dwAlphaBeticInputMode = CS_ALPHABETIC_WORD_INPUT;

    CS_IMM_SetOptionsEx(handle, &option);

    return;
}

/*****************************************************************************/
//  Description : 外部消息转化为内部消息类型
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TransMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_TP_CSTAR_MSG_T *msg_ptr
)
{
    if(PNULL == data_ptr
        || PNULL == event_data_ptr
        || PNULL == msg_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TransMsg: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_1068_112_2_18_2_28_19_209,(uint8*)"");
        return;
    }

    MMIIMUI_TransMsg(data_ptr, event_data_ptr, msg_ptr);
}

/*****************************************************************************/
//  Description : 在无输入状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
)
{
    BOOLEAN is_dealt = FALSE;

    //SCI_ASSERT(PNULL != data_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);/*assert verified*/
    if(PNULL == data_ptr ||PNULL == msg_ptr)
    {
        return FALSE;
    }
    
    //处理消息
    switch (msg_ptr->id)
    {
    case MMIIM_TP_CSTAR_MSG_TPDOWN:
        is_dealt = TpDownProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPMOVE:
        is_dealt = TpMoveProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPUP:
        is_dealt = TpUpNoInputProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPLONG:
        is_dealt = TpLongProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_KEY_LONG_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_CANCEL:
                {
                    GUIIM_NOTIFY_PACK_T notify_pack = {0};
                    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : 在输入状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    GUI_POINT_T tp_point = {0};
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);/*assert verified*/
    if(PNULL == data_ptr ||PNULL == msg_ptr)
    {
        return FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    //处理消息
    switch (msg_ptr->id)
    {
    case MMIIM_TP_CSTAR_MSG_TPDOWN:
        is_dealt = TpDownProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPMOVE:
        is_dealt = TpMoveProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPUP:
        is_dealt = TpUpInputingProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPLONG:
        is_dealt = TpLongProcess(data_ptr, &msg_ptr->para.tp);
        break;


        /*有输入内容时，可能会再响应如下TP消息*/
    case MMIIM_TP_CSTAR_MSG_KEY_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_WEB:
            case KEY_OK:
                {
                    if (data_ptr->result_len > 0)
                    {
                        uint16 nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = 0;
                        if (MMITHEME_IsMainScreenLandscape())
                        {
                            nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H;
                        }
                        else
                        {
                            nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V;
                        }
                        
                        tp_point.x = data_ptr->cand_rect.left + nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X + 3;
                        tp_point.y = data_ptr->cand_rect.top + 5;
                        TpCandPressUp(data_ptr, tp_point, data_ptr->event_data_ptr, TRUE);

                        if (bk_ui_state != data_ptr->ui_state)
                        {
                            data_ptr->is_rect_change = TRUE;
                        }

                        is_dealt = TRUE;
                    }
                    else if (data_ptr->nPreEdit > 0)
                    {
                        //有一个状态是：nPreEdit>0, nCand=0，此时响应ok键
                        HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_SELECT);

                        data_ptr->is_rect_change = TRUE;
                        
                        is_dealt = TRUE;
                    }
                    else
                    {
                        is_dealt = FALSE;
                    }
                }
                break;

            case KEY_CANCEL:
                {
                    //由于候选字没有lable，如果不是编辑状态，先送一个delete, 
                    if (CS_FLAG_STATE_CAND == data_ptr->cstar_window_state && !data_ptr->is_full_keypad)
                    {
#if defined (IM_SIMP_CHINESE_SUPPORT)
                        BOOLEAN is_need_delete = TRUE;
                        is_need_delete = (MMIIM_TP_CSTAR_STATE_PY_26KEY != data_ptr->stateId);
                        if (is_need_delete)
                        {
                            CS_IMM_CONTENT content = {0};
                            CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
                        }
#else
                        {
                            CS_IMM_CONTENT content = {0};
                            CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
                        }
#endif
                    }

                    HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE);

                    if (bk_ui_state != data_ptr->ui_state)
                    {
                        data_ptr->is_rect_change = TRUE;
                    }

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    case MMIIM_TP_CSTAR_MSG_KEY_LONG_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_CANCEL:
                {
                    SetToNoInput(data_ptr);
                    
                    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

                    if (bk_ui_state != data_ptr->ui_state)
                    {
                        data_ptr->is_rect_change = TRUE;
                    }

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : 在联想状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    GUI_POINT_T tp_point = {0};
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);/*assert verified*/
    if(PNULL == data_ptr ||PNULL == msg_ptr)
    {
        return FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    //处理消息
    switch (msg_ptr->id)
    {
    case MMIIM_TP_CSTAR_MSG_TPDOWN:
        is_dealt = TpDownProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPMOVE:
        is_dealt = TpMoveProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPUP:
        is_dealt = TpUpAssociateProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPLONG:
        is_dealt = TpLongProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_KEY_PRESS:
        {            
            switch (msg_ptr->para.key)
            {
            case KEY_WEB:
            case KEY_OK:
                {
                    if (data_ptr->result_len > 0)
                    {
                        uint16 nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = 0;
                        if (MMITHEME_IsMainScreenLandscape())
                        {
                            nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H;
                        }
                        else
                        {
                            nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V;
                        }
                        
                        tp_point.x = data_ptr->cand_rect.left + nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X + 3;
                        tp_point.y = data_ptr->cand_rect.top + 5;
                        TpCandPressUp(data_ptr, tp_point, data_ptr->event_data_ptr, TRUE);

                        if (bk_ui_state != data_ptr->ui_state)
                        {
                            data_ptr->is_rect_change = TRUE;
                        }

                        is_dealt = TRUE;
                    }
                    else
                    {
                        is_dealt = FALSE;
                    }
                }
                break;

            case KEY_CANCEL:
                {
                    HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE);

                    if (bk_ui_state != data_ptr->ui_state)
                    {
                        data_ptr->is_rect_change = TRUE;
                    }

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    case MMIIM_TP_CSTAR_MSG_KEY_LONG_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_CANCEL:
                {
                    SetToNoInput(data_ptr);
                    
                    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

                    if (bk_ui_state != data_ptr->ui_state)
                    {
                        data_ptr->is_rect_change = TRUE;
                    }

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : 在自造词状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgUserWord(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    GUI_POINT_T tp_point = {0};
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    //GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);/*assert verified*/
    if(PNULL == data_ptr ||PNULL == msg_ptr)
    {
        return FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    //处理消息
    switch (msg_ptr->id)
    {
    case MMIIM_TP_CSTAR_MSG_TPDOWN:
        is_dealt = TpDownProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPMOVE:
        is_dealt = TpMoveProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPUP:
        is_dealt = TpUpUserWordProcess(data_ptr, &msg_ptr->para.tp);
        break;

    case MMIIM_TP_CSTAR_MSG_TPLONG:
        is_dealt = TpLongProcess(data_ptr, &msg_ptr->para.tp);
        break;


        /*有输入内容时，可能会再响应如下TP消息*/
    case MMIIM_TP_CSTAR_MSG_KEY_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_WEB:
            case KEY_OK:
                {
                    if (data_ptr->result_len > 0)
                    {
                        uint16 nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = 0;
                        if (MMITHEME_IsMainScreenLandscape())
                        {
                            nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H;
                        }
                        else
                        {
                            nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V;
                        }
                        
                        tp_point.x = data_ptr->cand_rect.left + nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X + 3;
                        tp_point.y = data_ptr->cand_rect.top + 5;
                        TpCandPressUp(data_ptr, tp_point, data_ptr->event_data_ptr, TRUE);

                        if (bk_ui_state != data_ptr->ui_state)
                        {
                            data_ptr->is_rect_change = TRUE;
                        }

                        is_dealt = TRUE;
                    }
                    else
                    {
                        is_dealt = FALSE;
                    }
                }
                break;

            case KEY_CANCEL:
                {
                    //由于候选字没有lable，如果不是编辑状态，先送一个delete, 
                    if (CS_FLAG_STATE_CAND == data_ptr->cstar_window_state && !data_ptr->is_full_keypad)
                    {
#if defined (IM_SIMP_CHINESE_SUPPORT)
                        BOOLEAN is_need_delete = TRUE;
                        is_need_delete = (MMIIM_TP_CSTAR_STATE_PY_26KEY -MMIIM_TP_CSTAR_KB_START_ID != data_ptr->stateId);
                        if (is_need_delete)
                        {
                            CS_IMM_CONTENT content = {0};
                            CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
                        }
#else
                        {
                            CS_IMM_CONTENT content = {0};
                            CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
                        }
#endif
                    }

                    HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE);

                    if (bk_ui_state != data_ptr->ui_state)
                    {
                        data_ptr->is_rect_change = TRUE;
                    }

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    case MMIIM_TP_CSTAR_MSG_KEY_LONG_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_CANCEL:
                {
                    HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE | CS_VK_LONGCLICK);
                    //notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                    //GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

                    if (bk_ui_state != data_ptr->ui_state)
                    {
                        data_ptr->is_rect_change = TRUE;
                    }

                    is_dealt = TRUE;
                }
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : TpDownProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpDownProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr ||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    if (MMIIM_TP_CSTAR_MSG_TP_NONE == para_ptr->area)
    {
        return FALSE;
    }

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            if (para_ptr->idx < 0 || para_ptr->h_idx < 0 || para_ptr->v_idx < 0)
                break;

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_kb_dirty = TRUE;

            //画按下
            MMIIMUI_ReDrawAllBoard(data_ptr);
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_bar_cand_dirty = TRUE;

            //画按下
            MMIIMUI_ReDrawAllBoard(data_ptr);
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            MMIIMUI_TpDownOnChoiceBar(&data_ptr->choice_bar, 
                para_ptr->idx, &data_ptr->is_bar_choice_dirty);

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            if (data_ptr->is_bar_choice_dirty)
            {
                //画按下
                MMIIMUI_ReDrawAllBoard(data_ptr);
            }
        }
        break;

    default:
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : TpMoveProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpMoveProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr ||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            if (MMIIM_TP_CSTAR_MSG_TP_CAND == data_ptr->tp_last_msg_param.area)
            {
                //保存最后一次TP被按下的参数
                data_ptr->tp_last_msg_param.idx = -1;

                data_ptr->is_bar_cand_dirty = TRUE;
            }

            if (MMIIM_TP_CSTAR_MSG_TP_CHOICE == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_SetNormalStateOnChoiceBar(&data_ptr->choice_bar);
                
                data_ptr->is_bar_choice_dirty = TRUE;
            }

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt);

            //仍然在同一个键上
            if (para_ptr->idx == data_ptr->tp_last_msg_param.idx)
            {
                break;
            }

            //隐藏tips
            MMIIMUI_HideKeyTipsEx(data_ptr);

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_kb_dirty = TRUE;
            if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr))
            {
                data_ptr->is_bar_choice_dirty = TRUE;
            }
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            //表示焦点从键盘处滑到候选区
            if (MMIIM_TP_CSTAR_MSG_TP_KB == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_ClearPressKBMark(data_ptr);
                data_ptr->is_kb_dirty = TRUE;
            }

            if (MMIIM_TP_CSTAR_MSG_TP_CHOICE == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_SetNormalStateOnChoiceBar(&data_ptr->choice_bar);
                
                data_ptr->is_bar_choice_dirty = TRUE;
            }

//          if (-1 == para_ptr->idx)
//             {
//                 break;
//             }

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_bar_cand_dirty = TRUE;
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            //表示焦点从键盘处滑到候选区
            if (MMIIM_TP_CSTAR_MSG_TP_KB == data_ptr->tp_last_msg_param.area)
            {
                data_ptr->is_kb_dirty = TRUE;
            }

            if (MMIIM_TP_CSTAR_MSG_TP_CAND == data_ptr->tp_last_msg_param.area)
            {
                //文字区up后，立马清掉tips
                MMIIMUI_HideKeyTipsEx(data_ptr);

                data_ptr->tp_last_msg_param.idx = -1;

                data_ptr->is_bar_cand_dirty = TRUE;
            }

            MMIIMUI_TpDownOnChoiceBar(&data_ptr->choice_bar, 
                para_ptr->idx, &data_ptr->is_bar_choice_dirty);

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);
        }
        break;

    default:
        break;
    }

    if (data_ptr->is_kb_dirty
        || data_ptr->is_bar_cand_dirty
        || data_ptr->is_bar_choice_dirty
        || data_ptr->is_bar_edit_dirty)
    {
        MMIIMUI_ReDrawAllBoard(data_ptr);
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : TpUpNoInputProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpUpNoInputProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    MMIIM_TP_CSTAR_STATE_T state = {0};
    MMIIM_STATE_FUNC func = PNULL;
    int16 count = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    if(PNULL == data_ptr
        || PNULL == para_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TpUpNoInputProcess: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_1799_112_2_18_2_28_20_210,(uint8*)"");
        return FALSE;
    }
    
    if (MMIIM_TP_CSTAR_MSG_TP_KB != para_ptr->area)
    {
        MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
        return TRUE;
    }

    if (MMIIM_TP_CSTAR_MSG_TP_NONE == para_ptr->area)
    {
        return FALSE;
    }

    if (MMIIM_TP_CSTAR_MSG_TP_KB != para_ptr->area
            || -1 == para_ptr->idx
            || para_ptr->idx != data_ptr->tp_last_msg_param.idx)
    {
        MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
        return TRUE;
    }

    //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt);
    state = g_keyboard_state[data_ptr->stateId];
    count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);
    bk_ui_state = data_ptr->ui_state;

    func = state.functab[count].func;

    if (!func)
    {
        return TRUE;
    }

    is_ret = func(data_ptr, data_ptr->event_data_ptr, state.functab[count].id);

    if (bk_ui_state != data_ptr->ui_state)
    {
        data_ptr->is_rect_change = TRUE;
    }
    
    if (!data_ptr->is_rect_change)
    {        
        MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
    }
    
    if (!is_ret)
    {
        return TRUE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : TpUpInputingProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 屏上有内容时，up处理
/*****************************************************************************/
LOCAL BOOLEAN TpUpInputingProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    MMIIM_TP_CSTAR_STATE_T state = {0};
    MMIIM_STATE_FUNC func = PNULL;
    int16 count = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    if (PNULL == data_ptr
        || PNULL == para_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TpUpInputingProcess: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_1872_112_2_18_2_28_20_211,(uint8*)"");
        return FALSE;
    }

    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            MMIIMUI_ClearPressKBMark(data_ptr);

            //抬起之前未击中键盘
            if (-1 == data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                break;
            }

            //不在同一个键上
            if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                break;
            }

            //TP处理
            state = g_keyboard_state[data_ptr->stateId];
            count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);

            func = state.functab[count].func;

            if (!func)
            {
                return TRUE;
            }

            is_ret = func(data_ptr, data_ptr->event_data_ptr, state.functab[count].id);

            if (bk_ui_state != data_ptr->ui_state)
            {
                data_ptr->is_rect_change = TRUE;
            }

            if (!is_ret)
            {
                return TRUE;
            }
        }

        break;

        //处理候选区up
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            TpCandPressUp(data_ptr, para_ptr->point, data_ptr->event_data_ptr, FALSE);

            if (bk_ui_state != data_ptr->ui_state)
            {
                data_ptr->is_rect_change = TRUE;
            }
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            if (0 > para_ptr->idx)
            {
                data_ptr->is_bar_choice_dirty = TRUE;
                data_ptr->choice_bar.hot_index = -1;
                MMIIMUI_ReDrawAllBoard(data_ptr);
                break;
            }

            //先处理一下内容
            is_ret = HandleChoicePageSimulator(data_ptr, &data_ptr->choice_bar, para_ptr->idx);

            //再处理显示
            TpUpOnChoiceBar(&data_ptr->choice_bar, para_ptr->idx, &data_ptr->is_bar_choice_dirty);

            if (data_ptr->is_bar_choice_dirty
                    || data_ptr->is_bar_edit_dirty
                    || data_ptr->is_bar_cand_dirty)
            {
                data_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
                data_ptr->tp_last_msg_param.idx = -1;

                MMIIMUI_ReDrawAllBoard(data_ptr);
            }
        }
        break;

    default:
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
        }
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : TpUpAssociateProcess
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 屏上有内容时，up处理
/*****************************************************************************/
LOCAL BOOLEAN TpUpAssociateProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    MMIIM_TP_CSTAR_STATE_T state = {0};
    MMIIM_STATE_FUNC func = PNULL;
    int16 count = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    if (PNULL == data_ptr
        || PNULL == para_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TpUpAssociateProcess: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_1992_112_2_18_2_28_20_212,(uint8*)"");
        return FALSE;
    }

    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:

        MMIIMUI_ClearPressKBMark(data_ptr);


        //抬起之前未击中键盘
        if (-1 == data_ptr->tp_last_msg_param.idx)
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            break;
        }

        //不在同一个键上
        if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            break;
        }

        //TP处理
        state = g_keyboard_state[data_ptr->stateId];
        count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);

        func = state.functab[count].func;

        if (!func)
        {
            return TRUE;
        }

        is_ret = func(data_ptr, data_ptr->event_data_ptr, state.functab[count].id);

        if (bk_ui_state != data_ptr->ui_state)
        {
            data_ptr->is_rect_change = TRUE;
        }

        if (!is_ret)
        {
            return TRUE;
        }

        break;

        //处理候选区up
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            TpCandPressUp(data_ptr, para_ptr->point, data_ptr->event_data_ptr, FALSE);

            if (bk_ui_state != data_ptr->ui_state)
            {
                data_ptr->is_rect_change = TRUE;
            }
        }
        break;

    default:
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
        }
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : TpUpUserWordProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 屏上有内容时，up处理
/*****************************************************************************/
LOCAL BOOLEAN TpUpUserWordProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    MMIIM_TP_CSTAR_STATE_T state = {0};
    MMIIM_STATE_FUNC func = PNULL;
    int16 count = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    if (PNULL == data_ptr
        || PNULL == para_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TpUpUserWordProcess: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_2084_112_2_18_2_28_21_213,(uint8*)"");
        return FALSE;
    }

    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            MMIIMUI_ClearPressKBMark(data_ptr);

            //抬起之前未击中键盘
            if (-1 == data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                break;
            }

            //不在同一个键上
            if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                break;
            }

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt);

            //TP处理
            state = g_keyboard_state[data_ptr->stateId];
            count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);

            func = state.functab[count].func;

            if (!func)
            {
                return TRUE;
            }

            is_ret = func(data_ptr, data_ptr->event_data_ptr, state.functab[count].id);

            if (bk_ui_state != data_ptr->ui_state)
            {
                data_ptr->is_rect_change = TRUE;
            }

            if (!is_ret)
            {
                return TRUE;
            }
        }

        break;

        //处理候选区up
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            TpCandPressUp(data_ptr, para_ptr->point, data_ptr->event_data_ptr, FALSE);

            if (bk_ui_state != data_ptr->ui_state)
            {
                data_ptr->is_rect_change = TRUE;
            }
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            if (0 > para_ptr->idx)
            {
                data_ptr->is_bar_choice_dirty = TRUE;
                data_ptr->choice_bar.hot_index = -1;
                MMIIMUI_ReDrawAllBoard(data_ptr);
                break;
            }

            //先处理一下内容
            is_ret = HandleChoicePageSimulator(data_ptr, &data_ptr->choice_bar, para_ptr->idx);

            //再处理显示
            if (is_ret)
            {
                TpUpOnChoiceBar(&data_ptr->choice_bar, para_ptr->idx, &data_ptr->is_bar_choice_dirty);

                if (data_ptr->is_bar_choice_dirty
                        || data_ptr->is_bar_edit_dirty
                        || data_ptr->is_bar_cand_dirty)
                {
                    data_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
                    data_ptr->tp_last_msg_param.idx = -1;

                    MMIIMUI_ReDrawAllBoard(data_ptr);
                }
            }
        }
        break;

    default:
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
        }
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 在联想状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpLongProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    MMIIM_TP_CSTAR_STATE_T state = {0};
    int16 count = 0;
    uint32 id = 0;    
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    if (PNULL == data_ptr
        || PNULL == para_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TpLongProcess: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_2209_112_2_18_2_28_21_214,(uint8*)"");
        return FALSE;
    }

    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:

        //抬起之前未击中键盘
        if (-1 == data_ptr->tp_last_msg_param.idx)
        {
            break;
        }

        //不在同一个键上
        if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
        {
            break;
        }

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt);

        /*TP上触笔长按，无非就是一个回退键，如果不是回退键，则不做任何处理，直接返回true*/
        state = g_keyboard_state[data_ptr->stateId];
        count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);
        id = state.functab[count].id;

        if(id >= g_keyboard_state[data_ptr->stateId].char_cnt)
            break;
        
        if (!MMIIMUI_SearchFromStrtab(&s_sym_del[1], g_keyboard_state[data_ptr->stateId].char_tab[id]))
        {
            return TRUE;
        }

        //全删除

        if (MMIIM_TP_CSTAR_STATE_USER == data_ptr->ui_state)
        {
            HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE | CS_VK_LONGCLICK);
        } 
        else
        {
            if (g_keyboard_state[data_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
            {
                SetToNoInput(data_ptr);
            }
            
            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }        

        if (bk_ui_state != data_ptr->ui_state)
        {
            data_ptr->is_rect_change = TRUE;
        }

        break;

        //处理候选区长按不处理
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        break;

    default:
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 触笔点击候选框弹起操作
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: simulator 是否是由键盘模拟TP按在了候选区
/*****************************************************************************/
LOCAL void TpCandPressUp(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,    //in:输入法句柄
    GUI_POINT_T tp_point,                   //in:点击的位置
    GUIIM_EVENT_DATA_U* event_data_ptr,     //in&out:传入和传出的参数
    BOOLEAN is_simulator
)
{
    GUI_RECT_T font_rect = {0};
    uint16 iLoop = 0;
    uint16 jLoop = 0;
    uint16 pos = 0;
    GUI_FONT_T font = {0};
    uint16 word_width = 0;
    uint16 label_width = 0;
    GUI_RECT_T text_rect = {0};                         //文本区区域
    uint16 nMMIIM_TP_CSTAR_CAND_UP_LEFT_X = 0;
    uint16 nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = 0;
    MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();
    uint8  direction  =  0;
    
    //SCI_TRACE_LOW:"[MMIIM] TpCandPressUp, p.x=%d, p.y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_2305_112_2_18_2_28_21_215,(uint8*)"dd", tp_point.x, tp_point.y);

    if (PNULL == handle_ptr
        || PNULL == handle_ptr->lcd_info_ptr
        || PNULL == event_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] TpCandPressUp: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_2311_112_2_18_2_28_21_216,(uint8*)"");
        return;
    }
    
    if (MMITHEME_IsMainScreenLandscape())
    {
        nMMIIM_TP_CSTAR_CAND_UP_LEFT_X = MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H;
        nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H;
    }
    else
    {
        nMMIIM_TP_CSTAR_CAND_UP_LEFT_X = MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V;
        nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V;
    }

    direction = MMIIM_TPGetAligntype(handle_ptr->init_param.method_param_ptr->im_set.method);
    text_rect.left = handle_ptr->cand_rect.left + nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X;
    text_rect.right = nMMIIM_TP_CSTAR_CAND_UP_LEFT_X - nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X;
    text_rect.top = handle_ptr->cand_rect.top;
    text_rect.bottom = handle_ptr->cand_rect.bottom;

    //文字区up后
    if (is_simulator)
    {
        //立马清掉tips
        MMIIMUI_HideKeyTips(handle_ptr);
    }
    else
    {
        //tips则启动定时器，时间到了再来清tips
        MMIIMUI_HideKeyTipsEx(handle_ptr);
    }

    if (GUI_PointIsInRect(tp_point, text_rect))
    {
        if (MMIIM_TP_CSTAR_STATE_USER == handle_ptr->ui_state)
        {
            /*自造词状态*/
            if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
            {
                font = text_bar_theme->font_text;
                font_rect.top = handle_ptr->cand_rect.top;
                font_rect.bottom = handle_ptr->cand_rect.bottom;
                font_rect.left = handle_ptr->cand_rect.left;
                label_width = GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;
                iLoop = 0;

                while (iLoop++ < handle_ptr->nUserWord)
                {
                    jLoop = 0;

                    while (handle_ptr->pUserWordBuffer[pos+ (jLoop++)] != '\0')
                        NULL;       //lint !e522

                    word_width = GUI_GetStringWidth(font, &handle_ptr->pUserWordBuffer[pos], jLoop);
                    font_rect.right = font_rect.left + word_width;

                    if (GUI_PointIsInRect(tp_point, font_rect))
                    {
                        //接着处理下一级联想
                        handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
                        handle_ptr->tp_last_msg_param.idx = -1;
                        HandleTpKeySelect(handle_ptr, iLoop - 1);
                        break;
                    }

                    pos += jLoop;
                    font_rect.left = (int16)(font_rect.right + label_width);
                }
                
                //找不到值可能是边界框，则检查一下是否有被按下，若有则要清除一下背景
                if (MMIIM_TP_CSTAR_MSG_TP_CAND == handle_ptr->tp_last_msg_param.area
                    && -1 != handle_ptr->tp_last_msg_param.idx)
                {
                    handle_ptr->tp_last_msg_param.idx = -1;
                    handle_ptr->is_bar_cand_dirty = TRUE;
                    MMIIMUI_ReDrawAllBoard(handle_ptr);
                }
            }
        }
        else if (handle_ptr->result_len > 0)
        {
            /*候选词状态*/
            if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
            {
                font = text_bar_theme->font_text;
                font_rect.top = handle_ptr->cand_rect.top;
                font_rect.bottom = handle_ptr->cand_rect.bottom;
                if(MMIIM_DRAW_RIGHT2LEFT == direction)
                {
                    font_rect.right = text_rect.right - 2;
                }
                font_rect.left = handle_ptr->cand_rect.left;
                label_width = GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;
                iLoop = 0;

                while (iLoop++ < handle_ptr->result_len)
                {
                    jLoop = 0;

                    while (handle_ptr->result[pos+ (jLoop++)] != '\0')
                        NULL;       //lint !e522

                    word_width = GUI_GetStringWidth(font, &handle_ptr->result[pos], jLoop);
                    if(MMIIM_DRAW_RIGHT2LEFT == direction)
                    {
                        font_rect.left = font_rect.right - word_width;
                    }
                    else
                    {
                        font_rect.right = font_rect.left + word_width;
                    }   

                    if (GUI_PointIsInRect(tp_point, font_rect))
                    {
                        //接着处理下一级联想
                        handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
                        handle_ptr->tp_last_msg_param.idx = -1;
                        HandleTpKeySelect(handle_ptr, iLoop - 1);
                        break;
                    }

                    pos += jLoop;
                    if(MMIIM_DRAW_RIGHT2LEFT == direction)
                    {
                        font_rect.right = (int16)(font_rect.left - label_width);
                    }
                    else
                    {
                        font_rect.left = (int16)(font_rect.right + label_width);
                    }
                }
                
                //找不到值可能是边界框，则检查一下是否有被按下，若有则要清除一下背景
                if (MMIIM_TP_CSTAR_MSG_TP_CAND == handle_ptr->tp_last_msg_param.area
                    && -1 != handle_ptr->tp_last_msg_param.idx)
                {
                    handle_ptr->tp_last_msg_param.idx = -1;
                    handle_ptr->is_bar_cand_dirty = TRUE;
                    MMIIMUI_ReDrawAllBoard(handle_ptr);
                }
            }
        }
    }
    else if (GUI_PointIsInRect(tp_point, handle_ptr->up_rect))
    {
        if (!(handle_ptr->pageflag&MMIIM_TP_CSTAR_PAGEFLAG_PREVIOUS))
        {
            return;
        }

        if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
        {
            //这里是由我们触笔不打数字标签，在UI上让人感觉编辑状态和候选状态没区别
            //所以如果点向上键，此时又不是候选状态，则需要送一个down消息使用内核为候选状态
            if (CS_FLAG_STATE_CAND != handle_ptr->cstar_window_state)
            {
                CS_IMM_CONTENT content = {0};
                (void) CS_IMM_EventHandle(handle_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);
                SetUIStateByContent(handle_ptr, &content);
            }

            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
            handle_ptr->tp_last_msg_param.idx = -1;
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_UP);
        }
    }
    else if (GUI_PointIsInRect(tp_point, handle_ptr->down_rect))
    {
        if (!(handle_ptr->pageflag&MMIIM_TP_CSTAR_PAGEFLAG_NEXT))
        {
            return;
        }

        if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
        {
            if (CS_FLAG_STATE_CAND != handle_ptr->cstar_window_state)
            {                
                CS_IMM_CONTENT content = {0};
                (void) CS_IMM_EventHandle(handle_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);
                SetUIStateByContent(handle_ptr, &content);
            }

            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
            handle_ptr->tp_last_msg_param.idx = -1;
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_DOWN);
        }
    }
}

/*****************************************************************************/
//  Description : 当触笔在choice bar上up，对应处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpUpOnChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx,
    BOOLEAN *dirty_flg_ptr
)
{
    //SCI_ASSERT(PNULL != bar_ptr);/*assert verified*/

    if (idx < 0 || PNULL == bar_ptr)
    {
        return;
    }

    if (CAND_LEFT_ARROW_IDX == idx)
    {
        //向上翻页
        if (0 == bar_ptr->page_index)
        {
            bar_ptr->hot_index = -1;
            *dirty_flg_ptr = TRUE;
            return;
        }

        bar_ptr->page_index--;
        bar_ptr->hot_index = -1;

        if (bar_ptr->page_index > 0)
        {
            bar_ptr->idc_up_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;
        }
        else
        {
            bar_ptr->idc_up_state = MMIIM_TP_CSTAR_KEY_STATE_INACTIVE;
        }

        bar_ptr->cur_page_line_cnt = bar_ptr->choice_line_cnt;
        bar_ptr->idc_down_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;
    }
    else if (CAND_RIGHT_ARROW_IDX == idx)
    {
        //向下翻页
        if (bar_ptr->page_index >= bar_ptr->page_cnt - 1)
        {
            bar_ptr->hot_index = -1;
            *dirty_flg_ptr = TRUE;
            return;
        }

        bar_ptr->page_index++;
        bar_ptr->hot_index = -1;

        if (bar_ptr->page_index < bar_ptr->page_cnt - 1)
        {
            bar_ptr->idc_down_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;
            bar_ptr->cur_page_line_cnt = bar_ptr->choice_line_cnt;
        }
        else
        {
            bar_ptr->idc_down_state = MMIIM_TP_CSTAR_KEY_STATE_INACTIVE;
            bar_ptr->cur_page_line_cnt = bar_ptr->total_choice_cnt - bar_ptr->page_index * bar_ptr->choice_line_cnt;
        }

        bar_ptr->idc_up_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;

    }
    else if (-1 < bar_ptr->hot_index && bar_ptr->hot_index < bar_ptr->cur_page_line_cnt)
    {
        bar_ptr->hot_index = -1;
    }
    else
    {
        return;
    }

    *dirty_flg_ptr = TRUE;
    return;
}

/*****************************************************************************/
//  Description : 模拟混淆区的拼音翻页功能
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 内核引擎只能一个一个翻，我们自己来模拟翻页
/*****************************************************************************/
LOCAL BOOLEAN HandleChoicePageSimulator(
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr,
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx
)
{
    CS_IMM_CONTENT content = {0};
    int i = 0;
    int8 move_cnt = 0;          //此参数表示要移动的次数，负数表示左移，正数表示右移

    if (PNULL == data_ptr
        || PNULL == bar_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleChoicePageSimulator: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_2600_112_2_18_2_28_22_217,(uint8*)"");
        return FALSE;
    }

    if (idx < 0)
    {
        return FALSE;
    }

    if (CAND_LEFT_ARROW_IDX == idx)
    {
        //向上翻页
        if (0 == bar_ptr->page_index)
        {
            return FALSE;
        }

        //如果是候选字状态，则要送一个delete给内核引擎，这样才会向下翻，此时不必拷贝内容
        /*下面这个做法实在是有点勉强，因为引擎就不支持混淆音翻页功能，UI确要自己来模拟了*/
        if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr)
                && CS_FLAG_STATE_CAND == data_ptr->cstar_window_state)
        {
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
        }

        if (!bar_ptr->is_choice_state)
        {
            move_cnt = bar_ptr->choice_line_cnt;
#ifndef INPUT_METHOD_CSTAR_COMMON_MINI//单字情况，与SetOptionEx的模式设置有关联
            move_cnt += 1;
#endif
        }
        else
        {
            move_cnt = bar_ptr->choice_line_cnt + bar_ptr->choice_last_index;
        }

        //向上翻页，意味着要送内核多少个left
        for (i = 0; i < move_cnt; i++)
        {
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_LEFT, &content);
        }

        //移动当前总序中的索引，放在上一页的第一个
        bar_ptr->choice_last_index = 0;
        bar_ptr->is_choice_state = TRUE;
    }
    else if (CAND_RIGHT_ARROW_IDX == idx)
    {
        //向下翻页
        if (bar_ptr->page_index >= bar_ptr->page_cnt - 1)
        {
            return FALSE;
        }

        if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr)
                && CS_FLAG_STATE_CAND == data_ptr->cstar_window_state)
        {
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
        }

        if (!bar_ptr->is_choice_state)
        {
            move_cnt = bar_ptr->choice_line_cnt;
#ifndef INPUT_METHOD_CSTAR_COMMON_MINI//单字情况，与SetOptionEx的模式设置有关联
            move_cnt += 1;
#endif
        }
        else
        {
            move_cnt = bar_ptr->choice_line_cnt - bar_ptr->choice_last_index;
        }

        //向下翻页
        for (i = 0; i < move_cnt; i++)
        {
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_RIGHT, &content);
        }

        //移动当前总序中的索引，放在下一页的第一个
        bar_ptr->choice_last_index = 0;
        bar_ptr->is_choice_state = TRUE;
    }
    else if (-1 < bar_ptr->hot_index && bar_ptr->hot_index < bar_ptr->cur_page_line_cnt)
    {
        if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr)
                && CS_FLAG_STATE_CAND == data_ptr->cstar_window_state)
        {
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
        }

        if (!bar_ptr->is_choice_state)
        {
            //-1表示初次进入，其实就是状态不一样，处理不一样
            move_cnt = bar_ptr->choice_index;
#ifndef INPUT_METHOD_CSTAR_COMMON_MINI
            move_cnt += 1;
#endif
        }
        else
        {
            move_cnt = bar_ptr->choice_index - bar_ptr->choice_last_index;

#ifndef INPUT_METHOD_CSTAR_COMMON_MINI
            //不是第一次，说明点的是同一个位置
            if (0 == move_cnt)
            {
                return TRUE;
            }
#endif
        }

        if (move_cnt > 0)
        {
            for (i = 0; i < move_cnt; i++)
            {
                (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_RIGHT, &content);
            }
        }
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
        if(0 == move_cnt)
        {
            //(void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);

            //mini版本存在bug，暂且仅仅应用在该版本上，防止存在以外风险
            //具体见NEWMS104272:9键，拼音输入点击虚拟键盘候选字翻页后，从新点击choice字符后，
            //候选字字符显示错乱，之所以这样模拟，是因为翻页后，点击同一个choice索引
            //down消息并没有从新刷新上次翻页后的数据，应该从新刷新到起始数据
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_LEFT, &content);
            (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_RIGHT, &content);
        }

#endif
        else
        {
            move_cnt = -move_cnt;

            for (i = 0; i < move_cnt; i++)
            {
                (void) CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_LEFT, &content);
            }
        }

        bar_ptr->choice_last_index = bar_ptr->choice_index;
        bar_ptr->is_choice_state = TRUE;
    }
    else
    {
        return FALSE;
    }

    //保存一些必要的值
    data_ptr->cstar_window_state = content.flagState;
    data_ptr->pageflag = content.flagPageCand;

    /*处理完成后，还要保存内容到result中*/
    SCI_MEMCPY(data_ptr->result, content.pCandBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->result_len = content.nCand;

    /*处理完成后保存编辑内容到pPreBuffer中*/
    SCI_MEMCPY(data_ptr->pPreBuffer, content.pPreBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nPreEdit = content.nPreEdit;

    /*处理完成后，保存underline的内容到pUnderLine中*/
    SCI_MEMCPY(data_ptr->pUnderLine, content.pUnderlineBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nUnderLine = content.nUnderline;

    data_ptr->is_bar_cand_dirty = TRUE;
    data_ptr->is_bar_edit_dirty = TRUE;

    return TRUE;
}

/*****************************************************************************/
//  Description : 把外部的输入法项转换成内部的状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: cstar不管9/26，内部键盘状态关心9/26, 内部输入法 < cstar输入法*2
/*****************************************************************************/
LOCAL uint32 ConvertMethodtoState(
    GUIIM_METHOD_T method,
    BOOLEAN is_full_keypad
)
{
    uint16 i = 0;
    uint16 im_cnt = ARR_SIZE(g_tp_cstar_method_relation_list);
    CS_UINT cs_im = IM_NONE;

    if(GUIIM_M_NONE == method)
        return cs_im;

    for(i = 0; i < im_cnt; i++)
    {
        if(g_tp_cstar_method_relation_list[i].guiim_method == method)
        {
            cs_im = g_tp_cstar_method_relation_list[i].cstar_method;
            break;
        }
    }
   
    if (is_full_keypad)
    {
        switch (cs_im)
        {
        case IM_STROKE:
        case IM_STROKE_BIG5:
        case IM_123:
            break;
        default:
            {
                if (GUIIM_M_TPQ_ZHUYIN == method)
                {
                    cs_im = IM_ZHUYIN + IM_FULLKEY_BASE;
                }
                else
                {
                    cs_im = cs_im + IM_FULLKEY_BASE;
                }
            }
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIIM] ConvertMethodtoState cs_im =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_2821_112_2_18_2_28_22_218,(uint8*)"d", cs_im);
    return cs_im;
}

/******************************************************************************/
// Description: get text width call back
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL int GetTextWidthCallback(const unsigned short* text, int length)
{
    MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();
    uint16 len = (length >= 0) ? length : 0;
    uint16 textwidth =  GUI_GetStringWidth(text_bar_theme->font_text, text, len);
    return textwidth;
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

/*****************************************************************************/
//  Description : 设置输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 依据cstar输入法及是否全键盘给引擎设置正确方法,state用于更新当前状态
/*****************************************************************************/
LOCAL void SetMethod(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    GUIIM_METHOD_T method,
    MMIIM_TP_CSTAR_STATE_ID_T kb_state
)
{    
    CS_RESULT result = TRUE;
    CS_UINT input_method = 0;

    //SCI_ASSERT(PNULL != handle_ptr);/*assert verified*/
    if ( PNULL == handle_ptr||PNULL == handle_ptr->cstar_handle)
    {
        return;
    }
    
    MMIIMUI_InitUIHandleData(handle_ptr, kb_state);

    //SCI_ASSERT(PNULL != handle_ptr->cstar_handle);/*assert verified*/

    input_method = ConvertMethodtoState(method, handle_ptr->is_full_keypad);
    result = CS_IMM_SetInputMethod(handle_ptr->cstar_handle, input_method);
//  if (0 != result)
//  {
//      SCI_TRACE_LOW("[MMIIM] method=%d", input_method);
//      SCI_ASSERT(0 == result);
//  }


    return;
}

/*****************************************************************************/
//  Description : 由cstar返回的内容设置当前UI状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetUIStateByContent(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    CS_IMM_CONTENT* content_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr);/*assert verified*/
    //SCI_ASSERT(PNULL != content_ptr);/*assert verified*/
    if ( PNULL == data_ptr||PNULL == content_ptr)
    {
        return;
    }
    
    data_ptr->cstar_window_state = content_ptr->flagState;

    /*处理完成后，还要保存内容到result中*/
    SCI_MEMCPY(data_ptr->result, content_ptr->pCandBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->result_len = content_ptr->nCand;

    /*处理完成后保存编辑内容到pPreBuffer中*/
    SCI_MEMCPY(data_ptr->pPreBuffer, content_ptr->pPreBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nPreEdit = content_ptr->nPreEdit;

    /*处理完成后，保存underline的内容到pUnderLine中*/
    SCI_MEMCPY(data_ptr->pUnderLine, content_ptr->pUnderlineBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nUnderLine = content_ptr->nUnderline;

    /*处理完成后，保存UserWord的内容到pUserWordBuffer中*/
    SCI_MEMCPY(data_ptr->pUserWordBuffer, content_ptr->pUserWordBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nUserWord= content_ptr->nUserWord - 1;    

    if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr))
    {
        MMIIMUI_GetChoiceBufferByKernel(&data_ptr->choice_bar, content_ptr->pChoiceBuffer, content_ptr->nChoice);
    }

    /*根据返回的内容来检查当前输入法的UI显示状态
    如果有commit内容，则一定没有preedit内容，则检查是否有候选字*/

    do
    {
        if (CS_FLAG_STATE_USER == content_ptr->flagState)
        {
            //如果是自造词，则直接修改状态并直接break
            data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_USER;
            break;
        }
        
#ifdef INPUT_METHOD_CSTAR_COMMON_MINI
        if(0 < content_ptr ->nChoice)
        {
            if (0 < content_ptr->nCand)
            {
                //有编辑内容，有候选字，则为编辑输入状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;//2行choice_win+cand
            }
        }
        else
        {
            if (0 < content_ptr->nCand)
            {
                //无编辑内容，有候选字，则为联想字状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_ASSOCIATE;
            }
            else
            {
                //无编辑内容，无候选字，则为无输入状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
            }
        }
#else
        if (0 == content_ptr->nPreEdit)
        {
            if (0 < content_ptr->nCand)
            {
                //无编辑内容，有候选字，则为联想字状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_ASSOCIATE;
            }
            else
            {
                //无编辑内容，无候选字，则为无输入状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
            }

            break;
        }
        else
        {
            if (0 < content_ptr->nCand)
            {
                //有编辑内容，有候选字，则为编辑输入状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;//2行：pre_edit+cand
            }
            else
            {
                //有编辑内容，无候选字，则为无输入状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;
            }
            break;
        }
#endif

    }
    while (0);

    return;
}

/*****************************************************************************/
//  Description : 处理cstar事件
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleCstarEvent(MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, CS_UINT hEvent, CS_UINT iMsg)
{
    CS_RESULT result = TRUE;
    CS_IMM_CONTENT content = {0};
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    //SCI_TRACE_LOW:"[MMIIM] HandleCstarEvent"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3004_112_2_18_2_28_22_219,(uint8*)"");

    if (PNULL == handle_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleCstarEvent: handle_ptr is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3008_112_2_18_2_28_22_220,(uint8*)"");
        return FALSE;
    }

    if (handle_ptr->cstar_handle)
    {
        result = CS_IMM_EventHandle(handle_ptr->cstar_handle, hEvent, iMsg, &content);
        SetUIStateByContent(handle_ptr, &content);
    }

    handle_ptr->pageflag = content.flagPageCand;
    /*因为userword目前使用的都是无翻页功能*/
    if (MMIIM_TP_CSTAR_STATE_USER == handle_ptr->ui_state)
    {
        handle_ptr->pageflag = 0;

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
            MMIIM_StartTimer(&(handle_ptr->mul_timer_opened), handle_ptr->ctrl_handle);
        }
        else if(CS_TIMER_CLEAR == content.flagTimer || 0 == content.flagTimer)
        {
            MMIIM_StopTimer(&(handle_ptr->mul_timer_opened));
        }
    }

    if (content.pCommit[0])
    {
        uint32 len_1 = GUIIM_COMMIT_BUFFER_MAX;
        uint32 len_2 = MMIAPICOM_Wstrlen(content.pCommit);
        //SCI_ASSERT(PNULL != handle_ptr->event_data_ptr); /*assert verified*/
        notify_pack.data.commit_data.commit_len = MIN(len_1, len_2);
        SCI_MEMCPY(notify_pack.data.commit_data.commit, content.pCommit,
                   notify_pack.data.commit_data.commit_len * sizeof(content.pCommit[0]));

        if (content.nUnderline == 0)
        {
            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(handle_ptr->init_param.im_handle, &notify_pack);
        }
    }

    if (content.nUnderline > 0)
    {
        uint16 i = 0;
        content.nUnderline = MIN(content.nUnderline, GUIIM_COMMIT_BUFFER_MAX) -1;

        for (i = 0; i <= content.nUnderline; i++)
        {
            notify_pack.data.commit_data.underline[i] = (wchar)(content.pUnderlineBuffer[i]);
        }

        notify_pack.data.commit_data.underline_len = (uint8)(content.nUnderline);
        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(handle_ptr->init_param.im_handle, &notify_pack);
    }
    else
    {
        MMIIMUI_HideKeyTipsEx(handle_ptr);

        //管显示刷新
        switch (handle_ptr->ui_state)
        {
        case MMIIM_TP_CSTAR_STATE_INPUTING:
        case MMIIM_TP_CSTAR_STATE_USER:
        #ifndef INPUT_METHOD_CSTAR_COMMON_MINI
            if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_ED)
            {
                MMIIMUI_DrawEditBar(handle_ptr);
            }
        #endif
            //lint -fallthrough
        case MMIIM_TP_CSTAR_STATE_ASSOCIATE:

            if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_CAND)
            {
                MMIIMUI_DrawCandBar(handle_ptr);
            }

            //lint -fallthrough
        case MMIIM_TP_CSTAR_STATE_NOINPUT:
            {
                //注意我们是要从下往上画
                MMIIMUI_DrawKeyBoard(handle_ptr);

                //画重复的图标
                MMIIMUI_DrawReplaceKey(handle_ptr);

                //如果是9键，则需要画出混淆区
                /*[注意]原本在输入法设置时，会设置混淆区的上下按键的区域，待代码合并后，把这些区域的设置放在画画时
                再设即可*/
                if (!handle_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(handle_ptr))
                {
                    //MMIIMUI_GetChoiceBufferByKernel(&handle_ptr->choice_bar, content.pChoiceBuffer, content.nChoice);
                    MMIIMUI_DrawChoiceBar(&handle_ptr->choice_bar);
                }
            }
            break;
        default:
            break;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 返回键处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleReturn(
    void *handle_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = MMIIM_TP_CSTAR_KB_START_ID;

    //SCI_TRACE_LOW:"[MMIIM] HandleReturn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3136_112_2_18_2_28_23_221,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleReturn: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3141_112_2_18_2_28_23_222,(uint8*)"");
        return FALSE;
    }
    
    //偏移
    kb_state = 0;

    MMIIMUI_InitUIHandleData(handle_ptr, kb_state);
    MMIIMUI_InitDisplayData(handle_ptr);
    MMIIMUI_DrawAllBoard(handle_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : 切换键处理
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleSwicth(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = MMIIM_TP_CSTAR_KB_START_ID;

    //SCI_TRACE_LOW:"[MMIIM] HandleSwicth"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3169_112_2_18_2_28_23_223,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleSwicth: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3174_112_2_18_2_28_23_224,(uint8*)"");
        return FALSE;
    }

    switch (id)
    {
    case MMIIM_TP_CSTAR_STATE_DIG:
        notify_pack.data.set_method = GUIIM_M_TP_123;
        notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
        //data_ptr->is_full_keypad = TRUE;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        break;

     case MMIIM_TP_CSTAR_STATE_DIGSYM:
        if (!(data_ptr->init_param.method_param_ptr->limit & GUIIM_LIMIT_SYMBOL))
        {
            kb_state = MMIIM_TP_CSTAR_STATE_DIGSYM - MMIIM_TP_CSTAR_KB_START_ID;

            MMIIMUI_InitUIHandleData(handle_ptr, kb_state);
            MMIIMUI_InitDisplayData(data_ptr);
            MMIIMUI_DrawAllBoard(data_ptr);
        }
        break;

    default:
        notify_pack.data.set_method = GUIIM_M_HANDWRITING;
        notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
        data_ptr->is_full_keypad = TRUE;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        break;

    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 字符键处理
//  Global resource dependence : none
//  Author: rui.xu
//  Note: 处理直接上屏的内容，注意配置键盘函数表时，要注意
/*****************************************************************************/
PUBLIC BOOLEAN HandleChar(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    uint16 const* strtab_ptr = PNULL;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"[MMIIM] HandleChar"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3224_112_2_18_2_28_23_225,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr
        || data_ptr->stateId >= MMIIM_TP_CSTAR_KB_STATE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleChar: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3230_112_2_18_2_28_23_226,(uint8*)"");
        return FALSE;
    }
    
    if (id >= g_keyboard_state[data_ptr->stateId].char_cnt)
    {
        return TRUE;
    }

    //取出对应的键码
    strtab_ptr = g_keyboard_state[data_ptr->stateId].char_tab[id];

    if (PNULL == strtab_ptr)
    {
        return TRUE;
    }

    //处理直接上屏内容、只处理一一对应的字符，不管理一个键可能对应三个字符
    if (1 == strtab_ptr[0])
    {
        //我们这里只管上屏一个字符，一般用于配置不需要输入法内核的键盘状态配置中
        if (strtab_ptr[1] != s_sym_del[1] || (data_ptr->result_len == 0))
        {
            if(strtab_ptr[1] == s_sym_space[1])        //中文时，使用全角空格
            {
                if(data_ptr->init_param.method_param_ptr && 
                    (data_ptr->init_param.method_param_ptr->im_set.method == GUIIM_M_TPQ_PINYIN
                    || data_ptr->init_param.method_param_ptr->im_set.method == GUIIM_M_TPQ_ZHUYIN))  	
                {
                    notify_pack.data.commit_data.commit[0] = 0x3000;
                    notify_pack.data.commit_data.commit_len = 1;
                    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                    return TRUE;
                }
            }
            notify_pack.data.commit_data.commit[0] = strtab_ptr[1];
            notify_pack.data.commit_data.commit_len = 1;
        }

        /*下面这两个判断，如果当前有内容，则全部清空*/
        if (MMIIM_TP_CSTAR_STATE_INPUTING == data_ptr->ui_state ||
                MMIIM_TP_CSTAR_STATE_ASSOCIATE == data_ptr->ui_state ||
                MMIIM_TP_CSTAR_STATE_USER == data_ptr->ui_state)
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            //SetToNoInput(handle_ptr);
        }

        //增加此条件，是为了去掉underline的值 cr:191987
        if (0 < data_ptr->nUnderLine)
        {
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE | CS_VK_LONGCLICK);
        }

        //通知上屏
        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

        //上屏后，如果键盘是需要返回到上一次键盘状态的，则增加回调函数设置
        if ((g_keyboard_state[data_ptr->stateId].priv_data&MMIIM_TP_CSTAR_LOCK)
                && (!data_ptr->is_need_lock))
        {
            HandleReturn(data_ptr, event_data_ptr, id);
        }
    }

    data_ptr->bak_id = id;

    return TRUE;
}

/*****************************************************************************/
//  Description : 26键盘处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 9键和26键的区别是由输入法引擎决定的，主要是由CS_EVENT_KEYBOARD,CS_EVENT_CHAR
//  这两个不同的事件及后面跟的消息值不同引起的，我们在本地全局变量对按键对应的操作表中
//  配置事件时，要灵活选择，26: 'a', 'char'; 9: 'cs_vk_1','keyboard'
/*****************************************************************************/
PUBLIC BOOLEAN Handle26Key(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
)
{
    uint16 const* strtab_ptr = PNULL;
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    BOOLEAN result = TRUE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"[MMIIM] Handle26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3319_112_2_18_2_28_23_227,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr
        || data_ptr->stateId >= MMIIM_TP_CSTAR_KB_STATE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleChar: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3325_112_2_18_2_28_23_228,(uint8*)"");
        return FALSE;
    }

    if (id >= g_keyboard_state[data_ptr->stateId].char_cnt)
    {
        return TRUE;
    }

    strtab_ptr = g_keyboard_state[data_ptr->stateId].char_tab[id];

    if (PNULL == strtab_ptr)
    {
        return TRUE;
    }

    //处理回退删除掉键
    if (strtab_ptr == s_sym_del)
    {
        if (data_ptr->result_len > 0)
        {
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE);
        }
        else
        {
            notify_pack.data.commit_data.commit[0] = strtab_ptr[1];
            notify_pack.data.commit_data.commit_len = 1;
            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        data_ptr->bak_id = id;
    }
    else
    {
        result = HandleCstarEvent(data_ptr, CS_EVENT_CHAR, strtab_ptr[1]);
        data_ptr->bak_id = 0xff;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 9键盘处理
//  Global resource dependence : none
//  Author: rui.xu
//  Note: 说明同上，要注意回退键
/*****************************************************************************/
PUBLIC BOOLEAN Handle9Key(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
)
{
    uint16 const* strtab_ptr = PNULL;
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    BOOLEAN result = TRUE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    //SCI_TRACE_LOW:"[MMIIM] Handle9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3381_112_2_18_2_28_23_229,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr
        || data_ptr->stateId >= MMIIM_TP_CSTAR_KB_STATE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIM] Handle9Key: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3387_112_2_18_2_28_23_230,(uint8*)"");
        return FALSE;
    }

    if (id >= g_keyboard_state[data_ptr->stateId].char_cnt)
    {
        return TRUE;
    }

    strtab_ptr = g_keyboard_state[data_ptr->stateId].char_tab[id];

    if (PNULL == strtab_ptr)
    {
        return TRUE;
    }

    if (strtab_ptr == s_sym_del)
    {
        if (data_ptr->result_len > 0)
        {
            //由于候选字没有lable，如果不是编辑状态，先送一个delete
            if (CS_FLAG_STATE_CAND == data_ptr->cstar_window_state)
            {
                CS_IMM_CONTENT content = {0};
                CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
            }
            
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE);
        }
        else
        {
            if (data_ptr->nPreEdit > 0)
            {
                HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE);
            }
            else
            {
                notify_pack.data.commit_data.commit[0] = strtab_ptr[1];
                notify_pack.data.commit_data.commit_len = 1;
                notify_pack.notify = GUIIM_NOTIFY_COMMIT;
                GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            }
        }

        data_ptr->bak_id = id;
    }
    else if(' ' == strtab_ptr[1])
    {
        if(data_ptr->init_param.method_param_ptr && 
            (data_ptr->init_param.method_param_ptr->im_set.method == GUIIM_M_TP_PINYIN
		|| data_ptr->init_param.method_param_ptr->im_set.method == GUIIM_M_TP_ZHUYIN))  	//中文时，使用全角符号
        {
            notify_pack.data.commit_data.commit[0] = 0x3000;
            notify_pack.data.commit_data.commit_len = 1;
            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            return TRUE;
        }
        else
        {
            //目前针对数字0键，需求要求给配成空格，但是对于cstar内核来讲，希望送进去一个CS_VK_0
            result = HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, CS_VK_0);
            data_ptr->bak_id = 0xff;        
        }
    }
    else
    {
        //下面这个条件所做的事，必须说个明白：
        /*因为我们的TP输入法候选状态，总是做成没有没有打上label数字的，所以，就无所谓当输入法
        在候选状态时，当TP键盘上面的数字123，来实现上屏的操作，既然没有了，那么我们在9key时，
        其实按abc之类的，传给内核的，还是数字123，就会造成类似CR：218358这种现象，所以，我们要
        对如果再候选状态，用户接着输入1~9这个键，我们应该先把输入法内核回退到编译状态，再送。
        这样就保证了，其实我们维护的就是一个编辑状态*/
        if (CS_FLAG_STATE_CAND == data_ptr->cstar_window_state)
        {
            CS_IMM_CONTENT content = {0};
            CS_IMM_EventHandle(data_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_BACKSPACE, &content);
        }        
        //9键传得是cs_vk_1等
        result = HandleCstarEvent(data_ptr, CS_EVENT_KEYBOARD, strtab_ptr[1]);
        data_ptr->bak_id = 0xff;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 输入法选择
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleMethodSelect(
    void *handle_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_HANDLE_T * data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUI_BOTH_RECT_T pop_menu_rect = {0};

    //SCI_TRACE_LOW:"[MMIIM] HandleMethodSelect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3485_112_2_18_2_28_23_231,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleMethodSelect: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3490_112_2_18_2_28_23_232,(uint8*)"");
        return FALSE;
    }

    MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
    MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);

    return TRUE;
}

/*****************************************************************************/
//  Description : 大小写之间切换
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:true:发送改变通知；false:不需要切换大小写
/*****************************************************************************/
PUBLIC BOOLEAN HandleCaps(
    void *handle_ptr,                     //in:输入法句柄
    GUIIM_EVENT_DATA_U *event_data_ptr,   //in&out:传入传出参数
    uint32 id                              //in:传入的id号
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_CAPITAL_MODE_T cur_caps = GUIIM_MODE_MAX;
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"[MMIIM] HandleCaps"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3516_112_2_18_2_28_23_233,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr
        || PNULL == data_ptr->init_param.method_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleCaps: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3522_112_2_18_2_28_23_234,(uint8*)"");
        return FALSE;
    }

    //取当前输入法
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;
    cur_caps = data_ptr->init_param.method_param_ptr->im_set.capital;

    //过滤不需要切换大小写的输入法索引
    if (GUIIM_MODE_MAX <= cur_caps)
        return FALSE;

    //取不改变语言，却切换大小写的输入法索引
    next_method = MMIIMUI_GetNextCapsByMethod(cur_method, cur_caps);

    if (next_method == cur_method)
        return FALSE;

    //SCI_TRACE_LOW:"[MMIIM] HandleCaps: method=%04X, new_method=%04X."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3540_112_2_18_2_28_24_235,(uint8*)"dd", cur_method, next_method);

    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}

/*****************************************************************************/
//  Description : 26->9之间切换
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle26KeyTo9Key(
    void *handle_ptr,                  //in:输入法句柄
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id                           //in:传入的id号
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;

    //SCI_TRACE_LOW:"[MMIIM] Handle26KeyTo9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3565_112_2_18_2_28_24_236,(uint8*)"");
    
    if (PNULL == data_ptr ||PNULL == data_ptr->init_param.method_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] Handle26KeyTo9Key: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3569_112_2_18_2_28_24_237,(uint8*)"");
        return FALSE;
    }

    //检查当前是否已经是9键
    if (!data_ptr->is_full_keypad)
        return FALSE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);/*assert verified*/

    //应需求要求，如果当前是横屏就不必切换到9键
    if (MMITHEME_IsMainScreenLandscape())
    {
        return FALSE;
    }

    //取当前输入法
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;

    if (GUIIM_M_NONE == cur_method)
        return FALSE;    

    //取对应26键的tp输入法
    next_method = MMITHEME_IMGetMethodByInput(cur_method, GUIIM_INPUT_TYPE_TOUCHPEN);

    if (next_method == cur_method || GUIIM_M_NONE == next_method)
        return FALSE;

    //当前确实是全键盘，需要切换，修改标志，用于写nv
    data_ptr->is_full_keypad = FALSE;

    //SCI_TRACE_LOW:"[MMIIM] Handle26KeyTo9Key: method=%04X, new_method=%04X."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3600_112_2_18_2_28_24_238,(uint8*)"dd", cur_method, next_method);

    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}

/*****************************************************************************/
//  Description : 9->26之间切换
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle9KeyTo26Key(
    void *handle_ptr, //in:输入法句柄
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id  //in:传入的id号
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;

    //SCI_TRACE_LOW:"[MMIIM] Handle9KeyTo26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3625_112_2_18_2_28_24_239,(uint8*)"");
    
    if (PNULL == data_ptr ||PNULL == data_ptr->init_param.method_param_ptr )
    {
        //SCI_TRACE_LOW:"[MMIIM] Handle9KeyTo26Key: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3629_112_2_18_2_28_24_240,(uint8*)"");
        return FALSE;
    }

    //检查当前是否已经是26键
    if (data_ptr->is_full_keypad)
        return FALSE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);/*assert verified*/

    //取当前输入法
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;

    if (GUIIM_M_NONE == cur_method)
        return FALSE;    

    //取对应26键的tp输入法
    next_method = MMITHEME_IMGetMethodByInput(cur_method, GUIIM_INPUT_TYPE_TP_QWERT);

    if (next_method == cur_method || GUIIM_M_NONE == next_method)
        return FALSE;

    //当前确实是全键盘，需要切换，修改标志，用于写nv
    data_ptr->is_full_keypad = TRUE;

    //SCI_TRACE_LOW:"[MMIIM] Handle9KeyTo26Key: method=%04X, new_method=%04X."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3654_112_2_18_2_28_24_241,(uint8*)"dd", cur_method, next_method);

    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}

/*****************************************************************************/
//  Description : 锁键盘处理
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleLock(
    void *handle_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    //SCI_TRACE_LOW:"[MMIIM] HandleLock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3675_112_2_18_2_28_24_242,(uint8*)"");
    
    if (PNULL == handle_ptr
        || PNULL == event_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleLock: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3680_112_2_18_2_28_24_243,(uint8*)"");
        return FALSE;
    }

    if (data_ptr->is_need_lock)
    {
        data_ptr->is_need_lock = FALSE;
    }
    else
    {
        data_ptr->is_need_lock = TRUE;
    }

    data_ptr->lock_rect = data_ptr->press_rect;
    data_ptr->lock_img_rect = data_ptr->press_img_rect;

    //有些地方是先清掉痕迹，故这里先画上。每次保证画正确的图
    if (data_ptr->is_need_lock)
    {
        GUIRES_DisplayImg(PNULL, (GUI_RECT_T*) &data_ptr->lock_rect, (GUI_RECT_T*) &data_ptr->lock_img_rect,
                          data_ptr->init_param.win_handle, g_keyboard_state[data_ptr->stateId].press_image_id, data_ptr->lcd_info_ptr);
    }
    else
    {
        GUIRES_DisplayImg(PNULL, (GUI_RECT_T*) &data_ptr->lock_rect, (GUI_RECT_T*) &data_ptr->lock_img_rect,
                          data_ptr->init_param.win_handle, g_keyboard_state[data_ptr->stateId].normal_image_id, data_ptr->lcd_info_ptr);
    }

    data_ptr->bak_id = 0xff;
    return TRUE;
}

/*****************************************************************************/
//  Description : 切换到符号窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleSwicthSymbol(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;

    if (PNULL == data_ptr
        || PNULL == event_data_ptr
        || PNULL == data_ptr->init_param.method_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIM] HandleSwicthSymbol: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_CSTAR_3728_112_2_18_2_28_24_244,(uint8*)"");
        return FALSE;
    }
    
    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
        data_ptr->init_param.method_param_ptr->im_set.lang);

    return TRUE;
}



/******************************************************************************/
// Description: 处理TP消息
// Global resource dependence: NONE
// Author:
// Note:
/******************************************************************************/
LOCAL void HandleTpKeySelect(MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, uint16 tp_index)
{
    CS_UINT input_method = 0;

    input_method = CS_IMM_GetInputMethod(handle_ptr->cstar_handle);

    if (!handle_ptr->is_full_keypad
#if defined (IM_SIMP_CHINESE_SUPPORT)
            || MMIIM_TP_CSTAR_STATE_STROKE - MMIIM_TP_CSTAR_KB_START_ID == handle_ptr->stateId
#endif
#if defined (IM_TRAD_CHINESE_SUPPORT)
            || MMIIM_TP_CSTAR_STATE_TRAD_STROKE - MMIIM_TP_CSTAR_KB_START_ID == handle_ptr->stateId
#endif
       )
    {
        if (CS_FLAG_STATE_CAND != handle_ptr->cstar_window_state)
        {
            CS_IMM_CONTENT content = {0};
            CS_IMM_EventHandle(handle_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);
            SetUIStateByContent(handle_ptr, &content);
        }
        
        /*自选词时，按中的是第1位置的词，但必须模拟按中的是0k键*/
        if (MMIIM_TP_CSTAR_STATE_USER == handle_ptr->ui_state && 0 == tp_index)
        {
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_SELECT);
        }
        else
        {
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_1 + tp_index);
        }

        if (handle_ptr->result_len > 0 
            && MMIIM_TP_CSTAR_STATE_USER != handle_ptr->ui_state)
        {
            if (IM_PINYIN != input_method && IM_STROKE != input_method
                &&IM_ZHUYIN_MT != input_method && IM_STROKE_BIG5 != input_method && IM_PINYIN_DZ != input_method)
            {
                HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_SELECT);
            }
        }
    }
    else //26键
    {
        if (IM_STROKE != input_method && IM_STROKE_BIG5 != input_method)
        {
            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_1 + tp_index);
        }
        else
        {
            if (CS_FLAG_STATE_CAND != handle_ptr->cstar_window_state)
            {
                CS_IMM_CONTENT content = {0};
                (void) CS_IMM_EventHandle(handle_ptr->cstar_handle, CS_EVENT_KEYBOARD, CS_VK_DOWN, &content);
                SetUIStateByContent(handle_ptr, &content);
            }

            HandleCstarEvent(handle_ptr, CS_EVENT_KEYBOARD, CS_VK_1 + tp_index);
        }
    }
}

/*==============================================================================
Description: SetToNoInput
Global resource dependence: NONE
Author: haiwu.chen
Note:
==============================================================================*/
LOCAL void SetToNoInput(MMIIM_TP_CSTAR_HANDLE_T *data_ptr)
{
    if (PNULL == data_ptr)
    {
        return;
    }
    
    MMIIMUI_HideKeyTips(data_ptr);
        
    if (0 != data_ptr->tips_timer_id)
    {
        MMK_StopTimer(data_ptr->tips_timer_id);
        data_ptr->tips_timer_id = 0;
    }

    //我们这里需要重新设置输入法，其实最主要目的就是要把引擎的内容清干净
    if (PNULL != data_ptr->cstar_handle)
    {
        CS_IMM_SetInputMethod(data_ptr->cstar_handle, 
            CS_IMM_GetInputMethod(data_ptr->cstar_handle));
    }
        
    //alter the ui state
    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    /*处理完成后，还要保存内容到result中*/
    SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->result_len = 0;

    /*处理完成后保存编辑内容到pPreBuffer中*/
    SCI_MEMSET(data_ptr->pPreBuffer, 0, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nPreEdit = 0;

    /*处理完成后，保存underline的内容到pUnderLine中*/
    SCI_MEMSET(data_ptr->pUnderLine, 0, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nUnderLine = 0;

    MMIIMUI_ClearChoiceBarData(&data_ptr->choice_bar);
    
    //set the redraw signal
    data_ptr->is_rect_change = TRUE;
    data_ptr->is_bar_cand_dirty = TRUE;
    data_ptr->is_bar_choice_dirty = TRUE;
    data_ptr->is_bar_edit_dirty = TRUE;
    data_ptr->is_kb_dirty = TRUE;

    return;
}

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitImKernel(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    CS_HIMM *init_Ker_ptr
)
{
    CS_IMM_PARAMS im_params = {0};
    CS_USER_PARAMS user_params = {0};
    MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();
    
    user_params.pUserCache = PNULL;
    user_params.iUserCacheSize = 0;
    im_params.nMaxPYChoice = 1;
    im_params.nWinWidth = MMITHEME_IsMainScreenLandscape() ? MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H : MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V;
    im_params.nWinWidth -= MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H * 2; //头尾都要留出固定像素的间隔
    im_params.nLableWidth = GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;  //增加两个像素，中间宽一点，好看好选
    im_params.pGetTextWidthFunc = GetTextWidthCallback;
    *init_Ker_ptr = CS_IMM_Initialize(PNULL, &im_params, &user_params);

    return;
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

GUIIM_KERNEL_APIS_T const g_guiim_tp_cstar_apis = {0};

#endif //#if defined(IM_ENGINE_CSTAR) && defined (TOUCH_PANEL_SUPPORT)


