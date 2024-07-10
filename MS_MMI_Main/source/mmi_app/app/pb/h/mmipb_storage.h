/*****************************************************************************
** File Name:      mmipb_storage.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with the physical device access function *
**                 �����������洢�豸�ķ���                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_STORAGE_H_
#define _MMIPB_STORAGE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmk_type.h"
#include "mmipb_common.h"
#include "mmi_custom_define.h"
#include "mmi_filemgr.h"
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
#define	MMIPB_INVALID_MAX_ENTRY_ID	    0xffff
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define MMI_ANR1_MASK             0x01
#define MMI_ANR2_MASK             0x02
#define MMI_ANR3_MASK             0x04
#define MMI_EMAIL_MASK            0x08
 //��ϵ���ֶ�����    
typedef enum
{
    MMIPB_CONTACT_NUMBER,//����
    MMIPB_CONTACT_NAME,//����
    MMIPB_CONTACT_EMAIL,//email
    //MMIPB_CONTACT_EMAIL_FREE_NUM,//email���и���
    MMIPB_CONTACT_SNE, //����
    //MMIPB_CONTACT_SNE_FREE_NUM, //�������и���
    MMIPB_SIM_EXT1, //��չ��Ϣ
}MMIPB_CONTACT_DOMAIN_E;
//contact name
typedef struct _MMIPB_INTERNAL_NAME_T
{
    BOOLEAN is_ucs2;                     // �ַ����ı�������
    uint8   alpha_len;                   // alpha�����BYTE����
	uint8   alpha[MMIPB_NAME_MAX_LEN  + 1];   // ����������鿴GSM 11.11
}MMIPB_INTERNAL_NAME_T;

// �绰���е�����
typedef struct _MMIPB_INTERNAL_MAIL_T
{
    BOOLEAN is_ucs2;					 // �Ƿ���UCS2
    uint8   mail_len;                    // mail�����BYTE����
	uint8   mail[MMIPB_MAX_MAIL_LEN + 1];    // �����mail��ַ
} MMIPB_INTERNAL_MAIL_T;

// �绰���е��Զ�������
typedef struct  
{
	BOOLEAN is_valid;							// �Ƿ���Ч
	BOOLEAN is_ucs2;							// �Ƿ���UCS2
	uint8	text_len;							// ����
	uint8	text_buf[MMIPB_MAX_CUSTOM_TEXT_LEN  + 1];
												
}MMIPB_INTERNAL_CUSTOM_TEXT_T;

// �绰���е�ְ��
typedef struct  
{
    BOOLEAN is_ucs2;                        // �ַ����ı�������
    uint8   title_len;                      // �����BYTE����
    uint8   title_text[MMIPB_MAX_TITLE_LEN + 1];// �ַ�������
}MMIPB_INTERNAL_TITLE_T;

// �绰���еĹ�˾����
typedef struct  
{
    BOOLEAN is_ucs2;                     // �ַ����ı�������
    uint8   org_len;                     // �����BYTE����
    uint8   org_text[MMIPB_MAX_ORG_LEN + 1]; // �ַ�������
}MMIPB_INTERNAL_ORG_T;

// �绰���еĹ�����ַ
typedef struct  
{
    BOOLEAN is_ucs2;                          // �ַ����ı�������
    uint8   adr_len;                          // �����BYTE����
    uint8   adr_text[MMIPB_MAX_WORKADR_LEN + 1];// �ַ�������
}MMIPB_INTERNAL_WORK_ADR_T;

// sign�洢�ṹ
typedef struct  
{
    BOOLEAN is_ucs2;                          // �ַ����ı�������
    uint8   sign_len;                          // �����BYTE����
    uint8   sign_text[MMIPB_SNS_SIGN_LEN + 1];// �ַ�������
}MMIPB_INTERNAL_SIGN_T;

// qq�洢�ṹ
typedef struct  
{
    BOOLEAN is_ucs2;                          // �ַ����ı�������
    uint8   im_len;                          // �����BYTE����
    uint8   im_buf[MMIPB_IM_MAX_LEN + 1];// �ַ�������
}MMIPB_INTERNAL_IM_T;

// birth�洢�ṹ
typedef struct  
{
    BOOLEAN is_ucs2;                          // �ַ����ı�������
    uint8   birth_len;                          // �����BYTE����
    uint8   birth_buf[MMIPB_SNS_BIRTH_LEN + 1];// �ַ�������
}MMIPB_INTERNAL_BIRTH_T;

// URL�洢�ṹ
typedef struct  
{
    BOOLEAN is_ucs2; 
    uint8   url_len;                        // �����BYTE����
    uint8   url_buf[MMIPB_URL_MAX_LEN + 1];// �ַ�������
}MMIPB_INTERNAL_URL_T;

//�ֻ���ϵ�˵Ĵ洢�ṹ
typedef struct
{
	uint16             entry_id; //��ϵ�˼�¼id
	uint16             storage_id;//��ϵ�˼�¼�洢��id����8λ��PHONEBOOK_STORAGE_E����8λ��MMIPB_STORAGE_ID_E��
#ifdef MMIPB_UID_SUPPORT
    uint32             uid;
#endif
	uint32             group;     //��ϵ��������group, (uint32��ÿһλ����һ����������)
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    uint8              im_type;//��ʱ��Ϣ���ͣ�AIM, MSN, yahoo, skype, qq, google talk
#endif
	uint8              ring_id_flag;//��ϵ���Ƿ��������������������8λΪ1��ʾΪ�̶���������7λ��ʾ�̶�������ring id�����ֵΪ1��ʾ�û������˷ǹ̶�����
	uint8              photo_id_flag;//��ϵ���Ƿ���������ͼƬ��0����ʾû�ж���
#ifdef ASP_SUPPORT
	uint8	                asp_id_flag; //asp id information:��8λ��ʾ�Ƿ���asp:0:û��,1:�У�	                                     //��7λ��ʾ�ǹ̶�id,�����û��Զ���:1:Fixed, 0:user defined;	                                     //��6λΪFixed id��ֵ 
#endif
	uint8              num_type[MMIPB_MAX_NV_PHONE_NUM];//ÿ����������ͣ�������ڶ����ͬ���͵ĺ���
	MMIPB_BCD_NUMBER_T number[MMIPB_MAX_NV_PHONE_NUM];//��ϵ�˼�¼�����к���
	MMIPB_INTERNAL_NAME_T     name; //��ϵ������
#ifdef MMIPB_MAIL_SUPPORT
	MMIPB_INTERNAL_MAIL_T      mail;//��ϵ��mail��ַ    
#endif
    //MMIPB_INTERNAL_SIGN_T  sign;   
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_INTERNAL_IM_T    imessager;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_INTERNAL_BIRTH_T birth;
	MMIPB_INTERNAL_CUSTOM_TEXT_T  memo;	//����¼
    MMIPB_INTERNAL_URL_T   url;
	#if 0
	MMIPB_INTERNAL_TITLE_T        title; //ְ��  
	MMIPB_INTERNAL_ORG_T         company; //��˾����                      
	MMIPB_INTERNAL_WORK_ADR_T  address;  //��ַ
    #endif
#endif
#ifdef BT_PBAP_SUPPORT
	BOOLEAN		is_bt_contact;  //������ϵ�˱��
#endif
}MMIPB_CONTACT_PHONE_T;


#ifdef MMIPB_MOST_USED_SUPPORT
//������ϵ�˵Ĵ洢�ṹ
typedef struct
{
	uint16             entry_id; //��ϵ�˼�¼id
	uint16             storage_id;//��ϵ�˼�¼�洢��id����8λ��PHONEBOOK_STORAGE_E����8λ��MMIPB_STORAGE_ID_E��
	uint32             used_freq; //ʹ��Ƶ��
    MMIPB_BCD_NUMBER_T number;    //ʹ�õĺ���  
}MMIPB_CONTACT_MOSTUSED_T;
#endif

//sns��ϵ�˾�������
typedef struct
{
    char       user_id[MMIPB_SNS_USERID_LEN];
    char       file_path[MMIPB_SNS_PHOTO_PATH_LEN + 1]; // ��ͷ��·��
    char       name[MMIPB_SNS_NAME_LEN + 1]; //������
    char       email[MMIPB_SNS_EMAIL_LEN + 1]; //Email ��ַ
    char       birth[MMIPB_SNS_BIRTH_LEN + 1]; //��������
    char       sign[MMIPB_SNS_SIGN_LEN + 1]; //����ǩ��
    uint16       type; // ����(SINA,TWITTER, FACEBOOK...)
}MMIPB_SQLITE_DATA_T;

//sns��ϵ�˵Ĵ洢�ṹ
typedef struct
{
    uint32 nRow;
    uint16 uiNum;      //��ѯ��ֹ
    uint16 uiOffset;    // ��¼id
    MMIPB_SQLITE_DATA_T search_result;
}MMIPB_SNS_CONTACT_T;

// ���õ������Ϣ����������������ͼ�������������NV�б����ֵӦ�ñ���һ�¡�
typedef struct _MMIPB_GROUP_T
{
    BOOLEAN             is_valid;   // TRUE, ��Ч
    uint8	            group_id;   // Ⱥ���id��
    uint8	            ring_id_flag;   // Ⱥ���id��
    MMIPB_NAME_T        group_name; // Ⱥ�������
} MMIPB_GROUP_T;

typedef struct _MMIPB_STORAGE_INFO_T
{
    BOOLEAN is_valid;           // TRUE���洢����Ӧ�ĵ绰����Ч
	uint8	max_alpha_len;		// �洢����֧����ϵ����������󳤶�
#ifdef MMIPB_MAIL_SUPPORT
    uint8   max_email_alpha_len;//�洢����֧����ϵ��email����󳤶�
#endif
#ifdef CMCC_VCARD_SUPPORT
    uint8   max_sne_alpha_len;  //�洢����֧�ֵ���ϵ�˱�������󳤶�
#endif
    uint8   max_num_len;        // �洢����֧�ֵĵ绰���������󳤶�
	uint16	max_record_num;		// �洢���������Ա���ļ�¼����
    uint16  used_record_num;    // �洢�����Ѿ�����ļ�¼����
} MMIPB_STORAGE_INFO_T;

//��ϵ�˸����ֶε�ʹ�������Ϣ
typedef struct _MMIPB_FIELD_USAGE_INOF_T
{
	BOOLEAN	number_free_flag[MMIPB_MAX_NV_PHONE_NUM]; //����ռ��Ƿ���б�־��TRUE:�����ͼ�¼�п��� 
    BOOLEAN photo_free_flag;    //�Ƿ�������������ͼƬ
    BOOLEAN group_free_falg;     //�Ƿ��������÷���
    BOOLEAN ring_free_falg;     //�Ƿ�����������������
#ifdef ASP_SUPPORT
    BOOLEAN asp_free_falg;      //�Ƿ����������Զ���¼��
#endif
#ifdef MMIPB_MAIL_SUPPORT
    BOOLEAN mail_free_flag;     //mail�Ƿ���б�־��TRUE: �����ͼ�¼mail�п���
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    BOOLEAN im_free_flag;       //�Ƿ�֧�ּ�ʱ��Ϣ
#endif
#ifdef CMCC_VCARD_SUPPORT
    BOOLEAN sne_free_flag;      //�����Ƿ���б�־:TRUE:�����ͼ�¼�����п���
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    BOOLEAN memo_free_flag;     //�����ֶ�¼�Ƿ����
    BOOLEAN birth_free_flag;    //�����ֶ��Ƿ����
    BOOLEAN url_free_flag;     //��ҳ�ֶ��Ƿ����
#endif
#ifdef SNS_SUPPORT 
    BOOLEAN sign_free_flag;    //ǩ���ֶ��Ƿ����
#endif
} MMIPB_FIELD_USAGE_INOF_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : �����ϵ�˶�Ӧ�ֶε����uid
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetMaxUid(void);

/*****************************************************************************/
//  Description : set��ϵ�˶�Ӧ�ֶε����uid
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_SetMaxUid(uint32 uid);
#endif


 #ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:starmer.sun
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSnsAccountLogIn(void);
/*****************************************************************************/
//  Description : 
//  Author: MARY.XIAO
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GotoSNSActiveWin(uint8  group_id,        //in    
                                 uint16 contact_id);           //in : index  
