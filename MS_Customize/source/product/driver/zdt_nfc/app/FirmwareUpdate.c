#include "FirmwareUpdate.h"
uint32_t gtrecvlen = RSP_LEN;

const unsigned short xmodemCrcTable[256] = {
    0x0,    0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x210,  0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x420,  0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x630,  0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x840,  0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0xa50,  0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0xc60,  0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0xe70,  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0xa1,   0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x2b1,  0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x481,  0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x691,  0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x8e1,  0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0xaf1,  0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0xcc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0xed1,  0x1ef0
};

uint16_t calc_crc(uint8_t * dat, uint16_t len) {
    uint16_t ch;
    uint8_t j;
    uint16_t i;
    uint16_t crc;
    crc = 0;
    for(i = 0; i < len; i++) {
        ch = dat[i];
        j = (crc >> 8) ^ ch;
        crc = (crc << 8) ^ xmodemCrcTable[j];
    }
    return crc;
}

uint16_t recv_data(uint8_t * data) {
    struct buf_entry *pbuf;
    TIMER_PIB  recvdata_timeout;
    TimerSet(&recvdata_timeout, NCI_ACK_TIMEOUT);
    while(1) {
        pbuf = get_buffer_from_list(&nci_rx_list);//从list中获取数据
        if(pbuf != NULL) {
            uint16_t len = pbuf->len;
            memcpy(data, pbuf->block, len);
            free_a_buf(&pbuf);
            //MYHEXS(DEBUG, data, len, "recv_data:");
            return len;;
        }
        if(TimerExpired(&recvdata_timeout) == 1) {
            return 0;
        }
    }
}

uint16_t clean_recv_data(void) {
    struct buf_entry *pbuf;
    while(1) 
    {
        pbuf = get_buffer_from_list(&nci_rx_list);//从list中获取数据
        if(pbuf != NULL) {
            free_a_buf(&pbuf);
        }
        else
        {
            return 0;
        }
    }
}

void dump_memory(int handle, unsigned int addr, unsigned short len) {
    unsigned char command[11] = {'S', 'L', 'I', 'N', 'K', CMD_DUMP_MEMORY, 0x1, 0x0, 0x0, 0x1, 0x0};
    unsigned char Answer[1040];
    int offset = 6;
//    unsigned short crc, crc_recvd;
    if(len > FRAME_LEN) {
        len = FRAME_LEN;
    }
    command[offset++] = addr >> 16 & 0xFF;
    command[offset++] = addr >> 8 & 0xFF;
    command[offset++] = addr & 0xFF;
    command[offset++] = len >> 8;
    command[offset++] = len;
    gtrecvlen = RSP_LEN;
    writing_buf(command, sizeof(command));
    recv_data(Answer);//RSP_LEN
    if(memcmp(command, Answer, RSP_LEN - 1) || Answer[RSP_LEN - 1]) {
        return;
    }
    recv_data(Answer);
//    crc = calc_crc(Answer, len);
//    crc_recvd = Answer[len] << 8 | Answer[len + 1];
}

uint8_t write_flash(uint32_t addr, uint8_t * hexstring, uint8_t erase, uint16_t len) {
    unsigned short crc;
    unsigned char command[1040] = {'S', 'L', 'I', 'N', 'K', CMD_WRITE_WITH_ERASE, 0x1, 0x0, 0x0, 0x1, 0x0};//07  11
    int offset = 6;
    unsigned char Answer[256];
    if(erase) {
        command[5] = CMD_WRITE_WITH_ERASE;
    } else {
        command[5] = CMD_WRITE_FLASH;
    }
    if(addr >= 0x20800) {
        return 0;
    }
    command[offset++] = addr >> 16 & 0xFF;
    command[offset++] = addr >> 8 & 0xFF;
    command[offset++] = addr & 0xFF;
    command[offset++] = len  >> 8;
    command[offset++] = len;
    memcpy(&command[offset], hexstring, len);
    crc = calc_crc(&command[offset], len);
    offset += len;
    command[offset++] = crc >> 8;
    command[offset++] = crc;
    gtrecvlen = RSP_LEN;
//    if(writing_buf(command, offset) != 0) {
//        return 1;
//    }
    writing_buf(command, offset);
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1) || Answer[RSP_LEN - 1]) {
        return 1;
    }
    return 0;
}

