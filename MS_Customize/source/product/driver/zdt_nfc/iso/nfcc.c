#include "nfcc.h"
#include "MyInclude.h"

uint8_t Trans_Port = NFCC_I2C;      //����I2CͨѶ��ʽ
uint8_t upgradeflag = 0;                //����flag
SL_POLLING_PARAM    gtPollingParam;
SL_LISTENCARD_PARAM gtListencardParam;
extern uint8 g_nfc_card_simulation_init_over;
uint8 g_nfc_card_rcv_ok = 0;

void NFCC_SPI_Write_buff(uint8_t* data, uint16_t lenth) {
}

uint16_t NFCC_SPI_Read_buff(uint8_t* data) {
    return 0;
}

t_RetStatus NFCC_Init(void) {
    MYLOG(DEBUG, "NFCC_Init");
    Polling_Param_Init();     //Ĭ�ϴ򿪶���������
    Listen_Param_Init();      //Ĭ�Ϲرտ�ģ�⹦��
    //NFCC_Io_Init(Trans_Port);
    NFCC_I2C_Init();
    NFCC_Rest(0);   //��λ������������ģʽ
    delay_ms(100);  //���õ���30ms
    return NO_ERR;
}

t_RetStatus NFCC_Deinit(void) {
    Snci_Rf_Idel_Mode();
    return NO_ERR;
}

void NFCC_ReceiveData(void) {
    uint8_t  pbuf[BUFFER_BLOCK_SIZE];
    uint16_t plen = 0;
    if(GPIO_GetNFCIRQ()) {             //�ߵ�ƽ����ʾ������
        if(g_nfc_card_rcv_ok == 0)
        {
            g_nfc_card_rcv_ok = 1;
        }
        plen = reading_buf(pbuf);
        if(plen > 0) {
            put_buffer_to_list(pbuf, plen, &nci_rx_list);
        }
//      MYLOG(DEBUG,"��������");
//      MYHEX(DEBUG,gMacRecvBuf,gMacRecvLen);
    }
}

void NFCC_Set_BootMode(uint8_t mode) {
    MYLOG(DEBUG, "SetBootMode %d", mode);
    if(mode) {                               //BOOTģʽ
        GPIO_SetNFCDWL(1);//����ʹ�ܹܽţ�ʹƬ�ӽ���bootģʽ
    } else {                                 //��������ģʽ
        GPIO_SetNFCDWL(0); //����ʹ�ܹܽţ�ʹƬ�ӽ�������ģʽ
    }
}

void NFCC_Rest(uint8_t mode) {            //mode = 0 ��������ģʽ  mode = 0 bootģʽ
    GPIO_NFCIRQCtrl(FALSE);
#ifdef ZDT_PCBA_ZC100_HW_V22_SUPPORT
    if(g_nfc_card_simulation_init_over == 0)
    {
        GPIO_SetNFC_LDO_SIM_EN(FALSE);
        SCI_Sleep(1500);
    }
#endif
    GPIO_SetNFCEnable(1);    //����ʹ�ܹܽţ�ʹƬ�ӵ���
    delay_ms(100);                         //��ʱ50ms
    GPIO_SetNFCEnable(0);    //����ʹ�ܹܽţ�ʹƬ�ӵ���
    delay_ms(100);                         //��ʱ50ms
    NFCC_Set_BootMode(mode);              //����ʹ�ܹܽţ�ʹƬ�ӽ���bootģʽ
    delay_ms(50);                         //��ʱ50ms
    GPIO_SetNFCEnable(1);   //����ʹ�ܹܽţ�ʹƬ���ϵ�
    delay_ms(50);
    clean_recv_data(); //��ջ���
    GPIO_NFCIRQCtrl(TRUE);
    delay_ms(20);
#if 0 //def ZDT_PCBA_ZC100_HW_V22_SUPPORT
    GPIO_SetNFC_LDO_SIM_EN(TRUE);
#endif
}

void SetIoType(uint8_t *data) {
    MYLOG(DEBUG, "SetIoType = %s", data);
    NFCC_write_info(0x14, data, 3);
}

