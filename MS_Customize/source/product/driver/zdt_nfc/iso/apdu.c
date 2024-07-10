#include "apdu.h"
/*  示例
    1、选择MF(主文件)
    CLA INS P1 P2 Lc Data
    00  A4  00 00
    P1=00选择主文件

    2、根据应用标识符选择应用
    CLA INS P1 P2 Lc Data
    00  A4  04 0C 07 A0 00 00 02 47 10 01
    P1=04 选择DF(专用文件)，此时数据域为DF的文件名（AID应用标识符）
    DF包括EF(基本文件)和其他DF
    DF1:EF.COM、EF.DG1........................EF.DG16、EF.Sod

    3、使用SELECT命令选择基本文件
    例：选择机读取区
    CLA INS P1 P2 Lc Data
    00  A4  02 0C 02 01 01

    4、读二进制
    CLA INS P1 P2 Lc Data
    00  B0  05 00 00       //读取文件前256Byte
    如果P1的最高位b8为1时，b6和b7位必须为0，b5-b1位表示待读取的基本文件的SFI；这时P2表示要读取数据起始位置在文件中的偏移量。
    如果P1的最高位为0时，P1的后7位和P2相连共同组成了一个超过256的整数，表示待读取数据块的起始位置在文件中的偏移量，可以用于超过256Byte文件的读取。
*/
/*TimeCOS/PBOS基本命令**********************************************************************************************************************/
//增加记录
//应用1：00 E2 00 08 08 11 22 33 44 55 66 77 88
//应用2：00 E2 00 00 06 11 22 33 44 55 66
APDU_SEND AppendRecord(uint8_t aid, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                  //0x00或者0x04
    apdu.INS  = 0xE2;
    apdu.P1   = 0x00;
    apdu.P2   = aid << 3;              //xxxxx000  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.len  = len + 1 ;              //data数据长度
    apdu.data[0] = len;                //lc
    memcpy(apdu.data + 1, data, len);  //写入的数据
    return apdu;
}
//扣款
//应用1：80 30 00 1C 04 00 00 00 02
APDU_SEND Decrease(uint8_t aid, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x80;                  //0x80或者0x84
    apdu.INS  = 0x30;
    apdu.P1   = 0x00;
    apdu.P2   = (aid << 3) | 0x04;     //xxxxx100  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.len  = len + 1;               //写入的数据长度 + Le
    apdu.data[0] = len;                //lc
    memcpy(apdu.data + 1, data, len);  //写入的数据
    return apdu;
}
//充钱
//应用1：80 32 00 0C 04 00 00 00 02
APDU_SEND Increase(uint8_t aid, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x80;                  //0x80或者0x84
    apdu.INS  = 0x32;
    apdu.P1   = 0x00;
    apdu.P2   = (aid << 3) | 0x04;    //xxxxx100  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.len  = len + 1;               //写入的数据长度 + Le
    apdu.data[0] = len;                //lc
    memcpy(apdu.data + 1, data, len);  //写入的数据
    return apdu;
}
//返回61XX XX表示响应数据长度。需要用这条命令获取响应数据
//应用1：00 C0 00 00 08
APDU_SEND GetResponse(uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                   //0x00 返回61XX XX表示响应数据长度。
    apdu.INS  = 0xC0;
    apdu.P1   = 0x00;
    apdu.P2   = 0x00;                   //xxxxx100  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.len  = 0x01;                   //写入的数据长度 + Le
    apdu.data[0] = len;                 //Le需要获取的数据长度
    return apdu;
}
//取随机数
//应用1：00 84 00 00 08
APDU_SEND GetChallenge(uint8_t ChallengeLen) {
    APDU_SEND apdu;
    apdu.CLA     = 0x00;
    apdu.INS     = 0x84;
    apdu.P1      = 0x00;
    apdu.P2      = 0x00;
    apdu.len     = 0x01;                 //长度
    apdu.data[0] = ChallengeLen;         //len = 0x08 取8字节的随机数 要求卡片返回的随机数长度4到10个字节
    return apdu;
}
//外部认证
//应用1：00 82 00 00 08 C1 8A 5B 4B 13 40 25 21
APDU_SEND ExternalAuthentication(uint8_t aid, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                  //0x00
    apdu.INS  = 0x82;
    apdu.P1   = 0x00;
    apdu.P2   = aid;
    apdu.len  = len + 1 ;              //加密后随机数长度 8
    apdu.data[0] = len;                //lc
    memcpy(apdu.data + 1, data, len);  //加密后的8字节随机数
    return apdu;
}
//内部认证
//应用1：00 88 00 01 08 11 22 33 44 55 66 77 88 DES加密
//应用2：00 88 01 01 08 07 CB F6 15 E7 D7 2F 96 DES解密
//应用3：00 88 02 01 08 11 22 33 44 55 66 77 88 DES和MAC加密
APDU_SEND InternalAuthentication(uint8_t aid, uint8_t mode, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                  //0x00
    apdu.INS  = 0x88;
    apdu.P1   = mode;                  //00加密  01  解密  02计算MAC
    apdu.P2   = aid;
    apdu.len  = len + 1;               //加密后随机数长度 8
    apdu.data[0] = len;
    memcpy(apdu.data + 1, data, len);  //加密后的8字节随机数
    return apdu;
}
//读取二级制文件
//应用1:00 B0 85 00 00   (6C08)
//      00 B0 85 00 08   (6C08)
APDU_SEND ReadBinary(uint8_t aid, uint8_t file_offset, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                        //0x00/04
    apdu.INS  = 0xB0;
    apdu.P1   = (0x04 << 5) | (aid & 0x1f); //100XXXXX P1高三位为100 低五位为短文件标识aid
    apdu.P2   = file_offset;                 //文件偏移量
    apdu.len  = 1;
    apdu.data[0] = len;                      //要读取的文件长度
    return apdu;
}
//读取二级制文件 当前文件 预读偏移量
//应用1:00 B0 00 00 00   (6C08)
//      00 B0 00 00 08
APDU_SEND ReadBinaryD(uint16_t file_offset, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                  //0x00/04
    apdu.INS  = 0xB0;
    apdu.P1   = ((uint8_t)(file_offset >> 8)) & 0x7f; //0XXXXXXX XXXXXXXX  最高位为0 其它为文件标识
    apdu.P2   = (uint8_t)(file_offset);                //文件偏移量
    apdu.len  = 1;
    apdu.data[0] = len;                                //要读取的文件长度
    return apdu;
}