uint8_t read_reg(uint32_t addr) {
    unsigned char Answer[RSP_LEN];
    unsigned char command[11] = {'S', 'L', 'I', 'N', 'K', CMD_READ_REG, };//0c
    int offset = 6;
    command[offset++] = (uint8_t)(addr >> 24);
    command[offset++] = (uint8_t)(addr >> 16);
    command[offset++] = (uint8_t)(addr >> 8);
    command[offset++] = (uint8_t)(addr);
    gtrecvlen = RSP_LEN;
    writing_buf(command, offset);
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1 - 1) || Answer[RSP_LEN - 1]) {
        return 1;
    }
    return Answer[RSP_LEN - 2];
}

void write_reg(uint32_t addr, uint8_t val) {
    uint8_t Answer[RSP_LEN];
    uint8_t command[11] = {'S', 'L', 'I', 'N', 'K', CMD_WRITE_REG};
    uint8_t offset = 6;
    command[offset++] = addr >> 24 & 0xFF;
    command[offset++] = addr >> 16 & 0xFF;
    command[offset++] = addr >> 8 & 0xFF;
    command[offset++] = addr & 0xFF;
    command[offset++] = val;
    gtrecvlen = RSP_LEN;
    writing_buf(command, offset);
    recv_data(Answer);
//    if (memcmp(command, Answer, RSP_LEN-1)||Answer[RSP_LEN-1])
//    {
//        return 1;
//    }
//    return 0;
}

uint8_t write_infoxx(uint32_t addr, uint8_t * hexstring, uint8_t len) {
    if(addr >= 0x800) {
        return 1;
    }
    return write_flash(addr + 0x20000, hexstring, 1, len);
}

uint8_t write_info(uint32_t addr, uint8_t * hexstring, uint8_t len) {
    uint16_t crc;
    uint8_t Answer[RSP_LEN];
    uint8_t command[256] = {'S', 'L', 'I', 'N', 'K', CMD_WRITE_INFO, 0x0, 0x1, 0x0};
    uint8_t offset = 6;
    command[offset++] = addr;
    command[offset++] = len;
    gtrecvlen = RSP_LEN;
    //MYHEXS(DEBUG, command, offset, "writing_buf");
    if(writing_buf(command, offset) != 0) {
        return 1;
    }
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1 - 4) || Answer[RSP_LEN - 1]) {
        NFC_TRACE("write_info ERRO 1\r\n");
        return 1;
    }
    memcpy(&command[offset], hexstring, len);
    crc = calc_crc(&command[offset], len);
    offset += len;
    command[offset++] = crc >> 8;
    command[offset++] = crc;
    gtrecvlen = RSP_LEN;
    //MYHEXS(DEBUG, (command + offset - len - 2), len + 2, "writing_buf");
    writing_buf((command + offset - len - 2), len + 2);
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1 - 4) || Answer[RSP_LEN - 1]) {
        NFC_TRACE("write_info ERRO 2\r\n");
        return 1;
    }
    return 0;
}

uint8_t erase_chip(unsigned char val) {
    unsigned char command[RSP_LEN] = {'S', 'L', 'I', 'N', 'K', CMD_ERASE_FLASH};
    unsigned char Answer[256];
    int offset = 6;
    command[offset++] = val;
    gtrecvlen = RSP_LEN;
    writing_buf(command, offset);
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1 - 5) || Answer[RSP_LEN - 1]) {
        return 1;
    }
    return 0;
}

