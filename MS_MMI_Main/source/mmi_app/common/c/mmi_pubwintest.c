#ifdef WIN32
/*****************************************************************************
** File Name:      mmi_pubwintest.c                                          *
** Author:                                                                   *
** Date:           06/17/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011      James.Zhang           Creat                                  *
******************************************************************************/
#define _MMI_PUBWIN_TEST_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmibt_text.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "mmi_id.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "window_parse.h"
#include "mmimp3_id.h"
#include "mmidisplay_data.h"
#include "mmimp3_menutable.h"
#include "mmimp3_text.h"
#include "mmimp3_anim.h"
#include "mmialarm_export.h"
#include "mmibt_func.h"
#include "mmi_appmsg.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmiset_id.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"

#define TEST_ITEM_MAX 10

LOCAL MMI_STRING_T s_prompt_str_array[TEST_ITEM_MAX] = {0};

/*****************************************************************************/
//  Description : handle wait window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id,
                                    DPARAM            param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    //MMI_TEXT_ID_T   label = 0;

    switch(msg_id)
    {
    case MSG_APP_LEFT: 
//         label = STXT_ADD;
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+1);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+2);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+3);
        MMIPUB_SetWaitWinText( TRUE, 1, s_prompt_str_array,s_prompt_str_array + 1,s_prompt_str_array + 2, win_id );
        break;
        
    case MSG_APP_RIGHT: 
//         label = TXT_BT_HELP_VISIBLE;
//         MMI_GetLabelTextByLang(label--, s_prompt_str_array);
//         MMI_GetLabelTextByLang(label--, s_prompt_str_array+1);
//         MMI_GetLabelTextByLang(label--, s_prompt_str_array+2);
//         MMI_GetLabelTextByLang(label--, s_prompt_str_array+3);
        MMIPUB_SetWaitWinText( TRUE, 3, s_prompt_str_array,s_prompt_str_array + 1,s_prompt_str_array + 2, win_id );
        break;

    case MSG_APP_UP:
//         label = TXT_CC_HOLDING;
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+1);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+2);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+3);
        MMIPUB_SetWaitWinText( TRUE, 3, s_prompt_str_array,s_prompt_str_array + 1,s_prompt_str_array + 2, win_id );
        break;
    case MSG_APP_DOWN:
