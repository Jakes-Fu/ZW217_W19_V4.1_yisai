#ifndef __FEATUREDLL_H__
#define __FEATUREDLL_H__

#include "FeatureDllItf.h"

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
	BOOL AddControlPage( DWORD dwPageID  );
	
	// ----------------------------------------------------------------------------
	// FUNCTION NAME:   CP_CloseControlPage
	// AUTHOR:          apple gao andy.bian
	// DESCRIPTION:     Close the control page.
	// ----------------------------------------------------------------------------
	void CloseControlPage();	
	
	
	void RegisterFuns(CP_FUNC_LIST *pFuncList);
	
	
	void InnerThreadContextSave();

	void InnerThreadContextRestore();

	void InnerSCICreateSignal(xSignalHeader * ppSig,
						  uint16 nSigCode,
						  uint16 nSigSize,
						  BLOCK_ID nSender
						  );

	void InnerSCISendSignal(xSignalHeader pSig,					
					    BLOCK_ID nReceiver
						);

	void InnerSCIFreeSignal(xSignalHeader pSig);

	int SendMsgToCtrlPanTask(xSignalHeader psig, UINT uiTaskid);

	
#ifdef __cplusplus
}
#endif

#endif //__FEATUREDLL_H__