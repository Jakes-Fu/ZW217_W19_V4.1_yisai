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
#ifndef __SNCI_LIB_H__
#define __SNCI_LIB_H__

#include "zdt_nfc.h"

#define DISCOVER_NTF_TIMEOUT            300//5ms
#define NCI_ACK_TIMEOUT                 500 //5ms  500
#define DATA_ACK_TIMEOUT                300//20ms

/*********************************************************************************************
    控制 Packet 格式:(NCI 包组成)
        MT(3bit)+PBF(1bit)+GID(4bit)+RFU(2bit)+OID(6bit)+payloadlen(8bit)+payload(nbits)
    MT:
        001b Control Packet - Command Message      （命令 , 首字节通常为 20)
        010b Control Packet - Response Message     （响应， 首字节通常为 40）
        011b Control Packet - Notification Message （通知， 首字节通常为 60）
    PBF:
        0b: 完整消息或者是分组消息的最后一个，无后续消息
        1b: 有后续消息
    GID:
        0000b  NCI Core
        0001b  RF Management
        0010b  NFCEE Management
        0011b  NFCC Management
        0100b  Test Management
    OID:
        命令ID，太多了，请参考规范
    payloadlen:
        一个字节，因此最大负载长度为255字节
*********************************************************************************************
    数据 Packet 格式:(NCI 包组成)
        MT(3bit)+PBF(1bit)+Conn ID(4bit)+RFU(6bit)+CR(2bit)+payloadlen(8bit)+payload(nbits)
    MT:
         000b Data Packet - Data Message      （首字节通常为 00)
    Conn ID:
        连接ID, 表示数据属于哪个逻辑通道，创建逻辑通道的时候分配 Conn ID
    CR:
        Credits， 取值为0~3
    payloadlen:
        数据负载的长度，最大为255
**********************************************************************************************/
//Message Type (MT)
#define MT_DATA_PACKET                  (uint8_t)((0x00<<5)&0xe0)//0x00  //数据消息
//GROUP IDENTIFIER(GID)
#define DATA_CONN_ID                    (uint8_t)(0x00) //需要解析出来
//Opcode IDENTIFIER(OID)
//NCI_CORE
#define DATA_RFU                        (uint8_t)(0x00)
/******************************************************************************************************/
//数据包：MT(3bit)+PBF(1bit)+Conn ID(4bit)+RFU(6bit)+CR(2bit)+payloadlen(8bit)+payload(nbits)
//控制包：MT(3bit)+PBF(1bit)+GID(4bit)+RFU(2bit)+OID(6bit)+payloadlen(8bit)+payload(nbits)
//Message Type (MT)
#define MT_CTL_CMD_PACKET               (uint8_t)((0x01<<5)&0xe0)//0x20  //控制信息 命令
#define MT_CTL_RES_PACKET               (uint8_t)((0x02<<5)&0xe0)//0x40  //控制信息 响应
#define MT_CTL_NTF_PACKET               (uint8_t)((0x03<<5)&0xe0)//0x60  //控制信息 通知
//GROUP IDENTIFIER(GID)
#define GID_NCI_CORE                    (uint8_t)(0x00) //定义设备主机（DH）和NFC控制器（NFCC）之间所需的基本NCI功能。
#define GID_RF_MANAGE                   (uint8_t)(0x01) //射频管理
#define GID_NFCEE_MANAGE                (uint8_t)(0x02) //NFCEE管理
#define GID_SLM                         (uint8_t)(0x0E) //捷联私有命令

#define GID_NFCCC_MANAGE                (uint8_t)(0x03)
#define GID_TEST_MANAGE                 (uint8_t)(0x04)

//Opcode IDENTIFIER(OID)
//NCI_CORE(20) 响应
#define OID_CORE_RESET                  (uint8_t)(0x00) //重启NFCC的控制消息
#define OID_CORE_INIT                   (uint8_t)(0x01) //初始化NFCC的控制消息
#define OID_CORE_SET_CONFIG             (uint8_t)(0x02) //配置参数设置控制消息
#define OID_CORE_GET_CONFIG             (uint8_t)(0x03) //读取当前配置的控制消息
#define OID_CONN_GREATE                 (uint8_t)(0x04) //DH连接创建控制消息
#define OID_CONN_CLOSE                  (uint8_t)(0x05) //连接关闭的控制消息
//NCI_CORE(60) 通知
#define OID_CORE_RESET_NTF              (uint8_t)(0x00) //重启NFCC的通知消息
#define OID_CORE_CONN_CREDITS_NTF       (uint8_t)(0x06) //连接信用管理的通知消息
#define OID_CORE_GENERIC_ERROR_NTF      (uint8_t)(0x07) //通用错误的控制消息
#define OID_CORE_INTERFACE_ERROR_NTF    (uint8_t)(0x08) //接口错误的控制消息
//RF_MANAGE(21) 响应
#define OID_RF_DISCOVER_MAP             (uint8_t)(0x00) //射频接口映射配置的控制消息
#define OID_RF_SET_ROUTING              (uint8_t)(0x01) //控制消息配置监听模式路由
#define OID_RF_GET_ROUTING              (uint8_t)(0x02) //读取NFCC监听模式路由的控制消息
#define OID_RF_DISCOVER                 (uint8_t)(0x03) //开始发现的控制消息
#define OID_RF_DISCOVER_SELECT          (uint8_t)(0x04) //控制消息选择已发现的目标
#define OID_RF_DEACTIVATE               (uint8_t)(0x06) //射频接口去激活控制信息
#define OID_RF_T3T_POLLING              (uint8_t)(0x08) //控制消息请求NFCC发送Type 3标签轮询命令
#define OID_RF_PARAMETER_UPDATE         (uint8_t)(0x0b) //控制消息的RF参数更新
//RF_MANAGE(61) 通知
#define OID_RF_GET_ROUTING_NTF          (uint8_t)(0x02) //读取NFCC监听模式路由的控制消息
#define OID_RF_DISCOVER_NTF             (uint8_t)(0x03) //开始发现的控制消息
#define OID_RF_INTF_ACTIVATED_NTF       (uint8_t)(0x05) //射频接口激活通知
#define OID_RF_DEACTIVATE_NTF           (uint8_t)(0x06) //射频接口去激活控制信息
#define OID_RF_FIELD_INFO_NTF           (uint8_t)(0x07) //射频FIELD信息
#define OID_RF_T3T_POLLING_NTF          (uint8_t)(0x08) //控制消息请求NFCC发送Type 3标签轮询命令
#define OID_RF_NFCEE_ACTION_NTF         (uint8_t)(0x09) //报告NFCEE动作的通知
#define OID_RF_NFCEE_DISCOVERY_REQ_NTF  (uint8_t)(0x0a) //NFCEE发现请求通知
/******************************************************************************************************/

//TOUTING BASE
#define Technology_based                (uint8_t)(0x00)
#define Protocol_based                  (uint8_t)(0x01)
#define AID_based                       (uint8_t)(0x02)
//NFCEE_IDS
#define DH_NFCEE_ID                     (uint8_t)(0x00)
//POWER STATES
#define SWITCHED_ON                     (uint8_t)(0x01)
#define SWITCHED_OFF                    (uint8_t)(0x02)
#define BATTERY_OFF                     (uint8_t)(0x04)
//RF TECHNOLOGY
#define NFC_TECHNOLOGY_A                (uint8_t)(0x00)
#define NFC_TECHNOLOGY_B                (uint8_t)(0x01)
#define NFC_TECHNOLOGY_F                (uint8_t)(0x02)
#define NFC_TECHNOLOGY_15693            (uint8_t)(0x03)
#define NFC_TECHNOLOGY_RFU_05           (uint8_t)(0x05)
//RF Protocols
//#define PROTOCOL_ISO_DEP                (uint8_t)(0x04)
//#define PROTOCOL_NFC_DEP                (uint8_t)(0x05)
//NCI_NFCC_PARAM_ID
#define NCI_TOTAL_DURATION              0x00
#define NCI_CON_DEVICES_LIMIT           0x01
#define NCI_PN_NFC_DEP_SPEED            0x28
#define NCI_PI_BIT_RATE                 0x21
#define NCI_RF_FIELD_INFO               0x80
#define NCI_RF_NFCEE_ACTION             0x81
#define NCI_PF_BIT_RATE                 0x18
#define NCI_PROTOCOL_ISO_DEP            0x04
#define NCI_PROTOCOL_NFC_DEP            0x05
#define NCI_NFC_A_ACTIVE_POLL_MODE      0x03


