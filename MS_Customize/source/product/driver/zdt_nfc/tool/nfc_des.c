#include "nfc_des.h"
/*-------------------------------------------------------
   2017  06 . 08
   DES 加密   8字节为一个数据块
   实现单、双、三DES加密解密 并实现CBC模式接口
--------------------------------------------------------*/

void BitsCopy(unsigned char *DatOut, unsigned char *DatIn, int Len);  // 数组复制

void ByteToBit(unsigned char *DatOut, unsigned char *DatIn, int byte_Num); // 字节到位
void BitToByte(unsigned char *DatOut, unsigned char *DatIn, int byte_Num); // 位到字节

void TablePermute(unsigned char *DatOut, unsigned char *DatIn, const unsigned char *Table, int Num); // 位表置换函数
void LoopMove(unsigned char *DatIn, int Len, int Num);     // 循环左移 Len长度 Num移动位数
void Xor(unsigned char *DatA, unsigned char *DatB, int Num);         // 异或函数

void S_Change(unsigned char *DatOut, unsigned char *DatIn);   // S盒变换
void F_Change(unsigned char *DatIn, unsigned char *DatKi);    // F函数

//设置默认密钥 获取子密钥Ki
void Set_One_DES_64bitKey(unsigned char *KeyIn);

// 执行DES加密
void DES_Encrypt_Block(unsigned char *MesIn, unsigned char *MesOut);

// 执行DES解密
void DES_Decode_Block(unsigned  char *MesIn, unsigned char *MesOut);


// 对明文执行IP置换得到L0,R0 （L左32位,R右32位）               [明文操作]
const unsigned char IP_Table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// 对迭代后的L16,R16执行IP逆置换,输出密文
const unsigned char IPR_Table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

/*--------------------------- 迭代法则 ----------------------------*/

