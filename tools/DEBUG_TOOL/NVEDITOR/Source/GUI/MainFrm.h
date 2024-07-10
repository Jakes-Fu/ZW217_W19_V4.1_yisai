// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__7CA05671_6CCB_4203_9058_4BDFBEF887DC__INCLUDED_)
#define AFX_MAINFRM_H__7CA05671_6CCB_4203_9058_4BDFBEF887DC__INCLUDED_

#include "DlgBMDloader.h"
#include <map>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CustView.h"
#include "NVEditorView.h"
#include "MyHSplitterWnd.h"

#define STRING_PASS      _T("Passed")
#define STRING_FAILED    _T("Failed")
#define STRING_WAIT      _T("Waiting...")
#define STRING_DOING     _T("In progress")
#define STRING_BADPORT   _T(" X ")
#define STRING_UNPLUGGED _T("Unplugged")

#define STEP_INDEX				1
#define STATUS_INDEX			2
#define PROG_INDEX				3 
#define TIME_INDEX              4  
#define MCPTYPE_INDEX           5
#define MAX_BUF_SIZE            8192

typedef enum DL_STAGE_ENUM
{
    DL_NONE_STAGE,
	DL_CHK_BAUD,
	DL_CONNECT,
	DL_ERASE_FLASH,
	DL_DL_STAGE,
	DL_READ_STAGE,
	DL_RESET_STAGE,
	DL_READCHIPTYPE_STAGE,
	DL_READNVITEM_STAGE,
	DL_CHANGEBUAD_STAGE,
	DL_FINISH_STAGE,
	DL_UNPLUGGED_STAGE,
	DL_PAUSED
}DL_STAGE;

#define  DL_NO_FILE            0
#define  DL_FIRST_FILE         1

struct TSYNCCALIINFO
{
    TCHAR   szCaliName[50];
	WORD    wCaliID;
};


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CDlgBMDloader   m_dlgBMLoader;
	int				m_nPort;
	CStringArray    m_StepDescription;
	int             m_nStage;
	int             m_nFileStep;
	int				m_nSave2PhoneMode;//0:disable 1:calibration mode 2: normal mode
	std::map<WORD,CString> m_mapSyncCali;
	CToolBar    m_wndToolBar;
	CMyHSplitterWnd m_SplitterH;
	BOOL		m_bDisplayHex;
	BOOL		m_bSplitted;

// Operations
public:
	void ShowCustPane(BOOL bShow = FALSE);
	void MinSplitPane(int nColPane);
    CNVEditorView*  GetEditorView();
	CCustView*		GetCustView();
    BOOL SetStatus(int nPort,DL_STAGE stage,BOOL bNeedProg,
                int nMin = 0,int nMax = 0);
    BOOL GetStatus(int nPort,DL_STAGE& stage);
    /*nFlag = 1 表示lpszErrmsg有效
	 *nFlag = 0 表示bSuccess有效
	 */
    BOOL SetResult(int nPort,BOOL bSuccess, LPCTSTR lpszErrMsg = NULL,int nFlag = 1 ); 
	 
    BOOL SetStep( int nPort );
        
    void ResetStatus( int nPort );
    
    BOOL StartProg( int nPort,int nMin = 0,int nMax = 100 );
    BOOL StepProg( int nPort,int nStep = 1 );
    BOOL EndProg( int nPort );
    BOOL SetProgStep( int nPort,int nStep );
    BOOL SetProgPos( int nPort,int nPos );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDisplayHex();
	afx_msg void OnUpdateDisplayHex(CCmdUI* pCmdUI);
	afx_msg void OnOpenProject();
	afx_msg void OnUpdateOpenProject(CCmdUI* pCmdUI);
	afx_msg void OnSaveProject();
	afx_msg void OnUpdateSaveProject(CCmdUI* pCmdUI);
	afx_msg void OnSaveProjectAs();
	afx_msg void OnUpdateSaveProjectAs(CCmdUI* pCmdUI);
	afx_msg void OnSaveToPhone();
	afx_msg void OnUpdateSaveToPhone(CCmdUI* pCmdUI);
	afx_msg void OnBtnLoadCustomernv();
	afx_msg void OnUpdateBtnLoadCustomernv(CCmdUI* pCmdUI);
	afx_msg void OnCloseProject();
	afx_msg void OnUpdateCloseProject(CCmdUI* pCmdUI);
	afx_msg void OnPortSettings();
	afx_msg void OnLoadImage();
	afx_msg void OnUpdateLoadImage(CCmdUI* pCmdUI);
	afx_msg void OnLoadFromPhoneBoot();
	afx_msg void OnUpdateLoadFromPhoneBoot(CCmdUI* pCmdUI);
	afx_msg void OnLoadFromPhone();
	afx_msg void OnUpdateLoadFromPhone(CCmdUI* pCmdUI);
	afx_msg void OnSaveImage();
	afx_msg void OnUpdateSaveImage(CCmdUI* pCmdUI);
	afx_msg void OnSaveToPhoneBoot();
	afx_msg void OnUpdateSaveToPhoneBoot(CCmdUI* pCmdUI);
	afx_msg void OnFailNvinfo();
	afx_msg void OnUpdateFailNvinfo(CCmdUI* pCmdUI);
	afx_msg void OnReadFlash();
	afx_msg void OnFlashMemory();
	afx_msg void OnLoadNvvalue();
	afx_msg void OnUpdateLoadNvvalue(CCmdUI* pCmdUI);
	afx_msg void OnImportItem();
	afx_msg void OnUpdateImportItem(CCmdUI* pCmdUI);
	afx_msg void OnExportStatic();
	afx_msg void OnUpdateExportStatic(CCmdUI* pCmdUI);
	afx_msg void OnCalibrationNv();
	afx_msg void OnUpdateCalibrationNv(CCmdUI* pCmdUI);
	afx_msg void OnMergeNv();
	afx_msg void OnUpdateMergeNv(CCmdUI* pCmdUI);
	afx_msg void OnPacketModerm();
	afx_msg void OnUpdatePacketModerm(CCmdUI* pCmdUI);
	afx_msg void OnMenuFuncitions();
	afx_msg void OnUpdateMenuFuncitions(CCmdUI* pCmdUI);
	afx_msg void OnMenuFind();
	afx_msg void OnUpdateMenuFind(CCmdUI* pCmdUI);
	afx_msg void OnMenuFindNext();
	afx_msg void OnUpdateMenuFindNext(CCmdUI* pCmdUI);
	afx_msg void OnGenerStatistical();
	afx_msg void OnUpdateGenerStatistical(CCmdUI* pCmdUI);
	afx_msg void OnShowModify();
	afx_msg void OnUpdateShowModify(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnBMCheckBaud( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMConnect( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMEraseFlash( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMDownLoad( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMDLoadProcess( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadFlash( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadFlashProcess( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReset( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadChipType( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadNVItem( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMChangeBuad( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMBegin( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMFileBegin( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMEnd( WPARAM wParam = 0, LPARAM lParam = 0 ); 
	DECLARE_MESSAGE_MAP()
private:
	void InitSyncCaliMap();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7CA05671_6CCB_4203_9058_4BDFBEF887DC__INCLUDED_)