//写二级制文件  定长记录文件
//应用1:00 DC 01 14 01 02 03 04 05 06 07 08 09 0A 0B 0C
APDU_SEND UpdateBinaryD(uint8_t aid, uint8_t RecordNumber, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                 //0x00/04
    apdu.INS  = 0xDC;
    apdu.P1   = RecordNumber;                         //记录号
    apdu.P2   = aid << 3;                             //XXXXX000  XXXXX短文件标识aid XXXXX = 0表示当前文件
    apdu.len  = 1;
    apdu.data[0] = len;                               //要读取的文件长度
    return apdu;
}

//写二级制文件  变长记录文件
//应用1:00 DC 00 OA 04 AA 02 11 22 新增1条标识为AA的新记录
//应用1:00 DC AA O8 04 CC 02 11 22 修改标识为AA的记录改为CC
APDU_SEND UpdateBinaryB(uint8_t mode, uint8_t aid, uint8_t RecordNumber, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                 //0x00/04
    apdu.INS  = 0xDC;
    apdu.P1   = RecordNumber;                         //记录号
    apdu.P2   = (aid << 3) | mode;                    //XXXXX短文件标识aid XXXXX = 0表示当前文件  mode = 0修改原始记录 mode = 2新增记录
    apdu.len  = 1;
    apdu.data[0] = len;                               //要读取的文件长度
    return apdu;
}
//读记录文件 按照记录号读取
//应用1:00 B2 02 0C 00   (6C0C)
//      00 B2 02 0C 0C
APDU_SEND ReadRecord(uint8_t aid, uint8_t RecordNumber, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                  //0x00/04
    apdu.INS  = 0xB2;
    apdu.P1   = RecordNumber;                          //记录号 定长记录文件 变长记录文件；循环文件和钱包文件，当前写入文件为01，上一条为02
    apdu.P2   = (aid << 3) | 0x04;                     //xxxxx100  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.len  = 1;
    apdu.data[0] = len;                                //要读取的文件长度
    return apdu;
}

//读记录文件  变成记录文件，按照记录表示读取
//应用1:00 B2 AA 38 00   (6C03)
//      00 B2 AA 38 03
APDU_SEND ReadRecordB(uint8_t aid, uint8_t Recordaid, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                   //0x00/04
    apdu.INS  = 0xB2;
    apdu.P1   = Recordaid;                              //记录标识 变长记录文件 P2低三位必须为0
    apdu.P2   = aid << 3;                               //xxxxx000  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.len  = 1;
    apdu.data[0] = len;                                 //要读取的文件长度
    return apdu;
}
//选择文件 按照文件标识符选择
//P1 =0x00 按照文件标识选择 P2 = 0x00
//P2 =0x04 按照DF名称选择   P2 = 0x00表示第一个或者仅有一个 P2=0x02表示下一个
//在任何情况下都可以使用3F00或者1PAY.SYS.DDF01选择MF
//应用1:00 A4 00 00 02 3F 00   (6117)
APDU_SEND SelectFileB(uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                  //0x00/04
    apdu.INS  = 0xA4;
    apdu.P1   = 0x00;
    apdu.P2   = 0x00;
    apdu.len  = len + 1;
    apdu.data[0] = len;                               //文件标识长度
    memcpy(apdu.data + 1, data, len);                 //文件标识符
    return apdu;
}

