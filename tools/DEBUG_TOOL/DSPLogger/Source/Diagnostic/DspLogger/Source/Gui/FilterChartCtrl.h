#if !defined(AFX_FILTERCHARTCTRL_H__414A972E_B8FD_4DF2_9B46_7DC6F90E8939__INCLUDED_)
#define AFX_FILTERCHARTCTRL_H__414A972E_B8FD_4DF2_9B46_7DC6F90E8939__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterChartCtrl.h : header file
//

#include "vector"
using namespace std;


typedef vector<string> VEC_STR;
typedef vector<int>    VEC_INT;
/////////////////////////////////////////////////////////////////////////////
// CFilterChartCtrl form view
class CFilterChartView;
class CFilterDoc;
class CFilterChartCtrl : public CFormView
{
protected:
	CFilterChartCtrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFilterChartCtrl)

// Form Data
public:
	//{{AFX_DATA(CFilterChartCtrl)
	enum { IDD = IDD_DIALOG_FILTERCTRL };
	CListCtrl	m_lstFml;
	int		m_screen_x;
	int		m_screen_y;
	int		m_nUnsiged;
	int		m_nBytime;
	//}}AFX_DATA



public:
    CFilterDoc * m_pDoc;

private:
    void DoFit(UINT fitType);

    void EnableAxisCtrl(BOOL bEnable);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterChartCtrl)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFilterChartCtrl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFilterChartCtrl)
	afx_msg void OnButtonClear();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonHide();
	afx_msg void OnItemchangedListMathfml(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFitpage();
	afx_msg void OnButtonFitheight();
	afx_msg void OnButtonWidth();
	afx_msg void OnRadioPoint();
	afx_msg void OnRadioDrag();
	afx_msg void OnRadioZoom();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonExport();
	afx_msg void OnCheckAutoscroll();
	afx_msg void OnRdoUnsigned();
	afx_msg void OnRdoSigned();
	afx_msg void OnRdoBytime();
	afx_msg void OnRdoByidx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    VEC_STR m_vecFmlStr;
    VEC_INT m_vecGphIdx;


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERCHARTCTRL_H__414A972E_B8FD_4DF2_9B46_7DC6F90E8939__INCLUDED_)
