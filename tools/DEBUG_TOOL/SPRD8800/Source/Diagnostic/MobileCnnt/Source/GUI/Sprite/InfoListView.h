#if !defined(AFX_INFOLISTVIEW_H__AB4AC545_6D64_4359_B0F9_F63299DE1109__INCLUDED_)
#define AFX_INFOLISTVIEW_H__AB4AC545_6D64_4359_B0F9_F63299DE1109__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoListView.h : header file
//
#include "..\effect\CoolMenu.h"

#include "cderr.h"
#include "ProgThread.h"
#include "..\MainFrm.h"
#include "msgiddef.h"
#include "Imemo.h"

struct _SMS;
struct _PB;
struct _MEMO;
struct _SCHDL;
struct _PBGROUP;
/////////////////////////////////////////////////////////////////////////////
// CInfoListView view
class CInfoWrapper;  
class CSMSDoc;
class CInfoListView : public CListView
{
protected:
	CInfoListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInfoListView)

// Attributes
public:
   CSMSDoc * GetDocument();
// Operations
public:
    void OnNew();
	void OnCommand(UINT uID);
	BOOL IsItemSelected()
	{
		CListCtrl &lst = GetListCtrl();//lint !e1514

		return lst.GetSelectedCount() != 0;
	}

	BOOL IsSingleItemSelected()
	{
		CListCtrl &lst = GetListCtrl();//lint !e1514

		return lst.GetSelectedCount() == 1;
	}

	int GetItemCount()
	{
		CListCtrl &lst = GetListCtrl();//lint !e1514

		return lst.GetItemCount();
	}
	void SetSaveToFileFlag(){m_bSaveToFile = TRUE;}
	void ClearSaveToFileFlag(){m_bSaveToFile = FALSE;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* /*pDC*/);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInfoListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
   
	void NewSms();
	void EditSms(_SMS * psms);
	BOOL DeleteSms(_SMS * psms);

	void NewPb();
	void EditPb(_PB * pPB);
	BOOL DeletePb(_PB * pPB);
	int  ComparePB(_PB * pPBSrc,_PB * pPBDst);
	_PB *CombinePB(_PB * pPB1,_PB *pPB2,_PB * pPBDst);
    enum {
		TOTAL_SAME   = 0, //完全相同
		REPLACE_SAME = 1, //名字相同，内容互补
		REPLACE_DIFF = 2, //名字相同，内容冲突
		TOTAL_DIFF   = 3, //名字不同		
	};
	void EditPBG(_PBGROUP * pPBG);

	void NewMemo();
	void EditMemo(_MEMO * pMemo);
	BOOL DeleteMemo(_MEMO * pMemo);
	int  CompareMemo(_MEMO * pMemoSrc,_MEMO * pMemoDst);

	void OnEdit();
	void OnCopy();
	void OnDelete();
	void OnSelectAll();
	// Generated message map functions
