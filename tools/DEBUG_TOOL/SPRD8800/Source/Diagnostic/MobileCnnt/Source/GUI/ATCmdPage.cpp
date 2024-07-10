// ATCmdPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "ATCmdPage.h"
#include "MainFrm.h"
#include "MobileCnntDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CATCmdPage property page

IMPLEMENT_DYNCREATE(CATCmdPage, CPropertyPage)

CATCmdPage::CATCmdPage() : CPropertyPage(CATCmdPage::IDD)
{
	//{{AFX_DATA_INIT(CATCmdPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
	m_bInit = FALSE;
	m_bStopCheck = FALSE;
}

CATCmdPage::~CATCmdPage()
{
}

void CATCmdPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATCmdPage)
	DDX_Text(pDX, IDC_EDT_AT, m_strCmd);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResult);
	DDX_Text(pDX, IDC_EDIT_SCRIPT, m_strFile);
	DDX_Control(pDX, IDC_RADIO_SCRIPT, m_btnFile);
	DDX_Control(pDX, IDC_RADIO_LINE, m_btnLine);
	DDX_Control(pDX, IDC_BTN_CMD_BROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_BTN_STOP_CHECK, m_btnStopCheck);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CATCmdPage, CPropertyPage)
	//{{AFX_MSG_MAP(CATCmdPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_CMD_BROWSE, OnBtnCmdBrowse)
	ON_BN_CLICKED(IDC_RADIO_SCRIPT, OnRadioScript)
	ON_BN_CLICKED(IDC_RADIO_LINE, OnRadioLine)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_BTN_STOP_CHECK, OnBtnStopCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATCmdPage message handlers
void CATCmdPage::ClearAll()
{
	m_strCmd.Empty();
	m_lstResult.ResetContent();
	UpdateData(FALSE);
}

HBRUSH CATCmdPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

void CATCmdPage::OnBtnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_btnFile.GetCheck() == 1)
	{
		if(m_strFile.IsEmpty())
		{
			AfxMessageBox(_T("Please input file path first!"));
			return;
		}
		SendCmdEx();
	}
	else
	{
		SendCmd();
	}
	
}

void CATCmdPage::SendCmd()
{
	UpdateData();

	//teana hu 2010.06.30
	CString strTemp = m_strCmd;
	strTemp.MakeUpper();
	if(strTemp.Find(_T("AT##AUTOTEST")) != -1) //AT##AUTOTEST#1#1#2#abc123
	{
		m_strCmd.Delete(0, 13); //1#1#2#abc123
		int nIndex = m_strCmd.Find(_T("#"));
		if(nIndex != -1)
		{
			m_strCmd.Delete(0, nIndex + 1); // 1#2#abc123
			nIndex = m_strCmd.Find(_T("#"));
			if(nIndex != -1)
			{
				m_strCmd.Delete(0, nIndex + 1); // 2#abc123
				nIndex = m_strCmd.Find(_T("#"));
				if(nIndex != -1)
				{
					m_strCmd.Delete(0, nIndex + 1); // abc123
					
					CString strData = m_strCmd;
					int nLen = strData.GetLength();
					int nTLen = strTemp.GetLength();
					strTemp.Delete(nTLen - nLen, nLen);
					m_strCmd = strTemp;
					//			char szData[1024] = {0};
					//#ifdef _UNICODE
					//			WideCharToMultiByte( CP_ACP, 0, strData, -1, szData, 1024, NULL, NULL);
					//#else
					//			_tcscpy(szData, strData);
					//#endif	
					char szDes[1024] = {0};
					int nDes = 0;
					//BinaryToHexChar((unsigned char *)szData, strlen(szData), szDes, nDes);
					BOOL bBigEndian  = GetPrivateProfileInt( _T("AutoTest"), _T("BigEndian"), 1, theApp.m_szCfgPathName );					
					BinaryToHexChar((unsigned char *)strData.GetBuffer(strData.GetLength()), strData.GetLength()*2, szDes, nDes,bBigEndian);

					//m_strCmd = m_strCmd.Left(nIndex+1);
					m_strCmd += szDes;
				}
				else
				{
					m_strCmd = strTemp;
				}
			}
			else
			{
				m_strCmd = strTemp;
			}
		}
		else
		{
			m_strCmd = strTemp;
		}
	}
	//

	CWaitCursor cursor;

	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		TCHAR szPara[MAX_PATH] = {0};
		
		_tcscpy(szPara, m_strCmd);
		theApp.m_pEtionFactory->SendATCmd(AT_CMD, 0, (LPVOID)szPara, _tcslen(szPara));
	}
	
}

void CATCmdPage::SendCmdEx()
{
	UpdateData();
	char szFile[MAX_PATH] = {0};
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)m_strFile,-1,szFile,sizeof(szFile),NULL,NULL);
#else
	_tcscpy(szFile, m_strFile);
