/*==============================================================================
File Name: mmiim_tp_multitap_9key.c
Author: Haiwu.Chen
Date: 2010/09/27
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/09/27   Haiwu.Chen      Create.
==============================================================================*/


/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmiim_base.h"

#if defined(TOUCH_PANEL_SUPPORT)

#include "mmiim_internal.h"
#include "mmiim_tp_multitap_9key.h"
#include "mmiim_tp_cstar_internal.h"
#include "mmiim_sp_symbol.h"
#include "mmiim_im_switching.h"
#include "mmiim_tp_cstar_keyboard_cfg.h"
#include "mmiim_tp_ui.h"
#include "mmiim_sp_ml_data.h"
#include "mmk_timer.h"




/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(
    GUIIM_INIT_KERNEL_T *param_ptr   // [in]
);

/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(
    void *handle_ptr    // [in]
);

/******************************************************************************/
// Description: 事件处理
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void *handle_ptr,                       // [in]
    GUIIM_EVENT_T event,                // [in]
    GUIIM_EVENT_DATA_U *event_data_ptr  // [in]
);

/*==============================================================================
Function Name: HandleSysMsg
Description: HandleSysMsg
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,  // [in]
    GUIIM_EVENT_DATA_U const *event_data_ptr        // [in]
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

/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,      // [in]
    GUIIM_EVENT_DATA_U const *event_data_ptr,       // [in]
    MMIIM_TP_CSTAR_MSG_T *msg_ptr                           // [in]
);

/*==============================================================================
Function Name: TpDownProcess
Description: TpDownProcess
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpDownProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,  // [in]
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr        // [in]
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
//  Description : 在联想状态下的系统消息处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpLongProcess(
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

/*==============================================================================
Description: ClearData
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ClearData(MMIIM_TP_CSTAR_HANDLE_T *data_ptr);

/*==============================================================================
Description: SetToNoInput
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetToNoInput(MMIIM_TP_CSTAR_HANDLE_T *data_ptr);

/*==============================================================================
Function Name: SetMethod
Author: haiwu.chen
Global resource dependence: NONE
Note: haiwu.chen
==============================================================================*/
LOCAL BOOLEAN SetMethod(
    MMIIM_TP_CSTAR_HANDLE_T   *data_ptr,    // [in] handle of method
    GUIIM_METHOD_T                method       // [in]
);

/*==============================================================================
Description: CommitChar
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ResetData(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
);

/******************************************************************************/
// Description: 创建定时器
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void CreateContextTimer(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
);

/******************************************************************************/
// Description: 停止定时器
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void StopContextTimer(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
);

/******************************************************************************/
// Description: 定时器处理上屏的回调函数
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void ProcessContextTimerCB(
    uint8 timer_id, 
    uint32 param
);

/*==============================================================================
Function Name: CommitChar
Description: CommitChar
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitChar(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,    // [in]
    wchar c                                   // [in]
);

/*==============================================================================
Description: 提交数据包
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitPackData(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,              // [in] handle of method
    GUIIM_COMMIT_DATA_T        *commit_data_ptr,    // [in] commit data
    BOOLEAN is_lead_caps                            // [in] caps of lead
);

/*==============================================================================
Function Name: DealAfterCommit
Author: haiwu.chen
Global resource dependence: NONE
Note: 善后工作，如果是lead类型则自动再转换一下
==============================================================================*/
LOCAL void DealAfterCommit(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
);

/*==============================================================================
Function Name: HandleMultitap9To26Key
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMultitap9To26Key(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
);

/*****************************************************************************/
//  Description : 当触笔在choice bar上up，对应处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpUpOnChoiceBar(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    int32 idx
);



/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
GUIIM_KERNEL_APIS_T const g_guiim_tp_multitap_9key_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

