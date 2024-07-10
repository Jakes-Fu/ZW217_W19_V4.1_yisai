#if !defined(AFX_DLGSYNCPB_H__38DAC8C2_9069_4234_9F8F_7313BD268FC4__INCLUDED_)
#define AFX_DLGSYNCPB_H__38DAC8C2_9069_4234_9F8F_7313BD268FC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSyncPB.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgSyncPB dialog
struct _PB;
class IPB;
class CInfoWrapper;

class CDlgSyncPB : public CDialog
{
// Construction
public:
	CDlgSyncPB(CWnd* pParent = NULL);   // standard constructor

    void SyncPB();

	CTypedPtrArray<CPtrArray, _PB*>m_arrPBInPC;
	CTypedPtrArray<CPtrArray, _PB*>m_arrPBInMS;
	CTypedPtrArray<CPtrArray, _PB*>m_arrPBInRET;

// Dialog Data
	//{{AFX_DATA(CDlgSyncPB)
	enum { IDD = IDD_DLG_SYNC_PB };
	CButton	m_btnUp;
	CButton	m_btnDown;
	CListCtrl	m_lstRET;
	CListCtrl	m_lstPC;
	CListCtrl	m_lstMS;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSyncPB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitColomn();
	void SetListStyle(CListCtrl &lstCtrl);
	void FillList(CListCtrl &lstCtrl,_PB * pPB);
	void ModifyList(CListCtrl &lstCtrl,int nItem);
	void GetStorePlace(WORD wStore, CString &strStore);
	_PB* GetFocusItemData(BOOL bDown =TRUE);
	// Generated message map functions
	//{{AFX_MSG(CDlgSyncPB)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnUp();
	afx_msg void OnDblclkLstSyncRst(NMHDR* /*pNMHDR*/, LRESULT* pResult);
	afx_msg void OnClickLstSyncPc(NMHDR* /*pNMHDR*/, LRESULT* pResult);
	afx_msg void OnClickLstSyncMs(NMHDR* /*pNMHDR*/, LRESULT* pResult);
	afx_msg void OnClickLstSyncRst(NMHDR* /*pNMHDR*/, LRESULT* pResult);
	afx_msg void OnItemchangedLstSyncMs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedLstSyncPc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedLstSyncRst(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int  m_nUsedInPhone;
	int  m_nTotalInPhone;
	int  m_nUsedInSIM;
	int  m_nTotalInSIM;

	int  m_nOldStore;

	int  m_nFocusItemPC;
	int  m_nFocusItemMS;
	int  m_nFocusItemRET;
    
	_PB          * m_pPB;
	CInfoWrapper * m_pIW;
	IPB          * m_pPBFactory;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYNCPB_H__38DAC8C2_9069_4234_9F8F_7313BD268FC4__INCLUDED_)
