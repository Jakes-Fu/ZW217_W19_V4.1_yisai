/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_feature.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef  _COMM_FEATURE_H_
#define _COMM_FEATURE_H_

//是否支持完整OMA download tag 目前不支持
//#define OMA_DOWNLOAD_FULL_SUPPORT

//
#define COMM_DEBUG_MODE

//支持将log信息保存到文件中
#define COMM_LOG_IN_FILE_SUPPORT

//支持IMAP4
//#define MAIL_SUPPORT_IMAP4

//是否使用DEBUG模式内存
//#define MAIL_MEMORY_DEBUG_MODE

//支持CAPA
//#define MAIL_SUPPORT_POP3_CAPA


//cmpop 协议扩展
#define MAIL_SUPPORT_CMPOP 

//cmsmtp 协议扩展
#define MAIL_SUPPORT_CMSMTP 

//#define MAIL_PUSH_SETTING_TEST

//#define MAIL_DEBUG_SAVE_FILE
#ifdef WIN32
//模拟器是否采用windows socket接口
//#define MAIL_SUPPORT_WIN_SOCKET
#endif

//本地管理feature
#define MAIL_FLDR_MANAGER
#define MAIL_FLDR_LIST_CHECK
#define MAIL_FLDR_FILE_ERROR  //定义将文件操作错误归为一个MAIL_FILE_ERROR
#define MAIL_FLDR_CUT_SETTING //本地管理模块去掉设置
#define MAIL_FLDR_UNKNOWN_MSG //需要确认的交互消息
#define MAIL_FLDR_CUT_MIME //本地管理模块暂时去掉mime接口
#define MAIL_FLDR_CUT_OTHER //用于前期编译砍掉不明确的接口
//#define MAIL_FLDR_FOR_QO7 //增加升级部分,增加多帐户处理
//#define MAIL_FLDR_DEBUG //增加用于前期调试用
#endif

