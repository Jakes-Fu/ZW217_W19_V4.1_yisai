#include "snci_lib.h"
#include "MyInclude.h"


NFC_A_Poll_Mode NfcAPollMode;
NFC_B_Poll_Mode NfcBPollMode;
NFC_F_Poll_Mode NfcFPollMode;
NFC_V_Poll_Mode NfcVPollMode;
uint8_t         CurrentCard = 0;   //当前正在处理的卡片
SL_CARD_INFO    NfcCardinfo[3];    //网上最多看到3种组合
static uint8_t         g_nfc_hw_init_ok = 0; 
static uint8_t         g_nfc_uid_buf[50] = {0};
static uint8_t         g_nfc_uid_len = 0;

uint16_t reading_buf(uint8_t *data) {
    uint16_t len = 0;
    if(Trans_Port == NFCC_SPI)
    {
        len = NFCC_SPI_Read_buff(data);
    }
    else 
    {
        len = i2cReadBytes(data);
    }
    //MYLOG(DEBUG,"接收数据");
//    if(len) {
//        MYHEX(INFO, data, len);
//    }
    return len;
}

uint16_t writing_buf(uint8_t* data, uint16_t len) {
    //MYLOG(DEBUG,"发送数据");
//    if(len){
//        MYHEX(INFO, data, len);
//    }
    if(Trans_Port == NFCC_SPI) {
        delay_ms(1);  //delay_us(500);
        NFCC_SPI_Write_buff(data, len);
        return 0;
    } else {
        //MYHEXS(DEBUG, data, len, "write_data:");
        return i2cWriteBytes(data, len);
    }
}

t_RetStatus Snci_decode(uint8_t * data, SL_NCI_PACK *const nci_recv) {//接收解码NCI数据
    nci_recv->MT  = data[0] & 0xe0;
    nci_recv->PBF = data[0] & 0x10;
    nci_recv->GID = data[0] & 0x0f;
    nci_recv->OID = data[1] & 0x3f;
    nci_recv->PAYLOADLEN = data[2];
    memcpy(nci_recv->PAYLOAD, &data[3], nci_recv->PAYLOADLEN);
    MYHEXS(INFO, data, data[2] + 3, "<<");
    return NO_ERR;
}

t_RetStatus Snci_encode(SL_NCI_PACK nci_send, uint8_t mode) {         //打包发送数据
    uint8_t  pbuf[BUFFER_BLOCK_SIZE];
    uint16_t plen = nci_send.PAYLOADLEN + 3 ;           //buf数据长度
    pbuf[0] = nci_send.MT | nci_send.GID;
    pbuf[1] = nci_send.OID;
    pbuf[2] = nci_send.PAYLOADLEN; //数据长度
    memcpy(pbuf + 3, nci_send.PAYLOAD, nci_send.PAYLOADLEN);
    if(mode) {
        put_buffer_to_list(pbuf, plen, &nci_tx_list); //放入链表 1
    } else {
        delay_ms(2);
        writing_buf(pbuf, plen);                      //直接发送 0
    }
    MYHEXS(INFO, pbuf, plen, ">>");
    return NO_ERR;
}

/***********************************************************************************************************************************/
void Gid_Nci_Core(SL_NCI_PACK  *nci_recv)  {         //响应包处理     <NFCC被动响应>
    uint8_t SoftVer[4] = {0};
    MYLOG(DEBUG, "Gid_Nci_Core OID = %x", nci_recv->OID);
    switch(nci_recv->OID) {
        case OID_CORE_RESET:
            MYLOG(DEBUG, "OID_CORE_RESET 重启NFCC的控制消息");
            break;
        case OID_CORE_INIT:
            MYLOG(DEBUG, "OID_CORE_INIT 初始化NFCC的控制消息");
            memcpy(SoftVer,nci_recv->PAYLOAD + nci_recv->PAYLOADLEN -4,4);
            MYHEXS(DEBUG, SoftVer, 4, "SoftVer  ");
            MYHEXS(DEBUG, (uint8_t *)(bank1_image+0xf7f0), 4, "bank1_Ver");
            if(memcmp(SoftVer,bank1_image+0xf7f0,4)!=0){
                upgradeflag =1;
                MYLOG(DEBUG, "Different versions Need upgrade %d",upgradeflag);
            } else {
                MYLOG(DEBUG, "Same version No upgrade required");
            }
            break;
        case OID_CORE_SET_CONFIG:
            MYLOG(DEBUG, "OID_CORE_SET_CONFIG 配置参数设置控制消息");
            break;
        case OID_CORE_GET_CONFIG:
            MYLOG(DEBUG, "OID_CORE_GET_CONFIG 读取当前配置的控制消息");
            if(nci_recv->PAYLOADLEN > 4 
                && nci_recv->PAYLOAD[1] == 0x01
                && nci_recv->PAYLOAD[2] == 0xB0
                && nci_recv->PAYLOAD[3] == 0xF9)
            {
                uint8_t left_len = nci_recv->PAYLOAD[4];
                uint8_t uid_len = nci_recv->PAYLOAD[5];
                MYLOG(DEBUG, "OID_CORE_GET_CONFIG - UID Len=%d",uid_len);
                g_nfc_hw_init_ok = 1;
                if(uid_len > 0)
                {
                    g_nfc_uid_len = uid_len;
                    memset(g_nfc_uid_buf,0,sizeof(g_nfc_uid_buf));
                    memcpy(g_nfc_uid_buf,&(nci_recv->PAYLOAD[6]),uid_len);
                }
            }
            break;
        case OID_CONN_GREATE:
            MYLOG(DEBUG, "OID_CONN_GREATE DH连接创建控制消息");
            break;
        case OID_CONN_CLOSE:
            MYLOG(DEBUG, "OID_CONN_CLOSE 连接关闭的控制消息");
            break;
        default: break;
    }
}

void Gid_Nci_Core_Ntf(SL_NCI_PACK  *nci_recv)  {     //通知包处理     <NFCC主动响应>
    MYLOG(DEBUG, "Gid_Nci_Core OID = %x", nci_recv->OID);
    switch(nci_recv->OID) {
        case OID_CORE_RESET_NTF:
            MYLOG(DEBUG, "OID_CORE_RESET_NTF 通用错误的控制消息");
            break;
        case OID_CORE_CONN_CREDITS_NTF:
            MYLOG(DEBUG, "OID_CORE_CONN_CREDITS_NTF 连接信用管理的通知消息");
            break;
        case OID_CORE_GENERIC_ERROR_NTF:
            MYLOG(DEBUG, "OID_CORE_GENERIC_ERROR_NTF 通用错误的控制消息");
            break;
        case OID_CORE_INTERFACE_ERROR_NTF:
            MYLOG(DEBUG, "OID_CORE_INTERFACE_ERROR_NTF 接口错误的控制消息");
            break;
        default: break;
    }
}

