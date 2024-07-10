#include "mifare.h"
/* 验证密钥
 * 1、使用固定密钥 000009 40(cmd) 00(sector) 10(A/Bkey) A5A4A3A2A1A0(key)  ---- 10(Akey) 90(key)
      sector  表示扇区
      A/Bkey  最高位为1为Bkey   最高位为0为Akey
   2、使用默认密钥 000003 40(cmd) 02(sector) 02(A/Bkey)                    -- 82(B key2)
    uint8_t aMfc_keys[NXP_NUMBER_OF_MFC_KEYS][NXP_MFC_KEY_SIZE] = {
        {0xFF, 0XFF, 0xFF, 0XFF, 0xFF, 0XFF},
        {0xA0, 0XA1, 0xA2, 0XA3, 0xA4, 0XA5},
        {0xD3, 0XF7, 0xD3, 0XF7, 0xD3, 0XF7},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    } ;
    sector  表示扇区
    A/Bkey  最高位为1为Bkey   最高位为0为Akey    后四位表示使用那个默认密钥

    3、响应        000002 40(cmd) 00(state)
       state 00：成功   03：失败
    4、默认控制字：FF0780h 69h  默认密钥ffffffffff
       数据块：000     Akey|Bkey
       秘钥块：100     Akey
 */
//auth_mode 使用Akey还是Bkey  第一个默认密钥
//sector    扇区
//
static void pcd_auth_state(uint8_t *pbuf, uint8_t sector, uint8_t auth_mode, uint8_t *pkey) {
    //000003 40 01 02                 第一种模式
    //000009 40 01 05 ffffffffffff    第二种模式
    //成功 0000 02 40 00
    //失败 0000 02 40 03
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_MfcAuthReq;  //命令 40
    if(pkey == NULL) {                    //表示使用默认密钥
        pbuf[len++] = sector;             //扇区
        pbuf[len++] = auth_mode;          //命令 82(B key2)
        pbuf[0] = len - 1 ;               //长度
    } else {
        pbuf[len++] = sector;             //扇区
        pbuf[len++] = auth_mode;          //命令 80(Bkey)
        memcpy(pbuf + len, pkey, 6);
        pbuf[0] = len + 6 - 1 ;           //长度
    }
    MYLOG(DEBUG, "pcd_auth_state sector =%d", sector);
}

//block 第几块
static void pcd_read(uint8_t *pbuf, uint8_t block) {
    //第一步：000003103006
    //成功 0000 12 10(cmd) CEF310705D080400020BEA48A4C5431D 00
    //失败 0000 03 10(cmd) 04 00
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;  //命令 10
    pbuf[len++] = NFC_READ;                   //命令 30
    pbuf[len++] = block;                      //扇区
    pbuf[0] = len - 1 ;                        //长度
    MYLOG(DEBUG, "pcd_read block =%d", block);
}

//block 第几块
static void pcd_write(uint8_t *pbuf, uint8_t block, uint8_t step, uint8_t *write) {
    //写块需要两步完成
    //第一步：00000310A014
    //成功 0000 03 10 0A 14
    //失败 0000 03 10 04 14
    //第二步：0000111011223344556677889900112233445566
    //成功 0000 03 10 0A 14
    //失败 0000 03 10 04 14
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;  //命令 10
    if(step == 1) {
        pbuf[len++] = NFC_WRITE;                  //命令 a0
        pbuf[len++] = block;                    //扇区
        pbuf[0] = len - 1 ;                   //长度
    }
    if(step == 2) {
        memcpy(pbuf + len, write, 16);
        pbuf[0] = len + 16 - 1 ;             //长度
    }
    MYLOG(DEBUG, "pcd_write block =%d:step %d", block, step);
}

