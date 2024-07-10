// DlgLoadText.cpp : implementation file
//

#include "stdafx.h"
#include "Ownership.h"
#include "DlgLoadText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadText dialog


CDlgLoadText::CDlgLoadText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoadText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoadText)
	m_strCityFile = _T("");
	//}}AFX_DATA_INIT
	m_bSelCityFile = TRUE;
	m_hAddBitmap = NULL;
	m_hDelBitmap = NULL;
}

CDlgLoadText::~CDlgLoadText()
{
	if(m_hAddBitmap != NULL)
	{
		DeleteObject(m_hAddBitmap);
		m_hAddBitmap = NULL;
	}
	if(m_hDelBitmap != NULL)
	{
		DeleteObject(m_hDelBitmap);
		m_hDelBitmap = NULL;
	}
}

void CDlgLoadText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoadText)
	DDX_Control(pDX, IDC_DLT_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_DLT_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_DLT_LST_PHONE_NUMBER_FILES, m_lstNumberFiles);
	DDX_Text(pDX, IDC_DLT_EDT_CITY_FILE, m_strCityFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoadText, CDialog)
	//{{AFX_MSG_MAP(CDlgLoadText)
	ON_BN_CLICKED(IDC_DLT_BTN_ADD, OnDltBtnAdd)
	ON_BN_CLICKED(IDC_DLT_BTN_DEL, OnDltBtnDel)
	ON_BN_CLICKED(IDC_DLT_SEL, OnDltSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadText message handlers

BOOL CDlgLoadText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lstNumberFiles.SetBkColor(RGB(232, 232, 232));	
	m_lstNumberFiles.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstNumberFiles.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstNumberFiles.SetExtendedStyle(dwExStyle);	
	m_lstNumberFiles.InsertColumn(0, _T("Path"),LVCFMT_LEFT,260);

	m_hAddBitmap = LoadBitmap(g_theApp.m_hInstance,MAKEINTRESOURCE(IDB_ADD));
	m_hDelBitmap = LoadBitmap(g_theApp.m_hInstance,MAKEINTRESOURCE(IDB_DEL));

	m_btnAdd.SetBitmap(m_hAddBitmap);
	m_btnDel.SetBitmap(m_hDelBitmap);

	if(!m_bSelCityFile)
	{
		GetDlgItem(IDC_DLT_EDT_CITY_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLT_SEL)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoadText::OnDltBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, 
		_T("Phone Number file (*.txt)|*.txt||"));

/*	CString strFiles(_T(""),260*500);
	dlg.m_ofn.lpstrFile = strFiles.GetBuffer(260*500);
	dlg.m_ofn.nMaxFile = 260*500;
	strFiles.ReleaseBuffer();*/
	
	CString strWarning = _T("");
	if(dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		while(pos)
		{
			CString strFilePath = dlg.GetNextPathName(pos);
			int nCount = m_lstNumberFiles.GetItemCount();
			BOOL bExist = FALSE;
			for(int i = 0; i< nCount; i++ )
			{
				if(strFilePath.CompareNoCase(m_lstNumberFiles.GetItemText(i,0))==0)
				{
					bExist = TRUE;
					strWarning += strFilePath;
					strWarning += _T("\n");
					break;
				}
			}
			if(!bExist)
			{
				m_lstNumberFiles.InsertItem(nCount,strFilePath);
			}			
		}	
		
		if(!strWarning.IsEmpty())
		{
			strWarning += _T("\nFiles already exist in the list!");
			AfxMessageBox(strWarning);
		}
	}

}

void CDlgLoadText::OnDltBtnDel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstNumberFiles.GetFirstSelectedItemPosition();
	CUIntArray arrItems;
	while(pos)
	{
		UINT nItem = (UINT)m_lstNumberFiles.GetNextSelectedItem(pos);
		arrItems.Add(nItem);		
	}
	
	for(int i= 0; i< arrItems.GetSize(); i++)
	{
		m_lstNumberFiles.DeleteItem(((int)arrItems.GetAt(i))-i);
	}
}

void CDlgLoadText::OnDltSel() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".dat"),_T("R_ACC_MPLOCSTR"),OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("City files (*.dat)|*.dat|All files (*.*)|*.*||"));
	
	if(dlg.DoModal() == IDOK)
	{
		m_strCityFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgLoadText::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	for(int i=0;i<m_lstNumberFiles.GetItemCount();i++)
	{
		m_agNumberFiles.Add(m_lstNumberFiles.GetItemText(i,0));
	}

	if(m_agNumberFiles.GetSize() == 0)
	{
		AfxMessageBox(_T("Please input number files!"));
		return;
	}
	
	if(m_bSelCityFile && m_strCityFile.IsEmpty())
	{
		AfxMessageBox(_T("Please input city file!"));
		return;
	}
	CFileFind finder;
	if(m_bSelCityFile && !finder.FindFile(m_strCityFile))
	{
		AfxMessageBox(_T("City file does not exist!\nPlease input city file!"));
		return;
	}

	CDialog::OnOK();
}
