/******************************************************************************
                                   ����WCDMA  ����ҵ��ģ��
        ����3G  ������𽥳��죬������ҵ�������Ϊ������
        �ƶ��ն˵����㣬�𽥳�Ϊ���ɻ�ȱ��һ���֡���
        ģ����ƻ���OMA  ��������Ļ���syncml Э������ݺ���
        �˹���ʵ���ڶ�ƽ̨�������ϵͳ�е�PIMҵ��Ŀ��
        ��Ҫ�ﵽ����ƽ̨�Ŀ���ֲ�����õĹ����ȶ���ģ�顣
        ��ģ���ʵ�ַ�Ϊ��������:
        1.ʵ��Data sync����
        2.ʵ��Device Management����
        3.ʵ�ֶ�ƽ̨��ƽ����ֲ
        ģ�鿪���ο�OMA��Ա��ͬ������SRT_c��
        Ŀǰģ��ƻ�����֧��OMA syncML Э��1.1


        ��Ȩ����:�������Ȩ���ں���ͨ�ţ�����һ�з���Ȩ��
        ������Ա:����
        ��������:2005��4��11�ա�
******************************************************************************/


/******************************************************************************
�ļ�����:PIMmd5.h
�ļ�����:md5 codec functions header file.
�ļ�˵��:md5 encode/decode functions for syncml authentication and http authentication.
������Ա:����
�״ο�������:2005-4-11
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
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2004-4-11              ����                ��һ�ο������롣
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*�����FEATURE��ֹͷ�ļ��ظ�����*/
#ifndef   HEADER_FILE_MD5
#define HEADER_FILE_MD5
  /*  ����ͷ�ļ�*/

  /*  ���ݽṹ����*/
/* MD5 context. */
typedef struct {
  unsigned long int state[4];                                   /* state (ABCD) */
  unsigned long int count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

  /*  ȫ�ֱ�������*/
#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];

  /*  ���ܺ�������*/
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

