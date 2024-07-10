/*****************************************************************************
** File Name:      mmipb_adapt.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with standard data format and specific    *
                   physical device data format conversation*
**                 ����ļ���Ҫ�Ǵ���PB�ı�׼���ݸ�ʽ���ض������豸�����ݸ�ʽ
**                 ֮���ת��               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_ADAPT_H_
#define _MMIPB_ADAPT_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_storage.h"
#include "mmi_filemgr.h"
#include "mmi_image.h"
#include "mn_type.h"
#include "mmi_custom_define.h"
#include "mmipb_nv.h"
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
#define MMIPB_GET_STORAGE(storage_id)  (storage_id & 0xFF)
#define MMIPB_GET_CONTACT_TYPE(storage_id)  (storage_id >> 8)
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
 
 // �绰�����Զ���ͼƬ,������󳤶�
#define MMIPB_MAX_CUSTOM_RING_PATH_LEN		(MMIFILE_FULL_PATH_MAX_LEN)	// ע��NV�ռ�����,��Ҫ̫��
#define MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN		(MMIFILE_FULL_PATH_MAX_LEN)
        
#define MMIPB_MAX_CUSTOM_RING_FILE_SIZE		(50 * 1024)
#define MMIPB_MAX_CUSTOM_PHOTO_FILE_SIZE	(50 * 1024)
        
        
//@maggie modify for 6800A P

// �ַ�������󳤶�
#define MMIPB_MAX_STRING_LEN		(/*lint -save -e506 */MAX((MMIPB_NAME_MAX_LEN << 1), 100) /*lint -restore */)
#define MMIPB_PHOTO_ID_EXIST           (uint8)0x80
#define MMIPB_PHOTO_ID_CLEAN           (uint8)0x7F
#define MMIPB_RING_ID_EXIST            (uint8)0x40
#define MMIPB_RING_ID_CLEAN            (uint8)0xBF
#define MMIPB_FIXED_RING_FLAG          (uint8)0x20
#define MMIPB_FIXED_RING_FLAG_CLEAN    (uint8)0xDF

#define MMIPB_FIXED_RING_ID_MASK       (uint8)0x0F
#define MMIPB_FIXED_RING_ID_CLEAN      (uint8)0xF0

#define MMIPB_ASP_ID_EXIST             (uint8)0x80
//#define MMIPB_ASP_ID_CLEAN             (uint8)0xEF
#define MMIPB_ASP_FIX_ID_EXIST         (uint8)0x40
//#define MMIPB_ASP_FIX_ID_CLEAN         (uint8)0xE0
#define MMIPB_ASP_FIX_ID_MASK          (uint8) 0x1F

#define MMIPB_DEFAULT_NPI			MN_NUM_PLAN_ISDN_TELE


#define MMIPB_NAME_FULL_CHAR        0x1 //ȫƴ���룬��֮��û�м������ȫ�ַ��Ƚ�
#define MMIPB_NAME_HEAD_CHAR        0x2 //�ַ�ƥ�䣺����ĸ�Ƚ�
#define MMIPB_NAME_FULL_CHAR_SPACER 0x4 //ȫƴ���룬��֮���м����
typedef uint8 MMIPB_NAME_STRING_TYPE_T1;//�����ַ�������
#ifdef MMIPB_COVERT_LETTER_SUPPORT
#define MMIPB_MAP_KEY 9
#define MMIPB_MIN_MAP_KEY 0x0066
#define MMIPB_MAX_MAP_KEY 0x03C9
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#ifdef MMIPB_COVERT_LETTER_SUPPORT
typedef struct
{
    wchar  map_key;                             
    wchar  raw_key_code[MMIPB_MAP_KEY+1];  
} MMIPB_MAP_KEY_T; 
#endif


// �绰���е��Զ�������
typedef struct 
{
	BOOLEAN	is_valid;							// �Ƿ���Ч
	//BOOLEAN is_fixed_ring;						// �̶�����,������������
	//uint8	fixed_ring_id;						// ����ǹ̶�����,����ID
	uint32	file_name_len;						// �������������,����������·��������
	wchar	file_name[MMIFILE_FULL_PATH_MAX_LEN +1];   //ע���������
												// �������������,����������·��
	uint32	file_size;							// �������������,�����������ļ���С
	uint32	ring_type;							// �������������,��������������
}MMIPB_CUSTOM_RING_T;

