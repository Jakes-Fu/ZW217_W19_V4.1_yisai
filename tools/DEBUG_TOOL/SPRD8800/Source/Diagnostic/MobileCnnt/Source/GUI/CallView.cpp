// CallView.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "CallView.h"
#include "MainFrm.h"

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_BUF_LEN 512
/////////////////////////////////////////////////////////////////////////////
// CCallView

IMPLEMENT_DYNCREATE(CCallView, CFormView)

CCallView::CCallView()
	: CCoolFormView(CCallView::IDD)
{
	//{{AFX_DATA_INIT(CCallView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_BeginTime = 0;
	m_bTimer = FALSE;
	m_bComCalling = FALSE;
	m_nFlagRecord = -1;
	m_nFlagState = -1;
	m_pReportFile = NULL;
	memset(&m_PhoneBook, 0, sizeof(TELEPHONT_BOOK));
	m_bRejectByOther = FALSE;
}//lint !e1401

CCallView::~CCallView()
{
	WriteCallReport();
	CloseReportFile();
}

void CCallView::DoDataExchange(CDataExchange* pDX)
{
	CCoolFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallView)
	DDX_Control(pDX, IDC_FORMCALL_CMB_SIM, m_cmbSim);
	DDX_Control(pDX, IDC_LST_CALL_REPORT, m_lstReport);
	DDX_Control(pDX, IDC_EDT_CALL_NUM, m_edtPhoneNum);
	DDX_Text(pDX, IDC_EDT_CALL_NUM, m_strNum);
	DDX_Control(pDX, IDC_CHECK_ALL, m_btnAll);
	DDX_Control(pDX, IDC_CHECK_DIALED, m_btnCalled);
	DDX_Control(pDX, IDC_CHECK_ANSWERED, m_btnAnswered);
	DDX_Control(pDX, IDC_CHECK_UNANSWER, m_btnRejected);
	DDX_Text(pDX, IDC_STC_CALL_PROMPT, m_strPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallView, CCoolFormView)
	//{{AFX_MSG_MAP(CCallView)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CALL_AUDIO, OnBtnCallAudio)
	ON_BN_CLICKED(IDC_BTN_CALL_OFF, OnBtnCallOff)
	ON_BN_CLICKED(IDC_BTN_CALL_ON, OnBtnCallOn)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_FORMCALL_CMB_SIM, OnSelchangeFormcallCmbSim)
	ON_BN_CLICKED(IDC_BTN_CLEAR_REPORT, OnBtnClearReport)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_NUM_0,IDC_BTN_NUM_11, OnClickNum)
	ON_COMMAND_RANGE(IDC_CHECK_ALL, IDC_CHECK_DIALED, OnClickRecords)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallView diagnostics

#ifdef _DEBUG
void CCallView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCallView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCallView message handlers


void CCallView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	DWORD dwStyle = m_lstReport.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstReport.SetExtendedStyle(dwStyle);

	CString strText;
	strText.LoadString(IDS_LINKMAN);
	m_lstReport.InsertColumn(0,strText,LVCFMT_LEFT,100);
	strText.LoadString(IDS_CALL_TIME);
	m_lstReport.InsertColumn(1,strText,LVCFMT_LEFT,100);
	strText.LoadString(IDS_CALL_LONG);
    m_lstReport.InsertColumn(2, strText,LVCFMT_LEFT, 100);

	m_font.CreateFont(20, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T("Batang"));
	m_edtPhoneNum.SetFont(&m_font);

	//m_edtPhoneNum.SetFocus();
	m_btnAll.SetFocus();
	m_btnAll.SetCheck(1);


	for(int i = 0; i < RECORDS_COUNT; i++)
	{
		m_lstReport.InsertItem(i, _T(" "));
	}

	if(OpenReportFile())
	{
		LoadCallReport();
	}

	int nIdx = 0;
	m_cmbSim.ResetContent();
	for(i = 1; i<= MAX_SIM_NUM; i++)
	{
		if(theApp.IsPluggedSIM(i))
		{
			CString str;
			str.Format(_T("SIM%d"),i);
			m_cmbSim.AddString(str);
			m_cmbSim.SetItemData(nIdx++,i);
		}
	}

	if(nIdx > 0)
	{
		m_cmbSim.SetCurSel(0);
	}

	if(theApp.m_nSIMCount ==  1 )
	{
		m_cmbSim.EnableWindow(FALSE);
	}

