/*****************************************************************************
** File Name:      mmiim_tp_sp.c                                             *
** Author:                                                                   *
** Date:           2010/08/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle pb menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/08/16     jian.ma            Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_im_trc.h"
#include "mmiim_base.h"

#if defined (TOUCH_PANEL_SUPPORT) && defined (IM_ENGINE_S9)

#include "Os_api.h"
#include "sci_types.h"

#include "mmiim_tp_sp.h"
#include "mmiim_tp_cstar_keyboard_cfg.h"
#include "mmiim_tp_cstar_position.h"
#include "mmiim_tp_cstar_internal.h"

#include "mmiim_internal.h"
#include "mmitheme_im.h"
#include "mmiim_im_switching.h"
#include "mmiim_sp_symbol.h"
#include "mmiim_tp_ui.h"
#include "mmiim_internal.h"
#include "guires.h"
#include "spim_ch_py26k.h"
#include "spim_ch_py9k.h"
#include "spim_ch_stroke.h"
#include "spim_english.h"
#include "spim_english9k.h"
#include "spim_ch_asso.h"
#include "mmk_timer.h"

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
//  Description : ��ʼ�����뷨
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void* Initialize(
    GUIIM_INIT_KERNEL_T* param_ptr
);

/*****************************************************************************/
//  Description : �������뷨
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Terminate(
    void* handle_ptr
);

/*****************************************************************************/
//  Description : ���뷨�Ĵ�����
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void* handle_ptr,
    GUIIM_EVENT_T event,
    GUIIM_EVENT_DATA_U* event_data_ptr
);

/******************************************************************************/
// Description: ����ϵͳ��Ϣ
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr
);

/*****************************************************************************/
//  Description : �ⲿ��Ϣת��Ϊ�ڲ���Ϣ����
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
//  Description : ��������״̬�µ�ϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : ������״̬�µ�ϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : ������״̬�µ�ϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgAssociate(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_T const *msg_ptr
);

/*****************************************************************************/
//  Description : TpDownProcessϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpDownProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpMoveProcessϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpMoveProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpUpNoInputProcessϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpUpNoInputProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpUpInputingProcessϵͳ��Ϣ����
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
//  Note: ����������ʱ��up����
/*****************************************************************************/
LOCAL BOOLEAN TpUpAssociateProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : TpLongProcessϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpLongProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr
);

/*****************************************************************************/
//  Description : ��cstar���ص��������õ�ǰUI״̬
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetUIStateByContent(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_IMM_CONTENT *content_ptr
);

/*****************************************************************************/
//  Description : ����cstar�¼�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSPEvent(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    uint32 hEvent,
    SPIM_EVENT_PARA_T *api_para
);

/*****************************************************************************/
//  Description : ���ʵ����ѡ�����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpCandPressUp(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr,
    GUI_POINT_T tp_point,
    GUIIM_EVENT_DATA_U* event_data_ptr
);

/*****************************************************************************/
//  Description : ��������choice bar��up����Ӧ����
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
//  Description : ģ���������ƴ����ҳ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: �ں�����ֻ��һ��һ�����������Լ���ģ�ⷭҳ
/*****************************************************************************/
LOCAL BOOLEAN HandleChoicePageSimulator(
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr,
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx
);

/*****************************************************************************/
//  Description : �������뷨
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����cstar���뷨���Ƿ�ȫ���̸�����������ȷ����,state���ڸ��µ�ǰ״̬
/*****************************************************************************/
LOCAL void SetMethod(
    MMIIM_TP_CSTAR_HANDLE_T* andle_ptr,
    GUIIM_METHOD_T method,
    MMIIM_TP_CSTAR_STATE_ID_T kb_state
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

/*****************************************************************************/
//  Description : init sp im kernal code
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL SPIM_HANDLE_T InitSpImKernalCode(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : terminatate sp im kernal code
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TerminateSpImKernalCode(GUIIM_METHOD_T method, MMIIM_TP_CSTAR_HANDLE_T* handle);

/*****************************************************************************/
//  Description : handle sp event
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL SPIM_RESULT_T SPIM_EventHandle(
    GUIIM_METHOD_T method,
    MMIIM_TP_CSTAR_HANDLE_T* handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    MMIIM_IMM_CONTENT * content_ptr
);


void Pringtinfo(wchar *buffer, uint32 len);
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//guiimʹ�õĺ�����
GUIIM_KERNEL_APIS_T const g_guiim_tp_sp_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

extern uint16 s_sym_del[2];
extern MMIIM_TP_CSTAR_STATE_T g_keyboard_state[MMIIM_TP_CSTAR_STATE_MAX - MMIIM_TP_CSTAR_KB_START_ID];

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ��ʼ�����뷨
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T* param_ptr)
{
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr = PNULL;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = 0;                     //Ӧ�ó������״̬

    SPIM_HANDLE_T * core_handle = PNULL;
    GUI_RECT_T rect_null = {0};

    //SCI_TRACE_LOW:"Initialize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_307_112_2_18_2_29_10_257,(uint8*)"");

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param_ptr->lcd_info_ptr); /*assert verified*/
    if (PNULL == param_ptr || PNULL == param_ptr->lcd_info_ptr)
    {
        return  PNULL;
    }
    
    //Ӧ�þ����������ʼ��������ʼ���Ĳ�����messetΪ0
    handle_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) SCI_ALLOC_APP(sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    if (PNULL == handle_ptr)
        return PNULL;

    SCI_MEMSET(handle_ptr, 0, sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    //��ʼ��
    handle_ptr->ctrl_handle = param_ptr->im_handle;
    handle_ptr->lcd_info_ptr = param_ptr->lcd_info_ptr;
    handle_ptr->init_param = *param_ptr;

    //����tips��
    MMIIMUI_CreateTipsLayer(handle_ptr, param_ptr);

    handle_ptr->stateId = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD - MMIIM_TP_CSTAR_KB_START_ID;           //��Ҫ��������ʼ��Ĭ��Ϊ��д
    handle_ptr->is_full_keypad = TRUE;
    handle_ptr->is_need_lock = FALSE;
    handle_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    //���ݵ�ǰ���뷨�˼�handle���Ƿ�ȫ���̿��ػ�ȡ��ǰ����״̬
    if (MMITHEME_IsMainScreenLandscape()) //�����Ļ�������ĳ�26��
    {
        handle_ptr->is_full_keypad = TRUE;
    }
    else if (GUIIM_INPUT_TYPE_TOUCHPEN == handle_ptr->init_param.method_param_ptr->im_set.input)
    {
        handle_ptr->is_full_keypad = FALSE;
    }

    kb_state = MMIIMUI_GetVKboardState(handle_ptr->init_param.method_param_ptr->im_set.method, handle_ptr->is_full_keypad);

    //����Ĭ�ϵ�״̬
    SetMethod(handle_ptr, handle_ptr->init_param.method_param_ptr->im_set.method, kb_state);
    //������
    MMIIMUI_InitDisplayData(handle_ptr);

    return (void*) handle_ptr;
}

/*****************************************************************************/
//  Description : �������뷨
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Terminate(void* handle_ptr)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;

    //SCI_TRACE_LOW:"Terminate"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_365_112_2_18_2_29_11_258,(uint8*)"");

    //SCI_ASSERT(data_ptr != PNULL); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return  TRUE;
    }
    
    TerminateSpImKernalCode(data_ptr->init_param.method_param_ptr->im_set.method, data_ptr);

    /*���������е����ģ�����Ժ����Ҳ֧��9���Ļ����ǾͲ��ش����������ڵ��߼����Ǻ���������Ҳ���޸�9��26״̬��������Ϊ26��*/
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
    handle_ptr = PNULL;

    return TRUE;
}

