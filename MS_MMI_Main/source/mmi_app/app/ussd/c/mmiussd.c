/*****************************************************************************
** File Name:      mmiussd.c                                                   *
** Author:                                                                   *
** Date:           2004/11/19                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe ussd                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2004/11/19       bin.ji       Create
******************************************************************************/

#define _MMIUSSD_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ussd_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiussd_export.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "mmicc_export.h"
#include "mmi_textfun.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmi_anim.h"
#include "mmiussd_id.h"
#include "mmiussd_text.h"
#include "mmipub.h"
#include "window_parse.h"
#include "guilcd.h"
#include "guirichtext.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmicl_export.h"
#include "mmiudisk_export.h"
#include "mmiphone_export.h"
#include "mmistk_export.h"
#include "guiedit.h"
#include "sci_codec.h"
#include "gui_ucs2b_converter.h"
#include "mmi_default.h"
#include "mmisms_export.h"
#include "mmisms_text.h"
#include "mmicom_extract.h"
#include "spml_api.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMIUSSD_TEXT_BG_COLOR           0xFF56 //RGB: 255, 234, 181

#define MMIUSSD_TEXT_NUM                30 
#define MMISMS_VIBRA_TIMEROUT		    3000

#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
#define MMIUSSD_USSR_DISPLAY_TIMER_OUT (5*60*1000)// USSR需要显示2-5分钟，这里设置为5分钟
#define MMIUSSD_USSN_DISPLAY_TIMER_OUT (6*60*1000)// USSN需要大于5分钟，这里设置为6分钟
#endif

typedef enum
{
    MMIUSSD_STATUS_USSD_IND,
    MMIUSSD_STATUS_USSD_NOTIFY_IND,
    MMIUSSD_STATUS_USSD_SERVICE_CNF,
    MMIUSSD_STATUS_USSD_SEND,
    MMIUSSD_STATUS_MAX
}MMIUSSD_STATUS_E;

/*
chapter 5, 3GPP 23.038:
0x10
GSM 7 bit default alphabet; message preceded by language indication. 
The first 3 characters of the message are a two-character representation of the language encoded according to ISO 639 [12],
followed by a CR character. The CR character is then followed by 90 characters of text.

0x11 UCS2; message preceded by language indication
The message starts with a two 7-bit default alphabet character representation of the language encoded according to ISO 639 [12].
This is padded to the octet boundary with two bits set to 0 and then followed by 40 characters of UCS2-encoded message.
An MS not supporting UCS2 coding will present the two character language identifier followed by improperly interpreted user data.
*/
#define USSD_7BIT_WITH_INDICATION_DCS_LANG_LEN 3
#define USSD_UCS2_WITH_INDICATION_DCS_LANG_LEN 2

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMICL_CALL_INFO_T  s_ussd_call_info = {0};/*lint !e64*/
LOCAL BOOLEAN s_ussd_status[MMIUSSD_STATUS_MAX] = {0};
#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
LOCAL uint8 s_ussd_display_timer_id = 0;
#endif
LOCAL BOOLEAN s_ussd_need_playring = TRUE;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MN_DUAL_SYS_E s_ussd_dual_sys = MN_DUAL_SYS_1;

/*****************************************************************************/
//  Description : open showservicecnf window
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.22
/*****************************************************************************/
LOCAL void OpenPromptWaitingWin(
                                MMI_WIN_ID_T        win_id, 
			                    MMI_STRING_T*       prompt_str_ptr, //text array pointer
                                MMIPUB_HANDLE_FUNC  func            //handle message function
                                );

/*****************************************************************************/
// 	Description : Record the call info
//	Global resource dependence : 
//  Author:Harvey.Huang
//	Note:
/*****************************************************************************/
LOCAL void USSDRecordCallInfo(                                   
                                                        MN_DUAL_SYS_E dual_sys,
                                                        uint8	*str_ptr,
                                                        uint8	str_len
							  );

/*****************************************************************************/
//  Description :to send ussd response
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
LOCAL BOOLEAN SendUssdResponse(
                               wchar* str_ptr,
                               uint16 str_len
                               );

/*****************************************************************************/
// 	Description : input editbox，ok键请求服务，cancel键退出服务
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdInputWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
// 	Description : 显示service cnf内容，退出键不发送消息
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdShowServiceCnfWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  );

/*****************************************************************************/
// 	Description : 显示notify ind内容，cancel键退出服务
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdShowNotifyWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : 显示ind或notify ind内容，退出键发送消息
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdShowDataWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : handle ussd send window msg
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdSendWinMsg(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        );

/*****************************************************************************/
// 	Description : set ussd error code
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL void SetUssdErrorCode(                        
                            MMI_TEXT_ID_T              *error_code_text_ptr,   // IN/OUT: the scenario parameter
                            MN_SS_ERROR_CODE_E  err_code    // IN:
                            );

/*****************************************************************************/
//  Description :press cancel or redkey to exit
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
LOCAL void ReleaseUssdService(void);

/*****************************************************************************/
//  Description :to parse ussd data, show prompt string
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
LOCAL void ParseUssdData(
                         MN_SS_USSD_DATA_T* ussd_data_ptr,
                         uint16   msg_id
                         );

/*****************************************************************************/
// 	Description : to play msg ring
//	Global resource dependence : 
//  Author:Harvey.Huang
//	Note: 
/*****************************************************************************/
LOCAL void PlayUSSDRing(void);

/*****************************************************************************/
//  Description : set which sim to send ussd
//  Global resource dependence : none
//  Author:bown.zhang
//  Note: 2007.9.6
/*****************************************************************************/
LOCAL void MMIUSSD_SetDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get which sim to send ussd
//  Global resource dependence : none
//  Author:bown.zhang
//  Note: 2007.9.6
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMIUSSD_GetDualSys(void);

