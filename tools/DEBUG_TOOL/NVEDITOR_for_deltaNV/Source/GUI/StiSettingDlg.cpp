// StiSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "StiSettingDlg.h"
#include "EdtBtnCtrl.h"
#include "MainFrm.h"
#include "NVEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStiSettingDlg dialog


CStiSettingDlg::CStiSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStiSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStiSettingDlg)
	m_strDspFileName = _T("");
	m_bWriteDsp = FALSE;

	//[[ by Joly
	m_bActiveFiles = FALSE;

	m_strFixBinName = _T("");
	m_bWriteFix = FALSE;

	m_bVerFlag = FALSE;
	//]]

	//}}AFX_DATA_INIT
}


void CStiSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStiSettingDlg)
	DDX_Control(pDX, IDC_STI_DSPFILE, m_ctlDspFile);
	DDX_Control(pDX, IDC_STI_BROWSE, m_ctlBrowse);
	DDX_Control(pDX, IDC_STI_BROWSE2, m_ctlBrowse2);
	DDX_Control(pDX, IDC_STI_FIXBIN, m_ctlFixBin);
	DDX_Text(pDX, IDC_STI_FIXBIN, m_strFixBinName);
	DDX_Text(pDX, IDC_STI_DSPFILE, m_strDspFileName);
	DDX_Check(pDX, IDC_STI_WRITEFIX, m_bWriteFix);
	DDX_Check(pDX, IDC_STI_WRITEDSP, m_bWriteDsp);
	//[[ by Joly
	DDX_Control(pDX, IDC_FILE_LIST, m_lstWriteFile);
	DDX_Check(pDX, IDC_MUL_FILE_CHECK, m_bActiveFiles);
	DDX_Check(pDX, IDC_VERSION_FLAG, m_bVerFlag);
	//]]
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStiSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CStiSettingDlg)
	ON_BN_CLICKED(IDC_STI_WRITEDSP, OnStiWritedsp)
	ON_BN_CLICKED(IDC_STI_BROWSE, OnStiBrowse)
	ON_BN_CLICKED(IDC_STI_WRITEFIX, OnStiWriteFix)
	ON_BN_CLICKED(IDC_STI_BROWSE2, OnStiBrowse2)
	ON_BN_CLICKED(IDC_MUL_FILE_CHECK, OnCheckWriteFiles)
	ON_BN_CLICKED(IDC_CLEAR_BUT, onClearButton)
	ON_NOTIFY(NM_CLICK, IDC_FILE_LIST, OnClickClear) 
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_FILE_LIST, OnEndlabeleditLstDsp)
	ON_BN_CLICKED(IDC_VERSION_FLAG, OnStiVersionFlag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStiSettingDlg message handlers

BOOL CStiSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);

    ::SetFocus( m_ctlDspFile.m_hWnd );

    m_ctlDspFile.EnableWindow( m_bWriteDsp );
    m_ctlBrowse.EnableWindow( m_bWriteDsp );

	// add by Joly for fix nv bin 
	::SetFocus(m_ctlFixBin.m_hWnd);
	m_ctlFixBin.EnableWindow(m_bWriteFix);
	m_ctlBrowse2.EnableWindow(m_bWriteFix);
	
	//[[ by Joly
	InitList(m_lstWriteFile);
	FillList();
	((CButton*)(GetDlgItem(IDC_MUL_FILE_CHECK)))->SetCheck(m_bActiveFiles);
	m_lstWriteFile.EnableWindow(m_bActiveFiles);
	GetDlgItem(IDC_CLEAR_BUT)->EnableWindow(m_bActiveFiles);
	
	//]]
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//[[add by Joly for fix nv bin
void CStiSettingDlg::OnStiWriteFix()
{
	UpdateData();
	m_ctlFixBin.EnableWindow(m_bWriteFix);
	m_ctlBrowse2.EnableWindow(m_bWriteFix);
}

void CStiSettingDlg::OnStiBrowse2() 
{
	// TODO: Add your control notification handler code here
	static TCHAR szFilter[] = _T("Binary Files (*.bin)|*.bin|All files (*.*)|*.*||");
	
    UpdateData();
	CFileDialog dlg(TRUE, NULL, m_strFixBinName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strFixBinName = dlg.GetPathName();
        UpdateData(FALSE);
		
        return;
    }
	
}
//]]
void CStiSettingDlg::OnStiWritedsp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
    m_ctlDspFile.EnableWindow( m_bWriteDsp );
    m_ctlBrowse.EnableWindow( m_bWriteDsp );
	
}

