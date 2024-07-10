#if !defined(AFX_DLGARMVOLUME_H__24A09BE0_1165_4129_938B_1496A27B911C__INCLUDED_)
#define AFX_DLGARMVOLUME_H__24A09BE0_1165_4129_938B_1496A27B911C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgArmVolume.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgArmVolume dialog
class SpecialRectOfArmVolume;
class CDlgArmVolume : public CDialog
{
// Construction
public:
	CDlgArmVolume(CWnd* pParent = NULL);   // standard constructor
	SpecialRectOfArmVolume* m_pArea;

// Dialog Data
	//{{AFX_DATA(CDlgArmVolume)
	enum { IDD = IDD_DIALOG_ARM_VOLUME };
	CListCtrlEx m_lstVolume;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgArmVolume)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
		void InitListCtrl();
		int CalcVdg ( double& vdg, int volume );
		int CalcP2 ( double& p2, unsigned short volume );
		int CalcP1 ( double& p1, unsigned short volume );
public:



protected:
	typedef CListCtrlEx::COLUMN_DATA LIST_COLUME;
    typedef std::vector<LIST_COLUME>ROW_DATA;   
    typedef std::vector<ROW_DATA> LINE_DATA;
    LINE_DATA m_vAllLinesData;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgArmVolume)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGARMVOLUME_H__24A09BE0_1165_4129_938B_1496A27B911C__INCLUDED_)
