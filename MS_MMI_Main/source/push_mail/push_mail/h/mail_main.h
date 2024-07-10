/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_main.h
 *	Author		: sunguochen    
 *	Description	: 手机邮箱主函数
 *	Vesion		: 1.0        
 *	Date			: 2010-4-13  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-13      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MAIL_MAIN_H_
#define _MAIL_MAIN_H_


#include "comm_platform.h"
#include "mail_setting.h"
#include "mail_network.h"

#include "mail_message.h"


#define MAIL_BOX_MAIL_MAX_NUM        200             //200 mail
#define MAIL_REMAIN_SPACE_SIZE        1*1024*1024//预留空间


//版本号
#define MAIL_HS_VERSION "H-MAIL-V1.1.0.45"
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
#define MAIL_RECODE_INDEX 0xFF000000
#endif
//mail event list
typedef enum
{
    MSG_MAIL_EVENT_NULL = 0x400,
    MSG_MAIL_NET_NOTIFY,
    MSG_MAIL_NET_ERROR,
    MSG_MAIL_NET_RESEND,
    MSG_MAIL_NET_RERETR,    
    MSG_MAIL_NET_QUERY,    
    MSG_MAIL_NET_EXIT,
    MSG_MAIL_NET_SSL_OK,
    MSG_MAIL_NET_SSL_EVENT,     
    MSG_MAIL_ACTION_EVENT,    
    MSG_MAIL_STOP_EVENT,
    MSG_MAIL_TIME_OUT,
    MSG_MAIL_HEADER_START,
    MSG_MAIL_HEADER_FINISH,  
    MSG_MAIL_NET_CLOSE_DELAY,
    MSG_MAIL_NET_PDP_DEACTIVE,     

    MSG_MAIL_RETR_START,      //开始接收
    MSG_MAIL_SEND_START,      //开始发送
    MSG_MAIL_DATA_ARRIVAL,    //数据到达
    MSG_MAIL_DATA_SEND,       //数据发送
    MSG_MAIL_SEND_NEXT,       //发送下一个
    MSG_MAIL_RECV_PARSE_START,//接收解析
    MSG_MAIL_VIEW_PARSE_START,//浏览解析--读文件
    MSG_MAIL_VIEW_PARSE_BEGIN,//浏览解析--开始    
    MSG_MAIL_FLDR_PARSE_START,//本地管理解析   
    MSG_MAIL_RECV_PARSE_FINISH,//接收解析 结束   
    MSG_MAIL_VIEW_PARSE_FINISH,//浏览解析 结束   
    MSG_MAIL_FLDR_PARSE_FINISH,//本地管理解析 结束    
    MSG_MAIL_MAILVIEW_START,     

   MSG_MAIL_OPERATE_DEL_START,//开始删除邮件
   MSG_MAIL_OPERATE_DEL_CANCLE,//删除被取消

   MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX, //发送邮件时保存邮件到草稿箱
   MSG_MAIL_OPERATE_SEND_MSG,//邮件未编辑发送
   MSG_MAIL_OPERATE_READ_SORT,//阅读邮件后排序收件箱
   MSG_MAIL_OPERATE_SAVE_OUTBOX,//仅保存邮件到草稿箱
   MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG,//删除指定帐户邮件
   MSG_MAIL_OPERATE_BACK_FACTORY,//恢复出厂设置
   MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG,//删除所有帐户邮件

    MSG_MAIL_EVENT_MAX

}MSG_MAIL_EVENT_TYPE;



//获取方式
typedef enum 
{
    RETRIEVE_MODE_NORMAL,//接收邮件
    RETRIEVE_MODE_ENABLE, //设置账户可用
    RETRIEVE_MODE_DISABLE,//设置账户不可用
    RETRIEVE_MODE_ATTACH,//接收附件
    RETRIEVE_MODE_PASSWD,// 验证密码
    RETRIEVE_MODE_MAX
   
  } RETRIEVE_MODE_E;


//用于发送多个参数
typedef struct _MAIL_EVENT_MULTIPARA_
{
    int data1;
    int data2;
    int data3;
    int data4;

}MAIL_EVENT_MULTIPARA;


