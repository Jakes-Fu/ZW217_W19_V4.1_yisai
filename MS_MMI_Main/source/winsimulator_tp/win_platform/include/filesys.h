#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "sci_api.h"

#ifdef FILESYS_EXPORTS_INIT
#define FILESYS_API __declspec(dllexport)
#else
#define FILESYS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


// ----------------------------------------------------------------------------
// contrl panel task ID
// ----------------------------------------------------------------------------
#define CTRLPAN_TASK_ID 50-1	 

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


// ----------------------------------------------------------------------------
// Init file system 
// ----------------------------------------------------------------------------
FILESYS_API void FileSys_Init();


#ifdef __cplusplus
	}
#endif // __cplusplus

#endif//__FILESYSTEM_H__