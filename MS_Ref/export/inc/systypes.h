/* $id:sysTypes.h  V1.0 2001/08/08 */

/******************************************************************************
 *  This source code has been made available to you by SEIKO EPSON  on
 *  AS-IS.Anyone receiving this source is licensed under
 *  SEIKO EPSON copyrights to use it in any way he or she deems fit,including
 *  copying it,modifying it,compiling it,and redistributing it either with
 *  or without modifictions.
 *
 *
 *  Any person who transfers this source code or any derivative work must
 *  include the SEIKO EPSON copyright notice, this paragraph,and the preceding
 *  two paragraphs in the transferred software.
 *
 *
 *       COPYRIGHT SEIKO EPSON CORPORATION 2001
 *     LICENSED MATERIAL - PROGRAM PROPERTY OF CORETEK
 *****************************************************************************/

/******************************************************************************
 *
 *  FILE: sysTypes.h
 *
 *  MODULE: System predefined data types
 *
 *  PURPOSE: Define some system level data types and some key macros
 *
 *  AUTHOR(S):Lemon
 *
 *  GROUP:TOOL_GROUP
 *
 *  DATE CREATED:2001/08/08
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name       Description
 *  2001/08/08    Lemon           Create this file
 *  2001/08/17    Lemon           add 'T_' prefix for all redefined data type
 *********************************************************************************/

#ifndef _SYSTYPES_H
#define _SYSTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char							T_CHAR;		/* character typde */
typedef	char							T_BYTE;     /*8-bit signed integer */
typedef  unsigned char					T_UBYTE;    /*8-bit unsigned integer */

typedef	signed short					T_HWORD;    /*16-bit signed integer */
typedef	unsigned short					T_UHWORD;   /*16-bit unsigned integer */

typedef	signed int						T_WORD;     /*32-bit signed integer */
typedef  unsigned int					T_UWORD;    /*32-bit unsigned integer */

/**********************************************
 *  When in VC enviroment:

 *    typedef LONGLONG    T_DWORD;
 *    typedef ULONGLONG   T_UDWORD;
 *
 **********************************************/
typedef __int64							T_DWORD;     /*64-bit signed integer */
typedef unsigned __int64				T_UDWORD;    /*64-bit unsigned integer */

typedef	double							T_DOUBLE;
typedef  float							T_FLOAT;

typedef  unsigned int					T_BOOL;    /*32-bit unsigned integer */

typedef	volatile signed char			T_VBYTE;
typedef  volatile unsigned char			T_VUBYTE;

typedef	volatile signed short			T_VHWORD;
typedef	volatile unsigned short			T_VUHWORD;

typedef	volatile signed int				T_VWORD;
typedef volatile unsigned int			T_VUWORD;

/**********************************************
 *  When in VC enviroment:
 *    typedef volatile LONGLONG    T_VDWORD;
 *    typedef volatile ULONGLONG   T_VUDWORD;
 *
 **********************************************/
typedef volatile __int64				T_VDWORD;
typedef volatile unsigned __int64		T_VUDWORD;

typedef	volatile double					T_VDOUBLE;
typedef volatile float					T_VFLOAT;

typedef volatile unsigned int			T_VBOOL; /*32-bit unsigned integer */

#define T_VOID			void
#define T_VVOID			volatile void

#define T_MODULE		static
#define T_INLINE		T_MODULE inline
#define T_EXTERN		extern
#define T_VOLATILE		volatile
#define T_CONST			const



#ifndef TRUE
#define TRUE    	1
#endif

#ifndef FALSE
#define FALSE    	0
#endif

#ifndef NULL
#define NULL    	0
#endif

#ifdef __cplusplus
}
#endif

#endif