/*****************************************************************************/
//  Description : ����sns��ϵ����Ϣ
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsContactByGroupId(uint8 group_id);
#endif
/*****************************************************************************/
//  Description : �����ϵ�˶�Ӧ�ֶε����alpha����
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetContactMaxAlphaLen(PHONEBOOK_STORAGE_E storage, 
                                  MN_DUAL_SYS_E dual_sys,
                                  MMIPB_CONTACT_DOMAIN_E feild//��ϵ����
                                  );

/*****************************************************************************/
//  Description : �����ϵ�˶�Ӧ�ֶε����wstr����
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetContactMaxWstrLen(PHONEBOOK_STORAGE_E storage,// 
                                  MN_DUAL_SYS_E dual_sys,//��
                                  MMIPB_CONTACT_DOMAIN_E feild//��ϵ����
                                 );


/*****************************************************************************/
//  Description : ��õ绰���Ĵ�����Ϣ
//  Global resource dependence : s_mmipb_nv_info_t
//  Author: 
//  Note: 
/*****************************************************************************/
MMIPB_STORAGE_INFO_T MMIPB_GetContactStorage //RETURN: �绰����Ϣ
                                            (
                                             MN_DUAL_SYS_E       dual_sys,
                                             PHONEBOOK_STORAGE_E storage             //IN: �洢������
                                             );

