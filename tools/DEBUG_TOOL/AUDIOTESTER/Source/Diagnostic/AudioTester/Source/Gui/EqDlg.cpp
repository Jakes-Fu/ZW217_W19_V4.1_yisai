// EqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "EqDlg.h"
#include "BMPCtrlWithSpecRect.h"

#include "ConvertFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEqDlg dialog


CEqDlg::CEqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEqDlg)
	m_dDbBass = 0.0;
	m_dDbTreble = 0.0;
	m_dDfBass = 0.0;
	m_dDfTreble = 0.0;
	m_dF0Bass = 0.0;
	m_dF0Treble = 0.0;
	m_dGainBass = 0.0;
	m_dGainTreble = 0.0;
	m_strBassAlpha = _T("");
	m_strBassBeta = _T("");
	m_strBassGain = _T("");
	m_strBassGama = _T("");
	m_strTrebleAlpha = _T("");
	m_strTrebleBeta = _T("");
	m_strTrebleGain = _T("");
	m_strTrebleGama = _T("");
	m_dDbMid = 0.0;
	m_dDfMid = 0.0;
	m_dF0Mid = 0.0;
	m_dGainMid = 0.0;
	m_strMidAlpha = _T("");
	m_strMidBeta = _T("");
	m_strMidGain = _T("");
	m_strMidGama = _T("");
	m_wBassBeta = 0;
	m_wBassGain = 0;
	m_wBassGama = 0;
	m_wTrebleAlpha = 0;
	m_wTrebleBeta = 0;
	m_wTrebleGain = 0;
	m_wTrebleGama = 0;
	m_wBassAlpha = 0;
	//}}AFX_DATA_INIT

    m_pTempArea = new SpecialRectOfEQ;
    m_bEnableMid = TRUE;
    m_pArea = NULL;

    m_wMidAlpha = 0;
    m_wMidBeta = 0;
    m_wMidGain = 0;
    m_wMidGama = 0;
}

CEqDlg::~CEqDlg()
{
    if( m_pTempArea )
    {
        delete m_pTempArea;
        m_pTempArea = NULL;
    }
}

void CEqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEqDlg)
	DDX_Control(pDX, IDC_EDIT_MID_GAMA, m_edtMidGama);
	DDX_Control(pDX, IDC_EDIT_MID_GAIN, m_edtMidGain);
	DDX_Control(pDX, IDC_EDIT_MID_BETA, m_edtMidBeta);
	DDX_Control(pDX, IDC_EDIT_MID_ALPHA, m_edtMidAlpha);
	DDX_Control(pDX, IDC_EDIT_TRENBLE_GAMA, m_edtTrebleGama);
	DDX_Control(pDX, IDC_EDIT_TRENBLE_GAIN, m_edtTrebleGain);
	DDX_Control(pDX, IDC_EDIT_TRENBLE_BETA, m_edtTrebleBeta);
	DDX_Control(pDX, IDC_EDIT_TRENBLE_ALPHA, m_edtTrebleAlpha);
	DDX_Control(pDX, IDC_EDIT_BASS_GAMA, m_edtBassGama);
	DDX_Control(pDX, IDC_EDIT_BASS_GAIN, m_edtBassGain);
	DDX_Control(pDX, IDC_EDIT_BASS_BETA, m_edtBassBeta);
	DDX_Control(pDX, IDC_EDIT_BASS_ALPHA, m_edtBassAlpha);
	DDX_Control(pDX, IDC_STATIC_EQ_GRAPH, m_eqGraph);
	DDX_Text(pDX, IDC_EDIT_DB_BASS, m_dDbBass);
	DDX_Text(pDX, IDC_EDIT_DB_TRENBLE, m_dDbTreble);
	DDX_Text(pDX, IDC_EDIT_DF_BASS, m_dDfBass);
	DDX_Text(pDX, IDC_EDIT_DF_TRENBLE, m_dDfTreble);
	DDX_Text(pDX, IDC_EDIT_F0_BASS, m_dF0Bass);
	DDX_Text(pDX, IDC_EDIT_F0_TRENBLE, m_dF0Treble);
	DDX_Text(pDX, IDC_EDIT_GAIN_BASS, m_dGainBass);
	DDX_Text(pDX, IDC_EDIT_GAIN_TRENBLE, m_dGainTreble);
	DDX_Text(pDX, IDC_EDIT_BASS_ALPHA, m_strBassAlpha);
	DDV_MaxChars(pDX, m_strBassAlpha, 6);
	DDX_Text(pDX, IDC_EDIT_BASS_BETA, m_strBassBeta);
	DDV_MaxChars(pDX, m_strBassBeta, 6);
	DDX_Text(pDX, IDC_EDIT_BASS_GAIN, m_strBassGain);
	DDV_MaxChars(pDX, m_strBassGain, 6);
	DDX_Text(pDX, IDC_EDIT_BASS_GAMA, m_strBassGama);
	DDV_MaxChars(pDX, m_strBassGama, 6);
	DDX_Text(pDX, IDC_EDIT_TRENBLE_ALPHA, m_strTrebleAlpha);
	DDV_MaxChars(pDX, m_strTrebleAlpha, 6);
	DDX_Text(pDX, IDC_EDIT_TRENBLE_BETA, m_strTrebleBeta);
	DDV_MaxChars(pDX, m_strTrebleBeta, 6);
	DDX_Text(pDX, IDC_EDIT_TRENBLE_GAIN, m_strTrebleGain);
	DDV_MaxChars(pDX, m_strTrebleGain, 6);
	DDX_Text(pDX, IDC_EDIT_TRENBLE_GAMA, m_strTrebleGama);
	DDV_MaxChars(pDX, m_strTrebleGama, 6);
	DDX_Text(pDX, IDC_EDIT_DB_MID, m_dDbMid);
	DDX_Text(pDX, IDC_EDIT_DF_MID, m_dDfMid);
	DDX_Text(pDX, IDC_EDIT_F0_MID, m_dF0Mid);
	DDX_Text(pDX, IDC_EDIT_GAIN_MID, m_dGainMid);
	DDX_Text(pDX, IDC_EDIT_MID_ALPHA, m_strMidAlpha);
	DDX_Text(pDX, IDC_EDIT_MID_BETA, m_strMidBeta);
	DDX_Text(pDX, IDC_EDIT_MID_GAIN, m_strMidGain);
	DDX_Text(pDX, IDC_EDIT_MID_GAMA, m_strMidGama);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEqDlg, CDialog)
	//{{AFX_MSG_MAP(CEqDlg)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqDlg message handlers

