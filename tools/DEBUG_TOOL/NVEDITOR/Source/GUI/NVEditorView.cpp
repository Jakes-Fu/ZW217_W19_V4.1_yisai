// NVEditorView.cpp : implementation of the CNVEditorView class
//

#include "stdafx.h"

#include "NVEditor.h"
#include "NVEditorDoc.h"
#include "NVEditorView.h"

#include "NewProject.h"
#include "NewModule.h"
#include "NewItem.h"

#include "PacketModerm.h"
#include "PromptInfoDlg.h"

//[[ for static nv

//#include "global.h" 
extern "C"
{
	#include "md5.h"
};

//]]

//@# Kai Liu 2003.2.20
// [[
#include "DLSettingsDlg.h"


#include "FileMap.h"

#define FDL_BASE        0x32020000          /* the midst of the RAM */
#define NV_SIZE         0x20000
#define REPAR_STRATEGY_DO_ALWAYS        0  /*CR212808 xmzhang add*/

//#include "NVItem.h"
#include "PortSettingsDlg.h"
//@# Kai Liu 2003.4.21
// [[
#include "FlashDlg.h"
// ]]
#include "StiSettingDlg.h"

// @ZHANG Ming : 2003-05-07
extern CNVEditorApp theApp;

#include "ReadFlashDlg.h"
#include "Global.h"
#include "BMFile.h"
#include "DlgFunctions.h"
#include <shlwapi.h>
#include "MainFrm.h"
#include <atlconv.h>

#define  OPR_SUCCESS  (0)


#define NAND_FDL_BASE       0x30000000
#define NAND_FDL2_BASE      0x40000000


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bBigEndian;
void WINAPI CBSyncCustView()
{
	CMainFrame * pMF= (CMainFrame *)AfxGetMainWnd();		
	pMF->GetEditorView()->UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,(CObject *)(pMF->GetEditorView()->m_pProject));	
}
/////////////////////////////////////////////////////////////////////////////
// CNVEditorView

IMPLEMENT_DYNCREATE(CNVEditorView, CView)

BEGIN_MESSAGE_MAP(CNVEditorView, CView)
	//{{AFX_MSG_MAP(CNVEditorView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_NEW_PROJECT, OnNewProject)
	ON_UPDATE_COMMAND_UI(ID_NEW_PROJECT, OnUpdateNewProject)
	ON_COMMAND(ID_NEW_MODULE, OnNewModule)
	ON_UPDATE_COMMAND_UI(ID_NEW_MODULE, OnUpdateNewModule)
	ON_COMMAND(ID_NEW_ITEM, OnNewItem)
	ON_UPDATE_COMMAND_UI(ID_NEW_ITEM, OnUpdateNewItem)
	ON_COMMAND(ID_SAVE_CHANGED_TO_PHONE, OnSaveChangedToPhone)
	ON_UPDATE_COMMAND_UI(ID_SAVE_CHANGED_TO_PHONE, OnUpdateSaveChangedToPhone)
	ON_COMMAND(ID_INSERT_MODULE, OnInsertModule)
	ON_UPDATE_COMMAND_UI(ID_INSERT_MODULE, OnUpdateInsertModule)
	ON_COMMAND(ID_FILL_DEFAULT, OnFillDefault)
	ON_UPDATE_COMMAND_UI(ID_FILL_DEFAULT, OnUpdateFillDefault)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16,OnFileMruFile)
	ON_MESSAGE(BM_STOP_MSG,OnBMStopMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVEditorView construction/destruction

CNVEditorView::CNVEditorView():m_bEnableEditPrj(TRUE)
{
	// TODO: add construction code here
    
    //menu condition
	m_bOpenedPrj      = FALSE;       //Project has opened
    m_bSelectedModule = FALSE;       //
    m_bPrjIsDirty     = FALSE; 
    m_bHasFillValue   = FALSE;
    m_bValueChanged   = FALSE;

    m_bInitListCtrl   = FALSE;
	m_bFullWrite	  = FALSE;

    m_pProject = NULL;

    // @ZHANG Ming : 2003-04-16
    // [[
    //m_pUart = NULL;
    m_nNVItemTotalCount = 0;
    m_nCurrCount = 0;
    // ]]
    m_lpReadBuffer = NULL;
    m_dwReadBufferSize = 0;
    
    m_dwNandFdlAddr = NAND_FDL_BASE;
    m_dwNandFdl2Addr = NAND_FDL2_BASE;

	m_nCheckBaudTimes = 3;

	m_pClearCaliFlag = NULL;

	m_bCCPrompt = FALSE;

	m_bUserCancel = FALSE;

	m_strLFPLastErr.Empty();

	m_strLoadFailInfo.Empty();  //Joly

	m_strLoadFailInfo.Empty();

	m_bLoadNV = FALSE;

	m_bDisplayHex = TRUE;

	m_dwNVMaxSize = NV_SIZE;

    /*CR212808 xmzhang add begin*/
    m_bNvBaseStr = FALSE;
    /*CR212808 xmzhang add end*/
    m_szappend = NULL;

	m_bPortSecondEnum	= FALSE;
	m_bHasCustBase		= FALSE;
    CreateBootModeObj();

}

CNVEditorView::~CNVEditorView()
{    
    //Must Close Project
    if(NULL != m_pProject)
    {
        // @ZHANG Ming : 2003-05-07
        // [[
        m_bWriteDSP = m_pProject->IsWriteDsp();
        // ]]

      delete m_pProject;
      m_pProject = NULL;
    }

    // @ZHANG Ming : 2003-04-17
    // [[
    LFPClearUp();
//     if ( NULL != m_pUart )
//     {
//         m_pUart->Close();
//         delete m_pUart;
//         m_pUart = NULL;
//     }
	m_DiagObj.Close();
    // ]]

    SaveSettings();
    FreeRecvBuffer();
    DestroyBootModeObj();

	ClearFuncs();
}

BOOL CNVEditorView::AllocRecvBuffer(  const LPBYTE lpRecvBuffer,  DWORD dwSize )
{
    if( m_lpReadBuffer != NULL )
    {
        if( m_dwReadBufferSize == dwSize )
        {
            ZeroMemory( m_lpReadBuffer, m_dwReadBufferSize );
        }
        else
        {
            delete[] m_lpReadBuffer;
            m_lpReadBuffer = NULL;
            m_dwReadBufferSize = 0;
        }
    }
    if( m_lpReadBuffer == NULL )
    {
        m_lpReadBuffer = new BYTE [ dwSize ];
        if( m_lpReadBuffer == NULL )
            return FALSE;
        m_dwReadBufferSize = dwSize;
    }
    memcpy( m_lpReadBuffer, lpRecvBuffer, dwSize );
    
    return TRUE;
}

void CNVEditorView::FreeRecvBuffer()
{
    if( m_lpReadBuffer != NULL )
    {
        delete[] m_lpReadBuffer;
        m_lpReadBuffer = NULL;
        m_dwReadBufferSize = 0;    
    }
}

BOOL CNVEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNVEditorView drawing

void CNVEditorView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	CNVEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNVEditorView printing

BOOL CNVEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNVEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNVEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNVEditorView diagnostics

#ifdef _DEBUG
void CNVEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CNVEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNVEditorDoc* CNVEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNVEditorDoc)));
	return (CNVEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNVEditorView message handlers

void CNVEditorView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_List.GetSafeHwnd () && m_bInitListCtrl)
    {
       m_List.SetWindowPos (0, -1, -1, cx, cy, SWP_SHOWWINDOW);
    }
}

int CNVEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_List.SetSyncCBFunc(CBSyncCustView);
    m_List.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);
    m_List.InitializeGrid();

    LoadSettings();

	m_List.SetModifyID(m_bModifyID);

	m_xBMOprObserver.pThis = this;

	return 0;
}

BOOL CNVEditorView::LoadSettings()
{
    TCHAR szFilePath[_MAX_PATH]={0};
    if(!theApp.GetIniFilePath(szFilePath))
    {
        return FALSE;
    }

    TCHAR szResult[_MAX_PATH] = _T("");
    GetPrivateProfileString( _T("Files"), _T("DSP"), _T(""), szResult, _MAX_PATH, szFilePath);
    m_strDspFileName =  szResult;
    GetPrivateProfileString( _T("Files"), _T("Project"), _T(""), szResult, _MAX_PATH, szFilePath);
    m_strPrjFileName =  szResult;
    GetPrivateProfileString( _T("Files"), _T("FDL"), _T(""), szResult, _MAX_PATH, szFilePath);
    m_strFdlFileName =  szResult;
    GetPrivateProfileString( _T("Files"), _T("FDL2"), _T(""), szResult, _MAX_PATH, szFilePath);
    m_strFdl2 =  szResult;
    GetPrivateProfileString( _T("Settings"), _T("Select Product"), _T(""), szResult, _MAX_PATH, szFilePath);
    m_strSelProduct =  szResult;
    
    m_dwNandFdlAddr = GetPrivateProfileInt( _T("Address"), _T("FDL"), NAND_FDL_BASE, szFilePath);
    m_dwNandFdl2Addr = GetPrivateProfileInt( _T("Address"), _T("FDL2"), NAND_FDL2_BASE, szFilePath);
    
    m_bWriteDSP = GetPrivateProfileInt( _T("Settings"), _T("WriteDSP"), 0, szFilePath );

	m_bModifyID		= GetPrivateProfileInt( _T("Settings"), _T("ModifyNVId"), 0, szFilePath );
	m_bFullWrite	= GetPrivateProfileInt( _T("Settings"), _T("FullWrite"), FALSE, szFilePath );

    /*CR212808 xmzhang modify begin*/
    if(_T("Shark") == m_strSelProduct)
    {
        memset(szResult,0,sizeof(szResult));
        GetPrivateProfileString( _T("Product list"), _T("Shark"), _T(""), szResult, _MAX_PATH, szFilePath);
        _tcscpy(m_dwNvBaseAddr.szAddr,szResult);
        m_bNvBaseStr = GetPrivateProfileInt( _T("Settings"), _T("IsShark"), 0, szFilePath );
    }
    
    else
    {
        m_dwNvBaseAddr.dwAddr = GetPrivateProfileInt( _T("Settings"), _T("NV Base"), 0x90000001, szFilePath );
    }
    /*CR212808 xmzhang modify end*/

    m_bNewFDLFile = GetPrivateProfileInt( _T("Settings"), _T("NewFDLFile"), 1, szFilePath );
    m_nPort      = GetPrivateProfileInt( _T("SerialPort"), _T("Port"), 1, szFilePath );
    m_nBaudrate = GetPrivateProfileInt( _T("SerialPort"), _T("Baudrate"), 115200, szFilePath ); 

	m_nCheckBaudTimes = GetPrivateProfileInt( _T("Settings"), _T("CheckBaudTimes"), 3, szFilePath ); 

	m_bCCPrompt = GetPrivateProfileInt( _T("Settings"), _T("PromptClearCalibrationFlag"), 1, szFilePath );

	TCHAR szBMFiletypeIni[_MAX_PATH] = {0};

	TCHAR szFuncIni[_MAX_PATH] = {0};	
	_tcscpy(szFuncIni,szFilePath);
	PathRemoveFileSpec(szFuncIni);
	_tcscpy(szBMFiletypeIni,szFuncIni);
	_tcscat(szFuncIni,_T("\\functions.ini"));
	LoadFuncConfig(szFuncIni);

	_tcscat(szBMFiletypeIni,_T("\\BMFileType.ini"));

	m_dwNVMaxSize = GetPrivateProfileInt( _T("DownloadNV"), _T("MaxReadLength"), NV_SIZE, szBMFiletypeIni );

    return TRUE;    
}

BOOL CNVEditorView::SaveSettings()
{
    TCHAR szFilePath[_MAX_PATH];
    if(!theApp.GetIniFilePath(szFilePath))
    {
        return FALSE;
    }
	CFileFind finder;
	if(finder.FindFile(szFilePath))
	{
		DWORD dwAttr = ::GetFileAttributes(szFilePath);
		dwAttr &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFilePath,dwAttr);
	}

    ::WritePrivateProfileString( _T("Files"),_T("DSP"),m_strDspFileName,szFilePath);
    ::WritePrivateProfileString( _T("Files"),_T("Project"),m_strPrjFileName,szFilePath);
    ::WritePrivateProfileString( _T("Files"),_T("FDL"),m_strFdlFileName,szFilePath);    
    ::WritePrivateProfileString( _T("Files"),_T("FDL2"),m_strFdl2,szFilePath);    
    ::WritePrivateProfileString( _T("Settings"), _T("Select Product"),m_strSelProduct,szFilePath);    
    
    CString strTemp;
    strTemp.Format( _T("%d"),m_bWriteDSP );
    ::WritePrivateProfileString(_T("Settings"),_T("WriteDSP"),strTemp,szFilePath);
    /*CR212808 xmzhang modify begin*/
    strTemp.Format(_T("0x%X"),m_dwNvBaseAddr.dwAddr);
    /*CR212808 xmzhang modify end*/
    ::WritePrivateProfileString(_T("Settings"),_T("NV Base"),strTemp,szFilePath);
    strTemp.Format(_T("%d"),m_bNewFDLFile);
    ::WritePrivateProfileString(_T("Settings"),_T("NewFDLFile"),strTemp,szFilePath);
    strTemp.Format( _T("%d"),m_nPort );
    ::WritePrivateProfileString(_T("SerialPort"),_T("Port"),strTemp,szFilePath);
    strTemp.Format( _T("%d"),m_nBaudrate );
    ::WritePrivateProfileString(_T("SerialPort"),_T("Baudrate"),strTemp,szFilePath);  

	strTemp.Format( _T("0x%X"),m_dwNandFdlAddr );
    ::WritePrivateProfileString(_T("Address"),_T("FDL"),strTemp,szFilePath);  

	strTemp.Format( _T("0x%X"),m_dwNandFdl2Addr );
    ::WritePrivateProfileString(_T("Address"),_T("FDL2"),strTemp,szFilePath);  

    /*CR212808 xmzhang add begin*/
    if(m_bNvBaseStr)
    {
        strTemp.Format( _T("%s"),m_dwNvBaseAddr.szAddr);
        ::WritePrivateProfileString(_T("Product list"),_T("Shark"),strTemp,szFilePath);
    }
    strTemp.Format( _T("%d"),m_bNvBaseStr);
    ::WritePrivateProfileString(_T("Settings"),_T("IsShark"),strTemp,szFilePath); 
    /*CR212808 xmzhang add end*/
	
    return TRUE;
}

void CNVEditorView::OnNewProject() 
{
	// TODO: Add your command handler code here
    CNewProject   dlgPrj;    
    if(IDOK == dlgPrj.DoModal ())
    {        
        if(NULL != m_pProject)
        {
            //Notify to save and close the current project
            if(FALSE == SaveAndCloseProject())
              return;
        }
        m_strPrjFileName =  dlgPrj.m_szPrjFileName.operator LPCTSTR ();
        //Create a new project
        m_pProject = new CFileParser();

        // @ZHANG Ming : 2003-05-06
        // [[
        m_pProject->SetWriteDsp( m_bWriteDSP );
        // ]]

        /*xml file support by xmzhang add begin*/
        if( !theApp.m_bNvmFormat )
        {        
            m_pProject->CreateProjectXML (dlgPrj.m_szPrjName.operator LPCTSTR () , 
                                   dlgPrj.m_szPrjFileName.operator LPCTSTR (), 
                                   dlgPrj.m_szTargetFileName.operator LPCTSTR(),
                                   dlgPrj.m_szPrjDesc .operator LPCTSTR ());
        }
        else
        {
            m_pProject->CreateProject (dlgPrj.m_szPrjName.operator LPCTSTR () , 
                                   dlgPrj.m_szPrjFileName.operator LPCTSTR (), 
                                   dlgPrj.m_szTargetFileName.operator LPCTSTR(),
                                   dlgPrj.m_szPrjDesc .operator LPCTSTR ());
        
        }
        /*xml file support by xmzhang add end*/


        m_List.SetProject (m_pProject);
        m_List.Refresh ();
		UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	

        //Show List
        m_bInitListCtrl = TRUE;        
        CRect rcWindow;
        GetClientRect(&rcWindow);
        OnSize(0, rcWindow.Width(), rcWindow.Height());

        m_bOpenedPrj = TRUE;
    }
}

void CNVEditorView::OnNewModule() 
{
	// TODO: Add your command handler code here
	CNewModule  dlgModule;

    dlgModule.SetProject (m_pProject);
    dlgModule.DoModal ();

    m_List.Refresh ();
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
}

void CNVEditorView::OnNewItem() 
{
	// TODO: Add your command handler code here
	CNewItem  dlgItem;

    dlgItem.SetProject (m_pProject, m_List.GetSelectedModuleIndex());
    dlgItem.DoModal ();
    m_List.Refresh ();
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	

}