// �绰���е��Զ���ͼƬ
typedef struct 
{
	BOOLEAN is_valid;							// �Ƿ���Ч
	wchar	file_name[MMIFILE_FULL_PATH_MAX_LEN +1];  //ע���������
	uint32	file_name_len;						// ����ͼƬ·������
												// ����ͼƬ��·��
	uint32	file_size;							// ����ͼƬ�ļ���С
	uint32	photo_type;							// ����ͼƬ������
}MMIPB_CUSTOM_PHOTO_T;

//�洢����
#define MMIPB_STORAGE_FLASH        0x0 //Vcard
#define MMIPB_STORAGE_PHONE        0x1 //�ֻ���¼
#define MMIPB_STORAGE_SNS          0x2 //SNS��ϵ��
#define MMIPB_STORAGE_SIM1         0x3 //SIM1��¼
#define MMIPB_STORAGE_SIM2         0x4 //SIM2��¼
#define MMIPB_STORAGE_SIM3         0x5 //SIM3��¼
#define MMIPB_STORAGE_SIM4         0x6 //SIM4��¼
#define MMIPB_STORAGE_MAX          0x7 //Storage����
//�ֻ���¼����
#define MMIPB_NORMAL_PHONE_CONTACT   0x0//һ���ֻ���ϵ��
#ifdef MMIPB_MOST_USED_SUPPORT  
#define MMIPB_MOSTUSED_PHONE_CONTACT 0x1//������ϵ��
#endif
//��ʱ��Ϣ����
#define MMIPB_IM_QQ          0x00 //qq
#define MMIPB_IM_MSN         0x01 //MSG, windows live
#define MMIPB_IM_GOOGLE_TALK 0x02 //google talk
#define MMIPB_IM_YAHOO       0x03 //yahoo
#define MMIPB_IM_SKYPE       0x04 //skype
#define MMIPB_IM_AIM         0x05 //AIM
typedef uint8 MMIPB_IM_TYPE;//��ʱ��Ϣ����

// �绰���е�����
typedef struct _MMIPB_MAIL_T
{
    uint16   wstr_len;                    
	wchar   wstr[MMIPB_MAX_MAIL_LEN + 1];   
} MMIPB_MAIL_T;

// URL�洢�ṹ
typedef struct  
{
    uint16   wstr_len;                        // �����BYTE����
    wchar   wstr[MMIPB_URL_MAX_LEN + 1];// �ַ�������
}MMIPB_URL_T;

typedef struct _MMIPB_IM_T
{
    uint16   wstr_len;                    
	wchar   wstr[MMIPB_IM_MAX_LEN + 1];   
} MMIPB_IM_T;

typedef struct _MMIPB_BIRTH_T
{
    uint16   wstr_len;                    
	wchar   wstr[MMIPB_SNS_BIRTH_LEN + 1];   
} MMIPB_BIRTH_T;

// �绰���е��Զ�������
typedef struct  
{
	uint16	wstr_len;							// ����
	wchar	wstr[MMIPB_MAX_CUSTOM_TEXT_LEN + 1];
												
}MMIPB_CUSTOM_TEXT_T;


// �绰���е�ְ��
typedef struct  
{
    uint16   wstr_len;                      
    wchar   wstr[MMIPB_MAX_TITLE_LEN + 1];
}MMIPB_TITLE_T;

// �绰���еĹ�˾����
typedef struct  
{
    uint16   wstr_len;                     
    wchar   wstr[MMIPB_MAX_ORG_LEN + 1]; 
}MMIPB_ORG_T;

//��Ϣ�ļ�����
typedef enum
{
    MMIPB_PHOTO_FILE, //ͼƬ�ļ���Ϣ
    MMIPB_RING_FILE,  //�����ļ���Ϣ
    MMIPB_ASP_FILE,  //�Զ���¼���ļ�
    MMIPB_FILE_TYPE_MAX
}MMIPB_FILE_TYPE;

// �绰���еĹ�����ַ
typedef struct  
{
    uint16   wstr_len;                          
    wchar   wstr[MMIPB_MAX_WORKADR_LEN + 1];
}MMIPB_WORK_ADR_T;

