/****************************************************************************
** File Name:      mmicdma_app.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 tlt, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef  _MMI_ZDT_SMS_C_
#define _MMI_ZDT_SMS_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmisms_export.h"
#include "zdt_app.h"

#define ZDTAPP_SMS_TRACE            SCI_TRACE_LOW

#define ZDT_SMS_SEND_REPEAT_TIMES		3		

static ZDT_SMS_DATA_LINK_NODE_T  *    s_sms_data_link_head = PNULL;

static SCI_TIMER_PTR	s_zdt_timer_sms_send_ptr = NULL;
static BOOLEAN s_sms_is_sending_data = FALSE;
static MMI_ZDT_SMS_DATA_T *        s_sms_cur_send_pdata = PNULL;

static BOOLEAN s_sms_is_poping_data = FALSE;
static MMI_ZDT_SMS_DATA_T *        s_sms_cur_pop_pdata = PNULL;

static BOOLEAN ZDT_SMS_Send_Over(BOOLEAN is_success, BOOLEAN is_finish);
void MMIZDT_SMS_PopSend_Result(uint8  result);

BOOLEAN  MMIZDT_SMS_SendSigTo_APP(ZDT_APP_SIG_E sig_id, MMI_ZDT_SMS_DATA_T * p_sms_data)
{
    uint8 * pstr = PNULL;
    MMI_ZDT_SMS_SIG_T * psig = PNULL;
            
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_ZDT_SMS_SIG_T),SCI_IdentifyThread());
    psig->p_sms_data = p_sms_data;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);

    return TRUE;
}

//不 分配内存
static BOOLEAN ZDT_SMS_Link_Add(MMI_ZDT_SMS_DATA_T * p_data)
{
    ZDT_SMS_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_SMS_DATA_LINK_NODE_T  * p2 = PNULL;
    
    if(p_data == PNULL 
        || p_data->num == PNULL || p_data->num_len ==  0 
        || p_data->msg == PNULL || p_data->msg_len == 0)
    {
        return FALSE;
    }
    
    p1=(ZDT_SMS_DATA_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(ZDT_SMS_DATA_LINK_NODE_T));
    if(p1 == PNULL)
    {
        return FALSE;
    }
    
    p1->p_data = p_data;    
    p1->next_ptr = PNULL;
    
    if(s_sms_data_link_head == PNULL)
    {
        s_sms_data_link_head = p1;
    }
    else
    {
        p2 = s_sms_data_link_head;
        while(p2->next_ptr != PNULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

//不释放内存
static BOOLEAN  ZDT_SMS_Link_GetByType(ZDT_SMS_SEND_TYPE_E send_type, MMI_ZDT_SMS_DATA_T ** sms_data)
{
    ZDT_SMS_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_SMS_DATA_LINK_NODE_T  * p2 = PNULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = s_sms_data_link_head;
    
    while(p1 != PNULL)
    {
        if(p1->p_data != PNULL && p1->p_data->send_type == send_type)
        {
            if(send_type == ZDT_GPRS_SMS_MSG && p1->p_data->is_delay)
            {
                p2 = p1;
                p1 = p2->next_ptr;
                continue;
            }
            
            *sms_data = p1->p_data;
            if(p1 == s_sms_data_link_head)
            {
                s_sms_data_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            res = TRUE;
            SCI_FREE(p1);
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

//不释放内存
static BOOLEAN   ZDT_SMS_Link_Get(MMI_ZDT_SMS_DATA_T ** sms_data)
{
    ZDT_SMS_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_SMS_DATA_LINK_NODE_T  * p2 = PNULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = s_sms_data_link_head;
    
    while(p1 != PNULL)
    {
        if(p1->p_data != PNULL && p1->p_data->num_len != 0)
        {
            if(p1->p_data->send_type == ZDT_GPRS_SMS_MSG && p1->p_data->is_delay)
            {
                p2 = p1;
                p1 = p2->next_ptr;
                continue;
            }
            *sms_data = p1->p_data;
            if(p1 == s_sms_data_link_head)
            {
                s_sms_data_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            res = TRUE;
            SCI_FREE(p1);
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN   ZDT_SMS_Link_ClearDelay(void)
{
    ZDT_SMS_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_SMS_DATA_LINK_NODE_T  * p2 = PNULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = s_sms_data_link_head;
    
    while(p1 != PNULL)
    {
        if(p1->p_data != PNULL && p1->p_data->send_type == ZDT_GPRS_SMS_MSG && p1->p_data->is_delay)
        {
            p1->p_data->is_delay =  0;
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN   ZDT_SMS_Link_DelType(ZDT_SMS_SEND_TYPE_E send_type)
{
    ZDT_SMS_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_SMS_DATA_LINK_NODE_T  * p2 = PNULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = s_sms_data_link_head;
    
    while(p1 != PNULL)
    {
        if(p1->p_data != PNULL && p1->p_data->send_type == send_type)
        {            
            if(p1 == s_sms_data_link_head)
            {
                s_sms_data_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            res = TRUE;
            if(p1->p_data->msg != PNULL)
            {
                SCI_FREE(p1->p_data->msg);
            }
            if(p1->p_data->num != PNULL)
            {
                SCI_FREE(p1->p_data->num);
            }
            SCI_FREE(p1->p_data);
            SCI_FREE(p1);
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN  ZDT_SMS_Link_DelAll(void)
{
    ZDT_SMS_DATA_LINK_NODE_T  * p1 = PNULL;
    
    while(s_sms_data_link_head != PNULL)
    {
        p1 = s_sms_data_link_head;
        s_sms_data_link_head = p1->next_ptr;
        if(p1->p_data != PNULL)
        {
            if(p1->p_data->num != PNULL)
            {
                SCI_FREE(p1->p_data->num);
            }
            if(p1->p_data->msg != PNULL)
            {
                SCI_FREE(p1->p_data->msg);
            }
            SCI_FREE(p1->p_data);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static MMI_ZDT_SMS_DATA_T *  ZDT_SMS_Link_PopData(BOOLEAN need_type, ZDT_SMS_SEND_TYPE_E send_type)
{
    uint8 * pstr = PNULL;
    MMI_ZDT_SMS_DATA_T * p_sms_data = PNULL;
    BOOLEAN res = FALSE;
    
    if(need_type)
    {
        res = ZDT_SMS_Link_GetByType(send_type,&p_sms_data);
        if(res == FALSE)
        {
            res = ZDT_SMS_Link_Get(&p_sms_data);
        }
    }
    else
    {
        res = ZDT_SMS_Link_Get(&p_sms_data);
    }
    
    if(res == FALSE || p_sms_data == NULL)
    {
        ZDTAPP_SMS_TRACE("ZDT_SMS_Link_PopData Empty !!!");
        return PNULL;
    }
    
    if(p_sms_data->num_len == 0 || p_sms_data->num == PNULL
        || p_sms_data->msg_len == 0 || p_sms_data->msg == PNULL)
    {
        if(p_sms_data->num != PNULL)
        {
            SCI_FREE(p_sms_data->num);
        }
        if(p_sms_data->msg != PNULL)
        {
            SCI_FREE(p_sms_data->msg);
        }
        SCI_FREE(p_sms_data);
        ZDTAPP_SMS_TRACE("ZDT_SMS_Link_PopData ERR Len");
        return PNULL;
    }
    
    ZDTAPP_SMS_TRACE("ZDT_SMS_Link_PopData OK p_sms_data=0x%x,num=%s,send_type=%d,times=%d,msg_len=%d",p_sms_data,p_sms_data->num,p_sms_data->send_type,p_sms_data->times,p_sms_data->msg_len);
    return p_sms_data;
}

static void ZDT_SMS_Timer_Send_Handle(uint32 param)
{
    ZDT_SMS_Send_Over(0,1);
    return;
}

static void ZDT_SMS_Timer_Send_Create(uint32 time_ms)
{    
	char timer_name[] = "SMSDataSendTmr";
    
	if (NULL == s_zdt_timer_sms_send_ptr)
	{
		s_zdt_timer_sms_send_ptr = SCI_CreateTimer(timer_name,
                                                                                            ZDT_SMS_Timer_Send_Handle,
                                                                                            0,
                                                                                            time_ms,
                                                                                            SCI_AUTO_ACTIVATE);
	}
	else
	{
		SCI_DeactiveTimer(s_zdt_timer_sms_send_ptr);
		SCI_DeleteTimer(s_zdt_timer_sms_send_ptr);
		s_zdt_timer_sms_send_ptr = SCI_CreateTimer(timer_name,
                                                                                            ZDT_SMS_Timer_Send_Handle,
                                                                                            0,
                                                                                            time_ms,
                                                                                            SCI_AUTO_ACTIVATE);
	}
}     

static void ZDT_SMS_Timer_Send_Stop(void)
{
	if (NULL != s_zdt_timer_sms_send_ptr)
	{
            SCI_DeactiveTimer(s_zdt_timer_sms_send_ptr);
            SCI_DeleteTimer(s_zdt_timer_sms_send_ptr);
            s_zdt_timer_sms_send_ptr = NULL;
	}
}     

static BOOLEAN  ZDT_SMS_Send_Over(BOOLEAN is_success, BOOLEAN is_finish)
{
    
   ZDTAPP_SMS_TRACE("ZDT_SMS_Send_Over: is_success=%d, is_sending=%d",is_success,s_sms_is_sending_data);

    if(s_sms_is_sending_data)
    {
        s_sms_is_sending_data = FALSE;
    }
    
    if(is_success)
    {
        MMIZDT_SMS_PopSend_Result(0);
    }
    else
    {
        MMIZDT_SMS_PopSend_Result(1);
    }
#ifndef WIN32
    //SCI_ZDT_SMS_EnableDeepSleep(TRUE);
#endif
    
    return TRUE;
}

static BOOLEAN ZDT_SMSCnf(BOOLEAN is_succss, DPARAM parm)
{
    BOOLEAN resValue = TRUE;
    ZDT_SMS_Timer_Send_Stop();
    ZDT_SMS_Send_Over(is_succss,1);
    return resValue;
}

static BOOLEAN ZDT_SMS_Send(
                                        uint8	        *sms_data_ptr,	//IN:
                                        int32          sms_data_len,
                                        uint8            *dest_addr_ptr,    //IN:
                                        uint8            dest_addr_len,    //IN:
                                        int32          type
                                        )
{
	BOOLEAN result = FALSE;
	MMISMS_SEND_DATA_T send_data_ptr = {0};
	MMI_STRING_T msg = {0};

	if(sms_data_ptr == NULL  
		|| dest_addr_ptr == NULL 
		|| dest_addr_len == 0)
	{
             ZDTAPP_SMS_TRACE("ZDT_SMS_Send ERR Len");
		return FALSE;
	}
    
       ZDTAPP_SMS_TRACE("ZDT_SMS_Send: addr=%d,%s,type=%d,data_len=%d",dest_addr_len,dest_addr_ptr,type,sms_data_len);
    
	if( type == 0 || type == 1) //MN_SMS_DEFAULT_ALPHABET || MN_SMS_8_BIT_ALPHBET
	{
		msg.wstr_len = (uint16)sms_data_len;
		msg.wstr_ptr = (wchar*)SCI_ALLOCA((msg.wstr_len + 1) * sizeof(wchar)); 
		SCI_MEMSET(msg.wstr_ptr, 0, (msg.wstr_len + 1) * sizeof(wchar));
		MMI_STRNTOWSTR((wchar*)msg.wstr_ptr, msg.wstr_len, (uint8*)sms_data_ptr, msg.wstr_len, msg.wstr_len); /*lint !e64*/
	}
	else
	{
            msg.wstr_ptr = (wchar*)SCI_ALLOCA(sms_data_len+2);
            SCI_MEMSET(msg.wstr_ptr, 0, (sms_data_len+2));
            msg.wstr_len = sms_data_len/2;
            SCI_MEMCPY(msg.wstr_ptr,sms_data_ptr,sms_data_len);
	}
	
	if(0 != msg.wstr_len)
	{
		send_data_ptr.is_need_packet = TRUE;
		send_data_ptr.dual_sys = MMIZDT_Net_GetActiveSys();
		send_data_ptr.src_port = 0;
		send_data_ptr.dst_port = 0;
		send_data_ptr.sms_content_ptr = &msg;
		send_data_ptr.dest_addr_ptr = (const uint8*)dest_addr_ptr;
		send_data_ptr.dest_addr_len = dest_addr_len;
		send_data_ptr.send_callback = ZDT_SMSCnf;

		result = MMIAPISMS_SendSmsForOther(&send_data_ptr);
	}

	if(NULL != msg.wstr_ptr)
	{
		SCI_FREE(msg.wstr_ptr);
	}
	
	return result;
}