//下载邮件结构体
typedef struct
{
    
    RETRIEVE_MODE_E     retr_mode;
    int                            accountid;
    char*                        uidl;
    int                            attach_num;
    void*                        attachid;

}mail_retr_method_t;



//接收/发送结果 错误值
typedef enum
{
    MAIL_RETR_SUCCESS,
    /*通常错误*/        
    MAIL_RETR_ERR_PARA,                   //参数错误
    MAIL_RETR_ERR_USERCANCEL,       //失败，用户取消   
    MAIL_RETR_ERR_TOLARGE,            //失败，附件过大  
    MAIL_RETR_ERR_SENDING,            //失败，正在发送   
    MAIL_RETR_ERR_RETRING,           //失败，正在接收    
    MAIL_RETR_ERR_NOSPACE,           //空间不足
    MAIL_RETR_ERR_BOXFULL,           //邮件已满--200
    /*网络错误*/    
    MAIL_RETR_ERR_PDPFAIL,            //pdp 激活失败
    MAIL_RETR_ERR_SOCKFAIL,          //创建socket失败
    MAIL_RETR_ERR_PROXYFAIL,        //代理服务器失败10
    MAIL_RETR_ERR_CONNECTFAIL,    //连接失败
    MAIL_RETR_ERR_SERVERFAIL,      //邮件服务器出错
    MAIL_RETR_ERR_SENDFAIL,          //发送失败
    MAIL_RETR_ERR_RETRFAIL,          //接收失败
    /*超时错误*/    
    MAIL_RETR_ERR_ESTABTIMEOUT,  //连接服务器超时
    MAIL_RETR_ERR_RETRTIMEOUT,   //接收邮件超时    
    MAIL_RETR_ERR_DELETIMEOUT,    //删除/标记邮件超时    
    MAIL_RETR_ERR_QUITTIMEOUT,    //退出超时       
    MAIL_RETR_ERR_SEND_FAIL,        //发送失败
    
    MAIL_RETR_ERR_COMMON,            //未知原因失败
    
    MAIL_RETR_MAX
    
}mail_retr_result;



//接收/发送结果
typedef struct
{
    SIGNAL_VARS
    int                       retr_num;
    mail_retr_result     retr_result;
    int                       accountid;
    
}mail_retr_result_t;




/*==========================================================
 * Function 	: mail_mainInit
 * Author		: sunguochen      
 * Description : mail main init
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_mainInit(void);



/*==========================================================
 * Function 	: mail_SendSignaltoMailTask
 * Author		: sunguochen      
 * Description : 发送事件给mail线程
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_SendSignaltoMailTask(
                             MSG_MAIL_EVENT_TYPE event, 
                             int data
                             );



/*==========================================================
 * Function 	: mail_SendSignaltoMailTask
 * Author		: sunguochen      
 * Description : 发送事件给aPP线程
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_SendSignaltoAppTask(
                             int event, 
                             int data
                             );



/*==========================================================
 * Function 	: mail_SendSignaltoMailParseTask
 * Author		: sunguochen      
 * Description : 发送事件给网络线程
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_SendSignaltoMailParseTask(
                             MSG_MAIL_EVENT_TYPE event, 
                             int data
                             );



/*==========================================================
 * Function 	: mail_GetThreadId
 * Author		: sunguochen      
 * Description : 获取邮箱线程ID
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
comm_task_id mail_GetThreadId(void);



/*==========================================================
 * Function 	: mail_CreateThread
 * Author		: sunguochen      
 * Description : 创建网络交互线程
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/7/2010 
 ==========================================================*/
comm_boolean mail_CreateThread(void);