// ��ϵ�˵�ǩ��
typedef struct  
{
    uint16   wstr_len;                          
    wchar   wstr[MMIPB_SNS_SIGN_LEN + 1];
}MMIPB_SIGN_ADR_T;

typedef struct
{
    uint16            contact_id; //��¼ID
    uint16            storage_id; //��¼�洢��ID
#ifdef MMIPB_UID_SUPPORT
    uint32            uid;
#endif
    union {
        uint32            group; //��¼��Ⱥ�飺û��һλ����һ�����飬�������group idΪ1��group�ĵ�2λΪ1���������ڶ��group
        uint32            used_freq;//Ϊ������ϵ�˱��ʹ��Ƶ��
    }union_t;
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_IM_TYPE     im_type;//��ʱ��Ϣ���ͣ�AIM, MSN, yahoo, skype, qq, google talk
#endif
    uint8             ring_id_flag; //��ϵ�˵��������
    uint8             photo_id_flag;//��ϵ��ͷ����
#ifdef ASP_SUPPORT
    uint8             asp_id_flag; //��ϵ���Զ���¼����
#endif
    BOOLEAN		      is_read_ext; //��չ��Ϣ�Ƿ��Ѿ���ʼ��
    uint8             num_type[MMIPB_MAX_NV_PHONE_NUM];//��ϵ�˸������������
    MMIPB_BCD_NUMBER_T number[MMIPB_MAX_NV_PHONE_NUM];//��ϵ�˺�������
    MMIPB_NAME_T     name;  //��ϵ������
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_MAIL_T      mail; //��ϵ��mail
#endif
#ifdef SNS_SUPPORT 
    MMIPB_SIGN_ADR_T  sign;//ǩ��
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_IM_T     imessager;  //qq,msn and so on
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_BIRTH_T     birth;//����
    MMIPB_URL_T       url;//��վ
    MMIPB_CUSTOM_TEXT_T  memo;//����¼
#endif
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_NAME_T        alias;//�ǳ�	
#endif
#ifdef BT_PBAP_SUPPORT
	BOOLEAN		is_bt_contact;  //������ϵ�˱��
#endif
}MMIPB_CONTACT_T;

//���ٲ��ҵ��ַ���Ϣ
typedef struct _MMIPB_NAME_LETTERS_T
{
    uint16  full_letters_len;                   //ȫƴ������
    wchar   full_letters[MMIPB_NAME_MAX_LEN*6 + 1];   // ȫƴ��
    uint16  head_letters_len;                   // ���ַ�������
    wchar   head_letters[MMIPB_NAME_MAX_LEN +1];   //���ַ��� 
} MMIPB_NAME_LETTERS_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ��ͼƬid��Ϣ�ļ��ж�ȡͼƬ�����buffer
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadPhotoFile(
                                    uint16 entry_id,//IN
                                    uint16 storage_id, //IN:
                                    MMIPB_CUSTOM_PHOTO_T *photo_ptr//OUT
                                  );


/*****************************************************************************/
//  Description : ������id��Ϣ�ļ��ж�ȡ����id��·����Ϣ  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadRingFile(
                                    uint16 entry_id,//IN
                                    uint16 storage_id, //IN:
                                    MMIPB_CUSTOM_RING_T *ring_ptr//OUT
                                   );


/*****************************************************************************/
//  Description : ��photo_ptr���浽����group_id��entry_idӳ��ɵ�ͼƬid��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_SaveASPInfoByStorageId(
                                            uint16 entry_id,
                                            uint16 storage_id, //IN:
                                            const MMIPB_CUSTOM_RING_T *asp_ptr//IN
                                            );
/*****************************************************************************/
//  Description : ��ȡ��ϵ�˿������ļ��ģ�������ͼƬ����asp���ݽ�buffer 
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadASPFile(
                                uint16 entry_id,//IN
                                uint16 storage_id, //IN:
                                MMIPB_CUSTOM_RING_T *ring_ptr//out
                                );


/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_PhotoInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_PHOTO_T    *photo_ptr
                                        );

/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_RingInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_RING_T    *ring_ptr
                                        );
/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_ASPInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_RING_T     *asp_ptr
                                        );