static BOOLEAN  ZDT_SMS_Send_Start(MMI_ZDT_SMS_DATA_T * p_sms_data)
{
    if(s_sms_is_sending_data)
    {
        ZDTAPP_SMS_TRACE("ZDT_SMS_Send_Start is Sending");
        return FALSE;
    }
    
    if(p_sms_data == PNULL || p_sms_data->num_len == 0 || p_sms_data->num == PNULL
        || p_sms_data->msg_len == 0 || p_sms_data->msg == PNULL)
    {
        ZDTAPP_SMS_TRACE("ZDT_SMS_Send_Start ERR Len");
        return FALSE;
    }

    #ifndef WIN32
        //SCI_ZDT_SMS_EnableDeepSleep(FALSE);
    #endif

    if(ZDT_SMS_Send(p_sms_data->msg, p_sms_data->msg_len,p_sms_data->num,p_sms_data->num_len,p_sms_data->dcs))
    {
        s_sms_is_sending_data = TRUE;
        s_sms_cur_send_pdata = p_sms_data;
        ZDT_SMS_Timer_Send_Create(1000* 50);
        return TRUE;
    }
    else
    {
        ZDTAPP_SMS_TRACE("ZDT_SMS_Send_Start ERR Send");
    #ifndef WIN32
        //SCI_ZDT_SMS_EnableDeepSleep(TRUE);
    #endif
    }

    return FALSE;
}

