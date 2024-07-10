// DlgMTVA.cpp : implementation file
//

#include "stdafx.h"
#include "ResOver.h"
#include "DlgMtva.h"


// CDlgMTVA dialog

IMPLEMENT_DYNAMIC(CDlgMtva, CDialog)

CDlgMtva::CDlgMtva(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMtva::IDD, pParent)
{
	m_nMcc = 0;
	m_nMnc = 0;
	m_nCurIndex = -1;
	m_strTmpPath = _T("");
	m_strMTVA = _T("");
	m_bViewOnly = 0;
	m_bActive = 0;
}

CDlgMtva::~CDlgMtva()
{
}

void CDlgMtva::OnOK()
{
		// TODO: Add extra validation here
	if(m_nCurIndex == -1)
	{
		CDialog::OnCancel();
		return;
	}
	UpdateData();

	SaveCurMtva();

	LPBYTE pBuf = NULL;
	DWORD  dwSize = 0;
	m_Parser.EncodeMTVA(pBuf,dwSize);

	m_strMTVA.Empty();

	CString strFile;
	strFile.Format(_T("%s\\_mtva.dat"),m_strTmpPath);
	FILE *pFile = _tfopen(strFile.operator LPCTSTR(),_T("wb"));
	if(pFile == NULL)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}
	
	DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
	fclose(pFile);
	pFile = NULL;

	if(dwWirte != dwSize)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}

	m_strMTVA = strFile;

	CDialog::OnOK();
}
BOOL CDlgMtva::SetBuf(LPBYTE pBuf,DWORD dwSize, LPCTSTR lpszTmpPath)
{
	BOOL bOK = m_Parser.DecodeMTVA(pBuf,dwSize);
	m_strTmpPath = lpszTmpPath;

	if(m_Parser.m_tHead.nCount > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;
	}
	
	if(m_bViewOnly)
	{
		ShowCurMtva();
		UpdateCtrlState();
		GetDlgItem(IDC_MVS_BTN_PREV)->EnableWindow(FALSE);
		GetDlgItem(IDC_MVS_BTN_NEXT)->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	return bOK;
}
void CDlgMtva::OnPrev()
{
	if(m_nCurIndex == -1)
	{
		return;
	}
	UpdateData();

	SaveCurMtva();
	m_nCurIndex --;
	if(m_nCurIndex<=0)
	{
		m_nCurIndex = 0;		
	}	
	ShowCurMtva();

	if(m_nCurIndex <= 0)
	{
		GetDlgItem(IDC_MVS_BTN_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_MVS_BTN_PREV)->EnableWindow(TRUE);
	}
}
void CDlgMtva::OnNext() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurIndex == -1)
	{
		//if(!m_bViewOnly)
		{
			if(AfxMessageBox(_T("Do you want to insert one pageat the tail?"),MB_YESNO) == IDYES)
			{
				MTVA_T bm;
				bm.Init();
				m_Parser.m_vMTVA.push_back(bm);	
				m_Parser.m_tHead.nCount = m_Parser.m_vMTVA.size();
				
				//m_bViewOnly = TRUE;
				m_nCurIndex = 0;
				ShowCurMtva();	

			}
			return;
		}
	}

	UpdateData();

	SaveCurMtva();
	m_nCurIndex++;

	int nUpbound = m_Parser.m_vMTVA.size();

	if(m_nCurIndex >= nUpbound)
	{
		if(m_Parser.m_vMTVA.size() < MTVA_MAX_NUM)
		{
			if(AfxMessageBox(_T("Do you want to insert one page at the tail?"),MB_YESNO) == IDYES)
			{
				MTVA_T bm;
				bm.Init();
				m_Parser.m_vMTVA.push_back(bm);
				m_Parser.m_tHead.nCount = m_Parser.m_vMTVA.size();
				nUpbound ++;

			}
			else
			{
				m_nCurIndex--;
				return;
			}
		}
	}	