extern const uint16 s_sym_del[];
extern MMIIM_TP_CSTAR_STATE_T g_keyboard_state[MMIIM_TP_CSTAR_STATE_MAX - MMIIM_TP_CSTAR_KB_START_ID];


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(
    GUIIM_INIT_KERNEL_T *param_ptr   // [in]
)
{
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = PNULL;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = 0;
    BOOLEAN result = TRUE;

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param_ptr->lcd_info_ptr); /*assert verified*/
    if (PNULL == param_ptr ||PNULL == param_ptr->lcd_info_ptr)
    {
        return PNULL;
    }

    //应用句柄各参数初始化
    data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*)SCI_ALLOC_APP(sizeof(MMIIM_TP_CSTAR_HANDLE_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    //9key特有的参数初始化
    data_ptr->multitap_9key_date_ptr = 
        (MMIIM_TP_MULTITAP_9KEY_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_TP_MULTITAP_9KEY_DATA_T));
    SCI_MEMSET(data_ptr->multitap_9key_date_ptr, 0, sizeof(MMIIM_TP_MULTITAP_9KEY_DATA_T));
    
    data_ptr->multitap_9key_date_ptr->last_multitap_char_index = -1;
    data_ptr->multitap_9key_date_ptr->last_multitap_press_index = -1;
    data_ptr->multitap_9key_date_ptr->key_timer_id = 0;
    
    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->lcd_info_ptr = param_ptr->lcd_info_ptr;
    data_ptr->init_param = *param_ptr;

    //create tips layer
    MMIIMUI_CreateTipsLayer(data_ptr, param_ptr);

    data_ptr->stateId = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD;           //重要参数，初始化默认为手写
    data_ptr->is_full_keypad = FALSE;
    data_ptr->is_need_lock = FALSE;
    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;

    //init language data
    MMIIM_SP_ML_Init(&data_ptr->multitap_9key_date_ptr->language_data);

    if (PNULL == data_ptr->init_param.method_param_ptr)
    {
        goto _error_;
    }
    result = MMIIM_SP_ML_SetMethod(&data_ptr->multitap_9key_date_ptr->language_data,
        data_ptr->init_param.method_param_ptr->im_set.method);

    if (!result)
    {
        goto _error_;
    }

    //get keyboard state
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
    kb_state = MMIIMUI_GetVKboardState(data_ptr->init_param.method_param_ptr->im_set.method, data_ptr->is_full_keypad);
    MMIIMUI_InitUIHandleData(data_ptr, kb_state);

    //set display data
    MMIIMUI_InitDisplayData(data_ptr);

    //set method with capital
    result = SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);

    if (!result)
    {
        goto _error_;
    }

    if(g_keyboard_state[data_ptr->stateId].p_disp_text == NULL 
        || g_keyboard_state[data_ptr->stateId].p_disp_text == MMIIMUI_MulKb9KeyGetDefData())      /*lint !e746*/
    {
        g_keyboard_state[data_ptr->stateId].p_disp_text = MMIIMUI_MulKb9KeyGetDefData();          /*lint !e746*/
        MMIIMUI_SetKbMul9KeyTextData(data_ptr->init_param.method_param_ptr->im_set.method, 
            g_keyboard_state[data_ptr->stateId].p_disp_text );
    }

    return (void*) data_ptr;

_error_:
    Terminate(data_ptr);

    return PNULL;
}


/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(
    void *handle_ptr    // [in]
)
{
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;

    //SCI_ASSERT(data_ptr != PNULL); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return FALSE;
    }
    
    StopContextTimer(data_ptr);

    if (0 != data_ptr->tips_timer_id)
    {
        MMK_StopTimer(data_ptr->tips_timer_id);
        data_ptr->tips_timer_id = 0;
    }

    MMIIM_SP_ML_Term(&data_ptr->multitap_9key_date_ptr->language_data);
    SCI_FREE(data_ptr->multitap_9key_date_ptr);

    /*这个检查是有点多余的，如果以后横屏也支持9键的话，那就不必此条件，现在的逻辑就是横屏不保存也不修改9，26状态，均设置为26键*/
    if (!MMITHEME_IsMainScreenLandscape())
    {
        MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&(data_ptr->tips_lcd_info));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(data_ptr->tips_lcd_info));
    }

    SCI_FREE(data_ptr);

    return TRUE;
}

