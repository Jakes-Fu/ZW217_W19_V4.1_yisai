/*==============================================================================
File Name: mmiim_sp_multitap.c
Author: Lianxiang.Zhou
Date: 2010/06/03
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/03   Lianxiang.Zhou  Create.
==============================================================================*/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmk_msg.h"

#include "mmiim_sp_multitap.h"
#include "mmiim_sp_symbol.h"
#include "mmiim_im_switching.h"

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/
// 定义键索引个数。因为这是给物理键盘的，而物理键盘只可能有一种键盘个数
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
#define MMIIM_SP_KEY_INDEX_MAX MMIIM_SP_26KEY_INDEX_MAX
#else
#define MMIIM_SP_KEY_INDEX_MAX MMIIM_SP_9KEY_INDEX_MAX
#endif

/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/

/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);

/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(void *handle);

/*==============================================================================
Function Name:
Description:
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(
    void *handle,
    GUIIM_EVENT_T event,
    GUIIM_EVENT_DATA_U *event_data_ptr
);

/*==============================================================================
Function Name: HandleSysMsg
Description: HandleSysMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_SP_MT_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr
);

/*==============================================================================
Function Name: TransMsg
Description: TransMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_SP_MT_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
);

/*==============================================================================
Function Name: SetMethod
Description: SetMethod
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method);

/*==============================================================================
Function Name: ConvertKey
Description: ConvertKey
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_KEY_INDEX_T ConvertKey(MMIIM_SP_VKEY_T key);

/*==============================================================================
Function Name: CommitLastKey
Description: CommitLastKey
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN CommitLastKey(MMIIM_SP_MT_DATA_T *data_ptr);

/*==============================================================================
Function Name: ResetKeyBoard
Description: 重新设置键盘
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ResetKeyBoard(MMIIM_SP_MT_DATA_T *data_ptr);

/*==============================================================================
Function Name: DealAfterCommit
Description: 提交后的处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DealAfterCommit(
    MMIIM_SP_MT_DATA_T *data_ptr,
    wchar last_chr
);

/*==============================================================================
Function Name: CommitPackData
Description: 提交数据包
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitPackData(
    MMIIM_SP_MT_DATA_T *data_ptr,
    GUIIM_COMMIT_DATA_T *commit_data_ptr
);

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
GUIIM_KERNEL_APIS_T const g_guiim_sp_multitap_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*==============================================================================
Function Name: Initialize
Description: Initialize
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void* Initialize(
    GUIIM_INIT_KERNEL_T *param_ptr   // [in] param
)
{
    MMIIM_SP_MT_DATA_T *data_ptr = PNULL;
    BOOLEAN is_init_ok = FALSE;

    //SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    if (PNULL == param_ptr )
    {
        return PNULL;
    }
    data_ptr = (MMIIM_SP_MT_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_MT_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_SP_MT_DATA_T));

    //data
    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->init_param = *param_ptr;
    data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
    data_ptr->last_key_index = 0;

    SCI_MEMSET(&data_ptr->rect, 0, sizeof(GUI_RECT_T));

    // init language data
    MMIIM_SP_ML_Init(&data_ptr->language_data);

    is_init_ok = MMIIM_SP_ML_SetMethod(&data_ptr->language_data,
                                  param_ptr->method_param_ptr->im_def_ptr->method);

    if (!is_init_ok)
    {
        return PNULL;
    }

    return data_ptr;
}


/*==============================================================================
Function Name: Terminate
Description: Terminate
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN Terminate(
    void *handle    // [in] handle of input method
)
{
    MMIIM_SP_MT_DATA_T *data_ptr = (MMIIM_SP_MT_DATA_T *)handle;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr )
    {
        return FALSE;
    }
    
    MMIIM_SP_ML_Term(&data_ptr->language_data);

    SCI_FREE(data_ptr);

    return TRUE;
}


/*==============================================================================
Function Name: HandleMsg
Description: HandleMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleMsg(
    void *handle,                       // [in] handle of method
    GUIIM_EVENT_T event,                // [in] event type
    GUIIM_EVENT_DATA_U *event_data_ptr  // [in] event data
)
{
    MMIIM_SP_MT_DATA_T *data_ptr = (MMIIM_SP_MT_DATA_T *)handle;
    BOOLEAN result = FALSE;
    GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr = PNULL;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //某些消息传入的参数event_data_ptr 可能为空，不能直接返回,如GUIIM_EVENT_FORCE_COMMIT
    if (PNULL == data_ptr)
    {
        return FALSE;
    }
    
    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:
        //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
        result = HandleSysMsg(data_ptr, event_data_ptr);

        break;

    case GUIIM_EVENT_SET_METHOD:
        if (event_data_ptr != PNULL)
        {
            result = SetMethod(handle, event_data_ptr->set_method);
        }
        
        break;

    case GUIIM_EVENT_GET_METHOD:
        if (event_data_ptr != PNULL)
        {
            event_data_ptr->get_method = data_ptr->init_param.method_param_ptr->im_def_ptr->method;
            result = TRUE;
        }

        break;

    case GUIIM_EVENT_FORCE_COMMIT:
    {
        wchar last_chr = MMIIM_SP_ML_GetChar(&data_ptr->language_data,
                                             data_ptr->last_key,
                                             data_ptr->last_key_index);

        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
        data_ptr->last_key_index = 0;

        DealAfterCommit(data_ptr, last_chr);

        result = TRUE;
        
        break;
    }
        
    case GUIIM_EVENT_SLEEP:
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
        data_ptr->last_key_index = 0;
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_CTRL_RECT:
    case GUIIM_EVENT_GET_RECT:
        if (event_data_ptr != PNULL)        
        {
            event_data_ptr->get_rect = data_ptr->rect;
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:
        result = FALSE;
        break;

    case GUIIM_EVENT_CUSTOM_KEY:
        if (event_data_ptr != PNULL)      
        {
            key_info_ptr = event_data_ptr->custom_key_info_ptr;
            result = MMIIM_SP_ML_SetCustomKey(&data_ptr->language_data, key_info_ptr);
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
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleSysMsg(
    MMIIM_SP_MT_DATA_T *data_ptr,        // [in] handle of method
    GUIIM_EVENT_DATA_U *event_data_ptr   // [in] event data
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    MMIIM_SP_KEY_INDEX_T cur_key = MMIIM_SP_KEY_INDEX_MAX;
    BOOLEAN is_last_key = FALSE;
    int32 commit_index = 0;
    GUIIM_COMMIT_DATA_T commit_data = {0};
    uint16 char_count = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUI_BOTH_RECT_T pop_menu_rect = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        return FALSE;
    }
    
    //特别处理屏幕转动的消息 - 即不做任何处理
    if (MSG_LCD_SWITCH == event_data_ptr->sys_msg.msg_id)
    {
        return TRUE;
    }
    
    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
        data_ptr->last_key_index = 0;

        return FALSE;
    }

    //特别处理定时器的情况
    //1 定时器触发
    //2 其他按键的发生
    if (MSG_TIMER == event_data_ptr->sys_msg.msg_id
            && MMIIM_EXPL_TIMER_ID == *(uint8*)(event_data_ptr->sys_msg.param))
    {
        return CommitLastKey(data_ptr);
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        return FALSE;
    }

    cur_key = ConvertKey(msg.para.key);
    is_last_key = (cur_key == data_ptr->last_key && cur_key < MMIIM_SP_KEY_INDEX_MAX);

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        //无论如何先停下定时器
        MMIIM_StopTimer(&(data_ptr->is_timer_active));

        //首先处理功能键
        //再处理有效输入按键
        //如果星号和井号没有被处理，则弹出符号和切换输入法

        if (MMIIM_SP_KEY_UP == msg.para.key
                || MMIIM_SP_KEY_DOWN == msg.para.key
                || MMIIM_SP_KEY_LEFT == msg.para.key
                || MMIIM_SP_KEY_RIGHT == msg.para.key
                || MMIIM_SP_KEY_OK == msg.para.key
                || MMIIM_SP_KEY_CANCEL == msg.para.key)
        {
            data_ptr->last_char = '\0';
            ResetKeyBoard(data_ptr);

            is_dealt = CommitLastKey(data_ptr);

            is_dealt = 2*TRUE;
            
            break;
        }
        else  if (is_last_key)
        {
            char_count = MMIIM_SP_ML_GetCount(&data_ptr->language_data, data_ptr->last_key);

            //重复按键
            data_ptr->last_key_index += 1;

            if (data_ptr->last_key_index > char_count)
            {
                data_ptr->last_key_index = 1;
            }

            commit_data.underline[0] = MMIIM_SP_ML_GetChar(
                                           &data_ptr->language_data,
                                           data_ptr->last_key, data_ptr->last_key_index);
            commit_data.underline_len = 1;

            CommitPackData(data_ptr, &commit_data);
        }
        else if (cur_key < MMIIM_SP_KEY_INDEX_MAX)
        {
            commit_index = 0;

            if (data_ptr->last_key < MMIIM_SP_KEY_INDEX_MAX)
            {
                CommitLastKey(data_ptr);
            }

            char_count = MMIIM_SP_ML_GetCount(&data_ptr->language_data, cur_key);

            //按新键
            if (1 == char_count)
            {
                //只一个
                commit_data.commit[commit_index] = MMIIM_SP_ML_GetChar(
                                                       &data_ptr->language_data, cur_key, 1);
                commit_index++;
                commit_data.commit_len = commit_index;
                commit_data.underline_len = 0;
                data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
                data_ptr->last_key_index = 0;

                CommitPackData(data_ptr, &commit_data);
            }
            else if (char_count > 1)
            {
                //多个
                data_ptr->last_key = cur_key;
                data_ptr->last_key_index = 1;

                commit_data.commit_len = commit_index;

                commit_data.underline[0] = MMIIM_SP_ML_GetChar(
                                               &data_ptr->language_data,
                                               data_ptr->last_key, data_ptr->last_key_index);

                commit_data.underline_len = 1;

                CommitPackData(data_ptr, &commit_data);
            }

#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
            else
            {
                switch (cur_key)
                {
                case MMIIM_SP_26KEY_INDEX_0:
                case MMIIM_SP_26KEY_INDEX_1:
                case MMIIM_SP_26KEY_INDEX_2:
                case MMIIM_SP_26KEY_INDEX_3:
                case MMIIM_SP_26KEY_INDEX_4:
                case MMIIM_SP_26KEY_INDEX_5:
                case MMIIM_SP_26KEY_INDEX_6:
                case MMIIM_SP_26KEY_INDEX_7:
                case MMIIM_SP_26KEY_INDEX_8:
                case MMIIM_SP_26KEY_INDEX_9:
                    commit_data.commit[0] = '0' + cur_key - MMIIM_SP_26KEY_INDEX_0;
                    break;
                case MMIIM_SP_26KEY_INDEX_STAR:
                    commit_data.commit[0] = '*';
                    break;
                case MMIIM_SP_26KEY_INDEX_HASH:
                    commit_data.commit[0] = '#';
                    break;
                default:
                    is_dealt = FALSE;
                    break;
                }

                if (is_dealt)
                {
                    commit_data.commit_len = 1;
                    CommitPackData(data_ptr, &commit_data);
                }
            }

#else
            else
            {
                //无输入
                is_dealt = FALSE;
            }

#endif
        }
        else
        {
            is_dealt = FALSE;
        }

        if (!is_dealt)
        {
            if (MMIIM_SP_KEY_STAR == msg.para.key && (PNULL == data_ptr->language_data.custom_kb_ptr))
            {
#ifndef MAINLCD_SIZE_128X64 //dphone屏幕较小，不能显示
                if (GUIIM_M_123 == data_ptr->init_param.method_param_ptr->im_set.method)
                {
                    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                        GUIIM_LANG_ENGLISH);
                }
                else
                {
                    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang);
                }
                is_dealt = TRUE;
#endif
            }
            else if (MMIIM_SP_KEY_HASH == msg.para.key && (PNULL == data_ptr->language_data.custom_kb_ptr))
            {
                MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
                MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_TOP_LEFT);
                is_dealt = TRUE;
            }

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //全键盘是通过ctrl键切换输入法
            else if (MMIIM_SP_KEY_CTRL == msg.para.key && (PNULL == data_ptr->language_data.custom_kb_ptr))
            {
                MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
                MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_TOP_LEFT);
                is_dealt = TRUE;
            }
            else if (MMIIM_SP_KEY_FN == msg.para.key && (PNULL == data_ptr->language_data.custom_kb_ptr))
            {
                if (GUIIM_M_Q_123 == data_ptr->init_param.method_param_ptr->im_set.method)
                {
                    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                        GUIIM_LANG_ENGLISH);
                }
                else
                {
                    MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang);
                }
                is_dealt = TRUE;
            }

#endif
        }

        break;

    case MMIIM_SP_MSG_KEY_RELEASE:

        if (is_last_key)
        {
            MMIIM_StartTimer(&(data_ptr->is_timer_active), data_ptr->ctrl_handle);
            is_dealt = TRUE;
        }

        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:

        if (MMIIM_SP_KEY_CANCEL == msg.para.key || MMIIM_SP_KEY_DEL == msg.para.key)
        {
            data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
            data_ptr->last_key_index = 0;

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

            is_dealt = TRUE;
        }
		else if (MMIIM_SP_KEY_STAR == msg.para.key
				&& GUIIM_LANG_BENGALI == data_ptr->init_param.method_param_ptr->im_def_ptr->lang
				&& PNULL == data_ptr->language_data.custom_kb_ptr)
		{
			//need delete the current underline char
			data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
            data_ptr->last_key_index = 0;
			
            notify_pack.notify = GUIIM_NOTIFY_DELETE;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

			MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
			MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang);
			
            is_dealt = TRUE;
			break;
		}
        else if (cur_key < MMIIM_SP_KEY_INDEX_MAX && is_last_key)
        {
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
            int32 last_index = MMIIM_SP_ML_GetCount(&data_ptr->language_data, cur_key);

            //如果当前的字母仅有一个候选字，则不处理
            if (1 != last_index)
            {
                //最后一个字符
                commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                            &data_ptr->language_data,
                                            data_ptr->last_key, last_index);

                commit_data.commit_len = 1;

                CommitPackData(data_ptr, &commit_data);

                data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
                data_ptr->last_key_index = 0;

                is_dealt = TRUE;
            }
#else
            is_dealt = TRUE;

            if (PNULL != data_ptr->init_param.method_param_ptr
                && PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr
                && GUIIM_LANG_BENGALI == data_ptr->init_param.method_param_ptr->im_def_ptr->lang
                && MMIIM_SP_9KEY_INDEX_HASH == cur_key)
            {
				//need delete the current underline char
				data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
				data_ptr->last_key_index = 0;
				
				notify_pack.notify = GUIIM_NOTIFY_DELETE;
				GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

                MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
                MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_TOP_LEFT);
                
                break;
            }

            switch (cur_key)
            {
            case MMIIM_SP_9KEY_INDEX_0:
                commit_data.commit[0] = '0';
                break;
            case MMIIM_SP_9KEY_INDEX_1:
                commit_data.commit[0] = '1';
                break;
            case MMIIM_SP_9KEY_INDEX_2:
                commit_data.commit[0] = '2';
                break;
            case MMIIM_SP_9KEY_INDEX_3:
                commit_data.commit[0] = '3';
                break;
            case MMIIM_SP_9KEY_INDEX_4:
                commit_data.commit[0] = '4';
                break;
            case MMIIM_SP_9KEY_INDEX_5:
                commit_data.commit[0] = '5';
                break;
            case MMIIM_SP_9KEY_INDEX_6:
                commit_data.commit[0] = '6';
                break;
            case MMIIM_SP_9KEY_INDEX_7:
                commit_data.commit[0] = '7';
                break;
            case MMIIM_SP_9KEY_INDEX_8:
                commit_data.commit[0] = '8';
                break;
            case MMIIM_SP_9KEY_INDEX_9:
                commit_data.commit[0] = '9';
                break;
            default:
                is_dealt = FALSE;
                break;
            }

            if (is_dealt)
            {
                commit_data.commit_len = 1;
                CommitPackData(data_ptr, &commit_data);
                data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
                data_ptr->last_key_index = 0;
            }
#endif
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
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TransMsg(
    MMIIM_SP_MT_DATA_T *data_ptr,               // [in] handle of method
    GUIIM_EVENT_DATA_U const *event_data_ptr,   // [in] event data
    MMIIM_SP_MSG_T *msg_ptr                     // [out] tranlated msg
)
{
    MMIIM_SP_TP_AREA_INFO_T tp_area_info = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if (PNULL == data_ptr || PNULL == event_data_ptr|| PNULL == msg_ptr )
    {
        return;
    }
    MMIIM_SP_TransMsg(event_data_ptr, &tp_area_info, msg_ptr);

    return;
}

/*==============================================================================
Function Name: SetMethod
Description: SetMethod
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN SetMethod(
    void *handle,           // [in] handle of method
    GUIIM_METHOD_T method   // [in] method type
)
{
    BOOLEAN result = TRUE;
    MMIIM_SP_MT_DATA_T  *data_ptr = (MMIIM_SP_MT_DATA_T *)handle;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return FALSE;
    }
    
    MMIIM_SP_ML_Term(&data_ptr->language_data);

    result = MMIIM_SP_ML_SetMethod(&data_ptr->language_data, method);

    if (result)
    {
        //重置参数
        data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
        data_ptr->last_key_index = 0;
    }

    return result;
}

/*==============================================================================
Function Name: ConvertKey
Description: ConvertKey
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMIIM_SP_KEY_INDEX_T ConvertKey(
    MMIIM_SP_VKEY_T key // [in] key value
)
{
    MMIIM_SP_KEY_INDEX_T vkey = MMIIM_SP_KEY_INDEX_MAX;

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD

    switch (key)
    {
    case MMIIM_SP_KEY_A:
        vkey = MMIIM_SP_26KEY_INDEX_A;
        break;
    case MMIIM_SP_KEY_B:
        vkey = MMIIM_SP_26KEY_INDEX_B;
        break;
    case MMIIM_SP_KEY_C:
        vkey = MMIIM_SP_26KEY_INDEX_C;
        break;
    case MMIIM_SP_KEY_D:
        vkey = MMIIM_SP_26KEY_INDEX_D;
        break;
    case MMIIM_SP_KEY_E:
        vkey = MMIIM_SP_26KEY_INDEX_E;
        break;
    case MMIIM_SP_KEY_F:
        vkey = MMIIM_SP_26KEY_INDEX_F;
        break;
    case MMIIM_SP_KEY_G:
        vkey = MMIIM_SP_26KEY_INDEX_G;
        break;
    case MMIIM_SP_KEY_H:
        vkey = MMIIM_SP_26KEY_INDEX_H;
        break;
    case MMIIM_SP_KEY_I:
        vkey = MMIIM_SP_26KEY_INDEX_I;
        break;
    case MMIIM_SP_KEY_J:
        vkey = MMIIM_SP_26KEY_INDEX_J;
        break;
    case MMIIM_SP_KEY_K:
        vkey = MMIIM_SP_26KEY_INDEX_K;
        break;
    case MMIIM_SP_KEY_L:
        vkey = MMIIM_SP_26KEY_INDEX_L;
        break;
    case MMIIM_SP_KEY_M:
        vkey = MMIIM_SP_26KEY_INDEX_M;
        break;
    case MMIIM_SP_KEY_N:
        vkey = MMIIM_SP_26KEY_INDEX_N;
        break;
    case MMIIM_SP_KEY_O:
        vkey = MMIIM_SP_26KEY_INDEX_O;
        break;
    case MMIIM_SP_KEY_P:
        vkey = MMIIM_SP_26KEY_INDEX_P;
        break;
    case MMIIM_SP_KEY_Q:
        vkey = MMIIM_SP_26KEY_INDEX_Q;
        break;
    case MMIIM_SP_KEY_R:
        vkey = MMIIM_SP_26KEY_INDEX_R;
        break;
    case MMIIM_SP_KEY_S:
        vkey = MMIIM_SP_26KEY_INDEX_S;
        break;
    case MMIIM_SP_KEY_T:
        vkey = MMIIM_SP_26KEY_INDEX_T;
        break;
    case MMIIM_SP_KEY_U:
        vkey = MMIIM_SP_26KEY_INDEX_U;
        break;
    case MMIIM_SP_KEY_V:
        vkey = MMIIM_SP_26KEY_INDEX_V;
        break;
    case MMIIM_SP_KEY_W:
        vkey = MMIIM_SP_26KEY_INDEX_W;
        break;
    case MMIIM_SP_KEY_X:
        vkey = MMIIM_SP_26KEY_INDEX_X;
        break;
    case MMIIM_SP_KEY_Y:
        vkey = MMIIM_SP_26KEY_INDEX_Y;
        break;
    case MMIIM_SP_KEY_Z:
        vkey = MMIIM_SP_26KEY_INDEX_Z;
        break;

    case MMIIM_SP_KEY_DEL:
        vkey = MMIIM_SP_26KEY_INDEX_DEL;
        break;
    case MMIIM_SP_KEY_COMMA:
        vkey = MMIIM_SP_26KEY_INDEX_COMMA;
        break;
    case MMIIM_SP_KEY_PERIOD:
        vkey = MMIIM_SP_26KEY_INDEX_PERIOD;
        break;
    case MMIIM_SP_KEY_ENTER:
        vkey = MMIIM_SP_26KEY_INDEX_ENTER;
        break;
    case MMIIM_SP_KEY_FN:
        vkey = MMIIM_SP_26KEY_INDEX_FN;
        break;
    case MMIIM_SP_KEY_SHIFT:
        vkey = MMIIM_SP_26KEY_INDEX_SHIFT;
        break;
    case MMIIM_SP_KEY_AT_QWERTY:
        vkey = MMIIM_SP_26KEY_INDEX_AT_QWERTY;
        break;
    case MMIIM_SP_KEY_SPACE:
        vkey = MMIIM_SP_26KEY_INDEX_SPACE;
        break;
    case MMIIM_SP_KEY_AND:
        vkey = MMIIM_SP_26KEY_INDEX_AND;
        break;
    case MMIIM_SP_KEY_QUESTION:
        vkey = MMIIM_SP_26KEY_INDEX_QUESTION;
        break;
    case MMIIM_SP_KEY_CTRL:
        vkey = MMIIM_SP_26KEY_INDEX_CTRL;
        break;

    case MMIIM_SP_KEY_PLUS:
        vkey = MMIIM_SP_26KEY_INDEX_PLUS;
        break;
    case MMIIM_SP_KEY_LEFT_PARENTHESIS:
        vkey = MMIIM_SP_26KEY_INDEX_LEFT_PARENTHESIS;
        break;
    case MMIIM_SP_KEY_RIGHT_PARENTHESIS:
        vkey = MMIIM_SP_26KEY_INDEX_RIGHT_PARENTHESIS;
        break;
    case MMIIM_SP_KEY_MINUS:
        vkey = MMIIM_SP_26KEY_INDEX_MINUS;
        break;
    case MMIIM_SP_KEY_DOUBLE_QUOTES:
        vkey = MMIIM_SP_26KEY_INDEX_DOUBLE_QUOTES;
        break;
    case MMIIM_SP_KEY_SEMICOLON:
        vkey = MMIIM_SP_26KEY_INDEX_SEMICOLON;
        break;
    case MMIIM_SP_KEY_COLON:
        vkey = MMIIM_SP_26KEY_INDEX_COLON;
        break;
    case MMIIM_SP_KEY_SLASH:
        vkey = MMIIM_SP_26KEY_INDEX_SLASH;
        break;
    case MMIIM_SP_KEY_SHIFT_AND:
        vkey = MMIIM_SP_26KEY_INDEX_SHIFT_AND;
        break;
    case MMIIM_SP_KEY_EXCLAMATION:
        vkey = MMIIM_SP_26KEY_INDEX_EXCLAMATION;
        break;
    //MMK不支持的按键
    case MMIIM_SP_KEY_QUOTES:
        vkey = MMIIM_SP_26KEY_INDEX_QUOTES;
        break;
    case MMIIM_SP_KEY_EQUAL:
        vkey = MMIIM_SP_26KEY_INDEX_EQUAL;
        break;
    case MMIIM_SP_KEY_RIGHT_SLASH:
        vkey = MMIIM_SP_26KEY_INDEX_RIGHT_SLASH;
        break;
    
    case MMIIM_SP_KEY_0:
        vkey = MMIIM_SP_26KEY_INDEX_0;
        break;
    case MMIIM_SP_KEY_1:
        vkey = MMIIM_SP_26KEY_INDEX_1;
        break;
    case MMIIM_SP_KEY_2:
        vkey = MMIIM_SP_26KEY_INDEX_2;
        break;
    case MMIIM_SP_KEY_3:
        vkey = MMIIM_SP_26KEY_INDEX_3;
        break;
    case MMIIM_SP_KEY_4:
        vkey = MMIIM_SP_26KEY_INDEX_4;
        break;
    case MMIIM_SP_KEY_5:
        vkey = MMIIM_SP_26KEY_INDEX_5;
        break;
    case MMIIM_SP_KEY_6:
        vkey = MMIIM_SP_26KEY_INDEX_6;
        break;
    case MMIIM_SP_KEY_7:
        vkey = MMIIM_SP_26KEY_INDEX_7;
        break;
    case MMIIM_SP_KEY_8:
        vkey = MMIIM_SP_26KEY_INDEX_8;
        break;
    case MMIIM_SP_KEY_9:
        vkey = MMIIM_SP_26KEY_INDEX_9;
        break;
    case MMIIM_SP_KEY_STAR:
        vkey = MMIIM_SP_26KEY_INDEX_STAR;
        break;
    case MMIIM_SP_KEY_HASH:
        vkey = MMIIM_SP_26KEY_INDEX_HASH;
        break;

    default:
        break;
    }

#else

    switch (key)
    {
    case MMIIM_SP_KEY_0:
        vkey = MMIIM_SP_9KEY_INDEX_0;
        break;
    case MMIIM_SP_KEY_1:
        vkey = MMIIM_SP_9KEY_INDEX_1;
        break;
    case MMIIM_SP_KEY_2:
        vkey = MMIIM_SP_9KEY_INDEX_2;
        break;
    case MMIIM_SP_KEY_3:
        vkey = MMIIM_SP_9KEY_INDEX_3;
        break;
    case MMIIM_SP_KEY_4:
        vkey = MMIIM_SP_9KEY_INDEX_4;
        break;
    case MMIIM_SP_KEY_5:
        vkey = MMIIM_SP_9KEY_INDEX_5;
        break;
    case MMIIM_SP_KEY_6:
        vkey = MMIIM_SP_9KEY_INDEX_6;
        break;
    case MMIIM_SP_KEY_7:
        vkey = MMIIM_SP_9KEY_INDEX_7;
        break;
    case MMIIM_SP_KEY_8:
        vkey = MMIIM_SP_9KEY_INDEX_8;
        break;
    case MMIIM_SP_KEY_9:
        vkey = MMIIM_SP_9KEY_INDEX_9;
        break;
    case MMIIM_SP_KEY_STAR:
        vkey = MMIIM_SP_9KEY_INDEX_STAR;
        break;
    case MMIIM_SP_KEY_HASH:
        vkey = MMIIM_SP_9KEY_INDEX_HASH;
        break;

    default:
        break;
    }

#endif

    return vkey;
}

/*==============================================================================
Function Name: CommitLastKey
Description: 提交最后一个按键的字符
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN CommitLastKey(
    MMIIM_SP_MT_DATA_T *data_ptr    // handle of method
)
{
    GUIIM_COMMIT_DATA_T commit_data = {0};

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return FALSE;
    }
    
    if (data_ptr->last_key >= MMIIM_SP_KEY_INDEX_MAX)
    {
        return FALSE;
    }

    commit_data.commit[0] = MMIIM_SP_ML_GetChar(
                                &data_ptr->language_data,
                                data_ptr->last_key, data_ptr->last_key_index);

    commit_data.commit_len = 1;

    CommitPackData(data_ptr, &commit_data);

    data_ptr->last_key = MMIIM_SP_KEY_INDEX_MAX;
    data_ptr->last_key_index = 0;

    return TRUE;
}

/*==============================================================================
Function Name: ResetKeyBoard
Description: 重新设置键盘
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 一些语言的键盘设定由前一个输入的字符决定
==============================================================================*/
LOCAL void ResetKeyBoard(
    MMIIM_SP_MT_DATA_T *data_ptr    // [in] handle of method
)
{
    wchar prev_char = 0;
    BOOLEAN need_change_kb = FALSE;
    CHECK_TABLE_FUNC_T checking_func_ptr = PNULL;

    //SCI_ASSERT(PNULL != data_ptr); /*assert verified*/
    if (PNULL == data_ptr)
    {
        return;
    }
    
    need_change_kb = MMIIM_SP_ML_CheckKeyTable(&data_ptr->language_data, &checking_func_ptr);

    if (need_change_kb)
    {
        if (PNULL != data_ptr->init_param.f_get_context)
        {
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // 这一段代码需要配合输入法控件调试
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            MMI_HANDLE_T edit_ctrl_handle = MMK_GetParentCtrlHandle(data_ptr->init_param.im_handle);
            GUIIM_CONTEXT_T im_context = {0};

            (*(data_ptr->init_param.f_get_context))(edit_ctrl_handle, &im_context);
            prev_char = im_context.str_ptr[im_context.pos];
        }
        else
        {
            prev_char = data_ptr->last_char;
        }

        data_ptr->language_data.key_table_ptr = (*checking_func_ptr)(prev_char);
        
    }

    return;
}


