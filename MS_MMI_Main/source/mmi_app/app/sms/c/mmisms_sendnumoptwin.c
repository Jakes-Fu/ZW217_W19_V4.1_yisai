/****************************************************************************
** File Name:      mmisms_sendnumoptwin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012       rong.gu     Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmisms_order.h"
#include "mmisms_send.h"
#include "mmisms_internal.h"
#include "mmisms_commonui.h"
#include "mmipub.h"
#include "mmisms_send.h"
#include "mmismsapp_interface.h"
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#include "mmicc_export.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct
{
    MN_DUAL_SYS_E dual_sys;                                 //卡
    MMISMS_SMSEDIT_CONTENT_T edit_content;                      //dest infomation
    MMIVC_SMS_SEND_TYPE_E  send_type;
	MMI_HANDLE_T win_handle;
} MMISMS_SENDNUMWIN_DATA_T;

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//     Description : to handle the message of editwin      
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendNumOptWinMsg(
                                           MMI_WIN_ID_T        win_id,        //IN:
                                           MMI_MESSAGE_ID_E msg_id,        //IN:
                                           DPARAM            param        //IN:
                                           );

/*****************************************************************/
//  Description :发送菜单选择联系人的callback
//  Global resource dependence : none
//  Author: 
//  Note: 
//  Date: 
/*****************************************************************/
LOCAL void SendNumOptSelectContactCallback(MMIPB_HANDLE_T handle); 

/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_GetNumberFromPb( 
                              MMISMS_NUMBER_LIST_T *dest_list_ptr,
                              MMIPB_HANDLE_T handle,
                              MMI_MESSAGE_ID_E    msg_id    //IN:
                              );
						  
/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_GetNumberFromEditbox(                            //RETURN: 
                                   MMISMS_DEST_ADDR_INFO_T *dest_info_ptr,
                                   MMI_CTRL_ID_T ctrl_id,     //IN:
                                   BOOLEAN  is_delete_invalid
                                   );

