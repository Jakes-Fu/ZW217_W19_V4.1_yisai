#if !defined(AFX_SMSDLG_H__5197C657_4BB3_4FE2_BA75_D74A5047136B__INCLUDED_)
#define AFX_SMSDLG_H__5197C657_4BB3_4FE2_BA75_D74A5047136B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSDlg.h : header file
//

#include "..\effect\CoolEdit.h"
#include "DlgSendPrg.h"

typedef enum E_SMS_EDIT_MODE
{
	SEM_UNKNOWN = 0,
	SEM_EDIT    = 1,
	SEM_FORWARD = 2,
	SEM_ANSWER  = 3,
} SMS_EDIT_MODE;
/////////////////////////////////////////////////////////////////////////////
// CSMSDlg dialog
struct _SMS;
class ISMS;
class CInfoWrapper;
class CSMSDlg : public CDialog
{
// Construction
public:
	void SetEditInfo( int nEditMode, _SMS * psms );

	//void SetForwardMode( int nIdx, BOOL bForward = TRUE );
	//void SetAnswerMode( int nIdx, BOOL bAnswer = TRUE );
	//void SetEditMode( int nIdx, BOOL bEdit = TRUE);
	CSMSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSMSDlg)
	enum { IDD = IDD_DLG_SMS };
	CCoolEdit	m_edtContent;
	CButton	m_btnGroup;
	CStatic	m_stcPlace;
	CComboBox	m_cmbSendSave;
	CComboBox   m_cmbStore;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnPresetSMS;
	CButton	m_btnPhoneNum;
	CString	m_strContent;
	CString	m_strPhoneNum;
	int		m_nStore;
	CString	m_strInputNum;
	CString	m_strLeftNum;
	BOOL	m_bAutoSplish;
	BOOL	m_bStateReport;
	CString	m_strSmsNum;
	int		m_nCurSim;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL IsExistInPc( LPCTSTR pszPhoneNum, LPCTSTR pszContent );
	void InitCtrls(void);
	void SetSMSPlace( int nStore );
	BOOL SaveSMS( LPCTSTR pszName, LPCTSTR pszPhoneNum, BOOL bSend);
	BOOL SendSMS(LPCTSTR pszPhoneNum, BOOL bMore = FALSE);
	BOOL StrIsValidate(LPCTSTR pszStr);
	BOOL StrIsNumeric(LPCTSTR pszStr, int nLen);
	BOOL StrIsNumeric(LPCTSTR pszStr);
	BOOL EmptyPrompt( UINT nID );
	BOOL IsValidateData(void);
	void InitCmbBox(void);
#ifndef _UNICODE
	BOOL HasUcs2Char(LPCSTR pStr) const;
#else
	BOOL HasUcs2Char(LPCTSTR pStr) const;
#endif
	int  SplishContent(void);
	LPTSTR GetStrLeft(LPCTSTR pStr,int nCount);
	LPTSTR GetStrRight(LPCTSTR pStr,int nCount);

    int  GetFullCharCount( LPCTSTR pStr );

	// Generated message map functions
	//{{AFX_MSG(CSMSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEdtPhonenum();
	afx_msg void OnVscrollEdtPhonenum();
	afx_msg void OnBtnOk();
	virtual void OnOK();
	afx_msg void OnBtnPresetsms();
	afx_msg void OnBtnPhoneNum();
	afx_msg void OnSelchangeCmbSendsave();
	afx_msg void OnRdoStore();
	afx_msg void OnRdoMs();
	afx_msg void OnRdoSim();
	afx_msg void OnChangeEdtContent();
	afx_msg void OnChkAutosplish();
	afx_msg void OnBtnGroup();
	afx_msg void OnChkStatereport();
	afx_msg void OnChangeEdtPhonenum();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static DWORD WINAPI GetThreadFunc(LPVOID lpParam);

	DWORD OnSend();

	void  ModifyCtrls(BOOL bActive);

	BOOL m_bStore;
    BOOL m_bTrunked;
    CStringArray m_arrContent;

	int  m_nUsedInPhone;
	int  m_nTotalInPhone;
	int  m_nUsedInSIM;
	int  m_nTotalInSIM;

	int  m_nUsedInSIM2;
	int  m_nTotalInSIM2;

	int  m_nUsedInSIM3;
	int  m_nTotalInSIM3;

	int  m_nUsedInSIM4;
	int  m_nTotalInSIM4;
	
	
	_TCHAR m_szBuf[1024 * 2];

	//BOOL m_bEditMode;     // 编辑模式
	//BOOL m_bAnswerMode;   // 应答模式
	//BOOL m_bForwardMode;  // 转发模式
	//int  m_nSMSIdx;     

	// 名称和电话号码的映射
	CMap<CString, LPCTSTR, CString, LPCTSTR>m_mapNameToPhone;
	// 可能既包含名称，也包含号码;如果是名称则需到m_mapNameToPhone中找对应的号码
	// 号码全为数字，名称至少一位不是数字
	CStringArray m_arrName;  

	CMap<int, int, int, int> m_mapSimIndex;

	SMS_EDIT_MODE  m_EditMode;
	_SMS         * m_pSMS;
	ISMS         * m_pSMSFactory;
	CInfoWrapper * m_pIW;
	
	CEdit          m_edtPhoneNum;
	
	BOOL           m_bModified;  //对于打开一个SMS时，是否改变的标识


    CDlgSendPrg   * pDlgSendPrg;
	static const DWORD SEND_FLAG;
	static const DWORD SAVE_FLAG;
	static const DWORD SEND_SAVE_FLAG;

	BOOL           m_bSimSelCmdEnable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSDLG_H__5197C657_4BB3_4FE2_BA75_D74A5047136B__INCLUDED_)