void CNVEditorView::OnInsertModule() 
{
	// TODO: Add your command handler code here
	/*xml file support by xmzhang modify begin*/
	TCHAR szfiles[_MAX_PATH]={0};
	TCHAR szappend[_MAX_PATH]={0};
	if( !theApp.m_bNvmFormat )
	{
		_tcscpy(szfiles,_T("Module Files (*.xml *.xml)|*.xml;*.xml||"));
		_tcscpy(szappend,_T("*.xml"));
	}
	else
	{
		_tcscpy(szfiles,_T("Module Files (*.nvm)|*.nvm||"));
		_tcscpy(szappend,_T("*.nvm"));
	}
	CFileDialog dlgOpenModule( TRUE ,              //Open File
                            szappend,               //default Extension
                            NULL,
                            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER,
                            szfiles,
                            this
                            );
	/*xml file support by xmzhang modify end*/
	
	if(IDOK == dlgOpenModule.DoModal ())
	{       
		//Get Module file name
		CString szModuleFileName = dlgOpenModule.GetPathName () ;
        
        //Insert the given module into current project
		CStringArray agWarning;
        m_pProject->InsertModule((LPTSTR)(LPCTSTR)szModuleFileName,agWarning);
		
		if (theApp.m_bShowWorning && agWarning.GetSize())
		{
			CPromptInfoDlg dlg;
			dlg.SetPromptText(agWarning);
			dlg.DoModal();
		}
        
        //refresh list
        m_List.Refresh ();
		UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
    }
}
BOOL CNVEditorView::IsHasCustBaseNV()
{
	BOOL bRet = FALSE;
	if(	m_bHasCustBase || m_List.IsExistCustNV(TRUE))
	{
		m_bHasCustBase = TRUE;
		bRet = TRUE; 
	}
	return bRet;
}
void CNVEditorView::UpdatedMsgAllViews( int nMsgID, CObject* pObj/* = NULL*/ )
{
	CDocument * pDoc = NULL;
	pDoc = (CDocument *)GetDocument();
	if ( pDoc && IsHasCustBaseNV() )
	{
		pDoc->UpdateAllViews(this,nMsgID,pObj);
	}
}

void CNVEditorView::AutoLoadCustNV(LPCTSTR lpNVPrjFile,BOOL bCustBase /*= FALSE*/)
{
	TCHAR szCfgFile[MAX_PATH] = {0};
	theApp.GetIniFilePath(szCfgFile);
	BOOL bAutoLoadCustNV = ::GetPrivateProfileInt(_T("CustomerNV"),_T("AutoLoad"),1,szCfgFile);
	if (bAutoLoadCustNV)
	{
		CString strCustNVFile = GetStrFilePath(lpNVPrjFile,FALSE,TRUE);
		if (bCustBase)
		{
			strCustNVFile += g_szCustBaseFile;
		}
		else
		{
			strCustNVFile += g_szCustNVName;
		}
		
		if( IsExistFile(strCustNVFile.operator LPCTSTR()))
		{
			if (bCustBase)
			{
				m_bHasCustBase = TRUE;	
				m_List.ImportItem(strCustNVFile.operator LPCTSTR(),FALSE,TRUE);
				UpdatedMsgAllViews(WM_CREATE_CUST_TREE,(CObject *)m_pProject);				
			}
			else
			{
				m_List.ImportItem(strCustNVFile.operator LPCTSTR(),TRUE,FALSE);
			}
		}
	}
	

}

void CNVEditorView::OnOpenProject() 
{
	// TODO: Add your command handler code here

	/*xml file support by xmzhang modify begin*/
	TCHAR szfiles[_MAX_PATH]={0};
	if( theApp.m_bNvmFormat )
	{
		_tcscpy(szfiles,_T("Project Files (*.prj)|*.prj|NV Item files (*.nvm)|*.nvm|All Files (*.*)|*.*||"));
	}
	else 
	{
		//_tcscpy(szfiles,_T("Project Files (*.xprj)|*.xprj|NV Item files (*.xml)|*.xml|All Files (*.*)|*.*|"));
		_tcscpy(szfiles,_T("Project Files (*.xprj *.prj)|*.xprj;*.prj|NV Item files (*.xml *.nvm)|*.xml;*.nvm|All Files (*.*)|*.*||"));
	}
	CFileDialog dlgOpenPrj( TRUE ,              //Open File
                            NULL,            //default Extension
                            m_strPrjFileName,
                            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER,
                            szfiles,
                            this
                            );
 	/*xml file support by xmzhang modify end*/
//    dlgOpenPrj.m_ofn.lpstrFile = szFileName;

    if(IDOK == dlgOpenPrj.DoModal ())
    {
        if(NULL != m_pProject)
        {
            //Notify to save and close the current project
            if(FALSE == SaveAndCloseProject())
                return;
        }

		CWaitCursor waitCur;

		CString strFilePath = dlgOpenPrj.GetPathName();

		CString strExt = dlgOpenPrj.GetFileExt();

		m_bHasCustNV = FALSE;
		/*xml file support by xmzhang modify begin*/
		if( strExt.CompareNoCase(_T("prj")) ==0 || strExt.CompareNoCase(_T("xprj")) ==0 )
		/*xml file support by xmzhang modify end*/
		{
			if (!OpenProject(strFilePath) )
			{
				CString strText;
				strText.LoadString(IDS_OPEN_PRJ_FAIL);
				AfxMessageBox(strText);
			}
			AutoLoadCustNV(strFilePath.operator LPCTSTR(),TRUE);
			AutoLoadCustNV(strFilePath.operator LPCTSTR());	
				
			
		}
		else if(strExt.CompareNoCase(_T("nvm"))==0)
		{
			TCHAR szNVMName[_MAX_PATH] = {0};
			_tcscpy(szNVMName,strFilePath);
			PathRemoveExtension(szNVMName);
			PathStripPath(szNVMName);			

			TCHAR szTmpPath[_MAX_PATH] = {0};
			GetTempPath(_MAX_PATH,szTmpPath);

			CString strPrjName = szNVMName;
			strPrjName.Format(_T("_%s"),szNVMName);
			CString strPrjFileName;
			strPrjFileName.Format(_T("%s\\_%s.prj"),szTmpPath,szNVMName);
			CString strPrjBinName;
			strPrjBinName.Format(_T("%s\\_%s.bin"),szTmpPath,szNVMName);

		
			//Create a new project
			m_pProject = new CFileParser();

			// @ZHANG Ming : 2003-05-06
			// [[
			m_pProject->SetWriteDsp( m_bWriteDSP );
			// ]]

			m_pProject->CreateProject(strPrjName.operator LPCTSTR () , 
									   strPrjFileName.operator LPCTSTR (), 
									   strPrjBinName.operator LPCTSTR(),
									   _T("temporary project"));

			CStringArray agWarning;
			TCHAR szCfgFile[MAX_PATH] = {0};
			theApp.GetIniFilePath(szCfgFile);
			BOOL bCheckItemName	= ::GetPrivateProfileInt(_T("Settings"),_T("CheckItemName"),0,szCfgFile);

			m_pProject->InsertModule((LPTSTR)(LPCTSTR)strFilePath,agWarning,bCheckItemName);
			if (theApp.m_bShowWorning && agWarning.GetSize())
			{
				CPromptInfoDlg dlg;
				dlg.SetPromptText(agWarning);
				dlg.DoModal();
			}
			m_List.SetProject (m_pProject);	
			//refresh list
			m_List.Refresh ();	
			UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
			
			//Show List
			m_bInitListCtrl = TRUE;        
			CRect rcWindow;
			GetClientRect(&rcWindow);
			OnSize(0, rcWindow.Width(), rcWindow.Height());
			
			m_bOpenedPrj = TRUE;			
	
		}
		/*xml file support by xmzhang add begin*/
		else if(strExt.CompareNoCase(_T("xml"))==0)
		{
			TCHAR szNVMName[_MAX_PATH] = {0};
			_tcscpy(szNVMName,strFilePath);
			PathRemoveExtension(szNVMName);
			PathStripPath(szNVMName);

			TCHAR szTmpPath[_MAX_PATH] = {0};
			GetTempPath(_MAX_PATH,szTmpPath);

			CString strPrjName = szNVMName;
			strPrjName.Format(_T("_%s"),szNVMName);
			CString strPrjFileName;
			strPrjFileName.Format(_T("%s\\_%s.xprj"),szTmpPath,szNVMName);
			CString strPrjBinName;
			strPrjBinName.Format(_T("%s\\_%s.bin"),szTmpPath,szNVMName);

		
			//Create a new project
			m_pProject = new CFileParser();

			// @ZHANG Ming : 2003-05-06
			// [[
			m_pProject->SetWriteDsp( m_bWriteDSP );
			// ]]

			m_pProject->CreateProjectXML(strPrjName.operator LPCTSTR () , 
									   strPrjFileName.operator LPCTSTR (), 
									   strPrjBinName.operator LPCTSTR(),
									   _T("temporary project"));

			CStringArray agWarning;
			m_pProject->InsertModule((LPTSTR)(LPCTSTR)strFilePath,agWarning);

			if (theApp.m_bShowWorning && agWarning.GetSize())
			{
				CPromptInfoDlg dlg;
				dlg.SetPromptText(agWarning);
				dlg.DoModal();
			}
			m_List.SetProject (m_pProject);	
			//refresh list
			m_List.Refresh ();	
			UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
			
			//Show List
			m_bInitListCtrl = TRUE;        
			CRect rcWindow;
			GetClientRect(&rcWindow);
			OnSize(0, rcWindow.Width(), rcWindow.Height());
			
			m_bOpenedPrj = TRUE;			
	
		}
		/*xml file support by xmzhang modify end*/
	}
}

// Must give full path name
 BOOL CNVEditorView::OpenProject(const TCHAR* szPrjFileName)
{
	m_bHasCustBase = FALSE;
    m_strPrjFileName = szPrjFileName;
    
	if ( m_pProject != NULL )
	{
		delete m_pProject;
	}

    m_pProject = new CFileParser();
    // @ZHANG Ming : 2003-05-06
    // [[
    m_pProject->SetWriteDsp( m_bWriteDSP );
    // ]]

    /*xml file support by xmzhang modify begin*/
    m_szappend=_tcsrchr(m_strPrjFileName,'.');
	CStringArray agWarning;
	((CMainFrame *)AfxGetMainWnd())->m_wndToolBar.EnableWindow(FALSE);	
	BOOL bLoadRet = FALSE;
	TCHAR szCfgFile[MAX_PATH] = {0};
	theApp.GetIniFilePath(szCfgFile);
	BOOL bCheckItemName	= ::GetPrivateProfileInt(_T("Settings"),_T("CheckItemName"),0,szCfgFile);

    if(0==_tcscmp(m_szappend,_T(".xprj")))
    {
		bLoadRet = m_pProject->LoadProjectXML ((LPTSTR)(LPCTSTR)m_strPrjFileName,agWarning,bCheckItemName );
    }
    else if(0==_tcscmp(m_szappend,_T(".prj")))
    {
		bLoadRet = m_pProject->LoadProject ((LPTSTR)(LPCTSTR)m_strPrjFileName,agWarning,bCheckItemName );
    }
	((CMainFrame *)AfxGetMainWnd())->m_wndToolBar.EnableWindow(TRUE);	
	if (!bLoadRet)
	{
		SAFE_DELETE(m_pProject);
		return FALSE;
	}
    /*xml file support by xmzhang modify end*/
	if (theApp.m_bShowWorning && agWarning.GetSize())
	{
		CPromptInfoDlg dlg;
		dlg.SetPromptText(agWarning);
		dlg.DoModal();
	}

    m_List.SetProject (m_pProject);
    m_List.Refresh ();
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
    
    //Show List
    m_bInitListCtrl = TRUE;        
    CRect rcWindow;
    GetClientRect(&rcWindow);
    OnSize(0, rcWindow.Width(), rcWindow.Height());
    
    m_bOpenedPrj = TRUE;
    m_bHasFillValue = TRUE;

    AfxGetApp()->AddToRecentFileList(szPrjFileName);
    
    return TRUE;
}


void CNVEditorView::OnLoadImage() 
{
	// TODO: Add your command handler code here
	CFileDialog dlgOpenImage( TRUE ,              //Open File
                            _T("*.bin"),            //default Extension
                            NULL,
                            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER,
                            _T("NV Image Files (*.bin)|*.bin|All files (*.*)|*.*||"),
                            this
                            );
 
    if(IDOK == dlgOpenImage.DoModal ())
    {
		CFileMap map( dlgOpenImage.GetPathName () );
		
		const void * pvFile = map;
		if ( NULL == pvFile )
		{
			CString strErr;
			strErr.Format( _T("Map file %s failed"), dlgOpenImage.GetPathName () );
			AfxMessageBox(strErr);
			return;
		}
		DWORD cbFileSize = map.GetSize();

		CWaitCursor wc;

		//[[ add check hash
// 		WORD wLength;
// 
// 		const void* pHashItem = GetNVItemFromBlock((const unsigned char*)pvFile,
// 			                 cbFileSize, 0, &wLength);
// 		
// 		if (NULL != pHashItem && 36 == wLength && !m_pProject->CompareHashValue((const unsigned char*)pHashItem))
// 		{
// 			AfxMessageBox( _T("Stop by different NV version, Please check NV version!\n") );
// 			return;
// 		}
		//]]
		((CMainFrame *)AfxGetMainWnd())->m_wndToolBar.EnableWindow(FALSE);	
		LoadNVItemFromImage( ( const unsigned char* )pvFile,cbFileSize );
		((CMainFrame *)AfxGetMainWnd())->m_wndToolBar.EnableWindow(TRUE);	
        m_List.Refresh ();
		UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);
    }

    
}

void CNVEditorView::OnSaveProject() 
{
	// TODO: Add your command handler code here
	CWaitCursor wc;
	BOOL result=TRUE;

	TCHAR szCfgFile[MAX_PATH] = {0};
	theApp.GetIniFilePath(szCfgFile);
	BOOL bAutoSaveCustNV = ::GetPrivateProfileInt(_T("CustomerNV"),_T("AutoSave"),1,szCfgFile);
	if (bAutoSaveCustNV && m_pProject && _tcslen(m_pProject->m_szPrjFileName) )
	{
		CString strCustNVFile = GetStrFilePath(m_pProject->m_szPrjFileName,FALSE,TRUE);
		strCustNVFile += g_szCustNVName;
		if( IsExistFile(strCustNVFile.operator LPCTSTR()) || m_List.IsExistCustNV())
		{
			m_List.ExportCustNV(strCustNVFile.operator LPCTSTR());
		}	
	}	
	UpdatedMsgAllViews(WM_SAVE_CUST_BASE,(CObject *)(m_pProject->m_szPrjFileName));
	m_szappend=_tcsrchr(m_strPrjFileName,'.');
	if (NULL == m_szappend)
	{
		m_szappend=_tcsrchr(m_pProject->m_szPrjFileName,'.');
	}
	
	if (m_szappend)
	{
		if( 0 == _tcsicmp(m_szappend,_T(".xprj")) )
		{
			result = m_pProject->SaveProjectXML();
		}
		else
		{
			result = m_pProject->SaveProject();
		}
	}

	if(FALSE == result)
	{
		//Notify that save project failed
		AfxMessageBox(_T("Save project failed!"));
    }
	else
	{
		m_List.Refresh ();/*xml file support by xmzhang add*/
		UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
		AfxMessageBox(_T("Save project success!"));
	}
	/*xml file support by xmzhang modify end*/
}

