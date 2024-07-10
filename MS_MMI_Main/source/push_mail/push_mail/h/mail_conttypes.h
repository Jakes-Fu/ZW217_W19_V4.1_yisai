/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_conttypes.h
 *	Author		: sunguochen    
 *	Description	: ���в��������ӿ�
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef _MAIL_CONTTYPES_H_
#define _MAIL_CONTTYPES_H_


#include "mail_mem.h"



typedef struct MailListImp MailList;

typedef void mailList_freeData(void* data);


/*==========================================================
 * Function 	: mailList_create
 * Author		: sunguochen      
 * Description : �����б�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
MailList* mailList_create(comm_uint8 modId);



/*==========================================================
 * Function 	: mailList_destroy
 * Author		: sunguochen      
 * Description : ɾ���б�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_destroy(MailList* list, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_isEmpty
 * Author		: sunguochen      
 * Description : �б��Ƿ�Ϊ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_boolean mailList_isEmpty(const MailList* list);



/*==========================================================
 * Function 	: mailList_getNumOfElements
 * Author		: sunguochen      
 * Description : ��ȡ�б��е�Ԫ�ظ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_uint16 mailList_getNumOfElements(const MailList* list);



/*==========================================================
 * Function 	: mailList_getElementAtPos
 * Author		: sunguochen      
 * Description : ��ȡָ��λ�õ�Ԫ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void* mailList_getElementAtPos(const MailList* list, comm_uint16 pos);



/*==========================================================
 * Function 	: mailList_getFirstElement
 * Author		: sunguochen      
 * Description : ��ȡ�б��еĵ�һ��Ԫ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void* mailList_getFirstElement(const MailList* list);



/*==========================================================
 * Function 	: mailList_getLastElement
 * Author		: sunguochen      
 * Description : ��ȡ�б�����һ��Ԫ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void* mailList_getLastElement(const MailList* list);



/*==========================================================
 * Function 	: mailList_insertAtPos
 * Author		: sunguochen      
 * Description : ��ָ��λ�ò���һ��Ԫ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_insertAtPos(MailList* list, comm_uint16 pos, void* data);



/*==========================================================
 * Function 	: mailList_insertFirst
 * Author		: sunguochen      
 * Description : �����һ��λ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_insertFirst(MailList* list, void* data);



/*==========================================================
 * Function 	: mailList_insertLast
 * Author		: sunguochen      
 * Description : �������һ��λ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_insertLast(MailList* list, void* data);



/*==========================================================
 * Function 	: mailList_removeAtPos
 * Author		: sunguochen      
 * Description : �Ƴ��б�ָ��λ�õ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeAtPos(MailList* list, comm_uint16 pos, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_removeFirst
 * Author		: sunguochen      
 * Description : �Ƴ���һ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeFirst(MailList* list, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_removeLast
 * Author		: sunguochen      
 * Description : �Ƴ����һ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeLast(MailList* list, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_removeIfDataEquals
 * Author		: sunguochen      
 * Description : �Ƴ�ָ�����ݵĶ�����Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeIfDataEquals(MailList* list, const void* data, mailList_freeData* freeFn);




#endif 
