// XagcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "XagcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXagcDlg dialog


CXagcDlg::CXagcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXagcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXagcDlg)
	m_nMode = 0;
	m_nNoiseGateOn = 1;
	m_nTargetTight = 0;
	m_nAttack = 0;
	m_nDecay = 2;
	m_nFrameTime = 0;
	m_nHoldTime = 4;
	m_nMaxPgaGain = 11;
	m_nMinPgaGain = 0;
	m_nNoiseGateAttenu = 512;
	m_nNoiseGateThresh = 6;
	m_nPgaStep = 0;
	m_nTarget = 0;
	m_nNoiseGateHold = 0;
	//}}AFX_DATA_INIT

    m_pArea = NULL;
}


void CXagcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXagcDlg)
	DDX_CBIndex(pDX, IDC_COMBO_XAGC_MODE, m_nMode);
	DDX_CBIndex(pDX, IDC_COMBO_XAGC_NOISE_GATE_ON, m_nNoiseGateOn);
	DDX_CBIndex(pDX, IDC_COMBO_XAGC_TARGET_TIGHT, m_nTargetTight);
	DDX_Text(pDX, IDC_EDIT_XAGC_ATTACK, m_nAttack);
	DDV_MinMaxInt(pDX, m_nAttack, 0, 7);
	DDX_Text(pDX, IDC_EDIT_XAGC_DECAY, m_nDecay);
	DDV_MinMaxInt(pDX, m_nDecay, 0, 7);
	DDX_Text(pDX, IDC_EDIT_XAGC_FRAME_TIME, m_nFrameTime);
	DDV_MinMaxInt(pDX, m_nFrameTime, 0, 7);
	DDX_Text(pDX, IDC_EDIT_XAGC_HOLDTIME, m_nHoldTime);
	DDV_MinMaxInt(pDX, m_nHoldTime, 0, 31);
	DDX_Text(pDX, IDC_EDIT_XAGC_MAX_PGA_GAIN, m_nMaxPgaGain);
	DDV_MinMaxInt(pDX, m_nMaxPgaGain, 0, 12);
	DDX_Text(pDX, IDC_EDIT_XAGC_MIN_PGA_GAIN, m_nMinPgaGain);
	DDV_MinMaxInt(pDX, m_nMinPgaGain, 0, 12);
	DDX_Text(pDX, IDC_EDIT_XAGC_NOISE_GATE_ATTENU, m_nNoiseGateAttenu);
	DDV_MinMaxInt(pDX, m_nNoiseGateAttenu, 0, 4096);
	DDX_Text(pDX, IDC_EDIT_XAGC_NOISE_GATE_THRESH, m_nNoiseGateThresh);
	DDV_MinMaxInt(pDX, m_nNoiseGateThresh, 0, 7);
	DDX_Text(pDX, IDC_EDIT_XAGC_PGA_STEP, m_nPgaStep);
	DDV_MinMaxInt(pDX, m_nPgaStep, 0, 7);
	DDX_Text(pDX, IDC_EDIT_XAGC_TARGET, m_nTarget);
	DDV_MinMaxInt(pDX, m_nTarget, 0, 7);
	DDX_Text(pDX, IDC_EDIT_XAGC_NOISE_GATE_HOLD, m_nNoiseGateHold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXagcDlg, CDialog)
	//{{AFX_MSG_MAP(CXagcDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXagcDlg message handlers

void CXagcDlg::OnOK() 
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }

    m_pArea->m_nAecEnable = (short)m_nMode;
    m_pArea->m_nPdelay = (short)m_nPgaStep;
    m_pArea->m_nCoeffFrozen = (short)m_nTargetTight;
    m_pArea->m_nDtDectThreshold = (short)m_nFrameTime;
    m_pArea->m_nStepSize = (short)m_nAttack;
    m_pArea->m_nSaCtrl = (short)m_nNoiseGateOn;
    m_pArea->m_nSendThreshold = (short)m_nHoldTime;
    m_pArea->m_nR2dtThreshold = (short)m_nNoiseGateHold;
    m_pArea->m_nS2dtThreshold = (short)m_nNoiseGateAttenu;
    m_pArea->m_nRecvThreshold = (short)m_nDecay;
    m_pArea->m_nBn40 = (short)m_nTarget;
    m_pArea->m_nSignalDelay = (short)m_nMinPgaGain;
    m_pArea->m_nSaDelay = (short)m_nMaxPgaGain;
    m_pArea->m_nDtNoiseFloor = (short)m_nNoiseGateThresh;

    EndDialog(IDOK);
}

BOOL CXagcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_nMode = m_pArea->m_nAecEnable;
    m_nPgaStep = m_pArea->m_nPdelay;
    m_nTargetTight = m_pArea->m_nCoeffFrozen;
    m_nFrameTime = m_pArea->m_nDtDectThreshold;
    m_nAttack = m_pArea->m_nStepSize;
    m_nNoiseGateOn = m_pArea->m_nSaCtrl;
    m_nHoldTime = m_pArea->m_nSendThreshold;
    m_nNoiseGateHold = m_pArea->m_nR2dtThreshold;
    m_nNoiseGateAttenu = m_pArea->m_nS2dtThreshold;
    m_nDecay = m_pArea->m_nRecvThreshold;
    m_nTarget = m_pArea->m_nBn40;
    m_nMaxPgaGain = m_pArea->m_nSaDelay;
    m_nMinPgaGain = m_pArea->m_nSignalDelay;
    m_nNoiseGateThresh = m_pArea->m_nDtNoiseFloor;

    UpdateData( FALSE );
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
