/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_pushparse.h
 *	Author		: 
 *	Description	: 手机邮箱相关push解析
 *	Vesion		: 1.0        
 *	Date			: 2010-5-12  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-5-12      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
 
#ifndef _MAIL_PUSHPARSE_H_
#define _MAIL_PUSHPARSE_H_

#include "comm_feature.h"



#include "mmimms_push.h"


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct
{
    uint16 pro_r;//协议的版本
    uint8  record_type;//数据记录类型
    uint16 ssl;//ssl的值，0x0000 表示表示启用SSL0x0100 表示非SSL 
    uint32 length;//是指从该字段后的数据记录的长度。（不包括该字段的长度）
    uint16 receive_length;//接收邮件最大长度
    uint32 send_ip_add;//发送服务器IP地址
    uint16 send_port;//发送服务器端口
    uint32 receive_ip_add;//接收服务器IP地址
    uint16 receive_port;//接收服务器端口
    uint8 user_name[64];//邮箱账号
    uint8 user_password[16];//邮箱账号的密码
    uint8 sms_number[16];//短信号码
    uint8 apn[16];//APN
}MMIMAIL_SETTING_INFO_T;//单帐户设置信息的结构体

typedef struct
{
    uint8 cid[6];//帐户所在服务器的CID
    uint32 send_ip_addr;//发送服务器IP地址
    uint16 send_port;//发送服务器端口
    uint32 receive_ip_addr;//接收服务器IP地址
    uint16 receive_port;//接收服务器端口
    uint32 proxy_ip;//代理IP地址
    uint16 proxy_port;//代理端口
    uint8 ssl;//ssl
    uint8 user_name[64];//邮箱账号
    uint8 user_password[16];//邮箱帐户的密码
    uint8 apn[16];//apn
    uint8 smsnumber[16];//短信号码
}MMIMAIL_MORE_SETTING_CONTENT_DATA_INFO_T;//多帐户配置消息中的setting的内容

typedef struct
{
    uint16 setting_info_length;//设置信息的长度
    MMIMAIL_MORE_SETTING_CONTENT_DATA_INFO_T setting_data;//设置信息的内容
}MMIMAIL_MORE_SETTING_CONTENT_INFO_T;//多帐户配置消息中的setting的结构体，内容和长度

typedef struct
{
    uint16 pro_r;//协议版本
    uint8  record_type;//数据记录的类型
    uint16 record_length;//数据记录的总长度
    uint8  phone_number[6];//手机号码
    uint8  account_num;//帐户数
    MMIMAIL_MORE_SETTING_CONTENT_INFO_T setting_info[4];//多帐户配置信息的setting信息，最多帐户4个
}MMIMAIL_MORE_SETTING_INFO_T;//多帐户设置信息的结构体

typedef struct
{
    uint8 charset;//编码方式
    uint8 emn_attributes;//emn 属性
    uint8 mail_attributes;//邮件属性
    uint8 user_name[64];//用户名
    uint8 user_password[16];//用户密码
    uint8 timestamp_len;//时间戳的长度
    uint8 timestamp[10];//时间戳
}MMIMAIL_EMN_INFO_T;//邮件通知的结构体

typedef struct
{
    uint16 pro_r;//协议版本号
    uint8  record_type;//数据记录类型
    uint32 length;//数据记录的长度
    uint8 md5[16];//md5串
    uint8 timestamp[20];//时间戳
    uint8 user_name[64];//邮箱账号  
}MMIMAIL_BOMB_INFO_T;//炸弹信息的结构体

typedef struct
{
    uint16 pro_r;//协议版本
    uint8 record_type;//数据记录类型
    uint32 length;//数据记录长度
    char * data;//数据字段1的数据
}MMIMAIL_INFOREMIND_INFO_T;//资费信息的结构体



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*==========================================================
 * author		: sunlu      
 * function 	: MMIMAIL_IsPushMailForPort
 * decr		: 判断是不是pushmail的消息，先判断端口号
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/15/2010 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_IsPushMailForPort(uint8 *data_head, uint8 length);

/*==========================================================
 * author		: sunlu      
 * function 	: MMIMAIL_IsPushMailForContent
 * decr		: 判断是不是pushmail的消息(setting\emn)
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/6/2010 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_IsPushMailForContent(uint8 * user_valid_data_arr,uint16 length);


/*==========================================================
 * author		: sunlu      
 * function 	: mail_strnistr
 * decr		: 用来判断在str中是不是包括sstr
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/6/2010 
 ==========================================================*/
char* mail_strnistr(const char* str,const int str_len, const char* sstr);


/*==========================================================
 * author		: sunlu      
 * function 	: mail_ToUpperCase
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/6/2010 
 ==========================================================*/
char mail_ToUpperCase(char ch);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif



#endif // _MAIL_PUSHPARSE_H_

