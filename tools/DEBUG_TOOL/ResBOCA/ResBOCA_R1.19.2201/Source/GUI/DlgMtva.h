#pragma once
#include "MAParser.h"

#define ECallMaxLen	30
#define VMailMaxLen	30

#define URLMaxLen	126


// CDlgMTVA dialog

class CDlgMtva : public CDialog
{
	DECLARE_DYNAMIC(CDlgMtva)

public:
	CDlgMtva(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMtva();

	BOOL    m_bViewOnly;
	BOOL	m_bActive;
	CString	m_strECall[ECallCount];
	CString	m_strVMail[VMailCount];
	CString	m_strURL;
	CString	m_strPos;
	//CString m_strUsed;
	UINT	m_nMnc;
	UINT	m_nMcc;
	BOOL SetBuf(LPBYTE pBuf,DWORD dwSize, LPCTSTR lpszTmpPath);
	CString   m_strMTVA;

// Dialog Data
	enum { IDD = IDD_DLG_MULIT_VARIANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void UpdateCtrlState();
	void ShowCurMtva();
	void SaveCurMtva();

	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg void OnNext();
	afx_msg void OnPrev();
	virtual void OnOK();

private:
	CMAParser m_Parser;
	int		  m_nCurIndex;
	CString   m_strTmpPath;

	DECLARE_MESSAGE_MAP()
public:
};