void Gid_Rf_Management(SL_NCI_PACK  *nci_recv)  {    //响应包处理     <NFCC被动响应>
    MYLOG(DEBUG, "Gid_Rf_Management OID = %x", nci_recv->OID);
    switch(nci_recv->OID) {
        case OID_RF_DISCOVER_MAP:
            MYLOG(DEBUG, "OID_RF_DISCOVER_MAP 射频接口映射配置的控制消息");
            break;
        case OID_RF_SET_ROUTING:
            MYLOG(DEBUG, "OID_RF_SET_ROUTING 控制消息配置监听模式路由");
            break;
        case OID_RF_GET_ROUTING:
            MYLOG(DEBUG, "OID_RF_GET_ROUTING 读取NFCC监听模式路由的控制消息");
            break;
        case OID_RF_DISCOVER:
            //MYLOG(DEBUG, "OID_RF_DISCOVER 开始寻卡");
            if(gtPollingParam.POLLING_ENABLE)   MYLOG(DEBUG, "读卡器模式 正在扫描卡片......");
            if(gtListencardParam.LISTEN_ENABLE) MYLOG(DEBUG, "卡模拟模式 正在等待读卡器扫描......");
            break;
        case OID_RF_DISCOVER_SELECT:
            MYLOG(DEBUG, "OID_RF_DISCOVER_SELECT 选卡");
            break;
        case OID_RF_DEACTIVATE:
            MYLOG(DEBUG, "OID_RF_DEACTIVATE 射频接口去激活控制信息");
            break;
        case OID_RF_T3T_POLLING:
            MYLOG(DEBUG, "OID_RF_T3T_POLLING 控制消息请求NFCC发送Type 3标签轮询命令");
            break;
        default: break;
    }
}

void Gid_Rf_Management_Ntf(SL_NCI_PACK  *nci_recv)  {    //响应包处理     <NFCC被动响应>
    MYLOG(DEBUG, "Gid_Rf_Management OID = %x", nci_recv->OID);
    switch(nci_recv->OID) {
        case OID_RF_GET_ROUTING_NTF:
            MYLOG(DEBUG, "OID_RF_GET_ROUTING_NTF 读取NFCC监听路由表的通知");
            break;
        case OID_RF_DISCOVER_NTF:
            MYLOG(DEBUG, "OID_RF_DISCOVER_NTF 始发现的控制消息");
            if(nci_recv->PAYLOAD[0] < 4) { //大于4不处理
                Seperate_card_info_6103(&NfcCardinfo[nci_recv->PAYLOAD[0] - 1], nci_recv);
            }
            break;
        case OID_RF_INTF_ACTIVATED_NTF: //6105 表示寻到卡片了
            MYLOG(DEBUG, "OID_RF_INTF_ACTIVATED_NTF =0x%02x 射频接口激活通知", nci_recv->PAYLOAD[3]);
            switch(nci_recv->PAYLOAD[3]) {
                case 0x00:  //NFC_A_PASSIVE_POLL_MODE
                case 0x01:  //NFC_B_PASSIVE_POLL_MODE
                case 0x02:  //NFC_F_PASSIVE_POLL_MODE
                case 0x06:  //NFC_V_PASSIVE_POLL_MODE
                    if(nci_recv->PAYLOAD[0] < 4) { //大于4不处理
                        Seperate_card_info_6105(&NfcCardinfo[nci_recv->PAYLOAD[0] - 1], nci_recv);
                    }
                    break;
                case 0x80:
                    break;
                case 0x81:
                    break;
                case 0x82:
                    break;
                case 0x83:
                    break;
            }
            break;
        case OID_RF_DEACTIVATE_NTF:
            //MYLOG(DEBUG, "OID_RF_DEACTIVATE_NTF 去激活通知");
            if(gtPollingParam.POLLING_ENABLE)   MYLOG(DEBUG, "OID_RF_DEACTIVATE_NTF 去激活通知");
            if(gtListencardParam.LISTEN_ENABLE) MYLOG(DEBUG, "卡模拟模式 正在等待读卡器扫描......");
            break;
        case OID_RF_FIELD_INFO_NTF:
            MYLOG(DEBUG, "OID_RF_FIELD_INFO_NTF 射频FIELD信息");
            break;
        case OID_RF_T3T_POLLING_NTF:
            MYLOG(DEBUG, "OID_RF_T3T_POLLING_NTF 控制消息请求NFCC发送Type 3标签轮询命令");
            break;
        case OID_RF_NFCEE_ACTION_NTF:
            MYLOG(DEBUG, "OID_RF_NFCEE_ACTION_NTF 报告NFCEE动作的通知");
            break;
        case OID_RF_NFCEE_DISCOVERY_REQ_NTF:
            MYLOG(DEBUG, "OID_RF_NFCEE_DISCOVERY_REQ_NTF NFCEE发现请求通知");
            break;
        default: break;
    }
}

void Gid_Nfcee_Management(SL_NCI_PACK  *nci_recv)  { //响应包处理     <NFCC被动响应>
    MYLOG(DEBUG, "Gid_Nfcee_Management OID = %x", nci_recv->OID);
    switch(nci_recv->OID) {
        default: break;
    }
}

void Gid_Nfcee_Management_Ntf(SL_NCI_PACK  *nci_recv)  { //响应包处理     <NFCC被动响应>
    MYLOG(DEBUG, "Gid_Nfcee_Management_Ntf OID = %x", nci_recv->OID);
    switch(nci_recv->OID) {
        default: break;
    }
}

//flag+info区数据（客户自行存flash备份）
uint8_t info_data_bak[35] = "\x00\x40\x02\x80\x04\x20\x01\x20\x01\x00\x94\x11\x00\x3a\x00\x00\x00\x00\x00\x01\x00\x49\x32\x43\x00\x54\x0c\xa8\x02\xff\x00\x00\xc3\xff\x07"; //5C 0C
uint8 Snci_get_info_data(void)
{
    return MMIZDT_NVGetNFC_INFO(info_data_bak);
}
void Gid_SLM_Ntf(SL_NCI_PACK  *nci_recv)  { //响应包处理     <NFCC被动响应>
    MYLOG(DEBUG, "Gid_SLM_Ntf OID = %x nci_recv->PAYLOADLEN = 0x%x", nci_recv->OID, nci_recv->PAYLOADLEN);
    switch(nci_recv->OID) {
        case 0x02: {
            switch(nci_recv->PAYLOADLEN) {
                case 0x02://flash data crc fail 需要升级固件
                    upgradeflag = 1;
                    MYLOG(DEBUG, "flash data crc fail! Firmware upgrade required %d",upgradeflag);
                    break;
                case 0x24://info crc ok         需要备份info区数据并存储《客户自行存flash》
                    MYLOG(DEBUG, "info crc ok 客户需要把此info区数据存储到flash备用！！！");
                    //开机会上报info区内容：6e0224 0022  4002800420012001009411003a0000000000010049324300540ca802ff0000c3ff07 需要备份首次上电的info内容，一旦发现info区损坏需要用此进行还原(34字节)
                    if(info_data_bak[0] == 0) {
                        memcpy(&info_data_bak[1], nci_recv->PAYLOAD + 2, 34); //备份info区数据，客户需要自行存flash备用 info区数据34字节
                        info_data_bak[0] = 1;
                        MMIZDT_NVSetNFC_INFO(info_data_bak);
                        MYHEXS(DEBUG, info_data_bak, 35, "info_data_bak");
                    }
                    break;
                case 0x26://info crc fail      需要恢复info区，并升级
                    if(info_data_bak[0]){
                        upgradeflag = 2;
                    } else {
                        upgradeflag = 1;
                    }
                    MYLOG(DEBUG, "info crc fail! Firmware upgrade required %d",upgradeflag);
                    break;
                default: break;
            }
            break;
        }
        default: break;
    }
}

