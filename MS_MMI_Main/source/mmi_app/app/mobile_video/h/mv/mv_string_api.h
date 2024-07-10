#ifndef _MV_STRING_API_H
#define _MV_STRING_API_H

#include "_Types.h"

_INT CM_MultiBytesToWideChar(
							_UINT nCodePage,
							_CHAR* strMultiBytes,
							_INT nMultiBytesLen,
							_WCHAR* wstrWideChar,
							_INT nWideCharLen
);


_INT CM_WideCharToMultiBytes(
							_UINT nCodePage,
							_WCHAR* wstrWideChar,
							_INT nWideCharLen,
							_CHAR* strMultiBytes,
							_INT nMultiBytesLen
);

#endif
