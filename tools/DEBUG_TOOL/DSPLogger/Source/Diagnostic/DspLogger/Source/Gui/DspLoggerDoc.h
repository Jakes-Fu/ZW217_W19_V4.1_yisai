// DspLoggerDoc.h : interface of the CDspLoggerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPLOGGERDOC_H__743CF375_BA65_4CEB_910D_7E5B18F81317__INCLUDED_)
#define AFX_DSPLOGGERDOC_H__743CF375_BA65_4CEB_910D_7E5B18F81317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DspKernelExport.h"

class CDspLoggerDoc : public CDocument
{
protected: // create from serialization only
	CDspLoggerDoc();
	DECLARE_DYNCREATE(CDspLoggerDoc)

// Attributes
public:

    TP_FIND_DATA m_arrFindData[MAX_FIND_DATA_NUM];
    int          m_nFindDataNum;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspLoggerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDspLoggerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDspLoggerDoc)
	afx_msg void OnFindTp();
	afx_msg void OnFilterTp();
	afx_msg void OnKeyinfo();
	afx_msg void OnUpdateKeyinfo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPLOGGERDOC_H__743CF375_BA65_4CEB_910D_7E5B18F81317__INCLUDED_)
