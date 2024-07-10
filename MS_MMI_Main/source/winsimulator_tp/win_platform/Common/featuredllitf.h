#ifndef __FEATUREDLLITF_H__
#define __FEATUREDLLITF_H__

#include "sci_api.h"
#include "sci_types.h"

#ifdef __cplusplus
extern   "C"
{
#endif	
	// ----------------------------------------------------------------------------
	// FUNCTION NAME:   CP_AddControlPage
	// AUTHOR:          apple.gao andy.bian
	// RETURN:          TRUE if Control Page is added successfully,else FALSE.
	// DESCRIPTION:     
	// ----------------------------------------------------------------------------
	BOOL CP_AddControlPage(  );
	
	// ----------------------------------------------------------------------------
	// FUNCTION NAME:   CP_CloseControlPage
	// AUTHOR:          apple gao andy.bian
	// DESCRIPTION:     Close the control page.
	// ----------------------------------------------------------------------------
	void CP_CloseControlPage();
	
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
	
	
	void CP_RegisterFuns(CP_FUNC_LIST *pFuncList);
	
#ifdef __cplusplus
}
#endif

#endif //__FEATUREDLLITF_H__