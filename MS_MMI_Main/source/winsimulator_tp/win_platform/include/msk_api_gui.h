/******************************************************************************
 ** File Name:      msk_api_gui.h                                             *
 ** Author:         Liu Jun                                                   *
 ** Date:           25/02/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file defines the API functions which will be  *
 **                 called by the MSSim GUI module.                           *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 25/03/2003      Liu Jun     Create.                                       *
 ******************************************************************************/

#ifndef __MSK_API_GUI_H__
#define __MSK_API_GUI_H__

/**---------------------------------------------------------------------------*
 **                         DLL Import/Export Macro                           *
 **---------------------------------------------------------------------------*/
#ifdef MSKERNEL_EXPORTS_GUI
#define MSKERNEL_API_GUI __declspec(dllexport)
#else
#define MSKERNEL_API_GUI __declspec(dllimport)
#endif

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "msk_type.h"

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Function:    MSK_RegisterMSSimFunList
// Author:      Liu Jun
// Purpose:     Register the functions defined in the MSSim GUI module
// Calls:       BL_RegisterMSSimFunList()
// Called by:   The MSSim GUI module
// Remarks:     1. MSK_MSSIM_FUN_LIST is defined in "msk_type.h"
//              2. Call this function when the MSSim starts up, thus, MSKernel
//                 can call the GUI functions via 'pFunList'.
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_RegisterMSSimFunList(const MSK_MSSIM_FUN_LIST *pFunList); // function pointers

//-----------------------------------------------------------------------------
// Function:    MSK_StartRTOS
// Author:      Liu Jun
// Purpose:     Start the real-time OS
// Calls:       BL_StartRTOS()
// Called by:   The MSSim GUI module
// Remarks:     This function must be invoked in an individual thread
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_StartRTOS();

//-----------------------------------------------------------------------------
// Function:    MSK_StopRTOS
// Author:      Liu Jun
// Purpose:     Stop the real-time OS
// Calls:       BL_StopRTOS()
// Called by:   The MSSim GUI module
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_StopRTOS();

//-----------------------------------------------------------------------------
// Function:    MSK_IsPowerOff
// Author:      Liu Jun
// Purpose:     Judge the MS whether it's powered on or off
// Calls:       BL_IsPowerOff()
// Called by:   The MSSim GUI module
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI BOOL // TRUE indicates powered off
MSK_IsPowerOff();

//-----------------------------------------------------------------------------
// Function:    MSK_GetKeyCodeByName
// Author:      Liu Jun
// Purpose:     Get the virtual key code of a specified key
// Calls:       BL_GetKeyCodeByName()
// Called by:   The MSSim GUI module
// Remarks:     1. Key names are defined in the configuration file, MSSim.ini
//              2. Specify the key via it's name, such as '1', '*', ...
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI int                            // virtual key code
MSK_GetKeyCodeByName(const char * lpszKeyName); // key name in string format

//-----------------------------------------------------------------------------
// Function:    MSK_SendKeycodeToRTOS
// Author:      Liu Jun
// Purpose:     Send the virtual key code to the real-time OS
// Calls:       BL_SendKeycodeToRTOS()
// Called by:   The MSSim GUI module
// Remarks:     Call this function when a key is pressed down/up
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void
MSK_SendKeycodeToRTOS(WORD wKeyCode, // virtual key code
                      UINT state);   // key state: KPD_DOWN or KPD_UP

//-----------------------------------------------------------------------------
// Function:    MSK_SendTPcodeToRTOS
// Author:      Beijing Team
// Purpose:     Send the virtual touch panel key code to the real-time OS
// Calls:       BL_SendTPcodeToRTOS()
// Called by:   The MSSim GUI module
// Remarks:     
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void
MSK_SendTPcodeToRTOS(WORD xKeyCode,WORD yKeyCode, // virtual key code
                      UINT state);   // key state: KPD_DOWN or KPD_UP

//-----------------------------------------------------------------------------
// Function:    MSK_SendMsgToRTOS
// Author:      Liu Jun
// Purpose:     Send a message to the real-time OS
// Calls:       BL_SendMsgToRTOS()
// Called by:   The MSSim GUI module
// Remarks:     Call this function when a key is pressed down/up
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI UINT             // sending results: SCI_SUCCESS indicates success
MSK_SendMsgToRTOS(BYTE *pBuffer,  // message body
                  int   nLength); // message size

//-----------------------------------------------------------------------------
// Function:    MSK_Free
// Author:      Liu Jun
// Purpose:     Free the spcified memory block
// Calls:       BL_Free()
// Called by:   The MSSim GUI module
// Remarks:     Call this function to free a memory block which is allocated in
//              the MSKernel module.
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_Free(void *pBuffer); // memory block to be freed

//-----------------------------------------------------------------------------
// Function:    MSK_GetmessagefromMSDev
// Author:      Liu Jun
// Purpose:     Get resource file path
// Remarks:     Call this function to Get resource file path
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI char* 
MSK_GetResourceFile(); // Get resource file path

#endif // __MSK_API_GUI_H__