// F函数,32位的R0进行E变换,扩为48位输出 (R1~R16)        [备用A]  [明文操作]
const unsigned char E_Table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// 子密钥K(i)的获取 密钥为K 抛弃第6,16,24,32,40,48,64位          [密钥操作]
// 用PC1选位 分为 前28位C0,后28位D0 两部分
const unsigned char PC1_Table[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

// 对C0,D0分别进行左移,共16次,左移位数与下面对应                 [密钥操作]
const unsigned char Move_Table[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// C1,D1为第一次左移后得到,进行PC2选位,得到48位输出K1   [备用B]   [密钥操作]
const unsigned char PC2_Table[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

/*------------- F函数 备用A和备用B 异或 得到48位输出 ---------------*/

// 异或后的结果48位分为8组,每组6位,作为8个S盒的输入             [组合操作]
// S盒以6位作为输入(8组),4位作为输出(4*(8组)=32位)
// S工作原理 假设输入为A=abcdef ,则bcde所代表的数是0-15之间的
// 一个数记为 X=bcde ,af代表的是0-3之间的一个数,记为 Y=af
// 在S1的X列,Y行找到一个数Value,它在0-15之间,可以用二进制表示
// 所以为4bit (共32位)
const unsigned char S_Box[8][4][16] = {
    // S1
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
    // S2
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
    // S3
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
    // S4
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
    // S5
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
    // S6
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
    // S7
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
    // S8
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

// F函数 最后第二步,对S盒输出的32进行P置换                     [组合操作]
// 输出的值参与一次迭代:
// L(i)=R(i-1)
// R(i)=L(i-1)^f(R(i-1),K(i)) 异或
const unsigned char P_Table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

// 16个子密钥K(1~16)
static unsigned char SubKey[16][48] = { 0 };


/*-------------------------------
把DatIn开始的长度位Len位的二进制
复制到DatOut后
--------------------------------*/
void BitsCopy(unsigned char *DatOut, unsigned char *DatIn, int Len) {   // 数组复制 OK
    int i = 0;
    for(i = 0; i < Len; i++) {
        DatOut[i] = DatIn[i];
    }
}

/*-------------------------------
字节转换成位函数
byte_Num 多少个字节
--------------------------------*/
void ByteToBit(unsigned char *DatOut, unsigned char *DatIn, int byte_Num) {     // OK
    int i = 0;
    for(i = 0; i < byte_Num * 8; i++) {
        //低bit在缓存高地址
        DatOut[i] = ((DatIn[i / 8] << (i % 8)) & 0x80) ? 1 : 0;
    }
}

/*-------------------------------
位转换成字节函数
byte_Num: 多少个字节
---------------------------------*/
void BitToByte(unsigned char *DatOut, unsigned char *DatIn, int byte_Num) {      // OK
    int i = 0;
    for(i = 0; i < byte_Num; i++) { //先把数据清零
        DatOut[i] = 0;
    }
    for(i = 0; i < byte_Num * 8; i++) {
        DatOut[i / 8] = (DatOut[i / 8] << 1) | DatIn[i];  //低bit在缓存高地址
    }
}

// 表置换函数  OK
void TablePermute(unsigned char *DatOut, unsigned char *DatIn, const unsigned char *Table, int Num) {
    int i = 0;
    unsigned char Temp[256] = { 0 };
    for(i = 0; i < Num; i++) {               // Num为置换的长度
        Temp[i] = DatIn[Table[i] - 1];  // 原来的数据按对应的表上的位置排列
    }
    BitsCopy(DatOut, Temp, Num);       // 把缓存Temp的值输出
}

// 子密钥的移位
void LoopMove(unsigned char *DatIn, int Len, int Num) { // 循环左移 Len数据长度 Num移动位数
    unsigned char Temp[10] = { 0 };    // 缓存
    BitsCopy(Temp, DatIn, Num);       // 将数据最左边的Num位(被移出去的)存入Temp
    BitsCopy(DatIn, DatIn + Num, Len - Num); // 将数据左边开始的第Num移入原来的空间
    BitsCopy(DatIn + Len - Num, Temp, Num);  // 将缓存中移出去的数据加到最右边
}

// 按位异或
void Xor(unsigned char *DatA, unsigned char *DatB, int Num) {         // 异或函数
    int i = 0;
    for(i = 0; i < Num; i++) {
        DatA[i] = DatA[i] ^ DatB[i];                  // 异或
    }
}

// 输入48位 输出32位 与Ri异或。
//把48bit 数据分成八组 进入八个S盒 得到8个十进制数，把8个十进制数转换成32位bit串
void S_Change(unsigned char *DatOut, unsigned char *DatIn) {  // S盒变换
    int i, X, Y;                                    // i为8个S盒
    unsigned char data[8];                       //S盒中的数据
    for(i = 0, Y = 0, X = 0; i < 8; i++, DatIn += 6) {        // 每执行一次,输入数据偏移6位
        // 每执行一次,输出数据偏移4位
        Y = (DatIn[0] << 1) + DatIn[5];                          // af代表第几行
        X = (DatIn[1] << 3) + (DatIn[2] << 2) + (DatIn[3] << 1) + DatIn[4]; // bcde代表第几列
        data[i] = S_Box[i][Y][X];  //得到S盒中的数据  4bit 数据
    }
    // 把找到的点数据换为二进制
    for(i = 0; i < 8; i++) {
        DatOut[i * 4 + 0] = (data[i] & 0x08) ? 1 : 0; //最高位
        DatOut[i * 4 + 1] = (data[i] & 0x04) ? 1 : 0;
        DatOut[i * 4 + 2] = (data[i] & 0x02) ? 1 : 0;
        DatOut[i * 4 + 3] = (data[i] & 0x01) ? 1 : 0;
    }
}

// F函数
void F_Change(unsigned char *DatIn, unsigned char *DatKi) {     // F函数
    unsigned char MiR[48] = { 0 };             // 输入32位通过E选位变为48位
    TablePermute(MiR, DatIn, E_Table, 48);      //把32bit 数据扩展到 48bit
    Xor(MiR, DatKi, 48);                   // 和子密钥异或
    S_Change(DatIn, MiR);                 // S盒变换 把48bit 数据分成八组 进入八个S盒
    TablePermute(DatIn, DatIn, P_Table, 32);   // P置换后输出  32bit串
}


//设置默认密钥 获取子密钥Ki
void Set_One_DES_64bitKey(unsigned char *KeyIn) {
    int i = 0;
    unsigned char KeyBit[64] = { 0 };                // 密钥二进制存储空间
    unsigned char *KiL = &KeyBit[0];    //前28
    unsigned char *KiR = &KeyBit[28];  //后28共56
    ByteToBit(KeyBit, KeyIn, 8);                    // 把密钥转为二进制存入KeyBit
    TablePermute(KeyBit, KeyBit, PC1_Table, 56);      // PC1表置换 56次
    for(i = 0; i < 16; i++) {
        LoopMove(KiL, 28, Move_Table[i]);       // 前28位左移
        LoopMove(KiR, 28, Move_Table[i]);         // 后28位左移
        TablePermute(SubKey[i], KeyBit, PC2_Table, 48);  //得到48位 子秘钥
        // 二维数组 SubKey[i]为每一行起始地址
        // 每移一次位进行PC2置换得 Ki 48位
    }
}

// 执行DES加密
void DES_Encrypt_Block(unsigned char *MesIn, unsigned char *MesOut) {
    // 字节输入 Bin运算 Hex输出
    int i = 0;
    unsigned char MesBit[64] = { 0 };        // 明文二进制存储空间 64位
    unsigned char Temp[32] = { 0 };
    unsigned char *MiL = &MesBit[0];//前32位
    unsigned char *MiR = &MesBit[32]; //  后32位
    ByteToBit(MesBit, MesIn, 8);                 // 把明文换成二进制存入MesBit
    TablePermute(MesBit, MesBit, IP_Table, 64);    // IP置换
    for(i = 0; i < 16; i++) {                      // 迭代16次
        BitsCopy(Temp, MiL, 32);             // 临时存储 备份 Li-1
        BitsCopy(MiL, MiR, 32);              // 得到Li   Li = Ri-1
        F_Change(MiR, SubKey[i]);            // F函数变换  进行E扩展 变成48位 与子秘钥异或
        Xor(MiR, Temp, 32);                  // 得到Ri   Ri =Li-1 异或 F（Ri-1，Ki）
    }
    //最后一轮要改变左右32bit位置   Ri在前 Li 在后进行合并   合并后进行IP逆置换
    BitsCopy(Temp, MiL, 32);   //把左边 32bit 备份
    BitsCopy(MesBit, MiR, 32);  //把右32位 移到前面
    BitsCopy(&MesBit[32], Temp, 32);  //把前32位 移到后面  合并完成
    TablePermute(MesBit, MesBit, IPR_Table, 64); //进行IP逆置换
    BitToByte(MesOut, MesBit, 8);
}

// 执行DES解密
void DES_Decode_Block(unsigned  char *MesIn, unsigned char *MesOut) {
    // Hex输入 Bin运算 字节输出
    int i = 0;
    unsigned char MesBit[64] = { 0 };         // 密文二进制存储空间 64位
    unsigned char Temp[32] = { 0 };
    unsigned char *MiL = &MesBit[0];  //前32位
    unsigned char *MiR = &MesBit[32]; //  后32位
    ByteToBit(MesBit, MesIn, 8);
    TablePermute(MesBit, MesBit, IP_Table, 64);    // IP置换
    for(i = 0; i < 16; i++) {
        BitsCopy(Temp, MiL, 32);             // 临时存储 备份 Li-1
        BitsCopy(MiL, MiR, 32);              // 得到Li   Li = Ri-1
        F_Change(MiR, SubKey[15 - i]);        // F函数变换  进行E扩展 变成48位 与子秘钥异或
        Xor(MiR, Temp, 32);                   // 得到Ri   Ri =Li-1 异或 F（Ri-1，Ki）
    }
    //最后一轮需要左右对换 Ri在前 Li 在后进行合并   合并后进行IP逆置换
    BitsCopy(Temp, MiL, 32);   //把左边 32bit 备份
    BitsCopy(MesBit, MiR, 32);  //把右32位 移到前面
    BitsCopy(&MesBit[32], Temp, 32);  //把前32位 移到后面  合并完成
    TablePermute(MesBit, MesBit, IPR_Table, 64);
    BitToByte(MesOut, MesBit, 8);
}

/*************************************************************************************
以下函数加入 DES算法 向量    为CBC加密模式 提供接口
一、加密过程 ：
密文块0 = （明文0 异或 IV ）的结果   用秘钥加密
密文块1 = （明文1 异或 密文块0） 的结果 用秘钥加密

二、解密过程 ：
1、先用秘钥对密文解密  得到 数据  X0
2、X0 异或 向量  得到明文
3、前一个数据的密文块 为后一个数据块的向量
***************************************************************************************/

//执行单DES加密  加密一个块      8字节数据
//IV_IN_OUT：    初始化向量输入  密文输出  为空时则无向量值（CBC模式加密时，前一个数据的密文为后一个明文块的向量）
//Mes_IN_OUT:    明文输入        密文输出
//Key64bit:      64bit 秘钥      8字节
void One_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit) {
    unsigned char   temp[8];
    if(Key64bit == NULL) { //秘钥不能为空
        return;
    }
    if(IV_IN_OUT != NULL) {
        Xor(Mes_IN_OUT, IV_IN_OUT, 8);     //先把明文数据与加密向量 异或   如果向量全为0 则不改变明文的值
    }
    Set_One_DES_64bitKey(Key64bit);        //设置秘钥  生成子秘钥
    DES_Encrypt_Block(Mes_IN_OUT, temp);   //执行DES加密  密文保存在 temp 中
    if(IV_IN_OUT != NULL) {
        BitsCopy(IV_IN_OUT, temp, 8);      //把密文拷贝到向量缓存中
    }
    BitsCopy(Mes_IN_OUT, temp, 8);         //把密文拷贝到明文数据缓存中
}


//执行单DES解密  解密一个块      8字节数据
//IV_IN_OUT：    初始化向量输入  原密文输出 为空时则无向量值（CBC模式解密时，前一个数据块的密文为后一个密文块解密后的向量）
//Mes_IN_OUT:    密文输入        明文输出
//Key64bit:      64bit 秘钥      8字节
void One_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit) {
    unsigned char   temp[8];
    if(Key64bit == NULL) { //秘钥不能为空
        return;
    }
    Set_One_DES_64bitKey(Key64bit);            //设置秘钥  生成子秘钥
    DES_Decode_Block(Mes_IN_OUT, temp);        //执行DES解密  解密的结果保存在 temp 中
    if(IV_IN_OUT != NULL) {
        Xor(temp, IV_IN_OUT, 8);               //把解密后的结果与向量异或得到明文  如果向量全为0 则不改变值
        BitsCopy(IV_IN_OUT, Mes_IN_OUT, 8);    //把原密文拷贝到向量数据缓存中
    }
    BitsCopy(Mes_IN_OUT, temp, 8);             //把明文拷贝到密文数据缓存中
}




//执行双DES加密  加密一个块      8字节数据 加密顺序  左加密 右解密 左加密
//IV_IN_OUT：    初始化向量输入  密文输出 （CBC模式加密时，前一个数据的密文为后一个明文块的向量）
//Mes_IN_OUT:    明文输入        密文输出
//Key128bit:     128bit 秘钥     16字节
void Two_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit) {
    unsigned char   temp[8];
    if(Key128bit == NULL) {
        return;
    }
    if(IV_IN_OUT != NULL) {
        Xor(Mes_IN_OUT, IV_IN_OUT, 8);          //先把明文数据与加密向量 异或   如果向量全为0 则不改变明文的值
    }
    Set_One_DES_64bitKey(Key128bit);            //设置秘钥  生成子秘钥    左边8字节秘钥
    DES_Encrypt_Block(Mes_IN_OUT, temp);        //执行DES加密  密文保存在 temp 中
    Set_One_DES_64bitKey(&Key128bit[8]);        //设置秘钥  生成子秘钥    右边8字节秘钥
    DES_Decode_Block(temp, temp);               //执行DES解密  结果保存在 temp 中
    Set_One_DES_64bitKey(Key128bit);            //设置秘钥  生成子秘钥    左边8字节秘钥
    DES_Encrypt_Block(temp, temp);              //执行DES加密  密文保存在 temp 中
    if(IV_IN_OUT != NULL) {
        BitsCopy(IV_IN_OUT, temp, 8);           //把密文拷贝到向量缓存中
    }
    BitsCopy(Mes_IN_OUT, temp, 8);              //把密文拷贝到明文数据缓存中
}


//执行双DES解密  解密一个块      8字节数据 解密流程  左解密  右加密   左解密
//IV_IN_OUT：    初始化向量输入  原密文输出 为空时则无向量值（CBC模式解密时，前一个数据块的密文为后一个密文块解密后的向量）
//Mes_IN_OUT:    密文输入        明文输出
//Key128bit:     128bit 秘钥     16字节
void Two_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit) {
    unsigned char   temp[8];
    if(Key128bit == NULL) { //秘钥不能为空
        return;
    }
    Set_One_DES_64bitKey(Key128bit);           //设置秘钥  生成子秘钥
    DES_Decode_Block(Mes_IN_OUT, temp);        //执行DES解密  解密的结果保存在 temp 中
    Set_One_DES_64bitKey(&Key128bit[8]);       //设置秘钥  生成子秘钥    右边8字节秘钥
    DES_Encrypt_Block(temp, temp);             //执行DES加密  密文保存在 temp 中
    Set_One_DES_64bitKey(Key128bit);           //设置秘钥  生成子秘钥
    DES_Decode_Block(temp, temp);              //执行DES解密  解密的结果保存在 temp 中
    if(IV_IN_OUT != NULL) {
        Xor(temp, IV_IN_OUT, 8);               //把解密后的结果与向量异或得到明文  如果向量全为0 则不改变值
        BitsCopy(IV_IN_OUT, Mes_IN_OUT, 8);    //把原密文拷贝到向量数据缓存中
    }
    BitsCopy(Mes_IN_OUT, temp, 8);             //把明文拷贝到密文数据缓存中
}



