// DlgGenQuhao.cpp : implementation file
//

#include "stdafx.h"
#include "ownership.h"
#include "DlgGenQuhao.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define R_ACC_CITY_TYPE_QUHAO _T("R_ACC_CITY_TYPE_QUHAO")
#define R_ACC_CITYID_QUHAO	  _T("R_ACC_CITYID_QUHAO")
#define R_ACC_QUHAOCITY       _T("R_ACC_QUHAOCITY")
#define R_ACC_MPLOCSTR        _T("R_ACC_MPLOCSTR")

/////////////////////////////////////////////////////////////////////////////
// CDlgGenQuhao dialog


CDlgGenQuhao::CDlgGenQuhao(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenQuhao::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGenQuhao)
	m_strCityQuhaoFile = R_ACC_CITY_TYPE_QUHAO;
	m_strQuhaoFile = R_ACC_CITYID_QUHAO;
	m_strQuhaoCityFile = R_ACC_QUHAOCITY;
	m_strCityOutFile = R_ACC_MPLOCSTR;
	m_strCityInFile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgGenQuhao::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGenQuhao)
	DDX_Text(pDX, IDC_EDT_CITY_QUHAO, m_strCityQuhaoFile);
	DDX_Text(pDX, IDC_EDT_QUHAO, m_strQuhaoFile);
	DDX_Text(pDX, IDC_EDT_QUHAO_CITY, m_strQuhaoCityFile);
	DDX_Text(pDX, IDC_DQ_EDT_CITY_OUT, m_strCityOutFile);
	DDX_Text(pDX, IDC_DQ_EDT_CITY, m_strCityInFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGenQuhao, CDialog)
	//{{AFX_MSG_MAP(CDlgGenQuhao)
	ON_BN_CLICKED(IDC_BTN_CQ_BROW, OnBtnCqBrow)
	ON_BN_CLICKED(IDC_BTN_QC_BROW, OnBtnQcBrow)
	ON_BN_CLICKED(IDC_BTN_Q_BROW, OnBtnQBrow)
	ON_BN_CLICKED(IDC_BTN_C_BROW, OnBtnCBrow)
	ON_BN_CLICKED(IDC_BTN_C_OUT_BROW, OnBtnCOutBrow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGenQuhao message handlers

void CDlgGenQuhao::OnBtnCqBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE,_T(".dat"),R_ACC_CITY_TYPE_QUHAO,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("City quhao files (*.dat)|*.dat|All files (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strCityQuhaoFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgGenQuhao::OnBtnQcBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE,_T(".dat"),R_ACC_QUHAOCITY,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Quhao City file (*.dat)|*.dat|All files (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strQuhaoCityFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgGenQuhao::OnBtnQBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE,_T(".dat"),R_ACC_CITYID_QUHAO,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Quhao files (*.dat)|*.dat|All files (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strQuhaoFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgGenQuhao::OnBtnCBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".dat"),R_ACC_MPLOCSTR,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("Quhao files (*.dat)|*.dat|All files (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strCityInFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgGenQuhao::OnBtnCOutBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE,_T(".dat"),R_ACC_MPLOCSTR,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Quhao files (*.dat)|*.dat|All files (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strCityOutFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CDlgGenQuhao::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
//	if(!m_strQuhaoFile.IsEmpty() && m_strCityOutFile.IsEmpty())
	CDialog::OnOK();
}



BOOL CDlgGenQuhao::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