/*==============================================================================
Function Name: DealAfterCommit
Description: 提交后的处理
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 一些准备工作
==============================================================================*/
LOCAL void DealAfterCommit(
    MMIIM_SP_MT_DATA_T *data_ptr,           // [in] handle of method
    wchar last_chr
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if ('\0' != last_chr)
    {
        data_ptr->last_char = last_chr;
        ResetKeyBoard(data_ptr);
    }

    return;
}


/*==============================================================================
Function Name: CommitPackData
Description: 提交数据包
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitPackData(
    MMIIM_SP_MT_DATA_T *data_ptr,         // [in] handle of method
    GUIIM_COMMIT_DATA_T *commit_data_ptr  // [in] commit data
)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    BOOLEAN is_commit_one = FALSE;

    is_commit_one = (1 == commit_data_ptr->commit_len);

    //优先处理提交的操作
    if (is_commit_one && MMIIM_SP_KEY_OP_SYMBOL_PANEL == commit_data_ptr->commit[0])
    {
        if (GUIIM_M_123 == data_ptr->init_param.method_param_ptr->im_set.method
            || GUIIM_M_Q_123 == data_ptr->init_param.method_param_ptr->im_set.method)
        {
            MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                GUIIM_LANG_ENGLISH);
        }
        else
        {
            MMIIM_SP_SymbCreateWinByLang(data_ptr->init_param.im_handle,
                data_ptr->init_param.method_param_ptr->im_set.lang);
        }

        commit_data_ptr->commit[0] = 0;
        commit_data_ptr->commit_len = 0;
    }
    else if (is_commit_one && MMIIM_SP_KEY_OP_SWITCH_MENU == commit_data_ptr->commit[0])
    {
        GUI_BOTH_RECT_T pop_menu_rect = {0};
        
        MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
        MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_TOP_LEFT);

        commit_data_ptr->commit[0] = 0;
        commit_data_ptr->commit_len = 0;
    }

    if (commit_data_ptr->commit_len > 0 || commit_data_ptr->underline_len > 0)
    {

       if(GUIIM_MODE_LEAD_UPPER == data_ptr->language_data.method_cfg_ptr->capital)
        {
            MMI_HANDLE_T    parent_handle = 0;
            wchar           cursor_pre_char = 0;
            BOOLEAN         prechar_success = FALSE;
            BOOLEAN         is_leadup_char = FALSE;
 
            SCI_ASSERT(MMI_HANDLE_CONTROL == MMK_GetHandleType(data_ptr->init_param.im_handle)); /*assert verified*/

            parent_handle = MMK_GetParentCtrlHandle(data_ptr->init_param.im_handle);
            
            if (0 == parent_handle)
            {
                parent_handle = MMK_GetWinHandleByCtrl(data_ptr->init_param.im_handle);
            }

            if ((commit_data_ptr->commit[0]))
            {
            	is_leadup_char = MMIIM_IsPreCurrentWordSeperateChar(parent_handle);
            	if(!is_leadup_char)
            	{
            		commit_data_ptr->commit[0] = MMIAPICOM_Wchartolower(commit_data_ptr->commit[0]);                
            	}
            }
            if ((commit_data_ptr->underline[0]))
            {
            	is_leadup_char = MMIIM_IsPreUnderlineWordSeperateChar(parent_handle);
            	if(!is_leadup_char)
            	{
            		commit_data_ptr->underline[0] = MMIAPICOM_Wchartolower(commit_data_ptr->underline[0]);
            	}
            }
			
        }

        notify_pack.notify = GUIIM_NOTIFY_COMMIT;
        notify_pack.data.commit_data = *commit_data_ptr;

        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

        DealAfterCommit(data_ptr, commit_data_ptr->commit[0]);
    }

    return;
}



