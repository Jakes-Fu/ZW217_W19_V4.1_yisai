// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NVEditor.h"

#include "MainFrm.h"

#include "NVEditorDoc.h"

#include <shlwapi.h>
#include "Global.h"
#include <dbt.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_PASS    RGB(0,202,0)
#define COLOR_FAILED  RGB(255,0,0)
#define COLOR_WAIT    RGB(0,0,255)

static CString StatusString[] = 
{
    "Waiting",
	"Checking baudrate",
	"Connecting",
	"Erasing flash",
	"Downloading...",
	"Reading Flash",
	"Reseting",
	"Read Chip Type",
	"Read NV Item",
	"Change Baud",
	"Finish",
	"Unplugged",
	"Paused"
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_HEX, OnDisplayHex)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_HEX, OnUpdateDisplayHex)
	ON_COMMAND(ID_OPEN_PROJECT, OnOpenProject)
	ON_UPDATE_COMMAND_UI(ID_OPEN_PROJECT, OnUpdateOpenProject)
	ON_COMMAND(ID_SAVE_PROJECT, OnSaveProject)
	ON_UPDATE_COMMAND_UI(ID_SAVE_PROJECT, OnUpdateSaveProject)
	ON_COMMAND(ID_SAVE_PROJECT_AS, OnSaveProjectAs)
	ON_UPDATE_COMMAND_UI(ID_SAVE_PROJECT_AS, OnUpdateSaveProjectAs)
	ON_COMMAND(ID_SAVE_TO_PHONE, OnSaveToPhone)
	ON_UPDATE_COMMAND_UI(ID_SAVE_TO_PHONE, OnUpdateSaveToPhone)
	ON_COMMAND(ID_BTN_LOAD_CUSTOMERNV, OnBtnLoadCustomernv)
	ON_UPDATE_COMMAND_UI(ID_BTN_LOAD_CUSTOMERNV, OnUpdateBtnLoadCustomernv)
	ON_COMMAND(ID_CLOSE_PROJECT, OnCloseProject)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_PROJECT, OnUpdateCloseProject)
	ON_COMMAND(ID_PORT_SETTINGS, OnPortSettings)
	ON_COMMAND(ID_LOAD_IMAGE, OnLoadImage)
	ON_UPDATE_COMMAND_UI(ID_LOAD_IMAGE, OnUpdateLoadImage)
	ON_COMMAND(ID_LOAD_FROM_PHONE_BOOT, OnLoadFromPhoneBoot)
	ON_UPDATE_COMMAND_UI(ID_LOAD_FROM_PHONE_BOOT, OnUpdateLoadFromPhoneBoot)
	ON_COMMAND(ID_LOAD_FROM_PHONE, OnLoadFromPhone)
	ON_UPDATE_COMMAND_UI(ID_LOAD_FROM_PHONE, OnUpdateLoadFromPhone)
	ON_COMMAND(ID_SAVE_IMAGE, OnSaveImage)
	ON_UPDATE_COMMAND_UI(ID_SAVE_IMAGE, OnUpdateSaveImage)
	ON_COMMAND(ID_SAVE_TO_PHONE_BOOT, OnSaveToPhoneBoot)
	ON_UPDATE_COMMAND_UI(ID_SAVE_TO_PHONE_BOOT, OnUpdateSaveToPhoneBoot)
	ON_COMMAND(ID_FAIL_NVINFO, OnFailNvinfo)
	ON_UPDATE_COMMAND_UI(ID_FAIL_NVINFO, OnUpdateFailNvinfo)
	ON_COMMAND(ID_READ_FLASH, OnReadFlash)
	ON_COMMAND(ID_FLASH_MEMORY, OnFlashMemory)
	ON_COMMAND(ID_LOAD_NVVALUE, OnLoadNvvalue)
	ON_UPDATE_COMMAND_UI(ID_LOAD_NVVALUE, OnUpdateLoadNvvalue)
	ON_COMMAND(ID_IMPORT_ITEM, OnImportItem)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_ITEM, OnUpdateImportItem)
	ON_COMMAND(ID_EXPORT_STATIC, OnExportStatic)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_STATIC, OnUpdateExportStatic)
	ON_COMMAND(ID_CALIBRATION_NV, OnCalibrationNv)
	ON_UPDATE_COMMAND_UI(ID_CALIBRATION_NV, OnUpdateCalibrationNv)
	ON_COMMAND(ID_MERGE_NV, OnMergeNv)
	ON_UPDATE_COMMAND_UI(ID_MERGE_NV, OnUpdateMergeNv)
	ON_COMMAND(IDC_PACKET_MODERM, OnPacketModerm)
	ON_UPDATE_COMMAND_UI(IDC_PACKET_MODERM, OnUpdatePacketModerm)
	ON_COMMAND(ID_MENU_FUNCITIONS, OnMenuFuncitions)
	ON_UPDATE_COMMAND_UI(ID_MENU_FUNCITIONS, OnUpdateMenuFuncitions)
	ON_COMMAND(ID_MENU_FIND, OnMenuFind)
	ON_UPDATE_COMMAND_UI(ID_MENU_FIND, OnUpdateMenuFind)
	ON_COMMAND(ID_MENU_FIND_NEXT, OnMenuFindNext)
	ON_UPDATE_COMMAND_UI(ID_MENU_FIND_NEXT, OnUpdateMenuFindNext)
	ON_COMMAND(ID_GENER_STATISTICAL, OnGenerStatistical)
	ON_UPDATE_COMMAND_UI(ID_GENER_STATISTICAL, OnUpdateGenerStatistical)
	ON_COMMAND(ID_SHOW_MODIFY, OnShowModify)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODIFY, OnUpdateShowModify)
	//}}AFX_MSG_MAP
	ON_MESSAGE( BM_CHECK_BAUDRATE, OnBMCheckBaud )
    ON_MESSAGE( BM_CONNECT, OnBMConnect )
    ON_MESSAGE( BM_ERASE_FLASH, OnBMEraseFlash )
    ON_MESSAGE( BM_DOWNLOAD, OnBMDownLoad )
    ON_MESSAGE( BM_DOWNLOAD_PROCESS, OnBMDLoadProcess )
    ON_MESSAGE( BM_READ_FLASH, OnBMReadFlash )
    ON_MESSAGE( BM_READ_FLASH_PROCESS, OnBMReadFlashProcess )
    ON_MESSAGE( BM_RESET, OnBMReset )
    ON_MESSAGE( BM_READ_CHIPTYPE, OnBMReadChipType )
    ON_MESSAGE( BM_READ_NVITEM, OnBMReadNVItem )
    ON_MESSAGE( BM_CHANGE_BAUD, OnBMChangeBuad )    
    ON_MESSAGE (BM_BEGIN, OnBMBegin )
    ON_MESSAGE (BM_FILE_BEGIN, OnBMFileBegin )
    ON_MESSAGE (BM_END, OnBMEnd )
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nStage = 0;
	m_nFileStep = 0;
	m_nPort = 0;
	m_bDisplayHex	= TRUE;
	m_bSplitted		= FALSE;
	InitSyncCaliMap();
}