/*==============================================================================
Function Name: HandleMsg
Description: HandleMsg
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(
    void *handle_ptr,                       // [in]
    GUIIM_EVENT_T event,                // [in]
    GUIIM_EVENT_DATA_U *event_data_ptr  // [in]
)
{
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr = PNULL;
    BOOLEAN result = FALSE;

	if(PNULL == handle_ptr)
	{
		return FALSE;
	}
	data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    data_ptr->event_data_ptr = event_data_ptr;
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if (data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return FALSE;
    }
    
    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:
        {
            result = HandleSysMsg(data_ptr, event_data_ptr);
        }
        break;

    case GUIIM_EVENT_SET_METHOD:
        {
            result = SetMethod(data_ptr, event_data_ptr->set_method);
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

    case GUIIM_EVENT_PAINT:
        {
            MMIIMUI_DrawAllBoard(data_ptr);
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_FORCE_COMMIT:
    {
        GUIIM_NOTIFY_PACK_T notify_pack = {0};
        
        DealAfterCommit(data_ptr);
        SetToNoInput(data_ptr);

        notify_pack.notify = GUIIM_NOTIFY_CHANGE_EDIT_STATE;
        GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

        break;
    }
    
    case GUIIM_EVENT_SLEEP:
        {
            //sleep时，应用也会被guiim销毁
            MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);

            SetToNoInput(data_ptr);
            
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_CTRL_RECT: //接收触笔响应的区域
        {
            MMIIMUI_GetRect(data_ptr, &(event_data_ptr->get_ctrl_rect));
            result = TRUE;
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
            if (data_ptr->multitap_9key_date_ptr->has_choice_content)
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

    default:
        break;
    }

    return result;
}


/*==============================================================================
Function Name: HandleSysMsg
Description: HandleSysMsg
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,   // [in]
    GUIIM_EVENT_DATA_U const *event_data_ptr // [in]
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_TP_CSTAR_MSG_T msg = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr || data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return TRUE;
    }
    //如果有tips时，此时来电
    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        //commit last char
        if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
        {
            GUIIM_COMMIT_DATA_T commit_data = {0};
            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                            &data_ptr->multitap_9key_date_ptr->language_data, 
                            data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
                            data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

            commit_data.underline_len = 0;
            commit_data.commit_len = 1;

            CommitPackData(data_ptr, &commit_data, TRUE);
        }
                        
        ClearData(data_ptr);

        return FALSE;
    }

    //特别处理定时器的情况
    //1 定时器触发
    //2 其他按键的发生，暂时没事

    //把外部消息转为内部消息，检查是down,up,long,repeat消息
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    switch (data_ptr->ui_state)
    {
    case MMIIM_TP_CSTAR_STATE_NOINPUT:
        is_dealt = HandleSysMsgNoInput(data_ptr, &msg);
        break;

    default:
        break;
    }

    if (!is_dealt)
    {
        return FALSE;
    }
    
    MMIIMUI_ChangeRect(data_ptr);

    return is_dealt;
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
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == msg_ptr)
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

    case MMIIM_TP_CSTAR_MSG_KEY_PRESS:
        {
            if (PNULL == data_ptr->multitap_9key_date_ptr)
                return FALSE;
            
            switch (msg_ptr->para.key)
            {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
            case KEY_WEB:
            case KEY_OK:
                {
                    StopContextTimer(data_ptr);
                    
                    is_dealt = data_ptr->multitap_9key_date_ptr->has_choice_content;
                    
                    if (is_dealt)
                    {
                        //commit candidate content
                        if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
                        {
                            GUIIM_COMMIT_DATA_T commit_data = {0};
                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                            &data_ptr->multitap_9key_date_ptr->language_data, 
                                            data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
                                            data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

                            commit_data.underline_len = 0;
                            commit_data.commit_len = 1;

                            CommitPackData(data_ptr, &commit_data, TRUE);

                            SetToNoInput(data_ptr);
                        }
                    }

                    //is_dealt *= 2;
                }
                break;

            case KEY_CANCEL:
                {
                    StopContextTimer(data_ptr);
                    
                    is_dealt = data_ptr->multitap_9key_date_ptr->has_choice_content;
                    
                    if (is_dealt)
                    {                        
                        //clear candidate content
                        data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;
                        data_ptr->multitap_9key_date_ptr->last_multitap_char_index = -1;
                        data_ptr->multitap_9key_date_ptr->last_multitap_press_index = -1;
                        CommitChar(data_ptr, '\b');
                    }

                    SetToNoInput(data_ptr);
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
                    StopContextTimer(data_ptr);
                    
                    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                    
                    SetToNoInput(data_ptr);
                    
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

/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_TP_CSTAR_MSG_T *msg_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == event_data_ptr || PNULL == msg_ptr)
    {
        return ;
    }
    MMIIMUI_TransMsg(data_ptr, event_data_ptr, msg_ptr);
}

/*==============================================================================
Function Name: TpDownProcess
Description: TpDownProcess
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN TpDownProcess(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,   // [in]
    MMIIM_TP_CSTAR_MSG_PARA_TP_T const *para_ptr   // [in]
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    if (MMIIM_TP_CSTAR_MSG_TP_NONE == para_ptr->area)
    {
        return FALSE;
    }

    //tp down时，停止定时器
    StopContextTimer(data_ptr);
            
    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            if (para_ptr->idx < 0 || para_ptr->h_idx < 0 || para_ptr->v_idx < 0)
                break;

            //切换到不同的按键，如果choice存在内容，则choice清空
            if((data_ptr->tp_last_msg_param.v_idx != para_ptr->v_idx || data_ptr->tp_last_msg_param.h_idx != para_ptr->h_idx)    
                && data_ptr->choice_bar.total_choice_cnt != 0)
            {
                data_ptr->choice_bar.total_choice_cnt = 0;     
                SCI_MEMSET((wchar *)data_ptr->choice_bar.pChoiceBuffer, 0, MMIIM_CHOICE_MAX_NUM * MMIIM_MAX_PYCHOICE_CHAR_NUM *sizeof(wchar));
            }

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            data_ptr->is_kb_dirty = TRUE;

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
                data_ptr->is_kb_dirty = TRUE;
                
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
    BOOLEAN is_move_out_control = FALSE;
    
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    //tp down时，停止定时器
    StopContextTimer(data_ptr);

    if (MMIIM_TP_CSTAR_MSG_TP_NONE == para_ptr->area)
    {
        is_move_out_control = TRUE;
    }
            
    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            //从candbar进入候选区
            if (MMIIM_TP_CSTAR_MSG_TP_CHOICE == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_SetNormalStateOnChoiceBar(&data_ptr->choice_bar);
                
                data_ptr->is_bar_choice_dirty = TRUE;
            }

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
        }
        break;

    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {            
            //表示焦点从键盘处滑到候选区
            if (MMIIM_TP_CSTAR_MSG_TP_KB == data_ptr->tp_last_msg_param.area)
            {
                data_ptr->is_kb_dirty = TRUE;
            }

            MMIIMUI_TpDownOnChoiceBar(&data_ptr->choice_bar, 
                para_ptr->idx, &data_ptr->is_bar_choice_dirty);

            //保存最后一次TP被按下的参数
            MMIIMUI_SaveLastTpMsgParam(data_ptr, para_ptr);

            if (data_ptr->is_bar_choice_dirty)
            {
                data_ptr->is_kb_dirty = TRUE;
            }
        }
        break;

    default:
        break;
    }

    if (is_move_out_control 
        && PNULL != data_ptr->multitap_9key_date_ptr 
        && data_ptr->multitap_9key_date_ptr->has_choice_content)
    {
        MMIIMUI_SetNormalStateOnChoiceBar(&data_ptr->choice_bar);
        
        data_ptr->is_kb_dirty = TRUE;
        data_ptr->is_bar_choice_dirty = TRUE;
        
        //移出去了，且有可选内容，就重启一下timer
        CreateContextTimer(data_ptr);
    }

    if (data_ptr->is_kb_dirty
        || data_ptr->is_bar_choice_dirty)
    {
        MMIIMUI_ReDrawAllBoard(data_ptr);
    }

    return FALSE;
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
    uint8 id = 0;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
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

        /*TP上触笔长按，无非就是一个回退键，如果不是回退键，则不做任何处理，直接返回true*/
        state = g_keyboard_state[data_ptr->stateId];
        count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);
        id = state.functab[count].id;

        if(id >= g_keyboard_state[data_ptr->stateId].char_cnt)
            break;

        if (!MMIIMUI_SearchFromStrtab(&s_sym_del[1], g_keyboard_state[data_ptr->stateId].char_tab[id]))
        {
            //如果以后要求长按数字直接上屏，则在此增加
            return TRUE;
        }

        //长按删除的处理
        if (data_ptr->multitap_9key_date_ptr->has_choice_content)
        {
#ifdef IM_ENGINE_SOGOU
			CommitChar(data_ptr, '\b');
			SetToNoInput(data_ptr);
#endif
            data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;
            MMIIMUI_ClearChoiceBarData(&data_ptr->choice_bar);
            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);

            //清除连续按键处理，否则删除完成后，下次输入导致多输入一个字符
            data_ptr->multitap_9key_date_ptr->last_multitap_press_index = -1;   
        }
        //else  //需求变更，存在choicebar的时候，依然可以删除完毕
        {
            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

            if (bk_ui_state != data_ptr->ui_state)
            {
                data_ptr->is_rect_change = TRUE;
            }
        }        
        break;
        
    default:
        break;
    }

    return TRUE;
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
    int16 count = 0;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_COMMIT_DATA_T commit_data = {0};
    GUI_BOTH_RECT_T pop_menu_rect = {0};
    uint32 fun_id = 0;

    uint16 const* strtab_ptr = PNULL;
    wchar value = 0;
    wchar commit_value = 0;
    uint16 char_count = 0;
    MMIIM_SP_KEY_INDEX_T cur_key_idx = 0;
    uint16 commit_index = 0;
    uint16 i = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == para_ptr)
    {
        return FALSE;
    }
    
    switch (para_ptr->area)
    {
    case MMIIM_TP_CSTAR_MSG_TP_KB:
        {
            //抬起之前未击中键盘
            if (-1 == data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                return FALSE;
            }

            //不在同一个键上
            if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                return FALSE;
            }
            
            state = g_keyboard_state[data_ptr->stateId];
            count = (int16)(para_ptr->v_idx * state.hnum + para_ptr->h_idx);
            bk_ui_state = data_ptr->ui_state;
            fun_id = state.functab[count].id;

            switch (fun_id)
            {
            case MMIIM_TP_CSTAR_TAG_SHIFT_IM:
                {
                    ResetData(data_ptr);
                    MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
                    MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);
                }
                break;
                
            case MMIIM_TP_CSTAR_TAG_SHIFT_926:
                {
                    HandleMultitap9To26Key(data_ptr); 
                }
                break;
                
            case MMIIM_TP_CSTAR_TAG_SYMBOL:
                {
                    //commit last candidate
                    if (data_ptr->multitap_9key_date_ptr->has_choice_content)
                    {
                        data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;
                        if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
                        {
                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                            &data_ptr->multitap_9key_date_ptr->language_data, 
                                            data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
                                            data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

                            commit_data.commit_len = 1;

                            CommitPackData(data_ptr, &commit_data, FALSE);
                        }
                    }
                    
                    ResetData(data_ptr);
                    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang);
                }
                break;
                
            case MMIIM_TP_CSTAR_STATE_DIG:
                {
                    ResetData(data_ptr);
                    
                    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
                    notify_pack.data.set_method = GUIIM_M_TP_123;
                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                }
                break;
                
            default:
                {
                    strtab_ptr = g_keyboard_state[data_ptr->stateId].char_tab[fun_id];
                    
                    if (PNULL == strtab_ptr)
                    {
                        MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                        return TRUE;
                    }
                    
                    if (1 != strtab_ptr[0])
                        return TRUE;
                    
                    //one key to one character, the handle special character first
                    value = strtab_ptr[1];
                    if ('\b' == value)
                    {
                        CommitChar(data_ptr, value);
                        
                        SetToNoInput(data_ptr);
                    }
                    else if ( '\n' == value
                        || ',' == value)
                    {
                        //commit last candidate
                        if (data_ptr->multitap_9key_date_ptr->has_choice_content)
                        {
                            data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;
                            if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
                            {
                                commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                                &data_ptr->multitap_9key_date_ptr->language_data, 
                                                data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
                                                data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

                                commit_data.commit_len = 1;

                                CommitPackData(data_ptr, &commit_data, TRUE);
                            }
                        }

                        CommitChar(data_ptr, value);
                        
                        SetToNoInput(data_ptr);
                    }
                    else
                    {
#ifdef IM_ENGINE_SOGOU
                        if (' ' == value)
                        {
                                data_ptr->tp_last_msg_param.idx = -1;
                                commit_index = 0;
                                if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
                                {                                
									commit_data.commit[0] = MMIIM_SP_ML_GetChar(
													&data_ptr->multitap_9key_date_ptr->language_data, 
													data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
													data_ptr->multitap_9key_date_ptr->last_multitap_char_index);
                                    commit_data.commit_len = 1;
                                    CommitPackData(data_ptr, &commit_data, TRUE);
                                }
                                commit_data.commit[0] = ' ';
                                commit_data.commit_len = 1;
                                CommitPackData(data_ptr, &commit_data, TRUE);
                                SetToNoInput(data_ptr);
                                break;
                        }
#else
                        if (' ' == value)
                        {
                            cur_key_idx = 9; //9键 空格配置的位置就是9                       
                        }
#endif    
                        else
                        {
                            //get the only one character, then commit, not show the candidate bar
                            if ('1' > value || '9' < value)
                            {
                                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                                return TRUE;
                            }
                            
                            cur_key_idx = value - '1';
                        }                        
                        
                        char_count = MMIIM_SP_ML_GetCount(&data_ptr->multitap_9key_date_ptr->language_data, cur_key_idx);
                        
                        if (1 == char_count)    // if just have 1 char, commit it directly
                        {
                            data_ptr->tp_last_msg_param.idx = -1;
                            
                            //process last candidate
                            commit_index = 0;
                            
                            //commit last candidate
                            if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
                            {                                
                                commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                                &data_ptr->multitap_9key_date_ptr->language_data, 
                                                data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
                                                data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

                                commit_data.commit_len = 1;

                                CommitPackData(data_ptr, &commit_data, TRUE);
                            }
                                                        
                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                &data_ptr->multitap_9key_date_ptr->language_data,
                                cur_key_idx, 1);                            
                            commit_data.commit_len = 1;
                            
                            CommitPackData(data_ptr, &commit_data, TRUE);
                            
                            SetToNoInput(data_ptr);
                        }
                        else if (char_count > 1)
                        {
                            //press last key again
                            if (data_ptr->multitap_9key_date_ptr->last_multitap_press_index == cur_key_idx)
                            {                                
                                data_ptr->multitap_9key_date_ptr->last_multitap_char_index++;

                                if (data_ptr->multitap_9key_date_ptr->last_multitap_char_index > char_count)
                                {
                                    data_ptr->multitap_9key_date_ptr->last_multitap_char_index = 1;
                                }

                                commit_data.underline_len = 1;
                                commit_data.underline[0] = MMIIM_SP_ML_GetChar(
                                        &data_ptr->multitap_9key_date_ptr->language_data, 
                                        cur_key_idx, 
                                        data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

                                CommitPackData(data_ptr, &commit_data, FALSE);
                                
                                CreateContextTimer(data_ptr);
                                
                                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                            }
                            else
                            {
                                wchar choice_content[CAND_MAX_NUM] = {0};
                                
                                commit_index = 0;
                                //commit last candidate
                                if (-1 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
                                {
                                    StopContextTimer(data_ptr);
                                    
                                    commit_data.commit[commit_index] = MMIIM_SP_ML_GetChar(
                                                    &data_ptr->multitap_9key_date_ptr->language_data,
                                                    data_ptr->multitap_9key_date_ptr->last_multitap_press_index,
                                                    data_ptr->multitap_9key_date_ptr->last_multitap_char_index);

                                    commit_data.commit_len = 1;

                                    CommitPackData(data_ptr, &commit_data, TRUE);
                                }

                                data_ptr->multitap_9key_date_ptr->last_multitap_press_index = cur_key_idx;
                                data_ptr->multitap_9key_date_ptr->last_multitap_char_index = 1;
                                SCI_MEMSET(choice_content, 0, sizeof(choice_content));
                                
                                data_ptr->multitap_9key_date_ptr->has_choice_content = TRUE;

                                //set content
                                for (i = 0; i < char_count; i++)
                                {
                                    commit_value = MMIIM_SP_ML_GetChar(
                                        &data_ptr->multitap_9key_date_ptr->language_data,
                                        cur_key_idx, i + 1);
                                    
                                    choice_content[2*i] = commit_value;
                                    choice_content[2*i + 1] = '\0';
                                }
                                
                                choice_content[2*char_count+1] = '\0'; //add one more '\0' to the end.
                                choice_content[2*char_count+2] = '\0';

                                //get underline content
                                commit_data.underline[0] = choice_content[0];
                                commit_data.underline_len = 1;
                                commit_data.commit_len = 0;
                                CommitPackData(data_ptr, &commit_data, FALSE);
                                
                                CreateContextTimer(data_ptr);
                                
                                MMIIMUI_GetChoiceBufferByKernel(&data_ptr->choice_bar, choice_content, char_count);
                               
                                MMIIMUI_ClearPressKBMark(data_ptr);
                                
                                //data_ptr->is_rect_change = TRUE;
                                data_ptr->is_kb_dirty = TRUE;

                                //redraw    
                                data_ptr->tp_last_msg_param.idx = -1;
                                MMIIMUI_ReDrawAllBoard(data_ptr); 
                            }                                                               
                        }
                        else
                        {
                            //无输入
                            MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                            return FALSE;
                        }
                    }
                }
                break;
            }
        }
        break;
        
    case MMIIM_TP_CSTAR_MSG_TP_CHOICE:
        {
            //先处理一下内容
            TpUpOnChoiceBar(data_ptr, para_ptr->idx);

            if (data_ptr->is_bar_choice_dirty
                    || data_ptr->is_kb_dirty)
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

/*==============================================================================
Description: ClearData
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ClearData(MMIIM_TP_CSTAR_HANDLE_T *data_ptr)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return ;
    }
    
    MMIIMUI_HideKeyTips(data_ptr);
    
    StopContextTimer(data_ptr);

    if (0 != data_ptr->tips_timer_id)
    {
        MMK_StopTimer(data_ptr->tips_timer_id);
        data_ptr->tips_timer_id = 0;
    }

    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;
    data_ptr->multitap_9key_date_ptr->last_multitap_press_index= -1;
    data_ptr->multitap_9key_date_ptr->last_multitap_char_index = -1;
    MMIIMUI_ClearChoiceBarData(&data_ptr->choice_bar);
    data_ptr->tp_last_msg_param.area = MMIIM_TP_CSTAR_MSG_TP_NONE;
    data_ptr->tp_last_msg_param.idx = -1;
    data_ptr->result_len = 0;

    return;
}

/*==============================================================================
Description: SetToNoInput
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetToNoInput(MMIIM_TP_CSTAR_HANDLE_T *data_ptr)
{              
    //alter the ui state
    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    StopContextTimer(data_ptr);
    
    MMIIMUI_ClearChoiceBarData(&data_ptr->choice_bar);
    
    data_ptr->multitap_9key_date_ptr->has_choice_content = FALSE;
    data_ptr->multitap_9key_date_ptr->last_multitap_press_index = -1;
    data_ptr->multitap_9key_date_ptr->last_multitap_char_index = -1;
    data_ptr->is_kb_dirty= TRUE;
    data_ptr->is_bar_choice_dirty = TRUE;
    MMIIMUI_ClearKbUpOnInValidRect(data_ptr);

    return;
}

/*==============================================================================
Function Name: SetMethod
Author: haiwu.chen
Global resource dependence: NONE
Note: haiwu.chen
==============================================================================*/
LOCAL BOOLEAN SetMethod(
    MMIIM_TP_CSTAR_HANDLE_T   *data_ptr,    // [in] handle of method
    GUIIM_METHOD_T                method       // [in]
)
{
    BOOLEAN result = TRUE;
    MMIIM_TP_CSTAR_STATE_ID_T kb_state = 0;                     //应用程序键盘状态
    GUIIM_METHOD_CFG_T *method_cfg_ptr =  PNULL;
    //GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr);     /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->multitap_9key_date_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/
    if(PNULL == data_ptr||PNULL == data_ptr->multitap_9key_date_ptr||PNULL == data_ptr->init_param.method_param_ptr||PNULL == data_ptr->init_param.method_param_ptr->im_def_ptr)
    {
        return FALSE;
    }
    
    ClearData(data_ptr);
    
    method_cfg_ptr =  MMITHEME_IMGetDefByMethod(method);

    if (PNULL == method_cfg_ptr)
    {
        return FALSE;
    }
    
    kb_state = MMIIMUI_GetVKboardState(method, data_ptr->is_full_keypad);
    MMIIMUI_InitUIHandleData(data_ptr, kb_state);

    //set display data
    MMIIMUI_InitDisplayData(data_ptr);
    
    result = MMIIM_SP_ML_SetMethod(&data_ptr->multitap_9key_date_ptr->language_data, method);
    
    //输入法切换后，应该从新配置虚拟键盘按键丝印数据
    if(g_keyboard_state[kb_state].p_disp_text == NULL 
        || g_keyboard_state[kb_state].p_disp_text == MMIIMUI_MulKb9KeyGetDefData())
    {
        g_keyboard_state[kb_state].p_disp_text = MMIIMUI_MulKb9KeyGetDefData();
        MMIIMUI_SetKbMul9KeyTextData(method, g_keyboard_state[kb_state].p_disp_text );
    }
    
    if (GUIIM_MODE_LEAD_UPPER == method_cfg_ptr->capital)
    {
        //need to reset kb imgae with the upper one
        MMIIMUI_ResetKbImageByLeadCape(data_ptr);
    }

    //notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
    //GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return result;
}

/*==============================================================================
Description: CommitChar
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ResetData(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
)
{
    if (-1 != data_ptr->tp_last_msg_param.idx ||
        0 != data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
    {
        if (0 != data_ptr->tips_timer_id)
        {
            MMK_StopTimer(data_ptr->tips_timer_id);
            data_ptr->tips_timer_id = 0;
        }
        
        data_ptr->multitap_9key_date_ptr->last_multitap_press_index = -1;
        data_ptr->multitap_9key_date_ptr->last_multitap_char_index = -1;
        data_ptr->tp_last_msg_param.idx = -1;
    }

    return;
}

/******************************************************************************/
// Description: 创建定时器
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void CreateContextTimer(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    
    StopContextTimer(data_ptr);
    
    data_ptr->multitap_9key_date_ptr->key_timer_id = 
        MMK_CreateTimerCallback(MMIIM_MULTITAP_9KEY_CANDIDATE_TIMER_OUT, ProcessContextTimerCB, (uint32)data_ptr, FALSE);                                    
    
    return;
}

/******************************************************************************/
// Description: 停止定时器
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void StopContextTimer(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->multitap_9key_date_ptr); /*assert verified*/
    if(PNULL == data_ptr ||PNULL == data_ptr->multitap_9key_date_ptr)
    {
        return;
    }
    
    if (0 != data_ptr->multitap_9key_date_ptr->key_timer_id)
    {
        MMK_StopTimer(data_ptr->multitap_9key_date_ptr->key_timer_id);
        data_ptr->multitap_9key_date_ptr->key_timer_id = 0;
    }
    
    return;
}

