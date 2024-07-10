/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_main.h
 *	Author		: sunguochen    
 *	Description	: �ֻ�����������
 *	Vesion		: 1.0        
 *	Date			: 2010-4-13  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-13      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MAIL_MAIN_H_
#define _MAIL_MAIN_H_


#include "comm_platform.h"
#include "mail_setting.h"
#include "mail_network.h"

#include "mail_message.h"


#define MAIL_BOX_MAIL_MAX_NUM        200             //200 mail
#define MAIL_REMAIN_SPACE_SIZE        1*1024*1024//Ԥ���ռ�


//�汾��
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

    MSG_MAIL_RETR_START,      //��ʼ����
    MSG_MAIL_SEND_START,      //��ʼ����
    MSG_MAIL_DATA_ARRIVAL,    //���ݵ���
    MSG_MAIL_DATA_SEND,       //���ݷ���
    MSG_MAIL_SEND_NEXT,       //������һ��
    MSG_MAIL_RECV_PARSE_START,//���ս���
    MSG_MAIL_VIEW_PARSE_START,//�������--���ļ�
    MSG_MAIL_VIEW_PARSE_BEGIN,//�������--��ʼ    
    MSG_MAIL_FLDR_PARSE_START,//���ع������   
    MSG_MAIL_RECV_PARSE_FINISH,//���ս��� ����   
    MSG_MAIL_VIEW_PARSE_FINISH,//������� ����   
    MSG_MAIL_FLDR_PARSE_FINISH,//���ع������ ����    
    MSG_MAIL_MAILVIEW_START,     

   MSG_MAIL_OPERATE_DEL_START,//��ʼɾ���ʼ�
   MSG_MAIL_OPERATE_DEL_CANCLE,//ɾ����ȡ��

   MSG_MAIL_OPERATE_SEND_SAVE_OUTBOX, //�����ʼ�ʱ�����ʼ����ݸ���
   MSG_MAIL_OPERATE_SEND_MSG,//�ʼ�δ�༭����
   MSG_MAIL_OPERATE_READ_SORT,//�Ķ��ʼ��������ռ���
   MSG_MAIL_OPERATE_SAVE_OUTBOX,//�������ʼ����ݸ���
   MSG_MAIL_OPERATE_DEL_ACCOUNT_MSG,//ɾ��ָ���ʻ��ʼ�
   MSG_MAIL_OPERATE_BACK_FACTORY,//�ָ���������
   MSG_MAIL_OPERATE_DEL_ALL_ACCOUNT_MSG,//ɾ�������ʻ��ʼ�

    MSG_MAIL_EVENT_MAX

}MSG_MAIL_EVENT_TYPE;



//��ȡ��ʽ
typedef enum 
{
    RETRIEVE_MODE_NORMAL,//�����ʼ�
    RETRIEVE_MODE_ENABLE, //�����˻�����
    RETRIEVE_MODE_DISABLE,//�����˻�������
    RETRIEVE_MODE_ATTACH,//���ո���
    RETRIEVE_MODE_PASSWD,// ��֤����
    RETRIEVE_MODE_MAX
   
  } RETRIEVE_MODE_E;


//���ڷ��Ͷ������
typedef struct _MAIL_EVENT_MULTIPARA_
{
    int data1;
    int data2;
    int data3;
    int data4;

}MAIL_EVENT_MULTIPARA;


//�����ʼ��ṹ��
typedef struct
{
    
    RETRIEVE_MODE_E     retr_mode;
    int                            accountid;
    char*                        uidl;
    int                            attach_num;
    void*                        attachid;

}mail_retr_method_t;