CMainFrame::~CMainFrame()
{
}
void CMainFrame::InitSyncCaliMap()
{
	m_mapSyncCali.clear();
	TCHAR szPath[_MAX_PATH] = {0};
	::GetModuleFileName(AfxGetApp()->m_hInstance,szPath,_MAX_PATH);
	PathRemoveExtension(szPath);
	_tcscat(szPath,_T(".ini"));
	m_nSave2PhoneMode = ::GetPrivateProfileInt(_T("Settings"),_T("EnableSaveByNormal"),0,szPath);
	if (!m_nSave2PhoneMode)
	{
		return;
	}
	_TCHAR szKeyValue[ MAX_BUF_SIZE ]={0}; 
	GetPrivateProfileSection( _T("SyncCaliTbl"), szKeyValue, MAX_BUF_SIZE, szPath );
	if ( 0 == _tcslen(szKeyValue) )
	{
		TSYNCCALIINFO agSyncCaliTable[] =		
		{
			{ _T("GSM_CaliParam"),		0x02  },
			{ _T("GSM_DownLoad"),		0x01  },
			{ _T("TD_CaliParam"),		0x516 },    
			{ _T("TD_DownLoad"),		0x517 },    
			{ _T("W_CaliParam"),		0x12D },
			{ _T("W_DownLoad"),			0x12E },
			{ _T("LTE_CaliCommon"),		0xA05 },   
			{ _T("LTE_CaliParam_T1"),	0x9C4 }, 
			{ _T("LTE_CaliParam_T2"),	0xA04 },
			{ _T("LTE_CaliParam_T3"),	0x9C8 },
			{ _T("LTE_CaliParam_T4"),	0x9C9 },
			{ _T("LTE_CaliParam_T5"),	0x9CA },
			{ _T("LTE_CaliParam_T6"),	0x9CB },
			{ _T("LTE_CaliParam_T7"),	0x9CC },
			{ _T("LTE_CaliParam_T8"),	0x9CD },
			{ _T("LTE_CaliParam_T9"),	0x9CE },
			{ _T("LTE_CaliParam_T10"),	0x9CF },
			{ _T("LTE_CaliParam_T11"),	0x9D0 },
			{ _T("LTE_CaliParam_T12"),	0x9D1 },
			{ _T("LTE_CaliParam_T13"),	0x9D2 },
			{ _T("LTE_CaliParam_T14"),	0x9D3 },
			{ _T("LTE_CaliParam_T15"),	0x9D4 },
			{ _T("LTE_CaliParam_T16"),	0x9D5 },
			{ _T("LTE_CaliParam_T17"),	0x9D6 },
			{ _T("LTE_CaliParam_T18"),	0x9D7 },
			{ _T("LTE_CaliParam_T19"),	0xA01 },
			{ _T("LTE_CaliParam_T20"),	0xA02 },
			{ _T("LTE_DownLoad_T1"),	0x9C5 },
			{ _T("LTE_DownLoad_T2"),	0xA03 },
		};
		int nCount = sizeof(agSyncCaliTable)/sizeof(TSYNCCALIINFO);
		for (int i = 0 ; i < nCount; ++i)
		{
			m_mapSyncCali[agSyncCaliTable[i].wCaliID] = agSyncCaliTable[i].szCaliName;
		}
	}
	else
	{
		CStringArray arrKeyData;    
		int nCount = (UINT)EnumKeys(szKeyValue,&arrKeyData);
		for(int i=0; i<nCount;i++)
		{
			WORD dwID = (WORD)GetDigit(arrKeyData[i*2+1]);
			m_mapSyncCali[dwID] = arrKeyData[i*2];
		}
	}
	
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	TCHAR szPath[_MAX_PATH] = {0};
	::GetModuleFileName(AfxGetApp()->m_hInstance,szPath,_MAX_PATH);
	PathRemoveExtension(szPath);
	_tcscat(szPath,_T(".ini"));
	BOOL bReadFlash = ::GetPrivateProfileInt(_T("Settings"),_T("EnableReadFlash"),0,szPath);

	CMenu *pMenu = GetMenu();
	//ASSERT( NULL != pMenu );

	if(!bReadFlash && pMenu != NULL)
	{
		pMenu->GetSubMenu(2)->DeleteMenu(ID_READ_FLASH,MF_BYCOMMAND); 	
		pMenu->GetSubMenu(2)->DeleteMenu(ID_LOAD_NVVALUE,MF_BYCOMMAND);		
	}

	if(!m_nSave2PhoneMode)
	{
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_SAVE_TO_PHONE);
		if (pMenu != NULL)
		{
			pMenu->GetSubMenu(0)->DeleteMenu(ID_SAVE_TO_PHONE,MF_BYCOMMAND); 
		}	
	}
	


