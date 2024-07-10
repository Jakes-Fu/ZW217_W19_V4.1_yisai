/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMmd5.h
文件描述:md5 codec functions header file.
文件说明:md5 encode/decode functions for syncml authentication and http authentication.
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/
/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software.
 */


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*加入此FEATURE防止头文件重复包含*/
#ifndef   HEADER_FILE_MD5
#define HEADER_FILE_MD5
  /*  包含头文件*/

  /*  数据结构声明*/
/* MD5 context. */
typedef struct {
  unsigned long int state[4];                                   /* state (ABCD) */
  unsigned long int count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

  /*  全局变量声明*/
#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];

  /*  功能函数声明*/
void PIMMD5Init (MD5_CTX *);
void PIMMD5Update (MD5_CTX *, char *, unsigned int);
void PIMMD5Final (char[16], MD5_CTX *);
void PIMCvtHex(  HASH Bin,  HASHHEX Hex);
void PIMDigestCalcHA1(
     char * pszAlg,
     char * pszUserName,
     char * pszRealm,
     char * pszPassword,
     char * pszNonce,
     char * pszCNonce,
     HASHHEX SessionKey
    );
void PIMDigestCalcResponse(
     HASHHEX HA1,           /* H(A1) */
     char * pszNonce,       /* nonce from server */
     char * pszNonceCount,  /* 8 hex digits */
     char * pszCNonce,      /* client nonce */
     char * pszQop,         /* qop-value: "", "auth", "auth-int" */
     char * pszMethod,      /* method from the request */
     char * pszDigestUri,   /* requested URL */
     HASHHEX HEntity,       /* H(entity body) if qop="auth-int" */
     HASHHEX Response      /* request-digest or response-digest */
    );

#endif