//         label = TXT_KL_PSW_EMERGENCY_CALL;
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+1);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+2);
//         MMI_GetLabelTextByLang(label++, s_prompt_str_array+3);
        MMIPUB_SetWaitWinText( TRUE, 2, s_prompt_str_array,s_prompt_str_array + 1,s_prompt_str_array + 2, win_id );
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle wait window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePrgWinMsg(
                                   MMI_WIN_ID_T      win_id, 
                                   MMI_MESSAGE_ID_E  msg_id,
                                   DPARAM            param
                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
//    MMI_STRING_T    s_prompt_str_array[4] = {0};
 //   MMI_TEXT_ID_T   label = 0;
    static uint32 i = 50;

    switch(msg_id)
    {
    case MSG_APP_LEFT:
        i--;
        MMIPUB_UpdateProgressBar(PNULL, i);
        break;
        
    case MSG_APP_RIGHT: 
        i++;
        MMIPUB_UpdateProgressBar(PNULL, i);
        break;
        
    case MSG_APP_UP:
        i--;
        MMIPUB_UpdateProgressBar(PNULL, i);
        break;
        
    case MSG_APP_DOWN:
        i++;
        MMIPUB_UpdateProgressBar(PNULL, i);
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle alarm ring set window message
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3AlarmRingSetWinMsg(
                                               MMI_WIN_ID_T       win_id,     
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    uint16          cur_selection = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMP3_ALARM_RING_SET_LIST_CTRL_ID;
	uint16 i=0;
	MMI_STRING_T str_ptr={0};
    wchar	str_content[MMIACC_EVENT_TEXT_MAX_LEN+1] = {0};	

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, ALM_ALARM_NUM, FALSE );
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
		for(i=0;i<ALM_ALARM_NUM;i++)
		{
			str_ptr.wstr_ptr = str_content;
			str_ptr.wstr_len= MMIACC_EVENT_TEXT_MAX_LEN;
			MMIAPIALM_GetAlarmName(i,&str_ptr);

			MMIAPISET_AppendListItemByTextBuffer(str_ptr.wstr_ptr,str_ptr.wstr_len,0,TXT_COMMON_OK,TXT_NULL, STXT_RETURN,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
		}
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        //SetMp3AsAlarmRing(cur_selection);
        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Discription: Handle blue tooth edit device name win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditDeviceNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    uint16                   device_name[BT_DEVICE_NAME_SIZE] = {0};
    uint16                   ucs_new_name_len = 0;
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_STRING_T             new_name = {0};
    wchar                    ucs_new_name[BT_DEVICE_NAME_SIZE] = {0};
    MMI_CTRL_ID_T            ctrl_id = MMIBT_EDIT_DEVICE_NAME_EDITBOX_CTRL_ID;
    BT_STATUS                return_value = BT_SUCCESS;
    uint32                   is_local_name = FALSE;
    #ifdef BLUETOOTH_SUPPORT
    extern BT_DEVICE_INFO     g_current_bt;
    #endif

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_TEXT_ID_T text_id = TXT_NULL;
            MMI_STRING_T  title = {0};

			GUIEDIT_SetTextMaxLen(ctrl_id, 100, 100);
            is_local_name = (uint32)MMK_GetWinAddDataPtr(win_id);
            SCI_MEMSET(device_name,0x00,(BT_DEVICE_NAME_SIZE*sizeof(uint16))); 
            if(is_local_name)
            {
               text_id = TXT_COMMON_NAME;
               BT_GetLocalName(device_name);
               GUIEDIT_SetString(ctrl_id, device_name,MMIAPICOM_Wstrlen((wchar*)device_name) );       

            }
            else
            {
                text_id = TXT_BT_DEVICE_RENAME;
                #ifdef BLUETOOTH_SUPPORT
                GUIEDIT_SetString(ctrl_id, g_current_bt.name,MMIAPICOM_Wstrlen((wchar*)g_current_bt.name) );      
                #endif
            }
			GUIEDIT_SetSoftkey(ctrl_id,	0, 1, TXT_NULL,	TXT_COMMON_OK, PNULL);
            MMI_GetLabelTextByLang(text_id, &title);
            GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);

            GUILABEL_SetText(MMIBT_BT_SEND_FILE_TEXT_CTRL_ID, s_prompt_str_array, FALSE);
        }
        break;
        
//     case MSG_FULL_PAINT:
//         ClrClientRect();
//         break;
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        is_local_name = (uint32)MMK_GetWinAddDataPtr(win_id);
        SCI_MEMSET(&new_name,0,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(ctrl_id,&new_name);
        ucs_new_name_len = MIN(MMIBT_DEVICE_NAME_ALPHA_MAX_LEN, new_name.wstr_len);
        MMIAPICOM_Wstrncpy( ucs_new_name, new_name.wstr_ptr, new_name.wstr_len);
        ucs_new_name[new_name.wstr_len] = 0;
        
        if ( 0 < new_name.wstr_len  && new_name.wstr_len <= MMIBT_DEVICE_NAME_ALPHA_MAX_LEN)
        {
            if(is_local_name)
            {
                return_value = BT_SetLocalName(ucs_new_name);
            }
            else
            {
            		#ifdef BLUETOOTH_SUPPORT
                return_value = BT_SetPairedDeviceName( &g_current_bt.addr,(uint16*)ucs_new_name);
                #endif
            }
            if (BT_SUCCESS == return_value)
            {
                if(is_local_name)
                {
                     MMIPUB_OpenAlertSuccessWin(TXT_DEVICE_NAME_FINISHED);
                }
                else
                {
                     MMIPUB_OpenAlertSuccessWin(TXT_BT_RENAME_FINISHED);
                }
// #ifdef MMI_PDA_SUPPORT
//                 AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_DEVICE_NAME,IMAGE_COMMON_POP_MENU_RIGHT,new_name,BT_INDEX_NAME,TRUE);
// #endif
                MMK_CloseWin(win_id);
                if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
                {   
                    MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
                    MMK_CloseWin(MMIBT_DEVICE_DETAILS_WIN_ID);
                } 
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }                   
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_DC_NAMEINVALID);
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : 自定义分钟提示音窗口
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetMinuteReminderEditWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =   MMISET_SET_MINUTE_REMINDER_EDITBOX_CTRL_ID;
    
    MMI_STRING_T time_number = {0};
    wchar num_wchar_str[3] = {0};
    char num_char_str[3] = {0};
    uint32 call_reminder_time = 0;
    
    time_number.wstr_ptr = num_wchar_str;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMISET_SET_MINUTE_REMINDER_EDIT_FROM1_ID,GUIFORM_STYLE_UNIT);
        
        GUILABEL_SetTextById(MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID,TXT_SET_MINUTE_REMINDER_TIME,FALSE);
        
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        
        call_reminder_time = MMIAPISET_GetMinuteReminderTime();
        MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
        MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
        time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
        GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
        break;
        
    case MSG_CTL_EDITBOX_UPDATE_STRNUM:
        GUIEDIT_GetString(ctrl_id, &time_number);
        
        MMIAPICOM_WstrToNum(time_number.wstr_ptr, time_number.wstr_len, &call_reminder_time);     
        if (call_reminder_time > CC_CALL_REMINDER_TIME_60S)
        {
            time_number.wstr_ptr = num_wchar_str;
            call_reminder_time = CC_CALL_REMINDER_TIME_60S;
            MMIAPICOM_Int2Str((uint32)call_reminder_time, (uint8*)num_char_str,2);
            MMI_STRNTOWSTR(time_number.wstr_ptr,4,(uint8 *)num_char_str,2,2);
            time_number.wstr_len = MMIAPICOM_Wstrlen(time_number.wstr_ptr);
            GUIEDIT_SetString(ctrl_id, time_number.wstr_ptr, time_number.wstr_len);
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, PNULL);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id, &time_number);
        
        MMIAPICOM_WstrToNum(time_number.wstr_ptr, time_number.wstr_len, &call_reminder_time);        

        MMISET_SetMinuteReminderTime(call_reminder_time); 

        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
 
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:  
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

