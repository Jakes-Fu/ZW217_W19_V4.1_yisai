// DlgReport.cpp : implementation file
//

#include "stdafx.h"
#include "ResOver.h"
#include "DlgReport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReport dialog


CDlgReport::CDlgReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pResParser = NULL;
}

CDlgReport::~CDlgReport()
{
	m_pResParser = NULL;
}
void CDlgReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReport)
	DDX_Control(pDX, IDC_LST_STAT, m_lstReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReport, CDialog)
	//{{AFX_MSG_MAP(CDlgReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReport message handlers

BOOL CDlgReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lstReport.SetBkColor(RGB(232, 232, 232));	
	m_lstReport.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstReport.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstReport.SetExtendedStyle(dwExStyle);

	m_lstReport.InsertColumn(0,_T("Desc"),LVCFMT_LEFT,200);
	m_lstReport.InsertColumn(1,_T("Theme"),LVCFMT_LEFT,80);
	m_lstReport.InsertColumn(2,_T("Type"),LVCFMT_LEFT,80);
	m_lstReport.InsertColumn(3,_T("OrgSize"),LVCFMT_LEFT,80);
	m_lstReport.InsertColumn(4,_T("NewSize"),LVCFMT_LEFT,80);
	m_lstReport.InsertColumn(5,_T("DeltSize"),LVCFMT_LEFT,80);

	if(m_pResParser != NULL)
	{
		std::vector<ITEM_STAT_T> vItemStat;
		m_pResParser->GetResItemStat(vItemStat);
		UINT nCount = vItemStat.size();
		CString strTmp;
		for(UINT i = 0;  i< nCount; i++)
		{
			UINT nItemIndex = vItemStat[i].nItemIndex;
			UINT nThemeIndex = vItemStat[i].nThemeIndex;
			RES_ITEM_PTR pRI = m_pResParser->m_vResItem[nItemIndex];
			m_lstReport.InsertItem(i,pRI->szDesc);
			m_lstReport.SetItemText(i,1,m_pResParser->m_agThemeName[nThemeIndex].operator LPCTSTR());
			m_lstReport.SetItemText(i,2,m_pResParser->GetResTypeString(pRI->uResType).operator LPCTSTR());
			strTmp.Format(_T("%d"),ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize));
			m_lstReport.SetItemText(i,3,strTmp.operator LPCTSTR());

			strTmp.Format(_T("%d"),ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].rep.dwSize));
			m_lstReport.SetItemText(i,4,strTmp.operator LPCTSTR());

			strTmp.Format(_T("%d"),vItemStat[i].nDeltSize);
			m_lstReport.SetItemText(i,5,strTmp.operator LPCTSTR());
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
