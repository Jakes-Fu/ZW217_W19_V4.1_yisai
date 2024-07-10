/*****************************************************************************
** File Name:      mmipb_datalist.h                                         *
** Author:                                                                   *
** Date:           11/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file deal with the contact list and qsort list       *
**                 这个文件对记录索引表，快速查找的管理.                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_DATALIST_H_
#define _MMIPB_DATALIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_adapt.h"
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


#ifdef CMCC_VCARD_SUPPORT

typedef struct _MMIPB_BCD_NUMBER_LIST_T_TAG
{
    MMIPB_BCD_NUMBER_T number;
    uint8              num_type;
    struct _MMIPB_BCD_NUMBER_LIST_T_TAG *next_ptr;
}MMIPB_BCD_NUMBER_LIST_T;

typedef struct
{
    MMIPB_BCD_NUMBER_LIST_T *number_list;
    MMI_STRING_T mail_str;
}MMIPB_NUM_MAIL_LIST_T;

#endif
//索引表节点的基本信息
typedef struct
{
    uint32 group;//
    uint16 index;//在索引表中的位置
    uint16 contact_id;//联系人记录id
    uint16 storage_id;//该节点在对应联系人表中的位置：手机联系人和sim卡联系人会有基本信息表，sns联系人直接从数据库中读取；
    uint16 first_char;//首字母
    uint8  num_type[MMIPB_MAX_NV_PHONE_NUM];//每个号码的类型，允许存在多个相同类型的号码
    uint8  mail_exist;//是否存在email
    uint8  visible_flag;//可见性标志：每一位代表一类号码是否可见            
#ifdef BT_PBAP_SUPPORT
    BOOLEAN	 is_bt_contact;  //蓝牙联系人标记
#endif
}MMIPB_CONTACT_BASE_INFO_T;

//联系人索引表节点
typedef struct
{
    uint32 group;     //
    
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_NUM_MAIL_LIST_T *num_mail_list;
    BOOLEAN	is_read_ext;//扩展信息是否读过
#endif
     uint16 index;   //该节点在对应联系人表中的位置：手机联系人会有基本信息表，sim卡联系人直接从sim卡读取,sns联系人直接从数据库中读取；
    uint16 contact_id;//联系人记录id
#ifdef MMIPB_UID_SUPPORT
    uint32 uid;
#endif
    uint16 first_char;//首字母
    uint8  short_storage_id;//联系人记录存储器id（高4位是PHONEBOOK_STORAGE_E，低4位是MMIPB_STORAGE_ID_E）
    uint8  visible_flag;//可见性标志：每一位代表一类号码是否可见            
#ifdef BT_PBAP_SUPPORT
    BOOLEAN		       is_bt_contact;  //蓝牙联系人标记
#endif
}MMIPB_CONTACT_LIST_NODE_T;

//联系人索引表基本信息
typedef struct
{
    uint16  count; //全部联系人的个数     
#ifdef TCARD_PB_SUPPORT
   uint16 tcontact_count;//t卡联系人个数
#endif
    
    uint16  mail_count;//全部邮箱的个数
    uint16  number_count;//全部号码的个数
    uint16  first_char_count;//首字母的个数
    uint16	group_count[MMIPB_MAX_GROUP_NUM];//每个分组中的记录个数
    uint16	storage_count[MMIPB_STORAGE_MAX];//每个存储设备的几个个数
    uint16	group_number_count[MMIPB_MAX_GROUP_NUM]; //每个分组中的电话号码的个数
    uint16	group_mail_count[MMIPB_MAX_GROUP_NUM];  //每个分组中的邮箱的个数  
}MMIPB_CONTACT_LIST_INFO_T;

//联系人索引表
typedef struct
{
    uint16  max_count;
    MMIPB_CONTACT_LIST_INFO_T list_info;
    MMIPB_CONTACT_LIST_NODE_T *list;
}MMIPB_CONTACT_LIST_T;
#ifdef SNS_SUPPORT
//sns联系人概要信息
typedef struct
{
    uint32       group;   //分组
    MMI_STRING_T name_str;//联系人姓名        
}MMIPB_SNS_CONTACT_NODE_T;

//联系人索引表
typedef struct 
{
    uint16                    max_count;//sns联系人的最大个数
    uint16                    current_count;//sns联系人当前个数
    MMIPB_SNS_CONTACT_NODE_T  *contact_table;//每个联系人的索引信息表
}MMIPB_SNS_CONTACT_LIST_T;
#endif
//手机联系人索引表的节点结构
typedef struct
{
    //uint16             contact_id;//联系人记录id
    //uint16             storage_id;//联系人记录存储器id（高8位是PHONEBOOK_STORAGE_E，低8位是MMIPB_STORAGE_ID_E）
    //uint32             group;//联系人所属的group, (uint32：每一位代表一个分组类型)
    uint8              num_type[MMIPB_MAX_NV_PHONE_NUM];//每个号码的类型，允许存在多个相同类型的号码
    MMI_STRING_T       name_str;//联系人姓名
    //uint8              mail_exist;//是否存在email
    MMI_STRING_T       mail_str;//email
    MMIPB_BCD_NUMBER_T number[MMIPB_MAX_NV_PHONE_NUM];//联系人记录的所有号码 
#ifdef BT_PBAP_SUPPORT
    BOOLEAN		       is_bt_contact;  //蓝牙联系人标记
#endif
}MMIPB_PHONE_CONTACT_NODE_T;


//联系人索引表
typedef struct 
{
    uint16                    max_count;//联系人的最大个数
    uint16                    count_count;
    MMIPB_PHONE_CONTACT_NODE_T  *contact_table;//每个联系人的索引信息表
}MMIPB_PHONE_CONTACT_LIST_T;

#ifdef MMIPB_MOST_USED_SUPPORT
//联系人索引表的节点结构
typedef struct
{
    uint16             entry_id;//联系人记录id
    uint16             storage_id;//联系人记录存储器id（高8位是PHONEBOOK_STORAGE_E，低8位是MMIPB_STORAGE_ID_E）
    uint32             used_freq;//使用频率
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_BCD_NUMBER_T number;//使用的号码
#endif
}MMIPB_MOSTUSED_CONTACT_NODE_T;

//常用联系人索引表
typedef struct 
{
    uint16  count;//联系人个数  
    MMIPB_MOSTUSED_CONTACT_NODE_T  *contact_table;//每个联系人的索引信息表
}MMIPB_MOSTUSED_CONTACT_LIST_T;
#endif

//快速查表
typedef struct
{
    uint16   max_count;//
    uint16	 count;//表记录个数
    uint16   *list;//快速查表
}MMIPB_QSORT_LIST_T;

//qsort cmp data
typedef struct
{
    MMI_STRING_T *str_ptr;//比较字符串
    BOOLEAN           is_char;//是否比较字符还是数字
}MMIPB_QSORT_CMP_DATA_T;

//姓名，号码和email的机构
typedef struct  
{
    MMIPB_NAME_T name;
    MMIPB_BCD_NUMBER_T number_t[MMIPB_MAX_NV_PHONE_NUM];
    MMIPB_MAIL_T mail;
    uint32       group;
}MMIPB_NAME_NUMBER_MAIL_T;

//一个联系人有效号码索引表和有效mail索引
typedef struct
{
    uint8 visible_num_index[MMIPB_MAX_NV_PHONE_NUM];//可见的号码的索引数组, 1表示可见，0表示不可见
    BOOLEAN is_mail_visible;
}MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T;

// 记录在处理存储器对应的entry_id时的位置信息
typedef struct _MMIPB_ENTRY_ID_FLAG_T
{
    uint16   array_pos;  // 数组的下标。
    uint8   byte_pos;   // 表示是BYTE中的第几个BIT。
} MMIPB_ENTRY_ID_FLAG_T;

//contact id的使用情况，valid_flag中每一位表示一个id的使用情况， 1表示被占用，0表示空闲
//used conditon of contact id, each bits represents usage of each contact id, 1 means occupied, 0 means spared. 
typedef struct _MMIPB_ENTRY_ID_T
{
    uint16  max_record_num; //   某一存储器中最多可以保存的记录数。
    uint16  spare_id;                         // 尚未被使用的entry_id
    uint8   *valid_flag;                      //contact id的使用情况。指向一个元素为uint8的数组.（dual sim中的记录id有重复的）
} MMIPB_ENTRY_ID_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  search_str: 
// 
/*****************************************************************************/
PUBLIC void MMIPB_InitDataList(void);