void Mt_Data_Packet(SL_NCI_PACK  *nci_recv) { //data数据包处理 <NFCC被动响应>
    MYLOG(DEBUG, "Mt_Data_Packet");
    if(nci_recv->GID == 0x03) {
        MYLOG(DEBUG, "Mt_HCI_Packet");
        if((nci_recv->PAYLOADLEN == 0x07) && (nci_recv->PAYLOAD[0] == 0x81) && (nci_recv->PAYLOAD[1] == 0x12)) {
            uint8_t Resp[]  = "\x03\x00\x02\x81\x80";
            Resp[3] = nci_recv->PAYLOAD[0];
            //03 00 07 81 12 02 41 01 41 07 //报余额 PIPE=07
            Snci_Send_String(0, Resp, sizeof(Resp) - 1, NCI_ACK_TIMEOUT, nci_recv);
        }
        if((nci_recv->PAYLOADLEN == 0x02) && (nci_recv->PAYLOAD[0] == 0x87) && (nci_recv->PAYLOAD[1] == 0x03)) {
            uint8_t Resp[]  = "\x03\x00\x03\x87\x80\x00";
            //03 00 02 87 03   //SIM卡建立打开PIPE
            Resp[3] = nci_recv->PAYLOAD[0];
            Snci_Send_String(0, Resp, sizeof(Resp) - 1, NCI_ACK_TIMEOUT, nci_recv);
        }
        if((nci_recv->PAYLOADLEN == 0x03) && (nci_recv->PAYLOAD[0] == 0x81) && (nci_recv->PAYLOAD[1] == 0x15) && (nci_recv->PAYLOAD[2] == 0x02)) {
            uint8_t Resp[]  = "\x03\x00\x03\x81\x80\x00";
            //03 00 03 81 15 02  //清除所有PIPE
            Resp[3] = nci_recv->PAYLOAD[0];
            Snci_Send_String(0, Resp, sizeof(Resp) - 1, NCI_ACK_TIMEOUT, nci_recv);
        }
    }
}

void Mt_Res_Packet(SL_NCI_PACK  *nci_recv)  { //响应包处理     <NFCC被动响应>
    MYLOG(DEBUG, "Mt_Res_Packet GID = %x", nci_recv->GID);
    switch(nci_recv->GID) {
        case GID_NCI_CORE:     Gid_Nci_Core(nci_recv)         ; break; //定义设备主机（DH）和NFC控制器（NFCC）之间所需的基本NCI功能。
        case GID_RF_MANAGE:    Gid_Rf_Management(nci_recv)    ; break; //射频管理
        case GID_NFCEE_MANAGE: Gid_Nfcee_Management(nci_recv) ; break; //NFCEE管理
        default: break;
    }
}

void Mt_Ntf_Packet(SL_NCI_PACK  *nci_recv)  {    //通知数据包处理 <NFCC主动上传>
    MYLOG(DEBUG, "Mt_Ntf_Packet GID = %x", nci_recv->GID);
    switch(nci_recv->GID) {
        case GID_NCI_CORE:     Gid_Nci_Core_Ntf(nci_recv)         ; break; //定义设备主机（DH）和NFC控制器（NFCC）之间所需的基本NCI功能。
        case GID_RF_MANAGE:    Gid_Rf_Management_Ntf(nci_recv)    ; break; //射频管理
        case GID_NFCEE_MANAGE: Gid_Nfcee_Management_Ntf(nci_recv) ; break; //NFCEE管理
        case GID_SLM:          Gid_SLM_Ntf(nci_recv) ;              break; //捷联私有命令
        default: break;
    }
}

t_RetStatus Snci_Receive_Res_Ntf(uint8_t OID) {              //接收响应通知
    struct buf_entry *pbuf;
    SL_NCI_PACK  nci_recv;
//    SL_APUD_TRAN apdu_send;
//    SL_APUD_TRAN apdu_recv;
    pbuf = get_buffer_from_list(&nci_rx_list);    //接收数据
    if(pbuf != NULL) {                            //接收到数据
        Snci_decode(pbuf->block, &nci_recv);      //解码nci数据
        MYLOG(DEBUG, "MT = 0X%02x PBF = 0X%02x GID = 0X%02x OID = 0X%02x PAYLOADLEN 0X%02x", nci_recv.MT, nci_recv.PBF, nci_recv.GID, nci_recv.OID, nci_recv.PAYLOADLEN);
        switch(nci_recv.MT) {
            case MT_DATA_PACKET:    Mt_Data_Packet(&nci_recv) ; break; //data数据包处理 <NFCC被动响应>
            case MT_CTL_RES_PACKET: Mt_Res_Packet(&nci_recv)  ; break; //响应包处理     <NFCC被动响应>
            case MT_CTL_NTF_PACKET: Mt_Ntf_Packet(&nci_recv)  ; break; //通知数据包处理 <NFCC主动上传>
            default: break;
        }
        free_a_buf(&pbuf);                        //归还数据空间
        MYLOG(DEBUG, "CMD_RECE_OK <%x %x>", nci_recv.OID, OID);
        if(nci_recv.OID == OID) {
            return CMD_RECE_OK;                  //获取到正确响应
        }
    }
    return CMD_RECE_WAIT;
}


t_RetStatus Snci_Receive(SL_NCI_PACK *nci_recv) { //接收响应通知
    struct buf_entry *pbuf;
    pbuf = get_buffer_from_list(&nci_rx_list);    //接收数据
    if(pbuf != NULL) {                            //接收到数据
        Snci_decode(pbuf->block, nci_recv);       //解码nci数据
        MYLOG(DEBUG, "MT = 0X%02x PBF = 0X%02x GID = 0X%02x OID = 0X%02x PAYLOADLEN 0X%02x", nci_recv->MT, nci_recv->PBF, nci_recv->GID, nci_recv->OID, nci_recv->PAYLOADLEN);
        switch(nci_recv->MT) {
            case MT_DATA_PACKET:    Mt_Data_Packet(nci_recv) ; break; //data数据包处理 <NFCC被动响应>
            case MT_CTL_RES_PACKET: Mt_Res_Packet(nci_recv)  ; break; //响应包处理     <NFCC被动响应>
            case MT_CTL_NTF_PACKET: Mt_Ntf_Packet(nci_recv)  ; break; //通知数据包处理 <NFCC主动上传>
            default: break;
        }
        free_a_buf(&pbuf);                        //归还数据空间
        return CMD_RECE_OK;                  //获取到正确响应
    }
    return CMD_RECE_WAIT;
}
/***********************************************************************************************************************************/
SL_NCI_PACK Snci_data_message(uint8_t *pbuf, uint8_t len) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = 0;
    nci_send.OID = 0;
    nci_send.PAYLOADLEN = len;
    memcpy(nci_send.PAYLOAD, pbuf, len); //拷贝数据
    MYLOG(DEBUG, "Snci_data_message");
    return nci_send;
}