void config_flash_read(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
    unsigned char command[RSP_LEN] = {'S', 'L', 'I', 'N', 'K', CMD_CONFIG_FLASH_READ,};
    unsigned char Answer[RSP_LEN];
    int offset = 6;
    command[offset++] = b0;
    command[offset++] = b1;
    command[offset++] = b2;
    command[offset++] = b3;
    gtrecvlen = RSP_LEN;
    writing_buf(command, offset);
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1 - 1) || Answer[RSP_LEN - 1]) {
        return;
    }
}

t_RetStatus download_firmware_start(unsigned int addr, uint32_t filesize) {
    unsigned char Answer[256] = {0};
    unsigned char command[11] = {'S', 'L', 'I', 'N', 'K'};
    command[5] = CMD_DOWNLOAD;
    command[6] = addr >> 16 & 0xFF;
    command[7] = addr >> 8 & 0xFF;
    command[8] = addr & 0xFF;
    #ifdef BACK_DOOR
    command[9] = 0;
    command[10] = 0;
    #else
    command[9] = filesize >> 8;
    command[10] = filesize;
    #endif
    gtrecvlen = RSP_LEN;
    writing_buf(command, 11);
    recv_data(Answer);//len = RSP_LEN
    if(memcmp(command, Answer, RSP_LEN - 1) || Answer[RSP_LEN - 1]) {
        return DOWN_START_ERROR;
    } else {
        return NO_ERR;
    }
}

t_RetStatus download_firmware(uint8_t * firmware, uint8_t cur_seq, uint32_t len) {
    unsigned short crc = 0;
    unsigned char Answer[256] = {0};
    unsigned char load_new, retry;
    uint8_t rxlen = 0;
    load_new = 1;
    retry = 0;
    rxlen = 0;
    while(1) {
        if(load_new) {
            firmware[0] = SOH;
            firmware[1] = cur_seq;
            firmware[2] = 0xFF - cur_seq;
            #if 0
            crc = calc_crc(firmware + 3, FRAME_LEN);
            firmware[FRAME_LEN + 3] = crc >> 8;
            firmware[FRAME_LEN + 4] = crc & 0xFF;
            #else
            crc = calc_crc(firmware + 3, len);
            firmware[len + 3] = crc >> 8;
            firmware[len + 4] = crc & 0xFF;
            #endif
        }
        gtrecvlen = 3;
        writing_buf(firmware, len + 5);
        rxlen = recv_data(Answer);//len = 3
        if((Answer[0] != 0) || (rxlen == 0)) {
            load_new = 0;
            if(++retry == 3)
                break;
        } else {
            load_new = 1;
            break;
        }
    }
    if(retry == 3) {
        return DOWN_DATA_ERROR;
    } else {
        return NO_ERR;
    }
}

t_RetStatus download_firmware_end(uint8_t * firmware, uint8_t cur_seq, uint32_t len) {
    unsigned short crc = 0;
    unsigned char Answer[256] = {0};
    unsigned char load_new, retry;
    load_new = 1;
    retry = 0;
    while(1) {
        if(load_new) {
            firmware[0] = EOT;
            firmware[1] = cur_seq;
            firmware[2] = 0xFF - cur_seq;
            #if 0
            crc = calc_crc(firmware + 3, FRAME_LEN);
            firmware[FRAME_LEN + 3] = crc >> 8;
            firmware[FRAME_LEN + 4] = crc & 0xFF;
            #else
            crc = calc_crc(firmware + 3, len);
            firmware[len + 3] = crc >> 8;
            firmware[len + 4] = crc & 0xFF;
            #endif
        }
        gtrecvlen = 3;
        writing_buf(firmware, len + 5);
        recv_data(Answer);//len = 3
        if(Answer[0] != 0) {
            load_new = 0;
            if(++retry == 3)
                break;
        } else {
            load_new = 1;
            break;
        }
    }
    if(retry == 3) {
        return DOWN_STOP_ERROR;
    } else {
        return NO_ERR;
    }
}