#ifdef _EGO
	ActiveCurSIM();
#endif

}

BOOL CCallView::ActiveCurSIM()
{
	BOOL bOK = TRUE;
	if(theApp.m_pPortMain != NULL && theApp.m_bConnected)	
	{
		theApp.m_pPortMain->StopAutoReceive();
	}
	if(theApp.m_pSMSFactory != NULL && theApp.m_bConnected)
	{
		int nCurSim = m_cmbSim.GetCurSel();
		if(nCurSim != -1)
		{
			nCurSim = (int)m_cmbSim.GetItemData(nCurSim);
			bOK = theApp.m_pSMSFactory->SetActiveSim((_SMS_STORE_PLACE_E)nCurSim);
		}
	}
	if(theApp.m_pPortMain != NULL && theApp.m_bConnected)	
	{
		CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->StartAutoReport();
	}
	return bOK;
}

BOOL CCallView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
	{
		
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		
		if( cValue < _T('0') || cValue > _T('9') )
		{
			if( cValue != _T('+') && cValue != _T('*') && 
				cValue != _T('#') && cValue != _T(',') &&
				cValue != _T('P') && cValue != _T('W') && cValue != _T('N')&&
				cValue != _T(':') && cValue != _T(';') &&
				cValue != VK_BACK && 
				cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
			{
				return TRUE;
			}	
		}	
	}
	if (WM_KEYDOWN == pMsg->message)
    {
        // Process when receive ENTER
        if (VK_RETURN == pMsg->wParam)
        {
            SendMessage(WM_KEYDOWN, VK_RETURN);
            return TRUE;
        }
    }
	
	return CCoolFormView::PreTranslateMessage(pMsg);
}

HBRUSH CCallView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	return CCoolFormView::OnCtlColor(pDC,pWnd,nCtlColor);

}

void CCallView::OnClickNum(UINT uID)
{
	UpdateData();
	
	int nNum = uID - IDC_BTN_NUM_0;
	
	CString strNum;
	strNum.Format(_T("%d"), nNum);

	if(nNum == 10)
	{
		strNum = _T("*");
	}

	if(nNum == 11)
	{
		strNum = _T("#");
	}

	m_strNum += strNum;

	if(m_bTimer)
	{
		SendVTS(strNum);
	}

	UpdateData(FALSE);
}

void CCallView::OnBtnCallAudio() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm != NULL)
	{
		if(pFrm->m_pMSInfoView->m_bUrgency)
		{
			if(!IsUrgencyCall(m_strNum))
			{
				AfxMessageBox(IDS_ONLY_URGENCY);
				return;
			}
		}
	}

	m_BeginTime = 0;

	char szRet[MAX_PATH] = {0};
	CString strText;

	TCHAR szPara[MAX_PATH] = {0};


	if(theApp.m_pEtionFactory == NULL || !theApp.m_bConnected)
	{
		return;
	}

	
	if(!m_strNum.IsEmpty())
	{
		m_nFlagState = TOOL_CALL;

		CString strNum = _T(""); 
//		if(IsUrgencyCall(m_strNum))
//		{
//			DisplayPrompt(IDS_ERGENCE_CALLING);
//			strNum = m_strNum;//+ _T(",#");
//			if(m_strNum == _T("120"))
//			{
//				strNum += _T(",#");
//			}
//		}
//		else
		{
			DisplayPrompt(IDS_CALLING);
			strNum = m_strNum + _T(";");
		}
		
		_tcscpy(szPara, strNum);
		CWaitCursor cursor;
		EnableWindow(FALSE);
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CALL, 0, (LPVOID)szPara, _tcslen(szPara));
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		EnableWindow(TRUE);
		m_nFlagRecord = REPORT_CALL;
//		m_BeginTime = CTime::GetCurrentTime();
//		m_strEndTime = _T("00:00:00:00");
#ifdef _EGO
		if(strstr(szRet, "OK") != NULL || strstr(szRet, "+SIND: 9") != NULL)
#else
		if(strstr(szRet, "+SIND: 9") != NULL)
#endif
		{
			StartCalling();
		}
		else if(strstr(szRet, "ERROR") != NULL || strstr(szRet, "NO CARRIER") != NULL
			|| strstr(szRet, "+SIND: 6") != NULL)
		{
			DisplayPrompt(IDS_CALL_FAILED);
			StopRecords();
		}

	}
	else
	{
		DisplayPrompt(IDS_PHONE_NUMBER_EMPTY);
	}
	
}

