/*****************************************************************************
** File Name:      mmiim_tp_sogou.c                                          *
** Author:                                                                   *
** Date:           2009/09/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle pb menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifndef MMI_IM_PDA_SUPPORT
#include "mmi_app_im_trc.h"
#include "mmiim_sogou.h"
#ifdef MMIIM_USE_TP_SOGOU
#include "Os_api.h"
#include "mmiim_tp_cstar_position.h"
#include "guires.h"
#include "mmiim_internal.h"
#include "mmiim_im_switching.h"
#include "mmiim_sp_symbol.h"
#include "mmiim_tp_ui.h"
#include "mmk_timer.h"
#include "spml_api.h"
#include "mmiim.h"
#include "WD_Def.h"//6_12_2

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define IME_LIST_MAX        10

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// 搜狗输入法指针变量
extern  WD_imeResult g_imeResult;
// 搜狗输入法指针变量
extern uint8 g_FlashData[FLASH_CAPACITY];
// 当前显示的最后一个备选字索引
LOCAL  uint8        s_lastdisp = 10;

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
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T* param_ptr);

/*****************************************************************************/
//  Description : 析构输入法
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Terminate(void* handle_ptr);

/*****************************************************************************/
//  Description : 输入法的处理函数
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMsg(void* handle_ptr, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U* event_data_ptr);

/******************************************************************************/
// Description: 处理系统消息
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, GUIIM_EVENT_DATA_U* event_data_ptr);

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
LOCAL void SetUIStateByContent(MMIIM_TP_CSTAR_HANDLE_T *data_ptr);

LOCAL WD_UINT8 AsciiToWDK (uint8 ascii);

/*****************************************************************************/
//  Description : 处理cstar事件
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSogouEvent(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    uint16 key
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


/*******************************************************************************************/
//  Description : get last disp
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
PUBLIC uint8 MMIMLS9_GetLastDisp(void);

/*****************************************************************************/
//  Description :输入法事件响应函数
//  Global resource dependence : none
//  Author:
//  Note:event 事件的ID， param  触笔的坐标
/*****************************************************************************/
PUBLIC uint8 HandleIMEOnKeyEvent(uint16 key);

/*******************************************************************************************/
//  Description : Get ime Result
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
PUBLIC WD_imeResult* WD_GetImeResult(void);

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//guiim使用的函数表
PUBLIC GUIIM_KERNEL_APIS_T const g_guiim_tp_sogou_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

extern const uint16 s_sym_del[];
extern const uint16 s_sym_ent[];
extern const uint16 s_sym_sep[];
extern const uint16 s_sym_space[];

extern MMIIM_TP_CSTAR_STATE_T g_keyboard_state[MMIIM_TP_CSTAR_STATE_MAX - MMIIM_TP_CSTAR_KB_START_ID];

LOCAL MMIIM_SOGOU_METHOD_RELATION_T const s_tp_sogou_method_relation_list[] =
{
    //TP 9键
#ifdef IM_SIMP_CHINESE_SUPPORT
    {GUIIM_M_TP_PINYIN,                     WDI_CN_CPINYIN},
    {GUIIM_M_TP_STROKE,                     WDI_CN_BIHUA},
    {GUIIM_M_TP_ZHUYIN,                     WDI_CN_ZHUYIN},
#endif
#ifdef IM_TRAD_CHINESE_SUPPORT
    {GUIIM_M_TP_STROKE_CHT,                 WDI_CN_BIHUAF},
#endif
    {GUIIM_M_TP_123,                        WDI_DIGIT},
    {GUIIM_M_TP_ENGLISH_LEAD_UPPER,         WDI_ENGLISH+(2<<16)},
    {GUIIM_M_TP_ENGLISH_LOWER,              WDI_ENGLISH+(1<<16)},
    {GUIIM_M_TP_ENGLISH_UPPER,              WDI_ENGLISH+(3<<16)},
    {GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER,     WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TP_ENGLISH_ABC_LOWER,          WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TP_ENGLISH_ABC_UPPER,          WDI_ENGLISH_ABC+(3<<16)},
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_TP_ARABIC,                     WDI_ARABIC},
    {GUIIM_M_TP_ARABIC_ABC,                 WDI_ARABIC_ABC},
#endif
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_TP_FRENCH_LEAD_UPPER,          WDI_FRENCH+(2<<16)},
    {GUIIM_M_TP_FRENCH_LOWER,               WDI_FRENCH+(1<<16)},
    {GUIIM_M_TP_FRENCH_UPPER,               WDI_FRENCH+(3<<16)},
    {GUIIM_M_TP_FRENCH_ABC_LOWER,           WDI_FRENCH_ABC+(1<<16)},
    {GUIIM_M_TP_FRENCH_ABC_UPPER,           WDI_FRENCH_ABC+(3<<16)},
    {GUIIM_M_TP_GERMAN_LEAD_UPPER,          0},
#endif
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_TP_GERMAN_LOWER,               0},
    {GUIIM_M_TP_GERMAN_UPPER,               0},
#endif
#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_TP_GREEK_LEAD_UPPER,           0},
    {GUIIM_M_TP_GREEK_LOWER,                0},
    {GUIIM_M_TP_GREEK_UPPER,                0},
#endif
#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_TP_HINDI,                      0},
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_TP_HUNGARIAN_LEAD_UPPER,       0},
    {GUIIM_M_TP_HUNGARIAN_LOWER,            0},
    {GUIIM_M_TP_HUNGARIAN_UPPER,            0},
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_TP_INDONESIAN_LEAD_UPPER,      0},
    {GUIIM_M_TP_INDONESIAN_LOWER,           0},
    {GUIIM_M_TP_INDONESIAN_UPPER,           0},
#endif
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_TP_ITALIAN_LEAD_UPPER,         WDI_ITALIAN+(2<<16)},
    {GUIIM_M_TP_ITALIAN_LOWER,              WDI_ITALIAN+(1<<16)},
    {GUIIM_M_TP_ITALIAN_UPPER,              WDI_ITALIAN+(3<<16)},
    {GUIIM_M_TP_ITALIAN_ABC_LOWER,          WDI_ITALIAN_ABC+(1<<16)},
    {GUIIM_M_TP_ITALIAN_ABC_UPPER,          WDI_ITALIAN_ABC+(3<<16)},
#endif
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_TP_MALAY_LEAD_UPPER,           0},
    {GUIIM_M_TP_MALAY_LOWER,                0},
    {GUIIM_M_TP_MALAY_UPPER,                0},