//����/���ͽ�� ����ֵ
typedef enum
{
    MAIL_RETR_SUCCESS,
    /*ͨ������*/        
    MAIL_RETR_ERR_PARA,                   //��������
    MAIL_RETR_ERR_USERCANCEL,       //ʧ�ܣ��û�ȡ��   
    MAIL_RETR_ERR_TOLARGE,            //ʧ�ܣ���������  
    MAIL_RETR_ERR_SENDING,            //ʧ�ܣ����ڷ���   
    MAIL_RETR_ERR_RETRING,           //ʧ�ܣ����ڽ���    
    MAIL_RETR_ERR_NOSPACE,           //�ռ䲻��
    MAIL_RETR_ERR_BOXFULL,           //�ʼ�����--200
    /*�������*/    
    MAIL_RETR_ERR_PDPFAIL,            //pdp ����ʧ��
    MAIL_RETR_ERR_SOCKFAIL,          //����socketʧ��
    MAIL_RETR_ERR_PROXYFAIL,        //���������ʧ��10
    MAIL_RETR_ERR_CONNECTFAIL,    //����ʧ��
    MAIL_RETR_ERR_SERVERFAIL,      //�ʼ�����������
    MAIL_RETR_ERR_SENDFAIL,          //����ʧ��
    MAIL_RETR_ERR_RETRFAIL,          //����ʧ��
    /*��ʱ����*/    
    MAIL_RETR_ERR_ESTABTIMEOUT,  //���ӷ�������ʱ
    MAIL_RETR_ERR_RETRTIMEOUT,   //�����ʼ���ʱ    
    MAIL_RETR_ERR_DELETIMEOUT,    //ɾ��/����ʼ���ʱ    
    MAIL_RETR_ERR_QUITTIMEOUT,    //�˳���ʱ       
    MAIL_RETR_ERR_SEND_FAIL,        //����ʧ��
    
    MAIL_RETR_ERR_COMMON,            //δ֪ԭ��ʧ��
    
    MAIL_RETR_MAX
    
}mail_retr_result;



//����/���ͽ��
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
 * Description : �����¼���mail�߳�
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
 * Description : �����¼���aPP�߳�
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
 * Description : �����¼��������߳�
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
 * Description : ��ȡ�����߳�ID
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
comm_task_id mail_GetThreadId(void);



/*==========================================================
 * Function 	: mail_CreateThread
 * Author		: sunguochen      
 * Description : �������罻���߳�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/7/2010 
 ==========================================================*/
comm_boolean mail_CreateThread(void);



