#include "nfc_des.h"
/*-------------------------------------------------------
   2017  06 . 08
   DES ����   8�ֽ�Ϊһ�����ݿ�
   ʵ�ֵ���˫����DES���ܽ��� ��ʵ��CBCģʽ�ӿ�
--------------------------------------------------------*/

void BitsCopy(unsigned char *DatOut, unsigned char *DatIn, int Len);  // ���鸴��

void ByteToBit(unsigned char *DatOut, unsigned char *DatIn, int byte_Num); // �ֽڵ�λ
void BitToByte(unsigned char *DatOut, unsigned char *DatIn, int byte_Num); // λ���ֽ�

void TablePermute(unsigned char *DatOut, unsigned char *DatIn, const unsigned char *Table, int Num); // λ���û�����
void LoopMove(unsigned char *DatIn, int Len, int Num);     // ѭ������ Len���� Num�ƶ�λ��
void Xor(unsigned char *DatA, unsigned char *DatB, int Num);         // �����

void S_Change(unsigned char *DatOut, unsigned char *DatIn);   // S�б任
void F_Change(unsigned char *DatIn, unsigned char *DatKi);    // F����

//����Ĭ����Կ ��ȡ����ԿKi
void Set_One_DES_64bitKey(unsigned char *KeyIn);

// ִ��DES����
void DES_Encrypt_Block(unsigned char *MesIn, unsigned char *MesOut);

// ִ��DES����
void DES_Decode_Block(unsigned  char *MesIn, unsigned char *MesOut);


// ������ִ��IP�û��õ�L0,R0 ��L��32λ,R��32λ��               [���Ĳ���]
const unsigned char IP_Table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// �Ե������L16,R16ִ��IP���û�,�������
const unsigned char IPR_Table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

/*--------------------------- �������� ----------------------------*/