#endif
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_TP_PERSIAN,                    0},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_TP_PORTUGUESE_LEAD_UPPER,      0},
    {GUIIM_M_TP_PORTUGUESE_LOWER,           0},
    {GUIIM_M_TP_PORTUGUESE_UPPER,           0},
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_TP_RUSSIAN_LEAD_UPPER,         0},
    {GUIIM_M_TP_RUSSIAN_LOWER,              0},
    {GUIIM_M_TP_RUSSIAN_UPPER,              0},
#endif
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_TP_SPANISH_LEAD_UPPER,         0},
    {GUIIM_M_TP_SPANISH_LOWER,              0},
    {GUIIM_M_TP_SPANISH_UPPER,              0},
#endif
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_TP_TAGALOG,                    0},
#endif
#ifdef IM_THAI_SUPPORT
    {GUIIM_M_TP_THAI,                       0},
#endif
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_TP_TURKISH_LEAD_UPPER,         0},
    {GUIIM_M_TP_TURKISH_LOWER,              0},
    {GUIIM_M_TP_TURKISH_UPPER,              0},
#endif
#ifdef IM_URDU_SUPPORT
    {GUIIM_M_TP_URDU,                       0},
#endif
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_TP_HEBREW,                     0},
#endif
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_TP_BENGALI,                    0},
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_TP_VIETNAMESE_LEAD_UPPER,      0},
    {GUIIM_M_TP_VIETNAMESE_LOWER,           0},
    {GUIIM_M_TP_VIETNAMESE_UPPER,           0},
#endif
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_TP_CZECH_LEAD_UPPER,           0},
    {GUIIM_M_TP_CZECH_LOWER,                0},
    {GUIIM_M_TP_CZECH_UPPER,                0},
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_TP_SLOVENIAN_LEAD_UPPER,       0},
    {GUIIM_M_TP_SLOVENIAN_LOWER,            0},
    {GUIIM_M_TP_SLOVENIAN_UPPER,            0},
#endif
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_TP_ROMANIAN_LEAD_UPPER,        0},
    {GUIIM_M_TP_ROMANIAN_LOWER,             0},
    {GUIIM_M_TP_ROMANIAN_UPPER,             0},
#endif
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_TP_CROATIAN_LEAD_UPPER,        0},
    {GUIIM_M_TP_CROATIAN_LOWER,             0},
    {GUIIM_M_TP_CROATIAN_UPPER,             0},
#endif
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_TP_DUTCH_LEAD_UPPER,           0},
    {GUIIM_M_TP_DUTCH_LOWER,                0},
    {GUIIM_M_TP_DUTCH_UPPER,                0},
#endif
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_TP_BULGARIAN_LEAD_UPPER,       0},
    {GUIIM_M_TP_BULGARIAN_LOWER,            0},
    {GUIIM_M_TP_BULGARIAN_UPPER,            0},
#endif
#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_TP_POLISH_LEAD_UPPER,          0},
    {GUIIM_M_TP_POLISH_LOWER,               0},
    {GUIIM_M_TP_POLISH_UPPER,               0},
#endif
#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_TP_DANISH_LEAD_UPPER,          0},
    {GUIIM_M_TP_DANISH_LOWER,               0},
    {GUIIM_M_TP_DANISH_UPPER,               0},
#endif
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_TP_SLOVAK_LEAD_UPPER,          0},
    {GUIIM_M_TP_SLOVAK_LOWER,               0},
    {GUIIM_M_TP_SLOVAK_UPPER,               0},
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_TP_LITHUANIAN_LEAD_UPPER,      0},
    {GUIIM_M_TP_LITHUANIAN_LOWER,           0},
    {GUIIM_M_TP_LITHUANIAN_UPPER,           0},
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_TP_NORWEGIAN_LEAD_UPPER,       0},
    {GUIIM_M_TP_NORWEGIAN_LOWER,            0},
    {GUIIM_M_TP_NORWEGIAN_UPPER,            0},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_TP_SWEDISH_LEAD_UPPER,         0},
    {GUIIM_M_TP_SWEDISH_LOWER,              0},
    {GUIIM_M_TP_SWEDISH_UPPER,              0},
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_TP_UKRAINIAN_LEAD_UPPER,       0},
    {GUIIM_M_TP_UKRAINIAN_LOWER,            0},
    {GUIIM_M_TP_UKRAINIAN_UPPER,            0},
#endif

    //tp 26键
#ifdef IM_SIMP_CHINESE_SUPPORT
    {GUIIM_M_TPQ_PINYIN,                    WDI_CN_FULLPINYIN},
    {GUIIM_M_TPQ_STROKE,                    WDI_CN_BIHUA},
    {GUIIM_M_TPQ_ZHUYIN,                    WDI_CN_ZHUYIN},
#endif
#ifdef IM_TRAD_CHINESE_SUPPORT
    {GUIIM_M_TPQ_STROKE_CHT,                WDI_CN_BIHUAF},
#endif
    {GUIIM_M_TPQ_123,                       WDI_DIGIT},
    {GUIIM_M_TPQ_ENGLISH_LEAD_UPPER,        WDI_ENGLISH+(2<<16)},
    {GUIIM_M_TPQ_ENGLISH_LOWER,             WDI_ENGLISH+(1<<16)},
    {GUIIM_M_TPQ_ENGLISH_UPPER,             WDI_ENGLISH+(3<<16)},
    {GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER,    WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_ENGLISH_ABC_LOWER,         WDI_ENGLISH_ABC+(1<<16)},
    {GUIIM_M_TPQ_ENGLISH_ABC_UPPER,         WDI_ENGLISH_ABC+(3<<16)},
#ifdef IM_ARABIC_SUPPORT
    {GUIIM_M_TPQ_ARABIC,                    WDI_ARABIC},
    {GUIIM_M_TPQ_ARABIC_ABC,                WDI_ARABIC_ABC},
#endif
#ifdef IM_FRENCH_SUPPORT
    {GUIIM_M_TPQ_FRENCH_LEAD_UPPER,         WDI_FRENCH+(2<<16)},
    {GUIIM_M_TPQ_FRENCH_LOWER,              WDI_FRENCH+(1<<16)},
    {GUIIM_M_TPQ_FRENCH_UPPER,              WDI_FRENCH+(3<<16)},
    {GUIIM_M_TPQ_FRENCH_ABC_LOWER,          WDI_FRENCH_ABC+(1<<16)},
    {GUIIM_M_TPQ_FRENCH_ABC_UPPER,          WDI_FRENCH_ABC+(3<<16)},
#endif
#ifdef IM_GERMAN_SUPPORT
    {GUIIM_M_TPQ_GERMAN_LEAD_UPPER,         0},
    {GUIIM_M_TPQ_GERMAN_LOWER,              0},
    {GUIIM_M_TPQ_GERMAN_UPPER,              0},
