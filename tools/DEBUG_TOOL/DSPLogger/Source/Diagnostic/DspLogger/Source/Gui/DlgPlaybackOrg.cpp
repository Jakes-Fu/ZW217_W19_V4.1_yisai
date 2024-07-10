// DlgPlaybackOrg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "DlgPlaybackOrg.h"
#include "MainFrm.h"
#include <process.h>
#include "ParseOrgExFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVENT_PROMPT 2334

UINT WINAPI PlaybackProc(
        LPVOID lpParameter
)
{
	CDlgPlaybackOrg *pWnd = (CDlgPlaybackOrg *)lpParameter;
	ParseOrgExFile	cOrgParse;
	BOOL bIsOrgExFile = FALSE;
	if(pWnd == NULL)
	{
		return 0;
	}
	bIsOrgExFile = cOrgParse.IsOrgExtendFile((pWnd->m_strPlaybackFile).operator LPCTSTR());
	pWnd->SendMessage(WM_SET_PROMPT_TIMER, NULL, NULL);
	if (bIsOrgExFile)
	{
		//pWnd->SetPlaybackData(NULL, 0, 1);  //begin timer
		cOrgParse.PlaybackOrgEx((pWnd->m_strPlaybackFile).operator LPCTSTR(),pWnd);
	//	pWnd->SetPlaybackData(NULL, 0, 2); //end timer
	}
	else
	{
		FILE *fp = NULL;
		fp = fopen(pWnd->m_strPlaybackFile, "rb");
		if(fp == NULL)
		{
			char szPrompt[128] = {0};
			sprintf(szPrompt, "Open %s failed!", pWnd->m_strPlaybackFile);
			AfxMessageBox(szPrompt);
			return 0;
		}
				
		pWnd->SetPlaybackData(NULL, 0, 1);  //begin timer
		BYTE buf[1024] = {0};
		
		while (!feof(fp))
		{
			int nSize = fread(buf, 1, 1024, fp);
			if(nSize != 0)
			{
				pWnd->SetPlaybackData(buf, nSize, 0);
			}
		}
		
		if(fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}
		pWnd->SetPlaybackData(NULL, 0, 2); //end timer
		
	}
	pWnd->PostMessage(WM_KILL_PROMPT_TIMER, NULL, NULL);
    return 0;

}

/////////////////////////////////////////////////////////////////////////////
// CDlgPlaybackOrg dialog


CDlgPlaybackOrg::CDlgPlaybackOrg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlaybackOrg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPlaybackOrg)
	m_strPrompt = "";
	m_strOrgPath = "";
	m_strTPPath = "";
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPlaybackFile = "";
	m_nCount = 1;
	m_bConverting = FALSE;
	m_hThread = NULL;
}


void CDlgPlaybackOrg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPlaybackOrg)
	DDX_Text(pDX, IDC_EDT_ORG_PATH, m_strOrgPath);
	DDX_Text(pDX, IDC_EDT_TP_DIRECTORY, m_strTPPath);
	DDX_Text(pDX, IDC_STATIC_PORMPT, m_strPrompt);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlaybackOrg, CDialog)
	//{{AFX_MSG_MAP(CDlgPlaybackOrg)
	ON_BN_CLICKED(IDC_BTN_BROWSE_ORG, OnBtnBrowseOrg)
	ON_BN_CLICKED(IDC_BTN_BROWSE_TP, OnBtnBrowseTp)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SET_PROMPT_TIMER, OnSetPromptTimer)
	ON_MESSAGE(WM_KILL_PROMPT_TIMER, OnKillPromptTimer)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlaybackOrg message handlers

BOOL CDlgPlaybackOrg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlaybackOrg::OnBtnBrowseOrg() 
{
	// TODO: Add your control notification handler code here
	char filter[] = "Log Files (*.org)|*.org|All Files (*.*)|*.*||";
	CFileDialog dlgFile(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,filter);
    if(IDOK == dlgFile.DoModal())
    {
		m_strOrgPath = dlgFile.GetPathName();
		int nIdnex = m_strOrgPath.ReverseFind('\\');
		if(nIdnex != -1)
		{
			m_strTPPath = m_strOrgPath.Left(nIdnex);
		}
	}
	UpdateData(FALSE);
}

void CDlgPlaybackOrg::OnBtnBrowseTp() 
{
	// TODO: Add your control notification handler code here
	CFileDlg dlg;
	m_strTPPath =  dlg.GetSelectDir();
	//m_strTPPath += "\\";
	UpdateData(FALSE);
}