/*****************************************************************************/
//  Description : open ussd send win
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void OpenUssdSendWin(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
/*****************************************************************************/
//  Description : handle Ussd Display Timer Out
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void HandleUssdDisplayTimerOut(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Create Ussd Display Timer
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void CreateUssdDisplayTimer(MMI_WIN_ID_T win_id, uint32 time_out);

/*****************************************************************************/
//  Description : Destroy Ussd Display Timer
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void DestroyUssdDisplayTimer(void);
#endif

#if (defined(WIN32))||(!defined(PLATFORM_SC7702))
/*****************************************************************************/
//  Description : mmi response unstructured ss notify
//  Global resource dependence : none
//  Author: haoming.lu
//  Note: 
/*****************************************************************************/
LOCAL ERR_MNSS_CODE_E MNSS_ResponseUSSDNotifyEx(
										  MN_DUAL_SYS_E           dual_sys
										  );

/*****************************************************************************/
// 	Description : mmi calls this api to reject the ussd service with certain causes
//	Global resource dependence : none
//  Author:	xiaonian.li
//	Note: 
/*****************************************************************************/
LOCAL ERR_MNSS_CODE_E MNSS_RejUssdServiceEx(
									  MN_DUAL_SYS_E dual_sys,
									  MN_SS_ERROR_CODE_E cause // MN_SS_ERROR_UNKNOW_ALPHABET/MN_SS_ERROR_USSD_BUSY/MN_SS_ERROR_UNEXPECTED_DATA_VALUE
									  );
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : set ussd error code
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL void SetUssdErrorCode(                        
                            MMI_TEXT_ID_T              *error_code_text_ptr,   // IN/OUT: the scenario parameter
                            MN_SS_ERROR_CODE_E  err_code    // IN:
                            )
{
    if (PNULL == error_code_text_ptr)
    {
        return;
    }

    switch (err_code)
    { 
    case MN_SS_ERROR_CALL_BARRED:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_CALL_BARRED;
        break;

#ifdef ORANGE_SUPPORT
    case MN_SS_ERROR_BEARER_SERVICE_NOT_PROVISIONED:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_BEARER_SERVICE_NOT_PROVISIONED;
        break;

    case MN_SS_ERROR_TELE_SERVICE_NOT_PROVISIONED:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_TELE_SERVICE_NOT_PROVISIONED;
        break;

    case MN_SS_ERROR_ILLEGAL_SS_OPERATION:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_ILLEGAL_SS_OPERATION;
        break;

    case MN_SS_ERROR_ERROR_SS_STATUS:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_ERROR_SS_STATUS;
        break;

    case MN_SS_ERROR_SS_UNAVAILABLE:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_UNAVAILABLE;
        break;

    case MN_SS_ERROR_SS_SUBSCRIPTION_VIOLATION:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_SUBSCRIPTION_VIOLATION;
        break;

    case MN_SS_ERROR_SS_INCOMPATIBILITY:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_INCOMPATIBILITY;
        break;

    case MN_SS_ERROR_FACILITY_NOT_SUPPORTED:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_FACILITY_NOT_SUPPORTED;
        break;

    case MN_SS_ERROR_SS_ABSENT_SUBSCIRBER:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_ABSENT_SUBSCIRBER;
        break;

    case MN_SS_ERROR_SHORT_TERMDENIAL:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_SHORT_TERMDENIAL;
        break;

    case MN_SS_ERROR_LONG_TERMDENIAL:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_LONG_TERMDENIAL;
        break;

    case MN_SS_ERROR_SYSTEM_FAILURE:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_USSD_REQUEST_UNCOMPLETED;
        break;

    case MN_SS_ERROR_DATA_MISSING:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_DATA_MISSING;
        break;

    case MN_SS_ERROR_UNEXPECTED_DATA_VALUE:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_UNEXPECTED_DATA_VALUE;
        break;

    case MN_SS_ERROR_PW_REGISTRATION_FAILURE:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_PW_REGISTRATION_FAILURE;
        break;

    case MN_SS_ERROR_NEGATIVE_PW_CHECK:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_NEGATIVE_PW_CHECK;
        break;

    case MN_SS_ERROR_TEMP_FAILURE:
    	*error_code_text_ptr = (MMI_TEXT_ID_T)TXT_TEMP_FAILURE;
        break;

    case MN_SS_ERROR_NUMBER_OF_PW_ATTEMPTS_VIOLATION:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_NUMBER_OF_PW_ATTEMPTS_VIOLATION;
        break;

    case MN_SS_ERROR_UNKNOW_ALPHABET:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_UNKNOW_ALPHABET;
        break;

    case MN_SS_ERROR_USSD_BUSY:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_USSD_BUSY;
        break;

    case MN_SS_ERROR_INVAILD_DEFLECTED_NUMBER:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_INVAILD_DEFLECTED_NUMBER;
        break;

    case MN_SS_ERROR_SS_MAX_NUM_MPTR_EXCEEDED:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_SS_MAX_NUM_MPTR_EXCEEDED;
        break;

    case MN_SS_ERROR_RESOURCE_UNAVAILABLE:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_RESOURCE_UNAVAILABLE;
        break;
        
    default:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_USSD_RELEASEERRCODE;
        break;
#else
    default:
        *error_code_text_ptr = (MMI_TEXT_ID_T)TXT_USSD_REQUEST_UNCOMPLETED;
        break;
#endif
    }
}

/*****************************************************************************/
//  Description :press cancel or redkey to exit
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
LOCAL void ReleaseUssdService(void)    
{
    uint32 i = 0;
    ERR_MNSS_CODE_E ss_code = ERR_MNSS_NO_ERR;

    if (s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF])
    {
        //SCI_TRACE_LOW:"ReleaseUssdService has received service cnf"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_389_112_2_18_3_4_14_0,(uint8*)"");

        return;
    }

    for (i = MMIUSSD_STATUS_USSD_IND; i < MMIUSSD_STATUS_MAX; i++)
    {
        if (s_ussd_status[i])
        {
            //SCI_TRACE_LOW:"ReleaseUssdService s_ussd_status[%d]=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_398_112_2_18_3_4_14_1,(uint8*)"dd", i, s_ussd_status[i]);
            return;
        }
    }

    ss_code = MNSS_RelActivingServiceEx(MMIUSSD_GetDualSys());

    //SCI_TRACE_LOW:"ReleaseUssdService ss_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_405_112_2_18_3_4_14_2,(uint8*)"d", ss_code);

    MMIAPISTK_OnRelUSSD(MMIUSSD_GetDualSys());

    MMIAPISMS_CheckTimerMSG();
}

/*****************************************************************************/
//  Description : open showservicecnf window
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.22
/*****************************************************************************/
LOCAL void OpenPromptWaitingWin(
                                MMI_WIN_ID_T        win_id, 
			                    MMI_STRING_T*       prompt_str_ptr, //text array pointer
                                MMIPUB_HANDLE_FUNC  func            //handle message function
                                )
{
    MMI_STRING_T title = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;
    BOOLEAN is_open = FALSE;
    int iCheckIndex = 0;

    if (PNULL == prompt_str_ptr)
    {
        return;
    }

    //test if can display
    iCheckIndex = (prompt_str_ptr->wstr_len>1?prompt_str_ptr->wstr_len-1:0);
    while(iCheckIndex>=0)
    {    
        if((FALSE==SPMLAPI_TestChar(*(prompt_str_ptr->wstr_ptr+iCheckIndex)))
            &&(0xa != *(prompt_str_ptr->wstr_ptr+iCheckIndex))
            &&(0xd != *(prompt_str_ptr->wstr_ptr+iCheckIndex)))			
        {
            SCI_TRACE_LOW("USSD:[OpenPromptWaitingWin] : SPMLAPI_TestChar char== %x" ,*(prompt_str_ptr->wstr_ptr+iCheckIndex));
            MNSS_RejUssdServiceEx(MMIUSSD_GetDualSys(), MN_SS_ERROR_UNKNOW_ALPHABET);
            break;
        }
        iCheckIndex--;
    }

    if (!MMK_IsOpenWin(win_id)) 
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        MMI_CONTROL_CREATE_T create = {0};
        GUIRICHTEXT_INIT_DATA_T init_data = {0};
        void* ctrl_ptr = PNULL;

        is_open = FALSE;
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = win_id;
        win_create.func = func;

        if (MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
            || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
        {
            win_create.win_priority = WIN_LOWEST_LEVEL;
        }
        else
        {
            win_create.win_priority = WIN_ONE_LEVEL;
        }
        
        MMK_CreateWindow( &win_create );

#ifdef MMI_PDA_SUPPORT
        GUIWIN_CreateStbDyn(win_id);
#endif

        {
            //create softkey
            GUISOFTKEY_INIT_DATA_T softkey_data = {0};
            MMI_CONTROL_CREATE_T softkey_create = {0};
            MMI_CTRL_ID_T softkey_ctrl_id = 0;

#ifdef MMI_PDA_SUPPORT
            softkey_data.leftsoft_id = TXT_NULL;
            softkey_data.midsoft_id = TXT_COMMON_OK;
            softkey_data.rightsoft_id = TXT_NULL;
#else
            softkey_data.leftsoft_id = TXT_COMMON_OK;
            softkey_data.midsoft_id = TXT_NULL;
            softkey_data.rightsoft_id = STXT_RETURN;
#endif
            softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
            softkey_create.ctrl_id = softkey_ctrl_id;
            softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
            softkey_create.parent_win_handle = win_id;
            softkey_create.init_data_ptr = &softkey_data;

#ifdef MMI_PDA_SUPPORT
            MMK_SetWinDisplayStyleState( win_id, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );
#endif

            MMK_CreateControl( &softkey_create );
            MMK_SetWinSoftkeyCtrlId(win_id, softkey_ctrl_id);
        } 
        
        //set title
        GUIWIN_CreateTitleDyna(win_id, TXT_NULL);

        init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);

        create.ctrl_id = MMIUSSD_TEXTBOX_CTRL_ID;
        create.guid    = SPRD_GUI_RICHTEXT_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &init_data;
        ctrl_ptr = MMK_CreateControl( &create );
    }
    else
    {
        is_open = TRUE;
    }

    //Set title
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_USSD_NOTIFICATION, &title);
    GUIWIN_SetTitleText(win_id,title.wstr_ptr,title.wstr_len,FALSE);
    
    GUIRICHTEXT_DeleteAllItems(MMIUSSD_TEXTBOX_CTRL_ID);

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
#ifdef MAINLCD_SIZE_128X64
    item_data.tag_type = GUIRICHTEXT_TAG_NONE;