#define NCI_SDD_VALUE                   0x30
#define NCI_PLATFORM_CONFIG             0x31
#define NCI_LA_SEL_INFO                 0x32
#define NCI_NFC_ID                      0x33
#define NCI_LB_SENSEB_INFO              0x38
#define NCI_LF_PROTOCOL_TYPE            0x50
#define NCI_LF_CON_BITR_F               0x54
#define NCI_LI_BIT_RATEC                0x5B
#define NCI_LN_WT                       0x60
#define NCI_LN_ATR_RES_GEN_BYTES        0x61
#define NCI_LA_PASSIVE                  0x80
#define NCI_LB_PASSIVE                  0x81
#define NCI_RF_NFCDEP_OP                0x82
#define NCI_A_ACTIVE_LISTEN_MODE        0x83
//FIELD INFO
#define NTF_RF_FIELD_ON                 0x01
#define NTF_RF_FIELD_OFF                0x00
//RF DEACTIVATE MODE
#define IDEL_MODE                       0x00
#define SLEEP_MODE                      0x01
//寻卡类别、方式参数设置
typedef struct _APUD_TRAN {
    uint8_t     LEN;
    uint8_t     TYPE;
    uint8_t     DATA[256];
} SL_APUD_TRAN;

//NCI PACK
//NCI 包组成：MT(3bit)+PBF(1bit)+GID(8bit)+OID(8bit)+payloadlen(8bit)+payload(nbits)
typedef struct _SL_NCI_PACK {
    uint8_t  MT;
    uint8_t  PBF;
    uint8_t  GID;
    uint8_t  OID;
    uint8_t  PAYLOADLEN;
    uint8_t  PAYLOAD[256];
} SL_NCI_PACK;



#define PROTOCOL_UNDETERMINED           0x00
#define PROTOCOL_T1T                    0x01
#define PROTOCOL_T2T                    0x02
#define PROTOCOL_T3T                    0x03
#define PROTOCOL_ISO_DEP                0x04
#define PROTOCOL_NFC_DEP                0x05
#define PROTOCOL_T5T                    0x06
#define PROTOCOL_NDEF                   0x07
#define ForProprietaryUse               0x80

#define NCI_NFC_A_PASSIVE_POLL_MODE     0x00
#define NCI_NFC_B_PASSIVE_POLL_MODE     0x01
#define NCI_NFC_F_PASSIVE_POLL_MODE     0x02
#define NCI_NFC_V_PASSIVE_POLL_MODE     0x06
//V2.1 Table 68
typedef struct {
    uint16_t SENS_RES;     //ATQA 2个字节长度
    uint8_t  NFCID_LEN;    //uid长度
    uint8_t  NFCID[10];    //uid最多10字节 4字节 7字节 10字节
    uint8_t  SEL_RES_LEN;  //SAK长度 0 1
    uint8_t  SEL_RES;      //SAK
    uint8_t  HRx_LEN;      //HRx长度 0 2
    uint8_t  HRx[2];       //HRx 头ROM字节大小
} NFC_A_Poll_Mode;

typedef struct {
    uint8_t SENSB_LEN;     //响应参数长度 11 12
    uint8_t SENSB_RES[12]; //响应参数 UID(4)+APPdata(4)+APPdata(4)+Protocol(3)

    uint8_t UID_LEN;       //UID长度
    uint8_t UID[4];
    uint8_t AppData[4];
    uint8_t Protocol[4];  //3个或者4个字节
} NFC_B_Poll_Mode;

typedef struct {
    uint8_t Bit_Rate;      //1:212 kbps 2:424 kbps
    uint8_t SENSF_LEN;     //响应参数长度
    uint8_t SENSF_RES[18]; //响应参数

    uint8_t UID_LEN;       //UID长度
    uint8_t UID[8];

    uint8_t C1[2];
    uint8_t C2[6];
    uint8_t C3[2];        //6103里面
} NFC_F_Poll_Mode;

typedef struct {
    uint8_t RES_FLAG;      //响应参数标志
    uint8_t DSFID;         //DSFID
    uint8_t UID[8];        //UID
    uint8_t UID_LEN;       //UID长度
} NFC_V_Poll_Mode;

//CARD INFO
typedef struct {
    //公共部分 1
    uint8_t RfDiscoveryId;      //0 射频扫描ID   NFCC动态分配
    uint8_t Rfinterface;        //1 射频接口
    uint8_t RfProtocol;         //2 射频协议
    uint8_t TechnologyMode;     //3 射频技术与模式 A/B/F/V
    uint8_t MaxPayload;         //4 最大数据包有效负载大小
    uint8_t NumberCredits;      //5 初始节点Credits
    uint8_t LengthParameters;   //6 射频参数长度
    void  *RfTechnology;        //7 射频参数
    //公共部分 2
    uint8_t DataExchangeMode;    //1 数据交换射频技术与模式
    uint8_t DataTransmitBitRate; //2 数据交换传输比特率
    uint8_t DataReceiveBitRate;  //3 数据交换接收比特率
    uint8_t LengthActivation;    //4 激活参数长度
    void   *Activation;          //激活参数
    //共有参数
    uint8_t uidlen;              //uid长度
    uint8_t uid[10];             //uid最多10字节 4字节 7字节 10字节 8字节
} SL_CARD_INFO;


