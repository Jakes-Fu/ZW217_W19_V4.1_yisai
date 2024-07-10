/******************************************************************************
 ** File Name:      ConPan.cpp                                                *
 ** Author:         Liu Jun                                                   *
 ** Date:           12/03/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file implements the functions which will be exported *
 **                 to the simulator, also it defines some internal functions.*
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 12/03/2003      Liu Jun     Create based on ConPan.cpp by Jianyong Gao    *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 
#include "stdafx.h"
#include "ConPanFunc.h"
#include "ConPan.h"
#include "tasks_id.h"
#include "ConPanPropertySheet.h"
#include "SIMConPadInt.h"

#ifdef CONPAN_INTERNAL_USE
#include "msk_type.h"
#include "msk_api_cp.h"
#endif // CONPAN_INTERNAL_USE


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

// HANDLE of event control panel has been started.
static HANDLE gs_hConPanStarted = (HANDLE) -1;
// HANDLE of control panel thread.
static HANDLE gs_hCPThread = (HANDLE) -1;
// Pointer to Control Panel PropertySheet.
static CConPanPropertySheet *gs_pwndConPan = NULL;

#ifndef CONPAN_INTERNAL_USE
// External function list
static CP_EXT_FUNC_LIST gs_ExtFuncList = { NULL };
#endif // CONPAN_INTERNAL_USE


/**---------------------------------------------------------------------------*
 **                         Inner Function Declarations                       *
 **---------------------------------------------------------------------------*/
 
DWORD WINAPI ControlPanelThreadFunc(LPVOID lpParameter);