#ifdef _FOR_CUSTOMER
	if(pMenu != NULL)
	{
		VERIFY( pMenu->DeleteMenu( 3, MF_BYPOSITION ) );
	}
	DrawMenuBar();
#endif // _FOR_CUSTOMER

	ShowCustPane(FALSE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 1280;
	cs.cy = 760;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (GetEditorView()->SaveAndCloseProject())
	{
		CFrameWnd::OnClose();
	}
	
}

LRESULT CMainFrame::OnBMCheckBaud( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_CHK_BAUD, FALSE, 0 , 0 );
}

LRESULT CMainFrame::OnBMConnect( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_CONNECT, FALSE, 0 , 0 );
}

LRESULT CMainFrame::OnBMEraseFlash( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_ERASE_FLASH, FALSE, 0 , 0 );
}

LRESULT CMainFrame::OnBMDownLoad( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    return SetStatus( wParam, DL_DL_STAGE,TRUE, 0, lParam  );  
}

LRESULT CMainFrame::OnBMDLoadProcess( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    return SetProgPos( wParam, lParam );
}

LRESULT CMainFrame::OnBMReadFlash( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    return SetStatus( wParam, DL_READ_STAGE,TRUE, 0, lParam  );
}

LRESULT CMainFrame::OnBMReset( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_RESET_STAGE,FALSE, 0, 0  );
}

