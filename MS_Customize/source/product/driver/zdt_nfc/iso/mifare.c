#include "mifare.h"
/* ��֤��Կ
 * 1��ʹ�ù̶���Կ 000009 40(cmd) 00(sector) 10(A/Bkey) A5A4A3A2A1A0(key)  ---- 10(Akey) 90(key)
      sector  ��ʾ����
      A/Bkey  ���λΪ1ΪBkey   ���λΪ0ΪAkey
   2��ʹ��Ĭ����Կ 000003 40(cmd) 02(sector) 02(A/Bkey)                    -- 82(B key2)
    uint8_t aMfc_keys[NXP_NUMBER_OF_MFC_KEYS][NXP_MFC_KEY_SIZE] = {
        {0xFF, 0XFF, 0xFF, 0XFF, 0xFF, 0XFF},
        {0xA0, 0XA1, 0xA2, 0XA3, 0xA4, 0XA5},
        {0xD3, 0XF7, 0xD3, 0XF7, 0xD3, 0XF7},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    } ;
    sector  ��ʾ����
    A/Bkey  ���λΪ1ΪBkey   ���λΪ0ΪAkey    ����λ��ʾʹ���Ǹ�Ĭ����Կ

    3����Ӧ        000002 40(cmd) 00(state)
       state 00���ɹ�   03��ʧ��
    4��Ĭ�Ͽ����֣�FF0780h 69h  Ĭ����Կffffffffff
       ���ݿ飺000     Akey|Bkey
       ��Կ�飺100     Akey
 */
//auth_mode ʹ��Akey����Bkey  ��һ��Ĭ����Կ
//sector    ����
//
static void pcd_auth_state(uint8_t *pbuf, uint8_t sector, uint8_t auth_mode, uint8_t *pkey) {
    //000003 40 01 02                 ��һ��ģʽ
    //000009 40 01 05 ffffffffffff    �ڶ���ģʽ
    //�ɹ� 0000 02 40 00
    //ʧ�� 0000 02 40 03
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_MfcAuthReq;  //���� 40
    if(pkey == NULL) {                    //��ʾʹ��Ĭ����Կ
        pbuf[len++] = sector;             //����
        pbuf[len++] = auth_mode;          //���� 82(B key2)
        pbuf[0] = len - 1 ;               //����
    } else {
        pbuf[len++] = sector;             //����
        pbuf[len++] = auth_mode;          //���� 80(Bkey)
        memcpy(pbuf + len, pkey, 6);
        pbuf[0] = len + 6 - 1 ;           //����
    }
    MYLOG(DEBUG, "pcd_auth_state sector =%d", sector);
}

//block �ڼ���
static void pcd_read(uint8_t *pbuf, uint8_t block) {
    //��һ����000003103006
    //�ɹ� 0000 12 10(cmd) CEF310705D080400020BEA48A4C5431D 00
    //ʧ�� 0000 03 10(cmd) 04 00
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;  //���� 10
    pbuf[len++] = NFC_READ;                   //���� 30
    pbuf[len++] = block;                      //����
    pbuf[0] = len - 1 ;                        //����
    MYLOG(DEBUG, "pcd_read block =%d", block);
}

//block �ڼ���
static void pcd_write(uint8_t *pbuf, uint8_t block, uint8_t step, uint8_t *write) {
    //д����Ҫ�������
    //��һ����00000310A014
    //�ɹ� 0000 03 10 0A 14
    //ʧ�� 0000 03 10 04 14
    //�ڶ�����0000111011223344556677889900112233445566
    //�ɹ� 0000 03 10 0A 14
    //ʧ�� 0000 03 10 04 14
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;  //���� 10
    if(step == 1) {
        pbuf[len++] = NFC_WRITE;                  //���� a0
        pbuf[len++] = block;                    //����
        pbuf[0] = len - 1 ;                   //����
    }
    if(step == 2) {
        memcpy(pbuf + len, write, 16);
        pbuf[0] = len + 16 - 1 ;             //����
    }
    MYLOG(DEBUG, "pcd_write block =%d:step %d", block, step);
}