protected:
	
	void GetSMSStatus( WORD wStatus, CString &strStatus );
	void GetStorePlace( WORD wStore, CString &strStore );
	
	void InserSchdl(WORD wStore);
	void InsertSchdl(_SCHDL * pSchdl, BOOL bNew = TRUE, int nItem = -1);

	void UpdatePBG(WORD wStore);
	void InsertPBG(WORD wStore);
	void InsertPBG(_PBGROUP *pPBG,BOOL bNew = TRUE, int nItem = -1);
	void UpdatePBGM(WORD wStore);
	void InsertPbgMember(WORD wStore);
	void InsertPbgMember(_PB *pPB, BOOL bNew = TRUE, int nItem = -1);

	void ExportVCard(LPCTSTR pstrDirName);
	void ImportVCard(LPCTSTR pstrDirName);
	
	void UpdateMemo(WORD wStore);
	void InsertMemo(WORD wStore);
	void InsertMemo(_MEMO * pMemo, BOOL bNew = TRUE, int nItem = -1);
	void ExportVCalendar(LPCTSTR pstrDirName);
	void ImportVCalendar(LPCTSTR pstrDirName);
    BOOL ChangeMemoStorePlace( WORD wStore);

	void UpdatePB(WORD wStore);
	void InsertPB(WORD wStore);
	void InsertPB(_PB * pPB, BOOL bNew = TRUE, int nItem = -1);
	BOOL ChangePBStorePlace( WORD wStore);
	BOOL ChangePBGroup( WORD wGroupID);
	
	void UpdateSMS(WORD wStore);
	void InsertSMS(WORD wStore);
	void InsertInboxSMS(WORD wStore);
	void InsertOutboxSMS(WORD wStore);
	void InsertSendsSMS(WORD wStore);
	void InsertDraftsSMS(WORD wStore);
	void InsertSMS(int nItem, _SMS * psms);

	void PreTrackPopopMenu( CMenu * pMenu, int nItem );
	void UpdateColumn(DWORD dwFlag);
	void UpdateView( DWORD dwFlag );
	void SetHeader( DWORD dwFlag );

	void DropItemOnTree(CTreeCtrl* pDropTree);
	void ChangeStorePlace(WORD wStore);

	DWORD GetFocusItemData(void);
	//{{AFX_MSG(CInfoListView)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnIlmSmsAnswer();
	afx_msg void OnIlmSmsForward();
	afx_msg void OnIlmSmsSend();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExportToVcard();
	afx_msg void OnUpdateExportToVcard(CCmdUI* /*pCmdUI*/);
	afx_msg void OnSyncPb();
	afx_msg void OnExportToVcalendar();
	afx_msg void OnImportVcalendar();
	afx_msg void OnImportVcard();
	afx_msg void OnExtractNumber();
	afx_msg void OnSyncMemo();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCopyMoveToPc();
	afx_msg void OnUpdateCopyMoveToPc(CCmdUI* pCmdUI);
	afx_msg void OnCopyMoveToMs();
	afx_msg void OnUpdateCopyMoveToMs(CCmdUI* pCmdUI);
	afx_msg void OnCopyMoveToSim();
	afx_msg void OnUpdateCopyMoveToSim(CCmdUI* pCmdUI);
	afx_msg void OnCopyMoveToSim2();
	afx_msg void OnUpdateCopyMoveToSim2(CCmdUI* pCmdUI);
	afx_msg void OnCopyMoveToSim3();
	afx_msg void OnUpdateCopyMoveToSim3(CCmdUI* pCmdUI);
	afx_msg void OnCopyMoveToSim4();
	afx_msg void OnUpdateCopyMoveToSim4(CCmdUI* pCmdUI);
	afx_msg void OnOptionCallFromPB();
	afx_msg void OnOptionSendSMSFromPB();
	//}}AFX_MSG
	afx_msg void OnGroupChange(UINT nID);
	DECLARE_MESSAGE_MAP()
private:
    int            m_nNoNameID;
	int            m_nFocusItem;
	CCoolMenu      m_CoolMenu;
	CInfoWrapper * m_pIW;
	IMemo        * m_pMemo;//lint !e1401
	DWORD          m_dwCategID;	

    CWinThread * m_pProgThread;

	CImageList* m_pDragImage;
	DWORD       m_dwDragCategID;
	BOOL		m_bDragging;
	int			m_nDragIndex;
	int     	m_nDropIndex;
	HTREEITEM	m_hDropItem;
	CWnd*		m_pDropWnd;
	CWnd*       m_pTreeView;

	BOOL        m_bFromMenu;
	
	BOOL        m_bSaveToFile;

	BOOL        m_bFocusItemLock;

	static const int MAX_CONTENT_LEN;
	static const int DEL_PB_INTERVAL;
	static const int DEL_SMS_INTERVAL;
	static const int DEL_MEMO_INTERVAL;

	CString m_strPhoneNum;
};

#ifndef _DEBUG  // debug version in MobileSpriteView.cpp
inline CSMSDoc* CInfoListView::GetDocument()
   { return (CSMSDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOLISTVIEW_H__AB4AC545_6D64_4359_B0F9_F63299DE1109__INCLUDED_)
