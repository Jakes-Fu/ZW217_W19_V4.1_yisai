#if !defined(AFX_PACKETMODERM_H__E2ABDB24_9100_4B85_8239_8F21614651C2__INCLUDED_)
#define AFX_PACKETMODERM_H__E2ABDB24_9100_4B85_8239_8F21614651C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketModerm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPacketModerm dialog

class CPacketModerm : public CDialog
{
// Construction
public:
	CPacketModerm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPacketModerm)
	enum { IDD = IDD_PACKET_DLG };
	CEdit	m_ctlModermFile;
	CEdit   m_ctlNVFile;
	CButton	m_ctlNVBrowse;
	CButton m_ctlModermBrowse;
	CString	m_strNVFilePath;
	CString m_strModermFileName;

	BOOL    m_bMerge;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketModerm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPacketModerm)
	virtual BOOL OnInitDialog();
	afx_msg void OnPacketnvBrowse();
	afx_msg void OnPacketmBrowse();
	virtual void OnOK();
	afx_msg void OnModermCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETMODERM_H__E2ABDB24_9100_4B85_8239_8F21614651C2__INCLUDED_)