#endif
#ifdef IM_GREEK_SUPPORT
    {GUIIM_M_TPQ_GREEK_LEAD_UPPER,          0},
    {GUIIM_M_TPQ_GREEK_LOWER,               0},
    {GUIIM_M_TPQ_GREEK_UPPER,               0},
#endif
#ifdef IM_HINDI_SUPPORT
    {GUIIM_M_TPQ_HINDI,                     0},
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {GUIIM_M_TPQ_HUNGARIAN_LEAD_UPPER,      0},
    {GUIIM_M_TPQ_HUNGARIAN_LOWER,           0},
    {GUIIM_M_TPQ_HUNGARIAN_UPPER,           0},
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {GUIIM_M_TPQ_INDONESIAN_LEAD_UPPER,     0},
    {GUIIM_M_TPQ_INDONESIAN_LOWER,          0},
    {GUIIM_M_TPQ_INDONESIAN_UPPER,          0},
#endif
#ifdef IM_ITALIAN_SUPPORT
    {GUIIM_M_TPQ_ITALIAN_LEAD_UPPER,        WDI_ITALIAN+(2<<16)},
    {GUIIM_M_TPQ_ITALIAN_LOWER,             WDI_ITALIAN+(1<<16)},
    {GUIIM_M_TPQ_ITALIAN_UPPER,             WDI_ITALIAN+(3<<16)},
    {GUIIM_M_TPQ_ITALIAN_ABC_LOWER,         WDI_ITALIAN_ABC+(1<<16)},
    {GUIIM_M_TPQ_ITALIAN_ABC_UPPER,         WDI_ITALIAN_ABC+(3<<16)},
#endif
#ifdef IM_MALAY_SUPPORT
    {GUIIM_M_TPQ_MALAY_LEAD_UPPER,          0},
    {GUIIM_M_TPQ_MALAY_LOWER,               0},
    {GUIIM_M_TPQ_MALAY_UPPER,               0},
#endif
#ifdef IM_PERSIAN_SUPPORT
    {GUIIM_M_TPQ_PERSIAN,                   0},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {GUIIM_M_TPQ_PORTUGUESE_LEAD_UPPER,     0},
    {GUIIM_M_TPQ_PORTUGUESE_LOWER,          0},
    {GUIIM_M_TPQ_PORTUGUESE_UPPER,          0},
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {GUIIM_M_TPQ_RUSSIAN_LEAD_UPPER,        0},
    {GUIIM_M_TPQ_RUSSIAN_LOWER,             0},
    {GUIIM_M_TPQ_RUSSIAN_UPPER,             0},
#endif
#ifdef IM_SPANISH_SUPPORT
    {GUIIM_M_TPQ_SPANISH_LEAD_UPPER,        0},
    {GUIIM_M_TPQ_SPANISH_LOWER,             0},
    {GUIIM_M_TPQ_SPANISH_UPPER,             0},
#endif
#ifdef IM_TAGALOG_SUPPORT
    {GUIIM_M_TPQ_TAGALOG,                   0},
#endif
#ifdef IM_THAI_SUPPORT
    {GUIIM_M_TPQ_THAI,                      0},
#endif
#ifdef IM_TURKISH_SUPPORT
    {GUIIM_M_TPQ_TURKISH_LEAD_UPPER,        0},
    {GUIIM_M_TPQ_TURKISH_LOWER,             0},
    {GUIIM_M_TPQ_TURKISH_UPPER,             0},
#endif
#ifdef IM_URDU_SUPPORT
    {GUIIM_M_TPQ_URDU,                      0},
#endif
#ifdef IM_HEBREW_SUPPORT
    {GUIIM_M_TPQ_HEBREW,                    0},
#endif
#ifdef IM_BENGALI_SUPPORT
    {GUIIM_M_TPQ_BENGALI,                   0},
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {GUIIM_M_TPQ_VIETNAMESE_LEAD_UPPER,     0},
    {GUIIM_M_TPQ_VIETNAMESE_LOWER,          0},
    {GUIIM_M_TPQ_VIETNAMESE_UPPER,          0},
#endif
#ifdef IM_CZECH_SUPPORT
    {GUIIM_M_TPQ_CZECH_LEAD_UPPER,          0},
    {GUIIM_M_TPQ_CZECH_LOWER,               0},
    {GUIIM_M_TPQ_CZECH_UPPER,               0},
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    {GUIIM_M_TPQ_SLOVENIAN_LEAD_UPPER,      0},
    {GUIIM_M_TPQ_SLOVENIAN_LOWER,           0},
    {GUIIM_M_TPQ_SLOVENIAN_UPPER,           0},
#endif
#ifdef IM_ROMANIAN_SUPPORT
    {GUIIM_M_TPQ_ROMANIAN_LEAD_UPPER,       0},
    {GUIIM_M_TPQ_ROMANIAN_LOWER,            0},
    {GUIIM_M_TPQ_ROMANIAN_UPPER,            0},
#endif
#ifdef IM_CROATIAN_SUPPORT
    {GUIIM_M_TPQ_CROATIAN_LEAD_UPPER,       0},
    {GUIIM_M_TPQ_CROATIAN_LOWER,            0},
    {GUIIM_M_TPQ_CROATIAN_UPPER,            0},
#endif
#ifdef IM_DUTCH_SUPPORT
    {GUIIM_M_TPQ_DUTCH_LEAD_UPPER,          0},
    {GUIIM_M_TPQ_DUTCH_LOWER,               0},
    {GUIIM_M_TPQ_DUTCH_UPPER,               0},
#endif
#ifdef IM_BULGARIAN_SUPPORT
    {GUIIM_M_TPQ_BULGARIAN_LEAD_UPPER,      0},
    {GUIIM_M_TPQ_BULGARIAN_LOWER,           0},
    {GUIIM_M_TPQ_BULGARIAN_UPPER,           0},
#endif
#ifdef IM_POLISH_SUPPORT
    {GUIIM_M_TPQ_POLISH_LEAD_UPPER,         0},
    {GUIIM_M_TPQ_POLISH_LOWER,              0},
    {GUIIM_M_TPQ_POLISH_UPPER,              0},
#endif
#ifdef IM_DANISH_SUPPORT
    {GUIIM_M_TPQ_DANISH_LEAD_UPPER,         0},
    {GUIIM_M_TPQ_DANISH_LOWER,              0},
    {GUIIM_M_TPQ_DANISH_UPPER,              0},