/*****************************************************************************/
//  Description : ���뷨�Ĵ�����
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void *handle_ptr,                   //in :���뷨���
    GUIIM_EVENT_T event,                //in :�����¼�
    GUIIM_EVENT_DATA_U *event_data_ptr  //in&out: ������������
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    BOOLEAN result = FALSE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD;

    data_ptr->event_data_ptr = event_data_ptr;
    //SCI_TRACE_LOW:"HandleMsg:event:%d,stateId:%d,stateId_bak:%d,stateId_bakii:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_414_112_2_18_2_29_11_259,(uint8*)"dddd", event, data_ptr->stateId, data_ptr->bak_stateId, data_ptr->bak_stateIdii);

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if (PNULL == data_ptr || data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
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
            //sleepʱ��Ӧ��Ҳ�ᱻguiim����
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
            MMIIMUI_GetRect(data_ptr, & (event_data_ptr->get_rect));
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

    case GUIIM_EVENT_GET_CTRL_RECT: //���մ�����Ӧ������
        {
            MMIIMUI_GetRect(data_ptr, & (event_data_ptr->get_ctrl_rect));
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
// Description: ����ϵͳ��Ϣ
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
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD;
    MMIIM_TP_CSTAR_MSG_T msg = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if (PNULL == data_ptr || data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return  FALSE;
    }
    
    if (MSG_LCD_SWITCH == event_data_ptr->sys_msg.msg_id
        && MMIIM_TP_CSTAR_STATE_DIGSYM == data_ptr->stateId + MMIIM_TP_CSTAR_KB_START_ID)
    {
        //�������л�ʱ��lock��Ϣ����
        handle_ptr->is_need_lock = FALSE;

        MMIIMUI_InitDisplayData(data_ptr);
        
        return TRUE;
    }

    //�����tipsʱ����ʱ����
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

    //���ⲿ��ϢתΪ�ڲ���Ϣ�������down,up,long,repeat��Ϣ
    TransMsg(data_ptr, event_data_ptr, &msg);

    //�Ǵ������ݣ����أ��޴���Ϣ���ͣ�ֱ��return
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
// Description: ���ò���
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
// LOCAL void SetOptionEx(void *handle)
// {
//     CS_IMM_OPTIONS option = {0};
//
//     if (0 == handle)
//     {
//         return;
//     }
//
//     option.dwPYFuzzy = (uint32)MMIIM_GetPyFuzzy();
//     option.dwStrokeLayout = CS_STROKE_STANDARD_LAYOUT;
//     option.dwAssoPinyin = CS_ASSOCIATION_ON;
//     option.dwAssoStroke = CS_ASSOCIATION_ON;
//     option.dwPinyinMode = CS_PINYIN_MODE_3;
//     option.dwCandidateMode = CS_CANDIDATE_MODE_1;
//
//     CS_IMM_SetOptionsEx(handle, &option);
//
//     return;
// }

/*****************************************************************************/
//  Description : �ⲿ��Ϣת��Ϊ�ڲ���Ϣ����
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
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/

    MMIIMUI_TransMsg(data_ptr, event_data_ptr, msg_ptr);
}

/*****************************************************************************/
//  Description : ��������״̬�µ�ϵͳ��Ϣ����
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL ==  msg_ptr)
    {
        return  FALSE;
    }
    
    //������Ϣ
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

    default:
        break;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : ������״̬�µ�ϵͳ��Ϣ����
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL ==  msg_ptr)
    {
        return  FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    //������Ϣ
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


        /*����������ʱ�����ܻ�����Ӧ����TP��Ϣ*/
    case MMIIM_TP_CSTAR_MSG_KEY_PRESS:
        {
            switch (msg_ptr->para.key)
            {
            case KEY_WEB:
            case KEY_OK:
                {
                    if (data_ptr->result_len > 0)
                    {
                        tp_point.x = data_ptr->cand_rect.left + 1;
                        tp_point.y = data_ptr->cand_rect.top + 1;
                        TpCandPressUp(data_ptr, tp_point, data_ptr->event_data_ptr);

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
                    HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_REMOVE, PNULL);

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
                    HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
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
//  Description : ������״̬�µ�ϵͳ��Ϣ����
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL ==  msg_ptr)
    {
        return  FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    //������Ϣ
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
                        tp_point.x = data_ptr->cand_rect.left + 1;
                        tp_point.y = data_ptr->cand_rect.top + 1;
                        TpCandPressUp(data_ptr, tp_point, data_ptr->event_data_ptr);

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
                    HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_REMOVE, PNULL);

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
                    HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
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
//  Description : TpDownProcessϵͳ��Ϣ����
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

    if (PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            if (para_ptr->idx < 0 || para_ptr->h_idx < 0 || para_ptr->v_idx < 0)
                break;

            //�������һ��TP�����µĲ���
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_kb_dirty = TRUE;

            //������
            MMIIMUI_ReDrawAllBoard(data_ptr);
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            //�������һ��TP�����µĲ���
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_bar_cand_dirty = TRUE;

            //������
            MMIIMUI_ReDrawAllBoard(data_ptr);
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            MMIIMUI_TpDownOnChoiceBar(&data_ptr->choice_bar, 
                para_ptr->idx, &data_ptr->is_bar_choice_dirty);
            
            //�������һ��TP�����µĲ���
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);
            
            if (data_ptr->is_bar_choice_dirty)
            {               
                //������
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
//  Description : TpMoveProcessϵͳ��Ϣ����
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
    if (PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            //��candbar�����ѡ��
            if (MMIIM_TP_CSTAR_MSG_TP_CAND == data_ptr->tp_last_msg_param.area)
            {
                //�������һ��TP�����µĲ���
                data_ptr->tp_last_msg_param.idx = -1;

                data_ptr->is_bar_cand_dirty = TRUE;
            }

            if (MMIIM_TP_CSTAR_MSG_TP_CHOICE == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_SetNormalStateOnChoiceBar(&data_ptr->choice_bar);
                
                data_ptr->is_bar_choice_dirty = TRUE;
            }

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

            //��Ȼ��ͬһ������
            if (para_ptr->idx == data_ptr->tp_last_msg_param.idx)
            {
                break;
            }

            //����tips
            MMIIMUI_HideKeyTipsEx(data_ptr);

            //�������һ��TP�����µĲ���
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
            //��ʾ����Ӽ��̴�������ѡ��
            if (MMIIM_TP_CSTAR_MSG_TP_KB == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_ClearPressKBMark(data_ptr);
                data_ptr->is_kb_dirty = TRUE;
            }

            if (MMIIM_TP_CSTAR_MSG_TP_CHOICE == data_ptr->tp_last_msg_param.area)
            {
                data_ptr->choice_bar.hot_index = -1;
                data_ptr->is_bar_choice_dirty = TRUE;
            }

            if (-1 == para_ptr->idx)
            {
                break;
            }

            //�������һ��TP�����µĲ���
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_bar_cand_dirty = TRUE;
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            //��ʾ����Ӽ��̴�������ѡ��
            if (MMIIM_TP_CSTAR_MSG_TP_KB == data_ptr->tp_last_msg_param.area)
            {
                data_ptr->is_kb_dirty = TRUE;
            }

            if (MMIIM_TP_CSTAR_MSG_TP_CAND == data_ptr->tp_last_msg_param.area)
            {
                //������up���������tips
                MMIIMUI_HideKeyTips(data_ptr);

                data_ptr->tp_last_msg_param.idx = -1;

                data_ptr->is_bar_cand_dirty = TRUE;
            }
            
            MMIIMUI_TpDownOnChoiceBar(&data_ptr->choice_bar, 
                para_ptr->idx, &data_ptr->is_bar_choice_dirty);

            //�������һ��TP�����µĲ���
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
//  Description : TpUpNoInputProcessϵͳ��Ϣ����
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr||PNULL == para_ptr)
    {
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

    //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
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
//  Description : TpUpInputingProcessϵͳ��Ϣ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����������ʱ��up����
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {            
            MMIIMUI_ClearPressKBMark(data_ptr);

            //̧��֮ǰδ���м���
            if (-1 == data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                break;
            }

            //����ͬһ������
            if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                break;
            }

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

            //TP����
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

        //�����ѡ��up
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            TpCandPressUp(data_ptr, para_ptr->point, data_ptr->event_data_ptr);

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
                break;
            }

            //�ȴ���һ������
            is_ret = HandleChoicePageSimulator(data_ptr, &data_ptr->choice_bar, para_ptr->idx);

            //�ٴ�����ʾ
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
//  Description : TpUpAssociateProcess
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����������ʱ��up����
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:

        MMIIMUI_ClearPressKBMark(data_ptr);

        //̧��֮ǰδ���м���
        if (-1 == data_ptr->tp_last_msg_param.idx)
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            break;
        }

        //����ͬһ������
        if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
        {
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            break;
        }

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

        //TP����
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

        //�����ѡ��up
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            TpCandPressUp(data_ptr, para_ptr->point, data_ptr->event_data_ptr);

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
//  Description : ������״̬�µ�ϵͳ��Ϣ����
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
    MMIIM_STATE_FUNC func = PNULL;
    int16 count = 0;
    uint32 id = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if (PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    bk_ui_state = data_ptr->ui_state;

    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:

        //̧��֮ǰδ���м���
        if (-1 == data_ptr->tp_last_msg_param.idx)
        {
            break;
        }

        //����ͬһ������
        if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
        {
            break;
        }

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

        /*TP�ϴ��ʳ������޷Ǿ���һ�����˼���������ǻ��˼��������κδ���ֱ�ӷ���true*/
        state = g_keyboard_state[data_ptr->stateId];
        count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);
        id = state.functab[count].id;

        if(id >= g_keyboard_state[data_ptr->stateId].char_cnt)
            break;

        if (!MMIIMUI_SearchFromStrtab(&s_sym_del[1], g_keyboard_state[data_ptr->stateId].char_tab[id]))
        {
            return TRUE;
        }

        //ȫɾ��
        if (g_keyboard_state[data_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
        {
            HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
        }

        notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

        if (bk_ui_state != data_ptr->ui_state)
        {
            data_ptr->is_rect_change = TRUE;
        }

        break;

        //�����ѡ������������
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        break;

    default:
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : ���ʵ����ѡ�������
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpCandPressUp(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, //in:���뷨���
    GUI_POINT_T tp_point,                  //in:�����λ��
    GUIIM_EVENT_DATA_U* event_data_ptr         //in&out:����ʹ����Ĳ���
)
{
    GUI_RECT_T font_rect = {0};
    uint16 str[2] = {0};
    uint16 iLoop = 0;
    uint16 jLoop = 0;
    uint16 pos = 0;
    int32 len = CAND_MAX_NUM;
    GUI_FONT_T font = {0};
    uint16 word_width = 0;
    uint16 label_width = 0;
    uint32 input_method = 0;
    GUI_RECT_T text_rect = {0};                         //�ı�������
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T state = GUISTR_STATE_WORDBREAK;
    MMI_STRING_T  string = {0};
    MMIIM_IMM_CONTENT content = {0};
    uint16 nMMIIM_TP_CSTAR_CAND_UP_LEFT_X = 0;
    uint16 nMMIIM_TP_CSTAR_CAND_IDC_SPACE_X = 0;    

    MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();

    //SCI_TRACE_LOW:"TpCandPressUp"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_1531_112_2_18_2_29_13_260,(uint8*)"");

    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != handle_ptr->lcd_info_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == handle_ptr||PNULL == handle_ptr->lcd_info_ptr||PNULL == event_data_ptr)
    {
        return FALSE;
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

    //������up���������tips
    MMIIMUI_HideKeyTips(handle_ptr);

    if (GUI_PointIsInRect(tp_point, text_rect))
    {
        if (handle_ptr->result_len > 0)
        {
            if (g_keyboard_state[handle_ptr->stateId].priv_data&MMIIM_TP_CSTAR_PY)
            {
                font = text_bar_theme->font_text;
                font_rect.top = handle_ptr->cand_rect.top;
                font_rect.bottom = handle_ptr->cand_rect.bottom;
                font_rect.left = handle_ptr->cand_rect.left;
                label_width = GUI_GetFontWidth(text_bar_theme->font_label, UNICODE_ASCII) + 2;
                iLoop = 0;

                while (iLoop++ < handle_ptr->result_len)
                {
                    jLoop = 0;

                    while (handle_ptr->result[pos+ (jLoop++)] != '\0')
                        NULL;

                    word_width = GUI_GetStringWidth(font, &handle_ptr->result[pos], jLoop);
                    font_rect.right = font_rect.left + word_width;

                    if (GUI_PointIsInRect(tp_point, font_rect))
                    {
                        SPIM_EVENT_PARA_T api_para = {0};
                        //���Ŵ�����һ������
                        handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
                        handle_ptr->tp_last_msg_param.idx = -1;

                        if (handle_ptr->ui_state != MMIIM_TP_CSTAR_STATE_ASSOCIATE)
                        {
                            api_para.select = iLoop - 1;
                            HandleSPEvent(handle_ptr, SPIM_EVENT_CAND_SELECT, &api_para);
                        }
                        else
                        {
                            api_para.select = pos;
                            HandleSPEvent(handle_ptr, SPIM_EVENT_COMMIT_INPUT, &api_para);
                        }

                        break;
                    }

                    pos += jLoop;
                    font_rect.left = (int16)(font_rect.right + label_width);
                }
                
                //�Ҳ���ֵ�����Ǳ߽������һ���Ƿ��б����£�������Ҫ���һ�±���
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
            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
            handle_ptr->tp_last_msg_param.idx = -1;
            HandleSPEvent(handle_ptr, SPIM_EVENT_PAGE_PREVIOUS, PNULL);
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
            handle_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
            handle_ptr->tp_last_msg_param.idx = -1;
            HandleSPEvent(handle_ptr, SPIM_EVENT_PAGE_NEXT, PNULL);
        }
    }
}

/*****************************************************************************/
//  Description : ��������choice bar��up����Ӧ����
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
    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/

    if (idx < 0||PNULL == bar_ptr)
    {
        return;
    }

    if (CAND_LEFT_ARROW_IDX == idx)
    {
        //���Ϸ�ҳ
        if (0 == bar_ptr->page_index)
        {
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
        //���·�ҳ
        if (bar_ptr->page_index >= bar_ptr->page_cnt - 1)
        {
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
//  Description : ģ���������ƴ����ҳ����
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: �ں�����ֻ��һ��һ�����������Լ���ģ�ⷭҳ
/*****************************************************************************/
LOCAL BOOLEAN HandleChoicePageSimulator(
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr,
    MMIIM_TP_CSTAR_CHOICE_BAR_T *bar_ptr,
    int32 idx
)
{
    MMIIM_IMM_CONTENT content = {0};
    int i = 0;
    int8 move_cnt = 0;          //�˲�����ʾҪ�ƶ��Ĵ�����������ʾ���ƣ�������ʾ����
    SPIM_EVENT_PARA_T para = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/

    if (idx < 0 || PNULL == data_ptr||PNULL == bar_ptr)
    {
        return FALSE;
    }

    if (CAND_LEFT_ARROW_IDX == idx)
    {
        //���Ϸ�ҳ
        if (0 == bar_ptr->page_index)
        {
            return FALSE;
        }

        if (!bar_ptr->is_choice_state)
        {
            move_cnt = bar_ptr->choice_line_cnt;
        }
        else
        {
            move_cnt = bar_ptr->choice_line_cnt + bar_ptr->choice_last_index;
        }

        para.select = bar_ptr->choice_line_cnt * bar_ptr->page_index + bar_ptr->choice_last_index - move_cnt;

        HandleSPEvent(data_ptr, SPIM_EVENT_PINYIN_SELECT, &para);

        //�ƶ���ǰ�����е�������������һҳ�ĵ�һ��
        bar_ptr->choice_last_index = 0;
        bar_ptr->is_choice_state = TRUE;
    }
    else if (CAND_RIGHT_ARROW_IDX == idx)
    {
        //���·�ҳ
        if (bar_ptr->page_index >= bar_ptr->page_cnt - 1)
        {
            return FALSE;
        }

        if (!bar_ptr->is_choice_state)
        {
            move_cnt = bar_ptr->choice_line_cnt;
        }
        else
        {
            move_cnt = bar_ptr->choice_line_cnt - bar_ptr->choice_last_index;
        }

        para.select = bar_ptr->choice_line_cnt * bar_ptr->page_index + bar_ptr->choice_last_index + move_cnt;
        HandleSPEvent(data_ptr, SPIM_EVENT_PINYIN_SELECT, &para);

        //�ƶ���ǰ�����е�������������һҳ�ĵ�һ��
        bar_ptr->choice_last_index = 0;
        bar_ptr->is_choice_state = TRUE;
    }
    else if (-1 < bar_ptr->hot_index && bar_ptr->hot_index < bar_ptr->cur_page_line_cnt)
    {
        if (!bar_ptr->is_choice_state)
        {
            //-1��ʾ���ν��룬��ʵ����״̬��һ��������һ��
            move_cnt = bar_ptr->choice_index;
        }
        else
        {
            move_cnt = bar_ptr->choice_index - bar_ptr->choice_last_index;

            //���ǵ�һ�Σ�˵�������ͬһ��λ��
            //if (0 == move_cnt)
            //{
            //  return TRUE;
            //}
        }

        para.select = bar_ptr->choice_line_cnt * bar_ptr->page_index + bar_ptr->choice_last_index + move_cnt;
        HandleSPEvent(data_ptr, SPIM_EVENT_PINYIN_SELECT, &para);

        bar_ptr->choice_last_index = bar_ptr->choice_index;
        bar_ptr->is_choice_state = TRUE;
    }
    else
    {
        return FALSE;
    }

    data_ptr->is_bar_cand_dirty = TRUE;
    data_ptr->is_bar_edit_dirty = TRUE;
    data_ptr->is_bar_choice_dirty = TRUE;
    return TRUE;
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

/*****************************************************************************/
//  Description : �������뷨
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����cstar���뷨���Ƿ�ȫ���̸�����������ȷ����,state���ڸ��µ�ǰ״̬
/*****************************************************************************/
LOCAL void SetMethod(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    GUIIM_METHOD_T method,
    MMIIM_TP_CSTAR_STATE_ID_T kb_state
)
{
    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/

    MMIIMUI_InitUIHandleData(handle_ptr, kb_state);

    handle_ptr->cstar_handle = InitSpImKernalCode(handle_ptr, method);

    return;
}

/*****************************************************************************/
//  Description : ��cstar���ص��������õ�ǰUI״̬
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetUIStateByContent(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_IMM_CONTENT* content_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != content_ptr); /*assert verified*/

    if (PNULL == data_ptr||PNULL == content_ptr)
    {
        return ;
    }
    
    data_ptr->cstar_window_state = content_ptr->flagState;

    /*������ɺ󣬻�Ҫ�������ݵ�result��*/
    SCI_MEMCPY(data_ptr->result, content_ptr->pCandBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->result_len = content_ptr->nCand;

    /*������ɺ󱣴�༭���ݵ�pPreBuffer��*/
    SCI_MEMCPY(data_ptr->pPreBuffer, content_ptr->pPreBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nPreEdit = content_ptr->nPreEdit;

    /*������ɺ󣬱���underline�����ݵ�pUnderLine��*/
    SCI_MEMCPY(data_ptr->pUnderLine, content_ptr->pUnderlineBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nUnderLine = content_ptr->nUnderline;

    if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr))
    {
        MMIIMUI_GetChoiceBufferByKernel(&data_ptr->choice_bar, content_ptr->pChoiceBuffer, content_ptr->nChoice);
    }

    /*���ݷ��ص���������鵱ǰ���뷨��UI��ʾ״̬
    �����commit���ݣ���һ��û��preedit���ݣ������Ƿ��к�ѡ��*/

    do
    {
        if (0 == content_ptr->nPreEdit)
        {
            if (0 < content_ptr->nCand)
            {
                //�ޱ༭���ݣ��к�ѡ�֣���Ϊ������״̬
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_ASSOCIATE;
            }
            else
            {
                //�ޱ༭���ݣ��޺�ѡ�֣���Ϊ������״̬
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
            }

            break;
        }
        else
        {
            if (0 < content_ptr->nCand)
            {
                //�б༭���ݣ��к�ѡ�֣���Ϊ�༭����״̬
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;
            }
            else
            {
                //�б༭���ݣ��޺�ѡ�֣���Ϊ������״̬
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;
            }

            break;
        }

    }
    while (0);

    return;
}

/*****************************************************************************/
//  Description : ����sp�¼�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSPEvent(MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, SPIM_EVENT_T hEvent, SPIM_EVENT_PARA_T *api_para)
{
    SPIM_RESULT_T result = TRUE;
    MMIIM_IMM_CONTENT content = {0};
    int32 len = CAND_MAX_NUM;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    size_t commit_len = 0;
    uint32 engine_type = 0;
    uint16 str[2] = {0};

    //SCI_TRACE_LOW:"HandleSPEvent"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_1957_112_2_18_2_29_14_261,(uint8*)"");
    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/
    if (PNULL == handle_ptr)
    {
        return TRUE;
    }
    
    if (SPIM_EVENT_COMMIT_INPUT == hEvent)
    {
        str[0] = handle_ptr->result[api_para->select];
        str[1] = '\0';
        content.pCommit[0] = str[0];
        //Pringtinfo(handle_ptr->result,handle_ptr->result_len);
        //Pringtinfo(content.pCommit,1);
    }
    else
    {
        if (handle_ptr->cstar_handle)
        {
            result = SPIM_EventHandle(handle_ptr->init_param.method_param_ptr->im_set.method, handle_ptr, hEvent, api_para, &content);
            SetUIStateByContent(handle_ptr, &content);
            handle_ptr->pageflag = content.flagPageCand;
        }
        else
        {
            if (api_para)
            {
                content.pCommit[0] = api_para->append;
            }
        }
    }

    if (content.pCommit[0])
    {
        int32 asso_cnt = MMIIM_SP_ASSO_CNT_MAX;
        wchar asso_chr[MMIIM_SP_ASSO_CNT_MAX];
        //SCI_ASSERT(PNULL != handle_ptr->event_data_ptr); /*assert verified*/
        notify_pack.data.commit_data.commit_len = MIN(sizeof(notify_pack.data.commit_data.commit), MMIAPICOM_Wstrlen(content.pCommit));
        SCI_MEMCPY(notify_pack.data.commit_data.commit, content.pCommit,
                   notify_pack.data.commit_data.commit_len * sizeof(content.pCommit[0]));

        if (content.nUnderline == 0)
        {
            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(handle_ptr->init_param.im_handle, &notify_pack);
        }

        if (GUIIM_M_TP_PINYIN == handle_ptr->init_param.method_param_ptr->im_set.method
                || GUIIM_M_TP_STROKE == handle_ptr->init_param.method_param_ptr->im_set.method
                || GUIIM_M_TPQ_PINYIN == handle_ptr->init_param.method_param_ptr->im_set.method
                || GUIIM_M_TPQ_STROKE == handle_ptr->init_param.method_param_ptr->im_set.method)
        {
            SPIM_CH_QueryAsso(content.pCommit[0], asso_chr, &asso_cnt);

            if (asso_cnt > 0)
            {
                int32 i = 0;
                uint32 j = 0;
                SCI_MEMSET(handle_ptr->result, 0, (sizeof(wchar) *CAND_MAX_NUM));

                for (i = 0 ; i <= asso_cnt; i++)
                {
                    if ('\0' != asso_chr[i])
                    {
                        SCI_MEMCPY(&handle_ptr->result[j++], &asso_chr[i], sizeof(wchar));
                        handle_ptr->result[j] = '\0';
                    }

                    //UI ��ʾ���ƣ���ͳһ���ٵ���
                    if (i >= 8)
                    {
                        break;
                    }
                }

                handle_ptr->result_len = i;
                handle_ptr->ui_state = MMIIM_TP_CSTAR_STATE_ASSOCIATE;
            }
        }
    }

    MMIIMUI_HideKeyTips(handle_ptr);
    //����ʾˢ��
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
    case MMIIM_TP_CSTAR_STATE_NOINPUT:
        {
            //ע��������Ҫ�������ϻ�
            MMIIMUI_DrawKeyBoard(handle_ptr);

            //���ظ���ͼ��
            MMIIMUI_DrawReplaceKey(handle_ptr);

            //�����9��������Ҫ����������
            /*[ע��]ԭ�������뷨����ʱ�������û����������°��������򣬴�����ϲ��󣬰���Щ��������÷��ڻ���ʱ
            ���輴��*/
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

    return TRUE;
}

/*****************************************************************************/
//  Description : ���ؼ�����
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
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = MMIIM_TP_CSTAR_KB_START_ID;

    //SCI_TRACE_LOW:"HandleReturn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2097_112_2_18_2_29_14_262,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    
    kb_state = MMIIM_TP_CSTAR_STATE_DIG - MMIIM_TP_CSTAR_KB_START_ID;

    MMIIMUI_InitUIHandleData(handle_ptr, kb_state);
    MMIIMUI_InitDisplayData(handle_ptr);
    MMIIMUI_DrawAllBoard(handle_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : �л�������
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2124_112_2_18_2_29_14_263,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return TRUE;
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
//  Description : �ַ�������
//  Global resource dependence : none
//  Author: rui.xu
//  Note: ����ֱ�����������ݣ�ע�����ü��̺�����ʱ��Ҫע��
/*****************************************************************************/
PUBLIC BOOLEAN HandleChar(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    uint16 const* strtab_ptr = PNULL;
    uint16 i = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"HandleChar"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2178_112_2_18_2_29_14_264,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return TRUE;
    }
    
    if (id >= g_keyboard_state[data_ptr->stateId].char_cnt)
    {
        return TRUE;
    }

    //ȡ����Ӧ�ļ���
    strtab_ptr = g_keyboard_state[data_ptr->stateId].char_tab[id];

    if (PNULL == strtab_ptr)
    {
        return TRUE;
    }

    //����ֱ���������ݡ�ֻ����һһ��Ӧ���ַ���������һ�������ܶ�Ӧ�����ַ�
    if (1 == strtab_ptr[0])
    {
        //��������ֻ������һ���ַ���һ���������ò���Ҫ���뷨�ں˵ļ���״̬������
        if (strtab_ptr[1] != s_sym_del[1] || (data_ptr->result_len <= 0))
        {
            notify_pack.data.commit_data.commit[0] = strtab_ptr[1];
            notify_pack.data.commit_data.commit_len = 1;
        }

        /*�����������жϣ������ǰ�����ݣ���ȫ�����*/
        if (MMIIM_TP_CSTAR_STATE_INPUTING == data_ptr->ui_state ||
                MMIIM_TP_CSTAR_STATE_ASSOCIATE == data_ptr->ui_state)
        {
            HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
        }

        //���Ӵ���������Ϊ��ȥ��underline��ֵ cr:191987
        if (0 < data_ptr->nUnderLine)
        {
            HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
        }

        //֪ͨ����
        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

        //�����������������Ҫ���ص���һ�μ���״̬�ģ������ӻص���������
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
//  Description : 26���̴���
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 9����26���������������뷨��������ģ���Ҫ����CS_EVENT_KEYBOARD,CS_EVENT_CHAR
//  ��������ͬ���¼������������Ϣֵ��ͬ����ģ������ڱ���ȫ�ֱ����԰�����Ӧ�Ĳ�������
//  �����¼�ʱ��Ҫ���ѡ��26: 'a', 'char'; 9: 'cs_vk_1','keyboard'
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

    //SCI_TRACE_LOW:"Handle26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2257_112_2_18_2_29_14_265,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/

    if (PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return TRUE;
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

    //�������ɾ������
    if (strtab_ptr == s_sym_del)
    {
        if (data_ptr->result_len > 0)
        {
            HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE, PNULL);
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
        SPIM_EVENT_PARA_T api_para = {0};
        api_para.append = strtab_ptr[1];
        result = HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_APPEND, &api_para);
        data_ptr->bak_id = 0xff;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 9���̴���
//  Global resource dependence : none
//  Author: rui.xu
//  Note: ˵��ͬ�ϣ�Ҫע����˼�
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
    //SCI_TRACE_LOW:"Handle9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2321_112_2_18_2_29_14_266,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return TRUE;
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
            HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE, PNULL);
        }
        else
        {
            if (data_ptr->nPreEdit > 0)
            {
                HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE, PNULL);
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
    else
    {
        SPIM_EVENT_PARA_T api_para = {0};
        api_para.append = strtab_ptr[1];
        //9��������cs_vk0��
        result = HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_APPEND, &api_para);
        data_ptr->bak_id = 0xff;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : ���뷨ѡ��
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2390_112_2_18_2_29_15_267,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        return TRUE;
    }
    
    MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
    MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);

    return TRUE;
}

/*****************************************************************************/
//  Description : ��Сд֮���л�
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:true:���͸ı�֪ͨ��false:����Ҫ�л���Сд
/*****************************************************************************/
PUBLIC BOOLEAN HandleCaps(
    void *handle_ptr,                     //in:���뷨���
    GUIIM_EVENT_DATA_U *event_data_ptr,   //in&out:���봫������
    uint32 id                              //in:�����id��
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_CAPITAL_MODE_T cur_caps = GUIIM_MODE_MAX;
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_TRACE_LOW:"HandleCaps"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2421_112_2_18_2_29_15_268,(uint8*)"");

    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/

    if (PNULL == handle_ptr || PNULL == event_data_ptr||PNULL == data_ptr->init_param.method_param_ptr)
    {
        return TRUE;
    }

    //ȡ��ǰ���뷨
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;
    cur_caps = data_ptr->init_param.method_param_ptr->im_set.capital;

    //���˲���Ҫ�л���Сд�����뷨����
    if (GUIIM_MODE_MAX <= cur_caps)
        return FALSE;

    //ȡ���ı����ԣ�ȴ�л���Сд�����뷨����
    next_method = MMIIMUI_GetNextCapsByMethod(cur_method, cur_caps);

    if (next_method == cur_method)
        return FALSE;

    //���л����뷨����ͨ�����뷨�ؼ�
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}

/*****************************************************************************/
//  Description : 26->9֮���л�
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle26KeyTo9Key(
    void *handle_ptr,                  //in:���뷨���
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id                           //in:�����id��
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;    
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;

    //SCI_TRACE_LOW:"Handle26KeyTo9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2469_112_2_18_2_29_15_269,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    //��鵱ǰ�Ƿ��Ѿ���9��
    if (PNULL == data_ptr|| !data_ptr->is_full_keypad ||PNULL == data_ptr->init_param.method_param_ptr )
        return FALSE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/

    //Ӧ����Ҫ�������ǰ�Ǻ����Ͳ����л���9��
    if (MMITHEME_IsMainScreenLandscape())
    {
        return FALSE;
    }

    //ȡ��ǰ���뷨
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;

    if (GUIIM_M_NONE == cur_method)
        return FALSE;    

    //ȡ��Ӧ26����tp���뷨
    next_method = MMITHEME_IMGetMethodByInput(cur_method, GUIIM_INPUT_TYPE_TOUCHPEN);

    if (next_method == cur_method || GUIIM_M_NONE == next_method)
        return FALSE;

    //��ǰȷʵ��ȫ���̣���Ҫ�л����޸ı�־������дnv
    data_ptr->is_full_keypad = FALSE;

    //���л����뷨����ͨ�����뷨�ؼ�
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 9->26֮���л�
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle9KeyTo26Key(
    void *handle_ptr, //in:���뷨���
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id  //in:�����id��
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;

    //SCI_TRACE_LOW:"Handle9KeyTo26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2523_112_2_18_2_29_15_270,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    //��鵱ǰ�Ƿ��Ѿ���26��
    if (PNULL == data_ptr || data_ptr->is_full_keypad||PNULL == data_ptr->init_param.method_param_ptr)
        return FALSE;
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/

    //ȡ��ǰ���뷨
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;

    if (GUIIM_M_NONE == cur_method)
        return FALSE;    

    //ȡ��Ӧ26����tp���뷨
    next_method = MMITHEME_IMGetMethodByInput(cur_method, GUIIM_INPUT_TYPE_TP_QWERT);

    if (next_method == cur_method || GUIIM_M_NONE == next_method)
        return FALSE;

    //��ǰȷʵ��ȫ���̣���Ҫ�л����޸ı�־������дnv
    data_ptr->is_full_keypad = TRUE;

    //���л����뷨����ͨ�����뷨�ؼ�
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : �����̴���
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2566_112_2_18_2_29_15_271,(uint8*)"");
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/

    if(PNULL == data_ptr || PNULL == event_data_ptr)
    {
        return TRUE;
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

    //��Щ�ط���������ۼ����������Ȼ��ϡ�ÿ�α�֤����ȷ��ͼ
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
//  Description : �л������Ŵ���
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
    if(PNULL == data_ptr || PNULL == event_data_ptr ||PNULL == data_ptr->init_param.method_param_ptr)
    {
        return TRUE;
    }
    
    MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle,
        MMIIM_GetSymbType(data_ptr->init_param.method_param_ptr->im_set.method));

    return TRUE;
}

/*****************************************************************************/
//  Description : init sp im kernal code
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL SPIM_HANDLE_T InitSpImKernalCode(
    MMIIM_TP_CSTAR_HANDLE_T *handle_ptr,
    GUIIM_METHOD_T method
)
{
    GUI_RECT_T win_rect = MMITHEME_GetClientRectEx(handle_ptr->init_param.win_handle);
    uint8 width_char = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, UNICODE_HANZI);
    uint8 width_label = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_LABEL, UNICODE_ASCII);
    uint8 width_spc = MMIIM_SP_TEXT_BAR_SPACE; //width_label + ;
    SPIM_INIT_PARAMETER_T init_params = {0};
    SPIM_HANDLE_T handle = PNULL;

    //kernel
    init_params.width_win = MMIIM_SP_TEXT_BAR_CAND_WIDTH;
    init_params.width_chr = width_char;
    init_params.width_spc = width_spc;

    if (PNULL != handle_ptr)
    {
        switch (method)
        {
        case GUIIM_M_TP_PINYIN:
            init_params.type = SPIM_TYPE_PINYIN_9KEY;
            handle = SPIM_CH_PY9K_Init(&init_params);
            break;

        case GUIIM_M_TPQ_PINYIN:
            init_params.type = SPIM_TYPE_PINYIN_26KEY;
            handle = SPIM_CH_PY26K_Init(&init_params);
            break;

        case GUIIM_M_TP_ZHUYIN:
            init_params.type = SPIM_TYPE_ZHUYIN_9KEY;
            handle = SPIM_CH_PY9K_Init(&init_params);
            break;

        case GUIIM_M_TPQ_ZHUYIN:
            init_params.type = SPIM_TYPE_ZHUYIN_26KEY;
            handle = SPIM_CH_PY9K_Init(&init_params);
            break;

        case GUIIM_M_TP_STROKE:
            handle = SPIM_CH_STROKE_Init(&init_params);
            break;
            
        case GUIIM_M_TPQ_STROKE:
            handle = SPIM_CH_STROKE_Init(&init_params);
            break;
            
        case GUIIM_M_TP_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TP_ENGLISH_LOWER:
        case GUIIM_M_TP_ENGLISH_UPPER:
        case GUIIM_M_TP_ENGLISH:
            init_params.width_chr = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, UNICODE_ASCII);
            handle = SPIM_SPELLS9K_InitEnglish(&init_params);
            break;
            
        case GUIIM_M_TPQ_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TPQ_ENGLISH_LOWER:
        case GUIIM_M_TPQ_ENGLISH_UPPER:
        case GUIIM_M_TPQ_ENGLISH:
            init_params.width_chr = GUI_GetFontWidth(MMIIM_SP_TEXT_BAR_FONT_TEXT, UNICODE_ASCII);
            handle = SPIM_SPELLS_InitEnglish(&init_params);
            break;
            
        default:
            break;
        }
    }

    return handle;
}


/*****************************************************************************/
//  Description : terminatate sp im kernal code
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TerminateSpImKernalCode(GUIIM_METHOD_T method, MMIIM_TP_CSTAR_HANDLE_T* handle)
{
    if (PNULL != handle)
    {
        switch (method)
        {
        case GUIIM_M_TP_PINYIN:
            SPIM_CH_PY9K_term(handle->cstar_handle);
            break;

        case GUIIM_M_TPQ_PINYIN:
            SPIM_CH_PY26K_term(handle->cstar_handle);
            break;

        case GUIIM_M_TP_ZHUYIN:
            SPIM_CH_PY9K_term(handle->cstar_handle);
            break;

        case GUIIM_M_TPQ_ZHUYIN:
            SPIM_CH_PY9K_term(handle->cstar_handle);
            break;

        case GUIIM_M_TP_STROKE:
            SPIM_CH_STROKE_term(handle->cstar_handle);
            break;

        case GUIIM_M_TPQ_STROKE:
            SPIM_CH_STROKE_term(handle->cstar_handle);
            break;

        case GUIIM_M_TP_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TP_ENGLISH_LOWER:
        case GUIIM_M_TP_ENGLISH_UPPER:
        case GUIIM_M_TP_ENGLISH:
            SPIM_SPELLS9K_Term(handle->cstar_handle);
            break;

        case GUIIM_M_TPQ_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TPQ_ENGLISH_LOWER:
        case GUIIM_M_TPQ_ENGLISH_UPPER:
        case GUIIM_M_TPQ_ENGLISH:
            SPIM_SPELLS_Term(handle->cstar_handle);
            break;

        default:
            break;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle sp event
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL SPIM_RESULT_T SPIM_EventHandle(
    GUIIM_METHOD_T method,
    MMIIM_TP_CSTAR_HANDLE_T* handle,
    SPIM_EVENT_T event,
    SPIM_EVENT_PARA_T const * para_ptr,
    MMIIM_IMM_CONTENT * content_ptr
)
{
    SPIM_RESULT_T result = 0;
    uint32 i = 0;
    uint32 j = 0;

    switch (method)
    {
    case GUIIM_M_TP_PINYIN:
    case GUIIM_M_TP_ZHUYIN:
        {
            SPIM_CONTENT_T api_content = {0};
            result = SPIM_CH_PY9K_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            //copy spim data to im data
            SCI_MEMCPY(content_ptr->pCommit, api_content.commit, sizeof(content_ptr->pCommit));
            SCI_MEMCPY(content_ptr->pPreBuffer, api_content.pre_edit, sizeof(content_ptr->pPreBuffer));

            for (i = 0; i < ARR_SIZE(api_content.pre_edit); i++)
            {
                if ('\0' != api_content.pre_edit[i])
                {
                    content_ptr->nPreEdit = 1;
                    break;
                }
            }

            SCI_MEMCPY(content_ptr->pCandBuffer, api_content.candidate, sizeof(content_ptr->pCandBuffer));
            content_ptr->nCand = api_content.cand_cnt;

            SCI_MEMCPY(content_ptr->pChoiceBuffer, api_content.py_choice, sizeof(content_ptr->pChoiceBuffer));
            content_ptr->nChoice = api_content.py_cnt;

            content_ptr->flagPageCand = api_content.page_flag;
        }
        break;
        
    case GUIIM_M_TPQ_PINYIN:
    case GUIIM_M_TPQ_ZHUYIN:
        {
            SPIM_CONTENT_T api_content = {0};
            if (GUIIM_M_TPQ_PINYIN == method)
            {
                result = SPIM_CH_PY26K_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            }
            else
            {
                result = SPIM_CH_PY9K_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            }
            //copy spim data to im data
            SCI_MEMCPY(content_ptr->pCommit, api_content.commit, sizeof(content_ptr->pCommit));
            SCI_MEMCPY(content_ptr->pPreBuffer, api_content.pre_edit, sizeof(content_ptr->pPreBuffer));

            for (i = 0; i < ARR_SIZE(api_content.pre_edit); i++)
            {
                if ('\0' != api_content.pre_edit[i])
                {
                    content_ptr->nPreEdit = 1;
                    break;
                }
            }

            SCI_MEMCPY(content_ptr->pCandBuffer, api_content.candidate, MIN(sizeof(api_content.candidate), sizeof(content_ptr->pCandBuffer)));
            content_ptr->nCand = api_content.cand_cnt;
            content_ptr->flagPageCand = api_content.page_flag;
        }
        break;

    case GUIIM_M_TP_STROKE:
        {
            SPIM_CONTENT_T api_content = {0};
            result = SPIM_CH_STROKE_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            //copy spim data to im data
            content_ptr->pCommit[0] = api_content.commit[0];
            SCI_MEMCPY(content_ptr->pPreBuffer, api_content.pre_edit, sizeof(content_ptr->pPreBuffer));

            for (i = 0; i < ARR_SIZE(api_content.pre_edit); i++)
            {
                if ('\0' != api_content.pre_edit[i])
                {
                    content_ptr->nPreEdit = 1;
                    break;
                }
            }

            for (i = 0, j = 0 ; i < ARR_SIZE(api_content.candidate); i++)
            {
                if ('\0' != api_content.candidate[i])
                {
                    SCI_MEMCPY(&content_ptr->pCandBuffer[j++], &api_content.candidate[i], sizeof(SPIM_UNICODE_T));
                    content_ptr->pCandBuffer[j] = '\0';
                }
            }

            content_ptr->nCand = api_content.cand_cnt;
            content_ptr->flagPageCand = api_content.page_flag;
        }
        break;
        
    case GUIIM_M_TPQ_STROKE:
        {
            SPIM_CONTENT_T api_content = {0};
            result = SPIM_CH_STROKE_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            //copy spim data to im data
            content_ptr->pCommit[0] = api_content.commit[0];
            SCI_MEMCPY(content_ptr->pPreBuffer, api_content.pre_edit, sizeof(content_ptr->pPreBuffer));

            for (i = 0; i < ARR_SIZE(api_content.pre_edit); i++)
            {
                if ('\0' != api_content.pre_edit[i])
                {
                    content_ptr->nPreEdit = 1;
                    break;
                }
            }

            for (i = 0, j = 0 ; i < ARR_SIZE(api_content.candidate); i++)
            {
                if ('\0' != api_content.candidate[i])
                {
                    SCI_MEMCPY(&content_ptr->pCandBuffer[j++], &api_content.candidate[i], sizeof(SPIM_UNICODE_T));
                    content_ptr->pCandBuffer[j] = '\0';
                }
            }

            content_ptr->nCand = api_content.cand_cnt;
            content_ptr->flagPageCand = api_content.page_flag;
        }
        break;
        
    case GUIIM_M_TP_ENGLISH_LEAD_UPPER:
    case GUIIM_M_TP_ENGLISH_LOWER:
    case GUIIM_M_TP_ENGLISH_UPPER:
    case GUIIM_M_TP_ENGLISH:
        {
            SPIM_CONTENT_T api_content = {0};
            result = SPIM_SPELLS9K_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            //copy spim data to im data
            SCI_MEMCPY(content_ptr->pCommit, api_content.commit, sizeof(content_ptr->pCommit));
            SCI_MEMCPY(content_ptr->pPreBuffer, api_content.pre_edit, sizeof(content_ptr->pPreBuffer));
            content_ptr->nPreEdit = api_content.pre_edit_cnt;
            SCI_MEMCPY(content_ptr->pCandBuffer, api_content.candidate, sizeof(content_ptr->pCandBuffer));
            content_ptr->nCand = api_content.cand_cnt;
            content_ptr->flagPageCand = api_content.page_flag;
        }
        break;
        
    case GUIIM_M_TPQ_ENGLISH_LEAD_UPPER:
    case GUIIM_M_TPQ_ENGLISH_LOWER:
    case GUIIM_M_TPQ_ENGLISH_UPPER:
    case GUIIM_M_TPQ_ENGLISH:
        {
            SPIM_CONTENT_T api_content = {0};
            result = SPIM_SPELLS_Handle(handle->cstar_handle, event, para_ptr, &api_content);
            //copy spim data to im data
            SCI_MEMCPY(content_ptr->pCommit, api_content.commit, sizeof(content_ptr->pCommit));
            SCI_MEMCPY(content_ptr->pPreBuffer, api_content.pre_edit, sizeof(content_ptr->pPreBuffer));
            content_ptr->nPreEdit = api_content.pre_edit_cnt;
            SCI_MEMCPY(content_ptr->pCandBuffer, api_content.candidate, sizeof(content_ptr->pCandBuffer));
            content_ptr->nCand = api_content.cand_cnt;
            content_ptr->flagPageCand = api_content.page_flag;
        }
        break;

    default:
        break;
    }

    return result;
}

void Pringtinfo(wchar *buffer, uint32 len)
{
    uint32 i = 0;

    for (i = 0 ; i < len; i++)
    {
        //SCI_TRACE_LOW:"buffer[%d]= 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2949_112_2_18_2_29_16_272,(uint8*)"dd", i, buffer[i]);
    }
}


#else

GUIIM_KERNEL_APIS_T const g_guiim_tp_sp_apis = {0};

#endif //#if defined (TOUCH_PANEL_SUPPORT) && defined (IM_ENGINE_S9)