void CNVEditorView::OnSaveProjectAs() 
{
	// TODO: Add your command handler code here
	/*xml file support by xmzhang modify begin*/
	TCHAR szfiles[_MAX_PATH]={0};
	TCHAR szappend[_MAX_PATH]={0};
	if( !theApp.m_bNvmFormat )
	{
		_tcscpy(szfiles,_T("Project Files (*.xprj)|*.xprj|All Files (*.*)|*.*||"));
		_tcscpy(szappend,_T("*.xprj"));
	}
	else
	{
		_tcscpy(szfiles,_T("Project Files (*.prj)|*.prj|All Files (*.*)|*.*||"));
		_tcscpy(szappend,_T("*.prj"));
	}
	CFileDialog dlgSavePrjAs( FALSE ,              //Open File
		szappend,               //default Extension
		_T("nvitem"),
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER,
		szfiles,
		this
		);
	/*xml file support by xmzhang modify end*/

	
//	m_pProject->SetHexFormat(m_bDisplayHex); //add by Joly
	
	if(IDOK == dlgSavePrjAs.DoModal ())
	{
		CString szPrjFileName = dlgSavePrjAs.GetPathName () ;
		CWaitCursor wc;


		TCHAR szCfgFile[MAX_PATH] = {0};
		theApp.GetIniFilePath(szCfgFile);
		BOOL bAutoSaveCustNV = ::GetPrivateProfileInt(_T("CustomerNV"),_T("AutoSave"),1,szCfgFile);
		if (bAutoSaveCustNV)
		{
			CString strCustNVFile = GetStrFilePath(szPrjFileName.operator LPCTSTR(),FALSE,TRUE);
			strCustNVFile += g_szCustNVName;
			if( IsExistFile(strCustNVFile.operator LPCTSTR()) || m_List.IsExistCustNV())
			{
				if( AfxMessageBox( _T("Are you sure to save customer.nv file?"),MB_YESNO ) == IDYES )
				{
					m_List.ExportCustNV(strCustNVFile.operator LPCTSTR());
				}	
			}	
		}	

		UpdatedMsgAllViews(WM_SAVEAS_CUST_BASE,(CObject *)(szPrjFileName.operator LPCTSTR()));
        if(!m_pProject->SaveProjectAs ((LPTSTR)(LPCTSTR)szPrjFileName))
        {
            //Notify that save project failed
			AfxMessageBox(_T("Save project as failed!"));
        }
        else
        {
            // Successfully saved,close old project and open new one
            m_pProject->SetModified(FALSE);
            OnCloseProject();
			wc.Restore();
            if ( !OpenProject(szPrjFileName) )
			{
				CString strText;
				strText.LoadString(IDS_OPEN_PRJ_FAIL);
				AfxMessageBox(strText);
			}
			AutoLoadCustNV(szPrjFileName.operator LPCTSTR(),TRUE);	
			AutoLoadCustNV(szPrjFileName.operator LPCTSTR());
			
        }
    }
}

void CNVEditorView::OnSaveImage() 
{
	// TODO: Add your command handler code here
	BOOL fWriteDsp = FALSE;
	LPCTSTR lpszDspFileName = NULL;

#ifndef _FOR_CUSTOMER

	CStiSettingDlg dlg;
	dlg.m_bWriteDsp = m_pProject->IsWriteDsp();
    dlg.m_strDspFileName = m_strDspFileName;

    if(IDOK != dlg.DoModal())
        return;

	fWriteDsp = dlg.m_bWriteDsp;
	lpszDspFileName = dlg.m_strDspFileName;

	if (dlg.m_bWriteFix)
	{
		m_pProject->SetWriteFix(dlg.m_strFixBinName.operator LPCTSTR());
	}
	
	m_pProject->SetVersionFlag(dlg.m_bVerFlag);
	

#endif // ndef _FOR_CUSTOMER

	if(m_pClearCaliFlag != NULL)
	{
		BOOL bClear = TRUE;
		if(!m_List.TestItemChange(m_pClearCaliFlag->agRules,TRUE))
		{
			bClear = FALSE;
		}
		if(bClear && m_bCCPrompt)
		{
			if(AfxMessageBox(_T("Clear calibration flag?"),MB_YESNO) == IDNO)
			{
				bClear = FALSE;
			}
		}
		if(bClear)
		{
			if( !m_List.ImportItem(m_pClearCaliFlag->agRules,TRUE))
			{
				::AfxMessageBox( _T("Clear calibration flag failed!") );
				return;
			}
			UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);
		}
	}
// 	else
// 	{
// 		::AfxMessageBox( _T("Can not find [ClearCalibrationFlag] in functions.ini!") );
// 		return;
// 	}

	//[[ by Joly
	//if ( dlg.m_bActiveFiles )
	{
		m_pProject->SetMulFileAg( dlg.m_bActiveFiles, dlg.m_vecFileInfo );
	}
	//]]

	// For customized verson, there is no need to write
	// DSP code and fWriteDsp should be FALSE.
	if ( SaveToImage( fWriteDsp, lpszDspFileName ) )
	{
	//	GenerateItemInfo();
 		::AfxMessageBox( _T("Image file has been saved succefully!") );
	}
	else
	{
		// Error message has been shown in CFileParser::SaveToImage
	}
}

// @ZHANG Ming : 2003-05-06
void CNVEditorView::OnSaveToPhoneBoot() 
{
    //@# Kai Liu 2003.2.20
    // [[
   

   // If m_strFdlFileName is empty,
   // we must intialize it first.
   if ( m_strFdlFileName.IsEmpty() )
   {
	   	if ( !PortSettings( FALSE ) )
		{
			return;
		}
   }

   // For customized versoin, there is no need to write dsp
   // code. And preserving calibration parameter is mandatory, 
   // replacing new calibration parameter with old one must 
   // be FALSE.
   BOOL    fWriteDsp       = FALSE;
   LPCTSTR lpszDspFileName = NULL;

#ifndef _FOR_CUSTOMER

   m_DLSetDlg.m_bWriteDsp = m_pProject->IsWriteDsp();
   m_DLSetDlg.m_strDspFileName = m_strDspFileName;


    if(IDOK != m_DLSetDlg.DoModal())
        return;

    // Reserve user's choice
	fWriteDsp = m_DLSetDlg.m_bWriteDsp;
	lpszDspFileName = m_DLSetDlg.m_strDspFileName;

#endif // ndef _FOR_CUSTOMER

	CWaitCursor wait;
//  @Xiaoping, 2010-05-10: support little endian [[[
//	extern BOOL g_bBigEndian;
//  BOOL bEndian = g_bBigEndian;
//  g_bBigEndian = TRUE;
	if ( !SaveToImage( fWriteDsp, lpszDspFileName ) )
	{
	//	GenerateItemInfo();// 生成明文nvitem 信息
		::AfxMessageBox( _T("Generating image file fail") );
//		g_bBigEndian = bEndian;
		return;
	}
//	g_bBigEndian = bEndian;

	CFileFind finder;

	if(_tcslen(m_pProject->m_szTargetFileName) == 0 ||
		!finder.FindFile(m_pProject->m_szTargetFileName))
	{
		::AfxMessageBox( _T("Generating image file fail") );
		return;
	}

	// For customer version, 
	SaveToPhone( m_nPort,
				 m_nBaudrate,
                 m_strFdlFileName,		
				 m_dwNandFdlAddr,			
                 m_pProject->m_szTargetFileName,//szImageFile,
                 m_dwNvBaseAddr);

}


void CNVEditorView::OnSaveChangedToPhone() 
{
	// TODO: Add your command handler code here
	
}

void CNVEditorView::OnLoadFromPhone() 
{
    // Get NVItem count
	DWORD dwStartTime = GetTickCount();
	DWORD dwElapsedTime = 0;
	CString strElapsedTime;

    m_nNVItemTotalCount = m_pProject->GetNVItemCount();
	m_nReadFail = 0;
	m_nInterpFail = 0;

	DWORD dwThreadID = 0;
	HANDLE hThread = NULL;
	int ret = 0;

	TCHAR szFilePath[_MAX_PATH]={0};
	theApp.GetIniFilePath(szFilePath);
	theApp.GetPermission();

    if ( 0 == m_nNVItemTotalCount )
    {
        ::AfxMessageBox( _T("No NVItems") );
        return;
    }
    m_nCurrCount = 0;

    // Clear up m_pUart and 
    LFPClearUp();
	m_DiagObj.Close();

	
	if(m_DiagObj.Open(szFilePath) != DIAG_SUCCESS)
	{
		::AfxMessageBox( _T("Error: Open channel fail"), MB_ICONSTOP );
        goto LFP_ERROR;
	}

	m_bUserCancel = FALSE;

	// Show Feedback dialog to users
	m_dlgFeedBack.m_bRead = TRUE;
	m_dlgFeedBack.m_bInit = FALSE;
    m_dlgFeedBack.m_nRange = m_nNVItemTotalCount;
    m_dlgFeedBack.m_strInfo.Format( _T("0 item received. (Total %d items)"), 
                                    m_nNVItemTotalCount );
	m_strLFPLastErr.Empty();
	m_strLoadFailInfo.Empty();

	hThread = CreateThread( NULL,
							0,
							(LPTHREAD_START_ROUTINE)GetReadWorkThread,
							(LPVOID)this,
							NULL,
							&dwThreadID);

    ret = m_dlgFeedBack.DoModal();

	m_dlgFeedBack.m_bInit = FALSE;
	m_bUserCancel = TRUE;

	if(hThread)
	{
		WaitForSingleObject( hThread, 5000);
		CloseHandle( hThread);
		hThread = NULL;
	}

	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	dwElapsedTime = GetTickCount() - dwStartTime;
	//strElapsedTime.Format(_T("ReadNV ElapsedTime: %d.%ds."),dwElapsedTime/1000,dwElapsedTime%1000);
    //AfxMessageBox(strElapsedTime);

    if ( IDCANCEL == ret )
    {
		m_bUserCancel = TRUE;
        // User canceled this load process
        AfxMessageBox( _T("This load process is canceled") );
    }
    else if ( IDOK == ret )
    {
        // DoModal returned from OnCmmSendData 
        // when everything is done successfully
		CString strInfo;
		strInfo.Format( _T("This load process Over! Elapsed time: %d.%ds.\nTotal %d items; Read fail: %d items; Interpret fail: %d items\n\n%s"),
			dwElapsedTime/1000,
			dwElapsedTime%1000,
			m_nNVItemTotalCount,
			m_nReadFail,
			m_nInterpFail, m_strLoadFailInfo);
        AfxMessageBox( strInfo );
    }
    else if ( IDIGNORE == ret )
    {
        // If ret IDABORT, there must be sth wrong happened
        // m_dlgFeedBack.EndDialog() is invoked with IDIGNORE in LFPClearUp();
        return;
    }
    else if( IDABORT == ret )
    {
        AfxMessageBox( _T( "Can not read item from phone!" ),MB_ICONSTOP );
    }
	else if(IDNO == ret)
	{
		AfxMessageBox(_T("Can not read NV version info from phone!"), MB_ICONSTOP);
	}

    LFPClearUp();
    return;


LFP_ERROR:
    LFPClearUp();
    return; 
}

void CNVEditorView::OnFillDefault() 
{
	// TODO: Add your command handler code here	
    m_pProject->FillWithDefault ();
    m_bHasFillValue = TRUE;
    m_List.Refresh ();
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	

}

void CNVEditorView::OnCloseProject() 
{
	// TODO: Add your command handler code here
    //Notify to save and close the current project
    if(FALSE == SaveAndCloseProject())
        return;

    //Show List
    m_bOpenedPrj  = FALSE;
    m_bInitListCtrl = FALSE;
    m_List.SetWindowPos (NULL, -1, -1, 0, 0, SWP_HIDEWINDOW);
}


void CNVEditorView::OnUpdateNewProject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (m_bEnableEditPrj);
}

void CNVEditorView::OnUpdateNewModule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (m_bEnableEditPrj && m_bOpenedPrj);
}

void CNVEditorView::OnUpdateNewItem(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    m_bSelectedModule = m_List.IsSelectedModule();

	pCmdUI->Enable ( m_bEnableEditPrj && 
                     m_bOpenedPrj &&
                     m_bSelectedModule);
}

void CNVEditorView::OnUpdateInsertModule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (m_bEnableEditPrj && m_bOpenedPrj);
}

void CNVEditorView::OnUpdateSaveChangedToPhone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (m_bOpenedPrj && 
                    m_bHasFillValue &&
                    m_bValueChanged);
}

void CNVEditorView::OnUpdateFillDefault(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (m_bOpenedPrj);
}

BOOL CNVEditorView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here and/or call default
    if(FALSE == m_bInitListCtrl)
    {
        //改变视窗背景色 
        CBrush Brush;
        Brush.CreateSolidBrush (::GetSysColor (COLOR_SCROLLBAR));
        CBrush *pOldBrush = pDC->SelectObject (&Brush);
        
        CRect rcClip;
        pDC->GetClipBox (&rcClip);
        pDC->PatBlt (rcClip.left , rcClip.top , rcClip.Width ( ) ,
            rcClip.Height( ) , PATCOPY );
        
        pDC->SelectObject (pOldBrush);
        
        return TRUE;
    }

	return CView::OnEraseBkgnd(pDC);
}

BOOL CNVEditorView::SaveAndCloseProject(BOOL bCanCancel)
{
    //Make sure to save and close the current Project
    BOOL bRetCode = TRUE;

    if(NULL == m_pProject)
        return TRUE;

    m_bValueChanged = m_pProject->IsModified();

    if(m_bValueChanged)
    {
        CString szMsg;
        int retCode;

        szMsg  = "Save changed to " ;
        szMsg += m_pProject->m_szTargetFileName;
        if(bCanCancel)
        {
            retCode = AfxMessageBox(szMsg, MB_YESNOCANCEL);
        }
        else
        {
            retCode = AfxMessageBox(szMsg, MB_YESNO);
        }

        switch (retCode)
        {
        case IDYES:
            OnSaveProject();
            OnSaveImage();
			UpdatedMsgAllViews(WM_DESTROY_CUST_TREE,NULL);
			Sleep(1000);
            m_List.CloseProject();
            // @ZHANG Ming : 2003-05-07
            // [[
            m_bWriteDSP = m_pProject->IsWriteDsp();
            // ]]
            delete m_pProject;
            m_pProject = NULL;
            break;
        case IDNO:
			UpdatedMsgAllViews(WM_DESTROY_CUST_TREE,NULL);
			Sleep(1000);
            m_List.CloseProject();	
            // @ZHANG Ming : 2003-05-07
            // [[
            m_bWriteDSP = m_pProject->IsWriteDsp();
            // ]]
            delete m_pProject;
            m_pProject = NULL;
            break;
        case IDCANCEL:
            bRetCode = FALSE;
            break;
        }
    }
    // @ZHANG Ming : 2003-05-06
    // [[
    else
    {
		UpdatedMsgAllViews(WM_DESTROY_CUST_TREE,NULL);
		Sleep(1000);
        m_List.CloseProject();	
        // @ZHANG Ming : 2003-05-07
        // [[
        m_bWriteDSP = m_pProject->IsWriteDsp();
        // ]]
        delete m_pProject;
        m_pProject = NULL;
    }
    // ]]

    return bRetCode;
}