//选择文件 按照文件名称选择
//应用1:00 A4 04 00 09 A0 00 00 00 03 86 98 07 01
APDU_SEND SelectFileM(uint8_t P2, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                  //0x00/04
    apdu.INS  = 0xA4;
    apdu.P1   = 0x04;
    apdu.P2   = P2;                                    //P2 = 0 表示第一个或者仅有一个  P2 = 02 表示下一个
    apdu.len  = len + 1;
    apdu.data[0] = len;                               //文件标识长度
    memcpy(apdu.data + 1, data, len);                 //DF文件名
    return apdu;
}
//解锁
//应用1:80 2C 00 06 10 11 22 33 44 55 66 77 88 01 02 03 04 05 06 07 08
APDU_SEND Unblock(uint8_t aid, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x80;                                 //0x00/04
    apdu.INS  = 0x2C;
    apdu.P1   = 0x00;
    apdu.P2   = aid;
    apdu.len  = 0x11;
    apdu.data[0] = 0x10;
    memcpy(apdu.data + 1, data, len);                //8字节解锁口令 + 8字节新口令
    return apdu;
}

//验证口令
APDU_SEND VeriyPIN(uint8_t aid, uint8_t *data, uint8_t len) {
    APDU_SEND apdu;
    apdu.CLA  = 0x00;                                //0x00/04
    apdu.INS  = 0x20;
    apdu.P1   = 0x00;
    apdu.P2   = aid;                                 //口令密钥标识
    apdu.len  = len + 1;                             //02-08
    apdu.data[0] = len;
    memcpy(apdu.data + 1, data, len);                //外部输入的口令密钥
    return apdu;
}

//验证并修改口令
APDU_SEND VeriyChangePIN(uint8_t aid, uint8_t *data1, uint8_t *data2) {
    APDU_SEND apdu;
    apdu.CLA  = 0x80;                //0x80/84
    apdu.INS  = 0x24;
    apdu.P1   = 0x00;
    apdu.P2   = aid;                 //口令密钥标识
    apdu.len  = 0x11;                //02-08
    apdu.data[0] = 0x10;
    memcpy(apdu.data + 1, data1, 8); //8字节旧口令
    memcpy(apdu.data + 9, data2, 8); //8字节新口令
    return apdu;
}
/*中国金融IC卡专有命令**********************************************************************************************************************/
//读余额
APDU_SEND GetBalance(uint8_t type) {
    APDU_SEND apdu;
    apdu.CLA  = 0x80;
    apdu.INS  = 0x5C;
    apdu.P1   = 0x00;
    apdu.P2   = type;           //01 电子存折  02电子钱包
    apdu.len  = 0x01;
    apdu.data[0] = 4;           //要读取的余额长度
    return apdu;
}
//卡片擦除
APDU_SEND EraseDF(void) {
    APDU_SEND apdu;
    apdu.CLA  = 0x80;
    apdu.INS  = 0x0E;
    apdu.P1   = 0x00;
    apdu.P2   = 0x00;           //01 电子存折  02电子钱包
    apdu.len  = 0x01;
    apdu.data[0] = 0x00;         //要读取的余额长度
    return apdu;
}

//选择文件， 选择文件的方式通过参数p1和p2来配置
APDU_SEND SelectFile(uint8_t P1, uint8_t P2, uint8_t *data, uint8_t len, uint8_t le) {
    APDU_SEND apdu;
    apdu.CLA = 0x00; //0x00/04
    apdu.INS = 0xA4;
    apdu.P1 = P1;
    apdu.P2 = P2; //P2 = 0 表示第一个或者仅有一个  P2 = 02 表示下一个
    apdu.len = len;
    memcpy(apdu.data, data, len); //DF文件名
    if (le != 0xff)
        apdu.le = le;
    return apdu;
}

