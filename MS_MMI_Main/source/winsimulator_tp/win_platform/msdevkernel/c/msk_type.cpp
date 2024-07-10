/******************************************************************************
 ** File Name:      msk_type.cpp                                              *
 ** Author:         Liu Jun                                                   *
 ** Date:           24/09/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file implements the API functions declared in        *
 **                 msk_type.h                                                *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 24/09/2003      Liu Jun     Create.                                       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "msk_type.h"
#include <bl_os.h>

/**---------------------------------------------------------------------------*
 **                         Function Implementations                          *
 **---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Function:    MSK_ThreadContextSave
// Author:      Liu Jun
// Purpose:     Save the context of current thread
// Calls:       SCI_THREAD_CONTEXT_SAVE
// Called by:   The MS Control Panel
// Remarks:     1. Use with MSK_ThreadContextRestore() in pairs
//              2. Call MSK_ThreadContextSave() before invoking RTOS functions
//              3. Call MSK_ThreadContextRestore() after invoking RTOS functions
//-----------------------------------------------------------------------------
MSKERNEL_API void 
MSK_ThreadContextSave()
{
    SCI_THREAD_CONTEXT_SAVE;
}

//-----------------------------------------------------------------------------
// Function:    MSK_ThreadContextRestore
// Author:      Liu Jun
// Purpose:     Restore the context of current thread
// Calls:       SCI_THREAD_CONTEXT_RESTORE
// Called by:   The MS Control Panel
// Remarks:     1. Use with in MSK_ThreadContextRestore pairs
//              2. Call MSK_ThreadContextSave() before invoking RTOS functions
//              3. Call MSK_ThreadContextRestore() after invoking RTOS functions
//-----------------------------------------------------------------------------
MSKERNEL_API void 
MSK_ThreadContextRestore()
{
    SCI_THREAD_CONTEXT_RESTORE;
}