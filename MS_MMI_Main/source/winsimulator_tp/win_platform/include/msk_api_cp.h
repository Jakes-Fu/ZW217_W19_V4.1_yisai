/******************************************************************************
 ** File Name:      msk_api_cp.h                                              *
 ** Author:         Liu Jun                                                   *
 ** Date:           25/02/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file defines the API functions which will be  *
 **                 called by the MS Contron Panel.                           *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 25/03/2003      Liu Jun     Create.                                       *
 ******************************************************************************/

#ifndef __MSK_API_CP_H__
#define __MSK_API_CP_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**---------------------------------------------------------------------------*
 **                         DLL Import/Export Macro                           *
 **---------------------------------------------------------------------------*/
#ifdef MSKERNEL_EXPORTS_CP
#define MSKERNEL_API_CP __declspec(dllexport)
#else
#define MSKERNEL_API_CP __declspec(dllimport)
#endif

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
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
MSK_CreateSignal(xSignalHeader *sig,     // signal pointer
                 uint16         code,    // signal code
                 uint16         size,    // signal size
                 BLOCK_ID       sender); // sender ID

//-----------------------------------------------------------------------------
// Function:    MSK_SendSignal
// Author:      Liu Jun
// Purpose:     Send a signal to a specified receiver
// Calls:       SCI_SEND_SIGNAL
// Called by:   The MS Control Panel
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_CP void
MSK_SendSignal(xSignalHeader sig,       // signal pointer
               BLOCK_ID      receiver); // receiver ID

//-----------------------------------------------------------------------------
// Function:    MSK_FreeSignal
// Author:      Liu Jun
// Purpose:     Free a signal
// Calls:       SCI_FREE_SIGNAL
// Called by:   The MS Control Panel
// Remarks:     See MSK_CreateSignal()
//-----------------------------------------------------------------------------
MSKERNEL_API_CP void
MSK_FreeSignal(xSignalHeader sig); // signal pointer

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
MSK_CPSetCallbackFunToPS(int (*fun_ptr)(xSignalHeader sig)); // callback function

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MSK_API_CP_H__