/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_AddContact_Fileinfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_PHOTO_T    *photo_ptr, //IN:
                                        MMIPB_CUSTOM_RING_T     *ring_ptr, //IN:
                                        MMIPB_CUSTOM_RING_T     *asp_ptr
                                        );
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : תsns��Ϣ�ṹ�嵽contact �ṹ��
//  Global resource dependence : 
//  Author:MARYXIAO
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SnsToContact(MMIPB_SNS_CONTACT_T* sns_contact_ptr, MMIPB_CONTACT_T* contact_ptr);
#endif
 /*****************************************************************************/
//  Description : ת��ͨ���ַ���ΪNV������ַ�������
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_WstrToInternalString(
                                wchar * wstr_ptr,//[IN]
                                uint32  wstr_len,//[IN]
                                uint8   *alpha_ptr, //[OUT]
                                uint8   *alpha_len_ptr,//[OUT] 
                                BOOLEAN *is_ucs2_ptr,  //[OUT]
                                uint16  max_out_len
                                );
 /*****************************************************************************/
//  Description : ת��NV������ַ�������Ϊͨ���ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_InternalStringToWstr(
                                uint8  * alpha_ptr, //[IN]
                                uint8   alpha_len,//[IN] 
                                BOOLEAN is_ucs2,  //[IN]
                                uint16   max_wstr_len,//[IN]
                                wchar * wstr_ptr,//[OUT]
                                uint16  *wstr_len_ptr//[OUT]
                                );
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊ�ֻ���ϵ�˵Ĵ洢��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToPhoneContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            MMIPB_CONTACT_PHONE_T *phone_contact_ptr//[OUT]
                                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊ������ϵ�˵Ĵ洢��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToMostUsedContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            MMIPB_CONTACT_MOSTUSED_T *phone_contact_ptr//[OUT]
                                            );
#endif
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��ΪSIM���Ĵ洢��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToSIMContact(
                                            MMIPB_CONTACT_T   *contact_ptr,//[IN]
                                            uint16            max_num_len,//[IN]
                                            PHONEBOOK_ENTRY_T *sim_contact_ptr//OUT
                                            );

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��ΪUSIM������չ��Ϣ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToUSIMAddtionalContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            PHONEBOOK_ADDITIONAL_ENTRY_T *add_contact_ptr//OUT
                                            );

/*****************************************************************************/
//  Description : ��USIM������չ��Ϣת��Ϊ��ϵ����Ϣ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_USIMAddtionalContactToContact(
                                            PHONEBOOK_ADDITIONAL_ENTRY_T *add_contact_ptr,//[IN]
                                            MMIPB_CONTACT_T 	*contact_ptr//[OUT]
                                            
                                            );

/*****************************************************************************/
//  Description : �ж��Ƿ�usim�ӿ�
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUsim(uint16 storage_id);

/*****************************************************************************/
//  Description : ��aas idת��ΪUSIM�ĺ�������
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetNumTypeByAasID(uint8 aas_id);

/*****************************************************************************/
//  Description : ����������ת��ΪUSIM��aas id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetAasIDByNumType(uint8 num_type);

#endif
#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊvcard���ݸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToVcard(
                                     MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                     uint32          data_max_size,//[IN]
                                     uint8 *data_ptr,//[OUT]
                                     uint32 *data_len_ptr//[OUT]
                                    );
#endif
/*****************************************************************************/
//  Description : ���ֻ���ϵ����Ϣ������洢��ʽת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_PhoneContactToContact(
                                            MMIPB_CONTACT_PHONE_T  *phone_contact_ptr,//[IN]
                                            MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ��������ϵ����Ϣ������洢��ʽת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_MostUsedContactToContact(
                                                MMIPB_CONTACT_MOSTUSED_T  *mostused_contact_ptr,//[IN]
                                                MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                                );
#endif
/*****************************************************************************/
//  Description : ����ϵ����Ϣ��SIM��������洢��ʽת��Ϊ��ϵ����Ϣ��ͨ�ø�
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_SIMContactToContact(
                                            PHONEBOOK_ENTRY_T   *sim_contact_ptr,//[IN]
                                            MN_DUAL_SYS_E       dual_sys,//[IN]
                                            PHONEBOOK_STORAGE_E storage_type,//[IN]
                                            MMIPB_CONTACT_T 	*contact_ptr//[OUT]
                                            );

#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : ��vcard����ת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_VcardToContact(
                                    uint8 *data_ptr,//[IN]
                                    uint32  data_len,//[IN]
                                    MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                    );
