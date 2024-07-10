// VolumeCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "VolumeCtrlDlg.h"
#include "BMPCtrlWithSpecRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeCtrlDlg dialog


CVolumeCtrlDlg::CVolumeCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVolumeCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeCtrlDlg)
	m_bWriteImmediately = FALSE;
	//}}AFX_DATA_INIT
    m_pVolumeCtrlArea = NULL;
}


void CVolumeCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeCtrlDlg)
	DDX_Control(pDX, IDC_LIST_VALUE, m_listValue);
	DDX_Check(pDX, IDC_CHECK_WRITE_IMEEDIATE, m_bWriteImmediately);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CVolumeCtrlDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeCtrlDlg message handlers

BOOL CVolumeCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    ASSERT(m_pVolumeCtrlArea);
    
    m_pVolumeCtrlArea->ReCaculate();
    
    SetWindowText(m_pVolumeCtrlArea->m_strName );
    ChangeFont();
    InitListBox();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVolumeCtrlDlg::OnOK() 
{
    UpdateData();
    SpecialRect::m_bImmediateWrite = m_bWriteImmediately;
    
    int nIndex = m_listValue.GetCurSel();
    int nCount = m_listValue.GetCount();
    if ((nIndex != LB_ERR) && (nCount > 1))
    {
        int iSize=0;
        if( m_pVolumeCtrlArea->m_enumParaType == SpecialRect::volume_ctrl )
        {
            iSize = m_pVolumeCtrlArea->m_nLevel;
        }
        
        m_pVolumeCtrlArea->m_iValue =  iSize - nIndex -1;
        m_pVolumeCtrlArea->ReCaculate();
    }
    
	CDialog::OnOK();
}

void CVolumeCtrlDlg::InitListBox()
{
    std::vector<CString>vListValue;
    m_pVolumeCtrlArea->FormatValueString( vListValue );
    int iSize = vListValue.size();
    for( int i=0; i<iSize; ++i )
    {
        m_listValue.AddString( vListValue[i] );
    }
    ASSERT( m_pVolumeCtrlArea->m_iValue >=0 && m_pVolumeCtrlArea->m_iValue < iSize );
    m_listValue.SetCurSel ( iSize - m_pVolumeCtrlArea->m_iValue -1  );
}

void CVolumeCtrlDlg::ChangeFont()
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
