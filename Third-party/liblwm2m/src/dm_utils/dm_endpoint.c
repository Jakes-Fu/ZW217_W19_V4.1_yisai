/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/

#include <string.h>
#include <stddef.h>
#include "internals.h"

#ifdef CONFIG_MBEDTLS_LIB_SUPPORT
#include <mbedtls/config.h>
#include <mbedtls/sha256.h>
#include <mbedtls/aes.h>
#else
#include "sha256.h"
#include "aes.h"
#endif

#include "j_base64.h"

//#include "osi_log.h"
#include "liblwm2m.h"
#include "dm_endpoint.h"


#define AES_BLOCK_SIZE 16

int my_aes_encrypt(char* enckey, char* encbuf, char* decbuf, int inlen, int* outlen)

{
    char key[34] = { 0 }; // = "12345678"
    uint8_t iv[16] = { 0 };
    int nBei;
    int nNumber;
    int nLen = inlen; //input_string.length();
    mbedtls_aes_context ctx;
    int nTotal;
    uint8_t *enc_s = NULL;

    memset(key,0,sizeof(key));
    memcpy(key, enckey, 32);
    if ((!encbuf) || (!decbuf))
    {
        return -1;
    }

    nBei = nLen / AES_BLOCK_SIZE + 1;
    nTotal = nBei * AES_BLOCK_SIZE;
    enc_s = ( unsigned char*)lwm2m_malloc(nTotal);

    if (enc_s == NULL)
    {
        OSI_LOGI(0x10007bca, "enc_s mem err\n");
        return -1;
    }

    if (nLen % AES_BLOCK_SIZE > 0)
    {
        nNumber = nTotal - nLen;
    }
    else
    {
        nNumber = AES_BLOCK_SIZE;
    }

    memset(enc_s, nNumber, nTotal);
    memcpy(enc_s, encbuf, nLen);

    mbedtls_aes_init( &ctx );
    mbedtls_aes_setkey_enc(&ctx, (const unsigned char *)key, 256);
    mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, nBei * AES_BLOCK_SIZE, iv, enc_s, (unsigned char *)decbuf );
    mbedtls_aes_free(&ctx );

    *outlen = nBei * AES_BLOCK_SIZE;

    lwm2m_free(enc_s);
    enc_s = NULL;

    return 0;
}

void my_sha256(const char *src,int srclen, char *resultT)
{
#ifdef CONFIG_MBEDTLS_LIB_SUPPORT
    mbedtls_sha256_ret((const unsigned char *)src, srclen, (unsigned char *)resultT, 0);
#else
    mbedtls_sha256_ret(src, srclen, resultT, 0);
#endif
    return ;
}


