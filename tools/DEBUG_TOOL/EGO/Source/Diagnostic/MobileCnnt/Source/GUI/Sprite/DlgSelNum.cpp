// DlgSelNum.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "InfoWrapper.h"
#include "DlgSelNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelNum dialog


CDlgSelNum::CDlgSelNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelNum)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nIndex = -1;
	m_strSearchOldName = _T("");
}


void CDlgSelNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelNum)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_FIND, m_btnFind);
	DDX_Control(pDX, IDC_LST_NAMENUM, m_lst);
	DDX_Text(pDX, IDC_EDT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelNum, CDialog)
	//{{AFX_MSG_MAP(CDlgSelNum)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelNum message handlers

BOOL CDlgSelNum::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_edtName.SubclassDlgItem(IDC_EDT_NAME, this);

	InitLstCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelNum::InitLstCtrl()
{

	m_lst.ModifyStyle(0, LVS_SHOWSELALWAYS);

	DWORD dwExStyle = m_lst.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_CHECKBOXES;

	m_lst.SetExtendedStyle(dwExStyle);

	CString strClmn;
	VERIFY( strClmn.LoadString(IDS_SELNUM_CLMN) );

	int     nLen   = strClmn.GetLength();
	LPTSTR  lpBuf  = strClmn.GetBuffer(nLen);
	LPTSTR  lpFind = _tcschr(lpBuf, _T(','));
	_ASSERTE( lpFind != NULL );
	
	// index
	*lpFind = _T('\0');
	m_lst.InsertColumn(0, lpBuf, LVCFMT_LEFT, 50 );

	// name
	lpBuf  = lpFind + 1;
	lpFind = _tcschr(lpBuf, _T(','));
	_ASSERTE( lpFind != NULL );
	*lpFind = _T('\0');
	m_lst.InsertColumn(1, lpBuf, LVCFMT_LEFT, 90 );

	// num
	lpBuf = lpFind + 1;
	m_lst.InsertColumn(2, lpBuf, LVCFMT_LEFT, 130 );

	strClmn.ReleaseBuffer(nLen);

	CInfoWrapper * pIW = theApp.GetInfoWrapper();
	_ASSERTE( pIW != NULL );

	_TCHAR szIdx[8];
	PPB pPb    = NULL;
	int nCount = pIW->GetPBCount(ID_STORE_ALL);
	for( int i = 0; i < nCount; ++i )
	{
		_stprintf(szIdx, _T("%d"), i + 1);
		m_lst.InsertItem(i, szIdx);

		pPb = pIW->GetPB(i);
		m_lst.SetItemText(i, 1, pPb->szName);
		m_lst.SetItemText(i, 2, pPb->szPhone);
	}
	// 测试用
	/*
	m_lst.InsertItem(0, "1");
	m_lst.SetItemText(0, 1, "张00");
	m_lst.SetItemText(0, 2, "1234567489");

	m_lst.InsertItem(1, "2");
	m_lst.SetItemText(1, 1, "张11");
	m_lst.SetItemText(1, 2, "111111111");

	m_lst.InsertItem(2, "3");
	m_lst.SetItemText(2, 1, "张22");
	m_lst.SetItemText(2, 2, "2222222");
	*/
}

void CDlgSelNum::OnBtnFind() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if( m_strName.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_PB_NAME_EMPTY);
		return;
	}
	if(m_strName.Compare(m_strSearchOldName)!=0)
	{
		m_nIndex = -1;
		m_strSearchOldName = m_strName;
	}

	static const int nNameLen = 32;
	_TCHAR szName[nNameLen + 1] = { 0 };
	int nCount = m_lst.GetItemCount();
	int i=0;
	for( i = m_nIndex+1; i < nCount; ++i )
	{
		if(!(m_lst.GetItemText(i, 1, szName, nNameLen)))
			continue;
		if( m_strName.FindOneOf(szName) >= 0 )
		{
			m_lst.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
			m_lst.SetFocus();
            m_lst.EnsureVisible(i, FALSE);
			m_nIndex = i;
			break;			
		}
	}
	if((nCount == 0 || i >= nCount)&& m_nIndex==-1)
		AfxMessageBox(IDS_NO_RECORD_FOUND);

	if(i>=nCount && m_nIndex != -1)
	{
		m_nIndex = -1;
		for( i = m_nIndex+1; i < nCount; ++i )
		{
			if(!(m_lst.GetItemText(i, 1, szName, nNameLen)))
				continue;
			if( m_strName.FindOneOf(szName) >= 0 )
			{
				m_lst.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED );
				m_lst.SetFocus();
				m_lst.EnsureVisible(i, FALSE);
				m_nIndex = i;
				break;			
			}
		}
	}
}

void CDlgSelNum::OnOK() 
{
	// TODO: Add extra validation here
	m_arrSel.RemoveAll();
	int nCount = m_lst.GetItemCount();
	for( int i = 0; i < nCount; ++i )
	{
		if( m_lst.GetCheck(i) )
			m_arrSel.Add(i);
	}

	CDialog::OnOK();
}