void CCallView::OnBtnCallOff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szRet[MAX_PATH] = {0};
	CString strText;

	if(theApp.m_pEtionFactory == NULL || !theApp.m_bConnected /*|| m_strNum.IsEmpty()*/)
	{
		return;
	}
	
	m_nFlagState = TOOL_HANG_UP;

	CWaitCursor cursor;
	LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(HANG_UP, 0, NULL, 0);
	if(lpRet != NULL)
	{
		strcpy(szRet, lpRet);
	}
	
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,_T(" "));
	m_strPrompt = _T("");
	UpdateData(FALSE);

	//test
	WCHAR szTmp[100] = {0};
	MultiByteToWideChar(CP_ACP, 0, szRet, -1, szTmp, 100);
	TRACE(_T("----------------------------------------------ret: %s\n"), szTmp);
	//test
	
	//teana hu 2009.12.02
	BOOL bComCall = m_bComCalling;
	//
	if(m_bComCalling)
	{
		m_bComCalling = FALSE;
		((CMainFrame *)AfxGetMainWnd())->KillCallingTimer(FALSE);
	}
	
	if(strstr(szRet, "OK") != NULL)
	{
		if(m_nFlagRecord == REPORT_CALL)
		{
			m_BeginTime = CTime::GetCurrentTime();
			m_strEndTime = _T("00:00:00:00");
		}
		
		if(m_nFlagRecord == -1 && bComCall)
		{
			m_nFlagRecord = REPORT_REJECT;
			m_BeginTime = CTime::GetCurrentTime();
			m_strEndTime = _T("00:00:00:00");
		}

		StopRecords();			
		DisplayPrompt(IDS_HANGUP_SUCCESS);
	}
	else //if(strstr(szRet, "ERROR") != NULL)
	{			
		StopRecords();
		DisplayPrompt(IDS_HANGUP_FAILED);
	}

	Sleep(500);
	
}

void CCallView::OnBtnCallOn() 
{
	// TODO: Add your control notification handler code here

	if(!m_bComCalling)
	{
		return;
	}

	char szRet[MAX_PATH] = {0};
	CString strText;
	
	if(theApp.m_pEtionFactory == NULL || !theApp.m_bConnected)
	{
		return;
	}
	m_nFlagState = TOOL_HOLD_ON;

	CWaitCursor cursor;
	EnableWindow(FALSE);
	LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(HOLD_ON, 0, NULL, 0);
	if(lpRet != NULL)
	{
		strcpy(szRet, lpRet);
	}
	
	EnableWindow(TRUE);
	
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,_T(" "));
	m_strPrompt = _T("");
	UpdateData(FALSE);

	((CMainFrame *)AfxGetMainWnd())->KillCallingTimer();
	
	if(strstr(szRet, "NO CARRIER") != NULL || strstr(szRet, "ERROR") != NULL)
	{
		DisplayPrompt(IDS_HOLDON_FAILED);
		if(m_bComCalling)
		{
			m_bComCalling = FALSE;
		}
	}
	else if(strstr(szRet, "OK") != NULL || strstr(szRet, "+SIND: 9") != NULL)
	{
		m_nFlagRecord = REPORT_ANSWER;
		StartHoldOn();
	}
	
}

void CCallView::PromtComingCall(LPCTSTR szRet)
{
	if(m_nFlagState != TOOL_HOLD_ON && m_nFlagState != TOOL_HANG_UP)
	{
		m_strComingCall = szRet;
		
		m_strNum = m_strComingCall;
		
		m_strPrompt.LoadString(IDS_CALLING_PHONENUM);
		if(m_strComingCall.IsEmpty())
		{
			m_strComingCall.LoadString(IDS_UNKNOWN_NUMBER);
		}
		m_strPrompt += m_strComingCall;

		((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,m_strPrompt);
		
		UpdateData(FALSE);	
		
		if(!m_bComCalling)
		{
			m_bComCalling = TRUE;
		}
		m_bRejectByOther = FALSE;
	}
}

CString CCallView::ConvercTStoS(CTimeSpan ts)
{
	CString strTime;
	strTime.Format(_T("%02ld:%02ld:%02ld:%02ld"),
	ts.GetDays(), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
	return strTime;
}

CString CCallView::ConvertTimeToDate(CTime ts)
{
	CString strTime;
	strTime.Format(_T("%4d/%02d/%02d %02d:%02d"), ts.GetYear(), ts.GetMonth(), ts.GetDay(), ts.GetHour(), ts.GetMinute());
	return strTime;
}

void CCallView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == EVENT_TIME_LONG)
	{
		CTime curTime = CTime::GetCurrentTime();	

		CTimeSpan ts= curTime - m_BeginTime;
		
		m_strEndTime = ConvercTStoS(ts);

		int nCount = m_vecAllRecord.size();
		
		if(GetCheckedButton() != IDC_CHECK_ALL)
		{
			OnClickRecords(IDC_CHECK_ALL);
		}		

		m_lstReport.SetItemText(nCount, 0, m_strNum);
		CString strBegainTime;
		strBegainTime = ConvertTimeToDate(m_BeginTime);
		m_lstReport.SetItemText(nCount, 1, strBegainTime);
		m_lstReport.SetItemText(nCount, 2, m_strEndTime);
		
	}
	
	CCoolFormView::OnTimer(nIDEvent);
}

