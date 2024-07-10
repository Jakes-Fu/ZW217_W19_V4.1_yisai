/****************************************************************************
** File Name:      mmismsm_appcontrol.c                          *
** Author:         liming deng                                             *
** Date:           08/01/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        rong.gu      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmisms_export.h"
#include "watch_sms_main.h"
#include "mmisms_appcontrol.h"
#include "guiurl.h"
#include "mmimms_id.h"
#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
#include "mmicc_export.h"
#include "mmisms_mainmenuwin.h"
#include "mmisms_receive.h"
#include "mmisms_send.h"
#include "mmisms_delete.h"
#include "mmisms_read.h"
#include "mmialarm_export.h"
#include "mmisms_commonui.h"
#include "mmisms_internal.h"
#include "mmisms_save.h"
#include "mmisms_set.h"
#include "mmicl_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmimms_file.h"
#include "mmiudisk_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmidm_export.h"
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#include "mmiss_export.h"
#include "mmidc_export.h"
#include "mmismsapp_interface.h"
#include "mmisms_messageboxwin.h"
#ifdef MMI_OTA_SUPPORT
#include "mmiota_export.h"
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
    BOOLEAN         is_need_play_ring;
    MN_DUAL_SYS_E   dual_sys;
}MMISMS_PLAY_NEW_SMS_T;
        
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;
extern BOOLEAN g_mmisms_is_need_mem_full;
/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_del_all = FALSE;//是否全部删除
LOCAL BOOLEAN s_is_move_all = FALSE;//是否全部移除完毕
LOCAL BOOLEAN s_is_move_single = FALSE;//单条短信是否移除完毕
LOCAL MMISMS_PLAY_NEW_SMS_T s_sms_play_ring_info = {0};//sms播放铃声信息

//w317
LOCAL MMISMS_PRESETFROM_E  s_sms_presetfrom = MMISMS_PRESETFROM_NONE;   //进入preset窗的来源flag
LOCAL MMISMS_DEST_ADDR_INFO_T g_sms_numberinfo_for_send = {0};  //存储发送联系人号码
LOCAL MMISMS_PRESET_BUFF_T s_sms_preset_info = {0};  //存储预置信息

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Fuction DEFINITION
**---------------------------------------------------------------------------*/



/*****************************************************************************/
// Description : answer mms get to list string
// Global resource dependence : 
// Author:minghu.mao
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_AnswerMMSGetToString(MMI_STRING_T *to_str);

/*****************************************************************************/
//     Description : to handle the MSGBOX WIN
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM param
                                      );

/*****************************************************************************/
//  Description : for cr00116469 @tzw
//  Global resource dependence : 
//  Author: tzw
//  Note: 打开sms 主界面窗口
/*****************************************************************************/
PUBLIC void MMISMS_OpenMainMenuWindowExt(void)
{
    MMK_CloseWin( MMISMS_NEWMSG_WIN_ID );
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT 
#ifdef MMI_SMS_CHAT_SUPPORT
	MMISMS_EnterSmsBox(MMISMS_BOX_CHAT);
#endif
#else
    MMISMS_EnterSMSMainMenuWin(); 
#endif    
}


/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_MarkItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num)
{
    uint16        item_pos = 0;
    item_pos = GUILIST_GetCurItemIndex(ctrl_id);
    if (order_id != PNULL && !order_id->flag.is_marked)
    {
        GUILIST_SetTextListMarkable(ctrl_id, TRUE);
        GUILIST_SetMaxSelectedItem(ctrl_id, MMISMS_GetMaxSmsNumInOrder());
        GUILIST_SetSelectedItem(ctrl_id,item_pos,TRUE);
        (*mark_num)++;
        MMISMS_SetCurOrderMarked(order_id,TRUE);
    }
}
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_MarkChatItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num)
{
    uint16        item_pos = 0;
    item_pos = GUILIST_GetCurItemIndex(ctrl_id);
    if (order_id != PNULL && !order_id->flag.is_marked)
    {
        GUILIST_SetTextListMarkable(ctrl_id, TRUE);
        GUILIST_SetMaxSelectedItem(ctrl_id, MMISMS_GetMaxSmsNumInOrder());
        GUILIST_SetSelectedItem(ctrl_id,item_pos,TRUE);
        (*mark_num)++;
        MMISMS_SetChatMsgMarkedInList(item_pos,TRUE);
    }
   
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_CancelMarkChatItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num)
{
    uint16        item_pos = 0;
    item_pos = GUILIST_GetCurItemIndex(ctrl_id);
    if (order_id != PNULL && order_id->flag.is_marked)
    {
        GUILIST_SetSelectedItem(ctrl_id, item_pos, FALSE);
        if(*mark_num > 0)
        {
            (*mark_num)--;
        }
        
        MMISMS_SetChatMsgMarkedInList(item_pos,FALSE);
        #ifndef MMI_PDA_SUPPORT
        if (0 == *mark_num)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
        }
        #endif
    }    
}
#endif

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void MMISMS_CancelMarkItem(MMISMS_ORDER_ID_T order_id,MMI_CTRL_ID_T ctrl_id,uint32* mark_num)
{
    uint16        item_pos = 0;
    item_pos = GUILIST_GetCurItemIndex(ctrl_id);
    if (order_id != PNULL && order_id->flag.is_marked)
    {
        GUILIST_SetSelectedItem(ctrl_id, item_pos, FALSE);
        if(*mark_num > 0)
        {
            (*mark_num)--;
        }
        
        MMISMS_SetCurOrderMarked(order_id,FALSE);
        #ifndef MMI_PDA_SUPPORT
        if (0 == *mark_num)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
        }
        #endif
    }
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkAllItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num,BOOLEAN is_push_mark)
{
    uint16 i = 0;
    GUILIST_SetTextListMarkable(ctrl_id, TRUE);
    GUILIST_SetMaxSelectedItem(ctrl_id, MMISMS_GetMaxSmsNumInOrder());
    for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
    {
        GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
    }
    *mark_num = GUILIST_GetTotalItemNum(ctrl_id);
    #ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    if(is_push_mark)
    {
        MMISMS_SetAllPushMsgMarked(TRUE);            
    }
    else       
    #endif
    {
        MMISMS_SetAllMsgMarked(box_type, TRUE);
    }
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: liming.deng
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_CancelMarkAllItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num,BOOLEAN is_push_mark)
{
    uint16 i = 0;

    for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
    {
        GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
    }
    *mark_num = 0;
    #ifndef MMI_PDA_SUPPORT
    GUILIST_SetTextListMarkable(ctrl_id, FALSE);
    #endif
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    if(is_push_mark)
    {
        MMISMS_SetAllPushMsgMarked(FALSE);            
    }
    else       
#endif
    {
        MMISMS_SetAllMsgMarked(box_type, FALSE);
    }
    
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkSameNumItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num)
{
    uint16 cur_index = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0;
    BOOLEAN is_marked = FALSE;
    uint16 i = 0;
    GUILIST_SetTextListMarkable(ctrl_id, TRUE);
    GUILIST_SetMaxSelectedItem(ctrl_id, MMISMS_GetMaxSmsNumInOrder());
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    
    num_len = MMISMS_GetOriginNumFromOrder(box_type,(uint8 *)number,cur_index);
    //SCI_TRACE_LOW:"MMISMS:Mark num all , num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_13701_112_2_18_2_54_49_772,(uint8*)"d",GUILIST_GetTotalItemNum(ctrl_id));
    
    for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
    {
        is_marked = MMISMS_SetPointedMsgNumMarked(box_type, (uint8 *)number, num_len, i);
    
        if (is_marked)
        {
            GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
            (*mark_num)++;
        }
    }    
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_CancelMarkSameNumItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num)
{
    uint16 cur_index = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 num_len = 0; 
    BOOLEAN is_marked = FALSE;
    uint16 i = 0;
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    //SCI_TRACE_LOW:"MMISMS:Mark num all , num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_13719_112_2_18_2_54_49_773,(uint8*)"d",GUILIST_GetTotalItemNum(ctrl_id));
    
    num_len = MMISMS_GetOriginNumFromOrder(box_type, (uint8 *)number, cur_index);

    for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
    {
        

        is_marked = MMISMS_SetPointedMsgNumMarkedCancel(box_type, (uint8 *)number, num_len, i);
    
        if (is_marked)
        {
            GUILIST_SetSelectedItem(ctrl_id, i, FALSE);
            (*mark_num)--;
        }

        if (*mark_num == 0)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
    #ifdef MMI_PDA_SUPPORT
            MMISMS_SetBoxSoftkeyDisplay(ctrl_id, GUIFORM_CHILD_DISP_HIDE);
    #endif
        }
    }
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkSimItem(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,MN_DUAL_SYS_E mark_sim_id,uint32* mark_num)
{
    BOOLEAN is_marked = FALSE;
    uint16 i = 0;
    GUILIST_SetMaxSelectedItem(ctrl_id, MMISMS_GetMaxSmsNumInOrder());
    for (i = 0; i< GUILIST_GetTotalItemNum(ctrl_id); i++)
    {
        is_marked = MMISMS_SetPointedMsgMarked(box_type, mark_sim_id, MN_SMS_STORAGE_SIM, i);
    
        if (is_marked)
        {
            GUILIST_SetTextListMarkable(ctrl_id, TRUE);
            GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
            (*mark_num)++;
        }
    }
    
    if (0 == *mark_num)
    {
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
    #ifdef MMI_PDA_SUPPORT
        MMISMS_SetBoxSoftkeyDisplay(box_type, GUIFORM_CHILD_DISP_HIDE);
    #endif
    }                
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_MarkOpt(MMI_CTRL_ID_T ctrl_id,MMISMS_BOX_TYPE_E box_type,uint32* mark_num)
{

    MMI_CTRL_ID_T form_ctrl_id = 0;
    switch(box_type)
    {
        case MMISMS_BOX_MT:
        form_ctrl_id = MMISMS_MTBOX_FORM_CTRL_ID;
        break;
        case MMISMS_BOX_SENDSUCC:
        form_ctrl_id = MMISMS_SENDSUCCBOX_FORM_CTRL_ID;
        break;
        case MMISMS_BOX_SENDFAIL:
        form_ctrl_id = MMISMS_SENDFAILBOX_FORM_CTRL_ID;
        break;
        case MMISMS_BOX_NOSEND:
        form_ctrl_id = MMISMS_DRAFTBOX_FORM_CTRL_ID;
        break;
    #if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
        form_ctrl_id = MMISMS_SECURITYBOX_FORM_CTRL_ID;
        break;
    #endif                
    #ifdef MMI_SMS_CHAT_SUPPORT 
        case MMISMS_BOX_CHAT:
        form_ctrl_id = MMISMS_CHATBOX_FORM_CTRL_ID;
        break;
    #endif
        
        default:
        break;
    }            
           
    GUILIST_SetTextListMarkable(ctrl_id, TRUE);

    GUILIST_SetMaxSelectedItem(ctrl_id, MMISMS_GetMaxSmsNumInOrder());
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL); 
    if(MMISMS_BOX_CHAT == box_type)
    {
        GUIFORM_SetChildDisplay(form_ctrl_id, MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);      
        GUIWIN_SetTitleButtonState(MMISMS_CHATBOX_CHILD_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);    
    }
#else 
#ifdef MMI_SMS_CHAT_SUPPORT 
    if(MMISMS_BOX_CHAT == box_type)
    {
        GUISOFTKEY_SetTextId(MMISMS_CHATBOX_CHILD_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID,
							  TXT_MARK_ALL,TXT_DELETE, STXT_CANCEL, 
							  FALSE);               
    }
    else
#endif
    {
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);                
    }
#endif            

}
#endif




