/************************************************************************
*
*   Copyright(c) 2002 SANYO Electric Co., Ltd.
*   All Rights Reserved.
*
*   USB Speaker Sample Program
*   File name:	fm_common.h
*
*   $Id$
*
*************************************************************************/

#if !defined _COMMON_H
#define _COMMON_H


//#define CF12MHZ

/******************************************************************************
    typedef宣言
******************************************************************************/
typedef unsigned char   BYTE;       // 8bit
typedef unsigned short  WORD;       // 16bit
typedef unsigned int    UINT;       // 16bit
typedef unsigned long   DWORD;      // 32bit

// HLE added
typedef BYTE*   PBYTE;
typedef WORD*   PWORD;
typedef DWORD*  PDWORD;
typedef BYTE    BOOL;
typedef BYTE*   PBOOL;

typedef signed long INT32;

/******************************************************************************
    システム制御用定数定義
******************************************************************************/
#define		TRUE		1
#define 	FALSE		0
#define		EMPTY		0
#define		FULL		1
#define     NULL        0

#define     OFF         (BYTE)0
#define     ON          (BYTE)!OFF

#define     BIT0        (BYTE)(0x01)
#define     BIT1        (BYTE)(0x01) << 1
#define     BIT2        (BYTE)(0x01) << 2
#define     BIT3        (BYTE)(0x01) << 3
#define     BIT4        (BYTE)(0x01) << 4
#define     BIT5        (BYTE)(0x01) << 5
#define     BIT6        (BYTE)(0x01) << 6
#define     BIT7        (BYTE)(0x01) << 7


#endif // _COMMON_H