/*****************************************************************************/
//  Description : 创建数据表的访问控制信号量
//  
// 
/*****************************************************************************/
PUBLIC void MMIPB_CreateDataListSemphore(void);

/*****************************************************************************/
//  Description : get a free contact
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFreeContactId(uint16  storage_id);

/*****************************************************************************/
//  Description : 获取NV contact id的分配情况。
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfNV(uint8 *valid_buf, 
					                        uint8 buf_len
					                        );

/*****************************************************************************/
//  Description : 获取sim contact id的分配情况。
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfSIM(
                                            MN_DUAL_SYS_E dual_sys,
                                            uint8 *valid_buf, 
                                            uint16 max_count
                                           );

/*****************************************************************************/
//  Description : 分配索引表空间和快速查找表空间
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_ALLOCContactList(void);

/*****************************************************************************/
//  Description : Delete one group's contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOneGroupContactList(uint32 group);

/*****************************************************************************/
//  Description : Find visiable contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindContact(
                            uint16 start_index,//【IN】   
                            uint32 group_mask,//[IN]所有要查找的群组 
                            uint16 storage_id, //要查找的联系人类型                           
                            uint16 *find_index,//[OUT]
                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : Find visiable mostused contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindMostUsedContact(
                            uint16 start_index,
                            uint16 *find_index,//[OUT]                            
                            MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr//[OUT]
                            );
