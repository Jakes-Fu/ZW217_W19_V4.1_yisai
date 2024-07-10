#ifndef __ICODEC_H__
#define __ICODEC_H__

#ifdef  CODEC_EXPORTS
#define CODEC_API extern "C" __declspec(dllexport)
#else
#define CODEC_API extern "C" __declspec(dllimport)
#endif


enum {
    ALG_UNKNOWN = 0,
    ALG_AES     = 1,
    ALG_RSA2048 = 2,
};


CODEC_API int DecryptData(LPBYTE lpInput,DWORD dwInputLen, LPBYTE lpOutput, LPBYTE lpKey, DWORD dwAlg); 

CODEC_API BOOL RSA2048KeyGen(LPCWSTR lpszOutputFilePath); 


#endif //__ICODEC_H__