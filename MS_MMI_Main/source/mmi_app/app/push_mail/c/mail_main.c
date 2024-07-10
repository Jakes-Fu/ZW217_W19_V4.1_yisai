/****************************************************************************
** File Name:      mail_main.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MAIL_MAIN_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_push_mail_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mail_main.h"
#include "mail_setting.h"
#include "IN_message.h"
#include "comm_platform.h"
#include "mail_network.h"
#include "comm_memory.h"
#include "mail_retr.h"
#include "mail_type.h"

#include "mmimail_export.h"
#include "mmimail_internal.h"
#include "mmi_appmsg.h"
#include "mail_mem.h"
#include "mmi_module.h"
#include "mail_fldr.h"
#include "sci_api.h"
#include "mail_time.h"
#include "mail_send.h"
#include "mail_attachment.h"
#include "mail_file.h"
#include "os_api.h"
#include "mail_sig.h"
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
#include "mmisms_export.h"
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */ 
#include "mmiudisk_export.h"
#include "mail_message.h"
#include "priority_app.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAIL_MAIN_TASK                   "MAIL_MAIN_Task"
#define MAIL_MAIN_TASK_QUEUE         "MAIL_MAIN_Task_Queue"
#define MAIL_MAIN_STACK_SIZE         (8*1024)
#define MAIL_MAIN_QUEUE_NUM        32 
#define MAIL_MAIN_TASK_PRIO          PRI_MAIL_MAIN_TASK
#define MAIL_MAIN_TASK_PREEMPT   SCI_PREEMPT 
#define MAIL_MAIN_AUTO_START       SCI_AUTO_START


#define MAIL_PARSE_TASK                   "MAIL_parse_Task"
#define MAIL_PARSE_TASK_QUEUE      "MAIL_parse_Task_Queue"
#define MAIL_PARSE_STACK_SIZE         (8*1024)
#define MAIL_PARSE_QUEUE_NUM        32 
#define MAIL_PARSE_TASK_PRIO          PRI_MAIL_PARSE_TASK_PRIO
#define MAIL_PARSE_TASK_PREEMPT   SCI_PREEMPT 
#define MAIL_PARSE_AUTO_START       SCI_AUTO_START

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//存放未解析邮件列表
typedef struct mail_unparsed_list_t_
{
    wchar file_name[MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1];
    int accountid;
    struct mail_unparsed_list_t_ *next;
    
}mail_unparsed_list_t;

//定义结构体用于保存手机邮箱全局数据
typedef struct mail_main_info_t_
{
    comm_boolean  is_initial;           //初始化
    comm_boolean  is_recving;       //接收中
    comm_boolean  is_sending;      //发送中
    comm_boolean  is_parsing;       //解析中
    comm_boolean  need_recving;  //需要接收
    comm_boolean  need_sending; //需要发送
    comm_boolean  is_autorecving; //自动接收
    comm_boolean  is_sslpasser;    //ssl 处理中
    comm_boolean  user_cancel_sending;    //用户取消
    int sock_needclose;    //sock session 需要关闭
    int total_message_num;
    int unrecv_message_num;
    int free_space_size;
    int unparsed_mail_num;
    mail_unparsed_list_t *mail_list;
    
}mail_main_info_t;

LOCAL comm_task_id s_mail_task = COMM_INVALID_BLOCK_ID;

LOCAL comm_task_id s_mail_parse_task = COMM_INVALID_BLOCK_ID;

//手机邮箱状态位
//开机置位- 随时更新状态
mail_main_info_t      s_mail_app_info = {0};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern void mail_SignalProcess (EStateMachine machine);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
comm_boolean g_mail_is_recv_timeout = FALSE;/*lint -e552 */

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/ 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*==========================================================
 * Function     : mail_mainInit
 * Author	    : sunguochen      
 * Description : mail main init
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_mainInit(void)
{
    memset(&s_mail_app_info, 0, sizeof(mail_main_info_t));

    s_mail_app_info.is_initial = TRUE;
}

/*==========================================================
 * Function     : mail_SendSignaltoMailTask
 * Author	    : sunguochen      
 * Description : 发送事件给mail线程
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_SendSignaltoMailTask(
                             MSG_MAIL_EVENT_TYPE event, 
                             int data
                             )
{
    MAIL_SIG_T *sig_ptr = PNULL;
    comm_task_id cur_task_id = COMM_INVALID_BLOCK_ID;  

    sig_ptr = (MAIL_SIG_T *)comm_alloc(sizeof(MAIL_SIG_T));

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail mail_SendSignaltoMailTask PNULL == sig_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_159_112_2_18_2_47_44_57,(uint8*)"");
        return ;
    }
    cur_task_id = mail_GetThreadId();

    sig_ptr->SignalCode = (uint16)event;
    sig_ptr->SignalSize = sizeof(xSignalHeaderRec);
    sig_ptr->Sender = comm_IdentifyThread( );
    sig_ptr->data = data;

    //COMM_TRACE:"MAIL: mail_SendSignaltoMailTask sender:%d cur: %d .event: %x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_169_112_2_18_2_47_44_58,(uint8*)"ddd",sig_ptr->Sender, cur_task_id, event);

    COMM_SendSignal((xSignalHeader)sig_ptr, cur_task_id);
}

/*==========================================================
 * Function     : mail_SendSignaltoMailTask
 * Author	    : sunguochen      
 * Description : 发送事件给aPP线程
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_SendSignaltoAppTask(
                             int event, 
                             int data
                             )
{
    MAIL_SIG_T *sig_ptr = PNULL;
    comm_task_id cur_task_id = COMM_INVALID_BLOCK_ID;  
    comm_task_id sender_task_id = COMM_INVALID_BLOCK_ID;  

    cur_task_id = COMM_P_APP_ID;
    sender_task_id = comm_IdentifyThread();

    SCI_CREATE_SIGNAL(sig_ptr, event, sizeof(MAIL_SIG_T), sender_task_id);
    //COMM_ASSERT(sig_ptr);

    sig_ptr->data = data;

    //COMM_TRACE:"MAIL: mail_SendSignaltoMailTask sender:%d cur: %d .event: %x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_199_112_2_18_2_47_44_59,(uint8*)"ddd",sig_ptr->Sender, cur_task_id, event);

    // Send the signal to the dedicated task's queue
    COMM_SendSignal((xSignalHeader)sig_ptr, cur_task_id);
}

/*==========================================================
 * Function     : mail_SendSignaltoMailParseTask
 * Author	    : sunguochen      
 * Description : 发送事件给网络线程
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_SendSignaltoMailParseTask(
                             MSG_MAIL_EVENT_TYPE event, 
                             int data
                             )
{
      MAIL_SIG_T *sig_ptr = PNULL;
      comm_task_id cur_task_id = COMM_INVALID_BLOCK_ID;  

      sig_ptr = (MAIL_SIG_T *)comm_alloc(sizeof(MAIL_SIG_T));

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail mail_SendSignaltoMailParseTask PNULL == sig_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_225_112_2_18_2_47_44_60,(uint8*)"");
        return ;
    }
      cur_task_id = s_mail_parse_task;
      sig_ptr->SignalCode = (uint16)event;
      sig_ptr->SignalSize = sizeof(xSignalHeaderRec);
      sig_ptr->Sender = comm_IdentifyThread( );
      sig_ptr->data = data;

      //COMM_TRACE:"MAIL: mail_SendSignaltoMailParseTask sender:%d cur: %d .event: %x\n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_234_112_2_18_2_47_44_61,(uint8*)"ddd",sig_ptr->Sender, cur_task_id, event);
      COMM_SendSignal((xSignalHeader)sig_ptr, cur_task_id);
}

/*==========================================================
 * Function     : mail_handleTaskEvent
 * Author	    : sunguochen      
 * Description : mail process task handler
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/7/2010 
 ==========================================================*/
