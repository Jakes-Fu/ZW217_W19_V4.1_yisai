/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_setting.h
 *	Author		: sunguochen    
 *	Description	: 手机邮箱设置
 *	Vesion		: 1.0        
 *	Date			: 2010-4-13  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-13      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_SETTING_H_
#define _MAIL_SETTING_H_


#include "comm_platform.h"
#include "mmiconnection_export.h"
#include "comm_socket.h"

#include "mail_pushparse.h"
#include "dal_time.h"
#include "mmimail_internal.h"

#define MAIL_MAX_ACCOUNT_NUM        4  //最多4个账户

#define MAIL_ACCOUNT_PTR_LEN        (4 + 4) //两个地址不保存

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
    uint16 pro_r;//协议的版本
    uint16 ssl;//ssl的值，0x0000 表示表示启用SSL0x0100 表示非SSL 
    uint16 receive_length;//接收邮件最大长度
    uint32 send_ip_add;//发送服务器IP地址
    uint16 send_port;//发送服务器端口
    uint32 receive_ip_add;//接收服务器IP地址
    uint16 receive_port;//接收服务器端口
    uint32 proxy_ip;//代理IP地址
    uint16 proxy_port;//代理端口    
    uint8 user_name[MAIL_USER_NAME_LEN + 1];//邮箱账号
    uint8 user_password[MAIL_PASSWORD_LEN + 1];//邮箱账号的密码
    uint8 sms_number[MAIL_SMS_NUM_LEN + 1];//短信号码
    uint8 apn[MAIL_APN_LEN + 1];//APN
    uint8 cid[MAIL_CID_LEN + 1];//帐户所在服务器的CID
    uint8 is_mas_account;//判断是否是MAS企业版账户,MAS账户需要进行ssl连接
    uint8 is_adc_account;//判断是否是ADC企业版账户,ADC账户不需要进行ssl连接
    
}MAIL_SETTING_INFO_T;

typedef struct _mail_account_info_
{

    int account_id;
    wchar   account_name[MMIMAIL_MAX_ACC_NAME_LEN + 1]; //帐号名称    
    MAIL_SETTING_INFO_T mail_account_settings;

}mail_account_info;


//设置信息结构
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
    //设置信息-可能有多个
    mail_account_info* mail_setting_list;

    //收到的临时数据信息
    mail_account_info* tmp_account_info;

}mail_setting_info;




/*==========================================================
 * Function 	: mail_settingInit
 * Author		: sunguochen      
 * Description : 设置初始化
                      将设置信息从配置文件中读出
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_settingInit(void);



/*==========================================================
 * Function 	: mail_settingInit
 * Author		: sunguochen      
 * Description : 设置终止
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_settingFinal(void);



/*==========================================================
 * Function 	: mail_settingRest
 * Author		: sunguochen      
 * Description : 设置复位
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_settingRest(void);



/*==========================================================
 * Function 	: mail_GetDefaultAccount
 * Author		: sunguochen      
 * Description : 获取默认账户id
 * Parameter	: type          parameter       Description
 * Return		: Void
                      如果是0-说明无默认账户-
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetDefaultAccount(void);



/*==========================================================
 * Function 	: mail_GetAccountInfoById
 * Author		: sunguochen      
 * Description : 通过账户id获得账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
mail_account_info* mail_GetAccountInfoById(int accountId);



/*==========================================================
 * Function 	: mail_GetAccountInfoById
 * Author		: sunguochen      
 * Description : 通过账户名称获得id
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAccountIdByUserName(char * user_name);



/*==========================================================
 * Function 	: mail_GetDefaultAccount
 * Author		: sunguochen      
 * Description : 获取账户个数
 * Parameter	: type          parameter       Description
 * Return		: Void
                      如果是0-说明无账户-
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAccountNum(void);



/*==========================================================
 * Function 	: mail_ExistLocalAccount
 * Author		: sunguochen      
 * Description : 是否存在本地账户
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_ExistLocalAccount(void);



/*==========================================================
 * Function 	: mail_GetDefAccount
 * Author		: sunguochen      
 * Description : 获得账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetDefAccount( MMIMAIL_ACCOUNT_INFO_T *account_info);



/*==========================================================
 * Function 	: mail_GetDefAccount
 * Author		: sunguochen      
 * Description : 根据索引获得账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAccountByIndex(int index, MMIMAIL_ACCOUNT_INFO_T *account_info);



/*==========================================================
 * Function 	: mail_SetDefAccount
 * Author		: sunguochen      
 * Description :设置默认账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetDefAccount(int accountid);



/*==========================================================
 * Function 	: mail_SetDefAccountByIndex
 * Author		: sunguochen      
 * Description :设置默认账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetDefAccountByIndex(int index);



/*==========================================================
 * Function 	: mail_DeleteAccount
 * Author		: sunguochen      
 * Description : 删除账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_DeleteAccount(int accountid);



/*==========================================================
 * Function 	: mail_DeleteAccount
 * Author		: sunguochen      
 * Description : 删除账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_DeleteAccountByIndex(int index);



/*==========================================================
 * Function 	: mail_SetAccountInfo
 * Author		: sunguochen      
 * Description : 设置账户信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetAccountInfo( MMIMAIL_ACCOUNT_INFO_T *account_info);



/*==========================================================
 * Function 	: mail_SaveAccountName
 * Author		: sunguochen      
 * Description : 保存密码
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
comm_boolean mail_SaveAccountName(int accountid, char *password);



/*==========================================================
 * Function 	: mail_WriteAccountSettings
 * Author		: sunguochen      
 * Description : 保存接收到的账户信息
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
 * Description : 设置-回复时是否带原邮件正文
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetReplyWithOriginalCont(comm_boolean result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description :获取设置-回复时是否带原邮件正文
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetReplyWithOriginalCont(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 设置-是否添加签名信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAddSignature(comm_boolean result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 获取设置-是否添加签名信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAddSignature(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 设置-签名信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetSignature_text(wchar* Signature_text);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 获取设置-签名信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
wchar* mail_GetSignature_text(void);



/*==========================================================
 * Function 	: mail_SetSilenceDuration
 * Author		: sunguochen      
 * Description :设置-静音时段
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetSilenceDuration(SCI_TIME_T* silence_start_time, SCI_TIME_T* silence_end_time);



/*==========================================================
 * Function 	: mail_SetSilenceDuration
 * Author		: sunguochen      
 * Description :获取设置-静音时段
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_GetSilenceDuration(SCI_TIME_T* silence_start_time, SCI_TIME_T* silence_end_time);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description :设置-是否自动下载附件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachAutoDownload(comm_boolean result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 获取设置-是否自动下载附件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAttachAutoDownload(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description :设置-是否下载附件大小
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachDownloadLimit(int limit_size);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 获取设置-是否下载附件大小
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAttachDownloadLimit(void);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 附件格式转换
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachDownMode(int result);



/*==========================================================
 * Function 	: mail_SetReplyWithOriginalCont
 * Author		: sunguochen      
 * Description : 附件格式转换
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAttachDownMode(void);



/*==========================================================
 * Function 	: mail_IsSilenceDuration
 * Author		: sunguochen      
 * Description : 判断现在是否是静音时段
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
 * Description : 获得账户名
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
char* mail_UserName(int  accountid);



/*==========================================================
 * Function 	: mail_passWord
 * Author		: sunguochen      
 * Description : 获得账户密码
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
char* mail_passWord(int  accountid);



/*==========================================================
 * Function 	: mail_SenderAddr
 * Author		: sunguochen      
 * Description : 获取邮箱发件人地址
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
char * mail_SenderAddr(int accountid);



/*==========================================================
 * Function 	: mail_PopLoginMethod
 * Author		: sunguochen      
 * Description : 是否POP方式登录邮箱
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
comm_boolean mail_IsPopLoginMethod(int accountid);



/*==========================================================
 * Function 	: mail_protocol_type
 * Author		: sunguochen      
 * Description : 获取邮箱协议，分为POP3 & IMAP4
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID 
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
 * Description : 是否使用代理
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
 * Description : 获得账户名称
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
char* mail_AccountName(uint16 accountid);



/*==========================================================
 * Function 	: mail_AccountIsSSL
 * Author		: sunguochen      
 * Description : 账户是否采用ssl
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_AccountIsSSL(int accountid);

/*==========================================================
 * author		: liangshun      
 * function 	: comm_boolean mail_AccountIsGroupAccount(int accountid)
 * decr		: 判断是否是企业账户
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  4/1/2011 
 ==========================================================*/
