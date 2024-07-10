// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "ExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog


CExportDlg::CExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportDlg)
	m_strDesFile = _T("");
	m_strSrcFile = _T("");
	m_strFrom = _T("0");
	m_strTo = _T("");
	m_bAddDes = FALSE;
	//}}AFX_DATA_INIT

    m_bCurFile   = TRUE;
    m_bExportAll = TRUE;
	m_dwType = FILE_LOG_UNDES;
}


void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDlg)
	DDX_Control(pDX, IDC_CMB_EXPORT_TO, m_cmbTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_DES, m_btnBrowseDes);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_SRC, m_btnBrowseSrc);
	DDX_Control(pDX, IDC_EDIT_EXPORT_FROM, m_edtFrom);
	DDX_Control(pDX, IDC_EDIT_SRC, m_edtSrc);
	DDX_Control(pDX, IDC_EDIT_DES, m_edtDes);
	DDX_Text(pDX, IDC_EDIT_DES, m_strDesFile);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSrcFile);
	DDX_Text(pDX, IDC_EDIT_EXPORT_FROM, m_strFrom);
	DDX_CBString(pDX, IDC_CMB_EXPORT_TO, m_strTo);
	DDX_Control(pDX, IDC_RADIO_LOG, m_btnLog);
	DDX_Control(pDX, IDC_RADIO_ORG, m_btnOrg);
	DDX_Check(pDX, IDC_CHK_DESCRIPTION, m_bAddDes);
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_RADIO_CUR_FILE, m_btnCurFile);
    DDX_Control(pDX, IDC_RADIO_CHOOSE_FILE,m_btnChooseFile);
    DDX_Control(pDX, IDC_RADIO_RANGE,m_btnRange);
    DDX_Control(pDX, IDC_RADIO_ALL,m_btnAll);
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialog)
	//{{AFX_MSG_MAP(CExportDlg)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_CHOOSE_FILE, OnRadioChooseFile)
	ON_BN_CLICKED(IDC_RADIO_CUR_FILE, OnRadioCurFile)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_SRC, OnButtonBrowseSrc)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_DES, OnButtonBrowseDes)
	ON_BN_CLICKED(IDC_RADIO_LOG, OnRadioLogBtn)
	ON_BN_CLICKED(IDC_RADIO_ORG, OnRadioOrgBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

void CExportDlg::OnRadioAll() 
{
	m_bExportAll = TRUE;
    ShowControls();
}

void CExportDlg::OnRadioChooseFile() 
{
    m_bCurFile = FALSE;
    ShowControls();
}

void CExportDlg::OnRadioCurFile() 
{
	m_bCurFile = TRUE;
    ShowControls();
}

void CExportDlg::OnRadioRange() 
{
    m_bExportAll = FALSE;
    m_edtFrom.SetFocus();
    ShowControls();
}

void CExportDlg::OnButtonBrowseSrc() 
{
    char filter[] = "Log Files (*.tp)|*.tp|All Files (*.*)|*.*||";
    CFileDialog dlgFile(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,filter);
    if(IDOK == dlgFile.DoModal())
    {
        m_edtSrc.SetWindowText(dlgFile.GetPathName());
    }	
}

void CExportDlg::OnButtonBrowseDes() 
{
	char filter[100] = {0};
	char szExt[6] = {0};
	
	if(m_btnLog.GetCheck() == 1)
	{
		strcpy(filter, "Log Files (*.Log)|*.Log|All Files (*.*)|*.*||");
		strcpy(szExt, "*.Log");
	}
	else if(m_btnOrg.GetCheck() == 1)
	{
		strcpy(filter, "Org Files (*.org)|*.org|All Files (*.*)|*.*||");
		strcpy(szExt, "*.org");
	}

    CFileDialog dlgFile(FALSE,szExt,NULL,OFN_OVERWRITEPROMPT,filter);
    if(IDOK == dlgFile.DoModal())
    {
        m_edtDes.SetWindowText(dlgFile.GetPathName());
    }	
}

BOOL CExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_btnCurFile.SetCheck( m_bCurFile );
    m_btnChooseFile.SetCheck( !m_bCurFile );
    m_btnAll.SetCheck( m_bExportAll );
    m_btnRange.SetCheck( !m_bExportAll );

    m_cmbTo.SetCurSel(0);
    ShowControls();

	m_btnLog.SetCheck(1);
	m_dwType = FILE_LOG_UNDES;
		
	return TRUE;  
}

CString CExportDlg::GetSrcFile()
{
    if( m_bCurFile )
    {
        return CString("");
    }
    else
    {
        return m_strSrcFile;
    }
}

CString CExportDlg::GetDesFile()
{
    CString strPath= m_strDesFile;
    strPath.MakeUpper();

	switch(m_dwType) {
	case FILE_LOG_UNDES:
	case FILE_LOG_ADDDES:
		if (-1 == strPath.Find(".LOG"))
		{
			m_strDesFile+= ".Log";
		}
		break;
	case FILE_ORG:
		if (-1 == strPath.Find(".ORG"))
		{
			m_strDesFile+= ".org";
		}
		break;
	default:
		break;
	}

    return m_strDesFile;
}

LONGLONG CExportDlg::GetRangeFrom()
{
    if(m_bExportAll)
    {
        return 0;
    }
    else
    {
        return _atoi64(m_strFrom);
    }
}

LONGLONG CExportDlg::GetRangeTo()
{
    if(m_bExportAll)
    {
        return -1;
    }
    else
    {
        return _atoi64(m_strTo);
    }
}

void CExportDlg::ShowControls()
{
    m_edtSrc.EnableWindow( !m_bCurFile );
    m_btnBrowseSrc.EnableWindow( !m_bCurFile );

    m_edtFrom.EnableWindow( !m_bExportAll );
    m_cmbTo.EnableWindow( !m_bExportAll );
}

void CExportDlg::OnOK() 
{
    UpdateData();

    if (m_strDesFile == "")
    {
        AfxMessageBox("Destinate file path can't be empty!");
        GetDlgItem(IDC_EDIT_DES)->SetFocus();        
        return;
    }

    if (m_strFrom == "" && !m_bExportAll)
    {
        AfxMessageBox("Please input export range !");
        GetDlgItem(IDC_EDIT_EXPORT_FROM)->SetFocus();
        return;
    } 

    if (m_strTo == "" && !m_bExportAll)
    {
        AfxMessageBox("Please input export range !");
        GetDlgItem(IDC_CMB_EXPORT_TO)->SetFocus();
        return;
    } 
    
    if (m_strTo == "End" && !m_bExportAll)
    {
        m_strTo= "-1";
    }
	
	if(m_btnLog.GetCheck() == 1)
	{
		m_dwType = m_bAddDes ? FILE_LOG_ADDDES : FILE_LOG_UNDES;
	}
	else if(m_btnOrg.GetCheck() == 1)
	{
		m_dwType = FILE_ORG;
	}

	CDialog::OnOK();
}

void CExportDlg::OnRadioLogBtn() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CHK_DESCRIPTION)->EnableWindow(TRUE);
	
}

void CExportDlg::OnRadioOrgBtn() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CHK_DESCRIPTION)->EnableWindow(FALSE);
}