void mail_handleTaskEvent(uint32 argc, void *argv) 
{
    comm_uint32                      sock_id = 0;
    comm_uint32                      sock_err = 0;
    MAIL_SIG_T *           sig_ptr = NULL;

    while (TRUE) /*lint !e716 */
    {
        sig_ptr = (MAIL_SIG_T *)comm_GetSignal(comm_IdentifyThread());

        if (PNULL == sig_ptr)
        {
            //SCI_TRACE_LOW:"MMIMail mail_handleTaskEvent PNULL == sig_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_258_112_2_18_2_47_44_62,(uint8*)"");
            return ;
        }

        //COMM_TRACE:"Mail: mail_handleTaskEvent signal id: %x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_262_112_2_18_2_47_44_63,(uint8*)"d", sig_ptr->SignalCode);

        switch (sig_ptr->SignalCode)
        {
            case MSG_MAIL_NET_NOTIFY:
                //网络状态处理函数
                mail_NetNotifyHandler(sig_ptr->data);
                break;

            case MSG_MAIL_NET_ERROR:
                mail_NetworkErrorProcess(sig_ptr->data);
                //网络错误处理函数
                break;

            case MSG_MAIL_NET_QUERY:
                //网络查询
                mail_NetStateQuery((mail_net_sock_t*) sig_ptr->data);
                break;

            case MSG_MAIL_NET_RESEND:
                //重新/继续 发送
                SCI_Sleep(10);
                //COMM_TRACE:"Mail:start resend!!!!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_284_112_2_18_2_47_44_64,(uint8*)"");

                if(mail_NetSessionValid((mail_net_sock_t *) sig_ptr->data))
                {

                    //session is termated
                    mail_net_sock_t * net_id = (mail_net_sock_t *) sig_ptr->data;
                    net_id->sock_is_resending = FALSE;                    
                }
                mail_NetDoSend((mail_net_sock_t*) sig_ptr->data);
                break;

            case MSG_MAIL_NET_RERETR:
                //重复接收
                SCI_Sleep(10);
                mail_NetDataRecvProcess((int) sig_ptr->data);
                break;

            case MSG_MAIL_ACTION_EVENT:
                //发送时的事件处理
                mail_SignalProcess(E_FSM_MAIL_MAIN);
                break;

            case MSG_MAIL_STOP_EVENT:
                //结束当前发送接收操作
                mail_HandleStopAction((MAIL_SESSION_STOP_STATE_E)sig_ptr->data);
                break;

            case MSG_MAIL_TIME_OUT:
                if(mail_GetRecvingState())
                {
                    g_mail_is_recv_timeout = TRUE;
                    //COMM_TRACE:"Mail: MSG_MAIL_TIME_OUT mail recv timeout!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_316_112_2_18_2_47_44_65,(uint8*)"");
                }
                mail_timer_taskcallback((comm_uint32) sig_ptr->data);
                break;

            case MSG_MAIL_SEND_START:
                {
                    MAIL_EVENT_MULTIPARA *send_papr = NULL;
                
                    send_papr = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;
                    mail_smtpsendStart((int)send_papr->data1, (MAIL_MESSAGE_T*)send_papr->data2, 
                                                  (char*)send_papr->data3, (char*)send_papr->data4);
                    comm_free(send_papr);
                    //mail_StartSendMsgNormal((int)sig_ptr->data);
                }
                break;

            case MSG_MAIL_RETR_START:
                if(mail_GetRecvingState())
                {
                    //COMM_TRACE:"Mail: MSG_MAIL_RETR_START is retring "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_336_112_2_18_2_47_44_66,(uint8*)"");
                    break;
                }
                g_mail_is_recv_timeout= FALSE;
                //COMM_TRACE:"Mail: MSG_MAIL_RETR_START Reset g_mail_is_recv_timeout to FALSE!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_340_112_2_18_2_47_44_67,(uint8*)"");
                mail_StartRetrMsgNormal((mail_retr_method_t *)sig_ptr->data);
                break;

            case MSG_MAIL_NET_SSL_OK:
                mail_sslDescryptOk((mail_net_sock_t *)sig_ptr->data);
                break;

            case MSG_MAIL_NET_SSL_EVENT:
                {
                    MAIL_EVENT_MULTIPARA *send_papr = NULL;

                    send_papr = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;
                    mail_sslEventProcess((mail_net_sock_t *)send_papr->data1, send_papr->data2);
                    comm_free(send_papr);
                 }
                break;

            case MSG_MAIL_NET_CLOSE_DELAY:
                {
                    int sock_session = 0;

                    sock_session = mail_GetSockNeedCloseState();
                    mail_SetSockNeedCloseState(0);
                    mail_NetCloseSession((mail_net_sock_t *)sock_session);
                }
                break;

            case MSG_MAIL_NET_PDP_DEACTIVE:
                mail_NetPdpDeactiveDelay( );
                break;
         
            case SOCKET_READ_EVENT_IND://data arrival
                sock_id = ((SOCKET_READ_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                //COMM_TRACE:"Mail: comm_NetTask data arrival "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_374_112_2_18_2_47_44_68,(uint8*)"");
                mail_SendSignaltoMailTask(MSG_MAIL_DATA_ARRIVAL, sock_id );
                SCI_FREE(sig_ptr);
                sig_ptr = NULL;
                break;

            case MSG_MAIL_DATA_ARRIVAL://data arrival
                //COMM_TRACE:"Mail: MSG_MAIL_DATA_ARRIVAL "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_381_112_2_18_2_47_44_69,(uint8*)"");
                mail_NetDataRecvProcess( sig_ptr->data );
                comm_free(sig_ptr);/*lint !e774*/
                sig_ptr = NULL;
                break;                
            case SOCKET_WRITE_EVENT_IND://can send data
                 sock_id = ((SOCKET_WRITE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                 //COMM_TRACE:"Mail: comm_NetTask data can send "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_388_112_2_18_2_47_44_70,(uint8*)"");
                 mail_SendSignaltoMailTask(MSG_MAIL_DATA_SEND, sock_id );                 
                 SCI_FREE(sig_ptr);
                 sig_ptr = NULL;                 
                 break;

            case MSG_MAIL_DATA_SEND://can send data
                 //COMM_TRACE:"Mail: MSG_MAIL_DATA_SEND "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_395_112_2_18_2_47_44_71,(uint8*)"");
                 mail_NetDataSentProcess( sig_ptr->data );
                 comm_free(sig_ptr);/*lint !e774*/
                 sig_ptr = NULL;                 
                 break;

            case SOCKET_CONNECT_EVENT_IND://connect ok
                 sock_id = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                 sock_err = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->error_code;
                 //COMM_TRACE:"Mail: comm_NetTask connect error = %d "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_404_112_2_18_2_47_44_72,(uint8*)"d", sock_err);
                 mail_NetConnectedProcess(sock_id, sock_err);
                 SCI_FREE(sig_ptr);
                 sig_ptr = NULL;                 
                 break;

            case SOCKET_CONNECTION_CLOSE_EVENT_IND://connection is closed
                 sock_id = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                 sock_err = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)sig_ptr)->error_code;
                 //COMM_TRACE:"Mail: comm_NetTask server close this connection err: %d\n "
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_413_112_2_18_2_47_44_73,(uint8*)"d", sock_err);
                 if(sock_err != 0)
                 {
                    //连接异常结束
                    mail_NetConnectionClosedProcess(sock_id);
                 }
                 SCI_FREE(sig_ptr);
                 sig_ptr = NULL;                 
                 break;

            default :
                //COMM_TRACE:"Mail:comm_NetTask unkonwn event :%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_424_112_2_18_2_47_44_74,(uint8*)"d", sig_ptr->SignalCode);
                break;
        }

        if(sig_ptr)
        {
            comm_free(sig_ptr);/*lint !e774*/
            sig_ptr = NULL;
        }
    }
}



/*==========================================================
 * Function     : mail_handleParseTaskEvent
 * Author	    : sunguochen      
 * Description : mail process task handler
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/7/2010 
 ==========================================================*/
void mail_handleParseTaskEvent(uint32 argc, void *argv) 
{
    MAIL_SIG_T *           sig_ptr = NULL;
    MAIL_EVENT_MULTIPARA * multi_para = NULL;

    while (TRUE) /*lint !e716 */
    {
        sig_ptr = (MAIL_SIG_T *)comm_GetSignal(comm_IdentifyThread());

        if (PNULL == sig_ptr)
        {
            //SCI_TRACE_LOW:"MMIMail mail_handleParseTaskEvent PNULL == sig_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_457_112_2_18_2_47_44_75,(uint8*)"");
            return ;
        }

        //COMM_TRACE:"Mail: mail_handleParseTaskEvent signal id: %x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_461_112_2_18_2_47_44_76,(uint8*)"d", sig_ptr->SignalCode);

        switch (sig_ptr->SignalCode)
        {
            case MSG_MAIL_ACTION_EVENT:
                //发送时的事件处理
                mail_SignalProcess(E_FSM_FLDR_MANAGER);
                break;

            case MSG_MAIL_HEADER_START:
                //COMM_TRACE:"Mail: MSG_MAIL_HEADER_START \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_471_112_2_18_2_47_44_77,(uint8*)"");
                multi_para = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;
                mail_GetMailMessageForUi(multi_para->data1, (MMIMAIL_BOX_INFO_T *)multi_para->data2);
                //这里扩展参数,因此需要释放sig_ptr.data
                comm_free(multi_para);
                //call fdl function
                comm_free(sig_ptr);/*lint !e774*/
                sig_ptr = NULL;
                break;

            case MSG_MAIL_HEADER_FINISH:
                //COMM_TRACE:"Mail: MSG_MAIL_HEADER_FINISH  \n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_482_112_2_18_2_47_44_78,(uint8*)"");
                //call mmi finished function
                mail_SendSignaltoAppTask(MSG_MAIL_CLOSE_WAITING_WIN, sig_ptr->data);
                comm_free(sig_ptr);/*lint !e774*/
                sig_ptr = NULL;
                break;

            case MSG_MAIL_RECV_PARSE_START:
                {
                //保存
                //////////////////
                    MAIL_EVENT_MULTIPARA *parse_papr = NULL;

                    parse_papr = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;           

                    mail_SetParsingState( TRUE );
                    mail_GetMailMessageFromParser((wchar *)parse_papr->data1, (int)parse_papr->data2);

                    //mail_SetParsingState( FALSE );
                    comm_free(parse_papr);
                    comm_free(sig_ptr);/*lint !e774*/
                    sig_ptr = NULL;
                }             
                break;                
                
            case MSG_MAIL_VIEW_PARSE_START:                
                {

                    MAIL_MESSAGE_T *message_ptr = PNULL;                
                    char* mail_name = NULL;
                    comm_uint16* file_name = NULL;                    
                    //开始解析邮件
                    mail_name = mail_getMailNameByMsgId((comm_uint32)sig_ptr->data);

                    mail_fldrmgrSetReadFlag((comm_uint32)sig_ptr->data, TRUE);
                    
                    file_name = comm_alloc((strlen(mail_name)+1)*sizeof(comm_uint16));
                    comm_str2wstr(file_name, mail_name);                
                    message_ptr =MMIEMAIL_CreateMessage(file_name, 1000, Inbox);

                    if(message_ptr && message_ptr->uidl == NULL)
                    {
                        char *uidl = NULL;
                        char *uidl_tmp = NULL;
                        //获得UIDL
                        uidl_tmp = mail_getMailUidlByMsgId(sig_ptr->data);
                        if(uidl_tmp)
                        {
                            uidl = comm_alloc(strlen(uidl_tmp) + 1);
                            strcpy(uidl, uidl_tmp);
                            message_ptr->uidl = (uint8 *)uidl;
                        }
                    }

                    if(!message_ptr)
                    {
                        //open file error
                        mail_ViewMailFinished((int)message_ptr); 
                    }
                    else
                    {
                        //start parse
                        mail_SendSignaltoMailParseTask(MSG_MAIL_VIEW_PARSE_BEGIN, (int)message_ptr);
                    }
                    comm_free(file_name);
                    
                }
                break;

            case MSG_MAIL_VIEW_PARSE_BEGIN:
                {

                    MAIL_MESSAGE_T *message_ptr = PNULL;

                    message_ptr = (MAIL_MESSAGE_T *)sig_ptr->data;
                    
                    if(message_ptr)
                    {
                        message_ptr->fine = TRUE;
                        ParseMessage(message_ptr, TRUE);

                    }
                    mail_ViewMailFinished((int)message_ptr);                    
                }
                break;
                
            case MSG_MAIL_OPERATE_DEL_START:
                
                mail_deleteMsgForUi();           
                break;
                
            case MSG_MAIL_OPERATE_DEL_CANCLE:
                setDelProcessState(TRUE);
                break;

            case MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_578_112_2_18_2_47_44_79,(uint8*)"");

                multi_para = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;
                (void)mail_sendSaveMsg((comm_int32)multi_para->data1,(MAIL_MESSAGE_T*)multi_para->data2,
                                  (char*)multi_para->data3);
                comm_free(multi_para);
                //call fdl function
                comm_free(sig_ptr);/*lint !e774*/
                sig_ptr = NULL;
                break;
            
            case MSG_MAIL_SEND_NEXT:
                //COMM_TRACE:"Mail: MSG_MAIL_SEND_NEXT \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_590_112_2_18_2_47_44_80,(uint8*)"");
                mail_sendOkHandle((mail_retr_result)sig_ptr->data);
                break;
                
            case MSG_MAIL_OPERATE_SEND_MSG:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_SEND_MSG \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_595_112_2_18_2_47_44_81,(uint8*)"");
                multi_para = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;
                (void)mail_sendMsgByMsgId((comm_int32)multi_para->data1,(comm_uint32)multi_para->data2);
                comm_free(multi_para);
                //call fdl function
                comm_free(sig_ptr);/*lint !e774*/
                sig_ptr = NULL;
                break;
                
            case MSG_MAIL_OPERATE_READ_SORT:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_READ_SORT \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_605_112_2_18_2_47_44_82,(uint8*)"");
                (void)mail_MailListSort(MAIL_INBOX,MAIL_SORT_BY_STATE, TRUE);
                (void)mail_MailListSort(MAIL_INBOX,MAIL_SORT_BY_TIME,TRUE);
                break;
                
            case MSG_MAIL_OPERATE_SAVE_OUTBOX:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_SAVE_OUTBOX \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_611_112_2_18_2_47_44_83,(uint8*)"");
                multi_para = (MAIL_EVENT_MULTIPARA *)sig_ptr->data;
                (void)mail_saveMailToDraft((comm_int32)multi_para->data1,(MAIL_MESSAGE_T *)multi_para->data2);
                comm_free(multi_para);
                comm_free(sig_ptr);/*lint !e774*/
                sig_ptr = NULL;
                break;
                
            case MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_620_112_2_18_2_47_45_84,(uint8*)"");
                mail_deleteAccountMsgForUi((comm_int32)sig_ptr->data);
                break;
                
            case MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_625_112_2_18_2_47_45_85,(uint8*)"");
                mail_deleteAllAccountMsgForUi();
                break;
                
            case MSG_MAIL_OPERATE_BACK_FACTORY:
                //COMM_TRACE:"Mail: MSG_MAIL_OPERATE_BACK_FACTORY \n "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_630_112_2_18_2_47_45_86,(uint8*)"");
                mail_ResetFactorySetting();
                //通知UI 复位完毕
                mail_backToFactoryFinish();
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
                MMIAPISMS_DeleteAllMailMsg();
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */                
                break;
                
            default :
                //COMM_TRACE:"Mail:comm_NetTask unkonwn event :%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_640_112_2_18_2_47_45_87,(uint8*)"d", sig_ptr->SignalCode);
                break;
        }

        if(sig_ptr)
        {
            //COMM_TRACE:"Mail: mail_handleParseTaskEvent free event memory \n "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_646_112_2_18_2_47_45_88,(uint8*)"");
            comm_free(sig_ptr);/*lint !e774*/
            sig_ptr = NULL;
        }
    }
}



