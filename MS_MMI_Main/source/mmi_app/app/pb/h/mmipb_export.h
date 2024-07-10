/*****************************************************************************
** File Name:      mmipb_export.h                                                   *
** Author:                                                                   *
** Date:           03/09/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        Tracy Zhang      Create
******************************************************************************/

#ifndef _MMIPB_EXPORT_H_
#define _MMIPB_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "mmipb_mnapi.h"
//#include "mmk_type.h"
#ifdef MMI_OCR_ENABLE
#include "mmiocr_export.h"
#endif
#include "mmi_atc.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmipb_datalist.h"
#include "mmipb_interface.h"

#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif

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
// 字符串的最大长度
//#define MMIPB_MAX_STRING_LEN		(/*lint -save -e506 */MAX((MMIPB_NAME_MAX_LEN << 1), 100) /*lint -restore */)
#define MMIPB_MAX_FAVOURITE_NUM      20

#define PB_PROTRAIT_FATHER                1
#define PB_PROTRAIT_MOTHER                2
#define PB_PROTRAIT_GRANDPA               3
#define PB_PROTRAIT_GRANDMA               4
#define PB_PROTRAIT_TEACHER               5
#define PB_PROTRAIT_CLASSMATE             6
#ifdef SCREEN_SHAPE_CIRCULAR
#define PB_PROTRAIT_SISTER                7
#define PB_PROTRAIT_BROTHER               8
#define PB_PROTRAIT_STRANGER              9
#define PB_PROTRAIT_EMERGRNCY             10
#else
#define PB_PROTRAIT_STRANGER              7
#define PB_PROTRAIT_EMERGRNCY             8
#endif
#define WATCHPB_CONTACT_NUM_MAX             10
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//PB Table包含的应用类型
typedef enum
{
    MMIPB_ALL_CONTACT,
#ifdef MMIPB_MOST_USED_SUPPORT
    MMIPB_MOSTUSED_CONTACT,
#endif
#ifdef BT_PBAP_SUPPORT
    MMIPB_BT_CONTACT,//蓝牙联系人
#endif
    MMIPB_GROUP_CONTACT,
#ifdef MMI_PDA_SUPPORT
    MMIPB_DIAL_APPLICATION,
    MMIPB_CALLLOG_APPLICATION,
#endif
    MMIPB_TAB_MAX_NUM
}MMIPB_SUB_FUNCTION_TYPE_E;

//联系人各个域的显示方式
typedef enum
{
    MMIPB_LIST_FIELD_WHOLE= 0x0,//联系人整体显示，即一个联系人一个记录
    MMIPB_LIST_FIELD_NUMBER= 0x1,//号码分开显示，即一个号码一个记录
    MMIPB_LIST_FIELD_MAIL =0x2 //Email分开显示，即一个Email一个记录
} MMIPB_LIST_FIELD_TYPE_E;


// 记录列表的类型
typedef enum _MMIPB_LIST_TYPE_E
{
    MMIPB_LIST_NORMAL_MAIN  = 0x0, //常规主列表，相当于PB主界面，有option
    MMIPB_LIST_RADIO        = 0x1, //单选列表
    MMIPB_LIST_CHECK        = 0x2, //复选列表
} MMIPB_LIST_TYPE_E;

typedef enum _MMIPB_ADD_TYPE_T
{
    MMIPB_ADD_TYPE_MSISDN,
    MMIPB_ADD_TYPE_NUMBER,
    MMIPB_ADD_TYPE_EMAIL,
    MMIPB_ADD_TYPE_URL,
    MMIPB_ADD_TYPE_PIC,

}MMIPB_ADD_TYPE_T;

// 字符串
typedef struct _MMIPB_STRING_T
{
	uint16	strlen;
	uint8	strbuf[MMIPB_MAX_STRING_LEN +2];
} MMIPB_STRING_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化Phonebook的全局变量、回调函数等
//  Global resource dependence : g_mmipb_sim_t, g_mmipb_nv_t, g_mmipb_app
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitApp 
(
	void
);

/*****************************************************************************/
// 	Description : register pb nv and menu
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitModule(void);