static BOOLEAN ZDT_SMS_CheckHead(uint8 * pData,int DataLen)
{
    return FALSE;
}

static ZDTSMSServerPos ZDT_SMS_IsFromServer(uint8 * sms_data_ptr, uint32 sms_data_len,BOOLEAN is_ucs2)
{    
    ZDTSMSServerPos ret = ZDT_SMS_SERVER_NULL;
    if(sms_data_ptr == NULL || sms_data_len < 15)
    {
        return ZDT_SMS_SERVER_NULL;
    }
    
    if(is_ucs2 == FALSE)
    {
        return ZDT_SMS_SERVER_NULL;
    }
    
    if(ZDT_SMS_CheckHead(sms_data_ptr,sms_data_len))
    {
        ret = ZDT_SMS_SERVER_HEAD;
    }
    else
    {
        int pos_len = sms_data_len-14;
        if(ZDT_SMS_CheckHead(sms_data_ptr+pos_len,14))
        {
            ret = ZDT_SMS_SERVER_TAIL;
        }
    }
    return ret;
}

static uint16 ZDT_SMS_IsFromSOS(uint8 *sms_num_ptr, uint8 sms_num_len)
{    
    return 0;
}

static BOOLEAN ZDT_SMS_Server_ContentValid(uint8 *sms_num_ptr, uint8 sms_num_len,uint8 * sms_data_ptr, uint32 sms_data_len,BOOLEAN is_ucs2)
{
    BOOLEAN  ret = FALSE;
    return ret;
}

BOOLEAN  ZDT_SMS_UCS_is_All_Ascii(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len //[IN]  源String的长度，byte数
                             )
{
	uint8 tmp;
	uint16 i;
	for(i = 0; i < src_len; i++)
	{
		tmp = src_ptr[i];
		if( tmp >= 0x80 )
		{
		    return FALSE;
		}
	}
	return TRUE;
}

#ifdef ZDT_GPS_SUPPORT
static void ZDT_SMS_GPS_On(void)
{
    #ifdef ZDT_PRODUCT_Z805
        MMIZDTGPS_PowerOn_Interval(g_z805_cfg_data.gps_internal);
        MMIZDT_CFG_GPS_OnOff(1);
    #else
        MMIZDTGPS_PowerOn_Interval(40);
    #endif
    return;
}

static void ZDT_SMS_GPS_Off(void)
{
    MMIZDTGPS_PowerOff_Interval();
    #ifdef ZDT_PRODUCT_Z805
        MMIZDT_CFG_GPS_OnOff(0);
    #endif
    return;
}
#endif

