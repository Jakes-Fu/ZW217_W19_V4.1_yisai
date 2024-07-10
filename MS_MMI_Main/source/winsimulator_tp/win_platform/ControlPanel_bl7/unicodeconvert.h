// UnicodeConvert.h

#include "stdafx.h"

#ifndef __UNICODECONVERT_H__
#define __UNICODECONVERT_H__

class CUnicodeConvert
{
public:
    CUnicodeConvert();
    ~CUnicodeConvert();

    int GetChineseCharCount( LPCTSTR lpszText );

    WORD * ConvertToUnicode( WCHAR * pwcUnicode,
                             int     nUnicodeBufSize,
                             LPCTSTR lpszAscii,
                             int     nAsciiBufSize );
        
};

#endif // __UNICODECONVERT_H__