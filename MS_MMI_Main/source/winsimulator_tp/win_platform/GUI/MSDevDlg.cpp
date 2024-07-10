// MSDevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSDev.h"
#include "MSDevDlg.h"
#include "ImageConfigDlg.h"
#include "ATCommandDlg.h"
#include "ColorUtility.h"
#include <atlconv.h>
#include "XAboutDlg.h"
#include "comsettings.h"
#include "MSSimInterface.h"
#include "msk_api_gui.h"

#include "keypad.h"
#include "tb_dal.h"
#include "power.h"

#ifndef MSCODE_MOD
#include "os_api.h"
#include "dal_keypad.h"
#endif

#ifdef _CONTROL_PANEL
#include "ConPan.h"
#endif 

//Add for Sim Card at 2002-1-16 By Leo.Feng
#ifdef __SIM_SIMULATOR_
#include "sim_simulator.h"
#endif 

#define SINGLE_CHAR_SEND //每次从串口接收一个字节
#define RECIVE_SLEEP_TIME 5 //从串口接受数据的间隔
#define SEND_SLEEP_TIME 5 //向串口发送数据的间隔
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   WM_UDB_ANSWER               WM_USER + 203
// Port number for MS Simulator (server)
#define MS_PORT     36667   

// @andy.bian modify 2007-1-17
#ifndef SCI_VK_TOUCHPANEL
#define SCI_VK_TOUCHPANEL	0	
#define TP_MOVE				0x00000213
#define TP_DOWN				0x00000211
#define TP_UP				0x00000212
#endif

static const char SECTION_FILE[]        = "FILE";
static const char FILE_ENTRY_BINPATH[]  = "bin_path";
static long com_data_count = 0;
static long com_data_count_r = 0;

#define ID_LONGPRESS_TIMER 1000+1
#define ID_SLIDE_TIMER     1000+2
#define ID_FLIP_TIMER      1000+3
#define ID_DCVIEW_TIMER    1000+4
#define ID_VIBRATE_TIMER   1000+5

#define FRMAE_NUM          5
#define MAX_TEST_ITEM      20

#define X_LCD_BIT_COUNT      (16)
#define X_SUB_LCD_BIT_COUNT  (32)
//w:width b:bitcount
#define GET_WIDTH_BYTE(w,b)  ((((w)*(b)+31)/32)*4)

#define SAFE_DEL_OBJ(h) do{\
	if(h)\
	{\
		::DeleteObject(h);\
		h=NULL;\
	}\
}while(0)

#define GDIP_FREE(p) do{\
	if(p)\
	{\
		DllExports::GdipFree(p);\
		p=NULL;\
	}\
}while(0)
/////////////////////////////////////////////////////////////////////////////
// CMSDevDlg dialog
static CString strcmd = "";

CMSDevDlg::CMSDevDlg(CWnd* pParent /*=NULL*/)
	: CImgDialog(CMSDevDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMSDevDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bStarted      = FALSE;
    m_bTopMost      = FALSE;
    m_bPickedUp     = FALSE;
    m_bHandsFree    = FALSE;
    m_bFlipOn       = FALSE;
	m_bSlideOn      = FALSE;
    m_bDualLCD      = FALSE;

	m_bShowCtrlPanel= FALSE;
    m_bShowAtDlg    = FALSE;

    m_hcHand1       = AfxGetApp()->LoadCursor(IDC_HAND1);
    m_hcHand2       = AfxGetApp()->LoadCursor(IDC_HAND2);
    m_hcArrow       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
    m_hcCursor      = m_hcArrow;

    m_pwLCDArray    = NULL;
    m_pwSubLCDArray = NULL;
	m_nLCDArraySize = 0;
	m_nSubLCDArraySize = 0;

    m_pMonitorDlg   = new CMonitorDlg;
    m_pAtCommandDlg = new CATCommandDlg;
    
    m_wLastKeyPress = -1;

	m_bShift = FALSE;

	m_nRotateType = Screen_Original;

	m_bLcdBackLight = TRUE;

	m_nCurKeyCode = -1;
	m_bLDown = FALSE;
	m_hAccel = NULL;

	m_bMiddle = FALSE;
	m_bLockLcd = FALSE;

	m_clrMask = 0x00FF00;
	m_rgnOrg.CreateRectRgn(0, 0, 0, 0);

	m_bPowerKeyUp = TRUE;
	m_bLongPressPower = FALSE;
	m_pLogFile = NULL;

	m_pBGImage= NULL;      // Image *
	m_pMidImage = NULL;
	
	m_pImgOverBtn = NULL;
	m_pImgPressBtn = NULL;
	m_pImgBlackMask= NULL;

	m_pImgDCView = NULL;

	m_hMainLCD = NULL;     // HBITMAP of DIB
	m_hSubLCD = NULL;	   // HBITMAP of DIB
	m_pMainLCDBuf = NULL;  // it is deleted with m_hMainLCD.
	m_pSubLCDBuf = NULL;   // it is deleted with m_hSubLCD.

	m_bDrawMiddle = FALSE; // Draw middle image for slide

	m_nNormalSlideXOffset = 0; // Slide X offset
	m_nNormalSlideYOffset = 0; // Slide Y offset
	m_nRotateSlideXOffset = 0;
	m_nRotateSlideYOffset = 0;

	m_nDCVFrameCount = 0;
	m_nDCVFramePos = 0;

	m_bBlock = FALSE;

	m_hBmpMem = NULL;

	m_pBGBuf = NULL;

	m_bCameraOpen = FALSE;

	m_bSnapShot = FALSE;
	m_bEndRecord = FALSE;

	m_nInterTime = 100;

	m_aDCPhotos.RemoveAll();

	m_pDVAviBuf = NULL;
	m_dwDVAviSize = 0;

	m_dwSocketPort = MS_PORT;
	
	m_cxImage = 1;
	m_cyImage = 1;

	m_wKeyCode = -1;
	m_hRtosThread = NULL;
	m_dwBGBufSize = 0;

	m_dwSIOThreadID = 0;
	m_hSIOThread = NULL;
	
	m_dwLastTPpos = MAXDWORD;

	m_bTraceOn = TRUE;

	memset(m_layers,0,sizeof(m_layers));
}

CMSDevDlg::~CMSDevDlg()
{
    delete m_pMonitorDlg;
    delete m_pAtCommandDlg;

    delete [] m_pwLCDArray;
    delete [] m_pwSubLCDArray;
	m_bLongPressPower=FALSE;
	m_bPowerKeyUp=TRUE;

	if(NULL != m_pLogFile)
	{
		fclose(m_pLogFile);
		m_pLogFile = NULL;
	}

	GDIP_FREE(m_pBGImage);
	GDIP_FREE(m_pMidImage);
	GDIP_FREE(m_pImgOverBtn);
	GDIP_FREE(m_pImgPressBtn);
	GDIP_FREE(m_pImgBlackMask);

	GDIP_FREE(m_pImgDCView);

	SAFE_DEL_OBJ(m_hMainLCD);
	SAFE_DEL_OBJ(m_hSubLCD);
	SAFE_DEL_OBJ(m_hBmpMem);
	int nCount = m_aDCPhotos.GetSize();
	for(int i=0; i< nCount;i++)
	{
		delete m_aDCPhotos[i];
	}
	m_aDCPhotos.RemoveAll();

	if(m_pDVAviBuf != NULL)
	{
		delete [] m_pDVAviBuf;
		m_pDVAviBuf = NULL;
	}

	ClearMMILayer();
	closePort();
}

void CMSDevDlg::DoDataExchange(CDataExchange* pDX)
{
	CImgDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSDevDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMSDevDlg, CImgDialog)
	//{{AFX_MSG_MAP(CMSDevDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_IMAGE, OnUpdateChangeImage)
	ON_COMMAND(ID_CHANGE_IMAGE, OnChangeImage)
	ON_COMMAND(ID_SHOW_LAYER_INFO, OnShowLayerInfo)
	ON_UPDATE_COMMAND_UI(ID_SHOW_LAYER_INFO, OnUpdateShowLayerInfo)
	ON_COMMAND(ID_TRACE_ON, OnTraceOn)
	ON_UPDATE_COMMAND_UI(ID_TRACE_ON, OnUpdateTraceOn)
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DATA_MONITOR, OnDataMonitor)
	ON_COMMAND(ID_SET_TOPMOST, OnSetTopmost)
	ON_UPDATE_COMMAND_UI(ID_SET_TOPMOST, OnUpdateSetTopmost)
	ON_UPDATE_COMMAND_UI(ID_DATA_MONITOR, OnUpdateDataMonitor)
	ON_WM_DESTROY()
	ON_COMMAND(ID_START, OnStart)
	ON_UPDATE_COMMAND_UI(ID_START, OnUpdateStart)
	ON_COMMAND(ID_FILE_CLOSE, OnExit)
	ON_COMMAND(ID_LCD_SELFTEST, OnLcdSelftest)
	ON_UPDATE_COMMAND_UI(ID_LCD_SELFTEST, OnUpdateLcdSelftest)
    ON_WM_KEYDOWN()                                                                                                      
    ON_WM_KEYUP() 
	ON_COMMAND(ID_SHOW_PIXEL_PROP, OnShowPixelProp)
	ON_COMMAND(ID_SHOW_CONTROL_PANEL, OnShowControlPanel)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CONTROL_PANEL, OnUpdateShowControlPanel)
	ON_COMMAND(ID_AT, OnShowAtWindow)
	ON_COMMAND(ID_COM_SETTINGS, OnComSettings)
	ON_UPDATE_COMMAND_UI(ID_AT, OnUpdateShowAtWindow)
    ON_MESSAGE(WM_SCM_SENDDATA, OnRcvData)
	ON_MESSAGE(WM_ROTATE_SCREEN, OnRotateScreen)
	ON_COMMAND(ID_COPY_SCREEN, OnCopyScreen)
	ON_COMMAND_RANGE(ID_TEST1,ID_TEST20,OnTestItems)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TEST1,ID_TEST20,OnUpdateTestItems)
	ON_MESSAGE(WM_SLIDE_PHONE, OnSlidePhone)
	ON_MESSAGE(WM_CAMERA_OPEN, OnCameraOpen)
	ON_MESSAGE(WM_CAMERA_CLOSE, OnCameraClose)

    ON_MESSAGE(WM_RCV_ATRES, OnRcvATResponse)
	ON_MESSAGE(WM_COMM_RXCHAR, reciveFromPort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSDevDlg message handlers

BOOL CMSDevDlg::OnInitDialog()
{
	CImgDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(_T("MSDev"));
	
	// TODO: Add extra initialization here
	m_pLogFile = fopen("MS_Log.txt", "w+");
    if (!m_pLogFile)
        AfxMessageBox("Can't write log file : \"MS_Log.txt\"");


	((CMSDevApp *)AfxGetApp())->GetImageFilesPath(m_strBmpPath, m_strIniPath,m_strBinPath);
	m_strOriBmpPath = m_strBmpPath;
	m_strOriIniPath = m_strIniPath;
	m_strOriBinPath = m_strBinPath;
	
	m_bTraceOn = ((CMSDevApp *)AfxGetApp())->GetTraceState();
	UpdateData(FALSE);

    InitMS();

    // Adjust the frame window size according to the handset image size
    AdjustFrameSize();

    // Initialize the socket for connection
    if (!InitSocket())
    {
        AfxMessageBox("Can't initialize the socket!", MB_ICONSTOP);
        // Force the application to exit
//        ExitProcess(-1);
    }

	//if(g_theApp.m_nPort != 0)
	{
		CString strTitle;
		strTitle.Format(_T("MSDev [PORT:%d]"),g_theApp.m_nPort != 0 ? g_theApp.m_nPort : m_dwSocketPort);
		SetWindowText(strTitle);
	}
	

	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR));

    // Register the functions defined in the 'MSSimInterface' to MSKernel
    MSK_MSSIM_FUN_LIST funList =
		{
            MSSIM_GetSIOLockEvent,
            MSSIM_sio_flush_tx,
            MSSIM_sio_free,
            MSSIM_bridge_power_off,
            MSSIM_Trace,
            MSSIM_KPD_ScanKey,

            MSSIM_LCD_GetPanelSize,
            MSSIM_LCD_GetBufferPointer,
            MSSIM_LCD_SetPixelColor,
            MSSIM_LCD_GetPixelColor,
            MSSIM_LCD_DrawPixel,
            MSSIM_LCD_InvalidateRect,
            MSSIM_LCD_Invalidate,

            MSSIM_SubLCD_GetPanelSize,
            MSSIM_SubLCD_GetBufferPointer,
            MSSIM_SubLCD_SetPixelColor,
            MSSIM_SubLCD_GetPixelColor,
            MSSIM_SubLCD_DrawPixel,
            MSSIM_SubLCD_InvalidateRect,
            MSSIM_SubLCD_Invalidate,

            //@ Liu Kai 2004-6-30 CR10261
            MSSIM_GetLCDInfo,

			// used to rotate screen
			MSSIM_RotateScreen,
			
			//apple 2009-04-14
			MSSIM_SetLcdBackLight,

			//apple 2010-02-25 滑盖模拟
			MSSIM_SetSlideState,

			//hongliang 2010-04-01 flip on/off simulate
			MSSIM_SetFlipState,

			MSSIM_SetDCViewState,

			MSSIM_DoSnapshot,

			MSSIM_DoRecord,

            MSSIM_sio_at_flush_tx,

			MSSIM_GetLayerInfo,

			MSSIM_SetVibrate,

			MSSIM_IsTraceOn,

			MSSIM_InvalidateLayerInfo,
			MSSIM_SendData2Com
			
        };
    MSK_RegisterMSSimFunList(&funList);

#ifdef _CONTROL_PANEL
    //start control panel
    StartControlPanel();