//block 第几块 扣款
static void pcd_decrement(uint8_t *pbuf, uint8_t block, uint8_t step, int32_t value) {
    //扣款需要3步完成
    //第一步：00000310C014
    //成功 0000 03 10 0A 14
    //第二步：0000051005000000
    //成功 0000 02 10 B2
    //第三步：00000310B014
    //成功 0000 03 10 0A 14
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;      //命令 10
    if(step == 1) {
        pbuf[len++] = NFC_DECREMENT;              //命令 C0
        pbuf[len++] = block;                      //扇区
    }
    if(step == 2) {
        pbuf[len++] = (uint8_t)(value);           //扇区
        pbuf[len++] = (uint8_t)(value >> 8);      //扇区
        pbuf[len++] = (uint8_t)(value >> 16);     //扇区
        pbuf[len++] = (uint8_t)(value >> 24);     //扇区
    }
    if(step == 3) {
        pbuf[len++] = NFC_TRANSFER;              //命令 C0
        pbuf[len++] = block;                     //扇区
    }
    pbuf[0] = len - 1 ;                          //长度
    MYLOG(DEBUG, "pcd_decrement block =%d:step %d", block, step);
}

//block 第几块 充值
static void pcd_increment(uint8_t *pbuf, uint8_t block, uint8_t step, int32_t value) {
    //扣款需要3步完成
    //第一步：00000310C114
    //成功 0000 03 10 0A 14
    //第二步：0000051002000000
    //成功 0000 02 10 B2
    //第三步：00000310B014
    //成功 0000 03 10 0A 14
    uint8_t len = 1;
    pbuf[len++] = phNciNfc_e_T1TXchgDataReq;       //命令 10
    if(step == 1) {
        pbuf[len++] = NFC_INCREMENT;              //命令 C0
        pbuf[len++] = block;                      //扇区
    }
    if(step == 2) {
        pbuf[len++] = (uint8_t)(value);           //扇区
        pbuf[len++] = (uint8_t)(value >> 8);      //扇区
        pbuf[len++] = (uint8_t)(value >> 16);     //扇区
        pbuf[len++] = (uint8_t)(value >> 24);     //扇区
    }
    if(step == 3) {
        pbuf[len++] = NFC_TRANSFER;              //命令 B0
        pbuf[len++] = block;                     //扇区
    }
    pbuf[0] = len - 1 ;                          //长度
    MYLOG(DEBUG, "pcd_increment block =%d:step %d", block, step);
}

/**mifare API接口函数*************************************************************************************************************/
t_DoStatus mifare_read_block(uint8_t *outbuf, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "读取第%d块", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //验证密钥
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_read(pbuf, block);                 //读取第0块
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOADLEN == 0x12) {
                    memcpy(outbuf, &nci_recv.PAYLOAD[1], 16); //拷贝读取到的扇区
                    MYLOG(DEBUG, "块%d读成功", block);
                    return DO_OK;
                } else {
                    MYLOG(DEBUG, "块%d读失败", block);
                }
            }
        } else {
            MYLOG(DEBUG, "扇区%d密钥验证失败", block / 4);
        }
    }
    return DO_FAIL;
}

t_DoStatus mifare_write_block(uint8_t *inbuf, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "写入第%d块", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //验证密钥
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_write(pbuf, block, 1, inbuf);    //第一步
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOAD[1] == 0x0a) { //成功
                    pcd_write(pbuf, block, 2, inbuf); //第二步
                    Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv) ;
                    MYLOG(DEBUG, "块%d写成功", block);
                    return DO_OK;
                } else {
                    MYLOG(DEBUG, "块%d写失败", block);
                }
            }
        } else {
            MYLOG(DEBUG, "扇区%d密钥验证失败", block / 4);
        }
    }
    return DO_FAIL;
}



t_DoStatus mifare_Purse_init(int32_t value, uint8_t *pkey, uint8_t block) {
    // 钱包值(4byte) + 钱包值反(4byte) + 钱包值(4byte) + 地址(1byte) + 地址反(1byte) + 地址(1byte) + 地址反(1byte)
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
        MYLOG(DEBUG, "mifare卡第%d块钱包读取成功 余额 : %d", block, *balance);
        return DO_OK;
    }
    MYLOG(DEBUG, "mifare卡第%d块钱包读取失败", block);
    return DO_FAIL;
}

