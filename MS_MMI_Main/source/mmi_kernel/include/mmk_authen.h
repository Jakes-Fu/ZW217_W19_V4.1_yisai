/****************************************************************************
** File Name:      mmk_authen.c                                            *
** Author:         Wei.Zhou                                                *
** Date:           03/21/2009                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2009        Wei.Zhou         Create
** 
****************************************************************************/

#ifndef _MMK_AUTHEN_H_
#define _MMK_AUTHEN_H_

/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typdef the following:
 *    name              meaning
 *  uint32         unsigned 32 bit integer
 *  uint8          unsigned 8 bit integer (i.e., unsigned char)
 *  int16    integer of >= 16 bits
 *
 */

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError       /* called Input after Result */
};
#endif
#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context
{
    uint32 Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

    uint32 Length_Low;            /* Message length in bits      */
    uint32 Length_High;           /* Message length in bits      */

                               /* Index into message block array   */
    int16 Message_Block_Index;
    uint8 Message_Block[64];      /* 512-bit message blocks      */

    int Computed;               /* Is the digest computed?         */
    int Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;

/*
 *  Function Prototypes
 */

/* MD5 context. */
typedef struct {
    uint32 state[4];                                   /* state (ABCD) */
    uint32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;
PUBLIC void MD5Init(MD5_CTX *);
PUBLIC void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
PUBLIC void MD5Final(unsigned char [16], MD5_CTX *);
PUBLIC BOOLEAN MMK_VerifySignFile(const wchar *filename_ptr);

#endif


