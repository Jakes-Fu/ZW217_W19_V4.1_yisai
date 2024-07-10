// ImgView.cpp : implementation file
//

#include "stdafx.h"
#include "ResOver.h"
#include "ImgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//DllExports::GdipFree(p);
#define GDIP_FREE(p) do{\
	if(p)\
	{\
		delete p;\
		p=NULL;\
	}\
}while(0)

#define SAFE_DEL_OBJ(h) do{\
	if(h)\
	{\
		::DeleteObject(h);\
		h=NULL;\
	}\
}while(0)


#define ANIM_TIMER_ENVENT  (1000)
#define GIF_TIMER_ENVENT   (1001)
#define COLOR_WIDTH        (100)
#define COLOR_HEIGHT       (100)
/////////////////////////////////////////////////////////////////////////////
// CImgView

CImgView::CImgView()
{
	m_pImage = NULL;
	m_cx = 800;
	m_cy = 800;
	m_hBmpMem = NULL;
	m_pBGBuf = NULL;
	m_dwBGBufSize = 0;
	m_pBGImage = NULL;
	m_pRingImage = NULL;
	m_bColor = FALSE;
	m_dwColor = 0;
	m_bDrawImageBG = TRUE;
	m_nGifFrameNum=0;
	m_nInterTime=0;
	m_nGifFramePos=0;
	Init();
}

CImgView::~CImgView()
{
/*lint -save -e1551 */
	GDIP_FREE(m_pImage);
	GDIP_FREE(m_pBGImage);
	GDIP_FREE(m_pRingImage);
	SAFE_DEL_OBJ(m_hBmpMem);
	m_pBGBuf = NULL;
	m_dwBGBufSize = 0;
	m_wndNtac.DestroyWindow();
/*lint -restore */
}


BEGIN_MESSAGE_MAP(CImgView, CStatic)
	//{{AFX_MSG_MAP(CImgView)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgView message handlers

void CImgView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
 	CRect rect;
 	GetClientRect(&rect);
// 	rect.right -= 1;
//  	rect.bottom -= 1;
//  	CBrush br(RGB(0xFF,0xFF,0xFF));
//  	dc.FillRect(rect,&br);
	
	// Do not call CStatic::OnPaint() for painting messages
	if(m_pImage != NULL || m_pBGImage != NULL)
	{
		HDC hdcTemp = dc.GetSafeHdc();
		//HDC hdcMemory = CreateCompatibleDC(hdcTemp);
		//if(m_hBmpMem)
		//{
		//	memset(m_pBGBuf,0,m_dwBGBufSize);
		//	HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, m_hBmpMem); 			

			Graphics graph(hdcTemp);
			graph.SetSmoothingMode(SmoothingModeNone);

			if(m_bDrawImageBG && m_pBGImage != NULL)
			{
				TextureBrush tbr(m_pBGImage);
				graph.FillRectangle(&tbr,0,0,rect.Width(),rect.Height());			
			}
			else
			{
				Color colorbg(0xFFA0A0A0);
				SolidBrush bs(colorbg);
				graph.FillRectangle(&bs,0,0,rect.Width(),rect.Height());	
			}

			if(m_pImage != NULL)
			{
				int x = rect.Width()/2 - (int)m_pImage->GetWidth()/2;
				int y = rect.Height()/2 - (int)m_pImage->GetHeight()/2;
				if( x< 0)
					x = 0;
				if(y< 0)
					y = 0;
				graph.DrawImage(m_pImage,x,y,m_pImage->GetWidth(),m_pImage->GetHeight());
			}

			if(m_bColor)
			{
				int x = rect.Width()/2 - (int)COLOR_WIDTH/2;
				int y = rect.Height()/2 - (int)COLOR_HEIGHT/2;
				if( x< 0)
					x = 0;
				if(y< 0)
					y = 0;
				Color color(m_dwColor);
		        SolidBrush sbs(color);
			    graph.FillRectangle(&sbs,x,y,COLOR_WIDTH,COLOR_HEIGHT);
			}


			graph.ReleaseHDC(hdcTemp);

			//::SelectObject( hdcMemory, hbmpOld);  
		//}

		//DeleteDC(hdcMemory);
		//hdcMemory=NULL;
		DeleteDC(hdcTemp);
		hdcTemp=NULL;
	}
}

