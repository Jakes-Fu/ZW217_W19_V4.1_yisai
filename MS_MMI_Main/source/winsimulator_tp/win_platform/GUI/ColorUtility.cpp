// ColorUtility.cpp: implementation of the CColorUtility class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MSDev.h"
#include "ColorUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorUtility::CColorUtility()
{

}

CColorUtility::~CColorUtility()
{

}

// ----------------------------------------------------------------------------
// Function:
//      Color16ToColor24
// Description:
//		16bit color format: rrrr rggg gggb bbbb
//		24bit color format: bbbb bbbb gggg gggg rrrr rrrr
//		Converted to      : 0000 0000 rrrr r000 gggg gg00 bbbb b000
// ----------------------------------------------------------------------------
COLORREF CColorUtility::Color16ToColor24( WORD color16 )
{
    BYTE red   = ( color16 & 0xF800 ) >> 8;
    BYTE green = ( color16 & 0x07E0 ) >> 3;
    BYTE blue  = ( color16 & 0x001F ) << 3;
    
    return RGB( red, green, blue );
}



WORD CColorUtility::Color24ToColor16( COLORREF color24 )
{
    BYTE red   = GetRValue( color24 );
    BYTE green = GetGValue( color24 );
    BYTE blue  = GetBValue( color24 );
    
    WORD color16 =   ( (red   & 0x00F8) << 11 )
                   + ( (green & 0x00FC) << 5 )
                   + ( (blue  & 0x00F8) );
    
    return color16;
}


// ----------------------------------------------------------------------------
// Function:
//      Color12ToColor24
// 12bit color format: 0000 rrrr gggg bbbb
// 24bit color format: 0000 0000 bbbb bbbb gggg gggg rrrr rrrr
//
// Converted to      : 0000 0000 bbbb 0000 gggg 0000 rrrr 0000
// ----------------------------------------------------------------------------

COLORREF CColorUtility::Color12ToColor24( WORD color12 )
{
    BYTE red   = ( color12 & 0x0F00 ) >> 4;
    BYTE green = ( color12 & 0x00F0 );
    BYTE blue  = ( color12 & 0x000F ) << 4;
    
    return RGB( red, green, blue );
}

WORD CColorUtility::Color24ToColor12( COLORREF color24 )
{
    BYTE red   = GetRValue( color24 );
    BYTE green = GetGValue( color24 );
    BYTE blue  = GetBValue( color24 );
    
    WORD color16 =   ( (red   & 0x00F0) << 4 )
                   + ( (green & 0x00F0) )
                   + ( (blue  & 0x00F0) >> 4 );
    
    return color16;
}
BYTE CColorUtility::Get1BitsNumOfMask( WORD mask )
{
	WORD flag = 1;
	BYTE nNum = 0;
	for( int i = 0; i < 32; i++ )
	{
		if( ( mask & ( flag << i ) ) != 0 )
		{
			nNum += 1;
		}
	}
	return nNum;
}

COLORREF CColorUtility::ColorByMaskToColor24( WORD colorByMask, WORD  r_mask, WORD g_mask, WORD b_mask )
{
	BYTE nR1Num = Get1BitsNumOfMask( r_mask );
	BYTE nG1Num = Get1BitsNumOfMask( g_mask );
	BYTE nB1Num = Get1BitsNumOfMask( b_mask );

	BYTE red,green,blue;
	if( nB1Num + nG1Num > 8 - nR1Num ) //left:8-nr1num ,right: nB1Num + nG1Num
	{
		red   = ( colorByMask & r_mask ) >> ((nB1Num + nG1Num) - ( 8 - nR1Num));
	}
	else
	{
		red   = ( colorByMask & r_mask ) << (( 8 - nR1Num) -(nB1Num + nG1Num)  );
	}

	if( nB1Num > 8 - nG1Num ) //left:8-ng1num ,right: nB1Num 
	{
		green   = ( colorByMask & g_mask ) >> ( nB1Num  - ( 8 - nG1Num));
	}
	else
	{
		green   = ( colorByMask & g_mask ) << (( 8 - nG1Num) - nB1Num   );
	}

	//left:8-nb1num ,right: 0
	blue   = ( colorByMask & b_mask ) << ( 8 - nB1Num);
    
    return RGB( red, green, blue );
}

WORD CColorUtility::Color24ToColorByMask( COLORREF color24 , WORD r_mask, WORD g_mask, WORD b_mask )
{
	BYTE red   = GetRValue( color24 );
    BYTE green = GetGValue( color24 );
    BYTE blue  = GetBValue( color24 );
	
	BYTE nR1Num = Get1BitsNumOfMask( r_mask );
	BYTE nG1Num = Get1BitsNumOfMask( g_mask );
	BYTE nB1Num = Get1BitsNumOfMask( b_mask );
	
	BYTE colorMaskR,colorMaskG,colorMaskB;
	if( nB1Num + nG1Num > 8 - nR1Num ) //right:8-nr1num ,left: nB1Num + nG1Num
	{
		colorMaskR   = ( red  << ((nB1Num + nG1Num) - ( 8 - nR1Num)) ) & r_mask;
	}
	else
	{
		colorMaskR   = ( red  >> (( 8 - nR1Num) - (nB1Num + nG1Num)  )) & r_mask ;
	}
	
	if( nB1Num  > 8 - nR1Num ) //right:8-ng1num ,left: nB1Num 
	{
		colorMaskG   = ( green << ( nB1Num  - ( 8 - nG1Num)) ) & g_mask ;
	}
	else
	{
		colorMaskG   = ( green  >> (( 8 - nG1Num) - nB1Num  ) ) & g_mask;
	}
	
	//right:8-nb1num ,left: 0
	colorMaskB = ( blue >> (8 - nB1Num) ) & b_mask; 	
    
    return  colorMaskR + colorMaskG + colorMaskB;
/*
	
	if( 0xF800 == r_mask && 0x07E0 == g_mask && 0x001F == b_mask )
	{
		return Color24ToColor16( color24 );
	}
	else if( 0x0F00 == r_mask && 0x00F0 == g_mask && 0x000F == b_mask )
	{
		return Color24ToColor12( color24 );
	}
*/
}

Image * CColorUtility::LoadImage( UINT nID, LPCTSTR lpszType, HINSTANCE hInstance /*=NULL*/)
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