/*****************************************************************************/
//  Description :set sort menu current focus item
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetSortMenuFocusItem(MMI_CTRL_ID_T menu_ctrl_id,MMI_MENU_GROUP_ID_T group_id)
{
    GUIMENU_POP_SEL_INFO_T    pop_info = {0};
    MMISMS_ORDERTYPE_E order_type = MMISMS_ORDER_BY_TIME;
    order_type = MMISMS_GetOrderType();
    switch(order_type)
    {
    case MMISMS_ORDER_BY_TIME:
        pop_info.menu_id = MMISMS_MENU_TIME_SORT_ITEM_ID;
        break;
        
    case MMISMS_ORDER_BY_NAME:
        pop_info.menu_id = MMISMS_MENU_NAME_SORT_ITEM_ID;
        break;
        
    case MMISMS_ORDER_BY_CONTENT:
        pop_info.menu_id = MMISMS_MENU_SUBJECT_SORT_ITEM_ID;
        break;
        
    case MMISMS_ORDER_BY_TYPE:
        pop_info.menu_id = MMISMS_MENU_TYPE_SORT_ITEM_ID;
        break;
        
    default:
        //SCI_TRACE_LOW:"sms order type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_31826_112_2_18_2_55_26_879,(uint8*)"d", order_type);
        pop_info.menu_id = MMISMS_MENU_TIME_SORT_ITEM_ID;
        break;
    }
#ifdef     MMI_SMS_CHAT_SUPPORT
    if(
#ifdef MMI_PDA_SUPPORT        
        MMISMS_CHAT_MENUKEY_OPT_MENU_CTRL_ID == menu_ctrl_id || 
#endif        
        MMISMS_CHATBOX_OPT_MENU_CTRL_ID == menu_ctrl_id
        )
    {
        if(order_type != MMISMS_ORDER_BY_NAME)
        {
            pop_info.menu_id = MMISMS_MENU_TIME_SORT_ITEM_ID;
        }        
    }
#endif    
    pop_info.is_static = TRUE;
    pop_info.group_id = group_id;
    pop_info.ctrl_id = menu_ctrl_id;         
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);    
}

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
LOCAL BOOLEAN ReadSmsMessageCenterInfoCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint8    num_len = 0;     
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};  
    MMISMS_ORDER_ID_T cur_order_id=PNULL;

    if (callback_data == PNULL)
    {
        return TRUE;
    }


    cur_order_id = (MMISMS_ORDER_ID_T)callback_data->user_data;
    //打开写消息窗口
    if (PNULL != cur_order_id)
    {
        // get the address of the SMS message 
        num_len = MMISMS_GetCurNumByOrder(cur_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

        //open edit window and set the address to the deliver address
        MMISMS_WriteNewMessage(
                                dual_sys,
                                PNULL,
                                FALSE,
                                number,
                                num_len
                                );
    }
    else
    {
        MN_DUAL_SYS_E origin_dual_sys = MMISMS_GetMtBoxMarkedOriginCard();            
        MMISMS_DEST_ADDR_INFO_T dest_info = {0};
        //open edit window and set the address to the deliver address

        MMISMS_GetMtBoxMarkedDestNum(&dest_info);
        MMISMS_SendGroupSms(origin_dual_sys,PNULL,&dest_info);
    }
    SCI_FREE(cur_order_id);
    MMISMS_ClearOperInfo();
    if (MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    }
    return TRUE;
}

#endif

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_AnswerSms(MMISMS_ORDER_ID_T cur_order_id,MMISMS_BOX_TYPE_E box_type,MMI_CTRL_ID_T cur_ctrl_id,uint32 *marked_sms_num)
{
    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T string_to = {0};
    uint8    num_len = 0;     
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};    
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    //MMISMS_SetIsNeedLoadSaved(FALSE);
    MMI_STRING_T     prompt_str ;
#ifdef MMI_SMS_REPLY_PATH_SUPPORT	
    MMISMS_ORDER_ID_T sms_order = {0};
    sms_order = SCI_ALLOCAZ(sizeof(MMISMS_STATE_T));

    if (MMISMS_GetReplyPathFlag())
    {
        if(cur_order_id != PNULL && (MMISMS_MT_TO_BE_READ == cur_order_id->flag.mo_mt_type || MMISMS_MT_HAVE_READ == cur_order_id->flag.mo_mt_type))
        { 
            if (cur_order_id->flag.is_concatenate_sms)
            {
                MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
                MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMISMS_READSMS_WAIT_WIN_ID, IMAGE_NULL,
                                   ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, PNULL);
            }
            SCI_MEMCPY(sms_order,cur_order_id,sizeof(MMISMS_STATE_T));
            MMISMSAPP_ReadSmsCenter(cur_order_id,ReadSmsMessageCenterInfoCallback,
                    PNULL,
                    (void*)sms_order);

            //g_mmisms_global.is_answer_sms = TRUE;
            MMISMS_SetIsAnswerSms(TRUE);
        }
    }
    else
#endif
    {

        if(0 == *marked_sms_num)
        {
            BOOLEAN is_email_answer = FALSE;
            uint16 number_len = 0;
            
            //打开写消息窗口
            if(cur_order_id != PNULL)
            { 
                if (cur_order_id->flag.msg_type == MMISMS_TYPE_MMS)
                {
                    MMIAPICOM_StrToWstr(cur_order_id->number.sender, wnumber);
                    number_len = MMIAPICOM_Wstrlen(wnumber);
                    if (URL_IsMailtoDefault((URL_CHAR_T const *)wnumber, (int32 *)&number_len))
                    {
                        is_email_answer = TRUE;
                        string_to.wstr_ptr = (wchar*)wnumber;
                        string_to.wstr_len = number_len;
                    }
        
                }
                if (!is_email_answer)
                {
                    num_len = MMISMS_GetCurNumByOrder(cur_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);
                }
        
                //open edit window and set the address to the deliver address
                if (!is_email_answer)
                {
                    MMISMS_WriteNewMessage(
                        dual_sys,
                        PNULL,
                        FALSE,
                        number,
                        num_len
                    );
                }
                else
                {
        #ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMIAPIMMS_SetIsEmailAddrExist(TRUE);
        #endif
                    MMIAPIMMS_AnswerMMS(dual_sys, PNULL, &string_to);
                } 
            } 
            else
            {
                MMISMS_SetSecurityBoxType(MMISMS_BOX_NONE);
                MMISMS_WriteNewMessage(
                    MN_DUAL_SYS_MAX,
                    PNULL,
                    FALSE,
                    PNULL,
                    0
                );         
            }
        }
        else
        {
                MN_DUAL_SYS_E origin_dual_sys = MMISMS_GetMtBoxMarkedOriginCard();
//                MMISMS_SMSEDIT_CONTENT_T* edit_ptr = MMISMS_GetEditContentInfoPtr();
                MMISMS_DEST_ADDR_INFO_T dest_info = {0};
                //open edit window and set the address to the deliver address
                MMISMS_GetMtBoxMarkedDestNum(&dest_info);
                MMISMS_SendGroupSms( origin_dual_sys, PNULL, &dest_info);
                *marked_sms_num = 0;//回复后去掉标记
                GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
                MMISMS_SetBoxSoftkeyDisplay(box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
                MMISMS_SetAllMsgMarked(box_type, FALSE);
        }
    }  
}

/*****************************************************************************/
// Description : answer mms get to list string
// Global resource dependence : 
// Author:minghu.mao
// Note: 
/*****************************************************************************/
LOCAL void MMISMS_AnswerMMSGetToString(MMI_STRING_T *to_str)
{
    wchar  *all_number_wstr   = PNULL;
    uint16   all_number_len   = 0;
    uint16  temp_len = 0;

    if (PNULL == to_str)
    {
        return;
    }
    temp_len =  (MMIMMS_MAX_ADDRESS_LEN * MMIMMS_MAX_RECEIVER_NUM + 1) * sizeof(wchar);
    all_number_wstr = SCI_ALLOCAZ(temp_len);
    if (PNULL == all_number_wstr)
    {
        return;
    }
    //SCI_MEMSET(all_number_wstr, 0, temp_len);

    MMIMMS_SetMtBoxMarkedDestNum(&all_number_wstr, &all_number_len);
    to_str->wstr_ptr = all_number_wstr;
    to_str->wstr_len = all_number_len;
    return;
}

/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_AnswerMMS(MMISMS_ORDER_ID_T  cur_order_id,MMISMS_BOX_TYPE_E box_type,MMI_CTRL_ID_T cur_ctrl_id,uint32 *marked_sms_num)
{
    uint8    num_len = 0;     
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0}; 

    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    MMI_STRING_T string_to = {0};
    MMI_STRING_T string_subject = {0};
    
    if(0 == *marked_sms_num)
    {
        num_len = MMISMS_GetCurNumByOrder(cur_order_id,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

        if(num_len > 0)
        {
#ifdef MMS_SUPPORT
            MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER);
#endif
            string_to.wstr_ptr= (wchar *)SCI_ALLOC_APP(num_len * sizeof(wchar));
            MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)number, num_len, num_len);
            MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)number, num_len, num_len);
            string_to.wstr_len = num_len;
            if (cur_order_id != PNULL)
            {
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);//subject可能分配空间
            }
            MMIAPIMMS_AnswerMMS(dual_sys,&string_subject,&string_to);//释放subject空间
            SCI_FREE(string_to.wstr_ptr);
        }
    }
    else
    {
        MMISMS_AnswerMMSGetToString(&string_to);
        if (PNULL != string_to.wstr_ptr && 0 != string_to.wstr_len)
        {
#ifdef MMS_SUPPORT
            MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER);
#endif
            MMIAPIMMS_AnswerMMS(dual_sys,PNULL,&string_to);//多条回复彩信没有主题

            SCI_FREE(string_to.wstr_ptr);
            *marked_sms_num = 0;//回复后去掉标记
            GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
#ifdef MMI_PDA_SUPPORT
            MMISMS_SetBoxSoftkeyDisplay(box_type, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMISMS_SetAllMsgMarked(box_type, FALSE);
        }            
    }
}

/*****************************************************************************/
//     Description : to operate the delete
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OperateDelorDelAll(MMISMS_ORDER_ID_T  cur_order_id,uint32 marked_sms_num,MMIPUB_HANDLE_FUNC handle_func)
{
    MMI_STRING_T prompt_str = {0};

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif
   
    MMISMS_SetDelAllFlag(TRUE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,handle_func);
    if(0 == marked_sms_num)
    {
        MMISMS_SetCurOrderMarked(cur_order_id,TRUE);
    }
    MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT);  
}



/*****************************************************************************/
//     Description : handle msg box answser sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReplyCall(MMISMS_ORDER_ID_T cur_order_id,uint32 call_type)
{
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;        
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0}; 
    uint8 num_len = 0;
    num_len = MMISMS_GetCurNumByOrder(cur_order_id ,&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

    if (num_len > 0)
    {
        CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

#ifdef BT_DIALER_SUPPORT
        if(CC_CALL_BT_CALL == call_type)
        {
            cc_result = MMIAPICC_MakeBtCall(number, num_len);
        }
        else
#endif
        {
            cc_result = MMIAPICC_MakeCall(
                                                                dual_sys,
                                                                number,
                                                                num_len,
                                                                PNULL,
                                                                PNULL,
                                                                CC_CALL_SIM_MAX,
                                                                call_type,
                                                                PNULL
                                                                );
        }

        SCI_TRACE_LOW("mmisms cc_result %d" ,cc_result);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }
}

/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_AddToPbNew(MMISMS_ORDER_ID_T order_id,uint8 *number_arr)
{
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};

    if(PNULL == order_id)
    {
        return;
    }
    if (MMISMS_TYPE_SMS == order_id->flag.msg_type)
    {        
        MMIAPICOM_GenPartyNumber((uint8 *)number_arr,(uint16)SCI_STRLEN((char *)number_arr), &party_num);
        pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
        pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
        SCI_MEMCPY( 
            pb_bcd.number,  
            party_num.bcd_num, 
            pb_bcd.number_len 
            );
        MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
    }          
#ifdef MMI_SMS_MINI_SUPPORT
    else if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
    {
        MMIAPICOM_GenPartyNumber((uint8 *)order_id->number.sender,(uint16)SCI_STRLEN((char *)order_id->number.sender), &party_num);
        pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
        pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
        SCI_MEMCPY( 
            pb_bcd.number,  
            party_num.bcd_num, 
            pb_bcd.number_len 
            );               
        MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
    }
