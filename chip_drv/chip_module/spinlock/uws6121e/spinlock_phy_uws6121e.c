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
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_spinlock.c
/// hwspinlock module driver implementation
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "deepsleep_drvapi.h"
#include "spinlock_phy_uws6121e.h"
#include "spinlock_reg_6121e.h"

// =============================================================================
//  MACROS
// =============================================================================
#define SPINLOCK_WRITE_KEY	(0x1)	/*processor specific write lock id */

#define SPINLOCK_NOTTAKEN_V1		(0x55aa10c5)	/*free: RLSL */

#define SPINLOCK_READL(addr)    (*(volatile uint32 *)(addr))
#define SPINLOCK_WRITEL(value,addr)   ((*(volatile uint32 *)(addr))=value)

#pragma arm section code = "SPINLOCK_AONRAM"	
//#define FILL_SPINLOCK(_X_)	do {a_spinlock_implemented[(_X_)] = 1;} while(0)
#define RECORD_SPINLOCK_STATUS_LOCK(_X_)	do {a_spinlock_status[(_X_)] = 1;} while(0)
#define RECORD_SPINLOCK_STATUS_UNLOCK(_X_)	do {a_spinlock_status[(_X_)] = 0;} while(0)
#pragma arm section code


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//unsigned char a_spinlock_implemented[SPINLOCK_MAX_ID];
unsigned int v_spinlock_vid;
#pragma arm section zidata = "ZIDATA_AONRAM"
unsigned char a_spinlock_status[HWSPINLOCK_MAX_ID];
#pragma arm section zidata

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// spinlock_addr
// -----------------------------------------------------------------------------
/// get the hw spinlock register address with the id
// =============================================================================
static void disable_irq_fiq(uint32 lock_id)
{
    if(HWSPINLOCK_CPSLEEP != lock_id)
    {
        SCI_DisableIRQ();
        SCI_DisableFIQ();
    }
}

static void enable_irq_fiq(uint32 lock_id)
{
    if(HWSPINLOCK_CPSLEEP != lock_id)
    {
        SCI_RestoreFIQ();
        SCI_RestoreIRQ();
    }
}

static unsigned long spinlock_addr(unsigned int id)
{
    return (REG_SPINLOCK_BASE + 0x800 + 0x4*id);
}

static int spinlock_fast_trylock(uint32 lock_id,uint32 to)
{
    unsigned long addr;
    uint32 v_curCnt;
    uint8 wait_count = 0;

    //get the current counter,
    v_curCnt = OS_GetTickCount();

    addr = spinlock_addr(lock_id);

    disable_irq_fiq(lock_id);

    do{
        // Try to take the hwspinlock
        if (!SPINLOCK_READL((void *)addr))
        {
            RECORD_SPINLOCK_STATUS_LOCK(lock_id);
            return 0;
        }

        //The lock is already taken, let's check if the user wants us to try again
        if(OS_GetTickCount() - v_curCnt > to)
        {
            enable_irq_fiq(lock_id);
            // get the spinlock fail
            return -1;
        }

        /* Rrespond to interrupt,if any */
        enable_irq_fiq(lock_id);

        for(wait_count = 0; wait_count <10; wait_count++)
        {
            ;
        }

        disable_irq_fiq(lock_id);
    }while(1);

}

static void spinlock_fast_unlock(uint32 lock_id)
{
    unsigned long addr;

    addr = spinlock_addr(lock_id);
    RECORD_SPINLOCK_STATUS_UNLOCK(lock_id);

    SPINLOCK_WRITEL(SPINLOCK_NOTTAKEN_V1, (void *)addr);
    enable_irq_fiq(lock_id);
}

// =============================================================================
// hwspin_lock_timeout - lock an hwspinlock with timeout limit
// -----------------------------------------------------------------------------
//lock_id: the hwspinlock id to be locked
//@to: timeout value in usecs
//Returns 0 when the @hwlock was successfully taken,
//Returns -1 when  the @hwlock is still busy after @to usecs
// =============================================================================

int hwspin_lock_timeout(uint32 lock_id, uint32 to)
{
    return spinlock_fast_trylock(lock_id,to);
}

// =============================================================================
// hwspin_unlock - unlock a specific hwspinlock
// -----------------------------------------------------------------------------
//lock_id: the hwspinlock id to be locked
// =============================================================================

void hwspin_unlock(uint32  lock_id)
{
    spinlock_fast_unlock(lock_id);
}

int hwspin_lock_get(uint32 lock_id)
{
    unsigned long addr;

    addr = spinlock_addr(lock_id);

    if(SCI_GetAssertFlag()){
        RECORD_SPINLOCK_STATUS_LOCK(lock_id);
        return 0;
    }

    disable_irq_fiq(lock_id);

    // Try to take the hwspinlock
    if (!SPINLOCK_READL((void *)addr))
    {
        RECORD_SPINLOCK_STATUS_LOCK(lock_id);
        return 0;
    }
    else
    {
        enable_irq_fiq(lock_id);

        return -1;
    }
}



