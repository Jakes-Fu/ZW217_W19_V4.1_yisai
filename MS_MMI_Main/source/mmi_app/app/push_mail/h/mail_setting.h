/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_setting.h
 *	Author		: sunguochen    
 *	Description	: �ֻ���������
 *	Vesion		: 1.0        
 *	Date			: 2010-4-13  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-13      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_SETTING_H_
#define _MAIL_SETTING_H_


#include "comm_platform.h"
#include "mmiconnection_export.h"
#include "comm_socket.h"

#include "mail_pushparse.h"
#include "dal_time.h"
#include "mmimail_internal.h"

#define MAIL_MAX_ACCOUNT_NUM        4  //���4���˻�

#define MAIL_ACCOUNT_PTR_LEN        (4 + 4) //������ַ������

#define MAIL_INVALD_ACCOUNT_ID      0

#define MAIL_ACCOUNT_ID_START       1


#define MAIL_IMSI_SAVE_LEN          20
#define MAIL_SIGNATURE_LEN          64

#define MAIL_USER_NAME_LEN          MMIMAIL_MAX_ADDRESS_LEN
#define MAIL_PASSWORD_LEN           MMIMAIL_MAX_PASSWORD_LEN
#define MAIL_APN_LEN                16
#define MAIL_SMS_NUM_LEN            16
#define MAIL_CID_LEN                6


#define MAIL_BODY_MAX_LEN           100*1024

#ifdef WIN32
#define COMM_NET_PROXY_PORT       808
#define COMM_NET_PROXY_ADDR       0xC0A8011F
#else
#define COMM_NET_PROXY_PORT       0x50
#define COMM_NET_PROXY_ADDR       0x0A0000AC
#endif

#ifdef WIN32
#define COMM_NET_POP3_SERVER_PORT       110
#define COMM_NET_POP3_SERVER_ADDR       0xC0A80122

#define COMM_NET_SMTP_SERVER_PORT       25
#define COMM_NET_SMTP_SERVER_ADDR       0xC0A80122
#else
#define COMM_NET_POP3_SERVER_PORT       0x46BE
#define COMM_NET_POP3_SERVER_ADDR       0xDAC8F3EA

#define COMM_NET_SMTP_SERVER_PORT       0x4669
#define COMM_NET_SMTP_SERVER_ADDR       0xDAC8F3EA
#endif


#define MAIL_SETTINGS_FILE         "C:\\mail\\mail_settings.dat"

typedef enum
{      
    MAIL_PROTOCOL_POP3,
    MAIL_PROTOCOL_IMAP4 

}MAIL_PROTOCOL_E;




typedef enum
{
    MAIL_PROGRESS_CONNECTING = 0,             
    MAIL_PROGRESS_GETTING_MSG_NUM,  
    MAIL_PROGRESS_SYNCHRONIZING_MSG,      
    MAIL_PROGRESS_RETRIEVING_MSG,         
    MAIL_PROGRESS_SENDING_MSG,
    MAIL_PROGRESS_RETRIEVING_ATTACH,    
    MAIL_PROGRESS_MAX    
                                             
} MAIL_PROGRESS_E;


typedef enum
{
    MAIL_NO_CHANGE,
    MAIL_CHANGE_TO_PNG,
    MAIL_CHANGE_TO_JPG,
    MAIL_CHANGE_MAX
}MAIL_CHANGE_FORMAT_E;


typedef struct _mail_ProgressInfo_
{
    SIGNAL_VARS
    comm_uint32         totNumOfMsg;      
    comm_uint32         processedBytes;   

    comm_int32  totalByteSize;    
    comm_uint16 msgIndex;         
    comm_uint16         bytePercent;      

    comm_uint16         staticPercent;    

    comm_uint16         lastNotified;     
    int currentMailStatus;

} mail_ProgressInfo;



