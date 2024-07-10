//
//	_Math.h
//
#ifndef	__MATH_API_H
#define	__MATH_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

typedef _INT (*_COMPAREPROC)(_CONST _VOID* p1, _CONST _VOID* p2);

extern _VOID  _qsort(_VOID* base, _INT num, _UINT width, _COMPAREPROC compare);
extern _VOID* _bsearch(_CONST _VOID* key, _CONST _VOID* base, _UINT num, _UINT width, _COMPAREPROC compare);


#ifdef	__cplusplus
}
#endif

#endif
