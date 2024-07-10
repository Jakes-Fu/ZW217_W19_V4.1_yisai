/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_send.h
 *	Author		: sunguochen    
 *	Description	: ���ʹ���
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MAIL_SEND_H_
#define _MAIL_SEND_H_


/*==========================================================
 * Function 	: mail_SendMailInit
 * Author		: sunguochen      
 * Description : ���ͳ�ʼ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mail_SendMailInit( void );

/*==========================================================
 * Function 	: mail_sendTest
 * Author		: sunguochen      
 * Description : ���Ͳ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mail_sendTest(int accountid);

/*==========================================================
 * Function 	: mail_sendStart
 * Author		: sunguochen      
 * Description  : ��ʼ�����ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/31/2010 
 ==========================================================*/
void mail_smtpsendStart(int accountid,MAIL_MESSAGE_T *msg, char *file_name, char* fwd_uidl);

/*==========================================================
 * Function 	: mail_send_terminate
 * Author		: sunguochen      
 * Description : ��ֹ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mail_send_terminate(void);



#endif
