/******************************************************************************
 ** File Name:      msk_api_cp.cpp                                            *
 ** Author:         Liu Jun                                                   *
 ** Date:           25/02/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file implements the API functions declared in        *
 **                 msk_api_cp.h                                              *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 25/03/2003      Liu Jun     Create.                                       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "msk_api_cp.h"
#include "bl_os.h"

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/
extern void BL_CPSetCallbackFunToPS(int (*fun_ptr)(xSignalHeader sig));

/**---------------------------------------------------------------------------*
 **                         Function Implementations                          *
 **---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Function:    MSK_CreateSignal
// Author:      Liu Jun
// Purpose:     Create a signal
// Calls:       SCI_CREATE_SIGNAL
// Called by:   The MS Control Panel
// Remarks:     It will allocate memory for 'sig', so do remember to call
//              MSK_FreeSignal() to free this memory when you don't need this
//              signal.
//-----------------------------------------------------------------------------
MSKERNEL_API_CP void 
MSK_CreateSignal(xSignalHeader *sig,    // signal pointer
                 uint16         code,   // signal code
                 uint16         size,   // signal size
                 BLOCK_ID       sender) // sender ID
{
	// @Xueliang.Wang; (2004-12-10)
	//SCI_CREATE_SIGNAL(*sig, code, size, sender);
	*sig = (xSignalHeader)SCI_ALLOC(size);
	SCI_ASSERT((*sig) != SCI_NULL); 
	SCI_MEMSET((void*)(*sig), 0 , size); 
	(*sig)->SignalSize = size; 
	(*sig)->SignalCode = code; 
	(*sig)->Sender     = sender;
}

//-----------------------------------------------------------------------------
// Function:    MSK_SendSignal
// Author:      Liu Jun
// Purpose:     Send a signal to a specified receiver
// Calls:       SCI_SEND_SIGNAL
// Called by:   The MS Control Panel
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_CP void
MSK_SendSignal(xSignalHeader sig,      // signal pointer
               BLOCK_ID      receiver) // receiver ID
{
    // @Xueliang.Wang; (2004-12-10)
	//SCI_SEND_SIGNAL(sig, receiver);
	SCI_SendSignal(sig, receiver);
}

//-----------------------------------------------------------------------------
// Function:    MSK_FreeSignal
// Author:      Liu Jun
// Purpose:     Free a signal
// Calls:       SCI_FREE_SIGNAL
// Called by:   The MS Control Panel
// Remarks:     See MSK_CreateSignal()
//-----------------------------------------------------------------------------
MSKERNEL_API_CP void
MSK_FreeSignal(xSignalHeader sig) // signal pointer
{
    // @Xueliang.Wang; (2004-12-10)
	//SCI_FREE_SIGNAL(sig);
	SCI_FREE(sig);
}

//-----------------------------------------------------------------------------
// Function:    MSK_CPSetCallbackFunToPS
// Author:      Liu Jun
// Purpose:     Set the callback function to PS so that PS can send messages to
//              the MS Control Panel
// Calls:       BL_CPSetCallbackFunToPS
// Called by:   The MS Control Panel
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_CP void
MSK_CPSetCallbackFunToPS(int (*fun_ptr)(xSignalHeader sig)) // callback function
{
    BL_CPSetCallbackFunToPS(fun_ptr);
}
