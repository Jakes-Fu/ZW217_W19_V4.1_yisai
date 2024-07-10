/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_memory.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-1  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-1      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _COMM_MEMORY_H_
#define _COMM_MEMORY_H_

#include "comm_platform.h"

#ifdef MAIL_MEMORY_DEBUG_MODE

/*==========================================================
 * Function 	: mail_mem_link_init
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/20/2010 
 ==========================================================*/
void mail_mem_link_init(void);



/*==========================================================
 * Function 	: debug_comm_alloc
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void* debug_comm_alloc(comm_uint32 size, char* file_name, int lin_num);


#define comm_alloc(size )  debug_comm_alloc(size, __FILE__, __LINE__)

void comm_free_debug(void* ptr, char* file_name, int lin_num);

#define comm_free(ptr )  if(ptr){comm_free_debug((ptr), __FILE__, __LINE__); (ptr) = NULL;}

#else

/*==========================================================
 * Function 	: comm_alloc
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void* comm_alloc(comm_uint32 size);


/*==========================================================
 * Function 	: comm_free_debug
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_free_debug(void* ptr);

#define comm_free(ptr )  if(ptr){comm_free_debug(ptr); (ptr) = NULL;}
#endif



/*==========================================================
 * Function 	: comm_memory_unfree
 * Author		: sunguochen      
 * Description : ���Ժ���
                      �˳�ʱ����,���Դ�ӡ������δ�ͷ��ڴ���Ϣ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_memory_unfree(void);


#endif

