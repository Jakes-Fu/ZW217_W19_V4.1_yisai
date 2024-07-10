#if !defined(AFX_DLGBYTE_H__33A66129_4D43_4BCF_8DDC_D10E004FD653__INCLUDED_)
#define AFX_DLGBYTE_H__33A66129_4D43_4BCF_8DDC_D10E004FD653__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgByte.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgByte dialog

class CDlgByte : public CDialog
{
// Construction
public:
	CDlgByte(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgByte)
	enum { IDD = IDD_DIALOG_BYTE };
		// NOTE: the ClassWizard will add data members here
	CString m_strByte0;
	CString m_strByte1;
	CString m_strByte2;
	CString m_strByte3;
	//}}AFX_DATA
	UINT	m_uByte0;
	UINT	m_uByte1;
	UINT	m_uByte2;
	UINT	m_uByte3;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgByte)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetValue(WORD wValue);
	void FetchValue(WORD &wValue);

private:
	void InitValue(WORD wValue);
	void GetValue(WORD &wValue);

private:
	WORD m_wValue;




// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgByte)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBYTE_H__33A66129_4D43_4BCF_8DDC_D10E004FD653__INCLUDED_)