/*==========================================================
 * Function     : mail_CreateThread
 * Author	    : sunguochen      
 * Description : 创建网络交互线程
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/7/2010 
 ==========================================================*/
comm_boolean mail_CreateThread(void)
{
    if(COMM_INVALID_BLOCK_ID == s_mail_task)
    {
        s_mail_task = comm_CreateThread( 
                                                (char *)MAIL_MAIN_TASK, 
                                                (char *)MAIL_MAIN_TASK_QUEUE, 
                                                mail_handleTaskEvent, 
                                                0, 
                                                0, 
                                                MAIL_MAIN_STACK_SIZE, 
                                                MAIL_MAIN_QUEUE_NUM, 
                                                MAIL_MAIN_TASK_PRIO, 
                                                MAIL_MAIN_TASK_PREEMPT, 
                                                MAIL_MAIN_AUTO_START);
    }    

    if(COMM_INVALID_BLOCK_ID == s_mail_parse_task)
    {
        s_mail_parse_task = comm_CreateThread( 
                                                (char *)MAIL_PARSE_TASK, 
                                                (char *)MAIL_PARSE_TASK_QUEUE, 
                                                mail_handleParseTaskEvent, 
                                                0, 
                                                0, 
                                                MAIL_PARSE_STACK_SIZE, 
                                                MAIL_PARSE_QUEUE_NUM, 
                                                MAIL_PARSE_TASK_PRIO, 
                                                MAIL_PARSE_TASK_PREEMPT, 
                                                MAIL_PARSE_AUTO_START);
    }    

    if(COMM_INVALID_BLOCK_ID == s_mail_task || COMM_INVALID_BLOCK_ID == s_mail_parse_task)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



/*==========================================================
 * Function     : mail_GetThreadId
 * Author	    : sunguochen      
 * Description : 获取邮箱线程ID
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/16/2010 
 ==========================================================*/
comm_task_id mail_GetThreadId(void)
{
    comm_boolean result = FALSE;

    if(COMM_INVALID_BLOCK_ID == s_mail_task)
    {
        //net thread is not ready ,create it
        result = mail_CreateThread( );
        if(!result)
        {
            //thread create fail
            COMM_ASSERT(FALSE);/*assert verified*/
        }
    }

    return s_mail_task;
}

/*==========================================================
 * Function     : mail_GetlHeadInfoStart
 * Author	    : sunguochen      
 * Description : 开始解析邮件列表信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_GetlHeadInfoStart(int accountid, void * header_info)
{
    MAIL_EVENT_MULTIPARA * event_para = NULL;

    event_para = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));

    event_para->data1 = accountid;
    event_para->data2 = (int) header_info;    
    mail_SendSignaltoMailParseTask(MSG_MAIL_HEADER_START, (int) event_para);
}



/*==========================================================
 * Function     : mail_GetlHeadInfoFinish
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_GetlHeadInfoFinish(comm_boolean result)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_HEADER_FINISH, (int)result);
}

/*==========================================================
 * Function     : mail_RecevMailFinish
 * Author	    : lixu  
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_RecevMailFinish(void)
{
    mail_SendSignaltoAppTask(MSG_MAIL_LIST_CHANGE, 0);
}


/*==========================================================
 * Function     : mail_SendRetrMsg
 * Author	    : sunguochen      
 * Description : 发送接收请求
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrMsgReq(int accountid)
{
    mail_retr_method_t* retr_mode = NULL;

    retr_mode = comm_alloc(sizeof(mail_retr_method_t));
   
    if (PNULL == retr_mode)
    {
        //SCI_TRACE_LOW:"MMIMail mail_SendRetrMsgReq PNULL == retr_mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_797_112_2_18_2_47_45_89,(uint8*)"");
        return ;
    }
    
    retr_mode->retr_mode = RETRIEVE_MODE_NORMAL;
    retr_mode->accountid = accountid;
    retr_mode->uidl         = NULL;
    retr_mode->attachid   = 0;

    mail_SendSignaltoMailTask(MSG_MAIL_RETR_START, (int)retr_mode);
}



/*==========================================================
 * Function     : mail_SendRetrMsg
 * Author	    : sunguochen      
 * Description : 发送接收请求-附件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrAttachReq(int accountid, mail_retr_method_t* retr_mode)
{
    mail_SendSignaltoMailTask(MSG_MAIL_RETR_START, (int)retr_mode);
}



/*==========================================================
 * Function     : mail_SendRetrEnableReq
 * Author	    : sunguochen      
 * Description : 暂停/恢复邮件接收
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrEnableReq(int accountid, comm_boolean is_enable)
{
    mail_retr_method_t* retr_mode = NULL;

    retr_mode = comm_alloc(sizeof(mail_retr_method_t));

    if (PNULL == retr_mode)
    {
        //SCI_TRACE_LOW:"MMIMail mail_SendRetrEnableReq PNULL == retr_mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_842_112_2_18_2_47_45_90,(uint8*)"");
        return ;
    }

    if(is_enable)
    {
        retr_mode->retr_mode = RETRIEVE_MODE_ENABLE;
    }
    else
    {
        retr_mode->retr_mode = RETRIEVE_MODE_DISABLE;        
    }
    retr_mode->accountid = accountid;
    retr_mode->uidl         = 0;
    retr_mode->attachid   = 0;
        
    mail_SendSignaltoMailTask(MSG_MAIL_RETR_START, (int)retr_mode);
}



/*==========================================================
 * Function     : mail_StartRetr
 * Author	    : sunguochen      
 * Description : 开始接收邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartRetrMsgNormal(mail_retr_method_t * rete_method)
{
    mail_retr_result_t     *retr_result = NULL;
    //接收总入口
    //判断网络情况及空间

    if(!mail_IsNetworkValid())
    {
        //error
        //COMM_TRACE:"Mail: network invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_880_112_2_18_2_47_45_91,(uint8*)"");
        switch(rete_method->retr_mode)
        {
            case RETRIEVE_MODE_NORMAL:
            case RETRIEVE_MODE_ATTACH:

                mail_SetRecvingState(FALSE);
                mail_SetAutoRecvingState(FALSE);//解决MS00227403
                retr_result = comm_alloc(sizeof(mail_retr_result_t));
                retr_result->retr_num = 0;
                retr_result->retr_result = MAIL_RETR_ERR_PDPFAIL;
                mail_RetrAttachNotify(retr_result);
                break;

            case RETRIEVE_MODE_ENABLE:
            case RETRIEVE_MODE_DISABLE:
                break;
                
            case RETRIEVE_MODE_PASSWD:
                mail_CheckPasswordResult(FALSE);
                break;

            default:
                break;            
        }
        return;
    }

    if(!mail_IsFreeSpaceOk())
    {
        //error
        //COMM_TRACE:"Mail: free space invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_911_112_2_18_2_47_45_92,(uint8*)"");

        switch(rete_method->retr_mode)
        {
            case RETRIEVE_MODE_NORMAL:
            case RETRIEVE_MODE_ATTACH:
                mail_SetRecvingState(FALSE);
                mail_SetAutoRecvingState(FALSE);//解决MS00227403
                retr_result = comm_alloc(sizeof(mail_retr_result_t));
                retr_result->retr_num = 0;
                retr_result->retr_result = MAIL_RETR_ERR_NOSPACE;
                mail_RetrAttachNotify(retr_result);
                return;

            default:
                break;            
        }        
    }

    if(mail_IsMailBoxFull())
    {
        //error
        //COMM_TRACE:"Mail: box is full 200 mail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_933_112_2_18_2_47_45_93,(uint8*)"");
        switch(rete_method->retr_mode)
        {
            case RETRIEVE_MODE_NORMAL:
            case RETRIEVE_MODE_ATTACH:
                mail_SetRecvingState(FALSE);
                mail_SetAutoRecvingState(FALSE);//解决MS00227403
                retr_result = comm_alloc(sizeof(mail_retr_result_t));
                retr_result->retr_num = 0;
                retr_result->retr_result = MAIL_RETR_ERR_PDPFAIL;
                mail_RetrAttachNotify(retr_result);
                return;

            default:
                break;            
        }        
    }    
    
    mail_SetRecvingState(TRUE);
    mail_RetrieveMsg(rete_method->accountid, (void *)mail_progressbar,  //lint !e611
                              rete_method->retr_mode, (void *)rete_method);
}



/*==========================================================
 * Function     : mail_SendRetrMsg
 * Author	    : sunguochen      
 * Description : 发送请求
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendSendMsgReq(int accountid)
{

    MAIL_EVENT_MULTIPARA *send_papr = NULL;

    send_papr = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));
    send_papr->data1 = (int)accountid;
    send_papr->data2 = (int)0;
    send_papr->data3 = (int)0;
    send_papr->data4 = (int)0;

    //发送事件给接收线程
    mail_SendSignaltoMailTask(MSG_MAIL_SEND_START, (int)send_papr);
}



/*==========================================================
 * Function     : mail_StartRetr
 * Author	    : sunguochen      
 * Description : 开始发送邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartSendMsgNormal(int accountid)
{
    mail_sendTest(accountid);
}




/*==========================================================
 * Function     : mail_StartViewMail
 * Author	    : sunguochen      
 * Description : 开始解析邮件-接收解析邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartRecvParseMail(comm_uint16* file_name, int accountid)
{
    MAIL_EVENT_MULTIPARA *parse_papr = NULL;

    parse_papr = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));
    parse_papr->data1 = (int)file_name;
    parse_papr->data2 = (int)accountid;

    if(!mail_getAppInitState())
    {
        //正在初始化-不发送事件-存入列表
        //COMM_TRACE:"Mail:mail_StartRecvParseMail fldr is not initial"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1018_112_2_18_2_47_45_94,(uint8*)"");
        mail_NewMailToParse(file_name, &accountid);        
        return;
    }

    if(mail_GetParsingState())
    {
        //正在解析-不发送事件-存入列表
        mail_NewMailToParse(file_name, &accountid);
    }
    else
    {
        mail_SetParsingState( TRUE );
        mail_SendSignaltoMailParseTask(MSG_MAIL_RECV_PARSE_START, (int)parse_papr);
    }
}


/*==========================================================
 * Function     : mail_StartViewMail
 * Author	    : sunguochen      
 * Description : 开始解析邮件-准备浏览邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartViewParseMail(int msgid)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_VIEW_PARSE_START, msgid);
}

/*==========================================================
 * Function     : mail_StartDelMail
 * Author	    : lixu     
 * Description : UI调用，通知parser task开始删除邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartDelMail(void)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_DEL_START,0);
}
/*==========================================================
 * Function     : mail_StartDelMail
 * Author	    : lixu     
 * Description : UI调用，通知parser task停止删除邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_CancleDelMail(void)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_DEL_CANCLE,0);
}
/*==========================================================
 * Function     : mail_DelMailFinish
 * Author	    : lixu     
 * Description : 删除结束后通知UI，结束等待
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_DelMailFinish(comm_boolean result)
{
    mail_SendSignaltoAppTask(MSG_MAIL_DELETE_FINISH,(int)result);
}

/*==========================================================
 * Function     : mail_sendMailStartSave
 * Author	    : lixu     
 * Description :  UI调用，发送邮件时通知parser task编码同时保存邮件到outbox
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailStartSave(comm_int32 account_id,MAIL_MESSAGE_T *message_ptr,
                                  char* fwd_uidl)
{
    MAIL_EVENT_MULTIPARA * event_para = NULL;

    event_para = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));
    event_para->data1 = account_id;
    event_para->data2 = (int) message_ptr;
    event_para->data3 = (int) fwd_uidl;

    mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX,(int)event_para);
}
/*==========================================================
 * Function     : mail_sortInboxMailList
 * Author	    : lixu     
 * Description :  已读邮件阅读后刷新收件箱邮件列表
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/

void mail_sortInboxMailList(void)
{
   mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_READ_SORT,0);
}

/*==========================================================
 * Function     : mail_saveMailToDraft
 * Author	    : lixu     
 * Description :  仅保存邮件到草稿箱
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/

void mail_saveMailToDrafts(comm_int32 account_id,MAIL_MESSAGE_T* message_ptr)
{
    MAIL_EVENT_MULTIPARA *event_para = NULL;

    event_para = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));
    event_para->data1 = (int)account_id;
    event_para->data2 = (int)message_ptr;

    mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_SAVE_OUTBOX,(int)event_para);
}

/*==========================================================
 * Function     : mail_delAccountAllMail
 * Author	    : lixu     
 * Description :  删除指定帐户邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_delAccountAllMail(comm_int32 account_id)
{
   mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG,(int)account_id);
}

/*==========================================================
 * Function     : mail_delAllAccountAllMail
 * Author	    : lixu     
 * Description :  删除所有帐户邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_delAllAccountAllMail(void)
{
   mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG,1);
}

/*==========================================================
 * Function     : mail_delAllAccountAllMailFinish
 * Author	    : lixu     
 * Description :  删除所有帐户邮件完成通知
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_delAllAccountAllMailFinish(void)
{
    mail_SendSignaltoAppTask(MSG_MAIL_DELETE_FINISH, 1);
}

/*==========================================================
 * Function     : mail_delAccountAllMailFinish
 * Author	    : lixu     
 * Description :  删除指定帐户邮件完成通知
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_delAccountAllMailFinish(void)
{
    mail_SendSignaltoAppTask(MSG_MAIL_DELETE_FINISH, 0);
}

/*==========================================================
 * Function     : mail_resetFactory
 * Author	    : lixu     
 * Description :  恢复出厂设置
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_resetFactory(void)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_BACK_FACTORY,0);
}

/*==========================================================
 * Function     : mail_backToFactoryFinish
 * Author	    : lixu     
 * Description :  恢复出厂设置完成
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_backToFactoryFinish(void)
{
    mail_SendSignaltoAppTask(MSG_MAIL_RESET_FACTORY_OVER,1);
}

/*==========================================================
 * Function     : mail_sendMailSendByMsgId
 * Author	    : lixu     
 * Description :  UI调用，通过msgId发送邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailSendByMsgId(comm_int32 accountId,comm_uint32 msgId)
{
    MAIL_EVENT_MULTIPARA * event_para = NULL;

    event_para = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));

    event_para->data1 = accountId;
    event_para->data2 = (int) msgId;

    mail_SendSignaltoMailParseTask(MSG_MAIL_OPERATE_SEND_MSG,(int)event_para);
}
/*==========================================================
 * Function     : mail_StartDelMail
 * Author	    : lixu     
 * Description :  发送邮件本地保存结束通知UI
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/

void mail_sendMailSaveOk(comm_boolean result)
{
  mail_SendSignaltoAppTask(MSG_MAIL_SAVE_FINISH, (int)result);
}

/*==========================================================
 * Function     : mail_StartDelMail
 * Author	    : lixu     
 * Description :  发送邮件网络线程返回发送结果后通知ui
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailCallBackUi(comm_boolean result)
{
   mail_SendSignaltoAppTask(MSG_MAIL_SEND_FINISH, (int)result);
}

/*==========================================================
 * Function     : mail_StartViewMail
 * Author	    : sunguochen      
 * Description : 开始解析邮件- 本地管理解析
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartFldrParseMail(comm_uint16* file_name)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_FLDR_PARSE_START, (int)file_name);
}

/*==========================================================
 * Function     : mail_StartViewMail
 * Author	    : sunguochen      
 * Description : 开始解析邮件-准备浏览邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartViewMail(int msgid)
{   
    mail_StartViewParseMail(msgid);                    
}

/*==========================================================
 * Function     : mail_StartViewMail
 * Author	    : sunguochen      
 * Description : 解析邮件完毕-通知UI
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_ViewMailFinished(int message_ptr)
{
    mail_SendSignaltoAppTask(MSG_MAIL_FINISH_READ, (int)message_ptr);
}

//增加公用接口
//用于手机邮箱全局信息
/*==========================================================
 * Function     : mail_IsNetworkValid
 * Author	    : sunguochen      
 * Description : 判断网络是否可用
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_IsNetworkValid(void)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    comm_boolean result = FALSE;

    dual_sys = MMIAPISET_GetActiveSim();
    result = MMIAPIPHONE_IsSimOk(dual_sys);
    if(!result)
    {
        return FALSE;
    }

     result = MMIAPIPHONE_GetIsPsReady(dual_sys);
     return result;
}

/*==========================================================
 * Function     : mail_SetRecvingState
 * Author	    : sunguochen      
 * Description : 设置接收状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetRecvingState(comm_boolean result)
{
    s_mail_app_info.is_recving = result;
}

/*==========================================================
 * Function     : mail_GetRecvingState
 * Author	    : sunguochen      
 * Description : 获取接收状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetRecvingState(void)
{
    return s_mail_app_info.is_recving ;
}

/*==========================================================
 * Function     : mail_SetSendingState
 * Author	    : sunguochen      
 * Description : 设置发送状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSendingState(comm_boolean result)
{
    s_mail_app_info.is_sending = result;
    if(!result)
    {
        mail_SetUserCancelSendingState(FALSE);
    }
    //COMM_TRACE:"Mail: mail_SetSendingState state: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1370_112_2_18_2_47_46_95,(uint8*)"d", result);
}

/*==========================================================
 * Function     : mail_GetSendingState
 * Author	    : sunguochen      
 * Description : 获取发送状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetSendingState(void)
{
    //COMM_TRACE:"mail_GetSendingState: is_sending = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1383_112_2_18_2_47_46_96,(uint8*)"d", s_mail_app_info.is_sending);
    return s_mail_app_info.is_sending;
}

/*==========================================================
 * Function     : mail_SetParsingState
 * Author	    : sunguochen      
 * Description : 设置解析状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetParsingState(comm_boolean result)
{
    s_mail_app_info.is_parsing = result;
}

/*==========================================================
 * Function     : mail_GetParsingState
 * Author	    : sunguochen      
 * Description : 获取解析状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetParsingState(void)
{
    return s_mail_app_info.is_parsing;
}

/*==========================================================
 * Function     : mail_SetNeedRecvingState
 * Author	    : sunguochen      
 * Description : 设置需要接收状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetNeedRecvingState(comm_boolean result)
{
    s_mail_app_info.need_recving = result;
}

/*==========================================================
 * Function     : mail_GetNeedRecvingState
 * Author	    : sunguochen      
 * Description : 获取是否需要接收状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetNeedRecvingState(void)
{
    return s_mail_app_info.need_recving ;
}

/*==========================================================
 * Function     : mail_SetAutoRecvingState
 * Author	    : sunguochen      
 * Description : 设置自动接收状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetAutoRecvingState(comm_boolean result)
{
    s_mail_app_info.is_autorecving = result;
}

/*==========================================================
 * Function     : mail_GetAutoRecvingState
 * Author	    : sunguochen      
 * Description : 获取自动接收状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetAutoRecvingState(void)
{
    return s_mail_app_info.is_autorecving ;
}
/*==========================================================
 * Function     : mail_SetNeedSendingState
 * Author	    : sunguochen      
 * Description : 设置是否需要发送状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetNeedSendingState(comm_boolean result)
{
    s_mail_app_info.need_sending = result;
}

/*==========================================================
 * Function     : mail_GetNeedSendingState
 * Author	    : sunguochen      
 * Description : 获取是否需要发送状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetNeedSendingState(void)
{
    return s_mail_app_info.need_sending;
}

/*==========================================================
 * Function     : mail_SetSockNeedCloseState
 * Author	    : sunguochen      
 * Description : sock session 需要关闭
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSockNeedCloseState(int result)
{
    s_mail_app_info.sock_needclose = result;
}

/*==========================================================
 * Function     : mail_GetSockNeedCloseState
 * Author	    : sunguochen      
 * Description : sock session 需要关闭
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
int mail_GetSockNeedCloseState(void)
{
    return s_mail_app_info.sock_needclose;
}

/*==========================================================
 * Function     : mail_SetSSLPasserState
 * Author	    : sunguochen      
 * Description : 设置是否ssl中
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSSLPasserState(comm_boolean result)
{
    s_mail_app_info.is_sslpasser = result;
}

/*==========================================================
 * Function     : mail_GetSSLPasserState
 * Author	    : sunguochen      
 * Description : 获取是否ssl中
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetSSLPasserState(void)
{
    return s_mail_app_info.is_sslpasser;
}

/*==========================================================
 * Function     : mail_GetTotalBoxMailNum
 * Author	    : sunguochen      
 * Description : 获得信箱全部邮件数量
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
int mail_GetTotalBoxMailNum(void)
{
    int mail_num = 0;

    mail_num = mail_getCurMailTotalNum();

    return mail_num;
}

/*==========================================================
 * Function     : mail_IsMailBoxFull
 * Author	    : sunguochen      
 * Description : 邮件是否已经200封-已满
                      包括:已保存的+ 正在解析的+ 已接收未解析的
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_IsMailBoxFull(void)
{

    int mail_num = 0;

    mail_num = mail_GetTotalBoxMailNum();
    //COMM_TRACE:"Mail num: total mail:%d  un parsed mail: %d parsing mail: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1575_112_2_18_2_47_46_97,(uint8*)"ddd", mail_num, s_mail_app_info.unparsed_mail_num, s_mail_app_info.is_parsing);

    mail_num += s_mail_app_info.unparsed_mail_num;

    if(s_mail_app_info.is_parsing)
    {
        mail_num ++;
    }

    s_mail_app_info.total_message_num = mail_num ;

    if(mail_num >= MAIL_BOX_MAIL_MAX_NUM)
    {
        //COMM_TRACE:"Mail num: mail is full: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1588_112_2_18_2_47_46_98,(uint8*)"d", mail_num);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*==========================================================
 * Function     : mail_IsFreeSpaceOk
 * Author	    : sunguochen      
 * Description : 剩余空间是否可以接收邮件
                      当剩余空间小于1M，不再接收
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_IsFreeSpaceOk(void)
{
    comm_uint32 free_space = 0;
    
    free_space = mail_FileFreeSpace();
    
    //COMM_TRACE:"Mail FreeSpace: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1612_112_2_18_2_47_46_99,(uint8*)"d", free_space);
    s_mail_app_info.free_space_size = free_space;

    if(free_space > 0)//预留 2 M
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*==========================================================
 * Function     : mail_AddUnRecvMailNum
 * Author	    : sunguochen      
 * Description : 新增未接收邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_AddUnRecvMailNum(int num)
{
    s_mail_app_info.unrecv_message_num += num;
}

/*==========================================================
 * Function     : mail_AddUnRecvMailNum
 * Author	    : sunguochen      
 * Description : 未接收邮件数
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
int mail_GetUnRecvMailNum(void)
{
    return s_mail_app_info.unrecv_message_num;

}

/*==========================================================
 * Function     : mail_NewMailToParse
 * Author	    : sunguochen      
 * Description : 接收完邮件处理
                      如果解析中,保存链表
                      返回TRUE-不发事件
                      否则发送解析事件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_NewMailToParse(comm_uint16* file_name, int* accountid)
{
    if(s_mail_app_info.is_parsing)
    {
        mail_unparsed_list_t * mail_name = NULL;
        mail_unparsed_list_t * mail_tmp = NULL;        
        //正在解析中- 保存当前未解析邮件
        s_mail_app_info.unparsed_mail_num ++;

        mail_name = comm_alloc(sizeof(mail_unparsed_list_t));
        
        if(!mail_name)
        {
            return FALSE;
        }

        memset(mail_name, 0, sizeof(mail_unparsed_list_t));
        MMIAPICOM_Wstrcpy(mail_name->file_name, file_name);
        mail_name->accountid = *accountid;

        mail_name->next = NULL;
        mail_tmp = s_mail_app_info.mail_list;

        if(s_mail_app_info.mail_list == NULL)
        {
            //first
            s_mail_app_info.mail_list = mail_name;
        }
        else
        {
            while(mail_tmp->next)
            {
                mail_tmp = mail_tmp->next;
            }

            mail_tmp->next = mail_name;

        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*==========================================================
 * Function     : mail_GetNextMailToParse
 * Author	    : sunguochen      
 * Description : 获得下一个未解析邮件
                      如果返回FALSE-说明解析完毕
                      如果正在解析-则返回FALSE
 * Parameter	: type          parameter       Description
                      comm_uint16* file_name - 用于保存邮件名
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_GetNextMailToParse(comm_uint16* file_name, int *accountid)
{
    mail_unparsed_list_t * mail_name = NULL;

    if(s_mail_app_info.is_parsing || file_name == NULL)
    {
        //COMM_TRACE:"Mail: mail_GetNextMailToParse is parsing! num : %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1726_112_2_18_2_47_47_100,(uint8*)"d", s_mail_app_info.unparsed_mail_num);
        //正在解析中
        return FALSE;
    }

    if(s_mail_app_info.unparsed_mail_num && s_mail_app_info.mail_list)
    {
        //COMM_TRACE:"Mail: parsing! num : %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1733_112_2_18_2_47_47_101,(uint8*)"d", s_mail_app_info.unparsed_mail_num);
        s_mail_app_info.unparsed_mail_num --;

        mail_name = s_mail_app_info.mail_list;

        s_mail_app_info.mail_list = s_mail_app_info.mail_list->next;

        MMIAPICOM_Wstrcpy(file_name, mail_name->file_name);
        *accountid = mail_name->accountid;
        comm_free(mail_name);
        return TRUE;
    }
    else
    {
        //COMM_TRACE:"Mail: unparsed list error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1747_112_2_18_2_47_47_102,(uint8*)"");

        s_mail_app_info.unparsed_mail_num = 0;
        mail_name = s_mail_app_info.mail_list;

        while(mail_name)
        {
            s_mail_app_info.mail_list = s_mail_app_info.mail_list->next;
            comm_free(mail_name);/*lint !e774*/

            mail_name = s_mail_app_info.mail_list;
        }
        return FALSE;
    }
}