//@# Kai Liu 2003.2.20
// [[
/*CR212808 xmzhang modify begin*/
void CNVEditorView::SaveToPhone(int nPort,
                                int nBaudRate,
                                CString strFDL,
                                DWORD dwFDLBase,
                                CString strNV,
                                NvBaseAddr dwNVBase,
								BOOL  bPresCaliParam,
                                BOOL  bOldReplaceNew
                               )
{
	UNREFERENCED_PARAMETER(bPresCaliParam);
	UNREFERENCED_PARAMETER(bOldReplaceNew);
    BMFileInfo fileInfo[ 3 ];

	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();
	pMF->m_StepDescription.RemoveAll();
	pMF->m_nFileStep = 0;
	pMF->m_nStage = 0;

	if(strNV.IsEmpty())
	{
		return;
	}
	CheckPort2ndEnum();
	if (m_bPortSecondEnum)
	{
		BMFileInfo fileInfo1[3], fileInfo2[3];
		BOOL  bRet = FALSE;
		//FDL1
		_tcscpy(fileInfo1[0].szFileID , _T("CheckBaud"));
		_tcscpy(fileInfo1[0].szFileType , _T("CheckBaud"));
		fileInfo1[0].bChangeCode = FALSE;
		fileInfo1[0].bLoadCodeFromFile = FALSE;
		fileInfo1[0].dwBase = 0; 
		pMF->m_StepDescription.Add(_T("CheckBaud"));
		
		_tcscpy(fileInfo1[1].szFileName, strFDL);
		_tcscpy(fileInfo1[1].szFileID , _T("FDLA"));
		_tcscpy(fileInfo1[1].szFileType , _T("FDL1"));
		//fileInfo1[1].bChangeCode = FALSE;
		fileInfo1[1].bLoadCodeFromFile = TRUE;
		fileInfo1[1].dwBase = m_dwNandFdlAddr;
		pMF->m_StepDescription.Add(_T("FDL1"));
		
		bRet = StartBootModeWork(nPort, nBaudRate, (DWORD)fileInfo1, 2);
		Sleep(theApp.m_dwEnumPortTimeOut);
		//FDL2
		if (bRet)
		{
			pMF->m_StepDescription.RemoveAll();
			pMF->m_nFileStep = 0;
			pMF->m_nStage = 0;
			_tcscpy(fileInfo2[0].szFileID , _T("CheckBaud"));
			_tcscpy(fileInfo2[0].szFileType , _T("CheckBaud"));
			fileInfo2[0].bChangeCode = FALSE;
			fileInfo2[0].bLoadCodeFromFile = FALSE;
			fileInfo2[0].dwBase = 0; 
			pMF->m_StepDescription.Add(_T("CheckBauds"));
			
			_tcscpy(fileInfo2[1].szFileName, m_strFdl2);
			_tcscpy(fileInfo2[1].szFileID , _T("FDLB"));
			_tcscpy(fileInfo2[1].szFileType , _T("FDL2"));
			fileInfo2[1].bLoadCodeFromFile = TRUE;
			fileInfo2[1].dwBase = m_dwNandFdl2Addr;
			pMF->m_StepDescription.Add(_T("FDL2"));

			//NV
			_tcscpy(fileInfo2[2].szFileName, strNV );
			fileInfo2[2].dwBase = dwNVBase.dwAddr;
			_tcscpy(fileInfo2[2].szRepID , dwNVBase.szAddr);
			fileInfo2[2].bLoadCodeFromFile = TRUE;
			fileInfo2[2].dwOprSize = m_dwNVMaxSize;
			fileInfo2[2].bChangeCode = TRUE;
			_tcscpy(fileInfo2[2].szFileType, _T("CODE2") ) ;  
			_tcscpy(fileInfo2[2].szFileID,_T("NV"));
			pMF->m_StepDescription.Add(_T("NV"));
			StartBootModeWork( nPort, nBaudRate, (DWORD)fileInfo2, 3 );
		}
		return;
	}
    /*CR267264 by xmzhang modify begin*/        
    if( m_strSelProduct.CompareNoCase(_T("Nand"))&&m_strSelProduct.CompareNoCase(_T("Shark")))
    {	
		//FDL1
        _tcscpy(fileInfo[0].szFileName, strFDL );
        fileInfo[0].dwBase = dwFDLBase;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy(fileInfo[0].szFileID,_T("FDL"));
		pMF->m_StepDescription.Add(_T("FDL"));
    
        //NV
        _tcscpy(fileInfo[1].szFileName, strNV );
        fileInfo[1].dwBase = dwNVBase.dwAddr;
        _tcscpy(fileInfo[1].szRepID , dwNVBase.szAddr);
        fileInfo[1].bLoadCodeFromFile = TRUE;
        fileInfo[1].dwOprSize = m_dwNVMaxSize;
        fileInfo[1].bChangeCode = TRUE;
        _tcscpy(fileInfo[1].szFileType, _T("NV") ) ; 
		_tcscpy(fileInfo[1].szFileID,_T("NV"));
		pMF->m_StepDescription.Add(_T("NV"));
      
        StartBootModeWork( nPort, nBaudRate, (DWORD)fileInfo, 2 );
    }
    else if(m_strSelProduct.CompareNoCase(_T("Shark")))
    {
		//FDL1
        _tcscpy(fileInfo[0].szFileName, strFDL );
        fileInfo[0].dwBase = m_dwNandFdlAddr;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy(fileInfo[0].szFileID,_T("FDL"));
		pMF->m_StepDescription.Add(_T("FDL"));
		
		//FDL2
        _tcscpy(fileInfo[1].szFileName, m_strFdl2 );
        fileInfo[1].dwBase = m_dwNandFdl2Addr;
        fileInfo[1].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[1].szFileType , _T("Nand_FDL") );
		_tcscpy(fileInfo[1].szFileID,_T("FDL2"));
		pMF->m_StepDescription.Add(_T("FDL2"));
		
		//NV
        _tcscpy(fileInfo[2].szFileName, strNV );
        fileInfo[2].dwBase = dwNVBase.dwAddr;
        _tcscpy(fileInfo[2].szRepID , dwNVBase.szAddr);
        fileInfo[2].bLoadCodeFromFile = TRUE;
        fileInfo[2].dwOprSize = m_dwNVMaxSize;
        fileInfo[2].bChangeCode = TRUE;
        _tcscpy(fileInfo[2].szFileType, _T("CODE") ) ;  
		_tcscpy(fileInfo[2].szFileID,_T("NV"));
		pMF->m_StepDescription.Add(_T("NV"));
      
        StartBootModeWork( nPort, nBaudRate, (DWORD)fileInfo, 3 );
    }    
	else
	{
		//FDL1
        _tcscpy(fileInfo[0].szFileName, strFDL );
        fileInfo[0].dwBase = m_dwNandFdlAddr;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy(fileInfo[0].szFileID,_T("FDL"));
		pMF->m_StepDescription.Add(_T("FDL"));
		
		//FDL2
        _tcscpy(fileInfo[1].szFileName, m_strFdl2 );
        fileInfo[1].dwBase = m_dwNandFdl2Addr;
        fileInfo[1].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[1].szFileType , _T("Nand_FDL") );
		_tcscpy(fileInfo[1].szFileID,_T("FDL2"));
		pMF->m_StepDescription.Add(_T("FDL2"));
		
		//NV
        _tcscpy(fileInfo[2].szFileName, strNV );
        fileInfo[2].dwBase = dwNVBase.dwAddr;
        _tcscpy(fileInfo[2].szRepID , dwNVBase.szAddr);
        fileInfo[2].bLoadCodeFromFile = TRUE;
        fileInfo[2].dwOprSize = m_dwNVMaxSize;
        fileInfo[2].bChangeCode = TRUE;
        _tcscpy(fileInfo[2].szFileType, _T("CODE2") ) ;  

		_tcscpy(fileInfo[2].szFileID,_T("NV"));
		pMF->m_StepDescription.Add(_T("NV"));
      
        StartBootModeWork( nPort, nBaudRate, (DWORD)fileInfo, 3 );
    }
    /*CR267264 by xmzhang modify end*/ 
}
// ]]

//@# Kai Liu 2003.4.21
// [[
BOOL CNVEditorView::FlashMemory(UINT uPort,UINT uBaudRate,CString strFDLFile,NvBaseAddr stAddress,DWORD dwSize,CString strFdl2)
{
	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();
	pMF->m_StepDescription.RemoveAll();
	pMF->m_nFileStep = 0;
	pMF->m_nStage = 0;
	
    BMFileInfo fileInfo[ 3 ];
	
    if( strFdl2.IsEmpty() )
    {
        _tcscpy(fileInfo[0].szFileName, strFDLFile );
		fileInfo[0].dwBase = m_dwNandFdlAddr;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy( fileInfo[0].szFileID , _T("FDL") );
		pMF->m_StepDescription.Add(_T("FDL"));
		
        fileInfo[1].dwBase = stAddress.dwAddr;
        _tcscpy(fileInfo[1].szRepID,stAddress.szAddr);
        fileInfo[1].bLoadCodeFromFile = FALSE;
        fileInfo[1].dwOprSize = dwSize;
        fileInfo[1].lpCode = NULL;
        if(m_bNvBaseStr)
		{
			_tcscpy(fileInfo[1].szFileType, m_bNewFDLFile?_T("EraseFlash2"):_T("EraseFlash_Old2") ) ;  
        }
        else
        {
            _tcscpy(fileInfo[1].szFileType, m_bNewFDLFile?_T("EraseFlash"):_T("EraseFlash_Old") ) ;  
        }
		_tcscpy( fileInfo[1].szFileID , _T("EraseFlash") );
		
		pMF->m_StepDescription.Add(_T("EraseFlash"));
		
        return StartBootModeWork( uPort, uBaudRate, (DWORD)fileInfo, 2 );
    }
    else
    {
        // Nand flash
		CheckPort2ndEnum();
		if (m_bPortSecondEnum)
		{
			BOOL  bRet = FALSE;
			BMFileInfo fileInfo1[3], fileInfo2[3];
			//FDL1
			
			pMF->m_StepDescription.RemoveAll();
			pMF->m_nFileStep = 0;
			pMF->m_nStage = 0;
			_tcscpy(fileInfo1[0].szFileID , _T("CheckBaud"));
			_tcscpy(fileInfo1[0].szFileType , _T("CheckBaud"));
			fileInfo1[0].bChangeCode = FALSE;
			fileInfo1[0].bLoadCodeFromFile = FALSE;
			fileInfo1[0].dwBase = 0; 
			pMF->m_StepDescription.Add(_T("CheckBaud"));
			
			_tcscpy(fileInfo1[1].szFileName, strFDLFile);
			_tcscpy(fileInfo1[1].szFileID , _T("FDLA"));
			_tcscpy(fileInfo1[1].szFileType , _T("FDL1"));
			//fileInfo1[1].bChangeCode = FALSE;
			fileInfo1[1].bLoadCodeFromFile = TRUE;
			fileInfo1[1].dwBase = m_dwNandFdlAddr;
			pMF->m_StepDescription.Add(_T("FDL1"));
			
			bRet = StartBootModeWork(uPort, uBaudRate, (DWORD)fileInfo1, 2);
			
			Sleep(theApp.m_dwEnumPortTimeOut);
			if (bRet)
			{
				pMF->m_StepDescription.RemoveAll();
				pMF->m_nFileStep = 0;
				pMF->m_nStage = 0;
				_tcscpy(fileInfo2[0].szFileID , _T("CheckBaud"));
				_tcscpy(fileInfo2[0].szFileType , _T("CheckBaud"));
				fileInfo2[0].bChangeCode = FALSE;
				fileInfo2[0].bLoadCodeFromFile = FALSE;
				fileInfo2[0].dwBase = 0; 
				pMF->m_StepDescription.Add(_T("CheckBauds"));
				
				_tcscpy(fileInfo2[1].szFileName, strFdl2);
				_tcscpy(fileInfo2[1].szFileID , _T("FDLB"));
				_tcscpy(fileInfo2[1].szFileType , _T("FDL2"));
				//fileInfo2[1].bChangeCode = FALSE;
				fileInfo2[1].bLoadCodeFromFile = TRUE;
				fileInfo2[1].dwBase = m_dwNandFdl2Addr;
				pMF->m_StepDescription.Add(_T("FDL2"));

				fileInfo2[2].dwBase = stAddress.dwAddr;
				_tcscpy(fileInfo2[2].szRepID,stAddress.szAddr);
				fileInfo2[2].bLoadCodeFromFile = FALSE;
				fileInfo2[2].dwOprSize = dwSize;
				fileInfo2[2].lpCode = NULL;
				if(m_bNvBaseStr)
				{
					_tcscpy(fileInfo2[2].szFileType, _T("EraseFlash2") ) ; 
				}
				else
				{
					_tcscpy(fileInfo2[2].szFileType, _T("EraseFlash") ) ; 
				}
				_tcscpy( fileInfo2[2].szFileID , _T("EraseFlash") );
				pMF->m_StepDescription.Add(_T("EraseFlash"));

				bRet = StartBootModeWork(uPort, uBaudRate, (DWORD)fileInfo2, 3);

			}
			return bRet;
		}

        _tcscpy(fileInfo[0].szFileName, strFDLFile );
        fileInfo[0].dwBase = m_dwNandFdlAddr;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy( fileInfo[0].szFileID , _T("FDL") );
		pMF->m_StepDescription.Add(_T("FDL"));
        
        _tcscpy(fileInfo[1].szFileName, strFdl2 );
        fileInfo[1].dwBase = m_dwNandFdl2Addr;
        fileInfo[1].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[1].szFileType , _T("Nand_FDL") );
		_tcscpy( fileInfo[1].szFileID , _T("FDL2") );
		pMF->m_StepDescription.Add(_T("FDL2"));
		
        fileInfo[2].dwBase = stAddress.dwAddr;
        _tcscpy(fileInfo[2].szRepID,stAddress.szAddr);
        fileInfo[2].bLoadCodeFromFile = FALSE;
        fileInfo[2].dwOprSize = dwSize;
        fileInfo[2].lpCode = NULL;
        if(m_bNvBaseStr)
		{
			_tcscpy(fileInfo[2].szFileType, _T("EraseFlash2") ) ; 
        }
        else
        {
            _tcscpy(fileInfo[2].szFileType, _T("EraseFlash") ) ; 
        }
		_tcscpy( fileInfo[2].szFileID , _T("EraseFlash") );
		pMF->m_StepDescription.Add(_T("EraseFlash"));
        
        return StartBootModeWork( uPort, uBaudRate, (DWORD)fileInfo, 3 );
    }
}
/*CR212808 xmzhang modify end*/
// ]]

void CNVEditorView::OnFlashMemory() 
{
	if ( m_strFdlFileName.IsEmpty() )
	{
		if ( !PortSettings( FALSE ) )
		{
			return;
		}
	}

	BOOL bNand = TRUE;
	CString strTemp = m_strSelProduct;
	strTemp.MakeLower();
	/*CR212808 xmzhang modify begin*/
	if(strTemp.Find( _T("nand") ) == -1&&strTemp.Find( _T("shark") ) == -1)
	/*CR212808 xmzhang modify end*/
	{
		bNand = FALSE;
	}
	else
	{
		if(m_strFdl2.IsEmpty())
		{
			if ( !PortSettings( FALSE ) )
			{
				return;
			}
		}
	}

    static CFlashDlg flashDlg;

    if(IDOK != flashDlg.DoModal())
    {
        return;
    }

    /*CR212808 xmzhang modify begin*/
    CString address = flashDlg.GetAddressEx();
    NvBaseAddr stNVBase;

    if(m_bNvBaseStr)
    {
        _tcscpy(stNVBase.szAddr,address);
    }
    else
    {
        DWORD dwAddr;
        if(address.Left(2).CompareNoCase(_T("0x")))
        {
            // Decimal
            _stscanf(address,_T("%d"),&dwAddr);
        }
        else
        {
            // Heximal
            _stscanf(address,_T("%x"),&dwAddr);
        }
        stNVBase.dwAddr = dwAddr;
    }



    FlashMemory( m_nPort,
                 m_nBaudrate,
                 m_strFdlFileName,
                 stNVBase,
                 flashDlg.GetSize(),
				 bNand?m_strFdl2:_T("")
               );
    /*CR212808 xmzhang modify end*/
}
// ]]

// @ZHANG Ming : 2003-04-16
// [[
void CNVEditorView::LFPClearUp( void )
{
    int size;
    if ( ( size = m_aNVItems.GetSize() ) != 0 )
    {
        for( int i = 0; i < size; i++ )
        {
            LPBYTE pBuf = m_aNVItems.GetAt(i); 
			if(pBuf != NULL)
			{
				delete [] pBuf; 
			}
        }

        m_aNVItems.RemoveAll();
    }

	m_DiagObj.Close();

    if ( m_dlgFeedBack.GetSafeHwnd() != NULL )
    {
        m_dlgFeedBack.EndDialog( IDIGNORE );
    }
}

BOOL CNVEditorView::WriteAllNVItems( void )
{    
    int size = m_aNVItems.GetSize();
	CString strTip =  _T("Refreshing... Please Waiting!");
	m_dlgFeedBack.m_nRange = size;
	if(FALSE == m_pProject->ReCalcNVStruct())
	{
		return FALSE;
	}
    for( int i=0; i < size; i++ )
    {
		m_dlgFeedBack.UpdateFeedBack( strTip, i, TRUE );
		LPBYTE pBuf = m_aNVItems[i];
		ASSERT( pBuf != NULL );  
		uint16 nLen  = *( ( uint16* ) (pBuf) ); // need not convert endian
		if ( nLen <= 2 )
		{
			m_strLFPLastErr = _T("Size of NV item loaded from phone is error");
			return FALSE; 
		}
		nLen = (uint16)(nLen - 2); // remove ID 
		uint16 nItemId = *( ( uint16* ) (pBuf+2) ); // skip ID 
		CONVERT_SHORT2( nItemId, nItemId );
		LPVOID pData = pBuf+4;   // skip Len and ID
		BOOL ret =  m_pProject->WriteNVItem( nItemId, pData, nLen, TRUE,FALSE );
		if ( !ret )
		{
			CString strErr;
			strErr.Format( _T("Can't interpret Item %d."),nItemId );
			m_dlgFeedBack.UpdateFeedBack( strTip, i, TRUE, strErr.operator LPCTSTR() );
			m_strLoadFailInfo += strErr;
			m_strLoadFailInfo += _T("\n");
			m_nInterpFail++;
		}
    }
    return TRUE;
}
    


void CNVEditorView::OnFileMruFile(UINT uID) 
{
    // Close current opened project if exist one
    if(FALSE == SaveAndCloseProject())
        return;

    //Show List
    m_bOpenedPrj  = FALSE;
    m_bInitListCtrl = FALSE;
    m_List.SetWindowPos (NULL, -1, -1, 0, 0, SWP_HIDEWINDOW);

    CNVEditorApp* pApp = (CNVEditorApp*)AfxGetApp();
	CString strPrjFile = pApp->GetMruFileName(uID - ID_FILE_MRU_FILE1);

    if( !OpenProject(strPrjFile) )
    {
        CString strText;
        strText.LoadString(IDS_OPEN_PRJ_FAIL);
        AfxMessageBox(strText);
    }
	AutoLoadCustNV(strPrjFile.operator LPCTSTR(),TRUE);	
	AutoLoadCustNV(strPrjFile.operator LPCTSTR());
	
}

// ]]