uint8_t NFCC_write_info(uint32_t addr, uint8_t * hexstring, uint16_t len) {
    return write_flash(addr + 0x20000, hexstring, 1, 3);
}

unsigned char read_reg_get_val(unsigned int addr) {
    unsigned char Answer[RSP_LEN];
    unsigned char command[11] = {'S', 'L', 'I', 'N', 'K', CMD_READ_REG, };
    int offset = 6;
    command[offset++] = addr >> 24 & 0xFF;
    command[offset++] = addr >> 16 & 0xFF;
    command[offset++] = addr >> 8 & 0xFF;
    command[offset++] = addr & 0xFF;
    gtrecvlen = RSP_LEN;
    writing_buf(command, offset);
    recv_data(Answer);
    //PRINT_BUF("recv rsp:", Answer, RSP_LEN);
    //NFC_TRACE("reg=%X\r\n", Answer[RSP_LEN - 2]);
    if(memcmp(command, Answer, RSP_LEN - 1 - 1) || Answer[RSP_LEN - 1]) {
        //MYHEXS(DEBUG, Answer, RSP_LEN,"error rsp:");
        return 0;
    } else
        return Answer[RSP_LEN - 2];
}

void calibrate(void) {
    unsigned char Answer[256];
    unsigned char command[6] = {'S', 'L', 'I', 'N', 'K'};
    unsigned char cal_res[2];
    unsigned char flag = 0xA8;
    gtrecvlen = RSP_LEN;
    cal_res[0] = read_reg_get_val(0x3201A);//第一条命令不回 所以需要发两次
    //cal_res[0] = read_reg_get_val(0x3201A);
    if(0xA8 == cal_res[0]) {
        MYLOG(DEBUG, "no calibrate");
    } else {
        MYLOG(DEBUG, "calibrate");
        write_reg(0x3986d, 1);
        command[5] = CMD_CALIBRATE;
        gtrecvlen = RSP_LEN;
        writing_buf(command, sizeof(command));
        recv_data(Answer);
        if(memcmp(command, Answer, RSP_LEN - 1 - 5) || Answer[RSP_LEN - 1]) {
            MYHEXS(DEBUG, Answer, RSP_LEN - 5, "error rsp:");
            return;
        }
        write_info(0x1a, &flag, 1);
    }
    cal_res[0] = read_reg_get_val(0x32018);
    cal_res[1] = read_reg_get_val(0x32019);
    MYLOG(DEBUG, "calibrate : %02x %02x", cal_res[0], cal_res[1]);
    write_reg(0x39816, cal_res[0]);
    write_reg(0x39817, cal_res[1]);
}

