// FlashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "FlashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlashDlg dialog


CFlashDlg::CFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashDlg)
//	m_nBaudRate = -1;
//	m_nPort = -1;
	m_strAddress = _T("0x90000003");
	m_strSize = _T("0x4000");
//	m_strFileName = _T("");
	//}}AFX_DATA_INIT
}


void CFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashDlg)
//	DDX_CBIndex(pDX, IDC_COMBO_FLASH_BAUDRATE, m_nBaudRate);
//	DDX_CBIndex(pDX, IDC_COMBO_FLASH_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_FLASH_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_EDIT_FLASH_SIZE, m_strSize);
//	DDX_Text(pDX, IDC_EDIT_FLASH_FILE, m_strFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlashDlg, CDialog)
	//{{AFX_MSG_MAP(CFlashDlg)
//	ON_BN_CLICKED(IDC_BUTTON_FLASH_BROWSE, OnButtonFlashBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashDlg message handlers

BOOL CFlashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UpdateData(FALSE);
    
    GetDlgItem(IDC_EDIT_FLASH_ADDRESS)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlashDlg::OnOK() 
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }
    
    CString strText;
    
    GetDlgItem(IDC_EDIT_FLASH_ADDRESS)->GetWindowText(strText);
    if(strText.GetLength() == 0)
        return;
    
    GetDlgItem(IDC_EDIT_FLASH_SIZE)->GetWindowText(strText);
    if(strText.GetLength() == 0)
        return;
/*    
    if(m_nBaudRate == -1 || m_nPort == -1)
    {
        return;
    }

    ((CComboBox*)GetDlgItem(IDC_COMBO_FLASH_BAUDRATE))->GetLBText(m_nBaudRate,strText);
    sscanf(strText,"%d",&m_uBaudRate);
    
    ((CComboBox*)GetDlgItem(IDC_COMBO_FLASH_PORT))->GetLBText(m_nPort,strText);
    sscanf(strText,"COM%d",&m_uPort);
*/
    
    EndDialog(IDOK);
}

/*
void CFlashDlg::OnButtonFlashBrowse() 
{
	static char BASED_CODE szFilter[] = "Binary Files (*.bin)|*.bin||";

    UpdateData();
	CFileDialog dlg(TRUE, NULL, m_strFileName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strFileName = dlg.GetPathName();
        UpdateData(FALSE);

        return;
    }	
}
*/