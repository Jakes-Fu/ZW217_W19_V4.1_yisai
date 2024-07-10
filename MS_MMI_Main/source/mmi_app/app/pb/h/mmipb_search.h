/*****************************************************************************
** File Name:      mmipb_search.h                                             *
** Author:         Lin Lin                                                   *
** Date:           23/11/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi phonebook search        *
**                 function .                                                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2004        Lin Lin      Create
******************************************************************************/

#ifndef _MMIPB_SEARCH_H_
#define _MMIPB_SEARCH_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

//#include "mmk_type.h"
//#include "mmipb_mnapi.h"
#include "mmipb_interface.h"
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

        //需要显示的所有记录索引
typedef struct
{
    uint16 max_count;//表的最大个数
    uint16 used_count;//现在使用的个数
    uint32 *index_table;//其中低16每个记录在数据库或数据表中的索引，高16位对应该记录的哪个号码即：号码的index
}MMIPB_VISIBLE_INDEX_TABLE_T;

typedef struct _SEARCH_PHONE_NUMBER
{
    //MMIPB_BCD_NUMBER_T	number_t;
    uint32  low;     //保存低8位的号码
    uint16  high;    //保存高3位的号码
	BOOLEAN is_plus_in_head; //号码前面是否带"+"
} SEARCH_PHONE_NUMBER_T;

//号码匹配表节点
typedef struct 
{
    SEARCH_PHONE_NUMBER_T   phone_number;  
    uint16                  entry_id;         
    uint16                  storage_id;    
    uint8                   npi_ton;
    uint8                   number_len;         
    uint8                   original_len;        
    uint8                   num_index;         
} MMIPB_SEARCH_ITEM_T;

//号码匹配表基本信息
typedef struct _MMIPB_SEARCH_LIST_INFO_T
{   
    uint16               count;  //号码个数        
	uint16               max_count;//号码最大个数
} MMIPB_SEARCH_LIST_INFO_T;

//号码匹配表结构
typedef struct _MMIPB_SEARCH_LIST_T
{
    MMIPB_SEARCH_ITEM_T* item_ptr; // 号码列表      
    MMIPB_SEARCH_LIST_INFO_T list_inof;  //号码表的基本信息          
} MMIPB_SEARCH_LIST_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化号码搜索列表。
//  Global resource dependence : g_mmipb_search_list_ptr
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitNumberList(uint32 num_max_count);
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 初始化号码搜索列表。
//  Global resource dependence : s_pb_number_list, s_pb_mostused_number_list
//  Author: baokun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CleanAllMostusedNumber(void);
#endif
/*****************************************************************************/
//  Description : add phone number to search list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertNumberToList(
                                const MMIPB_CONTACT_T* entry_ptr
                                );                              

#ifdef CMCC_VCARD_SUPPORT

/*****************************************************************************/
//  Description : add phone number to search list
//  Global resource dependence : 
//  Author: yao.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertAddtionNumberToList(
                                const MMIPB_CONTACT_T* entry_ptr
                                );                              
#endif

/*****************************************************************************/
//  Description : delete phone number from search list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DelNumberFromList(
                                        uint16 entry_id,	//IN: 待删除记录的记录号
                                        uint16 storage_id,  //IN: 待删除记录的storage_id                                        
                                        uint8* num_index_ptr //IN: 删除的号码index,如果为空，表示删除该记录对应的所有号码
                                        );

/*****************************************************************************/
//  Description : 电话号码的职能匹配
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:两个号码能够职能匹配，其他不能匹配
/*****************************************************************************/
PUBLIC int32 MMIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, 
                               const MMIPB_BCD_NUMBER_T *bcd_number2);

/*****************************************************************************/
//  Description : 获得存储位置所对应的姓名、群组ID、图片ID
//  Global resource dependence : none
//  Parameter:  bcd_number : 输入的电话号码[IN]
//              name_ptr:  姓名[OUT]
//              max_name_len:姓名的最大长度(以字节计)  [IN]
//   RETURN: TRUE, 操作成功；FALSE, 操作失败
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNameByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
                                     MMI_STRING_T *name_ptr,
                                     uint16 max_name_len,
                                     uint16* entry_id,
                                     uint16* storage_id,
                                     uint32* group,
                                     BOOLEAN is_use_default_name,
                                     BOOLEAN is_protect);

