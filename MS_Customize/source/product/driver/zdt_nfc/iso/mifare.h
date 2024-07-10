#ifndef _MIFARE_H
#define _MIFARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

#define NFC_AUTHENT1A        0x60               //验证A密钥
#define NFC_AUTHENT1B        0x61               //验证B密钥
#define NFC_READ             0x30               //读块
#define NFC_WRITE            0xA0               //写块
#define NFC_DECREMENT        0xC0               //扣款             对存储在数值块中的数值做减法操作，并将结果存到临时数据寄存器
#define NFC_INCREMENT        0xC1               //充值             对存储在数值块中的数值做加法操作，并将结果存到临时数据寄存器
#define NFC_RESTORE          0xC2               //调块数据到缓冲区 将数值块内容存入临时数据寄存器
#define NFC_TRANSFER         0xB0               //保存缓冲区中数据 将临时数据寄存器的内容写入数值块
#define NFC_RESET            0xE0               //复位

typedef enum phNciNfc_ExtnReqId {
    phNciNfc_e_T1TXchgDataReq     = 0x10, /* T1T Raw Data Request from DH */
    phNciNfc_e_T1TWriteNReq       = 0x20, /* T1T N bytes write request from DH */
    phNciNfc_e_MfRawDataXchgHdr   = 0x10, /* MF Raw Data Request from DH */
    phNciNfc_e_MfWriteNReq        = 0x31, /* MF N bytes write request from DH */
    phNciNfc_e_MfReadNReq         = 0x32, /* MF N bytes read request from DH */
    phNciNfc_e_MfSectorSelReq     = 0x33, /* MF Block select request from DH */
    phNciNfc_e_MfPlusProxCheckReq = 0x28, /* MF + Prox check request for NFCC from DH */
    phNciNfc_e_MfcAuthReq         = 0x40, /* MFC Authentication request for NFCC from DH */
    phNciNfc_e_InvalidReq                 /* Invalid ReqId */
} phNciNfc_ExtnReqId_t;

extern t_DoStatus mifare_read_block(uint8_t *outbuf, uint8_t *pkey, uint8_t block);     //读块
extern t_DoStatus mifare_write_block(uint8_t *inbuf, uint8_t *pkey, uint8_t block);     //写块
extern t_DoStatus mifare_Purse_init(int32_t value, uint8_t *pkey, uint8_t block);       //钱包初始化
extern t_DoStatus mifare_Purse_balance(int32_t *balance, uint8_t *pkey, uint8_t block); //获取钱包余额
extern t_DoStatus mifare_Purse_deduction(int32_t value, uint8_t *pkey, uint8_t block);  //钱包扣款
extern t_DoStatus mifare_Purse_increment(int32_t value, uint8_t *pkey, uint8_t block);  //钱包充值
extern void mifaretest(void);                                                           //测试程序

#ifdef __cplusplus
}
#endif
#endif
