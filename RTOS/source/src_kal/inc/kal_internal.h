/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

*   Main include file to be included by the kal files.

*   Component-specific prefix : kal

*********************************************************************/
#ifndef _KAL_INTERNAL_H
#define _KAL_INTERNAL_H

/*****************
Include Statements
******************/

typedef enum
{
  KAL_FIXED_LEN_MSG_Q,
  KAL_VARIABLE_LEN_MSG_Q
}kal_qtype;

/* +cr184806 */ 
#if defined(DEBUG_KAL)
/*
In the debug version there is an overhead of KAL_MEM_HEADER_SIZE with
each memory location, if flag KAL_LOG_MEM_HISTORY is enabled
*/
/* +cr216218 */
#ifndef MEMORY_CUT
#define LOG_MEM_HISTORY
#endif
/* -cr216218 */
#endif
/* -cr184806 */ 

#if defined(RELEASE_KAL)
#include "kal_internal_release.h"
#endif

#if defined(DEBUG_KAL)
#include "kal_internal_debug.h"
#endif


/************************
Macros Without Parameters 
*************************/

#define KAL_INTERNAL_NILTASK_ID     (kal_internal_taskid)NULL
#define KAL_INTERNAL_NILTIMER_ID    (kal_internal_timerid)NULL
#define KAL_INTERNAL_NILPOOL_ID     (kal_internal_poolid)NULL
#define KAL_INTERNAL_NILMUTEX_ID    (kal_internal_mutexid)NULL
#define KAL_INTERNAL_NILSEM_ID      (kal_internal_semid)NULL
#define KAL_INTERNAL_NILQ_ID        (kal_internal_msgqid)NULL


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

/******************************
Declarations Of Exported Globals 
*******************************/

extern kal_uint16             kal_max_num_timers_g;
extern kal_uint16              kal_max_num_tasks_g;/* cr229061 */

#endif /* _KAL_INTERNAL_H */