#else
    item_data.tag_type = GUIRICHTEXT_TAG_PARSE_PHONENUM;

#if defined(MMS_SUPPORT) || defined(MMIEMAIL_SUPPORT)
    item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_EMAIL;/*lint !e655*/
#endif

#ifdef BROWSER_SUPPORT
    item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_URL;/*lint !e655*/
#endif    
#endif

    item_data.text_data.buf.str_ptr = prompt_str_ptr->wstr_ptr;
    item_data.text_data.buf.len = prompt_str_ptr->wstr_len;

    //change '\r' to '\n' for only ending with '\r', or richtext control won't display the content after the single '\r'
    {
        int32 iCurPos = 0;
        for(iCurPos = 0; iCurPos < item_data.text_data.buf.len; iCurPos++)
        {
            if('\r' == item_data.text_data.buf.str_ptr[iCurPos] &&
                ( (iCurPos+1 >= item_data.text_data.buf.len) ||
                ('\n' != item_data.text_data.buf.str_ptr[iCurPos + 1]) )
                )
            {
                item_data.text_data.buf.str_ptr[iCurPos] = '\n';
            }
        }
        
    }

    GUIRICHTEXT_AddItem(
        MMIUSSD_TEXTBOX_CTRL_ID,
        &item_data,
        &index
        );