SL_NCI_PACK Snci_Core_Reset(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_NCI_CORE;
    nci_send.OID = OID_CORE_RESET;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = 0x01;      //复位并且rf配置也清零
    MYLOG(DEBUG, "Snci_Core_Reset");
    return nci_send;
}

SL_NCI_PACK Snci_Core_Init(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_NCI_CORE;
    nci_send.OID = OID_CORE_INIT;
    nci_send.PAYLOADLEN = 0x00;
    MYLOG(DEBUG, "Snci_Core_Init");
    return nci_send;
}

SL_NCI_PACK Snci_Get_Config(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_NCI_CORE;
    nci_send.OID = OID_CORE_GET_CONFIG;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = 0x00;      //复位并且rf配置也清零
    MYLOG(DEBUG, "Snci_Get_Config");
    return nci_send;
}

SL_NCI_PACK Snci_Set_Config(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_NCI_CORE;
    nci_send.OID = OID_CORE_SET_CONFIG;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = 0x00;//配置条数
    MYLOG(DEBUG, "Snci_Set_Config  POLLING_ENABLE = %d LISTEN_ENABLE = %d", gtPollingParam.POLLING_ENABLE, gtListencardParam.LISTEN_ENABLE);
    if(gtPollingParam.POLLING_ENABLE == 1) {
        //20 02 14 06 28 01 01 21 01 00 80 01 00 81 01 00 18 01 01 00 02 f4 01
        nci_send.PAYLOAD[1] = NCI_PN_NFC_DEP_SPEED;//28
        nci_send.PAYLOAD[2] = 0x01;
        nci_send.PAYLOAD[3] = gtPollingParam.PN_NFC_DEP_SPEED;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[4] = NCI_PI_BIT_RATE;    //21
        nci_send.PAYLOAD[5] = 0x01;
        nci_send.PAYLOAD[6] = gtPollingParam.PI_BIT_RATE;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[7] = NCI_RF_FIELD_INFO;  //80
        nci_send.PAYLOAD[8] = 0x01;
        nci_send.PAYLOAD[9] = gtPollingParam.RF_FIELD_INFO;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[10] = NCI_RF_NFCEE_ACTION;//81
        nci_send.PAYLOAD[11] = 0x01;
        nci_send.PAYLOAD[12] = gtPollingParam.RF_NFCEE_ACTION;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[13] = NCI_PF_BIT_RATE;   //18
        nci_send.PAYLOAD[14] = 0x01;
        nci_send.PAYLOAD[15] = gtPollingParam.PF_BIT_RATE;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[16] = NCI_TOTAL_DURATION;//00
        nci_send.PAYLOAD[17] = 0x02;
        nci_send.PAYLOAD[18] = (uint8_t)(0x00ff & gtPollingParam.TOTAL_DURATION);
        nci_send.PAYLOAD[19] = (uint8_t)(0x00ff & (gtPollingParam.TOTAL_DURATION >> 8));
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 4;
        nci_send.PAYLOAD[0]++;
    }
    #ifdef EN_P2P_ACTIVE_FUNC
    if((gtListencardParam.LISTEN_ENABLE == 1) || (gtListencardParam.P2P_ACTIVED_ENABLE == 1))
    #else
    if(gtListencardParam.LISTEN_ENABLE == 1)
    #endif
    {
        nci_send.PAYLOAD[1] = NCI_CON_DEVICES_LIMIT;
        nci_send.PAYLOAD[2] = 0x01;
        nci_send.PAYLOAD[3] = gtListencardParam.CON_DEVICES_LIMIT;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[4] = NCI_SDD_VALUE;
        nci_send.PAYLOAD[5] = 0x01;
        nci_send.PAYLOAD[6] = gtListencardParam.SDD_VALUE;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[7] = NCI_PLATFORM_CONFIG;
        nci_send.PAYLOAD[8] = 0x01;
        nci_send.PAYLOAD[9] = gtListencardParam.PLATFORM_CONFIG;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[10] = NCI_LA_SEL_INFO;
        nci_send.PAYLOAD[11] = 0x01;
        nci_send.PAYLOAD[12] = gtListencardParam.LA_SEL_INFO;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[13] = NCI_LB_SENSEB_INFO;
        nci_send.PAYLOAD[14] = 0x01;
        nci_send.PAYLOAD[15] = gtListencardParam.LB_SENSEB_INFO;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[16] = NCI_TOTAL_DURATION;
        nci_send.PAYLOAD[17] = 0x02;
        nci_send.PAYLOAD[18] = (uint8_t)(0x00ff & gtListencardParam.TOTAL_DURATION);
        nci_send.PAYLOAD[19] = (uint8_t)(0x00ff & (gtListencardParam.TOTAL_DURATION >> 8));
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 4;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[20] = NCI_LF_PROTOCOL_TYPE;
        nci_send.PAYLOAD[21] = 0x01;
        nci_send.PAYLOAD[22] = gtListencardParam.LF_PROTOCOL_TYPE;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[23] = NCI_LF_CON_BITR_F;
        nci_send.PAYLOAD[24] = 0x01;
        nci_send.PAYLOAD[25] = gtListencardParam.LF_CON_BITR_F;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[26] = NCI_LI_BIT_RATEC;
        nci_send.PAYLOAD[27] = 0x01;
        nci_send.PAYLOAD[28] = gtListencardParam.LF_CON_BITR_F;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[29] = NCI_LN_WT;
        nci_send.PAYLOAD[30] = 0x01;
        nci_send.PAYLOAD[31] = gtListencardParam.LN_WT;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[32] = NCI_RF_FIELD_INFO;
        nci_send.PAYLOAD[33] = 0x01;
        nci_send.PAYLOAD[34] = gtListencardParam.RF_FIELD_INFO;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[35] = NCI_RF_NFCEE_ACTION;
        nci_send.PAYLOAD[36] = 0x01;
        nci_send.PAYLOAD[37] = gtListencardParam.RF_NFCEE_ACTION;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[38] = NCI_RF_NFCDEP_OP;
        nci_send.PAYLOAD[39] = 0x01;
        nci_send.PAYLOAD[40] = gtListencardParam.RF_NFCDEP_OP;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
        nci_send.PAYLOAD[41] = NCI_NFC_ID;
        nci_send.PAYLOAD[42] = 0x04;
        nci_send.PAYLOAD[43] = (uint8_t)(gtListencardParam.NFC_ID >> 24);
        nci_send.PAYLOAD[44] = (uint8_t)(gtListencardParam.NFC_ID >> 16);
        nci_send.PAYLOAD[45] = (uint8_t)(gtListencardParam.NFC_ID >> 8);
        nci_send.PAYLOAD[46] = (uint8_t)(gtListencardParam.NFC_ID);
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 6;
        nci_send.PAYLOAD[0]++;
        #ifdef EN_P2P_ACTIVE_FUNC
        if(gtListencardParam.P2P_ACTIVED_ENABLE == 1) {
            nci_send.PAYLOAD[47] = NCI_LN_ATR_RES_GEN_BYTES;
            nci_send.PAYLOAD[48] = gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN;
            memcpy(&nci_send.PAYLOAD[49], gtListencardParam.LN_ATR_REQ_GEN_BYTES, gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN);
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN + 2;
            nci_send.PAYLOAD[0]++;
        }
        #endif
    }
    return nci_send;
}