/*==========================================================
 * Function     : mail_parseNextMail
 * Author	    : sunguochen      
 * Description :  继续解析邮件
 * Parameter	: type          parameter       Description
                      comm_uint16* file_name - 用于保存邮件名
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_parseNextMail(void)
{
    comm_boolean result;
    wchar *parser_filename_ptr = NULL;
    int accountid = 0;

    MAIL_EVENT_MULTIPARA *parse_papr = NULL;

    //COMM_TRACE:"Mail: mail_parseNextMail\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1780_112_2_18_2_47_47_103,(uint8*)"");
    parser_filename_ptr = comm_alloc((MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1)*2);

    result = mail_GetNextMailToParse((comm_uint16 *)parser_filename_ptr, &accountid);

    if(result)
    {
        parse_papr = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));
        parse_papr->data1 = (int)parser_filename_ptr;
        parse_papr->data2 = (int)accountid;
 
        //COMM_TRACE:"Mail: mail_parseNextMail start parser next msg! \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1791_112_2_18_2_47_47_104,(uint8*)"");
        mail_SendSignaltoMailParseTask(MSG_MAIL_RECV_PARSE_START, (int)parse_papr);
    } 
    else
    {
        //COMM_TRACE:"Mail: mail_parseNextMail parser finish!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1796_112_2_18_2_47_47_105,(uint8*)"");
        comm_free(parser_filename_ptr);
    }
}

/*==========================================================
 * Function     : mail_RetrAttachNotify
 * Author	    : sunguochen      
 * Description : 接收附件结果-通知UI
                      retr_result 需要MSG_MAIL_RETRATTACH_RESULT事件处理处释放
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachNotify(mail_retr_result_t     *retr_result)
{
    //COMM_TRACE:"Mail: receive attachment num: %d err: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1812_112_2_18_2_47_47_106,(uint8*)"dd", retr_result->retr_num, retr_result->retr_result);
    mail_SendSignaltoAppTask(MSG_MAIL_RETRATTACH_RESULT, (int)retr_result);
}

/*==========================================================
 * Function     : mail_RetrAttachStart
 * Author	    : sunguochen      
 * Description : 接收附件
 * Parameter	: type          parameter       Description
                       int accountid,                                     账户id
                       char *uidl,                                         邮件uidl
                       MAIL_MESSAGE_PART_T * attach_list,   附件信息
                       int retr_num                                      接收个数
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachStart(int accountid, char *uidl, MAIL_MESSAGE_PART_T * attach_list, int retr_num)
{
    int i = 0;
    int account = 0;
    int att_allow_size = 0;
    mail_retr_method_t* retr_mode = NULL;
    MAIL_MESSAGE_PART_T * att_ptr = NULL;

    //COMM_TRACE:"Mail:mail_RetrAttachStartaccount:%d uidl:%s retr_num %d,cont:%s, size:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1837_112_2_18_2_47_47_107,(uint8*)"dsdsd",accountid, uidl, retr_num, attach_list->content, attach_list->size);

    if(!accountid)
    {
        account = mail_GetDefaultAccount();
    }
    else
    {
        account = accountid;
    }

    if(mail_IsFreeSpaceOk() == FALSE)
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-no space"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1851_112_2_18_2_47_47_108,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_NOSPACE;
        mail_RetrAttachNotify(retr_result);
        return;

    }

    if(!uidl || !attach_list || (retr_num <= 0))
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-para error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1864_112_2_18_2_47_47_109,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_PARA ;
        mail_RetrAttachNotify(retr_result);
        return;

    }

    att_allow_size = mail_GetAttachDownloadLimit();

    //检查附件列表参数
    for(i = 0; i < retr_num; i++)
    {
        att_ptr = &(attach_list[i]);
/* PC lint*/
#if 0
        if(att_ptr == NULL) /* Impossible */
        {
            mail_retr_result_t     *retr_result = NULL;
            //COMM_TRACE:"Mail:mail_RetrAttachStart-para error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1884_112_2_18_2_47_47_110,(uint8*)"");

            retr_result = comm_alloc(sizeof(mail_retr_result_t));
            retr_result->retr_num = 0;
            retr_result->retr_result = MAIL_RETR_ERR_PARA ;
            mail_RetrAttachNotify(retr_result);
            return;            
        }
        else
        {
            if(att_ptr->size > (uint32)att_allow_size)
            {
                //附件过大
                mail_retr_result_t     *retr_result = NULL;
                //COMM_TRACE:"Mail:mail_RetrAttachStart- att too large!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1898_112_2_18_2_47_47_111,(uint8*)"");

                retr_result = comm_alloc(sizeof(mail_retr_result_t));
                retr_result->retr_num = 0;
                retr_result->retr_result = MAIL_RETR_ERR_TOLARGE;
                mail_RetrAttachNotify(retr_result);
                return;                  
            }
        }
