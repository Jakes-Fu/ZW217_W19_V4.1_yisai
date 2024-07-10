#if !defined(AFX_DLGLINEINGAIN_H__7C6B9726_471E_4E26_9F9D_18A2F4F266EF__INCLUDED_)
#define AFX_DLGLINEINGAIN_H__7C6B9726_471E_4E26_9F9D_18A2F4F266EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLineinGain.h : header file
//
#include "ListCtrlEx.h"
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgLineinGain dialog

class CDlgLineinGain : public CDialog
{
// Construction
public:
	CDlgLineinGain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLineinGain)
	enum { IDD = IDD_DIALOG_LINEIN_GAIN };
	CListCtrlEx m_lstVolume;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLineinGain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	void SetValue(VOLUME_STRUCT& volume_data);
	void FetchValue(VOLUME_STRUCT& volume_data);
	void SetTitle(CString &strTitle){m_strTitle = strTitle;}

private:
	void InitListCtrl();
	int CalcGOL ( double& gol, int volume );
	int CalcGOBL ( double& gobl, unsigned short volume );
	//teana hu 2012.06.12
	int CalcPA( double& pa, LONG volume );
	//
	
public:
	BOOL m_bDspCodecInfo;
	//teana hu 2012.06.11
	BOOL m_bSmartPhone;
	int m_nLstLevel;
	//
		
protected:
	typedef CListCtrlEx::COLUMN_DATA LIST_COLUME;
	typedef std::vector<LIST_COLUME>ROW_DATA;   
	typedef std::vector<ROW_DATA> LINE_DATA;
	LINE_DATA m_vAllLinesData;
	VOLUME_STRUCT m_volume_data;
	CString m_strTitle;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLineinGain)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINEINGAIN_H__7C6B9726_471E_4E26_9F9D_18A2F4F266EF__INCLUDED_)