/******************************************************************************/
// Description: 定时器处理上屏的回调函数
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void ProcessContextTimerCB(
    uint8 timer_id, 
    uint32 param
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_COMMIT_DATA_T commit_data = {0};
    MMIIM_TP_CSTAR_HANDLE_T* data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*)param;

    if (PNULL == data_ptr || PNULL == data_ptr->multitap_9key_date_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != data_ptr->multitap_9key_date_ptr->key_timer_id)
    {
        return;
    }
    
    if (-1 == data_ptr->multitap_9key_date_ptr->last_multitap_press_index)
    {
        return;
    }

    commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                            &data_ptr->multitap_9key_date_ptr->language_data, 
                            data_ptr->multitap_9key_date_ptr->last_multitap_press_index, 
                            data_ptr->multitap_9key_date_ptr->last_multitap_char_index);
    commit_data.commit_len = 1;

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data = commit_data;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    if (GUIIM_MODE_LEAD_UPPER == data_ptr->multitap_9key_date_ptr->language_data.method_cfg_ptr->capital)
    {
        GUIIM_METHOD_T new_method = MMITHEME_IMGetMethodByCapital(
                                        data_ptr->multitap_9key_date_ptr->language_data.method_cfg_ptr->method,
                                        GUIIM_MODE_LOWER);

        if (GUIIM_M_NONE == new_method)
        {
            return;
        }

        SetMethod(data_ptr, new_method);

        notify_pack.data.switch_method = new_method;
        notify_pack.notify = GUIIM_NOTIFY_SWITCH_METHOD;
        
        GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);
    }
    else
    {
        ClearData(data_ptr);

        MMIIMUI_DrawAllBoard(data_ptr);

        //由于自动上屏后，我们需求更新softkey状态
        notify_pack.notify = GUIIM_NOTIFY_CHANGE_EDIT_STATE;
        GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);
    }

    return;
}

