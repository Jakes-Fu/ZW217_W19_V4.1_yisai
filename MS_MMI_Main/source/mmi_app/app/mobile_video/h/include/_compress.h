#ifndef __COMPRESS_H
#define __COMPRESS_H

#include "_Types.h"

#if __cplusplus
extern "C"
{
#endif

extern _INT		wdinfdir (_CONST _BYTE* pArray, _INT nLen);	
extern _BOOLEAN _ResDirectoryArray_Check(_CONST _WCHAR* pFileName, _CONST _BYTE* pArray, _INT nLen);
extern _BOOLEAN GetResSkinVersion(_WCHAR* strVersion, _INT nLen);

#if __cplusplus
}
#endif

#endif