void CImgView::UpdateResItem(RES_ITEM_PTR pRI,int nThemeIndex)
{
	m_bColor = FALSE;
	if(m_pImage != m_pRingImage)
	{
		GDIP_FREE(m_pImage);
	}

	m_pImage = NULL;

	m_wndNtac.ShowWindow(SW_HIDE);	
	m_wndBkmk.ShowWindow(SW_HIDE);	
	m_wndMtva.ShowWindow(SW_HIDE);
	
	KillTimer(ANIM_TIMER_ENVENT);
	KillTimer(GIF_TIMER_ENVENT);

	if(pRI == NULL)
	{
		Invalidate();
		return;	
	}

	if(pRI->uResType == MMI_RT_IMAGE)
	{
		WORD wItemType = 0;
		CString strImgFile;
		if(pRI->itemInfo[nThemeIndex].wModified == 1)
		{
			strImgFile = pRI->itemInfo[nThemeIndex].rep.szFile;
			wItemType  = pRI->itemInfo[nThemeIndex].rep.wItemType;
		}
		else
		{
			strImgFile = pRI->itemInfo[nThemeIndex].org.szFile;
			wItemType  = pRI->itemInfo[nThemeIndex].org.wItemType;
		}

		CFileFind finder;
		if(!finder.FindFile(strImgFile))
		{
			Invalidate();
			return;	
		}
		
	
	
#if defined (_UNICODE) || defined(UNICODE) 
		m_pImage = Image::FromFile(strImgFile.operator LPCTSTR());
#else
		m_pImage = Image::FromFile(A2W((LPSTR)strImgFile.operator LPCTSTR()));			
#endif
		if(m_pImage->GetLastStatus() != 0)
		{
			GDIP_FREE(m_pImage);
		}
		else
		{
			if(wItemType == IMG_GIF)
			{
				UINT nCount = 0;
				nCount = m_pImage->GetFrameDimensionsCount();
				GUID* pDimensionIDs = new GUID[nCount];
				
				// Get the list of frame dimensions from the Image object.
				m_pImage->GetFrameDimensionsList(pDimensionIDs, nCount);
				
				// Get the number of frames in the first dimension.
				m_nGifFrameNum = m_pImage->GetFrameCount(&pDimensionIDs[0]);				
				delete  []pDimensionIDs;

				// Assume that the image has a property item of type PropertyItemEquipMake.
				// Get the size of that property item.
				int nSize = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
				
				// Allocate a buffer to receive the property item.
				PropertyItem* pPropertyItem = (PropertyItem*) malloc(nSize);
				if(pPropertyItem != NULL)
				{
					m_pImage->GetPropertyItem(PropertyTagFrameDelay, nSize, pPropertyItem);	
					
					m_nInterTime = ((long*) pPropertyItem->value)[0] * 10;				
					free(pPropertyItem);
				}

				if(m_nInterTime == 0)
				{
					m_nInterTime = 300;
				}

				SetTimer(GIF_TIMER_ENVENT,m_nInterTime,NULL);
			}
		}
		
	}
	else if(pRI->uResType == MMI_RT_ANIM)
	{
		KillTimer(ANIM_TIMER_ENVENT);

		WORD wCmpType = 0;
		CString strImgFile;
		if(pRI->itemInfo[nThemeIndex].wModified == 1)
		{
			strImgFile = pRI->itemInfo[nThemeIndex].rep.szFile;
			wCmpType  = pRI->itemInfo[nThemeIndex].rep.wCmpType;
		}
		else
		{
			strImgFile = pRI->itemInfo[nThemeIndex].org.szFile;
			wCmpType  = pRI->itemInfo[nThemeIndex].org.wCmpType;
		}

		CFileFind finder;
		if(!finder.FindFile(strImgFile))
		{
			Invalidate();
			return;	
		}
		
		GetAnimFrameFile(strImgFile,wCmpType);
		if(m_agAnimFrameFile.GetSize() > 0)
		{

#if defined (_UNICODE) || defined(UNICODE) 
			m_pImage = Image::FromFile(m_agAnimFrameFile.GetAt(0).operator LPCTSTR());
#else
			m_pImage = Image::FromFile(A2W((LPSTR)m_agAnimFrameFile.GetAt(0).operator LPCTSTR()));			
#endif
			if(m_pImage->GetLastStatus() != 0)
			{
				GDIP_FREE(m_pImage);
			}

			SetTimer(ANIM_TIMER_ENVENT,300,NULL);
		}
	}
	else if(pRI->uResType == MMI_RT_RING)
	{
		m_pImage = m_pRingImage;
	}
	else if(pRI->uResType == MMI_RT_COLOR)
	{
		DWORD dwColor=0;
		if(pRI->itemInfo[nThemeIndex].wModified == 1)
		{
			dwColor  = pRI->itemInfo[nThemeIndex].rep.dwColor;
		}
		else
		{
			dwColor =  pRI->itemInfo[nThemeIndex].org.dwColor;
		}

		dwColor =g_theApp.Color565To24(dwColor);

		m_dwColor = Color::MakeARGB(0xFF,GetRValue(dwColor),GetGValue(dwColor),GetBValue(dwColor));
	
		m_bColor = TRUE;
	}
	else if(pRI->uResType == MMI_RT_OTHER)
	{
		if(_tcsicmp(pRI->szDesc,_T("R_NETWORK_ACCOUNT")) == 0)
		{	
			if(pRI->itemInfo[nThemeIndex].wModified == 1)
			{
				m_wndNtac.SetBuf(pRI->itemInfo[nThemeIndex].rep.pBuf,pRI->itemInfo[nThemeIndex].rep.dwSize,NULL);
			}
			else
			{
				m_wndNtac.SetBuf(pRI->itemInfo[nThemeIndex].org.pBuf,pRI->itemInfo[nThemeIndex].org.dwSize,NULL);
			}
				
			m_wndNtac.ShowWindow(SW_SHOW);			
		}
		else if(_tcsicmp(pRI->szDesc,_T("R_MULIT_VARIANT")) == 0)
		{	
			if(pRI->itemInfo[nThemeIndex].wModified == 1)
			{
				m_wndMtva.SetBuf(pRI->itemInfo[nThemeIndex].rep.pBuf,pRI->itemInfo[nThemeIndex].rep.dwSize,NULL);
			}
			else
			{
				m_wndMtva.SetBuf(pRI->itemInfo[nThemeIndex].org.pBuf,pRI->itemInfo[nThemeIndex].org.dwSize,NULL);
			}
				
			m_wndMtva.ShowWindow(SW_SHOW);			
		}
		else if(_tcsicmp(pRI->szDesc,_T("R_NETWORK_BOOKMARK")) == 0)
		{
			if(pRI->itemInfo[nThemeIndex].wModified == 1)
			{
				m_wndBkmk.SetBuf(pRI->itemInfo[nThemeIndex].rep.pBuf,pRI->itemInfo[nThemeIndex].rep.dwSize,NULL);
			}
			else
			{
				m_wndBkmk.SetBuf(pRI->itemInfo[nThemeIndex].org.pBuf,pRI->itemInfo[nThemeIndex].org.dwSize,NULL);
			}	
			m_wndBkmk.ShowWindow(SW_SHOW);	
		}
	}

	Invalidate();
}