#endif // #ifdef _CONTROL_PANEL

	m_dlgLayers.Create(IDD_DIALOG_MAIN_LCD_UI_LAYER,GetDesktopWindow());
	m_PixelDlg.Create( IDD_DIALOG_PIXEL_PROP,this );	

	m_pImgOverBtn = CColorUtility::LoadImage(IDR_PNG_OVER_BTN,_T("PNG"),AfxGetResourceHandle());
	m_pImgPressBtn = CColorUtility::LoadImage(IDR_PNG_PRESS_BTN,_T("PNG"),AfxGetResourceHandle());
	m_pImgBlackMask = CColorUtility::LoadImage(IDR_PNG_BLACK_MASK,_T("PNG"),AfxGetResourceHandle());
	InitDCView();

	if(((CMSDevApp*)AfxGetApp())->m_bAutoStart)
	{
		PostMessage(WM_COMMAND,ID_START);
		PostMessage(WM_COMMAND,ID_SHOW_CONTROL_PANEL);
	}

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CMSDevDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CXAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CImgDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMSDevDlg::OnPaint() 
{
	if(m_bBlock)
		return;


	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		HDC hdcTemp = GetDC()->m_hDC;
		HDC hdcMemory = CreateCompatibleDC(hdcTemp);
		if(m_hBmpMem)
		{
			memset(m_pBGBuf,0,m_dwBGBufSize);
			HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, m_hBmpMem); 
			HDC hdcScreen = ::GetDC(m_hWnd);
			RECT rct;
			GetWindowRect(&rct);
			POINT ptWinPos = {rct.left, rct.top};

			Graphics graph(hdcMemory);
			graph.SetSmoothingMode(SmoothingModeNone);		
			if(m_bDrawMiddle)
			{
				Point points2[] = { Point(m_nRotateSlideXOffset, m_nRotateSlideYOffset),
									Point(m_cxImage+m_nRotateSlideXOffset, m_nRotateSlideYOffset),
									Point(m_nRotateSlideXOffset, m_cyImage+m_nRotateSlideYOffset)};
				graph.DrawImage(m_pMidImage,points2,3);
				
			}
			
			//Point points[] = { Point(0, 0),	Point(m_cxImage, 0),Point(0, m_cyImage)};
			//graph.DrawImage(m_pBGImage, points,3);

			CachedBitmap cbBG((Bitmap*)m_pBGImage,&graph);
			graph.DrawCachedBitmap(&cbBG,0,0);				

			DrawLCD(graph);

			SIZE sizeWindow={m_cxImage,m_cyImage};
			POINT ptSrc={0,0};
			DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
			if((dwExStyle&WS_EX_LAYERED)!=WS_EX_LAYERED)
			{
				SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^WS_EX_LAYERED);
			}
			UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &m_Blend, 2);
			
			graph.ReleaseHDC(hdcMemory);
			::ReleaseDC(m_hWnd,hdcScreen);
			hdcScreen=NULL;			

			::SelectObject( hdcMemory, hbmpOld);  

		}
		DeleteDC(hdcMemory);
		hdcMemory=NULL;
		DeleteDC(hdcTemp);
		hdcTemp=NULL;

		CImgDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMSDevDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMSDevDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menu;
    menu.LoadMenu(IDR_MAINFRAME);
	
	CMenu *pTestMenu = menu.GetSubMenu(0)->GetSubMenu(2);
	if(pTestMenu != NULL)
	{
		CStringArray aTest;
		int nCount = m_IniFile.GetTestNames(aTest);
		nCount = nCount>MAX_TEST_ITEM ? MAX_TEST_ITEM: nCount;
	
		for(int i = 0; i< nCount; i++)
		{
			pTestMenu->ModifyMenu(i,MF_BYPOSITION|MF_STRING,ID_TEST1+i,aTest[i]);
		}
		for(int j = (MAX_TEST_ITEM - 1); j>= i; j--)
		{
			pTestMenu->DeleteMenu(j,MF_BYPOSITION);
		}

		if(nCount == 0)
		{
			menu.GetSubMenu(0)->EnableMenuItem(2,MF_BYPOSITION | MF_GRAYED);
		}
	}
    menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd()); 
}


void CMSDevDlg::WriteLogInfo(BYTE *pBuffer, int nLength, LPCTSTR strTag)
{
	if(m_pLogFile == NULL)
		return;

    CTime   tTime;
    CString strTime;
    CString strHeader;
    CString strBuffer;
   
    // Get current time
    tTime = CTime::GetCurrentTime();
    strTime = tTime.Format(_T("%Y-%m-%d %H:%M:%S"));
    
    // Set the log information header --> Received [2001-11-12 10:25:56]
    strHeader = strTag;
    strHeader += _T(" [") + strTime + _T("]");

    // Convert the buffer to string format
    strBuffer = CHexEdit::BufferToString(pBuffer, nLength, 20);
    
    // Write to log file
    _ftprintf(m_pLogFile, _T("/* %s */\n\n"), strHeader);
    _ftprintf(m_pLogFile, _T("%s\n\n"), strBuffer);
    fflush(m_pLogFile);     // Update the file content immediately
}


/////////////////////////////////////////////////////////////////////////////
// Public interfaces
// ----------------------------------------------------------------------------
// Function:
//      GetKeyCode
// Description:
//      Get the scan code of current pressed key
// ----------------------------------------------------------------------------
WORD CMSDevDlg::GetKeyCode()
{
    return m_wKeyCode;
}

// ----------------------------------------------------------------------------
// Function:
//      ChangeHandsetImage
// Description:
//      Change the handset image and refresh 
// ----------------------------------------------------------------------------
void CMSDevDlg::ChangeHandsetImage( CString strBmpPath,CString strIniPath,CString strBinPath )
{
	m_strBmpPath = strBmpPath;
	m_strIniPath = strIniPath;
	m_strBinPath = strBinPath;

	m_strOriBmpPath = m_strBmpPath;
	m_strOriIniPath = m_strIniPath;
	m_strOriBinPath = m_strBinPath;

	ChangeHandsetImage();
}

void CMSDevDlg::ChangeHandsetImage()
{
    // Save the current window position
    CRect rcMainWnd;
    GetWindowRect(&rcMainWnd);
    ((CMSDevApp *)AfxGetApp())->SaveWindowPos(rcMainWnd);

    // Reset the handset image and configuration information
    InitMS();

    // Change the window shape against the new handset image
    AdjustFrameSize();

    // Refresh
    Invalidate();
}

//
// main-LCD panel
//
UINT CMSDevDlg::GetLCDPanelSize()
{
    return MAKELONG( m_rcOriLCD.Width(), m_rcOriLCD.Height() );
}

WORD * CMSDevDlg::GetLCDBufferPointer()
{
    return m_pwLCDArray;
}

void CMSDevDlg::SetLCDPixelColor( WORD x, WORD y, WORD color )
{
    LCD_INFO_T info;
    GetLCDInfo( MAIN_LCD_ID,&info );
	
    if( 1 == info.bits_per_pixel )
    {
        int nIndexInArray = ( y * m_rcOriLCD.Width() + x ) / 16;
        int mask = ( 0x8000 >> ( ( y * m_rcOriLCD.Width() + x ) % 16 ) );
        
        if( color )
        {
            m_pwLCDArray[nIndexInArray] |= mask;
        }
        else
        {
            m_pwLCDArray[nIndexInArray] &= ~mask;
        }
    }
    else
    {
        int nIndexInArray = y * m_rcOriLCD.Width() + x;
        m_pwLCDArray[nIndexInArray] = color;
    }
}

WORD CMSDevDlg::GetLCDPixelColor( WORD x, WORD y )
{
    LCD_INFO_T info;
    GetLCDInfo( MAIN_LCD_ID,&info );

    if( 1 == info.bits_per_pixel )
    {
        int nIndexInArray = ( y * m_rcLCD.Width() + x ) / 16;
        int mask = ( 0x8000 >> ( ( y * m_rcLCD.Width() + x ) % 16 ) );
        
        return ( m_pwLCDArray[nIndexInArray] & mask ) ;        
    }
    else
    {   
        int nIndexInArray = y * m_rcLCD.Width() + x;
        return m_pwLCDArray[nIndexInArray];
    }
}

void CMSDevDlg::DrawLCDPixel( WORD x, WORD y, WORD color )
{
    // Save the pixel color in the LCD array
    SetLCDPixelColor( x, y, color );
	
	// Redraw the window
	Invalidate();
}

void CMSDevDlg::InvalidateLCDRect( WORD left, WORD top, WORD right, WORD bottom )
{
	if( left == right )
    {
        right++;
    }

    if( top == bottom )
    {
        bottom++;
    }

	// MFC CRect not include the right and bottom
	// which is different MMI definition, so must enlarge the rect one pixel
    CRect rect( left, top, right+1, bottom+1 );

    rect.OffsetRect( m_rcLCD.left, m_rcLCD.top );
	
    InvalidateRect( &rect,FALSE);
	//if(m_dlgLayers.m_bShow)
	//{
	//	m_dlgLayers.LayerInvalidateRect(left,top,right+1,bottom+1);
	//}
	
	//UpdateWindow();
}

void CMSDevDlg::InvalidateLCD()
{
	// MFC CRect not include the right and bottom
	// which is different MMI definition, so must enlarge the rect one pixel
	CRect rect(&m_rcLCD);
	rect.right += 1;
	rect.bottom +=1;

	InvalidateRect( &rect ,FALSE);

	//if(m_dlgLayers.m_bShow)
	//{
	//	m_dlgLayers.LayerInvalidate();
	//}
}

void CMSDevDlg::DrawLCD( Graphics &g )
{
    int      pixelIndexInArray;
    COLORREF pixelColorRGB;

    LCD_INFO_T info;
    GetLCDInfo( MAIN_LCD_ID,&info );

	int nHeight = m_rcLCD.Height();
	int nWidth = m_rcLCD.Width();
	int nWidthBytes = GET_WIDTH_BYTE(nWidth,X_LCD_BIT_COUNT);
	int nSize = nWidthBytes* nHeight;
/*  
    for (int y = 0; y < nHeight; y++)
    {
        for (int x = 0; x < nWidth; x++)
        {
            if( 1 == info.bits_per_pixel )
            {
                if( GetLCDPixelColor( x,y ) )
                {
                    pixelColorRGB = RGB( 255,255,255 );
                }
                else
                {
                    pixelColorRGB = RGB( 0,0,0 );
                }
            }
            else
            {
                pixelIndexInArray = y * m_rcLCD.Width() + x;
                pixelColorRGB = CColorUtility::Color16ToColor24( m_pwLCDArray[pixelIndexInArray] );
            }
			int x1= x*(X_LCD_BIT_COUNT)/8;
            if( m_bLcdBackLight )
			{
				m_pMainLCDBuf[y*nWidthBytes+x1] = GetBValue( pixelColorRGB );
				m_pMainLCDBuf[y*nWidthBytes+x1+1] = GetGValue( pixelColorRGB );
				m_pMainLCDBuf[y*nWidthBytes+x1+2] = GetRValue( pixelColorRGB );
			}
			else
			{
				m_pMainLCDBuf[y*nWidthBytes+x1] = GetBValue( pixelColorRGB )/3;
				m_pMainLCDBuf[y*nWidthBytes+x1+1] = GetGValue( pixelColorRGB )/3;
				m_pMainLCDBuf[y*nWidthBytes+x1+2] = GetRValue( pixelColorRGB )/3;
			}
        }
    }
*/
	if( 1 == info.bits_per_pixel )
	{
		for (int y = 0; y < nHeight; y++)
		{
			for (int x = 0; x < nWidth; x++)
			{
				int x1= x*(X_LCD_BIT_COUNT)/8;				
				if( GetLCDPixelColor( x,y ) )
				{					
					m_pMainLCDBuf[y*nWidthBytes+x1]   = 255;
					m_pMainLCDBuf[y*nWidthBytes+x1+1] = 255;					
				}
				else
				{
					m_pMainLCDBuf[y*nWidthBytes+x1]   = 0;
					m_pMainLCDBuf[y*nWidthBytes+x1+1] = 0;
				}
			}
		}
	}
	else
	{
		memcpy(m_pMainLCDBuf,m_pwLCDArray,m_nLCDArraySize*2);
	}
	Bitmap bmpMainLCD(m_hMainLCD,NULL);

	//test DC 
	if((m_bCameraOpen || m_bSnapShot || m_bEndRecord) && m_pImgDCView != NULL )
	{
		//Point points[] = { Point(m_rcLCD.left, m_rcLCD.top),
		//				   Point(m_rcLCD.left+m_rcLCD.Width(), m_rcLCD.top),
		//				   Point(m_rcLCD.left, m_rcLCD.top+m_rcLCD.Height())};
		//g.DrawImage(m_pImgDCView,points,3);
		Color color(0xFF000001);
		SolidBrush bs(color);
		g.FillRectangle(&bs,m_rcLCD.left, m_rcLCD.top,m_rcLCD.Width(),m_rcLCD.Height());
		g.DrawImage(m_pImgDCView,m_rcCameraDisp.left, m_rcCameraDisp.top,m_rcCameraDisp.Width(),m_rcCameraDisp.Height());

		ImageAttributes iAtts;
		iAtts.SetColorKey(Color(0,0,0),Color(0,0,0));
		g.DrawImage(&bmpMainLCD,Rect(m_rcLCD.left,m_rcLCD.top,nWidth,nHeight),0,0,nWidth,nHeight,UnitPixel,&iAtts);
	
	}
	else
	{
		//g.DrawImage(&bmpMainLCD, m_rcLCD.left,m_rcLCD.top,nWidth,nHeight);
		CachedBitmap cbmp(&bmpMainLCD,&g);		
		g.DrawCachedBitmap(&cbmp, m_rcLCD.left,m_rcLCD.top);
	}
	

	if(!m_bLcdBackLight)
	{
		//Point points[] = { Point(m_rcLCD.left, m_rcLCD.top),
		//				   Point(m_rcLCD.left+m_rcLCD.Width(), m_rcLCD.top),
		//				   Point(m_rcLCD.left, m_rcLCD.top+m_rcLCD.Height())};
		//g.DrawImage(m_pImgBlackMask,points,3);

		g.DrawImage(m_pImgBlackMask,m_rcLCD.left, m_rcLCD.top,m_rcLCD.Width(),m_rcLCD.Height());
	}

    if ( m_bDualLCD )
	{	
		GetLCDInfo( SUB_LCD_ID,&info );
		nHeight = m_rcSubLCD.Height();
		nWidth  = m_rcSubLCD.Width();
		nWidthBytes = GET_WIDTH_BYTE(nWidth,X_SUB_LCD_BIT_COUNT);

		for (int y = 0; y < m_rcSubLCD.Height(); y++)
		{
			for (int x = 0; x < m_rcSubLCD.Width(); x++)
			{			
				pixelIndexInArray = y * m_rcSubLCD.Width() + x;
				pixelColorRGB = CColorUtility::ColorByMaskToColor24( m_pwSubLCDArray[pixelIndexInArray] ,
					(WORD)info.r_bitmask, (WORD)info.g_bitmask, (WORD)info.b_bitmask );				
	
				int x1 = x*(X_SUB_LCD_BIT_COUNT)/8;
				
				if( m_bLcdBackLight )
				{
					m_pSubLCDBuf[y*nWidthBytes+x1] = GetBValue( pixelColorRGB );
					m_pSubLCDBuf[y*nWidthBytes+x1+1] = GetGValue( pixelColorRGB );
					m_pSubLCDBuf[y*nWidthBytes+x1+2] = GetRValue( pixelColorRGB );
				}
				else
				{
					m_pSubLCDBuf[y*nWidthBytes+x1] = GetBValue( pixelColorRGB )/3;
					m_pSubLCDBuf[y*nWidthBytes+x1+1] = GetGValue( pixelColorRGB )/3;
					m_pSubLCDBuf[y*nWidthBytes+x1+2] = GetRValue( pixelColorRGB )/3;
				}
			}
		}
		
		Bitmap bmpSubLCD(m_hSubLCD,NULL);
		//g.DrawImage(&bmpSubLCD, m_rcSubLCD.left,m_rcSubLCD.top,nWidth,nHeight);
		CachedBitmap cbSubBmp(&bmpSubLCD,&g);
		g.DrawCachedBitmap(&cbSubBmp, m_rcSubLCD.left,m_rcSubLCD.top);
	}


	if(m_nCurKeyCode != -1 && m_nCurKeyCode != SCI_VK_TOUCHPANEL)
	{
		Point points[] = { Point(m_rcCurKey.left, m_rcCurKey.top),
						   Point(m_rcCurKey.left+m_rcCurKey.Width(), m_rcCurKey.top),
						   Point(m_rcCurKey.left, m_rcCurKey.top+m_rcCurKey.Height())};
		if(m_bLDown)
		{
			if(m_pImgPressBtn!= NULL)
			{
				g.DrawImage(m_pImgPressBtn, points,3);
			}
		}
		else
		{
			if(m_pImgOverBtn!= NULL)
			{
				g.DrawImage(m_pImgOverBtn, points,3);
			}
		}		
	}
}

