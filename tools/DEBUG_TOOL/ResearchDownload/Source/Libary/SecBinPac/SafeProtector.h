#pragma once
#define DEFAULE_XOR_KEY (0x20160707)

class CSafeProtector
{
public:
    CSafeProtector(void);
    ~CSafeProtector(void);
    void SetSafeKey(DWORD dwKey);
    BOOL Encrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize);
    BOOL Decrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize);
    DWORD GetEncryptedSize(DWORD dwInSize);//return: 0xFFFFFFFF,if packet fail;other is successfully
private:
    //xor Encrypt and Decrypt
    BOOL XorEncrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize);
    BOOL XorDecrypted(unsigned char* pInBuf, DWORD dwInSize,LPBYTE &pOutBuf,DWORD &dwOutSize);
    DWORD GetXorEncryptedSize(DWORD dwInSize);

private:
    DWORD m_dwXorKey;
};