void CStiSettingDlg::OnStiBrowse() 
{
	// TODO: Add your control notification handler code here
	static TCHAR szFilter[] = _T("Binary Files (*.bin)|*.bin|All files (*.*)|*.*||");

    UpdateData();
	CFileDialog dlg(TRUE, NULL, m_strDspFileName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strDspFileName = dlg.GetPathName();
        UpdateData(FALSE);

        return;
    }
	
}

void CStiSettingDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

    CString strText;
    
    if ( m_bWriteDsp )
    {
        m_ctlDspFile.GetWindowText( strText ); 
        if(strText.GetLength() == 0)
        {
            ::AfxMessageBox( _T("Must specify DSP file!") );
            return;
        }
    }
	//[[ by Joly
	if (m_bWriteFix)
	{
		m_ctlFixBin.GetWindowText(strText);
		if(strText.GetLength() == 0)
        {
            ::AfxMessageBox( _T("Must specify FixNV bin file!") );
            return;
        }

	}
	if ( m_bActiveFiles )
	{
		CString strErrorMsg;
		int nCount = m_lstWriteFile.GetItemCount();
		for(int i = 0; i < nCount && m_bActiveFiles; i++)
		{
			if(m_lstWriteFile.GetCheck(i))
			{
			
				CString strID = m_lstWriteFile.GetItemText(i,1);
				CString strFile = m_lstWriteFile.GetItemText(i,2);
				ST_FILE_INFO sFileInfo;
				if( ( NULL != _tcschr(strID.operator LPCTSTR(), 'x') ) ||
					( NULL != _tcschr(strID.operator LPCTSTR(), 'X') ) )
				{
					sFileInfo.nFileID  = _tcstol( strID.operator LPCTSTR(), NULL, 16 );
				}
				else
				{
					sFileInfo.nFileID  = _tcstol( strID.operator LPCTSTR(), NULL, 10 );
                }
				CheckID( sFileInfo.nFileID, strErrorMsg);
				_tcscpy( sFileInfo.szFilePath, strFile.operator LPCTSTR() );
				sFileInfo.bCheckFlag = TRUE;	
				m_vecFileInfo.push_back(sFileInfo);
			
			}
		}
		if ( !strErrorMsg.IsEmpty() )
		{
			AfxMessageBox(strErrorMsg);
			return;
		}
	}
	
	SaveList();
	//]]
    EndDialog( IDOK );
//	CDialog::OnOK();
}


// [[ by Joly
void CStiSettingDlg::CheckID(UINT nID,CString& strMsg)
{
	CMainFrame *pMF = (CMainFrame*)AfxGetMainWnd();
	CNVEditorView *pView = (CNVEditorView *)pMF->GetActiveView();
	if ( pView->m_pProject->IsExistedID(nID) )
	{
		CString strErrorMsg;
		strErrorMsg.Format( _T("FileID %d had existed!\t \n"), nID );
		strMsg += strErrorMsg;
	}	
}

void CStiSettingDlg::OnCheckWriteFiles()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bActiveFiles)
	{
		m_lstWriteFile.EnableWindow(TRUE);
		GetDlgItem(IDC_CLEAR_BUT)->EnableWindow(TRUE);
	}
	else
	{
		m_lstWriteFile.EnableWindow(FALSE);
		GetDlgItem(IDC_CLEAR_BUT)->EnableWindow(FALSE);
	}
	
}


void CStiSettingDlg::onClearButton() 
{
	// TODO: Add your control notification handler code here
	m_lstWriteFile.DeleteAllItems();
}

void CStiSettingDlg::OnEndlabeleditLstDsp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstWriteFile.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}

void CStiSettingDlg::OnClickClear(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ClickList(pNMHDR,m_lstWriteFile);
	*pResult = 0;
}

void CStiSettingDlg::InitList(CListCtrl& ctrList)
{
	ctrList.SetBkColor( RGB(232, 232, 232) );	
	ctrList.ModifyStyle( 0, LVS_SHOWSELALWAYS );
	
	DWORD dwExStyle = ctrList.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;
	
	ctrList.SetExtendedStyle(dwExStyle);
	ctrList.InsertColumn( 0, _T(" "), LVCFMT_LEFT, 50 );	
	ctrList.InsertColumn( 1, _T("ID"), LVCFMT_LEFT, 100 );		
	ctrList.InsertColumn( 2,  _T("File"), LVCFMT_LEFT, 179 );



}

