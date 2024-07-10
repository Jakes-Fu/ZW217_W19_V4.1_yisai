// Converter.cpp: implementation of the CConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Converter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConverter::CConverter()
{

}

CConverter::~CConverter()
{

}

void CConverter::BinaryToHexChar(LPBYTE lpSrc, int nSrcSize, LPSTR lpDes, int &nDeSize)
{
	_ASSERTE(lpSrc != NULL && nSrcSize > 0);
	_ASSERTE(lpDes != NULL && nDeSize > nSrcSize * 2);

    char szValue[ 4 ] = { 0 };
    for(int i = 0; i < nSrcSize; ++i)
    {
        sprintf(szValue, "%02x", *lpSrc);
        *lpDes = szValue[0];
        lpDes++;
        *lpDes = szValue[1];
        lpDes++;

        lpSrc++;
    }
	*lpDes = '\0';
	nDeSize = nSrcSize * 2;
}

void CConverter::HexCharToBinary(LPCSTR lpSrc, int nSrcLen, LPBYTE lpDes, int &nDesSize)
{
	_ASSERTE(lpSrc != NULL && nSrcLen > 0);
    _ASSERTE( nDesSize >= nSrcLen / 2 );

    BYTE btValue;
    int nDesIdx = 0;
     
    for( int i = 0; i < nSrcLen; ++i, ++nDesIdx)
    {
        btValue = HexCharToByte( *(lpSrc + i) );
        *(lpDes + nDesIdx) = (BYTE)(btValue << 4);

		++i;
        *(lpDes + nDesIdx) |= HexCharToByte( *(lpSrc + i) );          
    }
	nDesSize = nSrcLen / 2;
}

__inline BYTE CConverter::HexCharToByte(char cValue)
{
	BYTE btValue = 0;
    if( cValue >= '0' && cValue <= '9' )
    {
        btValue = (BYTE)(cValue - '0');
    }
    else if( cValue >= 'a' && cValue <= 'z' )
    {
        btValue = (BYTE)(cValue - 'a' + 10);
    }
    else if( cValue >= 'A' && cValue <= 'Z' )
    {
        btValue = (BYTE)(cValue - 'A' + 10);
    }
    else
    {
        _ASSERTE( 0 );
    }
    
    return btValue;
}