//执行三DES加密  加密一个块      8字节数据 加密顺序  左加密 中解密 右加密
//IV_IN_OUT：    初始化向量输入  密文输出 （CBC模式加密时，前一个数据的密文为后一个明文块的向量）
//Mes_IN_OUT:    明文输入        密文输出
//Key192bit:     192bit 秘钥     24字节
void Three_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit) {
    unsigned char   temp[8];
    if(Key192bit == NULL) {
        return;
    }
    if(IV_IN_OUT != NULL) {
        Xor(Mes_IN_OUT, IV_IN_OUT, 8);          //先把明文数据与加密向量 异或   如果向量全为0 则不改变明文的值
    }
    Set_One_DES_64bitKey(Key192bit);            //设置秘钥  生成子秘钥    左边8字节秘钥
    DES_Encrypt_Block(Mes_IN_OUT, temp);        //执行DES加密  密文保存在 temp 中
    Set_One_DES_64bitKey(&Key192bit[8]);        //设置秘钥  生成子秘钥    中间8字节秘钥
    DES_Decode_Block(temp, temp);               //执行DES解密  结果保存在 temp 中
    Set_One_DES_64bitKey(&Key192bit[16]);       //设置秘钥  生成子秘钥    右边8字节秘钥
    DES_Encrypt_Block(temp, temp);              //执行DES加密  密文保存在 temp 中
    if(IV_IN_OUT != NULL) {
        BitsCopy(IV_IN_OUT, temp, 8);           //把密文拷贝到向量缓存中
    }
    BitsCopy(Mes_IN_OUT, temp, 8);              //把密文拷贝到明文数据缓存中
}

