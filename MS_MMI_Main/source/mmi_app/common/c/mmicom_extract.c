/*****************************************************************************
** File Name:      mmicom_extract.c                                                   *
** Author:                                                                   *
** Date:           2012/01/04                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe extract number/url                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**
******************************************************************************/

#define _MMICOM_EXTRACT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "window_parse.h"
#include "gui_ucs2b_converter.h"
#include "mmi_menutable.h"
#include "mmipb_export.h"
#include "mmiudisk_export.h"
#include "mmicc_export.h"
#include "mmisms_export.h"
#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mail_fldr.h"
#include "mmimail_export.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :extract number handle func
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleExtractNumberPopWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :extract url handle func
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleExtractUrlPopWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :extract email handle func
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleExtractEmailPopWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :Create extract Pop Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateExtractPopWin(MMI_WIN_ID_T win_id,
                                                            ADD_DATA add_data_ptr,
                                                            uint16 data_len,
                                                            MN_DUAL_SYS_E dual_sys,
                                                            MMI_MENU_GROUP_ID_T group_id,
                                                            MMI_CTRL_ID_T ctrl_id,
                                                            PROCESSMSG_FUNC func);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :extract number handle fun
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleExtractNumberPopWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_CTRL_ID_T ctrl_id = MMI_EXTRACT_NUMBER_POP_CTRL_ID;
    wchar *add_data_ptr = (wchar *)MMK_GetWinAddDataPtr(win_id);
    uint8 *number_ptr = PNULL;
    uint16 num_len = 0;
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
#ifdef MMS_SUPPORT
    MMI_STRING_T string_to  = {0};
#endif
    MMI_STRING_T string_subject = {0};
    MMIPB_BCD_NUMBER_T pb_bcd = {0};
    MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataEx(win_id);
    
    //SCI_TRACE_LOW:"HandleExtractNumberPopWinMsg msg_id = 0x%x, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_EXTRACT_130_112_2_18_1_59_7_82,(uint8*)"dd", msg_id, dual_sys);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPICOM_EnableGrayed(win_id, MENU_EXTRACT_NUMBER_POP_MENU, ID_EXTRACT_ANSWER_MMS_ITEM_ID, TRUE);
        }

        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);

        //弹出popmenu
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

        if (PNULL == add_data_ptr)
        {
            break;
        }

        num_len = MMIAPICOM_Wstrlen(add_data_ptr);

        number_ptr = SCI_ALLOC_APPZ(num_len + 1);

        if (PNULL == number_ptr)
        {
            break;
        }

        MMI_WSTRNTOSTR(number_ptr, num_len, add_data_ptr, num_len, num_len);

        switch (menu_id)
        {
        case ID_EXTRACT_DIAL_ITEM_ID:
#ifdef VT_SUPPORT
        case ID_EXTRACT_VIDEOCALL_ITEM_ID:
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case ID_EXTRACT_IPDIAL_ITEM_ID:          
#endif
            if(ID_EXTRACT_DIAL_ITEM_ID == menu_id)
            {                        
                cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    CC_CALL_SIM_MAX,
                                                                    CC_CALL_NORMAL_CALL,
                                                                    PNULL
                                                                    );
            }
#ifdef VT_SUPPORT
            else if (ID_EXTRACT_VIDEOCALL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    CC_CALL_SIM_MAX,
                                                                    CC_CALL_VIDEO_CALL,
                                                                    PNULL
                                                                    );
            }
#endif
#ifdef MMI_IP_CALL_SUPPORT
            else
            {
                cc_result = MMIAPICC_MakeCall(
                                                                    dual_sys,
                                                                    number_ptr,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    CC_CALL_SIM_MAX,
                                                                    CC_CALL_IP_CALL,
                                                                    PNULL
                                                                    );
            }
#endif
            break;

        case ID_EXTRACT_ANSWER_SMS_ITEM_ID:
            {
                num_len = MIN(num_len, MMISMS_NUMBER_MAX_LEN);

                MMIAPISMS_AnswerMessage(dual_sys, PNULL, number_ptr, num_len);
            }
            break;

#ifdef MMS_SUPPORT
        case ID_EXTRACT_ANSWER_MMS_ITEM_ID:
            {
                //answer mms
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);

                string_to.wstr_ptr = (wchar *)add_data_ptr;
                string_to.wstr_len = num_len;

                MMIAPIMMS_AnswerMMS(dual_sys, &string_subject, &string_to);
            }
            break;
#endif
            
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
        case ID_EXTRACT_ANSWER_MAIL_ITEM_ID:
            {
                char * address = PNULL;
                char *add_string ="@139.com";
                uint16 string_len = NULL;
                
                string_len = SCI_STRLEN((char *)add_string);
                
                address= (char*)SCI_ALLOC_APP((num_len + string_len +1) * sizeof(char));
                SCI_MEMSET(address, 0x00, ((num_len + string_len+1) * sizeof(char)));
                SCI_STRCPY(address, (char*)number_ptr);
                strncat(address,add_string,string_len);
                MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, address);
            }
            break;