static BOOLEAN ZDT_SMS_SOS_ContentValid(uint8 *sms_num_ptr, uint8 sms_num_len,uint8 * sms_data_ptr, uint32 sms_data_len,BOOLEAN is_ucs2)
{
    uint8 * msg_str = NULL;
    uint16 msg_len = 0;
    BOOLEAN  ret = FALSE;
    uint16 i = 0;
    if(sms_data_ptr == NULL || sms_data_len == 0)
    {
        return FALSE;
    }

    if(is_ucs2)
    {
#ifdef ZDT_GPS_SUPPORT
       /*
          【GPS0】3010 0047 0050 0053 0030 3011 
      */
        if(sms_data_len >= 12)
        {
            if(sms_data_ptr[0] == 0x10
                && sms_data_ptr[1] == 0x30
                && (sms_data_ptr[2] == 0x47 || sms_data_ptr[2] == 0x67)
                && sms_data_ptr[3] == 0x00
                && (sms_data_ptr[4] == 0x50 || sms_data_ptr[4] == 0x70)
                && sms_data_ptr[5] == 0x00
                && (sms_data_ptr[6] == 0x53 || sms_data_ptr[6] == 0x73)
                && sms_data_ptr[7] == 0x00
                && (sms_data_ptr[8] == 0x30 || sms_data_ptr[8] == 0x31)
                && sms_data_ptr[9] == 0x00
                && sms_data_ptr[10] == 0x11
                && sms_data_ptr[11] == 0x30
                )
            {
                if(sms_data_ptr[8] == 0x30)
                {
                    ZDTAPP_SMS_TRACE("MT SMS UCS GPS OFF");
                    ZDT_SMS_GPS_Off();
                }
                else
                {
                    ZDTAPP_SMS_TRACE("MT SMS UCS GPS ON");
                    ZDT_SMS_GPS_On();
                }
                return TRUE;
            }
       }
#endif
        msg_str = SCI_ALLOC_APPZ(sms_data_len*2);
        if(msg_str == NULL)
        {
            return FALSE;
        }
        #ifndef WIN32
        msg_len = ucs2str_to_gb2312strExt(sms_data_ptr, sms_data_len,msg_str,0);
        #else
        msg_len = sms_data_len;
        #endif
        if(FALSE == ZDT_SMS_UCS_is_All_Ascii(msg_str,msg_len))
        {
            SCI_FREE(msg_str);
            return FALSE;
        }
    }
    else
    {
        msg_str = SCI_ALLOC_APPZ(sms_data_len+1);
        if(msg_str == NULL)
        {
            return FALSE;
        }
        SCI_MEMCPY(msg_str,sms_data_ptr,sms_data_len);
        msg_len = sms_data_len;
    }
    
    for(i = 0; i < msg_len; i++)
    {
        msg_str[i] = toupper(msg_str[i]);
    }

#ifdef ZDT_GPS_SUPPORT
    if(strncmp((char*)msg_str, "[GPS0]", 6) ==0 )
    {
        ZDTAPP_SMS_TRACE("MT SMS GPS OFF");
        ZDT_SMS_GPS_Off();
        ret = TRUE;
    }
    else if(strncmp((char*)msg_str, "[GPS1]", 6) ==0 )
    {
        ZDTAPP_SMS_TRACE("MT SMS GPS ON");
        ZDT_SMS_GPS_On();
        ret = TRUE;
    }
#endif

    SCI_FREE(msg_str);
    return ret;
}

#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
extern uint8 g_cur_mesage_index;
static BOOLEAN s_sms_need_open_new_win = FALSE;
static uint8 s_sms_need_open_new_win_type = 0;

LOCAL MMI_RESULT_E handleNewsConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
        break;
		
    case MSG_KEYUP_GREEN:
    case MSG_APP_OK:
    case MSG_APP_WEB:
             MMIZDT_OpenMessagePreWin();
             MMIPUB_CloseQuerytWin(PNULL);
             MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}

PUBLIC void MMIAPI_New_Message_Dialog(uint8 netsms_type)
{
    MMI_WIN_ID_T	dialog_win_id = MMIPUB_ALERT_WIN_ID;
    MMI_TEXT_ID_T text_id_list[] = 
    {
    	STR_MESSAGE_URGENT,
    	STR_MESSAGE_NOTICE,
    	STR_MESSAGE_WORK,
    	STR_MESSAGE_ATTENDANCE,
    	STR_MESSAGE_COST,
    	STR_MESSAGE_NEWS,
    	STR_MESSAGE_HAVE_MESSAGE
    };
    g_cur_mesage_index = 0;
    s_sms_need_open_new_win = FALSE;
    MMIPUB_CloseAlertWin();
    MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,text_id_list[netsms_type],PNULL,PNULL,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handleNewsConfirmationDialog,0);
}

PUBLIC void MMIAPI_New_Message_Check(void)
{
    if(s_sms_need_open_new_win)
    {
        MMIAPI_New_Message_Dialog(s_sms_need_open_new_win_type);
    }
}

PUBLIC void MMIAPI_New_Message_Reset(void)
{
    s_sms_need_open_new_win = FALSE;
}

PUBLIC void MMIAPI_New_Message_Win(uint8 netsms_type)
{
    if(MMIZDT_IsSmsListFocusWin())
    {
        g_cur_mesage_index = 0;
        MMIZDT_UpdateSmsListWin();
    }
    else if(MMIZDT_IsSmsPreFocusWin())
    {
    }
    else
    {
        if(MMIAPIKL_IsPhoneLocked() || YX_DB_TIMER_IsValid_Class() || MMIALARM_IsActiveWin())
        {
            s_sms_need_open_new_win_type = netsms_type;
            s_sms_need_open_new_win = TRUE;
        }
        else
        {
            MMIAPI_New_Message_Dialog(netsms_type);
        }
    }
}
#endif