APDU_SEND ReadFile(uint16_t offset, uint16_t len) {
    APDU_SEND apdu;
    memset(&apdu, 0x00, sizeof(apdu));
    apdu.CLA = 0x00; //0x00/04
    apdu.INS = 0xB0;
    apdu.P1 = (offset >> 8) & 0xff; //记录标识 变长记录文件 P2低三位必须为0
    apdu.P2 = offset & 0xff;        //xxxxx000  B4-B8为短文件标识符  xxxxx = 0表示当前文件
    apdu.le = len;
    return apdu;
}

/**APUD API接口函数*************************************************************************************************************/
SL_NCI_PACK Snci_encode_Apdu_Res(uint8_t *data, uint8_t len) { //编码 响应
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = 0;
    nci_send.OID = 0;
    nci_send.PAYLOADLEN = len;
    memcpy(nci_send.PAYLOAD, data, len);
    MYLOG(DEBUG, "Snci_encode_Apdu_Res");
    return nci_send;
}

int8_t Snci_decode_Apdu_Res(uint8_t *buf, SL_NCI_PACK *nci_recv) { //解码 响应
    if ((0x90 == nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2]) && (0x00 == nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1])) {
        if (nci_recv->PAYLOADLEN >= 2)
            memcpy(buf, nci_recv->PAYLOAD, (nci_recv->PAYLOADLEN - 2));
        return nci_recv->PAYLOADLEN;
    }
    return -1;
}

SL_NCI_PACK Snci_encode_Apdu(APDU_SEND apdu) {         //编码 打包数据
    SL_NCI_PACK nci_send;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = 0;
    nci_send.OID = 0;
    nci_send.PAYLOADLEN = apdu.len + 4;
    memcpy(nci_send.PAYLOAD,    &apdu.CLA, 4);         //拷贝APDU包头
    memcpy(nci_send.PAYLOAD + 4, apdu.data, apdu.len); //拷贝APDU数据
    MYLOG(DEBUG, "Snci_encode_Apdu");
    return nci_send;
}

SL_NCI_PACK Snci_encode_Apdu_ext(APDU_SEND apdu, uint8_t le_flag) { //编码 打包数据
    SL_NCI_PACK nci_send;
    uint8_t offset = 4;
    nci_send.MT = MT_DATA_PACKET;
    nci_send.PBF = 0;
    nci_send.GID = 0;
    nci_send.OID = 0;
    nci_send.PAYLOADLEN = apdu.len + 4;
    memcpy(nci_send.PAYLOAD, &apdu.CLA, 4); //拷贝APDU包头
    if (apdu.len != 0) {
        nci_send.PAYLOAD[offset] = apdu.len;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 1;
        offset++;
        memcpy(nci_send.PAYLOAD + offset, apdu.data, apdu.len); //拷贝APDU数据
    }
    if (1 == le_flag) {
        nci_send.PAYLOAD[offset + apdu.len] = apdu.le;
        nci_send.PAYLOADLEN = nci_send.PAYLOADLEN + 1;
    }
    MYLOG(DEBUG, "Snci_encode_Apdu_ext");
    return nci_send;
}

t_DoStatus Snci_decode_Apdu(APDU_SEND *apdu, SL_NCI_PACK *nci_recv) {    //解码 解析数据
    apdu->CLA  = nci_recv->PAYLOAD[0];
    apdu->INS  = nci_recv->PAYLOAD[1];
    apdu->P1   = nci_recv->PAYLOAD[2];
    apdu->P2   = nci_recv->PAYLOAD[3];
    if(nci_recv->PAYLOADLEN >= 4) {
        apdu->len  = nci_recv->PAYLOADLEN - 4;//剩余字节长度
        memcpy(apdu->data, &nci_recv->PAYLOAD[4], apdu->len);
        MYLOG(DEBUG, "CLA = %02x INS = %02x P1 = %02x P2 = %02x len = %02x", apdu->CLA, apdu->INS, apdu->P1, apdu->P2, apdu->len);
        return DO_OK;
    }
    return DO_FAIL;
}