int genDmRegEndpointName(char *type, char ** data, void *dmconfig)
{
#define EP_MEM_SIZE  (264)
    char key[64] = {0};
    char *szEpname = NULL;
    char *ciphertext = NULL;
    char *szStars="****";
    char *name = "";
    int  ciphertext_len;
    int  ret = 0;
    char *base64tmp = NULL;
    char *epnametmp = NULL;
    int  i = 0;
    char *passwd ="00000000000000000000000000000000";
    char iv[16] = { 0 };
    /* A 128 bit IV */
    /* Buffer for the decrypted text */
    char *plaintext =  "plaintext";
    Options *g_opt = (Options *)dmconfig;
    //base64
    char *testbase64 = "123456789";//MTIzNDU2Nzg5
    unsigned char *encData = 0;
    int encDataLen = 0;

    memset(iv, 0, 16);
    for (i = 0; i < 16; ++i)
    {
        iv[i] = 0;
    }

    szEpname=( char *)lwm2m_malloc(EP_MEM_SIZE);
    if (szEpname == NULL)
    {
        OSI_LOGI(0x10007bcb, "mem err r1\n");
        ret=-1;
        goto out;
    }

    ciphertext=( char *)lwm2m_malloc(EP_MEM_SIZE);
    if (ciphertext == NULL)
    {
        OSI_LOGI(0x10007bcc, "mem err r2\n");
        ret=-1;
        goto out;
    }

    memset(ciphertext, 0, EP_MEM_SIZE);
    memset(szEpname, 0, EP_MEM_SIZE);
    sprintf(szEpname,"%s-%s-%s-%s",
        strlen((const char *)(g_opt->szCMEI_IMEI)) > 0 ? g_opt->szCMEI_IMEI:szStars,
        strlen((const char *)(g_opt->szCMEI_IMEI2)) > 0 ? g_opt->szCMEI_IMEI2:szStars,
        strlen((const char *)(g_opt->szIMSI)) > 0 ? g_opt->szIMSI:szStars,
        strlen((const char *)(g_opt->szDMv)) > 0 ? g_opt->szDMv:szStars);

    if (strlen((const char *)name) <= 0)
    {
        name=szEpname;
    }

    if (strlen((const char *)(g_opt->szPwd)) > 0)
    {
        passwd = g_opt->szPwd;
    }
    else
    {
        OSI_LOGI(0x10007bcd, "pwd is null,use default pwd is:%s~\n", passwd);
    }

    my_sha256(passwd,strlen((const char *)passwd),key);

    plaintext = name;

    /* Encrypt the plaintext */
    my_aes_encrypt((char *)key, (char *)plaintext, ciphertext, strlen((const char *)plaintext), &ciphertext_len);

    name = ciphertext; 

    testbase64 = name;

    base64tmp = (char *)lwm2m_malloc(EP_MEM_SIZE);//szEpname is lwm2m_free now,use again;
    if (base64tmp == NULL)
    {
        OSI_LOGI(0x10007bce, "mem err r4\n");
        ret = -1;
        goto out;
    }
    memset(base64tmp, 0, EP_MEM_SIZE);

    ret = j_base64_encode((unsigned char *)testbase64, ciphertext_len, &encData, (unsigned int *)&encDataLen);
    memcpy(base64tmp, encData, encDataLen);
    j_base64_free(encData, encDataLen);

    epnametmp = ( char *)lwm2m_malloc(EP_MEM_SIZE*4);
    if (epnametmp == NULL)
    {
        OSI_LOGI(0x10007bcf, "mem err\n,3");
        ret=-1;
        goto out;
    }
    memset(epnametmp, 0, EP_MEM_SIZE*4);

    snprintf((char *)epnametmp, EP_MEM_SIZE*4,"%s@#@%s@#@%s@#@%s@#@%s", type, base64tmp, g_opt->szAppKey, g_opt->szCMEI_IMEI, g_opt->szDMv);
    name = epnametmp;

    *data = (char *)lwm2m_malloc(strlen((const char *)name)+1);
    if (*data == NULL)
    {
        ret=-1;
        goto out;
    }

    memset(*data, 0, strlen((const char *)name)+1);
    memcpy(*data, name, strlen((const char *)name));
    ret = 0;

out:
    if (szEpname)
    {
        lwm2m_free(szEpname);
        szEpname = NULL;
    }

    if (ciphertext)
    {
        lwm2m_free(ciphertext);
        ciphertext = NULL;
    }

    if (epnametmp)
    {
        lwm2m_free(epnametmp);
        epnametmp = NULL;
    }

    if (base64tmp)
    {
        lwm2m_free(base64tmp);
        base64tmp = NULL;
    }
    return ret;
}

