/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_feature.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef  _COMM_FEATURE_H_
#define _COMM_FEATURE_H_

//�Ƿ�֧������OMA download tag Ŀǰ��֧��
//#define OMA_DOWNLOAD_FULL_SUPPORT

//
#define COMM_DEBUG_MODE

//֧�ֽ�log��Ϣ���浽�ļ���
#define COMM_LOG_IN_FILE_SUPPORT

//֧��IMAP4
//#define MAIL_SUPPORT_IMAP4

//�Ƿ�ʹ��DEBUGģʽ�ڴ�
//#define MAIL_MEMORY_DEBUG_MODE

//֧��CAPA
//#define MAIL_SUPPORT_POP3_CAPA


//cmpop Э����չ
#define MAIL_SUPPORT_CMPOP 

//cmsmtp Э����չ
#define MAIL_SUPPORT_CMSMTP 

//#define MAIL_PUSH_SETTING_TEST

//#define MAIL_DEBUG_SAVE_FILE
#ifdef WIN32
//ģ�����Ƿ����windows socket�ӿ�
//#define MAIL_SUPPORT_WIN_SOCKET
#endif

//���ع���feature
#define MAIL_FLDR_MANAGER
#define MAIL_FLDR_LIST_CHECK
#define MAIL_FLDR_FILE_ERROR  //���彫�ļ����������Ϊһ��MAIL_FILE_ERROR
#define MAIL_FLDR_CUT_SETTING //���ع���ģ��ȥ������
#define MAIL_FLDR_UNKNOWN_MSG //��Ҫȷ�ϵĽ�����Ϣ
#define MAIL_FLDR_CUT_MIME //���ع���ģ����ʱȥ��mime�ӿ�
#define MAIL_FLDR_CUT_OTHER //����ǰ�ڱ��뿳������ȷ�Ľӿ�
//#define MAIL_FLDR_FOR_QO7 //������������,���Ӷ��ʻ�����
//#define MAIL_FLDR_DEBUG //��������ǰ�ڵ�����
#endif