#endif

/*****************************************************************************/
//  Description : ת��ͨ���ַ���ΪNV������ַ�������
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_WstrToInternalString(
                                wchar * wstr_ptr,//[IN]
                                uint32  wstr_len,//[IN]
                                uint8  * alpha_ptr, //[OUT]
                                uint8  * alpha_len_ptr,//[OUT] 
                                BOOLEAN *is_ucs2_ptr,  //[OUT]
                                uint16  max_out_len
                                );

/*****************************************************************************/
//  Description : ת��NV������ַ�������Ϊͨ���ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_InternalStringToWstr(
                                uint8  * alpha_ptr, //[IN]
                                uint8   alpha_len,//[IN] 
                                BOOLEAN is_ucs2,  //[IN]
                                uint16   max_wstr_len,//[IN]
                                wchar * wstr_ptr,//[OUT]
                                uint16  *wstr_len_ptr//[OUT]
                                );

/*****************************************************************************/
//  Description : ת��ͨ���ַ���ΪgsmЭ��涨���ַ�����ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_WstrToGSMAlphabet(
                                wchar * wstr_ptr,//[IN]
                                uint32  wstr_len,//[IN]
                                uint8  * alpha_ptr, //[OUT]
                                uint8  * alpha_len_ptr,//[OUT] 
                                uint16  max_out_len
                                );

/*****************************************************************************/
//  Description : gsmЭ��涨���ַ�����ʽת��Ϊͨ���ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_GSMAlphabetToWstr(
                                uint8  * alpha_ptr,//[IN] 
                                uint8   alpha_len,//[IN]
                                wchar * wstr_ptr,//[OUT]
                                uint16  *wstr_len_ptr,//[OUT]
                                uint16  max_out_len
                                );

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��ΪgsmЭ��涨�ĸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_BCDNumberToGSMNumber(
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr, 
                                    MN_NUMBER_TYPE_E    *ton_ptr,             
                                    MN_NUMBER_PLAN_E    *npi_ptr,      
                                    uint8               *gsm_number_ptr,
                                    uint8               *gsm_number_len_ptr
                                    );

/*****************************************************************************/
//  Description : gsmЭ��涨�ĺ����ʽת��Ϊbcd��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_GSMNumberToBCDNumber(
                                MN_NUMBER_TYPE_E    *ton_ptr,             
                                MN_NUMBER_PLAN_E    *npi_ptr,      
                                uint8               *gsm_number_ptr,
                                uint8                gsm_number_len,
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr
                                );
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��ΪgsmЭ��涨�ĸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_BCDNumberToANRNumber (
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr,
                                    uint8                   aas_id, 
                                    MN_NUMBER_TYPE_E    *ton_ptr,             
                                    MN_NUMBER_PLAN_E    *npi_ptr,      
                                    uint8               *gsm_number_ptr,
                                    uint8               *gsm_number_len_ptr,
                                    uint8               *aas_id_ptr
                                    );
#endif
/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��Ϊascii�ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
uint8	MMIPB_BCDNumberToUTF8(
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in   
                                    uint8               str_max_len,//IN
                                    uint8               *str_ptr//OUT                                    
                                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
uint16	MMIPB_GetBCDNumberStrlen(
                              MMIPB_BCD_NUMBER_T  *bcd_number_ptr//in                                       
                              );

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��Ϊwchar�ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
uint16	MMIPB_BCDNumberToWstr(
                              MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in   
                              uint16               wstr_max_len,//IN
                              wchar                *wstr_ptr//OUT                                    
                              );

/*****************************************************************************/
//  Description : ��UTF8�ַ���ת��Ϊbcd��ʽ�ĵ绰����
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
void	MMIPB_UTF8ToBCDNumber(
                                uint8               *str_ptr,//IN 
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr//OUT                                                                          
                                );

/*****************************************************************************
 	Description : get name's letters and it's header letters
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
BOOLEAN MMIPB_GetNameLetters(
	const MMIPB_NAME_T 	*name_ptr,//IN
	MMIPB_NAME_STRING_TYPE_T1  letter_type,//IN
	BOOLEAN                     is_transfer_letter,//IN
	MMIPB_NAME_LETTERS_T *letters_str//OUT
);

/*****************************************************************************
 	Description : �ǿո���Ϊ��ͷ���ַ�
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
PUBLIC wchar MMIPB_GetHeadLetter(const wchar *name_ptr);

/*****************************************************************************/
//  Description : ��õ绰���Ĵ�����Ϣ
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_STORAGE_INFO_T MMIPB_GetContactStorageInfo //RETURN: �绰����Ϣ
                                           (
                                            uint16 storage_id
                                            );
/*****************************************************************************/
//  Description : ��õ绰���Ŀռ���Ϣ
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactStorageSpace //RETURN: �ռ���Ϣ
                                           (
                                            uint16 storage_id
                                            );


#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ�����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsMostUsedContact(uint16 storage_id);
#endif
/*****************************************************************************/
//  Description : �ж��Ƿ��ֻ���ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsPhoneContact(uint16 storage_id);
#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ�T����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsTCardContact(uint16 storage_id,
                                                                  uint16 contact_id);
