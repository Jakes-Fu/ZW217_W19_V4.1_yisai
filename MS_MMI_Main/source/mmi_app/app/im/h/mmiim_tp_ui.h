/*****************************************************************************
** File Name:      mmiim_tp_ui.h                                             *
** Author:                                                                   *
** Date:           2010/08/14                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle pb menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/08/14    jian.ma           Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifndef _MMIIM_TP_UI_H_
#define _MMIIM_TP_UI_H_

#include "sci_types.h"
#include "guiim_base.h"
#include "mmiim_tp_cstar_keyboard_cfg.h"
#include "mmiim_tp_cstar_internal.h"



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIIM_UI_TIPS_TIMER_OUT   400


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//输入法与键盘的对应表
typedef struct
{
    GUIIM_METHOD_T gui_method;                  //输入法ID
    MMIIM_TP_CSTAR_STATE_ID_T kb_state;     //对应的键盘绝对值
}MMIIM_METHOD_KEYBOARD_RELATION_T ;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化显示数据
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_InitDisplayData(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : 重置UI句柄相关变量
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_InitUIHandleData(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    MMIIM_TP_CSTAR_STATE_ID_T kb_state
);

/*****************************************************************************/
//  Description : 初始化9键的pychoicebar数据
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 一些不受内核返回数据的数据都要在此一次初始化完成
/*****************************************************************************/
PUBLIC void MMIIMUI_InitChoiceBarData(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : 获得图片的宽高
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_GetImageWidthHeight(
    MMI_WIN_ID_T win_id,
    MMI_IMAGE_ID_T image_id,
    uint16* width_ptr,
    uint16* height_ptr
);

/*****************************************************************************/
//  Description : 画整张键盘，注意根据UI状态来写，可能只显示部分
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawAllBoard(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr //in:输入法句柄
);

/*****************************************************************************/
//  Description : 画键盘
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 要根据当前键盘状态刷图，注意大小写要画对
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawKeyBoard(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr //in:输入法句柄
);

/*****************************************************************************/
//  Description : 画键盘上方需要替换的小图标
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawReplaceKey(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr //in:输入法句柄
);

/*****************************************************************************/
//  Description : 画编辑框
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawEditBar(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : 设置choice bar的正常状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_SetNormalStateOnChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr
);

/*****************************************************************************/
//  Description : 当触笔点击在choice bar上的时候显示焦点等
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_TpDownOnChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx,
    BOOLEAN *dirty_flg_ptr
);

/*****************************************************************************/
//  Description : 触笔down点击候选筐操作，只是准备数据，焦点位置等
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_SetTpOnCandBarIdx(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUI_POINT_T tp_point,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T *para_ptr
);

/*****************************************************************************/
//  Description : 画候选框
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawCandBar(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : 重置混淆区相关变量
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ClearChoiceBarData(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *choice_bar_ptr
);

/*****************************************************************************/
//  Description : 有按键按下，或者移动弹起时重绘键盘
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ReDrawAllBoard(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr
);

/*****************************************************************************/
//  Description : 输入法区域发生变成
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_ChangeRect(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr
);

/*****************************************************************************/
//  Description : get text bar theme info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* MMIIMUI_GetTextBarThemeIfo(void);

/*****************************************************************************/
//  Description : 画混淆区
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int MMIIMUI_DrawChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *choice_bar_ptr
);

