// File Name:   Debug.h 
// Author:      anli.wei
// Date:        2006-01-25
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 
//

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <crtdbg.h>

#pragma once

#if defined(_DEBUG) || defined(DEBUG)

#if !defined(TRACE)

extern void TRACE(LPCTSTR pszFormat, ...);

#define TRACE0 TRACE
#define TRACE1 TRACE
#define TRACE2 TRACE
#define TRACE3 TRACE

#endif // !defined(TRACE)

#else // !defined(_DEBUG) && !defined(DEBUG)

__inline void TRACE(LPCTSTR pszFormat, ...) {UNREFERENCED_PARAMETER(pszFormat);}

#define TRACE0 
#define TRACE1 
#define TRACE2 
#define TRACE3
 
#endif // defined(_DEBUG) || defined(DEBUG)

#endif // __DEBUG_H__