#endif

/*****************************************************************************/
//  Description : �ж��Ƿ�sim����¼
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:��������ϵ�ˣ��������룬�̶����ţ���������
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSimRecord(uint16 storage_id);

#ifdef MMIPB_MOST_USED_SUPPORT
 /*****************************************************************************/
//  Description : is the favouite contact
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsFavouriteContact(uint32 group);
#endif
#ifdef SNS_SUPPORT 
/*****************************************************************************/
//  Description : �ж��Ƿ�sns��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSNSContact(uint16 storage_id);
#endif

#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : �ж��Ƿ�sns��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsVcardContact(uint16 storage_id);
#endif
/*****************************************************************************/
//  Description : �ж��Ƿ�sim��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSIMContact(uint16 storage_id);

#ifdef SNS_SUPPORT  
/*************************************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSnsTypeByGroupId(uint32 group_id, uint16 *sns_type_ptr);
#endif

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ�sns��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsFdnContact(uint16 storage_id);
#endif

#if defined (MMIPB_SIMDN_SUPPORT) 
/*****************************************************************************/
//  Description : �ж��Ƿ�sns��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsMsisdnContact(uint16 storage_id);
#endif

/*****************************************************************************/
//  Description : �ж��Ƿ�SDN��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSDNContact(uint16 storage_id);

/*****************************************************************************/
//  Description :ͨ��groupȡ�ø�group�������������� group id
//  Global resource dependence : 
//  Author:baokun
//  Note: return valid group count
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetAllGroupIDs(uint32 group, 
                                  uint8  group_id[],
                                  uint8  max_group_count
                                  );

/*****************************************************************************/
//  Description : ����storage id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetStorageID(uint8 storage_type, uint8 contact_type);

/*****************************************************************************/
//  Description : ����storage id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16	MMIPB_GetStorageIDByGroupID(uint8 group_id);

/*****************************************************************************/
//  Description : ����entry id ��storage id�� ����ܱ�ʾһ��contact��Ψһid
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetContactId(uint16 entry_id, uint16 storage_id);

/*****************************************************************************/
//  Description : ��ȡdual sys
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPB_GetDualSysByStorageId (uint16 storage_id);

/*****************************************************************************/
//  Description : ��ȡdual sys
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPB_GetDualSysByGroupId(uint8 group_id);

/*****************************************************************************/
//  Description : ��ȡSIM TYPE
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMI_SIM_TYPE_E MMIPB_GetSimTypeByStorageId(uint16 storage_id);

/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: MARYXIAO
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInSIMGroupByStorage(uint16 storage_id);
/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: MARYXIAO
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInNvGroupByStorage(uint16 storage_id);
/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInSIMGroup(uint8 group_id);

/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInNvGroup(uint8 group_id);
/*****************************************************************************/
// 	Description : judge whether the group id is user group
//	Global resource dependence : none
//  Author: starmer.sun
//	Note: �ж��Ƿ����Զ������
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUserGroup(uint8 group_id);

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Discription: is sns group
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSNSGroup(uint16 group_id);
#endif

/*****************************************************************************/
//  Discription: is SIM group
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSIMGroup(uint16 group_id);

