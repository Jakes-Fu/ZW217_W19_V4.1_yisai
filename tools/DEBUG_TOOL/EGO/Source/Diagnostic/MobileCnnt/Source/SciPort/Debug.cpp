// File Name:   Debug.cpp
// Author:      anli.wei
// Date:        2006-01-25
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 
//

#include "StdAfx.h"
#include "Debug.h"

#if defined(_DEBUG) || defined(DEBUG)

#if !defined(TRACE)

void TRACE(LPCTSTR pszFormat, ...)
{
    va_list vlst;

    va_start( vlst, pszFormat );

    static _TCHAR szBuf[512];

    memset( szBuf, 0, sizeof(szBuf) );
    _vsntprintf(szBuf, sizeof(szBuf)/sizeof(_TCHAR) - 1, pszFormat, vlst);

    va_end( vlst );

    OutputDebugString(szBuf); 
}

#endif // !defined(TRACE)

#endif // defined(_DEBUG) || defined(DEBUG)