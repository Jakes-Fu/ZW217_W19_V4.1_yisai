/*==============================================================================
File Name: mmiim_tp_multitap_26key.c
Author: Haiwu.Chen
Date: 2010/09/04
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/09/04   Haiwu.Chen      Create.
==============================================================================*/

#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmiim_base.h"

#if defined(TOUCH_PANEL_SUPPORT)

#include "mmiim_internal.h"
#include "mmiim_tp_multitap_26key.h"
#include "mmiim_tp_cstar_internal.h"
#include "mmiim_sp_symbol.h"
#include "mmiim_im_switching.h"
#include "mmiim_tp_cstar_keyboard_cfg.h"
#include "mmiim_tp_cstar_position.h"
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
//  Description : 触笔点击候选框弹起操作
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void TpCandPressUp(
    MMIIM_TP_CSTAR_HANDLE_T* handle_ptr, //in:输入法句柄
    GUI_POINT_T tp_point,                  //in:点击的位置
    GUIIM_EVENT_DATA_U* event_data_ptr         //in&out:传入和传出的参数
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
Function Name: HandleMultitapCaps
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMultitapCaps(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
);

/*==============================================================================
Function Name: HandleMultitap26To9Key
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMultitap26To9Key(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
);



/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
GUIIM_KERNEL_APIS_T const g_guiim_tp_multitap_26key_apis =
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
    BOOLEAN result = FALSE;

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param_ptr->lcd_info_ptr); /*assert verified*/
    if(PNULL == param_ptr || PNULL == param_ptr->lcd_info_ptr)
    {
        return PNULL;
    }
    
    //应用句柄各参数初始化，不初始化的参数即messet为0
    data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) SCI_ALLOC_APP(sizeof(MMIIM_TP_CSTAR_HANDLE_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_TP_CSTAR_HANDLE_T));

    //26key特有的参数初始化
    data_ptr->multitap_26key_date_ptr = 
        (MMIIM_TP_MULTITAP_26KEY_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_TP_MULTITAP_26KEY_DATA_T));
    SCI_MEMSET(data_ptr->multitap_26key_date_ptr, 0, sizeof(MMIIM_TP_MULTITAP_26KEY_DATA_T));
    
    data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
    data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;
    data_ptr->multitap_26key_date_ptr->key_timer_id = 0;

    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->lcd_info_ptr = param_ptr->lcd_info_ptr;
    data_ptr->init_param = *param_ptr;

    //create tips layer
    MMIIMUI_CreateTipsLayer(data_ptr, param_ptr);

    data_ptr->stateId = MMIIM_TP_CSTAR_STATE_ABC_26KEY_LEAD;           //重要参数，初始化默认为手写
    data_ptr->is_full_keypad = TRUE;
    data_ptr->is_need_lock = FALSE;
    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;

    //init language data
    MMIIM_SP_ML_Init(&data_ptr->multitap_26key_date_ptr->language_data);
	
    //get keyboard state
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr);     /*assert verified*/
    if(PNULL == data_ptr->init_param.method_param_ptr)
    {
        goto _error_;
    }
	
    result = MMIIM_SP_ML_SetMethod(&data_ptr->multitap_26key_date_ptr->language_data,
        data_ptr->init_param.method_param_ptr->im_set.method);

    if (!result)
    {
        goto _error_;
    }
    //set method with capital
    result = SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);

    if (!result)
    {
        goto _error_;
    }

    //为空表示未配置
    //默认配置，默认配置需要重写，因切换语言可能修改该值，而初始化配置不需要该值
    if(g_keyboard_state[data_ptr->stateId].p_disp_text == NULL		
    	|| g_keyboard_state[data_ptr->stateId].p_disp_text == MMIIMUI_MulKb26KeyGetDefData())	    /*lint !e746*/
    {
    	g_keyboard_state[data_ptr->stateId].p_disp_text = MMIIMUI_MulKb26KeyGetDefData();       /*lint !e746*/
    	MMIIMUI_SetKbMul26KeyTextData(data_ptr->init_param.method_param_ptr->im_set.method, 
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
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
    
    StopContextTimer(data_ptr);

    MMIIM_SP_ML_Term(&data_ptr->multitap_26key_date_ptr->language_data);
    SCI_FREE(data_ptr->multitap_26key_date_ptr);

    /*这个检查是有点多余的，如果以后横屏也支持9键的话，那就不必此条件，现在的逻辑就是横屏不保存也不修改9，26状态，均设置为26键*/
    if (!MMITHEME_IsMainScreenLandscape())
    {
        MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);
    }

    if (0 != data_ptr->tips_timer_id)
    {
        MMK_StopTimer(data_ptr->tips_timer_id);
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&(data_ptr->tips_lcd_info));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(& (data_ptr->tips_lcd_info));
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
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr = (MMIIM_TP_CSTAR_HANDLE_T*) handle_ptr;
    BOOLEAN result = FALSE;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(data_ptr->stateId < MMIIM_TP_CSTAR_STATE_MAX); /*assert verified*/
    if(PNULL == data_ptr || data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return FALSE;
    }

    data_ptr->event_data_ptr = event_data_ptr;
    
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
            SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
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
        DealAfterCommit(data_ptr);
        SetToNoInput(data_ptr);
        MMIIMUI_ChangeRect(data_ptr);
        result = TRUE;
        break;

    case GUIIM_EVENT_SLEEP:
        {
            //sleep时，应用也会被guiim销毁
            MMIIM_SetKeyBoradType(data_ptr->is_full_keypad);

            SetToNoInput(data_ptr);
            MMIIMUI_ChangeRect(data_ptr);
            
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_CTRL_RECT: //接收触笔响应的区域
        {
            MMIIMUI_GetRect(data_ptr, & (event_data_ptr->get_ctrl_rect));
            result = TRUE;
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
            if (MMIIM_TP_CSTAR_STATE_ASSOCIATE == data_ptr->ui_state)
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
    if(PNULL == data_ptr || PNULL == event_data_ptr ||data_ptr->stateId >= MMIIM_TP_CSTAR_STATE_MAX)
    {
        return FALSE;
    }


    //如果有tips时，此时来电
    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if(PNULL == data_ptr || PNULL == msg_ptr)
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
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_COMMIT_DATA_T commit_data = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if(PNULL == data_ptr || PNULL == msg_ptr)
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
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
            case KEY_WEB:
            case KEY_OK:
                {
                    StopContextTimer(data_ptr);

                    is_dealt = (data_ptr->result_len > 0);
                    
                    if (is_dealt)
                    {
                        //commit candidate content
                        if (-1 != data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
                        {
                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                            &data_ptr->multitap_26key_date_ptr->language_data, 
                                            data_ptr->multitap_26key_date_ptr->last_multitap_press_index,
                                            data_ptr->multitap_26key_date_ptr->last_multitap_char_index);

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
                    is_dealt = (data_ptr->result_len > 0);
                    
                    if (is_dealt)
                    {
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
    if(PNULL == data_ptr || PNULL == msg_ptr||PNULL == event_data_ptr)
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
    if(PNULL == data_ptr || PNULL == para_ptr)
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
            {
                break;
            }

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
    if(PNULL == data_ptr || PNULL == para_ptr)
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
            if (MMIIM_TP_CSTAR_MSG_TP_CAND == data_ptr->tp_last_msg_param.area)
            {
                //保存最后一次TP被按下的参数
                data_ptr->tp_last_msg_param.idx = -1;

                data_ptr->is_bar_cand_dirty = TRUE;
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

    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            //表示焦点从键盘处滑到候选区
            if (MMIIM_TP_CSTAR_MSG_TP_KB == data_ptr->tp_last_msg_param.area)
            {
                MMIIMUI_ClearPressKBMark(data_ptr);
                data_ptr->is_kb_dirty = TRUE;
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

    default:
        break;
    }

    if (data_ptr->is_kb_dirty
        || data_ptr->is_bar_cand_dirty)
    {
        MMIIMUI_ReDrawAllBoard(data_ptr);
    }

    if (is_move_out_control 
        && PNULL != data_ptr->multitap_26key_date_ptr
        && -1 != data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
    {
        //移出去了，且有可选内容，就重启一下timer
        CreateContextTimer(data_ptr);
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
    if(PNULL == data_ptr || PNULL == para_ptr)
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
    GUI_BOTH_RECT_T pop_menu_rect = {0};
    uint32 fun_id = 0;

    uint16 const* strtab_ptr = PNULL;
    wchar value = 0;
    wchar commit_value = 0;
    uint16 char_count = 0;
    MMIIM_SP_KEY_INDEX_T cur_key_idx = 0;
    GUIIM_COMMIT_DATA_T commit_data = {0};
    uint16 i = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
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
            MMIIM_SwitchingCreateWin(& (data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);
        }
        break;

    case MMIIM_TP_CSTAR_TAG_SHIFT_CAP:
        {
            HandleMultitapCaps(data_ptr);
        }
        break;

    case MMIIM_TP_CSTAR_TAG_SHIFT_926:
        {
            if(!HandleMultitap26To9Key(data_ptr))
            {
                //如果没有处理9,26切换，则需要刷下键盘，去除高亮（横屏时需要）
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            }
        }
        break;

    case MMIIM_TP_CSTAR_TAG_SYMBOL:
        {
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
                return TRUE;
            }
            
            if (1 != strtab_ptr[0])
                return TRUE;
            
            //one key to one character, the handle special character first
            value = strtab_ptr[1];

            if ('\b' == value
                    || '\n' == value
                    || ' ' == value
                    || ',' == value)
            {
                CommitChar(data_ptr, value);

                data_ptr->tp_last_msg_param.idx = -1;
                data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
                data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;

                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
            }
            else
            {
                //get the only one character, then commit, not show the candidate bar
                if ('a' > value || 'z' < value)
                {
                    return TRUE;
                }

                cur_key_idx = value - 'a';

                char_count = MMIIM_SP_ML_GetCount(&data_ptr->multitap_26key_date_ptr->language_data, cur_key_idx);

                if (1 == char_count)    // if just have 1 char, commit it directly
                {
                    data_ptr->tp_last_msg_param.idx = -1;
                    data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
                    data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;

                    commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                                           &data_ptr->multitap_26key_date_ptr->language_data,
                                                           cur_key_idx, 1);
                    commit_data.commit_len = 1;

                    CommitPackData(data_ptr, &commit_data, TRUE);
                    MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                }
                else if (char_count > 1)
                {
                    data_ptr->multitap_26key_date_ptr->last_multitap_press_index = cur_key_idx;
                    data_ptr->multitap_26key_date_ptr->last_multitap_char_index = 1;

                    SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM*(sizeof(wchar)));

                    for (i = 0; i < char_count; i++)
                    {
                        commit_value = MMIIM_SP_ML_GetChar(
                                           &data_ptr->multitap_26key_date_ptr->language_data,
                                           cur_key_idx, i + 1);

                        data_ptr->result[2*i] = commit_value;
                        data_ptr->result[2*i + 1] = '\0';
                    }

                    data_ptr->result[2*char_count+1] = '\0'; //add one more '\0' to the end.
                    data_ptr->result[2*char_count+2] = '\0';
                    data_ptr->result_len = char_count;

                    commit_data.underline[0] = data_ptr->result[0];
                    commit_data.underline_len = 1;
                    CommitPackData(data_ptr, &commit_data, FALSE);
                                
                    CreateContextTimer(data_ptr);

                    //alter the ui state
                    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_ASSOCIATE;

                    //set the redraw signal
                    MMIIMUI_HideKeyTips(data_ptr);
                    data_ptr->is_rect_change = TRUE;
                    data_ptr->is_bar_cand_dirty = TRUE;
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
    int16 count = 0;
    MMIIM_TP_CSTAR_IM_STATE_T bk_ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUI_BOTH_RECT_T pop_menu_rect = {0};
    uint32 fun_id = 0;
    uint16 const* strtab_ptr = PNULL;
    wchar value = 0;
    wchar commit_value = 0;
    uint16 char_count = 0;
    MMIIM_SP_KEY_INDEX_T cur_key_idx = 0;
    GUIIM_COMMIT_DATA_T commit_data = {0};
    uint16 commit_index = 0;
    uint16 i = 0;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != para_ptr); /*assert verified*/
    if(PNULL == data_ptr || PNULL == para_ptr)
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
                return FALSE;
            }

            //不在同一个键上
            if (para_ptr->idx != data_ptr->tp_last_msg_param.idx)
            {
                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                return FALSE;
            }

            //MMIIM_StopTimer(&(data_ptr->is_timer_active));

            //SCI_ASSERT(para_ptr->idx >= 0 && para_ptr->idx < data_ptr->kb.key_cnt); /*assert verified*/
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
                    MMIIM_SwitchingCreateWin(& (data_ptr->init_param), pop_menu_rect, MMIIM_EP_BOTTOM_LEFT);
                }
                break;

            case MMIIM_TP_CSTAR_TAG_SHIFT_CAP:
               {
                    HandleMultitapCaps(data_ptr);
               }
            break;

            case MMIIM_TP_CSTAR_TAG_SHIFT_926:
                {
                    HandleMultitap26To9Key(data_ptr);
                }
            break;

            case MMIIM_TP_CSTAR_TAG_SYMBOL:
                {
                    //commit last candidate
                    if (-1 != data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
                    {
                        StopContextTimer(data_ptr);
                        
                        commit_data.commit[commit_index] = MMIIM_SP_ML_GetChar(
                                        &data_ptr->multitap_26key_date_ptr->language_data,
                                        data_ptr->multitap_26key_date_ptr->last_multitap_press_index,
                                        data_ptr->multitap_26key_date_ptr->last_multitap_char_index);
                        commit_data.commit_len = 1;

                        CommitPackData(data_ptr, &commit_data, FALSE);
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
                        return TRUE;

                    if (1 != strtab_ptr[0])
                        return TRUE;
                    
                    //one key to one character, the handle special character first
                    value = strtab_ptr[1];

                    if ('\b' == value)
                    {
                        //commit '\b'
                        CommitChar(data_ptr, value);

                        SetToNoInput(data_ptr);
                    }
                    else if ('\n' == value || ' ' == value || ',' == value)
                    {
                        SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM*(sizeof(wchar)));
                        data_ptr->result_len = 0;

                        //commit char
                        if (-1 != data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
                        {
                            StopContextTimer(data_ptr);
                            
                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                            &data_ptr->multitap_26key_date_ptr->language_data, 
                                            data_ptr->multitap_26key_date_ptr->last_multitap_press_index,
                                            data_ptr->multitap_26key_date_ptr->last_multitap_char_index);
                            
                            commit_data.commit_len = 1;

                            CommitPackData(data_ptr, &commit_data, TRUE);
                        }

                        CommitChar(data_ptr, value);

                        SetToNoInput(data_ptr);
                    }
                    else
                    {
                        //if there are some candidate characters
                        //get the only one character, then commit, not show the candidate bar
                        if ('a' > value || 'z' < value)
                        {
                            return TRUE;
                        }

                        cur_key_idx = value - 'a';

                        char_count = MMIIM_SP_ML_GetCount(&data_ptr->multitap_26key_date_ptr->language_data, cur_key_idx);

                        if (1 == char_count)    // if just have 1 char, commit it directly
                        {
                            //commit last candidate
                            if (-1 != data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
                            {
                                StopContextTimer(data_ptr);
                            
                                commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                                &data_ptr->multitap_26key_date_ptr->language_data, 
                                                data_ptr->multitap_26key_date_ptr->last_multitap_press_index,
                                                data_ptr->multitap_26key_date_ptr->last_multitap_char_index);

                                commit_data.commit_len = 1;

                                CommitPackData(data_ptr, &commit_data, TRUE);
                            }
                            
                            //用户放弃候选框的内容
                            data_ptr->tp_last_msg_param.idx = -1;
                            data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
                            data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;

                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                                                   &data_ptr->multitap_26key_date_ptr->language_data,
                                                                   cur_key_idx, 1);

                            commit_data.commit_len = 1;

                            CommitPackData(data_ptr, &commit_data, TRUE);

                            SetToNoInput(data_ptr);
                        }
                        else if (char_count > 1)
                        {
                            //press last key again
                            if (data_ptr->multitap_26key_date_ptr->last_multitap_press_index == cur_key_idx)
                            {
                                data_ptr->multitap_26key_date_ptr->last_multitap_char_index++;

                                if (data_ptr->multitap_26key_date_ptr->last_multitap_char_index > char_count)
                                {
                                    data_ptr->multitap_26key_date_ptr->last_multitap_char_index = 1;
                                }

                                commit_data.underline_len = 1;
                                commit_data.underline[0] = MMIIM_SP_ML_GetChar(
                                        &data_ptr->multitap_26key_date_ptr->language_data, 
                                        cur_key_idx, 
                                        data_ptr->multitap_26key_date_ptr->last_multitap_char_index);

                                CommitPackData(data_ptr, &commit_data, FALSE);
                                
                                CreateContextTimer(data_ptr);
                                
                                MMIIMUI_ClearKbUpOnInValidRect(data_ptr);
                            }
                            else 
                            {
                                //commit last candidate
                                if (-1 != data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
                                {
                                    StopContextTimer(data_ptr);
                                    
                                    commit_data.commit[commit_index] = MMIIM_SP_ML_GetChar(
                                                    &data_ptr->multitap_26key_date_ptr->language_data,
                                                    data_ptr->multitap_26key_date_ptr->last_multitap_press_index,
                                                    data_ptr->multitap_26key_date_ptr->last_multitap_char_index);
                                    commit_data.commit_len = 1;

                                    CommitPackData(data_ptr, &commit_data, TRUE);
                                }
                                
                                data_ptr->multitap_26key_date_ptr->last_multitap_press_index = cur_key_idx;
                                data_ptr->multitap_26key_date_ptr->last_multitap_char_index = 1;

                                SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM*(sizeof(wchar)));

                                for (i = 0; i < char_count; i++)
                                {
                                    commit_value = MMIIM_SP_ML_GetChar(
                                                       &data_ptr->multitap_26key_date_ptr->language_data,
                                                       cur_key_idx, i + 1);

                                    data_ptr->result[2*i] = commit_value;
                                    data_ptr->result[2*i + 1] = '\0';
                                }

                                data_ptr->result[2*char_count+1] = '\0'; //add one more '\0' to the end.
                                data_ptr->result[2*char_count+2] = '\0';
                                data_ptr->result_len = char_count;

                                //get underline content
                                commit_data.underline[0] = data_ptr->result[0];
                                commit_data.underline_len = 1;
                                commit_data.commit_len = 0;
                                CommitPackData(data_ptr, &commit_data, FALSE);
                                
                                CreateContextTimer(data_ptr);

                                //alter the ui state
                                data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_ASSOCIATE;

                                //set the redraw signal
                                MMIIMUI_HideKeyTips(data_ptr);
                                data_ptr->is_rect_change = TRUE;
                                data_ptr->is_bar_cand_dirty = TRUE;
                            }
                        }
                        else if(0 == char_count)
                        {                
                            commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                                &data_ptr->multitap_26key_date_ptr->language_data, 
                                                data_ptr->multitap_26key_date_ptr->last_multitap_press_index,
                                                data_ptr->multitap_26key_date_ptr->last_multitap_char_index);

                            commit_data.commit_len = 1;
                            CommitPackData(data_ptr, &commit_data, TRUE);

                            data_ptr->tp_last_msg_param.idx = -1;
                            data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
                            data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;

                            SetToNoInput(data_ptr);
                        }
                        else
                        {
                            //无输入 这种情况加个保护
                            return FALSE;
                        }
                    }
                }
                break;
            }

            data_ptr->is_kb_dirty = TRUE;
            data_ptr->tp_last_msg_param.idx = -1;
            MMIIMUI_ReDrawAllBoard(data_ptr);
        }
        break;

        //处理候选区up
    case MMIIM_TP_CSTAR_MSG_TP_CAND:
        {
            TpCandPressUp(data_ptr, para_ptr->point, data_ptr->event_data_ptr);

            SetToNoInput(data_ptr);
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
    uint16 iLoop = 0;
    uint16 jLoop = 0;
    uint16 pos = 0;
    GUI_FONT_T font = {0};
    uint16 word_width = 0;
    uint16 label_width = 0;
    GUI_RECT_T text_rect = {0};                         //文本区区域
    uint16 MMIIM_TP_CSTAR_CAND_UP_LEFT_X = 0;
    MMIIM_TP_CSTAR_TEXT_BAR_THEME_T* text_bar_theme = MMIIMUI_GetTextBarThemeIfo();
    BOOLEAN  is_commit = FALSE;
    uint8    direction = 0;

    //SCI_ASSERT(PNULL != handle_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != handle_ptr->lcd_info_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if(PNULL == handle_ptr || PNULL == handle_ptr->lcd_info_ptr||PNULL == event_data_ptr)
    {
        return;
    }
    
    MMIIM_TP_CSTAR_CAND_UP_LEFT_X = MMITHEME_IsMainScreenLandscape() ? MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H : MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V;

    text_rect.left = handle_ptr->cand_rect.left;
    text_rect.right = MMIIM_TP_CSTAR_CAND_UP_LEFT_X - 1;
    text_rect.top = handle_ptr->cand_rect.top;
    text_rect.bottom = handle_ptr->cand_rect.bottom;

    direction = MMIIM_TPGetAligntype(handle_ptr->init_param.method_param_ptr->im_set.method);

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
                        NULL;   /*lint !e522*/

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
                        CommitChar(handle_ptr, handle_ptr->result[pos]);

                        DealAfterCommit(handle_ptr);

                        ClearData(handle_ptr);
                        
                        is_commit = TRUE;

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
                if(!is_commit)
                {
                    CommitChar(handle_ptr, handle_ptr->result[0]);

                    DealAfterCommit(handle_ptr);

                    ClearData(handle_ptr);
                }
            }
        }
    }
    else if (GUI_PointIsInRect(tp_point, handle_ptr->up_rect))
    {
#if defined MMIIM_CORE_USE_CSTAR
        if (!(handle_ptr->pageflag&MMIIM_TP_CSTAR_PAGEFLAG_PREVIOUS))
        {
            //...
        }
#endif
    }
    else if (GUI_PointIsInRect(tp_point, handle_ptr->down_rect))
    {
#if defined MMIIM_CORE_USE_CSTAR
        if (!(handle_ptr->pageflag&MMIIM_TP_CSTAR_PAGEFLAG_NEXT))
        {
            //...
        }
#endif
    }

    return;
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
        return;
    }
    
    MMIIMUI_HideKeyTips(data_ptr);
    
    StopContextTimer(data_ptr);

    if (0 != data_ptr->tips_timer_id)
    {
        MMK_StopTimer(data_ptr->tips_timer_id);
        data_ptr->tips_timer_id = 0;
    }

    data_ptr->ui_state = MMIIM_TP_CSTAR_STATE_NOINPUT;
    data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
    data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;
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

    SCI_MEMSET(data_ptr->result, 0, CAND_MAX_NUM*(sizeof(wchar)));
    data_ptr->result_len = 0;
    
    //set the redraw signal
    data_ptr->is_rect_change = TRUE;
    data_ptr->is_bar_cand_dirty = TRUE;

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

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/
    if(PNULL == data_ptr ||PNULL == data_ptr->init_param.method_param_ptr||PNULL == data_ptr->init_param.method_param_ptr->im_def_ptr)
    {
        return  FALSE;
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

    result = MMIIM_SP_ML_SetMethod(&data_ptr->multitap_26key_date_ptr->language_data, method);

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
Description: 
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ResetData(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in]
)
{
    if (-1 != data_ptr->tp_last_msg_param.idx)
    {
        if (0 != data_ptr->tips_timer_id)
        {
            MMK_StopTimer(data_ptr->tips_timer_id);
            data_ptr->tips_timer_id = 0;
        }

        data_ptr->multitap_26key_date_ptr->last_multitap_press_index = -1;
        data_ptr->multitap_26key_date_ptr->last_multitap_char_index = -1;
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
    
    data_ptr->multitap_26key_date_ptr->key_timer_id = 
        MMK_CreateTimerCallback(MMIIM_MULTITAP_26KEY_CANDIDATE_TIMER_OUT, ProcessContextTimerCB, (uint32)data_ptr, FALSE);                                    
    
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
    //SCI_ASSERT(PNULL != data_ptr->multitap_26key_date_ptr); /*assert verified*/
    if(PNULL == data_ptr || PNULL == data_ptr->multitap_26key_date_ptr)
    {
        return;
    }
    
    if (0 != data_ptr->multitap_26key_date_ptr->key_timer_id)
    {
        MMK_StopTimer(data_ptr->multitap_26key_date_ptr->key_timer_id);
        data_ptr->multitap_26key_date_ptr->key_timer_id = 0;
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

    if (PNULL == data_ptr || PNULL == data_ptr->multitap_26key_date_ptr)
    {
        return;
    }

    if (0 == timer_id || timer_id != data_ptr->multitap_26key_date_ptr->key_timer_id)
    {
        return;
    }
    
    if (-1 == data_ptr->multitap_26key_date_ptr->last_multitap_press_index)
    {
        return;
    }

    commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                            &data_ptr->multitap_26key_date_ptr->language_data, 
                            data_ptr->multitap_26key_date_ptr->last_multitap_press_index, 
                            data_ptr->multitap_26key_date_ptr->last_multitap_char_index);
    commit_data.commit_len = 1;

    notify_pack.data.commit_data = commit_data;
    notify_pack.notify = GUIIM_NOTIFY_COMMIT;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    if (GUIIM_MODE_LEAD_UPPER == data_ptr->multitap_26key_date_ptr->language_data.method_cfg_ptr->capital)
    {
        GUIIM_METHOD_T new_method = MMITHEME_IMGetMethodByCapital(
                                        data_ptr->multitap_26key_date_ptr->language_data.method_cfg_ptr->method,
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

        notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
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
    if (GUIIM_MODE_LEAD_UPPER == data_ptr->multitap_26key_date_ptr->language_data.method_cfg_ptr->capital)
    {
        new_method = MMITHEME_IMGetMethodByCapital(data_ptr->multitap_26key_date_ptr->language_data.method_cfg_ptr->method, GUIIM_MODE_LOWER);

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
Function Name: HandleMultitapCaps
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMultitapCaps(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
    )
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_CAPITAL_MODE_T cur_caps = GUIIM_MODE_MAX;
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;
    
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
    
    ResetData(data_ptr);
    
    //get method in using
    cur_method = data_ptr->init_param.method_param_ptr->im_set.method;
    cur_caps = data_ptr->init_param.method_param_ptr->im_set.capital;
    
    //filtrate the method which is not care about capital
    if (GUIIM_MODE_MAX <= cur_caps)
        return FALSE;
    
    //get the next method
    next_method = MMIIMUI_GetNextCapsByMethod(cur_method, cur_caps);
    
    if (next_method == cur_method)
        return FALSE;
    
    //发切换输入法索引通告输入法控件
    notify_pack.data.set_method = next_method;
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}

/*==============================================================================
Function Name: HandleMultitap26To9Key
Author: haiwu.chen
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMultitap26To9Key(
    MMIIM_TP_CSTAR_HANDLE_T *data_ptr  // [in] handle of method
    )
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    GUIIM_METHOD_T next_method = GUIIM_M_NONE;
    
    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/
    if(PNULL == data_ptr ||PNULL == data_ptr->init_param.method_param_ptr)
    {
        return  FALSE;
    }
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
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}

#else

GUIIM_KERNEL_APIS_T const g_guiim_tp_multitap_26key_apis = {0};

#endif //TOUCH_PANEL_SUPPORT
#endif
