#if !defined(_IDATACONVERTER_H_)
#define _IDATACONVERTER_H_

#pragma once

#include "SciDllExport.h"

class IDataConverter
{
public:
	virtual BOOL BinaryToHexChar(LPBYTE lpSrc, int nSrcSize, LPSTR lpDes, int &nDeSize)=0;
    virtual BOOL HexCharToBinary(LPCSTR lpSrc, int nSrcLen, LPBYTE lpDes, int &nDesSize)=0;
	virtual WORD BigEdnToLitEdn( WORD wBigEdn )=0;    
	virtual BOOL BigEdnToLitEdn( LPWORD pwBigEdn, int nNum )=0;
	virtual WORD LitEdnToBigEdn( WORD wLitEdn )=0;
	virtual BOOL LitEdnToBigEdn( LPWORD pwLitEdn, int nNum )=0;
	virtual DWORD ConvEdn_INT( DWORD dwEdn ) =0;
	virtual void Release()=0;
	virtual ~IDataConverter()=0;
};

SCIDLL_EXPORT_API BOOL CreateDataConverter ( IDataConverter ** ppDataConverter );

#endif //_IDATACONVERTER_H_