#endif
	
	FILE *fp = NULL;
	fp = fopen(szFile, "r");
	if(fp == NULL)
	{
		AfxMessageBox(_T("Open script file failed!"));
		return;
	}

	char szLine[512] = {0};
	while (!feof(fp))
	{
		fgets(szLine, 512, fp);
		
		//teana hu 2009.12.11
		if(szLine[strlen(szLine) - 1] == '\n')
		{
			szLine[strlen(szLine) - 1] = '\0';
		}
		if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
		{
			TCHAR szPara[MAX_PATH] = {0};
#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP,0,szLine,-1,szPara,sizeof(szPara)/sizeof(_TCHAR));
#else
			_tcscpy(szPara, szLine);
#endif
			AddTextToList(szPara);
			theApp.m_pEtionFactory->SendATCmd(AT_CMD, 0, (LPVOID)szPara, _tcslen(szPara));
		}
		Sleep(100);
	}

	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
}

BOOL CATCmdPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (WM_KEYDOWN == pMsg->message)
    {
        // Process when receive ENTER
        if (VK_RETURN == pMsg->wParam)
        {
            SendCmd();
            return TRUE;
        }
    }
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CATCmdPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	UpdateData();

	CString strResult = szRet;

	int nIndex = strResult.Find(_T("\r\n"));

	while(nIndex != -1)
	{
		CString strContent = strResult.Left(nIndex);

		AddTextToList(strContent);
	
		strResult.Delete(0, nIndex + 1);
		nIndex = strResult.Find(_T("\r\n"));
	}

	UpdateData(FALSE);
}

void CATCmdPage::AddTextToList(CString strText)
{
	CString strContent = strText;
	
	if(!strContent.IsEmpty())
	{
		int nCount = m_lstResult.GetCount();
		if(nCount >= MAX_LIST_COUNT)
		{
			m_lstResult.ResetContent();
		}

		m_lstResult.AddString(strContent);
		nCount = m_lstResult.GetCount();
		m_lstResult.SetTopIndex(nCount - 1);
		if (nCount > 0)
		{
			m_lstResult.SetCurSel(nCount-1);
		}
	}
}

BOOL CATCmdPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bInit = TRUE;
	m_btnLine.SetCheck(1);
	GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(FALSE);
	m_btnBrowse.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CATCmdPage::OnBtnCmdBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST,_T("TXT files (*.txt) |*.txt|All files(*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
	
}

void CATCmdPage::OnRadioScript() 
{
	// TODO: Add your control notification handler code here
	if(m_btnFile.GetCheck() == 1)
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SCRIPT);
		pEdit->EnableWindow(TRUE);
		m_btnBrowse.EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_AT)->EnableWindow(FALSE);
		pEdit->SetFocus();
		pEdit->SetSel(0,-1);
	}
	
}

void CATCmdPage::OnRadioLine() 
{
	// TODO: Add your control notification handler code here
	if(m_btnLine.GetCheck() == 1)
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDT_AT);
		GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(FALSE);
		m_btnBrowse.EnableWindow(FALSE);
		pEdit->EnableWindow(TRUE);
		pEdit->SetFocus();
		pEdit->SetSel(0,-1);
	}
	
}

void CATCmdPage::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_lstResult.ResetContent();
}

void CATCmdPage::OnBtnStopCheck() 
{
	// TODO: Add your control notification handler code here
	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument();

	if(!m_bStopCheck)
	{
		m_btnStopCheck.SetWindowText(_T("Startup Check"));
		if(pDoc != NULL)
		{
			pDoc->UpdateAllViews(NULL, WM_STOP_CHECK, NULL);
		}
	}
	else
	{
		m_btnStopCheck.SetWindowText(_T("Stop Check"));
		if(pDoc != NULL)
		{
			pDoc->UpdateAllViews(NULL, WM_STARTUP_CHECK, NULL);
		}
	}
	m_bStopCheck = !m_bStopCheck;
	
}

void CATCmdPage::BinaryToHexChar(LPBYTE lpSrc, int nSrcSize, LPSTR lpDes, int &nDeSize, BOOL bBigEndian /*= TRUE*/)
{
    char szValue[ 5 ] = { 0 };
    for(int i = 0; i < nSrcSize/2; i++)
    {
        sprintf(szValue, "%02x%02x", *lpSrc, *(lpSrc + 1));
		if(bBigEndian)
		{
			*lpDes = szValue[2];
			lpDes++;
			*lpDes = szValue[3];
			lpDes++;
			*lpDes = szValue[0];
			lpDes++;
			*lpDes = szValue[1];
			lpDes++;
		}
		else
		{		
			*lpDes = szValue[0];
			lpDes++;
			*lpDes = szValue[1];
			lpDes++;
			*lpDes = szValue[2];
			lpDes++;
			*lpDes = szValue[3];
			lpDes++;
		}

        lpSrc+=2;
    }
	*lpDes = '\0';
	nDeSize = nSrcSize * 2;
}