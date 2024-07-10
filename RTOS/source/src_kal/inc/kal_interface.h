/*********************************************************************

   (C) Spreadtrum Communications Co. Ltd, 
            All rights reserved.

  This file chooses DEBUG_KAL or RELEASE_KAL header file
  depending on the KAL version choosen.

  Component-specific prefix : kal

*********************************************************************/

#ifndef _KAL_INTERFACE_H
#define _KAL_INTERFACE_H

/*****************
Include Statements
******************/
/* Only one of DEBUG_KAL or RELEASE_KAL should be declared */
#if (defined( DEBUG_KAL) && defined( RELEASE_KAL ) )
#error "Only one of DEBUG_KAL and RELEASE_KAL can be defined"
#endif
 
#if (!defined( DEBUG_KAL) && !defined( RELEASE_KAL ))
#error "At least one of DEBUG_KAL or RELEASE_KAL should be defined"
#endif
                                             
#define _INCLS_FROM_KAL_INTERFACE_H

#include "kal_common.h"

#ifdef DEBUG_KAL
#include "kal_debug.h"
#else
#include "kal_release.h"
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

#endif /* _KAL_INTERFACE_H */