#ifdef MAINLCD_SIZE_128X64
    GUIRICHTEXT_SetFocusMode(MMIUSSD_TEXTBOX_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
#else
    GUIRICHTEXT_SetFocusMode(MMIUSSD_TEXTBOX_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
#endif

    if (is_open)
    {
        if (!MMK_IsFocusWin(win_id))
        {
            MMK_WinGrabFocus(win_id);
        }
        else
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    }
}

/*****************************************************************************/
//  Description : open show service cnf window
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.22
/*****************************************************************************/
LOCAL void OpenUssdShowServiceCnfWin(
                                     MMI_STRING_T *prompt_str_ptr
                                     )
{
    OpenPromptWaitingWin(
        MMIUSSD_SHOWSERVICECNF_WIN_ID, 
        prompt_str_ptr,
        HandleUssdShowServiceCnfWinMsg);
}

/*****************************************************************************/
//  Description : open show notify window
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.22
/*****************************************************************************/
LOCAL void OpenUssdShowNotifyWin(
                                 MMI_STRING_T *prompt_str_ptr
                                 )
{
    OpenPromptWaitingWin(
        MMIUSSD_SHOWNOTIFY_WIN_ID, 
        prompt_str_ptr,
        HandleUssdShowNotifyWinMsg);

#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
    CreateUssdDisplayTimer(MMIUSSD_SHOWNOTIFY_WIN_ID, MMIUSSD_USSN_DISPLAY_TIMER_OUT);
#endif
}

/*****************************************************************************/
//  Description : open showdata window
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.22
/*****************************************************************************/
LOCAL void OpenUssdShowDataWin(
                               MMI_STRING_T *prompt_str_ptr
                               )
{
    OpenPromptWaitingWin(
        MMIUSSD_SHOWDATA_WIN_ID, 
        prompt_str_ptr,
        HandleUssdShowDataWinMsg);

#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
    CreateUssdDisplayTimer(MMIUSSD_SHOWDATA_WIN_ID, MMIUSSD_USSR_DISPLAY_TIMER_OUT);
#endif
}

/*****************************************************************************/
//  Description : open input window
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.22
/*****************************************************************************/
LOCAL void OpenUssdInputWin(void)
{
    wchar key_value[]= {'*','#'};
    MMI_WIN_ID_T win_id = MMIUSSD_INPUT_WIN_ID;
    
    MMK_CloseWin(win_id);
    
    //if (!MMK_IsOpenWin(win_id)) 
    {        
        MMI_CONTROL_CREATE_T create = {0};
        GUIEDIT_INIT_DATA_T init_data = {0};
        MMI_WINDOW_CREATE_T win_create = {0};
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = win_id;
        win_create.func = HandleUssdInputWinMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        
        MMK_CreateWindow( &win_create );

        {
            //create softkey
            GUISOFTKEY_INIT_DATA_T softkey_data = {0};
            MMI_CONTROL_CREATE_T softkey_create = {0};
            MMI_CTRL_ID_T softkey_ctrl_id = 0;

#ifdef MMI_PDA_SUPPORT
            softkey_data.leftsoft_id = TXT_NULL;
            softkey_data.midsoft_id = TXT_COMMON_OK;
            softkey_data.rightsoft_id = TXT_NULL;
#else
            softkey_data.leftsoft_id = TXT_COMMON_OK;
            softkey_data.midsoft_id = TXT_NULL;
            softkey_data.rightsoft_id = STXT_RETURN;
#endif

            softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
            softkey_create.ctrl_id = softkey_ctrl_id;
            softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
            softkey_create.parent_win_handle = win_id;
            softkey_create.init_data_ptr = &softkey_data;

#ifdef MMI_PDA_SUPPORT
            MMK_SetWinDisplayStyleState( win_id, WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE, TRUE );
#endif

            MMK_CreateControl( &softkey_create );

            MMK_SetWinSoftkeyCtrlId(win_id, softkey_ctrl_id);
        } 
        
        //set title
        GUIWIN_CreateTitleDyna(win_id, TXT_NULL);

        init_data.type = GUIEDIT_TYPE_TEXT;
        init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
        init_data.str_max_len = CC_MAX_TELE_NUM_LEN;
        
        create.ctrl_id = MMIUSSD_INPUT_EDITBOX_CTRL_ID;
        create.guid    = SPRD_GUI_EDITBOX_ID;
        create.parent_win_handle = win_id; //use win handle here
        create.init_data_ptr = &init_data;
        
        MMK_CreateControl( &create );
        GUIEDIT_SetTextMaxLen(MMIUSSD_INPUT_EDITBOX_CTRL_ID, 
            (MN_SS_MAX_USSD_STRING_LENGTH_AFTER_ENCODE>>1), 
            MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE);
    }

    GUIWIN_SetTitleTextId(win_id, (MMI_TEXT_ID_T)TXT_REPLY, FALSE);

    MMK_SetAtvCtrl(win_id, MMIUSSD_INPUT_EDITBOX_CTRL_ID );
	//fixed for bug76901 via adjusting the executing sequence

#ifdef MMI_USSD_UCS2_NOT_SUPPORT
    GUIEDIT_SetIm(MMIUSSD_INPUT_EDITBOX_CTRL_ID,GUIIM_TYPE_SET_ALL,GUIIM_TYPE_DIGITAL);
#else
    GUIEDIT_SetIm(MMIUSSD_INPUT_EDITBOX_CTRL_ID, GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL|GUIIM_TYPE_MULTITAP, GUIIM_TYPE_DIGITAL);/*lint !e655*/
#endif
	
    GUIEDIT_ConfigImCustomKey(MMIUSSD_INPUT_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_CUSTOM_KEY_STAR, key_value, 2);
    GUIEDIT_SetImTag(MMIUSSD_INPUT_EDITBOX_CTRL_ID, GUIIM_TAG_NUM_WITH_SWITCHING);//modify to support switching vkb between abc and digital
}

/*****************************************************************************/
//  Description :to parse ussd data, show prompt string(according to chapter 5, 3GPP 23.038)
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
LOCAL void ParseUssdData(
                         MN_SS_USSD_DATA_T* ussd_data_ptr,
                         uint16   msg_id
                         ) 
{
    wchar ussd_str[MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE + 1] = {0};
    uint16 len = 0;
    MMI_STRING_T prompt_str = {0};
    
    if (PNULL == ussd_data_ptr)
    {
        return;
    }

    if (0 == ussd_data_ptr->str_len)
    {
        if (APP_MN_USSD_SERVICE_CNF == msg_id)
        {
            s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = TRUE;
        }
    
        //SCI_TRACE_LOW:"mmiussd.c ParseUssdData: 0 == ussd_data_ptr->str_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_714_112_2_18_3_4_15_3,(uint8*)"");
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        MMK_CloseWin(MMIUSSD_INPUT_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);

        if (APP_MN_USSD_SERVICE_CNF == msg_id)
        {
            s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = FALSE;
        }
        return;
    }
    if (MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE < ussd_data_ptr->str_len)
    {
//        SCI_TRACE_LOW("mmiussd.c ParseUssdData: ussd_data_ptr->str_len = %d", ussd_data_ptr->str_len);
        ussd_data_ptr->str_len = MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE;
    }

    //SCI_TRACE_LOW:"ParseUssdData data_code_scheme=0x%02x, str_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_731_112_2_18_3_4_15_4,(uint8*)"dd", ussd_data_ptr->data_code_scheme, ussd_data_ptr->str_len);

    switch (ussd_data_ptr->data_code_scheme)
    {
    case MN_SS_DEFAULT_DCS_VALUE:
    case MN_SS_8BIT_NO_CLASS_DCS:
    case MN_SS_7BIT_WITH_INDICATION_DCS:
        // gsm default alphabet
        if (MN_SS_DEFAULT_DCS_VALUE == ussd_data_ptr->data_code_scheme
            || MN_SS_7BIT_WITH_INDICATION_DCS == ussd_data_ptr->data_code_scheme)
        {
            uint8 * temp_buf = PNULL;
            uint32 ret_val = 0;
            uint8 *temp_ussd_str = PNULL;
            uint16 temp_str_len = 0;

            if (MN_SS_7BIT_WITH_INDICATION_DCS == ussd_data_ptr->data_code_scheme
                && ussd_data_ptr->str_len > USSD_7BIT_WITH_INDICATION_DCS_LANG_LEN)
            {
                temp_ussd_str = (uint8 *)(ussd_data_ptr->ussd_str + USSD_7BIT_WITH_INDICATION_DCS_LANG_LEN);
                temp_str_len = ussd_data_ptr->str_len - USSD_7BIT_WITH_INDICATION_DCS_LANG_LEN;
                len = temp_str_len * 8 / 7 + 1;
            }
            else
            {
                temp_ussd_str = ussd_data_ptr->ussd_str;
                temp_str_len = ussd_data_ptr->str_len;
                len = temp_str_len * 8 / 7 + 1;
            }

            temp_buf = SCI_ALLOC_APP(len);
            SCI_MEMSET(temp_buf, 0x00, len);

            ret_val = SCI_Unpack7bitsTo8bits(temp_ussd_str,
                                                            (int16)temp_str_len,
                                                            0,
                                                            temp_buf,
                                                            (int16 *)&len);

            //SCI_TRACE_LOW:"ParseUssdData ret_val=%d, len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_754_112_2_18_3_4_15_5,(uint8*)"dd", ret_val, len);

            len = MIN(MN_SS_MAX_USSD_STRING_LENGTH_BEFORE_ENCODE, len);

            len = MMIAPICOM_Default2Wchar(
                                            temp_buf,
                                            ussd_str,
                                            len
                                            );

            SCI_FREE(temp_buf);
            temp_buf = PNULL;
        }
        else if (MN_SS_8BIT_NO_CLASS_DCS == ussd_data_ptr->data_code_scheme)
        {
            len = ussd_data_ptr->str_len;

            len = MMIAPICOM_Default2Wchar(
                                            (uint8 *)ussd_data_ptr->ussd_str,
                                            ussd_str,
                                            len
                                            );
        }

        prompt_str.wstr_len = len;
        prompt_str.wstr_ptr = ussd_str;
		
#ifdef OD_GAME_SUPPORT
        if(od_game_unlock_game_by_ussd(&prompt_str,msg_id,ReleaseUssdService))
        {
            return;
        }
#endif		
        switch (msg_id)
        {
        case APP_MN_USSD_IND:
            SCI_TRACE_LOW("ParseUssdData APP_MN_USSD_IND of MN_SS_DEFAULT_DCS_VALUE or MN_SS_8BIT_NO_CLASS_DCS");
#if MMI_TCL_ORANGE_SUPORT            
            if (!(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
                || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
                )
				
                )
#endif                
            {
                OpenUssdShowDataWin(&prompt_str);
                PlayUSSDRing();
                MMIDEFAULT_TurnOnBackLight();
            }
#if MMI_TCL_ORANGE_SUPORT
            else
            {
                SCI_TRACE_LOW("ParseUssdData reject APP_MN_USSD_IND of MN_SS_DEFAULT_DCS_VALUE or MN_SS_8BIT_NO_CLASS_DCS");
                MNSS_RejUssdServiceEx(MMIUSSD_GetDualSys(), MN_SS_ERROR_USSD_BUSY);
            }            
#endif            
            break;

        case APP_MN_USSD_NOTIFY_IND:
            SCI_TRACE_LOW("ParseUssdData APP_MN_USSD_NOTIFY_IND of MN_SS_DEFAULT_DCS_VALUE or MN_SS_8BIT_NO_CLASS_DCS");
#if MMI_TCL_ORANGE_SUPORT         
            if (!(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
                || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
                )
                )
#endif                
            {
                MNSS_ResponseUSSDNotifyEx(MMIUSSD_GetDualSys());
                OpenUssdShowNotifyWin(&prompt_str);
                PlayUSSDRing();
                MMIDEFAULT_TurnOnBackLight();
            }
#if MMI_TCL_ORANGE_SUPORT
            else
            {
                SCI_TRACE_LOW("ParseUssdData reject APP_MN_USSD_NOTIFY_IND of MN_SS_DEFAULT_DCS_VALUE or MN_SS_8BIT_NO_CLASS_DCS");
                MNSS_RejUssdServiceEx(MMIUSSD_GetDualSys(), MN_SS_ERROR_USSD_BUSY);
            }
#endif            
            break;

        case APP_MN_USSD_SERVICE_CNF:
        default:
            OpenUssdShowServiceCnfWin(&prompt_str);
            break;
        }
        break;

    case MN_SS_USC2_NO_CLASS_DCS:
    case MN_SS_UCS2_WITH_INDICATION_DCS:
        // ucs2 alphabet
        {
            uint8 *temp_ussd_str = PNULL;

            if (MN_SS_UCS2_WITH_INDICATION_DCS == ussd_data_ptr->data_code_scheme
                && ussd_data_ptr->str_len > USSD_UCS2_WITH_INDICATION_DCS_LANG_LEN)
            {
                temp_ussd_str = (uint8 *)(ussd_data_ptr->ussd_str + USSD_UCS2_WITH_INDICATION_DCS_LANG_LEN);
                len = (uint16)(ussd_data_ptr->str_len - USSD_UCS2_WITH_INDICATION_DCS_LANG_LEN)/sizeof(wchar);
            }
            else
            {
                temp_ussd_str = ussd_data_ptr->ussd_str;
                len = ussd_data_ptr->str_len/sizeof(wchar);
            }

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            MMI_WSTRNCPY(
                ussd_str,
                sizeof(ussd_str),
                (wchar *)temp_ussd_str,
                len,
                len
                );
#else
            GUI_UCS2L2UCS2B((uint8 *)ussd_str,
                                            sizeof(ussd_str) * sizeof(wchar),
                                            (uint8 *)temp_ussd_str,
                                            len * sizeof(wchar));
#endif
        }

        prompt_str.wstr_len = len;
        prompt_str.wstr_ptr = ussd_str;
        //prompt_str.is_ucs2 = TRUE;
		
#ifdef OD_GAME_SUPPORT
        if(od_game_unlock_game_by_ussd(&prompt_str,msg_id,ReleaseUssdService))
        {
            return;
        }
#endif		
        switch (msg_id)
        {
        case APP_MN_USSD_IND:
            SCI_TRACE_LOW("ParseUssdData APP_MN_USSD_IND of MN_SS_USC2_NO_CLASS_DCS");
#if MMI_TCL_ORANGE_SUPORT
            if (!(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
                || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
                )
                )
#endif                
            {
                OpenUssdShowDataWin(&prompt_str);
                PlayUSSDRing();
                MMIDEFAULT_TurnOnBackLight();
            }
#if MMI_TCL_ORANGE_SUPORT
            else
            {
                SCI_TRACE_LOW("ParseUssdData reject APP_MN_USSD_IND of MN_SS_USC2_NO_CLASS_DCS");
                MNSS_RejUssdServiceEx(MMIUSSD_GetDualSys(), MN_SS_ERROR_USSD_BUSY);
            }
#endif            
            break;

        case APP_MN_USSD_NOTIFY_IND:
            SCI_TRACE_LOW("ParseUssdData APP_MN_USSD_NOTIFY_IND of MN_SS_USC2_NO_CLASS_DCS");
#if MMI_TCL_ORANGE_SUPORT
            if (!(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
                || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
                )
                )
#endif                
            {
                MNSS_ResponseUSSDNotifyEx(MMIUSSD_GetDualSys());  // just used in notify ind
                OpenUssdShowNotifyWin(&prompt_str);
                PlayUSSDRing();
                MMIDEFAULT_TurnOnBackLight();
            }
#if MMI_TCL_ORANGE_SUPORT
            else
            {
                SCI_TRACE_LOW("ParseUssdData reject APP_MN_USSD_NOTIFY_IND of MN_SS_USC2_NO_CLASS_DCS");
                MNSS_RejUssdServiceEx(MMIUSSD_GetDualSys(), MN_SS_ERROR_USSD_BUSY);
            }
#endif            
            break;

        case APP_MN_USSD_SERVICE_CNF:
        default:
            OpenUssdShowServiceCnfWin(&prompt_str);
            break;
        }
        break;

    default:
//        SCI_TRACE_LOW("mmiussd.c ParseUssdData: ussd_data_ptr->data_code_scheme = %d", ussd_data_ptr->data_code_scheme);
        MNSS_RejUssdServiceEx(MMIUSSD_GetDualSys(), MN_SS_ERROR_UNEXPECTED_DATA_VALUE);
        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWarningWin(TXTUSSD_UNRECOGNIZED);
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        break;
    }
}

/*****************************************************************************/
//  Description :to handle ussd ind
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
MMI_RESULT_E MMIAPIUSSD_HandleUssdInd(
                                   DPARAM param
                                   )   
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_USSD_IND_T    ussd_ind   = *((APP_MN_USSD_IND_T*)param); 
    
    MMIUSSD_SetDualSys(ussd_ind.dual_sys);

    ParseUssdData(&ussd_ind.ussd_data, APP_MN_USSD_IND);
    return result;
}

