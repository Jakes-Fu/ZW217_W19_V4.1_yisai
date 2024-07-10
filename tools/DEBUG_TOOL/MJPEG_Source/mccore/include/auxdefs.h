/******************************************************************************
|* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
|* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
|* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
|* PARTICULAR PURPOSE.
|* 
|* Copyright 1995-2005 Nero AG. All Rights Reserved.
|*-----------------------------------------------------------------------------
|* NeroSDK / AudioPluginManager
|*
|* PROGRAM: auxdefs.h
|*
|* PURPOSE: Auxiliary definitions for the plugin manager
******************************************************************************/

#ifndef _AUX_DEFS_
#	define _AUX_DEFS_

#define	BITS_PER_TETRAD		4
#define	BITS_PER_BYTE		8
#define	BITS_PER_WORD		16
#define BITS_PER_3BYTE		24
#define	BITS_PER_DWORD		32
#define	BITS_PER_QWORD		64

#define BYTES_PER_KB		(1024)
#define BYTES_PER_MB		(BYTES_PER_KB * 1024)
#define BYTES_PER_GB		(BYTES_PER_MB * 1024)

// Indicates infinite or wrong lengths
#define	NO_LENGTH			((ULONGLONG)-1)

#endif // _AUX_DEFS_