/*
// @ZHANG Ming : 2003-04-23
// [[
void CNVEditorView::OnConfig( void )
{
    ASSERT( m_pProject != NULL );
    
    CConfigDlg dlg;
    dlg.m_bWriteDsp = m_pProject->IsWriteDsp();

    if ( IDOK == dlg.DoModal() )
    {
        m_pProject->SetWriteDsp( dlg.m_bWriteDsp );
    }
}

void CNVEditorView::OnUpdateConfig( CCmdUI *pCmdUI )
{
    pCmdUI->Enable( m_pProject != NULL );
}

// ]]
*/

void CNVEditorView::OnPortSettings() 
{
	// TODO: Add your command handler code here
	PortSettings( TRUE );
}

BOOL CNVEditorView::PortSettings( BOOL bCanNoFdl )
{
	CPortSettingsDlg dlg;

	dlg.m_bCanNoFdl = bCanNoFdl;
	dlg.m_nBaudrate = m_nBaudrate;
	dlg.m_nPort = m_nPort;
	dlg.m_strFdl = m_strFdlFileName;
    dlg.m_strProduct = m_strSelProduct;
    dlg.m_bNewFDLFile = m_bNewFDLFile;
    dlg.m_strFdl2 = m_strFdl2;
	dlg.m_bNvBaseStr = m_bNvBaseStr;

	// @Hongliang Xin, 2009-3-25, 
	//[[
	dlg.m_dwFDLBase = m_dwNandFdlAddr;
	dlg.m_dwFDL2Base = m_dwNandFdl2Addr;
	//]]

    TCHAR szFilePath[_MAX_PATH];
    if(!theApp.GetIniFilePath(szFilePath))
    {
        return FALSE;
    }
    
    dlg.m_strIniFilePath = szFilePath;
	// @ZHANG Ming, April 1, 2004, cr8397
	dlg.m_dwNvBaseAddr = m_dwNvBaseAddr;

	if ( dlg.DoModal() == IDOK )
	{
		m_nBaudrate = dlg.m_nBaudrate;
		m_nPort     = dlg.m_nPort;
		m_strFdlFileName  = dlg.m_strFdl;
		// @ZHANG Ming, April 1, 2004, cr8397
		m_dwNvBaseAddr = dlg.m_dwNvBaseAddr;
        m_strSelProduct = dlg.m_strProduct;
        m_bNewFDLFile = dlg.m_bNewFDLFile;
        m_strFdl2 = dlg.m_strFdl2;
		m_bNvBaseStr = dlg.m_bNvBaseStr;

		// @Hongliang Xin, 2009-3-25, 
		//[[
		m_dwNandFdlAddr  = dlg.m_dwFDLBase;
		m_dwNandFdl2Addr = dlg.m_dwFDL2Base;
		//]]

		if(!SaveSettings())
		{
			AfxMessageBox(_T("Save settings failed"));
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CNVEditorView::OnLoadFromPhoneBoot() 
{
	ASSERT( NULL != m_pProject );


	// If m_strFdlFileName is empty,
	// we must intialize it first.
	if ( m_strFdlFileName.IsEmpty() )
	{
		if ( !PortSettings( FALSE ) )
		{
			return;
		}
	}

    CWaitCursor wait;
    BOOL bSuccess;

	BOOL bNand = TRUE;
	CString strTemp = m_strSelProduct;
	strTemp.MakeLower();
	/*CR212808 xmzhang modify begin*/
	if(strTemp.Find( _T("nand") ) == -1&&strTemp.Find( _T("shark") ) == -1 )
	/*CR212808 xmzhang modify end*/
	{
		bNand = FALSE;
	}

	CheckPort2ndEnum();
    if( !bNand )
    {
	    // @ZHANG Ming, April 1, 2004, cr8397
        bSuccess = GetFlashContent( m_nPort,
	    			 m_nBaudrate,
					 m_strFdlFileName,
					 m_dwNvBaseAddr,
					 m_dwNVMaxSize
					) ;
    }
    else
    {
		// add for 446444
		if (m_bPortSecondEnum)
		{
			bSuccess = GetFlashContentEx( m_nPort,
				m_nBaudrate,
				m_strFdlFileName,
				m_dwNvBaseAddr,
				m_dwNVMaxSize,
				m_strFdl2
        					) ;
		}
		else
		{
            bSuccess = GetFlashContent( m_nPort,
        	    			 m_nBaudrate,
        					 m_strFdlFileName,
        					 m_dwNvBaseAddr,
        					 m_dwNVMaxSize,
                             m_strFdl2
        					) ;
		}
    }

	if ( NULL == m_lpReadBuffer || !bSuccess )
	{
//		 ::AfxMessageBox( _T("Error: Can't read content from flash!") );
		 return;
	}

#if defined ( _DEBUG ) && !defined ( _FOR_CUSTOMER )

		TCHAR szFileName[ MAX_PATH ];
		::GetModuleFileName( NULL, szFileName, sizeof( szFileName ) );
		CString strTempImageFile = szFileName;
		int nIndex  = strTempImageFile.ReverseFind( _T('\\') );
		if ( nIndex != -1 )
		{
			strTempImageFile = strTempImageFile.Left( nIndex + 1 );
		}
		strTempImageFile += _T("NvItemImage.bin"); 
		FILE *pImageFile = ::_tfopen( strTempImageFile, _T("wb") );
		if ( pImageFile != NULL )
		{
			::fwrite( m_lpReadBuffer, m_dwNVMaxSize, 1, pImageFile );
			::fclose( pImageFile );
		}

#endif // defined ( _DEBUG ) && !defined ( _FOR_CUSTOMER )

//  @Xiaoping, 2010-05-10: support little endian [[[
	// phone ware must be big endian;
//  extern BOOL g_bBigEndian;
//  BOOL bEndian = g_bBigEndian;
//  g_bBigEndian = TRUE;

// 	WORD wLength;
// 	const void* pHashItem = GetNVItemFromBlock(m_lpReadBuffer, m_dwNVMaxSize, 0, &wLength);
// 		
// 	if (NULL != pHashItem && 36 == wLength && !m_pProject->CompareHashValue((const unsigned char*)pHashItem))
// 	{
// 		AfxMessageBox( _T("Stop by different NV version, Please check NV version!\n") );
// 		return;
// 	}


	LoadNVItemFromImage( m_lpReadBuffer, m_dwNVMaxSize );

//	g_bBigEndian = bEndian;

	m_List.Refresh();
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);
	return;
}

void CNVEditorView::CheckPort2ndEnum()
{
	m_bPortSecondEnum = FALSE;
	
	CFileFind finder;
	if(!finder.FindFile(m_strFdlFileName))
	{
		return;
	}
	
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = ::CreateFile(m_strFdlFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	DWORD dwSize = GetFileSize(hFile,NULL);
	
	if(dwSize == 0 || dwSize == 0xFFFFFFFF)
	{
		return;
	}
	
	BYTE *pBuf = new BYTE[dwSize];
	
	DWORD dwRealRead =  0;
	ReadFile(hFile,pBuf,dwSize,&dwRealRead,NULL);
	
	CloseHandle(hFile);
	
	const char szFlag[] = "##PORT-SECOND-ENUM##";
	
	BYTE* lpPos = NULL;
	lpPos = std::search( pBuf,pBuf + dwSize,szFlag,szFlag + strlen( szFlag ));
	if(lpPos < (pBuf + dwSize) )
	{
		m_bPortSecondEnum = TRUE;
	}		
    delete [] pBuf;
}

BOOL CNVEditorView::GetFlashContentEx(
									int nPort,
									int nBaudrate,
									const CString& strFdl,
									NvBaseAddr dwBase,
									DWORD dwSize,
									CString strFdl2
									 )
{
	BMFileInfo fileInfo1[3], fileInfo2[3];
	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();

	BOOL  bRet = FALSE;
	//FDL1
	{
		pMF->m_StepDescription.RemoveAll();
		pMF->m_nFileStep = 0;
		pMF->m_nStage = 0;
		_tcscpy(fileInfo1[0].szFileID , _T("CheckBaud"));
		_tcscpy(fileInfo1[0].szFileType , _T("CheckBaud"));
		fileInfo1[0].bChangeCode = FALSE;
		fileInfo1[0].bLoadCodeFromFile = FALSE;
		fileInfo1[0].dwBase = 0; 
		pMF->m_StepDescription.Add(_T("CheckBaud"));
		
		_tcscpy(fileInfo1[1].szFileName, strFdl);
		_tcscpy(fileInfo1[1].szFileID , _T("FDLA"));
		_tcscpy(fileInfo1[1].szFileType , _T("FDL1"));
		//fileInfo1[1].bChangeCode = FALSE;
		fileInfo1[1].bLoadCodeFromFile = TRUE;
		fileInfo1[1].dwBase = m_dwNandFdlAddr;
		pMF->m_StepDescription.Add(_T("FDL1"));

		bRet = StartBootModeWork(nPort, nBaudrate, (DWORD)fileInfo1, 2);
	}
	Sleep(theApp.m_dwEnumPortTimeOut);
	//FDL2
	if (bRet)
	{
		pMF->m_StepDescription.RemoveAll();
		pMF->m_nFileStep = 0;
		pMF->m_nStage = 0;
		_tcscpy(fileInfo2[0].szFileID , _T("CheckBaud"));
		_tcscpy(fileInfo2[0].szFileType , _T("CheckBaud"));
		fileInfo2[0].bChangeCode = FALSE;
		fileInfo2[0].bLoadCodeFromFile = FALSE;
		fileInfo2[0].dwBase = 0; 
		pMF->m_StepDescription.Add(_T("CheckBauds"));
		
		_tcscpy(fileInfo2[1].szFileName, strFdl2);
		_tcscpy(fileInfo2[1].szFileID , _T("FDLB"));
		_tcscpy(fileInfo2[1].szFileType , _T("FDL2"));
		//fileInfo2[1].bChangeCode = FALSE;
		fileInfo2[1].bLoadCodeFromFile = TRUE;
		fileInfo2[1].dwBase = m_dwNandFdl2Addr;
		pMF->m_StepDescription.Add(_T("FDL2"));
		

		fileInfo2[2].dwBase = dwBase.dwAddr;
        _tcscpy(fileInfo2[2].szRepID,dwBase.szAddr);
        fileInfo2[2].bLoadCodeFromFile = FALSE;
        fileInfo2[2].dwOprSize = dwSize;
        fileInfo2[2].lpCode = NULL;
		if(m_bNvBaseStr)
		{
			_tcscpy(fileInfo2[2].szFileType, _T("ReadFlash2") ) ;
		}
		else
		{
			_tcscpy(fileInfo2[2].szFileType, _T("ReadFlash") ) ;
		}
		_tcscpy(fileInfo2[2].szFileID , _T("ReadFlash") );
		
		bRet = StartBootModeWork(nPort, nBaudrate, (DWORD)fileInfo2, 3);

	}

	return bRet;
	


}
/*CR212808 xmzhang modify begin*/
BOOL CNVEditorView::GetFlashContent(
									 int nPort,
									 int nBaudrate,
									 const CString& strFdl,
									 NvBaseAddr dwBase,
									 DWORD dwSize,
                                     CString strFdl2
									 )
{
    BMFileInfo fileInfo[ 3 ];

	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();
	pMF->m_StepDescription.RemoveAll();
	pMF->m_nFileStep = 0;
	pMF->m_nStage = 0;
	
    
    if( strFdl2.IsEmpty() )
    {
        _tcscpy(fileInfo[0].szFileName, strFdl );
		fileInfo[0].dwBase = m_dwNandFdlAddr;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy( fileInfo[0].szFileID , _T("FDL") );
		pMF->m_StepDescription.Add(_T("FDL"));
    
        fileInfo[1].dwBase = dwBase.dwAddr;
        _tcscpy(fileInfo[1].szRepID,dwBase.szAddr);
        fileInfo[1].bLoadCodeFromFile = FALSE;
        fileInfo[1].dwOprSize = dwSize;
        fileInfo[1].lpCode = NULL;
		
		if(m_bNvBaseStr)
		{
			_tcscpy(fileInfo[1].szFileType, _T("ReadFlash2") ) ;  
		}
		else
		{
			_tcscpy(fileInfo[1].szFileType, _T("ReadFlash") ) ;  
		} 
		_tcscpy( fileInfo[1].szFileID , _T("ReadFlash") );
		pMF->m_StepDescription.Add(_T("ReadFlash"));

        return StartBootModeWork( nPort, nBaudrate, (DWORD)fileInfo, 2 );
    }
    else
    {
        _tcscpy(fileInfo[0].szFileName, strFdl );
        fileInfo[0].dwBase = m_dwNandFdlAddr;
        fileInfo[0].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[0].szFileType , _T("FDL") );
		_tcscpy( fileInfo[0].szFileID , _T("FDL") );
		pMF->m_StepDescription.Add(_T("FDL"));
        
        _tcscpy(fileInfo[1].szFileName, m_strFdl2 );
        fileInfo[1].dwBase = m_dwNandFdl2Addr;
        fileInfo[1].bLoadCodeFromFile = TRUE;
        _tcscpy( fileInfo[1].szFileType , _T("Nand_FDL") );
		_tcscpy( fileInfo[1].szFileID , _T("FDL2") );
		pMF->m_StepDescription.Add(_T("FDL2"));
        
        fileInfo[2].dwBase = dwBase.dwAddr;
        _tcscpy(fileInfo[2].szRepID,dwBase.szAddr);
        fileInfo[2].bLoadCodeFromFile = FALSE;
        fileInfo[2].dwOprSize = dwSize;
        fileInfo[2].lpCode = NULL;
		if(m_bNvBaseStr)
		{
			_tcscpy(fileInfo[2].szFileType, _T("ReadFlash2") ) ;
		}
		else
		{
			_tcscpy(fileInfo[2].szFileType, _T("ReadFlash") ) ;
		}
		_tcscpy(fileInfo[2].szFileID , _T("ReadFlash") );
		pMF->m_StepDescription.Add(_T("ReadFlash"));

        return StartBootModeWork( nPort, nBaudrate, (DWORD)fileInfo, 3 );
    }
}
/*CR212808 xmzhang modify end*/

const void* CNVEditorView::GetNVItemFromBlock( const unsigned char *pBlock,
										       DWORD cbBlockLen,
										       WORD wID, 
										       WORD *pLen )
{
	ASSERT( NULL != pBlock );
	ASSERT( NULL != pLen );

	DWORD nOffset = 0;
	// Insert item into CFileParser
	// Skip time stamp( 4 bytes long )
	nOffset += 4;

	static const WORD NV_END = 0xffff;
	while ( nOffset < cbBlockLen )
	{
		WORD wItemId = *( WORD* )( pBlock + nOffset );
		if ( NV_END == wItemId )
		{
			break;
		}

		CONVERT_SHORT2( wItemId, wItemId );
		
		nOffset += sizeof( WORD );
		if ( nOffset >= cbBlockLen )
		{
			return NULL;
		}

		WORD wLen = *( WORD* )( pBlock + nOffset );
		CONVERT_SHORT2( wLen, wLen );

		nOffset += sizeof( WORD );
		if ( nOffset + wLen > cbBlockLen )
		{
			return NULL;
		}

		if ( wItemId == wID )
		{
			*pLen = wLen;
			return ( pBlock + nOffset );
		}
		else
		{
			nOffset += wLen;
		    // Adjust for alignment of 4 bytes
		    nOffset = ( nOffset + 3 ) & 0xfffffffc ;
		}
	}

	return NULL;
}

void CNVEditorView::OnReadFlash() 
{
	// If m_strFdlFileName is empty,
	// we must intialize it first.
	if ( m_strFdlFileName.IsEmpty() )
	{
		if ( !PortSettings( FALSE ) )
		{
			return;
		}
	}

	BOOL bNand = TRUE;
	CString strTemp = m_strSelProduct;
	strTemp.MakeLower();
	if(strTemp.Find( _T("nand") ) == -1)
	{
		bNand = FALSE;
	}
	else
	{
		if(m_strFdl2.IsEmpty())
		{
			if ( !PortSettings( FALSE ) )
			{
				return;
			}
		}
	}

	static CReadFlashDlg dlg;
 
	if ( IDOK != dlg.DoModal() )
	{
		return;
	}

	FILE *pFile = ::_tfopen( dlg.m_strFilePath, _T("wb") );
	if ( NULL == pFile )
	{
		::AfxMessageBox( _T("Open file fail") );
		return;
	}

    CWaitCursor wait;
    NvBaseAddr StartAddr;
    StartAddr.dwAddr = dlg.m_dwStartAddr;

	BOOL bSuccess = FALSE;
	CheckPort2ndEnum();
	if (m_bPortSecondEnum)
	{
		bSuccess = GetFlashContentEx( m_nPort,
			m_nBaudrate,
			m_strFdlFileName,
			StartAddr, 
			dlg.m_dwBlockSize,
			m_strFdl2
			) ;
	}
	else
	{
		bSuccess = GetFlashContent( m_nPort,
			m_nBaudrate,
			m_strFdlFileName,
			StartAddr, 
			dlg.m_dwBlockSize,
			bNand?m_strFdl2:_T("")
			) ;
	}

	if ( NULL == m_lpReadBuffer || !bSuccess )
	{
        ::fclose( pFile );
		 return;
	}
	
	DWORD dwLen = ::fwrite( m_lpReadBuffer, 1, dlg.m_dwBlockSize, pFile );
	::fclose( pFile );

	CString strErr;
	strErr.Format( _T("total %d bytes written to file"),
		           dwLen  );
	::AfxMessageBox( strErr );

	return;
}

void CNVEditorView::LoadNVItemFromImage( const unsigned char *pImage,
							             DWORD cbImageLen )
{
	ASSERT( NULL != m_pProject );

	m_strLoadFailInfo.Empty();
	BOOL  bFailFlag = FALSE;
	m_bLoadNV = TRUE;
	m_pProject->ClearNVLoadState();

	CWaitCursor wc;

	int nCount = m_pProject->GetNVItemCount();
	int nReadFail = 0;
	int nInterFail = 0;
	theApp.GetPermission();

	for( int i=0; i<nCount; i++ )
	{
		WORD wLen;
		WORD wItemId = (WORD)m_pProject->GetNVItemID( i );
		
		const void *pItem = GetNVItemFromBlock( pImage, 
			                                    cbImageLen, 
												wItemId, 
												&wLen );
		if ( NULL == pItem )
		{
			// if an error occured, we just report it 
			// and continue
			CString strFailedItem;
			strFailedItem.Format(_T("Can't get NV Item: Id(%d),  Name(%s)\n"), wItemId, (CString)m_pProject->GetNVItemName(i));
			m_strLoadFailInfo += strFailedItem;

			m_pProject->SetNVLoadState(i, TRUE);

			bFailFlag = TRUE;

			CString strErr;
			strErr.Format( _T("Can't get NV Item %d from module.\n")
				           _T("The project file is corrupted\nor not compatible ")
						   _T("with the nv items in the module.\n")
						   _T("Click OK to continue."), 
				           wItemId ); 
			if(AfxMessageBox( strErr,MB_OKCANCEL ) == IDOK)
			{
				wc.Restore();
				nReadFail++;
				continue;
			}
			else
			{
				break;
			}
		}

		if ( !m_pProject->WriteNVItem( wItemId,
			                           ( void* )pItem,
									   wLen,
									   TRUE,FALSE) )
		{
			// if an error occured, we just report it
			// and continue
			CString strErr;
			strErr.Format( _T("Interpret Nv Item %d fail!\n")
				           _T("The project file is corrupted\nor not compatible ")
						   _T("with the nv items in the module.\n")
						   _T("Click OK to continue."), 
						   wItemId );
			if(AfxMessageBox( strErr,MB_OKCANCEL ) == IDOK)
			{
				wc.Restore();
				nReadFail++;
				continue;
			}
			else
			{
				break;
			}
		}
	}

	CString strErr;
	strErr.Format( _T("Total %d items\nRead fail: %d items\nInterpret fail: %d items"),
		           nCount, nReadFail, nInterFail );
	::AfxMessageBox( _T("Load NV Items Over!\n") + strErr );
}


BOOL CNVEditorView::SaveToImage( BOOL fWriteDsp, 
								 LPCTSTR lpszDspFilePath )
{
	ASSERT( !fWriteDsp || ( NULL != lpszDspFilePath ) );

	m_pProject->GetWriteTailFlag(); // add by Joly for bin Tail Flag

	m_pProject->SetWriteDsp( fWriteDsp );

	if ( NULL != lpszDspFilePath )
	{
		m_strDspFileName = lpszDspFilePath;
	}

#ifdef _FOR_CUSTOMER

	TCHAR szPath[ MAX_PATH ];
	if ( 0 == ::GetModuleFileName( NULL, szPath, sizeof szPath ) )
		return FALSE;

	TCHAR *p = _tcsrchr( szPath, _T('\\') );
	ASSERT( NULL != p );
	if ( p == NULL )
		return FALSE;

	*( p + 1 ) = _T('\0');

#define SCI_NV			_T("scinv.bin")
	::_tcscat( szPath, SCI_NV );

	CFileMap map( szPath );
	const void * lpAppendage = map;
	if ( NULL == lpAppendage )
	{
		CString strErr;
		strErr.Format( _T("Open file %s failed"), szPath );
		::AfxMessageBox( strErr );
		return FALSE;
	}

	return m_pProject->SaveToImage( m_strDspFileName,
		                            lpAppendage,
									map.GetSize() );

#else

	return m_pProject->SaveToImage( m_strDspFileName );

#endif // _FOR_CUSTOMER

}

void CNVEditorView::OnLoadNVValue() 
{	
    CFileDialog dlg( TRUE,        
                     _T("*.nvv"),            
                     NULL,
                     OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER,
                     _T("NV Value Files (*.nvv)|*.nvv||"),
                     this );

    if ( IDOK != dlg.DoModal() )
        return;

    CFileMap map( dlg.GetPathName() );
    const void * pImage = map;
    if ( NULL == pImage )
    {
        CString strErr;
        strErr.Format( _T("Can not open file %s"), dlg.GetPathName() );
        ::AfxMessageBox( strErr );
        return;
    }

    const NVVALUE_FILE_HEADER * pHeader = ( const NVVALUE_FILE_HEADER* )pImage;
    if ( NVVALUE_FILE_SIGNATURE != pHeader->Magic )
    {
        CString strErr;
        strErr.Format( _T("%s is not a valid nv value file"), dlg.GetPathName() );
        ::AfxMessageBox( strErr );
        return;
    }

    CString strFileInfo;
    if ( ( 0 != pHeader->TimeDateStamp ) && ( '\0' != pHeader->Machine[0] ) )
    {
        /*lint -save -e578 */
        struct tm * time = ::localtime( ( const time_t* )&( pHeader->TimeDateStamp ) );
        /*lint -restore */
        ASSERT( NULL != time );

        strFileInfo.Format( _T("The file is created on %s at %04d-%02d-%02d %02d:%02d:%02d"),
                            pHeader->Machine,
                            time->tm_year + 1900,
                            time->tm_mon,
                            time->tm_mday,
                            time->tm_hour,
                            time->tm_min,
                            time->tm_sec );
    }
    else
    {
        strFileInfo.Format( _T("%s"), _T("The file do not have creation information") );
    }

    CString strInfo;
    strInfo.Format( _T("%s\nAre you sure to continue?"), strFileInfo );
    if ( IDOK != ::AfxMessageBox( strInfo, MB_OKCANCEL ) )
        return;
    
    LoadNVValue( pImage );
}

void CNVEditorView::LoadNVValue( const void * pImage )
{
    ASSERT( NULL != pImage );

    WORD wVer = ((const NVVALUE_FILE_HEADER*)pImage)->Version;

    if( wVer != VERSION_1 && wVer != VERSION_2 )
    {
        CString strError;
        strError.Format( _T("Unsurpport version %d!"),wVer );
        AfxMessageBox( strError );
        return;
    }

    DWORD NumberOfSections = ((const NVVALUE_FILE_HEADER*)pImage)->NumberOfSections;

    const NVVALUE_SECTION_ENTRY * pSection =  SECTION_TABLE( pImage );
    for ( DWORD i=0; i<NumberOfSections; ++i, ++pSection )
    {
        BOOL bRet;
        if( wVer == VERSION_1 )
        {
            bRet = LoadNVValueSection( NVDIR_NUM( pSection ),
                                          NVDIR_TABLE( pImage, pSection ),
                                          NVVALUE_NUM( pSection ),
                                          NVVALUE_TABLE( pImage, pSection ) );
        }
        else // if( wVer == VERSION_2 )
        {
            bRet = LoadNVValueSection( NVDIR_NUM( pSection ),
                NVDIR_TABLE_EX( pImage, pSection ),
                NVVALUE_NUM( pSection ),
                NVVALUE_TABLE_EX( pImage, pSection ) );
        }

        if( !bRet )
        {
            ::AfxMessageBox( _T("The NV value file is not compitable with the NV project") );
            return;
        }

    }
}

BOOL CNVEditorView::LoadNVValueSection( int                         nNumberOfDirs,
                                        const char               *  pDir,
                                        int                         nNumberOfValues,
                                        const NVVALUE_TABLE_ENTRY * pValue )
{
    ASSERT( 0 < nNumberOfDirs );
    ASSERT( NULL != pDir );
    ASSERT( 0 < nNumberOfValues );
    ASSERT( NULL != pValue );

    return m_List.LoadNVValue( nNumberOfDirs, pDir, nNumberOfValues, pValue );
}

BOOL CNVEditorView::CreateBootModeObj()
{
    CString strErrMsg;    
    m_pBootModeObj = NULL;

	if(!CreateBMObject(&m_pBootModeObj))
	{
		strErrMsg.LoadString( IDS_CREATE_BM_FAIL );
        AfxMessageBox( strErrMsg );
        return FALSE; 
	}

	m_pBootModeObj->SubscribeOperationObserver(&m_xBMOprObserver, NULL, &m_dwCookie);     

    m_hFinishEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    return TRUE;
}

BOOL CNVEditorView::DestroyBootModeObj()
{
    if( m_pBootModeObj == NULL )
        return FALSE;
    
    m_pBootModeObj->UnsubscribeOperationObserver( m_dwCookie  );
	ReleaseBMObject(m_pBootModeObj);
	m_pBootModeObj = NULL;
    
    if( m_hFinishEvent != NULL )
    {
        CloseHandle( m_hFinishEvent );
    }
    return TRUE;
}
  
BOOL CNVEditorView::StartBootModeWork( DWORD dwPort, DWORD dwBaud, 
                                      DWORD lpDLFileInfo, UINT uFileCount )
{
	USES_CONVERSION;

    if( m_pBootModeObj == NULL)
    {
        AfxMessageBox( _T("Invalid Operation!") );
        return FALSE;
    }

	CMainFrame * pMF = (CMainFrame*)AfxGetMainWnd();
	pMF->m_nPort = (int)dwPort;
	pMF->m_dlgBMLoader.SetProg(_T(""));
	pMF->m_dlgBMLoader.SetState(_T(""));
	pMF->m_dlgBMLoader.SetProg(_T(""));
	pMF->m_dlgBMLoader.m_nState = -1;


    CWaitCursor wait;
    CString strErrMsg;
    HRESULT hr = 0;            
    BYTE btOpenArgument[ 8 ];
    *(DWORD *)&btOpenArgument[ 0 ] = dwPort;
    *(DWORD *)&btOpenArgument[ 4 ] = dwBaud; 
    
    //@ Liu kai 2005-9-28
    // Set checkbaud times.
    IBMSettings * pSettings = (IBMSettings *)m_pBootModeObj;  
    pSettings->SetCheckBaudTimes( m_nCheckBaudTimes );
	//@ hongliang.xin 2010-2-5
	pSettings->SetProperty(0,_T("PRODUCT"), (LPVOID)(LPTSTR)(LPCTSTR)m_strSelProduct);

	pSettings->SetProperty(0,_T("EnablePortSecondEnum"), (LPVOID)&m_bPortSecondEnum);// 下载端口二次枚举功能
	
	/*CR212808 xmzhang modify begin*/
	if(m_bNvBaseStr)
	{
		pSettings->SetRepartitionFlag( REPAR_STRATEGY_DO_ALWAYS );
	}
	/*CR212808 xmzhang modify end*/
    
    
    //ResetEvent( m_hFinishEvent );  
    m_bOprSuccess = FALSE; 
    m_strErrDesp.Empty();
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
    hr = m_pBootModeObj->StartBootModeOperation( (LPVOID)lpDLFileInfo, 
		uFileCount,
        (LPVOID)btOpenArgument, 
		TRUE, 
		dwPort, 
		FALSE, 
        (LPVOID)&hwnd, 
		L"Channel.Uart" );    

    if(hr == (HRESULT)BM_E_CREATETHREAD_FAILED)
	{
        strErrMsg.LoadString( IDS_CREATETHREAD_FAIL );
        AfxMessageBox( strErrMsg );
        return FALSE;     
	}
    else if(hr == BM_E_FILEINFO_ERROR)
	{
        strErrMsg.LoadString( IDS_FILEINFO_ERROR );
        AfxMessageBox( strErrMsg );
        return FALSE;   
	}
    else if(hr == BM_E_CHANNEL_FAILED)
	{
        strErrMsg.LoadString( IDS_CHANNEL_ERROR );
        AfxMessageBox( strErrMsg );
        return FALSE;    
	}
    else if(hr == BM_E_OPR_NOTREG)
	{
        strErrMsg.LoadString( IDS_BMOPR_NOTREG );
        AfxMessageBox( strErrMsg );
        return FALSE;      
	}
    else if(hr == BM_E_FAILED)
	{
        strErrMsg.LoadString( IDS_START_FAIL );
        AfxMessageBox( strErrMsg );
        return FALSE;         
	}
    else
	{
        if( SUCCEEDED( hr ) )
        {
            //WaitForSingleObject( m_hFinishEvent, INFINITE );
			pMF->m_dlgBMLoader.DoModal();

            if( m_bOprSuccess )
            {
                //AfxMessageBox( _T("Operation Success.") );

            }
            else
            {
                if( m_strErrDesp.IsEmpty() )
                {
                    AfxMessageBox( _T("Operation Failed."));
                }
                else
                {
                    AfxMessageBox( m_strErrDesp );
                }
            }
            StopBootModeWork();
            return m_bOprSuccess;
        }		
	}       


    return FALSE;    
}


BOOL CNVEditorView::StopBootModeWork()
{
    if( m_pBootModeObj == NULL )
        return FALSE;
    CWaitCursor wait;
    m_pBootModeObj->StopBootModeOperation();
    return TRUE;
}


HRESULT CBMOprObserver::OnOperationStart(   DWORD dwOprCookie, 
		LPCWSTR cbstrFileID,
		LPCWSTR cbstrFileType,		
		LPCWSTR cbstrOperationType,
		LPVOID pBMFileInterface )
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( cbstrFileType );
    UNUSED_ALWAYS( cbstrOperationType );
    UNUSED_ALWAYS( pBMFileInterface );
    UNUSED_ALWAYS( cbstrFileID );
    return S_OK;
}

HRESULT CBMOprObserver::OnOperationEnd(  DWORD dwOprCookie, 
		LPCWSTR cbstrFileID,
		LPCWSTR cbstrFileType, 
		LPCWSTR cbstrOperationType, 
		DWORD dwResult,
		LPVOID pBMFileInterface )
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( cbstrFileType );
    UNUSED_ALWAYS( cbstrOperationType );
    UNUSED_ALWAYS( dwResult );
    UNUSED_ALWAYS( cbstrFileID );
	UNUSED_ALWAYS( pBMFileInterface );


	BOOL bSuccess = (BOOL)(dwResult == OPR_SUCCESS);
    if( !bSuccess )
    {
        pThis->m_strErrDesp.Format( _T("%s operation Failed at %s."), 
               cbstrOperationType,  cbstrFileType);
    }
    /*CR212808 xmzhang modify begin*/
    if( bSuccess && 
        _tcsnicmp(cbstrFileType, _T("ReadFlash"),9) == 0 &&
        _tcsnicmp( cbstrOperationType,_T("ReadFlash"),9) == 0 )
    /*CR212808 xmzhang modify begin*/
    {
        LPBYTE lpReadBuffer = NULL; 
        DWORD dwReadSize = 0;

		lpReadBuffer =  pThis->m_pBootModeObj->GetReadBuffer();
		dwReadSize  =   pThis->m_pBootModeObj->GetReadBufferSize();    
        pThis->AllocRecvBuffer( lpReadBuffer,  dwReadSize );
    }

    return S_OK;
}

