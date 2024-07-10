#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <stdint.h>

typedef int					INT;
typedef unsigned int		UINT;
typedef unsigned int		BOOL;
typedef unsigned long		DWORD;
typedef unsigned short		WORD;
typedef unsigned char		BYTE;
typedef short				SHORT;
typedef int					LONG;
typedef void*				PVOID;
typedef void*				LPVOID;
typedef DWORD*				LPDWORD;
typedef BYTE*				LPBYTE;
typedef unsigned long		ULONG;
typedef char				TCHAR;
typedef char				_TCHAR;
typedef int                 WPARAM;
typedef void*               LPARAM;
typedef const char * LPCTSTR;

#ifndef _TEOF
#define _TEOF       EOF
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef _MAX_PATH
#define _MAX_PATH               512
#endif

#ifndef MAX_PATH
#define MAX_PATH                512
#endif

#ifndef INVALID_FILE_SIZE
#define INVALID_FILE_SIZE       (DWORD)(-1)          
#endif



#define __T(x)      			x
#define _T(x)       			x
#define MAKEWORD(a, b)			((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)			((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)				((WORD)(l))
#define HIWORD(l)				((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)				((BYTE)(w))
#define HIBYTE(w)				((BYTE)(((WORD)(w) >> 8) & 0xFF))




#endif // TYPEDEF_H
