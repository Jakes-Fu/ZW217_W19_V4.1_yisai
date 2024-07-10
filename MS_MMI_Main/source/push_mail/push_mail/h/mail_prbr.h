/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_prbr.h
 *	Author		: sunguochen    
 *	Description	: 发送/接收进度
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_PRBR_H_
#define _MAIL_PRBR_H_

#include "mail_type.h"




/*==========================================================
 * Function 	: initMailProgressInfo
 * Author		: sunguochen      
 * Description : 初始化进度信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
MailProgressInfo* initMailProgressInfo( void );



/*==========================================================
 * Function 	: calculateProgress
 * Author		: sunguochen      
 * Description : 计算进度
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
comm_boolean calculateProgress(MailProgressInfo* progress);



/*==========================================================
 * Function 	: adjustProgressTotalSize
 * Author		: sunguochen      
 * Description : 总大小修正
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void adjustProgressTotalSize(MailProgressInfo* progress, comm_uint32 actualHeaderSize);



/*==========================================================
 * Function 	: destroyMailProgressInfo
 * Author		: sunguochen      
 * Description : 删除进度信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void destroyMailProgressInfo(MailProgressInfo* progress);

#endif 