//
// sub-LCD panel
//
CRect CMSDevDlg::GetSubLCDRect()
{
    return m_rcSubLCD;
}

UINT CMSDevDlg::GetSubLCDPanelSize()
{
    CRect rect = GetSubLCDRect();
    return MAKELONG( rect.Width(), rect.Height() );
}

WORD * CMSDevDlg::GetSubLCDBufferPointer()
{
    return m_pwSubLCDArray;
}

void CMSDevDlg::SetSubLCDPixelColor( WORD x, WORD y, WORD color )
{
    int nIndexInArray = y * m_rcSubLCD.Width() + x;
    m_pwSubLCDArray[nIndexInArray] = color;
}

WORD CMSDevDlg::GetSubLCDPixelColor( WORD x, WORD y )
{
    int nIndexInArray = y * m_rcSubLCD.Width() + x;
    return m_pwSubLCDArray[nIndexInArray];
}

void CMSDevDlg::DrawSubLCDPixel( WORD x, WORD y, WORD color )
{
    // Save the pixel color in the LCD array
    SetSubLCDPixelColor( x, y, color );
    
    // Display the pixel
    WORD xAbs = m_rcSubLCD.left + x;
    WORD yAbs = m_rcSubLCD.top + y;
	//apple change
    //COLORREF colorRGB = Color12ToColor24( color );
	LCD_INFO_T info;
	GetLCDInfo( MAIN_LCD_ID,&info );
	COLORREF colorRGB = CColorUtility::ColorByMaskToColor24( color ,
		(WORD)info.r_bitmask, (WORD)info.g_bitmask, (WORD)info.b_bitmask );
    
    CDC * pDC = GetDC();
    pDC->SetPixel( xAbs, yAbs, colorRGB );
    ReleaseDC( pDC );
}

void CMSDevDlg::InvalidateSubLCDRect( WORD left, WORD top, WORD right, WORD bottom )
{
    //@ Liu kai 2004-7-20 CR11092
    // Rect may be null
    if( left == right )
    {
        right++;
    }
    if( top == bottom )
    {
        bottom++;
    }

    CRect rect( left, top, right, bottom );
    rect.OffsetRect( m_rcSubLCD.left, m_rcSubLCD.top );
    
    InvalidateRect( &rect );
}

void CMSDevDlg::InvalidateSubLCD()
{
    InvalidateRect( &m_rcSubLCD );
}

// ----------------------------------------------------------------------------
// Function:
//      GetRtosThread
// Description:
//      Get the handle of RTOS thread;
//      This functoin will be called by Bridge Layer
// ----------------------------------------------------------------------------
HANDLE CMSDevDlg::GetRtosThread()
{
    return m_hRtosThread;
}

// ----------------------------------------------------------------------------
// Function:
//      GetSioThread
// Description:
//      Get the handle of SIO thread;
//      This functoin will be called by Bridge Layer
// ----------------------------------------------------------------------------
HANDLE CMSDevDlg::GetSioThread()
{
    return m_hSIOThread;
}

// ----------------------------------------------------------------------------
// Function:
//      GetCommSockPtr
// Description:
//      Get the pointer of CommSock instance
// ----------------------------------------------------------------------------
CCommSock* CMSDevDlg::GetCommSockPtr()
{
    return &m_commSock;
}

// ----------------------------------------------------------------------------
// Function:
//      GetMonitorDlgPtr
// Description:
//      Get the pointer of Monitor Dialog instance
// ----------------------------------------------------------------------------
CMonitorDlg* CMSDevDlg::GetMonitorDlgPtr()
{
    return m_pMonitorDlg;
}

CATCommandDlg* CMSDevDlg::GetAtCommandDlgPtr()
{
    return m_pAtCommandDlg;
}


void CMSDevDlg::RtosInitVariables()
{
    m_bStarted = TRUE;
}

void CMSDevDlg::RtosResetVariables()
{
    m_bStarted   = FALSE;
    m_bPickedUp  = FALSE;
    m_bHandsFree = FALSE;
    m_bFlipOn    = FALSE;
	m_bSlideOn   = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMSSimView private functions

// --------------------------------------------------------------------------
// Function: 
//      LoadHandsetImage
// Description:
//      Load handset image from the specified file
// --------------------------------------------------------------------------
Image * CMSDevDlg::LoadHandsetImage(LPCTSTR szPathName,BOOL bMidle/*= FALSE*/)
{
	USES_CONVERSION;
    /*
     * Load image from the specified file
     */    
	Image *pBGImage = NULL;
#ifdef _UNICODE
	pBGImage = Image::FromFile(szPathName);
#else 
	pBGImage = Image::FromFile(A2W(szPathName));
#endif	
	if ( NULL == pBGImage )
    {
        // Force the application to exit
		CString strError = _T("Can't load phone backgroud image.\nMSDev will exit!");
		AfxMessageBox( strError, MB_ICONSTOP );					
		AfxGetApp()->PostThreadMessage(WM_QUIT,0,0); 
		return NULL;
    }

	if(!bMidle)
	{	
		m_cxImage = pBGImage->GetWidth();	
		m_cyImage = pBGImage->GetHeight();

		if(m_hBmpMem)
		{
			SAFE_DEL_OBJ(m_hBmpMem);
		}

		BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
		int nBytesPerLine = ((m_cxImage* 32 + 31) & (~31)) >> 3;
		stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
		stBmpInfoHeader.biWidth = m_cxImage;   
		stBmpInfoHeader.biHeight = m_cyImage;   
		stBmpInfoHeader.biPlanes = 1;
		stBmpInfoHeader.biBitCount = 32;   
		stBmpInfoHeader.biCompression = BI_RGB;   
		stBmpInfoHeader.biClrUsed = 0;   
		stBmpInfoHeader.biSizeImage = nBytesPerLine * m_cyImage;
		
		//PUINT32 pvBits = NULL;  
		
		m_dwBGBufSize = nBytesPerLine * m_cyImage;

		m_hBmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, (LPVOID*)&m_pBGBuf, NULL, 0);
	}
	

	return pBGImage;
}

// --------------------------------------------------------------------------
// Function: 
//      AdjustFrameSize
// Description:
//      Adjust the frame window size according to the handset image size
// --------------------------------------------------------------------------
void CMSDevDlg::AdjustFrameSize()
{
	CRect   rcClient;
    CRect   rcFrame;
    UINT    cxNewFrame;
    UINT    cyNewFrame;
    
    /*
     * Get the current view size and frame window size
     */

    GetClientRect(&rcClient);
    AfxGetMainWnd()->GetWindowRect(&rcFrame);

    /*
     * Calculate the new frame window size to be suitale for the handset image
     */

    cxNewFrame = rcFrame.Width()  - (rcClient.Width() - m_cxImage);
    cyNewFrame = rcFrame.Height()  - (rcClient.Height() - m_cyImage);

    /*
     * Change the frame window size
     */

    AfxGetMainWnd()->SetWindowPos(NULL, 0, 0, cxNewFrame, cyNewFrame, SWP_NOMOVE);
    
    CRect rcMainWnd;
    ((CMSDevApp *)AfxGetApp())->GetWindowPos(rcMainWnd);

    if (rcMainWnd.Width() <= 0)     // Not saved before
        AfxGetMainWnd()->CenterWindow();
    else                            // Use last window pos
        AfxGetMainWnd()->SetWindowPos(NULL, rcMainWnd.left, rcMainWnd.top, 0, 0, SWP_NOSIZE);
}
// --------------------------------------------------------------------------
// Function: 
//      InitSocket
// Description:
//      Initialize the socket connection
//      MSSim is connected as a SERVER side, while BBSim as CLIENT side
// --------------------------------------------------------------------------
BOOL CMSDevDlg::InitSocket()
{
    // Callback structure for Init()
    CALLBACKPARA cbPara;
    cbPara.CallBackFlag     = CALLBACK_WINDOW;
    cbPara.Handle           = GetSafeHwnd();

    // Open parameter for Open()
    OPENPARA openPara;
    openPara.ChannelType    = WINSOCKET;
    openPara.SCMType        = CONNTYPE_SERVER;
    openPara.Port           = MS_PORT;
    openPara.Timeouts       = 500;

	if(g_theApp.m_nPort != 0)
	{
		openPara.Port  = (int)g_theApp.m_nPort;
	}
	// Initialize socket
	BOOL bStatus = m_commSock.Init(&cbPara);

		
	if (bStatus)
	{
		for( int i = 0 ; i< 500; i++)
		{
			openPara.Port += i;
			// Open socket to wait for connection
			bStatus = m_commSock.Open(&openPara, FALSE);
			if (!bStatus)                   // Open failed
			{
				m_commSock.Close();				
			}
			else
			{
				m_dwSocketPort = openPara.Port;
				((CMSDevApp*)AfxGetApp())->SaveSocketPort(m_dwSocketPort);
				break;
			}
			
		}
	}

    return bStatus;
}

DWORD WINAPI SIOSendThread(LPVOID lpParameter)
{
	MSG m_msg;
	// Get the wnd pointer
	CMSDevDlg *pWnd = (CMSDevDlg *)AfxGetMainWnd();


	// Forward message to Comm Sock Module
	CCommSock *pCommSock = pWnd->GetCommSockPtr();
	// Display this message buffer in GUI
	CMonitorDlg *pDlg = pWnd->GetMonitorDlgPtr();	

	// @Xueliang.Wang modified(2002-12-17)
	while(GetMessage(&m_msg, NULL, 0, 0) > 0)
	{
		switch (m_msg.message)
		{
		case SIO_SEND_DATA:
			pCommSock->Send( (void *)m_msg.lParam, m_msg.wParam);

			if (pDlg->GetSafeHwnd() != NULL)    // Monitor dialog is active
				pDlg->DisplayBuffer((uint8 *)m_msg.lParam, m_msg.wParam, "RTOS");

			pWnd->WriteLogInfo((uint8 *)m_msg.lParam, m_msg.wParam, "Sent");

			// Free the memory buffer
			MSK_Free((void *) m_msg.lParam);
			break;
		default:
			//ASSERT(0);
            TRACE("Error Message\n");
            DispatchMessage(&m_msg);
			break;
		}
	}

	return 0;
}

LRESULT CMSDevDlg::OnRcvATResponse(WPARAM wParam, LPARAM lParam)
{
	CATCommandDlg *pDlg = ((CMSDevDlg *)AfxGetMainWnd())->GetAtCommandDlgPtr();	
	
	if (pDlg->GetSafeHwnd() == NULL)
	{
		return 0;
	}
	
    LPBYTE pStart = (LPBYTE)lParam;
    //LPBYTE pEnd   = pStart + (int)lParam;

    Protocol_Header  header;

    // Get the header of assert response
    memcpy(&header, pStart, sizeof(header));
    pStart += sizeof(header);

    // Get the information of assert response
    CString strATRes;
    int     nATResLen     = header.length - sizeof(header);
    LPTSTR  lpszATRes     = strATRes.GetBuffer(nATResLen);
    memcpy(lpszATRes, pStart, nATResLen);
    lpszATRes[nATResLen] = '\0';
    strATRes.ReleaseBuffer();

    // Set AT response
    pDlg->SendATResponse(strATRes);

	if(lParam != NULL)
	{
		delete [] (LPBYTE)lParam;
	}
    return 0;
}

// --------------------------------------------------------------------------
// Function: 
//      RunRtos
// Description:
//      Run the Real Time OS
// --------------------------------------------------------------------------
// gSIOLock is used to enable high priority for CMD message and low 
// priority for log message

// @Richard Yang
HANDLE   hSIOLock;
HANDLE   g_hStopRtos = NULL;

// Restart parameter
typedef struct tagTERMINATE_RTOS_PARAM
{
    HANDLE      hTimer;     // Timer thread
    HANDLE      hTimerInt;  // Timer interrupt thread
    CMSDevDlg  *pView;      // MSSim view pointer
} TERMINATE_RTOS_PARAM;

TERMINATE_RTOS_PARAM *g_pTerminateParam = NULL;


DWORD WINAPI RunRtosProc(LPVOID lpParameter)
{
    // Initialize the variables first
    // Must be ahead of MSK_StartRTOS() because it's a dead-loop
    CMSDevDlg *pView = (CMSDevDlg *)lpParameter;
    pView->RtosInitVariables();

    // Run the RTOS : ThreadX
    MSK_StartRTOS(); // A dead-loop

    return 0;
}

void CMSDevDlg::RunRtos()
{
    if (!MSK_IsPowerOff())
    {
        return;
    }

//Add for Sim Card at 2002-1-16 By Leo.Feng
#ifdef __SIM_SIMULATOR_
       if (!simd_bridge_init())
       {
           AfxMessageBox("Can't initialize Sim Driver Bridge!", MB_ICONSTOP);
           return;
       }
#endif //__SIM_SIMULATOR_

    // Create gSIOLock to enable high priority for CMD message and low 
    // priority for log message
	hSIOLock = CreateEvent(NULL, TRUE, TRUE, FALSE);
    ASSERT(NULL != hSIOLock);

    // Create SIO send thread, here SIO is WINSOCK!
    m_hSIOThread    = ::CreateThread(0, 0, SIOSendThread, NULL, 0, &m_dwSIOThreadID);

    // Create the thread to run RTOS
    DWORD dwThreadId;
    m_hRtosThread = ::CreateThread(0, 0, RunRtosProc, this, 0, &dwThreadId);
}

