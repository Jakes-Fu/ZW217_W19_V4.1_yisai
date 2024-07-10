// ColorUtility.h: interface for the CColorUtility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORUTILITY_H__BECA7B45_9C38_457E_955A_9EEC9FC1A5A6__INCLUDED_)
#define AFX_COLORUTILITY_H__BECA7B45_9C38_457E_955A_9EEC9FC1A5A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorUtility  
{
public:
	CColorUtility();
	virtual ~CColorUtility();
    static COLORREF Color12ToColor24( WORD color12 );
    static COLORREF Color16ToColor24( WORD color16 );
    static WORD     Color24ToColor12( COLORREF color24 );
    static WORD     Color24ToColor16( COLORREF color24 );

	static BYTE     Get1BitsNumOfMask( WORD mask );
	static COLORREF ColorByMaskToColor24( WORD colorByMask, WORD r_mask, WORD g_mask, WORD b_mask );
	static WORD     Color24ToColorByMask( COLORREF color24 , WORD r_mask, WORD g_mask, WORD b_mask );
	static Image*   LoadImage( UINT nID, LPCTSTR lpszType, HINSTANCE hInstance =NULL);
};

#endif // !defined(AFX_COLORUTILITY_H__BECA7B45_9C38_457E_955A_9EEC9FC1A5A6__INCLUDED_)