void CStiSettingDlg::ClickList(NMHDR* pNMHDR,CListCtrl& ctrList)
{
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;
	
	CEdtBtnCtrl *pCtrl = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	if(nItem == -1)
	{
		int nCount = ctrList.GetItemCount();
		CString strText;
		if( nCount == 0 || \
			(!ctrList.GetItemText( nCount-1, 1 ).IsEmpty() && \
			!ctrList.GetItemText( nCount-1, 2 ).IsEmpty() ))
		{
			ctrList.InsertItem( nCount, _T("") );
			if(nCount > 0)			
			{
				ctrList.SetCheck(nCount-1);
			}
			nItem = nCount;
		}	
		else if(ctrList.GetItemText( nCount-1, 1 ).IsEmpty() && 
			ctrList.GetItemText( nCount-1, 2 ).IsEmpty() )
		{
			ctrList.DeleteItem(nCount-1);
			return;
		}
		else
		{
			return;
		}
	}
	
	if( nSubItem == 0 )
		return;
			
	pCtrl = new CEdtBtnCtrl();
	
	BOOL bEnablBtn = FALSE;
	BOOL bOpenFile = FALSE;
	if( 2 == nSubItem )
	{
		bEnablBtn = TRUE;
	}
	
	
	if( NULL != pCtrl )
	{		
		pCtrl->EnableFileSelButton(bEnablBtn);
		pCtrl->SetBtnOpenFlag(bOpenFile);
		if( 1 == nSubItem )
		{
			pCtrl->SetEditFmtMode(EX_FMT_HEX);
		}
		pCtrl->m_nItem = nItem;
		pCtrl->m_nSubItem = nSubItem;
		pCtrl->m_strText = ctrList.GetItemText( nItem, nSubItem );
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(ctrList.m_hWnd)))
		{
			AfxMessageBox(_T("Error to create EdtBtnCtrl"));
			return;
		}
	}
	else
	{
		return;
	}
	
	CRect rect;
	ctrList.GetSubItemRect( nItem, nSubItem, LVIR_BOUNDS, rect );
	CRect rtList;
	ctrList.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	pCtrl->SetWindowPos( &wndTop, rect.left, rect.top-1, rect.right-rect.left, rect.bottom-rect.top, NULL );
	
    pCtrl->ShowWindow(SW_SHOW);
}

void CStiSettingDlg::FillList()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName( NULL, szPath, MAX_PATH );
	TCHAR *p = _tcsrchr( szPath, '.' );
	if(p)
	{
		*p='\0';
	}	
	_tcscat( szPath, _T(".ini") );

	TCHAR szBuf[1024] = {0};

	m_bActiveFiles = GetPrivateProfileInt( _T("OtherSetting"),_T("WriteFiles"), 0, szPath);
	
	GetPrivateProfileSection( _T("OtherFiles"), szBuf, 1024, szPath );

	CStringArray agFiles;
	

	int nKeys = EnumKeys( szBuf, &agFiles );

	int nItem = 0;

	for( int i = 0; i < nKeys; i++ )
	{
		CString strFileInfo = agFiles[2*i+1];	
		CStringArray agInfo;
		SplitStr( strFileInfo.operator LPCTSTR(), agInfo );
		if( 3 != agInfo.GetSize() )
		{
			continue;
		}
		CString strItem = agInfo[0];
		strItem.TrimLeft();
		strItem.TrimRight();

		BOOL bCheck = _ttoi(strItem);

		strItem = agInfo[1];
		strItem.TrimLeft();
		strItem.TrimRight();

		CString strID = strItem;

		strItem = agInfo[2];
		strItem.TrimLeft();
		strItem.TrimRight();

		m_lstWriteFile.InsertItem( nItem, _T("") );
		m_lstWriteFile.SetCheck( nItem, bCheck );
		m_lstWriteFile.SetItemText( nItem, 1, strID );
		m_lstWriteFile.SetItemText( nItem, 2, strItem );
		
		nItem++;
	}

}
void CStiSettingDlg::SaveList()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName( NULL, szPath, MAX_PATH );
	TCHAR *p = _tcsrchr( szPath, '.' );
	if(p)
	{
		*p='\0';
	}	
	_tcscat( szPath, _T(".ini") );

	
	int nCount = m_lstWriteFile.GetItemCount();	
	
	WritePrivateProfileSection( _T("OtherFiles"), _T(""), szPath );
	CString strCheck;
	strCheck.Format( _T("%d"), m_bActiveFiles );

	WritePrivateProfileString( _T("OtherSetting"), _T("WriteFiles"), strCheck, szPath );

	for( int i = 0; i < nCount; i++ )
	{
		CString strItem;
		CString strKey;
		strKey.Format( _T("File%d"), i + 1 );

		strItem.Format( _T("%d,%s,%s"),
			m_lstWriteFile.GetCheck(i),
			m_lstWriteFile.GetItemText(i,1),
			m_lstWriteFile.GetItemText(i,2) );

		WritePrivateProfileString( _T("OtherFiles"), strKey, strItem, szPath );
	}
}

// ]]


void CStiSettingDlg::OnStiVersionFlag() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
}