DWORD WINAPI TerminateRTOSThread( LPVOID lpParam )
{
    ASSERT( g_pTerminateParam != NULL );
    ASSERT( g_pTerminateParam->pView != NULL );

    // Terminate timer management thread
    TerminateThread(g_pTerminateParam->hTimer, 0);
    TerminateThread(g_pTerminateParam->hTimerInt, 0);

    // Terminate the whole RTOS
    TerminateThread(g_pTerminateParam->pView->GetRtosThread(), 0);

    // @Xueliang.Wang added to delete hSIOThread for repower(2002-08-27)
    TerminateThread(g_pTerminateParam->pView->GetSioThread(), 0);

    // @Liu Jun : 2003-03-10
    // Restart the RTOS
    // pRestartParam->pView->RunRtos();

    
    // Set the RTOS status flag
    g_pTerminateParam->pView->RtosResetVariables();

    delete g_pTerminateParam;
    g_pTerminateParam = NULL;
    return 0;
}

// Called by the MSSimInterface.cpp
void CMSDevDlg::OS_TerminateRtos(HANDLE hTimer, HANDLE hTimerInt)
{
    ASSERT( NULL == g_pTerminateParam );

    // Fill in the restart parameter
    g_pTerminateParam = new TERMINATE_RTOS_PARAM;
    g_pTerminateParam->hTimer     = hTimer;
    g_pTerminateParam->hTimerInt  = hTimerInt;
    g_pTerminateParam->pView      = this;

    ResumeThread( g_hStopRtos );
}

// Called by the GUI
void CMSDevDlg::TerminateRtos()
{
    if (MSK_IsPowerOff())
    {
        return;
    }

    // Create one thread to kill these 3 threads and restart RTOS
    DWORD dwThreadID;
    g_hStopRtos = CreateThread(NULL, 0, TerminateRTOSThread, NULL, CREATE_SUSPENDED, &dwThreadID);

    // Send a request to the RTOS to terminate
    MSK_StopRTOS();
    
    // Wait till the RTOS has completely terminated
    const int MAX_STOP_TIME = 10000;
    WaitForSingleObject(g_hStopRtos, MAX_STOP_TIME);
    g_hStopRtos = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CMSSimView message handlers

BOOL CMSDevDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

    m_toolTip.RelayEvent(pMsg);

	if(WM_KEYFIRST <= pMsg->message && WM_KEYLAST >= pMsg->message)
	{
		HACCEL hAccel = m_hAccel;
		if(hAccel && ::TranslateAccelerator(this->m_hWnd,hAccel,pMsg))
		{
			return TRUE;
		}
	}

	if (pMsg->message == WM_KEYDOWN )
	{
// 		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
// 		{
// 			TranslateMessage(pMsg);
// 			DispatchMessage(pMsg);
// 			return TRUE;
// 		}
// 		else
		{
			TranslateMessage(pMsg);
			DispatchMessage(pMsg);
			return TRUE;
		}
	}
	
	return CImgDialog::PreTranslateMessage(pMsg);
}

void CMSDevDlg::InitMS()
{
    HANDLE    hFile;

	//
    // Load the handset image
    //
    hFile = ::CreateFile( m_strBmpPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL );
    if( INVALID_HANDLE_VALUE == hFile )
    {	
        // The file does not exist, reselect a .bmp file
        AfxMessageBox("Please select a MS Image file!");

        char BASED_CODE szFilter[] = "MS Image Files (*.png)|*.png||";
        CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter);
        if (IDOK != dlg.DoModal())
        {
			AfxGetApp()->PostThreadMessage(WM_QUIT,0,0); 
			return ;
        }
		m_strBmpPath = dlg.GetPathName();
		
		if( m_nRotateType == Screen_Original )
		{
			//save image file path
			m_strOriBmpPath = m_strBmpPath;
			((CMSDevApp *)AfxGetApp())->SetImageFilesPath(m_strBmpPath, "","");
		}
     } 
	CloseHandle( hFile );
    	
	//
	// Load the handset configuration file
	//

    hFile = ::CreateFile( m_strIniPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL );
    if( INVALID_HANDLE_VALUE == hFile )
	{
        // The file does not exist, reselect a .ini file
        AfxMessageBox("Please select a MS configuration file!");		
        char BASED_CODE szFilter[] = "MS Configuration Files (*.ini)|*.ini||";
        CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter);
        if (IDOK != dlg.DoModal())
        {
			AfxGetApp()->PostThreadMessage(WM_QUIT,0,0); 			
			return;	
        }
		m_strIniPath = dlg.GetPathName();

		if( m_nRotateType == Screen_Original )
		{
			//save ini file path
			m_strOriIniPath = m_strIniPath;
			((CMSDevApp *)AfxGetApp())->SetImageFilesPath("", m_strIniPath,"");
		}
    }
	CloseHandle( hFile );
	
	//check ini file 
	int nInitResult = m_IniFile.Init(m_strIniPath);
	if (CMSSimIniFile::MIF_SUCCESS != nInitResult)
	{
		CString strError;
        
		if (CMSSimIniFile::MIF_INVALID_KEY_INFO == nInitResult)
		{
			strError.Format("%s: invalid key information.\nThe MSSim will exit!", m_strIniPath);
		}
		else if (CMSSimIniFile::MIF_INVALID_LCD_INFO == nInitResult)
		{
			strError.Format("%s: invalid LCD information.\nThe MSSim will exit!", m_strIniPath);
		}
		else if( CMSSimIniFile::MIF_FILE_NOT_EXIST == nInitResult )
		{
			strError.Format( "%s,file does not exist or can not be opened.\nThe MSSim will exit!",m_strIniPath ); 
		}
		AfxMessageBox( strError, MB_ICONSTOP );	
		AfxGetApp()->PostThreadMessage(WM_QUIT,0,0); 			
		return;	
	}

    m_bDualLCD = m_IniFile.IsDualLCD();
    // check the resouce file
    hFile = ::CreateFile( m_strBinPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL );
	//0x00000020!=GetLastError() meaning other task using this file
    if( INVALID_HANDLE_VALUE == hFile && 0x00000020!=GetLastError())
	{
        // The file does not exist, reselect a resouce file
        AfxMessageBox("Please select a MS resouce file!");
		
        char BASED_CODE szFilter[] = "MS resouce Files (*.bin)|*.bin||";
        CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter);
        if (IDOK != dlg.DoModal())
        {
			AfxGetApp()->PostThreadMessage(WM_QUIT,0,0); 			
			return;	
        }
		m_strBinPath = dlg.GetPathName();

		if( m_nRotateType == Screen_Original )
		{
			//save resouce file path
			m_strOriBinPath = m_strBinPath;
			((CMSDevApp *)AfxGetApp())->SetImageFilesPath("", "",m_strBinPath);
		}		
    }
	CloseHandle( hFile );

	GDIP_FREE(m_pBGImage);
	GDIP_FREE(m_pMidImage);

	m_pBGImage = LoadHandsetImage(m_strBmpPath);

	char* pBuff = MSK_GetResourceFile();
	strcpy(pBuff,m_strBinPath);

    // Save the previous LCD foreground information before loading new LCD panel
    WORD * pwPrevLCDArray    = NULL;
    CRect  rcPrevLCD;

    if ( m_pwLCDArray && m_nLCDArraySize!=0)
    {
        rcPrevLCD       = m_rcLCD;
        pwPrevLCDArray  = new WORD[m_nLCDArraySize];
        memcpy( pwPrevLCDArray, m_pwLCDArray, m_nLCDArraySize * sizeof(WORD) );

        delete [] m_pwLCDArray;
        m_pwLCDArray = NULL;
    }

    // Get the new mask color and LCD size
    m_clrMask = m_IniFile.GetMaskColor();

	m_rcLCD = m_IniFile.GetLCDRect();
	if( Screen_Original == m_nRotateType )
	{
		m_rcOriLCD = m_rcLCD;
	}
    
    // Set up the new LCD pixel array
    m_nLCDArraySize = m_rcLCD.Width() * m_rcLCD.Height();
    m_pwLCDArray = new WORD[ m_nLCDArraySize ];
    memset( m_pwLCDArray, 0, m_nLCDArraySize * sizeof(WORD) );

    if ( pwPrevLCDArray )
    {
        // Restore the LCD foreground information for the LCD panel of new handset image
        RestoreLCDForeground( pwPrevLCDArray, rcPrevLCD.Width(), rcPrevLCD.Height(),
                              m_pwLCDArray,   m_rcLCD.Width(), m_rcLCD.Height() );

        delete [] pwPrevLCDArray;
        pwPrevLCDArray = NULL;
    }

	
	
    if ( m_bDualLCD )
    {
        WORD * pwPrevSubLCDArray = NULL;
        CRect  rcPrevSubLCD;

        if ( m_pwSubLCDArray && m_nSubLCDArraySize!=0)
        {
            rcPrevSubLCD       = m_rcSubLCD;
            pwPrevSubLCDArray  = new WORD[m_nSubLCDArraySize];
            memcpy( pwPrevSubLCDArray, m_pwSubLCDArray, m_nSubLCDArraySize * sizeof(WORD) );
            
            delete [] m_pwSubLCDArray;
            m_pwSubLCDArray = NULL;
        }
        
        m_rcSubLCD = m_IniFile.GetSubLCDRect();
        
        m_nSubLCDArraySize = m_rcSubLCD.Width() * m_rcSubLCD.Height();
        m_pwSubLCDArray = new WORD[ m_nSubLCDArraySize ];
        memset( m_pwSubLCDArray, 0, m_nSubLCDArraySize * sizeof(WORD) );
        
        if ( pwPrevSubLCDArray )
        {
            // Restore the sub-LCD foreground information for the LCD panel of new handset image
            RestoreLCDForeground( pwPrevSubLCDArray, rcPrevSubLCD.Width(), rcPrevSubLCD.Height(),
                m_pwSubLCDArray,   m_rcSubLCD.Width(), m_rcSubLCD.Height() );
            
            delete [] pwPrevSubLCDArray;
            pwPrevSubLCDArray = NULL;
        }
    }

	//must be invoked after loading Handset image INI.
	InitLCD();

	InitMMILayer();
    //
    // Set the tooltip for each key
    //

    if (m_toolTip.GetSafeHwnd())
    {
        m_toolTip.DestroyWindow();
    }

    m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);
    KEYARRAY *paKeys = m_IniFile.GetKeys();
    int nKeyCount = paKeys->GetSize();
	for ( int i = 0; i < nKeyCount; i++ )
    {
		m_toolTip.AddTool(this, paKeys->GetAt(i).strTip, paKeys->GetAt(i).rcKeyRect, i+1);
    }

	CStringArray aTestName;
	int nTestCount = m_IniFile.GetTestNames(aTestName);
	m_aTestItemState.RemoveAll();
	m_aTestItemState.SetSize(nTestCount);
}

void CMSDevDlg::RestoreLCDForeground(WORD *pwPrevLCDArray, int nPrevLCDWidth, int nPrevLCDHeight,
                                      WORD *pwLCDArray,     int nLCDWidth,     int nLCDHeight)
{
    int nSmallerLCDWidth    = nLCDWidth < nPrevLCDWidth ? nLCDWidth : nPrevLCDWidth;
    int nSmallerLCDHeight   = nLCDHeight < nPrevLCDHeight ? nLCDHeight : nPrevLCDHeight;

    for ( int i = 0; i < nSmallerLCDHeight; i++ )
    {
        memcpy( pwLCDArray, pwPrevLCDArray, nSmallerLCDWidth * sizeof(WORD) );
        
        pwLCDArray     += nLCDWidth;     // *****************
        pwPrevLCDArray += nPrevLCDWidth; // !!! Important !!!
    }
}


BOOL CMSDevDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    SetCursor(m_hcCursor);
	
    return TRUE;
	// return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CMSDevDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
    int x_offset = m_rcLCD.left;
    int y_offset = m_rcLCD.top;

	CRect rcKeyOld(&m_rcCurKey);
    int nKeyCode = m_IniFile.HitTest(point,&m_rcCurKey);
	m_nCurKeyCode = nKeyCode;
	if(m_nCurKeyCode != -1 && m_nCurKeyCode !=  SCI_VK_TOUCHPANEL)
	{
		if(SCI_VK_POWER == nKeyCode)
		{
			//TRACE(_T("+-=--------=-+\n"));			
			m_bLongPressPower=FALSE;
		}
		InvalidateRect(&rcKeyOld,FALSE);
		InvalidateRect(&m_rcCurKey,FALSE);		
	}
	else
	{
		InvalidateRect(&rcKeyOld,FALSE);
	}

	//TRACE(_T("OnMouseMove: point.x=%d   point.y=%d\n"),point.x,point.y);
	//TRACE(_T("m_rcCurKey:  left=%d   right=%d   top=%d   bottom=%d\n"),m_rcCurKey.left,m_rcCurKey.right,m_rcCurKey.top,m_rcCurKey.bottom);
    //TRACE(_T("nKeyCode:    %d\n"),nKeyCode);
	if (nKeyCode != -1)
    {
        m_hcCursor = m_hcHand1;
	    if((SCI_VK_TOUCHPANEL == nKeyCode) && ((MK_LBUTTON & nFlags) > 0))
		{
			int x = point.x - x_offset;
			int y = point.y - y_offset;
			Simu2Lcd( x,y );			
			//TRACE(_T("MSK_SendTPcodeToRTOS: start *******************\n"));
	        MSK_SendTPcodeToRTOS(x,y, TP_MOVE);
			//TRACE(_T("MSK_SendTPcodeToRTOS: end   *******************\n"));
		}
    }
    else
    {
        m_hcCursor = m_hcArrow;
		if( m_dwLastTPpos != MAXDWORD &&
			(point.x >= m_rcLCD.right || point.x < m_rcLCD.left ||
		    point.y >= m_rcLCD.bottom || point.y < m_rcLCD.top) )
		{
			int x = HIWORD(m_dwLastTPpos);
			int y = LOWORD(m_dwLastTPpos);
			if(point.x >= m_rcLCD.right)
			{
				x = m_rcLCD.Width()-1;
			}
			if(point.x <= m_rcLCD.left)
			{
				x = 0;
			}
			if(point.y >= m_rcLCD.bottom)
			{
				y = m_rcLCD.Height()-1;
			}
			if(point.y <= m_rcLCD.left)
			{
				y = 0;
			}
			Simu2Lcd(x,y);
			MSK_SendTPcodeToRTOS(x,y,TP_UP);
			m_dwLastTPpos =MAXDWORD;
			SetCursor(m_hcArrow);
		}
    }

	LCD_INFO_T info ={0};
	DWORD dwColor = 0;

	if( point.x >= m_rcLCD.left && point.x < m_rcLCD.right &&
		point.y >= m_rcLCD.top && point.y < m_rcLCD.bottom )
	{
		// In main lcd
		x_offset = point.x - m_rcLCD.left;
		y_offset = point.y - m_rcLCD.top;

		GetLCDInfo( MAIN_LCD_ID,&info );

		dwColor = GetLCDPixelColor( x_offset,y_offset );
	}
	else if( point.x >= m_rcSubLCD.left && point.x < m_rcSubLCD.right &&
		point.y >= m_rcSubLCD.top && point.y < m_rcSubLCD.bottom )
	{
		// In sub lcd
		x_offset = point.x - m_rcSubLCD.left;
		y_offset = point.y - m_rcSubLCD.top;

		GetLCDInfo( SUB_LCD_ID,&info );

		dwColor = GetSubLCDPixelColor( x_offset,y_offset );
	}
	else
	{
		x_offset = -1;
		y_offset = -1;

		dwColor = 0x80000000;
	}

	m_PixelDlg.SetCoordinate( x_offset,y_offset );
	m_PixelDlg.SetMsColor( dwColor,info.r_bitmask,info.g_bitmask,info.b_bitmask );

	HDC hDc = ::GetDC( NULL );
	CPoint pt = point;
	ClientToScreen( &pt );

	m_PixelDlg.SetPcColor( GetPixel( hDc,pt.x,pt.y ) );

	// CView::OnMouseMove(nFlags, point);
}

void CMSDevDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	TRACE( "MSDev: Enter OnLButtonDown" );
	
    int x_offset = m_rcLCD.left;
    int y_offset = m_rcLCD.top; 
	
    if( -1 != m_wLastKeyPress )
    {
        // The KEY_UP message of the last key was not sent by 
        // some reason,we sent it here
		//TRACE( "MSDev: Send last keyup to rtos\n" );
        MSK_SendKeycodeToRTOS( m_wLastKeyPress,KPD_UP );
        m_wLastKeyPress = -1;
    }

	if( MAXDWORD != m_dwLastTPpos)
	{
		int x = HIWORD(m_dwLastTPpos);
		int y = LOWORD(m_dwLastTPpos);
		if(point.x >= m_rcLCD.right)
		{
			x = m_rcLCD.Width()-1;
		}
		if(point.x <= m_rcLCD.left)
		{
			x = 0;
		}
		if(point.y >= m_rcLCD.bottom)
		{
			y = m_rcLCD.Height()-1;
		}
		if(point.y <= m_rcLCD.left)
		{
			y = 0;
		}
		Simu2Lcd(x,y);
		MSK_SendTPcodeToRTOS(x,y,TP_UP);
        m_dwLastTPpos =MAXDWORD;
	}
    
    // Press on the key
    int nKeyCode = m_IniFile.HitTest(point,&m_rcCurKey);
	m_nCurKeyCode = nKeyCode;
    if (nKeyCode != -1)
    {
        SetCapture();

        // Change the cursor shape
        SetCursor(m_hcHand2);
		//TRACE( "MSDEV: Send keycode to rtos\n" );

		if(SCI_VK_TOUCHPANEL == nKeyCode)
		{
			int x = point.x - x_offset;
			int y = point.y - y_offset;
			Simu2Lcd( x,y );
	        MSK_SendTPcodeToRTOS(x,y,TP_DOWN);
			m_dwLastTPpos = MAKELONG(x,y);
		}
		else
		{		
			m_bLDown = TRUE;
			InvalidateRect(&m_rcCurKey,FALSE);
			
			// Save the scan code of this key
            m_wKeyCode = nKeyCode;
            m_wLastKeyPress = nKeyCode;
        
            // Send the key code to RTOS
            MSK_SendKeycodeToRTOS( nKeyCode, KPD_DOWN );
			if(SCI_VK_POWER == nKeyCode)
			{
				//TRACE(_T("=======m_bPowerKeyUp---%d==========\n"),m_bPowerKeyUp);
				if(!m_bPowerKeyUp)
				{
					return;
				}
				m_bPowerKeyUp=FALSE;
				m_bLongPressPower=TRUE;
				//HWND hWnd;
				KillTimer(ID_LONGPRESS_TIMER);
				//TRACE(_T("=======SetTimer==========\n"));
				SetTimer(ID_LONGPRESS_TIMER, 2000,0);
				UpdateWindow();
				return;
// 				if(!m_bStarted)
// 				{
// 					RunRtos();
// 				}		
			}
		}

//		TRACE( "MSDEV: Leave OnLButtonDown" );

        return;
    }

    // Not on the key, then drag the handset to move
    AfxGetMainWnd()->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
}


void CMSDevDlg::OnTimer(UINT nIDEvent)
{
	//TRACE(_T("----m_bLongPressPower--%d---m_bStarted---%d---nIDEvent--%d--\n"),
	//	m_bLongPressPower,m_bStarted,nIDEvent);
	if( (m_bLongPressPower) &&(nIDEvent==ID_LONGPRESS_TIMER))
	{
		KillTimer(ID_LONGPRESS_TIMER);
		if(!m_bStarted)
		{
			RunRtos();
		}
	}
	if(nIDEvent == ID_SLIDE_TIMER)
	{
		int nMaxYOffset = 0; 
		m_IniFile.GetSlideOffset(NULL,&nMaxYOffset);

		if(m_bSlideOn)
		{
			m_nNormalSlideYOffset += 10;
			if(m_nNormalSlideYOffset >=nMaxYOffset)
			{
				m_nNormalSlideYOffset = nMaxYOffset;
				KillTimer(ID_SLIDE_TIMER);
			}
		}
		else
		{
			m_nNormalSlideYOffset -= 10;
			if(m_nNormalSlideYOffset <=0)
			{
				m_nNormalSlideYOffset = 0;
				KillTimer(ID_SLIDE_TIMER);
				m_bDrawMiddle = FALSE;
			}			
		}

		if( m_nRotateType == Screen_Original )
		{			
			m_nRotateSlideXOffset = m_nNormalSlideXOffset;
			m_nRotateSlideYOffset = m_nNormalSlideYOffset;			
		}
		else if( m_nRotateType == Screen_r90 )
		{			
			m_nRotateSlideXOffset = -m_nNormalSlideYOffset;
			m_nRotateSlideYOffset = m_nNormalSlideXOffset;			
		}
		else if( m_nRotateType == Screen_r180 )
		{
			m_nRotateSlideXOffset = m_nNormalSlideXOffset;
			m_nRotateSlideYOffset = -m_nNormalSlideYOffset;
		}
		else if( m_nRotateType == Screen_r270 )
		{
			m_nRotateSlideXOffset = m_nNormalSlideYOffset;
			m_nRotateSlideYOffset = m_nNormalSlideXOffset;
		}
		OnPaint();
	}

	if(ID_DCVIEW_TIMER == nIDEvent)
	{
		SetNextFrame();
		//OnPaint();
		InvalidateRect(m_rcLCD);
		//Invalidate();
	}

	if(ID_VIBRATE_TIMER == nIDEvent)
	{
		static int nState = 0;
		nState = (nState + 1)%2;
		CRect rect;
		GetWindowRect(&rect);

		int nWidth = rect.Width();
		int nHeight = rect.Height();
		int x = rect.left;
		int y = rect.top;
		if(nState == 0)
		{	
			SetWindowPos(&wndTopMost , x+10,y+10, nWidth,nHeight,SWP_NOSIZE);
		}
		else
		{
			SetWindowPos(&wndTopMost , x-10,y-10, nWidth,nHeight,SWP_NOSIZE);
		}
	}

	CImgDialog::OnTimer(nIDEvent);
} 

void CMSDevDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	TRACE( "MSDEV:Enter OnLButtonUp" );
    int x_offset = m_rcLCD.left;
    int y_offset = m_rcLCD.top;    

	m_bLDown = FALSE;

    int nKeyCode = m_IniFile.HitTest(point,&m_rcCurKey);
    /* 
    //fortest
	if( nKeyCode == 37 )
	{
		SetSlideState( !m_bSlideIn );
	}
	*/
	// if button up on any key, set m_bLongPressPower = FALSE
	//if(SCI_VK_POWER == nKeyCode)
	{
		//TRACE(_T("+++++++++++++++++\n"));
		m_bPowerKeyUp=TRUE;
		m_bLongPressPower=FALSE;
	}
    if ( GetCapture() != this )
    {
        return;
    }

	m_nCurKeyCode = nKeyCode;
	if(m_nCurKeyCode != -1 && m_nCurKeyCode !=  SCI_VK_TOUCHPANEL)
	{
		InvalidateRect(&m_rcCurKey,FALSE);
	}

    if( -1 != m_wLastKeyPress )
    {
		//TRACE( "MSDEV: Send last keyup to rtos\n" );
        // Send the last KEY_UP message,and discard the new one
        MSK_SendKeycodeToRTOS( m_wKeyCode,KPD_UP );        
        m_wLastKeyPress = -1;
    }
    else
    {
        if (nKeyCode != -1)
        {
            SetCursor(m_hcHand1);
   			//TRACE( "MSDEV: Send keycode to rtos\n" );

            if(SCI_VK_TOUCHPANEL == nKeyCode)
            {
				int x = point.x - x_offset;
				int y = point.y - y_offset;
				Simu2Lcd( x,y );

                MSK_SendTPcodeToRTOS(x,y,TP_UP);
				m_dwLastTPpos =MAXDWORD;
            }
            else
            {
				if( MAXDWORD != m_dwLastTPpos)
				{
					int x = HIWORD(m_dwLastTPpos);
					int y = LOWORD(m_dwLastTPpos);			
				
					if(point.x >= m_rcLCD.right)
					{
						x = m_rcLCD.Width()-1;
					}
					if(point.x <= m_rcLCD.left)
					{
						x = 0;
					}
					if(point.y >= m_rcLCD.bottom)
					{
						y = m_rcLCD.Height()-1;
					}
					if(point.y <= m_rcLCD.left)
					{
						y = 0;
					}
					Simu2Lcd(x,y);
					MSK_SendTPcodeToRTOS(x,y,TP_UP);
					m_dwLastTPpos =MAXDWORD;
				}

				// Save the scan code of this key
                m_wKeyCode = nKeyCode;
                // Send message to RTOS
                MSK_SendKeycodeToRTOS( nKeyCode, KPD_UP );
            
                SetCursor(m_hcHand1);
            }			
            
            ReleaseCapture();

//			TRACE( "MSDEV: Leave OnLButtonUp" );
            return;
        }
    }

	if( MAXDWORD != m_dwLastTPpos)
	{
		int x = HIWORD(m_dwLastTPpos);
		int y = LOWORD(m_dwLastTPpos);
		if(point.x >= m_rcLCD.right)
		{
			x = m_rcLCD.Width()-1;
		}
		if(point.x <= m_rcLCD.left)
		{
			x = 0;
		}
		if(point.y >= m_rcLCD.bottom)
		{
			y = m_rcLCD.Height()-1;
		}
		if(point.y <= m_rcLCD.left)
		{
			y = 0;
		}
		Simu2Lcd(x,y);
		MSK_SendTPcodeToRTOS(x,y,TP_UP);
		m_dwLastTPpos =MAXDWORD;
	}

    // Restore the cursor if not in the last area
    m_hcCursor = m_hcArrow;
    ReleaseCapture();

//	TRACE( "MSDEV: Leave OnLButtonUp" );

    return;    

    // CView::OnLButtonUp(nFlags, point);
}

//@Apple Gao cr14042: 2004-10-15
//[[

void CMSDevDlg::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )                                                          
{                             
    if( -1 != m_wLastKeyPress )
    {
        
        // The KEY_UP message of the last key was not sent by 
        // some reason,we sent it here
        MSK_SendKeycodeToRTOS( m_wLastKeyPress,KPD_UP );
        m_wLastKeyPress = -1;
    }

	UINT nCharPress = nChar;
    
	if( m_bShift )
	{
		if( nCharPress == '3' )
		{
			nCharPress = '#';
		}

		if( nCharPress == '8' )
		{
			nCharPress = '*';
		}
	}
    int nKeyCode = GetKeyCodeFromChar( nCharPress );
	if(m_IniFile.GetKeyRectByCode(nKeyCode,&m_rcCurKey))
	{
		m_bLDown = TRUE;
		m_nCurKeyCode = nKeyCode;
		if(m_nCurKeyCode != -1 && m_nCurKeyCode != SCI_VK_TOUCHPANEL)
		{
			InvalidateRect(&m_rcCurKey,FALSE);
		}

	}
	else
	{
		m_nCurKeyCode = -1;
	}

    if( nKeyCode != -1 )                                                                                                     
    {                                                                                                                        
        MSK_SendKeycodeToRTOS( nKeyCode, KPD_DOWN );                                                                         
        m_wKeyCode = nKeyCode;   
        m_wLastKeyPress = nKeyCode;
        return;                                                                                                              
    }
	
	if( VK_SHIFT == nChar )
	{
		m_bShift = TRUE;
	}
    
    CImgDialog::OnKeyDown( nChar, nRepCnt, nFlags );                                                                              
}                                                                                                                            

void CMSDevDlg::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )                                                            
{
	m_bLDown = FALSE;
	if(m_nCurKeyCode != -1 && m_nCurKeyCode !=  SCI_VK_TOUCHPANEL)
	{
		InvalidateRect(&m_rcCurKey,FALSE);
	}

	if( -1 != m_wLastKeyPress )
	{
        // Send the last KEY_UP message,and discard the new one
        MSK_SendKeycodeToRTOS( m_wKeyCode,KPD_UP );        
        m_wLastKeyPress = -1;
		m_bShift = FALSE;
	}
	else
	{
		int nKeyCode = GetKeyCodeFromChar( nChar );                                                                              
		if( nKeyCode != -1 )                                                                                                     
		{                                                                                                                        
			m_wKeyCode = nKeyCode;   
			m_wLastKeyPress = -1;
			MSK_SendKeycodeToRTOS( nKeyCode, KPD_UP );                                                                           
			return;                                                                                                              
		} 
	}
	if( VK_SHIFT == nChar )
	{
		m_bShift = FALSE;
	}
    
    CImgDialog::OnKeyUp( nChar, nRepCnt, nFlags );                                                                                
}                                                                                                                            


