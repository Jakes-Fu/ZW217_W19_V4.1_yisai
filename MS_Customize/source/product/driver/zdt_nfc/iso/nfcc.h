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

//6320状态字
typedef enum {
    NFCC_DEINIT = 0,                //0  nfcc去初始化
    NFCC_INIT,                      //1  nfcc初始化
    NFCC_WUPE_CARD,                 //2  开始寻卡
    NFCC_ATQ_A,                     //3  寻到A卡
    NFCC_ATQ_B,                     //4  寻到b卡
    NFCC_ATQ_F,                     //5  寻到f卡
    NFCC_ATQ_15693,                 //6  寻到15693
    NFCC_SLEEP,                     //7  nfcc睡眠
    NFCC_ACT_A,                     //8  激活A卡
    NFCC_ACT_B,                     //9  激活b卡
    NFCC_ACT_F,                     //10 激活f卡
    NFCC_ACT_15693,                 //11 激活15693
    NFCC_PASS_LA,                   //12 寻到A卡
    NFCC_PASS_LB,                   //13 寻到b卡
    NFCC_PASS_LF,                   //14 寻到f卡
    NFCC_ACTV_A,                    //15 P2P active模式
    NFCC_TRANSPARENT_TRANSPORT_RF,  //16 RF数据透传
    NFCC_TRANSPARENT_TRANSPORT_SE,  //17SE数据透传
} t_NFCC_States;


typedef enum {
    WAITING_STATE                  = 0x0000, //无命令等待状态
    //NCI Core CMD
    CORE_RESET_CMD                 = 0x2000, //重启NFCC的控制消息                     Table 5
    CORE_INIT_CMD                  = 0x2001, //初始化NFCC的控制消息                   Table 8
    CORE_SET_CONFIG_CMD            = 0x2002, //配置参数设置控制消息                   Table 10
    CORE_GET_CONFIG_CMD            = 0x2003, //配置参数设置控制消息                   Table 11
    CORE_CONN_CREATE_CMD           = 0x2004, //DH连接创建控制消息                     Table 13
    CORE_CONN_CLOSE_CMD            = 0x2005, //连接关闭的控制消息                     Table 16
    //RF Management CMD
    RF_DISCOVER_MAP_CMD            = 0x2100, //射频接口映射配置的控制消息             Table 42
    RF_SET_LISTEN_MODE_ROUTING_CMD = 0x2101, //控制消息配置监听模式路由               Table 44
    RF_GET_LISTEN_MODE_ROUTING_CMD = 0x2102, //读取NFCC监听模式路由的控制消息         Table 51
    RF_DISCOVER_CMD                = 0x2103, //开始发现的控制消息                     Table 52
    RF_DISCOVER_SELECT_CMD         = 0x2104, //控制消息选择已发现的目标               Table 60
    RF_DEACTIVATE_CMD              = 0x2106, //射频接口去激活控制信息                 Table 62
    RF_T3T_POLLING_CMD             = 0x2108, //控制消息请求NFCC发送Type 3标签轮询命令 Table 78
    RF_PARAMETER_UPDATE_CMD        = 0x210b, //控制消息的RF参数更新                   Table 71
} t_Nfc_Cmd;


//寻卡协议参数设置
typedef struct SL_POLLING_PARAM {
    uint8_t     POLLING_ENABLE;                 //为1则启动读卡功能，为0则关闭读卡功能
    uint8_t     PN_NFC_DEP_SPEED;               //data交互时通信速度配置，0用最高速度，1用默认速度，默认置1
    uint8_t     PI_BIT_RATE;                    //AB卡最大比特率0:106,1:212;2:424;3:848;4:1695;5:3390;6:6780默认置0
    uint8_t     RF_FIELD_INFO;                  //是否上报场状态，默认置0
    uint8_t     RF_NFCEE_ACTION;                //是否操作NFCEE，默认置0
    uint8_t     PF_BIT_RATE;                    //F卡初始比特率0:106,1:212;2:424;3:848;4:1695;5:3390;6:6780，默认置1
    uint16_t    TOTAL_DURATION;                 //POLL&IDEL 周期  F401 （1f4:500ms）注意是lsb的，这里按照msb，在nci中倒置，默认F401
    uint16_t    PROTOCOL_DEP_TYPE;              //0x02：ISO-DEP     0x03:NFC-DEP，默认用0x0102(01代表pollmode)
    uint16_t    PROTOCOL_NFC_TYPE;              //0x02：ISO-DEP     0x03:NFC-DEP，默认用0x0102(01代表pollmode)
    uint8_t     NFC_A_PASSIVE_POLL_MODE;        //为1使能寻A卡，  为0失能寻A卡（默认用1）
    uint8_t     NFC_B_PASSIVE_POLL_MODE;        //为1使能寻B卡，  为0失能寻B卡（默认用1）
    uint8_t     NFC_F_PASSIVE_POLL_MODE;        //为1使能寻F卡，  为0失能寻F卡（默认用1）
    uint8_t     NFC_15693_PASSIVE_POLL_MODE;    //为1使能寻15693，为0失能15693（默认用1）
} SL_POLLING_PARAM;
//模拟卡协议参数设置
typedef struct SL_LISTENCARD_PARAM {
    uint8_t     LISTEN_ENABLE;                  //为1则启动卡模拟功能，为0则关闭功能
    uint8_t     SIM_ENABLE;                     //为1表示SIM卡模拟     为0表示主机卡模拟
    uint8_t     P2P_ACTIVED_LISTEN;             //为1则启动卡模拟功能，为0则关闭功能
    uint8_t     P2P_ACTIVED_POLL;               //为1则启动卡模拟功能，为0则关闭功能
    uint16_t    TOTAL_DURATION;                 //POLL&IDEL 周期  F401 （1f4:500ms）注意是lsb的，这里按照msb，在nci中倒置，默认F401
    uint8_t     CON_DEVICES_LIMIT;              //碰撞设备数上限，默认为4
    uint8_t     SDD_VALUE;                      //SDD的值，sense_res的第一个字节，默认为8或4
    uint8_t     PLATFORM_CONFIG;                //platform的值，sense_res的第二个字节，默认为0
    uint8_t     LA_SEL_INFO;                    //sense_res的第3个字节，默认为0x20
    uint32_t    NFC_ID;                         //卡ID
    uint8_t     LB_SENSEB_INFO;                 //为1则支持ISO DEP,为0则不支持，默认为1
    uint8_t     LF_PROTOCOL_TYPE;               //为2则支持NFC DEP，默认为2
    uint8_t     LF_CON_BITR_F;                  //为6则F卡同时支持424和212k速率，默认为6
    uint8_t     LI_BIT_RATEC;                   //为0则支持106k，默认为0
    uint8_t     LN_WT;                          //默认置0x0E
    uint8_t     RF_FIELD_INFO;                  //是否上报场状态，默认置0
    uint8_t     RF_NFCEE_ACTION;                //是否操作NFCEE，默认置0
    uint8_t     RF_NFCDEP_OP;                   //NFCEE配置，默认置0x0E
    uint8_t     LA_PASSIVE_ENABLE;              //是否使能listenA卡，为1使能，默认1
    uint8_t     LB_PASSIVE_ENABLE;              //是否使能listenB卡，为1使能，默认0，AB不能同时使能
    uint8_t     P2P_ACTIVED_ENABLE;             //是否使能p2p功能
    uint8_t     LN_ATR_REQ_GEN_BYTES_LEN;       //P2P功能的atq的长度
    uint8_t     LN_ATR_REQ_GEN_BYTES[20];       //P2P功能的atq内容
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