BOOLEAN  MMIZDT_SMS_Rcv_IsValid(MN_DUAL_SYS_E dual_sys,uint8 *sms_num_ptr, uint8 sms_num_len,uint8 * sms_data_ptr, uint32 sms_data_len,BOOLEAN is_ucs2)
{
    BOOLEAN need_display = TRUE;
    uint16 ys_is_from_sos = 0;
    BOOLEAN ret = FALSE;
    ZDTSMSServerPos server_status = ZDT_SMS_SERVER_NULL;
    MMISET_CALL_RING_TYPE_E     call_ring_type = MMISET_CALL_RING;
#ifndef ZDT_LCD_NOT_SUPPORT
    #if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
    uint16 buf[300+1] = {0};
    YX_DB_NETSMS_Type cur_data = {0};
    #endif
#endif

    if(sms_num_ptr == NULL || sms_data_ptr == NULL )
    {
        return FALSE;
    }
    server_status = ZDT_SMS_IsFromServer(sms_data_ptr,sms_data_len,is_ucs2);
    if(server_status == ZDT_SMS_SERVER_HEAD)
    {
        ZDTAPP_SMS_TRACE("MT SMS Valid From Server Head");
        ZDT_SMS_Server_ContentValid(sms_num_ptr,sms_num_len,sms_data_ptr+12,sms_data_len-12,is_ucs2);
        ret = TRUE;
    }
    else if(server_status == ZDT_SMS_SERVER_TAIL)
    {
        ZDTAPP_SMS_TRACE("MT SMS Valid From Server Tail");
        ZDT_SMS_Server_ContentValid(sms_num_ptr,sms_num_len,sms_data_ptr,sms_data_len-12,is_ucs2);
        ret = TRUE;
    }
    else
    {
#if defined(ZDT_PLAT_SHB_SUPPORT)
    ret = SHB_SMS_IsValid(sms_num_ptr,sms_data_ptr,sms_data_len,is_ucs2);
#elif defined(ZDT_PLAT_YX_SUPPORT)
    ret = YX_SMS_IsValid(sms_num_ptr,sms_data_ptr,sms_data_len,is_ucs2);
#else
    if(0 != ZDT_SMS_IsFromSOS(sms_num_ptr,sms_num_len))
    {
        ZDTAPP_SMS_TRACE("MT SMS Valid From SOS");
        ret = ZDT_SMS_SOS_ContentValid(sms_num_ptr,sms_num_len,sms_data_ptr,sms_data_len,is_ucs2);
    }
#endif
    }

    if(ret)
    {
        need_display = FALSE;
    }
    
#ifndef ZDT_LCD_NOT_SUPPORT
    #if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
#ifdef ZDT_PLAT_YX_SUPPORT_CY
    ys_is_from_sos = YX_SMS_IsFromSOS(sms_num_ptr);
    if(ys_is_from_sos == 0)
    {
        need_display = FALSE;
    }
#endif
    if(need_display)
    {
        cur_data.netsms_type = 6;
        cur_data.netsms_mode = 1;
        cur_data.netsms_playtimes = 0;
        cur_data.netsms_isvib = 0;
        cur_data.netsms_isring = 1;
        cur_data.netsms_isflash = 0;
        cur_data.netsms_rcv_time = ZDT_Get_TimeSecond();
        call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, CALL_RING_TYPE);
        switch (call_ring_type)
        {
            case MMISET_CALL_SILENT:
                    cur_data.netsms_isvib = 0;
                    cur_data.netsms_isring = 0;
             break;
            case MMISET_CALL_RING:
                    cur_data.netsms_isvib = 0;
                    cur_data.netsms_isring = 1;
             break;
            case MMISET_CALL_VIBRA:
                    cur_data.netsms_isvib = 1;
                    cur_data.netsms_isring = 0;
             break;
            case MMISET_CALL_VIBRA_AND_RING:
                    cur_data.netsms_isvib = 1;
                    cur_data.netsms_isring = 1;
             break;
            default:
                    cur_data.netsms_isvib = 0;
                    cur_data.netsms_isring = 1;
            	 break;
        }
        
        if(sms_data_len > YX_DB_NETSMS_MAX_NAME_SIZE)
        {
            sms_data_len = YX_DB_NETSMS_MAX_NAME_SIZE;
        }
        SCI_MEMCPY(cur_data.netsms_content,sms_data_ptr,sms_data_len);
        cur_data.netsms_content_len = sms_data_len;
        if(is_ucs2)
        {
            cur_data.netsms_encoding = 1;
        }
        else
        {
            cur_data.netsms_encoding = 0;
        }
        SCI_MEMCPY(cur_data.msg_id,sms_num_ptr,sms_num_len);
        YX_DB_NetSms_Add(&cur_data);
        MMIDEFAULT_TurnOnBackLight();
        if(cur_data.netsms_isvib)
        {
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,1000);
        }
        if(cur_data.netsms_isring)
        {
            if(!YX_API_Mute_IsValid() && !MMIALARM_IsActiveWin())
            {
#ifdef ZDT_PLAY_RING_CUSTOM
                MMIAPISET_PlayCustomRing(MMISET_CUST_VOICE_NEW,1);
#else
                //MMIAPISET_PlayRingByIdEx(FALSE,1,7,1,MMISET_RING_TYPE_MSG,PNULL);
#endif
            }
        }
        if(cur_data.netsms_isflash)
        {
            #ifdef ZDT_LED_SUPPORT
            ZDT_Led_Start_Call_Incoming();
            #endif
        }
        MMIAPI_New_Message_Win(cur_data.netsms_type);
        MMIAPICOM_StatusAreaSetMsgState(TRUE);
        SCI_TRACE_LOW("W9_LOG:____MMIZDT_SMS_Rcv_IsValid:%d",is_ucs2);
    }
    #endif