#else
        if(att_ptr->size > (uint32)att_allow_size)
        {
            //附件过大
            mail_retr_result_t     *retr_result = NULL;
            //COMM_TRACE:"Mail:mail_RetrAttachStart- att too large!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1912_112_2_18_2_47_47_112,(uint8*)"");

            retr_result = comm_alloc(sizeof(mail_retr_result_t));
            retr_result->retr_num = 0;
            retr_result->retr_result = MAIL_RETR_ERR_TOLARGE;
            mail_RetrAttachNotify(retr_result);
            return;                  
        }
#endif
    }

    //拷贝附件列表
    att_ptr = comm_alloc(sizeof(MAIL_MESSAGE_PART_T)*((uint32)retr_num));
    if(att_ptr == NULL)
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-para error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1928_112_2_18_2_47_47_113,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_NOSPACE ;
        mail_RetrAttachNotify(retr_result);
        return;
    }

    for(i = 0; i < retr_num; i++)
    {
        
        if(attach_list[i].contentid)
        {
            att_ptr[i].contentid = (uint8 *)mail_strdup((char *)attach_list[i].contentid);
        }
        
        att_ptr[i].size = attach_list[i].size;

        if((attach_list[i].filename.wstr_ptr != NULL) && (attach_list[i].filename.wstr_len != 0))
        {
            att_ptr[i].filename.wstr_len = attach_list[i].filename.wstr_len;            
            att_ptr[i].filename.wstr_ptr = comm_alloc((attach_list[i].filename.wstr_len + 1)*sizeof(wchar));
            memcpy(att_ptr[i].filename.wstr_ptr, attach_list[i].filename.wstr_ptr, (attach_list[i].filename.wstr_len*sizeof(wchar)));
        }        
    }

    //是否在接收    
    if(mail_GetRecvingState())
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-retring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1959_112_2_18_2_47_47_114,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_RETRING ;
        mail_RetrAttachNotify(retr_result);
        return;
        
    }

    //是否在发送
    if(mail_GetSendingState())
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-retring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1973_112_2_18_2_47_47_115,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_SENDING ;
        mail_RetrAttachNotify(retr_result);
        return;
    }

    retr_mode = comm_alloc(sizeof(mail_retr_method_t));

    if (PNULL == retr_mode)
    {
        //SCI_TRACE_LOW:"MMIMail mail_RetrAttachStart PNULL == retr_mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_1986_112_2_18_2_47_47_116,(uint8*)"");
        return ;
    }

    retr_mode->retr_mode = RETRIEVE_MODE_ATTACH;
    retr_mode->accountid = account;
    retr_mode->uidl         = mail_strdup(uidl);
    retr_mode->attachid   = (void *)att_ptr;
    retr_mode->attach_num = retr_num;

    //发送接收附件请求
    mail_SendRetrAttachReq(account, retr_mode);
}