#endif
/*****************************************************************************/
//  Description : 读记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactList(
                            MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                            );

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : check current dail number contact can be empty
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DailNumberContactCanbeEmpty(uint16 storage_id);
#endif

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : FDN有效记录的个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFdnContactCount(uint16 storage_id);
#endif
/*****************************************************************************/
//  Description SIM卡特殊号码的个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSIMDNContactCount(uint16 storage_id);

/*****************************************************************************/
//  Description : 获取该联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactSimpleInfo(
                            uint16 index, 
                            MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : 获取手机联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadPhoneContactBaseInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               );

/*****************************************************************************/
//  Description : 插入一个节点到记录表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactListNode(
                            MMIPB_CONTACT_T *contact_ptr//[IN]
                            );

/*****************************************************************************/
//  Description : 从记录表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactListNode(
                            uint16 contact_id,
                            uint16 storage_id
                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 从常用联系人索引表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllMostUsedContactListNode(void);
#endif
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 从联系人索引表中删除所以sns索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllSNSContactListNode(void);
#endif

#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr);

/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid);
#endif
/*****************************************************************************/
//  Description : 根据记录id和storage id来确定该记录在索引表的索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactListIndex(
                            uint16 contact_id,
                            uint16 stoage_id
                            );

/*****************************************************************************/
//  Description : 从联系人索引表中读取联系人姓名
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: MMIPB_ERROR_SUCCESS:读成功; 返回其他值 :操作失败
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactNameNumberByIndex(
                                                 MMIPB_NAME_NUMBER_MAIL_T *cmp_str,//[OUT]: 比较字符串
                                                 uint16        pos     //IN: 在存储器中的位置
                                                 );

/*****************************************************************************/
//  Description : 读取对应联系人的姓名号码和email信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactNameNumberMail(
                                                         uint16 contact_id,
                                                         uint16 storage_id,
                                                         MMIPB_NAME_NUMBER_MAIL_T *string_ptr
                                                         );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 根据记录id和storage id来确定该记录在常用联系人索引表的索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMostUsedContactListIndex(
                            uint16 contact_id,
                            uint16 stoage_id
                            );