// the window for mp3 alarm ring setting
LOCAL WINDOW_TABLE( MMIMP3_SET_ALARM_RING_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_TITLE( TXT_AUDIO ),
    WIN_FUNC( (uint32)HandleMp3AlarmRingSetWinMsg),
    WIN_ID( MMIMP3_ALARM_RING_SET_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIMP3_ALARM_RING_SET_LIST_CTRL_ID),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};
#ifdef BLUETOOTH_SUPPORT
//the window for edit blue tooth device name win table
LOCAL WINDOW_TABLE(MMIBT_EDIT_DEVICE_NAME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditDeviceNameMsg),    
    WIN_ID(MMIBT_EDIT_DEVICE_NAME_WIN_ID),
    WIN_TITLE(TXT_AUDIO),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIBT_BT_SEND_FILE_TEXT_CTRL_ID),
    CREATE_EDIT_TEXT_CTRL(0,MMIBT_EDIT_DEVICE_NAME_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};
#endif
//分钟提示音时间自定义窗口
LOCAL WINDOW_TABLE(MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleSetMinuteReminderEditWindow),  
    WIN_ID( MMISET_SET_MINUTE_REMINDER_EDIT_WIN_ID ),
    WIN_TITLE(TXT_SET_CALL_TIME_REMINDER),
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_MINUTE_REMINDER_EDIT_FROM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SET_MINUTE_REMINDER_EDIT_FROM1_ID,MMISET_SET_MINUTE_REMINDER_EDIT_FROM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SET_MINUTE_REMINDER_LABEL_CTRL_ID,MMISET_SET_MINUTE_REMINDER_EDIT_FROM1_ID),
    CHILD_EDIT_DIGITAL_CTRL(TRUE,2,MMISET_SET_MINUTE_REMINDER_EDITBOX_CTRL_ID,MMISET_SET_MINUTE_REMINDER_EDIT_FROM1_ID),
    END_WIN
}; 

