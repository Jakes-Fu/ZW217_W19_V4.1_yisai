#include "CardReader.h"

t_RetStatus Snci_Send_String(uint8_t mode,uint8_t* pbuf, uint16_t plen, uint32_t timeout, SL_NCI_PACK *nci_recv) { //��������
    uint8_t count = 3;
    TIMER_PIB recvdata_timeout;            //���ճ�ʱʱ��
    TIMER_PIB read_timeout;            //���ճ�ʱʱ��
    if(upgradeflag) return CMD_TIME_OUT;
    while(count--) {
        MYHEXS(INFO, pbuf, plen, ">>");
        writing_buf(pbuf, plen);               //ֱ�ӷ��� 0
        if(mode == 0) return CMD_RECE_OK;     //���ȴ���Ӧ
        TimerSet(&recvdata_timeout, timeout);  //��ʱʱ�� NCI_ACK_TIMEOUT
        TimerSet(&read_timeout, 10); 
        while(1) {
            if(Snci_Receive(nci_recv) == CMD_RECE_OK) { //��������
                if((nci_recv->MT == MT_CTL_RES_PACKET) && (pbuf[1] == nci_recv->OID)) {
                    return CMD_RECE_OK;
                } else if(nci_recv->MT == MT_DATA_PACKET) {
                    return CMD_RECE_OK;
                }
                if(nci_recv->MT == MT_CTL_NTF_PACKET) {
                }
            }
            if(TimerExpired(&recvdata_timeout) == 1) {
                MYLOG(DEBUG, "Snci_Send_String CMD_TIME_OUT");
                //return CMD_TIME_OUT;
                break;
            }
	        if(TimerExpired(&read_timeout) == 1) {
                   TimerSet(&read_timeout, 10); 
	            //ZDT_NFC_ReadTimer_IntHandler();
	        }
        }
    }
    MYLOG(DEBUG, "NCI not responding  Firmware is forced to upgrade");
    if(info_data_bak[0]) {
        upgradeflag = 3;
    } else {
        upgradeflag = 1;
    }
    return CMD_TIME_OUT;
}

t_RetStatus Snci_Send_Data(uint8_t mode, SL_NCI_PACK nci_send, uint32_t timeout, SL_NCI_PACK *nci_recv) { //��������
    uint8_t count = 3;
    TIMER_PIB recvdata_timeout;            //���ճ�ʱʱ��
    TIMER_PIB read_timeout;            //���ճ�ʱʱ��
    if(upgradeflag) return CMD_TIME_OUT;
    while(count--) {
        Snci_encode(nci_send, 0);              //ֱ�ӷ���
        if(mode == 0) return CMD_RECE_OK;     //���ȴ���Ӧ
        TimerSet(&recvdata_timeout, timeout);  //��ʱʱ�� NCI_ACK_TIMEOUT
        TimerSet(&read_timeout, 10); 
        while(1) {
            if(Snci_Receive(nci_recv) == CMD_RECE_OK) { //��������
                if((nci_recv->MT == MT_CTL_RES_PACKET) && (nci_send.OID == nci_recv->OID)) {
                    return CMD_RECE_OK;
                } else if(nci_recv->MT == MT_DATA_PACKET) {
                    return CMD_RECE_OK;
                }
                if(nci_recv->MT == MT_CTL_NTF_PACKET) {
                }
            }
            if(TimerExpired(&recvdata_timeout) == 1) {
                //return CMD_TIME_OUT;
                break;
            }
	        if(TimerExpired(&read_timeout) == 1) {
                   TimerSet(&read_timeout, 10); 
	            //ZDT_NFC_ReadTimer_IntHandler();
	        }
        }
    }
    MYLOG(DEBUG, "NCI not responding  Firmware is forced to upgrade");
    if(info_data_bak[0]) {
        upgradeflag = 3;
    } else {
        upgradeflag = 1;
    }
    return CMD_TIME_OUT;
}

t_DoStatus Nfc_Card_Scanning(SL_NCI_PACK *nci_recv) { //�ȴ�6105����
    if(Snci_Receive(nci_recv) == CMD_RECE_OK) {
        if((nci_recv->MT == MT_CTL_NTF_PACKET) && (nci_recv->GID == GID_RF_MANAGE) && (nci_recv->OID == OID_RF_INTF_ACTIVATED_NTF))  { //�ȴ�6105
            return DO_6105;  //�ɹ�
        }
        if((nci_recv->MT == MT_CTL_NTF_PACKET) && (nci_recv->GID == GID_RF_MANAGE) && (nci_recv->OID == OID_RF_DISCOVER_NTF))  {      //�ȴ�6103
            return DO_6103;  //�ɹ�
        }
        if((nci_recv->MT == MT_CTL_NTF_PACKET) && (nci_recv->GID == GID_NCI_CORE) && (nci_recv->OID == OID_CORE_GENERIC_ERROR_NTF)) {  //600701a1
            return DO_FAIL; //ʧ��
        }
    }
    return DO_TIMEOUT;      //��ʱ
}