typedef struct _AIL_SETTING_INFO_T_
{
    uint16 pro_r;//Э��İ汾
    uint16 ssl;//ssl��ֵ��0x0000 ��ʾ��ʾ����SSL0x0100 ��ʾ��SSL 
    uint16 receive_length;//�����ʼ���󳤶�
    uint32 send_ip_add;//���ͷ�����IP��ַ
    uint16 send_port;//���ͷ������˿�
    uint32 receive_ip_add;//���շ�����IP��ַ
    uint16 receive_port;//���շ������˿�
    uint32 proxy_ip;//����IP��ַ
    uint16 proxy_port;//����˿�    
    uint8 user_name[MAIL_USER_NAME_LEN + 1];//�����˺�
    uint8 user_password[MAIL_PASSWORD_LEN + 1];//�����˺ŵ�����
    uint8 sms_number[MAIL_SMS_NUM_LEN + 1];//���ź���
    uint8 apn[MAIL_APN_LEN + 1];//APN
    uint8 cid[MAIL_CID_LEN + 1];//�ʻ����ڷ�������CID
    uint8 is_mas_account;//�ж��Ƿ���MAS��ҵ���˻�,MAS�˻���Ҫ����ssl����
    uint8 is_adc_account;//�ж��Ƿ���ADC��ҵ���˻�,ADC�˻�����Ҫ����ssl����
    
}MAIL_SETTING_INFO_T;

typedef struct _mail_account_info_
{

    int account_id;
    wchar   account_name[MMIMAIL_MAX_ACC_NAME_LEN + 1]; //�ʺ�����    
    MAIL_SETTING_INFO_T mail_account_settings;

}mail_account_info;


//������Ϣ�ṹ
typedef struct _mail_setting_info_
{

    int                     account_num;
    int                     default_account;
    char                   card_imsi[MAIL_IMSI_SAVE_LEN + 1];
    wchar                 signature_text[MAIL_SIGNATURE_LEN + 1];
    SCI_TIME_T         silence_start_time;
    SCI_TIME_T         silence_end_time;
    int                      attach_limit_size;
    int                      attach_trans_mode;    
    comm_boolean     attach_auto_download;
    comm_boolean     reply_with_original_cont;
    comm_boolean     add_signature;    
    comm_boolean     recved_settings;      
    //������Ϣ-�����ж��
    mail_account_info* mail_setting_list;

    //�յ�����ʱ������Ϣ
    mail_account_info* tmp_account_info;

}mail_setting_info;




/*==========================================================
 * Function 	: mail_settingInit
 * Author		: sunguochen      
 * Description : ���ó�ʼ��
                      ��������Ϣ�������ļ��ж���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_settingInit(void);



/*==========================================================
 * Function 	: mail_settingInit
 * Author		: sunguochen      
 * Description : ������ֹ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_settingFinal(void);



/*==========================================================
 * Function 	: mail_settingRest
 * Author		: sunguochen      
 * Description : ���ø�λ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_settingRest(void);



/*==========================================================
 * Function 	: mail_GetDefaultAccount
 * Author		: sunguochen      
 * Description : ��ȡĬ���˻�id
 * Parameter	: type          parameter       Description
 * Return		: Void
                      �����0-˵����Ĭ���˻�-
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetDefaultAccount(void);



/*==========================================================
 * Function 	: mail_GetAccountInfoById
 * Author		: sunguochen      
 * Description : ͨ���˻�id����˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
mail_account_info* mail_GetAccountInfoById(int accountId);



/*==========================================================
 * Function 	: mail_GetAccountInfoById
 * Author		: sunguochen      
 * Description : ͨ���˻����ƻ��id
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAccountIdByUserName(char * user_name);



/*==========================================================
 * Function 	: mail_GetDefaultAccount
 * Author		: sunguochen      
 * Description : ��ȡ�˻�����
 * Parameter	: type          parameter       Description
 * Return		: Void
                      �����0-˵�����˻�-
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAccountNum(void);



/*==========================================================
 * Function 	: mail_ExistLocalAccount
 * Author		: sunguochen      
 * Description : �Ƿ���ڱ����˻�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_ExistLocalAccount(void);



/*==========================================================
 * Function 	: mail_GetDefAccount
 * Author		: sunguochen      
 * Description : ����˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetDefAccount( MMIMAIL_ACCOUNT_INFO_T *account_info);



/*==========================================================
 * Function 	: mail_GetDefAccount
 * Author		: sunguochen      
 * Description : ������������˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAccountByIndex(int index, MMIMAIL_ACCOUNT_INFO_T *account_info);



/*==========================================================
 * Function 	: mail_SetDefAccount
 * Author		: sunguochen      
 * Description :����Ĭ���˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetDefAccount(int accountid);



/*==========================================================
 * Function 	: mail_SetDefAccountByIndex
 * Author		: sunguochen      
 * Description :����Ĭ���˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetDefAccountByIndex(int index);



/*==========================================================
 * Function 	: mail_DeleteAccount
 * Author		: sunguochen      
 * Description : ɾ���˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_DeleteAccount(int accountid);



/*==========================================================
 * Function 	: mail_DeleteAccount
 * Author		: sunguochen      
 * Description : ɾ���˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_DeleteAccountByIndex(int index);



/*==========================================================
 * Function 	: mail_SetAccountInfo
 * Author		: sunguochen      
 * Description : �����˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetAccountInfo( MMIMAIL_ACCOUNT_INFO_T *account_info);



/*==========================================================
 * Function 	: mail_SaveAccountName
 * Author		: sunguochen      
 * Description : ��������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
comm_boolean mail_SaveAccountName(int accountid, char *password);



/*==========================================================
 * Function 	: mail_WriteAccountSettings
 * Author		: sunguochen      
 * Description : ������յ����˻���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteAccountSettings(MMIMAIL_SETTING_INFO_T* account_info);



/*==========================================================
 * Function 	: mail_accountIsValid
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/14/2010 
 ==========================================================*/