/*****************************************************************************/
//  Description : 获取给定号码对应的号码类型
//  Global resource dependence : none
//  Parameter:  bcd_number : 输入的电话号码[IN]
//              type_ptr:  号码类型[OUT]
//              max_name_len:号码类型字符串的最大长度(以wchar计)  [IN]
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNumberTypeByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
                                           wchar *type_ptr,
                                           uint16 max_type_len);

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录和姓名中按照首字母匹配的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 找到的号码的个数
/*****************************************************************************/
PUBLIC uint32 MMIPB_SearchList(MMI_STRING_T *search_str, 
                               uint32 field_type, 
                               MMIPB_SEARCH_TYPE_T search_type, 
                               MMIPB_HANDLE_T *handle_ptr//[OUT]
                               );

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录和姓名中按照首字母匹配的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 找到的号码的个数
/*****************************************************************************/
PUBLIC void MMIPB_ExitSearchList(MMIPB_HANDLE_T handle);


/*****************************************************************************/
//  Description : 该函数实现模糊查找
// str_input：用户的字符串
// group：要装载的记录的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示
/*****************************************************************************/
PUBLIC uint32 MMIPB_MultiSearch
(
     MMI_STRING_T *str_input,//IN
     uint32 group,//IN
     uint32 exclude_group,
     uint32 search_type
);

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的姓名，和号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSearchEntry(MMIPB_HANDLE_T handle,
                                       MMI_STRING_T *name_str,//[OUT] 
                                       uint16 max_name_len, 
                                       MMIPB_BCD_NUMBER_T *bcd_num,//[OUT]
                                       uint32* group,
                                       uint32 index);

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的contact_id and storage id
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 为外部进行进pb选择后
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSearchEntryID(MMIPB_HANDLE_T handle,                                       
                                       uint32 index,
                                       uint16 *contact_id_ptr,
                                       uint16 *storage_id_ptr);

/*****************************************************************************/
//  Description : Search phone number in list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_SearchPhoneNumber(
                                MMIPB_BCD_NUMBER_T  *bcd_num_ptr,   //IN: 源号码
                                uint16* entry_id,                   //OUT: entry_id
                                uint16* storage_id,                 //OUT: entry_id 
                                uint8*  num_index_ptr               //OUT: num_index 
                                );

/*****************************************************************************/
//  Description : Search phone number in list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_SearchPhoneNumberInMostUsedlist(
                                MMIPB_BCD_NUMBER_T  *bcd_num_ptr,   //IN: 源号码
                                uint16* entry_id,                   //OUT: entry_id
                                uint16* storage_id                    //OUT: storage
                                );

/*****************************************************************************/
//  Description : transfer bcd phone number to hex number
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
uint8 PhoneNumberToHex                          //return phone number length
(
    const MMIPB_BCD_NUMBER_T	*bcd_num_ptr,	//IN:
    SEARCH_PHONE_NUMBER_T       *phone_number_ptr //OUT
);
///*****************************************************************************/
////  Description :恢复默认
////  Global resource dependence : none
////  Author: 
////Return: 
///*****************************************************************************/
// PUBLIC void MMIPB_SetDefaultRange(uint8 group_id);


/*****************************************************************************/
//  Description :check two number is partial matched
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsNumberPartialMatch(MMIPB_SEARCH_ITEM_T* base_item_ptr, 
										  MMIPB_SEARCH_ITEM_T* table_item_ptr);

/*****************************************************************************/
//  Description :判断该号码是否需要精确匹配
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExactMatchNumber(SEARCH_PHONE_NUMBER_T phone_number, uint16 num_len);

/*****************************************************************************/
//  Description : 设置tab切换时是否需要修改contact页面的style
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIPB_SetPermitChangeType(BOOLEAN ispermit);

/*****************************************************************************/
//  Description : 该函数实现名字查找
// str_input：用户的字符串
// group：要查找的记录的所有group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsNameExist(MMI_STRING_T *str_input, uint32 storage_id, uint32* pos);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
