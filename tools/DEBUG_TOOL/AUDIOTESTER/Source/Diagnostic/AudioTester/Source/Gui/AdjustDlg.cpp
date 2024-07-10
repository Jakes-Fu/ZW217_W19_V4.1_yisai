// AdjustDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "AdjustDlg.h"
#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjustDlg dialog


CAdjustDlg::CAdjustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjustDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdjustDlg)
	m_strByte0 = _T("");
	m_strByte1 = _T("");
	m_strByte2 = _T("");
	m_strByte3 = _T("");
	m_strByte4 = _T("");
	m_strByte5 = _T("");
	m_strByte6 = _T("");
	m_strByte7 = _T("");
	m_strB0 = _T("");
	m_strB1 = _T("");
	m_strB2 = _T("");
	m_strB3 = _T("");
	m_strB4 = _T("");
	m_strB5 = _T("");
	m_strB6 = _T("");
	m_strB7 = _T("");
	//}}AFX_DATA_INIT

    m_uByte0 = 0;
    m_uByte1 = 0;
    m_uByte2 = 0;
    m_uByte3 = 0;
    m_uByte4 = 0;
    m_uByte5 = 0;
    m_uByte6 = 0;
    m_uByte7 = 0;
    
}


void CAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustDlg)
	DDX_Text(pDX, IDC_STATIC_BYTE0, m_strByte0);
	DDX_Text(pDX, IDC_STATIC_BYTE1, m_strByte1);
	DDX_Text(pDX, IDC_STATIC_BYTE2, m_strByte2);
	DDX_Text(pDX, IDC_STATIC_BYTE3, m_strByte3);
	DDX_Text(pDX, IDC_STATIC_BYTE4, m_strByte4);
	DDX_Text(pDX, IDC_STATIC_BYTE5, m_strByte5);
	DDX_Text(pDX, IDC_STATIC_BYTE6, m_strByte6);
	DDX_Text(pDX, IDC_STATIC_BYTE7, m_strByte7);
	DDX_Text(pDX, IDC_EDIT_BYTE0, m_strB0);
	DDV_MaxChars(pDX, m_strB0, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE1, m_strB1);
	DDV_MaxChars(pDX, m_strB1, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE2, m_strB2);
	DDV_MaxChars(pDX, m_strB2, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE3, m_strB3);
	DDV_MaxChars(pDX, m_strB3, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE4, m_strB4);
	DDV_MaxChars(pDX, m_strB4, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE5, m_strB5);
	DDV_MaxChars(pDX, m_strB5, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE6, m_strB6);
	DDV_MaxChars(pDX, m_strB6, 1);
	DDX_Text(pDX, IDC_EDIT_BYTE7, m_strB7);
	DDV_MaxChars(pDX, m_strB7, 1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjustDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjustDlg message handlers
void CAdjustDlg::Init( UINT uData,UINT uStartId )
{
    // Set windows text
    m_strTitle.LoadString( uStartId++ );
    m_strHiword.LoadString( uStartId++ );
    m_strLoword.LoadString( uStartId++ );
    m_strByte0.LoadString( uStartId++ );
    m_strByte1.LoadString( uStartId++ );
    m_strByte2.LoadString( uStartId++ );
    m_strByte3.LoadString( uStartId++ );
    m_strByte4.LoadString( uStartId++ );
    m_strByte5.LoadString( uStartId++ );
    m_strByte6.LoadString( uStartId++ );
    m_strByte7.LoadString( uStartId++ );

    m_uByte0 = uData & 0xF;
    m_uByte1 = ( uData >> 4 ) & 0xF;
    m_uByte2 = ( uData >> 8 ) & 0xF;
    m_uByte3 = ( uData >> 12 ) & 0xF;
    m_uByte4 = ( uData >> 16 ) & 0xF;
    m_uByte5 = ( uData >> 20 ) & 0xF;
    m_uByte6 = ( uData >> 24 ) & 0xF;
    m_uByte7 = ( uData >> 28 ) & 0xF;

    char buffer[32];
    itoa( m_uByte0,buffer,16 );
    m_strB0 = buffer;

    itoa( m_uByte1,buffer,16 );
    m_strB1 = buffer;

    itoa( m_uByte2,buffer,16 );
    m_strB2 = buffer;

    itoa( m_uByte3,buffer,16 );
    m_strB3 = buffer;

    itoa( m_uByte4,buffer,16 );
    m_strB4 = buffer;
    
    itoa( m_uByte5,buffer,16 );
    m_strB5 = buffer;
    
    itoa( m_uByte6,buffer,16 );
    m_strB6 = buffer;
    
    itoa( m_uByte7,buffer,16 );
    m_strB7 = buffer;
}

UINT CAdjustDlg::GetResult()
{
    UINT uResult;
    uResult = m_uByte0 | 
              ( m_uByte1 << 4 ) | 
              ( m_uByte2 << 8 ) | 
              ( m_uByte3 << 12 ) | 
              ( m_uByte4 << 16 ) | 
              ( m_uByte5 << 20 ) | 
              ( m_uByte6 << 24 ) | 
              ( m_uByte7 << 28 );

    return uResult;
}

BOOL CAdjustDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    SetWindowText( m_strTitle );
    
    GetDlgItem( IDC_STATIC_HIWORD )->SetWindowText( m_strHiword );
    GetDlgItem( IDC_STATIC_LOWORD )->SetWindowText( m_strLoword );
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	CString strAll("All");
	pSheet->m_CtrlStatus.RefreshControl( strAll,IDD_DIALOG_ADJUST,this );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdjustDlg::OnOK() 
{
	CDialog::OnOK();
#ifdef _UNICODE
	char szText[32] = {0};
	WideCharToMultiByte(CP_ACP,0,m_strB0,-1,szText,32,NULL,NULL);
	m_uByte0 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB1,-1,szText,32,NULL,NULL);
    m_uByte1 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB2,-1,szText,32,NULL,NULL);
    m_uByte2 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB3,-1,szText,32,NULL,NULL);
    m_uByte3 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB4,-1,szText,32,NULL,NULL);
    m_uByte4 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB5,-1,szText,32,NULL,NULL);
    m_uByte5 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB6,-1,szText,32,NULL,NULL);
    m_uByte6 = strtol( szText,NULL,16 );
	WideCharToMultiByte(CP_ACP,0,m_strB7,-1,szText,32,NULL,NULL);
    m_uByte7 = strtol( szText,NULL,16 );
#else
	m_uByte0 = strtol( (LPSTR)(LPCTSTR)m_strB0,NULL,16 );
    m_uByte1 = strtol( (LPSTR)(LPCTSTR)m_strB1,NULL,16 );
    m_uByte2 = strtol( (LPSTR)(LPCTSTR)m_strB2,NULL,16 );
    m_uByte3 = strtol( (LPSTR)(LPCTSTR)m_strB3,NULL,16 );
    m_uByte4 = strtol( (LPSTR)(LPCTSTR)m_strB4,NULL,16 );
    m_uByte5 = strtol( (LPSTR)(LPCTSTR)m_strB5,NULL,16 );
    m_uByte6 = strtol( (LPSTR)(LPCTSTR)m_strB6,NULL,16 );
    m_uByte7 = strtol( (LPSTR)(LPCTSTR)m_strB7,NULL,16 );
#endif
}
