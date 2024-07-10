/****************************************************************************
** File Name:      mmisms_securitywin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef CMCC_UI_STYLE
#ifndef MMI_SMS_SIMBOX
#define MMI_SMS_SIMBOX
#endif
#endif

#ifdef MMI_SMS_SIMBOX
#include "mmisms_app.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_order.h"
#include "mmisms_simboxwin.h"
#include "mmiota_id.h"
#include "mmisms_send.h"
#include "Mmisms_appcontrol.h"
#include "mmismsapp_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    MMISMS_SIMBOX_DATA_T        s_simbox_msglist;
    MMISMS_ORDER_ID_T cur_read_order_id;
    BOOLEAN     is_open;
}MMISMS_SIMBOX_WINDATA_T;
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
//  Description : to handle the SMS SIMBOX window message
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsSIMBOXWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );
/*****************************************************************************/
//  Description : entre sim box win
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMISMS_EnterSimBoxwin(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description :AppendSIMBOXMsgList
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
//Append search result to list
LOCAL void AppendSIMBOXMsgList(MMISMS_SIMBOX_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T   ctrl_id);

/*****************************************************************************/
//  Description :SIMBOX_UpdateLogList
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void SIMBOX_UpdateLogList(MMISMS_SIMBOX_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T   ctrl_id);

/*****************************************************************************/
//  Description : to handle the SMS SIMBOX window message
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SimboxReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T* callback_data);

WINDOW_TABLE( MMISMS_SIMBOX_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSmsSIMBOXWinMsg),    
    WIN_ID( MMISMS_SIMBOX_WIN_ID ),
    WIN_TITLE(TXT_SMS_SIM_BOX),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SIMBOX_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_VIEW, STXT_RETURN ),
    END_WIN    
};

//init and Search message
LOCAL void SIMBOX_BeginSearch(MMISMS_SIMBOX_WINDATA_T *win_data_ptr,MN_DUAL_SYS_E dual_sys)
{
    SCI_MEMSET(&win_data_ptr->s_simbox_msglist,0x00,sizeof(MMISMS_SIMBOX_DATA_T));
    //Begin Search
    MMISMS_SIMBOX_SearchMatchedItem(&win_data_ptr->s_simbox_msglist,dual_sys);
}
//Append search result to list
LOCAL void AppendSIMBOXMsgList(MMISMS_SIMBOX_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T   ctrl_id)
{
    uint32          i = 0;
    GUILIST_ITEM_T  item = {0};

    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id,win_data_ptr->s_simbox_msglist.matched_count,TRUE);
    //set item style
    item.item_style =  GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
    //item.item_state = GUIITEM_STATE_CONTENT_CHECK;//NEWMS00121925
    for (i=0;i<win_data_ptr->s_simbox_msglist.matched_count;i++)
    {
            GUILIST_AppendItem( ctrl_id, &item );
    }
    
}

//Update search list
LOCAL void SIMBOX_UpdateLogList(MMISMS_SIMBOX_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T   ctrl_id)
{
    AppendSIMBOXMsgList(win_data_ptr,ctrl_id);
}

//Select SIM Callback
LOCAL MMI_RESULT_E  MMISMS_OpenSIMBOXCallBack(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        dual_sys = (MN_DUAL_SYS_E)dual_sys;
        MMISMS_EnterSimBoxwin(dual_sys);
    }

    return recode;
}

/*****************************************************************************/
//  Description : entre sim box win
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMISMS_EnterSimBoxwin(MN_DUAL_SYS_E dual_sys)
{       
    MMI_HANDLE_T win_handle = 0;
    MMISMS_SIMBOX_WINDATA_T *win_data_ptr = PNULL;    
    
    if(MMK_IsOpenWin(MMISMS_SIMBOX_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SIMBOX_WIN_ID);
    }
        
    win_data_ptr = (MMISMS_SIMBOX_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_SIMBOX_WINDATA_T));    
    if(PNULL == win_data_ptr)
    {
       SCI_TRACE_LOW("mmisms win_data_ptr alloc fail"); 
       return;
    }
    SCI_MEMSET(&win_data_ptr->s_simbox_msglist,0x00,sizeof(MMISMS_SIMBOX_DATA_T));
        
    win_handle = MMK_CreateWin((uint32*)MMISMS_SIMBOX_WIN_TAB,(ADD_DATA)dual_sys);
    if(win_handle != PNULL)
    {
        MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);  
    }    
}

//Open SIMBOX.if multi SIM,call Select SIM API
PUBLIC void MMISMS_OpenSIMBOX(void)
{
    // MMI_STRING_T wait_text = {0};
    uint16 sim_ok = 0;
    uint32 sim_ok_num = 0;

        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);

        if (0 == sim_ok_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_ok_num)
        {
            MMK_CreateWin((uint32*)MMISMS_SIMBOX_WIN_TAB,(ADD_DATA)sim_ok);
            MMISMS_EnterSimBoxwin((MN_DUAL_SYS_E)sim_ok);
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
             MMISMS_SelectSIMFunc(MMISMS_OpenSIMBOXCallBack,PNULL,FALSE);
        }