/*==========================================================
 * Function 	: mail_GetlHeadInfoStart
 * Author		: sunguochen      
 * Description : ��ʼ�����ʼ��б���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_GetlHeadInfoStart(int accountid, void * header_info);



/*==========================================================
 * Function 	: mail_SendRetrMsg
 * Author		: sunguochen      
 * Description : ���ͽ�������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrMsgReq(int accountid);



/*==========================================================
 * Function 	: mail_SendRetrAttachReq
 * Author		: sunguochen      
 * Description : ���ͽ�������-����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrAttachReq(int accountid, mail_retr_method_t* retr_mode);



/*==========================================================
 * Function 	: mail_SendRetrEnableReq
 * Author		: sunguochen      
 * Description : ��ͣ/�ָ��ʼ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendRetrEnableReq(int accountid, comm_boolean is_enable);



/*==========================================================
 * Function 	: mail_StartRetr
 * Author		: sunguochen      
 * Description : ��ʼ�����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartRetrMsgNormal(mail_retr_method_t * rete_method);



/*==========================================================
 * Function 	: mail_SendRetrMsg
 * Author		: sunguochen      
 * Description : ��������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_SendSendMsgReq(int accountid);



/*==========================================================
 * Function 	: mail_StartRetr
 * Author		: sunguochen      
 * Description : ��ʼ�����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartSendMsgNormal(int accountid);



/*==========================================================
 * Function 	: mail_StartParseReq
 * Author		: sunguochen      
 * Description : ��ʼ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_StartParseReq(comm_uint16* file_name);



/*==========================================================
 * Function 	: mail_StartViewMail
 * Author		: sunguochen      
 * Description : ��ʼ�����ʼ�-���ս����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartRecvParseMail(comm_uint16* file_name, int accountid);



/*==========================================================
 * Function 	: mail_StartViewMail
 * Author		: sunguochen      
 * Description : ��ʼ�����ʼ�-׼������ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartViewMail(int msgid);



/*==========================================================
 * Function 	: mail_StartViewMail
 * Author		: sunguochen      
 * Description : �����ʼ����-֪ͨUI
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_ViewMailFinished(int message_ptr);



//���ӹ��ýӿ�
//�����ֻ�����ȫ����Ϣ
/*==========================================================
 * Function 	: mail_IsNetworkValid
 * Author		: sunguochen      
 * Description : �ж������Ƿ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_IsNetworkValid(void);



/*==========================================================
 * Function 	: mail_SetRecvingState
 * Author		: sunguochen      
 * Description : ���ý���״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetRecvingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetRecvingState
 * Author		: sunguochen      
 * Description : ��ȡ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetRecvingState(void);



/*==========================================================
 * Function 	: mail_SetSendingState
 * Author		: sunguochen      
 * Description : ���÷���״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSendingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetSendingState
 * Author		: sunguochen      
 * Description : ��ȡ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetSendingState(void);



/*==========================================================
 * Function 	: mail_SetParsingState
 * Author		: sunguochen      
 * Description : ���ý���״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetParsingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetParsingState
 * Author		: sunguochen      
 * Description : ��ȡ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetParsingState(void);



/*==========================================================
 * Function 	: mail_SetNeedRecvingState
 * Author		: sunguochen      
 * Description : ������Ҫ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetNeedRecvingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetRecvingState
 * Author		: sunguochen      
 * Description : ��ȡ�Ƿ���Ҫ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetNeedRecvingState(void);



/*==========================================================
 * Function 	: mail_SetAutoRecvingState
 * Author		: sunguochen      
 * Description : �����Զ�����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetAutoRecvingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetutoRecvingState
 * Author		: sunguochen      
 * Description : ��ȡ�Զ�����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetAutoRecvingState(void);



/*==========================================================
 * Function 	: mail_SetNeedSendingState
 * Author		: sunguochen      
 * Description : �����Ƿ���Ҫ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetNeedSendingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetNeedSendingState
 * Author		: sunguochen      
 * Description : ��ȡ�Ƿ���Ҫ����״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetNeedSendingState(void);



/*==========================================================
 * Function 	: mail_SetSockNeedCloseState
 * Author		: sunguochen      
 * Description : sock session ��Ҫ�ر�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSockNeedCloseState(int result);



/*==========================================================
 * Function 	: mail_GetSockNeedCloseState
 * Author		: sunguochen      
 * Description : sock session ��Ҫ�ر�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
int mail_GetSockNeedCloseState(void);



/*==========================================================
 * Function 	: mail_SetSSLPasserState
 * Author		: sunguochen      
 * Description : �����Ƿ�ssl��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetSSLPasserState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetSSLPasserState
 * Author		: sunguochen      
 * Description : ��ȡ�Ƿ�ssl��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetSSLPasserState(void);



/*==========================================================
 * Function 	: mail_GetTotalBoxMailNum
 * Author		: sunguochen      
 * Description : �������ȫ���ʼ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
int mail_GetTotalBoxMailNum(void);



/*==========================================================
 * Function 	: mail_IsMailBoxFull
 * Author		: sunguochen      
 * Description : �ʼ��Ƿ��Ѿ�200��-����
                      ����:�ѱ����+ ���ڽ�����+ �ѽ���δ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_IsMailBoxFull(void);



/*==========================================================
 * Function 	: mail_IsFreeSpaceOk
 * Author		: sunguochen      
 * Description : ʣ��ռ��Ƿ���Խ����ʼ�
                      ��ʣ��ռ�С��1M�����ٽ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_IsFreeSpaceOk(void);



/*==========================================================
 * Function 	: mail_AddUnRecvMailNum
 * Author		: sunguochen      
 * Description : ����δ�����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_AddUnRecvMailNum(int num);



/*==========================================================
 * Function 	: mail_AddUnRecvMailNum
 * Author		: sunguochen      
 * Description : δ�����ʼ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
int mail_GetUnRecvMailNum(void);



/*==========================================================
 * Function 	: mail_NewMailToParse
 * Author		: sunguochen      
 * Description : �������ʼ�����
                      ���������,��������
                      ����TRUE-�����¼�
                      �����ͽ����¼�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_NewMailToParse(comm_uint16* file_name, int *accountid);



/*==========================================================
 * Function 	: mail_GetNextMailToParse
 * Author		: sunguochen      
 * Description : �����һ��δ�����ʼ�
                      �������FALSE-˵���������
                      ������ڽ���-�򷵻�FALSE
 * Parameter	: type          parameter       Description
                      comm_uint16* file_name - ���ڱ����ʼ���
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_GetNextMailToParse(comm_uint16* file_name, int *accountid);



/*==========================================================
 * Function 	: mail_parseNextMail
 * Author		: sunguochen      
 * Description :  ���������ʼ�
 * Parameter	: type          parameter       Description
                      comm_uint16* file_name - ���ڱ����ʼ���
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_parseNextMail(void);



/*==========================================================
 * Function 	: mail_RetrAttachNotify
 * Author		: sunguochen      
 * Description : ���ո������-֪ͨUI
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachNotify(mail_retr_result_t *retr_result);



/*==========================================================
 * Function 	: mail_RetrAttachStart
 * Author		: sunguochen      
 * Description : ���ո���
 * Parameter	: type          parameter       Description
                       int accountid,                                     �˻�id
                       char *uidl,                                         �ʼ�uidl
                       MAIL_MESSAGE_PART_T * attach_list,   ������Ϣ
                       int retr_num                                      ���ո���
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachStart(int accountid, char *uidl, MAIL_MESSAGE_PART_T * attach_list, int retr_num);



/*==========================================================
 * Function 	: mail_CheckPasswordResult
 * Author		: sunguochen      
 * Description : ֪ͨUI  ��֤������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_CheckPasswordResult(comm_boolean result);



/*==========================================================
 * Function 	: mail_CheckPassWord
 * Author		: sunguochen      
 * Description : ��֤��������
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
 * Description : UI���ã�֪ͨparser task��ʼɾ���ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_StartDelMail(void);


/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description : UI���ã�֪ͨparser taskֹͣɾ���ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_CancleDelMail(void);


/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description : ɾ��������֪ͨUI�������ȴ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_DelMailFinish(comm_boolean result);

/*==========================================================
 * Function 	: mail_sendMailStartSave
 * Author		: lixu     
 * Description :  UI���ã������ʼ�ʱ֪ͨparser task����ͬʱ�����ʼ���outbox
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
 * Description :  �����ʼ����ر������֪ͨUI
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailSaveOk(comm_boolean result);


/*==========================================================
 * Function 	: mail_sortInboxMailList
 * Author		: lixu     
 * Description :  �Ѷ��ʼ��Ķ���ˢ���ռ����ʼ��б�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sortInboxMailList(void);

/*==========================================================
 * Function 	: mail_saveMailToDraft
 * Author		: lixu     
 * Description :  �������ʼ����ݸ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_saveMailToDrafts(comm_int32 account_id,MAIL_MESSAGE_T* message_ptr);


/*==========================================================
 * Function 	: mail_delAccountAllMail
 * Author		: lixu     
 * Description :  ɾ��ָ���ʻ��ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_delAccountAllMail(comm_int32 account_id);

/*==========================================================
 * Function 	: mail_delAccountAllMailFinish
 * Author		: lixu     
 * Description :  ɾ��ָ���ʻ��ʼ����֪ͨ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_delAccountAllMailFinish(void);

/*==========================================================
 * Function 	: mail_resetFactory
 * Author		: lixu     
 * Description :  �ָ���������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_resetFactory(void);

/*==========================================================
 * Function 	: mail_backToFactoryFinish
 * Author		: lixu     
 * Description :  �ָ������������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_backToFactoryFinish(void);

/*==========================================================
 * Function 	: mail_sendMailSendByMsgId
 * Author		: lixu     
 * Description :  UI���ã�ͨ��msgId�����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailSendByMsgId(comm_int32 accountId,comm_uint32 msgId);

/*==========================================================
 * Function 	: mail_StartDelMail
 * Author		: lixu     
 * Description :  �����ʼ������̷߳��ط��ͽ����֪ͨui
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_sendMailCallBackUi(comm_boolean result);

/*==========================================================
 * Function 	: mail_CancelRetringManual
 * Author		: sunguochen      
 * Description :  ֹͣ�ֶ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/27/2010 
 ==========================================================*/
