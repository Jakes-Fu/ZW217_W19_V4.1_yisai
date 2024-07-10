/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_send.h
 *	Author		: sunguochen    
 *	Description	: 发送处理
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MAIL_SEND_H_
#define _MAIL_SEND_H_


/*==========================================================
 * Function 	: mail_SendMailInit
 * Author		: sunguochen      
 * Description : 发送初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mail_SendMailInit( void );

/*==========================================================
 * Function 	: mail_sendTest
 * Author		: sunguochen      
 * Description : 发送测试
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mail_sendTest(int accountid);

/*==========================================================
 * Function 	: mail_sendStart
 * Author		: sunguochen      
 * Description  : 开始发送邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/31/2010 
 ==========================================================*/
void mail_smtpsendStart(int accountid,MAIL_MESSAGE_T *msg, char *file_name, char* fwd_uidl);

/*==========================================================
 * Function 	: mail_send_terminate
 * Author		: sunguochen      
 * Description : 终止发送
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mail_send_terminate(void);



#endif
