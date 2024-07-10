// Converter.h: interface for the CConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERTER_H__DC1AAA39_FAF0_4585_A7C9_BFC755151F38__INCLUDED_)
#define AFX_CONVERTER_H__DC1AAA39_FAF0_4585_A7C9_BFC755151F38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConverter  
{
public:
	void BinaryToHexChar(LPBYTE lpSrc, int nSrcSize, LPSTR lpDes, int &nDeSize);
    void HexCharToBinary(LPCSTR lpSrc, int nSrcLen, LPBYTE lpDes, int &nDesSize);

	WORD BigEdnToLitEdn( WORD wBigEdn ) { return MAKEWORD(HIBYTE(wBigEdn), LOBYTE(wBigEdn)); }

    DWORD ConvEdn_INT( DWORD dwEdn ) 
    { 
        return MAKELONG( MAKEWORD(HIBYTE(HIWORD(dwEdn)), LOBYTE(HIWORD(dwEdn))),
                         MAKEWORD(HIBYTE(LOWORD(dwEdn)), LOBYTE(LOWORD(dwEdn)))); 
    }

    //void ConvEdn_INT( LPDWORD pdwEdn ) 
    //{ 
    //    DWORD dwEdn = *pdwEdn;
    //    *pdwEdn = MAKELONG( MAKEWORD(HIBYTE(HIWORD(dwEdn)), LOBYTE(HIWORD(dwEdn))),
    //                        MAKEWORD(HIBYTE(LOWORD(dwEdn)), LOBYTE(LOWORD(dwEdn)))); 
    //}
	void BigEdnToLitEdn( LPWORD pwBigEdn, int nNum )
	{
		_ASSERTE( pwBigEdn != NULL );

		for( int i = 0; i < nNum; ++i, ++pwBigEdn )
		{
			*pwBigEdn = BigEdnToLitEdn(*pwBigEdn);
		}
	}
	WORD LitEdnToBigEdn( WORD wLitEdn ) { return MAKEWORD(HIBYTE(wLitEdn), LOBYTE(wLitEdn)); }
	void LitEdnToBigEdn( LPWORD pwLitEdn, int nNum )
	{
		_ASSERTE( pwLitEdn != NULL );

		for( int i = 0; i < nNum; ++i, ++pwLitEdn )
		{
			*pwLitEdn = LitEdnToBigEdn(*pwLitEdn);
		}
	}

public:
	CConverter();
	~CConverter();
protected:
	__inline BYTE HexCharToByte(char cValue);
};

#endif // !defined(AFX_CONVERTER_H__DC1AAA39_FAF0_4585_A7C9_BFC755151F38__INCLUDED_)