int genDmUpdateEndpointName(char *type, char **data,void *dmconfig)
{
#define EP_MEM_SIZE  (264)
    char key[64] = {0};
    char *szEpname = NULL;
    char *ciphertext = NULL;
    char *szStars="****";
    char * name = "";
    int  ciphertext_len;
    char *base64tmp = NULL;
    char *epnametmp = NULL;
    int ret = -1;
    char *passwd = "00000000000000000000000000000000";
    Options *g_opt=(Options *)dmconfig;
    /* A 128 bit IV */
    /* Buffer for the decrypted text */
    char *plaintext =  "plaintext";
    //base64
    char *testbase64="123456789";//MTIzNDU2Nzg5
    unsigned char *encData = 0;
    int encDataLen = 0;

    szEpname=( char *)lwm2m_malloc(EP_MEM_SIZE);
    if (szEpname == NULL)
    {
        OSI_LOGI(0x10007bd0, "mem err u1\n");
        ret=-1;
        goto out;
    }

    ciphertext=( char *)lwm2m_malloc(EP_MEM_SIZE);
    if (ciphertext == NULL)
    {
        OSI_LOGI(0x10007bd1, "mem err u2\n");
        ret=-1;
        goto out;
    }

    memset(ciphertext,0,EP_MEM_SIZE);
    memset(szEpname,0,EP_MEM_SIZE);
    snprintf((char *)szEpname,EP_MEM_SIZE,"%s-%s-%s-%s",
        strlen((const char *)(g_opt->szCMEI_IMEI))>0?g_opt->szCMEI_IMEI:szStars,
        strlen((const char *)(g_opt->szCMEI_IMEI2))>0?g_opt->szCMEI_IMEI2:szStars,
        strlen((const char *)(g_opt->szIMSI))>0?g_opt->szIMSI:szStars,szStars);

    if (strlen((const char *)name) <= 0)
    {
        name = szEpname;
    }

    if (strlen((const char *)(g_opt->szPwd)) > 0)
    {
        passwd = g_opt->szPwd;
    }
    else
    {
        OSI_LOGI(0x10007bcd, "pwd is null,use default pwd is:%s~\n", passwd);
    }

    my_sha256(passwd,strlen((const char *)passwd),key);

    plaintext = name;

    /* Encrypt the plaintext */
    my_aes_encrypt((char *)key, plaintext, ciphertext, strlen((const char *)plaintext), &ciphertext_len);
    name = ciphertext;

    testbase64 = name;
    base64tmp=( char *)lwm2m_malloc(EP_MEM_SIZE);//szEpname is lwm2m_free now,use again;
    if (base64tmp==NULL)
    {
        OSI_LOGI(0x10007bd2, "mem err u4\n");
        goto out;
    }

    memset(base64tmp, 0, EP_MEM_SIZE);

    ret= j_base64_encode((unsigned char *)testbase64, ciphertext_len, &encData,(unsigned int *)&encDataLen);
    memcpy(base64tmp, encData, encDataLen);
    j_base64_free(encData, encDataLen);

    epnametmp = (char *)lwm2m_malloc(EP_MEM_SIZE*4);
    if (epnametmp == NULL)
    {
        OSI_LOGI(0x10007bd3, "mem err u3\n");ret=-1;
        goto out;
    }
    memset(epnametmp,0,EP_MEM_SIZE*4);

    sprintf((char *)epnametmp,"%s@#@%s@#@%s@#@%s@#@%s", type, base64tmp, g_opt->szAppKey, g_opt->szCMEI_IMEI, g_opt->szDMv);

    name = epnametmp;
    *data = (char *)lwm2m_malloc(strlen((const char *)name)+1);
    if (*data == NULL)
    {
        ret=-1;
        goto out;
    }

    memset(*data, 0, strlen((const char *)name)+1);
    memcpy(*data, name, strlen((const char *)name));
    ret = 0;

out:
    if (szEpname)
    {
        lwm2m_free(szEpname);
        szEpname=NULL;
    }

    if (ciphertext)
    {
        lwm2m_free(ciphertext);
        ciphertext=NULL;
    }

    if (epnametmp)
    {
        lwm2m_free(epnametmp);
        epnametmp=NULL;
    }

    if (base64tmp)
    {
        lwm2m_free(base64tmp);
        base64tmp=NULL;
    }
    return ret;
}

int prv_getDmUpdateQueryLength(void *contextP,
                                          void *server)
{
    int index;

    index = strlen("epi=");
    index += strlen((const char *)(((lwm2m_context_t *)contextP)->DMprivData));
    return index + 1;
}

