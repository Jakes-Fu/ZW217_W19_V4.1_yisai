// DlgLoadMids.cpp : implementation file
//

#include "stdafx.h"
#include "Ownership.h"
#include "DlgLoadMids.h"
#include "EdtBtnCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadMids dialog


CDlgLoadMids::CDlgLoadMids(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoadMids::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoadMids)
	m_strCityFile = _T("");
	//}}AFX_DATA_INIT
	m_bSelCityFile = TRUE;
}


void CDlgLoadMids::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoadMids)
	DDX_Control(pDX, IDC_DSS_LST_DST_FILES, m_lstOwnershipFiles);
	DDX_Text(pDX, IDC_DLC_EDT_CITY, m_strCityFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoadMids, CDialog)
	//{{AFX_MSG_MAP(CDlgLoadMids)
	ON_BN_CLICKED(IDC_DLC_BTN_SEL_CITY, OnSelCity)
	ON_NOTIFY(NM_DBLCLK, IDC_DSS_LST_DST_FILES, OnDblclkDssLstDstFiles)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_DSS_LST_DST_FILES, OnEndlabeleditDssLstDstFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadMids message handlers

void CDlgLoadMids::OnSelCity() 
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

void CDlgLoadMids::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	m_agOwnershipFiles.RemoveAll();
	for(int i=0; i< m_lstOwnershipFiles.GetItemCount(); i++)
	{
		m_agOwnershipFiles.Add(m_lstOwnershipFiles.GetItemText(i,1));
	}
	if(m_agOwnershipFiles.GetSize() == 0)
	{
		AfxMessageBox(_T("Please input ownership files!"));
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

BOOL CDlgLoadMids::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lstOwnershipFiles.SetBkColor(RGB(232, 232, 232));	
	m_lstOwnershipFiles.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstOwnershipFiles.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstOwnershipFiles.SetExtendedStyle(dwExStyle);	
	m_lstOwnershipFiles.InsertColumn(0, _T("Number Range"),LVCFMT_LEFT,120);
	m_lstOwnershipFiles.InsertColumn(1, _T("Path"),LVCFMT_LEFT,180);
	
	
	int nCount = g_theApp.m_agNumberSector.GetSize();
	int nIndex = 0;
	for(int i=0; i< nCount; i = i+2)
	{
		CString strID;
		strID.Format(_T("%d-%d"),g_theApp.m_agNumberSector[i],g_theApp.m_agNumberSector[i+1]);
		m_lstOwnershipFiles.InsertItem(nIndex++,strID);
	}

	if(!m_bSelCityFile)
	{
		GetDlgItem(IDC_DLC_EDT_CITY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLC_BTN_SEL_CITY)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoadMids::OnDblclkDssLstDstFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;
	
	CEdtBtnCtrl *pCtrl = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	if(nItem == -1 || !(nSubItem == 1 || nSubItem == 2))
		return;
	
	
	pCtrl = new CEdtBtnCtrl();
	
	if(NULL != pCtrl)
	{
		pCtrl->m_nItem = nItem;
		pCtrl->m_nSubItem = nSubItem;
		pCtrl->m_strText = m_lstOwnershipFiles.GetItemText(nItem,nSubItem);
		
		pCtrl->m_bOpen = TRUE;
		
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(m_lstOwnershipFiles.m_hWnd)))
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
	m_lstOwnershipFiles.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	CRect rtList;
	m_lstOwnershipFiles.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top,NULL);
	
    pCtrl->ShowWindow(SW_SHOW);	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgLoadMids::OnEndlabeleditDssLstDstFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstOwnershipFiles.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}