void NFCC_Set_TransType(uint8_t transport) {  //ģʽ�л�ֻ��bootloader��ʹ������ģʽ������
    Trans_Port = NFCC_I2C;
    NFCC_Rest(1);
    calibrate();
}

void Polling_Param_Init(void) {
    MYLOG(DEBUG, "Polling_Param_Init");
    gtPollingParam.POLLING_ENABLE              = 0; //Ĭ�ϴ򿪶���������
    gtPollingParam.PN_NFC_DEP_SPEED            = 1;
    gtPollingParam.PI_BIT_RATE                 = 0;
    gtPollingParam.RF_FIELD_INFO               = 1;
    gtPollingParam.RF_NFCEE_ACTION             = 1;
    gtPollingParam.PF_BIT_RATE                 = 1;
    gtPollingParam.TOTAL_DURATION              = 500;
    gtPollingParam.PROTOCOL_DEP_TYPE           = 0x0102;
    gtPollingParam.NFC_A_PASSIVE_POLL_MODE     = 1;
    gtPollingParam.NFC_B_PASSIVE_POLL_MODE     = 1;
    gtPollingParam.NFC_F_PASSIVE_POLL_MODE     = 1;
    gtPollingParam.NFC_15693_PASSIVE_POLL_MODE = 1;
}

void Listen_Param_Init(void) {
    MYLOG(DEBUG, "Listen_Param_Init");
    gtListencardParam.LISTEN_ENABLE            = 1; //Ĭ�Ϲرտ�ģ�⹦��
    gtListencardParam.SIM_ENABLE               = 1; //Ĭ�Ͽ���sim��ģ��
    gtListencardParam.TOTAL_DURATION           = 500;
    gtListencardParam.CON_DEVICES_LIMIT        = 4;
    gtListencardParam.SDD_VALUE                = 8;
    gtListencardParam.PLATFORM_CONFIG          = 0;
    gtListencardParam.LA_SEL_INFO              = 0x20;
    gtListencardParam.NFC_ID                   = 0x01020304;
    gtListencardParam.LB_SENSEB_INFO           = 1;
    gtListencardParam.LF_PROTOCOL_TYPE         = 2;
    gtListencardParam.LF_CON_BITR_F            = 6;
    gtListencardParam.LI_BIT_RATEC             = 0;
    gtListencardParam.LN_WT                    = 0x0e;
    gtListencardParam.RF_FIELD_INFO            = 1;
    gtListencardParam.RF_NFCEE_ACTION          = 1;
    gtListencardParam.RF_NFCDEP_OP             = 0x0e;
    gtListencardParam.LA_PASSIVE_ENABLE        = 0x01;
    gtListencardParam.LB_PASSIVE_ENABLE        = 0x00;
}

void P2P_Param_Init(void) {
    #ifdef EN_P2P_ACTIVE_FUNC
    uint8_t ln_atq[20] = {0x46, 0x66, 0x6D, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07, 0xFF, 0x03, 0x02, 0x00, 0x13, 0x04, 0x01, 0x64, 0x07, 0x01, 0x04};
    MYLOG(DEBUG, "P2P_Param_Init");
    Listen_Param_Init();
    gtListencardParam.P2P_ACTIVED_ENABLE       = 0;
    gtListencardParam.P2P_ACTIVED_LISTEN       = 1;
    gtListencardParam.P2P_ACTIVED_POLL         = 0;
    gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN = 20;
    gtPollingParam.PROTOCOL_DEP_TYPE           = 0x0302;
    gtPollingParam.PROTOCOL_NFC_TYPE           = 0x0303;
    memcpy(gtListencardParam.LN_ATR_REQ_GEN_BYTES, ln_atq, gtListencardParam.LN_ATR_REQ_GEN_BYTES_LEN);
    if(gtListencardParam.P2P_ACTIVED_LISTEN == 1) {
        gtListencardParam.LN_WT = 0x07;
    }
    #endif
}