/*==========================================================
 * Function     : mail_CheckPasswordResult
 * Author	    : sunguochen      
 * Description : 通知UI  验证密码结果
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_CheckPasswordResult(comm_boolean result)
{
    //COMM_TRACE:"Mail:mail_CheckPasswordResult result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2010_112_2_18_2_47_47_117,(uint8*)"d", result);
    mail_SendSignaltoAppTask(MSG_MAIL_CHECK_PASSWD_RESULT, result);
}

/*==========================================================
 * Function     : mail_CheckPassWord
 * Author	    : sunguochen      
 * Description : 验证密码请求
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_CheckPassWord(int accountid, wchar *passwd, int len)
{
    int account = 0;
    mail_retr_method_t *retr_mode = NULL;
    char *pass_word = NULL;

    //COMM_TRACE:"Mail:mail_CheckPassWord:%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2028_112_2_18_2_47_47_118,(uint8*)"d", accountid);

    if(accountid == 0)
    {
        account = mail_GetDefaultAccount();
    }
    else
    {
        account = accountid;
    }

    if(passwd == NULL || *passwd == 0)
    {
        //COMM_TRACE:"Mail:mail_CheckPassWord-password error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2041_112_2_18_2_47_47_119,(uint8*)"");
        mail_CheckPasswordResult(FALSE);
        return;
    }

    if(mail_GetRecvingState())
    {
        //COMM_TRACE:"Mail:mail_CheckPassWord-retring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2048_112_2_18_2_47_47_120,(uint8*)"");
        mail_CheckPasswordResult(FALSE); 
        return;
    }

    if(mail_GetSendingState())
    {
        //COMM_TRACE:"Mail:mail_CheckPassWord-retring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2055_112_2_18_2_47_47_121,(uint8*)"");
        mail_CheckPasswordResult(FALSE);
        return;
    }

    retr_mode = comm_alloc(sizeof(mail_retr_method_t));

    if (PNULL == retr_mode)
    {
        //SCI_TRACE_LOW:"MMIMail mail_CheckPassWord PNULL == retr_mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2064_112_2_18_2_47_47_122,(uint8*)"");
        return ;
    }

    pass_word = comm_alloc(len + 1);
    if (PNULL == pass_word)
    {
        //SCI_TRACE_LOW:"MMIMail mail_CheckPassWord PNULL == pass_word"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2071_112_2_18_2_47_47_123,(uint8*)"");
        return ;
    }

    MMIAPICOM_WstrToStr(passwd,  (uint8 *)pass_word);
    retr_mode->retr_mode = RETRIEVE_MODE_PASSWD;
    retr_mode->accountid = account;
    retr_mode->uidl = pass_word;
    //发送验证密码请求
    mail_SendRetrAttachReq(account, retr_mode);
}

/*==========================================================
 * Function     : mail_CancelRetringManual
 * Author	    : sunguochen      
 * Description :  停止手动接收
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/27/2010 
 ==========================================================*/