#endif
    return TRUE;
}

BOOLEAN  MMIZDT_SMS_IsSending(void)
{
    return s_sms_is_poping_data;
}

void MMIZDT_SMS_PopSend_Result(uint8  result)
{
    ZDTAPP_SMS_TRACE("MMIZDT_SMS_PopSend_Result %d,is_poping=%d",result,s_sms_is_poping_data);
    if(result == 0)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SMS_SEND_OVER,PNULL,0);
    }
    else
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SMS_SEND_OVER,(uint8 *)"1",1);
    }
}

BOOLEAN  MMIZDT_SMS_PopSend(BOOLEAN need_type, ZDT_SMS_SEND_TYPE_E send_type)
{
    MMI_ZDT_SMS_DATA_T * p_sms_data = PNULL;
    BOOLEAN res = FALSE;
    if(s_sms_is_poping_data || !MMIAPISMS_IsOrderOk() /*|| !MMIAPIIDLE_IdleWinIsOpen()*/)
    {
        ZDTAPP_SMS_TRACE("MMIZDT_SMS_PopSend ERR Busy is_poping=%d",s_sms_is_poping_data);
        return PNULL;
    }

    p_sms_data = ZDT_SMS_Link_PopData(need_type,send_type);

    if(p_sms_data != PNULL)
    {
        s_sms_is_poping_data = TRUE;
        s_sms_cur_pop_pdata = p_sms_data;
        res = ZDT_SMS_Send_Start(p_sms_data);
        if(res == FALSE)
        {
            MMIZDT_SMS_PopSend_Result(2);
        }
    }
    return res;
}

BOOLEAN  MMIZDT_SMS_AppSendType(ZDT_SMS_SEND_TYPE_E send_type,MN_DUAL_SYS_E dual_sys,uint8 * msg_str,uint16 msg_len,uint8 * num_str,uint16 num_len,uint8 dcs,uint16 repeat_times)
{
    MMI_ZDT_SMS_DATA_T * p_sms_data = PNULL;
    BOOLEAN res = FALSE;
    if(num_str == PNULL || num_len == 0
       || msg_str == PNULL || msg_len == 0)
    {
        return FALSE;
    }

    p_sms_data = SCI_ALLOC_APPZ(sizeof(MMI_ZDT_SMS_DATA_T));
    if (p_sms_data == PNULL)
    {
        return FALSE;
    }
    
    p_sms_data->num = SCI_ALLOC_APPZ(num_len+1);//free it in AT task
    if (p_sms_data->num == PNULL)
    {
        SCI_FREE(p_sms_data);
        return FALSE;
    }
    
    SCI_MEMCPY(p_sms_data->num,num_str,num_len);
    p_sms_data->num_len = num_len;

    if(dcs == 1 || dcs == 0) // gbk码要转成UCS2
    {
        p_sms_data->msg = SCI_ALLOC_APPZ(msg_len*2+2);//free it in AT task
        if (p_sms_data->msg == PNULL)
        {
            SCI_FREE(p_sms_data->num);
            SCI_FREE(p_sms_data);
            return FALSE;
        }
        p_sms_data->msg_len = gb2312_to_ucs2(msg_str, msg_len, (wchar *)p_sms_data->msg, msg_len);
        p_sms_data->msg_len = p_sms_data->msg_len*2;
        dcs = 2;
    }
    else if(dcs == 2)
    {
        p_sms_data->msg = SCI_ALLOC_APPZ(msg_len+2);//free it in AT task
        if (p_sms_data->msg == PNULL)
        {
            SCI_FREE(p_sms_data->num);
            SCI_FREE(p_sms_data);
            return FALSE;
        }
        SCI_MEMCPY(p_sms_data->msg,msg_str,msg_len);
        p_sms_data->msg_len = msg_len;
    }
    else
    {
        p_sms_data->msg = SCI_ALLOC_APPZ(msg_len*2+2);//free it in AT task
        if (p_sms_data->msg == PNULL)
        {
            SCI_FREE(p_sms_data->num);
            SCI_FREE(p_sms_data);
            return FALSE;
        }
        p_sms_data->msg_len = gb2312_to_ucs2(msg_str, msg_len, (wchar *)p_sms_data->msg, msg_len);
        p_sms_data->msg_len = p_sms_data->msg_len*2;
        dcs = 2;
    }
    
    p_sms_data->times = repeat_times;
    
    if(send_type == ZDT_GPRS_SMS_MSG)
    {
        p_sms_data->is_delay = 1;
    }
    else
    {
        p_sms_data->is_delay = 0;
    }
    
    p_sms_data->dual_sys = dual_sys;
    p_sms_data->send_type = send_type;
    p_sms_data->dcs = dcs;

#if 1
    MMIZDT_SMS_SendSigTo_APP(ZDT_APP_SIGNAL_SMS_SEND,p_sms_data);
#else
    res = ZDT_SMS_Link_Add(p_sms_data);
    if(res)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SMS_SEND,PNULL,0);
    }
    else
    {
        SCI_FREE(p_sms_data->msg);
        SCI_FREE(p_sms_data->num);
        SCI_FREE(p_sms_data);
    }
