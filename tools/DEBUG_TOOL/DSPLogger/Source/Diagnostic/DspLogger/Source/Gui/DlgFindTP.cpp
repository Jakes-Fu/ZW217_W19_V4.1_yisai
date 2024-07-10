// DlgFindTP.cpp : implementation file
//
#include "stdafx.h"
#include "dsplogger.h"
#include "DlgFindTP.h"

#include "TestPointsView.h"

#define MAX_COUNT 20

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFindTP dialog

CDlgFindTP::CDlgFindTP(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindTP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindTpDlg)
	m_strAddr = _T("");
	m_strData = _T("");
	m_strDesc = _T("");
	//}}AFX_DATA_INIT
    m_bDire = FALSE;
    m_pView = NULL;

    m_cmbAddr.SetMaxMruCount(10);
	CString strAddList("Address list");
	m_cmbAddr.SetMruSection(strAddList);

    CString strAddFormat("Address%d");
    m_cmbAddr.SetMruEntryFormat(strAddFormat);

    m_cmbData.SetMaxMruCount(10);
	CString strDataList("Data list");
    m_cmbData.SetMruSection(strDataList);
	CString strDataFormat("Data%d");
    m_cmbData.SetMruEntryFormat(strDataFormat);
}


void CDlgFindTP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindTpDlg)
	DDX_CBString(pDX, IDC_COMBO_ADDR, m_strAddr);
	DDX_CBString(pDX, IDC_COMBO_DATA, m_strData);
	DDX_Text(pDX, IDC_EDT_SEARCH_DESC, m_strDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindTP, CDialog)
	//{{AFX_MSG_MAP(CFindTpDlg)
	ON_BN_CLICKED(IDC_RADIO_UP, OnRadioUp)
	ON_BN_CLICKED(IDC_RADIO_DOWN, OnRadioDown)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindTP message handlers

void CDlgFindTP::OnRadioUp() 
{
    m_bDire	= TRUE;
}

void CDlgFindTP::OnRadioDown() 
{
	m_bDire = FALSE;	
}

BOOL CDlgFindTP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UINT uID;
    if(m_bDire)
        uID = IDC_RADIO_UP;
    else
        uID = IDC_RADIO_DOWN;

	CButton* pButton = (CButton*)GetDlgItem(uID);
	pButton->SetCheck(1);

    m_cmbAddr.SubclassDlgItem(IDC_COMBO_ADDR,this);
    m_cmbData.SubclassDlgItem(IDC_COMBO_DATA,this);

    LoadStrings();

    m_cmbData.LimitText(4);
    m_cmbData.SetCurSel(0);

    m_cmbAddr.LimitText(4);
    m_cmbAddr.SetCurSel(0);
    m_cmbAddr.SetFocus();

    return FALSE;
                  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFindTP::OnOK() 
{	
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
    
    m_cmbAddr.Add2MruList();
    m_cmbData.Add2MruList();

    if(m_pView != NULL)
    {
        m_pView->FindTp(m_strAddr,m_strData,m_bDire,m_strDesc);
    }
}

void CDlgFindTP::LoadStrings()
{
    m_cmbAddr.LoadMruList();

    m_cmbData.LoadMruList();
}

void CDlgFindTP::SaveStrings()
{
    m_cmbAddr.SaveMruList();
    m_cmbData.SaveMruList();
}

void CDlgFindTP::OnDestroy() 
{
	CDialog::OnDestroy();
    SaveStrings();	
}