/*****************************************************************************/
//  Description : Resort Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: 这个命令会引起pb的立即重新排序，如果sim都ready，不需要等CALL_READY和SIM_NOT_READY
/*****************************************************************************/
PUBLIC void MMIAPIPB_ResortPB(void);

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : reset pb group info
//  Global resource dependence : s_mmipb_group_info
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_ResetPBGroupInfo(void);
#endif
/*****************************************************************************/
//  Description : re_initialize Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: 这个命令会等ALL_READY和SIM_NOT_READY后再进行重排
//  [特别提示Warnning]:调用这个重排接口，一定要保证sim卡会重新发ALL_READY和SIM_NOT_READY，即sim会重新初始化
/*****************************************************************************/
PUBLIC void MMIAPIPB_ReinitPB(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : MMIAPIPB_InitSimStatus
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitSimStatus(MN_DUAL_SYS_E dual_sys);


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
                        );
#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : FDN列表窗口
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenFDNContactListWin(MN_DUAL_SYS_E dual_sys);
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
                            );

/*****************************************************************************/
//  Description : 创建handle
//  Global resource dependence : none
//  Author:baokun.yin 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_CreateHandle(void);

/*****************************************************************************/
//  Description : 销毁handle
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_CloseHandle(
                                 MMIPB_HANDLE_T handle
                                 );

/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: OUT: contact_ptr
//        OUT: num_index
//        IN: handle
//        IN: index
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T * contact_ptr, uint16 index, uint16 * num_index);
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人姓名。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index);
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人email。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index);
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人号码。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectNumber(MMIPB_HANDLE_T handle, MMIPB_BCD_NUMBER_T *nameber_ptr, uint16 max_number_len,  uint16 index);

/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人vcard。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectVcard(MMIPB_HANDLE_T handle, uint8  *data_ptr, uint16 max_buf_len,  uint16 index);

/*****************************************************************************/
//  Description : 异步新增联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);

/*****************************************************************************/
//  Description : 异步 编辑联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 导出联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ExportVcard(uint16 entry_id, uint16 storage_id, const wchar *path_name_ptr);

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
                                    );


/*****************************************************************************/
// Description : 异步导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ImportVcardAsyn(MMIPB_HANDLE_T handle, //IN
                                              const wchar *path_name_ptr,//IN
                                              MMIPB_ACCESS_CALLBACK_FUNC callback
                                              );
#endif

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
                                                );
/*****************************************************************************/
//  Description : 复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_CopyContact(uint16 to_storage_id, uint16 from_entry_id, uint16 from_storage_id);
/*****************************************************************************/
//  Description : 异步复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:提供给外部接口调用, 暂时屏蔽该接口！！！
/*****************************************************************************/
// PUBLIC MMIPB_ERROR_E MMIAPIPB_CopyContactAsyn(
//                                               MMIPB_HANDLE_T handle, 
//                                               uint16 dest_storage_id, 
//                                               uint16 src_contact_id, 
//                                               uint16 src_storage_id, 
//                                               MMIPB_ACCESS_CALLBACK_FUNC callback
//                                               );

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :写群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id);
#endif

/*****************************************************************************/
//  Description : ADD  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContact(MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : delete  CONTACT
//  Global resource dependence : none
//  Author: 
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteContact(uint16 entry_id, uint16 storage_id);
/*****************************************************************************/
//  Description : MODIFY  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContact(MMIPB_CONTACT_T *contact_ptr);

/*****************************************************************************/
//  Description : 获取该设备的storage id
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetStorageId(
                                    MN_DUAL_SYS_E dual_sys,
                                    PHONEBOOK_STORAGE_E storage
                                    );

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
                                    );

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
                                    );
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
								 BOOLEAN is_use_default_name);

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
);

