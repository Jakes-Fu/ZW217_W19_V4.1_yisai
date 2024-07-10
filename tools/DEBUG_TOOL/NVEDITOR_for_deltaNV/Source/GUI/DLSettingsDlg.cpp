// DLSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "DLSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLSettingsDlg dialog


CDLSettingsDlg::CDLSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLSettingsDlg)
	m_bWriteDsp = FALSE;
	m_strDspFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDLSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLSettingsDlg)
	DDX_Control(pDX, IDC_EDT_NOTICE, m_edtNotice);
	DDX_Control(pDX, IDC_DSPBROWSE, m_ctlDspBrowse);
	DDX_Control(pDX, IDC_EDIT_DSPFILE, m_ctlDsp);
	DDX_Check(pDX, IDC_CHECK_WRITEDSP, m_bWriteDsp);
	DDX_Text(pDX, IDC_EDIT_DSPFILE, m_strDspFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CDLSettingsDlg)
	ON_BN_CLICKED(IDC_DSPBROWSE, OnDspbrowse)
	ON_BN_CLICKED(IDC_CHECK_WRITEDSP, OnCheckWritedsp)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLSettingsDlg message handlers

BOOL CDLSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UpdateData(FALSE);

    // @ZHANG Ming : 2003-05-06
    // [[
    m_ctlDsp.EnableWindow( m_bWriteDsp );
    m_ctlDspBrowse.EnableWindow( m_bWriteDsp );
    // ]]
	m_edtNotice.SetWindowText(_T("**Notice**\r\nIt will not backup Calibrations and any\r\nother NV items!\r\nPlease use download tool to backup them."));

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLSettingsDlg::OnOK() 
{
  	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

    CString strText;
    
    if ( m_bWriteDsp )
    {
        m_ctlDsp.GetWindowText( strText ); 
        if(strText.GetLength() == 0)
        {
            ::AfxMessageBox( _T("Must specify Dsp file!") );
            return;
        }
    }


    EndDialog(IDOK);
//	CDialog::OnOK();
}


// @ZHANG Ming : 2003-05-06
// [[
void CDLSettingsDlg::OnDspbrowse() 
{
	// TODO: Add your control notification handler code here
    static TCHAR BASED_CODE szFilter[] = _T("Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||");

    UpdateData();
	CFileDialog dlg(TRUE, NULL, m_strDspFileName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strDspFileName = dlg.GetPathName();
        UpdateData(FALSE);

        return;
    }
	
}
// ]]

void CDLSettingsDlg::OnCheckWritedsp() 
{
	// TODO: Add your control notification handler code here
    UpdateData();
    m_ctlDsp.EnableWindow( m_bWriteDsp );
    m_ctlDspBrowse.EnableWindow( m_bWriteDsp );
	
}



HBRUSH CDLSettingsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID() == IDC_EDT_NOTICE)
	{
		pDC->SetTextColor(RGB(255,0,0));
 	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
