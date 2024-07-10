//
//	Types.h
//
#ifndef __TYPES_H
#define	__TYPES_H

typedef signed char		_INT8;
typedef unsigned char	_UINT8;

typedef	signed short	_INT16;
typedef	unsigned short	_UINT16;

typedef	signed int		_INT32;
typedef	unsigned int	_UINT32;

typedef	long			_LONG;

typedef	float			_FLOAT;
typedef	double			_DOUBLE;

typedef	_INT32			_INT;
typedef	_UINT32			_UINT;

typedef	char			_CHAR;
typedef unsigned short	_WCHAR;

typedef	unsigned char	_BYTE;
typedef	unsigned short	_WORD;
typedef	unsigned long	_DWORD;

typedef	_INT			_BOOLEAN;

typedef	void			_VOID;
typedef	void*			_PVOID;

typedef _PVOID			_HANDLE;

typedef _INT			_TIME;

#ifndef	_CONST
#define	_CONST			const
#endif

#define	IsValidHandle(hHandle)	(((_INT32)hHandle) >= 0)

#ifndef _TRUE
	#define	_TRUE		(1)
#endif
#ifndef _FALSE
	#define	_FALSE		(0)
#endif
#ifndef	_NULL
	#define	_NULL		(0)
#endif

//////////////////////////////////////////////////////////////////////////
struct tagPoint
{
	_INT	nX, nY;
};
typedef struct tagPoint	_POINT, *_PPOINT;
//////////////////////////////////////////////////////////////////////////
struct tagSize
{
	_INT	nWidth, nHeight; 
};
typedef struct tagSize	_SIZE, *_PSIZE;
//////////////////////////////////////////////////////////////////////////
struct tagRect
{
	_INT	nLeft, nTop, nWidth, nHeight;
};
typedef struct tagRect	_RECT, *_PRECT;
//////////////////////////////////////////////////////////////////////////

#endif	//__TYPES_H
