/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_time.h
 *	Author		: sunguochen    
 *	Description	: ��ʱ��������
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_TIME_H_
#define _MAIL_TIME_H_

#include "comm_platform.h"



//��ʱ���ص�
typedef void mail_TimeoutFn(comm_uint32 timerId1, void* timerId2);




/*==========================================================
 * Function 	: mail_TimerInit
 * Author		: sunguochen      
 * Description : ��ʱ����Ϣ��ʼ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerInit(void);



/*==========================================================
 * Function 	: mail_TimerTerminate
 * Author		: sunguochen      
 * Description : ��ʱ������ֹ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerTerminate(void);



/*==========================================================
 * Function 	: mail_TimerSet
 * Author		: sunguochen      
 * Description : ���ö�ʱ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerSet(comm_uint32 dsmail, mail_TimeoutFn* fn, comm_uint32 timerId1, void* timerId2);



/*==========================================================
 * Function 	: mail_TimerReset
 * Author		: sunguochen      
 * Description : ɾ����ʱ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerReset(mail_TimeoutFn* fn, comm_uint32 timerId1, const void* timerId2);



/*==========================================================
 * Function 	: mail_timerExpiry
 * Author		: sunguochen      
 * Description : ��ʱ��ǿ�Ƶ�ʱ
                      ���ö�ʱ���ص�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_timerExpiry(comm_uint32 extTimerId);


/*==========================================================
 * Function 	: mail_timer_callback
 * Author		: sunguochen      
 * Description : ��ʱ���ص�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_timer_taskcallback(comm_uint32 arg);


#endif 
