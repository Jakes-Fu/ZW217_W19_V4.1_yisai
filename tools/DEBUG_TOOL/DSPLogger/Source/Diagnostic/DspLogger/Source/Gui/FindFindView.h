#if !defined(AFX_CHARTFINDVIEW_H__601D0AF5_8462_4321_915E_E4A84234263F__INCLUDED_)
#define AFX_CHARTFINDVIEW_H__601D0AF5_8462_4321_915E_E4A84234263F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartFindView.h : header file
//
#include "MruComboBox.h"
#include "FindDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CFindFindView form view
class CFindDoc;
class CFindFindView : public CFormView
{
protected:
	CFindFindView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFindFindView)

// Form Data
public:
	//{{AFX_DATA(CFindFindView)
	enum { IDD = IDD_DLG_FIND };
	CListCtrl	m_lstCustomCtrl;
	CSpinButtonCtrl	m_spinCtrl;
    CListCtrl	m_lstInfo;
	CComboBox	m_cmbAddr;
	CComboBox   m_cmbData;
	CString	m_strData;
	CString	m_strAddr;
	CString	m_strSpace;
	CButton m_btnCheckBox;
	CString m_strLeftAddr;
	CString m_strRightAddr;
	CString m_strLeftData;
	CString m_strRightData;
	CButton m_btnCheckDesc;
	CString m_strEdtDesc;
	CComboBox m_cmbMatching;
	//}}AFX_DATA
    CMruComboBox m_cmbAddrVal;
// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindFindView)
public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFindFindView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFindFindView)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnItemchangedLstInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnFind();
	afx_msg void OnCustomDrawLstInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbData();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAuto();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDeltaposSpinSpace(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonExport();
	afx_msg void OnCheckDuality();
	afx_msg void OnCheckDesc();
	afx_msg void OnItemchangedLstCustomizeChart(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFindDoc* m_pDoc; 

	//polo.jiang 2013.01.06
	TCHAR	m_szCfgFile[MAX_PATH];
	
private:
	void InitCustomizeGraph();
	void InitInput();
	void GetItemInfo(int nItem, WORD& wAddrFlag, CString& strAddr, WORD& wDataFlag, CString& strData);  
	int  Item2FindDataIdx(int nIdx);

	int  AddListItem(CString strAddrFlag, CString strAddr, CString strDataFlag, CString strData, DWORD dwFlag = FLAG_GENERAL);

	void OnCheckStatus(int nItem , BOOL bCheck);
	void SetListItemColor();
	void UpdataTPCount();

	void LoadSettings();
	void SaveSettings();

    void AutoRefresh();
	void LoadSelectedFilter();
	void CustomGrapItemChanged(int nItem , BOOL bCheck);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTFINDVIEW_H__70FEEAE1_D7D9_4CDA_930C_118F3027A3F5__INCLUDED_)
