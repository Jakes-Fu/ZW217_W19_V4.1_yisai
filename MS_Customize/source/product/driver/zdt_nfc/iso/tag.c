#include "tag.h"

void tag1_read_16block(uint8_t block) { //一次读取16块 128字节 = 14 * 8  block = 8byte
    //       11223344556677889900 11223344
    //00000E 10 + block + 0000000000000000 45637700 //一次读取16块 128字节 = 14 * 8
    SL_NCI_PACK nci_recv;
    uint8_t pbuf[14] = {0};
    pbuf[0] = 0x10;
    pbuf[1] = block;
    memcpy(pbuf + 10, NfcCardinfo[CurrentCard].uid, 4);
    Snci_Send_Data(1, Snci_data_message(pbuf, 14), NCI_ACK_TIMEOUT, &nci_recv);
    //内容暂时不解析，以后有时间在研究
}
void tag1_read_uid(void) {
    //000007 780000 45637700
    SL_NCI_PACK nci_recv;
    uint8_t pbuf[7] = {0};
    pbuf[0] = 0x78;
    memcpy(pbuf + 3, NfcCardinfo[CurrentCard].uid, 4);
    Snci_Send_Data(1, Snci_data_message(pbuf, 7), NCI_ACK_TIMEOUT, &nci_recv);
    //内容暂时不解析，以后有时间在研究
}

void tag1test(void) {
    tag1_read_16block(0x00);
//    tag1_read_16block(0x10);
//    tag1_read_16block(0x20);
//    tag1_read_16block(0x30);
    tag1_read_uid();
}

void tag2_read_4block(uint8_t block) { //一次读取4块 16字节 = 4 * 4  block = 4byte
    //000002 30 04
    SL_NCI_PACK nci_recv;
    uint8_t pbuf[2] = {0};
    pbuf[0] = 0x30;
    pbuf[1] = block;
    Snci_Send_Data(1, Snci_data_message(pbuf, 2), NCI_ACK_TIMEOUT, &nci_recv);
    //内容暂时不解析，以后有时间在研究
}

void tag2test(void) {
    //其它命令以后再研究
    tag2_read_4block(0x00);
    tag2_read_4block(0x04);
}

void tag3_read_block(uint8_t block) { //一次读取4块 16字节 = 4 * 4  block = 4byte
    //       0011 2233445566778899 0a0b0c 0d0e 0f
    //000010 1006 012E4CE15C954FBB 010B00 0180 00
    //00001E 1D07 012E4CE15C954FBB 000001 100401000D0000000000010000500073 00
    SL_NCI_PACK nci_recv;
    uint8_t pbuf[16] = {0};
    pbuf[0] = 0x10;
    pbuf[1] = 0x06;
    memcpy(pbuf + 2, NfcCardinfo[CurrentCard].uid, 8);
    pbuf[10] = 0x01;
    pbuf[11] = 0x0b;
    pbuf[12] = 0x00;
    pbuf[13] = 0x01;
    pbuf[14] = 0x80;
    pbuf[15] = block;
    Snci_Send_Data(1, Snci_data_message(pbuf, 16), NCI_ACK_TIMEOUT, &nci_recv);
}

void tag3_read_ndef(void) { //一次读取4块 16字节 = 4 * 4  block = 4byte
    //       0011 2233445566778899 0a0b0c 0d0e 0f
    //000016 1606 012E4CE15C954FBB 010900 0480 0180 0280 0380 04
    //00004E 4D07 012E4CE15C954FBB 000004 91010E55017469616E616E78696E2E636F6D51013A54 02656E5368656E7A68656E205469616E616E78696E20546563686E6F6C6F677920436F2E2C204C74642E 00
    SL_NCI_PACK nci_recv;
    uint8_t pbuf[22] = {0};
    pbuf[0] = 0x16;
    pbuf[1] = 0x06;
    memcpy(pbuf + 2, NfcCardinfo[CurrentCard].uid, 8);
    pbuf[10] = 0x01;
    pbuf[11] = 0x09;
    pbuf[12] = 0x00;
    pbuf[13] = 0x04;
    pbuf[14] = 0x80;
    pbuf[15] = 0x01;
    pbuf[16] = 0x80;
    pbuf[17] = 0x02;
    pbuf[18] = 0x80;
    pbuf[19] = 0x03;
    pbuf[20] = 0x80;
    pbuf[21] = 0x04;
    Snci_Send_Data(1, Snci_data_message(pbuf, 22), NCI_ACK_TIMEOUT, &nci_recv);
}

void tag3test(void) {
    tag3_read_block(0x00);
    tag3_read_ndef();
}

void tag4test(void) {
    //暂时不实现
}
void tag5test(void) {
}