int CMSDevDlg::GetKeyCodeFromChar( UINT nChar  )                                                                            
{                                                                                                                            
    int nKeyCode = -1;                                                                                                       
    switch( nChar )                                                                                                          
    {                                                                                                                        
    //case VK_F5:                                                                                                        
    //    nKeyCode = SCI_VK_EXT1;                                                                                              
    //    break;                                                                                                               
    //case VK_F6:                                                                                                             
    //    nKeyCode = SCI_VK_EXT2;                                                                                              
    //    break;                                                                                                               
    //case VK_F7:
    //    nKeyCode = SCI_VK_CALL;                                                                                              
    //    break;                                                                                                               
    //case VK_F8:                                                                                                          
    //    nKeyCode = SCI_VK_POWER;                                                                                             
    //    break;                                                                                                               
    case VK_ESCAPE:                                                                                                          
        nKeyCode = SCI_VK_CANCEL;                                                                                            
        break;                                                                                                               
    case '#':                                                                                                          
        nKeyCode = SCI_VK_POUND;                                                                                             
        break;                                                                                                               
    case VK_MULTIPLY:                                                                                                        
	case '*':
        nKeyCode = SCI_VK_STAR;                                                                                              
        break;                                                                                                               
    case VK_UP:                                                                                                              
        nKeyCode = SCI_VK_UP;                                                                                                
        break;                                                                                                               
    case VK_DOWN:                                                                                                            
        nKeyCode = SCI_VK_DOWN;                                                                                              
        break;                                                                                                               
    case VK_LEFT:                                                                                                            
        nKeyCode = SCI_VK_LEFT;                                                                                              
        break;                                                                                                               
    case  VK_RIGHT:                                                                                                          
        nKeyCode = SCI_VK_RIGHT;                                                                                             
        break;                                                                                                               
    case VK_NUMPAD0:   
	case '0':
        nKeyCode = SCI_VK_0;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD1:  
	case '1':
        nKeyCode = SCI_VK_1;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD2:                                                                                                         
	case '2':
        nKeyCode = SCI_VK_2;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD3:                                                                                                         
	case '3':
        nKeyCode = SCI_VK_3;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD4:                                                                                                         
    case '4':
		nKeyCode = SCI_VK_4;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD5:                                                                                                         
	case '5':
        nKeyCode = SCI_VK_5;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD6:                                                                                                         
	case '6':
        nKeyCode = SCI_VK_6;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD7:                                                                                                         
	case '7':
        nKeyCode = SCI_VK_7;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD8:                                                                                                         
	case '8':
        nKeyCode = SCI_VK_8;                                                                                                 
        break;                                                                                                               
    case VK_NUMPAD9:                                                                                                         
    case '9':
		nKeyCode = SCI_VK_9;                                                                                                 
        break;       
	case VK_PRIOR:	/* Page up */
		nKeyCode = SCI_VK_EXT3;
		break;
	case VK_NEXT:	/* Page down */
		nKeyCode = SCI_VK_EXT4;
		break;
	case VK_F4:
		nKeyCode = SCI_VK_EXT5;
		break;
	case VK_F3:
		nKeyCode = SCI_VK_CAMERA;
		break;
	case VK_RETURN:
		nKeyCode = SCI_VK_EXT1;
		break;
	case VK_BACK:
		nKeyCode = SCI_VK_EXT2;
		break;		
    }                                                                                                                        
    return nKeyCode;                                                                                                         
}                           
//]]

// ----------------------------------------------------------------------------
// Function:
//      OnRcvData
// Description:
//      parse the buffer and send each message to rtos queue
// ----------------------------------------------------------------------------
LRESULT CMSDevDlg::OnRcvData(WPARAM wParam, LPARAM lParam)
{
#if 0//debug用代码
	int para = (int)lParam;
	char ack[1] = {0x06}; 
	char nak[1] = {0x15}; 
	if(0 != para)
	{
		/*HWND h = ::FindWindow(0,_T("demoCmd"));
		if(NULL == h)
		{
			AfxMessageBox( "find demoCmd error" );
			return 1;
		}*/
		switch (para)
		{
		case 0x101:
			//AfxMessageBox( "install js" );
			//::SendMessage(h,WM_UDB_ANSWER,0,1);
			break;
        case 0x102:
			//AfxMessageBox( "uninstall js" );
			//::SendMessage(h,WM_UDB_ANSWER,0,2);
			break;
	case 0x103:
			//AfxMessageBox( "uninstall js" );
			//::SendMessage(h,WM_UDB_ANSWER,0,2);
			sendData("C");
			break;
	case 0x104:
			//AfxMessageBox( "uninstall js" );
			//::SendMessage(h,WM_UDB_ANSWER,0,2);
			sendData(ack);
			break;
	case 0x105:
			//AfxMessageBox( "uninstall js" );
			//::SendMessage(h,WM_UDB_ANSWER,0,2);
			sendData(nak);
			break;
			default:
				break;
		}
		return 0;
	}
#endif//debug用代码
    PCOMMONDATA pBuffer = (PCOMMONDATA)wParam;  // Buffer to receive data  (message)
    int nLength = pBuffer->lDataLen;            // Length of received data (message)

    // Receive data from SCM;
    if (nLength > 0)            // Buffer received
    {
        // Display the message content in the edit control
        if (m_pMonitorDlg->GetSafeHwnd() != NULL)   // Monitor dialog is active
            m_pMonitorDlg->m_wndClientBuffer.DisplayBuffer(pBuffer->lpData, nLength);

        // Write this message to log file
        WriteLogInfo(pBuffer->lpData, nLength, "Received");

        // Handle the specified messages
		#pragma pack(push, 1)
        typedef struct MSG_HEAD_tag
        {
            BYTE   bFlagByte;   // flag byte. Here we use 0x7E
            WORD   seq_num;	    // sequence number
            WORD   dummy;	    // reserved
            WORD   wSize;       // frame size
            BYTE   bType;       // command type
            BYTE   bSubtype;    // command subtype
        } MSG_HEAD;
		#pragma pack(pop)

        MSG_HEAD *pMsgHead = (MSG_HEAD *)pBuffer->lpData;
        if (DIAG_POWER_F == pMsgHead->bType)
        {
            if (POWER_ON == pMsgHead->bSubtype)
            {
                RunRtos();
            }
            else
            {
                TerminateRtos();
            }
        }
        else
        {
            // Forward this message to RTOS
            UINT status = MSK_SendMsgToRTOS(pBuffer->lpData, nLength);
            if (status != SCI_SUCCESS)
            {
                CString strError;
                strError.Format("Can't send messages to RTOS! Error code : %d", status);
                AfxMessageBox(strError, MB_ICONSTOP);
            }
        }

        // Free message buffer in SCM
        m_commSock.DestroyData(&pBuffer);
    }

    return 0;
}

void CMSDevDlg::OnStart() 
{	
	 RunRtos();
#if 0
    if (m_bStarted)
    {
        TerminateRtos();
    }
    else
    {
        RunRtos();
    }
#endif
}

void CMSDevDlg::OnDataMonitor() 
{
	// TODO: Add your command handler code here
	
    if (m_pMonitorDlg->GetSafeHwnd() == NULL)
    {
        m_pMonitorDlg->Create(IDD_MONITOR);

        CRect rcMainWnd;
        AfxGetMainWnd()->GetWindowRect(&rcMainWnd);
        m_pMonitorDlg->SetWindowPos(NULL, rcMainWnd.right, rcMainWnd.top, 0, 0, SWP_NOSIZE);

        m_pMonitorDlg->ShowWindow(SW_SHOW);
        SetFocus();
    }
}

void CMSDevDlg::OnChangeImage() 
{
	if( m_nRotateType == Screen_Original )
	{
		CImageConfigDlg dlg;
		dlg.DoModal();
	}
}

void CMSDevDlg::OnSetTopmost() 
{
    m_bTopMost = !m_bTopMost;
	
    if (m_bTopMost)
        AfxGetMainWnd()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else
        AfxGetMainWnd()->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);	
}

void CMSDevDlg::OnUpdateStart(CCmdUI* pCmdUI) 
{
	CString strMenuText;
	strMenuText.LoadString(IDS_START);
	pCmdUI->SetText(strMenuText);
	
	if (m_bStarted)
	{	
		pCmdUI->Enable(FALSE);
	}

#if 0	
    UINT    idMenuText;
    CString strMenuText;

    idMenuText = m_bStarted ? IDS_STOP : IDS_START;
    strMenuText.LoadString(idMenuText);
    pCmdUI->SetText(strMenuText);
#endif
}


void CMSDevDlg::OnUpdateSetTopmost(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

    pCmdUI->SetCheck(m_bTopMost);
}

void CMSDevDlg::OnUpdateDataMonitor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

    pCmdUI->Enable(!m_pMonitorDlg->GetSafeHwnd());
}

DWORD CMSDevDlg::GetSIOThreadID()
{
    return m_dwSIOThreadID;
}


#ifdef _CONTROL_PANEL

void CMSDevDlg::EndControlPanel()
{
    CP_EndControlPanel();
}

BOOL CMSDevDlg::StartControlPanel()
{
    CRect rcThis;
    GetWindowRect(rcThis);
    return  CP_StartControlPanel(rcThis.right + 10, rcThis.top);
}

void CMSDevDlg::ShowControlPanel(BOOL bShow/* = TRUE*/)
{
	CP_ShowControlPanel(bShow);
}

#endif // _CONTROL_PANEL

void CMSDevDlg::OnDestroy() 
{
	CImgDialog::OnDestroy();
	
#ifdef _CONTROL_PANEL    
    EndControlPanel();
#endif // _CONTROL_PANEL

}
void CMSDevDlg::OnExit() 
{
	
    if (m_bStarted)
    {
        TerminateRtos();
    }

    AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CMSDevDlg::OnLcdSelftest() 
{
	
    int x = 0;
    int cxLCD = 0;
    
    WORD * pBuf = GetLCDBufferPointer();
    
    AfxMessageBox( "Main LCD Test" );
    
    memset( pBuf, 0xFF, m_rcLCD.Width() * m_rcLCD.Height() * sizeof(WORD) );
    InvalidateLCD();
    AfxMessageBox( "FULL WHITE : 0xFFFF" );
    
    cxLCD = m_rcLCD.Width();
    for ( x = 0; x < m_nLCDArraySize; x++ )
    {
        if ( 0 == x % cxLCD )
        {
            m_pwLCDArray[x] = 128 * ( x / cxLCD );
        }
        else
        {
            m_pwLCDArray[x] = m_pwLCDArray[x-1] + 1;
        }
    }
    InvalidateLCD();
    AfxMessageBox( "64K mode : 16bit - RGB:565" );

    if ( m_bDualLCD )
    {
        WORD * pSubBuf = GetSubLCDBufferPointer();
    
        AfxMessageBox( "Sub LCD Test" );
    
        memset( pSubBuf, 0xFF, m_rcSubLCD.Width() * m_rcSubLCD.Height() * sizeof(WORD) );
        InvalidateSubLCD();
    
        AfxMessageBox( "FULL WHITE : 0xFFFF" );
    
        cxLCD = m_rcSubLCD.Width();
        for ( x = 0; x < m_nSubLCDArraySize; x++ )
        {
            if ( 0 == x % cxLCD )
            {
                m_pwSubLCDArray[x] = 128 * ( x / cxLCD );
            }
            else
            {
                m_pwSubLCDArray[x] = m_pwSubLCDArray[x - 1] + 1;
            }
        }
        InvalidateSubLCD();
        AfxMessageBox( "4K mode : 12bit - RGB:444" );

        memset( m_pwLCDArray, 0, m_nLCDArraySize * sizeof(WORD) );
        memset( m_pwSubLCDArray, 0, m_nSubLCDArraySize * sizeof(WORD) );
        InvalidateLCD();
        InvalidateSubLCD();
    }
}

void CMSDevDlg::OnUpdateLcdSelftest(CCmdUI* pCmdUI) 
{

    pCmdUI->Enable( MSK_IsPowerOff() );
}

//@ Liu Kai 2004-6-30 CR10261
BYTE CMSDevDlg::GetLCDInfo( BYTE lcdid,void* lpInfo )
{
    if( lpInfo == NULL )
    {
        return ERR_LCD_POINTER_NULL;
    }

    if( MAIN_LCD_ID == lcdid )
    {
        return GetMainLCDInfo( lpInfo );
    }
    else if( SUB_LCD_ID == lcdid )
    {
        return GetSubLCDInfo( lpInfo );
    }
    else
    {
        return ERR_LCD_PARAMETER_WRONG;
    }
}

BYTE CMSDevDlg::GetRealLCDInfo( BYTE lcdid,void* lpInfo )
{
	if( lpInfo == NULL )
    {
        return ERR_LCD_POINTER_NULL;
    }
	
    if( MAIN_LCD_ID == lcdid )
    {
        BYTE bRet = GetMainLCDInfo( lpInfo );
		LCD_INFO_T* lpLCDInfo = (LCD_INFO_T*)lpInfo;
		lpLCDInfo->lcd_width = m_rcOriLCD.Width();
		lpLCDInfo->lcd_height = m_rcOriLCD.Height();
		return bRet;
    }
	else
	{
		return GetLCDInfo( lcdid,lpInfo );
	}
}

//@ Liu Kai 2004-6-30 CR10261
BYTE CMSDevDlg::GetMainLCDInfo( void* lpInfo )
{
    LCD_INFO_T* lpLCDInfo = m_IniFile.GetLCDInfo( (LCD_ID_E)MAIN_LCD_ID );
    memcpy( lpInfo,lpLCDInfo,sizeof( LCD_INFO_T ) );
    ( ( LCD_INFO_T* )lpInfo )->lcdbuff_ptr = GetLCDBufferPointer();

    return 0;    
}

BYTE CMSDevDlg::GetSubLCDInfo( void* lpInfo )
{
    LCD_INFO_T* lpLCDInfo = m_IniFile.GetLCDInfo( (LCD_ID_E)SUB_LCD_ID );
    memcpy( lpInfo,lpLCDInfo,sizeof( LCD_INFO_T ) );
    ( ( LCD_INFO_T* )lpInfo )->lcdbuff_ptr = GetSubLCDBufferPointer();
    
    return 0;
}

void CMSDevDlg::OnShowPixelProp() 
{
	m_PixelDlg.ShowWindow( SW_SHOW );	
}

CString ModifyPath( CString strPath,CString strModify )
{
	CString strRet;

	int nIndex = strPath.ReverseFind( '.' );
	if( nIndex == -1 )
	{
		strRet = strPath + strModify;
	}
	else
	{
		strRet = strPath.Left( nIndex ) + "_" + strModify + strPath.Right( strPath.GetLength() - nIndex );
	}

	return strRet;
}

BYTE CMSDevDlg::RotateScreen( BYTE type )
{
	if( type == m_nRotateType )
	{
		// Do not need rotate
		return TRUE;
	}

	int nOldRotateType = m_nRotateType;

	m_nRotateType = type;

	RotateHandset(nOldRotateType);

	// Change the window shape against the new handset image
    AdjustFrameSize();

    // Refresh
    Invalidate();
	
	return TRUE;
}

void CMSDevDlg::Simu2Lcd( int& x,int& y )
{
	int x1 = x,y1 = y;
	
	if( m_nRotateType == Screen_r90 )
	{
		x = y1;
		y = m_rcLCD.Width() - 1 - x1;
	}
	else if( m_nRotateType == Screen_r180 )
	{
		x = m_rcLCD.Width() - 1 - x1;
		y = m_rcLCD.Height() - 1 - y1;
	}
	else if( m_nRotateType == Screen_r270 )
	{
		x = m_rcLCD.Height() - 1 - y1;
		y = x1;
	}
}

void CMSDevDlg::Lcd2Simu( int& x,int& y )
{
	int x1 = x,y1 = y;

	if( m_nRotateType == Screen_r90 )
	{
		x = m_rcLCD.Height() - 1 - y1;
		y = x1;
	}
	else if( m_nRotateType == Screen_r180 )
	{
		x = m_rcLCD.Width() - 1 - x1;
		y = m_rcLCD.Height() - 1 - y1;
	}
	else if( m_nRotateType == Screen_r270 )
	{
		x = y1;
		y = m_rcLCD.Width() - 1 - x1;
	}
}

LRESULT CMSDevDlg::OnRotateScreen( WPARAM wParam ,LPARAM )
{
	return RotateScreen( wParam );
}

//apple 2009-04-14
void CMSDevDlg::SetLcdBackLight( BOOL bOn )
{
	m_bLcdBackLight = bOn;
	InvalidateLCD();
	InvalidateSubLCD();
}

//apple 2010-02-25 滑盖模拟
void CMSDevDlg::SetSlideState( BOOL bOn )
{
	if( m_bSlideOn != bOn )
	{
		m_bSlideOn = bOn;

		if( bOn )
		{			
			CString strMidBmpPath= ModifyPath( m_strBmpPath,"middle" );
			Image *pMidImage  =LoadHandsetImage(strMidBmpPath,TRUE);
			if(pMidImage != NULL)
			{
				if( m_nRotateType == Screen_Original )
				{
					pMidImage->RotateFlip(RotateNoneFlipNone);			
				}
				else if( m_nRotateType == Screen_r90 )
				{
					pMidImage->RotateFlip(Rotate90FlipNone);					
				}
				else if( m_nRotateType == Screen_r180 )
				{
					pMidImage->RotateFlip(Rotate180FlipNone);			
				}
				else if( m_nRotateType == Screen_r270 )
				{
					pMidImage->RotateFlip(Rotate270FlipNone);		
				}
				GDIP_FREE(m_pMidImage);
				m_pMidImage=pMidImage;
				m_bDrawMiddle = TRUE;	

				::SetTimer(this->GetSafeHwnd(),ID_SLIDE_TIMER,10,0);				
			}
			
		}
		else
		{
			if(m_pMidImage != NULL)
			{		
				m_bDrawMiddle = TRUE;	
				::SetTimer(this->GetSafeHwnd(),ID_SLIDE_TIMER,10,0);				
			}
		}
	}	
}

//Hongliang 2010-04-01 flip on/off simulate
void CMSDevDlg::SetFlipState( BOOL bOn )
{
	if( m_bFlipOn != bOn )
	{
		m_bFlipOn = bOn;

		if( bOn )
		{
			m_strBmpPath = ModifyPath( m_strOriBmpPath,"flip" );
			m_strIniPath = ModifyPath( m_strOriIniPath,"flip" );
		}
		else
		{
			m_strBmpPath = m_strOriBmpPath;
			m_strIniPath = m_strOriIniPath;
		}

		ChangeHandsetImage();
	}
	
}

void CMSDevDlg::OnCopyScreen()
{
	if ( !OpenClipboard() )
	{
		AfxMessageBox( "Cannot open the Clipboard" );
		return;
	}
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
	{
		AfxMessageBox( "Cannot empty the Clipboard" );
		return;
	}

	CClientDC cdc(this);

	CBitmap* bmMemSrc = CBitmap::FromHandle( m_hMainLCD );
	CDC     dcMemSrc;
	dcMemSrc.CreateCompatibleDC(&cdc);
	dcMemSrc.SelectObject( bmMemSrc );

	// Get the currently selected data
	CBitmap *pjunk = new CBitmap();
	
	CDC dcmem;
	dcmem.CreateCompatibleDC(&cdc);
	pjunk->CreateCompatibleBitmap(&cdc,m_rcLCD.Width(),m_rcLCD.Height());
	dcmem.SelectObject(pjunk);
	dcmem.BitBlt(0, 0, m_rcLCD.Width(),m_rcLCD.Height(), &dcMemSrc, 0, 0, SRCCOPY);

	// For the appropriate data formats...
	if ( ::SetClipboardData( CF_BITMAP, pjunk->m_hObject ) == NULL )
	{
		AfxMessageBox( "Unable to set Clipboard data" );
		CloseClipboard();
		return;
	}
	// ...
	CloseClipboard();	

	delete pjunk;
}

void CMSDevDlg::OnShowControlPanel()
{
#ifdef _CONTROL_PANEL
	m_bShowCtrlPanel = !m_bShowCtrlPanel;
	ShowControlPanel(m_bShowCtrlPanel);
#endif
}

void CMSDevDlg::OnShowAtWindow()
{
    if (m_pAtCommandDlg->GetSafeHwnd() == NULL)
    {
        m_pAtCommandDlg->Create(IDD_AT);

        CRect rcMainWnd;
        AfxGetMainWnd()->GetWindowRect(&rcMainWnd);
        m_pAtCommandDlg->SetWindowPos(NULL, rcMainWnd.right, rcMainWnd.top, 0, 0, SWP_NOSIZE);

        m_pAtCommandDlg->ShowWindow(SW_SHOW);
        SetFocus();
    }
}
//弹出串口设置对话框
void CMSDevDlg::OnComSettings()
{
    CComSettings dlg;
	int code = dlg.DoModal();
	if(OPEN_PORT == code)//open
	{
        openPort();
		//return;
	}
	else if(CLOSE_PORT == code)
	{
		closePort();
		//return;
	}
}

void CMSDevDlg::OnUpdateShowControlPanel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

    pCmdUI->SetCheck(m_bShowCtrlPanel);
}

