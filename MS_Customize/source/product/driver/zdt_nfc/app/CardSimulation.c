#include "CardSimulation.h"

uint8 g_nfc_card_simulation_init_over = 0;
uint8 g_nfc_card_simulation_init_error = 0;
uint8 g_nfc_card_simulation_reinit_times = 0;
extern uint8 g_nfc_card_rcv_ok;

void Snci_handle_Apdu(SL_NCI_PACK *nci_recv) {
    static uint16_t aid = 0;
    APDU_SEND apdu;
    if(nci_recv->MT == MT_DATA_PACKET) { //APDU�����
        if(Snci_decode_Apdu(&apdu, nci_recv) == DO_OK) {
            if((apdu.CLA == 0x00) && (apdu.INS == 0xA4)) { //ѡ�ļ�
                //00000d 00a40400 07 d276000085010100     ��Ӧ��000002 9000
                //000007 00a4000c 02 e103                 ��Ӧ��000002 9000
                //000007 00A4000C 02 e104                 ��Ӧ��000002 9000
                if(apdu.data[0] == 7) {
                    MYHEXS(DEBUG, apdu.data, 7, "��������");
                    if(memcmp(apdu.data + 1, "\xd2\x76\x00\x00\x85\x01\x01\x00", 7) == 0) {
                        Snci_Send_Data(0, Snci_encode_Apdu_Res("\x90\x00", 2), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                        aid = 0;
                        return;
                    }
                }
                if(apdu.data[0] == 2) {
                    if(((apdu.data[1] == 0xe1) && (apdu.data[2] == 0x03)) || ((apdu.data[1] == 0xe1) && (apdu.data[2] == 0x04))) {
                        aid = (apdu.data[1] << 8) + apdu.data[2];
                        Snci_Send_Data(0, Snci_encode_Apdu_Res("\x90\x00", 2), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                        return;
                    }
                }
                aid = 0;
            }
            if((apdu.CLA == 0x00) && (apdu.INS == 0xb0)) { //���������ļ�
                //����ǰ�ļ�ǰ15���ֽ�
                //000005 00B0000F 00            <e103�ļ�>��Ӧ��000011 000F2000FF00FF0406E10400FF00FF 9000 //NDEF access CC
                //000005 00b00000 0f            <e103�ļ�>��Ӧ��000011 000F2000FF00FF0406E10400FF00FF 9000 //NDEF access CC
                //000005 00B00000 02            <e104�ļ�>��Ӧ��000004 000E 9000
                //000005 00B00002 0E            <e104�ļ�>��Ӧ��000010     D1010A550162616964752E636F6D 9000 //U .badu.com  NDEF message
                //000005 00B00000 10            <e104�ļ�>��Ӧ��000012 000ED1010A550162616964752E636F6D 9000 //contents
                //000005 00B00010 00            <e104�ļ�>��Ӧ��000002 9000
                if(aid == 0xe103) {
                    Snci_Send_Data(0, Snci_encode_Apdu_Res("\x00\x0F\x20\x00\xFF\x00\xFF\x04\x06\xE1\x04\x00\xFF\x00\xFF\x90\x00", 0x11), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                    return;
                }
                if(aid == 0xe104) {
                    if((apdu.P2 == 0x00) && (apdu.data[0] == 0x02)) {
                        Snci_Send_Data(0, Snci_encode_Apdu_Res("\x00\x0E\x90\x00", 0x04), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                    }
                    if((apdu.P2 == 0x02) && (apdu.data[0] == 0x0e)) {
                        Snci_Send_Data(0, Snci_encode_Apdu_Res("\xD1\x01\x0A\x55\x01\x62\x61\x69\x64\x75\x2E\x63\x6F\x6D\x90\x00", 0x10), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                    }
                    if((apdu.P2 == 0x00) && (apdu.data[0] == 0x10)) {
                        Snci_Send_Data(0, Snci_encode_Apdu_Res("\x00\x0E\xD1\x01\x0A\x55\x01\x62\x61\x69\x64\x75\x2E\x63\x6F\x6D\x90\x00", 0x12), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                    }
                    if((apdu.P2 == 0x10) && (apdu.data[0] == 0x00)) {
                        Snci_Send_Data(0, Snci_encode_Apdu_Res("\x00\x00\x02\x90\x00", 0x04), NCI_ACK_TIMEOUT, NULL); //�ɹ���Ӧ
                    }
                    return;
                }
            }
            //000009 905A0000 0300000000              ��Ӧ��000002 6A82
            //000004 5A000000                         ��Ӧ��000002 6A82
            //000005 90600000 00                      ��Ӧ��000002 6A82
            //000016 00A40400 10A00000039656434103F0154020D4000D00 ��Ӧ��000002 6A82
            //00000C 00A40400 07D2760000850100        ��Ӧ��000002 6A82
            //000008 00A40200 022F0000                ��Ӧ��000002 6A82
            //000007 00A4020C 022F00                  ��Ӧ��000002 6A82
            //000007 00A40204 022F00                  ��Ӧ��000002 6A82
            //000008 00A40204 022F0000                ��Ӧ��000002 6A82
            //000007 00A4000C 022F00                  ��Ӧ��000002 6A82
            //000007 00A40000 022F00                  ��Ӧ��000002 6A82
            //000008 00A40000 022F0000                ��Ӧ��000002 6A82
            //00000A 00B12F00 045402000000            ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000003000000      ��Ӧ��000002 6A82
            //00000E 00A40400 08A00000000300000000    ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000151000000      ��Ӧ��000002 6A82
            //00000E 00A40400 08A000000018434D0000    ��Ӧ��000002 6A82
            //00000E 00A40400 08A00000000400000000    ��Ӧ��000002 6A82
            //000005 80CA9F7F 00                      ��Ӧ��000002 6A82
            //000005 80CADF50 17                      ��Ӧ��000002 6A82
            //000005 80CADF52 17                      ��Ӧ��000002 6A82
            //000006 80000000 007F                    ��Ӧ��000002 6A82
            //000014 00A40400 0E325041592E5359532E444446303100 ��Ӧ��000002 6A82
            //00000F 00A40400 09D2760000220000000200  ��Ӧ��000002 6A82
            //00000C 00A40400 06A0000000250100        ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000004101000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000004306000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000003101000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000003201000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000003301000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000003801000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000003241000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000065101000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000152301000      ��Ӧ��000002 6A82
            //000010 00A40400 0AA0000000023060D1580000��Ӧ��000002 6A82
            //00000D 00A40400 07A000000476201000      ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000042101000      ��Ӧ��000002 6A82
            //00000E 00A40400 08D04000000100000200    ��Ӧ��000002 6A82
            //000014 00A40400 0E325041592E5359532E444446303100 ��Ӧ��000002 6A82
            //000014 00A40400 0E315041592E5359532E444446303100 ��Ӧ��000002 6A82
            //00000B 00A404C0 06A00000006900          ��Ӧ��000002 6A82
            //00000C 00A40400 07D4100000030001        ��Ӧ��000002 6A82
            //00000C 00A40400 07D4100000140001        ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000476303000      ��Ӧ��000002 6A82
            //000011 00A40400 0BA0000003964D66344D000200 ��Ӧ��000002 6A82
            //00000C 00A4040C 07A0000002471001        ��Ӧ��000002 6A82
            //00000E 00A4040C 09D27600002545500200    ��Ӧ��000002 6A82
            //00000F 00A4040C 0AA000000167455349474E  ��Ӧ��000002 6A82
            //00000B 00A4040C 06D27600006601          ��Ӧ��000002 6A82
            //000012 00A40400 0CD2760001354B414E4D30310000 ��Ӧ��000002 6A82
            //000011 00A40400 0BA0000002164954534F2D3100 ��Ӧ��000002 6A82
            //000011 00A40400 0BA000000291D0560001900100 ��Ӧ��000002 6A82
            //00000E 00A40400 08A00000052720010100    ��Ӧ��000002 6A82
            //00000D 00A40400 07A000000527100200      ��Ӧ��000002 6A82
            Snci_Send_Data(0, Snci_encode_Apdu_Res("\x6A\x82", 2), NCI_ACK_TIMEOUT, NULL);
        } else {
            //000002E080                              ��Ӧ��000006 057880710000  ATS
            //000001C2                                ��Ӧ��000002 C200          SAK
            if((nci_recv->PAYLOAD[0] == 0xe0) && (nci_recv->PAYLOAD[1] == 0x80)) {
                Snci_Send_Data(0, Snci_encode_Apdu_Res("\x05\x78\x80\x71\x00\x00", 6), NCI_ACK_TIMEOUT, NULL);
            }
            if(nci_recv->PAYLOAD[0] == 0xc2) {
                Snci_Send_Data(0, Snci_encode_Apdu_Res("\xc2\x00", 2), NCI_ACK_TIMEOUT, NULL);
            }
        }
    }
}

//demo������Ƶ����0xf0, 0x44   C1��D4Ч����    0xf0, 0x24  B2��E0Ч����
uint8_t B003[] = {0xb0, 0x03, 0x04, 0x83, 0x86, 0x89, 0x8d};
uint8_t B006[] = {0xb0, 0x06, 0x10, 0x12, 0x12};
uint8_t B007[] = {0xb0, 0x07, 0x14, 0xB4, 0xf0, 0x24, 0x00, 0x00, 0xB4, 0xf0, 0x24, 0x00, 0x00, 0xB4, 0xf0, 0x24, 0x00, 0x00, 0xc3, 0xf0, 0x44, 0x00, 0x00};
uint8_t B008[] = {0xb0, 0x08, 0x14, 0xD2, 0xf0, 0x24, 0x00, 0x00, 0xD2, 0xf0, 0x24, 0x00, 0x00, 0xD2, 0xf0, 0x24, 0x00, 0x00, 0xd3, 0xf0, 0x44, 0x00, 0x00};
uint8_t B009[] = {0xb0, 0x09, 0x14, 0xD7, 0xf0, 0x24, 0x00, 0x00, 0xD7, 0xf0, 0x24, 0x00, 0x00, 0xD7, 0xf0, 0x24, 0x00, 0x00, 0xd0, 0xf0, 0x44, 0x00, 0x00};
uint8_t g_alm_automatic[] = { //69
    //0x85, 0x00, 0x00, 0x00, 0xC0,
    0X80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0X90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0XA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0XB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0XC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
    0XD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0XE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0XF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
};
void set_alm(SL_NCI_PACK *nci_recv) {
    Snci_Send_Data(1, Snci_Set_PrivateConfig(B003, sizeof(B003)), NCI_ACK_TIMEOUT, nci_recv) ;
    Snci_Send_Data(1, Snci_Set_PrivateConfig(B006, sizeof(B006)), NCI_ACK_TIMEOUT, nci_recv) ;
    Snci_Send_Data(1, Snci_Set_PrivateConfig(B008, sizeof(B008)), NCI_ACK_TIMEOUT, nci_recv) ;
    Snci_Send_Data(1, Snci_Set_PrivateConfig(B009, sizeof(B009)), NCI_ACK_TIMEOUT, nci_recv) ;
    Snci_Send_Data(1, Snci_Set_PrivateConfig(B007, sizeof(B007)), NCI_ACK_TIMEOUT, nci_recv) ;
}

void scan_alm(SL_NCI_PACK *nci_recv) {
    static uint8_t i = 0;
    //8d��λ
//    B007[18] = g_alm_automatic[i];
//    B008[18] = B007[18];
//    B009[18] = B007[18];
//    MYLOG(DEBUG, "g_alm_automatic[%02d] = %02x", i, B007[18]);
    //89��λ
    B007[13] = g_alm_automatic[i];
    B008[13] = B007[13];
    B009[13] = B007[13];
    set_alm(nci_recv);
    MYLOG(DEBUG, "g_alm_automatic[%02d] = %02x", i, B007[13]);
    if(++i > 63) i = 0;
}

void Nfc_Card_Simulation(void) {
    uint8_t alm_scan = 0;                  //�Զ�ɨ���������  1���� 0�ر�  ƽ��ʹ������0  ������Ƶ����ʱ��ʹ�ã����2523ʹ�ã�
    SL_NCI_PACK nci_recv;
    TIMER_PIB recvdata_timeout;            //���ճ�ʱʱ��
    TIMER_PIB scan_timeout;                //���ճ�ʱʱ��
    Nfc_Card_Init();
    if(gtListencardParam.SIM_ENABLE) {
        MYLOG(DEBUG, "SIM����ģ�⣬��ʼ��");
        Nfc_SimCard_Init();
    } else {
        MYLOG(DEBUG, "��������ģ�⣬��ʼ��");
        Nfc_Card_Discover();
    }
    TimerSet(&scan_timeout, NCI_ACK_TIMEOUT * 4);
    while(1) {
        if((TimerExpired(&scan_timeout) == 1) && alm_scan) {
            scan_alm(&nci_recv);//�Զ�ɨ���������
            TimerSet(&scan_timeout, NCI_ACK_TIMEOUT * 4);
        }
        if(Nfc_Card_Scanning(&nci_recv) == DO_6105) {            //��Ƭɨ��
            TimerSet(&recvdata_timeout, NCI_ACK_TIMEOUT);         //��ʱʱ�� NCI_ACK_TIMEOUT
            while(1) {
                if(Snci_Receive(&nci_recv) == CMD_RECE_OK) {
                    Snci_handle_Apdu(&nci_recv);
                    TimerSet(&recvdata_timeout, NCI_ACK_TIMEOUT); //��ʱʱ�� NCI_ACK_TIMEOUT
                }
                if(TimerExpired(&recvdata_timeout) == 1) {       //��ʱNCI_ACK_TIMEOUT �˳���Ƭ���� �������´�ɨ��
                    //MYLOG(DEBUG, "��ģ��ģʽ ���ڵȴ�������ɨ��......");
                    break;
                }
            }
        }
        if(upgradeflag) {
            g_nfc_card_simulation_reinit_times++;
            upgradeflag = 0;
            if(g_nfc_card_simulation_reinit_times >= 2 && g_nfc_card_rcv_ok == 0)
            {
                g_nfc_card_simulation_reinit_times = 0;
                g_nfc_card_simulation_init_error = 1;
                if(g_nfc_card_simulation_init_over == 0)
                {
                    g_nfc_card_simulation_init_over = 1;
                    NFC_SendTo_APP_InitOver();
                }
            }
            else
            {
                NFC_Firmware_Update(upgradeflag, &info_data_bak[1], 34); //�̼����� info_data_bak��Ҫ���б���
            }
            return;
        }
        SCI_Sleep(10);
        if(g_nfc_card_simulation_init_over == 0)
        {
            g_nfc_card_simulation_init_over = 1;
            NFC_SendTo_APP_InitOver();
        }
        if(ZDT_NFC_EnterSleepExt() > 0)
        {
            return;
        }
    }
}
