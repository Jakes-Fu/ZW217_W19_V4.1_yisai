/*****************************************************************************
** File Name:      mmipb.c                                                   *
** Author:                                                                   *
** Date:           27/08/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi phonebook               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2004        Tracy Zhang      Create
******************************************************************************/
//#include "std_header.h"
#include "mmi_app_pb_trc.h"
#include "mmi_signal.h"
#ifdef PIM_SUPPORT
#include "mmipim.h"
#endif
#define _MMIPB_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "nvitem.h"
#include "window_parse.h"
#include "mmipub.h"
#include "mmipb_export.h"
#include "mmipb_adapt.h"
#include "mmk_app.h"
#include "mmi_text.h"
#include "mmi_appmsg.h"
#include "mmipb_search.h"
#include "mmipb_view.h" 
#include "mmipb_id.h"
#include "mmipb_text.h"
//#include "Mmipb_atc.h"
#include "mmiudisk_export.h"
#include "mmivcard_export.h"
#include "mmifdn_export.h"
#include "mmiidle_export.h"
#include "mmiim.h"
#include "guitab.h"
#include "mmipb_common.h"
#include "mmipb_task.h"
#include "mmipb_interface.h"
#include "mmiim.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Phonebook初始化
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitApp (
						void
						)
{
    MMIPB_InitApp();
}

/*****************************************************************************/
// 	Description : register pb nv and menu
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitModule(void)
{
	MMIPB_RegNv();
	MMIPB_RegWinIdNameArr();
    MMIPB_RegAppletInfo();
    MMIPB_InitAppletQueue();
}

/*****************************************************************************/
//  Description : MMIAPIPB_InitSimStatus
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitSimStatus(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_InitSimStatus(dual_sys);
}

/*****************************************************************************/
//  Description : Resort Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: 这个命令会引起pb的立即重新排序，如果sim都ready，不需要等CALL_READY和SIM_NOT_READY
/*****************************************************************************/
PUBLIC void MMIAPIPB_ResortPB(void)
{
     MMIPB_ResortPB();
}

/*****************************************************************************/
//  Description : re_initialize Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: 这个命令会等ALL_READY和SIM_NOT_READY后再进行重排
//  [特别提示Warnning]:调用这个重排接口，一定要保证sim卡会重新发ALL_READY和SIM_NOT_READY，即sim会重新初始化
/*****************************************************************************/
PUBLIC void MMIAPIPB_ReinitPB(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_ReinitPB(dual_sys);
}

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : reset pb group info
//  Global resource dependence : s_mmipb_group_info
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_ResetPBGroupInfo(void)
{
    MMI_HANDLE_T handle =0;

    MMIPB_GetCurAppletHandle(&handle);
    // 初始化组信息列表
    MMIPB_ResetGroupInfo();
}
#endif
/*****************************************************************************/
//  Description : 创建handle
//  Global resource dependence : none
//  Author:baokun.yin 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_CreateHandle(void)
{
    return MMIPB_CreateHandle(MMIPB_OPC_FIND, 0);
}

/*****************************************************************************/
//  Description : 销毁handle
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_CloseHandle(
                                 MMIPB_HANDLE_T handle
                                 )
{
    MMIPB_CloseHandle(handle);
}

/*****************************************************************************/
//  Description : PB主入口函数，进入pb主窗口，根据参数，显示PB主列表
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenMainWin(
                        MMIPB_SUB_FUNCTION_TYPE_E app_type,
                        MMIPB_LIST_TYPE_E list_type, 
                        MMIPB_LIST_FIELD_TYPE_E field_display_type,
                        uint16 		max_select_count,
                        const MMI_STRING_T  *str_input,
                        MMIPB_RETURN_CALLBACK_FUNC callback//can be PNULL
                        )
{
    return MMIPB_OpenMainWin(app_type, list_type, field_display_type, MMIPB_SEARCH_NONE,max_select_count, str_input,callback);
}
/*****************************************************************************/
//  Description : ADD  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContact(MMIPB_CONTACT_T *contact_ptr)
{
    return MMIPB_AddContact(contact_ptr);
}


/*****************************************************************************/
//  Description : delete  CONTACT
//  Global resource dependence : none
//  Author: 
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteContact(uint16 entry_id, uint16 storage_id)
{
    return MMIPB_DeleteContact(entry_id, storage_id);
}


/*****************************************************************************/
//  Description : MODIFY  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContact(MMIPB_CONTACT_T *contact_ptr)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_ModifyContact(contact_ptr);
}
#ifdef MMI_VCARD_SUPPORT  
/*****************************************************************************/
// Description : 异步导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ImportVcardAsyn(MMIPB_HANDLE_T handle, //IN
                                              const wchar *path_name_ptr,//IN
                                              MMIPB_ACCESS_CALLBACK_FUNC callback
                                              )
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_ImportVcardAsyn(handle, path_name_ptr, callback);
}

/*****************************************************************************/
// Description : 导出联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ExportVcard(uint16 entry_id, uint16 storage_id, const wchar *path_name_ptr)
{
    return MMIPB_ExportVcard(entry_id, storage_id, path_name_ptr);
}
#endif
/*****************************************************************************/
//  Description : 复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_CopyContact(uint16 to_storage_id, uint16 from_entry_id, uint16 from_storage_id)
{
    return MMIPB_CopyContact(to_storage_id, from_entry_id, from_storage_id);
}



#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :写群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id)
{
    return MMIPB_WriteGroupName(name, group_id);
}
#endif
/*****************************************************************************/
//  Description : 异步新增联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_AddContactAsyn(handle, contact_ptr, callback);
}

/*****************************************************************************/
//  Description : 异步 编辑联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_ModifyContactAsyn(handle, contact_ptr, callback);
}

/*****************************************************************************/
//  Description : 异步删除联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteContactAsyn(
                                                MMIPB_HANDLE_T handle, 
                                                uint16 entry_id,
                                                uint16 storage_id, 
                                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                                )
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_DeleteContactAsyn(handle,entry_id, storage_id, callback);
}

/*****************************************************************************/
//  Description : 异步复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:提供给外部接口调用，暂时屏蔽该接口！！！
/*****************************************************************************/
// PUBLIC MMIPB_ERROR_E MMIAPIPB_CopyContactAsyn(
//                                               MMIPB_HANDLE_T handle, 
//                                               uint16 dest_storage_id, 
//                                               uint16 src_contact_id, 
//                                               uint16 src_storage_id, 
//                                               MMIPB_ACCESS_CALLBACK_FUNC callback
//                                               )
// {
//     MMIPB_HANDLE_NODE_T*  handle_node_ptr = (MMIPB_HANDLE_NODE_T*)MMIPB_GetHandleContent(handle);
//     
//     if(handle_node_ptr != PNULL)
//     {
//         handle_node_ptr->callback = callback;
//     }
//     return MMIPB_CopyOneContactAsyn(handle, dest_storage_id, src_contact_id, src_storage_id, MMIPB_FEILD_FLAG_MASK_ALL);
// }

#ifdef MMI_VCARD_SUPPORT  
/*****************************************************************************/
// Description : 导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ImportVcard(
                                    //uint16      to_storage_id,      
                                    const wchar *path_name_ptr,//IN
                                    MMIPB_CONTACT_T* contact_ptr//OUT
                                    )
{
    return MMIPB_ImportVcard(path_name_ptr, contact_ptr);
}
#endif

/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: OUT: contact_ptr
//        OUT: num_index
//        IN: handle
//        IN: index
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T * contact_ptr, uint16 index, uint16 * num_index)
{
    return MMIPB_GetSelectContact(handle, contact_ptr, index, num_index);
}

/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人姓名。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index)
{
    return MMIPB_GetSelectName(handle, name_str, max_name_len, index);
}

/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人email。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index)
{
    return MMIPB_GetSelectEmail(handle, mail_ptr, max_mail_len, index);
}

/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人号码。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectNumber(MMIPB_HANDLE_T handle, MMIPB_BCD_NUMBER_T *nameber_ptr, uint16 max_number_len,  uint16 index)
{
    return MMIPB_GetSelectNumber(handle, nameber_ptr, max_number_len, index);
}
#ifdef MMI_VCARD_SUPPORT  
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人vcard。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectVcard(MMIPB_HANDLE_T handle, uint8  *data_ptr, uint16 max_buf_len,  uint16 index)
{
    return MMIPB_GetSelectVcard(handle, data_ptr, max_buf_len, index);
}
#endif
#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : FDN列表窗口
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenFDNContactListWin(MN_DUAL_SYS_E dual_sys)
{
    return MMIPB_OpenFDNContactList(dual_sys);
}
#endif
/*****************************************************************************/
//  Description : PB外部查找函数，通过字符串查找符合条件的联系人列表
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIPB_SearchAllWin(
                            MMIPB_LIST_TYPE_E list_type, 
                            MMIPB_LIST_FIELD_TYPE_E field_display_type,
                            MMI_STRING_T *search_str,
                            MMIPB_RETURN_CALLBACK_FUNC callback
                            )
{
   return MMIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        list_type, //IN: 号码列表的类型
                        field_display_type,
                        (MMIPB_SEARCH_NAME + MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_EMAIL),
                        1,//如果是多选时，此项限制了多选的最大个数
                        search_str,
                        callback
                        );
}

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录和姓名中按照首字母匹配的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 操作句柄，这个句柄使用完成后要调用MMIAPIPB_SearchExit释放
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_SearchList(MMI_STRING_T *search_str, 
                               MMIPB_LIST_FIELD_TYPE_E field_type, 
                               uint16 *count_ptr
                               )
{
    MMIPB_HANDLE_T handle = 0;

    if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB))
    {
        *count_ptr = 0;
        return 0;
    }
    
    if(count_ptr != PNULL)
    {
        *count_ptr = MMIPB_SearchList(search_str, field_type, MMIPB_SEARCH_NAME + MMIPB_SEARCH_NUMBER,&handle);
    }
    return handle;
}

/*****************************************************************************/
//  Description : 外部搜索退出，当外部搜索退出是，这个接口一定要调用
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SearchExit(MMIPB_HANDLE_T handle)
{
	MMIPB_ExitSearchList(handle);
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的姓名，和号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntry(MMIPB_HANDLE_T handle,
                                       MMI_STRING_T *name_str,//[OUT] 
                                       uint16 max_name_len, 
                                       MMIPB_BCD_NUMBER_T *bcd_num,//[OUT]
                                       uint32* group,
                                       uint32 index)
{
    return MMIPB_GetSearchEntry(handle, name_str, max_name_len, bcd_num, group,index);
}


/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的contact_id and storage id
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 为外部进行进pb选择后
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntryID(MMIPB_HANDLE_T handle,                                       
                                       uint32 index,
                                       uint16 *contact_id_ptr,
                                       uint16 *storage_id_ptr)
{
    return MMIPB_GetSearchEntryID(handle, index, contact_id_ptr, storage_id_ptr);
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的小图标
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetSearchEntryIcon(uint32 group)
{
    return MMIPB_GetSearchEntryIcon(group);
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 设置电话号码的被使用

//  Note: 更新该号码的使用频率。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_SetNumberUsed(uint8 *num_str, uint16 number_len)
{
	MMIPB_BCD_NUMBER_T number_t = {0};
	MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};

	if (number_len > 0  && MMIAPICOM_GenPartyNumber((uint8 *)(num_str), number_len, &party_num))
	{
		number_t.npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
		number_t.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
		SCI_MEMCPY(number_t.number, party_num.bcd_num, number_t.number_len);
	}
	else
	{
		number_t.number_len = 0;
		number_t.npi_ton = 0;
	}	   

    return MMIPB_SetNumberUsed(&number_t);
}
#endif
/*****************************************************************************/
//  Description : 打开添加菜单的窗口。
//  Global resource dependence : g_mmipb_bcd_number_t
//  Author: 
//  Note: 此函数供短消息和彩信提取号码并保存的时候调用；
//        供通话记录号码保存到电话簿中调用；供本模块调用。
//        供其它模块调用时，需要传入号码；供本模块调用时，不用传号码。
/*****************************************************************************/

void MMIAPIPB_AddContactWin
(
    void	*data_ptr,	    //IN: 输入的电话号码
    uint8   group_id,                  //In:group id,
    MMIPB_ADD_TYPE_T  type
    
)
{
	MMI_WIN_ID_T alert_win_id = MMIPB_MESSAGE_ALERT_WIN_ID;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#ifdef PIM_SUPPORT //add by wangxiaolin 2010.04.07
 if(MMIPIM_IsSyncProcing())
 {
    return ;
 }
#endif
    if(!MMIPB_IsPbReady())
	{
	    //phonebook processing, can not enter pb agin!!
        for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
        {
            if(SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_NOT_INIT,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                return;
            }
        } 
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_ISNOT_READY,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

	}
	else
	{
		MMIPB_AddContactWin(PNULL, data_ptr, group_id, 0, type,TRUE);
	}
}
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 为通话记录打开记录列表窗口。
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 此函数供通话记录将号码保存到已有姓名时调用。
/*****************************************************************************/
PUBLIC void MMIAPIPB_AddToExistContact
(
    void	* data_ptr,
    MMIPB_ADD_TYPE_T  type,
   MMIPB_RETURN_CALLBACK_FUNC callback
)
{
	//MMIPB_CloseAllPBWindows();
    MMIPB_AddToExistContact(data_ptr,type,callback);
}
#endif
/*****************************************************************************/
//  Description : 获得存储位置所对应的姓名、群组ID、图片ID
//  Global resource dependence : none
//  Parameter:  bcd_number : 输入的电话号码[IN]
//              name_ptr:  姓名[OUT]
//              max_name_len:姓名的最大长度(以字节计)  [IN]
//              is_use_default_name:是否需要返回缺省名称[IN]
//   RETURN: TRUE, 操作成功；FALSE, 操作失败
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
								 MMI_STRING_T *name_ptr,
								 uint16 max_name_len,
								 BOOLEAN is_use_default_name)
{
    return  MMIPB_GetNameByNumber(bcd_number, name_ptr, max_name_len, PNULL,PNULL,PNULL,is_use_default_name, TRUE);
}

/*****************************************************************************/
//  Description : 获取给定号码对应的号码类型
//  Global resource dependence : none
//  Parameter:  bcd_number : 输入的电话号码[IN]
//              type_ptr:  号码类型[OUT]
//              max_name_len:号码类型字符串的最大长度(以wchar计)  [IN]
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNumberTypeByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
								 wchar *type_ptr,
								 uint16 max_name_len
                                 )
{
    return  MMIPB_GetNumberTypeByNumber(bcd_number, type_ptr, max_name_len);
}
/*****************************************************************************/
//  Description : 获得电话簿是否可以被其它模块调用
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

BOOLEAN MMIAPIPB_IsPbReady(void) //RETURN: TRUE, 可以使用
{
    return MMIPB_IsPbReady();
}


/*****************************************************************************/
//  Description :读取群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ReadGroupName(MMIPB_NAME_T* name,  uint8 group_id)
{
    return MMIPB_ReadGroupName(name, group_id);
}
/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetCustomPhotoPathByNumber(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr,
						wchar	*path_ptr,
						uint32	*path_len_ptr,
						uint32	*file_size_ptr
						)
{
	return MMIPB_GetCustomPhotoPathByNumber(
						bcd_num_ptr,
						path_ptr,
						path_len_ptr,
						file_size_ptr
						);
}
#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : get custom asp path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetCustomASPPathByNumber(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr,
						BOOLEAN  *is_fixed_id,
						wchar	*path_ptr,
						uint32	*path_len_ptr,
						uint32	*fixed_id
						)
{
		return MMIPB_GetCustomASPFileByNumber(
						bcd_num_ptr,
						is_fixed_id,
						path_ptr,
						path_len_ptr,
						fixed_id
						);
}
#endif
/*****************************************************************************/
//  Description : get custom ring path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetCustomRingPathByNumber(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr,
						BOOLEAN  *is_fixed_id,
						wchar	*path_ptr,
						uint32	*path_len_ptr,
						uint32	*fixed_id
						)
{
		return MMIPB_GetCustomRingFileByNumber(
						bcd_num_ptr,
						is_fixed_id,
						path_ptr,
						path_len_ptr,
						fixed_id
						);
}

/*****************************************************************************/
//  Description : set contact ring to defaul
//  Global resource dependence : none
//  Author:
//  Note:	
/*****************************************************************************/
PUBLIC void MMIAPIPB_ClearContactRing(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr
						)
{
    uint16 contact_id = 0;
    uint16 storage_id = 0;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    if(MMIPB_SearchPhoneNumber(bcd_num_ptr,&contact_id, &storage_id, PNULL))
    {
        if(MMIPB_IsPhoneContact(storage_id))
        {
            contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL != contact_ptr)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(contact_id, storage_id, contact_ptr)
                  && contact_ptr->ring_id_flag > 0
                  )
                {
                    contact_ptr->ring_id_flag = 0;
                    MMIPB_ModifyContact(contact_ptr);
                }
                SCI_FREE(contact_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_ReadPhotoFileName(uint16 entry_id, 
                                       uint16 storage_id, 
                                       MMIPB_CUSTOM_PHOTO_T *photo_ptr)
{
    return MMIPB_ReadPhotoFile(entry_id, storage_id, photo_ptr);
}					

/*****************************************************************************/
//  Description :	打断PB
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_AbortPB(void)
{
      MMIPB_AbortPB(TRUE);
}

/*****************************************************************************/
//  Description : 获取该设备的storage id
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetStorageId(
                                    MN_DUAL_SYS_E dual_sys,
                                    PHONEBOOK_STORAGE_E storage
                                    )
{
    //uint16 storage_id;
    uint8  storage_type = 0;
    uint8  contact_type = 0; 

    if(PHONEBOOK_NV_STORAGE == storage)
    {
        storage_type = MMIPB_STORAGE_PHONE;
        contact_type = MMIPB_NORMAL_PHONE_CONTACT;
    }
    else
    {
        storage_type = MMIPB_STORAGE_SIM1+dual_sys;
        contact_type = storage;
    }
    return MMIPB_GetStorageID(storage_type, contact_type);
    
}
#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : 将联系人信息转换为vcard数据格式
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIAPIPB_ContactToVcard(
                                     MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                     uint32          data_max_size,//[IN]
                                     uint8 *data_ptr,//[OUT]
                                     uint32 *data_len_ptr//[OUT]
                                    )
{
     return MMIPB_ContactToVcard(contact_ptr, data_max_size, data_ptr, data_len_ptr);
}
#endif
#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : 将vcard数据转换为联系人信息的通用格式
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIAPIPB_VcardToContact(
                                    uint8 *data_ptr,//[IN]
                                    uint32  data_len,//[IN]
                                    MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                    )
{
   return MMIPB_VcardToContact(data_ptr, data_len, contact_ptr);
}
#endif
/*****************************************************************************/
// Description : open one favorite entry called only by widget.
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_OpenContactWin(uint16 entry_id, uint16 storage_id)
{
    MMIPB_OpenContactWin(PNULL, entry_id, storage_id);
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 解析VCard data,并打开该文件
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_OpenVcardContactWin(uint8  *data_ptr, uint32 data_size)
{
    MMIPB_OpenVcardContactWin(data_ptr, data_size);
}
#endif
#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
// Description : 解析VCard data,并打开该文件
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_OpenVcardFileContactWin(uint16  *file_name_ptr)
{
    MMIPB_OpenVcardFileContactWin(file_name_ptr);
}
#endif
/*****************************************************************************/
// Description : open one contact details by number.
// Global resource dependence :
// Author:baokun.yin
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenNumberContactWin(MMIPB_BCD_NUMBER_T *number_ptr)
{
    BOOLEAN result = FALSE;
    uint16  contact_id = 0;
    uint16  storage_id = 0;
    if(PNULL !=  number_ptr)
    {
        if(TRUE == MMIPB_SearchPhoneNumber(
                                            number_ptr,
                                            &contact_id,                                    
                                            &storage_id,
                                            PNULL
                                            )
         )
        {
            MMIPB_OpenContactWin(PNULL, contact_id, storage_id);
            result = TRUE;
        }
    }
    return result;
}


#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//+++mdy by qing.yu@for BQB
PUBLIC void MMIAPIPB_GetMSISDNumber(MMIPB_BCD_NUMBER_T *number)
{
  MMIPB_GetMSISDNumberArrayBySim(number, 
                                MMIAPISET_GetActiveSim(),
                                1);
}
//---mdy by qing.yu@for BQB
#endif


/*****************************************************************************/
//  Description : Get First Valid Number index from number array number_t_ptr
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_GetFirstValidNumberFromNumArray(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count)
{
     uint32 loop_count = MIN(MMIPB_MAX_NV_PHONE_NUM, max_count);
     uint32  i = 0;
     int32  result = -1; 

     for(i = 0; i < loop_count; i++)
     {
         if(MMIPB_IsValidPhoneNumber(number_t_ptr[i].npi_ton, number_t_ptr[i].number_len))
         {
             result = (int32)i;
             break;
         }
     }
     return result;
}

/*****************************************************************************/
//  Description : delete all pb info 
//  Author: 
//  Note: for dm cmcc 
/*****************************************************************************/
PUBLIC void MMIAPIPB_DeleteAllPBInfo(void)
{
    MMIPB_DeleteAllPBInfo();
}
//#endif



/*****************************************************************************/
//  Description : 电话号码的职能匹配
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:两个号码能够职能匹配，其他不能匹配
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, const MMIPB_BCD_NUMBER_T *bcd_number2)
{
	return MMIPB_BcdSmartCmp(bcd_number1, bcd_number2);
}

/*****************************************************************************/
//  Description : 将bcd格式的电话号码转换为字符串
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPB_BCDNumberToString(
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in   
                                    uint8               str_max_len,//IN
                                    uint8               *str_ptr//OUT                                    
                                    )
{
    return MMIPB_BCDNumberToUTF8(
                                    bcd_number_ptr,//in   
                                    str_max_len,//IN
                                    str_ptr//OUT                                    
                                    );
}

/*****************************************************************************/
//  Description : 将字符串转换为bcd格式的电话号码
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void	MMIAPIPB_StringToBCDNumber(
                                uint8               *str_ptr,//IN 
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr//OUT                                                                          
                                )
{
    MMIPB_UTF8ToBCDNumber(str_ptr, bcd_number_ptr);
}

/*****************************************************************************/
//  Description : 获得联系人信息
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr)
{
    return MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
}

/*****************************************************************************/
//  Description : 普通联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIAPIPB_GetContactBaseInfo(uint16 index)
{
    return MMIPB_GetContactBaseInfo(index);
}


/*****************************************************************************/
//  Description : 获得空闲的entry_id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetFreeContactId		//RETURN: 空闲的entry_id
                                       (
                                        uint16   storage_id
                                        )
{
    return MMIPB_GetFreeContactId(storage_id);
}

#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : PIN code verify ok
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_Pin2VerifyCnf(void)
{
    MMIPB_Pin2VerifyCnf();
}
#endif
/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  void MMIAPIPB_SendSignal(uint32 sig_id)
{
    MMIPB_SendSignal(sig_id);
}

/*****************************************************************************/
//  Description : 获得手机中记录的总个数
//  Global resource dependence : g_mmipb_nv_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetPhoneContactCount(void)
{
    return MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
}
/*****************************************************************************/
//  Description : 获得SIM卡中号码的总个数
//  Global resource dependence : g_mmipb_entry_list
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetAllSimContactCount
(
    void
)
{
    uint16 count  = 0;
    MN_DUAL_SYS_E dual_sys =MN_DUAL_SYS_1;
    for(dual_sys= MN_DUAL_SYS_1 ; dual_sys < MN_DUAL_SYS_MAX; dual_sys++)
    {
        count+= MMIPB_GetSimContactCount(dual_sys);
    }
    return count;
    
}
/*****************************************************************************/
//  Description :   获得本机号码
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_ReadMsisdnContact(MN_DUAL_SYS_E dual_sys, 
                                          MMIPB_CONTACT_T *pb_msisdn_ptr)
{
    return MMIPB_GetPBMSISDN(dual_sys, pb_msisdn_ptr);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SwitchToPreTab(void)
{
    MMIPB_SwitchToPreTab();
}
#endif
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_IsUseDefaultName(wchar* name_ptr)
{
    return MMIPB_IsUseDefaultName(name_ptr);
}

/*****************************************************************************/
//  Description : 打开窗口，显示PB全部记录
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenListAllGroupWin(void)
{
    return MMIPB_OpenListAllGroupWin();
}

 #ifdef FDN_SUPPORT
/*****************************************************************************/
// 	Description : 取得sim卡特定号码列表的窗口数组
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetDailNumberListWinTab(void)
{
    return MMIPB_GetDailNumberListWinTab();
}
 #endif
#ifdef MMIPB_SIMDN_SUPPORT
/*****************************************************************************/
//  Description : 打开本机号码设置窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenMSISDNWin(BOOLEAN is_in_wizard, BOOLEAN is_enter_from_callsetting)
{
    return MMIPB_OpenMSISDNWin(PNULL,is_enter_from_callsetting);
}
#endif

/*****************************************************************************/
//  Description :获取MSISD的數目
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMSISDNum(void)
{
    return MMIPB_GetMSISDNum();
}

/*****************************************************************************/
//  Description :获取MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumberArray(MMIPB_BCD_NUMBER_T *number, int16 max_num)
{
    MMIPB_GetMSISDNumberArray(number, max_num);
}


/*****************************************************************************/
//  Description :获取指定SIM的MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num)
{
    MMIPB_GetMSISDNumberArrayBySim(number, dual_sys, max_num);
}
/*****************************************************************************/
//  Description : 判断电话号码是否有效
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPIPB_IsValidPhoneNumber
(
 uint8 npi_ton,
 uint8 number_length
 )
{
    return MMIPB_IsValidPhoneNumber(npi_ton, number_length);
}

/*****************************************************************************/
//  Description : 获得applet关联的选择列表的节点数,不区分group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIAPIPB_GetSelectCount(MMI_HANDLE_T handle)
{
    return MMIPB_GetSelectCount(handle);
}

/*****************************************************************************/
//  Description : 获得号码为普通形式时的最大长度
//  Global resource dependence : s_mmipb_max_len_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

uint8 MMIAPIPB_GetNumberNormalMaxLen
(
 void
 )
{   
    return (MMIPB_NUMBER_MAX_STRING_LEN);
}
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT
/*****************************************************************************/
//  Description : search sub string postion index in string
//  Global resource dependence : 
//  Author: 
//  Note:str_pos的空间必须是调用者分配的
//  Return:在str_pos返回根据PB所设定的查找算法，string2在string1所匹配的字符的位置数组
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_Wstrstrpos(const MMI_STRING_T * string1,//IN
								   const MMI_STRING_T * string2,//IN
								   MMI_STRING_T       * str_pos,//OUT
								   BOOLEAN             is_char//TRUE:字符查找；FALSE：数字查找
								   )
{
    MMI_STRING_T str_head_pos = {0};
    MMI_STRING_T lower_string = {0};
    wchar  letter_buf[40] = {0};
    wchar  head_letter_buf[2] = {0};
    uint16 i = 0;
    uint16 j = 0;
    uint16 head_cmp_len = 0;
    uint16 full_cmp_len = 0;
    uint16 full_letter_len = 0;
    BOOLEAN pre_is_punct = TRUE;
    BOOLEAN is_punct = FALSE;
    BOOLEAN result = FALSE;
    MMIPB_NAME_LETTERS_T *letters_str = PNULL;
    
    if(PNULL == string1 || string2 == PNULL || PNULL == string2->wstr_ptr || str_pos == PNULL)
    {
        //SCI_TRACE_LOW:"MMIAPIPB_Wstrstrpos pnull"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_EXPORT_1136_112_2_18_2_43_26_189,(uint8*)"");
        return FALSE;
    }

    letters_str = SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
    if(PNULL == letters_str)
    {
        return FALSE;
    }
    SCI_MEMSET(letters_str, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
    str_head_pos.wstr_ptr = SCI_ALLOCA(string1->wstr_len*sizeof(wchar));
    if(str_head_pos.wstr_ptr == PNULL)
    {
        SCI_FREE(letters_str);
        return FALSE;
    }
    lower_string.wstr_len =  string2->wstr_len;
    lower_string.wstr_ptr= SCI_ALLOCA((string2->wstr_len + 1)*sizeof(wchar));
    if(lower_string.wstr_ptr != PNULL)
    {
        SCI_MEMSET(lower_string.wstr_ptr, 0x00, (string2->wstr_len + 1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy(lower_string.wstr_ptr, string2->wstr_ptr, string2->wstr_len);
        MMIAPICOM_Wstrlower(lower_string.wstr_ptr);
        SCI_MEMSET(str_head_pos.wstr_ptr, 0x00, string1->wstr_len*sizeof(wchar));
        for(i = 0; i < string1->wstr_len; i++)
        {
            SCI_MEMSET(letter_buf, 0x00, sizeof(wchar)*40);
            MMIAPIIM_GetTextLetters(string1->wstr_ptr[i], letter_buf);
            full_letter_len = MMIAPICOM_Wstrlen(letter_buf);
            //set full letter
            if(is_char)
            {
                MMIAPICOM_Wstrncpy(&letters_str->full_letters[letters_str->full_letters_len], letter_buf, full_letter_len);
            }
            else
            {
                MMIIM_GetWcharKeyValueByTable(letter_buf, &letters_str->full_letters[letters_str->full_letters_len], FALSE);            
            }        
            //先从该字的头匹配
            if(MMIAPICOM_Wstrncmp(&letters_str->full_letters[letters_str->full_letters_len], 
                &lower_string.wstr_ptr[full_cmp_len], 
                MIN(lower_string.wstr_len - full_cmp_len, full_letter_len)))
            {
                //不匹配，重新开始
                full_cmp_len = 0;
                str_pos->wstr_len = 0;
            }
            else
            {
                full_cmp_len += MIN(lower_string.wstr_len - full_cmp_len, full_letter_len);            
                str_pos->wstr_ptr[str_pos->wstr_len] = i;
                str_pos->wstr_len++;
            }
            //字头匹配不成功，从中间匹配
            if(full_cmp_len == 0 && full_letter_len >1)
            {
                //可能从中间匹配
                for(j = 0; j < full_letter_len && full_cmp_len < lower_string.wstr_len; j++)
                {
                    if(letters_str->full_letters[letters_str->full_letters_len + j] == lower_string.wstr_ptr[full_cmp_len])   
                    {                    
                        full_cmp_len++;                
                    }
                    else
                    {
                        full_cmp_len = 0;                    
                        //break;
                    }
                }
                if(full_cmp_len > 0)
                {
                    str_pos->wstr_ptr[str_pos->wstr_len] = i;
                    str_pos->wstr_len++;
                }
                else
                {
                    //str_pos->wstr_ptr[str_pos->wstr_ptr] = i;
                    str_pos->wstr_len = 0;
                }
            }
            letters_str->full_letters_len += full_letter_len;
            //set head letter
            if(*letter_buf == 0x20 || MMIAPIIM_IsPunct(*letter_buf))
            {
                is_punct = TRUE;
            }
            else
            {
                is_punct = FALSE;
            }  
            if((!is_punct && pre_is_punct) //前一个是符号，这个是字符，可以作首字符            
                ||full_letter_len > 1)//这个是符合字，第一个字符可以为首字符
            {
                head_letter_buf[0] = MMIPB_GetHeadLetter(letter_buf);                    
                
                if(head_letter_buf[0] > 0)
                {
                    if(is_char)
                    {
                        letters_str->head_letters[letters_str->head_letters_len] = head_letter_buf[0];
                    }
                    else
                    {
                        MMIIM_GetWcharKeyValueByTable(letter_buf, &letters_str->head_letters[letters_str->head_letters_len], FALSE);                            
                    }
                    
                    if(letters_str->head_letters[letters_str->head_letters_len] == lower_string.wstr_ptr[head_cmp_len])   
                    {                    
                        head_cmp_len++;
                        str_head_pos.wstr_ptr[str_head_pos.wstr_len] = i;
                        str_head_pos.wstr_len++;
                    }
                    else
                    {
                        //不匹配，重新开始
                        head_cmp_len = 0;    
                        str_head_pos.wstr_len = 0;
                    }  
                    letters_str->head_letters_len++;
                }
            }
            pre_is_punct = is_punct; 
            //get text letters
            if(head_cmp_len >= lower_string.wstr_len || full_cmp_len >= lower_string.wstr_len)
            {
                //匹配字符匹配完成
                if(head_cmp_len >= lower_string.wstr_len)
                {
                    str_pos->wstr_len = str_head_pos.wstr_len;
                    SCI_MEMCPY(str_pos->wstr_ptr, str_head_pos.wstr_ptr, str_head_pos.wstr_len*sizeof(wchar));
                }
                result = TRUE;
                break;
            }        
    }
    }
    SCI_FREE(lower_string.wstr_ptr);
    SCI_FREE(letters_str);
    SCI_FREE(str_head_pos.wstr_ptr);
    return result;
}
#endif
/*****************************************************************************/
//  Description : 异步操作后的处理
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_CmdConfirm(                                                           
                              MMIPB_ERROR_E  result
                              )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};
    MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
    return MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), result);
}

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//+++ TC_AG_OCM_BV_01_I
PUBLIC BOOLEAN MMIAPIPB_GetNumberByIndex(uint16 index, uint8* pstrbuf)
{
    BOOLEAN bRet = FALSE;
    MMIPB_CONTACT_BASE_INFO_T base_info = MMIPB_GetContactBaseInfo(index);
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    int32 phone_num_index = 0;

    if(pstrbuf != PNULL)
    {
        if(base_info.contact_id > 0 && base_info.storage_id > 0)
        {
            contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                if(MMIPB_ERROR_SUCCESS == MMIAPIPB_GetContactInfo(base_info.contact_id, base_info.storage_id, contact_ptr))
                {
                    phone_num_index = MMIAPIPB_GetFirstValidNumberFromNumArray(contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM);
                    if(phone_num_index >= 0 && phone_num_index < MMIPB_MAX_NV_PHONE_NUM)

                    {
                        MMIPB_BCDNumberToUTF8(&contact_ptr->number[phone_num_index], MMIPB_MAX_STRING_LEN, pstrbuf);
                        bRet = TRUE;
                    }
                }
                SCI_FREE(contact_ptr);
            }
        }
    }
    return bRet;
}
//--- TC_AG_OCM_BV_01_I
#endif

#ifdef SEARCH_SUPPORT
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_UpdatePbSearching(void)
{
    MMIPB_UpdatePbSearching();
}

/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_LocalAsyncSearch(const MMI_STRING_T *key_word_str)
{
    MMIPB_LocalAsyncSearch(key_word_str);
}
/*****************************************************************************/
//     Description : stop async search pb
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPbAsyncSearch(void)
{
    MMIPB_StopPbAsyncSearch();
}
/*****************************************************************************/
//     Description : is pb search end
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsPbAsyncSearchEnd(void)
{
    return MMIPB_IsPbAsyncSearchEnd();
}
#else
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num)
{
    MMIPB_LocalSearch(key_word_str, PNULL, match_data_ptr, max_num);
}
#endif

/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_FreeLocalSearchResource(void)
{
    MMIPB_FreeLocalSearchResource();
}
#endif

/*****************************************************************************/
// Description : 返回最大可以存储的contacts的数目
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMaxContactNum(void)
{
    return MMIPB_GetMaxContactNum();
}

/*****************************************************************************/
//  Description : THE INTERFACE for autotest only
//  Global resource dependence : none
//  Author:maryxiao
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_FillPhoneContactFull(void)
{
    return MMIPB_FillPhoneContactFull();
}

#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr)
{
    return MMIPB_GetcontactIdByUid(uid, entry_id_ptr, storage_id_ptr);
}


/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid)
{
    return MMIPB_GetUidByContactId(entry_id, storage_id, uid);
}
#endif

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_IsTCardContactExist(void)
{
    return MMIPB_IsTCardContactExist();
}
#endif
/*****************************************************************************/
//  Description : get storage free count
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetStorageFreeSpace(uint16 storage_id, uint16 *free_num_ptr)
{
    return MMIPB_GetStorageFreeSpace(storage_id, free_num_ptr);
}

/*****************************************************************************/
//  Description : Get pb num forn nv
//  Global resource dependence : none
//  Author:
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMIAPIPB_GetPBRecordNumFromNV(void)
{
    MMIPB_STORAGE_INFO_T storage_info =  MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));

    return storage_info.used_record_num;
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : judge whether export contact isnot
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsExportContactOpc()
{
    return MMIPB_IsExportContactOpc();
}
#endif

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :联系人同步，初始化pbap数据列表
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_InitPBAPDataList(void)
{
	return MMIPB_InitPBAPDataList();
}
/*******************************************************************************/
//  Description :联系人同步，初始化pbap数据列表
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_InsertPBAPDataToList(void* buff,uint16 buff_len)
{
	return MMIPB_InsertPBAPDataToList(buff,buff_len);
}
/*******************************************************************************/
//  Description :get free space
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetPhoneStorageFreeSpace(void)
{
     return MMIPB_GetStorageFreeSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT), PNULL);
}
/*******************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPIPB_NotifyPBStartParser(MMIPB_PBAP_PARSER_CALLBACK_FUNC callback)
{
    MMIPB_NotifyPBStartParser(callback);
}
/*****************************************************************************/
//  Description : 清除全部蓝牙联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteAllPbapSyncData(void)
{
    MMIPB_ERROR_E   ret = MMIPB_ERROR_ERROR;
    MMIAPIPB_InitPBAPDataList();
    ret = MMIPB_ClearAllBtContactForPbap();
    
    SCI_TRACE_LOW("MMIAPIPB_DeleteAllPbapSyncData ret %d", ret);
    return ret;
}
/*****************************************************************************/
//  Description : 清除连线timer
//  Global resource dependence : 
//  Author:allen.lee
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPBAPTimer(void)
{
    MMIPB_StopPBAPTimer();
}
#endif