/*****************************************************************************/
//  Description : 打开添加菜单的窗口。
//  Global resource dependence : g_mmipb_operate_type, g_mmipb_bcd_number_t
//  Author: Tracy.zhang
//  Note: 此函数供短消息和彩信提取号码并保存的时候调用；
//        供通话记录号码保存到电话簿中调用；供本模块调用。
//        供其它模块调用时，需要传入号码；供本模块调用时，不用传号码。
/*****************************************************************************/
PUBLIC void MMIAPIPB_AddContactWin
(
    void	*data_ptr,	    //IN: 输入的电话号码
    uint8                               group_id,                  //In:group id,
    MMIPB_ADD_TYPE_T  type
    
);
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
);
#endif
/*****************************************************************************/
//  Description : 获得电话簿是否可以被其它模块调用
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPIPB_IsPbReady(void); //RETURN: TRUE, 可以使用

/*****************************************************************************/
// 	Description : 打开窗口，显示PB全部记录
//	Global resource dependence : 
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenListAllGroupWin(void);

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
						);

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
						);

/*****************************************************************************/
//  Description : set contact ring to defaul
//  Global resource dependence : none
//  Author:
//  Note:	
/*****************************************************************************/
PUBLIC void MMIAPIPB_ClearContactRing(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr
						);
#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : get asp path name or fixed id
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
						);
#endif
/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_ReadPhotoFileName(uint16 entry_id, 
                                       uint16 storage_id, 
                                       MMIPB_CUSTOM_PHOTO_T *photo_ptr);


/*****************************************************************************/
//  Description : Refresh SIM卡中的Phone book，修改全局变量的信息
//  Global resource dependence : g_mmipb_sim_t
//  Author: Kelly.li
//  Note: 
/*****************************************************************************/
// PUBLIC void MMIAPIPB_RefreshSimList (void);

/*****************************************************************************/
//  Description : set all Sim  to not initialize status
//  Global resource dependence : none
//  PARAMETER: 
//  Return：
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIPB_SetAllSIMNotInit(void);
#ifdef MMIPB_SIMDN_SUPPORT
/*****************************************************************************/
//  Description : 打开本机号码设置窗口
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenMSISDNWin(BOOLEAN is_in_wizard, BOOLEAN is_enter_from_callsetting);
#endif
#ifdef MMI_OCR_ENABLE
/*****************************************************************************/
//  Description : 添加联系人信息到电话簿
//  Global resource dependence : g_mmipb_nv_entry_t
//  Author: Annie.an
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_AddContactToPhonebook(MMIOCR_INFO_ENTRY_T *ocr_info);
#endif

/*****************************************************************************/
//  Description :	打断PB
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_AbortPB(void);

/*****************************************************************************/
//     Description : get sum size of all pb nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetSumNvSize(void);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 解析VCard data,并打开该文件
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_OpenVcardContactWin(uint8  *data_ptr, uint32 data_size);

/*****************************************************************************/
// Description : 解析VCard data,并打开该文件
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_OpenVcardFileContactWin(uint16  *file_name_ptr);
#endif
/*****************************************************************************/
//  Description : 判断电话号码是否有效
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN MMIAPIPB_IsValidPhoneNumber
(
 uint8   npi_ton,
 uint8   number_length
 );

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
                                    );

/*****************************************************************************/
//  Description : 将字符串转换为bcd格式的电话号码
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void	MMIAPIPB_StringToBCDNumber(
                                uint8               *str_ptr,//IN 
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr//OUT                                                                          
                                );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_IsUseDefaultName(wchar* name_ptr);

//#ifndef PB_SEARCH_SURPPORT_NONE
/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录和姓名中按照首字母匹配的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 操作句柄，这个句柄使用完成后要调用MMIAPIPB_SearchExit释放
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_SearchList(MMI_STRING_T *search_str,
                                  MMIPB_LIST_FIELD_TYPE_E disp_field, 
                                  uint16 *count_ptr);

/*****************************************************************************/
//  Description : 外部搜索退出，当外部搜索退出是，这个接口一定要调用
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SearchExit(MMIPB_HANDLE_T handle);

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
                                       uint32 index);

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
                                       uint16 *storage_id_ptr);									  

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的小图标
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetSearchEntryIcon(uint32 index);

#ifdef MMIPB_MOST_USED_SUPPORT
//#endif
/*****************************************************************************/
//  Description : 设置电话号码的被使用