/******************************************************************************/
// Description: 无条件隐藏按键的tips
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
PUBLIC void MMIIMUI_HideKeyTips(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/******************************************************************************/
// Description: 定时等待并隐藏按键的tips
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
PUBLIC void MMIIMUI_HideKeyTipsEx(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/******************************************************************************/
// Description: 定时器隐藏回调函数
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void HideTipsTimerCB(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : 得到输入法UI的大小
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_GetRect(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUI_RECT_T* rect
);

/*****************************************************************************/
//  Description : 是否是普通键盘状态
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 键盘状态可以理解成9键，26键，无所谓9键26键三种状态这里就称为普通状态
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_IsCommonKeyBoardState(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : 取得输入法的最小按键的宽度
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIMUI_GetKeyButtonWidth(
    MMIIM_TP_CSTAR_STATE_T state
);

/*****************************************************************************/
//  Description : 取得输入法的最小按键的高度
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIMUI_GetKeyButtonHeight(
    MMIIM_TP_CSTAR_STATE_T state
);

/*****************************************************************************/
//  Description : 把引擎给的内容一次复制到应用上来
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_GetChoiceBufferByKernel(
    MMIIM_TP_CSTAR_CHOICE_BAR_T* choice_bar_ptr,
    wchar* buf,
    uint16 num
);

/*****************************************************************************/
//  Description : 得到焦点区的区域，相对图片的左上角
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIIMUI_GetFocusRect(
    uint16 rect_num,
    uint16 button_width,
    uint16 button_height,
    uint16 hor_index,
    uint16 ver_index
);

/*****************************************************************************/
//  Description : 检查按键点是否在图标之上
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:如果点刚好在间隔区内则返回false, 在上面返回偏移;
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_CheckPointAvailability(
    MMIIM_TP_CSTAR_STATE_T state,
    GUI_POINT_T point,
    uint16 button_width,
    uint16 button_height,
    uint16* hor_index_ptr,
    uint16* ver_index_ptr
);

/*****************************************************************************/
//  Description : create tips latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_CreateTipsLayer(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    GUIIM_INIT_KERNEL_T *param_ptr
);

/******************************************************************************/
// Description: 由当前的输入法及是否全键盘得到键盘的状态
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_CSTAR_STATE_ID_T MMIIMUI_GetVKboardState(
    GUIIM_METHOD_T method,
    BOOLEAN is_full_keypad
);

/*****************************************************************************/
//  Description : 根据当前的输入法获取下一个同类语言的大小写输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 如果语言区分大小写，则在三种之间来回切换
/*****************************************************************************/
PUBLIC GUIIM_METHOD_T MMIIMUI_GetNextCapsByMethod(
    GUIIM_METHOD_T cur_method,
	GUIIM_CAPITAL_MODE_T cur_caps
);

/*****************************************************************************/
//  Description : 外部消息转化为内部消息类型
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_TransMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_TP_CSTAR_MSG_T *msg_ptr
);

/*****************************************************************************/
//  Description : TpDownProcess系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_SaveLastTpMsgParam(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : 清除按键痕迹
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ClearPressKBMark(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : 清除候选栏按键痕迹
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ClearPressCandMark(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : 查找字符
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_SearchFromStrtab(
    const wchar *char_ptr,
    const wchar *strtab_ptr
);

/*****************************************************************************/
//  Description : MMIIMUI_ClearKbUpOnInValidRect
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_ClearKbUpOnInValidRect(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr
);

/*****************************************************************************/
//  Description : 重新设置键盘图片
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 由于multitap首字符大写造成的, multitap，lead要求配大写后变小写
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_ResetKbImageByLeadCape(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : 根据输入法获得对齐方式(从左往右还是从右往左)
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIIM_TPGetAligntype(GUIIM_METHOD_T method);

PUBLIC const MMIIM_TP_KEY_DISPLAY_TEXT** MMIIMUI_MulKb26KeyGetDefData(void);
PUBLIC const MMIIM_TP_KEY_DISPLAY_TEXT** MMIIMUI_MulKb9KeyGetDefData(void);
PUBLIC int32 MMIIMUI_ClearKB26KeyTextDefData(const MMIIM_TP_KEY_DISPLAY_TEXT **p_kb_key);
PUBLIC int32 MMIIMUI_SetKbMul26KeyTextData(GUIIM_METHOD_T method, const MMIIM_TP_KEY_DISPLAY_TEXT **p_kb_key);
PUBLIC int32 MMIIMUI_SetSmartKb26KeyTextData(GUIIM_METHOD_T method, const MMIIM_TP_KEY_DISPLAY_TEXT **p_kb_key);
PUBLIC int32 MMIIMUI_ClearKB9KeyTextDefData(const MMIIM_TP_KEY_DISPLAY_TEXT **p_kb_key);
PUBLIC int32 MMIIMUI_SetKbMul9KeyTextData(GUIIM_METHOD_T method, const MMIIM_TP_KEY_DISPLAY_TEXT **p_kb_key);
PUBLIC int32 MMIIMUI_SetSmartKb9KeyTextData(GUIIM_METHOD_T method, const MMIIM_TP_KEY_DISPLAY_TEXT **p_kb_key);
PUBLIC int32 MMIIMUI_DrawKeyBoardWithText(MMIIM_TP_CSTAR_HANDLE_T* handle_ptr);
PUBLIC int32 MMIIMUI_DrawVituralButtonWithText(MMIIM_TP_CSTAR_HANDLE_T* data_ptr,BOOLEAN highlight);



#endif


