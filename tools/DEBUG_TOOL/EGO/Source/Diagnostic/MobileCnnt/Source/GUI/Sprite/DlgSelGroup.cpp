// DlgSelGroup.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "DlgSelGroup.h"
#include "InfoWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelGroup dialog


CDlgSelGroup::CDlgSelGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelGroup)
	m_nGroup = -1;
	//}}AFX_DATA_INIT
}


void CDlgSelGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelGroup)
	DDX_Control(pDX, IDC_CMB_GROUP, m_cmbGroup);
	DDX_Control(pDX, IDOK, m_btnCancel);
	DDX_Control(pDX, IDCANCEL, m_btnOK);
	DDX_Control(pDX, IDC_LST_GMEMBER, m_lstGM);
	DDX_CBIndex(pDX, IDC_CMB_GROUP, m_nGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelGroup, CDialog)
	//{{AFX_MSG_MAP(CDlgSelGroup)
	ON_CBN_SELCHANGE(IDC_CMB_GROUP, OnSelchangeCmbGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelGroup message handlers

void CDlgSelGroup::OnOK() 
{
	// TODO: Add extra validation here
	m_arrSel.RemoveAll();
	int nCount = m_lstGM.GetItemCount();
	for( int i = 0; i < nCount; ++i )
	{
		if( m_lstGM.GetCheck(i) )
			m_arrSel.Add(m_lstGM.GetItemData(i));
	}

	CDialog::OnOK();
}

BOOL CDlgSelGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    InitCmbCtrl();
	InitLstCtrl();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelGroup::InitCmbCtrl()
{
	_ASSERTE( theApp != NULL );

	CInfoWrapper * pIW =theApp.GetInfoWrapper();
	_ASSERTE( pIW != NULL );

	int nCount = pIW->GetPBGCount(ID_STORE_ALL);
	if(nCount==0)
		return;
	for(int i=0;i<nCount;i++)
		m_cmbGroup.AddString((pIW->GetPBG(i))->szName);
	m_nGroup = 0;
	UpdateData(FALSE);
}

void CDlgSelGroup::InitLstCtrl()
{
	m_lstGM.ModifyStyle(0, LVS_SHOWSELALWAYS);

	DWORD dwExStyle = m_lstGM.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;

	m_lstGM.SetExtendedStyle(dwExStyle);

	CString strClmn;
	VERIFY( strClmn.LoadString(IDS_SELNUM_CLMN) );

	int     nLen   = strClmn.GetLength();
	LPTSTR  lpBuf  = strClmn.GetBuffer(nLen);
	LPTSTR  lpFind = _tcschr(lpBuf, _T(','));
	_ASSERTE( lpFind != NULL );
	
	// index
	*lpFind = _T('\0');
	m_lstGM.InsertColumn(0, lpBuf, LVCFMT_LEFT, 50 );

	// name
	lpBuf  = lpFind + 1;
	lpFind = _tcschr(lpBuf, _T(','));
	_ASSERTE( lpFind != NULL );
	*lpFind = _T('\0');
	m_lstGM.InsertColumn(1, lpBuf, LVCFMT_LEFT, 90 );

	// num
	lpBuf = lpFind + 1;
	m_lstGM.InsertColumn(2, lpBuf, LVCFMT_LEFT, 130 );

	strClmn.ReleaseBuffer(nLen);

	_ASSERTE( theApp != NULL );

	CInfoWrapper * pIW = theApp.GetInfoWrapper();
	_ASSERTE( pIW != NULL );
    if(m_nGroup == -1)
		return;

	_TCHAR szIdx[8];
	PPB pPb    = NULL;
	int nCount = pIW->GetPBCount(ID_STORE_ALL);
	int k=0;
		
	for( int i = 0; i < nCount; ++i )
	{
		UINT nPbgID = pIW->GetPBG(0)->nID;
		pPb = pIW->GetPB(i);
		if(pPb->nGroupIndex == nPbgID && pPb->wStore != ID_STORE_PC)
		{
			_stprintf(szIdx, _T("%d"), k + 1);
		    m_lstGM.InsertItem(k, szIdx);
			m_lstGM.SetItemText(k, 1, pPb->szName);
			m_lstGM.SetItemText(k, 2, pPb->szPhone);
			m_lstGM.SetCheck(k);
			k++;
			m_lstGM.SetItemData(k,(DWORD)i);
		}
	}
}

void CDlgSelGroup::OnSelchangeCmbGroup() 
{
	// TODO: Add your control notification handler code here
    UpdateData();

	m_lstGM.DeleteAllItems();


	CInfoWrapper * pIW = theApp.GetInfoWrapper();
	_ASSERTE( pIW != NULL );
    if(m_nGroup == -1)
		return;

	_TCHAR szIdx[8];
	PPB pPb    = NULL;
	int nCount = pIW->GetPBCount(ID_STORE_ALL);
	int k=0;
	
	for( int i = 0; i < nCount; ++i )
	{
		UINT nPbgID = pIW->GetPBG(m_nGroup)->nID;
		pPb = pIW->GetPB(i);
		if(pPb->nGroupIndex == nPbgID)
		{
			_stprintf(szIdx, _T("%d"), k + 1);
		    m_lstGM.InsertItem(k, szIdx);
			m_lstGM.SetItemText(k, 1, pPb->szName);
			m_lstGM.SetItemText(k, 2, pPb->szPhone);
			m_lstGM.SetCheck(k);
			m_lstGM.SetItemData(k,(DWORD)i);
			k++;
		}
	}
}
