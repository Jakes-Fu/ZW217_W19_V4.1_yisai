// CtrlPan_DLL.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "FeatureDll.h"

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

// Pointer to Control Panel main wnd.
CWnd * gs_pWndConPan = NULL;

//functions list
CP_FUNC_LIST gs_FuncList = { NULL };



/**---------------------------------------------------------------------------*
 **                         Exported Functions                                *
 **---------------------------------------------------------------------------*/
BOOL AddControlPage( DWORD dwPageID )
{
	HWND handle=::FindWindow(NULL,_T("TD-SCDMA Control Panel"));
	if (NULL == handle)
	{
		handle=::FindWindow(NULL,_T("Control Panel"));
	}
	
    if (NULL!=handle)
    {
		CPropertySheet objSheet;
		objSheet.Attach(handle);
		objSheet.AddPage((CPropertyPage*)gs_pWndConPan);
		objSheet.SetActivePage((CPropertyPage*)gs_pWndConPan);
		objSheet.Detach();
		
    }
	else
	{
		//if not find control panel window ,create file system dialog only
		((CDialog*)gs_pWndConPan)->Create(dwPageID);
		gs_pWndConPan->CenterWindow();
		gs_pWndConPan->ShowWindow(SW_SHOW);
		gs_pWndConPan->UpdateWindow();
	}
	
	return TRUE;
}

void CloseControlPage()
{
	if (gs_pWndConPan == NULL)
	{
		return ;		
	}

	gs_pWndConPan->DestroyWindow();
	delete gs_pWndConPan;
	gs_pWndConPan = NULL;

	return ;
} 

void RegisterFuns(CP_FUNC_LIST *pFuncList)
{
    assert(pFuncList != NULL);

    // Get all the functions point
    memcpy(&gs_FuncList, pFuncList, sizeof(CP_FUNC_LIST));
}



void InnerThreadContextSave()
{
   ASSERT(gs_FuncList.fpOS_ThreadContextSave != NULL);
   gs_FuncList.fpOS_ThreadContextSave();
}

void InnerThreadContextRestore()
{
   ASSERT(gs_FuncList.fpOS_ThreadContextRestore != NULL);
   gs_FuncList.fpOS_ThreadContextRestore();
}

void InnerSCICreateSignal(xSignalHeader * ppSig,
						uint16 nSigCode,
						uint16 nSigSize,
						BLOCK_ID nSender)
{
   ASSERT(gs_FuncList.fpOS_CreateSignal != NULL);
   gs_FuncList.fpOS_CreateSignal(ppSig, nSigCode, nSigSize, nSender);
}

void InnerSCISendSignal(xSignalHeader pSig,	 BLOCK_ID nReceiver)									   
{
	ASSERT(gs_FuncList.fpOS_SendSignal != NULL);
    gs_FuncList.fpOS_SendSignal(pSig, nReceiver);
}

void InnerSCIFreeSignal(xSignalHeader pSig)
{
	ASSERT(gs_FuncList.fpOS_FreeSignal != NULL);
    gs_FuncList.fpOS_FreeSignal(pSig);
}


int SendMsgToCtrlPanTask(xSignalHeader psig, UINT uiTaskid )
{	
    InnerThreadContextSave();
    InnerSCISendSignal(psig, uiTaskid);		
    InnerThreadContextRestore();

    return SCI_SUCCESS;
}