t_DoStatus Snci_Send_Apdu(APDU_SEND apdu, SL_NCI_PACK *nci_recv) { //编码
    if(Snci_Send_Data(1, Snci_encode_Apdu(apdu), NCI_ACK_TIMEOUT, nci_recv)  == CMD_RECE_OK) {
        MYLOG(DEBUG, "APDU(SW1 SW2) = 0x%02x%02x", nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ], nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ]);
        if((nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ] == 0x90) && (nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ] == 0x00)) { //SW1  SW2
            return DO_OK;
        }
        if(nci_recv->PAYLOADLEN == 2) {
            if(nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ] == 0x61) { //需要获取响应数据
                if(Snci_Send_Data(1, Snci_encode_Apdu(GetResponse(nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ])), NCI_ACK_TIMEOUT, nci_recv)  == CMD_RECE_OK) {
                    MYLOG(DEBUG, "APDU_61(SW1 SW2) = 0x%02x%02x", nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ], nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ]);
                    if((nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ] == 0x90) && (nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ] == 0x00)) { //SW1  SW2
                        return DO_OK;
                    }
                    return DO_FAIL;
                }
                return DO_TIMEOUT;
            }
            if(nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ] == 0x6C) { //需要数据长度
                apdu.data[apdu.len - 1] = nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1]; //修正获取文件长度
                if(Snci_Send_Data(1, Snci_encode_Apdu(apdu), NCI_ACK_TIMEOUT, nci_recv)  == CMD_RECE_OK) {
                    MYLOG(DEBUG, "APDU_6C(SW1 SW2) = 0x%02x%02x", nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ], nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ]);
                    if((nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 2 ] == 0x90) && (nci_recv->PAYLOAD[nci_recv->PAYLOADLEN - 1 ] == 0x00)) { //SW1  SW2
                        return DO_OK;
                    }
                    return DO_FAIL;
                }
                return DO_TIMEOUT;
            }
        }
        return DO_FAIL;
    }
    return DO_TIMEOUT;
}