void CImgView::SetNextFrame()
{
	if(m_pImage == NULL || m_nGifFrameNum <= 1)
		return;

	GUID   pageGuid = FrameDimensionTime;

	Status s = m_pImage->SelectActiveFrame(&pageGuid, m_nGifFramePos++);
	UNUSED_ALWAYS(s);
	if (m_nGifFramePos == m_nGifFrameNum)
	{
		m_nGifFramePos = 0;
	}
}

void CImgView::Init()
{
	if(m_hBmpMem)
	{
		SAFE_DEL_OBJ(m_hBmpMem);
	}

	BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
	int nBytesPerLine = ((m_cx * 32 + 31) & (~31)) >> 3;
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
	stBmpInfoHeader.biWidth = m_cx;   
	stBmpInfoHeader.biHeight = m_cy;   
	stBmpInfoHeader.biPlanes = 1;
	stBmpInfoHeader.biBitCount = 32;   
	stBmpInfoHeader.biCompression = BI_RGB;   
	stBmpInfoHeader.biClrUsed = 0;   
	stBmpInfoHeader.biSizeImage = nBytesPerLine * m_cy;
	
	//PUINT32 pvBits = NULL;  
	
	m_dwBGBufSize = nBytesPerLine * m_cy;

	m_hBmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, (LPVOID*)&m_pBGBuf, NULL, 0);

	HBITMAP hBitmap = ::LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_BMP_BKG) );
	m_pBGImage = (Image*)Bitmap::FromHBITMAP(hBitmap, 0);
	::DeleteObject(hBitmap);
	
	m_pRingImage = LoadImage(IDR_RING,_T("PNG"),AfxGetResourceHandle());
}