LRESULT CMainFrame::OnBMReadChipType( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_READCHIPTYPE_STAGE ,FALSE, 0, 0  );
}

LRESULT CMainFrame::OnBMReadNVItem( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_READNVITEM_STAGE,FALSE, 0, 0  );
}

LRESULT CMainFrame::OnBMChangeBuad( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );
    return SetStatus( wParam, DL_CHANGEBUAD_STAGE,FALSE, 0, 0  );
}

LRESULT CMainFrame::OnBMReadFlashProcess( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    return SetProgPos( wParam, lParam );
}

LRESULT CMainFrame::OnBMBegin( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );    
    return SetStatus( wParam, DL_NONE_STAGE, FALSE, 0 ,0 );
}


LRESULT CMainFrame::OnBMFileBegin( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
    UNUSED_ALWAYS( lParam );    
    return SetStep( wParam );
}

LRESULT CMainFrame::OnBMEnd( WPARAM wParam /* = 0 */, LPARAM lParam /* = 0  */ )
{
   return SetResult( wParam, NULL, (LPCTSTR)lParam );
}


BOOL CMainFrame::SetStatus(int nPort,DL_STAGE stage,BOOL bNeedProg,
                             int nMin /* = 0 */,int nMax /* = 0 */)
{
    
    DL_STAGE oriStage = (DL_STAGE)m_nStage;
    if( oriStage == DL_FINISH_STAGE && stage == DL_UNPLUGGED_STAGE )
    {
        // Ignore this message
        return TRUE;
    }

    //save status
	m_nStage = stage;
	m_dlgBMLoader.SetState(StatusString[stage]);


    EndProg( nPort );

    if(bNeedProg)
    {        
        StartProg( nPort,nMin,nMax );
    }
    else
    {
        if(stage < DL_CONNECT)
        {
			CString strText = m_dlgBMLoader.m_strProg;
			if( !strText.IsEmpty() && ( strText.Find(STRING_PASS)==0  || 
					 strText.Find(STRING_PASS) == 10 || // [Removed]+space
					 strText.Find(STRING_PASS) == 9 || // Previous+space
					 strText.Find(STRING_FAILED) != -1) )
			{					
				m_dlgBMLoader.SetProg(strText);			
			}
			else
			{				
				m_dlgBMLoader.SetProg(STRING_WAIT);
			}            
        }
        else if( stage < DL_UNPLUGGED_STAGE )
        { 
			m_dlgBMLoader.SetProg(STRING_DOING);
        }
    }
    
    return TRUE;    
}

BOOL CMainFrame::GetStatus( int nPort,DL_STAGE& stage )
{ 
	UNUSED_ALWAYS(nPort);
    stage = (DL_STAGE)m_nStage;    
    return TRUE;
}

void CMainFrame::ResetStatus( int nPort )
{
    SetStatus( nPort, DL_NONE_STAGE, FALSE );

}