// F����,32λ��R0����E�任,��Ϊ48λ��� (R1~R16)        [����A]  [���Ĳ���]
const unsigned char E_Table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// ����ԿK(i)�Ļ�ȡ ��ԿΪK ������6,16,24,32,40,48,64λ          [��Կ����]
// ��PC1ѡλ ��Ϊ ǰ28λC0,��28λD0 ������
const unsigned char PC1_Table[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

// ��C0,D0�ֱ��������,��16��,����λ���������Ӧ                 [��Կ����]
const unsigned char Move_Table[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// C1,D1Ϊ��һ�����ƺ�õ�,����PC2ѡλ,�õ�48λ���K1   [����B]   [��Կ����]
const unsigned char PC2_Table[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

/*------------- F���� ����A�ͱ���B ��� �õ�48λ��� ---------------*/

// ����Ľ��48λ��Ϊ8��,ÿ��6λ,��Ϊ8��S�е�����             [��ϲ���]
// S����6λ��Ϊ����(8��),4λ��Ϊ���(4*(8��)=32λ)
// S����ԭ�� ��������ΪA=abcdef ,��bcde�����������0-15֮���
// һ������Ϊ X=bcde ,af�������0-3֮���һ����,��Ϊ Y=af
// ��S1��X��,Y���ҵ�һ����Value,����0-15֮��,�����ö����Ʊ�ʾ
// ����Ϊ4bit (��32λ)
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

// F���� ���ڶ���,��S�������32����P�û�                     [��ϲ���]
// �����ֵ����һ�ε���:
// L(i)=R(i-1)
// R(i)=L(i-1)^f(R(i-1),K(i)) ���
const unsigned char P_Table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

// 16������ԿK(1~16)
static unsigned char SubKey[16][48] = { 0 };


/*-------------------------------
��DatIn��ʼ�ĳ���λLenλ�Ķ�����
���Ƶ�DatOut��
--------------------------------*/
void BitsCopy(unsigned char *DatOut, unsigned char *DatIn, int Len) {   // ���鸴�� OK
    int i = 0;
    for(i = 0; i < Len; i++) {
        DatOut[i] = DatIn[i];
    }
}

/*-------------------------------
�ֽ�ת����λ����
byte_Num ���ٸ��ֽ�
--------------------------------*/
void ByteToBit(unsigned char *DatOut, unsigned char *DatIn, int byte_Num) {     // OK
    int i = 0;
    for(i = 0; i < byte_Num * 8; i++) {
        //��bit�ڻ���ߵ�ַ
        DatOut[i] = ((DatIn[i / 8] << (i % 8)) & 0x80) ? 1 : 0;
    }
}

/*-------------------------------
λת�����ֽں���
byte_Num: ���ٸ��ֽ�
---------------------------------*/
void BitToByte(unsigned char *DatOut, unsigned char *DatIn, int byte_Num) {      // OK
    int i = 0;
    for(i = 0; i < byte_Num; i++) { //�Ȱ���������
        DatOut[i] = 0;
    }
    for(i = 0; i < byte_Num * 8; i++) {
        DatOut[i / 8] = (DatOut[i / 8] << 1) | DatIn[i];  //��bit�ڻ���ߵ�ַ
    }
}

// ���û�����  OK
void TablePermute(unsigned char *DatOut, unsigned char *DatIn, const unsigned char *Table, int Num) {
    int i = 0;
    unsigned char Temp[256] = { 0 };
    for(i = 0; i < Num; i++) {               // NumΪ�û��ĳ���
        Temp[i] = DatIn[Table[i] - 1];  // ԭ�������ݰ���Ӧ�ı��ϵ�λ������
    }
    BitsCopy(DatOut, Temp, Num);       // �ѻ���Temp��ֵ���
}

// ����Կ����λ
void LoopMove(unsigned char *DatIn, int Len, int Num) { // ѭ������ Len���ݳ��� Num�ƶ�λ��
    unsigned char Temp[10] = { 0 };    // ����
    BitsCopy(Temp, DatIn, Num);       // ����������ߵ�Numλ(���Ƴ�ȥ��)����Temp
    BitsCopy(DatIn, DatIn + Num, Len - Num); // ��������߿�ʼ�ĵ�Num����ԭ���Ŀռ�
    BitsCopy(DatIn + Len - Num, Temp, Num);  // ���������Ƴ�ȥ�����ݼӵ����ұ�
}

// ��λ���
void Xor(unsigned char *DatA, unsigned char *DatB, int Num) {         // �����
    int i = 0;
    for(i = 0; i < Num; i++) {
        DatA[i] = DatA[i] ^ DatB[i];                  // ���
    }
}

// ����48λ ���32λ ��Ri���
//��48bit ���ݷֳɰ��� ����˸�S�� �õ�8��ʮ����������8��ʮ������ת����32λbit��
void S_Change(unsigned char *DatOut, unsigned char *DatIn) {  // S�б任
    int i, X, Y;                                    // iΪ8��S��
    unsigned char data[8];                       //S���е�����
    for(i = 0, Y = 0, X = 0; i < 8; i++, DatIn += 6) {        // ÿִ��һ��,��������ƫ��6λ
        // ÿִ��һ��,�������ƫ��4λ
        Y = (DatIn[0] << 1) + DatIn[5];                          // af����ڼ���
        X = (DatIn[1] << 3) + (DatIn[2] << 2) + (DatIn[3] << 1) + DatIn[4]; // bcde����ڼ���
        data[i] = S_Box[i][Y][X];  //�õ�S���е�����  4bit ����
    }
    // ���ҵ��ĵ����ݻ�Ϊ������
    for(i = 0; i < 8; i++) {
        DatOut[i * 4 + 0] = (data[i] & 0x08) ? 1 : 0; //���λ
        DatOut[i * 4 + 1] = (data[i] & 0x04) ? 1 : 0;
        DatOut[i * 4 + 2] = (data[i] & 0x02) ? 1 : 0;
        DatOut[i * 4 + 3] = (data[i] & 0x01) ? 1 : 0;
    }
}

// F����
void F_Change(unsigned char *DatIn, unsigned char *DatKi) {     // F����
    unsigned char MiR[48] = { 0 };             // ����32λͨ��Eѡλ��Ϊ48λ
    TablePermute(MiR, DatIn, E_Table, 48);      //��32bit ������չ�� 48bit
    Xor(MiR, DatKi, 48);                   // ������Կ���
    S_Change(DatIn, MiR);                 // S�б任 ��48bit ���ݷֳɰ��� ����˸�S��
    TablePermute(DatIn, DatIn, P_Table, 32);   // P�û������  32bit��
}


//����Ĭ����Կ ��ȡ����ԿKi
void Set_One_DES_64bitKey(unsigned char *KeyIn) {
    int i = 0;
    unsigned char KeyBit[64] = { 0 };                // ��Կ�����ƴ洢�ռ�
    unsigned char *KiL = &KeyBit[0];    //ǰ28
    unsigned char *KiR = &KeyBit[28];  //��28��56
    ByteToBit(KeyBit, KeyIn, 8);                    // ����ԿתΪ�����ƴ���KeyBit
    TablePermute(KeyBit, KeyBit, PC1_Table, 56);      // PC1���û� 56��
    for(i = 0; i < 16; i++) {
        LoopMove(KiL, 28, Move_Table[i]);       // ǰ28λ����
        LoopMove(KiR, 28, Move_Table[i]);         // ��28λ����
        TablePermute(SubKey[i], KeyBit, PC2_Table, 48);  //�õ�48λ ����Կ
        // ��ά���� SubKey[i]Ϊÿһ����ʼ��ַ
        // ÿ��һ��λ����PC2�û��� Ki 48λ
    }
}

// ִ��DES����
void DES_Encrypt_Block(unsigned char *MesIn, unsigned char *MesOut) {
    // �ֽ����� Bin���� Hex���
    int i = 0;
    unsigned char MesBit[64] = { 0 };        // ���Ķ����ƴ洢�ռ� 64λ
    unsigned char Temp[32] = { 0 };
    unsigned char *MiL = &MesBit[0];//ǰ32λ
    unsigned char *MiR = &MesBit[32]; //  ��32λ
    ByteToBit(MesBit, MesIn, 8);                 // �����Ļ��ɶ����ƴ���MesBit
    TablePermute(MesBit, MesBit, IP_Table, 64);    // IP�û�
    for(i = 0; i < 16; i++) {                      // ����16��
        BitsCopy(Temp, MiL, 32);             // ��ʱ�洢 ���� Li-1
        BitsCopy(MiL, MiR, 32);              // �õ�Li   Li = Ri-1
        F_Change(MiR, SubKey[i]);            // F�����任  ����E��չ ���48λ ������Կ���
        Xor(MiR, Temp, 32);                  // �õ�Ri   Ri =Li-1 ��� F��Ri-1��Ki��
    }
    //���һ��Ҫ�ı�����32bitλ��   Ri��ǰ Li �ں���кϲ�   �ϲ������IP���û�
    BitsCopy(Temp, MiL, 32);   //����� 32bit ����
    BitsCopy(MesBit, MiR, 32);  //����32λ �Ƶ�ǰ��
    BitsCopy(&MesBit[32], Temp, 32);  //��ǰ32λ �Ƶ�����  �ϲ����
    TablePermute(MesBit, MesBit, IPR_Table, 64); //����IP���û�
    BitToByte(MesOut, MesBit, 8);
}

// ִ��DES����
void DES_Decode_Block(unsigned  char *MesIn, unsigned char *MesOut) {
    // Hex���� Bin���� �ֽ����
    int i = 0;
    unsigned char MesBit[64] = { 0 };         // ���Ķ����ƴ洢�ռ� 64λ
    unsigned char Temp[32] = { 0 };
    unsigned char *MiL = &MesBit[0];  //ǰ32λ
    unsigned char *MiR = &MesBit[32]; //  ��32λ
    ByteToBit(MesBit, MesIn, 8);
    TablePermute(MesBit, MesBit, IP_Table, 64);    // IP�û�
    for(i = 0; i < 16; i++) {
        BitsCopy(Temp, MiL, 32);             // ��ʱ�洢 ���� Li-1
        BitsCopy(MiL, MiR, 32);              // �õ�Li   Li = Ri-1
        F_Change(MiR, SubKey[15 - i]);        // F�����任  ����E��չ ���48λ ������Կ���
        Xor(MiR, Temp, 32);                   // �õ�Ri   Ri =Li-1 ��� F��Ri-1��Ki��
    }
    //���һ����Ҫ���ҶԻ� Ri��ǰ Li �ں���кϲ�   �ϲ������IP���û�
    BitsCopy(Temp, MiL, 32);   //����� 32bit ����
    BitsCopy(MesBit, MiR, 32);  //����32λ �Ƶ�ǰ��
    BitsCopy(&MesBit[32], Temp, 32);  //��ǰ32λ �Ƶ�����  �ϲ����
    TablePermute(MesBit, MesBit, IPR_Table, 64);
    BitToByte(MesOut, MesBit, 8);
}

/*************************************************************************************
���º������� DES�㷨 ����    ΪCBC����ģʽ �ṩ�ӿ�
һ�����ܹ��� ��
���Ŀ�0 = ������0 ��� IV ���Ľ��   ����Կ����
���Ŀ�1 = ������1 ��� ���Ŀ�0�� �Ľ�� ����Կ����

�������ܹ��� ��
1��������Կ�����Ľ���  �õ� ����  X0
2��X0 ��� ����  �õ�����
3��ǰһ�����ݵ����Ŀ� Ϊ��һ�����ݿ������
***************************************************************************************/

//ִ�е�DES����  ����һ����      8�ֽ�����
//IV_IN_OUT��    ��ʼ����������  �������  Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݵ�����Ϊ��һ�����Ŀ��������
//Mes_IN_OUT:    ��������        �������
//Key64bit:      64bit ��Կ      8�ֽ�
void One_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit) {
    unsigned char   temp[8];
    if(Key64bit == NULL) { //��Կ����Ϊ��
        return;
    }
    if(IV_IN_OUT != NULL) {
        Xor(Mes_IN_OUT, IV_IN_OUT, 8);     //�Ȱ������������������ ���   �������ȫΪ0 �򲻸ı����ĵ�ֵ
    }
    Set_One_DES_64bitKey(Key64bit);        //������Կ  ��������Կ
    DES_Encrypt_Block(Mes_IN_OUT, temp);   //ִ��DES����  ���ı����� temp ��
    if(IV_IN_OUT != NULL) {
        BitsCopy(IV_IN_OUT, temp, 8);      //�����Ŀ���������������
    }
    BitsCopy(Mes_IN_OUT, temp, 8);         //�����Ŀ������������ݻ�����
}


//ִ�е�DES����  ����һ����      8�ֽ�����
//IV_IN_OUT��    ��ʼ����������  ԭ������� Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݿ������Ϊ��һ�����Ŀ���ܺ��������
//Mes_IN_OUT:    ��������        �������
//Key64bit:      64bit ��Կ      8�ֽ�
void One_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit) {
    unsigned char   temp[8];
    if(Key64bit == NULL) { //��Կ����Ϊ��
        return;
    }
    Set_One_DES_64bitKey(Key64bit);            //������Կ  ��������Կ
    DES_Decode_Block(Mes_IN_OUT, temp);        //ִ��DES����  ���ܵĽ�������� temp ��
    if(IV_IN_OUT != NULL) {
        Xor(temp, IV_IN_OUT, 8);               //�ѽ��ܺ�Ľ�����������õ�����  �������ȫΪ0 �򲻸ı�ֵ
        BitsCopy(IV_IN_OUT, Mes_IN_OUT, 8);    //��ԭ���Ŀ������������ݻ�����
    }
    BitsCopy(Mes_IN_OUT, temp, 8);             //�����Ŀ������������ݻ�����
}




//ִ��˫DES����  ����һ����      8�ֽ����� ����˳��  ����� �ҽ��� �����
//IV_IN_OUT��    ��ʼ����������  ������� ��CBCģʽ����ʱ��ǰһ�����ݵ�����Ϊ��һ�����Ŀ��������
//Mes_IN_OUT:    ��������        �������
//Key128bit:     128bit ��Կ     16�ֽ�
void Two_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit) {
    unsigned char   temp[8];
    if(Key128bit == NULL) {
        return;
    }
    if(IV_IN_OUT != NULL) {
        Xor(Mes_IN_OUT, IV_IN_OUT, 8);          //�Ȱ������������������ ���   �������ȫΪ0 �򲻸ı����ĵ�ֵ
    }
    Set_One_DES_64bitKey(Key128bit);            //������Կ  ��������Կ    ���8�ֽ���Կ
    DES_Encrypt_Block(Mes_IN_OUT, temp);        //ִ��DES����  ���ı����� temp ��
    Set_One_DES_64bitKey(&Key128bit[8]);        //������Կ  ��������Կ    �ұ�8�ֽ���Կ
    DES_Decode_Block(temp, temp);               //ִ��DES����  ��������� temp ��
    Set_One_DES_64bitKey(Key128bit);            //������Կ  ��������Կ    ���8�ֽ���Կ
    DES_Encrypt_Block(temp, temp);              //ִ��DES����  ���ı����� temp ��
    if(IV_IN_OUT != NULL) {
        BitsCopy(IV_IN_OUT, temp, 8);           //�����Ŀ���������������
    }
    BitsCopy(Mes_IN_OUT, temp, 8);              //�����Ŀ������������ݻ�����
}


