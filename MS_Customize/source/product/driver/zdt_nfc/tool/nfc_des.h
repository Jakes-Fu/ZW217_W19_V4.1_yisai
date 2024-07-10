#ifndef _DES_H
#define _DES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

//ִ�е�DES����  ����һ����      8�ֽ�����
//IV_IN_OUT��    ��ʼ����������  �������  Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݵ�����Ϊ��һ�����Ŀ��������
//Mes_IN_OUT:    ��������        �������
//Key64bit:      64bit ��Կ      8�ֽ�
void One_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit);
//ִ�е�DES����  ����һ����      8�ֽ�����
//IV_IN_OUT��    ��ʼ����������  ԭ������� Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݿ������Ϊ��һ�����Ŀ���ܺ��������
//Mes_IN_OUT:    ��������        �������
//Key64bit:      64bit ��Կ      8�ֽ�
void One_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key64bit);
//ִ��˫DES����  ����һ����      8�ֽ����� ����˳��  ����� �ҽ��� �����
//IV_IN_OUT��    ��ʼ����������  ������� ��CBCģʽ����ʱ��ǰһ�����ݵ�����Ϊ��һ�����Ŀ��������
//Mes_IN_OUT:    ��������        �������
//Key128bit:     128bit ��Կ     16�ֽ�
void Two_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit);
//ִ��˫DES����  ����һ����      8�ֽ����� ��������  �����  �Ҽ���   �����
//IV_IN_OUT��    ��ʼ����������  ԭ������� Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݿ������Ϊ��һ�����Ŀ���ܺ��������
//Mes_IN_OUT:    ��������        �������
//Key128bit:     128bit ��Կ     16�ֽ�
void Two_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key128bit);
//ִ����DES����  ����һ����      8�ֽ����� ����˳��  ����� �н��� �Ҽ���
//IV_IN_OUT��    ��ʼ����������  ������� ��CBCģʽ����ʱ��ǰһ�����ݵ�����Ϊ��һ�����Ŀ��������
//Mes_IN_OUT:    ��������        �������
//Key192bit:     192bit ��Կ     24�ֽ�
void Three_DES_IV_Encrypt_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit);
//ִ����DES����  ����һ����      8�ֽ����� ��������  �����  �м���   �ҽ���
//IV_IN_OUT��    ��ʼ����������  ԭ������� Ϊ��ʱ��������ֵ��CBCģʽ����ʱ��ǰһ�����ݿ������Ϊ��һ�����Ŀ���ܺ��������
//Mes_IN_OUT:    ��������        �������
//Key192bit:     192bit ��Կ     24�ֽ�
void Three_DES_IV_Decode_Block(unsigned char *IV_IN_OUT, unsigned char *Mes_IN_OUT, unsigned char *Key192bit);

void DES_Test(void) ;

#ifdef __cplusplus
}
#endif
#endif