#endif
    return res;
        
}

BOOLEAN  MMIZDT_SMS_AppSend(MN_DUAL_SYS_E dual_sys,uint8 * msg_str,uint16 msg_len,uint8 * num_str,uint16 num_len,uint8 dcs,uint16 repeat_times)
{
    return MMIZDT_SMS_AppSendType(ZDT_SMS_MSG,dual_sys,msg_str,msg_len,num_str,num_len,dcs,repeat_times);
}

BOOLEAN  MMIZDT_SMS_AppReSend(void)
{
    return MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SMS_RESEND,PNULL,0);
}

BOOLEAN  MMIZDT_SMS_Handle_Send(DPARAM param)
{
#if 1
    BOOLEAN res = FALSE;
    MMI_ZDT_SMS_SIG_T * pp_getdata = (MMI_ZDT_SMS_SIG_T *)(param);
    MMI_ZDT_SMS_DATA_T * p_getdata = pp_getdata->p_sms_data;
    if(p_getdata == PNULL)
    {
        return FALSE;
    }
    
    res = ZDT_SMS_Link_Add(p_getdata);
    if(res == FALSE)
    {
        if(p_getdata->msg != NULL)
        {
            SCI_FREE(p_getdata->msg);
        }
        if(p_getdata->msg != NULL)
        {
            SCI_FREE(p_getdata->num);
        }
        SCI_FREE(p_getdata);
        return FALSE;
    }
    
    if(s_sms_is_poping_data == FALSE)
    {
        MMIZDT_SMS_PopSend(FALSE,ZDT_SMS_MSG);
    }
#else
    MMI_ZDT_DATA_T * p_getdata = (MMI_ZDT_DATA_T *)param;

    ZDTAPP_SMS_TRACE("MMIZDT_SMS_Handle_Send is_poping =%d",s_sms_is_poping_data);
    if(s_sms_is_poping_data == FALSE)
    {
        MMIZDT_SMS_PopSend(FALSE,ZDT_SMS_MSG);
    }
    
    if(p_getdata->str != NULL)
    {
        SCI_FREE(p_getdata->str);
    }

#endif
    return TRUE;
}

BOOLEAN  MMIZDT_SMS_Handle_ReSend(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    BOOLEAN res = FALSE;    
    if(p_getdata->str != NULL)
    {
        SCI_FREE(p_getdata->str);
    }
    
    ZDTAPP_SMS_TRACE("MMIZDT_SMS_Handle_ReSend is_poping =%d,p_getdata=0x%x",s_sms_is_poping_data,s_sms_cur_pop_pdata);

    if(s_sms_is_poping_data == FALSE)
    {
        MMIZDT_SMS_PopSend(FALSE,ZDT_SMS_MSG);
    }
    else
    {
        if(s_sms_cur_pop_pdata != PNULL)
        {
            res = ZDT_SMS_Send_Start(s_sms_cur_pop_pdata);
            if(res == FALSE)
            {
                MMIZDT_SMS_PopSend_Result(3);
            }
        }
        else
        {
            s_sms_is_poping_data = FALSE;
            MMIZDT_SMS_PopSend(FALSE,ZDT_SMS_MSG);
        }
    }
    return TRUE;
}

BOOLEAN  MMIZDT_SMS_Handle_SendOver(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);

    if(p_getdata->len == 0)
    {
        //成功
        ZDTAPP_SMS_TRACE("MMIZDT_SMS_Handle_SendOver OK,is_poping =%d,p_getdata=0x%x",s_sms_is_poping_data,s_sms_cur_pop_pdata);
        if(s_sms_is_poping_data)
        {
            if(s_sms_cur_pop_pdata != PNULL)
            {
                if(s_sms_cur_pop_pdata->msg != NULL)
                {
                    SCI_FREE(s_sms_cur_pop_pdata->msg);
                }

                if(s_sms_cur_pop_pdata->num != NULL)
                {
                    SCI_FREE(s_sms_cur_pop_pdata->num);
                }
                SCI_FREE(s_sms_cur_pop_pdata);
            }
            s_sms_is_poping_data = FALSE;
            MMIZDT_SMS_PopSend(FALSE,ZDT_SMS_MSG);
        }
        else
        {
            MMIZDT_SMS_AppReSend();
        }
    }
    else
    {
        //失败
        ZDTAPP_SMS_TRACE("MMIZDT_SMS_Handle_SendOver FAIL, is_poping =%d,p_getdata=0x%x",s_sms_is_poping_data,s_sms_cur_pop_pdata);
        if(s_sms_is_poping_data)
        {
            if(s_sms_cur_pop_pdata != PNULL)
            {
                if(s_sms_cur_pop_pdata->times > 0)
                {
                    s_sms_cur_pop_pdata->times--;
                }
                else
                {
                    if(s_sms_cur_pop_pdata->msg != NULL)
                    {
                        SCI_FREE(s_sms_cur_pop_pdata->msg);
                    }

                    if(s_sms_cur_pop_pdata->num != NULL)
                    {
                        SCI_FREE(s_sms_cur_pop_pdata->num);
                    }
                    SCI_FREE(s_sms_cur_pop_pdata);
                    s_sms_is_poping_data = FALSE;
                }
            }
            else
            {
                s_sms_is_poping_data = FALSE;
            }
        }
        MMIZDT_SMS_AppReSend();
    }
    
    if(p_getdata->str != NULL)
    {
        SCI_FREE(p_getdata->str);
    }

    return TRUE;
}