//  Note: 更新该号码的使用频率。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_SetNumberUsed(uint8 *num_str, uint16 number_len);
#endif
/*****************************************************************************/
//  Description : update FDN list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_UpdateSIMDNList(void);

/*****************************************************************************/
//Description : compare bcd number
// Return: 0: number1 = number2; 1: number1 > number2; -1:number1 < number2
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BCDCmp(
							 const MMIPB_BCD_NUMBER_T * number1,
                             const MMIPB_BCD_NUMBER_T * number2
 );

/*****************************************************************************/
//  Description : 电话号码的职能匹配
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:两个号码能够职能匹配，其他不能匹配
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, //IN:
								  const MMIPB_BCD_NUMBER_T *bcd_number2//IN
								  );

/*****************************************************************************/
//  Description : 按照UCS2字符串的字符序列进行比较
//  Global resource dependence : none
//  Author: 
//  Note: 例如 “百”和"不"，是按照"bai"和"bu"进行比较的
/*****************************************************************************/
PUBLIC int MMIAPIPB_CmpStrByLetters(
										   wchar   *ucs2_string_ptr1,        //IN:
										   uint8   wstr1_len,            //IN:
										   wchar   *ucs2_string_ptr2,        //IN:
										   uint8   wstr2_len            //IN:

										   );
#ifdef MMIWIDGET_SUPPORT

/*****************************************************************************/
//  Description : Get First Valid Number index from number array number_t_ptr
//  Global resource dependence : none
//  Author: xport
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_GetFirstValidNumberFromNumArray(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count);
#endif

/*****************************************************************************/
// Description : open one contact details by number.
// Global resource dependence :
// Author:baokun.yin
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenNumberContactWin(MMIPB_BCD_NUMBER_T *number_ptr);

/*****************************************************************************/
// Description : open one favorite entry called only by widget.
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_OpenContactWin(uint16 entry_id, uint16 storage_id);													

/*****************************************************************************/
//  Description : delete all pb info 
//  Author: 
//  Note: for dm cmcc 
/*****************************************************************************/
PUBLIC void MMIAPIPB_DeleteAllPBInfo(void);


/*****************************************************************************/
//  Description : 获得空闲的entry_id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetFreeContactId		//RETURN: 空闲的entry_id
                                       (
                                        uint16   storage_id
                                        );


#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : PIN code verify ok
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_Pin2VerifyCnf(void);
#endif


/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  void MMIAPIPB_SendSignal(uint32 sig_id);

/*****************************************************************************/
//  Description : 获得手机中记录的总个数
//  Global resource dependence : g_mmipb_nv_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetPhoneContactCount(void);

/*****************************************************************************/
//  Description : 获得SIM卡中号码的总个数
//  Global resource dependence : g_mmipb_entry_list
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetAllSimContactCount
(
    void
);


/*****************************************************************************/
//  Description : 将姓名转化为字符串
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_ConvertNameToString
(
    MMI_STRING_T    	*str_ptr,   //OUT:
    const MMIPB_NAME_T	*name_ptr   //IN:
);


/*****************************************************************************/
//  Description : 获得存储器对应的电话簿所能保存的最大记录数
//  Global resource dependence : g_mmipb_entry_list, g_mmipb_nv_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetStorageMaxCount(uint16 storage_id);   //RETURN: 最大记录数
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
								   BOOLEAN            is_char//TRUE:字符查找；FALSE：数字查找
								   );
#endif
/*****************************************************************************/
//  Description : 获得applet关联的选择列表的节点数,不区分group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIAPIPB_GetSelectCount(MMI_HANDLE_T handle);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SwitchToPreTab(void);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SetCLUnProtect(BOOLEAN is_unprotect);

/*****************************************************************************/
//  Description : 异步操作后的处理
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_CmdConfirm(                                                           
                              MMIPB_ERROR_E  result
                              );

/*****************************************************************************/
//  Description : 获得联系人信息
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr);

/*****************************************************************************/
//  Description :   获得本机号码
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_ReadMsisdnContact(MN_DUAL_SYS_E dual_sys, 
            MMIPB_CONTACT_T *pb_msisdn_ptr);

/*****************************************************************************/
//  Description :读取群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ReadGroupName(MMIPB_NAME_T* name,  uint8 group_id);

