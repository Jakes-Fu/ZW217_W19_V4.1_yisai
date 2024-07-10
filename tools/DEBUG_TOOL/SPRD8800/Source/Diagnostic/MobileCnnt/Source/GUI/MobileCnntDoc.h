// MobileCnntDoc.h : interface of the CMobileCnntDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILECNNTDOC_H__B6EEF5B5_68AC_4453_A933_DBC2FFA06890__INCLUDED_)
#define AFX_MOBILECNNTDOC_H__B6EEF5B5_68AC_4453_A933_DBC2FFA06890__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum
{
	HOME_CHECK,
	DIAL_CHECK,
	SMS_CHECK,
	CALL_CHECK,
	ENGINEERING_CHECK,
	CONFIG_CHECK,
};

#define TB_NUM 6

class CMobileCnntDoc : public CDocument
{
protected: // create from serialization only
	CMobileCnntDoc();
	DECLARE_DYNCREATE(CMobileCnntDoc)

// Attributes
public:

// Operations
public:
	void ChangeTBButton(UINT nOrigID, UINT nDestID);
	void SetTBCheck(DWORD dwTBFlag);
	void EnableAllItems(BOOL bEnable);

public:
	BOOL m_bDisDialUP;
	BOOL m_bEnableDialUP;
	BOOL m_bEnableCall;
	BOOL m_bEnableMSinfo;
	BOOL m_bEnableSMS;
	BOOL m_bEnableConfig;
	BOOL m_bEnableEngineer;
	BOOL m_bEnablePoweroff;
	BOOL m_bShowSMSWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMobileCnntDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL


public:
	UINT m_uTbCheckFlag[TB_NUM];
	

// Implementation
public:
	virtual ~CMobileCnntDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMobileCnntDoc)
	afx_msg void OnLoadSMS();
	afx_msg void OnDialUp();
	afx_msg void OnUpdateDialUp(CCmdUI* pCmdUI);
	afx_msg void OnLoadCall();
	afx_msg void OnUpdateLoadCall(CCmdUI* pCmdUI);
	afx_msg void OnLoadConfig();
	afx_msg void OnUpdateLoadConfig(CCmdUI* pCmdUI);
	afx_msg void OnLoadEngeneering();
	afx_msg void OnUpdateLoadEngeneering(CCmdUI* pCmdUI);
	afx_msg void OnLoadHelp();
	afx_msg void OnUpdateLoadHelp(CCmdUI* pCmdUI);
	afx_msg void OnHome();
	afx_msg void OnUpdateLoadSms(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHome(CCmdUI* pCmdUI);
	afx_msg void OnDisDialUp();
	afx_msg void OnUpdateDisDialUp(CCmdUI* pCmdUI);
	afx_msg void OnLoadAll();
	afx_msg void OnUpdateLoadAll(CCmdUI* pCmdUI);
	afx_msg void OnPowerOff();
	afx_msg void OnUpdatePowerOff(CCmdUI* pCmdUI);
	afx_msg void OnHsdpaVersion();
	afx_msg void OnUpdateHsdpaVersion(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOBILECNNTDOC_H__B6EEF5B5_68AC_4453_A933_DBC2FFA06890__INCLUDED_)