/*==============================================================================
Function Name: CommitChar
Description: CommitChar
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitChar(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,    // [in]
    wchar c                                   // [in]
)
{
    GUIIM_NOTIFY_PACK_T     notify_pack = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
    notify_pack.notify = GUIIM_NOTIFY_COMMIT;

    // commit the specified char
    notify_pack.data.commit_data.commit[0] = c;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    
    return;
}

/*==============================================================================
Description: 提交数据包
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitPackData(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,              // [in] handle of method
    GUIIM_COMMIT_DATA_T        *commit_data_ptr,    // [in] commit data
    BOOLEAN is_lead_caps                            // [in] caps of lead
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data = *commit_data_ptr;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    //only lead capital use
    if (is_lead_caps)
    {
        DealAfterCommit(data_ptr);
    }

    return;
}

/*==============================================================================
Function Name: DealAfterCommit
Author: haiwu.chen
Global resource dependence: NONE
Note: 善后工作，如果是lead类型则自动再转换一下
==============================================================================*/
LOCAL void DealAfterCommit(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T new_method = {0};

    // if it is lead upper mode, change to lower method
    if (GUIIM_MODE_LEAD_UPPER == data_ptr->multitap_9key_date_ptr->language_data.method_cfg_ptr->capital)
    {
        new_method = MMITHEME_IMGetMethodByCapital(data_ptr->multitap_9key_date_ptr->language_data.method_cfg_ptr->method, GUIIM_MODE_LOWER);

        if (GUIIM_M_NONE == new_method)
        {
            return;
        }
        
        SetMethod(data_ptr, new_method);

        notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
        notify_pack.data.set_method = new_method;
        
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }

    return;
}