void CMSDevDlg::OnUpdateShowAtWindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable(!m_pMonitorDlg->GetSafeHwnd());
}

void CMSDevDlg::OnTestItems( UINT nID )
{
	if((int)(nID-ID_TEST1)>=0 && (int)(nID-ID_TEST1)<m_aTestItemState.GetSize())
	{
		m_aTestItemState[nID-ID_TEST1] = !m_aTestItemState[nID-ID_TEST1];
	}

	BOOL bGPIO = FALSE;
	int nKeyCode = GetTestKeyCodeByCommand(nID,bGPIO);
	if(nKeyCode != -1)
	{
		if(!bGPIO)
		{
			MSK_SendKeycodeToRTOS( nKeyCode,KPD_DOWN );
			MSK_SendKeycodeToRTOS( nKeyCode,KPD_UP );
		}
		else
		{
			if(m_aTestItemState[nID-ID_TEST1])
			{
				MSK_SendKeycodeToRTOS( nKeyCode,KPD_DOWN );
			}
			else
			{
				MSK_SendKeycodeToRTOS( nKeyCode,KPD_UP );
			}
		}	
	}

}

void CMSDevDlg::OnUpdateTestItems( CCmdUI* pCmdUI)
{
	int nID = pCmdUI->m_nID;
	BOOL bGPIO = FALSE;
	int nKeyCode = GetTestKeyCodeByCommand(nID,bGPIO);
    pCmdUI->Enable( !MSK_IsPowerOff() && nKeyCode!=-1);

	if(nKeyCode!=-1 && bGPIO)
	{		
		pCmdUI->SetCheck( m_aTestItemState[nID-ID_TEST1] );			
	}
    
}

int CMSDevDlg::GetTestKeyCodeByCommand(UINT nID,BOOL &bGPIO)
{
	CStringArray aTestName;
	m_IniFile.GetTestNames(aTestName);
	if((int)(nID-ID_TEST1)>=0 && (int)(nID-ID_TEST1)<aTestName.GetSize())
	{
		CString strCodeName = aTestName[nID-ID_TEST1];
		strCodeName.Insert(0,_T("TEST_"));
		return m_IniFile.GetKeyCodeByName(strCodeName,&bGPIO);
	}
	else
	{
		return -1;
	}
}


int CMSDevDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CImgDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CMSDevDlg::InitLCD()
{	
	//main LCD
	{
		SAFE_DEL_OBJ(m_hMainLCD);
		BYTE *pBuf = new BYTE[sizeof(BITMAPINFOHEADER)+3*sizeof(DWORD)];
		memset(pBuf,0,sizeof(BITMAPINFOHEADER)+3*sizeof(DWORD));
		BITMAPINFOHEADER *bmih = (PBITMAPINFOHEADER)pBuf;
		bmih->biHeight = -m_rcLCD.Height();
		bmih->biWidth  = m_rcLCD.Width();
		bmih->biBitCount = X_LCD_BIT_COUNT;
		bmih->biPlanes = 1;
		bmih->biSize = sizeof(BITMAPINFOHEADER);
		bmih->biCompression = BI_BITFIELDS;
		bmih->biClrUsed = 3;
		int biWidthBytes = GET_WIDTH_BYTE(bmih->biWidth,bmih->biBitCount);
		bmih->biSizeImage = -biWidthBytes*bmih->biHeight;	
		DWORD *pMask = (DWORD*)(pBuf+sizeof(BITMAPINFOHEADER));
		*(pMask) =   0xf800;
		*(pMask+1) = 0x07e0;
		*(pMask+2) = 0x001f;
		m_hMainLCD = ::CreateDIBSection(NULL,(BITMAPINFO*)pBuf,DIB_RGB_COLORS,(LPVOID *)&m_pMainLCDBuf,0,0);

		delete [] pBuf;
/*		BITMAPINFO bmi = {0};
		BITMAPINFOHEADER &bmih = bmi.bmiHeader;
		bmih.biHeight = -m_rcLCD.Height();
		bmih.biWidth  = m_rcLCD.Width();
		bmih.biBitCount = X_LCD_BIT_COUNT;
		bmih.biPlanes = 1;
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biCompression = BI_RGB;
		int biWidthBytes = GET_WIDTH_BYTE(bmih.biWidth,bmih.biBitCount);
		bmih.biSizeImage = biWidthBytes*bmih.biHeight;	
		m_hMainLCD = ::CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,(LPVOID *)&m_pMainLCDBuf,0,0);*/
	}

	//Sub LCD
	if(m_bDualLCD)
	{
		// sub LCD uses 24bit DIB to display, and not use 16bit DIB.
		// DIB only support 16-555 or 16-565, and not support 16-444 or other customer color mask.
		// so GUI must change 16-444 to 24.
		SAFE_DEL_OBJ(m_hSubLCD);
		BITMAPINFO bmi = {0};
		BITMAPINFOHEADER &bmih = bmi.bmiHeader;
		bmih.biHeight = -m_rcSubLCD.Height(); // top-bottom
		bmih.biWidth  = m_rcSubLCD.Width();
		bmih.biBitCount = X_SUB_LCD_BIT_COUNT;
		bmih.biPlanes = 1;
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		int biWidthBytes = GET_WIDTH_BYTE(bmih.biWidth,bmih.biBitCount);
		bmih.biSizeImage = -biWidthBytes*bmih.biHeight;
		m_hSubLCD = ::CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,(LPVOID *)&m_pSubLCDBuf,0,0);
	}
}


void CMSDevDlg::RotateHandset(int nOldRotateType)
{
	int nRotateType = m_nRotateType;
	BOOL bReset = FALSE;
	if( m_nRotateType == Screen_Original )
	{
		bReset = TRUE;
		m_nRotateSlideXOffset = m_nNormalSlideXOffset;
		m_nRotateSlideYOffset = m_nNormalSlideYOffset;

		if(nOldRotateType == Screen_Original)
		{
			return;
		}
		else if( nOldRotateType == Screen_r90 )
		{
			nRotateType = Screen_r270;
		}
		else if( nOldRotateType == Screen_r180 )
		{
			nRotateType = Screen_r180;
		}
		else if( nOldRotateType == Screen_r270 )
		{
			nRotateType = Screen_r90;
		}		
	}

	int nAngle = 0;
	int nSrcW = m_cxImage;
	int nSrcH = m_cyImage;

	if( nRotateType == Screen_r90 )
	{
		m_pBGImage->RotateFlip(Rotate90FlipNone);	
		if(m_pMidImage)
		{
			m_pMidImage->RotateFlip(Rotate90FlipNone);
			if(!bReset)
			{
				m_nRotateSlideXOffset = -m_nNormalSlideYOffset;
				m_nRotateSlideYOffset = m_nNormalSlideXOffset;
			}
	
		}
		nAngle = 90;
	}
	else if( nRotateType == Screen_r180 )
	{
		m_pBGImage->RotateFlip(Rotate180FlipNone);		
		if(m_pMidImage)
		{
			m_pMidImage->RotateFlip(Rotate180FlipNone);
			if(!bReset)
			{
				m_nRotateSlideXOffset = m_nNormalSlideXOffset;
				m_nRotateSlideYOffset = -m_nNormalSlideYOffset;
			}
		}
		nAngle = 180;
	}
	else if( nRotateType == Screen_r270 )
	{
		m_pBGImage->RotateFlip(Rotate270FlipNone);
		if(m_pMidImage)
		{
			m_pMidImage->RotateFlip(Rotate270FlipNone);
			if(!bReset)
			{
				m_nRotateSlideXOffset = m_nNormalSlideYOffset;
				m_nRotateSlideYOffset = m_nNormalSlideXOffset;
			}
		}
		nAngle = 270;
	}
	m_cxImage = m_pBGImage->GetWidth();
	m_cyImage = m_pBGImage->GetHeight();

	// Rotate 
	m_IniFile.Rotate(nAngle,nSrcW,nSrcH);

	// Tooltips
	KEYARRAY *paKeys = m_IniFile.GetKeys();
    int nKeyCount = paKeys->GetSize();
	for ( int i = 0; i < nKeyCount; i++ )
    {
		m_toolTip.SetToolRect(this,i+1,paKeys->GetAt(i).rcKeyRect);
    }

	m_rcLCD = m_IniFile.GetLCDRect();
	m_rcSubLCD = m_IniFile.GetSubLCDRect();

	InitLCD();

	if(m_hBmpMem)
	{
		SAFE_DEL_OBJ(m_hBmpMem);
	}

	BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
	int nBytesPerLine = ((m_cxImage* 32 + 31) & (~31)) >> 3;
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
	stBmpInfoHeader.biWidth = m_cxImage;   
	stBmpInfoHeader.biHeight = m_cyImage;   
	stBmpInfoHeader.biPlanes = 1;
	stBmpInfoHeader.biBitCount = 32;   
	stBmpInfoHeader.biCompression = BI_RGB;   
	stBmpInfoHeader.biClrUsed = 0;   
	stBmpInfoHeader.biSizeImage = nBytesPerLine * m_cyImage;
	
	//PUINT32 pvBits = NULL;  
	
	m_dwBGBufSize = nBytesPerLine * m_cyImage;

	m_hBmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, (LPVOID*)&m_pBGBuf, NULL, 0);

}