HRESULT CBMOprObserver::OnFileOprStart( DWORD dwOprCookie,
		LPCWSTR cbstrFileID, 
		LPCWSTR cbstrFileType , 
		LPVOID pBMFileInterface )
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( cbstrFileType );
    UNUSED_ALWAYS( pBMFileInterface );
    UNUSED_ALWAYS( cbstrFileID );


	::PostMessage( AfxGetMainWnd()->GetSafeHwnd(), 
		BM_FILE_BEGIN, dwOprCookie, 0 );

    return S_OK;
}

HRESULT CBMOprObserver::OnFileOprEnd(  DWORD dwOprCookie, 
		LPCTSTR cbstrFileID,
		LPCTSTR cbstrFileType, 
		DWORD dwResult )
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( cbstrFileType );
    UNUSED_ALWAYS( dwResult );
    UNUSED_ALWAYS( cbstrFileID );
    
    if( pThis->m_strErrDesp.IsEmpty() )
    {
        if( dwResult != OPR_SUCCESS )
        {
            pThis->m_strErrDesp.Format(_T("Operation Failed at %s."), cbstrFileType);
        }        
    }

    return S_OK;
}

HRESULT CBMOprObserver::OnEnd( THIS_ DWORD dwOprCookie, DWORD dwResult )
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( dwResult );

	LPARAM lParam = NULL;
    if( dwResult != OPR_SUCCESS )
    {
        lParam = (LPARAM)(LPCTSTR)pThis->m_strErrDesp;
    }

	pThis->m_bOprSuccess = (BOOL)(dwResult==OPR_SUCCESS);

	::PostMessage( AfxGetMainWnd()->GetSafeHwnd(), 
		BM_END, dwOprCookie, lParam );

	if (pThis->m_bOprSuccess)
	{
		CMainFrame * pMF = (CMainFrame*)AfxGetMainWnd();
	    ::PostMessage(pMF->m_dlgBMLoader.GetSafeHwnd(), WM_CLOSE, 0, 0);
	}
    
    //SetEvent( pThis->m_hFinishEvent );


    return S_OK;
}