void CCallView::StopRecords()
{
	if(m_bTimer)
	{
		KillTimer(EVENT_TIME_LONG);
		m_bTimer = FALSE;
	}
	if(m_BeginTime != 0 )
	{
	#ifdef _UNICODE
		wsprintf(m_PhoneBook.szBegainTime, _T("%s"), ConvertTimeToDate(m_BeginTime));
	#else
		sprintf(m_PhoneBook.szBegainTime, _T("%s"), ConvertTimeToDate(m_BeginTime));
	#endif
		
		_tcscpy(m_PhoneBook.szPhoneNum, m_strNum);
		_tcscpy(m_PhoneBook.szTotalTime, m_strEndTime);
		
		if(m_nFlagRecord == REPORT_CALL)
		{
			m_vecCallRecord.push_back(m_PhoneBook);
		}
		if(m_nFlagRecord == REPORT_REJECT)
		{
			m_vecRejectRecord.push_back(m_PhoneBook);
		}
		if(m_nFlagRecord == REPORT_ANSWER)
		{
			m_vecAnwseredRecord.push_back(m_PhoneBook);
		}
		m_vecAllRecord.push_back(m_PhoneBook);
	}

	m_btnAll.SetFocus();

	OnClickRecords(IDC_CHECK_ALL);

	m_nFlagRecord = -1;
	m_nFlagState = -1;
}

void CCallView::StopCalling()
{
	//notice customer state
	UINT uID = 0;
	TRACE(_T("----------------------State: %d, Timer: %d\n"), m_nFlagState, m_bTimer);
	if((m_nFlagState == TOOL_CALL && m_bTimer) || (m_nFlagState == TOOL_HOLD_ON && m_bTimer))
	{
		uID = IDS_ANOTHER_HOLD_DOWN;
		DisplayPrompt(uID);
		StopRecords();
	}
	else if(m_nFlagState == TOOL_CALL && !m_bTimer)
	{
		uID = IDS_CALL_FAILED;
		DisplayPrompt(uID);
		m_nFlagRecord = -1;
		m_nFlagState = -1;
	}
	else if(m_nFlagState == TOOL_HOLD_ON && !m_bTimer)
	{
		uID = IDS_HOLDON_FAILED;
		DisplayPrompt(uID);
		m_BeginTime = 0;
		m_nFlagRecord = -1;
		m_nFlagState = -1;
	}
	else if(m_nFlagState == TOOL_HANG_UP )
	{
		uID = IDS_HANGUP_FAILED;
		DisplayPrompt(uID);
		StopRecords();
	}
	else if(m_nFlagRecord == -1 && !m_bRejectByOther) //teana hu 2009.10.16
	{
		m_nFlagRecord = REPORT_REJECT;
		m_BeginTime = CTime::GetCurrentTime();
		m_strEndTime = _T("00:00:00:00");
		StopRecords();
		m_nFlagRecord= -1;
		m_BeginTime = 0;
		m_bRejectByOther = TRUE;
	}
	
	if(m_bComCalling)
	{
		uID = IDS_NO_CARRIER;
		DisplayPrompt(uID);
	}

//	DisplayPrompt(uID);
	
//	StopRecords();
}

