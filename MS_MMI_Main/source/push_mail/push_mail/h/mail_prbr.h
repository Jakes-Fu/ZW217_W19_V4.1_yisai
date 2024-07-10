/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_prbr.h
 *	Author		: sunguochen    
 *	Description	: ����/���ս���
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_PRBR_H_
#define _MAIL_PRBR_H_

#include "mail_type.h"




/*==========================================================
 * Function 	: initMailProgressInfo
 * Author		: sunguochen      
 * Description : ��ʼ��������Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
MailProgressInfo* initMailProgressInfo( void );



/*==========================================================
 * Function 	: calculateProgress
 * Author		: sunguochen      
 * Description : �������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
comm_boolean calculateProgress(MailProgressInfo* progress);



/*==========================================================
 * Function 	: adjustProgressTotalSize
 * Author		: sunguochen      
 * Description : �ܴ�С����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void adjustProgressTotalSize(MailProgressInfo* progress, comm_uint32 actualHeaderSize);



/*==========================================================
 * Function 	: destroyMailProgressInfo
 * Author		: sunguochen      
 * Description : ɾ��������Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void destroyMailProgressInfo(MailProgressInfo* progress);

#endif 