//ִ��˫DES����  ����һ����      8�ֽ����� ��������  �����  �Ҽ���   �����
//IV_IN_OUT��    ��ʼ����������  ԭ������� Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݿ������Ϊ��һ�����Ŀ���ܺ��������
//Mes_IN_OUT:    ��������        �������
//Key128bit:     128bit ��Կ     16�ֽ�
void Two_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit) {
    unsigned char   temp[8];
    if(Key128bit == NULL) { //��Կ����Ϊ��
        return;
    }
    Set_One_DES_64bitKey(Key128bit);           //������Կ  ��������Կ
    DES_Decode_Block(Mes_IN_OUT, temp);        //ִ��DES����  ���ܵĽ�������� temp ��
    Set_One_DES_64bitKey(&Key128bit[8]);       //������Կ  ��������Կ    �ұ�8�ֽ���Կ
    DES_Encrypt_Block(temp, temp);             //ִ��DES����  ���ı����� temp ��
    Set_One_DES_64bitKey(Key128bit);           //������Կ  ��������Կ
    DES_Decode_Block(temp, temp);              //ִ��DES����  ���ܵĽ�������� temp ��
    if(IV_IN_OUT != NULL) {
        Xor(temp, IV_IN_OUT, 8);               //�ѽ��ܺ�Ľ�����������õ�����  �������ȫΪ0 �򲻸ı�ֵ
        BitsCopy(IV_IN_OUT, Mes_IN_OUT, 8);    //��ԭ���Ŀ������������ݻ�����
    }
    BitsCopy(Mes_IN_OUT, temp, 8);             //�����Ŀ������������ݻ�����
}