/*****************************************************************************/
//  Description :to handle ussd notify ind
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
MMI_RESULT_E MMIAPIUSSD_HandleUssdNotifyInd(
                                         DPARAM param
                                         ) 
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_USSD_NOTIFY_IND_T  ussd_notify_ind = *((APP_MN_USSD_NOTIFY_IND_T*)param);

    MMIUSSD_SetDualSys(ussd_notify_ind.dual_sys);

    ParseUssdData(&ussd_notify_ind.ussd_data, APP_MN_USSD_NOTIFY_IND);
    return result;
}

/*****************************************************************************/
//  Description :to handle ussd service confirm
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
MMI_RESULT_E MMIAPIUSSD_HandleUssdServiceCnf(
                                          DPARAM param
                                          )    
{
    APP_MN_USSD_SERVICE_CNF_T ussd_service_cnf = *((APP_MN_USSD_SERVICE_CNF_T*)param);
    MMI_TEXT_ID_T err_code_text = TXT_NULL;

    if (!MMK_IsOpenWin(MMIUSSD_SEND_WIN_ID)
        && MN_SS_OPERATION_SUCC != ussd_service_cnf.result_type)//单独的ussd notify业务 ，如果网络返回error，不将问题显示给用户
    {
        //SCI_TRACE_LOW:"not user request! ussd cnf error%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_903_112_2_18_3_4_15_6,(uint8*)"d", ussd_service_cnf.err_code);

        MMK_CloseWin(MMIUSSD_INPUT_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);

        return MMI_RESULT_FALSE;
    }

    MMIUSSD_SetDualSys(ussd_service_cnf.dual_sys);

    switch (ussd_service_cnf.result_type)
    {
    case MN_SS_OPERATION_SUCC:
        ParseUssdData(&ussd_service_cnf.ussd_data, APP_MN_USSD_SERVICE_CNF);
        break;

    case MN_SS_OPERATION_ERROR:
        s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = TRUE;
        //SCI_TRACE_LOW:"ussd cnf error%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_921_112_2_18_3_4_15_7,(uint8*)"d", ussd_service_cnf.err_code);
        SetUssdErrorCode(&err_code_text, ussd_service_cnf.err_code);
        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWarningWin(err_code_text);
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWNOTIFY_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
        s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = FALSE;
        break;

    case MN_SS_OPERATION_REJECT:
        s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = TRUE;
        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWarningWin(TXT_REJECT);
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWNOTIFY_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
        s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = FALSE;
        break;

    default:
//        SCI_TRACE_LOW("mmiussd.c MMIAPIUSSD_HandleUssdServiceCnf: ussd_ser_cnf_ptr->result_type = %d", ussd_service_cnf.result_type);
        break;
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : 显示service cnf内容
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdShowServiceCnfWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIUSSD_TEXTBOX_CTRL_ID;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type = GUIRICHTEXT_FOCUS_NONE;
    uint8 *focus_data_str_ptr = PNULL;
    uint16 index = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = TRUE;
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWNOTIFY_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);//not return
#endif
        MMK_SetAtvCtrl(win_id, MMIUSSD_TEXTBOX_CTRL_ID);    
        break;
		
    case MSG_FULL_PAINT:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_EXIT, TXT_NULL, FALSE);
#else
            BOOLEAN is_need_update = FALSE;

            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (MSG_FULL_PAINT == msg_id)
            {
                is_need_update = FALSE;
            }
            else
            {
                is_need_update = TRUE;
            }
            
            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_NUMBER, STXT_EXIT, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_URL_ADRESS, STXT_EXIT, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_EMAIL_BOX, STXT_EXIT, is_need_update);
                break;

            default:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, is_need_update);
                break;
            }
#endif
        }
        break;

    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
            uint32 call_type = CC_CALL_NORMAL_CALL;
            
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (focus_type == GUIRICHTEXT_FOCUS_PHONENUM)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                focus_data_str_ptr = SCI_ALLOCAZ(min_num_len + 1);
                if (PNULL == focus_data_str_ptr)
                {
                    break;
                }
                MMI_WSTRNTOSTR((uint8 *)focus_data_str_ptr, MMISMS_PBNUM_MAX_LEN, richtext_element.data.str_ptr, richtext_element.data.len, min_num_len);

                cc_result = MMIAPICC_MakeCall(
                                                                MMIUSSD_GetDualSys(),
                                                                focus_data_str_ptr,
                                                                richtext_element.data.len,
                                                                PNULL,
                                                                PNULL,
                                                                sim_type,
                                                                call_type,
                                                                PNULL
                                                                );

                SCI_FREE(focus_data_str_ptr);
                focus_data_str_ptr = PNULL;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
        {
            uint32 src_id = ((MMI_NOTIFY_T *)param)->src_id;

            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == src_id)
            {
                MMK_CloseWin(win_id);

                break;
            }
        }
