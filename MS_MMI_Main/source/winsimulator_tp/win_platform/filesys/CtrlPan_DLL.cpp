// CtrlPan_DLL.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "resource.h"
#include "CtrlPan_DLL.h"
#include "FileSysDlag.h"
#include "CtrlPanFuns.h"

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
// HANDLE of event control panel has been started.
HANDLE gs_hConPanStarted = (HANDLE) NULL;

// HANDLE of control panel thread.
HANDLE gs_hCPThread = (HANDLE) NULL;

// Pointer to Control Panel main wnd.
CWnd * gs_pWndConPan = NULL;

//functions list
CP_FUNC_LIST gs_FuncList = { NULL };

/**---------------------------------------------------------------------------*
 **                         Thread Function Declarations                       *
 **---------------------------------------------------------------------------*/
DWORD WINAPI ControlPanelThreadFunc(LPVOID lpParameter);


/**---------------------------------------------------------------------------*
 **                         Exported Functions                                *
 **---------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_StartControlPanel
// RETURN:          TRUE if Control Panel is started successfully,else false.
// DESCRIPTION:     this is a thread function ,it creates control panel,
//                  do message loop, and destroy control panel when finished.
// ----------------------------------------------------------------------------
CTRLPAN_DLL_API BOOL CP_StartControlPanel()
{
    gs_hConPanStarted = CreateEvent(NULL,FALSE,FALSE,NULL);
	gs_hCPThread=CreateThread(NULL,
				 0,
				 ControlPanelThreadFunc,
				 (LPVOID) NULL,			
				 0,
				 0);

	if (gs_hCPThread == NULL)
	{
		TRACE(_T("StartCtrlPan: CreateThread Failed!"));
		return FALSE;
	}	

    if (WaitForSingleObject(gs_hConPanStarted, 30*1000) == WAIT_TIMEOUT)
    {
	    TRACE(_T("StartCtrlPan: CreateThread Failed!"));
		return FALSE;
    }


	return TRUE;
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_EndControlPanel
// DESCRIPTION:     Stop the control panel.
// ----------------------------------------------------------------------------
CTRLPAN_DLL_API void CP_EndControlPanel()
{
	if (gs_pWndConPan == NULL)
	{
		return ;		
	}
	
	PostMessage(gs_pWndConPan->GetSafeHwnd(), WM_QUIT, 0, 0);

	//waite 100 Milliseconds
	if (WaitForSingleObject(gs_hCPThread, 100) == WAIT_TIMEOUT)
	{
		TerminateThread(gs_hCPThread, 0);
	}

	return ;
} 

CTRLPAN_DLL_API void CP_RegisterFuns(CP_FUNC_LIST *pFuncList)
{
    assert(pFuncList != NULL);

    // Get all the functions point
    memcpy(&gs_FuncList, pFuncList, sizeof(CP_FUNC_LIST));
}


// ----------------------------------------------------------------------------
// FUNCTION NAME:   ControlPanelThreadFunc
// RETURN:          thread function return value.
// DESCRIPTION:     this is a thread function ,it creates control panel,
//                  do message loop, and destroy control panel when finished.
// ----------------------------------------------------------------------------
DWORD WINAPI ControlPanelThreadFunc(LPVOID lpParameter)
{
    // sleep a few time, since rtos threads may not really started.
    Sleep(0);
	//find control panel window 
	HWND handle=::FindWindow(NULL,_T("TD-SCDMA Control Panel"));
	if (NULL == handle)
	{
		handle=::FindWindow(NULL,_T("Control Panel"));
	}
	
    if (NULL!=handle)
    {
		//add files system page to control panel
		gs_pWndConPan = new CFileSysDlg();
		CPropertySheet objSheet;
		objSheet.Attach(handle);
		objSheet.AddPage((CPropertyPage*)gs_pWndConPan);
		objSheet.SetActivePage((CPropertyPage*)gs_pWndConPan);
		objSheet.Detach();

    }
	else
	{
		//if not find control panel window ,create file system dialog only
		gs_pWndConPan = new CFileSysDlg();
		((CDialog*)gs_pWndConPan)->Create(IDD_CTRLPAN_DLG);
		gs_pWndConPan->CenterWindow();
		gs_pWndConPan->ShowWindow(SW_SHOW);
		gs_pWndConPan->UpdateWindow();
	}
	SetEvent(gs_hConPanStarted);

	//msg loop begine
	MSG msg ; 
	while (GetMessage(&msg, NULL, 0, 0)) 
	{        
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}

	if (gs_pWndConPan != NULL)
	{
		gs_pWndConPan->DestroyWindow();
		delete gs_pWndConPan;
		gs_pWndConPan = NULL;
	}
	return 0;
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


int SendMsgToCtrlPanTask(xSignalHeader psig)
{	
    InnerThreadContextSave();
    InnerSCISendSignal(psig, CTRLPAN_TASK_ID);		
    InnerThreadContextRestore();

    return SCI_SUCCESS;
}