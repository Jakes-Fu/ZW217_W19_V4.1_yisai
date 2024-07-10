/****************************************************************************
** File Name:      mmi_atc.h                                               *
** Author:                                                                 *
** Date:           03/11/2004                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 MMMI_SyncTool interface                                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2004        Lin.Lin           Create
** 
****************************************************************************/
#ifndef MMI_ATC_H
#define MMI_ATC_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/
typedef unsigned char			BOOLEAN;
typedef unsigned char			uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
typedef unsigned int			uint;

typedef signed char				int8;
typedef signed short			int16;
typedef signed long int		int32;
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_AT_INDICATE             "at##"
#define MMI_SYNC_INDICATE			"MMI"
#define SYNC_TO_MMI_FINISH_SYMBOL	"\r\n"

#define MMI_SYNC_START              "READY?"
#define MMI_AT_VERSION              "ver1.00\r\nMMI\r\nOK\r\n"

#define MMI_TO_SYNC_OK_SYMBOL		"\r\nMMI\r\nOK\r\n"
#define MMI_TO_SYNC_ERROR_SYMBOL	"ERROR\r\n"

#define MMI_MAX_INDICATE_LEN         16

//module
#define MMI_SYNC_PB			        '0'
#define MMI_SYNC_SMS		        '1'
#define MMI_SYNC_RING		        '2'
#define MMI_SYNC_PICTURE		    '3'
#define MMI_SYNC_MEMO	        	'4'
#define MMI_SYNC_CALLLOG		    '5'
#define MMI_SYNC_PBGROUP  	    	'6'

//operation
#define MMI_SYNC_GETINFO		    '0'
#define MMI_SYNC_ADD		        '1'
#define MMI_SYNC_DELETE		        '2'
#define MMI_SYNC_MODIFY		        '3'
#define MMI_SYNC_GETITEM	        '4'

#define SMS_SYNC_STORAGE_SIM	    '0'
#define SMS_SYNC_STORAGE_ME	        '1'

#define SMS_SYNC_STATUS_MT	        '0'
#define SMS_SYNC_STATUS_MO	        '1'

#define PB_SYNC_NOT_IN_SIM          '0'
#define PB_SYNC_IN_SIM              '1'


//���µĺ궨������PC��MMI AT֮�䴫������ʱʹ��(��Ҫ�Ǵ����ϵ�һЩ�涨)����ͬ���ֻ��ڲ��绰����Ⱥ�顢�ճ̵�ģ��
//�еĸ��ԵĲ�������, ����MMI_SYNC_PB_NAME_MAX_LEN��ʾ����20���ֽ�������绰����¼�е�����һ������������
//����20���ֽڣ���β��������䣨ע������������Ǵ��ڵ绰���й涨��������󳤶ȣ����򣬴���������ȫ��
//phonebook
#define MMI_SYNC_PB_MAX_ENTRY_NUM           1024
#define MMI_SYNC_PB_NAME_MAX_LEN            20
#define MMI_SYNC_PB_NUM_MAX_LEN             40
#define MMI_SYNC_PB_MAX_MAIL_LEN 	        40
#define MMI_SYNC_PB_MAX_NV_PHONE_NUM	    3

//group
#define MMI_SYNC_GROUP_MAX_NUM              20
#define	MMI_SYNC_GROUP_NAME_MAX_LEN		    20

//memo
#define	MEMO_MAX_ENTRY_NUM        	        100
#define	MEMO_TEXT_MAX_LEN			        70

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _PB_HEAD_INFO_T
{
    BOOLEAN is_valid;   //TRUE���洢����Ӧ�ĵ绰����Ч; ��֮����Ч
    uint8	max_alpha_len;	 //�洢����֧�ֵĵ绰����������󳤶�
    uint8   max_num_len;    //�洢����֧�ֵĵ绰���������󳤶�
    uint8   max_email_len;
    uint16	max_record_num; //�洢���������Ա���ļ�¼��
    uint16  used_record_num; //�洢�����Ѿ�����ļ�¼��
    uint8   all_entry_id[MMI_SYNC_PB_MAX_ENTRY_NUM/8];
}PB_HEAD_INFO_T;

typedef struct _PB_NAME_T
{
    BOOLEAN is_ucs2;        //�ַ����ı�������
    uint8   alpha_len;      //alpha�����BYTE����
    uint8   alpha[MMI_SYNC_PB_NAME_MAX_LEN];      //alph string
}PB_NAME_T;