#endif

        GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id,&index);

        switch(focus_type) 
        {
        case GUIRICHTEXT_FOCUS_PHONENUM:
            MMIAPICOM_OpenExtractNumberWin(MMIUSSD_EXTRACT_NUMBER_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;

        case GUIRICHTEXT_FOCUS_URL:
            MMIAPICOM_OpenExtractUrlWin(MMIUSSD_EXTRACT_URL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;

        case GUIRICHTEXT_FOCUS_EMAIL:
            MMIAPICOM_OpenExtractEmailWin(MMIUSSD_EXTRACT_EMAIL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;
     
        case GUIRICHTEXT_FOCUS_NONE:
            break;

        default:
            break;
        }
        break;

    case MSG_GET_FOCUS:
        if(MMK_IsOpenWin(MMIUSSD_SEND_WIN_ID))
        {
            MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        }        
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_SERVICE_CNF] = FALSE;
        break;

    default:
        recode = MMI_RESULT_FALSE;
//        SCI_TRACE_LOW("mmiussd.c: HandleUssdShowServiceCnfWinMsg() msg_id = %d", msg_id);
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : 显示notify ind内容，cancel键退出服务
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdShowNotifyWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIUSSD_TEXTBOX_CTRL_ID;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type = GUIRICHTEXT_FOCUS_NONE;
    uint8 *focus_data_str_ptr = PNULL;
    uint16 index = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_NOTIFY_IND] = TRUE;
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWSERVICECNF_WIN_ID);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);//not return
#endif
        MMK_SetAtvCtrl(win_id, MMIUSSD_TEXTBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_EXIT, TXT_NULL, FALSE);
#else
            BOOLEAN is_need_update = FALSE;

            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (MSG_FULL_PAINT == msg_id)
            {
                is_need_update = FALSE;
            }
            else
            {
                is_need_update = TRUE;
            }
            
            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_NUMBER, STXT_EXIT, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_URL_ADRESS, STXT_EXIT, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_EMAIL_BOX, STXT_EXIT, is_need_update);
                break;

            default:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, is_need_update);
                break;
            }
#endif
        }
        break;

    case MSG_APP_CALL1:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        {
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
            uint32 call_type = CC_CALL_NORMAL_CALL;
            
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (focus_type == GUIRICHTEXT_FOCUS_PHONENUM)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                focus_data_str_ptr = SCI_ALLOCA(min_num_len + 1);
                if (PNULL == focus_data_str_ptr)
                {
                    break;
                }
                SCI_MEMSET(focus_data_str_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)focus_data_str_ptr, MMISMS_PBNUM_MAX_LEN, richtext_element.data.str_ptr, richtext_element.data.len, min_num_len);

                cc_result = MMIAPICC_MakeCall(
                                                                MMIUSSD_GetDualSys(),
                                                                focus_data_str_ptr,
                                                                richtext_element.data.len,
                                                                PNULL,
                                                                PNULL,
                                                                sim_type,
                                                                call_type,
                                                                PNULL
                                                                );

                SCI_FREE(focus_data_str_ptr);
                focus_data_str_ptr = PNULL;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT
        {
            uint32 src_id = ((MMI_NOTIFY_T *)param)->src_id;

            if (MSG_CTL_PENOK == msg_id
                && MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == src_id)
            {
                MMK_CloseWin(win_id);

                break;
            }
        }
#endif

        GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id,&index);

        switch(focus_type) 
        {
        case GUIRICHTEXT_FOCUS_PHONENUM:
            MMIAPICOM_OpenExtractNumberWin(MMIUSSD_EXTRACT_NUMBER_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;

        case GUIRICHTEXT_FOCUS_URL:
            MMIAPICOM_OpenExtractUrlWin(MMIUSSD_EXTRACT_URL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;

        case GUIRICHTEXT_FOCUS_EMAIL:
            MMIAPICOM_OpenExtractEmailWin(MMIUSSD_EXTRACT_EMAIL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;
     
        case GUIRICHTEXT_FOCUS_NONE:
            break;

        default:
            break;
        }
        break;

    case MSG_CLOSE_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_NOTIFY_IND] = FALSE;
        ReleaseUssdService();
        s_ussd_call_info.call_time.stop_time = MMIAPICOM_GetCurTime();       
#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
        DestroyUssdDisplayTimer();
#endif
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
		
    return recode;
}

/*****************************************************************************/
// 	Description : input editbox，ok键请求服务，cancel键退出服务
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdInputWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T str_info = {0};
    MMI_CTRL_ID_T ctrl_id = MMIUSSD_INPUT_EDITBOX_CTRL_ID;
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIEDIT_GetString(ctrl_id, &str_info);
        if (str_info.wstr_len != 0)
        {
            SendUssdResponse(str_info.wstr_ptr, str_info.wstr_len);

            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
    	  GUIEDIT_SetIm(MMIUSSD_INPUT_EDITBOX_CTRL_ID,GUIIM_TYPE_SET_ALL,GUIIM_TYPE_ABC);
    	  break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : 显示ind内容，ok键editbox，cancel键退出服务
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdShowDataWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIUSSD_TEXTBOX_CTRL_ID;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type = GUIRICHTEXT_FOCUS_NONE;
    uint8 *focus_data_str_ptr = PNULL;
    uint16 index = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_IND] = TRUE;
        MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWNOTIFY_WIN_ID);
        MMK_CloseWin(MMIUSSD_SHOWSERVICECNF_WIN_ID);
        MMK_SetAtvCtrl(win_id, MMIUSSD_TEXTBOX_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id , TXT_NULL, TXT_REPLY, TXT_NULL, FALSE);
#else
            BOOLEAN is_need_update = FALSE;

            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (MSG_FULL_PAINT == msg_id)
            {
                is_need_update = FALSE;
            }
            else
            {
                is_need_update = TRUE;
            }
            
            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_REPLY, TXT_COMMON_NUMBER, STXT_EXIT, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_REPLY, TXT_URL_ADRESS, STXT_EXIT, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, TXT_REPLY, TXT_EMAIL_BOX, STXT_EXIT, is_need_update);
                break;

            default:
                GUIWIN_SetSoftkeyTextId(win_id , TXT_REPLY, TXT_NULL, STXT_EXIT, is_need_update);
                break;
            }
