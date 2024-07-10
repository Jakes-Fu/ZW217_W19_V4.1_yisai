#include "stdafx.h"
#include "SafeProtector.h"

CSafeProtector::CSafeProtector(void)
{
    m_dwXorKey = DEFAULE_XOR_KEY;
}

CSafeProtector::~CSafeProtector(void)
{
}

void CSafeProtector::SetSafeKey(DWORD dwKey)
{
    m_dwXorKey = DEFAULE_XOR_KEY;
    if (dwKey)
    {
        m_dwXorKey = dwKey;
    }
}
BOOL CSafeProtector::Encrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize)
{
    return XorEncrypted(pInBuf,dwInSize,pOutBuf,dwOutSize);
}
BOOL CSafeProtector::Decrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize)
{
    return XorDecrypted(pInBuf,dwInSize,pOutBuf,dwOutSize);
}
DWORD CSafeProtector::GetEncryptedSize(DWORD dwInSize)
{
    return GetXorEncryptedSize(dwInSize);
}
DWORD CSafeProtector::GetXorEncryptedSize(DWORD dwInSize)
{
    return dwInSize;
}

BOOL CSafeProtector::XorEncrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize)
{
    if (NULL == pInBuf || 0 == dwInSize)
    {
        return FALSE;
    }

    dwOutSize = dwInSize;
    pOutBuf = new BYTE[dwOutSize];
    if(pOutBuf == NULL)
    {
        return FALSE;
    }
    memset(pOutBuf,0,dwOutSize);
    for(DWORD i=0; i<dwInSize; ++i)
    {
        unsigned int nEncryptedData;
        unsigned int nTmpData = *(pInBuf+i);	
        nEncryptedData = 0xffffffff - (m_dwXorKey^nTmpData);
        *(pOutBuf+i) = (BYTE)nEncryptedData;
    }

    return TRUE;
}
BOOL CSafeProtector::XorDecrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize)
{
    if (NULL == pInBuf || 0 == dwInSize)
    {
        return FALSE;
    }

    dwOutSize = dwInSize;
    pOutBuf = new BYTE[dwOutSize];
    if(pOutBuf == NULL)
    {
        return FALSE;
    }
    memset(pOutBuf,0,dwOutSize);
    for(DWORD i=0; i<dwInSize; ++i)
    {
        unsigned int nDecryptedData;
        unsigned int nTmpData = *(pInBuf+i);
        nDecryptedData =  ((0xffffffff - nTmpData) ^ m_dwXorKey) & 0x000000ff;
        *(pOutBuf+i) = (BYTE)nDecryptedData;
    }

    return TRUE;
}