/*****************************************************************************/
//  Description : ��õ绰���Ŀռ���Ϣ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  uint16 MMIPB_GetStorageSpace //RETURN: �绰����Ϣ
(
 MN_DUAL_SYS_E       dual_sys,
 PHONEBOOK_STORAGE_E storage             //IN: �洢������
 );

/*****************************************************************************/
//  Description : read sns contact
//  Global resource dependence : 
//  Author:maryxiao
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadSnsContact(
                                            uint16 sns_type,
                                            uint16  index,
                                            MMIPB_SNS_CONTACT_T* data_ptr//[OUT]
                                            );
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : Open sns database
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateSNSDBHandle(
                                        uint16 sns_type                                           
                                        );

/*****************************************************************************/
//  Description : close sns database
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseSNSDBHandle(void);


/*****************************************************************************/
//  Description : ���sns��ϵ�˸���
//  Author: 
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMIPB_GetSNSContactsCount(uint16 sns_type);

/*****************************************************************************/
//  Description : ���sns�˻���Ϣ(email)�ĳ���
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC uint32  MMIPB_GetSNSAccountEmailLen(void);

/*****************************************************************************/
//  Description : ���sns�˻���Ϣ(email)
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSNSAccountEmail(uint8 group_id,                //in 
                                                                                    MMI_STRING_T* email);     //out

