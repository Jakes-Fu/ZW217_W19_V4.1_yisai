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
//  Author: rui.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void* handle_ptr,
    GUIIM_EVENT_T event,
    GUIIM_EVENT_DATA_U* event_data_ptr
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
LOCAL void SetUIStateByContent(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    MMIIM_IMM_CONTENT *content_ptr
);

/*****************************************************************************/
//  Description : 处理cstar事件
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
//  Description : 触笔点击候选筐操作
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
//guiim使用的函数表
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
//  Description : 初始化输入法
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T* param_ptr)
{
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr = PNULL;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = 0;                     //应用程序键盘状态

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
    
    //应用句柄各参数初始化，不初始化的参数即messet为0
    handle_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) SCI_ALLOC_APP(sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    if (PNULL == handle_ptr)
        return PNULL;

    SCI_MEMSET(handle_ptr, 0, sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    //初始化
    handle_ptr->ctrl_handle = param_ptr->im_handle;
    handle_ptr->lcd_info_ptr = param_ptr->lcd_info_ptr;
    handle_ptr->init_param = *param_ptr;

    //创建tips层
    MMIIMUI_CreateTipsLayer(handle_ptr, param_ptr);

    handle_ptr->stateId = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD - MMIIM_TP_CSTAR_KB_START_ID;           //重要参数，初始化默认为手写
    handle_ptr->is_full_keypad = TRUE;
    handle_ptr->is_need_lock = FALSE;
    handle_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

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

    //SCI_TRACE_LOW:"Terminate"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_365_112_2_18_2_29_11_258,(uint8*)"");

    //SCI_ASSERT(data_ptr != PNULL); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return  TRUE;
    }
    
    TerminateSpImKernalCode(data_ptr->init_param.method_param_ptr->im_set.method, data_ptr);

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
    handle_ptr = PNULL;

    return TRUE;
}