void CImgView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int nFrameID = 1;
	if(nIDEvent == ANIM_TIMER_ENVENT)
	{
		if(nFrameID >=  m_agAnimFrameFile.GetSize())
		{
			nFrameID = 0;
		}
		GDIP_FREE(m_pImage);

#if defined (_UNICODE) || defined(UNICODE) 
		m_pImage = Image::FromFile(m_agAnimFrameFile.GetAt(nFrameID).operator LPCTSTR());
#else
		m_pImage = Image::FromFile(A2W((LPSTR)m_agAnimFrameFile.GetAt(nFrameID).operator LPCTSTR()));			
#endif
		if(m_pImage->GetLastStatus() != 0)
		{
			GDIP_FREE(m_pImage);
		}
		
		nFrameID ++;

		Invalidate(FALSE);
	}
	else if(nIDEvent == GIF_TIMER_ENVENT)
	{
		SetNextFrame();
		Invalidate(FALSE);
	}
	
	CStatic::OnTimer(nIDEvent);
}

void  CImgView::GetAnimFrameFile(LPCTSTR lpszAnimFolder,UINT uCmpType)
{
	UNUSED_ALWAYS(uCmpType);
	_ASSERTE( lpszAnimFolder != NULL );	

	TCHAR szExt[4] = {0};
	_tcscpy(szExt,_T("bmp"));
	
	m_agAnimFrameFile.RemoveAll();
	
    CString strFileName;
    strFileName.Format(_T("%s\\*.%s"), lpszAnimFolder,szExt);
	
    WIN32_FIND_DATA wfd;
    HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = ::FindFirstFile(strFileName, &wfd);
    if( hFile == INVALID_HANDLE_VALUE )
    {      
        return;
    }
    
    int nFrameNum = 1;
    for( ; ::FindNextFile(hFile, &wfd); ++nFrameNum )
    {
        // here is null
    }

    ::FindClose(hFile);
	hFile = INVALID_HANDLE_VALUE;

	int i= 0;
	if(nFrameNum>9)
	{
		for( i = 0; i< nFrameNum;i++)
		{
			strFileName.Format(_T("%s\\%02d.%s"),lpszAnimFolder,i+1,szExt);
			m_agAnimFrameFile.Add(strFileName);
		}
	}
	else
	{
		for( i = 0; i< nFrameNum;i++)
		{
			strFileName.Format(_T("%s\\%d.%s"),lpszAnimFolder,i+1,szExt);
			m_agAnimFrameFile.Add(strFileName);
		}
	}
}
Image * CImgView::LoadImage( UINT nID, LPCTSTR lpszType, HINSTANCE hInstance /*=NULL*/)
{
	Image * pImage = NULL;

	if( lpszType == RT_BITMAP )
	{
		HBITMAP hBitmap = ::LoadBitmap( hInstance, MAKEINTRESOURCE(nID) );
		pImage = (Image*)Bitmap::FromHBITMAP(hBitmap, 0);
		::DeleteObject(hBitmap);
		return pImage;
	}		

	hInstance = (hInstance == NULL) ? ::AfxGetResourceHandle() : hInstance;
	HRSRC hRsrc = ::FindResource ( hInstance, MAKEINTRESOURCE(nID), lpszType); 
	ASSERT(hRsrc != NULL);

	DWORD dwSize = ::SizeofResource( hInstance, hRsrc);
	LPBYTE lpRsrc = (LPBYTE)::LoadResource( hInstance, hRsrc);
	ASSERT(lpRsrc != NULL);
	if(lpRsrc == NULL)
	{
		return NULL;
	}

	HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
	LPBYTE pMem = (LPBYTE)::GlobalLock(hMem);
	memcpy( pMem, lpRsrc, dwSize);
	IStream * pStream = NULL;
	::CreateStreamOnHGlobal( hMem, FALSE, &pStream);

	pImage = Image::FromStream(pStream);

	::GlobalUnlock(hMem);
	pStream->Release();
	::FreeResource(lpRsrc);

	return pImage;
}