void CpuCardTest(void) {
    //1、读取北京一卡通余额和交易记录
    //2、读取深圳通余额和交易记录
    //3、读长安通卡余额
    uint16_t i = 0;
    SL_NCI_PACK nci_recv;
    uint8_t SZT[] = "PAY.SZT";                                             //深圳通
    uint8_t CAT[] = {0xA0, 0x00, 0x00, 0x00, 0x03, 0x86, 0x98, 0x07, 0x01, 0x00}; //长安通卡 9
    uint8_t data[64] ;
    data[0] = 0x3f; data[1] = 0x00;
    if(Snci_Send_Apdu(SelectFileB(data, 2), &nci_recv) != DO_OK) {
        MYLOG(DEBUG, "选择MF失败");
    }
//    if(Snci_Send_Apdu(GetChallenge(4), &nci_recv) != DO_OK) {
//        //00000a a2612544dd26e86b 9000
//        //00000a 14a3b3675dbcc58f 9000
//        MYLOG(DEBUG, "获取8字节的随机数失败");
//    }
    if(Snci_Send_Apdu(GetBalance(0x02), &nci_recv) != DO_OK) {                           //读钱包余额
        //读深圳通余额或者交易记录
        if(Snci_Send_Apdu(SelectFileM(0, SZT, sizeof(SZT) - 1), &nci_recv) != DO_OK) {  //选择深圳通
            MYLOG(DEBUG, "选择SZT失败");
        } else {
            if(Snci_Send_Apdu(ReadBinary(0x15, 0, 0), &nci_recv) != DO_OK) {             //0X15 公共应用基本文件
                MYLOG(DEBUG, "读0x15文件失败");
            }
            if(Snci_Send_Apdu(GetBalance(0x02), &nci_recv) != DO_OK) {                   //读钱包余额
                MYLOG(DEBUG, "读余额失败");
            } else {
                float value = 0;
                value = (((nci_recv.PAYLOAD[0] & 0x7f) << 24) + (nci_recv.PAYLOAD[1] << 16) + (nci_recv.PAYLOAD[2] << 8) + nci_recv.PAYLOAD[3]) / 100.0;
                MYLOG(DEBUG, "深圳通余额：%.2f元", value);
            }
            //获取交易记录
            while(1) {
                if(Snci_Send_Apdu(ReadRecord(0x18, ++i, 0), &nci_recv) != DO_OK) {              //0X18 读交易记录
                    MYLOG(DEBUG, "交易记录读取完毕"); //返回000002 6a83
                    return;
                } else {
                    //       index   0元    1.9元  刷卡   卡机编号           日期
                    //000019  0bf6 000000 000000be  09  000062011101  2017-11-27 08:45:32 9000
                    //        0011 223344 55667788  99  001122334455  6677 88 99 00 11 22
                    float value = 0;
                    value = (((nci_recv.PAYLOAD[5] & 0x7f) << 24) + (nci_recv.PAYLOAD[6] << 16) + (nci_recv.PAYLOAD[7] << 8) + nci_recv.PAYLOAD[8]) / 100.0;
                    MYLOG(DEBUG, "%02x%02x-%02x-%02x %02x:%02x:%02x 卡机：%02x%02x%02x%02x%02x%02x 消费：%.2f元", nci_recv.PAYLOAD[16], nci_recv.PAYLOAD[17], nci_recv.PAYLOAD[18], nci_recv.PAYLOAD[19],
                          nci_recv.PAYLOAD[20], nci_recv.PAYLOAD[21], nci_recv.PAYLOAD[22], nci_recv.PAYLOAD[10], nci_recv.PAYLOAD[11], nci_recv.PAYLOAD[12], nci_recv.PAYLOAD[13], nci_recv.PAYLOAD[14], nci_recv.PAYLOAD[15],
                          value);
                }
            }
//            return;
        }
        if(Snci_Send_Apdu(SelectFileM(0, CAT, 9), &nci_recv) != DO_OK) {  //选择深圳通
            MYLOG(DEBUG, "选择CAT失败");
        } else {
            if(Snci_Send_Apdu(GetBalance(0x02), &nci_recv) != DO_OK) {                   //读钱包余额
                MYLOG(DEBUG, "读余额失败");
            } else {
                float value = 0;
                value = (((nci_recv.PAYLOAD[0] & 0x7f) << 24) + (nci_recv.PAYLOAD[1] << 16) + (nci_recv.PAYLOAD[2] << 8) + nci_recv.PAYLOAD[3]) / 100.0;
                MYLOG(DEBUG, "深长安通余额：%.2f元", value);
            }
        }
    } else {
        //读北京一卡通余额或者交易记录（在根目录，直接可以读取）
        float value = 0;
        value = (((nci_recv.PAYLOAD[0] & 0x7f) << 24) + (nci_recv.PAYLOAD[1] << 16) + (nci_recv.PAYLOAD[2] << 8) + nci_recv.PAYLOAD[3]) / 100.0;
        MYLOG(DEBUG, "北京一卡通余额：%.2f元", value);
        if(Snci_Send_Apdu(ReadBinary(0x15, 0, 0), &nci_recv) != DO_OK) {             //0X15 公共应用基本文件
            MYLOG(DEBUG, "读0x15文件失败");
        }
        //获取交易记录
        while(1) {
            if(Snci_Send_Apdu(ReadRecord(0x18, ++i, 0), &nci_recv) != DO_OK) {              //0X18 读交易记录
                MYLOG(DEBUG, "交易记录读取完毕"); //返回000002 6a83
                return;
            } else {
                //       index   0元    1.9元  刷卡   卡机编号           日期
                //000019  0bf6 000000 000000be  09  000062011101  2017-11-27 08:45:32 9000
                //        0011 223344 55667788  99  001122334455  6677 88 99 00 11 22
                float value = 0;
                value = (((nci_recv.PAYLOAD[5] & 0x7f) << 24) + (nci_recv.PAYLOAD[6] << 16) + (nci_recv.PAYLOAD[7] << 8) + nci_recv.PAYLOAD[8]) / 100.0;
                MYLOG(DEBUG, "%02x%02x-%02x-%02x %02x:%02x:%02x 卡机：%02x%02x%02x%02x%02x%02x 消费：%.2f元", nci_recv.PAYLOAD[16], nci_recv.PAYLOAD[17], nci_recv.PAYLOAD[18], nci_recv.PAYLOAD[19],
                      nci_recv.PAYLOAD[20], nci_recv.PAYLOAD[21], nci_recv.PAYLOAD[22], nci_recv.PAYLOAD[10], nci_recv.PAYLOAD[11], nci_recv.PAYLOAD[12], nci_recv.PAYLOAD[13], nci_recv.PAYLOAD[14], nci_recv.PAYLOAD[15],
                      value);
            }
        }
    }
    //A0 DB BF C1 19 2F F2 4A
//    data[0] = 0xA0; data[1] = 0xDB;data[2] = 0xBF; data[3] = 0xC1;data[4] = 0x19; data[5] = 0x2F; data[6] = 0xF2; data[7] = 0x4A;
//    if(Snci_Send_Apdu(ExternalAuthentication(0,data,8), &nci_recv) != DO_OK) {
//        //00000a a2612544dd26e86b 9000
//        //00000a 14a3b3675dbcc58f 9000
//        MYLOG(DEBUG, "外部认证失败");
//    }
//    if(Snci_Send_Apdu(EraseDF(), &nci_recv) != DO_OK) {
//        //00000a a2612544dd26e86b 9000
//        //00000a 14a3b3675dbcc58f 9000
//        MYLOG(DEBUG, "卡片擦除失败");
//    }
}
void CpuCardTest_FM1208_WhiteCard(void) {
    uint8_t KEY[]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    SL_NCI_PACK nci_recv;
    uint8_t data[64] ;
    data[0] = 0x3f; data[1] = 0x00;
    //1、选择主目录
    if(Snci_Send_Apdu(SelectFileB(data, 2), &nci_recv) != DO_OK) {
        MYLOG(DEBUG, "选择MF失败");
    }
    //2、获取4字节随机数
    if(Snci_Send_Apdu(GetChallenge(4), &nci_recv) != DO_OK) {
        //00000a a2612544dd26e86b 9000
        //00000a 14a3b3675dbcc58f 9000
        MYLOG(DEBUG, "获取4字节的随机数失败");
    }
    //3、加密06CEE4F20+0000000 与秘钥FFFFFFFFFFFFFFFF进行单倍长DES加密得到FEC63184B307AEFD
    memset(data, 0, 8);                      //清空8个字节的长度
    memcpy(data, nci_recv.PAYLOAD, 4);       //拷贝4字节的随机数
    MYHEXS(DEBUG, data, 8, "初始值：");
    One_DES_IV_Decode_Block(NULL, data, KEY);//DES加密
    //外部认证
    if(Snci_Send_Apdu(ExternalAuthentication(0, data, 8), &nci_recv) != DO_OK) {
        MYLOG(DEBUG, "外部认证失败");
    }
    MYLOG(DEBUG, "CpuCardTest_FM1208_WhiteCard over");
}