SL_NCI_PACK Snci_Set_PrivateConfig(uint8_t *pbuf, uint8_t len) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_NCI_CORE;
    nci_send.OID = OID_CORE_SET_CONFIG;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = 0x00;//配置条数
    MYLOG(DEBUG, "Snci_Set_Config  POLLING_ENABLE = %d LISTEN_ENABLE = %d", gtPollingParam.POLLING_ENABLE, gtListencardParam.LISTEN_ENABLE);
    if(gtPollingParam.POLLING_ENABLE == 1) {
        //0x20 0x3 0x9 0x1   0xb0, 0x03, 0x04, 0x89, 0x8a, 0x8b, 0x8d,
        memcpy(&nci_send.PAYLOAD[1], pbuf, len);
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + len;
        nci_send.PAYLOAD[0]++;
    }
    #ifdef EN_P2P_ACTIVE_FUNC
    if((gtListencardParam.LISTEN_ENABLE == 1) || (gtListencardParam.P2P_ACTIVED_ENABLE == 1))
    #else
    if(gtListencardParam.LISTEN_ENABLE == 1)
    #endif
    {
        memcpy(&nci_send.PAYLOAD[1], pbuf, len);
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + len;
        nci_send.PAYLOAD[0]++;
        #ifdef EN_P2P_ACTIVE_FUNC
        if(gtListencardParam.P2P_ACTIVED_ENABLE == 1) {
            nci_send.PAYLOAD[47] = NCI_LN_ATR_RES_GEN_BYTES;
            nci_send.PAYLOAD[48] = gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN;
            memcpy(&nci_send.PAYLOAD[49], gtListencardParam.LN_ATR_REQ_GEN_BYTES, gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN);
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN + 2;
            nci_send.PAYLOAD[0]++;
        }
        #endif
    }
    return nci_send;
}
//注意在发送完Snci_Rf_Discover后，nfcc开始寻卡，发送Snci_Rf_Discover前必须发送一次Snci_Rf_Idel_Mode
SL_NCI_PACK Snci_Rf_Idel_Mode(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_RF_MANAGE;
    nci_send.OID = OID_RF_DEACTIVATE;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = IDEL_MODE;      //Idel Mode
    MYLOG(DEBUG, "Snci_Rf_Idel_Mode");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Sleep_Mode(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_RF_MANAGE;
    nci_send.OID = OID_RF_DEACTIVATE;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = SLEEP_MODE;      //Sleep Mode
    MYLOG(DEBUG, "Snci_Rf_Sleep_Mode");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Select_Card(SL_CARD_INFO *const dataout) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_RF_MANAGE;
    nci_send.OID = OID_RF_DISCOVER_SELECT;
    nci_send.PAYLOADLEN = 0x03;
    nci_send.PAYLOAD[0] = dataout->RfDiscoveryId;
    nci_send.PAYLOAD[1] = dataout->RfProtocol;
    nci_send.PAYLOAD[2] = dataout->Rfinterface;
    MYLOG(DEBUG, "Snci_Rf_Select_Card");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Reader_Send(SL_APUD_TRAN *const dataout, SL_APUD_TRAN *const datain) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = DATA_CONN_ID;
    nci_send.OID = DATA_RFU;
    nci_send.PAYLOADLEN = dataout->LEN;
    memcpy(nci_send.PAYLOAD, dataout->DATA, dataout->LEN);
    MYLOG(DEBUG, "Snci_Rf_Reader_Send");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Discover(void) {
    SL_NCI_PACK nci_send;
    uint16_t payload_cnt = 0;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_RF_MANAGE;
    nci_send.OID = OID_RF_DISCOVER;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = 0x00;      //配置条数
    payload_cnt = 1;
    if(gtPollingParam.POLLING_ENABLE == 1) {
        //21 03 09 04 00 01 01 01 02 01 06 01
        if(gtPollingParam.NFC_A_PASSIVE_POLL_MODE == 1) {
            nci_send.PAYLOAD[payload_cnt] = NCI_NFC_A_PASSIVE_POLL_MODE;//00
            payload_cnt++;
            nci_send.PAYLOAD[payload_cnt] = 1;
            payload_cnt++;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
        if(gtPollingParam.NFC_B_PASSIVE_POLL_MODE == 1) {
            nci_send.PAYLOAD[payload_cnt] = NCI_NFC_B_PASSIVE_POLL_MODE;//01
            payload_cnt++;
            nci_send.PAYLOAD[payload_cnt] = 1;
            payload_cnt++;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
        if(gtPollingParam.NFC_F_PASSIVE_POLL_MODE == 1) {
            nci_send.PAYLOAD[payload_cnt] = NCI_NFC_F_PASSIVE_POLL_MODE;//02
            payload_cnt++;
            nci_send.PAYLOAD[payload_cnt] = 1;
            payload_cnt++;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
        if(gtPollingParam.NFC_15693_PASSIVE_POLL_MODE == 1) {
            nci_send.PAYLOAD[payload_cnt] = NCI_NFC_V_PASSIVE_POLL_MODE;//06
            payload_cnt++;
            nci_send.PAYLOAD[payload_cnt] = 1;
            payload_cnt++;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
    }
    if(gtListencardParam.LISTEN_ENABLE == 1) {
        if(gtListencardParam.LA_PASSIVE_ENABLE == 1) {
            nci_send.PAYLOAD[1] = NCI_LA_PASSIVE;
            nci_send.PAYLOAD[2] = 0x01;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
        if(gtListencardParam.LB_PASSIVE_ENABLE == 1) {
            nci_send.PAYLOAD[1] = NCI_LB_PASSIVE;
            nci_send.PAYLOAD[2] = 0x01;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
    }
    #ifdef EN_P2P_ACTIVE_FUNC
    if(gtListencardParam.P2P_ACTIVED_ENABLE == 1) {
        if(gtListencardParam.P2P_ACTIVED_LISTEN == 1) {
            nci_send.PAYLOAD[1] = NCI_A_ACTIVE_LISTEN_MODE;
            nci_send.PAYLOAD[2] = 0x01;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        } else {
            nci_send.PAYLOAD[1] = NCI_NFC_A_ACTIVE_POLL_MODE;
            nci_send.PAYLOAD[2] = 0x01;
            nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 2;
            nci_send.PAYLOAD[0]++;
        }
    }
    #endif
    MYLOG(DEBUG, "Snci_Rf_Discover");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Set_Listen_Route(void) { //需要注意一下
    SL_NCI_PACK nci_send;
    if(gtListencardParam.LISTEN_ENABLE == 1) {
        nci_send.MT = MT_CTL_CMD_PACKET;
        nci_send.PBF = 0;
        nci_send.GID = GID_RF_MANAGE;
        nci_send.OID = OID_RF_SET_ROUTING;
        nci_send.PAYLOADLEN = 0x00;
        nci_send.PAYLOAD[0] = 0x00;      //no more
        nci_send.PAYLOAD[1] = 0x00;      //配置条数
        nci_send.PAYLOADLEN = 2;
        nci_send.PAYLOAD[2] = Technology_based;
        nci_send.PAYLOAD[3] = 0x03;
        nci_send.PAYLOAD[4] = DH_NFCEE_ID;
        nci_send.PAYLOAD[5] = SWITCHED_ON;
        nci_send.PAYLOAD[6] = NFC_TECHNOLOGY_F;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 5;
        nci_send.PAYLOAD[1]++;
        nci_send.PAYLOAD[7] = Protocol_based;
        nci_send.PAYLOAD[8] = 0x03;
        nci_send.PAYLOAD[9] = DH_NFCEE_ID;
        nci_send.PAYLOAD[10] = SWITCHED_ON | BATTERY_OFF;
        nci_send.PAYLOAD[11] = PROTOCOL_ISO_DEP;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 5;
        nci_send.PAYLOAD[1]++;
        nci_send.PAYLOAD[12] = Protocol_based;
        nci_send.PAYLOAD[13] = 0x03;
        nci_send.PAYLOAD[14] = DH_NFCEE_ID;
        nci_send.PAYLOAD[15] = SWITCHED_ON | BATTERY_OFF;
        nci_send.PAYLOAD[16] = 0xA0;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 5;
        nci_send.PAYLOAD[1]++;
        nci_send.PAYLOAD[17] = Protocol_based;
        nci_send.PAYLOAD[18] = 0x03;
        nci_send.PAYLOAD[19] = DH_NFCEE_ID;
        nci_send.PAYLOAD[20] = SWITCHED_ON;
        nci_send.PAYLOAD[21] = PROTOCOL_NFC_DEP;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 5;
        nci_send.PAYLOAD[1]++;
        #ifdef EN_P2P_ACTIVE_FUNC
    } else if(gtListencardParam.P2P_ACTIVED_ENABLE == 1) {
        nci_send.MT = MT_CTL_CMD_PACKET;
        nci_send.PBF = 0;
        nci_send.GID = GID_RF_MANAGE;
        nci_send.OID = OID_RF_SET_ROUTING;
        nci_send.PAYLOADLEN = 0x00;
        nci_send.PAYLOAD[0] = 0x00;      //no more
        nci_send.PAYLOAD[1] = 0x00;      //配置条数
        nci_send.PAYLOADLEN = 2;
        nci_send.PAYLOAD[2] = Protocol_based;
        nci_send.PAYLOAD[3] = 0x03;
        nci_send.PAYLOAD[4] = DH_NFCEE_ID;
        nci_send.PAYLOAD[5] = SWITCHED_ON;
        nci_send.PAYLOAD[6] = NFC_TECHNOLOGY_RFU_05;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 5;
        nci_send.PAYLOAD[1]++;
        #endif
    }
    MYLOG(DEBUG, "Snci_Rf_Set_Listen_Route");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Discover_Map(void) {
    SL_NCI_PACK nci_send;
    if(gtListencardParam.LISTEN_ENABLE == 1) {//卡模拟退出
        return nci_send;
    }
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_RF_MANAGE;
    nci_send.OID = OID_RF_DISCOVER_MAP;
    nci_send.PAYLOADLEN = 0x01;
    nci_send.PAYLOAD[0] = 0x00;      //配置条数
    nci_send.PAYLOAD[1] = NCI_PROTOCOL_ISO_DEP;
    nci_send.PAYLOAD[2] = (uint8_t)(0x00ff & (gtPollingParam.PROTOCOL_DEP_TYPE >> 8));
    nci_send.PAYLOAD[3] = (uint8_t)(0x00ff & gtPollingParam.PROTOCOL_DEP_TYPE);
    nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
    nci_send.PAYLOAD[0]++;
    #ifdef EN_P2P_ACTIVE_FUNC
    if(gtListencardParam.P2P_ACTIVED_ENABLE == 1) {
        nci_send.PAYLOAD[4] = NCI_PROTOCOL_NFC_DEP;
        nci_send.PAYLOAD[5] = (uint8_t)(0x00ff & (gtPollingParam.PROTOCOL_NFC_TYPE >> 8));
        nci_send.PAYLOAD[6] = (uint8_t)(0x00ff & gtPollingParam.PROTOCOL_NFC_TYPE);
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 3;
        nci_send.PAYLOAD[0]++;
    }
    #endif
    //21 00 04 01 04 01 02
    MYLOG(DEBUG, "Snci_Rf_Discover_Map");
    return nci_send;
}

SL_NCI_PACK Snci_RF_T3T_POLLING_CMD(void) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_CTL_CMD_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = GID_RF_MANAGE;
    nci_send.OID = OID_RF_T3T_POLLING;
    nci_send.PAYLOADLEN = 0x04;
    nci_send.PAYLOAD[0] = 0xff;      //配置条数  12FC0000
    nci_send.PAYLOAD[1] = 0xff;
    nci_send.PAYLOAD[2] = 0x01;
    nci_send.PAYLOAD[3] = 0x00;
    //210804 FFFF0100
    MYLOG(DEBUG, "Snci_RF_T3T_POLLING_CMD");
    return nci_send;
}

t_RetStatus Seperate_card_info_6103(SL_CARD_INFO *Cardinfo, SL_NCI_PACK *nci_recv) {
    uint8_t index;
    Cardinfo->RfDiscoveryId    = nci_recv->PAYLOAD[0];
    Cardinfo->RfProtocol       = nci_recv->PAYLOAD[1];
    if(Cardinfo->RfProtocol == 0x04) {
        Cardinfo->Rfinterface  = 0x02;
    }
    if(Cardinfo->RfProtocol == 0x80) {
        Cardinfo->Rfinterface  = 0x80;
    }
    Cardinfo->TechnologyMode   = nci_recv->PAYLOAD[2];
    Cardinfo->LengthParameters = nci_recv->PAYLOAD[3];//09
    if(Cardinfo->TechnologyMode == NCI_NFC_A_PASSIVE_POLL_MODE) {
        //        00112233 4455 66 7788 99 0A0B0C0D 0E0F        11223344
        //6105 14 01808000 ff01 09 0400 04 cef31070 0108        00000000   (mifare V1.0协议)
        //6105 16 01010100 ff01 0b 000c 04 45637700 00   02124c 00000000   (Tag1   V2.1协议)
        //6105 17 01010200 ff01 0c 4400 07 04053e32e33e81 0100  00000000   (Tag2   V1.0协议)
        NfcAPollMode.SENS_RES = (nci_recv->PAYLOAD[4] << 8) + nci_recv->PAYLOAD[5];
        NfcAPollMode.NFCID_LEN = nci_recv->PAYLOAD[6];                               //uid长度
        memcpy(NfcAPollMode.NFCID, nci_recv->PAYLOAD + 7, NfcAPollMode.NFCID_LEN);  //拷贝剩余数据
        index = 7 + NfcAPollMode.NFCID_LEN;                                          //修正位置
        NfcAPollMode.SEL_RES_LEN = nci_recv->PAYLOAD[index++];//SAK长度 0 1
        if(NfcAPollMode.SEL_RES_LEN) {
            NfcAPollMode.SEL_RES  = nci_recv->PAYLOAD[index++];//SAK
            //MYHEXS(DEBUG, dataout->uid, dataout->uidlen, "ATQA : %04x   SAK : %02x   UID :", dataout->atqa,dataout->cardtype);
        }
        if(Cardinfo->RfProtocol == PROTOCOL_T1T) {
            NfcAPollMode.HRx_LEN = nci_recv->PAYLOAD[index++]; //HRx长度 0 2
            if(NfcAPollMode.HRx_LEN) {
                NfcAPollMode.HRx[0] = nci_recv->PAYLOAD[index++];
                NfcAPollMode.HRx[1] = nci_recv->PAYLOAD[index++];
//            MYHEXS(DEBUG, dataout->uid, dataout->uidlen, "ATQA : %04x   Header BOM bytes : %02x   UID :", dataout->atqa,dataout->hrx);
            }
        }
        Cardinfo->RfTechnology = (NFC_A_Poll_Mode *)&NfcAPollMode;
        Cardinfo->uidlen = NfcAPollMode.NFCID_LEN;
        memcpy(Cardinfo->uid, NfcAPollMode.NFCID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    if(Cardinfo->TechnologyMode == NCI_NFC_B_PASSIVE_POLL_MODE) {
        //        00112233 4455 66 77 88990A0B 0C0D0E0D            11223344
        //6105 19 01020401 ff01 0c 0b 0a1c49bc 54434f53   b38381   01000002 0100
        //                              ID     APP DATA   Protocol
        //dataout->uidlen = 4;
        NfcBPollMode.SENSB_LEN = nci_recv->PAYLOAD[4];
        memcpy(NfcBPollMode.SENSB_RES, nci_recv->PAYLOAD + 5, NfcBPollMode.SENSB_LEN);
        NfcBPollMode.UID_LEN = 4;
        memcpy(NfcBPollMode.UID, NfcBPollMode.SENSB_RES, 4);
        memcpy(NfcBPollMode.AppData, NfcBPollMode.SENSB_RES + 4, 4);
        memcpy(NfcBPollMode.Protocol, NfcBPollMode.SENSB_RES + 8, NfcBPollMode.SENSB_LEN - 8);
        Cardinfo->RfTechnology = (NFC_B_Poll_Mode *)&NfcBPollMode;
        Cardinfo->uidlen = NfcBPollMode.UID_LEN;
        memcpy(Cardinfo->uid, NfcBPollMode.UID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    if(Cardinfo->TechnologyMode == NCI_NFC_F_PASSIVE_POLL_MODE) {
        //        00112233 4455 6677 88     UID(8)        2        6      11223344
        //6105 1d 01010302 ff01 1201 10 012e4ce15c954fbb 00f1 000000014300 02010100
        NfcFPollMode.Bit_Rate = nci_recv->PAYLOAD[4];
        NfcFPollMode.SENSF_LEN = nci_recv->PAYLOAD[5];
        memcpy(NfcFPollMode.SENSF_RES, nci_recv->PAYLOAD + 9, NfcFPollMode.SENSF_LEN);
        NfcFPollMode.UID_LEN = 8;
        memcpy(NfcFPollMode.UID, NfcFPollMode.SENSF_RES, 8);
        memcpy(NfcFPollMode.C1, NfcFPollMode.SENSF_RES + 8, 2);
        memcpy(NfcFPollMode.C2, NfcFPollMode.SENSF_RES + 10, 6);
        Cardinfo->RfTechnology = (NFC_F_Poll_Mode *)&NfcFPollMode;
        Cardinfo->uidlen = NfcFPollMode.UID_LEN;
        memcpy(Cardinfo->uid, NfcFPollMode.UID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    if(Cardinfo->TechnologyMode == NCI_NFC_V_PASSIVE_POLL_MODE) {
        //        00112233 4455 66 7788      UID(8)      11223344
        //6105 15 01010606 ff01 0a 0000 6023378b500104e0 06808000
        NfcVPollMode.RES_FLAG = nci_recv->PAYLOAD[4];
        NfcVPollMode.DSFID = nci_recv->PAYLOAD[5];
        memcpy(NfcVPollMode.UID, nci_recv->PAYLOAD + 9, 8);
        NfcVPollMode.UID_LEN = 8;
        Cardinfo->RfTechnology = (NFC_V_Poll_Mode *)&NfcVPollMode;
        Cardinfo->uidlen = NfcVPollMode.UID_LEN;
        memcpy(Cardinfo->uid, NfcVPollMode.UID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    return NO_ERR;
}

t_RetStatus Seperate_card_info_6105(SL_CARD_INFO *Cardinfo, SL_NCI_PACK *nci_recv) {
    uint8_t index;
    Cardinfo->RfDiscoveryId    = nci_recv->PAYLOAD[0];
    Cardinfo->Rfinterface      = nci_recv->PAYLOAD[1];
    Cardinfo->RfProtocol       = nci_recv->PAYLOAD[2];
    Cardinfo->TechnologyMode   = nci_recv->PAYLOAD[3];
    Cardinfo->MaxPayload       = nci_recv->PAYLOAD[4];
    Cardinfo->NumberCredits    = nci_recv->PAYLOAD[5];
    Cardinfo->LengthParameters = nci_recv->PAYLOAD[6];//09
    Cardinfo->DataExchangeMode    = nci_recv->PAYLOAD[7  + Cardinfo->LengthParameters];
    Cardinfo->DataTransmitBitRate = nci_recv->PAYLOAD[8  + Cardinfo->LengthParameters];
    Cardinfo->DataReceiveBitRate  = nci_recv->PAYLOAD[9  + Cardinfo->LengthParameters];
    Cardinfo->LengthActivation    = nci_recv->PAYLOAD[10 + Cardinfo->LengthParameters];
    if(Cardinfo->TechnologyMode == NCI_NFC_A_PASSIVE_POLL_MODE) {
        //        00112233 4455 66 7788 99 0A0B0C0D 0E0F        11223344
        //6105 14 01808000 ff01 09 0400 04 cef31070 0108        00000000   (mifare V1.0协议)
        //6105 16 01010100 ff01 0b 000c 04 45637700 00   02124c 00000000   (Tag1   V2.1协议)
        //6105 17 01010200 ff01 0c 4400 07 04053e32e33e81 0100  00000000   (Tag2   V1.0协议)
        NfcAPollMode.SENS_RES = (nci_recv->PAYLOAD[7] << 8) + nci_recv->PAYLOAD[8];
        NfcAPollMode.NFCID_LEN = nci_recv->PAYLOAD[9];                               //uid长度
        memcpy(NfcAPollMode.NFCID, nci_recv->PAYLOAD + 10, NfcAPollMode.NFCID_LEN);  //拷贝剩余数据
        index = 10 + NfcAPollMode.NFCID_LEN;                                          //修正位置
        NfcAPollMode.SEL_RES_LEN = nci_recv->PAYLOAD[index++];//SAK长度 0 1
        if(NfcAPollMode.SEL_RES_LEN) {
            NfcAPollMode.SEL_RES  = nci_recv->PAYLOAD[index++];//SAK
            //MYHEXS(DEBUG, dataout->uid, dataout->uidlen, "ATQA : %04x   SAK : %02x   UID :", dataout->atqa,dataout->cardtype);
        }
        if(Cardinfo->RfProtocol == PROTOCOL_T1T) {
            NfcAPollMode.HRx_LEN = nci_recv->PAYLOAD[index++]; //HRx长度 0 2
            if(NfcAPollMode.HRx_LEN) {
                NfcAPollMode.HRx[0] = nci_recv->PAYLOAD[index++];
                NfcAPollMode.HRx[1] = nci_recv->PAYLOAD[index++];
//            MYHEXS(DEBUG, dataout->uid, dataout->uidlen, "ATQA : %04x   Header BOM bytes : %02x   UID :", dataout->atqa,dataout->hrx);
            }
        }
        Cardinfo->RfTechnology = (NFC_A_Poll_Mode *)&NfcAPollMode;
        Cardinfo->uidlen = NfcAPollMode.NFCID_LEN;
        memcpy(Cardinfo->uid, NfcAPollMode.NFCID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    if(Cardinfo->TechnologyMode == NCI_NFC_B_PASSIVE_POLL_MODE) {
        //        00112233 4455 66 77 88990A0B 0C0D0E0D            11223344
        //6105 19 01020401 ff01 0c 0b 0a1c49bc 54434f53   b38381   01000002 0100
        //                              ID     APP DATA   Protocol
        //dataout->uidlen = 4;
        NfcBPollMode.SENSB_LEN = nci_recv->PAYLOAD[7];
        memcpy(NfcBPollMode.SENSB_RES, nci_recv->PAYLOAD + 8, NfcBPollMode.SENSB_LEN);
        NfcBPollMode.UID_LEN = 4;
        memcpy(NfcBPollMode.UID, NfcBPollMode.SENSB_RES, 4);
        memcpy(NfcBPollMode.AppData, NfcBPollMode.SENSB_RES + 4, 4);
        memcpy(NfcBPollMode.Protocol, NfcBPollMode.SENSB_RES + 8, NfcBPollMode.SENSB_LEN - 8);
        Cardinfo->RfTechnology = (NFC_B_Poll_Mode *)&NfcBPollMode;
        Cardinfo->uidlen = NfcBPollMode.UID_LEN;
        memcpy(Cardinfo->uid, NfcBPollMode.UID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    if(Cardinfo->TechnologyMode == NCI_NFC_F_PASSIVE_POLL_MODE) {
        //        00112233 4455 6677 88     UID(8)        2        6      11223344
        //6105 1d 01010302 ff01 1201 10 012e4ce15c954fbb 00f1 000000014300 02010100
        NfcFPollMode.Bit_Rate = nci_recv->PAYLOAD[7];
        NfcFPollMode.SENSF_LEN = nci_recv->PAYLOAD[8];
        memcpy(NfcFPollMode.SENSF_RES, nci_recv->PAYLOAD + 9, NfcFPollMode.SENSF_LEN);
        NfcFPollMode.UID_LEN = 8;
        memcpy(NfcFPollMode.UID, NfcFPollMode.SENSF_RES, 8);
        memcpy(NfcFPollMode.C1, NfcFPollMode.SENSF_RES + 8, 2);
        memcpy(NfcFPollMode.C2, NfcFPollMode.SENSF_RES + 10, 6);
        Cardinfo->RfTechnology = (NFC_F_Poll_Mode *)&NfcFPollMode;
        Cardinfo->uidlen = NfcFPollMode.UID_LEN;
        memcpy(Cardinfo->uid, NfcFPollMode.UID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    if(Cardinfo->TechnologyMode == NCI_NFC_V_PASSIVE_POLL_MODE) {
        //        00112233 4455 66 7788      UID(8)      11223344
        //6105 15 01010606 ff01 0a 0000 6023378b500104e0 06808000
        NfcVPollMode.RES_FLAG = nci_recv->PAYLOAD[7];
        NfcVPollMode.DSFID = nci_recv->PAYLOAD[8];
        memcpy(NfcVPollMode.UID, nci_recv->PAYLOAD + 9, 8);
        NfcVPollMode.UID_LEN = 8;
        Cardinfo->RfTechnology = (NFC_V_Poll_Mode *)&NfcVPollMode;
        Cardinfo->uidlen = NfcVPollMode.UID_LEN;
        memcpy(Cardinfo->uid, NfcVPollMode.UID, Cardinfo->uidlen);  //拷贝剩余数据
    }
    return NO_ERR;
}

//CLA   INS  P1  P2 Lc  Data  Le
t_RetStatus Snci_Data_Proc(SL_APUD_TRAN *const apdudata) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = DATA_CONN_ID;
    nci_send.OID = DATA_RFU;
    nci_send.PAYLOADLEN = apdudata->LEN;
    memcpy(nci_send.PAYLOAD, apdudata->DATA, nci_send.PAYLOADLEN);
    MYLOG(DEBUG, "Snci_Data_Proc");
    Snci_encode(nci_send, 1);
    return NO_ERR;
}

SL_NCI_PACK Snci_Rf_Card_Tx(SL_APUD_TRAN *const dataout) {
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = DATA_CONN_ID;
    nci_send.OID = DATA_RFU;
    nci_send.PAYLOADLEN = dataout->LEN;
    memcpy(nci_send.PAYLOAD, dataout->DATA, dataout->LEN);
    MYLOG(DEBUG, "Snci_Rf_Card_Tx");
    return nci_send;
}

SL_NCI_PACK Snci_Rf_Card_Rx(SL_APUD_TRAN *const datain) {
    SL_NCI_PACK nci_send;
    MYLOG(DEBUG, "Snci_Rf_Card_Rx");
    return nci_send;
}

static uint8_t nfc_Nibble2HexChar(uint8_t bNibble)
{
    uint8_t  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

static uint16_t nfc_U8_To_Hex(uint8_t *buf,uint16_t len,uint8_t * dest_buf,uint16_t dest_len)
{
    uint8_t low = 0;
    uint8_t high = 0;
    uint16_t i = 0;
    uint16_t j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = nfc_Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = nfc_Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

uint16_t ZDT_NFC_SIM_UID_IsOK(uint8_t * pUID,uint16_t uid_max_len)
{
    uint16_t uid_str_len = 0;
    if(g_nfc_hw_init_ok && g_nfc_uid_len > 0)
    {
        uid_str_len = nfc_U8_To_Hex(g_nfc_uid_buf,g_nfc_uid_len,pUID,uid_max_len);
    }
    return uid_str_len;
}

uint8_t ZDT_NFC_HW_IsOK(void)
{
    return g_nfc_hw_init_ok;
}

