/*****************************************************************************
** File Name:      mmipb_adapt.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with standard data format and specific    *
                   physical device data format conversation*
**                 这个文件主要是处理PB的标准数据格式和特定物理设备的数据格式
**                 之间的转换               *
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
 
 
 // 电话簿中自定义图片,铃声最大长度
#define MMIPB_MAX_CUSTOM_RING_PATH_LEN		(MMIFILE_FULL_PATH_MAX_LEN)	// 注意NV空间有限,不要太大
#define MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN		(MMIFILE_FULL_PATH_MAX_LEN)
        
#define MMIPB_MAX_CUSTOM_RING_FILE_SIZE		(50 * 1024)
#define MMIPB_MAX_CUSTOM_PHOTO_FILE_SIZE	(50 * 1024)
        
        
//@maggie modify for 6800A P

// 字符串的最大长度
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


#define MMIPB_NAME_FULL_CHAR        0x1 //全拼翻译，字之间没有间隔符：全字符比较
#define MMIPB_NAME_HEAD_CHAR        0x2 //字符匹配：首字母比较
#define MMIPB_NAME_FULL_CHAR_SPACER 0x4 //全拼翻译，字之间有间隔符
typedef uint8 MMIPB_NAME_STRING_TYPE_T1;//姓名字符串类型
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


// 电话簿中的自定义铃声
typedef struct 
{
	BOOLEAN	is_valid;							// 是否有效
	//BOOLEAN is_fixed_ring;						// 固定铃声,还是下载铃声
	//uint8	fixed_ring_id;						// 如果是固定铃声,铃声ID
	uint32	file_name_len;						// 如果是下载铃声,下载铃声的路径名长度
	wchar	file_name[MMIFILE_FULL_PATH_MAX_LEN +1];   //注意结束符号
												// 如果是下载铃声,下载铃声的路径
	uint32	file_size;							// 如果是下载铃声,下载铃声的文件大小
	uint32	ring_type;							// 如果是下载铃声,下载铃声的类型
}MMIPB_CUSTOM_RING_T;

// 电话簿中的自定义图片
typedef struct 
{
	BOOLEAN is_valid;							// 是否有效
	wchar	file_name[MMIFILE_FULL_PATH_MAX_LEN +1];  //注意结束符号
	uint32	file_name_len;						// 下载图片路径长度
												// 下载图片的路径
	uint32	file_size;							// 下载图片文件大小
	uint32	photo_type;							// 下载图片的类型
}MMIPB_CUSTOM_PHOTO_T;

//存储类型
#define MMIPB_STORAGE_FLASH        0x0 //Vcard
#define MMIPB_STORAGE_PHONE        0x1 //手机记录
#define MMIPB_STORAGE_SNS          0x2 //SNS联系人
#define MMIPB_STORAGE_SIM1         0x3 //SIM1记录
#define MMIPB_STORAGE_SIM2         0x4 //SIM2记录
#define MMIPB_STORAGE_SIM3         0x5 //SIM3记录
#define MMIPB_STORAGE_SIM4         0x6 //SIM4记录
#define MMIPB_STORAGE_MAX          0x7 //Storage个数
//手机记录类型
#define MMIPB_NORMAL_PHONE_CONTACT   0x0//一般手机联系人
#ifdef MMIPB_MOST_USED_SUPPORT  
#define MMIPB_MOSTUSED_PHONE_CONTACT 0x1//常用联系人
#endif
//即时信息类型
#define MMIPB_IM_QQ          0x00 //qq
#define MMIPB_IM_MSN         0x01 //MSG, windows live
#define MMIPB_IM_GOOGLE_TALK 0x02 //google talk
#define MMIPB_IM_YAHOO       0x03 //yahoo
#define MMIPB_IM_SKYPE       0x04 //skype
#define MMIPB_IM_AIM         0x05 //AIM
typedef uint8 MMIPB_IM_TYPE;//即时信息类型

// 电话簿中的邮箱
typedef struct _MMIPB_MAIL_T
{
    uint16   wstr_len;                    
	wchar   wstr[MMIPB_MAX_MAIL_LEN + 1];   
} MMIPB_MAIL_T;

// URL存储结构
typedef struct  
{
    uint16   wstr_len;                        // 数组的BYTE个数
    wchar   wstr[MMIPB_URL_MAX_LEN + 1];// 字符串数据
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

// 电话簿中的自定义文字
typedef struct  
{
	uint16	wstr_len;							// 长度
	wchar	wstr[MMIPB_MAX_CUSTOM_TEXT_LEN + 1];
												
}MMIPB_CUSTOM_TEXT_T;


// 电话簿中的职务
typedef struct  
{
    uint16   wstr_len;                      
    wchar   wstr[MMIPB_MAX_TITLE_LEN + 1];
}MMIPB_TITLE_T;

// 电话簿中的公司名称
typedef struct  
{
    uint16   wstr_len;                     
    wchar   wstr[MMIPB_MAX_ORG_LEN + 1]; 
}MMIPB_ORG_T;

//信息文件类型
typedef enum
{
    MMIPB_PHOTO_FILE, //图片文件信息
    MMIPB_RING_FILE,  //铃声文件信息
    MMIPB_ASP_FILE,  //自动答录语文件
    MMIPB_FILE_TYPE_MAX
}MMIPB_FILE_TYPE;

// 电话簿中的工作地址
typedef struct  
{
    uint16   wstr_len;                          
    wchar   wstr[MMIPB_MAX_WORKADR_LEN + 1];
}MMIPB_WORK_ADR_T;

// 联系人的签名
typedef struct  
{
    uint16   wstr_len;                          
    wchar   wstr[MMIPB_SNS_SIGN_LEN + 1];
}MMIPB_SIGN_ADR_T;

typedef struct
{
    uint16            contact_id; //记录ID
    uint16            storage_id; //记录存储器ID
#ifdef MMIPB_UID_SUPPORT
    uint32            uid;
#endif
    union {
        uint32            group; //记录的群组：没有一位代表一个分组，如果几个group id为1，group的第2位为1，可能属于多个group
        uint32            used_freq;//为常用联系人标记使用频率
    }union_t;
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_IM_TYPE     im_type;//即时信息类型：AIM, MSN, yahoo, skype, qq, google talk
#endif
    uint8             ring_id_flag; //联系人的铃声标记
    uint8             photo_id_flag;//联系人头像标记
#ifdef ASP_SUPPORT
    uint8             asp_id_flag; //联系人自动答录语标记
#endif
    BOOLEAN		      is_read_ext; //扩展信息是否已经初始化
    uint8             num_type[MMIPB_MAX_NV_PHONE_NUM];//联系人各个号码的类型
    MMIPB_BCD_NUMBER_T number[MMIPB_MAX_NV_PHONE_NUM];//联系人号码数组
    MMIPB_NAME_T     name;  //联系人姓名
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_MAIL_T      mail; //联系人mail
#endif
#ifdef SNS_SUPPORT 
    MMIPB_SIGN_ADR_T  sign;//签名
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_IM_T     imessager;  //qq,msn and so on
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_BIRTH_T     birth;//生日
    MMIPB_URL_T       url;//网站
    MMIPB_CUSTOM_TEXT_T  memo;//备忘录
#endif
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_NAME_T        alias;//昵称	
#endif
#ifdef BT_PBAP_SUPPORT
	BOOLEAN		is_bt_contact;  //蓝牙联系人标记
#endif
}MMIPB_CONTACT_T;

//快速查找的字符信息
typedef struct _MMIPB_NAME_LETTERS_T
{
    uint16  full_letters_len;                   //全拼串长度
    wchar   full_letters[MMIPB_NAME_MAX_LEN*6 + 1];   // 全拼串
    uint16  head_letters_len;                   // 首字符串长度
    wchar   head_letters[MMIPB_NAME_MAX_LEN +1];   //首字符串 
} MMIPB_NAME_LETTERS_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 在图片id信息文件中读取图片保存进buffer
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
//  Description : 在铃声id信息文件中读取铃声id的路径信息  
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
//  Description : 把photo_ptr保存到根据group_id和entry_id映射成的图片id信息文件中 
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
//  Description : 读取联系人卡本地文件的，铃声，图片或者asp内容进buffer 
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
//  Description : 为本地的contact存储铃声，照片等额外信息提供接口   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_PhotoInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_PHOTO_T    *photo_ptr
                                        );

/*****************************************************************************/
//  Description : 为本地的contact存储铃声，照片等额外信息提供接口   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_RingInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_RING_T    *ring_ptr
                                        );
