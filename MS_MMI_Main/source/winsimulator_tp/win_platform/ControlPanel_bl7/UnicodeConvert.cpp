// UnicodeConvert.cpp

#include "UnicodeConvert.h"

CUnicodeConvert::CUnicodeConvert()
{
}

CUnicodeConvert::~CUnicodeConvert()
{
}

// Calculate the Chinese character count
int CUnicodeConvert::GetChineseCharCount( LPCTSTR lpszText )
{
    ASSERT( lpszText != NULL );
    
    int nChineseCharCount = 0;
    const unsigned char * pChar = ( const unsigned char * )lpszText;
    
    while ( *pChar != '\0' )
    {
        if ( *pChar <= 0x80 ) // pChar must be 'unsigned' !
        {
            pChar++;
        }
        else
        {
            pChar += 2;
            nChineseCharCount++;
        }
    }
    
    return nChineseCharCount;
}

// Convert the string to unicode format
WCHAR * CUnicodeConvert::ConvertToUnicode(
    WCHAR * pwcUnicode,
    int     nUnicodeBufSize, // size in byte
    LPCTSTR lpszAscii,
    int     nAsciiBufSize )  // size in byte
{
    ASSERT( pwcUnicode != NULL );
    ASSERT( lpszAscii != NULL );
    
    MultiByteToWideChar(
        CP_ACP,
        0,
        lpszAscii,
        nAsciiBufSize,
        pwcUnicode,
        nUnicodeBufSize );
    
    return pwcUnicode;
}
