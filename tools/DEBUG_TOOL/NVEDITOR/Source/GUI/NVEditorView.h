// NVEditorView.h : interface of the CNVEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NVEDITORVIEW_H__25F7D2B2_AAF9_4BF2_9074_06F582C9D517__INCLUDED_)
#define AFX_NVEDITORVIEW_H__25F7D2B2_AAF9_4BF2_9074_06F582C9D517__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySuperGrid.h"

//@# Kai Liu 2003.2.20
// [[
#include "DLSettingsDlg.h"
// ]]

// @ZHANG Ming : 2003-04-16
// [[
//#include "ChanMan.h"
#include "LFPFeedBack.h"
//#include "PortSettingsDlg.h"
// ]]
#include "NVValueFile.h"
#include "BootModeitf.h"
#include "DlgFindItem.h"
#include "DiagObj.h"
#include "NVEditorDoc.h"

#pragma warning(push,3)
#include <vector>
#pragma warning(pop)

#include "PortSettingsDlg.h"/*CR212808 xmzhang add*/
struct BMFileInfo_TAG;
class CNVEditorView;
const TCHAR g_szCustNVName[50]		= _T("customer.nv");
const TCHAR g_szCustBaseFile[50]	= _T("custbase.db");
class CBMOprObserver: public IBMOprObserver
{
public:
	CBMOprObserver(){ pThis = NULL;}
	virtual HRESULT OnStart( DWORD dwOprCookie, 
		DWORD dwResult ); 
	virtual HRESULT OnEnd ( DWORD dwOprCookie, 
		DWORD dwResult );         
	virtual HRESULT OnOperationStart(   DWORD dwOprCookie, 
		LPCWSTR cbstrFileID,
		LPCWSTR cbstrFileType,		
		LPCWSTR cbstrOperationType,
		LPVOID pBMFileInterface );      
	virtual HRESULT OnOperationEnd(  DWORD dwOprCookie, 
		LPCWSTR cbstrFileID,
		LPCWSTR cbstrFileType, 
		LPCWSTR cbstrOperationType, 
		DWORD dwResult,
		LPVOID pBMFileInterface );      
	virtual HRESULT OnFileOprStart( DWORD dwOprCookie,
		LPCWSTR cbstrFileID, 
		LPCWSTR cbstrFileType , 
		LPVOID pBMFileInterface );    
	virtual HRESULT OnFileOprEnd(  DWORD dwOprCookie, 
		LPCTSTR cbstrFileID,
		LPCTSTR cbstrFileType, 
		DWORD dwResult );	
	virtual HRESULT OnFilePrepare(   DWORD dwOprCookie,
		LPCWSTR bstrProduct,
		LPCWSTR bstrFileName,
		LPVOID  lpFileInfo,
		LPVOID   pBMFileInfoArr,
		LPDWORD  lpBMFileInfoCount,
		LPDWORD  lpdwFlag);    
public:
	CNVEditorView* pThis;
};


class CNVEditorView : public CView
{
protected: // create from serialization only
	CNVEditorView();
	DECLARE_DYNCREATE(CNVEditorView)

// Attributes
public:
	CNVEditorDoc* GetDocument();
    CMySuperGrid  m_List;
    CFileParser *m_pProject;
	BOOL  m_bDisplayHex;

	std::vector<FUNC_ITEM_PTR> m_vFuncs;