void mail_CancelRetringManual(void)
{

    if(!mail_GetRecvingState())
    {
        //COMM_TRACE:"MAIL:mail_CancelRetringManual not retring \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2096_112_2_18_2_47_47_124,(uint8*)"");
        return;
    }
    else
    {
        mail_StopNetAction(MAIL_SESSION_STOP_RECVING);
    }    

}

/*==========================================================
 * Function     : mail_CancelSendingManual
 * Author	    : sunguochen      
 * Description :  停止手动发送
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/27/2010 
 ==========================================================*/
void mail_CancelSendingManual(void)
{
    //COMM_TRACE:"Mail: mail_CancelSendingManual \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2116_112_2_18_2_47_47_125,(uint8*)"");

    if(mail_GetSendingState())
    {
        mail_SetUserCancelSendingState(FALSE);        
        mail_StopNetAction(MAIL_SESSION_STOP_SENDING);
    }
    else
    {
        mail_SetUserCancelSendingState(TRUE);
        //COMM_TRACE:"Mail: mail_CancelSendingManual  not sending\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2126_112_2_18_2_47_47_126,(uint8*)"");
    }
}



/*==========================================================
 * Function     : mail_SendStart
 * Author	    : sunguochen      
 * Description  : 开始发送邮件
 * Parameter	: type          parameter       Description
                  int accountid            账户id
                  MAIL_MESSAGE_T *msg      邮件信息 
                  char *file_name          邮件名 
                  char* fwd_uidl           是否服务器转发
 * Return	    : Void
 * Effect	    : NO
 * History	    : created  5/31/2010 
 ==========================================================*/