#endif

        case ID_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(number_ptr, num_len, &party_num);
                
                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);

                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );

                //MMIAPIPB_OpenAddMenu(&pb_bcd, PB_GROUP_ALL);
                MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(number_ptr, num_len, &party_num);
                
                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);

                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
                //MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
            }
            break;
#endif
        default:
            break;
        }

        SCI_FREE(number_ptr);

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :extract url handle fun
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleExtractUrlPopWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id  = 0;
    MMI_CTRL_ID_T ctrl_id = MMI_EXTRACT_URL_POP_CTRL_ID;
    wchar * add_data_ptr = ( wchar *)MMK_GetWinAddDataPtr(win_id);
    uint8 *url_ptr  = PNULL;
    uint16 url_len = 0;
#if (!defined PB_SUPPORT_LOW_MEMORY) || (defined MMIPB_ADD_EXIST_CONTACT_SUPPORT)
    MMIPB_CUSTOM_TEXT_T pb_url = {0};
#endif
    uint32 dual_sys = (uint32)MMK_GetWinAddDataEx(win_id);
    
    //SCI_TRACE_LOW:"MMIUSSSD: HandleExtractUrlPopWinMsg msg_id = 0x%x, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_EXTRACT_342_112_2_18_1_59_7_83,(uint8*)"dd", msg_id, dual_sys);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

        if (PNULL == add_data_ptr)
        {
            break;
        }

        url_len = MMIAPICOM_Wstrlen(add_data_ptr);

        url_ptr = SCI_ALLOC_APPZ(url_len + 1);

        if (PNULL == url_ptr)
        {
            break;
        }

        MMI_WSTRNTOSTR(url_ptr, url_len, add_data_ptr, url_len, url_len);

        switch (menu_id)
        {
        case ID_EXTRACT_URL_BROWSE_ITEM_ID:
            {
#ifdef BROWSER_SUPPORT
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
                
                entry_param.type = MMIBROWSER_ACCESS_URL;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
#ifdef  CMCC_UI_STYLE
                entry_param.is_alert= TRUE;
#endif
                MMIAPIBROWSER_Entry(&entry_param);
#endif
            }
            break;

        case ID_EXTRACT_URL_ADDTOBOOKMARK_ITEM_ID:
            {
#ifdef BROWSER_SUPPORT
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
                
                entry_param.type = MMIBROWSER_ADD_BOOKMARK;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&entry_param);
#endif
            }
            break;

#ifndef PB_SUPPORT_LOW_MEMORY
        case ID_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                url_len = MIN(url_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));

                MMI_STRNTOWSTR(pb_url.wstr, MMIPB_MAX_CUSTOM_TEXT_LEN, url_ptr, url_len, url_len);

                pb_url.wstr_len = url_len;

                MMIAPIPB_AddContactWin(&pb_url, PB_GROUP_ALL,MMIPB_ADD_TYPE_URL);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                url_len = MIN(url_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));

                MMI_STRNTOWSTR(pb_url.wstr, MMIPB_MAX_CUSTOM_TEXT_LEN, url_ptr, url_len, url_len);

                pb_url.wstr_len = url_len;
                MMIAPIPB_AddToExistContact(&pb_url, MMIPB_ADD_TYPE_URL, PNULL);
                //MMIAPIPB_OpenEntryListForEx(&pb_url,MMIPB_ADD_TYPE_URL);
            }
            break;  
#endif            
#endif

        default :
            break;
        }

        SCI_FREE(url_ptr);
        
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :extract email handle fun
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleExtractEmailPopWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E err_code  = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    MMI_CTRL_ID_T ctrl_id = MMI_EXTRACT_EMAIL_POP_CTRL_ID;
    wchar *email_ptr = ( wchar * )MMK_GetWinAddDataPtr(win_id);
    uint16 email_len = 0;
#ifdef MMS_SUPPORT
    MMI_STRING_T string_to = {0};
#endif
    MMI_STRING_T string_subject = {0};
    MMIPB_MAIL_T pb_email = {0};
    uint32 dual_sys = (uint32)MMK_GetWinAddDataEx(win_id);
    
    //SCI_TRACE_LOW:"HandleExtractEmailPopWinMsg msg_id = 0x%x, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_EXTRACT_481_112_2_18_1_59_7_84,(uint8*)"dd", msg_id, dual_sys);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId( ctrl_id, &group_id, &menu_id);

        if (PNULL == email_ptr)
        {
            break;
        }

        email_len = MMIAPICOM_Wstrlen(email_ptr);

        switch (menu_id)
        {
#ifdef MMS_SUPPORT
        case ID_EXTRACT_SEND_MMS_ITEM_ID:
            {
                email_len = MIN(email_len, MMIMMS_MAX_ADDRESS_LEN);

                string_to.wstr_ptr = (wchar *)email_ptr;
                string_to.wstr_len = email_len;
                MMIAPIMMS_AnswerMMS(dual_sys, &string_subject, &string_to);
            }
            break;
#endif

        case ID_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                email_len = MIN(email_len, MMIPB_MAX_MAIL_LEN);

                SCI_MEMSET(&pb_email,0x0,sizeof(MMIPB_MAIL_T));

                MMI_WSTRNCPY(pb_email.wstr, email_len, email_ptr, email_len, email_len);

                pb_email.wstr_len = email_len;
                MMIAPIPB_AddContactWin(&pb_email,PB_GROUP_ALL,MMIPB_ADD_TYPE_EMAIL);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                email_len = MIN(email_len, MMIPB_MAX_MAIL_LEN);

                SCI_MEMSET(&pb_email,0x0,sizeof(MMIPB_MAIL_T));

                MMI_WSTRNCPY(pb_email.wstr, email_len, email_ptr, email_len, email_len);

                pb_email.wstr_len = email_len;
                MMIAPIPB_AddToExistContact(&pb_email, MMIPB_ADD_TYPE_EMAIL, PNULL);
                //MMIAPIPB_OpenEntryListForEx(&pb_email, MMIPB_ADD_TYPE_EMAIL);
            }
            break;