/**---------------------------------------------------------------------------*
 **                         Exported Functions                                *
 **---------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_StartControlPanel
// AUTHOR:          Jianyong Gao
// INPUT:           CWnd pointer to parent 
// RETURN:          TRUE if Control Panel is started successfully,else false.
// DESCRIPTION:     this is a thread function ,it creates control panel,
//                  do message loop, and destroy control panel when finished.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT BOOL CP_StartControlPanel(int nX/* = -1 */, 
                       int nY/* = -1*/)
{

	BOOL bReturn = FALSE;

    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (nX < 0 || nX >= nScreenWidth)
    {
        nX = nScreenWidth / 2;
    }
    if (nY < 0 || nY >= nScreenHeight)
    {
        nY = nScreenHeight / 2;
    }
    CPoint *pptPos = new CPoint(nX, nY); 
    ASSERT(pptPos);

    gs_hConPanStarted = CreateEvent(NULL,FALSE,FALSE,NULL);
	gs_hCPThread=CreateThread(NULL,
				 0,
				 ControlPanelThreadFunc,
				 (LPVOID) pptPos,			
				 0,
				 0);
	if (gs_hCPThread == NULL)
	{
		TRACE("StartControlPanel: CreateThread Failed!");
		return FALSE;
	}	
    if (WaitForSingleObject(gs_hConPanStarted, 30*1000) == WAIT_TIMEOUT)
    {
	    TRACE("StartControlPanel: CreateThread Failed!");
		return FALSE;
    }
    bReturn = TRUE;
	return bReturn;
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_EndControlPanel
// AUTHOR:          Jianyong Gao
// INPUT:           None
// RETURN:          None
// DESCRIPTION:     Stop the control panel.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT void CP_EndControlPanel()
{
	if (gs_pwndConPan == NULL)
	{
		return ;		
	}
	PostMessage(gs_pwndConPan->GetSafeHwnd(), WM_QUIT, 0, 0);
	if (WaitForSingleObject(gs_hCPThread, 60*1000) == WAIT_TIMEOUT)
	{
		TerminateThread(gs_hCPThread, 0);
	}

	return ;
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_ShowControlPanel
// AUTHOR:          Hongliang Xin
// INPUT:           None
// RETURN:          None
// DESCRIPTION:     Show or hide the control panel.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT void CP_ShowControlPanel(BOOL bShow /*= TRUE*/)
{
	if (gs_pwndConPan != NULL)
	{
		if(bShow)
		{
			gs_pwndConPan->ShowWindow(SW_SHOW);	
		}
		else
		{
			gs_pwndConPan->ShowWindow(SW_HIDE);	
		}
	}
}

#ifndef CONPAN_INTERNAL_USE
// ----------------------------------------------------------------------------
// FUNCTION NAME:   CP_RegisterFunctions
// AUTHOR:          Liu Jun
// INPUT:           The external function pointers stored in a structure.
// RETURN:          None
// DESCRIPTION:     Calling this function when the simulator starts.
// ----------------------------------------------------------------------------
_CONPAN_DLLPORT void CP_RegisterFunctions(CP_EXT_FUNC_LIST *pFuncList)
{
    assert(pFuncList != NULL);

    // Get all the external functions
    memcpy(&gs_ExtFuncList, pFuncList, sizeof(CP_EXT_FUNC_LIST));

    // Register PS functions for SIM-Card
    // NOTES: fpSIMINT_QuerySIM is the start address of SIM-Card related
    //        functions in the structure.
    SIMCON_PAD_IMPORT_API_T simFuncList;
    memcpy(&simFuncList, &gs_ExtFuncList.fpSIMINT_QuerySIM, sizeof(SIMCON_PAD_IMPORT_API_T));
    SIM_CP_RegisterImportAPI(&simFuncList);
}
#endif // CONPAN_INTERNAL_USE


/**---------------------------------------------------------------------------*
 **                         Inner Functions                                   *
 **---------------------------------------------------------------------------*/

int SendCPMsgToPS(xSignalHeader psig)
{
    InnerThreadContextSave();
    InnerSCISendSignal(psig, P_MN);		
    InnerThreadContextRestore();

    return SCI_SUCCESS;
}
int SendCPMsgToPSDummy(xSignalHeader psig)
{
    InnerThreadContextSave();
    InnerSCISendSignal(psig, P_PSDummy);		
    InnerThreadContextRestore();

    return SCI_SUCCESS;
}

/**************************************************************************************/
// Description      : send control panel message to MMI for ATEST.
//--------------------------------------------------------------------------------------
// Input Parameter  : xSignalHeader psig  
// Output Parameter : NONE
// Return Value     : SCI_SUCCESS
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
int SendCPMsgToMMI(xSignalHeader psig)
{
    InnerThreadContextSave();
    InnerSCISendSignal(psig, P_APP);		
    InnerThreadContextRestore();
    
    return SCI_SUCCESS;
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   SendCPMsgToConPan
// AUTHOR:          Jianyong Gao
// INPUT:           psig, pointer to the message 
// RETURN:          int
// DESCRIPTION:     send message from protocal stack
//					to control panel(user interface)
// ----------------------------------------------------------------------------
int SendCPMsgToConPan(xSignalHeader psig)
{
	int nResult = -1;
	if (gs_pwndConPan == NULL||
		gs_pwndConPan->GetSafeHwnd() == NULL)
	{
		AfxMessageBox("the message will be discarded since no control panel.",					
					MB_ICONWARNING,
					0);			
	}
	else
	{
		if (PostMessage(gs_pwndConPan->GetSafeHwnd(), CONPAN_DATA, (WPARAM) psig, 0))
		{
			nResult = 0;
		}
	}
	return nResult;
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   ControlPanelThreadFunc
// AUTHOR:          Jianyong Gao
// INPUT:           CWnd pointer to parent 
// RETURN:          thread function return value.
// DESCRIPTION:     this is a thread function ,it creates control panel,
//                  do message loop, and destroy control panel when finished.
// ----------------------------------------------------------------------------
DWORD WINAPI ControlPanelThreadFunc(LPVOID lpParameter)
{
    InnerPSRegisterMsgHandler();

    CPoint *pptPosition = (CPoint *) lpParameter;
    ASSERT(pptPosition);

    // sleep a few time, since rtos threads may not really started.
    Sleep(0);
    
	gs_pwndConPan = new CConPanPropertySheet("Control Panel");			
    gs_pwndConPan->CreateConPan(NULL);
	
	CRect rectconpan,rectnew;
    gs_pwndConPan->GetWindowRect(&rectconpan);    
    rectnew.left     = pptPosition->x;
	rectnew.top      = pptPosition->y;
	rectnew.right    = rectnew.left+rectconpan.Width();
	rectnew.bottom   = rectnew.top+rectconpan.Height();		

    gs_pwndConPan->MoveWindow(&rectnew);
	gs_pwndConPan->ShowWindow(SW_HIDE);
	gs_pwndConPan->UpdateWindow();

    delete pptPosition;

	SetEvent(gs_hConPanStarted);

	MSG msg ; 
	while (GetMessage(&msg, NULL, 0, 0)) 
	{        
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}

	if (gs_pwndConPan != NULL)
	{
		gs_pwndConPan->ReleaseConPan();
		delete gs_pwndConPan;
		gs_pwndConPan = NULL;
	}

	return 0;
}

void InnerThreadContextSave()
{
#ifdef CONPAN_INTERNAL_USE
    MSK_ThreadContextSave();
#else
    gs_ExtFuncList.fpOS_ThreadContextSave();
#endif
}

void InnerThreadContextRestore()
{
#ifdef CONPAN_INTERNAL_USE
    MSK_ThreadContextRestore();
#else
    gs_ExtFuncList.fpOS_ThreadContextRestore();
#endif
}

void InnerSCICreateSignal(xSignalHeader * ppSig,
						uint16 nSigCode,
						uint16 nSigSize,
						BLOCK_ID nSender)
{
#ifdef CONPAN_INTERNAL_USE
    MSK_CreateSignal(ppSig, nSigCode, nSigSize, nSender);
#else
    gs_ExtFuncList.fpOS_CreateSignal(ppSig, nSigCode, nSigSize, nSender);
#endif
}

void InnerSCISendSignal(xSignalHeader pSig,					
					    BLOCK_ID nReceiver)
{
#ifdef CONPAN_INTERNAL_USE
    MSK_SendSignal(pSig, nReceiver);
#else
    gs_ExtFuncList.fpOS_SendSignal(pSig, nReceiver);
#endif
}

void InnerSCIFreeSignal(xSignalHeader pSig)
{
#ifdef CONPAN_INTERNAL_USE
    MSK_FreeSignal(pSig);
#else
    gs_ExtFuncList.fpOS_FreeSignal(pSig);
#endif
}

void InnerPSRegisterMsgHandler()
{
#ifdef CONPAN_INTERNAL_USE
    MSK_CPSetCallbackFunToPS(SendCPMsgToConPan);
#else
    gs_ExtFuncList.fpPS_RegisterMsgHandler(SendCPMsgToConPan);
#endif
}

void InnerInsertSIM(int card_num)
{
    SIMCP_InsertSIM(card_num);
}

void InnerRejectSIM(int card_num)
{
    SIMCP_RejectSIM(card_num);
}

BOOL InnerQuerySIM(int card_num)
{
    return SIMCP_QuerySIM(card_num);
}

void InnerShowPadSIM(int card_num)
{
    SIMCP_ShowPad(card_num);
}