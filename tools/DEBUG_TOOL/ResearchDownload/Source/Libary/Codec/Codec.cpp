// Codec.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ICodec.h"
#include "simlock_rsa.h"
#include "simlock_aes.h"



#include <stdlib.h>


BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

BOOL InitRsaContext(rsa_context *ctx, LPBYTE lpKey)
{
    int re = simlock_rand( 0 );
    rsa_init(ctx, RSA_PKCS_V15, 0, simlock_rand, 0);
   
    ctx->len = *(int*)lpKey;
    lpKey += 4;

    if (ctx->len != 256)
    {
        return FALSE;
    }

    do 
    {
        ctx->N.p = (t_int*)malloc(256);
        if (ctx->N.p == NULL)
        {
            break;
        }
        memcpy(ctx->N.p, lpKey,256);
        lpKey += 256;
        ctx->N.n = 64;  // 256/4
        ctx->N.s = 1;

        ctx->E.p = (t_int*)malloc(4);
        if (ctx->E.p == NULL)
        {
            break;
        }
        memcpy(ctx->E.p, lpKey,4);
        lpKey += 4;
        ctx->E.n = 1;  // 4/4
        ctx->E.s = 1;

        ctx->D.p = (t_int*)malloc(256);
        if (ctx->D.p == NULL)
        {
            break;
        }
        memcpy(ctx->D.p, lpKey,256);
        lpKey += 256;
        ctx->D.n = 64;  // 256/4
        ctx->D.s = 1;

        ctx->P.p = (t_int*)malloc(128);
        if (ctx->P.p == NULL)
        {
            break;
        }
        memcpy(ctx->P.p, lpKey,128);
        lpKey += 128;
        ctx->P.n = 32;  // 128/4
        ctx->P.s = 1;

        ctx->Q.p = (t_int*)malloc(128);
        if (ctx->Q.p == NULL)
        {
            break;
        }
        memcpy(ctx->Q.p, lpKey,128);
        lpKey += 128;
        ctx->Q.n = 32;  // 128/4
        ctx->Q.s = 1;

        ctx->DP.p = (t_int*)malloc(128);
        if (ctx->DP.p == NULL)
        {
            break;
        }
        memcpy(ctx->DP.p, lpKey,128);
        lpKey += 128;
        ctx->DP.n = 32;  // 128/4
        ctx->DP.s = 1;

        ctx->DQ.p = (t_int*)malloc(128);
        if (ctx->DQ.p == NULL)
        {
            break;
        }
        memcpy(ctx->DQ.p, lpKey,128);
        lpKey += 128;
        ctx->DQ.n = 32;  // 128/4
        ctx->DQ.s = 1;

        ctx->QP.p = (t_int*)malloc(128);
        if (ctx->QP.p == NULL)
        {
            break;
        }
        memcpy(ctx->QP.p, lpKey,128);
        lpKey += 128;
        ctx->QP.n = 32;  // 128/4
        ctx->QP.s = 1;

        if (rsa_check_privkey(ctx) != 0)
        {
            break;
        }
        return TRUE;

    } while (0);

    return FALSE;
}

CODEC_API int DecryptData(LPBYTE lpInput,DWORD dwInputLen, LPBYTE lpOutput,LPBYTE lpKey, DWORD dwAlg)
{
    if (dwAlg == ALG_AES)
    {
        if (!lpInput || (dwInputLen & 0xF) != 0 || !lpOutput || !lpKey)
        {
            return 1;
        }

        int nKeyLen = (int)*(DWORD*)lpKey;
        if (nKeyLen != 32)
        {
            return 1;
        }
        aes_context ctx = {0};

        aes_setkey_dec(&ctx, lpKey+4, 256);

        unsigned char iv[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

        aes_crypt_cbc(&ctx, AES_DECRYPT, (int)dwInputLen, iv, lpInput, lpOutput);

        return 0; 

    }
    else if (dwAlg == ALG_RSA2048)
    {
        if (!lpInput || dwInputLen != 256 || !lpOutput || !lpKey)
        {
            return 1;
        }
        rsa_context ctx;
        if (!InitRsaContext(&ctx,lpKey))
        {
            return 2;
        }

        if(rsa_private(&ctx,lpInput,lpOutput) == 0)
        {
            return 0;
        }
        else
        {
            return 3;
        }
    }
    else
    {
        return 1;
    }   
}


CODEC_API BOOL RSA2048KeyGen(LPCWSTR lpszOutputFilePath)
{
    rsa_context ctx;
    int re = simlock_rand( 0 );
    rsa_init(&ctx, RSA_PKCS_V15, 0, simlock_rand, 0);
    ctx.len = 256;
    if(rsa_gen_key(&ctx, ctx.len * 8, 65537))
    {
        return FALSE;
    }

    if( rsa_check_pubkey(&ctx) != 0 ||
        rsa_check_privkey(&ctx) != 0 )
    {
        rsa_free( &ctx );
        return FALSE;
    }

    TCHAR szPath[MAX_PATH] = {0};
    if (lpszOutputFilePath)
    {
        wcscpy_s(szPath,MAX_PATH,lpszOutputFilePath);
        CreateDirectory(lpszOutputFilePath,NULL);
        wcscat_s(szPath,MAX_PATH, L"\\rsakey.h");
    }
    else
    {
        wcscat_s(szPath,MAX_PATH, L"rsakey.h");
    }

    FILE * pFile = _wfopen(szPath, L"wb");
    if (!pFile)
    {
        rsa_free( &ctx );
        return FALSE;
    }
    int i = 0;
    int nCount = 0;
    fprintf_s(pFile,"uint8 public_key_e[] = {");

    
    for (i = 3; i>=0; --i)
    {
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.E.p)[i]);
    }

    fprintf_s(pFile,"};\r\n\r\n");


    fprintf_s(pFile,"uint8 public_key_n[] = {\r\n");
    fprintf_s(pFile,"      ");
 

    nCount = 0;
    for (i = 255; i>=0; --i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.N.p)[i]);
        nCount ++;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }
    fprintf_s(pFile,"};\r\n\r\n");

    fprintf_s(pFile,"uint8 privite_key[] = {\r\n");
    fprintf_s(pFile,"      ");
    
    nCount = 0;

    // len 
    for (i = 0; i< 4; ++i)
    {
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)&ctx.len)[i]);
    }
    nCount += 4;

    //N    
    for (i = 0; i< 256; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.N.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //E
    for (i = 0; i< 4; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.E.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //D
    for (i = 0; i< 256; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.D.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //P
    for (i = 0; i< 128; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.P.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //Q
    for (i = 0; i< 128; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.Q.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //DP
    for (i = 0; i< 128; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.DP.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //DQ
    for (i = 0; i< 128; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.DQ.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    //QP
    for (i = 0; i< 128; ++i)
    {  
        fprintf_s(pFile,"0x%02X, ", ((LPBYTE)ctx.QP.p)[i]);

        ++nCount;

        if (nCount % 16 == 0)
        {
            fprintf_s(pFile,"\r\n      ");
        }
    }

    fprintf_s(pFile,"};\r\n\r\n");

    fclose(pFile);

    rsa_free(&ctx);

    return TRUE;
}