/*==========================================================
 * Function 	: mail_GetlHeadInfoStart
 * Author		: sunguochen      
 * Description : 开始解析邮件列表信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_GetlHeadInfoStart(int accountid, void * header_info);



/*==========================================================
 * Function 	: mail_SendRetrMsg
 * Author		: sunguochen      
 * Description : 发送接收请求
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrMsgReq(int accountid);



/*==========================================================
 * Function 	: mail_SendRetrAttachReq
 * Author		: sunguochen      
 * Description : 发送接收请求-附件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrAttachReq(int accountid, mail_retr_method_t* retr_mode);



/*==========================================================
 * Function 	: mail_SendRetrEnableReq
 * Author		: sunguochen      
 * Description : 暂停/恢复邮件接收
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrEnableReq(int accountid, comm_boolean is_enable);



/*==========================================================
 * Function 	: mail_StartRetr
 * Author		: sunguochen      
 * Description : 开始接收邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartRetrMsgNormal(mail_retr_method_t * rete_method);



/*==========================================================
 * Function 	: mail_SendRetrMsg
 * Author		: sunguochen      
 * Description : 发送请求
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendSendMsgReq(int accountid);



/*==========================================================
 * Function 	: mail_StartRetr
 * Author		: sunguochen      
 * Description : 开始发送邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartSendMsgNormal(int accountid);



/*==========================================================
 * Function 	: mail_StartParseReq
 * Author		: sunguochen      
 * Description : 开始解析
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartParseReq(comm_uint16* file_name);



/*==========================================================
 * Function 	: mail_StartViewMail
 * Author		: sunguochen      
 * Description : 开始解析邮件-接收解析邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartRecvParseMail(comm_uint16* file_name, int accountid);



/*==========================================================
 * Function 	: mail_StartViewMail
 * Author		: sunguochen      
 * Description : 开始解析邮件-准备浏览邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartViewMail(int msgid);



/*==========================================================
 * Function 	: mail_StartViewMail
 * Author		: sunguochen      
 * Description : 解析邮件完毕-通知UI
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_ViewMailFinished(int message_ptr);



//增加公用接口
//用于手机邮箱全局信息
/*==========================================================
 * Function 	: mail_IsNetworkValid
 * Author		: sunguochen      
 * Description : 判断网络是否可用
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_IsNetworkValid(void);



/*==========================================================
 * Function 	: mail_SetRecvingState
 * Author		: sunguochen      
 * Description : 设置接收状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetRecvingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetRecvingState
 * Author		: sunguochen      
 * Description : 获取接收状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetRecvingState(void);



/*==========================================================
 * Function 	: mail_SetSendingState
 * Author		: sunguochen      
 * Description : 设置发送状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSendingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetSendingState
 * Author		: sunguochen      
 * Description : 获取发送状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetSendingState(void);



/*==========================================================
 * Function 	: mail_SetParsingState
 * Author		: sunguochen      
 * Description : 设置解析状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetParsingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetParsingState
 * Author		: sunguochen      
 * Description : 获取解析状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetParsingState(void);



/*==========================================================
 * Function 	: mail_SetNeedRecvingState
 * Author		: sunguochen      
 * Description : 设置需要接收状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetNeedRecvingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetRecvingState
 * Author		: sunguochen      
 * Description : 获取是否需要接收状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetNeedRecvingState(void);



/*==========================================================
 * Function 	: mail_SetAutoRecvingState
 * Author		: sunguochen      
 * Description : 设置自动接收状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetAutoRecvingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetutoRecvingState
 * Author		: sunguochen      
 * Description : 获取自动接收状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetAutoRecvingState(void);



/*==========================================================
 * Function 	: mail_SetNeedSendingState
 * Author		: sunguochen      
 * Description : 设置是否需要发送状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetNeedSendingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetNeedSendingState
 * Author		: sunguochen      
 * Description : 获取是否需要发送状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetNeedSendingState(void);



/*==========================================================
 * Function 	: mail_SetSockNeedCloseState
 * Author		: sunguochen      
 * Description : sock session 需要关闭
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSockNeedCloseState(int result);



/*==========================================================
 * Function 	: mail_GetSockNeedCloseState
 * Author		: sunguochen      
 * Description : sock session 需要关闭
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
int mail_GetSockNeedCloseState(void);



/*==========================================================
 * Function 	: mail_SetSSLPasserState
 * Author		: sunguochen      
 * Description : 设置是否ssl中
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSSLPasserState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetSSLPasserState
 * Author		: sunguochen      
 * Description : 获取是否ssl中
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetSSLPasserState(void);



/*==========================================================
 * Function 	: mail_GetTotalBoxMailNum
 * Author		: sunguochen      
 * Description : 获得信箱全部邮件数量
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
int mail_GetTotalBoxMailNum(void);



/*==========================================================
 * Function 	: mail_IsMailBoxFull
 * Author		: sunguochen      
 * Description : 邮件是否已经200封-已满
                      包括:已保存的+ 正在解析的+ 已接收未解析的
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_IsMailBoxFull(void);



/*==========================================================
 * Function 	: mail_IsFreeSpaceOk
 * Author		: sunguochen      
 * Description : 剩余空间是否可以接收邮件
                      当剩余空间小于1M，不再接收
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_IsFreeSpaceOk(void);



/*==========================================================
 * Function 	: mail_AddUnRecvMailNum
 * Author		: sunguochen      
 * Description : 新增未接收邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_AddUnRecvMailNum(int num);



/*==========================================================
 * Function 	: mail_AddUnRecvMailNum
 * Author		: sunguochen      
 * Description : 未接收邮件数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
int mail_GetUnRecvMailNum(void);



/*==========================================================
 * Function 	: mail_NewMailToParse
 * Author		: sunguochen      
 * Description : 接收完邮件处理
                      如果解析中,保存链表
                      返回TRUE-不发事件
                      否则发送解析事件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_NewMailToParse(comm_uint16* file_name, int *accountid);



/*==========================================================
 * Function 	: mail_GetNextMailToParse
 * Author		: sunguochen      
 * Description : 获得下一个未解析邮件
                      如果返回FALSE-说明解析完毕
                      如果正在解析-则返回FALSE
 * Parameter	: type          parameter       Description
                      comm_uint16* file_name - 用于保存邮件名
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_GetNextMailToParse(comm_uint16* file_name, int *accountid);



/*==========================================================
 * Function 	: mail_parseNextMail
 * Author		: sunguochen      
 * Description :  继续解析邮件
 * Parameter	: type          parameter       Description
                      comm_uint16* file_name - 用于保存邮件名
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_parseNextMail(void);



/*==========================================================
 * Function 	: mail_RetrAttachNotify
 * Author		: sunguochen      
 * Description : 接收附件结果-通知UI
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachNotify(mail_retr_result_t *retr_result);



/*==========================================================
 * Function 	: mail_RetrAttachStart
 * Author		: sunguochen      
 * Description : 接收附件
 * Parameter	: type          parameter       Description
                       int accountid,                                     账户id
                       char *uidl,                                         邮件uidl
                       MAIL_MESSAGE_PART_T * attach_list,   附件信息
                       int retr_num                                      接收个数
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachStart(int accountid, char *uidl, MAIL_MESSAGE_PART_T * attach_list, int retr_num);



/*==========================================================
 * Function 	: mail_CheckPasswordResult
 * Author		: sunguochen      
 * Description : 通知UI  验证密码结果
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_CheckPasswordResult(comm_boolean result);



/*==========================================================
 * Function 	: mail_CheckPassWord
 * Author		: sunguochen      
 * Description : 验证密码请求
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_CheckPassWord(int accountid, wchar *passwd, int len);

/*==========================================================
 * Function 	: mail_GetlHeadInfoFinish
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_GetlHeadInfoFinish(comm_boolean result);

/*==========================================================
 * Function 	: mail_RecevMailFinish
 * Author		: lixu  
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_RecevMailFinish(void);

/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description : UI调用，通知parser task开始删除邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartDelMail(void);


/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description : UI调用，通知parser task停止删除邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_CancleDelMail(void);


/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description : 删除结束后通知UI，结束等待
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_DelMailFinish(comm_boolean result);

/*==========================================================
 * Function 	: mail_sendMailStartSave
 * Author		: lixu     
 * Description :  UI调用，发送邮件时通知parser task编码同时保存邮件到outbox
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailStartSave(comm_int32 account_id,MAIL_MESSAGE_T *message_ptr,
                                  char* fwd_uidl);

/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description :  发送邮件本地保存结束通知UI
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailSaveOk(comm_boolean result);


/*==========================================================
 * Function 	: mail_sortInboxMailList
 * Author		: lixu     
 * Description :  已读邮件阅读后刷新收件箱邮件列表
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sortInboxMailList(void);

/*==========================================================
 * Function 	: mail_saveMailToDraft
 * Author		: lixu     
 * Description :  仅保存邮件到草稿箱
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_saveMailToDrafts(comm_int32 account_id,MAIL_MESSAGE_T* message_ptr);


/*==========================================================
 * Function 	: mail_delAccountAllMail
 * Author		: lixu     
 * Description :  删除指定帐户邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_delAccountAllMail(comm_int32 account_id);

/*==========================================================
 * Function 	: mail_delAccountAllMailFinish
 * Author		: lixu     
 * Description :  删除指定帐户邮件完成通知
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_delAccountAllMailFinish(void);

/*==========================================================
 * Function 	: mail_resetFactory
 * Author		: lixu     
 * Description :  恢复出厂设置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_resetFactory(void);

/*==========================================================
 * Function 	: mail_backToFactoryFinish
 * Author		: lixu     
 * Description :  恢复出厂设置完成
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_backToFactoryFinish(void);

/*==========================================================
 * Function 	: mail_sendMailSendByMsgId
 * Author		: lixu     
 * Description :  UI调用，通过msgId发送邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailSendByMsgId(comm_int32 accountId,comm_uint32 msgId);

/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description :  发送邮件网络线程返回发送结果后通知ui
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailCallBackUi(comm_boolean result);

/*==========================================================
 * Function 	: mail_CancelRetringManual
 * Author		: sunguochen      
 * Description :  停止手动接收
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/27/2010 
 ==========================================================*/