#else
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT            
    else if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
    {
        MMIAPICOM_GenPartyNumber((uint8 *)order_id->number.sender,(uint16)SCI_STRLEN((char *)order_id->number.sender), &party_num);
        pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
        pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
        SCI_MEMCPY( 
            pb_bcd.number,  
            party_num.bcd_num, 
            pb_bcd.number_len 
            );               
        MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
    }
#endif    
#endif
#ifdef MMI_SMS_MINI_SUPPORT
    MMK_SendMsg(MMISMS_MTBOX_CHILD_WIN_ID, MMISMS_MSG_NEEDUPDATELIST, PNULL);
#endif    
}

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT   
/*****************************************************************************/
//     Description : append list item data
//    Global resource dependence : 
//  Author: rong.gu
//    Note: uint32* mark_num [out]
/*****************************************************************************/
PUBLIC void  MMISMS_AddToPbExist(MMISMS_ORDER_ID_T order_id,uint8 *number_arr)
{
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};

    if(PNULL == order_id)
    {
        return;
    }
    if (MMISMS_TYPE_SMS == order_id->flag.msg_type)
    {
        MMIAPICOM_GenPartyNumber((uint8 *)number_arr,(uint16)SCI_STRLEN((char *)number_arr), &party_num);

    }
    else if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
    {
        MMIAPICOM_GenPartyNumber((uint8 *)order_id->number.sender,(uint16)SCI_STRLEN((char *)order_id->number.sender), &party_num);             
    }   
	pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
    SCI_MEMCPY( 
        pb_bcd.number,  
        party_num.bcd_num, 
        pb_bcd.number_len 
        );                
    MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
}
#endif


#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_AddSenderToBlackList(MMIPB_BCD_NUMBER_T pb_bcd)
{
    uint8  num_ton = 0;
    BOOLEAN is_plus_symbol = FALSE;
    MMI_STRING_T mmi_pb_name = {0};

    num_ton = pb_bcd.npi_ton >> 4;
    if ((0 == pb_bcd.number_len) && (MN_NUM_TYPE_INTERNATIONAL == num_ton))
    {
        is_plus_symbol = TRUE;
    }

    if ((pb_bcd.number_len > 0) || is_plus_symbol)
    {
        mmi_pb_name.wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar));

        if (PNULL != mmi_pb_name.wstr_ptr)
        {
            SCI_MEMSET(mmi_pb_name.wstr_ptr, 0x00, ((MMIPB_NAME_MAX_LEN + 1)));
        }
        else
        {
            return;
        }
        MMIAPIPB_GetNameByNumber(&pb_bcd,
                                 &mmi_pb_name,
                                 MMIPB_NAME_MAX_LEN,
                                 FALSE  //use default name,"no name"
                                );
        MMIAPISET_AddBlacklist(mmi_pb_name, pb_bcd);
        SCI_FREE(mmi_pb_name.wstr_ptr);
    }
    else
    {
        //提示窗口没有号码
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
    }
}
#endif

/*****************************************************************************/
//     Description : to get the origin number form order info
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetCurNumByOrder(                        //RETURN: the length of num_ptr
                                      MMISMS_ORDER_ID_T  cur_order_id, //in
                                      MN_DUAL_SYS_E     *dual_sys_ptr,  //out
                                      uint8                *num_ptr,        //OUT:
                                      uint8                 max_num_len    //IN:                                          
                                      )
{
    uint8 len = 0;
    uint8 i = 0;

    if (PNULL == dual_sys_ptr || PNULL == num_ptr || PNULL == cur_order_id)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetOriginNumByPos dual_sys_ptr num_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_MAIN_1685_112_2_18_2_53_49_532,(uint8*)"");
        return 0;
    }
    
    if (cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        if(MN_NUM_TYPE_ALPHANUMERIC == MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton))
        {
            len = 0;
        }
        else
        {
            len = MMIAPICOM_GenDispNumber(
                MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton),
                MIN(MN_MAX_ADDR_BCD_LEN, cur_order_id->number.bcd_number.number_len),
                cur_order_id->number.bcd_number.number,
                num_ptr,
                max_num_len
                );    
        }
    }
    else
    {
        len = strlen((char *)cur_order_id->number.sender);
        //changed by minghu cr128630
        for (i = 0; i < len; i++)
        {
            if (MMIAPICOM_IsTelephoneNum(*(cur_order_id->number.sender+i)))
            {
                *(num_ptr + i) = *(cur_order_id->number.sender + i);
            }
            else
            {
                len = i;
                break;
            }
        }
        *(num_ptr + len) = 0;
    }
    *dual_sys_ptr = (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys;

    return (len);
}


/*****************************************************************************/
//     Description : to open the msg box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenMsgBox( 
                       MMI_TEXT_ID_T    text_id,    //IN:
                       MMI_IMAGE_ID_T    image_id    //IN:
                       )
{
    MMI_WIN_ID_T alert_win_id = MMISMS_MSGBOX_WIN_ID;
    
    // if SMS box is opened, close it at first.
    if (MMK_IsOpenWin(MMISMS_MSGBOX_WIN_ID))
    {
        MMK_CloseWin(MMISMS_MSGBOX_WIN_ID);
    }

    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleMsgBoxWinMsg);
}


/*****************************************************************************/
//     Description : to handle the MSGBOX WIN
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgBoxWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"MMISMS: HandleMsgBoxWinMsg msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19994_112_2_18_2_55_2_805,(uint8*)"d",msg_id);

    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        if( !MMISMS_GetIsWaitUpdate() )
        {
        #ifdef MMI_TIMERMSG_SUPPORT        
            if(!MMK_IsFocusWin(MMISMS_TIMERMSG_WAIT_WIN_ID))
        #endif
            {
                MMISMS_ClearOperInfo();
            }
        }

        if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
        {            
            if ( (0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_GetCurBoxType())))
            {
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                if(MMISMS_BOX_SECURITY != MMISMS_GetCurBoxType())
#endif                    
                {
                    //不需要提示信箱为空
                    //MMIPUB_OpenAlertWarningWin(TXT_SMS_BOX_EMPTY);
                    MMK_CloseParentWin(MMISMS_MSGBOX_MAIN_WIN_ID);
                }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                else
                {                 
                    //g_mmisms_global.security_box_type = MMISMS_BOX_NONE;
                    MMISMS_SetSecurityBoxType(MMISMS_BOX_NONE);
                }
#endif                
            }
        }

        MMIPUB_CloseWaitWin( MMISMS_UPDATE_LIST_ANIM_WIN_ID );
        MMK_CloseWin(MMISMS_SEND_PROMPT_OPT_WIN_ID);
        MMIAPIALM_CheckAndStartDelayedAlarm();
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//     Description : get delete all flag
//                  TRUE:        do delete all SMS operation  
//                  FALSE:    without do delete all SMS operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetDelAllFlag(void)
{
    return s_is_del_all;
}

/*****************************************************************************/
//     Description : set delete all flag
//                  TRUE:        do delete all SMS operation  
//                  FALSE:    without do delete all SMS operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetDelAllFlag(BOOLEAN is_del_all)
{
    s_is_del_all = is_del_all;
}


/*****************************************************************************/
//     Description : get move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetMoveAllFlag(void)
{
    return s_is_move_all;
}

/*****************************************************************************/
//     Description : set move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMoveAllFlag(BOOLEAN is_move_all)
{
    MMIAPISMS_SetSavePush(is_move_all);

    s_is_move_all = is_move_all;
}


/*****************************************************************************/
//     Description : get move single flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move single SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetMoveSingleFlag(void)
{
    //SCI_TRACE_LOW:"MMISMS_GetMoveSingleFlag:s_is_move_single = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24868_112_2_18_2_55_12_835,(uint8*)"d",s_is_move_single);
    return s_is_move_single;
}

/*****************************************************************************/
//     Description : set move single flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move single SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMoveSingleFlag(BOOLEAN is_move_single)
{
    MMIAPISMS_SetSavePush(is_move_single);

    s_is_move_single = is_move_single;
    //SCI_TRACE_LOW:"MMISMS_SetMoveSingleFlag:s_is_move_single = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_24883_112_2_18_2_55_12_836,(uint8*)"d",s_is_move_single);
}


/*****************************************************************************/
//     Description : Get the focus child win id.
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMISMS_GetCurActiveListCtrlId(void)
{
    MMI_CTRL_ID_T cur_list_ctrlid = 0;

    switch(MMISMS_GetCurBoxType())
    {
    case MMISMS_BOX_MT:
        cur_list_ctrlid = MMISMS_MTBOX_LISTBOX_CTRL_ID;
        break;
    case MMISMS_BOX_SENDSUCC:
        cur_list_ctrlid = MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID;
        break;
    case MMISMS_BOX_SENDFAIL:
        cur_list_ctrlid = MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID;
        break;
    case MMISMS_BOX_NOSEND:
        cur_list_ctrlid = MMISMS_NOSENDBOX_LISTBOX_CTRL_ID;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BOX_SECURITY:
        cur_list_ctrlid = MMISMS_SECURITYBOX_LISTBOX_CTRL_ID;        
        break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT        
    case MMISMS_BOX_CHAT:
        cur_list_ctrlid = MMISMS_CHATBOX_LISTBOX_CTRL_ID;        
        break;       
#endif

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    case MMISMS_BT_BOX_MT:
        cur_list_ctrlid = MMISMS_BT_MTBOX_LISTBOX_CTRL_ID;
        break;
    case MMISMS_BT_BOX_SENDSUCC:
        cur_list_ctrlid = MMISMS_BT_SENDSUCCBOX_LISTBOX_CTRL_ID;
        break;
    case MMISMS_BT_BOX_SENDFAIL:
        cur_list_ctrlid = MMISMS_BT_SENDFAILBOX_LISTBOX_CTRL_ID;
        break;
    case MMISMS_BT_BOX_NOSEND:
        cur_list_ctrlid = MMISMS_BT_NOSENDBOX_LISTBOX_CTRL_ID;
        break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    case MMISMS_BT_BOX_SECURITY:
        cur_list_ctrlid = MMISMS_BT_SECURITYBOX_LISTBOX_CTRL_ID;        
        break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT        
    case MMISMS_BT_BOX_CHAT:
        cur_list_ctrlid = MMISMS_BT_CHATBOX_LISTBOX_CTRL_ID;        
        break;       
#endif
#endif

    default:
        break;
    }

    return cur_list_ctrlid;
}

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_HandleSendOperation(
                               MN_DUAL_SYS_E    dual_sys,
                               MMI_WIN_ID_T    win_id,    //IN:
                               BOOLEAN        is_re_send 
                               )
{
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
    MMINV_SMS_AUTOSAVE_T status = MMISMS_GetAutoSave();
#else
    MMINV_SMS_AUTOSAVE_T status = MMINV_SMS_SETTING_SAVE;
#endif
    MMISMS_OPER_ERR_E send_err_code = MMISMS_SENDSMS_FAIL;

    //SCI_TRACE_LOW:"MMISMS_HandleSendOperation: win_id = 0x%x,is_re_send = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17818_112_2_18_2_54_57_793,(uint8*)"dd",win_id,is_re_send);

#ifdef MMI_SMS_CHAT_SUPPORT 
    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
    {
        status = MMINV_SMS_SETTING_SAVE;
        GUIEDIT_ClearAllStr(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
    }
#endif	

    if (is_re_send)
    {
        MMISMS_SetIsSendFail(TRUE);
    }
    else
    {
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_TIMERMSG_CheckTimerSend(TRUE))
        {
            //SCI_TRACE_LOW:"MMISMS_HandleSendOperation: timer sms"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17837_112_2_18_2_54_58_794,(uint8*)"");
            send_err_code = MMISMS_SYSTEM_BUSY;
            return send_err_code;
        }    
#endif
        MMISMS_SetIsSendFail(FALSE);
    }
    
    switch (status)
    {
    case MMINV_SMS_SETTING_SAVE:
        if(MMISMS_GetDeliverResendFlag())
        {
            MMISMS_SetSaveFlag(FALSE);
        }
        else
        {
            MMISMS_SetSaveFlag(TRUE);
        }
        //判断是否空间不足，不足的话直接不能够保存，但是可以发送
        if (!MMISMS_HaveEnoughSpace(dual_sys, TRUE, 0))
        {
            MMISMS_SetSaveFlag(FALSE);
        }

        // if there have enough space, send and save the SMS immediately.
        send_err_code = MMISMS_WintabSendSMS(is_re_send);
        MMISMS_DisplaySendErr(dual_sys, send_err_code);
        MMISMS_CloseEditWin();
        break;

    case MMINV_SMS_SETTING_NOTSAVE:
        MMISMS_SetSaveFlag(FALSE);
        send_err_code = MMISMS_WintabSendSMS(is_re_send);
        MMISMS_DisplaySendErr(dual_sys ,send_err_code);
        MMISMS_CloseEditWin();
        break;

    case MMINV_SMS_SETTING_PROMPT:
    default:
        MMISMS_EnterSendPromptWin((ADD_DATA)dual_sys);

        break;
    }
    
    return send_err_code;
}