BOOL CMainFrame::SetResult(int nPort,BOOL bSuccess,
							  LPCTSTR lpszErrMsg /* = NULL  */,int nFlag /*= 1*/ )
{    
	UNUSED_ALWAYS(bSuccess);

	BOOL bSuc = FALSE;
	if((nFlag && lpszErrMsg == NULL) || (!nFlag && bSuccess))
		bSuc = TRUE; 


	BOOL bRemoved = FALSE;

	CString strItemText = m_dlgBMLoader.m_strProg;
	if(strItemText.Find(_T("[Removed]")) == 0)
	{
		bRemoved = TRUE;
	}

	if(m_nStage == DL_UNPLUGGED_STAGE )
	{
		//if(m_dlgBMLoader.GetSafeHwnd() != NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
		//{
		//	::PostMessage(m_dlgBMLoader.GetSafeHwnd(),bSuc?BM_PASS_MSG:BM_FAIL_MSG,0,0);
		//}
		return TRUE;
	}

	if(nFlag == 2)
	{
		EndProg( nPort );		
		strItemText = m_dlgBMLoader.m_strProg;
		if(strItemText.Find(_T("[Removed]")) !=0)
		{			
			strItemText.Insert(0,_T("[Removed] "));	
		}

		m_nStage = DL_UNPLUGGED_STAGE;
		m_dlgBMLoader.SetProg(strItemText);
		m_dlgBMLoader.SetState(StatusString[DL_UNPLUGGED_STAGE]);
		m_nStage = DL_NO_FILE;	

		//if(m_dlgBMLoader.GetSafeHwnd() != NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
		//{
		//	::PostMessage(m_dlgBMLoader.GetSafeHwnd(),bSuc?BM_PASS_MSG:BM_FAIL_MSG,0,0);
		//}

		return TRUE;
	}


     
    
    m_nStage = DL_FINISH_STAGE;
	m_dlgBMLoader.SetState(StatusString[DL_FINISH_STAGE]);

	EndProg( nPort );

    if( bSuc )
    {        
		m_dlgBMLoader.SetProg(STRING_PASS);
		//if(m_dlgBMLoader.GetSafeHwnd() != NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
		//{
		//	::PostMessage(m_dlgBMLoader.GetSafeHwnd(),BM_PASS_MSG,0,0);
		//}
    }
    else
    { 
		CString strErrText = STRING_FAILED;
		if( NULL != lpszErrMsg && lpszErrMsg[0] != '\0' )
		{
			strErrText += _T(": ");
			strErrText += lpszErrMsg;                    
		}
		if(bRemoved)
		{	
			if(strErrText.CompareNoCase(STRING_FAILED) == 0)
			{	
				bSuc = TRUE;
				strErrText = strItemText;
			}
			else
			{
				if(m_nStage >= DL_CONNECT)
				{
					strErrText.Insert(0,_T("[Removed] "));
				}
				else
				{
					strErrText = _T("[Removed] ");
				}
			}
		}
 
		m_dlgBMLoader.SetProg(strErrText);

		if(bRemoved)
		{
			m_nStage = DL_UNPLUGGED_STAGE;
			m_dlgBMLoader.SetState(StatusString[DL_UNPLUGGED_STAGE]);
		}

		//if(m_dlgBMLoader.GetSafeHwnd() != NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
		//{
		//	::PostMessage(m_dlgBMLoader.GetSafeHwnd(),BM_FAIL_MSG,0,0);
		//}
    }	

	m_nFileStep = DL_NO_FILE;


    return TRUE;
}

BOOL CMainFrame::SetStep( int nPort  )
{    
	UNUSED_ALWAYS(nPort);
	m_nFileStep++;   
    
    CString strText = _T("Process");

	if(m_StepDescription.GetSize() >= 1)
	{
		if( m_nFileStep > m_StepDescription.GetSize() )
		{	
			strText = m_StepDescription[m_StepDescription.GetSize()-1];	
		}
		else
		{
			strText = m_StepDescription[ m_nFileStep - 1 ];
		}
	}

	m_dlgBMLoader.SetStep(strText);

    return TRUE;    
}

