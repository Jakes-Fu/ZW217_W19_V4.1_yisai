#if !defined(AFX_FINDDOC_H__AADFBA01_3ACC_4204_98F8_AE38BF068C8F__INCLUDED_)
#define AFX_FINDDOC_H__AADFBA01_3ACC_4204_98F8_AE38BF068C8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindDoc.h : header file
//
#include "DspKernelExport.h"

#define WM_UPDATE_FIND         (WM_USER+850)
#define WM_UPDATE_CHART        (WM_USER+851)
#define WM_UPDATE_CLEAR        (WM_USER+852)
#define WM_GOTO_ITEM_BY_FIND   (WM_USER+853)
#define WM_GOTO_ITEM_BY_CHART  (WM_USER+854)
#define WM_SELECT_CHART 	   (WM_USER+855)
#define WM_SELECT_CROSS 	   (WM_USER+856)
#define WM_AUTO_REFRESH        (WM_USER+857)
#define WM_SAVE_CURRENT_TP 	   (WM_USER+858)
#define WM_UPDATE_CLEAR_STATE  (WM_USER+859)
#define WM_FIND_ITEM_IN_DOCVIEW (WM_USER+860)
#define WM_SELECT_ITEM_IN_DOCVIEW (WM_USER+861)
#define WM_UPDATE_REFIND	(WM_USER+862)

#define WM_AUTO_LOAD_FILTER         (WM_USER+863)

#define  DRAW_MAX_POINTS		(2*10000)

typedef enum
{
    UCA_X_SCALE = 0,
    UCA_Y_SCALE,
    UCA_UNSIGNED,
    UCA_SIGNED,
    UCA_BYTIME,
    UCA_BYINDEX,

	UCA_DRAWTP,
	UCA_DRAWTOGETHER,
	UCA_RESET,
	UCA_DRAWONSAMEY,
	UCA_CHANGE_CURVE_NUM,
} UC_ACTION;

typedef struct _UC_HINT
{
    UC_ACTION eAct;
    WPARAM    wParam;
    LPARAM    lParam;
    int       nIdx;
} UC_HINT, * PUC_HINT;

typedef enum
{
	FLAG_GENERAL,
	FLAG_DUALITY,
	FLAG_DESCRIPTION
};

/////////////////////////////////////////////////////////////////////////////
// CFindDoc document

class CFindDoc : public CDocument
{
protected:
	void	LoadSetting();
	CFindDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFindDoc)

// Attributes
public:

    TP_FIND_DATA m_arrFindData [MAX_FIND_DATA_NUM];
    DWORD        m_arrFindColor[MAX_FIND_DATA_NUM];
    int          m_nFindDataNum;
	int			 m_nCurveNum;
	BOOL		 m_bAutoRefresh;
	DWORD		 m_dwDrawMaxPoints;
// Operations
public:
	BOOL   AddFindData      (WORD wAddrFlag,CString strAddr,WORD wDataFlag, CString strData,DWORD dwFlag = FLAG_GENERAL,CHAR* pszTpAlias=NULL);
	
	BOOL   DelFindData      (WORD wAddrFlag,CString strAddr,WORD wDataFlag, CString strData,CHAR* pszTpAlias=NULL);

	int    GetFindDataIdx (WORD wAddrFlag,CString strAddr,WORD wDataFlag, CString strData);

	void   SetCurveNum(int nCurveNum);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFindDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFindDoc)
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDDOC_H__AADFBA01_3ACC_4204_98F8_AE38BF068C8F__INCLUDED_)