comm_boolean mail_AccountIsGroupAccount(int accountid);

/*==========================================================
 * Function 	: mail_isCardSwitch
 * Author		: sunguochen      
 * Description : 卡是否更换
 * Parameter	: type          parameter       Description
 * Return		: Void
                      TRUE:  已换
                      FALSE: 未换
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_isCardSwitch(void);



/*==========================================================
 * Function 	: mail_DelOnServerAfterRetr
 * Author		: sunguochen      
 * Description : 下载后删除服务器邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_boolean mail_DelOnServerAfterRetr(void);



/*==========================================================
 * Function 	: mail_RetrMailMaxLength
 * Author		: sunguochen      
 * Description : 下载邮件最大个数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_RetrMailMaxLength(void);



/*==========================================================
 * Function 	: mail_ReceivingNewMsg
 * Author		: sunguochen      
 * Description : 接收邮件范围:
 * Parameter	: type          parameter       Description
 * Return		: TRUE : 接收新邮件
                       FALSE : 接收全部
 * Effect		: NO
 * History	: created  5/5/2010 
 ==========================================================*/
comm_boolean mail_ReceivingNewMsg(uint16 accountid);



/*==========================================================
 * Function 	: mail_AccountSettingsArrival
 * Author		: sunguochen      
 * Description : 账户设置信息处理
 * Parameter	: type          parameter       Description
                       void *accountinfo,           --账户信息
                       comm_boolean is_single  --是否单个
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_AccountSettingsArrival(void *accountinfo, comm_boolean is_single);



/*==========================================================
 * Function 	: mail_WriteAllSettings
 * Author		: sunguochen      
 * Description : 保存接收到的账户信息-临时信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteAllSettings( void );



/*==========================================================
 * Function 	: mail_SettinginfoRecved
 * Author		: sunguochen      
 * Description : 是否收到过配置信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
comm_boolean mail_SettinginfoRecved(void);



/*==========================================================
 * Function 	: mail_RecvedSettinginfoReset
 * Author		: sunguochen      
 * Description : 清除收到的配置信息标志
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_RecvedSettinginfoReset(void);



/*==========================================================
 * Function 	: mail_ExistTmpSettinginfo
 * Author		: sunguochen      
 * Description : 是否存在临时配置信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/12/2010 
 ==========================================================*/
comm_boolean mail_ExistTmpSettinginfo(void);



/*==========================================================
 * Function 	: mail_AccountNeedPassword
 * Author		: sunguochen      
 * Description : 企业版用户需要用户提前输入密码
                      否则无法收发邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/13/2010 
 ==========================================================*/
comm_boolean mail_AccountNeedPassword(int accountid);



#endif
