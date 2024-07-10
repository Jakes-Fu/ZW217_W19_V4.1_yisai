/**
 ****************************************************************
 * @file mh523.h
 *
 * @brief
 *
 * @author
 *
 *
 ****************************************************************
 */
#ifndef __NFCC_H__
#define __NFCC_H__

//#include "MyInclude.h"
#include "snci_lib.h"

#define NFCC_I2C                0
#define NFCC_SPI                1

//6320״̬��
typedef enum {
    NFCC_DEINIT = 0,                //0  nfccȥ��ʼ��
    NFCC_INIT,                      //1  nfcc��ʼ��
    NFCC_WUPE_CARD,                 //2  ��ʼѰ��
    NFCC_ATQ_A,                     //3  Ѱ��A��
    NFCC_ATQ_B,                     //4  Ѱ��b��
    NFCC_ATQ_F,                     //5  Ѱ��f��
    NFCC_ATQ_15693,                 //6  Ѱ��15693
    NFCC_SLEEP,                     //7  nfcc˯��
    NFCC_ACT_A,                     //8  ����A��
    NFCC_ACT_B,                     //9  ����b��
    NFCC_ACT_F,                     //10 ����f��
    NFCC_ACT_15693,                 //11 ����15693
    NFCC_PASS_LA,                   //12 Ѱ��A��
    NFCC_PASS_LB,                   //13 Ѱ��b��
    NFCC_PASS_LF,                   //14 Ѱ��f��
    NFCC_ACTV_A,                    //15 P2P activeģʽ
    NFCC_TRANSPARENT_TRANSPORT_RF,  //16 RF����͸��
    NFCC_TRANSPARENT_TRANSPORT_SE,  //17SE����͸��
} t_NFCC_States;


typedef enum {
    WAITING_STATE                  = 0x0000, //������ȴ�״̬
    //NCI Core CMD
    CORE_RESET_CMD                 = 0x2000, //����NFCC�Ŀ�����Ϣ                     Table 5
    CORE_INIT_CMD                  = 0x2001, //��ʼ��NFCC�Ŀ�����Ϣ                   Table 8
    CORE_SET_CONFIG_CMD            = 0x2002, //���ò������ÿ�����Ϣ                   Table 10
    CORE_GET_CONFIG_CMD            = 0x2003, //���ò������ÿ�����Ϣ                   Table 11
    CORE_CONN_CREATE_CMD           = 0x2004, //DH���Ӵ���������Ϣ                     Table 13
    CORE_CONN_CLOSE_CMD            = 0x2005, //���ӹرյĿ�����Ϣ                     Table 16
    //RF Management CMD
    RF_DISCOVER_MAP_CMD            = 0x2100, //��Ƶ�ӿ�ӳ�����õĿ�����Ϣ             Table 42
    RF_SET_LISTEN_MODE_ROUTING_CMD = 0x2101, //������Ϣ���ü���ģʽ·��               Table 44
    RF_GET_LISTEN_MODE_ROUTING_CMD = 0x2102, //��ȡNFCC����ģʽ·�ɵĿ�����Ϣ         Table 51
    RF_DISCOVER_CMD                = 0x2103, //��ʼ���ֵĿ�����Ϣ                     Table 52
    RF_DISCOVER_SELECT_CMD         = 0x2104, //������Ϣѡ���ѷ��ֵ�Ŀ��               Table 60
    RF_DEACTIVATE_CMD              = 0x2106, //��Ƶ�ӿ�ȥ���������Ϣ                 Table 62
    RF_T3T_POLLING_CMD             = 0x2108, //������Ϣ����NFCC����Type 3��ǩ��ѯ���� Table 78
    RF_PARAMETER_UPDATE_CMD        = 0x210b, //������Ϣ��RF��������                   Table 71
} t_Nfc_Cmd;


