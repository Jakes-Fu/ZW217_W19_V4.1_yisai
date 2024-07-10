#if !defined(AFX_CALLVIEW_H__1EA18509_4199_410D_9921_BCA81D358A6F__INCLUDED_)
#define AFX_CALLVIEW_H__1EA18509_4199_410D_9921_BCA81D358A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallView.h : header file
//
#pragma warning(push,3)
#include <vector>
#pragma warning(pop)

using namespace std;


typedef struct _TELEPHONT_BOOK
{
	TCHAR szPhoneNum[MAX_PATH];
	TCHAR szBegainTime[MAX_PATH];
	TCHAR szTotalTime[MAX_PATH];
}TELEPHONT_BOOK;

#define EVENT_TIME_LONG  127
#define RECORDS_COUNT 100

typedef enum{
	TOOL_CALL,
	TOOL_HANG_UP,
	TOOL_HOLD_ON
};

typedef enum{
	REPORT_CALL,
	REPORT_REJECT,
	REPORT_ANSWER
};

/////////////////////////////////////////////////////////////////////////////
// CCallView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CoolFormView.h"

class CCallView : public CCoolFormView
{
public:
	CCallView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCallView)

// Form Data
public:
	//{{AFX_DATA(CCallView)
	enum { IDD = IDD_FORMVIEW_CALL };
	CComboBox	m_cmbSim;
	CListCtrl m_lstReport;
	CEdit     m_edtPhoneNum;
	CString   m_strNum;
	CButton   m_btnAll;
	CButton   m_btnCalled;
	CButton   m_btnAnswered;
	CButton   m_btnRejected;
	CString	  m_strPrompt;
	//}}AFX_DATA

// Attributes
public:
	void PromtComingCall(LPCTSTR szRet);
	void StopRecords();
	void StartCalling();
	void StopCalling();
	void ClearAll();
	void DisplayPrompt(UINT uID);
	void StartHoldOn();

	void LoadCallReport();
	void WriteCallReport();
	BOOL OpenReportFile();
	void CloseReportFile();
	void ParseReportLine(CString strLine);
	
	void SendVTS(CString strNum);
	int GetCallState();

// Operations
public:
	BOOL  m_bComCalling;

private:
	CFont m_font;
	CString m_strComingCall;
	CTime m_BeginTime;
	CString m_strEndTime;
	BOOL  m_bTimer;
	
	//teana hu 2009.10.16
	BOOL m_bRejectByOther;

	TELEPHONT_BOOK m_PhoneBook;
	int  m_nFlagRecord; //0: call; 1:hangup; 2: hangon 
	int  m_nFlagState; //0: call; 1:hangup; 2: hangon 

	FILE *m_pReportFile;

	vector <TELEPHONT_BOOK> m_vecAllRecord;
	vector <TELEPHONT_BOOK> m_vecCallRecord;
	vector <TELEPHONT_BOOK> m_vecAnwseredRecord;
	vector <TELEPHONT_BOOK> m_vecRejectRecord;
	


private:
	CString ConvercTStoS(CTimeSpan ts);
	CString ConvertTimeToDate(CTime ts);

	BOOL ActiveCurSIM();
	BOOL IsUrgencyCall(CString strNum);
	
	UINT GetCheckedButton();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCallView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCallView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnCallAudio();
	afx_msg void OnBtnCallOff();
	afx_msg void OnBtnCallOn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnSelchangeFormcallCmbSim();
	afx_msg void OnBtnClearReport();
	//}}AFX_MSG
	void OnClickNum(UINT uID);
	void OnClickRecords(UINT uID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLVIEW_H__1EA18509_4199_410D_9921_BCA81D358A6F__INCLUDED_)
