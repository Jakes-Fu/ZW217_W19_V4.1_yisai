#ifndef _APDU_H
#define _APDU_H


#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"
//APDU= ApplicationProtocol data unit, 是智能卡与智能卡读卡器之间传送的信息单元, （给智能卡发送的命令）指令（ISO 7816-4规范有定义）
//CLA   INS  P1  P2 Lc  Data  Le
//其中CLA为指令类别；INS为指令码；P1、P2为参数；Lc为Data的长度；Le为希望响应时回答的数据字节数，0表最大可能长度。
typedef struct _APUD_00 {//没有数据区域
    uint8_t CLA;      //CLA为指令类别
    uint8_t INS;      //INS为指令码
    uint8_t P1;       //P1参数
    uint8_t P2;       //P2参数
} APDU_00;
typedef struct _APUD_01 {//没有数据区域
    uint8_t CLA;      //CLA为指令类别
    uint8_t INS;      //INS为指令码
    uint8_t P1;       //P1参数
    uint8_t P2;       //P2参数
    uint8_t Le;       //Le为希望响应时回答的数据字节数，0表最大可能长度。
} APDU_01;
typedef struct _APUD_02 { //没有响应
    uint8_t CLA;      //CLA为指令类别
    uint8_t INS;      //INS为指令码
    uint8_t P1;       //P1参数
    uint8_t P2;       //P2参数
    uint8_t Lc;       //Lc为Data的长度
    uint8_t Data[256]; //data数据
} APDU_02;

typedef struct _APUD_03 { //都有
    //CLA INS P1 P2      Lc Data Le
    //     必备头         条件体
    uint8_t CLA;      //命令类别
    uint8_t INS;      //指令代码
    uint8_t P1;       //指令参数1
    uint8_t P2;       //指令餐数2

    uint8_t Lc;       //命令数据域中存在的字节数
    uint8_t Data[256]; //命令发送的数据（=LC）
    uint8_t Le;       //期望响应时回答的数据字节数，0表最大可能长度。
} APDU_03;



typedef struct _APUD_SEND {
    uint8_t CLA;      //CLA为指令类别
    uint8_t INS;      //INS为指令码
    uint8_t P1;       //P1参数
    uint8_t P2;       //P2参数

    uint8_t len;      //数据长度
    uint8_t data[256]; //数据内容
    uint16_t le;        //应答报文携带的数据长度
} APDU_SEND;

typedef struct _APUD_RES {
    uint8_t len;      //数据长度
    uint8_t data[256]; //数据内容

    uint8_t SW1;      // 返回命令处理状态
    uint8_t SW2;      // 返回命令处理限定
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