t_DoStatus mifare_Purse_deduction(int32_t value, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "mifare卡第%d块钱包扣款", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //验证密钥
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_decrement(pbuf, block, 1, value); //第一步
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOAD[1] == 0x0a) { //成功
                    pcd_decrement(pbuf, block, 2, value); //第二步
                    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                        if(nci_recv.PAYLOAD[1] == 0xb2) { //成功
                            pcd_decrement(pbuf, block, 3, value); //第二步
                            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                                if(nci_recv.PAYLOAD[1] == 0x0a) { //成功
                                    MYLOG(DEBUG, "mifare卡第%d块钱包扣款 成功 %d", block, value);
                                    return DO_OK;
                                } else {
                                    MYLOG(DEBUG, "mifare卡第%d块钱包扣款 失败 3", block);
                                }
                            }
                        } else {
                            MYLOG(DEBUG, "mifare卡第%d块钱包扣款 失败 2", block);
                        }
                    }
                } else {
                    MYLOG(DEBUG, "mifare卡第%d块钱包扣款 失败 1", block);
                }
            }
        } else {
            MYLOG(DEBUG, "扇区%d密钥验证失败", block / 4);
        }
    }
    return DO_FAIL;
}

t_DoStatus mifare_Purse_increment(int32_t value, uint8_t *pkey, uint8_t block) {
    uint8_t pbuf[64];
    SL_NCI_PACK nci_recv;
    MYLOG(DEBUG, "mifare卡第%d块钱包充值", block);
    pcd_auth_state(pbuf, block / 4, 0, pkey) ; //验证密钥
    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
        if(nci_recv.PAYLOAD[1] == 0) {
            pcd_increment(pbuf, block, 1, value); //第一步
            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                if(nci_recv.PAYLOAD[1] == 0x0a) { //成功
                    pcd_increment(pbuf, block, 2, value); //第二步
                    if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                        if(nci_recv.PAYLOAD[1] == 0xb2) { //成功
                            pcd_increment(pbuf, block, 3, value); //第二步
                            if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
                                if(nci_recv.PAYLOAD[1] == 0x0a) { //成功
                                    MYLOG(DEBUG, "mifare卡第%d块钱包充值 成功 %d", block, value);
                                    return DO_OK;
                                } else {
                                    MYLOG(DEBUG, "mifare卡第%d块钱包充值 失败 3", block);
                                }
                            }
                        } else {
                            MYLOG(DEBUG, "mifare卡第%d块钱包充值 失败 2", block);
                        }
                    }
                } else {
                    MYLOG(DEBUG, "mifare卡第%d块钱包充值 失败 1", block);
                }
            }
        } else {
            MYLOG(DEBUG, "扇区%d密钥验证失败", block / 4);
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
        pcd_auth_state(pbuf, block / 4, 0, key[i]) ; //验证密钥
        if(Snci_Send_Data(1, Snci_data_message(pbuf + 1, pbuf[0]), NCI_ACK_TIMEOUT, &nci_recv)  == CMD_RECE_OK) {
            if(nci_recv.PAYLOAD[1] == 0) {
                MYHEXS(DEBUG, pkey, 6, "扇区密钥验证成功:");
                mifare_Purse_init(200, key[i], block);       //初始化钱包
                delay_ms(5);
                mifare_Purse_balance(&value, key[i], block); //获取钱包余额
                delay_ms(5);
                mifare_Purse_deduction(10, key[i], block);   //扣款 5
                delay_ms(5);
                mifare_Purse_increment(5, key[i], block);    //充值 10
                SCI_Sleep(100);
                MYLOG(DEBUG, "mifaretest 完成");
                break;
            }
        }
    }
}
