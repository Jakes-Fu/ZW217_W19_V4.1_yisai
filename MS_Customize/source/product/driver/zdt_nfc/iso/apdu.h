#ifndef _APDU_H
#define _APDU_H


#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"
//APDU= ApplicationProtocol data unit, �����ܿ������ܿ�������֮�䴫�͵���Ϣ��Ԫ, �������ܿ����͵����ָ�ISO 7816-4�淶�ж��壩
//CLA   INS  P1  P2 Lc  Data  Le
//����CLAΪָ�����INSΪָ���룻P1��P2Ϊ������LcΪData�ĳ��ȣ�LeΪϣ����Ӧʱ�ش�������ֽ�����0�������ܳ��ȡ�
typedef struct _APUD_00 {//û����������
    uint8_t CLA;      //CLAΪָ�����
    uint8_t INS;      //INSΪָ����
    uint8_t P1;       //P1����
    uint8_t P2;       //P2����
} APDU_00;
typedef struct _APUD_01 {//û����������
    uint8_t CLA;      //CLAΪָ�����
    uint8_t INS;      //INSΪָ����
    uint8_t P1;       //P1����
    uint8_t P2;       //P2����
    uint8_t Le;       //LeΪϣ����Ӧʱ�ش�������ֽ�����0�������ܳ��ȡ�
} APDU_01;
typedef struct _APUD_02 { //û����Ӧ
    uint8_t CLA;      //CLAΪָ�����
    uint8_t INS;      //INSΪָ����
    uint8_t P1;       //P1����
    uint8_t P2;       //P2����
    uint8_t Lc;       //LcΪData�ĳ���
    uint8_t Data[256]; //data����
} APDU_02;

typedef struct _APUD_03 { //����
    //CLA INS P1 P2      Lc Data Le
    //     �ر�ͷ         ������
    uint8_t CLA;      //�������
    uint8_t INS;      //ָ�����
    uint8_t P1;       //ָ�����1
    uint8_t P2;       //ָ�����2

    uint8_t Lc;       //�����������д��ڵ��ֽ���
    uint8_t Data[256]; //����͵����ݣ�=LC��
    uint8_t Le;       //������Ӧʱ�ش�������ֽ�����0�������ܳ��ȡ�
} APDU_03;



typedef struct _APUD_SEND {
    uint8_t CLA;      //CLAΪָ�����
    uint8_t INS;      //INSΪָ����
    uint8_t P1;       //P1����
    uint8_t P2;       //P2����

    uint8_t len;      //���ݳ���
    uint8_t data[256]; //��������
    uint16_t le;        //Ӧ����Я�������ݳ���
} APDU_SEND;

typedef struct _APUD_RES {
    uint8_t len;      //���ݳ���
    uint8_t data[256]; //��������

    uint8_t SW1;      // ���������״̬
    uint8_t SW2;      // ����������޶�
} APDU_RES;


void CpuCardTest(void);
void CpuCardTest_FM1208_WhiteCard(void);

SL_NCI_PACK Snci_encode_Apdu_Res(uint8_t *data, uint8_t len);
t_DoStatus Snci_decode_Apdu(APDU_SEND *apdu, SL_NCI_PACK *nci_recv);
SL_NCI_PACK Snci_encode_Apdu(APDU_SEND apdu);
int8_t Snci_decode_Apdu_Res(uint8_t *buf, SL_NCI_PACK *nci_recv);
SL_NCI_PACK Snci_encode_Apdu_ext(APDU_SEND apdu, uint8_t le_flag);
void get_ndef(void);
#ifdef __cplusplus
}
#endif
#endif
