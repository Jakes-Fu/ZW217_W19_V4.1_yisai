// MSDevDlg.h : header file
//

#if !defined(AFX_MSDEVDLG_H__FB34750E_4F26_41DC_B1B1_DDF3E9B7D957__INCLUDED_)
#define AFX_MSDEVDLG_H__FB34750E_4F26_41DC_B1B1_DDF3E9B7D957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AfxTempl.h"
#include "CommSock.h"
#include "MonitorDlg.h"
#include "ATCommandDlg.h"
#include "MSSimIniFile.h"
#include "Winuser.h"

#include "PixelPropDlg.h"
#include "ImgDialog.h"
//#include "DlgLayers.h"
#include "DlgMainLcdLayer.h"
#include "dal_display.h"
#include "SerialCom.h"
// Define LCD related color
#define COLOR__LCD_EMPTY_PIXEL      RGB(142, 174, 153)

/* Sio send data message */
#define SIO_SEND_DATA       (WM_USER + 3324)

#define WM_ROTATE_SCREEN (WM_USER + 100 )
#define WM_SLIDE_PHONE   (WM_USER + 101 )
#define WM_CAMERA_OPEN   (WM_USER + 102 )
#define WM_CAMERA_CLOSE  (WM_USER + 103 )
#define WM_RCV_ATRES     (WM_USER + 104 )

#define MMI_MAX_LAYER_NUM   8

/////////////////////////////////////////////////////////////////////////////
// CMSDevDlg dialog

class CMSDevDlg : public CImgDialog
{
	enum
	{
		Screen_Original,	// Original screen
		Screen_r90,			// Rotate 90 degree in clockwise
		Screen_r180,		// Rotate 180 degree in counterwise
		Screen_r270,		// Rotate 270 degree in counterwise
	};

// Public interfaces
public:
	void   WriteLogInfo(BYTE *pBuffer, int nLength, LPCTSTR strTag);
    WORD   GetKeyCode();
    void   ChangeHandsetImage( CString strBmpPath,CString strIniPath,CString strBinPath );
	void   ChangeHandsetImage();

	//main LCD
    UINT   GetLCDPanelSize();
    WORD * GetLCDBufferPointer();
    void   SetLCDPixelColor( WORD x, WORD y, WORD color16 );
    WORD   GetLCDPixelColor( WORD x, WORD y );
    void   DrawLCDPixel( WORD x, WORD y, WORD color16 );
    void   InvalidateLCDRect( WORD left, WORD top, WORD right, WORD bottom );
    void   InvalidateLCD();

	//sub LCD
    UINT   GetSubLCDPanelSize();
    WORD * GetSubLCDBufferPointer();
    void   SetSubLCDPixelColor( WORD x, WORD y, WORD color16 );
    WORD   GetSubLCDPixelColor( WORD x, WORD y );
    void   DrawSubLCDPixel( WORD x, WORD y, WORD color16 );
    void   InvalidateSubLCDRect( WORD left, WORD top, WORD right, WORD bottom );
    void   InvalidateSubLCD();

    //@ Liu Kai 2004-6-30 CR10261
    BYTE    GetLCDInfo( BYTE lcdid,void* lpInfo );
	BYTE	GetRealLCDInfo( BYTE lcdid,void* lpInfo );
    
	BYTE RotateScreen( BYTE type );

    void          RunRtos();
    HANDLE        GetRtosThread();
    HANDLE        GetSioThread();

    CCommSock *   GetCommSockPtr();
    CMonitorDlg * GetMonitorDlgPtr();
    DWORD         GetSIOThreadID();
    
    CATCommandDlg * GetAtCommandDlgPtr();

    void          OS_TerminateRtos(HANDLE hTimer, HANDLE hTimerInt);
    void          RtosInitVariables();
    void          RtosResetVariables();

	//apple 2009-04-14
	void SetLcdBackLight( BOOL bOn );
	//apple 2010-02-25 »¬¸ÇÄ£Äâ
	void SetSlideState( BOOL bOn );

	//hongliang 2010-04-01 flip on/off simulate
	void SetFlipState( BOOL bOn );

	void DoSnapshot( void ** ppBuf, DWORD *pLength );
	void DoRecord( void ** ppBuf, DWORD *pLength,BOOL bEnd );

#ifdef _CONTROL_PANEL
    BOOL StartControlPanel();
    void EndControlPanel();
	void ShowControlPanel(BOOL bShow = TRUE);
#endif // _CONTROL_PANEL

