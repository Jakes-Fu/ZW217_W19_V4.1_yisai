/*****************************************************************************
** File Name:      mmipb_storage.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with the physical device access function *
**                 处理各种物理存储设备的访问                  *
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
 //联系人字段类型    
typedef enum
{
    MMIPB_CONTACT_NUMBER,//号码
    MMIPB_CONTACT_NAME,//姓名
    MMIPB_CONTACT_EMAIL,//email
    //MMIPB_CONTACT_EMAIL_FREE_NUM,//email空闲个数
    MMIPB_CONTACT_SNE, //别名
    //MMIPB_CONTACT_SNE_FREE_NUM, //别名空闲个数
    MMIPB_SIM_EXT1, //扩展信息
}MMIPB_CONTACT_DOMAIN_E;
//contact name
typedef struct _MMIPB_INTERNAL_NAME_T
{
    BOOLEAN is_ucs2;                     // 字符串的编码类型
    uint8   alpha_len;                   // alpha数组的BYTE个数
	uint8   alpha[MMIPB_NAME_MAX_LEN  + 1];   // 具体意义请查看GSM 11.11
}MMIPB_INTERNAL_NAME_T;

// 电话簿中的邮箱
typedef struct _MMIPB_INTERNAL_MAIL_T
{
    BOOLEAN is_ucs2;					 // 是否是UCS2
    uint8   mail_len;                    // mail数组的BYTE个数
	uint8   mail[MMIPB_MAX_MAIL_LEN + 1];    // 具体的mail地址
} MMIPB_INTERNAL_MAIL_T;

// 电话簿中的自定义文字
typedef struct  
{
	BOOLEAN is_valid;							// 是否有效
	BOOLEAN is_ucs2;							// 是否是UCS2
	uint8	text_len;							// 长度
	uint8	text_buf[MMIPB_MAX_CUSTOM_TEXT_LEN  + 1];
												
}MMIPB_INTERNAL_CUSTOM_TEXT_T;

// 电话簿中的职务
typedef struct  
{
    BOOLEAN is_ucs2;                        // 字符串的编码类型
    uint8   title_len;                      // 数组的BYTE个数
    uint8   title_text[MMIPB_MAX_TITLE_LEN + 1];// 字符串数据
}MMIPB_INTERNAL_TITLE_T;

// 电话簿中的公司名称
typedef struct  
{
    BOOLEAN is_ucs2;                     // 字符串的编码类型
    uint8   org_len;                     // 数组的BYTE个数
    uint8   org_text[MMIPB_MAX_ORG_LEN + 1]; // 字符串数据
}MMIPB_INTERNAL_ORG_T;

// 电话簿中的工作地址
typedef struct  
{
    BOOLEAN is_ucs2;                          // 字符串的编码类型
    uint8   adr_len;                          // 数组的BYTE个数
    uint8   adr_text[MMIPB_MAX_WORKADR_LEN + 1];// 字符串数据
}MMIPB_INTERNAL_WORK_ADR_T;

// sign存储结构
typedef struct  
{
    BOOLEAN is_ucs2;                          // 字符串的编码类型
    uint8   sign_len;                          // 数组的BYTE个数
    uint8   sign_text[MMIPB_SNS_SIGN_LEN + 1];// 字符串数据
}MMIPB_INTERNAL_SIGN_T;

// qq存储结构
typedef struct  
{
    BOOLEAN is_ucs2;                          // 字符串的编码类型
    uint8   im_len;                          // 数组的BYTE个数
    uint8   im_buf[MMIPB_IM_MAX_LEN + 1];// 字符串数据
}MMIPB_INTERNAL_IM_T;

// birth存储结构
typedef struct  
{
    BOOLEAN is_ucs2;                          // 字符串的编码类型
    uint8   birth_len;                          // 数组的BYTE个数
    uint8   birth_buf[MMIPB_SNS_BIRTH_LEN + 1];// 字符串数据
}MMIPB_INTERNAL_BIRTH_T;

// URL存储结构
typedef struct  
{
    BOOLEAN is_ucs2; 
    uint8   url_len;                        // 数组的BYTE个数
    uint8   url_buf[MMIPB_URL_MAX_LEN + 1];// 字符串数据
}MMIPB_INTERNAL_URL_T;

//手机联系人的存储结构
typedef struct
{
	uint16             entry_id; //联系人记录id
	uint16             storage_id;//联系人记录存储器id（高8位是PHONEBOOK_STORAGE_E，低8位是MMIPB_STORAGE_ID_E）
#ifdef MMIPB_UID_SUPPORT
    uint32             uid;
#endif
	uint32             group;     //联系人所属的group, (uint32：每一位代表一个分组类型)
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    uint8              im_type;//即时信息类型：AIM, MSN, yahoo, skype, qq, google talk
#endif
	uint8              ring_id_flag;//联系人是否定义了来电铃声，如果第8位为1表示为固定铃声，低7位表示固定铃声的ring id，如果值为1表示用户定义了非固定铃声
	uint8              photo_id_flag;//联系人是否定义了来电图片，0：表示没有定义
#ifdef ASP_SUPPORT
	uint8	                asp_id_flag; //asp id information:第8位表示是否有asp:0:没有,1:有；	                                     //第7位表示是固定id,还是用户自定义:1:Fixed, 0:user defined;	                                     //低6位为Fixed id的值 
#endif
	uint8              num_type[MMIPB_MAX_NV_PHONE_NUM];//每个号码的类型，允许存在多个相同类型的号码
	MMIPB_BCD_NUMBER_T number[MMIPB_MAX_NV_PHONE_NUM];//联系人记录的所有号码
	MMIPB_INTERNAL_NAME_T     name; //联系人姓名
#ifdef MMIPB_MAIL_SUPPORT
	MMIPB_INTERNAL_MAIL_T      mail;//联系人mail地址    
#endif
    //MMIPB_INTERNAL_SIGN_T  sign;   
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_INTERNAL_IM_T    imessager;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_INTERNAL_BIRTH_T birth;
	MMIPB_INTERNAL_CUSTOM_TEXT_T  memo;	//备忘录
    MMIPB_INTERNAL_URL_T   url;
	#if 0
	MMIPB_INTERNAL_TITLE_T        title; //职务  
	MMIPB_INTERNAL_ORG_T         company; //公司名称                      
	MMIPB_INTERNAL_WORK_ADR_T  address;  //地址
    #endif
#endif
#ifdef BT_PBAP_SUPPORT
	BOOLEAN		is_bt_contact;  //蓝牙联系人标记
#endif
}MMIPB_CONTACT_PHONE_T;


#ifdef MMIPB_MOST_USED_SUPPORT
//常用联系人的存储结构
typedef struct
{
	uint16             entry_id; //联系人记录id
	uint16             storage_id;//联系人记录存储器id（高8位是PHONEBOOK_STORAGE_E，低8位是MMIPB_STORAGE_ID_E）
	uint32             used_freq; //使用频率
    MMIPB_BCD_NUMBER_T number;    //使用的号码  
}MMIPB_CONTACT_MOSTUSED_T;
#endif

//sns联系人具体内容
typedef struct
{
    char       user_id[MMIPB_SNS_USERID_LEN];
    char       file_path[MMIPB_SNS_PHOTO_PATH_LEN + 1]; // 大头贴路径
    char       name[MMIPB_SNS_NAME_LEN + 1]; //好友名
    char       email[MMIPB_SNS_EMAIL_LEN + 1]; //Email 地址
    char       birth[MMIPB_SNS_BIRTH_LEN + 1]; //好友生日
    char       sign[MMIPB_SNS_SIGN_LEN + 1]; //好友签名
    uint16       type; // 类型(SINA,TWITTER, FACEBOOK...)
}MMIPB_SQLITE_DATA_T;

//sns联系人的存储结构
typedef struct
{
    uint32 nRow;
    uint16 uiNum;      //查询起止
    uint16 uiOffset;    // 记录id
    MMIPB_SQLITE_DATA_T search_result;
}MMIPB_SNS_CONTACT_T;

// 设置的组的信息，包括组命名、组图标和组铃音。与NV中保存的值应该保持一致。
typedef struct _MMIPB_GROUP_T
{
    BOOLEAN             is_valid;   // TRUE, 有效
    uint8	            group_id;   // 群组的id号
    uint8	            ring_id_flag;   // 群组的id号
    MMIPB_NAME_T        group_name; // 群组的命名
} MMIPB_GROUP_T;

typedef struct _MMIPB_STORAGE_INFO_T
{
    BOOLEAN is_valid;           // TRUE，存储器对应的电话簿有效
	uint8	max_alpha_len;		// 存储器中支持联系人姓名的最大长度
#ifdef MMIPB_MAIL_SUPPORT
    uint8   max_email_alpha_len;//存储器中支持联系人email的最大长度
#endif
#ifdef CMCC_VCARD_SUPPORT
    uint8   max_sne_alpha_len;  //存储器中支持的联系人别名的最大长度
#endif
    uint8   max_num_len;        // 存储器中支持的电话簿号码的最大长度
	uint16	max_record_num;		// 存储器中最多可以保存的记录数。
    uint16  used_record_num;    // 存储器中已经保存的记录数。
} MMIPB_STORAGE_INFO_T;

//联系人各个字段的使用情况信息
typedef struct _MMIPB_FIELD_USAGE_INOF_T
{
	BOOLEAN	number_free_flag[MMIPB_MAX_NV_PHONE_NUM]; //号码空间是否空闲标志：TRUE:该类型记录有空闲 
    BOOLEAN photo_free_flag;    //是否允许设置来电图片
    BOOLEAN group_free_falg;     //是否允许设置分组
    BOOLEAN ring_free_falg;     //是否允许设置来电铃声
#ifdef ASP_SUPPORT
    BOOLEAN asp_free_falg;      //是否允许设置自动答录语
#endif
#ifdef MMIPB_MAIL_SUPPORT
    BOOLEAN mail_free_flag;     //mail是否空闲标志：TRUE: 该类型记录mail有空闲
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    BOOLEAN im_free_flag;       //是否支持即时信息
#endif
#ifdef CMCC_VCARD_SUPPORT
    BOOLEAN sne_free_flag;      //别名是否空闲标志:TRUE:该类型记录别名有空闲
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    BOOLEAN memo_free_flag;     //备忘字段录是否空闲
    BOOLEAN birth_free_flag;    //生日字段是否存在
    BOOLEAN url_free_flag;     //网页字段是否存在
#endif
#ifdef SNS_SUPPORT 
    BOOLEAN sign_free_flag;    //签名字段是否存在
#endif
} MMIPB_FIELD_USAGE_INOF_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 获得联系人对应字段的最大uid
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetMaxUid(void);

/*****************************************************************************/
//  Description : set联系人对应字段的最大uid
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
//  Description : 更新sns联系人信息
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsContactByGroupId(uint8 group_id);
#endif
/*****************************************************************************/
//  Description : 获得联系人对应字段的最大alpha长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetContactMaxAlphaLen(PHONEBOOK_STORAGE_E storage, 
                                  MN_DUAL_SYS_E dual_sys,
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                  );