/*****************************************************************************/
//  Description : 为本地的contact存储铃声，照片等额外信息提供接口   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_ASPInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_RING_T     *asp_ptr
                                        );
/*****************************************************************************/
//  Description : 为本地的contact存储铃声，照片等额外信息提供接口   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_AddContact_Fileinfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_PHOTO_T    *photo_ptr, //IN:
                                        MMIPB_CUSTOM_RING_T     *ring_ptr, //IN:
                                        MMIPB_CUSTOM_RING_T     *asp_ptr
                                        );
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 转sns信息结构体到contact 结构体
//  Global resource dependence : 
//  Author:MARYXIAO
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SnsToContact(MMIPB_SNS_CONTACT_T* sns_contact_ptr, MMIPB_CONTACT_T* contact_ptr);
#endif
 /*****************************************************************************/
//  Description : 转换通用字符串为NV保存的字符串类型
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
//  Description : 转换NV保存的字符串类型为通用字符串
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
//  Description : 将联系人信息转换为手机联系人的存储格式
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
//  Description : 将联系人信息转换为常用联系人的存储格式
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
//  Description : 将联系人信息转换为SIM卡的存储格式
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
//  Description : 将联系人信息转换为USIM卡的扩展信息
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToUSIMAddtionalContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            PHONEBOOK_ADDITIONAL_ENTRY_T *add_contact_ptr//OUT
                                            );

