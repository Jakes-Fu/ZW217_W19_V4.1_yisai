// NewModule.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "NewModule.h"
#include "FileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNVEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNewModule dialog


CNewModule::CNewModule(CWnd* pParent /*=NULL*/)
	: CDialog(CNewModule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewModule)
	m_szModuleName = _T("");
	m_szModuleDesc = _T("");
	m_szModuleFileName = _T("C:\\");
	//}}AFX_DATA_INIT

    m_pProject = NULL;
    m_nTotalModuleCount = 0;
    m_nCurrModuleIndex = 0;
}


void CNewModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewModule)
	DDX_Control(pDX, ID_BTN_NEXT, m_ctlBtnNext);
	DDX_Control(pDX, IDOK, m_ctlBtnOk);
	DDX_Control(pDX, IDC_BTN_LAST, m_ctlLastModule);
	DDX_Control(pDX, IDC_FILE_LOCATION, m_ctlModuleFileName);
	DDX_Text(pDX, IDC_MODULE_NAME, m_szModuleName);
	DDX_Text(pDX, IDC_MODULE_DESCRIPTION, m_szModuleDesc);
	DDX_Text(pDX, IDC_FILE_LOCATION, m_szModuleFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewModule, CDialog)
	//{{AFX_MSG_MAP(CNewModule)
	ON_BN_CLICKED(ID_BTN_NEXT, OnBtnNext)
	ON_BN_CLICKED(IDC_BTN_LAST, OnBtnLast)
	ON_EN_CHANGE(IDC_MODULE_NAME, OnChangeModuleName)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FOLDER, OnButtonBrowseFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CNewModule::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    
    m_ctlLastModule.EnableWindow (FALSE);
    m_ctlBtnOk.EnableWindow (FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CNewModule message handlers

void CNewModule::SetProject(CFileParser *pProject)
{
   ASSERT(NULL != pProject);

   m_pProject = pProject;
   m_nTotalModuleCount = m_pProject->GetModuleCount ();
   m_nCurrModuleIndex = m_nTotalModuleCount ;
   
}

void CNewModule::OnBtnNext() 
{
    UpdateData(TRUE);

    CString strPrjPath;
    m_pProject->GetProjectPath(strPrjPath);
    /*xml file support by xmzhang modify begin*/
    if( !theApp.m_bNvmFormat )
    {
        m_szModuleFileName = strPrjPath + "\\" + m_szModuleName + ".xml";
    }
    else
    {
        m_szModuleFileName = strPrjPath + "\\" + m_szModuleName + ".nvm";
    }
    
    /*xml file support by xmzhang modify end*/

    if(m_szModuleName == "")
    {
       AfxMessageBox(_T("The Module Name Can't be NULL"));
       return;
    }

    // TODO: Add your control notification handler code here	
    if(m_nTotalModuleCount == m_nCurrModuleIndex)
    {    
        int index = m_szModuleName.ReverseFind('\\');
        if(index != -1)
        {
            // Remove path
            m_szModuleName.Delete(0,index + 1);
        }
        
        int nCount = m_pProject->GetModuleCount();
        for(int i=0;i<nCount;i++)
        {
            Module& md = m_pProject->GetModule(i);
            if(!m_szModuleName.CompareNoCase(md.szModuleName))
            {
                AfxMessageBox(_T("There is already a module with the same name!"));
                return;
            }
        }

        m_pProject->CreateModule (m_szModuleName.operator LPCTSTR (),
            m_szModuleFileName.operator LPCTSTR (),
            m_szModuleDesc.operator LPCTSTR ());

        m_pProject->SetModified(TRUE);
        
        m_szModuleName = _T("");
        m_szModuleFileName = _T("C:\\");
        m_szModuleDesc = _T("");
        UpdateData(FALSE);
        
        m_nTotalModuleCount++;
        m_nCurrModuleIndex++;
    }        
    else    
    {    
        m_nCurrModuleIndex++;

        if(m_nTotalModuleCount > m_nCurrModuleIndex)
        {
            
            Module tmpModule = m_pProject->GetModule(m_nCurrModuleIndex);
            
            m_szModuleName = tmpModule.szModuleName ;
            m_szModuleFileName = tmpModule.szModuleFileName ;
            m_szModuleDesc = tmpModule.szModuleDescription ;
        }
        else
        {
            m_szModuleName = _T("");
            m_szModuleFileName = _T("C:\\");
            m_szModuleDesc = _T("");
        }

        UpdateData(FALSE);
    }
    
    m_ctlLastModule.EnableWindow (TRUE);   
}

void CNewModule::OnBtnLast() 
{
    // TODO: Add your control notification handler code here
    Module tmpModule = m_pProject->GetModule(m_nCurrModuleIndex - 1);
    
    m_szModuleName = tmpModule.szModuleName ;
    m_szModuleFileName = tmpModule.szModuleFileName ;
    m_szModuleDesc = tmpModule.szModuleDescription ;
    UpdateData(FALSE);
    
    m_nCurrModuleIndex--;
    
    if(0 == m_nCurrModuleIndex)
    {
        m_ctlLastModule.EnableWindow (FALSE);
    }
}


void CNewModule::OnChangeModuleName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_ctlBtnNext.EnableWindow (TRUE);
    m_ctlBtnOk.EnableWindow (TRUE);
}