/*==============================================================================
Function Name: HandleMultitap9To26Key
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMultitap9To26Key(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
    )
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return  FALSE;
    }
    // find the relative tp 9key method
    notify_pack.data.set_method = MMITHEME_IMGetMethodByInput(
        data_ptr->multitap_9key_date_ptr->language_data.method_cfg_ptr->method, GUIIM_INPUT_TYPE_TP_QWERT);
    
    if (GUIIM_M_NONE != notify_pack.data.set_method)
    {
        ResetData(data_ptr);
        
        notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 当触笔在choice bar上up，对应处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpUpOnChoiceBar(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr,
    int32 idx
)
{
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return;
    }
        
    if (idx >= 0)
    {
        if (CAND_LEFT_ARROW_IDX == idx)
        {           
            //向上翻页
            if (0 < data_ptr->choice_bar.page_index)
            {
                data_ptr->choice_bar.page_index--;

                if (data_ptr->choice_bar.page_index > 0)
                {
                    data_ptr->choice_bar.idc_up_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;
                }
                else
                {
                    data_ptr->choice_bar.idc_up_state = MMIIM_TP_CSTAR_KEY_STATE_INACTIVE;
                }

                data_ptr->choice_bar.cur_page_line_cnt = data_ptr->choice_bar.choice_line_cnt;
                data_ptr->choice_bar.idc_down_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;

                CreateContextTimer(data_ptr);
            }
        }
        else if (CAND_RIGHT_ARROW_IDX == idx)
        {            
            //向下翻页
            if (data_ptr->choice_bar.page_index < data_ptr->choice_bar.page_cnt - 1)
            {
                data_ptr->choice_bar.page_index++;

                if (data_ptr->choice_bar.page_index < data_ptr->choice_bar.page_cnt - 1)
                {
                    data_ptr->choice_bar.idc_down_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;
                    data_ptr->choice_bar.cur_page_line_cnt = data_ptr->choice_bar.choice_line_cnt;
                }
                else
                {
                    data_ptr->choice_bar.idc_down_state = MMIIM_TP_CSTAR_KEY_STATE_INACTIVE;
                    data_ptr->choice_bar.cur_page_line_cnt = data_ptr->choice_bar.total_choice_cnt - data_ptr->choice_bar.page_index * data_ptr->choice_bar.choice_line_cnt;
                }

                data_ptr->choice_bar.idc_up_state = MMIIM_TP_CSTAR_KEY_STATE_ACTIVE;

                CreateContextTimer(data_ptr);
            }
        }
        else if (-1 < data_ptr->choice_bar.hot_index && data_ptr->choice_bar.hot_index < data_ptr->choice_bar.cur_page_line_cnt)
        {
            //find content and commit
            GUIIM_COMMIT_DATA_T commit_data = {0};
            uint16 choice_index = 0;
            wchar commit_value = 0;
            choice_index = data_ptr->choice_bar.choice_line_cnt * data_ptr->choice_bar.page_index + data_ptr->choice_bar.hot_index;

            if (choice_index <= data_ptr->choice_bar.total_choice_cnt)
            {
                commit_value = data_ptr->choice_bar.pChoiceBuffer[choice_index][0];
                if (0 != commit_value)
                {
                   commit_data.commit[0] = commit_value;
                    commit_data.commit_len = 1;
                    CommitPackData(data_ptr, &commit_data, TRUE);

                    SetToNoInput(data_ptr); 
                }
            }
        }
    }

    data_ptr->choice_bar.hot_index = -1;
    data_ptr->is_bar_choice_dirty = TRUE;
    data_ptr->is_kb_dirty = TRUE;
    
    return;
}

#else

GUIIM_KERNEL_APIS_T const g_guiim_tp_multitap_9key_apis = {0};

#endif //TOUCH_PANEL_SUPPORT

