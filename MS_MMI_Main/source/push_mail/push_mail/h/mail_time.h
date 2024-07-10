/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_time.h
 *	Author		: sunguochen    
 *	Description	: 定时器处理函数
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_TIME_H_
#define _MAIL_TIME_H_

#include "comm_platform.h"



//定时器回调
typedef void mail_TimeoutFn(comm_uint32 timerId1, void* timerId2);




/*==========================================================
 * Function 	: mail_TimerInit
 * Author		: sunguochen      
 * Description : 定时器信息初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerInit(void);



/*==========================================================
 * Function 	: mail_TimerTerminate
 * Author		: sunguochen      
 * Description : 定时操作终止
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerTerminate(void);



/*==========================================================
 * Function 	: mail_TimerSet
 * Author		: sunguochen      
 * Description : 设置定时器
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerSet(comm_uint32 dsmail, mail_TimeoutFn* fn, comm_uint32 timerId1, void* timerId2);



/*==========================================================
 * Function 	: mail_TimerReset
 * Author		: sunguochen      
 * Description : 删除定时器
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_TimerReset(mail_TimeoutFn* fn, comm_uint32 timerId1, const void* timerId2);



/*==========================================================
 * Function 	: mail_timerExpiry
 * Author		: sunguochen      
 * Description : 定时器强制到时
                      调用定时器回调函数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_timerExpiry(comm_uint32 extTimerId);


/*==========================================================
 * Function 	: mail_timer_callback
 * Author		: sunguochen      
 * Description : 定时器回调函数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mail_timer_taskcallback(comm_uint32 arg);


#endif 
