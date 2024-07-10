////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2017, RDA.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of RDA and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  RDA.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL: http://svn.rdamicro.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_debug.h $
//    $Author: tao.shi $
//    $Date: 2022-07-29 19:16:35 +0800 (Sat, 14 Jun 2014) $
//    $Revision: 23052 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file hal_spinlock.h
/// This file contains  hwspinlock functionalities API
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_SPINLOCK_H_
#define _HAL_SPINLOCK_H_

#include "sci_types.h"


// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================
#define HWSPINLOCK_ID_TOTAL_NUMS	(64)
typedef enum
{
  HWSPINLOCK_ADI = 0,
  HWSPINLOCK_APIFC, 	//	(1)
  HWSPINLOCK_AGPIO, 	//	(2)
  HWSPINLOCK_AEIC, 		//	(3)
  HWSPINLOCK_ADC, 		//	(4)
  HWSPINLOCK_CPSLEEP, 	//	(5)
  HWSPINLOCK_EFUSE=8, 	//	(8)
  HWSPINLOCK_IPC, 		// 	(9)
  HWSPINLOCK_WFI_26M,
  HWSPINLOCK_MAX_ID
}RDA_SPINLOCK_ID;


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================

// =============================================================================
// hwspin_lock_timeout - lock an hwspinlock with timeout limit
// -----------------------------------------------------------------------------
//lock_id: the hwspinlock id to be locked
//@to: timeout value in usecs
//Returns 0 when the @hwlock was successfully taken,
//Returns -1 when  the @hwlock is still busy after @to usecs
// =============================================================================
PUBLIC int hwspin_lock_timeout(uint32 lock_id, uint32 to);

// =============================================================================
// hwspin_unlock - unlock a specific hwspinlock
// -----------------------------------------------------------------------------
//lock_id: the hwspinlock id to be locked
// =============================================================================

PUBLIC void hwspin_unlock(uint32  lock_id);


#endif // _HAL_SPINLOCK_H_






