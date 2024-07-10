// GSensorProperyPage.cpp : implementation file
//

#include "stdafx.h"
#include "GSensorProperyPage.h"
#include "ConPanPropertySheet.h"

#include "ConPanLayer.h"
#include "ConPanFunc.h"
#include "ConPan.h"
#include "signals_header.h"
#include "mmi_signal.h"

#define CP_GSENSOR

#ifdef CP_GSENSOR
#include "msensor_drv.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGSensorProperyPage property page

IMPLEMENT_DYNCREATE(CGSensorProperyPage, CPropertyPage)

CGSensorProperyPage::CGSensorProperyPage() : CPropertyPage(CGSensorProperyPage::IDD)
{
	//{{AFX_DATA_INIT(CGSensorProperyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGSensorProperyPage::~CGSensorProperyPage()
{
}

void CGSensorProperyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGSensorProperyPage)
	DDX_Control(pDX, IDC_GSP_CMB_GSENSOR, m_cmbGSStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGSensorProperyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGSensorProperyPage)
	ON_CBN_SELCHANGE(IDC_GSP_CMB_GSENSOR, OnSelchangeCmbGsensor)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_TESTDM, OnTestDM)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGSensorProperyPage message handlers

BOOL CGSensorProperyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
/*
    MMK_GSENSOR_STATUS_SHAKE = 0x00, 	// NO direction, Only detect shake        
	//�״̬    
	MMK_GSENSOR_STATUS_LEFT,
	MMK_GSENSOR_STATUS_RIGHT,
	MMK_GSENSOR_STATUS_UP,
	MMK_GSENSOR_STATUS_DOWN,
	MMK_GSENSOR_STATUS_FORWARD,
	MMK_GSENSOR_STATUS_BACKWARD,		
	//��ֹ״̬
	MMK_GSENSOR_STATUS_LEFT_SIDE,
	MMK_GSENSOR_STATUS_RIGHT_SIDE,	
	MMK_GSENSOR_STATUS_UP_SIDE,
	MMK_GSENSOR_STATUS_DOWN_SIDE,
	MMK_GSENSOR_STATUS_FORWRAD_SIDE,
    MMK_GSENSOR_STATUS_BACKWARD_SIDE,	
	MMK_GSENSOR_STATUS_NONE
*/
	// TODO: Add extra initialization here
	m_cmbGSStatus.AddString(_T("Shake"));
	m_cmbGSStatus.AddString(_T("Left"));
	m_cmbGSStatus.AddString(_T("Right"));
	m_cmbGSStatus.AddString(_T("Up"));
	m_cmbGSStatus.AddString(_T("Down"));
	m_cmbGSStatus.AddString(_T("Forward"));
	m_cmbGSStatus.AddString(_T("Backward"));
	m_cmbGSStatus.AddString(_T("LeftSide"));
	m_cmbGSStatus.AddString(_T("RightSide"));
	m_cmbGSStatus.AddString(_T("UpSide"));
	m_cmbGSStatus.AddString(_T("DownSide"));
	m_cmbGSStatus.AddString(_T("ForwardSide"));
	m_cmbGSStatus.AddString(_T("BackwardSide"));
//	m_cmbGSStatus.AddString(_T("None"));

//	m_cmbGSStatus.SetCurSel(m_cmbGSStatus.GetCount()-1);
	m_cmbGSStatus.SetCurSel(9); //upside


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGSensorProperyPage::OnSelchangeCmbGsensor() 
{
	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(1) &&
		!((CConPanPropertySheet *)GetParent())->IsPowerOn(2) )
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}

	int nCurSel = m_cmbGSStatus.GetCurSel();
	if(nCurSel == m_cmbGSStatus.GetCount()-1)
	{
		return;
	}

#ifdef CP_GSENSOR
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
		APP_GSENSOR,
		sizeof(MmiKeyPress)+sizeof(xSignalHeaderRec),
		P_APP);

	int nStatus = MSENSOR_MOTION_TYPE_UP_SIDE;
	switch(nCurSel)
	{
	case 0:
		nStatus = MSENSOR_MOTION_TYPE_SHAKE;
		break;
	case 1:
		nStatus = MSENSOR_MOTION_TYPE_LEFT;
		break;
	case 2:
		nStatus = MSENSOR_MOTION_TYPE_RIGHT;
	    break;
	case 3:
		nStatus = MSENSOR_MOTION_TYPE_UP;
	    break;
	case 4:
		nStatus = MSENSOR_MOTION_TYPE_DOWN;
		break;
	case 5:
		nStatus = MSENSOR_MOTION_TYPE_FORWARD;
		break;
	case 6:
		nStatus = MSENSOR_MOTION_TYPE_BACKWARD;
	    break;
	case 7:
		nStatus = MSENSOR_MOTION_TYPE_LEFT_SIDE;
		break;
	case 8:
		nStatus = MSENSOR_MOTION_TYPE_RIGHT_SIDE;
		break;
	case 9:
		nStatus = MSENSOR_MOTION_TYPE_UP_SIDE;
		break;
	case 10:
		nStatus = MSENSOR_MOTION_TYPE_DOWN_SIDE;
		break;
	case 11:
		nStatus = MSENSOR_MOTION_TYPE_FORWRAD_SIDE;
		break;
	case 12:
		nStatus = MSENSOR_MOTION_TYPE_BACKWARD_SIDE;
		break;
	default:
	    break;
	}

	((MmiKeyPress*)(sig_ptr+1))->keyCode = nStatus;	
	
    // Save the thread context first
    InnerThreadContextSave();
	// Send message
	InnerSCISendSignal((xSignalHeader) sig_ptr, P_APP);
    // Restore the thread context
    InnerThreadContextRestore();
#else
	MessageBox("Sorry, not support GSensor!", "Warning", MB_ICONWARNING);
#endif

}

void CGSensorProperyPage::OnTestDM()
{
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(1) &&
		!((CConPanPropertySheet *)GetParent())->IsPowerOn(2) )
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
		APP_DM_MESSAGE_IND,
		sizeof(MmiKeyPress)+sizeof(xSignalHeaderRec),
		P_APP);

	((MmiKeyPress*)(sig_ptr+1))->keyCode = 0xFE;	
	
    // Save the thread context first
    InnerThreadContextSave();
	// Send message
	InnerSCISendSignal((xSignalHeader) sig_ptr, P_APP);
    // Restore the thread context
    InnerThreadContextRestore();
}