/*****************************************************************************/
//  Description : 输入法的处理函数
//  Global resource dependence : none
//  Author: rui.xu
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

    case GUIIM_EVENT_GET_CTRL_RECT: //接收触笔响应的区域
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
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/

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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL ==  msg_ptr)
    {
        return  FALSE;
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL ==  msg_ptr)
    {
        return  FALSE;
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL ==  msg_ptr)
    {
        return  FALSE;
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
    if (PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            //从candbar进入候选区
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

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

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
                data_ptr->choice_bar.hot_index = -1;
                data_ptr->is_bar_choice_dirty = TRUE;
            }

            if (-1 == para_ptr->idx)
            {
                break;
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
                MMIIMUI_HideKeyTips(data_ptr);

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

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

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

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

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

        //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/

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
//  Note:
/*****************************************************************************/
LOCAL void TpCandPressUp(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, //in:输入法句柄
    GUI_POINT_T tp_point,                  //in:点击的位置
    GUIIM_EVENT_DATA_U* event_data_ptr         //in&out:传入和传出的参数
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
    GUI_RECT_T text_rect = {0};                         //文本区区域
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

    //文字区up后，立马清掉tips
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
                        //接着处理下一级联想
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
    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/

    if (idx < 0||PNULL == bar_ptr)
    {
        return;
    }

    if (CAND_LEFT_ARROW_IDX == idx)
    {
        //向上翻页
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
        //向下翻页
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
    MMIIM_IMM_CONTENT content = {0};
    int i = 0;
    int8 move_cnt = 0;          //此参数表示要移动的次数，负数表示左移，正数表示右移
    SPIM_EVENT_PARA_T para = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != bar_ptr); /*assert verified*/

    if (idx < 0 || PNULL == data_ptr||PNULL == bar_ptr)
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

        //移动当前总序中的索引，放在下一页的第一个
        bar_ptr->choice_last_index = 0;
        bar_ptr->is_choice_state = TRUE;
    }
    else if (-1 < bar_ptr->hot_index && bar_ptr->hot_index < bar_ptr->cur_page_line_cnt)
    {
        if (!bar_ptr->is_choice_state)
        {
            //-1表示初次进入，其实就是状态不一样，处理不一样
            move_cnt = bar_ptr->choice_index;
        }
        else
        {
            move_cnt = bar_ptr->choice_index - bar_ptr->choice_last_index;

            //不是第一次，说明点的是同一个位置
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
    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/

    MMIIMUI_InitUIHandleData(handle_ptr, kb_state);

    handle_ptr->cstar_handle = InitSpImKernalCode(handle_ptr, method);

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

    /*处理完成后，还要保存内容到result中*/
    SCI_MEMCPY(data_ptr->result, content_ptr->pCandBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->result_len = content_ptr->nCand;

    /*处理完成后保存编辑内容到pPreBuffer中*/
    SCI_MEMCPY(data_ptr->pPreBuffer, content_ptr->pPreBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nPreEdit = content_ptr->nPreEdit;

    /*处理完成后，保存underline的内容到pUnderLine中*/
    SCI_MEMCPY(data_ptr->pUnderLine, content_ptr->pUnderlineBuffer, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->nUnderLine = content_ptr->nUnderline;

    if (!data_ptr->is_full_keypad && !MMIIMUI_IsCommonKeyBoardState(data_ptr))
    {
        MMIIMUI_GetChoiceBufferByKernel(&data_ptr->choice_bar, content_ptr->pChoiceBuffer, content_ptr->nChoice);
    }

    /*根据返回的内容来检查当前输入法的UI显示状态
    如果有commit内容，则一定没有preedit内容，则检查是否有候选字*/

    do
    {
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
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;
            }
            else
            {
                //有编辑内容，无候选字，则为无输入状态
                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_INPUTING;
            }

            break;
        }

    }
    while (0);

    return;
}

/*****************************************************************************/
//  Description : 处理sp事件
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

                    //UI 显示控制，等统一后再调整
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
        if (strtab_ptr[1] != s_sym_del[1] || (data_ptr->result_len <= 0))
        {
            notify_pack.data.commit_data.commit[0] = strtab_ptr[1];
            notify_pack.data.commit_data.commit_len = 1;
        }

        /*下面这两个判断，如果当前有内容，则全部清空*/
        if (MMIIM_TP_CSTAR_STATE_INPUTING == data_ptr->ui_state ||
                MMIIM_TP_CSTAR_STATE_ASSOCIATE == data_ptr->ui_state)
        {
            HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
        }

        //增加此条件，是为了去掉underline的值 cr:191987
        if (0 < data_ptr->nUnderLine)
        {
            HandleSPEvent(handle_ptr, SPIM_EVENT_CHAR_REMOVE_ALL, PNULL);
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

    //处理回退删除掉键
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
        //9键传得是cs_vk0等
        result = HandleSPEvent(data_ptr, SPIM_EVENT_CHAR_APPEND, &api_para);
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

    //SCI_TRACE_LOW:"HandleCaps"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2421_112_2_18_2_29_15_268,(uint8*)"");

    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/

    if (PNULL == handle_ptr || PNULL == event_data_ptr||PNULL == data_ptr->init_param.method_param_ptr)
    {
        return TRUE;
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
//  Author: jian.ma
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

    //SCI_TRACE_LOW:"Handle26KeyTo9Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2469_112_2_18_2_29_15_269,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    //检查当前是否已经是9键
    if (PNULL == data_ptr|| !data_ptr->is_full_keypad ||PNULL == data_ptr->init_param.method_param_ptr )
        return FALSE;

    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/

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
//  Author: jian.ma
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

    //SCI_TRACE_LOW:"Handle9KeyTo26Key"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TP_SP_2523_112_2_18_2_29_15_270,(uint8*)"");

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/

    //检查当前是否已经是26键
    if (PNULL == data_ptr || data_ptr->is_full_keypad||PNULL == data_ptr->init_param.method_param_ptr)
        return FALSE;
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/

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