/*****************************************************************************/
//  Description : 获得联系人对应字段的最大wstr长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetContactMaxWstrLen(PHONEBOOK_STORAGE_E storage,// 
                                  MN_DUAL_SYS_E dual_sys,//卡
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                 );


/*****************************************************************************/
//  Description : 获得电话簿的储存信息
//  Global resource dependence : s_mmipb_nv_info_t
//  Author: 
//  Note: 
/*****************************************************************************/
MMIPB_STORAGE_INFO_T MMIPB_GetContactStorage //RETURN: 电话簿信息
                                            (
                                             MN_DUAL_SYS_E       dual_sys,
                                             PHONEBOOK_STORAGE_E storage             //IN: 存储器类型
                                             );

/*****************************************************************************/
//  Description : 获得电话簿的空间信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  uint16 MMIPB_GetStorageSpace //RETURN: 电话簿信息
(
 MN_DUAL_SYS_E       dual_sys,
 PHONEBOOK_STORAGE_E storage             //IN: 存储器类型
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
//  Description : 获得sns联系人个数
//  Author: 
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMIPB_GetSNSContactsCount(uint16 sns_type);

/*****************************************************************************/
//  Description : 获得sns账户信息(email)的长度
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC uint32  MMIPB_GetSNSAccountEmailLen(void);

/*****************************************************************************/
//  Description : 获得sns账户信息(email)
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSNSAccountEmail(uint8 group_id,                //in 
                                                                                    MMI_STRING_T* email);     //out

/*****************************************************************************/
//  Description : 获得sns账户活动更新信息
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetSNSActiveInfo(uint8  group_id,        //in    
                                 uint16 contact_id,           //in : index
                                 MMIPB_NAME_T name,    //in 
                                 void **activity_ptr);     //out
                                 
/*****************************************************************************/
//  Description : 更新sns联系人信息
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsContact(void);                                 
#endif                                                                                    
/*****************************************************************************/
//  Description : 获得手机联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetPhoneContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 获得SNS联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetSNSContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 获得常用联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetMostUsedContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );
#endif
/*****************************************************************************/
//  Description : 获得sim卡联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:如果contact_id为0，表示新增，返回一个新的id
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
//  Description : 读取USIM 扩展号码的类型字符串
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
//  Description : 写USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:本功能有3G PS还没有支持，暂时保留
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteUSIMAASString(
                           MN_DUAL_SYS_E dual_sys,//[IN]
                           uint8         aas_index,//[IN]
                           MMIPB_NAME_T *name_ptr//[OUT]
                           );

/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的类型个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetUSIMAASCount(
                           MN_DUAL_SYS_E dual_sys
                           );
/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的可用个数
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
