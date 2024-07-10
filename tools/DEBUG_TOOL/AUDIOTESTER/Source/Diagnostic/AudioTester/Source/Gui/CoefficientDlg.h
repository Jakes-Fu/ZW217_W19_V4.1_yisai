#if !defined(AFX_COEFFICIENTDLG_H__08CBCA3F_564B_424E_8A6B_1BEBDDA899C7__INCLUDED_)
#define AFX_COEFFICIENTDLG_H__08CBCA3F_564B_424E_8A6B_1BEBDDA899C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoefficientDlg.h : header file
//
#pragma warning( disable: 4786 ) 
class SpecialRectOfCoefficient;
#include "ListCtrlEx.h"
#include "FrequencyGraphyStatic.h"
#include "BMPCtrlWithSpecRect.h"
/////////////////////////////////////////////////////////////////////////////
// CCoefficientDlg dialog
 
class CCoefficientDlg : public CDialog
{    
// Construction
public:
	CCoefficientDlg(CWnd* pParent = NULL);   // standard constructor
    ~CCoefficientDlg();

    int m_nSampleRate;


// Dialog Data
	//{{AFX_DATA(CCoefficientDlg)
	enum { IDD = IDD_DIALOG_MODIFY_COEFFICIENT };
	CStatic	m_staticDescript;
	CEdit	m_editPowerAddValue;
	CStatic	m_staticToolBarPos;
	CEdit	m_editCoefficient;
	CFrequencyGraphyStatic	m_staticGraphy;
	CListCtrlEx	m_listPo;
	CString	m_strCoefficient;
	BOOL	m_bWriteImmediately;
	double	m_dAddValue;
	double	m_dDlEqFirGain;
	//}}AFX_DATA

    SpecialRectOfCoefficient* m_pArea;
    SpecialRectOfCoefficient* m_pCoefficientAreaTemp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoefficientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCoefficientDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL ShowToolTips(UINT id,NMHDR* pTTTStruct,LRESULT* pResult );
	afx_msg void OnButtonPowerAddValue();
	afx_msg void OnItemchangedListPoValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAutoAdjust();
	afx_msg void OnButtonUpdatePo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitListData();
	void SetListCtrl();
    void LoadListData(int iSelect= -1);
    static int m_vPowerFrequence[ROW_COUNT];
private:	    
	void InitToolBar();
	void ChangeFont();
	void RefreshCoefficientValue( BOOL bFirstTime = FALSE );
    typedef CListCtrlEx::COLUMN_DATA LIST_COLUME;
    typedef std::vector<LIST_COLUME>ROW_DATA;   
    typedef std::vector<ROW_DATA> LINE_DATA;
    LINE_DATA m_vAllLinesData;
    CToolBar m_toolBarPower;    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COEFFICIENTDLG_H__08CBCA3F_564B_424E_8A6B_1BEBDDA899C7__INCLUDED_)