BOOL CMainFrame::StartProg(int nPort,int nMin,int nMax)
{	
	SetProgStep(nPort,1);	
	if(m_dlgBMLoader.GetSafeHwnd()!=NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
	{
		m_dlgBMLoader.m_prgrsCtrl.SetRange32(nMin,nMax);	
		m_dlgBMLoader.m_prgrsCtrl.ShowWindow(SW_SHOW);
	}
    return TRUE;
}

BOOL CMainFrame::StepProg(int nPort,int nStep)
{     	
	OutputDebugString( _T("CMainFrame::StepProg\n") );

	UNUSED_ALWAYS(nPort);
	if(m_dlgBMLoader.GetSafeHwnd()!=NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
	{
		m_dlgBMLoader.m_prgrsCtrl.OffsetPos( nStep );
	}
    return TRUE;
}

BOOL CMainFrame::EndProg(int nPort)
{   
	OutputDebugString( _T("CMainFrame::EndProg\n") );
	UNUSED_ALWAYS(nPort);
	if(m_dlgBMLoader.GetSafeHwnd()!=NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
	{
		m_dlgBMLoader.m_prgrsCtrl.SetPos(0);	
		m_dlgBMLoader.m_prgrsCtrl.ShowWindow(SW_HIDE);	
	}
    return TRUE;
}

BOOL CMainFrame::SetProgStep(int nPort,int nStep)
{   
	UNUSED_ALWAYS(nPort);
    if(nStep <= 0)
        nStep = 1;
	
	if(m_dlgBMLoader.GetSafeHwnd()!=NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
	{
		m_dlgBMLoader.m_prgrsCtrl.SetStep(nStep);
	}
	
    return TRUE;
}

BOOL CMainFrame::SetProgPos(int nPort,int nPos)
{
	OutputDebugString( _T("CMainFrame::SetProgPos\n") );

	UNUSED_ALWAYS(nPort);
	if(m_dlgBMLoader.GetSafeHwnd()!=NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
	{
		m_dlgBMLoader.m_prgrsCtrl.SetPos(nPos);	
		//m_dlgBMLoader.m_prgrsCtrl.Invalidate(FALSE);
	}

    return TRUE;
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( /*m_bAutoStart && */(message == WM_DEVICECHANGE) )
    {
        PDEV_BROADCAST_HDR pdbh = (PDEV_BROADCAST_HDR)lParam;	
        switch(wParam)
        {
        case DBT_DEVICEARRIVAL:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
                PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
                _ASSERTE( _tcsstr(pdbp->dbcp_name, _T("COM")) != NULL );
				
				// skip COM
                int nPort = (int)_ttol( pdbp->dbcp_name + 3 ); //lint !e416
                _ASSERTE( nPort > 0 && nPort < 256 );

				
                Sleep(200);
				if(nPort != m_nPort)
				{
					break;
				}					

                DL_STAGE stage;
                if(GetStatus( nPort,stage ) )
                {
                    if( DL_FINISH_STAGE != stage && DL_NONE_STAGE != stage && DL_UNPLUGGED_STAGE != stage)
                    {
                        // Last download process is not finished.
                        break;
                    }
                }

                //StartOnePortWork(nPort,FALSE);

            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if( pdbh != NULL && pdbh->dbch_devicetype == DBT_DEVTYP_PORT )
            {
				PDEV_BROADCAST_PORT pdbp = (PDEV_BROADCAST_PORT)lParam;
                _ASSERTE( _tcsstr(pdbp->dbcp_name, _T("COM")) != NULL );
				
                int nPort = (int)_ttol( pdbp->dbcp_name + 3 );//lint !e416
                _ASSERTE( nPort > 0 && nPort < 256 );

				if(nPort != m_nPort)
				{
					break;
				}						

				DL_STAGE _stage;
                if( GetStatus( nPort,_stage ) )
                {
                    if( DL_NONE_STAGE == _stage)
                    {
                        // Last download process is not finished.
                        break;
                    }
                }

				if(m_dlgBMLoader.GetSafeHwnd()!=NULL && IsWindow(m_dlgBMLoader.GetSafeHwnd()))
				{
					::PostMessage(m_dlgBMLoader.GetSafeHwnd(),BM_STOP_MSG,0,0);
				}

            }
            break;
        default:
            break;
        }
    }
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SplitterH.CreateStatic(this, 1, 2);
	int cx = lpcs->cx;
	m_SplitterH.CreateView( 0, E_EDITOR_PANE, RUNTIME_CLASS(CNVEditorView), CSize(cx,0), pContext );
	m_SplitterH.CreateView( 0, E_CUST_PANE, RUNTIME_CLASS(CCustView), CSize(0,0), pContext );
	m_bSplitted = TRUE;
	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CMainFrame::OnDisplayHex() 
{
	// TODO: Add your command handler code here
	m_bDisplayHex = !m_bDisplayHex;
	GetEditorView()->OnDisplayHex();
	if(GetEditorView()->IsHasCustBaseNV())
	{
		GetCustView()->OnDisplayHex();
	}
}

void CMainFrame::OnUpdateDisplayHex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDisplayHex);
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}
CNVEditorView*  CMainFrame::GetEditorView()
{
	CNVEditorView* pView = NULL;
	if ( m_bSplitted)
	{
		pView = (CNVEditorView* )m_SplitterH.GetPane(0, E_EDITOR_PANE);
	}
	return pView;
}

CCustView*	CMainFrame::GetCustView()
{
	CCustView* pView = NULL;
	if ( m_bSplitted)
	{
		pView = (CCustView* )m_SplitterH.GetPane(0, E_CUST_PANE);
	}
	return pView;
}

void CMainFrame::ShowCustPane(BOOL bShow/* = FALSE*/)
{
	bShow ? m_SplitterH.ShowRightCol() : m_SplitterH.HideRightCol();
}

void CMainFrame::MinSplitPane(int nColPane)
{
	if (nColPane < E_COUNT_PANE)
	{
		m_SplitterH.SetColumnInfo(nColPane,0,0);
		m_SplitterH.RecalcLayout();
	}

}

void CMainFrame::OnOpenProject() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnOpenProject();
	
}

void CMainFrame::OnUpdateOpenProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnSaveProject() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnSaveProject();
}

void CMainFrame::OnUpdateSaveProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnSaveProjectAs() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnSaveProjectAs();
}

void CMainFrame::OnUpdateSaveProjectAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnSaveToPhone() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnSaveToPhone();
}

