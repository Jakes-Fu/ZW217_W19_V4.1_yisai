/****************************************************************************
** File Name:      mmk_kbd.c                                               *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the key message and       *
**                  function of operated  key                              *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
**
****************************************************************************/
#define MMK_KBD_C

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmi_kernel_trc.h"
#include "mmk_kbd.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "tb_dal.h"
#include "mmi_default.h"
#include "mmk_tp.h"
#include "mmk_msg_internal.h"
#include "mmk_kbd_internal.h"
#include "mmi_theme.h"
#include "mmi_autotest.h"
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif

//#include "Mmisrvaud_api.h"

/**-----------------------------------------------------------------------------*
 **                         MACRO Declaration                                   *
 **----------------------------------------------------------------------------*/
//按键信息的结构
typedef struct
{
    BOOLEAN		is_valid;			//whether this structure is valid
    uint8		timer_id;			//key long down timer id
    BOOLEAN		is_timer_active;	//whether timer is active
    uint8		key_code;			//key code saved
    BOOLEAN		is_long_press;		//whether enter long press status
    MMI_HANDLE_T ctrl_handle;       //keep ctrl handle
} MMK_KEY_DOWN_BAK_T;

typedef enum
{
    KBD_DBLCLK_NONE,
    KBD_DBLCLK_1_DOWN,    
    KBD_DBLCLK_1_UP,
    KBD_DBLCLK_2_DOWN,
    
    KBD_DBLCLK_MAX
}MMK_KEY_DBLCLK_STATE_E;

//所有按键信息的结构
typedef struct
{
    MMK_KEY_DOWN_BAK_T kbd_down_bak[MMI_MAX_MULTI_KEYS]; //同时按键信息的数组
    MMI_HANDLE_T       cur_handle;                       //当前控件句柄
    uint8              cur_key_code;                     //当前的键值
    uint8              cur_repeat_index;                 //当前repeat的索引
    uint8		       repeat_timer_id;                  //重复按键定时器的id, 最后按下的键才有重复按键消息
    BOOLEAN            is_repeat;                        //是否重复按键

	BOOLEAN			   is_need_clk_notify;				 //是否需要clk和dblclk notify
	uint8              dblclk_timer_id;
    uint8              first_key_code;                   //第一次down键值
    MMK_KEY_DBLCLK_STATE_E dblclk_state;
} MMK_KEY_DOWN_BAK_INFO_T;

/**-------------------------------------------------------------------------*
 **                         Global Variables                                *
 **------------------------------------------------------------------------*/
LOCAL MMK_KEY_QUEUE_INFO    s_waiting_key_info = {0};//消息队列中的Key info,最多保持4对

/*the global variable to keep the key code*/
LOCAL MMK_KEY_DOWN_BAK_INFO_T s_kbd_down_bak_info = {0};

LOCAL LCD_ANGLE_E s_direct_key_angle[4] = {0};

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : convert key code by logic angle
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void ConvertKeyCodeByLogicAngle(
    BOOLEAN   is_pressed,     //in:
    uint8     *key_code_ptr   //in/out:
);

/*****************************************************************************/
// 	Description : get multi key and tp param
//	Global resource dependence :
//  Author: bruce.chi
//	Note:	do not include cur_key_code
/*****************************************************************************/
LOCAL void GetMultiKeyTPParam(MMI_MULTI_KEY_TP_MSG_PARAM_T *key_tp_msg_ptr, uint8 cur_key_code);

/*****************************************************************************/
// 	Description : save key long down status
//	Global resource dependence :
//  Author: bruce.chi
//	Return : key code
//	Note:
/*****************************************************************************/
LOCAL uint8 SaveKeyLongDownStatus(uint8 timer_id);

/*****************************************************************************/
// 	Description : save key repeat status
//	Global resource dependence :
//  Author: bruce.chi
//	Return : key code
//	Note:
/*****************************************************************************/
LOCAL uint8 SaveKeyRepeatStatus(void);

/*****************************************************************************/
// 	Description : save key code and start long down timer
//	Global resource dependence :
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveKeyDownStatus(uint8 key_code);

/*****************************************************************************/
// 	Description : save key up status
//	Global resource dependence :
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveKeyUpStatus(uint8 key_code, BOOLEAN *is_long_press_ptr);

/*****************************************************************************/
//  Description : check if handle key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckHandleKey(
    uint8      key_code,   //in:
    uint16     single_code //in:down or up
);

/*****************************************************************************/
// 	Description : check IsPressSensitive (return false if can press down without up)
//
//	Global resource dependence :
//  Author: jibin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPressSensitive(uint8 key_code);

/*****************************************************************************/
// 	Description : check key msg number, kpd if can send to app
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckKeyMsgNumber(
    uint16  single_code,    //[IN] key type, KPD_DOWN or KPD_UP
    uint32  signal_keycode  //[IN] key code
);