BOOL CEqDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_pArea->m_dMaxXValue = *(m_pArea->m_pSampleRate) / 2;
    
    *m_pTempArea = *m_pArea;
    m_eqGraph.m_pDataSource = m_pTempArea;

    m_dF0Treble = m_pTempArea->m_dF0Treble;
    m_dDfTreble = m_pTempArea->m_dDfTreble;
    m_dDbTreble = m_pTempArea->m_dDbTreble;
    m_dGainTreble = m_pTempArea->m_dGainTreble;
    
    m_wTrebleAlpha = m_pTempArea->m_wTrebleAlpha;
    m_wTrebleBeta = m_pTempArea->m_wTrebleBeta;
    m_wTrebleGama = m_pTempArea->m_wTrebleGama;
    m_wTrebleGain = m_pTempArea->m_wTrebleGain;
    
    m_dF0Bass = m_pTempArea->m_dF0Bass;
    m_dDfBass = m_pTempArea->m_dDfBass;
    m_dDbBass = m_pTempArea->m_dDbBass;
    m_dGainBass = m_pTempArea->m_dGainBass;
    
    m_wBassAlpha = m_pTempArea->m_wBassAlpha;
    m_wBassBeta = m_pTempArea->m_wBassBeta;
    m_wBassGama = m_pTempArea->m_wBassGama;
    m_wBassGain = m_pTempArea->m_wBassGain;

    if( m_bEnableMid )
    {
        m_dF0Mid = m_pTempArea->m_dF0Mid;
        m_dDfMid = m_pTempArea->m_dDfMid;
        m_dDbMid = m_pTempArea->m_dDbMid;
        m_dGainMid = m_pTempArea->m_dGainMid;
        
        m_wMidAlpha = m_pTempArea->m_wMidAlpha;
        m_wMidBeta = m_pTempArea->m_wMidBeta;
        m_wMidGama = m_pTempArea->m_wMidGama;
        m_wMidGain = m_pTempArea->m_wMidGain;
    }
        
    SetWindowText( m_pArea->m_strName );
    ChangeFont();

    m_edtBassAlpha.SetDispMode( HEX );    
    m_edtBassBeta.SetDispMode( HEX );    
    m_edtBassGain.SetDispMode( HEX );    
    m_edtBassGama.SetDispMode( HEX );    
    m_edtTrebleAlpha.SetDispMode( HEX );    
    m_edtTrebleBeta.SetDispMode( HEX );    
    m_edtTrebleGain.SetDispMode( HEX );    
    m_edtTrebleGama.SetDispMode( HEX );    

    if( m_bEnableMid )
    {
        m_edtMidAlpha.SetDispMode( HEX );    
        m_edtMidBeta.SetDispMode( HEX );    
        m_edtMidGain.SetDispMode( HEX );    
        m_edtMidGama.SetDispMode( HEX );    
    }
    else
    {
        m_edtMidAlpha.EnableWindow( FALSE );    
        m_edtMidBeta.EnableWindow( FALSE );    
        m_edtMidGain.EnableWindow( FALSE );    
        m_edtMidGama.EnableWindow( FALSE ); 
        
        GetDlgItem( IDC_EDIT_F0_MID )->EnableWindow( FALSE );
        GetDlgItem( IDC_EDIT_DF_MID )->EnableWindow( FALSE );
        GetDlgItem( IDC_EDIT_DB_MID )->EnableWindow( FALSE );
        GetDlgItem( IDC_EDIT_GAIN_MID )->EnableWindow( FALSE );
    }

    //RefreshData( FALSE );
    RefreshPaint();
    
    UpdateHexData( FALSE );
    UpdateData( FALSE );

    m_eqGraph.EraseBK();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEqDlg::ChangeFont()
{
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = GetWindow(GW_CHILD);
    CFont* pFatherFont = GetParent()->GetFont();
    SetFont(pFatherFont);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(pFatherFont);
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}

void CEqDlg::OnButtonRefresh() 
{
//    CWnd* pWnd = GetFocus();
//    DWORD dwId = pWnd->GetDlgCtrlID();

    UpdateData();
/*
    if( 
        dwId == IDC_EDIT_TRENBLE_ALPHA     ||
        dwId == IDC_EDIT_TRENBLE_BETA     ||
        dwId == IDC_EDIT_TRENBLE_GAMA     ||
        dwId == IDC_EDIT_TRENBLE_GAIN   ||
        dwId == IDC_EDIT_BASS_ALPHA        ||
        dwId == IDC_EDIT_BASS_BETA        ||
        dwId == IDC_EDIT_BASS_GAMA        ||
        dwId == IDC_EDIT_BASS_GAIN
      )
    {
        UpdateHexData();
        RefreshData( FALSE );
    }
    else
    {
    */
    RefreshData( TRUE );
    //}

    UpdateHexData( FALSE );
    UpdateData( FALSE );

    RefreshPaint();
}

void CEqDlg::OnOK() 
{
    UpdateData();
    UpdateHexData();
    
    *m_pArea = *m_pTempArea;	
	CDialog::OnOK();
}