void calibratexxx(void) {
    unsigned char Answer[256];
    unsigned char command[6] = {'S', 'L', 'I', 'N', 'K'};
    unsigned char cal_res[2];
    unsigned char command1[11] = {'S', 'L', 'I', 'N', 'K', CMD_WRITE_REG};//B
    unsigned char command2[11] = {'S', 'L', 'I', 'N', 'K', CMD_READ_REG}; //C
    unsigned char offset = 6;
    MYLOG(DEBUG, "calibrate");
    command1[offset++] = (uint8_t)(0x3986d >> 24);
    command1[offset++] = (uint8_t)(0x3986d >> 16);
    command1[offset++] = (uint8_t)(0x3986d >> 8);
    command1[offset++] = (uint8_t)(0x3986d);
    command1[offset++] = 1;
    gtrecvlen = RSP_LEN;
    writing_buf(command1, offset);
    recv_data(Answer);
    command[5] = CMD_CALIBRATE;//F
//  write_reg(0x3986d, 1);
    writing_buf(command, sizeof(command));
    recv_data(Answer);
    if(memcmp(command, Answer, RSP_LEN - 1 - 5) || Answer[RSP_LEN - 1]) {
    } else {
        offset = 6;
        command2[offset++] = (uint8_t)(0x32018 >> 24);
        command2[offset++] = (uint8_t)(0x32018 >> 16);
        command2[offset++] = (uint8_t)(0x32018 >> 8);
        command2[offset++] = (uint8_t)(0x32018);
        writing_buf(command2, offset);
        recv_data(Answer);
        cal_res[0] = Answer[RSP_LEN - 2];
        offset = 6;
        command2[offset++] = (uint8_t)(0x32019 >> 24);
        command2[offset++] = (uint8_t)(0x32019 >> 16);
        command2[offset++] = (uint8_t)(0x32019 >> 8);
        command2[offset++] = (uint8_t)(0x32019);
        writing_buf(command2, offset);
        recv_data(Answer);
        cal_res[1] = Answer[RSP_LEN - 2];
        offset = 6;
        command1[offset++] = (uint8_t)(0x39816 >> 24);
        command1[offset++] = (uint8_t)(0x39816 >> 16);
        command1[offset++] = (uint8_t)(0x39816 >> 8);
        command1[offset++] = (uint8_t)(0x39816);
        command1[offset++] = cal_res[0];
        writing_buf(command1, offset);
        recv_data(Answer);
        offset = 6;
        command1[offset++] = (uint8_t)(0x39817 >> 24);
        command1[offset++] = (uint8_t)(0x39817 >> 16);
        command1[offset++] = (uint8_t)(0x39817 >> 8);
        command1[offset++] = (uint8_t)(0x39817);
        command1[offset++] = cal_res[1];
        writing_buf(command1, offset);
        recv_data(Answer);
//        write_reg(0x39816, cal_res[0]);
//        write_reg(0x39817, cal_res[1]);
        // mark as calibrated
        write_flash(0x2001A, (uint8_t *)"A8", 1, 1);
    }
}


void write_info_init(void) {
    unsigned char flag = 1;
    unsigned char i;
    unsigned char buf[34];
    //6e02240022  4002800420012001009411003a0000000000010049324300540da802ff0000c3ff07
    //                      "\x40  \x02  \x80  \x04  \x20  \x01  \x20  \x01   \x00  \x94  \x11  \x00  \x3a  \x00  \x00  \x00  \x00  \x00   \x01  \x00  \x49  \x32  \x43  \x00  \x54  \x0c   \xa8 \x02    \xff \x00  \x00  \xc3  \xff  \x07";
    uint8_t info_buff[] = {0x40, 0x02, 0x80, 0x04, 0x20, 0x01, 0x20, 0x01, 0x00, 0x94, 0x11, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x49, 0x32, 0x43, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xc3, 0xff, 0x07};
    MYLOG(DEBUG, "write_info");
    //NFCC_Io_Init(Trans_Port);
    NFCC_Rest(1);
    write_info(0, info_buff, sizeof(info_buff));
    #if defined(SL6320)
    write_info(0x1b, &flag, 1);//打开6320串口 6320 = 1    6550 = 02   <6320demo板把此句话改为01  6550把此句话改为02>
    #else
    write_info(0x1b, &flag, 2);//打开6550串口 6320 = 1    6550 = 02   <6320demo板把此句话改为01  6550把此句话改为02>
    #endif
    calibrate();
    for(i = 0; i < 34; i++) {
        buf[i] = read_reg_get_val(0x32000 + i);
    }
    MYHEXS(DEBUG, buf, 34, "info");
}

void write_info_init_recovery(uint8_t *info_buff, uint8_t len) {
    MYLOG(DEBUG, "write_info");
    //NFCC_Io_Init(Trans_Port);
    NFCC_Rest(1);
    write_info(0, info_buff, len);
    MYHEXS(DEBUG, info_buff, len, "info");
}



