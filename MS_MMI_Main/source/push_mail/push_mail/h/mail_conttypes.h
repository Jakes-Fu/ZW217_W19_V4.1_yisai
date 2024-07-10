/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_conttypes.h
 *	Author		: sunguochen    
 *	Description	: 队列操作函数接口
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef _MAIL_CONTTYPES_H_
#define _MAIL_CONTTYPES_H_


#include "mail_mem.h"



typedef struct MailListImp MailList;

typedef void mailList_freeData(void* data);


/*==========================================================
 * Function 	: mailList_create
 * Author		: sunguochen      
 * Description : 创建列表
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
MailList* mailList_create(comm_uint8 modId);



/*==========================================================
 * Function 	: mailList_destroy
 * Author		: sunguochen      
 * Description : 删除列表
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_destroy(MailList* list, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_isEmpty
 * Author		: sunguochen      
 * Description : 列表是否为空
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_boolean mailList_isEmpty(const MailList* list);



/*==========================================================
 * Function 	: mailList_getNumOfElements
 * Author		: sunguochen      
 * Description : 获取列表中的元素个数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
comm_uint16 mailList_getNumOfElements(const MailList* list);



/*==========================================================
 * Function 	: mailList_getElementAtPos
 * Author		: sunguochen      
 * Description : 获取指定位置的元素数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void* mailList_getElementAtPos(const MailList* list, comm_uint16 pos);



/*==========================================================
 * Function 	: mailList_getFirstElement
 * Author		: sunguochen      
 * Description : 获取列表中的第一个元素
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void* mailList_getFirstElement(const MailList* list);



/*==========================================================
 * Function 	: mailList_getLastElement
 * Author		: sunguochen      
 * Description : 获取列表的最后一个元素
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void* mailList_getLastElement(const MailList* list);



/*==========================================================
 * Function 	: mailList_insertAtPos
 * Author		: sunguochen      
 * Description : 在指定位置插入一个元素
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_insertAtPos(MailList* list, comm_uint16 pos, void* data);



/*==========================================================
 * Function 	: mailList_insertFirst
 * Author		: sunguochen      
 * Description : 出入第一个位置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_insertFirst(MailList* list, void* data);



/*==========================================================
 * Function 	: mailList_insertLast
 * Author		: sunguochen      
 * Description : 插入最后一个位置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_insertLast(MailList* list, void* data);



/*==========================================================
 * Function 	: mailList_removeAtPos
 * Author		: sunguochen      
 * Description : 移除列表指定位置的数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeAtPos(MailList* list, comm_uint16 pos, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_removeFirst
 * Author		: sunguochen      
 * Description : 移除第一个数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeFirst(MailList* list, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_removeLast
 * Author		: sunguochen      
 * Description : 移除最后一个数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeLast(MailList* list, mailList_freeData* freeFn);



/*==========================================================
 * Function 	: mailList_removeIfDataEquals
 * Author		: sunguochen      
 * Description : 移除指定数据的队列信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void mailList_removeIfDataEquals(MailList* list, const void* data, mailList_freeData* freeFn);




#endif 