/*****************************************************************************/
//     Description : to send group sms
//    Global resource dependence :
//  Author:rong.gu
//    Note: 用于多个联系人的短信编辑
/*****************************************************************************/
PUBLIC void MMISMS_SendGroupSms(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    MMISMS_DEST_ADDR_INFO_T *dest_info
)
{
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMIMMS_SMS_EDIT_TEXT_CTRL_ID;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
   
    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));

#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_SetIsTimerMode(FALSE);
#endif
    edit_content_ptr = MMISMS_GetEditContentInfoPtr();
    // set destination address
    MMISMS_ClearDestNum();
    if ( dest_info != PNULL)
    {        
        SCI_MEMCPY(&edit_content_ptr->dest_info,dest_info,sizeof(MMISMS_DEST_ADDR_INFO_T));
    }

    //本卡回复
    if (MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS)
            && dual_sys < MMI_DUAL_SYS_MAX
            && MMIAPIPHONE_IsSimAvailable(dual_sys))
    {
        edit_content_ptr->dual_sys = dual_sys;
    }
    else
    {
        edit_content_ptr->dual_sys = MN_DUAL_SYS_MAX;
    }

    //MMISMS_SetEditContentInfo(edit_content);
    
    // open edit window
    if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))//短彩合一编辑界面
    {
        MMISMS_CloseEditWin();
    }
    {
        MMIAPIMMS_SetIsMMSMode(FALSE);
#ifdef MMS_SUPPORT
        MMIAPIMMS_CreateMMSEditWin();   
#endif
    }    
   
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    GUIEDIT_SetFont(ctrl_id, MMISMS_GetWinFont());
    GUIEDIT_SetFont(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMISMS_GetWinFont());
#endif

    MMISMS_SetEditTextProperty(ctrl_id);

    // set init SMS content to edit_box
    if (PNULL != sms_init_ptr)
    {
        //有编辑的短消息内容
        // set the parameter sms_init_ptr as SMS content
        MMISMS_SetMessageContent(sms_init_ptr->wstr_len, sms_init_ptr->wstr_ptr, TRUE, edit_content_ptr);
        //MMISMS_SetEditContentInfo(edit_content);
         
        GUIEDIT_SetString(ctrl_id,
                          sms_init_ptr->wstr_ptr,
                          sms_init_ptr->wstr_len);

        if (sms_init_ptr->wstr_len > MMISMS_MAX_MESSAGE_LEN)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SMS_TOO_LONG_MSG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        }
    }
#else
    MMISMS_SendGroupSmsEx(dual_sys,sms_init_ptr,dest_info);
#endif
}


/*****************************************************************************/
//     Description : to enter the window for writing SMS
//    Global resource dependence :
//  Author:louis.wei
//    Note:  单个联系人或无联系人短信编辑
/*****************************************************************************/
PUBLIC void MMISMS_WriteNewMessage(
    MN_DUAL_SYS_E   dual_sys,           //IN:
    MMI_STRING_T    *sms_init_ptr,        //IN:
    BOOLEAN            need_load_saved,    //IN:    TRUE, load the saved content
    uint8            *dest_addr_ptr,        //IN:
    uint8            dest_addr_len        //IN:
)
{
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMIMMS_SMS_EDIT_TEXT_CTRL_ID;
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
   
    //SCI_MEMSET(content_r8_ptr,0x0,(sizeof(wchar) * ( MMISMS_R8_MESSAGE_LEN + 1 )));

#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_SetIsTimerMode(FALSE);
    MMISMS_SetIsTimerMsgReplace(FALSE);
#endif
    edit_content_ptr = MMISMS_GetEditContentInfoPtr();
    // set destination address
    MMISMS_ClearDestNum();
    if ( (0 < dest_addr_len) && (PNULL != dest_addr_ptr) )
    {        
        MMISMS_SetAddressToMessage(dest_addr_ptr, dest_addr_len, &edit_content_ptr->dest_info.dest_list);
    }

    //本卡回复
    if (MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS)
            && dual_sys < MMI_DUAL_SYS_MAX
            && MMIAPIPHONE_IsSimAvailable(dual_sys))
    {
        edit_content_ptr->dual_sys = dual_sys;
    }
    else
    {
        edit_content_ptr->dual_sys = MN_DUAL_SYS_MAX;
    }

    //MMISMS_SetEditContentInfo(edit_content);
    
    // open edit window
    if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))//短彩合一编辑界面
    {
        MMISMS_CloseEditWin();
    }
    {
        MMIAPIMMS_SetIsMMSMode(FALSE);
#ifdef MMS_SUPPORT
        MMIAPIMMS_CreateMMSEditWin();   
#endif
    }    

   
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    GUIEDIT_SetFont(ctrl_id, MMISMS_GetWinFont());
    GUIEDIT_SetFont(MMISMS_EDIT_SMS_PHONENUM_CTRL_ID, MMISMS_GetWinFont());
#endif

    MMISMS_SetEditTextProperty(ctrl_id);

    // set init SMS content to edit_box
    if (PNULL != sms_init_ptr)
    {
        //有编辑的短消息内容
        // set the parameter sms_init_ptr as SMS content
        MMISMS_SetMessageContent(sms_init_ptr->wstr_len, sms_init_ptr->wstr_ptr, TRUE, edit_content_ptr);
        //MMISMS_SetEditContentInfo(edit_content);
         
        GUIEDIT_SetString(ctrl_id,
                          sms_init_ptr->wstr_ptr,
                          sms_init_ptr->wstr_len);

        if (sms_init_ptr->wstr_len > MMISMS_MAX_MESSAGE_LEN)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SMS_TOO_LONG_MSG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        }
    }
    else
    {
#ifdef MMI_SMS_MINI_SUPPORT
        if (need_load_saved || MMISMS_GetIsNeedLoadSaved())//该分支给设置完字体大小后重新创建编辑窗口加载内容使用
        {
            MMI_STRING_T    sms_string={0};
            wchar *data_wstr_ptr  = PNULL;

            data_wstr_ptr = SCI_ALLOCAZ((MMISMS_SPLIT_MAX_NUM * MMISMS_DEFAULT_CHAR_LEN + 1) * sizeof(wchar));
            if (PNULL == data_wstr_ptr)
            {
                return;
            }
            
            sms_string.wstr_ptr =data_wstr_ptr;
            if (MMISMS_LoadStoreddMsgBuffer(&sms_string))
            {
                GUIEDIT_SetString(ctrl_id, sms_string->wstr_ptr, sms_string->wstr_len);
            }    
        }
#endif        
    }
#else
    MMISMS_WriteNewMessageEx(
    dual_sys,
    sms_init_ptr,
    need_load_saved,
    dest_addr_ptr,
    dest_addr_len);
#endif
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN SMSForward_ReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_READ_MSG_T *read_msg_ptr=PNULL;
    MMI_STRING_T msg_content = {0};
    BOOLEAN      *is_edit_sms = PNULL;
    uint8   addr_array[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
    uint8   addr_len= 0;     
	

    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    if (PNULL == callback_data
        ||callback_data->opreation_cb==PNULL)
    { 
        SCI_TRACE_LOW("MMISMS callback_data pnull");
        return TRUE;
    }
    read_msg_ptr = (MMISMS_READ_MSG_T *)callback_data->opreation_cb;

    if (callback_data->result == FALSE)
    {
        SCI_FREE(read_msg_ptr);
        SCI_FREE(is_edit_sms);   
        return TRUE;
    }

    is_edit_sms =(BOOLEAN*)callback_data->user_data;

    if(is_edit_sms !=PNULL && *is_edit_sms)
    {
        MN_CALLED_NUMBER_T *address_ptr = &(read_msg_ptr->address);
        
        addr_len = MMIAPICOM_GenDispNumber(
            address_ptr->number_type,
            MIN(MN_MAX_ADDR_BCD_LEN, address_ptr->num_len),
            address_ptr->party_num,
            addr_array,
            MMISMS_MAX_USERNUMBER_LEN + 2
            );
    }
    
    //Now MMIMMS_EDITWIN use global data. so free it.
   
    MMISMS_GetSMSStringFromContent(&read_msg_ptr->read_content,&msg_content);    
    //Open EditSMS Win
    MMISMS_WriteNewMessage(
        MN_DUAL_SYS_MAX,
        &msg_content,
        FALSE,
        addr_array,
        addr_len
        );
    
    SCI_FREE(read_msg_ptr); 
    SCI_FREE(is_edit_sms);   
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_EditSMSbyOrderID(MMISMS_ORDER_ID_T order_id)
{
    if (order_id == PNULL)
    {
        return;
    }

#ifdef MMIMMS_SMS_IN_1_SUPPORT
{
	MMI_STRING_T	prompt_str = {0};
	MMISMS_READ_MSG_T *read_msg_ptr=PNULL;
	BOOLEAN         *is_edit_sms = PNULL;

    read_msg_ptr = SCI_ALLOCAZ(sizeof(MMISMS_READ_MSG_T));
    if (PNULL == read_msg_ptr)
    {
        return;
    }

    is_edit_sms = SCI_ALLOCAZ(sizeof(BOOLEAN));
    if (PNULL == is_edit_sms)
    {
        return;
    }
    *is_edit_sms = TRUE;

    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);

    MMISMSAPP_ReadOneSms(
                order_id,
                SMSForward_ReadSmsCallback,
                (void *)read_msg_ptr,   //action data
                (void *)is_edit_sms);
}
#else
    MMISMS_EnterEditSMSWinByOrderID(order_id);
#endif
}

/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_ForwardSMSbyOrderID(MMISMS_ORDER_ID_T order_id)
{
    if (order_id == PNULL)
    {
        return;
    }

#ifdef MMIMMS_SMS_IN_1_SUPPORT
{
	MMI_STRING_T	prompt_str = {0};
	MMISMS_READ_MSG_T *read_msg_ptr=PNULL;

    read_msg_ptr = SCI_ALLOCAZ(sizeof(MMISMS_READ_MSG_T));
    if (PNULL == read_msg_ptr)
    {
        return;
    }

    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);

    MMISMSAPP_ReadOneSms(
                order_id,
                SMSForward_ReadSmsCallback,
                read_msg_ptr,   //action data
                PNULL);
}    
#else
    MMISMS_EnterEditSMSWinForForward(order_id);
#endif
}