//block �ڼ��� �ۿ�
static void pcd_decrement(uint8_t *pbuf, uint8_t block, uint8_t step, int32_t value) {
    //�ۿ���Ҫ3�����
    //��һ����00000310C014
    //�ɹ� 0000 03 10 0A 14
    //�ڶ�����0000051005000000
    //�ɹ� 0000 02 10 B2
    //��������00000310B014
    //�ɹ� 0000 03 10 0A 14
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;      //���� 10
    if(step == 1) {
        pbuf[len++] = NFC_DECREMENT;              //���� C0
        pbuf[len++] = block;                      //����
    }
    if(step == 2) {
        pbuf[len++] = (uint8_t)(value);           //����
        pbuf[len++] = (uint8_t)(value >> 8);      //����
        pbuf[len++] = (uint8_t)(value >> 16);     //����
        pbuf[len++] = (uint8_t)(value >> 24);     //����
    }
    if(step == 3) {
        pbuf[len++] = NFC_TRANSFER;              //���� C0
        pbuf[len++] = block;                     //����
    }
    pbuf[0] = len - 1 ;                          //����
    MYLOG(DEBUG, "pcd_decrement block =%d:step %d", block, step);
}

//block �ڼ��� ��ֵ
static void pcd_increment(uint8_t *pbuf, uint8_t block, uint8_t step, int32_t value) {
    //�ۿ���Ҫ3�����
    //��һ����00000310C114
    //�ɹ� 0000 03 10 0A 14
    //�ڶ�����0000051002000000
    //�ɹ� 0000 02 10 B2
    //��������00000310B014
    //�ɹ� 0000 03 10 0A 14
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;       //���� 10
    if(step == 1) {
        pbuf[len++] = NFC_INCREMENT;              //���� C0
        pbuf[len++] = block;                      //����
    }
    if(step == 2) {
        pbuf[len++] = (uint8_t)(value);           //����
        pbuf[len++] = (uint8_t)(value >> 8);      //����
        pbuf[len++] = (uint8_t)(value >> 16);     //����
        pbuf[len++] = (uint8_t)(value >> 24);     //����
    }
    if(step == 3) {
        pbuf[len++] = NFC_TRANSFER;              //���� B0
        pbuf[len++] = block;                     //����
    }
    pbuf[0] = len - 1 ;                          //����
    MYLOG(DEBUG, "pcd_increment block =%d:step %d", block, step);
}

/**mifare API�ӿں���*************************************************************************************************************/
t_DoStatus mifare_read_block(uint8_t *outbuf, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "��ȡ��%d��", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //��֤��Կ
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_read(pbuf, block);                 //��ȡ��0��
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOADLEN == 0x12) {
                    memcpy(outbuf, &nci_recv.PAYLOAD[1], 16); //������ȡ��������
                    MYLOG(DEBUG, "��%d���ɹ�", block);
                    return DO_OK;
                } else {
                    MYLOG(DEBUG, "��%d��ʧ��", block);
                }
            }
        } else {
            MYLOG(DEBUG, "����%d��Կ��֤ʧ��", block / 4);
        }
    }
    return DO_FAIL;
}

t_DoStatus mifare_write_block(uint8_t *inbuf, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "д���%d��", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //��֤��Կ
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_write(pbuf, block, 1, inbuf);    //��һ��
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOAD[1] == 0x0a) { //�ɹ�
                    pcd_write(pbuf, block, 2, inbuf); //�ڶ���
                    Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv) ;
                    MYLOG(DEBUG, "��%dд�ɹ�", block);
                    return DO_OK;
                } else {
                    MYLOG(DEBUG, "��%dдʧ��", block);
                }
            }
        } else {
            MYLOG(DEBUG, "����%d��Կ��֤ʧ��", block / 4);
        }
    }
    return DO_FAIL;
}



t_DoStatus mifare_Purse_init(int32_t value, uint8_t *pkey, uint8_t block) {
    // Ǯ��ֵ(4byte) + Ǯ��ֵ��(4byte) + Ǯ��ֵ(4byte) + ��ַ(1byte) + ��ַ��(1byte) + ��ַ(1byte) + ��ַ��(1byte)
    uint8_t i = 0;
    uint8_t blockbuf[16];
    int32_t tempvalue = value;
    for(i = 0; i < 4; i++) {
        blockbuf[i] = tempvalue >> 8 * i;
    }
    tempvalue = ~value;
    for(i = 0; i < 4; i++) {
        blockbuf[4 + i] = tempvalue >> 8 * i;
    }
    tempvalue = value;
    for(i = 0; i < 4; i++) {
        blockbuf[8 + i] = tempvalue >> 8 * i;
    }
    blockbuf[12] =  block;
    blockbuf[13] = ~block;
    blockbuf[14] =  block;
    blockbuf[15] = ~block;
    MYHEXS(DEBUG, blockbuf, 16, "mifare_Purse_init");
    return mifare_write_block(blockbuf, pkey, block) ;
}

