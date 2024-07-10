#ifndef _DES_H
#define _DES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

//执行单DES加密  加密一个块      8字节数据
//IV_IN_OUT：    初始化向量输入  密文输出  为空时则无向量值（CBC模式加密时，前一个数据的密文为后一个明文块的向量）
//Mes_IN_OUT:    明文输入        密文输出
//Key64bit:      64bit 秘钥      8字节
void One_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit);
//执行单DES解密  解密一个块      8字节数据
//IV_IN_OUT：    初始化向量输入  原密文输出 为空时则无向量值（CBC模式解密时，前一个数据块的密文为后一个密文块解密后的向量）
//Mes_IN_OUT:    密文输入        明文输出
//Key64bit:      64bit 秘钥      8字节
void One_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit);
//执行双DES加密  加密一个块      8字节数据 加密顺序  左加密 右解密 左加密
//IV_IN_OUT：    初始化向量输入  密文输出 （CBC模式加密时，前一个数据的密文为后一个明文块的向量）
//Mes_IN_OUT:    明文输入        密文输出
//Key128bit:     128bit 秘钥     16字节
void Two_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit);
//执行双DES解密  解密一个块      8字节数据 解密流程  左解密  右加密   左解密
//IV_IN_OUT：    初始化向量输入  原密文输出 为空时则无向量值（CBC模式解密时，前一个数据块的密文为后一个密文块解密后的向量）
//Mes_IN_OUT:    密文输入        明文输出
//Key128bit:     128bit 秘钥     16字节
void Two_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit);
//执行三DES加密  加密一个块      8字节数据 加密顺序  左加密 中解密 右加密
//IV_IN_OUT：    初始化向量输入  密文输出 （CBC模式加密时，前一个数据的密文为后一个明文块的向量）
//Mes_IN_OUT:    明文输入        密文输出
//Key192bit:     192bit 秘钥     24字节
void Three_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit);
//执行三DES解密  解密一个块      8字节数据 解密流程  左解密  中加密   右解密
//IV_IN_OUT：    初始化向量输入  原密文输出 为空时则无向量值（CBC模式解密时，前一个数据块的密文为后一个密文块解密后的向量）
//Mes_IN_OUT:    密文输入        明文输出
//Key192bit:     192bit 秘钥     24字节
void Three_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit);

void DES_Test(void) ;

#ifdef __cplusplus
}
#endif
#endif