/*****************************************************************************/
//  Description : ���sns�˻��������Ϣ
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetSNSActiveInfo(uint8  group_id,        //in    
                                 uint16 contact_id,           //in : index
                                 MMIPB_NAME_T name,    //in 
                                 void **activity_ptr);     //out
                                 
/*****************************************************************************/
//  Description : ����sns��ϵ����Ϣ
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsContact(void);                                 
#endif                                                                                    
/*****************************************************************************/
//  Description : ����ֻ���ϵ�˶�Ӧ�ֶεĿռ�ʹ�����
//  Global resource dependence :
//  Author: 
//  Note: ÿ���ֶ���Ӧ����пռ䣬����ΪTRUE������ΪFALSE
//        ͨ���ú������Ի�ȡ���������ֶΣ�mail�ֶΣ������ֶε�ʹ�����
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetPhoneContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : ���SNS��ϵ�˶�Ӧ�ֶεĿռ�ʹ�����
//  Global resource dependence :
//  Author: 
//  Note: ÿ���ֶ���Ӧ����пռ䣬����ΪTRUE������ΪFALSE
//        ͨ���ú������Ի�ȡ���������ֶΣ�mail�ֶΣ������ֶε�ʹ�����
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetSNSContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ��ó�����ϵ�˶�Ӧ�ֶεĿռ�ʹ�����
//  Global resource dependence :
//  Author: 
//  Note: ÿ���ֶ���Ӧ����пռ䣬����ΪTRUE������ΪFALSE
//        ͨ���ú������Ի�ȡ���������ֶΣ�mail�ֶΣ������ֶε�ʹ�����
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetMostUsedContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );
#endif
/*****************************************************************************/
//  Description : ���sim����ϵ�˶�Ӧ�ֶεĿռ�ʹ�����
//  Global resource dependence :
//  Author: 
//  Note: ÿ���ֶ���Ӧ����пռ䣬����ΪTRUE������ΪFALSE
//        ͨ���ú������Ի�ȡ���������ֶΣ�mail�ֶΣ������ֶε�ʹ�����
//  Return:���contact_idΪ0����ʾ����������һ���µ�id
/*****************************************************************************/
PUBLIC void MMIPB_GetSIMContactFieldUageInfo(   
                                 MN_DUAL_SYS_E dual_sys,
                                 PHONEBOOK_STORAGE_E storage,
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );

/*****************************************************************************/
//  Description : read phone contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadPhoneContact(uint16 entry_id,
                                            uint16 storage_id,
                                            MMIPB_CONTACT_PHONE_T *contact_ptr//[OUT]
                                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : read mostused contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContact(
                                               uint16 entry_id,
                                               uint16 storage_id,
                                               MMIPB_CONTACT_MOSTUSED_T *contact_ptr//[OUT]
                                               );
#endif
/*****************************************************************************/
//  Description : read sim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadSIMContact(
                                          uint16 entry_id,
                                          MN_DUAL_SYS_E dual_sys,
                                          PHONEBOOK_STORAGE_E storage,
                                          PHONEBOOK_ENTRY_T   *contact_ptr//[OUT]
                                          );