// CardReader mode(r/w)
// CLA INS P1 P2 LEN data LE
// 00 A4 04 00 07 D2 76 00 00 85 01 01 00 (r/w->CE)
// 90 00 (CE->r/w)

// 00 A4 00 0C 02 E1 03 (r/w->CE)
// 90 00 (CE->r/w)

// 00 B0 00 00 0F (r/w->CE)
// 00 0F 20 00 FF 00 FF 04 06 E1 04 00 FF 00 FF 90 00 (CE->r/w)

// 00 A4 00 0C 02 E1 04 (r/w->CE)
// 90 00 (CE->r/w)

// 00 B0 00 00 02 (r/w->CE)
// 00 0E 90 00 (CE->r/w)

// 00 B0 00 02 0E (r/w->CE)
// D1 01 0A 54 02 65 6E 31 32 36 2E 63 6F 6D 90 00 (CE->r/w)

#define enum2str(s)  (#s)

typedef enum {
    IDLE,
    SELECT_NDEF_APP,
    SELECT_NDEF_CC,
    READ_NDEF_CC,
    SELECT_NDEF_FILE,
    READ_NDEF_FILE_LEN,
    READ_NDEF_FILE
} NDEF_INTERACTIVE_STATE;

static NDEF_INTERACTIVE_STATE state = IDLE;

static int8_t select_ndef_context(uint8_t P1, uint8_t P2, uint8_t *data, uint8_t data_len, uint8_t rsp_len) {
    SL_NCI_PACK nci_recv;
    uint8_t res_buf[255];
    int8_t len;

    if (CMD_TIME_OUT == Snci_Send_Data(1, Snci_encode_Apdu_ext(SelectFile(P1, P2, data, data_len, rsp_len), (rsp_len != 0xff)), NCI_ACK_TIMEOUT, &nci_recv))
        return -1;

    if (nci_recv.MT == MT_DATA_PACKET) {
        if ((len = Snci_decode_Apdu_Res(res_buf, &nci_recv)) != -1) {
            if (2 == len) {
                return 0;
            }
        }
    }
    return -1;
}

int8_t select_ndef_APP(void) {
    int8_t ret;

    uint8_t NDEF_AID[] = {0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01};

    MYLOG(DEBUG, "%s", enum2str(SELECT_NDEF_APP));

    ret = select_ndef_context(0x04, 0x00, NDEF_AID, 7, 0x00);

    if (0 == ret) {
        state = SELECT_NDEF_CC;
        return 0;
    }

    state = IDLE;
    return -1;
}

int8_t select_ndef_CC(void)
{
    int8_t ret;

    uint8_t NDEF_CC_UID[] = {0xE1, 0x03};

    MYLOG(DEBUG, "%s", enum2str(SELECT_NDEF_CC));

    ret = select_ndef_context(0x00, 0x0c, NDEF_CC_UID, 2, 0xff);

    if (0 == ret) {
        state = READ_NDEF_CC;
        return 0;
    }

    state = IDLE;
    return -1;
}