void CImgView::CreateCtrls()
{
	CRect rect;
 	GetClientRect(&rect);

	//[[ network account
	m_wndNtac.m_bViewOnly = TRUE;
	m_wndNtac.Create(IDD_DLG_NETWORK_ACCOUNT,NULL);	
	//Change to child style dialog
	SetWindowLong(m_wndNtac.m_hWnd,GWL_STYLE,0x44000044);
  	SetWindowLong(m_wndNtac.m_hWnd,GWL_EXSTYLE,0x00000000);
	m_wndNtac.SetParent(this); // this is the key step, must be set after change style	
	m_wndNtac.SetWindowPos(&wndTop,rect.left,rect.top, rect.Width(),rect.Height(),SWP_DRAWFRAME);
	m_wndNtac.ShowWindow(SW_HIDE);

	m_wndMtva.m_bViewOnly = TRUE;
	m_wndMtva.Create(IDD_DLG_MULIT_VARIANT,NULL);	
	//Change to child style dialog
	SetWindowLong(m_wndMtva.m_hWnd,GWL_STYLE,0x44000044);
  	SetWindowLong(m_wndMtva.m_hWnd,GWL_EXSTYLE,0x00000000);
	m_wndMtva.SetParent(this); // this is the key step, must be set after change style	
	m_wndMtva.SetWindowPos(&wndTop,rect.left,rect.top, rect.Width(),rect.Height(),SWP_DRAWFRAME);
	m_wndMtva.ShowWindow(SW_HIDE);


	//[[ bookmark
	m_wndBkmk.m_bViewOnly = TRUE;
	m_wndBkmk.Create(IDD_DLG_BOOKMARK,NULL);	
	//Change to child style dialog
	SetWindowLong(m_wndBkmk.m_hWnd,GWL_STYLE,0x44000044);
  	SetWindowLong(m_wndBkmk.m_hWnd,GWL_EXSTYLE,0x00000000);
	m_wndBkmk.SetParent(this); // this is the key step, must be set after change style
	m_wndBkmk.SetWindowPos(&wndTop,rect.left,rect.top, rect.Width(),rect.Height(),SWP_DRAWFRAME);
	m_wndBkmk.ShowWindow(SW_HIDE);
}