/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN SMSResend_ReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_READ_MSG_T *read_msg_ptr=PNULL;
    MMI_STRING_T msg_content = {0};
    MN_CALLED_NUMBER_T *address_ptr = PNULL;

    uint8   addr_array[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
    uint8   addr_len= 0;     
    MN_DUAL_SYS_E *dual_sys_ptr = PNULL;
    MMISMS_DEST_ADDR_INFO_T  sms_addr = {0};
        
    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    if (!MMISMS_IsSendListEmpty())
    {
        MMISMS_DeleteSendList();
    }
    if (PNULL == callback_data
        ||callback_data->opreation_cb==PNULL)
    { 
        SCI_TRACE_LOW("MMISMS callback_data pnull");
        return TRUE;
    }
    read_msg_ptr = (MMISMS_READ_MSG_T *)callback_data->opreation_cb;

    if (FALSE == callback_data->result || PNULL == read_msg_ptr)
    {
        SCI_FREE(read_msg_ptr);
        return TRUE;
    }
    dual_sys_ptr = (MN_DUAL_SYS_E*) callback_data->user_data;     

    address_ptr = &(read_msg_ptr->address);
    
    addr_len = MMIAPICOM_GenDispNumber(
        address_ptr->number_type,
        MIN(MN_MAX_ADDR_BCD_LEN, address_ptr->num_len),
        address_ptr->party_num,
        addr_array,
        MMISMS_MAX_USERNUMBER_LEN + 2
        );
    
    //Now MMIMMS_EDITWIN use global data. so free it.

    MMISMS_GetSMSStringFromContent(&read_msg_ptr->read_content,&msg_content);    
    MMISMS_SetAddressToMessage((uint8 *)addr_array, addr_len, &sms_addr.dest_list);    
    MMISMSAPP_SendSmsWithData(*dual_sys_ptr,
            TRUE,
            &sms_addr,
            msg_content.wstr_ptr,
            msg_content.wstr_len,
            PNULL
            );
    
    SCI_FREE(read_msg_ptr);
    SCI_FREE(dual_sys_ptr);
    return TRUE;
}


/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_ResendSMSbyOrderID(MMISMS_ORDER_ID_T order_id,MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T	prompt_str = {0};
    MMISMS_READ_MSG_T *read_msg_ptr=PNULL;
    MN_DUAL_SYS_E         *dual_sys_ptr = PNULL;
        
    if (order_id == PNULL)
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

    read_msg_ptr = SCI_ALLOCAZ(sizeof(MMISMS_READ_MSG_T));
    
    if (PNULL == read_msg_ptr)
    {
        return;
    }
    dual_sys_ptr = SCI_ALLOCAZ(sizeof(MN_DUAL_SYS_E));
    
    if (PNULL == dual_sys_ptr)
    {
        SCI_FREE(read_msg_ptr);
        return;
    }

    *dual_sys_ptr = dual_sys;
    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);

    MMISMSAPP_ReadOneSms(
                order_id,
                SMSResend_ReadSmsCallback,
                read_msg_ptr,   //action data
                dual_sys_ptr);

}

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/******************************************************************************
//  Description : Callback of read sms
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL BOOLEAN SMSBtResend_ReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_READ_MSG_T *read_msg_ptr=PNULL;
    MMI_STRING_T msg_content = {0};
    MN_CALLED_NUMBER_T *address_ptr = PNULL;

    uint8   addr_array[MMISMS_MAX_USERNUMBER_LEN + 2] = {0};
    uint8   addr_len= 0;     
    MN_DUAL_SYS_E *dual_sys_ptr = PNULL;
    MMISMS_DEST_ADDR_INFO_T  sms_addr = {0};
        
    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
    if (!MMISMS_IsSendListEmpty())
    {
        MMISMS_DeleteSendList();
    }
    if (PNULL == callback_data
        ||callback_data->opreation_cb==PNULL)
    { 
        SCI_TRACE_LOW("MMISMS callback_data pnull");
        return TRUE;
    }
    read_msg_ptr = (MMISMS_READ_MSG_T *)callback_data->opreation_cb;

    if (FALSE == callback_data->result || PNULL == read_msg_ptr)
    {
        SCI_FREE(read_msg_ptr);
        return TRUE;
    }
    dual_sys_ptr = (MN_DUAL_SYS_E*) callback_data->user_data;     

    address_ptr = &(read_msg_ptr->address);
    
    addr_len = MMIAPICOM_GenDispNumber(
        address_ptr->number_type,
        MIN(MN_MAX_ADDR_BCD_LEN, address_ptr->num_len),
        address_ptr->party_num,
        addr_array,
        MMISMS_MAX_USERNUMBER_LEN + 2
        );
    
    //Now MMIMMS_EDITWIN use global data. so free it.

    MMISMS_GetSMSStringFromContent(&read_msg_ptr->read_content,&msg_content);
    MMISMS_SetMessageContent(msg_content.wstr_len, msg_content.wstr_ptr, FALSE, &g_mmisms_global.edit_content);

    MMISMS_SetAddressToMessage((uint8 *)addr_array, addr_len, &g_mmisms_global.edit_content.dest_info.dest_list);
    MMIAPISMS_SetSendAddrCount(g_mmisms_global.edit_content.dest_info.dest_list.number);

    MMISMS_BtSendItem(TRUE);

    SCI_FREE(read_msg_ptr);
    SCI_FREE(dual_sys_ptr);
    return TRUE;
}

/*****************************************************************************/
//  Description : to enter the window for edit SMS
//  Global resource dependence :
//  Author:louis.wei
//  Note: interface for other module
/*****************************************************************************/
PUBLIC void MMISMS_BtResendSMSbyOrderID(MMISMS_ORDER_ID_T order_id,MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T	prompt_str = {0};
    MMISMS_READ_MSG_T *read_msg_ptr=PNULL;
    MN_DUAL_SYS_E         *dual_sys_ptr = PNULL;
    
    if (order_id == PNULL)
    {
        return;
    }
    
    read_msg_ptr = SCI_ALLOCAZ(sizeof(MMISMS_READ_MSG_T));
    
    if (PNULL == read_msg_ptr)
    {
        return;
    }
    dual_sys_ptr = SCI_ALLOCAZ(sizeof(MN_DUAL_SYS_E));
    
    if (PNULL == dual_sys_ptr)
    {
        SCI_FREE(read_msg_ptr);
        return;
    }
    
    *dual_sys_ptr = dual_sys;
    
    MMISMSAPP_ReadOneSms(
        order_id,
        SMSBtResend_ReadSmsCallback,
        read_msg_ptr,   //action data
        dual_sys_ptr);
    
}
#endif

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsForbitNewMsgWin(BOOLEAN is_lockwin_show_prompt)
{
    if( MMIAPIUdisk_IsOpenPcCamera() 
#ifdef MMI_RECORD_SUPPORT
        || MMIAPIRECORD_IsRecording() 
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsOpenPlayerWin()
        || MMIAPIVP_IsLiving()
#ifdef CMCC_TEST_FLAG
        || MMIAPIVP_IsPlayWinFocus()
#endif
#endif  
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
        || MMIAPIDM_GetHandsetLock()
#if !defined(CMCC_TEST_FLAG)
        || MMIAPIDC_IsOpened()
#endif
        || (MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID) && !is_lockwin_show_prompt)
        || MMIAPIATV_IsRecording()
#ifdef CMCC_UI_STYLE
        || MMIAPICC_IsInState(CC_IN_CALL_STATE)
#else        
#if defined(MMI_PDA_SUPPORT) 
        || (MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef MMI_CALL_MINIMIZE_SUPPORT  
        && !MMIAPICC_IsMinimize()
#endif
        )
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT        
	|| MMK_IsFocusWin(MMIKL_PSW_WIN_ID)
#endif
#endif
#endif
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :sms edit text property
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SetEditTextProperty(MMI_CTRL_ID_T ctrl_id)
{
    uint8 single_ascii_num = 0;
    uint8 long_ascii_num = 0;
    uint8 single_ucs2_num = 0;
    uint8 long_ucs2_num = 0;
    GUIEDIT_TEXT_MAX_T  max_len = {0};
    GUIEDIT_TEXT_PAGE_T sms_page = {0};
    GUIEDIT_SMS_ENCODE_E encode_type = GUIEDIT_SMS_ENCODE_AUTO;
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    MMISMS_ALPHABET_MANAGEMENT_E alphabet_management = MMISMS_GetAlphabetManagement();
#endif

    //set max len
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    if (MMISMS_GetConcatenatedSMS())
#endif
    {
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
        if((MMISMS_ALPHABET_MANAGEMENT_8BIT == alphabet_management) ||
           (MMISMS_ALPHABET_MANAGEMENT_UCS2 == alphabet_management))
        {
            max_len.ascii    = MMISMS_MAX_UCS2_SPLIT_LEN * MMISMS_SPLIT_MAX_NUM;
            max_len.ucs2     = MMISMS_MAX_UCS2_MESSAGE_LEN/sizeof(wchar);
            max_len.r8_1byte = MMISMS_MAX_UCS2_SPLIT_LEN * MMISMS_SPLIT_MAX_NUM - 1;
            max_len.r8_2byte = MMISMS_MAX_UCS2_SPLIT_LEN * MMISMS_SPLIT_MAX_NUM - 2;
        }
        else
#endif
        {
            max_len.ascii    = MMISMS_MAX_MESSAGE_LEN;
            max_len.ucs2     = MMISMS_MAX_UCS2_MESSAGE_LEN/sizeof(wchar);
            max_len.r8_1byte = MMISMS_R8_MAX_LOCK_SPLIT_LEN*MMISMS_SPLIT_MAX_NUM;
            max_len.r8_2byte = MMISMS_R8_MAX_SINGLE_SPLIT_LEN*MMISMS_SPLIT_MAX_NUM;
        }
    }
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    else
    {
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
        if((MMISMS_ALPHABET_MANAGEMENT_8BIT == alphabet_management) ||
           (MMISMS_ALPHABET_MANAGEMENT_UCS2 == alphabet_management))
        {
            max_len.ascii    = MMISMS_MAX_8BIT_BYTE_LEN;
            max_len.ucs2     = MMISMS_MAX_UCS2_BYTE_LEN/sizeof(wchar);
            max_len.r8_1byte = MMISMS_MAX_8BIT_BYTE_LEN - 1;
            max_len.r8_2byte = MMISMS_MAX_8BIT_BYTE_LEN - 2;
        }
        else
#endif
        {
            max_len.ascii    = MMISMS_MAX_DEF_BYTE_LEN;
            max_len.ucs2     = MMISMS_MAX_UCS2_BYTE_LEN/sizeof(wchar);
            max_len.r8_1byte = MMISMS_R8_LOCK_DEFAULT_CHAR_LEN;
            max_len.r8_2byte = MMISMS_R8_SINGLE_DEFAULT_CHAR_LEN;
        }
    }
#endif

    GUIEDIT_SetTextMaxLenEx(ctrl_id,&max_len);
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    if(MMISMS_ALPHABET_MANAGEMENT_8BIT == alphabet_management ||
           MMISMS_ALPHABET_MANAGEMENT_UCS2 == alphabet_management)
    {
        MMISMS_GetLenByAlpha(MN_SMS_UCS2_ALPHABET, &long_ascii_num, &single_ascii_num, FALSE);
    }
    else
#endif        
    {
        MMISMS_GetLenByAlpha(MN_SMS_DEFAULT_ALPHABET, &long_ascii_num, &single_ascii_num, FALSE);
    }
    MMISMS_GetLenByAlpha(MN_SMS_UCS2_ALPHABET, &long_ucs2_num, &single_ucs2_num, FALSE);

    //set page
    sms_page.single_ucs2_len  = (uint16)(single_ucs2_num/2);
    sms_page.single_ascii_len    = single_ascii_num;
    sms_page.multi_ucs2_len   = (uint16)(long_ucs2_num/2);
    sms_page.multi_ascii_len     = long_ascii_num;
    sms_page.single_r8_1byte_len = MMISMS_R8_MAX_LOCK_DEF_LEN;
    sms_page.multi_r8_1byte_len  = MMISMS_R8_MAX_LOCK_SPLIT_LEN;
    sms_page.single_r8_2byte_len = MMISMS_R8_MAX_SINGLE_DEF_LEN;
    sms_page.multi_r8_2byte_len  = MMISMS_R8_MAX_SINGLE_SPLIT_LEN;

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    if (MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET == alphabet_management)
    {
        encode_type = GUIEDIT_SMS_ENCODE_7BIT;
    }
    else if (MMISMS_ALPHABET_MANAGEMENT_8BIT == alphabet_management)
    {
        encode_type = GUIEDIT_SMS_ENCODE_8BIT;
    }
    else if (MMISMS_ALPHABET_MANAGEMENT_UCS2 == alphabet_management)
    {
        encode_type = GUIEDIT_SMS_ENCODE_UCS2;
    }
    else
    {
        encode_type = GUIEDIT_SMS_ENCODE_AUTO;
    }
#endif

    GUIEDIT_SetTextPage(ctrl_id,&sms_page,&encode_type);
}

#ifdef MMI_SMS_CHAT_SUPPORT

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ReadChatMsg(MMISMS_ORDER_ID_T cur_order_id)
{
    MMI_STRING_T   prompt_str = {0};    
    //MMK_CreateWin((uint32 *)MMISMS_SHOW_CHAT_SMS_WIN_TAB,PNULL);
    MMISMS_EnterSMSChatReadWin(cur_order_id);
    
    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
    MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);        
}
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE

/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  MultiSendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIVC_SMS_SEND_TYPE_E  send_type=MMIVC_SMS_SEND_NORMAL;
#if defined(MMI_VCARD_SUPPORT) || defined (MMI_VCALENDAR_SUPPORT)
    MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;
#endif
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();

    if(NULL != (uint32)MMK_GetWinAddDataPtr(win_id))
    {
        send_type = (uint32)MMK_GetWinAddDataPtr(win_id);
    }

    if (isReturnOK)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        MMISMS_SetCurSendDualSys((MN_DUAL_SYS_E)dual_sys);
        // 开始发送消息
        MMISMS_ClearDestId();

        if (MMIVC_SMS_SEND_NORMAL==send_type)
        {
            MMISMS_SetDeliverResendFlag(FALSE);
            MMISMS_HandleSendOperation((MN_DUAL_SYS_E)dual_sys, win_id, FALSE);
            #ifdef MMI_PDA_SUPPORT
            MMK_CloseWin(win_id);
            #endif
        }
#if defined MMI_VCARD_SUPPORT
        else if(MMIVC_SMS_SEND_VCARD==send_type)
        {
            send_err_code = MMISMS_WintabSendVcard((MN_DUAL_SYS_E)dual_sys);
            MMISMS_DisplaySendErr((MN_DUAL_SYS_E)dual_sys, send_err_code );            
            if(MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SENDNUMOPTION_WIN_ID);
            }
            MMK_CloseWin(win_id);
        }
#endif
#if defined MMI_VCALENDAR_SUPPORT
        else if(MMIVC_SMS_SEND_VCALENDAR==send_type)
        {
            send_err_code = MMISMS_WintabSendVcalendar((MN_DUAL_SYS_E)dual_sys);
            MMISMS_DisplaySendErr((MN_DUAL_SYS_E)dual_sys, send_err_code );
            if(MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SENDNUMOPTION_WIN_ID);
            }
            MMK_CloseWin(win_id);
        }
#endif
        else
        {
            MMK_SendMsg(MMISMS_QUERY_WIN_ID, MSG_SMS_SELECTRESULT, PNULL);
#ifdef MMI_PDA_SUPPORT
            MMK_CloseWin(win_id);
#endif
        }
    }    
    return recode;
}

#endif

/*****************************************************************************/
//     Description : Get the focus child win id.
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMISMS_GetSmsFocusWinId(MMISMS_BOX_TYPE_E box_type)
{
    MMI_WIN_ID_T cur_child_winid = 0;

    SCI_TRACE_LOW("[MMISMS] MMISMS_GetSmsFocusWinId box_type = %d", box_type);
    
    switch (box_type)
    {
        case MMISMS_BOX_MT:
            cur_child_winid = MMISMS_MTBOX_CHILD_WIN_ID;
            break;
        case MMISMS_BOX_SENDSUCC:
            cur_child_winid = MMISMS_SENDSUCCBOX_CHILD_WIN_ID;
            break;
        case MMISMS_BOX_SENDFAIL:
            cur_child_winid = MMISMS_SENDFAILBOX_CHILD_WIN_ID;
            break;
        case MMISMS_BOX_NOSEND:
            cur_child_winid = MMISMS_NOSENDBOX_CHILD_WIN_ID;
            break;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_BOX_SECURITY:
            cur_child_winid = MMISMS_SECURITYBOX_CHILD_WIN_ID;
            break;
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
        case MMISMS_BOX_CHAT:
            cur_child_winid = MMISMS_CHATBOX_CHILD_WIN_ID;
            break;
#endif
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
        case MMISMS_BT_BOX_MT:
            cur_child_winid = MMISMS_BT_MTBOX_CHILD_WIN_ID;
            break;
        case MMISMS_BT_BOX_SENDSUCC:
            cur_child_winid = MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID;
            break;
        case MMISMS_BT_BOX_SENDFAIL:
            cur_child_winid = MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID;
            break;
        case MMISMS_BT_BOX_NOSEND:
            cur_child_winid = MMISMS_BT_NOSENDBOX_CHILD_WIN_ID;
        break;
#endif
        default:
            break;
    }

    return cur_child_winid;
}


/*****************************************************************************/
//  Description : phonebook callback
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:  
/*****************************************************************************/
PUBLIC void MMISMS_HandlePBEventFunc(MMIEVENT_PB_EVENT_E event)
{
    if((MMIEVENT_PB_ADD == event)
       || (MMIEVENT_PB_DELETE == event)
       || (MMIEVENT_PB_UPDATE == event))
    {
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT           
        if(MMK_IsOpenWin(MMISMS_CHATBOX_CHILD_WIN_ID))
        {
            MMK_SetWinDirty(MMISMS_CHATBOX_CHILD_WIN_ID,TRUE);
            if(MMK_IsFocusWin(MMISMS_CHATBOX_CHILD_WIN_ID))
            {
                MMK_PostMsg(MMISMS_CHATBOX_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL,0);        
            }    
        }
#else
        if(MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
        {
            MMI_WIN_ID_T cur_focus_win = 0;
            MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;
            box_type = MMISMS_GetCurBoxType();
            cur_focus_win = MMISMS_GetSmsFocusWinId(box_type);
            MMK_SetWinDirty(cur_focus_win,TRUE);
            if(MMK_IsFocusWin(cur_focus_win))
            {
                MMK_PostMsg(cur_focus_win, MSG_FULL_PAINT, PNULL,0);        
            }                                      
        } 
#endif
#ifdef MMI_SMS_CHAT_SUPPORT		
        if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_MSG_UPDATE_NUMBER, PNULL,0);
        }
#endif
        if(MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
        {
            MMK_PostMsg(MMISMS_SHOWMSG_WIN_ID, MMISMS_MSG_UPDATE_NUMBER, PNULL,0);
        }
    }
}


/*****************************************************************************/
//  Description : close edit win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CloseEditWin(void)
{    
#ifdef MMIMMS_SMS_IN_1_SUPPORT 
    MMIAPIMMS_CloseEditWin();
#else
    MMIAPISMS_SetNormalClose(TRUE);
    MMK_CloseWin(MMISMS_EDIT_SMS_WIN_ID);
#endif
    
}


#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : set win font size
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMISMS_GetWinFont(void)
{
    GUI_FONT_T font = GUI_MAX_FONT_NUM;
    MMISMS_FONT_E font_size = MMISMS_GetFontSize();/*lint !e64 */
        
    switch(font_size)
    {
    case MMISMS_FONT_BIG:
        font = MMI_DEFAULT_BIG_FONT;
        break;

    case MMISMS_FONT_NORMAL:
#ifdef DPHONE_SUPPORT 
        font = MMI_DEFAULT_TEXT_FONT;
#else
        font = MMI_DEFAULT_NORMAL_FONT;
#endif 
        break;            

    case MMISMS_FONT_SMALL:
        font = MMI_DEFAULT_SMALL_FONT;
        break;

    default:
        font = MMI_DEFAULT_NORMAL_FONT;
        break;
    }

    //SCI_TRACE_LOW:"MMISMS_GetWinFont: font is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_25217_112_2_18_2_55_12_842,(uint8*)"d",font);
    return font;
}

/*****************************************************************************/
//  Description : reset win with different font size
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ResetWinFont(MMISMS_BOX_TYPE_E box_type)
{
    uint8 num_len = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 number [MMISMS_PBNUM_MAX_LEN + 2] = {0};
#ifdef MMI_TIMERMSG_SUPPORT
    BOOLEAN is_time_mode = FALSE;
//    MMISMS_TIMERMSG_NODE temp_timer_node = {0};
#endif

    if(MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
    {
        GUIRICHTEXT_SetFont(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID, MMISMS_GetWinFont());
        GUIRICHTEXT_Update(MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID);
    }
    #ifdef MMIMMS_SMS_IN_1_SUPPORT 
    if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    #else
    if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
    #endif
    {
        MMI_STRING_T msg_content = {0};
        MMI_STRING_T edit_string = {0};

        GUIEDIT_GetString(MMISMS_EDIT_SMS_TEXT_CTRL_ID, &edit_string);
        // to save the information
        MMISMS_SetMessageContent( 
                                    edit_string.wstr_len,
            edit_string.wstr_ptr, 
            TRUE,
            MMISMS_GetEditContentFromGlobal()
                                    );
        GUIEDIT_ClearAllStr(MMISMS_EDIT_SMS_TEXT_CTRL_ID);

        num_len = MMISMS_GetOriginNum(&dual_sys, number, MMISMS_PBNUM_MAX_LEN + 2);

#ifdef MMI_TIMERMSG_SUPPORT
        is_time_mode = MMISMS_GetIsTimerMode();
        //SCI_MEMCPY(&temp_timer_node,&g_mmisms_global.curr_timer_mode_node,sizeof(MMISMS_TIMERMSG_NODE));//no use code
#endif

        // open edit window
        MMISMS_WriteNewMessage(
                                MN_DUAL_SYS_MAX,
                                PNULL,
                                TRUE,
                                number,
                                num_len
                                );

#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_SetIsTimerMode(is_time_mode);
        //SCI_MEMCPY(&g_mmisms_global.curr_timer_mode_node,&temp_timer_node,sizeof(MMISMS_TIMERMSG_NODE)) //no use code
#endif

        if (PNULL != msg_content.wstr_ptr)
        {
            SCI_FREE(msg_content.wstr_ptr);
            msg_content.wstr_ptr = PNULL;
        }
    }

    return;
}
#endif

/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MEHaveEnoughSpace(void)
{
    uint8 org_num = 0;
    uint8 total_num = 0;
    uint16 free_num = 0;
    BOOLEAN ret_val = FALSE;
    uint16 used_num = 0;
    uint16 max_num = 0;
    MMISMS_SMSEDIT_CONTENT_T    *edit_content_ptr = PNULL;
    if (MMISMS_MsgIsInDraftBox())
    {
        org_num = MMISMS_GetReadSMSNum();
    }
    edit_content_ptr = MMISMS_GetEditContentInfoPtr();
    total_num = MMISMS_SplitLongMsg(&edit_content_ptr->send_message, FALSE);
    //get NV SMS num
    MMISMS_GetSMSMemory(MN_DUAL_SYS_1, TRUE, &max_num, &used_num);
    if(max_num < used_num)
    {
        //SCI_TRACE_LOW:" sms num error max_num = %d,used_num = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_22888_112_2_18_2_55_8_822,(uint8*)"dd", max_num, used_num);
    }
    else
    {
          free_num = max_num - used_num + org_num;
    }    
           
   
    if (free_num >= total_num)
    {
        ret_val = TRUE;
    }
    return ret_val;
}

/*****************************************************************************/
//     Description : get the sms is in the draft box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MsgIsInDraftBox(void)
{
    BOOLEAN is_in_draft = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_GetFoldType();
    MMISMS_BOX_TYPE_E security_type = MMISMS_GetSecurityBoxType();
    if(((
        #ifdef MMISMS_CHATMODE_ONLY_SUPPORT 
        MMK_IsOpenWin(MMISMS_NOSENDBOX_CHILD_WIN_ID)&& 
        #else
        MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)&& 
        #endif
        (MMISMS_BOX_NOSEND == MMISMS_GetCurBoxType())) ||
        ((MMISMS_FOLDER_SECURITY == folder_type) && (MMISMS_BOX_NOSEND == security_type)))
#ifdef MMI_TIMERMSG_SUPPORT
        && !MMISMS_GetIsTimerMsg()
#endif
      )
    {
        is_in_draft = TRUE;
    }
    return is_in_draft;
}