typedef struct PB_NUMBER_T
{
    uint8   number_len;     //number�����BYTE����
    uint8   number[MMI_SYNC_PB_NUM_MAX_LEN];     //�ַ���
}PB_NUMBER_T;

// �绰���е�����
typedef struct _PB_MAIL_T
{
    BOOLEAN is_ucs2;                     // �ַ����ı�������
    uint8   mail_len;                    // mail�����BYTE����
	uint8   mail[MMI_SYNC_PB_MAX_MAIL_LEN];    // �����mail��ַ
} PB_MAIL_T;

typedef struct _PB_PHONEBOOK_ENTRY_T
{
    uint16          entry_id;       //��¼��id��
    PB_NAME_T		entry_name;     //����
    PB_NUMBER_T	    number_t[MMI_SYNC_PB_MAX_NV_PHONE_NUM];    //�绰���룬����Ϊ���ˡ�סլ�͹�˾
    PB_MAIL_T	    mail;           //�����ַ
    uint8           group_id;
}PB_PHONEBOOK_ENTRY_T;

//��ý����Ϣ��������Ƭ��ͼƬ������
typedef struct 
{
    uint16 max_number;      //�������
    uint16 total_number;    //Ŀǰ����
    uint32 max_memory;      //����ڴ�
    uint32 total_memory;    //Ŀǰ�����ڴ�,������Ϣ�и�����������
}MULTIM_INFO_T;


//Group struct
//group infor header
typedef struct _GROUP_HEAD_INFO_T
{
    uint16	    max_name_len;	   		//max group name len
    uint16	    max_group_num;     		//max group num
    uint16      used_num;          		//used group num
    uint16      all_entry_id[MMI_SYNC_GROUP_MAX_NUM];
}GROUP_HEAD_INFO_T;

//record infor
typedef struct _GROUP_ENTRY_T
{
    uint16      entry_id;       			    //id
    BOOLEAN		is_fixed_group;                 //fixed group can't be deleted or modified
    BOOLEAN     is_ucs2;
    uint16      name_len;
    uint8       group_name[MMI_SYNC_GROUP_NAME_MAX_LEN]; //group name
    uint16      member_num;                     //how many members in this group
}GROUP_ENTRY_T;


//Memo struct
//memo info header
typedef struct _MEMO_HEAD_INFO_T
{
    uint16	 			max_text_len;	      		//max memo text len
    uint16	 			max_record_num;   		    //max memo record num
    uint16  			used_record_num;   		    //used memo record num
    uint16  			all_entry_id[MEMO_MAX_ENTRY_NUM];  
}MEMO_HEAD_INFO_T;

//memo time struct
typedef enum{
    MEMO_NULL,
    MEMO_MODE_ONCE,
    MEMO_MODE_EVERYDAY,
    MEMO_MODE_EVERYWEEK,
    MEMO_MODE_EVERYMONTH,
    MEMO_MODE_EVERYYEAR,	
    MEMO_MODE_NO_ALARM,
    MEMO_MODE_MAX_TYPE    
}MEMO_MODE_TYPE_E;

typedef struct _MEMO_TIME_T
{
    uint16				year;
    uint8				month;
    uint8				day;
    uint8				hour;
    uint8				minute;
}MEMO_TIME_T;

//memo text struct
typedef struct _MEMO_TEXT_T
{
    uint8               fill_in;
    BOOLEAN 			is_ucs2;        			//is ucs2 code
    uint16    			text_len;         			//text length
    uint8     			text[MEMO_TEXT_MAX_LEN];    //text string
}MEMO_TEXT_T;

//record info
typedef struct _MEMO_ENTRY_T
{
    uint16          	entry_id;       //id
    MEMO_TIME_T		    memo_time;
    uint16              memo_mode;
    MEMO_TEXT_T		    memo_text;
    
	MEMO_TIME_T         memo_start_time;
	MEMO_TIME_T         memo_end_time;
	uint8               fre;
	uint8               reserved1;
	uint16              reserved2;
	uint32              service_id;
}MEMO_ENTRY_T;

// fre�ڰ������ѵ�ʱ�����ã����û�ѡ��������ʱ��
// fre������¼����ѡ�����ܼ���freֵ������ĺ���л�����õ���
#define     MASK_B0         0x01//����
#define     MASK_B1         0x02//��һ
#define     MASK_B2         0x04
#define     MASK_B3         0x08
#define     MASK_B4         0x10    
#define     MASK_B5         0x20
#define     MASK_B6         0x40//����

#endif