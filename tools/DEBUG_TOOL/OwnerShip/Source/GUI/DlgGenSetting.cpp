// DlgGenSetting.cpp : implementation file
//

#include "stdafx.h"
#include "Ownership.h"
#include "DlgGenSetting.h"
#include "EdtBtnCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGenSetting dialog


CDlgGenSetting::CDlgGenSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGenSetting)
	//}}AFX_DATA_INIT
}


void CDlgGenSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGenSetting)
	DDX_Control(pDX, IDC_DGS_LST_CITY_MAP_FILES, m_lstDstFiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGenSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgGenSetting)
	ON_NOTIFY(NM_DBLCLK, IDC_DGS_LST_CITY_MAP_FILES, OnDblclkDgsLstOwnershipFiles)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_DGS_LST_CITY_MAP_FILES, OnEndlabeleditDgsLstOwnershipFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGenSetting message handlers

BOOL CDlgGenSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lstDstFiles.SetBkColor(RGB(232, 232, 232));	
	m_lstDstFiles.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstDstFiles.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstDstFiles.SetExtendedStyle(dwExStyle);	
	m_lstDstFiles.InsertColumn(0, _T("Number Range"),LVCFMT_LEFT,130);
	m_lstDstFiles.InsertColumn(1, _T("TargetPath"),LVCFMT_LEFT,216);

	
	int nCount = g_theApp.m_agNumberSector.GetSize();
	int nIndex = 0;
	for(int i=0; i< nCount; i = i+2)
	{		
		if(m_agFlag.GetAt(i/2) != 0)
		{
			CString strID;
			strID.Format(_T("%d-%d"),g_theApp.m_agNumberSector[i],g_theApp.m_agNumberSector[i+1]);
			m_lstDstFiles.InsertItem(nIndex++,strID);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGenSetting::OnOK() 
{
	// TODO: Add extra validation here
	m_agDstFiles.RemoveAll();
	for(int i=0; i< m_lstDstFiles.GetItemCount(); i++)
	{
		m_agDstFiles.Add(m_lstDstFiles.GetItemText(i,1));
	}
	if(m_agDstFiles.GetSize()==0)
	{
		AfxMessageBox(_T("Please input target files!"));
		return;
	}

	CDialog::OnOK();
}

void CDlgGenSetting::OnDblclkDgsLstOwnershipFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
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
		pCtrl->m_strText = m_lstDstFiles.GetItemText(nItem,nSubItem);
	
		pCtrl->m_bOpen = FALSE;
		
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(m_lstDstFiles.m_hWnd)))
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
	m_lstDstFiles.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	CRect rtList;
	m_lstDstFiles.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top,NULL);
	
    pCtrl->ShowWindow(SW_SHOW);
	
	*pResult = 0;
}

void CDlgGenSetting::OnEndlabeleditDgsLstOwnershipFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstDstFiles.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}
