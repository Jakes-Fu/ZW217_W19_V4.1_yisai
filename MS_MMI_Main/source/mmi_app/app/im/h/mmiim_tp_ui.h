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
//���뷨����̵Ķ�Ӧ��
typedef struct
{
    GUIIM_METHOD_T gui_method;                  //���뷨ID
    MMIIM_TP_CSTAR_STATE_ID_T kb_state;     //��Ӧ�ļ��̾���ֵ
}MMIIM_METHOD_KEYBOARD_RELATION_T ;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ʼ����ʾ����
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_InitDisplayData(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : ����UI�����ر���
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_InitUIHandleData(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    MMIIM_TP_CSTAR_STATE_ID_T kb_state
);

/*****************************************************************************/
//  Description : ��ʼ��9����pychoicebar����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: һЩ�����ں˷������ݵ����ݶ�Ҫ�ڴ�һ�γ�ʼ�����
/*****************************************************************************/
PUBLIC void MMIIMUI_InitChoiceBarData(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : ���ͼƬ�Ŀ��
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
//  Description : �����ż��̣�ע�����UI״̬��д������ֻ��ʾ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawAllBoard(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr //in:���뷨���
);

/*****************************************************************************/
//  Description : ������
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: Ҫ���ݵ�ǰ����״̬ˢͼ��ע���СдҪ����
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawKeyBoard(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr //in:���뷨���
);

/*****************************************************************************/
//  Description : �������Ϸ���Ҫ�滻��Сͼ��
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawReplaceKey(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr //in:���뷨���
);

/*****************************************************************************/
//  Description : ���༭��
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawEditBar(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : ����choice bar������״̬
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_SetNormalStateOnChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr
);

/*****************************************************************************/
//  Description : �����ʵ����choice bar�ϵ�ʱ����ʾ�����
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
//  Description : ����down�����ѡ�������ֻ��׼�����ݣ�����λ�õ�
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
//  Description : ����ѡ��
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_DrawCandBar(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : ���û�������ر���
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ClearChoiceBarData(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *choice_bar_ptr
);

/*****************************************************************************/
//  Description : �а������£������ƶ�����ʱ�ػ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ReDrawAllBoard(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr
);

/*****************************************************************************/
//  Description : ���뷨���������
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
//  Description : ��������
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC int MMIIMUI_DrawChoiceBar(
    MMIIM_TP_CSTAR_CHOICE_BAR_T *choice_bar_ptr
);

/******************************************************************************/
// Description: ���������ذ�����tips
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
PUBLIC void MMIIMUI_HideKeyTips(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/******************************************************************************/
// Description: ��ʱ�ȴ������ذ�����tips
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
PUBLIC void MMIIMUI_HideKeyTipsEx(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/******************************************************************************/
// Description: ��ʱ�����ػص�����
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void HideTipsTimerCB(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : �õ����뷨UI�Ĵ�С
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_GetRect(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUI_RECT_T* rect
);

/*****************************************************************************/
//  Description : �Ƿ�����ͨ����״̬
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����״̬��������9����26��������ν9��26������״̬����ͳ�Ϊ��ͨ״̬
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_IsCommonKeyBoardState(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : ȡ�����뷨����С�����Ŀ��
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIMUI_GetKeyButtonWidth(
    MMIIM_TP_CSTAR_STATE_T state
);

/*****************************************************************************/
//  Description : ȡ�����뷨����С�����ĸ߶�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIMUI_GetKeyButtonHeight(
    MMIIM_TP_CSTAR_STATE_T state
);

/*****************************************************************************/
//  Description : �������������һ�θ��Ƶ�Ӧ������
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
//  Description : �õ����������������ͼƬ�����Ͻ�
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
//  Description : ��鰴�����Ƿ���ͼ��֮��
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:�����պ��ڼ�������򷵻�false, �����淵��ƫ��;
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
// Description: �ɵ�ǰ�����뷨���Ƿ�ȫ���̵õ����̵�״̬
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_CSTAR_STATE_ID_T MMIIMUI_GetVKboardState(
    GUIIM_METHOD_T method,
    BOOLEAN is_full_keypad
);

/*****************************************************************************/
//  Description : ���ݵ�ǰ�����뷨��ȡ��һ��ͬ�����ԵĴ�Сд���뷨
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����������ִ�Сд����������֮�������л�
/*****************************************************************************/
PUBLIC GUIIM_METHOD_T MMIIMUI_GetNextCapsByMethod(
    GUIIM_METHOD_T cur_method,
	GUIIM_CAPITAL_MODE_T cur_caps
);

/*****************************************************************************/
//  Description : �ⲿ��Ϣת��Ϊ�ڲ���Ϣ����
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
//  Description : TpDownProcessϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_SaveLastTpMsgParam(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : ��������ۼ�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ClearPressKBMark(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : �����ѡ�������ۼ�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIMUI_ClearPressCandMark(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr
);

/*****************************************************************************/
//  Description : �����ַ�
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
//  Description : �������ü���ͼƬ
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����multitap���ַ���д��ɵ�, multitap��leadҪ�����д���Сд
/*****************************************************************************/
PUBLIC BOOLEAN MMIIMUI_ResetKbImageByLeadCape(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr
);

/*****************************************************************************/
//  Description : �������뷨��ö��뷽ʽ(�������һ��Ǵ�������)
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