//ִ����DES����  ����һ����      8�ֽ����� ����˳��  ����� �н��� �Ҽ���
//IV_IN_OUT��    ��ʼ����������  ������� ��CBCģʽ����ʱ��ǰһ�����ݵ�����Ϊ��һ�����Ŀ��������
//Mes_IN_OUT:    ��������        �������
//Key192bit:     192bit ��Կ     24�ֽ�
void Three_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit) {
    unsigned char   temp[8];
    if(Key192bit == NULL) {
        return;
    }
    if(IV_IN_OUT != NULL) {
        Xor(Mes_IN_OUT, IV_IN_OUT, 8);          //�Ȱ������������������ ���   �������ȫΪ0 �򲻸ı����ĵ�ֵ
    }
    Set_One_DES_64bitKey(Key192bit);            //������Կ  ��������Կ    ���8�ֽ���Կ
    DES_Encrypt_Block(Mes_IN_OUT, temp);        //ִ��DES����  ���ı����� temp ��
    Set_One_DES_64bitKey(&Key192bit[8]);        //������Կ  ��������Կ    �м�8�ֽ���Կ
    DES_Decode_Block(temp, temp);               //ִ��DES����  ��������� temp ��
    Set_One_DES_64bitKey(&Key192bit[16]);       //������Կ  ��������Կ    �ұ�8�ֽ���Կ
    DES_Encrypt_Block(temp, temp);              //ִ��DES����  ���ı����� temp ��
    if(IV_IN_OUT != NULL) {
        BitsCopy(IV_IN_OUT, temp, 8);           //�����Ŀ���������������
    }
    BitsCopy(Mes_IN_OUT, temp, 8);              //�����Ŀ������������ݻ�����
}

