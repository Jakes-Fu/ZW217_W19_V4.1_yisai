/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_debug.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _COMM_DEBUG_H_
#define _COMM_DEBUG_H_


#include "os_api.h"
#include "comm_feature.h"


/*==========================================================
 * Function 	: mail_debug_init
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/26/2010 
 ==========================================================*/
void mail_debug_init(void);



/*==========================================================
 * Function 	: mail_debug_finial
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/26/2010 
 ==========================================================*/
void mail_debug_finial(void);



/*==========================================================
 * Function 	: comm_TraceLogType
 * Author		: sunguochen      
 * Description :  log写文件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_TraceLogType(char *in_format, ...);


/*==========================================================
 * Function 	: comm_TraceFile
 * Author		: sunguochen      
 * Description :  log写文件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_TraceFile(char *buf, int len);


#ifdef COMM_DEBUG_MODE

#define COMM_TRACE SCI_TRACE_LOW

#ifdef MAIL_DEBUG_SAVE_FILE
#define COMM_TRACE_FILE comm_TraceLogType
#else
#define COMM_TRACE_FILE SCI_TRACE_LOW
#endif

#else

#define COMM_TRACE 

#endif

#endif