/*****************************************************************************/
//  Description : 读常用联系人记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactCount(
                            uint16  *count_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : 读常用联系人索引表的节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactListNode(
                            uint16 index, 
                            MMIPB_MOSTUSED_CONTACT_NODE_T *data_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : 插入一个节点到常用联系人索引表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertMostUsedContactListNode(
                            MMIPB_CONTACT_T *contact_ptr//[IN]
                            );

/****************************************************************************/
//  Description : 从常用联系人索引表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteMostUsedContactListNode(
                                                         uint16 contact_id,
                                                         uint16 storage_id
                                                         );
#endif
/*****************************************************************************/
//  Description : 查找名字包含该字符串的记录
//  search_str: searching string
// group_id：要查找的记录所在的group id;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInList(wchar *search_str, 
                                          MMIPB_SEARCH_TYPE_T search_type, 
                                          BOOLEAN                      is_char,
                                          uint32 group,
                                          uint32 exclude_group);

/*****************************************************************************/
//  Description : 清除联系人的可见性表
// 
/*****************************************************************************/
PUBLIC void MMIPB_CleanVisibleTable(uint32 group);

/*****************************************************************************/
//  Description : 是所有的联系人可见
// 
/*****************************************************************************/
PUBLIC void MMIPB_EnableAllContactVisible(void);

/*****************************************************************************/
//  Description : 插入一个节点到快速查找表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertQListNode(
                                           uint16       contact_index, //记录索引表的索引
                                           wchar       *wchar_ptr,
                                           uint16       wchar_len,
                                           wchar       *Key_wchar_ptr,
                                           uint16       Key_wchar_len
                                           );

/*****************************************************************************/
//  Description : 从快速查找表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteQListNode(
                            uint16 contact_index
                            );

/*****************************************************************************/
//  Description : 在快速查找表中查找
// str_input：用户的字符串
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchQSortList
(
     MMI_STRING_T *str_input,//IN
     uint32        group,
     uint32       exclude_group,
	 BOOLEAN is_char//TRUE:字符查找；FALSE：数字查找
);

/*****************************************************************************/
//  Description : set mempool handle
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetMempoolHandle(MPOOL_HANDLE handle);

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 从g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录ID
//  Global resource
//  Author: 
//  Parameter: entry_id_ptr：[OUT] g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录ID
//             group_id_ptr：[OUT] g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录的分组ID
//  Note: 
//  Return: TURE: 存在没有读取扩展信息的记录；
//          FALSE: 不存在没有读取扩展信息的记录。
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNextNotInitEntryID(
    uint16 *entry_id_ptr,
    uint16 *storage_id_ptr
    );

/*****************************************************************************/
//  Description : 从内存中读取USIM 扩展信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalFromContactList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );

/*****************************************************************************/
//  Description : USIM 扩展信息保存到索引表中和号码查找表中
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveAddtionToList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );


#endif
#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
//  Description :获取相应组的第某个索引值开始的第一个entry_id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetFirstIdWithIndex(uint32       group_id, uint32* m_index,int *entry_id);

#endif//MCARE_V31_SUPPORT

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsTCardContactExist(void);

#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : Find visiable bt contact from start_index
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindBtContact(
										 uint16 start_index,
										 uint16 *find_index,//[OUT]                            
										 MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
										 );
#endif

/*****************************************************************************/
//  Description : get contacts number in pb
//  Parameter: [In] None
//             [Out] None
//             [Return] count
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 WatchPB_SearchPresetContact(void);

/*****************************************************************************/
//  Description : get contact array by name
//  Parameter: [In] idx_array
//             [Out] idx_array
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_GetPresetContactArray(uint32* idx_array);

/*****************************************************************************/
//  Description : get contact info by index
//  Parameter: [In] index
//             [Out] contact info
//             [Return] success or failed
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchPB_GetPresetContactInfoByIndex(uint32 index, MMIPB_CONTACT_T *info);

#ifndef MMIPB_MEMPOOL_SUPPORT
/*****************************************************************************/
//  Description :释放string cache
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_FreeCache(void);
#endif

/*****************************************************************************/
//  Description : 查找名字是否存在
//  search_str: searching string
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchFullStringInList(MMI_STRING_T *search_str, uint32 storage_id, uint32* pos);

#ifdef   __cplusplus
    }
#endif

#endif