/*****************************************************************************/
//  Discription: This fucntion is used to open the bluetooth main menu win  
//  
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIPUB_Test( void )
{
#ifdef BLUETOOTH_SUPPORT
    LOCAL uint32 s_test = 10;
    LOCAL BOOLEAN s_is_pubwin = TRUE;
    uint32 time = 0;
    
    s_prompt_str_array[0].wstr_ptr = L"James Zhang test Line 1 ===== 123456789123456789123456789123456789123456789";
    s_prompt_str_array[1].wstr_ptr = L"test Line 2 !!!!!";
    s_prompt_str_array[2].wstr_ptr = L"James\nZhang\ntest\nLine3\1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n";
    s_prompt_str_array[3].wstr_ptr = L"James Zhang test Line 4, 1234567890";
    
    s_prompt_str_array[0].wstr_len = MMIAPICOM_Wstrlen( s_prompt_str_array[0].wstr_ptr );
    s_prompt_str_array[1].wstr_len = MMIAPICOM_Wstrlen( s_prompt_str_array[1].wstr_ptr );
    s_prompt_str_array[2].wstr_len = MMIAPICOM_Wstrlen( s_prompt_str_array[2].wstr_ptr );
    s_prompt_str_array[3].wstr_len = MMIAPICOM_Wstrlen( s_prompt_str_array[3].wstr_ptr );
    
    switch(s_test)
    {
    case 1:
        MMIPUB_OpenAlertTextWinByTextId( &time,TXT_BT_HELP_DEVICE,PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,PNULL);
        MMIPUB_SetWinSoftkeyEx( MMIPUB_ALERT_TEXT_WIN_ID, TXT_COMMON_OK, STXT_EXIT, STXT_CANCEL, FALSE );
        break;
    case 2:
        MMIPUB_OpenQueryTextWinByTextId( TXT_BT_HELP_DEVICE,PNULL,PNULL );
        break;
    case 3:
        MMIPUB_OpenAlertWinByTextPtr( &time, s_prompt_str_array, s_prompt_str_array+2, IMAGE_PUBWIN_SUCCESS ,PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, PNULL );
        MMIPUB_SetWinSoftkey( MMIPUB_ALERT_WIN_ID, TXT_NULL, STXT_EXIT, FALSE );
        break;
    case 4:
        MMIPUB_OpenQueryWinByTextId( STXT_EXIT, IMAGE_PUBWIN_WARNING, PNULL,PNULL );
        MMIPUB_SetWinTitleTextId( MMIPUB_QUERY_WIN_ID, TXT_BT_HELP_DEVICE, FALSE );
        break;
        
    case 5:
        MMIPUB_OpenProgressWinByTextId( TXT_BT_HELP_DEVICE, PNULL, PNULL, 0, HandlePrgWinMsg );
        MMIPUB_SetProgressTotalSize(PNULL, 100);
        break;
        
    case 6:
        MMIPUB_OpenWaitWin(2,s_prompt_str_array,s_prompt_str_array+1,s_prompt_str_array + 2,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_PUBWIN_SUCCESS,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE, HandleWaitWinMsg );
        break;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    case 7:
        {
            MMIPUB_WAITING_PARAM_T param = {0};

            param.wait_string = s_prompt_str_array[2];

            MMIPUB_OpenWaitingWin( &param );
        }
        break;

    case 8:
        {
            if ( s_is_pubwin )
            {
                MMK_CreatePubwinTab(SPRD_GUI_LIST_ID, (uint32 *)MMIMP3_SET_ALARM_RING_WIN_TAB, PNULL);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMIMP3_SET_ALARM_RING_WIN_TAB, PNULL);
            }
        }
        break;
        
    case 9:
        {
            if ( s_is_pubwin )
            {
                MMK_CreatePubwinTab(SPRD_GUI_FORM_ID, (uint32 *)MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB, PNULL);
                
            }
            else
            {
                MMK_CreateWin((uint32 *)MMISET_SET_MINUTE_REMINDER_EDIT_WIN_TAB, PNULL);
            }
        }
        break;

    case 10:
        {
            if ( s_is_pubwin )
            {
                MMK_CreatePubwinTab(SPRD_GUI_EDITBOX_ID, (uint32 *)MMIBT_EDIT_DEVICE_NAME_WIN_TAB, (ADD_DATA)TRUE);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMIBT_EDIT_DEVICE_NAME_WIN_TAB, (ADD_DATA)TRUE);
            }
        }
        break;
#endif
    default:
        break;
    }
#endif
}
#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