int prv_getDmUpdateQuery(void *contextP,
                                    void *server,
                                    uint8_t *buffer,
                                    size_t length)
{
    int index;
    int res;

    index = utils_stringCopy((char *)buffer, length, "epi=");
    if (index < 0) return 0;
    res = utils_stringCopy((char *)buffer + index, length - index, (const char *)(((lwm2m_context_t *)contextP)->DMprivData));
    if (res < 0) return 0;
    index += res;

    if(index < (int)length)
    {
        buffer[index++] = '\0';
    }
    else
    {
        return 0;
    }

    return index;
}

int dm_read_info_encode(char *szin,char **szout, char *pwd)
{
#define EP_MEM_SIZE  (264)
    char key[64] = {0};
    char *szEpname = NULL;
    char *ciphertext = NULL;
    char *name = "";
    int  ciphertext_len;
    int  ret = 0;
    char *base64tmp = NULL;
    char *epnametmp = NULL;
    char *passwd ="00000000000000000000000000000000";
    /* A 128 bit IV */
    /* Buffer for the decrypted text */
    char *plaintext =  "plaintext";
    //base64
    char *testbase64 = "123456789";//MTIzNDU2Nzg5
    unsigned char *encData = 0;
    int encDataLen = 0;

    szEpname=(char *)lwm2m_malloc(EP_MEM_SIZE);
    if (szEpname == NULL)
    {
        ret = -1;
        goto out;
    }

    ciphertext=(char *)lwm2m_malloc(EP_MEM_SIZE);
    if (ciphertext == NULL)
    {
        ret = -1;
        goto out;
    }

    memset(ciphertext, 0, EP_MEM_SIZE);
    memset(szEpname, 0, EP_MEM_SIZE);
    sprintf(szEpname, "%s", szin);

    if (strlen((const char *)name) <= 0)
    {
        name = szEpname;
    }

    if (strlen((const char *)(pwd)) > 0)
    {
        passwd = pwd;
    }
    else
    {
        OSI_LOGI(0x10007bd4, "dm_read_info_encode pwd is null, use default passwd");
    }

    my_sha256(passwd,strlen((const char *)passwd),key);

    plaintext = name;

    /* Encrypt the plaintext */
    my_aes_encrypt((char *)key, (char *)plaintext, ciphertext, strlen((const char *)plaintext), &ciphertext_len);

    name = ciphertext;

    testbase64 = name;

    base64tmp = (char *)lwm2m_malloc(EP_MEM_SIZE);//szEpname is lwm2m_free now,use again;
    if (base64tmp == NULL)
    {
        OSI_LOGI(0x10007bce, "mem err r4\n");
        ret = -1;
        goto out;
    }
    memset(base64tmp, 0, EP_MEM_SIZE);

    ret = j_base64_encode((unsigned char *)testbase64, ciphertext_len, &encData, (unsigned int *)&encDataLen);
    memcpy(base64tmp, encData, encDataLen);
    j_base64_free(encData, encDataLen);

    epnametmp = ( char *)lwm2m_malloc(EP_MEM_SIZE*4);
    if (epnametmp == NULL)
    {
        ret = -1;
        goto out;
    }
    memset(epnametmp, 0, EP_MEM_SIZE*4);

    snprintf((char *)epnametmp, EP_MEM_SIZE*4, "%s", base64tmp);
    name = epnametmp;

    *szout = (char *)lwm2m_malloc(strlen((const char *)name)+1);
    if (*szout == NULL)
    {
        ret=-1;
        goto out;
    }

    memset(*szout, 0, strlen((const char *)name)+1);
    memcpy(*szout, name, strlen((const char *)name));
    ret = 0;
out:
    if (szEpname)
    {
        lwm2m_free(szEpname);
        szEpname = NULL;
    }

    if (ciphertext)
    {
        lwm2m_free(ciphertext);
        ciphertext = NULL;
    }

    if (epnametmp)
    {
        lwm2m_free(epnametmp);
        epnametmp = NULL;
    }

    if (base64tmp)
    {
        lwm2m_free(base64tmp);
        base64tmp = NULL;
    }
    return ret;
}
