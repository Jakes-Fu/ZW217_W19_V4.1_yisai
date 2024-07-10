#include "FeatureExp.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         GOUBLE DEFINITION                                *
 **--------------------------------------------------------------------------*/
//file system dll module handle
#include "LoadLibrary.h"
extern "C" DLL_INFO_S g_dll_info[FEATURE_COUNT];

extern void		FFS_Task(uint32 argc, void * argv);
extern void		MSK_ThreadContextSave();
extern void		MSK_ThreadContextRestore();
extern void     MSK_CreateSignal(xSignalHeader *sig,uint16 code,uint16 size, BLOCK_ID sender);      
extern void     MSK_SendSignal(xSignalHeader sig,  BLOCK_ID receiver);
extern void		MSK_FreeSignal(xSignalHeader sig);                 

//define the callback function prototype
typedef int  (* CTRLPAN_CALLBACK)	(xSignalHeader psig);
typedef void (* CP_RegisterFunsType)(CP_FUNC_LIST *pFuncList)  ;

CTRLPAN_CALLBACK	g_pf_ctrlpan_callback = NULL;     
CP_RegisterFunsType pfn_registerFuns = NULL;

/**---------------------------------------------------------------------------*
 **                         Function definition                               *
 **---------------------------------------------------------------------------*/

void CP_RegisterCallback(CTRLPAN_CALLBACK fpFunc)
{
    assert(fpFunc != NULL);
    // Get the callback functions
    g_pf_ctrlpan_callback = fpFunc;
}

void CtrlPan_Task(uint32 /*argc*/, void * /*argv*/)
{	
	// to receive the signal 
    xSignalHeaderRec   *pSig = PNULL;           	
	while (1)
	{
        //receive a signal
		SCI_RECEIVE_SIGNAL (pSig, CTRLPAN_TASK_ID);
		SCI_ASSERT(pSig!=PNULL);
		
		if (g_pf_ctrlpan_callback!=NULL)
		{
			g_pf_ctrlpan_callback(pSig);
		}
	
		if (pSig != NULL)
		{
			SCI_FREE_SIGNAL(pSig);
		}	
	}		
}


/*****************************************************************************/
// 	Description : Init file system
//  Author:andy.bian
//	Note: 
/*****************************************************************************/
FEATURE_API void Feature_Init( int nFeatureIndex )
{		

	CP_FUNC_LIST FuncList;
	FuncList.fpOS_ThreadContextSave		=MSK_ThreadContextSave;
	FuncList.fpOS_ThreadContextRestore	=MSK_ThreadContextRestore;
	FuncList.fpOS_CreateSignal			=MSK_CreateSignal;
	FuncList.fpOS_SendSignal			=MSK_SendSignal;
	FuncList.fpOS_FreeSignal			=MSK_FreeSignal;
	FuncList.fp_RegisterCallback		=CP_RegisterCallback;
	
	//register callback functions
	pfn_registerFuns= (CP_RegisterFunsType)GetProcAddress(g_dll_info[nFeatureIndex].hDll ,"CP_RegisterFuns");
	(*pfn_registerFuns)(&FuncList );
	
}

#ifdef __cplusplus
	}
#endif