//
//	_Memory.h
//
#ifndef	__MEMORY_API_H
#define	__MEMORY_API_H


#ifdef	__cplusplus
extern "C"
{
#endif

#define	_Malloc(size)	_DebugMalloc(size, __FILE__, __LINE__)

extern _VOID  _MemoryInit(_VOID* p, _INT);
extern _VOID  _MemoryTerminate();
extern _VOID* _DebugMalloc(_INT size, _CONST _CHAR* strFilename, _INT nLine);
extern _VOID* _Realloc(_VOID*, _INT);
extern _VOID  _Free(_VOID*);
extern _VOID  _MemSet(_VOID*, _BYTE, _INT);
extern _VOID  _MemCopy(_VOID*, _CONST _VOID*, _INT);
extern _INT   _MemCmp(_CONST _VOID*, _CONST _VOID*, _INT);
extern _VOID* _MemMove(_VOID*, _CONST _VOID*, _INT);

#ifdef	__cplusplus
}
#endif

#endif
