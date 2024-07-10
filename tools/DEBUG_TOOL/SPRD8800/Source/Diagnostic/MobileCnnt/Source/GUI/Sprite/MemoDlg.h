#if !defined(AFX_MEMODLG_H__23D7A3F5_42FD_4BCB_A831_9D79A970BD8C__INCLUDED_)
#define AFX_MEMODLG_H__23D7A3F5_42FD_4BCB_A831_9D79A970BD8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoDlg.h : header file
//

#include "GlobalDef.h"
#include "..\effect\CoolEdit.h"

struct _DATE_TIME_T;
struct _MEMO;
class CInfoWrapper;
class IMemo;


#define CLR_DLGBCKGRND RGB(232,232,232)
/////////////////////////////////////////////////////////////////////////////
// CMemoDlg dialog
class CMemoDlg : public CDialog
{
// Construction
public:
	_MEMO * m_pMemo;
	BOOL    m_bNew;

	CMemoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMemoDlg)
	enum { IDD = IDD_DLG_MEMO };
	CComboBox	m_cmbMode;
	CButton	    m_btnOK;
	CButton	    m_btnCancel;
	CStatic		m_stcPlace;
	CDateTimeCtrl	m_dtpTime;
	CDateTimeCtrl	m_dtpDate;
	CDateTimeCtrl	m_dtpDateEnd;
	CString	        m_strContent;
	int		        m_nStore;
	CButton      m_btnSun;
	CButton      m_btnMon;
	CButton      m_btnTue;
	CButton      m_btnWed;
	CButton      m_btnThu;
	CButton      m_btnFri;
	CButton      m_btnSat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCtrls(void);	
    void SetSMSPlace( WORD wStore );

	// Generated message map functions
	//{{AFX_MSG(CMemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnVscrollEdtContent();
	afx_msg void OnUpdateEdtContent();
	virtual void OnOK();
	afx_msg void OnRdoPc();
	afx_msg void OnRdoPhone();	
	afx_msg void OnSelchangeCmbMode();
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
private:
	// Store index to store ID
	__inline int GetStoreIDFromIdx(int nIdx);
	// Store ID to store index
	__inline int GetStoreIdxFromID(int nID);

	void  SetModeFre(int Mode,BYTE fre);
	BYTE  GetMemoFre(int Mode);

private:
	int  m_nUsedInPhone;
	int  m_nTotalInPhone;
	int            m_nOldStore;
	CInfoWrapper * m_pIW;
	IMemo        * m_pMemoFactory;

	CCoolEdit      m_edtContent;

	static const int m_nMaxContentLen;

	static const int MIN_YEAR;
	static const int MAX_YEAR;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMODLG_H__23D7A3F5_42FD_4BCB_A831_9D79A970BD8C__INCLUDED_)