void mail_SendStart(int accountid, MAIL_MESSAGE_T *msg, char *file_name, char* fwd_uidl)
{
    MAIL_EVENT_MULTIPARA *send_papr = NULL;

    send_papr = comm_alloc(sizeof(MAIL_EVENT_MULTIPARA));
    send_papr->data1 = (int)accountid;
    send_papr->data2 = (int)msg;
    send_papr->data3 = (int)file_name;
    send_papr->data4 = (int)fwd_uidl;

    //start send mail set sending indicator    
    mail_SetSendingState( TRUE );

    //发送事件给接收线程
    mail_SendSignaltoMailTask(MSG_MAIL_SEND_START, (int)send_papr);
}

/*==========================================================
 * Function     : mail_SendNext
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/31/2010 
 ==========================================================*/
void mail_SendNext(mail_retr_result send_result)
{
    mail_SendSignaltoMailParseTask(MSG_MAIL_SEND_NEXT, (int)send_result);
}

/*==========================================================
 * Function     : mail_CanSendingMail
 * Author	    : sunguochen      
 * Description  : 是否可以发送邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	    : created  5/31/2010 
 ==========================================================*/
comm_boolean mail_CanSendingMail(void)
{
    if(mail_GetRecvingState() || mail_GetSendingState())
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*==========================================================
 * Function     : mail_AppHandler
 * Author	    : sunguochen      
 * Description : 获得mail的应用模块
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/2/2010 
 ==========================================================*/
uint32 mail_AppHandler(void)
{
    return MMI_MODULE_PUSHMAIL;
}

/*==========================================================
 * Function     : mail_RetrMailNotify
 * Author	    : sunguochen      
 * Description : 通知用户接收邮件个数
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/9/2010 
 ==========================================================*/
void mail_RetrMailNotify(mail_retr_result_t     *retr_result)
{
    //COMM_TRACE:"Mail: mail_AutoRetrNotify retr num: %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2217_112_2_18_2_47_48_127,(uint8*)"d", retr_result);
    mail_SendSignaltoAppTask(MSG_MAIL_RETRMAIL_RESULT, (int)retr_result);
}

/*==========================================================
 * Function     : mail_ResetFactorySetting
 * Author	    : sunguochen      
 * Description : 手机邮箱恢复出厂设置
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/30/2010 
 ==========================================================*/
void mail_ResetFactorySetting(void)
{
    mail_settingFinal();
    mail_BackToFactory();
}

/*==========================================================
 * Function     : mail_progressbar
 * Author	    : sunguochen      
 * Description : 显示接收进度
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_progressbar(mail_ProgressInfo* prog_info, void* userData)
{
    //显示接收/发送进度
    //COMM_TRACE:"Mail:mail_progressbar %d/%d -%d/%d  - %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2247_112_2_18_2_47_48_128,(uint8*)"ddddd",prog_info->msgIndex,prog_info->totNumOfMsg,prog_info->processedBytes, prog_info->totalByteSize,prog_info->bytePercent);

    //自动接收不显示进度
    if(mail_GetAutoRecvingState() == FALSE)
    {
        mail_SendSignaltoAppTask(MSG_MAIL_RECV_MAIL_PROGRESS, (int)prog_info);
    }
}

/*==========================================================
 * Function     : mail_FilesysIsInvalid
 * Author	    : sunguochen      
 * Description : 文件系统是否可用
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  9/25/2010 
 ==========================================================*/
BOOLEAN mail_FilesysIsInvalid(void)
{    
    return MMIAPIUDISK_UdiskIsRun();
}

/*==========================================================
 * Function     : mail_SetUserCancelSendingState
 * Author	    : sunguochen      
 * Description : 设置取消发送状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetUserCancelSendingState(comm_boolean result)
{

    s_mail_app_info.user_cancel_sending = result;
    //COMM_TRACE:"Mail: mail_SetUserCancelSendingState state: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_MAIN_2281_112_2_18_2_47_48_129,(uint8*)"d", result);
}

/*==========================================================
 * Function     : mail_GetUserCancelSendingState
 * Author	    : sunguochen      
 * Description : 获取取消发送状态
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetUserCancelSendingState(void)
{
    return s_mail_app_info.user_cancel_sending;
}