/*****************************************************************************/
//  Description : 将USIM卡的扩展信息转换为联系人信息
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_USIMAddtionalContactToContact(
                                            PHONEBOOK_ADDITIONAL_ENTRY_T *add_contact_ptr,//[IN]
                                            MMIPB_CONTACT_T 	*contact_ptr//[OUT]
                                            
                                            );

/*****************************************************************************/
//  Description : 判断是否usim接口
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUsim(uint16 storage_id);

/*****************************************************************************/
//  Description : 将aas id转化为USIM的号码类型
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetNumTypeByAasID(uint8 aas_id);

/*****************************************************************************/
//  Description : 将号码类型转化为USIM的aas id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetAasIDByNumType(uint8 num_type);

#endif
#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : 将联系人信息转换为vcard数据格式
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
//  Description : 将手机联系人信息从物理存储格式转换为联系人信息的通用格式
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
//  Description : 将常用联系人信息从物理存储格式转换为联系人信息的通用格式
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
//  Description : 将联系人信息从SIM卡的物理存储格式转换为联系人信息的通用格
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
//  Description : 将vcard数据转换为联系人信息的通用格式
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
//  Description : 转换通用字符串为NV保存的字符串类型
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
//  Description : 转换NV保存的字符串类型为通用字符串
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
//  Description : 转换通用字符串为gsm协议规定的字符串格式
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
//  Description : gsm协议规定的字符串格式转换为通用字符串
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
//  Description : 将bcd格式的电话号码转换为gsm协议规定的格式
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
//  Description : gsm协议规定的号码格式转换为bcd格式
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
//  Description : 将bcd格式的电话号码转换为gsm协议规定的格式
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
//  Description : 将bcd格式的电话号码转换为ascii字符串
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
//  Description : 将bcd格式的电话号码转换为wchar字符串
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
//  Description : 将UTF8字符串转换为bcd格式的电话号码
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
 	Description : 非空格作为开头的字符
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
PUBLIC wchar MMIPB_GetHeadLetter(const wchar *name_ptr);