//Ѱ��Э���������
typedef struct SL_POLLING_PARAM {
    uint8_t     POLLING_ENABLE;                 //Ϊ1�������������ܣ�Ϊ0��رն�������
    uint8_t     PN_NFC_DEP_SPEED;               //data����ʱͨ���ٶ����ã�0������ٶȣ�1��Ĭ���ٶȣ�Ĭ����1
    uint8_t     PI_BIT_RATE;                    //AB����������0:106,1:212;2:424;3:848;4:1695;5:3390;6:6780Ĭ����0
    uint8_t     RF_FIELD_INFO;                  //�Ƿ��ϱ���״̬��Ĭ����0
    uint8_t     RF_NFCEE_ACTION;                //�Ƿ����NFCEE��Ĭ����0
    uint8_t     PF_BIT_RATE;                    //F����ʼ������0:106,1:212;2:424;3:848;4:1695;5:3390;6:6780��Ĭ����1
    uint16_t    TOTAL_DURATION;                 //POLL&IDEL ����  F401 ��1f4:500ms��ע����lsb�ģ����ﰴ��msb����nci�е��ã�Ĭ��F401
    uint16_t    PROTOCOL_DEP_TYPE;              //0x02��ISO-DEP     0x03:NFC-DEP��Ĭ����0x0102(01����pollmode)
    uint16_t    PROTOCOL_NFC_TYPE;              //0x02��ISO-DEP     0x03:NFC-DEP��Ĭ����0x0102(01����pollmode)
    uint8_t     NFC_A_PASSIVE_POLL_MODE;        //Ϊ1ʹ��ѰA����  Ϊ0ʧ��ѰA����Ĭ����1��
    uint8_t     NFC_B_PASSIVE_POLL_MODE;        //Ϊ1ʹ��ѰB����  Ϊ0ʧ��ѰB����Ĭ����1��
    uint8_t     NFC_F_PASSIVE_POLL_MODE;        //Ϊ1ʹ��ѰF����  Ϊ0ʧ��ѰF����Ĭ����1��
    uint8_t     NFC_15693_PASSIVE_POLL_MODE;    //Ϊ1ʹ��Ѱ15693��Ϊ0ʧ��15693��Ĭ����1��
} SL_POLLING_PARAM;
//ģ�⿨Э���������
typedef struct SL_LISTENCARD_PARAM {
    uint8_t     LISTEN_ENABLE;                  //Ϊ1��������ģ�⹦�ܣ�Ϊ0��رչ���
    uint8_t     SIM_ENABLE;                     //Ϊ1��ʾSIM��ģ��     Ϊ0��ʾ������ģ��
    uint8_t     P2P_ACTIVED_LISTEN;             //Ϊ1��������ģ�⹦�ܣ�Ϊ0��رչ���
    uint8_t     P2P_ACTIVED_POLL;               //Ϊ1��������ģ�⹦�ܣ�Ϊ0��رչ���
    uint16_t    TOTAL_DURATION;                 //POLL&IDEL ����  F401 ��1f4:500ms��ע����lsb�ģ����ﰴ��msb����nci�е��ã�Ĭ��F401
    uint8_t     CON_DEVICES_LIMIT;              //��ײ�豸�����ޣ�Ĭ��Ϊ4
    uint8_t     SDD_VALUE;                      //SDD��ֵ��sense_res�ĵ�һ���ֽڣ�Ĭ��Ϊ8��4
    uint8_t     PLATFORM_CONFIG;                //platform��ֵ��sense_res�ĵڶ����ֽڣ�Ĭ��Ϊ0
    uint8_t     LA_SEL_INFO;                    //sense_res�ĵ�3���ֽڣ�Ĭ��Ϊ0x20
    uint32_t    NFC_ID;                         //��ID
    uint8_t     LB_SENSEB_INFO;                 //Ϊ1��֧��ISO DEP,Ϊ0��֧�֣�Ĭ��Ϊ1
    uint8_t     LF_PROTOCOL_TYPE;               //Ϊ2��֧��NFC DEP��Ĭ��Ϊ2
    uint8_t     LF_CON_BITR_F;                  //Ϊ6��F��ͬʱ֧��424��212k���ʣ�Ĭ��Ϊ6
    uint8_t     LI_BIT_RATEC;                   //Ϊ0��֧��106k��Ĭ��Ϊ0
    uint8_t     LN_WT;                          //Ĭ����0x0E
    uint8_t     RF_FIELD_INFO;                  //�Ƿ��ϱ���״̬��Ĭ����0
    uint8_t     RF_NFCEE_ACTION;                //�Ƿ����NFCEE��Ĭ����0
    uint8_t     RF_NFCDEP_OP;                   //NFCEE���ã�Ĭ����0x0E
    uint8_t     LA_PASSIVE_ENABLE;              //�Ƿ�ʹ��listenA����Ϊ1ʹ�ܣ�Ĭ��1
    uint8_t     LB_PASSIVE_ENABLE;              //�Ƿ�ʹ��listenB����Ϊ1ʹ�ܣ�Ĭ��0��AB����ͬʱʹ��
    uint8_t     P2P_ACTIVED_ENABLE;             //�Ƿ�ʹ��p2p����
    uint8_t     LN_ATR_REQ_GEN_BYTES_LEN;       //P2P���ܵ�atq�ĳ���
    uint8_t     LN_ATR_REQ_GEN_BYTES[20];       //P2P���ܵ�atq����
} SL_LISTENCARD_PARAM;

extern struct SL_POLLING_PARAM    gtPollingParam;
extern struct SL_LISTENCARD_PARAM gtListencardParam;
extern uint8_t Trans_Port;
extern uint8_t upgradeflag;
extern uint8_t info_data_bak[35];
void Polling_Param_Init(void);
void Listen_Param_Init(void);
t_RetStatus NFCC_Init(void);
t_RetStatus NFCC_Deinit(void);
void NFCC_I2C_DeInit(void);
void SetIoType(uint8_t *data);
void P2P_Param_Init(void);
void NFCC_Rest(uint8_t mode);
void NFCC_Set_BootMode(uint8_t mode);
void NFCC_Set_TransType(uint8_t transport);
void NFCC_ReceiveData(void);
#endif