#endif
#ifdef IM_SLOVAK_SUPPORT
    {GUIIM_M_TPQ_SLOVAK_LEAD_UPPER,         0},
    {GUIIM_M_TPQ_SLOVAK_LOWER,              0},
    {GUIIM_M_TPQ_SLOVAK_UPPER,              0},
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    {GUIIM_M_TPQ_LITHUANIAN_LEAD_UPPER,     0},
    {GUIIM_M_TPQ_LITHUANIAN_LOWER,          0},
    {GUIIM_M_TPQ_LITHUANIAN_UPPER,          0},
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    {GUIIM_M_TPQ_NORWEGIAN_LEAD_UPPER,      0},
    {GUIIM_M_TPQ_NORWEGIAN_LOWER,           0},
    {GUIIM_M_TPQ_NORWEGIAN_UPPER,           0},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {GUIIM_M_TPQ_SWEDISH_LEAD_UPPER,        0},
    {GUIIM_M_TPQ_SWEDISH_LOWER,             0},
    {GUIIM_M_TPQ_SWEDISH_UPPER,             0},
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    {GUIIM_M_TPQ_UKRAINIAN_LEAD_UPPER,      0},
    {GUIIM_M_TPQ_UKRAINIAN_LOWER,           0},
    {GUIIM_M_TPQ_UKRAINIAN_UPPER,           0},
#endif
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
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state;                     //应用程序键盘状态

    //SCI_ASSERT(PNULL != param_ptr);     /*assert verified*/
    //SCI_ASSERT(PNULL != param_ptr->lcd_info_ptr);   /*assert verified*/
    if(PNULL == param_ptr || PNULL == param_ptr->lcd_info_ptr)
    {
        return  PNULL;
    }
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

    WD_ImeInit(0, MMIAPIIM_GetLanguage());

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

    //为空表示未配置
    //默认配置，默认配置需要重写，因切换语言可能修改该值，而初始化配置不需要该值
    if(g_keyboard_state[handle_ptr->stateId].p_disp_text == NULL
        || g_keyboard_state[handle_ptr->stateId].p_disp_text == MMIIMUI_MulKb26KeyGetDefData() /*lint !e746*/
        || g_keyboard_state[handle_ptr->stateId].p_disp_text == MMIIMUI_MulKb9KeyGetDefData()) /*lint !e746*/
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

    //SCI_ASSERT(data_ptr != PNULL);      /*assert verified*/
    if(PNULL == data_ptr)
    {
        return  FALSE;
    }
    
    if (PNULL != data_ptr->cstar_handle)
    {
        SG_Quit();
    }

    /*这个检查是有点多余的，如果以后横屏也支持9键的话，那就不必此条件，现在的逻辑就是横屏不保存也不修改9，26状态，均设置为26键*/
    if (!MMITHEME_IsMainScreenLandscape())
        MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);

    if (0 != data_ptr->tips_timer_id)
    {
        MMK_StopTimer(data_ptr->tips_timer_id);
        data_ptr->tips_timer_id = 0;
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&(data_ptr->tips_lcd_info));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(data_ptr->tips_lcd_info));
//        data_ptr->tips_lcd_info.block_id = UILAYER_NULL_HANDLE;
    }

    SCI_FREE(data_ptr);
#ifdef SOGOU_USE_PDA_SKIN
    MMIAPIIM_SGFreeImeSkinBuffer();