HRESULT CBMOprObserver::OnStart( THIS_ DWORD dwOprCookie, DWORD dwResult )
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( dwResult );

	::PostMessage( AfxGetMainWnd()->GetSafeHwnd(),
		BM_BEGIN, dwOprCookie, 0 );

    return S_OK;
}

HRESULT CBMOprObserver::OnFilePrepare(   DWORD dwOprCookie,
		LPCWSTR bstrProduct,
		LPCWSTR bstrFileName,
		LPVOID  lpFileInfo,
		LPVOID   pBMFileInfoArr,
		LPDWORD  lpBMFileInfoCount,
		LPDWORD  lpdwFlag)
{
    UNUSED_ALWAYS( dwOprCookie );
    UNUSED_ALWAYS( bstrProduct );
    UNUSED_ALWAYS( bstrFileName );
    UNUSED_ALWAYS( lpFileInfo );
    UNUSED_ALWAYS( pBMFileInfoArr );
    UNUSED_ALWAYS( lpBMFileInfoCount );
    UNUSED_ALWAYS( lpdwFlag );

    return S_OK;
}

void CNVEditorView::OnImportItem() 
{
	// Open file for read first
    static TCHAR BASED_CODE szFilter[] = _T("NV Import Files (*.nv)|*.nv|All files (*.*)|*.*||");
	
	CFileDialog dlg(TRUE, _T("nv"), NULL, OFN_FILEMUSTEXIST, szFilter, NULL);
    if( IDOK != dlg.DoModal() )
    {
        // User cancel
        return;
    }
	
	m_List.ImportItem( dlg.GetPathName() );
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);
}

void CNVEditorView::OnFuncitions() 
{
	// TODO: Add your command handler code here
	CDlgFunctions dlg;
	dlg.m_pList = &m_List;
	dlg.m_pFucList = &m_vFuncs;

	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
}

void  CNVEditorView::ClearFuncs()
{
	int nSize = m_vFuncs.size();
	for(int i= 0; i< nSize; i++)
	{
		delete (FUNC_ITEM_PTR)m_vFuncs[i];
	}
	m_vFuncs.clear();

	m_pClearCaliFlag = NULL;
}

#define MAX_BUFF  (8*1024)


BOOL CNVEditorView::LoadFuncConfig(LPCTSTR lpszFunIni)
{
	ClearFuncs();

	TCHAR szBuf[MAX_BUFF] = {0};
	GetPrivateProfileSection(_T("Functions"),szBuf,MAX_BUFF,lpszFunIni);
	CStringArray agKeys;

	int nCount = EnumKeys(szBuf,&agKeys);

	for(int i=0; i< nCount; i++)
	{
		FUNC_ITEM_PTR pFuncItem = new FUNC_ITEM_T;
		CString strName = agKeys[2*i];
		CString strDesc = agKeys[2*i+1];
		_tcscpy(pFuncItem->szFuncName,strName);
		_tcscpy(pFuncItem->szFuncDesc,strDesc);

		TCHAR szBuf2[MAX_BUFF] = {0};
		GetPrivateProfileSection(strName,szBuf2,MAX_BUFF,lpszFunIni);
		int nCount2 = EnumKeys(szBuf2,&pFuncItem->agRules);
		if(nCount2 == 0)
		{
			delete pFuncItem;
		}
		else
		{
			m_vFuncs.push_back(pFuncItem);
			if(strName.CompareNoCase(_T("ClearCalibrationFlag")) == 0)
			{
				m_pClearCaliFlag = pFuncItem;
			}
		}		
	}

	return TRUE;
}

void CNVEditorView::OnFind() 
{
	// TODO: Add your command handler code here
	
	if(m_dlgFind.DoModal()== IDCANCEL)
	{
		return;
	}

	m_List.OnFind(m_dlgFind.m_strTarget,m_dlgFind.m_dwFlag);
}

void CNVEditorView::OnFindNext() 
{
	// TODO: Add your command handler code here
	m_List.OnFindNext(m_dlgFind.m_strTarget,m_dlgFind.m_dwFlag);
}

DWORD WINAPI CNVEditorView::GetReadWorkThread( LPVOID lpParam )
{    
    CNVEditorView * pThis = (CNVEditorView*)lpParam;

    return pThis->ReadWorkThreadProc( );
}

DWORD  CNVEditorView::ReadWorkThreadProc( )
{
    CString strInfo;
	CString strErr;

	//add for load nv failed log mark
	m_bLoadNV = TRUE;
	m_pProject->ClearNVLoadState();

	
	LPCTSTR pLog = NULL;
	
	// wait m_dlgFeedBack init
	while(  m_dlgFeedBack.GetSafeHwnd() == NULL || !m_dlgFeedBack.m_bInit )
	{
		Sleep(0);
	}

	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(1000);

	while( m_nCurrCount < m_nNVItemTotalCount &&  !m_bUserCancel  )
    {
		strErr.Empty();
		pLog = NULL;
		uint16 nItemId = static_cast<uint16>( m_pProject->GetNVItemID( m_nCurrCount ) );        
		
		LPBYTE pBuf = NULL;
		DWORD  dwSize = 0;
		
		DWORD dwRet = m_DiagObj.ReadNVItem(nItemId,&pBuf,dwSize);
		
		if( dwRet == DIAG_SUCCESS)
		{
			m_aNVItems.Add( pBuf );
			m_nCurrCount++;
		}
		else
		{
			if( DIAG_E_NOT_CONNECTED == dwRet || DIAG_E_CHANNEL_ERROR == dwRet)
			{
				strErr.Format( _T("Error: Send read-request fail"));
				strInfo=_T("KEEP");	
				if(m_dlgFeedBack.IsWindowEnabled())
				{
					m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, strErr.operator LPCTSTR());
				}
				goto CSD_ERROR;
			}

			//[[ retry
			dwRet = m_DiagObj.ReadNVItem(nItemId,&pBuf,dwSize);
		
			if( dwRet == DIAG_SUCCESS)
			{
				m_aNVItems.Add( pBuf );
				m_nCurrCount++;
			}
			else
			{
				if( DIAG_E_NOT_CONNECTED == dwRet || DIAG_E_CHANNEL_ERROR == dwRet)
				{
					strErr.Format( _T("Error: Send read-request fail"));
					strInfo=_T("KEEP");	
					if(m_dlgFeedBack.IsWindowEnabled())
					{
						m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, strErr.operator LPCTSTR());
					}
					goto CSD_ERROR;
				}
			
			
				strErr.Format( _T("Get item %s (ID %d) from Module fail."),
					(CString)m_pProject->GetNVItemName(m_nCurrCount),
					m_pProject->GetNVItemID(m_nCurrCount));

				m_pProject->SetNVLoadState(m_nCurrCount, TRUE);

				m_strLoadFailInfo += strErr;
				m_strLoadFailInfo += _T("\n");
				
				m_nCurrCount++;
				m_nReadFail++;
			}
			//]]
		}
		
		// Update the feedback dialog
		strInfo.Format( _T("%d items received.(Total %d items)"),
			m_nCurrCount,
			m_nNVItemTotalCount );
		
		if(!strErr.IsEmpty())
		{
			pLog = strErr.operator LPCTSTR();
		}		
	
		m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, pLog);		
    }
	
	
	if(m_bUserCancel &&  m_nCurrCount < m_nNVItemTotalCount)
	{
		return 0;
	}
	
    // All data have been got
    // Update feedback, disable Cancel buttons
    strInfo = _T("Refreshing... Please Waiting!");
    m_dlgFeedBack.UpdateFeedBack( strErr, m_nCurrCount, TRUE );
	
    // write them to m_pProject
    if( !WriteAllNVItems() )
    {
        strErr =  _T("Update NVItems fail");
		strInfo=_T("KEEP");				
		m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, strErr.operator LPCTSTR());		
        goto CSD_ERROR;
    }
	
	
    m_List.Refresh(); // Refresh m_List   
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
	
    if ( m_dlgFeedBack.GetSafeHwnd() != NULL )
    {
		::PostMessage(m_dlgFeedBack.GetSafeHwnd(),WM_UPDTAE_STATE,IDOK,0);
    }  
	
    return 0;
	
CSD_ERROR:
	if ( m_dlgFeedBack.GetSafeHwnd() != NULL )
	{
		::PostMessage(m_dlgFeedBack.GetSafeHwnd(),WM_UPDTAE_STATE,IDABORT,0);
	} 
    return 0;
}

DWORD WINAPI CNVEditorView::GetWriteWorkThread( LPVOID lpParam )
{    
    CNVEditorView * pThis = (CNVEditorView*)lpParam;

    return pThis->WriteWorkThreadProc( );
}

DWORD  CNVEditorView::WriteWorkThreadProc( )
{
    CString strInfo;
	CString strErr;
	
	LPCTSTR pLog = NULL;
	
	// wait m_dlgFeedBack init
	while(  m_dlgFeedBack.GetSafeHwnd() == NULL || !m_dlgFeedBack.m_bInit )
	{
		Sleep(0);
	}

	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(1000);
#define MAX_NVITEM_LEN 64*1024+2

	LPBYTE pBuf = new BYTE[MAX_NVITEM_LEN];

// 	if(FALSE == m_pProject->ReCalcNVStruct())
// 	{
// 		goto WWT_ERROR;
// 	}

	
	while( m_nCurrCount < m_nNVItemTotalCount &&  !m_bUserCancel  )
    {
		strErr.Empty();
		pLog = NULL;
		memset(pBuf,0,MAX_NVITEM_LEN);
		uint16 nItemId = m_vecNVID[m_nCurrCount];
		//uint16 nItemId = static_cast<uint16>( m_pProject->GetNVItemID( m_nCurrCount ) );  

		DWORD dwSize = 0;
		m_pProject->ReadNVItem(nItemId,pBuf+2,MAX_NVITEM_LEN,&dwSize,g_bBigEndian,FALSE);

		uint16 nItemIdTmp = nItemId;
		CONVERT_SHORT2(nItemIdTmp,nItemIdTmp);
		*((uint16*)pBuf) = nItemIdTmp;
		dwSize += 2; // add ID
		DWORD dwRet = m_DiagObj.WriteNVItem(nItemId,pBuf,dwSize);

		
		if( dwRet == DIAG_SUCCESS)
		{
			m_nCurrCount++;
		}
		else
		{
			if( DIAG_E_NOT_CONNECTED == dwRet || DIAG_E_CHANNEL_ERROR == dwRet)
			{
				strErr.Format( _T("Error: Send write-request fail"));
				strInfo=_T("KEEP");	
				if(m_dlgFeedBack.IsWindowEnabled())
				{
					m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, strErr.operator LPCTSTR());
				}
				goto WWT_ERROR;
			}

			//[[ retry
			dwRet = m_DiagObj.WriteNVItem(nItemId,pBuf,dwSize);
		
			if( dwRet == DIAG_SUCCESS)
			{	
				m_nCurrCount++;
			}
			else
			{
				if( DIAG_E_NOT_CONNECTED == dwRet || DIAG_E_CHANNEL_ERROR == dwRet)
				{
					strErr.Format( _T("Error: Send write-request fail"));
					strInfo=_T("KEEP");	
					if(m_dlgFeedBack.IsWindowEnabled())
					{
						m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, strErr.operator LPCTSTR());
					}
					goto WWT_ERROR;
				}
			
			
				strErr.Format( _T("write item %s(ID %d) to phone fail."),
					m_pProject->GetNVItemName( m_nCurrCount ),
					m_pProject->GetNVItemID( m_nCurrCount ) );
				
				m_nCurrCount++;
				m_nWriteFail++;
			}
			//]]
		}
		
		// Update the feedback dialog
		strInfo.Format( _T("%d items sended.(Total %d items)"),
			m_nCurrCount,
			m_nNVItemTotalCount );
		
		if(!strErr.IsEmpty())
		{
			pLog = strErr.operator LPCTSTR();
		}		
	
		m_dlgFeedBack.UpdateFeedBack( strInfo, m_nCurrCount,FALSE, pLog);	
		
		Sleep(1);
    }

	delete [] pBuf;
	pBuf = NULL;	
	
	if(m_bUserCancel &&  m_nCurrCount < m_nNVItemTotalCount)
	{
		return 0;
	}
	
    // All data have been got
    // Update feedback, disable Cancel buttons
    //strInfo = _T("Refreshing... Please Waiting!");
    //m_dlgFeedBack.UpdateFeedBack( strErr, m_nCurrCount, TRUE );
	
    if ( m_dlgFeedBack.GetSafeHwnd() != NULL )
    {
		::PostMessage(m_dlgFeedBack.GetSafeHwnd(),WM_UPDTAE_STATE,IDOK,0);
    }  	
    
	return 0;
	
WWT_ERROR:

	if(pBuf != NULL)
	{
		delete [] pBuf;
		pBuf= NULL;
	}

	if ( m_dlgFeedBack.GetSafeHwnd() != NULL )
	{
		::PostMessage(m_dlgFeedBack.GetSafeHwnd(),WM_UPDTAE_STATE,IDABORT,0);
	} 
    return 0;
}

void CNVEditorView::OnDisplayHex() 
{
	// TODO: Add your command handler code here
	m_bDisplayHex = !m_bDisplayHex;
	m_List.SetDispHex(m_bDisplayHex);
}