#endif
}

/*****************************************************************************/
//  Description : to handle the SMS SIMBOX window message
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SimboxReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
    MMI_STRING_T msg_content = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint8    num_len = 0;     
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};  
    MMISMS_SIMBOX_WINDATA_T *win_data_ptr = PNULL;
    if(PNULL == callback_data)
    {
        SCI_TRACE_LOW("mmisms SimboxReadSmsCallback data pnull");
        return FALSE;
    }
    win_data_ptr = (MMISMS_SIMBOX_WINDATA_T*)MMK_GetWinUserData(MMISMS_SIMBOX_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr data pnull");
        return FALSE;
    }

    if (win_data_ptr->is_open)
    {
        MMISMS_READ_MSG_T read_msg = {0};
        
        read_msg = MMISMS_GetReadResult();
        MMISMS_EnterShowSmsWin(read_msg);
    }
    else
    {
        // set the init SMS content
        MMISMS_GetSMSContent(&msg_content);
        
        num_len = MMISMS_GetCurNumByOrder(win_data_ptr->cur_read_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
        
        // open edit window
        MMISMS_WriteNewMessage(
            MN_DUAL_SYS_MAX,
            &msg_content,
            FALSE,
            number,
            num_len
            );    
        
        if (PNULL != msg_content.wstr_ptr)
        {
            SCI_FREE(msg_content.wstr_ptr);
            msg_content.wstr_ptr = PNULL;
        }    
    }    
    return TRUE;
}

/*****************************************************************************/
//  Description : to handle the SMS SIMBOX window message
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsSIMBOXWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISMS_SIMBOX_LISTBOX_CTRL_ID;
    MN_DUAL_SYS_E   dual_sys =(MN_DUAL_SYS_E)((uint32)MMK_GetWinAddDataPtr(win_id));
    MMISMS_SIMBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr PNULL");
        return recode;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            {            
                SIMBOX_BeginSearch(win_data_ptr,dual_sys);

                SIMBOX_UpdateLogList(win_data_ptr,ctrl_id);
                if(win_data_ptr->s_simbox_msglist.matched_count==0)
                {
                    GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
                }
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_VIEW, STXT_RETURN, TRUE);
                }
                MMK_SetAtvCtrl(win_id,ctrl_id);
            }
            break;
        case MSG_FULL_PAINT:
            if (0 == GUILIST_GetTotalItemNum(ctrl_id))
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            else
            {
               GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_VIEW, STXT_RETURN, FALSE);
            }
	    break;
            
        case MSG_GET_FOCUS:
                SIMBOX_BeginSearch(win_data_ptr,dual_sys);
                SIMBOX_UpdateLogList(win_data_ptr,ctrl_id);
                MMK_SetAtvCtrl(win_id,ctrl_id);
            break;
        case MSG_CTL_LIST_NEED_ITEM_DATA:
            {
                GUILIST_NEED_ITEM_DATA_T    *need_data_ptr = PNULL;
        
                need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
                need_data_ptr->ctrl_handle = ctrl_id;
                MMIAPISMS_SetSIMBOXMatchItemContent(need_data_ptr,(uint32)win_data_ptr->s_simbox_msglist.order_id_list[need_data_ptr->item_index],FALSE);
            }
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
			{
                uint16 sel_item_index = 0;
                MMISMS_ORDER_ID_T cur_order_id = PNULL;
                sel_item_index = GUILIST_GetCurItemIndex(ctrl_id);
                cur_order_id = win_data_ptr->s_simbox_msglist.order_id_list[sel_item_index];
                if(PNULL == cur_order_id)
                {
                    break;
                }
                win_data_ptr->cur_read_order_id = cur_order_id;
                if(cur_order_id->flag.mo_mt_type != MMISMS_MO_DRAFT)                
                {
                    win_data_ptr->is_open = TRUE;   //Open msg
                   // MMIAPISMS_OpenMatchedItem((uint32)cur_order_id);
                   MMISMSAPP_ReadOneSms(cur_order_id,SimboxReadSmsCallback,PNULL,PNULL);
                }
                else
                {
                    win_data_ptr->is_open = FALSE;   //Edit msg
                    MMISMSAPP_ReadOneSms(cur_order_id,SimboxReadSmsCallback,PNULL,PNULL);
                }
            }
            break;
        case MSG_APP_MENU:
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //Set Back 
            MMK_CloseWin(win_id);    
            break;

        case MSG_CLOSE_WINDOW:
            SCI_FREE(win_data_ptr);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode); 

}                                            

#endif

