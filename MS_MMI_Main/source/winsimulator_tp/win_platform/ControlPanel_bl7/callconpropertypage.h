#if !defined(AFX_CALLCONPROPERTYPAGE_H__0E677F2A_5D22_451C_B753_78190AB57868__INCLUDED_)
#define AFX_CALLCONPROPERTYPAGE_H__0E677F2A_5D22_451C_B753_78190AB57868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallConPropertyPage.h : header file
//
#include "stdafx.h"
#include "ConPanLayer.h"
#include "resource.h"
#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// CCallConPropertyPage dialog

class CCallConPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCallConPropertyPage)

// Construction
public:
	CCallConPropertyPage();
	~CCallConPropertyPage();


public:
	int  sys_id;
	void OnPowerOff();  
	void DealCPMsg(void *pmsg);

// Dialog Data
	//{{AFX_DATA(CCallConPropertyPage)
	enum { IDD = IDD_DIALOG_CALLCON };
	CStatic	m_wndCallImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCallConPropertyPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallConPropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMakecall();
	afx_msg void OnButtonAnswer();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboCallno();
	afx_msg void OnItemchangedListCall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckSilcalled();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	typedef enum CALL_STATUS
	{        
		CALL_IDLE = 0,
		CALL_CALLING = 1,
		CALL_BEING_CALLED,
		CALL_COMMUNICATING
	} CALL_STATUS_E;
    CALL_STATUS_E m_csCallLastStatus;
    CALL_STATUS_E m_csCallCurStatus;
    int  m_nCallTi;
    typedef struct {
    CALL_STATUS_E csCallLastStatus;
    CALL_STATUS_E csCallCurStatus;
    BOOL          bStartFromMobile;            
    CString       csRemotePhoneNum;
    CString       csDialNum;
    CString       csDtmfString;
    } CALL_ELEMENT_T;
    CArray<CALL_ELEMENT_T, CALL_ELEMENT_T> m_aCalls;
    int m_nIndexSelectedCall;

    BOOL m_bSilentWhenCalled;
    BOOL m_bStopCalledRing;
    BOOL m_bStopMovie;
    int  m_nMoiveCount;
    CImageList *m_pilPhoneImages;
    
    BOOL InitializeCallList(int nRow);
    int GetActiveCallNum();
    int GetActiveCallNumStartFromConPan();
    int GetMinAvailableCallNo();
    void SetCallStatus(int nIndex, 
                       int nCallStatus, 
                       CString csFromPhoneNum = "", 
                       CString csToPhoneNum ="");
    void DisplayCall(int nIndex);    
    BOOL StartCalledRing();
    void StopCalledRing();
    BOOL StartMoive();
    void StopMovie();
    void SetDtmfString(int nIndex, 
                       char csDtmfString);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLCONPROPERTYPAGE_H__0E677F2A_5D22_451C_B753_78190AB57868__INCLUDED_)
