#ifndef __CTRLPAN_H__
#define __CTRLPAN_H__

#include "sci_api.h"
#include "sci_types.h"

#ifdef CTRLPAN_DLL_EXPORTS
#define CTRLPAN_DLL_API __declspec(dllexport)
#else
#define CTRLPAN_DLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
    extern   "C"
    {
#endif
//----------------------------------------------------------------------------
// contrl panel task ID
//----------------------------------------------------------------------------

#define CTRLPAN_TASK_ID 50-1	

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_StartControlPanel
// AUTHOR:          andy.bian
// RETURN:          TRUE if Control Panel is started successfully,else FALSE.
// DESCRIPTION:     This is a thread function ,it creates the control panel.
// ----------------------------------------------------------------------------
CTRLPAN_DLL_API BOOL CP_StartControlPanel();

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_EndControlPanel
// AUTHOR:          andy.bian
// DESCRIPTION:     Stop the control panel.
// ----------------------------------------------------------------------------
CTRLPAN_DLL_API void CP_EndControlPanel();

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_RegisterFunctions
// AUTHOR:          andy.bian
// DESCRIPTION:     Calling this function when the simulator starts.
// ----------------------------------------------------------------------------
typedef struct CP_FUNC_LIST_tag // Callback function list
{
    // RTOS functions
    void (*fpOS_ThreadContextSave)();
    void (*fpOS_ThreadContextRestore)();
	
    void (*fpOS_CreateSignal)(xSignalHeader *ppSig,
                              uint16         sigCode,
                              uint16         sigSize,
                              BLOCK_ID       sender);

    void (*fpOS_SendSignal)(xSignalHeader    pSig, 
                            BLOCK_ID         receiver);

    void (*fpOS_FreeSignal)(xSignalHeader    pSig);

    // Callback function for OS to send message to Control Panel
    void (*fp_RegisterCallback)(int (*fpCallback)(xSignalHeader psig));

} CP_FUNC_LIST;


CTRLPAN_DLL_API void CP_RegisterFuns(CP_FUNC_LIST *pFuncList);

#ifdef __cplusplus
	}
#endif

#endif //_CTRLPAN_H_