/*****************************************************************************/
//  Description : �ж����Ƿ����
//  Global resource dependence : s_mmipb_group_info
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsExistedGroup  //RETURN: TRUE������; FALSE��������
(
 uint8   group_id    //IN: Ⱥ���id��
 );

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ��ǹ̶�����
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsFixedGroup  //RETURN: TRUE������; FALSE��������
(
    uint8   group_id    //IN: Ⱥ���id��
);
#endif
/*****************************************************************************/
//  Description : �ж��Ƿ��Ǵ洢�豸�Ƿ��ǲ������û���ӵļ�¼���豸
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsFixedStorage  //RETURN: TRUE������; FALSE��������
(
    uint16   storage_id    //IN:
);

/*****************************************************************************/
//  Description : ɾ��group ����id�ļ�  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteGroupRingInfo(uint32 group_id);
/*****************************************************************************/
//  Description : ������ת��Ϊ�ַ���
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_ConvertNameToString(
 MMI_STRING_T    	*str_ptr,   //OUT:
 const MMIPB_NAME_T	*name_ptr   //IN:
 );

/*****************************************************************************/
//  Description : Get First Valid Number index from number array number_t_ptr
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetFirstValidNumberIndex(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 num_count);
/*****************************************************************************
 	Description : get string
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
PUBLIC wchar * MMIPB_GetStrByNameType(
         uint8                name_type, //IN
         MMIPB_NAME_LETTERS_T *letters_ptr,//In
		 MMIPB_NAME_LETTERS_T *number_ptr//IN
         );
//pclint 
/*****************************************************************************/
//  Description : ��ring_ptr���浽����group_idӳ��ɵ�����id��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveGroupRingInfo(
                                      uint32 group_id,
                                      const MMIPB_CUSTOM_RING_T *ring_ptr//IN
                                      );

/*****************************************************************************/
//  Description : ������id��Ϣ�ļ��ж�ȡ����id��·����Ϣ  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadGroupRingFile(
                                    uint32 group_id,//IN
                                    MMIPB_CUSTOM_RING_T *ring_ptr//OUT
                                   );

/*****************************************************************************/
//  Description : ��BYTE�л��NUMBER TYPE
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC MN_NUMBER_TYPE_E MMIPB_GetNumberTypeFromUint8(uint8   npi_ton );

/*****************************************************************************/
//  Description : �жϵ绰�����Ƿ���Ч
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsValidPhoneNumber
(
 uint8  npi_ton,
 uint8  number_length
 );

/*****************************************************************************/
//  Description : �жϺ����������Ƿ������Ч����
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExistValidNumber(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count);

/*****************************************************************************/
//  Description : check the contact valid
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsContactValid(MMIPB_CONTACT_T *contact_ptr);


/*****************************************************************************/
//  Description : change file error to pb error
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetErrorType(MMIFILE_ERROR_E file_error);
#ifdef MMIPB_COVERT_LETTER_SUPPORT
/*****************************************************************************/
//  Description : covert polish etc a, to english a
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC wchar ConvertLetters(wchar head_letter);
#endif

/*****************************************************************************/
//  Description : get the actual max length of contact name(save to sim) under
//	pb-encoding 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxLengthAllowedOfGsmAlphabet(
													wchar * wstr_ptr,//[IN]
                                					uint32  wstr_len,//[IN]
                                					uint16  max_out_len,//[IN]
                                					BOOLEAN *is_ucs2//[IN/OUT]
                                					);

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description:�ж��Ƿ�ΪBT Contact
//  Parameter: entry_id, storage_id
//  Global resource dependence :                                
//  Author: Yanyan.an
//  Return : [TRUE] is bt contact; [FALSE] is not bt contact
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsBTContact(uint16 entry_id, uint16 storage_id);

/*****************************************************************************/
//  Description : check if bt contact item
//  Global resource dependence : 
//  Author:cheney wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsBTContactListNode(
                                         uint16 contact_id,
                                         uint16 storage_id
                                         );
#endif

#if (defined(MMI_CONTACT_PHOTO_SUPPORT)||defined(SNS_SUPPORT))
/*****************************************************************************/
//  Description : ��photo_ptr���浽����group_id��entry_idӳ��ɵ�ͼƬid��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_SavePhotoInfoByStorageId(
                                              uint16 entry_id,
                                              uint16 storage_id, //IN:
                                              const MMIPB_CUSTOM_PHOTO_T *photo_ptr//IN
                                              );
#endif
#ifdef   __cplusplus
    }
#endif

#endif