comm_boolean mail_accountIsValid(int accountid);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ����-�ظ�ʱ�Ƿ��ԭ�ʼ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetReplyWithOriginalCont(comm_boolean result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description :��ȡ����-�ظ�ʱ�Ƿ��ԭ�ʼ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetReplyWithOriginalCont(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ����-�Ƿ����ǩ����Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAddSignature(comm_boolean result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ��ȡ����-�Ƿ����ǩ����Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAddSignature(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ����-ǩ����Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetSignature_text(wchar* Signature_text);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ��ȡ����-ǩ����Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
wchar* mail_GetSignature_text(void);



/*==========================================================
 * Function 	: mail_SetSilenceDuration
 * Author		: sunguochen      
 * Description :����-����ʱ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetSilenceDuration(SCI_TIME_T* silence_start_time, SCI_TIME_T* silence_end_time);



/*==========================================================
 * Function 	: mail_SetSilenceDuration
 * Author		: sunguochen      
 * Description :��ȡ����-����ʱ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_GetSilenceDuration(SCI_TIME_T* silence_start_time, SCI_TIME_T* silence_end_time);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description :����-�Ƿ��Զ����ظ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachAutoDownload(comm_boolean result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ��ȡ����-�Ƿ��Զ����ظ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAttachAutoDownload(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description :����-�Ƿ����ظ�����С
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachDownloadLimit(int limit_size);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ��ȡ����-�Ƿ����ظ�����С
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAttachDownloadLimit(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ������ʽת��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachDownMode(int result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : ������ʽת��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAttachDownMode(void);



/*==========================================================
 * Function 	: mail_IsSilenceDuration
 * Author		: sunguochen      
 * Description : �ж������Ƿ��Ǿ���ʱ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/14/2010 
 ==========================================================*/
comm_boolean mail_IsSilenceDuration(void);



/*==========================================================
 * Function 	: mail_TlsEnabled
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
comm_boolean mail_TlsEnabled(void);



/*==========================================================
 * Function 	: mail_UserName
 * Author		: sunguochen      
 * Description : ����˻���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
char* mail_UserName(int  accountid);



/*==========================================================
 * Function 	: mail_passWord
 * Author		: sunguochen      
 * Description : ����˻�����
 * Parameter	: type          parameter       Description
                      uint16 accountid --�˻�ID
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
char* mail_passWord(int  accountid);



/*==========================================================
 * Function 	: mail_SenderAddr
 * Author		: sunguochen      
 * Description : ��ȡ���䷢���˵�ַ
 * Parameter	: type          parameter       Description
                      uint16 accountid --�˻�ID 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
char * mail_SenderAddr(int accountid);



/*==========================================================
 * Function 	: mail_PopLoginMethod
 * Author		: sunguochen      
 * Description : �Ƿ�POP��ʽ��¼����
 * Parameter	: type          parameter       Description
                      uint16 accountid --�˻�ID 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
comm_boolean mail_IsPopLoginMethod(int accountid);



/*==========================================================
 * Function 	: mail_protocol_type
 * Author		: sunguochen      
 * Description : ��ȡ����Э�飬��ΪPOP3 & IMAP4
 * Parameter	: type          parameter       Description
                      uint16 accountid --�˻�ID 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
MAIL_PROTOCOL_E mail_protocol_type(int accountid);



/*==========================================================
 * Function 	: comm_NetGetProxyAddr
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/12/2010 
 ==========================================================*/
void comm_NetProxyAddr(int accountid, comm_sockaddr* addr);



/*==========================================================
 * Function 	: comm_NetSendServerAddr
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/12/2010 
 ==========================================================*/
void comm_NetSendServerAddr(int accountid, comm_sockaddr* addr);



/*==========================================================
 * Function 	: comm_NetRetrServerAddr
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/12/2010 
 ==========================================================*/
void comm_NetRetrServerAddr(int accountid, comm_sockaddr* addr);



/*==========================================================
 * Function 	: mail_NetUsedProxy
 * Author		: sunguochen      
 * Description : �Ƿ�ʹ�ô���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
comm_boolean mail_NetUsedProxy(int accountid);



/*==========================================================
 * Function 	: comm_NetGetSetting
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void comm_NetGetSetting(int  accountid, MMICONNECTION_LINKSETTING_DETAIL_T* settings);



/*==========================================================
 * Function 	: mail_AccountName
 * Author		: sunguochen      
 * Description : ����˻�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
char* mail_AccountName(uint16 accountid);



/*==========================================================
 * Function 	: mail_AccountIsSSL
 * Author		: sunguochen      
 * Description : �˻��Ƿ����ssl
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_AccountIsSSL(int accountid);

/*==========================================================
 * author		: liangshun      
 * function 	: comm_boolean mail_AccountIsGroupAccount(int accountid)
 * decr		: �ж��Ƿ�����ҵ�˻�
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  4/1/2011 
 ==========================================================*/
comm_boolean mail_AccountIsGroupAccount(int accountid);

/*==========================================================
 * Function 	: mail_isCardSwitch
 * Author		: sunguochen      
 * Description : ���Ƿ����
 * Parameter	: type          parameter       Description
 * Return		: Void
                      TRUE:  �ѻ�
                      FALSE: δ��
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_isCardSwitch(void);



/*==========================================================
 * Function 	: mail_DelOnServerAfterRetr
 * Author		: sunguochen      
 * Description : ���غ�ɾ���������ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_boolean mail_DelOnServerAfterRetr(void);



/*==========================================================
 * Function 	: mail_RetrMailMaxLength
 * Author		: sunguochen      
 * Description : �����ʼ�������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_RetrMailMaxLength(void);



/*==========================================================
 * Function 	: mail_ReceivingNewMsg
 * Author		: sunguochen      
 * Description : �����ʼ���Χ:
 * Parameter	: type          parameter       Description
 * Return		: TRUE : �������ʼ�
                       FALSE : ����ȫ��
 * Effect		: NO
 * History	: created  5/5/2010 
 ==========================================================*/
comm_boolean mail_ReceivingNewMsg(uint16 accountid);



/*==========================================================
 * Function 	: mail_AccountSettingsArrival
 * Author		: sunguochen      
 * Description : �˻�������Ϣ����
 * Parameter	: type          parameter       Description
                       void *accountinfo,           --�˻���Ϣ
                       comm_boolean is_single  --�Ƿ񵥸�
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_AccountSettingsArrival(void *accountinfo, comm_boolean is_single);



/*==========================================================
 * Function 	: mail_WriteAllSettings
 * Author		: sunguochen      
 * Description : ������յ����˻���Ϣ-��ʱ��Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteAllSettings( void );



/*==========================================================
 * Function 	: mail_SettinginfoRecved
 * Author		: sunguochen      
 * Description : �Ƿ��յ���������Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
comm_boolean mail_SettinginfoRecved(void);



/*==========================================================
 * Function 	: mail_RecvedSettinginfoReset
 * Author		: sunguochen      
 * Description : ����յ���������Ϣ��־
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_RecvedSettinginfoReset(void);



/*==========================================================
 * Function 	: mail_ExistTmpSettinginfo
 * Author		: sunguochen      
 * Description : �Ƿ������ʱ������Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
comm_boolean mail_ExistTmpSettinginfo(void);



/*==========================================================
 * Function 	: mail_AccountNeedPassword
 * Author		: sunguochen      
 * Description : ��ҵ���û���Ҫ�û���ǰ��������
                      �����޷��շ��ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/13/2010 
 ==========================================================*/
comm_boolean mail_AccountNeedPassword(int accountid);



#endif
