#ifndef _MIFARE_H
#define _MIFARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

#define NFC_AUTHENT1A        0x60               //��֤A��Կ
#define NFC_AUTHENT1B        0x61               //��֤B��Կ
#define NFC_READ             0x30               //����
#define NFC_WRITE            0xA0               //д��
#define NFC_DECREMENT        0xC0               //�ۿ�             �Դ洢����ֵ���е���ֵ��������������������浽��ʱ���ݼĴ���
#define NFC_INCREMENT        0xC1               //��ֵ             �Դ洢����ֵ���е���ֵ���ӷ���������������浽��ʱ���ݼĴ���
#define NFC_RESTORE          0xC2               //�������ݵ������� ����ֵ�����ݴ�����ʱ���ݼĴ���
#define NFC_TRANSFER         0xB0               //���滺���������� ����ʱ���ݼĴ���������д����ֵ��
#define NFC_RESET            0xE0               //��λ

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

extern t_DoStatus mifare_read_block(uint8_t *outbuf, uint8_t *pkey, uint8_t block);     //����
extern t_DoStatus mifare_write_block(uint8_t *inbuf, uint8_t *pkey, uint8_t block);     //д��
extern t_DoStatus mifare_Purse_init(int32_t value, uint8_t *pkey, uint8_t block);       //Ǯ����ʼ��
extern t_DoStatus mifare_Purse_balance(int32_t *balance, uint8_t *pkey, uint8_t block); //��ȡǮ�����
extern t_DoStatus mifare_Purse_deduction(int32_t value, uint8_t *pkey, uint8_t block);  //Ǯ���ۿ�
extern t_DoStatus mifare_Purse_increment(int32_t value, uint8_t *pkey, uint8_t block);  //Ǯ����ֵ
extern void mifaretest(void);                                                           //���Գ���

#ifdef __cplusplus
}
#endif
#endif