void mail_CancelRetringManual(void);



/*==========================================================
 * Function 	: mail_CancelSendingManual
 * Author		: sunguochen      
 * Description :  停止手动发送
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/27/2010 
 ==========================================================*/
void mail_CancelSendingManual(void);




/*==========================================================
 * Function 	: mail_SendStart
 * Author		: sunguochen      
 * Description  : 开始发送邮件
 * Parameter	: type          parameter       Description
                  int accountid            账户id
                  MAIL_MESSAGE_T *msg      邮件信息 
                  char *file_name          邮件名 
                  char* fwd_uidl           是否服务器转发
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/31/2010 
 ==========================================================*/
void mail_SendStart(int accountid, MAIL_MESSAGE_T *msg, char *file_name, char* fwd_uidl);



/*==========================================================
 * Function 	: mail_SendNext
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/31/2010 
 ==========================================================*/
void mail_SendNext(mail_retr_result send_result);



/*==========================================================
 * Function 	: mail_CanSendingMail
 * Author		: sunguochen      
 * Description  : 是否可以发送邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/31/2010 
 ==========================================================*/
comm_boolean mail_CanSendingMail(void);



/*==========================================================
 * Function 	: mail_AppHandler
 * Author		: sunguochen      
 * Description : 获得mail的应用模块
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
uint32 mail_AppHandler(void);



/*==========================================================
 * Function 	: mail_RetrMailNotify
 * Author		: sunguochen      
 * Description : 通知用户接收邮件个数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/9/2010 
 ==========================================================*/
