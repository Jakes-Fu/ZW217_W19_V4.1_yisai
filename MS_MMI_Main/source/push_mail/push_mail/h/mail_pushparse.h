/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_pushparse.h
 *	Author		: 
 *	Description	: �ֻ��������push����
 *	Vesion		: 1.0        
 *	Date			: 2010-5-12  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-5-12      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/
 
#ifndef _MAIL_PUSHPARSE_H_
#define _MAIL_PUSHPARSE_H_

#include "comm_feature.h"



#include "mmimms_push.h"


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct
{
    uint16 pro_r;//Э��İ汾
    uint8  record_type;//���ݼ�¼����
    uint16 ssl;//ssl��ֵ��0x0000 ��ʾ��ʾ����SSL0x0100 ��ʾ��SSL 
    uint32 length;//��ָ�Ӹ��ֶκ�����ݼ�¼�ĳ��ȡ������������ֶεĳ��ȣ�
    uint16 receive_length;//�����ʼ���󳤶�
    uint32 send_ip_add;//���ͷ�����IP��ַ
    uint16 send_port;//���ͷ������˿�
    uint32 receive_ip_add;//���շ�����IP��ַ
    uint16 receive_port;//���շ������˿�
    uint8 user_name[64];//�����˺�
    uint8 user_password[16];//�����˺ŵ�����
    uint8 sms_number[16];//���ź���
    uint8 apn[16];//APN
}MMIMAIL_SETTING_INFO_T;//���ʻ�������Ϣ�Ľṹ��

typedef struct
{
    uint8 cid[6];//�ʻ����ڷ�������CID
    uint32 send_ip_addr;//���ͷ�����IP��ַ
    uint16 send_port;//���ͷ������˿�
    uint32 receive_ip_addr;//���շ�����IP��ַ
    uint16 receive_port;//���շ������˿�
    uint32 proxy_ip;//����IP��ַ
    uint16 proxy_port;//����˿�
    uint8 ssl;//ssl
    uint8 user_name[64];//�����˺�
    uint8 user_password[16];//�����ʻ�������
    uint8 apn[16];//apn
    uint8 smsnumber[16];//���ź���
}MMIMAIL_MORE_SETTING_CONTENT_DATA_INFO_T;//���ʻ�������Ϣ�е�setting������

typedef struct
{
    uint16 setting_info_length;//������Ϣ�ĳ���
    MMIMAIL_MORE_SETTING_CONTENT_DATA_INFO_T setting_data;//������Ϣ������
}MMIMAIL_MORE_SETTING_CONTENT_INFO_T;//���ʻ�������Ϣ�е�setting�Ľṹ�壬���ݺͳ���

typedef struct
{
    uint16 pro_r;//Э��汾
    uint8  record_type;//���ݼ�¼������
    uint16 record_length;//���ݼ�¼���ܳ���
    uint8  phone_number[6];//�ֻ�����
    uint8  account_num;//�ʻ���
    MMIMAIL_MORE_SETTING_CONTENT_INFO_T setting_info[4];//���ʻ�������Ϣ��setting��Ϣ������ʻ�4��
}MMIMAIL_MORE_SETTING_INFO_T;//���ʻ�������Ϣ�Ľṹ��

typedef struct
{
    uint8 charset;//���뷽ʽ
    uint8 emn_attributes;//emn ����
    uint8 mail_attributes;//�ʼ�����
    uint8 user_name[64];//�û���
    uint8 user_password[16];//�û�����
    uint8 timestamp_len;//ʱ����ĳ���
    uint8 timestamp[10];//ʱ���
}MMIMAIL_EMN_INFO_T;//�ʼ�֪ͨ�Ľṹ��

typedef struct
{
    uint16 pro_r;//Э��汾��
    uint8  record_type;//���ݼ�¼����
    uint32 length;//���ݼ�¼�ĳ���
    uint8 md5[16];//md5��
    uint8 timestamp[20];//ʱ���
    uint8 user_name[64];//�����˺�  
}MMIMAIL_BOMB_INFO_T;//ը����Ϣ�Ľṹ��

typedef struct
{
    uint16 pro_r;//Э��汾
    uint8 record_type;//���ݼ�¼����
    uint32 length;//���ݼ�¼����
    char * data;//�����ֶ�1������
}MMIMAIL_INFOREMIND_INFO_T;//�ʷ���Ϣ�Ľṹ��



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*==========================================================
 * author		: sunlu      
 * function 	: MMIMAIL_IsPushMailForPort
 * decr		: �ж��ǲ���pushmail����Ϣ�����ж϶˿ں�
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/15/2010 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_IsPushMailForPort(uint8 *data_head, uint8 length);

/*==========================================================
 * author		: sunlu      
 * function 	: MMIMAIL_IsPushMailForContent
 * decr		: �ж��ǲ���pushmail����Ϣ(setting\emn)
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/6/2010 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_IsPushMailForContent(uint8 * user_valid_data_arr,uint16 length);


/*==========================================================
 * author		: sunlu      
 * function 	: mail_strnistr
 * decr		: �����ж���str���ǲ��ǰ���sstr
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/6/2010 
 ==========================================================*/
char* mail_strnistr(const char* str,const int str_len, const char* sstr);


/*==========================================================
 * author		: sunlu      
 * function 	: mail_ToUpperCase
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  5/6/2010 
 ==========================================================*/
char mail_ToUpperCase(char ch);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif



#endif // _MAIL_PUSHPARSE_H_

