/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _INTEGER
#define _INTEGER

#ifdef _WIN32   /* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else           /* Embedded platform */

/* These types must be 16-bit, 32-bit or larger integer */
typedef int             INT;
typedef unsigned int    UINT;

/* These types must be 8-bit integer */
typedef char            CHAR;
typedef unsigned char   UCHAR;
typedef unsigned char   BYTE;

/* These types must be 16-bit integer */
typedef short           SHORT;
typedef unsigned short  USHORT;
typedef unsigned short  WORD;
typedef unsigned short  WCHAR;

/* These types must be 32-bit integer */
typedef long            LONG;
typedef unsigned long   ULONG;
typedef unsigned long   DWORD;

#endif

#ifdef TF_LITTLE_ENDIAN
#define TF_WORD_SWAP(x)					x
#define TF_DWORD_SWAP(x)				x
#else
#define TF_WORD_SWAP(x)    		        ((((x)<<8)&0xff00)|(((x)>>8)&0x00ff))
#define TF_DWORD_SWAP(x)   	   		 	((((x)<<24)&0xff000000L)|(((x)<<8)&0x00ff0000L)|(((x)>>8)&0x0000ff00L)|(((x)>>24)&0x000000ffL))
#endif

#endif