void CNewModule::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
    int index = m_szModuleName.ReverseFind('\\');
    if(index != -1)
    {
        // Remove path
        m_szModuleName.Delete(0,index + 1);
    }

    int nCount = m_pProject->GetModuleCount();
    for(int i=0;i<nCount;i++)
    {
        Module& md = m_pProject->GetModule(i);
        if(!m_szModuleName.CompareNoCase(md.szModuleName))
        {
            AfxMessageBox(_T("There is already a module with the same name!"));
            return;
        }
    }

    CString strPrjPath;
    m_pProject->GetProjectPath(strPrjPath);
    /*xml file support by xmzhang modify begin*/
    if( !theApp.m_bNvmFormat )
    {
        m_szModuleFileName = strPrjPath + "\\" + m_szModuleName + ".xml";
    }
    else
    {
        m_szModuleFileName = strPrjPath + "\\" + m_szModuleName + ".nvm";
    }
    
    /*xml file support by xmzhang modify end*/
    

    if(m_szModuleName == "")
    {
       AfxMessageBox(_T("The Module Name Can't is NULL"));
       return;
    }

    // TODO: Add your control notification handler code here	
    if(m_nTotalModuleCount == m_nCurrModuleIndex )
    {    
        m_pProject->CreateModule (m_szModuleName.operator LPCTSTR (),
            m_szModuleFileName.operator LPCTSTR (),
            m_szModuleDesc.operator LPCTSTR ());

        m_pProject->SetModified(TRUE);
    }

	CDialog::OnOK();
}

BOOL BrowForFolder(HWND hwdnOwner,TCHAR* szPath)
{
    if(szPath == NULL)
    {
        return FALSE;
    }

    LPMALLOC pMalloc;
    if (::SHGetMalloc(&pMalloc)!= NOERROR)		
    {
        return FALSE;
    }
    
    BROWSEINFO bi;
    ZeroMemory ( (PVOID) &bi,sizeof (BROWSEINFO));
    
    bi.hwndOwner		 = hwdnOwner;
    bi.pszDisplayName = szPath;
    bi.lpszTitle		 = _T("Open");
    bi.ulFlags		 = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
    bi.lpfn			 = NULL;
    bi.lParam		 = 0;
    
    LPITEMIDLIST pId = ::SHBrowseForFolder(&bi);
    if(pId != NULL)
    {
        if(::SHGetPathFromIDList(pId,szPath) == FALSE)
        {
            pMalloc->Free(pId);
            pMalloc->Release();
            return FALSE;
        }
        
        pMalloc->Free(pId);
    }
    else
    {
        pMalloc->Release();
        return FALSE;
    }
    
    pMalloc->Release();

    return TRUE;
}

void CNewModule::OnButtonBrowseFolder() 
{
    TCHAR szPath[MAX_PATH];

    if(::BrowForFolder(GetSafeHwnd(),szPath))
    {
        UpdateData(TRUE);
        m_szModuleFileName = szPath;
        UpdateData(FALSE);
    }
}