void CMSDevDlg::OnSlidePhone( WPARAM wParam,LPARAM lParam )
{
	SetSlideState((BOOL)wParam);
}

void CMSDevDlg::OnAbout() 
{
	m_bBlock = TRUE;
	CXAboutDlg dlg;
	dlg.DoModal();
	m_bBlock = FALSE;
}

void CMSDevDlg::OnUpdateChangeImage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_nRotateType == Screen_Original)?TRUE:FALSE);		
}

void CMSDevDlg::InitDCView()
{
	USES_CONVERSION;
	m_rcCameraDisp.CopyRect(m_rcLCD);
	// load DC View gif
	CString strPath = ((CMSDevApp *)AfxGetApp())->GetCurrentPathName();
	CString strDCPath = strPath+_T("\\res\\dc_sample.gif");
	CString strDVPath = strPath+_T("\\res\\dv_sample.avi");
	CFileFind finder;
	if(finder.FindFile(strDCPath))
	{
#if defined (_UNICODE) || defined(UNICODE) 
		m_pImgDCView = Image::FromFile(strDCPath);
#else
		m_pImgDCView = Image::FromFile(A2W((LPSTR)(LPCTSTR)strDCPath));	
		
#endif
	}

	if(m_pImgDCView == NULL)
	{
		return;
	}
	
	UINT nCount = 0;
	nCount = m_pImgDCView->GetFrameDimensionsCount();
	GUID* pDimensionIDs = new GUID[nCount];
	
	// Get the list of frame dimensions from the Image object.
	m_pImgDCView->GetFrameDimensionsList(pDimensionIDs, nCount);
	
	// Get the number of frames in the first dimension.
	m_nDCVFrameCount = m_pImgDCView->GetFrameCount(&pDimensionIDs[0]);
	
	// Assume that the image has a property item of type PropertyItemEquipMake.
	// Get the size of that property item.
	int nSize = m_pImgDCView->GetPropertyItemSize(PropertyTagFrameDelay);
	
	// Allocate a buffer to receive the property item.
	PropertyItem* pPropertyItem = (PropertyItem*) malloc(nSize);	
	m_pImgDCView->GetPropertyItem(PropertyTagFrameDelay, nSize, pPropertyItem);	
	delete  []pDimensionIDs;

	m_nInterTime = ((long*) pPropertyItem->value)[0] * 10;
	if(m_nInterTime == 0)
	{
		m_nInterTime = 100;
	}
	free(pPropertyItem);

	FILE *pFile = _tfopen(strDVPath,_T("rb"));
	if(pFile != NULL)
	{
		fseek(pFile,0,SEEK_END);
		m_dwDVAviSize = ftell(pFile);
		fseek(pFile,0,SEEK_SET);
		m_pDVAviBuf = new BYTE[m_dwDVAviSize];
		fread(pFile,1,m_dwDVAviSize,pFile);
		fclose(pFile);
	}


}

void CMSDevDlg::SetNextFrame()
{
	if(m_pImgDCView == NULL || m_nDCVFrameCount <= 1)
		return;

	GUID   pageGuid = FrameDimensionTime;

	Status s = m_pImgDCView->SelectActiveFrame(&pageGuid, m_nDCVFramePos++);
	
	if (m_nDCVFramePos == m_nDCVFrameCount)
	{
		m_nDCVFramePos = 0;
	}
}

void CMSDevDlg::OnCameraOpen( WPARAM wParam,LPARAM lParam )
{	
	m_bEndRecord = FALSE;
	m_bSnapShot = FALSE;
	m_bCameraOpen = TRUE;

	CRect rect;
	rect.left = HIWORD((DWORD)wParam) + m_rcLCD.left;
	rect.top  = LOWORD((DWORD)wParam) + m_rcLCD.top;
	rect.right = rect.left + HIWORD((DWORD)lParam); 
	rect.bottom = rect.top + LOWORD((DWORD)lParam); 
	if(rect.right > m_rcLCD.right || rect.bottom > m_rcLCD.bottom || rect.Width() == 0 || rect.Height() == 0)
	{
		m_rcCameraDisp.CopyRect(&m_rcLCD);
	}
	else
	{
		m_rcCameraDisp.CopyRect(&rect);
	}

	SetTimer(ID_DCVIEW_TIMER,m_nInterTime,0);
}

void CMSDevDlg::OnCameraClose( WPARAM wParam,LPARAM lParam )
{
	m_bCameraOpen = FALSE;
	m_rcCameraDisp.CopyRect(&m_rcLCD);
	KillTimer(ID_DCVIEW_TIMER);
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)  
{  
	UINT num  = 0;    // number   of   image   encoders  
	UINT size =  0;   // size   of   the   image   encoder   array   in   bytes  
	
	ImageCodecInfo*   pImageCodecInfo   =   NULL;  	
	GetImageEncodersSize(&num,   &size);  
	if(size == 0)  
		return -1;    // Failure
	
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));  
	if(pImageCodecInfo == NULL)  
		return -1;    // Failure  
	
	GetImageEncoders(num, size, pImageCodecInfo);  
	
	for(UINT j = 0; j < num; ++j)  
	{  
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )  
		{  
			*pClsid   =   pImageCodecInfo[j].Clsid;  
			free(pImageCodecInfo);  
			return j;  // Success  
		}          
	}  
	
	free(pImageCodecInfo);  
	return -1; // Failure  
}  

void CMSDevDlg::DoSnapshot( void ** ppBuf, DWORD *pLength )
{
	USES_CONVERSION;
	m_bSnapShot = TRUE;
	if(m_pImgDCView != NULL)
	{
		CLSID  encoderClsid;  
		WCHAR  strGuid[39];  
		if(GetEncoderClsid(L"image/jpeg", &encoderClsid) >= 0) 
		{
			StringFromGUID2(encoderClsid, strGuid, 39);  
			WCHAR szPath[_MAX_PATH] = {0};
			WCHAR *pPath = _wgetenv(L"tmp");
			wcscpy(szPath,pPath);
			swprintf(szPath,L"%s\\_mssim_dc.jpg",pPath);
			m_pImgDCView->Save(szPath,&encoderClsid,NULL);  

			FILE *pFile = _wfopen(szPath,L"rb");
			if(pFile != NULL)
			{
				fseek(pFile,0,SEEK_END);
				DWORD dwSize = ftell(pFile);
				fseek(pFile,0,SEEK_SET);
				BYTE *pBuf = new BYTE[dwSize];
				fread(pBuf,1,dwSize,pFile);
				fclose(pFile);
				*ppBuf = pBuf;
				*pLength = dwSize;
				m_aDCPhotos.Add((LPVOID)pBuf);
			}
			::DeleteFileW(szPath);			
		}
	}
}
void  CMSDevDlg::DoRecord( void ** ppBuf, DWORD *pLength,BOOL bEnd )
{	
	m_bSnapShot = FALSE;
	m_bEndRecord = bEnd;	

	*ppBuf = m_pDVAviBuf;
	*pLength = m_dwDVAviSize;
	
}


void CMSDevDlg::OnShowLayerInfo() 
{
	// TODO: Add your command handler code here
	m_dlgLayers.ShowWindow( SW_SHOW );
	m_dlgLayers.m_bShow = TRUE;
}

void CMSDevDlg::OnUpdateShowLayerInfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMSDevDlg::InitMMILayer()
{
	ClearMMILayer();

	memset(m_layers,0,sizeof(m_layers));
	for(int i = 0; i< MMI_MAX_LAYER_NUM; i++)
	{
		LPBYTE pBuf = new BYTE[1024*1024*4];
		m_layers[i].layer_info.buf_addr = (uint32)pBuf;
	}

	m_dlgLayers.Init(m_layers,m_rcLCD.Height(),m_rcLCD.Width());

}

void CMSDevDlg::ClearMMILayer()
{
	for(int i = 0; i< MMI_MAX_LAYER_NUM; i++)
	{
		if(m_layers[i].layer_info.buf_addr != 0)
		{
			delete [] (LPBYTE)m_layers[i].layer_info.buf_addr;
		}
	}
	memset(m_layers,0,sizeof(m_layers));	
}

BOOL CMSDevDlg::GetLayerInfo(BYTE lcdid,void** lpInfo, DWORD *pMaxNum)
{	
	if(!m_dlgLayers.m_bShow)
	{
		return FALSE;
	}
	if(lcdid == 0)
	{
		*lpInfo = (void*)m_layers;
		*pMaxNum = MMI_MAX_LAYER_NUM;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CMSDevDlg::InvalidateLayerInfo()
{
	if(!m_dlgLayers.m_bShow)
	{
		return;
	}

	m_dlgLayers.LayerInvalidate();
}

void CMSDevDlg::SetVibrate(BOOL bOn)
{
	::KillTimer(this->GetSafeHwnd(),ID_VIBRATE_TIMER);

	if(bOn)
	{
		::SetTimer(this->GetSafeHwnd(),ID_VIBRATE_TIMER,10,0);
	}	
}

BOOL CMSDevDlg::IsTraceOn()
{
	return m_bTraceOn;
}


void CMSDevDlg::OnTraceOn() 
{
	// TODO: Add your command handler code here
	m_bTraceOn = !m_bTraceOn;
	((CMSDevApp *)AfxGetApp())->SaveTraceState(m_bTraceOn);
}

void CMSDevDlg::OnUpdateTraceOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bTraceOn);
}
//打开串口
BOOL CMSDevDlg::openPort()
{
	CString str;
    GetPrivateProfileString("com settings","com","1", str.GetBuffer(MAX_PATH),MAX_PATH,".\\comsettings.ini");
    int port = atoi(str.GetBuffer(str.GetLength()));
    GetPrivateProfileString("com settings","baud","9600", str.GetBuffer(MAX_PATH),MAX_PATH,".\\comsettings.ini");
    int baud = atoi(str.GetBuffer(str.GetLength()));
	if(com.InitPort(this,port,baud))
	{
		AfxMessageBox( "打开串口成功" );
		if(com.StartMonitoring())
		{
                     CString strText;
                     strText.Format("监听串口成功,端口%d,波特率%d",port,baud);
			AfxMessageBox(strText);
			return TRUE;
		}
		AfxMessageBox("监听串口失败");
		com.ClosePort();
		return FALSE;
	}
	AfxMessageBox("打开串口失败");
	return FALSE;
}
//关闭串口
void CMSDevDlg::closePort()
{
	com.ClosePort();
	AfxMessageBox("关闭串口");
}
//向串口发送数据
void CMSDevDlg::sendData(char *data)
{
    if(NULL == com.m_hComm)
    {
        return;
    }
	FILE *fp;
	fp = fopen("comlog.txt","a");
	if(NULL != fp)
	{
		fseek(fp,0,SEEK_END);
		fprintf(fp,"rcvData[%d]<--%s\n",com_data_count_r++,data);
	}
	fclose(fp);
	com.WriteToPort(data,strlen(data));
	Sleep(SEND_SLEEP_TIME);
	//logOut(TRUE,CString(at_head).Left(20));
}

LONG CMSDevDlg::reciveFromPort(WPARAM ch, LPARAM port)
{
	CString str;
    GetPrivateProfileString("com settings","com","1", str.GetBuffer(MAX_PATH),MAX_PATH,".\\comsettings.ini");
    int serno = atoi(str.GetBuffer(str.GetLength()));
    if(serno == port)
    {
        char c = char(ch);
		FILE *fp;
		fp = fopen("comlog.txt","a");
		if(NULL != fp)
		{
			fseek(fp,0,SEEK_END);
			fprintf(fp,"rcvData[%.4d]-->%x\n",com_data_count++,c);//把从串口收到的数据写入到日志文件
		}
		fclose(fp);
#ifdef SINGLE_CHAR_SEND//每次从串口接收一个字节的情况
        sendChar2Rtos(c);
		Sleep(RECIVE_SLEEP_TIME);
	 return 0;
#else//每次从串口接收一包数据的情况（以0xd和0xa结尾）
        if(0xa == c)
        {
			int len = strcmd.GetLength();
			if(strcmd.GetAt(len - 1) == 0xd)
			{
                strcmd += CString(c);
                sendStr2Rtos(strcmd);
				//AfxMessageBox(strcmd);
				strcmd = "";
			    return 0;
			}		
		}
		strcmd += CString(c);
#endif
    }
    return 0;

}
//向rtos转发串口数据，每次发一个字节
void CMSDevDlg::sendChar2Rtos(char ch)
{
    CString strCommand;
	//发送0x7e或0x7d时会有问题，需要使用转义字符
    if(0x7e == ch)
    {
     char a[2] = {0x7d,0x5e};
	 strCommand = CString(a[0]);
	 strCommand += CString(a[1]);
    }
	else if(0x7d == ch)
	{
		 char a[2] = {0x7d,0x5d};
	     strCommand = CString(a[0]);
	     strCommand += CString(a[1]);
	}
	else
	{
        strCommand = CString(ch);
	}
    unsigned int    nCommandLength  = strCommand.GetLength();
    BYTE            *request        = new BYTE[nCommandLength + PROTOCOL_HEADER_SIZE + 2];
    MSG_HEAD_T      MsgHeader       = {0};

    memset(request, FLAG_BYTE, nCommandLength + PROTOCOL_HEADER_SIZE + 2);

    MsgHeader.len       = nCommandLength + sizeof (MSG_HEAD_T);
    MsgHeader.type      = COM_DATA_R;//更新了消息类型与之前CM模块收的消息区分开
    MsgHeader.subtype   = 0;

    memcpy (request+1, &MsgHeader, sizeof (MSG_HEAD_T));
    memcpy (request+1+sizeof (MSG_HEAD_T), strCommand, nCommandLength);

    MSK_SendMsgToRTOS(request, nCommandLength + sizeof (MSG_HEAD_T)+2);
    
    delete[] request;
}

//向rtos转发串口数据，每次发一包数据
void CMSDevDlg::sendStr2Rtos(CString strCommand)
{
    unsigned int    nCommandLength  = strCommand.GetLength();
    BYTE            *request        = new BYTE[nCommandLength + PROTOCOL_HEADER_SIZE + 2];
    MSG_HEAD_T      MsgHeader       = {0};

    memset(request, FLAG_BYTE, nCommandLength + PROTOCOL_HEADER_SIZE + 2);

    MsgHeader.len       = nCommandLength + sizeof (MSG_HEAD_T);
    MsgHeader.type      = COM_DATA_R;//更新了消息类型与之前CM模块收的消息区分开
    MsgHeader.subtype   = 0;

    memcpy (request+1, &MsgHeader, sizeof (MSG_HEAD_T));
    memcpy (request+1+sizeof (MSG_HEAD_T), strCommand, nCommandLength);

    MSK_SendMsgToRTOS(request, nCommandLength + sizeof (MSG_HEAD_T)+2);
    
    delete[] request;
}
