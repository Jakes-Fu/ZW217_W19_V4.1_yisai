#if !defined(AFX_CONSDIAGSTC_H__392223DE_DCC5_4D58_9AE7_7D21F125CD8B__INCLUDED_)
#define AFX_CONSDIAGSTC_H__392223DE_DCC5_4D58_9AE7_7D21F125CD8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConsDiagStc.h : header file
//
#define CONST_DIAG_NUM 1408
/////////////////////////////////////////////////////////////////////////////
// CConsDiagStc window

class CConsDiagStc : public CStatic
{
// Construction
public:
	CConsDiagStc();

// Attributes
public:

// Operations
public:
	void AddData(WORD wData, BOOL bClear);
	void Clear();

private:
	void DrawAxis(CDC *pDC, CRect rect);
	void DrawChart(CDC *pDC, CRect rect);

private:
	int m_wData[CONST_DIAG_NUM];
	int m_nIndex;
	int m_nMaxValue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsDiagStc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConsDiagStc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConsDiagStc)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSDIAGSTC_H__392223DE_DCC5_4D58_9AE7_7D21F125CD8B__INCLUDED_)