uint32_t datalen = 0;
uint32_t totallen = 0;
uint16_t update_proc_from_code(void) {
    t_RetStatus ret;
    uint32_t cur_seq = 0;
    uint32_t pos = 0;
    uint8_t firmware_buff[1030];
    cur_seq = 0;
    MYLOG(DEBUG, "update_proc_from_code");
    //NFCC_Io_Init(Trans_Port);
    NFCC_Rest(1); //进入boot工作模式
    MYLOG(DEBUG, "erase_chip");
    if(erase_chip(0) == 1) {
        MYLOG(DEBUG, "erase_chip error");
        return 1;
    }
    MYLOG(DEBUG, "download_firmware_start(0x0000, %d)", totallen);
    //update bank1
    totallen = sizeof(bank1_image);
    if(NO_ERR != download_firmware_start(0x0000, totallen)) {
        MYLOG(DEBUG, "download_firmware_start(0x0000, %d) error", totallen);
        return 1;
    }
    cur_seq = 0;
    datalen = 1024;
    pos = 0;
    MYLOG(DEBUG, "download_firmware bank1");
    do {
        memcpy(firmware_buff + 3, (bank1_image + pos), datalen);
        if(NO_ERR != download_firmware(firmware_buff, cur_seq, datalen)) {
            MYLOG(DEBUG, "download_firmware bank1 error");
            return 1;
        }
        pos = pos + datalen;
        cur_seq++;
        if(totallen > 1024) {
            datalen = 1024;
            totallen = totallen - datalen;
        }
        if(totallen <= 1024) {
            datalen = totallen;
            break;
        }
    } while(1);
    MYLOG(DEBUG, "download_firmware_end");
    memcpy(firmware_buff + 3, (bank1_image + pos), datalen);
    if(NO_ERR != download_firmware_end(firmware_buff, cur_seq, datalen)) {
        MYLOG(DEBUG, "download_firmware_end error");
        return 1;
    }
    //update bank2
    MYLOG(DEBUG, "download_firmware_start(0x0000, %d)", totallen);
    totallen = sizeof(bank2_image);
    if(NO_ERR != download_firmware_start(0x010000, totallen)) {
        MYLOG(DEBUG, "download_firmware_start(0x0000, %d) error", totallen);
        return 1;
    }
    cur_seq = 0;
    datalen = 1024;
    pos = 0;
    MYLOG(DEBUG, "download_firmware bank2");
    do {
        memcpy(firmware_buff + 3, (bank2_image + pos), datalen);
        if(NO_ERR != download_firmware(firmware_buff, cur_seq, datalen)) {
            MYLOG(DEBUG, "download_firmware bank2 error");
            return 1;
        }
        pos = pos + datalen;
        cur_seq++;
        if(totallen > 1024) {
            datalen = 1024;
            totallen = totallen - datalen;
        }
        if(totallen <= 1024) {
            datalen = totallen;
            break;
        }
    } while(1);
    MYLOG(DEBUG, "download_firmware_end");
    memcpy(firmware_buff + 3, (bank2_image + pos), datalen);
    if(NO_ERR != download_firmware_end(firmware_buff, cur_seq, datalen)) {
        MYLOG(DEBUG, "download_firmware_end error");
        return 1;
    }
    MYLOG(DEBUG, "update_proc_from_code ok");
    MYENTER(DEBUG);
    return ret;
}
extern uint8_t info_data_bak[35];
void NFC_Firmware_Update(uint16_t flag, uint8_t *info_buff, uint8_t len) {     //固件升级
    MYLOG(DEBUG, "NFC_Firmware_Update %d", flag);
    if(flag == 3) {
        write_info_init();                       //如果芯片起来没有响应，尝试打开此宏
    } else {
        if(info_data_bak[0])
        {
            write_info_init_recovery(info_buff, len); //使用开机info区内容还原info内容，需要备份首次上电info区内容
        }
        else
        {
            write_info_init(); 
        }
    }
    update_proc_from_code();                      //开始升级固件
    SCI_Sleep(100);
}
