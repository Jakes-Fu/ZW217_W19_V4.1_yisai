/****************************************************************************
** File Name:      cafstdlib.h                                             *
** Author:         James.Zhang                                             *
** Date:           04/16/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_STDLIB_H_    
#define  _CAF_STDLIB_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include <stddef.h>     // size_t, etc.
#include <stdarg.h>
   
#include "caf.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
/* stdlib */
typedef struct
{
   //ARM string.h
   void*    (*Memcpy)   ( void *dst, const void *src, size_t count );
   void*    (*Memmove)  ( void *dst, const void *src, size_t count );
   char*    (*Strcpy)   ( char *dst, const char *src );
   char*    (*Strncpy)  ( char *dst, const char *src, size_t count );
   char*    (*Strcat)   ( char *dst, const char *src );
   char*    (*Strncat)  ( char *dst, const char *src, size_t count );
   int      (*Memcmp)   ( const void *src1, const void *src2, size_t count );
   int      (*Strcmp)   ( const char *src1, const char *src2 );
   int      (*Strncmp)  ( const char *src1, const char *src2, size_t count );
   int      (*Strcoll)  ( const char *src1, const char *src2 );
   size_t   (*Strxfrm)  ( char *dst, const char *src, size_t count );
   void*    (*Memchr)   ( const void * src, int ch, size_t count );
   char*    (*Strchr)   ( const char *src, int ch );
   size_t   (*Strcspn)  ( const char *src1, const char *src2 );
   char*    (*Strpbrk)  ( const char *src1, const char *src2 );
   char*    (*Strrchr)  ( const char *src, int ch );
   size_t   (*Strspn)   ( const char *src1, const char *src2 );
   char*    (*Strstr)   ( const char *src1, const char *src2 );
   char*    (*Strtok)   ( char *src1, const char *src2 );
   void*    (*Memset)   ( void *dst, int value, size_t count );
   char*    (*Strerror) ( int errnum );
   size_t   (*Strlen)   ( const char *src );

   //ARM ctype.h
   int      (*Isalnum)  ( int ch );
   int      (*Isalpha)  ( int ch );
   int      (*Iscntrl)  ( int ch );
   int      (*Isdigit)  ( int ch );
   int      (*Isgraph)  ( int ch );
   int      (*Islower)  ( int ch );
   int      (*Isprint)  ( int ch );
   int      (*Ispunct)  ( int ch );
   int      (*Isspace)  ( int ch );
   int      (*Isupper)  ( int ch );
   int      (*Isxdigit) ( int ch );
   int      (*Tolower)  ( int ch );
   int      (*Toupper)  ( int ch );

   //ARM stdlib.h
   int      (*Atoi)     ( const char *string );
   long int (*Atol)     ( const char *string );
   long int (*Strtol)   ( const char * nptr, char ** endptr, int base );
   unsigned long int  (*Strtoul) (const char * nptr,char ** endptr, int base );
   int      (*Rand)     ( void );
   void     (*Srand)    ( unsigned int seed );
   void     (*Qsort)    ( void * base, size_t num, size_t size,int (* compar)( const void *, const void * ) );
   int      (*Abs)      ( int n );
    
    //ARM stdio.h
   int      (*Sprintf)  ( char *string, const char *format, ... );
   int      (*Vsprintf) ( char *string, const char *format, va_list arg );

   void*    (*Malloc)   ( uint32 size, const char *file, uint32 line );
   void     (*Free)     ( void* mem_ptr );

   void     (*Assert)   ( void* exp, void *file, uint32 line );
   uint32   (*Trace)    ( const char *format, ...);

   void     (*Sleep)    ( uint32 milliseconds );
   //wstring
   wchar*   (*Wstrcpy)  ( wchar *dst, const wchar *src );
   wchar*   (*Wstrncpy) ( wchar *dst, const wchar *src, size_t count );
   wchar*   (*Wstrcat)  ( wchar *dst, const wchar *src );
   wchar*   (*Wstrncat) ( wchar *dst, const wchar *src, size_t count );
   int      (*Wstrcmp)  ( const wchar *src1, const wchar *src2 );
   int      (*Wstrncmp) ( const wchar *src1, const wchar *src2, size_t count );
   size_t   (*Wstrlen)  ( const wchar *src );

   wchar*   (*Strtowstr)( const char *src, wchar *dst );
   char*    (*Wstrtostr)( const wchar *src, char *dst );
   
   char*    (*Strlower) ( char *dst );
   char*    (*Strupper) ( char *dst );
   wchar*   (*Wstrlower)( wchar *dst );
   wchar*   (*Wstrupper)( wchar *dst );
   
   uint32   (*Htonl)    ( uint32 host_long );
   uint16   (*Htons)    ( uint16 host_short );
   uint32   (*Ntohl)    ( uint32 net_long );
   uint16   (*Ntohs)    ( uint16 net_short );
   int      (*INetaton) ( const char *cp, uint32* pin );
   char*    (*INetntoa) ( uint32 in );
   uint32   (*GetTicket) ( void );
   uint16   (*ConvertUCS2GB)( const wchar *ucs_ptr, uint16 length, uint8 *gb_ptr);
   uint16   (*ConvertGB2UCS)(const uint8 *gb_ptr, uint16 length, wchar *ucs_ptr);
   uint16   (*ConvertUCS2B2UTF8)( const wchar *ucs2b_ptr, uint16 ucs2b_len, uint8 *utf8_ptr, uint16 utf8_buf_len);
   uint16   (*ConvertUTF82UCS2B)( const uint8 *utf8_ptr, uint16 utf8_len, wchar *ucs2b_ptr, uint16 ucs2b_buf_len);
   uint16   (*ConvertUCS2BE2UCS2LE)( const uint8 *ucs2b_ptr, uint16 ucs2b_len, uint8 *ucs2l_ptr, uint16 ucs2l_buf_len );
   uint16   (*ConvertUCS2LE2UCS2BE)( const uint8 *ucs2l_ptr, uint16 ucs2l_len, uint8 *ucs2b_ptr, uint16 ucs2b_buf_len );
}CAF_STDLIB_T;

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