void CCallView::OnClickRecords(UINT uID)
{
	for(int i = IDC_CHECK_ALL; i <= IDC_CHECK_DIALED; i++)
	{
		if((UINT)i == uID)
		{
			((CButton *)GetDlgItem(i))->SetCheck(1);
		}
		else
		{
			((CButton *)GetDlgItem(i))->SetCheck(0);
		}
	}

	for(i = 0; i < RECORDS_COUNT; i++)
	{
		m_lstReport.SetItemText(i, 0, _T(" "));
		m_lstReport.SetItemText(i, 1, _T(" "));
		m_lstReport.SetItemText(i, 2, _T(" "));
	}


	int nCount  = 0;

	switch(uID) 
	{
	case IDC_CHECK_ALL:
		nCount = m_vecAllRecord.size();
		for( i = 0; i < nCount; i++)
		{
			m_lstReport.SetItemText(i, 0, m_vecAllRecord[i].szPhoneNum);
			m_lstReport.SetItemText(i, 1, m_vecAllRecord[i].szBegainTime);
			m_lstReport.SetItemText(i, 2, m_vecAllRecord[i].szTotalTime);
		}
		break;
	case IDC_CHECK_ANSWERED:
		nCount = m_vecAnwseredRecord.size();
		for( i = 0; i < nCount; i++)
		{
			m_lstReport.SetItemText(i, 0, m_vecAnwseredRecord[i].szPhoneNum);
			m_lstReport.SetItemText(i, 1, m_vecAnwseredRecord[i].szBegainTime);
			m_lstReport.SetItemText(i, 2, m_vecAnwseredRecord[i].szTotalTime);
		}
		break;
	case IDC_CHECK_UNANSWER:
		nCount = m_vecRejectRecord.size();
		for( i = 0; i < nCount; i++)
		{
			m_lstReport.SetItemText(i, 0, m_vecRejectRecord[i].szPhoneNum);
			m_lstReport.SetItemText(i, 1, m_vecRejectRecord[i].szBegainTime);
			m_lstReport.SetItemText(i, 2, m_vecRejectRecord[i].szTotalTime);
		}
		break;
	case IDC_CHECK_DIALED:
		nCount = m_vecCallRecord.size();
		for( i = 0; i < nCount; i++)
		{
			m_lstReport.SetItemText(i, 0, m_vecCallRecord[i].szPhoneNum);
			m_lstReport.SetItemText(i, 1, m_vecCallRecord[i].szBegainTime);
			m_lstReport.SetItemText(i, 2, m_vecCallRecord[i].szTotalTime);
		}
		break;
	default:
		break;
	}
}

void CCallView::StartCalling()
{
	TRACE(_T("----------------------Start 1\n"));
	if(!m_bTimer && !m_bComCalling)
	{
		TRACE(_T("----------------------Start 2\n"));
		m_BeginTime = CTime::GetCurrentTime();
		m_strEndTime = _T("00:00:00:00");

		SetTimer(EVENT_TIME_LONG, 1000, NULL);
		m_bTimer = TRUE;
		
		DisplayPrompt(IDS_CALL_SUCCESS);
	}
}

BOOL CCallView::OnEraseBkgnd( CDC* pDC )
{
	return CCoolFormView::OnEraseBkgnd(pDC);
}

void CCallView::ClearAll()
{
	m_strNum.Empty();
	m_strComingCall.Empty();
	m_strEndTime.Empty();
	m_strPrompt.Empty();
	m_bComCalling = FALSE;
	memset(&m_PhoneBook, 0, sizeof(TELEPHONT_BOOK));
	if(m_bTimer)
	{
		KillTimer(EVENT_TIME_LONG);
		m_bTimer = FALSE;
	}

	for(int i = 0; i < RECORDS_COUNT; i++)
	{
		m_lstReport.SetItemText(i, 0, _T(" "));
		m_lstReport.SetItemText(i, 1, _T(" "));
		m_lstReport.SetItemText(i, 2, _T(" "));
	}

	m_vecAllRecord.clear();
	m_vecCallRecord.clear();
	m_vecAnwseredRecord.clear();
	m_vecRejectRecord.clear();

	UpdateData(FALSE);
	m_nFlagRecord = -1;
	m_nFlagState = -1;
}

void CCallView::DisplayPrompt(UINT uID)
{
	m_strPrompt.LoadString(uID);
	UpdateData(FALSE);
}

