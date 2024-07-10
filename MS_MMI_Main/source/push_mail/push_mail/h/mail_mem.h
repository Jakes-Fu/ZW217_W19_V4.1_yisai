/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_mem.h
 *	Author		: sunguochen    
 *	Description	: 手机邮箱内部接口
 *	Vesion		: 1.0        
 *	Date			: 2010-4-14  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-14      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_MEM_H_
#define _MAIL_MEM_H_


#include "mail_type.h"
#include "comm_platform.h"

#include "comm_interface.h"


#define MAIL_ASSERT(b) COMM_ASSERT(b) 



#ifdef MAIL_MEMORY_DEBUG_MODE

#define E_ALLOC(size)                         debug_comm_alloc(size, __FILE__, __LINE__)

#define E_ALLOCMOD(modid,size)        debug_comm_alloc(size, __FILE__, __LINE__)

#define E_CALLOC(size)                       debug_comm_alloc(size, __FILE__, __LINE__)

#define E_ALLOCTYPE(t)                      (t *)debug_comm_alloc((sizeof(t)), __FILE__, __LINE__) 

#define E_ALLOCTYPEMOD(m,t)           (t *)debug_comm_alloc((sizeof(t)), __FILE__, __LINE__) 


#define E_FREE(p)                               (comm_free_debug((p), __FILE__, __LINE__), (p)=NULL)

#define E_FREEMOD(modid,p)              (comm_free_debug((p), __FILE__, __LINE__), (p)=NULL)

#else

#define E_ALLOC(size)                         comm_alloc(size)

#define E_ALLOCMOD(modid,size)        comm_alloc(size)


#define E_CALLOC(size)                       comm_alloc(size)

#define E_ALLOCTYPE(t)                      (t *)comm_alloc (sizeof(t))

#define E_ALLOCTYPEMOD(m,t)           (t *)comm_alloc (sizeof(t))


#define E_FREE(p)                               (comm_free((p)), (p)=NULL)

#define E_FREEMOD(modid,p)              (comm_free((p)), (p)=NULL)

#endif




/*==========================================================
 * Function 	: mailStrdup
 * Author		: sunguochen      
 * Description : 字符串复制
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
char *mailStrdup( const char *str, const char *s, int l );



/*==========================================================
 * Function 	: mailStrndup
 * Author		: sunguochen      
 * Description : 字符串拷贝-小写 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
char *mailStrndup( const char *str, int maxlen, const char *s, int l );



/*==========================================================
 * Function 	: mail_strdup2
 * Author		: sunguochen      
 * Description : 字符串复制连接
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
char *mail_strdup2( const char *s1, const char *s2 );



/*==========================================================
 * Function 	: mail_strdup3
 * Author		: sunguochen      
 * Description : 字符串复制连接
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
char *mail_strdup3( const char *s1, const char *s2, const char *s3 );



/*==========================================================
 * Function 	: mail_memcat
 * Author		: sunguochen      
 * Description : 内存连接函数
                      返回地址需要释放
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void *mail_memcat( void *buf1, int len1, void *buf2, int len2 );



/*==========================================================
 * Function 	: mail_strnlen
 * Author		: sunguochen      
 * Description : 获得字符串长度
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
int mail_strnlen( const char *s, int maxlen );



/*==========================================================
 * Function 	: mail_strncpy_UTF8
 * Author		: sunguochen      
 * Description : 拷贝 UTF8 格式字符串
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
char *mail_strncpy_UTF8( char *dst, const char *src, int maxlen );



/*==========================================================
 * Function 	: mail_linlen
 * Author		: sunguochen      
 * Description : 查找字符串指定长度内回车换行位置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
int mail_linlen( const char *p, int length );



/*==========================================================
 * Function 	: mail_realloc
 * Author		: sunguochen      
 * Description : 重新分配内存
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void *mail_realloc( void *oldp, int oldsize, int newsize );




#define mail_strdup(s)                mailStrdup((s),__FILE__,__LINE__)
#define mail_strndup(s,maxlen)  mailStrndup((s),(maxlen),__FILE__,__LINE__)


#define mail_memdup(p,plen)    ( p ? memcpy(E_ALLOC(plen),p,plen) : p )


#define MAIL_NULLEMPTY(s)    ( (s) ? (s) : "" )


#define MAIL_STRALEN(chararray)  mail_strnlen((chararray), sizeof(chararray))


#define MAIL_STRLEN(s)   (sizeof(s)-1)


#define MAIL_STRDUP(s)  strcpy( E_ALLOC(sizeof(s)), (s) )



#endif 
