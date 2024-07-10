#if !defined(AFX_SMSDOC_H__D30D13D5_D6A1_4262_8C68_27CED8468B6B__INCLUDED_)
#define AFX_SMSDOC_H__D30D13D5_D6A1_4262_8C68_27CED8468B6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSDoc.h : header file
//

#include "ISMS.H"
#include "SMSFrame.h"
/////////////////////////////////////////////////////////////////////////////
// CSMSDoc document

class CSMSDoc : public CDocument
{
protected:
	CSMSDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSMSDoc)

// Attributes


public:
	public:
    CWinThread * m_pProgThread;
	DWORD m_dwCateg;
	BOOL m_bLoadAll;
	BOOL m_bLoadPB;
	BOOL m_bSync;
	CMap<DWORD,DWORD,CWinThread *,CWinThread *>m_mapLoadThread;
	

	//CCriticalSection m_cs;
	CWinThread * m_pCurLoadThread;
	BOOL m_bLoadFinish;

	
// Operations
public:

	void PresetSMS(void);
	BOOL LoadSplishSMS(PSMS_INFO pFirstSI,int *pFlag,int nCount);
	void SetSyncFlag(BOOL bSync){ m_bSync =  bSync;	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void UpdateFrameCounts();
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSMSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CString GetPresetSMSFileName(void);
	//{{AFX_MSG(CSMSDoc)
	afx_msg void OnEditNew(UINT nID);
	afx_msg void OnOptionSmsCenter();
	afx_msg void OnOptionSetting();
	afx_msg void OnOptionSmsPreset();
	afx_msg void OnEditEdit();
	afx_msg void OnOptionLoadMSAll();
	afx_msg void OnOptionLoadSms();
	afx_msg void OnOptionLoadPb();
	afx_msg void OnOptionLoadMemo();
	afx_msg void OnUpdateEditEdit(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnEditNew();
	afx_msg void OnUpdateOptionLoadMSAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionLoadMemo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionLoadPb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionLoadSms(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionSmsCenter(CCmdUI* pCmdUI);
	afx_msg void OnOptionLoadPBG();
	afx_msg void OnUpdateOptionLoadPBG(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditNew(CCmdUI* pCmdUI);
	afx_msg void OnOptionSmsSendMode();
	afx_msg void OnUpdateOptionSmsSendMode(CCmdUI* pCmdUI);
	afx_msg void OnOptionSmsStore();
	afx_msg void OnUpdateOptionSmsStore(CCmdUI* pCmdUI);
	afx_msg void OnOptionPBType();
	afx_msg void OnUpdateOptionPBType(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	__inline CView * GetInfoListView(void);

private:

	BOOL m_bReadAccessory;

	
	

	static const _TCHAR PRESETSMS_FILENAME[];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSDOC_H__D30D13D5_D6A1_4262_8C68_27CED8468B6B__INCLUDED_)