MMI_RESULT_E  MMIZDT_SMS_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    switch(msg_id)
    {                       
        case ZDT_APP_SIGNAL_SMS_SEND:
                MMIZDT_SMS_Handle_Send(param);
            break;
            
        case ZDT_APP_SIGNAL_SMS_RESEND:
                MMIZDT_SMS_Handle_ReSend(param);
            break;
            
        case ZDT_APP_SIGNAL_SMS_SEND_OVER:
                MMIZDT_SMS_Handle_SendOver(param);
            break;
            
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}

#ifdef ZDT_GPS_SUPPORT
BOOLEAN  MMIZDT_GPS_SMS_Send_Loc(BOOLEAN loc_ok)
{
    uint16  Sendbuf[65] = {0};
    uint16 num_len = 0;
    uint16 len = 0;
    uint16 send_len = 0;
    
    if(FALSE == MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
    {
        return FALSE;
    }
    
    #ifdef ZDT_PRODUCT_Z805
        num_len = strlen(g_z805_cfg_data.gps_num);
        if(num_len == 0)
        {
            return FALSE;
        }
        #ifdef WIN32
            //定位失败!5B9A 4F4D 5931 8D25 0021 
            Sendbuf[0] = 0x5B9A;
            Sendbuf[1] = 0x4F4D;
            Sendbuf[2] = 0x5931;
            Sendbuf[3] = 0x8D25;
            Sendbuf[4] = 0x0021;
            send_len = 5;
        #else
            {
                GPS_DATA_T pos_data = {0};
                GPS_API_GetLastPos(&pos_data);
                #if 0
                    pos_data.is_valid = 1;
                    pos_data.Lat_Dir = 'N';
                    pos_data.Latitude = 22.552072 * 180000;
                    pos_data.Long_Dir = 'E';
                    pos_data.Longitude = 113.921288 * 180000;
                    pos_data.date = 0x100901;
                    pos_data.time = (12*3600000) + (30*60000) + (30*1000);
                #endif
                
                if(pos_data.is_valid && loc_ok)
                {
                    len = GPS_Data_Get_WStrLong(pos_data.Long_Dir,pos_data.Longitude,&Sendbuf[send_len]);
                    send_len += len;
                    len = GPS_Data_Get_WStrLat(pos_data.Lat_Dir,pos_data.Latitude,&Sendbuf[send_len]);
                    send_len += len;
                    len = GPS_Data_Get_WStrLocalDateTime(pos_data.date,pos_data.time,&Sendbuf[send_len]);
                    send_len += len;
                }
                else
                {
                    //定位失败!5B9A 4F4D 5931 8D25 0021 
                    Sendbuf[0] = 0x5B9A;
                    Sendbuf[1] = 0x4F4D;
                    Sendbuf[2] = 0x5931;
                    Sendbuf[3] = 0x8D25;
                    Sendbuf[4] = 0x0021;
                    send_len = 5;
                }
            }
        #endif
        return MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)Sendbuf,send_len*2,g_z805_cfg_data.gps_num,num_len,2,3);
    #endif
    return FALSE;
}
#endif
#ifdef ZDT_PRODUCT_Z805
BOOLEAN  MMIZDT_SOS_SMS_Send(void)
{
    uint16  Sendbuf[120] = {0};
    uint16 send_len = 0;
    uint16 num_len = 0;
    uint16 len = 0;
    BOOLEAN res = FALSE;
    uint16 send_time = 0;

    if(FALSE == MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1))
    {
        return FALSE;
    }
    
    num_len = strlen(g_z805_cfg_data.sms_num);
    if(num_len == 0)
    {
        return FALSE;
    }

    //紧急呼叫！7D27 6025 547C 53EB FF01
    Sendbuf[0] = 0x7D27;
    Sendbuf[1] = 0x6025;
    Sendbuf[2] = 0x547C;
    Sendbuf[3] = 0x53EB;
    Sendbuf[4] = 0xFF01;
    send_len = 5;
    #ifndef WIN32
    #ifdef ZDT_GPS_SUPPORT
        {
            GPS_DATA_T pos_data = {0};
            GPS_API_GetLastPos(&pos_data);
            if(pos_data.is_valid)
            {
                len = GPS_Data_Get_WStrLong(pos_data.Long_Dir,pos_data.Longitude,&Sendbuf[send_len]);
                send_len += len;
                len = GPS_Data_Get_WStrLat(pos_data.Lat_Dir,pos_data.Latitude,&Sendbuf[send_len]);
                send_len += len;
                len = GPS_Data_Get_WStrLocalDateTime(pos_data.date,pos_data.time,&Sendbuf[send_len]);
                send_len += len;
            }
        }
    #endif
    #endif
    send_time = g_z805_cfg_data.sms_sum;
    if(send_time == 0)
    {
        send_time = 1;
    }
    else if(send_time > 10)
    {
        send_time = 10;
    }
    
    do
    {
        MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)Sendbuf,send_len*2,g_z805_cfg_data.sms_num,num_len,2,3);
        send_time--;
    }
    while(send_time);
    return TRUE;
}
#endif
BOOLEAN  MMIZDT_SMS_Test(void)
{
    uint16  Sendbuf[65] = {0};
    uint16 len = 0;
    //定位失败!5B9A 4F4D 5931 8D25 0021 
    Sendbuf[0] = 0x5B9A;
    Sendbuf[1] = 0x4F4D;
    Sendbuf[2] = 0x5931;
    Sendbuf[3] = 0x8D25;
    Sendbuf[4] = 0x0021;
    
    MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)Sendbuf,10,(uint8 *)"18948735889",11,2,3);
    return FALSE;
}

#endif

