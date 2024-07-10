#if !defined(AFX_FILTERDOC_H__DD93DEE8_0BE3_4C62_96B5_E0F6414727FB__INCLUDED_)
#define AFX_FILTERDOC_H__DD93DEE8_0BE3_4C62_96B5_E0F6414727FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilterChartView.h"
#include "FilterChartCtrl.h"
#include "Filter.h"
#include "vector"
using namespace std;

#define MAX_GRAPH_NUM 4

/////////////////////////////////////////////////////////////////////////////
class CFilterDoc : public CDocument
{
protected:
	CFilterDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFilterDoc)

// Attributes
public:
    CFilterChartView* m_pChartView;
    CFilterChartCtrl* m_pChartCtrl;
   
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFilterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDOC_H__DD93DEE8_0BE3_4C62_96B5_E0F6414727FB__INCLUDED_)