void CNVEditorView::OnBMStopMsg()
{
	StopBootModeWork();
}
void CNVEditorView::GetSaveNVIDInfo(BOOL bFullWrite /*= TRUE*/)
{
	m_vecNVID.clear();
	int nTotalCount = m_pProject->GetNVItemCount(TRUE);
	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();
	for (int nIndex = 0; nIndex < nTotalCount; ++nIndex)
	{
		WORD wItemId = static_cast<WORD>( m_pProject->GetNVItemID( nIndex ) );  
		if (bFullWrite)
		{
			if ( 1 == pMF->m_nSave2PhoneMode ) //calibration mode
			{
				std::map<WORD,CString>::iterator it = (pMF->m_mapSyncCali).find(wItemId);
				if ((pMF->m_mapSyncCali).end() == it)
				{
					continue;
				}		
			}
			m_vecNVID.push_back(wItemId);
		}
		else
		{
			if (m_pProject->IsChangedNVItem( nIndex ))
			{
				if ( 1 == pMF->m_nSave2PhoneMode ) //calibration mode
				{
					std::map<WORD,CString>::iterator it = (pMF->m_mapSyncCali).find(wItemId);
					if ((pMF->m_mapSyncCali).end() == it)
					{
						continue;
					}		
				}
				m_vecNVID.push_back(wItemId);
			}
		}		

	}

}
void CNVEditorView::OnSaveToPhone() 
{
	// Get NVItem count
	TCHAR szFilePath[_MAX_PATH]={0};
	DWORD dwStartTime	= GetTickCount();
	DWORD dwElapsedTime = 0;
	CString strElapsedTime;
	DWORD dwThreadID	= 0;
	HANDLE hThread		= NULL;
	int ret				= 0;	
	CMainFrame* pMF		= (CMainFrame *)AfxGetMainWnd();
	m_nWriteFail		= 0;
	m_nInterpFail		= 0;
	theApp.GetIniFilePath(szFilePath);
	pMF->m_nSave2PhoneMode	= ::GetPrivateProfileInt(_T("Settings"),_T("EnableSaveByNormal"),0,szFilePath);
	m_bFullWrite			= GetPrivateProfileInt( _T("Settings"), _T("FullWrite"), FALSE, szFilePath );

	GetSaveNVIDInfo(m_bFullWrite);
    m_nNVItemTotalCount = m_vecNVID.size();
	
    if ( 0 == m_nNVItemTotalCount )
    {
		if(m_bFullWrite)
		{
			AfxMessageBox( _T("No NVItems") );
			return;
		}
		else
		{
			if( AfxMessageBox( _T("No find any modification item, Are you sure to write all nv items?"),MB_OKCANCEL ) == IDOK )
			{
				dwStartTime	= GetTickCount();
				GetSaveNVIDInfo(TRUE);
				m_nNVItemTotalCount = m_vecNVID.size();
				if ( 0 == m_nNVItemTotalCount )
				{
					AfxMessageBox( _T("No NVItems") );
					return;
				}
			}
			else
			{
				return;
			}
		}
        
    }
    m_nCurrCount = 0;
	
    // Clear up
    LFPClearUp();	
	
	if(m_DiagObj.Open(szFilePath) != DIAG_SUCCESS)
	{
		::AfxMessageBox( _T("Error: Open channel fail"), MB_ICONSTOP );
        goto LFPW_ERROR;
	}
	
	m_bUserCancel = FALSE;
	
	// Show Feedback dialog to users
	m_dlgFeedBack.m_bRead = FALSE;
	m_dlgFeedBack.m_bInit = FALSE;
    m_dlgFeedBack.m_nRange = m_nNVItemTotalCount;
    m_dlgFeedBack.m_strInfo.Format( _T("0 item send. (Total %d items)"), 
		m_nNVItemTotalCount );
	m_strLFPLastErr.Empty();
	
	
	hThread = CreateThread( NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetWriteWorkThread,
		(LPVOID)this,
		NULL,
		&dwThreadID);
	
    ret = m_dlgFeedBack.DoModal();
	
	m_dlgFeedBack.m_bInit = FALSE;
	m_bUserCancel = TRUE;
	if(hThread)
	{
		WaitForSingleObject( hThread, 5000);
		CloseHandle( hThread);
		hThread = NULL;
	}
	
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	dwElapsedTime = GetTickCount() - dwStartTime;
	//strElapsedTime.Format(_T("ReadNV ElapsedTime: %d.%ds."),dwElapsedTime/1000,dwElapsedTime%1000);
    //AfxMessageBox(strElapsedTime);
	
    if ( IDCANCEL == ret )
    {
		m_bUserCancel = TRUE;
        // User canceled this load process
        AfxMessageBox( _T("This write process is canceled") );
    }
    else if ( IDOK == ret )
    {
        // DoModal returned from OnCmmSendData 
        // when everything is done successfully
		CString strInfo;
		strInfo.Format( _T("This write process Over! Elapsed time: %d.%ds.\nTotal %d items\nWrite fail: %d items\nInterpret fail: %d items"),
			dwElapsedTime/1000,
			dwElapsedTime%1000,
			m_nNVItemTotalCount,
			m_nWriteFail,
			m_nInterpFail );
        AfxMessageBox( strInfo );
    }
    else if ( IDIGNORE == ret )
    {
        // If ret IDABORT, there must be sth wrong happened
        // m_dlgFeedBack.EndDialog() is invoked with IDIGNORE in LFPClearUp();
        return;
    }
    else if( IDABORT == ret )
    {
        AfxMessageBox( _T( "Can not write item to phone!" ),MB_ICONSTOP );
    }
	
    LFPClearUp();
    return;
	
	
LFPW_ERROR:
    LFPClearUp();
    return; 
	
}

void CNVEditorView::OnExportStatic() 
{
	// TODO: Add your command handler code here
	  // CDlgExport dlg;
	   TCHAR szfiles[_MAX_PATH]={0};
	   TCHAR szappend[_MAX_PATH]={0};
	 
	   _tcscpy(szfiles,_T("Static NV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));
	   _tcscpy(szappend,_T("csv"));
	   
	   CFileDialog dlgSaveStatic(FALSE ,              //Open File
		   szappend,               //default Extension
		   NULL,
		   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		   szfiles,
		   this
		);

	   if( IDOK != dlgSaveStatic.DoModal() )
	   {
		   // User cancel
		   return;
	   }	
	   
	   try
	   {
		   CString szPrjFileName = dlgSaveStatic.GetPathName () ;
	       CWaitCursor wc;
		   CStdioFile file(szPrjFileName.GetBuffer(0),CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		   
		   CMapData* mapStaticNv;
           m_pProject->GetStaticNVID(mapStaticNv);
		   if (mapStaticNv->empty())
		   {
               AfxMessageBox(_T("No Static NV"));
		   }
		   // Write file 
		   ExportData(file, mapStaticNv) ;
		   	  
	   }
	   catch( CFileException* pfe )
	   {
		   // Error occurs
		   pfe->ReportError( MB_ICONSTOP );
		   AfxMessageBox( _T( "Export failed!" ) );
		   pfe->Delete();
	}
	
}
void CNVEditorView::ExportData(CStdioFile& file, CMapData* mapNVData) 
{
	MD5_CTX context;
	CString strEnd;
  
	unsigned char digest[16];

	MD5Init (&context);
	m_pProject->SaveToImage(NULL);
	
	CFileMap map(m_pProject->m_szTargetFileName);
	
	const void * pvFile = map;
	if ( NULL == pvFile )
	{
		CString strErr;
		strErr.Format(_T("Map file %s failed"), m_pProject->m_szTargetFileName);
		AfxMessageBox(strErr);
		return;
	}
	DWORD cbFileSize = map.GetSize();
	
	WORD wLen;
	WORD wItemId;
	
	//	LoadNVItemFromImage((const unsigned char*)pvFile, cbFileSize);
    
	
	CMapData::iterator  it = mapNVData->begin();
	
	for(; it != mapNVData->end(); ++it)
	{
		CString strPath; 

		strPath.Format(_T("%d,%s,"), it->first, it->second);

		wItemId = (WORD)it->first;
		const void* pItem = GetNVItemFromBlock((const unsigned char*)pvFile, cbFileSize, wItemId, &wLen);

		BYTE* pData = (BYTE*)pItem;
		if (NULL != pItem)
		{
			TCHAR  szContent[MAX_ITEM_CONTENT];
			strPath += "\"";
			MD5Update (&context, (unsigned char*)pItem, wLen);
			for (int i = 0; i < wLen; i++)
			{
				_stprintf(szContent, _T("%02X"), pData[i]);
				strPath += szContent;
				if (wLen-1 == i)
				{
					strPath += "\"";
				}
				else
				{
					strPath += ",";
				}
			}		

		}
		strPath += "\n";
		file.WriteString(strPath);		
	}
	MD5Final (digest, &context);

	CString strCheckSum;
	
	for (int i = 0; i < 16; i++)
	{
		CString strTmp;
		strTmp.Format(_T("%02X"), digest[i]);
		strCheckSum += strTmp;
	}

	strEnd.Format(_T("0,checkSum,'%s"), strCheckSum);

//	AfxMessageBox(strEnd);
	file.WriteString(strEnd);
}

void CNVEditorView::GenerateItemInfo(CStdioFile& file)
{
	file.WriteString(_T("ItemName,ItemID,Length\n"));
	int nCount = m_pProject->GetNVItemCount();
	 
 	for( int i=0; i<nCount; i++ )
	{
		WORD wItemId = (WORD)m_pProject->GetNVItemID(i);
		DWORD wLen = m_pProject->GetNVItemLength(i);
		CString strPath;
		strPath.Format(_T("%s,'%04X,%d\n"), m_pProject->GetNVItemName(i), wItemId, wLen);
		file.WriteString(strPath);	
	}
	
	/* count by bin*/
// 	CString szPrjFileName(_T("nvitem.csv"));
// 	CStdioFile file(szPrjFileName.GetBuffer(0),CFile::modeCreate | CFile::modeWrite | CFile::typeText);
// 	file.WriteString(_T("ItemName,ItemID,Length\n"));
// 
// 		   
// 	CFileMap map(m_pProject->m_szTargetFileName);
// 	
// 	const void * pvFile = map;
// 	if ( NULL == pvFile )
// 	{
// 		CString strErr;
// 		strErr.Format(_T("Map file %s failed"), m_pProject->m_szTargetFileName);
// 		AfxMessageBox(strErr);
// 		return;
// 	}
// 	DWORD cbFileSize = map.GetSize();
// 	int nCount = m_pProject->GetNVItemCount();
// 
// 	for( int i=0; i<nCount; i++ )
// 	{
// 		WORD wLen;
// 		WORD wItemId = (WORD)m_pProject->GetNVItemID(i);
// 		const void *pItem = GetNVItemFromBlock( (const unsigned char*)pvFile, 
// 		 	                                 cbFileSize, 
// 												wItemId, 
// 												&wLen);
// 		if (NULL != pItem)
// 		{
// 			CString strPath;
// 			strPath.Format(_T("%s,'%04X,%d\n"), m_pProject->GetNVItemName(i), wItemId, wLen);
// 			file.WriteString(strPath);	
// 		}
// 
// 	}
}

void CNVEditorView::OnGenerStatistical() 
{
	// TODO: Add your command handler code here

	// TODO: Add your command handler code here
	// CDlgExport dlg;
	   TCHAR szfiles[_MAX_PATH]={0};
	   TCHAR szappend[_MAX_PATH]={0};
	   
	   _tcscpy(szfiles,_T("Statistical NVInfo (*.csv)|*.csv|All Files (*.*)|*.*||"));
	   _tcscpy(szappend,_T("csv"));
	   
	   CFileDialog dlgNVInfo(FALSE ,              //Open File
		   szappend,               //default Extension
		   NULL,
		   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		   szfiles,
		   this
		   );
	   
	   if( IDOK != dlgNVInfo.DoModal() )
	   {
		   // User cancel
		   return;
	   }	
	   
	   try
	   {
		   CString szPrjFileName = dlgNVInfo.GetPathName () ;
		   CWaitCursor wc;
		   CStdioFile file(szPrjFileName.GetBuffer(0),CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		   
		   // Write file 
		   GenerateItemInfo(file);
		   
	   }
	   catch( CFileException* pfe )
	   {
		   // Error occurs
		   pfe->ReportError( MB_ICONSTOP );
		   AfxMessageBox( _T( "Statistics failed!" ) );
		   pfe->Delete();
	   }
}

void CNVEditorView::OnMergeNv() 
{
	// TODO: Add your command handler code here
	CString strFileDir = BrowseForFolder(_T("choose the project config_nv directory"), 
        (LPCTSTR)m_pProject->m_szPrjFileName);
	
    if (strFileDir.IsEmpty())
    {
        return;
    }

	TCHAR strFileName[20];
	_tcscpy(strFileName, _T("nv_config.h"));
	if (!m_pProject->GenerateConfigFile(strFileDir, strFileName))
	{
		AfxMessageBox(_T("Merge NVFiles failed,can not generate ss_nv_config.h."));
		return;
	}
	AfxMessageBox(_T("Merge NVFiles Success,please check ss_nv_config.h in project Path"));

}

CString CNVEditorView::BrowseForFolder(LPCTSTR lpszPromt, LPCTSTR pBuf)
{
    TCHAR  szDisplayName[MAX_PATH] = _T("");
    TCHAR  szPath[MAX_PATH] = _T("");
    BROWSEINFO bi;
	
    bi.hwndOwner = GetSafeHwnd();
    bi.pidlRoot = NULL;
    bi.lpszTitle = lpszPromt;
    bi.pszDisplayName = szDisplayName;
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = BrowseCtrlCallback;
    bi.lParam = (LPARAM)pBuf;
	
    LPITEMIDLIST pItemIDList = SHBrowseForFolder( &bi );
    if( pItemIDList )
    {
        SHGetPathFromIDList(pItemIDList, szPath) ;
		
        IMalloc *pMalloc;
        if( SHGetMalloc( &pMalloc ) != NOERROR )
        {
            AfxMessageBox(_T("Failed to get pointer to shells task allocator")) ;
            return szPath;
        }
        pMalloc->Free( pItemIDList );
        if( pMalloc )
            pMalloc->Release();
    }
	
    return szPath;
}


int CALLBACK CNVEditorView::BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    UNREFERENCED_PARAMETER(lParam);
    //UNREFERENCED_PARAMETER(lpData);
	
    TCHAR szDir[MAX_PATH];
    ::ZeroMemory(szDir, sizeof(TCHAR));
	
    switch (uMsg)
    {
        // Set the selected directory at startup.
    case BFFM_INITIALIZED:
        {
            _tcscpy(szDir, (TCHAR*)lpData);
			
            // if no path was availiable, try the current directory.
            if (_tcslen(szDir) == 0)
            {
                ::GetCurrentDirectory(MAX_PATH, szDir);
            }
			
            // set the directory path only if szDir is defined.
            if (_tcslen(szDir) != 0)
            {
                // wParam is TRUE since you are passing a path,
                // it would be FALSE if you were passing a pidl.
				
                ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
            }
        }
        break;
		
    case BFFM_VALIDATEFAILED:
        break;
		
    default:
        break;
    }
    return 0;
}

// void CNVEditorView::OnMergeBin() 
// {
// 	// TODO: Add your command handler code here
// 	// TODO: Add your command handler code here
// 	CString strFileDir = BrowseForFolder(_T("choose the project config_nv directory"), 
//         (LPCTSTR)m_pProject->m_szPrjFileName);
// 	
//     if (strFileDir.IsEmpty())
//     {
//         return;
//     }
// 	
// 	TCHAR strFileName[20];
// 	_tcscpy(strFileName, _T("delta_nv.bin"));
// 	if (!m_pProject->GenerateConfigFile(strFileDir, strFileName, FALSE))
// 	{
// 		AfxMessageBox(_T("Merge NVFiles failed,can not generate delta_nv.bin."));
// 		return;
// 	}
// 	AfxMessageBox(_T("Merge NVFiles Success,please check delta_nv.bin in project Path"));	
// }

// void CNVEditorView::OnUpdateMergeBin(CCmdUI* pCmdUI) 
// {
// 	// TODO: Add your command update UI handler code here
// 	pCmdUI->Enable(m_bOpenedPrj);
// 	
// }

void CNVEditorView::OnPacketModerm() 
{
	// TODO: Add your command handler code here
	CPacketModerm dlg;	
    if(IDOK != dlg.DoModal())
        return;

	if (!m_pProject->GenerateConfigFile(dlg.m_strNVFilePath, dlg.m_strModermFileName.GetBuffer(0), FALSE))
	{
		AfxMessageBox(_T("Merge NVFiles failed."));
	
		return;
	}

	AfxMessageBox(_T("Merge NVFiles Success."));	
}

void CNVEditorView::OnShowFailNv() 
{
	// TODO: Add your command handler code here
	if (m_strLoadFailInfo.IsEmpty())
	{
		AfxMessageBox(_T("No NV Items Load Failed."));
	}
	else
	{
		AfxMessageBox(m_strLoadFailInfo);
	}
	
}

void CNVEditorView::OnShowModify() 
{
	// TODO: Add your command handler code here
	m_List.Refresh();
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
	
}

void CNVEditorView::OnCalibrationNv() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// CDlgExport dlg;
	   TCHAR szfiles[_MAX_PATH]={0};
	   TCHAR szappend[_MAX_PATH]={0};
	   
	   _tcscpy(szfiles,_T("Calibration NV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));
	   _tcscpy(szappend,_T("csv"));
	   
	   CFileDialog dlgSaveCalibration(FALSE ,              //Open File
		   szappend,               //default Extension
		   NULL,
		   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		   szfiles,
		   this
		   );
	   
	   if( IDOK != dlgSaveCalibration.DoModal() )
	   {
		   // User cancel
		   return;
	   }	
	   
	   try
	   {
		   CString szPrjFileName = dlgSaveCalibration.GetPathName () ;
		   CWaitCursor wc;
		   CStdioFile file(szPrjFileName.GetBuffer(0),CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		   
		   // Write file 
		   CMapData* mapCalibrationNv;
           m_pProject->GetCalibrationNVID(mapCalibrationNv); 
		   if (mapCalibrationNv->empty())
		   {
               AfxMessageBox(_T("No Calibration NV"));
		   }
		   ExportData(file, mapCalibrationNv) ;	
		      
	   }
	   catch( CFileException* pfe )
	   {
		   // Error occurs
		   pfe->ReportError( MB_ICONSTOP );
		   AfxMessageBox( _T( "Export failed!" ) );
		   pfe->Delete();
	}
	
}

void CNVEditorView::OnLoadCustomerNV() 
{
	// TODO: Add your command handler code here
	// Open file for read first
    static TCHAR BASED_CODE szFilter[] = _T("Import Customer NV (*.nv)|*.nv||");
	
	CFileDialog dlg(TRUE, _T("nv"), _T("customer"), OFN_FILEMUSTEXIST, szFilter, NULL);
    if( IDOK != dlg.DoModal() )
    {
        // User cancel
        return;
    }
	m_List.ImportItem( dlg.GetPathName(),TRUE);
	UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
	
}

void CNVEditorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pSender != NULL && lHint == WM_REFRESH_CUST_TREE )
	{
		m_List.Refresh();
	}
	
}