/*
	if(m_nCurIndex > nUpbound)
	{
		m_nCurIndex = nUpbound-1;	
		if(m_nCurIndex == -1)
		{
			m_nCurIndex = 0;
		}
	}*/
	ShowCurMtva();


	if(m_nCurIndex > 0)
	{
		GetDlgItem(IDC_MVS_BTN_PREV)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_MVS_BTN_PREV)->EnableWindow(FALSE);
	}

	
}
void CDlgMtva::SaveCurMtva()
{
	UpdateData();

	if(m_nCurIndex == -1)
		return;

	if((m_nCurIndex + 1) > (int)m_Parser.m_vMTVA.size())
	{
		MTVA_T bm;
		bm.Init();
		m_Parser.m_vMTVA.push_back(bm);
		m_Parser.m_tHead.nCount = m_Parser.m_vMTVA.size();
	}

	m_Parser.m_vMTVA[m_nCurIndex].nMcc = m_nMcc;
	m_Parser.m_vMTVA[m_nCurIndex].nMnc = m_nMnc;
	size_t covered = 0;

	for(int i = 0; i < VMailCount; i++)
	{
		//m_Parser.m_vMTVA.VMail[i] = m_m_strVMail[i];
		//strcpy(m_Parser.m_vMTVA[m_nCurIndex].VMail[i], (LPCSTR)m_strVMail[i].GetBuffer());
		if(wcstombs_s(&covered,m_Parser.m_vMTVA[m_nCurIndex].VMail[i], VMailSize,\
			m_strVMail[i].GetBuffer(),VMailSize) < 0)
		{
			AfxMessageBox(_T("Voice Mail included illlegal characters !"));
			break;
		}
	}
	for(int i = 0; i < ECallCount; i++)
	{
		//m_Parser.m_vMTVA.ECall[i] = m_m_strECall[i];
		//strcpy(m_Parser.m_vMTVA[m_nCurIndex].ECall[i], (LPCSTR)m_strECall[i].GetBuffer());
		if(wcstombs_s(&covered,m_Parser.m_vMTVA[m_nCurIndex].ECall[i], ECallSize,\
			m_strECall[i].GetBuffer(),ECallSize) < 0)
		{
			AfxMessageBox(_T("Emergence Call included illlegal characters !"));
			break;
		}
		
	}

	//strcpy(m_Parser.m_vMTVA[m_nCurIndex].Url, (LPCSTR)m_strURL.GetBuffer());
	if(wcstombs_s(&covered,m_Parser.m_vMTVA[m_nCurIndex].Url, URLSize,\
			m_strURL.GetBuffer(),URLSize) < 0)
	{
		AfxMessageBox(_T("URL included illlegal characters !"));
	}
	
}
void CDlgMtva::ShowCurMtva()
{
	if(m_nCurIndex == -1)
	{
		m_strPos.Format(_T("%d/%d"),m_Parser.m_tHead.nCount==0 ? 0 : (m_nCurIndex+1), m_Parser.m_tHead.nCount);
		UpdateData(FALSE);
		UpdateCtrlState();
		return;
	}

	UpdateData();
	for(int i = 0; i < ECallCount; i++)
	{
		m_strECall[i] = m_Parser.m_vMTVA[m_nCurIndex].ECall[i];
	}
	for(int i = 0; i < VMailCount; i++)
	{
		m_strVMail[i] = m_Parser.m_vMTVA[m_nCurIndex].VMail[i];
	}
	m_strURL = m_Parser.m_vMTVA[m_nCurIndex].Url;
	m_nMcc = m_Parser.m_vMTVA[m_nCurIndex].nMcc;
	m_nMnc = m_Parser.m_vMTVA[m_nCurIndex].nMnc;
	m_strPos.Format(_T("%d/%d"),m_nCurIndex+1, m_Parser.m_vMTVA.size());
	m_bActive = TRUE;

	UpdateData(FALSE);	

	UpdateCtrlState();

}
void CDlgMtva::OnExport()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	SaveCurMtva();

	if(m_Parser.m_vMTVA.size() == 0)
	{
		AfxMessageBox(_T("No item to save!"));
		return;
	}

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All file(*.*)|*.*||"));
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	LPBYTE pBuf = NULL;
	DWORD  dwSize = 0;
	m_Parser.EncodeMTVA(pBuf,dwSize);

	FILE *pFile = _tfopen(dlg.GetPathName(),_T("wb"));
	if(pFile == NULL)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}
	
	DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
	fclose(pFile);
	pFile = NULL;

	if(dwWirte != dwSize)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}

	AfxMessageBox(_T("Export complete!"));
}
void CDlgMtva::OnImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,_T("All file(*.*)|*.*||"));
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	FILE *pFile = _tfopen(dlg.GetPathName(),_T("rb"));
	if(pFile == NULL)
	{
		CString strErr;
		strErr.Format(_T("Open file \"%s\" failed!"),dlg.GetPathName());
		AfxMessageBox(strErr);
		return;
	}
	fseek(pFile,0,SEEK_END);
	DWORD dwSize = ftell(pFile);
	fseek(pFile,0,SEEK_SET);
	if(dwSize == 0)
	{
		CString strErr;
		strErr.Format(_T("File size is zero!"));
		AfxMessageBox(strErr);
		return;
	}
	LPBYTE pBuf = new BYTE[dwSize];
	DWORD dwRead = fread(pBuf,1,dwSize,pFile);

	fclose(pFile);

	if(dwRead != dwSize)
	{
		CString strErr;
		strErr.Format(_T("Read file failed!"));
		AfxMessageBox(strErr);
		return;
	}

	BOOL bOK = m_Parser.DecodeMTVA(pBuf,dwSize);

	if(!bOK)
	{
		CString strErr;
		strErr.Format(_T("Import failed!\nPlease check the file format!"));
		AfxMessageBox(strErr);
	}

	delete [] pBuf;
	if(bOK && m_Parser.m_vMTVA.size() > 0)
	{
		m_nCurIndex = 0;
		//m_bViewOnly = TRUE;
	}
	else
	{
		m_nCurIndex = -1;
	}
	ShowCurMtva();
	UpdateCtrlState();
	GetDlgItem(IDC_MVS_BTN_PREV)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