#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : read usim additional contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAdditionalContact(
                                          uint16 entry_id,
                                          MN_DUAL_SYS_E dual_sys
                                          );

/*****************************************************************************/
//  Description : ��ȡUSIM ��չ����������ַ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAASString (
                           MN_DUAL_SYS_E dual_sys,//[IN]
                           uint8         aas_index,//[IN]
                           MMIPB_NAME_T *name_ptr//[OUT]
                           );

/*****************************************************************************/
//  Description : дUSIM ��չ����������ַ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:��������3G PS��û��֧�֣���ʱ����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteUSIMAASString(
                           MN_DUAL_SYS_E dual_sys,//[IN]
                           uint8         aas_index,//[IN]
                           MMIPB_NAME_T *name_ptr//[OUT]
                           );

/*****************************************************************************/
//  Description : ��ȡusim ֧�ֵ���չ��������͸���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetUSIMAASCount(
                           MN_DUAL_SYS_E dual_sys
                           );
/*****************************************************************************/
//  Description : ��ȡusim ֧�ֵ���չ����Ŀ��ø���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetUSIMFreeANRCount(
								   MN_DUAL_SYS_E dual_sys
                           );

#endif

/*****************************************************************************/
//  Description : read group information
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadGroup(
                                     uint16      group_id,
                                     MMIPB_GROUP_T *group_ptr//[OUT]
                                    );
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:starmer.sun
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsSnsAccountLogIn(uint8 group_id);
#endif
/*****************************************************************************/
//  Description : write phone contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WritePhoneContact(uint16 entry_id,
                                             uint16 storage_id,
                                             MMIPB_CONTACT_PHONE_T *contact_ptr//[IN]
                                             );

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : write mostused contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteMostUsedContact(uint16 entry_id,
                                                uint16 storage_id,
                                                MMIPB_CONTACT_MOSTUSED_T *contact_ptr//[IN]
                                                );

#endif
/*****************************************************************************/
//  Description : write sim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteSIMContact(										  
                                           uint16 entry_id,
                                           MN_DUAL_SYS_E dual_sys,
                                           PHONEBOOK_STORAGE_E storage,
                                           PHONEBOOK_ENTRY_T  *contact_ptr//[IN]
                                           );




#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : write usim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteUSIMContact(										  
                                           uint16 entry_id,
                                           MN_DUAL_SYS_E dual_sys,
                                           PHONEBOOK_STORAGE_E storage,
			                               BOOLEAN	anr_flag[MN_MAX_USIM_ANR_NUM],
			                               BOOLEAN	email_flag,
			                               BOOLEAN  sne_flag,
			                               BOOLEAN	adn_flag,
                                           PHONEBOOK_ENTRY_T  *contact_ptr,//[IN]
                                           PHONEBOOK_ADDITIONAL_ENTRY_T	*additional_entry_ptr//[IN]
                                           );
#endif
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : write vcard contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteVcardContact(										  
                                             MMIFILE_HANDLE file_handle,
                                             uint32 data_len,
                                             void* data_ptr//[IN]
                                             );
#endif
/*****************************************************************************/
//  Description : write group information
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteGroup(
                                     uint16      group_id,
                                     MMIPB_GROUP_T *group_ptr//[IN]
                                    );

/*****************************************************************************/
//  Description : delete phone contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeletePhoneContact(uint16 entry_id,
                                             uint16 storage_id
                                             );

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : delete most used contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteMostContact(uint16 entry_id,
                                             uint16 storage_id
                                             );
#endif
/*****************************************************************************/
//  Description : delete sim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteSIMContact( uint16 entry_id,
                                             MN_DUAL_SYS_E dual_sys,
                                             PHONEBOOK_STORAGE_E storage
                                             );

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : delete usim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteUSIMContact( uint16 entry_id,
                                             MN_DUAL_SYS_E dual_sys,
                                             PHONEBOOK_STORAGE_E storage,
                                             BOOLEAN anr_flag[MNSIM_ANR_MAX_NUM],
                                             BOOLEAN email_flag,
                                             BOOLEAN sne_flag
                                             );
#endif

/*****************************************************************************/
//  Description : delete group information
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteGroup(
                                     uint16      group_id
                                    );

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : create tcard file mutex
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateTcardHandleMutex(void);

/*****************************************************************************/
//  Description : create tcard file mutex
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitTcardFileHandle(void);

/*****************************************************************************/
//  Description : tcard file exist
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsTcardFileExist(void);


#endif /* TCARD_PB_SUPPORT */

#ifdef   __cplusplus
    }
#endif

#endif