#endif
        }
        break;

    case MSG_APP_CALL1:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        {
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
            uint32 call_type = CC_CALL_NORMAL_CALL;
            
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            focus_type = richtext_element.type;

            if (focus_type == GUIRICHTEXT_FOCUS_PHONENUM)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                focus_data_str_ptr = SCI_ALLOCA(min_num_len + 1);
                if (PNULL == focus_data_str_ptr)
                {
                    break;
                }
                SCI_MEMSET(focus_data_str_ptr, 0, (min_num_len + 1));
                MMI_WSTRNTOSTR((uint8 *)focus_data_str_ptr, MMISMS_PBNUM_MAX_LEN, richtext_element.data.str_ptr, richtext_element.data.len, min_num_len);

                cc_result = MMIAPICC_MakeCall(
                                                                MMIUSSD_GetDualSys(),
                                                                focus_data_str_ptr,
                                                                richtext_element.data.len,
                                                                PNULL,
                                                                PNULL,
                                                                sim_type,
                                                                call_type,
                                                                PNULL
                                                                );

                SCI_FREE(focus_data_str_ptr);
                focus_data_str_ptr = PNULL;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id,&index);

        switch(focus_type) 
        {
        case GUIRICHTEXT_FOCUS_PHONENUM:
            MMIAPICOM_OpenExtractNumberWin(MMIUSSD_EXTRACT_NUMBER_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;

        case GUIRICHTEXT_FOCUS_URL:
            MMIAPICOM_OpenExtractUrlWin(MMIUSSD_EXTRACT_URL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;

        case GUIRICHTEXT_FOCUS_EMAIL:
            MMIAPICOM_OpenExtractEmailWin(MMIUSSD_EXTRACT_EMAIL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, MMIUSSD_GetDualSys());
            break;
     
        case GUIRICHTEXT_FOCUS_NONE:
            OpenUssdInputWin();
#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
            DestroyUssdDisplayTimer();
#endif
            break;

        default:
            break;
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        OpenUssdInputWin();
#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
        DestroyUssdDisplayTimer();
#endif
        break;

     case MSG_CLOSE_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_IND] = FALSE;
        ReleaseUssdService();
        s_ussd_call_info.call_time.stop_time = MMIAPICOM_GetCurTime();
#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
        DestroyUssdDisplayTimer();
#endif
        break;
       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : handle ussd send window msg
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.11.19
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUssdSendWinMsg(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_SEND] = TRUE;
        MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        s_ussd_status[MMIUSSD_STATUS_USSD_SEND] = FALSE;
        ReleaseUssdService();
        s_ussd_call_info.call_time.stop_time = MMIAPICOM_GetCurTime();
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
//        SCI_TRACE_LOW("mmiussd.c: HandleUssdSendWinMsg() msg_id = %d", msg_id);
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description :to send ussd response
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
LOCAL BOOLEAN SendUssdResponse(
                               wchar* str_ptr,
                               uint16 str_len
                               )
{ 
    ERR_MNSS_CODE_E response_code = ERR_MNSS_NO_ERR;
    
    if (MMIAPICOM_IsDefaultString(str_ptr, str_len))
    {
        uint8 temp_str[(CC_MAX_TELE_NUM_LEN << 1) + 1] = {0};

        str_len = MMIAPICOM_Wchar2default(str_ptr, temp_str, str_len);

        response_code = MNSS_ResponseUSSDEx(
                                            MMIUSSD_GetDualSys(),//dual sys later
                                            MN_SS_DEFAULT_DCS_VALUE, 
                                            (uint8)str_len, 
                                            (uint8*)temp_str);
    }    
    else
    {
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        response_code = MNSS_ResponseUSSDEx(
                                            MMIUSSD_GetDualSys(),//dual sys later
                                            MN_SS_USC2_NO_CLASS_DCS, 
                                            (uint8)str_len, 
                                            (uint8*)str_ptr);
#else
        uint8 *temp_str = PNULL;

        temp_str = SCI_ALLOCA(str_len * sizeof(wchar) + 1);

        if (PNULL != temp_str)
        {
            SCI_MEMSET(temp_str, 0x00, (str_len * sizeof(wchar) + 1));

            GUI_UCS2B2UCS2L((uint8 *)temp_str,
                                            str_len * sizeof(wchar),
                                            (uint8 *)str_ptr,
                                            str_len * sizeof(wchar));

            response_code = MNSS_ResponseUSSDEx(
                                                MMIUSSD_GetDualSys(),//dual sys later
                                                MN_SS_USC2_NO_CLASS_DCS, 
                                                (uint8)str_len, 
                                                (uint8*)temp_str);

            SCI_FREE(temp_str);
            temp_str = PNULL;
        }
        else
        {
            response_code = ERR_MNSS_INVALID_OPERATION;
        }
#endif
    }
    if (ERR_MNSS_NO_ERR == response_code)
    {
        OpenUssdSendWin(MMIUSSD_GetDualSys());

        return TRUE;
    }    
    return FALSE;
}

/*****************************************************************************/
//  Description :to parse ussd string
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
BOOLEAN MMIAPIUSSD_ParseUssdString(
                                   MN_DUAL_SYS_E dual_sys,//in, 标示使用哪张sim卡拨出
                                   uint8*   str_ptr,
                                   uint8	str_len
                                   )
{
    MN_PARSE_INFO_U ss_parse_info;
    MN_OPERATION_E  phone_oper = MN_UNKONOWN_OPERATION;
    ERR_MNSS_CODE_E  err_code = ERR_MNSS_NO_ERR;

    SCI_MEMSET(&ss_parse_info, 0, sizeof(MN_PARSE_INFO_U));
    //SCI_ASSERT(PNULL != str_ptr);/* assert to do*/
    if(PNULL == str_ptr)
    {
       return FALSE;
    }

    //set ussd by which sim
    MMIUSSD_SetDualSys(dual_sys);

    //get the string oper type
    phone_oper = MNPHONE_ParseBStringEx(dual_sys, str_ptr,str_len, &ss_parse_info);//dual sys later

    if (MN_USSD_OPERATION == phone_oper
        || MN_SS_OPERATION == phone_oper)
    {
#ifdef MMI_NOT_SUPPORT_USSD_SS_INCC
        //delete ussd function for watch
        SCI_TRACE_LOW("MMIAPIUSSD_ParseUssdString is USSD String!\n");
        return TRUE;
#else
        if (MMIAPISET_GetFlyMode())
        {
            MMIAPIPHONE_AlertSimNorOKStatus();

            return TRUE;
        }

        if (!MMIAPIPHONE_IsSimOk(dual_sys))
        {
            if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (!MMIAPIPHONE_IsSimAvailable(dual_sys))//该卡槽是否有卡并且待机
            {
                MMIAPIPHONE_AlertSimNorOKStatusII(dual_sys);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CC_EMERGENCYONLY);
            }

            return TRUE;
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();

            return TRUE;
        }
#endif

        err_code = MNSS_UserOriginateUSSDEx(//dual sys later
                                                                        dual_sys,
                                                                        FALSE,
                                                                        MN_SS_DEFAULT_DCS_VALUE,
                                                                        str_len,
                                                                        str_ptr
                                                                        );

        //SCI_TRACE_LOW:"MMIUSSD: MMIAPIUSSD_ParseUssdString ss_code=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUSSD_1703_112_2_18_3_4_17_8,(uint8*)"d", err_code);

        if (ERR_MNSS_NO_ERR == err_code)
        {
            //Record the call info
            USSDRecordCallInfo(dual_sys, str_ptr, str_len);	

            OpenUssdSendWin(dual_sys);

            return TRUE;
        }
        else if (ERR_MNSS_FDN_FAILED == err_code)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
#endif
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :Is Send Win Open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUSSD_IsSendWinOpen(void)
{
    return MMK_IsOpenWin(MMIUSSD_SEND_WIN_ID);
}

/*****************************************************************************/
//  Description :to close ussd sending win
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_CloseSendWin(void)
{
    MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
}

/*****************************************************************************/
//  Description :to close ussd input string win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_CloseInputWin(void)
{
    MMK_CloseWin(MMIUSSD_INPUT_WIN_ID);
}

/*****************************************************************************/
//  Description :is open show data win
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_CloseShowDataWin(void)
{
    MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
}

/*****************************************************************************/
// 	Description : to play ussd ring
//	Global resource dependence : 
//  Author:Harvey.Huang
//	Note: 
/*****************************************************************************/
LOCAL void PlayUSSDRing(void)
{
    if(MMIAPIUSSD_GetPlayringFlage())
    {
        MMIAPISMS_PlayNewSMSRing(MMIUSSD_GetDualSys());
    }
}

/*****************************************************************************/
// 	Description :Record the call info
//	Global resource dependence : 
//  Author:Harvey.Huang
//	Note:
/*****************************************************************************/
LOCAL void USSDRecordCallInfo(
                                                        MN_DUAL_SYS_E dual_sys,
                                                        uint8	*str_ptr,
                                                        uint8	str_len
							  )
{
    MMICL_CALL_INFO_T		call_info = {0};/*lint !e64*/
    MMI_PARTY_NUMBER_T      party_num = {0};/*lint !e64*/

    call_info.dual_sys = dual_sys;

    //get call time info
    call_info.call_time.start_time = MMIAPICOM_GetCurTime();
    call_info.call_time.stop_time =	call_info.call_time.start_time;

    //get call number info
    call_info.call_num.is_num_present = TRUE;

    MMIAPICOM_GenPartyNumber(str_ptr, str_len, &party_num);

    call_info.call_num.number.number_plan = MN_NUM_PLAN_ISDN_TELE;
    call_info.call_num.number.number_type = party_num.number_type;

    MMI_MEMCPY(call_info.call_num.number.party_num,     /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                            MN_MAX_ADDR_BCD_LEN,        /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                            party_num.bcd_num,          /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                            party_num.num_len,          /* parasoft-suppress BD-RES-INVFREE "有判断检查" */
                            party_num.num_len);         /* parasoft-suppress BD-RES-INVFREE "有判断检查" */

    call_info.call_num.number.num_len = party_num.num_len;

    SCI_MEMSET(&s_ussd_call_info,0,sizeof(MMICL_CALL_INFO_T));
    MMI_MEMCPY(&s_ussd_call_info,
                            sizeof(MMICL_CALL_INFO_T),
                            &call_info,
                            sizeof(MMICL_CALL_INFO_T),
                            sizeof(MMICL_CALL_INFO_T));
}

/*****************************************************************************/
//  Description : set which sim to send ussd
//  Global resource dependence : none
//  Author:bown.zhang
//  Note: 2007.9.6
/*****************************************************************************/
LOCAL void MMIUSSD_SetDualSys(MN_DUAL_SYS_E dual_sys)
{
    s_ussd_dual_sys = dual_sys;
}

/*****************************************************************************/
//  Description : get which sim to send ussd
//  Global resource dependence : none
//  Author:bown.zhang
//  Note: 2007.9.6
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMIUSSD_GetDualSys(void)
{
    return s_ussd_dual_sys;
}

/*****************************************************************************/
//  Description : get which sim to send ussd
//  Global resource dependence : none
//  Author:bown.zhang
//  Note: 2007.9.6
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIUSSD_GetDualSys(void)
{
    return MMIUSSD_GetDualSys();
}

/*****************************************************************************/
//  Description : open ussd send win
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void OpenUssdSendWin(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T prompt_str = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T sim_string_ptr = {0};
#endif

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    MMI_GetLabelTextByLang(
                                            (MMI_TEXT_ID_T)TXT_USSD_REQUEST,
                                            &prompt_str
                                            );

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    sim_name = MMIAPISET_GetSimName(dual_sys);

    sim_string_ptr.wstr_len = sim_name.wstr_len;
    sim_string_ptr.wstr_ptr = sim_name.wstr_arr;

    MMIPUB_OpenWaitWin(2,&sim_string_ptr,&prompt_str,PNULL,MMIUSSD_SEND_WIN_ID,IMAGE_NULL,
                                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandleUssdSendWinMsg);
#else
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIUSSD_SEND_WIN_ID,IMAGE_NULL,
                                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandleUssdSendWinMsg);
#endif
}

/*****************************************************************************/
//Description : Get USSD SIM(Not In USSD return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIUSSD_GetUssdSIM(void)
{
    uint32 i = 0;

    for (i = MMIUSSD_STATUS_USSD_IND; i < MMIUSSD_STATUS_MAX; i++)
    {
        if (MMIUSSD_STATUS_USSD_SERVICE_CNF != i && s_ussd_status[i])
        {
            return MMIUSSD_GetDualSys();
        }
    }

    return MMI_DUAL_SYS_MAX;
}

/*****************************************************************************/
//  Description :Grab to Focus
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_WinGrabFocus(void)
{
    MMK_SetWinPriority(MMK_ConvertIdToHandle(MMIUSSD_SHOWDATA_WIN_ID), WIN_ONE_LEVEL);

    MMK_SetWinPriority(MMK_ConvertIdToHandle(MMIUSSD_SHOWSERVICECNF_WIN_ID), WIN_ONE_LEVEL);

    MMK_SetWinPriority(MMK_ConvertIdToHandle(MMIUSSD_SHOWNOTIFY_WIN_ID), WIN_ONE_LEVEL);

    if (MMK_IsOpenWin(MMIUSSD_SHOWSERVICECNF_WIN_ID))
    {
        MMK_WinGrabFocus(MMIUSSD_SHOWSERVICECNF_WIN_ID);
    }

    if (MMK_IsOpenWin(MMIUSSD_SHOWNOTIFY_WIN_ID))
    {
        MMK_WinGrabFocus(MMIUSSD_SHOWNOTIFY_WIN_ID);
    }

    if (MMK_IsOpenWin(MMIUSSD_SHOWDATA_WIN_ID))
    {
        MMK_WinGrabFocus(MMIUSSD_SHOWDATA_WIN_ID);
    }
    
    if (MMK_IsOpenWin(MMIUSSD_INPUT_WIN_ID))
    {
        MMK_WinGrabFocus(MMIUSSD_INPUT_WIN_ID);
    }
}

#ifdef MMI_USSD_DISPLAY_TIMEOUT_SUPPORT
/*****************************************************************************/
//  Description : handle Ussd Display Timer Out
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL void HandleUssdDisplayTimerOut(uint8 timer_id, uint32 param)
{
    uint32 win_id = (uint32)param;

    if (timer_id == s_ussd_display_timer_id)
    {
        //MMK_CloseWin(MMIUSSD_SEND_WIN_ID);
        //MMK_CloseWin(MMIUSSD_SHOWDATA_WIN_ID);
        //MMK_CloseWin(MMIUSSD_SHOWSERVICECNF_WIN_ID);
        //MMK_CloseWin(MMIUSSD_SHOWNOTIFY_WIN_ID);
        MMK_CloseWin((MMI_WIN_ID_T)win_id);

        s_ussd_display_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : Create Ussd Display Timer
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void CreateUssdDisplayTimer(MMI_WIN_ID_T win_id, uint32 time_out)
{
    DestroyUssdDisplayTimer();

    s_ussd_display_timer_id = MMK_CreateTimerCallback(time_out, HandleUssdDisplayTimerOut, (uint32)win_id, FALSE);
}

/*****************************************************************************/
//  Description : Destroy Ussd Display Timer
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void DestroyUssdDisplayTimer(void)
{
    if (0 != s_ussd_display_timer_id)
    {
        MMK_StopTimer(s_ussd_display_timer_id);

        s_ussd_display_timer_id = 0;
    }
}
#endif

/*****************************************************************************/
//  Description :Set playring flage
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_SetPlayringFlage(BOOLEAN isNeedRing)
{
    s_ussd_need_playring = isNeedRing;
}

/*****************************************************************************/
//  Description :Get playring flage
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUSSD_GetPlayringFlage(void)
{
    return s_ussd_need_playring;
}

#if (defined(WIN32))||(!defined(PLATFORM_SC7702))
/*****************************************************************************/
//  Description : mmi response unstructured ss notify
//  Global resource dependence : none
//  Author: haoming.lu
//  Note: 
/*****************************************************************************/
LOCAL ERR_MNSS_CODE_E MNSS_ResponseUSSDNotifyEx(
										  MN_DUAL_SYS_E           dual_sys
										  )
{
	
	return ERR_MNSS_NO_ERR;
}

/*****************************************************************************/
// 	Description : mmi calls this api to reject the ussd service with certain causes
//	Global resource dependence : none
//  Author:	xiaonian.li
//	Note: 
/*****************************************************************************/
LOCAL ERR_MNSS_CODE_E MNSS_RejUssdServiceEx(
									  MN_DUAL_SYS_E dual_sys,
									  MN_SS_ERROR_CODE_E cause // MN_SS_ERROR_UNKNOW_ALPHABET/MN_SS_ERROR_USSD_BUSY/MN_SS_ERROR_UNEXPECTED_DATA_VALUE
									  )
{
	return ERR_MNSS_NO_ERR;
}
#endif
/*Edit by script, ignore 4 case. Fri Apr 27 09:38:55 2012*/ //IGNORE9527