void Nfc_Card_Discover(void) {  //���¿�ʼѰ��
    SL_NCI_PACK nci_recv;
    MYENTER(DEBUG);
    if(upgradeflag) return;
    Snci_Send_Data(1, Snci_Rf_Set_Listen_Route(), NCI_ACK_TIMEOUT, &nci_recv) ;
    Snci_Send_Data(1, Snci_Rf_Idel_Mode(), NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_Data(1, Snci_Rf_Discover(), NCI_ACK_TIMEOUT, &nci_recv);
    MYENTER(DEBUG);
}

uint8_t Nfc_Card_Search(void) {                                   //���Ѱ���Ƿ�ɹ�
    SL_NCI_PACK nci_recv;
    t_DoStatus status = Nfc_Card_Scanning(&nci_recv);             //��Ƭɨ��
    if(status == DO_6105) {
        return 1;
    } else if(status == DO_6103) {
        while(Nfc_Card_Scanning(&nci_recv) == DO_6103);          //�ȴ�ɨ�����
        return nci_recv.PAYLOAD[0];
    } else if(status == DO_FAIL) {
        Nfc_Card_Discover();                                      //��������Ѱ��
        return 0;
    } else {
        return 0;
    }
}

t_DoStatus Nfc_Card_Select(void) {
    SL_NCI_PACK nci_recv;
    MYENTER(DEBUG);
    Snci_Send_Data(1, Snci_Rf_Sleep_Mode(), NCI_ACK_TIMEOUT, &nci_recv);                           //����
    Snci_Send_Data(1, Snci_Rf_Select_Card(&NfcCardinfo[CurrentCard]), NCI_ACK_TIMEOUT, &nci_recv); //ѡ��
    MYENTER(DEBUG);
    if(Nfc_Card_Scanning(&nci_recv) == DO_6105) {
        return DO_OK;
    }
    return DO_FAIL;
}

void Nfc_SimCard_Init(void) {
    uint16_t count = 50;
    SL_NCI_PACK nci_recv;
    uint8_t Power_off_and_swipe_card[]   = "\x20\x02\x05\x01\xa0\x07\x01\x03";       //�ػ�ˢ��
    uint8_t GET_Firmware_INFO[]    = "\x20\x03\x03\x01\xb0\xfe";                    //��ȡ�̼���Ϣ
    uint8_t SNCI_SET_CONFIG[]      = "\x20\x02\x1D\x07\xA0\xED\x01\x01\xA0\x37\x01\x35\xA0\xF2\x01\x00\xA0\xD5\x01\x0A\xA0\xD8\x01\x02\xA0\xEC\x01\x01\xA0\x12\x01\x02"; //�������ò���
    uint8_t NFCEE_DISCOVER_CMD[]   = "\x22\x00\x01\x01";                             //NFCEEɨ�跢������
    uint8_t CORE_CONN_CREATE_CMD[] = "\x20\x04\x06\x03\x01\x01\x02\x01\x01";         //��ʼѰ��SIM������ʼ��ʼ��
    uint8_t HCI_DAT_PIPE[]         = "\x03\x00\x02\x81\x03";                         //�򿪹���PIPE
    uint8_t HCI_DAT_WHITE_LIST[]   = "\x03\x00\x05\x81\x01\x03\x02\xC0";             //����WHITE_LIST
    uint8_t HCI_DAT_HOST_TYPE[]    = "\x03\x00\x05\x81\x01\x06\x01\x00";             //����HOST_TYPE
    uint8_t CORE_GET_CONFIG_CMD[]  = "\x20\x03\x03\x01\xA0\xEA";                     //��ȡSIM����SESSION ID
    uint8_t Set_Listen_Route[]     = "\x21\x01\x0C\x00\x02\x00\x03\x02\xC3\x00\x01\x03\x02\xC3\x04";//����·��
    uint8_t Card_Discover[]        = "\x21\x03\x04\x01\x80\x01";                                    //������Ƶ����
    uint8_t GET_SIM_CARD_ID[]      = "\x20\x03\x03\x01\xb0\xf9";
    if(upgradeflag) return;
    MYENTER(DEBUG);
    Snci_Send_String(1, GET_Firmware_INFO, sizeof(GET_Firmware_INFO) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, Power_off_and_swipe_card, sizeof(Power_off_and_swipe_card) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, SNCI_SET_CONFIG, sizeof(SNCI_SET_CONFIG) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, NFCEE_DISCOVER_CMD, sizeof(NFCEE_DISCOVER_CMD) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, CORE_CONN_CREATE_CMD, sizeof(CORE_CONN_CREATE_CMD) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, HCI_DAT_PIPE, sizeof(HCI_DAT_PIPE) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, HCI_DAT_WHITE_LIST, sizeof(HCI_DAT_WHITE_LIST) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, HCI_DAT_HOST_TYPE, sizeof(HCI_DAT_HOST_TYPE) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    if(upgradeflag == 0)
    {
        count = 50; //��ʱ0.5S �ȴ�SIM���������
        while(count--) {
            Snci_Receive(&nci_recv);
            delay_ms(10);
        }
    }
    Snci_Send_String(1, Set_Listen_Route, sizeof(Set_Listen_Route) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, Card_Discover, sizeof(Card_Discover) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    Snci_Send_String(1, CORE_GET_CONFIG_CMD, sizeof(CORE_GET_CONFIG_CMD) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    //�ڴ˴��ж��Ƿ��ܻ�ȡSensor ID
#ifdef ZDT_PCBA_ZC100_HW_V22_SUPPORT
    GPIO_SetNFC_LDO_SIM_EN(TRUE);
#endif
    Snci_Send_String(1, GET_SIM_CARD_ID, sizeof(GET_SIM_CARD_ID) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    if(upgradeflag == 0)
    {
        count = 150; //��ʱ1.5S �ȴ�NFC˯��
        while(count--) {
            Snci_Receive(&nci_recv);
            delay_ms(10);
        }
    }
    Snci_Send_String(1, GET_SIM_CARD_ID, sizeof(GET_SIM_CARD_ID) - 1, NCI_ACK_TIMEOUT, &nci_recv); //iic����
//   //�ػ�ˢ������
//    count=150;    //��ʱ1.5S �ȴ�NFC˯��  ���뱣֤�̼�˯�߲��ܸ�λ������̼����ܶ�ʧ
//    while(count--){
//        Snci_Receive(&nci_recv);
//        delay_ms(10);
//    }
//    NFCC_Rest(0); //��λnfc
//    delay_ms(100);//��ʱ100ms ��ʹnfc�����ȶ��ػ�ˢ������
//    while(1);    //��ѭ�����൱�����ضϵ�ػ����ͻ����и�vdd_pad�ϵ磬Ȼ������ضϵ磩
}

void Nfc_Card_Init(void) {
    uint8_t Open_External_DCDC[]   = "\x20\x02\x08\x01\xB0\x06\x04\x38\x18\x01\x78";       //���ⲿdcdc��Դ����
    SL_NCI_PACK nci_recv;
    if(gtPollingParam.POLLING_ENABLE == 1) {
        Snci_Send_Data(1, Snci_Core_Reset(), NCI_ACK_TIMEOUT, &nci_recv) ;
        Snci_Send_Data(1, Snci_Core_Init(), NCI_ACK_TIMEOUT, &nci_recv) ;
        Snci_Send_Data(1, Snci_Set_Config(), NCI_ACK_TIMEOUT, &nci_recv) ;
        //Snci_Send_String(1, Open_External_DCDC, sizeof(Open_External_DCDC) - 1, NCI_ACK_TIMEOUT, &nci_recv);
        Snci_Send_Data(1, Snci_Rf_Discover_Map(), NCI_ACK_TIMEOUT, &nci_recv) ;
        
    }
    if(gtListencardParam.LISTEN_ENABLE == 1) {
        Snci_Send_Data(1, Snci_Core_Reset(), NCI_ACK_TIMEOUT, &nci_recv) ;
        Snci_Send_Data(1, Snci_Core_Init(), NCI_ACK_TIMEOUT, &nci_recv) ;
        Snci_Send_Data(1, Snci_Set_Config(), NCI_ACK_TIMEOUT, &nci_recv) ;
        //Snci_Send_String(1, Open_External_DCDC, sizeof(Open_External_DCDC) - 1, NCI_ACK_TIMEOUT, &nci_recv);
    }
}

void Nfc_Card_Handle(SL_CARD_INFO card) {
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "Nfc_Card_Handle = %02x", card.RfProtocol);
    switch(card.RfProtocol) {
        case 0x80:
            switch(((NFC_A_Poll_Mode *)(card.RfTechnology))->SEL_RES) { //mifare��  For proprietary use
                case 0x08:
                case 0x18:
                    mifaretest();
                    break;
                default: break;
            }
            break;
        case 0x00://PROTOCOL_UNDETERMINED
            break;
        case 0x01://PROTOCOL_T1T
            tag1test();
            break;
        case 0x02://PROTOCOL_T2T
            tag2test();
            break;
        case 0x03://PROTOCOL_T3T
            Snci_Send_Data(1, Snci_RF_T3T_POLLING_CMD(), NCI_ACK_TIMEOUT, &nci_recv);
            delay_ms(100);
            tag3test();
            break;
        case 0x04://PROTOCOL_ISO_DEP
            if(((NFC_A_Poll_Mode *)(card.RfTechnology))->SENS_RES == 0x0800) { //CPU��
                //CpuCardTest_FM1208_WhiteCard();
                CpuCardTest();
            }
            if(((NFC_A_Poll_Mode *)(card.RfTechnology))->SENS_RES == 0x4403) { //TAG4
                tag4test();
            }
            if(((NFC_A_Poll_Mode *)(card.RfTechnology))->SENS_RES == 0x0400) { //SL6xxx CE
                get_ndef();
            }
            break;
        case 0x05://PROTOCOL_NFC_DEP
            tag5test();
            break;
        default: break;
    }
}

void Nfc_Card_Reader(void) {
    uint8_t CardNumber;
    TIMER_PIB recvdata_timeout;            //���ճ�ʱʱ��
    SL_CARD_INFO card;
    Nfc_Card_Init();
    Nfc_Card_Discover();
    TimerSet(&recvdata_timeout, 1000);  //��ʱʱ�� NCI_ACK_TIMEOUT
    while(1) {
        CardNumber = Nfc_Card_Search(); //���Ͽ�����
        if(CardNumber) {
            if((memcmp(NfcCardinfo[0].uid, card.uid, NfcCardinfo[0].uidlen) == 0) && (card.uidlen)) {
                MYLOG(DEBUG, "�ظ�ˢ��");
            } else {
                if(CardNumber == 1) { //����Ӧ��
                    CurrentCard = 0;
                    MYLOG(DEBUG, "����ѡ���ɹ� %d", CurrentCard + 1);
                    Nfc_Card_Handle(NfcCardinfo[CurrentCard]);
                    SCI_Sleep(100);
                } else {             //���Ͽ�Ӧ��
                    MYENTER(DEBUG);
                    MYLOG(DEBUG, "���Ͽ�Ӧ�� %d", CardNumber);
                    for(CurrentCard = 0 ; CurrentCard < CardNumber ; CurrentCard++) {
                        if(Nfc_Card_Select() == DO_OK) {
                            MYENTER(DEBUG);
                            MYLOG(DEBUG, "���Ͽ�ѡ���ɹ� %d", CurrentCard + 1);
                            Nfc_Card_Handle(NfcCardinfo[CurrentCard]);
                            SCI_Sleep(100);
                        }
                    }
                }
            }
            SCI_Sleep(1000);
            //delay_ms(1000);                      //������ȴ�1s
            card = NfcCardinfo[0];
            Nfc_Card_Discover();
            TimerSet(&recvdata_timeout, 1000);  //��ʱʱ�� NCI_ACK_TIMEOUT
        }
        if(card.uidlen) {
            if(TimerExpired(&recvdata_timeout) == 1) {
                card.uidlen = 0;
            }
        }
        if(upgradeflag) {
            NFC_Firmware_Update(upgradeflag, &info_data_bak[1], 34); //�̼�����
            upgradeflag = 0;
            return;
        }

        SCI_Sleep(10);
        //�������ݴ���
    }
}

void Nfc_Card_Run(void)
{
    NFC_TRACE("Nfc_Card_Run,POLLING_ENABLE=%d,LISTEN_ENABLE=%d", gtPollingParam.POLLING_ENABLE,gtListencardParam.LISTEN_ENABLE);
    if(gtPollingParam.POLLING_ENABLE) Nfc_Card_Reader();     //����������
    if(gtListencardParam.LISTEN_ENABLE) Nfc_Card_Simulation(); //������ģ�⹦��
}