void CEqDlg::UpdateHexData(BOOL bSaveAndValidate)
{
    if( bSaveAndValidate )
    {
        m_wBassAlpha = (short)m_edtBassAlpha.GetDigit();
        m_wBassBeta = (short)m_edtBassBeta.GetDigit();
        m_wBassGama = (short)m_edtBassGama.GetDigit();
        m_wBassGain = (short)m_edtBassGain.GetDigit();
        m_wTrebleAlpha = (short)m_edtTrebleAlpha.GetDigit();
        m_wTrebleBeta = (short)m_edtTrebleBeta.GetDigit();
        m_wTrebleGama = (short)m_edtTrebleGama.GetDigit();
        m_wTrebleGain = (short)m_edtTrebleGain.GetDigit();

        if( m_bEnableMid )
        {
            m_wMidAlpha = (short)m_edtMidAlpha.GetDigit();
            m_wMidBeta = (short)m_edtMidBeta.GetDigit();
            m_wMidGama = (short)m_edtMidGama.GetDigit();
            m_wMidGain = (short)m_edtMidGain.GetDigit();
        }
    }
    else
    {
        m_strBassAlpha.Format( _T("0x%x"),m_wBassAlpha );
        if( m_wBassAlpha < 0 )
        {
            m_strBassAlpha.Delete( 2,4 );
        }

        m_strBassBeta.Format( _T("0x%x"),m_wBassBeta );
        if( m_wBassBeta < 0 )
        {
            m_strBassBeta.Delete( 2,4 );
        }

        m_strBassGama.Format( _T("0x%x"),m_wBassGama );
        if( m_wBassGama < 0 )
        {
            m_strBassGama.Delete( 2,4 );
        }

        m_strBassGain.Format( _T("0x%x"),m_wBassGain );
        if( m_wBassGain < 0 )
        {
            m_strBassGain.Delete( 2,4 );
        }

        m_strTrebleAlpha.Format( _T("0x%x"),m_wTrebleAlpha );
        if( m_wTrebleAlpha < 0 )
        {
            m_strTrebleAlpha.Delete( 2,4 );
        }
        
        m_strTrebleBeta.Format( _T("0x%x"),m_wTrebleBeta );
        if( m_wTrebleBeta < 0 )
        {
            m_strTrebleBeta.Delete( 2,4 );
        }
        
        m_strTrebleGama.Format( _T("0x%x"),m_wTrebleGama );
        if( m_wTrebleGama < 0 )
        {
            m_strTrebleGama.Delete( 2,4 );
        }
        
        m_strTrebleGain.Format( _T("0x%x"),m_wTrebleGain );
        if( m_wTrebleGain < 0 )
        {
            m_strTrebleGain.Delete( 2,4 );
        }

        if( m_bEnableMid )
        {
            m_strMidAlpha.Format( _T("0x%x"),m_wMidAlpha );
            if( m_wMidAlpha < 0 )
            {
                m_strMidAlpha.Delete( 2,4 );
            }
            
            m_strMidBeta.Format( _T("0x%x"),m_wMidBeta );
            if( m_wMidBeta < 0 )
            {
                m_strMidBeta.Delete( 2,4 );
            }
            
            m_strMidGama.Format( _T("0x%x"),m_wMidGama );
            if( m_wMidGama < 0 )
            {
                m_strMidGama.Delete( 2,4 );
            }
            
            m_strMidGain.Format( _T("0x%x"),m_wMidGain );
            if( m_wMidGain < 0 )
            {
                m_strMidGain.Delete( 2,4 );
            }
        }
    }
}