void CCallView::LoadCallReport()
{
	if(m_pReportFile == NULL)
	{
		return;
	}
	
	TCHAR szText[MAX_BUF_LEN] = {0};
	CString strText;

	while(!feof(m_pReportFile))
	{
		if(_fgetts(szText, MAX_BUF_LEN, m_pReportFile) != NULL)
		{
			strText = szText;
			ParseReportLine(strText);
		}
		
	}

	_chsize(m_pReportFile->_file, 0);
	
	//OnClickRecords(IDC_CHECK_ALL);
	int nCount = m_vecAllRecord.size();
	for( int i = 0; i < nCount; i++)
	{
		m_lstReport.SetItemText(i, 0, m_vecAllRecord[i].szPhoneNum);
		m_lstReport.SetItemText(i, 1, m_vecAllRecord[i].szBegainTime);
		m_lstReport.SetItemText(i, 2, m_vecAllRecord[i].szTotalTime);
	}
}

void CCallView::ParseReportLine(CString strLine)
{
	DWORD dwFlag = (DWORD)-1; //0: D 1: R 2: C 3: A

	if(strLine.GetAt(0) == _T('D'))
	{
		dwFlag = 0;
	}
	if(strLine.GetAt(0) == _T('R'))
	{
		dwFlag = 1;
	}
	if(strLine.GetAt(0) == _T('C'))
	{
		dwFlag = 2;
	}
	if(strLine.GetAt(0) == _T('A'))
	{
		dwFlag = 3;
	}

	TELEPHONT_BOOK PhoneBook;

	int nIndex = strLine.Find(_T("|"));
	if(nIndex != -1)
	{
		strLine.Delete(0, nIndex + 1);
		nIndex = strLine.Find(_T("|"));
		if(nIndex != -1)
		{
			_tcscpy(PhoneBook.szPhoneNum, strLine.Left(nIndex));
			
			strLine.Delete(0, nIndex + 1);
			nIndex = strLine.Find(_T("|"));
			if(nIndex != -1)
			{
				_tcscpy(PhoneBook.szBegainTime, strLine.Left(nIndex));
				strLine.Delete(0, nIndex + 1);
				strLine.TrimRight(_T("\r\n"));
				strLine.TrimRight(_T("\n"));
				_tcscpy(PhoneBook.szTotalTime, strLine);
			}
		}
	}

	switch(dwFlag) {
	case 0:
		m_vecRejectRecord.push_back(PhoneBook);
		break;
	case 1:
		m_vecAnwseredRecord.push_back(PhoneBook);
		break;
	case 2:
		m_vecCallRecord.push_back(PhoneBook);
		break;
	case 3:
		m_vecAllRecord.push_back(PhoneBook);
		break;
	default:
		break;
	}
}

void CCallView::WriteCallReport()
{
	TCHAR szText[MAX_BUF_LEN] = {0};
	char  szTextA[MAX_BUF_LEN] = {0};

	for(int i = 0; i <(int) m_vecRejectRecord.size(); i++)
	{
		_stprintf(szText, _T("D|%s|%s|%s\r\n"), m_vecRejectRecord[i].szPhoneNum, m_vecRejectRecord[i].szBegainTime, m_vecRejectRecord[i].szTotalTime);//lint !e559
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,szText,_tcslen(szText),szTextA,MAX_BUF_LEN,NULL,NULL);
		fwrite(szTextA, sizeof(char), strlen(szTextA), m_pReportFile);
#else
		fwrite(szText, sizeof(TCHAR), _tcslen(szText), m_pReportFile);
#endif
	}

	for(i = 0; i < (int)m_vecAnwseredRecord.size(); i++)
	{
		_stprintf(szText, _T("R|%s|%s|%s\r\n"), m_vecAnwseredRecord[i].szPhoneNum, m_vecAnwseredRecord[i].szBegainTime, m_vecAnwseredRecord[i].szTotalTime);//lint !e559
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,szText,_tcslen(szText),szTextA,MAX_BUF_LEN,NULL,NULL);
		fwrite(szTextA, sizeof(char), strlen(szTextA), m_pReportFile);
#else
		fwrite(szText, sizeof(TCHAR), _tcslen(szText), m_pReportFile);
#endif
	}

	for(i = 0; i < (int)m_vecCallRecord.size(); i++)
	{
		_stprintf(szText, _T("C|%s|%s|%s\r\n"), m_vecCallRecord[i].szPhoneNum, m_vecCallRecord[i].szBegainTime, m_vecCallRecord[i].szTotalTime);//lint !e559
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,szText,_tcslen(szText),szTextA,MAX_BUF_LEN,NULL,NULL);
		fwrite(szTextA, sizeof(char), strlen(szTextA), m_pReportFile);
