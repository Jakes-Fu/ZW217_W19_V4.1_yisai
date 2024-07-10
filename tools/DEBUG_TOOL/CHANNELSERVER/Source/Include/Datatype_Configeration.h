/*============================================================================
Define basic data types:

You should define these data types in this section:

  BYTE, WORD, DWORD, BOOL, NULL, FALSE, TRUE
==============================================================================*/

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*===============================================================================
Define if the target machine is big endian. 

DEFAULT: NOT DEFINE

=================================================================================*/

//#define BIG_ENDIAN

/*================================================================================
Define the machine independent micros. These micros should work well both in
little endian system and big endian system.
==================================================================================*/

#ifndef MAKEWORD
#define MAKEWORD(l, h)      ((WORD)(((BYTE)(l)) | ((WORD)((BYTE)(h))) << 8))
#endif

#ifndef MAKEDWORD
#define MAKEDWORD(l,h)      ((DWORD)(((WORD)(l)) | ((DWORD)((WORD)(h))) << 16))
#endif

#ifndef LOWORD
#define LOWORD(l)           ((WORD)(l))
#endif

#ifndef HIWORD
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(w))
#endif

#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif
