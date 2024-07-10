/****************************************************************************
** File Name:      mmipb_datalist.c                                        *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with the contact list and qsort list      *             
**                 这个文件对记录索引表，快速查找表的管理.  
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_DATALIST_C_  


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mn_type.h"
#include "mmipb_datalist.h"
#include "mmipb_storage.h"
#include "mmipb_interface.h"
#include "mmifdn_export.h"
#include "mmiim.h"
#include "mmipb_search.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPB_MASK_ENTRY_ID_FLAG_1              0X01

#define MMIPB_INVALID_INDEX                     0xFFFF

#define MMIPB_EMPTY_STRING_FLAG                 0xFFFF
#define MMIPB_KEY_HEAD_FLAG                     0X8000//最高为1表示是键值，否则为字符
#define MMIPB_CONTACT_INDEX_MASK                0X7FFF//取联系人索引



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 在entry_id对应的列表中为某个entry_id赋值，并为spare_id赋值
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: spare_id是列表中最小的未被使用的entry_id
/*****************************************************************************/
LOCAL void SetEntryIdFlag
(
 BOOLEAN                is_add,         //IN: TRUE，表示置1；FALSE，表示置0
 uint16              entry_id,      //IN: 从1开始
 MMIPB_ENTRY_ID_T   *entry_id_ptr   //IN/OUT: entry_id列?                       
 );

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int ContactListCmp(uint32 base_index, void *compare_data, void *list);

/*****************************************************************************/
//  Description : get contact email
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: group
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsContactHasMail(
                      MMIPB_CONTACT_LIST_NODE_T node
                     );


/*****************************************************************************/
//  Description : get num_type
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: num_type
//  Note: 
/*****************************************************************************/
LOCAL void GetContactNumType(
                      MMIPB_CONTACT_LIST_NODE_T node,
                      uint8 num_type[],//OUT
                      uint8 type_count
                     );

/*****************************************************************************/
//  Description : change storage id to short storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint8 StorageIDToShortID(
                         uint16 storage_id
                         );

/*****************************************************************************/
//  Description : change short storage id to storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint16 ShortIDToStorageID(
                         uint8 short_storage_id
                         );

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : Find dail number contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: msisdn, fdn, sdn...
/*****************************************************************************/
LOCAL MMIPB_ERROR_E FindDailNumberContact(
                                       uint16 start_index,   
                                       uint16 storage_id, //要查找的联系人类型
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       );
#endif
/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListCmp(uint32 base_index, void *compare_data, void* list);

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListNCmp(uint32 base_index, void *compare_data, void *list);

/*****************************************************************************/
//  Description :根据字符串在快速查找表中查找对应的index
//str_input:需要查找的字符串
//is_back: TRUE:查找最后一个匹配的位置;FALSE:查找第一个匹配的位置
// 返回匹配的位置
/*****************************************************************************/
LOCAL uint16 GetQSortListIndex
(
    MMI_STRING_T *str_input,//IN
    BOOLEAN      is_char,//IN
    BOOLEAN      is_back
); 

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   
//快速查找表
LOCAL MMIPB_QSORT_LIST_T s_pb_qsort_list = {0};
//记录索引表
LOCAL MMIPB_CONTACT_LIST_T s_pb_contact_list = {0};

//手机联系人缓存表
LOCAL MMIPB_PHONE_CONTACT_LIST_T s_pb_phone_contact_list = {0};
#ifdef SNS_SUPPORT
//sns联系人缓存表
LOCAL MMIPB_SNS_CONTACT_LIST_T s_pb_sns_contact_list = {0};
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
//常用联系人索引表
LOCAL MMIPB_MOSTUSED_CONTACT_LIST_T s_pb_mostused_contact_list = {0};
LOCAL MMIPB_ENTRY_ID_T s_pb_mostused_contact_id_table = {0};
#endif
//contact id的使用情况表
LOCAL MMIPB_ENTRY_ID_T s_pb_phone_contact_id_table = {0};

LOCAL MPOOL_HANDLE     s_pb_mempool_handle = 0;

//semphore
LOCAL SCI_SEMAPHORE_PTR              s_contact_list_semphore_p = PNULL;        //联系人列表的访问信号量
#ifdef MMIPB_MOST_USED_SUPPORT
LOCAL SCI_SEMAPHORE_PTR              s_mostusedcontext_list_semphore_p = PNULL;//常用联系人列表的访问信号量
#endif
LOCAL SCI_SEMAPHORE_PTR              s_qsort_list_semphore_p = PNULL;          //快速查找的访问信号量
                                                                               /**--------------------------------------------------------------------------*
                                                                               **                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 在entry_id对应的列表中为某个entry_id赋值，并为spare_id赋值
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: spare_id是列表中最小的未被使用的entry_id
/*****************************************************************************/
LOCAL void SetEntryIdFlag
(
 BOOLEAN                is_add,         //IN: TRUE，表示置1；FALSE，表示置0
 uint16              entry_id,      //IN: 从1开始
 MMIPB_ENTRY_ID_T   *entry_id_ptr   //IN/OUT: entry_id列?                       
 )
{
    uint16   index_value = 0;
    uint16    i = 0;
    uint16    j = 0;
    uint16    k = 0;
    
    uint16   first_array_pos = 0;
    uint16   last_array_pos  = 0;
    
    uint16   last_byte_pos  = 0;
    
    BOOLEAN is_spare_id = FALSE;
    
    MMIPB_ENTRY_ID_FLAG_T   flag_t = {0};
    MMIPB_ENTRY_ID_FLAG_T   max_flag_t = {0};
    //MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_1;

    if(0 == entry_id || PNULL == entry_id_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]SetEntryIdFlag entry_id 0x%x pb_sort_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_185_112_2_18_2_43_18_174,(uint8*)"dd", entry_id, entry_id_ptr);
        return;
    }     
    // 找到待修改的entry_id在第几个byte中的第几个bit
    flag_t.array_pos    = ((entry_id - 1) >> 3);
    flag_t.byte_pos     = (uint8)((entry_id - 1) & 0x07);
    
    // 找到entry_id的最大值在第几个byte中的第几个bit
    max_flag_t.array_pos    = ((entry_id_ptr->max_record_num - 1) >> 3);
    max_flag_t.byte_pos     = (uint8)((entry_id_ptr->max_record_num - 1) & 0x07);
    
    if (is_add)
    {
        // 在对应entry_id的位置置1
        entry_id_ptr->valid_flag[flag_t.array_pos] |= (MMIPB_MASK_ENTRY_ID_FLAG_1 << flag_t.byte_pos);
        
        // 正好使用的是spare_id
        if (entry_id == entry_id_ptr->spare_id)
        {
            // 为spare_id赋一个无效的值
            entry_id_ptr->spare_id = MMIPB_INVALID_MAX_ENTRY_ID;
            
            first_array_pos = flag_t.array_pos;
            last_array_pos  = max_flag_t.array_pos;
            
            // 找到下一个spare_id的位置
            // 分段查找
            // 先从目前空闲记录号所在的BYTE开始查找
            // 如果没有找到，再从最前面开始找到目前空闲记录号所在的BYTE
            while ((!is_spare_id) && (k < 2))
            {
                // 从当前记录号对应BYTE的最低位开始查找
                // 如果没有记录，该BYTE会查找两次
                for (i=first_array_pos; i<=last_array_pos; i++)
                {
                    // 如果不是最后一个BYTE, 表示有空闲的记录号
                    // 如果是最后一个BYTE, 则不一定
                    if (0xff != entry_id_ptr->valid_flag[i])
                    {
                        last_byte_pos   = 7;
                        
                        if (i == max_flag_t.array_pos)
                        {
                            last_byte_pos = max_flag_t.byte_pos;
                        }
                        
                        for (j=0; j<=last_byte_pos; j++)
                        {
                            if (!(entry_id_ptr->valid_flag[i] & (MMIPB_MASK_ENTRY_ID_FLAG_1 << j)))
                            {
                                // 如果该位没有被置为1
                                index_value = i;
                                entry_id_ptr->spare_id = (index_value << 3) + j + 1;
                                is_spare_id = TRUE;
                                return;
                            }
                        }
                    } // end of if
                } // end of for
                
                k++;
                
                if (1 == k)
                {
                    // 查找的范围是前一部分
                    first_array_pos = 0;
                    last_array_pos  = flag_t.array_pos;
                }
                
            } // end of while
        }
    }
    else
    {
        // 在对应entry_id的位置置0
        entry_id_ptr->valid_flag[flag_t.array_pos] &= ~(uint8)(MMIPB_MASK_ENTRY_ID_FLAG_1 << flag_t.byte_pos);
        
        // 将spare_id指向更小的值
        if (entry_id < entry_id_ptr->spare_id)
        {
            entry_id_ptr->spare_id = entry_id;
        }
    }
}

/*****************************************************************************/
//  Description : get contact email
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: group
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsContactHasMail(
                      MMIPB_CONTACT_LIST_NODE_T node
                     )
{
    BOOLEAN result = FALSE;
#ifdef MMIPB_MAIL_SUPPORT
    if(MMIPB_IsPhoneContact(ShortIDToStorageID(node.short_storage_id)))
    {
        if(node.index < s_pb_phone_contact_list.count_count)
        {
            if(s_pb_phone_contact_list.contact_table[node.index].mail_str.wstr_len > 0
              &&s_pb_phone_contact_list.contact_table[node.index].mail_str.wstr_len < MMIPB_EMPTY_STRING_FLAG   
              )
            {
                result = TRUE;
            }
        }


#ifndef MMIPB_MEMPOOL_SUPPORT
        else
        {
            MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        
            if(contact_ptr != PNULL)
            {
                MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
                MMIPB_CONTACT_T *contact_ptr = PNULL; 
                MMIPB_ACCESS_POINT_T access_point = {0};
        
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = node.contact_id;
                access_point.contact.storage_id =ShortIDToStorageID(node.short_storage_id);
                ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == ret)
                {
                    if(contact_ptr->mail.wstr_len>0)
                    {
                        result = TRUE;
                    }
                }
                SCI_FREE(contact_ptr);
            }
        }      
#endif
    }
#ifdef CMCC_VCARD_SUPPORT
        else if(MMIPB_IsUsim(ShortIDToStorageID(node.short_storage_id)))
        {
            if(node.num_mail_list != PNULL && node.num_mail_list->mail_str.wstr_len > 0)
            {
                result = TRUE;
            }
        }
#endif 
#endif 
    return result;
}

/*****************************************************************************/
//  Description : get num_type
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: num_type
//  Note: 
/*****************************************************************************/
LOCAL void GetContactNumType(
                      MMIPB_CONTACT_LIST_NODE_T node,
                      uint8 num_type[],//OUT
                      uint8 type_count
                     )
{

    if(num_type == PNULL || type_count == 0)
    {
        return;
    }
    if(MMIPB_IsPhoneContact(ShortIDToStorageID(node.short_storage_id)))
    {
        if(node.index < s_pb_phone_contact_list.count_count)
        {
            SCI_MEMCPY(num_type, s_pb_phone_contact_list.contact_table[node.index].num_type, sizeof(uint8)*type_count);
        }


#ifndef MMIPB_MEMPOOL_SUPPORT
        else
        {
            MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
                MMIPB_ACCESS_POINT_T access_point = {0};
        
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = node.contact_id;
                access_point.contact.storage_id =ShortIDToStorageID(node.short_storage_id);
                ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == ret)
                {
                    SCI_MEMCPY(num_type, contact_ptr->num_type, sizeof(uint8)*type_count);
                }
                SCI_FREE(contact_ptr);
            }
        }
#endif
    }
	//get num_type if is sim record, for Bug 113812
    else if (MMIPB_IsSimRecord(ShortIDToStorageID(node.short_storage_id)))
    {
		MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
		SCI_TRACE_LOW("xiaohua GetContactNumType is sim contact\n");
		if(contact_ptr != PNULL)
		{
			MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
			MMIPB_ACCESS_POINT_T access_point = {0};
			
			SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
			access_point.contact.contact_id = node.contact_id;
			access_point.contact.storage_id =ShortIDToStorageID(node.short_storage_id);
			ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
				access_point,
				sizeof(MMIPB_CONTACT_T),
				contact_ptr
				);
			if(MMIPB_ERROR_SUCCESS == ret)
			{
				SCI_MEMSET(num_type, 0X00, type_count*sizeof(uint8));
				SCI_MEMCPY(num_type, contact_ptr->num_type, sizeof(uint8));
			}
			SCI_FREE(contact_ptr);
		}
	}
#ifdef CMCC_VCARD_SUPPORT
    if(MMIPB_IsUsim(ShortIDToStorageID(node.short_storage_id)))
    {
        MMIPB_BCD_NUMBER_LIST_T*	number_list = PNULL;
        uint8 						num_index = 1;

        SCI_TRACE_LOW("xiaohua GetContactNumType is usim contact\n");
        if(node.num_mail_list != PNULL)
        {
            number_list = node.num_mail_list->number_list;
            while(num_type[num_index]>0&&num_index < type_count)
            {
                num_index++;
            }   
        }
        SCI_TRACE_LOW("xiaohua GetContactNumType number_list:%d num_index %d\n",number_list,num_index);

        while(number_list != PNULL && num_index < type_count)
        {
            num_type[num_index] = number_list->num_type;
            number_list = number_list->next_ptr;
            num_index++;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : change storage id to short storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint8 StorageIDToShortID(
                         uint16 storage_id
                         )
{
    uint8 shor_id = (uint8)((storage_id &0x0F00) >> 4) + (storage_id &0xF);

    return shor_id;
   
}

/*****************************************************************************/
//  Description : change short storage id to storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint16 ShortIDToStorageID(
                         uint8 short_storage_id
                         )
{
    uint16 storage_id = ((short_storage_id & 0xF0) << 4) + (short_storage_id &0x0F);

    return storage_id;
   
}

/*****************************************************************************/
//  Description : get contacts number in pb
//  Parameter: [In] None
//             [Out] None
//             [Return] count
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 WatchPB_SearchPresetContact()
{
    MMI_STRING_T string = {0};
    uint32 exclude_group = MMIPB_GetAllSIMGroup();
    uint32 count =MMIPB_MultiSearch(&string, MMIPB_ALL_GROUP_MASK, exclude_group, MMIPB_SEARCH_NAME);

    SCI_TRACE_LOW("WatchPB_SearchPresetContact count = %d", count);
    count = count >= WATCHPB_CONTACT_NUM_MAX ?  WATCHPB_CONTACT_NUM_MAX : count;
    SCI_TRACE_LOW("WatchPB_SearchPresetContact limit count  = %d", count);
    return count;
}

/*****************************************************************************/
//  Description : get contact array by name
//  Parameter: [In] idx_array
//             [Out] idx_array
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_GetPresetContactArray(uint32* idx_array)
{
    uint32 i = 0;
    uint32 index = 0;
    if (idx_array == NULL)
    {
        return;
    }
    for (i = 0; i < WATCHPB_CONTACT_NUM_MAX; i++)
    {
        if (s_pb_contact_list.list[i].visible_flag != 0)
        {
            idx_array[index] = i;
            index++;
        }
    }
}

/*****************************************************************************/
//  Description : get contact info by index
//  Parameter: [In] index
//             [Out] contact info
//             [Return] success or failed
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchPB_GetPresetContactInfoByIndex(uint32 index, MMIPB_CONTACT_T *info)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;
    MMIPB_ACCESS_POINT_T access_point = {0};
    uint32 storage_id = 0;
    uint32 entry_id = 0;
    if (info == NULL)
    {
        return FALSE;
    }
    entry_id = s_pb_contact_list.list[index].contact_id;
    storage_id = ShortIDToStorageID(s_pb_contact_list.list[index].short_storage_id);

    access_point.contact.contact_id = entry_id;
    access_point.contact.storage_id = storage_id;
    result = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    info);

    return (result == MMIPB_ERROR_SUCCESS);
}


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
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 storage_id = 0;
    uint16 cache_index = 0;
    MMI_STRING_T name_str = {0};
#ifdef MMIPB_MAIL_SUPPORT
    MMI_STRING_T mail_str = {0};
#endif
    MMIPB_CONTACT_T *contact_ptr = PNULL; 
    MMIPB_ACCESS_POINT_T access_point = {0};
   
    if((PNULL == cmp_str))
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetContactNameNumberByIndex invalid param "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_382_112_2_18_2_43_19_175,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }  
    //SCI_TRACE_LOW("[MMIPB]MMIPB_GetContactNameNumberByIndex pos %d",pos);
    
    if(s_pb_contact_list.list != PNULL && pos < s_pb_contact_list.list_info.count)
    {
        storage_id = ShortIDToStorageID(s_pb_contact_list.list[pos].short_storage_id);

        cache_index = s_pb_contact_list.list[pos].index;
        //read name
        if(MMIPB_IsPhoneContact(storage_id) && (cache_index < s_pb_phone_contact_list.count_count))
        {
            name_str = s_pb_phone_contact_list.contact_table[cache_index].name_str;
            SCI_MEMCPY(&cmp_str->number_t, &s_pb_phone_contact_list.contact_table[cache_index].number, sizeof(cmp_str->number_t));

#ifdef MMIPB_MAIL_SUPPORT
            mail_str = s_pb_phone_contact_list.contact_table[cache_index].mail_str;
#endif
            cmp_str->group = s_pb_contact_list.list[pos].group;
        }

#ifdef SNS_SUPPORT
        else if(MMIPB_IsSNSContact(storage_id) &&(cache_index < s_pb_sns_contact_list.current_count))
        {
            name_str = s_pb_sns_contact_list.contact_table[cache_index].name_str;
        }
#endif

        if((MMIPB_EMPTY_STRING_FLAG != name_str.wstr_len && name_str.wstr_ptr == PNULL)           
#ifdef MMIPB_MAIL_SUPPORT
            &&(MMIPB_EMPTY_STRING_FLAG != mail_str.wstr_len && mail_str.wstr_ptr == PNULL)
#endif
            )
        {
            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = s_pb_contact_list.list[pos].contact_id;
                access_point.contact.storage_id = storage_id;
                result = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == result)
                {
                    SCI_MEMCPY(&cmp_str->name, &contact_ptr->name, sizeof(cmp_str->name));
                    SCI_MEMCPY(&cmp_str->number_t, contact_ptr->number, sizeof(cmp_str->number_t));
#ifdef MMIPB_MAIL_SUPPORT
                    SCI_MEMCPY(&cmp_str->mail, &contact_ptr->mail, sizeof(cmp_str->mail));
#endif
                    cmp_str->group = contact_ptr->union_t.group;
                     result = MMIPB_ERROR_SUCCESS;
                }
                SCI_FREE(contact_ptr);
            }
           
        }
        else
        {
            if(name_str.wstr_len > 0 && name_str.wstr_ptr != PNULL)
            {
                cmp_str->name.wstr_len = MIN(name_str.wstr_len, MMIPB_NAME_MAX_LEN);
                SCI_MEMCPY(&cmp_str->name.wstr, name_str.wstr_ptr, sizeof(wchar)*cmp_str->name.wstr_len);               
            }
#ifdef MMIPB_MAIL_SUPPORT
            if(mail_str.wstr_len > 0 && mail_str.wstr_ptr != PNULL)
            {
                cmp_str->mail.wstr_len = MIN(mail_str.wstr_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMCPY(&cmp_str->mail.wstr, mail_str.wstr_ptr, sizeof(wchar)*cmp_str->mail.wstr_len);               
            }
#endif
            result = MMIPB_ERROR_SUCCESS;
        }
    }
    return (result);
}

/*****************************************************************************/
//  Description :调整记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E AdjustContactListInfo(
                                          MMIPB_CONTACT_BASE_INFO_T *base_info_ptr,//[IN]
                                          BOOLEAN is_increase,
                                          MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                                          )
{
    
    uint16 i = 0;
    uint32 group_val = 0;
    uint16 num_count = 0;
    uint16 mail_count = 0;

    if(data_ptr == PNULL || base_info_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //计算number个数
    for (i=0; i<MMIPB_MAX_NV_PHONE_NUM; i++)
    {
        if (base_info_ptr->num_type[i] != MMIPB_INVLID_NUMBER)
        {
            num_count++;
        }
    }
    
    // 计算邮箱的个数
    if (base_info_ptr->mail_exist)
    {
        mail_count++;
    }    
    //调整每个存储设备中的联系人数量
    if(MMIPB_GET_STORAGE(base_info_ptr->storage_id) < MMIPB_STORAGE_MAX)
    {
        if(is_increase)
        {
            data_ptr->storage_count[MMIPB_GET_STORAGE(base_info_ptr->storage_id)]++;
        }
        else if(data_ptr->storage_count[MMIPB_GET_STORAGE(base_info_ptr->storage_id)] > 0)
        {
            data_ptr->storage_count[MMIPB_GET_STORAGE(base_info_ptr->storage_id)]--;
        }
    }
#ifdef TCARD_PB_SUPPORT	
    //adjust tcard contact count
    if(MMIPB_IsTCardContact(base_info_ptr->storage_id, base_info_ptr->contact_id))
    {
        if(is_increase)
        {
            data_ptr->tcontact_count++;
        }
        else if(data_ptr->tcontact_count > 0)
        {
            data_ptr->tcontact_count--;
        }
    }
#endif  

    //调整总号码个数和mail个数
    if(is_increase)
    {
        data_ptr->mail_count += mail_count;
        data_ptr->number_count += num_count;
    }
    else
    {
        if(data_ptr->mail_count >= mail_count)
        {
            data_ptr->mail_count -= mail_count;
        }
        if(data_ptr->number_count >= num_count)
        {
            data_ptr->number_count -= num_count;
        }
    }
    for(i =0; i < MMIPB_MAX_GROUP_NUM; i++)
    {
        group_val = (1 << i );
        if(group_val > base_info_ptr->group)
        {
            break;
        }
        
        if((group_val & base_info_ptr->group))
        {
            //调整各个分组的联系人数量，号码个数和mail个数
            if(is_increase)
            {
                data_ptr->group_count[i]++;
                data_ptr->group_mail_count[i] += mail_count;
                data_ptr->group_number_count[i] += num_count;
            }
            else
            {
                if(data_ptr->group_count[i] > 0)
                {
                    data_ptr->group_count[i]--;
                }
                if(data_ptr->group_mail_count[i] >= mail_count)
                {
                    data_ptr->group_mail_count[i] -= mail_count;
                }
                if(data_ptr->group_number_count[i] >= num_count)
                {
                    data_ptr->group_number_count[i] -= num_count;
                }
            }
        }
    }
    return MMIPB_ERROR_SUCCESS;
}


/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListCmp(uint32 base_index, void *compare_data, void *list)
{
  MMIPB_QSORT_LIST_T  *qsort_list = (MMIPB_QSORT_LIST_T * )list;
  //MMIPB_NAME_T         name     = {0};
  MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
  MMI_STRING_T       *input_str = (MMI_STRING_T *)compare_data;
  MMIPB_NAME_NUMBER_MAIL_T *cmp_str_ptr = PNULL;
  int ret = 0;
  BOOLEAN  is_char = TRUE;
  
  letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
  if(PNULL == letters_ptr)
  {
      return -1;
  }
  if(PNULL == qsort_list || PNULL == qsort_list->list)
  {
      //SCI_TRACE_LOW:"CompareLettersInSortTable valid parameter !!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_568_112_2_18_2_43_19_176,(uint8*)"");

      SCI_FREE(letters_ptr);
      return -1;
  }
  cmp_str_ptr = (MMIPB_NAME_NUMBER_MAIL_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));
  if(cmp_str_ptr != PNULL)
  {
      SCI_MEMSET(cmp_str_ptr, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
      MMIPB_GetContactNameNumberByIndex(cmp_str_ptr, (qsort_list->list[base_index] &MMIPB_CONTACT_INDEX_MASK));  
      //get name_in_list_t 第range_index个character对应的PinYin sn
      SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
      if(qsort_list->list[base_index] >= MMIPB_KEY_HEAD_FLAG)
      {
          //key value
          is_char = FALSE;
      }
      else
      {
          is_char = TRUE;
      }
      MMIPB_GetNameLetters(&cmp_str_ptr->name, MMIPB_NAME_HEAD_CHAR, is_char, letters_ptr);
      ret = MMIAPICOM_Wstrcmp(letters_ptr->head_letters, input_str->wstr_ptr);
      SCI_FREE(cmp_str_ptr);
  }
  
  //SCI_TRACE_LOW("QSortListCmp ret %d ",ret);
  SCI_FREE(letters_ptr);
  return ret;
}

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListNCmp(uint32 base_index, void *compare_data, void *list)
{
  MMIPB_QSORT_LIST_T  *qsort_list = (MMIPB_QSORT_LIST_T * )list;
  //MMIPB_NAME_T         name     = {0};
  MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
  MMIPB_QSORT_CMP_DATA_T  *cmp_data_ptr = (MMIPB_QSORT_CMP_DATA_T *)compare_data;
  //wchar               *name_str = PNULL;
  MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
  int ret = 0;

  if(cmp_data_ptr == PNULL || cmp_data_ptr->str_ptr == PNULL)
  {
        return -1;
  }
  letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
  if(PNULL == letters_ptr)
  {
      return -1;
  }
  if(PNULL == qsort_list || PNULL == qsort_list->list)
  {
      //SCI_TRACE_LOW:"CompareLettersInSortTable valid parameter !!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_615_112_2_18_2_43_19_177,(uint8*)"");
      SCI_FREE(letters_ptr);
      return -1;
  }
  MMIPB_GetContactNameNumberByIndex(&cmp_str, (qsort_list->list[base_index] & MMIPB_CONTACT_INDEX_MASK));  
  //get name_in_list_t 第range_index个character对应的PinYin sn
  SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
  if(qsort_list->list[base_index] >= MMIPB_KEY_HEAD_FLAG)
  {
      MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_HEAD_CHAR, FALSE, letters_ptr);          
  }
  else
  {       
      MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_HEAD_CHAR, TRUE, letters_ptr);
  }
  ret = MMIAPICOM_Wstrncmp(letters_ptr->head_letters, cmp_data_ptr->str_ptr->wstr_ptr, cmp_data_ptr->str_ptr->wstr_len);
  
  //SCI_TRACE_LOW("QSortListCmp ret %d ",ret);
  SCI_FREE(letters_ptr);
  return ret;
}

/*****************************************************************************/
//  Description :根据字符串在快速查找表中查找对应的index
//str_input:需要查找的字符串
//is_back: TRUE:查找最后一个匹配的位置;FALSE:查找第一个匹配的位置
// 返回匹配的位置
/*****************************************************************************/
LOCAL uint16 GetQSortListIndex
  (
  MMI_STRING_T *str_input,//IN
  BOOLEAN      is_char,//IN
  BOOLEAN      is_back
  )
{
  MMI_BIN_SEARCH_INFO_T search_info = {0};
  uint32                  ret_pos = 0;
  BIN_COMPARE_FUNC        func = PNULL;
  MMIPB_QSORT_CMP_DATA_T cmp_data = {0};
  
  if((PNULL!= s_pb_qsort_list.list) 
      && (s_pb_qsort_list.count > 0))
  {
      //use binary search
      if(is_back)
      {
          //查找最后一个相等的记录
          search_info.search_type = MMICOM_SEARCH_LAST_EQUAL;
      }
      else
      {
          //查找第一个相等的记录
          search_info.search_type = MMICOM_SEARCH_FIRST_EQUAL;
      }

      cmp_data.is_char = is_char;
      cmp_data.str_ptr = str_input;
      search_info.start_pos = 0;
      search_info.end_pos = s_pb_qsort_list.count-1;
      search_info.compare_base_data = &cmp_data;
      //find matched string in sort list table
      func = QSortListNCmp;
      if(MMIAPICOM_BinSearch
          (&search_info, 
          (BIN_COMPARE_FUNC)func,
          &ret_pos,
          &s_pb_qsort_list
          ))
      {
          ret_pos = PB_NOT_EXIST;
      }
  }
  //SCI_TRACE_LOW("[MMIPB]QSortListSearch ret_pos %d", ret_pos);
  return (uint16)ret_pos;
} 
                                         
/*****************************************************************************/
//  Description : 初始化数据表
//  
// 
/*****************************************************************************/
PUBLIC void MMIPB_InitDataList(void)
{

  //uint32 sort_content_size = 0;
  MMIPB_STORAGE_INFO_T storage_info_t = {0};
  //nv init
  storage_info_t = MMIPB_GetContactStorage(MMI_DUAL_SYS_MAX, PHONEBOOK_NV_STORAGE);
  //total count
  //contact list size 
  //malloc contact list memory and init it
  /*
  if(s_pb_phone_contact_list.contact_table != PNULL)
  {
      SCI_FREE(s_pb_phone_contact_list.contact_table);
  }
  */

  if(s_pb_phone_contact_list.contact_table == PNULL)
  {
        s_pb_phone_contact_list.max_count = storage_info_t.max_record_num;
#ifdef MMIPB_MEMPOOL_SUPPORT
       s_pb_phone_contact_list.contact_table = SCI_ALLOC_CONST(sizeof(MMIPB_PHONE_CONTACT_NODE_T)* s_pb_phone_contact_list.max_count);
#else
       s_pb_phone_contact_list.contact_table = SCI_ALLOCA(sizeof(MMIPB_PHONE_CONTACT_NODE_T)* s_pb_phone_contact_list.max_count);
#endif
  }

  if(s_pb_phone_contact_list.contact_table != PNULL)
  {
          SCI_MEMSET(s_pb_phone_contact_list.contact_table, 
                    0x00, 
                    sizeof(MMIPB_PHONE_CONTACT_NODE_T)* (s_pb_phone_contact_list.max_count));
  }
  if(PNULL == s_pb_phone_contact_id_table.valid_flag)
  {
      s_pb_phone_contact_id_table.valid_flag= (uint8 *)SCI_ALLOCA(((storage_info_t.max_record_num + 7) / 8 ));
      s_pb_phone_contact_id_table.max_record_num = storage_info_t.max_record_num;
  }
  SCI_MEMSET(s_pb_phone_contact_id_table.valid_flag,0, ((s_pb_phone_contact_id_table.max_record_num+ 7) / 8 ));
  s_pb_phone_contact_id_table.spare_id = 1;
#ifdef MMIPB_MOST_USED_SUPPORT
  //常用联系人列表 
  if(PNULL == s_pb_mostused_contact_list.contact_table)
  {
      s_pb_mostused_contact_list.contact_table = (MMIPB_MOSTUSED_CONTACT_NODE_T *)SCI_ALLOC_CONST(MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS *sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
  }
  if(PNULL != s_pb_mostused_contact_list.contact_table)
  {
        SCI_MEMSET(s_pb_mostused_contact_list.contact_table,0, MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS *sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
  }
  //常用联系人id使用情况表
  if(PNULL == s_pb_mostused_contact_id_table.valid_flag)
  {
      s_pb_mostused_contact_id_table.valid_flag= (uint8 *)SCI_ALLOCA(((MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS + 7) / 8 ));
  }
  if(PNULL != s_pb_mostused_contact_id_table.valid_flag)
  {
        SCI_MEMSET(s_pb_mostused_contact_id_table.valid_flag,0, ((MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS + 7) / 8 ));
  }
  s_pb_mostused_contact_id_table.max_record_num = MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;
  s_pb_mostused_contact_id_table.spare_id = 1;
  s_pb_mostused_contact_list.count = 0;
  #endif
  //初始化总索引表，并分配空间,快速查找表和sns缓存表 
  s_pb_contact_list.list_info.count = 0;
  SCI_MEMSET(&s_pb_contact_list.list_info, 0x00, sizeof(MMIPB_CONTACT_LIST_INFO_T));
  s_pb_qsort_list.count = 0;
  s_pb_phone_contact_list.count_count = 0;
#ifdef SNS_SUPPORT
  s_pb_sns_contact_list.current_count = 0;
#endif

  //MMIPB_InitNumberList((MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX + (storage_info_t.max_record_num* MMIPB_MAX_NV_PHONE_NUM)));
}
#ifndef MMIPB_MEMPOOL_SUPPORT
/*****************************************************************************/
//  Description :释放string cache
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_FreeCache(void)
{
    SCI_FREE(s_pb_phone_contact_list.contact_table);
    s_pb_phone_contact_list.contact_table = PNULL;
    s_pb_phone_contact_list.count_count = 0;
#ifdef SNS_SUPPORT
    SCI_FREE(s_pb_sns_contact_list.contact_table);
    s_pb_sns_contact_list.contact_table = PNULL;
    s_pb_sns_contact_list.current_count= 0;
#endif
}
#endif
/*****************************************************************************/
//  Description : 分配索引表空间和快速查找表空间
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_ALLOCContactList(void)
{

  uint32  item_count = 0;
#ifdef SNS_SUPPORT
  uint32  sns_count = MMIPB_SNS_MAX_RECORD_NUM;//MMIPB_GetSNSContactsCount(0);
#endif
  MMIPB_STORAGE_INFO_T storage_info_t = MMIPB_GetContactStorage(MMI_DUAL_SYS_MAX, PHONEBOOK_NV_STORAGE);
#ifdef SNS_SUPPORT
  item_count = (MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX) + storage_info_t.max_record_num + sns_count;
#else
    item_count = (MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX)+storage_info_t.max_record_num;
#endif

  //快速查找表的初始化和内存分配
  s_pb_qsort_list.list = (uint16 *)SCI_ALLOC_CONST(item_count *2*sizeof(uint16));
  if(s_pb_qsort_list.list != PNULL)
  {
      SCI_MEMSET(s_pb_qsort_list.list, 0x00, item_count*2 *sizeof(uint16));
      s_pb_qsort_list.max_count = item_count*2;
  }      
  
  //重新分配索引表 

  s_pb_contact_list.list = (MMIPB_CONTACT_LIST_NODE_T *)SCI_ALLOC_CONST(item_count *sizeof(MMIPB_CONTACT_LIST_NODE_T));
  if(s_pb_contact_list.list != PNULL)
  {
      SCI_MEMSET(s_pb_contact_list.list, 0x00, item_count*sizeof(MMIPB_CONTACT_LIST_NODE_T));
      s_pb_contact_list.max_count = item_count;
  }
  SCI_MEMSET(&s_pb_contact_list.list_info, 0x00, sizeof(MMIPB_CONTACT_LIST_INFO_T));
#ifdef SNS_SUPPORT
  //重新分配sns缓存表

  s_pb_sns_contact_list.contact_table = (MMIPB_SNS_CONTACT_NODE_T *)SCI_ALLOC_CONST(sizeof(MMIPB_SNS_CONTACT_NODE_T)*sns_count);
  if(s_pb_sns_contact_list.contact_table != PNULL)
  {
      SCI_MEMSET(s_pb_sns_contact_list.contact_table, 
          0x00, sizeof(MMIPB_SNS_CONTACT_NODE_T)*sns_count);
      s_pb_sns_contact_list.max_count = sns_count;
  }
  else
  {
      s_pb_sns_contact_list.max_count = 0;
  }
  s_pb_sns_contact_list.current_count = 0;
#endif
}
/*****************************************************************************/
//  Description : 创建数据表的访问控制信号量
//  
// 
/*****************************************************************************/
PUBLIC void MMIPB_CreateDataListSemphore(void)
{
    s_contact_list_semphore_p = SCI_CreateSemaphore("s_contact_list_semphore_p", 1);
#ifdef MMIPB_MOST_USED_SUPPORT    
    s_mostusedcontext_list_semphore_p = SCI_CreateSemaphore("s_mostusedcontext_list_semphore_p", 1);
#endif    
    s_qsort_list_semphore_p = SCI_CreateSemaphore("s_qsort_list_semphore_p", 1);    
}
/*****************************************************************************/
//  Description : 获取NV contact id的分配情况。
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfNV(uint8 *valid_buf, 
                                           uint8 buf_len
                                           )
{
    if(valid_buf != PNULL && buf_len > 0)
    {
        SCI_MEMCPY(valid_buf, s_pb_phone_contact_id_table.valid_flag, buf_len*sizeof(uint8));
    }
}

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
                                           )
{
    uint32 i = 0;
    uint16 storage_id = 0;
    MMIPB_ENTRY_ID_T   entry_id_flag = {0};

    if(valid_buf != PNULL && max_count > 0 && dual_sys < MMI_DUAL_SYS_MAX)
    {
        entry_id_flag.spare_id = 1;
        entry_id_flag.max_record_num = max_count;
        entry_id_flag.valid_flag = valid_buf;
        for(i =0; i < s_pb_contact_list.list_info.count; i++)
        {
            storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
            if(MMIPB_IsSIMContact(storage_id) && dual_sys == MMIPB_GetDualSysByStorageId(storage_id))
            {
               SetEntryIdFlag(TRUE, s_pb_contact_list.list[i].contact_id, &entry_id_flag);
            }
        }
    }
}
/*****************************************************************************/
//  Description : Find visiable contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindContact(
                                       uint16 start_index,  
                                       uint32 group_mask,//[IN]所有要查找的群组
                                       uint16 storage_id, //要查找的联系人类型                                        
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 i = 0;
    uint16 j = 0;
    
    //get a visibile contact from start_index
    if(valid_index_array != PNULL)
    {
        //初始化有效号码索引和email数组
        SCI_MEMSET(valid_index_array->visible_num_index, 0x0, sizeof(valid_index_array->visible_num_index));
        valid_index_array->is_mail_visible = FALSE;
    }
    if(MMIPB_IsSIMContact(storage_id) 
        || MMIPB_IsPhoneContact(storage_id)
#ifdef SNS_SUPPORT        
        || MMIPB_IsSNSContact(storage_id) 
#endif
        )
    {
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
        if(s_pb_contact_list.list != PNULL)
        {
            for(i = start_index; i < s_pb_contact_list.list_info.count; i++)
            {
                if(( ((1<< PB_GROUP_ALL) &group_mask) || group_mask & s_pb_contact_list.list[i].group)//全部分组或包含要检索的分组
                    && (s_pb_contact_list.list[i].visible_flag > 0)//可见
#ifdef BT_PBAP_SUPPORT
                    && (!MMIPB_IsBTContact(s_pb_contact_list.list[i].contact_id, ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
#endif
                    )
                {
                    //如果要检索的分组是全部分组或该记录分组包含要检索的分组，并且该记录可见  //，且非蓝牙联系人
                    result = MMIPB_ERROR_SUCCESS;
                    find_data_ptr->contact_id = s_pb_contact_list.list[i].contact_id;
                    find_data_ptr->first_char = s_pb_contact_list.list[i].first_char;
                    find_data_ptr->group = s_pb_contact_list.list[i].group;
                    find_data_ptr->index = i;
                    find_data_ptr->mail_exist = IsContactHasMail(s_pb_contact_list.list[i]);
                    find_data_ptr->storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
#ifdef BT_PBAP_SUPPORT
                    find_data_ptr->is_bt_contact = FALSE;
#endif
                    GetContactNumType(s_pb_contact_list.list[i], find_data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
                    if(find_index != PNULL)
                    {
                        //set find index
                        *find_index = i; 
                    }
                    //get visible number index
                    for(j =0; j < MMIPB_MAX_NV_PHONE_NUM; j++)
                    {
                        
                        if(s_pb_contact_list.list[i].visible_flag < (1 << j))
                        {
                            break;
                        }
                        if(valid_index_array != PNULL && find_data_ptr->num_type[j] == MMIPB_INVLID_NUMBER)
                        {
                            //                            if(valid_index_array != PNULL)
                            {
                                valid_index_array->visible_num_index[j] = 0;
                            }
                        }
                        else if(s_pb_contact_list.list[i].visible_flag & (1 << j))
                        {
                            //该号码可见; this number is visible
                            if(valid_index_array != PNULL)
                            {
                                valid_index_array->visible_num_index[j] = 1;
                            }
                        }
                        
                    }
                    //set  visible 
                    if(valid_index_array != PNULL)
                    {
                        //该mail可见；this mail is visible
                        if(IsContactHasMail(s_pb_contact_list.list[i]) && (s_pb_contact_list.list[i].visible_flag &MMIPB_FEILD_FLAG_MASK_MAIL))
                        {
                            valid_index_array->is_mail_visible = TRUE;
                        }
                    }
                    break;
                }
            }
        }
        //SCI_RestoreIRQ();
        SCI_PutSemaphore(s_contact_list_semphore_p);
    }
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)    
    else
    {
        //other contact list
        result = FindDailNumberContact(start_index, storage_id, find_index, valid_index_array, find_data_ptr);
    }
#endif    
    //SCI_TRACE_LOW("MMIPB_FindContact start_index %d group_mask %d  result %d",start_index, group_mask,  result);
    return result;
}

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : Find dail number contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: msisdn, fdn, sdn...
/*****************************************************************************/
LOCAL MMIPB_ERROR_E FindDailNumberContact(
                                       uint16 start_index,   
                                       uint16 storage_id, //要查找的联系人类型
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;    
    uint16 i =0;
    uint16 j =0;
    MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(storage_id);
    MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    MMIPB_ACCESS_POINT_T access_point = {0};

    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] FindDailNumberContact PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_981_112_2_18_2_43_20_178,(uint8*)"");
        return MMIPB_ERROR_ERROR;
    }
    for(i = start_index; i < storage_info.max_record_num; i++)
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
        access_point.contact.contact_id = i + 1;
        access_point.contact.storage_id = storage_id;
        if(MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_CONTACT,
            access_point,
            sizeof(MMIPB_CONTACT_T),
            contact_ptr
            ))
        {
            if(MMIPB_DailNumberContactCanbeEmpty(storage_id)
              ||MMIPB_IsContactValid(contact_ptr)  
              )
            {
                //find contact
                if(find_index != PNULL)
                {
                    //set index
                   *find_index = i;                   
                }
                if(find_data_ptr != PNULL)
                {
                    //set find contact base information
                    find_data_ptr->contact_id = access_point.contact.contact_id;
                    find_data_ptr->storage_id = storage_id;
                    find_data_ptr->index = i;
                    find_data_ptr->group = (1<<(PB_GROUP_SIM_1 + MMIPB_GetDualSysByStorageId(storage_id)));
                    find_data_ptr->visible_flag = 1;
#ifdef MMIPB_MAIL_SUPPORT
                    find_data_ptr->mail_exist = (contact_ptr->mail.wstr_len > 0 ? TRUE : FALSE);
#else
                    find_data_ptr->mail_exist = FALSE;
#endif
                    SCI_MEMCPY(find_data_ptr->num_type, contact_ptr->num_type, sizeof(find_data_ptr->num_type));
                }
                if(valid_index_array != PNULL)
                {
                    //set number or mail valid flag
                    for(j =0; j < MMIPB_MAX_NV_PHONE_NUM; j++)
                    {
                        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[j].npi_ton, contact_ptr->number[j].number_len))
                        {
                            valid_index_array->visible_num_index[j] = 1;
                        }
                    }
#ifdef MMIPB_MAIL_SUPPORT
                    if(contact_ptr->mail.wstr_len > 0)
                    {
                        valid_index_array->is_mail_visible = TRUE;
                    }
#else
                    valid_index_array->is_mail_visible = FALSE;
#endif
                }
                result = MMIPB_ERROR_SUCCESS;
                break;
            }
        }
        else if(MMIPB_DailNumberContactCanbeEmpty(storage_id))
        {
            //无效记录也需要显示
            result = MMIPB_ERROR_SUCCESS;
            if(find_index != PNULL)
            {
                //set index
                *find_index = i;                   
            }
            if(find_data_ptr != PNULL)
            {
                //set find contact base information
                find_data_ptr->contact_id = access_point.contact.contact_id;
                find_data_ptr->storage_id = storage_id;
                find_data_ptr->index = i;
                find_data_ptr->group = (1<<(PB_GROUP_SIM_1 + MMIPB_GetDualSysByStorageId(storage_id)));
                find_data_ptr->visible_flag = 1;
                find_data_ptr->mail_exist = 0;//(contact_ptr->mail.wstr_len > 0 ? TRUE : FALSE);
                SCI_MEMSET(find_data_ptr->num_type, 0x00, sizeof(find_data_ptr->num_type));
            }
            break;
        }
    }
    SCI_FREE(contact_ptr);

    return result;
}

/*****************************************************************************/
//  Description : check current dail number contact can be empty
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DailNumberContactCanbeEmpty(uint16 storage_id)
{
     BOOLEAN result = FALSE;
#if defined (MMIPB_SIMDN_SUPPORT) 
     if(MMIPB_IsMsisdnContact(storage_id))
     {
         result = TRUE;
     }
#endif     
     return result;
}
#endif
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
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 i = 0;
    //uint16 j = 0;
    
    //get a visibile contact from start_index
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER);
    if(s_pb_mostused_contact_list.contact_table != PNULL)
    {
        for(i = start_index; i < s_pb_mostused_contact_list.count; i++)
        {
            //if(s_pb_mostused_contact_list.visibility_table[i] > 0)
            {
                //find a visible contact
                if(find_index != PNULL)
                {
                    //set find index
                    *find_index = i; 
                }
                result = MMIPB_ERROR_SUCCESS;
                if(find_data_ptr != PNULL)
                {
                    *find_data_ptr = s_pb_mostused_contact_list.contact_table[i];
                }
                break;
            }
        }
    }
    //SCI_TRACE_LOW("MMIPB_FindMostUsedContact start_index %d   result %d",start_index, result);
    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p);
    //SCI_RestoreIRQ();
    return result;
}
#endif
/*****************************************************************************/
//  Description : 读记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactList(
                                           MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                                           )
{
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    *data_ptr = s_pb_contact_list.list_info;
    //SCI_RestoreIRQ();
    
    return MMIPB_ERROR_SUCCESS;
}
#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : FDN有效记录的个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFdnContactCount(uint16 storage_id)
{
    uint16 count = 0;
#ifdef FDN_SUPPORT
    MMIFDN_CONTEXT_T *fdn_context_ptr = MMIAPIFDN_GetFDNContext(MMIPB_GetDualSysByStorageId(storage_id));

    if(fdn_context_ptr != PNULL)
    {
        count = fdn_context_ptr->fdn_info.used_record_num;
    }
#endif
    return count;
}
#endif

/*****************************************************************************/
//  Description : sim卡特殊号码的个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSIMDNContactCount(uint16 storage_id)
{

    MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetDualSysByStorageId(storage_id));
    return storage_info.used_record_num;
}

/*****************************************************************************/
//  Description : 获取该联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactSimpleInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        data_ptr->group = s_pb_contact_list.list[index].group;//GetContactGroup(s_pb_contact_list.list[index]);
        data_ptr->contact_id = s_pb_contact_list.list[index].contact_id;
        data_ptr->first_char = s_pb_contact_list.list[index].first_char;
        data_ptr->index = s_pb_contact_list.list[index].index;
        data_ptr->storage_id = ShortIDToStorageID(s_pb_contact_list.list[index].short_storage_id);
        data_ptr->visible_flag = s_pb_contact_list.list[index].visible_flag;
        GetContactNumType(s_pb_contact_list.list[index], data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
        data_ptr->mail_exist = IsContactHasMail(s_pb_contact_list.list[index]);
#ifdef BT_PBAP_SUPPORT
        data_ptr->is_bt_contact = s_pb_contact_list.list[index].is_bt_contact;
#endif
        result = MMIPB_ERROR_SUCCESS;
    }
    //SCI_RestoreIRQ();
    
    return result;
}

/*****************************************************************************/
//  Description : 获取手机联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadPhoneContactBaseInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16        i  = 0;
    uint16        phone_index  = 0;
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    for(i = 0; i < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL; i++)
    {
        if(MMIPB_IsPhoneContact(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
        {
            if(index == phone_index)
            {
                if(data_ptr != PNULL)
                {
                    data_ptr->group = s_pb_contact_list.list[i].group;//GetContactGroup(s_pb_contact_list.list[index]);
                    data_ptr->contact_id = s_pb_contact_list.list[i].contact_id;
                    data_ptr->first_char = 0;
                    data_ptr->index = index;
                    data_ptr->storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                    data_ptr->visible_flag = 0xFF;
#ifdef BT_PBAP_SUPPORT
                    data_ptr->is_bt_contact = s_pb_contact_list.list[i].is_bt_contact;
#endif
                    if(index < s_pb_phone_contact_list.count_count && s_pb_phone_contact_list.contact_table != PNULL)
                    {
                        //GetContactNumType(s_pb_contact_list.list[index], data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
                        SCI_MEMCPY(data_ptr->num_type, 
                            s_pb_phone_contact_list.contact_table[index].num_type, 
                            sizeof(uint8)*MMIPB_MAX_NV_PHONE_NUM);
                        result = MMIPB_ERROR_SUCCESS;

                    }
                   
#ifndef MMIPB_MEMPOOL_SUPPORT
                    else
                    {
                        MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                        if(contact_ptr != PNULL)
                        {
                            MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
                            MMIPB_ACCESS_POINT_T access_point = {0};
                    
                            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                            access_point.contact.contact_id = s_pb_contact_list.list[i].contact_id;
                            access_point.contact.storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                            ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                                access_point,
                                sizeof(MMIPB_CONTACT_T),
                                contact_ptr
                                );
                            if(MMIPB_ERROR_SUCCESS == ret)
                            {
                                SCI_MEMCPY(data_ptr->num_type, contact_ptr->num_type, sizeof(uint8)*MMIPB_MAX_NV_PHONE_NUM);
                            }
                            SCI_FREE(contact_ptr);
                        }
                    }
#endif
                }
                break;
            }
            phone_index++;  
        }
              
    }
    return result;
}



/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int ContactListCmp(uint32 base_index, void *compare_data, void *list)
{
    MMIPB_CONTACT_LIST_NODE_T  *contact_list = (MMIPB_CONTACT_LIST_NODE_T * )list;
    //MMIPB_NAME_T         name     = {0};
    MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
    MMIPB_NAME_LETTERS_T  *input_str = (MMIPB_NAME_LETTERS_T *)compare_data;
    MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
    int                     ret = 0;
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    wchar*                   input_full_letter_ptr = PNULL;
    wchar*                   cmp_full_letter_ptr =PNULL;
    uint16                  pos_index = 0;
#endif
    //uint16 name_len = 0;
    //uint16 i = 0;
    if(PNULL == contact_list)
    {
        //SCI_TRACE_LOW:"ContactListCmp valid parameter !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1255_112_2_18_2_43_21_179,(uint8*)"");
        return -1;
    }
    letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_LETTERS_T));
    if(PNULL == letters_ptr)
    {
        return -1;
    }
    
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    input_full_letter_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_NAME_MAX_LEN*6+1));
    if(PNULL == input_full_letter_ptr)
    {
        SCI_FREE(letters_ptr);
        return -1;
    }
    cmp_full_letter_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_NAME_MAX_LEN*6+1));
    if(PNULL == cmp_full_letter_ptr)
    {
        SCI_FREE(letters_ptr);
        SCI_FREE(input_full_letter_ptr);
    }
#endif    
    MMIPB_GetContactNameNumberByIndex(&cmp_str, base_index);
    MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR_SPACER, TRUE,letters_ptr);
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    for(pos_index=0;pos_index<input_str->full_letters_len; pos_index++)
    {
        input_full_letter_ptr[pos_index] = ConvertLetters(input_str->full_letters[pos_index]);
    }
    for(pos_index=0;pos_index<letters_ptr->full_letters_len; pos_index++)
    {
        cmp_full_letter_ptr[pos_index] = ConvertLetters(letters_ptr->full_letters[pos_index]);  
    }

    ret = MMIAPICOM_Wstrcmp(cmp_full_letter_ptr, input_full_letter_ptr);
    if(ret == 0)
    {
       ret = MMIAPICOM_Wstrcmp(letters_ptr->full_letters, input_str->full_letters); 
    }
#else
    ret = MMIAPICOM_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
#endif
    if(ret < 0 
      && !MMIAPIIM_IsPunct(*letters_ptr->full_letters) 
      &&MMIAPIIM_IsPunct(*input_str->full_letters) 
      )
    {
        //符号不能排在非符号的后面
        ret = 1;
    }
    else if(ret > 0 
      && MMIAPIIM_IsPunct(*letters_ptr->full_letters) 
      &&!MMIAPIIM_IsPunct(*input_str->full_letters) 
      )
    {
        //符号不能排在非符号的后面
        ret = -1;
    }
    //SCI_TRACE_LOW("ContactListCmp ret %d ",ret);
    SCI_FREE(letters_ptr);
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    SCI_FREE(cmp_full_letter_ptr);
    SCI_FREE(input_full_letter_ptr);
#endif
    return ret;
}

/*****************************************************************************/
//  Description : 插入一个节点到记录表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactListNode(
                                                 MMIPB_CONTACT_T *contact_ptr//[IN]
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    uint16                count = MMIPB_INVALID_INDEX;
    //uint16                first_wchar = 0;
    uint32                pos = 0;
    uint32                i = 0;
    MMIPB_NAME_LETTERS_T  *letters_ptr = PNULL;
    MMIPB_CONTACT_BASE_INFO_T *simple_info_ptr = PNULL;

    wchar *key_wstr_ptr = PNULL;
    
    if(contact_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    if(s_pb_contact_list.list_info.count >= s_pb_contact_list.max_count)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_InsertContactListNode max_count %d, count %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1296_112_2_18_2_43_21_180,(uint8*)"dd", s_pb_contact_list.max_count, s_pb_contact_list.list_info.count);
        return result;
    }
    letters_ptr = (MMIPB_NAME_LETTERS_T  *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
    if(PNULL == letters_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_InsertQListNode letters_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1302_112_2_18_2_43_21_181,(uint8*)"");
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
    //#endif
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    MMIPB_GetNameLetters(&contact_ptr->name, MMIPB_NAME_FULL_CHAR_SPACER|MMIPB_NAME_HEAD_CHAR,TRUE,letters_ptr);
    search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
    search_info.compare_base_data = letters_ptr;
    search_info.start_pos = 0;
    //如果是手机联系人保存到先保存手机联系人缓存表
    if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
    {
        {

            count = s_pb_phone_contact_list.count_count;
            if(count < s_pb_phone_contact_list.max_count 
               && s_pb_phone_contact_list.contact_table != PNULL
              )
            {
                //添加在最后
                //插入该节点
                SCI_MEMSET(&s_pb_phone_contact_list.contact_table[count], 0x00, sizeof(MMIPB_PHONE_CONTACT_NODE_T));
                SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].num_type, 
                    contact_ptr->num_type, 
                    sizeof(s_pb_phone_contact_list.contact_table[count].num_type));
                //number
                SCI_MEMCPY(&s_pb_phone_contact_list.contact_table[count].number, &contact_ptr->number, sizeof(contact_ptr->number));
                //name
                if(contact_ptr->name.wstr_len == 0)
                {
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
                }
                else 
                {
//lzk mark
#ifdef BT_PBAP_SUPPORT				
                    BOOLEAN   is_usc2 = !MMIAPICOM_IsASCIIString(contact_ptr->name.wstr, contact_ptr->name.wstr_len); 
                    if (is_usc2)
                    {
                        s_pb_phone_contact_list.contact_table[count].name_str.wstr_len = (MIN(contact_ptr->name.wstr_len * sizeof(wchar), MMIPB_NAME_MAX_LEN))/sizeof(wchar);
                    }
                    else
                    {
                        s_pb_phone_contact_list.contact_table[count].name_str.wstr_len = contact_ptr->name.wstr_len;
                    }
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*(s_pb_phone_contact_list.contact_table[count].name_str.wstr_len+1),s_pb_mempool_handle);
                    if(s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr != PNULL)
                    {
                        SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr, contact_ptr->name.wstr, sizeof(wchar)*s_pb_phone_contact_list.contact_table[count].name_str.wstr_len);
                    }
#else
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_len = contact_ptr->name.wstr_len;
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->name.wstr_len,s_pb_mempool_handle);
                    if(s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr != PNULL)
                    {
                        SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr, contact_ptr->name.wstr, sizeof(wchar)*contact_ptr->name.wstr_len);
                    }

#endif
                }

            //email
#ifdef MMIPB_MAIL_SUPPORT
                if(contact_ptr->mail.wstr_len == 0)
                {
                    s_pb_phone_contact_list.contact_table[count].mail_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
                }
                else 
                {
                    s_pb_phone_contact_list.contact_table[count].mail_str.wstr_len = contact_ptr->mail.wstr_len;
                    s_pb_phone_contact_list.contact_table[count].mail_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->mail.wstr_len,s_pb_mempool_handle);
                    if(s_pb_phone_contact_list.contact_table[count].mail_str.wstr_ptr != PNULL)
                    {
                        SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].mail_str.wstr_ptr, 
                                  contact_ptr->mail.wstr, 
                                  sizeof(wchar)*contact_ptr->mail.wstr_len);
                    }
                }
#endif
#ifdef BT_PBAP_SUPPORT
                s_pb_phone_contact_list.contact_table[count].is_bt_contact = contact_ptr->is_bt_contact;
#endif
                s_pb_phone_contact_list.count_count++;
            }
        }
    }
#ifdef SNS_SUPPORT
    //add sns base information
    else if(MMIPB_IsSNSContact(contact_ptr->storage_id))
    {
        count = s_pb_sns_contact_list.current_count;
        if(count < s_pb_sns_contact_list.max_count 
           && s_pb_sns_contact_list.contact_table != PNULL
          )
        {
            //添加在最后
            //插入该节点
            s_pb_sns_contact_list.contact_table[count].group = contact_ptr->union_t.group;
            //name
            if(contact_ptr->name.wstr_len == 0)
            {
                s_pb_sns_contact_list.contact_table[count].name_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
            }
            else 
            {
                s_pb_sns_contact_list.contact_table[count].name_str.wstr_len = contact_ptr->name.wstr_len;
                s_pb_sns_contact_list.contact_table[count].name_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->name.wstr_len, s_pb_mempool_handle);
                if(s_pb_sns_contact_list.contact_table[count].name_str.wstr_ptr != PNULL)
                {
                    SCI_MEMCPY(s_pb_sns_contact_list.contact_table[count].name_str.wstr_ptr, contact_ptr->name.wstr, sizeof(wchar)*contact_ptr->name.wstr_len);
                }
            }
            s_pb_sns_contact_list.current_count++;
                
        }
    }
#endif
    //find insert postion in s_pb_contact_list
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        search_info.end_pos = s_pb_contact_list.list_info.count -1;
        MMIAPICOM_BinSearch(&search_info,
            ContactListCmp,
            &pos,
            s_pb_contact_list.list);
    }
    //insert node 
    if(s_pb_contact_list.list_info.count < s_pb_contact_list.max_count && pos < s_pb_contact_list.max_count)
    {
        //SCI_DisableIRQ();
        // 顺序后移
        for(i=s_pb_contact_list.list_info.count; i>pos; i--)
        {
            s_pb_contact_list.list[i] = s_pb_contact_list.list[i-1];
        }   
        //clear old data
        SCI_MEMSET(&s_pb_contact_list.list[pos], 0x00, sizeof(MMIPB_CONTACT_LIST_NODE_T));
        //group
        s_pb_contact_list.list[pos].group = contact_ptr->union_t.group;
        //contact id
        s_pb_contact_list.list[pos].contact_id = contact_ptr->contact_id;
        //set index 
        s_pb_contact_list.list[pos].index = count;
#ifdef BT_PBAP_SUPPORT
        s_pb_contact_list.list[pos].is_bt_contact = contact_ptr->is_bt_contact;
#endif
        //set start char
        if(MMIAPIIM_IsPunct(letters_ptr->full_letters[0]))
        {
            s_pb_contact_list.list[pos].first_char = '#';  
        }
        else
        {
            s_pb_contact_list.list[pos].first_char = letters_ptr->full_letters[0];  
        }
        if(pos == 0 || s_pb_contact_list.list[pos].first_char != s_pb_contact_list.list[pos -1].first_char)
        {
            //第一个记录，或者和上一个记录的首字母不相同
            s_pb_contact_list.list_info.first_char_count++;
        }
        //storage id
        s_pb_contact_list.list[pos].short_storage_id = StorageIDToShortID(contact_ptr->storage_id);
#ifdef MMIPB_UID_SUPPORT        
        s_pb_contact_list.list[pos].uid = contact_ptr->uid;
#endif
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(contact_ptr->storage_id))
        {
            s_pb_contact_list.list[pos].is_read_ext = FALSE;
        }
        else
        {
            s_pb_contact_list.list[pos].is_read_ext = TRUE;
        }
        //SCI_TRACE_LOW("[baokun] MMIPB_InsertContactListNode is_read_ext %d",s_pb_contact_list.list[pos].is_read_ext);
#endif
        
        s_pb_contact_list.list_info.count++;
        simple_info_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_BASE_INFO_T));
        if(PNULL != simple_info_ptr)
        {
            SCI_MEMSET(simple_info_ptr, 0x00, sizeof(MMIPB_CONTACT_BASE_INFO_T));
            MMIPB_ReadContactSimpleInfo(pos, simple_info_ptr);
            //adjust base info
            result = AdjustContactListInfo(simple_info_ptr,
                TRUE,
                &s_pb_contact_list.list_info);
            SCI_FREE(simple_info_ptr);            
        }                
        //update contact id usage condition
        if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
        {
            //更新普通联系人的id使用情况表
            SetEntryIdFlag(TRUE, contact_ptr->contact_id, &s_pb_phone_contact_id_table);
        }
        if(letters_ptr->head_letters_len > 0)
        {
            key_wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN +1)*sizeof(wchar));
            if(key_wstr_ptr != PNULL)
            {
                SCI_MEMSET(key_wstr_ptr, 0x00, (MMIPB_NAME_MAX_LEN +1)*sizeof(wchar));
                MMIIM_GetWcharKeyValueByTable(letters_ptr->head_letters, key_wstr_ptr, FALSE);
            }
        }
        MMIPB_InsertQListNode(pos, 
            letters_ptr->head_letters, 
            letters_ptr->head_letters_len,
            key_wstr_ptr,
            letters_ptr->head_letters_len);
        if(key_wstr_ptr != PNULL)
        {
            SCI_FREE(key_wstr_ptr);
        }
        SCI_FREE(letters_ptr);
    }
    SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return result;
}
/*****************************************************************************/
//  Description : 从记录表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactListNode(
                                                 uint16 contact_id,
                                                 uint16 storage_id
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 i = 0;
    //uint32 dest_index = 0;
    //uint32 del_count = 0;
    uint16 index = 0;
    uint16 cache_index = 0;
    //uint16 storage_id = 0;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPB_CONTACT_BASE_INFO_T simple_info = {0};
    index = MMIPB_GetContactListIndex(contact_id, storage_id);
   
    
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        result = MMIPB_ERROR_SUCCESS;
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
        MMIPB_ReadContactSimpleInfo(index, &simple_info);
        cache_index = s_pb_contact_list.list[index].index;
        //从缓存表中删除

        //10手机或USIM
        if(MMIPB_IsPhoneContact(storage_id))
        {

            if(cache_index < s_pb_phone_contact_list.count_count 
                && PNULL != s_pb_phone_contact_list.contact_table
                )
            {
                //删除从手机缓存表中删除该节点
                //delete name str 
                if(s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_ptr);
                    s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_ptr = PNULL;
                    s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_len = 0;
                }
                //delete mail str
                if(s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_ptr);
                    s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_ptr = PNULL;
                    s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_len = 0;
                }
                for(i = cache_index; i < s_pb_phone_contact_list.count_count -1; i++)
                {
                    s_pb_phone_contact_list.contact_table[i] = s_pb_phone_contact_list.contact_table[i + 1];
                }
                s_pb_phone_contact_list.count_count--;
            }
        }
#ifdef SNS_SUPPORT
        //delete from sns cache
        else if(MMIPB_IsSNSContact(storage_id))
        {
            if(cache_index < s_pb_sns_contact_list.current_count 
                && PNULL != s_pb_sns_contact_list.contact_table
                )
            {
                //删除从sns缓存表中删除该节点
                //delete name str 
                if(s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_ptr);
                    s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_ptr = PNULL;
                    s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_len = 0;
                }
                for(i = cache_index; i < s_pb_sns_contact_list.current_count -1; i++)
                {
                    s_pb_sns_contact_list.contact_table[i] = s_pb_sns_contact_list.contact_table[i + 1];
                }
                s_pb_sns_contact_list.current_count--;
            }            
        }
#endif

#ifdef CMCC_VCARD_SUPPORT
          else if(MMIPB_IsUsim(storage_id))
          {
              MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
              MMIPB_BCD_NUMBER_LIST_T *free_list = PNULL;
              if(s_pb_contact_list.list[index].num_mail_list != PNULL)
              {
                  number_list = s_pb_contact_list.list[index].num_mail_list->number_list;
                  while(number_list != PNULL)
                  {
                      free_list = number_list;
                      number_list = number_list->next_ptr;
                      SCI_MPFREE(free_list);
                      free_list = PNULL;
                  }
                  if(s_pb_contact_list.list[index].num_mail_list->mail_str.wstr_ptr != PNULL)
                  {
                      SCI_MPFREE(s_pb_contact_list.list[index].num_mail_list->mail_str.wstr_ptr);
                  }
                  SCI_MPFREE(s_pb_contact_list.list[index].num_mail_list);
                  s_pb_contact_list.list[index].num_mail_list = PNULL;

              }
          }
#endif 
        //从索引表中
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(i >= index && i < s_pb_contact_list.list_info.count -1)
            {
               s_pb_contact_list.list[i] = s_pb_contact_list.list[i + 1];
            }


            if(
               storage_id == ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)
               && s_pb_contact_list.list[i].index > cache_index
              )

            {
                s_pb_contact_list.list[i].index--;
            }

        }
        
        //该节点将被删除，调整基本信息
        AdjustContactListInfo(&simple_info, FALSE, &s_pb_contact_list.list_info);            
        s_pb_contact_list.list_info.count--;
        
        //update contact id usage condition
        if(MMIPB_IsPhoneContact(storage_id))
        {
            //更新普通联系人的id使用情况表
            SetEntryIdFlag(FALSE, contact_id, &s_pb_phone_contact_id_table);
        }
        SCI_PutSemaphore(s_contact_list_semphore_p); 
        //从快速查找表中删除
        //delete index from qsort list
        MMIPB_DeleteQListNode(index);
    }
    return result;
}

/*****************************************************************************/
//  Description : Delete one group's contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOneGroupContactList(uint32 group)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 i = 0;
    uint16 del_count = 0;
    MMIPB_CONTACT_BASE_INFO_T contact_info = {0};

    if(group > 0)
    {
        //delete qsort list
        for(i = 0; i < s_pb_qsort_list.count; i++)
        {
            if(group & s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].group)
            {
                //该节点需要删除
                del_count++;                
            }
            else
            {
                if(del_count > 0)
                {
                    s_pb_qsort_list.list[i - del_count] = s_pb_qsort_list.list[i];
                }
            }
        }
        if(del_count <= s_pb_qsort_list.count)
        {
            s_pb_qsort_list.count = s_pb_qsort_list.count - del_count;
        }

        //delete contact list
        del_count = 0;
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(group & s_pb_contact_list.list[i].group)
            {
                //该节点需要删除
                if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactSimpleInfo(i, &contact_info))
                {
                    AdjustContactListInfo(&contact_info, FALSE, &s_pb_contact_list.list_info);                        
                    SCI_MEMSET(&contact_info, 0x00, sizeof(contact_info));
                }
                del_count++;                
            }
            else
            {
                if(del_count > 0)
                {
                    s_pb_contact_list.list[i - del_count] = s_pb_contact_list.list[i];
                }
            }
        }
        if(del_count <= s_pb_contact_list.list_info.count)
        {
            s_pb_contact_list.list_info.count = s_pb_contact_list.list_info.count - del_count;
        }        
        result = MMIPB_ERROR_SUCCESS;
    }    
    return result;
}
#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr)
{
    uint16 i = 0;
    BOOLEAN ret = FALSE;
    if(PNULL == entry_id_ptr || PNULL == storage_id_ptr)
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_GetcontactIdByUid PNULL == entry_id_ptr || PNULL == storage_id_ptr");
        return ret;
    }
    if(s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(uid == s_pb_contact_list.list[i].uid)
            {
                *entry_id_ptr = s_pb_contact_list.list[i].contact_id;
                *storage_id_ptr = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                ret = TRUE;
                break;
            }
        }
        
    }
    return ret;
}



/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid)
{
    uint16 i = 0;
    BOOLEAN ret = FALSE;
    MMIPB_CONTACT_T contact_info={0};
    if(PNULL == uid)
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_GetUidByContactId PNULL == uid");
        return ret;
    }
    if(MMIPB_ERROR_SUCCESS ==  MMIPB_GetContactInfo(entry_id, storage_id, &contact_info))
    {
        *uid = contact_info.uid;
        
    }
    return ret;
}
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
                                        )
{
    
    uint16 index = MMIPB_INVALID_INDEX;
    uint16 i = 0;
    
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); ;
    if(contact_id > 0 && s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(contact_id == s_pb_contact_list.list[i].contact_id
                && stoage_id == ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)
                )
            {
                index = i;
                break;
            }
        }
        
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    
    return index;
}
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
                                                 )
{
    uint16 index = MMIPB_INVALID_INDEX;
    uint16 i = 0;
    
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
    if(contact_id > 0 && s_pb_mostused_contact_list.contact_table != PNULL)
    {
        for(i = 0; i < s_pb_mostused_contact_list.count; i++)
        {
            if(contact_id == s_pb_mostused_contact_list.contact_table[i].entry_id
                && stoage_id == s_pb_mostused_contact_list.contact_table[i].storage_id
                )
            {
                index = i;
                break;
            }
        }
        
    }
    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p); 
    
    return index;
}
/*****************************************************************************/
//  Description : 读常用联系人记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactCount(
                                                    uint16  *count_ptr//[OUT]
                                                    )
{
    if(count_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    *count_ptr = s_pb_mostused_contact_list.count;
    //SCI_RestoreIRQ();
    
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 读常用联系人索引表的节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactListNode(
                                                       uint16 index, 
                                                       MMIPB_MOSTUSED_CONTACT_NODE_T *data_ptr//[OUT]
                                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
    if(index < s_pb_mostused_contact_list.count)
    {
        *data_ptr = s_pb_mostused_contact_list.contact_table[index];
        result = MMIPB_ERROR_SUCCESS;
    }
    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p); 
    return result;
}

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int MostUsedContactListCmp(uint32 base_index, void *compare_data, void *list)
{
    MMIPB_MOSTUSED_CONTACT_NODE_T   *contact_list = (MMIPB_MOSTUSED_CONTACT_NODE_T * )list;
    
    uint32       used_freq = (uint32)compare_data;
    int ret = 0;
    
    
    if(PNULL == contact_list)
    {
        //SCI_TRACE_LOW:"MostUsedContactListCmp valid parameter !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1764_112_2_18_2_43_22_182,(uint8*)"");
        return -1;
    }
    
    ret = (int32)used_freq - (int32)contact_list[base_index].used_freq;
    
    //SCI_TRACE_LOW:"MostUsedContactListCmp ret %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1770_112_2_18_2_43_22_183,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : 插入一个节点到常用联系人索引表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertMostUsedContactListNode(
                                                         MMIPB_CONTACT_T *contact_ptr//[IN]
                                                         )
{
    MMIPB_ERROR_E       result = MMIPB_ERROR_ERROR;
    uint32              insert_pos = 0;
    uint16              i = 0; 
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    
    if((PNULL == contact_ptr) || (PNULL == s_pb_mostused_contact_list.contact_table))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_InsertMostUsedContactListNode invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1789_112_2_18_2_43_22_184,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    
    // 将号码加入到insert列表，同时完成在该列表中的排序
    
    if(s_pb_mostused_contact_list.count < MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS)
    {
        SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
        search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
        search_info.start_pos = 0;
        search_info.end_pos = s_pb_mostused_contact_list.count -1;
        search_info.compare_base_data = (void *)contact_ptr->union_t.used_freq;
        //查找插入位置
        MMIAPICOM_BinSearch(&search_info,
            MostUsedContactListCmp,
            &insert_pos,
            s_pb_mostused_contact_list.contact_table
            );
        // 顺序后移
        for (i=s_pb_mostused_contact_list.count; i>insert_pos; i--)
        {
            s_pb_mostused_contact_list.contact_table[i] = s_pb_mostused_contact_list.contact_table[i-1];
            //s_pb_mostused_contact_list.visibility_table[i] = s_pb_mostused_contact_list.visibility_table[i-1];
        }   
        SCI_MEMSET(&s_pb_mostused_contact_list.contact_table[insert_pos], 0x00, sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
        //插入
        s_pb_mostused_contact_list.contact_table[insert_pos].entry_id = contact_ptr->contact_id;
        s_pb_mostused_contact_list.contact_table[insert_pos].used_freq = contact_ptr->union_t.used_freq;  
        s_pb_mostused_contact_list.contact_table[insert_pos].storage_id = contact_ptr->storage_id;
#ifndef PB_SUPPORT_LOW_MEMORY
        SCI_MEMCPY(&s_pb_mostused_contact_list.contact_table[insert_pos].number, &contact_ptr->number[0], sizeof(MMIPB_BCD_NUMBER_T));
#endif
        //s_pb_mostused_contact_list.visibility_table[insert_pos] = 1;
        s_pb_mostused_contact_list.count++;
        
        result = MMIPB_ERROR_SUCCESS;
        //SCI_RestoreIRQ();
        //update contact id usage condition
        //更新常用联系人的id使用情况表
        SetEntryIdFlag(TRUE, contact_ptr->contact_id, &s_pb_mostused_contact_id_table);
        SCI_PutSemaphore(s_mostusedcontext_list_semphore_p); 
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 从常用联系人索引表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteMostUsedContactListNode(
                                                         uint16 contact_id,
                                                         uint16 storage_id
                                                         )
{
    uint16 i =0;
    uint32 prority = 0;
    uint16 contact_index = MMIPB_GetMostUsedContactListIndex(contact_id, storage_id);
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    
    if(contact_index < s_pb_mostused_contact_list.count && s_pb_mostused_contact_list.contact_table != PNULL)
    {
        SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
        prority = (s_pb_mostused_contact_list.contact_table[contact_index].used_freq & 0xFFFF);         
        //顺序前移
        for (i=contact_index; i<(s_pb_mostused_contact_list.count-1); i++)
        {
            s_pb_mostused_contact_list.contact_table[i] = s_pb_mostused_contact_list.contact_table[i+1];
            //s_pb_mostused_contact_list.visibility_table[i] = s_pb_mostused_contact_list.visibility_table[i+1];
            //高于该优先级的记录减1
            if((s_pb_mostused_contact_list.contact_table[i].used_freq & 0xFFFF) >= prority)
            {
                s_pb_mostused_contact_list.contact_table[i].used_freq --;
            }
            
        }
        //高于该优先级的记录减1
        for (i=0; i<contact_index; i++)
        {
            if((s_pb_mostused_contact_list.contact_table[i].used_freq & 0xFFFF) >= prority)
            {
                s_pb_mostused_contact_list.contact_table[i].used_freq--;
            }
        }
        s_pb_mostused_contact_list.count--;    
        SCI_PutSemaphore(s_mostusedcontext_list_semphore_p);         
        //更新常用联系人的id使用情况表
        SetEntryIdFlag(FALSE, contact_id, &s_pb_mostused_contact_id_table);
        result = MMIPB_ERROR_SUCCESS;
    }
    return result;
}


/*****************************************************************************/
//  Description : 从常用联系人索引表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllMostUsedContactListNode(void)
{
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER);
    s_pb_mostused_contact_list.count = 0;
    if(s_pb_mostused_contact_list.contact_table != PNULL)
    {
        SCI_MEMSET(s_pb_mostused_contact_list.contact_table, 0x00, sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T)*MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS);
    }
    //常用联系人id使用情况表
    if(PNULL != s_pb_mostused_contact_id_table.valid_flag)
    {
        SCI_MEMSET(s_pb_mostused_contact_id_table.valid_flag,0, ((MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS + 7) / 8 ));
    }
    s_pb_mostused_contact_id_table.max_record_num = MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;
    s_pb_mostused_contact_id_table.spare_id = 1;

    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p);       
}
#endif
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 从联系人索引表中删除所以sns索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllSNSContactListNode(void)
{
    uint16 i = 0;
    uint16 sns_count = 0;
    uint16 first_sns_index = 0;
    uint16 first_char_count = 0;
    uint16 pre_first_char = 0xFFFF;
    uint16 next_first_char = 0xFFFF;
    uint16 *index_offset_ptr = PNULL;

    
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER);
    //clean sns from qsort list
    for(i =0; i < s_pb_qsort_list.count && (PNULL != s_pb_contact_list.list); i++)
    {
        if(MMIPB_IsSNSContact(ShortIDToStorageID(s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].short_storage_id)))
        {
            sns_count++;

        }
        else
        {
            if(sns_count > 0)
            {
                s_pb_qsort_list.list[i - sns_count] = s_pb_qsort_list.list[i];
            }
        }
    } 
    if(sns_count <= s_pb_qsort_list.count)
    {
           s_pb_qsort_list.count -= sns_count;
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p);
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    sns_count = 0;
    //delete all sns index node
    index_offset_ptr = (uint16 *)SCI_ALLOCA(s_pb_contact_list.list_info.count*sizeof(uint16));
    if(index_offset_ptr != PNULL)
    {
        SCI_MEMSET(index_offset_ptr, 0x00, s_pb_contact_list.list_info.count*sizeof(uint16));
    }
    for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
    {   
        if(index_offset_ptr != PNULL)
        {
            index_offset_ptr[i] = sns_count;
        }
        if(MMIPB_IsSNSContact(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
        {
            if(0 == sns_count)
            {
                //设置sns开始的索引值
                first_sns_index = i;
            }
            sns_count++; 
            
            pre_first_char = ((0 == i) ? 0xFFFF :  s_pb_contact_list.list[i -1].first_char);
            next_first_char = ((i + 1< s_pb_contact_list.list_info.count) ? s_pb_contact_list.list[i + 1].first_char:0xFFFF);
            if(s_pb_contact_list.list[i].first_char != pre_first_char
              &&s_pb_contact_list.list[i].first_char != next_first_char
              )
            {
                //这个记录的首字母和前后都不同，该记录删除后，总的首字母个数要-1
                first_char_count++;
            }
            
        }
        else
        {
            if(sns_count > 0)
            {
                //
                s_pb_contact_list.list[i - sns_count] = s_pb_contact_list.list[i];
            }
        }
    }   
    //adjust list info
    s_pb_contact_list.list_info.count = s_pb_contact_list.list_info.count - sns_count;
    if(first_char_count <= s_pb_contact_list.list_info.first_char_count)
    {
        s_pb_contact_list.list_info.first_char_count -= first_char_count;
    }
    for(i = MMIPB_GROUP_SNS_SINA; i < MMIPB_GROUP_SNS_MAX; i++)
    {
        s_pb_contact_list.list_info.group_count[i] = 0;
        s_pb_contact_list.list_info.group_mail_count[i] = 0;
        s_pb_contact_list.list_info.group_number_count[i] = 0;
    }
    s_pb_contact_list.list_info.storage_count[MMIPB_STORAGE_SNS] = 0;
    SCI_PutSemaphore(s_contact_list_semphore_p); 
    
    //adjust sqlist index
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER);     
    if(index_offset_ptr != PNULL)
    {
        for(i =0; i < s_pb_qsort_list.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if(index_offset_ptr[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)] > 0)
            {
                s_pb_qsort_list.list[i] = s_pb_qsort_list.list[i] - index_offset_ptr[(s_pb_qsort_list.list[i]&MMIPB_CONTACT_INDEX_MASK)];
            }
        } 
        SCI_FREE(index_offset_ptr);
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p);
    //reset sns list
    s_pb_sns_contact_list.current_count = 0;
}
#endif
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
                                                     )
{
    return MMIPB_GetContactNameNumberByIndex(string_ptr, MMIPB_GetContactListIndex(contact_id, storage_id));
}


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
                                           )
{
    uint32 sortlist_index = 0;
    int32 i = 0;
    int32 j = 0;
    MMI_BIN_SEARCH_INFO_T  search_info = {0};
    MMI_STRING_T           str_t[2] = {0};
    BOOLEAN             need_insert = FALSE;
    if(s_pb_qsort_list.list == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //change contact name to name letters and number letters
    if(s_pb_qsort_list.count >= s_pb_qsort_list.max_count)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER); 
    
    //adjust index
    for(i =0; i < (int32)s_pb_qsort_list.count; i++)
    {
        if((s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) >= contact_index)
        {
            s_pb_qsort_list.list[i]++;
        }
    }  
    str_t[0].wstr_ptr = wchar_ptr;
    str_t[0].wstr_len = wchar_len;
    str_t[1].wstr_ptr = Key_wchar_ptr;
    str_t[1].wstr_len = Key_wchar_len;
    for(j = 0; j < 2; j++)
    {
        //get insert postion
        search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
        search_info.start_pos = 0;
        search_info.end_pos = (int32)s_pb_qsort_list.count -1;
        if(str_t[j].wstr_len > 0 && str_t[j].wstr_ptr  != PNULL)
        {
            need_insert = TRUE;
            search_info.compare_base_data = &str_t[j];
            MMIAPICOM_BinSearch(&search_info, 
                QSortListCmp,
                &sortlist_index,
                &s_pb_qsort_list
                );
        }  
        //insert to qsort list
        if(need_insert)
        {
            if(sortlist_index < s_pb_qsort_list.count)
            {
                for(i = s_pb_qsort_list.count; i > sortlist_index ; i--) /*lint !e574 !e737*/
                {                                                        
                    s_pb_qsort_list.list[i] = s_pb_qsort_list.list[i -1];
                }  
                //SCI_MEMCPY(&s_pb_qsort_list.list[sortlist_index +1], &s_pb_qsort_list.list[sortlist_index], sizeof(uint16)*(s_pb_qsort_list.count -sortlist_index)); 
            }
            if(sortlist_index <= s_pb_qsort_list.count)
            {
                if(j == 0)
                {
                    s_pb_qsort_list.list[sortlist_index] = contact_index;
                }
                else
                {
                    s_pb_qsort_list.list[sortlist_index] = (contact_index | MMIPB_KEY_HEAD_FLAG);
                }            
            }
            s_pb_qsort_list.count++;    
            need_insert = FALSE;
        }
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p); 
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 从快速查找表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteQListNode(
                                           uint16 contact_index
                                           )
{
    uint32 i = 0;
    uint32 cur_index = 0;
    uint32 del_count = 0;
    BOOLEAN is_valid = FALSE;
    
    //找到需要删除的解决，从表中删除这些节点
    cur_index = 0;
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER); 
    for(i = 0; i < s_pb_qsort_list.count ; i++)
    {
        
        is_valid = TRUE;
        if((s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) > contact_index)
        {
            s_pb_qsort_list.list[i] --;
        }
        else if((s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) == contact_index)
        {
            //该数据被删除
            is_valid = FALSE;
            del_count++;
        }
        if(is_valid)
        {
            //把有效数据放到表中
            if(del_count > 0)
            {
                s_pb_qsort_list.list[cur_index] = s_pb_qsort_list.list[i]; 
            }
            cur_index++;
        }
    }
    
    if(s_pb_qsort_list.count >= del_count)
    {
        s_pb_qsort_list.count -= del_count;
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p); 
    return MMIPB_ERROR_SUCCESS;    
    
}

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
 )
{
    
    uint16 start_pos = 0;
    uint16 end_pos = 0;
    uint16 i = 0;
    //uint16 count = 0;
    uint32 contact_group = 0;
    BOOLEAN result = FALSE;
    if(PNULL != str_input && (str_input->wstr_len > 0))
    {
        if(s_pb_qsort_list.count == 0)
        {
            return 0;
        }
        //find first matched pos
        SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER); 
        start_pos = GetQSortListIndex(str_input, is_char,FALSE);
        //find last matched pos
        end_pos = GetQSortListIndex(str_input,is_char, TRUE);
        //根据查找结果设置记录的可见性
        if(PB_NOT_EXIST != start_pos && (PB_NOT_EXIST != end_pos))
        {
            result = TRUE; 
            for (i = start_pos; i <= end_pos; i++)
            {                
                    //字符查找，并且类型是字符类型
                    //数字查找，并且类型是数字类型
                    //enable this entry to visible
                    contact_group = s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].group;
                    if(
                       contact_group > 0
                       &&(exclude_group & contact_group)
                      )
                    {
                        //在exclude_group中分组记录不显示
                        s_pb_contact_list.list[(s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0;
                    }
                    else if(0 == contact_group && (exclude_group & 0x01))
                    {
                        //去掉未分组
                        s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0;
                    }
#ifdef BT_PBAP_SUPPORT
                    else if((group & 0x01) || (group & (1<<PB_GROUP_BT)) ||(group &contact_group))
#else
                    else if((group & 0x01) || (group &contact_group))
#endif
                    {
                        //列出全部或者在所列的分组中
                        if((is_char && s_pb_qsort_list.list[i] < MMIPB_KEY_HEAD_FLAG)//字符查找，该节点是字符节点
                           || (!is_char &&s_pb_qsort_list.list[i] >= MMIPB_KEY_HEAD_FLAG)//键值查找，该节点为键值节点
                           )
                        {
                            s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0xFF;
                        }
                    }
                }                         
        }
        SCI_PutSemaphore(s_qsort_list_semphore_p); 
    }
    else
    {
        if(s_pb_contact_list.list != PNULL)
        {
            result = TRUE;
            //enable this entry to visible
            for(i =0; i < s_pb_contact_list.list_info.count; i++)
            {
                contact_group = s_pb_contact_list.list[i].group;
                if((contact_group > 0 &&(exclude_group & contact_group))//在exclude_group分组                 
                 )
                {
                    //在exclude_group中分组记录不显示
                    s_pb_contact_list.list[i].visible_flag = 0;
                }
                else if(0 == contact_group && (exclude_group & 0x01))
                {
                    //去掉未分组
                    s_pb_contact_list.list[i].visible_flag = 0;
                }
#ifdef BT_PBAP_SUPPORT
                else if((group & 0x01) || (group & (1<<PB_GROUP_BT)) ||(group &contact_group))
#else
                else if((group & 0x01) || (group &contact_group))
#endif
                {
                    //列出全部或者在所列的分组中
                    s_pb_contact_list.list[i].visible_flag = 0xFF;
                }
            }
        }
    }
    return result;   
}

/*****************************************************************************/
//  Description : 清除联系人的可见性表
// 
/*****************************************************************************/
PUBLIC void MMIPB_CleanVisibleTable(uint32 group)
{
    uint16 i = 0;
#ifdef MMIPB_MOST_USED_SUPPORT    
    if( (1<< PB_GROUP_MOSTUSED) != group)
#endif
    {
        //清除一般联系人的可见性表
        for(i = 0; i < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL; i++)
        {
            s_pb_contact_list.list[i].visible_flag = 0;
        }       
    }
}

/*****************************************************************************/
//  Description : 是所有的联系人可见
// 
/*****************************************************************************/
PUBLIC void MMIPB_EnableAllContactVisible(void)
{
    uint16 i = 0;

    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for(i = 0; i < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL; i++)
    {
        s_pb_contact_list.list[i].visible_flag = 0xFF;
    } 
    SCI_PutSemaphore(s_contact_list_semphore_p);
}
#ifdef MMIPB_SEARCH_CONTACT_CONTENT 
/*****************************************************************************/
//  Discription:TF  find string first not equal position
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 wchar* Wstrstr(const wchar* pString1, const wchar* pString2)
{
	wchar *pPos = (wchar *) pString1; 
	wchar *pStr2 = (wchar *) pString2; 
    uint16 string2_len = 0;

	string2_len = MMIAPICOM_Wstrlen(pString2);
	if(0 == string2_len)
	{
		return((wchar *)pString1);
	}
    MMIAPICOM_Wstrlower(pString1);
	while (*pPos)
	{
		if(0 == MMIAPICOM_Wstrncmp(pPos, pStr2, string2_len))
		{
			return(pPos);
		}
		pPos++; 
	}

	return (PNULL);
}
/*****************************************************************************/
//  Description :check two number is partial matched
//  Global resource dependence : none
//  Author: maryxiao
//Return: 
/*****************************************************************************/
LOCAL BOOLEAN SearchSubStrInContactContent(wchar* substr, MMIPB_CONTACT_T contact_info)
{
    BOOLEAN  ret = FALSE;

    do
    {
#ifdef MMIPB_MAIL_SUPPORT
        if(PNULL!=Wstrstr(contact_info.mail.wstr ,substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifdef CMCC_VCARD_SUPPORT
        if(PNULL!=Wstrstr(contact_info.alias.wstr,substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
        if(PNULL!=Wstrstr(contact_info.imessager.wstr, substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifdef SNS_SUPPORT 
        if(PNULL!=Wstrstr(contact_info.sign.wstr, substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
        if(PNULL!=Wstrstr(contact_info.birth.wstr, substr))
        {
            ret = TRUE;
            break;
        }
        if(PNULL!=Wstrstr(contact_info.url.wstr, substr))
        {
            ret = TRUE;
            break;
        }  
        if(PNULL!=Wstrstr(contact_info.memo.wstr, substr))
        {
            ret = TRUE;
            break;
        } 
#endif
        {
            uint8           group_id[PB_GROUP_MAX] = {0};
            uint8           group_num = 0;
            MMIPB_GROUP_T   group_info = {0};
            int             i = 0;

            group_num = MMIPB_GetAllGroupIDs(contact_info.union_t.group, group_id, PB_GROUP_MAX-1);

            for(i = 0; i < group_num; i++)
            {
                MMIPB_ReadGroup(group_id[i], &group_info);

                if(PNULL != Wstrstr(group_info.group_name.wstr, substr))
                {
                    ret = TRUE;
                    break;
                } 
            }
        }
    } while(0);

    return ret;
}
#endif
/*****************************************************************************/
//  Description : 查找名字包含该字符串的记录
//  search_str: searching string
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInList(wchar *search_str, 
                                          MMIPB_SEARCH_TYPE_T search_type,
                                          BOOLEAN                      is_char,
                                          uint32 group,
                                          uint32 exclude_group)
{
    uint32  i = 0;
    uint32  j = 0;
    uint32  contact_group = 0;
    uint16  count = 0;
    //uint16                num_len = 0;
    BOOLEAN               is_exist = FALSE;
    BOOLEAN               is_num_valid = FALSE;
    MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
    MMIPB_BCD_NUMBER_T bcd_number = {0};
    MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
    


    if(PNULL != search_str)
    {
        is_num_valid = MMIAPICOM_ConvertWStr2Num(search_str, MMIAPICOM_Wstrlen(search_str), &bcd_number);
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    if( (1<<PB_GROUP_MOSTUSED) == group)
    {
       count =  MMIPB_GetAllMostUsedContactCount();
    }
    else
#endif
    {
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
        //非常用联系人查找
        for(i = 0; i< s_pb_contact_list.list_info.count; i++)
        {
            is_exist = FALSE;
            if(s_pb_contact_list.list[i].visible_flag)
            {
                count++;
            }
            else
            {
                contact_group = s_pb_contact_list.list[i].group;
                if(contact_group > 0 && (exclude_group & contact_group))
                {
                    //在exclude_group中的记录不显示
                    continue;
                }
                else if(0 == contact_group && (exclude_group & 0x01))
                {
                    //去掉未分组
                    continue;
                }
#ifdef BT_PBAP_SUPPORT
                if( ((1<< PB_GROUP_ALL) & group) || ((1<< PB_GROUP_BT) & group) || (group & contact_group))
#else
                if( ((1<< PB_GROUP_ALL) & group) || (group & contact_group))
#endif
                {
                    if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactNameNumberByIndex(&cmp_str, i))
                    {
                        //search
                        if(MMIPB_SEARCH_NAME & search_type)/*lint !e655*/
                        {
                            //search name
                            letters_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
                            if(letters_ptr != PNULL)
                            {
                                if(search_str != PNULL)
                                {
                                    if(MMIAPICOM_WstrnstrExt(cmp_str.name.wstr, cmp_str.name.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                    {
                                        s_pb_contact_list.list[i].visible_flag = 0xFF;
                                        is_exist = TRUE;
                                        count++;                                        
                                    }
                                    else
                                    {
                                        SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
                                        MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
                                        if(MMIAPICOM_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                        {
                                            s_pb_contact_list.list[i].visible_flag = 0xFF;
                                            is_exist = TRUE;
                                            count++;                                                
                                        }
                                    }
                                }
                                SCI_FREE(letters_ptr);
                                if(is_exist)
                                {
                                    //count++;
                                    continue;
                                }
                            }
                        }
                        if(MMIPB_SEARCH_NUMBER & search_type)/*lint !e655*/
                        {
                            //search number
                            for ( j = 0; j < MMIPB_MAX_NV_PHONE_NUM; j++ )
                            {   
                                // 从记录中获得号码
                                if(is_num_valid && bcd_number.number_len > 0)
                                {                                   
                                    if(MMIAPICOM_bcdsstr(&cmp_str.number_t[j], &bcd_number))
                                    {
                                        s_pb_contact_list.list[i].visible_flag |= MMIPB_FEILD_MASK_SINGLE_NUMBER << j; 
                                        count++;                                        
                                    }
                                }
                            }
                        }
#ifdef MMIPB_MAIL_SUPPORT
                        if(MMIPB_SEARCH_EMAIL & search_type)/*lint !e655*/
                        {
                            //search email
                            if(search_str != PNULL && MMIAPICOM_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                            {
                                s_pb_contact_list.list[i].visible_flag |= MMIPB_FEILD_FLAG_MASK_MAIL;
                                count++;
                                //continue;
                            }
                        }                        
#endif
                    }
#ifdef MMIPB_SEARCH_CONTACT_CONTENT 
                if((MMIPB_SEARCH_OTHER & search_type) &&(s_pb_contact_list.list[i].visible_flag == 0))
                {
                    uint16 entry_id = 0;
                    uint16 storage_id = 0;
                    MMIPB_CONTACT_T contact;
                    storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                    
#ifdef MMIPB_MOST_USED_SUPPORT
                    if(MMIPB_IsMostUsedContact(storage_id))
                    {
                        MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node = {0};
                        mostused_contact_node = MMIPB_GetMostUsedContactBaseInfo(i);
                        entry_id = mostused_contact_node.entry_id;
                        storage_id =mostused_contact_node.storage_id;

                    }
                    else
#endif
                    {

                        MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
                        contact_node = MMIPB_GetContactBaseInfo(i);
                        entry_id = contact_node.contact_id;
                        storage_id =contact_node.storage_id;
                    }
                    SCI_MEMSET(&contact, 0X0, sizeof(MMIPB_CONTACT_T));
                    MMIPB_GetContactInfo(entry_id, storage_id, &contact);
                    if(SearchSubStrInContactContent(search_str, contact))
                    {
                            s_pb_contact_list.list[i].visible_flag = 0xFF;
                            is_exist = TRUE;
                            count++;                             
                    }

                }
#endif
                }
            }
        }
        SCI_PutSemaphore(s_contact_list_semphore_p);
    }
    return count;
}

/*****************************************************************************/
//  Description : get a free contact
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFreeContactId(uint16  storage_id)
{
    uint16 free_id = 0;
#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_IsMostUsedContact(storage_id))
    {
        free_id = s_pb_mostused_contact_id_table.spare_id;
    }
    else 
#endif
    if(MMIPB_IsPhoneContact(storage_id))
    {
        free_id = s_pb_phone_contact_id_table.spare_id; 
    }
    else if(MMIPB_STORAGE_SIM1 <= MMIPB_GET_STORAGE(storage_id))
    {
        if(PHONEBOOK_SIM_STORAGE == MMIPB_GET_CONTACT_TYPE(storage_id) && MMIPB_GetDualSysByStorageId(storage_id) < MMI_DUAL_SYS_MAX)
        {
            //adn contact
#ifndef MMI_DUALMODE_ENABLE
           free_id = (uint16)PHONEBOOK_GetFirstFreeContactId(MMIPB_GetDualSysByStorageId(storage_id), MMIPB_GET_CONTACT_TYPE(storage_id));
#endif
        }
#ifdef FDN_SUPPORT
        else if(PHONEBOOK_FDN_STORAGE == MMIPB_GET_CONTACT_TYPE(storage_id) && MMIPB_GetDualSysByStorageId(storage_id) < MMI_DUAL_SYS_MAX)
        {
            //fdn contact
            uint16  i = 0;
            MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
            
            fdn_context_ptr = MMIAPIFDN_GetFDNContext(MMIPB_GetDualSysByStorageId(storage_id)); 
            
            if(fdn_context_ptr != PNULL)
            {
            /*搜索到第一个为空的记录id*/
                for (i = 0; i < fdn_context_ptr->fdn_info.max_record_num; i++)
                {
                    //search a id which entry state is false
                    if (!fdn_context_ptr->entry_state[i])
                    {
                        break;
                    }
                }                
                free_id = i + 1;
                
            }
        }
#endif
    }
    return free_id;
}

/*****************************************************************************/
//  Description : set mempool handle
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetMempoolHandle(MPOOL_HANDLE handle)
{
    SCI_DisableIRQ();
    s_pb_mempool_handle = handle;
    SCI_RestoreIRQ();
}

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
    )
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    //uint16 index = 0;

    if(entry_id_ptr == PNULL || storage_id_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindFirstNotInitEntryID parameter invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2487_112_2_18_2_43_23_185,(uint8*)"");
        return FALSE;
    }
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for (i = 0; i < s_pb_contact_list.list_info.count; i++)
    {   
        if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
        {
            if( s_pb_contact_list.list != PNULL
                && (!s_pb_contact_list.list[i].is_read_ext)
             )
            {
                *entry_id_ptr = s_pb_contact_list.list[i].contact_id;
                *storage_id_ptr = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                result = TRUE;
                break;
            }
        }
    }

    SCI_PutSemaphore(s_contact_list_semphore_p);
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindFirstNotInitEntryID result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2509_112_2_18_2_43_23_186,(uint8*)"d", result);
    
    return result;
}

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
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint32 i = 0;
    //uint32 num_index = 0;
    //uint16 index = 0;
    MMIPB_NUM_MAIL_LIST_T *mail_num_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
    uint8  num_index = 0;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    if(PNULL == contact_ptr || contact_id == 0)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_ReadUSIMAddtionalFromContactList invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2532_112_2_18_2_43_23_187,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for(i =0; i < s_pb_contact_list.list_info.count; i++)
    {
        if(s_pb_contact_list.list[i].contact_id == contact_id
           && ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id) == storage_id  
          )
        {
            
            if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
            {                
                mail_num_list = s_pb_contact_list.list[i].num_mail_list;
                if(mail_num_list != PNULL)
                {
                    if(s_pb_contact_list.list[i].is_read_ext == FALSE)
                    {                
                        break;
                    }
                    //read number
                    number_list = mail_num_list->number_list;
                    num_index = 1;
                    while(number_list != PNULL && num_index < MMIPB_MAX_NV_PHONE_NUM)
                    {
                         SCI_MEMCPY(&contact_ptr->number[num_index], &number_list->number, sizeof(MMIPB_BCD_NUMBER_T));
                         contact_ptr->num_type[num_index] = number_list->num_type;
                         number_list = number_list->next_ptr;
                         num_index++;
                    }
                    
                    while(num_index < MMIPB_MAX_NV_PHONE_NUM)
                    {
                        contact_ptr->num_type[num_index] = MMIPB_ADN_NUMBER+num_index;
                        num_index++;
                    }
#ifdef MMIPB_MAIL_SUPPORT
                    //read email
                    if(mail_num_list->mail_str.wstr_len > 0 && mail_num_list->mail_str.wstr_ptr != PNULL)
                    {
                        contact_ptr->mail.wstr_len = MIN(MMIPB_MAX_MAIL_LEN, mail_num_list->mail_str.wstr_len);
                        SCI_MEMCPY(&contact_ptr->mail.wstr, mail_num_list->mail_str.wstr_ptr, sizeof(wchar)*contact_ptr->mail.wstr_len);
                    }           
#endif
                    result = MMIPB_ERROR_SUCCESS;                                  
                }
            }
            break; 
        }
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    //SCI_TRACE_LOW("[baokun] ReadUSIMAddtional contact_id %d storage_id %d result %d", contact_id, storage_id, result);
    return result;
}

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
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_NUM_MAIL_LIST_T *mail_num_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *temp_list = PNULL;
    uint32 i = 0;
    //uint32 index = 0;
    uint8  num_index = 0;

    if(PNULL == contact_ptr || contact_id == 0)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_SaveAddtionToList invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2589_112_2_18_2_43_24_188,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;

    }
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for(i =0; i < s_pb_contact_list.list_info.count; i++)
    {
        if(s_pb_contact_list.list[i].contact_id == contact_id
           && ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id) == storage_id  
          )
        {
            if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
            {     
                //SCI_TRACE_LOW("[mmipb] get same contact_id %d storage_id %d", contact_id, storage_id);
                mail_num_list = SCI_MPALLOC(sizeof(MMIPB_NUM_MAIL_LIST_T), s_pb_mempool_handle);
                if(mail_num_list != PNULL)
                {             
                    SCI_MEMSET(mail_num_list, 0x00, sizeof(MMIPB_NUM_MAIL_LIST_T));
                    s_pb_contact_list.list[i].is_read_ext = TRUE;
                    s_pb_contact_list.list[i].num_mail_list= mail_num_list;
                    //add number to num_list
                    for(num_index = 1; num_index < MMIPB_MAX_NV_PHONE_NUM; num_index++)
                    {
                        SCI_TRACE_LOW("xiaohua.liu MMIPB_SaveAddtionToList 1111\n");
	
                        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[num_index].number_len, 
                            contact_ptr->number[num_index].npi_ton))
                        {
                            number_list = (MMIPB_BCD_NUMBER_LIST_T *)SCI_MPALLOC(sizeof(MMIPB_BCD_NUMBER_LIST_T), s_pb_mempool_handle);
                            SCI_TRACE_LOW("xiaohua.liu MMIPB_SaveAddtionToList 2222\n");
				            if(number_list != PNULL)
                            {
                                if(temp_list != PNULL)
                                {
                                    //next node
                                    temp_list->next_ptr = number_list;
                                }
                                else                           
                                {
                                    //first node
                                    mail_num_list->number_list = number_list;
                                }
                                                                   
                                SCI_MEMSET(number_list, 0x00, sizeof(MMIPB_BCD_NUMBER_LIST_T));
                                SCI_MEMCPY(&number_list->number,&contact_ptr->number[num_index], sizeof(MMIPB_BCD_NUMBER_T));
                                number_list->num_type = contact_ptr->num_type[num_index];
                                temp_list = number_list;
                                s_pb_contact_list.list_info.number_count++;
                            }
                        }
                        
                    }
#ifdef CMCC_VCARD_SUPPORT
                    MMIPB_InsertAddtionNumberToList(contact_ptr);
#endif
#ifdef MMIPB_MAIL_SUPPORT
                        //read email
                        if(contact_ptr->mail.wstr_len > 0)
                        {
                            mail_num_list->mail_str.wstr_ptr = SCI_MPALLOC((contact_ptr->mail.wstr_len +1)*sizeof(wchar), s_pb_mempool_handle);
                            if(mail_num_list->mail_str.wstr_ptr  != PNULL)
                            {
                                SCI_MEMSET(mail_num_list->mail_str.wstr_ptr, 0x00, (contact_ptr->mail.wstr_len +1)*sizeof(wchar));
                                SCI_MEMCPY(mail_num_list->mail_str.wstr_ptr,
                                           contact_ptr->mail.wstr,
                                           contact_ptr->mail.wstr_len*sizeof(wchar));
                                mail_num_list->mail_str.wstr_len = contact_ptr->mail.wstr_len;
                                s_pb_contact_list.list_info.mail_count++;
                            }
                        }
#endif
                }
            }
            result = MMIPB_ERROR_SUCCESS;
            break;
        }
    }
    SCI_PutSemaphore(s_contact_list_semphore_p);
    return result;
}
#endif
#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsTCardContactExist(void)
{
   MMIPB_CONTACT_LIST_INFO_T list_info = {0};
   BOOLEAN ret = FALSE;

    //baokun need check
    if(MMIPB_ERROR_SUCCESS ==  MMIPB_ReadContactList(&list_info))
    {
    	   if(list_info.tcontact_count > 0)
    	   {
    	   	  ret = TRUE;
    	   }
    }	
    if(!ret && MMIPB_IsTcardFileExist())
    {
       ret = TRUE;
    }
    return ret;
}
#endif

#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
//  Description :获取相应组的第某个索引值开始的第一个entry_id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetFirstIdWithIndex(uint32       group_id, uint32* m_index,int *entry_id)
{
    BOOLEAN   ret = FALSE;
    uint32 index = 0;
    uint32 temp_group = 0;
    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex group_id=%d m_index=%d !",group_id,m_index);
    if(NULL == m_index)
    {
        return FALSE;
    }
    if( group_id >= MMIPB_MAX_GROUP_NUM)
    {
        SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex group_id=%d ",group_id);
        return FALSE;
    }
    if(PB_GROUP_ALL== group_id)//这里表示是手机上的
    {
        temp_group |=(1<<PB_GROUP_ALL);
        temp_group |=(1<<PB_GROUP_HOME);
        temp_group |=(1<<PB_GROUP_FRIEND);
        temp_group |=(1<<PB_GROUP_COLLEAGUE);
        temp_group |=(1<<PB_GROUP_SCHOOLMATE);
#ifdef MMIPB_MOST_USED_SUPPORT 
        temp_group |=(1<<PB_GROUP_MOSTUSED);
        temp_group |=(1<<PB_GROUP_FAVROUTE);
#endif
    }
    else
    {
        temp_group = (1<<group_id);
    }
    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex count=%d,temp_group=0x%x ",s_pb_contact_list.list_info.count,temp_group);
	for(index = *m_index; index < s_pb_contact_list.list_info.count; index++)
	{
	    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex group=%d ",s_pb_contact_list.list[index].group);

        if(PB_GROUP_ALL==s_pb_contact_list.list[index].group && PB_GROUP_ALL==group_id)
        {
            break;
        }
        if(temp_group&s_pb_contact_list.list[index].group)
        {
            break;
        }
        
	}
	if(index<s_pb_contact_list.list_info.count)
	{
        if(NULL != entry_id)
        {
            *entry_id = s_pb_contact_list.list[index].contact_id;
            SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex  index=%d,entry_id=%d",index,*entry_id);
        }
        *m_index = index;
        ret = TRUE;
	}
    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex ret=%d ",ret);
	return ret;
}

#endif//MCARE_V31_SUPPORT

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
										 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 i = 0;
    uint16 j = 0;
    uint16 storage_id = 0;
    uint16 contact_id = 0;
    
    //get a visibile contact from start_index
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    if(s_pb_contact_list.list != PNULL)
    {
        for(i = start_index; i < s_pb_contact_list.list_info.count; i++)
        {
            storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
            contact_id = s_pb_contact_list.list[i].contact_id;
            //如果该记录是蓝牙联系人
            if(MMIPB_IsBTContactListNode(contact_id, storage_id) && (s_pb_contact_list.list[i].visible_flag > 0))
            {
                result = MMIPB_ERROR_SUCCESS;
                find_data_ptr->storage_id = storage_id;
                find_data_ptr->contact_id = contact_id;
                find_data_ptr->index = i;
                find_data_ptr->group = s_pb_contact_list.list[i].group;
				find_data_ptr->visible_flag = 1;
                find_data_ptr->mail_exist = FALSE;
                find_data_ptr->is_bt_contact = TRUE;
                GetContactNumType(s_pb_contact_list.list[i], find_data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
                if(find_index != PNULL)
                {
                    //set find index
                    *find_index = i; 
                }
                
                break;               
            }
            
        }
        
    }
    
    SCI_TRACE_LOW("MMIPB_FindBtContact start_index %d, result %d", start_index, result);
    SCI_PutSemaphore(s_contact_list_semphore_p);
    return result;
}

/*****************************************************************************/
//  Description : check if bt contact item
//  Global resource dependence : 
//  Author:cheney wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsBTContactListNode(
                                         uint16 contact_id,
                                         uint16 storage_id
                                         )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    uint16 index = 0;
    uint16 cache_index = 0;
    
    index = MMIPB_GetContactListIndex(contact_id, storage_id);
    
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        result = MMIPB_ERROR_SUCCESS;
        cache_index = s_pb_contact_list.list[index].index;
        //从缓存表中删除
        
        //10手机或USIM
#if 0
        if(MMIPB_IsPhoneContact(storage_id))
        {
            if(cache_index < s_pb_phone_contact_list.count_count 
                && PNULL != s_pb_phone_contact_list.contact_table
                )
            {
                return s_pb_phone_contact_list.contact_table[cache_index].is_bt_contact;
            }
        }
#else
        return s_pb_phone_contact_list.contact_table[cache_index].is_bt_contact;
#endif
        
        
    }
    return result;
}
#endif

/*****************************************************************************/
//  Description : 查找名字是否存在
//  search_str: searching string
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchFullStringInList(MMI_STRING_T *search_str, uint32 storage_id, uint32* pos)
{
    uint32  i = 0;
    BOOLEAN               is_exist = FALSE;

    if(search_str == PNULL)
    {
        SCI_TRACE_LOW(" STR IS NULL!");
        return is_exist;
    }

    {
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
        //非常用联系人查找
        for(i = 0; i< s_pb_contact_list.list_info.count; i++)
        {
            MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};

            is_exist = FALSE;

            if(storage_id != ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id))
            {
                continue;
            }
            if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactNameNumberByIndex(&cmp_str, i))
            {
                if(0 == MMIAPICOM_CompareTwoWstr(cmp_str.name.wstr, cmp_str.name.wstr_len, search_str->wstr_ptr, search_str->wstr_len))
                {
                    is_exist = TRUE;
                    *pos = i;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        SCI_PutSemaphore(s_contact_list_semphore_p);
    }
    return is_exist;
}