t_DoStatus mifare_Purse_balance(int32_t *balance, uint8_t *pkey, uint8_t block) {
    uint8_t blockbuf[16];
    if(mifare_read_block(blockbuf, pkey, block) == DO_OK) {
        *balance = blockbuf[0] + (blockbuf[1] << 8) + (blockbuf[2] << 12) + (blockbuf[3] << 24);
        MYLOG(DEBUG, "mifare����%d��Ǯ����ȡ�ɹ� ��� : %d", block, *balance);
        return DO_OK;
    }
    MYLOG(DEBUG, "mifare����%d��Ǯ����ȡʧ��", block);
    return DO_FAIL;
}

t_DoStatus mifare_Purse_deduction(int32_t value, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "mifare����%d��Ǯ���ۿ�", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //��֤��Կ
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_decrement(pbuf, block, 1, value); //��һ��
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOAD[1] == 0x0a) { //�ɹ�
                    pcd_decrement(pbuf, block, 2, value); //�ڶ���
                    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                        if(nci_recv.PAYLOAD[1] == 0xb2) { //�ɹ�
                            pcd_decrement(pbuf, block, 3, value); //�ڶ���
                            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                                if(nci_recv.PAYLOAD[1] == 0x0a) { //�ɹ�
                                    MYLOG(DEBUG, "mifare����%d��Ǯ���ۿ� �ɹ� %d", block, value);
                                    return DO_OK;
                                } else {
                                    MYLOG(DEBUG, "mifare����%d��Ǯ���ۿ� ʧ�� 3", block);
                                }
                            }
                        } else {
                            MYLOG(DEBUG, "mifare����%d��Ǯ���ۿ� ʧ�� 2", block);
                        }
                    }
                } else {
                    MYLOG(DEBUG, "mifare����%d��Ǯ���ۿ� ʧ�� 1", block);
                }
            }
        } else {
            MYLOG(DEBUG, "����%d��Կ��֤ʧ��", block / 4);
        }
    }
    return DO_FAIL;
}

t_DoStatus mifare_Purse_increment(int32_t value, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "mifare����%d��Ǯ����ֵ", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //��֤��Կ
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_increment(pbuf, block, 1, value); //��һ��
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOAD[1] == 0x0a) { //�ɹ�
                    pcd_increment(pbuf, block, 2, value); //�ڶ���
                    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                        if(nci_recv.PAYLOAD[1] == 0xb2) { //�ɹ�
                            pcd_increment(pbuf, block, 3, value); //�ڶ���
                            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                                if(nci_recv.PAYLOAD[1] == 0x0a) { //�ɹ�
                                    MYLOG(DEBUG, "mifare����%d��Ǯ����ֵ �ɹ� %d", block, value);
                                    return DO_OK;
                                } else {
                                    MYLOG(DEBUG, "mifare����%d��Ǯ����ֵ ʧ�� 3", block);
                                }
                            }
                        } else {
                            MYLOG(DEBUG, "mifare����%d��Ǯ����ֵ ʧ�� 2", block);
                        }
                    }
                } else {
                    MYLOG(DEBUG, "mifare����%d��Ǯ����ֵ ʧ�� 1", block);
                }
            }
        } else {
            MYLOG(DEBUG, "����%d��Կ��֤ʧ��", block / 4);
        }
    }
    return DO_FAIL;
}

void mifaretest(void) {
    SL_NCI_PACK nci_recv;
    uint8_t pbuf[64];
    uint8_t *pkey;
    int32_t value, i, block = 4;
    uint8_t key[][6]   = {
        {0xFF, 0XFF, 0xFF, 0XFF, 0xFF, 0XFF},
        {0xA0, 0XA1, 0xA2, 0XA3, 0xA4, 0XA5},
        {0xD3, 0XF7, 0xD3, 0XF7, 0xD3, 0XF7},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };
    for(i = 0; i < 4; i++) {
        pkey = key[i];
        delay_ms(5);
        pcd_auth_state(pbuf, block / 4, 0, key[i]) ; //��֤��Կ
        if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
            if(nci_recv.PAYLOAD[1] == 0) {
                MYHEXS(DEBUG, pkey, 6, "������Կ��֤�ɹ�:");
                mifare_Purse_init(200, key[i], block);       //��ʼ��Ǯ��
                delay_ms(5);
                mifare_Purse_balance(&value, key[i], block); //��ȡǮ�����
                delay_ms(5);
                mifare_Purse_deduction(10, key[i], block);   //�ۿ� 5
                delay_ms(5);
                mifare_Purse_increment(5, key[i], block);    //��ֵ 10
                SCI_Sleep(100);
                MYLOG(DEBUG, "mifaretest ���");
                break;
            }
        }
    }
}