/*****************************************************************************/
//     Description : get the sms is in the senfail box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_MsgIsInSendFailBox(void)
{
    BOOLEAN is_in_sendfail = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_GetFoldType();
    MMISMS_BOX_TYPE_E security_type = MMISMS_GetSecurityBoxType();
    
    if((((MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID)) && (MMISMS_BOX_SENDFAIL == MMISMS_GetCurBoxType())) ||
       ((MMISMS_FOLDER_SECURITY == folder_type) && (MMISMS_BOX_SENDFAIL == security_type)))
#ifdef MMI_TIMERMSG_SUPPORT
       && !MMISMS_GetIsTimerMsg()
#endif
      )
    {
        is_in_sendfail = TRUE;
    }
    return is_in_sendfail;
}

/*****************************************************************************/
// 	Description : to show the mem full prompt
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowMemFullPrompt(
                                            MN_DUAL_SYS_E dual_sys,
                                            MN_SMS_MEM_FULL_STATUS_E mem_status)
{
    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;	
    uint32 time_out = 0;
    MMI_WIN_ID_T alert_win_id = MMISMS_MEMFULL_ALERT_USER_WIN_ID;
    MMI_TEXT_ID_T alert_txt_id = TXT_NULL;
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    // MN_DUAL_SYS_E i = 0;
    #endif

    //SCI_TRACE_LOW:"MMISMS_ShowMemFullPrompt:dual_sys=%d,mem_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_20994_112_2_18_2_55_4_814,(uint8*)"dd", dual_sys,mem_status);

    SCI_TRACE_LOW("MMISMS: g_mmisms_is_need_mem_full = %d", g_mmisms_is_need_mem_full);

    if (g_mmisms_is_need_mem_full)
    {
        g_mmisms_is_need_mem_full = FALSE;

        return;
    }

    if (!MMK_IsOpenWin(alert_win_id))
    {
#ifdef SCREENSAVER_SUPPORT
        if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
        {
            MMIAPIIDLESS_CloseScreenSaver();
        }
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            win_priority = WIN_ONE_LEVEL;
            MMIAPIKL_SetNewSMSStatus();
        }
        else
#endif
        {
            win_priority = WIN_ONE_LEVEL;
        }

        if(mem_status == MN_SMS_ME_FULL)
        {
            alert_txt_id = TXT_SMS_ME_MEM_FULL_ALERT_USER;
        }
        else if(mem_status == MN_SMS_SIM_FULL)
        {
#ifndef MMI_MULTI_SIM_SYS_SINGLE            
            alert_txt_id = TXT_SMS_SIM1_MEM_FULL_ALERT_USER+((MMI_TEXT_ID_T)dual_sys-MN_DUAL_SYS_1);          
#else
            alert_txt_id = TXT_SMS_SIM_MEM_FULL_ALERT_USER;
#endif
        }
        else if (mem_status == MN_SMS_ALL_FULL)
        {
            alert_txt_id = TXT_SMS_MEM_FULL_ALERT_USER;
        }

        if (TXT_NULL != alert_txt_id)
        {
            WatchPUB_Alert_Note_Win(MMISMS_NOTE_WIN_ID,alert_txt_id);//fIX BUG:1351488  1351476
        }
    }
}


/*****************************************************************************/
//     Description : set the flag of playing new SMS ring
//    Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
void MMISMS_SetNewSMSRingFlag(
                              BOOLEAN    is_need_play    //IN:
                              )
{
    s_sms_play_ring_info.is_need_play_ring = 0;//is_need_play;
}

/*****************************************************************************/
//     Description : get the flag of playing new SMS ring
//    Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
BOOLEAN MMISMS_GetNewSMSRingFlag(void)
{
    return (s_sms_play_ring_info.is_need_play_ring);
}

/*****************************************************************************/
//     Description : get dual sys when new msg arrive
//    Global resource dependence : s_dual_new_sms
//  Author: bown.zhang
//    Note:
/*****************************************************************************/
MN_DUAL_SYS_E MMISMS_GetSimIdOfNewMsg(void)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    return (s_sms_play_ring_info.dual_sys);
#else
    return MN_DUAL_SYS_1;
#endif
}

/*****************************************************************************/
//     Description : set dual sys when new msg arrive
//    Global resource dependence : s_dual_new_sms
//  Author: bown.zhang
//    Note:
/*****************************************************************************/
void MMISMS_SetSimIdOfNewMsg(MN_DUAL_SYS_E dual_sys)
{
    s_sms_play_ring_info.dual_sys = dual_sys;
}

/*****************************************************************************/
//  Description : read new sms ex
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ReadNewSmsEx(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMISMS_BOX_TYPE_E        box_type = MMISMS_BOX_NONE;
    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            box_type = *(MMISMS_BOX_TYPE_E*)param_ptr;
        }
        MMISMS_ReadNewSms(box_type);
    }
}

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_ReadNewSms(
                              MMISMS_BOX_TYPE_E        box_type
                              )
{
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
    MMISMS_ORDER_ID_T record_id = MMISMS_GetFirstUnreadMsgId();

    if ((1 == MMISMS_GetTobeReadMessageCount(MN_DUAL_SYS_MAX) 
        && PNULL != record_id))
#else
    if (1 == MMISMS_GetChatNodeTotalUnreadNum())
#endif
    {
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
        uint16 unread_msg_index = MMISMS_GetMTUnDownloadMsgCount(MN_DUAL_SYS_MAX);
        
        //排序顺序为未下载彩信，新信息，新彩信，其他MT信息
        MMISMS_EnterSmsBox(MMISMS_BOX_MT);
        GUILIST_SetCurItemIndex(MMISMS_MTBOX_LISTBOX_CTRL_ID, unread_msg_index);
        
        switch ((MMISMS_MSG_TYPE_E)record_id->flag.msg_type)
        {
        case MMISMS_TYPE_SMS:
            MMK_PostMsg(MMISMS_MSGBOX_MAIN_WIN_ID,
                MMISMS_MSG_READ_MSG,
                PNULL,
                0
                );
            break;

#ifdef MMS_SUPPORT
        case MMISMS_TYPE_MMS:
            MMIAPIMMS_ReadMMS((uint32)record_id->record_id);
            break;
#endif

#ifdef MMI_OTA_SUPPORT
        case MMISMS_TYPE_WAP_PUSH:
            MMIAPIOTA_WapPushEnter((uint16)record_id->record_id);
            break;

        case MMISMS_TYPE_WAP_OTA:
        case MMISMS_TYPE_MMS_OTA:
            MMIAPIOTA_OTAEnter((uint16)record_id->record_id);
            break;
#endif

        default:
            break;
        }
#else
        MMI_STRING_T     prompt_str = {0};
        MMISMS_ORDER_ID_T order_id = PNULL;
        MMISMS_ORDER_ID_T cur_order_id = PNULL;
        int32 i = 0;

#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);        
            return;
        }
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
        GUILIST_SetCurItemIndex(MMISMS_CHATBOX_LISTBOX_CTRL_ID, 0);
#endif
        MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        order_id = MMISMS_GetChatUnreadOrderId();
        if (PNULL == order_id
            || (MMISMS_TYPE_SMS != order_id->flag.msg_type && MMISMS_TYPE_MMS != order_id->flag.msg_type))
        {
            //SCI_TRACE_LOW:"mmisms: order_id is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_19673_112_2_18_2_55_1_801,(uint8*)"");
            return;
        }
        MMISMS_SetSMSUpdateInfo(order_id);
                             
        MMISMS_InitCurOperationOrderId();
        if (order_id->flag.is_concatenate_sms) //is long sms
        {
            cur_order_id = order_id;
            while (cur_order_id != PNULL)
            {
                MMISMS_SetCurOrderId(i,cur_order_id); //设置当前order_id到全局变量数组
                cur_order_id  = cur_order_id->next_long_sms_ptr;
                i++;
            }
        }
        else //is normal sms
        {
            MMISMS_SetCurOrderId(0,order_id); //设置当前order_id到全局变量数组 
        }  
        
        cur_order_id = MMISMS_GetCurOperationOrderId();
#ifdef MMI_SMS_CHAT_SUPPORT 
        if(0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_CHAT))
        {
            return;
        }
        MMISMS_EnterSMSChatReadWin(cur_order_id);
 //       MMK_CreateWin((uint32 *)MMISMS_SHOW_CHAT_SMS_WIN_TAB, PNULL);
#else
        if(0 == MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_MT))
        {
            return;
        }
        {
            MMISMS_READ_MSG_T read_msg = {0};

             read_msg = MMISMS_GetReadResult();
             MMISMS_EnterShowSmsWin(read_msg);
        }     
#endif
        MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);                
                
#endif
    }
    else
    {
        MMK_CloseMiddleWin(MMISMS_START_WIN_ID, MMISMS_END_WIN_ID); 
        MMISMS_OpenSmsBox(box_type);       
    }
    
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : get phone number from calllog
//    Global resource dependence : 
//  Author: jixin.xu
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetNumberFromCalllog(
                                   MMI_MESSAGE_ID_E    msg_id    //IN:
                                   )
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 count_cl = MMIAPICL_GetSelectedItemCount();
    uint8 tele_len = 0;
    uint8 max_len = 0;
    uint8 tele_num[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    BOOLEAN result = TRUE;
    MMIPB_BCD_NUMBER_T    number = {0};
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    edit_content_ptr = MMISMS_GetEditContentFromGlobal();
    
    //SCI_TRACE_LOW:"MMISMS: MMISMS_GetNumberFromCalllog msg_id = 0x%x, count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_17670_112_2_18_2_54_57_792,(uint8*)"dd", msg_id, count_cl);

    if ((MMI_CL_MULTI_SELECT_LIST_RETURN == msg_id))
    {
        if (MMISMS_DEST_ADDR_MAX_NUM < count_cl)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
        }
        else if (0 < count_cl)
        {
            // read the phone number from calllog one by one
            for (i=0, j=0; i<MMICL_RECORD_MAX_NUM; i++)
            {
                SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));
                if (MMIAPICL_SelectedListIsValid(i))
                {
                    MMIAPICL_GetSelectPhonenum(&number, i);
                    max_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, number.number_len);
                    tele_len = MMIAPICOM_GenDispNumber( 
                        MMIPB_GetNumberTypeFromUint8(number.npi_ton), 
                        max_len, 
                        number.number,  
                        tele_num,
                        MMISMS_NUMBER_MAX_LEN + 2
                        );
                    MMISMS_SetAddressToMessage(tele_num, tele_len, &edit_content_ptr->dest_info.dest_list);
                    
                    j++;
                }
                if (j >= count_cl)
                {
                    break;
                }
            }
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}
#endif

/*****************************************************************************/
//     Description : to open write SMS window    
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenWriteWin(void)
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();

    if (MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady())
    {
        if(!MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))//安全信箱中新建，无需再设置信箱类型
        {
            MMISMS_SetCurType(MMISMS_BOX_NONE);
        }
		    MMISMS_ClearDestNum(); 
		    #ifdef MMI_SMS_MINI_SUPPORT
		    MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, TRUE, PNULL, 0);
		    #else
		    MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, FALSE, PNULL, 0);
		    #endif
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMISMS_OpenInitWaitingWindow(MMISMS_WRITE_NEW_SMS);
    }
}



/*****************************************************************************/
//  Description : 打开sms 隐私保护密码输入成功后的写界面窗口
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenWriteWindowExt(BOOLEAN is_permitted, DPARAM param_ptr)
{    
    MMISMS_WRITE_MSG_INFO_T *write_info = (MMISMS_WRITE_MSG_INFO_T*)param_ptr;
    
    if(is_permitted)
    {
        MMISMS_WriteNewMessage(
            write_info->dual_sys,
            &write_info->sms_str,
            FALSE,
            write_info->dest_addr_ptr,
            write_info->dest_addr_len
            );
    }

    SCI_FREE(write_info->sms_str.wstr_ptr);
    SCI_FREE(write_info->dest_addr_ptr);
}