void mail_RetrMailNotify(mail_retr_result_t     *retr_result);

/*==========================================================
 * Function 	: mail_delAllAccountAllMail
 * Author		: lixu     
 * Description :  删除所有帐户邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_delAllAccountAllMail(void);

/*==========================================================
 * Function 	: mail_delAllAccountAllMailFinish
 * Author		: lixu     
 * Description :  删除所有帐户邮件完成通知
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_delAllAccountAllMailFinish(void);



/*==========================================================
 * Function 	: mail_ResetFactorySetting
 * Author		: sunguochen      
 * Description : 手机邮箱恢复出厂设置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/30/2010 
 ==========================================================*/
void mail_ResetFactorySetting(void);




/*==========================================================
 * Function 	: mail_progressbar
 * Author		: sunguochen      
 * Description : 显示接收进度
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_progressbar(mail_ProgressInfo* prog_info, void* userData);


/*==========================================================
 * Function 	: mail_FilesysIsInvalid
 * Author		: sunguochen      
 * Description : 文件系统是否可用
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  9/25/2010 
 ==========================================================*/
BOOLEAN mail_FilesysIsInvalid(void);



/*==========================================================
 * Function 	: mail_SetUserCancelSendingState
 * Author		: sunguochen      
 * Description : 设置取消发送状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetUserCancelSendingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetUserCancelSendingState
 * Author		: sunguochen      
 * Description : 获取取消发送状态
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetUserCancelSendingState(void);
#endif