BOOL CDlgMtva::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Parser.m_tHead.nCount > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;		
	}
	ShowCurMtva();
	UpdateCtrlState();
	UpdateData(FALSE);

	if(m_bViewOnly)
	{

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control

}
void CDlgMtva::UpdateCtrlState()
{
	GetDlgItem(IDC_MVS_EDT_MCC)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_MVS_EDT_MNC)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_1)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_2)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_3)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_4)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_5)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_6)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_7)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_8)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_9)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_ECALL_10)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_1)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_2)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_3)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_4)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_5)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_6)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_7)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_8)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_9)->EnableWindow(m_bActive && !m_bViewOnly);
	GetDlgItem(IDC_EDT_VOICEMAIL_10)->EnableWindow(m_bActive && !m_bViewOnly);

	GetDlgItem(IDC_MVS_EDT_URL)->EnableWindow(m_bActive && !m_bViewOnly);

	
	if(m_bViewOnly)
	{
		GetDlgItem(IDC_MVS_BTN_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MVS_BTN_EXPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_MVS_STC_USED)->ShowWindow(SW_HIDE);

		//GetDlgItem(IDC_BTN_IST_NTAC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BTN_GO)->ShowWindow(SW_HIDE);
	}
}
void CDlgMtva::DoDataExchange(CDataExchange* pDX)
{
		DDX_Text(pDX, IDC_EDT_ECALL_1, m_strECall[0]);
	DDV_MaxChars(pDX, m_strECall[0], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_2, m_strECall[1]);
	DDV_MaxChars(pDX, m_strECall[1], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_3, m_strECall[2]);
	DDV_MaxChars(pDX, m_strECall[2], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_4, m_strECall[3]);
	DDV_MaxChars(pDX, m_strECall[3], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_5, m_strECall[4]);
	DDV_MaxChars(pDX, m_strECall[4], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_6, m_strECall[5]);
	DDV_MaxChars(pDX, m_strECall[5], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_7, m_strECall[6]);
	DDV_MaxChars(pDX, m_strECall[6], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_8, m_strECall[7]);
	DDV_MaxChars(pDX, m_strECall[7], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_9, m_strECall[8]);
	DDV_MaxChars(pDX, m_strECall[8], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_ECALL_10, m_strECall[9]);
	DDV_MaxChars(pDX, m_strECall[9], ECallMaxLen);

		DDX_Text(pDX, IDC_EDT_VOICEMAIL_1, m_strVMail[0]);
	DDV_MaxChars(pDX, m_strVMail[0], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_2, m_strVMail[1]);
	DDV_MaxChars(pDX, m_strVMail[1], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_3, m_strVMail[2]);
	DDV_MaxChars(pDX, m_strVMail[2], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_4, m_strVMail[3]);
	DDV_MaxChars(pDX, m_strVMail[3], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_5, m_strVMail[4]);
	DDV_MaxChars(pDX, m_strVMail[4], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_6, m_strVMail[5]);
	DDV_MaxChars(pDX, m_strVMail[5], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_7, m_strVMail[6]);
	DDV_MaxChars(pDX, m_strVMail[6], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_8, m_strVMail[7]);
	DDV_MaxChars(pDX, m_strVMail[7], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_9, m_strVMail[8]);
	DDV_MaxChars(pDX, m_strVMail[8], ECallMaxLen);
		DDX_Text(pDX, IDC_EDT_VOICEMAIL_10, m_strVMail[9]);
	DDV_MaxChars(pDX, m_strVMail[9], ECallMaxLen);

		DDX_Text(pDX, IDC_MVS_EDT_URL, m_strURL);
	DDV_MaxChars(pDX, m_strURL, URLMaxLen);

		DDX_Text(pDX, IDC_MVS_EDT_MCC, m_nMcc);
	DDV_MinMaxUInt(pDX, m_nMcc, 0, 65535);
		DDX_Text(pDX, IDC_MVS_EDT_MNC, m_nMnc);
	DDV_MinMaxUInt(pDX, m_nMnc, 0, 65535);

		DDX_Text(pDX, IDC_MVS_STC_POS, m_strPos);

	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMtva, CDialog)
	ON_BN_CLICKED(IDC_MVS_BTN_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_MVS_BTN_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_MVS_BTN_NEXT, OnNext)
	ON_BN_CLICKED(IDC_MVS_BTN_PREV, OnPrev)

END_MESSAGE_MAP()



