// OsdGenerater.h: interface for the COsdGenerater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OSDGENERATER_H__998F7C73_198D_4E25_B6FE_041ED22496F5__INCLUDED_)
#define AFX_OSDGENERATER_H__998F7C73_198D_4E25_B6FE_041ED22496F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Generater.h"
#include "sci_types.h"
#include "BmpToArray.h"

class COsdGenerater : public CGenerater  
{
public:
	COsdGenerater(BOOL bBigEndian);
	COsdGenerater();
	virtual ~COsdGenerater();
	
	void SetParameters(int nTransparentColor,BMP_RECT_PTR * pBmpRectPtr,int nBmpRectNum);
	BOOL ParsePalette(LPBYTE pSrc24, int nSrcSize, uint8 * pDst8, int nDstSize);
	BOOL Generate(LPCTSTR pArrayName, const uint8 * pContent, uint32 nContentSize, BOOL bReverse = FALSE );


public:
	BOOL LoadFromFile(LPCTSTR pFileName, LPBYTE * pContent, int &nContentSize, int nConvertType = 0);

public:
	uint32  m_arrPalette[1024];

protected:	
	int m_nBmpHeight;
	int m_nBmpWidth;
	uint16	m_nPaletteTotalNum;
	uint32 m_nTransparentColor;

	BMP_RECT_PTR *m_pBmpRectPtr;	
	int           m_nBmpRectNum;
    
};

#endif // !defined(AFX_OSDGENERATER_H__998F7C73_198D_4E25_B6FE_041ED22496F5__INCLUDED_)