#else
		fwrite(szText, sizeof(TCHAR), _tcslen(szText), m_pReportFile);
#endif
	}

	for(i = 0; i < (int)m_vecAllRecord.size(); i++)
	{
		_stprintf(szText, _T("A|%s|%s|%s\r\n"), m_vecAllRecord[i].szPhoneNum, m_vecAllRecord[i].szBegainTime, m_vecAllRecord[i].szTotalTime);//lint !e559
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,szText,_tcslen(szText),szTextA,MAX_BUF_LEN,NULL,NULL);
		fwrite(szTextA, sizeof(char), strlen(szTextA), m_pReportFile);
#else
		fwrite(szText, sizeof(TCHAR), _tcslen(szText), m_pReportFile);
#endif
	}

}

BOOL CCallView::OpenReportFile()
{
	TCHAR szDirectory[MAX_PATH] = {0};
	GetModuleFileName(NULL, szDirectory, MAX_PATH);

	CString strDirectory = szDirectory;
	strDirectory = strDirectory.Left(strDirectory.ReverseFind('\\') + 1);

	strDirectory += _T("CallReport.txt");

	m_pReportFile = _tfopen(strDirectory, _T("a+"));

	if(m_pReportFile == NULL)
	{
		AfxMessageBox(_T("Open CallReport.txt file failed!"));
		return FALSE;
	}
	return TRUE;
}

void CCallView::CloseReportFile()
{
	if(m_pReportFile != NULL)
	{
		fclose(m_pReportFile);
		m_pReportFile = NULL;
	}
}

void CCallView::SendVTS(CString strNum)
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		TCHAR szPara[MAX_PATH] = {0};
		
#ifdef _UNICODE
		swprintf(szPara, _T("%s,10"), strNum);
#else
		sprintf(szPara, _T("%s,10"), strNum);
#endif		
		
		theApp.m_pEtionFactory->SendATCmd(VTS_SET, 0, (LPVOID)szPara, _tcslen(szPara));

//#ifdef _UNICODE
//		swprintf(szPara, _T("%s,0"), strNum);
//#else
//		sprintf(szPara, _T("%s,0"), strNum);
//#endif
//		strcpy(szRet, (LPCSTR)theApp.m_pEtionFactory->SendATCmd(VTS_SET, 0, (LPVOID)szPara, _tcslen(szPara)));
	}
}
void CCallView::OnSelchangeFormcallCmbSim() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ActiveCurSIM();
}

BOOL CCallView::IsUrgencyCall(CString strNum)
{
	if(strNum == _T("110") || strNum == _T("112")
		|| strNum == _T("119") || strNum == _T("911")
		|| strNum == _T("120"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CCallView::StartHoldOn()
{
	if(m_bComCalling)
	{
		m_BeginTime = CTime::GetCurrentTime();
		m_strEndTime = _T("00:00:00:00");

		SetTimer(EVENT_TIME_LONG, 0, NULL);
		m_bTimer = TRUE;
		m_bComCalling = FALSE;	
		((CMainFrame *)AfxGetMainWnd())->KillCallingTimer(FALSE);

		((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(PROMT_INFO_IND,_T(" "));
		DisplayPrompt(IDS_HOLDON_SUCCESS);
		m_nFlagRecord = REPORT_ANSWER;
	}
}

int CCallView::GetCallState()
{
	return m_nFlagState;
}

void CCallView::OnBtnClearReport() 
{
	// TODO: Add your control notification handler code here

	for(int i = 0; i < RECORDS_COUNT; i++)
	{
		m_lstReport.SetItemText(i, 0, _T(" "));
		m_lstReport.SetItemText(i, 1, _T(" "));
		m_lstReport.SetItemText(i, 2, _T(" "));
	}

	m_vecAllRecord.clear();
	m_vecCallRecord.clear();
	m_vecAnwseredRecord.clear();
	m_vecRejectRecord.clear();

	if(m_pReportFile != NULL)
	{
		_chsize(m_pReportFile->_file, 0);
		fflush(m_pReportFile);
	}
	
}

UINT CCallView::GetCheckedButton()
{
	for(int i = IDC_CHECK_ALL; i <= IDC_CHECK_DIALED; i++)
	{
		if(((CButton *)GetDlgItem(i))->GetCheck() == 1)
		{
			break;
		}
	}

	return i;
}