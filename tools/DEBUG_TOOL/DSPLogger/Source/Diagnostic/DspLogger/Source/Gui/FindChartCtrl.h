#if !defined(AFX_FINDCHARTCTRL_H__9B4425AC_CCAE_4D4E_AED1_1A09FD6D5FF8__INCLUDED_)
#define AFX_FINDCHARTCTRL_H__9B4425AC_CCAE_4D4E_AED1_1A09FD6D5FF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindChartCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindChartCtrl form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFindDoc;
class CFindChartCtrl : public CFormView
{
protected:
	CFindChartCtrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFindChartCtrl)

// Form Data
public:
	//{{AFX_DATA(CFindChartCtrl)
	enum { IDD = IDD_DLG_CHART_CTRL };
	CListCtrl	m_lstFindData;
	int		m_nUnsigned;
	int		m_nByTime;
	int		m_nCurveNum;
	BOOL	m_bDrawTP;
	BOOL	m_bDrawTogether;
	BOOL	m_bUseSameY;
	//}}AFX_DATA

// Attributes
public:
	void SetStatus(BOOL bEnable);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindChartCtrl)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFindChartCtrl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    void UpdateChartView( int nAct, LPARAM lParam, int nIdx );
	// Generated message map functions
	//{{AFX_MSG(CFindChartCtrl)
	afx_msg void OnItemchangedLstFind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdoUnsigned();
	afx_msg void OnRdoSigned();
	afx_msg void OnRdoBytime();
	afx_msg void OnRdoByidx();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusLstFind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckDrawtp();
	afx_msg void OnDblclkLstFind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckDrawtogether();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonChange();
	afx_msg void OnCheckUseSameY();
	//}}AFX_MSG

    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
        
private: 
	CFindDoc * m_pDoc;
    int        m_nSelect;
	//teana hu 2012.04.09
	int        m_nSelectBak;
	//

    BOOL       m_arrSigned[MAX_FIND_DATA_NUM];
    BOOL       m_arrByTime[MAX_FIND_DATA_NUM];
  
    int        m_arrYScale[MAX_FIND_DATA_NUM];
    
    WORD       m_arrMask[MAX_FIND_DATA_NUM];
    float      m_arrMult[MAX_FIND_DATA_NUM];

    static const int MAX_X_SCALE;
    static const int MAX_Y_SCALE;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDCHARTCTRL_H__9B4425AC_CCAE_4D4E_AED1_1A09FD6D5FF8__INCLUDED_)