	BOOL IsDispHex() const { return m_bDisplayHex; }


// Operations
public:
    BOOL SaveAndCloseProject(BOOL bCanCancel = TRUE);
	void  ClearFuncs();

protected:
    // Must give full path name
    BOOL OpenProject(const TCHAR* szPrjFileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNVEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNVEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
    //@# Kai Liu 2003.2.20
    // [[
    CDLSettingsDlg m_DLSetDlg;
    void SaveToPhone(int nPort,
		             int nBaudRate,
					 CString strFDL,
					 DWORD,
					 CString strNV,
					 NvBaseAddr dwNVBase,
					 BOOL  bPresCaliParm = FALSE,
                     BOOL  bOldReplaceNew = FALSE
                    );

//    BOOL LoadFromPhone(int nPort,int nBuadRate,int* pItemIds,int nCount);
    //void ProcessData(BYTE* pRcvData,int nDecodeLen);
    // ]]

    //@# Kai Liu 2003.4.21
    // [[
    /*CR212808 xmzhang modify begin*/
    BOOL FlashMemory(UINT uPort,UINT uBaudRate,CString strFDLFile,NvBaseAddr stAddress,DWORD dwSize,CString strFdl2 = "");
    /*CR212808 xmzhang modify end*/
    // ]]


    void LFPClearUp( void );
    BOOL WriteAllNVItems( void );

	BOOL PortSettings( BOOL bCanNoFdl );
        /*CR212808 xmzhang modify begin*/
	BOOL GetFlashContent( 
		int nPort,
		int nBaudrate,
		const CString& strFdl,
		NvBaseAddr dwBase,
		DWORD dwSize,
        CString strFdl2 = ""
		);
        /*CR212808 xmzhang modify end*/
	//CR 446444
	BOOL GetFlashContentEx( 
		int nPort,
		int nBaudrate,
		const CString& strFdl,
		NvBaseAddr dwBase,
		DWORD dwSize,
        CString strFdl2 = ""
		);
	void CheckPort2ndEnum();

    BOOL CreateBootModeObj(  );
    BOOL DestroyBootModeObj();

    BOOL StartBootModeWork( DWORD dwPort, DWORD dwBaud,
        DWORD lpDLFileInfo, UINT uFileCount );
    BOOL StopBootModeWork();
    BOOL AllocRecvBuffer( const LPBYTE lpRecvBuffer, DWORD dwSize );
    void FreeRecvBuffer();
    
    
	const void *GetNVItemFromBlock( const unsigned char *pBlock, 
		                            DWORD cbBlockLen,
							        WORD wItemId, 
							        WORD *pLen ); 
		                   

    // ]]
	void LoadNVItemFromImage( const unsigned char *pImage,
							  DWORD cbImageLen );

	BOOL SaveToImage( BOOL fWriteDsp, LPCTSTR lpszDspFilePath );

    void LoadNVValue( const void * pImage );
    BOOL LoadNVValueSection( int                         nNumberOfDirs,
                             const char               *  pDir,
                             int                         nNumberOfValues,
                             const NVVALUE_TABLE_ENTRY * pValue );

    BOOL LoadSettings();
    BOOL SaveSettings();
	BOOL LoadFuncConfig(LPCTSTR lpszFunIni);

	void ExportData(CStdioFile& file, CMapData* mapNVData) ; // add for static nv]
	void GenerateItemInfo(CStdioFile& file);  // add for NVItem Info into csv;
	void AutoLoadCustNV(LPCTSTR lpNVPrjFile,BOOL bCustBase = FALSE);
	

public:
	BOOL IsHasCustBaseNV();
	virtual void UpdatedMsgAllViews( int nMsgID,CObject* pObj = NULL );
	BOOL IsOpenedPrj(){return m_bOpenedPrj;}
	BOOL HasFillValue(){return m_bHasFillValue;}
	BOOL IsLoadNV(){return m_bLoadNV;}
	BOOL IsEnableEditPrj(){return m_bEnableEditPrj;}
	void OnOpenProject();
	void OnDisplayHex();	
	void OnLoadCustomerNV();
	void OnShowFailNv();
	void OnPortSettings();
	void OnLoadImage();
	void OnLoadFromPhone();
	void OnLoadFromPhoneBoot();

	void OnReadFlash();
	void OnFlashMemory();
	void OnLoadNVValue();
	void OnImportItem();
	void OnExportStatic();
	void OnCalibrationNv();
	void OnMergeNv();
	void OnPacketModerm();
	void OnFuncitions();
	void OnFind();
	void OnFindNext();
	void OnGenerStatistical();
	void OnShowModify();

	void OnSaveImage();
	void OnSaveProject();
	void OnSaveProjectAs();
	void OnSaveToPhone();
	void OnSaveToPhoneBoot();
	void OnCloseProject();

	//[[ add for SamSung NV update
	CString BrowseForFolder(LPCTSTR lpszPromt, LPCTSTR pBuf);
    static int CALLBACK BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	//]]
	
// Generated message map functions
protected:
	//{{AFX_MSG(CNVEditorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNewProject();
	afx_msg void OnUpdateNewProject(CCmdUI* pCmdUI);
	afx_msg void OnNewModule();
	afx_msg void OnUpdateNewModule(CCmdUI* pCmdUI);
	afx_msg void OnNewItem();
	afx_msg void OnUpdateNewItem(CCmdUI* pCmdUI);	
	afx_msg void OnSaveChangedToPhone();
	afx_msg void OnUpdateSaveChangedToPhone(CCmdUI* pCmdUI);
	afx_msg void OnInsertModule();
	afx_msg void OnUpdateInsertModule(CCmdUI* pCmdUI);
	afx_msg void OnFillDefault();
	afx_msg void OnUpdateFillDefault(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

    // @ZHANG Ming : 2003-04-23
    // [[
//    afx_msg void OnConfig();
//    afx_msg void OnUpdateConfig(CCmdUI* pCmdUI);
    //]]

    afx_msg void OnFileMruFile(UINT uID);
	afx_msg void OnBMStopMsg();
    
	DECLARE_MESSAGE_MAP()    
		
	CBMOprObserver m_xBMOprObserver;
	friend class CBMOprObserver;
private:
	void	GetSaveNVIDInfo(BOOL bFullWrite = TRUE);
	
    static DWORD WINAPI GetReadWorkThread( LPVOID lpParam );
    DWORD  ReadWorkThreadProc();

	static DWORD WINAPI GetWriteWorkThread( LPVOID lpParam );
    DWORD  WriteWorkThreadProc();

    //Menu condition
    // Not used now,kept for historical reason
    // Always be true
    const BOOL m_bEnableEditPrj;

	BOOL m_bOpenedPrj;
    BOOL m_bSelectedModule;
    BOOL m_bPrjIsDirty;
    BOOL m_bHasFillValue;
    BOOL m_bValueChanged;
   
    BOOL m_bInitListCtrl;

	BOOL m_bUserCancel;
    
	BOOL m_bHexFlag;  // add for show hex/dec  Joly
	
	CString m_strLoadFailInfo; 
	BOOL    m_bLoadNV;

	CDiagObj m_DiagObj;

    // Array of pointer to NVItems read from MS
    CArray< LPBYTE, LPBYTE>   m_aNVItems;    
	std::vector<WORD> m_vecNVID;
    int         m_nNVItemTotalCount;
    int         m_nCurrCount;
	int         m_nReadFail;
	int         m_nWriteFail;
	int         m_nInterpFail;
    CLFPFeedBack m_dlgFeedBack;
    // ]]

    CString m_strPrjFileName;
    CString m_strDspFileName;
    CString m_strFdlFileName;
    CString m_strSelProduct;

	BOOL	m_bFullWrite;
//	CString m_strLoadFailInfo;  //Joly
    BOOL    m_bNewFDLFile;
    BOOL    m_bWriteDSP;
	BOOL    m_bModifyID;
	BOOL    m_bPortSecondEnum; // 下载端口二次枚举功能
    TCHAR *m_szappend;/*xml file support by xmzhang modify add*/

	int     m_nPort;
	int     m_nBaudrate;
	// @ZHANG Ming, March 31, 2004, cr4293
	// [[
	NvBaseAddr m_dwNvBaseAddr;/*CR212808 xmzhang modify*/
	// ]]

	DWORD m_dwNVMaxSize;

    IBootModeHandler         *m_pBootModeObj;
    DWORD                     m_dwCookie;
    HANDLE                    m_hFinishEvent;
    LPBYTE                    m_lpReadBuffer;
    DWORD                     m_dwReadBufferSize;
    BOOL                      m_bOprSuccess;
    CString                   m_strErrDesp;
    
    CString m_strFdl2;

    DWORD m_dwNandFdlAddr;  //@hongliang xin, 2009-3-25, this address is used in NOR FDL too.
    DWORD m_dwNandFdl2Addr;
	
	int   m_nCheckBaudTimes;

	FUNC_ITEM_PTR m_pClearCaliFlag;

	BOOL  m_bCCPrompt;

	CDlgFindItem m_dlgFind;
	
	CString m_strLFPLastErr;

	
	BOOL  m_bHasCustNV;

	_TCHAR m_szDLErrMsg[_MAX_PATH];

    /*CR212808 xmzhang add begin*/
    BOOL    m_bNvBaseStr;
    /*CR212808 xmzhang add end*/  
	BOOL	m_bHasCustBase;
    
    // @ZHANG Ming : 2003-05-06
//#define  DEFAULT_DSPFILENAME  "..\\..\\..\\MS_CODE\\DSP\\bin\\main_prom.bin"
};

#ifndef _DEBUG  // debug version in NVEditorView.cpp
inline CNVEditorDoc* CNVEditorView::GetDocument()
   { return (CNVEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NVEDITORVIEW_H__25F7D2B2_AAF9_4BF2_9074_06F582C9D517__INCLUDED_)