void CEqDlg::RefreshData( BOOL bResult )
{
    if( bResult )
    {
        df2alpha( m_dDfTreble,m_wTrebleAlpha,*(m_pArea->m_pSampleRate) );
        f02beta( m_dF0Treble,m_wTrebleBeta,*(m_pArea->m_pSampleRate) );
        db2gama( m_dDbTreble,m_wTrebleGama );
        gain2rgain( m_dGainTreble,m_wTrebleGain );
        CheckGamaAndGain( m_dDbTreble,m_dGainTreble,m_wTrebleGama,m_wTrebleGain );
        
        df2alpha( m_dDfBass,m_wBassAlpha,*(m_pArea->m_pSampleRate) );
        f02beta( m_dF0Bass,m_wBassBeta,*(m_pArea->m_pSampleRate) );
        db2gama( m_dDbBass,m_wBassGama );
        gain2rgain( m_dGainBass,m_wBassGain );
        CheckGamaAndGain( m_dDbBass,m_dGainBass,m_wBassGama,m_wBassGain );

        if( m_bEnableMid )
        {
            df2alpha( m_dDfMid,m_wMidAlpha,*(m_pArea->m_pSampleRate) );
            f02beta( m_dF0Mid,m_wMidBeta,*(m_pArea->m_pSampleRate) );
            db2gama( m_dDbMid,m_wMidGama );
            gain2rgain( m_dGainMid,m_wMidGain );
            CheckGamaAndGain( m_dDbMid,m_dGainMid,m_wMidGama,m_wMidGain );            
        }
    }
    else
    {
        alpha2df( m_dDfTreble,m_wTrebleAlpha,*(m_pArea->m_pSampleRate) );
        beta2f0( m_dF0Treble,m_wTrebleBeta,*(m_pArea->m_pSampleRate) );
        gama2db( m_dDbTreble,m_wTrebleGama );
        rgain2gain( m_dGainTreble,m_wTrebleGain );
        
        alpha2df( m_dDfBass,m_wBassAlpha,*(m_pArea->m_pSampleRate) );
        beta2f0( m_dF0Bass,m_wBassBeta,*(m_pArea->m_pSampleRate) );
        gama2db( m_dDbBass,m_wBassGama );
        rgain2gain( m_dGainBass,m_wBassGain );

        if( m_bEnableMid )
        {
            alpha2df( m_dDfMid,m_wMidAlpha,*(m_pArea->m_pSampleRate) );
            beta2f0( m_dF0Mid,m_wMidBeta,*(m_pArea->m_pSampleRate) );
            gama2db( m_dDbMid,m_wMidGama );
            rgain2gain( m_dGainMid,m_wMidGain );
        }
    }
}

void CEqDlg::RefreshPaint()
{
    m_pTempArea->m_dF0Treble = m_dF0Treble;
    m_pTempArea->m_dDfTreble = m_dDfTreble;
    m_pTempArea->m_dDbTreble = m_dDbTreble;
    m_pTempArea->m_dGainTreble = m_dGainTreble;
    
    m_pTempArea->m_wTrebleAlpha = m_wTrebleAlpha;
    m_pTempArea->m_wTrebleBeta = m_wTrebleBeta;
    m_pTempArea->m_wTrebleGama = m_wTrebleGama;
    m_pTempArea->m_wTrebleGain = m_wTrebleGain;
    
    m_pTempArea->m_dF0Bass = m_dF0Bass;
    m_pTempArea->m_dDfBass = m_dDfBass;
    m_pTempArea->m_dDbBass = m_dDbBass;
    m_pTempArea->m_dGainBass = m_dGainBass;
    
    m_pTempArea->m_wBassAlpha = m_wBassAlpha;
    m_pTempArea->m_wBassBeta = m_wBassBeta;
    m_pTempArea->m_wBassGama = m_wBassGama;
    m_pTempArea->m_wBassGain = m_wBassGain;

    if( m_bEnableMid )
    {
        m_pTempArea->m_dF0Mid = m_dF0Mid;
        m_pTempArea->m_dDfMid = m_dDfMid;
        m_pTempArea->m_dDbMid = m_dDbMid;
        m_pTempArea->m_dGainMid = m_dGainMid;
        
        m_pTempArea->m_wMidAlpha = m_wMidAlpha;
        m_pTempArea->m_wMidBeta = m_wMidBeta;
        m_pTempArea->m_wMidGama = m_wMidGama;
        m_pTempArea->m_wMidGain = m_wMidGain;
    }
    
    m_pTempArea->CalculateFreqGraphyData();
    
    m_eqGraph.EraseBK(); 
}

void CEqDlg::EnableMid( BOOL bEnable )
{
    m_bEnableMid = bEnable;
}

HBRUSH CEqDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if( pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT )
    {
        return CreateSolidBrush( RGB( 0,0,255 ) );
    }

    if( pWnd->GetDlgCtrlID() == IDC_STATIC_BASS )
    {
        return CreateSolidBrush( RGB( 255,0,0 ) );
    }
    
    if( pWnd->GetDlgCtrlID() == IDC_STATIC_TREBLE )
    {
        return CreateSolidBrush( RGB( 0,255,0 ) );
    }

    if( pWnd->GetDlgCtrlID() == IDC_STATIC_MID )
    {
        return CreateSolidBrush( RGB( 255,255,0 ) );
    }
    
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