#endif    
    return TRUE; /*lint !e438*/
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

    data_ptr->event_data_ptr = event_data_ptr;
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX);   /*assert verified*/
    if(PNULL == data_ptr || data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return  FALSE;
    }
    
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
            //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
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
            if (data_ptr->nPreEdit > 0)
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
    BOOLEAN is_dealt = FALSE;
    MMIIM_TP_CSTAR_MSG_T msg = {0};

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX);   /*assert verified*/
    if(PNULL == data_ptr || data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
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

    default:
        break;
    }

    if (!is_dealt)
    {
        if ((msg.para.key == KEY_DOWN || msg.para.key == KEY_UP || msg.para.key == KEY_LEFT || msg.para.key == KEY_RIGHT) &&
            WD_GetImeResult()->imeStatus != WD_STATUS_CHINESE_NORMAL &&
            WD_GetImeResult()->imeStatus != WD_STATUS_FOREIGN_NORMAL &&
            WD_GetImeResult()->imeStatus != WD_STATUS_DIGIT_INPUT)
        {
            return TRUE;
        }
        
        return FALSE;
    }

    MMIIMUI_ChangeRect(data_ptr);

    return is_dealt;
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
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);  /*assert verified*/
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

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL ==msg_ptr)
    {
        return TRUE;
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
        if (msg_ptr->para.key == KEY_CANCEL)
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
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == msg_ptr)
    {
        return TRUE;
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
                        MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();

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
                        tp_point.x += GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;
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
                        HandleSogouEvent(data_ptr, WDK_OK);

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
                    HandleSogouEvent(data_ptr, WDK_C);

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
                    if (WD_GetImeResult()->returnKey)
                    {
                        notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                    }

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
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == msg_ptr)
    {
        return TRUE;
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
                        MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();

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
                        tp_point.x += GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;
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
                    HandleSogouEvent(data_ptr, WDK_C);

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
                    if (WD_GetImeResult()->returnKey)
                    {
                        notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                    }

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
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
    {
        return TRUE;
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
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
    {
        return TRUE;
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
    MMIIM_STATE_FUNC func;
    int16 count;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state;

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
    {
        return TRUE;
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

    state = g_keyboard_state[data_ptr->stateId];
    count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);
    bk_ui_state = data_ptr->ui_state;

    func = state.functab[count].func;

    if (!func)
    {
        return TRUE;
    }

    func(data_ptr, data_ptr->event_data_ptr, state.functab[count].id);

    if (bk_ui_state != data_ptr->ui_state)
    {
        data_ptr->is_rect_change = TRUE;
    }

    if (!data_ptr->is_rect_change)
    {
        MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
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
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state;

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
    {
        return TRUE;
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

                MMIIMUI_DrawEditBar(data_ptr);
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
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state;

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
    {
        return TRUE;
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
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state;

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr);  /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
    {
        return TRUE;
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
        if(0==data_ptr->stateId)
        {
            HandleIMEOnKeyEvent(WDK_LONG_C);
        }
        else if (g_keyboard_state[data_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
        {
            // SetToNoInput(data_ptr);
            HandleIMEOnKeyEvent(WDK_LONG_C);
        }
        if (WD_GetImeResult()->returnKey)
        {
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

    //SCI_ASSERT(PNULL != handle_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != handle_ptr->lcd_info_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    if(PNULL == handle_ptr || PNULL == handle_ptr->lcd_info_ptr ||PNULL == event_data_ptr)
    {
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
        if (handle_ptr->result_len > 0)
        {
            /*候选词状态*/
            if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
            {
                uint8 direction = MMIIM_TPGetAligntype(handle_ptr->init_param.method_param_ptr->im_set.method);
                font = text_bar_theme->font_text;
                font_rect.top = handle_ptr->cand_rect.top;
                font_rect.bottom = handle_ptr->cand_rect.bottom;
                if(MMIIM_DRAW_RIGHT2LEFT == direction)
                {
                    font_rect.right = text_rect.right - 2;
                }
                font_rect.left = handle_ptr->cand_rect.left + nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X;
                label_width = GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;
                font_rect.left += label_width;
                iLoop = 0;

                while (iLoop++ < handle_ptr->result_len)
                {
                    jLoop = 0;

                    while (handle_ptr->result[pos+ (jLoop++)] != '\0')
                        NULL;

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
                        if (((MMIIM_DRAW_RIGHT2LEFT == direction) && (font_rect.left >= text_rect.left || iLoop == 1)) ||
                            ((MMIIM_DRAW_LEFT2RIGHT == direction) && (font_rect.right <= text_rect.right || iLoop == 1)))
                        {
                            //接着处理下一级联想
                            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
                            handle_ptr->tp_last_msg_param.idx = -1;
                            HandleTpKeySelect(handle_ptr, iLoop - 1);
                            break;
                        }
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
        if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY && (WD_GetImeResult()->curPage || WD_GetImeResult()->imeStatus == WD_STATUS_FOREIGN_SELECTLETTER))
        {
            SetUIStateByContent(handle_ptr);
            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
            handle_ptr->tp_last_msg_param.idx = -1;
            HandleSogouEvent(handle_ptr, WDK_UP);
        }
    }
    else if (GUI_PointIsInRect(tp_point, handle_ptr->down_rect))
    {
        if (0 == (WD_GetImeResult()->selecting[MMIMLS9_GetLastDisp()][0] )
            || (WD_GetImeResult()->imeType < WDI_FOREIGN_BEGIN && WD_GetImeResult()->curPage == MAX_PAGE_FOR_SOGOU - 1)
            || (WD_GetImeResult()->imeType > WDI_FOREIGN_BEGIN && WD_GetImeResult()->curPage == 19 )
            || (WD_GetImeResult()->imeType < WDI_FOREIGN_BEGIN && WD_GetImeResult()->imeType > WDI_DIGIT
            && MMIMLS9_GetLastDisp() >= WD_GetImeResult()->selectingCount
            && WD_GetImeResult()->imeStatus != WD_STATUS_HOTKEY_SYMBOL && WD_GetImeResult()->imeStatus != WD_STATUS_DIGIT_INPUT )
            )
        {
            return;
        }
        if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
        {

            if (WD_GetImeResult()->imeStatus != WD_STATUS_CHINESE_SELECT
                && WD_GetImeResult()->imeStatus != WD_STATUS_FOREIGN_SELECT
                && WD_GetImeResult()->imeStatus != WD_STATUS_FOREIGN_SELECTLETTER)
            {
                HandleIMEOnKeyEvent(WDK_DOWN);
            }

            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
            handle_ptr->tp_last_msg_param.idx = -1;
            HandleSogouEvent(handle_ptr, WDK_DOWN);
        }
    }
	//SCI_TRACE_LOW:"TpCandPressUp iLoop=%d,font=%d,label_width=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2038_112_2_18_2_28_52_245,(uint8*)"ddd",iLoop,font,label_width);
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
    //SCI_ASSERT(PNULL != bar_ptr);  /*assert verified*/

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
    else
    {
        return;
    }

    *dirty_flg_ptr = TRUE;
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
    int i = 0;
    int j = 0;
    int l;
    //int8 move_cnt = 0;          //此参数表示要移动的次数，负数表示左移，正数表示右移

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != bar_ptr);  /*assert verified*/

    if (idx < 0 ||PNULL == data_ptr||PNULL == bar_ptr)
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

        WD_SetVariable(2, (bar_ptr->page_index - 1) * bar_ptr->choice_line_cnt + 1, WD_GetImeResult());

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

        WD_SetVariable(2, (bar_ptr->page_index + 1) * bar_ptr->choice_line_cnt + 1, WD_GetImeResult());

        //移动当前总序中的索引，放在下一页的第一个
        bar_ptr->choice_last_index = 0;
        bar_ptr->is_choice_state = TRUE;
    }
    else if (-1 < bar_ptr->hot_index && bar_ptr->hot_index < bar_ptr->cur_page_line_cnt)
    {
        WD_SetVariable(2, bar_ptr->page_index * bar_ptr->choice_line_cnt + 1 + bar_ptr->choice_index, WD_GetImeResult());

        bar_ptr->choice_last_index = bar_ptr->choice_index;
        bar_ptr->is_choice_state = TRUE;
    }
    else
    {
        return FALSE;
    }

    while (g_imeResult.selecting[j][0] && (i < (CAND_MAX_NUM - 1)))
    {
        l = 0;
        do
        {
            data_ptr->result[i] = g_imeResult.selecting[j][l];
            l++;
            i++;
        } while (g_imeResult.selecting[j][l]);
        data_ptr->result[i] = '\0';
        i++;
        j++;
    }
    data_ptr->result_len = j;

    /*处理完成后保存编辑内容到pPreBuffer中*/

    SCI_MEMCPY(data_ptr->pPreBuffer, WD_GetImeResult()->outputKeys, CAND_MAX_NUM * (sizeof(wchar)));
    data_ptr->nPreEdit = MMIAPICOM_Wstrlen(WD_GetImeResult()->outputKeys);

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
    uint16 i;
    uint16 im_cnt = ARR_SIZE(s_tp_sogou_method_relation_list);
    uint32 cs_im = GUIIM_M_NONE;

    if(GUIIM_M_NONE == method)
        return cs_im;

    for(i = 0; i < im_cnt; i++)
    {
        if(s_tp_sogou_method_relation_list[i].guiim_method == method)
        {
            cs_im = s_tp_sogou_method_relation_list[i].sogou_method;
            break;
        }
    }

    return cs_im;
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
    uint32 input_method;

    //SCI_ASSERT(PNULL != handle_ptr);  /*assert verified*/
    if (PNULL == handle_ptr)
    {
        return;
    }
    
    MMIIMUI_InitUIHandleData(handle_ptr, kb_state);

//    SCI_ASSERT(PNULL != handle_ptr->cstar_handle);

    input_method = ConvertMethodtoState(method, handle_ptr->is_full_keypad);

    WD_SwitchIme(input_method&0x0000ffff);

    switch(input_method>>16)
    {
    case 2:
        WD_SwitchForeignCapslock(1);
        break;
    case 3:
        WD_SwitchForeignCapslock(2);
        break;
    case 1:
        WD_SwitchForeignCapslock(0);
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 由cstar返回的内容设置当前UI状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetUIStateByContent(MMIIM_TP_CSTAR_HANDLE_T *data_ptr)
{
    int i = 0;
    int j = 0;
    int l;
    unsigned short letterSelecting[MAX_KEY_LETTERCOUNT*(MAX_PINYIN_LENGTH+1)];

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if (PNULL == data_ptr)
    {
        return;
    }
    
    SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM);

    /*处理完成后，还要保存内容到result中*/

    while (g_imeResult.selecting[j][0] && (i < (CAND_MAX_NUM - 1)))
    {
        l = 0;
        do
        {
            data_ptr->result[i] = g_imeResult.selecting[j][l];
            l++;
            i++;
        } while (g_imeResult.selecting[j][l]);
        data_ptr->result[i] = '\0';
        i++;
        j++;
    }
    data_ptr->result_len = j;

    /*处理完成后保存编辑内容到pPreBuffer中*/

    SCI_MEMCPY(data_ptr->pPreBuffer, WD_GetImeResult()->outputKeys, CAND_MAX_NUM * (sizeof(wchar)));
    data_ptr->nPreEdit = MMIAPICOM_Wstrlen(WD_GetImeResult()->outputKeys);

    j = 0;
    i = 0;
    while (g_imeResult.letterSelecting[j][0] && (i < (MAX_KEY_LETTERCOUNT * (MAX_PINYIN_LENGTH + 1) - 1)))
    {
        l = 0;
        do
        {
            letterSelecting[i] = g_imeResult.letterSelecting[j][l];
            l++;
            i++;
        } while (g_imeResult.letterSelecting[j][l]);
        letterSelecting[i] = '\0';
        i++;
        j++;
    }

    if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr)/* && WD_GetImeResult()->imeStatus == WD_STATUS_CHINESE_INPUT*/)
    {
        if (g_imeResult.imeType >= WDI_FOREIGN_BEGIN)
        {
            MMIIMUI_GetChoiceBufferByKernel(&data_ptr->choice_bar, PNULL, 0);
        }
        else
        {
        MMIIMUI_GetChoiceBufferByKernel(&data_ptr->choice_bar, letterSelecting, j);
        }
    }

    /*根据返回的内容来检查当前输入法的UI显示状态
    如果有commit内容，则一定没有preedit内容，则检查是否有候选字*/

    if (0 == data_ptr->nPreEdit)
    {
        if (0 < data_ptr->result_len)
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
    else
    {
        data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;
    }
}

/*****************************************************************************/
//  Description : 处理cstar事件
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSogouEvent(MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, uint16 key)
{
    //CS_RESULT result = TRUE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    uint8 oldIdxFocus = WD_GetImeResult()->idxFocus;

    //SCI_ASSERT(PNULL != handle_ptr);  /*assert verified*/
    if (PNULL == handle_ptr)
    {
        return  FALSE;
    }
    
    switch (HandleIMEOnKeyEvent(key))
    {
    case ERROR_INPUT_KEYS:
        if (oldIdxFocus != WD_GetImeResult()->idxFocus)
        {
            SetUIStateByContent(handle_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"mmiim_tp_sogou.c: HandleSogouEvent: HandleImeOnKeyEvent return ERROR_INPUT_KEYS!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2395_112_2_18_2_28_53_246,(uint8*)"");
        }
        break;

    default:
        SetUIStateByContent(handle_ptr);
        break;
    }

    handle_ptr->pageflag = 0;

    if (g_imeResult.output[0])
    {
        uint32 len_1 = sizeof(notify_pack.data.commit_data.commit);
        uint32 len_2 = MMIAPICOM_Wstrlen(g_imeResult.output);
        //SCI_ASSERT(PNULL != handle_ptr->event_data_ptr);  /*assert verified*/
        notify_pack.data.commit_data.commit_len = MIN(len_1, len_2);
        SCI_MEMCPY(notify_pack.data.commit_data.commit, g_imeResult.output,
            notify_pack.data.commit_data.commit_len * sizeof(g_imeResult.output[0]));

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(handle_ptr->init_param.im_handle, &notify_pack);
    }

    MMIIMUI_HideKeyTipsEx(handle_ptr);

    //管显示刷新
    switch (handle_ptr->ui_state)
    {
    case MMIIM_TP_CSTAR_STATE_INPUTING:

        if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_ED)
        {
            MMIIMUI_DrawEditBar(handle_ptr);
        }

        //lint -fallthrough
    case MMIIM_TP_CSTAR_STATE_ASSOCIATE:

        if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_CAND)
        {
            MMIIMUI_DrawCandBar(handle_ptr);
        }

        //lint -fallthrough
//         case MMIIM_TP_CSTAR_STATE_NOINPUT:
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
                MMIIMUI_DrawChoiceBar(&handle_ptr->choice_bar);
            }
        }
        break;
    default:
        break;
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
    MMIIM_TP_CSTAR_STATE_ID_T kb_state;

    //SCI_TRACE_LOW:"HandleReturn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2476_112_2_18_2_28_53_247,(uint8*)"");
    //SCI_ASSERT(PNULL != handle_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    if (PNULL == handle_ptr || PNULL == event_data_ptr )
    {
        return  FALSE;
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

    //SCI_TRACE_LOW:"HandleSwicth"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2507_112_2_18_2_28_53_248,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr )
    {
        return  FALSE;
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
    uint16 const* strtab_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"HandleChar"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2558_112_2_18_2_28_53_249,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return  FALSE;
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
        if (strtab_ptr[1] == s_sym_ent[1])
        {
            HandleSogouEvent(handle_ptr, WDK_ENTER);
            if (WD_GetImeResult()->returnKey == 0)
            {
                return TRUE;
            }
        }
        else if(strtab_ptr[1] == s_sym_sep[1])
        {
            HandleSogouEvent(handle_ptr, WDK_SEPERATER);
            if (WD_GetImeResult()->returnKey == 0)
            {
                return TRUE;
            }
        }
        else if(strtab_ptr[1] == s_sym_space[1])
        {
            HandleSogouEvent(handle_ptr, WDK_SPACE);
            if (WD_GetImeResult()->returnKey == 0)
            {
                return TRUE;
            }
        }

        //我们这里只管上屏一个字符，一般用于配置不需要输入法内核的键盘状态配置中
        if (strtab_ptr[1] != s_sym_del[1] || (data_ptr->result_len == 0))
        {
            notify_pack.data.commit_data.commit[0] = strtab_ptr[1];
            notify_pack.data.commit_data.commit_len = 1;
        }

        /*下面这两个判断，如果当前有内容，则全部清空*/
        if (MMIIM_TP_CSTAR_STATE_INPUTING == data_ptr->ui_state ||
                MMIIM_TP_CSTAR_STATE_ASSOCIATE == data_ptr->ui_state)
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            //SetToNoInput(handle_ptr);
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
    uint16 const* strtab_ptr;
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"Handle26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2656_112_2_18_2_28_53_250,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return  FALSE;
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
            HandleSogouEvent(handle_ptr, WDK_C);
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
        WD_UINT8 wd_key = 0;

        wd_key = AsciiToWDK (strtab_ptr[1]);
        HandleSogouEvent(data_ptr, wd_key);
        data_ptr->bak_id = 0xff;
    }

    return TRUE;
}


LOCAL WD_UINT8 AsciiToWDK (uint8 ascii)
{
    WD_UINT16 key = 0;
    
    if (ascii >= '1' && ascii <= '9')
    {
        key = WDK_NORMAL_1 + (ascii - '1');
    }
    else if (ascii == '0')
    {
        key = WDK_NORMAL_0;
    }
    else if (ascii >= 'a' && ascii <= 'z')
    {
        key = WDK_NORMAL_A + (ascii - 'a');
    }
    else if (ascii >= 'A' && ascii <= 'Z')
    {
        key = WDK_NORMAL_A + (ascii - 'A');
    }
    
    return key;
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
    uint16 const* strtab_ptr;
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"Handle9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2745_112_2_18_2_28_53_251,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return  FALSE;
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
            HandleSogouEvent(handle_ptr, WDK_C);
        }
        else
        {
            if (data_ptr->nPreEdit > 0)
            {
                HandleSogouEvent(handle_ptr, WDK_C);
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
        HandleSogouEvent(data_ptr, WDK_SPACE);
        data_ptr->bak_id = 0xff;
    }
    else
    {
        // bug 9512:
        // 在中文笔画输入法情况下，如果当前是选择状态，并且传入的是'1'到'6'的键值，则不处理
        if (WDI_CN_BIHUA == WD_GetImeResult()->imeType
            && WD_STATUS_CHINESE_SELECT == WD_GetImeResult()->imeStatus
            && strtab_ptr[1] >= 49
            && strtab_ptr[1] <= 55)
        {
            data_ptr->is_rect_change = TRUE;
            return TRUE;
        }
        //9键传得是cs_vk_1等
        HandleSogouEvent(data_ptr, strtab_ptr[1]);
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

    //SCI_TRACE_LOW:"HandleMethodSelect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2827_112_2_18_2_28_54_252,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        return  FALSE;
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
    GUIIM_CAPITAL_MODE_T cur_caps;
    GUIIM_METHOD_T cur_method;
    GUIIM_METHOD_T next_method;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"HandleCaps"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2858_112_2_18_2_28_54_253,(uint8*)"");

    //SCI_ASSERT(PNULL != handle_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);  /*assert verified*/
    if (PNULL == handle_ptr || PNULL == event_data_ptr ||PNULL == data_ptr->init_param.method_param_ptr)
    {
        return  FALSE;
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

    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

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
    GUIIM_METHOD_T cur_method;
    GUIIM_METHOD_T next_method;

    //SCI_TRACE_LOW:"Handle26KeyTo9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2905_112_2_18_2_28_54_254,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == data_ptr->init_param.method_param_ptr)
    {
        return  FALSE;
    }
    
    //检查当前是否已经是9键
    if (!data_ptr->is_full_keypad)
        return FALSE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);  /*assert verified*/

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

    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

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
    GUIIM_METHOD_T cur_method;
    GUIIM_METHOD_T next_method;

    //SCI_TRACE_LOW:"Handle9KeyTo26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_2963_112_2_18_2_28_54_255,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == data_ptr->init_param.method_param_ptr)
    {
        return  FALSE;
    }
    
    //检查当前是否已经是26键
    if (data_ptr->is_full_keypad)
        return FALSE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);  /*assert verified*/

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

    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

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
    //SCI_TRACE_LOW:"HandleLock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SOGOU_3011_112_2_18_2_28_54_256,(uint8*)"");
    //SCI_ASSERT(PNULL != handle_ptr);  /*assert verified*/
    if (PNULL == handle_ptr)
    {
        return  FALSE;
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

    //SCI_ASSERT(PNULL != data_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);  /*assert verified*/
    if (PNULL == data_ptr || PNULL == data_ptr->init_param.method_param_ptr)
    {
        return  FALSE;
    }
    
    // 如果切换符号窗口时有备选项, 则先清空备选项
    if (WD_GetImeResult()->selecting[0][0])
    {
        WD_OnKey(WDK_LONG_C, 0, MMI_DEFAULT_LASTDISPLAY_NUM, WD_GetImeResult());
        HandleSogouEvent(handle_ptr, 0);
    }

    MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle,
        MMIIM_GetSymbType(data_ptr->init_param.method_param_ptr->im_set.method));

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
    if (!handle_ptr->is_full_keypad
#if defined (IM_SIMP_CHINESE_SUPPORT)
            || MMIIM_TP_CSTAR_STATE_STROKE == handle_ptr->stateId
#endif
#if defined (IM_TRAD_CHINESE_SUPPORT)
            || MMIIM_TP_CSTAR_STATE_TRAD_STROKE == handle_ptr->stateId
#endif
       )
    {

        WD_SetVariable(1,tp_index + 1,WD_GetImeResult());
        HandleSogouEvent(handle_ptr, WDK_OK);
    }
    else //26键
    {
        WD_SetVariable(1,tp_index + 1,WD_GetImeResult());
        HandleSogouEvent(handle_ptr, WDK_OK);
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

    if (WD_GetImeResult()->selecting[0][0])
    {
        WD_OnKey(WDK_LONG_C, 0, MMI_DEFAULT_LASTDISPLAY_NUM, WD_GetImeResult());
        HandleSogouEvent(data_ptr, 0);
    }

    //alter the ui state
    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    /* 清空已输入的备选项与备选音 */
    SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM * (sizeof(wchar)));
    data_ptr->result_len = 0;
    SCI_MEMSET(data_ptr->pPreBuffer, 0, CAND_MAX_NUM * (sizeof(wchar)));
    data_ptr->nPreEdit = 0;

    MMIIMUI_ClearChoiceBarData(&data_ptr->choice_bar);

    //set the redraw signal
    data_ptr->is_rect_change = TRUE;
    data_ptr->is_bar_cand_dirty = TRUE;
    data_ptr->is_bar_choice_dirty = TRUE;
    data_ptr->is_bar_edit_dirty = TRUE;
    data_ptr->is_kb_dirty = TRUE;
}

/*******************************************************************************************/
//  Description : Get ime Result
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
PUBLIC WD_imeResult* WD_GetImeResult(void)
{
    return &g_imeResult;
}

/*******************************************************************************************/
//  Description : get last disp
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
PUBLIC uint8 MMIMLS9_GetLastDisp(void)
{
    return  s_lastdisp;
}

/*******************************************************************************************/
//  Description : set last disp
//  Global resource dependence :
//  Author:
//  Note:
/*********************************************************************************************/
PUBLIC void MMIMLS9_SetLastDisp(uint16 n)
{
    s_lastdisp = n;
}


/*****************************************************************************/
//  Description :将消息事件转化为输入法按键事件
//  Global resource dependence : none
//  Author:
//  Note:event 按键事件ID,event_in 事件的ID
/*****************************************************************************/
LOCAL uint16 ChangeEventToS9key(uint16 event,uint32 event_in)
{
    if (!event)
    {
        switch(event_in)
        {
        case KEY_UP:
            return WDK_UP;
        case KEY_DOWN:
            return WDK_DOWN;
        case KEY_LEFT:
            return WDK_LEFT;
        case KEY_RIGHT:
            return WDK_RIGHT;
        case KEY_GREEN:
            return 0;
        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
            return (event_in - KEY_1 + WDK_NORMAL_1);
        case KEY_STAR:
            return WDK_STAR;
        case KEY_0:
            return WDK_NORMAL_0;
        case KEY_HASH:
            return WDK_SHARP;
        case KEY_SPUP :
        case KEY_SPDW :
            return 0;
        case KEY_OK :
            return WDK_OK;
        case KEY_CANCEL:
            return WDK_C;
        case KEY_RED:
            return 0;
        case KEY_Q:
            return WDK_NORMAL_Q;
        case KEY_W:
            return WDK_NORMAL_W;
        case KEY_E:
            return WDK_NORMAL_E;
        case KEY_R:
            return WDK_NORMAL_R;
        case KEY_T:
            return WDK_NORMAL_T;
        case KEY_Y:
            return WDK_NORMAL_Y;
        case KEY_U:
            return WDK_NORMAL_U;
        case KEY_I:
            return WDK_NORMAL_I;
        case KEY_O:
            return WDK_NORMAL_O;
        case KEY_P:
            return WDK_NORMAL_P;
        case KEY_A:
            return WDK_NORMAL_A;
        case KEY_S:
            return WDK_NORMAL_S;
        case KEY_D:
            return WDK_NORMAL_D;
        case KEY_F:
            return WDK_NORMAL_F;
        case KEY_G:
            return WDK_NORMAL_G;
        case KEY_H:
            return WDK_NORMAL_H;
        case KEY_J:
            return WDK_NORMAL_J;
        case KEY_K:
            return WDK_NORMAL_K;
        case KEY_L:
            return WDK_NORMAL_L;
        case KEY_DEL:
            return WDK_C;
        case KEY_Z:
            return WDK_NORMAL_Z;
        case KEY_X:
            return WDK_NORMAL_X;
        case KEY_C:
            return WDK_NORMAL_C;
        case KEY_V:
            return WDK_NORMAL_V;
        case KEY_B:
            return WDK_NORMAL_B;
        case KEY_N  :
            return WDK_NORMAL_N ;
        case KEY_M:
            return WDK_NORMAL_M;
        case KEY_COMMA:
        case KEY_PERIOD:
        case KEY_ENTER:
        case KEY_FN :
            return 0;
        case KEY_SHIFT:
            return WDK_SHIFT;
        case KEY_AT_QWERTY:
            return WDK_STAR;
        case KEY_SPACE:
            return WDK_SPACE;
        case KEY_AND:
        case KEY_QUESTION:
            return 0;
        case KEY_CTRL:
            return WDK_CTRL;
        case KEY_PLUS  :
        case KEY_LEFT_PARENTHESIS :
        case KEY_RIGHT_PARENTHESIS:
        case KEY_MINUS       :
        case KEY_DOUBLE_QUOTES:
        case KEY_SEMICOLON :
        case KEY_COLON    :
        case KEY_SLASH   :
        case KEY_SHIFT_AND :
        case KEY_EXCLAMATION   :
            return 0;

        default:
            break;
        }
    }
    else
    {
        switch(event_in)
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            return  (event_in - '0' + WDK_NORMAL_0);
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6a:
        case 0x6b:
        case 0x6c:
        case 0x6d:
        case 0x6e:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7a:
            return  (event_in - 'a' + WDK_NORMAL_A);
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4b:
        case 0x4c:
        case 0x4d:
        case 0x4e:
        case 0x4f:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5a:
            return  (event_in - 'A' + WDK_NORMAL_A);

//6801 虚拟键盘部分代码，sogou公司合并代码时，使用到了cstar的数据，
//这个无法编译通过，先屏蔽            
#if 0   
        case CS_VK_SELECT:
            return WDK_OK;
        case CS_VK_BACKSPACE:
            return WDK_C;
        case CS_VK_UP:
            return WDK_UP;
        case CS_VK_DOWN:
            return WDK_DOWN;
        case CS_VK_LEFT:
            return WDK_LEFT;
        case CS_VK_RIGHT:
            return WDK_RIGHT;
        case CS_VK_SPACE:
            return WDK_SPACE;
        case CS_VK_BACKSPACE | CS_VK_LONGCLICK:
            return WDK_LONG_C;
#endif
        default:
            break;
        }
    }

    return 0;
}

/*****************************************************************************/
//  Description :输入法事件响应函数
//  Global resource dependence : none
//  Author:
//  Note:event 事件的ID， param  触笔的坐标
/*****************************************************************************/
PUBLIC uint8 HandleIMEOnKeyEvent(uint16 key)
{
    uint8 retValue = 0;

    //key = ChangeEventToS9key(event,msgid);//把事件转化成对应的KEY值

    if (key == WDK_SHARP && WD_GetImeResult()->imeType == WDI_CN_FULLPINYIN)
        {
            if(WD_GetImeResult()->selecting[0][0])
            {
                WD_OnKey(WDK_LONG_C,0,MMI_DEFAULT_LASTDISPLAY_NUM,WD_GetImeResult());
            }
            WD_OnKey(WDK_SHARP,0,MMIMLS9_GetLastDisp(),WD_GetImeResult());
        }

    if(key)
    {
        retValue = WD_OnKey(key,0,MMIMLS9_GetLastDisp(),WD_GetImeResult());//输入法接口函数]
    }

    return retValue;
}

#else

PUBLIC GUIIM_KERNEL_APIS_T const g_guiim_tp_sogou_apis = { 0 };

#endif //#ifdef MMIIM_USE_TP_SOGOU

#endif      //非PDA版本tp输入法
