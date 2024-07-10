// EditItemID.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "EditItemID.h"

#include "FileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditItemID dialog


CEditItemID::CEditItemID(CWnd* pParent /*=NULL*/)
	: CDialog(CEditItemID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditItemID)
	m_nNewID = 0;	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CEditItemID::SetProject(CFileParser *pProject, UINT nModuleIndex, UINT nItemIndex)
{
	ASSERT(NULL != pProject);
	if(pProject == NULL)
	{
		return;
	}
	
	m_pProject = pProject;
	m_nTotalItemCount = m_pProject->GetModuleItemCount(nModuleIndex);
	m_nCurrItemIndex = nItemIndex;
	m_nModuleIndex = nModuleIndex;   
}

UINT CEditItemID::GetNewID()
{
	return m_nNewID;
}

void CEditItemID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditItemID)
	DDX_Control(pDX, IDC_IT_NAME, m_ctrlItemName);
	DDX_Control(pDX, IDC_OLDITEM_ID, m_ctrlOldID);
	DDX_Control(pDX, IDC_NEWITEM_ID, m_ctrlNewID);
	DDX_Text(pDX, IDC_IT_NAME, m_strName);
	DDX_Text(pDX, IDC_OLDITEM_ID, m_nOldItemID);
	//DDX_Text(pDX, IDC_NEWITEM_ID, m_nNewItemID);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CEditItemID::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	ModuleItem tmpItem = m_pProject->GetModuleItem(m_nModuleIndex, m_nCurrItemIndex);
	m_strName = tmpItem.szItemName;
	m_nOldItemID = tmpItem.nItemID;
	m_nNewID = 0;
	UpdateData(FALSE);

	TCHAR szContent[MAX_ITEM_CONTENT] = {0};
	_stprintf(szContent, _T("0x%X"), m_nOldItemID);
	m_ctrlOldID.SetWindowText(szContent);
	m_ctrlNewID.SetWindowText(_T("0"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditItemID::OnOK() 
{
	CString strItemID;
	m_ctrlNewID.GetWindowText(strItemID);

	strItemID.MakeUpper();
	if(strItemID.Find(_T("0X")) == 0)
	{
		_stscanf(strItemID.operator LPCTSTR(), _T("0X%X"), &m_nNewID);
	}
	else
	{
		_stscanf(strItemID.GetBuffer(0), _T("%d"), &m_nNewID);
	}	
	
	// check repeated ID
	if (m_pProject->IsExistedID(m_nNewID))
	{
		AfxMessageBox(_T("Repeated ID"));
		return;
	}

	//update children's ID

	EndDialog(IDOK);

}

BEGIN_MESSAGE_MAP(CEditItemID, CDialog)
	//{{AFX_MSG_MAP(CEditItemID)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditItemID message handlers