#endif
#ifdef MMIEMAIL_SUPPORT
        case ID_EXTRACT_SEND_EMAIL_ITEM_ID:
            {
                MMI_STRING_T email_send_to = {0};

                email_send_to.wstr_len = email_len;
                email_send_to.wstr_ptr = (wchar *)email_ptr;

                MMIAPIEMAIL_EditEmail(&email_send_to);
            }
            break;
#endif
            
        default :
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :Create extract Pop Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateExtractPopWin(MMI_WIN_ID_T win_id,
                                                            ADD_DATA add_data_ptr,
                                                            uint16 data_len,
                                                            MN_DUAL_SYS_E dual_sys,
                                                            MMI_MENU_GROUP_ID_T group_id,
                                                            MMI_CTRL_ID_T ctrl_id,
                                                            PROCESSMSG_FUNC func)
{
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    void *ctrl_ptr = PNULL;
    void *data_ptr = PNULL;

    if (PNULL == add_data_ptr || 0 == data_len)
    {
        return;
    }

    data_ptr = SCI_ALLOC_APPZ((data_len + 1)*sizeof(wchar));

    if (PNULL == data_ptr)
    {
        return;
    }

    MMI_WSTRNCPY((wchar *)data_ptr, data_len, (wchar *)add_data_ptr, data_len, data_len);

    MMK_CloseWin(win_id);
    
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = win_id;
    win_create.func = func;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ptr = (ADD_DATA)data_ptr;
    win_create.add_data_ex_ptr = (ADD_DATA)dual_sys;
    
    MMK_CreateWindow( &win_create );

    {
        //create softkey
        GUISOFTKEY_INIT_DATA_T	softkey_data = {0};
        MMI_CONTROL_CREATE_T softkey_create = {0};
        MMI_CTRL_ID_T softkey_ctrl_id = 0;

        softkey_data.leftsoft_id	= TXT_COMMON_OK;
        softkey_data.midsoft_id = TXT_NULL;
        softkey_data.rightsoft_id = STXT_RETURN;

        softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
        softkey_create.ctrl_id = softkey_ctrl_id;
        softkey_create.guid = SPRD_GUI_SOFTKEY_ID;
        softkey_create.parent_win_handle = win_id;
        softkey_create.init_data_ptr = &softkey_data;

        MMK_CreateControl( &softkey_create );
        MMK_SetWinSoftkeyCtrlId(win_id, softkey_ctrl_id);
    } 

    init_data.is_static = TRUE;
#ifdef MMI_PDA_SUPPORT
    init_data.menu_style = GUIMENU_STYLE_POPUP_AUTO;
#else
    init_data.menu_style = GUIMENU_STYLE_POPUP;
#endif
    init_data.group_id = group_id;
    //init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);

    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    ctrl_ptr = MMK_CreateControl( &create );
}

/*****************************************************************************/
//  Description :Extract Number Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_OpenExtractNumberWin(MMI_WIN_ID_T win_id, wchar * num_ptr, uint16 num_len, MN_DUAL_SYS_E dual_sys)
{
    CreateExtractPopWin(win_id, num_ptr, num_len, dual_sys, MENU_EXTRACT_NUMBER_POP_MENU, MMI_EXTRACT_NUMBER_POP_CTRL_ID, HandleExtractNumberPopWinMsg);
}

/*****************************************************************************/
//  Description :Extract Url Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_OpenExtractUrlWin(MMI_WIN_ID_T win_id, wchar * url_ptr, uint16 url_len, MN_DUAL_SYS_E dual_sys)
{
    CreateExtractPopWin(win_id, url_ptr, url_len, dual_sys, MENU_EXTRACT_URL_POP_MENU, MMI_EXTRACT_URL_POP_CTRL_ID, HandleExtractUrlPopWinMsg);
}

/*****************************************************************************/
//  Description :Extract Email Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_OpenExtractEmailWin(MMI_WIN_ID_T win_id, wchar * email_ptr, uint16 email_len, MN_DUAL_SYS_E dual_sys)
{
    CreateExtractPopWin(win_id, email_ptr, email_len, dual_sys, MENU_EXTRACT_EMAIL_POP_MENU, MMI_EXTRACT_EMAIL_POP_CTRL_ID, HandleExtractEmailPopWinMsg);
}



/*Edit by script, ignore 3 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