	BOOL GetLayerInfo(BYTE lcdid,void** lpInfo, DWORD *pMaxNum);
	void InvalidateLayerInfo();

	void SetVibrate(BOOL bOn);
	
	BOOL IsTraceOn();

// Attributes
public:
    BOOL       m_bStarted;

// Construction
public:
	CMSDevDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMSDevDlg();
    BOOL openPort();
	void closePort();
	LONG reciveFromPort(WPARAM ch, LPARAM port);
	void sendData(char *data);
	void sendChar2Rtos(char ch);
	//void sendChar2Rtos(int ch);
	void sendStr2Rtos(CString strCommand);
// Dialog Data
	//{{AFX_DATA(CMSDevDlg)
	enum { IDD = IDD_MSDEV_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSDevDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMSDevDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAbout();
	afx_msg void OnUpdateChangeImage(CCmdUI* pCmdUI);
	afx_msg void OnChangeImage();
	afx_msg void OnShowLayerInfo();
	afx_msg void OnUpdateShowLayerInfo(CCmdUI* pCmdUI);
	afx_msg void OnTraceOn();
	afx_msg void OnUpdateTraceOn(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDataMonitor();
	afx_msg void OnSetTopmost();
	afx_msg void OnUpdateSetTopmost(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataMonitor(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnStart();
	afx_msg void OnUpdateStart(CCmdUI* pCmdUI);
	afx_msg void OnExit();
	afx_msg void OnLcdSelftest();
	afx_msg void OnUpdateLcdSelftest(CCmdUI* pCmdUI);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);                                              
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags); 
	afx_msg void OnShowPixelProp();
	afx_msg void OnCopyScreen();
	afx_msg void OnShowControlPanel();
	afx_msg void OnUpdateShowControlPanel(CCmdUI* pCmdUI);
	afx_msg void OnShowAtWindow();
	afx_msg void OnComSettings();
	afx_msg void OnUpdateShowAtWindow(CCmdUI* pCmdUI);
	afx_msg LRESULT OnRcvData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRotateScreen( WPARAM,LPARAM );
	afx_msg void OnTestItems( UINT nID );
	afx_msg void OnUpdateTestItems( CCmdUI* pCmdUI );
	afx_msg void OnSlidePhone( WPARAM wParam,LPARAM lParam );
	afx_msg void OnCameraOpen( WPARAM wParam,LPARAM lParam );
	afx_msg void OnCameraClose( WPARAM wParam,LPARAM lParam );
    
    afx_msg LRESULT OnRcvATResponse(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


// Functions
private:

    Image *  LoadHandsetImage(LPCTSTR szPathName,BOOL bMidle= FALSE);
    void     AdjustFrameSize();
    void     RestoreLCDForeground(WORD *pwPrevLCDArray, int nPrevLCDWidth, int nPrevLCDHeight,
                                  WORD *pwLCDArray,     int nLCDWidth,     int nLCDHeight);

	void	 DrawLCD( Graphics &g );

    CRect    GetSubLCDRect();

    BOOL     InitSocket();
    void     InitMS();
    void     TerminateRtos();

    //@ Liu Kai 2004-6-30 CR10261
    BYTE    GetMainLCDInfo( void* lpInfo );
    BYTE    GetSubLCDInfo( void* lpInfo );
    //@Apple Gao cr14042: 2004-10-15
    //[[    
    int     GetKeyCodeFromChar( UINT nChar ); 
    //]]

	void Lcd2Simu( int& x,int& y );
	void Simu2Lcd( int& x,int& y );

	int  GetTestKeyCodeByCommand(UINT nID,BOOL &bGPIO);

	void InitLCD();

	void RotateHandset(int nOldRotateType);

	void InitDCView();
	void SetNextFrame();

	void InitMMILayer();
	void ClearMMILayer();
	

// Variables
private:
	CSerialCom     com;
	Image          *m_pBGImage;
	Image          *m_pMidImage;
	
	Image          *m_pImgOverBtn;
	Image          *m_pImgPressBtn;
	Image          *m_pImgBlackMask;
	
	Image          *m_pImgDCView;

	HBITMAP         m_hMainLCD;
	HBITMAP         m_hSubLCD;
	BYTE           *m_pMainLCDBuf;
	BYTE           *m_pSubLCDBuf;
    // MS information
    int             m_cxImage;       // Handset image width
    int             m_cyImage;       // Handset image height

    WORD            m_wKeyCode;
    BOOL            m_bTopMost;      // Always on top ?
    //BOOL            m_bLCDBacklight; // Is LCD backlight on or off
    BOOL            m_bPickedUp;
    BOOL            m_bHandsFree;
    BOOL            m_bFlipOn;
	BOOL            m_bSlideOn;
    BOOL            m_bDualLCD;
    BOOL            m_bLongPressPower;
	BOOL            m_bPowerKeyUp;
    // Cursors on each key
    HCURSOR         m_hcHand1;
    HCURSOR         m_hcHand2;
    HCURSOR         m_hcArrow;
    HCURSOR         m_hcCursor;

    // Tooltip of each key
    CToolTipCtrl	m_toolTip;

    CCommSock       m_commSock;      // CommSock instance exported from DLL
    HANDLE          m_hRtosThread;   // RTOS thread handle
    CMonitorDlg     *m_pMonitorDlg;  // Data monitor window

    CMSSimIniFile   m_IniFile;
    COLORREF        m_clrMask;       // Mask color to make the handset transparent
    
    CRect           m_rcLCD;         // LCD size     
    int             m_nLCDArraySize; // LCD array size in bytes
    WORD          * m_pwLCDArray;    // 2D dynamic table presenting LCD pixel array in 16bit color format

	CRect			m_rcOriLCD;

    CRect           m_rcSubLCD;         // Sub-LCD size     
    int             m_nSubLCDArraySize; // Sub-LCD array size in bytes
    WORD          * m_pwSubLCDArray;    // 2D dynamic table presenting Sub-LCD pixel array in 16bit color format

    // SIO send thread control block!
    // Here we use a dependent thread to send winsock data.
    DWORD           m_dwSIOThreadID;
    HANDLE          m_hSIOThread;
 
    // The last key pressed but not released
    short m_wLastKeyPress;
	
	// Shift key is pressed
	BOOL m_bShift;

	CPixelPropDlg m_PixelDlg;

	CString m_strOriBmpPath;
	CString m_strOriIniPath;
	CString m_strOriBinPath;

	CString m_strBmpPath;
	CString m_strIniPath;
	CString m_strBinPath;

	BYTE m_nRotateType;

	//apple 2009-04-14
	BOOL m_bLcdBackLight;
	
	CRect  m_rcCurKey;
	int    m_nCurKeyCode;
	BOOL   m_bLDown;


	HACCEL m_hAccel;

//	int    m_nOriHeight;
	BOOL   m_bMiddle;
	BOOL   m_bLockLcd;
	CRgn   m_rgnOrg;

	BOOL   m_bShowCtrlPanel;
    BOOL   m_bShowAtDlg;

    CATCommandDlg     *m_pAtCommandDlg;  

	CUIntArray m_aTestItemState;
	BOOL   m_bDrawMiddle;

	int    m_nNormalSlideXOffset;
	int    m_nNormalSlideYOffset;
	int    m_nRotateSlideXOffset;
	int    m_nRotateSlideYOffset;
	BOOL   m_bBlock;

	// DC View
	UINT   m_nDCVFrameCount;
	UINT   m_nDCVFramePos;
	BOOL   m_bCameraOpen;
	long   m_nInterTime;
	BOOL   m_bSnapShot;
	BOOL   m_bEndRecord;
	CArray<LPVOID,LPVOID> m_aDCPhotos;
	LPBYTE m_pDVAviBuf;
	DWORD  m_dwDVAviSize;
	CRect  m_rcCameraDisp;

	HBITMAP m_hBmpMem;
	
	BYTE * m_pBGBuf;
	DWORD  m_dwBGBufSize;


	DWORD  m_dwSocketPort;
	
	DWORD  m_dwLastTPpos;

	//CDlgLayers m_dlgLayers;
	CDlgMainLcdLayer m_dlgLayers;

	DISPLAY_BLEND_LAYER_T m_layers[MMI_MAX_LAYER_NUM];

	BOOL  m_bTraceOn;
	

// Variables
private:
    FILE    *m_pLogFile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSDEVDLG_H__FB34750E_4F26_41DC_B1B1_DDF3E9B7D957__INCLUDED_)