/*****************************************************************************/
// 	Description : reduce the handle key num
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void ReduceKeyMsgNumber(void);

/*****************************************************************************/
// 	Description : convert key msg to key code
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint8 ConvertKeyMsgToKeyCode(
    MMI_MESSAGE_ID_E msg_id
);

/*****************************************************************************/
// 	Description : handle msg kbd
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMSGKbd(
    uint32  keys_status,
    uint32  key_code
)
{
    BOOLEAN pre_result = FALSE;
    BOOLEAN win_result = FALSE;
    BOOLEAN fix_result = FALSE;
    MMI_MULTI_KEY_TP_MSG_PARAM_T multi_key_tp_param = {0};

    //call the default function to handle key message
    pre_result = MMK_DefaultMSGKbd(keys_status , key_code);
    SCI_TRACE_LOW("HandleMSGKbd status:%d, code:%d",keys_status, key_code);
    //SCI_TRACE_LOW:"HandleMSGKbd result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_174_112_2_18_3_22_38_0,(uint8*)"d", pre_result);
    //被预处理了，返回
    if (!pre_result) return TRUE;

    if ((KEY_PRESSED == keys_status) &&
            ((KEY_OK == key_code) || (KEY_WEB == key_code) || (KEY_UP == key_code) || (KEY_DOWN == key_code) || (KEY_LEFT == key_code) || (KEY_RIGHT == key_code)))
    {
        //set key mode
        MMITHEME_SetTiggerMode(MMITHEME_TRIGGER_MODE_KEY);
    }

    //Dispatch the key message to focus window
    GetMultiKeyTPParam(&multi_key_tp_param, key_code);
    win_result = MMK_DispMsgToWin(keys_status + key_code, &multi_key_tp_param);
    //SCI_TRACE_LOW:"HandleMSGKbd recode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_188_112_2_18_3_22_38_1,(uint8*)"d", win_result);

#ifdef MCARE_V31_SUPPORT
    {
        if(MCareV31_GetKeyDownSlide())
        {
            MCareV31_SetKeyDownSlide(FALSE);
            win_result = FALSE;
        }
    }
#endif

    //被窗口处理了，返回
    if (win_result) return TRUE;

    fix_result = MMK_DefaultMSGKbdLater(keys_status, key_code);
    //被后处理了，返回
    if (!fix_result) return TRUE;

    //公共处理
    MMK_HandlePublicKey(keys_status + key_code, &multi_key_tp_param);

#ifndef WIN32
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
#ifdef BT_HID_SUPPORT
    MMIBT_HandleBTKey(keys_status, key_code);
#endif
#endif
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : convert key code by logic angle
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void ConvertKeyCodeByLogicAngle(
    BOOLEAN   is_pressed,     //in:
    uint8     *key_code_ptr   //in/out:
)
{
    BOOLEAN     result = FALSE;
    uint32      i = 0;
    uint32      key_code = 0;
    LCD_ANGLE_E cur_angle = LCD_ANGLE_0;

    //is direct key
    switch (*key_code_ptr)
    {
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
        i = *key_code_ptr - KEY_UP;
        cur_angle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
        result = TRUE;
        break;

    default:
        result = FALSE;
        break;
    }

    if (result)
    {
        key_code = *key_code_ptr;
        if (is_pressed)
        {
            GUILCD_ConvertKeyCode(&key_code, GUI_MAIN_LCD_ID, cur_angle);
            s_direct_key_angle[i] = cur_angle;
        }
        else
        {
            if (cur_angle != s_direct_key_angle[i])
            {
                cur_angle = s_direct_key_angle[i];
            }
            GUILCD_ConvertKeyCode(&key_code, GUI_MAIN_LCD_ID, cur_angle);
        }
        *key_code_ptr = (uint8)key_code;
    }
}

/*****************************************************************************/
// 	Description : window需要同时关注单击和双击事件时调用
//	Global resource dependence :
//  Author: Cheng.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMK_NeedKbdClkNotifyOnce(void)
{
    s_kbd_down_bak_info.is_need_clk_notify = TRUE;
}

/*******************************************************************/
//  Interface:		MMK_IsNeedKbClkNotify
//  Description : 	MMK_IsNeedKbClkNotify
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMK_IsNeedKbClkNotify(void)
{
    return s_kbd_down_bak_info.is_need_clk_notify;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author: Cheng.luo
//	Note:
/*****************************************************************************/
LOCAL void ResetDblclkTimer()
{
    MMK_StopTimer( s_kbd_down_bak_info.dblclk_timer_id );
    MMK_StartWinTimer(MMK_GetFirstAppletHandle(),
        s_kbd_down_bak_info.dblclk_timer_id,
        MILLISECONDS_TO_TICKS(MMI_KDB_DBLCLK_PRESS_VALUE),
        FALSE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author: Cheng.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ReInitKbdDblclkState(BOOLEAN is_notify)
{
    if (is_notify 
        && KEY_NONE != s_kbd_down_bak_info.first_key_code)
    {
        MMISRVAUD_HandleEarphoneEvent(MMISRVAUD_EARPHONE_EVENT_CLK, s_kbd_down_bak_info.first_key_code);
        MMK_DispMsgToWin(MSG_KEY_CLICK, (void*)&s_kbd_down_bak_info.first_key_code);
    }
    
    s_kbd_down_bak_info.is_need_clk_notify = FALSE;
    s_kbd_down_bak_info.dblclk_timer_id = MMI_KBD_DBCLK_TIMER_ID;
    s_kbd_down_bak_info.dblclk_state    = KBD_DBLCLK_NONE;
    s_kbd_down_bak_info.first_key_code  = 0;
}

/*****************************************************************************/
// 	Description : dispatch the key message after translate the signal of RTOS to
//                MMI Message
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
void MMK_DispatchMSGKbd(
    MmiKeyPress* signal_ptr // the pointer of message which need dispatch
)
{
    MmiKeyPress*    keypress_ptr = PNULL;
    uint32          key_status      = 0;
    uint8           key_code        = KEY_NONE;
    uint32          signal_keycode  = KEY_NONE;
    BOOLEAN			is_long_press = FALSE;


    SCI_ASSERT(PNULL != signal_ptr);   /*assert verified*/

    keypress_ptr = (MmiKeyPress*) signal_ptr;
    signal_keycode = keypress_ptr->keyCode ;

    key_code = MMIDEFAULT_ConvertKeyCode(signal_keycode);

    //cr237201,MMI task
    //tp down，tp task callback函数调用MMK_CheckKeySingle()发现没有key消息,因此创建了single,排入mmi对列。
    //red key down, key task callback函数调用MMK_CheckTPSignale()发现没有消息,因此创建了single,排入mmi对列。
    //MMI收到red key up消息，进行处理，一键灭屏，进入deep sleep状态。
    //tp task发现deep sleep状态就不会再发送任何消息了,其中也包括那个tp up消息！
    //MMI task，对key，tp是否能够处理再判断一次
    if (!CheckHandleKey(key_code, signal_ptr->SignalCode))
    {
        return;
    }

    //W307 用户自定义功能键复用KEY_DOWNSIDEKEY,需要处理的键值消息转换为KEY_USER_FUNC
#ifdef ADULT_WATCH_SUPPORT
    if (key_code == KEY_DOWNSIDEKEY)
    {
        key_code = KEY_USER_FUNC;
    }
#endif
    SCI_TRACE_LOW("MMK_DispatchMSGKbd: signalCode=%d, signal_keycode=%d, key_code=0x%x.",signal_ptr->SignalCode, signal_keycode, key_code);

    MMI_TraceAutoTestOfKeyPress(signal_ptr->SignalCode, signal_keycode);

    switch (signal_ptr->SignalCode)
    {
    case KPD_DOWN:
        //convert key by lcd logic angle
        ConvertKeyCodeByLogicAngle(TRUE, &key_code);

        if (IsPressSensitive(key_code))
        {
            if (!SaveKeyDownStatus(key_code))
            {
                return;
            }
        }

        HandleMSGKbd(KEY_PRESSED, key_code);

        if (MMK_IsNeedKbClkNotify())
        {
            //SCI_TRACE_LOW:"@KPD_DOWN kbd_dblclk_state = %d.\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_361_112_2_18_3_22_39_3,(uint8*)"d", s_kbd_down_bak_info.dblclk_state);
            if (KBD_DBLCLK_NONE == s_kbd_down_bak_info.dblclk_state)
            {
                s_kbd_down_bak_info.dblclk_state = KBD_DBLCLK_1_DOWN;
                s_kbd_down_bak_info.first_key_code = key_code;

                ResetDblclkTimer();
            }
            else if(KBD_DBLCLK_1_UP == s_kbd_down_bak_info.dblclk_state)
            {
                s_kbd_down_bak_info.dblclk_state = KBD_DBLCLK_2_DOWN;
                MMK_StopTimer( s_kbd_down_bak_info.dblclk_timer_id );

                if (key_code == s_kbd_down_bak_info.first_key_code)   // 和上次相等
                {
                    MMISRVAUD_HandleEarphoneEvent(MMISRVAUD_EARPHONE_EVENT_DBLCLK, key_code);
                    MMK_DispMsgToWin(MSG_KEY_DBLCLK, (void*)&key_code);
                }
                else
                {   
                    MMISRVAUD_HandleEarphoneEvent(MMISRVAUD_EARPHONE_EVENT_CLK, key_code);
                    MMK_DispMsgToWin(MSG_KEY_CLICK, (void*)&s_kbd_down_bak_info.first_key_code);
                }
            }        
        }

        break;

    case KPD_UP:
        //convert key by lcd logic angle
        ConvertKeyCodeByLogicAngle(FALSE, &key_code);

        //kbd_timer
        if (IsPressSensitive(key_code))
        {
            // 对组按键消息的管理不包括Handset和Flip
            ReduceKeyMsgNumber();

            if (!SaveKeyUpStatus(key_code, &is_long_press))
            {
                return;
            }
            if (!is_long_press)
            {
                key_status = KEY_RELEASED;
            }
            else
            {
                key_status = KEY_LONG_RELEASED;
            }
        }
        else
        {
            key_status = KEY_RELEASED;
        }

        HandleMSGKbd(key_status, key_code);

        if (MMK_IsNeedKbClkNotify())
        {
            //SCI_TRACE_LOW:"@KPD_UP kbd_dblclk_state = %d.\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_419_112_2_18_3_22_39_4,(uint8*)"d", s_kbd_down_bak_info.dblclk_state);
            if (KBD_DBLCLK_1_DOWN == s_kbd_down_bak_info.dblclk_state)
            {
                s_kbd_down_bak_info.dblclk_state = KBD_DBLCLK_1_UP;
            }
            else if (KBD_DBLCLK_2_DOWN == s_kbd_down_bak_info.dblclk_state)
            {
                MMK_ReInitKbdDblclkState(FALSE);
            }
        }

        break;

    default:
        //SCI_TRACE_LOW:"MMK_DispatchMSGKbd signalCode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_433_112_2_18_3_22_39_5,(uint8*)"d", &signal_ptr->SignalCode);
        break;
    }

}


/******************************************************************************/
/*! \brief			handle key long press timer
 *  \param[in]		timer_id		key long press timer id
 *  \author  bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_HandleKeyLongPressTimer(uint8 timer_id)
{
    uint8	key_code = 0;

    key_code = SaveKeyLongDownStatus(timer_id);
    if (key_code != KEY_NONE)
    {
        if (MMK_IsNeedKbClkNotify())
        {
            MMK_StopTimer( s_kbd_down_bak_info.dblclk_timer_id );
            MMK_ReInitKbdDblclkState(FALSE);
        }
        HandleMSGKbd(KEY_LONG_PRESSED, key_code);
    }
}

/******************************************************************************/
/*! \brief			handle key repeat timer
 *  \param[in]		timer_id		key long press timer id
 *  \author  bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_HandleKeyRepeatPressTimer(void)
{
    uint8	key_code = 0;

    key_code = SaveKeyRepeatStatus();

    if (key_code != KEY_NONE)
    {
        if (MMK_IsNeedKbClkNotify())
        {
            MMK_StopTimer( s_kbd_down_bak_info.dblclk_timer_id );
            MMK_ReInitKbdDblclkState(FALSE);
        }
        
        //start repeat key timer
        MMK_StartWinTimer(
            MMK_GetFirstAppletHandle(),
            s_kbd_down_bak_info.repeat_timer_id,
            MILLISECONDS_TO_TICKS(MMI_KBD_REPEAT_KEY_VALUE),
            FALSE);
        
        HandleMSGKbd(KEY_REPEATED, key_code);
    }
}

/******************************************************************************/
/*! \brief			stop repeat timer
 *  \param[in]		timer_id		key long press timer id
 *  \author  bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_StopKeyRepeatPressTimer(void)
{
    if (s_kbd_down_bak_info.is_repeat)
    {
        MMK_StopTimer(s_kbd_down_bak_info.repeat_timer_id);

        s_kbd_down_bak_info.is_repeat = FALSE;
    }

}

/*****************************************************************************/
// 	Description : check the key single because of the system queue full,
//                call by KeypadCallBackFunc
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckKeySingle(
    uint16 single_code, //[IN] key type, KPD_DOWN or KPD_UP
    uint32 signal_keycode     //[IN] key code
)
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_handle = FALSE;
    uint8	            key_code = KEY_NONE;

    key_code = MMIDEFAULT_ConvertKeyCode(signal_keycode);

    //is handle key
    is_handle = CheckHandleKey(key_code, single_code);
    if (is_handle)
    {
        // Flip和Handset不记录在num中
        if (!IsPressSensitive(key_code))
        {
            result = TRUE;
        }
        else
        {
            //检察按键消息，最多只能保存3对
            result = CheckKeyMsgNumber(single_code, signal_keycode);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : check if handle key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckHandleKey(
    uint8      key_code,   //in:
    uint16     single_code //in:down or up
)
{
    BOOLEAN             result = FALSE;
    GUI_POINT_T         point = {0};
    MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;

#ifdef TOUCH_PANEL_SUPPORT
    //有tp down消息时,只屏蔽key down消息
    MMK_GetMultiTPMsg(&tp_status, &point);
#endif

    if (MMI_TP_NONE == tp_status)
    {
        result = TRUE;
    }
    else
    {
        if (KPD_DOWN == single_code)
        {
            if (!IsPressSensitive(key_code))
            {
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"CheckHandleKey: has tp down,not handle key down msg!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_569_112_2_18_3_22_39_6,(uint8*)"");
            }
        }
        else
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : check IsPressSensitive (return false if can press down without up)
//
//	Global resource dependence :
//  Author: jibin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPressSensitive(uint8 key_code)
{
    if ((KEY_FLIP           == key_code)
            /*|| (KEY_HEADSET_BUTTIN == key_code)*/
            || (KEY_SDCARD_DETECT == key_code)
            || (KEY_HEADSET_DETECT == key_code)
            || (KEY_SLIDE == key_code)
#ifdef MAINLCD_SIZE_128X64
            || (KEY_HOOK == key_code)
#endif
       )
    {
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : save key code and start long down timer
//	Global resource dependence :
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveKeyDownStatus(uint8 key_code)
{
    int i = 0;

    //reset current info
    s_kbd_down_bak_info.cur_handle = 0;
    s_kbd_down_bak_info.cur_key_code = 0;

    //check whether key_code is valid
    for (i = 0; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if (s_kbd_down_bak_info.kbd_down_bak[i].is_valid && s_kbd_down_bak_info.kbd_down_bak[i].key_code == key_code)
        {
            //down msg of this key_code already exists in queue
            return FALSE;
        }
    }

    for (i = 0; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if (!s_kbd_down_bak_info.kbd_down_bak[i].is_valid)
        {
            s_kbd_down_bak_info.kbd_down_bak[i].is_valid = TRUE;
            s_kbd_down_bak_info.kbd_down_bak[i].key_code = key_code;

            //start the key timer
            MMK_StartWinTimer(
                MMK_GetFirstAppletHandle(),
                s_kbd_down_bak_info.kbd_down_bak[i].timer_id,
                MILLISECONDS_TO_TICKS(MMI_KBD_LONG_KEY_VALUE),
                FALSE);

            s_kbd_down_bak_info.kbd_down_bak[i].is_timer_active = TRUE;
            s_kbd_down_bak_info.kbd_down_bak[i].is_long_press = FALSE;

            //start repeat key timer
            MMK_StartWinTimer(
                MMK_GetFirstAppletHandle(),
                s_kbd_down_bak_info.repeat_timer_id,
                MILLISECONDS_TO_TICKS(MMI_KBD_START_REPEAT_KEY_VALUE),
                FALSE);
            s_kbd_down_bak_info.is_repeat        = TRUE;
            s_kbd_down_bak_info.cur_repeat_index = i;

            return TRUE;
        }
    }

//	SCI_TRACE_LOW("mmk_kbd.c:SaveKeyDownStatus s_kbd_down_bak_info.kbd_down_bak all is valid");
    return FALSE;
}

/*****************************************************************************/
// 	Description : save key up status
//	Global resource dependence :
//  Author: bruce.chi
//	Return : whether key long down timer is active
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveKeyUpStatus(uint8 key_code, BOOLEAN *is_long_press_ptr)
{
    int			i = 0;
    BOOLEAN     result = FALSE;

    //reset current info
    s_kbd_down_bak_info.cur_handle = 0;
    s_kbd_down_bak_info.cur_key_code = 0;

    SCI_ASSERT(PNULL != is_long_press_ptr);   /*assert verified*/
    *is_long_press_ptr = FALSE;

    for (i = 0; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if (s_kbd_down_bak_info.kbd_down_bak[i].is_valid
                && s_kbd_down_bak_info.kbd_down_bak[i].key_code == key_code)
        {
            if (s_kbd_down_bak_info.is_repeat
                    && i == s_kbd_down_bak_info.cur_repeat_index)
            {
                MMK_StopTimer(s_kbd_down_bak_info.repeat_timer_id);
                s_kbd_down_bak_info.is_repeat = FALSE;
            }

            if (s_kbd_down_bak_info.kbd_down_bak[i].is_timer_active)
            {
                MMK_StopTimer(s_kbd_down_bak_info.kbd_down_bak[i].timer_id);
                s_kbd_down_bak_info.kbd_down_bak[i].is_timer_active = FALSE;
            }

            *is_long_press_ptr = s_kbd_down_bak_info.kbd_down_bak[i].is_long_press;

            //keep current info
            s_kbd_down_bak_info.cur_handle = s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle;
            s_kbd_down_bak_info.cur_key_code = s_kbd_down_bak_info.kbd_down_bak[i].key_code;

            s_kbd_down_bak_info.kbd_down_bak[i].key_code = KEY_NONE;
            s_kbd_down_bak_info.kbd_down_bak[i].is_valid = FALSE;
            s_kbd_down_bak_info.kbd_down_bak[i].is_long_press = FALSE;
            s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle = 0;

            result = TRUE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : save key long down status
//	Global resource dependence :
//  Author: bruce.chi
//	Return : key code
//	Note:
/*****************************************************************************/
LOCAL uint8 SaveKeyLongDownStatus(uint8 timer_id)
{
    uint8 key_code = KEY_NONE;
    int i = 0;

    //reset current info
    s_kbd_down_bak_info.cur_handle = 0;
    s_kbd_down_bak_info.cur_key_code = 0;

    for (i = 0; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if (s_kbd_down_bak_info.kbd_down_bak[i].is_valid
                && s_kbd_down_bak_info.kbd_down_bak[i].timer_id == timer_id)
        {
            if (s_kbd_down_bak_info.kbd_down_bak[i].is_timer_active)
            {
                MMK_StopTimer(s_kbd_down_bak_info.kbd_down_bak[i].timer_id);
                s_kbd_down_bak_info.kbd_down_bak[i].is_timer_active = FALSE;
                s_kbd_down_bak_info.kbd_down_bak[i].is_long_press = TRUE;
            }

            key_code = s_kbd_down_bak_info.kbd_down_bak[i].key_code;

            //keep current info
            s_kbd_down_bak_info.cur_handle = s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle;
            s_kbd_down_bak_info.cur_key_code = key_code;

            break;
        }
    }

    return key_code;
}

/*****************************************************************************/
// 	Description : save key repeat status
//	Global resource dependence :
//  Author: bruce.chi
//	Return : key code
//	Note:
/*****************************************************************************/
LOCAL uint8 SaveKeyRepeatStatus(void)
{
    uint8 key_code = KEY_NONE;
    uint8 i        = s_kbd_down_bak_info.cur_repeat_index;

    //reset current info
    s_kbd_down_bak_info.cur_handle = 0;
    s_kbd_down_bak_info.cur_key_code = 0;

    if (s_kbd_down_bak_info.is_repeat
            && i < MMI_MAX_MULTI_KEYS
            && s_kbd_down_bak_info.kbd_down_bak[i].is_valid)
    {
        key_code = s_kbd_down_bak_info.kbd_down_bak[i].key_code;

        //keep current info
        s_kbd_down_bak_info.cur_handle = s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle;
        s_kbd_down_bak_info.cur_key_code = key_code;
    }

    return key_code;
}

/******************************************************************************/
/*! \brief			Init mmk keypad globle
 *  \author			bruce.chi
 */
/******************************************************************************/
PUBLIC void MMK_InitKBD(void)
{
    uint32  i = 0;

    //init waiting key info
    s_waiting_key_info.waiting_keys_num = 0;
    s_waiting_key_info.keycode_num = 0;
    for (i = 0; i < MMI_MAX_WAITING_KEYS; i++)
    {
        s_waiting_key_info.waiting_keycode[i] = KEY_NONE;
    }

    s_kbd_down_bak_info.kbd_down_bak[0].timer_id = MMI_KBD_TIMER_ID_1;
    s_kbd_down_bak_info.kbd_down_bak[1].timer_id = MMI_KBD_TIMER_ID_2;
    s_kbd_down_bak_info.kbd_down_bak[2].timer_id = MMI_KBD_TIMER_ID_3;

    s_kbd_down_bak_info.repeat_timer_id = MMI_KBD_REPEAT_TIMER_ID;

    MMK_ReInitKbdDblclkState(FALSE);
}


/******************************************************************************/
/*! \brief get multi keypad message
 *  \param[in]		multi_key_msg	multi key message array
 *  \author  bruce.chi
 *  \return
 *		- <i><c>message number</c></i>
 */
/******************************************************************************/
PUBLIC int32 MMK_GetMultiKeyMsg(MMI_MESSAGE_ID_E multi_key_msg[MMI_MAX_MULTI_KEYS], uint8 cur_key_code)
{
    int					i = 0;
    int					num = 0;
    MMI_MESSAGE_ID_E	msg = 0;

    SCI_ASSERT(multi_key_msg != PNULL); /*assert verified*/

    for (i = 0; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if ((s_kbd_down_bak_info.kbd_down_bak[i].is_valid) && (s_kbd_down_bak_info.kbd_down_bak[i].key_code != KEY_NONE) && (s_kbd_down_bak_info.kbd_down_bak[i].key_code != cur_key_code))
        {
            if (s_kbd_down_bak_info.kbd_down_bak[i].is_long_press)
            {
                msg = MSG_KEYDOWN + s_kbd_down_bak_info.kbd_down_bak[i].key_code;
            }
            else
            {
                msg = MSG_KEYLONG + s_kbd_down_bak_info.kbd_down_bak[i].key_code;
            }

            multi_key_msg[num] = msg;
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : convert key msg to key code
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint8 ConvertKeyMsgToKeyCode(
    MMI_MESSAGE_ID_E msg_id
)
{
    uint8        key_code    = 0;

    if (MSG_KEYDOWN <= msg_id && MSG_KEYDOWN_MAX_NUM >= msg_id)
    {
        key_code = (uint8)(msg_id - MSG_KEYDOWN);
    }
    else if (MSG_KEYUP <= msg_id && MSG_KEYUP_MAX_NUM >= msg_id)
    {
        key_code = (uint8)(msg_id - MSG_KEYUP);
    }
    else if (MSG_KEYREPEAT <= msg_id && MSG_KEYREPEAT_MAX_NUM >= msg_id)
    {
        key_code = (uint8)(msg_id - MSG_KEYREPEAT);
    }
    else if (MSG_KEYLONG <= msg_id && MSG_KEYLONG_MAX_NUM >= msg_id)
    {
        key_code = (uint8)(msg_id - MSG_KEYLONG);
    }
    else if (MSG_KEYPRESSUP <= msg_id && MSG_KEYPRESSUP_MAX_NUM >= msg_id)
    {
        key_code = (uint8)(msg_id - MSG_KEYPRESSUP);
    }

    return key_code;
}


/*****************************************************************************/
// 	Description : set key down ctrl
//	Global resource dependence :
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMK_SetKeyDownCtrl(
    MMI_HANDLE_T        ctrl_handle,
    MMI_MESSAGE_ID_E    msg_id
)
{
    BOOLEAN result   = FALSE;
    uint8   key_code = ConvertKeyMsgToKeyCode(msg_id);

    if (0 != key_code)
    {
        uint32 i = 0;

        for (; i < MMI_MAX_MULTI_KEYS; i++)
        {
            if (s_kbd_down_bak_info.kbd_down_bak[i].key_code == key_code)
            {
                s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle = ctrl_handle;
                result = TRUE;

                break;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : get key down ctrl
//	Global resource dependence :
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
MMI_HANDLE_T MMK_GetKeyDownCtrl(
    MMI_MESSAGE_ID_E msg_id
)
{
    MMI_HANDLE_T ctrl_handle = 0;

    if (ConvertKeyMsgToKeyCode(msg_id) == s_kbd_down_bak_info.cur_key_code)
    {
        ctrl_handle = s_kbd_down_bak_info.cur_handle;
    }

    return ctrl_handle;
}

/*****************************************************************************/
// 	Description : clear key down ctrl
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMK_ClearKeyDownCtrl(
    MMI_HANDLE_T        ctrl_handle
)
{
    BOOLEAN result   = FALSE;

    uint32 i = 0;

    for (; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if (s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle == ctrl_handle)
        {
            s_kbd_down_bak_info.kbd_down_bak[i].ctrl_handle = 0;
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : get multi key and tp param
//	Global resource dependence :
//  Author: bruce.chi
//	Note:	do not include cur_key_code
/*****************************************************************************/
LOCAL void GetMultiKeyTPParam(MMI_MULTI_KEY_TP_MSG_PARAM_T *key_tp_msg_ptr, uint8 cur_key_code)
{
    int32               cur_key_num = 0;
    MMI_MESSAGE_ID_E    multi_key_msg[MMI_MAX_MULTI_KEYS] = {0};
    MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;
    GUI_POINT_T         point = {0};

    SCI_ASSERT(key_tp_msg_ptr != PNULL); /*assert verified*/

    cur_key_num = MMK_GetMultiKeyMsg(multi_key_msg, cur_key_code);
#ifdef TOUCH_PANEL_SUPPORT
    MMK_GetMultiTPMsg(&tp_status, &point);
#endif

    key_tp_msg_ptr->cur_tp_point.x = 0;
    key_tp_msg_ptr->cur_tp_point.y = 0;
    key_tp_msg_ptr->multi_key_num = cur_key_num;
    SCI_MEMCPY(key_tp_msg_ptr->multi_key_msg, multi_key_msg, sizeof(multi_key_msg));
    key_tp_msg_ptr->multi_tp_status = tp_status;
    key_tp_msg_ptr->multi_tp_point = point;
}

/*****************************************************************************/
// 	Description : check key msg number, kpd if can send to app
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckKeyMsgNumber(
    uint16  single_code,    //[IN] key type, KPD_DOWN or KPD_UP
    uint32  signal_keycode  //[IN] key code
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_corresponding = FALSE;
    uint32      i = 0;
    uint32      j = 0;

    switch (single_code)
    {
    case KPD_DOWN:
        //是否已经有MMI_MAX_WAITING_KEYS对按键消息
        if (MMI_MAX_WAITING_KEYS >= s_waiting_key_info.waiting_keys_num)
        {
            //down消息，最多接收4个,是否已经有4个down消息
            if (MMI_MAX_WAITING_KEYS > s_waiting_key_info.keycode_num)
            {
                s_waiting_key_info.waiting_keycode[s_waiting_key_info.keycode_num] = signal_keycode;
                s_waiting_key_info.keycode_num++;
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"CheckKeyMsgNumber:key full, waiting_keys_num = %d,keycode_num=%d, %0x, %0x, %0x!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_KBD_1010_112_2_18_3_22_40_7,(uint8*)"ddddd", s_waiting_key_info.waiting_keys_num, s_waiting_key_info.keycode_num, s_waiting_key_info.waiting_keycode[0], s_waiting_key_info.waiting_keycode[1], s_waiting_key_info.waiting_keycode[2]);
        }
        break;

    case KPD_UP:
        //up消息，要成对出现,是否有对应的down消息
        for (i = 0; i < s_waiting_key_info.keycode_num; i++)
        {
            if (signal_keycode == s_waiting_key_info.waiting_keycode[i])
            {
                is_corresponding = TRUE;
                break;
            }
        }

        if (is_corresponding)
        {
            //clear key msg queue
            s_waiting_key_info.waiting_keycode[i] = KEY_NONE;
            for (j = i; j < (MMI_MAX_WAITING_KEYS - 1); j++)
            {
                s_waiting_key_info.waiting_keycode[j] = s_waiting_key_info.waiting_keycode[j+1];
            }
            s_waiting_key_info.waiting_keycode[MMI_MAX_WAITING_KEYS-1] = KEY_NONE;

            s_waiting_key_info.waiting_keys_num++;

            if (0 < s_waiting_key_info.keycode_num)
            {
                s_waiting_key_info.keycode_num--;
            }

            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : reduce the handle key num
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void ReduceKeyMsgNumber(void)
{
//    SCI_TRACE_LOW("ReduceKeyMsgNumber: waiting_keys_num = %d!", s_waiting_key_info.waiting_keys_num);

    if (0 < s_waiting_key_info.waiting_keys_num)
    {
        s_waiting_key_info.waiting_keys_num--;
    }
}

/*****************************************************************************/
// 	Description : is key msg
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsKeyMsg(
    MMI_MESSAGE_ID_E    msg_id
)
{
    BOOLEAN     result = FALSE;

    if (((MSG_KEYDOWN <= msg_id) && (MSG_KEYDOWN_MAX_NUM >= msg_id)) ||
            ((MSG_KEYUP <= msg_id) && (MSG_KEYUP_MAX_NUM >= msg_id)) ||
            ((MSG_KEYLONG <= msg_id) && (MSG_KEYLONG_MAX_NUM >= msg_id)) ||
            ((MSG_KEYREPEAT <= msg_id) && (MSG_KEYREPEAT_MAX_NUM >= msg_id)) ||
            ((MSG_KEYPRESSUP <= msg_id) && (MSG_KEYPRESSUP_MAX_NUM >= msg_id)))
    {
        result = TRUE;
    }

    return (result);
}

/******************************************************************************/
/*! \brief			quick key, key is nearly full
 *  \author			bin.ji
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsQuickKey(void)
{
    //有多个快速按键或触笔或repeat按键的情况下，认为是quick key，可禁止3D等特效
    if (s_waiting_key_info.waiting_keys_num > 1
#ifdef TOUCH_PANEL_SUPPORT
            || MMK_IsQuickTp()
#endif
            )
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : redispatch keypress
//	Global resource dependence : none
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_RedispatchKeypress(
    MMI_MESSAGE_ID_E    msg_id,     //the identify of message
    DPARAM              param_ptr   //the param of the message
)
{
    return MMK_DispMsgToWin(msg_id, param_ptr);
}

#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
/*****************************************************************************/
// 	Description : is shift key press
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsShiftKeyPress(void)
{
    BOOLEAN  result = FALSE;
    uint32   i = 0;

    for (i = 0; i < MMI_MAX_MULTI_KEYS; i++)
    {
        if (s_kbd_down_bak_info.kbd_down_bak[i].is_valid
                && MMIDEFAULT_IsShiftKey(s_kbd_down_bak_info.kbd_down_bak[i].key_code))
        {
            result = TRUE;
            break;
        }
    }

    return (result);
}

#endif