void CMainFrame::OnUpdateSaveToPhone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() && GetEditorView()->HasFillValue() );
}

void CMainFrame::OnBtnLoadCustomernv() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnLoadCustomerNV();
}

void CMainFrame::OnUpdateBtnLoadCustomernv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnCloseProject() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnCloseProject();
}

void CMainFrame::OnUpdateCloseProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
	
}

void CMainFrame::OnPortSettings() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnPortSettings();
}

void CMainFrame::OnLoadImage() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnLoadImage();
}

void CMainFrame::OnUpdateLoadImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnLoadFromPhoneBoot() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnLoadFromPhoneBoot();
}

void CMainFrame::OnUpdateLoadFromPhoneBoot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
	
}

void CMainFrame::OnLoadFromPhone() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnLoadFromPhone();
}

void CMainFrame::OnUpdateLoadFromPhone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnSaveImage() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnSaveImage();
}

void CMainFrame::OnUpdateSaveImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnSaveToPhoneBoot() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnSaveToPhoneBoot();
}

void CMainFrame::OnUpdateSaveToPhoneBoot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsOpenedPrj());
}

void CMainFrame::OnFailNvinfo() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnShowFailNv();
}

void CMainFrame::OnUpdateFailNvinfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (GetEditorView()->IsLoadNV());
}

void CMainFrame::OnReadFlash() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnReadFlash();
}

void CMainFrame::OnFlashMemory() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnFlashMemory();
}

void CMainFrame::OnLoadNvvalue() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnLoadNVValue();
}

void CMainFrame::OnUpdateLoadNvvalue(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() && GetEditorView()->IsEnableEditPrj());
}

void CMainFrame::OnImportItem() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnImportItem();
}

void CMainFrame::OnUpdateImportItem(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnExportStatic() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnExportStatic();
}

void CMainFrame::OnUpdateExportStatic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnCalibrationNv() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnCalibrationNv();
}

void CMainFrame::OnUpdateCalibrationNv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnMergeNv() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnMergeNv();
}

void CMainFrame::OnUpdateMergeNv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
	
}

void CMainFrame::OnPacketModerm() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnPacketModerm();
}

void CMainFrame::OnUpdatePacketModerm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnMenuFuncitions() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnFuncitions();
}

void CMainFrame::OnUpdateMenuFuncitions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnMenuFind() 
{
	// TODO: Add your command handler code here
	if (GetActiveView() == GetEditorView())
	{
		GetEditorView()->OnFind();
	}
	else if (GetActiveView() == GetCustView())
	{
		GetCustView()->OnFind();
	}
	
}

void CMainFrame::OnUpdateMenuFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnMenuFindNext() 
{
	// TODO: Add your command handler code here
	if (GetActiveView() == GetEditorView())
	{
		GetEditorView()->OnFindNext();
	}
	else if (GetActiveView() == GetCustView())
	{
		GetCustView()->OnFindNext();
	}
}

void CMainFrame::OnUpdateMenuFindNext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnGenerStatistical() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnGenerStatistical();
}

void CMainFrame::OnUpdateGenerStatistical(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}

void CMainFrame::OnShowModify() 
{
	// TODO: Add your command handler code here
	GetEditorView()->OnShowModify();
}

void CMainFrame::OnUpdateShowModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( GetEditorView()->IsOpenedPrj() );
}