/*****************************************************************************/
//  Description : 获得电话簿的储存信息
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_STORAGE_INFO_T MMIPB_GetContactStorageInfo //RETURN: 电话簿信息
                                           (
                                            uint16 storage_id
                                            );
/*****************************************************************************/
//  Description : 获得电话簿的空间信息
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactStorageSpace //RETURN: 空间信息
                                           (
                                            uint16 storage_id
                                            );


#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 判断是否常用联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsMostUsedContact(uint16 storage_id);
#endif
/*****************************************************************************/
//  Description : 判断是否手机联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsPhoneContact(uint16 storage_id);
#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : 判断是否T卡联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsTCardContact(uint16 storage_id,
                                                                  uint16 contact_id);
#endif

/*****************************************************************************/
//  Description : 判断是否sim卡记录
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:可能是联系人，本机号码，固定拨号，服务号码等
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
//  Description : 判断是否sns联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSNSContact(uint16 storage_id);
#endif

#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : 判断是否sns联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsVcardContact(uint16 storage_id);
#endif
/*****************************************************************************/
//  Description : 判断是否sim联系人
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
//  Description : 判断是否sns联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsFdnContact(uint16 storage_id);
#endif

#if defined (MMIPB_SIMDN_SUPPORT) 
/*****************************************************************************/
//  Description : 判断是否sns联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsMsisdnContact(uint16 storage_id);
#endif

/*****************************************************************************/
//  Description : 判断是否SDN联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSDNContact(uint16 storage_id);

/*****************************************************************************/
//  Description :通过group取得该group中所包含的所有 group id
//  Global resource dependence : 
//  Author:baokun
//  Note: return valid group count
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetAllGroupIDs(uint32 group, 
                                  uint8  group_id[],
                                  uint8  max_group_count
                                  );

/*****************************************************************************/
//  Description : 计算storage id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetStorageID(uint8 storage_type, uint8 contact_type);

/*****************************************************************************/
//  Description : 计算storage id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16	MMIPB_GetStorageIDByGroupID(uint8 group_id);

/*****************************************************************************/
//  Description : 根据entry id 和storage id， 获得能表示一条contact的唯一id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetContactId(uint16 entry_id, uint16 storage_id);

/*****************************************************************************/
//  Description : 获取dual sys
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPB_GetDualSysByStorageId (uint16 storage_id);

/*****************************************************************************/
//  Description : 获取dual sys
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPB_GetDualSysByGroupId(uint8 group_id);

/*****************************************************************************/
//  Description : 获取SIM TYPE
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
//	Note: 判断是否是自定义分组
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
//  Description : 判断组是否存在
//  Global resource dependence : s_mmipb_group_info
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsExistedGroup  //RETURN: TRUE，存在; FALSE，不存在
(
 uint8   group_id    //IN: 群组的id号
 );

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 判断是否是固定分组
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsFixedGroup  //RETURN: TRUE，存在; FALSE，不存在
(
    uint8   group_id    //IN: 群组的id号
);
#endif
/*****************************************************************************/
//  Description : 判断是否是存储设备是否是不允许用户添加的记录的设备
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsFixedStorage  //RETURN: TRUE，存在; FALSE，不存在
(
    uint16   storage_id    //IN:
);

/*****************************************************************************/
//  Description : 删除group 铃声id文件  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteGroupRingInfo(uint32 group_id);
/*****************************************************************************/
//  Description : 将姓名转化为字符串
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
//  Description : 把ring_ptr保存到根据group_id映射成的铃声id信息文件中 
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
//  Description : 在铃声id信息文件中读取铃声id的路径信息  
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
//  Description : 从BYTE中获得NUMBER TYPE
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC MN_NUMBER_TYPE_E MMIPB_GetNumberTypeFromUint8(uint8   npi_ton );

/*****************************************************************************/
//  Description : 判断电话号码是否有效
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
//  Description : 判断号码数组中是否存在有效号码
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
//  Description:判断是否为BT Contact
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
//  Description : 把photo_ptr保存到根据group_id和entry_id映射成的图片id信息文件中 
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
