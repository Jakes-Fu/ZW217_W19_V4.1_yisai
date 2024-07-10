/*********************************************************************

   (C) Spreadtrum communications Co.  Ltd,
   All rights reserved.
  
   This file contains the extern declarations and macros for
   using the Inter Layer Message structure.

   Component-specific prefix : Support routines for Nucleus

*********************************************************************/

#ifndef _THREADX_LIBC_H
#define _THREADX_LIBC_H

/*****************
Include Statements
******************/

#include "kal_interface.h"
/* @rick.zhang 2005-07-08 */
//#include "nucleus.h"

/************************
Macros Without Parameters 
*************************/

/*****************
Enums Without Tags 
******************/

/***************
Type Definitions 
****************/

/********
Constants 
*********/

/*********************
Macros With Parameters 
**********************/

/***************************
Exported Function Prototypes 
****************************/

extern long htonl(long val);
extern long ntohl(long val);
extern int  atoi(const char *str);
/* @rick.zhang 2005-07-08 */
//extern void *realloc( kal_uint8 entity_id, kal_uint8 mem_type, 
//                      void *mem, unsigned int size);

/******************************
Declarations Of Exported Globals 
*******************************/

#endif /* _THREADX_LIBC_H */
