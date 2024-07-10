#ifndef _MV_MM_INTERFACE_H
#define _MV_MM_INTERFACE_H

#include "_Types.h"

#if 1
_VOID* CM_Malloc_MV(_INT nSize, const char * file, uint32 line);
_VOID* CM_Realloc_MV(_VOID*pOldBuffer, _INT nResize, const char * file, uint32 line);
#endif

_VOID* CM_Malloc(_INT nSize);
_BOOLEAN CM_Free(_VOID* pBuffer);

_VOID* CM_MemSet(_VOID* pBuffer,_BYTE c, _INT nSize);

_VOID* CM_MemCopy(_VOID* pDes, _CONST _VOID* pSrc,_INT nSize);

_INT CM_MemCmp(_CONST _VOID* pDes, _CONST _VOID*pSrc,_INT nSize);

_VOID* CM_MemMove(_CONST _VOID* pDes, _CONST _VOID* pSrc,_INT nSize);

/****************************************************************************/
//Description : Create dlmalloc handle for mv block.
//Global resource dependence : 
//Author: 
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MV_CreatMemHandle(void);

/****************************************************************************/
//Description : Free dlmalloc handle for mv block.
//Global resource dependence : 
//Author: 
//Note:	
/****************************************************************************/
PUBLIC void MV_DeleteMemHandle(void);
#endif