/*****************************************************************************/
//  Description : 打开sms 隐私保护密码输入成功后的写界面窗口
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenSendGroupWinExt(BOOLEAN is_permitted, DPARAM param_ptr)
{    
    MMISMS_SEND_GROUP_INFO_T *send_group_info_ptr = (MMISMS_SEND_GROUP_INFO_T*)param_ptr;
    if(PNULL == send_group_info_ptr)
    {
        return;
    }
    if(is_permitted)
    {
        MMISMS_SendGroupSms(
            send_group_info_ptr->dual_sys,
            &send_group_info_ptr->sms_str,
            send_group_info_ptr->dest_info
            );
    }

    SCI_FREE(send_group_info_ptr->sms_str.wstr_ptr);
    SCI_FREE(send_group_info_ptr->dest_info);
}


/*****************************************************************************/
//     Description : save the message to the draft box
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveAsDraft(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    uint8 org_num = 0;
    MN_CALLED_NUMBER_T dest_num = {0};
    MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;
    edit_content_ptr = MMISMS_GetEditContentFromGlobal();
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif

//  Replace will use it
    if (MMISMS_MsgIsInDraftBox())
    {
        org_num = MMISMS_GetReadSMSNum();
    }

//Don't know the reason.    
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMS_SUPPORT
    if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT == MMIAPIMMS_GetMMSEditEnterReason()
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        && (MMISMS_BOX_SECURITY == MMISMS_GetCurBoxType())
#endif
        )
    {
        MMISMS_SetSecurityBoxType(MMISMS_BOX_NOSEND);
    }
#endif
    #endif

        
    if (MMISMS_HaveEnoughSpace(dual_sys, FALSE, org_num))
    {
        if (MMISMS_MsgIsInDraftBox()
#ifdef MMI_TIMERMSG_SUPPORT
            && !MMISMS_GetIsTimerMode()
#endif
        )
        {
        
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMS_SUPPORT
            if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT == MMIAPIMMS_GetMMSEditEnterReason()) //开始编辑时是彩信，最后保存为短信
            {                                       
//These should be moved to funcwin
                MMISMS_ORDER_ID_T current_order_id = PNULL;
                current_order_id = MMISMS_GetCurOperationOrderId();                 
                                       
                if(current_order_id != PNULL && MMISMS_TYPE_MMS == current_order_id->flag.msg_type)
                {
                    MMIMMS_DeleteMMSAndSaveList(current_order_id->record_id);
                    //MMIAPIMMS_DeleteMMS(current_order_id->record_id,MMISMS_DELETE_ANIM_WIN_ID);
                }
//
                MMISMS_SetIsSaveDraft(TRUE);
                err_code = MMISMS_AppSaveSms(dual_sys, FALSE);
                MMISMS_SetSecurityBoxType(MMISMS_BOX_NOSEND);                     
            } 
            else
#endif
#endif
            {
                //save update address
                MMISMS_SaveUpdateAddr(TRUE);
                if (MMISMS_GetDestNumber(&dest_num, &edit_content_ptr->dest_info))
                {
                    MMISMS_SetSMSSaveInfo(dual_sys, &dest_num);
                }
                else
                {
                    MMISMS_ClearSaveInfo();
                }

                // the draft box is opened, indicate update current message
                err_code = MMISMS_AppUpdateMoSms(dual_sys, MMISMS_UPDATE_NOSEND_SMS, MN_SMS_MO_TO_BE_SEND, PNULL);
            }
        }
        else
        {
            MMISMS_SetIsSaveDraft(TRUE);            
            err_code = MMISMS_AppSaveSms(dual_sys, FALSE);
        }
		
        if(MMISMS_NO_SPACE != err_code)
        {
#ifdef MMI_TIMERMSG_SUPPORT
            if(MMISMS_GetIsTimerMode())
			{
                MMISMS_DisplayPrompByErr(err_code, TXT_COMM_SAVE_SUCCESS);		
			}
			else
#endif
			{
				MMISMS_DisplayPrompByErr(err_code, TXT_COMM_SAVE_AS_DRAFT);		
			}
        }

        //MMISMS_SetIsNeedLoadSaved(FALSE);
    }
    else
    {
        //prompt: no free space
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
    }
}
#ifdef MMI_SMS_MINI_SUPPORT

/*****************************************************************************/
//  Description :Load msg string from stored buffer
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_LoadStoreddMsgBuffer(MMI_STRING_T *sms_string)
{
    if (sms_string == PNULL)
    {
        return FALSE;
    }

    MMISMS_ConvertValidDataToDispStr(sms_string->wstr_ptr,
                                     &sms_string->wstr_len,
                                     g_mmisms_global.edit_content.send_message.data,
                                     MIN(MMISMS_R8_MESSAGE_LEN,g_mmisms_global.edit_content.send_message.length),
                                     g_mmisms_global.edit_content.send_message.alphabet,
                                     g_mmisms_global.edit_content.send_message.language,
                                     TRUE,
                                     g_mmisms_global.edit_content.send_message.is_single_shift
                                     );
    return TRUE;
}

/*****************************************************************************/
//  Description :save msg string to stored buffer
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SaveStoredMsgdBuffer(MMI_STRING_T sms_string)
{
    if (MMISMS_GetCurBoxType() == MMISMS_BOX_NOSEND)
    {
        MMISMS_ClearEditContent();
    }
    else
    {
        MMISMS_SetMessageContent(sms_string.wstr_len, sms_string.wstr_ptr, TRUE, &g_mmisms_global.edit_content);
    }
    return TRUE;
}
#endif    

/*****************************************************************************/
//  Description : to handle curr sms busy
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSmsBusy(MMISMS_ORDER_ID_T cur_order_id)
{
    if (cur_order_id != PNULL)
    {
        if ((MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type || MMISMS_MO_DRAFT == cur_order_id->flag.mo_mt_type) && ( MMISMS_SENDSTATUS_SENDING == cur_order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == cur_order_id->backsend_status))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description :根据setting设置的显示格式,以及cur_order_id获取时间
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDateTimeStringByOrder(MMISMS_ORDER_ID_T cur_order_id,  //in
                                 BOOLEAN is_disp_date,
                                 BOOLEAN is_disp_year,
                                 BOOLEAN is_disp_time,
                                 wchar *date_time_ptr  //out
                                )
{
    if(PNULL == date_time_ptr || PNULL == cur_order_id)
    {
        SCI_TRACE_LOW("mmisms date_time_ptr PNULL");
        return 0;
    }
    return MMISMS_GetDateTimeStringBySecond(cur_order_id->time,is_disp_date,is_disp_year,is_disp_time,date_time_ptr);
}

/*****************************************************************************/
//  Description :根据setting设置的显示格式,以及相对秒数获取时间
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDateTimeStringBySecond(uint32 second,  //in
                                 BOOLEAN is_disp_date,
                                 BOOLEAN is_disp_year,
                                 BOOLEAN is_disp_time,
                                 wchar *date_time_ptr  //out
                                )
{
    MMI_TM_T mmi_time_stamp = {0};
    if(PNULL == date_time_ptr)
    {
        SCI_TRACE_LOW("mmisms date_time_ptr PNULL");
        return 0;
    }
    mmi_time_stamp = MMIAPICOM_Second2Tm(second);
    return MMISMS_GetDateTimeStringByStamp(&mmi_time_stamp,is_disp_date,is_disp_year,is_disp_time,date_time_ptr);
}

/*****************************************************************************/
//  Description :根据setting设置的显示格式,以及MN_SMS_TIME_STAMP_T获取时间
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetDateTimeStringByStamp(MMI_TM_T *sc_time_ptr,  //in
                                 BOOLEAN is_disp_date,
                                 BOOLEAN is_disp_year,
                                 BOOLEAN is_disp_time,
                                 wchar *date_time_ptr //out
                                )
{
    uint8    date_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8    time_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    uint8    len  = 0;
    uint8    time_tmp[20] = {0};
    if (PNULL == date_time_ptr || PNULL == sc_time_ptr)
    {
        //SCI_TRACE_LOW:"mmisms date_time_str PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_27455_112_2_18_2_55_17_846, (uint8*)"");
        return 0;
    }
    if(is_disp_date && is_disp_time)
    {
        MMIAPISET_FormatDateStrByDateEx(sc_time_ptr->tm_year, sc_time_ptr->tm_mon, sc_time_ptr->tm_mday,
                                        '-', date_str, MMISET_DATE_STR_LEN, is_disp_year);
        MMIAPISET_FormatTimeStrByTime(sc_time_ptr->tm_hour, sc_time_ptr->tm_min, time_str, MMISET_TIME_STR_12HOURS_LEN + 1);
        //Add item SMS time
        len = sprintf((char*)time_tmp, "%s %s", date_str, time_str);
    
        MMI_STRNTOWSTR(
            date_time_ptr,
            len,
            (const uint8 *)time_tmp,
            len,
            len
        );
    }
    else if(is_disp_date && !is_disp_time)  //只显示日期
    {
        MMIAPISET_FormatDateStrByDateEx(sc_time_ptr->tm_year, sc_time_ptr->tm_mon, sc_time_ptr->tm_mday,
                                '-', date_str, MMISET_DATE_STR_LEN, is_disp_year);
        len = SCI_STRLEN((char*)date_str);
        MMI_STRNTOWSTR(
            date_time_ptr,
            len,
            (const uint8 *)date_str,
            len,
            len
        );
    }
    else if(!is_disp_date && is_disp_time)
    {
        MMIAPISET_FormatTimeStrByTime(sc_time_ptr->tm_hour, sc_time_ptr->tm_min, time_str, MMISET_TIME_STR_12HOURS_LEN + 1);
        len = (uint8)SCI_STRLEN((char*)time_str);    
        MMI_STRNTOWSTR(
            date_time_ptr,
            len,
            (const uint8 *)time_str,
            len,
            len
        );
    }
    else
    {
        SCI_TRACE_LOW("mmisms time disp none");
        return 0;        
    }
    return len;
}



/******************************#W317 Public Func#******************************/
/*****************************************************************************/
//  Description : set number info for send
//  Parameter: [In] phone number
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetNumberForSend(MMISMS_DEST_ADDR_INFO_T s_send_dest_list)
{
    g_sms_numberinfo_for_send = s_send_dest_list;
}

/*****************************************************************************/
//  Description : Get number info for send
//  Parameter: [In] None
//             [Out] contact info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_DEST_ADDR_INFO_T* MMISMS_GetNumberForSend()
{
    MMISMS_DEST_ADDR_INFO_T* send_num_ptr = PNULL;
    send_num_ptr = &g_sms_numberinfo_for_send;
    return send_num_ptr;
}

/*****************************************************************************/
//  Description : Set Preset list win enter From
//  Parameter: [In] enum
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetPresetFrom(MMISMS_PRESETFROM_E s_presetfrom)
{
    s_sms_presetfrom = s_presetfrom;
}

/*****************************************************************************/
//  Description : Get Preset list win enter From
//  Parameter: [In] None
//             [Out] enum
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_PRESETFROM_E MMISMS_GetPresetFrom()
{
    return s_sms_presetfrom;
}

/*****************************************************************************/
//  Description : set preset info
//  Parameter: [In] message info buff&len
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetPresetForSend(wchar* preset_buff, uint16 preset_len)
{
    if( preset_buff == PNULL||preset_len==0 )
    {
        return FALSE;
    }

    SCI_MEMSET(s_sms_preset_info.preset_buff, 0, sizeof(s_sms_preset_info.preset_buff));
    MMIAPICOM_Wstrncpy(s_sms_preset_info.preset_buff,preset_buff,preset_len);
    s_sms_preset_info.preset_len = preset_len;
    return TRUE;
}

/*****************************************************************************/
//  Description :Get preset info
//  Parameter: [In] None
//             [Out] message info
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_PRESET_BUFF_T MMISMS_GetPresetForSend ()
{
    return s_sms_preset_info;
}