void mail_CancelRetringManual(void);



/*==========================================================
 * Function 	: mail_CancelSendingManual
 * Author		: sunguochen      
 * Description :  ֹͣ�ֶ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/27/2010 
 ==========================================================*/
void mail_CancelSendingManual(void);




/*==========================================================
 * Function 	: mail_SendStart
 * Author		: sunguochen      
 * Description  : ��ʼ�����ʼ�
 * Parameter	: type          parameter       Description
                  int accountid            �˻�id
                  MAIL_MESSAGE_T *msg      �ʼ���Ϣ 
                  char *file_name          �ʼ��� 
                  char* fwd_uidl           �Ƿ������ת��
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
 * Description  : �Ƿ���Է����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/31/2010 
 ==========================================================*/
comm_boolean mail_CanSendingMail(void);



/*==========================================================
 * Function 	: mail_AppHandler
 * Author		: sunguochen      
 * Description : ���mail��Ӧ��ģ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
uint32 mail_AppHandler(void);



/*==========================================================
 * Function 	: mail_RetrMailNotify
 * Author		: sunguochen      
 * Description : ֪ͨ�û������ʼ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/9/2010 
 ==========================================================*/
void mail_RetrMailNotify(mail_retr_result_t     *retr_result);

/*==========================================================
 * Function 	: mail_delAllAccountAllMail
 * Author		: lixu     
 * Description :  ɾ�������ʻ��ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
void mail_delAllAccountAllMail(void);

/*==========================================================
 * Function 	: mail_delAllAccountAllMailFinish
 * Author		: lixu     
 * Description :  ɾ�������ʻ��ʼ����֪ͨ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
 void mail_delAllAccountAllMailFinish(void);



/*==========================================================
 * Function 	: mail_ResetFactorySetting
 * Author		: sunguochen      
 * Description : �ֻ�����ָ���������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/30/2010 
 ==========================================================*/
void mail_ResetFactorySetting(void);




/*==========================================================
 * Function 	: mail_progressbar
 * Author		: sunguochen      
 * Description : ��ʾ���ս���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_progressbar(mail_ProgressInfo* prog_info, void* userData);


/*==========================================================
 * Function 	: mail_FilesysIsInvalid
 * Author		: sunguochen      
 * Description : �ļ�ϵͳ�Ƿ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  9/25/2010 
 ==========================================================*/
BOOLEAN mail_FilesysIsInvalid(void);



/*==========================================================
 * Function 	: mail_SetUserCancelSendingState
 * Author		: sunguochen      
 * Description : ����ȡ������״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
void mail_SetUserCancelSendingState(comm_boolean result);



/*==========================================================
 * Function 	: mail_GetUserCancelSendingState
 * Author		: sunguochen      
 * Description : ��ȡȡ������״̬
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/19/2010 
 ==========================================================*/
comm_boolean mail_GetUserCancelSendingState(void);
#endif