//执行三DES解密  解密一个块      8字节数据 解密流程  右解密  中加密   左解密
//IV_IN_OUT：    初始化向量输入  原密文输出 为空时则无向量值（CBC模式解密时，前一个数据块的密文为后一个密文块解密后的向量）
//Mes_IN_OUT:    密文输入        明文输出
//Key192bit:     192bit 秘钥     24字节
void Three_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit) {
    unsigned char   temp[8];
    if(Key192bit == NULL) { //秘钥不能为空
        return;
    }
    Set_One_DES_64bitKey(&Key192bit[16]);      //设置秘钥  生成子秘钥  右边8字节秘钥
    DES_Decode_Block(Mes_IN_OUT, temp);        //执行DES解密  解密的结果保存在 temp 中
    Set_One_DES_64bitKey(&Key192bit[8]);       //设置秘钥  生成子秘钥    中间8字节秘钥
    DES_Encrypt_Block(temp, temp);             //执行DES加密  密文保存在 temp 中
    Set_One_DES_64bitKey(Key192bit);           //设置秘钥  生成子秘钥  左边8字节秘钥
    DES_Decode_Block(temp, temp);              //执行DES解密  解密的结果保存在 temp 中
    if(IV_IN_OUT != NULL) {
        Xor(temp, IV_IN_OUT, 8);               //把解密后的结果与向量异或得到明文  如果向量全为0 则不改变值
        BitsCopy(IV_IN_OUT, Mes_IN_OUT, 8);    //把原密文拷贝到向量数据缓存中
    }
    BitsCopy(Mes_IN_OUT, temp, 8);             //把明文拷贝到密文数据缓存中
}

void DES_Test(void) {
    unsigned char KEY[]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char VALUE[] = {0x06, 0xCE, 0xE4, 0xF2, 0x00, 0x00, 0x00, 0x00};
    One_DES_IV_Encrypt_Block(NULL, VALUE, KEY);
    MYHEXS(DEBUG, VALUE, 8, "加密数据：");
    One_DES_IV_Decode_Block(NULL, VALUE, KEY);
    MYHEXS(DEBUG, VALUE, 8, "解密数据：");
}