void CDlgPlaybackOrg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	SetPrompt("Converting");
	CMainFrame *pMfr = (CMainFrame *)AfxGetMainWnd();
	::SendMessage(pMfr->GetSafeHwnd(), WM_COMMAND, ID_CLEAR, NULL);

	pMfr->m_pDspKernel->GetTpSet()->SetLogFilePath( (LPSTR)(LPCTSTR)m_strTPPath );
	if (pMfr->m_SettingsDlg.m_bSaveRecent)
	{
		pMfr->m_pDspKernel->GetTpSet()->SetMaxCount(pMfr->m_SettingsDlg.m_nTPNum * 1000000);
	}
	else
	{
		pMfr->m_pDspKernel->GetTpSet()->SetMaxCount(pMfr->m_SettingsDlg.m_nMaxNum * 1000000);		
	}

	pMfr->m_pDspKernel->SetUnFilterAddrFlag(pMfr->m_SettingsDlg.m_bNotFilterAddr);
	m_strPlaybackFile = m_strOrgPath;

	UINT uThreadId = 0;
	pMfr->m_pDspKernel->Clear();
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, PlaybackProc, (void*)this, 0, &uThreadId);
	
//	CDialog::OnOK();
}

void CDlgPlaybackOrg::SetPrompt(LPSTR lpStr)
{
	m_strPrompt = lpStr;
	UpdateData(FALSE);
}

void CDlgPlaybackOrg::SetPlaybackData (void* pData, DWORD dwSize, DWORD dwReserve )
{
	CMainFrame *pMfr = (CMainFrame *)AfxGetMainWnd();
	pMfr->m_pDspKernel->SetPlaybackData(pData, dwSize, dwReserve);
}
void CDlgPlaybackOrg::SetPlaybackDataEx(void* pData, DWORD dwSize, 
										DWORD dwReserve/*=0*/,DWORD dwPCTime/*=0*/ )
{
	CMainFrame *pMfr = (CMainFrame *)AfxGetMainWnd();
	pMfr->m_pDspKernel->SetPlaybackDataEx(pData, dwSize, dwReserve,dwPCTime);
}
LRESULT CDlgPlaybackOrg::OnSetPromptTimer(WPARAM wParam, LPARAM lParam)
{
	SetTimer(EVENT_PROMPT, 1000, NULL);
	m_bConverting = TRUE;
	return 1;
}

LRESULT CDlgPlaybackOrg::OnKillPromptTimer(WPARAM wParam, LPARAM lParam)
{
	SetPrompt("Convert Complete!");
	KillTimer(EVENT_PROMPT);
	m_bConverting = FALSE;
	CloseThread();
	PlayBackTP();
	CDialog::OnOK();
	return 1;
}

void CDlgPlaybackOrg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == EVENT_PROMPT)
	{
		if(m_nCount % 3 == 1)
		{
			SetPrompt("Converting ..");
		}
		else if(m_nCount % 3 == 2)
		{
			SetPrompt("Converting ....");
		}
		else
		{
			SetPrompt("Converting ......");
		}
		m_nCount ++;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgPlaybackOrg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_bConverting)
	{
		if(MessageBox("The data is converting now, are you sure interrupt it?","Close",MB_ICONWARNING|MB_OKCANCEL) == IDOK)
		{
			TerminateThread(m_hThread, (DWORD)-1);
			CloseThread();
			KillTimer(EVENT_PROMPT);
			CDialog::OnCancel();
		}
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CDlgPlaybackOrg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bConverting)
	{
		if(MessageBox("The data is converting now, are you sure interrupt it?","Close",MB_ICONWARNING|MB_OKCANCEL) == IDOK)
		{
			TerminateThread(m_hThread, (DWORD)-1);
			CloseThread();
			KillTimer(EVENT_PROMPT);
			CDialog::OnClose();
		}
	}
	else
	{
		CDialog::OnClose();
	}
}

void CDlgPlaybackOrg::CloseThread()
{
	CloseHandle(m_hThread);
	m_hThread = NULL;
}

void CDlgPlaybackOrg::PlayBackTP()
{
	CMainFrame *pFrm = (CMainFrame *) AfxGetMainWnd();
	if(pFrm != NULL)
	{
		char szFile[MAX_PATH] = {0}; 
		strcpy(szFile,pFrm->m_pDspKernel->GetTpSet()->GetTPFileName());
		pFrm->LoadLogFile(szFile);
	}
}