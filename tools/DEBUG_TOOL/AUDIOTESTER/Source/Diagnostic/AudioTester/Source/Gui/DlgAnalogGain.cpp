// DlgAnalogGain.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgAnalogGain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalogGain dialog


CDlgAnalogGain::CDlgAnalogGain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalogGain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnalogGain)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBIndexGain = 0;
	m_nCBIndexGIM = 0;
	//}}AFX_DATA_INIT
}


void CDlgAnalogGain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalogGain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX, IDC_COMBO_ANALOG_GAIN, m_nCBIndexGain);
	DDX_CBIndex(pDX, IDC_COMBO_GIM, m_nCBIndexGIM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnalogGain, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalogGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalogGain message handlers

BOOL CDlgAnalogGain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_VolumeData);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAnalogGain::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_VolumeData);
	
	CDialog::OnOK();
}

void CDlgAnalogGain::SetValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgAnalogGain::FetchValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

void CDlgAnalogGain::InitValue(VOLUME_STRUCT& volume_data)
{
	m_nCBIndexGain = volume_data.reserve[43] & 0x00ff;

	int nGim = (volume_data.reserve[43] & 0xff00) >> 8;
	if(nGim == 0)
	{
		m_nCBIndexGIM = 0;
	}
	else if(nGim == 1)
	{
		m_nCBIndexGIM = 1;
	}
	UpdateData(FALSE);
}

void CDlgAnalogGain::GetValue(VOLUME_STRUCT& volume_data)
{
	UpdateData();

	int nGim = 0;
	if(m_nCBIndexGIM == 0)
	{
		nGim = 0;
	}
	else
	{
		nGim = 1;
	}
	
	volume_data.reserve[43] = (m_nCBIndexGain | (nGim << 8));
	
}