typedef enum {
    //返回值正确，为0
    NO_ERR                = 0x00, //成功
    //定义NCI协议层错误
    MT_ERROR              = 0x01,
    PBF_ERROR             = 0x02,
    GID_ERROR             = 0x03,
    OID_ERROR             = 0x04,
    DECODE_LEN_ERROR      = 0x05,
    ENCODE_LEN_ERROR      = 0x06,
    //6320应用层错误
    CORE_RST_ERROR        = 0x20,
    CORE_INT_ERROR        = 0x21,
    CORE_SELECT_ERROR     = 0x22,
    CORE_APDU_ERROR       = 0x23,
    CORE_CRTL_REG_ERROR   = 0x24,
    CORE_SET_CONFIG_ERROR = 0x25,
    RF_DISCOVER_MAP_ERROR = 0x26,
    //升级错误
    DOWN_START_ERROR      = 0x40,
    DOWN_DATA_ERROR       = 0x41,
    DOWN_STOP_ERROR       = 0x42,

    CMD_RECE_OK           = 0xF2,  //NCI命令接收成功
    CMD_TIME_OUT          = 0xF3,  //NCI命令超时
    CMD_RECE_WAIT         = 0xF4,  //NCI命令接收等待
} t_RetStatus;

typedef enum {
    DO_OK                 = 0x00,  //处理成功
    DO_FAIL               = 0x01,  //处理失败
    DO_TIMEOUT            = 0x02,  //处理超时
    DO_6105               = 0x03,  //收到6105消息
    DO_6103               = 0x04,  //收到6103消息
} t_DoStatus;

extern NFC_A_Poll_Mode NfcAPollMode;
extern NFC_B_Poll_Mode NfcBPollMode;
extern NFC_F_Poll_Mode NfcFPollMode;
extern NFC_V_Poll_Mode NfcVPollMode;
extern uint8_t         CurrentCard;   //当前正在处理的卡片
extern SL_CARD_INFO    NfcCardinfo[3];//网上最多看见3种组合 三频卡

uint16_t reading_buf(uint8_t *data);
uint16_t writing_buf(uint8_t* data, uint16_t len);

t_RetStatus Snci_decode(uint8_t * data, SL_NCI_PACK *const aptncipackdecode);
t_RetStatus Snci_encode(SL_NCI_PACK atpncipackencode, uint8_t mode);
t_RetStatus Snci_Receive_Res_Ntf(uint8_t MT);
t_RetStatus Snci_Receive(SL_NCI_PACK *nci_recv);
t_RetStatus Snci_Ntf(SL_APUD_TRAN *const nftdata, uint8_t oid, uint8_t gid);
t_RetStatus Snci_Data_Proc(SL_APUD_TRAN *const apdudata);
t_RetStatus Seperate_card_info_6103(SL_CARD_INFO *Cardinfo, SL_NCI_PACK *nci_recv);
t_RetStatus Seperate_card_info_6105(SL_CARD_INFO *Cardinfo, SL_NCI_PACK *nci_recv);

SL_NCI_PACK Snci_data_message(uint8_t *pbuf, uint8_t len) ;
SL_NCI_PACK Snci_Core_Reset(void);
SL_NCI_PACK Snci_Core_Init(void);
SL_NCI_PACK Snci_Get_Config(void);
SL_NCI_PACK Snci_Set_Config(void);
SL_NCI_PACK Snci_Set_PrivateConfig(uint8_t *pbuf,uint8_t len);
SL_NCI_PACK Snci_Rf_Discover_Map(void);
SL_NCI_PACK Snci_Rf_Idel_Mode(void);
SL_NCI_PACK Snci_Rf_Sleep_Mode(void);
SL_NCI_PACK Snci_Rf_Discover(void);
SL_NCI_PACK Snci_Rf_Select_Card(SL_CARD_INFO *const dataout);
SL_NCI_PACK Snci_Rf_Reader_Send(SL_APUD_TRAN *const dataout, SL_APUD_TRAN *const datain);
SL_NCI_PACK Snci_Rf_Card_Tx(SL_APUD_TRAN *const dataout);
SL_NCI_PACK Snci_Rf_Card_Rx(SL_APUD_TRAN *const datain);
SL_NCI_PACK Snci_Rf_Set_Listen_Route(void);
SL_NCI_PACK Snci_RF_T3T_POLLING_CMD(void);

extern uint16_t ZDT_NFC_SIM_UID_IsOK(uint8_t * pUID,uint16_t uid_max_len);
extern uint8_t ZDT_NFC_HW_IsOK(void);

#endif