/*****************************************************************************/
//     Description : load number edit box
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_AddNumber2Edit(
                          MMISMS_NUMBER_LIST_T *dest_list,  
                          MMI_CTRL_ID_T    ctrl_id
                          );
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
// the window of 2.1.1.1 for destination option
WINDOW_TABLE( MMISMS_SENDNUMOPTION_WIN_TAB ) = 
{
    WIN_TITLE( TXT_COMMON_RECIPIENT ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_FUNC((uint32) HandleSendNumOptWinMsg ),    
    WIN_ID( MMISMS_SENDNUMOPTION_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE),
    WIN_SOFTKEY(TXT_SEND, TXT_NULL, STXT_MAIN_PB),
#else
    WIN_SOFTKEY(STXT_MAIN_PB, TXT_SEND, STXT_RETURN),
#endif
    CREATE_EDIT_LIST_CTRL(MMISMS_DEST_ADDR_MAX_NUM,MMISMS_PBNUM_MAX_LEN,MMISMS_INPUTNUM_EDITBOX_CTRL_ID),
    END_WIN        
};



/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/\
LOCAL void SMS_SEND(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T  win_handle)
{

    MMISMS_SENDNUMWIN_DATA_T *win_data_ptr = (MMISMS_SENDNUMWIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(win_data_ptr == PNULL)
    {
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif
    
    MMISMSAPP_SendSmsWithContent(dual_sys,
            FALSE,
            &win_data_ptr->edit_content,
            PNULL
            );
}


/*****************************************************************************/
//     Description : to handle the message of editwin      
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendNumOptWinMsg(
                                           MMI_WIN_ID_T        win_id,        //IN:
                                           MMI_MESSAGE_ID_E msg_id,        //IN:
                                           DPARAM            param        //IN:
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_INPUTNUM_EDITBOX_CTRL_ID;
    //BOOLEAN is_vcard = FALSE;
    MMIVC_SMS_SEND_TYPE_E  send_type=MMIVC_SMS_SEND_NORMAL;
#if 0
// no use
    static BOOLEAN s_is_opt_pb = FALSE; //是否对PB做过操作
#endif
    MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;
    uint16 max_selected_num= 0;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
#ifndef MMI_PDA_SUPPORT
    uint16 num_count = 0;
#endif
#if 0
    if(NULL != (uint32)MMK_GetWinAddDataPtr(win_id))
    {
        send_type = (uint32)MMK_GetWinAddDataPtr(win_id);
    }
#endif

    MMISMS_SENDNUMWIN_DATA_T *win_data_ptr = (MMISMS_SENDNUMWIN_DATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("HandleSendNumOptWinMsg win_data_ptr pnull");
        return recode;
    }
    else
    {
        send_type = win_data_ptr->send_type;
    }
    
    //SCI_TRACE_LOW:"HandleSendNumOptWinMsg, send_type = %d, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17320_112_2_18_2_54_56_790,(uint8*)"dd", send_type, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if 0
// no use
        s_is_opt_pb = FALSE;
#endif
        //+CR225110
        #ifdef MAINLCD_SIZE_128X64
        {
                GUI_RECT_T title_rect = {0};
                GUI_RECT_T edit_rect = {0};
                
                title_rect.left = 0;
                title_rect.top = 0;
                title_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
                title_rect.bottom = MMI_STATUSBAR_HEIGHT-1;

                GUIWIN_SetTitleRect(win_id, title_rect);
                GUIWIN_SetTitleBackground(win_id, 0,  MMI_WHITE_COLOR);

                edit_rect = MMITHEME_GetClientRect();
                edit_rect.top = MMI_STATUSBAR_HEIGHT;
                GUIEDIT_SetRect(ctrl_id, &edit_rect);
        }
        #else
        //set font
        GUIEDIT_SetFont(MMISMS_INPUTNUM_EDITBOX_CTRL_ID,SONG_FONT_29);
        #endif
        //-CR225110

        //显示目的号码信息
        MMISMS_AddNumber2Edit(&win_data_ptr->edit_content.dest_info.dest_list,ctrl_id);

#ifndef MMI_PDA_SUPPORT
        GUIEDIT_SetSoftkey(
                                        MMISMS_INPUTNUM_EDITBOX_CTRL_ID,
                                        1,
                                        1,
                                        TXT_NULL,
                                        TXT_SEND,
                                        PNULL
                                        );
#endif

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MMISMS_MSG_UPDATE_NUMBER:
        MMISMS_AddNumber2Edit(&win_data_ptr->edit_content.dest_info.dest_list,ctrl_id);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
    if (PNULL != param)
    {
        uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
        
        switch (src_id)
        {
        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            break;
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_CTL_MIDSK, PNULL);
            break;          
        default:
            break;
        }

    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMISMS_GetNumberFromEditbox(&win_data_ptr->edit_content.dest_info,ctrl_id,FALSE);
        MMISMS_SetEditContentInfo(&win_data_ptr->edit_content);    //解决显示空号码问题
        win_data_ptr->dual_sys = MMISMS_GetDestDualSys();

//        MMISMS_SetEditContentInfo(win_data_ptr->edit_content);  
        // open the input number window
        if (0 < win_data_ptr->edit_content.dest_info.dest_list.number)
        {   
            // send all message
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            uint16 sim_num = 0;
            uint16 sim_ok = 0;

            sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

            if (0 == sim_num)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (1 == sim_num)
            {
                win_data_ptr->dual_sys = (MN_DUAL_SYS_E)sim_ok;

                if (MMIVC_SMS_SEND_NORMAL==send_type)
                {
                    SMS_SEND(win_data_ptr->dual_sys,win_id);
                }
#if defined MMI_VCARD_SUPPORT
                else if(MMIVC_SMS_SEND_VCARD==send_type)
                {
                    send_err_code = MMISMS_WintabSendVcard(win_data_ptr->dual_sys);
                    MMISMS_DisplaySendErr(win_data_ptr->dual_sys, send_err_code );
                }
#endif
#ifdef MMI_VCALENDAR_SUPPORT
                else if(MMIVC_SMS_SEND_VCALENDAR==send_type)
                {
                    send_err_code = MMISMS_WintabSendVcalendar(win_data_ptr->dual_sys);
                    MMISMS_DisplaySendErr(win_data_ptr->dual_sys, send_err_code );
                }
#endif
                MMK_CloseWin(win_id);
            }
            else
            {
                if (win_data_ptr->dual_sys == MN_DUAL_SYS_MAX)
                {
                    MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
                     if(major_select_sim<MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                    {
                        win_data_ptr->dual_sys = MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1);
                    }    
                    else
                    {
#ifdef MMI_SMS_FIX_SEND_SIM
                        if (MMISMS_GetFixSIMSendingOnOFF())
                        {
                            win_data_ptr->dual_sys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
                        }
                        else
#endif
                        {                            
                            MMISMS_SelectSIMFunc(MultiSendSelectSimCallback,(ADD_DATA)send_type,TRUE);
                            return MMI_RESULT_TRUE;
                        }    
                    
                    }                   
                }
                else
                {
                    win_data_ptr->dual_sys = MMISMS_GetDestDualSys();
                }
                
                if (MMIVC_SMS_SEND_NORMAL==send_type)
                {
                    SMS_SEND(win_data_ptr->dual_sys,win_id);
                }
#if defined MMI_VCARD_SUPPORT
                else if(MMIVC_SMS_SEND_VCARD==send_type)
                {
                    send_err_code = MMISMS_WintabSendVcard(win_data_ptr->dual_sys);
                    MMISMS_DisplaySendErr(win_data_ptr->dual_sys, send_err_code );
                }
#endif
#ifdef MMI_VCALENDAR_SUPPORT
                else if(MMIVC_SMS_SEND_VCALENDAR==send_type)
                {
                    send_err_code = MMISMS_WintabSendVcalendar(win_data_ptr->dual_sys);
                    MMISMS_DisplaySendErr(win_data_ptr->dual_sys, send_err_code );
                }
#endif
                MMK_CloseWin(win_id);

                
            }
#else
            if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else
            {
                win_data_ptr->dual_sys = MN_DUAL_SYS_1;
                if (MMIVC_SMS_SEND_NORMAL==send_type)
                {
                    SMS_SEND(win_data_ptr->dual_sys,win_id);
                }
#if defined MMI_VCARD_SUPPORT
                else if(MMIVC_SMS_SEND_VCARD==send_type)
                {
                    send_err_code = MMISMS_WintabSendVcard(win_data_ptr->dual_sys);
                    MMISMS_DisplaySendErr(win_data_ptr->dual_sys, send_err_code );
                }
#endif
#ifdef MMI_VCALENDAR_SUPPORT
                else if(MMIVC_SMS_SEND_VCALENDAR==send_type)
                {
                    send_err_code = MMISMS_WintabSendVcalendar(win_data_ptr->dual_sys);
                    MMISMS_DisplaySendErr(win_data_ptr->dual_sys, send_err_code );
                }
#endif
                MMK_CloseWin(win_id);
            }
#endif
        }
        else
        {       
            #ifdef MMI_PDA_SUPPORT
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
            #else
            num_count = GUIEDIT_GetListItemNum(ctrl_id);
            if(num_count == 0)
            {
                //MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
                //Add by huang fengming for cr148999
                max_selected_num= MMISMS_DEST_ADDR_MAX_NUM - win_data_ptr->edit_content.dest_info.dest_list.number;

                /*MMIAPIPB_OpenNumberList(
                                    MMIPB_LIST_FOR_SMS,
                                    MMIPB_LIST_CHECK,
                                    max_selected_num,
                                    PNULL
                                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        max_selected_num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        SendNumOptSelectContactCallback
                        );
            }
            #endif
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMISMS_GetNumberFromEditbox(&win_data_ptr->edit_content.dest_info,ctrl_id,TRUE);
        //MMISMS_SetDestInfo(win_data_ptr->dest_info);  
        max_selected_num= MMISMS_DEST_ADDR_MAX_NUM - win_data_ptr->edit_content.dest_info.dest_list.number;
        /*MMIAPIPB_OpenNumberList(
                                MMIPB_LIST_FOR_SMS,
                                MMIPB_LIST_CHECK,
                                max_selected_num,
                                PNULL
                                );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_NUMBER,
                        max_selected_num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        SendNumOptSelectContactCallback
                        );

		//MMISMS_AddNumber2Edit(&win_data_ptr->dest_info.dest_list,ctrl_id);
        break;
        
    case MMI_SELECT_CONTACT_CNF:
#if 0
// no use
        s_is_opt_pb = TRUE;
#endif
        // from PHONEBOOK, call the API to get the phone number.
        if (MMISMS_GetNumberFromPb(&win_data_ptr->edit_content.dest_info.dest_list,(MMIPB_HANDLE_T)param,msg_id))
        {
            // update the edit box
            MMISMS_SetEditContentInfo(&win_data_ptr->edit_content);  
            MMISMS_AddNumber2Edit(&win_data_ptr->edit_content.dest_info.dest_list,ctrl_id);
            //total_item_num=GUILIST_GetTotalItemNum(ctrl_id);
               //GUILIST_SetCurItemIndex(ctrl_id, (total_item_num-1));
        }
        break;
      
    case MSG_CLOSE_WINDOW:
//        if ((MMIVC_SMS_SEND_VCARD==send_type) && s_is_opt_pb)
//        {
//            MMIPB_CloseVCardWin();
//         }
        MMISMS_GetNumberFromEditbox(&win_data_ptr->edit_content.dest_info,ctrl_id,TRUE);
        MMISMS_SetEditContentInfo(&win_data_ptr->edit_content);  
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
            
    case MSG_PROMPTWIN_OK:        
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMISMS_SetSaveFlag(FALSE);
        send_err_code = MMISMS_WintabSendSMS(FALSE);
        MMISMS_DisplaySendErr( MMISMS_GetCurSendDualSys(), send_err_code );
        break;
        
    case MSG_PROMPTWIN_CANCEL:        
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSendNumOptWin(uint32 send_type)
{
   
    MMI_HANDLE_T    win_handle = PNULL;

    MMISMS_SENDNUMWIN_DATA_T    *windata_p = PNULL;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    windata_p = SCI_ALLOCAZ(sizeof(MMISMS_SENDNUMWIN_DATA_T));
    if (PNULL == windata_p)
    {
        return;
    }
    
    // open edit window
    if (!MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))
    {
        win_handle = MMK_CreateWin((uint32 *)MMISMS_SENDNUMOPTION_WIN_TAB, PNULL);
    }
    else
    {
        MMK_WinGrabFocus(MMISMS_SENDNUMOPTION_WIN_ID);
        win_handle = MMK_ConvertIdToHandle( MMISMS_SENDNUMOPTION_WIN_ID );
    }
    windata_p->win_handle = win_handle;
    windata_p->send_type = (MMIVC_SMS_SEND_TYPE_E)send_type;
    edit_content_ptr = MMISMS_GetEditContentInfoPtr();
    SCI_MEMCPY(&windata_p->edit_content,edit_content_ptr,sizeof(MMISMS_SMSEDIT_CONTENT_T));
    MMK_SetWinUserData(win_handle, windata_p);

    return;
    
}

/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:yongli.tong
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterSendNumOptWinExt(BOOLEAN is_permitted, DPARAM param_ptr)
{
   
    MMI_HANDLE_T    win_handle = PNULL;
    uint32 send_type = 0;

    MMISMS_SENDNUMWIN_DATA_T    *windata_p = PNULL;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    send_type = *(int32 *)param_ptr;
    if(is_permitted)
    {
        windata_p = SCI_ALLOCAZ(sizeof(MMISMS_SENDNUMWIN_DATA_T));
        if (PNULL == windata_p)
        {
        return;
        }
        
        // open edit window
        if (!MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))
        {
        win_handle = MMK_CreateWin((uint32 *)MMISMS_SENDNUMOPTION_WIN_TAB, PNULL);
        }
        else
        {
        MMK_WinGrabFocus(MMISMS_SENDNUMOPTION_WIN_ID);
        win_handle = MMK_ConvertIdToHandle( MMISMS_SENDNUMOPTION_WIN_ID );
        }
        windata_p->win_handle = win_handle;
        windata_p->send_type = (MMIVC_SMS_SEND_TYPE_E)send_type;
        edit_content_ptr = MMISMS_GetEditContentInfoPtr();
        SCI_MEMCPY(&windata_p->edit_content,edit_content_ptr,sizeof(MMISMS_SMSEDIT_CONTENT_T));
        MMK_SetWinUserData(win_handle, windata_p);

    }
    else
    {
    #ifdef MMI_VCARD_SUPPORT
        MMIAPIVCARD_BuildFree();
    #endif
    }
    return;
    
}


/*****************************************************************/
//  Description :发送菜单选择联系人的callback
//  Global resource dependence : none
//  Author: 
//  Note: 
//  Date: 
/*****************************************************************/
LOCAL void SendNumOptSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISMS_SENDNUMOPTION_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}


/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_GetNumberFromPb(
                              MMISMS_NUMBER_LIST_T *dest_list_ptr,
                              MMIPB_HANDLE_T handle,
                              MMI_MESSAGE_ID_E    msg_id    //IN:
                              )
{
    int16 i = 0;
    uint16 count = MMIAPIPB_GetSelectCount(handle);
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    BOOLEAN result = TRUE;
    MMIPB_BCD_NUMBER_T    number = {0};

    SCI_TRACE_LOW("MMISMS: MMISMS_GetNumberFromPb msg_id = 0x%x, count = %d",msg_id,count);
    
    if ((MMI_SELECT_CONTACT_CNF == msg_id))
    {
        if (MMISMS_DEST_ADDR_MAX_NUM < count)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
        }
       for (i=0; i<MIN(MMISMS_DEST_ADDR_MAX_NUM, count); i++)
       {
           SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));

            // read the phone number from phonebook one by one
            MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, i);

            max_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, number.number_len);

            tele_len = MMIAPICOM_GenDispNumber( 
                                                MMIPB_GetNumberTypeFromUint8(number.npi_ton), 
                                                max_len, 
                                                number.number,  
                                                tele_num,
                                                MMISMS_NUMBER_MAX_LEN + 2
                                                );

           MMISMS_SetAddressToMessage(tele_num, tele_len, dest_list_ptr);
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}


/*****************************1************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************1************************************************/
LOCAL BOOLEAN MMISMS_GetNumberFromEditbox(                            //RETURN: 
                                   MMISMS_DEST_ADDR_INFO_T *dest_info_ptr,
                                   MMI_CTRL_ID_T ctrl_id,     //IN:
                                   BOOLEAN  is_delete_invalid
                                   )
{
    BOOLEAN result = FALSE;
    uint8 dest_addr[MMISMS_PBNUM_MAX_LEN+2] = {0};
    uint16 num_count = 0;
    uint16 i = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr = PNULL;

    //MMISMS_ClearDestNum();
    SCI_MEMSET(dest_info_ptr,0,sizeof(MMISMS_DEST_ADDR_INFO_T));
    
    //get number count
    num_count = GUIEDIT_GetListItemNum(ctrl_id);
    if (0 < num_count)
    {
        //alloc
        num_str_ptr = SCI_ALLOC_APP(num_count*sizeof(GUIEDIT_LIST_ITEM_STR_T));

        //get all phone number
        GUIEDIT_GetListItemString(ctrl_id,num_count,num_str_ptr);

        //set dest
        for (i=0; i<num_count; i++)
        {
            if (MMISMS_PBNUM_MAX_LEN < num_str_ptr[i].str_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
            }
            else if(!is_delete_invalid)
            {
                if (num_str_ptr[i].is_valid)
                {
                    if (MMISMS_DEST_ADDR_MAX_NUM >= num_count)
                    {
                        SCI_MEMSET(dest_addr,0, sizeof(dest_addr));
                        MMI_WSTRNTOSTR((uint8 *)dest_addr,MMISMS_PBNUM_MAX_LEN+2,
                        num_str_ptr[i].str_ptr,num_str_ptr[i].str_len,
                        num_str_ptr[i].str_len);

                        MMISMS_SetAddressToMessage((uint8 *)dest_addr,(uint8)strlen((char *)dest_addr), &dest_info_ptr->dest_list);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_INVALID_NUMBER);
                }
            }
            else
            {
                if (MMISMS_DEST_ADDR_MAX_NUM >= num_count)
                {
                    SCI_MEMSET(dest_addr,0, sizeof(dest_addr));
                    MMI_WSTRNTOSTR((uint8 *)dest_addr,MMISMS_PBNUM_MAX_LEN+2,
                    num_str_ptr[i].str_ptr,num_str_ptr[i].str_len,
                    num_str_ptr[i].str_len);

                    MMISMS_SetAddressToMessage((uint8 *)dest_addr,(uint8)strlen((char *)dest_addr), &dest_info_ptr->dest_list);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
                }
            }
        }
        SCI_FREE(num_str_ptr);

        result = TRUE;
    }

	return (result);
}


/*****************************************************************************/
//     Description : load number edit box
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
LOCAL void MMISMS_AddNumber2Edit(
                          MMISMS_NUMBER_LIST_T *dest_list,  
                          MMI_CTRL_ID_T    ctrl_id
                          )
{
    uint8 num_count = dest_list->number;
    //显示目的号码信息
    if (0 < num_count)
    {
        uint16   i = 0;
        uint16   actual_count = 0;
        wchar	*number_wstr_ptr = PNULL;
        GUIEDIT_LIST_ITEM_T *list_item_ptr = PNULL;

        GUIEDIT_ClearAllStr(ctrl_id);

        list_item_ptr = SCI_ALLOC_APP(sizeof(GUIEDIT_LIST_ITEM_T) * num_count);
        SCI_MEMSET(list_item_ptr, 0, (sizeof(GUIEDIT_LIST_ITEM_T) * num_count));

        for (i = 0; i< num_count; i++)
        {
            uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
            uint8 num_len = 0;
            MMIPB_BCD_NUMBER_T black_bcd_num = {0};
            MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
            BOOLEAN find_result = FALSE;
            wchar *number_name_wstr = PNULL;
            MMI_STRING_T number_name_addr = {0};

            SCI_MEMCPY( number,dest_list->dest_addr[i],MMISMS_PBNUM_MAX_LEN+1);
            num_len = strlen((char*)number);
            
            if (num_len > 0)
            {                                                
                number_wstr_ptr = SCI_ALLOC_APP(((MMISMS_PBNUM_MAX_LEN + 2) + 1) * sizeof(wchar));
                SCI_MEMSET(number_wstr_ptr, 0x00, (((MMISMS_PBNUM_MAX_LEN + 2) + 1) * sizeof(wchar)));
                MMI_STRNTOWSTR(number_wstr_ptr, (MMISMS_PBNUM_MAX_LEN + 2), 
                    (const uint8 *)number,(MMISMS_PBNUM_MAX_LEN + 2),num_len);
                MMIAPICOM_GenPartyNumber(number, num_len, &party_num);
                black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
                number_name_wstr = SCI_ALLOC_APP((MMIPB_NAME_MAX_LEN  + 1) * sizeof(wchar));
                SCI_MEMSET(number_name_wstr, 0x00, ((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar)));
                number_name_addr.wstr_ptr = number_name_wstr;
                find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &number_name_addr, MMIPB_NAME_MAX_LEN, FALSE);

                if(find_result && 0 < number_name_addr.wstr_len)
                {//如果匹配名称，则显示名称，不能够修改了
                    list_item_ptr[actual_count].is_object = TRUE;

                    list_item_ptr[actual_count].display_str_ptr = number_name_addr.wstr_ptr;
                    list_item_ptr[actual_count].display_str_len = number_name_addr.wstr_len;
                    list_item_ptr[actual_count].user_str_ptr = number_wstr_ptr;
                    list_item_ptr[actual_count].user_str_len = num_len;
                }
                else
                {
                    list_item_ptr[actual_count].is_object = FALSE;
                    list_item_ptr[actual_count].display_str_ptr = number_wstr_ptr;
                    list_item_ptr[actual_count].display_str_len = num_len;
                    list_item_ptr[actual_count].user_str_ptr = PNULL;
                    list_item_ptr[actual_count].user_str_len = 0;
                    SCI_FREE(number_name_wstr);
                    number_name_wstr = PNULL;
                }                

                actual_count++;
            }
        }

        GUIEDIT_AddListItemArray(ctrl_id, list_item_ptr, actual_count);

        for (i = 0; i < num_count; i++)
        {
            if (PNULL != list_item_ptr[i].display_str_ptr)
            {
                SCI_FREE(list_item_ptr[i].display_str_ptr);
            }
            if (PNULL != list_item_ptr[i].user_str_ptr)
            {
                SCI_FREE(list_item_ptr[i].user_str_ptr);
            }
        }

        SCI_FREE(list_item_ptr);
    }
}