/*****************************************************************************/
//  Description : 普通联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIAPIPB_GetContactBaseInfo(uint16 index);

/*****************************************************************************/
//  Description : 获得号码为普通形式时的最大长度
//  Global resource dependence : s_mmipb_max_len_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

uint8 MMIAPIPB_GetNumberNormalMaxLen
(
 void
 );
#ifdef  FDN_SUPPORT
/*****************************************************************************/
// 	Description : 取得sim卡特定号码列表的窗口数组
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetDailNumberListWinTab(void);
#endif
/*****************************************************************************/
//  Description :获取MSISD的數目
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMSISDNum(void);

/*****************************************************************************/
//  Description :获取指定SIM的MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num);

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//mdy by qing.yu@for BQB
/*****************************************************************************/
//  Description : get pb my phone number
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumber(MMIPB_BCD_NUMBER_T *number);

//+++ TC_AG_OCM_BV_01_I
PUBLIC BOOLEAN MMIAPIPB_GetNumberByIndex(uint16 index, uint8* pstrbuf);
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
PUBLIC void MMIAPIPB_UpdatePbSearching(void);

/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_LocalAsyncSearch(const MMI_STRING_T *key_word_str);

/*****************************************************************************/
//     Description : stop async search pb
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPbAsyncSearch(void);
/*****************************************************************************/
//     Description : is pb search end
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsPbAsyncSearchEnd(void);
#else
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num);
#endif

/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_FreeLocalSearchResource(void);
#endif
/*****************************************************************************/
// Description : 返回最大可以存储的contacts的数目
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMaxContactNum(void);

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
								 BOOLEAN is_use_default_name);


/*****************************************************************************/
//  Description : THE INTERFACE for autotest only
//  Global resource dependence : none
//  Author:maryxiao
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_FillPhoneContactFull(void);
#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr);

/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid);
#endif

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_IsTCardContactExist(void);
#endif
/*****************************************************************************/
//  Description : get storage free count
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetStorageFreeSpace(uint16 storage_id, uint16 *free_num_ptr);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : judge whether export contact isnot
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsExportContactOpc();
#endif

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :联系人同步，添加至通讯录
//  Parameter: contact_ptr[in]: 记录相关信息
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIBT_AddContact(MMIPB_CONTACT_T *contact_ptr);
/*******************************************************************************/
//  Description :get free space
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetPhoneStorageFreeSpace(void);
/*******************************************************************************/
//  Description :联系人同步，初始化pbap数据列表
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_InitPBAPDataList(void);
/*******************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPIPB_NotifyPBStartParser(MMIPB_PBAP_PARSER_CALLBACK_FUNC callback);
/*******************************************************************************/
//  Description :联系人同步，初始化pbap数据列表
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_InsertPBAPDataToList(void* buff,uint16 buff_len);
/*****************************************************************************/
//  Description :  清除全部蓝牙联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteAllPbapSyncData(void);
/*****************************************************************************/
//  Description : 清除连线timer
//  Global resource dependence : 
//  Author:allen.lee
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPBAPTimer(void);
/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_PbapClientDeregister(void);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/*****************************************************************************/
//  Description : get imageId by flag
//  Parameter: [In] photo_id_flag & in_call
//             [Out] None
//             [Return] imageId
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 WatchPB_GetImageIdByFlag(uint8 photo_id_flag, BOOLEAN in_call);

/*****************************************************************************/
//  Description : get name(or number) and imageId for display
//  Parameter: [In] tele_num & tele_len
//             [Out] (wchar)in_str
//             [Return] image number
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC uint8 WatchPB_GetDispInfoContent(uint8 *tele_num, uint8 tele_len, wchar *in_str, BOOLEAN is_mo_call);

/*****************************************************************************/
//  Description : get contact info by bcd number
//  Parameter: [In] BCD_num
//             [Out] contact info
//             [Return] TRUE or FALSE
//  Author: QI.LIU1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WatchPB_GetContactInfoByBcd(MMIPB_BCD_NUMBER_T *BCD_num, MMIPB_CONTACT_T *contact_ptr);
#endif