int8_t read_ndef_CC(uint8_t * ndef_file_uid)
{
    SL_NCI_PACK nci_recv;

    uint8_t res_buf[255];
    int8_t len;

#pragma pack(1)
    typedef struct NDEF_TLV {
        uint8_t Type;
        uint8_t Length;
        uint8_t value[6];
    } NDEF_TLV_t;

    typedef struct NDEF_CC {
        uint16_t CCLen;
        uint8_t  version;
        uint16_t MLe;
        uint16_t MLc;
        NDEF_TLV_t Tlv;
    } NDEF_CC_t;
#pragma pack()

    NDEF_CC_t ndef_cc;

    MYLOG(DEBUG, "%s", enum2str(READ_NDEF_CC));

    if(CMD_TIME_OUT == Snci_Send_Data(1, Snci_encode_Apdu_ext(ReadFile(0x00, 0x0f), 1), NCI_ACK_TIMEOUT, &nci_recv))
        return CMD_TIME_OUT;

    if (nci_recv.MT == MT_DATA_PACKET) {
        if ((len = Snci_decode_Apdu_Res(res_buf, &nci_recv)) != -1) {
            if (len > 2 ) {
                memcpy(&ndef_cc, res_buf, len-2);
                MYHEXS(DEBUG, ndef_cc.Tlv.value, sizeof(ndef_cc.Tlv.value),">>");
                if(ndef_file_uid != 0x00) {
                    ndef_file_uid[0] = ndef_cc.Tlv.value[0];
                    ndef_file_uid[1] = ndef_cc.Tlv.value[1];
                }

                state = SELECT_NDEF_FILE;
                return 0;
            }
        }
    }

    state = IDLE;
	return -1;
}


int8_t select_ndef_file(uint8_t * ndef_file_uid)
{
    int8_t ret;

    MYLOG(DEBUG, "%s", enum2str(SELECT_NDEF_FILE));

    if(0x00 == ndef_file_uid) {
        MYLOG(DEBUG, "ndef_file_uid is null");
        state = IDLE;
        return -1;
    }

    ret = select_ndef_context(0x00, 0x0c, ndef_file_uid, 2, 0xff);

    if (0 == ret) {
        state = READ_NDEF_FILE_LEN;
        return 0;
    }

    state = IDLE;
    return -1;
}

int8_t read_ndef_file_len(uint16_t * ndef_file_len)
{
    SL_NCI_PACK nci_recv;
    uint8_t res_buf[255];
    int8_t len;

    MYLOG(DEBUG, "%s", enum2str(READ_NDEF_FILE_LEN));

    if (CMD_TIME_OUT == Snci_Send_Data(1, Snci_encode_Apdu_ext(ReadFile(0x00, 0x02), 1), NCI_ACK_TIMEOUT, &nci_recv))
        return CMD_TIME_OUT;

    if (nci_recv.MT == MT_DATA_PACKET) {

        if ((len = Snci_decode_Apdu_Res(res_buf, &nci_recv)) != -1) {
            if (len > 2) {
                *ndef_file_len = (res_buf[1]|(res_buf[0] << 8));
                MYLOG(DEBUG, "ndef file len is %d", *ndef_file_len);
                state = READ_NDEF_FILE;
                return 0;
            }
        }
    }
    state = IDLE;
    return -1;
}

int8_t read_ndef_file(uint16_t ndef_file_len)
{
    SL_NCI_PACK nci_recv;
    uint8_t res_buf[255];
    int8_t len;

    MYLOG(DEBUG, "%s", enum2str(READ_NDEF_FILE));

    if (CMD_TIME_OUT == Snci_Send_Data(1, Snci_encode_Apdu_ext(ReadFile(0x02, ndef_file_len), 1), NCI_ACK_TIMEOUT, &nci_recv))
        return CMD_TIME_OUT;

    if (nci_recv.MT == MT_DATA_PACKET) {

        if ((len = Snci_decode_Apdu_Res(res_buf, &nci_recv)) != -1) {
            if (len > 2) {
                MYHEXS(DEBUG, res_buf, len-2,">> ndef file content: ");
                state = IDLE;
                return 0;
            }
        }
    }
    state = IDLE;
    return -1;
}

void get_ndef(void)
{
    uint8_t ndef_file_uid[2];
    uint16_t ndef_file_len;
    MYLOG(DEBUG, "get_ndef Entry");

    state = SELECT_NDEF_APP;
    while (IDLE != state) {
        switch (state) {
            case SELECT_NDEF_APP:
                select_ndef_APP();
                break;
            case SELECT_NDEF_CC:
                select_ndef_CC();
                break;
            case READ_NDEF_CC:
                read_ndef_CC(ndef_file_uid);
                break;
            case SELECT_NDEF_FILE:
                select_ndef_file(ndef_file_uid);
                break;
            case READ_NDEF_FILE_LEN:
                read_ndef_file_len(&ndef_file_len);
            case READ_NDEF_FILE:
                read_ndef_file(ndef_file_len);
                break;
            default:
                MYLOG(DEBUG, "the state is err !!!");
                break;
        }
    }
}





