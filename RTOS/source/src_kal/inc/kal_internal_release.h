/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

   This file includes the nescessary files for release version of KAL.
   It gets included through kal_internal.h

   Component-specific prefix : kal
*********************************************************************/

#ifndef _KAL_INTERNAL_RELEASE_H
#define _KAL_INTERNAL_RELEASE_H

/*****************
Include Statements
******************/
/* Depending upon the OS include the nescessary files for
 * release version of KAL
 */
#if defined(KAL_ON_RTT)
#include "kal_interface.h"
#include "kal_rtt.h"
/* include "kal_rtt_common_defs.h" */
#include "kal_common_defs.h"
#endif

#if defined(KAL_ON_NUCLEUS)
#include "kal_interface.h"
#include "kal_nucleus.h"
#include "kal_common_defs.h"
#endif

/* @rick.zhang 2005-06-29 */
#if defined(KAL_ON_THREADX)
#include "kal_interface.h"
#include "kal_threadx.h"
#include "kal_common_defs.h"
#endif


/* @apple.gao 2007-04-12 */
#if defined(KAL_ON_WIN32)
#include "kal_interface.h"
#include "kal_debug_win.h"
#include "kal_debug_common_defs.h"
#include "kal_common_defs.h"
#endif

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

/******************************
Declarations Of Exported Globals 
*******************************/

#endif /* _KAL_INTERNAL_RELEASE_H */