//ִ����DES����  ����һ����      8�ֽ����� ��������  �ҽ���  �м���   �����
//IV_IN_OUT��    ��ʼ����������  ԭ������� Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݿ������Ϊ��һ�����Ŀ���ܺ��������
//Mes_IN_OUT:    ��������        �������
//Key192bit:     192bit ��Կ     24�ֽ�
void Three_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit) {
    unsigned char   temp[8];
    if(Key192bit == NULL) { //��Կ����Ϊ��
        return;
    }
    Set_One_DES_64bitKey(&Key192bit[16]);      //������Կ  ��������Կ  �ұ�8�ֽ���Կ
    DES_Decode_Block(Mes_IN_OUT, temp);        //ִ��DES����  ���ܵĽ�������� temp ��
    Set_One_DES_64bitKey(&Key192bit[8]);       //������Կ  ��������Կ    �м�8�ֽ���Կ
    DES_Encrypt_Block(temp, temp);             //ִ��DES����  ���ı����� temp ��
    Set_One_DES_64bitKey(Key192bit);           //������Կ  ��������Կ  ���8�ֽ���Կ
    DES_Decode_Block(temp, temp);              //ִ��DES����  ���ܵĽ�������� temp ��
    if(IV_IN_OUT != NULL) {
        Xor(temp, IV_IN_OUT, 8);               //�ѽ��ܺ�Ľ�����������õ�����  �������ȫΪ0 �򲻸ı�ֵ
        BitsCopy(IV_IN_OUT, Mes_IN_OUT, 8);    //��ԭ���Ŀ������������ݻ�����
    }
    BitsCopy(Mes_IN_OUT, temp, 8);             //�����Ŀ������������ݻ�����
}

void DES_Test(void) {
    unsigned char KEY[]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char VALUE[] = {0x06, 0xCE, 0xE4, 0xF2, 0x00, 0x00, 0x00, 0x00};
    One_DES_IV_Encrypt_Block(NULL, VALUE, KEY);
    MYHEXS(DEBUG, VALUE, 8, "�������ݣ�");
    One_DES_IV_Decode_Block(NULL, VALUE, KEY);
    MYHEXS(DEBUG, VALUE, 8, "�������ݣ�");
}

